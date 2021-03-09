#pragma once

/*
  800cb6d8 0000a4 800cb6d8  4 GS_TexInit 	Global
  800cb77c 0000c0 800cb77c  4 GS_TexReInit 	Global
  800cb83c 000114 800cb83c  4 GS_TexSwizzleRGB5A3 	Global
  800cb950 0003b4 800cb950  4 GS_TexCreateNU 	Global
  800cbd04 0000a4 800cbd04  4 GS_ChangeTextureStates 	Global
  800cbda8 000038 800cbda8  4 GS_TexSetWrapModes 	Global
  800cbde0 000038 800cbde0  4 GS_TexSetWrapModet 	Global
  800cbe18 00025c 800cbe18  4 GS_TexSelect 	Global
  800cc074 000058 800cc074  4 GS_SetTevBlend 	Global
  800cc0cc 000024 800cc0cc  4 GS_SetTevModulate 	Global
  800cc0f0 000004 800cc0f0  4 GS_SetTextureStageState 	Global
*/

// Re-initialize textures.
void GS_TexReInit(); // TODO!!!