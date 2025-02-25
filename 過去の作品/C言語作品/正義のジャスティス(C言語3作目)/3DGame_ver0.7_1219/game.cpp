//========================================================
//
//８月３０日：エフェクトもう一回見直す[game.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#include "main.h"
#include "input.h"
#include "bg.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "tutorial.h"
#include "sound.h"
#include "fade.h"
#include <time.h>
#include "ranking.h"
#include "polygon.h"
#include "camera.h"
#include "light.h"
#include "field.h"
#include "wall.h"
#include "model.h"
#include "player.h"
#include "shadow.h"
#include <stdio.h>
#include "billboard.h"
#include "bullet.h"
#include "effect.h"
#include "emitter.h"
#include "meshfield.h"
#include "particle.h"
#include "meshcylinder.h"
#include "Block.h"
#include "edit.h"
#include "motion.h"
#include "enemy.h"
#include "editEnemy.h"
#include "AttackModel.h"
#include "timer.h"
#include "score.h"
#include "damage.h"
#include "SkillGage.h"
#include "SkillName.h"
#include "AttackBillBoard.h"
#include "health.h"
#include "combo.h"
#include "Summon.h"
#include "pause.h"
#include "item.h"
//グローバル変数
GAMESTATE g_GameState = GAMESTATE_NONE;//ゲームの状態

int g_nCounterGameState = 0;//状態管理カウンター

int g_nCountTime;//ゲーム開始時から経った時間を管理する変数

bool g_bPause = false;//ポーズ中かどうか

int g_nChoosePauseCommand;//ポーズ中の時に選択するメニューを選択する変数

int g_nCountPauseMenu;//ポーズ画面を開いている時間をカウントする変数

//===================================
//ゲーム画面の初期化処理
//===================================
void InitGame(void)
{
	g_nChoosePauseCommand = 0;

	g_nCountPauseMenu = 0;

	g_nCountTime = 0;//時間の初期化

	g_bPause = false;//ポーズ解除状態

	int nCnt;
	/*InitBG();
	InitFade();*/

	InitPause();
	//敵召喚用
	InitSummon();

	InitShadow();
	InitField();
	//InitMeshField();
	//InitMeshCylinder();
	InitDamage();
	InitWall();
	InitModel();
	//InitBlock();
	InitEnemy();
	InitItem();
	InitPlayer();
	//InitBillboard();
	InitBullet();
	InitAttackModel();
	InitEffect();
	InitAttackBillBoard();
	InitParticle();
	InitEmitter();
	InitEdit();
	InitEditEnemy();
	InitMotion();
	InitCamera();
	InitLight();

	//UI
	InitTimer();
	InitScore();
	InitSkillGage();
	InitSkillName();
	InitHealth();
	InitCombo();
	InitFade();

	g_GameState = GAMESTATE_NORMAL;//通常状態に設定

	g_nCounterGameState = 0;
	//キーボードの初期化処理
	//InitKeyboard;

	PlaySound(SOUND_LABEL_BGM004);
}

//==================================
//ゲーム画面の終了処理
//==================================
void UninitGame(void)
{
	//UninitBG();
	//UninitFade();

	UninitPause();
	UninitShadow();
	UninitField();
	//UninitMeshField();
	//UninitMeshCylinder();
	UninitWall();
	UninitDamage();
	UninitModel();
	//UninitBlock();
	UninitEnemy();
	UninitItem();
	UninitPlayer();
	//UninitBillboard();
	UninitBullet();
	UninitAttackModel();
	UninitEffect();
	UninitAttackBillBoard();
	UninitParticle();
	UninitEmitter();
	UninitEdit();
	UninitEditEnemy();
	//UninitMotion();
	UninitCamera();
	UninitLight();

	//UI
	UninitTimer();
	UninitScore();
	UninitSkillGage();
	UninitSkillName();
	UninitHealth();
	UninitCombo();
	UninitFade();

}

