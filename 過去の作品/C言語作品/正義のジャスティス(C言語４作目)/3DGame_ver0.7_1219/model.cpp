//========================================================
//
//１１月７日：モデル読み込み[Model.cpp]
//Author:ShinaTaiyo
//
//=========================================================

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
int g_nNumModel = 0;//読み込むモデルの最大数を決める
#define Model_SCALE (10.0f)
//#define SAVEModel "data\\TEXTFILE\\SaveModel.txt"

#define MAX_TEXTURE (32)


typedef struct
{
	LPD3DXMESH pMeshModel;//メッシュ（頂点情報）へのポインタ（モデルの種類数分）
	LPD3DXBUFFER pBuffMatModel;//マテリアルへのポインタ
	DWORD dwNumMatModel;//マテリアルの数
    LPDIRECT3DTEXTURE9 apTextureModel[MAX_TEXTURE];//テクスチャへのポインタ
}ModelMat;

D3DXVECTOR3 g_posModel;//位置
D3DXVECTOR3 g_rotModel;//向き
D3DXMATRIX g_mtxWorldModel;//ワールドマトリックス

Model g_aModel[MAX_MODEL];//モデル情報の構造体

ModelMat g_aModelMat[NUM_MODEL];//モデルのマテリアルや頂点の構造体

int g_nldxShadow = -1;//対象の影のインデックス（番号）
bool g_bIsLanding;//地面にいるかどうか

