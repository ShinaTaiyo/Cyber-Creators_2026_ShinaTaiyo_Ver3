//=============================================================================
//
// TEh [sound.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// TEhź
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM000 = 0,		// BGM0
	SOUND_LABEL_BGM001,			// BGM1
	SOUND_LABEL_BGM002,			// BGM2
	SOUND_LABEL_BGM003,			// BGM3
	SOUND_LABEL_SE_SHOT,		// e­Ė¹
	SOUND_LABEL_SE_HIT,			// qbg¹
	SOUND_LABEL_SE_DEFEAT_ENEMY,// ­¹
	SOUND_LABEL_SE_EXPLOSION_ENEMY_000,// G­¹
	SOUND_LABEL_SE_SHOT_ENEMY_000,//GĢe­Ė¹iPeøĀj
	SOUND_LABEL_SE_SHOT_ENEMY_001,//GĢe­Ė¹iąyōj
	SOUND_LABEL_SE_SHOT_ENEMY_002,//GĢe­Ė¹ihbJj
	SOUND_LABEL_SE_SHOT_ENEMY_003,//GĢe­Ė¹io[Ij
	SOUND_LABEL_SE_SHOT_ENEMY_004,//GĢe­Ė¹isbIj
	SOUND_LABEL_SE_SHOT_ENEMY_005,//GĢe­Ė¹isVIj
	SOUND_LABEL_SE_SHOT_ENEMY_006,//GĢe­Ė¹isbIj
	SOUND_LABEL_SE_SHOT_ENEMY_007,//GĢe­Ė¹iVbgKj
	SOUND_LABEL_SE_SHOT_ENEMY_008,//GĢe­Ė¹iĢ¹j
	SOUND_LABEL_SE_DECISION_000,//č{^ųŹ¹
	SOUND_LABEL_SE_SELECT_000,//Iš{^ųŹ¹
	SOUND_LABEL_SE_SLASH_000,//aųŹ¹
	SOUND_LABEL_SE_HEAL_000,//ńųŹ¹
	SOUND_LABEL_SE_ARROWSHOT_000,//|­Ė¹
	SOUND_LABEL_SE_WARNING_000,//x¹
	SOUND_LABEL_SE_WARNING_001,//ex¹
	SOUND_LABEL_SE_DAMAGE_000,//vC[ķe¹
	SOUND_LABEL_SE_SHIELD_000,//V[hWJ¹
	SOUND_LABEL_SE_APPLAUSE_000,//XRANCĢč¹
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// vg^Cvé¾
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
