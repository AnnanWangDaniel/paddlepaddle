//   Copyright (c) 2021 PaddlePaddle Authors. All Rights Reserved.
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

#include "paddle/pten/kernels/xpu/manipulation.h"
#include "paddle/pten/infermeta/unary.h"
#include "paddle/pten/kernels/hybird/general/manipulation.h"
#include "paddle/pten/kernels/xpu/utils.h"

namespace pten {

template <typename T>
void Flatten(const XPUContext& dev_ctx,
             const DenseTensor& x,
             int start_axis,
             int stop_axis,
             DenseTensor* out) {
  auto out_dims = out->dims();
  pten::Copy(dev_ctx, x, false, out);
  out->Resize(out_dims);
}

// TODO(yuanrisheng): this kernel is for training and xshape is a Intermediate
// Output Tensor，
// is there a more flexible way to deal with this case?
template <typename T>
void FlattenWithXShape(const XPUContext& dev_ctx,
                       const DenseTensor& x,
                       int start_axis,
                       int stop_axis,
                       DenseTensor* out,
                       DenseTensor* xshape) {
  Flatten<T>(dev_ctx, x, start_axis, stop_axis, out);
  const auto& in_dims = x.dims();
  std::vector<int64_t> xshape_dims(in_dims.size() + 1);
  xshape_dims[0] = 0;
  for (int i = 0; i < in_dims.size(); ++i) {
    xshape_dims[i + 1] = in_dims[i];
  }
  xshape->Resize(paddle::framework::make_ddim(xshape_dims));
  xshape->ResetLoD(x.lod());
}

void Reshape(const XPUContext& dev_ctx,
             const DenseTensor& x,
             const ScalarArray& shape,
             DenseTensor* out) {
  auto out_meta = InferMetaFromVecValue(x.meta(), shape.GetData());
  if (x.data() == out->data() && x.numel() == out->numel()) {
    out->Resize(out_meta.dims);
    return;
  }
  pten::Copy(dev_ctx, x, false, out);
  out->Resize(out_meta.dims);
  out->ResetLoD(x.lod());
}

void ReshapeWithXShape(const XPUContext& dev_ctx,
                       const DenseTensor& x,
                       const ScalarArray& shape,
                       DenseTensor* xshape,
                       DenseTensor* out) {
  general::SetXShape(x, xshape);
  Reshape(dev_ctx, x, shape, out);
}

}  // namespace pten

PT_REGISTER_KERNEL(flatten,
                   XPU,
                   ANY,
                   pten::Flatten,
                   float,
                   paddle::platform::float16,
                   double,
                   uint8_t,
                   int8_t,
                   int,
                   int64_t) {}

PT_REGISTER_KERNEL(flatten_with_xshape,
                   XPU,
                   ANY,
                   pten::FlattenWithXShape,
                   float,
                   paddle::platform::float16,
                   double,
                   uint8_t,
                   int8_t,
                   int,
                   int64_t) {}

PT_REGISTER_KERNEL_ALL_DTYPE(reshape, XPU, ANY, pten::Reshape) {}
