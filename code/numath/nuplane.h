#ifndef NUPLANE_H
#define NUPLANE_H

#include "../types.h"
#include "numathtypes.h"

// Check if there is an intersection between a plane and a line, parameters and variables currently unknown.
u32 NuPlnLine(struct Vec4* p1, struct Vec* p2, struct Vec* p3, struct Vec* p4);

#endif // !NUPLANE_H
