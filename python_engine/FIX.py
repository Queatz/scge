import re

f = open('scge.py', 'r+')
string = f.read()
f.seek(0)
f.truncate()
f.write(re.sub(r'arg0[^\)]*\)', '*args)', string, flags = re.DOTALL))
f.close()
