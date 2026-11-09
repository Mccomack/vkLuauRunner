// Homebrew LLVM 22 libc++ ABI 불일치 워크어라운드
// import std와 혼용 금지 — 이 파일은 독립적으로 컴파일됨

// 이 파일에서는 import std를 절대 사용하지 않음
// 헤더만 직접 포함
#include <__format/buffer.h>

namespace std {
inline namespace __1 {
namespace __format {
    template __allocating_buffer<char>::~__allocating_buffer();
}
}
}