//タイトル画面の初期化処理
void InitModel(void)
{

	//for (int nCntModel = 0; nCntModel < NUM_MODEL; nCntModel++)
	//{
	//	g_dwNumMatModel[nCntModel] = 0;//マテリアルの数
	//}
	g_nldxShadow = 0;//対象の影のインデックス

	g_nNumModel = 2;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスへのポインタと取得

	g_posModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	g_nNumModel = 0;

	g_bIsLanding = false;//地面にいるかどうか

	

	////モデルの読み込み
	//D3DXLoadMeshFromX("data\\Model\\bill.x",
	//	D3DXMESH_SYSTEMMEM,
	//	pDevice,
	//	NULL,
	//	&g_pBuffMatModel,
	//	NULL,
	//	&g_dwNumMatModel,
	//	&g_pMeshModel[ModelTYPE_BILL]);

	//D3DXLoadMeshFromX("data\\Model\\BigBriddge.x",
	//	D3DXMESH_SYSTEMMEM,
	//	pDevice,
	//	NULL,
	//	&g_pBuffMatModel,
	//	NULL,
	//	&g_dwNumMatModel,
	//	&g_pMeshModel[ModelTYPE_BRIDDGE]);


	for (int nCntMatModel = 0; nCntMatModel < NUM_MODEL; nCntMatModel++)
	{
		g_aModelMat[nCntMatModel].dwNumMatModel = {};
		g_aModelMat[nCntMatModel].pBuffMatModel = {};
		g_aModelMat[nCntMatModel].pMeshModel = {};
		for (int nCntTexture = 0; nCntTexture < MAX_TEXTURE; nCntTexture++)
		{
			g_aModelMat[nCntMatModel].apTextureModel[nCntTexture] = {};//一つのモデルに関連するテクスチャの初期化
		}
	}


	for (int nCnt = 0; nCnt < MAX_MODEL; nCnt++)
	{
		g_aModel[nCnt].bUse = false;//使用しているかどうか
		g_aModel[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//位置
		g_aModel[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//向き
		g_aModel[nCnt].nIdxShadow = -1;
		g_aModel[nCnt].nType = -1;
		g_aModel[nCnt].vtxMinModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//それぞれの座標のモデルの位置の最小値
		g_aModel[nCnt].vtxMaxModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//それぞれの座標のモデルの位置の最大値
		g_aModel[nCnt].nNumVtx = 0;//頂点数
		g_aModel[nCnt].sizeFVF;//頂点フォーマットのサイズ
		g_aModel[nCnt].pVtxBuff;//頂点バッファへのポインタ
	}


	char aString[100] = {};//文字列を読み込むための変数
	int nMaxRoadModel = 0;//読み込むモデルの最大数
	int nType = 0;//読み込んだモデルの種類

	float fPosX = 0.0f;//モデルの位置X
	float fPosY = 0.0f;//モデルの位置Y
	float fPosZ = 0.0f;//モデルの位置Z

	float fRotX = 0.0f;//モデルの位置X
	float fRotY = 0.0f;//モデルの位置Y
	float fRotZ = 0.0f;//モデルの位置Z
	int nCnt = 0;//読み込むモデルを数える用
	FILE* pFile;//ファイルポインタ

	char aStringPass[100] = {};

	pFile = fopen(MODELSET, "r");
	char cData;
	if (pFile != NULL)//ファイルが開いているかどうか
	{
		while (1)
		{

			fscanf(pFile, "%s", &aString[0]);


			cData = fgetc(pFile);

			if (strcmp(&cData, "#") == 0)
			{
				continue;
			}

			if (strcmp(&aString[0], "ENDSCRIPT") == 0)
			{
				fclose(pFile);
				break;//行の最後でこの文字列を判定させることにより、ループを終わらせる
			}
			else if (strcmp(&aString[0], "NUM_MODEL") == 0)//読み込むモデルの種類数を決める
			{
				fscanf(pFile, "%d", &g_nNumModel);

				for (int nCnt = 0; nCnt < 3; nCnt++)
				{
					fscanf(pFile, "%s", &aString[0]);
				}
				for (int nRoadModel = 0; nRoadModel < g_nNumModel; nRoadModel++)
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
							&g_aModelMat[nRoadModel].pBuffMatModel,//モデルでは、これも配列にする
							NULL,
							&g_aModelMat[nRoadModel].dwNumMatModel,//モデルでは、これも配列にする
							&g_aModelMat[nRoadModel].pMeshModel);
						fscanf(pFile, "%s", &aString[0]);
					}
				/*	nCnt++;
					if (nCnt == g_nNumModel)
					{
						break;
					}*/
				}
			}
			else if (strcmp(&aString[0], "MODELSET") == 0)
			{
				while (1)
				{
					fscanf(pFile, "%s", &aString[0]);
					if (strcmp(&aString[0], "TYPE") == 0)
					{
						fscanf(pFile, "%d", &nType);//ウォールの種類を決める
					}
					else if (strcmp(&aString[0], "POS") == 0)
					{
						fscanf(pFile, "%f%f%f", &fPosX, &fPosY, &fPosZ);//位置情報を読み込む
					}
					else if (strcmp(&aString[0], "ROT") == 0)
					{
						fscanf(pFile, "%f%f%f", &fRotX, &fRotY, &fRotZ);//向き情報を読み込む
					}
					else if (strcmp(&aString[0], "ENDMODELSET") == 0)
					{
						SetModel(D3DXVECTOR3(fPosX,fPosY,fPosZ), D3DXVECTOR3(fRotX,fRotY,fRotZ),nType);
						break;
					}
				}
			}
		}
	}

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{//全てのモデルに割り当てる
		for (int nCntMat = 0; nCntMat < g_nNumModel; nCntMat++)
		{//種類を割り当てるfor文
			if (g_aModel[nCntModel].nType == nCntMat)
			{//モデルの種類が一致

				//頂点数の取得
				g_aModel[nCntModel].nNumVtx = g_aModelMat[nCntMat].pMeshModel->GetNumVertices();

				//頂点フォーマットのサイズを取得
				g_aModel[nCntModel].sizeFVF = D3DXGetFVFVertexSize(g_aModelMat[nCntMat].pMeshModel->GetFVF());

				//頂点バッファのロック
				g_aModelMat[nCntMat].pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&g_aModel[nCntModel].pVtxBuff);


				for (int nCntVtx = 0; nCntVtx < g_aModel[nCntModel].nNumVtx; nCntVtx++)
				{
					//頂点座標の代入
					D3DXVECTOR3 vtx = *(D3DXVECTOR3*)g_aModel[nCntModel].pVtxBuff;

					//====================================================
					//頂点座標を比較してモデルの最小値最大値を取得
					//====================================================
					if (vtx.x > g_aModel[nCntModel].vtxMaxModel.x)
					{
						g_aModel[nCntModel].vtxMaxModel.x = vtx.x;
					}
					else if (vtx.x < g_aModel[nCntModel].vtxMinModel.x)
					{//今回読み込んだ頂点が、一番小さい頂点より小さい場合
						g_aModel[nCntModel].vtxMinModel.x = vtx.x;
					}
					else if (vtx.y > g_aModel[nCntModel].vtxMaxModel.y)
					{//今回読み込んだ頂点が、一番小さい頂点より小さい場合
						g_aModel[nCntModel].vtxMaxModel.y = vtx.y;
					}
					else if (vtx.y < g_aModel[nCntModel].vtxMinModel.y)
					{//今回読み込んだ頂点が、一番小さい頂点より小さい場合
						g_aModel[nCntModel].vtxMinModel.y = vtx.y;
					}
					else if (vtx.z > g_aModel[nCntModel].vtxMaxModel.z)
					{//今回読み込んだ頂点が、一番小さい頂点より小さい場合
						g_aModel[nCntModel].vtxMaxModel.z = vtx.z;
					}
					else if (vtx.z < g_aModel[nCntModel].vtxMinModel.z)
					{//今回読み込んだ頂点が、一番小さい頂点より小さい場合
						g_aModel[nCntModel].vtxMinModel.z = vtx.z;
					}
					//========================================================================

					//頂点フォーマットのサイズ文ポインタを進める
					g_aModel[nCntModel].pVtxBuff += g_aModel[nCntModel].sizeFVF;

				}

				//頂点バッファのアンロック
				g_aModelMat[nCntMat].pMeshModel->UnlockVertexBuffer();

				//種類特定したし、もうタイプのfor文回す必要ないので、break
				break;

			}
		}
	}
	//頂点数の取得

	D3DXMATERIAL* pMat;//マテリアルへのポインタ
	for (int nCntModelSet = 0; nCntModelSet < g_nNumModel; nCntModelSet++)
	{
		//マテリアルへのポインタを取得（セットしたモデルと同じタイプ）
		pMat = (D3DXMATERIAL*)g_aModelMat[nCntModelSet].pBuffMatModel->GetBufferPointer();
		int nNumTexture = 0;
		for (int nCntMat = 0; nCntMat < (int)g_aModelMat[nCntModelSet].dwNumMatModel; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{//テクスチャファイルが存在する
					//テクスチャの読み込み
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_aModelMat[nCntModelSet].apTextureModel[nNumTexture]);
				nNumTexture++;
			}
		}
	}

}

