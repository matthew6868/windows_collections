/**	@file	YaManager.h
 *	\brief	YaEndpoint Management Class,inherited from OpalManager.
 *	
 *	\author xuyao<matthew6868@163.com>
 *
 *	\revised history[version,who,when,what]
 *		v1.0.0,xuyao,2007-03-13,created.
 *		v1.0.2,xuyao,2007-03-26,finish basic call flow.
 */

#ifndef __YA_MANAGER_H__
#define __YA_MANAGER_H__

#include "YaAfx.h"
#include "YaTypes.h"
#include "YaH323.h"
#include "YaPcss.h"

#define LOG_INFO	0x0
#define LOG_WARNING 0x1
#define LOG_DEBUG	0x2
#define LOG_ERROR	0x4

#define YA_CIF_WIDTH   352
#define YA_CIF_HEIGHT  288
#define YA_QCIF_WIDTH  176
#define YA_QCIF_HEIGHT 144
#define YA_SIF_WIDTH   320
#define YA_SIF_HEIGHT  240
#define YA_QSIF_WIDTH  160
#define YA_QSIF_HEIGHT 120
#define YA_FRAME_SIZE  10

//forward declaration
class CYaManager;
class CYaPhoneDlg;
class CYaH323EndPoint;
class CYaPCSSEndPoint;

class CYaManager : public OpalManager
{
	PCLASSINFO(CYaManager, OpalManager);
private:
	typedef YaVoid (*fManagerLogFunc)(YaInt lvl, YaChar *szLog);

public:
	enum CallState{StandBy, Calling, Connected, Called, Released,};

	/**@name Construction */
	//@{
	CYaManager(YaVoid){
		m_pDlg		= NULL;
		m_fLogFunc	= NULL;
		m_h323EP	= NULL;
		m_pcssEP	= NULL;

		m_bIncoming		= TRUE;
		m_bIsRegister	= FALSE;
	};
	virtual ~CYaManager(){
		UnregisterGatekeeper();
		ClearAllCalls(OpalConnection::EndedByLocalUser, TRUE);
	};
	//@}

	/**@name Interactive with application */
	//@{
	YaBool Initialize(YaVoid *Param);

	YaVoid SetLogHandler(fManagerLogFunc logHandler);

	YaVoid Logger(const YaChar *fmt, ...);
	
	YaBool SetStackLogFile(const YaChar *fileName);

	YaBool SetStackDebugLevel(int debugLevel);

	CYaPCSSEndPoint *GetPCSSEndPoint(YaVoid);

	CYaH323EndPoint *GetH323EndPoint(YaVoid);
	
	CYaPhoneDlg		*GetMainDlg(YaVoid);

	PString GetCurrentCallToken(YaVoid);
	
	YaInt GetCurrentCallID(YaVoid);

	YaVoid UpdateVolumeSlider(YaUint32 volume, YaBool bRecord);

	YaVoid UpdateVolumeIndicate(YaUint32 volume, YaBool bRecord);
	
	YaVoid GetRemoteConnectionInfo (OpalConnection &, PString&,	PString&, PString&);

	/** Returns the local or remote OpalConnection for the given call.If there are 
		several connections, the first one is returned.
	  */
	PSafePtr<OpalConnection> GetConnection(PSafePtr<OpalCall> call, YaBool isRemote);

	/** Send the given text to the other party
	 */
	YaBool SendTextMessage(PString token, PString message);

	/** Update GUI
	 */
	YaVoid OnMediaStream(OpalMediaStream &stream, YaBool bClosing);

	/** Set all media formats following user config.
	 */
	YaVoid SetAllMediaFormats(YaVoid);

	/** Set audio formats following user config.
	 */
	YaVoid SetAudioMediaFormats(YaVoid);

	/** Set video formats following the user config.
	 */
	YaVoid SetVideoMediaFormats(YaVoid);

	/** Set the User Input Mode following user configuration options.
	 */
	YaVoid SetUserInputMode(YaVoid);

	/** Return the list of audio formats supported by the manager
	 */
	OpalMediaFormatList GetAvailableAudioMediaFormats(YaVoid);

	/** Return the list of video formats supported by the manager
	 */
	OpalMediaFormatList GetAvailableVideoMediaFormats(YaVoid);

	//@}

	/**@name Interactive with opal stack */
	//@{
	YaBool	CallMake(const YaChar *destination, YaInt &nCallID);
	YaBool	CallDrop(YaInt nCallID);
	YaBool	CallAnswer(YaInt nCallID);
	YaBool	CallReject(YaInt nCallID);
	
