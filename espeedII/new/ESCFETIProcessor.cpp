
/************************************************************************
||
|| DATE:                $Date: 2006/04/19 01:13:46 $
|| SOURCE:              $Source: /repository/projects/Canpx/espeedII/ESCFETIProcessor.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: ESCFETIProcessor.cpp,v 1.2 2006/04/19 01:13:46 nickz Exp $
|| REVISION:    $Revision: 1.2 $
|| LOG:                 $Log: ESCFETIProcessor.cpp,v $
|| LOG:                 Revision 1.2  2006/04/19 01:13:46  nickz
|| LOG:                 .
|| LOG:
|| LOG:                 Revision 1.1  2005/11/17 21:57:58  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#include "ESCFETIProcessor.hpp"
#include "ESCFETISession.hpp"
#include "Logger.hpp"
#include "DataCache.hpp"
#include "DataGraph.hpp"


ESCFETIProcessor* ESCFETIProcessor::_myProcessor = new ESCFETIProcessor();
int ESCFETIProcessor::_subscribedInst = 0;
void
ESCFETIProcessor::SystemCallback(CFETI_CMD cmd, CFETI_CMD_STATUS cmdStatus, 
				CFETI_CMDDATA cmdData, CFETI_UD userData)
{

	int result;
	if(cmdStatus && cmdStatus->statusText){
		Logger::getLogger()->log(Logger::TYPE_INFO, "ESCFETIProcessor::SystemCallback()  %s", cmdStatus->statusText);
	}

	switch(cmd){
                case CFETI_LOGIN_ACCEPTED:
                {
			CFETI_LOGIN_INFO *l = (CFETI_LOGIN_INFO *)cmdData;
			CFETI_TRADING_SYS *ts;
			Logger::getLogger()->log(Logger::TYPE_INFO, "ESCFETIProcessor::SystemCallback() Login accepted - session id %s", l->sessionId) ;
			if (!ESCFETISession::currentSession)
				ESCFETISession::currentSession = strdup(l->sessionId);

			for(ts = l->ts; ts && *ts; ts++) {
				if ((*ts)->tsId == ESCFETISession::room) {
					result = CFETIConnect(ESCFETISession::currentSession, ESCFETISession::_password, (*ts)->tsId, 0, ConnectCallback, (void*)((*ts)->tsId));
					break;
				}
			}
			switch(result) {
				case CFETI_SUCCESS:
					break;
				default:
					Logger::getLogger()->log(Logger::TYPE_INFO, "ESCFETIProcessor::SystemCallback() CFETIConnect to %d aka \"%s\" Failed : Error %d (%s)", (*ts)->tsId, ((*ts)->tsDescription ? (*ts)->tsDescription : "(no description)"), result, CFETIGetLastError());
			}
		}
			break;
		case CFETI_LOGIN_REJECTED:
			Logger::getLogger()->log(Logger::TYPE_INFO, "ESCFETIProcessor::SystemCallback() Login rejected : user %s", cmdData ? (char *)cmdData : "unknown");
			break;
		case CFETI_LOGIN_TERMINATED:
			Logger::getLogger()->log(Logger::TYPE_INFO, "ESCFETIProcessor::SystemCallback() Login terminated : session %s", cmdData ? (char *)cmdData : "unknown");
			break;
		case CFETI_LOGOUT_ACCEPTED:
			Logger::getLogger()->log(Logger::TYPE_INFO, "ESCFETIProcessor::SystemCallback() Logout accepted : session %s", cmdData ? (char *)cmdData : "unknown");
			break;
		case CFETI_LOGOUT_REJECTED:
			Logger::getLogger()->log(Logger::TYPE_INFO, "ESCFETIProcessor::SystemCallback() Logout rejected : session %s", cmdData ? (char *)cmdData : "unknown");
			break;
		default:
			Logger::getLogger()->log(Logger::TYPE_INFO, "ESCFETIProcessor::SystemCallback() Logout rejected <default>: session %s", cmdData ? (char *)cmdData : "unknown");
			break;
	}
}

void
ESCFETIProcessor::ConnectCallback(CFETI_CMD cmd, CFETI_CMD_STATUS cmdStatus, 
				  CFETI_CMDDATA cmdData, CFETI_UD userData)
{
    CFETI_CONNECT_INFO *c;
    
    switch(cmd) 
    {
    case CFETI_TRADE_CONFIRM:
	break;

    case CFETI_CONNECTION_ACCEPTED: 
    {
	c = (CFETI_CONNECT_INFO *)cmdData;
	
	if (ESCFETISession::currentTradingSession == 0) 
	{
	    ESCFETISession::currentTradingSession = c->sessionId;
	    
	    // To query available instruments in this room
	    CFETI_QUERY_DESC query;
	    CFETI_FIELD_DESC field;
	    query.queryType = CFETQ_INSTRUMENT;
	    
	    char value[] = "*";
	    field.fieldId   = (unsigned short)CFETF_CUSIP;
	    field.fieldType = (unsigned char)CFETI_FIELDTYPE_STRING;
	    field.fieldValue.string = (char*)((const char*)value);
	    
	    query.query.instrument.numIndicativeFields = 1;
	    query.query.instrument.indicativeFields = &field;
	    
	    int result = CFETIPostMessage((char*)((const char*)ESCFETISession::currentSession),
					  ESCFETISession::currentTradingSession, CFETC_QUERY,
					  (CFETI_CMDDATA)&query, 0);
	    
	    if (result != CFETI_SUCCESS) 
	    {
		Logger::getLogger()->log(Logger::TYPE_ERROR, 
					 "CFETIPostMessage (QUERY) Failed : Error %d (%s)", 
					 result, CFETIGetLastError()); 
	    }
	    
	}
	else 
	{
	    ESCFETISession::currentTradingSession = c->sessionId;
	    ESCFETISession::isQueryDone = true;
	}
	
    }
    break;

    case CFETI_CONNECTION_REJECTED:
	Logger::getLogger()->log(Logger::TYPE_ERROR, "Connection Rejected for session %s", 
				 (char *)cmdData);
	break;

    case CFETI_CONNECTION_TERMINATED:
	c = (CFETI_CONNECT_INFO *)cmdData;
	Logger::getLogger()->log(Logger::TYPE_INFO,
				 "Connection to %d terminated - trading session id %d", 
				 c->tradingSystem.tsId, c->sessionId);
	break;

    case CFETI_DISCONNECT_ACCEPTED:
	c = (CFETI_CONNECT_INFO *)cmdData;
	Logger::getLogger()->log(Logger::TYPE_INFO,
				 "Disconnect from %d accepted - trading session id %d", 
				 c->tradingSystem.tsId, c->sessionId);
	break;

    case CFETI_DISCONNECT_REJECTED:
	c = (CFETI_CONNECT_INFO *)cmdData;
	Logger::getLogger()->log(Logger::TYPE_ERROR,
				 "Disconnect from %d rejected - trading session id %d", 
				 c->tradingSystem.tsId, c->sessionId);
	break;

    case CFETI_QUERY_ACCEPTED:
    {
	unsigned int kdx;
	CFETI_QUERY_RESULT qr = (CFETI_QUERY_RESULT)cmdData;
	switch(qr->query.queryType) 
	{
	case CFETQ_TRADE:
	    Logger::getLogger()->log(Logger::TYPE_ERROR, "CFETQ_TRADE is not implemented yet");
	    break;
	case CFETQ_INSTRUMENT: {
	    int prevNumInstrument = ESCFETISession::numInstruments;
	    ESCFETISession::numInstruments += qr->result.instrument.numInstruments;
	    ESCFETISession::instArray = (char**)realloc(ESCFETISession::instArray, 
							ESCFETISession::numInstruments * 
							sizeof(char*));
	    
	    for (kdx = 0; kdx < qr->result.instrument.numInstruments; kdx++) 
	    {
		Logger::getLogger()->log(Logger::TYPE_INFO, "CFETQ_INSTRUMENT \t%s : %s", 
					 qr->result.instrument.instrumentList[kdx].name,
					 qr->result.instrument.instrumentList[kdx].description ? 
					 qr->result.instrument.instrumentList[kdx].description : 
					 "(no description)");

		int index  = prevNumInstrument + kdx;
		char* inst =qr->result.instrument.instrumentList[kdx].name;
		
		ESCFETISession::instArray[index] = new char[strlen(inst) +1];
		strcpy(ESCFETISession::instArray[index], inst);
	    }
	}
	break;

	default:
	    Logger::getLogger()->log(Logger::TYPE_ERROR, "Unknown query type");
	    break;
	}
    }
    break;

    case CFETI_SUBSCRIBE_ACCEPTED:
	_subscribedInst++;
	Logger::getLogger()->log(Logger::TYPE_INFO, "Subscribe Accepted : %s [%d == %d]", 
				 ((CFETI_INSTRUMENT_DATA)cmdData)->instName, _subscribedInst, 
				 ESCFETISession::numInstruments);

	OutputInstrumentData((CFETI_INSTRUMENT_DATA)cmdData);
	
	if( _subscribedInst == ESCFETISession::numInstruments)
	{
	    ESCFETISession::activated(true);
	    Logger::getLogger()->log(Logger::TYPE_INFO, 
				     "CFETI activated.  All subscriptions processed");
	}
	break;

    case CFETI_SUBSCRIBE_REJECTED:
	Logger::getLogger()->log(Logger::TYPE_ERROR, 
				 "Subscribe Rejected : \"%s\"", (CFETI_INSTRUMENT)cmdData);
	break;

    case CFETI_UPDATE:
	OutputInstrumentData((CFETI_INSTRUMENT_DATA)cmdData);
	break;

    case CFETI_INST_QUERY_COMPLETE:
	ESCFETISession::isQueryDone = true;
	break;

    default:
	break;
    }
}


void
ESCFETIProcessor::OutputInstrumentData(CFETI_INSTRUMENT_DATA instData)
{
    bool updated = false;

    Logger::getLogger()->log(Logger::TYPE_INFO, 
			     "ESCFETIProcessor::OutputInstrumentData Market Data Update : %s", 
			     instData->instName);
    unsigned short property = 0;
    int idx;
    ESInstrument *esI = _myProcessor->getESInstrument(instData->instName);

    if (esI == NULL)
    {
	Logger::getLogger()->log(Logger::TYPE_ERROR, 
	       "ESCFETIProcessor::OutputInstrumentData failed to getESInstrument for update %s\n", 
				 instData->instName);
	return;
    }
    
    if (ESCFETISession::code != 0) 
    {
	for (idx = 0; idx < instData->numFields; idx++) 
	{
	    if (instData->fieldTable[idx].fieldId == ESCFETISession::code) 
	    {
		property = instData->fieldTable[idx].fieldValue.uint16;
	    }
	}
    }
    
    for (idx = 0; idx < instData->numFields; idx++)
    {
	if (!updated) updated = true;

	unsigned short elemIndex = instData->fieldTable[idx].fieldId;

	switch(instData->fieldTable[idx].fieldType)
	{
	case CFETI_FIELDTYPE_INT8:
	    Logger::getLogger()->log(Logger::TYPE_INFO, 
				     "Ignoring field: %u : %d", 
				     instData->fieldTable[idx].fieldId, 
				     instData->fieldTable[idx].fieldValue.int8);
	    break;

	case CFETI_FIELDTYPE_BYTE:
	    Logger::getLogger()->log(Logger::TYPE_INFO, 
				     "Ignoring field: %u : %u", 
				     instData->fieldTable[idx].fieldId, 
				     instData->fieldTable[idx].fieldValue.byte);
	    break;

	case CFETI_FIELDTYPE_UINT16:
	{
	    int iVal = instData->fieldTable[idx].fieldValue.uint16;
	    esI->setValByIndex(iVal, elemIndex);	
	}
	break;
	
	case CFETI_FIELDTYPE_INT16:
	{
	    int iVal = instData->fieldTable[idx].fieldValue.uint16;
	    esI->setValByIndex(iVal, elemIndex);	
	}
	break;
	case CFETI_FIELDTYPE_INT32:
	{
	    int iVal = instData->fieldTable[idx].fieldValue.int32;
	    esI->setValByIndex(iVal, elemIndex);	
	}
	break;

	case CFETI_FIELDTYPE_UINT32:
	    Logger::getLogger()->log(Logger::TYPE_INFO, 
				     "Ignoring Field: %u : %u", 
				     instData->fieldTable[idx].fieldId, 
				     instData->fieldTable[idx].fieldValue.uint32);
	    break;

	case CFETI_FIELDTYPE_DATETIME:
	{
	    long lVal = instData->fieldTable[idx].fieldValue.dateTime;
	    esI->setValByIndex(lVal, elemIndex);	
	}
	break;

	case CFETI_FIELDTYPE_STRING:
	{
	    std::string str = instData->fieldTable[idx].fieldValue.string;
	    esI->setValByIndex(str, elemIndex);
	}
	break;

	case CFETI_FIELDTYPE_BUFFER:
	{
	    if (instData->fieldTable[idx].fieldId == CFETF_BID_LIST_1)
	    {
		Logger::getLogger()->log(Logger::TYPE_INFO, 
					 "Ignoring Field: %u : (Buffer, length %d)", 
					 instData->fieldTable[idx].fieldId, 
					 instData->fieldTable[idx].fieldValue.buffer.len);
	    }


	    CFETI_PARTICIPANT_LIST pList = NULL;
	    CFETI_RC rc = CFETIDecodeFieldExt(ESCFETISession::currentSession, 
					      ESCFETISession::currentTradingSession, 
					      &instData->fieldTable[idx], (void*)&property, 
					      (void **) &pList);

	    if (rc == CFETI_SUCCESS)
	    {
		for (unsigned int jdx = 0; jdx < pList->numParticipants; jdx++)
		    Logger::getLogger()->log(Logger::TYPE_INFO, 
					     "No handler \t%d: %d %f %d %d", jdx, 
					     pList->participant[jdx].tsId, 
					     pList->participant[jdx].size, 
					     pList->participant[jdx].code, 
					     pList->participant[jdx].counterpartyState);
	    } 
	    else 
	    {
		Logger::getLogger()->log(Logger::TYPE_ERROR, 
					 "\tError %d : %s\n", rc, CFETIGetLastError());
	    }
	}
	break;

	case CFETI_FIELDTYPE_DECIMAL:
	{
	    double dVal = instData->fieldTable[idx].fieldValue.decimal;
	    esI->setValByIndex(dVal, elemIndex);
	}
	break;

	default:
	    Logger::getLogger()->log(Logger::TYPE_INFO, 
				     "Ignoring Field: %u : (unknown data type)", 
				     instData->fieldTable[idx].fieldId);
	    break;
	}
    }

    if(updated)
	esI->update();
    
    return;
}

void*
ESCFETIProcessor::MessageLoop(void *arg)
{
	CFETIMessageLoop();
	return 0;
}


ESInstrument *
ESCFETIProcessor::getESInstrument(const char *label)
{
	ESInstrument *es  = NULL;
	map<string, ESInstrument* >::iterator it = _mapInst.find(label);
	if (it != _mapInst.end()){
		es = (*it).second;
	} else {
		es = new ESInstrument(label);
		if (es->activate()){
			_mapInst[label] = es;
			Logger::getLogger()->log(Logger::TYPE_INFO, "ESCFETIProcessor::getESInstrument add %s", label);
		}else{
			delete es;
			es = NULL;
			Logger::getLogger()->log(Logger::TYPE_ERROR, "ESCFETIProcessor::getESInstrument failed add %s", label);
		}

	}
	return es;

}

const char*
ESCFETIProcessor::getStateInfo(int state)
{
	static char tmpBuf[512];
	int len = 0;
	if (CFETI_IS_NO_MARKET(state))
		len = sprintf(tmpBuf, "No Market");
	else
	if (CFETI_IS_ERROR(state))
		len = sprintf(tmpBuf, "ERROR");
	else
	if (CFETI_IS_BID_AND_OFFER(state))
		len = sprintf(tmpBuf, "Bid and Offer");
	else
	if (CFETI_IS_UNCLEAR_BID(state))
		len = sprintf(tmpBuf, "Unclear Bid");
	else
	if (CFETI_IS_BID(state))
		len = sprintf(tmpBuf, "Bid");
	else
	if (CFETI_IS_UNCLEAR_OFFER(state))
		len = sprintf(tmpBuf, "Unclear Offer");
	else
	if (CFETI_IS_OFFER(state))
		len = sprintf(tmpBuf, "Offer");
	/*  */	

	if (CFETI_IS_BUY(state))
		len = sprintf(tmpBuf, "%s", "Buy");
	else
	if (CFETI_IS_SELL(state))
		len = sprintf(tmpBuf, "%s", "Sell");
	if (CFETI_IS_CHECK_CREDIT(state))
		len += sprintf(tmpBuf+len, "%s%s", (len == 0) ? "":"|", "Check Credit");
	return tmpBuf;

}

