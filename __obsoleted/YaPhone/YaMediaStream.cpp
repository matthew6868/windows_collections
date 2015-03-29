#include "StdAfx.h"
#include "YaMediaStream.h"

CYaMediaStream::CYaMediaStream(const OpalMediaFormat & mediaFormat, BOOL isSource)
:OpalMediaStream(mediaFormat, OpalMediaFormat::DefaultAudioSessionID, isSource), dataFrame((isSource) ? 3000 : 0)
{
}

CYaMediaStream::~CYaMediaStream(void)
{
}

BOOL CYaMediaStream::ReadData(BYTE * data, PINDEX size, PINDEX & length )
{
	return TRUE;
}

BOOL CYaMediaStream::ReadPacket(RTP_DataFrame & packet )
{
	return TRUE;
}