/************************************************************************
||
|| DATE:                $Date: 2005/09/28 19:17:28 $
|| SOURCE:              $Source: /repository/projects/Canpx/libutil/MsgGraph.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: MsgGraph.hpp,v 1.5 2005/09/28 19:17:28 mikhailt Exp $
|| REVISION:    $Revision: 1.5 $
|| LOG:                 $Log: MsgGraph.hpp,v $
|| LOG:                 Revision 1.5  2005/09/28 19:17:28  mikhailt
|| LOG:                 copy constructor
|| LOG:
|| LOG:                 Revision 1.4  2005/09/09 16:53:03  mikhailt
|| LOG:                 ldap compatible
|| LOG:
|| LOG:                 Revision 1.3  2005/03/29 16:09:36  mikhailt
|| LOG:                 delta added
|| LOG:
|| LOG:                 Revision 1.2  2004/04/29 19:49:16  mikhailt
|| LOG:                 migration to SUNPRO
|| LOG:
|| LOG:                 Revision 1.1  2004/01/08 20:52:40  tomcat
|| LOG:                 Initial revision
|| LOG:
************************************************************************/


#ifndef MSGGRAPH_HPP
#define MSGGRAPH_HPP

#include <map>
#include <set>
#include <string>
#include <vector>

using std::string;
using std::map;
using std::set;
using std::vector;


class MsgGraph 
{
	public:
		typedef map<string, string> GraphNode;
	public:
		MsgGraph();
		MsgGraph( const MsgGraph &mg);
		virtual ~MsgGraph();

		virtual bool addEntry(const string& entry);
		virtual bool addGraphNode(const string& entry, int index, const string& key, const string& val);
		virtual bool addLevel(const string& entry);
		virtual int getLevels(const string &address);

		virtual bool getAttributeValue(const string& attr, vector<string> &val);
		virtual bool getAttributeValue(const string& attr, int index, string &val);

		virtual bool getAttributeValueList(const string& attr, vector<string>& value){return false;};
		virtual set<string> getKeys(const string& dir, int level = 0);		
	protected:
		map<string, vector<GraphNode> > _msgImage;
};
#endif //MSGGRAPH_HPP
