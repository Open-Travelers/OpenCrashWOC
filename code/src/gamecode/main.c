#include "../nu.h"
#include "gamecode/main.h"
/*
  8004f584 0000bc 8004f584  4 InitTexAnimScripts 	Global
  8004f640 000168 8004f640  4 SetTexAnimSignals 	Global
  8004f7a8 000054 8004f7a8  4 LoadFont3D 	Global
  8004f7fc 00005c 8004f7fc  4 InitCreatureModels 	Global
  8004f858 000050 8004f858  4 InitCreatures 	Global
  8004f8a8 000010 8004f8a8  4 IsTitleScreen 	Global
  8004f8b8 00005c 8004f8b8  4 CreateFadeMtl 	Global
  8004f914 00009c 8004f914  4 UpdateFade 	Global
  8004f9b0 00005c 8004f9b0  4 DrawFade 	Global
  8004fa0c 0000e4 8004fa0c  4 InitParticleSystem 	Global
  8004faf0 0006a4 8004faf0  4 InitWorld 	Global
  80050194 0000d0 80050194  4 ResetSuperBuffer 	Global
  80050264 000128 80050264  4 ResetSuperBuffer2 	Global
  8005038c 000124 8005038c  4 MAHLoadingMessage 	Global
  800504b0 000240 800504b0  4 LoadLevel 	Global
  800506f0 0000a0 800506f0  4 DrawWorld 	Global
  80050790 000038 80050790  4 PauseRumble 	Global
  800507c8 000064 800507c8  4 PauseGame 	Global
  8005082c 000020 8005082c  4 ResumeGame 	Global
  8005084c 00042c 8005084c  4 DoInput 	Global
  80050c78 00006c 80050c78  4 TBCODESTART 	Global
  80050ce4 000040 80050ce4  4 TBCODEEND 	Global
  80050d24 000060 80050d24  4 TBDRAWSTART 	Global
  80050d84 000034 80050d84  4 TBDRAWEND 	Global
  80050db8 0000d8 80050db8  4 InitPauseRender 	Global
  80050e90 000048 80050e90  4 ClosePauseRender 	Global
  80050ed8 000158 80050ed8  4 HandlePauseRender 	Global
  80051030 0000ac 80051030  4 SetLevel 	Global
  800510dc 0000d8 800510dc  4 firstscreenfade 	Global
  800511b4 000084 800511b4  4 CopyFilesThreadProc 	Global
  80051238 000104 80051238  4 LoadGBABG 	Global
  8005133c 000068 8005133c  4 UnLoadGBABG 	Global
  800513a4 0000fc 800513a4  4 Reseter 	Global
  800514a0 000058 800514a0  4 Managememcard 	Global
  800514f8 0004d0 800514f8  4 firstscreens 	Global
  800519c8 001b04 800519c8  4 main 	Global
*/

union variptr_u superbuffer_base;
union variptr_u superbuffer_end;
s32 superbuffersize;
union variptr_u superbuffer_reset_base;
struct nugscn_s* font3d_scene;
struct nuscene_s* font3d_scene2;
static struct txanmlist texanimscripts[24];
char texanimbuff[32768];
u32 texanimbits;
static struct numtl_s* fade_mtl;
int fade_rate;
int fadeval;
int fadehack;
static int fadecol;
s32 SHEIGHT;
s32 SWIDTH;
s32 IsLoadingScreen;
s32 FRAME;
char tbtxt[16][16];
s32 i_tb_code;
s32 app_tbset;
s32 i_tb_draw;
struct numtl_s* pause_src_mtl;
struct numtl_s* pause_rndr_mtl;
s32 PHYSICAL_SCREEN_X;
s32 PHYSICAL_SCREEN_Y;
float WeatherBossSkeletonTimer; //vehicle.c
s32 WeatherBossDead; //vehicle.c
s32 GAMEOBJECTCOUNT; //game_obj.c
char* tLOADING[6]; //text.c


//NGC MATCH
void ResetSuperBuffer(void) {
  if (superbuffersize != (double)0x4156eb851eb851ec) {
    if (superbuffer_base.voidptr != NULL) {
      NuMemFree(superbuffer_base.voidptr);
    }
    superbuffer_base.voidptr = NuMemAlloc(0x5bae14);

    superbuffersize = 0x5bae14;
    superbuffer_reset_base = superbuffer_base;
    superbuffer_end.voidptr = (superbuffer_base.voidptr + superbuffersize);
    if (superbuffer_base.voidptr == NULL) {
      NuErrorProlog("C:/source/crashwoc/code/gamecode/main.c",0x5c5)
                                      ("unable to allocate super buffer");
    }
  }
  superbuffer_ptr = superbuffer_reset_base;
}

//NGC MATCH
void ResetSuperBuffer2(void) {
    if (superbuffersize != (double)6008340.48) {
        if (superbuffer_base.voidptr != NULL) {
            NuMemFree(superbuffer_base.voidptr);
        }
        superbuffer_base.voidptr = NuMemAlloc(0x5bae14);
        superbuffersize = 0x5bae14;
        superbuffer_reset_base = superbuffer_base;
        superbuffer_end.intaddr = (superbuffer_base.intaddr + 6008340.48);
        if (superbuffer_base.voidptr == NULL) {
            NuErrorProlog("C:/source/crashwoc/code/gamecode/main.c",0x5e2)
            ("unable to allocate super buffer");
        }
    }
    superbuffer_ptr = superbuffer_reset_base;
}

void InitTexAnimScripts(void)	//PS2
{
    struct nutexanimprog_s *prog;
    struct nudathdr_s* dfanim;
    struct txanmlist *list;
    union variptr_u ptr;

    list = texanimscripts;
    NuTexAnimProgSysInit();
    //dfanim = NuDatOpen("ats.dat", 0, 0);
    NuDatSet(0);  //NuDatSet(dfanim);
    ptr.voidptr = texanimbuff;
    memset(texanimbuff, 0, sizeof(texanimbuff));

    while( list->path != NULL ) {
        if ((list->levbits & LBIT) != 0){
            prog = NuTexAnimProgReadScript(&ptr, list->path);
            if (prog == NULL) {
                NuDebugMsgProlog(".\\main.c", 0x4ed, "", list->path);
            }
        }
        list = list + 1;
    }

    NuDatSet(0);
    /*if (dfanim != 0) {
        NuDatClose(dfanim);
    }*/
    return;
}


//NGC MATCH
void SetTexAnimSignals(void) {

    if ((player->used != '\0') && (player->obj.dead == '\x03' || (player->obj.dead == '\b') || (player->freeze != '\0'))) {
        texanimbits = texanimbits | 2;
    }
    else {
        texanimbits = texanimbits & 0xfffffffd;
    }
    if (((player->obj).mask != NULL) && ((player->obj).mask->active > 1)) {
        texanimbits = texanimbits | 4;
    }
    else {
        texanimbits = texanimbits & 0xfffffffb;
    }
    if (((player->used != '\0') && ((player->obj).dead != '\0')) && ((player->obj).dead != '\x12'))  {
        texanimbits = texanimbits | 0x20;
    }
    else {
        texanimbits = texanimbits & 0xffffffdf;
    }
    if (Level == 0x17) {
        texanimbits = texanimbits | 0x40;
    }
    else {
        texanimbits = texanimbits & 0xffffffbf;
    }
    if (Level == 0x18) {
        if (WeatherBossSkeletonTimer != 0.0f) {
            texanimbits = texanimbits | 0x100;
        }
        else {
            texanimbits = texanimbits & 0xfffffeff;
        }
        if (WeatherBossDead != 0) {
            texanimbits = texanimbits | 0x200;
        }
        else {
            texanimbits = texanimbits & 0xfffffdff;
        }
    }
    NuTexAnimSetSignals(texanimbits);
    NuTexAnimProcess();
    return;
}

//NGC MATCH
void LoadFont3D() {

  font3d_initialised = 0;
  font3d_scene2 = NuSceneReader2(&superbuffer_ptr,&superbuffer_end,"stuff\\font.nus");
  if (font3d_scene2 != NULL) {
    font3d_scene = font3d_scene2->gscene;
    InitFont3D(font3d_scene);
  }
  return;
}

//NGC MATCH
void InitCreatureModels(void) {
  s32 i;

  for (i = 0; i < 9; i++) {
    Character[i].used = '\0';
    Character[i].on = '\0';
    Character[i].off_wait = '\0';
  }
  GAMEOBJECTCOUNT = 0;
  InitSkinning(0);
  LoadCharacterModels();
  CloseSkinning();
  return;
}

//NGC MATCH
void InitCreatures(void) {
  s32 i;

  InitAI();
  InitChases();
  ResetChases();
  i = LDATA->flags & 1;
  if ((LDATA->flags & 1) != 0) {
    AddCreature((s32)LDATA->character,0,-1);
    i = 1;
  }
  PLAYERCOUNT = i;
  return;
}

