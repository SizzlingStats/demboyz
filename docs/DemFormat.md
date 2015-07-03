Source Engine DEM format
=======

This document applies to the version of TF2 as of July 1, 2015.  
Demo Protocol v3.  
Network Protocol v24.  

#### DEM Format
The Source Engine DEM format at a high level:

[Demo Header]  
[Command Header][Command Packet]  
[Command Header][Command Packet]  
...  
[Command Header = dem_stop]  
~fin

#### Demo Header
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

#### Command Header
| Type | Name | Description |
|------|------|-------------|
| int8 | Command Type | Type of the next command packet. |
| int32 | Tick | Game tick of the next command packet. |

#### Command Type
| Name | Value | Description |
|------|-------|-------------|
| dem_signon | 1 | Signon packet. Same format as a network packet. |
| dem_packet | 2 | Network packet. |
| dem_synctick | 3 | Signals the client to sync clock to demo tick. |
| dem_consolecmd | 4 | Console command. |
| dem_usercmd | 5 | User input command. |
| dem_datatables | 6 | Network data tables. |
| dem_stop | 7 | Signals the end of demo messages. |
| dem_stringtables | 8 | Stringtables. |

#### Command Packets
##### dem_signon/dem_packet
| Type | Name | Description |
|------|------|-------------|
| democmdinfo_t | Demo Command Info | Describes view angles of the current point of view. |
| int32 | Sequence Number In | Input sequence number of packet. |
| int32 | Sequence Number Out | Output sequence number of packet. |
| RawData | Netpackets | RawData structure containing a series of netpackets. |

##### dem_synctick
Command Packet of size 0 bytes.

##### dem_consolecmd
| Type | Name | Description |
|------|------|-------------|
| char[1023] | Command String | Console command string of max length 1023 (including null char). |

Note: A typo in the code that writes out console commands likely meant for a string of length 1024 (including null char).

##### dem_usercmd
##### dem_datatables
##### dem_stop
Command Packet of size 0 bytes.

##### dem_stringtables

#### Types Appendix
##### Vector
| Type | Name | Description |
|------|------|-------------|
| float32 | x | |
| float32 | y | |
| float32 | z | |

##### QAngle
| Type | Name | Description |
|------|------|-------------|
| float32 | pitch | |
| float32 | yaw | |
| float32 | roll | |

##### Split_t
| Type | Name | Description |
|------|------|-------------|
| int32 | Flags | |
| Vector | View Origin | |
| QAngle | View Angles | |
| QAngle | Local View Angles | |
| Vector | View Origin 2 | |
| QAngle | View Angles 2 | |
| QAngle | Local View Angles 2 | |

##### democmdinfo_t
| Type | Name | Description |
|------|------|-------------|
| Split_t[1] | POV angles | Array of Split_t. Constant length of 1 for demo protocol 3. |

##### RawData
| Type | Name | Description |
|------|------|-------------|
| int32 | Size | Size in bytes of the following data chunk. |
| uchar[Size] | Data | Data chunk of Size bytes. |
