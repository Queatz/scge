import scge

# Key Handler

PRESSED = {}
SETUP = False
BLOCKALLEXCEPT = False
CALLBACK = None

def _combine(k1, k2):
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
	if k == 'ctrl':
		return _combine('left ctrl', 'right ctrl')
	elif k == 'alt':
		return _combine('left alt', 'right alt')
	elif k == 'shift':
		return _combine('left shift', 'right shift')
	elif k == 'super':
		return _combine('left super', 'right super')
	else:
		if BLOCKALLEXCEPT and k not in BLOCKALLEXCEPT: return 0
		if k not in PRESSED: return 0
		return PRESSED[k]

def pressed(k, repeat = False):
	k = _ks(k)
	return k == 1 or repeat and k == 2

def released(k):
	return _ks(k) == -1

def down(k):
	return _ks(k) > 0

def block(k):
	global PRESSED
	if k not in PRESSED: return False
	PRESSED[k] = 4

def blockall(*a):
	global BLOCKALLEXCEPT
	if len(a) < 1:
		a = [True]
	BLOCKALLEXCEPT = a

def _handle(i, s):
	global PRESSED, CALLBACK
	if s == 1:
		if i not in PRESSED:
			PRESSED[i] = 1
		else:
			PRESSED[i] = 2
	else:
		PRESSED[i] = -1
	
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

def events():
	global PRESSED
	for k in PRESSED:
		if PRESSED[k] == -1:
			yield k, 'released'
		elif PRESSED[k] == 1:
			yield k, 'pressed'
		elif PRESSED[k] == 2:
			yield k, 'repeated'

def set_callback(f):
	"Callbacks accept 2 parameters: key (a string), state (0 released | 1 pressed | 2 repeated)"
	global CALLBACK
	CALLBACK = f

def set_callbacks(kf, bf):
	"Same as set_callback but mouse and keys are sent seperately and 'mouse ' is dropped from mouse events."
	global CALLBACK
	CALLBACK = kf, bf

def setup_callback(w):
	global SETUP
	w.callback('key', _keycallback)
	w.callback('button', _buttoncallback)
	w.set('key repeat')
	SETUP = True

def watch(w, poll = True):
	global SETUP, PRESSED, BLOCKALLEXCEPT
	if not SETUP:
		setup_callback(w)
	
	BLOCKALLEXCEPT = False
	
	for k in list(PRESSED.keys()):
		if PRESSED[k] == 0:
			del PRESSED[k]
	
	PRESSED = {k: 3 if v < 4 else v for k, v in PRESSED.items() if v > 0}
	
	if poll:
		scge.poll()
