# Build Instructions for Windows

## Before You Begin

Make sure all required tools and libraries are installed as described [here](./WINDOWS_DEPENDENCIES.md). This includes MinGW, Qt, CMake, and optional FMU support libraries.

### Build Options

```batch
-D BUILD_EXAMPLES=ON/OFF (default ON)
-D BUILD_WITH_FMU2_SUPPORT=ON/OFF (default OFF)
  -D FMI2_SOURCE_PATH=<path>
  -D PUGIXML_SOURCE_PATH=<path>
-D BUILD_WITH_FMU3_SUPPORT=ON/OFF (default OFF)
  -D FMI3_SOURCE_PATH=<path>
  -D PUGIXML_SOURCE_PATH=<path>
-D BUILD_ESMINI_EXAMPLE=ON/OFF (default OFF)
  -D ESMINI_LIBRARY_PATH=<path>
-D CMAKE_BUILD_TYPE=Debug/Release (default Release)
-D CMAKE_INSTALL_PREFIX=<path>
-D BUILD_32BIT=ON/OFF (default OFF)
```

## 1. Fetch source

Clone the openxilenv source code from <https://github.com/eclipse-openxilenv/openxilenv.git> e.g. to `%SRC_ROOT%\openxilenv`

## 2. Create build directory

```batch
mkdir "%BUILD_ROOT%\openxilenv"
cd "%BUILD_ROOT%\openxilenv"
```

## 3. Ensure needed tools are in PATH

Ensure cmake, mingw, Ninja and Qt are in PATH. If not:

```batch
set PATH=<path-to-cmake>\bin;%PATH%
set PATH=<path-to-mingw>\bin;%PATH%
set PATH=<path-to-ninja>;%PATH%
set PATH=<path-to-Qt>\bin;%PATH%
```

## 4. Build and Install

Common configuration example:

```batch
cmake -G Ninja -D "CMAKE_INSTALL_PREFIX=%TOOLS_ROOT%\openxilenv" "%SRC_ROOT%\openxilenv" [Options]
cmake --build .
cmake --install .
```

## 5. (Optional) Deploy Qt DLLs

```batch
cd "%TOOLS_ROOT%\openxilenv"
windeployqt6.exe XilEnvGui.exe
```

## :white_check_mark: You're Ready to Run OpenXilEnv

If all steps completed successfully, OpenXilEnv is now ready to run. You can start by:

**Launching the GUI**:

```batch
cd "%TOOLS_ROOT%\openxilenv"
XilEnvGui
```

or

**Running an example**:

```batch
cd "%TOOLS_ROOT%\openxilenv"
.\XilEnvGui -ini "%SRC_ROOT%\openxilenv\Samples\Configurations\ElectricCarSample.ini"
```
