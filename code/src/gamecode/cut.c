#define rate_val 1.0f
short RO_640460[8];

//NGC MATCH
static struct NUHGOBJ_s * InstNuGHGRead(union variptr_u *superbuf_ptr,char *path) {
  s32 ix;
  
  for (ix = 0; ix < ghg_inst_count; ix++) {
        if (strcasecmp(path,ghg_insts[ix].name) == 0) {
         return ghg_insts[ix].scene;
        }
  }
  ghg_insts[ix].name = path;
  ghg_insts[ix].scene = NuHGobjRead(superbuf_ptr,path);
  ghg_inst_count = ghg_inst_count + 1;
  return ghg_insts[ix].scene;
}

//NGC MATCH
void LoadCutSceneCharacters(struct csc_s *name) {
  
  while (name->path != NULL) {
    name->obj = InstNuGHGRead(&superbuffer_ptr,name->path);
    name++;
  }
  return;
}

//PS2
void InitCutScenes(void) {
  NuGCutSceneSysInit(cutscene_locatorfns);
  NuSetCutSceneFindCharactersFn(AppCutSceneFindCharacters);
  NuSetCutSceneCharacterRenderFn(AppCutSceneCharacterRender);
  return;
}

//NGC MATCH
static struct nuscene_s * InstNuGScnRead(union variptr_u *ptr,union variptr_u *end,char *name) {
  s32 ix;
  
  for (ix = 0; ix < scene_inst_count; ix++) {
        if (strcasecmp(name,scene_insts[ix].name) == 0) {
            return (struct nuscene_s *)scene_insts[ix].scene;
        }
  }
  NuMemSetExternal(ptr,end);
  scene_insts[ix].name = name;
  scene_insts[ix].scene = (struct nugscn_s *)NuSceneLoad(name);
  scene_inst_count++;
  NuMemSetExternal(NULL,NULL);
  return (struct nuscene_s *)scene_insts[ix].scene;
}

//NGC MATCH //inline with InitCutScenes??
static void CutSceneEndFunction(void *icutscene) {
    if (cutmovie == 0) {
        NewMenu(&Cursor,0,0,-1);
        if (logos_played == 0) {
            GameMusic(0xaa,0);
        }
    }
    else {
        cut_on = 0;
    }
    return;
}

//NGC MATCH //inline with InitCutScenes??
static void CutSceneNextFunction(void *icutscene) {
    cutworldix++;
    if (CutAudio[cutworldix] != -1) {
        NuSoundStopStream(4);
        do {
            NuSoundUpdate();
        } while (NuSoundKeyStatus(4) == 1);
        gamesfx_channel = 4;
        GameSfx(CutAudio[cutworldix],NULL);
        cutmovie_hold = 1;
    }
    if (fadeval != 0) {
        fade_rate = -8;
    }
    return;
}

//NGC MATCH //inline with InitCutScenes??
static void CutSceneNextLogoFunction(void *icutscene) {
    cutworldix++;
    NewMenu(&Cursor,0,0,-1);
    if (logos_played == 0) {
        GameMusic(0xaa,0);
    }
    return;
}

//NGC MATCH
void NewCut(s32 i) {
    gamecut = i;
    Game.cutbits = (s32) (Game.cutbits | (1 << i));
    music_volume = (s32) Game.music_volume;
}

//NGC MATCH
void GetSpaceCut(s32 last, s32 next) {
    s32 hub;

    gamecut = -1;
    if ((s32) Demo == 0) {
        if (next == 0x25) {
            if ((last == 0x15) && (new_lev_flags & 0x800)) {
                if ((Game.cutbits & 0x20) == 0) {
                    NewCut(5);
                }
            }
        } else {
            hub = HubFromLevel(next);
            switch (hub) {                      
            case 0:
                if (!(Game.cutbits & 2)) {
                    NewCut(1);
                } else if (((s32) Game.hub[0].crystals == 2) && !(Game.cutbits & 4)) {
                    NewCut(2);
                } else if (((s32) Game.hub[0].crystals == 4) && !(Game.cutbits & 8)) {
                    NewCut(3);
                } else if ((next == 0x15) && !(Game.cutbits & 0x10)) {
                    NewCut(4);
                }
                break;
            case 1:
                if (!(Game.cutbits & 0x80)) {
                    NewCut(7);
                } else if (((s32) Game.hub[1].crystals == 2) && !(Game.cutbits & 0x100)) {
                    NewCut(8);
                } else if (((s32) Game.hub[1].crystals == 4) && !(Game.cutbits & 0x200)) {
                    NewCut(9);
                } else if ((next == 0x17) && !(Game.cutbits & 0x400)) {
                    NewCut(0xA);
                }
                break;
            case 2:
                if (!(Game.cutbits & 0x1000)) {
                    NewCut(0xC);
                } else if (((s32) Game.hub[2].crystals == 2) && !(Game.cutbits & 0x2000)) {
                    NewCut(0xD);
                } else if (((s32) Game.hub[2].crystals == 4) && !(Game.cutbits & 0x4000)) {
                    NewCut(0xE);
                } else if ((next == 0x16) && !(Game.cutbits & 0x8000)) {
                    NewCut(0xF);
                }
                break;
            case 3:
                if (!(Game.cutbits & 0x20000)) {
                    NewCut(0x11);
                } else if (((s32) Game.hub[3].crystals == 2) && !(Game.cutbits & 0x40000)) {
                    NewCut(0x12);
                } else if (((s32) Game.hub[3].crystals == 4) && !(Game.cutbits & 0x80000)) {
                    NewCut(0x13);
                } else if ((next == 0x18) && !(Game.cutbits & 0x100000)) {
                    NewCut(0x14);
                }
                break;
            case 4:
                if (!(Game.cutbits & 0x400000)) {
                    NewCut(0x16);
                } else if (((s32) Game.hub[4].crystals == 2) && !(Game.cutbits & 0x800000)) {
                    NewCut(0x17);
                } else if (((s32) Game.hub[4].crystals == 4) && !(Game.cutbits & 0x01000000)) {
                    NewCut(0x18);
                } else if ((next == 0x19) && !(Game.cutbits & 0x02000000)) {
                    NewCut(0x19);
                }
                break;
            }
        }
        if ((s32) gamecut != -1) {
            gamecut_newlevel = next;
            new_level = 0x28;
        }
    }
}

