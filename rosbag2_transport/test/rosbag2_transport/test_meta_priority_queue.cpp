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

#include <gtest/gtest.h>

#include "meta_priority_queue.hpp"

TEST(meta_priority_queue, test_queue_trivial)
{
  MetaPriorityQueue<int, int> queue(1u, [](const int & v){return v;});

  EXPECT_TRUE(queue.enqueue(1, 0u));
  EXPECT_TRUE(queue.enqueue(2, 0u));
  EXPECT_TRUE(queue.enqueue(3, 0u));
  EXPECT_TRUE(queue.enqueue(4, 0u));

  EXPECT_EQ(1, *queue.peek());
  EXPECT_TRUE(queue.pop());
  EXPECT_EQ(2, *queue.peek());
  EXPECT_TRUE(queue.pop());
  EXPECT_EQ(3, *queue.peek());
  EXPECT_TRUE(queue.pop());
  EXPECT_EQ(4, *queue.peek());
  EXPECT_TRUE(queue.pop());
}

TEST(meta_priority_queue, test_queue_multi)
{
  MetaPriorityQueue<int, int> queue(3u, [](const int & v){return v;});

  // Empty
  EXPECT_EQ(nullptr, queue.peek());
  EXPECT_FALSE(queue.pop());
  EXPECT_EQ(nullptr, queue.peek());
  EXPECT_EQ(0u, queue.size_approx());
  EXPECT_EQ(0u, queue.size_approx(0u));
  EXPECT_EQ(0u, queue.size_approx(1u));
  EXPECT_EQ(0u, queue.size_approx(2u));

  // First new element
  EXPECT_TRUE(queue.enqueue(2, 1u));
  EXPECT_EQ(2, *queue.peek());
  EXPECT_EQ(1u, queue.size_approx());
  EXPECT_EQ(0u, queue.size_approx(0u));
  EXPECT_EQ(1u, queue.size_approx(1u));
  EXPECT_EQ(0u, queue.size_approx(2u));
  EXPECT_EQ(2, *queue.peek());

  // New element in different queue
  EXPECT_TRUE(queue.enqueue(1, 2u));
  EXPECT_EQ(2u, queue.size_approx());
  EXPECT_EQ(0u, queue.size_approx(0u));
  EXPECT_EQ(1u, queue.size_approx(1u));
  EXPECT_EQ(1u, queue.size_approx(2u));
  EXPECT_EQ(1, *queue.peek());

  // New element in same queue
  EXPECT_TRUE(queue.enqueue(3, 2u));

  // Pop
  EXPECT_EQ(1, *queue.peek());
  EXPECT_TRUE(queue.pop());
  EXPECT_EQ(2, *queue.peek());
  EXPECT_TRUE(queue.pop());
  std::cout << "line 57" << std::endl;
  EXPECT_EQ(3, *queue.peek());
  EXPECT_TRUE(queue.pop());
  EXPECT_EQ(nullptr, queue.peek());
  EXPECT_FALSE(queue.pop());
  EXPECT_EQ(0u, queue.size_approx());

  // Enqueue then dequeue a bunch
  EXPECT_TRUE(queue.enqueue(1, 0u));
  EXPECT_EQ(1, *queue.peek());
  EXPECT_TRUE(queue.enqueue(4, 2u));
  EXPECT_EQ(1, *queue.peek());
  EXPECT_TRUE(queue.enqueue(6, 2u));
  EXPECT_EQ(1, *queue.peek());
  EXPECT_TRUE(queue.enqueue(2, 1u));
  EXPECT_EQ(1, *queue.peek());
  EXPECT_TRUE(queue.enqueue(3, 0u));
  EXPECT_EQ(1, *queue.peek());
  EXPECT_TRUE(queue.enqueue(5, 0u));
  EXPECT_EQ(1, *queue.peek());
  EXPECT_TRUE(queue.enqueue(3, 1u));
  EXPECT_EQ(1, *queue.peek());
  EXPECT_TRUE(queue.enqueue(7, 1u));
  EXPECT_EQ(1, *queue.peek());
  EXPECT_EQ(8u, queue.size_approx());
  EXPECT_EQ(3u, queue.size_approx(0u));
  EXPECT_EQ(3u, queue.size_approx(1u));
  EXPECT_EQ(2u, queue.size_approx(2u));

  EXPECT_EQ(1, *queue.peek());
  EXPECT_TRUE(queue.pop());
  EXPECT_EQ(2, *queue.peek());
  EXPECT_TRUE(queue.pop());
  EXPECT_EQ(3, *queue.peek());
  EXPECT_TRUE(queue.pop());
  EXPECT_EQ(3, *queue.peek());
  EXPECT_TRUE(queue.pop());
  EXPECT_EQ(4, *queue.peek());
  EXPECT_TRUE(queue.pop());
  EXPECT_EQ(5, *queue.peek());
  EXPECT_TRUE(queue.pop());
  EXPECT_EQ(6, *queue.peek());
  EXPECT_TRUE(queue.pop());
  EXPECT_EQ(7, *queue.peek());
  EXPECT_TRUE(queue.pop());
  EXPECT_EQ(nullptr, queue.peek());
  EXPECT_FALSE(queue.pop());
  EXPECT_EQ(0u, queue.size_approx());
}
