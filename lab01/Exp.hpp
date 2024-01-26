#include <type_traits>

namespace ADAAI {
template <typename F>
    requires std::is_floating_point_v<F>
constexpr F Exp(F x) {
    return x;
}
}  // namespace ADAAI
