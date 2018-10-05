
/************************************************************************
||
|| DATE:                $Date:$
|| SOURCE:              $Source:$
|| STATE:               $State:$
|| ID:                  $Id:$
|| REVISION:            $Revision:$
|| LOG:                 $Log:$
|| LOG:
************************************************************************/

#ifndef MKLSUBJECTSUBMGR_HPP 
#define MKLSUBJECTSUBMGR_HPP 

// System includes
#include <map>

// Application includes
#include "subjectSubMgr.hpp"

class MKLFeedApp;

using std;

class MKLSubjectSubMgr : public subjectSubMgr
{
public:	
    MKLSubjectSubMgr(const std::string& name, MKLFeedApp* app);
    virtual ~MKLSubjectSubMgr();
    
public:
    // Virtual override
    virtual void registerAddress(const std::string& contributor, const std::string& type, 
				 SMspace& collection);
    virtual void registerAddress(const std::string& contributor, const std::string& type, 
				 const std::string& service, const std::string& item);
    virtual void onTimer(TimerId id);
    virtual string contributor(const string& item);

public:
    bool chainLoaded();
	bool imageProcessed(const string& i);
private:
    struct Instrument
    {
	// Item type.  Eg: tbill, prov, bond etc
	std::string	type;

	// Item as identified globally.  Example:  5.Aug11
	std::string	item;

	// Determines if we have received a full IMAGE for this instrument.
	bool		imageProcessed;
	std::string	contributor;
    };

private:
    typedef map<string, Instrument>	RealAddrMap;

    RealAddrMap		m_realItemMap;

private:
    MKLFeedApp*		m_mklFeedApp;
    string 		_contributor;
};


#endif // MKLSUBJECTSUBMGR_HPP 
