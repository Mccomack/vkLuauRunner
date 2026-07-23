$ErrorActionPreference = "Stop"

& "$PSScriptRoot\build.ps1" "Debug"

New-Item -ItemType Directory -Force -Path "runtest" | Out-Null
Push-Location runtest
try {
    # No lldb equivalent here (that's macOS-only in debug.sh); attach your
    # debugger of choice (e.g. Visual Studio, WinDbg) to the launched process,
    # or run it under one directly, e.g.:
    #   devenv /debugexe ..\build\Debug\src\program.exe
    & "..\build\Debug\src\program.exe"
} finally {
    Pop-Location
}
