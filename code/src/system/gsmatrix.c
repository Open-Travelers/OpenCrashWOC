#include "gs.h"

//NGC MATCH
void SetIdentity(struct _GSMATRIX *d) {
  d->_11 = 1.0f;
  d->_12 = 0.0f;
  d->_13 = 0.0f;
  d->_14 = 0.0f;
  d->_21 = 0.0f;
  d->_22 = 1.0f;
  d->_23 = 0.0f;
  d->_24 = 0.0f;
  d->_31 = 0.0f;
  d->_32 = 0.0f;
  d->_33 = 1.0f;
  d->_34 = 0.0f;
  d->_41 = 0.0f;
  d->_42 = 0.0f;
  d->_43 = 0.0f;
  d->_44 = 1.0f;
  return;
}

//DONE??
void MatMult(struct _GSMATRIX *MatrixA,struct _GSMATRIX *MatrixB) {
  struct _GSMATRIX NewMatrix;

  NewMatrix._11 = MatrixA->_14 * MatrixB->_41 + MatrixA->_13 * MatrixB->_31 + MatrixA->_11 * MatrixB->_11 + MatrixA->_12 * MatrixB->_21;
  NewMatrix._12 = MatrixA->_14 * MatrixB->_42 + MatrixA->_13 * MatrixB->_32 + MatrixA->_11 * MatrixB->_12 + MatrixA->_12 * MatrixB->_22;
  NewMatrix._13 = MatrixA->_14 * MatrixB->_43 + MatrixA->_13 * MatrixB->_33 + MatrixA->_11 * MatrixB->_13 + MatrixA->_12 * MatrixB->_23;
  NewMatrix._14 = MatrixA->_14 * MatrixB->_44 + MatrixA->_13 * MatrixB->_34 + MatrixA->_11 * MatrixB->_14 + MatrixA->_12 * MatrixB->_24;
  NewMatrix._21 = MatrixA->_24 * MatrixB->_41 + MatrixA->_23 * MatrixB->_31 + MatrixA->_21 * MatrixB->_11 + MatrixB->_11 * MatrixB->_21;
  NewMatrix._22 = MatrixA->_24 * MatrixB->_42 + MatrixA->_23 * MatrixB->_32 + MatrixA->_21 * MatrixB->_12 + MatrixB->_11 * MatrixB->_22;
  NewMatrix._23 = MatrixA->_24 * MatrixB->_43 + MatrixA->_23 * MatrixB->_33 + MatrixA->_21 * MatrixB->_13 + MatrixB->_11 * MatrixB->_23;
  NewMatrix._24 = MatrixA->_24 * MatrixB->_44 + MatrixA->_23 * MatrixB->_34 + MatrixA->_21 * MatrixB->_14 + MatrixB->_11 * MatrixB->_24;
  NewMatrix._31 = MatrixA->_34 * MatrixB->_41 + MatrixA->_33 * MatrixB->_31 + MatrixA->_31 * MatrixB->_11 + MatrixA->_32 * MatrixB->_21;
  NewMatrix._32 = MatrixA->_34 * MatrixB->_42 + MatrixA->_33 * MatrixB->_32 + MatrixA->_31 * MatrixB->_12 + MatrixA->_32 * MatrixB->_22;
  NewMatrix._33 = MatrixA->_34 * MatrixB->_43 + MatrixA->_33 * MatrixB->_33 + MatrixA->_31 * MatrixB->_13 + MatrixA->_32 * MatrixB->_23;
  NewMatrix._34 = MatrixA->_34 * MatrixB->_44 + MatrixA->_33 * MatrixB->_34 + MatrixA->_31 * MatrixB->_14 + MatrixA->_32 * MatrixB->_24;
  NewMatrix._41 = MatrixA->_44 * MatrixB->_41 +  MatrixA->_43 * MatrixB->_31 + MatrixA->_41 * MatrixB->_11 + MatrixA->_42 * MatrixB->_21;
  NewMatrix._42 = MatrixA->_44 * MatrixB->_42 +  MatrixA->_43 * MatrixB->_32 + MatrixA->_41 * MatrixB->_12 + MatrixA->_42 * MatrixB->_22;
  NewMatrix._43 = MatrixA->_44 * MatrixB->_43 +  MatrixA->_43 * MatrixB->_33 + MatrixA->_41 * MatrixB->_13 + MatrixA->_42 * MatrixB->_23;
  NewMatrix._44 = MatrixA->_44 * MatrixB->_44 +  MatrixA->_43 * MatrixB->_34 + MatrixA->_41 * MatrixB->_14 + MatrixA->_42 * MatrixB->_24;
  *MatrixA = NewMatrix;
  return;
}

//NGC MATCH
void MatReorder(struct _GSMATRIX *MatrixA) {
  struct _GSMATRIX NewMatrix;

  NewMatrix._11 = MatrixA->_11;
  NewMatrix._12 = MatrixA->_21;
  NewMatrix._13 = MatrixA->_31;
  NewMatrix._14 = MatrixA->_41;
  NewMatrix._21 = MatrixA->_12;
  NewMatrix._22 = MatrixA->_22;
  NewMatrix._23 = MatrixA->_32;
  NewMatrix._24 = MatrixA->_42;
  NewMatrix._31 = -MatrixA->_13;
  NewMatrix._32 = -MatrixA->_23;
  NewMatrix._33 = -MatrixA->_33;
  NewMatrix._34 = -MatrixA->_43;
  NewMatrix._41 = MatrixA->_14;
  NewMatrix._42 = MatrixA->_24;
  NewMatrix._43 = MatrixA->_34;
  NewMatrix._44 = MatrixA->_44;
   *MatrixA = NewMatrix;
  return;
}
