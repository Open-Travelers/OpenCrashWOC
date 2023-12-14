#ifndef NUVEC4_H
#define NUVEC4_H

#include "../types.h"
#include "numathtypes.h"

// Scale v by an amount and put it into the destination.
void NuVec4Scale(f32 scale, struct nuvec4_s* dest, struct nuvec4_s* v);

// Transform the vector with a matrix and store it into the destination.
void NuVec4MtxTransform(struct nuvec4_s* dest, struct nuvec4_s* a, struct Mtx* b);

// Transform the vector with the inverse matrix and store it into the destination.
void NuVec4MtxInvTransform(struct nuvec4_s* dest, struct nuvec4_s* a, struct Mtx* b);

#endif // !NUVEC4_H
