//========================================================
//
//スコアの処理
//Author:ShinaTaiyo
//
//=========================================================
#include "main.h"
#include "score.h"
#include "input.h"
#include "player.h"
#include "result.h"

//=========================================================
//グローバル変数
//=========================================================
LPDIRECT3DTEXTURE9 g_pTextureScore= NULL;//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;//弾の情報
Score g_aScore[MAX_SCORE];
D3DXVECTOR3 g_posScore;//位置
D3DXVECTOR3 g_moveScore;//移動量
int g_nScore=0;
float g_fScoreScale=0;
int g_aPosTexU[MAX_SCORE];//各桁の数字を格納
int g_nSaveScore=0;
int g_mode;

//=========================================================
//初期化処理
//=========================================================
void InitScore(void)
{
	g_nScore = 0;
	LPDIRECT3DDEVICE9 pDevice;
	int	nCntScore;

	//デバイスの取得
	pDevice = GetDevice();

    D3DXCreateTextureFromFile(pDevice,
	"data\\TEXTURE\\timer000.png",
	&g_pTextureScore);

	//テクスチャの読み込み（スコアのテクスチャを読み込む）
	for (nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{
		g_aPosTexU[nCntScore] = 0;
    }

	//スコア情報の初期化
	for (nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{
		g_aScore[nCntScore].pos = D3DXVECTOR3(SCORE_POSX - nCntScore * 40.0f, SCORE_POSY, 0.0f);
		g_aScore[nCntScore].bUse = false;//使用していない状態にする
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_SCORE,
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
		pVtx[0].pos = D3DXVECTOR3(g_aScore[nCntScore].pos.x - 25.0f, g_aScore[nCntScore].pos.y - 25.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aScore[nCntScore].pos.x + 25.0f, g_aScore[nCntScore].pos.y - 25.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aScore[nCntScore].pos.x - 25.0f, g_aScore[nCntScore].pos.y + 25.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aScore[nCntScore].pos.x + 25.0f, g_aScore[nCntScore].pos.y + 25.0f, 0.0f);

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

//=========================================================
//終了処理
//=========================================================
void UninitScore(void)
{
	//テクスチャの破棄
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}
}

//=========================================================
//更新処理
//=========================================================
void UpdateScore(void)
{

}

//=========================================================
//描画処理
//=========================================================
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
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureScore);

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntScore * 4, 2);
	}
}

//=========================================================
//設定処理
//=========================================================
void SetScore(int nScore)
{
	Player* pPlayer = GetPlayer();

	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();

	VERTEX_2D* pVtx;

	int nGetScore = 0;
	g_nScore += (int)((nScore * pPlayer->fCombo));
	nGetScore = g_nScore;//スコアを保存する
	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		g_aPosTexU[nCnt] = nGetScore % 10;
		nGetScore = nGetScore / 10;
	}

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{
		pVtx[0].tex = D3DXVECTOR2(0.0f + (0.1f * g_aPosTexU[nCntScore]), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f * g_aPosTexU[nCntScore]), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (0.1f * g_aPosTexU[nCntScore]), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * g_aPosTexU[nCntScore]), 1.0f);

		pVtx += 4;//頂点データのポインタを４つ分進める。
	}
	//頂点バッファをアンロックする
	g_pVtxBuffScore->Unlock();
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