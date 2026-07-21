# vkLuauRunner

## 목적

  Electron 대체

## 필요 라이브러리

  - [Vulkan](https://vulkan.lunarg.com/sdk/home): 그래픽 라이브러리. 설치 필요
  
  - [LLVM clang](https://github.com/llvm/llvm-project/releases/): 빌드 툴. macOS면 homebrew 버전. 설치 필요
  
  - [cmake](https://cmake.org/download/): 빌드 툴. 설치 필요

## 빌드 방법

### Linux / macOS

  ```bash
  ./build.sh [debug/release] [test]
  ```

  debug/release:
    컴파일 타겟 설정

  test:
    테스트 빌드 설정

### Windows

  필요 도구 (모두 PATH에 추가):

  - [Vulkan SDK](https://vulkan.lunarg.com/sdk/home)

  - [llvm-mingw](https://github.com/mstorsjo/llvm-mingw/releases): clang + libc++ + lld가 포함된 Windows용 배포판임. MSVC(`cl.exe`)는 안 써 (`-stdlib=libc++`를 프로젝트 전반에서 사용하기 때문). `LLVM_MINGW_ROOT` 환경변수로 설치 경로를 지정하거나, `bin` 폴더를 PATH에 추가해라

  - [cmake](https://cmake.org/download/)

  - [ninja](https://github.com/ninja-build/ninja/releases)

  - [Slang](https://github.com/shader-slang/slang/releases) (`slangc`): 셰이더 컴파일러

  ```powershell
  .\build.ps1 [Debug/Release] [test]
  .\run.ps1
  ```

  `test`:
    테스트 빌드 설정 (`.\test.ps1`로 실행)
