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
	SOUND_LABEL_BGM000 = 0,		                //BGM0
	SOUND_LABEL_BGM001,			                //BGM1
	SOUND_LABEL_BGM002,			                //BGM2
	SOUND_LABEL_BGM003,			                //BGM3
	SOUND_LABEL_BGM004,                         //BGM4 mement
	SOUND_LABEL_BGM005,                         //BGM5 yaiba
	SOUND_LABEL_BGM006,			                //BGM6_stage1
	SOUND_LABEL_BGM007,			                //BGM7_stage2
	SOUND_LABEL_BGM008,			                //BGM8_stage3
	SOUND_LABEL_BGM009,                         //BGM9_stage4
	SOUND_LABEL_BGM010,                         //BGM10_stage5
	SOUND_LABEL_BGM011,                         //BGM11_home
	SOUND_LABEL_BGM012,                         //BGM12_え、やば！？
	SOUND_LABEL_BGM013,                         //BGM13_game
	SOUND_LABEL_BGM014,                         //BGM14_result
	SOUND_LABEL_BGM015,                         //BGM15_BossBgm0
	SOUND_LABEL_SE_TIBIROBOWALK,                //SE00_ちびロボが歩く
	SOUND_LABEL_SE_SWORDSWING_000,              //SE01_ちびロボが剣を振る
	SOUND_LABEL_SE_SWORDSLASH_000,              //SE02_ちびロボが剣で斬る
	SOUND_LABEL_SE_SPEAKERWAVE_000,             //SE03_ちびロボがスピーカー攻撃をする
	SOUND_LABEL_SE_BOOMERANGTHROW_000,          //SE04_ちびロボがブーメランを投げる
	SOUND_LABEL_SE_HITBOOMERANG_000  ,          //SE05_ブーメランがヒットする
	SOUND_LABEL_SE_SPEAKERWAVEHIT_000,          //SE06_スピーカー攻撃がヒットする
	SOUND_LABEL_SE_SELECT_000,                  //SE07_選ぶ
	SOUND_LABEL_SE_BATTERYCAUTION_000,          //SE08_バッテリー残量が危ない
	SOUND_LABEL_SE_STATUSBUFF_000,              //SE09_ステータスアップ
	SOUND_LABEL_SE_CLAWSLASH_000,               //SE10_爪で斬る
	SOUND_LABEL_SE_BOSSAPPEAR_000,              //SE11_ボス登場
	SOUND_LABEL_SE_DEFEATENEMY_000,             //SE12_敵を倒した
	SOUND_LABEL_SE_DECIDE_000,                  //SE13_決定した
	SOUND_LABEL_SE_BATTERYCHARGE_000,           //SE14_バッテリーをチャージする
	SOUND_LABEL_SE_SCRAPCOLLECT_000,            //SE15_スクラップを獲得する
	SOUND_LABEL_SE_DODGE_000,                   //SE16_回避
	SOUND_LABEL_SE_CRITICAL_000,                //SE17_クリティカル
	SOUND_LABEL_SE_CHARGINGCOMPLETION_000,      //SE18_充電完了
	SOUND_LABEL_SE_GETCANDY_000,                //SE19_飴獲得
	SOUND_LABEL_SE_RASER_000,                   //SE20_レーザー
	SOUND_LABEL_SE_POWERCHARGE_000,             //SE21_力をためる
	SOUND_LABEL_SE_FASTMOVE_000,                //SE22_高速移動
	SOUND_LABEL_SE_FIRE_000,                    //SE23_炎
	SOUND_LABEL_SE_WIND_000,                    //SE24_風
	SOUND_LABEL_SE_DAMAGE_000,					//SE25_被弾
	SOUND_LABEL_SE_FLUSHPUNCH_000,              //SE26_フラッシュパンチ
	SOUND_LABEL_SE_BIRDSTRIKE_000,				//SE27_バードストライク
	SOUND_LABEL_SE_THUNDER_000,                 //SE28_サンダー
	SOUND_LABEL_SE_FALLINGMETEO_000,            //SE29_メテオが降る
	SOUND_LABEL_SE_BURSTMETEO_000,              //SE30_メテオがバーストする
	SOUND_LABEL_SE_SHOCKWAVE_000,               //SE31_衝撃波





	SOUND_LABEL_MAX
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
