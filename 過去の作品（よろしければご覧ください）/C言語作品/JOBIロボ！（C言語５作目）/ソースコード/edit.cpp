//=======================================================================================================================================================================================================================
//
// エディタの処理
// Author : Atsumu Kuboichi
//
//=======================================================================================================================================================================================================================
#include "edit.h"
#include <stdio.h>
#include "input.h"
#include "debugproc.h"
#include "game.h"
#include "editcamera.h"

//=======================================================================================================================================================================================================================
// グローバル変数
//=======================================================================================================================================================================================================================
Edit g_edit[MAX_MODEL];									//配置オブジェクトの情報
LPD3DXMESH g_EditMesh[NUM_EDITTYPE][NUM_CLASS];			//メッシュの(頂点情報)へのポインタ
LPD3DXBUFFER  g_EditBuffMat[NUM_EDITTYPE][NUM_CLASS];	//マテリアルのポインタ
DWORD  g_EditdwNumMat[NUM_EDITTYPE][NUM_CLASS];			//マテリアルの数
int g_nCntEdit;	//配置中のモデルの番号
int g_nEditUse;	//現在の画面

//=======================================================================================================================================================================================================================
// 初期化処理
//=======================================================================================================================================================================================================================
void InitEdit(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスの取得
	D3DXMATERIAL* pMat;						//マテリアルのデータへのポインタ

	//情報
	for (int nCntEdit = 0; nCntEdit < MAX_MODEL; nCntEdit++)
	{
		g_edit[nCntEdit].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//位置
		g_edit[nCntEdit].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//向き
		g_edit[nCntEdit].nType = 0;								//種類
		g_edit[nCntEdit].nClass = 0;							//配置物の種類
		g_edit[nCntEdit].nUse = -1;								//使用フィールド
		g_edit[nCntEdit].bBuild = false;						//配置済みかどうか

		//最初だけ使用する
		if (nCntEdit == 0)
		{//使用する
			g_edit[nCntEdit].bUse = true;
		}
		else
		{//使用しない
			g_edit[nCntEdit].bUse = false;
		}
	}

	//=====================================================================================================
	// Xファイルの読み込み
	//=====================================================================================================
	for (int nCntClass = 0; nCntClass < NUM_CLASS; nCntClass++)
	{
		//モデル
		if (nCntClass == 0)
		{
			for (int nCntModel = 0; nCntModel < NUM_MODEL; nCntModel++)
			{
				//Xファイルの読み込み
				D3DXLoadMeshFromX(MODEL_FILENAME[nCntModel], D3DXMESH_SYSTEMMEM, pDevice, NULL,
					&g_EditBuffMat[nCntModel][nCntClass], NULL, &g_EditdwNumMat[nCntModel][nCntClass], &g_EditMesh[nCntModel][nCntClass]);

				//マテリアルデータへのポインタを取得
				pMat = (D3DXMATERIAL*)g_EditBuffMat[nCntModel][nCntClass]->GetBufferPointer();

				//マテリアルデータを保存
				for (int nCntMat = 0; nCntMat < (int)g_EditdwNumMat[nCntModel][nCntClass]; nCntMat++)
				{
					g_edit[nCntModel].Diffuse[nCntMat] = pMat[nCntMat].MatD3D.Diffuse;
				}
			}
		}
	}

	//変数
	g_nCntEdit = 0;//配置中のモデルの番号
	g_nEditUse = 0;//現在の画面
}

//=======================================================================================================================================================================================================================
// 終了処理
//=======================================================================================================================================================================================================================
void UninitEdit(void)
{
	for (int nCntClass = 0; nCntClass < NUM_CLASS; nCntClass++)
	{
		for (int nCntEdit = 0; nCntEdit < NUM_EDITTYPE; nCntEdit++)
		{
			//メッシュの破棄
			if (g_EditMesh[nCntEdit][nCntClass] != NULL)
			{
				g_EditMesh[nCntEdit][nCntClass]->Release();
				g_EditMesh[nCntEdit][nCntClass] = NULL;
			}
			//マテリアルの破棄
			if (g_EditBuffMat[nCntEdit][nCntClass] != NULL)
			{
				g_EditBuffMat[nCntEdit][nCntClass]->Release();
				g_EditBuffMat[nCntEdit][nCntClass] = NULL;
			}
		}
	}
}

