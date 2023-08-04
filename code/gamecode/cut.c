//PS2
void InitCutScenes(void) {
  NuGCutSceneSysInit(cutscene_locatorfns);
  NuSetCutSceneFindCharactersFn(AppCutSceneFindCharacters);
  NuSetCutSceneCharacterRenderFn(AppCutSceneCharacterRender);
  return;
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