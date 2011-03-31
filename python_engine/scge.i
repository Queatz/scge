/* File : scge.i */
%module scge

%{
#include "../engine/scge.hpp"
%}

%rename(__add__) offset::operator+;
%rename(__sub__) offset::operator-;

%include "../engine/scge.hpp"

