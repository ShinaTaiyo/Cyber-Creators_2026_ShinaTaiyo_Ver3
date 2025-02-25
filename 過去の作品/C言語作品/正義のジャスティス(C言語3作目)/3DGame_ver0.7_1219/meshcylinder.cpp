//========================================================
//
//９月２６日：ハッカソンの基盤作り[MeshCylinder.cpp]
//Author:ShinaTaiyo
//
//=========================================================

#include "MeshCylinder.h"
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"

//マクロ定義
#define MAX_MESHCYLINDER (2)//メッシュフィールド最大数
#define MAX_VERTEX (9)
#define MAX_DIVISION (3)

//グローバル変数
PDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshCylinder = NULL;//頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshCylinder = NULL;//インデックスバッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureMeshCylinder = NULL;//テクスチャへのポインタ
D3DXVECTOR3 g_posMeshCylinder;//メッシュフィールドの位置
D3DXVECTOR3 g_rotMeshCylinder;//メッシュフィールドの向き
D3DXMATRIX g_mtxWorldMeshCylinder;//メッシュフィールドのワールドマトリックス(float4*4)


#define MESHCYLINDERSCALE (100.0f)

//タイトル画面の初期化処理
void InitMeshCylinder(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\ACT_Title.png",
		&g_pTextureMeshCylinder);

	g_posMeshCylinder = D3DXVECTOR3(0.0f, 0.0f, 100.0f);//位置の初期化
	g_rotMeshCylinder = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//向きの初期化

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 16,//頂点の数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshCylinder,
		NULL);

	//インデックスバッファの生成
		//頂点バッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * 18,//インデックス数
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshCylinder,
		NULL);


	////タイトル画面BGMの設定
	//PlaySound(SOUND_LABEL_BGM000);

	VERTEX_3D* pVtx;

	//===================
	//頂点情報を設定
	//===================

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffMeshCylinder->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定（初期位置）
	pVtx[0].pos = D3DXVECTOR3(sinf(-D3DX_PI * 1.0f) * MESHCYLINDERSCALE,100.0f, cosf(-D3DX_PI * 1.0f) * MESHCYLINDERSCALE);
	pVtx[1].pos = D3DXVECTOR3(sinf(-D3DX_PI * 1.25f) * MESHCYLINDERSCALE, 100.0f, cosf(-D3DX_PI * 1.25f) * MESHCYLINDERSCALE);
	pVtx[2].pos = D3DXVECTOR3(sinf(-D3DX_PI * 1.5f) * MESHCYLINDERSCALE, 100.0f, cosf(-D3DX_PI * 1.5f) * MESHCYLINDERSCALE);
	pVtx[3].pos = D3DXVECTOR3(sinf(-D3DX_PI * 1.75f) * MESHCYLINDERSCALE, 100.0f, cosf(-D3DX_PI * 1.75f) * MESHCYLINDERSCALE);
	pVtx[4].pos = D3DXVECTOR3(sinf(-D3DX_PI * 0.0f) * MESHCYLINDERSCALE, 100.0f, cosf(-D3DX_PI * 0.0f) * MESHCYLINDERSCALE);
	pVtx[5].pos = D3DXVECTOR3(sinf(-D3DX_PI * 0.25f) * MESHCYLINDERSCALE, 100.0f, cosf(-D3DX_PI * 0.25f) * MESHCYLINDERSCALE);
	pVtx[6].pos = D3DXVECTOR3(sinf(-D3DX_PI * 0.5f) * MESHCYLINDERSCALE, 100.0f, cosf(-D3DX_PI * 0.5f) * MESHCYLINDERSCALE);
	pVtx[7].pos = D3DXVECTOR3(sinf(-D3DX_PI * 0.75f) * MESHCYLINDERSCALE, 100.0f, cosf(-D3DX_PI * 0.75f) * MESHCYLINDERSCALE);
	pVtx[8].pos = D3DXVECTOR3(sinf(-D3DX_PI * 1.0f) * MESHCYLINDERSCALE,0.0f, cosf(-D3DX_PI * 1.0f) * MESHCYLINDERSCALE);
	pVtx[9].pos = D3DXVECTOR3(sinf(-D3DX_PI * 1.25f) * MESHCYLINDERSCALE,0.0f, cosf(-D3DX_PI * 1.25f) * MESHCYLINDERSCALE);
	pVtx[10].pos = D3DXVECTOR3(sinf(-D3DX_PI * 1.5f) * MESHCYLINDERSCALE,0.0f, cosf(-D3DX_PI * 1.5f) * MESHCYLINDERSCALE);
	pVtx[11].pos = D3DXVECTOR3(sinf(-D3DX_PI * 1.75f) * MESHCYLINDERSCALE,0.0f, cosf(-D3DX_PI * 1.75f) * MESHCYLINDERSCALE);
	pVtx[12].pos = D3DXVECTOR3(sinf(-D3DX_PI * 0.0f) * MESHCYLINDERSCALE,0.0f, cosf(-D3DX_PI * 0.0f) * MESHCYLINDERSCALE);
	pVtx[13].pos = D3DXVECTOR3(sinf(-D3DX_PI * 0.25f) * MESHCYLINDERSCALE,0.0f, cosf(-D3DX_PI * 0.25f) * MESHCYLINDERSCALE);
	pVtx[14].pos = D3DXVECTOR3(sinf(-D3DX_PI * 0.5f) * MESHCYLINDERSCALE,0.0f, cosf(-D3DX_PI * 0.5f) * MESHCYLINDERSCALE);
	pVtx[15].pos = D3DXVECTOR3(sinf(-D3DX_PI * 0.75f) * MESHCYLINDERSCALE,0.0f, cosf(-D3DX_PI * 0.75f) * MESHCYLINDERSCALE);


	//D3DXVec3Normalize = (0.0f, 0.0f, -1.0f);
	//norの設定（初期設定）
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[4].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[5].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[6].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[7].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[8].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[9].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[10].nor = D3DXVECTOR3(0.0f,0.0f, -1.0f);
	pVtx[11].nor = D3DXVECTOR3(0.0f,0.0f, -1.0f);
	pVtx[12].nor = D3DXVECTOR3(0.0f,0.0f, -1.0f);
	pVtx[13].nor = D3DXVECTOR3(0.0f,0.0f, -1.0f);
	pVtx[14].nor = D3DXVECTOR3(0.0f,0.0f, -1.0f);
	pVtx[15].nor = D3DXVECTOR3(0.0f,0.0f, -1.0f);


	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[9].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[10].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[11].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[12].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[13].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[14].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[15].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定（初期設定）
	pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.125f,0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.25f,0.0f);
	pVtx[3].tex = D3DXVECTOR2(0.375f,0.0f);
	pVtx[4].tex = D3DXVECTOR2(0.5f,0.0f);
	pVtx[5].tex = D3DXVECTOR2(0.625f,0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.75f,0.0f);
	pVtx[7].tex = D3DXVECTOR2(0.875f,0.0f);
	pVtx[8].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[9].tex = D3DXVECTOR2(0.125f,1.0f);
	pVtx[10].tex = D3DXVECTOR2(0.25f,1.0f);
	pVtx[11].tex = D3DXVECTOR2(0.375f,1.0f);
	pVtx[12].tex = D3DXVECTOR2(0.5f, 1.0f);
	pVtx[13].tex = D3DXVECTOR2(0.625f,1.0f);
	pVtx[14].tex = D3DXVECTOR2(0.75f,1.0f);
	pVtx[15].tex = D3DXVECTOR2(0.875f,1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffMeshCylinder->Unlock();

	//==========================================================================

	//==============================
	//インデックス情報の設定
	//==============================

	//インデックスバッファのロック
	WORD* pIdx;
	g_pIdxBuffMeshCylinder->Lock(0, 0, (void**)&pIdx, 0);

	int nCnt = 0;//インデックスの番号をカウントする用



		//テクスチャ座標の設定（初期設定）(一旦保留）
	pIdx[0] = 0;
	pIdx[1] = 1;
	pIdx[2] = 2;
	pIdx[3] = 3;
	pIdx[4] = 4;
	pIdx[5] = 5;
	pIdx[6] = 6;
	pIdx[7] = 7;
	pIdx[8] = 0;
	pIdx[9] = 8;
	pIdx[10] = 9;
	pIdx[11] = 10;
	pIdx[12] = 11;
	pIdx[13] = 12;
	pIdx[14] = 13;
	pIdx[15] = 14;
	pIdx[16] = 15;
	pIdx[17] = 8;

	//インデックスの設定（１４個）

	/*		pIdx[0] = 3;
			pIdx[1] = 0;
			pIdx[2] = 4;
			pIdx[3] = 1;
			pIdx[4] = 5;
			pIdx[5] = 2;
			pIdx[6] = 2;
			pIdx[7] = 6;
			pIdx[8] = 6;
			pIdx[9] = 3;
			pIdx[10] = 7;
			pIdx[11] = 4;
			pIdx[12] = 8;
			pIdx[13] = 5;*/

			//インデックスの設定
				/*	for (int nCntZ = 0; nCntZ < 2; nCntZ++)
					{
						for (int nCntX = 0; nCntX < 3; nCntX++)
						{
							pIdx[nCnt] = (3 + (3 * nCntZ) + nCntX);
							nCnt++;
							pIdx[nCnt] = (3 * nCntZ) + nCntX;
							nCnt++;
						}
						if (nCntZ != 1)
						{
							pIdx[nCnt] = pIdx[nCnt - 1];
							nCnt++;
							pIdx[nCnt] = 3 * (nCntZ + 2);
							nCnt++;
						}
					}*/

	/*for (int nCntZ = 0; nCntZ < MAX_DIVISION - 1; nCntZ++)
	{
		for (int nCntX = 0; nCntX < MAX_DIVISION; nCntX++)
		{
			pIdx[nCnt] = (MAX_DIVISION + (MAX_DIVISION * nCntZ) + nCntX);
			nCnt++;
			pIdx[nCnt] = (MAX_DIVISION * nCntZ) + nCntX;
			nCnt++;
		}
		if (nCntZ != 1)
		{
			pIdx[nCnt] = pIdx[nCnt - 1];
			nCnt++;
			pIdx[nCnt] = MAX_DIVISION * (nCntZ + 2);
			nCnt++;
		}
	}*/

	//インデックスバッファのアンロック
	g_pIdxBuffMeshCylinder->Unlock();

	//===================================================================

}

