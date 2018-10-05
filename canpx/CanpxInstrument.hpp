/************************************************************************
||
|| DATE:                $Date: 2006/04/01 12:11:53 $
|| SOURCE:              $Source: /repository/projects/Canpx/canpx/CanpxInstrument.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: CanpxInstrument.hpp,v 1.3 2006/04/01 12:11:53 nickz Exp $
|| REVISION:    $Revision: 1.3 $
|| LOG:                 $Log: CanpxInstrument.hpp,v $
|| LOG:                 Revision 1.3  2006/04/01 12:11:53  nickz
|| LOG:                 Changed to make RMDS aware.
|| LOG:
|| LOG:                 Revision 1.2  2005/10/04 16:51:28  mikhailt
|| LOG:                 prov added
|| LOG:
|| LOG:                 Revision 1.1  2005/06/24 16:21:53  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#ifndef CANPXINSTRUMENT_HPP
#define CANPXINSTRUMENT_HPP

#include "DataCache.hpp"
#include "Datum.hpp"
#include "DataGraph.hpp"
#include "Subscriber.hpp"
#include "TimerCallback.hpp"

#define _ML_GRAPH_ADDRESS_	"ML_GRAPH_ADDRESS"
#define _INST_CODE_		"ROW64_1"
#define _SYMBOL_		"SECUR_NAME"
#define _SOURCE_		"ROW64_2"
#define _DSPLY_NAME_		"DSPLY_NAME"
#define _BENCHMARK_		"ROW66_2"
#define _BID_			"BID" 
#define _BIDSIZE_		"BIDSIZE"
#define _BID_YIELD_		"BID_YIELD"
#define _BID_TIME_		"ROW66_3"
#define _LAST_BID_		"ROW64_3" 
#define _LAST_BID_YIELD_	"ROW64_4"
#define _LAST_BID_TIME_		"ROW66_5"
#define _CLOSE_BID_		"ROW64_5" 
#define _CLOSE_BID_YIELD_	"ROW64_6"
#define _CLOSE_BID_TIME_	"ROW66_4"
#define _BID_CHG_		"BID_NET_CH" 
#define _BID_YIELD_CHG_		"YLD_NETCHG"
#define _ASK_			"ASK" 
#define _ASKSIZE_		"ASKSIZE"
#define _ASK_YIELD_		"ASK_YIELD"
#define _ASK_TIME_		"ROW66_1"
#define _MASTER_TRADE_PRICE_	"ROW64_7" 
#define _MASTER_TRADE_SIZE_	"ROW64_8"
#define _MASTER_TRADE_YIELD_	"ROW64_9"
#define _MASTER_TRADE_TIME_ST_	"ROW66_12"
#define _MASTER_TRADE_TIME_END_	"ROW66_11"
#define _MASTER_TRADE_TONE_	"ROW66_13"
#define _MASTER_TRADE_SOURCE_	"ROW66_10"
#define _TRADE_PRICE_		"ROW66_15" 
#define _TRADE_SIZE_		"ROW66_16"
#define _TRADE_YIELD_		"ROW66_21"
#define _TRADE_TIME_ST_		"ROW66_19"
#define _TRADE_TIME_END_	"ROW66_18"
#define _TRADE_TONE_		"ROW66_20"
#define _TRADE_SOURCE_		"ROW66_17"
#define _LAST_TRADE_PRICE_	"ROW66_22" 
#define _LAST_TRADE_SIZE_	"TRDVOL_2"
#define _LAST_TRADE_YIELD_	"ROW64_10"
#define _LAST_TRADE_TIME_ST_	"ROW66_8"
#define _LAST_TRADE_TIME_END_	"ROW66_7"
#define _LAST_TRADE_TONE_	"ROW66_9"
#define _LAST_TRADE_SOURCE_	"ROW66_6"
#define _AVOL_			"ACVOL_1"
#define _TIMESTAMP_		"ROW64_12"
#define _ML_RAW_DATA_		"ROW64_11"

class CanpxSubjectSubMgr;

class CanpxInstrument: public Subscriber, public TimerCallback
{
public:
    CanpxInstrument(const string& address, const string& realName, 
		    const string& item, CanpxSubjectSubMgr* subjSubMgr);
    virtual ~CanpxInstrument();

public:
    enum {CLEAR = 0x0, HIT = 0x01, TAKE = 0x10};

public:
    
    virtual string name()const
	{
	    return _name;
	}

    virtual string realName() const
	{
	    return (_realName);
	}
    virtual void setIDNLabel(const string& label){
		_idnLabel = label;
    }

    virtual string getIDNLabel(){
		return _idnLabel;
    }

    virtual void notify(Datum *datum);
    virtual void onTimer(TimerId id);
    
    virtual DataGraph* getDataGraph()const
	{
	    return _dataGraph;
	}

    virtual string getInstrumentCode() const
	{
	    return _instCode;
	}
    
    virtual bool active() const 
	{
	    return(_dataGraph != NULL);
	}
    
    virtual void printTable();
    virtual string bidString()const ;
    virtual string askString()const ;
    virtual string bidSizeString()const ;
    virtual string askSizeString()const ;
    virtual double ask() const;
    virtual double bid() const;
    virtual double askSize() const;
    virtual double bidSize() const;
    virtual double askYield() const;
    virtual double bidYield() const;
    virtual int tradeTone() const;
    virtual double tradePrice(int tt)const;
    virtual double tradeSize(int tt) const;
    virtual double tradeYield(int tt) const;
    virtual string getFieldStr(const string& f);

protected:
    string _name;
    string _realName;
    string _instCode;
    string _idnLabel;
    bool _active;
    DataGraph* _dataGraph;
    CanpxSubjectSubMgr* _subjSubMgr;
};

#endif //CANPXINSTRUMENT_HPP


