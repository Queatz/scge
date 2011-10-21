import scge

# Key Handler

REPEAT_DELAY = 1/2
REPEAT_TIME = 1/32
PRESSED_TIME = 0
LAST_TIME = 0
PRESSED = {
	'@ left':0,
	'@ middle':0,
	'@ right':0,
}
BLOCKALLEXCEPT = False

def set_repeat(d = 1/2, t = 1/32):
	global REPEAT_DELAY, REPEAT_TIME
	REPEAT_DELAY, REPEAT_TIME = d, t
	
def assign(*a):
	for k in a:
		if k not in PRESSED: PRESSED[k] = 0

def unassign(*a):
	for k in a:
		if k in PRESSED: del PRESSED[k]
	
def pressed(k, repeat = False):
	if BLOCKALLEXCEPT and k not in BLOCKALLEXCEPT: return False
	if k not in PRESSED: PRESSED[k] = 0
	return PRESSED[k] == 1 or repeat and PRESSED[k] == 3

def released(k):
	if BLOCKALLEXCEPT and k not in BLOCKALLEXCEPT: return False
	if k not in PRESSED: PRESSED[k] = 0
	return PRESSED[k] == -1

def down(k):
	if BLOCKALLEXCEPT and k not in BLOCKALLEXCEPT: return False
	if k not in PRESSED: PRESSED[k] = 0
	return PRESSED[k] > 0

def block(k):
	if k not in PRESSED: PRESSED[k] = 0
	PRESSED[k] = 4

def blockall(*a):
	global BLOCKALLEXCEPT
	if len(a) < 1:
		a = [True]
	BLOCKALLEXCEPT = a
	

def watch():
	global PRESSED_TIME, REPEAT_DELAY, REPEAT_TIME, LAST_TIME, PRESSED, BLOCKALLEXCEPT
	
	BLOCKALLEXCEPT = False
	
	t = scge.window_timer()
	
	for i in PRESSED:
		if PRESSED[i] == -1:
			PRESSED[i] = 0
		elif PRESSED[i] == 1 or PRESSED[i] == 3:
			PRESSED[i] = 2
		
		if (scge.key(i) if i[0:2] != '@ ' else scge.button(i[2:])):
			if PRESSED[i] == 0:
				PRESSED[i] = 1
				PRESSED_TIME = t
				LAST_TIME = t
			elif PRESSED[i] == 2 and (t > PRESSED_TIME+REPEAT_DELAY and t > LAST_TIME+REPEAT_TIME):
				PRESSED[i] = 3
				LAST_TIME = t
		else:
			if PRESSED[i] > 0:
				PRESSED[i] = -1
