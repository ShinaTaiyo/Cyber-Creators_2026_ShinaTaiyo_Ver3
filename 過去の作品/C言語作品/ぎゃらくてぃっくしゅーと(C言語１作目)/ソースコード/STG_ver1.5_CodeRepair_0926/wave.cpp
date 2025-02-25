//========================================================
//
//８月３０日：エフェクトもう一回見直す[Wave.cpp]
//Author:ShinaTaiyo
//
//=========================================================

#include "main.h"
#include "Wave.h"
#include "input.h"
#include "explosion.h"
#include "enemy.h"
#include "player.h"
#include "health.h"
#include "result.h"
#include "wave.h"
#include "score.h"
#include "game.h"
#include "Difficulty.h"
#include "staging.h"
#include "scorebonus.h"

#define MAX_TIME (3)//弾の最大数
#define NUM_TIME (1)
#define MAX_WAVE (30)
#define TIMESCALEX (100)
#define TIMESCALEY (50)
#define TIMESCALE (100)

#define Wave_RADIUS (20.0f)

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureTime = NULL;//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTime = NULL;//弾の情報
Wave g_aWave[MAX_WAVE];
//float g_fXMove;//X座標の移動
//float g_fYMove;//Y座標の移動
D3DXVECTOR3 g_posWave;//位置
D3DXVECTOR3 g_moveWave;//移動量

//D3DXVECTOR3 g_rotPl;//向き

float g_fTIMESCALE = 0;

int g_aTimePosTexU[3];//各桁の数字を格納


bool g_bWaveFlag;//次のウェーブに進ませるフラグ（進みすぎないように調整する用）


int g_nNumWaveBonustime[MAX_TIME + 1];
bool g_bWaveBonusTimeCountStart[MAX_TIME + 1];//それぞれのウェーブが始まったときに、ウェーブクリア時間スコアボーナスのカウントをスタートする。

int g_nNumWave;//ウェーブ判定変数


bool g_bNohitBonusFlag;//プレイヤーが被弾なしでクリアした場合にスコアボーナスを与えるフラグ

bool g_bClearWaveFlag;//クリアしたときのスコアや画面遷移などの処理をするフラグ
//弾の初期化処理
void InitWave(void)
{
	ResultType* pResultType = GetResultType();

	g_nNumWave = -1;//最初は画面にいる敵の数は0なので、自然にg_nNumWaveはインクリメントされるから、-1から始める。

	LPDIRECT3DDEVICE9 pDevice;
	int	nCntWave;

	g_bNohitBonusFlag = false;

	//デバイスの取得
	pDevice = GetDevice();

	g_bClearWaveFlag = false;
	//D3DXCreateTextureFromFile(pDevice,
	//	"data\\number_001.png",
	//	&g_pTextureTime);
	////テクスチャの読み込み（スコアのテクスチャを読み込む）
	//for (nCntWave = 0; nCntWave < MAX_TIME; nCntWave++)
	//{

	//	g_aTimePosTexU[nCntWave] = 0;
	//}



		//スコア情報の初期化
	for (nCntWave = 0; nCntWave < MAX_TIME; nCntWave++)
	{

		g_aWave[nCntWave].pos = D3DXVECTOR3(SCREEN_WIDTH - ((nCntWave + 1) * 50.0f), 20.0f, 0.0f);
		/*g_aWave[nCntWave].pos = D3DXVECTOR3(0.0f,0.0f, 0.0f);*/
		/*g_aWave[nCntWave].move = D3DXVECTOR3(4.0f, 4.0f, 0.0f);
		g_aWave[nCntWave].nLife = 1000;*/
		g_aWave[nCntWave].bUse = false;//使用していない状態にする

	/*	g_aWave[nCntWave].type = BULLETTYPE_PLAYER;*/
	}

	//頂点バッファの生成
	//pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TIME,//４は必要な頂点数
	//	D3DUSAGE_WRITEONLY,
	//	FVF_VERTEX_2D,
	//	D3DPOOL_MANAGED,
	//	&g_pVtxBuffTime,
	//	NULL);

	VERTEX_2D* pVtx;

	int nCnt;
	//=================================================
	//ウェーブ処理
	//=================================================
	for (nCnt = 0; nCnt < MAX_WAVE; nCnt++)
	{
	     g_aWave[nCnt].bWaveFlag = false;
	}

	for (nCnt = 0; nCnt < MAX_WAVE; nCnt++)
	{
	    g_aWave[nCnt].nBonusCountStart = 6000;//一分のスコアボーナスまでの制限時間を設ける（60f*100s)
		g_aWave[nCnt].bWaveFlag = false;//ウェーブ開始時にtrueにし、スコアボーナスのカウントをスタートする。
		g_aWave[nCnt].bBonusCountFlag = false;
	}


	g_bWaveBonusTimeCountStart[0] = true;

	g_bWaveFlag = true;
	//======================================================================================================================

	////頂点バッファをロックし、頂点データへのポインタを取得
	//g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	//for (nCntWave = 0; nCntWave < MAX_TIME; nCntWave++)
	//{
	//	//頂点座標の設定


	//	/*pVtx[0].pos = D3DXVECTOR3(g_aWave[nCntWave].pos.x - 20.0f - g_fTIMESCALE, g_aWave[nCntWave].pos.y - 20.0f, 0.0f);
	//	pVtx[1].pos = D3DXVECTOR3(g_aWave[nCntWave].pos.x + 20.0f + g_fTIMESCALE, g_aWave[nCntWave].pos.y - 20.0f, 0.0f);
	//	pVtx[2].pos = D3DXVECTOR3(g_aWave[nCntWave].pos.x - 20.0f - g_fTIMESCALE, g_aWave[nCntWave].pos.y + 20.0f, 0.0f);
	//	pVtx[3].pos = D3DXVECTOR3(g_aWave[nCntWave].pos.x + 20.0f + g_fTIMESCALE, g_aWave[nCntWave].pos.y + 20.0f, 0.0f);*/

	//	/*	pVtx[0].pos = D3DXVECTOR3(g_aWave[nCntWave].pos.x - 20.0f + g_fTIMESCALE, g_aWave[nCntWave].pos.y - 20.0f, 0.0f);
	//		pVtx[1].pos = D3DXVECTOR3(g_aWave[nCntWave].pos.x + 20.0f + g_fTIMESCALE, g_aWave[nCntWave].pos.y - 20.0f, 0.0f);
	//		pVtx[2].pos = D3DXVECTOR3(g_aWave[nCntWave].pos.x - 20.0f + g_fTIMESCALE, g_aWave[nCntWave].pos.y + 20.0f, 0.0f);
	//		pVtx[3].pos = D3DXVECTOR3(g_aWave[nCntWave].pos.x + 20.0f + g_fTIMESCALE, g_aWave[nCntWave].pos.y + 20.0f, 0.0f);*/

	//	pVtx[0].pos = D3DXVECTOR3(g_aWave[nCntWave].pos.x - 20.0f, g_aWave[nCntWave].pos.y - 20.0f, 0.0f);
	//	pVtx[1].pos = D3DXVECTOR3(g_aWave[nCntWave].pos.x + 20.0f, g_aWave[nCntWave].pos.y - 20.0f, 0.0f);
	//	pVtx[2].pos = D3DXVECTOR3(g_aWave[nCntWave].pos.x - 20.0f, g_aWave[nCntWave].pos.y + 20.0f, 0.0f);
	//	pVtx[3].pos = D3DXVECTOR3(g_aWave[nCntWave].pos.x + 20.0f, g_aWave[nCntWave].pos.y + 20.0f, 0.0f);

	//	/*	g_aWave[nCntWave].nLife = 1000;*/
	//	g_aWave[nCntWave].bUse = true;//使用している状態にする
	//	/*g_rotPl[nCntWave] = 200;*/


	//	//rhwの設定
	//	pVtx[0].rhw = 1.0f;
	//	pVtx[1].rhw = 1.0f;
	//	pVtx[2].rhw = 1.0f;
	//	pVtx[3].rhw = 1.0f;



	//	//頂点カラーの設定
	//	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
	//	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
	//	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
	//	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。

	//	//テクスチャ座標の設定
	//	/*pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	//	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	//	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	//	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);*/

	//	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	//	pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
	//	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	//	pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
	//	/*	g_fTIMESCALE += 10.0f;*/

	//	pVtx += 4;//頂点データのポインタを４つ分進める。
	//}
	////頂点バッファをアンロックする
	//g_pVtxBuffTime->Unlock();

	///*for (nCntWave = 0; nCntWave < MAX_TIME; nCntWave++)
	//{
	//	SetWave(10);
	//}*/
	///*g_mode = GetMode();*/

	///*if (g_mode == MODE_RESULT)
	//{*/
	///*	g_nNumWave = g_nSaveWave;*/
	////}



}

