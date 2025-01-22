//NGC MATCH
s32 RemapAccentedCharacter(char* c) {
    s32 accent;

    switch (*c) {
        default:
            accent = -1;
            break;
        case -0x1F:
            *c = '\0';
            accent = 6;
            break;
        case -0x79:
        case -0x80:
            *c = 'C';
            accent = 0;
            break;

        // 1
        case -0x24:
        case -0x66:
        case -0x7F:
            *c = 'U';
            accent = 1;
            break;
        case -0x3c:
        case -0x72:
        case -0x7C:
            *c = 'A';
            accent = 1;
            break;
        case -0x77:
        case -0x2D:
            *c = 'E';
            accent = 1;
            break;
        case -0x28:
        case -0x75:
            *c = 'I';
            accent = 1;
            break;
        case -0x67:
        case -0x6C:
            *c = 'O';
            accent = 1;
            break;
        
        // 2
        case -0x2F:
        case -0x5B:
        case -0x5C:
            *c = 'N';
            accent = 2;
            break;
        case -0x39:
        case -0x3A:
            *c = 'A';
            accent = 2;
            break;
        case -0x1B:
        case -0x1C:
            *c = 'O';
            accent = 2;
            break;

        // 3
        case -0x4A:
        case -0x7D:
            *c = 'A';
            accent = 3;
            break;
        case -0x2E:
        case -0x36:
        case -0x78:
            *c = 'E';
            accent = 3;
            break;
        case -0x29:
        case -0x74:
            *c = 'I';
            accent = 3;
            break;
        case -0x1E:
        case -0x6D:
            *c = 'O';
            accent = 3;
            break;
        case -0x16:
        case -0x6A:
            *c = 'U';
            accent = 3;
            break;
        
        // 4
        case -0x37:
        case -0x70:
        case -0x7E:
            *c = 'E';
            accent = 4;
            break;
        case -0x3F:
        case -0x4B:
        case -0x60:
            *c = 'A';
            accent = 4;
            break;
        case -0x20:
        case -0x5E:
            *c = 'O';
            accent = 4;
            break;
        case -0x17:
        case -0x5D:
            *c = 'U';
            accent = 4;
            break;
        case -0x2A:
        case -0x5F:
            *c = 'I';
            accent = 4;
            break;
        case -0x13:
        case -0x14:
            *c = 'Y';
            accent = 4;
            break;
        
        // 5
        case -0x40:
        case -0x49:
        case -0x7B:
            *c = 'A';
            accent = 5;
            break;
        case -0x38:
        case -0x2C:
        case -0x76:
            *c = 'E';
            accent = 5;
            break;
        case -0x22:
        case -0x73:
            *c = 'I';
            accent = 5;
            break;
        case -0x6B:
        case -0x1D:
            *c = 'O';
            accent = 5;
            break;
        case -0x69:
        case -0x15:
            *c = 'U';
            accent = 5;
            break;
    }
    return accent;
}

//NGC MATCH
void Reset3DFontObjects(void) {
  volatile struct objtemp_s *tab;
  struct CharacterModel* model;
  s32 i;
  s32 j;
  
  tab = Font3DObjTab;
  for (i = 0; i < 0x1a; i++, tab++) {
    tab->anim_time = 1.0f;
    if ((tab->flags & 1) != 0) {
          if ((tab->i != -1) && ((tab->action & 0x80U) == 0)) {
                if (tab->action < 'v') {
                        j = CRemap[tab->i];
                      if (j != -1) {
                             model = &CModel[j];
                            if (model->anmdata[tab->action] != 0) {
                                tab->anim_time = 1.0f;
                            }
                      }
                }
          }
    }
  }
  return;
}

