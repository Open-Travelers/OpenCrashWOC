extern char* LevelFileName;
extern void* Chase[3]; //struct chase_s
s32 POINTCOUNT;
s32 AMBIENTCOUNT;
s32 LIGHTCOUNT;
s32 DIRECTCOUNT;

struct glowlight_s
{
    struct nucolour3_s HighColour;
    struct nucolour3_s LowColour;
    struct nucolour3_s CurColour;
    struct nucolour3_s ColDiff;
    u16 Step;
    u16 CurAngle;
};

// Size: 0x48
struct lights_s
{
    s32 type;
    struct nuvec_s pos;
    struct nuvec_s radius_pos;
    f32 radius;
    u8 r;
    u8 g;
    u8 b;
    u8 glow;
    struct nucolour3_s colour;
    struct nuvec_s direction;
    u8 globalflag;
    u8 blendtype;
    u8 brightness;
    struct glowlight_s* glowlight;
    u8 pad2;
};

struct lights_s Lights[320];

//94% NGC
void LoadLights(void) {
    s32 handle;
    s32 i;
    void* fbuff;
    s32 fsize;

    POINTCOUNT = 0;
    LIGHTCOUNT = 0;
    AMBIENTCOUNT = 0;
    DIRECTCOUNT = 0;
    for (fsize = 0; fsize < 0x100; fsize++) {
        Lights[fsize].type = -1;
    }
    sprintf(tbuf, "%s.lgt", LevelFileName);
    i = 0x100;
    if (NuFileExists(tbuf) != 0) {
        fbuff = Chase;
        fsize = NuFileLoadBuffer(tbuf, fbuff, 0x7fffffff);
        handle = NuMemFileOpen(fbuff, fsize, NUFILE_READ);
        if (handle != 0) {
            LIGHTCOUNT = NuFileReadInt(handle);
            AMBIENTCOUNT = NuFileReadInt(handle);
            DIRECTCOUNT = NuFileReadInt(handle);
            POINTCOUNT = NuFileReadInt(handle);
            if (0x100 < LIGHTCOUNT) {
                LIGHTCOUNT = 0x100;
            }
            for (i = 0; i < LIGHTCOUNT; i++) {
                Lights[i].type = NuFileReadInt(handle);
                Lights[i].pos.x = NuFileReadFloat(handle);
                (Lights[i].pos.y) = NuFileReadFloat(handle);
                (Lights[i].pos.z) = NuFileReadFloat(handle);
                Lights[i].radius_pos.x = NuFileReadFloat(handle);
                Lights[i].radius_pos.y = NuFileReadFloat(handle);
                Lights[i].radius_pos.z = NuFileReadFloat(handle);
                Lights[i].radius = NuFileReadFloat(handle);
                Lights[i].r = NuFileReadChar(handle);
                Lights[i].g = NuFileReadChar(handle);
                Lights[i].b = NuFileReadChar(handle);
                Lights[i].colour.r = NuFileReadFloat(handle);
                Lights[i].colour.g = NuFileReadFloat(handle);
                Lights[i].colour.b = NuFileReadFloat(handle);
                if (Lights[i].type == 1 || Lights[i].type == 2) {
                    Lights[i].direction.x = NuFileReadFloat(handle);
                    Lights[i].direction.y = NuFileReadFloat(handle);
                    i++;i--;
                    Lights[i].direction.z = NuFileReadFloat(handle);
                }
                Lights[i].globalflag = (u8)NuFileReadInt(handle);
                switch (Lights[i].globalflag) {
                    case 4:
                        Lights[i].brightness = (u8)NuFileReadInt(handle);
                        break;
                    default:
                        Lights[i].brightness = (u8)NuFileReadInt(handle);
                        break;
                }
            }
            NuFileClose(handle);
        }
    }
    return;
}

//NGC MATCH
void UpdateGlobals(struct Nearest_Light_s *nl) {
  s32 i;
  s32 found_amb;
  s32 found_dir;

  found_amb = 0;
  found_dir = 0;
  nl->glbambindex = -1;
  (nl->glbdirectional).Index = -1;
  for(i = 0; (i < LIGHTCOUNT) && (!found_amb || !found_dir); i++) {
        if (((Lights[i].type) == 0) && (Lights[i].globalflag == 4)) {
            nl->glbambindex = i;
            found_amb = 1;
        }
        if ( ((Lights[i].type == 1) || (Lights[i].type == 2)) && (Lights[i].globalflag == 4)) {
            (nl->glbdirectional).Index = i;
            found_dir = 1;
        }
  }
  return;
}

