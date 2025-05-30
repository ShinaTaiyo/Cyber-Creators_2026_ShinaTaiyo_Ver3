//================================================================================================================================================================================================================================
//
// オブジェクトの処理
// Author : Atsumu Kuboichi
//
//================================================================================================================================================================================================================================
#include "model.h"
#include "game.h"
#include "input.h"
#include <cstdio>
#include "edit.h"
#include "billboard.h"
#include "Rader.h"
#include "camera.h"
#include "particle.h"
#include "battery.h"
//================================================================================================================================================================================================================================
// グローバル変数
//================================================================================================================================================================================================================================
LPDIRECT3DTEXTURE9 g_apTextureModel[NUM_MODEL][MAX_MODELMAT] = {};	//テクスチャへのポインタ
Model g_aModel[MAX_MODEL];											//オブジェクト情報
int g_nUse;															//ゲームフィールドかちびロボハウス内かの判定(0か1)
bool g_bIsLandingModel;												//モデルの上にいるかどうかを判定する
int g_nMaxModel;													//オブジェクトのデータ
Model g_aModelInfo[MAX_TEXT];										//モデルの読み込み情報
int g_nCntModel = 0;												//モデルのデータ数
int g_nDelayModel = 0;												//ディレイ

//================================================================================================================================================================================================================================
// 初期化処理
//================================================================================================================================================================================================================================
void InitModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスの取得
	D3DXMATERIAL* pMat;						//マテリアルへのポインタ

	//構造体の情報
	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		g_aModel[nCntModel].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//位置
		g_aModel[nCntModel].posOld = NULL_VECTOR3;                  //1f前の位置
		g_aModel[nCntModel].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//移動量
		g_aModel[nCntModel].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//向き
		g_aModel[nCntModel].nType = 0;								//種類
		g_aModel[nCntModel].bUse = false;							//使用している
		g_aModel[nCntModel].nUse = -1;								//使用フィールド(0 = ゲームフィールド、1 = ちびロボハウス)
		g_aModel[nCntModel].nCntTime = 0;                           //様々なタイミングで時間を測る
		g_aModel[nCntModel].SavePos = NULL_VECTOR3;                 //初期位置を保存する
		g_aModel[nCntModel].nIdxRaderDisplay = -1;                  //レーダーディスプレイのインデックス

		//======================================
		//リフト限定処理
		//======================================
		g_aModel[nCntModel].bStopLift = false;	//リフトを止める
		g_aModel[nCntModel].bUpDownLift = false;//フラグの切り替えでリフトを上下させる
	}

	//変数
	g_nUse = 0;

	//=====================================
	//一瞬ゲーム画面が映し出される対策
	//=====================================
	if (GetMode() == MODE_TUTORIAL)
	{
		g_nUse = 1;
	}

	//==========================================================

	g_bIsLandingModel = false;
	g_nDelayModel = 0;//ディレイ

	for (int nCntType = 0; nCntType < NUM_MODEL; nCntType++)
	{
		//Xファイルの読み込み
		D3DXLoadMeshFromX(MODEL_FILENAME[nCntType], D3DXMESH_SYSTEMMEM, pDevice, NULL,
			&g_aModel[nCntType].pBuffMat, NULL, &g_aModel[nCntType].dwNumMat, &g_aModel[nCntType].pMesh);

		//マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_aModel[nCntType].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_aModel[nCntType].dwNumMat; nCntMat++)
		{
			//テクスチャファイルが存在する
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				//テクスチャの読み込み
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_apTextureModel[nCntType][nCntMat]);
			}
		}
	}
}

