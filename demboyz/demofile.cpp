//====== Copyright (c) 2014, Valve Corporation, All rights reserved. ========//
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are met:
//
// Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
// Redistributions in binary form must reproduce the above copyright notice, 
// this list of conditions and the following disclaimer in the documentation 
// and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
// THE POSSIBILITY OF SUCH DAMAGE.
//===========================================================================//

#include <stdio.h>
#include <cstring>
#include <assert.h>
#include "demofile.h"

DemoSequenceReader::DemoSequenceReader(const std::vector<unsigned char>& sequenceData):
    m_sequenceData(sequenceData),
    m_dataReadOffset(0)
{
}

int32 DemoSequenceReader::ReadRawData(char *buffer, int32 length)
{
    if (m_sequenceData.empty())
    {
        return 0;
    }

    const unsigned char* sequenceData = m_sequenceData.data();
    size_t currentReadOffset = m_dataReadOffset;

    // read length of data block
    const int32 size = *reinterpret_cast<const int32*>(sequenceData + currentReadOffset);
    currentReadOffset += sizeof(int32);

    if (buffer && (length < size))
    {
        fprintf(stderr, "CDemoFile::ReadRawData: buffer overflow (%i).\n", size);
        return -1;
    }

    if (buffer)
    {
        // read data into buffer
        memcpy(buffer, sequenceData + currentReadOffset, size);
    }
    currentReadOffset += size;

    m_dataReadOffset = currentReadOffset;
    return size;
}

bool DemoSequenceReader::ReadRawData(std::vector<unsigned char>& buf,
                                     const int32 maxReadSize /*= MAX_READ_SIZE*/)
{
    if (m_sequenceData.empty())
    {
        return false;
    }

    const unsigned char* sequenceData = m_sequenceData.data();
    size_t currentReadOffset = m_dataReadOffset;

    // read length of data block
    const int32 size = *reinterpret_cast<const int32*>(sequenceData + currentReadOffset);
    currentReadOffset += sizeof(int32);

    if (size < 0)
    {
        fprintf(stderr, "DemoSequenceReader::ReadRawData: invalid size (%i).\n", size);
        return false;
    }
    if (maxReadSize < 0 || size > maxReadSize)
    {
        fprintf(stderr, "DemoSequenceReader::ReadRawData: invalid size (%i) with max (%i).\n", size, maxReadSize);
        return false;
    }

    buf.resize(size);

    // read data into buffer
    memcpy(buf.data(), sequenceData + currentReadOffset, size);
    currentReadOffset += size;

    m_dataReadOffset = currentReadOffset;
    return true;
}

void DemoSequenceReader::ReadSequenceInfo(int32 &nSeqNrIn, int32 &nSeqNrOut)
{
    if (m_sequenceData.empty())
    {
        return;
    }

    const unsigned char* sequenceData = m_sequenceData.data();
    size_t currentReadOffset = m_dataReadOffset;

    nSeqNrIn = *reinterpret_cast<const int32*>(sequenceData + currentReadOffset);
    currentReadOffset += sizeof(int32);
    nSeqNrOut = *reinterpret_cast<const int32*>(sequenceData + currentReadOffset);
    currentReadOffset += sizeof(int32);

    m_dataReadOffset = currentReadOffset;
}

void DemoSequenceReader::ReadCmdInfo(democmdinfo_t& info)
{
    if (m_sequenceData.empty())
    {
        return;
    }

    size_t currentReadOffset = m_dataReadOffset;

    memcpy(&info, m_sequenceData.data() + currentReadOffset, sizeof(democmdinfo_t));
    currentReadOffset += sizeof(democmdinfo_t);

    m_dataReadOffset = currentReadOffset;
}

void DemoSequenceReader::ReadCmdHeader(unsigned char& cmd, int32& tick)
{
    if (m_sequenceData.empty())
    {
        return;
    }

    const unsigned char* sequenceData = m_sequenceData.data();
    size_t currentReadOffset = m_dataReadOffset;

    // Read the command
    cmd = sequenceData[currentReadOffset];
    currentReadOffset += sizeof(unsigned char);

    if (cmd > 0)
    {
        assert(cmd <= dem_lastcmd);

        // Read the timestamp
        tick = *reinterpret_cast<const int32*>(sequenceData + currentReadOffset);
        currentReadOffset += sizeof(int32);
    }
    else
    {
        fprintf(stderr, "CDemoFile::ReadCmdHeader: Missing end tag in demo file.\n");
        cmd = dem_stop;
    }
    m_dataReadOffset = currentReadOffset;
}

int32 DemoSequenceReader::ReadUserCmd(std::vector<unsigned char>& buf)
{
    if (m_sequenceData.empty())
    {
        return 0;
    }

    const int32 outgoing_sequence = *reinterpret_cast<const int32*>(m_sequenceData.data() + m_dataReadOffset);
    m_dataReadOffset += sizeof(int32);

    if (!ReadRawData(buf))
    {
        return 0;
    }
    return outgoing_sequence;
}

CDemoFile::CDemoFile():
    m_DemoHeader()
{
}

CDemoFile::~CDemoFile()
{
    Close();
}

bool CDemoFile::Open( const char *name )
{
    Close();

    FILE *fp = NULL;
    fp = fopen( name, "rb" );
    if( fp )
    {
        size_t Length;

        fseek( fp, 0, SEEK_END );
        Length = ftell( fp );
        fseek( fp, 0, SEEK_SET );

        if( Length < sizeof( m_DemoHeader ) )
        {
            fprintf( stderr, "CDemoFile::Open: file too small. %s.\n", name );
            fclose( fp );
            return false;
        }

        fread( &m_DemoHeader, 1, sizeof( m_DemoHeader ), fp );
        Length -= sizeof( m_DemoHeader );

        if ( strcmp ( m_DemoHeader.demofilestamp, DEMO_HEADER_ID ) )
        {
            fprintf(stderr, "CDemoFile::Open: %s has invalid demo header ID.\n", name);
            fclose( fp );
            return false;
        }

        if ( m_DemoHeader.demoprotocol != DEMO_PROTOCOL )
        {
            fprintf( stderr, "CDemoFile::Open: demo file protocol %i invalid, expected version is %i \n", m_DemoHeader.demoprotocol, DEMO_PROTOCOL );
            fclose( fp );
            return false;
        }

        const int32 signOnLength = m_DemoHeader.signonlength;
        if (signOnLength > 0)
        {
            m_signOnData.resize(signOnLength);
            fread(&m_signOnData[0], 1, signOnLength, fp);
            Length -= signOnLength;
        }

        m_fileBuffer.resize( Length );
        fread( &m_fileBuffer[ 0 ], 1, Length, fp );

        fclose( fp );
        fp = NULL;
    }

    if ( !m_fileBuffer.size() )
    {
        fprintf( stderr, "CDemoFile::Open: couldn't open file %s.\n", name );
        Close();
        return false;
    }
    return true;
}

void CDemoFile::Close()
{
    m_signOnData.clear();
    m_fileBuffer.clear();
}