//NGC MATCH
void ResetLights(struct Nearest_Light_s *nl) {
  nl->pDir1st = &nl->dir1;
  nl->pDir2nd = &nl->dir2;
  nl->pDir3rd = &nl->dir3;
  nl->ambientdist = 8000.0f;
  (nl->dir1).Distance = 8000.0f;
  (nl->dir2).Distance = 8000.0f;
  (nl->dir3).Distance = 8000.0f;
  nl->AmbIndex = -1;
  (nl->AmbCol).x = 0.0f;
  (nl->AmbCol).y = 0.0f;
  (nl->AmbCol).z = 0.0f;
  (nl->dir1).Index = -1;
  (nl->dir2).Index = -1;
  (nl->dir3).Index = -1;
  //(nl->dir1).Direction.x = 0.0f;
  (nl->pDir1st->Direction).x = 0.0f;
  (nl->pDir1st->Direction).y = 0.0f;
  (nl->pDir1st->Direction).z = 0.0f;
  (nl->pDir3rd->Direction).x = 0.0f;
  (nl->pDir2nd->Direction).x = 0.0f;
  (nl->pDir2nd->Direction).y = 0.0f;
  (nl->pDir2nd->Direction).z = 0.0f;
  (nl->pDir3rd->Direction).x = 0.0f;
  (nl->pDir3rd->Direction).x = 0.0f;
  (nl->pDir3rd->Direction).y = 0.0f;
  (nl->pDir3rd->Direction).z = 0.0f;
  (nl->pDir3rd->Direction).x = 0.0f;
  nl->negativeindex = -1;
  nl->negativedist = 8000.0f;
  UpdateGlobals(nl);
  return;
}

//NGC MATCH
void ScaleColour(struct nucolour3_s *colour,u8 r,u8 g,u8 b,u8 power) {
  if ((power == 6)) {
    colour->r = ((s32)r) * sf2;
    colour->g = ((s32)g) * sf2;
    colour->b = ((s32)b) * sf2;
  } else if (power == 7) {
    colour->r = ((s32)r) * sf;
    colour->g = ((s32)g) * sf;
    colour->b = ((s32)b) * sf;
    }
  return;
}

//NGC MATCH
void SortLights(struct Nearest_Light_s *nearLgt) {
  struct pdir_s *tptr;
  struct pdir_s *tptr2;
  
  tptr = nearLgt->pDir1st;
  tptr2 = nearLgt->pDir2nd;
  if (tptr->Distance > tptr2->Distance) {
    nearLgt->pDir1st = tptr2;
    nearLgt->pDir2nd = tptr;
  }
  tptr = nearLgt->pDir2nd;
  tptr2 = nearLgt->pDir3rd;
  if (tptr->Distance > tptr2->Distance) {
    nearLgt->pDir2nd = tptr2;
    nearLgt->pDir3rd = tptr;
  }
  tptr2 = nearLgt->pDir1st;
  tptr = nearLgt->pDir2nd;
  if (tptr2->Distance > tptr->Distance) {
      nearLgt->pDir1st = tptr;
      nearLgt->pDir2nd = tptr2;
  }
  return;
}

