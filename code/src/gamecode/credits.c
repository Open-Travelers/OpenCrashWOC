void InitCredits(void)
{
  float fVar1;
  Credit_s *Cr;
  short size;
  float tot;
  
  fVar1 = 0.0f;
  CREDITCOUNT = 0;
  if (0 < Credit[0].size) {
    CREDITCOUNT = 0;
    Cr = Credit;
    size = Credit[0].size;
    do {
      CREDITCOUNT = CREDITCOUNT + 1;
      size = Cr[1].size;
      tot = size / 1000.0f;
      fVar1 = fVar1 + tot + tot;
      Cr = Cr + 1;
    } while (0 < size);
  }
  credit_time = (fVar1 + 2.0f) / credit_speed;
  return;
}

void DrawCredits(void)
{
  s32 i;
  struct Credit_s *Cptr;
  float y;
  float xscale;
  float size;
  
  i = 0;
  Cptr = Credit;
  xscale = GameTimer.ftime * credit_speed + -1.0;
  if (0 < CREDITCOUNT) {
    do {
      y = xscale;
      size = Cptr->size/ 100.0f;
      if (((Cptr->txt != NULL) && (-1.5f < xscale)) && (xscale < 1.700000047683716f)) {
        strcpy(tbuf,Cptr->txt);
        xscale = 0.85f;
        if (strcmp(tbuf,"SR. MANAGER OF PUBLIC RELATIONS") != 0) {
          xscale = 0.9f;
        }
        if (Game.language == 'c') {
          AddSpacesIntoText(tbuf,1);
        }
        Text3D(tbuf,0.0f,y,1.0f,(size * xscale),size,(size + size ), 2,(s32)Cptr->colour);
      }
      i = i + 1;
      xscale = (y - (size * 0.1000000014901161f + size * 0.1000000014901161f));
      Cptr = Cptr + 1;
    } while (i < CREDITCOUNT);
  }
  return;
}