//========================================================
//
//８月３０日：エフェクトもう一回見直す[score.cpp]
//Author:ShinaTaiyo
//
//=========================================================

#include "main.h"
#include "score.h"
#include "input.h"
#include "explosion.h"
#include "enemy.h"
#include "player.h"
#include "health.h"
#include "result.h"
#include "wave.h"
#include "scorebonus.h"

#define MAX_SCORE (8)//弾の最大数
#define NUM_SCORE (1)
#define SCORESCALEX (100)
#define SCORESCALEY (50)
#define SCORESCALE (100)

#define SCORE_RADIUS (20.0f)

//弾構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;//位置
	//D3DXVECTOR3 move;//移動量
	////int nLife;//寿命（どのくらい弾が飛んでいくか）
	bool bUse;//弾を使用しているかどうか
}Score;

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureScore= NULL;//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;//弾の情報
Score g_aScore[MAX_SCORE];
//float g_fXMove;//X座標の移動
//float g_fYMove;//Y座標の移動
D3DXVECTOR3 g_posScore;//位置
D3DXVECTOR3 g_moveScore;//移動量

//D3DXVECTOR3 g_rotPl;//向き
int g_nScore=0;
float g_fScoreScale=0;

int g_aPosTexU[8];//各桁の数字を格納

int g_nSaveScore=0;

int g_mode;

//弾の初期化処理
void InitScore(void)
{
	ResultType* pResultType = GetResultType();
		

	/*if (pResultType->nResultType == false)
	{*/
		g_nScore = 0;//リザルトモード以外なら、スコアを初期化する
	//}
	//g_moveScore = D3DXVECTOR3(0.0f, -10.0f, 0.0f);//移動量を初期化する

	LPDIRECT3DDEVICE9 pDevice;
	int	nCntScore;

	//デバイスの取得
	pDevice = GetDevice();

	        D3DXCreateTextureFromFile(pDevice,
			"data\\number_001.png",
			&g_pTextureScore);
	//テクスチャの読み込み（スコアのテクスチャを読み込む）
	for (nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{

			g_aPosTexU[nCntScore] = 0;
     }

	/*D3DXCreateTextureFromFile(pDevice,
		"data\\lightbrueball_enemy.png",
		&g_pTextureScore[0]);*/


	/*g_nScore = 0;*/

	//スコア情報の初期化
	for (nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{
		
		g_aScore[nCntScore].pos = D3DXVECTOR3(SCREEN_WIDTH-((nCntScore+1) * 50.0f),20.0f, 0.0f);
		g_aScore[nCntScore].bUse = false;//使用していない状態にする
		
	/*	g_aScore[nCntScore].type = BULLETTYPE_PLAYER;*/
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_SCORE,//４は必要な頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL);

	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aScore[nCntScore].pos.x - 20.0f, g_aScore[nCntScore].pos.y - 20.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aScore[nCntScore].pos.x + 20.0f, g_aScore[nCntScore].pos.y - 20.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aScore[nCntScore].pos.x - 20.0f, g_aScore[nCntScore].pos.y + 20.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aScore[nCntScore].pos.x + 20.0f, g_aScore[nCntScore].pos.y + 20.0f, 0.0f);

		g_aScore[nCntScore].bUse = true;//使用している状態にする

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
		pVtx += 4;//頂点データのポインタを４つ分進める。
	}
	//頂点バッファをアンロックする
	g_pVtxBuffScore->Unlock();	
}

//弾の終了処理
void UninitScore(void)
{
	int nCntScore;
	//テクスチャの破棄
	//for (nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	//{
		if (g_pTextureScore != NULL)
		{
			g_pTextureScore->Release();
			g_pTextureScore = NULL;
		}
	//}
	//頂点バッファの破棄
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}
}

//弾の更新処理
void UpdateScore(void)
{
	int nCntScore;


	VERTEX_2D* pVtx;
	Player* pPlayer = GetPlayer();
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{
	


		//頂点カラーの設定
		if (pPlayer->pos.x >= g_aScore[nCntScore].pos.x - SCORE_RADIUS && pPlayer->pos.x <= g_aScore[nCntScore].pos.x + SCORE_RADIUS
			&& pPlayer->pos.y >=g_aScore[nCntScore].pos.y - SCORE_RADIUS&& pPlayer->pos.y <= g_aScore[nCntScore].pos.y + SCORE_RADIUS)
		{
			//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
		}
		else
		{

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
		}
		
	

		pVtx += 4;//頂点データのポインタを４つ分進める。
	}

	//頂点バッファをアンロックする
	g_pVtxBuffScore->Unlock();
}

//弾の描画処理
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	int nCntScore;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);



	for (nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{
		//if (g_aScore[nCntScore].bUse == true)
		//{//弾が使用されている
           
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureScore);

			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,//0は、描画する最初の頂点インデックスで、2は、描画するプリミティブ数
				nCntScore * 4,
				2);
		//}

	}


}

//スコアの設定処理
void SetScore(int nScore)
{

	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D* pVtx;
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	int nGetScore;
	bool bGetWaveFlag;
	bGetWaveFlag = GetWaveFlag();


	
	//nScore = 184545;1

	g_nScore +=nScore;
	
	

	//デバイスの取得
	pDevice = GetDevice();

	nGetScore = g_nScore;//スコアを保存する

	int nCnt;
	int	nCntScore;
	for (nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		g_aPosTexU[nCnt] = nGetScore % 10;
		nGetScore = nGetScore / 10;
	}

	for (nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{
		pVtx[0].tex = D3DXVECTOR2(0.0f + (0.1f * g_aPosTexU[nCntScore]), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f * g_aPosTexU[nCntScore]), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f+(0.1f * g_aPosTexU[nCntScore]), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f+ (0.1f * g_aPosTexU[nCntScore]), 1.0f);

		pVtx += 4;//頂点データのポインタを４つ分進める。
	}
	//頂点バッファをアンロックする
	g_pVtxBuffScore->Unlock();

	/*for (nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{
		SetScore(10);
	}*/
}

//=====================================
//スコア加算処理
//=====================================
void AddScore(int nValue)
{
	SetScore(nValue);
}

//===================================
//スコアの取得
//===================================
int GetScore(void)
{
	return g_nScore;
}