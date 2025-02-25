//========================================================
//
//１０月１５日：マップを呼び出す処理[game.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#include <stdio.h>
#include <string.h>
#include "game.h"
#include "bg.h"
#include "block.h"
#include "fade.h"
#include "pause.h"
#include "sound.h"
#include "input.h"
#include "player.h"
#include "Item.h"
#include "character.h"
#include "health.h"
#include "Ability.h"
#include "camera.h"
#include "Attack.h"
#include "enemy.h"
#include "damage.h"
#include "explosion.h"
#include "map.h"
#include "warp.h"
#include "bullet.h"
#include "effect.h"
#include "Treasure.h"
#include "Gage.h"
#include "staging.h"
#include "timer.h"
#include "score.h"
#include "main.h"
#include "particle.h"
#include "special.h"
//グローバル変数
GAMESTATE g_GameState = GAMESTATE_NONE;//ゲームの状態

int g_nCounterGameState = 0;//状態管理カウンター

int g_nCountTime;//ゲーム開始時から経った時間を管理する変数

bool g_bPause = false;//ポーズ中かどうか

int g_nChoosePauseCommand;//ポーズ中の時に選択するメニューを選択する変数

int g_nCountPauseMenu;//ポーズ画面を開いている時間をカウントする変数

PAUSETYPE g_aStopUpdate;//アップデートを止めた上で、特定の処理をする

bool g_bClearPause;//ゲームクリア時に特定のアップデートを止める

bool g_bTimeOver;//時間切れかどうかを判定する

bool g_bUseTimer;//タイマーの時間を減らすかどうかのフラグ
//int g_nMapBlockData[MAX_MAPHEIGHT][MAX_MAPWIDTH];//マップのブロックのデータを保存する

typedef struct
{
	float x;
	float y;
	BLOCKTYPE type;
}SetBlockInfo;//セットするブロックの情報を作る構造体

SetBlockInfo g_aSetBlockInfo[MAX_BLOCK] =
{
	{100,200,BLOCKTYPE_NORMAL},
	{200,300,BLOCKTYPE_NORMAL},
};