//=======================================================================================================================================================================================================================
// 更新処理
//=======================================================================================================================================================================================================================
void UpdateEdit(void)
{
	//操作方法表示
	PrintDebugProc("==============================\n 操作 \n==============================\n");
	PrintDebugProc("移動\nX軸移動:T,G　Y軸移動:Y,H　Z軸移動:U,J\n\n");
	PrintDebugProc("%fずつ移動\nX軸:1,2　Y軸:3,4　Z軸:5,6\n\n", EDIT_SPEED);
	PrintDebugProc("回転\n左回転:Z　右回転:C\n\n");
	PrintDebugProc("位置と向きをリセット:R\n\n");
	PrintDebugProc("カメラ\n移動:WASD　回転:←↑↓→\n\n");
	PrintDebugProc("種類を変更:Q,E\n");
	PrintDebugProc("配置:B\n");
	PrintDebugProc("編集対象変更:N,M\n");
	PrintDebugProc("ステージ切り替え:F6\n");
	PrintDebugProc("削除:BACK SPACE\n");
	PrintDebugProc("全リセット:0\n");
	PrintDebugProc("書き出し:F3\n\n");

	//配置物情報表示
	PrintDebugProc("==============================\n 配置物情報 \n==============================\n");
	PrintDebugProc("POS\nX:%f　Y:%f　Z:%f\n\n", g_edit[g_nCntEdit].pos.x, g_edit[g_nCntEdit].pos.y, g_edit[g_nCntEdit].pos.z);	//位置
	PrintDebugProc("ROT\nX:%f　Y:%f  Z:%f\n\n", g_edit[g_nCntEdit].rot.x, g_edit[g_nCntEdit].rot.y, g_edit[g_nCntEdit].rot.z);	//向き
	PrintDebugProc("TYPE:%d\n\n", g_edit[g_nCntEdit].nType);																	//種類
	PrintDebugProc("配置数:%d/%d\n", g_nCntEdit, MAX_MODEL);																	//配置数

	if (g_edit[g_nCntEdit].bUse)
	{//配置物が使用されている場合
		//=======================================================================
		// 移動(プレス)
		//======================================================================= 
		if (GetKeyboardPress(DIK_T)) //T
		{
			g_edit[g_nCntEdit].pos.x -= EDIT_SPEED;
		}
		else if (GetKeyboardPress(DIK_G)) //G
		{
			g_edit[g_nCntEdit].pos.x += EDIT_SPEED;
		}
		else if (GetKeyboardPress(DIK_Y)) //Y
		{
			g_edit[g_nCntEdit].pos.y += EDIT_SPEED;
		}
		else if (GetKeyboardPress(DIK_H)) //H
		{
			g_edit[g_nCntEdit].pos.y -= EDIT_SPEED;
		}
		else if (GetKeyboardPress(DIK_U)) //U
		{
			g_edit[g_nCntEdit].pos.z += EDIT_SPEED;
		}
		else if (GetKeyboardPress(DIK_J)) //J
		{
			g_edit[g_nCntEdit].pos.z -= EDIT_SPEED;
		}

		//=======================================================================
		// 移動(トリガー)
		//=======================================================================
		if (GetKeyboardTrigger(DIK_1)) //1
		{
			g_edit[g_nCntEdit].pos.x -= EDIT_SPEED;
		}
		else if (GetKeyboardTrigger(DIK_2)) //2
		{
			g_edit[g_nCntEdit].pos.x += EDIT_SPEED;
		}
		else if (GetKeyboardTrigger(DIK_3)) //3
		{
			g_edit[g_nCntEdit].pos.y += EDIT_SPEED;
		}
		else if (GetKeyboardTrigger(DIK_4)) //4
		{
			g_edit[g_nCntEdit].pos.y -= EDIT_SPEED;
		}
		else if (GetKeyboardTrigger(DIK_5)) //5
		{
			g_edit[g_nCntEdit].pos.z += EDIT_SPEED;
		}
		else if (GetKeyboardTrigger(DIK_6)) //6
		{
			g_edit[g_nCntEdit].pos.z -= EDIT_SPEED;
		}

		//=======================================================================
		// 回転
		//=======================================================================
		if (GetKeyboardTrigger(DIK_C))
		{//Cキーが押された場合
			g_edit[g_nCntEdit].rot.y -= EDIT_TURN;//左回転

			//角度の補正
			if (g_edit[g_nCntEdit].rot.y <= -D3DX_PI)
			{
				g_edit[g_nCntEdit].rot.y = D3DX_PI;
			}
		}
		else if (GetKeyboardTrigger(DIK_V))
		{//Vキーが押された場合
			g_edit[g_nCntEdit].rot.y += EDIT_TURN;//右回転

			//角度の補正
			if (g_edit[g_nCntEdit].rot.y >= D3DX_PI)
			{
				g_edit[g_nCntEdit].rot.y = -D3DX_PI;
			}
		}

		//=======================================================================
		// 位置と向きをリセット
		//=======================================================================
		if (GetKeyboardTrigger(DIK_R))
		{//Rキーが押された場合
			g_edit[g_nCntEdit].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_edit[g_nCntEdit].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}

		//=======================================================================
		// 種類変更
		//=======================================================================
		if (GetKeyboardTrigger(DIK_Q))
		{//Qキーが押された場合
			if (g_edit[g_nCntEdit].nType > 0)
			{
				g_edit[g_nCntEdit].nType--;
			}
		}
		else if (GetKeyboardTrigger(DIK_E))
		{//Eキーが押された場合
			if (g_edit[g_nCntEdit].nType < NUM_EDITTYPE - 1)
			{
				g_edit[g_nCntEdit].nType++;
			}
		}

		//=======================================================================
		// 配置
		//=======================================================================
		if (GetKeyboardTrigger(DIK_B))
		{//Bキーが押された場合
			g_edit[g_nCntEdit].bBuild = true;						//配置する
			g_edit[g_nCntEdit + 1].bUse = true;						//次のデータを使用する
			g_edit[g_nCntEdit + 1].pos = g_edit[g_nCntEdit].pos;	//次のデータに現在のデータの位置を反映
			g_edit[g_nCntEdit + 1].rot = g_edit[g_nCntEdit].rot;	//次のデータに現在のデータの位置を反映
			g_edit[g_nCntEdit + 1].nType = g_edit[g_nCntEdit].nType;//次のデータに現在のデータのタイプを反映

			g_nCntEdit++;	//配置数を加算
		}
	}

	//==================================================
	// 編集対象変更
	//==================================================
	if (GetKeyboardTrigger(DIK_N))
	{//Nキーが押された場合
		while (1)
		{
			g_nCntEdit--;//次のオブジェクトを対象にする

			if (g_nCntEdit < 0)
			{//カウントがオブジェクトの最大数に到達した場合
				g_nCntEdit = MAX_MODEL;
			}

			if (g_edit[g_nCntEdit].bUse)
			{//次のオブジェクトが使用されている場合
				break;
			}
		}

		SetPosEditCamera(g_nCntEdit);//カメラの位置を対象のオブジェクトに合わせて変更
	}
	else if (GetKeyboardTrigger(DIK_M))
	{//Mキーが押された場合
		while (1)
		{
			g_nCntEdit++;//次のオブジェクトを対象にする

			if (g_nCntEdit > MAX_MODEL)
			{//カウントがオブジェクトの最大数に到達した場合
				g_nCntEdit = 0;
			}

			if (g_edit[g_nCntEdit].bUse)
			{//次のオブジェクトが使用されている場合
				break;
			}
		}

		//カメラの位置を対象のオブジェクトに合わせて変更
		SetPosEditCamera(g_nCntEdit);
	}

	//=======================================================================
	// 削除
	//=======================================================================
	if (GetKeyboardTrigger(DIK_BACK))
	{//BACK SPACEが押された場合
		g_edit[g_nCntEdit].bUse = false;	//現在のデータを使用しない
		g_edit[g_nCntEdit].bBuild = false;	//配置されていない
		g_nCntEdit--;

		//カメラの位置を対象のオブジェクトに合わせて変更
		SetPosEditCamera(g_nCntEdit);
	}

	//=======================================================================
	// 全データリセット
	//=======================================================================
	if (GetKeyboardTrigger(DIK_0))
	{//0キーが押された場合
		//全情報初期化
		for (int nCntEdit = 0; nCntEdit < MAX_MODEL; nCntEdit++)
		{
			g_edit[nCntEdit].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//位置
			g_edit[nCntEdit].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//向き
			g_edit[nCntEdit].nType = 0;								//種類
			g_edit[nCntEdit].nClass = 0;							//配置物の種類
			g_edit[nCntEdit].nUse = -1;								//使用フィールド
			g_edit[nCntEdit].bBuild = false;						//配置済みかどうか

			//最初だけ使用する
			if (nCntEdit == 0)
			{//使用する
				g_edit[nCntEdit].bUse = true;
			}
			else
			{//使用しない
				g_edit[nCntEdit].bUse = false;
			}
		}

		g_nCntEdit = 0;//配置数を0に初期化
	}

	//=======================================================================
	// ゲームorチュートリアルの判定
	//=======================================================================
	if (!GetHouse())
	{//ゲーム
		g_edit[g_nCntEdit].nUse = 0;
		g_nEditUse = 0;
	}
	else
	{//チュートリアル
		g_edit[g_nCntEdit].nUse = 1;
		g_nEditUse = 1;
	}

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		if (g_edit[nCntModel].bBuild)
		{
			//使用ステージが一致する場合
			if (g_edit[nCntModel].nUse == g_nEditUse)
			{//使用する
				g_edit[nCntModel].bUse = true;
			}
			//一致しない場合
			else
			{//使用しない
				g_edit[nCntModel].bUse = false;
			}
		}
	}

	//=======================================================================
	// 情報の書き出し
	//=======================================================================
	WriteModel();//書き出し処理
}