//==============================
//タイトル画面の終了処理
//==============================
void UninitModel(void)
{
	for (int nCnt = 0; nCnt < g_nNumModel; nCnt++)
	{
		//メッシュの破棄
		if (g_aModelMat[nCnt].pMeshModel != NULL)
		{
			g_aModelMat[nCnt].pMeshModel ->Release();
			g_aModelMat[nCnt].pMeshModel = NULL;
		}
		//マテリアルの破棄
		if (g_aModelMat[nCnt].pBuffMatModel != NULL)
		{
			g_aModelMat[nCnt].pBuffMatModel->Release();
			g_aModelMat[nCnt].pBuffMatModel = NULL;
		}
	}
}
//============================
//モデルの更新処理
//============================
void UpdateModel(void)
{

	

	float fLength = 0.0f;
	for (int nCnt = 0; nCnt < MAX_MODEL; nCnt++)
	{
		if (g_aModel[nCnt].bUse == true)
		{
			Player* pPlayer = GetPlayer();
			
			/*if (pPlayer->pos.x + pPlayer->fWidth >= g_aModel[nCnt].pos.x + g_aModel[nCnt].vtxMinModel.x && 
				pPlayer->pos.x - pPlayer->fWidth <= g_aModel[nCnt].pos.x + g_aModel[nCnt].vtxMaxModel.x &&
				pPlayer->pos.y + pPlayer->fHeight >=g_aModel[nCnt].pos.y + g_aModel[nCnt].vtxMinModel.y &&
				pPlayer->pos.y - pPlayer->fHeight <=g_aModel[nCnt].pos.y + g_aModel[nCnt].vtxMaxModel.y &&
				pPlayer->pos.z + pPlayer->fWidth >= g_aModel[nCnt].pos.z + g_aModel[nCnt].vtxMinModel.z && 
				pPlayer->pos.z - pPlayer->fWidth <= g_aModel[nCnt].pos.z + g_aModel[nCnt].vtxMaxModel.z)
			{
				pPlayer->pos = pPlayer->posOld;
			}*/
			

		/*	fLength = sqrtf((g_aModel[nCnt].pos.x - pPlayer->pos.x) * (g_aModel[nCnt].pos.x - pPlayer->pos.x) +
				(g_aModel[nCnt].pos.z - pPlayer->pos.z) * (g_aModel[nCnt].pos.z - pPlayer->pos.z) + 
					(g_aModel[nCnt].pos.y - pPlayer->pos.y) * (g_aModel[nCnt].pos.y - pPlayer->pos.y)) / 3;

			if (fLength <= 20.0f)
			{
				pPlayer->pos = pPlayer->posOld;
			}*/

			

		//影の位置を設定
			SetPositionShadow(g_aModel[nCnt].nIdxShadow, g_aModel[nCnt].pos,-1,-1,D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));
		}
	}
}

