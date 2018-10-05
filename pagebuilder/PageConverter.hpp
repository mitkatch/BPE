/************************************************************************
||
|| DATE:                $Date: 2006/06/19 21:32:59 $
|| SOURCE:              $Source: /repository/projects/Canpx/pagebuilder/PageConverter.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: PageConverter.hpp,v 1.6 2006/06/19 21:32:59 mikhail Exp $
|| REVISION:    $Revision: 1.6 $
|| LOG:                 $Log: PageConverter.hpp,v $
|| LOG:                 Revision 1.6  2006/06/19 21:32:59  mikhail
|| LOG:                 cannot read the set of fileds from the parser
|| LOG:
|| LOG:                 Revision 1.5  2006/04/06 17:57:31  nickz
|| LOG:                 port to RMDS
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

#ifndef PAGECONVERTER_HPP
#define PAGECONVERTER_HPP

#include <algorithm>


#include "rmdsPageProducer.hpp"
#include "DBConnectorLdap.hpp"
#include "MsgGraph.hpp"
#include "DataPage.hpp"
#include "Converter.hpp"
#include "RowConverter.hpp"
#include "ChainManager.hpp"
#include "TimerCallback.hpp"


class PageArrayConverter;

class PageConverter : public Converter, public TimerCallback
{
	public:
		enum {
			MTRS_PAGE_ROW_MIN_IND = 0,
			MTRS_PAGE_ROW_MAX_IND = 15
		};
	public:
		typedef void (*Loader)(MsgGraph *graph, PageConverter* pg);

		PageConverter(PageArrayConverter *array, const string& ldapurl, const string& pname);
		virtual ~PageConverter();

		virtual void load(const string& url);
		virtual void publishUpdate(Converter *c);

		static void setPageProducer(rmdsPageProducer* prod)
		{
		    s_rmdsPageProducer = prod;
		};

		virtual void setChainMgr(ChainManager *cm){
			_chainMgr = cm;
		};
		
		virtual void dynamic(bool d){
			_dynamic = d;
		}

		virtual bool dynamic(){
			return _dynamic;
		}

		virtual int col()
		{
		    //return(1);
		    return(0);
		}

		virtual int row()
		{
		    //return(1);
		    return(0);
		}

    		virtual int rowCnt();

	        // Pure virtual required by Converter
    		virtual string value()
		{
		    return(_placeHolder);
		}

    		
		virtual set<string> getGridKeys();
		static string getPageName(const string & url);
		static void loadPageConfig(MsgGraph* rset, PageConverter *pg);
		static void loadRowConfig(MsgGraph* rset, PageConverter *pg);

		virtual void loadLdap(const string& connstr, const string& url, Loader loader);

		virtual bool loaded();
	protected:
		string _connstr, _host, _port ;
		string _pname;
    		string _service;
    		string _placeHolder;
		bool _dynamic;
		virtual bool subscribeSource(const string& rec, const string& row);
		virtual bool subscribeSourceChain(int counter, const string& row);
		virtual void addCellToRow(const string &key, MsgGraph *rset, const string& row);
		virtual void addCellToDynamicRows(const string& key, MsgGraph *rset, const string& row);
		virtual void onTimer(TimerId id);

	private:

		ChainManager *_chainMgr;
		map<string, RowConverter *> _grid;
		map<string, MsgGraph *> _msgGraphPatternMap;
		vector<RowConverter *> _dynamicRows;
		rmdsPageProducer* _prod;

    		static rmdsPageProducer* s_rmdsPageProducer;
};

#endif //PAGECONVERTER_HPP
