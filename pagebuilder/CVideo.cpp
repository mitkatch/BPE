/************************************************************************
||
|| DATE:                $Date: 2005/06/29 19:00:48 $
|| SOURCE:              $Source: /repository/projects/Canpx/pagebuilder/CVideo.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: CVideo.cpp,v 1.1 2005/06/29 19:00:48 mikhailt Exp $
|| REVISION:    $Revision: 1.1 $
|| LOG:                 $Log: CVideo.cpp,v $
|| LOG:                 Revision 1.1  2005/06/29 19:00:48  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#include "DataPage.hpp"
#include "DataGraph.hpp"
#include "CellConverter.hpp"
#include "CVideo.hpp"
#include "Scheduler.hpp"

CVideoFactory* CVideoFactory::_factory = NULL;
static map<int, string> attrMap;

CVideoFactory::CVideoFactory()
{
	attrMap[DataPage::V_NONE] = V_NONE_STR;
	attrMap[DataPage::V_HIGHLIGHT] = V_HIGHLIGHT_STR;
	attrMap[DataPage::V_BLINK] = V_BLINK_STR;
	attrMap[DataPage::V_UNDERLINE] = V_UNDERLINE_STR;
}

CVideoFactory::~CVideoFactory()
{

	map<string, CVideo *>::iterator it = _videoMap.begin();
	for (it ; it != _videoMap.end(); it++){
		delete (*it).second;
	}
	_videoMap.clear();

}

CVideo*
CVideoFactory::getVideo(const string& f)
{
	if (_factory == NULL){
		_factory = new CVideoFactory();
	}

	return _factory->getThisVideo(f);
}

CVideo*
CVideoFactory::getThisVideo(const string& f)
{
	CVideo* v = NULL;
	map<string, CVideo *>::iterator it = _videoMap.find(f);
	if (it != _videoMap.end()){
		v = (*it).second;
	} else {
		v = new CVideo(f);	
		_videoMap[f] = v;
	}
	return v;
}

CVideo::CVideo(const string& name):_name(name)
{
	string videoUrl ="video://";
	int url = name.find(videoUrl, 0);
	if (url == 0){
		string attr = name.substr(videoUrl.size(), name.size() - videoUrl.size());
		buildRules(attr);
			
	}else{
		_value = DataPage::V_NONE;
	}	

}

int
CVideo::video(DataGraph *data, CellConverter* cell)
{
	_value = DataPage::V_NONE;
	map<int, RulesMap>::iterator it = _rules.begin();
	for( it; it != _rules.end(); it++){
		RulesMap& rm = (*it).second;
		if (rm.size() == 0){
			_value |= (*it).first; //non-conditinal
		}else{
			RulesMap::iterator it2 = rm.begin();
			for (it2; it2 != rm.end(); it2++){
				string dataVal = data->getValue((*it2).first);
				//if (dataVal == (*it2).second){
				// dataVal can = "HT" when two side trades
				if (dataVal.find((*it2).second) != string::npos){
					//_value |= (*it).first; // met condition
					_value = (*it).first; // only eq
				}
			}
		}
	}
	return _value;
}

void
CVideo::buildRules(const string& attr)
{
	//attr is like "NONE|BLINK@TRADE_TONE=H"
	// we need to build map<int, map<string, string> > _rules;
	vector<string> w;
	Formatter::split(attr, "|", w);
	
	for (int i = 0; i < w.size(); i++){
		map<int, string>::iterator it = attrMap.begin();
		for (it; it != attrMap.end(); it++){
			int found = w[i].find((*it).second, 0);
			int foundAt = w[i].find_first_of("@", 0);
			if (found == 0 && foundAt == string::npos){
				//only ATTR, no rules
				if ((*it).second == w[i]){
					RulesMap rulesMap;
					_rules[(*it).first] = rulesMap;
				}
			} if (found == 0 && foundAt != string::npos){
				//BLINK@TRADE_TONE=H
				string vattr = w[i].substr(0, foundAt);
				int eq = w[i].find_first_of("=", foundAt);	
				if (eq != string::npos){
					string key = w[i].substr(foundAt +1, eq - (foundAt +1)); 	
					string keyVal = w[i].substr(eq +1, w[i].size() - (eq + 1)); 
					RulesMap rulesMap;
					rulesMap[key] = keyVal;
					_rules[(*it).first] = rulesMap;
				}
			}
		}
	}
}
