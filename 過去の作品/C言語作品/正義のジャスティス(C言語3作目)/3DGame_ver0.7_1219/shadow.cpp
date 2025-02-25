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
//グローバル変数
PDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureShadow = NULL;//テクスチャへのポインタ
D3DXVECTOR3 g_posShadow;//位置
D3DXVECTOR3 g_rotShadow;//向き
D3DXMATRIX g_mtxWorldShadow;//ワールドマトリックス(float4*4)


Shadow g_aShadow[MAX_SHADOW];

int g_nIdxShadow = -1;//影のインデックス

#define SHADOW_SCALE (30.0f)

//タイトル画面の初期化処理
void InitShadow(void)
{


	//bFadeFlag = false;

	//g_nCntTime = 0;

	//g_nColorChange = 255;//カラーの初期化（白色）

	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\shadow000.jpg",
		&g_pTextureShadow);

	g_posShadow = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//位置の初期化
	g_rotShadow = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//向きの初期化

	for (int nCnt = 0; nCnt < MAX_SHADOW; nCnt++)
	{
		g_aShadow[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aShadow[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aShadow[nCnt].bUse = false;
		g_aShadow[nCnt].nNumBullet = -1;//読み込んだ弾の番号
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_SHADOW,//頂点の数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffShadow,
		NULL);


	////タイトル画面BGMの設定
	//PlaySound(SOUND_LABEL_BGM000);

	VERTEX_3D* pVtx;

		//頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCnt = 0; nCnt < MAX_SHADOW; nCnt++)
	{

		//頂点座標の設定（初期位置）
		pVtx[0].pos = D3DXVECTOR3(-SHADOW_SCALE, 1.0f, SHADOW_SCALE);
		pVtx[1].pos = D3DXVECTOR3(SHADOW_SCALE, 1.0f, SHADOW_SCALE);
		pVtx[2].pos = D3DXVECTOR3(-SHADOW_SCALE, 1.0f, -SHADOW_SCALE);
		pVtx[3].pos = D3DXVECTOR3(SHADOW_SCALE, 1.0f, -SHADOW_SCALE);

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
//タイトル画面の終了処理
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
//タイトル画面の更新処理
//============================
void UpdateShadow(void)
{
	VERTEX_3D* pVtx;

	Bullet* pBullet = GetBullet();
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	
	//頂点バッファをアンロックする
	g_pVtxBuffShadow->Unlock();

}



//================================
//タイトル画面の描画処理
//================================
void DrawShadow(void)
{
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

			D3DXMATRIX mtxRot, mtxTrans;//計算用マトリックス

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_mtxWorldShadow);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aShadow[nCntShadow].rot.y, g_aShadow[nCntShadow].rot.x, g_aShadow[nCntShadow].rot.z);

			D3DXMatrixMultiply(&g_mtxWorldShadow, &g_mtxWorldShadow, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aShadow[nCntShadow].pos.x, g_aShadow[nCntShadow].pos.y, g_aShadow[nCntShadow].pos.z);
			D3DXMatrixMultiply(&g_mtxWorldShadow, &g_mtxWorldShadow, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldShadow);


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
		//nCntShadow++;
		if (g_aShadow[nCnt].bUse == false)
		{
			g_aShadow[nCnt].bUse = true;
			nCntShadow = nCnt;
			break;
		}
	}


	return nCntShadow;
}

void SetPositionShadow(int nldxShadow, D3DXVECTOR3 pos, int nNumBullet, int nNumEnemy, D3DXCOLOR col)
{
	VERTEX_3D* pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	if (nldxShadow >= 0 && nldxShadow < MAX_SHADOW)
	{
		if (g_aShadow[nldxShadow].bUse == true)
		{
			//頂点座標の設定（初期位置）
			pVtx[0].pos = D3DXVECTOR3(-SHADOW_SCALE, 1.0f, SHADOW_SCALE);
			pVtx[1].pos = D3DXVECTOR3(SHADOW_SCALE, 1.0f, SHADOW_SCALE);
			pVtx[2].pos = D3DXVECTOR3(-SHADOW_SCALE, 1.0f, -SHADOW_SCALE);
			pVtx[3].pos = D3DXVECTOR3(SHADOW_SCALE, 1.0f, -SHADOW_SCALE);
			g_aShadow[nldxShadow].pos = pos;

			g_aShadow[nldxShadow].pos.y = 0.0f;

			g_aShadow[nldxShadow].nNumBullet = nNumBullet;//弾の番号


			//頂点カラーの設定
			pVtx[0].col = col;//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
			pVtx[1].col = col;//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
			pVtx[2].col = col;//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
			pVtx[3].col = col;//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。

			/*Bullet* pBullet = GetBullet();
			for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++,pBullet++)
			{
				
				if (pBullet->bUse == false)
				{
					if (pBullet->nIdxShadow == nldxShadow)
					{
						g_aShadow[nldxShadow].bUse = false;
					}
				}
			}*/

			/*Enemy* pEnemy = GetEnemy();
			for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++, pEnemy++)
			{

				if (pEnemy->bUse == false)
				{
					if (pEnemy->nIdxShadow == nldxShadow)
					{
						g_aShadow[nldxShadow].bUse = false;
					}
				}
			}*/

		}
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
	g_aShadow[nIdxShadow].bUse = false;
}