//==============================
//タイトル画面の終了処理
//==============================
void UninitMeshCylinder(void)
{
	//テクスチャの破棄
	if (g_pTextureMeshCylinder != NULL)
	{
		g_pTextureMeshCylinder->Release();
		g_pTextureMeshCylinder = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffMeshCylinder != NULL)
	{
		g_pVtxBuffMeshCylinder->Release();
		g_pVtxBuffMeshCylinder = NULL;
	}

	//インデックスバッファの開放
	if (g_pIdxBuffMeshCylinder != NULL)
	{
		g_pIdxBuffMeshCylinder->Release();
		g_pIdxBuffMeshCylinder = NULL;
	}
}
//============================
//タイトル画面の更新処理
//============================
void UpdateMeshCylinder(void)
{

}



//================================
//タイトル画面の描画処理
//================================
void DrawMeshCylinder(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

		//デバイスの取得
	pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;//計算用マトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldMeshCylinder);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotMeshCylinder.y, g_rotMeshCylinder.x, g_rotMeshCylinder.z);

	D3DXMatrixMultiply(&g_mtxWorldMeshCylinder, &g_mtxWorldMeshCylinder, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_posMeshCylinder.x, g_posMeshCylinder.y, g_posMeshCylinder.z);
	D3DXMatrixMultiply(&g_mtxWorldMeshCylinder, &g_mtxWorldMeshCylinder, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldMeshCylinder);


	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMeshCylinder, 0, sizeof(VERTEX_3D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffMeshCylinder);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureMeshCylinder);

	//ポリゴンの描画
	//pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);//0は、描画する最初の頂点インデックスで、2は、描画するプリミティブ
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, 0, 0, 16, 0, 18);
}