//NGC MATCH //inline with CutLoadScreenThreadProc??
s32 LoadCutComponents(struct cutscenedesc_s *csd,struct csc_s *chars) {
    struct NUGCUTSCENE_s *cutscn;
    s32 ret;
    s32 ix;
    char *path;
    char *final_path;
    struct nudathdr_s *char_dat;
    char pad [6];
    
    char_dat = NULL;
    InitSkinning(0);
    ret = 0;
    CutChar = chars;
    if (loadcut_chardatfile != 0) {
        char_dat = NuDatOpenMem(chardatfilename,NULL,NULL);
        NuDatSet(char_dat);
    }
    LoadCutSceneCharacters(CutChar);
    if (char_dat != NULL) {
        NuDatClose(char_dat);
        NuDatSet(NULL);
    }
    CloseSkinning();

        
    for(ix = 0; csd[ix].gscname != NULL; ix++, ret++) {
        cutdebgroups[ix] = csd[ix].debgroup;
        world_scene2[ix] = InstNuGScnRead(&superbuffer_ptr,&superbuffer_end,csd[ix].gscname);
        world_scene[ix] = world_scene2[ix]->gscene;
        InitXboxGSceneEffects(world_scene[ix],&superbuffer_ptr,&superbuffer_end);
        CutScene[ix] = NuGCutSceneLoad(csd[ix].cutname,&superbuffer_ptr,&superbuffer_end);
        if (CutScene[ix] != NULL) {
            NuGCutSceneFixUp(CutScene[ix],world_scene[ix],NULL);
            path = csd[ix].cutname;
            cutscn = CutScene[ix];
            if (strlen(csd[ix].cutname) != 0x10) {
                path -= 0x10;
                final_path = path + strlen(csd[ix].cutname);
            } else {
                final_path = path;
            }
            CutInst[ix] = instNuGCutSceneCreate(cutscn,NULL,NULL,final_path,&superbuffer_ptr);
            if (CutInst[ix] != NULL) {
                CutInst[ix]->rate = 0.5f;
                CutAudio[ix] = csd[ix].sfxid;
                instNuGCutSceneSetEndCallback(CutInst[ix],CutSceneEndFunction);
                if ((ret != 0) && (CutInst[ix - 1] != 0)) {
                    CutInst[ix - 1]->next_to_play =  CutInst[ix];
                    instNuGCutSceneSetEndCallback(CutInst[ix - 1],CutSceneNextFunction);
                }
            }
        }
    } 
    return ret;
}

//NGC MATCH
u32 CutLoadScreenThreadProc(void * lpParameter) {
    u32 time;
    u32 dwQuantum; //
    
    GetTickCount();
    cut_load_framecount = 0;
    cut_loading_finished = 0;
    do {
        NuRndrClear(0xb,0,1.0f);
        if (NuRndrBeginScene(1) != 0) {
        pNuCam->mtx = numtx_identity;
            NuCameraSet(pNuCam);
            if ((font3d_scene != NULL) && (font3d_initialised != 0)) {
                DrawGameMessage(tLOADING[Game.language],cut_load_framecount,0.0f);
                cut_load_framecount++;
            }
            NuRndrEndScene();
        }
        NuRndrSwapScreen(1);
        Sleep(0x10);
    } while (cut_loading_finished == 0);
    time = GetTickCount();
    return time;
}

//PS2
void AppCutSceneCharacterRender
               (struct instNUGCUTSCENE_s *icutscene,struct NUGCUTSCENE_s *cutscene,struct instNUGCUTCHAR_s *icutchar,
               struct NUGCUTCHAR_s *cutchar,float current_frame)

{
    s32 j;
    s32 is_visible;
    struct numtx_s mtx;
    struct numtx_s tmtx[256];	//global
    float **array_dwa;
    struct NUHGOBJ_s *obj;
    short layertab[8];
    struct NUGCUTLOCATOR_s *locator;
    struct numtx_s locatormtx;
    struct NUGCUTLOCATORSYS_s *locatorsys;
    struct instNUGCUTLOCATORSYS_s *ilocatorsys;
    s32 locix;
    
    memcpy(&layertab, &RO_640460, 0x10);
    obj = icutchar->character;
    
    NuGCutCharAnimProcess(cutchar, current_frame, &mtx, &is_visible, 0, 0, 0);
    if ((icutscene->has_mtx & 0x10) != 0)
    {
        NuMtxMul(&mtx, &mtx, &icutscene->mtx);
    }
    
