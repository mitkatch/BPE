#! /usr/local/bin/python
import sys
import DataCache 
import re 
import string
import time
import os.path
import copy
import Inst
import services

##############################################################
class GenBond(Inst.Inst):
	def __init__(self, name, page, strlbl):
		Inst.Inst.__init__(self)
		self.symbol = 'GENERAL.' + name + '.' + strlbl 
		self.name = name
		self.page = page
		self.last_trade = ('', '', '', '')
		self.label_slot = ()
		self.data_slot = ()
		self.type = 'FI'
		self.subtype = 'BOND'
		self.subtype1 = 'STATE'	
		self.coupon = ' '
		self.desc = strlbl
		self.issuer = 'GOC'

######################################################################

class ShorcanBond(GenBond):
	def __init__(self, name, page, strlbl):
		GenBond.__init__(self, name, page, strlbl)
		self.symbol = services.shorcan + '.' + name + '.' + strlbl 
		t = re.search(r'([0-9]+)_([0-9]{4})', strlbl)
		if t:
			self.coupon, mat = t.groups()
			self._setMat(mat)
			couplen = len(self.coupon)
			handle = self.coupon[0:couplen-2]
			fraction = self.coupon[couplen-2:]
			self.coupon = handle + '.' + fraction
		
	def _setMat(self, mat):
		try:
			mytime = time.strptime(mat, '%m%y')
			self.mat = time.strftime('%m_%Y', mytime)
			self.matDD = '01' 
			self.matMM =  time.strftime('%m', mytime)
			self.matYYYY =  time.strftime('%Y', mytime)
			return time.strftime('%m%y', mytime)
		except ValueError, e:
			return mat


	def _toBid(self, data):
		if (re.search(r'[0-9]', data[4:7])):
			try:
				handle = int(data[0:3])
				bid_fraction = data[4:7]
				return str(handle) + '.' + bid_fraction 
			except ValueError, e:
                		print 'S:Python::_toBid error ', e, '(',data[0:3],') [', data , ']'
				return '0'
		return '0'

	def _toAsk(self, data):
		if (re.search(r'[0-9]', data[8:11])):
			try:
				handle = int(data[0:3])
				bid_fraction = data[4:7]
				ask_fraction = data[8:11]
				if ask_fraction < bid_fraction:
					handle = handle + 1
	
				return str(handle) + '.' + str(ask_fraction) 
			except ValueError, e:
                		print 'Python::_toAsk error ', e, data
				return '0'
		else:
			return '0'


	def _toBidSize(self, data):
		if (re.search(r'[0-9]', data[12:15])):
			return data[12:15]
		else:
			return '0'

	def _toAskSize(self, data):
		if (re.search(r'[0-9]', data[16:19])):
			return data[16:19]
		else:
			return '0'

	def _toAskYield(self, data):
		if (re.search(r'[0-9]', data[26:29])):
			try:
				handle = int(data[20:22])
				bid_yfraction = data[22:25]
				ask_yfraction = data[26:29]
				bid_exist = re.search(r'[0-9]', bid_yfraction)	
				if bid_exist and ask_yfraction > bid_yfraction:
					handle = handle - 1
				return str(handle) + '.' + str(ask_yfraction) 
			except ValueError, e:
                		print 'Python::_toAskYeild error ', e
				return '0'

		else:
			return '0'

	def _toBidYield(self, data):
		if (re.search(r'[0-9]', data[22:25])):
			try:
				handle = int(data[21])
				return str(handle) + '.' + data[22:25]
			except ValueError, e:
                		print 'Python::_toBidYeild error ', e, ', data = ', data[22:25]
				return '0'
		else:
			return '0'
	
	def _getTradeDirection(self, data, r1, r2, c1, c2):
		# if Bid is flashing it's Hit
		tradeDir = ' ' 
		#askSizeVideo = DataCache.getVideo(self.page, int(r1), int(c1) + 17) # askSize 
		#bidSizeVideo = DataCache.getVideo(self.page, int(r1), int(c1) + 13) # bibSize
		# check Price	
		askSizeVideo = DataCache.getVideo(self.page, int(r1), int(c1) + 8) # askSize 
		bidSizeVideo = DataCache.getVideo(self.page, int(r1), int(c1) + 4) # bibSize
		if (askSizeVideo & 0x01) == 0x01:
			tradeDir = 'T'
		if (bidSizeVideo & 0x01) == 0x01:
			tradeDir = 'H'
	
		print 'Python::_getTradeDirection Shorcan: ', tradeDir, ', askSizeVideo: ', askSizeVideo, ', bidSizeVideo: ', bidSizeVideo
		return tradeDir	

	def _toLastTradePrice(self, bid, ask, tradeDir):
		# set trade_price in the begging of the trade and
		# clear out for complition
		(price, size, tyield, trade_time) = self.last_trade
		new_time = time.strftime('%x %X', time.localtime())
		if tradeDir == 'H' and price == '' :
			self.last_trade = (bid, size, tyield, new_time)
			return bid 
		elif tradeDir == 'T'and price == '':
			self.last_trade = (ask, size, tyield, new_time)
			return ask
		elif tradeDir == ' ':
			self.last_trade = ('', size, tyield, trade_time)
			return '0'  
		else:
			return price 

	def _toLastTradeSize(self, bid_size, ask_size, tradeDir):
		# clear out after the compliton of the trade
		(price, size, tyield, trade_time) = self.last_trade
		if tradeDir == 'H':
			self.last_trade = (price, bid_size, tyield, trade_time)
			return bid_size
		elif tradeDir == 'T':
			self.last_trade = (price, ask_size, tyield, trade_time)
			return ask_size
		else:
			self.last_trade = (price, '', tyield, trade_time)
			return '0' 

	def _toLastTradeYield(self, bid_yield, ask_yield, tradeDir):
		(price, size, tyield, trade_time) = self.last_trade
		if tradeDir == 'H':
			self.last_trade = (price, size, bid_yield, trade_time)
			return bid_yield
		elif tradeDir == 'T':
			self.last_trade = (price, size, ask_yield, trade_time)
			return ask_yield
		else:
			self.last_trade = (price, size, '', trade_time)
			return '0' 

	def _toLastTradeTime(self):
		(price, size, tyield, trade_time) = self.last_trade
		if trade_time == '':
			return ' '
		return trade_time


