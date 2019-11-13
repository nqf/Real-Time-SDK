///*|-----------------------------------------------------------------------------
// *|            This source code is provided under the Apache 2.0 license      --
// *|  and is provided AS IS with no warranty or guarantee of fit for purpose.  --
// *|                See the project's LICENSE.md for details.                  --
// *|           Copyright (C) 2019 Refinitiv. All rights reserved.            --
///*|-----------------------------------------------------------------------------

package com.thomsonreuters.ema.examples.training.iprovider.series100.example160__Login__AcceptDeny;

import com.thomsonreuters.ema.access.DataType.DataTypes;
import com.thomsonreuters.ema.access.ElementEntry;
import com.thomsonreuters.ema.access.ElementList;
import com.thomsonreuters.ema.access.EmaFactory;
import com.thomsonreuters.ema.access.GenericMsg;
import com.thomsonreuters.ema.access.Msg;
import com.thomsonreuters.ema.access.OmmException;
import com.thomsonreuters.ema.access.OmmIProviderConfig;
import com.thomsonreuters.ema.access.OmmProvider;
import com.thomsonreuters.ema.access.OmmProviderClient;
import com.thomsonreuters.ema.access.OmmProviderEvent;
import com.thomsonreuters.ema.access.OmmState;
import com.thomsonreuters.ema.access.PostMsg;
import com.thomsonreuters.ema.access.RefreshMsg;
import com.thomsonreuters.ema.access.ReqMsg;
import com.thomsonreuters.ema.access.StatusMsg;
import com.thomsonreuters.ema.rdm.EmaRdm;

class AppClient implements OmmProviderClient
{	
	public void onReqMsg(ReqMsg reqMsg, OmmProviderEvent event)
	{
		switch (reqMsg.domainType())
		{
			case EmaRdm.MMT_LOGIN :
				processLoginRequest(reqMsg, event);
				break;
			default :
				processInvalidDomainRequest(reqMsg, event);
				break;
		}
	}
	
	public void onRefreshMsg(RefreshMsg refreshMsg,	OmmProviderEvent event){}
	public void onStatusMsg(StatusMsg statusMsg, OmmProviderEvent event){}
	public void onGenericMsg(GenericMsg genericMsg, OmmProviderEvent event){}
	public void onPostMsg(PostMsg postMsg, OmmProviderEvent event){}
	public void onReissue(ReqMsg reqMsg, OmmProviderEvent event){}
	public void onClose(ReqMsg reqMsg, OmmProviderEvent event){}
	public void onAllMsg(Msg msg, OmmProviderEvent event){}
	
	void processLoginRequest(ReqMsg reqMsg, OmmProviderEvent event)
	{
		if ( !reqMsg.name().equals( "user") )
		{
			event.provider().submit( EmaFactory.createStatusMsg().domainType(EmaRdm.MMT_LOGIN).name(reqMsg.name()).nameType(EmaRdm.USER_NAME).
					state(OmmState.StreamState.CLOSED, OmmState.DataState.SUSPECT, OmmState.StatusCode.NOT_AUTHORIZED, "Login denied"),
					event.handle() );
		}
		else
		{
			RefreshMsg loginRefresh = EmaFactory.createRefreshMsg();
			ElementList refreshAttributes = EmaFactory.createElementList();
			
			if ( reqMsg.attrib().dataType() == DataTypes.ELEMENT_LIST )
			{
				boolean setRefreshAttrib = false;
				ElementList reqAttributes = reqMsg.attrib().elementList();
				
				for( ElementEntry reqAttrib : reqAttributes )
				{
					String name = reqAttrib.name();
					
					if ( name.equals(EmaRdm.ENAME_ALLOW_SUSPECT_DATA) ||
							name.equals(EmaRdm.ENAME_SINGLE_OPEN) )
						{
							setRefreshAttrib = true;
							refreshAttributes.add( EmaFactory.createElementEntry().uintValue(name, reqAttrib.uintValue()) );
						}
					else if ( name.equals(EmaRdm.ENAME_APP_ID) || 
							name.equals(EmaRdm.ENAME_POSITION ) )
						{
							setRefreshAttrib = true;
							refreshAttributes.add( EmaFactory.createElementEntry().ascii(name, reqAttrib.ascii().toString()) );
						}
				}
				
				if ( setRefreshAttrib )
					loginRefresh.attrib( refreshAttributes );
			}
			
			event.provider().submit( loginRefresh.domainType(EmaRdm.MMT_LOGIN).name(reqMsg.name()).nameType(EmaRdm.USER_NAME).
					complete(true).solicited(true).state(OmmState.StreamState.OPEN, OmmState.DataState.OK, OmmState.StatusCode.NONE, "Login accepted"),
					event.handle() );
		}
	}
	
	void processInvalidDomainRequest(ReqMsg reqMsg, OmmProviderEvent event)
	{
		event.provider().submit( EmaFactory.createStatusMsg().name(reqMsg.name()).serviceName(reqMsg.serviceName()).
				state(OmmState.StreamState.CLOSED, OmmState.DataState.SUSPECT,	OmmState.StatusCode.NOT_FOUND, "Domain not found"),
				event.handle() );
	}
}

public class IProvider
{
	public static void main(String[] args)
	{
		OmmProvider provider = null;
		try
		{
			AppClient appClient = new AppClient();

			OmmIProviderConfig config = EmaFactory.createOmmIProviderConfig();
			
			provider = EmaFactory.createOmmProvider(config.port("14002"), appClient);
			
			Thread.sleep(60000);
		} 
		catch (InterruptedException | OmmException excp)
		{
			System.out.println(excp.getMessage());
		}
		finally 
		{
			if (provider != null) provider.uninitialize();
		}
	}
}