    if (is_visible != NULL) {
        if ((cutchar->joint_anim != NULL)) {
            NuHGobjEvalAnim2(obj, cutchar->joint_anim, current_frame, 0, NULL, &tmtx);
        }
        else {
            NuHGobjEval(obj, 0, NULL, &tmtx);
        }
        
        if (cutchar->face_anim != NULL) {
            array_dwa = NuHGobjEvalDwa2(obj->num_layers, layertab, cutchar->face_anim, current_frame);
        }
        else {
            array_dwa = 0;
        }
        
        NuHGobjRndrMtxDwa(obj, &mtx, obj->num_layers, layertab, &tmtx, array_dwa);
        if (cutchar->first_locator != 0xff) {
            locatorsys = &cutscene->locators[0];
            ilocatorsys = &icutscene->ilocators[0];
            for(j = 0; j < cutchar->nlocators; j++)
            {
                locix = cutchar->first_locator + j;
                locator = &locatorsys->locators[locix];
                if (locator->joint_ix == 0xff) {
                    instNuGCutLocatorUpdate(icutscene, locatorsys, &ilocatorsys->ilocators[locix], locator, current_frame, &mtx);
                }
                else {
                    NuMtxMulVU0(&locatormtx, &tmtx[locator->joint_ix], &mtx);
                    instNuGCutLocatorUpdate(icutscene, locatorsys, &ilocatorsys->ilocators[locix], &locatorsys->locators[locix], current_frame, &locatormtx);
                }
            }
        }
    }
    return;
}

//PS2
static inline struct NUHGOBJ_s* FindCutChar(char* name)
{
    struct csc_s *chr;
    
    chr = CutChar;
    if (CutChar != NULL) {
        for (; chr->path != NULL; chr++) {
            if (strcasecmp(name,chr->name) == 0) {
                return chr->obj;
            }
        }
    }
    return NULL;
}

//PS2
static void AppCutSceneFindCharacters(struct NUGCUTSCENE_s *cutscene)
{    
    struct NUGCUTCHARSYS_s *charSys;
    struct NUGCUTCHAR_s *cutchar;
    struct NUGCUTLOCATORSYS_s *locator;
    s32 i;
    
    charSys = cutscene->chars;
    locator = cutscene->locators;
    for (i = 0; i < charSys->nchars; i++)
    {
        cutchar = &charSys->chars[i];
        cutchar->character = FindCutChar(cutchar->name);
        if (cutchar->character == NULL) {
            NuDebugMsgProlog(".\\cut.c", 0x292)
            ("AppCutSceneFindCharacters: cannot fixup cutscene character <%s>", cutchar->name);
        }
        
        if ((cutchar->nlocators != 0) && ((s32)cutchar->locators < 0xff ))
        {
            cutchar->first_locator = (u8)cutchar->locators;
            cutchar->locators = &locator->locators[((s32)cutchar->locators)];
        }
        else {  
            cutchar->first_locator = -1;
        }
    }
    return;
}

//NGC MATCH //PS2 MATCH
void SetCutMovieRate() {
    CutInst[cutworldix]->rate = 0.5f;

    if ((cutmovie == 3 || cutmovie == 4) && cutworldix == 0) {
        CutInst[cutworldix]->rate *= 0.8f;
    }

    CutInst[cutworldix]->rate *= rate_val;
    
    if (cutratefrig != 0) {
        CutInst[cutworldix]->rate = cutratefrig;
    }
}

//NGC MATCH
void UpdateCutMovie(void) {
    if (((cutmovie_hold == 0) || (CutAudio[cutworldix] == -1)) || (NuSoundKeyStatus(4) != 1)) {
        CutInst[cutworldix]->rate = 0.0f;
    }
    else {
        cutmovie_hold = 0;
        SetCutMovieRate();
    }
    NuGCutSceneSysUpdate(Paused);
    return;
}

//PS2
void UpdateCutMovieCamera(struct CamMtx *cam)
{
  if (set_cutscenecammtx != 0) {
    memcpy(&cam->m, &cutscenecammtx, sizeof(struct numtx_s));
    set_cutscenecammtx = 0;
    memcpy(&pNuCam->mtx, &cam->m, sizeof(struct numtx_s));  
    NuCameraSet(pNuCam);
  }
  return;
}

//NGC MATCH
void StartCutMovie(void) {
    s32 sfx;
    
    cutmovie_hold = 0;
    if ((CutScene[0] != NULL) && (CutInst[0] != NULL)) {
        instNuGCutSceneStart(CutInst[0]);
        if ((cutmovie == 0) && (logos_played != 0)) {
            GameMusic(0xaa,0);
            sfx = -1;
            CutInst[cutworldix]->cframe =
                 (CutScene[cutworldix]->nframes - TITLESTARTTIME * TITLEFPS) + 0.0f;
            texanimbits = (texanimbits & 0xFFFFFFFD) | 4;
        }
        else {
            sfx = CutAudio[cutworldix];
            texanimbits = (texanimbits | 2) & 0xFFFFFFFB;
            if (cutmovie == 0) {
                CutInst[0]->cframe = 300.0f;
            }
        }
        if ((sfx != -1) && ((cutmovie != 0 || (logos_played == 0)))) {
            NuSoundStopStream(4);
            gamesfx_channel = 4;
            GameSfx(sfx,NULL);
            cutmovie_hold = 1;
        }
    }
    return;
}

//NGC MATCH
void DrawCutMovie(void) {
    NuGCutSceneSysRender();
    return;
}

