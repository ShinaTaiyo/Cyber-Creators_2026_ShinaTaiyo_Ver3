//========================================================
//
//１２月２６日：チーム制作の基盤づくり[Summon.cpp]
//Author:ShinaTaiyo
//
// 詳細：主に敵の召喚をしている処理
//=========================================================

#include "Summon.h"
#include "main.h"
#include "sound.h"
#include "player.h"
#include <stdio.h>
#include <string.h>
#include "timer.h"
#include "enemy.h"
#include "item.h"
#include "game.h"
#include "boss.h"
#include "enemy.h"
#include "editEnemy.h"
//グローバル変数
int g_nDelaySummon = 0;//何フレーム毎に処理をしたいときに使う。
float g_fRandumRot = 0;//中心点から向きを取って、その向きのどの位置に召喚するか決める。
float g_fRandumLength = 0;//中心点からの距離を取って、その距離の位置に召喚をする
int g_nType = 0;//敵の種類を割り当てる
int g_nMaxSummon;//セットした召喚する敵の最大数を記録する
bool g_bBossSummon;//ボス召喚フラグ

Summon g_aSummon[MAX_SUMMON];//敵召喚用構造体
SummonModelInfo g_aSummonModelInfo[NUM_ENEMY];
//タイトル画面の初期化処理
void InitSummon(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスへのポインタと取得


	D3DXMATERIAL* pMat;//マテリアルへのポインタ
	g_nDelaySummon = 0;
	g_fRandumRot = 0;
	g_fRandumLength = 0;
	g_nType = 0;
	g_nMaxSummon = 0;

	for (int nCntModelInfo = 0; nCntModelInfo < NUM_ENEMY; nCntModelInfo++)
	{
		g_aSummonModelInfo[nCntModelInfo].dwNumMat = {};
		g_aSummonModelInfo[nCntModelInfo].pBuffMat = {};
		g_aSummonModelInfo[nCntModelInfo].pMesh = {};
	}

	for (int nCntSummon = 0; nCntSummon < MAX_SUMMON; nCntSummon++)
	{
		g_aSummon[nCntSummon].bUse = false;//使用しているかどうか
		g_aSummon[nCntSummon].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//位置
		g_aSummon[nCntSummon].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//移動量
		g_aSummon[nCntSummon].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//向き
		g_aSummon[nCntSummon].nType = 0;//召喚する敵の種類
		g_aSummon[nCntSummon].nSummonTime = 0;//敵を召喚する時間
		g_aSummon[nCntSummon].mtxWorld;//マトリックスワールド
		g_aSummon[nCntSummon].nNumSetEdit = -1;//エディターでセットされた敵の番号
		g_aSummon[nCntSummon].bRemote = false;//遠隔からの変更を受けるかどうか
		g_aSummon[nCntSummon].nLayoutType = 0;//配置される方法の種類
	}



	for (int nCntType = 0; nCntType < NUM_ENEMY; nCntType++)
	{
		//ファイルの読み込み
		D3DXLoadMeshFromX(SUMMON_FILENAME[nCntType],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_aSummonModelInfo[nCntType].pBuffMat,
			NULL,
			&g_aSummonModelInfo[nCntType].dwNumMat,
			&g_aSummonModelInfo[nCntType].pMesh);

		//マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_aSummonModelInfo[nCntType].pBuffMat->GetBufferPointer();

		for (int nCntSummon = 0; nCntSummon < MAX_SUMMON; nCntSummon++)
		{
			for (int nCntMat = 0; nCntMat < (int)g_aSummonModelInfo[nCntType].dwNumMat; nCntMat++)
			{
				g_aSummon[nCntSummon].Diffuse[nCntType][nCntMat] = pMat[nCntMat].MatD3D.Diffuse;
				g_aSummon[nCntSummon].FormarDiffuse[nCntType][nCntMat] = pMat[nCntMat].MatD3D.Diffuse;//元の色合い
			}
		}
	}



	char aString[100] = {};//文字列を読み込むための変数
	int nType = 0;//読み込んだモデルの種類
	int nLayoutType = 0;//配置方法の種類

	float fPosX = 0.0f;//モデルの位置X
	float fPosY = 0.0f;//モデルの位置Y
	float fPosZ = 0.0f;//モデルの位置Z

	float fRotX = 0.0f;//モデルの位置X
	float fRotY = 0.0f;//モデルの位置Y
	float fRotZ = 0.0f;//モデルの位置Z

	int nSummonTime = 0;//召喚する時間を宣言する
	FILE* pFile;//ファイルポインタ

	pFile = fopen(SUMMONSET, "r");

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
			else if (strcmp(&aString[0], "SUMMONSET") == 0)
			{
				g_nMaxSummon++;//敵を召喚する最大数をインクリメントする
				while (1)
				{
					fscanf(pFile, "%s", &aString[0]);
					if (aString[0] == '#')
					{
						fgets(&aString[0], 100, pFile);
						continue;
					}
					else if (strcmp(&aString[0], "TYPE") == 0)
					{
						fscanf(pFile, "%d", &nType);//ウォールの種類を決める
					}
					else if (strcmp(&aString[0], "LAYOUTTYPE") == 0)
					{
						fscanf(pFile, "%d", &nLayoutType);//配置方法の種類を決める
					}
					else if (strcmp(&aString[0], "POS") == 0)
					{
						fscanf(pFile, "%f%f%f", &fPosX, &fPosY, &fPosZ);//位置情報を読み込む
					}
					else if (strcmp(&aString[0], "ROT") == 0)
					{
						fscanf(pFile, "%f%f%f", &fRotX, &fRotY, &fRotZ);//向き情報を読み込む
					}
					else if (strcmp(&aString[0], "TIME") == 0)
					{
						fscanf(pFile, "%d", &nSummonTime);//召喚する時間を読み込む。
					}
					else if (strcmp(&aString[0], "ENDSUMMONSET") == 0)
					{//初期化でセットされたデータは、既にあるデータなので、エディタの配置にはカウントしない
						SetSummon(nType, nSummonTime, -1, nLayoutType,false, D3DXVECTOR3(fPosX, fPosY, fPosZ), D3DXVECTOR3(fRotX, fRotY, fRotZ));
						break;
					}
				}
			}
		}
	}
}
//============================
//モデルの終了処理
//============================
void UninitSummon(void)
{
	for (int nCntSummon = 0; nCntSummon < NUM_ENEMY; nCntSummon++)
	{
		//メッシュの破棄
		if (g_aSummonModelInfo[nCntSummon].pMesh != NULL)
		{
			g_aSummonModelInfo[nCntSummon].pMesh->Release();
			g_aSummonModelInfo[nCntSummon].pMesh = NULL;
		}
		//マテリアルの破棄
		if (g_aSummonModelInfo[nCntSummon].pBuffMat != NULL)
		{
			g_aSummonModelInfo[nCntSummon].pBuffMat->Release();
			g_aSummonModelInfo[nCntSummon].pBuffMat = NULL;
		}
	}
}

