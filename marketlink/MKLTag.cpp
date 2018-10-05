
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

#include "MKLTag.hpp"
#include "Logger.hpp"

MKLTag* MKLTag::_tagInst = NULL;
int MKLTag::_tagVal = 0;

MKLTag*
MKLTag::getTag()
{
	if (_tagInst == NULL)
		_tagInst = new MKLTag();

	return _tagInst;

}

MKLTag::TAG
MKLTag::tag()
{
	if (_tagVal > 0xfff){
		Logger::getLogger()->log(Logger::TYPE_INFO, "MKLTag::tag tagVal reset %i ", _tagVal);
		_tagVal = 0; 
	}

	//char TAG[2]={0x00, 0x00};
	MKLTag::TAG t(2, 0x00);
	t[0] = (_tagVal & 0x3f) + 0x40;
	t[1] = ((_tagVal >>6) & 0x3f) + 0x40;
	_tagVal++;
	return t;
}

int 
MKLTag::tagVal(const MKLTag::TAG& t)
{
	int tagvalue = -1; 
	if (t.size() != 2){
		Logger::getLogger()->log(Logger::TYPE_ERROR, "MKLTag::tagVal failed size=%i", t.size());

	} else {
		tagvalue = ((t[1] - 0x40) << 6) + (t[0] - 0x40);
		Logger::getLogger()->log(Logger::TYPE_INFO, "MKLTag::tagVal tagvalue %i found", tagvalue);
	}
	return tagvalue;
}
