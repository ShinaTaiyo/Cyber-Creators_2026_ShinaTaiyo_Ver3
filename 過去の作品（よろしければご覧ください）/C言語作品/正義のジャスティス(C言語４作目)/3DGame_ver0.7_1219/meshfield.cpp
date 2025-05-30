//========================================================
//
//９月２６日：ハッカソンの基盤作り[MeshField.cpp]
//Author:ShinaTaiyo
//
//=========================================================

#include "meshfield.h"
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"

//マクロ定義
#define MAX_MESHFIELD (2)//メッシュフィールド最大数
#define MAX_VERTEX (9)
#define MAX_DIVISION (3)

//グローバル変数
PDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshField = NULL;//頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshField = NULL;//インデックスバッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureMeshField = NULL;//テクスチャへのポインタ
D3DXVECTOR3 g_posMeshField;//メッシュフィールドの位置
D3DXVECTOR3 g_rotMeshField;//メッシュフィールドの向き
D3DXMATRIX g_mtxWorldMeshField;//メッシュフィールドのワールドマトリックス(float4*4)


#define MESHFIELDSCALE (100.0f)

//タイトル画面の初期化処理
void InitMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\ACT_Title.png",
		&g_pTextureMeshField);

	g_posMeshField = D3DXVECTOR3(0.0f, 5.0f, 0.0f);//位置の初期化
	g_rotMeshField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//向きの初期化

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 9,//頂点の数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshField,
		NULL);

	//インデックスバッファの生成
		//頂点バッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * 14,//インデックス数
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshField,
		NULL);


	////タイトル画面BGMの設定
	//PlaySound(SOUND_LABEL_BGM000);

	VERTEX_3D* pVtx;

	//===================
	//頂点情報を設定
	//===================

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffMeshField->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定（初期位置）
	pVtx[0].pos = D3DXVECTOR3(-MESHFIELDSCALE, 0.0f, MESHFIELDSCALE);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, MESHFIELDSCALE);
	pVtx[2].pos = D3DXVECTOR3(MESHFIELDSCALE, 0.0f,MESHFIELDSCALE);
	pVtx[3].pos = D3DXVECTOR3(-MESHFIELDSCALE, 0.0f,0.0f);
	pVtx[4].pos = D3DXVECTOR3(0.0f,100.0f, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(MESHFIELDSCALE, 0.0f, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(-MESHFIELDSCALE, 0.0f,-MESHFIELDSCALE);
	pVtx[7].pos = D3DXVECTOR3(0.0f, 0.0f, -MESHFIELDSCALE);
	pVtx[8].pos = D3DXVECTOR3(MESHFIELDSCALE, 0.0f, -MESHFIELDSCALE);

	//norの設定（初期設定）
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[4].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[5].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[6].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[7].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[8].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);



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

	//テクスチャ座標の設定（初期設定）
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[3].tex = D3DXVECTOR2(0.0f, 0.5f);
	pVtx[4].tex = D3DXVECTOR2(0.5f, 0.5f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.5f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(0.5f, 1.0f);
	pVtx[8].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffMeshField->Unlock();
	
	//==========================================================================

	//==============================
	//インデックス情報の設定
	//==============================

	//インデックスバッファのロック
	WORD* pIdx;
	g_pIdxBuffMeshField->Lock(0, 0, (void**)&pIdx, 0);

	int nCnt = 0;//インデックスの番号をカウントする用

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

				for (int nCntZ = 0; nCntZ < MAX_DIVISION - 1; nCntZ++)
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
					}

	//インデックスバッファのアンロック
	g_pIdxBuffMeshField->Unlock();

	//===================================================================

}

//==============================
//タイトル画面の終了処理
//==============================
void UninitMeshField(void)
{
	//テクスチャの破棄
	if (g_pTextureMeshField != NULL)
	{
		g_pTextureMeshField->Release();
		g_pTextureMeshField = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffMeshField != NULL)
	{
		g_pVtxBuffMeshField->Release();
		g_pVtxBuffMeshField = NULL;
	}

	//インデックスバッファの開放
	if (g_pIdxBuffMeshField != NULL)
	{
		g_pIdxBuffMeshField->Release();
		g_pIdxBuffMeshField = NULL;
	}
}
//============================
//タイトル画面の更新処理
//============================
void UpdateMeshField(void)
{

}



//================================
//タイトル画面の描画処理
//================================
void DrawMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

		//デバイスの取得
	pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;//計算用マトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldMeshField);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotMeshField.y, g_rotMeshField.x, g_rotMeshField.z);

	D3DXMatrixMultiply(&g_mtxWorldMeshField, &g_mtxWorldMeshField, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_posMeshField.x, g_posMeshField.y, g_posMeshField.z);
	D3DXMatrixMultiply(&g_mtxWorldMeshField, &g_mtxWorldMeshField, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldMeshField);


	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMeshField, 0, sizeof(VERTEX_3D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffMeshField);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureMeshField);

	//ポリゴンの描画
	//pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);//0は、描画する最初の頂点インデックスで、2は、描画するプリミティブ
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, 9, 0, 14);
}