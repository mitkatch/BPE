
/************************************************************************
||
|| DATE:                $Date: 2006/06/19 21:33:00 $
|| SOURCE:              $Source: /repository/projects/Canpx/pagebuilder/RowConverter.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: RowConverter.cpp,v 1.5 2006/06/19 21:33:00 mikhail Exp $
|| REVISION:    $Revision: 1.5 $
|| LOG:                 $Log: RowConverter.cpp,v $
|| LOG:                 Revision 1.5  2006/06/19 21:33:00  mikhail
|| LOG:                 cannot read the set of fileds from the parser
|| LOG:
|| LOG:                 Revision 1.4  2006/04/06 17:57:31  nickz
|| LOG:                 port to RMDS
|| LOG:
|| LOG:                 Revision 1.3  2006/04/04 13:19:34  nickz
|| LOG:                 ported to rmds
|| LOG:
|| LOG:                 Revision 1.2  2005/09/21 17:49:55  mikhailt
|| LOG:                 activated only when loaded()
|| LOG:
|| LOG:                 Revision 1.1  2005/06/29 19:00:48  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#include "PageConverter.hpp"
#include "RowConverter.hpp"
#include "Logger.hpp"
#include "DataGraph.hpp"
#include "DataCache.hpp"
#include "DataPage.hpp"
#include "ConfigMap.hpp"
#include "Formatter.hpp"

RowConverter::RowConverter( const string& source, PageConverter* pc, const string& row):_source(source),_myPage(pc), _row(row), _rowNum(-1)
{
	vector<string> tmpV;
	string row_str = "rowNum=";
	Formatter::split(row, ",", tmpV);
	for (int j = 0; j < tmpV.size(); j++){
		int row_id = tmpV[j].find(row_str);
		if (row_id != string::npos){
			int eq1 = tmpV[j].find_first_of('=', row_id +1);
			if (eq1 != string::npos){
				string s_rowNum = tmpV[j].substr(row_id + row_str.size(), eq1 - (row_id + row_str.size()));
				_rowNum = Formatter::ATOF(s_rowNum.c_str());// +1;//config is 0 based vs. published Pages 1 based
			} 
		}

	}
	_active = source.empty();
}

RowConverter::~RowConverter()
{
	map<string, CellConverter *>::iterator it = _cells.begin();
	for (it; it != _cells.end(); it++){
		CellConverter *cc = (*it).second;
		delete cc;

	}
}

void 
RowConverter::notify(Datum *datum)
{
	//if first time -> load cellConfig
	if (!_active){
		_myPage->loadLdap("/" + _row, ConfigMap::getConfig()->getValue("LDAP_SEARCH_COLUMN"), PageConverter::loadRowConfig);
		_active = true;
	}

	DataGraph *data = dynamic_cast<DataGraph *>(datum);
	if (data){
		DataGraph::DataMap delta= /*data->getImage()*/ data->delta(this);
		//cout << "RowConverter::notify " << data->address()<< endl;
		DataGraph::DataMap::iterator it = delta.begin();
		for (it; it != delta.end(); it++){
			string val = DataGraph::serializeVariantValue((*it).second);
			MCells::iterator itc = _fieldsMap.find((*it).first);
			if (itc != _fieldsMap.end()){ 
				//cout <<"Updated ["<<(*it).first<<"] = ["<<val<<"]"<<endl;
				VCells& cells = (*itc).second;
				for (int i = 0; i < cells.size(); i++){
					cells[i]->update(data);//pass original DataGraph

				}
			}
		}
		//cout << "Updated "<<output<<endl; 

	}
	/**
	set<string> adr = DataCache::getCache()->getAddressSet();
	set<string>::iterator it = adr.begin();
	for (it; it != adr.end(); it++){
		//cout << "Address found ["<<(*it)<<"]"<<endl;
		Datum * d = DataCache::getCache()->getDatum((*it));
		DataGraph *dg = dynamic_cast<DataGraph *>(d);
		//cout << "ML_GRAPH_ADDRESS  "<< dg->getValue("ML_GRAPH_ADDRESS")<<endl;

	}
	**/
}

void
RowConverter::addCell(CellConverter *cc)
{
	if ((cc->address()).size()){

		if(_cells.insert(map<string, CellConverter *>::value_type(cc->address(), cc)).second){
			if (cc->dynamic()){
				string out = "";
				vector<string> datumField = cc->datumField();
				for (int i = 0; i < datumField.size(); i++){
					//cout<<"RowConverter::addCell subscribe "<<_source<<" field "<<datumField[i]<<endl;
					this->setSubscribedFields(_source, datumField[i], out);
					MCells::iterator mit = _fieldsMap.find(datumField[i]);
					if (mit != _fieldsMap.end()){
						VCells& cells = (*mit).second;
						cells.push_back(cc);
					} else {
						VCells cells;
						cells.push_back(cc);
						_fieldsMap[datumField[i]] = cells;

					}
				}
			} else {
				cc->update(cc->value());
				//DataPage *dp = _myPage->getDataPage();
				//if (dp)
				//	dp->update(cc->row(), cc->col(), cc->value());
			}

		}
	}


}

