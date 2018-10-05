#! /usr/local/bin/python
# load file in format "CUSIP	COUPON	MAT_DATE"
# usage : loadTranslator CANPX_GOC BondTable.txt

import os


dblist=[]
reclist=[]

def toMat (mat):
	# convert from 12/1/2021 to 1221, 6/1/2015 to 0615
	ls = mat.split('/')
	mnt = ''
	if len(ls[0]) == 1:
		mnt = "0" + ls[0]
	else:
		mnt = ls[0]
	newMat = mnt + ls[2][2:]
	return newMat	

def toCoupon( cpn ):
	#return as 725_0607 format
	cpn = cpn.replace(".", "")
	l_cpnNew=[]
	l_cpn = list(cpn)
	size = 3
	if len(cpn) > size:
		size = len(cpn)
	for i in range(0, size):
		l_cpnNew.append("0")
	#print l_cpnNew
	for i in range(0, len(l_cpn)):
		l_cpnNew[i] = l_cpn[i]
	mcpn = ''
	for i in range(0, len(l_cpnNew)):
		mcpn = mcpn + (l_cpnNew[i])
	return mcpn


def buildDBList():
	global dblist
	file = os.sys.argv[2]
	f = open(file)
	s = f.read()
	sl = s.splitlines()	
	for l in sl:
		s = l.split('\t')
		if s[1] != "0":	
			dblist.append(s)
	f.close()

def buildRecList():
	global dblist
	global reclist
	pref = os.sys.argv[1]
	for i in dblist:
		recid = pref + "." + toCoupon(i[1]) + "_" + toMat(i[2]) 
		reclist.append((i[0], recid))
		
def buildLdifFile():
	global reclist
	f = open("canpx2gocTranslator.ldif", 'w')
	for l in reclist:
		f.write("dn: item=%s, producer=canpx2goc, ou=Translator, o=moneyline.ca\n" % (l[0]))
		f.write("item: %s\n" % (l[0]))
		f.write("objectClass: top\n")
		f.write("objectClass: chainItem\n")
		f.write("chainRec: %s\n" % (l[1]) )
		f.write("\n")	
	f.close()
	

if __name__ == '__main__':
	global reclist
	buildDBList()
	buildRecList()
	print reclist
	buildLdifFile()