//NGC MATCH
s32 FindNearestLights(struct nuvec_s *vec,struct Nearest_Light_s *nearest_light,s32 SearchMode) {
  u8 i;
  u8 loop;
  u8 PrevIndex;
  float distance;
  s32 scount;
  struct nuvec_s direction;
  float sfactor;
  
  PrevIndex = (u8)(nearest_light->AmbIndex);
  loop = (u8)(nearest_light->CurLoopIndex);
  scount = 0x10;
  if (LIGHTCOUNT == 0) {
    return 0;
  }
    if ((SearchMode == 0) || (LIGHTCOUNT < 0x10)) {
      scount = LIGHTCOUNT;
    }
    if (nearest_light->pDir1st->Index != -1) {
      nearest_light->pDir1st->Distance = NuVecDist(&Lights[nearest_light->pDir1st->Index].pos,vec,NULL);
    }
    if (nearest_light->pDir2nd->Index != -1) {
      nearest_light->pDir2nd->Distance = NuVecDist(&Lights[nearest_light->pDir2nd->Index].pos,vec,NULL);
    }
    if (nearest_light->pDir3rd->Index != -1) {
      nearest_light->pDir3rd->Distance = NuVecDist(&Lights[nearest_light->pDir3rd->Index].pos,vec,NULL);
    }
    if (nearest_light->negativeindex != -1) {
      nearest_light->negativedist = NuVecDist(&Lights[nearest_light->negativeindex].pos,vec,NULL);
    }
    if (nearest_light->AmbIndex != -1) {
      nearest_light->ambientdist = NuVecDist(&Lights[nearest_light->AmbIndex].pos,vec,NULL);
    }
    SortLights(nearest_light);
   for(i = 0; i < scount; i++, loop++) {
        if ((s32)loop >= LIGHTCOUNT ) {
          loop = 0;
        }
        distance = NuVecDist(&Lights[loop].pos,vec,NULL);
        if (Lights[loop].type == 3) {
          if (distance < nearest_light->negativedist) {
            nearest_light->negativedist = distance;
            nearest_light->negativeindex = loop;
          }
        }
        else if (Lights[loop].type == 0) {
          if ((loop != nearest_light->glbambindex) && (distance < nearest_light->ambientdist)) {
            nearest_light->ambientdist = distance;
            nearest_light->AmbIndex = loop;
          }
        }
        else if ((Lights[loop].type == 1 || Lights[loop].type == 2) && (loop != nearest_light->pDir1st->Index)) {
              if ((loop != nearest_light->pDir2nd->Index) && ((loop != nearest_light->pDir3rd->Index &&
                  (loop != (nearest_light->glbdirectional).Index)))) {
                    if (distance < nearest_light->pDir1st->Distance) {
                      nearest_light->pDir3rd->Index = nearest_light->pDir2nd->Index;
                      nearest_light->pDir2nd->Index = nearest_light->pDir1st->Index;
                      nearest_light->pDir1st->Index = loop;
                      nearest_light->pDir3rd->Distance = nearest_light->pDir2nd->Distance;
                      nearest_light->pDir2nd->Distance = nearest_light->pDir1st->Distance;
                      nearest_light->pDir1st->Distance = distance;
                    }
                    else {
                          if (distance < nearest_light->pDir2nd->Distance) {
                            nearest_light->pDir3rd->Index = nearest_light->pDir2nd->Index;
                            nearest_light->pDir2nd->Index = loop;
                            nearest_light->pDir3rd->Distance = nearest_light->pDir2nd->Distance;
                            nearest_light->pDir2nd->Distance = distance; 
                          }
                          else if (distance < nearest_light->pDir3rd->Distance) {
                                nearest_light->pDir3rd->Index = loop;
                                nearest_light->pDir3rd->Distance = distance; 
                            } 
                    }
              }
        }
    }
    nearest_light->CurLoopIndex = loop;
    if (nearest_light->AmbIndex == (u32)PrevIndex) {
      nearest_light->ambientdist = NuVecDist(&Lights[nearest_light->AmbIndex].pos,vec,NULL);
    }
    if (nearest_light->AmbIndex != -1) {
      if (Lights[nearest_light->AmbIndex].brightness == 6) {
        sfactor = sf2;
      }
      else if (Lights[nearest_light->AmbIndex].brightness == 7) {
        sfactor = sf;
    }
        (nearest_light->AmbCol).x = (float)(sfactor * (Lights[nearest_light->AmbIndex].r));
        (nearest_light->AmbCol).y = (float)(sfactor * (Lights[nearest_light->AmbIndex].g));
        (nearest_light->AmbCol).z = (float)(sfactor * (Lights[nearest_light->AmbIndex].b));
    }
    else if (nearest_light->glbambindex != -1) {
        (nearest_light->AmbCol).x = (float)(sf * Lights[nearest_light->glbambindex].r);
        (nearest_light->AmbCol).y = (float)(sf * Lights[nearest_light->glbambindex].g);
        (nearest_light->AmbCol).z = (float)(sf * Lights[nearest_light->glbambindex].b);
    }
    if (nearest_light->pDir1st->Index != -1) {
          if (Lights[nearest_light->pDir1st->Index].type == 2) {
            NuVecSub(&direction,&Lights[nearest_light->pDir1st->Index].pos,vec);
            NuVecNorm(&direction,&direction);
            (nearest_light->pDir1st->Direction) = direction;
          }
          else {
            (nearest_light->pDir1st->Direction) = Lights[nearest_light->pDir1st->Index].direction;
          }
          ScaleColour(&nearest_light->pDir1st->Colour,Lights[nearest_light->pDir1st->Index].r,Lights[nearest_light->pDir1st->Index].g,Lights[nearest_light->pDir1st->Index].b,
                      Lights[nearest_light->pDir1st->Index].brightness);
    }
    else {
      if (((nearest_light->glbdirectional).Index != -1) && (Lights[(nearest_light->glbdirectional).Index].type == 1 || Lights[(nearest_light->glbdirectional).Index].type == 2)) {
            if (Lights[nearest_light->pDir1st->Index].brightness == 6) {
              sfactor = sf2;
            }
            else if (Lights[nearest_light->pDir1st->Index].brightness == 7) {
              sfactor = sf;
            }
            (nearest_light->pDir1st->Colour).r = (float)(sfactor * Lights[(nearest_light->glbdirectional).Index].r);
            (nearest_light->pDir1st->Colour).g = (float)(sfactor * Lights[(nearest_light->glbdirectional).Index].g);
            (nearest_light->pDir1st->Colour).b = (float)(sfactor * Lights[(nearest_light->glbdirectional).Index].b);
            nearest_light->pDir1st->Distance = 8000.0f;
          }
          else {
            (nearest_light->pDir1st->Colour).r = 0.0f;
            (nearest_light->pDir1st->Colour).g = 0.0f;
            (nearest_light->pDir1st->Colour).b = 0.0f;
            nearest_light->pDir1st->Distance = 8000.0f;
          }
    }
    if (nearest_light->pDir2nd->Index != -1) {
      if (Lights[nearest_light->pDir2nd->Index].type == 2) {
        NuVecSub(&direction,&Lights[nearest_light->pDir2nd->Index].pos,vec);
        NuVecNorm(&direction,&direction);
        (nearest_light->pDir2nd->Direction) = direction;
      }
      else {
        (nearest_light->pDir2nd->Direction) = Lights[nearest_light->pDir2nd->Index].direction;
      }
      ScaleColour(&nearest_light->pDir2nd->Colour,Lights[nearest_light->pDir2nd->Index].r,Lights[nearest_light->pDir2nd->Index].g,Lights[nearest_light->pDir2nd->Index].b,
                  Lights[nearest_light->pDir2nd->Index].brightness);
    }
    else {
      if (((nearest_light->glbdirectional).Index != -1) && (Lights[(nearest_light->glbdirectional).Index].type == 1 || Lights[(nearest_light->glbdirectional).Index].type == 2)) {
         if (Lights[nearest_light->pDir2nd->Index].brightness == 6) {
          sfactor = sf2;
        }
        else if (Lights[nearest_light->pDir2nd->Index].brightness == 7) {
          sfactor = sf;
        }
        (nearest_light->pDir2nd->Colour).r = (float)(sfactor * Lights[(nearest_light->glbdirectional).Index].r);
        (nearest_light->pDir2nd->Colour).g = (float)(sfactor * Lights[(nearest_light->glbdirectional).Index].g);
        (nearest_light->pDir2nd->Colour).b = (float)(sfactor * Lights[(nearest_light->glbdirectional).Index].b);
        nearest_light->pDir2nd->Distance = 8000.0f;
      }
      else {
        (nearest_light->pDir2nd->Colour).r = 0.0f;
        (nearest_light->pDir2nd->Colour).g = 0.0f;
        (nearest_light->pDir2nd->Colour).b = 0.0f;
        nearest_light->pDir2nd->Distance = 8000.0f;
      }
    }
    if (nearest_light->pDir3rd->Index != -1) {
      if (Lights[nearest_light->pDir3rd->Index].type == 2) {
        NuVecSub(&direction,&Lights[nearest_light->pDir3rd->Index].pos,vec);
        NuVecNorm(&direction,&direction);
        (nearest_light->pDir3rd->Direction) = direction;
      }
      else {
        (nearest_light->pDir3rd->Direction) = Lights[nearest_light->pDir3rd->Index].direction;
      }
      ScaleColour(&nearest_light->pDir3rd->Colour,Lights[nearest_light->pDir3rd->Index].r,Lights[nearest_light->pDir3rd->Index].g,Lights[nearest_light->pDir3rd->Index].b,
                  Lights[nearest_light->pDir3rd->Index].brightness);
    }
    else {
          if (((nearest_light->glbdirectional).Index != -1) && (Lights[(nearest_light->glbdirectional).Index].type == 1 || Lights[(nearest_light->glbdirectional).Index].type == 2)) {
            if (Lights[-1].brightness == 6) {
              sfactor = sf2;
            }
            else if (Lights[-1].brightness == 7) {
              sfactor = sf;
            }
            (nearest_light->pDir3rd->Colour).r = (float)(sfactor * Lights[(nearest_light->glbdirectional).Index].r);
            (nearest_light->pDir3rd->Colour).g = (float)(sfactor * Lights[(nearest_light->glbdirectional).Index].g);
            (nearest_light->pDir3rd->Colour).b = (float)(sfactor * Lights[(nearest_light->glbdirectional).Index].b);
            nearest_light->pDir3rd->Distance = 8000.0f;
          }
          else {
            (nearest_light->pDir3rd->Colour).r = 0.0f;
            (nearest_light->pDir3rd->Colour).g = 0.0f;
            (nearest_light->pDir3rd->Colour).b = 0.0f;
            nearest_light->pDir3rd->Distance = 8000.0f;
          }
    }
  return 1;
}

