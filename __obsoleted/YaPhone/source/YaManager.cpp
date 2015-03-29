//
#include "stdafx.h"
#include "../YaPhone.h"
#include "../YaPhoneDlg.h"
#include "YaManager.h"
#include "opal/mediafmt.h"
//

YaInt CYaManager::_instance = 0;

YaBool CYaManager::Initialize(YaVoid *Param)
{
	OpalEchoCanceler::Params ec;
	OpalSilenceDetector::Params sd;
	OpalMediaStreamList list;

	if (!Param)
		return FALSE;

	//
	PTrace::Initialise(10, "opal_stack.log", PTrace::FileAndLine | PTrace::DateAndTime| PTrace::Blocks |  PTrace::Thread /*|  PTrace::TraceLevel*/);
	//

	m_pDlg = (CYaPhoneDlg*)Param;

	/* H.323 Endpoint */
	m_h323EP = new CYaH323EndPoint(*this);
	m_h323EP->Init();
	AddRouteEntry("pc:.*             = h323:<da>");

	/* PC Sound System Endpoint */
	m_pcssEP = new CYaPCSSEndPoint(*this);
	AddRouteEntry("h323:.* = pc:<da>");

	//
	SetAudioJitterDelay(20, 1000);
	//

	//
	sd = GetSilenceDetectParams();
	if (1)
		sd.m_mode = OpalSilenceDetector::AdaptiveSilenceDetection;
	else
		sd.m_mode = OpalSilenceDetector::NoSilenceDetection;
	SetSilenceDetectParams(sd);
	//

	//
	ec = GetEchoCancelParams();
	if (1)
		ec.m_mode = OpalEchoCanceler::Cancelation;
	else
		ec.m_mode = OpalEchoCanceler::NoCancelation;
	SetEchoCancelParams(ec);
	//

	m_h323EP->StartListener("127.0.0.1", 1720);
	//m_h323EP->StartListeners(PStringArray("*"));
	
	AvgSignalTimer.SetNotifier(PCREATE_NOTIFIER(OnAvgSignalLevelTimeout));

	return TRUE;
}

YaVoid CYaManager::SetLogHandler(fManagerLogFunc logHandler)
{
	m_fLogFunc = logHandler;

	Logger("[YaManager]Set Log handle successful,the current address is[0x%p]", m_fLogFunc);
}

YaVoid CYaManager::Logger(const YaChar *fmt, ...)
{
	YaChar szLog[256] = {0};

	if (m_fLogFunc)
	{
		va_list argList;
		va_start(argList, fmt);
		vsprintf(szLog, fmt, argList);
		va_end(argList);

		m_fLogFunc(LOG_DEBUG, szLog);
	}
}

YaBool CYaManager::SetStackLogFile(const char *fileName)
{
	return FALSE;
}

YaBool CYaManager::SetStackDebugLevel(YaInt debugLevel)
{
	return FALSE;
}

CYaH323EndPoint *CYaManager::GetH323EndPoint(YaVoid)
{
	return m_h323EP;
}

CYaPCSSEndPoint *CYaManager::GetPCSSEndPoint(YaVoid)
{
	return m_pcssEP;
}

CYaPhoneDlg *CYaManager::GetMainDlg(YaVoid)
{
	return m_pDlg;
}

PString CYaManager::GetCurrentCallToken(YaVoid)
{
	return m_curCallToken;
}

YaInt CYaManager::GetCurrentCallID(YaVoid)
{
	return m_nCallID;
}

YaVoid CYaManager::UpdateVolumeSlider(YaUint32 volume, YaBool bRecord)
{
	m_pDlg->UpdateVolumeSlider(volume, bRecord);
}

YaVoid CYaManager::UpdateVolumeIndicate(YaUint32 volume, YaBool bRecord)
{
	m_pDlg->UpdateVolumeIndicate(volume, bRecord);
}

