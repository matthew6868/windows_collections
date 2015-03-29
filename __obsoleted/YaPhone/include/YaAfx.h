/**	@file	YaAfx.h
 *	\brief	
 *	
 *	\author xuyao<matthew6868@163.com>
 *
 *	\revised history[version,who,when,what]
 *		v1.0.0,xuyao,2007-02-13,created.
 */

#ifndef __YA_AFX_H__
#define __YA_AFX_H__

#include "ptlib.h"
#include "opal/manager.h"
#include "opal/transcoders.h"
#include "opal/mediafmt.h"
#include "opal/mediastrm.h"
#include "opal/pcss.h"
#include "h323/h323ep.h"
#include "h323/gkclient.h"

#include <list>
#include <map>

using namespace std;

#define MAX_CALL_NUMS 128

/* Incoming Call Mode */
typedef enum {
	AVAILABLE,
	AUTO_ANSWER,
	DO_NOT_DISTURB,
	FORWARD,
	NUM_MODES
}IncomingCallMode;

/**
 * Defines the audio codecs
 **/
typedef enum
{
	YaCodecIdentifier_UnknownCodec  =   0,

	// Audio Codecs
	YaCodecIdentifier_G711_uLaw     =   1,
	YaCodecIdentifier_G711_ALaw     =   2,
	YaCodecIdentifier_Speex         =   3,
	YaCodecIdentifier_GSM           =   4,

	// Audio Recording Codecs
	YaCodecIdentifier_LinearPCM     =  50,

	// Video Codecs
	YaCodecIdentifier_H261          = 100,
	YaCodecIdentifier_H263          = 101,
	YaCodecIdentifier_H264          = 102,

	// Video Recording Codecs
	YaCodecIdentifier_MPEG4         = 150,

}EYaCodecIdentifier;

/**
 * Defines the various VideoSizes which are supported
 * by the framework
 **/
typedef enum
{
	YaVideoSize_NoVideo = 0,
	YaVideoSize_SQCIF,
	YaVideoSize_QCIF,
	YaVideoSize_CIF,
	YaVideoSize_4CIF,
	YaVideoSize_16CIF,
	YaVideoSize_Custom,
	YaVideoSizeCount
}EYaVideoSize;

#endif	/* __YA_AFX_H__ */