//NGC MATCH
void CalculateSingleLightProportion(struct nuvec_s *vec,struct pdir_s *light,s32 global) {
  float DistFromLight;
  float glb_proportion;
  float loc_proportion;
  struct nuvec_s blend_vec;
  struct nuvec_s loc_vec;
  struct nuvec_s glob_vec;
  float sfactor;
  struct glowlight_s *glight;
  
  glb_proportion = 0.0f;
  if (light->Index != -1) {
        DistFromLight = NuVecDist(&Lights[light->Index].pos,vec,NULL);
        if (DistFromLight <= Lights[light->Index].radius) {
              loc_proportion = (1.0f - (1.0f / Lights[light->Index].radius) * DistFromLight);
              if (global != -1) {
                    glb_proportion = (1.0f - loc_proportion);
                    if (Lights[global].brightness == 6) {
                      glb_proportion = glb_proportion + glb_proportion;
                    }
                    if (Lights[light->Index].brightness == 6) {
                      loc_proportion = (loc_proportion + loc_proportion);
                    }
                    if (Lights[light->Index].glow != 0) {
                      glight = Lights[light->Index].glowlight;
                      (light->Colour).r = Lights[global].r * sf * glb_proportion + ((glight->CurColour).r * loc_proportion);
                      (light->Colour).g = Lights[global].g * sf * glb_proportion + ((glight->CurColour).g * loc_proportion);
                      (light->Colour).b = (Lights[global].b) * sf * glb_proportion + ((glight->CurColour).b * loc_proportion);
                    }
                    else {
                      (light->Colour).r = Lights[global].r * sf * glb_proportion + ((light->Colour).r * loc_proportion);
                      (light->Colour).g = Lights[global].g * sf * glb_proportion + ((light->Colour).g * loc_proportion);
                      (light->Colour).b = Lights[global].b * sf * glb_proportion + ((light->Colour).b * loc_proportion);
                    }
                    loc_vec = (light->Direction);
                    loc_vec.x *= loc_proportion;
                    loc_vec.y *= loc_proportion;
                    loc_vec.z *= loc_proportion;
                    glob_vec = Lights[global].direction;
                    glob_vec.x *= glb_proportion;
                    glob_vec.y *= glb_proportion;
                    glob_vec.z *= glb_proportion;
                    NuVecAdd(&blend_vec,&loc_vec,&glob_vec);
                    if (NuVecNorm(&blend_vec,&blend_vec) > 0.0f) {
                      (light->Direction) = blend_vec;
                    }
              }
              else {
                    if (Lights[light->Index].glow != 0) {
                      glight = Lights[light->Index].glowlight;
                      (light->Colour).r = ((glight->CurColour).r * loc_proportion);
                      (light->Colour).g = ((glight->CurColour).g * loc_proportion);
                      (light->Colour).b = ((glight->CurColour).b * loc_proportion);
                    }
                    else {
                      (light->Colour).r = ((light->Colour).r * loc_proportion);
                      (light->Colour).g = ((light->Colour).g * loc_proportion);
                      (light->Colour).b = ((light->Colour).b * loc_proportion);
                    }
              }
        }
        else {
              if (global != -1) {
                    if (Lights[global].brightness == 6) {
                      sfactor = sf2;
                    }
                    else if (Lights[global].brightness == 7) {
                      sfactor = sf;
                    }
                    (light->Colour).r = Lights[global].r * sfactor;
                    (light->Colour).g = Lights[global].g * sfactor;
                    (light->Colour).b = Lights[global].b * sfactor;
                    (light->Direction) = Lights[global].direction;
              }
              else {
                    (light->Colour).r = glb_proportion;
                    (light->Colour).g = glb_proportion;
                    (light->Colour).b = glb_proportion;
              }
        }
  }
  else {
        if (global != -1) {
          if (Lights[global].brightness == 6) {
            sfactor = sf2;
          }
          else if (Lights[global].brightness == 7) {
            sfactor = sf;
          }
          (light->Colour).r = (Lights[global].r * 0.33f) * sfactor;
          (light->Colour).g = (Lights[global].g * 0.33f) * sfactor;
          (light->Colour).b = (Lights[global].b * 0.33f) * sfactor;
          (light->Direction) = Lights[global].direction;
        }
  }
  return;
}