//============================
//モデルの更新処理
//============================
void UpdateSummon(void)
{
	Player* pPlayer = GetPlayer();
	Boss* pBoss = GetBoss();

	bool bEditEnemyMode = GetEditEnemyMode();

	//======================================
	//敵エディットモード以外の時にする処理
	//======================================
	if (bEditEnemyMode == false)
	{
		g_nDelaySummon++;

		if (g_nDelaySummon == 600)
		{
			SetEnemy(NULL_VECTOR3, NULL_VECTOR3, 4, false);
		}

		//======================================
		//乱数召喚敵抽選処理
		//======================================
		g_fRandumRot = float(rand() % 200 + 1) / 100;
		int nGetTimer = GetTimer();//残り時間の取得
		int nLottery = rand() % 100 + 1;//召喚する敵を抽選する
		//==================================================================================================================================================================


		//======================================
		//時間ごとに固定位置に敵召喚処理
		//======================================
		if (pBoss->bUse == false)
		{
			for (int nCntSummon = 0; nCntSummon < MAX_SUMMON; nCntSummon++)
			{
				if (g_aSummon[nCntSummon].bUse == true && g_aSummon[nCntSummon].nSummonTime == nGetTimer)//使用しているかつ、召喚時間が一致したら
				{
					if (g_aSummon[nCntSummon].nLayoutType == ENEMYEDITORLAYOUT00_WORLD)
					{
						SetEnemy(g_aSummon[nCntSummon].pos, g_aSummon[nCntSummon].rot, g_aSummon[nCntSummon].nType, false);
					}
					else if (g_aSummon[nCntSummon].nLayoutType == ENEMYEDITORLAYOUT01_PLAYERAROUND)
					{
						SetEnemy(pPlayer->pos + g_aSummon[nCntSummon].pos, g_aSummon[nCntSummon].rot, g_aSummon[nCntSummon].nType, false);

					}
					g_aSummon[nCntSummon].bUse = false;//一回召喚したので用済み
				}
			}
		}

		if (pBoss->bUse == false)
		{
			if (180 <= nGetTimer && nGetTimer <= 240)//開始３０秒
			{
				if (g_nDelaySummon % 140 == 0)
				{

					float fLength = 0.0f;
					int nLottery2 = rand() % 5;

					if (1 <= nLottery && nLottery <= 25)
					{
						g_nType = 0;//レッドブル
					}
					else if (26 <= nLottery && nLottery <= 50)
					{
						g_nType = 1;//ミルタンク
					}
					else if (51 <= nLottery && nLottery <= 75)
					{
						g_nType = 2;//モンスター
					}
					else if (76 <= nLottery && nLottery <= 100)
					{
						g_nType = 3;//ミックチュジューチュ
					}


					//==============================
					//敵によって出現位置を調整
					//==============================
					if (g_nType == 1 || g_nType == 3)
					{
						if (nLottery2 >= 0 && nLottery2 <= 2)
						{//60%の確率で遠いところに出る。
							fLength = float(rand() % 500 + 700);
						}
						else
						{//40%の確率で近い所に出る
							fLength = float(rand() % 500 + 300);

						}
					}
					else if (g_nType == 0 || g_nType == 2)
					{
						if (nLottery2 >= 0 && nLottery2 <= 2)
						{//60%の確率で近いところに出る。
							fLength = float(rand() % 500 + 300);
						}
						else
						{//40%の確率で遠い所に出る
							fLength = float(rand() % 500 + 700);
						}
					}
					else
					{
						fLength = float(rand() % 1000 + 500);
					}

					//===========================================================================================

					SetEnemy(D3DXVECTOR3(pPlayer->pos.x + sinf(-D3DX_PI * g_fRandumRot) * fLength, pPlayer->pos.y, pPlayer->pos.z + cosf(-D3DX_PI * g_fRandumRot) * fLength),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), g_nType,false);
				}
			}
			else if (120 <= nGetTimer && nGetTimer <= 180)
			{
				if (g_bBossSummon == false)
				{
					g_bBossSummon = true;

				}
				if (g_nDelaySummon % 120 == 0)
				{
					float fLength = 0.0f;
					int nLottery2 = rand() % 5;

					if (1 <= nLottery && nLottery <= 25)
					{
						g_nType = 0;//レッドブル
					}
					else if (26 <= nLottery && nLottery <= 50)
					{
						g_nType = 1;//ミルタンク
					}
					else if (51 <= nLottery && nLottery <= 75)
					{
						g_nType = 2;//モンスター
					}
					else if (76 <= nLottery && nLottery <= 100)
					{
						g_nType = 3;//ミックチュジューチュ
					}

					//==============================
					//敵によって出現位置を調整
					//==============================
					if (g_nType == 1 || g_nType == 3)
					{
						if (nLottery2 >= 0 && nLottery2 <= 2)
						{//60%の確率で遠いところに出る。
							fLength = float(rand() % 500 + 700);
						}
						else
						{//40%の確率で近い所に出る
							fLength = float(rand() % 500 + 300);

						}
					}
					else if (g_nType == 0 || g_nType == 2)
					{
						if (nLottery2 >= 0 && nLottery2 <= 2)
						{//60%の確率で近いところに出る。
							fLength = float(rand() % 500 + 300);
						}
						else
						{//40%の確率で遠い所に出る
							fLength = float(rand() % 500 + 700);
						}
					}
					else
					{
						fLength = float(rand() % 1000 + 500);
					}

					//===========================================================================================

					SetEnemy(D3DXVECTOR3(pPlayer->pos.x + sinf(-D3DX_PI * g_fRandumRot) * fLength, pPlayer->pos.y, pPlayer->pos.z + cosf(-D3DX_PI * g_fRandumRot) * fLength),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), g_nType, false);

				}
			}
			else if (60 <= nGetTimer && nGetTimer <= 120)
			{
				if (g_nDelaySummon % 100 == 0)
				{
					float fLength = 0.0f;
					int nLottery2 = rand() % 5;

					if (1 <= nLottery && nLottery <= 25)
					{
						g_nType = 0;//レッドブル
					}
					else if (26 <= nLottery && nLottery <= 50)
					{
						g_nType = 1;//ミルタンク
					}
					else if (51 <= nLottery && nLottery <= 75)
					{
						g_nType = 2;//モンスター
					}
					else if (76 <= nLottery && nLottery <= 100)
					{
						g_nType = 3;//ミックチュジューチュ
					}

					//==============================
					//敵によって出現位置を調整
					//==============================
					if (g_nType == 1 || g_nType == 3)
					{
						if (nLottery2 >= 0 && nLottery2 <= 2)
						{//60%の確率で遠いところに出る。
							fLength = float(rand() % 500 + 700);
						}
						else
						{//40%の確率で近い所に出る
							fLength = float(rand() % 500 + 300);

						}
					}
					else if (g_nType == 0 || g_nType == 2)
					{
						if (nLottery2 >= 0 && nLottery2 <= 2)
						{//60%の確率で近いところに出る。
							fLength = float(rand() % 500 + 300);
						}
						else
						{//40%の確率で遠い所に出る
							fLength = float(rand() % 500 + 700);
						}
					}
					else
					{
						fLength = float(rand() % 1000 + 500);
					}

					//===========================================================================================

					SetEnemy(D3DXVECTOR3(pPlayer->pos.x + sinf(-D3DX_PI * g_fRandumRot) * fLength, pPlayer->pos.y, pPlayer->pos.z + cosf(-D3DX_PI * g_fRandumRot) * fLength),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), g_nType, false);
				}
			}

			else if (0 <= nGetTimer && nGetTimer <= 60)
			{
				if (g_nDelaySummon % 60 == 0)
				{
					float fLength = 0.0f;
					int nLottery2 = rand() % 5;
					if (1 <= nLottery && nLottery <= 25)
					{
						g_nType = 0;//レッドブル
					}
					else if (26 <= nLottery && nLottery <= 50)
					{
						g_nType = 1;//ミルタンク
					}
					else if (51 <= nLottery && nLottery <= 75)
					{
						g_nType = 2;//モンスター
					}
					else if (76 <= nLottery && nLottery <= 100)
					{
						g_nType = 3;//ミックチュジューチュ
					}
					//==============================
					//敵によって出現位置を調整
					//==============================
					if (g_nType == 1 || g_nType == 3)
					{
						if (nLottery2 >= 0 && nLottery2 <= 2)
						{//60%の確率で遠いところに出る。
							fLength = float(rand() % 500 + 700);
						}
						else
						{//40%の確率で近い所に出る
							fLength = float(rand() % 500 + 300);

						}
					}
					else if (g_nType == 0 || g_nType == 2)
					{
						if (nLottery2 >= 0 && nLottery2 <= 2)
						{//60%の確率で近いところに出る。
							fLength = float(rand() % 500 + 300);
						}
						else
						{//40%の確率で遠い所に出る
							fLength = float(rand() % 500 + 700);
						}
					}
					else
					{
						fLength = float(rand() % 1000 + 500);
					}

					//===========================================================================================

					SetEnemy(D3DXVECTOR3(pPlayer->pos.x + sinf(-D3DX_PI * g_fRandumRot) * fLength, pPlayer->pos.y, pPlayer->pos.z + cosf(-D3DX_PI * g_fRandumRot) * fLength),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), g_nType, false);
				}
			}
		}
		//=====================================================================================================================================================================

	}
	//====================================================================================================================================================================================
	
}

