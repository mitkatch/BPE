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

#########################################################################
class GenBill(Inst.Inst):
	def __init__(self, name, page, label):
		Inst.Inst.__init__(self)
		self.symbol = 'GENERAL.' + name + '.' + label 
		self.name = name
		self.page = page
		self.last_trade = ('', '', '')
		self.label_slot = ()
		self.data_slot = ()
		self.type = 'FI'
		self.subtype = 'BILL'
		self.subtype1 = 'STATE'
		self._setMat(label)
		self.desc = label
		self.issuer = 'GOC'

	def _setMat(self, mat):
		try:
			today = time.localtime()
			than = time.strptime(mat, '%b%d')
			thisM = time.strftime('%m', today) 
			thisD = time.strftime('%d', today)
			thatM = time.strftime('%m', than) 
			thatD = time.strftime('%d', than)
			nextyear = (today[0]+1, today[1], today[2], today[3], today[4], today[5], today[6], today[7], today[8])
			if (thisM < thatM):
				#this year
				mat = mat + time.strftime('%y', today)
			elif (thisM == thatM and int(thisD) <= int(thatD) -3):
				#this year
				mat = mat + time.strftime('%y', today)
			elif (thisM == thatM and thisD < thatD):
				#next year
				mat = mat + time.strftime('%y', nextyear)
			else:
				#next year
				mat = mat + time.strftime('%y', nextyear)

			mytime = time.strptime(mat, '%b%d%y')
			self.mat = time.strftime('%m_%Y', mytime)
			self.matDD = time.strftime('%d', mytime) 
			self.matMM =  time.strftime('%m', mytime)
			self.matYYYY =  time.strftime('%Y', mytime)
			return time.strftime('%m%y', mytime)
		except ValueError, e:
			return mat


########################################################################
class ShorcanBill(GenBill):
	def __init__(self, name, page, label):
		GenBill.__init__(self, name, page, label)
		self.symbol = services.shorcan + '.' + name + '.' + label 
		tmatch = re.search(r'(WI)', label)
		if tmatch:
			self.subtype2 = 'WI'

	def _toBid(self, data):
		if (re.search(r'[0-9]', data[8:10])):
			try:
				handle = int(data[4:6].strip())
				bid_fraction = data[8:10].strip()
				return str(handle) + '.' + bid_fraction 
			except ValueError, e:
				print 'S:Python::_toBid error ',  e, '(',data[0:3],') [', data , ']'
				return '0'
		return '0'

	def _toAsk(self, data):
		if (re.search(r'[0-9]', data[14:17])):
			try:
				handle = int(data[4:6])
				bid_fraction = data[8:10].strip()
				ask_fraction = data[14:17].strip()
				if len(bid_fraction) and len(ask_fraction):
					if int(ask_fraction) > int(bid_fraction):
						handle = handle - 1
				return str(handle) + '.' + str(ask_fraction)
			except ValueError, e:
				print 'Python::_toAsk error ', e, data
				return '0'
                else:
                        return '0'

	def _toBidSize(self, data):
		if (re.search(r'[0-9]', data[20:23])):
			return data[20:23].strip()
		else:
			return '0'

	def _toAskSize(self, data):
		if (re.search(r'[0-9]', data[25:28])):
			return data[25:28].strip()
		else:
			return '0'

	def _getTradeDirection(self, data, r1, r2, c1, c2):
		# if Bid is flashing it's Hit
		tradeDir = ' ' 
		#askSizeVideo = DataCache.getVideo(self.page, int(r1), int(c1) + 26) # askSize 
		#bidSizeVideo = DataCache.getVideo(self.page, int(r1), int(c1) + 22) # bibSize
		# check Price
		askSizeVideo = DataCache.getVideo(self.page, int(r1), int(c1) + 15) # askSize 
		bidSizeVideo = DataCache.getVideo(self.page, int(r1), int(c1) + 8) # bibSize
		if (askSizeVideo & 0x01) == 0x01:
			tradeDir = 'T'
		if (bidSizeVideo & 0x01) == 0x01:
			tradeDir = 'H'
	
		return tradeDir	

	def _toLastTradePrice(self, bid, ask, tradeDir):
		# set trade_price in the begging of the trade and
		# clear out for complition
		(price, size, trade_time) = self.last_trade
		new_time = time.strftime('%x %X', time.localtime())
		if tradeDir == 'H' and price == '' :
			self.last_trade = (bid, size, new_time)
			return bid 
		elif tradeDir == 'T'and price == '':
			self.last_trade = (ask, size, new_time)
			return ask
		elif tradeDir == ' ':
			self.last_trade = ('', size, trade_time)
			return '0'  
		else:
			return price 

	def _toLastTradeSize(self, bid_size, ask_size, tradeDir):
		# clear out after the compliton of the trade
		(price, size, trade_time) = self.last_trade
		if tradeDir == 'H':
			self.last_trade = (price, bid_size, trade_time)
			return bid_size
		elif tradeDir == 'T':
			self.last_trade = (price, ask_size, trade_time)
			return ask_size
		else:
			self.last_trade = (price, '', trade_time)
			return '0' 

	def _toLastTradeTime(self):
		(price, size, trade_time) = self.last_trade
		if trade_time == '':
			return ' '
		return trade_time

