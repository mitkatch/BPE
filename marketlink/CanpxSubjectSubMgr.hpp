/************************************************************************
||
|| DATE:                $Date: 2006/06/08 15:29:17 $
|| SOURCE:              $Source: /repository/projects/Canpx/canpx/CanpxSubjectSubMgr.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: CanpxSubjectSubMgr.hpp,v 1.3 2006/06/08 15:29:17 nickz Exp $
|| REVISION:		$Revision: 1.3 $
|| LOG:                 $Log: CanpxSubjectSubMgr.hpp,v $
|| LOG:                 Revision 1.3  2006/06/08 15:29:17  nickz
|| LOG:                 ported to managed publishing
|| LOG:
|| LOG:                 Revision 1.2  2006/05/02 15:10:38  nickz
|| LOG:                 testing
|| LOG:
|| LOG:                 Revision 1.1  2006/04/01 12:11:54  nickz
|| LOG:                 Changed to make RMDS aware.
|| LOG:
|| LOG:
************************************************************************/

#ifndef _CANPX_SUBJECT_SUB_MGR_HPP_
#define _CANPX_SUBJECT_SUB_MGR_HPP_

// System includes
#include <map>

// Application includes
#include "subjectSubMgr.hpp"
#include "CanpxInstrument.hpp"

class CanpxApp;

using std;

class CanpxSubjectSubMgr : public subjectSubMgr
{
public:	
    CanpxSubjectSubMgr(const std::string& name, CanpxApp* app);
    virtual ~CanpxSubjectSubMgr();
    
public:
    // Virtual override
    virtual void registerAddress(const std::string& contributor, const std::string& type, 
				 SMspace& collection);
    virtual void registerAddress(const std::string& contributor, const std::string& type, 
				 const std::string& service, const std::string& item);
    virtual void onTimer(TimerId id);

public:
    // Notification that we have received a FULL IMAGE for the instrument
    bool toChain(CanpxInstrument* i);

    // Determine if we have received out complete list (chain) of subjects
    bool chainLoaded();

private:
    struct Instrument
    {
	// Item type.  Eg: tbill, prov, bond etc
	std::string	type;

	// Item as identified globally.  Example:  5.Aug11
	std::string	item;

	// Determines if we have received a full IMAGE for this instrument.
	bool		imageProcessed;
    };

private:
    typedef map<string, Instrument>	RealAddrMap;

    // The RealItemMap contains a collection of unique entries required by the Canpx engine.
    // The map contains a collection of name/value pairs representing the <address>/<instrument> pair.
    // Where:
    //		<address>:	service.<real item>  eg:  P_SHORCAN.Aug11
    //		<instrument>:	Instrument (see above)
    //
    // <real item> represents the common name across multiple pages within a specified source.
    //
    // Note: Page sources typically contain the same items on different pages.  For example,
    //       2 SHORCAN pages could both contain Aug11 data which is always identical.  As a result, 
    //       tHE 2 records, P_SHORCAN.1.Aug11 and P_SHORCAN.4.Aug11, for example, are always identical.
    // 	     The value Aug11 represents the common name or real item.
    //
    //       The purpose of this routine is to eliminate the need to subscribe to identical records.
    //
    RealAddrMap		m_realItemMap;

private:
    CanpxApp*		m_canpxApp;
};


#endif // _CANPX_SUBJECT_SUB_MGR_HPP_
