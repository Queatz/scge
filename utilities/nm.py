NUM = 2048

# Prints a nice C-style array of floats

def prt(n):
	n = round(n, 5)
	s = str(n)
	o = s.index('.')
	for i in range(5 - (len(s) - o)):
		s += '0'
	return s

import random
s = '{'
for i in range(NUM):
	s += prt(round(random.random(), 4))
	if i != NUM-1: s += ',' + ('\n' if i%12 == 11 else ' ')
s += '}'
print(s)
