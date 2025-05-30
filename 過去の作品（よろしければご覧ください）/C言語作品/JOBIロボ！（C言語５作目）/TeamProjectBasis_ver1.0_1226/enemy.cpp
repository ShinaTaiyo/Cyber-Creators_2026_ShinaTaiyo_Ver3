//========================================================
//
//敵の処理[Enemy.cpp]
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
#include "item.h"
#include "particle.h"
#include "scrap.h"
#include "game.h"
#include "bossgauge.h"
#include "enemygauge.h"
#include "battery.h"
#include "effect.h"
#include "attackmodel.h"
#include "mission.h"
#include "field.h"
#include "Rader.h"
#include "debugproc.h"
#include "camera.h"
#include "timer.h"
#include "boss.h"
#include "combo.h"
#define TEST_MOTION	0

//==============================
//グローバル変数
//==============================
EnemyKey g_aEnemyKey[NUM_ENEMY][MAX_ENEMYPARTS];//各キー情報
EnemyKeySet g_aEnemyKeySet[NUM_ENEMY][MAX_ENEMYKEY][MAX_ENEMYMOTION];//各キーセット情報
EnemyMotionSet g_aEnemyMotionSet[NUM_ENEMY][MAX_ENEMYMOTION];//モーション情報
EnemyParts g_aEnemyParts[NUM_ENEMY][MAX_ENEMYPARTS];//敵の各パーツ情報
Enemy g_aEnemy[MAX_ENEMY];//モデル情報の構造体
int g_nDelayEnemy = 0;
int g_nldxShadowEnemy = -1;//対象の影のインデックス（番号）
int g_nLastDamageEnemy;//最後に攻撃したときのダメージ量4