YaVoid CYaManager::GetRemoteConnectionInfo(OpalConnection &connection, PString &name, 
										   PString &app, PString &url)
{
	PINDEX idx;

	PString remoteUrl;
	PString remoteName;
	PString remoteApp;
	PString remoteAlias;

	/* Get information about the remote user */
	remoteName = connection.GetRemotePartyName ();
	idx = remoteName.Find ("(");
	if (idx != P_MAX_INDEX) 
	{
		remoteAlias = remoteName.Mid (idx + 1);
		remoteAlias =
			remoteAlias.Mid (0, (remoteAlias.Find (",") != P_MAX_INDEX) ?
			remoteAlias.Find (",") : remoteAlias.Find (")"));
		remoteName = remoteName.Left (idx);
	}
	idx = remoteName.Find ("[");
	if (idx != P_MAX_INDEX)
		remoteName = remoteName.Left (idx);
	idx = remoteName.Find ("@");
	if (idx != P_MAX_INDEX)
		remoteName = remoteName.Left (idx);

	/* The remote application */
	remoteApp = connection.GetRemoteApplication (); 
	idx = remoteApp.Find ("(");
	if (idx != P_MAX_INDEX)
		remoteApp = remoteApp.Left (idx);
	idx = remoteApp.Find ("[");
	if (idx != P_MAX_INDEX)
		remoteApp = remoteApp.Left (idx);
	
	/* The remote url */
	remoteUrl = connection.GetRemotePartyCallbackURL ();

	name	= remoteName;
	url		= remoteUrl;
	app		= remoteApp;
}

PSafePtr<OpalConnection> CYaManager::GetConnection(PSafePtr<OpalCall> call, YaBool isRemote)
{
	PSafePtr<OpalConnection> connection = NULL;

	if (!call)
		return NULL;

	connection = call->GetConnection(isRemote ? 1 : 0);
	if (!connection ||
		(isRemote && PIsDescendant(&(*connection), OpalPCSSConnection)) ||
		(!isRemote && !PIsDescendant(&(*connection), OpalPCSSConnection)))
	{
		connection = call->GetConnection(isRemote ? 0 : 1);
	}

	return connection;
}

YaBool CYaManager::SendTextMessage(PString token, PString message)
{
	PSafePtr<OpalCall>			call		= NULL;
	PSafePtr<OpalConnection>	connection	= NULL;

	call = FindCallWithLock(token);
	if (!call)
		return FALSE;

	connection = GetConnection(call, TRUE);
	if (connection)
	{
		connection->SendUserInputString(message);
		return TRUE;
	}

	return FALSE;
}

YaBool CYaManager::CallMake(const YaChar *destination, YaInt &nCallID)
{
	YaBool	status = FALSE;

	if (NULL == destination)
		return -1;

	status = OpalManager::SetUpCall("pc:*", destination, m_curCallToken);
	if (TRUE == status)
	{
		m_nCallID = nCallID = m_curCallToken.AsInteger();
		Logger("[YaManager]Setup call successful,waiting callee response...current call ID[%d]", m_nCallID);
		m_pDlg->SetCallState(Calling);
	}
	else
	{
		nCallID = 0;
		Logger("[YaManager]Setup call failed");
		m_pDlg->SetCallState(StandBy);
	}
	
	return status;
}

YaBool CYaManager::CallDrop(YaInt nCallID)
{
	YaBool	status = FALSE;

	status = ClearCall(PString(nCallID), OpalConnection::EndedByLocalUser);
	if (TRUE == status)
	{
		m_curCallToken = PString();
		m_nCallID = 0;
	}

	return status;
}

YaBool CYaManager::CallReject(YaInt nCallID)
{
	YaBool status;

	status = ClearCall(PString(nCallID), OpalConnection::EndedByAnswerDenied);
	if (TRUE == status)
	{
		Logger("[YaManager]Refuse incoming call£¡");
	}

	return TRUE;
}

YaBool CYaManager::CallAnswer(YaInt nCallID)
{
	OpalCall	*call	= NULL;
	
	call = FindCallWithLock(PString(nCallID));
	if (call)
	{
		//call->AnsweringCall(OpalConnection::AnswerCallNow);
	}
	else
	{
		Logger("[YaManager]Answer this call £Û%d£Ýfailed£¡", nCallID);
	}
	if (m_pcssEP)
		m_pcssEP->AcceptCurrentIncomingCall();

	return TRUE;
}

YaBool CYaManager::OpenLogicalChannel(YaVoid)
{
	return FALSE;
}

