
#ifndef NSMAP_HPP
#define NSMAP_HPP

struct URLNameSpace {
        string url;
        string source;
        string page;
	string slot;
	string security_id;
};

typedef map<string, URLNameSpace> NSMap;




#endif //NSMAP_HPP