//================================================================================================================================================================================================================================
// 終了処理
//================================================================================================================================================================================================================================
void UninitModel(void)
{
	for (int nCntModel = 0; nCntModel < NUM_MODEL; nCntModel++)
	{
		//メッシュの破棄
		if (g_aModel[nCntModel].pMesh != NULL)
		{
			g_aModel[nCntModel].pMesh->Release();
			g_aModel[nCntModel].pMesh = NULL;
		}
		//マテリアルの破棄
		if (g_aModel[nCntModel].pBuffMat != NULL)
		{
			g_aModel[nCntModel].pBuffMat->Release();
			g_aModel[nCntModel].pBuffMat = NULL;
		}

		//テクスチャの破棄
		for (int nCntMat = 0; nCntMat < MAX_MODELMAT; nCntMat++)
		{
			if (g_apTextureModel[nCntModel][nCntMat] != NULL)
			{
				g_apTextureModel[nCntModel][nCntMat]->Release();
				g_apTextureModel[nCntModel][nCntMat] = NULL;
			}
		}
	}
}

//================================================================================================================================================================================================================================
// 更新処理
//================================================================================================================================================================================================================================
void UpdateModel(void)
{
	//if (GetMode() != MODE_TUTORIAL)
	//{//チュートリアル画面ではない場合
	//	g_nUse = 0;
	//}
	//else
	//{//チュートリアル画面の場合
	//	g_nUse = 1;
	//}

	if (GetMode() == MODE_GAME)
	{//ゲーム画面の場合
		g_nDelayModel++;//ディレイ

		if (g_nDelayModel == 1)
		{//ゲームが始まった時に一度だけ実装
			for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
			{
				//使用ステージが一致する場合
				if (g_aModel[nCntModel].nUse == g_nUse)
				{//使用する
					g_aModel[nCntModel].bUse = true;
				}
				//一致しない場合
				else
				{//使用しない
					g_aModel[nCntModel].bUse = false;
				}
			}
		}
	}
	else
	{//ゲーム画面ではない場合
		for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
		{
			//使用ステージが一致する場合
			if (g_aModel[nCntModel].nUse == g_nUse)
			{//使用する
				g_aModel[nCntModel].bUse = true;
			}
			//一致しない場合
			else
			{//使用しない
				g_aModel[nCntModel].bUse = false;
			}
		}
	}

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		if (g_aModel[nCntModel].bUse)
		{
			switch (g_aModel[nCntModel].nType)
			{
			case 18://リフト
				g_aModel[nCntModel].nCntTime++;
				
				//================================================
			    //一定の高さに達したら、リフトの動きを止める
			    //================================================
				if (g_aModel[nCntModel].bStopLift)
				{
					if (g_aModel[nCntModel].nCntTime == 100)
					{
						g_aModel[nCntModel].bStopLift = false;
						g_aModel[nCntModel].nCntTime = 0;
					}
				}
				
				//==================================
				//リフトの移動方向を変える
				//==================================
				if (g_aModel[nCntModel].pos.y > 300.0f + g_aModel[nCntModel].SavePos.y)
				{
					g_aModel[nCntModel].pos.y = 300.0f + g_aModel[nCntModel].SavePos.y;

					if (!g_aModel[nCntModel].bStopLift)
					{
						g_aModel[nCntModel].move = NULL_VECTOR3;
						g_aModel[nCntModel].bUpDownLift = g_aModel[nCntModel].bUpDownLift ? false : true;
						g_aModel[nCntModel].nCntTime = 0;
						g_aModel[nCntModel].bStopLift = true;
					}
				}
				else if (g_aModel[nCntModel].pos.y < g_aModel[nCntModel].SavePos.y)
				{
					g_aModel[nCntModel].pos.y = g_aModel[nCntModel].SavePos.y;

					if (!g_aModel[nCntModel].bStopLift)
					{
						g_aModel[nCntModel].move = NULL_VECTOR3;
						g_aModel[nCntModel].bUpDownLift = g_aModel[nCntModel].bUpDownLift ? false : true;
						g_aModel[nCntModel].nCntTime = 0;
						g_aModel[nCntModel].bStopLift = true;
					}
				}
				
				//============================
				//リフトを上下移動させる
				//============================
				if (!g_aModel[nCntModel].bStopLift)
				{
					if (!g_aModel[nCntModel].bUpDownLift)
					{
						g_aModel[nCntModel].move.y = 2.0f;
					}
					else
					{
						g_aModel[nCntModel].move.y = -2.0f;
					}
				}
				
				break;

			default:
				break;
			}

			if (g_aModel[nCntModel].nType == 19 || g_aModel[nCntModel].nType == 41)
			{
				if (g_nDelayModel == 1)
				{
					g_aModel[nCntModel].nIdxRaderDisplay = SetRaderDisplay(RADERDISPLAY03_OUTLET);
					SetPositionRaderDisPlay(g_aModel[nCntModel].nIdxRaderDisplay, g_aModel[nCntModel].pos,0.0f);
				}
			}

			//===================================
			//移動量の反映
			//===================================
			g_aModel[nCntModel].posOld = g_aModel[nCntModel].pos;
			g_aModel[nCntModel].pos += g_aModel[nCntModel].move;
		}
	}
}

