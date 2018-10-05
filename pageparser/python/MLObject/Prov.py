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

########################################################################
class GenProv(Inst.Inst):
	def __init__(self, name, page, label):
		Inst.Inst.__init__(self)
		self.name = name
		self.page = page
		self.last_trade = ('', '', '')
		self.label_slot = ()
		self.data_slot = ()
		self.type = 'FI'
		self.subtype = 'BOND'
		self.subtype1 = 'PROVINCIAL'
		self.issuer = ' '
		self.coupon = ''
		self.mat = ' '
		self.bmark = ' '
		self.symbol = ' '


	def isEqual(self, prov):
		if (self.issuer == prov.issuer):
			if (self.coupon == prov.coupon):
				if (self.mat == prov.mat):
					print 'Python::Prov::isEqual equal ', prov.issuer, prov.coupon, prov.mat, prov.bmark
					return 1
		print 'Python::Prov::isEqual not equal ', prov.issuer, prov.coupon, prov.mat, ' != ', self.issuer, self.coupon, self.mat 
		return None

	def setSymbol(self, label_slot):
		self.label_slot = label_slot
		label = str(label_slot[0]) + '_' +  str(label_slot[2])
		self.symbol = 'GENERIC.' + self.name + '.' + label 

	def _toBMark(self, data):
		return ' '

	def copy(self, prov):
		self.name = prov.name
		self.page = prov.page
		self.last_trade = prov.last_trade
		self.label_slot = prov.label_slot
		self.data_slot = prov.data_slot
		self.type = prov.type
		self.issuer = prov.issuer
		self.coupon = prov.coupon
		self.mat = prov.mat
		self.bmark = prov.bmark
		self.symbol = prov.symbol

	def couponToStr(self, cpn):
		t = re.search(r'([0-9]+)\.([0-9]+)', cpn)
		strcpn = '00000'
		try:
			if t:
				hnd, frk = t.groups()
				if len(hnd) == 1:
					hnd = '0' + hnd

				if len(frk) == 1:
					frk = frk + '00'
				elif len(frk) == 2:
					frk = frk + '0'

				strcpn = hnd + frk
		except ValueError, e:
			print "Python::couponToStr %s %s" %(e , cpn)

		return strcpn
		
		


########################################################################

class ShorcanProv(GenProv):
	def __init__(self, name, page, label):
		GenProv.__init__(self, name, page, label)
		t = re.search(r'([A-Z* ]+) ([0-9O]+\.[0-9O]+)/([0-9O]{2})', label)
		if t:
			self.issuer, self.coupon, self.mat = t.groups()
			self.issuer = self.issuer.strip()
			self.coupon = re.sub('O', '0', self.coupon.strip())
			self.matYY = re.sub('O', '0', self.mat.strip())
			self.mat = '20' + self.matYY 
			self.matYYYY = self.mat


	def setSymbol(self, label_slot):
		self.label_slot = label_slot
		#label = str(label_slot[0]) + '_' +  str(label_slot[2])
		# label will be like ON_06250_13
		label = self.issuer + '_' + GenProv.couponToStr(self, self.coupon) + '_' + self.matYY 
		self.symbol = services.shorcan + '.' + self.name + '.' + label 
		self.desc = label

	def _toBMark(self, data):
		if (re.search(r'[0-9DON]', data[1:4])):
			self.bmark = data[1:4]
			return self.bmark
		else:
			return ' '

	def _toBid(self, data):
		if (re.search(r'[0-9]', data[6:9])):
			handle = '0' 
			bid_fraction = data[6:9].strip()
			return handle + '.' + bid_fraction 
		return '0'

	def _toAsk(self, data):
		if (re.search(r'[0-9]', data[12:15])):
			handle = '0' 
			ask_fraction = data[12:15].strip()
			return handle + '.' + ask_fraction
		else:
			return '0'

	def _toBidSize(self, data):
		if (re.search(r'[0-9]', data[17:21])):
			return data[17:21].strip()
		else:
			return '0'

	def _toAskSize(self, data):
		if (re.search(r'[0-9]', data[22:26])):
			return data[22:26].strip()
		else:
			return '0'

	def _getTradeDirection(self, data, r1, r2, c1, c2):
		# if Bid is flashing it's Hit
		tradeDir = ' ' 
		#bidSizeVideo = DataCache.getVideo(self.page, int(r1), int(c1) + 19) # bibSize
		#askSizeVideo = DataCache.getVideo(self.page, int(r1), int(c1) + 23) # askSize 
		# check Price
		bidSizeVideo = DataCache.getVideo(self.page, int(r1), int(c1) + 6) # bibSize
		askSizeVideo = DataCache.getVideo(self.page, int(r1), int(c1) + 12) # askSize 
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

