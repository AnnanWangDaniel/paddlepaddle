cc_library(math_cpu SRCS math.cc DEPS dense_tensor kernel_context kernel_factory eigen_function blas pten_transpose_cpu)
cc_library(linalg_cpu SRCS linalg.cc DEPS dense_tensor kernel_context kernel_factory)
cc_library(creation_cpu SRCS creation.cc DEPS dense_tensor kernel_context kernel_factory eigen_function)
cc_library(utils_cpu SRCS utils.cc DEPS dense_tensor kernel_context kernel_factory memory convert_utils)
cc_library(manipulation_cpu SRCS manipulation.cc DEPS dense_tensor kernel_context kernel_factory utils_cpu unary)
