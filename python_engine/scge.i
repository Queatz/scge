/* File : scge.i */
%module scge

%include "std_string.i"

%{
#define SWIG_FILE_WITH_INIT
#include "../engine/conf.h"
#include "../engine/scge.hpp"

PyObject* GLMModule = PyImport_ImportModule("glm");

PyObject* GLMvec2Type = PyObject_GetAttrString(GLMModule, "vec2");
PyObject* GLMvec3Type = PyObject_GetAttrString(GLMModule, "vec3");
PyObject* GLMvec4Type = PyObject_GetAttrString(GLMModule, "vec4");
PyObject* GLMivec2Type = PyObject_GetAttrString(GLMModule, "ivec2");
PyObject* GLMivec3Type = PyObject_GetAttrString(GLMModule, "ivec3");
PyObject* GLMivec4Type = PyObject_GetAttrString(GLMModule, "ivec4");
PyObject* GLMmat2Type = PyObject_GetAttrString(GLMModule, "mat2");
PyObject* GLMmat3Type = PyObject_GetAttrString(GLMModule, "mat3");
PyObject* GLMmat4Type = PyObject_GetAttrString(GLMModule, "mat4");
PyObject* GLMmat2x3Type = PyObject_GetAttrString(GLMModule, "mat2x3");
PyObject* GLMmat3x2Type = PyObject_GetAttrString(GLMModule, "mat3x2");
PyObject* GLMmat2x4Type = PyObject_GetAttrString(GLMModule, "mat2x4");
PyObject* GLMmat4x2Type = PyObject_GetAttrString(GLMModule, "mat4x2");
PyObject* GLMmat3x4Type = PyObject_GetAttrString(GLMModule, "mat3x4");
PyObject* GLMmat4x3Type = PyObject_GetAttrString(GLMModule, "mat4x3");
%}

%typemap(typecheck, precedence=SWIG_TYPECHECK_POINTER) glm::vec2 {
	$1 = PyObject_IsInstance($input, GLMvec2Type) ? 1 : 0;
}

%typemap(in) glm::vec2 {
	$1 = glm_vec2Data($input);
}

%typemap(out) glm::vec2 {
	PyObject *args;
	args = Py_BuildValue("ff"
	, $1[0]
	, $1[1]
	);
	$result = PyObject_CallObject(GLMvec2Type, args);
	Py_DECREF(args);
}

%typemap(typecheck, precedence=SWIG_TYPECHECK_POINTER) glm::vec3 {
	$1 = PyObject_IsInstance($input, GLMvec3Type) ? 1 : 0;
}

%typemap(in) glm::vec3 {
	$1 = glm_vec3Data($input);
}

%typemap(out) glm::vec3 {
	PyObject *args;
	args = Py_BuildValue("fff"
	, $1[0]
	, $1[1]
	, $1[2]
	);
	$result = PyObject_CallObject(GLMvec3Type, args);
	Py_DECREF(args);
}

%typemap(typecheck, precedence=SWIG_TYPECHECK_POINTER) glm::vec4 {
	$1 = PyObject_IsInstance($input, GLMvec4Type) ? 1 : 0;
}

%typemap(in) glm::vec4 {
	$1 = glm_vec4Data($input);
}

%typemap(out) glm::vec4 {
	PyObject *args;
	args = Py_BuildValue("ffff"
	, $1[0]
	, $1[1]
	, $1[2]
	, $1[3]
	);
	$result = PyObject_CallObject(GLMvec4Type, args);
	Py_DECREF(args);
}

%typemap(typecheck, precedence=SWIG_TYPECHECK_POINTER) glm::ivec2 {
	$1 = PyObject_IsInstance($input, GLMivec2Type) ? 1 : 0;
}

%typemap(in) glm::ivec2 {
	$1 = glm_ivec2Data($input);
}

%typemap(out) glm::ivec2 {
	PyObject *args;
	args = Py_BuildValue("ii"
	, $1[0]
	, $1[1]
	);
	$result = PyObject_CallObject(GLMivec2Type, args);
	Py_DECREF(args);
}

