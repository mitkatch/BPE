#! /usr/local/bin/python
import sys
import DataCache
import re
import string
import time
import os.path
import copy

######################################################################
class Inst:
	def __init__(self):
		self.type = 'INST'
		self.symbol = ' '
		self.name = ' ' 
		self.page = ' ' 
		self.coupon = ' '
		self.desc = ' '
		self.issuer = ' '
		self.currency = 'CAD'
		self.country = 'CA'
		self.seq = 0
		self.startup = time.strftime('%Y%m%d%H%M%S', time.localtime())
		self.subtype = ' '
		self.subtype1 = ' '
		self.subtype2 = ' '
		self.mat = ' '
		self.matDD = ' '
		self.matMM = ' '
		self.matYYYY = ' '
		self.bidChg = 0;
		self.old_bid = 0;
		self.bidYeldChg = 0;
		self.old_bid_yield = 0;
		self.bid_time = ' '
		self.askChg = 0;
		self.old_ask = 0;
		self.askYeldChg = 0;
		self.old_ask_yield = 0;
		self.ask_time = ' '

	def _hit(self, bid, tradeDir):
		if (tradeDir == 'H'):
			return '0'
		else:
			return bid

	def _take(self, ask, tradeDir):
		if (tradeDir == 'T'):
			return '0'
		else:
			return ask

	def _toBid(self, data):
		return '0'

	def _toAsk(self, data):
		return '0'


	def _toBidSize(self, data):
		return '0'

	def _toAskSize(self, data):
		return '0'

	def _toAskYield(self, data):
		return '0'

	def _toBidYield(self, data):
		return '0'

	def _toBMark(self, data):
		return  ' '

	def _getTradeDirection(self, data, r1, r2, c1, c2):
		return ' '

	def _toLastTradePrice(self, bid, ask, tradeDir):
		return '0'

	def _toLastTradeSize(self, bid_size, ask_size, tradeDir):
		return '0'

	def _toLastTradeYield(self, bid_yield, ask_yield, tradeDir):
		return '0'

	def _toLastTradeTime(self):
		return ' '

	def _getSeqOut(self):
		self.seq = self.seq + 1;
		return self.startup + '.' + self.symbol + '.' + str(self.seq)	

	def _bidChg(self, bid):
		try:
			fbid = float(bid) 
			if (fbid and self.old_bid == 0):
				self.bidChg = 0;
				self.old_bid = fbid 
			elif (fbid and self.old_bid ):
				if (fbid != self.old_bid):
					self.bidChg = fbid - self.old_bid
					self.old_bid = fbid 
			else:
				self.bidChg = 0 
				self.old_bid = 0 
		except ValueError, e:
			print 'Python::_bidChg error ', e, bid 
		#print 'Python::_bidChg return ', self.bidChg
		return self.bidChg 

	def _bidYieldChg(self, bid_yield):
		try:
			fbid_yield = float(bid_yield) 
			if (fbid_yield and self.old_bid_yield == 0):
				self.bidChgYield = 0;
				self.old_bid_yield = fbid_yield 
			elif (fbid_yield and self.old_bid_yield ):
				if (fbid_yield != self.old_bid_yield):
					self.bidChgYield = fbid_yield - self.old_bid_yield
					self.old_bid_yield = fbid_yield 
			else:
				self.bidChgYield = 0 
				self.old_bid_yield = 0 
		except ValueError, e:
			print 'Python::_bidChgYield error ', e, bid_yield 
		return self.bidChgYield 

	def _askChg(self, ask):
		try:
			fask = float(ask) 
			if (fask and self.old_ask == 0):
				self.askChg = 0;
				self.old_ask = fask 
			elif (fask and self.old_ask ):
				if (fask != self.old_ask):
					self.askChg = fask - self.old_ask
					self.old_ask = fask 
			else:
				self.askChg = 0 
				self.old_ask = 0 
		except ValueError, e:
			print 'Python::_askChg error ', e, ask 
		#print 'Python::_askChg return ', self.askChg
		return self.askChg 

	def _askYieldChg(self, ask_yield):
		try:
			fask_yield = float(ask_yield) 
			if (fask_yield and self.old_ask_yield == 0):
				self.askChgYield = 0;
				self.old_ask_yield = fask_yield 
			elif (fask_yield and self.old_ask_yield ):
				if (fask_yield != self.old_ask_yield):
					self.askChgYield = fask_yield - self.old_ask_yield
					self.old_ask_yield = fask_yield 
			else:
				self.askChgYield = 0 
				self.old_ask_yield = 0 
		except ValueError, e:
			print 'Python::_askChgYield error ', e, ask_yield 
		return self.askChgYield 



	def publish(self, data, r1, r2, c1, c2):
		data = re.sub('O', '0', data)
		try:
			data = re.sub('O', '0', data)

			tradeDir = self._getTradeDirection(data, r1, r2, c1, c2)
	
			bid = self._toBid(data)
			ask = self._toAsk(data)
			bid_size = self._toBidSize(data)
			ask_size = self._toAskSize(data)
			bid_yield = self._toBidYield(data) 
			ask_yield = self._toAskYield(data)
			bmark = self._toBMark(data)

			inst = {'ML_GRAPH_ADDRESS' : self.symbol,
				#'SYMBOL'	: self.symbol,
				'SECUR_NAME'	: self.symbol,
				###'INST_CODE'	: self.symbol,
				#'DESC'		: self.desc,
				'DSPLY_NAME'	: self.desc,
				'ISSUER'	: self.issuer,
				###'CURRENCY'	: self.currency,
				###'COUNTRY'	: self.country,
				#'SEQ_OUT'	: self._getSeqOut(), 
				'WEB_ADRS'	: self._getSeqOut(), 
				'INSTR_TYPE' 	: self.type,
				'INSTR_SUBTYPE'	: self.subtype,
				'INSTR_SUBTYPE_1': self.subtype1,
				'INSTR_SUBTYPE_2': self.subtype2,
				###'ML_MAPPED' :self.name,
				'ROW66_23' :self.name,
				###'BENCHMARK'    : bmark,
				'ROW66_2'    : bmark,
				###'ML_LABEL_SLOT' : str(self.label_slot),
				###'ML_DATA_SLOT' : str(self.page + ':' + str(self.data_slot)),
				###'ML_RAW_DATA': data,
				'ROW64_11': data,
				###'CPN'	: self.coupon,
				###'MATURITY'	: self.mat,
				###'MATURITY_DD'	: self.matDD,
				###'MATURITY_MM'	: self.matMM,
				###'MATURITY_YYYY'	: self.matYYYY,
				'BID'   : float(self._hit(bid, tradeDir)),
				'BIDSIZE' : float(self._hit(bid_size, tradeDir)),
				'BID_TIME'	: self.bid_time,
				###'BID_CHG'	: float(self._bidChg(bid)),

				'BID_YIELD' : float(self._hit(bid_yield, tradeDir)), 
				###'BID_YIELD_CHG' : float(self._bidYieldChg(bid_yield)),
				'ASK'	: float(self._take(ask, tradeDir)),
				'ASK_TIME'	: self.ask_time,
				###'ASK_CHG'	: float(self._askChg(ask)),
				'ASKSIZE' : float(self._take(ask_size, tradeDir)),
				'ASK_YIELD' : float(self._take(ask_yield, tradeDir)),
				###'ASK_YIELD_CHG' : float(self._askYieldChg(ask_yield)),
				#'TRADE_TONE' : str(tradeDir),
				'ROW66_20' : str(tradeDir),
				#'TRADE_PRICE' : float(self._toLastTradePrice(bid, ask, tradeDir)),
				'ROW66_15' : float(self._toLastTradePrice(bid, ask, tradeDir)),
				#'TRADE_SIZE' : float(self._toLastTradeSize(bid_size, ask_size, tradeDir)),
				'ROW66_16' : float(self._toLastTradeSize(bid_size, ask_size, tradeDir)),
				#'TRADE_YIELD' : float(self._toLastTradeYield(bid_yield, ask_yield, tradeDir)),
				'ROW66_21' : float(self._toLastTradeYield(bid_yield, ask_yield, tradeDir)),
				#'TRADE_TIME' : str(self._toLastTradeTime()),
				'ROW66_18' : str(self._toLastTradeTime()),
				##'TIMESTAMP'	: time.strftime('%x %X', time.localtime())
				'ROW64_12'	: time.strftime('%x %X', time.localtime())
			}
			#print 'Python::ShorcanBond::publish ', inst
			status = DataCache.updateInstrument(inst, self.symbol)
			return (str(status), )
		except Exception, e:
			print 'Python::Inst::publish error ', e, data 
			return ('0',)

######################################################################

