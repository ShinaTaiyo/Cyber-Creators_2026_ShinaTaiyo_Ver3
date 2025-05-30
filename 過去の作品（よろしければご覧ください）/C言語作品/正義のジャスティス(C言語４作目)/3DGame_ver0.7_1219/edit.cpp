//========================================================
//
//１１月７日：モデル読み込み[Edit.cpp]
//Author:ShinaTaiyo
//
//=========================================================

#include "edit.h"
#include "model.h"
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "shadow.h"
#include "player.h"
#include <stdio.h>
#include <string.h>

//グローバル変数
int g_nNumEdit = 0;
#define Edit_SCALE (10.0f)
#define EDITSET "data\\TEXTFILE\\Edit.txt"
#define SAVEEDIT "data\\TEXTFILE\\SaveEdit.txt"
#define SAVEENEMY "data\\TEXTFILE\\SaveEnemy.txt"


LPD3DXMESH g_pMeshEdit[NUM_EDIT] = {};//メッシュ（頂点情報）へのポインタ（モデルの種類数分）
LPD3DXBUFFER g_pBuffMatEdit[NUM_EDIT] = {};//マテリアルへのポインタ
DWORD g_dwNumMatEdit[NUM_EDIT] = {};//マテリアルの数
LPDIRECT3DTEXTURE9 g_apTextureEdit[NUM_EDIT] = {};
D3DXVECTOR3 g_posEdit;//位置
D3DXVECTOR3 g_rotEdit;//向き
D3DXMATRIX g_mtxWorldEdit;//ワールドマトリックス

Edit g_aEdit;//モデル情報の構造体

int g_nGetNumModel = 0;//取得したモデルの最大数を保存する
int g_nSetNumModel = 0;//モデルを設置する時のタイプ番号を保存する
int g_nSaveSetModel = 0;//モデルを設置した数を保存する
Edit g_aSaveEditModel[MAX_MODEL];

//int g_nldxShadow = -1;//対象の影のインデックス（番号）

//タイトル画面の初期化処理
void InitEdit(void)
{

	g_nGetNumModel = GetNumModel();//取得したモデルの最大数を保存する

    g_nSetNumModel = 0;//モデルを設置する時のタイプ番号を保存する

	g_nSaveSetModel = 0;//モデルを設置した数を保存する

	//for (int nCntEdit = 0; nCntEdit < NUM_EDIT; nCntEdit++)
	//{
	//	g_dwNumMatEdit[nCntEdit] = 0;//マテリアルの数
	//}
	//g_nldxShadow = 0;//対象の影のインデックス

	g_nNumEdit = 2;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスへのポインタと取得

	g_posEdit = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotEdit = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	g_nNumEdit = 0;

	for (int nCntEditModel = 0; nCntEditModel < MAX_MODEL; nCntEditModel++)
	{
		g_aSaveEditModel[nCntEditModel].bUse = false;
		g_aSaveEditModel[nCntEditModel].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aSaveEditModel[nCntEditModel].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aSaveEditModel[nCntEditModel].nType = 0;
	}
	////モデルの読み込み
	//D3DXLoadMeshFromX("data\\Edit\\bill.x",
	//	D3DXMESH_SYSTEMMEM,
	//	pDevice,
	//	NULL,
	//	&g_pBuffMatEdit,
	//	NULL,
	//	&g_dwNumMatEdit,
	//	&g_pMeshEdit[EditTYPE_BILL]);

	//D3DXLoadMeshFromX("data\\Edit\\BigBriddge.x",
	//	D3DXMESH_SYSTEMMEM,
	//	pDevice,
	//	NULL,
	//	&g_pBuffMatEdit,
	//	NULL,
	//	&g_dwNumMatEdit,
	//	&g_pMeshEdit[EditTYPE_BRIDDGE]);




	/*for (int nCnt = 0; nCnt < MAX_EDIT; nCnt++)
	{*/
		g_aEdit.bUse = false;//使用しているかどうか
		g_aEdit.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//位置
		g_aEdit.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//向き
		g_aEdit.nIdxShadow = -1;
		g_aEdit.nType = -1;
		g_aEdit.nEditMode = 0;
	//}

	g_aEdit.bUse = true;


	char aString[100] = {};//文字列を読み込むための変数
	int nMaxRoadEdit = 0;//読み込むモデルの最大数
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

	pFile = fopen(MODELSET, "r");

	if (pFile != NULL)//ファイルが開いているかどうか
	{
		while (1)
		{

			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(&aString[0], "ENDSCRIPT") == 0)
			{
				break;//行の最後でこの文字列を判定させることにより、ループを終わらせる
			}
			else if (strcmp(&aString[0], "NUM_MODEL") == 0)//読み込むモデルの種類数を決める
			{
				fscanf(pFile, "%d", &g_nNumEdit);

				for (int nCnt = 0; nCnt < 3; nCnt++)
				{
					fscanf(pFile, "%s", &aString[0]);
				}
				for (int nRoadEdit = 0; nRoadEdit < g_nNumEdit; nRoadEdit++)
				{
					/*	while (1)
						{*/
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "MODEL_FILENAME") == 0)
					{
						fscanf(pFile, "%s", &aStringPass[0]);
						//モデルの読み込み
						D3DXLoadMeshFromX(&aStringPass[0],
							D3DXMESH_SYSTEMMEM,
							pDevice,
							NULL,
							&g_pBuffMatEdit[nRoadEdit],//モデルでは、これも配列にする
							NULL,
							&g_dwNumMatEdit[nRoadEdit],//モデルでは、これも配列にする
							&g_pMeshEdit[nRoadEdit]);
						fscanf(pFile, "%s", &aString[0]);
					}
					/*	nCnt++;
						if (nCnt == g_nNumEdit)
						{
							break;
						}*/
				}
			}
			
		}
		fclose(pFile);
	}
}

