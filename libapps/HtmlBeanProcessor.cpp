
/************************************************************************
||
|| DATE:                $Date: 2005/06/29 18:53:33 $
|| SOURCE:              $Source: /repository/projects/Canpx/libapps/HtmlBeanProcessor.cpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: HtmlBeanProcessor.cpp,v 1.2 2005/06/29 18:53:33 mikhailt Exp $
|| REVISION:    $Revision: 1.2 $
|| LOG:                 $Log: HtmlBeanProcessor.cpp,v $
|| LOG:                 Revision 1.2  2005/06/29 18:53:33  mikhailt
|| LOG:                 *** empty log message ***
|| LOG:
|| LOG:                 Revision 1.1  2005/03/29 16:05:55  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#include "Formatter.hpp"
#include "Reflection.hpp"
#include "HtmlBeanProcessor.hpp"

HtmlBeanProcessor* HtmlBeanProcessor::_processor = NULL;

HtmlBeanProcessor::HtmlBeanProcessor()
{

}

HtmlBeanProcessor::~HtmlBeanProcessor()
{


}

HtmlBeanProcessor *
HtmlBeanProcessor::getProcessor()
{
	if ( _processor == NULL){
		_processor = new HtmlBeanProcessor();
	}

	return _processor;

}

bool
HtmlBeanProcessor::loadFile(const string& filename)
{
	if (HtmlBeanProcessor::getProcessor()->isFileLoaded(filename)){
		return true;
	}

	ifstream is;
	is.open(filename.c_str(), ifstream::in);
	if (is.bad()){
		Logger::getLogger()->log(Logger::TYPE_ERROR,"HTML file %s not found.", filename.c_str());
		return false;;
	}
	
	char line[1024];
	string buf;
	while (is.good()){
		is.getline(line, 1024);
		buf.append(line);
		buf.append(1, '\n');

	}
	HtmlBeanProcessor::getProcessor()->addNewFile(filename, buf);
	is.close();

	return true;

}

void
HtmlBeanProcessor::addNewFile(const string& filename, const string& file)
{
	_filesLoaded[filename] = file;

}

bool
HtmlBeanProcessor::isFileLoaded(const string& filename)
{
	map<string, string>::iterator it = _filesLoaded.find(filename);
	if (it != _filesLoaded.end()){
		return true;
	}
	return false;

}

string
HtmlBeanProcessor::process(const string& filename)
{
	map<string, string>::iterator it = _filesLoaded.find(filename);
	if (it != _filesLoaded.end()){
		return build((*it).second);
	} else {
		return string("");
	}

}

string
HtmlBeanProcessor::build(const string& src)
{
	string out = src;
	string htmllistname = "<" + string(MLCheckList::name());
	int ml_id = out.find(htmllistname, 0);
	while(ml_id != string::npos){
		ml_id = MLCheckList::build(out, ml_id);
	}

	string htmltabname = "<" + string(MLCheckTable::name());
	ml_id = out.find(htmltabname, 0);
	while(ml_id != string::npos){
		ml_id = MLCheckTable::build(out, ml_id);
	}

	return out;
}

string
MLCheckTable::toString(const string& var)
{
	string out;
	StringCheckMap found = Reflection::getReflection()->getMap(Formatter::trim(var));
	if (found.size() == 0){
		out = "Nothing Found for MLCheckTable";
	}else{
		out = "<TABLE border>";
		StringCheckMap::iterator it = found.begin();
		for (it; it != found.end(); it++){
			out += "<TR><TD><TABLE border bgcolor=#DAD510>\n" ;
			out += "<TR><TD>" + (*it).first.str1 + "</TD>";
			out += "<TD><B>" + (*it).first.str2 + "</B></TD>";
			out += "<TD>" + (*it).first.str3 + "</TD>";
			out += "<TD><B>" + (*it).first.str4 + "</B></TD>";
			out += "<TD>" + (*it).first.str5 + "</TD>";
			string value = " value='OFF' ";
			if ( ((*it).first).checked ){
				value = " value='ON' checked ";
			} 
			out += "<TD><INPUT type='checkbox' name='" + (*it).first.str1 +"' " + value;
			out += "></TD></TR></TABLE></TD>\n";
			//building second segment
			if (((*it).second).size() != 0){
				out += "<TD><TABLE border bgcolor=#3797B3>";
				vector<StringCheck>& v = (*it).second;	
				for (int i = 0; i < v.size(); i++){
					out += "<TR><TD>" + (v[i]).str1 + "</TD>";
					out += "<TD><B>" + (v[i]).str2+ "</B></TD>";
					out += "<TD>" + (v[i]).str3+ "</TD>";
					out += "<TD><B>" + (v[i]).str4+ "</B></TD>";
					out += "<TD>" + (v[i]).str5+ "</TD>";
					string value = " value='OFF' ";
					if ( (v[i]).checked ){
						value = " value='ON' checked ";
					} 
					out += "<TD><INPUT type='checkbox' name='" + (v[i]).str1 +"' " + value;
					out += "></TD></TR>";
				}
				out += "</TABLE></TD>\n";

			}
		}
		out+= "</TABLE>";
	}

	return out;
}

string
MLCheckList::toString(const string& var)
{
	string out;
	set<StringCheck> found = Reflection::getReflection()->getSet(Formatter::trim(var));
	if (found.size() == 0){
		out = "Nothing Found for MLCheckList";
	}else{
		out = "<TABLE border>";
		set<StringCheck>::iterator it = found.begin();
		for (it; it != found.end(); it++){
			out += "<TR><TD>" + (*it).str1 + "</TD>";
			out += "<TD><B>" + (*it).str2 + "</B></TD>";
			out += "<TD>" + (*it).str3+ "</TD>";
			out += "<TD><B>" + (*it).str4 + "</B></TD>";
			out += "<TD>" + (*it).str5+ "</TD>";
			string value = " value='OFF' ";
			if ( (*it).checked ){
				value = " value='ON' checked ";
			} 
			out += "<td><input type='checkbox' name='" + (*it).str1 +"' " + value;
			out += "></td></tr>";

		}
		out+= "</TABLE>";
	}
	return out;
}

int
MLCheckList::build(string& src, int start)
{
	string name = "<" + string(MLCheckList::name());
	int mllist_end = src.find("/>", start + name.size());
	if (mllist_end != string::npos){
		int mllist_size = start + name.size() + mllist_end; 
		string var = src.substr(start + name.size() +1, mllist_end - (start + name.size() +1));
		string mllist = MLCheckList::toString(var);  
		src.replace(start, mllist_end + 2 - start, mllist); 
	}
	return src.find(name, start + name.size()); 
}

int
MLCheckTable::build(string& src, int start)
{
	//it's ugly I'll fix it later on
	string name = "<" + string(MLCheckTable::name());
	int mllist_end = src.find("/>", start + name.size());
	if (mllist_end != string::npos){
		int mllist_size = start + name.size() + mllist_end; 
		string var = src.substr(start + name.size() +1, mllist_end - (start + name.size() +1));
		string mllist = MLCheckTable::toString(var);  
		src.replace(start, mllist_end + 2 - start, mllist); 
	}
	return src.find(name, start + name.size()); 
}