//NGC MATCH
void InitFont3D(struct nugscn_s* gscn) {
    struct numtl_s* mtl;
    int i;

    for (i = 0; Font3DTab[i].ascii != 0; i++) {
        Font3DTab[i].obj.scene = NULL;
        Font3DTab[i].obj.special = NULL;
    }
    for (i = 0; i < 256; i++) {
        Font3DRemap[i] = -1;
    }
    for (i = 0; i < 7; i++) {
        (Font3DAccentTab[i].obj).scene = NULL;
        (Font3DAccentTab[i].obj).special = NULL;
    }
    if (gscn != NULL) {
        for (i = 0; Font3DTab[i].ascii != 0; i++) {
            if (NuSpecialFind(gscn, &Font3DTab[i].obj, Font3DTab[i].name) != 0) {
                Font3DRemap[(u8)(Font3DTab[i].ascii)] = (s16)i;
            }
        }
        for (i = 0; i <= 6; i++) {
            NuSpecialFind(gscn, &Font3DAccentTab[i].obj, Font3DAccentTab[i].name);
        }
        for (i = 0; i < gscn->nummtl; i++) {
            mtl = gscn->mtls[i];
            if (mtl->special_id == 1 || mtl->special_id == 2 || mtl->special_id == 3 || mtl->special_id == 4 || mtl->special_id == 5) {
                Font3DMtlTab[mtl->special_id - 1][1] = mtl;
            }
        }
    }
    font3d_initialised = 1;
    return;
}

static char j_bc[2][11];
static char j_bd[2][41];

//NGC MATCH
s32 CombinationCharacterBD(char c0,char c1) {
    char *p;
    
    p = *j_bd;

    for (p = *j_bd; *p != '\0'; p+= 2) {
        if ((c0 == *p) && (c1 == p[1])) {
            return 1;
        }
    }
    return 0;
}

//NGC MATCH
s32 CombinationCharacterBC(char c0,char c1) {
    char *p;
    
    p = *j_bc;
    for (p = *j_bc; *p != '\0'; p+= 2) {
            if ((c0 == *p) && (c1 == p[1])) {
                return 1;
            }
    }
    return 0;
}