//NGC MATCH
float FindLightProportion(struct nuvec_s *vec,struct Nearest_Light_s *nearest_light) {
  float glb_proportion;
  float proportion;
  float loc_proportion;
  float DistFromLight;
  float sfactor;
  struct glowlight_s *glight;
  
  //DistFromLight = sf;
  proportion = 0.0f;
  if (nearest_light->AmbIndex != -1) {
    DistFromLight = NuVecDist(&Lights[nearest_light->AmbIndex].pos,vec,NULL);
    if (DistFromLight <= Lights[nearest_light->AmbIndex].radius) {
        loc_proportion = 1.0f - (1.0f / Lights[nearest_light->AmbIndex].radius) * DistFromLight;
        if (nearest_light->glbambindex != -1) {
              glb_proportion = 1.0f - loc_proportion;
              if (Lights[nearest_light->glbambindex].brightness == 6) {
                glb_proportion = glb_proportion + glb_proportion;
              }
              if (Lights[nearest_light->AmbIndex].brightness == 6) {
                loc_proportion = loc_proportion + loc_proportion;
              }
              if (Lights[nearest_light->AmbIndex].glow != 0) {
                glight = Lights[nearest_light->AmbIndex].glowlight;
                (nearest_light->AmbCol).x = (glight->CurColour).r * sf * glb_proportion + (glight->CurColour).r * loc_proportion;
                (nearest_light->AmbCol).y = (glight->CurColour).g * sf * glb_proportion + (glight->CurColour).g * loc_proportion;
                (nearest_light->AmbCol).z = (glight->CurColour).b * sf * glb_proportion + (glight->CurColour).b * loc_proportion;
              }
              else {
                (nearest_light->AmbCol).x = Lights[nearest_light->glbambindex].r * sf * glb_proportion + (nearest_light->AmbCol).x * loc_proportion;
                (nearest_light->AmbCol).y = Lights[nearest_light->glbambindex].g * sf * glb_proportion + (nearest_light->AmbCol).y * loc_proportion;
                (nearest_light->AmbCol).z = Lights[nearest_light->glbambindex].b * sf * glb_proportion + (nearest_light->AmbCol).z * loc_proportion;
              }
        }
        else {
              if (Lights[nearest_light->AmbIndex].glow != 0) {
                  glight = Lights[nearest_light->AmbIndex].glowlight;
                  (nearest_light->AmbCol).x = (glight->CurColour).r * loc_proportion;
                  (nearest_light->AmbCol).y = (glight->CurColour).g * loc_proportion;
                  (nearest_light->AmbCol).z = (glight->CurColour).b * loc_proportion;
              } else {
                (nearest_light->AmbCol).x = (nearest_light->AmbCol).x * loc_proportion;
                (nearest_light->AmbCol).y = (nearest_light->AmbCol).y * loc_proportion;
                (nearest_light->AmbCol).z = (nearest_light->AmbCol).z * loc_proportion;
              }
        }
    } else {
          if (nearest_light->glbambindex != -1) {
                if (Lights[nearest_light->glbambindex].brightness == 6) {
                        sfactor = sf2;
                } else if (Lights[nearest_light->glbambindex].brightness == 7) {
                    sfactor = sf;
                }
                (nearest_light->AmbCol).x = (Lights[nearest_light->glbambindex].r * sfactor);
                (nearest_light->AmbCol).y = (Lights[nearest_light->glbambindex].g * sfactor);
                (nearest_light->AmbCol).z = (Lights[nearest_light->glbambindex].b * sfactor);
          } else {
                (nearest_light->AmbCol).x = proportion;
                (nearest_light->AmbCol).y = proportion;
                (nearest_light->AmbCol).z = proportion;
          }
    }
  }
  else {
    if (nearest_light->glbambindex != -1) {
        if (Lights[nearest_light->glbambindex].brightness == 6) {
                sfactor = sf2;
        } else if (Lights[nearest_light->glbambindex].brightness == 7) {
            sfactor = sf;
        }
        (nearest_light->AmbCol).x = (Lights[nearest_light->glbambindex].r * sfactor);
        (nearest_light->AmbCol).y = (Lights[nearest_light->glbambindex].g * sfactor);
        (nearest_light->AmbCol).z = (Lights[nearest_light->glbambindex].b * sfactor);
    }
  }
  CalculateSingleLightProportion(vec,nearest_light->pDir1st,(nearest_light->glbdirectional).Index);
  CalculateSingleLightProportion(vec,nearest_light->pDir2nd,(nearest_light->glbdirectional).Index);
  CalculateSingleLightProportion(vec,nearest_light->pDir3rd,(nearest_light->glbdirectional).Index);
  if (nearest_light->negativeindex != -1) {
        DistFromLight = NuVecDist(&Lights[nearest_light->negativeindex].pos,vec,NULL);
        if (DistFromLight <= Lights[nearest_light->negativeindex].radius) {
              loc_proportion = (1.0f / Lights[nearest_light->negativeindex].radius) * DistFromLight;
              if (nearest_light->AmbIndex != -1) {
                (nearest_light->AmbCol).x = (nearest_light->AmbCol).x * loc_proportion;
                (nearest_light->AmbCol).y = (nearest_light->AmbCol).y * loc_proportion;
                (nearest_light->AmbCol).z = (nearest_light->AmbCol).z * loc_proportion;
              }
              else {
                (nearest_light->AmbCol).x = Lights[nearest_light->glbambindex].r * sf * loc_proportion;
                (nearest_light->AmbCol).y = Lights[nearest_light->glbambindex].g * sf * loc_proportion;
                (nearest_light->AmbCol).z = Lights[nearest_light->glbambindex].b * sf * loc_proportion;
              }
              if (nearest_light->pDir1st->Index != -1) {
                (nearest_light->pDir1st->Colour).r = (nearest_light->pDir1st->Colour).r * loc_proportion;
                (nearest_light->pDir1st->Colour).g = (nearest_light->pDir1st->Colour).g * loc_proportion;
                (nearest_light->pDir1st->Colour).b = (nearest_light->pDir1st->Colour).b * loc_proportion;
              }
              if (nearest_light->pDir2nd->Index != -1) {
                (nearest_light->pDir2nd->Colour).r = (nearest_light->pDir2nd->Colour).r * loc_proportion;
                (nearest_light->pDir2nd->Colour).g = (nearest_light->pDir2nd->Colour).g * loc_proportion;
                (nearest_light->pDir2nd->Colour).b = (nearest_light->pDir2nd->Colour).b * loc_proportion;
              }
              if (nearest_light->pDir3rd->Index != -1) {
                (nearest_light->pDir3rd->Colour).r = (nearest_light->pDir3rd->Colour).r * loc_proportion;
                (nearest_light->pDir3rd->Colour).g = (nearest_light->pDir3rd->Colour).g * loc_proportion;
                (nearest_light->pDir3rd->Colour).b = (nearest_light->pDir3rd->Colour).b * loc_proportion;
              }
        }
  }
  return proportion;
}

