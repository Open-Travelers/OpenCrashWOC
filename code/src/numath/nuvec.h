#ifndef NUVEC_H
#define NUVEC_H

#include "../types.h"
#include "numathtypes.h"

// Apply a transformation vector with a matrix and store it into the destination.
void NuVecMtxTransform(struct nuvec_s* dest, struct nuvec_s* a, struct Mtx* b);

// Idk what this one does.
void NuVecMtxTransformH(struct nuvec_s* dest, struct nuvec_s* a, struct Mtx* b);

// Rotate a vector with a matrix and put it into the destination.
void NuVecMtxRotate(struct nuvec_s* dest, struct nuvec_s* a, struct Mtx* b);

// Transform a vector with an inverse matrix and store it into the dest.
void NuVecInvMtxTransform(struct nuvec_s* dest, struct nuvec_s* a, struct Mtx* b);

// Rotate a vector by an inverse matrix and put it into the destination.
void NuVecInvMtxRotate(struct nuvec_s* dest, struct nuvec_s* a, struct Mtx* b);

// Rotate a vector v by an angle around the x axis, and place it into the destination.
void NuVecRotateX(struct nuvec_s* dest, struct nuvec_s* v, s32 angle);

// Rotate a vector v by an angle around the y axis, and place it into the destination.
void NuVecRotateY(struct nuvec_s* dest, struct nuvec_s* v, s32 angle);

// Rotate a vector v by an angle around the z axis, and place it into the destination.
void NuVecRotateZ(struct nuvec_s* dest, struct nuvec_s* v, s32 angle);

// Add a and b and put it into the destination.
void NuVecAdd(struct nuvec_s* dest, struct nuvec_s* a, struct nuvec_s* b);

// Subtract b from a and put it into the destination.
void NuVecSub(struct nuvec_s* dest, struct nuvec_s* a, struct nuvec_s* b);

// Scale v by the scale and put it into the destination.
void NuVecScale(f32 scale, struct nuvec_s* dest, struct nuvec_s* v);

// Scale v by the scale and put it into the destination, but the values accumulate.
void NuVecScaleAccum(f32 scale, struct nuvec_s* dest, struct nuvec_s* v);

// Cross a and b and put it into the destination.
void NuVecCross(struct nuvec_s* dest, struct nuvec_s* a, struct nuvec_s* b);

// Return the dot product of a and b.
f32 NuVecDot(struct nuvec_s* a, struct nuvec_s* b);

// Get the magnitude of v. This isn't how the original does it, but it makes more sense this way (calling existing functions).
f32 NuVecMag(struct nuvec_s* v);

// Get the squared magnitude of v. This isn't how the original does it, but it makes more sense this way (calling existing functions).
f32 NuVecMagSqr(struct nuvec_s* v);

// Normalize the vector v and put it into the destination. This isn't how the original does it, but it makes more sense this way (calling existing functions).
void NuVecNorm(struct nuvec_s* dest, struct nuvec_s* v);

// Get the distance between a and b. The distance vector is stored in dist.
f32 NuVecDist(struct nuvec_s* a, struct nuvec_s* b, struct nuvec_s* dist);

// Get the squared distance between a and b. The unsquared distance vector is stored in dist.
f32 NuVecDistSqr(struct nuvec_s* a, struct nuvec_s* b, struct nuvec_s* dist);

// Get the distance between X and Z and store it into the dist vector as well. Y distance is 0.
f32 NuVecXZDist(struct nuvec_s* a, struct nuvec_s* b, struct nuvec_s* dist);

// Get the squared distance between X and Z and store it into the dist vector as well. Y distance is 0. This diance vector is not squared.
f32 NuVecXZDistSqr(struct nuvec_s* a, struct nuvec_s* b, struct nuvec_s* dist);

#endif // !NUVEC_H
