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
#include "timer.h"
#include "summon.h"
#include "camera.h"
#include "debugproc.h"
//グローバル変数
#define ENEMYEDITOR_SPEED (5.0f)//敵エディタの速さ
#define EditEnemy_SCALE (10.0f)
#define EditEnemySET "data\\TEXTFILE\\EditEnemy.txt"
#define SAVEEditEnemy "data\\TEXTFILE\\SaveEditEnemy.txt"
#define SAVEENEMY "data\\TEXTFILE\\SaveEnemy.txt"


EnemyEditor g_aEnemyEditor;
EnemyEditorModelInfo g_aEnemyEditorModelInfo;
EditEnemy g_aEditEnemy[MAX_EDITENEMY];//セットした敵の情報
int g_nEditTime;//召喚する時間をエディットする。

//==================================
//敵エディタの初期化処理
//==================================
void InitEnemyEditor(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスへのポインタと取得
	g_aEnemyEditorModelInfo.dwNumMat = {};
	g_aEnemyEditorModelInfo.pBuffMat = {};
	g_aEnemyEditorModelInfo.pMesh = {};

	//==============================
	//モデル情報の取得
	//==============================
	D3DXLoadMeshFromX("data\\MODEL\\OBJECT\\Cube_000\\Cube_000.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_aEnemyEditorModelInfo.pBuffMat,
		NULL,
		&g_aEnemyEditorModelInfo.dwNumMat,
		&g_aEnemyEditorModelInfo.pMesh);

	//=================================================================================

	//===========================
	//初期化
	//===========================
	g_aEnemyEditor.bUse = true;//使用状態
	g_aEnemyEditor.mtxWorld = {};//マトリックスワールド
	g_aEnemyEditor.nIdxShadow = -1;//影のインデックス
	g_aEnemyEditor.nIdxRaderDisp = -1;//レーダー表示のインデックス
	g_aEnemyEditor.nSummonTime = 240;//召喚する時間
	g_aEnemyEditor.nType = 0;//セットする敵の種類
	g_aEnemyEditor.nLayoutType = 0;//敵の配置方法の種類
	g_aEnemyEditor.pos = NULL_VECTOR3;//位置
	g_aEnemyEditor.rot = NULL_VECTOR3;//向き
	g_aEnemyEditor.nNumSetEnemy = -1;//何番目の敵をセットしているか
	g_aEnemyEditor.targetrot = NULL_VECTOR3;
	//==================================================

	//=======================================
	//セットした敵の情報の初期化
	//=======================================

	for (int nCntEdit = 0; nCntEdit < MAX_EDITENEMY; nCntEdit++)
	{
		g_aEditEnemy[nCntEdit].bUse = false;//使用状態
		g_aEditEnemy[nCntEdit].mtxWorld = {};//マトリックスワールド
		g_aEditEnemy[nCntEdit].nIdxShadow = -1;//影のインデックス
		g_aEditEnemy[nCntEdit].nSummonTime = 0;//召喚する時間
		g_aEditEnemy[nCntEdit].nType = 0;//セットする敵の種類
		g_aEditEnemy[nCntEdit].nLayoutType = 0;//配置方法の種類
		g_aEditEnemy[nCntEdit].pos = NULL_VECTOR3;//位置
		g_aEditEnemy[nCntEdit].rot = NULL_VECTOR3;//向き
		g_aEditEnemy[nCntEdit].nNumEnemy = 0;//何番目のセットされた敵か
	}
}