//NGC MATCH
void InitGameCut(void) {
    
    cutang_FRONTEND[0] = (u16)NuAtan2D(cutpos_CRASH.x - cutpos_FRONTEND[0].x,cutpos_CRASH.z - cutpos_FRONTEND[0].z);
    cutang_FRONTEND[1] = (u16)NuAtan2D(cutpos_CRASH.x - cutpos_FRONTEND[1].x,cutpos_CRASH.z - cutpos_FRONTEND[1].z);
    cutang_SPACE[0] = (u16)NuAtan2D(campos_SPACE.x - cutpos_SPACE[0].x,campos_SPACE.z - cutpos_SPACE[0].z);
    cutang_SPACE[1] = (u16)NuAtan2D(campos_SPACE.x - cutpos_SPACE[1].x,campos_SPACE.z - cutpos_SPACE[1].z);
    cutang_SPACE[2] = (u16)NuAtan2D(campos_SPACE.x - cutpos_SPACE[2].x,campos_SPACE.z - cutpos_SPACE[2].z);
    pCutAnim = (struct pcutanm_s *)SpaceGameCutTab[gamecut][1];
    ResetAnimPacket(&CutAnim,(int)pCutAnim->action);
    gamecut_finished = 0;
    gamecut_hold = 1;
    gamecut_sfx = -1;
    ResetAnimPacket(&CutVortexAnim,0x22);
    return;
}

//NGC MATCH
void UpdateGameCut(void) {
    float dt;
    struct CharacterModel* model;
    s32 i;

    if (((GameMode != 1) && (gamecut_start != 0)) && ((Level == 0x25 || (Level == 0x28)))) {
        new_mode = 1;
        gamecut_hold = 1;
        gamecut_newlevel = -1;
        gamecut_start = 0;
        gamecut_finished = 0;
        gamecut = 0;
    } else if (GameMode == 1) {
        if (CutTimer.frame == 0) {
            InitGameCut();
            NewGameCutAnim();
        }
        if (gamecut_finished == 0) {
            model = NULL;
            if ((pCutAnim->character >= 0) && (pCutAnim->character < 0xbf) && (CRemap[pCutAnim->character] != -1)) {
                model = &CModel[CRemap[pCutAnim->character]];
            }
            CutAnim.oldaction = CutAnim.action;
            CutAnim.newaction = pCutAnim->action;
            if (model != NULL) {
                if (((gamecut_hold != 0) && (gamecut_sfx != -1)) && (NuSoundKeyStatus(4) != 1)) {
                    dt = 0.0f;
                } else {
                    dt = 0.5f;
                    gamecut_hold = 0;
                }
                UpdateAnimPacket(model, &CutAnim, dt, 0.0f);
            }
            UpdateTimer(&CutTimer);
            if (fadeval == 0) {
                if ((Pad[0] != NULL) && ((Pad[0]->oldpaddata & 0x840) != 0)) {
                    Game.music_volume = music_volume[3];
                    if (Level == 0x28) {
                        new_level = gamecut_newlevel;
                    } else {
                        new_mode = 0;
                    }
                }
            }
            if ((model == NULL) || ((CutAnim.flags & 1) != 0)) {
                i = pCutAnim->character;
                pCutAnim++;
                if (pCutAnim->character != -1) {
                    if (pCutAnim->character != i) {
                        ResetAnimPacket(&CutAnim, pCutAnim->action);
                    }
                    NewGameCutAnim();
                } else {
                    gamecut_finished = 1;
                }
            }
        }
        if (gamecut_finished != 0) {
            Game.music_volume = music_volume[3];
            if (Level == 0x28) {
                if (gamecut_newlevel != -1) {
                    new_level = gamecut_newlevel;
                } else {
                    new_level = 0x25;
                }
            } else {
                new_mode = 0;
            }
        }
        if (Level == 0x28) {
            i = CRemap[183];
            if (i != -1) {
                model = &CModel[i];
                CutVortexAnim.oldaction = CutVortexAnim.action;
                CutVortexAnim.newaction = 0x22;
                UpdateAnimPacket(model, &CutVortexAnim, 0.5f, 0.0f);
            }
            GameSfxLoop(0xcd, NULL);
        }
    }
    if (gamecut == 5) {
        gamecut_hack = 1;
    }
    if (new_mode == 0) {
        last_hub = -1;
        tumble_time = tumble_duration = 0.0f;
    }
    return;
}

