
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


#include "ConfigMap.hpp"
#include "RecordConverter.hpp"
#include "FieldTranslator.hpp"
#include "DataCache.hpp"
#include "RTApp.hpp"

RecordConverter::RecordConverter(const string& recName, const string& idnName):
                                _recName(recName),
				_idnName(idnName),
				_activated(false)
{

}

RecordConverter::~RecordConverter()
{
	Datum *idnDatum = DataCache::getCache()->getDatum(_idnName);
	DataGraph *idnData = dynamic_cast<DataGraph *>(idnDatum);
	if (idnData)
		idnData->unregisterSubscriber(theRTApp->getProducer());
	
}

//static
string
RecordConverter::getIdnName(const string& id)
{
	//item=135087XZ2,producer=canpx2goc,ou=Translator,o=moneyline.ca
	string idnid;
	string pre = ConfigMap::getConfig()->getValue("IDN_RIC_PREFIX");
	string suf = ConfigMap::getConfig()->getValue("IDN_RIC_SUFFIX");	
	string source = ConfigMap::getConfig()->getValue("IDN_RIC_SUBSCRIBER");	
	int ic = id.find("item=");
	int icom = id.find(",");
	if (ic != string::npos && icom != string::npos && icom > (ic + 5)){
		string cusip = id.substr(ic + 5, icom - (ic + 5));
		idnid = source + string(".") + pre + cusip + string("=") + suf;	
		Logger::getLogger()->log(Logger::TYPE_INFO, "RecordConverter::getIdnName %s for %s", idnid.c_str(), id.c_str());

	}else{
		Logger::getLogger()->log(Logger::TYPE_ERROR, "RecordConverter::getIdnName failed for %s", id.c_str());
	}

	return idnid;
}

void
RecordConverter::notify(Datum *datum)
{
	Logger::getLogger()->log(Logger::TYPE_INFO, "RecordConverter::notify %s as %s", _recName.c_str(), _idnName.c_str());

	if (!_activated){
		_activated = activateIdnRecord(datum);	

	} else {
		updateIdnRecord(datum);
	}
}

bool
RecordConverter::activateIdnRecord(Datum *datum)
{
	bool done = false;
	if (datum)
		done = DataCache::getCache()->addDatum(_idnName, Datum::GRAPH);	
	done = updateIdnRecord(datum);
	if (done){
		Datum* 	idnDatum = DataCache::getCache()->getDatum(_idnName);
		DataGraph *idnData = dynamic_cast<DataGraph *>(idnDatum);
		if (idnData){
			idnData->registerSubscriber(theRTApp->getProducer());
			idnData->registerSubscriber(theRTApp->getSubMgr());
		}
	}
	
	return done;
}

bool
RecordConverter::updateIdnRecord(Datum *datum)
{
	bool updated = false;
	Datum *idnDatum = NULL;
	if (datum){
		idnDatum = DataCache::getCache()->getDatum(_idnName);
	}

	DataGraph *data = dynamic_cast<DataGraph *>(datum);
	DataGraph *idnData = dynamic_cast<DataGraph *>(idnDatum);
	set<string> fields = data->getFieldSet();
	set<string>::iterator it = fields.begin();
	for (it; it != fields.end(); it++){
		string field = (*it);
		vector<string> idnField = FieldTranslator::getTranslator()->translateField(field);
		if (idnField.size() == 0)
			continue;
		DataGraph::Variant var = data->getVariant(field);
		vector<DataGraph::Variant> idnVar = FieldTranslator::getTranslator()->translateVariant(idnField, var);
		for(int i = 0; i < idnField.size(); i++){
			idnData->updateField(idnField[i], idnVar[i]);
			cout <<" IDN Updated "<< idnData->address()<< "["<<idnField[i]<<"]"<<endl;
			updated = true;
		}
	}
	if (updated) idnData->updated();
	
	return updated;
}