//==============================
//初期化処理
//==============================
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスへのポインタと取得
	BYTE* pVtxBuff;							//頂点バッファへのポインタ
	D3DXMATERIAL* pMat;						//マテリアルへのポインタ

	//変数
	g_nDelayEnemy = 0;
	g_nLastDamageEnemy = 0;//最後に与えたダメージ量

	for (int nCntType = 0; nCntType < NUM_ENEMY; nCntType++)
	{
		for (int nCntParts = 0; nCntParts < ENEMY_NUMPARTS[nCntType]; nCntParts++)
		{
			g_aEnemyParts[nCntType][nCntParts].pBuffMat = {};
			g_aEnemyParts[nCntType][nCntParts].dwNumMat = {};
			g_aEnemyParts[nCntType][nCntParts].pMesh = {};
			g_aEnemyParts[nCntType][nCntParts].mtxWorld = {};
			g_aEnemyParts[nCntType][nCntParts].nIdxModelParent = -1;

			for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
			{
				g_aEnemy[nCntEnemy].parts[nCntType][nCntParts].pBuffMat = {};
				g_aEnemy[nCntEnemy].parts[nCntType][nCntParts].dwNumMat = {};
				g_aEnemy[nCntEnemy].parts[nCntType][nCntParts].pMesh = {};
				g_aEnemy[nCntEnemy].parts[nCntType][nCntParts].mtxWorld = {};
				g_aEnemy[nCntEnemy].parts[nCntType][nCntParts].nIdxModelParent = -1;

			}
		}
	}
	
	//Xファイルの読み込み
	for (int nCntType = 0; nCntType < NUM_ENEMY; nCntType++)
	{
		for (int nCntParts = 0; nCntParts < ENEMY_NUMPARTS[nCntType]; nCntParts++)
		{
			//タイプ0
			if (nCntType == 0)
			{
				D3DXLoadMeshFromX(ENEMY000_FILENAME[nCntParts], D3DXMESH_SYSTEMMEM, pDevice, NULL,
					&g_aEnemyParts[nCntType][nCntParts].pBuffMat,
					NULL,
					&g_aEnemyParts[nCntType][nCntParts].dwNumMat,
					&g_aEnemyParts[nCntType][nCntParts].pMesh);
			}
			//タイプ1
			else if (nCntType == 1)
			{
				D3DXLoadMeshFromX(ENEMY001_FILENAME[nCntParts], D3DXMESH_SYSTEMMEM, pDevice, NULL,
					&g_aEnemyParts[nCntType][nCntParts].pBuffMat,
					NULL,
					&g_aEnemyParts[nCntType][nCntParts].dwNumMat,
					&g_aEnemyParts[nCntType][nCntParts].pMesh);
			}
			//タイプ2
			else if (nCntType == 2)
			{
				D3DXLoadMeshFromX(ENEMY002_FILENAME[nCntParts], D3DXMESH_SYSTEMMEM, pDevice, NULL,
					&g_aEnemyParts[nCntType][nCntParts].pBuffMat,
					NULL,
					&g_aEnemyParts[nCntType][nCntParts].dwNumMat,
					&g_aEnemyParts[nCntType][nCntParts].pMesh);
			}
			//タイプ3
			else if (nCntType == 3)
			{
				D3DXLoadMeshFromX(ENEMY003_FILENAME[nCntParts], D3DXMESH_SYSTEMMEM, pDevice, NULL,
					&g_aEnemyParts[nCntType][nCntParts].pBuffMat,
					NULL,
					&g_aEnemyParts[nCntType][nCntParts].dwNumMat,
					&g_aEnemyParts[nCntType][nCntParts].pMesh);
			}
			//タイプ4
			else if (nCntType == 4)
			{
				D3DXLoadMeshFromX(ENEMY004_FILENAME[nCntParts], D3DXMESH_SYSTEMMEM, pDevice, NULL,
					&g_aEnemyParts[nCntType][nCntParts].pBuffMat,
					NULL,
					&g_aEnemyParts[nCntType][nCntParts].dwNumMat,
					&g_aEnemyParts[nCntType][nCntParts].pMesh);
			}
	
			//マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_aEnemyParts[nCntType][nCntParts].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aEnemyParts[nCntType][nCntParts].dwNumMat; nCntMat++)
			{
				//テクスチャファイルが存在する
				if (pMat[nCntMat].pTextureFilename != NULL)
				{
					//テクスチャの読み込み
					D3DXCreateTextureFromFile(pDevice,
						pMat[nCntMat].pTextureFilename,
						&g_aEnemyParts[nCntType][nCntParts].pTexture[nCntMat]);
				}

				for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
				{
					g_aEnemy[nCntEnemy].parts[nCntType][nCntParts].Diffuse[nCntMat] = pMat[nCntMat].MatD3D.Diffuse;//色合い
					g_aEnemy[nCntEnemy].parts[nCntType][nCntParts].FormarDiffuse[nCntMat] = pMat[nCntMat].MatD3D.Diffuse;//元の色合い
				}
			}
		}

		//モーション情報の読み込み
		LoadEnemyMotion(nCntType);
	}

	//構造体の情報
	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
	{
		g_aEnemy[nCnt].bUse = false;//使用しているかどうか
		g_aEnemy[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//位置
		g_aEnemy[nCnt].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//1F前の位置
		g_aEnemy[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//移動量
		g_aEnemy[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//向き
		g_aEnemy[nCnt].nIdxShadow = -1;//影のインデックス
		g_aEnemy[nCnt].nIdxRaderDisplay = -1;//レーダーディスプレイのインデックス
		g_aEnemy[nCnt].nType = -1;//種類
		g_aEnemy[nCnt].vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//それぞれの座標のモデルの位置の最小値
		g_aEnemy[nCnt].vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//それぞれの座標のモデルの位置の最大値
		g_aEnemy[nCnt].Size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//敵の大きさ
		g_aEnemy[nCnt].bIsLanding = false;//地面についているかどうか
		g_aEnemy[nCnt].fVaimY1 = 0.0f;//総合したY方向のベクトル
		g_aEnemy[nCnt].fVXaim = 0.0f;
		g_aEnemy[nCnt].fVYaim = 0.0f;
		g_aEnemy[nCnt].fVZaim = 0.0f;
		g_aEnemy[nCnt].fVaim = 0.0f;
		g_aEnemy[nCnt].fPenduram = 0.0f;//振り子のように弾を発射させる処理
		g_aEnemy[nCnt].bPenduramFlag = false;
		g_aEnemy[nCnt].fLength = 0.0f;//距離をとる
		g_aEnemy[nCnt].state = ENEMYSTATE_NEUTRAL;//状態
		g_aEnemy[nCnt].nAttackTiming = 0;//攻撃判定のタイミングを決める
		g_aEnemy[nCnt].bHitFlag = false;//攻撃がヒットしたかどうかを決める変数
		g_aEnemy[nCnt].fMoveStartLength = 0.0f;//敵が移動を開始する距離
		g_aEnemy[nCnt].fAttackStartLength = 0.0f;//敵が攻撃を開始する距離
		g_aEnemy[nCnt].bUnderAttack = false;//敵が攻撃中かどうかを決める
		g_aEnemy[nCnt].nNumLandingModel = 0;//どのモデルの上に乗っているか
		g_aEnemy[nCnt].bDefeatDropScrap = false;//倒されたときにスクラップをドロップするかどうか
		g_aEnemy[nCnt].bSummonByBoss = false;//ボスによって召喚されたかどうか
		//========================================
		//敵の表示オプションの表示の変数
		//========================================
		g_aEnemy[nCnt].fOptionDispLength = 0.0f;//敵のオプションの表示を開始する距離
		g_aEnemy[nCnt].bUseEnemyDisplay = false;//敵のオプション表示を使用するかどうか

		//================================
		//状態の強制
		//================================
		g_aEnemy[nCnt].bCompulsionAttack = false;//強制敵に攻撃状態にする
		g_aEnemy[nCnt].bCompulsionNeutoral = false;//強制的にニュートラル状態にする
		
		//============================
		//三次元自機狙い処理
		//============================
		g_aEnemy[nCnt].fVXaim3D = 0.0f;
		g_aEnemy[nCnt].fVYaim3D = 0.0f;
		g_aEnemy[nCnt].fVZaim3D = 0.0f;
		g_aEnemy[nCnt].fVLaim3D = 0.0f;

		for (int nCntAttackModel = 0; nCntAttackModel < MAX_ATTACKMODEL; nCntAttackModel++)
		{
			g_aEnemy[nCnt].bHitStopBoomerang[nCntAttackModel] = false;//ブーメランに対するヒットストップ判定をする
			g_aEnemy[nCnt].nCoolTimeBoomerang[nCntAttackModel] = 0;//ブーメランに対するヒットストップの時間をカウントする
		}

		//==========================
		//攻撃受け付け調整
		//==========================
		for (int nCntHitStop = 0; nCntHitStop < PLAYERATTACK_MAX; nCntHitStop++)
		{
			g_aEnemy[nCnt].bHitStop[nCntHitStop] = false;//この変数がtrueの間、攻撃を受け付けない
			g_aEnemy[nCnt].nCoolTime[nCntHitStop] = 0;   //bHitStopをfalseにするまでのカウント
			g_aEnemy[nCnt].bWeakWeapon[nCntHitStop] = false;//どの武器に弱いかを決める。
		}

		//ステータス設定
		g_aEnemy[nCnt].nHp = 0;//体力
		g_aEnemy[nCnt].nAttack = 0;//攻撃力
		g_aEnemy[nCnt].nDefeatScore = 0;//倒したときにもらえるスコア
		g_aEnemy[nCnt].fSpeed = 0.0f;//敵の速さ
		g_aEnemy[nCnt].nMaxHp = 0;//敵の体力の最大値

		//===========================
		//中ボス変数
		//===========================
		g_aEnemy[nCnt].nLotteryAttack = 0;//どの攻撃をするかの抽選を行う。

		//各種類のサイズを求める
		for (int nCntType = 0; nCntType < NUM_ENEMY; nCntType++)
		{
			for (int nCntParts = 0; nCntParts < ENEMY_NUMPARTS[nCntType]; nCntParts++)
			{
				//頂点数の取得
				int nNumVtx = g_aEnemyParts[nCntType][nCntParts].pMesh->GetNumVertices();

				//頂点フォーマットのサイズを取得
				DWORD sizeFVF = D3DXGetFVFVertexSize(g_aEnemyParts[nCntType][nCntParts].pMesh->GetFVF());

				//頂点バッファのロック
				g_aEnemyParts[nCntType][nCntParts].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

				for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
				{
					//頂点座標の代入
					D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

					//モデルの最小値と最大値を取得
					if (vtx.x < g_aEnemy[nCnt].vtxMin.x)
					{//X座標の最小値
						g_aEnemy[nCnt].vtxMin.x = vtx.x;
					}
					if (vtx.y < g_aEnemy[nCnt].vtxMin.y)
					{//Y座標の最小値
						g_aEnemy[nCnt].vtxMin.y = vtx.y;
					}
					if (vtx.z < g_aEnemy[nCnt].vtxMin.z)
					{//Z座標の最小値
						g_aEnemy[nCnt].vtxMin.z = vtx.z;
					}
					if (vtx.x > g_aEnemy[nCnt].vtxMax.x)
					{//X座標の最大値
						g_aEnemy[nCnt].vtxMax.x = vtx.x;
					}
					if (vtx.y > g_aEnemy[nCnt].vtxMax.y)
					{//Y座標の最大値
						g_aEnemy[nCnt].vtxMax.y = vtx.y;
					}
					if (vtx.z > g_aEnemy[nCnt].vtxMax.z)
					{//Z座標の最大値
						g_aEnemy[nCnt].vtxMax.z = vtx.z;
					}

					//頂点フォーマットのサイズ分ポインタを進める
					pVtxBuff += sizeFVF;
				}

				//敵の大きさを決める。（椎名オリジナル処理)
				g_aEnemy[nCnt].Size = g_aEnemy[nCnt].vtxMax - g_aEnemy[nCnt].vtxMin;

				//頂点バッファのアンロック
				g_aEnemyParts[nCntType][nCntParts].pMesh->UnlockVertexBuffer();
			}
		}
	}
}

//==============================
//終了処理
//==============================
void UninitEnemy(void)
{
	for (int nCntType = 0; nCntType < NUM_ENEMY; nCntType++)
	{
		for (int nCntParts = 0; nCntParts < ENEMY_NUMPARTS[nCntType]; nCntParts++)
		{
			//メッシュの破棄
			if (g_aEnemyParts[nCntType][nCntParts].pMesh != NULL)
			{
				g_aEnemyParts[nCntType][nCntParts].pMesh->Release();
				g_aEnemyParts[nCntType][nCntParts].pMesh = NULL;
			}
			//マテリアルの破棄
			if (g_aEnemyParts[nCntType][nCntParts].pBuffMat != NULL)
			{
				g_aEnemyParts[nCntType][nCntParts].pBuffMat->Release();
				g_aEnemyParts[nCntType][nCntParts].pBuffMat = NULL;
			}

			//テクスチャの破棄
			for (int nCntMat = 0; nCntMat < MAX_ENEMYMAT; nCntMat++)
			{
				if (g_aEnemyParts[nCntType][nCntParts].pTexture[nCntMat] != NULL)
				{
					g_aEnemyParts[nCntType][nCntParts].pTexture[nCntMat]->Release();
					g_aEnemyParts[nCntType][nCntParts].pTexture[nCntMat] = NULL;
					
				}
			}

			for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
			{
				//メッシュの破棄
				if (g_aEnemy[nCntEnemy].parts[nCntType][nCntParts].pMesh != NULL)
				{
					g_aEnemy[nCntEnemy].parts[nCntType][nCntParts].pMesh->Release();
					g_aEnemy[nCntEnemy].parts[nCntType][nCntParts].pMesh = NULL;

				}
				
				//頂点バッファの破棄
				if (g_aEnemy[nCntEnemy].parts[nCntType][nCntParts].pBuffMat != NULL)
				{
					g_aEnemy[nCntEnemy].parts[nCntType][nCntParts].pBuffMat->Release();
					g_aEnemy[nCntEnemy].parts[nCntType][nCntParts].pBuffMat = NULL;

				}

				//テクスチャの破棄
				for (int nCntMat = 0; nCntMat < MAX_ENEMYMAT; nCntMat++)
				{
					if (g_aEnemy[nCntEnemy].parts[nCntType][nCntParts].pTexture[nCntMat] != NULL)
					{
						g_aEnemy[nCntEnemy].parts[nCntType][nCntParts].pTexture[nCntMat]->Release();
						g_aEnemy[nCntEnemy].parts[nCntType][nCntParts].pTexture[nCntMat] = NULL;

					}
				}

			}
		}
	}
}

//============================
//更新処理
//============================
void UpdateEnemy(void)
{
	Player* pPlayer = GetPlayer();		//プレイヤーの情報を取得
	Field* pField = GetField();			//フィールドの情報を取得
	EnemyHP* pEnemyHP = GetEnemyGauge();//敵の体力ゲージ情報を取得
	EnemyWeakDisp* pEnemyWeakDisp = GetEnemyWeakDisp();//敵の弱点表記の情報を取得
	Boss* pBoss = GetBoss();//ボスの情報を取得
	g_nDelayEnemy++;

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemyHP++, pEnemyWeakDisp++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{//使用している場合
			if (g_aEnemy[nCntEnemy].pos.y <= 0.0f)
			{//地面に触れている場合
				g_aEnemy[nCntEnemy].bIsLanding = true;//地面に立っている
			}

			if (g_aEnemy[nCntEnemy].bIsLanding == false)
			{//地面に立っていない場合
				g_aEnemy[nCntEnemy].move.y += -0.4f;//重力処理
			}
			else
			{
				Model* pModel = GetModel();
				g_aEnemy[nCntEnemy].move.y = 0.0f;

				for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++, pModel++)
				{
					if (pModel->bUse == true && nCntModel == g_aEnemy[nCntEnemy].nNumLandingModel)
					{
						g_aEnemy[nCntEnemy].pos.y = pModel->pos.y + pModel->vtxMax.y;
						break;
					}
				}
			}

			//==============================================================
			//ボスが出現した瞬間にボスによって召喚されてない敵を消す
			//==============================================================
			if (pBoss->bUse == true && g_aEnemy[nCntEnemy].bSummonByBoss == false)
			{
				g_aEnemy[nCntEnemy].bUse = false;
				g_aEnemy[nCntEnemy].bDefeatDropScrap = false;//倒された判定になっても、スクラップをドロップしない。
			}

			//===========================================
			//汎用的に使う乱数値
			//===========================================
			float fPurposeRandRot = float(rand() % 200 + 1) / 100.0f;
			float fPurposeRandSpeed = float(rand() % 20 + 1);
			float fPurposeRandSpeedY = float(rand() % 20 + 1);

			float fPurposeoPosX = float(rand() % 400 - 200);
			float fPurposeoPosY = float(rand() % 400 - 200);
			float fPurposeoPosZ = float(rand() % 400 - 200);

			CalculateMatrixEnemy();//マトリックスを計算する

			//======================================
			//攻撃受け付け調整
			//======================================
			AttackModel* pAttackModel = GetAttackModel();

			for (int nCntHitStop = 0; nCntHitStop < PLAYERATTACK_MAX; nCntHitStop++)
			{
				if (g_aEnemy[nCntEnemy].bHitStop[nCntHitStop] == true)
				{
					g_aEnemy[nCntEnemy].nCoolTime[nCntHitStop]++;

					switch (nCntHitStop)
					{
					case PLAYERATTACK00_SWORD:

						if (g_aEnemy[nCntEnemy].nCoolTime[nCntHitStop] >= 20)
						{
							g_aEnemy[nCntEnemy].bHitStop[nCntHitStop] = false;
							g_aEnemy[nCntEnemy].nCoolTime[nCntHitStop] = 0;
						}
						break;

					case PLAYERATTACK02_SPEAKER:

						if (g_aEnemy[nCntEnemy].nCoolTime[nCntHitStop] >= 2)
						{
							g_aEnemy[nCntEnemy].bHitStop[nCntHitStop] = false;
							g_aEnemy[nCntEnemy].nCoolTime[nCntHitStop] = 0;
						}
						break;
					}
				}
			}

			//==========================================
			//ブーメランに対するヒットストップ
			//==========================================
			for (int nCntAttackModel = 0; nCntAttackModel < MAX_ATTACKMODEL; nCntAttackModel++, pAttackModel++)
			{
				if (g_aEnemy[nCntEnemy].bHitStopBoomerang[nCntAttackModel] == true)
				{
					g_aEnemy[nCntEnemy].nCoolTimeBoomerang[nCntAttackModel]++;
					if (g_aEnemy[nCntEnemy].nCoolTimeBoomerang[nCntAttackModel] >= 20)
					{
						g_aEnemy[nCntEnemy].bHitStopBoomerang[nCntAttackModel] = false;
						g_aEnemy[nCntEnemy].nCoolTimeBoomerang[nCntAttackModel] = 0;
					}
				}
			}

			//=========================================
			// 敵とプレイヤーの距離を取る。
			//=========================================
			float fLength = sqrtf((g_aEnemy[nCntEnemy].pos.x - pPlayer->pos.x) * (g_aEnemy[nCntEnemy].pos.x - pPlayer->pos.x) +
				(g_aEnemy[nCntEnemy].pos.z - pPlayer->pos.z) * (g_aEnemy[nCntEnemy].pos.z - pPlayer->pos.z)) / 2;

			g_aEnemy[nCntEnemy].fVXaim = pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x;
			g_aEnemy[nCntEnemy].fVYaim = pPlayer->pos.y - g_aEnemy[nCntEnemy].pos.y;
			g_aEnemy[nCntEnemy].fVZaim = pPlayer->pos.z - g_aEnemy[nCntEnemy].pos.z;

			g_aEnemy[nCntEnemy].fVaim = atan2f(g_aEnemy[nCntEnemy].fVXaim, g_aEnemy[nCntEnemy].fVZaim);
			g_aEnemy[nCntEnemy].fVaimY1 = atan2f(g_aEnemy[nCntEnemy].fVYaim, g_aEnemy[nCntEnemy].fVZaim);



			//===============================
			//攻撃処理、移動処理
			//===============================
			switch (g_aEnemy[nCntEnemy].nType)
			{
			case 0://レッドブル

				//=========================
				//行動を決める
				//=========================
				if (fLength < g_aEnemy[nCntEnemy].fAttackStartLength)
				{
					g_aEnemy[nCntEnemy].state = ENEMYSTATE_ATTACK;//攻撃状態
					g_aEnemy[nCntEnemy].bCompulsionAttack = true;
				}
				//範囲外
				else
				{
					g_aEnemy[nCntEnemy].rot.y = g_aEnemy[nCntEnemy].fVaim + D3DX_PI;//プレイヤーの方向を向く
					g_aEnemy[nCntEnemy].state = ENEMYSTATE_MOVE;//移動状態
				}

				if (g_nDelayEnemy % 180 == 0)
				{
					g_aEnemy[nCntEnemy].bCompulsionNeutoral = false;
					int nLottery = rand() % 5;

					if (nLottery == 0)
					{
						g_aEnemy[nCntEnemy].bCompulsionNeutoral = true;
					}
				}

				//===========================
				//状態の強制
				//===========================
				if (g_aEnemy[nCntEnemy].bCompulsionNeutoral == true)
				{
					g_aEnemy[nCntEnemy].state = ENEMYSTATE_NEUTRAL;
				}

				if (g_aEnemy[nCntEnemy].bCompulsionAttack == true)
				{
					g_aEnemy[nCntEnemy].state = ENEMYSTATE_ATTACK;//ニュートラル状態
				}

				if (g_aEnemy[nCntEnemy].state == ENEMYSTATE_ATTACK)
				{
					g_aEnemy[nCntEnemy].bUnderAttack = true;//攻撃が始まったので、攻撃中の判定をするフラグをtrueにする

					float fLengthAttack = sqrtf((g_aEnemy[nCntEnemy].parts[0][0].PartsPos.x - pPlayer->pos.x) * (g_aEnemy[nCntEnemy].parts[0][0].PartsPos.x - pPlayer->pos.x) +
						(g_aEnemy[nCntEnemy].parts[0][0].PartsPos.z - pPlayer->pos.z) * (g_aEnemy[nCntEnemy].parts[0][0].PartsPos.z - pPlayer->pos.z) +
						(g_aEnemy[nCntEnemy].parts[0][0].PartsPos.y - pPlayer->pos.y) * (g_aEnemy[nCntEnemy].parts[0][0].PartsPos.y - pPlayer->pos.y)) / 3;

					g_aEnemy[nCntEnemy].nAttackTiming++;

					if (g_aEnemy[nCntEnemy].nAttackTiming == 1)
					{//攻撃が始まった時にプレイヤーの位置に向く
						g_aEnemy[nCntEnemy].rot.y = g_aEnemy[nCntEnemy].fVaim + D3DX_PI;//プレイヤーの方向を向く（攻撃時間ごとに向きを変える)
					}

					if (fLengthAttack < 20.0f && g_aEnemy[nCntEnemy].bHitFlag == false)
					{
						if (g_aEnemy[nCntEnemy].nAttackTiming >= 34 && g_aEnemy[nCntEnemy].nAttackTiming <= 50)
						{
							AttackHitPlayer(35);//攻撃がヒットした時にダメージ状態にする

							g_aEnemy[nCntEnemy].bHitFlag = true;
						}
					}
				}
				break;

			case 1://ミルタンク

				//===========================================
				//２秒ごとに行動を決める
				//===========================================
				if (g_nDelayEnemy % 120 == 0)
				{
					int nLottery = rand() % 4;
					if (nLottery == 0)
					{
						g_aEnemy[nCntEnemy].bUnderAttack = false;
						g_aEnemy[nCntEnemy].bCompulsionNeutoral = false;

					}
					else if (nLottery >= 1 && nLottery <= 2)
					{
						g_aEnemy[nCntEnemy].bUnderAttack = true;
						g_aEnemy[nCntEnemy].bCompulsionNeutoral = false;

					}
					else if (nLottery == 3)
					{
						g_aEnemy[nCntEnemy].bCompulsionNeutoral = true;
					}
				}

				if (g_aEnemy[nCntEnemy].bUnderAttack == false)
				{
					g_aEnemy[nCntEnemy].rot.y = g_aEnemy[nCntEnemy].fVaim + D3DX_PI;//プレイヤーの方向を向く
					g_aEnemy[nCntEnemy].state = ENEMYSTATE_MOVE;//移動状態
				}
				else
				{
					g_aEnemy[nCntEnemy].state = ENEMYSTATE_ATTACK;
				}

				if (g_aEnemy[nCntEnemy].state == ENEMYSTATE_ATTACK)
				{
					g_aEnemy[nCntEnemy].nAttackTiming++;

					if (g_aEnemy[nCntEnemy].nAttackTiming == 5)
					{
						for (int nCntBullet = 0; nCntBullet < 3; nCntBullet++)
						{
							SetBullet(BULLETTYPE_ENEMY, 120, 10.0f, 10.0f,
								D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y + 30.0f, g_aEnemy[nCntEnemy].pos.z), D3DXVECTOR3(sinf(g_aEnemy[nCntEnemy].rot.y + D3DX_PI) * (5.0f + 3.0f * nCntBullet), 0.0f, cosf(g_aEnemy[nCntEnemy].rot.y + D3DX_PI) * (5.0f + 3.0f * nCntBullet)),
								NULL_VECTOR3, D3DXCOLOR(0.545f, 0.27f, 0.074f, 1.0f), nCntEnemy, 0.0f);
						}
					}
				}
				break;

			case 2://ミックスジューチュ

				//===========================================
				//２秒ごとに行動を決める
				//===========================================
				if (g_nDelayEnemy % 120 == 0)
				{
					int nLottery = rand() % 4;
					if (nLottery == 0)
					{
						g_aEnemy[nCntEnemy].bUnderAttack = false;
						g_aEnemy[nCntEnemy].bCompulsionNeutoral = false;

					}
					else if (nLottery >= 1 && nLottery <= 2)
					{
						g_aEnemy[nCntEnemy].bUnderAttack = true;
						g_aEnemy[nCntEnemy].bCompulsionNeutoral = false;

					}
					else if (nLottery == 3)
					{
						g_aEnemy[nCntEnemy].bCompulsionNeutoral = true;
					}
				}

				if (g_aEnemy[nCntEnemy].bUnderAttack == false)
				{
					g_aEnemy[nCntEnemy].rot.y = g_aEnemy[nCntEnemy].fVaim + D3DX_PI;//プレイヤーの方向を向く
					g_aEnemy[nCntEnemy].state = ENEMYSTATE_MOVE;//移動状態
				}
				else
				{
					g_aEnemy[nCntEnemy].state = ENEMYSTATE_ATTACK;
				}
				
				if (g_aEnemy[nCntEnemy].state == ENEMYSTATE_ATTACK)
				{
					g_aEnemy[nCntEnemy].nAttackTiming++;

					if (g_aEnemy[nCntEnemy].nAttackTiming == 1)
					{//攻撃が始まった瞬間に
						g_aEnemy[nCntEnemy].rot.y = g_aEnemy[nCntEnemy].fVaim + D3DX_PI;//プレイヤーの方向を向く
					}

					if (g_aEnemy[nCntEnemy].nAttackTiming == 120)
					{
						SetBullet(BULLETTYPE_RING, 90, 20.0f, 20.0f,
							D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y + 20.0f, g_aEnemy[nCntEnemy].pos.z), D3DXVECTOR3(sinf(g_aEnemy[nCntEnemy].rot.y + D3DX_PI) * 8.0f, 0.0f, cosf(g_aEnemy[nCntEnemy].rot.y + D3DX_PI) * 8.0f),
							NULL_VECTOR3, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), nCntEnemy, 0.1f);
					}
				}
				break;

			case 3://モンスター

				//=========================
				//行動を決める
				//=========================
				if (fLength < g_aEnemy[nCntEnemy].fAttackStartLength)
				{//範囲内
					g_aEnemy[nCntEnemy].state = ENEMYSTATE_ATTACK;//攻撃状態
					g_aEnemy[nCntEnemy].bCompulsionAttack = true;
				}
				else
				{//範囲外
					g_aEnemy[nCntEnemy].rot.y = g_aEnemy[nCntEnemy].fVaim + D3DX_PI;//プレイヤーの方向を向く
					g_aEnemy[nCntEnemy].state = ENEMYSTATE_MOVE;//移動状態
				}

				if (g_nDelayEnemy % 180 == 0)
				{
					g_aEnemy[nCntEnemy].bCompulsionNeutoral = false;
					int nLottery = rand() % 5;

					if (nLottery == 0)
					{
						g_aEnemy[nCntEnemy].bCompulsionNeutoral = true;
					}
				}

				//===========================
				//状態の強制
				//===========================
				if (g_aEnemy[nCntEnemy].bCompulsionNeutoral == true)
				{
					g_aEnemy[nCntEnemy].state = ENEMYSTATE_NEUTRAL;
				}

				if (g_aEnemy[nCntEnemy].bCompulsionAttack == true)
				{
					g_aEnemy[nCntEnemy].state = ENEMYSTATE_ATTACK;//ニュートラル状態

				}

				if (g_aEnemy[nCntEnemy].state == ENEMYSTATE_ATTACK)
				{
					g_aEnemy[nCntEnemy].bUnderAttack = true;//攻撃が始まったので、攻撃中の判定をするフラグをtrueにする

					//右手とプレイヤーの距離
					float fLengthAttack = sqrtf((g_aEnemy[nCntEnemy].parts[3][2].PartsPos.x - pPlayer->pos.x) * (g_aEnemy[nCntEnemy].parts[3][2].PartsPos.x - pPlayer->pos.x) +
						(g_aEnemy[nCntEnemy].parts[3][2].PartsPos.z - pPlayer->pos.z) * (g_aEnemy[nCntEnemy].parts[3][2].PartsPos.z - pPlayer->pos.z) +
						(g_aEnemy[nCntEnemy].parts[3][2].PartsPos.y - pPlayer->pos.y) * (g_aEnemy[nCntEnemy].parts[3][2].PartsPos.y - pPlayer->pos.y)) / 3;

					g_aEnemy[nCntEnemy].nAttackTiming++;

					if (g_aEnemy[nCntEnemy].nAttackTiming == 1)
					{//攻撃が始まった時にプレイヤーの位置に向く
						g_aEnemy[nCntEnemy].rot.y = g_aEnemy[nCntEnemy].fVaim + D3DX_PI;//プレイヤーの方向を向く（攻撃時間ごとに向きを変える)
					}

					SetEffect(EFFECTTYPE_NORMAL, 200, 20.0f, 20.0f, 1.0f, g_aEnemy[nCntEnemy].parts[3][2].PartsPos, NULL_VECTOR3, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 0.0f, 0);

					if (fLengthAttack < 20.0f && g_aEnemy[nCntEnemy].bHitFlag == false)
					{
						if (g_aEnemy[nCntEnemy].nAttackTiming >= 28 && g_aEnemy[nCntEnemy].nAttackTiming <= 40)
						{
							AttackHitPlayer(40);//攻撃がヒットした時にダメージ状態にする

							g_aEnemy[nCntEnemy].bHitFlag = true;
						}
					}
				}
				break;

			case 4://カップ麺

				if (fLength < g_aEnemy[nCntEnemy].fMoveStartLength)
				{//範囲内
					//===========================================
					//２秒ごとに攻撃が移動かを決める
					//===========================================
					if (g_nDelayEnemy % 120 == 0)
					{
						int nLottery = rand() % 2;
						if (nLottery == 0)
						{
							g_aEnemy[nCntEnemy].bUnderAttack = false;
						}
						else
						{
							g_aEnemy[nCntEnemy].bUnderAttack = true;
						}
					}

					if (g_aEnemy[nCntEnemy].bUnderAttack == false)
					{
						g_aEnemy[nCntEnemy].rot.y = g_aEnemy[nCntEnemy].fVaim + D3DX_PI;//プレイヤーの方向を向く
						g_aEnemy[nCntEnemy].state = ENEMYSTATE_MOVE;//移動状態
					}
					else
					{
						g_aEnemy[nCntEnemy].state = ENEMYSTATE_ATTACK;
					}
				}
				else
				{//範囲外
					if (g_aEnemy[nCntEnemy].bUnderAttack == false)
					{
						g_aEnemy[nCntEnemy].state = ENEMYSTATE_NEUTRAL;//ニュートラル状態
					}
				}

				if (g_aEnemy[nCntEnemy].state == ENEMYSTATE_ATTACK)
				{
					g_aEnemy[nCntEnemy].nAttackTiming++;

					if (g_aEnemy[nCntEnemy].nAttackTiming == 1)
					{//攻撃が始まった瞬間にプレイヤーの方向を向く
						g_aEnemy[nCntEnemy].nLotteryAttack = rand() % 4;
						g_aEnemy[nCntEnemy].rot.y = g_aEnemy[nCntEnemy].fVaim + D3DX_PI;//プレイヤーの方向を向く
					}

					if (g_aEnemy[nCntEnemy].nLotteryAttack == 0)
					{//攻撃０：汁を飛ばす
						if (g_aEnemy[nCntEnemy].nAttackTiming % 5 == 0)
						{
							SetAttackModel(ATTACKMODEL08_SOUP, 100, g_aEnemy[nCntEnemy].parts[4][1].PartsPos,
								D3DXVECTOR3(sinf(D3DX_PI * fPurposeRandRot) * fPurposeRandSpeed, fPurposeRandSpeedY / 2.0f, cosf(D3DX_PI * fPurposeRandRot) * fPurposeRandSpeed), NULL_VECTOR3, ONE_VECTOR3, 0.0f, 0.0f, 0.1f, NULL_VECTOR3, ATTACKMODELBEHAVIOR01_ANOTHER);
						}
					}
					else if (g_aEnemy[nCntEnemy].nLotteryAttack == 1)
					{//攻撃１：スフィア弾
						if (g_aEnemy[nCntEnemy].nAttackTiming % 30 == 0)
						{
							SetAttackModel(ATTACKMODEL02_SPHERE, 200, g_aEnemy[nCntEnemy].pos, NULL_VECTOR3, NULL_VECTOR3, ONE_VECTOR3, 0.0f, 0.0f, 0.03f, NULL_VECTOR3, ATTACKMODELBEHAVIOR01_ANOTHER);
						}
					}
					else if (g_aEnemy[nCntEnemy].nLotteryAttack == 2)
					{//攻撃２：サンダーバリア

						//============================
						//点滅処理
						//============================
						if (g_aEnemy[nCntEnemy].nAttackTiming >= 0 && g_aEnemy[nCntEnemy].nAttackTiming <= 40)
						{
							if (g_aEnemy[nCntEnemy].nAttackTiming % 2 == 0)
							{
								for (int nCntParts = 0; nCntParts < ENEMY_NUMPARTS[g_aEnemy[nCntEnemy].nType]; nCntParts++)
								{
									for (int nCntMat = 0; nCntMat < (int)(g_aEnemyParts[g_aEnemy[nCntEnemy].nType][nCntParts].dwNumMat); nCntMat++)
									{
										g_aEnemy[nCntEnemy].parts[g_aEnemy[nCntEnemy].nType][nCntParts].Diffuse[nCntMat].b = 1.0f;
										g_aEnemy[nCntEnemy].parts[g_aEnemy[nCntEnemy].nType][nCntParts].Diffuse[nCntMat].r = 0.0f;
										g_aEnemy[nCntEnemy].parts[g_aEnemy[nCntEnemy].nType][nCntParts].Diffuse[nCntMat].g = 0.0f;
									}
								}
							}
							else
							{
								for (int nCntParts = 0; nCntParts < ENEMY_NUMPARTS[g_aEnemy[nCntEnemy].nType]; nCntParts++)
								{
									for (int nCntMat = 0; nCntMat < (int)(g_aEnemyParts[g_aEnemy[nCntEnemy].nType][nCntParts].dwNumMat); nCntMat++)
									{
										g_aEnemy[nCntEnemy].parts[g_aEnemy[nCntEnemy].nType][nCntParts].Diffuse[nCntMat] =
											g_aEnemy[nCntEnemy].parts[g_aEnemy[nCntEnemy].nType][nCntParts].FormarDiffuse[nCntMat];
									}
								}
							}

							if (g_aEnemy[nCntEnemy].nAttackTiming == 40)
							{
								for (int nCntParts = 0; nCntParts < ENEMY_NUMPARTS[g_aEnemy[nCntEnemy].nType]; nCntParts++)
								{
									for (int nCntMat = 0; nCntMat < (int)(g_aEnemyParts[g_aEnemy[nCntEnemy].nType][nCntParts].dwNumMat); nCntMat++)
									{
										g_aEnemy[nCntEnemy].parts[g_aEnemy[nCntEnemy].nType][nCntParts].Diffuse[nCntMat] =
											g_aEnemy[nCntEnemy].parts[g_aEnemy[nCntEnemy].nType][nCntParts].FormarDiffuse[nCntMat];
									}
								}
							}
						}

						//=============================
						//攻撃開始
						//=============================
						if (g_aEnemy[nCntEnemy].nAttackTiming == 80)
						{
							PlaySound(SOUND_LABEL_SE_THUNDER_000);


							for (int nCntModel = 0; nCntModel < ENEMY004_NUM_ATTACK2; nCntModel++)
							{
								float fRot = (2.0f / (float)(ENEMY004_NUM_ATTACK2)) * (float)(nCntModel);
								SetAttackModel(ATTACKMODEL06_THUNDER, 180, D3DXVECTOR3(sinf(D3DX_PI * fRot) * 200.0f + g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, cosf(D3DX_PI * fRot) * 200.0f + g_aEnemy[nCntEnemy].pos.z), NULL_VECTOR3, NULL_VECTOR3, D3DXVECTOR3(1.5f, 0.5f, 0.5f), 0.0f, 0.0f, 0.05f, NULL_VECTOR3, ATTACKMODELBEHAVIOR01_ANOTHER);
							}
						}
					}
					else if (g_aEnemy[nCntEnemy].nLotteryAttack == 3)
					{//ラビットショット
						//==========================
						//３次元自機狙い処理
						//==========================
						g_aEnemy[nCntEnemy].fVXaim3D = g_aEnemy[nCntEnemy].pos.x - (g_aEnemy[nCntEnemy].pos.x + fPurposeoPosX);
						g_aEnemy[nCntEnemy].fVYaim3D = g_aEnemy[nCntEnemy].pos.y - (g_aEnemy[nCntEnemy].pos.y + fPurposeoPosY);
						g_aEnemy[nCntEnemy].fVZaim3D = g_aEnemy[nCntEnemy].pos.z - (g_aEnemy[nCntEnemy].pos.z + fPurposeoPosZ);

						D3DXVECTOR3 BulletMove = NULL_VECTOR3;

						g_aEnemy[nCntEnemy].fVLaim3D = sqrtf((g_aEnemy[nCntEnemy].fVXaim3D * g_aEnemy[nCntEnemy].fVXaim3D) + (g_aEnemy[nCntEnemy].fVYaim3D * g_aEnemy[nCntEnemy].fVYaim3D)
							+ (g_aEnemy[nCntEnemy].fVZaim3D * g_aEnemy[nCntEnemy].fVZaim3D));

						BulletMove.x = g_aEnemy[nCntEnemy].fVXaim3D / g_aEnemy[nCntEnemy].fVLaim3D * 5.0f;
						BulletMove.y = g_aEnemy[nCntEnemy].fVYaim3D / g_aEnemy[nCntEnemy].fVLaim3D * 5.0f;
						BulletMove.z = g_aEnemy[nCntEnemy].fVZaim3D / g_aEnemy[nCntEnemy].fVLaim3D * 5.0f;

						SetBullet(BULLETTYPE_ENEMY, 300, 10.0f, 10.0f, g_aEnemy[nCntEnemy].pos, BulletMove, NULL_VECTOR3, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), nCntEnemy, 0.0f);
					}
				}

				break;
			}

			//======================================================
			//移動状態だったら、移動させる
			//======================================================
			if (g_aEnemy[nCntEnemy].state == ENEMYSTATE_MOVE)
			{

				if (g_aEnemy[nCntEnemy].nType == 0 || g_aEnemy[nCntEnemy].nType == 3)
				{
					if (fLength < 60.0f)
					{//距離が15.0f以下なら、移動しない
						g_aEnemy[nCntEnemy].fSpeed = 0.0f;
					}
					else
					{
						g_aEnemy[nCntEnemy].fSpeed = g_aEnemy[nCntEnemy].fFormarSpeed;
					}
				}
				else if (g_aEnemy[nCntEnemy].nType == 1 || g_aEnemy[nCntEnemy].nType == 2)
				{
					if (fLength < 200.0f)
					{//距離が15.0f以下なら、移動しない
						g_aEnemy[nCntEnemy].fSpeed = 0.0f;
					}
					else
					{
						g_aEnemy[nCntEnemy].fSpeed = g_aEnemy[nCntEnemy].fFormarSpeed;
					}
				}

				//ノックバックをmoveで使いたいので、移動状態の時に、敵のスピード分、直接位置をずらす。
				g_aEnemy[nCntEnemy].pos.x += sinf(g_aEnemy[nCntEnemy].fVaim) * g_aEnemy[nCntEnemy].fSpeed;
				g_aEnemy[nCntEnemy].pos.z += cosf(g_aEnemy[nCntEnemy].fVaim) * g_aEnemy[nCntEnemy].fSpeed;
			}

			g_aEnemy[nCntEnemy].posOld = g_aEnemy[nCntEnemy].pos;

			g_aEnemy[nCntEnemy].move.x += (0.0f - g_aEnemy[nCntEnemy].move.x) * 0.1f;
			g_aEnemy[nCntEnemy].move.z += (0.0f - g_aEnemy[nCntEnemy].move.z) * 0.1f;

			g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move;

			//===========================================================================
			//モデルの上に乗っているかどうかの判定と、乗っているモデルの番号の取得
			//===========================================================================
			g_aEnemy[nCntEnemy].nNumLandingModel = -1;
			g_aEnemy[nCntEnemy].bIsLanding = bCollisionModel(&g_aEnemy[nCntEnemy].pos, &g_aEnemy[nCntEnemy].posOld, g_aEnemy[nCntEnemy].Size,
				g_aEnemy[nCntEnemy].nNumLandingModel);

			//========================================================
			//オプションの表示をするかどうか
			//========================================================

			if (fLength < g_aEnemy[nCntEnemy].fOptionDispLength)
			{
				g_aEnemy[nCntEnemy].bUseEnemyDisplay = true;
			}

			if (g_aEnemy[nCntEnemy].bUseEnemyDisplay == false)
			{
				pEnemyHP->bUse = false;//体力表示をしない

				if (g_aEnemy[nCntEnemy].nType != 4)
				{
					pEnemyWeakDisp->bUse = false;//弱点表示をしない
				}
			}
			else
			{
				pEnemyHP->bUse = true;//体力表示を開始

				if (g_aEnemy[nCntEnemy].nType != 4)
				{
					pEnemyWeakDisp->bUse = true;//弱点表示をしない
				}
			}

			//======================================
			// 体力が０以下の場合
			//======================================
			if (g_aEnemy[nCntEnemy].nHp <= 0)
			{
				Mission* pMission = GetMission();//ミッション情報を取得

				/*PlaySound(SOUND_LABEL_SE_DEFEATENEMY_000);*/

				pMission->aMission1.nDefeatEnemyCount++;//敵撃破時にカウントをプラス

				g_aEnemy[nCntEnemy].bUse = false;//体力０以下なので、倒れる
				AddScore(ENEMY_SCORE[g_aEnemy[nCntEnemy].nType]);//スコアの加算
			}

			//影の位置を更新
			SetPositionShadow(g_aEnemy[nCntEnemy].nIdxShadow,
				D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].Size.y / 2, g_aEnemy[nCntEnemy].pos.z));

			SetPositionRaderDisPlay(g_aEnemy[nCntEnemy].nIdxRaderDisplay, g_aEnemy[nCntEnemy].pos, 0.0f);

			//モーション処理
			EnemyMotion(nCntEnemy);

			//=======================================================
			//敵がフィールド範囲外に出たときの、位置の調整
			//=======================================================

			if (g_aEnemy[nCntEnemy].pos.x + (g_aEnemy[nCntEnemy].Size.x / 2.0f) > pField->fWidth || g_aEnemy[nCntEnemy].pos.x - (g_aEnemy[nCntEnemy].Size.x / 2.0f) < -pField->fWidth ||
				g_aEnemy[nCntEnemy].pos.z + (g_aEnemy[nCntEnemy].Size.z / 2.0f) > pField->fHeight || g_aEnemy[nCntEnemy].pos.z - (g_aEnemy[nCntEnemy].Size.z / 2.0f) < -pField->fHeight)
			{
				g_aEnemy[nCntEnemy].pos = g_aEnemy[nCntEnemy].posOld;
			}

			//======================================
			// 敵が使用されていない場合
			//======================================
			if (g_aEnemy[nCntEnemy].bUse == false)
			{
				pPlayer->nTotalDefeatEnemy++;//合計敵撃破数をプラス

				if (g_aEnemy[nCntEnemy].bSummonByBoss == true)
				{
					pBoss->nCntSummonEnemy--;
				}

				//敵の各パーツの設定
				for (int nCntParts = 0; nCntParts < ENEMY_NUMPARTS[g_aEnemy[nCntEnemy].nType]; nCntParts++)
				{
					g_aEnemy[nCntEnemy].parts[g_aEnemy[nCntEnemy].nType][nCntParts].bUse = false;//使用する
				}

				//============================
				// スクラップのドロップ処理
				//============================
				if (g_aEnemy[nCntEnemy].bDefeatDropScrap == true)
				{
					//プレイヤーのコンボ判定カウントが0ではない場合
					if (pPlayer->nCntCombo > 0)
					{
						//コンボ倍率を加算
						pPlayer->fCombo += 0.1f;

						//コンボ倍率が最大倍率を上回った場合
						if (pPlayer->fCombo >= MAX_COMBO)
						{
							//最大倍率に調整
							pPlayer->fCombo = MAX_COMBO;
						}
					}
					
					//プレイヤーのコンボカウントをリセット
					pPlayer->nCntCombo = COMBO_COUNT;

					for (int nCntDrop = 0; nCntDrop < 10; nCntDrop++)
					{
						float fRandumRot = float(rand() % 200 + 1) / 100;
						float fRandumSpeed = float(rand() % 100 - 50) / 10;

						switch (g_aEnemy[nCntEnemy].nType)
						{
						case 0:
							SetScrap(SCRAPTYPE00_TIBIROBO, GENERALLYSCRAPLIFE, SCRAPRADIUSSIZE, SCRAPRADIUSSIZE,
								D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y + SCRAPRADIUSSIZE, g_aEnemy[nCntEnemy].pos.z)
								, g_aEnemy[nCntEnemy].rot,
								D3DXVECTOR3(sinf(-D3DX_PI * fRandumRot) * fRandumSpeed, 0.0f, cosf(-D3DX_PI * fRandumRot) * fRandumSpeed));
							break;
						case 1:
							SetScrap(SCRAPTYPE03_SPEAKER, GENERALLYSCRAPLIFE, SCRAPRADIUSSIZE, SCRAPRADIUSSIZE,
								D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y + SCRAPRADIUSSIZE, g_aEnemy[nCntEnemy].pos.z)
								, g_aEnemy[nCntEnemy].rot,
								D3DXVECTOR3(sinf(-D3DX_PI * fRandumRot) * fRandumSpeed, 0.0f, cosf(-D3DX_PI * fRandumRot) * fRandumSpeed));
							break;
						case 2:
							SetScrap(SCRAPTYPE02_BOOMERANG, GENERALLYSCRAPLIFE, SCRAPRADIUSSIZE, SCRAPRADIUSSIZE,
								D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y + SCRAPRADIUSSIZE, g_aEnemy[nCntEnemy].pos.z)
								, g_aEnemy[nCntEnemy].rot,
								D3DXVECTOR3(sinf(-D3DX_PI * fRandumRot) * fRandumSpeed, 0.0f, cosf(-D3DX_PI * fRandumRot) * fRandumSpeed));
							break;
						case 3:
							SetScrap(SCRAPTYPE01_SWORD, GENERALLYSCRAPLIFE, SCRAPRADIUSSIZE, SCRAPRADIUSSIZE,
								D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y + SCRAPRADIUSSIZE, g_aEnemy[nCntEnemy].pos.z)
								, g_aEnemy[nCntEnemy].rot,
								D3DXVECTOR3(sinf(-D3DX_PI * fRandumRot) * fRandumSpeed, 0.0f, cosf(-D3DX_PI * fRandumRot) * fRandumSpeed));
							break;
						case 4:
							SetScrap(SCRAPTYPE04_ALL, GENERALLYSCRAPLIFE, SCRAPRADIUSSIZE, SCRAPRADIUSSIZE,
								D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y + SCRAPRADIUSSIZE, g_aEnemy[nCntEnemy].pos.z)
								, g_aEnemy[nCntEnemy].rot,
								D3DXVECTOR3(sinf(-D3DX_PI * fRandumRot) * fRandumSpeed, 0.0f, cosf(-D3DX_PI * fRandumRot) * fRandumSpeed));
							break;
						}
					}
				}

				KillShadow(g_aEnemy[nCntEnemy].nIdxShadow);//影の破棄
				KillRaderDisplay(g_aEnemy[nCntEnemy].nIdxRaderDisplay);
			}
		}
	}

	PrintDebugProc("最後に与えたダメージ量：%d\n", g_nLastDamageEnemy);
}

