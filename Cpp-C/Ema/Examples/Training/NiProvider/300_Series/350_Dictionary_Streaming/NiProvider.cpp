///*|-----------------------------------------------------------------------------
// *|            This source code is provided under the Apache 2.0 license
// *|  and is provided AS IS with no warranty or guarantee of fit for purpose.
// *|                See the project's LICENSE.md for details.
// *|           Copyright (C) 2019 LSEG. All rights reserved.                 --
///*|-----------------------------------------------------------------------------

#include "NiProvider.h"
#include <string.h>

using namespace refinitiv::ema::access;
using namespace refinitiv::ema::rdm;
using namespace std;

DataDictionary dataDictionary;
bool fldComplete = false;
bool enumComplete = false;
bool dumpDictionary = false;

void AppClient::onRefreshMsg(const RefreshMsg& refreshMsg, const OmmProviderEvent& ommEvent)
{
	cout << endl << "Handle: " << ommEvent.getHandle() << " Closure: " << ommEvent.getClosure() << endl;

	cout << endl << "Item Name: " << (refreshMsg.hasName() ? refreshMsg.getName() : EmaString("<not set>")) << endl
		<< "Service Name: " << (refreshMsg.hasServiceName() ? refreshMsg.getServiceName() : EmaString("<not set>"));

	cout << endl << "Item State: " << refreshMsg.getState().toString() << endl;

	decode(refreshMsg, refreshMsg.getComplete());
}

void AppClient::onStatusMsg(const StatusMsg& statusMsg, const OmmProviderEvent& ommEvent)
{
	cout << endl << "Handle: " << ommEvent.getHandle() << " Closure: " << ommEvent.getClosure() << endl;

	cout << endl << "Item Name: " << (statusMsg.hasName() ? statusMsg.getName() : EmaString("<not set>")) << endl
		<< "Service Name: " << (statusMsg.hasServiceName() ? statusMsg.getServiceName() : EmaString("<not set>"));

	if (statusMsg.hasState())
		cout << endl << "Item State: " << statusMsg.getState().toString() << endl;
}

void AppClient::decode(const Msg& msg, bool complete)
{
	switch (msg.getPayload().getDataType())
	{
		case DataType::SeriesEnum:
		{
			if (msg.getName() == "RWFFld")
			{
				dataDictionary.decodeFieldDictionary(msg.getPayload().getSeries(), DICTIONARY_NORMAL);

				if (complete)
				{
					fldComplete = true;
				}
			}

			else if (msg.getName() == "RWFEnum")
			{
				dataDictionary.decodeEnumTypeDictionary(msg.getPayload().getSeries(), DICTIONARY_NORMAL);

				if (complete)
				{
					enumComplete = true;
				}
			}

			if (fldComplete && enumComplete)
			{
				cout << endl << "Dictionary download complete" << endl;
				cout << "Dictionary Id : " << dataDictionary.getDictionaryId() << endl;
				cout << "Dictionary field version : " << dataDictionary.getFieldVersion() << endl;
				cout << "Number of dictionary entries : " << dataDictionary.getEntries().size() << endl;

				if ( dumpDictionary )
					cout << dataDictionary << endl;
			}
		}
	}
}

int main( int argc, char* argv[] )
{
	try
	{
		int iargs = 1;

		while ( iargs < argc )
		{
			if ( ( strcmp("-dumpDictionary", argv[iargs] ) == 0) )
			{
				dumpDictionary = true;
			}

			++iargs;
		}

		OmmProvider provider( OmmNiProviderConfig().username( "user" ) );
		UInt64 triHandle = 5;
		UpdateMsg update;
		FieldList fieldList;
		AppClient client;

		// Open Dictionary streams
		UInt64 fldHandle = provider.registerClient(ReqMsg().name("RWFFld").filter(DICTIONARY_NORMAL).serviceName("NI_PUB").domainType(MMT_DICTIONARY), client);

		UInt64 enumHandle = provider.registerClient(ReqMsg().name("RWFEnum").filter(DICTIONARY_NORMAL).serviceName("NI_PUB").domainType(MMT_DICTIONARY), client);

		provider.submit( RefreshMsg().serviceName( "NI_PUB" ).name( "TRI.N" )
			.state( OmmState::OpenEnum, OmmState::OkEnum, OmmState::NoneEnum, "UnSolicited Refresh Completed" )
			.payload( fieldList.clear()
				.addReal( 22, 4100, OmmReal::ExponentNeg2Enum )
				.addReal( 25, 4200, OmmReal::ExponentNeg2Enum )
				.addReal( 30, 20, OmmReal::Exponent0Enum )
				.addReal( 31, 40, OmmReal::Exponent0Enum )
				.complete() )
			.complete(), triHandle );

		sleep( 1000 );

		for ( Int32 i = 0; i < 60; i++ )
		{
			provider.submit( update.clear().serviceName( "NI_PUB" ).name( "TRI.N" )
				.payload( fieldList.clear()
					.addReal( 22, 4100 + i, OmmReal::ExponentNeg2Enum )
					.addReal( 30, 21 + i, OmmReal::Exponent0Enum )
					.complete() ), triHandle );
			sleep( 1000 );
		}
	}
	catch ( const OmmException& excp )
	{
		cout << excp << endl;
	}
	return 0;
}
