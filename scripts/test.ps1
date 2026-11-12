$ErrorActionPreference = "Stop"

& "$PSScriptRoot\scripts\build.ps1" "Debug" "test"

ctest --test-dir build/Debug -j8 --output-on-failure @args
