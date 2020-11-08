//
// Created by 16009 on 2020/10/30.
//

#ifndef INFOMANAGER_ANALYSIS_H
#define INFOMANAGER_ANALYSIS_H

#include "dataUtil.h"
#include "status.h"
#include "query.h"
#include <string.h>
#include <stdio.h>

double median(ptrData pdata, int col);
double mean(ptrData pdata, int col);
status analyse(ptrData pdata, const char * fieldname);

#endif //INFOMANAGER_ANALYSIS_H