##########################################################################

class PrebonBond(GenBond):
	def __init__(self, name, page, strlbl):
		GenBond.__init__(self, name, page, strlbl)
		matY = ' '
		matM = ' '
		t = re.search(r'([0-9]*\.[0-9]*) ([A-Z]{3})([0-9]{2})', strlbl)
		if t:
			self.coupon, matM, matY = t.groups()

		strCoup = re.sub('\.', '', self.coupon)
		mat = matM + matY
		strMat = self._setMat(mat)
		self.symbol = services.prebon + '.' + name + '.' + strCoup + '_' + strMat
		
	def _setMat(self, mat):
		try:
			mytime = time.strptime(mat, '%b%y')
			#print "Python::PrebonBond::_toStrMat ", mytime
			self.mat = time.strftime('%m_%Y', mytime)
			self.matDD = '01' 
			self.matMM =  time.strftime('%m', mytime)
			self.matYYYY =  time.strftime('%Y', mytime)
			return time.strftime('%m%y', mytime)
		except ValueError, e:
			return mat

	def _toBid(self, data):
		if (re.search(r'[0-9]', data[4:7])):
			try:
				handle = int(data[0:3])
				bid_fraction = data[4:7]
				return str(handle) + '.' + bid_fraction 
			except ValueError, e:
                		print 'P:Python::_toBid error ', e, data[0:3], ' [', data , ']'
				return '0'
		return '0'

	def _toAsk(self, data):
		if (re.search(r'[0-9]', data[8:11])):
			try:
				handle = int(data[0:3])
				bid_fraction = data[4:7]
				ask_fraction = data[8:11]
				if ask_fraction < bid_fraction:
					handle = handle + 1
	
				return str(handle) + '.' + str(ask_fraction) 
			except ValueError, e:
                		print 'Python::_toAsk error ', e, data
				return '0'
		else:
			return '0'


	def _toBidSize(self, data):
		if (re.search(r'[0-9]', data[12:15])):
			return data[12:15]
		else:
			return '0'

	def _toAskSize(self, data):
		if (re.search(r'[0-9]', data[16:19])):
			return data[16:19]
		else:
			return '0'

	def _toAskYield(self, data):
		if (re.search(r'[0-9]', data[28:34])):
			return data[28:34] 
		return '0'

	def _toBidYield(self, data):
		if (re.search(r'[0-9]', data[21:27])):
			return data[21:27]
		return '0'
	
	def _getTradeDirection(self, data, r1, r2, c1, c2):
		# if Bid is flashing it's Hit
		tradeDir = ' ' 
		#askSizeVideo = DataCache.getVideo(self.page, int(r1), int(c1) + 17) # askSize 
		#bidSizeVideo = DataCache.getVideo(self.page, int(r1), int(c1) + 13) # bibSize
		# chack Price
		askSizeVideo = DataCache.getVideo(self.page, int(r1), int(c1) + 8) # askSize 
		bidSizeVideo = DataCache.getVideo(self.page, int(r1), int(c1) + 4) # bibSize
		if (askSizeVideo & 0x01) == 0x01:
			tradeDir = 'T'
		if (bidSizeVideo & 0x01) == 0x01:
			tradeDir = 'H'
	
		return tradeDir	

	def _toLastTradePrice(self, bid, ask, tradeDir):
		# set trade_price in the begging of the trade and
		# clear out for complition
		(price, size, tyield, trade_time) = self.last_trade
		new_time = time.strftime('%x %X', time.localtime())
		if tradeDir == 'H' and price == '' :
			self.last_trade = (bid, size, tyield, new_time)
			return bid 
		elif tradeDir == 'T'and price == '':
			self.last_trade = (ask, size, tyield, new_time)
			return ask
		elif tradeDir == ' ':
			self.last_trade = ('', size, tyield, trade_time)
			return '0'  
		else:
			return price 

	def _toLastTradeSize(self, bid_size, ask_size, tradeDir):
		# clear out after the compliton of the trade
		(price, size, tyield, trade_time) = self.last_trade
		if tradeDir == 'H':
			self.last_trade = (price, bid_size, tyield, trade_time)
			return bid_size
		elif tradeDir == 'T':
			self.last_trade = (price, ask_size, tyield, trade_time)
			return ask_size
		else:
			self.last_trade = (price, '', tyield, trade_time)
			return '0' 

	def _toLastTradeYield(self, bid_yield, ask_yield, tradeDir):
		(price, size, tyield, trade_time) = self.last_trade
		if tradeDir == 'H':
			self.last_trade = (price, size, bid_yield, trade_time)
			return bid_yield
		elif tradeDir == 'T':
			self.last_trade = (price, size, ask_yield, trade_time)
			return ask_yield
		else:
			self.last_trade = (price, size, '', trade_time)
			return '0' 

	def _toLastTradeTime(self):
		(price, size, tyield, trade_time) = self.last_trade
		if trade_time == '':
			return ' '
		return trade_time


##########################################################################

def likeShorcanBond(strlbl):
	strlbl = strlbl.strip()
	strlbl = re.sub(' ', '_', strlbl)
	strlbl = re.sub('O', '0', strlbl)
	tmatch = re.search(r'([0-9]*)_([0-9]{4}$)', strlbl)
	if tmatch:
		#print 'Python::_likeShorcanBond ', (tmatch.group(0), tmatch.group(1))
		#return (tmatch.group(0), tmatch.group(1))
		return strlbl
	else:
		return None


def likePrebonBond(strlbl):
	strlbl = strlbl.strip()
	tmatch = re.search(r'([0-9]*\.[0-9]*) ([A-Z]{3})([0-9]{2})', strlbl)
	if tmatch:
		print 'Python::likePrebonBond ', tmatch.groups()
		#return (tmatch.group(0), tmatch.group(1))
		return strlbl
	return None