//NGC MATCH
void GetLights(struct nuvec_s *pos,struct Nearest_Light_s *nearest_lights,s32 SearchMode) {
  FindNearestLights(pos,nearest_lights,SearchMode);
  FindLightProportion(pos,nearest_lights);
  return;
}

//NGC MATCH
void RotateDirectionalLight(struct nuvec_s *v,s32 xrot,s32 yrot) {
  NuVecRotateX(v,&v001,xrot);
  NuVecRotateY(v,v,yrot);
  return;
}

//NGC MATCH
void SetLights(struct nucolour3_s *vCOL0,struct nuvec_s *vDIR0,struct nucolour3_s *vCOL1,struct nuvec_s *vDIR1, struct nucolour3_s *vCOL2,struct nuvec_s *vDIR2,struct nuvec_s *vAMB) {
  NuLightSetDirectionalLights(vDIR0,vCOL0,vDIR1,vCOL1,vDIR2,vCOL2);
  NuLightSetAmbientLight(vAMB);
  return;
}

//NGC MATCH
void SetLevelLights() 
{
    if ((GameMode == 1) && (gamecut != 0) && (gamecut != 6) && (gamecut != 11) && (gamecut != 16)) 
    { 
        RotateDirectionalLight(&lcutdir[0], ((GameTimer.frame % 120) * 65536) / 120, ((GameTimer.frame % 120) * 65536) / 120);
        RotateDirectionalLight(&lcutdir[1], ((GameTimer.frame % 120) * 65536) / 120, ((GameTimer.frame % 120) * 65536) / 120);
        RotateDirectionalLight(&lcutdir[2], ((GameTimer.frame % 120) * 65536) / 120, ((GameTimer.frame % 120) * 65536) / 120);
        
        SetLights(&lcutdircol[0], &lcutdir[0], &lcutdircol[1], &lcutdir[1], &lcutdircol[2], &lcutdir[2], &lcutambcol);
    }
    else if (cutmovie == 0) 
    {
        RotateDirectionalLight(&titledir[0], ((GameTimer.frame % 240) * 65536) / 240, ((GameTimer.frame % 390) * 65536) / 390);
        RotateDirectionalLight(&titledir[1], ((GameTimer.frame % 330) * 65536) / 330, ((GameTimer.frame % 450) * 65536) / 450);
        RotateDirectionalLight(&titledir[2], ((GameTimer.frame % 120) * 65536) / 120, ((GameTimer.frame % 150) * 65536) / 150);
        
        SetLights(&titlergb, &titledir[0], &titlergb, &titledir[1], &titlergb2, &titledir[2], (struct nuvec_s*)&titlergb);
    }
    else if ((Level == 43) && (cutmovie == -1)) 
    {
        intensity = NuFabs(NuTrigTable[creditsang[0] & 0xFFFF]);
        
        NuVecRotateY(&creditsdir[0], &ZVec, creditsang[0] / 40.0f);
        
        creditsang[0] += 256;
        
        creditsdircol[0].r = intensity;
        
        intensity = NuFabs(NuTrigTable[creditsang[1] & 0xFFFF]);
        
        NuVecRotateY(&creditsdir[1], &ZVec, -creditsang[1] / 40.0f);
        
        creditsdircol[1].r = intensity;
        
        creditsang[1] += 256;
        
        intensity = NuFabs(NuTrigTable[creditsang[2] & 0xFFFF]);
        
        NuVecRotateX(&creditsdir[2], &ZVec, creditsang[2] / 40.0f);
        
        creditsang[2] += 256;
        
        creditsdircol[2].r = intensity;
        
        NuLightSetDirectionalLights(&creditsdir[0], &creditsdircol[0], &creditsdir[1], &creditsdircol[1], &creditsdir[2], &creditsdircol[2]);
        NuLightSetAmbientLight(&creditsrgb);
    }
    else 
    {
        SetLights(&lcol[0], &ldir[0], &lcol[1], &ldir[1], &lcol[2], &ldir[2], &acol);  
    } 
}

