set(CMAKE_C_COMPILER "/opt/homebrew/opt/llvm/bin/clang")
set(CMAKE_CXX_COMPILER "/opt/homebrew/opt/llvm/bin/clang++")

set(CMAKE_CXX_FLAG_INIT "-stdlib=libc++")

list(APPEND CMAKE_SYSTEM_INCLUDE_PATH "/opt/homebrew/opt/llvm/include")

set(CMAKE_EXE_LINKER_FLAGS 
    "-L/opt/homebrew/opt/llvm/lib/c++ -L/opt/homebrew/opt/llvm/lib \
    -Wl,-rpath,/opt/homebrew/opt/llvm/lib/c++ \
    -Wl,-rpath,/opt/homebrew/opt/llvm/lib \
    -stdlib=libc++"
)

set(CMAKE_CXX_STDLIB_MODULES_JSON "/opt/homebrew/opt/llvm/lib/c++/libc++.modules.json")