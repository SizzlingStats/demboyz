Source Engine DEM format
=======

This document applies to the version of TF2 as of July 1, 2015.

### DEM Format
The Source Engine DEM format at a high level:

[Demo Header]  
[Command Header][Command Packet]  
[Command Header][Command Packet]  
...  
[Command Header = dem_stop]  
~fin

### Demo Header
| Type | Name | Description | Example |
|------|------|-------------|---------|
| char[8] | Demo File Stamp | Demo file identifier. | "HL2DEMO" |
| int32 | Demo Protocol | Demo protocol version. | 3 |
| int32 | Network Protocol | Network protocol version. | 24 |
| char[260] | Server Name | Name of the server that recorded this demo. | "Team Fortress" |
| char[260] | Client Name | Name of the client that recorded this demo. | "SourceTV Demo" |
| char[260] | Map Name | Name of the map where this demo was recorded. | "cp_granary" |
| char[260] | Game Directory | Game directory relative to hl2.exe | "tf" |
| float32 | Playback Time | Length of the demo in seconds. | 9.05999947 |
| int32 | Playback Ticks | Length of the demo in ticks. | 604 |
| int32 | Playback Frames | Length of the demo in frames. | 151 |
| int32 | Signon Length | Length of the signon data in bytes. | 640166 |

Note: Examples are from a short demo recording, hence the 9 second playback time.

### Command Header
| Type | Name | Description |
|------|------|-------------|
| int8 | Command Type | Type of the next command packet. |
| int32 | Tick | Game tick of the next command packet. |

### Command Type
| Type | Value | Description |
|------|-------|-------------|
| dem_signon | 1 | Signon packet. Same format as a network packet. |
| dem_packet | 2 | Network packet. |
| dem_synctick | 3 | Signals the client to sync clock to demo tick. |
| dem_consolecmd | 4 | Console command. |
| dem_usercmd | 5 | User input command. |
| dem_datatables | 6 | Network data tables. |
| dem_stop | 7 | Signals the end of demo messages. |
| dem_stringtables | 8 | Stringtables. |

