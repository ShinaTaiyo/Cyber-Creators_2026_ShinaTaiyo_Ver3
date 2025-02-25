//========================================================
//
//８月３０日：エフェクトもう一回見直す[effect.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#include "main.h"
#include "bullet.h"
#include "input.h"
#include "explosion.h"
#include "enemy.h"
#include "player.h"
#include "health.h"
#include <time.h>
#include "effect.h"

#define MAX_EFFECT (20000)//エフェクトの最大数
#define NUM_EFFECT (1)//エフェクトの種類

//エフェクト構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXCOLOR col;//色
	float fRadius;//半径（大きさ）
	int nLife;//寿命（表示時間）
	bool bUse;//使用しているかどうか
	int nScaleType;//エフェクトの大きさを変えるスピードを決める。
}Effect;

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureEffect[NUM_EFFECT] = {};//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;//弾の情報
Effect g_aEffect[MAX_EFFECT];//エフェクトの情報

//エフェクトの初期化処理
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int	nCntEffect;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み（弾のテクスチャを読み込む）
	D3DXCreateTextureFromFile(pDevice,
		"data\\effect000.jpg",
		&g_pTextureEffect[0]);

	//エフェクト情報の初期化
	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].nLife = 0;
		g_aEffect[nCntEffect].bUse = false;//使用していない状態にする
		g_aEffect[nCntEffect].fRadius = 0.0f;
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EFFECT,//４は必要な頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL);

	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		//頂点座標の設定(エフェクトの大きさ）
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(20.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 20.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(20.0f, 20.0f, 0.0f);

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

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;//頂点データのポインタを４つ分進める。
	}
	//頂点バッファをアンロックする
	g_pVtxBuffEffect->Unlock();
}

//エフェクトの終了処理
void UninitEffect(void)
{
	//テクスチャの破棄
	int nCnt;
	for (nCnt = 0; nCnt < NUM_EFFECT; nCnt++)
	{
		if (g_pTextureEffect[nCnt] != NULL)
		{
			g_pTextureEffect[nCnt]->Release();
			g_pTextureEffect[nCnt] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}

//エフェクトの更新処理
void UpdateEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D* pVtx;

	//デバイスの取得
	pDevice = GetDevice();
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	int nCntEffect;
	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{//エフェクトが使用される
			//頂点カラーの設定
			//g_aEffect[nCntEffect].fRadius -= 3.0f;
			g_aEffect[nCntEffect].nLife--;

			pVtx[0].col = g_aEffect[nCntEffect].col;
			pVtx[1].col = g_aEffect[nCntEffect].col;
			pVtx[2].col = g_aEffect[nCntEffect].col;
			pVtx[3].col = g_aEffect[nCntEffect].col;

			if (g_aEffect[nCntEffect].nScaleType == EFFECTSCALE_NORMAL)
			{
				g_aEffect[nCntEffect].fRadius -= 0.5f;//半径の値を減らし続けて、エフェクトを小さくする。
				pVtx[0].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fRadius, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fRadius, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRadius, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRadius, 0.0f);
			}
			if (g_aEffect[nCntEffect].nScaleType == EFFECTSCALE_FAST)
			{
				g_aEffect[nCntEffect].fRadius -= 3.0f;
				pVtx[0].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fRadius, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fRadius, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRadius, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRadius, 0.0f);
			}



			if (g_aEffect[nCntEffect].nLife <= 0)
			{//エフェクトの寿命が尽きた
				g_aEffect[nCntEffect].bUse = false;
			}
			if (g_aEffect[nCntEffect].fRadius <= 0.0f)
			{
				g_aEffect[nCntEffect].bUse = 0;
			}
		}
		pVtx += 4;//ポインタを次の値へ
	}

	//頂点バッファをアンロックする
	g_pVtxBuffEffect->Unlock();	
}

//弾の描画処理
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	int nCntEffect;

	Enemy* pEnemy = GetEnemy();
	Player* pPlayer = GetPlayer();

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_2D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//aブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);

	//テクスチャの設定
	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{
		
				pDevice->SetTexture(0, g_pTextureEffect[0]);
		
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,//0は、描画する最初の頂点インデックスで、2は、描画するプリミティブ数
				nCntEffect * 4,
				2);
		}

	}
	//aブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//エフェクトの設定処理
void SetEffect(D3DXVECTOR3 pos,D3DXCOLOR col,float fRadius,int nLife,int nType)
{
	int nCntEffect;

	VERTEX_2D* pVtx;
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == false)
		{//弾が使用されていない
		/*	if (BULLETTYPE_ENEMY || BULLETTYPE_PLAYER || BULLETTYPE_SKILL1||BULLETTYPE_REAFY)
			{*/

			g_aEffect[nCntEffect].pos = pos;
			g_aEffect[nCntEffect].nScaleType = nType;
			g_aEffect[nCntEffect].col = col;
			g_aEffect[nCntEffect].fRadius = fRadius;//エフェクトの半径を設定
			g_aEffect[nCntEffect].nLife = nLife;
			g_aEffect[nCntEffect].bUse = true;//使用している状態にする

						//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRadius, 0.0f);

			pVtx[0].col = g_aEffect[nCntEffect].col;//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
			pVtx[1].col = g_aEffect[nCntEffect].col;//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
			pVtx[2].col = g_aEffect[nCntEffect].col;//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
			pVtx[3].col = g_aEffect[nCntEffect].col;//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
			break;
			
		}
		pVtx += 4;//ポインタを４つ分進める
	}
	g_pVtxBuffEffect->Unlock();
}