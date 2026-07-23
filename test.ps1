$ErrorActionPreference = "Stop"

& "$PSScriptRoot\build.ps1" "Debug" "test"

ctest --test-dir build/Debug --output-on-failure @args
