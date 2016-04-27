# demboyz
we dem boyz

## Build Status
| System | Compiler | Status |
| ------ | -------- | ------ |
| Linux 64-bit | G++-4.8.4 | [![Build Status](https://semaphoreci.com/api/v1/sizzlingcalamari/demboyz/branches/master/badge.svg)](https://semaphoreci.com/sizzlingcalamari/demboyz) |
| Windows | VS2015 Update 2 | [![Build status](https://ci.appveyor.com/api/projects/status/pc63pbl9b0t5tygl/branch/master?svg=true)](https://ci.appveyor.com/project/SizzlingCalamari/demboyz/branch/master) |

## What is it?

Demboyz is a command line tool to convert TF2 STV demos into a human readable json format. Demboyz runs on Linux, Windows, and OSX.
It supports the following conversions:

    .dem/.json <-> .dem/.json
    .dem/.json -> .con

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

### For Users

* Cheer on the developers.
* Pressure the developers.
* Become a developer.

## Who Uses demboyz?

* [KZMod Demo Player [beta]](http://xtreme-jumps.eu/e107_plugins/forum/forum_viewtopic.php?359435) by kraster

## Compiling From Source

When following instructions below, the compiled binary will be output in the bin folder.


### Using Vagrant

    # Launch VM and build
    vagrant up

    # Patch VM and re-build
    vagrant provision

    # Manual build
    vagrant ssh
    cd /vagrant/premake
    ./gmake.sh && cd gmake
    make

    # Disconnect session and stop VM
    exit
    vagrant halt

### With Visual Studio 2013

    # Generate vs 2013 project
    cd premake
    vs2013.bat
    
Open generated VS solution at premake/vs2013/demboyz.sln
