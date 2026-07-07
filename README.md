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

  아직 없음