//================================
// 描画処理
//================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスへのポインタと取得
	D3DXMATRIX mtxRot, mtxTrans, mtxParent;//計算用マトリックス
	D3DMATERIAL9 matDef;//現在のマテリアル保存用
	D3DXMATERIAL* pMat;//マテリアルデータへのポインタ

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//使用している場合
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aEnemy[nCntEnemy].rot.y, g_aEnemy[nCntEnemy].rot.x, g_aEnemy[nCntEnemy].rot.z);
			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld, &g_aEnemy[nCntEnemy].mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, g_aEnemy[nCntEnemy].pos.z);
			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld, &g_aEnemy[nCntEnemy].mtxWorld, &mtxTrans);

			//種類保存
			int nType = g_aEnemy[nCntEnemy].nType;

			for (int nCntParts = 0; nCntParts < ENEMY_NUMPARTS[nType]; nCntParts++)
			{
				//各パーツモデルのワールドマトリックスの初期化
				D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].parts[nType][nCntParts].mtxWorld);

				//各パーツモデルの向きを反映
				D3DXMatrixRotationYawPitchRoll(&mtxRot, 
					g_aEnemy[nCntEnemy].parts[nType][nCntParts].rot.y,
					g_aEnemy[nCntEnemy].parts[nType][nCntParts].rot.x,
					g_aEnemy[nCntEnemy].parts[nType][nCntParts].rot.z);

				D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].parts[nType][nCntParts].mtxWorld, &g_aEnemy[nCntEnemy].parts[nType][nCntParts].mtxWorld, &mtxRot);

				//各パーツモデルの位置を反映
				D3DXMatrixTranslation(&mtxTrans, 
					g_aEnemy[nCntEnemy].parts[nType][nCntParts].pos.x, 
					g_aEnemy[nCntEnemy].parts[nType][nCntParts].pos.y, 
					g_aEnemy[nCntEnemy].parts[nType][nCntParts].pos.z);

				D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].parts[nType][nCntParts].mtxWorld, &g_aEnemy[nCntEnemy].parts[nType][nCntParts].mtxWorld, &mtxTrans);

				//親番号の保存
				int nParent = g_aEnemy[nCntEnemy].parts[nType][nCntParts].nIdxModelParent;

				//親のマトリックスを反映
				if (nParent != -1)
				{//親の番号を持つ場合
					mtxParent = g_aEnemy[nCntEnemy].parts[nType][nParent].mtxWorld;//親モデルのマトリックス
				}
				else
				{//親の番号を持たない場合
					mtxParent = g_aEnemy[nCntEnemy].mtxWorld;//プレイヤーのマトリックス
				}

				//親のマトリックスを掛け合わせる
				D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].parts[nType][nCntParts].mtxWorld, &g_aEnemy[nCntEnemy].parts[nType][nCntParts].mtxWorld, &mtxParent);

				//ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &g_aEnemy[nCntEnemy].parts[nType][nCntParts].mtxWorld);

				//現在のマテリアルを取得
				pDevice->GetMaterial(&matDef);

				//マテリアルデータへのポインタを取得
				pMat = (D3DXMATERIAL*)g_aEnemyParts[nType][nCntParts].pBuffMat->GetBufferPointer();

				for (int nCntMat = 0; nCntMat < (int)g_aEnemyParts[nType][nCntParts].dwNumMat; nCntMat++)
				{
					pMat[nCntMat].MatD3D.Diffuse = g_aEnemy[nCntEnemy].parts[nType][nCntParts].Diffuse[nCntMat];

					//マテリアルの設定
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

					//テクスチャの設定
					pDevice->SetTexture(0,g_aEnemyParts[g_aEnemy[nCntEnemy].nType][nCntParts].pTexture[nCntMat]);

					//モデル(パーツ)の描画
					g_aEnemyParts[nType][nCntParts].pMesh->DrawSubset(nCntMat);
				}

				//保存していたマテリアルを戻す
				pDevice->SetMaterial(&matDef);
			}
		}
	}
}

