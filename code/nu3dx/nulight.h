#ifndef NULIGHT_H
#define NULIGHT_H

#include "../types.h"
#include "nu3dxtypes.h"

/*
  800b14dc 0000cc 800b14dc  4 NuLightInit 	Global
  800b15b8 0000b0 800b15b8  4 NuLightCreate 	Global
  800b1668 000160 800b1668  4 NuLightUpdate 	Global
  800b17c8 000028 800b17c8  4 NuLightGetLight 	Global
  800b17f0 000078 800b17f0  4 NuLightSetAmbientLight 	Global
  800b1868 000218 800b1868  4 NuLightSetDirectionalLights 	Global
  800b1a80 000148 800b1a80  4 NuLightStoreCurrentLights 	Global
  800b1bc8 000200 800b1bc8  4 NuLightSetStoredLights 	Global
  800b1dc8 000010 800b1dc8  4 NuLightClearStoredLights 	Global
  800b1dd8 000084 800b1dd8  4 NuLightFog 	Global
  800b1e5c 000020 800b1e5c  4 NuLightAddSpotXSpanFade 	Global
  800b1e7c 000090 800b1e7c  4 NuLightMatInit 	Global
  800b1f0c 00001c 800b1f0c  4 NuLgtRand 	Global
  800b1f28 000144 800b1f28  4 NuLgtArcLaser 	Global
  800b206c 000a28 800b206c  4 NuLgtArcLaserDraw 	Global
  800b2a94 000018 800b2a94  4 NuLgtSetArcMat 	Global
*/

// If the lights are initialised.
u32 initialised;

// Maximum amount of lights allowed.
u32 maxlights;

// Number of lights currently.
u32 numlights;

// NuLightInit

// Close the lights.
void NuLightClose();

// NuLightCreate

// NuLightUpdate

// NuLightGetLight

// NuLightSetAmbientLight

// NuLightSetDirectionalLights

// NuLightStoreCurrentLights

// NuLightSetStoredLights

// NuLightClearStoredLights

// NuLightFog

// NuLightAddSpotXSpanFade

// NuLightMatInit

// NuLgtRand

// NuLgtArcLaser

// NuLgtArcLaserDraw

// NuLgtSetArcMat

#endif // !NULIGHT_H