//================================
//タイトル画面の描画処理
//================================
void DrawModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();;//デバイスへのポインタと取得

			D3DXMATRIX mtxRot, mtxTrans;//計算用マトリックス

			D3DMATERIAL9 matDef;//現在のマテリアル保存用

			D3DXMATERIAL* pMat;//マテリアルデータへのポインタ

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);
	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		if (g_aModel[nCntModel].bUse == true)
		{

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_mtxWorldModel);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aModel[nCntModel].rot.y, g_aModel[nCntModel].rot.x, g_aModel[nCntModel].rot.z);

			D3DXMatrixMultiply(&g_mtxWorldModel, &g_mtxWorldModel, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aModel[nCntModel].pos.x, g_aModel[nCntModel].pos.y, g_aModel[nCntModel].pos.z);
			D3DXMatrixMultiply(&g_mtxWorldModel, &g_mtxWorldModel, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldModel);


				for (int nCntModelSet = 0; nCntModelSet < g_nNumModel; nCntModelSet++)
				{//読み込むモデルの種類数分for分を回し、同じタイプのマテリアルのポインタや、、
				//パーツの描画、マテリアルの設定をする。
					if (g_aModel[nCntModel].nType == nCntModelSet)
					{
						//マテリアルへのポインタを取得（セットしたモデルと同じタイプ）
						pMat = (D3DXMATERIAL*)g_aModelMat[nCntModelSet].pBuffMatModel->GetBufferPointer();

						int nNumTexture = 0;
						//セットされたモデルとタイプが同じもののモデルのマテリアルの数分読み込む。
						for (int nCntMat = 0; nCntMat < (int)g_aModelMat[nCntModelSet].dwNumMatModel; nCntMat++)
						{
							//マテリアルの設定
							pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

							if (pMat[nCntMat].pTextureFilename != NULL)
							{
								//テクスチャの設定
								pDevice->SetTexture(0,g_aModelMat[nCntModelSet].apTextureModel[nNumTexture]);
								nNumTexture++;
							}
							else
							{
								//テクスチャの設定
								pDevice->SetTexture(0, g_aModelMat[nCntModelSet].apTextureModel[nNumTexture]);
								nNumTexture++;
							}
	                        //モデル（パーツ）の描画
							g_aModelMat[nCntModelSet].pMeshModel->DrawSubset(nCntMat);
						}
						break;
					}
				}
			
		}
	}
	        //保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
}

void SetModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		if (g_aModel[nCntModel].bUse == false)
		{
			g_aModel[nCntModel].bUse = true;//使用しているかどうか
			g_aModel[nCntModel].pos = pos;//位置
			g_aModel[nCntModel].rot = rot;//向き
			g_aModel[nCntModel].nType = nType;//種類
			g_aModel[nCntModel].nIdxShadow = SetShadow();//影のインデックスを設定
			break;
		}
	}
}

int GetNumModel(void)
{
	return g_nNumModel;//読み込むモデルの最大数を取得する
}

