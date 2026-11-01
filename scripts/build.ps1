param(
    [string]$BuildTypeArg = "",
    [string]$TestArg = ""
)

$ErrorActionPreference = "Stop"

$tmp = $BuildTypeArg.ToLower()
if ($tmp -eq "release") {
    $buildType = "Release"
} else {
    if ($tmp -ne "debug" -and $tmp -ne "") {
        Write-Host "Please set build type(Release, Debug) at the first argument. using default build type(Debug)..."
    }
    $buildType = "Debug"
}

$buildTest = "OFF"
if ($TestArg.ToLower() -eq "test") {
    $buildTest = "ON"
}

$toolchain = "cmake/toolchain/windows.cmake"
$target = "$env:LOCALAPPDATA\vkLuauRunner"

cmake -S . -B "build/$buildType" -G "Ninja" "-DCMAKE_TOOLCHAIN_FILE=$toolchain" "-DCMAKE_BUILD_TYPE=$buildType" "-DBUILD_TEST=$buildTest"
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

cmake --build "build/$buildType"
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

# Directory symlinks need admin rights on Windows; junctions don't.
cmd /c "rmdir build\current" 2>$null | Out-Null
cmd /c "mklink /J build\current build\$buildType" | Out-Null

New-Item -ItemType Directory -Force -Path "logs" | Out-Null
New-Item -ItemType Directory -Force -Path "runtest" | Out-Null

# mklink /J resolves a relative target against the CURRENT DIRECTORY, not
# the junction's own location (unlike a symlink or a Unix `ln -s`), so
# relative targets here must use absolute paths to avoid landing one level
# too high (e.g. "..\asset" from repo root resolves outside the repo).
$repoRoot = (Get-Location).Path

foreach ($link in @(
    @{ Path = "runtest\shaders"; Target = "$repoRoot\build\shaders" },
    @{ Path = "runtest\asset";   Target = "$repoRoot\asset" },
    @{ Path = "runtest\logs";    Target = "$repoRoot\logs" }
)) {
    cmd /c "rmdir `"$($link.Path)`"" 2>$null | Out-Null
    cmd /c "mklink /J `"$($link.Path)`" `"$($link.Target)`"" | Out-Null
}

$runtestPath = (Resolve-Path runtest).Path

cmd /c "rmdir `"$target`"" 2>$null | Out-Null
cmd /c "mklink /J `"$target`" `"$runtestPath`"" | Out-Null
