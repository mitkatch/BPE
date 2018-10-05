
/************************************************************************
||
|| DATE:                $Date: 2005/11/17 21:57:58 $
|| SOURCE:              $Source: /repository/projects/Canpx/espeedII/ESCFETIProcessor.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: ESCFETIProcessor.hpp,v 1.1 2005/11/17 21:57:58 mikhailt Exp $
|| REVISION:    $Revision: 1.1 $
|| LOG:                 $Log: ESCFETIProcessor.hpp,v $
|| LOG:                 Revision 1.1  2005/11/17 21:57:58  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#ifndef ESCFETIPROCESSOR_HPP
#define ESCFETIPROCESSOR_HPP
#include <map>
#include <string>
#include "cfeti.h"

#include "ESInstrument.hpp"

using std::map;
using std::string;

class ESCFETIProcessor
{
	public:
		ESCFETIProcessor(){};
		virtual ~ESCFETIProcessor(){};

		static void SystemCallback(CFETI_CMD cmd, CFETI_CMD_STATUS cmdStatus, CFETI_CMDDATA cmdData, CFETI_UD userData);
		static void* MessageLoop(void *arg);
		static void ConnectCallback(CFETI_CMD cmd, CFETI_CMD_STATUS cmdStatus, CFETI_CMDDATA cmdData, CFETI_UD userData);
		static void OutputInstrumentData(CFETI_INSTRUMENT_DATA instData);
		static const char* getStateInfo(int state);
	protected:
		ESInstrument *getESInstrument(const char*);
		map<string, ESInstrument*> _instrMap;
		static ESCFETIProcessor *_myProcessor;
		static int _subscribedInst;
		map<string, ESInstrument* > _mapInst;
};

#endif //ESCFETIPROCESSOR_HPP
