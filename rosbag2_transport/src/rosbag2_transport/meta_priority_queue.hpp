// Copyright 2024 Apex.AI, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef ROSBAG2_TRANSPORT__META_PRIORITY_QUEUE_HPP_
#define ROSBAG2_TRANSPORT__META_PRIORITY_QUEUE_HPP_

#include <atomic>
#include <limits>
#include <mutex>
#include <utility>
#include <vector>

#include "moodycamel/readerwriterqueue.h"

#include "rcpputils/thread_safety_annotations.hpp"

/// Priority queue of queues.
/**
 * Contains N queues.
 * Elements can be enqueued into a specific queue.
 * Assumes that items are enqueued into the same queue in order of priority.
 * The priority value of an element is computed from a provided function.
 * Highest priority means a lower priority value.
 * Returns the next element from the highest-priority queue, which is determined by the queue with
 * the highest-priority first element, since the first element of each queue is the highest-priority
 * element of that queue.
 *
 * This does impose some more restrictions on concurrency compared to the underlying queue
 * implementation. However, if there is only 1 underlying queue, then there is no locking.
 *
 * \tparam T the element type
 * \tparam P the priority value type
 * \see moodycamel::ReaderWriterQueue for the underlying queue
 */
template<typename T, typename P>
class MetaPriorityQueue
{
public:
  /**
   * Constructor.
   *
   * \param num_queues the number of priority queues, greather than zero
   * \param element_priority_value a function to extract the priority value from an element
   */
  MetaPriorityQueue(
    const std::size_t num_queues,
    const std::function<P(const T &)> && element_priority_value)
  : queues_(num_queues),
    element_priority_value_(std::move(element_priority_value)),
    queue_priorities_(num_queues),
    // When all queues are empty, the index value doesn't matter
    priority_queue_index_(0u)
  {
    assert(num_queues > 0u);
    // Set all queue priority values to the lowest priority, i.e., highest value
    for (std::size_t i = 0; i < queues_.size(); i++) {
      queue_priorities_[i] = lowest_priority_value;
    }
  }

  /// Enqueue element into a given queue.
  /**
   * Should only be called by 1 producer thread.
   * Elements must be enqueued into the same queue in order of priority for elements to be
   * peeked/popped in order of priority.
   * No bounds checking is performed for the queue index.
   *
   * \param element the element to enqueue
   * \param queue_index the queue to enqueue it into
   * \return `true` if the element was enqueued, `false` otherwise
   * \see moodycamel::ReaderWriterQueue::enqueue
   */
  bool enqueue(const T & element, const std::size_t queue_index)
  {
    // Trivial case: 1 underlying queue
    if (1u == queues_.size()) {
      return queues_[queue_index].enqueue(element);
    }
    std::lock_guard<std::mutex> lk(mutex_priority_);
    const bool enqueued = queues_[queue_index].enqueue(element);
    // If this queue was empty, update the queue's priority value with the new element
    // Otherwise, it's not necessary, since the new element is supposed to be lower-priority
    if (enqueued && 1u == size_approx(queue_index)) {
      update_priorities(&element, queue_index);
    }
    return enqueued;
  }

  /// Peek at the next item from the highest-priority queue.
  /**
   * Should only be called by 1 consumer thread.
   *
   * \return the next item from the highest-priority queue, or `nullptr` if there is none
   * \see moodycamel::ReaderWriterQueue::peek
   */
  T * peek()
  {
    return queues_[priority_queue_index_.load()].peek();
  }

  /// Pop the next item from the highest-priority queue.
  /**
   * Should only be called by 1 consumer thread.
   *
   * \return `true` if there was an item, `false` otherwise
   * \see moodycamel::ReaderWriterQueue::pop
   */
  bool pop()
  {
    // Trivial case: 1 underlying queue
    if (1u == queues_.size()) {
      return queues_[0u].pop();
    }
    std::lock_guard<std::mutex> lk(mutex_priority_);
    const std::size_t priority_index = priority_queue_index_.load();
    const bool popped = queues_[priority_index].pop();
    // If we pop()ed, update priority
    if (popped) {
      // peek() and pop() should only be called by the 1 consumer thread, so this is OK
      const T * element = queues_[priority_index].peek();
      update_priorities(element, priority_index);
    }
    return popped;
  }

  /// Get the approximate size of the given queue.
  /**
   * Can be called by both the 1 producer thread and the 1 consumer thread.
   * No bounds checking is performed for the queue index.
   *
   * \param queue_index the index of the queue
   * \return the approximate size
   * \see moodycamel::ReaderWriterQueue::size_approx
   */
  inline std::size_t size_approx(const std::size_t queue_index) const
  {
    return queues_[queue_index].size_approx();
  }

  /// Get the approximate total size of all queues.
  /**
   * Can be called by both the 1 producer thread and the 1 consumer thread.
   * The size of the underlying queues may change during this call.
   *
   * \return the approximate total size
   * \see ::size_approx
   */
  inline std::size_t size_approx() const
  {
    std::size_t size = 0;
    for (std::size_t i = 0; i < queues_.size(); i++) {
      size += size_approx(i);
    }
    return size;
  }

private:
  /// Update the queue priority values and the priority queue index.
  /**
   * Update when:
   * 1. Enqueuing an element into a queue that was empty
   * 2. Popping an element from a queue
   *
   * \param element the new element (newly-enqueued or newly-front of queue), or `nullptr` if the
   *   queue is now empty
   * \param queue_index the index of the corresponding queue
   */
  void update_priorities(const T * element, const std::size_t queue_index)
  RCPPUTILS_TSA_REQUIRES(priority_queue_index_)
  {
    // Update priority value for the given queue
    if (nullptr != element) {
      const P priority_value = element_priority_value_(*element);
      queue_priorities_[queue_index].store(priority_value);
    } else {
      // Reset priority value if the queue is now empty
      queue_priorities_[queue_index].store(lowest_priority_value);
    }

    // Find the index of the highest-priority queue, i.e., to be used next
    // Notes:
    // 1. If multiple queues have the same priority value, this does favor the first queue
    // 2. If all queues are empty, this will return the index of the first queue, and
    //    popping/peeking will just return nothing
    std::size_t priority_queue_index = 0;
    for (std::size_t i = 1; i < queues_.size(); i++) {
      if (queue_priorities_[i] < queue_priorities_[priority_queue_index].load()) {
        priority_queue_index = i;
      }
    }
    priority_queue_index_.store(priority_queue_index);
  }

  // Underlying queues
  std::vector<moodycamel::ReaderWriterQueue<T>> queues_;
  // Element priority value extraction function
  const std::function<P(const T &)> element_priority_value_;
  // Priority values of each queue
  std::vector<std::atomic<P>> queue_priorities_;
  // Index of the current highest-priority queue
  std::atomic_size_t priority_queue_index_;
  // Mutex for priority queue tracking
  std::mutex mutex_priority_;

  /// Priority value corresponding to the lowest priority.
  static constexpr P lowest_priority_value = std::numeric_limits<P>::max();
};

#endif  // ROSBAG2_TRANSPORT__META_PRIORITY_QUEUE_HPP_
