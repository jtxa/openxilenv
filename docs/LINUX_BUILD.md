# Build Instructions for Linux

## Before You Begin

Make sure all required tools and libraries are installed as described [here](./LINUX_DEPENDENCIES.md). This includes GCC, Qt, CMake, and optional FMU support libraries.

### Build Options

```plain
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

Clone the openxilenv source code from <https://github.com/eclipse-openxilenv/openxilenv.git> e.g. to `$SRC_ROOT/openxilenv`

```bash
cd "$SRC_ROOT"
git clone https://github.com/eclipse-openxilenv/openxilenv.git
```

## 2. Create build directory

```bash
mkdir -p "$BUILD_ROOT/openxilenv"
cd "$BUILD_ROOT/openxilenv"
```

## 3. Ensure needed tools are in PATH

Ensure cmake, gcc, ninja and Qt are in PATH. If not:

```bash
export PATH="<path-to-cmake>/bin:$PATH"
export PATH="<path-to-ninja>:$PATH"
export PATH="$TOOLS_ROOT/Qt6.9/bin:$PATH"
export LD_LIBRARY_PATH="$TOOLS_ROOT/Qt6.9/lib:$LD_LIBRARY_PATH"
export Qt6_DIR="$TOOLS_ROOT/Qt6.9/lib/cmake/Qt6"
```

Example with the recommended paths:

```bash
export PATH="$TOOLS_ROOT/Qt6.9/bin:$PATH"
export LD_LIBRARY_PATH="$TOOLS_ROOT/Qt6.9/lib:$LD_LIBRARY_PATH"
export Qt6_DIR="$TOOLS_ROOT/Qt6.9/lib/cmake/Qt6"
```

## 4. Build and Install

Common configuration example:

```bash
cmake -G Ninja -D CMAKE_INSTALL_PREFIX="$TOOLS_ROOT/openxilenv" "$SRC_ROOT/openxilenv" [Options]
cmake --build .
cmake --install .
```

Common configuration example for examples:

```bash
cmake -G Ninja \
    -D CMAKE_INSTALL_PREFIX="$TOOLS_ROOT/openxilenv" \
    -D BUILD_EXAMPLES=ON \
    "$SRC_ROOT/openxilenv"
cmake --build .
cmake --install .
```

Example with FMU support:

```bash
cmake -G Ninja \
    -D CMAKE_INSTALL_PREFIX="$TOOLS_ROOT/openxilenv" \
    -D BUILD_EXAMPLES=ON \
    -D BUILD_WITH_FMU2_SUPPORT=ON \
    -D FMI2_SOURCE_PATH="$SRC_ROOT/FMI_2_0_4/headers" \
    -D PUGIXML_SOURCE_PATH="$SRC_ROOT/pugixml-1.15/src" \
    "$SRC_ROOT/openxilenv"
cmake --build .
cmake --install .
```

## 5. Add OpenXilEnv to PATH

To run OpenXilEnv from anywhere, add it to your PATH:

```bash
export PATH="$TOOLS_ROOT/openxilenv:$PATH"
export LD_LIBRARY_PATH="$TOOLS_ROOT/openxilenv:$LD_LIBRARY_PATH"
```

To make this permanent, add to your `~/.bashrc`:

```bash
echo 'export PATH="$TOOLS_ROOT/openxilenv:$PATH"' >> ~/.bashrc
echo 'export LD_LIBRARY_PATH="$TOOLS_ROOT/openxilenv:$LD_LIBRARY_PATH"' >> ~/.bashrc
source ~/.bashrc
```

## :white_check_mark: You're Ready to Run OpenXilEnv

If all steps completed successfully, OpenXilEnv is now ready to run. You can start by:

**Launching the GUI**:

```bash
cd "$TOOLS_ROOT/openxilenv"
./XilEnvGui
```

or

**Running an example**:

```bash
cd "$TOOLS_ROOT/openxilenv"
./XilEnvGui -ini "$SRC_ROOT/openxilenv/Samples/Configurations/ElectricCarSample.ini"
```

## Troubleshooting

### Qt libraries not found

If you get errors about missing Qt libraries when running XilEnvGui:

```bash
export LD_LIBRARY_PATH="$TOOLS_ROOT/Qt6.9/lib:$LD_LIBRARY_PATH"
```

### CMake cannot find Qt6

Set the Qt6_DIR variable explicitly:

```bash
export Qt6_DIR="$TOOLS_ROOT/Qt6.9/lib/cmake/Qt6"
```

Then reconfigure:

```bash
cd "$BUILD_ROOT/openxilenv"
rm -rf *
cmake -G Ninja -D CMAKE_INSTALL_PREFIX="$TOOLS_ROOT/openxilenv" "$SRC_ROOT/openxilenv"
```
