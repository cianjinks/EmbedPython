#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <iostream>

#include "vector.h"

static PyMethodDef VectorModuleMethods[] = {
    {NULL, NULL, 0, NULL}
};

PyDoc_STRVAR(VectorModuleDoc, "Testing custom vector utilities");
static struct PyModuleDef VectorModuleDef = {
    PyModuleDef_HEAD_INIT,
    "vector",
    VectorModuleDoc, /* Module Docs */
    0,
    VectorModuleMethods,  /* Module Methods*/
    NULL,
    NULL,
    NULL,
    NULL,

};

PyMODINIT_FUNC PyInit_vector(void)
{
    return PyModule_Create(&VectorModuleDef);
}

static struct _inittab custom_modules[] = {
    {"vector", PyInit_vector},
    {NULL, NULL}
};

int main(int argc, char *argv[])
{
    vec2 a(3, 3);
    vec2 b(2, 2);
    vec2 r = mul_v2(a, b);
    r.print();

    FILE *fp = fopen("./scripts/test.py", "r");
    wchar_t *program = Py_DecodeLocale(argv[0], NULL);
    if (program == NULL) {
        fprintf(stderr, "Fatal error: cannot decode argv[0]\n");
        exit(1);
    }
    PyImport_ExtendInittab(custom_modules);
    Py_SetProgramName(program);  /* optional but recommended */
    Py_Initialize();
    PyRun_SimpleFile(fp, "test.py");
    if (Py_FinalizeEx() < 0) {
        exit(120);
    }
    PyMem_RawFree(program);
    fclose(fp);
    return 0;
}