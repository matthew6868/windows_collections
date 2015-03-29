#include "stdafx.h"
#include "YaPcss.h"

extern "C" {
	unsigned char linear2ulaw(int pcm_val);
	int ulaw2linear(unsigned char u_val);
};

CYaAudioSignalFilter::CYaAudioSignalFilter(CYaManager &manager)
: receiveHandler(PCREATE_NOTIFIER(ReceivePacket)),sendHandler(PCREATE_NOTIFIER(SendPacket)), m_manager(manager)
{
	//
}

YaUint32 CYaAudioSignalFilter::GetAverageSignalLevel(const YaUint8 *buffer, YaInt size)
{
	// Calculate the average signal level of this frame
	YaUint	intLevel;
	YaInt	sum = 0;
	PINDEX	samples = size / 2;
	const YaInt16 * pcm = (const short *)buffer;
	const YaInt16 * end = pcm + samples;
	while (pcm != end) 
	{
		if (*pcm < 0)
			sum -= *pcm++;
		else
			sum += *pcm++;
	}

	/*(YaUint32)(log10 (9.0*sum/size/32767+1)*1.0);*/
	intLevel = (YaUint)sum/samples;
	// Convert to a logarithmic scale - use uLaw which is complemented
	intLevel = linear2ulaw(intLevel) ^ 0xff;
	// filter out some noise
	if(intLevel < 15) 
	{
		intLevel = 0;
	}

	return intLevel;
}

YaVoid CYaAudioSignalFilter::ReceivePacket(RTP_DataFrame & frame, INT extra)
{
	static YaUint32 cnt = 0;
	if (cnt ++ % 6)
		return;

	// Silent
	if (frame.GetPayloadSize() == 0)
		return;

	YaUint32 playVol;
	
	playVol = (YaUint32)GetAverageSignalLevel(frame.GetPayloadPtr(), frame.GetPayloadSize());
	//m_manager.Logger("playVol = %d", playVol);
	m_manager.UpdateVolumeIndicate(playVol, TRUE);
}

YaVoid CYaAudioSignalFilter::SendPacket(RTP_DataFrame & frame, INT extra)
{
	static YaUint32 cnt = 0;
	if (cnt ++ % 6)
		return;

	// Silent
	if (frame.GetPayloadSize() == 0)
		return;

	YaUint32 recordVol;

	recordVol = GetAverageSignalLevel(frame.GetPayloadPtr(), frame.GetPayloadSize());
	//m_manager.Logger("recordVol = %d", recordVol);
	m_manager.UpdateVolumeIndicate(recordVol, FALSE);
}

///////////////////////////////////////////////////////////////////////////////
CYaPCSSEndPoint::CYaPCSSEndPoint(CYaManager &manager):OpalPCSSEndPoint(manager), m_manager(manager)
{
	m_curIncomingConnectionToken = PString();
	m_audioSignalFiter = new CYaAudioSignalFilter(manager);
}

PString CYaPCSSEndPoint::OnGetDestination(const OpalPCSSConnection & connection)
{
	PString str("unknown");

	m_manager.Logger("[YaPcss]OnGetDestination");

	return str;
}

YaVoid CYaPCSSEndPoint::OnShowIncoming(const OpalPCSSConnection &connection)
{
	YaChar	szReadableToken[64] = {0};

	m_curIncomingConnectionToken = connection.GetToken();
	strncpy(szReadableToken, m_curIncomingConnectionToken, m_curIncomingConnectionToken.GetLength());

	m_manager.Logger("[YaPcss]OnShowIncomming with token[%s]", szReadableToken);
	m_manager.OnRing(connection);
}

YaBool CYaPCSSEndPoint::OnShowOutgoing(const OpalPCSSConnection & connection)
{
	PString token;
	YaChar	szReadableToken[256] = {0};

	token = connection.GetToken();
	strncpy(szReadableToken, token, token.GetLength());

	m_manager.Logger("[YaPcss]OnShowOutgoing with token[%s]", szReadableToken);

	return TRUE;
}

