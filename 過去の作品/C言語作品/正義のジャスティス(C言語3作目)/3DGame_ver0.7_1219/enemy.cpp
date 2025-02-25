//========================================================
//
//１１月７日：モデル読み込み[Enemy.cpp]
//Author:ShinaTaiyo
//
//=========================================================

#include "enemy.h"
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
#include "item.h"
#include"particle.h"
//グローバル変数
int g_nNumEnemy = 0;//読み込むモデルの最大数を決める
#define ENEMY_SCALE (10.0f)
//#define SAVEEnemy "data\\TEXTFILE\\SaveEnemy.txt"

#define MAX_ENEMYTEXTURE (32)
int g_nDelayEnemy = 0;

typedef struct
{
	LPD3DXMESH pMeshEnemy;//メッシュ（頂点情報）へのポインタ（モデルの種類数分）
	LPD3DXBUFFER pBuffMatEnemy;//マテリアルへのポインタ
	DWORD dwNumMatEnemy;//マテリアルの数
	LPDIRECT3DTEXTURE9 apTextureEnemy[MAX_ENEMYTEXTURE];//テクスチャへのポインタ
}EnemyMat;

D3DXVECTOR3 g_posEnemy;//位置
D3DXVECTOR3 g_rotEnemy;//向き
D3DXMATRIX g_mtxWorldEnemy;//ワールドマトリックス

Enemy g_aEnemy[MAX_ENEMY];//モデル情報の構造体

EnemyMat g_aEnemyMat[NUM_Enemy];//モデルのマテリアルや頂点の構造体

#define MAX_DELAY (50)//全体的に攻撃速度を下げる