//================================
//設定処理
//================================
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, bool bSummonByBoss)//設定処理
{
	Field* pField = GetField();
	Camera* pCamera = GetCamera();
	Boss* pBoss = GetBoss();
	int nTimer = GetTimer();
	float fStatusBuff = 0.0f;

	//================================
	//敵のステータス倍率
	//================================
	for (int nCnt = 0; nCnt < 12; nCnt++)
	{
		if (nTimer >= 220 - (20 * nCnt) && nTimer <= MAX_TIME - (20 * nCnt))
		{
			fStatusBuff = 1.0f + (float)(nCnt) * 0.15f;//20秒ごとに15%ずつ敵のステータスの倍率を上げる
			break;
		}
	}

	if (pCamera->nMode == CAMERAMODE_REVOLUTION)
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
				g_aEnemy[nCntEnemy].fLength = 0.0f;//距離をとる
				g_aEnemy[nCntEnemy].bDefeatDropScrap = true;//倒されたときにスクラップをドロップするかどうか

				//==================================
				//敵のステータス設定
				//==================================
				g_aEnemy[nCntEnemy].nHp =(int)(ENEMY_HP[nType] * fStatusBuff);     //体力
				g_aEnemy[nCntEnemy].nMaxHp = g_aEnemy[nCntEnemy].nHp;               //敵の体力の最大値
				g_aEnemy[nCntEnemy].nAttack = ENEMY_ATTACK[nType];                  //攻撃力
				g_aEnemy[nCntEnemy].nDefeatScore = ENEMY_SCORE[nType];              //倒した時にもらえるスコア
				g_aEnemy[nCntEnemy].fSpeed = ENEMY_SPEED[nType] * fStatusBuff;      //速さ
				g_aEnemy[nCntEnemy].fFormarSpeed = ENEMY_SPEED[nType] * fStatusBuff;//元の速さ
				g_aEnemy[nCntEnemy].fMoveStartLength = ENEMY_MOVESTART[nType];//移動を開始する距離
				g_aEnemy[nCntEnemy].fAttackStartLength = ENEMY_ATTACKSTART[nType];//攻撃を開始する距離
				g_aEnemy[nCntEnemy].nAttackTiming = 0;//攻撃判定のタイミングを決める
				g_aEnemy[nCntEnemy].bHitFlag = false;//攻撃がヒットしたかどうかを決める変数
				g_aEnemy[nCntEnemy].bUnderAttack = false;//敵が攻撃中かどうかを決める
				g_aEnemy[nCntEnemy].nNumLandingModel = -1;//どのモデルの上に乗っているか

				if (g_aEnemy[nCntEnemy].pos.x > pField->fWidth)
				{
					g_aEnemy[nCntEnemy].pos.x = pField->fWidth;
				}
				if (g_aEnemy[nCntEnemy].pos.x < -pField->fWidth)
				{
					g_aEnemy[nCntEnemy].pos.x = -pField->fWidth;
				}
				if (g_aEnemy[nCntEnemy].pos.z > pField->fHeight)
				{
					g_aEnemy[nCntEnemy].pos.z = pField->fHeight;
				}
				if (g_aEnemy[nCntEnemy].pos.z < -pField->fHeight)
				{
					g_aEnemy[nCntEnemy].pos.z = -pField->fHeight;
				}

				for (int nCntAttackModel = 0; nCntAttackModel < MAX_ATTACKMODEL; nCntAttackModel++)
				{
					g_aEnemy[nCntEnemy].bHitStopBoomerang[nCntAttackModel] = false;//ブーメランに対するヒットストップ判定をする
					g_aEnemy[nCntEnemy].nCoolTimeBoomerang[nCntAttackModel] = 0;//ブーメランに対するヒットストップの時間をカウントする
				}

				//============================
				//攻撃受け付け調整
				//============================
				for (int nCntHitStop = 0; nCntHitStop < PLAYERATTACK_MAX; nCntHitStop++)
				{
					g_aEnemy[nCntEnemy].bHitStop[nCntHitStop] = false;//この変数がtrueの間、攻撃を受け付けない
					g_aEnemy[nCntEnemy].nCoolTime[nCntHitStop] = 0;   //bHitStopをfalseにするまでのカウント
				}

				//========================================
				//敵の表示オプションの表示の変数
				//========================================
				g_aEnemy[nCntEnemy].fOptionDispLength = ENEMYDISPLENGTH[nType];//敵のオプションの表示を開始する距離
				g_aEnemy[nCntEnemy].bUseEnemyDisplay = false;//敵のオプション表示を使用するかどうか

				//=================================
				//武器への耐性調整
				//=================================
				switch (nType)
				{
				case 0://レッドブル
					g_aEnemy[nCntEnemy].bWeakWeapon[0] = true;//剣（弱い）
					g_aEnemy[nCntEnemy].bWeakWeapon[1] = false;//ブーメラン
					g_aEnemy[nCntEnemy].bWeakWeapon[2] = false;//スピーカー
					break;
				case 1://ミルタンク
					g_aEnemy[nCntEnemy].bWeakWeapon[0] = false;//剣
					g_aEnemy[nCntEnemy].bWeakWeapon[1] = true;//ブーメラン（弱い）
					g_aEnemy[nCntEnemy].bWeakWeapon[2] = false;//スピーカー
					break;
				case 2://ミックチュジューチュ
					g_aEnemy[nCntEnemy].bWeakWeapon[0] = false;//剣
					g_aEnemy[nCntEnemy].bWeakWeapon[1] = true;//ブーメラン（弱い）
					g_aEnemy[nCntEnemy].bWeakWeapon[2] = false;//スピーカー
					break;
				case 3://モンスター
					g_aEnemy[nCntEnemy].bWeakWeapon[0] = true;//剣（弱い）
					g_aEnemy[nCntEnemy].bWeakWeapon[1] = false;//ブーメラン
					g_aEnemy[nCntEnemy].bWeakWeapon[2] = false;//スピーカー
					break;
				case 4://カップ麺
					g_aEnemy[nCntEnemy].bWeakWeapon[0] = false;//剣
					g_aEnemy[nCntEnemy].bWeakWeapon[1] = false;//ブーメラン
					g_aEnemy[nCntEnemy].bWeakWeapon[2] = true;//スピーカー
					break;

				default:
					break;
				}
				
				//===================================================
				//上でbUseがfalseにならなかったら、処理を実行
				//===================================================
				if (g_aEnemy[nCntEnemy].bUse == true)
				{
					g_aEnemy[nCntEnemy].nIdxShadow = SetShadow();//影の設定
					g_aEnemy[nCntEnemy].nIdxRaderDisplay = SetRaderDisplay(RADERDISPLAY01_ENEMY);//レーダーディスプレイのインデックス
					g_aEnemy[nCntEnemy].bSummonByBoss = bSummonByBoss;//ボスによって召喚されたかどうか

					if (g_aEnemy[nCntEnemy].bSummonByBoss == true)
					{
						pBoss->nCntSummonEnemy++;//ボスによって召喚された敵をカウントする
					}

					//HPゲージの設定
					SetEnemyGauge(nCntEnemy, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].vtxMax.y, g_aEnemy[nCntEnemy].pos.z));

					for (int nCntWeak = 0; nCntWeak < 3; nCntWeak++)
					{
						if (g_aEnemy[nCntEnemy].bWeakWeapon[nCntWeak] == true)
						{
							//弱点表記の設定
							SetEnemyWeakDisp(nCntWeak, nCntEnemy, NULL_VECTOR3);
						}
					}

					//敵の各パーツの設定
					for (int nCntParts = 0; nCntParts < ENEMY_NUMPARTS[nType]; nCntParts++)
					{
						g_aEnemy[nCntEnemy].parts[nType][nCntParts].bUse = true;//使用する
						g_aEnemy[nCntEnemy].parts[nType][nCntParts].pos = g_aEnemyParts[nType][nCntParts].pos;//位置
						g_aEnemy[nCntEnemy].parts[nType][nCntParts].PartsPos = NULL_VECTOR3;//パーツごとの位置
						g_aEnemy[nCntEnemy].parts[nType][nCntParts].rot = g_aEnemyParts[nType][nCntParts].rot;//向き
						g_aEnemy[nCntEnemy].parts[nType][nCntParts].nIdxModelParent = g_aEnemyParts[nType][nCntParts].nIdxModelParent;//親の番号
						//g_aEnemy[nCntEnemy].parts[nType][nCntParts].pBuffMat = g_aEnemyParts[nType][nCntParts].pBuffMat;//マテリアルへのポインタ
						//g_aEnemy[nCntEnemy].parts[nType][nCntParts].dwNumMat = g_aEnemyParts[nType][nCntParts].dwNumMat;//マテリアルの数
						//g_aEnemy[nCntEnemy].parts[nType][nCntParts].pMesh = g_aEnemyParts[nType][nCntParts].pMesh;//メッシュ（頂点情報）
					}

					//======================================
					//各種類のサイズを求める
					//======================================
					BYTE* pVtxBuff;	//頂点バッファへのポインタ

					for (int nCntParts = 0; nCntParts < ENEMY_NUMPARTS[nType]; nCntParts++)
					{
						//頂点数の取得
						int nNumVtx = g_aEnemyParts[nType][nCntParts].pMesh->GetNumVertices();

						//頂点フォーマットのサイズを取得
						DWORD sizeFVF = D3DXGetFVFVertexSize(g_aEnemyParts[nType][nCntParts].pMesh->GetFVF());

						//頂点バッファのロック
						g_aEnemyParts[nType][nCntParts].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

						for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
						{
							//頂点座標の代入
							D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

							//モデルの最小値と最大値を取得
							if (vtx.x < g_aEnemy[nCntEnemy].vtxMin.x)
							{//X座標の最小値
								g_aEnemy[nCntEnemy].vtxMin.x = vtx.x;
							}
							if (vtx.y < g_aEnemy[nCntEnemy].vtxMin.y)
							{//Y座標の最小値
								g_aEnemy[nCntEnemy].vtxMin.y = vtx.y;
							}
							if (vtx.z < g_aEnemy[nCntEnemy].vtxMin.z)
							{//Z座標の最小値
								g_aEnemy[nCntEnemy].vtxMin.z = vtx.z;
							}
							if (vtx.x > g_aEnemy[nCntEnemy].vtxMax.x)
							{//X座標の最大値
								g_aEnemy[nCntEnemy].vtxMax.x = vtx.x;
							}
							if (vtx.y > g_aEnemy[nCntEnemy].vtxMax.y)
							{//Y座標の最大値
								g_aEnemy[nCntEnemy].vtxMax.y = vtx.y;
							}
							if (vtx.z > g_aEnemy[nCntEnemy].vtxMax.z)
							{//Z座標の最大値
								g_aEnemy[nCntEnemy].vtxMax.z = vtx.z;
							}

							//頂点フォーマットのサイズ分ポインタを進める
							pVtxBuff += sizeFVF;
						}

						//敵の大きさを決める。（椎名オリジナル処理)
						g_aEnemy[nCntEnemy].Size = g_aEnemy[nCntEnemy].vtxMax - g_aEnemy[nCntEnemy].vtxMin;

						//頂点バッファのアンロック
						g_aEnemyParts[nType][nCntParts].pMesh->UnlockVertexBuffer();
					}
				}

				break;
			}
		}
	}
}

