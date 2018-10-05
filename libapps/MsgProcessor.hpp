
/************************************************************************
||
|| DATE:                $Date: 2004/01/08 20:51:46 $
|| SOURCE:              $Source: /repository/projects/Canpx/libapps/MsgProcessor.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: MsgProcessor.hpp,v 1.1 2004/01/08 20:51:46 tomcat Exp $
|| REVISION:    $Revision: 1.1 $
|| LOG:                 $Log: MsgProcessor.hpp,v $
|| LOG:                 Revision 1.1  2004/01/08 20:51:46  tomcat
|| LOG:                 Initial revision
|| LOG:
************************************************************************/


#ifndef MSGPROCESSOR_HPP
#define MSGPROCESSOR_HPP
#include "MsgGraph.hpp"

class MsgProcessor
{
	public:
		MsgProcessor();
		virtual ~MsgProcessor();
		virtual void defaultMsgType(string& msgType, MsgGraph& image)=0;

};
#endif //MSGPROCESSOR_HPP