//NGC MATCH
void PlayCutMovie(s32 movie) {
    s32 quit;
    char pad [0x40];
    
    PANEL3DMULX = (float)SWIDTH / (DIVPANEL3DX * ((float)SWIDTH / (float)PHYSICAL_SCREEN_X));
    PANEL3DMULY = (float)SHEIGHT / (DIVPANEL3DY * ((float)SHEIGHT / (float)PHYSICAL_SCREEN_Y));
    
    do {
        next_cut_movie = -1;
        GetTickCount();
        ResetSuperBuffer2();
        DebrisSetup();
        DebrisRegisterCutoffCameraVec((struct NuVec *)&global_camera.mtx._30);
        ParticleReset();
        edppDestroyAllParticles();
        edppDestroyAllEffects();
        if (NuFileExists("levels\\b\\intro\\intro.ptl") != 0) {
            edppLoadEffects("levels\\b\\intro\\intro.ptl", 1);
        }
        if (NuFileExists("stuff\\general.ptl") != 0) {
            edppMergeEffects("stuff\\general.ptl", 0);
        }
        edppRestartAllEffectsInLevel();
        CreateFadeMtl();
        font3d_scene = NULL;
        texanimbits = 0;
        InitXboxEffectSystem(Level);
        NuSoundKillAllAudio();
        NuSoundKillAllAudio();
        if (LoadCutMovie(movie) != 0) {
            GetTickCount();
            StartCutMovie();
            ResetTimer(&GameTimer);
            Paused = 0;
            quit = 0;
            MaxVP();
            cut_on = 1;
            fadeval = 0xff;
            fade_rate = -8;

            do {
                DebrisSetRenderGroup(cutdebgroups[cutworldix]);
                DoInput();
                UpdateGameSfx();
                NuXboxSoundUpdate();
                UpdateCutMovie();
                if (cut_on != 0) {
                    UpdateCutMovieCamera(GameCam);
                    Debris(0);
                    if (world_scene[cutworldix] != NULL) {
                        NuGScnUpdate(1.0f, world_scene[cutworldix]);
                    }
                    GameTiming();
                    if (cutmovie == 0) {
                        ProcMenu(&Cursor, Pad[0]);
                    }
                    UpdateFade();
                    if (cutmovie != 0) {
                        if ((Pad[0] != NULL) && ((Pad[0]->oldpaddata & 0x840) != 0))
                        {
                            next_cut_movie = -1;
                            quit = 1;
                        }
                    }
                    NuMtlAnimate(0.016666668f);
                    NuTexAnimSetSignals(texanimbits);
                    NuTexAnimProcess();
                    pCam = GameCam;
                    if (NuRndrBeginScene(1) != 0) {
                        NuRndrClear(0xb, 0, 1.0f);
                        if (!quit) {
                            SetCutSceneLights();
                            DrawCutMovie();
                            if (world_scene[cutworldix] != NULL) {
                                NuGScnRndr3(world_scene[cutworldix]);
                            }
                            if (cutmovie != 0) {
                                DrawFade();
                            }
                        }
                        NuRndrEndScene();
                        if (NuRndrBeginScene(1) != 0) {
                            NuWaterRender();
                            NuRndrEndScene();
                        }
                        if (NuRndrBeginScene(1) != 0) {
                            DebrisDraw(0);
                            NuRndrEndScene();
                        }
                        if ((cutmovie == 0) && (NuRndrBeginScene(1) != 0)) {
                            NuRndrClear(10, 0, 1.0f);
                            DrawPanel();
                            NuRndrEndScene();
                            if (NuRndrBeginScene(1) != 0) {
                                DrawFade();
                                NuRndrEndScene();
                            }
                        }
                    }
                    NuRndrSwapScreen(1);
                    
                    if (cufps < 35.0f) {
                        CutInst[cutworldix]->cframe = CutInst[cutworldix]->cframe + CutInst[cutworldix]->rate;
                    }
                    if (cufps < 25.0f) {
                        CutInst[cutworldix]->cframe = CutInst[cutworldix]->cframe + CutInst[cutworldix]->rate;
                    }
                    if (cufps < 16.0f) {
                        CutInst[cutworldix]->cframe = CutInst[cutworldix]->cframe + CutInst[cutworldix]->rate;
                    }
                    Reseter(0);
                    GC_DiskErrorPoll();
                }
                
                if (cutmovie != 0) {
                    if (cut_on == 0 || quit == 0) {
                        break;
                    }
                } else if ((fadeval > 0xFE) && (fadehack == 0)) {
                    break;
                }
            } while (1);
        }
        
        NuSoundUpdate();
        NuSoundStopStream(0);
        NuSoundStopStream(1);
        NuSoundStopStream(2);
        NuSoundStopStream(3);
        NuSoundStopStream(4);
        NuSoundUpdate();
        if (NuRndrBeginScene(1) != 0) {
            NuRndrClear(0xb, 0, 1.0f);
            NuRndrEndScene();
            NuRndrSwapScreen(1);
        }
        CloseXboxEffectSystem();
        NuSceneDestroy(font3d_scene2);
        CloseCutMovie(1);
        movie = cutmovie = next_cut_movie;
    } while( cutmovie >= 0 );

    DebrisSetRenderGroup(0);
    return;
}

//PS2
void CloseCutMovie(s32 all)
{
    s32 i;
  
    for(i = 0; i < 0x20; i++)
    {
        if (CutInst[i] != NULL) {
            instNuGCutSceneDestroy(CutInst[i]);
            CutInst[i] = NULL;
        }
    }
    
    if ((ghg_inst_count != 0)) { 
      
        for(i = 0; i < ghg_inst_count; i++ ) {
            NuHGobjDestroy(ghg_insts[i].scene);
        }
    }
    
    ghg_inst_count = 0;
    for(i = 0; i < 0x20; i++)
    {
        world_scene[i] = NULL;
    }
    NuSoundUpdate();
    NuSoundStopStream(0);
    NuSoundStopStream(1);
    NuSoundStopStream(2);
    NuSoundStopStream(3);
    NuSoundStopStream(4);
    NuSoundUpdate();
    //NuSoundFlushLoops();	//PS2
    //UnLoadGBABG();  //NGC
    if (all != 0) {
        NuTexInit();
        NuMtlInit(); //NuMtlRelease(); //PS2
        NuGobjInit();
    }
    return;
}


//part of cutscene_locatorfns variable
static void locatorfn_fadeout(struct instNUGCUTSCENE_s *icutscene,struct NUGCUTLOCATORSYS_s *locatorsys, struct instNUGCUTLOCATOR_s *ilocator,struct NUGCUTLOCATOR_s *locator,struct numtx_s *wm) {
    float currf;
    nuanimtime_s atime;
    float remaining;
    
    if (locator->anim != NULL) {
        NuAnimData2CalcTime(locator->anim,currf,&atime);
        if ( (NuGCutLocatorIsVisble(locator,currf,&atime,NULL) != 0) && (ilocator->data == NULL)) {
            ilocator->data = (void *)0x1;
            remaining = 255.0f / (icutscene->cutscene->nframes - icutscene->cframe);
            if (1.0f <= remaining) {
                fade_rate = (s32)remaining;
            }
            else {
                fade_rate = 1;
            }
        }
    }
    return;
}

