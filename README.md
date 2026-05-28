# vkLuauRunner

## 목적

  응용 프로그램으로 배포할 수 있는 프로그램 형식의 GUI 창 만드는 게 너무 어려움 / 느림.
  Qt는 cpp 쓰면 만들 수 있지만 꽤 만들기 귀찮음.
  Electron은 Chromium을 기반으로 만들어서 만들기 쉽긴 하지만 느림.

## 필요 라이브러리

  Vulkan: 그래픽 라이브러리. 설치 필요
  LLVM clang: 빌드 툴. macOS면 homebrew 버전. 설치 필요
  cmake: 빌드 툴. 설치 필요

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
