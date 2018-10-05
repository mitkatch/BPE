
/************************************************************************
||
|| DATE:                $Date: 2006/04/20 19:06:53 $
|| SOURCE:              $Source: /repository/projects/Canpx/pageparser/PyParser.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: PyParser.hpp,v 1.3 2006/04/20 19:06:53 nickz Exp $
|| REVISION:    $Revision: 1.3 $
|| LOG:                 $Log: PyParser.hpp,v $
|| LOG:                 Revision 1.3  2006/04/20 19:06:53  nickz
|| LOG:                 .
|| LOG:
|| LOG:                 Revision 1.2  2004/11/15 17:40:09  mikhailt
|| LOG:                 more-less stable
|| LOG:
|| LOG:                 Revision 1.1  2004/10/06 19:44:25  mikhailt
|| LOG:                 Initial revision
|| LOG:
|| LOG:                 Revision 1.1  2004/10/06 19:42:08  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/


#ifndef PYPARSER_HPP
#define PYPARSER_HPP
#include "Subscriber.hpp"
#include "PageListener.hpp"
#include "Datum.hpp"
#include "DataPage.hpp"
#include "PyProxy.hpp"
#include "NSMap.hpp"

class PyParser: public Subscriber, public PageListener
{
	public:
		PyParser(const URLNameSpace& ns);
		virtual ~PyParser();

		virtual void notify(Datum *datum);
		virtual void onData(DataPage *page, DataPage::PageSlot slot);

		virtual void setMySlot();
		virtual vector<DataPage::PageSlot> getMySlot()const;

		virtual void buildHtmlAttr(map<int, string>& htmlAttr, const DataPage::PageSlot& slot);
	protected:
		vector<DataPage::PageSlot> _my_slot;
		map<DataPage::PageSlot*, string> _mySlotMap;
		map<DataPage::PageSlot*, string> _myHtmlAttr;
		PyProxy* _proxy;
		string _parser_map;
		string _address;
		string _myPyName;
    		string _source;
};


#endif //PYPARSER_HPP