s32 cutmovie; //cut.c

//NGC MATCH
s32 IsTitleScreen(void) {
  return (cutmovie == 0);
}

//NGC MATCH
void CreateFadeMtl() {
  struct numtl_s *mtl;

  mtl = NuMtlCreate(1);
  fade_mtl = mtl;
  fade_mtl->attrib.zmode = 3;
  fade_mtl->attrib.filter = 1;
  fade_mtl->attrib.lighting = 2;
  fade_mtl->attrib.utc = 1;
  fade_mtl->attrib.vtc = 1;
  fade_mtl->attrib.colour = 1;
  fade_mtl->attrib.alpha = 3;
  NuMtlUpdate(mtl);
  return;
}

//NGC MATCH
void UpdateFade(void) {
  s32 old;

  old = fadeval;
  fadeval = fadeval + fade_rate;
  if (fadeval > 0xff) {
    fadeval = 0xff;
  }
  else if (fadeval < 0) {
      fadeval = 0;
  }
  if ((old < 0xff) && (fadeval == 0xff)) {
    fadehack = 1;
  }
  else if (fadehack != 0) {
    fadehack = fadehack + -1;
  }
  if ((fadeval == 0) || (fadeval == 0xff)) {
    fade_rate = 0;
  }
  fadecol = fadeval | fadeval << 8 | fadeval << 0x10 | 0xff000000;
  return;
}

//NGC MATCH
void DrawFade(void) {
  if ((Cursor.menu != '\x02') && (fadeval != 0)) {
    NuRndrRect2di(0,0,SWIDTH << 4,SHEIGHT << 3,fadecol,fade_mtl);
  }
  return;
}

//NGC MATCH
void InitParticleSystem(void) {
  edppDestroyAllParticles();
  edppDestroyAllEffects();
  if ((LDATA->flags & 0x10) != 0) {
    sprintf(tbuf,"%s.ptl",LevelFileName);
    if (NuFileExists(tbuf) != 0) {
      edppLoadEffects(tbuf,'\x01');
    }
    sprintf(tbuf,"stuff\\general.ptl");
    if (NuFileExists(tbuf) != 0) {
      edppMergeEffects(tbuf,'\0');
    }
    sprintf(tbuf,"%s.cpt",LevelFileName);
    if (NuFileExists(tbuf) != 0) {
      edppMergeEffects(tbuf,'\x05');
    }
  }
  InitGameDebris();
  edppRestartAllEffectsInLevel();
  return;
}

//95% NGC
void InitWorld(void) {
    int i;
    struct nudathdr_s* dh = 0;// r0

    strcpy(LevelFileName, "levels\\");
    strcat(LevelFileName, LDATA->path);
    strcpy(PadRecordPath, LevelFileName);
    strcat(PadRecordPath, ".pad");
    for (i = 0; i < 0x20; i++) {
        world_scene[i] = NULL;
    }
    NuRndrInitWorld();
    if ((LDATA->flags & 4) != 0) {
        strcpy(tbuf, LevelFileName);
        strcat(tbuf, ".nux");
        strcpy(tbuf, LevelFileName);
        strcat(tbuf, ".nus");
        if ((Level != 0x27) && (Level != 0x29)) {
            // size = NuFileSize(tbuf);
            if (NuFileSize(tbuf) != -1) {
                world_scene2[0] = NuSceneReader(&superbuffer_ptr, &superbuffer_end, tbuf);
                if (world_scene2[0] != NULL) {
                    world_scene[0] = world_scene2[0]->gscene;
                }
            } else {
                world_scene[0] = NULL;
                world_scene2[0] = NULL;
            }
            strcpy(tbuf, LevelFileName);
            strcat(tbuf, ".vis");
            world_vd = visiLoadData(tbuf, world_scene[0], &superbuffer_ptr);
            if ((world_scene[0] != NULL) && (world_vd == NULL)) {
                BuildVisiTable(world_scene[0]);
            }
        }
        if (Level == 0xe) {
            world_scene2[1] = NuSceneReader(&superbuffer_ptr, &superbuffer_end, "levels\\a\\tsunami\\wave.nus");
            if (world_scene2[1] != NULL) {
                world_scene[1] = world_scene2[1]->gscene;
            }
        } else if ((Level == 0x27) || (Level == 0x29)) {
            world_scene2[0] = NuSceneReader(&superbuffer_ptr, &superbuffer_end, "levels\\b\\intro\\intro1.nus");
            if (world_scene2[0] != NULL) {
                world_scene[0] = world_scene2[0]->gscene;
            }
            world_scene2[1] = NuSceneReader(&superbuffer_ptr, &superbuffer_end, "levels\\b\\intro\\intro2.nus");
            if (world_scene2[1] != NULL) {
                world_scene[1] = world_scene2[1]->gscene;
            }
            world_scene2[2] = NuSceneReader(&superbuffer_ptr, &superbuffer_end, "levels\\b\\intro\\corridor.nus");
            if (world_scene2[2] != NULL) {
                world_scene[2] = world_scene2[2]->gscene;
            }
            world_scene2[3] = NuSceneReader(&superbuffer_ptr, &superbuffer_end, "levels\\b\\intro\\mystic.nus");
            if (world_scene2[3] != NULL) {
                world_scene[3] = world_scene2[3]->gscene;
            }
            world_scene2[4] = NuSceneReader(&superbuffer_ptr, &superbuffer_end, "levels\\b\\intro\\space.nus");
            if (world_scene2[4] != NULL) {
                world_scene[4] = world_scene2[4]->gscene;
            }
        }
        if ((LBIT & 0x400000) != 0) {
            world_scene2[2] = NuSceneReader(&superbuffer_ptr, &superbuffer_end, "stuff\\redbits.nus");
            if (world_scene2[2] != NULL) {
                world_scene[2] = world_scene2[2]->gscene;
            }
        } else if ((LBIT & 0x00000005a031894d) != 0) {
            world_scene2[2] = NuSceneReader(&superbuffer_ptr, &superbuffer_end, "stuff\\jeepbits.nus");
            if (world_scene2[2] != NULL) {
                world_scene[2] = world_scene2[2]->gscene;
            }
        }
    }
    
    edobjRegisterBaseScene(world_scene[0]);
    edbitsRegisterBaseScene(world_scene[0]);
    NuBridgeRegisterBaseScene(world_scene[0]);
    edanimRegisterBaseScene(world_scene[0]);
    edbitsRegisterSfx(SfxTabGLOBAL, LDATA->pSFX, 0xb1, SFXCOUNT_ALL);
    NuBridgeInit();
    NuWindInit();
    noterraininit();
    
    if ((LDATA->flags & 8) != 0) {
        TerrainSetCur(superbuffer_ptr.voidptr);
        terraininit(Level, &superbuffer_ptr.s16, superbuffer_end.s16, 0, LevelFileName, world_scene[0], 0);
        superbuffer_ptr.intaddr = (uint)((s32) & (superbuffer_ptr.vec4)->w + 3) & 0xfffffff0;
    }
    
    crate_scene = NULL;
    WUMPACOUNT = 0;
    if ((LDATA->flags & 0x40) != 0) {
        crate_scene2 = NuSceneReader(&superbuffer_ptr, &superbuffer_end, "stuff\\crates.nus");
        if (crate_scene2) {
            crate_scene = crate_scene2->gscene;
        }
    }
    
    pause_scene = NULL;
    if (Demo == 0 && ((LDATA->flags & 1) != 0 || (Level == 0x23))) {
        pause_scene2 = NuSceneReader(&superbuffer_ptr, &superbuffer_end, "levels\\b\\pause\\pause.nus ");
        if (pause_scene2) {
            pause_scene = pause_scene2->gscene;
        }
    }
    
    edobjObjectReset();
    
    sprintf(tbuf, "%s.obj", LevelFileName);
    if (NuFileExists(tbuf) != 0) {
        edobjFileLoadObjects(tbuf);
    }
    edanimParamReset();
    
    sprintf(tbuf, "%s.anm", LevelFileName);
    if (NuFileExists(tbuf) != 0) {
        edanimFileLoad(tbuf);
    }
    edgraClumpsReset();
    
    sprintf(tbuf, "%s.gra", LevelFileName);
    if (NuFileExists(tbuf) != 0) {
        edgraFileLoad(tbuf);
    }
    edgraInitAllClumps();

    NuDatSet(dh);
    if (dh) {
        NuDatClose(dh);
    }
    
    LoadVehicleStuff();
    
    if (Level == 0x27) {
        LoadCutMovie(1);
        StartCutMovie();
    } else if (Level == 0x29) {
        LoadCutMovie(3);
        StartCutMovie();
    }
    
    TerrainPlatformOldUpdate();
    if (world_scene[0] != NULL) {
        NuGScnUpdate(world_scene[0], 0.0f);
    }
    
    edobjUpdateObjects(0.0f);
    edanimUpdateObjects(0.0f);
    NuBridgeUpdate(&Character[0].obj.pos);
    NuWindUpdate(&Character[0].obj.pos);
    TerrainPlatformNewUpdate();
    
    if (((LDATA->flags & 1) != 0) && (Level != 0x25)) {
        ShadowMat = CreateAlphaBlendTexture64("stuff\\gradient.raw", 0, 1, 100);
        ShadowMatBodge = ShadowMat->tid;
    }
    
    ResetCheckpoint(-1, -1, 0.0f, NULL);
    InitSplineTable();
    InitObjectTable();
    InitRails();
    InitWumpa();
    ResetMaskFeathers();
    InitCrates();
    InitVehicleToggles();
    InitLevel();
    
    NuDatSet(dh);
    if (dh) {
        NuDatClose(dh);
    }
    
    for (i = 0; i < 32; i++) {
        if (world_scene[i] != NULL) {
            InitXboxGSceneEffects(world_scene[i], &superbuffer_ptr, &superbuffer_end);
        }
    }
    
    NuLightMatInit();
    return;
}

