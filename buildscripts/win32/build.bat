:: Setup environment variables
@call %~dp0env_var.bat

:: Create build directory
set BUILD_DIR = %GF_ROOT%\build
mkdir %BUILD_DIR%

:: Run cmake on the directory
cmake -S %GF_ROOT% -B %BUILD_DIR%