//==============================
//敵エディタの終了処理
//==============================
void UninitEnemyEditor(void)
{

	//メッシュの破棄
	if (g_aEnemyEditorModelInfo.pMesh != NULL)
	{
		g_aEnemyEditorModelInfo.pMesh->Release();
		g_aEnemyEditorModelInfo.pMesh = NULL;
	}
	//マテリアルの破棄
	if (g_aEnemyEditorModelInfo.pBuffMat != NULL)
	{
		g_aEnemyEditorModelInfo.pBuffMat->Release();
		g_aEnemyEditorModelInfo.pBuffMat = NULL;
	}

}
//============================
//敵エディタの更新処理
//============================
void UpdateEnemyEditor(void)
{
	Camera* pCamera = GetCamera();

	//=================================
	//召喚する敵の種類を決める
	//=================================

	if (GetKeyboardTrigger(DIK_RIGHT) == true)
	{
		g_aEnemyEditor.nType++;
	}
	else if (GetKeyboardTrigger(DIK_LEFT) == true)
	{
		g_aEnemyEditor.nType--;
	}

	if (g_aEnemyEditor.nType <= -1)
	{
		g_aEnemyEditor.nType = NUM_ENEMY - 1;
	}
	else if (g_aEnemyEditor.nType >= NUM_ENEMY)
	{
		g_aEnemyEditor.nType = 0;
	}

	//=================================
	//召喚方法の種類を決める
	//=================================

	if (GetKeyboardTrigger(DIK_O) == true)
	{
		g_aEnemyEditor.nLayoutType++;

		if (g_aEnemyEditor.nLayoutType == 1)
		{
			g_aEnemyEditor.pos = NULL_VECTOR3;
		}
	}
	else if (GetKeyboardTrigger(DIK_L) == true)
	{
		g_aEnemyEditor.nLayoutType--;

		if (g_aEnemyEditor.nLayoutType == 1)
		{
			g_aEnemyEditor.pos = NULL_VECTOR3;
		}
	}

	if (g_aEnemyEditor.nLayoutType <= -1)
	{
		g_aEnemyEditor.nLayoutType = ENEMYEDITORLAYOUT_MAX - 1;

		if (g_aEnemyEditor.nLayoutType == 1)
		{
			g_aEnemyEditor.pos = NULL_VECTOR3;
		}
	}
	else if (g_aEnemyEditor.nLayoutType >= ENEMYEDITORLAYOUT_MAX)
	{
		g_aEnemyEditor.nLayoutType = 0;
	}

	//==================================
	//召喚する時間を宣言する。
	//==================================

	if (GetKeyboardTrigger(DIK_6) == true)
	{
		g_aEnemyEditor.nSummonTime--;
	}
	else if (GetKeyboardPress(DIK_LSHIFT) == true)
	{
		if (GetKeyboardTrigger(DIK_6) == true)
		{
			g_aEnemyEditor.nSummonTime -= 5;
		}
	}
	if (GetKeyboardTrigger(DIK_7) == true)
	{
		g_aEnemyEditor.nSummonTime++;
	}
	else if (GetKeyboardPress(DIK_LSHIFT) == true)
	{
		if (GetKeyboardTrigger(DIK_6) == true)
		{
			g_aEnemyEditor.nSummonTime += 5;
		}
	}


	//=====================================================================================================================================

	

	//===========================================
	//移動処理
	//===========================================

	bool bMove = false;

	float X = 0.0f;
	float Z = 0.0f;
	if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true)
	{
		Z = 1.0f;
		bMove = true;
	}
	else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true)
	{
		Z = -1.0f;
		bMove = true;
	}

	if (GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT) == true)
	{
		X = 1.0f;
		bMove = true;
	}
	else if (GetKeyboardPress(DIK_A) == true || GetJoypadPress(JOYKEY_LEFT) == true)
	{
		X = -1.0f;
		bMove = true;
	}

	if (bMove == true)
	{
		g_aEnemyEditor.pos.x += sinf(atan2f(X, Z) + pCamera->rot.y) * ENEMYEDITOR_SPEED;
		g_aEnemyEditor.pos.z += cosf(atan2f(X, Z) + pCamera->rot.y) * ENEMYEDITOR_SPEED;


		g_aEnemyEditor.targetrot.y = atan2f(X, Z) + pCamera->rot.y + D3DX_PI;


	}

	g_aEnemyEditor.rot.y += ((g_aEnemyEditor.targetrot.y) - g_aEnemyEditor.rot.y) * 0.5f;

	//======================================================================================================================


	//=======================================
	//敵をセットする
	//=======================================
	if (GetKeyboardTrigger(DIK_DOWN) == true)
	{
		SetEditEnemy(g_aEnemyEditor.pos, g_aEnemyEditor.rot, g_aEnemyEditor.nType, g_aEnemyEditor.nSummonTime,g_aEnemyEditor.nLayoutType);//敵エディタで配置した位置を保存
	}

	//=======================================
	//セットした敵を破棄する
	//=======================================
	if (GetKeyboardTrigger(DIK_DELETE) == true)
	{
		g_aEditEnemy[g_aEnemyEditor.nNumSetEnemy].bUse = false;//使用状態
		g_aEditEnemy[g_aEnemyEditor.nNumSetEnemy].nType = -1;//種類
		g_aEditEnemy[g_aEnemyEditor.nNumSetEnemy].pos = NULL_VECTOR3;//位置
		g_aEditEnemy[g_aEnemyEditor.nNumSetEnemy].rot = NULL_VECTOR3;//向き
		g_aEditEnemy[g_aEnemyEditor.nNumSetEnemy].nLayoutType = -1;//配置方法の種類
		Summon* pSummon = GetSummon();

		for (int nCntSummon = 0; nCntSummon < MAX_SUMMON; nCntSummon++, pSummon++)
		{
			if (pSummon->bUse == true && pSummon->nNumSetEdit == g_aEnemyEditor.nNumSetEnemy)
			{//召喚した敵も消す
				pSummon->bUse = false;

				g_aEnemyEditor.nNumSetEnemy--;

				break;
			}

		}
	}


	//==============================================================================================================================


	//=================================================
	//テキストファイルに召喚した情報を保存
	//=================================================
	if (GetKeyboardTrigger(DIK_F10) == true)
	{

		FILE* pFile;//ファイルポインタを宣言
		pFile = fopen(SAVEEditEnemy, "w");//ファイルに今回配置したモデル情報を書き出す

		if (pFile != NULL)//ファイルが開いていたら
		{//今セット中の敵の前の番号までを保存
			for (int nCntSaveModel = 0; nCntSaveModel < g_aEnemyEditor.nNumSetEnemy; nCntSaveModel++)
			{
				fprintf(pFile, "SUMMONSET\n");
				fprintf(pFile, "TYPE %d\n", g_aEditEnemy[nCntSaveModel].nType);//設置したモデルの種類を書き出す
				fprintf(pFile, "LAYOUTTYPE %d\n", g_aEditEnemy[nCntSaveModel].nLayoutType);//設置したモデルの種類を書き出す
				fprintf(pFile, "POS %.3f  %.3f  %.3f\n", g_aEditEnemy[nCntSaveModel].pos.x
					, g_aEditEnemy[nCntSaveModel].pos.y, g_aEditEnemy[nCntSaveModel].pos.z);//設置したモデルの位置を書き出す
				fprintf(pFile, "ROT %.3f  %.3f  %.3f\n", g_aEditEnemy[nCntSaveModel].rot.x
					, g_aEditEnemy[nCntSaveModel].rot.y, g_aEditEnemy[nCntSaveModel].rot.z);//設置したモデルの向きを書き出す
				fprintf(pFile, "TIME %d\n", g_aEditEnemy[nCntSaveModel].nSummonTime);//召喚時間を書き出す。
				fprintf(pFile, "ENDSUMMONSET\n");
				fprintf(pFile, "\n");
			}
			fclose(pFile);
		}


		//=====================================================
		//保存し終わったので、召喚した敵を編集不可にする
		//=====================================================

		Summon* pSummon = GetSummon();
		for (int nCntSummon = 0; nCntSummon < MAX_SUMMON; nCntSummon++, pSummon++)
		{
			if (pSummon->bRemote == true)
			{
				if (pSummon->nNumSetEdit >= 0 && pSummon->nNumSetEdit <= g_aEnemyEditor.nNumSetEnemy)
				{
					pSummon->nNumSetEdit = -1;
					pSummon->bRemote = false;
				}
			}
		}

		//======================================================================================================

		g_aEnemyEditor.nNumSetEnemy = -1;//セットし終わったので、セットしている番号もー１に戻す。
	}

	//================================================================================================================================


	if (g_aEnemyEditor.nNumSetEnemy <= -2)
	{
		g_aEnemyEditor.nNumSetEnemy = -1;
	}

	//==============================
	//エディターの情報
	//==============================
	PrintDebugProc("//==================================\n");
	PrintDebugProc("//敵エディタの操作方法\n");
	PrintDebugProc("//==================================\n");
	PrintDebugProc("移動：WASD\n");
	PrintDebugProc("配置する敵の種類を変える：←　→\n");
	PrintDebugProc("配置方法の種類を変える：O　L\n");
	PrintDebugProc("召喚する時間を１秒変える（SHIFTを押しながらだと5秒)：６　７\n");
	PrintDebugProc("敵を配置する：↓\n");
	PrintDebugProc("配置した敵の情報を保存する:F10\n");
	PrintDebugProc("//====================================================================\n\n");


	PrintDebugProc("//==================================\n");
	PrintDebugProc("//敵エディタの情報\n");
	PrintDebugProc("//==================================\n");
	PrintDebugProc("敵エディタの位置：%f %f %f\n", g_aEnemyEditor.pos.x, g_aEnemyEditor.pos.y, g_aEnemyEditor.pos.z);
	PrintDebugProc("敵エディタの向き：%f %f %f\n", g_aEnemyEditor.rot.x, g_aEnemyEditor.rot.y, g_aEnemyEditor.rot.z);
	PrintDebugProc("セットする敵の種類：%d\n", g_aEnemyEditor.nType);

	if (g_aEnemyEditor.nLayoutType == 0)
	{
	    PrintDebugProc("配置方法の種類：世界\n");
	}
	else if (g_aEnemyEditor.nLayoutType == 1)
	{
		PrintDebugProc("配置方法の種類：プレイヤーの周り\n");
	}

	PrintDebugProc("敵を召喚する時間：%d\n", g_aEnemyEditor.nSummonTime);
	PrintDebugProc("何番目の敵をセットしているか：%d\n\n", g_aEnemyEditor.nNumSetEnemy);
	PrintDebugProc("//====================================================================\n");

	//====================================================================================================================

}