//NGC MATCH
void MAHLoadingMessage(void)
{
  IsLoadingScreen = 1;
  NuRndrClear(0xb,0,1.0);
  if (NuRndrBeginScene(1) != 0) {
    memcpy(&pNuCam->mtx, &numtx_identity, sizeof (struct numtx_s));
    NuCameraSet(pNuCam);
    if ((font3d_scene != NULL) && (font3d_initialised != 0)) {
      NuShaderSetBypassShaders(1);
      DrawGameMessage(tLOADING[Game.language],0,0.0);
      NuShaderSetBypassShaders(0);
    }
    NuRndrEndScene();
  }
  NuRndrSwapScreen(1);
  IsLoadingScreen = 0;
  return;
}

//NGC MATCH
void LoadLevel(void) {

    loadcount++;
    load_anim_data = NULL;
    hLoadScreenThread = NULL;
    InitXboxEffectSystem(Level);
    NuSoundKillAllAudio();
    XbWaitForAllBuffersToStop();
    NuStartLoadScreen(LDATA->totalsize);
    if (Level != 0x28) {
        LoadFont3D();
    }
    PurgeCharacterModels();
    crash_loaded = 0;
    if (Level != 0x28) {
        MAHLoadingMessage();
    }
    InitTexAnimScripts();
    InitCreatureModels();
    if ((LBIT & 0x200000a1) != 0) {
        InitClouds(&superbuffer_ptr, &superbuffer_end);
    }
    if ((LDATA->flags & 0x40) != 0) {
        if (CrateMat == NULL) {
            if (CrateMat2 == NULL) {
                CrateMat = CreateAlphaBlendTexture256128_32("stuff\\box.raw", 0, 1, 0x100, 0);
                CrateMat2 = CreateAlphaBlendTexture256128_32("stuff\\boxyellow.raw", 0, 1, 0x100, 0);
            }
        }
    }
    InitLocalSfx(LDATA->pSFX, (s32)LDATA->nSFX);
    InitSpecular();
    wumpa_scene = NULL;
    if ((LDATA->flags & 0x100) != 0) {
        wumpa_scene2 = NuSceneReader(&superbuffer_ptr, &superbuffer_end, "stuff\\wumpa.nus");
        if (wumpa_scene2 != NULL) {
            wumpa_scene = wumpa_scene2->gscene;
        }
    }
    
    {
        float arr0[2] = {0.625f, 0.75f};
        float arr1[2] = {0.25f, 0.5f};
        NuLigthSetPolyHazeMat(DebMat[4], arr0, arr1);
    }
    
    ClearGameObjects();
    ResetCheckpoint(-1, -1, 0.0f, NULL);
    InitWorld();
    i_cratetypedata = 0;
    if (((LDATA->flags & 0x40) != 0) && (ReadCrateData() != 0)) {
        ReadInCrateData();
    }
    LoadLights();
    InitCreatures();
    ParticleReset();
    InitParticleSystem();
    NuStopLoadScreen(0);
    if (Level == 7) {
        YTOL = 0.01166667f;
    } else {
        YTOL = 0.01f;
    }
    return;
}

//NGC MATCH
void DrawWorld(void) {
  if (level_part_2 != 0) {
    if (world_scene[1] != NULL) {
        NuGScnRndr3(world_scene[1]);
        NuBridgeDraw(world_scene[1],DebMat[6]);
        NuWindDraw(world_scene[1]);
        edobjRenderObjects(world_scene[1]);
      }
  }
  else if (world_scene[0] != NULL) {
      NuGScnRndr3(world_scene[0]);
      NuBridgeDraw(world_scene[0],DebMat[6]);
      NuWindDraw(world_scene[0]);
      edobjRenderObjects(world_scene[0]);
    }
  return;
}

//NGC MATCH
void PauseRumble(void) {
  if (Pad[0] != NULL) {
    NuPs2PadSetMotors(Pad[0],0,0);
  }
  return;
}

//NGC MATCH
void PauseGame(void) {
  pause_dir = 1;
  NewMenu(&Cursor,5,0,-1);
  ResetTimer(&PauseTimer);
  pausestats_frame = 0;
  PauseGameAudio(1);
  GameSfx(0x36,NULL);
  return;
}

void ResumeGame(void) {
  ResumeGameAudio();
  return;
}

/*
//NGC MATCH
void DoInput(void) {
  struct nupad_s *pad;
  s32 sfx;
  s32 menu_active;
  float f;
  float f12;
    s32 temp;
  
  XbPollPeripherals();
  menu_active = 1;
  if(Cursor.menu == -1) {
      menu_active = 0;
  }
  XbPollAllControllers(menu_active);
  pad = Pad[0];
  if (NuPs2ReadPad((struct nupad_s*)&pad) != 0) {
    lost_controller = 0;
    if ((pad->padhandle != NULL) || ((PadRecInfo != NULL && (PadRecInfo->padmode == 2)))) {
      if ((pad_record != 0) && (((pad->oldpaddata & 0x800) != 0 || ((player->obj).dead != 0)))) {
        pad_record = 0;
      }
      stick_bits = 0;
        f12 = 85.0f;
      f = (u32)pad->buttons_hi - 127.5f;
      if ((f < -f12)) {
        stick_bits = 0x8000;
      }
      else if (f > 85.0f) {
          stick_bits = 0x2000;
      }
      f = (u32)pad->buttons_lo - 127.5f;
      if (f < -f12) {
        stick_bits |= 0x1000;
      }
      else if (f > f12) {
          stick_bits |= 0x4000;
      } 
        temp = stick_bits;
      stick_bits_db = temp & ~stick_oldbits;
      stick_oldbits = temp;
      if (((((pad->oldpaddata & 0x800) != 0) && ((LDATA->flags & 1) != 0)) && (new_mode == -1)) &&
         (((new_level == -1 && (fadeval == 0)) && ((GameMode != 1 && (cutmovie == -1)))))) {
        if ((((Paused == 0) && (Cursor.menu == -1)) && ((PLAYERCOUNT == 0 || (((player->obj).finished == 0 &&
              (((Level != 0x25 || !(tumble_time < tumble_duration)) || (last_hub == -1)))))))) &&
           (((LBIT & 0x3e00000) == 0 || (boss_dead != 2)))) {
          PauseGame();
          sfx = 0x36;
        }
        else if (Paused == 0x1e) {
            pause_dir = 2;
            sfx = 0x3c;
          } else {
             return;
          }
        GameSfx(sfx,NULL);
      }  
    }
    else if ((((((Paused == 0) || (pause_dir == 2)) && ((Pad[0] != NULL &&
             (((((Pad[0]->paddata_db & 0x20000000) == 0 && (Demo == 0)) && (Cursor.menu != 0)) &&
              ((Cursor.menu != '#' && (GameMode != 1)))))))) && (cutmovie == -1)) &&
          (((new_mode == -1 && (new_level == -1)) && ((fadeval == 0 && (Cursor.menu == -1)))))) &&
         ((PLAYERCOUNT == 0 || (((player->obj).finished == 0 &&
           (((Level != 0x25 || !(tumble_time < tumble_duration)) || (last_hub == -1)))))))) {
        XbPollAllControllers(1);
        Pad[0] = NULL;
        PauseGame();
      }
  }
  else {
    if ((((Paused == 0) && (Pad[0] != NULL)) && ((Pad[0]->paddata_db & 0x20000000) == 0))
       && (((Demo == 0 && (Cursor.menu != 0)) && ((Cursor.menu != '#' && ((GameMode != 1 && (cutmovie == -1)))))))) {
      PauseGame();
    }
    Pad[0] = XbGetWorkingController(); 
  }
  return;
}
*/

//NGC MATCH
void TBCODESTART(s32 i,char *txt) {
  if (((FRAME == 0) && (i_tb_code == i)) && (strlen(txt) < 0x10)) {
    strcpy(tbtxt[8],txt);
    tbslotBegin(app_tbset,8);
  }
  return;
}

