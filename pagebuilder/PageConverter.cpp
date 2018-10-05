/************************************************************************
||
|| DATE:                $Date: 2006/05/02 15:12:09 $
|| SOURCE:              $Source: /repository/projects/Canpx/pagebuilder/PageConverter.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: PageConverter.cpp,v 1.7 2006/05/02 15:12:09 nickz Exp $
|| REVISION:    $Revision: 1.7 $
|| LOG:                 $Log: PageConverter.cpp,v $
|| LOG:                 Revision 1.7  2006/05/02 15:12:09  nickz
|| LOG:                 .
|| LOG:
|| LOG:                 Revision 1.6  2006/04/06 17:57:30  nickz
|| LOG:                 port to RMDS
|| LOG:
|| LOG:                 Revision 1.5  2006/04/04 14:37:12  nickz
|| LOG:                 building...
|| LOG:
|| LOG:                 Revision 1.4  2006/04/04 13:19:33  nickz
|| LOG:                 ported to rmds
|| LOG:
|| LOG:                 Revision 1.3  2005/09/28 19:23:40  mikhailt
|| LOG:                 re-use duplicated LDAP messages
|| LOG:
|| LOG:                 Revision 1.2  2005/09/21 17:49:55  mikhailt
|| LOG:                 activated only when loaded()
|| LOG:
|| LOG:                 Revision 1.1  2005/06/29 19:00:48  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#include "PageConverter.hpp"
#include "PageArrayConverter.hpp"
#include "DataCache.hpp"
#include "ConfigMap.hpp"
#include "DataCache.hpp"
#include "Formatter.hpp"
#include "CellConverter.hpp"


rmdsPageProducer* PageConverter::s_rmdsPageProducer = NULL;


PageConverter::PageConverter(PageArrayConverter *array, const string& ldapurl, const string& pname) :
			_prod(NULL), _pname(pname), _chainMgr(NULL),
			_dynamic(false)
{
	//"/ou=3101, ou=PageBuilder, o=moneyline.ca";
	_connstr = string("/") + ldapurl; //array->connstr(); 
	
	_host = array->host();
	_port = array->port();

	_service = s_rmdsPageProducer->service();
}

PageConverter::~PageConverter()
{
	map<string, RowConverter *>::iterator it = _grid.begin();
	for (it; it != _grid.end(); it++){
		RowConverter *gc = (RowConverter *)(*it).second;
		delete gc;
	}
	_grid.clear();
	_dynamicRows.clear();

	map<string, MsgGraph *>::iterator itm = _msgGraphPatternMap.begin();
	for (itm; itm != _msgGraphPatternMap.end(); itm++){
		MsgGraph * msg = (*itm).second;
		delete msg;
	}
	_msgGraphPatternMap.clear();
}

void
PageConverter::load(const string& url)
{
    //string address = getPageName(_connstr);
    string address = _service + "." + _pname;
    if (!address.empty()){
	if (DataCache::getCache()->addDatum(address, Datum::PAGE))
	{
	    DataPage *page = (DataPage *)DataCache::getCache()->getDatum(address);
	    if (page)
	    {
		setDataPage(page);
		_placeHolder = string(page->width(), ' ');
	    }
	}
	loadLdap(_connstr, url, PageConverter::loadPageConfig);
    }

    addTimer(10*1000, 1, this);
}



void
PageConverter::loadLdap(const string& connstr, const string& url, Loader loader)
{
	string index = connstr + "." + url;
	map<string, MsgGraph *>::iterator it = _msgGraphPatternMap.find(index);
	if (it != _msgGraphPatternMap.end() ){
		//cout << "PageConverter::loadLdap stored "<<index<<endl;
		MsgGraph *rset = (*it).second;
		(*loader)(rset, this);
	} else {
		DBConnectorLdap *_conn = new DBConnectorLdap(_host, _port);
		if (_conn->connect(connstr)){
			if (_conn->fetch(url)){
				MsgGraph * rset = _conn->search();
				if (rset){
					(*loader)(rset, this);
				}
				//cout << "PageConverter::loadLdap save "<<index<<endl;
				MsgGraph *new_rset = new MsgGraph(*rset);
				_msgGraphPatternMap[index] = new_rset;

			}
		}
		_conn->disconnect();
		delete _conn;
	}
}

bool
PageConverter::subscribeSourceChain(int counter, const string& rowurl)
{
	//rowurl  "rowNum=2, page=3102D, ou=PageBuilder, o=moneyline.ca"
	//cout <<" subscribeSourceChain "<<counter<<" from "<<rowurl<<endl;
	bool ret = false;
	char ch[15];
	string newurl;
	sprintf(ch, "rowNum=%d,", counter);	
	if (_chainMgr){
		string address = _chainMgr->getAddressNext();
		int coma1 = rowurl.find_first_of(',');
		if (coma1 != string::npos){
			newurl = rowurl.substr(coma1 +1, rowurl.size() - (coma1 +1));
			newurl = ch + newurl; //replace with dynamic rowNum
		}
		//cout << "subscribeSourceChain address "<<address<<" for "<<ch<<endl;
		if (address.size()){
			RowConverter *gc = new RowConverter(address, this, rowurl);
			gc->rowNum(counter);
			// address = canpx2tb.CANPX_TB.APR07
			DataCache::getCache()->subscribe(address, gc, Datum::GRAPH);
			_grid[newurl] = gc;
			_dynamicRows.push_back(gc);
			ret = true;
		}
	}
	return true;
}

bool
PageConverter::subscribeSource(const string& rec, const string& row)
{
    //rmds://PREBON.17670.APR07@prebonParser
    bool ret = false;
    string rmds = "rmds://"	;
    string chain = "chain://";

    int rmds_id = rec.find(rmds);
    int chain_id = rec.find(chain);
    if (rmds_id != string::npos)
    {
	string address = rec.substr(rmds.size());
	RowConverter *gc = new RowConverter(address, this, row);

	DataCache::getCache()->subscribe(address, gc, Datum::GRAPH);
	_grid[row] = gc;
	ret = true;
    }
    else if (chain_id != string::npos)
    {
	if (_chainMgr)
	{
	    string request = rec.substr(chain_id + chain.size(), rec.size() - (chain_id + chain.size()));
	    ret = _chainMgr->requestChain(request);	
	}		
	
    } 
    else 
    {   
	// no source provided looks like static row
	RowConverter *gc = new RowConverter(rec, this, row);
	_grid[row] = gc;
    }

    return ret;
}

//static
void
PageConverter::loadPageConfig(MsgGraph* rset, PageConverter* pg)
{
	int counter = 0;
	string rowurl;
	set<string> keys = rset->getKeys("");
	set<string>::iterator it = keys.begin();
	for (it; it != keys.end(); it++){
		//cout << "loadPageConfig Keys found "<<(*it)<<endl;
		if ((*it).size()){
			string rec, row;
			rowurl = (*it); 	
			rset->getAttributeValue("feedSource@" + rowurl, 0, rec);
			rset->getAttributeValue("rowNum@" + rowurl, 0, row);
			counter = Formatter::ATOF(row.c_str());

			if (pg->subscribeSource(rec, rowurl) == false){
				// not subscridable , static
				pg->loadLdap("/" +rowurl, ConfigMap::getConfig()->getValue("LDAP_SEARCH_COLUMN"), PageConverter::loadRowConfig);
			}
		}
	}
	//keep going for counter 'till end line
	while(pg->dynamic() && counter <= MTRS_PAGE_ROW_MAX_IND){
		pg->subscribeSourceChain(counter++, rowurl);
	}
}

//static
void
PageConverter::loadRowConfig(MsgGraph* rset, PageConverter* pg)
{
	set<string> keys1 = rset->getKeys("");
	set<string>::iterator it1 = keys1.begin();
	for (it1; it1 != keys1.end(); it1++){
		//cout << "loadRowConfig Keys found "<<(*it1)<<endl;
		// colNum=2,rowNum=2,page=3101,ou=PageBuilder,o=moneyline.ca
		string tmp = (*it1);
		int next_coma = tmp.find_first_of(",");
		if (next_coma != string::npos){
			string row = tmp.substr(next_coma +1, tmp.size() -(next_coma +1));
			if (pg->dynamic()){
				pg->addCellToDynamicRows(tmp, rset, row);
			}else{
				pg->addCellToRow(tmp, rset, row);
			}
		} 
	}
}

void
PageConverter::addCellToRow(const string& key, MsgGraph *rset, const string& row)
{
	map<string, RowConverter *>::iterator it = _grid.find(row);
	if (it != _grid.end()){
		RowConverter *gc = (*it).second;
		CellConverter *cc = new CellConverter(key, rset, gc);
		gc->addCell(cc);
	}
}

void
PageConverter::addCellToDynamicRows(const string& key, MsgGraph *rset, const string& row)
{
	//cout<< "addCellToDynamicRows for "<<key<<endl;
	map<string, RowConverter *>::iterator it = _grid.find(row);
	if (it != _grid.end()){
		RowConverter *gc = (*it).second;
		vector<RowConverter *>::iterator itv = std::find(_dynamicRows.begin(), _dynamicRows.end(), gc);
		if (itv == _dynamicRows.end()){
			//it's static rows
			CellConverter *cc = new CellConverter(key, rset, gc);
			gc->addCell(cc);
		}else{
			vector<RowConverter *>::iterator itd = _dynamicRows.begin();
			for (itd; itd != _dynamicRows.end(); itd++){
				RowConverter *gc = (*itd);
				CellConverter *cc = new CellConverter(key, rset, gc);
				gc->addCell(cc);
			}
		}
	}
}

void
PageConverter::publishUpdate(Converter *c)
{
	//cout << "PublishUpdate to "<<c->row()<<" "<<c->col()<<" "<<c->value()<<endl;
	if (_prod)
		_prod->updatePage(c);

}

string
PageConverter::getPageName(const string& url)
{
	string pagename;
	string page = "page=";
	int page_id = url.find(page);
	if (page_id != string::npos){
		int next_coma = url.find_first_of(',', page_id);
		pagename = url.substr(page_id + page.size(), next_coma - (page_id + page.size()));
		pagename = Formatter::trim(pagename);
	}

	return pagename;
}

bool
PageConverter::loaded()
{
    bool loaded = true;
    map<string, RowConverter *>::iterator it = _grid.begin();

    for (it; loaded && it != _grid.end(); it++)
    {
	RowConverter *gc = (RowConverter *)(*it).second;
	loaded &= gc->loaded();
    }
    
    return( loaded && s_rmdsPageProducer->connected());
}


set<string>
PageConverter::getGridKeys() 
{
	set<string> keys;
	map<string, RowConverter *>::iterator it = _grid.begin();
	for (it; it != _grid.end(); it++){
		keys.insert((*it).first);
	}
	return keys;
}

int
PageConverter::rowCnt()
{
    DataPage* page = getDataPage();

    return( page != NULL ? page->height() : 0 );
}


void
PageConverter::onTimer(TimerId id)
{
    if ( loaded() )
    {
	Logger::getLogger()->log(Logger::TYPE_INFO, 
				 "PageConverter::onTimer Loaded IMAGE for page: [%s]",
				 _pname.c_str());

	_prod = s_rmdsPageProducer;

	// Build page IMAGE and publish
	publishUpdate(this);
    } 
    else 
    {
	addTimer(1*1000, 1, this);
    }
}