YaBool CYaManager::OpenLogicalChannelAck(YaVoid)
{
	return FALSE;
}

YaBool CYaManager::OpenLogicalChannelReject(YaInt callId, YaInt channelId)
{
	return FALSE;
}

YaBool CYaManager::RegisterGatekeeper(PString szGKIP, PString szUserName, PString szTelNo, PString szPassword)
{
	if (m_bIsRegister)
		return FALSE;

	if (!szPassword.IsEmpty())
		m_h323EP->SetGatekeeperPassword(szPassword);
	SetUserNameAndAlias(szUserName, szTelNo);
	SetDefaultDisplayName(szUserName);

	m_h323EP->UseGatekeeper(szGKIP);
	m_h323EP->SetGatekeeperAddress(szGKIP);

	PString			msg = PString();
	H323Gatekeeper	*gatekeeper = m_h323EP->GetGatekeeper();
	if (gatekeeper)
	{
		switch (gatekeeper->GetRegistrationFailReason())
		{
		case H323Gatekeeper::RegistrationSuccessful:
			msg = PString("Successfully registered with Gatekeeper: ");
			msg += gatekeeper->GetName();
			break;
		case H323Gatekeeper::DuplicateAlias :
			msg = PString("Gatekeeper registration failed: duplicate alias");
			break;
		case H323Gatekeeper::SecurityDenied :
			msg =PString("Gatekeeper registration failed: bad username/password");
			break;
		case H323Gatekeeper::TransportError :
			msg = PString("Gatekeeper registration failed: transport error");
			break;
		default :
			msg = PString("Gatekeeper registration failed");
			break;
		}
	}
	else
	{
		msg = PString("Gatekeeper registration failed!");
	}

	Logger((const YaChar*)msg);
	return TRUE;
}

YaBool CYaManager::UnregisterGatekeeper(YaVoid)
{
	if (FALSE == m_bIsRegister)
		return FALSE;
	
	if (m_h323EP->RemoveGatekeeper(0))
	{
		Logger("Unregister Gatekeeper.");
		m_bIsRegister = FALSE;
	}

	return TRUE;
}

YaVoid CYaManager::OnRegistrationConfirm(YaVoid)
{
	m_bIsRegister = TRUE;
}

YaVoid CYaManager::OnRegistrationReject(YaVoid)
{
	m_bIsRegister = FALSE;
}

YaBool CYaManager::IsRegisteredWithGatekeeper(YaVoid)
{
	//return m_bIsRegister;
	return m_h323EP->IsRegisteredWithGatekeeper();
}

void CYaManager::SetUserNameAndAlias(PString userName, PString aliasName)
{
	//m_h323EP->SetDefaultLocalPartyName(userName);
	m_h323EP->SetDefaultDisplayName(userName);//Q.931 display name
	m_h323EP->SetLocalUserName(userName);//H.323-ID
	//m_h323EP->AddAliasName(userName);
	m_h323EP->AddAliasName(aliasName);
}

YaChar *CYaManager::GetRemoteAddrInfo(YaInt nCallID)
{
	return "NULL";
}

YaVoid CYaManager::SetTCPPortRange(YaUint16 nPortFrom, YaUint16 nPortTo)
{
	Logger("[YaManager]Unimplemented at present£¡");
}

YaVoid CYaManager::SetRTPPortRange(YaUint16 nPortFrom, YaUint16 nPortTo)
{
	Logger("[YaManager]Unimplemented at present£¡");
}

YaVoid CYaManager::SetRTPPort(YaInt callId, YaUint16 nDataPort, YaUint16 nControlPort)
{
	Logger("[YaManager]Unimplemented at present£¡");
}

YaVoid CYaManager::OnMediaStream(OpalMediaStream &stream, YaBool bClosing)
{
	//update GUI
	Logger("[YaManager]Update GUI£¡");
}

YaVoid CYaManager::SetAllMediaFormats(YaVoid)
{
	SetAudioMediaFormats();
	SetVideoMediaFormats();
	SetUserInputMode();
}

