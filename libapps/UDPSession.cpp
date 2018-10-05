/************************************************************************
||
|| DATE:                $Date: 2004/07/30 16:14:57 $
|| SOURCE:              $Source: /repository/projects/Canpx/libapps/UDPSession.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: UDPSession.cpp,v 1.1 2004/07/30 16:14:57 mikhailt Exp $
|| REVISION:    $Revision: 1.1 $
|| LOG:                 $Log: UDPSession.cpp,v $
|| LOG:                 Revision 1.1  2004/07/30 16:14:57  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#include "UDPSession.hpp"

#include <net/if.h>
#include <sys/types.h>
#include <unistd.h>


UDPSession::UDPSession(const ConfigMap &confmap)
{

	//get something from config
	_saSend = NULL;
	_saRecv = NULL;
}

UDPSession::~UDPSession()
{
	//cout<<"Close UDP file discriptor "<<_recvfd<<endl;
	close(_recvfd);
	close(_sendfd);
	delete _saSend;
	delete _saRecv;
}

bool
UDPSession::initUDP(int type)
{
	bool ret = true;

	if (_mc_port.empty() || _mc_ip.empty()){
		Logger::getLogger()->log(Logger::TYPE_ERROR, "UDPSession::initUDP MULTICAST_GROUP_PORT = %s, MULTICAST_GROUP_IP = <%s>", _mc_port, _mc_ip.c_str());	
		return false;
	}

	//////////////////////////////////
	//int sendfd, recvfd;
	const int on = 1;
	unsigned char ttl = 1;
	//socklen_t salen;
	//SA *sasend, *sarecv;
		
	_sendfd = udp_client(_mc_ip.c_str(), _mc_port.c_str(), (void **)&_saSend, &_saSendLen);
	_recvfd = socket(_saSend->sa_family, SOCK_DGRAM, 0);

	
	_saRecv = (struct sockaddr *)malloc(_saSendLen);
	memcpy(_saRecv, _saSend, _saSendLen);

	setsockopt(_recvfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

	if (type & UDP_SUBSCRIBER_TYPE){
		if (bind(_recvfd, /*sasend*/ _saRecv, _saSendLen) <0){
			Logger::getLogger()->log(Logger::TYPE_ERROR, "UDPSession::initUDP bind failed. ");	
			//exit(2);

		}

		mcast_join(_recvfd, _saSend, _saSendLen, NULL, 0);
		mcast_set_loop(_sendfd, /*0*/ 1);
	
		newIO(_recvfd, IOCallback::IO_READ, this);
	}


	////////////////////////////////
	return ret;
}


void
UDPSession::onRead(int fid, InputId id)
{
	if (fid == _recvfd){

		char line[512 +1];
		char portstr[7];
		char str[128];
		int n;
	
		struct sockaddr *safrom = new struct sockaddr;
		socklen_t len = sizeof(struct sockaddr);
	
		n = recvfrom(fid, line, 512, 0, safrom, &len);
		line[n] = 0;
	
		struct sockaddr_in *sin = (struct sockaddr_in *) safrom;
		if (inet_ntop(AF_INET, &sin->sin_addr, str, sizeof(str)) != NULL){
			if (ntohs(sin->sin_port) != 0){
				snprintf(portstr, sizeof(portstr), ".%d", ntohs(sin->sin_port));
				strcat(str, portstr);
			}
		}	
		processUDP(str, line);
	}
}

void
UDPSession::processUDP(const string& str, const string& line)
{
	Logger::getLogger()->log(Logger::TYPE_INFO, "UDPSession::processUDP Multicast from %s: %s", str, line);

}

char *
UDPSession::sock_ntop(const struct sockaddr *sa, socklen_t salen)
{
	char portstr[7];
	static char str[128];
	switch (sa->sa_family){
		case AF_INET: {
			struct sockaddr_in *sin = (struct sockaddr_in *) sa;
			
			if (inet_ntop(AF_INET, &sin->sin_addr, str, sizeof(str)) == NULL)
				return (NULL);
			if (ntohs(sin->sin_port) != 0) {
				snprintf(portstr, sizeof(portstr), ".%d", ntohs(sin->sin_port));
				strcat(str, portstr);
			}
			return (str);
		}
	}
	return (NULL);
}

