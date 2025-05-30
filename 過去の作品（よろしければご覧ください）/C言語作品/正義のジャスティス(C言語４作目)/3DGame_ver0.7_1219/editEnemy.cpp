//========================================================
//
//１１月７日：モデル読み込み[EditEnemy.cpp]
//Author:ShinaTaiyo
//
//=========================================================

#include "editEnemy.h"
#include "model.h"
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "shadow.h"
#include "player.h"
#include <stdio.h>
#include <string.h>
#include "enemy.h"
#include "edit.h"
//グローバル変数
int g_nNumEditEnemy = 0;
#define EditEnemy_SCALE (10.0f)
#define EditEnemySET "data\\TEXTFILE\\EditEnemy.txt"
#define SAVEEditEnemy "data\\TEXTFILE\\SaveEditEnemy.txt"
#define SAVEENEMY "data\\TEXTFILE\\SaveEnemy.txt"


LPD3DXMESH g_pMeshEditEnemy[NUM_EDITENEMY] = {};//メッシュ（頂点情報）へのポインタ（モデルの種類数分）
LPD3DXBUFFER g_pBuffMatEditEnemy[NUM_EDITENEMY] = {};//マテリアルへのポインタ
DWORD g_dwNumMatEditEnemy[NUM_EDITENEMY] = {};//マテリアルの数
LPDIRECT3DTEXTURE9 g_apTextureEditEnemy[NUM_EDITENEMY] = {};
D3DXVECTOR3 g_posEditEnemy;//位置
D3DXVECTOR3 g_rotEditEnemy;//向き
D3DXMATRIX g_mtxWorldEditEnemy;//ワールドマトリックス

EditEnemy g_aEditEnemy;//モデル情報の構造体

//int g_nGetNumModel = 0;//取得したモデルの最大数を保存する
int g_nSetNumEnemy = 0;//モデルを設置する時のタイプ番号を保存する
int g_nSaveSetEnemy = 0;//モデルを設置した数を保存する
EditEnemy g_aSaveEditEnemyModel[MAX_MODEL];

//int g_nldxShadow = -1;//対象の影のインデックス（番号）