//================================================================================================================================================================================================================================
// 描画処理
//================================================================================================================================================================================================================================
void DrawModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;			//計算用マトリックス
	D3DMATERIAL9 matDef;					//現在のマテリアル保存用
	D3DXMATERIAL* pMat;						//マテリアルのデータへのポインタ

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		//使用されている場合
		if (g_aModel[nCntModel].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aModel[nCntModel].mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aModel[nCntModel].rot.y, g_aModel[nCntModel].rot.x, g_aModel[nCntModel].rot.z);
			D3DXMatrixMultiply(&g_aModel[nCntModel].mtxWorld, &g_aModel[nCntModel].mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aModel[nCntModel].pos.x, g_aModel[nCntModel].pos.y, g_aModel[nCntModel].pos.z);
			D3DXMatrixMultiply(&g_aModel[nCntModel].mtxWorld, &g_aModel[nCntModel].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aModel[nCntModel].mtxWorld);

			//現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			//マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_aModel[g_aModel[nCntModel].nType].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aModel[g_aModel[nCntModel].nType].dwNumMat; nCntMat++)
			{
				//マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//テクスチャの設定
				pDevice->SetTexture(0, g_apTextureModel[g_aModel[nCntModel].nType][nCntMat]);

				//モデル(パーツ)の描画
				g_aModel[g_aModel[nCntModel].nType].pMesh->DrawSubset(nCntMat);
			}

			//保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//================================================================================================================================================================================================================================
// 設定処理
//================================================================================================================================================================================================================================
void SetModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nUse)
{
	BYTE* pVtxBuff;	//頂点バッファへのポインタ

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		if (!g_aModel[nCntModel].bUse)
		{//使用されていない場合
			g_aModel[nCntModel].pos = pos;		//位置
			g_aModel[nCntModel].SavePos = pos;	//初期位置を保存する
			g_aModel[nCntModel].rot = rot;		//向き
			g_aModel[nCntModel].nType = nType;	//種類
			g_aModel[nCntModel].nUse = nUse;	//使用フィールド
			g_aModel[nCntModel].bUse = true;	//使用する
			g_aModel[nCntModel].nCntTime = 0;   //様々なタイミングで時間を測る

			//当たり判定の有無
			if (nType == 2 || nType == 6 || nType == 19 || nType == 41)
			{//当たり判定無し
				g_aModel[nCntModel].bCollision = false;

				if (nType == 19 || nType == 41)
				{//充電器なら
					SetOutletDisp(D3DXVECTOR3(pos.x,pos.y + 80.0f,pos.z), nCntModel);
				}
			}
			else
			{//当たり判定あり
				g_aModel[nCntModel].bCollision = true;
			}

			//======================================
			//リフト限定処理
			//======================================
			g_aModel[nCntModel].bStopLift = false;  //リフトを止める
			g_aModel[nCntModel].bUpDownLift = false;//フラグの切り替えでリフトを上下させる

			//頂点数の取得
			int nNumVtx = g_aModel[nType].pMesh->GetNumVertices();

			//頂点フォーマットのサイズを取得
			DWORD sizeFVF = D3DXGetFVFVertexSize(g_aModel[nType].pMesh->GetFVF());

			//頂点バッファのロック
			g_aModel[nType].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

			for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
			{
				//頂点座標の代入
				D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

				//モデルの最小値と最大値を取得
				if (vtx.x < g_aModel[nCntModel].vtxMin.x)
				{//X座標の最小値
					g_aModel[nCntModel].vtxMin.x = vtx.x;
				}
				if (vtx.y < g_aModel[nCntModel].vtxMin.y)
				{//Y座標の最小値
					g_aModel[nCntModel].vtxMin.y = vtx.y;
				}
				if (vtx.z < g_aModel[nCntModel].vtxMin.z)
				{//Z座標の最小値
					g_aModel[nCntModel].vtxMin.z = vtx.z;
				}
				if (vtx.x > g_aModel[nCntModel].vtxMax.x)
				{//X座標の最大値
					g_aModel[nCntModel].vtxMax.x = vtx.x;
				}
				if (vtx.y > g_aModel[nCntModel].vtxMax.y)
				{//Y座標の最大値
					g_aModel[nCntModel].vtxMax.y = vtx.y;
				}
				if (vtx.z > g_aModel[nCntModel].vtxMax.z)
				{//Z座標の最大値
					g_aModel[nCntModel].vtxMax.z = vtx.z;
				}

				//頂点フォーマットのサイズ分ポインタを進める
				pVtxBuff += sizeFVF;
			}

			//頂点バッファのアンロック
			g_aModel[nType].pMesh->UnlockVertexBuffer();

			break;
		}
	}
}