//93% NGC
void Text3D(char* txt, float x, float y, float z, float scalex, float scaley, float scalez, s32 align, s32 colour) {
    s32 i;
    s32 j;
    s32 l;
    float xpulsescale;
    float orig_x;
    float dx;
    float dVar8;
    float f28;
    float f;
    float x0;
    float y0;
    float dx_scale;
    volatile FONT3DOBJECT *obj;
    s32 accent;
    char c;
    char c1;
    s32 rot;

    orig_x = x;
    xpulsescale = 1.0f;
    if (font3d_initialised == 0) {
        return;
    }
    if (font3d_scene == NULL) {
        return;
    }

    font3d_dy = font3d_dx = 0.0f;
    font3d_xleft = font3d_xmid = font3d_xright = x;
    font3d_ytop = font3d_ymid = font3d_ybottom = y;

    if (txt == NULL) {
        return;
    }
    
    l = strlen(txt);
    if (l < 1) {
        return;
    }
    
    f28 = dx = 0.0f;
    for (i = 0; txt[i] != 0; i++) {
        if (txt[i] == '#') {
            if (txt[i + 1] != 0) {
                i++;
            }
        } else if (Game.language == 0x63) {
            if (txt[i + 1] != 0) {
                c1 = txt[i + 1];
                c = txt[i];
                f = 1.0f;
                if (c1 == ' ') {
                    if ((c == ':') || (c == '.')) {
                        f = 0.5f;
                    }
                } else if (((c == '8' || c == '9') || (c >= 'A' && c <= 'F')) && ((c1 >= '0' && c1 <= '9') || (c1 >= 'A' && c1 <= 'F')))
                {
                    f = FONT3D_JSCALEDX;
                    if (txt[i + 2] == 'B'
                        && ((txt[i + 3] == 'D' && CombinationCharacterBD(c, c1))
                            || (txt[i + 3] == 0x43 && CombinationCharacterBC(c, c1))))
                    {
                        i = i + 2;
                    }
                }
                f28 += f;
                i++;
            }
        } else {
            if ((txt[i] == ':') || (txt[i] == '.')) {
                f28 += 0.5f;
            } else {
                f28 += 1.0f;
            }
        }
    }
    dx_scale = (scalex * 0.1f);
    i = align & 0x14;
    switch (i) {
        case 0x10:
            x0 = orig_x - ((dx_scale * f28) - (dx_scale * 0.5f));
            break;
        default:
            if (i == 4) {
                x0 = orig_x + (dx_scale * 0.5f);
                break;
            }
            x0 = orig_x - (((dx_scale * f28) * 0.5f) - (dx_scale * 0.5f));
            break;
    }
    x0 -= (dx_scale * 0.5f);
    font3d_xleft = x0;

    if (((disable_safearea_clamp == 0) && (x0 < -0.81f)) && (x0 > -1.3f)) {
        scalex = (scalex * ((-0.81f - x) / (x0 - x)));
        xpulsescale = 0.5f;
    }
    if ((align & 0x20) != 0) {
        scalex = (scalex * ((menu_pulsate - 1.0f) * xpulsescale + 1.0f));
        scaley = (scaley * menu_pulsate);
        scalez = (scalez * menu_pulsate);
    }
    font3d_dx = (scalex * 0.1f);
    x = dx;
    if (i == 0x10) {
        dx_scale = (font3d_dx * f28) - (font3d_dx * 0.5f);
    } else {
        if (i == 4) {
            x = font3d_dx * 0.5f + x;
            goto Skip;
        } else {
            dx_scale = (font3d_dx * f28) * 0.5f - (font3d_dx * 0.5f);
        }
    } 
    x -= dx_scale;
Skip:
    font3d_dy = (scaley * 0.1f) * FONT3DYMUL;
    font3d_xleft = (x - (xpulsescale * 0.5f));
    //x = font3d_dy;
    if ((align & 10U) == 8) {
        y += (font3d_dy * 0.5f);
    } else if ((align & 10U) == 2) {
        y -= (font3d_dy * 0.5f);
    }
    i = ((u32)colour < 5) ? colour : 0;
    nurndr_forced_mtl_table = &Font3DMtlTab[0][i * 2];
    //dVar7 = 1.0f;
    //dVar18 = 4.0f;
    for (j = 0; j < l; j++, txt++) {
        c = *txt;
        if (Game.language == 0x63) {
            if (txt[1] == 0) {
                goto Finish;
            }
            if ((c != '#') && (c != ' ')) {
                goto Skip_2;
            }
        } //else {
            if ((txt[8] >= 0) || (txt[8] == -8) || (txt[8] == -2)) {
            //c = txt[8];
            c1 = txt[8];
                if (c1 == '#') {
                    if (txt[1] != 0) {
                        switch (txt[1]) {
                            case 'o':
                                i = 0;
                                break;
                            case 'w':
                                i = 1;
                                break;
                            case 'c':
                                i = 2;
                                break;
                            case 'b':
                                i = 3;
                                break;
                            case 'g':
                                i = 4;
                                break;
                            default:
                                i = -1;
                                break;
                        }
                        if (i != -1) {
                            nurndr_forced_mtl_table = &Font3DMtlTab[0][c1 * 2];
                        }
                    }
                } else {
                    //i = (s32)c - 0x61;
                    if ((c1 >= 'a' && c1 <= 'z') && (Font3DRemap[(u8)c1] == -1)) {
                        obj = &Font3DObjTab[c1 - 'a'];
                        if (obj->i != -1) {
                            if ((obj->flags & 2) != 0) {
                                    if (ObjTab[obj->i].obj.special != NULL) {
                                        //iVar3 = obj->i;
                                        //dVar8 = obj->scale * scalex;
                                        //dVar9 = obj->scale * scaley;
                                        //fVar1 = obj->scale * scalez;
                                        DrawPanel3DObject(obj->i, x, y, z, (obj->scale * scalex), (obj->scale * scaley), (obj->scale * scalez), 0, 0, 0, ObjTab[obj->i].obj.scene, ObjTab[obj->i].obj.special, 1);
                                        break;
                                    }
                            } else if ((obj->flags & 1) != 0) {
                                    DrawPanel3DCharacter(
                                        obj->i, x, y, z,
                                        (obj->scale * scalex), //FONT3DSIZE * scalex
                                        (obj->scale * scaley),
                                        (obj->scale * scalez), 0, 0, 0,
                                        obj->action,
                                        obj->anim_time, 1
                                    );
                            }
                        }
                    } else {
                        i = (s32)Font3DRemap[c1];
                        if (i == -1) {
                            i = (s32)Font3DRemap[122];
                        }
                        //iVar3 = (s32)c;
                        if (c != ' ') {
                            if (c == 'x' || c == 'y' || c == 'a' || c == 'b' || c == 'w' || c == 'n') {
                                f = 1.0f;
                            } else {
                                f = 4.0f;
                            }

                            if ((c == ':') || (c == '.')) {
                                dVar8 = (x - (xpulsescale * 0.25f));
                            } else {
                                dVar8 = x;
                            }

                            if (c == '\xFE') {
                                dVar8 -= (xpulsescale * 0.2f);
                                y0 = (x * 0.3f + y);
                            } else {
                                y0 = y;
                            }

                            rot = 0;
                            if (j == 0) {
                                rot = (c == '?') ? 0x8000 : 0;
                                if (c == '!') {
                                    rot = 0x8000;
                                }
                            }
                            DrawPanel3DObject(
                                -1, dVar8, y0, z, (FONT3DSIZE * scalex), (FONT3DSIZE * scaley), (FONT3DSIZE * scalez) * f, 0,
                                0, rot, font3d_scene, Font3DTab[i].obj.special, 1
                            );
                        }
                    }
                    // goto LAB_80028414;
                }
            } 
            else if (txt[8] < 0) {
                    accent = RemapAccentedCharacter(&c);
                    if (accent != -1) {
                        if (Font3DRemap[(s32)(c & 0xff)] != -1) {
                            DrawPanel3DObject(
                                -1, x, y, z, (FONT3DSIZE * scalex), (FONT3DSIZE * scaley), ((FONT3DSIZE * scalez) * 4.0f), 0, 0,
                                0, font3d_scene, Font3DTab[Font3DRemap[(s32)(c & 0xff)]].obj.special, 1
                            );
                        }
                        //dVar9 = FONT3DSIZE * scaley;
                        //iVar3 = -1;
                        //fVar1 = (FONT3DSIZE * scalez) * 6.0f;
                        //dVar8 = FONT3DSIZE * scalex;
                    // LAB_800283c0:
                        DrawPanel3DObject(-1, x, y, z, FONT3DSIZE * scalex, (FONT3DSIZE * scaley), ((FONT3DSIZE * scalez) * 6.0f), 0, 0, 0, font3d_scene, Font3DAccentTab[accent].obj.special, 1);
                        break;
                    } else {
                        DrawPanel3DObject(
                            -1, x, y, z, (FONT3DSIZE * scalex), (FONT3DSIZE * scaley), ((FONT3DSIZE * scalez) * 4.0f), 0, 0, 0,
                            font3d_scene, Font3DTab[Font3DRemap[122]].obj.special, 1
                        );
                        //break;
                    }
                }
Skip_2:
                if ((c == ':') || (c == '.')) {
                    dVar8 += (xpulsescale * 0.5f) * x;
                } else {
                    dVar8 += (xpulsescale * 1.0f) * x;
                }
                if (Game.language == 0x63) {
                    txt++;
                    j++;
                }
            
       // }
    }
Finish:
    font3d_xright = (dVar8 - (xpulsescale * 0.5f));
    font3d_xmid = (font3d_xleft + font3d_xright) * 0.5f;
    font3d_ytop = (y + (x * 0.5f));
    font3d_ybottom = (y - (x * 0.5f));
    font3d_ymid = (font3d_ytop + font3d_ybottom) * 0.5f;
    nurndr_forced_mtl_table = NULL;
}