//==============================
//タイトル画面の終了処理
//==============================
void UninitEdit(void)
{
	for (int nCnt = 0; nCnt < NUM_EDIT; nCnt++)
	{
		//メッシュの破棄
		if (g_pMeshEdit[nCnt] != NULL)
		{
			g_pMeshEdit[nCnt]->Release();
			g_pMeshEdit[nCnt] = NULL;
		}
		//マテリアルの破棄
		if (g_pBuffMatEdit[nCnt] != NULL)
		{
			g_pBuffMatEdit[nCnt]->Release();
			g_pBuffMatEdit[nCnt] = NULL;
		}
	}
}
//============================
//モデルの更新処理
//============================
void UpdateEdit(void)
{
	Player* pPlayer = GetPlayer();


//=========================
//エディット状態を変える
//=========================
	if (GetKeyboardTrigger(DIK_F3) == true)
	{
		g_aEdit.nEditMode++;

		if (g_aEdit.nEditMode == 2)
		{
			g_aEdit.nEditMode = 0;
		}
	}
	//==================================================

	if (g_aEdit.nEditMode == EDITTYPE_MODEL)
	{//エディットのモードを決める処理で、モードが「モデル」だったら...
		if (GetKeyboardTrigger(DIK_RIGHT) == true)
		{
			g_aEdit.nType++;
		}
		else if (GetKeyboardTrigger(DIK_LEFT) == true)
		{
			g_aEdit.nType--;
		}

		if (g_aEdit.nType <= -1)
		{
			g_aEdit.nType = g_nNumEdit;
		}
		else if (g_aEdit.nType > g_nNumEdit)
		{
			g_aEdit.nType = 0;
		}




		if (g_aEdit.nEditMode == EDITTYPE_MODEL)
		{
			g_aEdit.pos = pPlayer->pos;
			g_aEdit.rot = pPlayer->rot;
			if (GetKeyboardTrigger(DIK_RETURN) == true)
			{
				SetEdit(g_aEdit.pos, g_aEdit.rot, g_aEdit.nType);
			}

			if (GetKeyboardTrigger(DIK_F10) == true)
			{

				FILE* pFile;//ファイルポインタを宣言
				pFile = fopen(SAVEEDIT, "w");//ファイルに今回配置したモデル情報を書き出す

				if (pFile != NULL)//ファイルが開いていたら
				{
					for (int nCntSaveModel = 0; nCntSaveModel < g_nSaveSetModel - 1; nCntSaveModel++)
					{
						fprintf(pFile, "MODELSET\n");
						fprintf(pFile, "TYPE %d\n", g_aSaveEditModel[nCntSaveModel].nType);//設置したモデルの種類を書き出す
						fprintf(pFile, "POS %.3f  %.3f  %.3f\n", g_aSaveEditModel[nCntSaveModel].pos.x
							, g_aSaveEditModel[nCntSaveModel].pos.y, g_aSaveEditModel[nCntSaveModel].pos.z);//設置したモデルの位置を書き出す
						fprintf(pFile, "ROT %.3f  %.3f  %.3f\n", g_aSaveEditModel[nCntSaveModel].rot.x
							, g_aSaveEditModel[nCntSaveModel].rot.y, g_aSaveEditModel[nCntSaveModel].rot.z);//設置したモデルの向きを書き出す
						fprintf(pFile, "ENDMODELSET\n");
						fprintf(pFile, "\n");
					}
					fclose(pFile);
				}
			}
		}//敵の配置はEditEnemy.cppで行う
	}


}

