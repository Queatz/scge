/* File : scge.i */
%module scge

%include "std_string.i"

%{
#define SWIG_FILE_WITH_INIT
#include "../engine/scge.hpp"
%}

//%apply const std::string& {std::string* foo};

// Will somehow use GLM in the future
%rename(__add__) offset::operator+;
%rename(__sub__) offset::operator-;
%rename(__eq__) offset::operator==;

%include "scge.hpp"

// Python Only

%{
	#include "../engine/python.cpp"
%}

%include "../engine/python.h"