YaVoid CYaManager::SetAudioMediaFormats(YaVoid)
{
	OpalMediaFormatList mediaFormats;
	OpalMediaFormatList list;
	PStringArray order, mask;
	PStringArray initialOrder, initialMask;
	
	initialOrder	= OpalManager::GetMediaFormatOrder();
	initialMask		= OpalManager::GetMediaFormatMask();

	/* Get all the media formats */
	mediaFormats = m_pcssEP->GetMediaFormats();
	list += OpalTranscoder::GetPossibleFormats(mediaFormats);
	
	//mask codecs that were not allow added by GUI.
	for (YaInt i = 0; i < list.GetSize(); i++)
	{
		if (list[i].GetDefaultSessionID() == OpalMediaFormat::DefaultAudioSessionID) //audio default session id equals to 1
		{
			//only support G.711.mu & G.711.A audio codec now.
			if (RTP_DataFrame::PCMU == list[i].GetPayloadType()
				|| RTP_DataFrame::PCMA == list[i].GetPayloadType())
			{
				order += list[i];
			}
			else
			{
				mask += list[i];
			}
		}
	}
	
	for (YaInt i = 0; i < initialMask.GetSize(); ++i)
	{
		if (OpalMediaFormat::DefaultAudioSessionID == OpalMediaFormat(initialMask[i]).GetDefaultSessionID())
			mask += initialMask[i];
	}
	for (YaInt i =0; i < initialOrder.GetSize(); ++i)
	{
		if (OpalMediaFormat::DefaultAudioSessionID == OpalMediaFormat(initialOrder[i]).GetDefaultSessionID())
			order += initialOrder[i];
	}

	//update the order and mask
	SetMediaFormatOrder(order);
	SetMediaFormatMask(mask);
}

YaVoid CYaManager::SetVideoMediaFormats(YaVoid)
{
	//
	static PString FrameWidthOption				= "Frame Width";
	static PString FrameHeightOption			= "Frame Height";
	static PString EncodingQualityOption		= "Encoding Quality";
	static PString TargetBitRateOption			= "Target Bit Rate";
	static PString DynamicVideoQualityOption	= "Dynamic Video Quality";
	static PString AdaptivePacketDelayOption	= "Adaptive Packet Delay";
	//

	YaInt size		= 0;
	YaInt quality	= 1;
	YaInt bitrate	= 512;
	YaInt height	= YA_CIF_HEIGHT;
	YaInt width		= YA_CIF_WIDTH;
	PStringArray order,mask;
	PStringArray initialOrder, initialMask;
	OpalMediaFormatList mediaFormats;
	OpalMediaFormatList list;

	initialMask		= OpalManager::GetMediaFormatMask();
	initialOrder	= OpalManager::GetMediaFormatOrder();

	mediaFormats	= m_pcssEP->GetMediaFormats();
	list			+= OpalTranscoder::GetPossibleFormats(mediaFormats);

	/* Will update the codec settings */
	quality = 25 - (int) ((double) quality / 100 * 24);

	for (YaInt i = 0; i < list.GetSize(); ++i)
	{
		if (OpalMediaFormat::DefaultVideoSessionID == list[i].GetDefaultSessionID() &&
			RTP_DataFrame::MaxPayloadType != list[i].GetPayloadType())
		{
			for (YaInt j = 0; j < list[i].GetOptionCount(); ++j)
			{
				const OpalMediaOption & option = list[i].GetOption(j);
				if (!option.IsReadOnly())
				{
					PString codecOptionName = option.GetName();

					if (EqualTo == codecOptionName.Compare(AdaptivePacketDelayOption))//ugly codes,please fix me!!!
					{
						list[i].SetOptionBoolean(AdaptivePacketDelayOption, TRUE);
					}
					else if (EqualTo == codecOptionName.Compare(DynamicVideoQualityOption))
					{
						list[i].SetOptionBoolean(DynamicVideoQualityOption, TRUE);
					}
					else if (EqualTo == codecOptionName.Compare(FrameHeightOption))
					{
						list[i].SetOptionInteger(FrameHeightOption, height);
					}
					else if (EqualTo == codecOptionName.Compare(TargetBitRateOption))
					{
						list[i].SetOptionInteger(TargetBitRateOption, bitrate*100);
					}
					else if (EqualTo == codecOptionName.Compare(EncodingQualityOption))
					{
						list[i].SetOptionInteger(EncodingQualityOption, quality);
					}
					else if (EqualTo == codecOptionName.Compare(FrameWidthOption))
					{
						list[i].SetOptionInteger(FrameWidthOption, width);
					}
				}
			}
			/*list[i].SetOptionInteger(OpalVideoFormat::FrameHeightOption, height);
			list[i].SetOptionInteger(OpalVideoFormat::FrameWidthOption, width);
			list[i].SetOptionInteger(OpalVideoFormat::EncodingQualityOption, quality);
			list[i].SetOptionBoolean(OpalVideoFormat::DynamicVideoQualityOption, TRUE);
			list[i].SetOptionBoolean(OpalVideoFormat::AdaptivePacketDelayOption, TRUE);
			list[i].SetOptionInteger(OpalVideoFormat::TargetBitRateOption, bitrate*100);*/
		}
	}
	
	for (YaInt i = 0; i < initialOrder.GetSize(); ++i)
	{
		if (OpalMediaFormat::DefaultAudioSessionID == OpalMediaFormat(initialOrder[i]).GetDefaultSessionID() ||
			OpalMediaFormat::DefaultVideoSessionID == OpalMediaFormat(initialOrder[i]).GetDefaultSessionID())
			order += initialOrder[i];
	}
	for (YaInt i = 0; i < initialMask.GetSize(); ++i)
	{
		if (OpalMediaFormat::DefaultAudioSessionID == OpalMediaFormat(initialMask[i]).GetDefaultSessionID() ||
			OpalMediaFormat::DefaultVideoSessionID == OpalMediaFormat(initialMask[i]).GetDefaultSessionID())
			mask += initialMask[i];
	}

	for (YaInt i = 0; i < list.GetSize(); ++i)
	{
		if (OpalMediaFormat::DefaultVideoSessionID == list[i].GetDefaultSessionID())
		{
			//if (RTP_DataFrame::H261 == list[i].GetPayloadType())//only interested h.261 capability at present.
			//{
			//	order += list[i];
			//}
			//else
			{
				mask += list[i];
			}
		}
	}

	SetMediaFormatOrder(order);
	SetMediaFormatMask(mask);
}

