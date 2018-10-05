/************************************************************************
||
|| DATE:                $Date: 2006/04/06 17:57:28 $
|| SOURCE:              $Source: /repository/projects/Canpx/pagebuilder/CellConverter.hpp,v $
|| STATE:               $State: Exp $
|| ID:                  $Id: CellConverter.hpp,v 1.3 2006/04/06 17:57:28 nickz Exp $
|| REVISION:    $Revision: 1.3 $
|| LOG:                 $Log: CellConverter.hpp,v $
|| LOG:                 Revision 1.3  2006/04/06 17:57:28  nickz
|| LOG:                 port to RMDS
|| LOG:
|| LOG:                 Revision 1.2  2005/09/21 17:49:55  mikhailt
|| LOG:                 activated only when loaded()
|| LOG:
|| LOG:                 Revision 1.1  2005/06/29 19:00:48  mikhailt
|| LOG:                 Initial revision
|| LOG:
************************************************************************/

#ifndef CELLCONVERTER_HPP
#define CELLCONVERTER_HPP

#include <iostream>
#include <strstream>
#include <vector>
#include "MsgGraph.hpp"
#include "Converter.hpp"
#include "DataGraph.hpp"
#include "CFormat.hpp"
#include "CVideo.hpp"

using std::strstream;
using std::ios_base;
using std::vector;

class RowConverter;

class CellConverter: public Converter
{
	public:
		CellConverter(const string& key, MsgGraph *rset, RowConverter *rc);
		virtual ~CellConverter();
		
		virtual string address() const{
			return _address;
		};

		virtual string datumAddress(){
			return _datumAddress;
		}

		virtual vector<string> datumField(){
			return _datumField;
		};

		virtual bool dynamic(){
			return _dynamic;
		}
	
		virtual int col(){
			return _cX;
		}

		virtual int row();

    		virtual int rowCnt()
		{
		    return(1);
		}

		virtual string value(){
			return _displayValue;
		}
	
		virtual string formatParameter() const{
			return _formatParameter;
		}


		virtual void update(DataGraph *data);
		virtual void update(const string& val, int video);
		virtual void update(const string& val);
	protected:
		void setValue(const string& val);
		void setDispStream();
		string setDisplayValue(const string& val); 
		void setDatumField(const string& field);
		string _address;
		int _cLength;
		int _cX, _cY;
		int _colNum;
		string _cValue, _cFormatStr;
		int _cAlignment;
		CVideo* _cVideo; 
		int _currentVideo;
		string _displayValue;
		bool _dynamic;
		vector<string> _datumField;
		string _datumAddress;
		RowConverter *_myRow;
		CFormat* _cFormat;
		string _formatParameter;

};


#endif //CELLCONVERTER_HPP

