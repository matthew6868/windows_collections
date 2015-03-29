/**	@file	h323.h
 *	\brief	encapsulate H323EndPoint class in order to convenient use.
 *	
 *	\author xuyao<matthew6868@163.com>
 *	
 *	\revised history[version,who,when,what]
 *		v1.0.0,xuyao,2007-03-24,created.
 */

#ifndef __YA_H323_H__
#define __YA_H323_H__

#include "YaAfx.h"
#include "YaTypes.h"
#include "YaManager.h"

class CYaManager;		//forward declaration

class CYaH323EndPoint : public H323EndPoint
{
	PCLASSINFO(CYaH323EndPoint, H323EndPoint);

public:
	enum UserInputMode{Q931, STRING, TONE, RFC2833,};

	/**@name The constructor */
	//@{
	CYaH323EndPoint(CYaManager &manager);

	/**The destructor.
	 */
	~CYaH323EndPoint(void){ClearAllCalls();};
	//@}

	/**@name */
	//@{
	/** Init the endpoint internal values and the various
	 */
	YaVoid Init (void);
	
	/** Listen special iface and port
	 */
	YaBool StartListener(PString iface , YaWord wPort);

	/** Add the User Input Mode following the configuration options.String,
	 *	Tone,and RFC2833 are support now.
	 */
	YaVoid SetUserInputMode(enum UserInputMode eMode = STRING);

	/** Return the list of supported audio formats by h.323 endpoint
	 */
	OpalMediaFormatList GetAvailableAudioMediaFormats(YaVoid);

	/** Return the list of supported video formats by h.323 endpoint
	 */
	OpalMediaFormatList GetAvailableVideoMediaFormats(YaVoid);

	YaVoid	SetGatekeeperAddress(PString gkAddress){m_gkAddress = gkAddress;}

	PString GetGatekeeperAddress(YaVoid)const {return m_gkAddress;}
	//@}

	/**@name callback*/
	//@{
	YaBool OnIncomingConnection(OpalConnection & conn);
	
	YaBool OnAlerting(H323Connection & connection, const H323SignalPDU & alertingPDU, const PString & user);

	YaVoid OnEstablished (OpalConnection &);
	
	YaVoid OnReleased (OpalConnection &);

	/** Called when the gatekeeper accept the registration.
	 */
	YaVoid OnRegistrationConfirm(YaVoid);
	
	/** Called when the gatekeeper reject he registration.
	 */
	YaVoid OnRegistrationReject(YaVoid);
	//@}

private:

	/**@name Notifier*/
	//@{
	/** Notifier called when an incoming call has not been answered in the required time.
	 *	Reject the call, or forward if forward on no answer is enabled in the config database.
	 */
	PDECLARE_NOTIFIER(PTimer, CYaH323EndPoint, OnNoAnswerTimeout);
	//@}

	PTimer NoAnswerTimer;

	CYaManager &m_manager;

	PString		m_gkAddress;
	//YaBool		m_bIsRegister;
};

#endif	/* __YA_H323_H__ */

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