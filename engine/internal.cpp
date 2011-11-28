#include "functions.cpp"

int width = 0, height = 0, glfw_state = 0;
bool fullscreened;

void _size_callback_default(GLFWwindow w, int x, int y) {
	width = x;
	height = y;
}

char key_int_to_char(int a) {
	if(a < 256) return (char) a;
	return NULL;
}

const char* int_to_button_name(int a) {
	if(a == GLFW_MOUSE_BUTTON_LEFT)
		return "left";
	if(a == GLFW_MOUSE_BUTTON_MIDDLE)
		return "middle";
	if(a == GLFW_MOUSE_BUTTON_RIGHT)
		return "right";
	return "";
}

const char* int_to_key_name(int a) {
	switch(a) {
	case 'A': return "a";
	case 'B': return "b";
	case 'C': return "c";
	case 'D': return "d";
	case 'E': return "e";
	case 'F': return "f";
	case 'G': return "g";
	case 'H': return "h";
	case 'I': return "i";
	case 'J': return "j";
	case 'K': return "k";
	case 'L': return "l";
	case 'M': return "m";
	case 'N': return "n";
	case 'O': return "o";
	case 'P': return "p";
	case 'Q': return "q";
	case 'R': return "r";
	case 'S': return "s";
	case 'T': return "t";
	case 'U': return "u";
	case 'V': return "v";
	case 'W': return "w";
	case 'X': return "x";
	case 'Y': return "y";
	case 'Z': return "z";
	case '1': return "1";
	case '2': return "2";
	case '3': return "3";
	case '4': return "4";
	case '5': return "5";
	case '6': return "6";
	case '7': return "7";
	case '8': return "8";
	case '9': return "9";
	case '0': return "0";
	case '-': return "-";
	case '=': return "=";
	case '[': return "[";
	case ']': return "]";
	case '\\': return "\\";
	case ';': return ";";
	case '\'': return "'";
	case ',': return ",";
	case '.': return ".";
	case '/': return "/";
	case '`': return "`";
	
	case GLFW_KEY_SPACE: return "space";
	case GLFW_KEY_ESC: return "escape";
	case GLFW_KEY_F1: return "f1";
	case GLFW_KEY_F2: return "f2";
	case GLFW_KEY_F3: return "f3";
	case GLFW_KEY_F4: return "f4";
	case GLFW_KEY_F5: return "f5";
	case GLFW_KEY_F6: return "f6";
	case GLFW_KEY_F7: return "f7";
	case GLFW_KEY_F8: return "f8";
	case GLFW_KEY_F9: return "f9";
	case GLFW_KEY_F10: return "f10";
	case GLFW_KEY_F11: return "f11";
	case GLFW_KEY_F12: return "f12";
	case GLFW_KEY_UP: return "up";
	case GLFW_KEY_DOWN: return "down";
	case GLFW_KEY_LEFT: return "left";
	case GLFW_KEY_RIGHT: return "right";
	case GLFW_KEY_LSHIFT: return "left shift";
	case GLFW_KEY_RSHIFT: return "right shift";
	case GLFW_KEY_LCTRL: return "left ctrl";
	case GLFW_KEY_RCTRL: return "right ctrl";
	case GLFW_KEY_LALT: return "left alt";
	case GLFW_KEY_RALT: return "right alt";
	case GLFW_KEY_TAB: return "tab";
	case GLFW_KEY_ENTER: return "enter";
	case GLFW_KEY_BACKSPACE: return "backspace";
	case GLFW_KEY_INSERT: return "insert";
	case GLFW_KEY_DEL: return "delete";
	case GLFW_KEY_PAGEUP: return "page up";
	case GLFW_KEY_PAGEDOWN: return "page down";
	case GLFW_KEY_HOME: return "home";
	case GLFW_KEY_END: return "end";
	case GLFW_KEY_KP_1: return "kp 1";
	case GLFW_KEY_KP_2: return "kp 2";
	case GLFW_KEY_KP_3: return "kp 3";
	case GLFW_KEY_KP_4: return "kp 4";
	case GLFW_KEY_KP_5: return "kp 5";
	case GLFW_KEY_KP_6: return "kp 6";
	case GLFW_KEY_KP_7: return "kp 7";
	case GLFW_KEY_KP_8: return "kp 8";
	case GLFW_KEY_KP_9: return "kp 9";
	case GLFW_KEY_KP_0: return "kp 0";
	case GLFW_KEY_KP_DIVIDE: return "kp divide";
	case GLFW_KEY_KP_MULTIPLY: return "kp multiply";
	case GLFW_KEY_KP_SUBTRACT: return "kp subtract";
	case GLFW_KEY_KP_ADD: return "kp add";
	case GLFW_KEY_KP_DECIMAL: return "kp decimal";
	case GLFW_KEY_KP_EQUAL: return "kp equal";
	case GLFW_KEY_KP_ENTER: return "kp enter";
	case GLFW_KEY_KP_NUM_LOCK: return "num lock";
	case GLFW_KEY_CAPS_LOCK: return "caps lock";
	case GLFW_KEY_SCROLL_LOCK: return "scroll lock";
	case GLFW_KEY_PAUSE: return "pause";
	case GLFW_KEY_LSUPER: return "right super";
	case GLFW_KEY_RSUPER: return "left super";
	case GLFW_KEY_MENU: return "menu";
	}
	return "unknown";
}