//NGC MATCH
s32 LoadCutMovie(s32 movie) {
    s32 i = 0;
    s32 scenecnt;
    union variptr_u ptr;
    struct nudathdr_s *dfcut;

    NuSoundKillAllAudio();
    XbWaitForAllBuffersToStop();
    LoadFont3D();
    MAHLoadingMessage();
    hCutLoadScreenThread = (void *)CreateThread(0,0,(s32)CutLoadScreenThreadProc,0,0,0);
    if (hCutLoadScreenThread != NULL) {
        SetThreadPriority((s32)hCutLoadScreenThread,0xf);
    }
    pause_scene = NULL;
    scene_inst_count = 0;
    for (i = 0x20; i > 0; i--) { 
        world_scene[i - 1] = NULL;
    }
    ghg_inst_count = 0;
    for (i = 0; i < 0x20; i++) {
        CutScene[i] = 0;
        CutInst[i] = 0;
        CutAudio[i] = -1;
    }
    CutChar = NULL;
    cutworldix = 0;
    NuTexAnimProgSysInit();
    switch (movie)
        {

            case 0:
                        ptr.voidptr = texanimbuff;
                        LoadGBABG();
                        NuTexAnimProgReadScript(&ptr,"ats\\levels\\title.ats");
                        dfcut = NuDatOpenMem("levels\\b\\titles\\titles.dat",NULL,NULL);
                        NuDatSet(dfcut);
                        scenecnt = LoadCutComponents(csd_titles,csc_titles);
                        NuDatSet(NULL);
                        if (dfcut != NULL) {
                            NuDatClose(dfcut);
                        }
                        instNuGCutSceneSetEndCallback(CutInst[0],CutSceneNextLogoFunction);
                        CutInst[scenecnt - 1]->looping = 1;
                        instNuGCutSceneSetEndCallback(CutInst[scenecnt - 1],NULL);
            break;
            case 1:
                           LoadCutComponents(csd_s32ro1,csc_s32ro1);
                           next_cut_movie = 2;     
            break;
            case 2:
                        ptr.voidptr = texanimbuff;
                        NuTexAnimProgReadScript(&ptr,"ats\\splash\\ripples.ats");
                        LoadCutComponents(csd_s32ro2,csc_s32ro2);
            break;
            case 3:
            case 4:
                        ptr.voidptr = texanimbuff;
                        NuTexAnimProgReadScript(&ptr,"ats\\elecy\\elecy2.ats");
                        if (movie == 4) {
                            NuTexAnimProgReadScript(&ptr,"ats\\splash\\ripples.ats");
                        }
                        if (movie == 3) {
                            dfcut = NuDatOpenMem("levels\\b\\outro\\outro.dat",NULL,NULL);
                            NuDatSet(dfcut);
                            LoadCutComponents(csd_outro,csc_outro);
                            NuDatSet(NULL);
                            if (dfcut != NULL) {
                                NuDatClose(dfcut);
                            }
                        } else {
                            loadcut_chardatfile = 1;
                            strcpy(chardatfilename, "levels\\b\\outro2\\outro2chars.dat");
                            LoadCutComponents(csd_outro2,csc_outro2);
                            loadcut_chardatfile = 0;
                            
                        }
            break;
            default:
                
            break;
        }
    return (CutInst[0] != NULL);
}

//NGC MATCH
void NewGameCutAnim(void) {
    s32 sfx;
    
    gamecut_hold = 1;
    gamecut_sfx = (s32)pCutAnim->sfx;
    if (gamecut_sfx != -1) {
        NuSoundStopStream(4);
        gamesfx_channel = 4;
        GameSfx(gamecut_sfx,NULL);
    }
    if (pCutAnim->action == 0xc) {
        sfx = 0x1e;
    }
    else  if (pCutAnim->action == 0xd) {
            sfx = 0x21;
    }
    else {
        sfx = -1;
    }
    if (sfx != -1) {
        GameSfx(sfx,NULL);
    }
    return;
}

//NGC MATCH
void DrawGameCut(void) {
    struct numtx_s m;
    float** dwa;
    struct CharacterModel* model;
    u16 yrot;
    struct nuvec_s v;
    struct nuvec_s s;
    int i;

    if (Level == 0x28) {
        if (CRemap[183] != -1) {
            model = &CModel[CRemap[183]];
            NuMtxSetTranslation(&m, &campos_SPACE);
            EvalModelAnim(model, &CutVortexAnim, &m, tmtx, &dwa, NULL);
            NuHGobjRndrMtxDwa(model->hobj, &m, 1, NULL, tmtx, dwa);
        }
    }
    if (GameMode != 1) {
        return;
    }
    if (gamecut_finished != 0) {
        return;
    }
    if (CRemap[pCutAnim->character] != -1) {
        model = CModel + CRemap[pCutAnim->character];
    } else {
        model = NULL;
    }
    if (model == NULL) {
        return;
    }
    v.x = (model->character == 0xB3 || model->character == 0xB4) ? 0.77f : 1.0f;
    if (Level == 0x25) {
        v.x *= HUBHOLOSCALE;
    }
    v.y = v.z = v.x;
    NuMtxSetScale(&m, &v);
    if (Level == 0x28) {
        i = pCutAnim->i;
        s = cutpos_SPACE[i];
        yrot = cutang_SPACE[i];
        yrot -= 0x8000;
        if (model->character == 0x55 || model->character == 0x57 || model->character == 0x56 || model->character == 0x58 || model->character == 2) {
            s.y = SPACEMASKADJUSTY * v.y + s.y;
        }
    } else {
        i = pCutAnim->i;
        yrot = cutang_FRONTEND[i];
        yrot -= 0x8000;
        s = cutpos_FRONTEND[i];
        if (model->character == 0xb5) {
            s.y = FRONTENDCORTEXADJUSTY * v.y + s.y;
        }
    }
    NuMtxRotateY(&m, yrot);
    NuMtxTranslate(&m, &s);
    EvalModelAnim(model, &CutAnim, &m, tmtx, &dwa, NULL);
    NuHGobjRndrMtxDwa(model->hobj, &m, 1, NULL, tmtx, dwa);
    return;
}

