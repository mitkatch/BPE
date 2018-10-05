/************************************************************************
||
|| DATE:                $Date: 2004/10/06 19:44:25 $
|| SOURCE:              $Source: /repository/projects/Canpx/pageparser/ParserTrsPageHandler.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: ParserTrsPageHandler.hpp,v 1.1 2004/10/06 19:44:25 mikhailt Exp $
|| REVISION:    $Revision: 1.1 $
|| LOG:                 $Log: ParserTrsPageHandler.hpp,v $
|| LOG:                 Revision 1.1  2004/10/06 19:44:25  mikhailt
|| LOG:                 Initial revision
|| LOG:
|| LOG:                 Revision 1.1  2004/10/06 19:42:08  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#ifndef PARSERTRSPAGEHANDLER_HPP
#define PARSERTRSPAGEHANDLER_HPP

#include "TrsPageHandler.hpp"

class ParserTrsPageHandler: public TrsPageHandler
{
	public:
		ParserTrsPageHandler(const ConfigMap &);
		virtual ~ParserTrsPageHandler(){};

	protected:

};


#endif //PARSERTRSPAGEHANDLER_HPP