//=======================================================================================================================================================================================================
// 敵のパーツ情報の読み込み
//=======================================================================================================================================================================================================
void LoadEnemy(void)
{
	char aDataSearch[MAX_TEXT];
	memset(&aDataSearch[0], NULL, sizeof(aDataSearch));

	for (int nCntEnemy = 0; nCntEnemy < NUM_ENEMY; nCntEnemy++)
	{
		int nCntParts = 0;

		//ファイルを開く
		FILE* pFile = fopen(ENEMY_FILE[nCntEnemy], "r");

		//ファイルが開けない場合
		if (pFile == NULL)
		{
			return;
		}

		//ENDが見つかるまで読み込みを繰り返す
		while (1)
		{
			fscanf(pFile, "%s", aDataSearch);	//検索

			//END_PARTSが見つかった場合
			if (!strcmp(aDataSearch, "END_PARTS"))
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
			if (!strcmp(aDataSearch, "PARTSSET"))
			{
				//項目ごとのデータを代入
				while (1)
				{
					fscanf(pFile, "%s", aDataSearch);	//検索

					//ENDMODELセットが見つかった場合
					if (!strcmp(aDataSearch, "END_PARTSSET"))
					{//モデルデータの読み込みを終了
						break;
					}
					//データの読み込み
					else if (!strcmp(aDataSearch, "POSx"))
					{//位置のX座標
						fscanf(pFile, "%f", &g_aEnemyParts[nCntEnemy][nCntParts].pos.x);
					}
					else if (!strcmp(aDataSearch, "POSy"))
					{//位置のY座標
						fscanf(pFile, "%f", &g_aEnemyParts[nCntEnemy][nCntParts].pos.y);
					}
					else if (!strcmp(aDataSearch, "POSz"))
					{//位置のZ座標
						fscanf(pFile, "%f", &g_aEnemyParts[nCntEnemy][nCntParts].pos.z);
					}
					else if (!strcmp(aDataSearch, "ROTx"))
					{//向きのX座標
						fscanf(pFile, "%f", &g_aEnemyParts[nCntEnemy][nCntParts].rot.x);
					}
					else if (!strcmp(aDataSearch, "ROTy"))
					{//向きのY座標
						fscanf(pFile, "%f", &g_aEnemyParts[nCntEnemy][nCntParts].rot.y);
					}
					else if (!strcmp(aDataSearch, "ROTz"))
					{//向きのZ座標
						fscanf(pFile, "%f", &g_aEnemyParts[nCntEnemy][nCntParts].rot.z);
					}
					else if (!strcmp(aDataSearch, "TYPE"))
					{//種類
						fscanf(pFile, "%d", &g_aEnemyParts[nCntEnemy][nCntParts].nType);
					}
					else if (!strcmp(aDataSearch, "PARENT"))
					{//親の番号
						fscanf(pFile, "%d", &g_aEnemyParts[nCntEnemy][nCntParts].nIdxModelParent);
					}
				}

				nCntParts++;//パーツ数を加算
			}
		}
	}
}

