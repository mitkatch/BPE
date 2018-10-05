
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

#include "MKLFeedApp.hpp"
#include "Logger.hpp"
#include "MKLRecordSubscriber.hpp"
#include "MKLFeedSession.hpp"
#include "ConfigMap.hpp"

MKLRecordSubscriber::MKLRecordSubscriber(FeedSession* s, const string& address):
				 _mySession(s), 
				_address(address), 
				_activated(false)
{
	setFields();
	int i = address.find_first_of(".");
	if (i != string::npos){
		_idn_address = address.substr(i +1, address.length() - (i + 1));
	}
	_tmstmpField = ConfigMap::getConfig()->getValue("SUBJECT_RECORD_TIMESTAMP");
	if (_tmstmpField.empty())
		_tmstmpField = CPX_REC_TIMESTAMP;
	
}

MKLRecordSubscriber::~MKLRecordSubscriber()
{

}
void
MKLRecordSubscriber::notify( Datum *datum)
{
	DataGraph *data = dynamic_cast<DataGraph *>(datum);
	if (data){
		string address = data->address();
		if (address == _address){
			if (!_activated){
				_activated = theMKLFeedApp->getSubMgr()->imageProcessed(_address);
			}

			string tmstmp = data->getValue(_tmstmpField);
			Logger::getLogger()->log(Logger::TYPE_INFO,"MKLRecordSubscriber::notify %s @ [%s]:%s ", address.c_str(), _tmstmpField.c_str(), tmstmp.c_str());
			theMKLFeedApp->setTimestamp(address, tmstmp);
			this->onData(data);
		}
	}
}

void
MKLRecordSubscriber::onData(DataGraph *rec)
{
	if (_mySession == NULL) return;
	DataGraph::DataMap data = rec->delta(this);	

	/****
	set<string> s = rec->getFieldSet();
	set<string>::iterator cc = s.begin();
	for (cc; cc != s.end(); cc++){
		cout << "Field ["<<(*cc)<<"]"<<endl;

	}
	****/

	buildDifData(data);
	DataGraph::DataMap::iterator it = _diffData.begin();
	Logger::getLogger()->log(Logger::TYPE_INFO,"MKLRecordSubscriber::onData processing %s {", _address.c_str());
	for (it; it != _diffData.end(); it++){
		Logger::getLogger()->log(Logger::TYPE_INFO," [%s]=%s",((*it).first).c_str(), (DataGraph::serializeVariantValue((*it).second)).c_str() );

	}
	Logger::getLogger()->log(Logger::TYPE_INFO, " } MKLRecordSubscriber::onData processing %s ", _address.c_str());

	MKLFeedSession *feed = dynamic_cast<MKLFeedSession *>(_mySession);
	if (feed){
		feed->publishInsertRequest(_diffData, _idn_address);
	}
	_diffData.clear();
}

void
MKLRecordSubscriber::buildDifData(DataGraph::DataMap& data)
{
	//save img in cache, diff add to _difData
	DataGraph::DataMap::iterator it = data.begin();
	for (it; it != data.end(); it++){
		DataGraph::DataMap::iterator itc = _cacheData.find((*it).first);
		if (itc != _cacheData.end()){
			//DataGraph::Variant new = ((*it).second);
			//DataGraph::Variant old = ((*itc).second);
			//if ( !(new == old)){
			if (! ((*it).second == (*itc).second) ){
				//(*itc).second = new;
				(*itc).second = (*it).second;
				_diffData.insert(DataGraph::DataMap::value_type((*itc).first, (*itc).second));
			}

		} else {
			if ((_cacheData.insert(DataGraph::DataMap::value_type((*it).first,
						(*it).second))).second){
				_diffData.insert(DataGraph::DataMap::value_type((*it).first, (*it).second));

			}else{
				Logger::getLogger()->log(Logger::TYPE_ERROR,"MKLRecordSubscriber::buildDifData failed for [%s]", (*it).first.c_str());

			} 

		}
	}	
}

void
MKLRecordSubscriber::setFields()
{
	string rec_tmpl = ConfigMap::getConfig()->getValue("SUBJECT_RECORD_TEMPLATE");
	if (rec_tmpl.empty()){
			Logger::getLogger()->log(Logger::TYPE_ERROR,"MKLRecordSubscriber::setFields failed. No config for SUBJECT_RECORD_TEMPLATE found, default is used");

		this->setSubscribedFields(_address, "DSPLY_NAME", "3");
		return;
	}
	
	map<string, string>::const_iterator it = ConfigMap::getConfig()->begin();
	for (it; it != ConfigMap::getConfig()->end(); it++){
		int ind = ((*it).first).find(rec_tmpl);
		if (ind == 0){
			string ff = ((*it).first).substr(rec_tmpl.size() + 1, ((*it).first).size() - (rec_tmpl.size() + 1));
			string id = (*it).second; 
			this->setSubscribedFields(_address, ff, id);
			cout << "setFields [" << ff <<"]:["<<id<<"]"<<endl;
		}


	}
	//this->setSubscribedFields(address, in , out);
}
