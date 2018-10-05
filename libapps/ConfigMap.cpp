/************************************************************************
||
|| DATE:                $Date: 2005/03/29 16:05:55 $
|| SOURCE:              $Source: /repository/projects/Canpx/libapps/ConfigMap.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: ConfigMap.cpp,v 1.5 2005/03/29 16:05:55 mikhailt Exp $
|| REVISION:    $Revision: 1.5 $
|| LOG:                 $Log: ConfigMap.cpp,v $
|| LOG:                 Revision 1.5  2005/03/29 16:05:55  mikhailt
|| LOG:                 static ConfigMap
|| LOG:
|| LOG:                 Revision 1.4  2004/09/09 19:40:12  mikhailt
|| LOG:                 512-> 1024
|| LOG:
|| LOG:                 Revision 1.3  2004/07/30 16:14:57  mikhailt
|| LOG:                 sends correct heartbeats
|| LOG:
|| LOG:                 Revision 1.2  2004/04/13 19:58:41  mikhailt
|| LOG:                 reads from the http
|| LOG:
|| LOG:                 Revision 1.1  2004/01/08 20:51:46  tomcat
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <ctype.h>
#include <strings.h>

#include <unistd.h>
#include<iostream>
#include<fstream>

#include "Application.hpp"
#include "ConfigMap.hpp"
#include "Logger.hpp"

static string EMPTY(""); 
ConfigMap * ConfigMap::_config = NULL;

ConfigMap::ConfigMap( const Application *app):_myApp(app)
{
	//register Loader
	_urlLoaders.insert(map<string, UrlLoader>::value_type(string("file"),
		ConfigMap::loadFileUrl));
	_urlLoaders.insert(map<string, UrlLoader>::value_type(string("http"),
		ConfigMap::loadHttpUrl));
}

ConfigMap::~ConfigMap()
{

}

void
ConfigMap::loadCommandLine(int argc, char ** argv)
{
	assert (_myApp != NULL);
	opterr = 0; // don't want getopt() writing to stderr
	string myoptstr = _myApp->getCommOpt();
	int c;
	while ( (c = getopt(argc, argv, myoptstr.c_str())) != EOF){
		Logger::getLogger()->log(Logger::TYPE_INFO, " Command line options -%c %s optopt=%c)", char(c), (optarg!=NULL?optarg:"<none>"), char(optopt)); 
		if (c == '?'){
			Logger::getLogger()->log(Logger::TYPE_ERROR,"Unknown options -%c. Exit", char(c));
			exit(1);
		}
		else
			_configs.insert(map<string, string>::value_type(string(1, char(c)), string(optarg==NULL?string(1, char(c)):optarg)));
	}	
	
}

const string&
ConfigMap::getValue(const char* key)const
{
	map<string, string>::const_iterator it = _configs.find(key);
	if (it == _configs.end())
		return EMPTY;
	else
		return (*it).second;
}

bool
ConfigMap::insertPair(string key, string val)
{
	return _configs.insert(map<string, string>::value_type(key, val)).second;

}

bool
ConfigMap::loadUrl(string& url_type, string& url)
{
	bool ret = false;
	map<string, UrlLoader>::const_iterator it = _urlLoaders.find(url_type);
	if (it != _urlLoaders.end()){
		ret = (*it).second(url, this);
	}
	return ret;
}

//static
bool
ConfigMap::loadFileUrl(string filename, ConfigMap *confmap)
{
	Logger::getLogger()->log(Logger::TYPE_INFO,"Try to load config file %s", filename.c_str());
	ifstream is;
	is.open(filename.c_str(), ifstream::in);
	if (is.bad()){
		Logger::getLogger()->log(Logger::TYPE_ERROR,"Config file %s not found. Exit", filename.c_str());
		exit(2);
	}
	if (confmap)
		confmap->parseToConfig(is);

	is.close();
		
	return true;
}

//static
bool
ConfigMap::loadHttpUrl(string httpurl, ConfigMap *confmap)
{
	// 127.0.0.1:8080/config/canpxcorp/file.txt
	Logger::getLogger()->log(Logger::TYPE_INFO,"Getting config from %s", httpurl.c_str());
	int iPort = 80;
	unsigned long inetAddress;
	int surl = httpurl.find_first_of("/");
	if (surl == string::npos){
		Logger::getLogger()->log(Logger::TYPE_ERROR,"Currupted URL %s", httpurl.c_str());
		return false;
	}
	string getstr = httpurl.substr(surl, httpurl.length() - surl);

	struct  sockaddr_in serverAddress;
	struct sockaddr* serverAddressPtr = (struct sockaddr*)&serverAddress;
	int servLen = sizeof(serverAddress);

	int s_portS = httpurl.find_first_of(":");
	if (s_portS != string::npos){

		string sport = httpurl.substr(s_portS+1, surl - s_portS-1);
		iPort = atoi(sport.c_str());
	}
	int eAddress = (s_portS == string::npos? surl:s_portS); 
	string sAddress = httpurl.substr(0, eAddress); 

	if (isdigit(sAddress[0])){
		inetAddress = inet_addr(sAddress.c_str());
	} else {
		struct hostent* hostStruct = gethostbyname(sAddress.c_str());
		if (hostStruct == NULL){
			Logger::getLogger()->log(Logger::TYPE_ERROR,"Host not found %s", httpurl.c_str());
			return false;
		}
		struct in_addr* hostNode = (struct in_addr*) hostStruct->h_addr;
		inetAddress = hostNode->s_addr;
	}
		
	bzero((char *)&serverAddress, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inetAddress; //IP
	serverAddress.sin_port = htons(iPort);

	int clientFd = socket(AF_INET, SOCK_STREAM, 0 /*DEFAULT_PROTOCOL*/);

	int result = connect(clientFd, serverAddressPtr, servLen);
	if (result == -1){
		Logger::getLogger()->log(Logger::TYPE_ERROR,"Connection Refused from %s", httpurl.c_str());
		close(clientFd);
		return false;
	}

	bool ret = false;
	if (confmap) 
		ret = confmap->doGetToConfig(clientFd, getstr);

	close(clientFd);
	return ret;

}

bool
ConfigMap::doGetToConfig(int fd, string& getstr)
{
	char ch;
	string config;
	string get("GET ");
	get.append(getstr);
	get.append(" HTTP/1.0\r\n\r\n\r\n");
	int i = write(fd, get.c_str(), get.length());
	ifstream is(fd);
	if (is.bad()){
		Logger::getLogger()->log(Logger::TYPE_ERROR,"URL %s not found. Exit", getstr.c_str());
		exit(2);
	}
	parseToConfig(is); 

	is.close();
	cout<<config<<endl;
	return true;	
}

void
ConfigMap::parseToConfig(ifstream& is)
{
	char line[1024];
	while (is.good()){
		is.getline(line, 1024);
		if (line[0] != '#'){
			string strline(line);
			unsigned int eq = strline.find_first_of('=');
			if (eq != string::npos){
				string key = strline.substr(0, eq);
				string val = strline.substr(eq+1, string::npos); 
				Logger::getLogger()->log(Logger::TYPE_INFO,"key=%s, value=%s", key.c_str(), val.c_str());
				insertPair(key, val);		
			}
		}
	}
}

//static
ConfigMap*
ConfigMap::getConfig()
{
	assert(_config != NULL);
	return _config;

}

//static
void
ConfigMap::setConfig(ConfigMap *conf)
{
	if (!_config){
		_config = conf;

	}else{
		Logger::getLogger()->log(Logger::TYPE_ERROR,"setConfig duplication Error");
	}

}

