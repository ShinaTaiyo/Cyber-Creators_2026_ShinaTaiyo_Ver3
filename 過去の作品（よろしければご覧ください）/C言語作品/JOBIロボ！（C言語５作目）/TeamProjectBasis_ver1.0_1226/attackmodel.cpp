//================================================================================================================================================================================================================================
//
// 攻撃モデルの処理
// Author : Taiyo Shina
//
//================================================================================================================================================================================================================================
#include "attackModel.h"
#include "game.h"
#include "input.h"
#include "enemy.h"
#include "player.h"
#include "particle.h"
#include "effect.h"
#include "battery.h"
#include "boss.h"
#include <math.h>
#include "model.h"
#include "emitter.h"
#include "dummy.h"
#include "field.h"
#include "shadow.h"
#include "sound.h"

//================================================================================================================================================================================================================================
// グローバル変数
//================================================================================================================================================================================================================================
LPDIRECT3DTEXTURE9 g_apTextureAttackModel[NUM_ATTACKMODEL][MAX_ATTACKMODELMAT] = {};//テクスチャへのポインタ
AttackModel g_aAttackModel[MAX_ATTACKMODEL];										//オブジェクト情報
bool g_bIsLandingAttackModel;														//モデルの上にいるかどうかを判定する
D3DXVECTOR3 g_PlayerPosLength;														//プレイヤーの位置の絶対値を求める
int g_nDelayAttackModel;															//ディレイ

AttackModelInfo g_aAttackModelInfo[NUM_ATTACKMODEL];                                //攻撃モデルの情報

//================================================================================================================================================================================================================================
// 初期化処理
//================================================================================================================================================================================================================================
void InitAttackModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスの取得
	D3DXMATERIAL* pMat;						//マテリアルへのポインタ

	//モデル情報の初期化
	for (int nCntModelInfo = 0; nCntModelInfo < NUM_ATTACKMODEL; nCntModelInfo++)
	{
		g_aAttackModelInfo[nCntModelInfo].dwNumMat = {};
		g_aAttackModelInfo[nCntModelInfo].pBuffMat = {};
		g_aAttackModelInfo[nCntModelInfo].pMesh = {};

		for (int nCntMat = 0; nCntMat < MAX_ATTACKMODELMAT; nCntMat++)
		{
			g_aAttackModelInfo[nCntModelInfo].pTexture[nCntMat] = {};
		}
	}

	//================================
	//構造体の情報の初期化
	//================================
	for (int nCntAttackModel = 0; nCntAttackModel < MAX_ATTACKMODEL; nCntAttackModel++)
	{
		g_aAttackModel[nCntAttackModel].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//位置
		g_aAttackModel[nCntAttackModel].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//移動量
		g_aAttackModel[nCntAttackModel].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//向き
		g_aAttackModel[nCntAttackModel].Vrot = NULL_VECTOR3;                    //ベクトルを考慮した向き
		g_aAttackModel[nCntAttackModel].Scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  //拡大率
		g_aAttackModel[nCntAttackModel].fRot = 0.0f;                            //動く向きを決める
		g_aAttackModel[nCntAttackModel].nType = 0;								//種類
		g_aAttackModel[nCntAttackModel].bUse = false;							//使用している
		g_aAttackModel[nCntAttackModel].nLife = 0;                              //体力
		g_aAttackModel[nCntAttackModel].nMaxLife = 0;                           //最大体力
		g_aAttackModel[nCntAttackModel].SupportPos = NULL_VECTOR3;              //支点となる位置
		g_aAttackModel[nCntAttackModel].fSaveRot = 0.0f;                        //プレイヤーがブーメランを投げた向きを記録する
		g_aAttackModel[nCntAttackModel].nCntTime = 0;                           //攻撃が出現してからの時間をカウントする
		g_aAttackModel[nCntAttackModel].fTotalRotMove = 0.0f;                   //プレイヤーがブーメランを投げてから回転移動をした合計量
		g_aAttackModel[nCntAttackModel].SupportPosPlayer = NULL_VECTOR3;        //プレイヤーから見た支点となる位置
		g_aAttackModel[nCntAttackModel].nBehaviorType = 0;                      //攻撃の挙動の種類
		g_aAttackModel[nCntAttackModel].bTurnFlag = false;                      //ブーメランをターンさせるフラグ
		g_aAttackModel[nCntAttackModel].PosLength = NULL_VECTOR3;               //ある物体との距離を求める時に使う
		g_aAttackModel[nCntAttackModel].fVaim = 0.0f;                           //ベクトル
		g_aAttackModel[nCntAttackModel].fVXaim = 0.0f;                          //X方向のベクトル
		g_aAttackModel[nCntAttackModel].fVZaim = 0.0f;                          //Z方向のベクトル
		g_aAttackModel[nCntAttackModel].Size = NULL_VECTOR3;                    //大きさ
		g_aAttackModel[nCntAttackModel].bIsLanding = false;                     //地面にいるかどうか
		g_aAttackModel[nCntAttackModel].PosOld = NULL_VECTOR3;                  //1f前の位置
		g_aAttackModel[nCntAttackModel].fSpeed = 0.0f;                          //スピード
		g_aAttackModel[nCntAttackModel].nNumLandingModel = 0;                   //どのモデルの上に乗っているか
		g_aAttackModel[nCntAttackModel].nTurnCnt = 0;                           //ブーメランがターンを開始したら、カウントを開始
		g_aAttackModel[nCntAttackModel].nIdxShadow = -1;                        //影のインデックス
		g_aAttackModel[nCntAttackModel].bCurveLaser = false;                    //レーザーをカーブさせる向きを決める
		g_aAttackModel[nCntAttackModel].fBoomerangRange = 0.0f;                 //ブーメランが召喚された時点での射程を保存する
		for (int nCntAim = 0; nCntAim < 4; nCntAim++)
		{
			g_aAttackModel[nCntAttackModel].fSupportVaim[nCntAim] = 0.0f;       //支点となるベクトル
		}
		//=============================
		//スフィア変数
		//=============================
		g_aAttackModel[nCntAttackModel].bSphereShot = false;                    //ライフが半分になったら自機狙いで動かす

		//=============================
		//三次元自機狙い変数
		//=============================
		g_aAttackModel[nCntAttackModel].fVXaim3D = 0.0f;
		g_aAttackModel[nCntAttackModel].fVYaim3D = 0.0f;
		g_aAttackModel[nCntAttackModel].fVZaim3D = 0.0f;
		g_aAttackModel[nCntAttackModel].fVLaim3D = 0.0f;
	}

	//変数
	g_bIsLandingAttackModel = false;
	g_PlayerPosLength = NULL_VECTOR3;
	g_nDelayAttackModel = 0;//ディレイ

	for (int nCntType = 0; nCntType < NUM_ATTACKMODEL; nCntType++)
	{
		//Xファイルの読み込み
		D3DXLoadMeshFromX(ATTACKMODEL_FILENAME[nCntType], D3DXMESH_SYSTEMMEM, pDevice, NULL,
			&g_aAttackModelInfo[nCntType].pBuffMat, NULL, &g_aAttackModelInfo[nCntType].dwNumMat, &g_aAttackModelInfo[nCntType].pMesh);

		//マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_aAttackModelInfo[nCntType].pBuffMat->GetBufferPointer();

		//マテリアルデータを保存
		for (int nCntMat = 0; nCntMat < (int)g_aAttackModelInfo[nCntType].dwNumMat; nCntMat++)
		{
			for (int nCntAttackModel = 0; nCntAttackModel < MAX_ATTACKMODEL; nCntAttackModel++)
			{
				g_aAttackModel[nCntAttackModel].DiffUse[nCntType][nCntMat] = pMat[nCntMat].MatD3D.Diffuse;
			}
		}

		for (int nCntMat = 0; nCntMat < (int)g_aAttackModelInfo[nCntType].dwNumMat; nCntMat++)
		{
			//テクスチャファイルが存在する
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				//テクスチャの読み込み
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_aAttackModelInfo[nCntType].pTexture[nCntMat]);
			}
		}
	}
}