//NGC MATCH
void SetCreatureLights(struct creature_s *c) {
  struct nuvec_s ambcol;
  struct nuvec_s dir[3];
  struct nucolour3_s col[3];
  float f;
  
  if ((USELIGHTS != 0) && (LIGHTCREATURES != 0)) {
    ambcol = (c->lights).AmbCol;
    col[0] = ((c->lights).pDir1st->Colour);
    dir[0] = ((c->lights).pDir1st->Direction);
    col[1] = ((c->lights).pDir2nd->Colour);
    dir[1] = ((c->lights).pDir2nd->Direction);
    col[2] = ((c->lights).pDir3rd->Colour);
    dir[2] = ((c->lights).pDir3rd->Direction);
  }
  else {
    if ((c->obj).dead != 0x11) {
      return;
    }
    ambcol = acol;
    col[0] = lcol[0];
    dir[0] = ldir[0];
    col[1] = lcol[1];
    dir[1] = ldir[1];
    col[2] = lcol[2];
    dir[2] = ldir[2];
  }
  if ((c->obj).dead == 0x11) {
    if ( (c->obj).die_time < (c->obj).die_duration) {
      f = 1.0f - (c->obj).die_time / (c->obj).die_duration;
    }
    else {
      f = 0.0f;
    }
  }
  else {
    f = 1.0f;
  }
  if (f != 1.0f) {
    ambcol.x = ambcol.x * f;
    ambcol.y = ambcol.y * f;
    ambcol.z = ambcol.z * f;
    col[0].r = col[0].r * f;
    col[0].g = col[0].g * f;
    col[0].b = col[0].b * f;
    col[1].r = col[1].r * f;
    col[1].g = col[1].g * f;
    col[1].b = col[1].b * f;
    col[2].r = col[2].r * f;
    col[2].g = col[2].g * f;
    col[2].b = col[2].b * f;
  }
  SetLights(&col[0],dir,&col[1],&dir[1],&col[2],&dir[2],&ambcol);
  return;
}

//NGC MATCH
void SetNearestLights(struct Nearest_Light_s *l) {
  if ((USELIGHTS != 0) && (LIGHTCREATURES != 0)) {
    SetLights(&l->pDir1st->Colour,&l->pDir1st->Direction,&l->pDir2nd->Colour,&l->pDir2nd->Direction,
              &l->pDir3rd->Colour,&l->pDir3rd->Direction,&l->AmbCol);
  }
  return;
}

//NGC MATCH
struct nuvec_s * GetLightPosition(s32 index) {
  return &Lights[index].pos;
}