int g_nldxShadowEnemy = -1;//対象の影のインデックス（番号）
//タイトル画面の初期化処理
void InitEnemy(void)
{

	//for (int nCntEnemy = 0; nCntEnemy < NUM_Enemy; nCntEnemy++)
	//{
	//	g_dwNumMatEnemy[nCntEnemy] = 0;//マテリアルの数
	//}
	//g_nldxShadow = 0;//対象の影のインデックス

	g_nNumEnemy = 2;

	g_nDelayEnemy = 0;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスへのポインタと取得

	g_posEnemy = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotEnemy = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	g_nNumEnemy = 0;

	//g_bIsLanding = false;//地面にいるかどうか



	////モデルの読み込み
	//D3DXLoadMeshFromX("data\\Enemy\\bill.x",
	//	D3DXMESH_SYSTEMMEM,
	//	pDevice,
	//	NULL,
	//	&g_pBuffMatEnemy,
	//	NULL,
	//	&g_dwNumMatEnemy,
	//	&g_pMeshEnemy[EnemyTYPE_BILL]);

	//D3DXLoadMeshFromX("data\\Enemy\\BigBriddge.x",
	//	D3DXMESH_SYSTEMMEM,
	//	pDevice,
	//	NULL,
	//	&g_pBuffMatEnemy,
	//	NULL,
	//	&g_dwNumMatEnemy,
	//	&g_pMeshEnemy[EnemyTYPE_BRIDDGE]);


	for (int nCntMatEnemy = 0; nCntMatEnemy < NUM_Enemy; nCntMatEnemy++)
	{
		g_aEnemyMat[nCntMatEnemy].dwNumMatEnemy = {};
		g_aEnemyMat[nCntMatEnemy].pBuffMatEnemy = {};
		g_aEnemyMat[nCntMatEnemy].pMeshEnemy = {};
		for (int nCntTexture = 0; nCntTexture < MAX_ENEMYTEXTURE; nCntTexture++)
		{
			g_aEnemyMat[nCntMatEnemy].apTextureEnemy[nCntTexture] = {};//一つのモデルに関連するテクスチャの初期化
		}
	}


	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
	{
		g_aEnemy[nCnt].bUse = false;//使用しているかどうか
		g_aEnemy[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//位置
		g_aEnemy[nCnt].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//1F前の位置

		g_aEnemy[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//移動量
		g_aEnemy[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//向き
		g_aEnemy[nCnt].nIdxShadow = -1;
		g_aEnemy[nCnt].nType = -1;
		g_aEnemy[nCnt].vtxMinEnemy = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//それぞれの座標のモデルの位置の最小値
		g_aEnemy[nCnt].vtxMaxEnemy = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//それぞれの座標のモデルの位置の最大値
		g_aEnemy[nCnt].nNumVtx = 0;//頂点数
		g_aEnemy[nCnt].sizeFVF;//頂点フォーマットのサイズ
		g_aEnemy[nCnt].pVtxBuff;//頂点バッファへのポインタ
		g_aEnemy[nCnt].Size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//敵の大きさ
		g_aEnemy[nCnt].bIsLanding = false;//地面についているかどうか
		g_aEnemy[nCnt].fVaimY1 = 0.0f;//総合したY方向のベクトル
		g_aEnemy[nCnt].fVaimY2 = 0.0f;//総合したY方向のベクトル
		g_aEnemy[nCnt].fVaim3 = 0.0f;

		g_aEnemy[nCnt].fVXaim = 0.0f;
		g_aEnemy[nCnt].fVYaim = 0.0f;
		g_aEnemy[nCnt].fVZaim = 0.0f;
		g_aEnemy[nCnt].fVaim = 0.0f;

		g_aEnemy[nCnt].fPenduram = 0.0f;//振り子のように弾を発射させる処理
		g_aEnemy[nCnt].bPenduramFlag = false;

		//ステータス設定
		g_aEnemy[nCnt].nHp = 0;//体力
		g_aEnemy[nCnt].nAttack = 0;//攻撃力
		g_aEnemy[nCnt].nDefeatScore = 0;//倒したときにもらえるスコア
		g_aEnemy[nCnt].fSpeed = 0.0f;//敵の速さ
	}


	char aString[100] = {};//文字列を読み込むための変数
	int nMaxRoadEnemy = 0;//読み込むモデルの最大数
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

	pFile = fopen(ENEMYSET, "r");
	char cData;
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
				fscanf(pFile, "%d", &g_nNumEnemy);

				for (int nCnt = 0; nCnt < 3; nCnt++)
				{
					fscanf(pFile, "%s", &aString[0]);
				}
				/*for (int nRoadEnemy = 0; nRoadEnemy < g_nNumEnemy; nRoadEnemy++)
				{*/
					/*	while (1)
						{*/
				int nRoadEnemy = 0;
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
							&g_aEnemyMat[nRoadEnemy].pBuffMatEnemy,//モデルでは、これも配列にする
							NULL,
							&g_aEnemyMat[nRoadEnemy].dwNumMatEnemy,//モデルでは、これも配列にする
							&g_aEnemyMat[nRoadEnemy].pMeshEnemy);
						fscanf(pFile, "%s", &aString[0]);
						nRoadEnemy++;
					}
					
				} while (nRoadEnemy < g_nNumEnemy);
					/*	nCnt++;
						if (nCnt == g_nNumEnemy)
						{
							break;
						}*/
				//}
			}
			else if (strcmp(&aString[0], "ENEMYSET") == 0)
			{
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
					else if (strcmp(&aString[0], "POS") == 0)
					{
						fscanf(pFile, "%f%f%f", &fPosX, &fPosY, &fPosZ);//位置情報を読み込む
					}
					else if (strcmp(&aString[0], "ROT") == 0)
					{
						fscanf(pFile, "%f%f%f", &fRotX, &fRotY, &fRotZ);//向き情報を読み込む
					}
					else if (strcmp(&aString[0], "ENDENEMYSET") == 0)
					{
						SetEnemy(D3DXVECTOR3(fPosX, fPosY, fPosZ), D3DXVECTOR3(fRotX, fRotY, fRotZ), nType);
						break;
					}
				}
			}
		}
	}

	//for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	//{//全てのモデルに割り当てる
	//	for (int nCntMat = 0; nCntMat < g_nNumEnemy; nCntMat++)
	//	{//種類を割り当てるfor文
	//		if (g_aEnemy[nCntEnemy].nType == nCntMat)
	//		{//モデルの種類が一致

	//			//頂点数の取得
	//			g_aEnemy[nCntEnemy].nNumVtx = g_aEnemyMat[nCntMat].pMeshEnemy->GetNumVertices();

	//			//頂点フォーマットのサイズを取得
	//			g_aEnemy[nCntEnemy].sizeFVF = D3DXGetFVFVertexSize(g_aEnemyMat[nCntMat].pMeshEnemy->GetFVF());

	//			//頂点バッファのロック
	//			g_aEnemyMat[nCntMat].pMeshEnemy->LockVertexBuffer(D3DLOCK_READONLY, (void**)&g_aEnemy[nCntEnemy].pVtxBuff);


	//			for (int nCntVtx = 0; nCntVtx < g_aEnemy[nCntEnemy].nNumVtx; nCntVtx++)
	//			{
	//				//頂点座標の代入
	//				D3DXVECTOR3 vtx = *(D3DXVECTOR3*)g_aEnemy[nCntEnemy].pVtxBuff;

	//				//====================================================
	//				//頂点座標を比較してモデルの最小値最大値を取得
	//				//====================================================
	//				if (vtx.x > g_aEnemy[nCntEnemy].vtxMaxEnemy.x)
	//				{
	//					g_aEnemy[nCntEnemy].vtxMaxEnemy.x = vtx.x;
	//				}
	//				else if (vtx.x < g_aEnemy[nCntEnemy].vtxMinEnemy.x)
	//				{//今回読み込んだ頂点が、一番小さい頂点より小さい場合
	//					g_aEnemy[nCntEnemy].vtxMinEnemy.x = vtx.x;
	//				}
	//				else if (vtx.y > g_aEnemy[nCntEnemy].vtxMaxEnemy.y)
	//				{//今回読み込んだ頂点が、一番小さい頂点より小さい場合
	//					g_aEnemy[nCntEnemy].vtxMaxEnemy.y = vtx.y;
	//				}
	//				else if (vtx.y < g_aEnemy[nCntEnemy].vtxMinEnemy.y)
	//				{//今回読み込んだ頂点が、一番小さい頂点より小さい場合
	//					g_aEnemy[nCntEnemy].vtxMinEnemy.y = vtx.y;
	//				}
	//				else if (vtx.z > g_aEnemy[nCntEnemy].vtxMaxEnemy.z)
	//				{//今回読み込んだ頂点が、一番小さい頂点より小さい場合
	//					g_aEnemy[nCntEnemy].vtxMaxEnemy.z = vtx.z;
	//				}
	//				else if (vtx.z < g_aEnemy[nCntEnemy].vtxMinEnemy.z)
	//				{//今回読み込んだ頂点が、一番小さい頂点より小さい場合
	//					g_aEnemy[nCntEnemy].vtxMinEnemy.z = vtx.z;
	//				}
	//				//========================================================================

	//				//頂点フォーマットのサイズ文ポインタを進める
	//				g_aEnemy[nCntEnemy].pVtxBuff += g_aEnemy[nCntEnemy].sizeFVF;

	//			}

	//			//頂点バッファのアンロック
	//			g_aEnemyMat[nCntMat].pMeshEnemy->UnlockVertexBuffer();

	//			//種類特定したし、もうタイプのfor文回す必要ないので、break
	//			break;

	//		}
	//	}
	//}
	//頂点数の取得



	D3DXMATERIAL* pMat;//マテリアルへのポインタ
	for (int nCntENEMYSET = 0; nCntENEMYSET < g_nNumEnemy; nCntENEMYSET++)
	{
		//マテリアルへのポインタを取得（セットしたモデルと同じタイプ）
		pMat = (D3DXMATERIAL*)g_aEnemyMat[nCntENEMYSET].pBuffMatEnemy->GetBufferPointer();
		int nNumTexture = 0;
		for (int nCntMat = 0; nCntMat < (int)g_aEnemyMat[nCntENEMYSET].dwNumMatEnemy; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{//テクスチャファイルが存在する
					//テクスチャの読み込み
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_aEnemyMat[nCntENEMYSET].apTextureEnemy[nNumTexture]);
				nNumTexture++;
			}
		}
	}
	SetEnemy(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ENEMY11_TREASURESLIME);
}

