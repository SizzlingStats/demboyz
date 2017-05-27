
#pragma once

enum constants
{
    // was 5
    NETMSG_TYPE_BITS = 6,           // 2^NETMSG_TYPE_BITS > SVC_LASTMSG

    // was 96000
    NET_MAX_PAYLOAD = 288000,       // largest message size in bytes

    NET_MAX_PAYLOAD_BITS_OLD = 17,  // for old demos
    NET_MAX_PAYLOAD_BITS = 19,      // 2^NET_MAX_PAYLOAD_BITS > NET_MAX_PAYLOAD

    // table index is sent in log2(MAX_TABLES) bits
    MAX_TABLES = 32,                // Table id is 4 bits

    // How many bits to use to encode an edict.
    MAX_EDICT_BITS = 11,            // # of bits needed to represent max edicts

    // Max # of edicts in a level
    MAX_EDICTS = (1 << MAX_EDICT_BITS),
    MAX_DECAL_INDEX_BITS = 9,

    SP_MODEL_INDEX_BITS_OLD = 11,
    SP_MODEL_INDEX_BITS = 13,

    MAX_SERVER_CLASS_BITS = 9,

    MAX_EVENT_NAME_LENGTH = 32,
    MAX_EVENT_BITS = 9,
    MAX_EVENT_NUMBER = (1 << MAX_EVENT_BITS),
    MAX_EVENT_BYTES = 1024,

    DELTASIZE_BITS = 20,             // must be: 2^DELTASIZE_BITS > (NET_MAX_PAYLOAD * 8)
    EVENT_INDEX_BITS = 8,

    MAX_SOUND_INDEX_BITS_OLD = 13,
    MAX_SOUND_INDEX_BITS = 14,

    MAX_USER_MSG_DATA = 255,

    SIGNONSTATE_NONE = 0, // no state yet, about to connect
    SIGNONSTATE_CHALLENGE = 1, // client challenging server, all OOB packets
    SIGNONSTATE_CONNECTED = 2, // client is connected to server, netchans ready
    SIGNONSTATE_NEW = 3, // just got serverinfo and string tables
    SIGNONSTATE_PRESPAWN = 4, // received signon buffers
    SIGNONSTATE_SPAWN = 5, // ready to receive entity packets
    SIGNONSTATE_FULL = 6, // we are fully connected, first non-delta packet received
    SIGNONSTATE_CHANGELEVEL = 7,  // server is changing level, please wait

    MAX_STRINGTABLE_DATA = 524288   // 2^19
};
