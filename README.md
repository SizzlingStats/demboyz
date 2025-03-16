# demboyz
we dem boyz

## Build Status
| System | Compiler | Status |
| ------ | -------- | ------ |
| Ubuntu | G++ | [![Build status](https://ci.appveyor.com/api/projects/status/vij4rx4kfsfa73ai/branch/master?svg=true)](https://ci.appveyor.com/project/SizzlingCalamari/demboyz-linux/branch/master) |
| Windows | VS2022 | [![Build status](https://ci.appveyor.com/api/projects/status/pc63pbl9b0t5tygl/branch/master?svg=true)](https://ci.appveyor.com/project/SizzlingCalamari/demboyz/branch/master) |
| OSX | Clang | [![Build status](https://ci.appveyor.com/api/projects/status/r9ka01wvs8ge5mx9/branch/master?svg=true)](https://ci.appveyor.com/project/SizzlingCalamari/demboyz-osx/branch/master) |

## What is it?

Demboyz is a command line tool to convert TF2 STV demos into a human readable json format. Demboyz runs on Linux, Windows, and OSX.
It supports the following conversions:

    .dem/.json <-> .dem/.json
    .dem/.json -> .con
    .dem/.json -> directory of .wav files (for voice data writer)

    The .dem format is the TF2 STV demo format.
    The .json format is the demboyz streaming json demo format.
    The .con format is a log equal to what would be produced by TF2
    with netmessage/demmessage logs enabled.

## Potential Uses
* Exporting Stats: Parsing for player stats and motion throughout maps.
* Anticheat: Interpreting player net data for malicious behavior and cheating
* Anti-Anticheat: Modifying player net data to hide your malicious behaviour and cheating
* Censoring: Cover up rude communication by removing player voice and chat messages
* Social engineering: Make your friends seem rude by adding phony player voice and chat messages
* ConeBone69 revival: Change the names of all players to ConeBone69

## Usage

    ./demboyz mystvdemo.dem mystvdemo.json
    ./demboyz mystvdemo.dem mystvdemo.con
    ./demboyz mystvdemo.json mystvdemo.dem
    ./demboyz mystvdemo.json mystvdemo.con
    ./demboyz mystvdemo.dem mystvdemofolder (voice data writer)

In the first example, mystvdemo.dem will be read from the current working directory, while mystvdemo.json will be written to the current working directory.

## Contributing

### For Developers

If you would like to contribute to demboyz, here are a few tasks up for grabs:

#### General programming experience:
* Add automated tests to demboyz.
    * Using the Catch C++ testing framework.
    * Unit test the serialization of network messages.
    * System test exact binary matches of dem -> json -> dem transformations.
* Document the .dem format.
    * Turn the working demboyz serialization code into a reference manual for the .dem fomat.
    * Any amount of work on this helps. Document one struct!
* Develop an app that uses demboyz.
    * Provide feedback on the usefulness of the json demo format.
    * Create new tools previously impossible to make.

#### C++ and asm experience:
* Continue reverse engineering the STV demo structs and serialization (svc_ messages).
    * Many of the reversed svc_ messages still have unknown binary chunks of data.
    * Reverse them for addition to the demboyz source.
* Reverse engineer the POV demo structs and serialization (clc_ messages).
    * Currently, demboyz only supports STV demos due to POV demo messages being separate and unknown.
    * Work on reverse engineering the clc_ messages just like the svc_ messages.
* Add a conversion to the replay demo format.
    * The replay demo format would require reverse engineering just like POV and STV demos.

## Compiling From Source

When following instructions below, the compiled binary will be output in the bin folder.

If using voice extraction capabilities, ensure steam is running and logged in.

### OSX & Linux

    cd premake
    ./gmake.sh && cd gmake
    make -j4

### With Visual Studio

    # Generate vs project
    cd premake
    vs2022.bat
    
Open generated VS solution at premake/vs2022/demboyz.sln
