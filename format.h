#ifndef __FORMAT_H__
#define __FORMAT_H__

#include "greg.h"

char *formatLine( const date_t gregDate, 
                 const date_t hebDate, 
                 const char *text,
                 char *output,
                 size_t maxOutputLen
    );

#endif

