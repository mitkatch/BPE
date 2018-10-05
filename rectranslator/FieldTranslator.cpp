
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

#include "FieldTranslator.hpp"
#include "ConfigMap.hpp"
#include "FieldFormatter.hpp"

FieldTranslator* FieldTranslator::_translator = NULL;

FieldTranslator::FieldTranslator()
{
	buildTemplateMap();
}

FieldTranslator*
FieldTranslator::getTranslator()
{
	if (_translator == NULL)
		_translator = new FieldTranslator();

	return _translator;

}

vector<string>
FieldTranslator::translateField(const string& f)
{
	vector<string> idnF;
	TemplateMap::iterator  it = _templateMap.begin();
	for(it; it != _templateMap.end(); it++){
		if ( f == (*it).second.first){
			idnF.push_back((*it).first);
			//cout <<"translateField "<<f<<" to "<<(*it).first<<endl;
		}
	} 
	return idnF;

}

vector<DataGraph::Variant>
FieldTranslator::translateVariant(const vector<string>& f, const DataGraph::Variant& v)
{
	string format;
	vector<DataGraph::Variant> datav;
	for (int i = 0; i < f.size(); i++){
		TemplateMap::iterator  it = _templateMap.find(f[i]);
		if (it != _templateMap.end()){
			format = (*it).second.second;
			datav.push_back(FieldFormatter::getFormatter(format)->formatVariant(v));
		}else{
			datav.push_back(FieldFormatter::getFormatter("")->formatVariant(v));
			Logger::getLogger()->log(Logger::TYPE_ERROR,"FieldTranslator::translateVariant not found format for %s. Default is used. ", (f[i]).c_str());
		} 
	}
	return datav;
}

void
FieldTranslator::buildTemplateMap()
{
	//TEMPLATE.TRDTIM_1=ROW66_11[FORMAT:TIME5]	
	ConfigMap* configMap = ConfigMap::getConfig();
	string t = "TEMPLATE.";
	string frm = "[FORMAT:";
	map<string, string>::const_iterator it = configMap->begin();
	for(it ; it != configMap->end(); it++){
		int t_ind = (*it).first.find(t);
		if (t_ind != 0)
			continue;	

		int i = (*it).first.length();
		string idn_f = (*it).first.substr(t.length(), i - t.length());
		string rec_f = configMap->getValue((t + idn_f).c_str());
		string format;
		int frm_ind = rec_f.find(frm);
		if (frm_ind != string::npos){
			int lst = rec_f.find_last_of("]");
			if (lst != string::npos){
				format = rec_f.substr(frm_ind + frm.length() , rec_f.length() - (frm_ind + frm.length()) - 1);
			} 
			rec_f = rec_f.substr(0, frm_ind);
		}	
		Logger::getLogger()->log(Logger::TYPE_INFO,"FieldTranslator::buildTemplateMap Field found %s as %s, format %s", rec_f.c_str(), idn_f.c_str(), format.c_str());
		FormatedField ff = std::make_pair(rec_f, format);
		if ((_templateMap.insert(TemplateMap::value_type(idn_f, ff))).second == false){
			Logger::getLogger()->log(Logger::TYPE_ERROR,"FieldTranslator::buildTemplateMap failed for %s as %s, format %s", rec_f.c_str(), idn_f.c_str(), format.c_str());

		}	
	}
}