//=======================================================================================================================================================================================================
// モデル情報の読み込み(txt)
//=======================================================================================================================================================================================================
void LoadModeltxt(void)
{
	int nCntModel = 0;
	char aDataSearch[MAX_TEXT];

	//ファイルを開く
	FILE* pFile = fopen(MODEL_TXT, "r");

	//ファイルが開けない場合
	if (pFile == NULL)
	{
		return;
	}

	//ENDが見つかるまで読み込みを繰り返す
	while (1)
	{
		int nResult = fscanf(pFile, "%s", aDataSearch);	//検索

		//EOFが見つかった場合
		if (nResult == EOF)
		{//読み込み終了
			fclose(pFile);
			break;
		}

		//コメントアウトが見つかった場合
		if (aDataSearch[0] == '#')
		{//スキップして読み込みを続ける
			continue;
		}

		//モデルセットが見つかった場合
		if (!strcmp(aDataSearch, "MODELSET"))
		{
			//項目ごとのデータを代入
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch);	//検索

				//ENDMODELセットが見つかった場合
				if (!strcmp(aDataSearch, "END_MODELSET"))
				{//モデルデータの読み込みを終了
					break;
				}

				//データの読み込み
				else if (!strcmp(aDataSearch, "POSx"))
				{//位置のX座標
					fscanf(pFile, "%f", &g_aModelInfo[nCntModel].pos.x);
				}
				else if (!strcmp(aDataSearch, "POSy"))
				{//位置のY座標
					fscanf(pFile, "%f", &g_aModelInfo[nCntModel].pos.y);
				}
				else if (!strcmp(aDataSearch, "POSz"))
				{//位置のZ座標
					fscanf(pFile, "%f", &g_aModelInfo[nCntModel].pos.z);
				}
				else if (!strcmp(aDataSearch, "ROTx"))
				{//向きのX座標
					fscanf(pFile, "%f", &g_aModelInfo[nCntModel].rot.x);
				}
				else if (!strcmp(aDataSearch, "ROTy"))
				{//向きのY座標
					fscanf(pFile, "%f", &g_aModelInfo[nCntModel].rot.y);
				}
				else if (!strcmp(aDataSearch, "ROTz"))
				{//向きのZ座標
					fscanf(pFile, "%f", &g_aModelInfo[nCntModel].rot.z);
				}
				else if (!strcmp(aDataSearch, "TYPE"))
				{//種類
					fscanf(pFile, "%d", &g_aModelInfo[nCntModel].nType);
				}
				else if (!strcmp(aDataSearch, "nUSE"))
				{//使用フィールド
					fscanf(pFile, "%d", &g_aModelInfo[nCntModel].nUse);
				}
			}

			nCntModel++;				//データ数を加算
			g_nCntModel = nCntModel;	//データ数を保存
		}
	}

	for (int nCntSet = 0; nCntSet < g_nCntModel; nCntSet++)
	{
		//==================================================================================================================================================================================
		// オブジェクトの設定
		//==================================================================================================================================================================================
		SetModel(g_aModelInfo[nCntSet].pos, g_aModelInfo[nCntSet].rot, g_aModelInfo[nCntSet].nType, g_aModelInfo[nCntSet].nUse);

		//机(生徒用)の場合
		if (g_aModelInfo[nCntSet].nType == 0)
		{
			//机の脚を設定
			SetModel(D3DXVECTOR3(g_aModelInfo[nCntSet].pos.x - CREATE_POSX_TYPE1, g_aModelInfo[nCntSet].pos.y, g_aModelInfo[nCntSet].pos.z), g_aModelInfo[nCntSet].rot, 1, g_aModelInfo[nCntSet].nUse);
			SetModel(D3DXVECTOR3(g_aModelInfo[nCntSet].pos.x + CREATE_POSX_TYPE1, g_aModelInfo[nCntSet].pos.y, g_aModelInfo[nCntSet].pos.z), g_aModelInfo[nCntSet].rot, 1, g_aModelInfo[nCntSet].nUse);
		}

		//椅子(生徒用)の場合
		else if (g_aModelInfo[nCntSet].nType == 2)
		{
			//椅子の脚を設定
			SetModel(D3DXVECTOR3(g_aModelInfo[nCntSet].pos.x - CREATE_POSX_TYPE3, g_aModelInfo[nCntSet].pos.y, g_aModelInfo[nCntSet].pos.z), g_aModelInfo[nCntSet].rot, 3, g_aModelInfo[nCntSet].nUse);
			SetModel(D3DXVECTOR3(g_aModelInfo[nCntSet].pos.x + CREATE_POSX_TYPE3, g_aModelInfo[nCntSet].pos.y, g_aModelInfo[nCntSet].pos.z), g_aModelInfo[nCntSet].rot, 3, g_aModelInfo[nCntSet].nUse);
		}

		//教卓の場合
		else if (g_aModelInfo[nCntSet].nType == 4)
		{
			//教卓の脚を設定
			SetModel(D3DXVECTOR3(g_aModelInfo[nCntSet].pos.x - CREATE_POSX_TYPE5, g_aModelInfo[nCntSet].pos.y, g_aModelInfo[nCntSet].pos.z), g_aModelInfo[nCntSet].rot, 5, g_aModelInfo[nCntSet].nUse);
			SetModel(D3DXVECTOR3(g_aModelInfo[nCntSet].pos.x + CREATE_POSX_TYPE5, g_aModelInfo[nCntSet].pos.y, g_aModelInfo[nCntSet].pos.z), g_aModelInfo[nCntSet].rot, 5, g_aModelInfo[nCntSet].nUse);
		}

		//椅子(先生用)の場合
		else if (g_aModelInfo[nCntSet].nType == 6)
		{
			//椅子(先生用)の脚を設定
			SetModel(D3DXVECTOR3(g_aModelInfo[nCntSet].pos.x - CREATE_POSX_TYPE7, g_aModelInfo[nCntSet].pos.y, g_aModelInfo[nCntSet].pos.z), g_aModelInfo[nCntSet].rot, 7, g_aModelInfo[nCntSet].nUse);
			SetModel(D3DXVECTOR3(g_aModelInfo[nCntSet].pos.x + CREATE_POSX_TYPE7, g_aModelInfo[nCntSet].pos.y, g_aModelInfo[nCntSet].pos.z), g_aModelInfo[nCntSet].rot, 8, g_aModelInfo[nCntSet].nUse);
		}

		//==================================================================================================================================================================================
		// エディタの配置物の設定
		//==================================================================================================================================================================================
		SetEdit(g_aModelInfo[nCntSet].pos, g_aModelInfo[nCntSet].rot, g_aModelInfo[nCntSet].nType, g_aModelInfo[nCntSet].nUse);
	}
}

