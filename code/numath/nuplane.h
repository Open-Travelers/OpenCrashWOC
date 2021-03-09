#ifndef NUPLANE_H
#define NUPLANE_H

#include "../types.h"
#include "numathtypes.h"

// Check if there is an intersection between a plane and a line, parameters and variables currently unknown.
u32 NuPlnLine(Vec4* p1, Vec* p2, Vec* p3, Vec* p4);

#endif // !NUPLANE_H