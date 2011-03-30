/* * Types
* */
/* *
box
A box, containing a width and height.
	w float
		the width of the box
	h float
		the height of the box

C++
box a();
a.w = 12.0;
a.h = 12.0;

box b(12.0, 12.0);

Python
a = box()
a.w = 12.0
a.h = 12.0

b = box(12.0, 12.0)
* */
box::box(float a, float b) {
	w = a;
	h = b;
}

/* *
ibox
An integer struct, containing a width and height.
	w int
		the width component
	h int
		the height component

C++
ibox a();
a.w = 12;
a.h = 12;

ibox b(12, 12);

Python
a = ibox()
a.w = 12
a.h = 12

b = ibox(12, 12)
* */
ibox::ibox(int a, int b) {
	w = a;
	h = b;
}

/* *
offset
A 2D offset.

	x float
		the x offset
	y float
		the y offset

C++
offset a();
a.x = 12.0;
a.y = 12.0;

offset b(12.0, 12.0);

Python
a = offset()
a.x = 12.0
a.y = 12.0

b = offset(12.0, 12.0)
* */
offset::offset(float a, float b) {
	x = a;
	y = b;
}

offset offset::operator+(const offset& other) const {
	return offset(x + other.x, y + other.y);
}

offset offset::operator-(const offset& other) const {
	return offset(x - other.x, y - other.y);
}

/* *
rect
A rect, with offset and dimention.

	x float
		the x offset
	y float
		the y offset
	w float
		the width
	h float
		the height

C++
rect a();
a.x = 12.0;
a.y = 32.0;
a.w = 12.0;
a.h = 32.0;

rect b(12.0, 32.0, 12.0, 32.0);

Python
a = rect()
a.x = 12.0
a.y = 32.0
a.w = 12.0
a.h = 32.0

b = rect(12.0, 32.0, 12.0, 32.0)
* */
rect::rect(float a, float b, float c, float d) {
	x = a;
	y = b;
	w = c;
	h = d;
}


/* *
irect
An integer rectangle.

	x int
		the x offset
	y int
		the y offset
	w int
		the width
	h int
		the height

C++
irect a();
a.x = 12;
a.y = 32;
a.w = 12;
a.h = 32;

irect b(12, 32, 12, 32);

Python
a = irect()
a.x = 12
a.y = 32
a.w = 12
a.h = 32

b = irect(12, 32, 12, 32)
* */
irect::irect(int a, int b, int c, int d) {
	x = a;
	y = b;
	w = c;
	h = d;
}

/* *
crop
A crop, with an enabled option and a rect.

	use bool
		whether or not the crop is enabled
	x float
		the x offset
	y float
		the y offset
	w float
		the width
	h float
		the height

C++
crop a();
a.use = true;
a.x = 12.0;
a.y = 32.0;
a.w = 12.0;
a.h = 32.0;

crop b(true, 12.0, 32.0, 12.0, 32.0);

Python
a = crop()
a.use = True
a.x = 12.0
a.y = 32.0
a.w = 12.0
a.h = 32.0

b = crop(True, 12.0, 32.0, 12.0, 32.0)
* */
crop::crop(bool a, float b, float c, float d, float e) {
	use = a;
	x = b;
	y = c;
	w = d;
	h = e;
}

/* *
icrop
An integer crop, with an enabled option and a rect.

	use bool
		whether or not the crop is enabled
	x int
		the x offset
	y int
		the y offset
	w int
		the width
	h int
		the height

C++
icrop a();
a.use = true;
a.x = 12;
a.y = 32;
a.w = 12;
a.h = 32;

icrop b(true, 12, 32, 12, 32);

Python
a = icrop()
a.use = True
a.x = 12
a.y = 32
a.w = 12
a.h = 32

b = icrop(True, 12, 32, 12, 32)
* */
icrop::icrop(bool a, int b, int c, int d, int e) {
	use = a;
	x = b;
	y = c;
	w = d;
	h = e;
}

/* *
rgba
A color, with alpha.

	r float
		the red amount
	g float
		the green component
	b float
		the blue factor
	a float
		the alpha value

C++
rgba a();
a.r = 1.0;
a.g = 0.8;
a.b = 0.6;
a.a = 0.4;

rgba b(1.0, 0.8, 0.6, 0.4);
rgba c(1.0, 0.8, 0.6); //alpha is 1.0

Python
a = rgba()
a.r = 1
a.g = .8
a.b = .6
a.a = .4

b = rgba(1, .8, .6, .4)
c = rgba(1, .8, .6) #alpha is 1.0
* */
rgba::rgba(float w, float x, float y, float z) {
	r = w;
	g = x;
	b = y;
	a = z;
}
