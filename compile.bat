@echo off
if "%1" == "-d" (
    echo Bulding for debug...
    echo.
    cmake -DCMAKE_BUILD_TYPE=Debug -G "MinGW Makefiles" ..
) else if "%1" == "-r" (
    echo Bulding for release...
    echo.
    cmake -DCMAKE_BUILD_TYPE=Release -G "MinGW Makefiles" ..
) else (
    echo You need to add command -d for debug build or -r for release build
    goto end
)
echo.
echo Using mingw32-make...
echo.
mingw32-make
:end