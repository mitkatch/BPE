
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

#ifndef MKLTHROTTLED_HPP
#define MKLTHROTTLED_HPP

class MKLThrottled
{
	public:
		MKLThrottled(): _ceiling(0), _rate(0), _credit(0){};
		virtual ~MKLThrottled(){};

		virtual void addWaitTime(int sec) = 0;
		virtual void addRefresh() = 0;

		virtual void addCeiling(int i){
			_ceiling = i;
		}

		virtual void addCredit (int i){
			_credit = i;
		}

		virtual void addRate (int i ){
			_rate = i;
		} 

	protected:
		int _ceiling;
		int _credit;
		int _rate;

};


#endif //MKLTHROTTLED_HPP