//弾の終了処理
void UninitWave(void)
{
	int nCntWave;
	//テクスチャの破棄
	//for (nCntWave = 0; nCntWave < MAX_TIME; nCntWave++)
	//{
	//if (g_pTextureTime != NULL)
	//{
	//	g_pTextureTime->Release();
	//	g_pTextureTime = NULL;
	//}
	////}
	////頂点バッファの破棄
	//if (g_pVtxBuffTime != NULL)
	//{
	//	g_pVtxBuffTime->Release();
	//	g_pVtxBuffTime = NULL;
	//}
}

//弾の更新処理
void UpdateWave(void)
{
	Defeat* pDefeat = GetDefeat();
	Result* pResult = GetResult();
	ResultType* pResultType = GetResultType();
	Player* pPlayer = GetPlayer();
	GameCustom* pGameCustom = GetGameCustom();
	int nCnt;
	for (nCnt = 0; nCnt < MAX_WAVE + 1; nCnt++)
	{
		if (g_aWave[nCnt].bWaveFlag == true)
		{
			g_aWave[nCnt].nBonusCountStart--;//nCntウェーブ目のスコアボーナスのカウントをスタート
		}
	}
	if (pDefeat->DefeatEnemyCount == 0)
	{
		g_bWaveFlag = true;
	}
	if (pGameCustom->nWaveLength == 1)//Waveの長さ「Short」
	{
		if (g_bWaveFlag==true)
		{
			g_nNumWave++;
			g_bWaveFlag = false;

			switch (g_nNumWave)
			{
			case 0:
				//WAVE0============================
				if (g_aWave[g_nNumWave].bWaveFlag == false)
				{
					g_aWave[g_nNumWave].bWaveFlag = true;//発動した瞬間にフラグを変えることにより、一回のみエネミーをセットする。
					g_aWave[g_nNumWave].bBonusCountFlag = true;//ウェーブ０のボーナスカウントを開始
					/*SetEnemy(D3DXVECTOR3(300.0f, 100.0f, 0.0f),ENEMYTYPE_BOSS1_STARWARRIOR,1000.0f, D3DXVECTOR3((sinf(-D3DX_PI * 0.5f) * 5.0f), (cosf(-D3DX_PI * 0.5f) * 5.0f), 0.0f));*/
					SetEnemy(D3DXVECTOR3(600.0f, 100.0f, 0.0f), ENEMYTYPE_DELBY, 30.0f, D3DXVECTOR3((sinf(-D3DX_PI * 0.5f) * 3.0f), (cosf(-D3DX_PI * 0.5f) * 3.0f), 0.0f));
					SetEnemy(D3DXVECTOR3(600.0f, 100.0f, 0.0f), ENEMYTYPE_DELBY, 30.0f, D3DXVECTOR3((sinf(-D3DX_PI * 0.5f) * 2.0f), (cosf(-D3DX_PI * 0.5f) * 2.0f), 0.0f));
					SetEnemy(D3DXVECTOR3(600.0f, 100.0f, 0.0f), ENEMYTYPE_DELBY, 30.0f, D3DXVECTOR3((sinf(-D3DX_PI * 0.5f) * 1.0f), (cosf(-D3DX_PI * 0.5f) * 1.0f), 0.0f));

					////===================================================================================================================
					//TESTWAVE=========================
					//SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2, 100.0f, 0.0f), ENEMYTYPE_BOSS1_STARWARRIOR, 5.0f, D3DXVECTOR3((sinf(-D3DX_PI * 0.0f) * 0.0f), (cosf(-D3DX_PI * 0.0f) * 0.0f), 0.0f));
					//==========================================================================================================================================================
				}
				break;
			case 1:
				//WAVE1==============================
				if (g_aWave[g_nNumWave].bWaveFlag == false)
				{
					g_aWave[g_nNumWave].bWaveFlag = true;//発動した瞬間にフラグを変えることにより、一回のみエネミーをセットする。
					g_aWave[g_nNumWave].bBonusCountFlag = true;//ウェーブ1のボーナスカウントを開始
					g_aWave[g_nNumWave - 1].bBonusCountFlag = false;//一つ前のウェーブが終わったので、一つ前のウェーブ（g_nNumWave-1）のスコアボーナスのカウントを止める。
					AddScore(g_aWave[g_nNumWave - 1].nBonusCountStart * 50);//ひとつ前のウェーブのクリア時間が早いほどスコアを加算する。
					SetScoreBonus(g_aWave[g_nNumWave - 1].nBonusCountStart * 50);//今回どれくらいスコアボーナスが付与されたかを表示する

					//SetEnemy(D3DXVECTOR3(SCREEN_WIDTH/2-150.0f, 100.0f, 0.0f), ENEMYTYPE_DELBY, 50.0f, D3DXVECTOR3(0.0f,0.0f,0.0f));
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH/2,        100.0f, 0.0f), ENEMYTYPE_SMILY, 50.0f, D3DXVECTOR3(0.0f,0.0f,0.0f));
					//SetEnemy(D3DXVECTOR3(SCREEN_WIDTH/2+150.0f, 100.0f, 0.0f), ENEMYTYPE_DELBY, 50.0f, D3DXVECTOR3(0.0f,0.0f,0.0f));
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH/2-500.0f, 90.0f, 0.0f), ENEMYTYPE_SQUDY, 75.0f, D3DXVECTOR3((sinf(-D3DX_PI * 1.5f) * 1.0f), (cosf(-D3DX_PI * 1.5f) * 1.0f), 0.0f));
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH/2+500.0f, 90.0f, 0.0f), ENEMYTYPE_SQUDY, 75.0f, D3DXVECTOR3((sinf(-D3DX_PI * 0.5f) * 1.0f), (cosf(-D3DX_PI * 0.5f) * 1.0f), 0.0f));

				}
				break;
			case 2:
				//WAVE1==============================
				if (g_aWave[g_nNumWave].bWaveFlag == false)
				{
					g_aWave[g_nNumWave].bWaveFlag = true;//発動した瞬間にフラグを変えることにより、一回のみエネミーをセットする。
					g_aWave[g_nNumWave].bBonusCountFlag = true;//ウェーブ1のボーナスカウントを開始
					g_aWave[g_nNumWave - 1].bBonusCountFlag = false;//一つ前のウェーブが終わったので、一つ前のウェーブ（g_nNumWave-1）のスコアボーナスのカウントを止める。
					AddScore(g_aWave[g_nNumWave - 1].nBonusCountStart * 50);//ひとつ前のウェーブのクリア時間が早いほどスコアを加算する。
					SetScoreBonus(g_aWave[g_nNumWave - 1].nBonusCountStart * 50);//今回どれくらいスコアボーナスが付与されたかを表示する
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2-400.0f, SCREEN_HEIGHT / 2-200.0f, 0.0f), ENEMYTYPE_REAFY, 60.0f, D3DXVECTOR3((sinf(-D3DX_PI * 1.0f) * 5.0f), (cosf(-D3DX_PI * 1.0f) * 5.0f), 0.0f));
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 , 0.0f), ENEMYTYPE_PUNDY, 80.0f, D3DXVECTOR3((sinf(-D3DX_PI * 1.0f) * 3.0f), (cosf(-D3DX_PI * 1.0f) * 3.0f), 0.0f));
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2+400.0f, SCREEN_HEIGHT / 2+200.0f, 0.0f), ENEMYTYPE_REAFY, 65.0f, D3DXVECTOR3((sinf(-D3DX_PI * 1.0f) * 5.0f), (cosf(-D3DX_PI * 1.0f) * 5.0f), 0.0f));

				}
				break;
			case 3:
				//WAVE1==============================
				if (g_aWave[g_nNumWave].bWaveFlag == false)
				{
					g_aWave[g_nNumWave].bWaveFlag = true;//発動した瞬間にフラグを変えることにより、一回のみエネミーをセットする。
					g_aWave[g_nNumWave].bBonusCountFlag = true;//ウェーブ1のボーナスカウントを開始
					g_aWave[g_nNumWave - 1].bBonusCountFlag = false;//一つ前のウェーブが終わったので、一つ前のウェーブ（g_nNumWave-1）のスコアボーナスのカウントを止める。
					AddScore(g_aWave[g_nNumWave - 1].nBonusCountStart * 50);//ひとつ前のウェーブのクリア時間が早いほどスコアを加算する。
					SetScoreBonus(g_aWave[g_nNumWave - 1].nBonusCountStart * 50);//今回どれくらいスコアボーナスが付与されたかを表示する
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2 + 100.0f, 10.0f, 0.0f), ENEMYTYPE_HONOBY, 40.0f, D3DXVECTOR3(0.0f,0.0f, 0.0f));
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2, 50.0f, 0.0f), ENEMYTYPE_SMILY, 40.0f, D3DXVECTOR3((sinf(-D3DX_PI * 0.5f) * 5.0f), (cosf(-D3DX_PI * 0.5f) * 5.0f), 0.0f));//敵の位置の初期化。右の数字は、敵のタイプ
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2 - 200.0f, 50.0f, 0.0f), ENEMYTYPE_SQUDY, 60.0f, D3DXVECTOR3((sinf(-D3DX_PI * 0.5f) * 5.0f), (cosf(-D3DX_PI * 0.5f) * 5.0f), 0.0f));//敵の位置の初期化。右の数字は、敵のタイプ
				}
				break;
			case 4:
				//WAVE1==============================
				if (g_aWave[g_nNumWave].bWaveFlag == false)
				{
					g_aWave[g_nNumWave].bWaveFlag = true;//発動した瞬間にフラグを変えることにより、一回のみエネミーをセットする。
					g_aWave[g_nNumWave].bBonusCountFlag = true;//ウェーブ1のボーナスカウントを開始
					g_aWave[g_nNumWave - 1].bBonusCountFlag = false;//一つ前のウェーブが終わったので、一つ前のウェーブ（g_nNumWave-1）のスコアボーナスのカウントを止める。
					AddScore(g_aWave[g_nNumWave - 1].nBonusCountStart * 50);//ひとつ前のウェーブのクリア時間が早いほどスコアを加算する。
					SetScoreBonus(g_aWave[g_nNumWave - 1].nBonusCountStart * 50);//今回どれくらいスコアボーナスが付与されたかを表示する
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2-630.0f, 150.0f, 0.0f), ENEMYTYPE_MIZUBY, 65.0f, D3DXVECTOR3((sinf(-D3DX_PI * 1.5f) * 4.0f), (cosf(-D3DX_PI * 1.5f) * 4.0f), 0.0f));
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2, 10.0f, 0.0f), ENEMYTYPE_WINBY, 95.0f, D3DXVECTOR3((sinf(-D3DX_PI * 0.5f) * 5.0f), (cosf(-D3DX_PI * 0.5f) * 5.0f), 0.0f));
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2+630.0f,150.0f, 0.0f), ENEMYTYPE_MIZUBY, 65.0f, D3DXVECTOR3((sinf(-D3DX_PI * 0.5f) * 4.0f), (cosf(-D3DX_PI * 0.5f) * 4.0f), 0.0f));
					
				}
				break;
			case 5:
				//WAVE1==============================
				if (g_aWave[g_nNumWave].bWaveFlag == false)
				{
					g_aWave[g_nNumWave].bWaveFlag = true;//発動した瞬間にフラグを変えることにより、一回のみエネミーをセットする。
					g_aWave[g_nNumWave].bBonusCountFlag = true;//ウェーブ1のボーナスカウントを開始
					g_aWave[g_nNumWave - 1].bBonusCountFlag = false;//一つ前のウェーブが終わったので、一つ前のウェーブ（g_nNumWave-1）のスコアボーナスのカウントを止める。
					AddScore(g_aWave[g_nNumWave - 1].nBonusCountStart * 50);//ひとつ前のウェーブのクリア時間が早いほどスコアを加算する。
					SetScoreBonus(g_aWave[g_nNumWave - 1].nBonusCountStart * 50);//今回どれくらいスコアボーナスが付与されたかを表示する
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2-500.0f, SCREEN_HEIGHT/2-300.0f, 0.0f), ENEMYTYPE_BOMUBY, 5.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f));//敵の位置の初期化。右の数字は、敵のタイプ
					//SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2 +400.0f,SCREEN_HEIGHT/2-200.0f, 0.0f), ENEMYTYPE_BOMUBY, 5.0f, D3DXVECTOR3(0.0f,0.0f,0.0f));
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2 +300.0f, 30.0f, 0.0f), ENEMYTYPE_BOLBY, 150.0f, D3DXVECTOR3((sinf(-D3DX_PI * 0.5f) * 4.0f), (cosf(-D3DX_PI * 0.5f) * 4.0f), 0.0f));
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2+135.0f, SCREEN_HEIGHT/2-238.0f, 0.0f), ENEMYTYPE_PUNDY, 80.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f));//敵の位置の初期化。右の数字は、敵のタイプ
					//SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2-140.0f, 40.0f, 0.0f), ENEMYTYPE_SQUDY, 70.0f, D3DXVECTOR3((sinf(-D3DX_PI * 1.5f) * 6.0f), (cosf(-D3DX_PI * 1.5f) * 6.0f), 0.0f));
					
				}
				break;
			case 6:
				//WAVE1==============================
				if (g_aWave[g_nNumWave].bWaveFlag == false)
				{
					g_aWave[g_nNumWave].bWaveFlag = true;//発動した瞬間にフラグを変えることにより、一回のみエネミーをセットする。
					g_aWave[g_nNumWave].bBonusCountFlag = true;//ウェーブ1のボーナスカウントを開始
					g_aWave[g_nNumWave - 1].bBonusCountFlag = false;//一つ前のウェーブが終わったので、一つ前のウェーブ（g_nNumWave-1）のスコアボーナスのカウントを止める。
					AddScore(g_aWave[g_nNumWave - 1].nBonusCountStart * 50);//ひとつ前のウェーブのクリア時間が早いほどスコアを加算する。
					SetScoreBonus(g_aWave[g_nNumWave - 1].nBonusCountStart * 50);//今回どれくらいスコアボーナスが付与されたかを表示する
					SetEnemy(D3DXVECTOR3(20.0f, 20.0f, 0.0f), ENEMYTYPE_BANDY, 90.0f, D3DXVECTOR3(0.0f,0.0f, 0.0f));
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH-20.0f, 20.0f, 0.0f), ENEMYTYPE_SMILY, 60.0f, D3DXVECTOR3((sinf(-D3DX_PI * 0.54f) * 4.0f), (cosf(-D3DX_PI * 0.54f) * 4.0f), 0.0f));
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH/2+230.0f, SCREEN_HEIGHT/2 + 100.0f, 0.0f), ENEMYTYPE_DELBY, 60.0f, D3DXVECTOR3((sinf(-D3DX_PI * 0.54f) * 4.0f), (cosf(-D3DX_PI * 0.54f) * 4.0f), 0.0f));
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH/2,50.0f, 0.0f), ENEMYTYPE_PUNDY, 60.0f, D3DXVECTOR3(0.0f,0.0f, 0.0f));


				}
				break;
			case 7:
				//WAVE1==============================
				if (g_aWave[g_nNumWave].bWaveFlag == false)
				{

					g_aWave[g_nNumWave].bWaveFlag = true;//発動した瞬間にフラグを変えることにより、一回のみエネミーをセットする。
					g_aWave[g_nNumWave].bBonusCountFlag = true;//ウェーブ1のボーナスカウントを開始
					g_aWave[g_nNumWave - 1].bBonusCountFlag = false;//一つ前のウェーブが終わったので、一つ前のウェーブ（g_nNumWave-1）のスコアボーナスのカウントを止める。
					AddScore(g_aWave[g_nNumWave - 1].nBonusCountStart * 50);//ひとつ前のウェーブのクリア時間が早いほどスコアを加算する。
					SetScoreBonus(g_aWave[g_nNumWave - 1].nBonusCountStart * 50);//今回どれくらいスコアボーナスが付与されたかを表示する
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH/2,5.0f, 0.0f), ENEMYTYPE_DELBY, 60.0f, D3DXVECTOR3(0.0f,0.0f,0.0f));//敵の位置の初期化。右の数字は、敵のタイプ
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH/2,5.0f, 0.0f), ENEMYTYPE_HONOBY, 50.0f, D3DXVECTOR3((sinf(-D3DX_PI * 0.5f) * 5.0f), (cosf(-D3DX_PI * 0.5f) * 5.0f), 0.0f));//敵の位置の初期化。右の数字は、敵のタイプ
					//SetEnemy(D3DXVECTOR3(SCREEN_WIDTH/2,5.0f, 0.0f), ENEMYTYPE_BOLBY, 80.0f, D3DXVECTOR3((sinf(-D3DX_PI * 0.5f) * 5.0f), (cosf(-D3DX_PI * 0.5f) * 5.0f), 0.0f));//敵の位置の初期化。右の数字は、敵のタイプ
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH/2,5.0f, 0.0f), ENEMYTYPE_MIZUBY, 70.0f, D3DXVECTOR3((sinf(-D3DX_PI * 0.5f) * 5.0f), (cosf(-D3DX_PI * 0.5f) * 5.0f), 0.0f));//敵の位置の初期化。右の数字は、敵のタイプ
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2-400.0f, 280.0f, 0.0f), ENEMYTYPE_REAFY, 50.0f, D3DXVECTOR3(0.0f,0.0f,0.0f));//敵の位置の初期化。右の数字は、敵のタイプ
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2 + 200.0f, 380.0f, 0.0f), ENEMYTYPE_REAFY, 70.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f));//敵の位置の初期化。右の数字は、敵のタイプ

				}
				break;
			case 8:
				//WAVE1==============================
				if (g_aWave[g_nNumWave].bWaveFlag == false)
				{
					g_aWave[g_nNumWave].bWaveFlag = true;//発動した瞬間にフラグを変えることにより、一回のみエネミーをセットする。
					g_aWave[g_nNumWave].bBonusCountFlag = true;//ウェーブ1のボーナスカウントを開始
					g_aWave[g_nNumWave - 1].bBonusCountFlag = false;//一つ前のウェーブが終わったので、一つ前のウェーブ（g_nNumWave-1）のスコアボーナスのカウントを止める。
					AddScore(g_aWave[g_nNumWave - 1].nBonusCountStart * 50);//ひとつ前のウェーブのクリア時間が早いほどスコアを加算する。
					SetScoreBonus(g_aWave[g_nNumWave - 1].nBonusCountStart * 50);//今回どれくらいスコアボーナスが付与されたかを表示する
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2, 100.0f, 0.0f), ENEMYTYPE_BOSS1_STARWARRIOR, 5000.0f, D3DXVECTOR3((sinf(-D3DX_PI * 0.0f) * 0.0f), (cosf(-D3DX_PI * 0.0f) * 0.0f), 0.0f));

					

				}
				break;
			case 9:
				//WAVE1==============================
				if (g_aWave[g_nNumWave].bWaveFlag == false)
				{
					Player* pPlayer = GetPlayer();
					int nNomissScore = 0;//ノーミスだった場合のスコア表示を行う。
					int nNotSkillUseBonus = 0;
					if (pPlayer->nHitCount == 0)
					{
						nNomissScore = 3000000;
						AddScore(3000000);
					}
					if (pPlayer->nSkillUseCount == 0)
					{//一度もスキルを使わずにクリアした場合・・・
						nNotSkillUseBonus = 2000000;
						AddScore(2000000);
					}
					g_aWave[g_nNumWave - 1].bBonusCountFlag = false;//一つ前のウェーブが終わったので、一つ前のウェーブ（g_nNumWave-1）のスコアボーナスのカウントを止める。
					AddScore(g_aWave[g_nNumWave - 1].nBonusCountStart * 50);//ひとつ前のウェーブのクリア時間が早いほどスコアを加算する。
					if (pGameCustom->nDifficulty == 1)
					{
						SetStaging(STAGINGTYPE_NORMAL_CLEARBONUS, -1, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
						SetScoreBonus(g_aWave[g_nNumWave - 1].nBonusCountStart * 50 + nNomissScore + nNotSkillUseBonus+3000000);//今回どれくらいスコアボーナスが付与されたかを表示する
					}
					if (pGameCustom->nDifficulty == 2)
					{
						SetStaging(STAGINGTYPE_NORMAL_CLEARBONUS, -1, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
						AddScore(300000);
						SetScoreBonus(g_aWave[g_nNumWave - 1].nBonusCountStart * 50 + 300000 + nNomissScore + nNotSkillUseBonus+3000000);//今回どれくらいスコアボーナスが付与されたかを表示する
					}
					if (pGameCustom->nDifficulty == 3)
					{
						SetStaging(STAGINGTYPE_HARD_CLEARBONUS, -1, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
						AddScore(500000);
						SetScoreBonus(g_aWave[g_nNumWave - 1].nBonusCountStart * 50 + 500000 + nNomissScore + nNotSkillUseBonus+3000000);//今回どれくらいスコアボーナスが付与されたかを表示する
					}
					if (pGameCustom->nDifficulty == 4)
					{
						SetStaging(STAGINGTYPE_VERYHARD_CLEARBONUS, -1, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
						AddScore(1000000);
						SetScoreBonus(g_aWave[g_nNumWave - 1].nBonusCountStart * 50 + 1000000 + nNomissScore + nNotSkillUseBonus+3000000);//今回どれくらいスコアボーナスが付与されたかを表示する
					}

				}
				break;
			}
			
		}
	}
		
	//=========================================================================================================================================================================
	if (pGameCustom->nWaveLength == 2)//Waveの長さ「Normal」
	{
		if (g_bWaveFlag == true)
		{
			g_nNumWave++;
			g_bWaveFlag = false;

			switch (g_nNumWave)
			{
			case 0:
				//WAVE0============================
				if (g_aWave[g_nNumWave].bWaveFlag == false)
				{
					g_aWave[g_nNumWave].bWaveFlag = true;//発動した瞬間にフラグを変えることにより、一回のみエネミーをセットする。
					g_aWave[g_nNumWave].bBonusCountFlag = true;//ウェーブ０のボーナスカウントを開始
					/*SetEnemy(D3DXVECTOR3(300.0f, 100.0f, 0.0f),ENEMYTYPE_BOSS1_STARWARRIOR,1000.0f, D3DXVECTOR3((sinf(-D3DX_PI * 0.5f) * 5.0f), (cosf(-D3DX_PI * 0.5f) * 5.0f), 0.0f));*/
					SetEnemy(D3DXVECTOR3(600.0f, 100.0f, 0.0f), ENEMYTYPE_DELBY, 10.0f, D3DXVECTOR3((sinf(-D3DX_PI * 0.0f) * 5.0f), (cosf(-D3DX_PI * 0.0f) * 5.0f), 0.0f));
					//SetEnemy(D3DXVECTOR3(600.0f, 100.0f, 0.0f), ENEMYTYPE_BOSS1_STARWARRIOR, 3000.0f, D3DXVECTOR3((sinf(-D3DX_PI * 0.5f) * 5.0f), (cosf(-D3DX_PI * 0.5f) * 5.0f), 0.0f));
					//SetEnemy(D3DXVECTOR3(SCREEN_WIDTH/2, 100.0f, 0.0f), ENEMYTYPE_BOSS1_STARWARRIOR,2000.0f, D3DXVECTOR3((sinf(-D3DX_PI * 0.0f) * 0.0f), (cosf(-D3DX_PI * 0.0f) * 0.0f), 0.0f));
					SetEnemy(D3DXVECTOR3(750.0f, 100.0f, 0.0f), ENEMYTYPE_DELBY, 20.0f, D3DXVECTOR3((sinf(D3DX_PI * 0.5f) * 5.0f), (cosf(D3DX_PI * 0.5f) * 5.0f), 0.0f));
					SetEnemy(D3DXVECTOR3(750.0f, 100.0f, 0.0f), ENEMYTYPE_DELBY, 20.0f, D3DXVECTOR3((sinf(-D3DX_PI * 0.5f) * 5.0f), (cosf(-D3DX_PI * 0.5f) * 5.0f), 0.0f));

					////===================================================================================================================
					//TESTWAVE=========================

						//SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2, 100.0f, 0.0f), ENEMYTYPE_BOSS1_STARWARRIOR, 5000.0f, D3DXVECTOR3((sinf(-D3DX_PI * 0.0f) * 0.0f), (cosf(-D3DX_PI * 0.0f) * 0.0f), 0.0f));

					//==========================================================================================================================================================
				}
				break;
			case 1:
				//WAVE1==============================
				if (g_aWave[g_nNumWave].bWaveFlag == false)
				{
					g_aWave[g_nNumWave].bWaveFlag = true;//発動した瞬間にフラグを変えることにより、一回のみエネミーをセットする。
					g_aWave[g_nNumWave].bBonusCountFlag = true;//ウェーブ1のボーナスカウントを開始
					g_aWave[g_nNumWave - 1].bBonusCountFlag = false;//一つ前のウェーブが終わったので、一つ前のウェーブ（g_nNumWave-1）のスコアボーナスのカウントを止める。
					AddScore(g_aWave[g_nNumWave - 1].nBonusCountStart * 50);//ひとつ前のウェーブのクリア時間が早いほどスコアを加算する。
					SetScoreBonus(g_aWave[g_nNumWave - 1].nBonusCountStart * 50);//今回どれくらいスコアボーナスが付与されたかを表示する
					SetEnemy(D3DXVECTOR3(150.0f, 100.0f, 0.0f), ENEMYTYPE_DELBY, 50.0f, D3DXVECTOR3((sinf(-D3DX_PI * 1.5f) * 5.0f), (cosf(-D3DX_PI * 1.5f) * 5.0f), 0.0f));//敵の位置の初期化。右の数字は、敵のタイプ,
					SetEnemy(D3DXVECTOR3(1000.0f, 200.0f, 0.0f), ENEMYTYPE_SQUDY, 30.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f));//敵の位置の初期化。右の数字は、敵のタイプ,
					SetEnemy(D3DXVECTOR3(300.0f, 120.0f, 0.0f), ENEMYTYPE_SQUDY, 30.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f));//敵の位置の初期化。右の数字は、敵のタイプ,

				}
				break;
			case 2:
				//WAVE1==============================
				if (g_aWave[g_nNumWave].bWaveFlag == false)
				{
					g_aWave[g_nNumWave].bWaveFlag = true;//発動した瞬間にフラグを変えることにより、一回のみエネミーをセットする。
					g_aWave[g_nNumWave].bBonusCountFlag = true;//ウェーブ1のボーナスカウントを開始
					g_aWave[g_nNumWave - 1].bBonusCountFlag = false;//一つ前のウェーブが終わったので、一つ前のウェーブ（g_nNumWave-1）のスコアボーナスのカウントを止める。
					AddScore(g_aWave[g_nNumWave - 1].nBonusCountStart * 50);//ひとつ前のウェーブのクリア時間が早いほどスコアを加算する。
					SetScoreBonus(g_aWave[g_nNumWave - 1].nBonusCountStart * 50);//今回どれくらいスコアボーナスが付与されたかを表示する
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 20.0f, 0.0f), ENEMYTYPE_PUNDY, 60.0f, D3DXVECTOR3(0, 0, 0));
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 199.0f, 0.0f), ENEMYTYPE_SQUDY, 50.0f, D3DXVECTOR3((sinf(-D3DX_PI * 1.5f) * 5.0f), (cosf(-D3DX_PI * 1.5f) * 5.0f), 0.0f));
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 198.0f, 0.0f), ENEMYTYPE_SMILY, 45.0f, D3DXVECTOR3((sinf(-D3DX_PI * 0.5f) * 5.0f), (cosf(-D3DX_PI * 0.5f) * 5.0f), 0.0f));

				}
				break;
			case 3:
				//WAVE1==============================
				if (g_aWave[g_nNumWave].bWaveFlag == false)
				{
					g_aWave[g_nNumWave].bWaveFlag = true;//発動した瞬間にフラグを変えることにより、一回のみエネミーをセットする。
					g_aWave[g_nNumWave].bBonusCountFlag = true;//ウェーブ1のボーナスカウントを開始
					g_aWave[g_nNumWave - 1].bBonusCountFlag = false;//一つ前のウェーブが終わったので、一つ前のウェーブ（g_nNumWave-1）のスコアボーナスのカウントを止める。
					AddScore(g_aWave[g_nNumWave - 1].nBonusCountStart * 50);//ひとつ前のウェーブのクリア時間が早いほどスコアを加算する。
					SetScoreBonus(g_aWave[g_nNumWave - 1].nBonusCountStart * 50);//今回どれくらいスコアボーナスが付与されたかを表示する
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2 + 100.0f, 100.0f, 0.0f), ENEMYTYPE_SMILY, 40.0f, D3DXVECTOR3((sinf(-D3DX_PI * 0.5f) * 5.0f), (cosf(-D3DX_PI * 0.5f) * 5.0f), 0.0f));
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2, 50.0f, 0.0f), ENEMYTYPE_SMILY, 40.0f, D3DXVECTOR3((sinf(-D3DX_PI * 0.25f) * 5.0f), (cosf(-D3DX_PI * 0.25f) * 5.0f), 0.0f));//敵の位置の初期化。右の数字は、敵のタイプ
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2 - 200.0f, 230.0f, 0.0f), ENEMYTYPE_SQUDY, 60.0f, D3DXVECTOR3((sinf(-D3DX_PI * 0.0f) * 5.0f), (cosf(-D3DX_PI * 0.0f) * 5.0f), 0.0f));//敵の位置の初期化。右の数字は、敵のタイプ
				}
				break;
			case 4:
				//WAVE1==============================
				if (g_aWave[g_nNumWave].bWaveFlag == false)
				{
					g_aWave[g_nNumWave].bWaveFlag = true;//発動した瞬間にフラグを変えることにより、一回のみエネミーをセットする。
					g_aWave[g_nNumWave].bBonusCountFlag = true;//ウェーブ1のボーナスカウントを開始
					g_aWave[g_nNumWave - 1].bBonusCountFlag = false;//一つ前のウェーブが終わったので、一つ前のウェーブ（g_nNumWave-1）のスコアボーナスのカウントを止める。
					AddScore(g_aWave[g_nNumWave - 1].nBonusCountStart * 50);//ひとつ前のウェーブのクリア時間が早いほどスコアを加算する。
					SetScoreBonus(g_aWave[g_nNumWave - 1].nBonusCountStart * 50);//今回どれくらいスコアボーナスが付与されたかを表示する
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2 + 150.0f, 200.0f, 0.0f), ENEMYTYPE_REAFY, 40.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f));//敵の位置の初期化。右の数字は、敵のタイプ
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2 - 150.0f, 200.0f, 0.0f), ENEMYTYPE_PUNDY, 80.0f, D3DXVECTOR3((sinf(-D3DX_PI * 0.5f) * 9.0f), (cosf(-D3DX_PI * 0.5f) * 9.0f), 0.0f));//敵の位置の初期化。右の数字は、敵のタイプ
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2, +320.0f, 600.0f), ENEMYTYPE_SQUDY, 45.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f));//敵の位置の初期化。右の数字は、敵のタイプ

				}
				break;
			case 5:
				//WAVE1==============================
				if (g_aWave[g_nNumWave].bWaveFlag == false)
				{
					g_aWave[g_nNumWave].bWaveFlag = true;//発動した瞬間にフラグを変えることにより、一回のみエネミーをセットする。
					g_aWave[g_nNumWave].bBonusCountFlag = true;//ウェーブ1のボーナスカウントを開始
					g_aWave[g_nNumWave - 1].bBonusCountFlag = false;//一つ前のウェーブが終わったので、一つ前のウェーブ（g_nNumWave-1）のスコアボーナスのカウントを止める。
					AddScore(g_aWave[g_nNumWave - 1].nBonusCountStart * 50);//ひとつ前のウェーブのクリア時間が早いほどスコアを加算する。
					SetScoreBonus(g_aWave[g_nNumWave - 1].nBonusCountStart * 50);//今回どれくらいスコアボーナスが付与されたかを表示する
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2, 200.0f, 0.0f), ENEMYTYPE_HONOBY, 60.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f));//敵の位置の初期化。右の数字は、敵のタイプ
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2 + 300.0f, 200.0f, 0.0f), ENEMYTYPE_PUNDY, 40.0f, D3DXVECTOR3((sinf(-D3DX_PI * 0.6f) * 2.0f), (cosf(-D3DX_PI * 0.6f) * 2.0f), 0.0f));//敵の位置の初期化。右の数字は、敵のタイプ
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2 - 300.0f, 100.0f, 560.0f), ENEMYTYPE_REAFY, 40.0f, D3DXVECTOR3((sinf(-D3DX_PI * 1.1f) * 5.0f), (cosf(-D3DX_PI * 1.1f) * 5.0f), 0.0f));//敵の位置の初期化。右の数字は、敵のタイプ

				}
				break;
			case 6:
				//WAVE1==============================
				if (g_aWave[g_nNumWave].bWaveFlag == false)
				{
					g_aWave[g_nNumWave].bWaveFlag = true;//発動した瞬間にフラグを変えることにより、一回のみエネミーをセットする。
					g_aWave[g_nNumWave].bBonusCountFlag = true;//ウェーブ1のボーナスカウントを開始
					g_aWave[g_nNumWave - 1].bBonusCountFlag = false;//一つ前のウェーブが終わったので、一つ前のウェーブ（g_nNumWave-1）のスコアボーナスのカウントを止める。
					AddScore(g_aWave[g_nNumWave - 1].nBonusCountStart * 50);//ひとつ前のウェーブのクリア時間が早いほどスコアを加算する。
					SetScoreBonus(g_aWave[g_nNumWave - 1].nBonusCountStart * 50);//今回どれくらいスコアボーナスが付与されたかを表示する
					SetEnemy(D3DXVECTOR3(20.0f, 20.0f, 0.0f), ENEMYTYPE_HONOBY, 20.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f));//敵の位置の初期化。右の数字は、敵のタイプ
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH - 20.0f, 20.0f, 0.0f), ENEMYTYPE_HONOBY, 20.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f));//敵の位置の初期化。右の数字は、敵のタイプ
					SetEnemy(D3DXVECTOR3(20.0f, SCREEN_HEIGHT - 20.0f, 0.0f), ENEMYTYPE_HONOBY, 20.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f));//敵の位置の初期化。右の数字は、敵のタイプ
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH - 20.0f, SCREEN_HEIGHT - 20.0f, 0.0f), ENEMYTYPE_HONOBY, 20.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f));//敵の位置の初期化。右の数字は、敵のタイプ


				}
				break;
			case 7:
				//WAVE1==============================
				if (g_aWave[g_nNumWave].bWaveFlag == false)
				{
					g_aWave[g_nNumWave].bWaveFlag = true;//発動した瞬間にフラグを変えることにより、一回のみエネミーをセットする。
					g_aWave[g_nNumWave].bBonusCountFlag = true;//ウェーブ1のボーナスカウントを開始
					g_aWave[g_nNumWave - 1].bBonusCountFlag = false;//一つ前のウェーブが終わったので、一つ前のウェーブ（g_nNumWave-1）のスコアボーナスのカウントを止める。
					AddScore(g_aWave[g_nNumWave - 1].nBonusCountStart * 50);//ひとつ前のウェーブのクリア時間が早いほどスコアを加算する。
					SetScoreBonus(g_aWave[g_nNumWave - 1].nBonusCountStart * 50);//今回どれくらいスコアボーナスが付与されたかを表示する
					SetEnemy(D3DXVECTOR3(150.0f, 500.0f, 0.0f), ENEMYTYPE_DELBY, 50.0f, D3DXVECTOR3((sinf(-D3DX_PI * 0.5f) * 50.0f), (cosf(-D3DX_PI * 0.5f) * 50.0f), 0.0f));//敵の位置の初期化。右の数字は、敵のタイプ
					SetEnemy(D3DXVECTOR3(180.0f, 200.0f, 0.0f), ENEMYTYPE_SMILY, 40.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f));//敵の位置の初期化。右の数字は、敵のタイプ
					SetEnemy(D3DXVECTOR3(110.0f, 250.0f, 0.0f), ENEMYTYPE_SQUDY, 60.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f));//敵の位置の初期化。右の数字は、敵のタイプ
					SetEnemy(D3DXVECTOR3(1100.0f, 290.0f, 0.0f), ENEMYTYPE_PUNDY, 60.0f, D3DXVECTOR3((sinf(-D3DX_PI * 1.0f) * 5.0f), (cosf(-D3DX_PI * 1.0f) * 5.0f), 0.0f));//敵の位置の初期化。右の数字は、敵のタイプ
					SetEnemy(D3DXVECTOR3(1100.0f, 90.0f, 0.0f), ENEMYTYPE_BOMUBY, 1.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f));//敵の位置の初期化。右の数字は、敵のタイプ

				}
				break;
			case 8:
				//WAVE1==============================
				if (g_aWave[g_nNumWave].bWaveFlag == false)
				{
					g_aWave[g_nNumWave].bWaveFlag = true;//発動した瞬間にフラグを変えることにより、一回のみエネミーをセットする。
					g_aWave[g_nNumWave].bBonusCountFlag = true;//ウェーブ1のボーナスカウントを開始
					g_aWave[g_nNumWave - 1].bBonusCountFlag = false;//一つ前のウェーブが終わったので、一つ前のウェーブ（g_nNumWave-1）のスコアボーナスのカウントを止める。
					AddScore(g_aWave[g_nNumWave - 1].nBonusCountStart * 50);//ひとつ前のウェーブのクリア時間が早いほどスコアを加算する。
					SetScoreBonus(g_aWave[g_nNumWave - 1].nBonusCountStart * 50);//今回どれくらいスコアボーナスが付与されたかを表示する
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2, 150.0f, 0.0f), ENEMYTYPE_MIZUBY, 120.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f));//敵の位置の初期化。右の数字は、敵のタイプ
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2 - 400.0f, 40, 0.0f), ENEMYTYPE_DELBY, 80.0f, D3DXVECTOR3((sinf(-D3DX_PI * 0.5f) * 5.0f), (cosf(-D3DX_PI * 0.5f) * 5.0f), 0.0f));//敵の位置の初期化。右の数字は、敵のタイプ
					SetEnemy(D3DXVECTOR3(1000.0f, SCREEN_HEIGHT / 2 + 135.0f, 0.0f), ENEMYTYPE_REAFY, 60.0f, D3DXVECTOR3((sinf(-D3DX_PI * 0.52f) * 5.0f), (cosf(-D3DX_PI * 0.52f) * 5.0f), 0.0f));//敵の位置の初期化。右の数字は、敵のタイプ
					SetEnemy(D3DXVECTOR3(260.0f, 390.0f, 0.0f), ENEMYTYPE_HONOBY, 105.0f, D3DXVECTOR3((sinf(-D3DX_PI * 0.0f) * 5.0f), (cosf(-D3DX_PI * 0.0f) * 5.0f), 0.0f));//敵の位置の初期化。右の数字は、敵のタイプ

				}
				break;
			case 9:
				//WAVE1==============================
				if (g_aWave[g_nNumWave].bWaveFlag == false)
				{
					g_aWave[g_nNumWave].bWaveFlag = true;//発動した瞬間にフラグを変えることにより、一回のみエネミーをセットする。
					g_aWave[g_nNumWave].bBonusCountFlag = true;//ウェーブ1のボーナスカウントを開始
					g_aWave[g_nNumWave - 1].bBonusCountFlag = false;//一つ前のウェーブが終わったので、一つ前のウェーブ（g_nNumWave-1）のスコアボーナスのカウントを止める。
					AddScore(g_aWave[g_nNumWave - 1].nBonusCountStart * 50);//ひとつ前のウェーブのクリア時間が早いほどスコアを加算する。
					SetScoreBonus(g_aWave[g_nNumWave - 1].nBonusCountStart * 50);//今回どれくらいスコアボーナスが付与されたかを表示する
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 300.0f, 0.0f), ENEMYTYPE_WINBY, 175.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2 - 250.0f, SCREEN_HEIGHT / 2 - 190.0f, 0.0f), ENEMYTYPE_SQUDY, 95.0f, D3DXVECTOR3((sinf(-D3DX_PI * 0.5f) * 5.0f), (cosf(-D3DX_PI * 0.5f) * 5.0f), 0.0f));
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2 + 280.0f, SCREEN_HEIGHT / 2 - 120.0f, 0.0f), ENEMYTYPE_REAFY, 110.0f, D3DXVECTOR3((sinf(-D3DX_PI * 1.5f) * 5.0f), (cosf(-D3DX_PI * 1.5f) * 5.0f), 0.0f));

				}
				break;
			case 10:
				//WAVE1==============================
				if (g_aWave[g_nNumWave].bWaveFlag == false)
				{
					g_aWave[g_nNumWave].bWaveFlag = true;//発動した瞬間にフラグを変えることにより、一回のみエネミーをセットする。
					g_aWave[g_nNumWave].bBonusCountFlag = true;//ウェーブ1のボーナスカウントを開始
					g_aWave[g_nNumWave - 1].bBonusCountFlag = false;//一つ前のウェーブが終わったので、一つ前のウェーブ（g_nNumWave-1）のスコアボーナスのカウントを止める。
					AddScore(g_aWave[g_nNumWave - 1].nBonusCountStart * 50);//ひとつ前のウェーブのクリア時間が早いほどスコアを加算する。
					SetScoreBonus(g_aWave[g_nNumWave - 1].nBonusCountStart * 50);//今回どれくらいスコアボーナスが付与されたかを表示する
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2 + 450.0f, SCREEN_HEIGHT / 2 - 240.0f, 0.0f), ENEMYTYPE_MIZUBY, 120.0f, D3DXVECTOR3((sinf(-D3DX_PI * 0.5f) * 5.0f), (cosf(-D3DX_PI * 0.5f) * 5.0f), 0.0f));
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2 - 450.0f, SCREEN_HEIGHT / 2 - 240.0f, 0.0f), ENEMYTYPE_MIZUBY, 120.0f, D3DXVECTOR3((sinf(-D3DX_PI * 1.5f) * 5.0f), (cosf(-D3DX_PI * 1.5f) * 5.0f), 0.0f));
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 240.0f, 0.0f), ENEMYTYPE_MIZUBY, 120.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 240.0f, 0.0f), ENEMYTYPE_HONOBY, 120.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f));


				}
				break;
			case 11:
				//WAVE1==============================
				if (g_aWave[g_nNumWave].bWaveFlag == false)
				{
					g_aWave[g_nNumWave].bWaveFlag = true;//発動した瞬間にフラグを変えることにより、一回のみエネミーをセットする。
					g_aWave[g_nNumWave].bBonusCountFlag = true;//ウェーブ1のボーナスカウントを開始
					g_aWave[g_nNumWave - 1].bBonusCountFlag = false;//一つ前のウェーブが終わったので、一つ前のウェーブ（g_nNumWave-1）のスコアボーナスのカウントを止める。
					AddScore(g_aWave[g_nNumWave - 1].nBonusCountStart * 50);//ひとつ前のウェーブのクリア時間が早いほどスコアを加算する。
					SetScoreBonus(g_aWave[g_nNumWave - 1].nBonusCountStart * 50);//今回どれくらいスコアボーナスが付与されたかを表示する
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2 - 300.0f, SCREEN_HEIGHT / 2 - 270.0f, 0.0f), ENEMYTYPE_BOLBY, 150.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f));//敵の位置の初期化。右の数字は、敵のタイプ
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2 + 300.0f, SCREEN_HEIGHT / 2 - 270.0f, 0.0f), ENEMYTYPE_BOLBY, 150.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f));//敵の位置の初期化。右の数字は、敵のタイプ



				}
				break;
			case 12:
				//WAVE1==============================
				if (g_aWave[g_nNumWave].bWaveFlag == false)
				{
					g_aWave[g_nNumWave].bWaveFlag = true;//発動した瞬間にフラグを変えることにより、一回のみエネミーをセットする。
					g_aWave[g_nNumWave].bBonusCountFlag = true;//ウェーブ1のボーナスカウントを開始
					g_aWave[g_nNumWave - 1].bBonusCountFlag = false;//一つ前のウェーブが終わったので、一つ前のウェーブ（g_nNumWave-1）のスコアボーナスのカウントを止める。
					AddScore(g_aWave[g_nNumWave - 1].nBonusCountStart * 50);//ひとつ前のウェーブのクリア時間が早いほどスコアを加算する。
					SetScoreBonus(g_aWave[g_nNumWave - 1].nBonusCountStart * 50);//今回どれくらいスコアボーナスが付与されたかを表示する

					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH - 50.0f, SCREEN_HEIGHT / 2 - 300.0f, 0.0f), ENEMYTYPE_WINBY, 70.0f, D3DXVECTOR3((sinf(-D3DX_PI * 1.1f) * 15.0f), (cosf(-D3DX_PI * 1.1f) * 15.0f), 0.0f));//敵の位置の初期化。右の数字は、敵のタイプ
					//SetEnemy(D3DXVECTOR3(50.0f, SCREEN_HEIGHT / 2 + 300.0f, 0.0f), ENEMYTYPE_WINBY, 70.0f, D3DXVECTOR3((sinf(D3DX_PI * 1.1f) * 15.0f), (cosf(D3DX_PI * 1.1f) * 15.0f), 0.0f));//敵の位置の初期化。右の数字は、敵のタイプ
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), ENEMYTYPE_SQUDY, 70.0f, D3DXVECTOR3((sinf(-D3DX_PI * 1.0f) * 4.0f), (cosf(-D3DX_PI * 1.0f) * 4.0f), 0.0f));//敵の位置の初期化。右の数字は、敵のタイプ
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2 - 120.0f, SCREEN_HEIGHT / 2 - 200.0f, 0.0f), ENEMYTYPE_MIZUBY, 50.0f, D3DXVECTOR3((sinf(-D3DX_PI * 0.5f) * 4.0f), (cosf(-D3DX_PI * 0.5f) * 4.0f), 0.0f));//敵の位置の初期化。右の数字は、敵のタイプ
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2 - 150.0f, SCREEN_HEIGHT / 2 - 160.0f, 0.0f), ENEMYTYPE_REAFY, 50.0f, D3DXVECTOR3((sinf(-D3DX_PI * 0.55f) * 6.0f), (cosf(-D3DX_PI * 0.55f) * 7.0f), 0.0f));//敵の位置の初期化。右の数字は、敵のタイプ
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2 + 460.0f, SCREEN_HEIGHT / 2 + 30.0f, 0.0f), ENEMYTYPE_BOMUBY, 10.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f));//敵の位置の初期化。右の数字は、敵のタイプ




				}
				break;
			case 13:
				//WAVE1==============================
				if (g_aWave[g_nNumWave].bWaveFlag == false)
				{
					g_aWave[g_nNumWave].bWaveFlag = true;//発動した瞬間にフラグを変えることにより、一回のみエネミーをセットする。
					g_aWave[g_nNumWave].bBonusCountFlag = true;//ウェーブ1のボーナスカウントを開始
					g_aWave[g_nNumWave - 1].bBonusCountFlag = false;//一つ前のウェーブが終わったので、一つ前のウェーブ（g_nNumWave-1）のスコアボーナスのカウントを止める。
					AddScore(g_aWave[g_nNumWave - 1].nBonusCountStart * 50);//ひとつ前のウェーブのクリア時間が早いほどスコアを加算する。
					SetScoreBonus(g_aWave[g_nNumWave - 1].nBonusCountStart * 50);//今回どれくらいスコアボーナスが付与されたかを表示する
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 100.0f, 0.0f), ENEMYTYPE_BANDY, 180.0f, D3DXVECTOR3(0, 0, 0));//敵の位置の初期化。右の数字は、敵のタイプ
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 300.0f, 0.0f), ENEMYTYPE_WINBY, 70.0f, D3DXVECTOR3((sinf(-D3DX_PI * 1.5f) * 5.0f), (cosf(-D3DX_PI * 1.5f) * 5.0f), 0.0f));
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 300.0f, 0.0f), ENEMYTYPE_DELBY, 85.0f, D3DXVECTOR3((sinf(-D3DX_PI * 1.5f) * 6.0f), (cosf(-D3DX_PI * 1.5f) * 6.0f), 0.0f));
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 320.0f, 0.0f), ENEMYTYPE_SQUDY, 100.0f, D3DXVECTOR3((sinf(-D3DX_PI * 0.5f) * 7.0f), (cosf(-D3DX_PI * 0.5f) * 7.0f), 0.0f));



				}
				break;
			case 14:
				//WAVE1==============================
				if (g_aWave[g_nNumWave].bWaveFlag == false)
				{
					g_aWave[g_nNumWave].bWaveFlag = true;//発動した瞬間にフラグを変えることにより、一回のみエネミーをセットする。
					g_aWave[g_nNumWave].bBonusCountFlag = true;//ウェーブ1のボーナスカウントを開始
					g_aWave[g_nNumWave - 1].bBonusCountFlag = false;//一つ前のウェーブが終わったので、一つ前のウェーブ（g_nNumWave-1）のスコアボーナスのカウントを止める。
					AddScore(g_aWave[g_nNumWave - 1].nBonusCountStart * 50);//ひとつ前のウェーブのクリア時間が早いほどスコアを加算する。
					SetScoreBonus(g_aWave[g_nNumWave - 1].nBonusCountStart * 50);//今回どれくらいスコアボーナスが付与されたかを表示する
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2, 100.0f, 0.0f), ENEMYTYPE_BOSS1_STARWARRIOR, 5000.0f, D3DXVECTOR3((sinf(-D3DX_PI * 0.0f) * 0.0f), (cosf(-D3DX_PI * 0.0f) * 0.0f), 0.0f));


				}
				break;
			}

			if (g_nNumWave == 15)
			{
				
				Player* pPlayer = GetPlayer();
				int nNomissScore = 0;//ノーミスだった場合のスコア表示を行う。
				int nNotSkillUseBonus = 0;
				//WAVE1==============================
				if (g_aWave[g_nNumWave].bWaveFlag == false)
				{
					
					if (pPlayer->nHitCount == 0)
					{
						nNomissScore = 3000000;
						AddScore(3000000);
					}
					if (pPlayer->nSkillUseCount == 0)
					{//一度もスキルを使わずにクリアした場合・・・
						nNotSkillUseBonus = 2000000;
						AddScore(2000000);
					}
					g_aWave[g_nNumWave - 1].bBonusCountFlag = false;//一つ前のウェーブが終わったので、一つ前のウェーブ（g_nNumWave-1）のスコアボーナスのカウントを止める。
					AddScore(g_aWave[g_nNumWave - 1].nBonusCountStart * 50);//ひとつ前のウェーブのクリア時間が早いほどスコアを加算する。
					if (pGameCustom->nDifficulty == 1)
					{
						SetStaging(STAGINGTYPE_NORMAL_CLEARBONUS, -1, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
						SetScoreBonus(g_aWave[g_nNumWave - 1].nBonusCountStart * 50 + nNomissScore + nNotSkillUseBonus + 3000000);//今回どれくらいスコアボーナスが付与されたかを表示する
					}
					if (pGameCustom->nDifficulty == 2)
					{
						SetStaging(STAGINGTYPE_NORMAL_CLEARBONUS, -1, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
						AddScore(300000);
						SetScoreBonus(g_aWave[g_nNumWave - 1].nBonusCountStart * 50 + 300000 + nNomissScore + nNotSkillUseBonus + 3000000);//今回どれくらいスコアボーナスが付与されたかを表示する
					}
					if (pGameCustom->nDifficulty == 3)
					{
						SetStaging(STAGINGTYPE_HARD_CLEARBONUS, -1, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
						AddScore(500000);
						SetScoreBonus(g_aWave[g_nNumWave - 1].nBonusCountStart * 50 + 500000 + nNomissScore + nNotSkillUseBonus + 3000000);//今回どれくらいスコアボーナスが付与されたかを表示する
					}
					if (pGameCustom->nDifficulty == 4)
					{
						SetStaging(STAGINGTYPE_VERYHARD_CLEARBONUS, -1, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
						AddScore(1000000);
						SetScoreBonus(g_aWave[g_nNumWave - 1].nBonusCountStart * 50 + 1000000 + nNomissScore + nNotSkillUseBonus + 3000000);//今回どれくらいスコアボーナスが付与されたかを表示する
					}

				}
				
			}
		}
	}
	//===========================================================================================================================================================================
		/*if (pDefeat->DefeatEnemyCount <= -1)
		{
			pDefeat->DefeatEnemyCount = 0;
		}*/


		
	//if (pDefeat->DefeatEnemyCount < 0)
	// 
	//{
	//	pDefeat->DefeatEnemyCount = 0;
	//}
	
	
}

//弾の描画処理
void DrawWave(void)
{
	//LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//int nCntWave;

	////デバイスの取得
	//pDevice = GetDevice();

	////頂点バッファをデータストリームに設定
	//pDevice->SetStreamSource(0, g_pVtxBuffTime, 0, sizeof(VERTEX_2D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

	////頂点フォーマットの設定
	//pDevice->SetFVF(FVF_VERTEX_2D);



	//for (nCntWave = 0; nCntWave < MAX_TIME; nCntWave++)
	//{
	//	//if (g_aWave[nCntWave].bUse == true)
	//	//{//弾が使用されている

	//		//テクスチャの設定
	//	pDevice->SetTexture(0, g_pTextureTime);

	//	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,//0は、描画する最初の頂点インデックスで、2は、描画するプリミティブ数
	//		nCntWave * 4,
	//		2);
	//	//}

	//}


}

////スコアの設定処理
//void SetWave(int nWave)
//{
//
//	LPDIRECT3DDEVICE9 pDevice;
//	VERTEX_2D* pVtx;
//	//頂点バッファをロックし、頂点データへのポインタを取得
//	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);
//
//
//	int nGetWave;
//	//
//	//nWave = 184545;
//	g_nNumWave += nWave;
//
//
//
//	//デバイスの取得
//	pDevice = GetDevice();
//
//	nGetWave = g_nNumWave;//スコアを保存する
//
//	int nCnt;
//	int	nCntWave;
//	for (nCnt = 0; nCnt < MAX_TIME; nCnt++)
//	{
//		g_aTimePosTexU[nCnt] = nGetWave % 10;
//		nGetWave = nGetWave / 10;
//	}
//
//	for (nCntWave = 0; nCntWave < MAX_TIME; nCntWave++)
//	{
//		pVtx[0].tex = D3DXVECTOR2(0.0f + (0.1f * g_aTimePosTexU[nCntWave]), 0.0f);
//		pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f * g_aTimePosTexU[nCntWave]), 0.0f);
//		pVtx[2].tex = D3DXVECTOR2(0.0f + (0.1f * g_aTimePosTexU[nCntWave]), 1.0f);
//		pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * g_aTimePosTexU[nCntWave]), 1.0f);
//
//		pVtx += 4;//頂点データのポインタを４つ分進める。
//	}
//	//頂点バッファをアンロックする
//	g_pVtxBuffTime->Unlock();
//
//	/*for (nCntWave = 0; nCntWave < MAX_TIME; nCntWave++)
//	{
//		SetWave(10);
//	}*/
//}
//
//

bool GetWaveFlag(void)//多分使わない！！！
{
	return &g_bWaveFlag;//ウェーブのフラグがtrue,falseになったかどうかを取得する。
}