%typemap(typecheck, precedence=SWIG_TYPECHECK_POINTER) glm::ivec3 {
	$1 = PyObject_IsInstance($input, GLMivec3Type) ? 1 : 0;
}

%typemap(in) glm::ivec3 {
	$1 = glm_ivec3Data($input);
}

%typemap(out) glm::ivec3 {
	PyObject *args;
	args = Py_BuildValue("iii"
	, $1[0]
	, $1[1]
	, $1[2]
	);
	$result = PyObject_CallObject(GLMivec3Type, args);
	Py_DECREF(args);
}

%typemap(typecheck, precedence=SWIG_TYPECHECK_POINTER) glm::ivec4 {
	$1 = PyObject_IsInstance($input, GLMivec4Type) ? 1 : 0;
}

%typemap(in) glm::ivec4 {
	$1 = glm_ivec4Data($input);
}

%typemap(out) glm::ivec4 {
	PyObject *args;
	args = Py_BuildValue("iiii"
	, $1[0]
	, $1[1]
	, $1[2]
	, $1[3]
	);
	$result = PyObject_CallObject(GLMivec4Type, args);
	Py_DECREF(args);
}

%typemap(typecheck, precedence=SWIG_TYPECHECK_POINTER) glm::mat2 {
	$1 = PyObject_IsInstance($input, GLMmat2Type) ? 1 : 0;
}

%typemap(in) glm::mat2 {
	$1 = glm_mat2Data($input);
}

%typemap(out) glm::mat2 {
	PyObject *args;
	args = Py_BuildValue("ffff"
	, $1[0][0]
	, $1[0][1]
	, $1[1][0]
	, $1[1][1]
	);
	$result = PyObject_CallObject(GLMmat2Type, args);
	Py_DECREF(args);
}

%typemap(typecheck, precedence=SWIG_TYPECHECK_POINTER) glm::mat3 {
	$1 = PyObject_IsInstance($input, GLMmat3Type) ? 1 : 0;
}

%typemap(in) glm::mat3 {
	$1 = glm_mat3Data($input);
}

%typemap(out) glm::mat3 {
	PyObject *args;
	args = Py_BuildValue("fffffffff"
	, $1[0][0]
	, $1[0][1]
	, $1[0][2]
	, $1[1][0]
	, $1[1][1]
	, $1[1][2]
	, $1[2][0]
	, $1[2][1]
	, $1[2][2]
	);
	$result = PyObject_CallObject(GLMmat3Type, args);
	Py_DECREF(args);
}

%typemap(typecheck, precedence=SWIG_TYPECHECK_POINTER) glm::mat4 {
	$1 = PyObject_IsInstance($input, GLMmat4Type) ? 1 : 0;
}

%typemap(in) glm::mat4 {
	$1 = glm_mat4Data($input);
}

%typemap(out) glm::mat4 {
	PyObject *args;
	args = Py_BuildValue("ffffffffffffffff"
	, $1[0][0]
	, $1[0][1]
	, $1[0][2]
	, $1[0][3]
	, $1[1][0]
	, $1[1][1]
	, $1[1][2]
	, $1[1][3]
	, $1[2][0]
	, $1[2][1]
	, $1[2][2]
	, $1[2][3]
	, $1[3][0]
	, $1[3][1]
	, $1[3][2]
	, $1[3][3]
	);
	$result = PyObject_CallObject(GLMmat4Type, args);
	Py_DECREF(args);
}

%typemap(typecheck, precedence=SWIG_TYPECHECK_POINTER) glm::mat2x3 {
	$1 = PyObject_IsInstance($input, GLMmat2x3Type) ? 1 : 0;
}

%typemap(in) glm::mat2x3 {
	$1 = glm_mat2x3Data($input);
}

%typemap(out) glm::mat2x3 {
	PyObject *args;
	args = Py_BuildValue("ffffff"
	, $1[0][0]
	, $1[0][1]
	, $1[0][2]
	, $1[1][0]
	, $1[1][1]
	, $1[1][2]
	);
	$result = PyObject_CallObject(GLMmat2x3Type, args);
	Py_DECREF(args);
}

