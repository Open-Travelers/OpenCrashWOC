#ifndef NUVEC_H
#define NUVEC_H

#include "../types.h"
#include "numathtypes.h"

// Apply a transformation vector with a matrix and store it into the destination.
void NuVecMtxTransform(Vec* dest, Vec* a, Mtx* b);

// Idk what this one does.
void NuVecMtxTransformH(Vec* dest, Vec* a, Mtx* b);

// Rotate a vector with a matrix and put it into the destination.
void NuVecMtxRotate(Vec* dest, Vec* a, Mtx* b);

// Transform a vector with an inverse matrix and store it into the dest.
void NuVecInvMtxTransform(Vec* dest, Vec* a, Mtx* b);

// Rotate a vector by an inverse matrix and put it into the destination.
void NuVecInvMtxRotate(Vec* dest, Vec* a, Mtx* b);

// Rotate a vector v by an angle around the x axis, and place it into the destination.
void NuVecRotateX(Vec* dest, Vec* v, angle a);

// Rotate a vector v by an angle around the y axis, and place it into the destination.
void NuVecRotateY(Vec* dest, Vec* v, angle a);

// Rotate a vector v by an angle around the z axis, and place it into the destination.
void NuVecRotateZ(Vec* dest, Vec* v, angle a);

// Add a and b and put it into the destination.
void NuVecAdd(Vec* dest, Vec* a, Vec* b);

// Subtract b from a and put it into the destination.
void NuVecSub(Vec* dest, Vec* a, Vec* b);

// Scale v by the scale and put it into the destination.
void NuVecScale(f32 scale, Vec* dest, Vec* v);

// Scale v by the scale and put it into the destination, but the values accumulate.
void NuVecScaleAccum(f32 scale, Vec* dest, Vec* v);

// Cross a and b and put it into the destination.
void NuVecCross(Vec* dest, Vec* a, Vec* b);

// Return the dot product of a and b.
f32 NuVecDot(Vec* a, Vec* b);

// Get the magnitude of v. This isn't how the original does it, but it makes more sense this way (calling existing functions).
f32 NuVecMag(Vec* v);

// Get the squared magnitude of v. This isn't how the original does it, but it makes more sense this way (calling existing functions).
f32 NuVecMagSqr(Vec* v);

// Normalize the vector v and put it into the destination. This isn't how the original does it, but it makes more sense this way (calling existing functions).
void NuVecNorm(Vec* dest, Vec* v);

// Get the distance between a and b. The distance vector is stored in dist.
f32 NuVecDist(Vec* a, Vec* b, Vec* dist);

// Get the squared distance between a and b. The unsquared distance vector is stored in dist.
f32 NuVecDistSqr(Vec* a, Vec* b, Vec* dist);

// Get the distance between X and Z and store it into the dist vector as well. Y distance is 0.
f32 NuVecXZDist(Vec* a, Vec* b, Vec* dist);

// Get the squared distance between X and Z and store it into the dist vector as well. Y distance is 0. This diance vector is not squared.
f32 NuVecXZDistSqr(Vec* a, Vec* b, Vec* dist);

#endif // !NUVEC_H