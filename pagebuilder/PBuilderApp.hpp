
/************************************************************************
||
|| DATE:                $Date: 2006/06/08 15:29:48 $
|| SOURCE:              $Source: /repository/projects/Canpx/pagebuilder/PBuilderApp.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: PBuilderApp.hpp,v 1.5 2006/06/08 15:29:48 nickz Exp $
|| REVISION:    $Revision: 1.5 $
|| LOG:                 $Log: PBuilderApp.hpp,v $
|| LOG:                 Revision 1.5  2006/06/08 15:29:48  nickz
|| LOG:                 ported to managed publishing
|| LOG:
|| LOG:                 Revision 1.4  2006/04/06 17:57:29  nickz
|| LOG:                 port to RMDS
|| LOG:
|| LOG:                 Revision 1.3  2006/04/04 13:19:32  nickz
|| LOG:                 ported to rmds
|| LOG:
|| LOG:                 Revision 1.2  2005/09/21 17:49:55  mikhailt
|| LOG:                 activated only when loaded()
|| LOG:
|| LOG:                 Revision 1.1  2005/06/29 19:00:48  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#ifndef PBUILDER_HPP
#define PBUILDER_HPP

// Application includes
#include "pbPageProducer.hpp"
#include "PBrmdsRecordHandler.hpp"
#include "Application.hpp"
#include "Logger.hpp"
#include "ConfigMap.hpp"
#include "PageArrayConverter.hpp"

class PBuilder: public Application
{
	public:
		PBuilder(char*);
		virtual ~PBuilder();
		virtual void onInit(const ConfigMap& configMap);
		virtual const char *const className(){return "PBuilder";}
                virtual void subscribeRecords();
	protected:
		virtual void onRead(int fd, InputId id){};
		virtual void onWrite(int fd, InputId id){};
		virtual void onExcept(int fd, InputId id){};

	private:
		const ConfigMap *_configMap;
		bool		_init;
		pbPageProducer* _rmdsPageProducer;
		PBrmdsRecordHandler * _rmdsRecordHandler;
		PageArrayConverter *_pArrayConverter;
};

extern PBuilder *thePageBuilder;

#endif //
