//========================================================
//
//１２月２６日：チーム制作の基盤作成[game.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"
#include "enemy.h"

//ゲームの状態
typedef enum
{
	GAMESTATE_NONE=0,//何もしていない状態
	GAMESTATE_NORMAL,//通常状態（ゲーム進行中）
	GAMESTATE_END,//終了状態（敵全滅、プレイヤー消滅、タイムオーバーなど）
	GAMESTATE_MAX
}GAMESTATE;

//プロトタイプ宣言
void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);
void SetGameState(GAMESTATE state);
void ChangeStage(void);//ステージ切り替え処理
GAMESTATE GetGameState(void);//情報をゲットする。
bool GetEditMode(void);//エディタ中かどうかの判定を取得
bool GetEditEnemyMode(void);//敵エディタ中かどうかの判定を取得
bool GetHouse(void);//ちびロボハウス内かどうかの判定を取得
bool GetPause(void);//ポーズ中かどうかの判定を取得
#endif