########################################################################
class PrebonBill(GenBill):
	def __init__(self, name, page, label):
		GenBill.__init__(self, name, page, label)
		self.symbol = services.prebon + '.' + name + '.' + label 
		tmatch = re.search(r'(WI)', label)
		if tmatch:
			self.subtype2 = 'WI'

	def _toBid(self, data):
		if (re.search(r'[0-9]', data[4:6])):
			try:
				handle = int(data[1:3].strip())
				bid_fraction = data[4:6].strip()
				return str(handle) + '.' + bid_fraction 
			except ValueError, e:
				print 'P:Python::_toBid error ', e, data
				return '0'
		return '0'

	def _toAsk(self, data):
		if (re.search(r'[0-9]', data[8:10])):
			try:
				handle = int(data[1:3])
				bid_fraction = data[4:6].strip()
				ask_fraction = data[8:10].strip()
				if len(bid_fraction) and len(ask_fraction):
					if int(ask_fraction) > int(bid_fraction):
						handle = handle - 1
				return str(handle) + '.' + str(ask_fraction)
			except ValueError, e:
				print 'Python::_toAsk error ', e, data
				return '0'
                else:
                        return '0'

		if (re.search(r'[0-9]', data[8:10])):
			handle = data[1:3].strip()
			ask_fraction = data[8:10].strip()
			return handle + '.' + ask_fraction
		else:
			return '0'

	def _toBidSize(self, data):
		if (re.search(r'[0-9]', data[12:16])):
			return data[12:16].strip()
		else:
			return '0'

	def _toAskSize(self, data):
		if (re.search(r'[0-9]', data[17:21])):
			return data[17:21].strip()
		else:
			return '0'

	def _getTradeDirection(self, data, r1, r2, c1, c2):
		# if Bid is flashing it's Hit
		tradeDir = ' ' 
		#askSizeVideo = DataCache.getVideo(self.page, int(r1), int(c1) + 18) # askSize 
		#bidSizeVideo = DataCache.getVideo(self.page, int(r1), int(c1) + 14) # bibSize
		# check Price
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
		(price, size, trade_time) = self.last_trade
		new_time = time.strftime('%x %X', time.localtime())
		if tradeDir == 'H' and price == '' :
			self.last_trade = (bid, size, new_time)
			return bid 
		elif tradeDir == 'T'and price == '':
			self.last_trade = (ask, size, new_time)
			return ask
		elif tradeDir == ' ':
			self.last_trade = ('', size, trade_time)
			return '0'  
		else:
			return price 

	def _toLastTradeSize(self, bid_size, ask_size, tradeDir):
		# clear out after the compliton of the trade
		(price, size, trade_time) = self.last_trade
		if tradeDir == 'H':
			self.last_trade = (price, bid_size, trade_time)
			return bid_size
		elif tradeDir == 'T':
			self.last_trade = (price, ask_size, trade_time)
			return ask_size
		else:
			self.last_trade = (price, '', trade_time)
			return '0' 

	def _toLastTradeTime(self):
		(price, size, trade_time) = self.last_trade
		if trade_time == '':
			return ' '
		return trade_time

########################################################################

def likeShorcanBill(strlbl):
	strlbl = strlbl.strip()
	tmatch = re.search(r'([A-Z]{3})([0-9O]{2})', strlbl)
	if tmatch:
		s = tmatch.group(1) + re.sub('O' , '0', tmatch.group(2))
		try:
			mytyme = time.strptime(s, '%b%d')
			return s
		except ValueError, e:
			return None
	else:
		tmatch = re.search(r'(WI) (1|3|6)(Y|M)', strlbl)
		if tmatch:
			s = tmatch.group(1) + '_' + tmatch.group(2) + tmatch.group(3)
			return s
		else:
			return None


def likePrebonBill(strlbl):
	strlbl = strlbl.strip()
	tmatch = re.search(r'([A-Z]{3})([0-9O]{2})', strlbl)
	if tmatch:
		s = tmatch.group(1) + re.sub('O' , '0', tmatch.group(2))
		try:
			mytyme = time.strptime(s, '%b%d')
			return s
		except ValueError, e:
			return None
	else:
		tmatch = re.search(r'(WI)-(Y|3|6)(R|M)', strlbl)
		if tmatch:
			s = tmatch.group(1) + '_' + tmatch.group(2) + tmatch.group(3)
			if (tmatch.group(2) == 'Y' and tmatch.group(3) == 'R'):
				s = tmatch.group(1) + '_1Y'
			return s
		else:
			return None


