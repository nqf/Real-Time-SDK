/*|-----------------------------------------------------------------------------
 *|            This source code is provided under the Apache 2.0 license
 *|  and is provided AS IS with no warranty or guarantee of fit for purpose.
 *|                See the project's LICENSE.md for details.
 *|           Copyright (C) 2019 LSEG. All rights reserved.                 --
 *|-----------------------------------------------------------------------------
 */

#ifndef __refinitiv_ema_access_OmmEnumDecoder_h
#define __refinitiv_ema_access_OmmEnumDecoder_h

#include "Decoder.h"
#include "EmaBufferInt.h"

namespace refinitiv {

namespace ema {

namespace access {

class OmmEnumDecoder : public Decoder
{
public :

	OmmEnumDecoder();

	virtual ~OmmEnumDecoder();

	bool setRsslData( UInt8 , UInt8 , RsslMsg* , const RsslDataDictionary* );

	bool setRsslData( UInt8 , UInt8 , RsslBuffer* , const RsslDataDictionary* , void* );

	bool setRsslData( RsslDecodeIterator* , RsslBuffer* );

	Data::DataCode getCode() const;

	const EmaString& toString();

	UInt16 getEnum() const;

	const EmaBuffer& getHexBuffer();

	const RsslBuffer& getRsslBuffer() const;

	OmmError::ErrorCode getErrorCode() const;

private :

	RsslBuffer*				_pRsslBuffer;

	RsslEnum				_rsslEnum;

	EmaString				_toString;

	EmaBufferInt			_hexBuffer;

	Data::DataCode			_dataCode;

	OmmError::ErrorCode		_errorCode;
};

}

}

}

#endif //__refinitiv_ema_access_OmmEnumDecoder_h