//================================================================================================================================================================================================================================
// 終了処理
//================================================================================================================================================================================================================================
void UninitAttackModel(void)
{
	for (int nCntAttackModel = 0; nCntAttackModel < NUM_ATTACKMODEL; nCntAttackModel++)
	{
		//メッシュの破棄
		if (g_aAttackModelInfo[nCntAttackModel].pMesh != NULL)
		{
			g_aAttackModelInfo[nCntAttackModel].pMesh->Release();
			g_aAttackModelInfo[nCntAttackModel].pMesh = NULL;
		}
		//マテリアルの破棄
		if (g_aAttackModelInfo[nCntAttackModel].pBuffMat != NULL)
		{
			g_aAttackModelInfo[nCntAttackModel].pBuffMat->Release();
			g_aAttackModelInfo[nCntAttackModel].pBuffMat = NULL;
		}

		//テクスチャの破棄
		for (int nCntMat = 0; nCntMat < ATTACKMODEL_MAT; nCntMat++)
		{
			if (g_aAttackModelInfo[nCntAttackModel].pTexture[nCntMat] != NULL)
			{
				g_aAttackModelInfo[nCntAttackModel].pTexture[nCntMat]->Release();
				g_aAttackModelInfo[nCntAttackModel].pTexture[nCntMat] = NULL;
			}
		}
	}
}

