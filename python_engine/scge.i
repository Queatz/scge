/* File : scge.i */
%module scge

//%include "std_string.i"

%{
#include "../engine/scge.hpp"
%}

//%apply const std::string& {std::string* foo};

%rename(__add__) offset::operator+;
%rename(__sub__) offset::operator-;

%include "../engine/scge.hpp"

