/* File : scge.i */
%module scge

%{
#include "../engine/engine.hpp"
%}

%rename(__add__) offset::operator+;
%rename(__sub__) offset::operator-;

%include "../engine/engine.hpp"

