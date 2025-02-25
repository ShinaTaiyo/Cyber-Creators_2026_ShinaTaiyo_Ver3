//========================================================
//
//９月２４：ウェーブクリア時のスコアボーナスがどのくらい入ったかを表す役割
//Author:ShinaTaiyo
//
//=========================================================

#include "main.h"
#include "ScoreBonus.h"
#include "input.h"
#include "explosion.h"
#include "enemy.h"
#include "player.h"
#include "health.h"
#include "result.h"
#include "score.h"

#define MAX_SCOREBONUS (8)//弾の最大数
#define NUM_SCOREBONUS (1)
#define SCOREBONUS_SCALE (100)

#define ScoreBonus_RADIUS (20.0f)

//弾構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;//位置
	int nLife;//寿命（どのくらい弾が飛んでいくか）
	bool bUse;//スコアを使用しているかどうか
	int nPosTexU;//各桁の数字を格納

}ScoreBonus;

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureScoreBonus = NULL;//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScoreBonus = NULL;//弾の情報
ScoreBonus g_aScoreBonus[MAX_SCOREBONUS];
//float g_fXMove;//X座標の移動
//float g_fYMove;//Y座標の移動


//D3DXVECTOR3 g_rotPl;//向き
int g_nScoreBonus = 0;
float g_fSCOREBONUS_SCALE = 0;


int g_nSaveScoreBonus = 0;



//弾の初期化処理
void InitScoreBonus(void)
{
	ResultType* pResultType = GetResultType();


	/*if (pResultType->nResultType == false)
	{*/
	g_nScoreBonus = 0;//リザルトモード以外なら、スコアを初期化する
//}
//g_moveScoreBonus = D3DXVECTOR3(0.0f, -10.0f, 0.0f);//移動量を初期化する

	LPDIRECT3DDEVICE9 pDevice;
	int	nCntScoreBonus;

	//デバイスの取得
	pDevice = GetDevice();

	D3DXCreateTextureFromFile(pDevice,
		"data\\number_001.png",
		&g_pTextureScoreBonus);

		//スコア情報の初期化
	for (nCntScoreBonus = 0; nCntScoreBonus < MAX_SCOREBONUS; nCntScoreBonus++)
	{
		g_aScoreBonus[nCntScoreBonus].nPosTexU = 0;
		g_aScoreBonus[nCntScoreBonus].nLife = 0;
		g_aScoreBonus[nCntScoreBonus].pos = D3DXVECTOR3(SCREEN_WIDTH - ((nCntScoreBonus + 1) * 50.0f), 50.0f, 0.0f);
		g_aScoreBonus[nCntScoreBonus].bUse = false;//使用していない状態にする
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_SCOREBONUS,//４は必要な頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScoreBonus,
		NULL);

	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffScoreBonus->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScoreBonus = 0; nCntScoreBonus < MAX_SCOREBONUS; nCntScoreBonus++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);

		g_aScoreBonus[nCntScoreBonus].bUse = true;//使用している状態にする

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
	g_pVtxBuffScoreBonus->Unlock();
}

//弾の終了処理
void UninitScoreBonus(void)
{
	int nCntScoreBonus;
	//テクスチャの破棄
	//for (nCntScoreBonus = 0; nCntScoreBonus < MAX_SCOREBONUS; nCntScoreBonus++)
	//{
	if (g_pTextureScoreBonus != NULL)
	{
		g_pTextureScoreBonus->Release();
		g_pTextureScoreBonus = NULL;
	}
	//}
	//頂点バッファの破棄
	if (g_pVtxBuffScoreBonus != NULL)
	{
		g_pVtxBuffScoreBonus->Release();
		g_pVtxBuffScoreBonus = NULL;
	}
}

//弾の更新処理
void UpdateScoreBonus(void)
{
	int nCntScoreBouns;
	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffScoreBonus->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScoreBouns = 0; nCntScoreBouns < MAX_SCOREBONUS; nCntScoreBouns++)
	{
		if (g_aScoreBonus[nCntScoreBouns].bUse == true)
		{

			g_aScoreBonus[nCntScoreBouns].nLife -= 1;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aScoreBonus[nCntScoreBouns].pos.x - 20.0f, g_aScoreBonus[nCntScoreBouns].pos.y - 20.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aScoreBonus[nCntScoreBouns].pos.x + 20.0f, g_aScoreBonus[nCntScoreBouns].pos.y - 20.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aScoreBonus[nCntScoreBouns].pos.x - 20.0f, g_aScoreBonus[nCntScoreBouns].pos.y + 20.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aScoreBonus[nCntScoreBouns].pos.x + 20.0f, g_aScoreBonus[nCntScoreBouns].pos.y + 20.0f, 0.0f);
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

			if (g_aScoreBonus[nCntScoreBouns].nLife < 1)
			{
				g_aScoreBonus[nCntScoreBouns].bUse = false;
			}
		}
			pVtx += 4;//頂点データのポインタを４つ分進める。
	}
	//頂点バッファをアンロックする
	g_pVtxBuffScoreBonus->Unlock();
}

//弾の描画処理
void DrawScoreBonus(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	int nCntScoreBonus;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffScoreBonus, 0, sizeof(VERTEX_2D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);



	for (nCntScoreBonus = 0; nCntScoreBonus < MAX_SCOREBONUS; nCntScoreBonus++)
	{
		if (g_aScoreBonus[nCntScoreBonus].bUse == true)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureScoreBonus);

			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,//0は、描画する最初の頂点インデックスで、2は、描画するプリミティブ数
				nCntScoreBonus * 4,
				2);
		}

	}


}

//スコアの設定処理
void SetScoreBonus(int nScoreBonus)
{

	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D* pVtx;
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffScoreBonus->Lock(0, 0, (void**)&pVtx, 0);


	int nGetScoreBonus;
	//
	//nScoreBonus = 184545;
	g_nScoreBonus = nScoreBonus;



	//デバイスの取得
	pDevice = GetDevice();

	nGetScoreBonus = g_nScoreBonus;//スコアを保存する

	int nCnt;
	int	nCntScoreBonus;
	for (nCnt = 0; nCnt < MAX_SCOREBONUS; nCnt++)
	{
		if (g_aScoreBonus[nCnt].bUse == false)
		{
			g_aScoreBonus[nCnt].nLife = 90;
			g_aScoreBonus[nCnt].bUse = true;//使用していない状態にする
			g_aScoreBonus[nCnt].nPosTexU = nGetScoreBonus % 10;//その位置のテクスチャのスコアの値を決める。
			nGetScoreBonus = nGetScoreBonus / 10;
		}
	}

	for (nCntScoreBonus = 0; nCntScoreBonus < MAX_SCOREBONUS; nCntScoreBonus++)
	{
		pVtx[0].tex = D3DXVECTOR2(0.0f + (0.1f * g_aScoreBonus[nCntScoreBonus].nPosTexU), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f * g_aScoreBonus[nCntScoreBonus].nPosTexU), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (0.1f * g_aScoreBonus[nCntScoreBonus].nPosTexU), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * g_aScoreBonus[nCntScoreBonus].nPosTexU), 1.0f);

		pVtx += 4;//頂点データのポインタを４つ分進める。
	}
	//頂点バッファをアンロックする
	g_pVtxBuffScoreBonus->Unlock();
}