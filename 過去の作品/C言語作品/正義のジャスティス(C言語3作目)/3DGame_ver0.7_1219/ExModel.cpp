////========================================================
////
////１１月７日：モデル読み込み[Model.cpp]
////Author:ShinaTaiyo
////
////=========================================================
//
//#include "model.h"
//#include "main.h"
//#include "input.h"
//#include "sound.h"
//#include "fade.h"
////グローバル変数
//LPD3DXMESH g_pMeshModel = NULL;//メッシュ（頂点情報）へのポインタ
//LPD3DXBUFFER g_pBuffMatModel = NULL;//マテリアルへのポインタ
//DWORD g_dwNumMatModel = 0;//マテリアルの数
//D3DXVECTOR3 g_posModel;//位置
//D3DXVECTOR3 g_rotModel;//向き
//D3DXMATRIX g_mtxWorldModel;//ワールドマトリックス
//#define Model_SCALE (10.0f)
//
////タイトル画面の初期化処理
//void InitModel(void)
//{
//	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスへのポインタと取得
//
//	g_posModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//	g_rotModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//	//モデルの読み込み
//	D3DXLoadMeshFromX("data\\MODEL\\bill_003.x",
//		D3DXMESH_SYSTEMMEM,
//		pDevice,
//		NULL,
//		&g_pBuffMatModel,
//		NULL,
//		&g_dwNumMatModel,
//		&g_pMeshModel);
//}
//
////==============================
////タイトル画面の終了処理
////==============================
//void UninitModel(void)
//{
//	//メッシュの破棄
//	if (g_pMeshModel != NULL)
//	{
//		g_pMeshModel->Release();
//		g_pMeshModel = NULL;
//	}
//
//	//マテリアルの破棄
//	if (g_pBuffMatModel != NULL)
//	{
//		g_pBuffMatModel->Release();
//		g_pBuffMatModel = NULL;
//	}
//}
////============================
////タイトル画面の更新処理
////============================
//void UpdateModel(void)
//{
//
//
//
//}
//
////================================
////タイトル画面の描画処理
////================================
//void DrawModel(void)
//{
//	LPDIRECT3DDEVICE9 pDevice = GetDevice();;//デバイスへのポインタと取得
//
//
//	D3DXMATRIX mtxRot, mtxTrans;//計算用マトリックス
//
//	D3DMATERIAL9 matDef;//現在のマテリアル保存用
//
//	D3DXMATERIAL* pMat;//マテリアルデータへのポインタ
//
//	//ワールドマトリックスの初期化
//	D3DXMatrixIdentity(&g_mtxWorldModel);
//
//	//向きを反映
//	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotModel.y, g_rotModel.x, g_rotModel.z);
//
//	D3DXMatrixMultiply(&g_mtxWorldModel, &g_mtxWorldModel, &mtxRot);
//
//	//位置を反映
//	D3DXMatrixTranslation(&mtxTrans, g_posModel.x, g_posModel.y, g_posModel.z);
//	D3DXMatrixMultiply(&g_mtxWorldModel, &g_mtxWorldModel, &mtxTrans);
//
//	//ワールドマトリックスの設定
//	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldModel);
//
//	//現在のマテリアルを取得
//	pDevice->GetMaterial(&matDef);
//
//	//マテリアルへのポインタを取得
//	pMat = (D3DXMATERIAL*)g_pBuffMatModel->GetBufferPointer();
//
//	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatModel; nCntMat++)
//	{
//		//マテリアルの設定
//		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
//
//		//テクスチャの設定
//		pDevice->SetTexture(0, NULL);
//
//		//モデル（パーツ）の描画
//		g_pMeshModel->DrawSubset(nCntMat);
//	}
//
//	//保存していたマテリアルを戻す
//	pDevice->SetMaterial(&matDef);
//}