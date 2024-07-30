@echo off
setlocal

:: Define the base BuildFolder environment variable
set BaseBuildFolder=%~dp0build

:: Check if cmake is installed
cmake --version >nul 2>&1
if %errorlevel% neq 0 (
    echo CMake is not installed. Please install CMake and try again.
    exit /b 1
)

:: Create build directories for x86 and x64
set BuildFolder_x86=%BaseBuildFolder%_x86
set BuildFolder_x64=%BaseBuildFolder%_x64

if not exist %BuildFolder_x86% mkdir %BuildFolder_x86%
if not exist %BuildFolder_x64% mkdir %BuildFolder_x64%

:: Configure for x86
cd %BuildFolder_x86%
cmake -A Win32 -DCMAKE_CONFIGURATION_TYPES="Debug;Release" ^
    -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG=%BuildFolder_x86%/Debug ^
    -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE=%BuildFolder_x86%/Release ^
    -DCMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG=%BuildFolder_x86%/Debug ^
    -DCMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE=%BuildFolder_x86%/Release ^
    -DCMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG=%BuildFolder_x86%/Debug ^
    -DCMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE=%BuildFolder_x86%/Release ^
    -DBUILD_SHARED_LIBS=OFF ..
if %errorlevel% neq 0 (
    echo Failed to configure the project for x86.
    exit /b 1
)
cd ..

:: Configure for x64
cd %BuildFolder_x64%
cmake -A x64 -DCMAKE_CONFIGURATION_TYPES="Debug;Release" ^
    -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG=%BuildFolder_x64%/Debug ^
    -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE=%BuildFolder_x64%/Release ^
    -DCMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG=%BuildFolder_x64%/Debug ^
    -DCMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE=%BuildFolder_x64%/Release ^
    -DCMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG=%BuildFolder_x64%/Debug ^
    -DCMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE=%BuildFolder_x64%/Release ^
    -DBUILD_SHARED_LIBS=OFF ..
if %errorlevel% neq 0 (
    echo Failed to configure the project for x64.
    exit /b 1
)
cd ..

:: Build for x86 Debug and Release
cd %BuildFolder_x86%
cmake --build . --config Debug
if %errorlevel% neq 0 (
    echo Failed to build the project for x86 Debug.
    exit /b 1
)
cmake --build . --config Release
if %errorlevel% neq 0 (
    echo Failed to build the project for x86 Release.
    exit /b 1
)
cd ..

:: Build for x64 Debug and Release
cd %BuildFolder_x64%
cmake --build . --config Debug
if %errorlevel% neq 0 (
    echo Failed to build the project for x64 Debug.
    exit /b 1
)
cmake --build . --config Release
if %errorlevel% neq 0 (
    echo Failed to build the project for x64 Release.
    exit /b 1
)
cd ..

echo Build completed successfully for both x86 and x64 in Debug and Release configurations.
endlocal