class PrebonProv(GenProv):
	def __init__(self, name, page, label):
		GenProv.__init__(self, name, page, label)
		t = re.search(r'([A-Z]+)([0-9]+\.[0-9O]+)/([0-9]{2})', label)
		if t:
			self.issuer, self.coupon, self.mat = t.groups()
			self.issuer = self.issuer.strip()
			self.coupon = re.sub('O', '0', self.coupon.strip())
			self.matYY = re.sub('O', '0', self.mat.strip())
			self.mat = '20' + self.matYY 
			self.matYYYY = self.mat


	def setSymbol(self, label_slot):
		self.label_slot = label_slot
		#label = str(label_slot[0]) + '_' +  str(label_slot[2])
		# label will be like ON_06250_13
		label = self.issuer + '_' + GenProv.couponToStr(self, self.coupon) + '_' + self.matYY 
		self.symbol = services.prebon + '.' + self.name + '.' + label 
		self.desc = label

	def _toBMark(self, data):
		if (re.search(r'[0-9DON/]', data[1:5])):
			self.bmark = re.sub('/', '', data[1:5])
			return self.bmark
		else:
			return ' '

	def _toBid(self, data):
		if (re.search(r'[0-9\.]', data[8:13])):
			handle = '0' 
			bid_fraction = data[8:13].strip()
			bid_fraction = re.sub('\.', '', bid_fraction) 
			return handle + '.' + bid_fraction 
		return '0'

	def _toAsk(self, data):
		if (re.search(r'[0-9\.]', data[14:19])):
			handle = '0' 
			ask_fraction = data[14:19].strip()
			ask_fraction = re.sub('\.', '', ask_fraction)
			return handle + '.' + ask_fraction
		else:
			return '0'

	def _toBidSize(self, data):
		if (re.search(r'[0-9]', data[21:24])):
			return data[21:24].strip()
		else:
			return '0'

	def _toAskSize(self, data):
		if (re.search(r'[0-9]', data[25:27])):
			return data[25:27].strip()
		else:
			return '0'

	def _getTradeDirection(self, data, r1, r2, c1, c2):
		# if Bid is flashing it's Hit
		tradeDir = ' ' 
		#bidSizeVideo = DataCache.getVideo(self.page, int(r1), int(c1) + 24) # bibSize
		#askSizeVideo = DataCache.getVideo(self.page, int(r1), int(c1) + 26) # askSize 
		#c check Price
		bidSizeVideo = DataCache.getVideo(self.page, int(r1), int(c1) + 12) # bibSize
		askSizeVideo = DataCache.getVideo(self.page, int(r1), int(c1) + 14) # askSize 
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

######################################################

def likeShorcanProv(strlbl):
	t = re.search(r'([A-Z* ]+) ([0-9O]+\.[0-9O]+)/([0-9O]{2})', strlbl)
	if t:
		return strlbl
	else:
		return None


def likePrebonProv(strlbl):
	t = re.search(r'([A-Z]+)([0-9]+\.[0-9O]+)/([0-9]{2})', strlbl)
	if t:
		return strlbl
	else:
		return None

