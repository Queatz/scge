import scge

# Key Handler

PRESSED = {}
SETUP = False
BLOCKALLEXCEPT = False
CALLBACK = None

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
			PRESSED[i] = 2
	else:
		PRESSED[i] = 0
	
	if CALLBACK:
		if isinstance(CALLBACK, tuple):
			if i[:6] == 'mouse ':
				CALLBACK[1](i[6:], PRESSED[i])
			else:
				CALLBACK[0](i, PRESSED[i])
		else:
			CALLBACK(i, PRESSED[i])

def _keycallback(k, s):
	_handle(k, s)

def _buttoncallback(b, s):
	_handle('mouse ' + b, s)

def events(self):
	for k in PRESSED:
		yield k, PRESSED[k]

def set_callback(f):
	"Callbacks accept 2 parameters: key (a string), state (0 released | 1 pressed | 2 repeated)"
	global CALLBACK
	CALLBACK = f

def set_callbacks(kf, bf):
	"Same as set_callback but mouse and keys are sent seperately and 'mouse ' is dropped from mouse events."
	global CALLBACK
	CALLBACK = kf, bf

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
		if PRESSED[i] < 1:
			dellist.append(i)
		elif PRESSED[i] == 1 or PRESSED[i] == 2:
			PRESSED[i] = 3
	for i in dellist:
		del PRESSED[i]
	if poll:
		scge.poll()
