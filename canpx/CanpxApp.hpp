/************************************************************************
||
|| DATE:                $Date: 2006/06/08 15:29:16 $
|| SOURCE:              $Source: /repository/projects/Canpx/canpx/CanpxApp.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: CanpxApp.hpp,v 1.6 2006/06/08 15:29:16 nickz Exp $
|| REVISION:    $Revision: 1.6 $
|| LOG:                 $Log: CanpxApp.hpp,v $
|| LOG:                 Revision 1.6  2006/06/08 15:29:16  nickz
|| LOG:                 ported to managed publishing
|| LOG:
|| LOG:                 Revision 1.5  2006/06/05 13:18:36  nickz
|| LOG:                 Ported record publisher as managed
|| LOG:
|| LOG:                 Revision 1.4  2006/04/01 12:11:52  nickz
|| LOG:                 Changed to make RMDS aware.
|| LOG:
|| LOG:                 Revision 1.3  2005/11/21 16:49:45  mikhailt
|| LOG:                 normilized TIME_FORMAT
|| LOG:
|| LOG:                 Revision 1.2  2005/09/12 21:57:06  mikhailt
|| LOG:                 builds ldap chain
|| LOG:
|| LOG:                 Revision 1.1  2005/06/24 16:21:53  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#ifndef CANPXAPP_HPP 
#define CANPXAPP_HPP 

// System includes
#include <vector>

// Application includes
#include "rmdsRecordHandler.hpp"
#include "canpxRecordProducer.hpp"
#include "Application.hpp"
#include "Logger.hpp"
#include "ConfigMap.hpp"
#include "CanpxSubjectSubMgr.hpp"
#include "CanpxOpenServer.hpp"


#define TIME_FORMAT	"%d/%m/%y %H:%M:%S"

class CanpxApp: public Application
{
	public:
		CanpxApp(char *);
		virtual ~CanpxApp();
		virtual void onInit(const ConfigMap& configMap);
		virtual const char *const className(){return "CanpxApp";}

		virtual const char *name() const {
			return _producer.c_str();
		}

		virtual void shutdown();

		virtual void broadcastControlMsg(const char* type);

    		virtual void subscribeRecords();

	protected:
		virtual void onRead(int fd, InputId id){};
		virtual void onWrite(int fd, InputId id){};
		virtual void onExcept(int fd, InputId id){};

	private:
		bool            _init;
		canpxRecordProducer* _rmdsProducer; 
		rmdsRecordHandler* _rmdsHandler; 
                CanpxSubjectSubMgr* _subjSubMgr;
		CanpxOpenServer 	* _myServer;
    		string		_producer;
};

extern CanpxApp *theCanpxApp;

#endif //PAGEPARSER_HPP
