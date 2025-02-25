//========================================================
//
//９月２６日：ハッカソンの基盤作り[Player.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#include "Player.h"
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "camera.h"
#include "shadow.h"
#include "bullet.h"
#include "emitter.h"
#include "block.h"
#include <stdio.h>
#include <string.h>
#include "model.h"
#include "motion.h"
#include "AttackModel.h"
#include "SkillGage.h"
#include "SkillName.h"
#include "AttackBillBoard.h"
#include "combo.h"
//グローバル変数
LPD3DXMESH g_pMeshPlayer = NULL;//メッシュ（頂点情報）へのポインタ
LPD3DXBUFFER g_pBuffMatPlayer = NULL;//マテリアルへのポインタ
DWORD g_dwNumMatPlayer = 0;//マテリアルの数
D3DXVECTOR3 g_posPlayer;//位置
D3DXVECTOR3 g_rotPlayer;//向き
D3DXMATRIX g_mtxWorldPlayer;//ワールドマトリックス
#define Player_SCALE (10.0f)

#define PLAYER_SPEED (7.0f)//プレイヤーの速度

#define PLAYERSET "data\\TEXTFILE\\PlayerSet.txt"//プレイヤーを設定する

Player g_aPlayer;//プレイヤーの情報
ModelParts g_aModelParts[MAX_PLAYERPARTS]; // 各パーツの情報
MatPlayer g_aMatPlayer[NUM_PLAYERPARTS];
int g_nDelay = 0;//ディレイカウント


int g_nNumPlayerModel = 0;//読み込むプレイヤーのパーツの最大数

