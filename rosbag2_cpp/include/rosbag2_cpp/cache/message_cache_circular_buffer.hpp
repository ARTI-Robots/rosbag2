// Copyright 2021 Amazonhe t.com, Inc. or its affiliates. All Rights Reserved.
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

#ifndef ROSBAG2_CPP__CACHE__MESSAGE_CACHE_CIRCULAR_BUFFER_HPP_
#define ROSBAG2_CPP__CACHE__MESSAGE_CACHE_CIRCULAR_BUFFER_HPP_

#include <deque>
#include <memory>
#include <vector>

#include "rosbag2_cpp/visibility_control.hpp"
#include "rosbag2_cpp/cache/cache_buffer_interface.hpp"
#include "rosbag2_storage/serialized_bag_message.hpp"

// This is necessary because of using stl types here. It is completely safe, because
// a) the member is not accessible from the outside
// b) there are no inline functions.
#ifdef _WIN32
# pragma warning(push)
# pragma warning(disable:4251)
#endif

namespace rosbag2_cpp
{
namespace cache
{

/// This class implements a circular buffer message cache. Since the buffer
/// size is limited by the total byte size of the storage messages or a total messages duration
/// rather than a fix number of messages, a deque is used instead of a vector since
/// older messages can always be dropped from the front and new messages added
/// to the end. The buffer will never consume more than max_cache_size bytes, if max_cache_size > 0.
/// And will log a warning message if an individual message exceeds the buffer size.
class ROSBAG2_CPP_PUBLIC MessageCacheCircularBuffer
  : public CacheBufferInterface
{
public:
  // Delete default constructor since max_cache_size is required
  MessageCacheCircularBuffer() = delete;

  /// \brief Parametrized constructor
  /// \param max_cache_size Maximum amount of memory which could be occupied by the messages stored
  /// in the circular buffer. Note. If max_cache_size is zero, the circular buffer will be only
  /// bounded by the max_cache_duration.
  /// \param max_cache_duration_ns Maximum duration in nanoseconds of message sequence allowed to be
  /// stored in the circular buffer. Note. If max_cache_duration is zero, the circular buffer will
  /// be only bounded by the max_cache_size.
  /// \throws std::invalid_argument if both max_cache_size and max_cache_duration are zero.
  explicit MessageCacheCircularBuffer(size_t max_cache_size, int64_t max_cache_duration_ns = 0);

  /// \brief Push new message to the circular buffer
  /// \details If buffer size has some space left, we push the message regardless of its size,
  /// but if this results in exceeding buffer size, we begin dropping old messages.
  /// \param msg Shared pointer to the rosbag2_storage::SerializedBagMessage
  /// \return False if the buffer_bytes_size > 0 and
  /// msg->serialized_data->buffer_length > max_bytes_size, otherwise true.
  bool push(CacheBufferInterface::buffer_element_t msg) override;

  /// Clear buffer
  void clear() override;

  /// Get number of elements in the buffer
  size_t size() override;

  /// Get buffer data
  const std::vector<CacheBufferInterface::buffer_element_t> & data() override;

private:
  std::deque<CacheBufferInterface::buffer_element_t> buffer_;
  std::vector<CacheBufferInterface::buffer_element_t> msg_vector_;
  size_t buffer_bytes_size_ {0u};
  const size_t max_bytes_size_;
  const int64_t max_cache_duration_;
};

}  // namespace cache
}  // namespace rosbag2_cpp

#ifdef _WIN32
# pragma warning(pop)
#endif

#endif  // ROSBAG2_CPP__CACHE__MESSAGE_CACHE_CIRCULAR_BUFFER_HPP_