//==============================
//描画処理
//==============================
void DrawSummon(void)
{
	EnemyEditor* pEnemyEditor = GetEnemyEditor();

	LPDIRECT3DDEVICE9 pDevice = GetDevice();;//デバイスへのポインタと取得

	D3DXMATRIX mtxRot, mtxTrans;//計算用マトリックス

	D3DMATERIAL9 matDef;//現在のマテリアル保存用

	D3DXMATERIAL* pMat;//マテリアルデータへのポインタ


	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	for (int nCntSummon = 0; nCntSummon < MAX_SUMMON; nCntSummon++)
	{
		if (g_aSummon[nCntSummon].nSummonTime == pEnemyEditor->nSummonTime && g_aSummon[nCntSummon].bUse == true && g_aSummon[nCntSummon].nLayoutType == pEnemyEditor->nLayoutType)
		{//使用状態であるかつ、召喚時間が、敵エディタの召喚時間と一致していたら描画を開始

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aSummon[nCntSummon].mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aSummon[nCntSummon].rot.y, g_aSummon[nCntSummon].rot.x, g_aSummon[nCntSummon].rot.z);

			D3DXMatrixMultiply(&g_aSummon[nCntSummon].mtxWorld, &g_aSummon[nCntSummon].mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aSummon[nCntSummon].pos.x, g_aSummon[nCntSummon].pos.y, g_aSummon[nCntSummon].pos.z);
			D3DXMatrixMultiply(&g_aSummon[nCntSummon].mtxWorld, &g_aSummon[nCntSummon].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aSummon[nCntSummon].mtxWorld);


			//マテリアルへのポインタを取得（セットしたモデルと同じタイプ）
			pMat = (D3DXMATERIAL*)g_aSummonModelInfo[g_aSummon[nCntSummon].nType].pBuffMat->GetBufferPointer();

			//セットされたモデルとタイプが同じもののモデルのマテリアルの数分読み込む。
			for (int nCntMat = 0; nCntMat < (int)g_aSummonModelInfo[g_aSummon[nCntSummon].nType].dwNumMat; nCntMat++)
			{
				pMat[nCntMat].MatD3D.Diffuse = g_aSummon[nCntSummon].Diffuse[g_aSummon[nCntSummon].nType][nCntMat];

				//マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//テクスチャの設定
				pDevice->SetTexture(0, NULL);

				//モデル（パーツ）の描画
				g_aSummonModelInfo[g_aSummon[nCntSummon].nType].pMesh->DrawSubset(nCntMat);
			}

			//保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}


//==============================
//召喚する敵の情報のセット
//==============================
void SetSummon(int nType, int nTime, int nNumSetEdit, int nLayoutType, bool bRemote, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	for (int nCntSummon = 0; nCntSummon < MAX_SUMMON; nCntSummon++)
	{
		if (g_aSummon[nCntSummon].bUse == false && nTime != 240)
		{
			g_aSummon[nCntSummon].bUse = true;
			g_aSummon[nCntSummon].pos = pos;//位置
			g_aSummon[nCntSummon].rot = rot;//向き
			g_aSummon[nCntSummon].nType = nType;//召喚する敵の種類
			g_aSummon[nCntSummon].nSummonTime = nTime;//敵を召喚する時間
			g_aSummon[nCntSummon].nNumSetEdit = nNumSetEdit;//エディタで召喚された敵の番号
			g_aSummon[nCntSummon].bRemote = bRemote;//編集可能かどうか
			g_aSummon[nCntSummon].nLayoutType = nLayoutType;//配置される方法の種類

			for (int nCntMat = 0; nCntMat < (int)(g_aSummonModelInfo[nType].dwNumMat); nCntMat++)
			{
				g_aSummon[nCntSummon].Diffuse[nType][nCntMat].a = 0.5f;
			}
			break;
		}
	}
}

//===================================
//敵を召喚する数を取得する
//===================================
int GetMaxSummon(void)
{
	return g_nMaxSummon;
}

//===================================
//構造体の情報を取得
//===================================
Summon* GetSummon(void)
{
	return &g_aSummon[0];
}

