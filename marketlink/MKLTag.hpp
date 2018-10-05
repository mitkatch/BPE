
/************************************************************************
||
|| DATE:                $Date:$
|| SOURCE:              $Source:$
|| STATE:               $State:$
|| ID:                  $Id:$
|| REVISION:            $Revision:$
|| LOG:                 $Log:$
|| LOG:
************************************************************************/

#ifndef MKLTAG_HPP
#define MKLTAG_HPP

#include <vector>
using namespace std;


class MKLTag
{
	public:
		typedef vector<char> TAG;
		virtual ~MKLTag(){};
		static MKLTag* getTag();
		virtual MKLTag::TAG tag();
		static int tagVal(const MKLTag::TAG& tagVal);

	protected:
		MKLTag(){};
		static MKLTag* _tagInst;

		static int _tagVal;

};



#endif //MKLTAG_HPP