//==============================
//タイトル画面の終了処理
//==============================
void UninitEnemy(void)
{
	for (int nCnt = 0; nCnt < g_nNumEnemy; nCnt++)
	{
		//メッシュの破棄
		if (g_aEnemyMat[nCnt].pMeshEnemy != NULL)
		{
			g_aEnemyMat[nCnt].pMeshEnemy->Release();
			g_aEnemyMat[nCnt].pMeshEnemy = NULL;
		}
		//マテリアルの破棄
		if (g_aEnemyMat[nCnt].pBuffMatEnemy != NULL)
		{
			g_aEnemyMat[nCnt].pBuffMatEnemy->Release();
			g_aEnemyMat[nCnt].pBuffMatEnemy = NULL;
		}
	}
}
//============================
//モデルの更新処理
//============================
void UpdateEnemy(void)
{
	Player* pPlayer = GetPlayer();

	g_nDelayEnemy++;

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			g_aEnemy[nCntEnemy].bIsLanding = bCollisionModel(&g_aEnemy[nCntEnemy].pos, &g_aEnemy[nCntEnemy].posOld, &g_aEnemy[nCntEnemy].move, g_aEnemy[nCntEnemy].Size);

			if (g_aEnemy[nCntEnemy].bIsLanding == false)
			{
				g_aEnemy[nCntEnemy].move.y = -2.0f;//重力処理
			}

			if (g_aEnemy[nCntEnemy].pos.y <= 0.0f)
			{
				g_aEnemy[nCntEnemy].pos.y = 0.0f;
				g_aEnemy[nCntEnemy].move.y = 0.0f;
			}

			switch (g_aEnemy[nCntEnemy].nType)
			{
			case ENEMY00_NOOBSLIME:
				g_aEnemy[nCntEnemy].rot.y = g_aEnemy[nCntEnemy].fVaim + D3DX_PI;
			/*	if (g_nDelayEnemy % (150 + MAX_DELAY) == 0)
				{
					SetBullet(BULLETTYPE_ENEMY, 200,10.0f,10.0f,D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x,g_aEnemy[nCntEnemy].pos.y + 10.0f,g_aEnemy[nCntEnemy].pos.z), D3DXVECTOR3(sinf(g_aEnemy[nCntEnemy].fVaim) * 2.5f, 0.0f, cosf(g_aEnemy[nCntEnemy].fVaim) * 3.5f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),-1,-1.0f);
				}*/
				break;
			case ENEMY01_ANGRYSLIME:
				g_aEnemy[nCntEnemy].rot.y = g_aEnemy[nCntEnemy].fVaim + D3DX_PI;
					if (g_nDelayEnemy % (110 + MAX_DELAY) == 0)
					{
						PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_000);
						float fRandumRot = float(rand() % 100 + 1) / 100;
						for (int nCnt = 0; nCnt < 8; nCnt++)
						{
							float fRot = (2.0f / 8) * nCnt;
							
							SetBullet(BULLETTYPE_ENEMY, 300, 10.0f, 10.0f, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y + g_aEnemy[nCntEnemy].Size.y / 2, g_aEnemy[nCntEnemy].pos.z), D3DXVECTOR3(sinf(D3DX_PI * fRot + fRandumRot) * 5.0f, 0.0f, cosf(D3DX_PI * fRot + fRandumRot) * 5.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),-1,-1.0f);
						}
					}
					break;
			case ENEMY02_SWORDSLIME:
				if (g_nDelayEnemy % (200) == 0)
				{
					PlaySound(SOUND_LABEL_SE_ARROWSHOT_000);

					for (int nCnt = 0; nCnt < 8; nCnt++)
					{
						SetBullet(BULLETTYPE_SWING, 200, 10.0f, 10.0f,
							D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y + 10.0f, g_aEnemy[nCntEnemy].pos.z),
							D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), nCntEnemy, 20.0f * nCnt);
					}
				}
				break;
			case ENEMY03_HAMMERSLIME:
				if (g_nDelayEnemy % (102) == 0)
				{
					PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_001);
					float fRandumRot = float(rand() % 200 + 1) / 100;
					float fRandumSpeed = float(rand() % 100 + 30) / 10;
					float fRandumMove = float(rand() % 21 - 10);
						SetBullet(BULLETTYPE_RANDUM, 420, 15.0f, 15.0f,
							D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y + g_aEnemy[nCntEnemy].Size.y / 2, g_aEnemy[nCntEnemy].pos.z),
							D3DXVECTOR3(sinf(-D3DX_PI * fRandumRot) * fRandumSpeed,0.0f, cosf(-D3DX_PI * fRandumRot) * fRandumSpeed), D3DXVECTOR3(0.0f, 0.0f, 0.0f), nCntEnemy,-1.0f);
				}
				break;
			case ENEMY04_ORANGESLIME:
				g_aEnemy[nCntEnemy].rot.y = g_aEnemy[nCntEnemy].fVaim + D3DX_PI;

				if (g_aEnemy[nCntEnemy].bPenduramFlag == false)
				{
					g_aEnemy[nCntEnemy].fPenduram += 0.03f;
				}
				else if (g_aEnemy[nCntEnemy].bPenduramFlag == true)
				{
					g_aEnemy[nCntEnemy].fPenduram -= 0.03f;
				}

				if (g_aEnemy[nCntEnemy].fPenduram >= 1.0f)
				{
					g_aEnemy[nCntEnemy].bPenduramFlag = true;
				}
				else if (g_aEnemy[nCntEnemy].fPenduram <= -1.0f)
				{
					g_aEnemy[nCntEnemy].bPenduramFlag = false;
				}
				if (g_nDelayEnemy % (87) == 0)
				{
				PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_004);
					float fRandumRot = float(rand() % 200 + 1) / 100;
					float fRandumSpeed = float(rand() % 100 + 30) / 10;
					float fRandumMove = float(rand() % 21 - 10);

					SetBullet(BULLETTYPE_ENEMY, 180, 15.0f, 15.0f,
						D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y + g_aEnemy[nCntEnemy].Size.y / 2, g_aEnemy[nCntEnemy].pos.z),
						D3DXVECTOR3(sinf(g_aEnemy[nCntEnemy].fVaim + g_aEnemy[nCntEnemy].fPenduram) * 7.0f,0.0f, cosf(g_aEnemy[nCntEnemy].fVaim + g_aEnemy[nCntEnemy].fPenduram) * 7.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), nCntEnemy, -1.0f);
				}
				break;
			case ENEMY05_GREENSLIME:
				if (g_nDelayEnemy % (90 + MAX_DELAY) == 0)
				{
				PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_006);
					float fRandumRot = float(rand() % 200 + 1) / 100;
					float fRandumSpeed = float(rand() % 100 + 30) / 10;
					float fRandumMove = float(rand() % 21 - 10);

					for (int nCnt = 0; nCnt < 6; nCnt++)
					{
						float fRot = (2.0f / 6) * nCnt;
						SetBullet(BULLETTYPE_CURVE, 300, 15.0f, 15.0f, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y + g_aEnemy[nCntEnemy].Size.y / 2, g_aEnemy[nCntEnemy].pos.z),
							D3DXVECTOR3(sinf(D3DX_PI * fRot + fRandumRot) * 5.0f, 0.0f, cosf(D3DX_PI * fRot + fRandumRot) * 5.0f), 
							D3DXVECTOR3(0.0f,fRot, 0.0f), -1, -1.0f);
					}
				}
				break;
			case ENEMY06_WHITESLIME:
				g_aEnemy[nCntEnemy].rot.y = g_aEnemy[nCntEnemy].fVaim + D3DX_PI;
				if (g_nDelayEnemy % (120) == 0)
				{
				PlaySound(SOUND_LABEL_SE_FALLINGBLOCK);
					for (int nCnt = 0; nCnt < 3; nCnt++)
					{
						SetBullet(BULLETTYPE_WALL, 300, 10.0f, 10.0f, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y + (30.0f * nCnt), g_aEnemy[nCntEnemy].pos.z), D3DXVECTOR3(sinf(g_aEnemy[nCntEnemy].fVaim) * 5.0f, 0.0f, cosf(g_aEnemy[nCntEnemy].fVaim) * 5.0f), D3DXVECTOR3(0.0f, g_aEnemy[nCntEnemy].fVaim, 0.0f), -1, -1.0f);
					}
				}
				break;
			case ENEMY07_TURQUOISSLIME:
				g_aEnemy[nCntEnemy].rot.y = g_aEnemy[nCntEnemy].fVaim + D3DX_PI;
				if (g_nDelayEnemy % (100 + MAX_DELAY) == 0)
				{
				    PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_007);
					float fRandumRot = float(rand() % 100 + 1) / 100;

					for (int nCnt = 0; nCnt < 10; nCnt++)
					{
						float fRot = (2.0f / 10) * nCnt;
						SetBullet(BULLETTYPE_BOUND, 360, 10.0f, 10.0f, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y + g_aEnemy[nCntEnemy].Size.y / 2, g_aEnemy[nCntEnemy].pos.z), D3DXVECTOR3(sinf(D3DX_PI * fRot + fRandumRot) * 4.0f, 0.0f, cosf(D3DX_PI * fRot + fRandumRot) * 4.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), -1, -1.0f);
					}
				}
				break;
			case ENEMY08_ENGELSLIME:
				g_aEnemy[nCntEnemy].rot.y = g_aEnemy[nCntEnemy].fVaim + D3DX_PI;
				if (g_nDelayEnemy % (82 + MAX_DELAY) == 0)
				{
				PlaySound(SOUND_LABEL_SE_ICICLECLAW);
					//float fRandumRot = float(rand() % 100 + 1) / 100;

					for (int nCnt = 0; nCnt < 3; nCnt++)
					{
						float fRot = (2.0f / 20) * nCnt;
						SetBullet(BULLETTYPE_ENEMY, 300, 20.0f, 20.0f, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y + g_aEnemy[nCntEnemy].Size.y / 2, g_aEnemy[nCntEnemy].pos.z), 
							D3DXVECTOR3(sinf(g_aEnemy[nCntEnemy].fVaim) *( 2.0f + 2.0 * nCnt), sinf(g_aEnemy[nCntEnemy].fVaimY1) * (2.0f + 2.0 * nCnt), cosf(g_aEnemy[nCntEnemy].fVaim) * 2.0f * (2.0f + 2.0 * nCnt)), D3DXVECTOR3(0.0f, 0.0f, 0.0f), -1, -1.0f);
					}
				}
				break;
			case ENEMY09_DARKSLIME:
				g_aEnemy[nCntEnemy].rot.y = g_aEnemy[nCntEnemy].fVaim + D3DX_PI;
				if (g_nDelayEnemy % (113) == 0)
				{
				PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_002);
					
					SetBullet(BULLETTYPE_RING, 420, 10.0f, 10.0f, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y + g_aEnemy[nCntEnemy].Size.y / 2, g_aEnemy[nCntEnemy].pos.z),
						D3DXVECTOR3(sinf(g_aEnemy[nCntEnemy].fVaim) * 6.0f, sinf(g_aEnemy[nCntEnemy].fVaimY1) * 6.0f, cosf(g_aEnemy[nCntEnemy].fVaim) * 6.0f), D3DXVECTOR3(0.0f, g_aEnemy[nCntEnemy].fVaim, 0.0f), -1, -1.0f);

				
				}
				break;
			case ENEMY10_RAINBOWSLIME:
				g_aEnemy[nCntEnemy].rot.y = g_aEnemy[nCntEnemy].fVaim + D3DX_PI;
				if (g_nDelayEnemy % 4  == 0)
				{
					if (g_nDelayEnemy % 28 == 0)
					{
						PlaySound(SOUND_LABEL_SE_SPLETBULLET);
					}
					float fRandumRot = float(rand() % 200 + 1) / 100;
					float fRandumSpeed = float(rand() % 100 + 30) / 10;
					float fRandumMove = float(rand() % 21 - 10);
					float fRandumLength = float(rand() % 550 + 200);

					SetBullet(BULLETTYPE_CHAOS, 420, 10.0f, 10.0f, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + sinf(-D3DX_PI * fRandumRot) * fRandumLength, g_aEnemy[nCntEnemy].pos.y + 600.0f, g_aEnemy[nCntEnemy].pos.z + cosf(-D3DX_PI * fRandumRot) * fRandumLength),
						D3DXVECTOR3(0.0f,0.0f,0.0f), D3DXVECTOR3(0.0f,0.0f, 0.0f), -1, -1.0f);


				}
				break;	
			case ENEMY11_TREASURESLIME:
					g_aEnemy[nCntEnemy].rot.y = g_aEnemy[nCntEnemy].fVaim + D3DX_PI;
					float fRandumRot = float(rand() % 200 + 1) / 100;
					float fRandumSpeed = float(rand() % 100 + 30) / 10;
					float fRandumMove = float(rand() % 21 - 10);
					float fRandumLength = float(rand() % 550 + 200);

					float fRandumColorR = float(rand() % 100 + 1) / 100;
					float fRandumColorG = float(rand() % 100 + 1) / 100;
					float fRandumColorB = float(rand() % 100 + 1) / 100;

					SetParticle(PARTICLETYPE_NORMAL, 200, 30.0f, 0.5f, g_aEnemy[nCntEnemy].pos,
						D3DXVECTOR3(sinf(-D3DX_PI * fRandumRot) * fRandumSpeed, fRandumMove, cosf(-D3DX_PI * fRandumRot) * fRandumSpeed),
						D3DXCOLOR(fRandumColorR, fRandumColorG, fRandumColorB, 1.0f), 0.0f, PARTICLETEX_NORMAL);

					break;
			}

		

			
			g_aEnemy[nCntEnemy].fVXaim = pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x;
			g_aEnemy[nCntEnemy].fVYaim = pPlayer->pos.y - g_aEnemy[nCntEnemy].pos.y;
			g_aEnemy[nCntEnemy].fVZaim = pPlayer->pos.z - g_aEnemy[nCntEnemy].pos.z;

			g_aEnemy[nCntEnemy].fVaim = atan2(g_aEnemy[nCntEnemy].fVXaim, g_aEnemy[nCntEnemy].fVZaim);
			g_aEnemy[nCntEnemy].fVaimY1 = atan2(g_aEnemy[nCntEnemy].fVYaim, g_aEnemy[nCntEnemy].fVZaim);

			g_aEnemy[nCntEnemy].move.x = sinf(g_aEnemy[nCntEnemy].fVaim) * g_aEnemy[nCntEnemy].fSpeed;
			;
			//g_aEnemy[nCntEnemy].move.y = sinf(g_aEnemy[nCntEnemy].fVaimY1) * 1.0f;
			g_aEnemy[nCntEnemy].move.z = cosf(g_aEnemy[nCntEnemy].fVaim) * g_aEnemy[nCntEnemy].fSpeed;
			//移動処理
			g_aEnemy[nCntEnemy].posOld = g_aEnemy[nCntEnemy].pos;
			g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move;

			

			//体力０処理
			if (g_aEnemy[nCntEnemy].nHp <= 0)
			{
				AddCombo(1);//コンボ数を増加させる
				int nCombo = GetCombo();//コンボ数を取得する
				float fScoreBonus = nCombo * 0.1f;
				g_aEnemy[nCntEnemy].bUse = false;//体力０以下なので、倒れる
				AddScore(g_aEnemy[nCntEnemy].nDefeatScore * (1.0f + fScoreBonus));

				if (g_aEnemy[nCntEnemy].nType == ENEMY11_TREASURESLIME)
				{
					int nDrop = rand() % ITEM_MAX;
					SetItem(g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].rot, nDrop);//アイテムをドロップする
				}
				
			}

			SetPositionShadow(g_aEnemy[nCntEnemy].nIdxShadow,D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x,g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].Size.y / 2,g_aEnemy[nCntEnemy].pos.z),-1,nCntEnemy,D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));


			if (g_aEnemy[nCntEnemy].bUse == false)
			{
				
			/*	PlaySound(SOUND_LABEL_SE_DEFEAT_ENEMY);*/
				KillShadow(g_aEnemy[nCntEnemy].nIdxShadow);
			}


		}
	}



}

