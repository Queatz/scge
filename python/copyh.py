#! /usr/bin/env python3

h = open('../engine/conf.h', 'r')
i = open('../engine/scge.hpp', 'r')
o = open('scge.hpp', 'w')

for l in h:
	o.write(l)
o.write('\n')

for l in i:
	if len(l)<4 or l[-4:-1] != '//x':
		o.write(l)

h.close()
o.close()
i.close()