//=======================================================================================================================================================================================================================
// 描画処理
//=======================================================================================================================================================================================================================
void DrawEdit(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;			//計算用マトリックス
	D3DMATERIAL9 matDef;					//現在のマテリアル保存用
	D3DXMATERIAL* pMat;						//マテリアルのデータへのポインタ

	for (int nCntEdit = 0; nCntEdit < MAX_MODEL; nCntEdit++)
	{
		//使用されている場合
		if (g_edit[nCntEdit].bUse)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_edit[nCntEdit].mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_edit[nCntEdit].rot.y, g_edit[nCntEdit].rot.x, g_edit[nCntEdit].rot.z);
			D3DXMatrixMultiply(&g_edit[nCntEdit].mtxWorld, &g_edit[nCntEdit].mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_edit[nCntEdit].pos.x, g_edit[nCntEdit].pos.y, g_edit[nCntEdit].pos.z);
			D3DXMatrixMultiply(&g_edit[nCntEdit].mtxWorld, &g_edit[nCntEdit].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_edit[nCntEdit].mtxWorld);

			//現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			//マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_EditBuffMat[g_edit[nCntEdit].nType][g_edit[nCntEdit].nClass]->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_EditdwNumMat[g_edit[nCntEdit].nType][g_edit[nCntEdit].nClass]; nCntMat++)
			{
				//編集対象のオブジェクトの場合
				if (nCntEdit == g_nCntEdit)
				{
					//モデルの色を変化
					pMat[nCntMat].MatD3D.Diffuse.a = 0.5f;
				}
				//編集対象のオブジェクトではない場合
				else if(nCntEdit != g_nCntEdit)
				{
					//モデルの色を戻す
					pMat[nCntMat].MatD3D.Diffuse.a = 1.0f;
				}

				//マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//テクスチャの設定
				pDevice->SetTexture(0, NULL);

				//モデル(パーツ)の描画
				g_EditMesh[g_edit[nCntEdit].nType][g_edit[nCntEdit].nClass]->DrawSubset(nCntMat);
			}

			//保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//=======================================================================================================================================================================================================================
// モデル情報の書き出し処理
//=======================================================================================================================================================================================================================
void WriteModel(void)
{
	FILE* pFile{};
	int nCntModel = 0;

	//====================================================================================================
	// 情報を書き出す
	//====================================================================================================
	if (GetKeyboardTrigger(DIK_F3))	//F3キーが押された場合
	{
		//================================================================================
		// txtへの書き出し
		//================================================================================

		//ファイルを開く
		pFile = fopen(MODEL_TXT, "w");

		if (pFile != NULL)
		{
			//ファイルの中身をリセット
			for (int nCnt = 0; nCnt < NUM_MODEL; nCnt++)
			{
				fprintf(pFile, "# TYPE %d : %s\n", nCnt, MODEL_FILENAME[nCnt]);
			}
			fprintf(pFile, "\n");

			for (int nCnt = 0; nCnt < MAX_MODEL; nCnt++)
			{
				//使用されている場合
				if (g_edit[nCnt].bBuild)
				{
					//データの記録
					fprintf(pFile, "# model%d\n", nCntModel);//データの番号
					fprintf(pFile, "MODELSET\n");
					fprintf(pFile, "	POSx %f\n", g_edit[nCnt].pos.x);//X座標
					fprintf(pFile, "	POSy %f\n", g_edit[nCnt].pos.y);//Y座標
					fprintf(pFile, "	POSz %f\n", g_edit[nCnt].pos.z);//Z座標
					fprintf(pFile, "	ROTx %f\n", g_edit[nCnt].rot.x);//X軸の向き
					fprintf(pFile, "	ROTy %f\n", g_edit[nCnt].rot.y);//Y軸の向き
					fprintf(pFile, "	ROTz %f\n", g_edit[nCnt].rot.z);//Z軸の向き
					fprintf(pFile, "	TYPE %d\n", g_edit[nCnt].nType);//種類
					fprintf(pFile, "	nUSE %d\n", g_edit[nCnt].nUse);	//使用フィールド
					fprintf(pFile, "END_MODELSET\n\n");

					nCntModel++;
				}
			}

			//ファイルを閉じる
			fclose(pFile);
		}

		//================================================================================
		// binへの書き出し
		//================================================================================

		//ファイルを開く
		pFile = fopen(MODEL_BIN, "wb");

		if (pFile != NULL)
		{
			//配置数を書き出す
			fwrite(&g_nCntEdit, sizeof(int), 1, pFile);

			for (int nCnt = 0; nCnt < MAX_MODEL; nCnt++)
			{
				//使用されている場合
				if (g_edit[nCnt].bBuild)
				{
					//データの記録
					fwrite(&g_edit[nCnt].pos, sizeof(D3DXVECTOR3), 1, pFile);	//位置
					fwrite(&g_edit[nCnt].rot, sizeof(D3DXVECTOR3), 1, pFile);	//向き
					fwrite(&g_edit[nCnt].nType, sizeof(int), 1, pFile);			//種類
					fwrite(&g_edit[nCnt].nUse, sizeof(int), 1, pFile);			//使用フィールド
				}
			}

			//ファイルを閉じる
			fclose(pFile);
		}
	}
}

//================================================================================================================================================================================================================================
// 設定処理
//================================================================================================================================================================================================================================
void SetEdit(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nUse)
{
	//読み込んだデータを設定
	g_edit[g_nCntEdit].pos = pos;		//位置
	g_edit[g_nCntEdit].rot = rot;		//向き
	g_edit[g_nCntEdit].nType = nType;	//種類
	g_edit[g_nCntEdit].nUse = nUse;		//使用フィールド
	g_edit[g_nCntEdit].bUse = true;		//使用する
	g_edit[g_nCntEdit].bBuild = true;	//配置済み

	//次のデータを初期化して有効にする
	g_edit[g_nCntEdit + 1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//位置
	g_edit[g_nCntEdit + 1].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//向き
	g_edit[g_nCntEdit + 1].nType = 0;	//種類
	g_edit[g_nCntEdit + 1].nUse = -1;	//使用フィールド
	g_edit[g_nCntEdit + 1].bUse = true;	//使用する

	g_nCntEdit++;//読み込み数を加算
}

//================================================================================================================================================================================================================================
// 構造体の情報の取得
//================================================================================================================================================================================================================================
Edit* GetEdit(void)
{
	return &g_edit[0];
}
