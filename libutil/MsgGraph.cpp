
/************************************************************************
||
|| DATE:                $Date: 2006/04/06 18:01:23 $
|| SOURCE:              $Source: /repository/projects/Canpx/libutil/MsgGraph.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: MsgGraph.cpp,v 1.6 2006/04/06 18:01:23 nickz Exp $
|| REVISION:    $Revision: 1.6 $
|| LOG:                 $Log: MsgGraph.cpp,v $
|| LOG:                 Revision 1.6  2006/04/06 18:01:23  nickz
|| LOG:                 Controlled logging
|| LOG:
|| LOG:                 Revision 1.5  2006/04/03 19:36:17  nickz
|| LOG:                 Cleaned up logging
|| LOG:
|| LOG:                 Revision 1.4  2005/09/28 19:17:28  mikhailt
|| LOG:                 copy constructor
|| LOG:
|| LOG:                 Revision 1.3  2005/09/09 16:53:03  mikhailt
|| LOG:                 ldap compatible
|| LOG:
|| LOG:                 Revision 1.2  2005/03/29 16:09:36  mikhailt
|| LOG:                 delta added
|| LOG:
|| LOG:                 Revision 1.1  2004/01/08 20:52:40  tomcat
|| LOG:                 Initial revision
|| LOG:
************************************************************************/


#include "MsgGraph.hpp"
#include "Logger.hpp"

MsgGraph::MsgGraph()
{

}

MsgGraph::MsgGraph(const MsgGraph& mg)
{
	_msgImage = mg._msgImage;	

}

MsgGraph::~MsgGraph()
{
}

bool
MsgGraph::getAttributeValue(const string& attr, vector<string> &val)
{
    bool found = false;

    //attr looks like subj_out@FIELD.REC.ML
    unsigned int delim = attr.find_first_of('@');
    string theAttr = attr.substr(0, delim);
    string theAddr = attr.substr(delim+1, string::npos);

    map<string, vector< GraphNode> >::iterator it = _msgImage.find(theAddr);
    if ( it != _msgImage.end())
    {
	vector<GraphNode>& v = (*it).second;
	for (int index = 0; index < v.size(); index++)
	{
	    GraphNode  gn = v[index];
	    map<string, string>::iterator git = gn.find(theAttr);
	    if (git != gn.end())
	    {
		val .push_back((*git).second);
		found = true;
	    }
	}

//	Logger::getLogger()->log(Logger::TYPE_INFO,
//				 "MsgGraph::getAttributeValue(vector) Found %d entries for attribute: [%s]",
//				 val.size(), attr.c_str());
    }
    else
    {
	Logger::getLogger()->log(Logger::TYPE_ERROR, "MsgGraph::getAttributeValue(vector) vector: [%s] not found.", 
				 theAddr.c_str());
    }

    return(found);
}

bool
MsgGraph::getAttributeValue(const string& attr, int index, string &val)
{
    bool found = false;

    //attr looks like subj_out@FIELD.REC.ML
    unsigned int delim = attr.find_first_of('@');
    string theAttr = attr.substr(0, delim);
    string theAddr = attr.substr(delim+1, string::npos);

    map<string, vector< GraphNode> >::iterator it = _msgImage.find(theAddr);
    if ( it != _msgImage.end())
    {
	if (index <= (int)(*it).second.size() -1)
	{
	    GraphNode  gn = (*it).second[index];
	    map<string, string>::iterator git = gn.find(theAttr);
	    if (git != gn.end())
	    {
		val = (*git).second;
		found = true;
	    }
	}

//	Logger::getLogger()->log(Logger::TYPE_INFO,
//				 "MsgGraph::getAttributeValue %s attribute[%d]: [%s]", 
//				 (found ? "found" : "did not find"), index, attr.c_str());
    }
    else
    {
	Logger::getLogger()->log(Logger::TYPE_ERROR, "MsgGraph::getAttributeValue: [%s] not found.", 
				 theAddr.c_str());
	
    }
    
    return(found);
}

bool
MsgGraph::addEntry(const string& entry)
{
	bool ret = false;
	if( _msgImage.insert(map<string, vector<GraphNode> >::value_type(entry, vector<GraphNode >() )).second) {
		_msgImage[entry].push_back(GraphNode());
		ret = true;
	}
	return ret;
	
}

bool
MsgGraph::addLevel(const string& entry)
{
	bool ret = false;
	map<string, vector<GraphNode> >::iterator it = _msgImage.find(entry);
	if (it != _msgImage.end()){
		(*it).second.push_back(GraphNode());
		ret = true;
	}
	return ret;

}

int
MsgGraph::getLevels(const string& address)
{
	int ret = 0;
	map<string, vector<GraphNode> >::iterator it = _msgImage.find(address);
	if (it != _msgImage.end()){
		ret = (*it).second.size();
	}
	return ret;
}

bool 
MsgGraph::addGraphNode(const string& entry, int index, const string& key, const string& val)
{
	bool ret = false;

	map<string, vector<GraphNode> >::iterator it = _msgImage.find(entry);
	if (it != _msgImage.end()){
		if (index <= (int)((*it).second).size() -1){
			ret = (*it).second[index].insert(map<string, string>::value_type(key, val)).second;
		}else{
			addLevel(entry);
			ret = addGraphNode(entry, index, key, val);
		}
	}
	//cout <<"addGraphNode "<<entry <<", index="<< index<<" key="<<key<<", value="<<val<<" return "<<ret<<endl;
	return ret;

}

set<string> 
MsgGraph::getKeys(const string& dir, int level)
{
	set<string> keys;
	if (dir.empty() || (dir == ".")){
		map<string, vector< GraphNode> >::iterator it = _msgImage.begin();
		for(it; it != _msgImage.end(); it++){
			keys.insert((*it).first);

		}
	} else {
		map<string, vector< GraphNode> >::iterator it = _msgImage.find(dir);
		if (it != _msgImage.end()){
			if ( ((*it).second).size() > level){
				GraphNode  gn = (*it).second[level];
				map<string, string>::iterator git = gn.begin();
				for (git; git != gn.end(); git++){
					keys.insert((*git).first);
				}
			}
		}
	}

	return keys;

}
