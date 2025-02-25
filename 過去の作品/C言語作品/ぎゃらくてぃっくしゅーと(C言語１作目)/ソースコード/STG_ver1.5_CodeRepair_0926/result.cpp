//========================================================
//
//８月３０日：エフェクトもう一回見直す[result.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#include "result.h"
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "ranking.h"
#include "Score.h"

#define MAX_SCORERESULT (8)
#define MAX_SCORERESULTSIZE (50.0f)
//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureResult[MAX_RESULT_TEX] = {};//テクスチャへのポインタ
PDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;//頂点バッファへのポインタ

LPDIRECT3DTEXTURE9 g_pTextureScoreResult = {};//テクスチャへのポインタ
PDIRECT3DVERTEXBUFFER9 g_pVtxBuffScoreResult = NULL;//頂点バッファへのポインタ


typedef struct
{
	D3DXVECTOR3 pos;//位置
	//D3DXVECTOR3 move;//移動量
	////int nLife;//寿命（どのくらい弾が飛んでいくか）
	bool bUse;//弾を使用しているかどうか
}ScoreResult;



ResultType g_aResultType;

ScoreResult g_aScoreResult[MAX_SCORERESULT];

bool g_bColorChangeResult;
int g_nColorChangeValue;

int g_nPosTexU[MAX_SCORERESULT];

//リザルト画面の初期化処理
void InitResult(void)
{
	int nCntResult;

	g_bColorChangeResult = false;

	g_nColorChangeValue = 0;

	//ランキングの設定
	

	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\Finish!_000.png",
		&g_pTextureResult[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\Gameover_Result.png",
		&g_pTextureResult[1]);



	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,//頂点の数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResult,
		NULL);

	PlaySound(SOUND_LABEL_BGM002);

	VERTEX_2D* pVtx;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);
		
			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
			//rhwの設定（初期設定）
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;
			//頂点カラーの設定（初期設定）
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			//テクスチャ座標の設定（初期設定）
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	//頂点バッファをアンロックする
	g_pVtxBuffResult->Unlock();

	D3DXCreateTextureFromFile(pDevice,
		"data\\number_001.png",
		&g_pTextureScoreResult);



	//==============================================================
	//最終スコアの初期化処理
	//==============================================================
	int nCntScoreResult;

	//テクスチャの読み込み（スコアのテクスチャを読み込む）
	for (nCntScoreResult = 0; nCntScoreResult < MAX_SCORERESULT; nCntScoreResult++)
	{

		g_nPosTexU[nCntScoreResult] = 0;
	}
		//スコア情報の初期化
	for (nCntScoreResult = 0; nCntScoreResult < MAX_SCORERESULT; nCntScoreResult++)
	{
		g_aScoreResult[nCntScoreResult].pos = D3DXVECTOR3(SCREEN_WIDTH/2+300.0f - ((nCntScoreResult + 1) * 80.0f), SCREEN_HEIGHT/2+300.0f, 0.0f);
		g_aScoreResult[nCntScoreResult].bUse = false;//使用していない状態にする
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_SCORERESULT,//４は必要な頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScoreResult,
		NULL);

	

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffScoreResult->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScoreResult = 0; nCntScoreResult < MAX_SCORERESULT; nCntScoreResult++)
	{
		pVtx[0].pos = D3DXVECTOR3(g_aScoreResult[nCntScoreResult].pos.x - MAX_SCORERESULTSIZE, g_aScoreResult[nCntScoreResult].pos.y -MAX_SCORERESULTSIZE, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aScoreResult[nCntScoreResult].pos.x + MAX_SCORERESULTSIZE, g_aScoreResult[nCntScoreResult].pos.y -MAX_SCORERESULTSIZE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aScoreResult[nCntScoreResult].pos.x - MAX_SCORERESULTSIZE, g_aScoreResult[nCntScoreResult].pos.y +MAX_SCORERESULTSIZE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aScoreResult[nCntScoreResult].pos.x + MAX_SCORERESULTSIZE, g_aScoreResult[nCntScoreResult].pos.y +MAX_SCORERESULTSIZE, 0.0f);
		g_aScoreResult[nCntScoreResult].bUse = true;//使用している状態にする
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
		//テクスチャの設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
	

		pVtx += 4;//頂点データのポインタを４つ分進める。
	}
	//頂点バッファをアンロックする
	g_pVtxBuffScoreResult->Unlock();
}

