/* Copyright (c) 2021 PaddlePaddle Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */

#pragma once
#include <cmath>
#include <limits>
#include "paddle/fluid/operators/reduce_ops/reduce_op.cu.h"
#include "paddle/fluid/platform/hostdevice.h"
#ifdef __HIPCC__
#include <hip/hip_runtime.h>
#endif

namespace paddle {
namespace operators {

namespace kps = paddle::operators::kernel_primitives;

template <typename Tx, typename Ty = Tx>
struct CustomMin {
  using Transformer = kps::IdentityFunctor<Tx>;

  inline Ty initial() {
    return static_cast<Ty>(std::numeric_limits<Ty>::max());
  }

  __device__ __forceinline__ Ty operator()(const Ty &a, const Ty &b) const {
    return (b < a) ? b : a;
  }
};

template <typename Tx, typename Ty = Tx>
struct CustomMax {
  using Transformer = kps::IdentityFunctor<Tx>;

  inline Ty initial() {
    return static_cast<Ty>(std::numeric_limits<Ty>::lowest());
  }

  __device__ __forceinline__ Ty operator()(const Ty &a, const Ty &b) const {
    return (b > a) ? b : a;
  }
};

// for cub::Reduce
template <typename Tx, typename Ty = Tx>
struct CustomSum {
  using Transformer = kps::IdentityFunctor<Tx, Ty>;

  inline Ty initial() { return static_cast<Ty>(0.0f); }

  __device__ __forceinline__ Ty operator()(const Ty &a, const Ty &b) const {
    return b + a;
  }
};

template <typename Tx, typename Ty = Tx>
struct CustomSub {
  using Transformer = kps::InverseFunctor<Tx>;

  inline Ty initial() { return static_cast<Ty>(0.0f); }

  __device__ __forceinline__ Ty operator()(const Ty &a, const Ty &b) const {
    return b + a;
  }
};

template <typename Tx, typename Ty = Tx>
struct CustomMean {
  using Transformer = kps::DivideFunctor<Tx>;

  inline Ty initial() { return static_cast<Ty>(0.0f); }

  __device__ __forceinline__ Ty operator()(const Ty &a, const Ty &b) const {
    return b + a;
  }
};

template <typename Tx, typename Ty = Tx>
struct CustomMul {
  using Transformer = kps::IdentityFunctor<Tx>;

  inline Ty initial() { return static_cast<Ty>(1.0f); }

  __device__ __forceinline__ Ty operator()(const Ty &a, const Ty &b) const {
    return b * a;
  }
};

template <typename Tx, typename Ty = Tx>
struct CustomLogicalOr {
  using Transformer = kps::IdentityFunctor<Tx>;

  inline Ty initial() { return static_cast<Ty>(false); }

  __device__ __forceinline__ Ty operator()(const Ty &a, const Ty &b) const {
    return b || a;
  }
};

template <typename Tx, typename Ty = Tx>
struct CustomLogicalAnd {
  using Transformer = kps::IdentityFunctor<Tx>;

  inline Ty initial() { return static_cast<Ty>(true); }

  __device__ __forceinline__ Ty operator()(const Ty &a, const Ty &b) const {
    return b && a;
  }
};

}  // namespace operators
}  // namespace paddle
