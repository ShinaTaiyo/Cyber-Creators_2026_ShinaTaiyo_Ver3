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
	SOUND_LABEL_BGM004,         //BGM4 mement
	SOUND_LABEL_BGM005,         //BGM5 yaiba
	SOUND_LABEL_BGM006,			// BGM6_stage1
	SOUND_LABEL_BGM007,			// BGM7_stage2
	SOUND_LABEL_BGM008,			// BGM8_stage3
	SOUND_LABEL_BGM009,         //BGM9_stage4
	SOUND_LABEL_BGM010,         //BGM10_stage5
	SOUND_LABEL_BGM011,         //BGM11_home
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
	SOUND_LABEL_SE_PUNCH,//スコアランクイン時の拍手音
	SOUND_LABEL_SE_MINISLASH,//小さい斬撃
	SOUND_LABEL_SE_ICE_EXPLOSION,//氷爆発
	SOUND_LABEL_SE_STAB,//串刺し攻撃
	SOUND_LABEL_SE_THUNDER,//雷攻撃
	SOUND_LABEL_SE_CLAW,//爪攻撃
	SOUND_LABEL_SE_FLASHBLADE,//真空刃攻撃
	SOUND_LABEL_SE_FALLINGBLOCK,//落石攻撃
	SOUND_LABEL_SE_BEAM,//ビーム攻撃
	SOUND_LABEL_SE_ICICLECLAW,//つらら攻撃
	SOUND_LABEL_SE_THUNDER2,//雷攻撃２
	SOUND_LABEL_SE_SHAKINGSWORD,//剣振りかぶり
	SOUND_LABEL_SE_FIRE,//炎攻撃
	SOUND_LABEL_SE_ABSOLUTEBLIZARD,//アブソリュートブリザード
	SOUND_LABEL_SE_FINALBALL,//ファイナルボール
	SOUND_LABEL_SE_JUMPINGBULLET,//ジャンプ弾
	SOUND_LABEL_SE_SPLETBULLET,//スプレッド弾
	SOUND_LABEL_SE_JUMP,//ジャンプ
	SOUND_LABEL_SE_ORBGET,//オーブを獲得
	SOUND_LABEL_SE_SPECIALATTACK,//必殺技



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