//================================================================================================================================================================================================================================
// 更新処理
//================================================================================================================================================================================================================================
void UpdateAttackModel(void)
{
	for (int nCntAttackModel = 0; nCntAttackModel < MAX_ATTACKMODEL; nCntAttackModel++)
	{
		if (g_aAttackModel[nCntAttackModel].bUse)
		{
			Player* pPlayer = GetPlayer();	//プレイヤーの情報を取得
			Enemy* pEnemy = GetEnemy();		//敵の情報を取得
			Boss* pBoss = GetBoss();		//ボスの情報を取得
			Dummy* pDummy = GetDummy();		//ダミーの情報を取得

			g_aAttackModel[nCntAttackModel].nCntTime++;//出現してからの時間を計測する
			g_nDelayAttackModel++;//ディレイ

			//===============================================
			//モデルの拡大率を参照したモデルの最大最小
			//===============================================
			g_aAttackModel[nCntAttackModel].vtxMin.x = g_aAttackModel[nCntAttackModel].Scale.y * g_aAttackModel[nCntAttackModel].OriginallyVtxMin.x;
			g_aAttackModel[nCntAttackModel].vtxMin.y = g_aAttackModel[nCntAttackModel].Scale.x * g_aAttackModel[nCntAttackModel].OriginallyVtxMin.y;
			g_aAttackModel[nCntAttackModel].vtxMin.z = g_aAttackModel[nCntAttackModel].Scale.z * g_aAttackModel[nCntAttackModel].OriginallyVtxMin.z;
			g_aAttackModel[nCntAttackModel].vtxMax.x = g_aAttackModel[nCntAttackModel].Scale.y * g_aAttackModel[nCntAttackModel].OriginallyVtxMax.x;
			g_aAttackModel[nCntAttackModel].vtxMax.y = g_aAttackModel[nCntAttackModel].Scale.x * g_aAttackModel[nCntAttackModel].OriginallyVtxMax.y;
			g_aAttackModel[nCntAttackModel].vtxMax.z = g_aAttackModel[nCntAttackModel].Scale.z * g_aAttackModel[nCntAttackModel].OriginallyVtxMax.z;
			g_aAttackModel[nCntAttackModel].Size = g_aAttackModel[nCntAttackModel].vtxMax - g_aAttackModel[nCntAttackModel].vtxMin;

			//===============================================
			//プレイヤーと攻撃モデルのベクトル
			//===============================================
			g_aAttackModel[nCntAttackModel].fVXaim = pPlayer->pos.x - g_aAttackModel[nCntAttackModel].pos.x;
			g_aAttackModel[nCntAttackModel].fVZaim = pPlayer->pos.z - g_aAttackModel[nCntAttackModel].pos.z;

			g_aAttackModel[nCntAttackModel].fVaim = atan2f(g_aAttackModel[nCntAttackModel].fVXaim, g_aAttackModel[nCntAttackModel].fVZaim);

			//===============================================
			//プレイヤーと攻撃モデルの距離
			//===============================================
			float fLineLength = fRotCollisionAModel(pPlayer->pos.x, g_aAttackModel[nCntAttackModel].pos.x, g_aAttackModel[nCntAttackModel].pos.x +
				sinf(g_aAttackModel[nCntAttackModel].rot.y) * g_aAttackModel[nCntAttackModel].vtxMax.z,
				pPlayer->pos.z, g_aAttackModel[nCntAttackModel].pos.z, g_aAttackModel[nCntAttackModel].pos.z +
				cosf(g_aAttackModel[nCntAttackModel].rot.y) * g_aAttackModel[nCntAttackModel].vtxMax.z);

			//===============================================
			//処理全体で使う可能性のある乱数値
			//===============================================
			float fPurposeRandRot = float(rand() % 200 + 1) / 100.0f;
			float fPurposeRandSpeed = float(rand() % 100 + 1) / 10.0f;
			float fPurposeRandSpeedY = float(rand() % 200 + 1) / 10.0f;

			float fPurposePosX = float(rand() % 400 - 200);
			float fPurposePosY = float(rand() % 400 - 200);
			float fPurposePosZ = float(rand() % 400 - 200);


			switch (g_aAttackModel[nCntAttackModel].nType)
			{
			case ATTACKMODEL00_BOOMERANG:

				if (g_aAttackModel[nCntAttackModel].nBehaviorType == ATTACKMODELBEHAVIOR00_NORMAL)
				{
					g_aAttackModel[nCntAttackModel].fSaveRot += 0.2f;//プレイヤーが召喚した時の向きに更に加算していく

					g_aAttackModel[nCntAttackModel].rot.y += 0.15f;//ブーメランを回転させる

					g_aAttackModel[nCntAttackModel].fTotalRotMove += 0.2f;//出現してから回転移動をした合計量

					//=========================
					//動きを決める	
					//=========================
					if (g_aAttackModel[nCntAttackModel].fTotalRotMove <= D3DX_PI * 5)
					{
						g_aAttackModel[nCntAttackModel].pos =
							D3DXVECTOR3(sinf(g_aAttackModel[nCntAttackModel].fSaveRot + D3DX_PI) * (pPlayer->aBoomerangInfo.fBoomerangRenge) + g_aAttackModel[nCntAttackModel].SupportPos.x + pPlayer->pos.x,
								0.0f,
								cosf(g_aAttackModel[nCntAttackModel].fSaveRot + D3DX_PI) * (pPlayer->aBoomerangInfo.fBoomerangRenge) + g_aAttackModel[nCntAttackModel].SupportPos.z + pPlayer->pos.z);
					}
					else
					{
						float fVXaim = pPlayer->pos.x - g_aAttackModel[nCntAttackModel].pos.x;
						float fVZaim = pPlayer->pos.z - g_aAttackModel[nCntAttackModel].pos.z;
						float fVaim = atan2f(fVXaim, fVZaim);

						g_aAttackModel[nCntAttackModel].move = D3DXVECTOR3(sinf(fVaim) * 10.0f, 0.0f, cosf(fVaim) * 10.0f);
					}

					SetEffect(EFFECTTYPE_NORMAL, 300, 90.0f, 15.0f, 1.0f, D3DXVECTOR3(g_aAttackModel[nCntAttackModel].pos.x, g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMax.y / 2,
						g_aAttackModel[nCntAttackModel].pos.z), NULL_VECTOR3, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 0);

					//プレイヤーに当たったら
					if (g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMax.x >= pPlayer->pos.x - pPlayer->Size.x / 2 &&
						g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMin.x <= pPlayer->pos.x + pPlayer->Size.x / 2 &&
						g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMax.y >= pPlayer->pos.y &&
						g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMin.y <= pPlayer->pos.y + pPlayer->Size.y &&
						g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMax.z >= pPlayer->pos.z - pPlayer->Size.z / 2 &&
						g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMin.z <= pPlayer->pos.z + pPlayer->Size.z / 2 &&
						g_aAttackModel[nCntAttackModel].fTotalRotMove >= (D3DX_PI * 0.8f))
					{
						g_aAttackModel[nCntAttackModel].bUse = false;

						if (pPlayer->aBoomerangInfo.nBoomerangPiece < pPlayer->aBoomerangInfo.nMaxBoomerangPiece)
						{//プレイヤーの持っているブーメランの個数が最大値よりも少なかったら
							pPlayer->aBoomerangInfo.nBoomerangPiece++;
						}
					}

					//敵との当たり判定
					for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
					{
						if (pEnemy->bUse)
						{
							if (g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMax.x >= pEnemy->pos.x - pEnemy->Size.x / 2 &&
								g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMin.x <= pEnemy->pos.x + pEnemy->Size.x / 2 &&
								g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMax.y >= pEnemy->pos.y &&
								g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMin.y <= pEnemy->pos.y + pEnemy->Size.y &&
								g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMax.z >= pEnemy->pos.z - pEnemy->Size.z / 2 &&
								g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMin.z <= pEnemy->pos.z + pEnemy->Size.z / 2)
							{
								SetDamageEnemy(pPlayer->aBoomerangInfo.nBoomerangPower, nCntEnemy, PLAYERATTACK01_BOOMERANG, nCntAttackModel);
							}
						}
					}

					//ボスとの当たり判定
					if (pBoss->bUse)
					{
						if (g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMax.x >= pBoss->pos.x - pBoss->Size.x / 2 &&
							g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMin.x <= pBoss->pos.x + pBoss->Size.x / 2 &&
							g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMax.y >= pBoss->pos.y &&
							g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMin.y <= pBoss->pos.y + pBoss->Size.y &&
							g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMax.z >= pBoss->pos.z - pBoss->Size.z / 2 &&
							g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMin.z <= pBoss->pos.z + pBoss->Size.z / 2)
						{
							SetDamageBoss(pPlayer->aBoomerangInfo.nBoomerangPower, PLAYERATTACK01_BOOMERANG, nCntAttackModel);
						}
					}
				}
				else
				{
					//===============================================
					//攻撃開始位置と攻撃モデルの距離
					//===============================================
					float fLength = sqrtf((g_aAttackModel[nCntAttackModel].pos.x - g_aAttackModel[nCntAttackModel].SupportPos.x) * (g_aAttackModel[nCntAttackModel].pos.x - g_aAttackModel[nCntAttackModel].SupportPos.x) +
						(g_aAttackModel[nCntAttackModel].pos.z - g_aAttackModel[nCntAttackModel].SupportPos.z) * (g_aAttackModel[nCntAttackModel].pos.z - g_aAttackModel[nCntAttackModel].SupportPos.z)) / 2;

					g_aAttackModel[nCntAttackModel].rot.y += 0.2f;

					if (g_aAttackModel[nCntAttackModel].bTurnFlag == false)
					{
						if (fLength > g_aAttackModel[nCntAttackModel].fBoomerangRange)
						{
							g_aAttackModel[nCntAttackModel].move.x += (0.0f - g_aAttackModel[nCntAttackModel].move.x) * 0.3f;
							g_aAttackModel[nCntAttackModel].move.z += (0.0f - g_aAttackModel[nCntAttackModel].move.z) * 0.3f;
						}


						if ((g_aAttackModel[nCntAttackModel].move.x >= -0.1f && g_aAttackModel[nCntAttackModel].move.x <= 0.1f) &&
							(g_aAttackModel[nCntAttackModel].move.z >= -0.1f && g_aAttackModel[nCntAttackModel].move.z <= 0.1f))
						{
							g_aAttackModel[nCntAttackModel].bTurnFlag = true;
						}
					}

					Field* pField = GetField();	//フィールドの情報を取得

					if (g_aAttackModel[nCntAttackModel].pos.x > pField->fWidth || g_aAttackModel[nCntAttackModel].pos.x < -pField->fWidth
						|| g_aAttackModel[nCntAttackModel].pos.z > pField->fHeight || g_aAttackModel[nCntAttackModel].pos.z < -pField->fHeight)
					{
						g_aAttackModel[nCntAttackModel].bTurnFlag = true;
					}

					if (g_aAttackModel[nCntAttackModel].bTurnFlag == true)
					{
						g_aAttackModel[nCntAttackModel].nTurnCnt++;

						if (g_nDelayAttackModel % 3 == 0 || g_aAttackModel[nCntAttackModel].nTurnCnt == 1)
						{
							//==========================
							//３次元自機狙い処理
							//==========================
							g_aAttackModel[nCntAttackModel].fVXaim3D = pPlayer->pos.x - g_aAttackModel[nCntAttackModel].pos.x;
							g_aAttackModel[nCntAttackModel].fVYaim3D = pPlayer->pos.y - g_aAttackModel[nCntAttackModel].pos.y;
							g_aAttackModel[nCntAttackModel].fVZaim3D = pPlayer->pos.z - g_aAttackModel[nCntAttackModel].pos.z;

							D3DXVECTOR3 BulletMove = NULL_VECTOR3;

							g_aAttackModel[nCntAttackModel].fVLaim3D = sqrtf((g_aAttackModel[nCntAttackModel].fVXaim3D * g_aAttackModel[nCntAttackModel].fVXaim3D) +
								(g_aAttackModel[nCntAttackModel].fVYaim3D * g_aAttackModel[nCntAttackModel].fVYaim3D)
								+ (g_aAttackModel[nCntAttackModel].fVZaim3D * g_aAttackModel[nCntAttackModel].fVZaim3D));

							BulletMove.x = g_aAttackModel[nCntAttackModel].fVXaim3D / g_aAttackModel[nCntAttackModel].fVLaim3D * 30.0f;
							BulletMove.y = g_aAttackModel[nCntAttackModel].fVYaim3D / g_aAttackModel[nCntAttackModel].fVLaim3D * 30.0f;
							BulletMove.z = g_aAttackModel[nCntAttackModel].fVZaim3D / g_aAttackModel[nCntAttackModel].fVLaim3D * 30.0f;

							g_aAttackModel[nCntAttackModel].move = BulletMove;
						}
					}

					//=========================
					//動きを決める	
					//=========================
					SetEffect(EFFECTTYPE_NORMAL, 300, g_aAttackModel[nCntAttackModel].Size.x * 1.5f, g_aAttackModel[nCntAttackModel].Size.y * 1.5f,
						1.0f, D3DXVECTOR3(g_aAttackModel[nCntAttackModel].pos.x, g_aAttackModel[nCntAttackModel].pos.y,
							g_aAttackModel[nCntAttackModel].pos.z), NULL_VECTOR3, D3DXCOLOR(1.0f, 0.3f, 0.0f, 1.0f), 0.0f, 0);

					//プレイヤーに当たったら
					if (g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMax.x >= pPlayer->pos.x - pPlayer->Size.x / 2 &&
						g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMin.x <= pPlayer->pos.x + pPlayer->Size.x / 2 &&
						g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMax.y >= pPlayer->pos.y &&
						g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMin.y <= pPlayer->pos.y + pPlayer->Size.y &&
						g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMax.z >= pPlayer->pos.z - pPlayer->Size.z / 2 &&
						g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMin.z <= pPlayer->pos.z + pPlayer->Size.z / 2 &&
						g_aAttackModel[nCntAttackModel].nCntTime >= 30)
					{
						g_aAttackModel[nCntAttackModel].bUse = false;

						if (pPlayer->aBoomerangInfo.nBoomerangPiece < pPlayer->aBoomerangInfo.nMaxBoomerangPiece)
						{//プレイヤーの持っているブーメランの個数が最大値よりも少なかったら
							pPlayer->aBoomerangInfo.nBoomerangPiece++;
						}
					}

					//敵との当たり判定
					for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
					{
						if (pEnemy->bUse == true)
						{//敵が使用されている場合
							if (g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMax.x >= pEnemy->pos.x - pEnemy->Size.x / 2 &&
								g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMin.x <= pEnemy->pos.x + pEnemy->Size.x / 2 &&
								g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMax.y >= pEnemy->pos.y &&
								g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMin.y <= pEnemy->pos.y + pEnemy->Size.y &&
								g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMax.z >= pEnemy->pos.z - pEnemy->Size.z / 2 &&
								g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMin.z <= pEnemy->pos.z + pEnemy->Size.z / 2)
							{
								SetDamageEnemy(pPlayer->aBoomerangInfo.nBoomerangPower, nCntEnemy, PLAYERATTACK01_BOOMERANG, nCntAttackModel);
							}
						}
					}

					//ボスとの当たり判定
					if (pBoss->bUse)
					{//ボスが使用されている場合
						if (g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMax.x >= pBoss->pos.x - pBoss->Size.x / 2 &&
							g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMin.x <= pBoss->pos.x + pBoss->Size.x / 2 &&
							g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMax.y >= pBoss->pos.y &&
							g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMin.y <= pBoss->pos.y + pBoss->Size.y &&
							g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMax.z >= pBoss->pos.z - pBoss->Size.z / 2 &&
							g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMin.z <= pBoss->pos.z + pBoss->Size.z / 2)
						{
							SetDamageBoss(pPlayer->aBoomerangInfo.nBoomerangPower, PLAYERATTACK01_BOOMERANG, nCntAttackModel);
						}
					}

					if (pDummy->bUse)
					{//ダミーが使用されている場合
						if (g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMax.x >= pDummy->pos.x - pDummy->Size.x / 2 &&
							g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMin.x <= pDummy->pos.x + pDummy->Size.x / 2 &&
							g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMax.y >= pDummy->pos.y &&
							g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMin.y <= pDummy->pos.y + pDummy->Size.y &&
							g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMax.z >= pDummy->pos.z - pDummy->Size.z / 2 &&
							g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMin.z <= pDummy->pos.z + pDummy->Size.z / 2)
						{
							DamageDummy(pPlayer->aBoomerangInfo.nBoomerangPower, PLAYERATTACK01_BOOMERANG);
						}
					}
				}

				break;

			case ATTACKMODEL01_WAVE:
				if (g_aAttackModel[nCntAttackModel].nBehaviorType == ATTACKMODELBEHAVIOR00_NORMAL)
				{
					g_aAttackModel[nCntAttackModel].Scale += D3DXVECTOR3(g_aAttackModel[nCntAttackModel].fExpansion,
						g_aAttackModel[nCntAttackModel].fExpansion, g_aAttackModel[nCntAttackModel].fExpansion);

					g_aAttackModel[nCntAttackModel].DiffUse[g_aAttackModel[nCntAttackModel].nType]->a = (float)(g_aAttackModel[nCntAttackModel].nLife) / (float)(g_aAttackModel[nCntAttackModel].nMaxLife);

					//====================
					//当たり判定
					//====================
					for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
					{
						if (pEnemy->bUse)
						{//敵が使用されている場合
							if (g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMax.x >= pEnemy->pos.x - pEnemy->Size.x / 2 &&
								g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMin.x <= pEnemy->pos.x + pEnemy->Size.x / 2 &&
								g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMax.y >= pEnemy->pos.y &&
								g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMin.y <= pEnemy->pos.y + pEnemy->Size.y &&
								g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMax.z >= pEnemy->pos.z - pEnemy->Size.z / 2 &&
								g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMin.z <= pEnemy->pos.z + pEnemy->Size.z / 2)
							{
								SetDamageEnemy((int)(pPlayer->aSpeakerInfo.fSpeakerPower), nCntEnemy, PLAYERATTACK02_SPEAKER, nCntAttackModel);
							}
						}
					}

					//ボスの当たり判定
					if (pBoss->bUse)
					{//ボスが使用されている場合
						if (g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMax.x >= pBoss->pos.x - pBoss->Size.x / 2 &&
							g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMin.x <= pBoss->pos.x + pBoss->Size.x / 2 &&
							g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMax.y >= pBoss->pos.y &&
							g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMin.y <= pBoss->pos.y + pBoss->Size.y &&
							g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMax.z >= pBoss->pos.z - pBoss->Size.z / 2 &&
							g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMin.z <= pBoss->pos.z + pBoss->Size.z / 2)
						{
							SetDamageBoss(pPlayer->aBoomerangInfo.nBoomerangPower, PLAYERATTACK02_SPEAKER, nCntAttackModel);
						}
					}

					//ダミーの当たり判定
					if (pDummy->bUse)
					{//ダミーが使用されている場合
						if (g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMax.x >= pDummy->pos.x - pDummy->Size.x / 2 &&
							g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMin.x <= pDummy->pos.x + pDummy->Size.x / 2 &&
							g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMax.y >= pDummy->pos.y &&
							g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMin.y <= pDummy->pos.y + pDummy->Size.y &&
							g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMax.z >= pDummy->pos.z - pDummy->Size.z / 2 &&
							g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMin.z <= pDummy->pos.z + pDummy->Size.z / 2)
						{
							DamageDummy(pPlayer->aBoomerangInfo.nBoomerangPower, PLAYERATTACK02_SPEAKER);
						}
					}
				}
				else
				{
					g_aAttackModel[nCntAttackModel].Scale += D3DXVECTOR3(g_aAttackModel[nCntAttackModel].fExpansion,
						g_aAttackModel[nCntAttackModel].fExpansion, g_aAttackModel[nCntAttackModel].fExpansion);

					//====================
					//当たり判定
					//====================
					for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
					{
						if (pEnemy->bUse)
						{//敵が使用されている場合
							if (g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMax.x >= pEnemy->pos.x - pEnemy->Size.x / 2 &&
								g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMin.x <= pEnemy->pos.x + pEnemy->Size.x / 2 &&
								g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMax.y >= pEnemy->pos.y &&
								g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMin.y <= pEnemy->pos.y + pEnemy->Size.y &&
								g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMax.z >= pEnemy->pos.z - pEnemy->Size.z / 2 &&
								g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMin.z <= pEnemy->pos.z + pEnemy->Size.z / 2)
							{
								SetDamageEnemy((int)(pPlayer->aSpeakerInfo.fSpeakerPower), nCntEnemy, PLAYERATTACK02_SPEAKER, nCntAttackModel);
							}
						}
					}

					//ボスの当たり判定
					if (pBoss->bUse)
					{//ボスが使用されている場合
						if (g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMax.x >= pBoss->pos.x - pBoss->Size.x / 2 &&
							g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMin.x <= pBoss->pos.x + pBoss->Size.x / 2 &&
							g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMax.y >= pBoss->pos.y &&
							g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMin.y <= pBoss->pos.y + pBoss->Size.y &&
							g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMax.z >= pBoss->pos.z - pBoss->Size.z / 2 &&
							g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMin.z <= pBoss->pos.z + pBoss->Size.z / 2)
						{
							SetDamageBoss(pPlayer->aBoomerangInfo.nBoomerangPower, PLAYERATTACK02_SPEAKER, nCntAttackModel);
						}
					}

					//ダミーの当たり判定
					if (pDummy->bUse)
					{//ダミーが使用されている場合
						if (g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMax.x >= pDummy->pos.x - pDummy->Size.x / 2 &&
							g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMin.x <= pDummy->pos.x + pDummy->Size.x / 2 &&
							g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMax.y >= pDummy->pos.y &&
							g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMin.y <= pDummy->pos.y + pDummy->Size.y &&
							g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMax.z >= pDummy->pos.z - pDummy->Size.z / 2 &&
							g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMin.z <= pDummy->pos.z + pDummy->Size.z / 2)
						{
							DamageDummy(pPlayer->aBoomerangInfo.nBoomerangPower, PLAYERATTACK02_SPEAKER);
						}
					}
				}

				break;

			case ATTACKMODEL02_SPHERE:
				if (g_aAttackModel[nCntAttackModel].nBehaviorType == ATTACKMODELBEHAVIOR00_NORMAL)
				{
					g_aAttackModel[nCntAttackModel].Scale += D3DXVECTOR3(g_aAttackModel[nCntAttackModel].fExpansion,
						g_aAttackModel[nCntAttackModel].fExpansion, g_aAttackModel[nCntAttackModel].fExpansion);

					g_aAttackModel[nCntAttackModel].DiffUse[g_aAttackModel[nCntAttackModel].nType]->a = (float)(g_aAttackModel[nCntAttackModel].nLife) / (float)(g_aAttackModel[nCntAttackModel].nMaxLife);

					//====================
					//当たり判定
					//====================
					if (g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMax.x >= pPlayer->pos.x - pPlayer->Size.x / 2 &&
						g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMin.x <= pPlayer->pos.x + pPlayer->Size.x / 2 &&
						g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMax.y >= pPlayer->pos.y &&
						g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMin.y <= pPlayer->pos.y + pPlayer->Size.y &&
						g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMax.z >= pPlayer->pos.z - pPlayer->Size.z / 2 &&
						g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMin.z <= pPlayer->pos.z + pPlayer->Size.z / 2)
					{
						AttackHitPlayer(20);
					}

				}
				else
				{
					g_aAttackModel[nCntAttackModel].Scale += D3DXVECTOR3(g_aAttackModel[nCntAttackModel].fExpansion,
						g_aAttackModel[nCntAttackModel].fExpansion, g_aAttackModel[nCntAttackModel].fExpansion);


					g_aAttackModel[nCntAttackModel].DiffUse[g_aAttackModel[nCntAttackModel].nType]->a = (float)(g_aAttackModel[nCntAttackModel].nLife) / (float)(g_aAttackModel[nCntAttackModel].nMaxLife);

					if (g_aAttackModel[nCntAttackModel].nLife < (g_aAttackModel[nCntAttackModel].nMaxLife / 2.0f) && g_aAttackModel[nCntAttackModel].bSphereShot == false)
					{

						g_aAttackModel[nCntAttackModel].move.x = sinf(g_aAttackModel[nCntAttackModel].fVaim) * 10.0f;
						g_aAttackModel[nCntAttackModel].move.z = cosf(g_aAttackModel[nCntAttackModel].fVaim) * 10.0f;

						g_aAttackModel[nCntAttackModel].bSphereShot = true;//一度方向決めたのでOK
					}
					if (g_aAttackModel[nCntAttackModel].nLife > (g_aAttackModel[nCntAttackModel].nMaxLife / 2.0f) && g_aAttackModel[nCntAttackModel].bSphereShot == false)
					{
						SetParticle(PARTICLETYPE_CHARGE, 200, 20.0f, 0.7f, D3DXVECTOR3(g_aAttackModel[nCntAttackModel].pos.x + fPurposePosX,
							g_aAttackModel[nCntAttackModel].pos.y + fPurposePosY,
							g_aAttackModel[nCntAttackModel].pos.z + fPurposePosZ),
							NULL_VECTOR3, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), 0.0f, PARTICLETEX_NORMAL, nCntAttackModel);
					}

					if (g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMin.y < 0.0f)
					{
						g_aAttackModel[nCntAttackModel].pos.y = g_aAttackModel[nCntAttackModel].Size.y / 2.0f;
					}

					//====================
					//当たり判定
					//====================
					if (g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMax.x >= pPlayer->pos.x - pPlayer->Size.x / 2 &&
						g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMin.x <= pPlayer->pos.x + pPlayer->Size.x / 2 &&
						g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMax.y >= pPlayer->pos.y &&
						g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMin.y <= pPlayer->pos.y + pPlayer->Size.y &&
						g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMax.z >= pPlayer->pos.z - pPlayer->Size.z / 2 &&
						g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMin.z <= pPlayer->pos.z + pPlayer->Size.z / 2)
					{
						AttackHitPlayer(20);
					}
				}

				break;

			case ATTACKMODEL03_BOMB:
				if (g_aAttackModel[nCntAttackModel].nBehaviorType == ATTACKMODELBEHAVIOR00_NORMAL)
				{
					g_aAttackModel[nCntAttackModel].bIsLanding = bCollisionModel(&g_aAttackModel[nCntAttackModel].pos,
						&g_aAttackModel[nCntAttackModel].PosOld,
						g_aAttackModel[nCntAttackModel].Size, g_aAttackModel[nCntAttackModel].nNumLandingModel);

					if (!g_aAttackModel[nCntAttackModel].bIsLanding)
					{
						g_aAttackModel[nCntAttackModel].move.y -= 0.2f;
					}
					else
					{
						g_aAttackModel[nCntAttackModel].move.y = 0.0f;
					}

					if (g_aAttackModel[nCntAttackModel].pos.y <= 0.0f)
					{
						g_aAttackModel[nCntAttackModel].pos.y = 0.0f;
						g_aAttackModel[nCntAttackModel].move.y = 0.0f;
					}
				}
				else
				{
					g_aAttackModel[nCntAttackModel].bIsLanding = bCollisionModel(&g_aAttackModel[nCntAttackModel].pos,
						&g_aAttackModel[nCntAttackModel].PosOld,
						g_aAttackModel[nCntAttackModel].Size, g_aAttackModel[nCntAttackModel].nNumLandingModel);

					if (!g_aAttackModel[nCntAttackModel].bIsLanding)
					{
						g_aAttackModel[nCntAttackModel].move.y -= 0.2f;
					}
					else
					{
						g_aAttackModel[nCntAttackModel].move.y = 0.0f;
					}

					if (g_aAttackModel[nCntAttackModel].pos.y <= 0.0f)
					{
						g_aAttackModel[nCntAttackModel].pos.y = 0.0f;
						g_aAttackModel[nCntAttackModel].move = NULL_VECTOR3;
					}

					if (g_aAttackModel[nCntAttackModel].bIsLanding)
					{
						g_aAttackModel[nCntAttackModel].move = NULL_VECTOR3;
					}
				}

				break;

			case ATTACKMODEL04_SHOCKWAVE:

				if (g_aAttackModel[nCntAttackModel].nBehaviorType == ATTACKMODELBEHAVIOR00_NORMAL)
				{
					g_aAttackModel[nCntAttackModel].rot.y += 0.25f;


					SetParticle(PARTICLETYPE_GRAVITY, 80, 5.0f, 0.05f, g_aAttackModel[nCntAttackModel].pos,
						D3DXVECTOR3(sinf(D3DX_PI * fPurposeRandRot) * fPurposeRandSpeed, fPurposeRandSpeedY, cosf(D3DX_PI * fPurposeRandRot) * fPurposeRandSpeed),
						D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 0.0f, PARTICLETEX_NORMAL, 0);

					//====================
					//当たり判定
					//====================
					if (g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMax.x >= pPlayer->pos.x - pPlayer->Size.x / 2 &&
						g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMin.x <= pPlayer->pos.x + pPlayer->Size.x / 2 &&
						g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMax.y >= pPlayer->pos.y &&
						g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMin.y <= pPlayer->pos.y + pPlayer->Size.y &&
						g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMax.z >= pPlayer->pos.z - pPlayer->Size.z / 2 &&
						g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMin.z <= pPlayer->pos.z + pPlayer->Size.z / 2)
					{
						AttackHitPlayer(30);

						for (int nCntParticle = 0; nCntParticle < 10; nCntParticle++)
						{
							float fRandRot = float(rand() % 200 + 1) / 10;
							float fRandSpeed = float(rand() % 20 + 1);
							float fRandMove = float(rand() % 40 - 20) / 2;

							SetParticle(PARTICLETYPE_NORMAL, 80, 5.0f, 0.4f, pPlayer->pos,
								D3DXVECTOR3(sinf(-D3DX_PI * fRandRot) * fRandSpeed, fRandMove, cosf(-D3DX_PI * fRandRot) * fRandSpeed),
								D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 0.0f, PARTICLETEX_NORMAL, 0);
						}
					}
				}
				else
				{
					g_aAttackModel[nCntAttackModel].rot.y -= 0.25f;


					g_aAttackModel[nCntAttackModel].nCntTime++;//出現してからの時間を測る	

					//========================================================
					//出現してからの時間ごとの処理
					//========================================================
					if (g_aAttackModel[nCntAttackModel].nCntTime == 2)
					{
						g_aAttackModel[nCntAttackModel].fSpeed = g_aAttackModel[nCntAttackModel].fExpansion * 1.1f;
					}

					if (g_aAttackModel[nCntAttackModel].nCntTime == 70)
					{
						g_aAttackModel[nCntAttackModel].rot.y = g_aAttackModel[nCntAttackModel].fVaim + D3DX_PI;
						g_aAttackModel[nCntAttackModel].move.x = sinf(g_aAttackModel[nCntAttackModel].fVaim) * g_aAttackModel[nCntAttackModel].fSpeed;
						g_aAttackModel[nCntAttackModel].move.z = cosf(g_aAttackModel[nCntAttackModel].fVaim) * g_aAttackModel[nCntAttackModel].fSpeed;

					}

					SetParticle(PARTICLETYPE_GRAVITY, 120, 5.0f, 0.05f, g_aAttackModel[nCntAttackModel].pos,
						D3DXVECTOR3(sinf(D3DX_PI * fPurposeRandRot) * fPurposeRandSpeed, fPurposeRandSpeedY, cosf(D3DX_PI * fPurposeRandRot) * fPurposeRandSpeed),
						D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 0.0f, PARTICLETEX_NORMAL, 0);

					//====================
					//当たり判定
					//====================
					if (g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMax.x >= pPlayer->pos.x - pPlayer->Size.x / 2 &&
						g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMin.x <= pPlayer->pos.x + pPlayer->Size.x / 2 &&
						g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMax.y >= pPlayer->pos.y &&
						g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMin.y <= pPlayer->pos.y + pPlayer->Size.y &&
						g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMax.z >= pPlayer->pos.z - pPlayer->Size.z / 2 &&
						g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMin.z <= pPlayer->pos.z + pPlayer->Size.z / 2)
					{
						AttackHitPlayer(30);
					}
				}

				break;

			case ATTACKMODEL05_TORNADO:

				SetEffect(EFFECTTYPE_NORMAL, 120, 10.0f, 10.0f, 0.1f, g_aAttackModel[nCntAttackModel].pos, NULL_VECTOR3, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 0.0f, 0);

				if (g_nDelayAttackModel % 10 == 0)
				{
					SetEmitterEffect(EMITTERTYPE_TORNADO, EMITTERTARGET_ATTACKMODEL, 180, 3, 10.0f, 0.02f, g_aAttackModel[nCntAttackModel].pos, D3DXVECTOR3(0.0f, 2.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 0.0f, nCntAttackModel, 0.0f, NULL_VECTOR3);
				}

				if (g_aAttackModel[nCntAttackModel].nBehaviorType == ATTACKMODELBEHAVIOR00_NORMAL)
				{
					g_aAttackModel[nCntAttackModel].rot.y += 0.17f;

					g_aAttackModel[nCntAttackModel].fExpansion *= 1.05f;
					g_aAttackModel[nCntAttackModel].fRot += 0.005f + g_aAttackModel[nCntAttackModel].fSaveRot;

					g_aAttackModel[nCntAttackModel].pos = D3DXVECTOR3(sinf(D3DX_PI * g_aAttackModel[nCntAttackModel].fRot) * g_aAttackModel[nCntAttackModel].fExpansion + g_aAttackModel[nCntAttackModel].SupportPos.x,
						0.0f, cosf(D3DX_PI * g_aAttackModel[nCntAttackModel].fRot) * g_aAttackModel[nCntAttackModel].fExpansion + g_aAttackModel[nCntAttackModel].SupportPos.z);

					//====================
					//当たり判定
					//====================
					if (g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMax.x >= pPlayer->pos.x - pPlayer->Size.x / 2 &&
						g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMin.x <= pPlayer->pos.x + pPlayer->Size.x / 2 &&
						g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMax.y >= pPlayer->pos.y &&
						g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMin.y <= pPlayer->pos.y + pPlayer->Size.y &&
						g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMax.z >= pPlayer->pos.z - pPlayer->Size.z / 2 &&
						g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMin.z <= pPlayer->pos.z + pPlayer->Size.z / 2)
					{
						AttackHitPlayer(30);
					}
				}
				else
				{
					g_aAttackModel[nCntAttackModel].move.x *= 1.025f;
					g_aAttackModel[nCntAttackModel].move.z *= 1.025f;

					g_aAttackModel[nCntAttackModel].rot.y += 0.1f;

					//====================
					//当たり判定
					//====================
					if (g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMax.x >= pPlayer->pos.x - pPlayer->Size.x / 2 &&
						g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMin.x <= pPlayer->pos.x + pPlayer->Size.x / 2 &&
						g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMax.y >= pPlayer->pos.y &&
						g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMin.y <= pPlayer->pos.y + pPlayer->Size.y &&
						g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMax.z >= pPlayer->pos.z - pPlayer->Size.z / 2 &&
						g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMin.z <= pPlayer->pos.z + pPlayer->Size.z / 2)
					{
						AttackHitPlayer(30);
					}
				}

				break;

			case ATTACKMODEL06_THUNDER:

				g_aAttackModel[nCntAttackModel].rot.y += 0.22f;

				//====================
				//当たり判定
				//====================
				if (g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMax.x >= pPlayer->pos.x - pPlayer->Size.x / 2 &&
					g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMin.x <= pPlayer->pos.x + pPlayer->Size.x / 2 &&
					g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMax.y >= pPlayer->pos.y &&
					g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMin.y <= pPlayer->pos.y + pPlayer->Size.y &&
					g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMax.z >= pPlayer->pos.z - pPlayer->Size.z / 2 &&
					g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMin.z <= pPlayer->pos.z + pPlayer->Size.z / 2)
				{
					AttackHitPlayer(40);
				}

				break;

			case ATTACKMODEL07_METEO:

				if (g_aAttackModel[nCntAttackModel].nBehaviorType == ATTACKMODELBEHAVIOR00_NORMAL)
				{
					if (g_aAttackModel[nCntAttackModel].nCntTime == 1)
					{
						SetEmitterEffect(EMITTERTYPE_ROTATION, EMITTERTARGET_ATTACKMODEL, 1000, 50, 15, 0.1f, NULL_VECTOR3, NULL_VECTOR3, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 0.0f, nCntAttackModel, g_aAttackModel[nCntAttackModel].Size.x / 2.0f, g_aAttackModel[nCntAttackModel].SupportPos);
						SetEmitterEffect(EMITTERTYPE_ROTATION, EMITTERTARGET_ATTACKMODEL, 1000, 50, 15, 0.1f, NULL_VECTOR3, NULL_VECTOR3, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), D3DX_PI, nCntAttackModel, g_aAttackModel[nCntAttackModel].Size.x / 2.0f, g_aAttackModel[nCntAttackModel].SupportPos);
					}

					//====================
					//当たり判定
					//====================

					if (g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMax.x >= pPlayer->pos.x - pPlayer->Size.x / 2 &&
						g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMin.x <= pPlayer->pos.x + pPlayer->Size.x / 2 &&
						g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMax.y >= pPlayer->pos.y &&
						g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMin.y <= pPlayer->pos.y + pPlayer->Size.y &&
						g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMax.z >= pPlayer->pos.z - pPlayer->Size.z / 2 &&
						g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMin.z <= pPlayer->pos.z + pPlayer->Size.z / 2)
					{
						AttackHitPlayer(100);
					}

					if (g_aAttackModel[nCntAttackModel].pos.y <= 0.0f)
					{
						g_aAttackModel[nCntAttackModel].bUse = false;
						PlaySound(SOUND_LABEL_SE_BURSTMETEO_000);
					}
				}

				break;

			case ATTACKMODEL08_SOUP:

				//====================
				//当たり判定
				//====================
				g_aAttackModel[nCntAttackModel].move.y += -0.3f;

				for (int nCntParticle = 0; nCntParticle < 3; nCntParticle++)
				{
					float fRandRot = float(rand() % 200 + 1) / 10;
					float fRandSpeed = float(rand() % 40 + 1);
					float fRandMove = float(rand() % 400) / 10;

					SetParticle(PARTICLETYPE_NORMAL, 10, 10.0f, 1.0f, g_aAttackModel[nCntAttackModel].pos,
						D3DXVECTOR3(sinf(D3DX_PI * fRandRot) * fRandSpeed, fRandMove, cosf(D3DX_PI * fRandRot) * fRandSpeed),
						D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f), 0.0f, PARTICLETEX_NORMAL, 0);
				}

				if (g_aAttackModel[nCntAttackModel].pos.y <= 0.0f)
				{
					g_aAttackModel[nCntAttackModel].pos.y = 0.0f;
					g_aAttackModel[nCntAttackModel].move.y = 0.0f;
				}

				if (g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMax.x >= pPlayer->pos.x - pPlayer->Size.x / 2 &&
					g_aAttackModel[nCntAttackModel].pos.x + g_aAttackModel[nCntAttackModel].vtxMin.x <= pPlayer->pos.x + pPlayer->Size.x / 2 &&
					g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMax.y >= pPlayer->pos.y &&
					g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMin.y <= pPlayer->pos.y + pPlayer->Size.y &&
					g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMax.z >= pPlayer->pos.z - pPlayer->Size.z / 2 &&
					g_aAttackModel[nCntAttackModel].pos.z + g_aAttackModel[nCntAttackModel].vtxMin.z <= pPlayer->pos.z + pPlayer->Size.z / 2)
				{
					AttackHitPlayer(10);
					g_aAttackModel[nCntAttackModel].bUse = false;
				}

				break;

			case ATTACKMODEL09_LASER:
				g_aAttackModel[nCntAttackModel].rot.z += 0.17f;


				if (g_aAttackModel[nCntAttackModel].nCntTime == 1)
				{
					g_aAttackModel[nCntAttackModel].fRot = 0.01f;

					if (g_aAttackModel[nCntAttackModel].rot.y >= g_aAttackModel[nCntAttackModel].fVaim)
					{
						g_aAttackModel[nCntAttackModel].bCurveLaser = false;
					}
					else
					{
						g_aAttackModel[nCntAttackModel].bCurveLaser = true;
					}
				}

				if (g_aAttackModel[nCntAttackModel].fRot <= 0.061f)
				{
					g_aAttackModel[nCntAttackModel].fRot *= 1.003f;
				}

				if (g_aAttackModel[nCntAttackModel].bCurveLaser == true)
				{
					g_aAttackModel[nCntAttackModel].rot.y += g_aAttackModel[nCntAttackModel].fRot;
				}
				else
				{
					g_aAttackModel[nCntAttackModel].rot.y -= g_aAttackModel[nCntAttackModel].fRot;
				}

				g_aAttackModel[nCntAttackModel].SupportPos = D3DXVECTOR3(sinf(g_aAttackModel[nCntAttackModel].rot.y) * (g_aAttackModel[nCntAttackModel].vtxMax.z * 2.0f), g_aAttackModel[nCntAttackModel].pos.y,
					cosf(g_aAttackModel[nCntAttackModel].rot.y) * (g_aAttackModel[nCntAttackModel].vtxMax.z * 2.0f));

				//==========================
			    //ランダムな方向にエミッターを飛ばす
			    //==========================
					float fRandPosX = float(rand() % 500 - 250);
					float fRandPosY = float(rand() % 500 - 250);
					float fRandPosZ = float(rand() % 500 - 250);
					float fColR = float(rand() % 100 + 1) / 100.0f;
					float fColG = float(rand() % 100 + 1) / 100.0f;
					float fColB = float(rand() % 100 + 1) / 100.0f;
					g_aAttackModel[nCntAttackModel].fVXaim3D = (g_aAttackModel[nCntAttackModel].pos.x + fRandPosX) - g_aAttackModel[nCntAttackModel].pos.x;
					g_aAttackModel[nCntAttackModel].fVYaim3D = (g_aAttackModel[nCntAttackModel].pos.y + fRandPosY) - g_aAttackModel[nCntAttackModel].pos.y;
					g_aAttackModel[nCntAttackModel].fVZaim3D = (g_aAttackModel[nCntAttackModel].pos.z + fRandPosZ) - g_aAttackModel[nCntAttackModel].pos.z;

					D3DXVECTOR3 BulletMove = NULL_VECTOR3;

					g_aAttackModel[nCntAttackModel].fVLaim3D = sqrtf((g_aAttackModel[nCntAttackModel].fVXaim3D * g_aAttackModel[nCntAttackModel].fVXaim3D) +
						(g_aAttackModel[nCntAttackModel].fVYaim3D * g_aAttackModel[nCntAttackModel].fVYaim3D)
						+ (g_aAttackModel[nCntAttackModel].fVZaim3D * g_aAttackModel[nCntAttackModel].fVZaim3D));

					BulletMove.x = g_aAttackModel[nCntAttackModel].fVXaim3D / g_aAttackModel[nCntAttackModel].fVLaim3D * 20.0f;
					BulletMove.y = g_aAttackModel[nCntAttackModel].fVYaim3D / g_aAttackModel[nCntAttackModel].fVLaim3D * 20.0f;
					BulletMove.z = g_aAttackModel[nCntAttackModel].fVZaim3D / g_aAttackModel[nCntAttackModel].fVLaim3D * 20.0f;

					if (g_nDelayAttackModel % 2 == 0)
					{
						SetEmitterEffect(EMITTERTYPE_ACCELEHORMING, 0, 300, 30, 30.0f, 0.8f, g_aAttackModel[nCntAttackModel].pos, BulletMove, D3DXCOLOR(fColR, fColG, fColB, 1.0f), 0.0f, nCntAttackModel, 0.0f,
							NULL_VECTOR3);
					}

				//==============================================================================================================================================
			
				if (fLineLength <= 5000.0f &&
					g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMax.y >= pPlayer->pos.y &&
					g_aAttackModel[nCntAttackModel].pos.y + g_aAttackModel[nCntAttackModel].vtxMin.y <= pPlayer->pos.y + pPlayer->Size.y)
				{
					AttackHitPlayer(100);
				}

				if (pBoss->bUse == false)
				{
					g_aAttackModel[nCntAttackModel].bUse = false;
				}


				//==================================================================================================================================

				break;
			}

			//=====================
			//移動
			//=====================
			g_aAttackModel[nCntAttackModel].PosOld = g_aAttackModel[nCntAttackModel].pos;
			g_aAttackModel[nCntAttackModel].pos += g_aAttackModel[nCntAttackModel].move;

			//=====================
			//体力を減らす
			//=====================
			g_aAttackModel[nCntAttackModel].nLife -= 1;

			if (g_aAttackModel[nCntAttackModel].nLife <= 0)
			{
				g_aAttackModel[nCntAttackModel].bUse = false;
			}

			if (!g_aAttackModel[nCntAttackModel].bUse)
			{
				if (g_aAttackModel[nCntAttackModel].nType == ATTACKMODEL03_BOMB)
				{
					if (g_aAttackModel[nCntAttackModel].nBehaviorType == ATTACKMODELBEHAVIOR00_NORMAL)
					{
						SetAttackModel(ATTACKMODEL02_SPHERE, 60, g_aAttackModel[nCntAttackModel].pos, NULL_VECTOR3, NULL_VECTOR3, NULL_VECTOR3,
							0.0f, 0.0f, 0.1f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
					}
					else
					{
						SetAttackModel(ATTACKMODEL02_SPHERE, 60, g_aAttackModel[nCntAttackModel].pos, NULL_VECTOR3, NULL_VECTOR3, NULL_VECTOR3,
							0.0f, 0.0f, 0.2f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
					}
				}
				else if (g_aAttackModel[nCntAttackModel].nType == ATTACKMODEL07_METEO)
				{
					for (int nCntParticle = 0; nCntParticle < 100; nCntParticle++)
					{
						float fRandRot = float(rand() % 200 + 1) / 10;
						float fRandSpeed = float(rand() % 40 + 1);
						float fRandMove = float(rand() % 400) / 10;
						float fRandumColorR = float(rand() % 100 + 1) / 100;
						float fRandumColorG = float(rand() % 100 + 1) / 100;
						float fRandumColorB = float(rand() % 100 + 1) / 100;

						SetParticle(PARTICLETYPE_GRAVITY, 25, 10.0f, 0.2f, g_aAttackModel[nCntAttackModel].pos,
							D3DXVECTOR3(sinf(D3DX_PI * fRandRot) * fRandSpeed, fRandMove, cosf(D3DX_PI * fRandRot) * fRandSpeed),
							D3DXCOLOR(fRandumColorR, fRandumColorG, fRandumColorB, 1.0f), 0.0f, PARTICLETEX_NORMAL, 0);
					}
				}
				else if (g_aAttackModel[nCntAttackModel].nType == ATTACKMODEL09_LASER)
				{
					StopSound(SOUND_LABEL_SE_RASER_000); 
				}
			}


			SetPositionShadow(g_aAttackModel[nCntAttackModel].nIdxShadow, D3DXVECTOR3(g_aAttackModel[nCntAttackModel].pos.x, g_aAttackModel[nCntAttackModel].pos.y, g_aAttackModel[nCntAttackModel].pos.z));
			if (g_aAttackModel[nCntAttackModel].bUse == false)
			{
				KillShadow(g_aAttackModel[nCntAttackModel].nIdxShadow);
			}

		}
	}

}

