//========================================================
//
//１１月７日：モデル読み込み[Item.cpp]
//Author:ShinaTaiyo
//
//=========================================================

#include "Item.h"
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "shadow.h"
#include "player.h"
#include <stdio.h>
#include <string.h>
#include "model.h"
#include "score.h"
#include "bullet.h"
#include "combo.h"
#include "SkillGage.h"
#include "particle.h"
//グローバル変数
int g_nNumItem = 0;//読み込むモデルの最大数を決める
#define Item_SCALE (10.0f)
//#define SAVEItem "data\\TEXTFILE\\SaveItem.txt"

#define MAX_ITEMTEXTURE (32)
int g_nDelayItem = 0;

typedef struct
{
	LPD3DXMESH pMeshItem;//メッシュ（頂点情報）へのポインタ（モデルの種類数分）
	LPD3DXBUFFER pBuffMatItem;//マテリアルへのポインタ
	DWORD dwNumMatItem;//マテリアルの数
	LPDIRECT3DTEXTURE9 apTextureItem[MAX_ITEMTEXTURE];//テクスチャへのポインタ
}ItemMat;

D3DXVECTOR3 g_posItem;//位置
D3DXVECTOR3 g_rotItem;//向き
D3DXMATRIX g_mtxWorldItem;//ワールドマトリックス

Item g_aItem[MAX_ITEM];//モデル情報の構造体

ItemMat g_aItemMat[NUM_ITEM];//モデルのマテリアルや頂点の構造体

#define MAX_DELAY (50)//全体的に攻撃速度を下げる

