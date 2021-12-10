#ifndef GOT_HERE_INCLUDED
#define GOT_HERE_INCLUDED

#include <stdio.h>

#define GOT_HERE() printf("Got here: '%s' in '%s' at line %d\n" , __func__, __FILE__, __LINE__)

#endif
