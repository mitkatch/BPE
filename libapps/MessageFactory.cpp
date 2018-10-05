
/************************************************************************
||
|| DATE:                $Date: 2004/04/28 16:25:14 $
|| SOURCE:              $Source: /repository/projects/Canpx/libapps/MessageFactory.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: MessageFactory.cpp,v 1.2 2004/04/28 16:25:14 mikhailt Exp $
|| REVISION:    $Revision: 1.2 $
|| LOG:                 $Log: MessageFactory.cpp,v $
|| LOG:                 Revision 1.2  2004/04/28 16:25:14  mikhailt
|| LOG:                 change string to const string
|| LOG:
|| LOG:                 Revision 1.1  2004/01/08 20:51:46  tomcat
|| LOG:                 Initial revision
|| LOG:
************************************************************************/


#include "MessageFactory.hpp"

MessageFactory* MessageFactory::_factory = NULL;

MessageFactory::MessageFactory()
{

}

MessageFactory::~MessageFactory()
{

}

MessageFactory *
MessageFactory::getFactory()
{
	if (_factory == NULL){
		_factory = new MessageFactory;
	}
	return _factory;
}

void
MessageFactory::processMsg(string &msgType, MsgGraph& image, MsgProcessor *proc)
{
	map<string, MsgHandler>::const_iterator it = _handlers.find(msgType);
	if (it == _handlers.end()){
		proc->defaultMsgType(msgType, image);
	} else {
		(*it->second)(image,proc);
	}
}

bool
MessageFactory::registerMsgHandler(const string& msgType, MsgHandler handler)
{
	return _handlers.insert(map<string, MsgHandler>::value_type(msgType, handler)).second;

}

bool
MessageFactory::unregisterMsgHandler(string msgType)
{
	return _handlers.erase(msgType) == 1;
}