//タイトル画面の初期化処理
void InitEditEnemy(void)
{

	//g_nGetNumModel = GetNumModel();//取得したモデルの最大数を保存する

	g_nSetNumEnemy = 0;//モデルを設置する時のタイプ番号を保存する

	g_nSaveSetEnemy = 0;//モデルを設置した数を保存する

	//for (int nCntEditEnemy = 0; nCntEditEnemy < NUM_EDITENEMY; nCntEditEnemy++)
	//{
	//	g_dwNumMatEditEnemy[nCntEditEnemy] = 0;//マテリアルの数
	//}
	//g_nldxShadow = 0;//対象の影のインデックス

	g_nNumEditEnemy = 2;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスへのポインタと取得

	g_posEditEnemy = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotEditEnemy = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	g_nNumEditEnemy = 0;

	for (int nCntEditEnemyModel = 0; nCntEditEnemyModel < MAX_MODEL; nCntEditEnemyModel++)
	{
		g_aSaveEditEnemyModel[nCntEditEnemyModel].bUse = false;
		g_aSaveEditEnemyModel[nCntEditEnemyModel].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aSaveEditEnemyModel[nCntEditEnemyModel].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aSaveEditEnemyModel[nCntEditEnemyModel].nType = 0;
	}
	////モデルの読み込み
	//D3DXLoadMeshFromX("data\\EditEnemy\\bill.x",
	//	D3DXMESH_SYSTEMMEM,
	//	pDevice,
	//	NULL,
	//	&g_pBuffMatEditEnemy,
	//	NULL,
	//	&g_dwNumMatEditEnemy,
	//	&g_pMeshEditEnemy[EditEnemyTYPE_BILL]);

	//D3DXLoadMeshFromX("data\\EditEnemy\\BigBriddge.x",
	//	D3DXMESH_SYSTEMMEM,
	//	pDevice,
	//	NULL,
	//	&g_pBuffMatEditEnemy,
	//	NULL,
	//	&g_dwNumMatEditEnemy,
	//	&g_pMeshEditEnemy[EditEnemyTYPE_BRIDDGE]);




	for (int nCnt = 0; nCnt < MAX_EDITENEMY; nCnt++)
	{
		g_aEditEnemy.bUse = false;//使用しているかどうか
		g_aEditEnemy.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//位置
		g_aEditEnemy.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//向き
		g_aEditEnemy.nIdxShadow = -1;
		g_aEditEnemy.nType = 0;
	}

	g_aEditEnemy.bUse = true;


	char aString[100] = {};//文字列を読み込むための変数
	int nMaxRoadEditEnemy = 0;//読み込むモデルの最大数
	int nType = 0;//読み込んだモデルの種類

	float fPosX = 0;//モデルの位置X
	float fPosY = 0;//モデルの位置Y
	float fPosZ = 0;//モデルの位置Z

	float fRotX = 0;//モデルの位置X
	float fRotY = 0;//モデルの位置Y
	float fRotZ = 0;//モデルの位置Z
	int nCnt = 0;//読み込むモデルを数える用
	FILE* pFile;//ファイルポインタ

	char aStringPass[100] = {};

	pFile = fopen(ENEMYSET, "r");

	if (pFile != NULL)//ファイルが開いているかどうか
	{
		while (1)
		{

			fscanf(pFile, "%s", &aString[0]);


			/*	cData = fgetc(pFile);*/

			if (aString[0] == '#')
			{
				fgets(&aString[0], 100, pFile);
				continue;
			}

			if (strcmp(&aString[0], "ENDSCRIPT") == 0)
			{
				fclose(pFile);
				break;//行の最後でこの文字列を判定させることにより、ループを終わらせる
			}
			else if (strcmp(&aString[0], "NUM_ENEMY") == 0)//読み込むモデルの種類数を決める
			{
				fscanf(pFile, "%d", &g_nNumEditEnemy);

				for (int nCnt = 0; nCnt < 3; nCnt++)
				{
					fscanf(pFile, "%s", &aString[0]);
				}
				/*for (int nRoadEditEnemy = 0; nRoadEditEnemy < g_nNumEditEnemy; nRoadEditEnemy++)
				{*/
				/*	while (1)
					{*/
				int nRoadEditEnemy = 0;
				do
				{
					fscanf(pFile, "%s", &aString[0]);

					if (aString[0] == '#')
					{
						fgets(&aString[0], 100, pFile);
						continue;
					}
					else if (strcmp(&aString[0], "ENEMY_FILENAME") == 0)
					{
						fscanf(pFile, "%s", &aStringPass[0]);
						//モデルの読み込み
						D3DXLoadMeshFromX(&aStringPass[0],
							D3DXMESH_SYSTEMMEM,
							pDevice,
							NULL,
							&g_pBuffMatEditEnemy[nRoadEditEnemy],//モデルでは、これも配列にする
							NULL,
							&g_dwNumMatEditEnemy[nRoadEditEnemy],//モデルでは、これも配列にする
							&g_pMeshEditEnemy[nRoadEditEnemy]);
						fscanf(pFile, "%s", &aString[0]);
						nRoadEditEnemy++;
					}

				} while (nRoadEditEnemy < g_nNumEditEnemy);
				/*	nCnt++;
					if (nCnt == g_nNumEditEnemy)
					{
						break;
					}*/
					//}
			}
		}
	}
}

