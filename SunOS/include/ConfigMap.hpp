
/************************************************************************
||
|| DATE:                $Date: 2005/03/29 16:05:55 $
|| SOURCE:              $Source: /repository/projects/Canpx/libapps/ConfigMap.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: ConfigMap.hpp,v 1.5 2005/03/29 16:05:55 mikhailt Exp $
|| REVISION:    $Revision: 1.5 $
|| LOG:                 $Log: ConfigMap.hpp,v $
|| LOG:                 Revision 1.5  2005/03/29 16:05:55  mikhailt
|| LOG:                 static ConfigMap
|| LOG:
|| LOG:                 Revision 1.4  2004/04/29 19:52:56  mikhailt
|| LOG:                 migration to SUNPRO
|| LOG:
|| LOG:                 Revision 1.3  2004/04/28 16:23:53  mikhailt
|| LOG:                 no changes, some include
|| LOG:
|| LOG:                 Revision 1.2  2004/04/13 19:58:41  mikhailt
|| LOG:                 reads from the http
|| LOG:
|| LOG:                 Revision 1.1  2004/01/08 20:51:46  tomcat
|| LOG:                 Initial revision
|| LOG:
************************************************************************/


#ifndef CONFIGMAP_HPP
#define CONFIGMAP_HPP

#include <iostream.h>
#include <string>
#include <map>
#include <fstream>

using std::string;
using std::map;
using std::ifstream;

class Application;

class ConfigMap
{
	public:
		typedef bool (*UrlLoader)(string url, ConfigMap *conf);
	public:
		ConfigMap( const Application *app);
		virtual ~ConfigMap();
		virtual const string&  getValue(const char * key)const;
		virtual bool insertPair(string key, string val);
		virtual void loadCommandLine(int argc, char ** argv);
		virtual bool loadUrl(string& url_type, string& url);
		virtual map<string, string>::const_iterator begin() const{
			return _configs.begin();
		}

		virtual map<string, string>::const_iterator end() const{
			return _configs.end();
		}
		static ConfigMap* getConfig();
		static void setConfig(ConfigMap* cm);
	protected:
		static bool loadFileUrl(string filename, ConfigMap *);
		static bool loadHttpUrl(string url, ConfigMap *);

		virtual bool doGetToConfig(int fd, string& getstr);
		virtual void parseToConfig(ifstream& is);
		static ConfigMap* _config;
	private:
		map<string, string> _configs;
		map<string, UrlLoader> _urlLoaders;
		const Application * _myApp;

};
#endif//CONFIGMAP_HPP