int g_nldxShadowItem = -1;//対象の影のインデックス（番号）
//タイトル画面の初期化処理
void InitItem(void)
{

	//for (int nCntItem = 0; nCntItem < NUM_ITEM; nCntItem++)
	//{
	//	g_dwNumMatItem[nCntItem] = 0;//マテリアルの数
	//}
	//g_nldxShadow = 0;//対象の影のインデックス

	g_nNumItem = 2;

	g_nDelayItem = 0;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスへのポインタと取得

	g_posItem = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotItem = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//g_bIsLanding = false;//地面にいるかどうか



	

	


	for (int nCntMatItem = 0; nCntMatItem < NUM_ITEM; nCntMatItem++)
	{
		g_aItemMat[nCntMatItem].dwNumMatItem = {};
		g_aItemMat[nCntMatItem].pBuffMatItem = {};
		g_aItemMat[nCntMatItem].pMeshItem = {};
		for (int nCntTexture = 0; nCntTexture < MAX_ITEMTEXTURE; nCntTexture++)
		{
			g_aItemMat[nCntMatItem].apTextureItem[nCntTexture] = {};//一つのモデルに関連するテクスチャの初期化
		}
	}


	for (int nCnt = 0; nCnt < MAX_ITEM; nCnt++)
	{
		g_aItem[nCnt].bUse = false;//使用しているかどうか
		g_aItem[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//位置
		g_aItem[nCnt].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//1F前の位置

		g_aItem[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//移動量
		g_aItem[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//向き
		g_aItem[nCnt].nIdxShadow = -1;
		g_aItem[nCnt].nType = -1;
		g_aItem[nCnt].vtxMinItem = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//それぞれの座標のモデルの位置の最小値
		g_aItem[nCnt].vtxMaxItem = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//それぞれの座標のモデルの位置の最大値
		g_aItem[nCnt].nNumVtx = 0;//頂点数
		g_aItem[nCnt].sizeFVF;//頂点フォーマットのサイズ
		g_aItem[nCnt].pVtxBuff;//頂点バッファへのポインタ
		g_aItem[nCnt].Size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//敵の大きさ
		g_aItem[nCnt].bIsLanding = false;//地面についているかどうか
	}

	//モデルの読み込み
	D3DXLoadMeshFromX("data\\MODEL\\Item\\Item00_HealHart.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_aItemMat[ITEM00_HEALHART].pBuffMatItem,
		NULL,
		&g_aItemMat[ITEM00_HEALHART].dwNumMatItem,
		&g_aItemMat[ITEM00_HEALHART].pMeshItem);

	//モデルの読み込み
	D3DXLoadMeshFromX("data\\MODEL\\Item\\Item01_EnergyBoost.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_aItemMat[ITEM01_ENERGYBOOST].pBuffMatItem,
		NULL,
		&g_aItemMat[ITEM01_ENERGYBOOST].dwNumMatItem,
		&g_aItemMat[ITEM01_ENERGYBOOST].pMeshItem);


	//モデルの読み込み
	D3DXLoadMeshFromX("data\\MODEL\\Item\\Item02_PowerFullSword.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_aItemMat[ITEM02_POWFULSWORD].pBuffMatItem,
		NULL,
		&g_aItemMat[ITEM02_POWFULSWORD].dwNumMatItem,
		&g_aItemMat[ITEM02_POWFULSWORD].pMeshItem);


	//モデルの読み込み
	D3DXLoadMeshFromX("data\\MODEL\\Item\\Item03_CapsuleBarrier.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_aItemMat[ITEM03_CAPSULEBARRIER].pBuffMatItem,
		NULL,
		&g_aItemMat[ITEM03_CAPSULEBARRIER].dwNumMatItem,
		&g_aItemMat[ITEM03_CAPSULEBARRIER].pMeshItem);


	

	//for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	//{//全てのモデルに割り当てる
	//	for (int nCntMat = 0; nCntMat < g_nNumItem; nCntMat++)
	//	{//種類を割り当てるfor文
	//		if (g_aItem[nCntItem].nType == nCntMat)
	//		{//モデルの種類が一致

	//			//頂点数の取得
	//			g_aItem[nCntItem].nNumVtx = g_aItemMat[nCntMat].pMeshItem->GetNumVertices();

	//			//頂点フォーマットのサイズを取得
	//			g_aItem[nCntItem].sizeFVF = D3DXGetFVFVertexSize(g_aItemMat[nCntMat].pMeshItem->GetFVF());

	//			//頂点バッファのロック
	//			g_aItemMat[nCntMat].pMeshItem->LockVertexBuffer(D3DLOCK_READONLY, (void**)&g_aItem[nCntItem].pVtxBuff);


	//			for (int nCntVtx = 0; nCntVtx < g_aItem[nCntItem].nNumVtx; nCntVtx++)
	//			{
	//				//頂点座標の代入
	//				D3DXVECTOR3 vtx = *(D3DXVECTOR3*)g_aItem[nCntItem].pVtxBuff;

	//				//====================================================
	//				//頂点座標を比較してモデルの最小値最大値を取得
	//				//====================================================
	//				if (vtx.x > g_aItem[nCntItem].vtxMaxItem.x)
	//				{
	//					g_aItem[nCntItem].vtxMaxItem.x = vtx.x;
	//				}
	//				else if (vtx.x < g_aItem[nCntItem].vtxMinItem.x)
	//				{//今回読み込んだ頂点が、一番小さい頂点より小さい場合
	//					g_aItem[nCntItem].vtxMinItem.x = vtx.x;
	//				}
	//				else if (vtx.y > g_aItem[nCntItem].vtxMaxItem.y)
	//				{//今回読み込んだ頂点が、一番小さい頂点より小さい場合
	//					g_aItem[nCntItem].vtxMaxItem.y = vtx.y;
	//				}
	//				else if (vtx.y < g_aItem[nCntItem].vtxMinItem.y)
	//				{//今回読み込んだ頂点が、一番小さい頂点より小さい場合
	//					g_aItem[nCntItem].vtxMinItem.y = vtx.y;
	//				}
	//				else if (vtx.z > g_aItem[nCntItem].vtxMaxItem.z)
	//				{//今回読み込んだ頂点が、一番小さい頂点より小さい場合
	//					g_aItem[nCntItem].vtxMaxItem.z = vtx.z;
	//				}
	//				else if (vtx.z < g_aItem[nCntItem].vtxMinItem.z)
	//				{//今回読み込んだ頂点が、一番小さい頂点より小さい場合
	//					g_aItem[nCntItem].vtxMinItem.z = vtx.z;
	//				}
	//				//========================================================================

	//				//頂点フォーマットのサイズ文ポインタを進める
	//				g_aItem[nCntItem].pVtxBuff += g_aItem[nCntItem].sizeFVF;

	//			}

	//			//頂点バッファのアンロック
	//			g_aItemMat[nCntMat].pMeshItem->UnlockVertexBuffer();

	//			//種類特定したし、もうタイプのfor文回す必要ないので、break
	//			break;

	//		}
	//	}
	//}
	//頂点数の取得



	D3DXMATERIAL* pMat;//マテリアルへのポインタ
	for (int nCntItemSET = 0; nCntItemSET < NUM_ITEM; nCntItemSET++)
	{
		//マテリアルへのポインタを取得（セットしたモデルと同じタイプ）
		pMat = (D3DXMATERIAL*)g_aItemMat[nCntItemSET].pBuffMatItem->GetBufferPointer();
		int nNumTexture = 0;
		for (int nCntMat = 0; nCntMat < (int)g_aItemMat[nCntItemSET].dwNumMatItem; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{//テクスチャファイルが存在する
					//テクスチャの読み込み
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_aItemMat[nCntItemSET].apTextureItem[nNumTexture]);
				nNumTexture++;
			}
		}
	}


}

//==============================
//タイトル画面の終了処理
//==============================
void UninitItem(void)
{
	for (int nCnt = 0; nCnt < NUM_ITEM; nCnt++)
	{
		//メッシュの破棄
		if (g_aItemMat[nCnt].pMeshItem != NULL)
		{
			g_aItemMat[nCnt].pMeshItem->Release();
			g_aItemMat[nCnt].pMeshItem = NULL;
		}
		//マテリアルの破棄
		if (g_aItemMat[nCnt].pBuffMatItem != NULL)
		{
			g_aItemMat[nCnt].pBuffMatItem->Release();
			g_aItemMat[nCnt].pBuffMatItem = NULL;
		}
	}
}
//============================
//モデルの更新処理
//============================
void UpdateItem(void)
{
	Player* pPlayer = GetPlayer();

	g_nDelayItem++;

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse == true)
		{
			g_aItem[nCntItem].bIsLanding = bCollisionModel(&g_aItem[nCntItem].pos, &g_aItem[nCntItem].posOld, &g_aItem[nCntItem].move, g_aItem[nCntItem].Size);

			if (g_aItem[nCntItem].bIsLanding == false)
			{
				g_aItem[nCntItem].move.y = -2.0f;//重力処理
			}

			if (g_aItem[nCntItem].pos.y <= 0.0f)
			{
				g_aItem[nCntItem].pos.y = 0.0f;
				g_aItem[nCntItem].move.y = 0.0f;
			}

			switch (g_aItem[nCntItem].nType)
			{
			case ITEM00_HEALHART:
				if (pPlayer->pos.x + pPlayer->Size.x / 2 >= g_aItem[nCntItem].pos.x - g_aItem[nCntItem].Size.x / 2 &&
					pPlayer->pos.x - pPlayer->Size.x / 2 <= g_aItem[nCntItem].pos.x + g_aItem[nCntItem].Size.x / 2 &&
					pPlayer->pos.y + pPlayer->Size.y >= g_aItem[nCntItem].pos.y - g_aItem[nCntItem].Size.y &&
					pPlayer->pos.y <= g_aItem[nCntItem].pos.y + g_aItem[nCntItem].Size.y &&
					pPlayer->pos.z + pPlayer->Size.z / 2 >= g_aItem[nCntItem].pos.z - g_aItem[nCntItem].Size.z / 2 &&
					pPlayer->pos.z - pPlayer->Size.z / 2 <= g_aItem[nCntItem].pos.z + g_aItem[nCntItem].Size.z / 2)
						{
							pPlayer->nHp += 300;//体力を３００回復する
							g_aItem[nCntItem].bUse = false;
							PlaySound(SOUND_LABEL_SE_HEAL_000);
						}
				break;
			case ITEM01_ENERGYBOOST:
				if (pPlayer->pos.x + pPlayer->Size.x / 2 >= g_aItem[nCntItem].pos.x - g_aItem[nCntItem].Size.x / 2 &&
					pPlayer->pos.x - pPlayer->Size.x / 2 <= g_aItem[nCntItem].pos.x + g_aItem[nCntItem].Size.x / 2 &&
					pPlayer->pos.y + pPlayer->Size.y >= g_aItem[nCntItem].pos.y - g_aItem[nCntItem].Size.y &&
					pPlayer->pos.y <= g_aItem[nCntItem].pos.y + g_aItem[nCntItem].Size.y &&
					pPlayer->pos.z + pPlayer->Size.z / 2 >= g_aItem[nCntItem].pos.z - g_aItem[nCntItem].Size.z / 2 &&
					pPlayer->pos.z - pPlayer->Size.z / 2 <= g_aItem[nCntItem].pos.z + g_aItem[nCntItem].Size.z / 2)
				{
					AddSkillGage(300.0f);
					g_aItem[nCntItem].bUse = false;
					PlaySound(SOUND_LABEL_SE_ORBGET);
				}
				break;
			case ITEM02_POWFULSWORD:
				if (pPlayer->pos.x + pPlayer->Size.x / 2 >= g_aItem[nCntItem].pos.x - g_aItem[nCntItem].Size.x / 2 &&
					pPlayer->pos.x - pPlayer->Size.x / 2 <= g_aItem[nCntItem].pos.x + g_aItem[nCntItem].Size.x / 2 &&
					pPlayer->pos.y + pPlayer->Size.y >= g_aItem[nCntItem].pos.y - g_aItem[nCntItem].Size.y &&
					pPlayer->pos.y <= g_aItem[nCntItem].pos.y + g_aItem[nCntItem].Size.y &&
					pPlayer->pos.z + pPlayer->Size.z / 2 >= g_aItem[nCntItem].pos.z - g_aItem[nCntItem].Size.z / 2 &&
					pPlayer->pos.z - pPlayer->Size.z / 2 <= g_aItem[nCntItem].pos.z + g_aItem[nCntItem].Size.z / 2)
				{
					pPlayer->nAttackUpStateCnt += 20;//少しの間攻撃力２倍
					g_aItem[nCntItem].bUse = false;
					PlaySound(SOUND_LABEL_SE_SPECIALATTACK);
				}
				break;
			case ITEM03_CAPSULEBARRIER:
				if (pPlayer->pos.x + pPlayer->Size.x / 2 >= g_aItem[nCntItem].pos.x - g_aItem[nCntItem].Size.x / 2 &&
					pPlayer->pos.x - pPlayer->Size.x / 2 <= g_aItem[nCntItem].pos.x + g_aItem[nCntItem].Size.x / 2 &&
					pPlayer->pos.y + pPlayer->Size.y >= g_aItem[nCntItem].pos.y - g_aItem[nCntItem].Size.y &&
					pPlayer->pos.y <= g_aItem[nCntItem].pos.y + g_aItem[nCntItem].Size.y &&
					pPlayer->pos.z + pPlayer->Size.z / 2 >= g_aItem[nCntItem].pos.z - g_aItem[nCntItem].Size.z / 2 &&
					pPlayer->pos.z - pPlayer->Size.z / 2 <= g_aItem[nCntItem].pos.z + g_aItem[nCntItem].Size.z / 2)
				{
					pPlayer->nBarrierCount += 5;//５回だけダメージを無効にするバリアを付与。
					g_aItem[nCntItem].bUse = false;
					PlaySound(SOUND_LABEL_SE_BARRIER);
				}
				break;
			}
			float fRandumRot = float(rand() % 200 + 1) / 100;
			float fRandumSpeed = float(rand() % 100 + 30) / 10;
			float fRandumMove = float(rand() % 21 - 10);
			float fRandumLength = float(rand() % 550 + 200);

			float fRandumColorR = float(rand() % 100 + 1) / 100;
			float fRandumColorG = float(rand() % 100 + 1) / 100;
			float fRandumColorB = float(rand() % 100 + 1) / 100;
			SetParticle(PARTICLETYPE_NORMAL, 200, 30.0f, 0.5f, g_aItem[nCntItem].pos,
				D3DXVECTOR3(sinf(-D3DX_PI * fRandumRot) * fRandumSpeed, fRandumMove, cosf(-D3DX_PI * fRandumRot) * fRandumSpeed),
				D3DXCOLOR(fRandumColorR, fRandumColorG, fRandumColorB, 1.0f), 0.0f, PARTICLETEX_NORMAL);

			SetPositionShadow(g_aItem[nCntItem].nIdxShadow, D3DXVECTOR3(g_aItem[nCntItem].pos.x, g_aItem[nCntItem].pos.y - g_aItem[nCntItem].Size.y / 2, g_aItem[nCntItem].pos.z), -1, nCntItem, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));


			if (g_aItem[nCntItem].bUse == false)
			{

				/*	PlaySound(SOUND_LABEL_SE_DEFEAT_Item);*/
				KillShadow(g_aItem[nCntItem].nIdxShadow);
			}


		}
	}



}

