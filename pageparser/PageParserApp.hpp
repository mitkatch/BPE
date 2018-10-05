/************************************************************************
||
|| DATE:                $Date: 2006/06/04 11:27:06 $
|| SOURCE:              $Source: /repository/projects/Canpx/pageparser/PageParserApp.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: PageParserApp.hpp,v 1.8 2006/06/04 11:27:06 nickz Exp $
|| REVISION:    $Revision: 1.8 $
|| LOG:                 $Log: PageParserApp.hpp,v $
|| LOG:                 Revision 1.8  2006/06/04 11:27:06  nickz
|| LOG:                 .
|| LOG:
|| LOG:                 Revision 1.7  2006/04/01 12:10:09  nickz
|| LOG:                 continue building and testing...
|| LOG:
|| LOG:                 Revision 1.6  2006/03/23 19:30:16  nickz
|| LOG:                 testing...
|| LOG:
|| LOG:                 Revision 1.5  2006/03/18 13:19:57  nickz
|| LOG:                 Added RMDS-based functionality
|| LOG:
|| LOG:                 Revision 1.4  2006/03/06 16:01:46  nickz
|| LOG:                 Updating interface to RMDS
|| LOG:
|| LOG:                 Revision 1.3  2005/03/29 16:01:41  mikhailt
|| LOG:                 sending broadcast LOADED
|| LOG:
|| LOG:                 Revision 1.2  2004/11/15 17:40:09  mikhailt
|| LOG:                 more-less stable
|| LOG:
|| LOG:                 Revision 1.1  2004/10/06 19:44:25  mikhailt
|| LOG:                 Initial revision
|| LOG:
|| LOG:                 Revision 1.1  2004/10/06 19:42:08  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#ifndef _PAGE_PARSER_HPP_
#define _PAGE_PARSER_HPP_

// System includes
#include <vector>

// Application includes
#include "rmdsPageHandler.hpp"
#include "ppRecordProducer.hpp"
#include "Application.hpp"
#include "Logger.hpp"
#include "ConfigMap.hpp"
#include "ParserOpenServer.hpp"
#include "NSMap.hpp"
#include "Datum.hpp"
#include "PageListener.hpp"
#include "subjectPubMgr.hpp"

// Config reference
#define URL "URL."
#define URL_NAME ".NAME"
#define URL_SOURCE ".SOURCE"
#define URL_PAGE ".PAGE"
#define URL_PARSER ".PARSER"
#define URL_PARSER_MAP ".PARSER_MAP"
#define URL_SUBSCRIBER ".SUBSCRIBER"


class PageParserApp: public Application, public Subscriber
{
public:
    PageParserApp(char *);
    virtual ~PageParserApp();
    virtual void onInit(const ConfigMap& configMap);
    virtual const char *const className(){return "PageParserApp";}
    
    virtual rmdsPageHandler* getRmdsHandler() const {
	return _rmdsHandler;
    }
    
    virtual NSMap getNameSpace() const {
	return _my_namespace;
    }
    
    virtual void notify(Datum *datum);
    
    virtual void shutdown();

    virtual void subscribePages();

protected:
    virtual void onRead(int fd, InputId id){};
    virtual void onWrite(int fd, InputId id){};
    virtual void onExcept(int fd, InputId id){};
    virtual void onTimer(TimerId id){};
    
    virtual void buildNameSpace(const ConfigMap &confMap);
    virtual void broadcastControlMsg(const char* type);
    Logger *_myLog;
    NSMap _my_namespace;
    
private:
    const ConfigMap*		_configMap;
    bool            		_init;
    rmdsPageHandler*		_rmdsHandler;
    ppRecordProducer* 		_rmdsDataProducer; 
    subjectPubMgr*		_subjPubManager;
    ParserOpenServer*		_myServer;
    vector<PageListener *> 	_vListeners;
};

extern PageParserApp *thePageParserApp;

#endif // _PAGE_PARSER_HPP_



