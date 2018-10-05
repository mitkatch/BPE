
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

#ifndef FTMANAGER_HPP
#define FTMANAGER_HPP


#include "ConfigMap.hpp"
#include "OpenClient.hpp"
#include "TimerCallback.hpp"
#include "IOCallback.hpp"

class FTManager: public TimerCallback, public IOCallback
{
	public:
		struct FT {
			int status;
			string host;
			string port;
		} ;
		enum status {	UNKNOWN, 
				PRIMARY, 
				SECONDARY};

		virtual ~FTManager();
		static FTManager* getFTManager();

		virtual FTManager::FT getFT();
	protected:
		FTManager();
		static FTManager* _myft;
		virtual void onTimer(TimerId id);
		class FTOpenClient;
		friend class FTOpenClient;
		class FTOpenClient: public OpenClient
		{
			public:
				FTOpenClient(){};
				virtual ~FTOpenClient(){};	
				virtual void onRead(int fd, InputId id);
				virtual void onWrite(int fd, InputId id){};
				virtual void onExcept(int fd, InputId id){};
			protected:
				string _strBuffer;
		};

		class FTPublisher;
		friend class FTPublisher;
		class FTPublisher: public IOCallback 
		{
			public:
				FTPublisher(){};
				virtual ~FTPublisher(){};	
				void publish(const string& str);
				void setFD(int fd){
					_pfd = fd;
				}
			protected:
				virtual void onRead(int fd, InputId id){};
				virtual void onWrite(int fd, InputId id);
				virtual void onExcept(int fd, InputId id){};
				string _pubstr;
				int _pfd;
		};

		int status;
		string _host, _port, _request;
		int _timer;
		int _myfd;
		FT _ftinst;
		FTPublisher* _publisher;
		FTOpenClient* _client;
};
#endif //FTMANAGER_HPP