//================================================================================================================================================================================================================================
// 描画処理
//================================================================================================================================================================================================================================
void DrawAttackModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;	//計算用マトリックス
	D3DMATERIAL9 matDef;					//現在のマテリアル保存用
	D3DXMATERIAL* pMat;						//マテリアルのデータへのポインタ

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	for (int nCntAttackModel = 0; nCntAttackModel < MAX_ATTACKMODEL; nCntAttackModel++)
	{
		if (g_aAttackModel[nCntAttackModel].bUse)
		{//使用されている場合
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aAttackModel[nCntAttackModel].mtxWorld);

			//大きさを反映
			D3DXMatrixScaling(&mtxScale, g_aAttackModel[nCntAttackModel].Scale.y, g_aAttackModel[nCntAttackModel].Scale.x, g_aAttackModel[nCntAttackModel].Scale.z);
			D3DXMatrixMultiply(&g_aAttackModel[nCntAttackModel].mtxWorld, &g_aAttackModel[nCntAttackModel].mtxWorld, &mtxScale);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aAttackModel[nCntAttackModel].rot.y, g_aAttackModel[nCntAttackModel].rot.x, g_aAttackModel[nCntAttackModel].rot.z);
			D3DXMatrixMultiply(&g_aAttackModel[nCntAttackModel].mtxWorld, &g_aAttackModel[nCntAttackModel].mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aAttackModel[nCntAttackModel].pos.x, g_aAttackModel[nCntAttackModel].pos.y, g_aAttackModel[nCntAttackModel].pos.z);
			D3DXMatrixMultiply(&g_aAttackModel[nCntAttackModel].mtxWorld, &g_aAttackModel[nCntAttackModel].mtxWorld, &mtxTrans);

			//===============================================================
			//描画の調整
			//===============================================================
			
			//法線の長さを１にする。（スケールなどを使った時は、必ず使う。)
			pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aAttackModel[nCntAttackModel].mtxWorld);

			//マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_aAttackModelInfo[g_aAttackModel[nCntAttackModel].nType].pBuffMat->GetBufferPointer();

			//============================
			//描画の調整
			//============================
			//アルファテストを有効
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

			//==================================================================

			for (int nCntAttackModel2 = 0; nCntAttackModel2 < (int)g_aAttackModelInfo[g_aAttackModel[nCntAttackModel].nType].dwNumMat; nCntAttackModel2++)
			{
				pMat[nCntAttackModel2].MatD3D.Diffuse.a = g_aAttackModel[nCntAttackModel].DiffUse[g_aAttackModel[nCntAttackModel].nType]->a;
				//マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntAttackModel2].MatD3D);

				//テクスチャの設定
				pDevice->SetTexture(0, g_aAttackModelInfo[g_aAttackModel[nCntAttackModel].nType].pTexture[nCntAttackModel2]);

				//モデル(パーツ)の描画
				g_aAttackModelInfo[g_aAttackModel[nCntAttackModel].nType].pMesh->DrawSubset(nCntAttackModel2);
			}

			//============================
			//描画の調整をもとに戻す
			//============================
			//アルファテストを無効に戻す
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

			//=====================================================================

			//次のcppでスケールを使うとは限らない（法線の初期値は1.0fなので）から、法線のノーマライズ（1.0f)にするのをオフにする。
			pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
		}
	}

	//保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//================================================================================================================================================================================================================================
