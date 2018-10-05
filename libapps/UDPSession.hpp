/************************************************************************
||
|| DATE:                $Date: 2004/07/30 16:16:06 $
|| SOURCE:              $Source: /repository/projects/Canpx/libapps/UDPSession.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: UDPSession.hpp,v 1.1 2004/07/30 16:16:06 mikhailt Exp $
|| REVISION:    $Revision: 1.1 $
|| LOG:                 $Log: UDPSession.hpp,v $
|| LOG:                 Revision 1.1  2004/07/30 16:16:06  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#ifndef UDPSESSION_HPP
#define UDPSESSION_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/un.h>
#include <stdlib.h>
#include <sys/ioctl.h>

#include <strings.h>

#include <vector>

#include "IOCallback.hpp"
#include "TimerCallback.hpp"
#include "ConfigMap.hpp"
#include "Logger.hpp"
#include "MCMsg.hpp"



using std::string;
using std::vector;

#define SA struct sockaddr
#define SALen	socklen_t

#define MAXLINE 4096
#define MAXSOCKADDR 128

// it creates UDP socket binds to specified in the config multicasting group
// and listens for all messages in this group
// if we need to publish we derive from this class and add publisher

using std::vector;

class UDPSession: public IOCallback 
{
	public:
		enum udp_type {UDP_UNKNOWN_TYPE 	= 0x00,
				UDP_SUBSCRIBER_TYPE	= 0x01,
				UDP_PUBLISHER_TYPE	= 0x10
		};
		UDPSession(const ConfigMap &confmap);
		virtual ~UDPSession();


		virtual bool initSession() = 0;

		virtual int udpRecvFd() const {
			return _recvfd;
		} 

		virtual int udpSendFd() const {
			return _sendfd;
		} 

		virtual SA* saSend() const {
			return _saSend;
		}

		virtual SALen saSendLen() const {
			return _saSendLen;
		}

		virtual void setPName(const string& name){
			_p_name = name;
		}

	protected:

		virtual void processUDP(const string& str, const string& line);

		virtual void setMCIP(const string& str);
		virtual void setMCPort(const string& str);

		virtual bool initUDP(int type);

		class UDPSessionPublisher: public IOCallback
		{
		   public:
			UDPSessionPublisher(UDPSession *session, int fd):
				_mySession(session), _sendFd(fd){};
			virtual ~UDPSessionPublisher();
			virtual void publish(MCMsg *msg);
		   protected:
			virtual void onRead(int fd, InputId id){};
			virtual void onWrite(int fd, InputId id);
			virtual void onExcept(int fd, InputId id){};
			UDPSession *_mySession;	
			int _sendFd;
			vector<MCMsg*>	_msglist;
		} * _publisher;

	protected:
		virtual void onRead(int fid, InputId id);
		virtual void onWrite(int fid, InputId id){};
		virtual void onExcept(int fid, InputId id){};

		string _p_name;
	private:
		virtual int udp_client( const char *host, const char *serv, void **saptr, socklen_t *lenp);
		virtual int mcast_join(int sockfd, const SA* sa, socklen_t salen,const char *ifname, u_int ifindex);
		virtual int mcast_set_loop(int sockfd, int onoff);
		virtual int sockfd_to_family(int sockfd);
		char *sock_ntop(const struct sockaddr *sa, socklen_t salen);

		string 	_mc_ip;
		string 	_mc_port;
		int 	_recvfd, _sendfd;
		SA	*_saSend;
		SA	*_saRecv;
		SALen	_saSendLen;
};
#endif //UDPSESSION_HPP
