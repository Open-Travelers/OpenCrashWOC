#ifndef NUPLANE_H
#define NUPLANE_H

#include "../types.h"
#include "numathtypes.h"

// Check if there is an intersection between a plane and a line, parameters and variables currently unknown.
u32 NuPlnLine(struct nuvec4_s* p1, struct nuvec_s* p2, struct nuvec_s* p3, struct nuvec_s* p4);

#endif // !NUPLANE_H