//タイトル画面の初期化処理
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスへのポインタと取得


	g_nNumPlayerModel = 0;

	//g_posPlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//g_rotPlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//============================
	//モデルパーツの情報の初期化
	//============================
	for (int nCntPlayerParts = 0; nCntPlayerParts < MAX_PLAYERPARTS; nCntPlayerParts++)
	{
		g_aPlayer.aModel[nCntPlayerParts].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
		g_aPlayer.aModel[nCntPlayerParts].rot = D3DXVECTOR3(0.0f,0.0f,0.0f);
		g_aPlayer.aModel[nCntPlayerParts].pMesh = NULL;
		g_aPlayer.aModel[nCntPlayerParts].pBuffMat = NULL;
		g_aPlayer.aModel[nCntPlayerParts].dwNumMatPlayer = 0;
		g_aPlayer.aModel[nCntPlayerParts].mtxWorld;
		g_aPlayer.aModel[nCntPlayerParts].nIdxModelParent = 0;
		g_aPlayer.aModel[nCntPlayerParts].bUse = 0;
		g_aPlayer.aModel[nCntPlayerParts].nType = 0;
		g_aPlayer.aModel[nCntPlayerParts].nNumVtx = 0;//頂点数
		g_aPlayer.aModel[nCntPlayerParts].vtxMinModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//頂点の最小値
		g_aPlayer.aModel[nCntPlayerParts].vtxMaxModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//頂点の最大値
		g_aPlayer.aModel[nCntPlayerParts].sizeFVF;
		g_aPlayer.aModel[nCntPlayerParts].SaveRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//初期向きを保存する
		g_aPlayer.aModel[nCntPlayerParts].SavePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//初期位置を保存する

	}

	/*for (int nCntPlayerParts = 0; nCntPlayerParts < NUM_PLAYERPARTS; nCntPlayerParts++)
	{
		g_aPlayer.aModelMat[nCntPlayerParts].pMesh = NULL;
		g_aPlayer.aModelMat[nCntPlayerParts].pBuffMat = NULL;
		g_aPlayer.aModelMat[nCntPlayerParts].dwNumMatPlayer = 0;
	}*/

	//===========================
	//プレイヤー情報の初期化
	//===========================
	g_aPlayer.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aPlayer.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aPlayer.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aPlayer.targetrot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aPlayer.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aPlayer.nIdxShadow = -1;//影のインデックス
	g_aPlayer.fWidth = 5.0f;
	g_aPlayer.fHeight = 10.0f;
	g_aPlayer.bIsLanding = false;//地面にいるかどうか
	g_aPlayer.nState = PLAYERSTATE_NUTORAL;//プレイヤーの状態を表す変数
	g_aPlayer.Size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//プレイヤー全体の大きさ
	g_aPlayer.vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//プレイヤー全体の最大頂点値
	g_aPlayer.vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//プレイヤー全体の最小頂点値
	g_aPlayer.nMode = 0;//モードの種類
	g_aPlayer.nCntMotionTime = 0;//モーションを使用している時間
	g_aPlayer.bMotionFlag = false;//モーションを使用状態にする
	g_aPlayer.bLandingFlag = false;//着地モーションを使い分ける
	g_aPlayer.nHp = 1500;
	g_aPlayer.nSpecialAttackCount = 0;//スペシャルアタック発動時間をカウントする
	g_aPlayer.bSpecialAttackFlag = false;//スペシャルアタックを発動させるかどうか

	g_aPlayer.bAttackUp = false;//攻撃力上昇状態を付与する
	g_aPlayer.nAttackUpStateCnt = 0;//攻撃力上昇状態中の時間をカウントをする

	g_aPlayer.nBarrierCount = 0;//このカウントの数だけ、被弾時のダメージを無効にする。

	//モデルの読み込み
	D3DXLoadMeshFromX("data\\MODEL\\RealStarWarrior.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatPlayer,
		NULL,
		&g_dwNumMatPlayer,
		&g_pMeshPlayer);

	g_aPlayer.nIdxShadow = SetShadow();

	g_nDelay = 0;//ディレイカウント



	char aString[100] = {};//文字列を読み込むための変数
	int nMaxRoadModel = 0;//読み込むモデルの最大数
	int nIdx = 0;//読み込んだモデルの番号（インデックス）
	int nType = 0;//読み込むモデルの種類

	float fPosX = 0.0f;//モデルの位置X
	float fPosY = 0.0f;//モデルの位置Y
	float fPosZ = 0.0f;//モデルの位置Z

	float fRotX = 0.0f;//モデルの位置X
	float fRotY = 0.0f;//モデルの位置Y
	float fRotZ = 0.0f;//モデルの位置Z
	int nCnt = 0;//読み込むモデルを数える用
	FILE* pFile;//ファイルポインタ

	char aStringPass[100] = {};

	pFile = fopen(PLAYERSET, "r");

	if (pFile != NULL)//ファイルが開いているかどうか
	{
		while (1)
		{

			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(&aString[0], "ENDSCRIPT") == 0)
			{
				fclose(pFile);
				break;//行の最後でこの文字列を判定させることにより、ループを終わらせる
			}
			else if (strcmp(&aString[0], "NUM_MODEL") == 0)//読み込むモデルの種類数を決める
			{
				fscanf(pFile, "%d", &g_nNumPlayerModel);

				for (int nCnt = 0; nCnt < 3; nCnt++)
				{
					fscanf(pFile, "%s", &aString[0]);
				}
				for (int nRoadModel = 0; nRoadModel < g_nNumPlayerModel; nRoadModel++)
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
							&g_aPlayer.aModel[nRoadModel].pBuffMat,//モデルでは、これも配列にする
							NULL,
							&g_aPlayer.aModel[nRoadModel].dwNumMatPlayer,//モデルでは、これも配列にする
							&g_aPlayer.aModel[nRoadModel].pMesh);
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
						fscanf(pFile, "%d", &nType);//インデックスの種類を決める
					}
					else if (strcmp(&aString[0], "IDX") == 0)
					{
						fscanf(pFile, "%d", &nIdx);//インデックスの種類を決める
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
						SetPlayerParts(D3DXVECTOR3(fPosX, fPosY, fPosZ), D3DXVECTOR3(fRotX, fRotY, fRotZ), nIdx,nType);
						break;
					}
				}
			}
		}
	}
	g_aPlayer.nIdxShadow = SetShadow();

	for (int nCntModel = 0; nCntModel < g_nNumPlayerModel; nCntModel++)
	{//全てのモデルに割り当てる
		//for (int nCntMat = 0; nCntMat < g_nNumModel; nCntMat++)
		//{//種類を割り当てるfor文
		//	if (g_aModel[nCntModel].nType == nCntMat)
		//	{//モデルの種類が一致

				//頂点数の取得
				g_aPlayer.aModel[nCntModel].nNumVtx = g_aPlayer.aModel[nCntModel].pMesh->GetNumVertices();

				//頂点フォーマットのサイズを取得
				g_aPlayer.aModel[nCntModel].sizeFVF = D3DXGetFVFVertexSize(g_aPlayer.aModel[nCntModel].pMesh->GetFVF());

				//頂点バッファのロック
				g_aPlayer.aModel[nCntModel].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&g_aPlayer.aModel[nCntModel].pVtxBuff);


				for (int nCntVtx = 0; nCntVtx < g_aPlayer.aModel[nCntModel].nNumVtx; nCntVtx++)
				{
					//頂点座標の代入
					D3DXVECTOR3 vtx = *(D3DXVECTOR3*)g_aPlayer.aModel[nCntModel].pVtxBuff;

					//====================================================
					//頂点座標を比較してモデルの最小値最大値を取得
					//====================================================
					if (vtx.x > g_aPlayer.aModel[nCntModel].vtxMaxModel.x)
					{
						g_aPlayer.aModel[nCntModel].vtxMaxModel.x = vtx.x;
					}
				    else if (vtx.x < g_aPlayer.aModel[nCntModel].vtxMinModel.x)
					{//今回読み込んだ頂点が、一番小さい頂点より小さい場合
						g_aPlayer.aModel[nCntModel].vtxMinModel.x = vtx.x;
					}
					if (vtx.y > g_aPlayer.aModel[nCntModel].vtxMaxModel.y)
					{//今回読み込んだ頂点が、一番小さい頂点より小さい場合
						g_aPlayer.aModel[nCntModel].vtxMaxModel.y = vtx.y;
					}
					else if (vtx.y < g_aPlayer.aModel[nCntModel].vtxMinModel.y)
					{//今回読み込んだ頂点が、一番小さい頂点より小さい場合
						g_aPlayer.aModel[nCntModel].vtxMinModel.y = vtx.y;
					}
					if (vtx.z > g_aPlayer.aModel[nCntModel].vtxMaxModel.z)
					{//今回読み込んだ頂点が、一番小さい頂点より小さい場合
						g_aPlayer.aModel[nCntModel].vtxMaxModel.z = vtx.z;
					}
					else if (vtx.z < g_aPlayer.aModel[nCntModel].vtxMinModel.z)
					{//今回読み込んだ頂点が、一番小さい頂点より小さい場合
						g_aPlayer.aModel[nCntModel].vtxMinModel.z = vtx.z;
					}
					//========================================================================

					//頂点フォーマットのサイズ文ポインタを進める
					g_aPlayer.aModel[nCntModel].pVtxBuff += g_aPlayer.aModel[nCntModel].sizeFVF;

				}

				//頂点バッファのアンロック
				g_aPlayer.aModel[nCntModel].pMesh->UnlockVertexBuffer();

		/*	}
		}*/
	}

	//========================================
	//プレイヤー全体の最大最小値を決める
	//========================================
	for (int nCntModel = 0; nCntModel < g_nNumPlayerModel; nCntModel++)
	{
		if (g_aPlayer.vtxMax.x < g_aPlayer.aModel[nCntModel].vtxMaxModel.x)
		{
			g_aPlayer.vtxMax.x = g_aPlayer.aModel[nCntModel].vtxMaxModel.x;
		}
		else if (g_aPlayer.vtxMin.x > g_aPlayer.aModel[nCntModel].vtxMinModel.x)
		{
			g_aPlayer.vtxMin.x = g_aPlayer.aModel[nCntModel].vtxMaxModel.x;
		}

		if (g_aPlayer.vtxMax.y < g_aPlayer.aModel[nCntModel].vtxMaxModel.y)
		{
			g_aPlayer.vtxMax.y = g_aPlayer.aModel[nCntModel].vtxMaxModel.y;
		}
		else if (g_aPlayer.vtxMin.y > g_aPlayer.aModel[nCntModel].vtxMinModel.y)
		{
			g_aPlayer.vtxMin.y = g_aPlayer.aModel[nCntModel].vtxMaxModel.y;
		}

		if (g_aPlayer.vtxMax.z < g_aPlayer.aModel[nCntModel].vtxMaxModel.z)
		{
			g_aPlayer.vtxMax.z = g_aPlayer.aModel[nCntModel].vtxMaxModel.z;
		}
		else if (g_aPlayer.vtxMin.z > g_aPlayer.aModel[nCntModel].vtxMinModel.z)
		{
			g_aPlayer.vtxMin.x = g_aPlayer.aModel[nCntModel].vtxMaxModel.z;
		}
	}
	//==================================================================================

	//==============================
	//プレイヤーのサイズを決める
	//==============================

	g_aPlayer.Size = g_aPlayer.vtxMax - g_aPlayer.vtxMin;

}