//=======================================================================================================================================================================================================================
// モデル情報の読み込み(bin)
//=======================================================================================================================================================================================================================
void LoadModelbin(void)
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 rot;	//向き
	int nType;			//種類
	int nUse;			//使用フィールド

	for (int nCntObj = 0; nCntObj < NUM_MODEL; nCntObj++)
	{
		//ファイルを開く
		FILE* pFile = fopen(MODEL_BIN, "rb");

		//オブジェクト情報の読み込み
		if (pFile != NULL)
		{
			//ファイルの読み込み
			fread(&g_nMaxModel, sizeof(int), 1, pFile);

			for (int nCnt = 1; nCnt < g_nMaxModel; nCnt++)
			{
				//座標の取得
				fread(&pos, sizeof(D3DXVECTOR3), 1, pFile);

				//向きの取得
				fread(&rot, sizeof(D3DXVECTOR3), 1, pFile);

				//タイプの取得
				fread(&nType, sizeof(int), 1, pFile);

				//使用フィールドの取得
				fread(&nUse, sizeof(int), 1, pFile);

				//オブジェクトの設定
				SetModel(pos, rot, nType, nUse);

				//机(生徒用)の場合
				if (nType == 0)
				{
					//机の脚を設定
					SetModel(D3DXVECTOR3(pos.x - CREATE_POSX_TYPE1, pos.y, pos.z), rot, 1, nUse);
					SetModel(D3DXVECTOR3(pos.x + CREATE_POSX_TYPE1, pos.y, pos.z), rot, 1, nUse);
				}

				//椅子(生徒用)の場合
				else if (nType == 2)
				{
					//椅子の脚を設定
					SetModel(D3DXVECTOR3(pos.x - CREATE_POSX_TYPE3, pos.y, pos.z), rot, 3, nUse);
					SetModel(D3DXVECTOR3(pos.x + CREATE_POSX_TYPE3, pos.y, pos.z), rot, 3, nUse);
				}

				//教卓の場合
				else if (nType == 4)
				{
					//教卓の脚を設定
					SetModel(D3DXVECTOR3(pos.x - CREATE_POSX_TYPE5, pos.y, pos.z), rot, 5, nUse);
					SetModel(D3DXVECTOR3(pos.x + CREATE_POSX_TYPE5, pos.y, pos.z), rot, 5, nUse);
				}

				//椅子(先生用)の場合
				else if (nType == 6)
				{
					//椅子(先生用)の脚を設定
					SetModel(D3DXVECTOR3(pos.x - CREATE_POSX_TYPE7, pos.y, pos.z), rot, 7, nUse);
					SetModel(D3DXVECTOR3(pos.x + CREATE_POSX_TYPE7, pos.y, pos.z), rot, 8, nUse);
				}
			}

			//ファイルを閉じる
			fclose(pFile);
		}
		else
		{
			return;
		}
	}
}