%typemap(typecheck, precedence=SWIG_TYPECHECK_POINTER) glm::mat3x2 {
	$1 = PyObject_IsInstance($input, GLMmat3x2Type) ? 1 : 0;
}

%typemap(in) glm::mat3x2 {
	$1 = glm_mat3x2Data($input);
}

%typemap(out) glm::mat3x2 {
	PyObject *args;
	args = Py_BuildValue("ffffff"
	, $1[0][0]
	, $1[0][1]
	, $1[1][0]
	, $1[1][1]
	, $1[2][0]
	, $1[2][1]
	);
	$result = PyObject_CallObject(GLMmat3x2Type, args);
	Py_DECREF(args);
}

%typemap(typecheck, precedence=SWIG_TYPECHECK_POINTER) glm::mat2x4 {
	$1 = PyObject_IsInstance($input, GLMmat2x4Type) ? 1 : 0;
}

%typemap(in) glm::mat2x4 {
	$1 = glm_mat2x4Data($input);
}

%typemap(out) glm::mat2x4 {
	PyObject *args;
	args = Py_BuildValue("ffffffff"
	, $1[0][0]
	, $1[0][1]
	, $1[0][2]
	, $1[0][3]
	, $1[1][0]
	, $1[1][1]
	, $1[1][2]
	, $1[1][3]
	);
	$result = PyObject_CallObject(GLMmat2x4Type, args);
	Py_DECREF(args);
}

%typemap(typecheck, precedence=SWIG_TYPECHECK_POINTER) glm::mat4x2 {
	$1 = PyObject_IsInstance($input, GLMmat4x2Type) ? 1 : 0;
}

%typemap(in) glm::mat4x2 {
	$1 = glm_mat4x2Data($input);
}

%typemap(out) glm::mat4x2 {
	PyObject *args;
	args = Py_BuildValue("ffffffff"
	, $1[0][0]
	, $1[0][1]
	, $1[1][0]
	, $1[1][1]
	, $1[2][0]
	, $1[2][1]
	, $1[3][0]
	, $1[3][1]
	);
	$result = PyObject_CallObject(GLMmat4x2Type, args);
	Py_DECREF(args);
}

%typemap(typecheck, precedence=SWIG_TYPECHECK_POINTER) glm::mat3x4 {
	$1 = PyObject_IsInstance($input, GLMmat3x4Type) ? 1 : 0;
}

%typemap(in) glm::mat3x4 {
	$1 = glm_mat3x4Data($input);
}

%typemap(out) glm::mat3x4 {
	PyObject *args;
	args = Py_BuildValue("ffffffffffff"
	, $1[0][0]
	, $1[0][1]
	, $1[0][2]
	, $1[0][3]
	, $1[1][0]
	, $1[1][1]
	, $1[1][2]
	, $1[1][3]
	, $1[2][0]
	, $1[2][1]
	, $1[2][2]
	, $1[2][3]
	);
	$result = PyObject_CallObject(GLMmat3x4Type, args);
	Py_DECREF(args);
}

%typemap(typecheck, precedence=SWIG_TYPECHECK_POINTER) glm::mat4x3 {
	$1 = PyObject_IsInstance($input, GLMmat4x3Type) ? 1 : 0;
}

%typemap(in) glm::mat4x3 {
	$1 = glm_mat4x3Data($input);
}

%typemap(out) glm::mat4x3 {
	PyObject *args;
	args = Py_BuildValue("ffffffffffff"
	, $1[0][0]
	, $1[0][1]
	, $1[0][2]
	, $1[1][0]
	, $1[1][1]
	, $1[1][2]
	, $1[2][0]
	, $1[2][1]
	, $1[2][2]
	, $1[3][0]
	, $1[3][1]
	, $1[3][2]
	);
	$result = PyObject_CallObject(GLMmat4x3Type, args);
	Py_DECREF(args);
}

%include "scge.hpp"
