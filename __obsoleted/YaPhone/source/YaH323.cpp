#include "stdafx.h"
#include "Yah323.h"

CYaH323EndPoint::CYaH323EndPoint(CYaManager &manager):H323EndPoint(manager), m_manager(manager)
{
	NoAnswerTimer.SetNotifier(PCREATE_NOTIFIER(OnNoAnswerTimeout));
	NoAnswerTimer.SetInterval(0, 60);	/* 60s timeout */
}

YaVoid CYaH323EndPoint::Init()
{
	DisableFastStart(TRUE);
	DisableH245Tunneling(TRUE);
	//AddAllCapabilities(P_MAX_INDEX, P_MAX_INDEX, "*");
	
	SetInitialBandwidth(40000);

	m_manager.Logger("[YaH323]H323EndPoint initializtion finished");
}

YaBool CYaH323EndPoint::StartListener(PString iface , YaWord wPort)
{
	YaBool bOk = FALSE;
	YaChar *listen_to = NULL;
	PINDEX	i = 0;
	PString ip;
	
	PIPSocket::InterfaceTable ifaces;

	RemoveListener(NULL);

	/*PIPSocket::GetInterfaceTable(ifaces);
	while (i < ifaces.GetSize())
	{
		ip = "[" + ifaces[i].GetAddress().AsString() + "]";
		if (ifaces[i].GetName() + ip == iface)
		{
			YaChar szBuf[32] = {0};
			sprintf(szBuf, "tcp$%s/%d", ifaces[i].GetAddress().AsString(), wPort);
			listen_to = _strdup(szBuf);
		}
		i++;
	}*/
	{
		YaChar szBuf[32] = {0};
		YaChar szIP[16] = {0};
		strncpy(szIP, iface, sizeof(szIP));
		sprintf(szBuf, "tcp$%s:%d", "*", wPort);
		listen_to = _strdup(szBuf);
	}
	if (!listen_to)
	{
		m_manager.Logger("[fatal error] listen interface failed!");
		return FALSE;
	}
	
	bOk = StartListeners(PStringArray(listen_to));
	if (bOk)
	{
		m_manager.Logger("[YaH323]H.323 listening on %s ", listen_to);
	}

	return bOk;
}

YaVoid CYaH323EndPoint::SetUserInputMode(enum UserInputMode eMode/* = TONE*/)
{
	switch (eMode)
	{
	case Q931:
		SetSendUserInputMode(OpalConnection::SendUserInputAsQ931);
		break;
	case STRING:
		SetSendUserInputMode(OpalConnection::SendUserInputAsString);
		break;
	case TONE:
		SetSendUserInputMode(OpalConnection::SendUserInputAsTone);
		break;
	case RFC2833:
		SetSendUserInputMode(OpalConnection::SendUserInputAsInlineRFC2833);
		break;
	default:
		break;
	}
}

OpalMediaFormatList CYaH323EndPoint::GetAvailableAudioMediaFormats(YaVoid)
{
	OpalMediaFormatList list;
	OpalMediaFormatList mediaFormats;
	OpalMediaFormatList h323List;

	CYaPCSSEndPoint *pcssEP = m_manager.GetPCSSEndPoint();

	mediaFormats = pcssEP->GetMediaFormats();
	list += OpalTranscoder::GetPossibleFormats(mediaFormats);

	for (YaInt i = 0; i < list.GetSize(); ++i)
	{
		if (list[i].GetDefaultSessionID() == OpalMediaFormat::DefaultAudioSessionID)
		{
			if (PString(list[i].GetEncodingName()).GetLength() > 0)
			{
				if (/*list[i].IsValidForProtocol("H323") && */list[i].GetPayloadType() < RTP_DataFrame::MaxPayloadType)
				{
					h323List += list[i];
				}
			}
		}
	}

	return h323List;
}

OpalMediaFormatList CYaH323EndPoint::GetAvailableVideoMediaFormats(YaVoid)
{
	OpalMediaFormatList list;
	OpalMediaFormatList mediaFormats;
	OpalMediaFormatList h323List;

	CYaPCSSEndPoint *pcssEP = m_manager.GetPCSSEndPoint();

	mediaFormats = pcssEP->GetMediaFormats();
	list += OpalTranscoder::GetPossibleFormats(mediaFormats);

	for (YaInt i = 0; i < list.GetSize(); ++i)
	{
		if (list[i].GetDefaultSessionID() == OpalMediaFormat::DefaultVideoSessionID)
		{
			if (PString(list[i].GetEncodingName()).GetLength() > 0)
			{
				if (/*list[i].IsValidForProtocol("H323") && */list[i].GetPayloadType() < RTP_DataFrame::MaxPayloadType)
				{
					h323List += list[i];
				}
			}
		}
	}

	return h323List;
}

YaBool CYaH323EndPoint::OnIncomingConnection(OpalConnection &conn)
{
	return m_manager.OnIncomingConnection(conn);
}

BOOL CYaH323EndPoint::OnAlerting(H323Connection & connection, const H323SignalPDU & alertingPDU, const PString & user)
{
	m_manager.OnAlerting(connection);

	return TRUE;
}

YaVoid CYaH323EndPoint::OnEstablished(OpalConnection &conn)
{
	NoAnswerTimer.Stop();

	H323EndPoint::OnEstablished(conn);
}

YaVoid CYaH323EndPoint::OnReleased(OpalConnection &conn)
{
	NoAnswerTimer.Stop();
	
	H323EndPoint::OnReleased(conn);
}

YaVoid CYaH323EndPoint::OnRegistrationConfirm()
{
	m_manager.OnRegistrationConfirm();
}

YaVoid CYaH323EndPoint::OnRegistrationReject()
{
	m_manager.OnRegistrationReject();
}

YaVoid CYaH323EndPoint::OnNoAnswerTimeout(PTimer & note, INT extra)
{
	ClearAllCalls(OpalConnection::EndedByNoAnswer, FALSE);
}