// 設定処理
//================================================================================================================================================================================================================================
void SetAttackModel(int nType, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, D3DXVECTOR3 Scale,
	float fRot, float fSaveRot, float fExpansion, D3DXVECTOR3 SupportPos, int nBehaviorType)
{
	BYTE* pVtxBuff;	//頂点バッファへのポインタ

	Player* pPlayer = GetPlayer();

	for (int nCntAttackModel = 0; nCntAttackModel < MAX_ATTACKMODEL; nCntAttackModel++)
	{
		if (!g_aAttackModel[nCntAttackModel].bUse)
		{//使用されていない場合
			g_aAttackModel[nCntAttackModel].pos = pos;		                //位置
			g_aAttackModel[nCntAttackModel].move = move;                    //移動量
			g_aAttackModel[nCntAttackModel].rot = rot;		                //向き
			g_aAttackModel[nCntAttackModel].Vrot = rot - D3DXVECTOR3(D3DX_PI,D3DX_PI,D3DX_PI);//ベクトルを考慮した向き
			g_aAttackModel[nCntAttackModel].Scale = Scale;                  //大きさ
			g_aAttackModel[nCntAttackModel].nType = nType;	                //種類
			g_aAttackModel[nCntAttackModel].nBehaviorType = nBehaviorType;	//挙動の種類
			g_aAttackModel[nCntAttackModel].bUse = true;	                //使用する
			g_aAttackModel[nCntAttackModel].fRot = fRot;                    //動く向きを決める
			g_aAttackModel[nCntAttackModel].nLife = nLife;                  //体力
			g_aAttackModel[nCntAttackModel].fSaveRot = fSaveRot;            //プレイヤーがブーメランを投げた向きを記録する
			g_aAttackModel[nCntAttackModel].fTotalRotMove = 0.0f;           //プレイヤーがブーメランを投げてから回転移動をした合計量
			g_aAttackModel[nCntAttackModel].bTurnFlag = false;              //ブーメランをターンさせるフラグ
			g_aAttackModel[nCntAttackModel].nCntTime = 0;                   //出現してからの時間をカウントする 
			g_aAttackModel[nCntAttackModel].fExpansion = fExpansion;        //拡大量
			g_aAttackModel[nCntAttackModel].nMaxLife = nLife;               //最大体力
			g_aAttackModel[nCntAttackModel].PosLength = NULL_VECTOR3;       //ある物体との距離を求める時に使う
			g_aAttackModel[nCntAttackModel].fVaim = 0.0f;                   //ベクトル
			g_aAttackModel[nCntAttackModel].fVXaim = 0.0f;                  //X方向のベクトル
			g_aAttackModel[nCntAttackModel].fVZaim = 0.0f;                  //Z方向のベクトル
			g_aAttackModel[nCntAttackModel].PosOld = NULL_VECTOR3;          //1f前の位置
			g_aAttackModel[nCntAttackModel].DiffUse[nType]->a = 1.0f;
			g_aAttackModel[nCntAttackModel].DiffUse[nType]->r = 1.0f;
			g_aAttackModel[nCntAttackModel].DiffUse[nType]->g = 1.0f;
			g_aAttackModel[nCntAttackModel].DiffUse[nType]->b = 1.0f;
			g_aAttackModel[nCntAttackModel].OriginallyVtxMin = NULL_VECTOR3;
			g_aAttackModel[nCntAttackModel].OriginallyVtxMax = NULL_VECTOR3;
			g_aAttackModel[nCntAttackModel].vtxMin = NULL_VECTOR3;
			g_aAttackModel[nCntAttackModel].vtxMax = NULL_VECTOR3;
			g_aAttackModel[nCntAttackModel].Size = NULL_VECTOR3;	//大きさ
			g_aAttackModel[nCntAttackModel].bIsLanding = false;		//地面にいるかどうか
			g_aAttackModel[nCntAttackModel].fSpeed = 0.0f;			//スピード
			g_aAttackModel[nCntAttackModel].nNumLandingModel = 0;	//どのモデルの上に乗っているか
			g_aAttackModel[nCntAttackModel].nTurnCnt = 0;            //ブーメランがターンを開始したら、カウントを開始
			g_aAttackModel[nCntAttackModel].bCurveLaser = false;                    //レーザーをカーブさせる向きを決める
			g_aAttackModel[nCntAttackModel].SupportPos = SupportPos;        //支点となる位置
			g_aAttackModel[nCntAttackModel].nIdxShadow = SetShadow();//影のインデックス
			g_aAttackModel[nCntAttackModel].fBoomerangRange = pPlayer->aBoomerangInfo.fBoomerangRenge;//召喚された時点での射程を保存
			//=============================
		    //三次元自機狙い変数
		    //=============================
			g_aAttackModel[nCntAttackModel].fVXaim3D = 0.0f;
			g_aAttackModel[nCntAttackModel].fVYaim3D = 0.0f;
			g_aAttackModel[nCntAttackModel].fVZaim3D = 0.0f;
			g_aAttackModel[nCntAttackModel].fVLaim3D = 0.0f;
			
			//=============================
			//スフィア変数
			//=============================
			g_aAttackModel[nCntAttackModel].bSphereShot = false;//ライフが半分になったら自機狙いで動かす

			//頂点数の取得
			int nNumVtx = g_aAttackModelInfo[nType].pMesh->GetNumVertices();

			//頂点フォーマットのサイズを取得
			DWORD sizeFVF = D3DXGetFVFVertexSize(g_aAttackModelInfo[nType].pMesh->GetFVF());

			//頂点バッファのロック
			g_aAttackModelInfo[nType].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

			for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
			{
				//頂点座標の代入
				D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

				//モデルの最小値と最大値を取得
				if (vtx.x < g_aAttackModel[nCntAttackModel].OriginallyVtxMin.x)
				{//X座標の最小値
					g_aAttackModel[nCntAttackModel].OriginallyVtxMin.x = vtx.x;
				}
				if (vtx.y < g_aAttackModel[nCntAttackModel].OriginallyVtxMin.y)
				{//Y座標の最小値
					g_aAttackModel[nCntAttackModel].OriginallyVtxMin.y = vtx.y;
				}
				if (vtx.z < g_aAttackModel[nCntAttackModel].OriginallyVtxMin.z)
				{//Z座標の最小値
					g_aAttackModel[nCntAttackModel].OriginallyVtxMin.z = vtx.z;
				}
				if (vtx.x > g_aAttackModel[nCntAttackModel].OriginallyVtxMax.x)
				{//X座標の最大値
					g_aAttackModel[nCntAttackModel].OriginallyVtxMax.x = vtx.x;
				}
				if (vtx.y > g_aAttackModel[nCntAttackModel].OriginallyVtxMax.y)
				{//Y座標の最大値
					g_aAttackModel[nCntAttackModel].OriginallyVtxMax.y = vtx.y;
				}
				if (vtx.z > g_aAttackModel[nCntAttackModel].OriginallyVtxMax.z)
				{//Z座標の最大値
					g_aAttackModel[nCntAttackModel].OriginallyVtxMax.z = vtx.z;
				}


				//頂点フォーマットのサイズ分ポインタを進める
				pVtxBuff += sizeFVF;
			}

			//頂点バッファのアンロック
			g_aAttackModelInfo[nType].pMesh->UnlockVertexBuffer();

			break;
		}
	}
}

//================================================================================================================================================================================================================================
// 情報の取得
//================================================================================================================================================================================================================================
AttackModel* GetAttackModel(void)
{
	return &g_aAttackModel[0];
}

float fRotCollisionAModel(float x0, float x1, float x2, float z0, float z1, float z2)
{
	float a = x2 - x1;
	float b = z2 - z1;
	float a2 = a * a;
	float b2 = b * b;
	float r2 = a2 + b2;
	float tt = -(a * (x1 - x0) + b * (z1 - z0));
	if (tt < 0) {
		return (x1 - x0) * (x1 - x0) + (z1 - z0) * (z1 - z0);
	}
	if (tt > r2) {
		return (x2 - x0) * (x2 - x0) + (z2 - z0) * (z2 - z0);
	}
	float f1 = a * (z1 - z0) - b * (x1 - x0);
	return (f1 * f1) / r2;
}