YaVoid CYaManager::SetUserInputMode(YaVoid)
{
	m_h323EP->SetUserInputMode();
}

OpalMediaFormatList CYaManager::GetAvailableAudioMediaFormats(YaVoid)
{
	return m_h323EP->GetAvailableAudioMediaFormats();
}

OpalMediaFormatList CYaManager::GetAvailableVideoMediaFormats(YaVoid)
{
	return m_h323EP->GetAvailableVideoMediaFormats();
}

YaVoid CYaManager::OnAlerting(OpalConnection & connection)
{
	Logger("[YaManager]Alerting");

	OpalManager::OnAlerting(connection);
}

YaBool CYaManager::OnIncomingConnection(OpalConnection & connection)
{
	PString remoteName;
	PString remoteApp;
	PString remoteUrl;

	Logger("[YaManager]Incoming connection from %s", (const YaChar*)connection.GetRemotePartyName());

	GetRemoteConnectionInfo(connection, remoteName, remoteApp, remoteUrl);
	m_curCallToken	= connection.GetCall().GetToken();
	m_nCallID		= m_curCallToken.AsInteger();
	m_pDlg->SetCallID(m_nCallID);
	m_pDlg->SetRemotePartyInfo(m_curCallToken, remoteName, remoteApp, remoteUrl);

	return OpalManager::OnIncomingConnection(connection);//auto answer or ask user.
}

YaVoid CYaManager::OnRing(const OpalPCSSConnection &connection)
{
	Logger("[YaManager]Ring...");
}

OpalConnection::AnswerCallResponse CYaManager::OnAnswerCall(OpalConnection & connection, const PString & caller)
{
	Logger("[YaManager]Incoming call from %s, please answer or reject this call.", (const YaChar*)caller);

	if (AUTO_ANSWER == m_pDlg->GetIncomingCallMode())
	{
		return OpalConnection::AnswerCallNow;
	}
	else
	{
		m_pDlg->PostMessage(WM_INCOMING_CALL, 0, 0);

		return OpalConnection::AnswerCallPending;
	}
}