bool bCollisionModel(D3DXVECTOR3* pPos,
	D3DXVECTOR3* pPosOld,
	D3DXVECTOR3* pMove,
	D3DXVECTOR3 Size
)
{
	Player* pPlayer = GetPlayer();
	g_bIsLanding = false;//着地しているかどうか
	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		if (g_aModel[nCntModel].bUse == true)
		{//モデルを使っているかどうか
		if (pPos->x + Size.x / 2 >= g_aModel[nCntModel].pos.x + g_aModel[nCntModel].vtxMinModel.x
			&& pPos->x - Size.x / 2 <= g_aModel[nCntModel].pos.x + g_aModel[nCntModel].vtxMaxModel.x
			&& pPos->y <= g_aModel[nCntModel].pos.y + g_aModel[nCntModel].vtxMaxModel.y
			&& pPosOld->y >= g_aModel[nCntModel].pos.y + g_aModel[nCntModel].vtxMaxModel.y
			&& pPos->z + Size.z / 2 >= g_aModel[nCntModel].pos.z + g_aModel[nCntModel].vtxMinModel.z
			&& pPos->z - Size.z / 2 <= g_aModel[nCntModel].pos.z + g_aModel[nCntModel].vtxMaxModel.z)
		{//対象の下端がモデルの+Yに当たった時の処理
			pPos->y = g_aModel[nCntModel].pos.y + g_aModel[nCntModel].vtxMaxModel.y;
			g_bIsLanding = true;
		}
		else if (pPos->x + Size.x / 2 >= g_aModel[nCntModel].pos.x + g_aModel[nCntModel].vtxMinModel.x
			&& pPos->x - Size.x / 2 <= g_aModel[nCntModel].pos.x + g_aModel[nCntModel].vtxMaxModel.x
			&& pPos->y + Size.y >= g_aModel[nCntModel].pos.y + g_aModel[nCntModel].vtxMinModel.y
			&& pPosOld->y + Size.y <= g_aModel[nCntModel].pos.y + g_aModel[nCntModel].vtxMinModel.y
			&& pPos->z + Size.z / 2 >= g_aModel[nCntModel].pos.z + g_aModel[nCntModel].vtxMinModel.z
			&& pPos->z - Size.z / 2 <= g_aModel[nCntModel].pos.z + g_aModel[nCntModel].vtxMaxModel.z)
		{//対象の下端がモデルの+Yに当たった時の処理
			pPos->y = g_aModel[nCntModel].pos.y + g_aModel[nCntModel].vtxMinModel.y - Size.y;
		}
		else if (pPos->x + Size.x / 2 >= g_aModel[nCntModel].pos.x + g_aModel[nCntModel].vtxMinModel.x//対象の最大X座標が、モデルの最小X座標より上
				&& pPosOld->x + Size.x / 2 <= g_aModel[nCntModel].pos.x + g_aModel[nCntModel].vtxMinModel.x//対象の1F前の最大X座標が、モデルの最小X座標より下
				&& pPos->y + Size.y >= g_aModel[nCntModel].pos.y + g_aModel[nCntModel].vtxMinModel.y//対象の最大Y座標がモデルの最小Y座標より上
				&& pPos->y <= g_aModel[nCntModel].pos.y + g_aModel[nCntModel].vtxMaxModel.y
				&& pPos->z + Size.z / 2 >= g_aModel[nCntModel].pos.z + g_aModel[nCntModel].vtxMinModel.z//対象の最大X座標が、モデルの最小X座標より上
				&& pPos->z - Size.z / 2 <= g_aModel[nCntModel].pos.z + g_aModel[nCntModel].vtxMaxModel.z)//対象の1F前の最大X座標が、モデルの最小X座標より下)//対象の最小Y座標がモデルの最大Y座標より下
			{//対象がブロックの-Xに当たった時の処理
				pPos->x = g_aModel[nCntModel].pos.x + g_aModel[nCntModel].vtxMinModel.x - Size.x / 2;
			}
			else if (pPos->x - Size.x / 2 <= g_aModel[nCntModel].pos.x + g_aModel[nCntModel].vtxMaxModel.x
				&& pPosOld->x - Size.x / 2 >= g_aModel[nCntModel].pos.x + g_aModel[nCntModel].vtxMaxModel.x
				&& pPos->y + Size.y >= g_aModel[nCntModel].pos.y + g_aModel[nCntModel].vtxMinModel.y
				&& pPos->y <= g_aModel[nCntModel].pos.y + g_aModel[nCntModel].vtxMaxModel.y
				&& pPos->z + Size.z / 2 >= g_aModel[nCntModel].pos.z + g_aModel[nCntModel].vtxMinModel.z
				&& pPos->z - Size.z / 2 <= g_aModel[nCntModel].pos.z + g_aModel[nCntModel].vtxMaxModel.z)
			{//対象がブロックの+Xに当たった時の処理
				pPos->x = g_aModel[nCntModel].pos.x + g_aModel[nCntModel].vtxMaxModel.x + Size.x / 2;
			}
			else if (pPos->x + Size.x / 2 >= g_aModel[nCntModel].pos.x + g_aModel[nCntModel].vtxMinModel.x
				&& pPos->x - Size.x / 2 <= g_aModel[nCntModel].pos.x + g_aModel[nCntModel].vtxMaxModel.x
				&& pPos->y + Size.y >= g_aModel[nCntModel].pos.y + g_aModel[nCntModel].vtxMinModel.y
				&& pPos->y <= g_aModel[nCntModel].pos.y + g_aModel[nCntModel].vtxMaxModel.y
				&& pPos->z + Size.z / 2 >= g_aModel[nCntModel].pos.z + g_aModel[nCntModel].vtxMinModel.z
				&& pPosOld->z + Size.z / 2 <= g_aModel[nCntModel].pos.z + g_aModel[nCntModel].vtxMinModel.z)
			{//対象がブロックの+Xに当たった時の処理
				pPos->z = g_aModel[nCntModel].pos.z + g_aModel[nCntModel].vtxMinModel.z - Size.z / 2;
			}
			else if (pPos->x + Size.x / 2 >= g_aModel[nCntModel].pos.x + g_aModel[nCntModel].vtxMinModel.x
				&& pPos->x - Size.x / 2 <= g_aModel[nCntModel].pos.x + g_aModel[nCntModel].vtxMaxModel.x
				&& pPos->y + Size.y >= g_aModel[nCntModel].pos.y + g_aModel[nCntModel].vtxMinModel.y
				&& pPos->y <= g_aModel[nCntModel].pos.y + g_aModel[nCntModel].vtxMaxModel.y
				&& pPos->z - Size.z / 2 <= g_aModel[nCntModel].pos.z + g_aModel[nCntModel].vtxMaxModel.z
				&& pPosOld->z - Size.z / 2 >= g_aModel[nCntModel].pos.z + g_aModel[nCntModel].vtxMaxModel.z)
			{//対象がブロックの+Xに当たった時の処理
				pPos->z = g_aModel[nCntModel].pos.z + g_aModel[nCntModel].vtxMaxModel.z + Size.z / 2;
			}
		
			//else if (pPos->x + pvtxMax->x >= g_aModel[nCntModel].pos.x + g_aModel[nCntModel].vtxMinModel.x
			//	&& pPos->x + pvtxMin->x <= g_aModel[nCntModel].pos.x + g_aModel[nCntModel].vtxMaxModel.x
			//	&& pPos->y + pvtxMax->y >= g_aModel[nCntModel].pos.y + g_aModel[nCntModel].vtxMinModel.y
			//	&& pPos->y + pvtxMin->y <= g_aModel[nCntModel].pos.y + g_aModel[nCntModel].vtxMaxModel.y
			//	&& pPos->z + pvtxMin->z <= g_aModel[nCntModel].pos.z + g_aModel[nCntModel].vtxMaxModel.z
			//	&& pPosOld->z + pvtxMin->z > g_aModel[nCntModel].pos.z + g_aModel[nCntModel].vtxMaxModel.z)
			//{//対象がブロックの+Zに当たった時の処理
			//	pPos->z = g_aModel[nCntModel].pos.z + g_aModel[nCntModel].vtxMaxModel.z + pvtxMax->z;
			//}
			//else if (pPos->x + pvtxMax->x >= g_aModel[nCntModel].pos.x + g_aModel[nCntModel].vtxMinModel.x
			//	&& pPos->x + pvtxMin->x <= g_aModel[nCntModel].pos.x + g_aModel[nCntModel].vtxMaxModel.x
			//	&& pPos->y + pvtxMax->y * 2 >= g_aModel[nCntModel].pos.y + g_aModel[nCntModel].vtxMinModel.y
			//	&& pPosOld->y + pvtxMax->y * 2 < g_aModel[nCntModel].pos.y + g_aModel[nCntModel].vtxMinModel.y
			//	&& pPos->z + pvtxMax->z >= g_aModel[nCntModel].pos.z + g_aModel[nCntModel].vtxMinModel.z
			//	&& pPos->z + pvtxMin->z <= g_aModel[nCntModel].pos.z + g_aModel[nCntModel].vtxMaxModel.z)
			//{//対象がモデルの-Yに当たった時の処理
			//	pPos->y = g_aModel[nCntModel].pos.y + g_aModel[nCntModel].vtxMinModel.y + pvtxMin->z * 2;
			//}
			//else if (pPos->x + pvtxMax->x >= g_aModel[nCntModel].pos.x + g_aModel[nCntModel].vtxMinModel.x
			//	&& pPos->x + pvtxMin->x <= g_aModel[nCntModel].pos.x + g_aModel[nCntModel].vtxMaxModel.x
			//	&& pPos->y <= g_aModel[nCntModel].pos.y + g_aModel[nCntModel].vtxMaxModel.y
			//	&& pPosOld->y > g_aModel[nCntModel].pos.y + g_aModel[nCntModel].vtxMaxModel.y
			//	&& pPos->z + pvtxMax->z >= g_aModel[nCntModel].pos.z + g_aModel[nCntModel].vtxMinModel.z
			//	&& pPos->z + pvtxMin->z <= g_aModel[nCntModel].pos.z + g_aModel[nCntModel].vtxMaxModel.z)
			//{//対象がモデルの下が+Yに当たった時の処理
			//	pPos->y = g_aModel[nCntModel].pos.y + g_aModel[nCntModel].vtxMaxModel.y;
			//	g_bIsLanding = true;
			//}
			//else if (pPos->x + pvtxMax->x >= g_aModel[nCntModel].pos.x + g_aModel[nCntModel].vtxMinModel.x
			//	&& pPos->x + pvtxMin->x <= g_aModel[nCntModel].pos.x + g_aModel[nCntModel].vtxMaxModel.x
			//	&& pPos->y + pvtxMax->y > g_aModel[nCntModel].pos.y + g_aModel[nCntModel].vtxMinModel.y
			//	&& pPos->y + pvtxMin->y < g_aModel[nCntModel].pos.y + g_aModel[nCntModel].vtxMaxModel.y
			//	&& pPos->z + pvtxMin->z <= g_aModel[nCntModel].pos.z + g_aModel[nCntModel].vtxMaxModel.z
			//	&& pPosOld->z + pvtxMin->z >= g_aModel[nCntModel].pos.z + g_aModel[nCntModel].vtxMaxModel.z)
			//{//対象がブロックの+Yに当たった時の処理
			//	pPos->z = g_aModel[nCntModel].pos.z + g_aModel[nCntModel].vtxMaxModel.z + pvtxMax->z;
			//}
			////		if (pPos->x - fWidth < g_aModel[nCntModel].pos.x + g_aModel[nCntModel].fRadius//プレイヤーの左端がブロックの右端より内側なら
			//			&& pPosOld->x - fWidth >= g_aModel[nCntModel].pos.x + 5.0f/*+ g_aModel[nCntModel].fRadius*///1F前のプレイヤーの左端がブロックの右端より外側なら
			//			&& pPos->y - fHeight < g_aModel[nCntModel].pos.y + g_aModel[nCntModel].fRadius//プレイヤーの上端がブロックの下端より内側なら
			//			&& pPos->y > g_aModel[nCntModel].pos.y - g_aModel[nCntModel].fRadius)//プレイヤーの下端がブロックの上端より内側なら
			//		{//プレイヤーの左端がブロックの中に入ったら
			//			pPos->x = g_aModel[nCntModel].pos.x + g_aModel[nCntModel].fRadius + fWidth;
			//			pMove->x = 0.0f;

			//			*bIsWalling = true;
			//			if (*bMoveWay == false)
			//			{
			//				*bMoveWay = true;
			//			}
			//			else if (*bMoveWay == true)
			//			{
			//				*bMoveWay = false;
			//			}

			//			if (g_aModel[nCntModel].nType == BLOCKTYPE_FRAME && C_use == COLLISIONUSE_PLAYER)
			//			{
			//				DamageHealth(1);//１ダメージを与え続ける
			//			}
			//		}
			//		if (pPos->x + fWidth > g_aModel[nCntModel].pos.x - g_aModel[nCntModel].fRadius
			//			&& pPos->x - fWidth < g_aModel[nCntModel].pos.x + g_aModel[nCntModel].fRadius
			//			&& pPos->y >= g_aModel[nCntModel].pos.y - g_aModel[nCntModel].fRadius
			//			&& pPosOld->y <= g_aModel[nCntModel].pos.y - 5.0f/*- g_aModel[nCntModel].fRadius*/)
			//		{//プレイヤーの下端が、ブロックの上端より下なら

			//			if (g_aModel[nCntModel].nType == BLOCKTYPE_ARROW && *bIsWalling == false)
			//			{
			//				pPos->x = g_aModel[nCntModel].pos.x;//プレイヤーの位置をブロックの上端に移動させる。
			//			}

			//			pPos->y = g_aModel[nCntModel].pos.y - g_aModel[nCntModel].fRadius;//プレイヤーの位置をブロックの上端に移動させる。
			//			pMove->y = 0.0f;//重力を0にする
			//			*fGravity = 0.0f;//重力の補正値も0にする　
			//			g_bIsLanding = true;//地面にいるフラグを立て、ジャンプを可能にする

			//			if (g_aModel[nCntModel].nType == BLOCKTYPE_FRAME && C_use == COLLISIONUSE_PLAYER)
			//			{
			//				DamageHealth(1);//１ダメージを与え続ける
			//			}
			//			else if (g_aModel[nCntModel].nType == BLOCKTYPE_ICE && g_aModel[nCntModel].nCollisionUse == COLLISIONUSE_PLAYER)
			//			{
			//				pPlayer->nInertia = INERTIA_ICE;//氷ブロックの上にいるので、慣性を強める
			//			}
			//			else if (g_aModel[nCntModel].nType == BLOCKTYPE_AUTOMOVE_RIGHT && g_aModel[nCntModel].nCollisionUse == COLLISIONUSE_PLAYER)
			//			{
			//				pPlayer->bAutoMoveR = true;//右方向へ自動的に移動するか
			//			}
			//			else if (g_aModel[nCntModel].nType == BLOCKTYPE_AUTOMOVE_LEFT && g_aModel[nCntModel].nCollisionUse == COLLISIONUSE_PLAYER)
			//			{
			//				pPlayer->bAutoMoveL = true;//左方向へ自動的に移動するか
			//			}


			//		}
			//		if (pPos->x + fWidth > g_aModel[nCntModel].pos.x - g_aModel[nCntModel].fRadius
			//			&& pPos->x - fWidth < g_aModel[nCntModel].pos.x + g_aModel[nCntModel].fRadius
			//			&& pPosOld->y - fHeight >= g_aModel[nCntModel].pos.y + 5.0f/* + g_aModel[nCntModel].fRadius*/
			//			&& pPos->y - fHeight <= g_aModel[nCntModel].pos.y + g_aModel[nCntModel].fRadius)
			//		{//PosOldの頭がブロックの下端より上で、プレイヤーのposがブロックの下端より下なら・・・



			//			pPos->y = g_aModel[nCntModel].pos.y + g_aModel[nCntModel].fRadius + fHeight + 6.0f;//プレイヤーの位置をブロックの下端に移動させる。
			//			pMove->y = 0.0f;//重力を０にする
			//			if (g_aModel[nCntModel].nType == BLOCKTYPE_FRAME && C_use == COLLISIONUSE_PLAYER)
			//			{
			//				DamageHealth(1);//１ダメージを与え続ける
			//			}
			//		}
			//	}
			//}
		}
	}
	return g_bIsLanding;

}
