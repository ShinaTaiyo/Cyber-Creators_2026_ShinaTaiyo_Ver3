//=============================================================================
//
// サウンド処理 [sound.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// サウンド一覧
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM000 = 0,		// BGM0
	SOUND_LABEL_BGM001,			// BGM1
	SOUND_LABEL_BGM002,			// BGM2
	SOUND_LABEL_BGM003,			// BGM3
	SOUND_LABEL_SE_SHOT,		// 弾発射音
	SOUND_LABEL_SE_HIT,			// ヒット音
	SOUND_LABEL_SE_DEFEAT_ENEMY,// 爆発音
	SOUND_LABEL_SE_EXPLOSION_ENEMY_000,// 敵爆発音
	SOUND_LABEL_SE_SHOT_ENEMY_000,//敵の弾発射音（１弾ずつ）
	SOUND_LABEL_SE_SHOT_ENEMY_001,//敵の弾発射音（炸裂）
	SOUND_LABEL_SE_SHOT_ENEMY_002,//敵の弾発射音（ドッカン）
	SOUND_LABEL_SE_SHOT_ENEMY_003,//敵の弾発射音（バーン！）
	SOUND_LABEL_SE_SHOT_ENEMY_004,//敵の弾発射音（ピュッ！）
	SOUND_LABEL_SE_SHOT_ENEMY_005,//敵の弾発射音（ピシャン！）
	SOUND_LABEL_SE_SHOT_ENEMY_006,//敵の弾発射音（ピッ！）
	SOUND_LABEL_SE_SHOT_ENEMY_007,//敵の弾発射音（ショットガン）
	SOUND_LABEL_SE_SHOT_ENEMY_008,//敵の弾発射音（風の音）
	SOUND_LABEL_SE_DECISION_000,//決定ボタン効果音
	SOUND_LABEL_SE_SELECT_000,//選択ボタン効果音
	SOUND_LABEL_SE_SLASH_000,//斬撃効果音
	SOUND_LABEL_SE_HEAL_000,//回復効果音
	SOUND_LABEL_SE_ARROWSHOT_000,//弓発射音
	SOUND_LABEL_SE_WARNING_000,//警告音
	SOUND_LABEL_SE_WARNING_001,//弾幕警告音
	SOUND_LABEL_SE_DAMAGE_000,//プレイヤー被弾音
	SOUND_LABEL_SE_SHIELD_000,//シールド展開音
	SOUND_LABEL_SE_APPLAUSE_000,//スコアランクイン時の拍手音
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
