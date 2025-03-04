/* Copyright (c) 2016 PaddlePaddle Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */

#include "paddle/fluid/memory/malloc.h"

#include "paddle/fluid/memory/allocation/allocator_facade.h"
#include "paddle/fluid/platform/place.h"

namespace paddle {
namespace memory {

std::shared_ptr<Allocation> AllocShared(const platform::Place& place,
                                        size_t size) {
  return allocation::AllocatorFacade::Instance().AllocShared(place, size);
}

AllocationPtr Alloc(const platform::Place& place, size_t size) {
  return allocation::AllocatorFacade::Instance().Alloc(place, size);
}

uint64_t Release(const platform::Place& place) {
  return allocation::AllocatorFacade::Instance().Release(place);
}

#if defined(PADDLE_WITH_CUDA) || defined(PADDLE_WITH_HIP)
std::shared_ptr<Allocation> AllocShared(const platform::CUDAPlace& place,
                                        size_t size,
                                        const gpuStream_t& stream) {
  return allocation::AllocatorFacade::Instance().AllocShared(place, size,
                                                             stream);
}

AllocationPtr Alloc(const platform::CUDAPlace& place, size_t size,
                    const gpuStream_t& stream) {
  return allocation::AllocatorFacade::Instance().Alloc(place, size, stream);
}

uint64_t Release(const platform::CUDAPlace& place, const gpuStream_t& stream) {
  return allocation::AllocatorFacade::Instance().Release(place, stream);
}

void RecordStream(Allocation* allocation, const gpuStream_t& stream) {
  return allocation::AllocatorFacade::Instance().RecordStream(allocation,
                                                              stream);
}
#endif
}  // namespace memory
}  // namespace paddle
