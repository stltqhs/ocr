#include <Python.h>
#include "ocr.h"
static PyObject *recognize_from_file(PyObject *self, PyObject *args)
{
	const char *s = NULL;
	if (!PyArg_ParseTuple(args, "s", &s))
		return NULL;
	char *text = RecognizeFromFile(s);
	if (text != NULL)
	{
		PyObject *py_string;
		py_string = Py_BuildValue("s", text);
		free(text);
		return py_string;
	}
	return NULL;
}

static PyObject *recognize_from_imagedata(PyObject *self, PyObject *args)
{
	char *data;
	int length;
	if (!PyArg_ParseTuple(args, "iw", &length, &data))
		return NULL;
	char *text = RecognizeFromRawImageData((unsigned char*)data, length);
	if (text != NULL)
	{
		PyObject *py_string;
		py_string = Py_BuildValue("s", text);
		free(text);
		return py_string;
	}
	return NULL;
}

static PyMethodDef OcrMethods[] = {
	{"recognize_from_file", recognize_from_file, METH_VARARGS, "recognize from file!"},
	{"recognize_from_imagedata", recognize_from_imagedata, METH_VARARGS, "recognize from data!"},
	{NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC
initcsdnocr(void)
{
	PyObject *m;
	m = Py_InitModule("csdnocr", OcrMethods);
}
