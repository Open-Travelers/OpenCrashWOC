#ifndef NUVEC_H
#define NUVEC_H

#include "../types.h"
#include "numathtypes.h"

// Apply a transformation vector with a matrix and store it into the destination.
void NuVecMtxTransform(struct Vec* dest, struct Vec* a, struct Mtx* b);

// Idk what this one does.
void NuVecMtxTransformH(struct Vec* dest, struct Vec* a, struct Mtx* b);

// Rotate a vector with a matrix and put it into the destination.
void NuVecMtxRotate(struct Vec* dest, struct Vec* a, struct Mtx* b);

// Transform a vector with an inverse matrix and store it into the dest.
void NuVecInvMtxTransform(struct Vec* dest, struct Vec* a, struct Mtx* b);

// Rotate a vector by an inverse matrix and put it into the destination.
void NuVecInvMtxRotate(struct Vec* dest, struct Vec* a, struct Mtx* b);

// Rotate a vector v by an angle around the x axis, and place it into the destination.
void NuVecRotateX(struct Vec* dest, struct Vec* v, s32 angle);

// Rotate a vector v by an angle around the y axis, and place it into the destination.
void NuVecRotateY(struct Vec* dest, struct Vec* v, s32 angle);

// Rotate a vector v by an angle around the z axis, and place it into the destination.
void NuVecRotateZ(struct Vec* dest, struct Vec* v, s32 angle);

// Add a and b and put it into the destination.
void NuVecAdd(struct Vec* dest, struct Vec* a, struct Vec* b);

// Subtract b from a and put it into the destination.
void NuVecSub(struct Vec* dest, struct Vec* a, struct Vec* b);

// Scale v by the scale and put it into the destination.
void NuVecScale(f32 scale, struct Vec* dest, struct Vec* v);

// Scale v by the scale and put it into the destination, but the values accumulate.
void NuVecScaleAccum(f32 scale, struct Vec* dest, struct Vec* v);

// Cross a and b and put it into the destination.
void NuVecCross(struct Vec* dest, struct Vec* a, struct Vec* b);

// Return the dot product of a and b.
f32 NuVecDot(struct Vec* a, struct Vec* b);

// Get the magnitude of v. This isn't how the original does it, but it makes more sense this way (calling existing functions).
f32 NuVecMag(struct Vec* v);

// Get the squared magnitude of v. This isn't how the original does it, but it makes more sense this way (calling existing functions).
f32 NuVecMagSqr(struct Vec* v);

// Normalize the vector v and put it into the destination. This isn't how the original does it, but it makes more sense this way (calling existing functions).
void NuVecNorm(struct Vec* dest, struct Vec* v);

// Get the distance between a and b. The distance vector is stored in dist.
f32 NuVecDist(struct Vec* a, struct Vec* b, struct Vec* dist);

// Get the squared distance between a and b. The unsquared distance vector is stored in dist.
f32 NuVecDistSqr(struct Vec* a, struct Vec* b, struct Vec* dist);

// Get the distance between X and Z and store it into the dist vector as well. Y distance is 0.
f32 NuVecXZDist(struct Vec* a, struct Vec* b, struct Vec* dist);

// Get the squared distance between X and Z and store it into the dist vector as well. Y distance is 0. This diance vector is not squared.
f32 NuVecXZDistSqr(struct Vec* a, struct Vec* b, struct Vec* dist);

#endif // !NUVEC_H
