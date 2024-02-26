s32 CREDITCOUNT;
struct CREDIT_s Credit [323];
f32 credit_speed;
f32 credit_time;

// Size: 0x8
struct CREDIT_s
{
    char* txt; // Offset: 0x0, DWARF: 0x188E9D
    short colour; // Offset: 0x4, DWARF: 0x188EC6
    short size; // Offset: 0x6, DWARF: 0x188EEF
};

//NGC MATCH
void InitCredits(void) {
  struct CREDIT_s* credit;
  float size;

    credit = Credit;
    size = 0.0f;
    for (CREDITCOUNT = 0; credit->size > 0; CREDITCOUNT++,credit++) {
      size += (((float)credit->size) / 1000.0f) + (((float)credit->size) / 1000.0f);
    }
  size += 2.0f;
  credit_time = size / credit_speed;
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