	YaBool	OpenLogicalChannel(YaVoid);
	YaBool	OpenLogicalChannelAck(YaVoid);
	YaBool	OpenLogicalChannelReject(YaInt nCallID, YaInt nChannelID);

	YaInt	RegisterGatekeeper(PString szGKIP, PString szUserName, PString szTelNo, PString szPassword = PString::Empty());
	YaInt	UnregisterGatekeeper(YaVoid);
	YaBool	IsRegisteredWithGatekeeper(YaVoid);	
	YaVoid	OnRegistrationConfirm(YaVoid);
	YaVoid	OnRegistrationReject(YaVoid);

	/** Set Default name and E.164 name.
	 * param1£ºszUserName£ºused to display,packed in Q.931 message¡£
	 * param2£ºszAlias£ºalias name used to register,availabe character¡°*#,0-9¡±¡£
	 */
	YaVoid	SetUserNameAndAlias(PString userName, PString aliasName);
	YaChar	*GetRemoteAddrInfo(YaInt nCallID);

	/** Set TCP and RTP port range, from 1024 to 65535¡£
	 */
	YaVoid	SetTCPPortRange(YaUint16 portFrom, YaUint16 portTo);
	YaVoid	SetRTPPortRange(YaUint16 portFrom, YaUint16 portTo);
	YaVoid	SetRTPPort(YaInt nCallID, YaUint16 dataPort, YaUint16 controlPort);

	/** overrides the default member function(s), so that we can know the call progress.
	 */
	YaVoid OnAlerting(OpalConnection & /*connection*/);

	YaBool OnIncomingConnection(OpalConnection & connection);
	
	YaVoid OnRing(const OpalPCSSConnection &connection);

	OpalConnection::AnswerCallResponse OnAnswerCall(OpalConnection & connection, const PString & caller);

	YaVoid OnConnected(OpalConnection & connection);

	YaVoid OnEstablished(OpalConnection & connection);

	YaVoid OnReleased(OpalConnection & connection);

	YaVoid OnClearedCall(OpalCall & call);
	
	/**This callback is called when receiving an input string.It is used to update the text chat GUI.
	 */
	YaVoid OnUserInputString(OpalConnection & connection, const PString & value);

	/**This callback is called when receiving an input tone.more detail can be found in manager.h
	 */
	YaVoid OnUserInputTone(OpalConnection & connection, char tone, int duration);

	/** Callback called when OPAL opens a new logical channel.
	 *	return TRUE if ok,FALSE if error.
	 */
	YaBool OnOpenMediaStream(OpalConnection & connection, OpalMediaStream & stream);

	/** Callback called when OPAL closes a new logical channel.
	 */
	YaVoid OnClosedMediaStream(const OpalMediaStream & stream);
	//@}

	/**@name Video and Audio related with GUI.
	//@{
	/** This callback is called when an input video device has to be opened.
	 */
	YaBool CreateVideoInputDevice(const OpalConnection & connection, 
		const OpalMediaFormat & mediaFormat, 
		PVideoInputDevice * & device, 
		BOOL & autoDelete);

	/** This callback is called when an output video device opened.
	*/
	YaBool CreateVideoOutputDevice(const OpalConnection & connection, 
		const OpalMediaFormat & mediaFormat, 
		BOOL preview, 
		PVideoOutputDevice * & device,
		BOOL & autoDelete );
	//@}

private:
	fManagerLogFunc		m_fLogFunc;
	
	YaBool				m_bIsRegister;
	YaBool				m_bIncoming;

	PString				m_curCallToken;
	YaInt				m_nCallID;

	CYaH323EndPoint		*m_h323EP;
	CYaPCSSEndPoint		*m_pcssEP;

	CYaPhoneDlg			*m_pDlg;

	static YaInt		_instance;
	
	PTimer				AvgSignalTimer;
	PDECLARE_NOTIFIER(PTimer, CYaManager, OnAvgSignalLevelTimeout);
};

#endif	/* __YA_MANAGER_H__ */

/*
 * The contents of this file are subject to the Mozilla Public License
 * Version 1.0 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS"
 * basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See
 * the License for the specific language governing rights and limitations
 * under the License.
 * 
 * The Original Code is Open Phone Abstraction Library. 
 *
 * The Initial Developer of the Original Code is ________________________. 
 * Portions created by ______________________ are Copyright (C) ______ _______________________. 
 * All Rights Reserved. 
 * 
 * Contributor(s): ______________________________________.''
 *
 */