//============================================================================================================================================================================================================
// モーション情報の読み込み処理
//============================================================================================================================================================================================================
void LoadEnemyMotion(int nType)
{
	int nMotion = 0;						//モーションの種類番号
	int nCntParts = 0;						//パーツ数
	int nCntKey[NUM_ENEMY][MAX_ENEMYMOTION];//各モーションキー数
	char aDataSearch[MAX_TEXT];				//データ検索用

	//各モーションキー数の初期化
	for (int nCnt = 0; nCnt < MAX_ENEMYMOTION; nCnt++)
	{
		nCntKey[nType][nCnt] = 0;
	}

	//ファイルを開く
	FILE* pFile = fopen(ENEMY_FILE[nType], "r");

	//ファイルが開けない場合
	if (pFile == NULL)
	{
		return;
	}

	while (1)
	{
		int nResult = fscanf(pFile, "%s", aDataSearch);	//検索

		//モーションセットが見つかった場合
		if (!strcmp(aDataSearch, "MOTIONSET"))
		{
			//モーションの種類を読み込む
			fscanf(pFile, "%d", &nMotion);

			//END_MOTIONSETが見つかるまで読み込みを繰り返す
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch);	//検索

				//END_MOTIONSETが見つかった場合
				if (!strcmp(aDataSearch, "END_MOTIONSET"))
				{//モーション読み込み終了
					break;
				}

				//コメントアウトが見つかった場合
				if (aDataSearch[0] == '#')
				{//スキップして読み込みを続ける
					continue;
				}

				//ループ判定が見つかった場合
				if (!strcmp(aDataSearch, "LOOP"))
				{//ループするかどうかを取得
					fscanf(pFile, "%d", &g_aEnemyMotionSet[nType][nMotion].nMotionLoop);
				}

				//キーセットが見つかった場合
				if (!strcmp(aDataSearch, "KEYSET"))
				{
					while (1)
					{
						fscanf(pFile, "%s", aDataSearch);	//検索

						//コメントアウトが見つかった場合
						if (aDataSearch[0] == '#')
						{//スキップして読み込みを続ける
							continue;
						}

						//KEYが見つかった場合
						if (!strcmp(aDataSearch, "KEY"))
						{
							//項目ごとのデータを代入
							while (1)
							{
								fscanf(pFile, "%s", aDataSearch);	//検索

								//FRAMEが見つかった場合
								if (!strcmp(aDataSearch, "FRAME"))
								{//フレーム数を読み込む
									fscanf(pFile, "%d", &g_aEnemyKeySet[nType][nCntKey[nType][nMotion]][nMotion].nMaxFrame);
								}

								//データの読み込み
								else if (!strcmp(aDataSearch, "POSx"))
								{//位置のX座標
									fscanf(pFile, "%f", &g_aEnemyKeySet[nType][nCntKey[nType][nMotion]][nMotion].key[nCntParts].pos.x);
								}
								else if (!strcmp(aDataSearch, "POSy"))
								{//位置のY座標
									fscanf(pFile, "%f", &g_aEnemyKeySet[nType][nCntKey[nType][nMotion]][nMotion].key[nCntParts].pos.y);
								}
								else if (!strcmp(aDataSearch, "POSz"))
								{//位置のZ座標
									fscanf(pFile, "%f", &g_aEnemyKeySet[nType][nCntKey[nType][nMotion]][nMotion].key[nCntParts].pos.z);
								}
								else if (!strcmp(aDataSearch, "ROTx"))
								{//向きのX座標
									fscanf(pFile, "%f", &g_aEnemyKeySet[nType][nCntKey[nType][nMotion]][nMotion].key[nCntParts].rot.x);
								}
								else if (!strcmp(aDataSearch, "ROTy"))
								{//向きのY座標
									fscanf(pFile, "%f", &g_aEnemyKeySet[nType][nCntKey[nType][nMotion]][nMotion].key[nCntParts].rot.y);
								}
								else if (!strcmp(aDataSearch, "ROTz"))
								{//向きのZ座標
									fscanf(pFile, "%f", &g_aEnemyKeySet[nType][nCntKey[nType][nMotion]][nMotion].key[nCntParts].rot.z);
								}

								//END_KEYセットが見つかった場合
								else if (!strcmp(aDataSearch, "END_KEY"))
								{//モデルデータの読み込みを終了
									nCntParts++;	//データ数を加算
									break;
								}
							}
						}

						//END_KEYSETが見つかった場合
						if (!strcmp(aDataSearch, "END_KEYSET"))
						{
							nCntParts = 0;
							nCntKey[nType][nMotion]++;//キー数を加算
							g_aEnemyMotionSet[nType][nMotion].nMaxEnemyKey = nCntKey[nType][nMotion];//合計キー数を保存
							break;
						}
					}
				}
			}
		}

		//EOFが見つかった場合
		else if (nResult == EOF)
		{//ファイルを閉じる
			fclose(pFile);
			break;
		}

		//モーションセット以外が見つかった場合
		else
		{//検索を続ける
			continue;
		}
	}
}

