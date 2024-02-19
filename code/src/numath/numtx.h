#ifndef NUMTX_H
#define NUMTX_H

#include "../types.h"
#include "numathtypes.h"

/*
  800bfe58 0003e4 800bfe58  4 NuMtxInvH 	Global
  800c023c 0002d0 800c023c  4 NuMtxAlignZ 	Global
*/

extern struct Mtx mident;
extern struct Mtx gm;

// Set m to the identity matrix.
void NuMtxSetIdentity(struct Mtx* m);

// Set the translation vector of the matrix.
void NuMtxSetTranslation(struct Mtx* m, struct nuvec_s* v);

// Set the scale vector of the matrix.
void NuMtxSetScale(struct Mtx* m, struct nuvec_s* v);

// Set a rotation matrix around the x axis.
void NuMtxSetRotationX(struct Mtx* m, s32 a);

// Set a rotation matrix around the y axis.
void NuMtxSetRotationY(struct Mtx* m, s32 a);

// Set a rotation matrix around the z axis.
void NuMtxSetRotationZ(struct Mtx* m, s32 a);

// Translate the matrix by a vector.
void NuMtxTranslate(struct Mtx* m, struct nuvec_s* v);

// Before translating???
void NuMtxPreTranslate(struct Mtx* m, struct nuvec_s* v);

// Scale a matrix by a vector.
void NuMtxScale(struct Mtx* m, struct nuvec_s* v);

// Get the scale of a matrix and put it into the destination.
void NuMtxGetScale(struct nuvec_s* dest, struct Mtx* m);

// Before scale???
void NuMtxPreScale(struct Mtx* m, struct nuvec_s* v);

// Rotate a matrix around the x axis.
void NuMtxRotateX(struct Mtx* m, s32 a);

// Pre-rotate a matrix around the x axis.
void NuMtxPreRotateX(struct Mtx* m, s32 a);

// Rotate a matrix around the y axis.
void NuMtxRotateY(struct Mtx* m, s32 a);

// Pre-rotate a matrix around the y axis.
void NuMtxPreRotateY(struct Mtx* m, s32 a);

// Rotate a matrix around the z axis.
void NuMtxRotateZ(struct Mtx* m, s32 a);

// Pre-rotate a matrix around the z axis.
void NuMtxPreRotateZ(struct Mtx* m, s32 a);

// Set the x, y, and z rotation.
void NuMtxSetRotateXYZ(struct Mtx* m, s32 a[3]);

// Multiply a and b and place them into the destination.
void NuMtxMul(struct Mtx* dest, struct Mtx* a, struct Mtx* b);

// Multiply a and b and place them into the destination.
void NuMtxMulH(struct Mtx* dest, struct Mtx* a, struct Mtx* b);

// Multiply a and b and place them into the destination.
void NuMtxMulR(struct Mtx* dest, struct Mtx* a, struct Mtx* b);

// Transpose a matrix and place it into the destination.
void NuMtxTranspose(struct Mtx* dest, struct Mtx* m);

// Take the inverse of a matrix and put it into the destination.
void NuMtxInv(struct Mtx* dest, struct Mtx* m);

// Take the inverse of a matrix and put it into the destination.
void NuMtxInvR(struct Mtx* dest, struct Mtx* m);

// Take the inverse of a matrix and apply a scale?
void NuMtxInvRSS(struct Mtx* dest, struct Mtx* m);

//NuMtxInvH

//NuMtxAlignZ
void NuMtxAlignZ(struct Mtx* dest, struct Mtx* m);

// Create a matrix to look at Z.
void NuMtxLookAtZ(struct Mtx* dest, struct nuvec_s* v);

// Add two matrices.
void NuMtxAddR(struct Mtx* dest, struct Mtx* a, struct Mtx* b);

// Skew a matrix by a vector.
void NuMtxSkewSymmetric(struct Mtx* m, struct nuvec_s* v);

// Make a matrix into ortho.
void NuMtxOrth(struct Mtx* m);

// Face a vector?
void NuMtxCalcCheapFaceOn(struct Mtx* dest, struct nuvec_s* v);

// Calculate where debris should face?
void NuMtxCalcDebrisFaceOn(struct Mtx* m);

#endif // !NUMTX_H
