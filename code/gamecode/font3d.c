int RemapAccentedCharacter(char *c)

{
  char accent;
  
  accent = *c;
  if (accent == -0x4a) {
LAB_8002782c:
    accent = 'A';
    goto LAB_80027830;
  }
  if (accent < -0x49) {
    if (accent == -0x70) {
LAB_8002785c:
      accent = 'E';
LAB_80027860:
      *c = accent;
      return 4;
    }
    if (accent < -0x6f) {
      if (accent == -0x79) {
LAB_800277cc:
        *c = 'C';
        return 0;
      }
      if (accent < -0x78) {
        if (accent == -0x7e) goto LAB_8002785c;
        if (accent < -0x7d) {
          if (accent == -0x80) goto LAB_800277cc;
          if (accent != -0x7f) {
            return -1;
          }
          goto LAB_800277dc;
        }
        if (accent != -0x7c) {
          if (-0x7d < accent) {
            if (accent != -0x7b) {
              return -1;
            }
            goto LAB_80027894;
          }
          goto LAB_8002782c;
        }
      }
      else {
        if (accent == -0x75) {
LAB_800277fc:
          accent = 'I';
          goto LAB_800277e0;
        }
        if (accent < -0x74) {
          if (accent == -0x77) {
LAB_800277f4:
            accent = 'E';
            goto LAB_800277e0;
          }
          if (accent < -0x76) goto LAB_8002783c;
          goto LAB_800278a4;
        }
        if (accent == -0x73) goto LAB_800278ac;
        if (accent < -0x73) goto LAB_80027844;
        if (accent != -0x72) {
          return -1;
        }
      }
LAB_800277ec:
      accent = 'A';
      goto LAB_800277e0;
    }
    if (accent != -0x66) {
      if (-0x66 < accent) {
        if (accent == -0x5d) goto LAB_8002787c;
        if (accent < -0x5c) {
          if (accent == -0x5f) {
LAB_80027884:
            accent = 'I';
            goto LAB_80027860;
          }
          if (-0x5f < accent) goto LAB_80027874;
          if (accent != -0x60) {
            return -1;
          }
        }
        else {
          if (accent < -0x5a) {
LAB_8002780c:
            accent = 'N';
LAB_80027810:
            *c = accent;
            return 2;
          }
          if (accent != -0x4b) {
            return -1;
          }
        }
LAB_8002786c:
        accent = 'A';
        goto LAB_80027860;
      }
      if (accent != -0x6b) {
        if (accent < -0x6a) {
          if (accent == -0x6d) goto LAB_8002784c;
          if (accent != -0x6c) {
            return -1;
          }
        }
        else {
          if (accent == -0x69) {
LAB_800278bc:
            *c = 'U';
            return 5;
          }
          if (accent < -0x69) goto LAB_80027854;
          if (accent != -0x67) {
            return -1;
          }
        }
        accent = 'O';
        goto LAB_800277e0;
      }
LAB_800278b4:
      accent = 'O';
      goto LAB_80027898;
    }
  }
  else {
    if (accent == -0x29) {
LAB_80027844:
      accent = 'I';
      goto LAB_80027830;
    }
    if (accent < -0x28) {
      if (accent == -0x38) {
LAB_800278a4:
        accent = 'E';
LAB_80027898:
        *c = accent;
        return 5;
      }
      if (accent < -0x37) {
        if (accent != -0x3f) {
          if (accent < -0x3e) {
            if ((accent != -0x49) && (accent != -0x40)) {
              return -1;
            }
LAB_80027894:
            accent = 'A';
            goto LAB_80027898;
          }
          if (accent != -0x3c) {
            if ((accent < -0x3c) || (accent < -0x3a)) {
              return -1;
            }
            accent = 'A';
            goto LAB_80027810;
          }
          goto LAB_800277ec;
        }
        goto LAB_8002786c;
      }
      if (accent != -0x2e) {
        if (-0x2e < accent) {
          if (accent != -0x2c) {
            if (accent < -0x2c) goto LAB_800277f4;
            if (accent != -0x2a) {
              return -1;
            }
            goto LAB_80027884;
          }
          goto LAB_800278a4;
        }
        if (accent != -0x36) {
          if (-0x37 < accent) {
            if (accent != -0x2f) {
              return -1;
            }
            goto LAB_8002780c;
          }
          goto LAB_8002785c;
        }
      }
LAB_8002783c:
      accent = 'E';
LAB_80027830:
      *c = accent;
      return 3;
    }
    if (accent == -0x1d) goto LAB_800278b4;
    if (-0x1d < accent) {
      if (accent == -0x16) {
LAB_80027854:
        accent = 'U';
        goto LAB_80027830;
      }
      if (-0x16 < accent) {
        if (accent == -0x15) goto LAB_800278bc;
        if (-0x13 < accent) {
          return -1;
        }
        accent = 'Y';
        goto LAB_80027860;
      }
      if (accent < -0x1a) {
        accent = 'O';
        goto LAB_80027810;
      }
      if (accent != -0x17) {
        return -1;
      }
LAB_8002787c:
      accent = 'U';
      goto LAB_80027860;
    }
    if (accent == -0x22) {
LAB_800278ac:
      accent = 'I';
      goto LAB_80027898;
    }
    if (-0x22 < accent) {
      if (accent == -0x1f) {
        *c = '\0';
        return 6;
      }
      if (accent < -0x1e) {
        if (accent != -0x20) {
          return -1;
        }
LAB_80027874:
        accent = 'O';
        goto LAB_80027860;
      }
LAB_8002784c:
      accent = 'O';
      goto LAB_80027830;
    }
    if (accent == -0x28) goto LAB_800277fc;
    if (accent != -0x24) {
      return -1;
    }
  }
LAB_800277dc:
  accent = 'U';
LAB_800277e0:
  *c = accent;
  return 1;
}