//================================
//敵エディタの描画処理
//================================
void DrawEnemyEditor(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();;//デバイスへのポインタと取得

	D3DXMATRIX mtxRot, mtxTrans;//計算用マトリックス

	D3DMATERIAL9 matDef;//現在のマテリアル保存用

	D3DXMATERIAL* pMat;//マテリアルデータへのポインタ


	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	if (g_aEnemyEditor.bUse == true)
	{

		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aEnemyEditor.mtxWorld);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aEnemyEditor.rot.y, g_aEnemyEditor.rot.x, g_aEnemyEditor.rot.z);

		D3DXMatrixMultiply(&g_aEnemyEditor.mtxWorld, &g_aEnemyEditor.mtxWorld, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_aEnemyEditor.pos.x, g_aEnemyEditor.pos.y, g_aEnemyEditor.pos.z);
		D3DXMatrixMultiply(&g_aEnemyEditor.mtxWorld, &g_aEnemyEditor.mtxWorld, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aEnemyEditor.mtxWorld);


		//マテリアルへのポインタを取得（セットしたモデルと同じタイプ）
		pMat = (D3DXMATERIAL*)g_aEnemyEditorModelInfo.pBuffMat->GetBufferPointer();

		int nNumTexture = 0;
		//セットされたモデルとタイプが同じもののモデルのマテリアルの数分読み込む。
		for (int nCntMat = 0; nCntMat < (int)g_aEnemyEditorModelInfo.dwNumMat; nCntMat++)
		{
			//マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//テクスチャの設定
			pDevice->SetTexture(0, NULL);
			nNumTexture++;

			//モデル（パーツ）の描画
			g_aEnemyEditorModelInfo.pMesh->DrawSubset(nCntMat);
		}

		//保存していたマテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}

}

