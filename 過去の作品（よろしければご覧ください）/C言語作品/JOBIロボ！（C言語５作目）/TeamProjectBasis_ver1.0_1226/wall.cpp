//=======================================================================================================================================================================================================================
//
// 壁の処理
// Author : Atsumu Kuboichi
//
//=======================================================================================================================================================================================================================
#include <stdio.h>
#include <string.h>
#include "wall.h"
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "field.h"
#include "game.h"

//=======================================================================================================================================================================================================================
// グローバル変数
//=======================================================================================================================================================================================================================
PDIRECT3DVERTEXBUFFER9 g_pVtxBuffWall = NULL;		//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureWall[NUM_WALL] = {};	//テクスチャへのポインタ
D3DXMATRIX g_mtxWorldWall;							//ワールドマトリックス
Wall g_aWall[MAX_WALL];								//壁の情報

//=======================================================================================================================================================================================================================
// 初期化処理
//=======================================================================================================================================================================================================================
void InitWall(void)
{
	VERTEX_3D* pVtx;						//頂点情報へのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスの取得

	//テクスチャの読み込み
	for (int nCntTex = 0; nCntTex < NUM_WALL; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
			WALL_FILENAME[nCntTex],
			&g_pTextureWall[nCntTex]);
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_WALL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffWall,
		NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		//変数
		g_aWall[nCntWall].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aWall[nCntWall].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aWall[nCntWall].fWidth = 0.0f;
		g_aWall[nCntWall].fHeight = 0.0f;
		g_aWall[nCntWall].bUse = true;

		//頂点座標の設定（初期位置）
		pVtx[0].pos = D3DXVECTOR3(0.0f,0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//norの設定（初期設定）
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定（初期設定）
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffWall->Unlock();
}

//=======================================================================================================================================================================================================================
// 終了処理
//=======================================================================================================================================================================================================================
void UninitWall(void)
{
	//テクスチャの破棄
	for (int nCntTex = 0; nCntTex < NUM_WALL; nCntTex++)
	{
		if (g_pTextureWall[nCntTex] != NULL)
		{//テクスチャ情報が存在する場合
			g_pTextureWall[nCntTex]->Release();
			g_pTextureWall[nCntTex] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffWall != NULL)
	{
		g_pVtxBuffWall->Release();
		g_pVtxBuffWall = NULL;
	}
}

//=======================================================================================================================================================================================================================
// 更新処理
//=======================================================================================================================================================================================================================
void UpdateWall(void)
{
	VERTEX_3D* pVtx;			//頂点情報へのポインタ
	Field* pField = GetField();	//フィールドの情報の構造体を取得

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	//位置
	g_aWall[0].pos = D3DXVECTOR3(pField->pos.x, pField->pos.y, pField->pos.z + pField->fHeight);
	g_aWall[1].pos = D3DXVECTOR3(pField->pos.x + pField->fWidth, pField->pos.y, pField->pos.z);
	g_aWall[2].pos = D3DXVECTOR3(pField->pos.x, pField->pos.y, pField->pos.z - pField->fHeight);
	g_aWall[3].pos = D3DXVECTOR3(pField->pos.x - pField->fWidth, pField->pos.y, pField->pos.z);

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		//向き
		g_aWall[nCntWall].rot = D3DXVECTOR3(0.0f, nCntWall * (D3DX_PI / 2.0f), 0.0f);

		if (g_aWall[nCntWall].bUse == true)
		{//使用されている場合
			if (!GetEditMode())
			{//ゲーム中
				if (GetMode() != MODE_TUTORIAL)
				{//チュートリアル画面以外の場合
					g_aWall[nCntWall].nType = 1;
				}
				else
				{//チュートリアル画面の場合
					g_aWall[nCntWall].nType = 0;
				}
			}
			else
			{//エディタ中
				if (!GetHouse())
				{//チュートリアル画面の場合
					g_aWall[nCntWall].nType = 1;
				}
				else
				{//ゲーム画面の場合
					g_aWall[nCntWall].nType = 0;
				}
			}

			//頂点座標の設定
			if (nCntWall % 2 == 0)
			{//前後の壁
				pVtx[0].pos = D3DXVECTOR3(-pField->fWidth, pField->fHeight, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(pField->fWidth, pField->fHeight, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(-pField->fWidth, 0.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(pField->fWidth, 0.0f, 0.0f);
			}
			else
			{//左右の壁
				pVtx[0].pos = D3DXVECTOR3(-pField->fHeight, pField->fHeight, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(pField->fHeight, pField->fHeight, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(-pField->fHeight, 0.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(pField->fHeight, 0.0f, 0.0f);
			}
		}

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffWall->Unlock();
}

//=======================================================================================================================================================================================================================
// 描画処理
//=======================================================================================================================================================================================================================
void DrawWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;			//計算用マトリックス

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		if (g_aWall[nCntWall].bUse == true)
		{//使用されている場合
	        //ワールドマトリックスの初期化
	        D3DXMatrixIdentity(&g_mtxWorldWall);
	        
	        //向きを反映
	        D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aWall[nCntWall].rot.y, g_aWall[nCntWall].rot.x, g_aWall[nCntWall].rot.z);
	        
	        D3DXMatrixMultiply(&g_mtxWorldWall, &g_mtxWorldWall, &mtxRot);
	        
	        //位置を反映
	        D3DXMatrixTranslation(&mtxTrans, g_aWall[nCntWall].pos.x, g_aWall[nCntWall].pos.y, g_aWall[nCntWall].pos.z);
	        D3DXMatrixMultiply(&g_mtxWorldWall, &g_mtxWorldWall, &mtxTrans);
	        
	        //ワールドマトリックスの設定
	        pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldWall);
	        
	        //頂点バッファをデータストリームに設定
	        pDevice->SetStreamSource(0, g_pVtxBuffWall, 0, sizeof(VERTEX_3D));
	        
	        //頂点フォーマットの設定
	        pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureWall[g_aWall[nCntWall].nType]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntWall * 4, 2);
		}
	}	
}

//=======================================================================================================================================================================================================================
// 情報の構造体を取得
//=======================================================================================================================================================================================================================
Wall* GetWall(void)
{
	return &g_aWall[0];
}