YaVoid CYaManager::OnConnected(OpalConnection & connection)
{
	Logger("[YaManager]Connected with token[%s]", (const YaChar*)connection.GetToken());

	OpalManager::OnConnected(connection);
}

YaVoid CYaManager::OnEstablished(OpalConnection & connection)
{
	OpalManager::OnEstablished(connection);
	
	if (PIsDescendant(&connection, OpalPCSSConnection))
	{
		return;
	}
	
	m_pDlg->SetCallState(Connected);//update GUI

	//update the state
	PString name, app, url;
	GetRemoteConnectionInfo (connection, name, app, url);
	m_pDlg->SetRemotePartyInfo(m_curCallToken, name, app, url);

	Logger("[YaManager]Established call from %s to %s ", (const YaChar*)connection.GetCall().GetPartyA(),
		(const YaChar *)connection.GetCall().GetPartyB());
	
	OpalEndPoint &endpoint = connection.GetEndPoint();
	if (PIsDescendant(&endpoint, H323EndPoint))
	{
		m_bIncoming = FALSE;
	}

	//update the timer
	AvgSignalTimer.RunContinuous(PTimeInterval(50));
}

YaVoid CYaManager::OnReleased(OpalConnection & connection)
{
	OpalManager::OnReleased(connection);//[Caution]This tells the OpalManager to release the (call )resource,otherwise it will cause deadlock.
	
	/* Do nothing for the PCSS connection */
	if (PIsDescendant(&connection, OpalPCSSConnection))
	{
		return;
	}

	Logger("[YaManager]Released with token[%s]", (const YaChar*)connection.GetToken());
	if (AvgSignalTimer.IsRunning())
		AvgSignalTimer.Stop();
}

YaVoid CYaManager::OnClearedCall(OpalCall &call)
{
	PString remoteName;

	remoteName = '"' + call.GetPartyB() + '"';

	switch (call.GetCallEndReason())
	{
	case OpalConnection::EndedByRemoteUser:
		{
			Logger("Remote user[%s] cleared the call", (const YaChar*)remoteName);
		}
		break;
	case OpalConnection::EndedByCallerAbort:
		{
			Logger("Remote user[%s] has stopped calling", (const YaChar*)remoteName);
		}
		break;
	case OpalConnection::EndedByRefusal:
		{
			Logger("Remote user[%s] rejected the call", (const YaChar*)remoteName);
		}
		break;
	case OpalConnection::EndedByNoAnswer :
		{
			Logger("%s did not answer your call", (const YaChar*)remoteName);
		}
		break;
	case OpalConnection::EndedByTransportFail :
		{
			Logger("Call with %s ended abnormally", (const YaChar*)remoteName);
		}
		break;
	case OpalConnection::EndedByCapabilityExchange :
		{
			Logger("Could not find common codec with %s", (const YaChar*)remoteName);
		}
			break;
	case OpalConnection::EndedByNoAccept :
		{
			Logger("Did not accept incoming call from %s", (const YaChar*)remoteName);
		}
		break;
	case OpalConnection::EndedByAnswerDenied :
		{
			Logger("Refused incoming call from %s", (const YaChar*)remoteName);
		}
		break;
	case OpalConnection::EndedByNoUser :
		{
			Logger("Gatekeeper or registrar could not find user %s", (const YaChar*)remoteName);
		}
		break;
	case OpalConnection::EndedByNoBandwidth :
		{
			Logger("Call to %s aborted, insufficient bandwidth.", (const YaChar*)remoteName);
		}
		break;
	case OpalConnection::EndedByUnreachable :
		{
			Logger("%s could not be reached.", (const YaChar*)remoteName);
		}
		break;
	case OpalConnection::EndedByNoEndPoint :
		{
			Logger("No phone running for %s", (const YaChar*)remoteName);
		}
		break;
	case OpalConnection::EndedByHostOffline :
		{
			Logger("%s is not online.", (const YaChar*)remoteName);
		}
		break;
	case OpalConnection::EndedByConnectFail :
		{
			Logger("Transport error calling %s", (const YaChar*)remoteName);
		}
		break;
	default :
		{
			Logger("Call with %s completed", (const YaChar*)remoteName);
		}
		break;
	}

	Logger("[YaManager]OnClearedCall");
	
	m_pDlg->SetCallState(Released);
	m_pDlg->SetCallID(0);
	m_nCallID = 0;
	m_curCallToken = PString();

	//reset timer
	AvgSignalTimer.Stop();

	OpalManager::OnClearedCall(call);
}

