#!/bin/python3

TEST_RANGE = 255
TEST_BEGINS = [0x00, 0x800, 0xD700, 0xD800, 0xD900, 0xDC00, 0xDE00, 0xDF00, 0xE000, 0x10000, 0x10FF00]

def GetArrayStr(s, width = 4):
	res = '{'
	for i in s:
		res += hex(i) + ', '
	if len(s) < width:
		for i in range(len(s), width):
			res += hex(0) + ', '
	res += '}'
	return res

with open('./src/Utf16Map.hpp', 'w') as outFile:
	outFile.write('#pragma once\n')
	outFile.write('\n')

	outFile.write('const char32_t gk_Utf16MapBegins[] =\n')
	outFile.write('{\n')
	for item in TEST_BEGINS:
		outFile.write('\t{},\n'.format(item))
	outFile.write('};\n\n')

	outFile.write('const unsigned char gk_Utf16Map[][{}][4] =\n'.format(TEST_RANGE))
	outFile.write('{\n')
	for item in TEST_BEGINS:
		outFile.write('\t{\n')
		for i in range(item, item + TEST_RANGE):
			v = i.to_bytes(4, byteorder='big')
			s = '{}'
			try:
				c = v.decode('utf-32be').encode('utf-16le')
				s = GetArrayStr(c)
			except:
				pass
			outFile.write('\t\t')
			outFile.write(s)
			outFile.write(',')
			outFile.write('  //')
			outFile.write(str(hex(i)))
			if i == 0xD800:
				outFile.write(' #1 Pt')
			if i == 0xDC00:
				outFile.write(' #2 Pt')
			if i == 0x10000:
				outFile.write(' #3 Pt')
			outFile.write('\n')
		outFile.write('\t},\n')
	outFile.write('};\n\n')