void Reset3DFontObjects(void)

{
  Font3DObjTab *tab;
  int i;
  
  i = 0x1a;
  tab = Font3DObjTab;
  do {
    tab->anim_time = 1.0;
    if ((tab->flags & 1) != 0) {
      if ((tab->i != -1) && ((tab->action & 0x80U) == 0)) {
        if (tab->action < 'v') {
          if (CRemap[tab->i] != -1) {
            if (*(int *)(CRemap[tab->i] * 0x7ac + -0x7fdac120 + tab->action * 4) != 0) {
              tab->anim_time = 1.0;
            }
          }
        }
      }
    }
    tab = tab + 1;
    i = i + -1;
  } while (i != 0);
  return;
}



void InitFont3D(nugscn_s *gscn)		//TODO
{
  int find;
  short *psVar1;
  uint uVar2;
  numtl_s *mtl;
  short sVar3;
  int i;
  nuhspecial_s *hspec;
  char **name;
  Font3DAccentTab *ptr;
  char *ascii;
  Font3DAccentTab *tab;
  
  if (Font3DTab[0].ascii != '\0') {
    i = 0;
    do {
      *(undefined4 *)((int)&Font3DTab[0].obj.scene + i) = 0;
      *(undefined4 *)((int)&Font3DTab[0].obj.special + i) = 0;
      ascii = &Font3DTab[1].ascii + i;
      i = i + 0x10;
    } while (*ascii != '\0');
  }
  i = 0x100;
  psVar1 = Font3DRemap + 0xff;
  do {
    *psVar1 = -1;
    psVar1 = psVar1 + -1;
    i = i + -1;
  } while (i != 0);
  i = 7;
  tab = Font3DAccentTab;
  do {
    (tab->obj).scene = (nugscn_s *)0x0;
    (tab->obj).special = (nuspecial_s *)0x0;
    tab = tab + 1;
    i = i + -1;
  } while (i != 0);
  if (gscn != (nugscn_s *)0x0) {
    sVar3 = 0;
    if (Font3DTab[0].ascii != '\0') {
      hspec = &Font3DTab[0].obj;
      i = 0;
      do {
        find = NuSpecialFind(gscn,hspec,*(char **)((int)&Font3DTab[0].name + i));
        if (find != 0) {
          Font3DRemap[(byte)(&Font3DTab[0].ascii)[i]] = sVar3;
        }
        hspec = hspec + 2;
        ascii = &Font3DTab[1].ascii + i;
        sVar3 = sVar3 + 1;
        i = i + 0x10;
      } while (*ascii != '\0');
    }
    name = &Font3DAccentTab[0].name;
    tab = Font3DAccentTab;
    do {
      ptr = tab + 1;
      NuSpecialFind(gscn,&tab->obj,*name);
      name = name + 3;
      tab = ptr;
    } while ((int)ptr < -0x7fec95cb);
    i = 0;
    if (0 < gscn->nummtl) {
      find = 0;
      do {
        mtl = *(numtl_s **)(find + (int)gscn->mtls);
        uVar2 = (uint)mtl->special_id;
        if (uVar2 - 1 < 5) {
          Font3DMtlTab[-1][uVar2 * 2 + 4] = mtl;
        }
        i = i + 1;
        find = find + 4;
      } while (i < gscn->nummtl);
    }
  }
  font3d_initialised = 1;
  return;
}

