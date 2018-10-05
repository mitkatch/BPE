
/************************************************************************
||
|| DATE:                $Date: 2004/11/19 21:09:14 $
|| SOURCE:              $Source: /repository/projects/Canpx/libapps/MsgParser.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: MsgParser.cpp,v 1.4 2004/11/19 21:09:14 mikhailt Exp $
|| REVISION:    $Revision: 1.4 $
|| LOG:                 $Log: MsgParser.cpp,v $
|| LOG:                 Revision 1.4  2004/11/19 21:09:14  mikhailt
|| LOG:                 xml handling
|| LOG:
|| LOG:                 Revision 1.3  2004/10/20 19:48:24  mikhailt
|| LOG:                 PageParser compatible
|| LOG:
|| LOG:                 Revision 1.2  2004/04/13 20:01:30  mikhailt
|| LOG:                 parse http return true
|| LOG:
|| LOG:                 Revision 1.1  2004/01/08 20:51:46  tomcat
|| LOG:                 Initial revision
|| LOG:
************************************************************************/


#include <expat.h>
#include "MsgParser.hpp"
#include "Logger.hpp"

//int Depth, tmpDepth;
//string subject;

bool
MsgParser::xmlParse(string & msg)
{
	//cout<<"MsgParser initiated ..."<<endl;
	XML_Parser p = XML_ParserCreate(NULL);

	if (!p){
		Logger::getLogger()->log(Logger::TYPE_ERROR, "MsgParser::MsgParser Can't initialize XML_ParserCreate");	
		_Depth = _tmpDepth = 0;
		_subject = "";
		return false;
	}
	

	XML_SetUserData(p, this);
	XML_SetElementHandler(p, MsgParser::start, MsgParser::end);
	for(;;){
		int len = msg.size();
		int  done = 1;
		//cout<<"Try to xmlParse with "<<len<<"::"<<done<<endl;
		if(XML_Parse(p, msg.c_str(), len, done) == XML_STATUS_ERROR){
			Logger::getLogger()->log(Logger::TYPE_ERROR, "MsgParser::xmlParser error at line %d:\n%s\n", XML_GetCurrentLineNumber(p), 
				XML_ErrorString(XML_GetErrorCode(p)));	
			Logger::getLogger()->log(Logger::TYPE_ERROR, "MsgParser::xmlParser error in \n%s\n", msg.c_str()); 
				XML_ParserFree(p);
				_Depth = _tmpDepth = 0;
				_subject = "";
				return false;
		}
		if (done)
			break;
	}
	XML_ParserFree(p);
	_Depth = _tmpDepth = 0;
	_index = 0;
	_subject = "";
	return true;
}

void XMLCALL
MsgParser::start(void *data, const char *el, const char **attr)
{
	int i;
	string tmpSubject;
	MsgParser *msgParser = (MsgParser *)data;

	if (msgParser->_Depth == 0){
		msgParser->_subject = el;
		msgParser->_tmpDepth = msgParser->_Depth;
		msgParser->_index = 0;
		(msgParser->_image).addEntry(msgParser->_subject);

	}

	if (msgParser->_tmpDepth != msgParser->_Depth){
		//go to new level
		tmpSubject = msgParser->_subject;
		msgParser->_subject = el;
		msgParser->_subject += ".";
		msgParser->_subject += tmpSubject;
		msgParser->_tmpDepth = msgParser->_Depth;
		//cout<<"Create map entry subject "<<msgParser->_subject<<endl;
		(msgParser->_image).addEntry(msgParser->_subject);
		msgParser->_index = 0;
	}else{
		if (msgParser->_Depth != 0){
			msgParser->_index++;
			//on the same level -> add index for vector
			(msgParser->_image).addLevel(msgParser->_subject);
		}
		
	}
	for (i = 0; attr[i]; i += 2) {
		(msgParser->_image).addGraphNode(msgParser->_subject, msgParser->_index, string(attr[i]), string(attr[i + 1]));
		printf(" %s='%s'", attr[i], attr[i + 1]);
	}

	printf("\n");
	msgParser->_Depth++;

}

void XMLCALL
MsgParser::end(void *data, const char *el)
{
	MsgParser *msgParser = (MsgParser *)data;
	msgParser->_Depth--;
}

bool
MsgParser::httpParse(string & msg)
{
	/** HTTP 1.1 supported 
	method
	url
	parsed attached url request by "&" and "="
	all attributes by first :
	***/
	//split msg on r n

	int n = msg.length();
	int start, stop, count = 0;
	string line;
	string DELIM = "\r\n";
	string HTTP = "HTTP";
	
	start = msg.find_first_not_of(DELIM);
	while ((start >= 0) && (start < n) ) {
		stop = msg.find_first_of(DELIM, start);
		if ( (stop < 0 ) || (stop > n) )
			stop = n;

		line = msg.substr(start, stop - start);
		//cout <<"LINE FOUND->"<<count<<" "<<line<<endl;
		if (count++ == 0){ //first line
			if(line.find("HTTP/1.") != string::npos){
				_image.addEntry(HTTP);
			}
			int sp_st = line.find_first_not_of(' ');
			if (sp_st >= 0){
				int sp_end = line.find_first_of(' ',sp_st);
				if (sp_end > 0){
					string method = line.substr(sp_st, sp_end - sp_st);
					_image.addGraphNode(HTTP, 0, string("method"), method);
					sp_st = line.find_first_not_of(' ', sp_end+1);	
					sp_end = line.find_first_of('?', sp_st); 
					if (sp_end == string::npos) { // no request found
						sp_end = line.find_first_of(' ', sp_st);

					}
					if (sp_st > 0 && sp_end > 0){
						string url = line.substr(sp_st, sp_end - sp_st);
						_image.addGraphNode(HTTP, 0, string("url"), url);
						sp_st = sp_end +1;
						sp_end = line.find_first_of(' ', sp_st);
						if (sp_st > 0 && sp_end > 0){
							string request = line.substr(sp_st, sp_end - sp_st);
							_image.addGraphNode(HTTP, 0, string("request"), request);
						}	
						
					}	
				}	
				

			}


		} else { 
			int st = line.find_first_of(':');
			if (st > 0){
				string key = line.substr(0, st);
				string val = line.substr(st+1, string::npos);
				_image.addGraphNode(HTTP, 0, key, val);
			}
		} 
		start = msg.find_first_not_of(DELIM, stop +1);
	}	
	return true;
}

