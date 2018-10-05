
/************************************************************************
||
|| DATE:                $Date: 2006/06/07 14:29:11 $
|| SOURCE:              $Source: /repository/projects/Canpx/espeedII/ESpeedApp.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: ESpeedApp.hpp,v 1.4 2006/06/07 14:29:11 nickz Exp $
|| REVISION:    $Revision: 1.4 $
|| LOG:                 $Log: ESpeedApp.hpp,v $
|| LOG:                 Revision 1.4  2006/06/07 14:29:11  nickz
|| LOG:                 Ported to managed published
|| LOG:
|| LOG:                 Revision 1.3  2006/04/19 01:13:47  nickz
|| LOG:                 .
|| LOG:
|| LOG:                 Revision 1.2  2006/04/11 16:03:56  nickz
|| LOG:                 Ported to RMDS (not tested)
|| LOG:
|| LOG:                 Revision 1.1  2005/11/17 21:57:58  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#ifndef ESPEEDAPP_HPP
#define ESPEEDAPP_HPP

#include <vector>
#include <string>

#include "esRecordProducer.hpp"
#include "Application.hpp"
#include "Logger.hpp"
#include "ConfigMap.hpp"
#include "ESOpenServer.hpp"
#include "ESCFETISession.hpp"
#include "Datum.hpp"
#include "subjectPubMgr.hpp"

using std::vector;
using std::string;

class ESpeedApp: public Application
{
public:
    ESpeedApp(char *);
    virtual ~ESpeedApp();
    virtual void onInit(const ConfigMap& configMap);
    virtual const char *const className(){return "ESpeedApp";}
    
    virtual void shutdown();
    virtual void loadDataProducer();
    virtual void sessionExpired(ESCFETISession *session);
    virtual void subscribeRecords();
protected:
    virtual void onRead(int fd, InputId id){};
    virtual void onWrite(int fd, InputId id){};
    virtual void onExcept(int fd, InputId id){};
    virtual void onTimer(TimerId id);
    
private:
    esRecordProducer* _rmdsDataProducer;
    subjectPubMgr*	_subjPubManager;
    ESOpenServer * _myServer;
    ESCFETISession * _cfetiSession;
    vector<long> _tV;
};

extern ESpeedApp *theESpeedApp;
#endif // ESPEEDAPP_HPP
