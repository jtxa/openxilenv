# OpenXilEnv

OpenXilEnv is a lightweight SIL/HIL environment. At the moment OpenXilEnv focus one SIL environment.

## SIL

With a **S**oftware **I**n the **L**oop system it is possible to run and test embedded software without a target platform and compiler. XilEnv is an environment to setup a SIL system on Windows or Linux host. It will be clean separate it own components from the embedded software under test with a network layer. Each component can run in an own executable, they are memory protected against each other. Communication between software under test, model and XilEnv are made with socket or on Windows Named pipe / Linux local sockets
and will be used for:

- Signal transfers.
- Virtual network (CAN and CAN FD) transfers.

![XilEnv SIL system](OpenXilEnv_Sil.png)

**XilEnvGui** have a configurable graphical user interface base on Qt. There exist value display/change, oscilloscope, calibration, slider, knob, ... elements for interaction.

**XilEnv** have no graphical user interface and can be used for automation with no user interaction.

- Current GCC compiler with dwarf debug information is Supported. Visual Studio compiler only without debug information.
- FMUs with FMI2.0 interface are supported (by **ExtProc_FMU2Extract(.EXE)**, **ExtProc_FMU2Loader32(.EXE)**, **ExtProc_FMU2Loader64(.EXE)**)
  Partial FMI3.0 interface are supported (by **ExtProc_FMU3Extract(.EXE)**, **ExtProc_FMU3Loader32(.EXE)**, **ExtProc_FMU3Loader64(.EXE)**)
  (It will be only support the additional data types).
- An small A2L parser is included for calibration.
- With a XCP over ethernet port a connection to a calibration system is possible.
- Multicore support with barriers for synchronization.
- XilEnv is automatable through a remote procedure call interface **XilEnvRpc.DLL/.so** or a builtin script interpreter.
- The software under test, FMU and the model can be a 32 or 64bit executable (a mixture is allowed). XilEnv must be a 64bit executable
- Time is simulated with a fixed configurable period with a resolution of 1ns.
- Residual bus simulation of none existing CAN (FD) bus members are available.
- A recording and stimulation is possible with text or MDF3 files.

The DLL/shared library **XilEnvExtProc64.DLL/.so** or **XilEnvExtProc32.DLL/.so** are the interface for the embedded test software or model.This DLL/shared library must be load dynamically. The main module **XilEnvExtProcMain.c** will do this for you. The declaration of the interface functions lives here: **XilEnvRtProc.h**. An easy example can be found inside the folder "Samples/ExternalProcesses/ExtProc_Simple".

## HIL option

To use OpenXilEnv as a HIL system a second PC with Linux is necessary. Currently only socket CAN (FD) are supported as hardware interfaces. A defined time response < 1ms are possible if RT-Preempt patch is installed.

To fulfill defined time response requirement XilEnv will be spit into two pieces (one with a defined time response and one without defined time response)
All component of XilEnv needed defined time response will be extracted to a shared library **LinuxRemoteMasterCore.so** the rest stays inside the XilEnv executable. The part with defined time response must be executed on a Linux system with RT-Preempt patch installed. The part with no defined time response can be execute on Linux or Windows. Between both a local (point to point) ethernet connection should be established. If a model is needed it must be compiled for Linux and must load the shared library **LinuxRemoteMasterCore.so** If no model is needed **LinuxRemoteMaster.Out** can be used. The service **RemoteStartServer** should be installed and activated on the second PC. So  XilEnv can control the second PC to copy and start all needed executable from the main PC

## Table of contents