//================================
//タイトル画面の描画処理
//================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();;//デバイスへのポインタと取得

	D3DXMATRIX mtxRot, mtxTrans;//計算用マトリックス

	D3DMATERIAL9 matDef;//現在のマテリアル保存用

	D3DXMATERIAL* pMat;//マテリアルデータへのポインタ


	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_mtxWorldEnemy);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aEnemy[nCntEnemy].rot.y, g_aEnemy[nCntEnemy].rot.x, g_aEnemy[nCntEnemy].rot.z);

			D3DXMatrixMultiply(&g_mtxWorldEnemy, &g_mtxWorldEnemy, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, g_aEnemy[nCntEnemy].pos.z);
			D3DXMatrixMultiply(&g_mtxWorldEnemy, &g_mtxWorldEnemy, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldEnemy);

			for (int nCntENEMYSET = 0; nCntENEMYSET < g_nNumEnemy; nCntENEMYSET++)
			{//読み込むモデルの種類数分for分を回し、同じタイプのマテリアルのポインタや、、
			//パーツの描画、マテリアルの設定をする。
				if (g_aEnemy[nCntEnemy].nType == nCntENEMYSET)
				{
					//マテリアルへのポインタを取得（セットしたモデルと同じタイプ）
					pMat = (D3DXMATERIAL*)g_aEnemyMat[nCntENEMYSET].pBuffMatEnemy->GetBufferPointer();

					int nNumTexture = 0;
					//セットされたモデルとタイプが同じもののモデルのマテリアルの数分読み込む。
					for (int nCntMat = 0; nCntMat < (int)g_aEnemyMat[nCntENEMYSET].dwNumMatEnemy; nCntMat++)
					{
						//マテリアルの設定
						pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

						if (pMat[nCntMat].pTextureFilename != NULL)
						{
							//テクスチャの設定
							pDevice->SetTexture(0, g_aEnemyMat[nCntENEMYSET].apTextureEnemy[nNumTexture]);
							nNumTexture++;
						}
						else
						{
							//テクスチャの設定
							pDevice->SetTexture(0, g_aEnemyMat[nCntENEMYSET].apTextureEnemy[nNumTexture]);
							nNumTexture++;
						}
						//モデル（パーツ）の描画
						g_aEnemyMat[nCntENEMYSET].pMeshEnemy->DrawSubset(nCntMat);
					}
					break;
				}
			}

			//保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == false)
		{
			g_aEnemy[nCntEnemy].bUse = true;//使用しているかどうか
			g_aEnemy[nCntEnemy].pos = pos;//位置
			g_aEnemy[nCntEnemy].posOld = pos;//1F前の位置
			g_aEnemy[nCntEnemy].rot = rot;//向き
			g_aEnemy[nCntEnemy].nType = nType;//種類
			g_aEnemy[nCntEnemy].fPenduram = 0.0f;//振り子のように弾を発射する変数
			g_aEnemy[nCntEnemy].bPenduramFlag = false;
			g_aEnemy[nCntEnemy].nIdxShadow = SetShadow();

			if (nType == ENEMY11_TREASURESLIME)
			{
				PlaySound(SOUND_LABEL_SE_WARNING_001);
			}

			//=====================================
			//モデルの位置の最大最小値を取得する
			//=====================================
			for (int nCntMat = 0; nCntMat < g_nNumEnemy; nCntMat++)
			{//種類を割り当てるfor文
				if (g_aEnemy[nCntEnemy].nType == nCntMat)
				{//モデルの種類が一致

					//頂点数の取得
					g_aEnemy[nCntEnemy].nNumVtx = g_aEnemyMat[nCntMat].pMeshEnemy->GetNumVertices();

					//頂点フォーマットのサイズを取得
					g_aEnemy[nCntEnemy].sizeFVF = D3DXGetFVFVertexSize(g_aEnemyMat[nCntMat].pMeshEnemy->GetFVF());

					//頂点バッファのロック
					g_aEnemyMat[nCntMat].pMeshEnemy->LockVertexBuffer(D3DLOCK_READONLY, (void**)&g_aEnemy[nCntEnemy].pVtxBuff);


					for (int nCntVtx = 0; nCntVtx < g_aEnemy[nCntEnemy].nNumVtx; nCntVtx++)
					{
						//頂点座標の代入
						D3DXVECTOR3 vtx = *(D3DXVECTOR3*)g_aEnemy[nCntEnemy].pVtxBuff;

						//====================================================
						//頂点座標を比較してモデルの最小値最大値を取得
						//====================================================
						if (vtx.x > g_aEnemy[nCntEnemy].vtxMaxEnemy.x)
						{
							g_aEnemy[nCntEnemy].vtxMaxEnemy.x = vtx.x;
						}
						else if (vtx.x < g_aEnemy[nCntEnemy].vtxMinEnemy.x)
						{//今回読み込んだ頂点が、一番小さい頂点より小さい場合
							g_aEnemy[nCntEnemy].vtxMinEnemy.x = vtx.x;
						}
						else if (vtx.y > g_aEnemy[nCntEnemy].vtxMaxEnemy.y)
						{//今回読み込んだ頂点が、一番小さい頂点より小さい場合
							g_aEnemy[nCntEnemy].vtxMaxEnemy.y = vtx.y;
						}
						else if (vtx.y < g_aEnemy[nCntEnemy].vtxMinEnemy.y)
						{//今回読み込んだ頂点が、一番小さい頂点より小さい場合
							g_aEnemy[nCntEnemy].vtxMinEnemy.y = vtx.y;
						}
						else if (vtx.z > g_aEnemy[nCntEnemy].vtxMaxEnemy.z)
						{//今回読み込んだ頂点が、一番小さい頂点より小さい場合
							g_aEnemy[nCntEnemy].vtxMaxEnemy.z = vtx.z;
						}
						else if (vtx.z < g_aEnemy[nCntEnemy].vtxMinEnemy.z)
						{//今回読み込んだ頂点が、一番小さい頂点より小さい場合
							g_aEnemy[nCntEnemy].vtxMinEnemy.z = vtx.z;
						}
						//========================================================================

						//頂点フォーマットのサイズ文ポインタを進める
						g_aEnemy[nCntEnemy].pVtxBuff += g_aEnemy[nCntEnemy].sizeFVF;

					}

					//頂点バッファのアンロック
					g_aEnemyMat[nCntMat].pMeshEnemy->UnlockVertexBuffer();

					//種類特定したし、もうタイプのfor文回す必要ないので、break
					break;

				}
			}
			//============================================================================================================================

			g_aEnemy[nCntEnemy].Size = g_aEnemy[nCntEnemy].vtxMaxEnemy - g_aEnemy[nCntEnemy].vtxMinEnemy;

			//=========================
			//ステータス設定処理
			//=========================
			switch (g_aEnemy[nCntEnemy].nType)
			{
			case ENEMY00_NOOBSLIME:
				g_aEnemy[nCntEnemy].nHp = 10;//体力
				g_aEnemy[nCntEnemy].nAttack = 5;//攻撃力
				g_aEnemy[nCntEnemy].nDefeatScore = 100;//倒した時にもらえるスコア
				g_aEnemy[nCntEnemy].fSpeed = 1.0f;//速さ
				break;
			case ENEMY01_ANGRYSLIME:
				g_aEnemy[nCntEnemy].nHp = 70;//体力
				g_aEnemy[nCntEnemy].nAttack = 30;//攻撃力
				g_aEnemy[nCntEnemy].nDefeatScore = 300;//倒した時にもらえるスコア
				g_aEnemy[nCntEnemy].fSpeed = 1.5f;//速さ
				break;
			case ENEMY02_SWORDSLIME:
				g_aEnemy[nCntEnemy].nHp = 60;//体力
				g_aEnemy[nCntEnemy].nAttack = 20;//攻撃力
				g_aEnemy[nCntEnemy].nDefeatScore =500;//倒した時にもらえるスコア
				g_aEnemy[nCntEnemy].fSpeed = 3.0f;//速さ
				break;
			case ENEMY03_HAMMERSLIME:
				g_aEnemy[nCntEnemy].nHp = 115;//体力
				g_aEnemy[nCntEnemy].nAttack = 60;//攻撃力
				g_aEnemy[nCntEnemy].nDefeatScore = 500;//倒した時にもらえるスコア
				g_aEnemy[nCntEnemy].fSpeed = 0.5f;//速さ

				break;
			case ENEMY04_ORANGESLIME:
				g_aEnemy[nCntEnemy].nHp = 70;//体力
				g_aEnemy[nCntEnemy].nAttack = 70;//攻撃力
				g_aEnemy[nCntEnemy].nDefeatScore = 800;//倒した時にもらえるスコア
				g_aEnemy[nCntEnemy].fSpeed = 1.6f;//速さ

				break;
			case ENEMY05_GREENSLIME:
				g_aEnemy[nCntEnemy].nHp = 150;//体力
				g_aEnemy[nCntEnemy].nAttack = 50;//攻撃力
				g_aEnemy[nCntEnemy].nDefeatScore = 1000;//倒した時にもらえるスコア
				g_aEnemy[nCntEnemy].fSpeed = 2.2f;//速さ

				break;
			case ENEMY06_WHITESLIME:
				g_aEnemy[nCntEnemy].nHp = 120;//体力
				g_aEnemy[nCntEnemy].nAttack = 50;//攻撃力
				g_aEnemy[nCntEnemy].nDefeatScore = 1000;//倒した時にもらえるスコア
				g_aEnemy[nCntEnemy].fSpeed = 2.2f;//速さ

				break;
			case ENEMY07_TURQUOISSLIME:
				g_aEnemy[nCntEnemy].nHp = 175;//体力
				g_aEnemy[nCntEnemy].nAttack = 110;//攻撃力
				g_aEnemy[nCntEnemy].nDefeatScore = 1500;//倒した時にもらえるスコア
				g_aEnemy[nCntEnemy].fSpeed = 1.8f;//速さ

				break;
			case ENEMY08_ENGELSLIME:
				g_aEnemy[nCntEnemy].nHp = 200;//体力
				g_aEnemy[nCntEnemy].nAttack = 90;//攻撃力
				g_aEnemy[nCntEnemy].nDefeatScore = 2000;//倒した時にもらえるスコア
				g_aEnemy[nCntEnemy].fSpeed = 3.5f;//速さ
				break;
			case ENEMY09_DARKSLIME:
				g_aEnemy[nCntEnemy].nHp = 250;//体力
				g_aEnemy[nCntEnemy].nAttack = 200;//攻撃力
				g_aEnemy[nCntEnemy].nDefeatScore = 2500;//倒した時にもらえるスコア
				g_aEnemy[nCntEnemy].fSpeed = 0.8f;//速さ

				break;
			case ENEMY10_RAINBOWSLIME:
				g_aEnemy[nCntEnemy].nHp = 500;//体力
				g_aEnemy[nCntEnemy].nAttack = 140;//攻撃力
				g_aEnemy[nCntEnemy].nDefeatScore = 5000;//倒した時にもらえるスコア
				g_aEnemy[nCntEnemy].fSpeed = 2.5f;//速さ
				break;
			case ENEMY11_TREASURESLIME:
				g_aEnemy[nCntEnemy].nHp = 777;//体力
				g_aEnemy[nCntEnemy].nAttack = 140;//攻撃力
				g_aEnemy[nCntEnemy].nDefeatScore = 3000;//倒した時にもらえるスコア
				g_aEnemy[nCntEnemy].fSpeed = 1.8f;//速さ
				break;
			}
			//===========================================================
			//g_aEnemy[nCntEnemy].nIdxShadow = SetShadow();//影のインデックスを設定
			break;
		}
	}
}

int GetNumEnemy(void)
{
	return g_nNumEnemy;//読み込むモデルの最大数を取得する
}

Enemy* GetEnemy(void)//敵構造体の情報を取得する
{
	return &g_aEnemy[0];
}

void SetDamageEnemy(int nDamage, int nNumEnemy)
{
	Player* pPlayer = GetPlayer();

	if (pPlayer->bAttackUp == true);
	{
		nDamage *= 2;
	}

	/*PlaySound(SOUND_LABEL_SE_HIT);*/
	g_aEnemy[nNumEnemy].nHp -= nDamage;//体力を減らす
}
