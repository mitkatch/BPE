
/************************************************************************
||
|| DATE:                $Date: 2004/04/29 19:52:56 $
|| SOURCE:              $Source: /repository/projects/Canpx/libapps/MsgParser.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: MsgParser.hpp,v 1.2 2004/04/29 19:52:56 mikhailt Exp $
|| REVISION:    $Revision: 1.2 $
|| LOG:                 $Log: MsgParser.hpp,v $
|| LOG:                 Revision 1.2  2004/04/29 19:52:56  mikhailt
|| LOG:                 migration to SUNPRO
|| LOG:
|| LOG:                 Revision 1.1  2004/01/08 20:51:46  tomcat
|| LOG:                 Initial revision
|| LOG:
************************************************************************/


#ifndef MSGPARSER_HPP
#define MSGPARSER_HPP 

#include <iostream.h>
#include <expat.h>
#include <string>
#include <vector>
#include <map>

using std::string;
using std::vector;
using std::map;

#include "MsgGraph.hpp"

class MsgParser
{
	public:

		MsgParser(MsgGraph& image):_image(image){};
		virtual ~MsgParser(){};
		virtual bool xmlParse(string& msg);
		virtual bool httpParse(string& msg);
		int _Depth, _tmpDepth, _index;
		string _subject;
		MsgGraph& _image;
	protected:

		static void XMLCALL end(void *data, const char *el);
		static void XMLCALL start(void *data, const char *el, const char **attr);


};

#endif//MSGPARSER_HPP