- [OpenXilEnv](#openxilenv)
  - [SIL](#sil)
  - [HIL option](#hil-option)
  - [Table of contents](#table-of-contents)
  - [Source Code](#source-code)
  - [Build Instructions](#build-instructions)
    - [Windows](#windows)
      - [Install dependencies](#install-dependencies)
      - [Build options](#build-options)
      - [Build and install](#build-and-install)
      - [Run example](#run-example)
    - [Linux](#linux)
      - [Install dependencies](#install-dependencies-1)
      - [Build and install](#build-and-install-1)
      - [Run example](#run-example-1)
        - [Build options](#build-options-1)
  - [Setting up your project](#setting-up-your-project)
  - [Setup an own external process](#setup-an-own-external-process)

<!-- markdownlint-disable-file MD033 -->

## <a name='source-code'></a>Source Code

You can get OpenXilEnv from:

<https://github.com/eclipse-openxilenv>

## <a name='build-instructions'></a>Build Instructions

### <a name='windows'></a>Windows

#### <a name='install-dependencies-window'></a>Install dependencies

- Qt Library 5.12.9 ... 6.7.2
- MinGW 11.2
- Strawberry perl
- pugixml-1.11 (optional if `-DBUILD_WITH_FMU2_SUPPORT=ON` or if `-DBUILD_WITH_FMU3_SUPPORT=ON`)

    <https://pugixml.org/>
<br/>

- FMU Parser (optional if `-DBUILD_WITH_FMU2_SUPPORT=ON` or if `-DBUILD_WITH_FMU3_SUPPORT=ON`)

    <https://github.com/modelica/fmi-standard/releases/download/v3.0.1/FMI-Standard-3.0.1.zip>

    <https://github.com/modelica/fmi-standard/releases/download/v2.0.4/FMI-Standard-2.0.4.zip>
<br/>

#### <a name='build-options-windows'></a>Build options

```bat
  > -DBUILD_EXAMPLES=ON/OFF (default is ON)
  > -DBUILD_WITH_FMU2_SUPPORT=ON/OFF (default is OFF)
  >   -D FMI2_SOURCE_PATH=<path>
  >   -D PUGIXML_SOURCE_PATH=<path>
  > -DBUILD_WITH_FMU3_SUPPORT=ON/OFF (default is OFF)
  >   -D FMI3_SOURCE_PATH=<path>
  >   -D PUGIXML_SOURCE_PATH=<path>
  > -DBUILD_ESMINI_EXAMPLE=ON/OFF (default is OFF)
  >   -D ESMINI_LIBRARY_PATH=<path>
  > -DCMAKE_BUILD_TYPE=Debug/Release (default is Release)
  > -DCMAKE_INSTALL_PREFIX=../install_win
  > -DBUILD_32BIT=ON/OFF (default is OFF, if you want to build the 32 bit support you need also a 32 bit MinGW)
```

#### <a name='build-and-install-windows'></a>Build and install

Open a command box and enter following commands:

```bat
  > mkdir xxx\openxilenv\build_win
  > cd xxx\openxilenv\build_win
  > set PATH=[PathToQt]\bin;%PATH%
  > set PATH=[PathToGcc]\bin;%PATH%
  > set PATH=[PathToStrawberry]\c\bin;%PATH%
  > set PATH=[PathToCMAKE]\bin;%PATH%
  > cmake -G [YOUR Compiler] [CMAKE-LIST-FILEPATH] [BUILD OPTIONS]
  > cmake --build .
```

Install

```bat
  > cmake --install .
```

Install optional (copy the Qt DLL to the install folder)

```bat
  > cd xxx\openxilenv\install_win
  > windeployqt6.exe XilEnvGui.exe
```

#### <a name='run-example-windows'></a>Run example

Running the electric car sample

```bat
  > cd xxx\openxilenv\install_win
  > .\XilEnvGui.exe -ini ..\Samples\Configurations\ElectricCarSample.ini
```

### <a name='linux'></a>Linux

#### <a name='install-dependencies-linux'></a>Install dependencies

- Qt Library 5.12.9 ... 6.7.2
- pugixml-1.11 (optional if `-DBUILD_WITH_FMU2_SUPPORT=ON` or if `-DBUILD_WITH_FMU3_SUPPORT=ON`)

    <https://pugixml.org/>
<br/>

- FMU Parser (optional if `-DBUILD_WITH_FMU2_SUPPORT=ON` or if `-DBUILD_WITH_FMU3_SUPPORT=ON`)

    <https://github.com/modelica/fmi-standard/releases/download/v3.0.1/FMI-Standard-3.0.1.zip>

    <https://github.com/modelica/fmi-standard/releases/download/v2.0.4/FMI-Standard-2.0.4.zip>
<br/>

#### <a name='build-and-install-linux'></a>Build and install

Open a bash and enter following commands:

```sh
  > mkdir xxx/openxilenv/build_linux
  > cd xxx/openxilenv/build_linux
  > cmake -DBUILD_EXAMPLES=ON -DCMAKE_INSTALL_PREFIX=../install_linux -DCMAKE_BUILD_TYPE=Release ../.
  > cmake --build .
```

Install

```sh
  > cmake --install .
```

#### <a name='run-example-linux'></a>Run example

Running the electric car sample

```sh
  > cd xxx\openxilenv\install_linux
  > ./XilEnvGui -ini ../Samples/Configurations/ElectricCarSample.ini
```

##### <a name='build-options-linux'></a>Build options

```sh
  > -DBUILD_EXAMPLES=ON/OFF (default is ON)
  > -DBUILD_WITH_FMU2_SUPPORT=ON/OFF (default is OFF)
  >   -D PUGIXML_SOURCE_PATH=<path>
  >   -D FMI2_SOURCE_PATH=<path>
  > -DBUILD_WITH_FMU3_SUPPORT=ON/OFF (default is OFF)
  >   -D PUGIXML_SOURCE_PATH=<path>
  >   -D FMI3_SOURCE_PATH=<path>
  > -DBUILD_ESMINI_EXAMPLE=ON/OFF (default is OFF)
  >   -D ESMINI_LIBRARY_PATH=<path>
  > -DCMAKE_BUILD_TYPE=Debug/Release (default is Release)
  > -DCMAKE_INSTALL_PREFIX=../install_win
  > -DBUILD_32BIT=ON/OFF (default is OFF)
```

## <a name='setting-up-your-project'></a>Setting up your project

## <a name='Setup-an-own-external-process'></a>Setup an own external process

An easy simple external process have to provide 4 functions.

```C
void reference_varis (void)
```

This function will be called one time if the process is started. Here should be added all signals the process is needed

```C
int init_test_object (void)
```

This function will be called after the reference_varis function. Here can be done some initialization stuff.

```C
void cyclic_test_object (void)
```

This is the main cyclic function of the external process this will be called for each simulated time cycle.

```C
void terminate_test_object (void)
```

This function will be called one time if the external process will be terminated.

Source code of `ExtProc_Simple.c`
(you can find the code also inside the folder `Samples\ExternalProcesses\ExtProc_Simple`)

```C
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define XILENV_INTERFACE_TYPE XILENV_DLL_INTERFACE_TYPE
#include "XilEnvExtProc.h"
#include "XilEnvExtProcMain.c"

// Global variable of the external process
short Ramp;
double Sinus;
short Random;
double SampleTime;

// use volatile const to avoid compiler optimization
volatile const short BOTTOM_LIMIT_RAMP = 0;
volatile const short UPPER_LIMIT_RAMP = 1000;

volatile const double SINUS_AMPLITUDE  = 1.0;
volatile const double SINUS_FREQUENCY = 0.2;

// This will be called first if the process is started
void reference_varis (void)
{
    REFERENCE_WORD_VAR(Ramp, "Ramp");
    REFERENCE_DOUBLE_VAR(Sinus, "Sinus");
    REFERENCE_WORD_VAR(Random, "Random");
    REFERENCE_DOUBLE_VAR(SampleTime, "XilEnv.SampleTime");
}

// This function will be called next to reference_varis
int init_test_object(void)
{
    return 0;   // == 0 -> No error continue
                // != 0 -> Error do not continue
}

// This will call every simulated cycle
void cyclic_test_object(void)
{
    static double SinusTime;
    if(Ramp++ > UPPER_LIMIT_RAMP) Ramp = BOTTOM_LIMIT_RAMP;
    SinusTime += 2.0 * M_PI * SampleTime * SINUS_FREQUENCY;
    Sinus = SINUS_AMPLITUDE * sin(SinusTime);
    Random = rand();
}

// This will be called if the external process will be terminated
void terminate_test_object(void)
{
}
```

To build this small sample:

On Windows:

```bat
  > gcc -g -I xxx\openxilenv\install_win\include ExtProc_Simple.c -o ExtProc_Simple.exe
```

On Linux:

```sh
  > gcc -g -I xxx/openxilenv/install_linux/include ExtProc_Simple.c -ldl -lpthread -o ExtProc_Simple.EXE
```

Now you should be able to start your own external process and add this to the electric car sample:

On Windows:

```bat
  > set PATH=%PATH%;xxx\openxilenv\install_win
  > ExtProc_Simple.exe -q2 XilEnvGui.exe -ini xxx\openxilenv\Samples\Configurations\ElectricCarSample.ini
```

On Linux:

```sh
  > export PATH=$PATH;xxx\openxilenv\install_linux
  > ExtProc_Simple.EXE -q2 XilEnvGui -ini xxx/openxilenv/Samples/Configurations/ElectricCarSample.ini
```
