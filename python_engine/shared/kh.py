import scge

# Key Handler

PRESSED = {}
SETUP = False
BLOCKALLEXCEPT = False

def combine(k1, k2):
	k1 = _ks(k1)
	k2 = _ks(k2)
	if k1 == 1 or k2 == 1:
		return 1
	elif k1 == 3 or k2 == 3:
		return 3
	elif k1 == -1 or k2 == -1:
		return -1
	elif k1 == 2 or k2 == 2:
		return 2
	else:
		return 0

def _ks(k):
	if BLOCKALLEXCEPT and k not in BLOCKALLEXCEPT: return 0
	if k not in PRESSED: return 0
	
	if k == 'ctrl':
		return _combile('left ctrl', 'right ctrl')
	elif k == 'alt':
		return _combile('left alt', 'right alt')
	elif k == 'shift':
		return _combile('left shift', 'right shift')
	elif k == 'super':
		return _combile('left super', 'right super')
	else:
		return PRESSED[k]

def pressed(k, repeat = False):
	k = _ks(k)
	return k == 1 or repeat and k == 3

def released(k):
	return _ks(k) == -1

def down(k):
	return _ks(k) > 0

def block(k):
	if k not in PRESSED: return False
	PRESSED[k] = 4

def blockall(*a):
	global BLOCKALLEXCEPT
	if len(a) < 1:
		a = [True]
	BLOCKALLEXCEPT = a

def _handle(i, s):
	global PRESSED
	if s == 1:
		if i not in PRESSED:
			PRESSED[i] = 1
		else:
			PRESSED[i] = 3
	else:
		PRESSED[i] = -1
	

def _keycallback(k, s):
	_handle(k, s)

def _buttoncallback(b, s):
	_handle('@ ' + b, s)

def watch(poll = True):
	global SETUP, PRESSED, BLOCKALLEXCEPT
	if not SETUP:
		scge.set_callback('key', _keycallback)
		scge.set_callback('button', _buttoncallback)
		scge.key_repeat(True)
		SETUP = True
	
	BLOCKALLEXCEPT = False
	PRESSED = {}
	
	dellist = []
	for i in PRESSED:
		if PRESSED[i] == -1:
			dellist.append(i)
		elif PRESSED[i] == 1 or PRESSED[i] == 3:
			PRESSED[i] = 2
	for i in dellist:
		del PRESSED[i]
	if poll:
		scge.poll()