void Text3D2(char *txt,float x,float y,float z,float scalex,float scaley,float scalez,int align,
            int colour)

{
  char c1;
  byte bVar1;
  float fVar2;
  int iVar3;
  ushort zrot;
  int iVar4;
  int iVar5;
  uint uVar6;
  int iVar7;
  double dVar8;
  double dVar9;
  double dVar10;
  double dVar11;
  double dVar12;
  double dVar13;
  double dVar14;
  double dVar15;
  double dVar16;
  double dVar17;
  double dVar18;
  double dVar19;
  double dVar20;
  double dVar21;
  double dVar22;
  byte local_120 [120];
  double local_a8;
  
  dVar16 = (double)scalez;
  dVar17 = (double)scaley;
  dVar18 = (double)scalex;
  dVar11 = (double)z;
  dVar19 = (double)y;
  dVar15 = (double)x;
  rrrt = rrrt + rrri;
  dVar13 = 1.0;
  if (font3d_initialised == 0) {
    return;
  }
  if (font3d_scene == (nugscn_s *)0x0) {
    return;
  }
  dVar22 = 0.0;
  font3d_dx = 0.0;
  font3d_dy = 0.0;
  if (txt == (char *)0x0) {
    font3d_xright = x;
    font3d_xmid = x;
    font3d_dx = 0.0;
    font3d_dy = 0.0;
    font3d_xleft = x;
    font3d_ybottom = y;
    font3d_ymid = y;
    font3d_ytop = y;
    return;
  }
  dVar8 = dVar15;
  font3d_xright = x;
  font3d_xmid = x;
  font3d_xleft = x;
  font3d_ybottom = y;
  font3d_ymid = y;
  font3d_ytop = y;
  iVar3 = strlen(txt);
  if (iVar3 < 1) {
    return;
  }
  iVar5 = 0;
  local_120[0] = *txt;
  while (local_120[0] != 0) {
    if (local_120[0] == 0x23) {
      if (txt[iVar5 + 1] != '\0') {
        iVar5 = iVar5 + 1;
      }
    }
    else if (Game.language == 'c') {
      c1 = txt[iVar5 + 1];
      iVar7 = (int)c1;
      if (iVar7 != 0) {
        dVar20 = 1.0;
        if (iVar7 == 0x20) {
          if ((local_120[0] == 0x3a) || (local_120[0] == 0x2e)) {
            dVar20 = 0.5;
          }
        }
        else if ((((((int)(char)local_120[0] - 0x38U < 2) || ((int)(char)local_120[0] - 0x41U < 6) )
                  && ((iVar7 - 0x30U < 10 || (iVar7 - 0x41U < 6)))) &&
                 (dVar20 = (double)FONT3D_JSCALEDX, txt[iVar5 + 2] == 'B')) &&
                (((txt[iVar5 + 3] == 'D' &&
                  (iVar7 = CombinationCharacterBD(local_120[0],c1), iVar7 != 0)) ||
                 ((txt[iVar5 + 3] == 'C' &&
                  (iVar7 = CombinationCharacterBC(local_120[0],c1), iVar7 != 0)))))) {
          iVar5 = iVar5 + 2;
        }
        dVar22 = (double)(float)(dVar22 + dVar20);
        iVar5 = iVar5 + 1;
      }
    }
    else {
      if ((local_120[0] == 0x3a) || (local_120[0] == 0x2e)) {
        fVar2 = 0.5;
      }
      else {
        fVar2 = 1.0;
      }
      dVar22 = (double)(float)(dVar22 + (double)fVar2);
    }
    iVar5 = iVar5 + 1;
    local_120[0] = txt[iVar5];
  }
  uVar6 = align & 0x14;
  dVar20 = (double)(float)(dVar18 * 0.1000000014901161);
  if (uVar6 == 0x10) {
    dVar14 = dVar20 * dVar22 - (double)(float)(dVar20 * 0.5);
LAB_80028784:
    dVar8 = dVar8 - (double)(float)dVar14;
  }
  else {
    if (uVar6 != 4) {
      dVar14 = (double)(float)(dVar20 * dVar22) * 0.5 - (double)(float)(dVar20 * 0.5);
      goto LAB_80028784;
    }
    dVar8 = dVar20 * 0.5 + dVar8;
  }
  dVar8 = (double)((float)dVar8 - (float)(dVar20 * 0.5));
  if ((dVar8 < -0.8100000023841858) && (-1.299999952316284 < dVar8)) {
    dVar18 = (double)(float)(dVar18 * (double)((float)(-0.8100000023841858 - dVar15) /
                                              (float)(dVar8 - dVar15)));
    dVar13 = 0.5;
  }
  if ((align & 0x20U) != 0) {
    dVar8 = (double)menu_pulsate;
    dVar16 = (double)(float)(dVar16 * dVar8);
    dVar18 = (double)(float)(dVar18 * (double)(float)((double)(float)(dVar8 - 1.0) * dVar13 + 1.0) );
    dVar17 = (double)(float)(dVar17 * dVar8);
  }
  font3d_dx = (float)(dVar18 * 0.1000000014901161);
  dVar13 = (double)font3d_dx;
  if (uVar6 == 0x10) {
    dVar22 = dVar13 * dVar22 - (double)(float)(dVar13 * 0.5);
  }
  else {
    if (uVar6 == 4) {
      dVar15 = dVar13 * 0.5 + dVar15;
      goto LAB_80028858;
    }
    dVar22 = (double)(float)(dVar13 * dVar22) * 0.5 - (double)(float)(dVar13 * 0.5);
  }
  dVar15 = dVar15 - (double)(float)dVar22;
LAB_80028858:
  dVar22 = (double)(float)dVar15;
  font3d_dy = (float)(dVar17 * 0.1000000014901161) * FONT3DYMUL;
  dVar15 = (double)font3d_dy;
  font3d_xleft = (float)(dVar22 - (double)(float)(dVar13 * 0.5));
  if ((align & 10U) == 8) {
    dVar19 = (double)(float)(dVar15 * 0.5 + dVar19);
  }
  else if ((align & 10U) == 2) {
    dVar19 = (double)(float)(dVar19 - (double)(float)(dVar15 * 0.5));
  }
  iVar5 = 0;
  if ((uint)colour < 5) {
    iVar5 = colour;
  }
  iVar7 = 0;
  nurndr_forced_mtl_table = Font3DMtlTab + iVar5 * 2;
  if (0 < iVar3) {
    dVar8 = 1.0;
    dVar14 = 4503601774854144.0;
    dVar21 = 32768.0;
    dVar20 = 4.0;
    do {
      local_120[0] = *txt;
      if (Game.language == 'c') {
        if (((byte *)txt)[1] == 0) break;
        if ((local_120[0] == 0x23) || (((byte *)txt)[1] == 0x20)) goto LAB_80028960;
LAB_80028dd8:
        if ((local_120[0] == 0x3a) || (dVar9 = dVar13, local_120[0] == 0x2e)) {
          dVar9 = (double)(float)(dVar13 * 0.5);
        }
        dVar22 = (double)(float)(dVar9 * dVar8 + dVar22);
        if (Game.language == 'c') {
LAB_80028e10:
          txt = (char *)((byte *)txt + 1);
          iVar7 = iVar7 + 1;
        }
      }
      else {
LAB_80028960:
        if (((char)local_120[0] < '\0') && ((local_120[0] != 0xf8 && (local_120[0] != 0xfe)))) {
          if ((char)local_120[0] < '\0') {
            iVar5 = RemapAccentedCharacter((char *)local_120);
            if (iVar5 == -1) {
              dVar9 = (double)FONT3DSIZE;
              DrawPanel3DObject(-1,(float)dVar22,(float)dVar19,(float)dVar11,(float)(dVar9 * dVar1 8)
                                ,(float)(dVar9 * dVar17),
                                (float)((double)(float)(dVar9 * dVar16) * dVar20),0,0,0,font3d_scen e
                                ,Font3DTab[Font3DRemap[122]].obj.special,1);
            }
            else {
              if (Font3DRemap[local_120[0]] != -1) {
                dVar9 = (double)FONT3DSIZE;
                DrawPanel3DObject(-1,(float)dVar22,(float)dVar19,(float)dVar11,
                                  (float)(dVar9 * dVar18),(float)(dVar9 * dVar17),
                                  (float)((double)(float)(dVar9 * dVar16) * dVar20),0,0,0,
                                  font3d_scene,Font3DTab[Font3DRemap[local_120[0]]].obj.special,1) ;
              }
              dVar9 = (double)FONT3DSIZE;
              DrawPanel3DObject(-1,(float)dVar22,(float)dVar19,(float)dVar11,(float)(dVar9 * dVar1 8)
                                ,(float)(dVar9 * dVar17),(float)(dVar9 * dVar16) * 6.0,0,0,0,
                                font3d_scene,Font3DAccentTab[iVar5].obj.special,1);
            }
          }
          goto LAB_80028dd8;
        }
        iVar5 = (int)(char)local_120[0];
        if (iVar5 != 0x23) {
          if ((iVar5 - 0x61U < 0x1a) && (Font3DRemap[local_120[0]] == -1)) {
            if (*(short *)&(&H2OElec[0x13].start)[iVar5].z != -1) {
              if ((*(byte *)((int)&(&H2OElec[0x13].start)[iVar5].z + 2) & 2) == 0) {
                if ((*(byte *)((int)&(&H2OElec[0x13].start)[iVar5].z + 2) & 1) != 0) {
                  local_a8 = (double)CONCAT44(0x43300000,(int)rrrt ^ 0x80000000);
                  iVar4 = (int)(dVar19 * dVar21 +
                               (double)(float)(dVar22 * dVar21 + (double)(float)(local_a8 - dVar14 ))
                               );
                  local_a8 = (double)(longlong)iVar4;
                  DrawPanel3DCharacter
                            ((int)*(short *)&(&H2OElec[0x13].start)[iVar5].z,(float)dVar22,
                             (float)dVar19,(float)dVar11,
                             (float)((double)(&H2OElec[0x13].end)[iVar5].y * dVar18),
                             (float)((double)(&H2OElec[0x13].end)[iVar5].y * dVar17),
                             (float)((double)(&H2OElec[0x13].end)[iVar5].y * dVar16),0,(ushort)iVa r4
                             ,0,(int)*(char *)((int)&(&H2OElec[0x13].start)[iVar5].z + 3),
                             (&H2OElec[0x13].end)[iVar5].x,1);
                }
              }
              else if (ObjTab[*(short *)&(&H2OElec[0x13].start)[iVar5].z].obj.special !=
                       (nuspecial_s *)0x0) {
                local_a8 = (double)CONCAT44(0x43300000,(int)rrrt ^ 0x80000000);
                iVar4 = (int)(dVar19 * dVar21 +
                             (double)(float)(dVar22 * dVar21 + (double)(float)(local_a8 - dVar14)) );
                local_a8 = (double)(longlong)iVar4;
                DrawPanel3DObject((int)*(short *)&(&H2OElec[0x13].start)[iVar5].z,(float)dVar22,
                                  (float)dVar19,(float)dVar11,
                                  (float)((double)(&H2OElec[0x13].end)[iVar5].y * dVar18),
                                  (float)((double)(&H2OElec[0x13].end)[iVar5].y * dVar17),
                                  (float)((double)(&H2OElec[0x13].end)[iVar5].y * dVar16),0,
                                  (ushort)iVar4,0,
                                  ObjTab[*(short *)&(&H2OElec[0x13].start)[iVar5].z].obj.scene,
                                  ObjTab[*(short *)&(&H2OElec[0x13].start)[iVar5].z].obj.special,1 );
              }
            }
          }
          else {
            iVar5 = (int)Font3DRemap[local_120[0]];
            if (iVar5 == -1) {
              iVar5 = (int)Font3DRemap[122];
            }
            iVar4 = (int)(char)local_120[0];
            if (iVar4 != 0x20) {
              if ((((iVar4 - 0x78U < 2) || (iVar4 == 0x61)) || (iVar4 == 0x62)) ||
                 ((iVar4 == 0x77 || (iVar4 == 0x6e)))) {
                fVar2 = 1.0;
              }
              else {
                fVar2 = 4.0;
              }
              if ((local_120[0] == 0x3a) || (dVar9 = dVar22, local_120[0] == 0x2e)) {
                dVar9 = (double)(float)(dVar22 - (double)(float)(dVar13 * 0.25));
              }
              dVar10 = dVar19;
              if (local_120[0] == 0xfe) {
                dVar9 = (double)(float)(dVar9 - (double)(float)(dVar13 * 0.2000000029802322));
                dVar10 = (double)(float)(dVar15 * 0.300000011920929 + dVar19);
              }
              zrot = 0;
              if ((iVar7 == 0) &&
                 (zrot = -(ushort)(local_120[0] == 0x3f) & 0x8000, local_120[0] == 0x21)) {
                zrot = 0x8000;
              }
              dVar12 = (double)FONT3DSIZE;
              DrawPanel3DObject(-1,(float)dVar9,(float)dVar10,(float)dVar11,(float)(dVar12 * dVar1 8)
                                ,(float)(dVar12 * dVar17),(float)(dVar12 * dVar16) * fVar2,0,0,zrot ,
                                font3d_scene,Font3DTab[iVar5].obj.special,1);
            }
          }
          goto LAB_80028dd8;
        }
        bVar1 = ((byte *)txt)[1];
        if (bVar1 != 0) {
          if (bVar1 == 0x67) {
            iVar5 = 4;
          }
          else if ((char)bVar1 < 'h') {
            if (bVar1 == 0x62) {
              iVar5 = 3;
            }
            else if (bVar1 == 99) {
              iVar5 = 2;
            }
            else {
LAB_800289fc:
              iVar5 = -1;
            }
          }
          else if (bVar1 == 0x6f) {
            iVar5 = 0;
          }
          else {
            if (bVar1 != 0x77) goto LAB_800289fc;
            iVar5 = 1;
          }
          if (iVar5 != -1) {
            nurndr_forced_mtl_table = Font3DMtlTab + iVar5 * 2;
          }
          goto LAB_80028e10;
        }
      }
      iVar7 = iVar7 + 1;
      txt = (char *)((byte *)txt + 1);
    } while (iVar7 < iVar3);
  }
  font3d_ybottom = (float)(dVar19 - (double)(float)(dVar15 * 0.5));
  font3d_ytop = (float)(dVar19 + (double)(float)(dVar15 * 0.5));
  font3d_xright = (float)(dVar22 - (double)(float)(dVar13 * 0.5));
  font3d_ymid = (font3d_ytop + font3d_ybottom) * 0.5;
  font3d_xmid = (font3d_xleft + font3d_xright) * 0.5;
  nurndr_forced_mtl_table = (numtl_s **)0x0;
  return;
}