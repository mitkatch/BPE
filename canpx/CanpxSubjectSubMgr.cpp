/************************************************************************
||
|| DATE:                $Date: 2006/06/08 15:29:16 $
|| SOURCE:              $Source: /repository/projects/Canpx/canpx/CanpxSubjectSubMgr.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: CanpxSubjectSubMgr.cpp,v 1.3 2006/06/08 15:29:16 nickz Exp $
|| REVISION:		$Revision: 1.3 $
|| LOG:                 $Log: CanpxSubjectSubMgr.cpp,v $
|| LOG:                 Revision 1.3  2006/06/08 15:29:16  nickz
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

// Application includes
#include "CanpxApp.hpp"
#include "CanpxSubjectSubMgr.hpp"
#include "Logger.hpp"
#include "ChainManager.hpp"
#include "Reflection.hpp"

CanpxSubjectSubMgr::CanpxSubjectSubMgr(const string& name, CanpxApp* app) : 
    subjectSubMgr(name)
{
    assert(app != NULL);
    m_canpxApp = app;
}

CanpxSubjectSubMgr::~CanpxSubjectSubMgr()
{
}


/*****************************************************************************
 * registerAddress()
 *
 * Register address within data cache.  Address will be monitored by the 
 * application for IMAGE and UPDATE events.
 *****************************************************************************/
void
CanpxSubjectSubMgr::registerAddress(const string& contributor, const string& type, 
				    SMspace& collection)
{
    Logger::getLogger()->log(Logger::TYPE_INFO,
			     "Request for LIST type: %s returned %d entries from Contributor: %s",
			     type.c_str(), collection.size(), contributor.c_str());

    // Iterate through the subject space passed and build the chain or collection
    // of real items.
    SMspace::iterator itr;

    for ( itr = collection.begin(); itr != collection.end(); itr++ )
    {
	// Register an item entry
	registerAddress( contributor, type, (*itr).first, (*itr).second );
    }

    // Register details for administrator interface
    StringCheck out(contributor, true);
    Reflection::getReflection()->addSetVar("Reflection::contribList", out); 
}

/***************************************************************************
 * registerAddress
 *
 * Register the service/item pair with this manager.  The service represents
 * the SERVICE such as: SHORCAN, PREBON, CANTOR.  The item represents the
 * full item name within the SERVICE, ie  5.AUG11, 157.SAS_303 etc.
 ***************************************************************************/
void
CanpxSubjectSubMgr::registerAddress(const string& contributor, const string& type, 
				    const string& service, const string& item)
{
    string realItem;
    
    // Determine the real item
    int dot = item.find_first_of(".", 0);
    if (dot != string::npos)
	realItem = item.substr(dot +1, item.size() - (dot +1));
    else
	realItem = item;
    
    // Build the unique address (key) to our map
    string uaddress = service + "." + realItem;
    
    // Now determine if the address is already registered
    RealAddrMap::iterator iter = m_realItemMap.find(uaddress);
    
    if ( iter == m_realItemMap.end( ) )
    {
	// No.  Address not register.  Add.
	Instrument instr;
    
	instr.type = type;
	instr.item = item;
	instr.imageProcessed = false;
	
	// No, let's register
	m_realItemMap[uaddress] = instr;
	
	string address = service + "." + item;
    
	// Subscribe to this address
	CanpxInstrument* i = new CanpxInstrument(address, uaddress, realItem, this);

	DataCache::getCache()->subscribe(address, i, Datum::GRAPH);
    }
}


bool
CanpxSubjectSubMgr::toChain(CanpxInstrument *i)
{
    bool ret = false;
    
    RealAddrMap::iterator itr = m_realItemMap.find(i->name());
    if ( itr != m_realItemMap.end())
    {
	(*itr).second.imageProcessed = ret = true;
    }
    else 
    {
	Logger::getLogger()->log(Logger::TYPE_ERROR, 
				 "CanpxSubjectSubMgr::toChain %s not registered for chain", 
				 i->name().c_str());
    }
    
    return ret;
}


bool
CanpxSubjectSubMgr::chainLoaded()
{
    bool ret = (m_realItemMap.size() > 0);
    int reg = 0, unreg = 0;

    RealAddrMap::iterator it = m_realItemMap.begin();
    for (it; it != m_realItemMap.end(); it++)
    {
	ret &= (*it).second.imageProcessed;

	if ((*it).second.imageProcessed)
	    reg++;
	else
	    unreg++;
    }

    cout << "CanpxSubjectSubMgr::chainLoaded() registerd " << reg 
	 << ", unregisterd " << unreg << endl;
    
    return ret;
}

void
CanpxSubjectSubMgr::onTimer(TimerId id)
{
    if (chainLoaded())
    {
	if (ConfigMap::getConfig()->getValue("CHAIN_DYNAMIC") == "TRUE")
	{
	    ChainManager::getChain()->update();
	    m_canpxApp->broadcastControlMsg("UPDATED");
	}
    } 
    else 
    {
	addTimer(1*1000, 1, this);	
    }
}
