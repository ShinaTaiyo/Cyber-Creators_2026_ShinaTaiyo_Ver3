//========================================================
//
//８月３０日：エフェクトもう一回見直す[game.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#include "game.h"
#include "bg.h"
#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "explosion.h"
#include "score.h"
#include "input.h"
#include "main.h"
#include "health.h"
#include "Gage.h"
#include "SkillName.h"
#include <stdlib.h>
#include <time.h>
#include "result.h"
#include "sound.h"
#include "effect.h"
#include "fade.h"
#include "staging.h"
#include "pause.h"
#include "particle.h"
#include "SkillGage.h"
#include "ranking.h"
#include "damage.h"
#include "wave.h"
#include "Difficulty.h"
#include "scorebonus.h"
#define MAX_WAVE (14) //ウェーブ数

//グローバル変数
GAMESTATE g_GameState = GAMESTATE_NONE;//ゲームの状態
int g_nCounterGameState = 0;//状態管理カウンター
int g_nCountTime;//ゲーム開始時から経った時間を管理する変数

bool g_bWaveFlag[MAX_WAVE];//それぞれのウェーブ開始のフラグを宣言する。

float g_fRandumEnemyPosX[MAX_ENEMY*2];
float g_fRandumEnemyPosY[MAX_ENEMY*2];

bool g_bPause = false;//ポーズ中かどうか

int g_nChoosePauseCommand;//ポーズ中の時に選択するメニューを選択する変数

int g_nCountPauseMenu;//ポーズ画面を開いている時間をカウントする変数

int g_nScoreBonustime[MAX_WAVE+1];
bool g_bScoreBonusTimeCountStart[MAX_WAVE+1];//それぞれのウェーブが始まったときに、ウェーブクリア時間スコアボーナスのカウントをスタートする。
//bool g_bResultFlag_gameover = false;
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
	InitBG();
	InitEffect();
	InitPlayer();
	InitHealth();
	InitGage();
	InitSkillName();
	InitSkillGage();
	InitDamage();
	InitEnemy();
	InitExplosion();
	InitBullet();
	InitWave();
	InitScore();
	InitScoreBonus();
	InitStaging();
	InitPause();
	InitParticle();
	InitFade();//フェードアウト後に移行するモードを設定

	PlaySound(SOUND_LABEL_BGM001);

	for (nCnt = 0; nCnt < MAX_WAVE; nCnt++)
	{
		g_bWaveFlag[nCnt] = false;
	}

	for (nCnt = 0; nCnt < MAX_WAVE+1; nCnt++)
	{
		g_nScoreBonustime[nCnt] = 6000;//一分のスコアボーナスまでの制限時間を設ける（60f*100s)
		g_bScoreBonusTimeCountStart[nCnt] = false;//ウェーブ開始時にtrueにし、スコアボーナスのカウントをスタートする。
	}
	

	
	g_bScoreBonusTimeCountStart[0] = true;

	//SetEnemy(D3DXVECTOR3(150.0f, 100.0f, 0.0f), 0);//敵の位置の初期化。右の数字は、敵の種類数


	//コピー用=====================================================================================
	//       , D3DXVECTOR3((sinf(-D3DX_PI * 0.5f) * 5.0f), (cosf(-D3DX_PI * 0.5f) * 5.0f), 0.0f)          敵を動かす
	//       , D3DXVECTOR3(0.0f,0.0f,0.0f)                                                                敵を動かさない
	//=============================================================================================



	////WAVE1============================
	///*SetEnemy(D3DXVECTOR3(300.0f, 100.0f, 0.0f),ENEMYTYPE_BOSS1_STARWARRIOR,1000.0f, D3DXVECTOR3((sinf(-D3DX_PI * 0.5f) * 5.0f), (cosf(-D3DX_PI * 0.5f) * 5.0f), 0.0f));*/
	//SetEnemy(D3DXVECTOR3(600.0f, 100.0f, 0.0f),ENEMYTYPE_DELBY,10.0f, D3DXVECTOR3((sinf(-D3DX_PI * 0.0f) * 5.0f), (cosf(-D3DX_PI * 0.0f) * 5.0f), 0.0f));
	//SetEnemy(D3DXVECTOR3(600.0f, 100.0f, 0.0f), ENEMYTYPE_DELBY, 30.0f, D3DXVECTOR3((sinf(-D3DX_PI * 0.5f) * 5.0f), (cosf(-D3DX_PI * 0.5f) * 5.0f), 0.0f));
	////SetEnemy(D3DXVECTOR3(SCREEN_WIDTH/2, 100.0f, 0.0f), ENEMYTYPE_BOSS1_STARWARRIOR,2000.0f, D3DXVECTOR3((sinf(-D3DX_PI * 0.0f) * 0.0f), (cosf(-D3DX_PI * 0.0f) * 0.0f), 0.0f));
	//SetEnemy(D3DXVECTOR3(750.0f, 100.0f, 0.0f), ENEMYTYPE_DELBY, 20.0f, D3DXVECTOR3((sinf(D3DX_PI * 0.5f) * 5.0f), (cosf(D3DX_PI * 0.5f) * 5.0f), 0.0f));
	//SetEnemy(D3DXVECTOR3(750.0f, 100.0f, 0.0f), ENEMYTYPE_DELBY, 20.0f, D3DXVECTOR3((sinf(-D3DX_PI * 0.5f) * 5.0f), (cosf(-D3DX_PI * 0.5f) * 5.0f), 0.0f));
	//////===================================================================================================================
	////TESTWAVE=========================

	//
	//==========================================================================================================================================================

	g_GameState = GAMESTATE_NORMAL;//通常状態に設定
	g_nCounterGameState = 0;
	//キーボードの初期化処理
	//InitKeyboard;
}