YaVoid CYaPCSSEndPoint::OnReleased(OpalConnection & connection)
{
	//Do nothing
	OpalPCSSEndPoint::OnReleased(connection);
}

YaVoid CYaPCSSEndPoint::OnPatchMediaStream(const OpalPCSSConnection & connection, BOOL isSource, OpalMediaPatch & patch)
{
	OpalPCSSEndPoint::OnPatchMediaStream(connection, isSource, patch);

	if (OpalMediaFormat::DefaultAudioSessionID == patch.GetSource().GetSessionID())
	{
		patch.AddFilter(isSource ? m_audioSignalFiter->GetReceiveHandler() : m_audioSignalFiter->GetSendHandler(), OpalPCM16);
	}
}

YaVoid CYaPCSSEndPoint::AcceptCurrentIncomingCall(YaVoid)
{
	if (!m_curIncomingConnectionToken.IsEmpty())
	{
		AcceptIncomingConnection(m_curIncomingConnectionToken);
		m_curIncomingConnectionToken = PString();
	}
}

YaBool CYaPCSSEndPoint::SetDeviceVolume(YaUint32 playVol, YaUint32 recordVol)
{
	if (playVol > 100 || recordVol > 100)
		return FALSE;

	PSafePtr<OpalCall>			call			= NULL;
	PSafePtr<OpalConnection>	connection		= NULL;
	OpalAudioMediaStream		*audioStream	= NULL;
	PSoundChannel				*audioChannel	= NULL;
	YaBool						bSuccess1		= FALSE;
	YaBool						bSuccess2		= FALSE;

	call = m_manager.FindCallWithLock(m_manager.GetCurrentCallToken());
	if (!call)
		return FALSE;

	connection = call->GetConnection(0);
	if (!connection ||
		!PIsDescendant(&(*connection), OpalPCSSConnection))
	{
		connection = call->GetConnection(1);
	}
	if (connection)
	{
		audioStream = (OpalAudioMediaStream*)connection->GetMediaStream(OpalMediaFormat::DefaultAudioSessionID, FALSE);
		if (audioStream)
		{
			audioChannel = (PSoundChannel*)audioStream->GetChannel();
			if (audioChannel)
			{
				audioChannel->SetVolume(playVol);
				bSuccess1 = TRUE;
			}
		}

		audioStream = (OpalAudioMediaStream*)connection->GetMediaStream(OpalMediaFormat::DefaultAudioSessionID, TRUE);
		if (audioStream)
		{
			audioChannel = (PSoundChannel*)audioStream->GetChannel();
			if (audioChannel)
			{
				audioChannel->SetVolume(recordVol);
				bSuccess2 = TRUE;
			}
		}
	}
	
	return bSuccess1 && bSuccess2;
}

YaVoid CYaPCSSEndPoint::GetDeviceVolume(YaUint32 &playVol, YaUint32 &recordVol)
{
	playVol		= 100;
	recordVol	= 100;
}

PSoundChannel *CYaPCSSEndPoint::CreateSoundChannel(const OpalPCSSConnection & connection, const OpalMediaFormat & mediaFormat, BOOL isSource)
{
	YaUint32		recordVol;
	YaUint32		playVol;
	PSoundChannel	*soundChannel = OpalPCSSEndPoint::CreateSoundChannel(connection, mediaFormat, isSource);

	if (isSource)
	{
		soundChannel->GetVolume(recordVol);
		if (recordVol > 100)recordVol = 100;
		m_manager.UpdateVolumeSlider(recordVol, TRUE);
	}
	else
	{
		soundChannel->GetVolume(playVol);
		if (playVol > 100)playVol = 100;
		m_manager.UpdateVolumeSlider(playVol, FALSE);
	}

	return soundChannel;
}