//============================================================================================================================================================================================================
// モーション処理
//============================================================================================================================================================================================================
void EnemyMotion(int nCnt)
{
	//g_aEnemy[nCnt].state = ENEMYSTATE_ATTACK;

	int nType = g_aEnemy[nCnt].nType;		//敵の種類を保存
	int nState = (int)(g_aEnemy[nCnt].state);	//敵の状態を保存
	int nCntKey = g_aEnemy[nCnt].motionset[nType][nState].nCntEnemyKey;															//現在のキーを保存
	int nNextKey = (nCntKey + 1) % g_aEnemyMotionSet[nType][nState].nMaxEnemyKey;												//次のキーを求める
	float fFrame = (float)g_aEnemy[nCnt].motionset[nType][nState].nCntFrame / g_aEnemyKeySet[nType][nCntKey][nState].nMaxFrame;	//現在のフレームと全体フレームの割合

	//パーツごとに情報を更新
	for (int nCntParts = 0; nCntParts < ENEMY_NUMPARTS[nType]; nCntParts++)
	{
		float EnemyPosX = g_aEnemyKeySet[nType][nCntKey][nState].key[nCntParts].pos.x;
		float EnemyPosY = g_aEnemyKeySet[nType][nCntKey][nState].key[nCntParts].pos.y;
		float EnemyPosZ = g_aEnemyKeySet[nType][nCntKey][nState].key[nCntParts].pos.z;
		float EnemyRotX = g_aEnemyKeySet[nType][nCntKey][nState].key[nCntParts].rot.x;
		float EnemyRotY = g_aEnemyKeySet[nType][nCntKey][nState].key[nCntParts].rot.y;
		float EnemyRotZ = g_aEnemyKeySet[nType][nCntKey][nState].key[nCntParts].rot.z;

		float NextEnemyPosX = g_aEnemyKeySet[nType][nNextKey][nState].key[nCntParts].pos.x;
		float NextEnemyPosY = g_aEnemyKeySet[nType][nNextKey][nState].key[nCntParts].pos.y;
		float NextEnemyPosZ = g_aEnemyKeySet[nType][nNextKey][nState].key[nCntParts].pos.z;
		float NextEnemyRotX = g_aEnemyKeySet[nType][nNextKey][nState].key[nCntParts].rot.x;
		float NextEnemyRotY = g_aEnemyKeySet[nType][nNextKey][nState].key[nCntParts].rot.y;
		float NextEnemyRotZ = g_aEnemyKeySet[nType][nNextKey][nState].key[nCntParts].rot.z;

		//次のキーまでの差分を求める
		float fPosx = NextEnemyPosX - EnemyPosX;	// X座標
		float fPosy = NextEnemyPosY - EnemyPosY;	// Y座標
		float fPosz = NextEnemyPosZ - EnemyPosZ;	// Z座標
		float fRotx = NextEnemyRotX - EnemyRotX;	// X軸の向き
		float fRoty = NextEnemyRotY - EnemyRotY;	// Y軸の向き
		float fRotz = NextEnemyRotZ - EnemyRotZ;	// Z軸の向き

		//向きを更新
		g_aEnemy[nCnt].parts[nType][nCntParts].rot.x = g_aEnemyParts[nType][nCntParts].rot.x + EnemyRotX + fRotx * fFrame;//X軸
		g_aEnemy[nCnt].parts[nType][nCntParts].rot.y = g_aEnemyParts[nType][nCntParts].rot.y + EnemyRotY + fRoty * fFrame;//Y軸
		g_aEnemy[nCnt].parts[nType][nCntParts].rot.z = g_aEnemyParts[nType][nCntParts].rot.z + EnemyRotZ + fRotz * fFrame;//Z軸

		//位置を更新
		g_aEnemy[nCnt].parts[nType][nCntParts].pos.x = g_aEnemyParts[nType][nCntParts].pos.x + EnemyPosX + fPosx * fFrame;//X座標
		g_aEnemy[nCnt].parts[nType][nCntParts].pos.y = g_aEnemyParts[nType][nCntParts].pos.y + EnemyPosY + fPosy * fFrame;//Y座標
		g_aEnemy[nCnt].parts[nType][nCntParts].pos.z = g_aEnemyParts[nType][nCntParts].pos.z + EnemyPosZ + fPosz * fFrame;//Z座標
	}

	//キー数の更新
	if (g_aEnemy[nCnt].motionset[nType][nState].nCntFrame == 0 || g_aEnemy[nCnt].motionset[nType][nState].nCntFrame >= g_aEnemyKeySet[nType][nCntKey][nState].nMaxFrame)
	{
		if (g_aEnemy[nCnt].motionset[nType][nState].nCntFrame >= g_aEnemyKeySet[nType][nCntKey][nState].nMaxFrame)
		{
			//フレーム数カウンタをリセット
			g_aEnemy[nCnt].motionset[nType][nState].nCntFrame = 0;
		}

		//キー数を加算
		g_aEnemy[nCnt].motionset[nType][nState].nCntEnemyKey++;

		//キー数が最大数になった
		if (g_aEnemy[nCnt].motionset[nType][nState].nCntEnemyKey >= g_aEnemyMotionSet[nType][nState].nMaxEnemyKey)
		{
			//ループしない場合
			if (g_aEnemyMotionSet[nType][nState].nMotionLoop == 1)
			{
				g_aEnemy[nCnt].nAttackTiming = 0;//攻撃が終わった時も含まれるので、攻撃カウントを０にする
				g_aEnemy[nCnt].bHitFlag = false;//攻撃が一周したので、ヒット判定をリセット
				g_aEnemy[nCnt].bUnderAttack = false;//攻撃が一周したので、攻撃中の判定をリセット
				g_aEnemy[nCnt].bCompulsionAttack = false;//強制的に攻撃状態にする
				//ニュートラル状態
				g_aEnemy[nCnt].state = ENEMYSTATE_NEUTRAL;
			}
			
			//キー数を初期化
			g_aEnemy[nCnt].motionset[nType][nState].nCntEnemyKey = 0;
		}
	}

	//フレーム数更新
	g_aEnemy[nCnt].motionset[nType][nState].nCntFrame++;
}