//==================================
//ゲーム画面の終了処理
//==================================
void UninitGame(void)
{
	UninitBG();
	UninitEffect();
	UninitPlayer();
	UninitHealth();
	UninitGage();
	UninitSkillName();
	UninitSkillGage();
	UninitDamage();
	UninitEnemy();
	UninitExplosion();
	UninitWave();
	UninitBullet();
	UninitScore();
	UninitScoreBonus();
	UninitStaging();
	UninitPause();
	UninitParticle();
	UninitFade();
	//InitKeyboard();
}

//=================================
//ゲーム画面の更新処理
//=================================
void UpdateGame(void)
{

	ResultType* pResultType = GetResultType();

	/*Result* pResult;*/
	int nData;

	int nCnt;

	if (GetKeyboardTrigger(DIK_P) == true||GetJoyPadTrigger(JOYKEY_BACK)==true)
	{//ポーズキーが押された
		g_bPause = g_bPause ? false : true;
	}

	if (g_bPause == false)
	{
		g_nCountPauseMenu = 0;

		g_nCountTime++;//ゲーム開始時からの時間を計測する。

	/*	if (g_nCountTime == 1)
		{
			srand((unsigned int)time(NULL));
		}*/

		for (nCnt = 0; nCnt < MAX_ENEMY * 2; nCnt++)
		{
			g_fRandumEnemyPosX[nCnt] = rand() % SCREEN_WIDTH + 1;//0～100の値を設定(乱数弾）
			g_fRandumEnemyPosY[nCnt] = rand() % SCREEN_HEIGHT / 2 + 1;
		}

		for (nCnt = 0; nCnt < MAX_WAVE + 1; nCnt++)
		{
			if (g_bScoreBonusTimeCountStart[nCnt] == true)
			{
				g_nScoreBonustime[nCnt]--;//nCntウェーブ目のスコアボーナスのカウントをスタート
			}
		}

		/*g_nScoreAdd++;*/
		/*UpDateBG();
		UpdateBullet();
		UpDatePlayer();
		UpdateExplosion();
		UpdateEnemy();
		UpdateScore();*/

		UpDateBG();
		UpdateEffect();
		UpDatePlayer();
		UpdateHealth(0);//ダメージを受けた回数（０回）を引数として渡す。
		UpdateGage();
		UpdateSkillName();
		UpdateSkillGage();
		UpdateDamage();
		UpdateEnemy();
		UpdateExplosion();
		UpdateWave();
		UpdateBullet();
		UpdateScore();
		UpdateScoreBonus();
		UpdateStaging();
		UpdateParticle();
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
		if (GetKeyboardTrigger(DIK_S) == true||GetJoyPadTrigger(JOYKEY_DOWN)==true)
		{
			g_nChoosePauseCommand += 1;//下に選択を移動する。
		}
		if (GetKeyboardTrigger(DIK_W) == true||GetJoyPadTrigger(JOYKEY_UP)==true)
		{
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

			if (GetKeyboardTrigger(DIK_RETURN) == true||GetJoyPadTrigger(JOYKEY_START)==true)
			{
				StopSound(SOUND_LABEL_BGM001);
				StopSound(SOUND_LABEL_BGM003);
				g_bPause = false;
			}
		}
		if (g_nChoosePauseCommand == 1)
		{//リトライコマンドを選択
			SetEnablePause(PAUSE_MENU_RETRY);
			if (GetKeyboardTrigger(DIK_RETURN) == true||GetJoyPadTrigger(JOYKEY_START)==true)
			{
				StopSound(SOUND_LABEL_BGM001);
				StopSound(SOUND_LABEL_BGM003);
				SetMode(MODE_GAME);
			}
		}
		if (g_nChoosePauseCommand == 2)
		{//タイトル画面に戻るコマンドを選択
			SetEnablePause(PAUSE_MENU_QUIT);
			if (GetKeyboardTrigger(DIK_RETURN) == true||GetJoyPadTrigger(JOYKEY_START)==true)
			{
				StopSound(SOUND_LABEL_BGM001);
				StopSound(SOUND_LABEL_BGM003);
				SetMode(MODE_TITLE);
			}
		}
	}

	Enemy *pEnemy=GetEnemy();

	Defeat* pDefeat = GetDefeat();

	


	switch (g_GameState)
	{
	case GAMESTATE_NORMAL://通常状態
		break;
	case GAMESTATE_END:
		g_nCounterGameState++;
		if (g_nCounterGameState >= 150)
		{
			
		

			//画面（モード）の設定
			Result* pResult = GetResult();
			Result_2* pResult_2 = GetResult_2();
			if (pResult->bResultFlag == true||pResult_2->bResultFlag2==true)
			{
				
				pResultType->bResultMode = true;

				StopSound(SOUND_LABEL_BGM001);
				StopSound(SOUND_LABEL_BGM003);





				SetMode(MODE_RESULT);
			}
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
	DrawEffect();
	DrawPlayer();
	DrawHealth();
	DrawGage();
	DrawSkillName();
	DrawSkillGage();
	DrawDamage();
	DrawEnemy();
	DrawExplosion();
	DrawWave();
	DrawBullet();
	DrawScore();
	DrawScoreBonus();
	DrawStaging();
	DrawParticle();
	DrawFade();
	if (g_bPause == true)
	{//ポーズ中
		//ポーズの描画処理
		DrawPause();
	}
}

//============================================
//ゲームの状態の設定
//============================================
void SetGameState(GAMESTATE state)
{
	g_GameState = state;
	/*g_nCounterGameState = 0;*/
}

//==============================================
//ゲームの状態の取得
//==============================================
GAMESTATE GetGameState(void)
{
	return g_GameState;
}
