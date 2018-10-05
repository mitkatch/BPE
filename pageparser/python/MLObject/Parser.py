
#########################################################
# Copyright (c) 2004 MoneyLine Network Inc. (Canada)
#
# AUTHOR:              $Author: mikhail $
# DATE:                $Date: 2006/06/19 21:33:01 $
# SOURCE:              $Source: /repository/projects/Canpx/pageparser/python/MLObject/Parser.py,v $
# STATE:               $State: Exp $
# ID:                  $Id: Parser.py,v 1.7 2006/06/19 21:33:01 mikhail Exp $
# REVISION:    $Revision: 1.7 $
# LOG:                 $Log: Parser.py,v $
# LOG:                 Revision 1.7  2006/06/19 21:33:01  mikhail
# LOG:                 cannot read the set of fileds from the parser
# LOG:
# LOG:                 Revision 1.6  2006/04/20 18:55:02  nickz
# LOG:                 Add 'source' reference for python calling
# LOG:
# LOG:                 Revision 1.5  2004/12/15 20:15:07  tomcat
# LOG:                 Prov's update
# LOG:
# LOG:                 Revision 1.4  2004/12/10 16:48:59  mikhailt
# LOG:                 handle Prov labels chamges
# LOG:
# LOG:                 Revision 1.3  2004/12/06 16:09:36  mikhailt
# LOG:                 isEqual Prov
# LOG:
# LOG:                 Revision 1.2  2004/11/30 21:27:33  tomcat
# LOG:                 typo fixed
# LOG:
# LOG:                 Revision 1.1  2004/11/29 15:11:24  mikhailt
# LOG:                 Initial revision
# LOG:
# LOG:                 Revision 1.8  2004/11/24 22:25:05  mikhailt
# LOG:                 splited to MLObject
# LOG:
##########################################################

import sys
sys.path.append('./python/MLObject')
import DataCache 
import re 
import string
import time
import os.path
import copy
import Bond 
import Bill 
import Prov

########################################################################
class ParserMap:
	def __init__(self, name, pagemap):
		self.pagemap = copy.deepcopy(pagemap)
		self.name = name
		self.my_len = 0
		self.parser_map = {}
		self.labels = {}
		self.last_trade = {}
		k = pagemap.keys()
		for i in range(len(k)):
			self.my_len += len(k[i])		
			for j in range(len(k[i])):
				self.parser_map[k[i][j]] = pagemap[k[i]][j]
		#print 'ParserMap:', self.parser_map

	def _setTuple(self, r1, r2, c1, c2):
		my_list = []
		my_list.append(int(r1))
		my_list.append(int(r2))
		my_list.append(int(c1))
		my_list.append(int(c2))
		return tuple(my_list)

	
	def _addInst(self, source, name, page, data, r1, r2, c1, c2): 
		#source = __name__
		strlbl = eval('Bond.like' + source +'Bond(data)')
		# call Bond.likeShorcanBond(data)
		status = 0
		refresh = 0
		label_slot = self._setTuple(r1, r2, c1, c2)
		if strlbl:
			bond = eval('Bond.'+source+'Bond(name, page, strlbl)')

			status = DataCache.addInstrument(bond.symbol)
			if status == 0:
				print 'Python::_addInst duplication found ', bond.symbol
				bondFound = self.labels.get(label_slot, None)
				if bondFound:
					if bond.symbol == bondFound.symbol:
						bond = bondFound
						print 'Run Python::_addInst refresh for Bond ', bond.symbol, data 
						refresh = 1
						# just a refresh

			if refresh == 0:
				bond.label_slot = label_slot
				bond.data_slot = self._getDataSlotByLabel(label_slot)
				print 'Run Python::_addInst Bond ', bond.symbol, r1, r2, c1, c2 
				self.labels[label_slot] = bond 
		else:
			strlbl = eval('Bill.like' + source + 'Bill(data)')
			if strlbl:
				bill = eval('Bill.'+source+'Bill(name, page, strlbl)')
				print 'Run Python::_addInst Bill ', bill.symbol, r1, r2, c1, c2 
				status = DataCache.addInstrument(bill.symbol)
	
				if status == 0:
					print 'Python::_addInst duplication found ', bill.symbol
					billFound = self.labels.get(label_slot, None)
					if billFound:
						if bill.symbol == billFound.symbol:
							bill = billFound
							print 'Run Python::_addInst refresh for Bill ', bill.symbol, data 
							refresh = 1
							# just a refresh

				if refresh == 0:
					bill.label_slot = label_slot
					bill.data_slot = self._getDataSlotByLabel(label_slot)

					self.labels[label_slot] = bill 
			else:
				strlbl = eval('Prov.like' + source +'Prov(data)')
				if strlbl:
					prov = eval('Prov.' + source +'Prov(name, page, strlbl)')
					prov.setSymbol(label_slot)
					prov.data_slot = self._getDataSlotByLabel(label_slot)
					print 'Run Python::_addInst Prov ', prov.symbol, r1, r2, c1, c2 
					status = DataCache.addInstrument(prov.symbol)
					if status == 0:
						print 'Python::_addInst duplication found ', prov.symbol
						provFound = self.labels.get(label_slot, None)
						if provFound:
							if (not prov.isEqual(provFound)):
								provFound.copy(prov)
								#inst changed 
						else:
							self.labels[label_slot] = prov 
							#it has been deleted
					else:
						self.labels[label_slot] = prov 
				else:
					print 'Run Python::_addInst i dont know what it is ', data, r1, r2, c1, c2 
					inst = self.labels.get(label_slot, None)
					if (inst):
						del self.labels[label_slot]
						print 'Run Python::_addInst remove from labels', label_slot, '[', inst.symbol, ']' 
						

		return status	
		

	def _getInst(self, r1, r2, c1, c2):
		my_slot = self._setTuple(r1, r2, c1, c2)
		label_slot = self._getLabelSlotByData(my_slot)
		return self.labels.get(label_slot, None)


	def _getLabelByData(self, slot):
		found_label = 'NOT_FOUND'
		#print "getLabel for slot ", slot
		k = self.pagemap.keys()
		for i in range(len(k)):	
			if k[i][1] == slot:
				found_label = self.labels.get(k[i][0], "failed_to_get_label")
				#print 'Found label =', found_label
	
		return found_label

	def _getLabelSlotByData(self, slot):
		label_slot =() 
		k = self.pagemap.keys()
		for i in range(len(k)):	
			if k[i][1] == slot:
				label_slot = k[i][0] 
				#print 'Found label_slot =', label_slot
		return label_slot

	def _getDataSlotByLabel(self, slot):
		data_slot =() 
		k = self.pagemap.keys()
		for i in range(len(k)):	
			if k[i][0] == slot:
				data_slot = k[i][1] 
				print 'Found data_slot =', data_slot
		return data_slot






