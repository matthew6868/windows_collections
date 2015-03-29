/**	@file	YaPcss.h
 *	\brief	PC Sound System encapsulate in order to convenient use.
 *
 *	\author xuyao<matthew6868@163.com>
 *
 *	\revised history[version,who,when,what]
 *		v1.0.0,xuyao,2007-03-24,created.
 */

#ifndef __YA_PCSS_H__
#define __YA_PCSS_H__

#include "YaAfx.h"
#include "YaTypes.h"
#include "YaManager.h"
#include "opal/patch.h"

class CYaManager;		//forward declaration

class CYaAudioSignalFilter : public PObject
{
	PCLASSINFO(CYaAudioSignalFilter, PObject);

public:
	CYaAudioSignalFilter(CYaManager &);

	const PNotifier &GetReceiveHandler(YaVoid) const
	{
		return receiveHandler;
	}
	const PNotifier &GetSendHandler(YaVoid) const
	{
		return sendHandler;
	}

	YaUint32 GetAverageSignalLevel(const YaUint8 *buffer, YaInt size);

protected:
	PDECLARE_NOTIFIER(RTP_DataFrame, CYaAudioSignalFilter, ReceivePacket);
	PDECLARE_NOTIFIER(RTP_DataFrame, CYaAudioSignalFilter, SendPacket);

	PNotifier receiveHandler;
	PNotifier sendHandler;

	CYaManager &m_manager;
};

class CYaPCSSEndPoint : public OpalPCSSEndPoint
{
	PCLASSINFO(CYaPCSSEndPoint, OpalPCSSEndPoint);
public:
	/**@name The constructor */
	//@{
	CYaPCSSEndPoint(CYaManager &manger);

	/** The destructor 
	 */
	virtual ~CYaPCSSEndPoint(void){
		if (m_audioSignalFiter){
			delete m_audioSignalFiter;
			m_audioSignalFiter = NULL;
		}
		};
	//@}

	/**@name callback */
	//@{
	/** Call back to get the destination for outgoing call.
	 *	If FALSE is returned the call is aborted.
	 */
	PString OnGetDestination(const OpalPCSSConnection & connection);
	
	/** Call back to indicate that remote is ringing.
	 *	If FALSE is returned the call is aborted.
	 */
	YaVoid OnShowIncoming(const OpalPCSSConnection & connection);

	/** Call back to indicate that remote is ringing.
	 *	If FALSE is returned the call is aborted.
	 */
	YaBool OnShowOutgoing(const OpalPCSSConnection & connection);

	YaVoid OnReleased(OpalConnection & connection);

	/** Call back when patching a media stream.
		This function is called when a connection has created a new media
		patch between two streams.
	*/
	YaVoid OnPatchMediaStream(const OpalPCSSConnection & connection, BOOL isSource, OpalMediaPatch & patch);

	/** Accept the current incoming call.
	 */
	YaVoid AcceptCurrentIncomingCall(YaVoid);

	//OpalMediaFormatList GetMediaFormats() const;

	/** Updating the playing and recording volume levels.return FALSE if fails.
	 */
	YaBool SetDeviceVolume(YaUint32 playVol, YaUint32 recordVol);

	/** Get the playing and recording volume levels.
	 */
	YaVoid GetDeviceVolume(YaUint32 &playVol, YaUint32 &recordVol);

	/** This callback is called when an audio channel has to be opened.The main purpose 
		is get the default volume value.
	 */
	PSoundChannel *CreateSoundChannel(const OpalPCSSConnection & connection, 
							const OpalMediaFormat & mediaFormat, BOOL isSource);
	//@}

private:
	CYaPCSSEndPoint(YaVoid);

	CYaManager				&m_manager;
	CYaAudioSignalFilter	*m_audioSignalFiter;
	PString					m_curIncomingConnectionToken;

};

#endif	/* __YA_PCSS_H__ */

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