//==============================
//タイトル画面の終了処理
//==============================
void UninitEditEnemy(void)
{
	for (int nCnt = 0; nCnt < NUM_EDITENEMY; nCnt++)
	{
		//メッシュの破棄
		if (g_pMeshEditEnemy[nCnt] != NULL)
		{
			g_pMeshEditEnemy[nCnt]->Release();
			g_pMeshEditEnemy[nCnt] = NULL;
		}
		//マテリアルの破棄
		if (g_pBuffMatEditEnemy[nCnt] != NULL)
		{
			g_pBuffMatEditEnemy[nCnt]->Release();
			g_pBuffMatEditEnemy[nCnt] = NULL;
		}
	}
}
//============================
//モデルの更新処理
//============================
void UpdateEditEnemy(void)
{
	Player* pPlayer = GetPlayer();
	Edit* pEdit = GetEdit();
	if (pEdit->nEditMode == EDITTYPE_ENEMY)
	{
		if (GetKeyboardTrigger(DIK_RIGHT) == true)
		{
			g_aEditEnemy.nType++;
		}
		else if (GetKeyboardTrigger(DIK_LEFT) == true)
		{
			g_aEditEnemy.nType--;
		}

		if (g_aEditEnemy.nType <= -1)
		{
			g_aEditEnemy.nType = g_nNumEditEnemy;
		}
		else if (g_aEditEnemy.nType > g_nNumEditEnemy)
		{
			g_aEditEnemy.nType = 0;
		}

		g_aEditEnemy.pos = pPlayer->pos;
		g_aEditEnemy.rot = pPlayer->rot;
		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{
			SetEditEnemy(g_aEditEnemy.pos, g_aEditEnemy.rot, g_aEditEnemy.nType);
		}

		if (GetKeyboardTrigger(DIK_F10) == true)
		{

			FILE* pFile;//ファイルポインタを宣言
			pFile = fopen(SAVEEditEnemy, "w");//ファイルに今回配置したモデル情報を書き出す

			if (pFile != NULL)//ファイルが開いていたら
			{
				for (int nCntSaveModel = 0; nCntSaveModel < g_nSaveSetEnemy - 1; nCntSaveModel++)
				{
					fprintf(pFile, "ENEMYSET\n");
					fprintf(pFile, "TYPE %d\n", g_aSaveEditEnemyModel[nCntSaveModel].nType);//設置したモデルの種類を書き出す
					fprintf(pFile, "POS %.3f  %.3f  %.3f\n", g_aSaveEditEnemyModel[nCntSaveModel].pos.x
						, g_aSaveEditEnemyModel[nCntSaveModel].pos.y, g_aSaveEditEnemyModel[nCntSaveModel].pos.z);//設置したモデルの位置を書き出す
					fprintf(pFile, "ROT %.3f  %.3f  %.3f\n", g_aSaveEditEnemyModel[nCntSaveModel].rot.x
						, g_aSaveEditEnemyModel[nCntSaveModel].rot.y, g_aSaveEditEnemyModel[nCntSaveModel].rot.z);//設置したモデルの向きを書き出す
					fprintf(pFile, "ENDENEMYSET\n");
					fprintf(pFile, "\n");
				}
				fclose(pFile);
			}
		}

	}
}