//======================================
// 構造体の情報を取得
//======================================
Enemy* GetEnemy(void)
{
	return &g_aEnemy[0];
}

//======================================
// ダメージ処理
//======================================
void SetDamageEnemy(int nDamage, int nNumEnemy, PLAYERATTACK PlayerAttackNum, int nNumAttackModel)
{
	Player* pPlayer = GetPlayer();
	float fDamageResult = (float)(nDamage);
	bool bCritical = false;

	if (g_aEnemy[nNumEnemy].bWeakWeapon[PlayerAttackNum] == true)
	{
		fDamageResult = (float)(nDamage) * 2.5f;
		bCritical = true;

		PlaySound(SOUND_LABEL_SE_CRITICAL_000);
	}

	if (g_aEnemy[nNumEnemy].bHitStop[PlayerAttackNum] == false)//ヒットストップ状態じゃなければ
	{
		if (PlayerAttackNum == PLAYERATTACK00_SWORD || PlayerAttackNum == PLAYERATTACK02_SPEAKER)
		{
			if (bCritical == false)
			{
				g_aEnemy[nNumEnemy].move.x += sinf(pPlayer->rot.y + D3DX_PI) * 10.0f;
				g_aEnemy[nNumEnemy].move.z += cosf(pPlayer->rot.y + D3DX_PI) * 10.0f;
			}

			g_aEnemy[nNumEnemy].nHp -= (int)(fDamageResult);//体力を減らす
			g_aEnemy[nNumEnemy].bHitStop[PlayerAttackNum] = true;  //ダメージを与えた攻撃に対してヒットストップ状態にする

			for (int nCntParticle = 0; nCntParticle < 100; nCntParticle++)
			{
				float fRot = float(rand() % 200 + 1) / 50;
				float fYmove = float(rand() % 200 - 100) / 10;
				float fSpeed = float(rand() % 200 - 100) / 10;

				SetParticle(PARTICLETYPE_NORMAL, 300, 10.0f, 0.6f, g_aEnemy[nNumEnemy].pos,
					D3DXVECTOR3(sinf(-D3DX_PI * fRot) * fSpeed, fYmove, cosf(-D3DX_PI * fRot) * fSpeed),
					D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 0.0f, PARTICLETEX_NORMAL,0);
			}

			//クリティカルなら
			if (bCritical == true)
			{
				for (int nCntParticle = 0; nCntParticle < 50; nCntParticle++)
				{
					float fRot = float(rand() % 200 + 1) / 100;
					float fYmove = float(rand() % 200 - 100) / 10;
					float fSpeed = float(rand() % 200 - 100) / 10;

					SetParticle(PARTICLETYPE_NORMAL, 300, 10.0f, 0.9f, g_aEnemy[nNumEnemy].pos,
						D3DXVECTOR3(sinf(-D3DX_PI * fRot) * (fSpeed * 2.0f), fYmove, cosf(-D3DX_PI * fRot) * (fSpeed * 2.0f)),
						D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 0.0f, PARTICLETEX_NORMAL, 0);
				}

				if (PlayerAttackNum == PLAYERATTACK00_SWORD)
				{
					g_aEnemy[nNumEnemy].move.x += sinf(pPlayer->rot.y + D3DX_PI) * 20.0f;
					g_aEnemy[nNumEnemy].move.z += cosf(pPlayer->rot.y + D3DX_PI) * 20.0f;
				}
				else
				{
					g_aEnemy[nNumEnemy].move.x += sinf(pPlayer->rot.y + D3DX_PI) * 3.0f;
					g_aEnemy[nNumEnemy].move.z += cosf(pPlayer->rot.y + D3DX_PI) * 3.0f;

				}
			}

			//========================
			//効果音処理
			//========================
			if (PlayerAttackNum == PLAYERATTACK00_SWORD)
			{
				PlaySound(SOUND_LABEL_SE_SWORDSLASH_000);
			}
			if (PlayerAttackNum == PLAYERATTACK02_SPEAKER)
			{
				PlaySound(SOUND_LABEL_SE_SPEAKERWAVEHIT_000);
			}
		}
	}
	if (g_aEnemy[nNumEnemy].bHitStopBoomerang[nNumAttackModel] == false && PlayerAttackNum == PLAYERATTACK01_BOOMERANG)
	{
	    g_aEnemy[nNumEnemy].nHp -= (int)(fDamageResult);//体力を減らす
	    g_aEnemy[nNumEnemy].bHitStopBoomerang[nNumAttackModel] = true;  //ダメージを与えた攻撃に対してヒットストップ状態にする
	    
	    for (int nCntParticle = 0; nCntParticle < 50; nCntParticle++)
	    {
	    	float fRot = float(rand() % 200 + 1) / 100;
	    	float fYmove = float(rand() % 200 - 100) / 10;
	    	float fSpeed = float(rand() % 200 - 100) / 10;
	    
	    	SetParticle(PARTICLETYPE_NORMAL, 300, 10.0f, 0.6f, g_aEnemy[nNumEnemy].pos,
	    		D3DXVECTOR3(sinf(-D3DX_PI * fRot) * fSpeed, fYmove, cosf(-D3DX_PI * fRot) * fSpeed),
	    		D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 0.0f, PARTICLETEX_NORMAL,0);
	    }

		
		//クリティカルなら
		if (bCritical == true)
		{
			for (int nCntParticle = 0; nCntParticle < 50; nCntParticle++)
			{
				float fRot = float(rand() % 200 + 1) / 100;
				float fYmove = float(rand() % 200 - 100) / 10;
				float fSpeed = float(rand() % 200 - 100) / 10;

				SetParticle(PARTICLETYPE_NORMAL, 300, 10.0f, 0.6f, g_aEnemy[nNumEnemy].pos,
					D3DXVECTOR3(sinf(-D3DX_PI * fRot) * (fSpeed * 2.0f), fYmove, cosf(-D3DX_PI * fRot) * (fSpeed * 2.0f)),
					D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 0.0f, PARTICLETEX_NORMAL, 0);
			}
		}
	    	PlaySound(SOUND_LABEL_SE_HITBOOMERANG_000);
	}

	g_nLastDamageEnemy = (int)(fDamageResult);
}

//======================================
// キル処理
//======================================
void KillEnemy(void)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			g_aEnemy[nCntEnemy].bUse = false;
			KillShadow(g_aEnemy[nCntEnemy].nIdxShadow);
		}
	}
}

//=======================================
//敵のマトリックスを計算する処理
//=======================================
void CalculateMatrixEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスへのポインタと取得
	D3DXMATRIX mtxRot, mtxTrans, mtxParent;//計算用マトリックス
	D3DMATERIAL9 matDef;//現在のマテリアル保存用
	//D3DXMATERIAL* pMat;//マテリアルデータへのポインタ

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//使用している場合
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aEnemy[nCntEnemy].rot.y, g_aEnemy[nCntEnemy].rot.x, g_aEnemy[nCntEnemy].rot.z);
			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld, &g_aEnemy[nCntEnemy].mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, g_aEnemy[nCntEnemy].pos.z);
			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld, &g_aEnemy[nCntEnemy].mtxWorld, &mtxTrans);

			//種類保存
			int nType = g_aEnemy[nCntEnemy].nType;

			for (int nCntParts = 0; nCntParts < ENEMY_NUMPARTS[nType]; nCntParts++)
			{
				//各パーツモデルのワールドマトリックスの初期化
				D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].parts[nType][nCntParts].mtxWorld);

				//各パーツモデルの向きを反映
				D3DXMatrixRotationYawPitchRoll(&mtxRot,
					g_aEnemy[nCntEnemy].parts[nType][nCntParts].rot.y,
					g_aEnemy[nCntEnemy].parts[nType][nCntParts].rot.x,
					g_aEnemy[nCntEnemy].parts[nType][nCntParts].rot.z);

				D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].parts[nType][nCntParts].mtxWorld, &g_aEnemy[nCntEnemy].parts[nType][nCntParts].mtxWorld, &mtxRot);

				//各パーツモデルの位置を反映
				D3DXMatrixTranslation(&mtxTrans,
					g_aEnemy[nCntEnemy].parts[nType][nCntParts].pos.x,
					g_aEnemy[nCntEnemy].parts[nType][nCntParts].pos.y,
					g_aEnemy[nCntEnemy].parts[nType][nCntParts].pos.z);

				D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].parts[nType][nCntParts].mtxWorld, &g_aEnemy[nCntEnemy].parts[nType][nCntParts].mtxWorld, &mtxTrans);

				//親番号の保存
				int nParent = g_aEnemy[nCntEnemy].parts[nType][nCntParts].nIdxModelParent;

				//親のマトリックスを反映
				if (nParent != -1)
				{//親の番号を持つ場合
					mtxParent = g_aEnemy[nCntEnemy].parts[nType][nParent].mtxWorld;//親モデルのマトリックス
				}
				else
				{//親の番号を持たない場合
					mtxParent = g_aEnemy[nCntEnemy].mtxWorld;//プレイヤーのマトリックス
				}

				//親のマトリックスを掛け合わせる
				D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].parts[nType][nCntParts].mtxWorld, &g_aEnemy[nCntEnemy].parts[nType][nCntParts].mtxWorld, &mtxParent);

				//ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &g_aEnemy[nCntEnemy].parts[nType][nCntParts].mtxWorld);

				D3DXVECTOR3 posZero = D3DXVECTOR3(0.0f,0.0f,0.0f);

				D3DXVec3TransformCoord(&g_aEnemy[nCntEnemy].parts[nType][nCntParts].PartsPos, &posZero,&g_aEnemy[nCntEnemy].parts[nType][nCntParts].mtxWorld);
			}
		}
	}
}
