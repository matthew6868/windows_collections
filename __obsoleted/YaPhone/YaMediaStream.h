#pragma once
#include "opal\mediastrm.h"

class CYaMediaStream : public OpalMediaStream
{
public:
	CYaMediaStream(const OpalMediaFormat & mediaFormat, BOOL isSource);
	virtual ~CYaMediaStream(void);

	/**@name overrides from OpalMediaStream */
	//@name
	/** Read raw media data from the source media stream.
		The default behavior simply calls ReadPacket() on the data portion of the
		RTP_DataFrame and sets the frames timestamp and marker from the internal
		member variables of the media stream class.
	*/
	BOOL ReadData(
		BYTE * data,      ///<  Data buffer to read to
		PINDEX size,      ///<  Size of buffer
		PINDEX & length   ///<  Length of data actually read
		);
	
	/** Read an RTP frame of data from the source media stream.
		The default behavior simply calls ReadData() on the data portion of the
		RTP_DataFrame and sets the frames timestamp and marker from the internal
		member variables of the media stream class.
	*/
	virtual BOOL ReadPacket(
		RTP_DataFrame & packet
		);

private:
	RTP_DataFrame	dataFrame;
	BOOL			hasStarted;
	BOOL			isTerminated;
};
