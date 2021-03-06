#include <ATen/core/boxing/KernelFunction.h>

#include <sstream>

namespace c10 {

// This a "fake" kernel which doesn't actually do anything.  Instead, it is a
// distinguished kernel which is special cased by the dispatch table to
// be handled specially.  Its semantics is that it redispatches to the
// *next* dispatch key that would have been processed, skipping the current
// one.
void fallthrough_kernel(OperatorKernel*, const OperatorHandle&, Stack*) {
  TORCH_INTERNAL_ASSERT(0,
    "fallthrough_kernel was executed but it should have been short-circuited by the dispatcher. "
    "This could occur if you registered a fallthrough kernel as a override for a specific operator "
    "(as opposed to a backend fallback); this is NOT currently supported, and we do not intend to "
    "add support for it in the near future.  If you do find yourself in need of this, "
    "let us know in the bug tracker.");
}

// single line summary of state
std::string KernelFunction::dumpState() const {
  std::ostringstream oss;
  if (boxed_kernel_func_ == fallthrough_kernel) {
    oss << "fallthrough ";
  }
  if (boxed_kernel_func_) {
    oss << "boxed ";
  }
  if (unboxed_kernel_func_) {
    oss << "unboxed ";
  }
  if (functorFactory_) {
    oss << "functor ";
  }
  return oss.str();
}

bool KernelFunction::_equalsBoxedAndUnboxed(const KernelFunction& other) const {
  return boxed_kernel_func_ == other.boxed_kernel_func_ &&
         unboxed_kernel_func_ == other.unboxed_kernel_func_;
}

} // namespace c10
