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

//NGC MATCH
void DrawCredits(void) {
  struct CREDIT_s* credit;
  float xscale;
  float y;
  float size;
  s32 i;
  

  credit = Credit;
  size = GameTimer.ftime * credit_speed + (-1.0f);
    for (i = 0; i < CREDITCOUNT; i++,credit++) {
      xscale = (credit->size / 100.0f);
      if (((credit->txt != NULL) && (size > -1.5f)) && (size < 1.7f)) {
        strcpy(tbuf,credit->txt);
        if (strcmp(tbuf,"SR. MANAGER OF PUBLIC RELATIONS") == 0) {
            y = 0.85f;
        } else {
            y = 0.9f;
        }
        if (Game.language == 0x63) {
          AddSpacesIntoText(tbuf,1);
        }
        Text3D(tbuf, 0.0f, size, 1.0f,  xscale * y, xscale, xscale * 2, 2, credit->colour);
      }
      size = (size - ((xscale * 0.1f) + (xscale * 0.1f)));
    }
  return;
}