//================================================================================================================================================================================================================================
// 当たり判定処理
//================================================================================================================================================================================================================================
bool bCollisionModel(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 Size, int& nNumLandingModel)
{
	g_bIsLandingModel = false;	//着地しているかどうか

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		if (g_aModel[nCntModel].bUse && g_aModel[nCntModel].bCollision)
		{//モデルを使用していて、当たり判定がある場合

			//上
			if (pPos->x + Size.x / 2 >= g_aModel[nCntModel].pos.x + g_aModel[nCntModel].vtxMin.x
				&& pPos->x - Size.x / 2 <= g_aModel[nCntModel].pos.x + g_aModel[nCntModel].vtxMax.x
				&& pPos->y <= g_aModel[nCntModel].pos.y + g_aModel[nCntModel].vtxMax.y
				&& pPosOld->y + 5.0f > g_aModel[nCntModel].pos.y + g_aModel[nCntModel].vtxMax.y
				&& pPos->z + Size.z / 2 >= g_aModel[nCntModel].pos.z + g_aModel[nCntModel].vtxMin.z
				&& pPos->z - Size.z / 2 <= g_aModel[nCntModel].pos.z + g_aModel[nCntModel].vtxMax.z)
			{//対象の下端がモデルの+Yに当たった時の処理
				if (g_aModel[nCntModel].nType == 18 || g_aModel[nCntModel].nType == 55)
				{
					g_aModel[nCntModel].vtxMax.y = 1.1f;
				}
				pPos->y = g_aModel[nCntModel].pos.y + g_aModel[nCntModel].vtxMax.y;
				g_bIsLandingModel = true;
				nNumLandingModel = nCntModel;//今乗っているモデルの番号を引数に返す
			}

			//下
			else if (pPos->x + Size.x / 2 >= g_aModel[nCntModel].pos.x + g_aModel[nCntModel].vtxMin.x
				&& pPos->x - Size.x / 2 <= g_aModel[nCntModel].pos.x + g_aModel[nCntModel].vtxMax.x
				&& pPos->y + Size.y >= g_aModel[nCntModel].pos.y + g_aModel[nCntModel].vtxMin.y
				&& pPosOld->y + Size.y < g_aModel[nCntModel].pos.y + g_aModel[nCntModel].vtxMin.y
				&& pPos->z + Size.z / 2 >= g_aModel[nCntModel].pos.z + g_aModel[nCntModel].vtxMin.z
				&& pPos->z - Size.z / 2 <= g_aModel[nCntModel].pos.z + g_aModel[nCntModel].vtxMax.z)
			{//対象の下端がモデルの+Yに当たった時の処理
				pPos->y = g_aModel[nCntModel].pos.y + g_aModel[nCntModel].vtxMin.y - Size.y;
			}

			//左
			else if (pPos->x + Size.x / 2 > g_aModel[nCntModel].pos.x + g_aModel[nCntModel].vtxMin.x//対象の最大X座標が、モデルの最小X座標より上
				&& pPosOld->x + Size.x / 2 <= g_aModel[nCntModel].pos.x + g_aModel[nCntModel].vtxMin.x//対象の1F前の最大X座標が、モデルの最小X座標より下
				&& pPos->y + Size.y >= g_aModel[nCntModel].pos.y + g_aModel[nCntModel].vtxMin.y//対象の最大Y座標がモデルの最小Y座標より上
				&& pPos->y <= g_aModel[nCntModel].pos.y + g_aModel[nCntModel].vtxMax.y
				&& pPos->z + Size.z / 2 >= g_aModel[nCntModel].pos.z + g_aModel[nCntModel].vtxMin.z//対象の最大X座標が、モデルの最小X座標より上
				&& pPos->z - Size.z / 2 <= g_aModel[nCntModel].pos.z + g_aModel[nCntModel].vtxMax.z)//対象の1F前の最大X座標が、モデルの最小X座標より下)//対象の最小Y座標がモデルの最大Y座標より下
			{//対象がブロックの-Xに当たった時の処理
				pPos->x = g_aModel[nCntModel].pos.x + g_aModel[nCntModel].vtxMin.x - Size.x / 2;

				if (g_aModel[nCntModel].nType == 18 || g_aModel[nCntModel].nType == 55)
				{//当たったモデルが「リフト」だったら
					pPos->y = g_aModel[nCntModel].pos.y + g_aModel[nCntModel].vtxMax.y + 1.0f;//リフトの上に乗るようにする
					nNumLandingModel = nCntModel;
				}
			}

			//右
			else if (pPos->x - Size.x / 2 < g_aModel[nCntModel].pos.x + g_aModel[nCntModel].vtxMax.x
				&& pPosOld->x - Size.x / 2 >= g_aModel[nCntModel].pos.x + g_aModel[nCntModel].vtxMax.x
				&& pPos->y + Size.y >= g_aModel[nCntModel].pos.y + g_aModel[nCntModel].vtxMin.y
				&& pPos->y <= g_aModel[nCntModel].pos.y + g_aModel[nCntModel].vtxMax.y
				&& pPos->z + Size.z / 2 >= g_aModel[nCntModel].pos.z + g_aModel[nCntModel].vtxMin.z
				&& pPos->z - Size.z / 2 <= g_aModel[nCntModel].pos.z + g_aModel[nCntModel].vtxMax.z)
			{//対象がブロックの+Xに当たった時の処理
				pPos->x = g_aModel[nCntModel].pos.x + g_aModel[nCntModel].vtxMax.x + Size.x / 2;

				if (g_aModel[nCntModel].nType == 18 || g_aModel[nCntModel].nType == 55)
				{//当たったモデルが「リフト」だったら
					pPos->y = g_aModel[nCntModel].pos.y + g_aModel[nCntModel].vtxMax.y + 1.0f;//リフトの上に乗るようにする
					nNumLandingModel = nCntModel;
				}
			}

			//前
			else if (pPos->x + Size.x / 2 >= g_aModel[nCntModel].pos.x + g_aModel[nCntModel].vtxMin.x
				&& pPos->x - Size.x / 2 <= g_aModel[nCntModel].pos.x + g_aModel[nCntModel].vtxMax.x
				&& pPos->y + Size.y >= g_aModel[nCntModel].pos.y + g_aModel[nCntModel].vtxMin.y
				&& pPos->y <= g_aModel[nCntModel].pos.y + g_aModel[nCntModel].vtxMax.y
				&& pPos->z - Size.z / 2 < g_aModel[nCntModel].pos.z + g_aModel[nCntModel].vtxMax.z
				&& pPosOld->z - Size.z / 2 >= g_aModel[nCntModel].pos.z + g_aModel[nCntModel].vtxMax.z)
			{//対象がブロックの+Xに当たった時の処理
				pPos->z = g_aModel[nCntModel].pos.z + g_aModel[nCntModel].vtxMax.z + Size.z / 2;

				if (g_aModel[nCntModel].nType == 18 || g_aModel[nCntModel].nType == 55)
				{//当たったモデルが「リフト」だったら
					pPos->y = g_aModel[nCntModel].pos.y + g_aModel[nCntModel].vtxMax.y + 1.0f;//リフトの上に乗るようにする
					nNumLandingModel = nCntModel;
				}
			}

			//後ろ
			else if (pPos->x + Size.x / 2 >= g_aModel[nCntModel].pos.x + g_aModel[nCntModel].vtxMin.x
				&& pPos->x - Size.x / 2 <= g_aModel[nCntModel].pos.x + g_aModel[nCntModel].vtxMax.x
				&& pPos->y + Size.y >= g_aModel[nCntModel].pos.y + g_aModel[nCntModel].vtxMin.y
				&& pPos->y <= g_aModel[nCntModel].pos.y + g_aModel[nCntModel].vtxMax.y
				&& pPos->z + Size.z / 2 > g_aModel[nCntModel].pos.z + g_aModel[nCntModel].vtxMin.z
				&& pPosOld->z + Size.z / 2 <= g_aModel[nCntModel].pos.z + g_aModel[nCntModel].vtxMin.z)
			{//対象がブロックの+Xに当たった時の処理
				pPos->z = g_aModel[nCntModel].pos.z + g_aModel[nCntModel].vtxMin.z - Size.z / 2;

				if (g_aModel[nCntModel].nType == 18 || g_aModel[nCntModel].nType == 55)
				{//当たったモデルが「リフト」だったら
					pPos->y = g_aModel[nCntModel].pos.y + g_aModel[nCntModel].vtxMax.y + 1.0f;//リフトの上に乗るようにする
					nNumLandingModel = nCntModel;
				}
			}
		}
	}

	return g_bIsLandingModel;
}

//================================================================================================================================================================================================================================
// 情報の取得
//================================================================================================================================================================================================================================
Model* GetModel(void)
{
	return &g_aModel[0];
}
