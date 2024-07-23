//#include "gamecode/game_obj.h"

/* TODO
	ObjectCylinderCollision
	PlayerCreatureCollisions
	CrateCollisions
	ObjectRotation
	KillGameObject
	AddProjectile
	UpdateProjectiles
	DrawProjectiles
*/

//NGC MATCH
void ClearGameObjects(void) {
  s32 i;
  
  for (i = 0; i < 0x40; i++) {
    pObj[i] = NULL;
  }
  return;
}

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
void ResetGameObject(struct obj_s *obj) {
  memset(obj,0,0x188);
  obj->reflect_y = 2000000.0f;
  obj->SCALE = 1.0f;
  (obj->RPos).iRAIL = -1;
  (obj->RPos).iALONG = -1;
  obj->layer_type = -1;
  obj->roof_type = -1;
  obj->scale = 1.0f;
  return;
}

//NGC MATCH
s32 AddGameObject(struct obj_s *obj,void *parent) {
  s32 i;
  s32 ok;
  
  ResetGameObject(obj);
  i = 0;
  while ((i < 0x40) && (pObj[i] != NULL)) {
    i++;
  } 
  if (i < 0x40) {
    ok = 1;
    pObj[i] = obj;
    obj->parent = (struct obj_s *)parent;
    obj->i = i;
  }
  else {
    ok = 0;
  }
  CountGameObjects();
  return ok;
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

//NGC MATCH
float GameObjectRadius(struct obj_s *obj) {
  float r;

  r = obj->radius * obj->SCALE;
  if ((obj->attack & 8) != 0) {
    return r + 0.5f;
  }
  if ((obj->attack & 4) != 0) {
    return r + 0.25f;
  }
  if ((obj->attack & 2) != 0) {
      if ((obj->character == 1) && (*(short *)((int)&obj->parent[8].startpos.y + 2) != 0)) {
        return r * 2.5f;
      } else {
        return r + r;
      }
  }
  return r;
}

//NGC 99% (regswap)
s32 CylinderCuboidOverlapXZ(struct nuvec_s* pos, float radius, struct obj_s* cub, struct nuvec_s* cub_pos) {
    struct nuvec_s vNew;
    float x0;  // f5
    float z0;  // f7
    float x1;  // f6
    float z1;  // f11
    float dx;  // f1
    float dz;  // f2
    float xc;  // f30
    float zc;  // f31
    float _x0; // f8
    float _z0; // f12
    float _x1; // f9
    float _z1; // f0
    unsigned short yrot; // r8
    s32 corner; // r9

    NuVecSub(&vNew, pos, cub_pos);
    NuVecRotateY(&vNew, &vNew, -cub->hdg);
    
    _x0 = cub->min.x * cub->SCALE;
    x0 = _x0 - radius;
    if (vNew.x < x0) {
        return 0;
    }

    _x1 = cub->max.x * cub->SCALE;
    x1 = _x1 + radius;
    if (vNew.x > x1) {
        return 0;
    }

    _z0 = cub->min.z * cub->SCALE;
    z0 = (_z0 - radius);
    if (vNew.z < z0) {
        return 0;
    }

    _z1 = cub->max.z * cub->SCALE;
    z1 = (_z1 + radius);
    if (vNew.z > z1) {
        return 0;
    }

    yrot = 0;
    if (vNew.x < _x0) {
        if (vNew.z < _z0) {
            yrot = 10;
            xc = _x0;
            zc = _z0;

            temp_cuboid_side = ((_z0 - vNew.z) < (_x0 - vNew.x)) ? 0x20 : 0x40;
        } else if (vNew.z > _z1) {
            yrot = 9;
            xc = _x0;
            zc = _z1;

            temp_cuboid_side = ((vNew.z - _z1) < (_x0 - vNew.x)) ? 0x10 : 0x40;
        }
    } else if (vNew.x > _x1) {
        if (vNew.z < _z0) {
            yrot = 6;
            xc = _x1;
            zc = _z0;

            temp_cuboid_side = ((_z0 - vNew.z) < (vNew.x - _x1)) ? 0x20 : 0x80;
        } else if (vNew.z > _z1) {
            yrot = 5;
            xc = _x1;
            zc = _z1;

            temp_cuboid_side = ((vNew.z - _z1) < (vNew.x - _x1)) ? 0x10 : 0x80;
        }
    }
    
    if (yrot != 0) {
        dx = vNew.x - xc;
        dz = vNew.z - zc;
        if ((dx * dx) + (dz * dz) > (radius * radius)) {
            return 0;
        } else {
            temp_cuboid_bounce_angle = NuAtan2D(dx, dz) + cub->hdg;
            return 1;
        }
    }
    
    
    if ((vNew.x - x0) < (x1 - vNew.x)) {
        if ((vNew.z - z0) < (z1 - vNew.z)) {
            corner = ((vNew.z - z0) < (vNew.x - x0)) ? 0x8000 : 0xC000;
        } else {
            corner = ((z1 - vNew.z) < (vNew.x - x0)) ? 0 : 0xC000;
        }
    } else {
        if ((vNew.z - z0) < (z1 - vNew.z)) {
            corner = ((vNew.z - z0) < (x1 - vNew.x)) ? 0x8000 : 0x4000;
        } else {
            corner = ((z1 - vNew.z) < (x1 - vNew.x)) ? 0 : 0x4000;
        }
    }
    
    temp_cuboid_bounce_angle = corner + cub->hdg;

    if (corner == 0) {
        temp_cuboid_side = 0x10;
    } else if (corner == 0x4000) {
        temp_cuboid_side = 0x80;
    } else if (corner == 0x8000) {
        temp_cuboid_side = 0x20;
    } else {
        temp_cuboid_side = 0x40;
    }

    return 1;
}

//NGC MATCH
s32 CylinderCylinderOverlapXZ(struct nuvec_s *p0,float r0,struct nuvec_s *p1,float r1) {
  float dz;
  float dx;
  
  dx = p1->x - p0->x;
  dz = p1->z - p0->z;
  if (dx * dx + dz * dz <= r0 * r0 + r1 * r1) {
     return 1;
  }
  return 0;
}

//NGC MATCH
s32 GameObjectOverlap(struct obj_s *p0,struct obj_s *p1,s32 flag) {
  float dx;
  float dz;
  float r;
  struct nuvec_s pos0;
  struct nuvec_s pos1;
  
  if ((((p0->flags & 0x2000) == 0) ||
      (((p0->draw_frame != 0 && (p0->pLOCATOR != NULL)) && ((p0->model == NULL || (p0->model->pLOCATOR[0] != NULL)))))) &&
     (((p1->flags & 0x2000) == 0 || (((p1->draw_frame != 0 && (p1->pLOCATOR != NULL)) &&
       ((p1->model == NULL || (p1->model->pLOCATOR[0] != NULL)))))))) {
    if ((p0->flags & 0x2000) != 0) {
      pos0.x = p0->pLOCATOR->_30;
      pos0.y = p0->pLOCATOR->_31;
      pos0.z = p0->pLOCATOR->_32;
    }
    else {
      pos0 = (p0->pos);
    }
    if ((p1->flags & 0x2000) != 0) {
      pos1.x = p1->pLOCATOR->_30;
      pos1.y = p1->pLOCATOR->_31;
      pos1.z = p1->pLOCATOR->_32;
    }
    else {
      pos1 = (p1->pos);
    }
    if (!(p0->bot * p0->SCALE + pos0.y > p1->top * p1->SCALE + pos1.y) &&
       !(p1->bot * p1->SCALE + pos1.y > p0->top * p0->SCALE + pos0.y)) {
      dz = pos1.z - pos0.z;
      r = p0->RADIUS + p1->RADIUS;
      dx = pos1.x - pos0.x;
      if (dx * dx + dz * dz <= r * r) {
        if (flag != 0) {
          temp_yrot = NuAtan2D(dx,dz);
        }
        return 1;
      }
    }
  }
  return 0;
}

//NGC MATCH
void FlyGameObject(struct obj_s *obj, u16 yrot) {
  (obj->mom).x = 0.0f;
  (obj->mom).y = 0.0f;
  (obj->mom).z = 0.1666667f;
  NuVecRotateX(&obj->mom,&obj->mom,-0x400);
  NuVecRotateY(&obj->mom,&obj->mom,yrot);
  GameSfx(0x37,&obj->pos);
  return;
}

//NGC MATCH
void GetTopBot(struct creature_s *c) {
  if (c == player) {
    if ((VEHICLECONTROL == 1) && (c->obj.vehicle != -1)) {
      c->obj.bot = CData[c->obj.vehicle].min.y;
      c->obj.top = CData[c->obj.vehicle].max.y;
      return;
    }
    if ((c == player) && (VEHICLECONTROL == 2)) {
      c->obj.bot = CData[115].min.y;
      c->obj.top = CData[115].max.y;
      return;
    }
  }
  if ((c->obj.flags & 0x10000) != 0) {
    c->obj.bot = -c->obj.max.y;
    c->obj.top = -c->obj.min.y;
    return;
  }
  if (c->obj.dangle != 0) {
    c->obj.top = c->obj.max.y;
    if (c->obj.dangle == 2) {
      c->obj.bot = (c->obj.min.y + c->obj.max.y) * 0.5f;
      return;
    }
    c->obj.bot = c->obj.min.y;
    return;
  }
  c->obj.bot = c->obj.min.y;
  if ((((c->obj.ground != 0) || (c->obj.old_ground != 0)) &&
      ((c->crawl != 0 || (((c->slide != 0 && (c->obj.character != 1)) || (c->slam_wait != 0)))))) 
      || (((c->obj.ground == 0 && (c->slam != 0)) && (c->slam < 3)))) {
          c->obj.top = (c->obj.min.y + c->obj.max.y) * 0.5f;
          return;
  }
  c->obj.top = c->obj.max.y;
  return;
}

//NGC MATCH
void NewTopBot(struct obj_s *obj) {
  obj->objbot = obj->bot * obj->SCALE + (obj->pos).y;
  obj->objtop = obj->top * obj->SCALE + (obj->pos).y;
  return;
}

//NGC MATCH
void OldTopBot(struct obj_s *obj) {
  obj->oldobjbot = obj->objbot;
  obj->oldobjtop = obj->objtop;
  return;
}

//NGC MATCH
float CreatureTopBelow(struct nuvec_s *pos, u32 obj_flags) {
    struct obj_s *obj;
    struct nuvec_s obj_pos;
    float top;
    float shadow;
    float dz;
    float dx;
    s32 i;
    
    shadow = 2000000.0f;
    for(i = 0; i < GAMEOBJECTCOUNT; i++) {
        obj = pObj[i];
        if (((obj != NULL) && (obj->invisible == 0)) && ((obj->flags & obj_flags) != 0)) {
            if ((obj->flags & 0x2000) != 0) {
                if (obj->draw_frame == 0) {
                    continue;
                }
                if ((obj->pLOCATOR != NULL) && (obj->model->pLOCATOR[0] != NULL)) {
                    obj_pos.x = obj->pLOCATOR->_30;
                    obj_pos.y = obj->pLOCATOR->_31;
                    obj_pos.z = obj->pLOCATOR->_32;
                } else {
                    goto here;
                }
            }
            else
            {
                here:
                obj_pos = obj->pos;
            }
            if ((obj->flags & 0x8000) != 0) {
                if (CylinderCuboidOverlapXZ(pos, 0.0f, obj, &obj_pos) == 0) {
                    continue;
                }
            }
            else {
                dx = pos->x - obj_pos.x;
                dz = pos->z - obj_pos.z;
                dx *= dx;
                dz *= dz;
                if ((dx + dz) > (obj->RADIUS * obj->RADIUS)) {
                    continue;
                }
            }
            
            top = (obj->top * obj->SCALE + obj->pos.y);
            if ((shadow == 2000000.0f) || (top > shadow)) {
                shadow = top;
            }
        }
    }
    return shadow;
}

//NGC MATCH
s32 WumpaCollisions(struct obj_s *obj) {
    struct wumpa_s *wumpa;
    struct winfo_s* info;
    float dx;
    float dz;
    float y;
    float r2;
    s32 i;
    s32 got;
    s32 attack;
    s32 key;
    
    if (TimeTrial == 0) {
        
        if (level_part_2 != 0) {
            return 0;
        }
        
        r2 = GameObjectRadius(obj) + 0.2f;
        r2 *= r2;
        attack = obj->attack & 2;
        wumpa = Wumpa;
    
        for(i = 0; i < 320; i++, wumpa++) {
            if (wumpa->active == 2) {
                info = &WInfo[i & 7];
                y = wumpa->pos.y + info->dy;
                if ((obj->bot * obj->SCALE + obj->pos.y) < (y + 0.1f) && (obj->top * obj->SCALE + obj->pos.y) > (y - 0.1f)) {
                    dx = obj->pos.x - wumpa->pos.x;
                    dz = obj->pos.z - wumpa->pos.z;
                    if ((dx * dx + dz * dz) < r2) {
                        if ((attack != 0)) {
                            FlyWumpa(wumpa);
                            break; 
                        }
                        if (obj->dead == 0) {
                            AddScreenWumpa(wumpa->pos.x, y, wumpa->pos.z, 1);
                            key = -1;
                            AddFiniteShotDebrisEffect(&key, GDeb[130].i, &wumpa->pos, 1);
                            wumpa->active = 0;
                            GameSfx(0x2c, &obj->pos);
                        }
                        break;
                    }
                }
            }
        }
    }
    return 0;
}

//NGC MATCH
void MoveLoopXZ(struct obj_s *obj,u16 *ay) {
  float temp;
  u16 yrot;
  struct nuvec_s mom;
  
  if (ay != NULL) {
    yrot = *ay;
  }
  else {
    yrot = obj->hdg;
  }
  NuVecRotateY(&mom,&obj->mom,-yrot);
  if (mom.x > 0.0f) {
    mom.x = mom.x - in_s_friction;
    if (mom.x < 0.0f) {
      mom.x = 0.0f;
    }    
  }
  else if (mom.x < 0.0f) {
        mom.x += in_s_friction;
        if (mom.x > 0.0f) {
            mom.x = 0.0f;
        }
  }
  temp = -0.075f;
  if ((mom.x < -0.075f) || (temp = 0.075f, 0.075f < mom.x)) {
    mom.x = (temp - mom.x) * 0.25f + mom.x;
  }
  obj->side = mom.x;
  obj->abs_side = NuFabs(mom.x);
  if (mom.z > in_speed) {
    mom.z -= in_f_friction;
    if (mom.z < in_speed) {
        mom.z = in_speed;
    }
  }
  else if (mom.z < in_speed) {
    mom.z += in_f_friction;
        if (mom.z > in_speed) {
            mom.z = in_speed;
        }
  }
  obj->forward = mom.z;
  obj->abs_forward = NuFabs(mom.z);
  NuVecRotateY(&obj->mom,&mom,yrot);
  return;
}

//NGC MATCH
void FindAnglesZX(struct nuvec_s *src) {
  struct nuvec_s v;
  unsigned short a;

  a = NuAtan2D(src->z,src->y);
  temp_xrot = a;
  NuVecRotateX(&v,src,-a);
  a = NuAtan2D(v.x,v.y); 
  temp_zrot = -a; 
}

//NGC MATCH
void GetSurfaceInfo(struct obj_s *obj,s32 ter,float surface_y) {
  s32 surface_type;
  s32 temp;
  s32 old;
  u16 yrot;
  struct nuvec_s v;

  if (surface_y != 2000000.0f) {
    surface_type = ShadowInfo();
    if ((u32)surface_type > 0xf) {
      surface_type = 0;
    }
    obj->reflect_y = ((TerSurface[surface_type].flags & 4) == 0) ? 2000000.0f : surface_y;
  }
  else {
    obj->reflect_y = 2000000.0f;
    surface_type = 0;
  }
  if (ter != 0) {
    old = obj->surface_type;
    obj->surface_type = (char)surface_type;
    if (obj->shadow == 2000000.0f) {
      obj->shadow = SAFEY;
      (obj->vSN) = v010;
    }
    else {
      (obj->vSN) = ShadNorm;
      if ((((obj->flags & 1) != 0) && (obj->ground != 0)) && (1 < ter)) {
        if (obj->surface_type == 6) {
          ShadowDir(&v);
          temp = NuAtan2D(v.x,v.z);
          yrot = temp + 0x4000U & 0xffff;
          plr_conveyor_yrot = (u16)(temp + 0x4000U);
          obj->pos.x = NuTrigTable[yrot] * 0.025f + obj->pos.x;
          obj->pos.z = NuTrigTable[(yrot + 0x4000) & 0x2ffff] * 0.025f + obj->pos.z;
          NuVecSub(&plr_conveyor_mom,&obj->pos,&obj->oldpos);
        }
        else if (old == 6) {
          temp = NuAtan2D(plr_conveyor_mom.x,plr_conveyor_mom.z);
          if (NuFabs(RotDiff(plr_conveyor_yrot,(u16)temp)) < 16384.0f) {
            (obj->mom).x = NuTrigTable[plr_conveyor_yrot] * 0.025f + NuTrigTable[plr_conveyor_yrot] * 0.025f;
            (obj->mom).z = NuTrigTable[(plr_conveyor_yrot + 0x4000) & 0x2ffff] * 0.025f;
            (obj->mom).z += (obj->mom).z;
          }
        }
      }
    }
  }
  obj->layer_shadow = EShadY;
  if ((EShadY != 2000000.0f) && (obj->bot * obj->SCALE + obj->pos.y < EShadY)) {
        obj->layer_type = (char)EShadowInfo();
        if (((u32)obj->layer_type & 0xff) > 10) {
          obj->layer_type = 0;
        }
        if ((TerLayer[obj->layer_type].flags & 0x40) == 0) {
          (obj->vLN) = EShadNorm;
          FindAnglesZX(&obj->vLN);
          obj->layer_xrot = temp_xrot;
          obj->layer_zrot = temp_zrot;
        } else {
          obj->layer_type = -1;
          (obj->vLN) = v010;
          obj->layer_xrot = 0;
          obj->layer_zrot = 0;
        }
  } else {
      obj->layer_type = -1;
      (obj->vLN) = v010;
      obj->layer_xrot = 0;
      obj->layer_zrot = 0;
  }
  obj->roof_y = ShadRoofY;
  if ((ShadRoofY != 2000000.0f) && (obj->bot * obj->SCALE + obj->pos.y < ShadRoofY)) {
    obj->roof_type = ShadowRoofInfo();
    if (0xf < ((u32)obj->roof_type & 0xff)) {
      obj->roof_type = 0;
    }
    (obj->vRN).x = -ShadRoofNorm.x;
    (obj->vRN).y = -ShadRoofNorm.y;
    (obj->vRN).z = -ShadRoofNorm.z;
    FindAnglesZX(&obj->vRN);
    temp_roof_xrot = temp_xrot;
    temp_roof_zrot = temp_zrot;
  }
  else {
    obj->roof_type = -1;
    (obj->vRN) = v010;
    temp_roof_xrot = 0;
    temp_roof_zrot = 0;
  }
  return;
}

//NGC MATCH
void KillItem(struct obj_s *obj) {
  struct creature_s *c;
  
  c = (struct creature_s *)obj->parent;
  obj->dead = 1;
  c->on = 0;
  c->off_wait = 2;
  return;
}

//NGC MATCH
void PickupCrystal(void) {
  plr_crystal.count = 1;
  plr_items |= 1;
  plr_crystal.frame = 0x1e;
  GameSfx(0x26,NULL);
  AddPanelDebris(0.0f,-0.7f,6,0.125f,0x10);
  return;
}

//NGC MATCH
void PickupCrateGem(void) {
  plr_crategem.count = 1;
  plr_items |= 2;
  plr_crategem.frame = 0x1e;
  GameSfx(0x26,NULL);
  AddPanelDebris(-0.2f,-0.7f,6,0.125f,0x10);
  return;
}

//NGC MATCH
void PickupBonusGem(unsigned int item) {
    plr_items |= item;
    plr_bonusgem.item = item;
    plr_bonusgem.count = 1;
    item = plr_items;
    plr_bonusgem.frame = 0x1e;
    
    GameSfx(0x26, NULL);
    AddPanelDebris(0.2f,-0.7f, 6, 0.125f, 0x10);
}

//NGC MATCH
void PickupPower(s32 character) {
    switch (character) {
    case 0xA7:
        new_power = 0;
        break;
    case 0xA5:
        new_power = 1;
        break;
    case 0xA6:
        new_power = 2;
        break;
    case 0xA2:
        new_power = 3;
        break;
    case 0xA4:
        new_power = 4;
        break;
    case 0xA3:
        new_power = 5;
        break;
    }
    Game.powerbits = Game.powerbits | (1 << new_power);
    NewMenu(&Cursor,0x22,-1,-1);
    GameSfx(0x26,NULL);
    if ((Level != 0x15) && (Level != 0x18)) {
        player->slide = 0;
        player->obj.mom.x = 0.0f;
        player->obj.mom.z = 0.0f;
    }
}

//NGC MATCH
void PickupItem(struct obj_s* obj) {
    u32 item;

    if ((new_mode == -1) && (new_level == -1)) {
        switch (obj->character) {
            case 0x75:
                PickupCrystal();
                ClockOff();
                GameSfx(0x26, 0);
                break;
            case 0x76:
                StartTimeTrial(&obj->pos, 0);
                break;
            case 0x77:
                PickupCrateGem();
                ClockOff();
                GameSfx(0x26, 0);
                break;
            case 0x78:
            case 0x79:
            case 0x7A:
            case 0x7B:
            case 0x7C:
            case 0x7D:
                if (obj->character == 0x79) {
                    item = 8;
                } else if (obj->character == 0x7A) {
                    item = 0x20;
                } else if (obj->character == 0x7B) {
                    item = 0x10;
                } else if (obj->character == 0x7C) {
                    item = 0x40;
                } else if (obj->character == 0x7D) {
                    item = 0x80;
                } else {
                    item = 4;
                }
                PickupBonusGem(item);
                GameSfx(0x26, 0);
                break;
            case 0xA2:
            case 0xA3:
            case 0xA4:
            case 0xA5:
            case 0xA6:
            case 0xA7:
                if ((LBIT & 0x03E00000) != 0) {
                    boss_dead = 2;
                }
                PickupPower(obj->character);
                break;
        }
    }

    KillItem(obj);
    return;
}

//NGC MATCH
s32 HitItems(struct obj_s *obj) {
  struct obj_s *cyl;
  s32 i;
  
  if (level_part_2 != 0) {
    return 0;
  }
  for(i = 0; i < 64; i++) {
      cyl = pObj[i];
      if ((((cyl != NULL) && (cyl->dead == 0)) && (cyl->invisible == 0)) &&
         (((cyl->flags & 0x10) != 0 && (GameObjectOverlap(obj,cyl,0) != 0)))) {
        PickupItem(cyl);
        return 1;
      }
  }
  return 0;
}

//NGC MATCH
s32 HitCreatures(struct obj_s *obj, s32 destroy, s32 type) {
    struct obj_s *cyl;
    s32 i; 
    s32 temp;
  
    if (level_part_2 != 0) {
        return 0;
    }
    for (i = 0; i < 64; i++) { 
        cyl = pObj[i];
        
        if ((((cyl != NULL) && (cyl->dead == 0)) && (cyl->invisible == 0)) &&
        ((((!(cyl->flags & 1)) && (temp = cyl->flags, (cyl->flags & 4) != 0)) && (GameObjectOverlap(obj,cyl,1) != 0)))) 
        {
            if ((cyl->vulnerable & 0x100)) {
                if ((cyl->flags & 0x40000)) {
                    cyl->kill_contact = 1;
                }
                else if (destroy == 2) {
                    KillGameObject(cyl, 21);
                }
                else if (destroy != 0) {
                    KillGameObject(cyl, 4);
                }
                else {
                    FlyGameObject(cyl, temp_yrot);
                    KillGameObject(cyl, 1);   
                }
            }
          
            ((struct creature_s*)(cyl->parent))->hit_type = type;
            return 1; 
        }
    }
    return 0;    
}

//NGC MATCH
s32 WipeCreatures(struct RPos_s *rpos) {
  struct obj_s *cyl;
  struct nuvec_s pos;
  s32 i;
  
  if (level_part_2 != 0) {
      return 0;
  }
  for(i = 0; i < 0x40; i++) {
      cyl = pObj[i];
      if ((cyl != NULL) && (cyl->dead == 0) && (cyl->invisible == 0) && (cyl->flags & 1) == 0) {
            if ((cyl->flags & 4) != 0) {
              if ((cyl->flags & 0x2000) != 0) {
                if (cyl->draw_frame != 0) {
                    if (cyl->pLOCATOR != NULL) {
                      if (cyl->model != NULL) {
                        if (cyl->model->pLOCATOR[0] == NULL) continue;
                      }
                      pos.x = cyl->pLOCATOR->_30;
                      pos.y = cyl->pLOCATOR->_31;
                      pos.z = cyl->pLOCATOR->_32;
                      goto LAB_80041e60;
                    }
                  }
                } else {
                   pos = cyl->pos;
    LAB_80041e60:
                   GetALONG(&pos,&cyl->RPos,(s32)(cyl->RPos).iRAIL,(s32)(cyl->RPos).iALONG,1);
                   if (FurtherALONG((s32)rpos->iRAIL,(s32)rpos->iALONG,rpos->fALONG,
                                         (s32)(cyl->RPos).iRAIL,(s32)(cyl->RPos).iALONG,
                                         (cyl->RPos).fALONG) != 0) {
                      FlyGameObject(cyl,NuAtan2D((rpos->pos).x - pos.x,(rpos->pos).z - pos.z));
                      KillGameObject(cyl,1);
                   } 
                }
            }
      }
  }
  return 0;
}

//NGC MATCH
s32 CreatureRayCast(struct nuvec_s *p0,struct nuvec_s *p1) {
    struct nuvec_s v0;
    struct nuvec_s v1;
    struct nuvec_s obj_pos;
    struct nuvec_s min;
    struct nuvec_s max;
    struct obj_s *obj;
    s32 i;
    s32 face;
    float ratio;
    
    ratio = 1.0f;
    for(i = 0; i < GAMEOBJECTCOUNT; i++) {
        obj = pObj[i];
        if ((obj != NULL) && (obj->invisible == 0) && (obj->flags & 0x14) != 0) {
            if ((obj->flags & 0x2000) != 0) {
                if (obj->draw_frame == 0) {
                    continue;
                }
                if ((obj->pLOCATOR != NULL) && (obj->model->pLOCATOR[0] != NULL)) {
                    obj_pos.x = obj->pLOCATOR->_30;
                    obj_pos.y = obj->pLOCATOR->_31;
                    obj_pos.z = obj->pLOCATOR->_32;
                } else {
                    goto here;
                }
            } else {
                here:
                obj_pos = obj->pos;
            }
            if ((obj->flags & 0x8000) != 0) {
                NuVecSub(&v0,p0,&obj_pos);
                NuVecSub(&v1,p1,&obj_pos);
                NuVecRotateY(&v0,&v0,-(u32)obj->hdg);
                NuVecRotateY(&v1,&v1,-(u32)obj->hdg);
                min.x = (obj->min).x * obj->SCALE;
                min.y = (obj->min).y * obj->SCALE;
                min.z = (obj->min).z * obj->SCALE;
                max.x = (obj->max).x * obj->SCALE;
                max.y = (obj->max).y * obj->SCALE;
                max.z = (obj->max).z * obj->SCALE;
                if ((RayIntersectCuboid(&v0,&v1,&min,&max) != 0) && (temp_ratio < ratio)) {
                    face = temp_face;
                    ratio = temp_ratio;
                }
            }
            else {
                NuVecSub(&v0,p0,&obj_pos);
                NuVecSub(&v1,p1,&obj_pos);
                NuVecRotateY(&v0,&v0,-(u32)obj->hdg);
                NuVecRotateY(&v1,&v1,-(u32)obj->hdg);
                min.x = -obj->RADIUS;
                min.y = (obj->min).y * obj->SCALE;
                min.z = -obj->RADIUS;
                max.x = obj->RADIUS;
                max.y = (obj->max).y * obj->SCALE;
                max.z = obj->RADIUS;
                if ((RayIntersectCuboid(&v0,&v1,&min,&max) != 0) && (temp_ratio < ratio)) {
                    face = temp_face;
                    ratio = temp_ratio;
                }
            }
        }
    }
    temp_face = face;
    temp_ratio = ratio;
    if (ratio < 1.0f) {
        return 1;
    }
    return 0;
}

//NGC MATCH
s32 GetDieAnim(struct obj_s *obj,s32 die) {
  if ((VEHICLECONTROL == 1) && (obj->vehicle != -1)) {
    if ((die != 6) || (die = 0xc, obj->vehicle != 0x99)) {
      die = 0xb;
    }
  }
  else if (VEHICLECONTROL == 2) {
    die = 0xe;
  }
  else if (die == -1) {
    if ((Level == 0x17) || (qrand() < 0x8000)) {
      die = 2;
    }
    else {
      die = 3;
    }
  }
  return die;
}

//NGC MATCH
s32 KillPlayer(struct obj_s *player_obj,s32 die) {
  if ((((player_obj->dead != 0) || (player_obj->finished != 0)) ||
      (player_obj->invincible != 0)) || (((vtog_duration < vtog_time) && (vtog_blend != 0)))) {
    return 0;
  }
  if ((player_obj->mask != NULL) && ((player_obj->mask->active != 0 && ((LDATA->flags & 0xe00) == 0)))) {
        if (player_obj->mask->active < 3) {
          LoseMask(player_obj);
        }
        return 0;
  }
  return KillGameObject(player_obj,die);
}

//NGC MATCH
void ScaleFlatShadow(struct nuvec_s *s,float y,float shadow,float scale) {
  float dy;
  
  dy = y - shadow;
  if (dy <= 0.0f) {
    s->x = scale;
  }
  else {
    if (dy >= 2.0f) {
      s->x = scale * 0.5f;
    }
    else {
      s->x = scale - scale * 0.5f * dy * 0.5f;
    }
  }
  s->z = s->x;
  s->y = 0.0f;
  return;
}

//NGC MATCH
void ResetProjectiles(void) {
  s32 i;
  struct projectile_s *p;
  
  p = Projectile;
  for(i = 0; i < 0x10; i++) {
    if (p->active != 0) {
      RemoveGameObject(&p->obj);
      p->active = 0;
    }
    p++;
  }
  return;
}