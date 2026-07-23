if(DEFINED ENV{LLVM_MINGW_ROOT} AND NOT DEFINED LLVM_MINGW_ROOT)
    set(LLVM_MINGW_ROOT "$ENV{LLVM_MINGW_ROOT}")
endif()

if(LLVM_MINGW_ROOT)
    # Mixed \ and / separators break string escaping when CMake writes this
    # path into generated cache files (e.g. "\l" is not a valid escape).
    file(TO_CMAKE_PATH "${LLVM_MINGW_ROOT}" LLVM_MINGW_ROOT)
endif()

if(NOT LLVM_MINGW_ROOT)
    find_program(LLVM_MINGW_CXX_HINT NAMES clang++ clang++.exe REQUIRED)
    get_filename_component(LLVM_MINGW_BIN "${LLVM_MINGW_CXX_HINT}" DIRECTORY)
    get_filename_component(LLVM_MINGW_ROOT "${LLVM_MINGW_BIN}" DIRECTORY)
endif()

# llvm-mingw's plain clang(++).exe reports a normalized "-w64-windows-gnu"
# triple via -dumpmachine that does NOT match its own on-disk directory
# layout (still "-w64-mingw32"), so deriving the sysroot dir from that would
# silently break. Use the triple-prefixed wrapper binaries instead
# (e.g. x86_64-w64-mingw32-clang++.exe) - their name IS the mingw32 triple
# and matches the directory layout directly.
if(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "ARM64|aarch64")
    set(LLVM_MINGW_TARGET_TRIPLE "aarch64-w64-mingw32")
else()
    set(LLVM_MINGW_TARGET_TRIPLE "x86_64-w64-mingw32")
endif()

set(CMAKE_C_COMPILER "${LLVM_MINGW_ROOT}/bin/${LLVM_MINGW_TARGET_TRIPLE}-clang.exe")
set(CMAKE_CXX_COMPILER "${LLVM_MINGW_ROOT}/bin/${LLVM_MINGW_TARGET_TRIPLE}-clang++.exe")

set(CMAKE_CXX_FLAGS_INIT "-stdlib=libc++")

# llvm-mingw links libc++/libunwind dynamically by default; those DLLs live
# under LLVM_MINGW_ROOT\bin, which isn't on a normal user's PATH, so the exe
# fails to start with STATUS_DLL_NOT_FOUND unless statically linked.
set(CMAKE_EXE_LINKER_FLAGS "-stdlib=libc++ -static")

set(LIBCXX_MODULES_JSON "${LLVM_MINGW_ROOT}/${LLVM_MINGW_TARGET_TRIPLE}/lib/libc++.modules.json")

if(EXISTS "${LIBCXX_MODULES_JSON}")
    set(CMAKE_CXX_STDLIB_MODULES_JSON "${LIBCXX_MODULES_JSON}")
else()
    message(WARNING
        "Could not find libc++.modules.json at ${LIBCXX_MODULES_JSON}. "
        "import std will not work until CMAKE_CXX_STDLIB_MODULES_JSON is set manually."
    )
endif()
