//#include "gamecode/game_obj.h"

//NGC MATCH
void CountGameObjects(void) {
  s32 i = 64;

    while (i > 0) {
       if (pObj[i - 1] != NULL) {
            break;
        }
        i--;
    }
    GAMEOBJECTCOUNT = i; 
  return;
}

//NGC MATCH
void RemoveGameObject(struct obj_s *obj) {
  s32 i;
  for (i = 0; i < 0x40; i++) {
    if (pObj[i] == obj) {
      pObj[i] = 0;
      i = 0x40;
    }
  }
  CountGameObjects();
  return;
}