//=================================
//ゲーム画面の更新処理
//=================================
void UpdateGame(void)
{

	

	/*Result* pResult;*/
	int nData;

	int nCnt;

	bool bTimeUpFlag = false;


	if (GetKeyboardTrigger(DIK_P) == true||GetJoyPadTrigger(JOYKEY_BACK)==true)
	{//ポーズキーが押された
		g_bPause = g_bPause ? false : true;
	}

	if (g_bPause == false)
	{
		g_nCountPauseMenu = 0;

		g_nCountTime++;//ゲーム開始時からの時間を計測する。

		

	/*	UpDateBG();
		UpdateFade();*/

		//敵召喚
		UpdateSummon();

		//ゲーム
		UpdateField();
		//UpdateMeshField();
		//UpdateMeshCylinder();
		UpdateWall();
		UpdateDamage();
		UpdateModel();
		//UpdateBlock();
		UpdateEnemy();
		UpdateItem();
		UpdatePlayer();
		UpdateShadow();
		//UpdateBillboard();
		UpdateBullet();
		UpdateAttackModel();
		UpdateEffect();
		UpdateAttackBillBoard();
		UpdateParticle();
		UpdateEmitter();
		UpdateEdit();
		UpdateEditEnemy();
		UpdateMotion();
		UpdateCamera();
		UpdateLight();

		//UI
		UpdateTimer();
		UpdateScore();
		UpdateSkillGage();
		UpdateSkillName();
		UpdateHealth();
		UpdateCombo();
		UpdateFade();

		
	
	}

	if (g_bPause == true)
	{//ポーズ中
	//ポーズの更新処理
		UpdatePause();
		g_nCountPauseMenu++;
		if (g_nCountPauseMenu == 1)
		{//ポーズ画面の選択位置の初期位置を設定。
			g_nChoosePauseCommand = 0;
		}
		if (GetKeyboardTrigger(DIK_S) == true || GetJoyPadTrigger(JOYKEY_DOWN) == true)
		{
			StopSound(SOUND_LABEL_SE_SELECT_000);
			g_nChoosePauseCommand += 1;//下に選択を移動する。
		}
		if (GetKeyboardTrigger(DIK_W) == true || GetJoyPadTrigger(JOYKEY_UP) == true)
		{
			StopSound(SOUND_LABEL_SE_SELECT_000);
			g_nChoosePauseCommand -= 1;//上に選択を移動する
		}

		if (g_nChoosePauseCommand == -1)
		{//選択場所を下側へ移動
			g_nChoosePauseCommand = 2;
		}
		if (g_nChoosePauseCommand == 3)
		{//選択場所を上側へ移動
			g_nChoosePauseCommand = 0;
		}

		if (g_nChoosePauseCommand == 0)
		{//コンティニューコマンドを選択
			SetEnablePause(PAUSE_MENU_CONTINUE);

			if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoyPadTrigger(JOYKEY_START) == true)
			{
				g_bPause = false;
			/*	StopSound(SOUND_LABEL_BGM004);*/
			}
		}
		if (g_nChoosePauseCommand == 1)
		{//リトライコマンドを選択
			SetEnablePause(PAUSE_MENU_RETRY);
			if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoyPadTrigger(JOYKEY_START) == true)
			{
				SetMode(MODE_GAME);
				StopSound(SOUND_LABEL_BGM004);
			}
		}
		if (g_nChoosePauseCommand == 2)
		{//タイトル画面に戻るコマンドを選択
			SetEnablePause(PAUSE_MENU_QUIT);
			if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoyPadTrigger(JOYKEY_START) == true)
			{
				SetMode(MODE_TITLE);
				StopSound(SOUND_LABEL_BGM004);
			}
		}
	}

	bTimeUpFlag = CheckTime();

	if (bTimeUpFlag == true)
	{//タイマーが０になったら、ゲームを終了させる。
		g_GameState = GAMESTATE_END;
	}
	//その他
	bool bDeathFlag = bPlayerDeath();
	if (bDeathFlag == true)
	{
		g_GameState = GAMESTATE_END;
	}

	if (GetKeyboardTrigger(DIK_0) == true /*|| GetJoyPadTrigger(JOYKEY_START) == true*/)
	{//ゲーム強制終了ボタン
		g_GameState = GAMESTATE_END;
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
		}
		break;
	}
	
}

//======================================
//ゲーム画面の描画処理
//======================================
void DrawGame(void)
{
	//DrawBG();
	if (g_bPause == true)
	{//ポーズ中
		//ポーズの描画処理
		DrawPause();
	}

	DrawField();
	//DrawMeshField();
	//DrawMeshCylinder();
	DrawWall();
	DrawDamage();
	DrawModel();
	//DrawBlock();
	DrawEnemy();
	DrawItem();
	DrawPlayer();
	DrawShadow();
	DrawEmitter();
	//DrawBillboard();
	DrawBullet();
	DrawAttackModel();
	DrawEffect();
	DrawAttackBillBoard();
	DrawParticle();
	DrawEdit();
	DrawEditEnemy();
	SetCamera();

    //UI
	DrawTimer();
	DrawScore();
	DrawSkillGage();
	DrawSkillName();
	DrawHealth();
	DrawCombo();
	DrawFade();

}

//============================================
//ゲームの状態の設定
//============================================
void SetGameState(GAMESTATE state)
{
	g_GameState = state;
	
}

//==============================================
//ゲームの状態の取得
//==============================================
GAMESTATE GetGameState(void)
{
	return g_GameState;
}
