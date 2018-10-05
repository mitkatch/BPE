
/************************************************************************
||
|| DATE:                $Date: 2004/04/28 16:25:14 $
|| SOURCE:              $Source: /export/home/tomcat/mlsrc/libapps/RCS/MessageFactory.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: MessageFactory.hpp,v 1.2 2004/04/28 16:25:14 mikhailt Exp $
|| REVISION:    $Revision: 1.2 $
|| LOG:                 $Log: MessageFactory.hpp,v $
|| LOG:                 Revision 1.2  2004/04/28 16:25:14  mikhailt
|| LOG:                 change string to const string
|| LOG:
|| LOG:                 Revision 1.1  2004/01/08 20:51:46  tomcat
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#ifndef MESSAGEFACTORY_HPP
#define MESSAGEFACTORY_HPP

#include "MsgProcessor.hpp"
#include "MsgGraph.hpp"

class MessageFactory
{
	public:
		typedef bool (*MsgHandler)(MsgGraph &image, MsgProcessor *processor); 
	public:
		virtual ~MessageFactory();
		static MessageFactory * getFactory();
		virtual void processMsg(string& msgType, MsgGraph& image, MsgProcessor *proc);
		virtual bool registerMsgHandler(const string& msgType, MsgHandler handler);
		virtual bool unregisterMsgHandler(string msgType);
	
	protected:
		static MessageFactory* _factory;
		MessageFactory();
		map<string, MsgHandler> _handlers;
		

};
 
#endif //MESSAGEFACTORY_HPP