//=================================
//配置する敵の情報をセットする
//=================================
void SetEditEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nTime, int nLayoutType)
{
	for (int nCntEdit = 0; nCntEdit < MAX_EDITENEMY; nCntEdit++)
	{
		if (g_aEditEnemy[nCntEdit].bUse == false)
		{
			g_aEnemyEditor.nNumSetEnemy++;//何体目にセットしたか
			SetSummon(g_aEnemyEditor.nType, g_aEnemyEditor.nSummonTime, g_aEnemyEditor.nNumSetEnemy,g_aEnemyEditor.nLayoutType, true, g_aEnemyEditor.pos, g_aEnemyEditor.rot);//敵召喚情報を設定

			g_aEditEnemy[nCntEdit].bUse = true;//使用状態
			g_aEditEnemy[nCntEdit].nType = nType;//種類
			g_aEditEnemy[nCntEdit].pos = pos;//位置
			g_aEditEnemy[nCntEdit].rot = rot;//向き
			g_aEditEnemy[nCntEdit].nSummonTime = nTime;//召喚する時間
			g_aEditEnemy[nCntEdit].nLayoutType = nLayoutType;//配置される方法の種類
			break;
		}
	}
}

int GetEditTime(void)//敵の召喚時間を決める変数を取得する。
{
	return g_nEditTime;
}

//==================================
//構造体の情報を取得
//==================================
EnemyEditor* GetEnemyEditor(void)
{
	return &g_aEnemyEditor;
}
//========================================================
