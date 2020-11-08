#ifndef EXCELHANDLER_H_INCLUDED
#define EXCELHANDLER_H_INCLUDED
#include "dataUtil.h"
status readFromCSV(const char *file_name, ptrData* ret, Type* default_types);
status saveAsCSV(const char *file_name, ptrData pdata);
const char * linesReader(const char *file_name);
#endif // EXCELHANDLER_H_INCLUDED