//===================================
//ゲーム画面の初期化処理
//===================================
void InitGame(void)
{
	g_nChoosePauseCommand = 0;

	g_bTimeOver = false;

	g_bUseTimer = true;

	g_nCountPauseMenu = 0;

	g_nCountTime = 0;//時間の初期化

	g_bPause = false;//ポーズ解除状態

	Player* pPlayer = GetPlayer();

	int nCnt = 0;
	int nCntWidth;//横のデータを読み取る
	int nCntHeight;//縦列のデータを読み取る

	
	InitBG();
	InitBlock();
	InitEffect();
	InitParticle();
	InitBullet();
	InitExplosion();
	InitItem();
	InitEnemy();
	InitAttack();
	InitPlayer();
	InitDamage();
	InitStaging();
	InitSpecial();
	InitHealth();
	InitScore();
	InitAbility();
	InitTreasure();
	InitWarp();
	InitGage();
	InitTimer();
	InitMap();
	InitFade();
	InitCamera();
	InitPause();

	g_GameState = GAMESTATE_NORMAL;//通常状態に設定

	g_nCounterGameState = 0;
	//キーボードの初期化処理
	//InitKeyboard;

	SetMap(MAPTYPE_8);
	DamageHealth(0);
	/*SetItem(ITEMTYPE_HEAL, D3DXVECTOR3(180.0f, 680.0f, 0.0f),1000);*/
	SetTreasure(TREASURETYPE_BOX, -1, D3DXVECTOR3(1200.0f, 30.0f, 0.0f));
	SetPlayer(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
	//SetItem(ITEMTYPE_REAFORB, D3DXVECTOR3(200.0f, 220.0f, 0.0f));
}


//==================================
//ゲーム画面の終了処理
//==================================
void UninitGame(void)
{
	UninitBG();
	UninitBlock();
	UninitEffect();
	UninitParticle();
	UninitBullet();
	UninitExplosion();
	UninitItem();
	UninitEnemy();
	UninitAttack();
	UninitPlayer();
	UninitDamage();
	UninitStaging();
	UninitSpecial();
	UninitHealth();
	UninitScore();
	UninitAbility();
	UninitTreasure();
	UninitWarp();
	UninitGage();
	UninitTimer();
	UninitPause();
	UninitFade();
	UninitCamera();
}

//=================================
//ゲーム画面の更新処理
//=================================
void UpdateGame(void)
{
	Player* pPlayer = GetPlayer();
	
	g_nCountTime++;

	CharacterNum* pCharacterNum = GetCharacterNum();

	/*Result* pResult;*/
	int nData;

	int nCnt;

	int nRandPos= 0;
	int nRandType = 0;
	int nRandItemPos = 0;

	int nRandItemSet = 0;

	if (g_aStopUpdate == PAUSETYPE_UNLOCK)
	{//ポーズ状態を解除
		g_bPause = false;
	}

	if (GetKeyboardTrigger(DIK_P) == true||GetJoyPadTrigger(JOYKEY_BACK)==true)
	{//ポーズキーが押された
		g_bPause = g_bPause ? false : true;
		g_aStopUpdate = PAUSETYPE_PAUSE;
	}
		if (g_bPause == false)
		{
			if (g_bUseTimer == true)
			{
				if (g_nCountTime % 60 == 0)
				{
					SubTimer(1);//タイマーの時間を減らす
				}
			}
			
			UpDateBG();
			UpdateBlock();
			UpdateEffect();
			UpdateParticle();
			UpdateBullet();
			UpdateExplosion();
			UpdateItem();
			UpdateEnemy();
			UpdateAttack();
			UpDatePlayer();
			UpdateDamage();
			UpdateStaging();
			UpdateSpecial();
			UpdateHealth();
			UpdateScore();
			UpdateAbility();
			UpdateTreasure();
			UpdateWarp();
			UpdateGage();
			UpdateTimer();
			UpdateFade();
			UpdateCamera();
			
			

		}
		if (g_bPause == true)
		{//ポーズ中
		//ポーズの更新処理

			SetEnablePause(g_nChoosePauseCommand);

			UpdatePause();

			if (g_aStopUpdate == PAUSETYPE_PAUSE)
			{
				if (GetKeyboardTrigger(DIK_S) == true || GetJoyPadTrigger(JOYKEY_DOWN) == true)
				{
					g_nChoosePauseCommand++;
				}
				else if (GetKeyboardTrigger(DIK_W) == true || GetJoyPadTrigger(JOYKEY_UP) == true)
				{
					g_nChoosePauseCommand--;
				}

				if (g_nChoosePauseCommand == 4)
				{
					g_nChoosePauseCommand = 1;
				}
				if (g_nChoosePauseCommand == 0)
				{
					g_nChoosePauseCommand = 3;
				}

				if (g_nChoosePauseCommand == 1)
				{
					if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoyPadTrigger(JOYKEY_START) == true)
					{
						g_bPause = false;
					}
				}
				else if (g_nChoosePauseCommand == 2)
				{
					if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoyPadTrigger(JOYKEY_START) == true)
					{
						SetMode(MODE_GAME);
					}
				}
				else if (g_nChoosePauseCommand == 3)
				{
					if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoyPadTrigger(JOYKEY_START) == true)
					{
						SetMode(MODE_TITLE);
					}
				}

				
			}
			else if (g_aStopUpdate == PAUSETYPE_STAGING)
			{//演出だけを行う
				UpdateSpecial();
			}
		}
	
		g_bTimeOver = CheckTime();

	//========================
	//強制終了ボタン
	//========================
	if (GetKeyboardTrigger(DIK_E) == true)
	{
		SetGameState(GAMESTATE_END);
	}
	if (g_bTimeOver == true)
	{
		SetGameState(GAMESTATE_END);
	}

	switch (g_GameState)
	{
	case GAMESTATE_NORMAL://通常状態
		break;
	case GAMESTATE_END:
		g_nCounterGameState++;
	
		if (g_nCounterGameState >= 150)
		{
			StopSound(SOUND_LABEL_BGM004);
			SetFade(MODE_RESULT);
			StopSound(SOUND_LABEL_BGM000);
			StopSound(SOUND_LABEL_BGM001);
			StopSound(SOUND_LABEL_BGM002);
			StopSound(SOUND_LABEL_BGM003);
			StopSound(SOUND_LABEL_BGM004);
			StopSound(SOUND_LABEL_BGM005);
			StopSound(SOUND_LABEL_BGM006);
			StopSound(SOUND_LABEL_BGM007);
			StopSound(SOUND_LABEL_BGM008);
			StopSound(SOUND_LABEL_BGM009);
			StopSound(SOUND_LABEL_BGM010);
			StopSound(SOUND_LABEL_BGM011);

		}
		break;
	}
	
}

//======================================
//ゲーム画面の描画処理
//======================================
void DrawGame(void)
{
	
	DrawBG();
	DrawBlock();
	DrawEffect();
	DrawParticle();
	DrawBullet();
	DrawExplosion();
	DrawItem();
	DrawEnemy();
	DrawAttack();
	DrawPlayer();
	DrawDamage();
	DrawStaging();
	DrawHealth();
	DrawScore();
	DrawAbility();
	DrawTreasure();
	DrawWarp();
	DrawGage();
	DrawTimer();
	DrawFade();
	DrawCamera();
	if (g_bPause == true)
	{//ポーズ中
		//ポーズの描画処理
	
			if (g_aStopUpdate == PAUSETYPE_PAUSE)
			{
				DrawPause();
			}
			else if (g_aStopUpdate == PAUSETYPE_STAGING)
			{//演出だけを行う
				DrawSpecial();
			}
	}
}

//============================================
//ゲームの状態の設定
//============================================
void SetGameState(GAMESTATE state)
{
	g_GameState = state;
	
}

//============================================
//ポーズ設定処理
void SetPause(PAUSETYPE type)
{
	g_aStopUpdate = type;
	g_bPause = true;
}

//==============================================
//ゲームの状態の取得
//==============================================
GAMESTATE GetGameState(void)
{
	return g_GameState;
}

//==================================
//タイマーの時間を減らすかどうか
//==================================
bool* GetUseTimer(void)
{
	return &g_bUseTimer;
}

