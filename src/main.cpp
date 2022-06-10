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

static void test_vec2()
{
    vec2 a(3, 3);
    vec2 b(2, 2);
    vec2 r = mul_v2(a, b);
    r.print();
}

int main(int argc, char *argv[])
{
    test_vec2();

    PyObject *modules = NULL, *builtins = NULL, *mod_main = NULL;
    PyObject *py_result = NULL, *py_dict = NULL;
    PyObject *pName = NULL, *pFunc = NULL;
    PyObject *pArgs = NULL, *pValue = NULL;

    long array[2] = {2, 3};

    const char* filename = "test.py";
    const char* filepath = "./scripts/test.py";
    FILE *fp = fopen(filepath, "r");
    if(fp)
    {
        wchar_t *program = Py_DecodeLocale(argv[0], NULL);
        if (program == NULL) {
            printf("Fatal error: cannot decode argv[0]\n");
            return 1;
        }
        PyImport_ExtendInittab(custom_modules);
        Py_SetProgramName(program);  /* optional but recommended */
        Py_Initialize();
        modules = PyImport_GetModuleDict();
        builtins = PyEval_GetBuiltins();
        mod_main = PyModule_New("__main__");
        PyDict_SetItemString(modules, "__main__", mod_main);
        Py_DECREF(mod_main); /* sys.modules owns now */
        PyModule_AddStringConstant(mod_main, "__name__", "__main__");
        PyModule_AddObject(mod_main, "__file__", PyUnicode_FromStringAndSize(filename, strlen(filename)));
        PyModule_AddObject(mod_main, "__builtins__", builtins);
        Py_INCREF(builtins); /* AddObject steals a reference */
        py_dict = PyModule_GetDict(mod_main);
        py_result = PyRun_File(fp, filepath, Py_file_input, py_dict, py_dict);

        /* Test running function from module. */
        if (mod_main != NULL) {
            pFunc = PyObject_GetAttrString(mod_main, "multiply");
            /* pFunc is a new reference */

            if (pFunc && PyCallable_Check(pFunc)) {
                pArgs = PyTuple_New(2);
                for (int i = 0; i < 2; ++i) {
                    pValue = PyLong_FromLong(array[i]);
                    if (!pValue) {
                        Py_DECREF(pArgs);
                        Py_DECREF(mod_main);
                        printf("Cannot convert argument\n");
                        return 1;
                    }
                    /* pValue reference stolen here: */
                    PyTuple_SetItem(pArgs, i, pValue);
                }
                pValue = PyObject_CallObject(pFunc, pArgs);
                Py_DECREF(pArgs);
                if (pValue != NULL) {
                    printf("Result of call: %ld\n", PyLong_AsLong(pValue));
                    Py_DECREF(pValue);
                }
                else {
                    Py_DECREF(pFunc);
                    Py_DECREF(mod_main);
                    PyErr_Print();
                    printf("Call failed\n");
                    return 1;
                }
            }
            else {
                if (PyErr_Occurred())
                    PyErr_Print();
                printf("Cannot find function multiply\n");
            }
            Py_XDECREF(pFunc);
            Py_DECREF(mod_main);
        }
        else {
            PyErr_Print();
            printf("Failed to load module\n");
            return 1;
        }

        if (Py_FinalizeEx() < 0) {
            printf("Interpreter failed to exit correctly :(\n");
            return 1;
        }
        PyMem_RawFree(program);
        fclose(fp);
    }
    else
    {
        fprintf(stderr, "Failed to load filepath: %s\n", filepath);
        return 1;
    }
    return 0;
}