#globals
# tuple is row1, row2, column1, column2
# parser is (label_slot, data_slot) : ( label_function, data_function)
parser = {}
pageParserMap = {}

# if we need pass some attr to html client define them here
html_attr = {
	"labelParser" : " bgcolor=#DAD510",
	"dataParser" : " bgcolor=#3797B3"
}

def pyLoadMyMap(map_file):
	#print "Python::pyLoadMyMap load ", map_file
	# load map to ParserClass and set global map to {}
	global parser
	global pageParserMap
	my_suffix = os.path.splitext(os.path.basename(map_file))[0]
	print 'Python::pyLoadMyMap my_suffix=', my_suffix
	execfile(map_file, globals())
	pm = ParserMap(my_suffix, parser)
	pageParserMap[my_suffix] = pm	
	parser = {}
	return (my_suffix,) 
	

def pyGetMySlotNum(name):
	global pageParserMap
	my_len = 0
	if pageParserMap.has_key(name):
		my_len = pageParserMap[name].my_len
	return (my_len,) 

def pyGetSlot(name, index):
	global pageParserMap
	print "Python::pyGetSlot for ", index, name 
	my_slot = ()
	try:
		i = int(index)
	except ValueError, e:
		print 'Python::pyGetSlot error ', e
		return my_slot

	if pageParserMap.has_key(name):
		k = pageParserMap[name].pagemap.keys()
		#print "K ", k
		if (i <= len(k) * 2):
			my_slot = k[i/2][i % 2]

	print "Python::pyGetSlot slot ", index, my_slot
	return my_slot


		
def pyGetParserBySlot(name, sr1, sr2, sc1, sc2):
	global html_attr
	global pageParserMap
	try:
		r1 = int(sr1)
		r2 = int(sr2)
		c1 = int(sc1)
		c2 = int(sc2)
	except ValueError, e:
		print 'Python::pyGetParserBySlot error ', e
		return (' ',' ')
	found_parser = ''
	if pageParserMap.has_key(name):
		found_parser = pageParserMap[name].parser_map.get( (r1, r2, c1, c2), "unknown_parser")
	html = html_attr.get(found_parser, ' ')	
	return (found_parser, html)
	



def labelParser(source, name, page, data, r1, r2, c1, c2):
	# parse label
	# add instrumnent to DataGraph
	global pageParserMap

	if pageParserMap.has_key(name):
		status = pageParserMap[name]._addInst(source, name, page, data, r1, r2, c1, c2)
		#print 'Python::Run status=', status
		return ( str(status), )
	else:
		return ('0',)


def dataParser(source, name, page, data, r1, r2, c1, c2):
	global pageParserMap
	if pageParserMap.has_key(name):
		pm = pageParserMap[name]	
		inst = pm._getInst(r1, r2, c1, c2)
		print 'Python::dataParser ', source, name, pm, inst
		if inst:
			status = inst.publish(data, r1, r2, c1, c2)
			return (str(status),)
		else:
			print 'Python::dataParser no Inst regestered for ', r1, r2, c1, c2 
			return ('0',)
	else:
		print 'Python::dataParser not found ', name
		return ('0',)

if __name__ == '__main__':
	#sys.path.append('')
	from Prov import *
	pyLoadMyMap('./shorcan_map/17648.py')
	labelParser('17648', 'MoneylineP', 'ON   6.1O/11 611', '8', '8', '0', '15')
	data = '     - 282      X 5    '
	dataParser('17648', 'MoneylineP', data , '8', '8', '16', '39')
	print 'Nothing'