int
UDPSession::udp_client( const char *host, const char *serv, void **saptr, socklen_t *lenp)
{
	int sockfd, n;
	struct addrinfo hints, *res, *ressave;

	bzero(&hints, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	
	if ((n = getaddrinfo(host, serv, &hints, &res)) != 0){
		Logger::getLogger()->log(Logger::TYPE_ERROR,"udp_client error for %s, %s: %s",
			host, serv, gai_strerror(n));
	}

	ressave = res;

	do{
		sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if (sockfd >= 0)
			break; //success
	} while ( (res = res->ai_next) != NULL);

	if (res == NULL){
		Logger::getLogger()->log(Logger::TYPE_ERROR,"udp_client error for %s, %s", host, serv);
	}
	*saptr = malloc(res->ai_addrlen);
	memcpy(*saptr, res->ai_addr, res->ai_addrlen);
	*lenp = res->ai_addrlen;

	freeaddrinfo(ressave);

	return (sockfd);
}

int
UDPSession::mcast_join(int sockfd, const SA* sa, socklen_t salen,const char *ifname, u_int ifindex)
{
	switch (sa->sa_family) {
		case AF_INET: {
			struct ip_mreq mreq;
			struct ifreq ifreq;

			memcpy(&mreq.imr_multiaddr,
				&((struct sockaddr_in *) sa)->sin_addr,
				sizeof(struct in_addr));

			if (ifindex > 0){
				if (if_indextoname(ifindex, ifreq.ifr_name) ==NULL) {
					errno = ENXIO;
					return (-1);
				}
				goto doioctl;
			}else if (ifname != NULL) {
				strncpy(ifreq.ifr_name, ifname, IFNAMSIZ);
			   doioctl:
				if (ioctl(sockfd, SIOCGIFADDR, &ifreq) < 0)
					return (-1);
				memcpy(&mreq.imr_interface,
					&((struct sockaddr_in *) &ifreq.ifr_addr)->sin_addr,
					sizeof(struct in_addr));
			} else
				mreq.imr_interface.s_addr = htonl(INADDR_ANY);

			return (setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP,
					&mreq, sizeof(mreq)));
		}
	}
	return (-1);
}

int
UDPSession::mcast_set_loop(int sockfd, int onoff)
{
	switch (sockfd_to_family(sockfd)){
		case AF_INET:{
			u_char flag;
			flag = onoff;
			return (setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_LOOP,
					&flag, sizeof(flag)));
		}
	}
	return (-1);
}

int
UDPSession::sockfd_to_family(int sockfd)
{
	union {
		struct sockaddr sa;
		char data[MAXSOCKADDR];
	} un;

	socklen_t len;

	len = MAXSOCKADDR;
	if (getsockname(sockfd, (SA *)un.data, &len) < 0)
		return (-1);
	
	return (un.sa.sa_family);
}

UDPSession::UDPSessionPublisher::~UDPSessionPublisher()
{
	removeIO(_sendFd);
}

void
UDPSession::UDPSessionPublisher::publish(MCMsg *msg)
{
	//cout <<"UDPSessionPublisher::publish "<<_msglist.size()<<" "<<msg->toString()<<endl;
	_msglist.push_back(msg);
	newIO(_sendFd, IOCallback::IO_WRITE, this);
}

void
UDPSession::UDPSessionPublisher::onWrite(int fd, InputId id)
{
	if (fd == _sendFd){
		vector<MCMsg*>::iterator  it = _msglist.begin();
		for (it; it != _msglist.end(); it++){
			MCMsg* msg = (*it);
			if (msg){
				sendto(_sendFd, msg->toString().c_str(), msg->length(), 0, _mySession->saSend(), _mySession->saSendLen());

				delete msg;
			}
		}
		_msglist.clear();
		this->removeIO(_sendFd);
	}
}

void
UDPSession::setMCIP(const string &ip)
{
	_mc_ip = ip;
}

void
UDPSession::setMCPort(const string &port)
{
	_mc_port = port;
}