//==============================
//タイトル画面の終了処理
//==============================
void UninitPlayer(void)
{
	for (int nCnt = 0; nCnt < g_nNumPlayerModel; nCnt++)
	{
		//メッシュの破棄
		if (g_aPlayer.aModel[nCnt].pMesh != NULL)
		{
			g_aPlayer.aModel[nCnt].pMesh->Release();
			g_aPlayer.aModel[nCnt].pMesh = NULL;
		}
		//マテリアルの破棄
		if (g_aPlayer.aModel[nCnt].pBuffMat != NULL)
		{
			g_aPlayer.aModel[nCnt].pBuffMat->Release();
			g_aPlayer.aModel[nCnt].pBuffMat = NULL;
		}
	}
}
//============================
//タイトル画面の更新処理
//============================
void UpdatePlayer(void)
{

	/*if (g_aPlayer.pos.y == g_aPlayer.posOld.y)
	{
		g_aPlayer.nState = PLAYERSTATE_LANDING;
	}*/

	Camera* pCamera = GetCamera();

	g_nDelay++;//ディレイカウント

	g_aPlayer.bIsLanding = false;

	g_aPlayer.nState = PLAYERSTATE_NUTORAL;//何もしていない場合、自動的にニュートラル状態になる。

	//for (int nCntPlayerModel = 0; nCntPlayerModel < g_nNumPlayerModel; nCntPlayerModel++)
	//{
		g_aPlayer.bIsLanding = bCollisionModel(&g_aPlayer.pos, &g_aPlayer.posOld, &g_aPlayer.move,g_aPlayer.Size);
	//}


	if (g_aPlayer.pos.y <= 0.0f)
	{
		g_aPlayer.bIsLanding = true;
		g_aPlayer.pos.y = 0.0f;
	}

	if (GetKeyboardTrigger(DIK_F2) == true)
	{
		g_aPlayer.nMode++;

		if (g_aPlayer.nMode == 2)
		{
			g_aPlayer.nMode = 0;
		}
	}
	

	if (g_aPlayer.nMode == 0)
	{
		if (g_aPlayer.bIsLanding == false)
		{
			g_aPlayer.move.y += -1.2f;
		}
		else if (g_aPlayer.bIsLanding == true)
		{
			/*	g_aPlayer.pos.y = 0.0f;*/
			g_aPlayer.move.y = 0.0f;
		}
	}
	else
	{
		if (GetKeyboardPress(DIK_UP) == true)
		{
			g_aPlayer.pos.y += 5.0f;
		}
		else if (GetKeyboardPress(DIK_DOWN) == true)
		{
			g_aPlayer.pos.y += -5.0f;
		}
	}

	Block* pBlock = GetBlock();
	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++, pBlock++)
	{
		if (pBlock->bUse == true)
		{
				if (g_aPlayer.pos.x + g_aPlayer.fWidth >= pBlock->pos.x - 20.0f &&
				    g_aPlayer.pos.x - g_aPlayer.fWidth <= pBlock->pos.x + 20.0f &&
				    g_aPlayer.pos.y <= pBlock->pos.y + 20.0f &&
				    g_aPlayer.posOld.y >= pBlock->pos.y + 20.0f &&
				    g_aPlayer.pos.z + g_aPlayer.fWidth >= pBlock->pos.z - 20.0f &&
				    g_aPlayer.pos.z - g_aPlayer.fWidth <= pBlock->pos.z + 20.0f)
			    {
				  g_aPlayer.pos.y = pBlock->pos.y + 20.0f;
				  g_aPlayer.move.y = 0.0f;
			    }
				else if(g_aPlayer.pos.x + g_aPlayer.fWidth >= pBlock->pos.x - 20.0f &&
					g_aPlayer.pos.x - g_aPlayer.fWidth <= pBlock->pos.x + 20.0f &&
					g_aPlayer.pos.y + g_aPlayer.fHeight >= pBlock->pos.y - 20.0f &&
					g_aPlayer.pos.y <= pBlock->pos.y + 20.0f &&
					g_aPlayer.pos.z + g_aPlayer.fWidth >= pBlock->pos.z - 20.0f &&
					g_aPlayer.pos.z + g_aPlayer.fWidth <= pBlock->pos.z + 20.0f)
				{
					g_aPlayer.move.x = 0.0f;
					g_aPlayer.move.z = 0.0f;
					g_aPlayer.pos = g_aPlayer.posOld;
				}

		}
	}

	

	if (pCamera->nMode == CAMERAMODE_REVOLUTION)
	{
		//if (GetKeyboardPress(DIK_W) == true)
		//{
		//	g_aPlayer.move.x += sinf(D3DX_PI + pCamera->rot.y) * -5.0f;
		//	g_aPlayer.move.z += cosf(D3DX_PI + pCamera->rot.y) * -5.0f;
		//	//g_aPlayer.rot.y = pCamera->rot.y;//カメラと同じ向きにプレイヤーの向きを揃える
		//	g_aPlayer.targetrot.y = pCamera->rot.y;
		//}
		//if (GetKeyboardPress(DIK_S) == true)
		//{
		//	g_aPlayer.move.x += sinf(D3DX_PI + pCamera->rot.y) * 5.0f;
		//	g_aPlayer.move.z += cosf(D3DX_PI + pCamera->rot.y) * 5.0f;
		//	//g_aPlayer.rot.y = pCamera->rot.y + D3DX_PI;//カメラの向きの後ろ側にプレイヤーの向きを揃える
		//	g_aPlayer.targetrot.y = pCamera->rot.y + D3DX_PI;//カメラの向きの後ろ側にプレイヤーの向きを揃える

		//}
		//if (GetKeyboardPress(DIK_D) == true)
		//{

		//	g_aPlayer.move.x += sinf((D3DX_PI + pCamera->rot.y) - (D3DX_PI / 2)) * 5.0f;
		//	g_aPlayer.move.z += cosf((D3DX_PI + pCamera->rot.y) - (D3DX_PI / 2)) * 5.0f;
		//	//g_aPlayer.rot.y = pCamera->rot.y + (D3DX_PI / 2);//カメラの向きの右横にプレイヤーの向きを揃える
		//	g_aPlayer.targetrot.y = pCamera->rot.y + (D3DX_PI / 2);//カメラの向きの右横にプレイヤーの向きを揃える

		//}
		//if (GetKeyboardPress(DIK_A) == true)
		//{
		//	g_aPlayer.move.x += sinf((D3DX_PI + pCamera->rot.y) - (D3DX_PI / 2)) * -5.0f;
		//	g_aPlayer.move.z += cosf((D3DX_PI + pCamera->rot.y) - (D3DX_PI / 2)) * -5.0f;
		//	//g_aPlayer.rot.y = pCamera->rot.y - (D3DX_PI / 2);//カメラの向きの左側にプレイヤーの向きを揃える
		//	g_aPlayer.targetrot.y = pCamera->rot.y - (D3DX_PI / 2);//カメラの向きの左側にプレイヤーの向きを揃える

		//}

		//=======================
		//ニュータイプ移動処理
		//=======================

		if (GetKeyboardPress(DIK_A) == true || GetJoypadPress(JOYKEY_LEFT) == true)
		{//Aキーが押された
			if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP))
			{//Wキーが押された
				//左上移動
				g_aPlayer.nState = PLAYERSTATE_MOVE;//移動状態

				g_aPlayer.move.x += sinf((D3DX_PI + pCamera->rot.y) - (D3DX_PI * 0.25f)) * -PLAYER_SPEED;
				g_aPlayer.move.z += cosf((D3DX_PI + pCamera->rot.y) - (D3DX_PI * 0.25f)) * -PLAYER_SPEED;
				//g_aPlayer.rot.y = pCamera->rot.y - (D3DX_PI / 2);//カメラの向きの左側にプレイヤーの向きを揃える
				g_aPlayer.targetrot.y = pCamera->rot.y - (D3DX_PI * 0.25f) + D3DX_PI;//カメラの向きの左側にプレイヤーの向きを揃える
			}
			else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true)
			{//Sキーが押された
				//左下移動
				g_aPlayer.nState = PLAYERSTATE_MOVE;//移動状態

				g_aPlayer.move.x += sinf((D3DX_PI + pCamera->rot.y) - (D3DX_PI * 0.75f)) * -PLAYER_SPEED;
				g_aPlayer.move.z += cosf((D3DX_PI + pCamera->rot.y) - (D3DX_PI * 0.75f)) * -PLAYER_SPEED;
				//g_aPlayer.rot.y = pCamera->rot.y - (D3DX_PI / 2);//カメラの向きの左側にプレイヤーの向きを揃える
				g_aPlayer.targetrot.y = pCamera->rot.y - (D3DX_PI * 0.75f) + D3DX_PI;//カメラの向きの左側にプレイヤーの向きを揃える
			}
			else
			{
				g_aPlayer.nState = PLAYERSTATE_MOVE;//移動状態

				g_aPlayer.move.x += sinf((D3DX_PI + pCamera->rot.y) - (D3DX_PI / 2)) * -PLAYER_SPEED;
				g_aPlayer.move.z += cosf((D3DX_PI + pCamera->rot.y) - (D3DX_PI / 2)) * -PLAYER_SPEED;
				//g_aPlayer.rot.y = pCamera->rot.y - (D3DX_PI / 2);//カメラの向きの左側にプレイヤーの向きを揃える
				g_aPlayer.targetrot.y = pCamera->rot.y - (D3DX_PI / 2) + D3DX_PI;//カメラの向きの左側にプレイヤーの向きを揃える
			}
		}


		else if (GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT) == true)
		{//Dキーが押された

			if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true)
			{//Wきーが押された
				g_aPlayer.nState = PLAYERSTATE_MOVE;//移動状態
				//右上移動
				g_aPlayer.move.x += sinf((D3DX_PI + pCamera->rot.y) - (D3DX_PI * 0.75f)) * PLAYER_SPEED;
				g_aPlayer.move.z += cosf((D3DX_PI + pCamera->rot.y) - (D3DX_PI * 0.75f)) * PLAYER_SPEED;
				//g_aPlayer.rot.y = pCamera->rot.y - (D3DX_PI / 2);//カメラの向きの左側にプレイヤーの向きを揃える
				g_aPlayer.targetrot.y = pCamera->rot.y + (D3DX_PI * 0.25f) + D3DX_PI;//カメラの向きの左側にプレイヤーの向きを揃える
			}
			else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true)
			{//Sきーが押されたs
				g_aPlayer.nState = PLAYERSTATE_MOVE;//移動状態
				//右下移動
				g_aPlayer.move.x += sinf((D3DX_PI + pCamera->rot.y) - (D3DX_PI * 0.25f)) * PLAYER_SPEED;
				g_aPlayer.move.z += cosf((D3DX_PI + pCamera->rot.y) - (D3DX_PI * 0.25f)) * PLAYER_SPEED;
				//g_aPlayer.rot.y = pCamera->rot.y - (D3DX_PI / 2);//カメラの向きの左側にプレイヤーの向きを揃える
				g_aPlayer.targetrot.y = pCamera->rot.y + (D3DX_PI * 0.75f) + D3DX_PI;//カメラの向きの左側にプレイヤーの向きを揃える
			}
			else
			{//右移動
				g_aPlayer.nState = PLAYERSTATE_MOVE;//移動状態
				g_aPlayer.move.x += sinf((D3DX_PI + pCamera->rot.y) - (D3DX_PI / 2)) * PLAYER_SPEED;
				g_aPlayer.move.z += cosf((D3DX_PI + pCamera->rot.y) - (D3DX_PI / 2)) * PLAYER_SPEED;
				//g_aPlayer.rot.y = pCamera->rot.y + (D3DX_PI / 2);//カメラの向きの右横にプレイヤーの向きを揃える
				g_aPlayer.targetrot.y = pCamera->rot.y + (D3DX_PI / 2) + D3DX_PI;//カメラの向きの右横にプレイヤーの向きを揃える
			}
		}
		else if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true)
		{//Wキーが押されたw
			g_aPlayer.nState = PLAYERSTATE_MOVE;//移動状態
			g_aPlayer.move.x += sinf(D3DX_PI + pCamera->rot.y) * -PLAYER_SPEED;
			g_aPlayer.move.z += cosf(D3DX_PI + pCamera->rot.y) * -PLAYER_SPEED;
			//g_aPlayer.rot.y = pCamera->rot.y;//カメラと同じ向きにプレイヤーの向きを揃える
			g_aPlayer.targetrot.y = pCamera->rot.y + D3DX_PI;
		}
		else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true)
		{//Sキーが押された
			g_aPlayer.nState = PLAYERSTATE_MOVE;//移動状態
			g_aPlayer.move.x += sinf(D3DX_PI + pCamera->rot.y) * PLAYER_SPEED;
			g_aPlayer.move.z += cosf(D3DX_PI + pCamera->rot.y) * PLAYER_SPEED;
			//g_aPlayer.rot.y = pCamera->rot.y + D3DX_PI;//カメラの向きの後ろ側にプレイヤーの向きを揃える
			g_aPlayer.targetrot.y = pCamera->rot.y;//カメラの向きの後ろ側にプレイヤーの向きを揃える
		}
	}
	else if (pCamera->nMode == CAMERAMODE_ROTATION)
	{
		if (GetKeyboardPress(DIK_W) == true)
		{
			g_aPlayer.nState = PLAYERSTATE_MOVE;//移動状態
			g_aPlayer.move.x += sinf(D3DX_PI + pCamera->rot.y) * PLAYER_SPEED;
			g_aPlayer.move.z += cosf(D3DX_PI + pCamera->rot.y) * PLAYER_SPEED;
			//g_aPlayer.rot.y = pCamera->rot.y;//カメラと同じ向きにプレイヤーの向きを揃える
			g_aPlayer.targetrot.y = pCamera->rot.y;//カメラと同じ向きにプレイヤーの向きを揃える

		}
		if (GetKeyboardPress(DIK_S) == true)
		{
			g_aPlayer.nState = PLAYERSTATE_MOVE;//移動状態
			g_aPlayer.move.x += sinf(D3DX_PI + pCamera->rot.y) * -PLAYER_SPEED;
			g_aPlayer.move.z += cosf(D3DX_PI + pCamera->rot.y) * -PLAYER_SPEED;
			//g_aPlayer.rot.y = pCamera->rot.y + D3DX_PI;//カメラの向きの後ろ側にプレイヤーの向きを揃える
			g_aPlayer.targetrot.y = pCamera->rot.y + D3DX_PI;//カメラの向きの後ろ側にプレイヤーの向きを揃える


		}
		if (GetKeyboardPress(DIK_D) == true)
		{
			g_aPlayer.nState = PLAYERSTATE_MOVE;//移動状態

			g_aPlayer.move.x += sinf((D3DX_PI + pCamera->rot.y) - (D3DX_PI / 2)) * -PLAYER_SPEED;
			g_aPlayer.move.z += cosf((D3DX_PI + pCamera->rot.y) - (D3DX_PI / 2)) * -PLAYER_SPEED;
			//g_aPlayer.rot.y = pCamera->rot.y + (D3DX_PI / 2);//カメラの向きの右横にプレイヤーの向きを揃える
			g_aPlayer.targetrot.y = pCamera->rot.y + (D3DX_PI / 2);//カメラの向きの右横にプレイヤーの向きを揃える


		}
		if (GetKeyboardPress(DIK_A) == true)
		{
			g_aPlayer.nState = PLAYERSTATE_MOVE;//移動状態

			g_aPlayer.move.x += sinf((D3DX_PI + pCamera->rot.y) - (D3DX_PI / 2)) * 5.0f;
			g_aPlayer.move.z += cosf((D3DX_PI + pCamera->rot.y) - (D3DX_PI / 2)) * 5.0f;
			//g_aPlayer.rot.y = pCamera->rot.y - (D3DX_PI / 2);//カメラの向きの左側にプレイヤーの向きを揃える
			g_aPlayer.targetrot.y = pCamera->rot.y - (D3DX_PI / 2);//カメラの向きの左側にプレイヤーの向きを揃える

		}
	}

	//=====================================================================================================================

	////移動量を更新（減衰させる）
	g_aPlayer.move.x += (0.0f - g_aPlayer.move.x) * 0.5f;

	if (g_aPlayer.bIsLanding == true)
	{
		if (GetKeyboardTrigger(DIK_SPACE) == true || GetJoyPadTrigger(JOYKEY_A) == true)
		{
			PlaySound(SOUND_LABEL_SE_JUMP);
			g_aPlayer.move.y += 20.0f;
		}
	}

	if (g_aPlayer.pos.y > g_aPlayer.posOld.y)//1fまえの位置より高かったら
	{
		g_aPlayer.nState = PLAYERSTATE_JUMP;
	}


	//g_aPlayer.move.y += (0.0f - g_aPlayer.move.y) * 0.5f;
	g_aPlayer.move.z += (0.0f - g_aPlayer.move.z) * 0.5f;
	//g_aPlayer.move.y += (0.0f - g_aPlayer.move.y) * 0.5f;
	g_aPlayer.posOld = g_aPlayer.pos;
	g_aPlayer.pos.y += g_aPlayer.move.y;
	g_aPlayer.pos.x += g_aPlayer.move.x;
	g_aPlayer.pos.z += g_aPlayer.move.z;


	//弾発射処理
	if (GetKeyboardPress(DIK_K) == true || GetJoypadPress(JOYKEY_X) == true)
	{
		SetBullet(0, 200, 20.0f, 20.0f,
			D3DXVECTOR3(g_aPlayer.pos.x, g_aPlayer.pos.y + 20.0f, g_aPlayer.pos.z), D3DXVECTOR3(sinf(g_aPlayer.rot.y + D3DX_PI) * 10.0f, 0.0f, cosf(g_aPlayer.rot.y + D3DX_PI) * 10.0f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),-1,-1.0f);
	}

	if (GetKeyboardTrigger(DIK_K) == true || GetJoyPadTrigger(JOYKEY_X) == true)
	{
		PlaySound(SOUND_LABEL_SE_SLASH_000);
		SetAttackBillBoard(A_BILLBOARDTYPE_SLASH, 30, 50, 20, D3DXVECTOR3(g_aPlayer.pos.x, g_aPlayer.pos.y + g_aPlayer.Size.y / 2, g_aPlayer.pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}



	

	
	//============================
	//スキル発動処理
	//============================
	SkillGage* pSKillGage = GetSkillGage();
	Skill* pSkill = GetSkill();
	if (pSKillGage->fSkillGage >= 50.0f &&pSkill->nSkillType == SKILLTYPE_FIRETORNADO)
	{
		if (GetKeyboardTrigger(DIK_M) == true || GetJoyPadTrigger(JOYKEY_B) == true)
		{
			PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_008);
			g_aPlayer.bMotionFlag = true;
			pSKillGage->fSkillGage -= 50.0f;
			SetAttackModel(g_aPlayer.pos, g_aPlayer.rot, D3DXVECTOR3(sinf(g_aPlayer.rot.y + D3DX_PI) * 10.0f, 0.0f, cosf(g_aPlayer.rot.y + D3DX_PI) * 10.0f), ATTACKMODELTYPE_FIRETORNADO, 300);
		}
	}
	else if (pSKillGage->fSkillGage >= 100.0f && pSkill->nSkillType == SKILLTYPE_THUNDERBURST)
	{
		if (GetKeyboardTrigger(DIK_M) == true || GetJoyPadTrigger(JOYKEY_B) == true)
		{
			PlaySound(SOUND_LABEL_SE_THUNDER);
			g_aPlayer.bMotionFlag = true;
			pSKillGage->fSkillGage -= 100.0f;
			SetAttackBillBoard(A_BILLBOARDTYPE_THUNDER, 500, 50.0f, 500.0f, D3DXVECTOR3(g_aPlayer.pos.x + sinf(g_aPlayer.rot.y + D3DX_PI) * 200.0f, g_aPlayer.pos.y, g_aPlayer.pos.z + cosf(g_aPlayer.rot.y + D3DX_PI) * 200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetAttackBillBoard(A_BILLBOARDTYPE_THUNDER, 500, 50.0f, 500.0f, D3DXVECTOR3(g_aPlayer.pos.x + sinf(g_aPlayer.rot.y + D3DX_PI + D3DX_PI * 0.5f) * 200.0f, g_aPlayer.pos.y, g_aPlayer.pos.z + cosf(g_aPlayer.rot.y + D3DX_PI + D3DX_PI * 0.5f) * 200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetAttackBillBoard(A_BILLBOARDTYPE_THUNDER, 500, 50.0f, 500.0f, D3DXVECTOR3(g_aPlayer.pos.x + sinf(g_aPlayer.rot.y) * 200.0f, g_aPlayer.pos.y, g_aPlayer.pos.z + cosf(g_aPlayer.rot.y) * 200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetAttackBillBoard(A_BILLBOARDTYPE_THUNDER, 500, 50.0f, 500.0f, D3DXVECTOR3(g_aPlayer.pos.x + sinf(g_aPlayer.rot.y + D3DX_PI - D3DX_PI * 0.5f) * 200.0f, g_aPlayer.pos.y, g_aPlayer.pos.z + cosf(g_aPlayer.rot.y + D3DX_PI - D3DX_PI * 0.5f) * 200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		}
	}
	else if (pSKillGage->fSkillGage >= 200.0f && pSkill->nSkillType == SKILLTYPE_METEOBALL)
	{
		if (GetKeyboardTrigger(DIK_M) == true || GetJoyPadTrigger(JOYKEY_B) == true)
		{
			PlaySound(SOUND_LABEL_SE_EXPLOSION_ENEMY_000);
			g_aPlayer.bMotionFlag = true;
			pSKillGage->fSkillGage -= 200.0f;
			SetAttackModel(D3DXVECTOR3(g_aPlayer.pos.x, g_aPlayer.pos.y + 500.0f, g_aPlayer.pos.z - 200.0f), g_aPlayer.rot, D3DXVECTOR3(sinf(g_aPlayer.rot.y + D3DX_PI) * 10.0f, -10.0f, cosf(g_aPlayer.rot.y + D3DX_PI) * 10.0f), ATTACKMODELTYPE_METEOBALL, 500);
		}
	}
	else if (pSKillGage->fSkillGage >= 150.0f && pSkill->nSkillType == SKILLTYPE_KILLNEEDLE)
	{
		PlaySound(SOUND_LABEL_SE_ICICLECLAW);
		if (GetKeyboardTrigger(DIK_M) == true || GetJoyPadTrigger(JOYKEY_B) == true)
		{
			g_aPlayer.bMotionFlag = true;
			pSKillGage->fSkillGage -= 150.0f;
			for (int nCnt = 0; nCnt < 16; nCnt++)
			{
				float fRot = (2.0f / 16) * nCnt;
			SetAttackModel(D3DXVECTOR3(g_aPlayer.pos.x + sinf(g_aPlayer.rot.y + D3DX_PI * fRot) * 200.0f, g_aPlayer.pos.y, g_aPlayer.pos.z + cosf(g_aPlayer.rot.y + D3DX_PI * fRot) * 200.0f), g_aPlayer.rot, D3DXVECTOR3(0.0f, 0.0f, 0.0f), ATTACKMODELTYPE_KILLNEEDLE, 900);
			}
		}
	}
	else if (pSKillGage->fSkillGage >= 300.0f && pSkill->nSkillType == SKILLTYPE_CHAINFIRE)
	{
		if (GetKeyboardTrigger(DIK_M) == true || GetJoyPadTrigger(JOYKEY_B) == true)
		{
			PlaySound(SOUND_LABEL_SE_PUNCH);
			g_aPlayer.bMotionFlag = true;
			pSKillGage->fSkillGage -= 300.0f;
			for (int nCnt = 0; nCnt < 9; nCnt++)
			{
				SetBullet(BULLETTYPE_FIRE, 300, 15.0f, 15.0f,
					D3DXVECTOR3(g_aPlayer.pos.x, g_aPlayer.pos.y + g_aPlayer.Size.y / 2, g_aPlayer.pos.z), D3DXVECTOR3(sinf(g_aPlayer.rot.y + D3DX_PI - 0.4f + (nCnt * 0.1f)) * 12.0f, 0.0f, cosf(g_aPlayer.rot.y + D3DX_PI - 0.3f + (nCnt * 0.1f)) * 12.0f),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), -1, -1.0f);
			}
		}
	}
	else if (pSKillGage->fSkillGage >= 1000.0f && pSkill->nSkillType == SKILLTYPE_ATOMICSLASH)
	{
		if (GetKeyboardTrigger(DIK_M) == true || GetJoyPadTrigger(JOYKEY_B) == true)
		{
			PlaySound(SOUND_LABEL_SE_FLASHBLADE);
			g_aPlayer.bMotionFlag = true;
			pSKillGage->fSkillGage -= 1000.0f;

			g_aPlayer.bSpecialAttackFlag = true;//スペシャルアタックを発動する。
		}
	}
	//============================
	//状態強化処理
	//============================
	if (g_aPlayer.nAttackUpStateCnt > 0)
	{
		if (g_nDelay % 60 == 0)
		{
			g_aPlayer.nAttackUpStateCnt -= 1;
		}

		g_aPlayer.bAttackUp = true;
	}
	else
	{
		g_aPlayer.bAttackUp = false;
	}




	//========================================================


	//========================
	//スペシャルアタック処理
	//========================
	if (g_aPlayer.bSpecialAttackFlag == true)
	{
		float fRandumRot = float(rand() % 200 + 1) / 100;
		float fRandumSpeed = float(rand() % 100 + 30) / 10;
		float fRandumMove = float(rand() % 21 - 10);
		float fRandumLength = float(rand() % 500);
		float fRandumPos = float(rand() % 1000 - 500);

		g_aPlayer.nSpecialAttackCount++;

		SetAttackBillBoard(A_BILLBOARDTYPE_ATOMICSLASH,120, 100.0f, 35.0f, 
			D3DXVECTOR3(g_aPlayer.pos.x + sinf(-D3DX_PI * fRandumRot) * fRandumLength,g_aPlayer.pos.y + fRandumPos,g_aPlayer.pos.z + cosf(-D3DX_PI * fRandumRot) * fRandumLength),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		if (g_aPlayer.nSpecialAttackCount == 900)
		{
			g_aPlayer.nSpecialAttackCount = 0;
			g_aPlayer.bSpecialAttackFlag = false;
		}
	}

	//=============================================================================================

	if (g_aPlayer.bMotionFlag == true)
	{
		g_aPlayer.nState = PLAYERSTATE_ATTACK;
	/*	g_aPlayer.nCntMotionTime++;
		if (g_aPlayer.nCntMotionTime >= 80)
		{
			g_aPlayer.nCntMotionTime = 0;
			g_aPlayer.bMotionFlag = false;
		}*/
	}




	float fRot1 = 0.0f;
	float fRot2 = 0.0f;

	//if (g_aPlayer.targetrot.y >= pCamera->rot.y - (D3DX_PI * 0.15f)
	//	&& g_aPlayer.targetrot.y <= pCamera->rot.y + (D3DX_PI * 0.15f))
	//{
	//	fRot1 = pCamera->rot.y - D3DX_PI;
	//}
	//else
	//{
	//	fRot1 = pCamera->rot.y;
	//}



	g_aPlayer.rot.y += ((g_aPlayer.targetrot.y + fRot1) - g_aPlayer.rot.y) * 0.5f;

	//g_aPlayer.rot.y = g_aPlayer.targetrot.y;


	//g_aPlayer.Size.x = sinf(g_aPlayer.rot.y * D3DX_PI) * 
	

	


	//影の位置を設定
	SetPositionShadow(g_aPlayer.nIdxShadow, g_aPlayer.pos,-1,-1,D3DXCOLOR(1.0f,1.0f,0.0f,1.0f));
	
}

//================================
//タイトル画面の描画処理
//================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();;//デバイスへのポインタと取得


	D3DXMATRIX mtxRot, mtxTrans;//計算用マトリックス

	D3DMATERIAL9 matDef;//現在のマテリアル保存用

	D3DXMATERIAL* pMat;//マテリアルデータへのポインタ

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldPlayer);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayer.rot.y, g_aPlayer.rot.x, g_aPlayer.rot.z);

	D3DXMatrixMultiply(&g_mtxWorldPlayer, &g_mtxWorldPlayer, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_aPlayer.pos.x, g_aPlayer.pos.y, g_aPlayer.pos.z);
	D3DXMatrixMultiply(&g_mtxWorldPlayer, &g_mtxWorldPlayer, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldPlayer);

	////現在のマテリアルを取得
	//pDevice->GetMaterial(&matDef);

	////マテリアルへのポインタを取得
	//pMat = (D3DXMATERIAL*)g_pBuffMatPlayer->GetBufferPointer();

	//for (int nCntMat = 0; nCntMat < (int)g_dwNumMatPlayer; nCntMat++)
	//{
	//	//マテリアルの設定
	//	pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

	//	//テクスチャの設定
	//	pDevice->SetTexture(0, NULL);

	//	//モデル（パーツ）の描画
	//	g_pMeshPlayer->DrawSubset(nCntMat);
	//}

	////保存していたマテリアルを戻す
	//pDevice->SetMaterial(&matDef);


	//=========================================
	//モデルパーツの描画処理
	//=========================================
	for (int nCntPlayerParts = 0; nCntPlayerParts < g_nNumPlayerModel; nCntPlayerParts++)
	{
		if (g_aPlayer.aModel[nCntPlayerParts].bUse == true)
		{
			D3DXMATRIX mtxRot, mtxTrans;//計算用マトリックス

			D3DMATERIAL9 matDef;//現在のマテリアル保存用

			D3DXMATERIAL* pMat;//マテリアルデータへのポインタ

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aPlayer.aModel[nCntPlayerParts].mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayer.aModel[nCntPlayerParts].rot.y, g_aPlayer.aModel[nCntPlayerParts].rot.x, g_aPlayer.aModel[nCntPlayerParts].rot.z);

			D3DXMatrixMultiply(&g_aPlayer.aModel[nCntPlayerParts].mtxWorld, &g_aPlayer.aModel[nCntPlayerParts].mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aPlayer.aModel[nCntPlayerParts].pos.x, g_aPlayer.aModel[nCntPlayerParts].pos.y, g_aPlayer.aModel[nCntPlayerParts].pos.z);
			D3DXMatrixMultiply(&g_aPlayer.aModel[nCntPlayerParts].mtxWorld, &g_aPlayer.aModel[nCntPlayerParts].mtxWorld, &mtxTrans);

			//===================================================
			//自分のモデルのマトリックスとの掛け算
			//===================================================
			D3DXMATRIX mtxParent;

			//D3DXMatrixMultiply(&g_aPlayer.aModel[nCntPlayerParts].mtxWorld, &g_aPlayer.aModel[nCntPlayerParts].mtxWorld, &mtxTrans);

			if (g_aPlayer.aModel[nCntPlayerParts].nIdxModelParent == -1)
			{
				mtxParent = g_mtxWorldPlayer;//親を起点として動くので、マトリックスはプレイヤー
			}
			else
			{//mtxParentを今のモデルの親のマトリックスワールドにする
				int nData = g_aPlayer.aModel[nCntPlayerParts].nIdxModelParent;
			    
         		mtxParent = g_aPlayer.aModel[nData].mtxWorld;
			}

			D3DXMatrixMultiply(&g_aPlayer.aModel[nCntPlayerParts].mtxWorld, &g_aPlayer.aModel[nCntPlayerParts].mtxWorld, &mtxParent);


			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aPlayer.aModel[nCntPlayerParts].mtxWorld);

		    //===============================================================================================================================


			pDevice->GetMaterial(&matDef);

					//マテリアルへのポインタを取得（セットしたモデルと同じタイプ）
					pMat = (D3DXMATERIAL*)g_aPlayer.aModel[nCntPlayerParts].pBuffMat->GetBufferPointer();

					//セットされたモデルとタイプが同じもののモデルのマテリアルの数分読み込む。
					for (int nCntMat = 0; nCntMat < (int)g_aPlayer.aModel[nCntPlayerParts].dwNumMatPlayer; nCntMat++)
					{
						//マテリアルの設定
						pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

						//テクスチャの設定
						pDevice->SetTexture(0, NULL/*g_aPlayerPartsMat[nCntPlayerPartsSet].apTexturePlayerParts*/);

						//モデル（パーツ）の描画
						g_aPlayer.aModel[nCntPlayerParts].pMesh->DrawSubset(nCntMat);
					}
			//保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

Player* GetPlayer(void)
{
	return &g_aPlayer;
}


void SetPlayerParts(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nIdx, int nType)
{
	for (int nCntPlayerParts = 0; nCntPlayerParts < MAX_PLAYERPARTS; nCntPlayerParts++)
	{
		if (g_aPlayer.aModel[nCntPlayerParts].bUse == false)
		{
			g_aPlayer.aModel[nCntPlayerParts].bUse = true;//使用しているかどうか
			g_aPlayer.aModel[nCntPlayerParts].pos = pos;//位置
			g_aPlayer.aModel[nCntPlayerParts].SavePos = pos;//位置
			g_aPlayer.aModel[nCntPlayerParts].rot = rot;//向き
			g_aPlayer.aModel[nCntPlayerParts].SaveRot = rot;//向き
			g_aPlayer.aModel[nCntPlayerParts].nIdxModelParent = nIdx;
			g_aPlayer.aModel[nCntPlayerParts].nType = nType;
			break;
		}
	}
}

void SetDamagePlayer(int nDamage)
{

	if (g_aPlayer.nBarrierCount > 0)
	{
		g_aPlayer.nBarrierCount -= 1;
		nDamage = 0;
		PlaySound(SOUND_LABEL_SE_BARRIER);
	}
	else
	{
		PlaySound(SOUND_LABEL_SE_DAMAGE_000);
		g_aPlayer.nHp -= nDamage;
		ResetCombo();
	}
}

bool bPlayerDeath(void)//プレイヤーが死んだかどうかの判定を返す
{
	bool bDeath = false;

	if (g_aPlayer.nHp <= 0)
	{
		bDeath = true;
		g_aPlayer.bUse = false;
	}
	return bDeath;
}


