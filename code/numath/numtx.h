#ifndef NUMTX_H
#define NUMTX_H

#include "../types.h"
#include "numathtypes.h"

/*
  800bfe58 0003e4 800bfe58  4 NuMtxInvH 	Global
  800c023c 0002d0 800c023c  4 NuMtxAlignZ 	Global
*/

Mtx mident;
Mtx gm;

// Set m to the identity matrix.
void NuMtxSetIdentity(Mtx* m);

// Set the translation vector of the matrix.
void NuMtxSetTranslation(Mtx* m, Vec* v);

// Set the scale vector of the matrix.
void NuMtxSetScale(Mtx* m, Vec* v);

// Set a rotation matrix around the x axis.
void NuMtxSetRotationX(Mtx* m, angle a);

// Set a rotation matrix around the y axis.
void NuMtxSetRotationY(Mtx* m, angle a);

// Set a rotation matrix around the z axis.
void NuMtxSetRotationZ(Mtx* m, angle a);

// Translate the matrix by a vector.
void NuMtxTranslate(Mtx* m, Vec* v);

// Before translating???
void NuMtxPreTranslate(Mtx* m, Vec* v);

// Scale a matrix by a vector.
void NuMtxScale(Mtx* m, Vec* v);

// Get the scale of a matrix and put it into the destination.
void NuMtxGetScale(Vec* dest, Mtx* m);

// Before scale???
void NuMtxPreScale(Mtx* m, Vec* v);

// Rotate a matrix around the x axis.
void NuMtxRotateX(Mtx* m, angle a);

// Pre-rotate a matrix around the x axis.
void NuMtxPreRotateX(Mtx* m, angle a);

// Rotate a matrix around the y axis.
void NuMtxRotateY(Mtx* m, angle a);

// Pre-rotate a matrix around the y axis.
void NuMtxPreRotateY(Mtx* m, angle a);

// Rotate a matrix around the z axis.
void NuMtxRotateZ(Mtx* m, angle a);

// Pre-rotate a matrix around the z axis.
void NuMtxPreRotateZ(Mtx* m, angle a);

// Set the x, y, and z rotation.
void NuMtxSetRotateXYZ(Mtx* m, angle a[3]);

// Multiply a and b and place them into the destination.
void NuMtxMul(Mtx* dest, Mtx* a, Mtx* b);

// Multiply a and b and place them into the destination.
void NuMtxMulH(Mtx* dest, Mtx* a, Mtx* b);

// Multiply a and b and place them into the destination.
void NuMtxMulR(Mtx* dest, Mtx* a, Mtx* b);

// Transpose a matrix and place it into the destination.
void NuMtxTranspose(Mtx* dest, Mtx* m);

// Take the inverse of a matrix and put it into the destination.
void NuMtxInv(Mtx* dest, Mtx* m);

// Take the inverse of a matrix and put it into the destination.
void NuMtxInvR(Mtx* dest, Mtx* m);

// Take the inverse of a matrix and apply a scale?
void NuMtxInvRSS(Mtx* dest, Mtx* m);

//NuMtxInvH

//NuMtxAlignZ

// Create a matrix to look at Z.
void NuMtxLookAtZ(Mtx* dest, Vec* v);

// Add two matrices.
void NuMtxAddR(Mtx* dest, Mtx* a, Mtx* b);

// Skew a matrix by a vector.
void NuMtxSkewSymmetric(Mtx* m, Vec* v);

// Make a matrix into ortho.
void NuMtxOrth(Mtx* m);

// Face a vector?
void NuMtxCalcCheapFaceOn(Mtx* dest, Vec* v);

// Calculate where debris should face?
void NuMtxCalcDebrisFaceOn(Mtx* m);

#endif // !NUMTX_H