//NGC MATCH
void TBCODEEND(s32 i) {
  if ((FRAME == 0) && (i_tb_code == i)) {
    tbslotEnd(app_tbset,8);
  }
  return;
}

//NGC MATCH
void TBDRAWSTART(s32 i,char *txt) {
  if ((i_tb_draw == i) && (strlen(txt) < 0x10)) {
    strcpy(tbtxt[0xb],txt);
    tbslotBegin(app_tbset,0xb);
  }
  return;
}

//NGC MATCH
void TBDRAWEND(s32 i) {
  if (i_tb_draw == i) {
    tbslotEnd(app_tbset,0xb);
  }
  return;
}

//NGC MATCH
void InitPauseRender(void) {
  pause_rndr_mtl = NuMtlCreate(1);
  //mtl->attrib = (numtlattrib_s)((uint)mtl->attrib & 0xc3cffff | 0x114e8000);
    pause_rndr_mtl->alpha = 1.0f;
    pause_rndr_mtl->tid = 0x270e;
    pause_rndr_mtl->attrib.zmode = 3;
    pause_rndr_mtl->attrib.utc = 1;
    pause_rndr_mtl->attrib.vtc = 1;
    pause_rndr_mtl->attrib.filter = 1;
    pause_rndr_mtl->attrib.lighting = 2;
    pause_rndr_mtl->attrib.colour = 1;
    pause_rndr_mtl->attrib.alpha = 4;
    pause_rndr_mtl->alpha_sort = 0x7fff;
    NuMtlUpdate(pause_rndr_mtl);
    
    pause_src_mtl = NuMtlCreate(1);
    pause_src_mtl->alpha = 1.0f;
    pause_src_mtl->tid = 0x270e;
  //mtl->attrib = (numtlattrib_s)((uint)mtl->attrib & 0xc3cffff | 0x114e8000);
    pause_src_mtl->attrib.zmode = 3;
    pause_src_mtl->attrib.utc = 1;
    pause_src_mtl->attrib.vtc = 1;
    pause_src_mtl->attrib.filter = 1;
    pause_src_mtl->attrib.lighting = 2;
    pause_src_mtl->attrib.colour = 1;
    pause_src_mtl->attrib.alpha = 0;
    pause_src_mtl->alpha_sort = 0x7ffe;
    
  NuMtlUpdate(pause_src_mtl);
  pause_rndr_on = 0;
  return;
}

//NGC MATCH
void ClosePauseRender(void) {
  if (pause_src_mtl != NULL) {
    NuMtlDestroy(pause_src_mtl);
  }
  if (pause_rndr_mtl != NULL) {
    NuMtlDestroy(pause_rndr_mtl);
  }
  pause_src_mtl = NULL;
  pause_rndr_mtl = NULL;
  return;
}

//NGC MATCH
void HandlePauseRender(int pause_count) {
  static s32 old_pause_state;
  s32 vp_x;
  s32 vp_y;
  struct nuviewport_s *vp;

  vp_x = (s32)((float)pause_count * 27.0f / 30.0f);
  vp_y = (s32)((float)pause_count * 19.0f / 30.0f);
  if ((pause_rndr_on != 0) && (NuRndrBeginScene(1) != 0)) {
    vp = NuVpGetCurrentViewport();
    NuRndrClear(0xb,0,1.0f);
    NuRndrRectUV2di(vp_x,vp_y,vp->width - vp_x,vp->height - vp_y,0.0f,0.0f,1.0f,1.0f,-1,pause_rndr_mtl);
    NuRndrEndScene();
  }
  if (Paused != 0) {
    if (old_pause_state == 0) {
      NudxFw_MakeBackBufferCopy(1);
      GS_CopyFBToPause();
      pause_rndr_on = 1;
    }
    if (Paused != 0) {
        goto block_end;
    }
  }
  if (old_pause_state != 0) {
    pause_rndr_on = 0;
  }
block_end:
  old_pause_state = Paused;
  return;
}

//PS2 MATCH
void SetLevel(void) {
    LBIT = 1L << (u64)Level; //NGC //LBIT = __ashldi3(0,1);
  LDATA = &LData[Level];
  if (pNuCam != NULL) {
    if (LDATA->farclip > 9) {
      pNuCam->farclip = (float) LDATA->farclip;
    }
    else {
     pNuCam->farclip = 1000.0f;
    }
  }
  AIVISRANGE = 25.0f;
  return;
}

//NGC MATCH
void firstscreenfade(struct numtl_s *mat,s32 dir) {
    s32 s;
    s32 t;
    u32 colour;
    s32 col;

    if (dir > 0) {
        colour = 0;
        s = 0x10;
    }
    else {
        colour = 0xff;
        s = -0x10;
    }
    for (t = 0; t < 0xe; t++) {
        col = (0xFF << 24) | (colour << 16) | (colour << 8) | colour;
        colour = colour + s;
        if (NuRndrBeginScene(1) != 0) {
            NuRndrClear(0xb,0,1.0f);
            NuRndrRectUV2di(0,0,PHYSICAL_SCREEN_X,PHYSICAL_SCREEN_Y,0.0f,0.0f,1.0f,1.0f,col,mat);
            NuRndrEndScene();
            NuRndrSwapScreen(1);
        }
    }
    return;
}

//NGC MATCH
s32 CopyFilesThreadProc() {
    s32 iVar1;
    char texBuf [128];
    u32 iStack_c;

    iVar1 = GetTickCount();
    InitLevelSfxTables();
    InitGlobalSfx();
    iStack_c = GetTickCount() - iVar1;
    sprintf(texBuf,"Filecopy took %.2f seconds", iStack_c / 1000.0f);
    return 0;
}

/*
//NGC MATCH
void LoadGBABG(void) {
  struct numtl_s *mtl;
  struct nutex_s tex;

  GBABG_Ptr = malloc_x(0x2000c);
  NuFileLoadBuffer("gfx\\crash2gb.s3",GBABG_Ptr,0x2000c);
  tex.width = 0x200;
  tex.height = 0x200;
  tex.decal = 0;
  tex.bits = GBABG_Ptr;
  iss3cmp = 0x20000;
  tex.mmcnt = 1;
  tex.pal = NULL;
  tex.type = NUTEX_RGB24;
  GBABG_tid = NuTexCreate(&tex);
  iss3cmp = 0;
  mtl = NuMtlCreate(1);
  GBABG_mtl = mtl;
  (mtl->diffuse).r = 1.0f;
  (mtl->diffuse).g = 1.0f;
  (mtl->diffuse).b = 1.0f;
  //mtl->attrib = (numtlattrib_s)((uint)attr & 0xcc0cffff | 0x16e8000);
    mtl->attrib.cull = 2;
    mtl->attrib.zmode = 3;
    mtl->attrib.filter = 0;
    mtl->attrib.lighting = 2;
    mtl->attrib.colour = 1;
    mtl->alpha = 0.999f;
    mtl->tid = GBABG_tid;
    mtl->attrib.utc = 1;
    mtl->attrib.vtc = 1;
  NuMtlUpdate(mtl);
  return;
}

//NGC MATCH
void UnLoadGBABG(void) {
  if (GBABG_Ptr != NULL) {
    free_x(GBABG_Ptr);
  }
  GBABG_Ptr = NULL;
  if (GBABG_mtl != NULL) {
    if (GBABG_mtl->tid != 0) {
      NuTexDestroy(GBABG_mtl->tid);
    }
    NuMtlDestroy(GBABG_mtl);
    GBABG_mtl = NULL;
  }
  return;
}
*/

/*

//NGC MATCH
void Reseter(void) {
  if (OSGetResetButtonState() != 0) {
    reset_256 = 1;
  }
  else {
    if (reset_256 != 0) {
      OSReport("SS_StopAllSFX\n");
      SS_StopAllSFX();
      OSReport("SS_TrackStop\n");
      SS_TrackStop(-1);
      OSReport("GXAbortFrame\n");
      OSReport("GXDrawDone\n");
      GXDrawDone();
      OSReport("VISetBlack\n");
      VISetBlack(1);
      OSReport("VIFlush\n");
      VIFlush();
      OSReport("VIWaitForRetrace\n");
      VIWaitForRetrace();
      OSReport("OSResetSystem\n");
      OSResetSystem(0,1,0);
    }
  }
  return;
}

//NGC MATCH
void Managememcard(void) {
  SS_StopAllSFX();
  SS_TrackStop(-1);
  GXDrawDone();
  VISetBlack(1);
  VIFlush();
  VIWaitForRetrace();
  OSResetSystem(1,1,1);
  return;
}


*/

