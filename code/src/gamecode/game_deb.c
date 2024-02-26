//#include "gamecode/game_deb.h"

s32 atlasbits[7];
s32 helibits[7];
s32 jeepbits[8];
s32 mechbits[7];

//NGC MATCH
void InitGameDebris(void) {
  s32 i;
  
  for(i = 0; i < 170; i++) {
    GDeb[i].i = -1;
    if (((GDeb[i].levbits >> Level) & 1) != 0) {
      GDeb[i].i = LookupDebrisEffect(GDeb[i].name);
    }
  }
  return;
}

//NGC MATCH
void AddGameDebris(s32 i,struct nuvec_s *pos) {
  s32 key;
  
  if (((NODEBRIS == 0) && ((uint)i < 0xaa)) && (GDeb[i].i != -1)) {
    key = -1;
    AddFiniteShotDebrisEffect(&key,GDeb[i].i,pos,1);
  }
  return;
}

//NGC MATCH
void AddGameDebrisRot(s32 i,struct nuvec_s *pos,s32 n,u16 xrot,u16 yrot) {
  if (((NODEBRIS == 0) && ((u32)i < 0xaa)) && (GDeb[i].i != -1)) {
    AddVariableShotDebrisEffect(GDeb[i].i,pos,n,xrot,yrot + 0x4000);
  }
  return;
}

//NGC MATCH
void AddGameDebrisMtx(s32 i,struct nuvec_s *pos,s32 n,struct numtx_s *m) {
  if (((NODEBRIS == 0) && ((u32)i < 0xaa)) && (GDeb[i].i != -1)) {
    NuMtxSetRotationX(&mTEMP,0x4000);
    NuMtxMulR(&mTEMP,&mTEMP,m);
    AddVariableShotDebrisEffectMtx2(GDeb[i].i,pos,n,&mTEMP,&numtx_identity);
  }
  return;
}

//NGC MATCH
void AddWarpDebris(struct obj_s *obj,struct nuvec_s *pos) {
  struct nuvec_s v;
  s32 key;
  
  key = -1;
  v.x = (obj->pos).x;
  v.y = (obj->bot + obj->top) * obj->SCALE * 0.5f + (obj->pos).y;
  v.z = (obj->pos).z;
  AddFiniteShotDebrisEffect(&key,GDeb[77].i,&v,1);
  return;
}

//NGC MATCH
void AddMechanicalDebris(struct nuvec_s* pos, s32 vehicle) {
    s32 i;
    s32* pt;

    pt = jeepbits;
    i = 1;
    switch (vehicle) {
    case 0x44:
    case 0xB2:
        i = 1;
        pt = mechbits;
        break;
    case 0x53:
        pt = atlasbits;
        break;
    case 0x3B:
        pt = helibits;
        break;
    case 0x20:
        AddGameDebris(0x8E, pos);
        i = 6;
        break;
    }
    while (*pt != -1) {
        AddDeb3(pos, pt[0], i, NULL);
        pt++;
    }
}