//==============================
//リザルト画面の終了処理
//==============================
void UninitResult(void)
{
	int nCnt;
	//テクスチャの破棄
	for (nCnt = 0; nCnt < MAX_RESULT_TEX; nCnt++)
	{
		if (g_pTextureResult[nCnt] != NULL)
		{
			g_pTextureResult[nCnt]->Release();
			g_pTextureResult[nCnt] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffResult != NULL)
	{
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}

	if (g_pTextureScoreResult != NULL)
	{
		g_pTextureScoreResult->Release();
		g_pTextureScoreResult = NULL;
	}
	//}
	//頂点バッファの破棄
	if (g_pVtxBuffScoreResult != NULL)
	{
		g_pVtxBuffScoreResult->Release();
		g_pVtxBuffScoreResult = NULL;
	}
}

//===================================
//リザルト画面の更新処理
//===================================
void UpdateResult(void)
{
	int nCntResult;
	VERTEX_2D* pVtx;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);
	//for (nCntResult = 0; nCntResult < MAX_RESULT_TEX; nCntResult++)
	//{
	//	//頂点座標の設定（初期位置）

		if (g_aResultType.nResultType == RESULTTYPE_CLEAR)
		{//ゲームをクリアできていたら呼ばれる。


			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
		}
		else if (g_aResultType.nResultType == RESULTTYPE_GAMEOVER)
		{//ゲームオーバーだったら呼ばれる。
			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
		}
		pVtx += 4;//頂点座標のポインタをインクリメント
	/*}*/
	g_pVtxBuffResult->Unlock();

	if (GetKeyboardTrigger(DIK_RETURN) == true||GetJoyPadTrigger(JOYKEY_START)==true)
	{//決定キー（ENTERキー）が押された
		//モードの設定（ゲーム画面に移行）
		PlaySound(SOUND_LABEL_SE_DECISION_000);
		StopSound(SOUND_LABEL_BGM002);
		SetFade(MODE_RANKING);
		//g_bColorChangeResult = true;

	}
//==========================================================================================================================

	//=================================
	//最終スコア表示処理
	//=================================

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffScoreResult->Lock(0, 0, (void**)&pVtx, 0);


	int nGetScore;
	//
	//nScore = 184545;
	/*g_nScore += nScore;*/

	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	nGetScore = GetScore();//スコアを取得する。

	int nCnt;
	int	nCntScore;
	for (nCnt = 0; nCnt < MAX_SCORERESULT; nCnt++)
	{
		g_nPosTexU[nCnt] = nGetScore % 10;
		nGetScore = nGetScore / 10;
	}

	for (nCntScore = 0; nCntScore < MAX_SCORERESULT; nCntScore++)
	{
		pVtx[0].tex = D3DXVECTOR2(0.0f + (0.1f * g_nPosTexU[nCntScore]), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f * g_nPosTexU[nCntScore]), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (0.1f * g_nPosTexU[nCntScore]), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * g_nPosTexU[nCntScore]), 1.0f);

		pVtx += 4;//頂点データのポインタを４つ分進める。
	}
	//頂点バッファをアンロックする
	g_pVtxBuffScoreResult->Unlock();

	/*for (nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{
		SetScore(10);
	}*/


}

//================================
//タイトル画面の描画処理
//================================
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	int nCntResult;
		//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffResult, 0, sizeof(VERTEX_2D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャの設定
		if (g_aResultType.nResultType == RESULTTYPE_CLEAR)
		{
			pDevice->SetTexture(0, g_pTextureResult[RESULTTYPE_CLEAR]);
		}
		if (g_aResultType.nResultType == RESULTTYPE_GAMEOVER)
		{
			pDevice->SetTexture(0, g_pTextureResult[RESULTTYPE_GAMEOVER]);
		}
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,0, 2);//0は、描画する最初の頂点インデックスで、2は、描画するプリミティブ数
	

		//==========================================
		//最終スコア表示処理の描画
		//==========================================
		//デバイスの取得
		pDevice = GetDevice();

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffScoreResult, 0, sizeof(VERTEX_2D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);


		int nCntScore;
		for (nCntScore = 0; nCntScore < MAX_SCORERESULT; nCntScore++)
		{
			
				//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureScoreResult);

			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,//0は、描画する最初の頂点インデックスで、2は、描画するプリミティブ数
				nCntScore * 4,
				2);
			

		}
}

//================================
//リザルト画面分け構造体の取得
//================================

ResultType* GetResultType(void)
{
	return &g_aResultType;//リザルトタイプの情報を返す。
}