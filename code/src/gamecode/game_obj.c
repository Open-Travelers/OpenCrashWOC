//#include "gamecode/game_obj.h"

//NGC MATCH
void CountGameObjects() 
{
    int i;

    for (i = 64; i > 0; i--) 
    { 
        if (pObj[i - 1] != NULL) 
        {
            break;
        }
    }
    
    GAMEOBJECTCOUNT = i; 
}

//NGC MATCH
void RemoveGameObject(struct obj_s *obj) 
{
    int i;
    
    for (i = 0; i < 64; i++) 
    {
        if (pObj[i] == obj) 
        {
            pObj[i] = NULL;
            
            i = 64;
        }
    }
    
    CountGameObjects();
}