//================================
//タイトル画面の描画処理
//================================
void DrawItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();;//デバイスへのポインタと取得

	D3DXMATRIX mtxRot, mtxTrans;//計算用マトリックス

	D3DMATERIAL9 matDef;//現在のマテリアル保存用

	D3DXMATERIAL* pMat;//マテリアルデータへのポインタ


	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse == true)
		{

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_mtxWorldItem);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aItem[nCntItem].rot.y, g_aItem[nCntItem].rot.x, g_aItem[nCntItem].rot.z);

			D3DXMatrixMultiply(&g_mtxWorldItem, &g_mtxWorldItem, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aItem[nCntItem].pos.x, g_aItem[nCntItem].pos.y, g_aItem[nCntItem].pos.z);
			D3DXMatrixMultiply(&g_mtxWorldItem, &g_mtxWorldItem, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldItem);

			for (int nCntItemSET = 0; nCntItemSET < NUM_ITEM; nCntItemSET++)
			{//読み込むモデルの種類数分for分を回し、同じタイプのマテリアルのポインタや、、
			//パーツの描画、マテリアルの設定をする。
				if (g_aItem[nCntItem].nType == nCntItemSET)
				{
					//マテリアルへのポインタを取得（セットしたモデルと同じタイプ）
					pMat = (D3DXMATERIAL*)g_aItemMat[nCntItemSET].pBuffMatItem->GetBufferPointer();

					int nNumTexture = 0;
					//セットされたモデルとタイプが同じもののモデルのマテリアルの数分読み込む。
					for (int nCntMat = 0; nCntMat < (int)g_aItemMat[nCntItemSET].dwNumMatItem; nCntMat++)
					{
						//マテリアルの設定
						pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

						if (pMat[nCntMat].pTextureFilename != NULL)
						{
							//テクスチャの設定
							pDevice->SetTexture(0, g_aItemMat[nCntItemSET].apTextureItem[nNumTexture]);
							nNumTexture++;
						}
						else
						{
							//テクスチャの設定
							pDevice->SetTexture(0, g_aItemMat[nCntItemSET].apTextureItem[nNumTexture]);
							nNumTexture++;
						}
						//モデル（パーツ）の描画
						g_aItemMat[nCntItemSET].pMeshItem->DrawSubset(nCntMat);
					}
					break;
				}
			}

			//保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

void SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse == false)
		{
			g_aItem[nCntItem].bUse = true;//使用しているかどうか
			g_aItem[nCntItem].pos = pos;//位置
			g_aItem[nCntItem].posOld = pos;//1F前の位置
			g_aItem[nCntItem].rot = rot;//向き
			g_aItem[nCntItem].nType = nType;//種類
			g_aItem[nCntItem].nIdxShadow = SetShadow();


			//=====================================
			//モデルの位置の最大最小値を取得する
			//=====================================
			for (int nCntMat = 0; nCntMat < g_nNumItem; nCntMat++)
			{//種類を割り当てるfor文
				if (g_aItem[nCntItem].nType == nCntMat)
				{//モデルの種類が一致

					//頂点数の取得
					g_aItem[nCntItem].nNumVtx = g_aItemMat[nCntMat].pMeshItem->GetNumVertices();

					//頂点フォーマットのサイズを取得
					g_aItem[nCntItem].sizeFVF = D3DXGetFVFVertexSize(g_aItemMat[nCntMat].pMeshItem->GetFVF());

					//頂点バッファのロック
					g_aItemMat[nCntMat].pMeshItem->LockVertexBuffer(D3DLOCK_READONLY, (void**)&g_aItem[nCntItem].pVtxBuff);


					for (int nCntVtx = 0; nCntVtx < g_aItem[nCntItem].nNumVtx; nCntVtx++)
					{
						//頂点座標の代入
						D3DXVECTOR3 vtx = *(D3DXVECTOR3*)g_aItem[nCntItem].pVtxBuff;

						//====================================================
						//頂点座標を比較してモデルの最小値最大値を取得
						//====================================================
						if (vtx.x > g_aItem[nCntItem].vtxMaxItem.x)
						{
							g_aItem[nCntItem].vtxMaxItem.x = vtx.x;
						}
						else if (vtx.x < g_aItem[nCntItem].vtxMinItem.x)
						{//今回読み込んだ頂点が、一番小さい頂点より小さい場合
							g_aItem[nCntItem].vtxMinItem.x = vtx.x;
						}
						else if (vtx.y > g_aItem[nCntItem].vtxMaxItem.y)
						{//今回読み込んだ頂点が、一番小さい頂点より小さい場合
							g_aItem[nCntItem].vtxMaxItem.y = vtx.y;
						}
						else if (vtx.y < g_aItem[nCntItem].vtxMinItem.y)
						{//今回読み込んだ頂点が、一番小さい頂点より小さい場合
							g_aItem[nCntItem].vtxMinItem.y = vtx.y;
						}
						else if (vtx.z > g_aItem[nCntItem].vtxMaxItem.z)
						{//今回読み込んだ頂点が、一番小さい頂点より小さい場合
							g_aItem[nCntItem].vtxMaxItem.z = vtx.z;
						}
						else if (vtx.z < g_aItem[nCntItem].vtxMinItem.z)
						{//今回読み込んだ頂点が、一番小さい頂点より小さい場合
							g_aItem[nCntItem].vtxMinItem.z = vtx.z;
						}
						//========================================================================

						//頂点フォーマットのサイズ文ポインタを進める
						g_aItem[nCntItem].pVtxBuff += g_aItem[nCntItem].sizeFVF;

					}

					//頂点バッファのアンロック
					g_aItemMat[nCntMat].pMeshItem->UnlockVertexBuffer();

					//種類特定したし、もうタイプのfor文回す必要ないので、break
					break;

				}
			}
			//============================================================================================================================

			g_aItem[nCntItem].Size = g_aItem[nCntItem].vtxMaxItem - g_aItem[nCntItem].vtxMinItem;

			
			
			//===========================================================
			//g_aItem[nCntItem].nIdxShadow = SetShadow();//影のインデックスを設定
			break;
		}
	}
}

int GetNumItem(void)
{
	return g_nNumItem;//読み込むモデルの最大数を取得する
}

Item* GetItem(void)//敵構造体の情報を取得する
{
	return &g_aItem[0];
}