//NGC MATCH
void MakeDirLightColour(s32 ix,float r,float g,float b) {
  cutdircol[ix].r = r;
  cutdircol[ix].g = g;
  cutdircol[ix].b = b;
  return;
}

//NGC MATCH
void MakeLightDirection(s32 ix,float x,float y,float z) {
  cutdir[ix].x = x;
  cutdir[ix].y = y;
  cutdir[ix].z = z;
  NuVecNorm(&cutdir[ix],&cutdir[ix]);
  return;
}

//NGC MATCH
void SetCutSceneLights(void) {
    
    switch (cutmovie) {
        case 1:
            if (cutworldix == 2) {
                cutambcol.x = 0.4f;
                cutambcol.y = 0.4f;
                cutambcol.z = 0.4f;
                MakeDirLightColour(0, 0.4f, 0.8f, 0.4f);
                MakeDirLightColour(1, 0.4f, 0.8f, 0.4f);
                MakeDirLightColour(2, 0.0f, 0.0f, 0.0f);
                MakeLightDirection(0, -0.5f, -1.0f, -0.5f);
                MakeLightDirection(1, 0.5f, -1.0f, 0.5f);
                MakeLightDirection(2, 0.0f, 0.0f, 0.0f);
                 SetLights(
                        &cutdircol[0], &cutdir[0], &cutdircol[1], &cutdir[1], &cutdircol[2], &cutdir[2], &cutambcol
                    );
                return;
            }
            SetLevelLights();
            return;
        case 2:
            switch (cutworldix) {
                default:
                    SetLevelLights();
                return;
                case 0:
                    cutambcol.x = 0.75f;
                    cutambcol.y = 0.75f;
                    cutambcol.z = 0.75f;
                    MakeDirLightColour(0, 1.25f, 1.0f, 0.65);
                    MakeDirLightColour(1, 0.5f, 0.5f, 0.8f);
                    MakeDirLightColour(2, 0.5f, 0.5f, 0.8f);
                    MakeLightDirection(0, -0.5f, 1.0f, -0.5f);
                    MakeLightDirection(1, -0.5f, -1.0f, -0.5f);
                    MakeLightDirection(2, 0.5f, -1.0f, 0.5f);
                    SetLights(
                        &cutdircol[0], &cutdir[0], &cutdircol[1], &cutdir[1], &cutdircol[2], &cutdir[2], &cutambcol
                    );
                    return;
                case 1:
                    cutambcol.x = 0.25f;
                    cutambcol.y = 0.25f;
                    cutambcol.z = 0.25f;
                    MakeDirLightColour(0, 1.0f, 0.8f, 0.65);
                    MakeDirLightColour(1, 0.75f, 0.75f, 0.75f);
                    MakeDirLightColour(2, 1.5f, 1.5f, 1.0f);
                    MakeLightDirection(0, -0.5f, 1.0f, 0.5f);
                    MakeLightDirection(1, 0.5f, 1.0f, -0.5f);
                    MakeLightDirection(2, -1.0f, 0.0f, 0.0f);
                    //MakeLightDirection(2, 0.0f, 0.0f, 0.0f);
                    SetLights(
                        &cutdircol[0], &cutdir[0], &cutdircol[1], &cutdir[1], &cutdircol[2], &cutdir[2], &cutambcol);
                    return;
                case 2:
                    cutambcol.x = 0.5f;
                    cutambcol.y = 0.5f;
                    cutambcol.z = 0.5f;
                    MakeDirLightColour(0, 1.0f, 1.0f, 0.5f);
                    MakeDirLightColour(1, 0.35, 0.35, 0.6);
                    MakeDirLightColour(2, 0.35, 0.35, 0.5f);
                    MakeLightDirection(0, -0.5f, 1.0f, -0.5f);
                    MakeLightDirection(1, -0.5f, -1.0f, -0.5f);
                    MakeLightDirection(2, -1.0f, 0.0f, 0.0f);
                    SetLights(
                        &cutdircol[0], &cutdir[0], &cutdircol[1], &cutdir[1], &cutdircol[2], &cutdir[2], &cutambcol
                    );
                    return;
                case 3:
                    return;
                case 4:
                    cutambcol.x = 0.75f;
                    cutambcol.y = 0.75f;
                    cutambcol.z = 0.75f;
                    MakeDirLightColour(0, 0.9, 0.9, 0.5f);
                    MakeDirLightColour(1, 0.0f, 0.0f, 0.0f);
                    MakeDirLightColour(2, 0.0f, 0.0f, 0.0f);
                    MakeLightDirection(0, -0.5f, 1.0f, -0.5f);
                    MakeLightDirection(1, 0.0f, 0.0f, 0.0f);
                    MakeLightDirection(2, 0.0f, 0.0f, 0.0f);
                    SetLights(
                        &cutdircol[0], &cutdir[0], &cutdircol[1], &cutdir[1], &cutdircol[2], &cutdir[2], &cutambcol
                    );
                    return;
                case 5:
                    cutambcol.x = 0.25f;
                    cutambcol.y = 0.25f;
                    cutambcol.z = 0.25f;
                    MakeDirLightColour(0, 1.1, 1.1, 0.9);
                    MakeDirLightColour(1, 1.1, 1.1, 0.9);
                    MakeDirLightColour(2, 0.5f, 0.5f, 0.5f);
                    MakeLightDirection(0, 0.25f, -1.0f, 0.0f);
                    MakeLightDirection(1, -0.5f, 1.0f, 0.0f);
                    MakeLightDirection(2, 0.25f, 1.0f, 0.0f);
                    SetLights(
                        &cutdircol[0], &cutdir[0], &cutdircol[1], &cutdir[1], &cutdircol[2], &cutdir[2], &cutambcol
                    );
                    return;
                case 6:
                    cutambcol.x = 0.75f;
                    cutambcol.y = 0.75f;
                    cutambcol.z = 0.75f;
                    MakeDirLightColour(0, 0.8f, 0.8f, 0.5f);
                    MakeDirLightColour(1, 0.0f, 0.25f, 0.0f);
                    MakeDirLightColour(2, 0.0f, 0.0f, 0.0f);
                    MakeLightDirection(0, 0.0f, 0.0f, -0.8f);
                    MakeLightDirection(1, 0.0f, -1.0f, 1.0f);
                    MakeLightDirection(2, 0.0f, 0.0f, 0.0f);
                    SetLights(
                        &cutdircol[0], &cutdir[0], &cutdircol[1], &cutdir[1], &cutdircol[2], &cutdir[2], &cutambcol
                    );
                    return;
                case 7:
                    cutambcol.x = 0.5f;
                    cutambcol.y = 0.5f;
                    cutambcol.z = 0.5f;
                    MakeDirLightColour(0, 1.0f, 1.0f, 0.8f);
                    MakeDirLightColour(1, 0.25f, 0.25f, 1.0f);
                    MakeDirLightColour(2, 0.0f, 0.0f, 0.0f);
                    MakeLightDirection(0, -1.0f, 0.75f, -1.0f);
                    MakeLightDirection(1, 1.0f, 0.5f, 1.0f);
                    MakeLightDirection(2, 0.0f, 0.0f, 0.0f);
                    SetLights(
                        &cutdircol[0], &cutdir[0], &cutdircol[1], &cutdir[1], &cutdircol[2], &cutdir[2], &cutambcol
                    );
                    return;
            }
            break;
        case 3:
            if (cutworldix == 0) {
                cutambcol.x = 0.5f;
                cutambcol.y = 0.5f;
                cutambcol.z = 0.5f;
                MakeDirLightColour(0, 0.25f, 0.25f, 1.75);
                MakeDirLightColour(1, 1.25f, 0.5f, 0.5f);
                MakeDirLightColour(2, 0.5f, 0.75f, 0.5f);
                MakeLightDirection(0, -1.0f, -0.5f, -1.0f);
                MakeLightDirection(1, 1.0f, -0.5f, -1.0f);
                MakeLightDirection(2, -1.0f, 0.5f, 1.0f);
                SetLights(
                    &cutdircol[0], &cutdir[0], &cutdircol[1], &cutdir[1], &cutdircol[2], &cutdir[2], &cutambcol
                );
                return;
            } else{
                SetLevelLights();
                return;
            }

            break;
        case 4:
            switch (cutworldix) {
                default:
                    SetLevelLights();
                return;
                case 0:
                    cutambcol.x = 0.5f;
                    cutambcol.y = 0.5f;
                    cutambcol.z = 0.5f;
                    MakeDirLightColour(0, 0.25f, 0.25f, 1.75);
                    MakeDirLightColour(1, 1.25f, 0.5f, 0.5f);
                    MakeDirLightColour(2, 0.5f, 0.75f, 0.5f);
                    MakeLightDirection(0, -1.0f, -0.5f, -1.0f);
                    MakeLightDirection(1, 1.0f, -0.5f, -1.0f);
                    MakeLightDirection(2, -1.0f, 0.5f, 1.0f);
                    SetLights(
                        &cutdircol[0], &cutdir[0], &cutdircol[1], &cutdir[1], &cutdircol[2], &cutdir[2], &cutambcol
                    );
                    return;
                case 1:
                    cutambcol.x = 0.5f;
                    cutambcol.y = 0.5f;
                    cutambcol.z = 0.5f;
                    MakeDirLightColour(0, 0.75f, 0.65, 0.5f);
                    MakeDirLightColour(1, 0.0f, 0.15, 0.0f);
                    MakeDirLightColour(2, 0.5f, 0.5f, 0.25f);
                    MakeLightDirection(0, 1.0f, -0.25f, 1.0f);
                    MakeLightDirection(1, 0.0f, -1.0f, 0.0f);
                    MakeLightDirection(2, 1.0f, -0.5f, -1.0f);
                    SetLights(
                        &cutdircol[0], &cutdir[0], &cutdircol[1], &cutdir[1], &cutdircol[2], &cutdir[2], &cutambcol
                    );
                    return;
                case 2:
                    cutambcol.x = 0.5f;
                    cutambcol.y = 0.5f;
                    cutambcol.z = 0.5f;
                    MakeDirLightColour(0, 1.15, 1.15, 1.15);
                    MakeDirLightColour(1, 1.0f, 1.0f, 1.0f);
                    MakeDirLightColour(2, 1.0f, 1.0f, 1.0f);
                    MakeLightDirection(0, 1.0f, -0.25f, 1.0f);
                    MakeLightDirection(1, 0.0f, -1.0f, 0.0f);
                    MakeLightDirection(2, 1.0f, -0.5f, -1.0f);
                    SetLights(
                        &cutdircol[0], &cutdir[0], &cutdircol[1], &cutdir[1], &cutdircol[2], &cutdir[2], &cutambcol
                    );
                    return;
            }
            break;
        case 0:
            SetLevelLights();
            return;
    }
    SetLevelLights();
    return;
}