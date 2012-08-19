/* File : scge.i */
%module scge

%include "std_string.i"

%{
#define SWIG_FILE_WITH_INIT
#include "../engine/conf.h"
#include "../engine/scge.hpp"

static
PyObject* GLMModule;

/*$ TYPEMAP $*/
static
PyObject* GLM${type}Type;
/*$ $*/
%}

%init %{
GLMModule = PyImport_ImportModule("glm");

/*$ TYPEMAP $*/
GLM${type}Type = PyObject_GetAttrString(GLMModule, "${type}");
/*$ $*/
%}
/*$ TYPEMAP $*/

%typemap(typecheck, precedence=SWIG_TYPECHECK_POINTER) glm::${type} {
	$1 = PyObject_IsInstance($input, GLM${type}Type) ? 1 : 0;
}

%typemap(in) glm::${type} {
	$1 = glm_${type}Data($input);
}

%typemap(out) glm::${type} {
	PyObject *args;
	args = Py_BuildValue("${(p if p else 'f') * c}"
/*$ {c} $*/
$?{base=='vec'
	, $1[${I}]
$??{base=='mat'
	, $1[${I//rows}][${I%rows}]
$?}
/*$ $*/
	);
	$result = PyObject_CallObject(GLM${type}Type, args);
	Py_DECREF(args);
}
/*$ $*/

%newobject scge::event::data;
%newobject scge::host::service;

%include "scge.hpp"
