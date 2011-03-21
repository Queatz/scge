box::box(float a, float b) {
	w = a;
	h = b;
}

ibox::ibox(int a, int b) {
	w = a;
	h = b;
}

offset::offset(float a, float b) {
	x = a;
	y = b;
}

rect::rect(float a, float b, float c, float d) {
	x = a;
	y = b;
	w = c;
	h = d;
}

irect::irect(int a, int b, int c, int d) {
	x = a;
	y = b;
	w = c;
	h = d;
}

crop::crop(bool a, float b, float c, float d, float e) {
	use = a;
	x = b;
	y = c;
	w = d;
	h = e;
}


icrop::icrop(bool a, int b, int c, int d, int e) {
	use = a;
	x = b;
	y = c;
	w = d;
	h = e;
}

rgba::rgba(float x, float y, float z, float w) {
	r = x;
	g = y;
	b = z;
	a = w;
}