//NGC MATCH
void firstscreens() {
    struct nutex_s tex;
    char * gerbils;
    struct numtl_s * firstscreen1_mtl;
    s32 firstscreen1_tid;

    firstscreen1_mtl = NULL;  
    CopyFilesThreadProc(0);
    gerbils = (char*)malloc_x(0x4000c);
    NuFileLoadBuffer("gfx\\licnin.s3",gerbils,0x2000c);
    tex.decal = 0;
    tex.pal = NULL;
    tex.height = tex.width = 0x200; 
    tex.bits = gerbils;
    tex.mmcnt = 1;
    tex.type = NUTEX_RGB24;

    iss3cmp = 0x20000;
    firstscreen1_tid = NuTexCreate(&tex);
    iss3cmp = 0;
    firstscreen1_mtl = NuMtlCreate(1);

    firstscreen1_mtl->tid = firstscreen1_tid;
    firstscreen1_mtl->diffuse.b = firstscreen1_mtl->diffuse.g = firstscreen1_mtl->diffuse.r = 1.0f;
    //((uint)attr & 0xcc0cffff | 0x16e8000);
    firstscreen1_mtl->attrib.cull = 2;
    firstscreen1_mtl->attrib.zmode = 3;
    firstscreen1_mtl->attrib.filter = 4;
    firstscreen1_mtl->attrib.lighting = 2;
    firstscreen1_mtl->attrib.colour = 1;
    firstscreen1_mtl->alpha = 0.999f;
    firstscreen1_mtl->attrib.utc = 1;
    firstscreen1_mtl->attrib.vtc = 1;
    NuMtlUpdate(firstscreen1_mtl);
    firstscreenfade(firstscreen1_mtl,1);
    for (nuvideo_global_vbcnt = 0; nuvideo_global_vbcnt < 0x78; ) {
        if (NuRndrBeginScene(1) != 0) {
            NuRndrClear(0xb,0,1.0f);
            NuRndrRectUV2di(0,0,PHYSICAL_SCREEN_X,PHYSICAL_SCREEN_Y,0.0f,0.0f,1.0f,1.0f,-1,firstscreen1_mtl);
            NuRndrEndScene();
            NuRndrSwapScreen(1);
        }
        Reseter(0);
        GC_DiskErrorPoll();
    } 
    nuvideo_global_vbcnt = 0;
    firstscreenfade(firstscreen1_mtl,-1);
    NuRndrClear(0xb,0,1.0f);
    NuRndrSwapScreen(1);
    if (firstscreen1_mtl->tid != 0) {
        NuTexDestroy(firstscreen1_mtl->tid);
    }
    NuFileLoadBuffer("gfx\\copyr1.s3",gerbils,0x4000c);
    tex.width = 0x200;
    tex.height = 0x200;
    tex.decal = 0;
    tex.bits = gerbils;
    tex.mmcnt = 1;
    tex.pal = NULL;
    tex.type = NUTEX_RGB24;
    iss3cmp = 0x40000;
    firstscreen1_tid = NuTexCreate(&tex);
    iss3cmp = 0;
    firstscreen1_mtl = NuMtlCreate(1);

    firstscreen1_mtl->tid = firstscreen1_tid;
    firstscreen1_mtl->diffuse.b = firstscreen1_mtl->diffuse.g = firstscreen1_mtl->diffuse.r = 1.0f;
    firstscreen1_mtl->attrib.cull = 2;
    firstscreen1_mtl->attrib.zmode = 3;
    firstscreen1_mtl->attrib.filter = 0;
    firstscreen1_mtl->attrib.lighting = 2;
    firstscreen1_mtl->attrib.colour = 1;
    firstscreen1_mtl->alpha = 0.999f;
    firstscreen1_mtl->attrib.utc = 1;
    firstscreen1_mtl->attrib.vtc = 1;
    NuMtlUpdate(firstscreen1_mtl);
    firstscreenfade(firstscreen1_mtl,1);
    for (nuvideo_global_vbcnt = 0; nuvideo_global_vbcnt < 0x78; ) {
        if (NuRndrBeginScene(1) != 0) {
            NuRndrClear(0xb,0,1.0f);
            NuRndrRectUV2di(0,0,PHYSICAL_SCREEN_X,PHYSICAL_SCREEN_Y,0.0f,0.0f,1.0f,1.0f,-1,firstscreen1_mtl);
            NuRndrEndScene();
            NuRndrSwapScreen(1);
        }
        Reseter(0);
        GC_DiskErrorPoll();
    } 
    nuvideo_global_vbcnt = 0;
    firstscreenfade(firstscreen1_mtl,-1);
    NuRndrClear(0xb,0,1.0f);
    NuRndrSwapScreen(1);
    if (firstscreen1_mtl->tid != 0) {
        NuTexDestroy(firstscreen1_mtl->tid);
    }
    NuFileLoadBuffer("gfx\\euro.s3",gerbils,0x2000c);
    tex.width = 0x200;
    tex.height = 0x200;
    tex.decal = 0;
    tex.mmcnt = 1;
    tex.pal = NULL;
    tex.type = NUTEX_RGB24;
    iss3cmp = 0x20000;
    firstscreen1_tid = NuTexCreate(&tex);
    iss3cmp = 0;
    firstscreen1_mtl = NuMtlCreate(1);
    firstscreen1_mtl->tid = firstscreen1_tid;
    firstscreen1_mtl->diffuse.b = firstscreen1_mtl->diffuse.g = firstscreen1_mtl->diffuse.r = 1.0f;
    //((uint)attr & 0xcc0cffff | 0x16e8000);
    firstscreen1_mtl->attrib.cull = 2;
    firstscreen1_mtl->attrib.zmode = 3;
    firstscreen1_mtl->attrib.filter = 0;
    firstscreen1_mtl->attrib.lighting = 2;
    firstscreen1_mtl->attrib.colour = 1;
    firstscreen1_mtl->alpha = 0.999f;
    firstscreen1_mtl->attrib.utc = 1;
    firstscreen1_mtl->attrib.vtc = 1;
    NuMtlUpdate(firstscreen1_mtl);
    firstscreenfade(firstscreen1_mtl,1);
    for (nuvideo_global_vbcnt = 0; nuvideo_global_vbcnt < 0x78; ) {
        if (NuRndrBeginScene(1) != 0) {
            NuRndrClear(0xb,0,1.0f);
            NuRndrRectUV2di(0,0,PHYSICAL_SCREEN_X,PHYSICAL_SCREEN_Y,0.0f,0.0f,1.0f,1.0f,-1,firstscreen1_mtl);
            NuRndrEndScene();
            NuRndrSwapScreen(1);
        }
        Reseter(0);
        GC_DiskErrorPoll();
    } 
    nuvideo_global_vbcnt = 0;
    firstscreenfade(firstscreen1_mtl,-1);
    NuRndrClear(0xb,0,1.0f);
    NuRndrSwapScreen(1);
    free_x(gerbils);
    if (firstscreen1_mtl != NULL) {
        if (firstscreen1_mtl->tid != 0) {
            NuTexDestroy(firstscreen1_mtl->tid);
        }
        NuMtlDestroy(firstscreen1_mtl);
    }
}


/*

s32 main(int argc /* r3 */, char * * argv /* r4 */) {
    // Local variables
    int room_ambience_update_count; // r18
    float f; // f0
    struct creature_s * plr; // r30
    int i; // r29
    int temp; // r9
    int temp_paused; // r31
    int ok; // r26
    int retro; // r23
    int old_paused;
    int gametimer;
    int y0; // r11
    int y1; // r6
    struct nuvec_s pos; // r1+0x8
    int just_been_paused; // r22
    int reset; // r1+0xA0

    // Labels
    NewMode: // 0x80051330

    // Blocks
    /* anonymous block */ {
        // Range: 0x800510B4 -> 0x80052A40
        /* anonymous block */ {
            // Range: 0x80051494 -> 0x800526D8
            /* anonymous block */ {
                // Range: 0x80051560 -> 0x80051794
            }
            /* anonymous block */ {
                // Range: 0x800519B4 -> 0x800519C8
            }
            /* anonymous block */ {
                // Range: 0x80051D30 -> 0x80051D90
                struct nuvec_s * flamepos[32]; // r1+0x20
                int cnt; // r3
                int offset; // r29
                int debristypeid; // r28
            }
            /* anonymous block */ {
                // Range: 0x80051F80 -> 0x80052610
            }
            /* anonymous block */ {
                // Range: 0x80052610 -> 0x800526B0
                int curfps; // r9
            }
        }
    }
}
*/