//================================
//タイトル画面の描画処理
//================================
void DrawEditEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();;//デバイスへのポインタと取得


	/*for (int nCntEditEnemy = 0; nCntEditEnemy < MAX_EDITENEMY; nCntEditEnemy++)
	{*/
		/*	if (g_aEditEnemy.bUse == true)
			{*/
		D3DXMATRIX mtxRot, mtxTrans;//計算用マトリックス

		D3DMATERIAL9 matDef;//現在のマテリアル保存用

		D3DXMATERIAL* pMat;//マテリアルデータへのポインタ

		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_mtxWorldEditEnemy);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aEditEnemy.rot.y, g_aEditEnemy.rot.x, g_aEditEnemy.rot.z);

		D3DXMatrixMultiply(&g_mtxWorldEditEnemy, &g_mtxWorldEditEnemy, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_aEditEnemy.pos.x, g_aEditEnemy.pos.y, g_aEditEnemy.pos.z);
		D3DXMatrixMultiply(&g_mtxWorldEditEnemy, &g_mtxWorldEditEnemy, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldEditEnemy);

		//現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		for (int nCntEditEnemySET = 0; nCntEditEnemySET < g_nNumEditEnemy; nCntEditEnemySET++)
		{//読み込むモデルの種類数分for分を回し、同じタイプのマテリアルのポインタや、、
		//パーツの描画、マテリアルの設定をする。
			if (g_aEditEnemy.nType == nCntEditEnemySET)
			{
				//マテリアルへのポインタを取得（セットしたモデルと同じタイプ）
				pMat = (D3DXMATERIAL*)g_pBuffMatEditEnemy[nCntEditEnemySET]->GetBufferPointer();

				//セットされたモデルとタイプが同じもののモデルのマテリアルの数分読み込む。
				for (int nCntMat = 0; nCntMat < (int)g_dwNumMatEditEnemy[nCntEditEnemySET]; nCntMat++)
				{
					//マテリアルの設定
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

					//テクスチャの設定
					pDevice->SetTexture(0, NULL);

					//モデル（パーツ）の描画
					g_pMeshEditEnemy[nCntEditEnemySET]->DrawSubset(nCntMat);
				}
				break;
			}
		}

		//保存していたマテリアルを戻す
		pDevice->SetMaterial(&matDef);
		//}
	//}
}

//void SetEditEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
//{
//	for (int nCntEditEnemy = 0; nCntEditEnemy < MAX_EDITENEMY; nCntEditEnemy++)
//	{
//		if (g_aEditEnemy.bUse == false)
//		{
//			g_aEditEnemy.bUse = true;//使用しているかどうか
//			g_aEditEnemy.pos = pos;//位置
//			g_aEditEnemy.rot = rot;//向き
//			g_aEditEnemy.nType = nType;//種類
//			g_aEditEnemy.nIdxShadow = SetShadow();//影のインデックスを設定
//			break;
//		}
//	}
//}

void SetEditEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	/*for (int nCntEditEnemy = 0; nCntEditEnemy < MAX_EDITENEMY; nCntEditEnemy++)
	{
		if (g_aEditEnemy.bUse == false)
		{*/
	Player* pPlayer = GetPlayer();
	g_aEditEnemy.bUse = true;//使用しているかどうか
	g_aEditEnemy.nType = nType;
	g_aEditEnemy.pos = pos;
	g_aEditEnemy.rot = rot;

	g_aSaveEditEnemyModel[g_nSaveSetEnemy].nType = nType;
	g_aSaveEditEnemyModel[g_nSaveSetEnemy].pos = pos;
	g_aSaveEditEnemyModel[g_nSaveSetEnemy].rot = rot;
	SetEnemy(g_aEditEnemy.pos, g_aEditEnemy.rot, g_aEditEnemy.nType);

	g_nSaveSetEnemy++;//設置したモデルの最大数をインクリメントする
	/*while (1)
	{
		g_aEditEnemy.pos = pPlayer->pos;
		g_aEditEnemy.rot = pPlayer->rot;

		if (GetKeyboardTrigger(DIK_RIGHT) == true)
		{
			g_aEditEnemy.nType++;
		}
		else if (GetKeyboardTrigger(DIK_LEFT) == true)
		{
			g_aEditEnemy.nType--;
		}

		if (g_aEditEnemy.nType <= -1)
		{
			g_aEditEnemy.nType = g_nNumEditEnemy;
		}
		else if (g_aEditEnemy.nType > g_nNumEditEnemy)
		{
			g_aEditEnemy.nType = 0;
		}

		if (GetKeyboardTrigger(DIK_F10) == true)
		{
			SetModel(g_aEditEnemy.pos, g_aEditEnemy.rot, g_aEditEnemy.nType);
			break;
		}
	}
	break;*/
	//	}
	//}
}

