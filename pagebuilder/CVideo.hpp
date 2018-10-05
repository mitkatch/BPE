/************************************************************************
||
|| DATE:                $Date: 2005/06/29 19:00:48 $
|| SOURCE:              $Source: /repository/projects/Canpx/pagebuilder/CVideo.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: CVideo.hpp,v 1.1 2005/06/29 19:00:48 mikhailt Exp $
|| REVISION:    $Revision: 1.1 $
|| LOG:                 $Log: CVideo.hpp,v $
|| LOG:                 Revision 1.1  2005/06/29 19:00:48  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#ifndef CVIDEO_HPP
#define CVIDEO_HPP

#include <map>
#include <vector>
#include <string>
#include "Logger.hpp"

class CellConverter;
class DataGraph;

using std::map;
using std::vector;
using std::string;

#define V_NONE_STR		"NONE"
#define V_BLINK_STR		"BLINK"
#define V_HIGHLIGHT_STR		"HIGHLIGHT"
#define V_UNDERLINE_STR		"UNDERLINE"


class CVideo
{
	public:
		typedef map<string, string> RulesMap;
	public:
		CVideo(const string& name);
		virtual ~CVideo(){};
		virtual int video(DataGraph *data, CellConverter* cell);

		virtual int value(){
			return _value;
		}

		virtual void value(int val){
			_value = val;
		}

	protected:
		void buildRules(const string& attr);
		string _name;
		int _value;
		map<int, RulesMap> _rules;
};

class CVideoFactory
{
	public:
		virtual ~CVideoFactory();

		static CVideo* getVideo(const string& f);
	protected:
		static CVideoFactory* _factory;
		CVideo* getThisVideo(const string& f);
		CVideoFactory();
		map<string, CVideo* > _videoMap;
};

#endif //CVIDEO_HPP