//89% NGC (86% PS2)
/*
s32 main(s32 argc,char **argv) {
  //s32 bVar1;
  //s32 bVar2;
  float fVar3;
  short sVar4;
  s32 uVar5;
  float fVar6;
  s32 iVar8;
  s32 iVar9;
  //char **in_r5;
  struct creature_s *plr;
  //struct cammtx_s *GameCam_;
  struct nuvec_s pos;
  s32 v155;
  s32 local_9c;
  s32 local_9d;
  
 // __main(argc,argv,in_r5);
  v155 = 0;
  DEMOInit(NULL);
  GS_Init();
  SS_Init();
  MC_Init();
  NuFileInitEx(0,0);
  Version();
  PadData = (struct _XINPUT_STATE*)GS_FrameBufferCopydataptr;
  DefaultGame();
  ResetGame();
  NuPs2Init();
  DebrisMalloc();
  Hub = -1;
  Level = -1;
  MAXVPSIZEX = SWIDTH;
  MAXVPSIZEY = SHEIGHT;
  MINVPSIZEX = (s32)(((float)SWIDTH * 390.0f) / 640.0f);
  MINVPSIZEY = (s32)(((float)SHEIGHT * 244.0f) / 480.0f);
  InitCutScenes();
  NuTrigInit();
  NuVpInit();
  NuTexInit();
  NuGobjInit();
  NuMtlInit();
  NuRndrInitEx(0x200000);
  NuLightInit();
  pNuCam = NuCameraCreate();
  firstscreens();
  Pad[0] = NuPs2OpenPad(0,0);
  Pad[1] = NuPs2OpenPad(1,0);
  app_tbset = tbsetCreate(NULL);
  if (Level != -1) {
    NewGame();
    CalculateGamePercentage(&Game);
    Hub = HubFromLevel(Level);
  }
  else {
    Level = 0x23;
  }
  last_level = Level;
LAB_80051ba4:
  srand(0);
  qseed = 0x3039;
  NuRandSeed(0);
  if (Level == 0x23) {
    NewGame();
    Hub = -1;
    NewMenu(&Cursor,-1,-1,-1);
    Demo = 0;
    cutmovie = 0;
    Level = 0x25;
  }
  SetLevel();
  highallocaddr = 0;
  NuTexInit();
  NuMtlInit();
  NuGobjInit();
  InstInit();
  NuSoundKillAllAudio();
  while (cutmovie != -1) {
    pad_play = 0;
    InitPadPlayRecord(PadRecordPath,0,0x13ec,PadData);
    PlayCutMovie(cutmovie);
    NewMenu(&Cursor,-1,-1,-1);
    cutmovie = -1;
    logos_played = 1;
    NuSoundKillAllAudio();
  }
  SetLevel();
  InitCameraTargetMaterial();
  DebrisSetup();
  VehicleSetup();
  DebrisRegisterCutoffCameraVec((struct nuvec_s *)&global_camera.mtx._30);
  CreateFadeMtl();
  ResetSuperBuffer();
  CrateMat2 = NULL;
  CrateMat = NULL;
  LoadLevel();
  InitPauseRender();
  NuLgtSetArcMat(DebMat[5],0.5019531f,0.7480469f,0.5605469f,0.9980469f);
  Reset3DFontObjects();
  InitPlayerEvents();
  if (Level == 0x28) {
    GameMode = 1;
  }
  else if (Level == 0x26) {
    ResetTempCharacter(2,0x22);
    tempanim_nextaction = 0x73;
    gamesfx_channel = 4;
    GameSfx(0xb2,NULL);
    cortex_gameover_i = -1;
    tempanim_waitaudio = 1;
  }
  else {
    if (((Level == 0x25) && ((Game.cutbits & 1) == 0)) && (force_menu != -1)) {
      iVar9 = 0;
    }
    else {
          if (just_continued != 0) {
              if (((Level == 0x25) && ((Game.level[21].flags & 0x800) != 0)) && ((Game.cutbits & 0x40) == 0)
                 ) {
                iVar9 = 6;
              }
              else {
                if (just_continued == 0) goto LAB_80051e34;
                if (((Level == 0x25) && ((Game.level[23].flags & 0x800) != 0)) &&
                   ((Game.cutbits & 0x800) == 0)) {
                  iVar9 = 0xb;
                }
                else {
                  if ((((just_continued == 0) || (Level != 0x25)) || ((Game.level[22].flags & 0x800) == 0) )
                     || ((Game.cutbits & 0x10000) != 0)) goto LAB_80051e34;
                  iVar9 = 0x10;
                }
              }   
            NewCut(iVar9);
            GameMode = 1;
              
          } //else{
    LAB_80051e34:
            GameMode = 0;
            //goto LAB_80051e38;
         // }
    }
  }
//LAB_80051e38:
  if (GameMode == 1) {
    if (Level == 0x25) {
      Hub = -1;
    }
    ResetTimer(&CutTimer);
  }
  just_continued = 0;
  if ((ForcePlayRecord == 0) && (pad_play = 0, Demo != 0)) {
    pad_play = (s32)(pad_record == 0);
  }
  if (pad_record != 0) {
   // iVar9 = 1;
    InitPadPlayRecord(PadRecordPath,1,0x13ec,PadData);
  }
  else {
    if (pad_play != 0) {
      InitPadPlayRecord(PadRecordPath,2,0x13ec,PadData);
      //goto LAB_80051ef4;
    } else{
      InitPadPlayRecord(PadRecordPath,0,0x13ec,PadData);
    }
   // iVar9 = 2;
  }
//LAB_80051ef4:
  ResetBonus();
  ResetDeath();
  ResetGemPath();
  game_music = -1;
  LostLife = 0;
  if ((Cursor.menu != '\x13') && (GameMode != 1)) {
    GameMusic((s32)LDATA->music[0],0);
  }
  USELIGHTS = 1;
  musicvol_mul = 0.0f;
  srand(0);
  qseed = 0x3039;
  NuRandSeed(0);
  while( 1 ) {
    NuSoundSetLevelAmbience();
    NuSoundUpdate();
    if (((Cursor.menu != '\x13') && (game_music != LDATA->music[0])) && (GameMode != 1)) {
      GameMusic((s32)LDATA->music[0],0);
    }
    InitCrateExplosions();
    if (Bonus != 4) {
      ResetBonus();
    }
    iVar9 = 0;
    ResetDeath();
    ResetTimeTrial();
    ResetCrates();
    ResetGemPath();
    ResetWumpa();
    NuSoundUpdate();
    ResetChases();
    ResetPlayerEvents();
    ResetGates();
    ResetRings();
    ResetMaskFeathers();
    ResetAI();
    NuSoundUpdate();
    InitGameMode(iVar9);
    ResetBug();
    ResetLevel();
    ResetVehicleControl((s32)(player->obj).RPos.iRAIL,(s32)(player->obj).RPos.iALONG,
                        (player->obj).RPos.fALONG);
    InitDeb3();
    NuSoundUpdate();
    ResetProjectiles();
    Paused = 0;
    pause_dir = 0;
    ResetPanelDebris();
    if (Pad[0] != NULL) {
      NuPs2PadSetMotors(Pad[0],0,0);
    }
    ResetGameSfx();
    local_9c = 0;
    local_9d = 0;
    vtog_time = 0.0f;
    vtog_duration = 0.0f;
    ResetGameCameras(GameCam,1);
    in_finish_range = 0;
    ResetAwards();
    NuSoundUpdate();
    fade_rate = -8;
    frameout_count = nuvideo_global_vbcnt;
    frameout = 0;
    NuInitFrameAdvance();
    while (((new_mode == -1 && (new_level == -1)) || ((fadeval < 0xff || (fadehack != 0))))) {
      DBTimerStart(1);
      tbslotBegin(app_tbset,0);
      if ((LDATA->fognear != LDATA->fogfar) && (pause_rndr_on == 0)) {
        level_clearcolour =
             (uint)LDATA->foga << 0x18 | (uint)LDATA->fogr << 0x10 | (uint)LDATA->fogg << 8 |
             (uint)LDATA->fogb;
        level_fogcolour =
             (uint)LDATA->foga << 0x18 | (uint)LDATA->fogb << 0x10 | (uint)LDATA->fogg << 8 |
             (uint)LDATA->fogr;
      }
      else {
        level_clearcolour = 0;
        level_fogcolour = 0;
      }
      //bVar2 = 0x1e < v155;
      v155 = v155 + 1;
      if (0x1e < v155) {
        v155 = 0;
        NuSoundSetLevelAmbience();
      }
      NuGetFrameAdvance();
        FRAMES = 1;
      if ((FixFrameRate != 0) || (pad_record != 0) || (pad_play != 0)) {
             FRAMES = 1;
      }
      plr = player;
      FRAMES = 1;
      if ((s32)cufps < 59.0f) {
        FRAMES = 2;
      }
      if (cufps < 29.0f) {
        FRAMES++;
      }
      if (cufps < 19.0f) {
        FRAMES++;
      }
      if (cufps < 14.0f) {
        FRAMES = FRAMES + 1;
      }
      if (FRAMES == 0) {
        FRAMES = 1;
      }
      
      //bVar2 = local_9d != 0;
      //if (FRAMES != 0) {
        for (FRAME = 0; FRAME < FRAMES; FRAME++) {
                  if (FRAME == 0) {
                    tbslotBegin(app_tbset,1);
                  }
                  ProcessGlass(Paused);
                  if ((((((GameMode != 1) && (0xb3 < GameTimer.frame)) && (GlobalTimer.frame % 0x3c == 0x1e)
                        ) && ((fadeval == 0 && (iVar9 = NuSoundKeyStatus(0), iVar9 != 1)))) && (Bonus != 1) )
                     && (Bonus != 3)) {
                    GameMusic(game_music,0);
                  }
                  if (mg_wumpatot != 0) {
                    mg_wumpatot = mg_wumpatot - 1;
                    plr_wumpas.count = plr_wumpas.count + 1;
                  }
                  if (((FRAME == 0) || (FRAMES == 1)) || ((Demo != 0 || (Level == 0x26)))) {
                    DoInput();
                  }
                  if ((Demo != 0) &&
                     (((pad_play != 0 && (iVar9 = NuPs2PadDemoEnd(), iVar9 != 0)) ||
                      (20.0f <= GameTimer.ftime)))) {
                    new_level = 0x23;
                  }
                  UpdateGameSfx();
                  NuXboxSoundUpdate();
                  DBTimerStart(2);
                  if (Paused != 0) {
                        PauseRumble();
                        pNuCam->mtx = GameCam->m;
                        NuCameraSet(pNuCam);
                        if ((Pad[0] != NULL) && ((Pad[0]->oldpaddata & 0xa0) != 0)) {
                          pausestats_frame = (pausestats_frame / 0xb4) * 0xb4 + 0xb4;
                          GameSfx(0x36,NULL);
                        }  
                  }
                  else {
                        crate_wumpa = Paused;
                        mask_crates = Paused;
                        RotateDirectionalLight(ldir,-0x2000,((GameTimer.frame % 0x1e0) * 0x10000) / 0x1e0);
                        for (iVar9 = 0; iVar9 < 9; iVar9++) {
                          Character[iVar9].anim_processed = '\0';
                        }
                        SetLevelLights();
                        SetTexAnimSignals();
                        TBCODESTART(0,"Plats");
                        if ((LDATA->flags & 8) != 0) {
                          TerrainPlatformOldUpdate();
                        }
                        if (world_scene[0] != NULL) {
                           NuGScnUpdate(world_scene[0],1.0f);
                        }
                        edobjUpdateObjects(1.0f);
                        edanimUpdateObjects(1.0f);
                        NuRndrWaterRippleUpdate(1);
                        NuBridgeUpdate(&Character[0].obj.pos);
                        NuWindUpdate(&Character[0].obj.pos);
                        if ((LDATA->flags & 8) != 0) {
                          TerrainPlatformNewUpdate();
                        }
                        TBCODEEND(0);
                        TBCODESTART(1,"Chars");
                        if ((LDATA->flags & 1) != 0) {
                          if (FRAME == 0) {
                            tbslotBegin(app_tbset,5);
                          }
                          ManageCreatures();
                          ProcessCreatures();
                          if (FRAME == 0) {
                            tbslotEnd(app_tbset,5);
                          }
                        }
                        TBCODEEND(1);
                        TBCODESTART(2,"Update");
                        if (Cursor.menu == '\x13') {
                          UpdateCutMovie();
                        }
                        if (level_part_2 == 0) {
                          UpdateLevel();
                          UpdateKabooms();
                          UpdateWumpa();
                          UpdateCrates();
                          UpdateMaskFeathers();
                          UpdateCrateExplosions();
                          UpdateChases();
                          UpdateProjectiles();
                          UpdateBugLight(plr);
                          UpdateGameCut();
                        }
                        ProcessVehicleLevel(Pad[0]);
                        TBCODESTART(3,"Deb");
                        ProcDeb3();
                        if ((NODEBRIS == 0) &&
                           ((Debris(0), GLASSPLAYER == 0 || !(5.0f > plr_invisibility_time)))) {
                          pos.x = (player->obj).pos.x;
                          pos.y = (((player->obj).bot + (player->obj).top) * (player->obj).SCALE) * 0.5f 
                                          + (player->obj).pos.y;
                          pos.z = (player->obj).pos.z;
                          iVar9 = DebrisCollisionCheck(&pos,(player->obj).RADIUS);
                          if (iVar9 != -1) {
                            KillPlayer(&player->obj,0x14);
                          }
                        }
                        //GameCam_ = (CamMtx *)0x20000000;
                        NuMtlAnimate(0.01666667f);
                        TBCODEEND(3);
                        UpdateTempCharacter();
                        UpdateAwards();
                        MoveGameCamera(GameCam,&plr->obj);
                        UpdateScreenWumpas();
                        if (PLAYERCOUNT != 0) {
                          UpdatePlayerStats(plr);
                        }
                        UpdatePanelDebris();
                  }
                  if ((FRAME == FRAMES - 1) && (pause_rndr_on == 0)) {
                    AddBugLight();
                  }
                  GameTiming();
                  if (FRAME == 0) {
                    ProcMenu(&Cursor,Pad[0]);
                  }
                  UpdateFade();
                  if ((Level == 2) && (VEHICLECONTROL != 1)) {
                    NuLightFog(0.0f,0.0f,0x40000000,BLUR,0);
                  }
                  else {
                    NuLightFog(LDATA->fognear,LDATA->fogfar,level_fogcolour,BLUR,
                               (uint)LDATA->hazea << 0x18 | (uint)LDATA->hazeb << 0x10 |
                               (uint)LDATA->hazeg << 8 | (uint)LDATA->hazer);
                  }
                  if (FRAME == 0) {
                    tbslotEnd(app_tbset,1);
                  }
                  DBTimerEnd(2);
                  DBTimerStart(3);
                  if ((((Level == 2) || (Level == 6)) || (Level == 0x18)) || (Level == 0x22)) {
                    GS_SetAlphaCompareForce(1);
                  }
                  else {
                    GS_SetAlphaCompareForce(0);
                  }
                  if (pause_rndr_on == 0) {
                    if (FRAME == FRAMES - 1) {
                      tbslotBegin(app_tbset,9);
                      GS_Parallax = 1;
                      pCam = GameCam;
                      iVar9 = NuRndrBeginScene(1);
                      if (iVar9 != 0) {
                        GameVP();
                        NuRndrClear(0xf,level_clearcolour,1.0f);
                        if (level_part_2 == 0) {
                          DrawParallax();
                        }
                      }
                      NuRndrEndScene();
                      GS_Parallax = 0;
                      NuRndrClear(10,0,1.0f);
                      local_9c = NuRndrBeginScene(1);
                      if (Level == 0x18) {
                        (pNuCam->mtx)._32 = AtmosphericPressureHackedZ;
                        NuCameraSet(pNuCam);
                        DrawWorld();
                        NuRndrEndScene();
                        pNuCam->mtx = pCam->m;
                        NuCameraSet(pNuCam);
                        local_9c = NuRndrBeginScene(1);
                      }
                      if (FRAME == FRAMES - 1) {
                        TBDRAWSTART(0,"Chars");
                      }
                    }
                    //bVar1 = FRAME == FRAMES - 1;
                   // local_9c[1] = (s32)((plr->obj).dead == '\x02');
                   // uVar5 = ((uint)(char)(bVar1 << 1) << 0x1c) >> 0x1d;
                    if ((FRAME == FRAMES - 1) << 0x1d) {
                      tbslotBegin(app_tbset,0xc);
                    }
                    if ((((LDATA->flags & 1) != 0) && (DRAWCREATUREHACK != 0)) && (cut_on == 0)) {
                      if (FRAME == FRAMES - 1) {
                        TBDRAWSTART(1,"Crash");
                      }
                      if (((plr->obj).contact == 0) &&
                         (((GLASSPLAYER == 0 || !(5.0f > plr_invisibility_time)) || (Level == 0x17)))) {
                        pCam = GameCam;
                        DrawCreatures(Character,1,uVar5,1);
                      }
                      if (FRAME == FRAMES - 1) {
                        TBDRAWEND(1);
                      }
                      if ((level_part_2 == 0) && (DrawCreatures(Character + 1,8,uVar5,1), Level == 0x1c)) {
                        DrawCreatures(&OppTubCreature,1,uVar5,0);
                      }
                      DrawNextVehicle(uVar5);
                      if (FRAME == FRAMES - 1) {
                        DrawExtraCreatures();
                        goto LAB_80052b3c;
                      }
                    }
                    else {
        LAB_80052b3c:
                      if (FRAME == FRAMES - 1) {
                        tbslotEnd(app_tbset,0xc);
                      }
                    }
                    if (cut_on == 0) {
                      DrawChases(uVar5);
                      DrawTempCharacter(uVar5);
                      DrawTempCharacter2(uVar5);
                    }
                  }
                  //FRAME = FRAME + 1;
        } //while (FRAME < FRAMES);
      //}
      if (screendump != 0) {
        iVar9 = save_paused;
      } else{
         iVar9 = Paused;
      }
      if ((pause_rndr_on == 0) && (local_9c != 0)) {
        if (iVar9 != 0) {
          fVar3 = 0.0f;
        }
        else {
          fVar3 = 1.0f;
        }
        NuWaterSpeed(fVar3);
        CurrentCausticTexture = (CurrentCausticTexture + 1) % 0x1f;
        TBDRAWSTART(2,"World");
        if ((LDATA->flags & 4) != 0) {
          tbslotBegin(app_tbset,10);
          if (world_vd != NULL) {
                if (pVIS != NULL) {
                 visiSetSplineKnot(world_vd,((s32)pVIS - (s32)world_scene[0]->splines) * -0x55555555 >> 2,iVIS);
                }
          }
          else {
            ApplyVisiTable(world_scene[0],&pCam->pos);
          }
          if ((DRAWWORLDHACK != 0) && (Level != 0x18)) {
            DrawWorld();
          }
          tbslotEnd(app_tbset,10);
        }
        TBDRAWEND(2);
        TBDRAWSTART(3,"Deb");
        if ((cut_on == 0) && (DrawVehicleTrail(), cut_on == 0)) {
          DrawDeb3();
          DrawProjectiles();
        }
        TBDRAWEND(3);
        if ((level_part_2 == 0) && (cut_on == 0)) {
          TBDRAWSTART(4,"Crates");
          DrawCrates();
          TBDRAWEND(4);
          TBDRAWSTART(5,"Wumpa");
          DrawWumpa();
          TBDRAWEND(5);
          TBDRAWSTART(6,"Misc");
          if ((LDATA->flags & 1) != 0) {
            if (((((plr->used != '\0') && ((plr->obj).mask != NULL)) &&
                 ((plr->obj).mask->active != 0)) && ((LDATA->flags & 0xe00) == 0)) &&
               (((VEHICLECONTROL != 1 || ((LBIT & 0x0000000105042000) == 0)) &&
                ((Cursor.menu != '$' && (advice_wait == 0)))))) {
              DrawMask((plr->obj).mask);
            }
            DrawMaskFeathers();
          }
          DrawKabooms();
          DrawTransporters();
          DrawAwards();
          DrawBug();
          DrawRings();
          DrawTarget();
          DrawLevel();
          DrawGameCut();
          if (Level == 0x1d) {
            DrawCarpet();
          }
          NuRndrShadPolys(ShadowMat);
          NuRndrWaterRip(DebMat[4]);
        }
        TBDRAWEND(6);
        if (Cursor.menu == '\x13') {
          DrawCutMovie();
        }
        if (cut_on == 0) {
          NuRndrFootPrints(DebMat[4],foot_u,foot_v);
        }
        tbslotEnd(app_tbset,9);
        NuWaterRender();
        NuRndrEndScene();
        iVar8 = 0;
        if (Cursor.menu != '\f') {
          iVar8 = iVar9;
        }
        NuRndrFx(iVar8,&Character[0].obj.pos);
        tbslotBegin(app_tbset,0xe);
        NuRndrBeginScene(1);
        DrawCrateExplosions();
        NuGlassRenderStatic();
        NuRndrEndScene();
        force_glass_screencopy_enable = 1;
        tbslotEnd(app_tbset,0xe);
        tbslotBegin(app_tbset,2);
        NuRndrBeginScene(1);
        if ((((LBIT & 0x200000a1) != 0) && (cut_on == 0)) &&
           ((Level != 7 || ((sVar4 = (player->obj).RPos.iALONG, sVar4 < 0x67 || (0x91 < sVar4))))) )
        {
          if ((Level == 5) && ((sVar4 = (player->obj).RPos.iALONG, 0x6d < sVar4 && (sVar4 < 0x79)) ))
          {
            if (sVar4 == 0x6e) {
              snowflake_scale = (1.0f - (player->obj).RPos.fALONG);
              DoClouds(Paused);
            }
            if ((player->obj).RPos.iALONG == 0x78) {
              snowflake_scale = (player->obj).RPos.fALONG;
              DoClouds(Paused);
            }
          }
          else {
            snowflake_scale = 1.0f;
            DoClouds(Paused);
          }
        }
        if ((((LDATA->flags & 0x10) != 0) && (NODEBRIS == 0)) && (DRAWDEBRISHACK != 0)) {
          DebrisDraw(iVar9);
        }
        NuRndrEndScene();
        tbslotEnd(app_tbset,2);
      }
      GameVP();
      HandlePauseRender(iVar9);
      TBDRAWSTART(7,"Panel");
      iVar9 = NuRndrBeginScene(1);
      if (iVar9 != 0) {
       // bVar1 = (plr->obj).dead == '\x02';
        if (((plr->obj).dead != '\x02') && (PLAYERCOUNT != 0)) {
          fVar6 = (plr->obj).die_time * 3.0f;
          if (fVar6 > (plr->obj).die_duration) {
            fVar6 = (plr->obj).die_duration;
          }
          uVar5 = ((fVar6 / (plr->obj).die_duration) * 255.0f);
          NuRndrRect2di(0,0,SWIDTH << 4,SHEIGHT << 3,uVar5 | uVar5 << 8 | uVar5 << 0x10 | 0x80000000,fade_mtl);
            
        }
        else {
          if (Cursor.menu == 0x22) {
            iVar9 = (s32)(1.0f - (((POWERTEXTY + 0.7f) + 1.0f) * 0.5f)) * (SHEIGHT);
            NuRndrRect2di(0,iVar9,SWIDTH,(s32)((1.0f - ((POWERTEXTY - 0.75f) + 1.0f) * 0.5f) * 
                                                (SHEIGHT)) - iVar9,0x18777777, fade_mtl);
          }
        }
        NuRndrClear(10,0,1.0f);
        if ((plr->obj).dead != '\x02') {
          DrawCreatures(Character,1,1,0);
        }
        if ((((((LDATA->flags & 1) != 0) && (plr->used != '\0')) && (((plr->obj).mask != NULL &&
              (((plr->obj).mask->active != 0 && ((LDATA->flags & 0xe00) == 0)))))) &&
            ((VEHICLECONTROL != 1 || ((LBIT & 0x0000000105042000) == 0)))) &&
           ((Cursor.menu == '$' || (advice_wait != 0)))) {
          DrawMask((plr->obj).mask);
        }
        DrawPanel();
        if ((new_mode != -1) || (new_level != -1)) {
          fade_rate = 8;
        }
        NuRndrEndScene();
      }
      iVar9 = NuRndrBeginScene(1);
      if (iVar9 != 0) {
        DrawFade();
        NuRndrEndScene();
      }
      TBDRAWEND(7);
      tbslotEnd(app_tbset,0);
      iVar9 = nuvideo_global_vbcnt - frameout_count;
      frameout_count = nuvideo_global_vbcnt;
      frameout = iVar9 - 1;
      if (frameout < 0) {
        frameout = 0;
      }
      if (local_9d != 0) {
        frameout = 0;
        local_9d = local_9d - 1;
      }
      if (pause_rndr_on != 0) {
        local_9d = 2;
      }
      DBTimerEnd(3);
      DBTimerEnd(1);
      NuRndrSwapScreen(1);
      NuDynamicWaterUpdate(0);
      Reseter();
      GC_DiskErrorPoll();
    }
    pause_dir = 0;
    Paused = 0;
    if (((plr_died != 0) && (best_cRPos != NULL)) && (Rail[best_cRPos->iRAIL].type == 0)) {
      LostLife = 1;
    }
    plr_died = 0;
    for(iVar9 = 0; iVar9 < 2; iVar9++) {
      if (NuRndrBeginScene(1) != 0) {
        NuRndrClear(0xb,0,1.0f);
        NuRndrEndScene();
      }
      NuRndrSwapScreen(1);
      iVar8 = new_mode;
    }
    if (new_mode == -1) break;
    PauseGameAudio(0);
    NuSoundStopStream(4);
    NuSoundUpdate();
    GameMode = new_mode;
  }
  NuSoundKillAllAudio();
  NuSoundUpdate();
  ClosePauseRender();
  if (Pad[0] != NULL) {
    NuPs2PadSetMotors(Pad[0],0,0);
  }
  if (Pad[1] != NULL) {
    NuPs2PadSetMotors(Pad[1],0,0);
  }
  ReleaseVisiTable();
  CloseCutMovie(0);
  CloseCreatures();
  CloseCrates();
  InstClose();
  CloseClouds();
  ResetBug();
  NuSceneDestroy(font3d_scene2);
  font3d_scene2 = NULL;
  font3d_initialised = 0;
  font3d_scene = NULL;
  CloseXboxEffectSystem();
  if (PLAYERCOUNT != 0) {
    last_character = (s32)(plr->obj).character;
  }
  else {
    last_character = iVar8;
  }
  if (last_character != -1) {
    last_used_character = last_character;
  }
  last_level = Level;
  last_hub = HubFromLevel(Level);
  GetSpaceCut(last_level,new_level);
  Level = new_level;
  number_of_times_played++;
  goto LAB_80051ba4;
}
*/