YaVoid CYaManager::OnUserInputString(OpalConnection & connection, const PString & value)
{
	if (!value.IsEmpty())
	{
		//update GUI
		PString name, app, url;
		GetRemoteConnectionInfo (connection, name, app, url);
		m_pDlg->SetRemotePartyInfo(m_curCallToken, name, app, url);

		m_pDlg->OutputChatMessage(GetCurrentCallToken(), value);
	}

}

YaVoid CYaManager::OnUserInputTone(OpalConnection & connection, char tone, int duration)
{
	//OpalManager::OnUserInputTone(connection, tone, duration);
}

YaBool CYaManager::OnOpenMediaStream(OpalConnection & connection, OpalMediaStream & stream)
{
	if (!OpalManager::OnOpenMediaStream(connection, stream))
		return FALSE;

	if (PIsDescendant(&connection, OpalPCSSConnection))
	{
		return TRUE;
	}

	Logger("[YaManager]Started %s media type %s[rtp payload %d] %s %s endpoint", (stream.IsSource() ? "receiving" : "sending"), stream.GetMediaFormat().GetEncodingName(), 
		stream.GetMediaFormat().GetPayloadType(), (stream.IsSource() ? "from" : "to"), (const YaChar*)connection.GetEndPoint().GetPrefixName());
	
	if (m_pcssEP->GetMediaFormats().FindFormat(stream.GetMediaFormat()) == P_MAX_INDEX)
		OnMediaStream(stream, FALSE);

	return TRUE;
}

YaVoid CYaManager::OnClosedMediaStream(const OpalMediaStream & stream)
{
	OpalManager::OnClosedMediaStream(stream);
	
	Logger("[YaManager]Stopped %s media type %s", (stream.IsSource() ? "receiving" : "sending"), stream.GetMediaFormat().GetEncodingName());

	if (m_pcssEP->GetMediaFormats().FindFormat(stream.GetMediaFormat()) == P_MAX_INDEX)
		OnMediaStream((OpalMediaStream &)stream, TRUE);
}

YaBool CYaManager::CreateVideoInputDevice(const OpalConnection &connection, const OpalMediaFormat &mediaFormat, PVideoInputDevice *&device, BOOL &autoDelete)
{
	//I don't know how to implement it at present.Just override default OpalManager function.
	return FALSE;
}

YaBool CYaManager::CreateVideoOutputDevice(const OpalConnection & connection, const OpalMediaFormat & mediaFormat, BOOL preview, PVideoOutputDevice * & device, BOOL & autoDelete )
{
	//I don't know how to implement it at present.Just override default OPalManager function.
	return FALSE;//return FALSE will cause master reject remote openLogicalChannel request.
}

YaVoid CYaManager::OnAvgSignalLevelTimeout(PTimer & note, INT extra)
{
	/*PSafePtr<OpalCall>			call		= NULL;
	PSafePtr<OpalConnection>	connection	= NULL;
	OpalRawMediaStream			*audioMedia = NULL;

	YaUint32 playVol	= 0;
	YaUint32 recordVol	= 0;

	call = FindCallWithLock(GetCurrentCallToken());
	if (!call)
		return;

	connection = GetConnection(call, FALSE);
	if (connection)
	{
		audioMedia = (OpalRawMediaStream *)connection->GetMediaStream(OpalMediaFormat::DefaultAudioSessionID, FALSE);
		if (audioMedia)
		{
			playVol = (linear2ulaw(audioMedia->GetAverageSignalLevel()) ^ 0xff) / 100;
		}

		audioMedia = (OpalRawMediaStream*)connection->GetMediaStream(OpalMediaFormat::DefaultAudioSessionID, TRUE);
		if (audioMedia)
		{
			recordVol = (linear2ulaw(audioMedia->GetAverageSignalLevel()) ^ 0xff) / 100;
		}
	}*/
}