static char j_bc[2][11];
static char j_bd[2][41];

s32 CombinationCharacterBD(char c0,char c1) {
    char *p;
    
    p = (char *)j_bd;

    for (p = (char *)j_bd; *p != '\0'; p+= 2) {
        if ((c0 == (int)*p) && (c1 == (int)p[1])) {
            return 1;
        }
    }
    return 0;
}


s32 CombinationCharacterBC(char c0,char c1) {
    char *p;
    
    p = (char *)j_bc;
    for (p = (char *)j_bc; *p != '\0'; p+= 2) {
            if ((c0 == (int)*p) &&
               (c1 == (int)p[1])) {
                return 1;
            }
    }
    return 0;
}



void Text3D(char *txt,float x,float y,float z,float scalex,float scaley,float scalez,int align,
           int colour)

{
  float fVar1;
  char c1;
  byte bVar2;
  int size_str;
  int iVar3;
  ushort zrot;
  nugscn_s *scn;
  nuspecial_s *obj;
  int iVar4;
  uint uVar5;
  int iVar6;
  double dVar7;
  double dVar8;
  double dVar9;
  double dVar10;
  double dVar11;
  double dVar12;
  double dVar13;
  double dVar14;
  double dVar15;
  double posY__1;
  double dVar16;
  double dVar17;
  double dVar18;
  byte local_e8 [96];
  
  dVar13 = (double)scalez;
  dVar14 = (double)scaley;
  dVar15 = (double)scalex;
  dVar10 = (double)z;
  posY__1 = (double)y;
  dVar16 = (double)x;
  dVar12 = 1.0;
  if (font3d_initialised == 0) {
    return;
  }
  if (font3d_scene == (nugscn_s *)0x0) {
    return;
  }
  dVar17 = 0.0;
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
  dVar7 = dVar16;
  font3d_xright = x;
  font3d_xmid = x;
  font3d_xleft = x;
  font3d_ybottom = y;
  font3d_ymid = y;
  font3d_ytop = y;
  size_str = strlen(txt);
  if (size_str < 1) {
    return;
  }
  iVar4 = 0;
  local_e8[0] = *txt;
  while (local_e8[0] != 0) {
    if (local_e8[0] == 0x23) {
      if (txt[iVar4 + 1] != '\0') {
        iVar4 = iVar4 + 1;
      }
    }
    else if (Game.language == 'c') {
      c1 = txt[iVar4 + 1];
      iVar6 = (int)c1;
      if (iVar6 != 0) {
        dVar18 = 1.0;
        if (iVar6 == 0x20) {
          if ((local_e8[0] == 0x3a) || (local_e8[0] == 0x2e)) {
            dVar18 = 0.5;
          }
        }
        else if ((((((int)(char)local_e8[0] - 0x38U < 2) || ((int)(char)local_e8[0] - 0x41U < 6))  &&
                  ((iVar6 - 0x30U < 10 || (iVar6 - 0x41U < 6)))) &&
                 (dVar18 = (double)FONT3D_JSCALEDX, txt[iVar4 + 2] == 'B')) &&
                (((txt[iVar4 + 3] == 'D' &&
                  (iVar6 = CombinationCharacterBD(local_e8[0],c1), iVar6 != 0)) ||
                 ((txt[iVar4 + 3] == 'C' &&
                  (iVar6 = CombinationCharacterBC(local_e8[0],c1), iVar6 != 0)))))) {
          iVar4 = iVar4 + 2;
        }
        dVar17 = (double)(float)(dVar17 + dVar18);
        iVar4 = iVar4 + 1;
      }
    }
    else {
      if ((local_e8[0] == 0x3a) || (local_e8[0] == 0x2e)) {
        fVar1 = 0.5;
      }
      else {
        fVar1 = 1.0;
      }
      dVar17 = (double)(float)(dVar17 + (double)fVar1);
    }
    iVar4 = iVar4 + 1;
    local_e8[0] = txt[iVar4];
  }
  uVar5 = align & 0x14;
  dVar18 = (double)(float)(dVar15 * 0.1000000014901161);
  if (uVar5 == 0x10) {
    dVar8 = dVar18 * dVar17 - (double)(float)(dVar18 * 0.5);
LAB_80027e40:
    dVar7 = dVar7 - (double)(float)dVar8;
  }
  else {
    if (uVar5 != 4) {
      dVar8 = (double)(float)(dVar18 * dVar17) * 0.5 - (double)(float)(dVar18 * 0.5);
      goto LAB_80027e40;
    }
    dVar7 = dVar18 * 0.5 + dVar7;
  }
  dVar7 = (double)((float)dVar7 - (float)(dVar18 * 0.5));
  if (((disable_safearea_clamp == 0) && (dVar7 < -0.8100000023841858)) &&
     (-1.299999952316284 < dVar7)) {
    dVar15 = (double)(float)(dVar15 * (double)((float)(-0.8100000023841858 - dVar16) /
                                              (float)(dVar7 - dVar16)));
    dVar12 = 0.5;
  }
  if ((align & 0x20U) != 0) {
    dVar7 = (double)menu_pulsate;
    dVar13 = (double)(float)(dVar13 * dVar7);
    dVar15 = (double)(float)(dVar15 * (double)(float)((double)(float)(dVar7 - 1.0) * dVar12 + 1.0) );
    dVar14 = (double)(float)(dVar14 * dVar7);
  }
  font3d_dx = (float)(dVar15 * 0.1000000014901161);
  dVar12 = (double)font3d_dx;
  if (uVar5 == 0x10) {
    dVar17 = dVar12 * dVar17 - (double)(float)(dVar12 * 0.5);
  }
  else {
    if (uVar5 == 4) {
      dVar16 = dVar12 * 0.5 + dVar16;
      goto LAB_80027f20;
    }
    dVar17 = (double)(float)(dVar12 * dVar17) * 0.5 - (double)(float)(dVar12 * 0.5);
  }
  dVar16 = dVar16 - (double)(float)dVar17;
LAB_80027f20:
  dVar17 = (double)(float)dVar16;
  font3d_dy = (float)(dVar14 * 0.1000000014901161) * FONT3DYMUL;
  dVar16 = (double)font3d_dy;
  font3d_xleft = (float)(dVar17 - (double)(float)(dVar12 * 0.5));
  if ((align & 10U) == 8) {
    posY__1 = (double)(float)(dVar16 * 0.5 + posY__1);
  }
  else if ((align & 10U) == 2) {
    posY__1 = (double)(float)(posY__1 - (double)(float)(dVar16 * 0.5));
  }
  iVar4 = 0;
  if ((uint)colour < 5) {
    iVar4 = colour;
  }
  iVar6 = 0;
  nurndr_forced_mtl_table = Font3DMtlTab + iVar4 * 2;
  if (0 < size_str) {
    dVar7 = 1.0;
    dVar18 = 4.0;
    do {
      local_e8[0] = *txt;
      if (Game.language == 'c') {
        if (((byte *)txt)[1] == 0) break;
        if ((local_e8[0] == 0x23) || (((byte *)txt)[1] == 0x20)) goto LAB_80028014;
LAB_80028414:
        if ((local_e8[0] == 0x3a) || (dVar8 = dVar12, local_e8[0] == 0x2e)) {
          dVar8 = (double)(float)(dVar12 * 0.5);
        }
        dVar17 = (double)(float)(dVar8 * dVar7 + dVar17);
        if (Game.language == 'c') {
LAB_8002844c:
          txt = (char *)((byte *)txt + 1);
          iVar6 = iVar6 + 1;
        }
      }
      else {
LAB_80028014:
        if (((char)local_e8[0] < '\0') && ((local_e8[0] != 0xf8 && (local_e8[0] != 0xfe)))) {
          if ((char)local_e8[0] < '\0') {
            iVar4 = RemapAccentedCharacter((char *)local_e8);
            if (iVar4 == -1) {
              dVar8 = (double)FONT3DSIZE;
              DrawPanel3DObject(-1,(float)dVar17,(float)posY__1,(float)dVar10,
                                (float)(dVar8 * dVar15),(float)(dVar8 * dVar14),
                                (float)((double)(float)(dVar8 * dVar13) * dVar18),0,0,0,font3d_scen e
                                ,Font3DTab[Font3DRemap[122]].obj.special,1);
            }
            else {
              if (Font3DRemap[local_e8[0]] != -1) {
                dVar8 = (double)FONT3DSIZE;
                DrawPanel3DObject(-1,(float)dVar17,(float)posY__1,(float)dVar10,
                                  (float)(dVar8 * dVar15),(float)(dVar8 * dVar14),
                                  (float)((double)(float)(dVar8 * dVar13) * dVar18),0,0,0,
                                  font3d_scene,Font3DTab[Font3DRemap[local_e8[0]]].obj.special,1);
              }
              dVar8 = (double)FONT3DSIZE;
              dVar9 = dVar8 * dVar14;
              iVar3 = -1;
              obj = Font3DAccentTab[iVar4].obj.special;
              fVar1 = (float)(dVar8 * dVar13) * 6.0;
              dVar8 = dVar8 * dVar15;
              scn = font3d_scene;
LAB_800283c0:
              DrawPanel3DObject(iVar3,(float)dVar17,(float)posY__1,(float)dVar10,(float)dVar8,
                                (float)dVar9,fVar1,0,0,0,scn,obj,1);
            }
          }
          goto LAB_80028414;
        }
        iVar4 = (int)(char)local_e8[0];
        if (iVar4 != 0x23) {
          if ((iVar4 - 0x61U < 0x1a) && (Font3DRemap[local_e8[0]] == -1)) {
            if (*(short *)&(&H2OElec[0x13].start)[iVar4].z != -1) {
              if ((*(byte *)((int)&(&H2OElec[0x13].start)[iVar4].z + 2) & 2) == 0) {
                if ((*(byte *)((int)&(&H2OElec[0x13].start)[iVar4].z + 2) & 1) != 0) {
                  DrawPanel3DCharacter
                            ((int)*(short *)&(&H2OElec[0x13].start)[iVar4].z,(float)dVar17,
                             (float)posY__1,(float)dVar10,
                             (float)((double)(&H2OElec[0x13].end)[iVar4].y * dVar15),
                             (float)((double)(&H2OElec[0x13].end)[iVar4].y * dVar14),
                             (float)((double)(&H2OElec[0x13].end)[iVar4].y * dVar13),0,0,0,
                             (int)*(char *)((int)&(&H2OElec[0x13].start)[iVar4].z + 3),
                             (&H2OElec[0x13].end)[iVar4].x,1);
                }
              }
              else if (ObjTab[*(short *)&(&H2OElec[0x13].start)[iVar4].z].obj.special !=
                       (nuspecial_s *)0x0) {
                iVar3 = (int)*(short *)&(&H2OElec[0x13].start)[iVar4].z;
                dVar8 = (double)(&H2OElec[0x13].end)[iVar4].y * dVar15;
                dVar9 = (double)(&H2OElec[0x13].end)[iVar4].y * dVar14;
                fVar1 = (float)((double)(&H2OElec[0x13].end)[iVar4].y * dVar13);
                obj = ObjTab[*(short *)&(&H2OElec[0x13].start)[iVar4].z].obj.special;
                scn = ObjTab[*(short *)&(&H2OElec[0x13].start)[iVar4].z].obj.scene;
                goto LAB_800283c0;
              }
            }
          }
          else {
            iVar4 = (int)Font3DRemap[local_e8[0]];
            if (iVar4 == -1) {
              iVar4 = (int)Font3DRemap[122];
            }
            iVar3 = (int)(char)local_e8[0];
            if (iVar3 != 0x20) {
              if ((((iVar3 - 0x78U < 2) || (iVar3 == 0x61)) || (iVar3 == 0x62)) ||
                 ((iVar3 == 0x77 || (iVar3 == 0x6e)))) {
                fVar1 = 1.0;
              }
              else {
                fVar1 = 4.0;
              }
              if ((local_e8[0] == 0x3a) || (dVar8 = dVar17, local_e8[0] == 0x2e)) {
                dVar8 = (double)(float)(dVar17 - (double)(float)(dVar12 * 0.25));
              }
              dVar9 = posY__1;
              if (local_e8[0] == 0xfe) {
                dVar8 = (double)(float)(dVar8 - (double)(float)(dVar12 * 0.2000000029802322));
                dVar9 = (double)(float)(dVar16 * 0.300000011920929 + posY__1);
              }
              zrot = 0;
              if ((iVar6 == 0) &&
                 (zrot = -(ushort)(local_e8[0] == 0x3f) & 0x8000, local_e8[0] == 0x21)) {
                zrot = 0x8000;
              }
              dVar11 = (double)FONT3DSIZE;
              DrawPanel3DObject(-1,(float)dVar8,(float)dVar9,(float)dVar10,(float)(dVar11 * dVar15 ),
                                (float)(dVar11 * dVar14),(float)(dVar11 * dVar13) * fVar1,0,0,zrot ,
                                font3d_scene,Font3DTab[iVar4].obj.special,1);
            }
          }
          goto LAB_80028414;
        }
        bVar2 = ((byte *)txt)[1];
        if (bVar2 != '\0') {
          if (bVar2 == 'g') {
            iVar4 = 4;
          }
          else if ((char)bVar2 < 'h') {
            if (bVar2 == 'b') {
              iVar4 = 3;
            }
            else if (bVar2 == 'c') {
              iVar4 = 2;
            }
            else {
LAB_800280b0:
              iVar4 = -1;
            }
          }
          else if (bVar2 == 'o') {
            iVar4 = 0;
          }
          else {
            if (bVar2 != 'w') goto LAB_800280b0;
            iVar4 = 1;
          }
          if (iVar4 != -1) {
            nurndr_forced_mtl_table = Font3DMtlTab + iVar4 * 2;
          }
          goto LAB_8002844c;
        }
      }
      iVar6 = iVar6 + 1;
      txt = (char *)((byte *)txt + 1);
    } while (iVar6 < size_str);
  }
  font3d_ybottom = (float)(posY__1 - (double)(float)(dVar16 * 0.5));
  font3d_ytop = (float)(posY__1 + (double)(float)(dVar16 * 0.5));
  font3d_xright = (float)(dVar17 - (double)(float)(dVar12 * 0.5));
  font3d_ymid = (font3d_ytop + font3d_ybottom) * 0.5;
  font3d_xmid = (font3d_xleft + font3d_xright) * 0.5;
  nurndr_forced_mtl_table = (numtl_s **)0x0;
  return;
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