//================================
//タイトル画面の描画処理
//================================
void DrawEdit(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();;//デバイスへのポインタと取得


	//for (int nCntEdit = 0; nCntEdit < MAX_EDIT; nCntEdit++)
	//{
	/*	if (g_aEdit.bUse == true)
		{*/
			D3DXMATRIX mtxRot, mtxTrans;//計算用マトリックス

			D3DMATERIAL9 matDef;//現在のマテリアル保存用

			D3DXMATERIAL* pMat;//マテリアルデータへのポインタ

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_mtxWorldEdit);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aEdit.rot.y, g_aEdit.rot.x, g_aEdit.rot.z);

			D3DXMatrixMultiply(&g_mtxWorldEdit, &g_mtxWorldEdit, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aEdit.pos.x, g_aEdit.pos.y, g_aEdit.pos.z);
			D3DXMatrixMultiply(&g_mtxWorldEdit, &g_mtxWorldEdit, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldEdit);

			//現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			for (int nCntEDITSET = 0; nCntEDITSET < g_nNumEdit; nCntEDITSET++)
			{//読み込むモデルの種類数分for分を回し、同じタイプのマテリアルのポインタや、、
			//パーツの描画、マテリアルの設定をする。
				if (g_aEdit.nType == nCntEDITSET)
				{
					//マテリアルへのポインタを取得（セットしたモデルと同じタイプ）
					pMat = (D3DXMATERIAL*)g_pBuffMatEdit[nCntEDITSET]->GetBufferPointer();

					//セットされたモデルとタイプが同じもののモデルのマテリアルの数分読み込む。
					for (int nCntMat = 0; nCntMat < (int)g_dwNumMatEdit[nCntEDITSET]; nCntMat++)
					{
						//マテリアルの設定
						pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

						//テクスチャの設定
						pDevice->SetTexture(0, NULL);

						//モデル（パーツ）の描画
						g_pMeshEdit[nCntEDITSET]->DrawSubset(nCntMat);
					}
					break;
				}
			}

			//保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		//}
	//}
}

//void SetEdit(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
//{
//	for (int nCntEdit = 0; nCntEdit < MAX_EDIT; nCntEdit++)
//	{
//		if (g_aEdit.bUse == false)
//		{
//			g_aEdit.bUse = true;//使用しているかどうか
//			g_aEdit.pos = pos;//位置
//			g_aEdit.rot = rot;//向き
//			g_aEdit.nType = nType;//種類
//			g_aEdit.nIdxShadow = SetShadow();//影のインデックスを設定
//			break;
//		}
//	}
//}

void SetEdit(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	/*for (int nCntEdit = 0; nCntEdit < MAX_EDIT; nCntEdit++)
	{
		if (g_aEdit.bUse == false)
		{*/
			Player* pPlayer = GetPlayer();
			//g_aEdit.bUse = true;//使用しているかどうか
	/*		g_aEdit.nType = nType;
			g_aEdit.pos = pos;
			g_aEdit.rot = rot;*/

			g_aSaveEditModel[g_nSaveSetModel].nType = nType;
			g_aSaveEditModel[g_nSaveSetModel].pos = pos;
			g_aSaveEditModel[g_nSaveSetModel].rot = rot;
			SetModel(g_aEdit.pos, g_aEdit.rot, g_aEdit.nType);
			
			g_nSaveSetModel++;//設置したモデルの最大数をインクリメントする
			/*while (1)
			{
				g_aEdit.pos = pPlayer->pos;
				g_aEdit.rot = pPlayer->rot;

				if (GetKeyboardTrigger(DIK_RIGHT) == true)
				{
					g_aEdit.nType++;
				}
				else if (GetKeyboardTrigger(DIK_LEFT) == true)
				{
					g_aEdit.nType--;
				}

				if (g_aEdit.nType <= -1)
				{
					g_aEdit.nType = g_nNumEdit;
				}
				else if (g_aEdit.nType > g_nNumEdit)
				{
					g_aEdit.nType = 0;
				}

				if (GetKeyboardTrigger(DIK_F10) == true)
				{
					SetModel(g_aEdit.pos, g_aEdit.rot, g_aEdit.nType);
					break;
				}
			}
			break;*/
	//	}
	//}
}

Edit* GetEdit(void)
{
	return &g_aEdit;//エディット状態を取得
}

