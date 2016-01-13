/*
 * Copyright (c) 2015, Georgia Tech Research Corporation
 * All rights reserved.
 *
 * Author(s): Michael X. Grey <mxgrey@gatech.edu>
 *
 * Georgia Tech Graphics Lab and Humanoid Robotics Lab
 *
 * Directed by Prof. C. Karen Liu and Prof. Mike Stilman
 * <karenliu@cc.gatech.edu> <mstilman@cc.gatech.edu>
 *
 * This file is provided under the following "BSD-style" License:
 *   Redistribution and use in source and binary forms, with or
 *   without modification, are permitted provided that the following
 *   conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 *   CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *   INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 *   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 *   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 *   USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *   AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *   ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *   POSSIBILITY OF SUCH DAMAGE.
 */

#include <chrono>
#include <future>

#include <gtest/gtest.h>

#include "kido/simulation/World.hpp"

#include "TestHelpers.hpp"

using namespace kido;
using namespace dynamics;

//==============================================================================
void createAndDestroyFrames(int threadNum)
{
  for(size_t i=0; i < 100; ++i)
  {
    EXPECT_EQ(Frame::World()->getNumChildEntities(), 0);
    EXPECT_EQ(Frame::World()->getNumChildFrames(), 0);

    SimpleFrame someFrame(Frame::World(),
                          "Frame_"+std::to_string(threadNum)+std::to_string(i));

    EXPECT_EQ(Frame::World()->getNumChildEntities(), 0);
    EXPECT_EQ(Frame::World()->getNumChildFrames(), 0);
  }
}

//==============================================================================
TEST(Concurrency, FrameDeletion)
{
  // Regression test for issue #576
  std::vector<std::future<void>> futures;
  for(size_t i=0; i < 10; ++i)
    futures.push_back(std::async(std::launch::async,
                                 &createAndDestroyFrames, i));

  for(size_t i=0; i < futures.size(); ++i)
  {
    EXPECT_EQ(Frame::World()->getNumChildEntities(), 0);
    EXPECT_EQ(Frame::World()->getNumChildFrames(), 0);
    futures[i].get();
  }

  EXPECT_EQ(Frame::World()->getNumChildEntities(), 0);
  EXPECT_EQ(Frame::World()->getNumChildFrames(), 0);
}

//==============================================================================
int main(int argc, char* argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
