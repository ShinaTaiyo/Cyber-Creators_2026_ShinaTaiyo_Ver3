//========================================================
//
//１１月７日：モデル読み込み[Block.cpp]
//Author:ShinaTaiyo
//
//=========================================================

#include "Block.h"
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "shadow.h"
#include "player.h"
//グローバル変数
LPD3DXMESH g_pMeshBlock[NUM_BLOCK] = {};//メッシュ（頂点情報）へのポインタ
LPD3DXBUFFER g_pBuffMatBlock = NULL;//マテリアルへのポインタ
DWORD g_dwNumMatBlock = 0;//マテリアルの数
D3DXVECTOR3 g_posBlock;//位置
D3DXVECTOR3 g_rotBlock;//向き
D3DXMATRIX g_mtxWorldBlock;//ワールドマトリックス
#define Block_SCALE (10.0f)

Block g_aBlock[MAX_BLOCK];//モデル情報の構造体

//int g_nldxShadow = -1;//対象の影のインデックス（番号）

//タイトル画面の初期化処理
void InitBlock(void)
{

	//g_nldxShadow = 0;//対象の影のインデックス

	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスへのポインタと取得

	g_posBlock = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotBlock = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//モデルの読み込み
	D3DXLoadMeshFromX("data\\Model\\Block_Red.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatBlock,
		NULL,
		&g_dwNumMatBlock,
		&g_pMeshBlock[BLOCKTYPE_RED]);

	for (int nCnt = 0; nCnt < MAX_BLOCK; nCnt++)
	{
		g_aBlock[nCnt].bUse = false;//使用しているかどうか
		g_aBlock[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//位置
		g_aBlock[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//向き
		g_aBlock[nCnt].nIdxShadow = -1;
	}




	SetBlock(D3DXVECTOR3(0.0f, 20.0f, 300.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), BLOCKTYPE_RED);
	/*SetBlock(D3DXVECTOR3(100.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), BlockTYPE_BILL);
	SetBlock(D3DXVECTOR3(10.0f, 0.0f, 50.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), BlockTYPE_BILL);
	SetBlock(D3DXVECTOR3(-100.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), BlockTYPE_BILL);*/





}

//==============================
//タイトル画面の終了処理
//==============================
void UninitBlock(void)
{
	for (int nCnt = 0; nCnt < NUM_BLOCK; nCnt++)
	{
		//メッシュの破棄
		if (g_pMeshBlock != NULL)
		{
			g_pMeshBlock[nCnt]->Release();
			g_pMeshBlock[nCnt] = NULL;
		}
	}
	//マテリアルの破棄
	if (g_pBuffMatBlock != NULL)
	{
		g_pBuffMatBlock->Release();
		g_pBuffMatBlock = NULL;
	}
}
//============================
//モデルの更新処理
//============================
void UpdateBlock(void)
{
	float fLength = 0.0f;
	for (int nCnt = 0; nCnt < MAX_BLOCK; nCnt++)
	{
		if (g_aBlock[nCnt].bUse == true)
		{
			Player* pPlayer = GetPlayer();

			fLength = sqrtf((g_aBlock[nCnt].pos.x - pPlayer->pos.x) * (g_aBlock[nCnt].pos.x - pPlayer->pos.x) +
				(g_aBlock[nCnt].pos.z - pPlayer->pos.z) * (g_aBlock[nCnt].pos.z - pPlayer->pos.z) +
				(g_aBlock[nCnt].pos.y - pPlayer->pos.y) * (g_aBlock[nCnt].pos.y - pPlayer->pos.y)) / 3;

		/*	if (fLength <= 20.0f)
			{
				pPlayer->pos = pPlayer->posOld;
			}*/

		/*	if (pPlayer->pos.x >= g_aBlock[nCnt].pos.x - 20.0f &&
				pPlayer->pos.x <= g_aBlock[nCnt].pos.x + 20.0f &&
				pPlayer->pos.y >= g_aBlock[nCnt].pos.y - 20.0f &&
				pPlayer->pos.y <= g_aBlock[nCnt].pos.y + 20.0f &&
				pPlayer->pos.z >= g_aBlock[nCnt].pos.z - 20.0f &&
				pPlayer->pos.z <= g_aBlock[nCnt].pos.z + 20.0f)
			{
				pPlayer->move.y = 0.0f;
				pPlayer->pos = pPlayer->posOld;
			}*/

			//影の位置を設定
	/*		SetPositionShadow(g_aBlock[nCnt].nIdxShadow, g_aBlock[nCnt].pos, -1);*/
		}
	}
}

//================================
//タイトル画面の描画処理
//================================
void DrawBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();;//デバイスへのポインタと取得


	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{
			D3DXMATRIX mtxRot, mtxTrans;//計算用マトリックス

			D3DMATERIAL9 matDef;//現在のマテリアル保存用

			D3DXMATERIAL* pMat;//マテリアルデータへのポインタ

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_mtxWorldBlock);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aBlock[nCntBlock].rot.y, g_aBlock[nCntBlock].rot.x, g_aBlock[nCntBlock].rot.z);

			D3DXMatrixMultiply(&g_mtxWorldBlock, &g_mtxWorldBlock, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aBlock[nCntBlock].pos.x, g_aBlock[nCntBlock].pos.y, g_aBlock[nCntBlock].pos.z);
			D3DXMatrixMultiply(&g_mtxWorldBlock, &g_mtxWorldBlock, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldBlock);

			//現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			//マテリアルへのポインタを取得
			pMat = (D3DXMATERIAL*)g_pBuffMatBlock->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_dwNumMatBlock; nCntMat++)
			{
				//マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//テクスチャの設定
				pDevice->SetTexture(0, NULL);

				//モデル（パーツ）の描画
				if (g_aBlock->nType == BLOCKTYPE_RED)
				{
					g_pMeshBlock[BLOCKTYPE_RED]->DrawSubset(nCntMat);
				}
			}

			//保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

void SetBlock(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == false)
		{
			g_aBlock[nCntBlock].bUse = true;//使用しているかどうか
			g_aBlock[nCntBlock].pos = pos;//位置
			g_aBlock[nCntBlock].rot = rot;//向き
			//g_aBlock[nCntBlock].nIdxShadow = SetShadow();//影のインデックスを設定
			break;
		}
	}
}

Block* GetBlock(void)
{
	return &g_aBlock[0];
}
