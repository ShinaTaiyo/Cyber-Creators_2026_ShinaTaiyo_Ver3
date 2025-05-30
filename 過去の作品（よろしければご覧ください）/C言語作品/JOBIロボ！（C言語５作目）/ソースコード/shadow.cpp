//========================================================
//
//９月２６日：ハッカソンの基盤作り[Shadow.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#include "shadow.h"
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "bullet.h"
#include "enemy.h"
#include "player.h"

//==============================
// グローバル変数
//==============================
PDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureShadow = NULL;//テクスチャへのポインタ
Shadow g_aShadow[MAX_SHADOW];
int g_nIdxShadow = -1;//影のインデックス

#define SHADOW_SCALE (30.0f)

//==============================
// 初期化処理
//==============================
void InitShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	g_nIdxShadow = -1;//影のインデックス


	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\shadow000.jpg",
		&g_pTextureShadow);

	for (int nCnt = 0; nCnt < MAX_SHADOW; nCnt++)
	{
		g_aShadow[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aShadow[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aShadow[nCnt].bUse = false;
		g_aShadow[nCnt].nNumBullet = -1;//読み込んだ弾の番号
		g_aShadow[nCnt].mtxWorld = {};//マトリックスワールド
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_SHADOW,//頂点の数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffShadow,
		NULL);

	VERTEX_3D* pVtx;

		//頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCnt = 0; nCnt < MAX_SHADOW; nCnt++)
	{

		//頂点座標の設定
		pVtx[0].pos = NULL_VECTOR3;
		pVtx[1].pos = NULL_VECTOR3;
		pVtx[2].pos = NULL_VECTOR3;
		pVtx[3].pos = NULL_VECTOR3;

		//norの設定（初期設定）
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。


		//テクスチャ座標の設定（初期設定）
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffShadow->Unlock();
}

//==============================
// 終了処理
//==============================
void UninitShadow(void)
{
	//テクスチャの破棄
	if (g_pTextureShadow != NULL)
	{
		g_pTextureShadow->Release();
		g_pTextureShadow = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffShadow != NULL)
	{
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}
}

//============================
// 更新処理
//============================
void UpdateShadow(void)
{

}

//================================
// 描画処理
//================================
void DrawShadow(void)
{
	D3DXMATRIX mtxRot, mtxTrans;//計算用マトリックス

	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ
		//デバイスの取得
	pDevice = GetDevice();


	//減算合成の設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{

		if (g_aShadow[nCntShadow].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aShadow[nCntShadow].mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aShadow[nCntShadow].rot.y, g_aShadow[nCntShadow].rot.x, g_aShadow[nCntShadow].rot.z);

			D3DXMatrixMultiply(&g_aShadow[nCntShadow].mtxWorld, &g_aShadow[nCntShadow].mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aShadow[nCntShadow].pos.x, g_aShadow[nCntShadow].pos.y, g_aShadow[nCntShadow].pos.z);
			D3DXMatrixMultiply(&g_aShadow[nCntShadow].mtxWorld, &g_aShadow[nCntShadow].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aShadow[nCntShadow].mtxWorld);


			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_3D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureShadow);

			//ポリゴンの描画
		    pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntShadow, 2);//0は、描画する最初の頂点インデックスで、2は、描画するプリミティブ
		}
	}
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

int SetShadow(void)
{
	int nCntShadow = -1;
	
	for (int nCnt = 0; nCnt < MAX_SHADOW; nCnt++)
	{
		if (g_aShadow[nCnt].bUse == false)
		{
			g_aShadow[nCnt].bUse = true;
			nCntShadow = nCnt;
			break;
		}
	}

	return nCntShadow;
}

void SetPositionShadow(int nldxShadow, D3DXVECTOR3 pos)
{
	VERTEX_3D* pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{

		if (g_aShadow[nldxShadow].bUse == true && nCntShadow == nldxShadow)
		{
			//頂点座標の設定（初期位置）
			pVtx[0].pos = D3DXVECTOR3(-SHADOW_SCALE,0.0f, SHADOW_SCALE);
			pVtx[1].pos = D3DXVECTOR3(SHADOW_SCALE, 0.0f, SHADOW_SCALE);
			pVtx[2].pos = D3DXVECTOR3(-SHADOW_SCALE,0.0f,-SHADOW_SCALE);
			pVtx[3].pos = D3DXVECTOR3(SHADOW_SCALE, 0.0f,-SHADOW_SCALE);
			g_aShadow[nldxShadow].pos = pos;

			g_aShadow[nldxShadow].pos.y = 1.0f;

			break;
		}

		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffShadow->Unlock();
}

Shadow* GetShadow(void)
{
	return &g_aShadow[0];
}

void KillShadow(int nIdxShadow)
{
	Player* pPlayer = GetPlayer();

	if (pPlayer->nIdxShadow != nIdxShadow)
	{
		g_aShadow[nIdxShadow].bUse = false;
	}
}

