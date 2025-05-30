//=======================================================================================================================================================================================================
//
// ボスの処理
// Author : Shina Taiyo
//
//=======================================================================================================================================================================================================
#include "boss.h"
#include <stdio.h>
#include "bossgauge.h"
#include "sound.h"
#include "model.h"
#include "debugproc.h"
#include "attackmodel.h"
#include "bullet.h"
#include "battery.h"
#include "particle.h"
#include"attackbillboard.h"
#include "timer.h"
#include "mission.h"
#include "field.h"
#include "shadow.h"
#include "camera.h"
#include "Rader.h"
#include "emitter.h"
#include "enemy.h"
#include "Warning.h"
//=======================================================================================================================================================================================================
// グローバル変数
//=======================================================================================================================================================================================================
BossKey g_aBossKey[MAX_BOSSPARTS];							//各キー情報
BossKeySet g_aBossKeySet[MAX_BOSSKEYSET][MAX_BOSSMOTION];	//各キーセット情報
BossMotionSet g_aBossMotionSet[MAX_BOSSMOTION];				//モーション情報
BossParts g_aBossParts[MAX_BOSSPARTS];						//各パーツ情報
Boss g_aBoss;												//構造体の情報
int g_nldxShadowBoss = -1;									//対象の影のインデックス（番号）
int g_nDelayBoss;     
bool g_bPurposeFlag[MAX_PURPOSEFLAG];						//汎用フラグ
int g_nPurposeCnt;//汎用カウント

#define GRASPBOSS_SIZE (100.0f)
LPDIRECT3DTEXTURE9 g_pTextureGraspBoss = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGraspBoss = NULL;
GraspBoss g_aGraspBoss;


//=======================================================================================================================================================================================================
// 初期化処理
//=======================================================================================================================================================================================================
void InitBoss(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	D3DXMATERIAL* pMat;							//マテリアルのデータへのポインタ

	//構造体の情報
	g_aBoss.bUse = false;//使用しているかどうか
	g_aBoss.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//位置
	g_aBoss.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//1F前の位置
	g_aBoss.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//移動量
	g_aBoss.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//向き
	g_aBoss.nIdxShadow = -1;
	g_aBoss.vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//それぞれの座標のモデルの位置の最小値
	g_aBoss.vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//それぞれの座標のモデルの位置の最大値
	g_aBoss.Size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//敵の大きさ
	g_aBoss.bIsLanding = false;//地面についているかどうか
	g_aBoss.fVaimY1 = 0.0f;//総合したY方向のベクトル
	g_aBoss.fVXaim = 0.0f;
	g_aBoss.fVYaim = 0.0f;
	g_aBoss.fVZaim = 0.0f;
	g_aBoss.fVaim = 0.0f;
	g_aBoss.fLength = 0.0f;//距離をとる
	g_aBoss.nAttackTiming = 0;//攻撃判定のタイミングを決める
	g_aBoss.bHitFlag = false;//攻撃がヒットしたかどうかを決める変数
	g_aBoss.fMoveStartLength = 0.0f;//敵が移動を開始する距離
	g_aBoss.fAttackStartLength = 0.0f;//敵が攻撃を開始する距離
	g_aBoss.bUnderAttack = false;//敵が攻撃中かどうかを決める
	g_aBoss.state = BOSSSTATE_NEUTRAL;//ボスの状態
	g_aBoss.nCntAttackTime = 0;//攻撃をしている時間を測る
	g_aBoss.nLotteryAttack = 0;//どの攻撃をするか抽選をする
	g_aBoss.nPattern = 0;//パターン
	g_aBoss.nAttackCoolTime = 0;//攻撃のクールタイム	
	g_aBoss.fSaveAim = 0.0f;//ベクトルをセーブする
	g_aBoss.bBossSummon = false;//ボス召喚フラグ
	g_aBoss.fRotMove = 0.0f;//一部の処理でこの変数の値の方向に移動させる
	g_aBoss.nNumLandingModel = 0;//どのモデルの上に乗っているか
	g_aBoss.nIdxRaderDisplay = -1;//レーダーディスプレイのインデックス
	g_aBoss.nHp = 0;//体力
	g_aBoss.nCntAppearTime = 0;//出現してからの時間
	g_aBoss.bBossAppear = false;//ボスが出現演出中かどうか
	g_aBoss.bCurveLaser = false;//レーザーをカーブさせる向きを決める
	g_aBoss.nRunAwayLottery = 0;//低確率でボスを逃げさせる乱数
	g_aBoss.bRunAwayBoss = false;//フラグ発動時にボスを逃げさせるフラグ
	g_aBoss.nCntSummonEnemy = 0;//ボスが召喚した敵の数をカウントする
	g_aBoss.nTotalDamage = 0;//与えたダメージ合計
	g_aBoss.nDamageCnt = 0;
	g_aBoss.nCntUseAttack = 0;//攻撃をした回数をカウントする
	//==================================
	//３次元ベクトルで使う変数
	//==================================
	g_aBoss.fVXaim3D = 0.0f;
	g_aBoss.fVYaim3D = 0.0f;
	g_aBoss.fVZaim3D = 0.0f;
	g_aBoss.fVLaim3D = 0.0f;
	g_aBoss.SaveMove3D = NULL_VECTOR3;
	g_nPurposeCnt = 0;//汎用カウント

	for (int nCntHit = 0; nCntHit < PLAYERATTACK_MAX; nCntHit++)
	{
		g_aBoss.bHitStop[nCntHit] = false;
		g_aBoss.nCoolTime[nCntHit] = 0;
	}

	for (int nCntAttackModel = 0; nCntAttackModel < MAX_ATTACKMODEL; nCntAttackModel++)
	{
		g_aBoss.bHitStopBoomerang[nCntAttackModel] = false;//ブーメランに対するヒットストップの判定をする
		g_aBoss.nCoolTimeBoomerang[nCntAttackModel] = 0;//ブーメランに対するヒットストップのカウントをする
	}

	for (int nCntFlag = 0; nCntFlag < MAX_PURPOSEFLAG; nCntFlag++)
	{
		g_bPurposeFlag[nCntFlag] = false;
	}

	g_nDelayBoss = 0;//ディレイ

	//パーツ情報の読み込み
	LoadBossParts();

	//モーション情報の読み込み
	LoadBossMotion();

	//各パーツモデル情報
	for (int nCntParts = 0; nCntParts < g_aBoss.nNumParts; nCntParts++)
	{
		g_aBoss.aParts[nCntParts].vtxMax = NULL_VECTOR3;
		g_aBoss.aParts[nCntParts].vtxMin = NULL_VECTOR3;
		g_aBoss.aParts[nCntParts].PartsPos = NULL_VECTOR3;
		//Xファイルの読み込み
		D3DXLoadMeshFromX(BOSSPARTS_FILENAME[nCntParts], D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_aBoss.aParts[nCntParts].pBuffMat, NULL, &g_aBoss.aParts[nCntParts].dwNumMat, &g_aBoss.aParts[nCntParts].pMesh);

		//マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_aBoss.aParts[nCntParts].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_aBoss.aParts[nCntParts].dwNumMat; nCntMat++)
		{
			g_aBoss.aParts[nCntParts].Diffuse[nCntMat] = pMat[nCntMat].MatD3D.Diffuse;
		}
	}
}

//=====================================
//ボス位置把握表示の更新
//=====================================
void InitGraspBoss(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	pDevice = GetDevice();

	VERTEX_2D* pVtx;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\GraspBoss_000.png",
		&g_pTextureGraspBoss);


	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffGraspBoss,
		NULL);

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffGraspBoss->Lock(0, 0, (void**)&pVtx, 0);

	g_aGraspBoss.pos = NULL_VECTOR3;
	g_aGraspBoss.rot = NULL_VECTOR3;
	g_aGraspBoss.bUse = false;
	g_aGraspBoss.fAnimationPattern = 0.0f;
	g_aGraspBoss.fAnimationCnt = 0.0f;

	pVtx[0].pos = NULL_VECTOR3;
	pVtx[1].pos = NULL_VECTOR3;
	pVtx[2].pos = NULL_VECTOR3;
	pVtx[3].pos = NULL_VECTOR3;

	//rhwの設定（初期設定）
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定（初期設定）
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定（初期設定）
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffGraspBoss->Unlock();

}

//=======================================================================================================================================================================================================
// 終了処理
//=======================================================================================================================================================================================================
void UninitBoss(void)
{
	for (int nCntParts = 0; nCntParts < g_aBoss.nNumParts; nCntParts++)
	{
		//メッシュの破棄
		if (g_aBossParts[nCntParts].pMesh != NULL)
		{//メッシュ情報が存在する場合
			g_aBossParts[nCntParts].pMesh->Release();
			g_aBossParts[nCntParts].pMesh = NULL;
		}
		//マテリアルの破棄
		if (g_aBossParts[nCntParts].pBuffMat != NULL)
		{//マテリアル情報が存在する場合
			g_aBossParts[nCntParts].pBuffMat->Release();
			g_aBossParts[nCntParts].pBuffMat = NULL;
		}
	}
}

//==================================
//ボス位置把握表示の終了処理
//==================================
void UninitGraspBoss(void)
{
	if (g_pTextureGraspBoss != NULL)
	{
		g_pTextureGraspBoss->Release();
		g_pTextureGraspBoss = NULL;
	}

	if (g_pVtxBuffGraspBoss != NULL)
	{
		g_pVtxBuffGraspBoss->Release();
		g_pVtxBuffGraspBoss = NULL;
	}
}

//=======================================================================================================================================================================================================
// 更新処理
//=======================================================================================================================================================================================================
void UpdateBoss(void)
{
	Player* pPlayer = GetPlayer();	//プレイヤーの情報を取得
	Field* pField = GetField();		//フィールドの情報を取得
	Camera* pCamera = GetCamera();  //カメラの情報を取得
	g_nDelayBoss++;

	if (GetTimer() == BOSS_SAMMON_TIME && !g_aBoss.bBossSummon)
	{//残り時間が120秒になった場合
		SetBoss(D3DXVECTOR3(0.0f, 0.0f, 1800.0f), 1200);//設定処理
		g_aBoss.bBossSummon = true;//ボス召喚済み
		StopSound(SOUND_LABEL_BGM013);//ゲームBGM
		PlaySound(SOUND_LABEL_BGM015);//ボスBGM
		SetGraspBoss();//ボス位置把握表示を設定
		g_aBoss.state = BOSSSTATE_NEUTRAL;
	}

	if (g_aBoss.bUse && g_aBoss.bBossAppear == false)
	{//使用されている場合かつ、出現演出がオフの場合
		CaluculateMatrixBoss();
		g_aBoss.nCntAppearTime++;//出現してからの時間
		g_aBoss.move.y += -2.0f;//重力処理

		if (g_aBoss.pos.y <= 0.0f)
		{//Y座標が0以下にならないように調整
			g_aBoss.pos.y = 0.0f;
			g_aBoss.move.y = 0.0f;
		}

		//===============================================
		//一定ダメージごとにスクラップをドロップ
		//===============================================
		if (g_aBoss.nDamageCnt > 0)
		{
			g_aBoss.nDamageCnt--;
			g_aBoss.nTotalDamage++;

			if (g_aBoss.nTotalDamage % 5 == 0)
			{

				float fPurposeoPosX = float(rand() % 400 - 200);
				float fPurposeoPosY = float(rand() % 400 - 200);
				float fPurposeoPosZ = float(rand() % 400 - 200);

				float fVXaim3D = g_aBoss.pos.x - (g_aBoss.pos.x + fPurposeoPosX);
				float fVYaim3D = g_aBoss.pos.y - (g_aBoss.pos.y + fPurposeoPosY);
				float fVZaim3D = g_aBoss.pos.z - (g_aBoss.pos.z + fPurposeoPosZ);

				D3DXVECTOR3 BulletMove = NULL_VECTOR3;

				float fVLaim3D = sqrtf((fVXaim3D * fVXaim3D) + (fVYaim3D * fVYaim3D)
					+ (fVZaim3D * fVZaim3D));

				BulletMove.x = fVXaim3D / fVLaim3D * 10.0f;
				BulletMove.y = 0.0f;
				BulletMove.z = fVZaim3D / fVLaim3D * 10.0f;

				int nLottery = rand() % 5;

				SetScrap(nLottery, 1000, SCRAPRADIUSSIZE, SCRAPRADIUSSIZE, D3DXVECTOR3(g_aBoss.pos.x, g_aBoss.pos.y + SCRAPRADIUSSIZE, g_aBoss.pos.z),
					NULL_VECTOR3, BulletMove);
			}

		}

		HitStopBoss();//ヒットストップ処理

		if (pCamera->nMode == CAMERAMODE_REVOLUTION)
		{
			//====================================================
			// 敵とプレイヤーの距離を取り、XZのベクトルを求める
			//====================================================
			float fLength = sqrtf((g_aBoss.pos.x - pPlayer->pos.x) * (g_aBoss.pos.x - pPlayer->pos.x) +
				(g_aBoss.pos.z - pPlayer->pos.z) * (g_aBoss.pos.z - pPlayer->pos.z)) / 2;

			g_aBoss.fVXaim = pPlayer->pos.x - g_aBoss.pos.x;
			g_aBoss.fVYaim = pPlayer->pos.y - g_aBoss.pos.y;
			g_aBoss.fVZaim = pPlayer->pos.z - g_aBoss.pos.z;

			g_aBoss.fVaim = atan2f(g_aBoss.fVXaim, g_aBoss.fVZaim);
			g_aBoss.fVaimY1 = atan2f(g_aBoss.fVYaim, g_aBoss.fVZaim);

			//=======================
			//通常行動
			//=======================
			if (g_nDelayBoss % 120 == 0)
			{
				g_aBoss.nPattern = rand() % 2;
			}

			if (g_aBoss.nPattern == 0)
			{//攻撃をしていないかつ、距離が700.0fより近かったら
				if (!g_aBoss.bUnderAttack)
				{
					g_aBoss.rot.y = g_aBoss.fVaim + D3DX_PI;//プレイヤーの方向を向く
					g_aBoss.state = BOSSSTATE_MOVE;//移動状態

					g_aBoss.move.x = sinf(g_aBoss.fVaim) * g_aBoss.fSpeed;
					g_aBoss.move.z = cosf(g_aBoss.fVaim) * g_aBoss.fSpeed;
				}
			}
			else
			{//範囲外
				if (!g_aBoss.bUnderAttack)
				{
					g_aBoss.state = BOSSSTATE_NEUTRAL;//ニュートラル状態

					g_aBoss.move.x = 0.0f;
					g_aBoss.move.z = 0.0f;
				}
			}

			//============================================
			//ボスの技発動処理
			//============================================
			if (g_aBoss.bUnderAttack == false)
			{
				g_aBoss.nAttackCoolTime++;
			}

			if (g_aBoss.nAttackCoolTime == 230)
			{
				SetWarning(100, 20.0f, 60.0f, g_aBoss.pos);
			}

			if (g_aBoss.nAttackCoolTime == 300)
			{//クールタイムに入ってから５秒経ったら、次の攻撃の抽選を開始し、攻撃状態にする
				float fRatioHp = float(g_aBoss.nHp) / float(g_aBoss.nMaxHp);
				g_aBoss.nRunAwayLottery = rand() % 10;

				if (fRatioHp >= 0.5f)
				{//体力が５０％以上なら
					g_aBoss.nLotteryAttack = rand() % (BOSSATTACK_MAX - 3);
				}
				else
				{//体力が５０％以下なら（強い技を使用するようになる）
					g_aBoss.nCntUseAttack++;//攻撃をした回数をカウントする

					g_aBoss.nLotteryAttack = rand() % (BOSSATTACK_MAX - 3) + 3;

					if (g_aBoss.nCntUseAttack == 1)
					{
						g_aBoss.nLotteryAttack = 9;
					}
				}
				//g_aBoss.nLotteryAttack = 5;

				g_aBoss.bUnderAttack = true;

				g_aBoss.nAttackCoolTime = 0;//攻撃発動したのでクールタイムを０に

				if (g_aBoss.nRunAwayLottery >= 0 && g_aBoss.nRunAwayLottery <= 1)
				{
					//g_aBoss.bRunAwayBoss = true;
					for (int nCntEnemy = 0; nCntEnemy < 10; nCntEnemy++)
					{
						float fLengthrand = float(rand() % 500);
						float fRot = float(rand() % 200 + 1) / 100.0f;
						int nType = rand() % 4;
						SetEnemy(D3DXVECTOR3(g_aBoss.pos.x + sinf(D3DX_PI * fRot) * fLengthrand, 0.0f, g_aBoss.pos.z + cosf(D3DX_PI * fRot) * fLengthrand),
							NULL_VECTOR3,
							nType, true);//敵をセットした数分、関数の先で、ボスが召喚した数をカウントしている
					}
				}
			}

			if (g_aBoss.bUnderAttack == true)
			{
				if (g_aBoss.nLotteryAttack == BOSSATTACK00_ATOMICPRESS)
				{
					BossAttack0_AtomicPress();//攻撃０：アトミックプレス
				}
				else if (g_aBoss.nLotteryAttack == BOSSATTACK01_FLASHPUNCH)
				{
					BossAttack1_FlashPunch();//攻撃１：フラッシュパンチ
				}
				else if (g_aBoss.nLotteryAttack == BOSSATTACK02_THRUSTSTUB)
				{
					BossAttack2_ThrustStub();//攻撃２：スラストスタブ
				}
				else if (g_aBoss.nLotteryAttack == BOSSATTACK03_BIRDSTRIKE)
				{
					BossAttack3_BirdStrike();//攻撃３：バードストライク
				}
				else if (g_aBoss.nLotteryAttack == BOSSATTACK04_BOMBRAIN)
				{
					BossAttack4_BombRain();//攻撃４：ボムレイン
				}
				else if (g_aBoss.nLotteryAttack == BOSSATTACK05_QUELLTHEEARTH)
				{
					BossAttack5_QuellTheEarth();//攻撃５：クウェルジアース
				}
				else if (g_aBoss.nLotteryAttack == BOSSATTACK06_BURNINGCHICKEN)
				{
					BossAttack6_BurningChicken();//ボス攻撃６：バーニングチキン
				}
				else if (g_aBoss.nLotteryAttack == BOSSATTACK07_SLANDERTORNADO)
				{
					BossAttack7_SlanderTornado();//攻撃７：スレンダートルネード
				}
				else if (g_aBoss.nLotteryAttack == BOSSATTACK08_STARDUSTMETEO)
				{
					BossAttack8_StardustMeteo();//攻撃８：スターダストメテオ
				}
				else if (g_aBoss.nLotteryAttack == BOSSATTACK09_COLLAPSELASER)
				{
					BossAttack9_CollapseLaser();//攻撃９：コラプスレーザー
				}

				//else
				//{//ボスが逃げれば
				//	RunAwayBoss();//ボスが逃げる
				//}
			}

			//=================================================
			//プレイヤーに接近する範囲の調整
			//=================================================
			if (g_aBoss.bUnderAttack == false)
			{
				if (fLength <= 50.0f)
				{
					g_aBoss.move.x = 0.0f;
					g_aBoss.move.z = 0.0f;
				}
			}

			g_aBoss.posOld = g_aBoss.pos;//1F前の位置を保存
			g_aBoss.pos += g_aBoss.move;//移動量を加算

			g_aBoss.nNumLandingModel = -1;

			if (!g_aBoss.bUnderAttack)
			{
				g_aBoss.bIsLanding = bCollisionModel(&g_aBoss.pos, &g_aBoss.posOld, g_aBoss.Size, g_aBoss.nNumLandingModel);
			}

			PrintDebugProc("ボスの位置：%f %f %f\n", g_aBoss.pos.x, g_aBoss.pos.y, g_aBoss.pos.z);
			PrintDebugProc("ボスの攻撃時間：%d\n", g_aBoss.nCntAttackTime);

			//=========================================
			//ボスの行動範囲制限
			//=========================================
			if (g_aBoss.pos.x + (g_aBoss.Size.x / 2.0f) > pField->fWidth || g_aBoss.pos.x - (g_aBoss.Size.x / 2.0f) < -pField->fWidth ||
				g_aBoss.pos.z + (g_aBoss.Size.z / 2.0f) > pField->fHeight || g_aBoss.pos.z - (g_aBoss.Size.z / 2.0f) < -pField->fHeight)
			{
				g_aBoss.pos = g_aBoss.posOld;
			}

			//影の設定
			SetPositionShadow(g_aBoss.nIdxShadow, g_aBoss.pos);

			//レーダーディスプレイの設定
			SetPositionRaderDisPlay(g_aBoss.nIdxRaderDisplay, g_aBoss.pos, 0.0f);

			//==============================================
			//ボスが倒された時
			//==============================================
			if (g_aBoss.nHp <= 0)
			{
				g_aBoss.bUse = false;
			}

			if (g_aBoss.bUse == false)
			{
				StopSound(SOUND_LABEL_BGM015);
				PlaySound(SOUND_LABEL_BGM013);

				for (int nCntScrap = 0; nCntScrap < 100; nCntScrap++)
				{

					float fRandPos = float(rand() % 100 + 1);
					float fRandRot = float(rand() % 200) / 100;

					SetScrap(4, 300, 30.0f, 30.0f, D3DXVECTOR3(sinf(D3DX_PI * fRandRot) * fRandPos + pPlayer->pos.x, 0.0f, cosf(D3DX_PI * fRandRot) * fRandPos + pPlayer->pos.z), NULL_VECTOR3, NULL_VECTOR3);
				}

				AddScore(5000);
				KillShadow(g_aBoss.nIdxShadow);
				KillRaderDisplay(g_aBoss.nIdxRaderDisplay);
			}
		}
		
		if (g_aBoss.nCntAppearTime == 1)
		{
			g_aBoss.state = BOSSSTATE_NEUTRAL;
		}
	}

	//モーション処理
	BossMotion();

	PrintDebugProc("ボスのベクトル：%f\n", g_aBoss.fVaim);
	PrintDebugProc("ボスの向き：%f\n", g_aBoss.rot.y - D3DX_PI);
	PrintDebugProc("合計ダメージ：%d\n",g_aBoss.nTotalDamage);

}

void UpdateGraspBoss(void)
{
	VERTEX_2D* pVtx;

	Player* pPlayer = GetPlayer();
	Camera* pCamera = GetCamera();
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffGraspBoss->Lock(0, 0, (void**)&pVtx, 0);

	if (g_aGraspBoss.bUse == true)
	{
		g_aGraspBoss.pos.x = sinf(pPlayer->fVaim - pCamera->rot.y) * 1500.0f + SCREEN_WIDTH / 2.0f;
		g_aGraspBoss.pos.y = cosf(pPlayer->fVaim - pCamera->rot.y + D3DX_PI) * 1500.0f + SCREEN_HEIGHT / 2.0f;

		g_aGraspBoss.pos += g_aGraspBoss.move;

		if (g_aGraspBoss.pos.x <= 0.0f + GRASPBOSS_SIZE / 2.0f)
		{//左
			g_aGraspBoss.pos.x = 0.0f + GRASPBOSS_SIZE / 2.0f;
			g_aGraspBoss.move = NULL_VECTOR3;
			g_aGraspBoss.fAnimationPattern = 0.0f;
		}
		if (g_aGraspBoss.pos.x >= SCREEN_WIDTH - GRASPBOSS_SIZE / 2.0f)
		{//右
			g_aGraspBoss.pos.x = SCREEN_WIDTH - GRASPBOSS_SIZE / 2.0f;
			g_aGraspBoss.move = NULL_VECTOR3;

			g_aGraspBoss.fAnimationPattern = 2.0f;

		}

		if (g_aGraspBoss.pos.y <= 0.0f + GRASPBOSS_SIZE / 2.0f)
		{//上
			g_aGraspBoss.pos.y = 0.0f + GRASPBOSS_SIZE / 2.0f;
			g_aGraspBoss.move = NULL_VECTOR3;
			g_aGraspBoss.fAnimationPattern = 1.0f;

		}
		if (g_aGraspBoss.pos.y >= SCREEN_HEIGHT - GRASPBOSS_SIZE / 2.0f)
		{//下
			g_aGraspBoss.pos.y = SCREEN_HEIGHT - GRASPBOSS_SIZE / 2.0f;
			g_aGraspBoss.move = NULL_VECTOR3;

			g_aGraspBoss.fAnimationPattern = 3.0f;


		}

		pVtx[0].tex = D3DXVECTOR2(0.0f + 0.25f * g_aGraspBoss.fAnimationPattern, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.25f + 0.25f * g_aGraspBoss.fAnimationPattern, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + 0.25f * g_aGraspBoss.fAnimationPattern, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.25f + 0.25f * g_aGraspBoss.fAnimationPattern,1.0f);

		pVtx[0].pos = D3DXVECTOR3(g_aGraspBoss.pos.x - (GRASPBOSS_SIZE / 2.0f), g_aGraspBoss.pos.y - (GRASPBOSS_SIZE / 2.0f), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aGraspBoss.pos.x + (GRASPBOSS_SIZE / 2.0f), g_aGraspBoss.pos.y - (GRASPBOSS_SIZE / 2.0f), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aGraspBoss.pos.x - (GRASPBOSS_SIZE / 2.0f), g_aGraspBoss.pos.y + (GRASPBOSS_SIZE / 2.0f), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aGraspBoss.pos.x + (GRASPBOSS_SIZE / 2.0f), g_aGraspBoss.pos.y + (GRASPBOSS_SIZE / 2.0f), 0.0f);

		if (g_aBoss.bUse == false)
		{
			g_aGraspBoss.bUse = false;
		}

	}

	g_pVtxBuffGraspBoss->Unlock();
}

//=======================================================================================================================================================================================================
// 描画処理
//=======================================================================================================================================================================================================
void DrawBoss(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスへのポインタと取得
	D3DXMATRIX mtxRot, mtxTrans, mtxParent;//計算用マトリックス
	D3DMATERIAL9 matDef;//現在のマテリアル保存用
	D3DXMATERIAL* pMat;//マテリアルデータへのポインタ

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	//使用している場合
	if (g_aBoss.bUse == true)
	{
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aBoss.mtxWorld);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aBoss.rot.y, g_aBoss.rot.x, g_aBoss.rot.z);
		D3DXMatrixMultiply(&g_aBoss.mtxWorld, &g_aBoss.mtxWorld, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_aBoss.pos.x, g_aBoss.pos.y, g_aBoss.pos.z);
		D3DXMatrixMultiply(&g_aBoss.mtxWorld, &g_aBoss.mtxWorld, &mtxTrans);

		for (int nCntParts = 0; nCntParts < g_aBoss.nNumParts; nCntParts++)
		{
			//各パーツモデルのワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aBoss.aParts[nCntParts].mtxWorld);

			//各パーツモデルの向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot,
				g_aBoss.aParts[nCntParts].rot.y,
				g_aBoss.aParts[nCntParts].rot.x,
				g_aBoss.aParts[nCntParts].rot.z);

			D3DXMatrixMultiply(&g_aBoss.aParts[nCntParts].mtxWorld, &g_aBoss.aParts[nCntParts].mtxWorld, &mtxRot);

			//各パーツモデルの位置を反映
			D3DXMatrixTranslation(&mtxTrans,
				g_aBoss.aParts[nCntParts].pos.x,
				g_aBoss.aParts[nCntParts].pos.y,
				g_aBoss.aParts[nCntParts].pos.z);

			D3DXMatrixMultiply(&g_aBoss.aParts[nCntParts].mtxWorld, &g_aBoss.aParts[nCntParts].mtxWorld, &mtxTrans);

			//親番号の保存
			int nParent = g_aBoss.aParts[nCntParts].nParent;

			//親のマトリックスを反映
			if (nParent != -1)
			{//親の番号を持つ場合
				mtxParent = g_aBoss.aParts[nParent].mtxWorld;//親モデルのマトリックス
			}
			else
			{//親の番号を持たない場合
				mtxParent = g_aBoss.mtxWorld;//プレイヤーのマトリックス
			}

			//親のマトリックスを掛け合わせる
			D3DXMatrixMultiply(&g_aBoss.aParts[nCntParts].mtxWorld, &g_aBoss.aParts[nCntParts].mtxWorld, &mtxParent);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aBoss.aParts[nCntParts].mtxWorld);

			//現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			//マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_aBoss.aParts[nCntParts].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aBoss.aParts[nCntParts].dwNumMat; nCntMat++)
			{
				//マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//テクスチャの設定
				pDevice->SetTexture(0, NULL);

				//モデル(パーツ)の描画
				g_aBoss.aParts[nCntParts].pMesh->DrawSubset(nCntMat);
			}

			//保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

void DrawGraspBoss(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffGraspBoss, 0, sizeof(VERTEX_2D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_aGraspBoss.bUse == true)
	{
		pDevice->SetTexture(0, g_pTextureGraspBoss);
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);//0は、描画する最初の頂点インデックスで、2は、描画するプリミティブ数
	}
}

//=======================================================================================================================================================================================================
// 敵のパーツ情報の読み込み
//=======================================================================================================================================================================================================
void LoadBossParts(void)
{
	int nIndex = 0;				//パーツ番号
	int nCntParts = 0;			//パーツ数
	char aDataSearch[MAX_TEXT];	//文字列検索用

	//ファイルを開く
	FILE* pFile = fopen(BOSSSET, "r");

	//ファイルが開けない場合
	if (pFile == NULL)
	{
		return;
	}

	//END_PARTSが見つかるまで読み込みを繰り返す
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch);	//検索

		//END_PARTSが見つかった場合
		if (!strcmp(aDataSearch, "END_PARTS"))
		{//読み込み終了
			fclose(pFile);	//ファイルを閉じる
			break;
		}

		//PARTSSETが見つかった場合
		if (!strcmp(aDataSearch, "PARTSSET"))
		{
			//項目ごとのデータを代入
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch);	//検索

				//END_PARTSSETが見つかった場合
				if (!strcmp(aDataSearch, "END_PARTSSET"))
				{//パーツ情報の読み込みを終了
					g_aBoss.aParts[nIndex].pos = g_aBossParts[nIndex].pos;
					g_aBoss.aParts[nIndex].rot = g_aBossParts[nIndex].rot;
					g_aBoss.aParts[nIndex].nParent = g_aBossParts[nIndex].nParent;
					break;
				}

				//INDEXが見つかった場合
				else if (!strcmp(aDataSearch, "INDEX"))
				{
					//番号を保存
					fscanf(pFile, "%d", &nIndex);

					//パーツの番号がパーツ数を超えている場合
					if (nIndex > g_aBoss.nNumParts)
					{//各パーツごとのデータ検索を終了
						break;
					}
				}

				//PARENTが見つかった場合
				else if (!strcmp(aDataSearch, "PARENT"))
				{//親の番号を保存
					fscanf(pFile, "%d", &g_aBossParts[nIndex].nParent);
				}

				//POSが見つかった場合
				else if (!strcmp(aDataSearch, "POS"))
				{//位置を保存
					fscanf(pFile, "%f", &g_aBossParts[nIndex].pos.x);//X座標
					fscanf(pFile, "%f", &g_aBossParts[nIndex].pos.y);//Y座標
					fscanf(pFile, "%f", &g_aBossParts[nIndex].pos.z);//Z座標
				}

				//ROTが見つかった場合
				else if (!strcmp(aDataSearch, "ROT"))
				{//向きを保存
					fscanf(pFile, "%f", &g_aBossParts[nIndex].rot.x);//X軸
					fscanf(pFile, "%f", &g_aBossParts[nIndex].rot.y);//Y軸
					fscanf(pFile, "%f", &g_aBossParts[nIndex].rot.z);//Z軸
				}
			}

			nCntParts++;					//パーツ数を加算
			g_aBoss.nNumParts = nCntParts;	//合計パーツ数を保存
		}

		//上記以外が見つかった場合
		else
		{//スキップして読み込みを続ける
			continue;
		}
	}
}

//============================================================================================================================================================================================================
// モーション情報の読み込み処理
//============================================================================================================================================================================================================
void LoadBossMotion(void)
{
	int nMotion = -1;			//モーションの種類
	int nCntParts = 0;			//パーツ数
	int nCntKey[MAX_BOSSMOTION];//各モーションキー数
	char aDataSearch[MAX_TEXT];	//データ検索用

	//各モーションキー数の初期化
	for (int nCnt = 0; nCnt < MAX_BOSSMOTION; nCnt++)
	{
		nCntKey[nCnt] = 0;
	}

	//ファイルを開く
	FILE* pFile = fopen(BOSSSET, "r");

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
			//モーションの種類数を加算
			nMotion++;

			//END_MOTIONSETが見つかるまで読み込みを繰り返す
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch);	//検索

				//END_MOTIONSETが見つかった場合
				if (!strcmp(aDataSearch, "END_MOTIONSET"))
				{//モーション読み込み終了
					break;
				}

				//ループ判定が見つかった場合
				else if (!strcmp(aDataSearch, "LOOP"))
				{//ループするかどうかを取得
					fscanf(pFile, "%d", &g_aBossMotionSet[nMotion].nLoop);
				}

				//キーセットが見つかった場合
				else if (!strcmp(aDataSearch, "KEYSET"))
				{
					while (1)
					{
						fscanf(pFile, "%s", aDataSearch);	//検索

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
									fscanf(pFile, "%d", &g_aBossKeySet[nCntKey[nMotion]][nMotion].nMaxFrame);
								}

								//POSが見つかった場合
								else if (!strcmp(aDataSearch, "POS"))
								{//位置
									fscanf(pFile, "%f", &g_aBossKeySet[nCntKey[nMotion]][nMotion].key[nCntParts].pos.x);//X座標
									fscanf(pFile, "%f", &g_aBossKeySet[nCntKey[nMotion]][nMotion].key[nCntParts].pos.y);//Y座標
									fscanf(pFile, "%f", &g_aBossKeySet[nCntKey[nMotion]][nMotion].key[nCntParts].pos.z);//Z座標
								}
								
								//ROTが見つかった場合
								else if (!strcmp(aDataSearch, "ROT"))
								{//向き
									fscanf(pFile, "%f", &g_aBossKeySet[nCntKey[nMotion]][nMotion].key[nCntParts].rot.x);//X軸
									fscanf(pFile, "%f", &g_aBossKeySet[nCntKey[nMotion]][nMotion].key[nCntParts].rot.y);//Y軸
									fscanf(pFile, "%f", &g_aBossKeySet[nCntKey[nMotion]][nMotion].key[nCntParts].rot.z);//Z軸
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
						else if (!strcmp(aDataSearch, "END_KEYSET"))
						{
							nCntParts = 0;
							nCntKey[nMotion]++;//キー数を加算
							g_aBossMotionSet[nMotion].nMaxKey = nCntKey[nMotion];//合計キー数を保存
							break;
						}

						//コメントアウトが見つかった場合
						else
						{//スキップして読み込みを続ける
							continue;
						}
					}
				}

				//コメントアウトが見つかった場合
				else
				{//スキップして読み込みを続ける
					continue;
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
void BossMotion(void)
{
	int nState = (int)(g_aBoss.state);																		//状態を保存
	int nCntKey = g_aBoss.aMotionset[nState].nCntKey;														//現在のキーを保存
	int nNextKey = (nCntKey + 1) % g_aBossMotionSet[nState].nMaxKey;										//次のキーを求める
	float fFrame = (float)g_aBoss.aMotionset[nState].nCntFrame / g_aBossKeySet[nCntKey][nState].nMaxFrame;	//現在のフレームと全体フレームの割合

	//パーツごとに情報を更新
	for (int nCntParts = 0; nCntParts < g_aBoss.nNumParts; nCntParts++)
	{
		float BossPosX = g_aBossKeySet[nCntKey][nState].key[nCntParts].pos.x;
		float BossPosY = g_aBossKeySet[nCntKey][nState].key[nCntParts].pos.y;
		float BossPosZ = g_aBossKeySet[nCntKey][nState].key[nCntParts].pos.z;
		float BossRotX = g_aBossKeySet[nCntKey][nState].key[nCntParts].rot.x;
		float BossRotY = g_aBossKeySet[nCntKey][nState].key[nCntParts].rot.y;
		float BossRotZ = g_aBossKeySet[nCntKey][nState].key[nCntParts].rot.z;

		float NextBossPosX = g_aBossKeySet[nNextKey][nState].key[nCntParts].pos.x;
		float NextBossPosY = g_aBossKeySet[nNextKey][nState].key[nCntParts].pos.y;
		float NextBossPosZ = g_aBossKeySet[nNextKey][nState].key[nCntParts].pos.z;
		float NextBossRotX = g_aBossKeySet[nNextKey][nState].key[nCntParts].rot.x;
		float NextBossRotY = g_aBossKeySet[nNextKey][nState].key[nCntParts].rot.y;
		float NextBossRotZ = g_aBossKeySet[nNextKey][nState].key[nCntParts].rot.z;

		//次のキーまでの差分を求める
		float fPosx = NextBossPosX - BossPosX;	// X座標
		float fPosy = NextBossPosY - BossPosY;	// Y座標
		float fPosz = NextBossPosZ - BossPosZ;	// Z座標
		float fRotx = NextBossRotX - BossRotX;	// X軸の向き
		float fRoty = NextBossRotY - BossRotY;	// Y軸の向き
		float fRotz = NextBossRotZ - BossRotZ;	// Z軸の向き

		//向きを更新
		g_aBoss.aParts[nCntParts].rot.x = g_aBossParts[nCntParts].rot.x + BossRotX + fRotx * fFrame;//X軸
		g_aBoss.aParts[nCntParts].rot.y = g_aBossParts[nCntParts].rot.y + BossRotY + fRoty * fFrame;//Y軸
		g_aBoss.aParts[nCntParts].rot.z = g_aBossParts[nCntParts].rot.z + BossRotZ + fRotz * fFrame;//Z軸

		//位置を更新
		g_aBoss.aParts[nCntParts].pos.x = g_aBossParts[nCntParts].pos.x + BossPosX + fPosx * fFrame;//X座標
		g_aBoss.aParts[nCntParts].pos.y = g_aBossParts[nCntParts].pos.y + BossPosY + fPosy * fFrame;//Y座標
		g_aBoss.aParts[nCntParts].pos.z = g_aBossParts[nCntParts].pos.z + BossPosZ + fPosz * fFrame;//Z座標
	}

	//キー数の更新
	if (g_aBoss.aMotionset[nState].nCntFrame == 0 || g_aBoss.aMotionset[nState].nCntFrame >= g_aBossKeySet[nCntKey][nState].nMaxFrame)
	{
		if (g_aBoss.aMotionset[nState].nCntFrame >= g_aBossKeySet[nCntKey][nState].nMaxFrame)
		{
			//フレーム数カウンタをリセット
			g_aBoss.aMotionset[nState].nCntFrame = 0;
		}

		//キー数を加算
		g_aBoss.aMotionset[nState].nCntKey++;

		//キー数が最大数になった
		if (g_aBoss.aMotionset[nState].nCntKey >= g_aBossMotionSet[nState].nMaxKey)
		{
			//ループしない場合
			if (g_aBossMotionSet[nState].nLoop == 1)
			{
				g_aBoss.nAttackTiming = 0;//攻撃が終わった時も含まれるので、攻撃カウントを０にする
				g_aBoss.bHitFlag = false;//攻撃が一周したので、ヒット判定をリセット
			
				//ニュートラル状態
				g_aBoss.state = BOSSSTATE_NEUTRAL;
			}

			//キー数を初期化
			g_aBoss.aMotionset[nState].nCntKey = 0;
		}
	}

	//フレーム数更新
	g_aBoss.aMotionset[nState].nCntFrame++;
}

//============================================================================================================================================================================================================
//ボスを召喚する
//============================================================================================================================================================================================================
void SetBoss(D3DXVECTOR3 pos, int nHp)
{
	D3DXMATERIAL* pMat;//マテリアルのデータへのポインタ
	BYTE* pVtxBuff;//頂点バッファへのポインタ

	if (g_aBoss.bUse == false)
	{
		g_aBoss.pos = pos;
		g_aBoss.bUse = true;
		g_aBoss.nHp = nHp;
		g_aBoss.nMaxHp = nHp;
		g_aBoss.fSpeed = 3.0f;
		g_aBoss.fRotMove = 0.0f;//一部の処理でこの変数の値の方向に移動させる
		g_aBoss.nNumLandingModel = 0;//どのモデルの上に乗っているか
		g_aBoss.nIdxShadow = SetShadow();//影のインデックスを設定
		g_aBoss.nIdxRaderDisplay = SetRaderDisplay(RADERDISPLAY02_BOSS);//レーダーディスプレイを設定
		g_aBoss.fVXaim3D = 0.0f;
		g_aBoss.fVYaim3D = 0.0f;
		g_aBoss.fVZaim3D = 0.0f;
		g_aBoss.fVLaim3D = 0.0f;
		g_aBoss.SaveMove3D = NULL_VECTOR3;
		g_aBoss.nCntAppearTime = 0;
		g_aBoss.bBossAppear = true;//ボス出現演出中かどうか
		g_aBoss.bCurveLaser = false;//レーザーをカーブさせる向きを決める
		g_aBoss.nRunAwayLottery = 0;//低確率でボスを逃げさせる乱数
		g_aBoss.bRunAwayBoss = false;//フラグ発動時にボスを逃げさせるフラグ
		g_aBoss.nCntSummonEnemy = 0;//ボスが召喚した敵の数をカウントする
		g_aBoss.nTotalDamage = 0;//与えたダメージ合計
		g_aBoss.nDamageCnt = 0;
		g_aBoss.nCntUseAttack = 0;//攻撃をした回数をカウントする

		SetBossGauge(0);

		for (int nCntParts = 0; nCntParts < g_aBoss.nNumParts; nCntParts++)
		{
			//マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_aBoss.aParts[nCntParts].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aBoss.aParts[nCntParts].dwNumMat; nCntMat++)
			{
				g_aBoss.aParts[nCntParts].Diffuse[nCntMat] = pMat[nCntMat].MatD3D.Diffuse;
			}

			//頂点数の取得
			int nNumVtx = g_aBoss.aParts[nCntParts].pMesh->GetNumVertices();

			//頂点フォーマットのサイズを取得
			DWORD sizeFVF = D3DXGetFVFVertexSize(g_aBoss.aParts[nCntParts].pMesh->GetFVF());

			//頂点バッファのロック
			g_aBoss.aParts[nCntParts].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

			for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
			{
				//頂点座標の代入
				D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;


				//================================
				// プレイヤーの最大最小を決める
				//================================

				//モデルの頂点座標の最小値と最大値を取得
				if (vtx.x < g_aBoss.vtxMin.x)
				{//X座標の最小値
					g_aBoss.vtxMin.x = vtx.x;
				}
				if (vtx.y < g_aBoss.vtxMin.y)
				{//Y座標の最小値
					g_aBoss.vtxMin.y = vtx.y;
				}
				if (vtx.z < g_aBoss.vtxMin.z)
				{//Z座標の最小値
					g_aBoss.vtxMin.z = vtx.z;
				}
				if (vtx.x > g_aBoss.vtxMax.x)
				{//X座標の最大値
					g_aBoss.vtxMax.x = vtx.x;
				}
				if (vtx.y > g_aBoss.vtxMax.y)
				{//Y座標の最大値
					g_aBoss.vtxMax.y = vtx.y;
				}
				if (vtx.z > g_aBoss.vtxMax.z)
				{//Z座標の最大値
					g_aBoss.vtxMax.z = vtx.z;
				}

				//頂点フォーマットのサイズ分ポインタを進める
				pVtxBuff += sizeFVF;
			}

			//種類ごとのサイズを求める
			g_aBoss.Size =
			{
				g_aBoss.vtxMax.x - g_aBoss.vtxMin.x,
				g_aBoss.vtxMax.y - g_aBoss.vtxMin.y,
				g_aBoss.vtxMax.z - g_aBoss.vtxMin.z
			};

			//頂点バッファのアンロック
			g_aBoss.aParts[nCntParts].pMesh->UnlockVertexBuffer();
		}
	}
}

void SetGraspBoss(void)
{
	if (g_aGraspBoss.bUse == false)
	{
		g_aGraspBoss.pos = NULL_VECTOR3;
		g_aGraspBoss.rot = NULL_VECTOR3;
		g_aGraspBoss.bUse = true;
	}
}

//============================================================================================================================================================================================================
//ボスの情報を取得する
//============================================================================================================================================================================================================
Boss * GetBoss(void)
{
	return &g_aBoss;
}

//============================================================================================================================================================================================================
//ボスにダメージを与える
//============================================================================================================================================================================================================
void SetDamageBoss(int nDamage, PLAYERATTACK PlayerAttackNum, int nNumAttackModel)
{
	if (g_aBoss.bHitStop[PlayerAttackNum] == false)//ヒットストップ状態じゃなければ
	{
		if (PlayerAttackNum == PLAYERATTACK00_SWORD || PlayerAttackNum == PLAYERATTACK02_SPEAKER)
		{
			g_aBoss.nDamageCnt += nDamage;
			g_aBoss.nHp -= nDamage;//体力を減らす
			g_aBoss.bHitStop[PlayerAttackNum] = true;  //ダメージを与えた攻撃に対してヒットストップ状態にする

			for (int nCntParticle = 0; nCntParticle < 100; nCntParticle++)
			{
				float fRot = float(rand() % 200 + 1) / 100;
				float fYmove = float(rand() % 200 - 100) / 10;
				float fSpeed = float(rand() % 200 - 100) / 10;

				SetParticle(PARTICLETYPE_NORMAL, 300, 10.0f, 0.6f, g_aBoss.pos,
					D3DXVECTOR3(sinf(-D3DX_PI * fRot) * fSpeed, fYmove, cosf(-D3DX_PI * fRot) * fSpeed),
					D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 0.0f, PARTICLETEX_NORMAL,0);
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
		if (g_aBoss.bHitStopBoomerang[nNumAttackModel] == false && PlayerAttackNum == PLAYERATTACK01_BOOMERANG)
		{
			g_aBoss.nDamageCnt += nDamage;
			g_aBoss.nHp -= nDamage;//体力を減らす
			g_aBoss.bHitStopBoomerang[nNumAttackModel] = true;

			for (int nCntParticle = 0; nCntParticle < 100; nCntParticle++)
			{
				float fRot = float(rand() % 200 + 1) / 100;
				float fYmove = float(rand() % 200 - 100) / 10;
				float fSpeed = float(rand() % 200 - 100) / 10;

				SetParticle(PARTICLETYPE_NORMAL, 300, 10.0f, 0.6f, g_aBoss.pos,
					D3DXVECTOR3(sinf(-D3DX_PI * fRot) * fSpeed, fYmove, cosf(-D3DX_PI * fRot) * fSpeed),
					D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 0.0f, PARTICLETEX_NORMAL,0);
			}

		    if (PlayerAttackNum == PLAYERATTACK01_BOOMERANG)
		    {
		    	PlaySound(SOUND_LABEL_SE_HITBOOMERANG_000);
		    }

		}
}

//============================================================================================================================================================================================================
//ヒットストップ処理
//============================================================================================================================================================================================================
void HitStopBoss(void)
{
	//======================================
	//攻撃受け付け調整
	//======================================

	for (int nCntHitStop = 0; nCntHitStop < PLAYERATTACK_MAX; nCntHitStop++)
	{
		if (g_aBoss.bHitStop[nCntHitStop] == true)
		{
			g_aBoss.nCoolTime[nCntHitStop]++;

			switch (nCntHitStop)
			{
			case PLAYERATTACK00_SWORD:

				if (g_aBoss.nCoolTime[nCntHitStop] >= 20)
				{
					g_aBoss.bHitStop[nCntHitStop] = false;
					g_aBoss.nCoolTime[nCntHitStop] = 0;
				}
				break;
			case PLAYERATTACK02_SPEAKER:

				if (g_aBoss.nCoolTime[nCntHitStop] >= 2)
				{
					g_aBoss.bHitStop[nCntHitStop] = false;
					g_aBoss.nCoolTime[nCntHitStop] = 0;
				}
				break;
			}
		}
	}

	for (int nCntAttackModel = 0; nCntAttackModel < MAX_ATTACKMODEL; nCntAttackModel++)
	{
		if (g_aBoss.bHitStopBoomerang[nCntAttackModel] == true)
		{
			g_aBoss.nCoolTimeBoomerang[nCntAttackModel]++;

			if (g_aBoss.nCoolTimeBoomerang[nCntAttackModel] >= 20)
			{
				g_aBoss.bHitStopBoomerang[nCntAttackModel] = false;
				g_aBoss.nCoolTimeBoomerang[nCntAttackModel] = 0;
			}
		}
	}
}

//============================================================================================================================================================================================================
//ボス攻撃０：アトミックプレス
//============================================================================================================================================================================================================
void BossAttack0_AtomicPress(void)
{
	Player* pPlayer = GetPlayer();	//プレイヤーの情報を取得

	g_aBoss.nCntAttackTime++;

	if (g_aBoss.nCntAttackTime == 1)
	{
		g_bPurposeFlag[0] = false;
		g_bPurposeFlag[1] = false;
	}
	else if (g_aBoss.nCntAttackTime == 20)
	{
		g_aBoss.state = BOSSSTATE_ATTACK;
	}

	if (g_aBoss.nCntAttackTime >= 30 && g_aBoss.nCntAttackTime <= 120)
	{
		g_aBoss.move.x = sinf(g_aBoss.fVaim) * 30.0f;
		g_aBoss.move.z = cosf(g_aBoss.fVaim) * 30.0f;

		g_aBoss.move.y = 16.0f;
	}

	if (g_aBoss.nCntAttackTime >= 90 && g_aBoss.nCntAttackTime <= 150)
	{
		if (g_aBoss.pos.x >= pPlayer->pos.x - 40.0f &&
			g_aBoss.pos.x <= pPlayer->pos.x + 40.0f &&
			g_aBoss.pos.z >= pPlayer->pos.z - 40.0f &&
			g_aBoss.pos.z <= pPlayer->pos.z + 40.0f && g_bPurposeFlag[0] == false)
		{
			g_bPurposeFlag[0] = true;
		}

		if (g_bPurposeFlag[0] == true)
		{
			g_aBoss.move = NULL_VECTOR3;
			g_aBoss.move.y = -20.0f;
		}

		if (g_aBoss.pos.y <= 0.0f && g_bPurposeFlag[1] == false)
		{
			SetAttackModel(ATTACKMODEL02_SPHERE, 180, g_aBoss.pos, NULL_VECTOR3, NULL_VECTOR3, NULL_VECTOR3, 0.0f, 0.0f, 0.2f, NULL_VECTOR3,0);
			g_bPurposeFlag[1] = true;

			PlaySound(SOUND_LABEL_SE_BIRDSTRIKE_000);

			for (int nCntBullet = 0; nCntBullet < 8; nCntBullet++)
			{
				float fRot = (2.0f / 12.0f) * (float)(nCntBullet);

				SetBullet(BULLETTYPE_CURVE, 180, 10.0f, 10.0f,D3DXVECTOR3(g_aBoss.pos.x,g_aBoss.pos.y + 10.0f,g_aBoss.pos.z),
					NULL_VECTOR3,D3DXVECTOR3(5.0f,fRot,5.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0, 0.0f);

				SetBullet(BULLETTYPE_CURVE, 180, 10.0f, 10.0f, D3DXVECTOR3(g_aBoss.pos.x, g_aBoss.pos.y + 10.0f, g_aBoss.pos.z),
					NULL_VECTOR3, D3DXVECTOR3(5.0f, fRot, 5.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 1, 0.0f);
			}
		}
	}

	if (g_aBoss.nCntAttackTime >= 130 && g_aBoss.nCntAttackTime <= 230)
	{
		g_aBoss.state = BOSSSTATE_ATTACK;

		if (g_aBoss.nCntAttackTime == 230)
		{
			g_aBoss.bUnderAttack = false;
			g_aBoss.nCntAttackTime = 0;
		}
	}
}

//============================================================================================================================================================================================================
//ボス攻撃１：フラッシュパンチ
//============================================================================================================================================================================================================
void BossAttack1_FlashPunch(void)
{
	Player* pPlayer = GetPlayer();	//プレイヤーの情報を取得

	g_aBoss.nCntAttackTime++;

	if (g_aBoss.nCntAttackTime == 1)
	{
		g_aBoss.move = NULL_VECTOR3;
		g_bPurposeFlag[0] = false;
	}

	if (g_aBoss.nCntAttackTime >= 20 && g_aBoss.nCntAttackTime <= 60)
	{
		g_aBoss.rot.y = g_aBoss.fVaim + D3DX_PI;//プレイヤーの方向を向く
	}

	if (g_aBoss.nCntAttackTime == 90)
	{
		g_aBoss.fSaveAim = g_aBoss.fVaim;
	}

	if (g_aBoss.nCntAttackTime >= 96 && g_aBoss.nCntAttackTime <= 136)
	{
		if (g_aBoss.nCntAttackTime == 96)
		{
			PlaySound(SOUND_LABEL_SE_FASTMOVE_000);
		}

		g_aBoss.move.x = sinf(g_aBoss.fSaveAim) * 20.0f;
		g_aBoss.move.z = cosf(g_aBoss.fSaveAim) * 20.0f;

		g_aBoss.state = BOSSSTATE_ATTACK;

		//右手とプレイヤーの距離
		float fLengthAttack = sqrtf((g_aBoss.aParts[3].PartsPos.x - pPlayer->pos.x) * (g_aBoss.aParts[3].PartsPos.x - pPlayer->pos.x) +
			(g_aBoss.aParts[3].PartsPos.z - pPlayer->pos.z) * (g_aBoss.aParts[3].PartsPos.z - pPlayer->pos.z) +
			(g_aBoss.aParts[3].PartsPos.y - pPlayer->pos.y) * (g_aBoss.aParts[3].PartsPos.y - pPlayer->pos.y)) / 3;

		if (fLengthAttack < 40.0f && g_bPurposeFlag[0] == false)
		{
			AttackHitPlayer(60);
			g_bPurposeFlag[0] = true;

			PlaySound(SOUND_LABEL_SE_FLUSHPUNCH_000);

			for (int nCntParticle = 0; nCntParticle < 40; nCntParticle++)
			{
				float fRandRot = float(rand() % 200 + 1) / 100.0f;
				float fRandSpeed = float(rand() % 20 + 1 - 10);
				float fRandYMove = float(rand() % 20 + 1 - 10);
				SetParticle(PARTICLETEX_NORMAL, 60, 10.0f, 0.4f, pPlayer->pos, D3DXVECTOR3(sinf(D3DX_PI * fRandRot) * fRandSpeed, fRandYMove, cosf(D3DX_PI * fRandRot) * fRandSpeed),
					D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 0.0f, PARTICLETEX_NORMAL,0);
			}
		}
	}

	if (g_aBoss.nCntAttackTime >= 150 && g_aBoss.nCntAttackTime <= 180)
	{
		g_aBoss.rot.y = g_aBoss.fVaim + D3DX_PI;//プレイヤーの方向を向く
	}

	if (g_aBoss.nCntAttackTime == 181)
	{
		g_aBoss.fSaveAim = g_aBoss.fVaim;
		g_bPurposeFlag[0] = false;
	}

	if (g_aBoss.nCntAttackTime >= 200 && g_aBoss.nCntAttackTime <= 235)
	{
		if (g_aBoss.nCntAttackTime == 200)
		{
			PlaySound(SOUND_LABEL_SE_FASTMOVE_000);
		}

		g_aBoss.move.x = sinf(g_aBoss.fSaveAim) * 20.0f;
		g_aBoss.move.z = cosf(g_aBoss.fSaveAim) * 20.0f;

		g_aBoss.state = BOSSSTATE_ATTACK;

		//右手とプレイヤーの距離
		float fLengthAttack = sqrtf((g_aBoss.aParts[3].PartsPos.x - pPlayer->pos.x) * (g_aBoss.aParts[3].PartsPos.x - pPlayer->pos.x) +
			(g_aBoss.aParts[3].PartsPos.z - pPlayer->pos.z) * (g_aBoss.aParts[3].PartsPos.z - pPlayer->pos.z) +
			(g_aBoss.aParts[3].PartsPos.y - pPlayer->pos.y) * (g_aBoss.aParts[3].PartsPos.y - pPlayer->pos.y)) / 3;

		if (fLengthAttack < 40.0f && g_bPurposeFlag[0] == false)
		{
			AttackHitPlayer(70);
			g_bPurposeFlag[0] = true;

			PlaySound(SOUND_LABEL_SE_FLUSHPUNCH_000);


			for (int nCntParticle = 0; nCntParticle < 50; nCntParticle++)
			{
				float fRandRot = float(rand() % 200 + 1) / 100.0f;
				float fRandSpeed = float(rand() % 20 + 1 - 10);
				float fRandYMove = float(rand() % 20 + 1 - 10);
				SetParticle(PARTICLETEX_NORMAL, 60, 20.0f, 0.3f, pPlayer->pos, D3DXVECTOR3(sinf(D3DX_PI * fRandRot) * fRandSpeed, fRandYMove, cosf(D3DX_PI * fRandRot) * fRandSpeed),
					D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 0.0f, PARTICLETEX_NORMAL,0);
			}
		}
	}

	if (g_aBoss.nCntAttackTime >= 250 && g_aBoss.nCntAttackTime <= 270)
	{
		g_aBoss.rot.y = g_aBoss.fVaim + D3DX_PI;//プレイヤーの方向を向く
	}

	if (g_aBoss.nCntAttackTime == 271)
	{
		g_aBoss.fSaveAim = g_aBoss.fVaim;

		g_bPurposeFlag[0] = false;
	}

	if (g_aBoss.nCntAttackTime >= 300 && g_aBoss.nCntAttackTime <= 340)
	{
		if (g_aBoss.nCntAttackTime == 300)
		{
			PlaySound(SOUND_LABEL_SE_FASTMOVE_000);
		}

		g_aBoss.move.x = sinf(g_aBoss.fSaveAim) * 25.0f;
		g_aBoss.move.z = cosf(g_aBoss.fSaveAim) * 25.0f;

		g_aBoss.state = BOSSSTATE_ATTACK;

		//右手とプレイヤーの距離
		float fLengthAttack = sqrtf((g_aBoss.aParts[3].PartsPos.x - pPlayer->pos.x) * (g_aBoss.aParts[3].PartsPos.x - pPlayer->pos.x) +
			(g_aBoss.aParts[3].PartsPos.z - pPlayer->pos.z) * (g_aBoss.aParts[3].PartsPos.z - pPlayer->pos.z) +
			(g_aBoss.aParts[3].PartsPos.y - pPlayer->pos.y) * (g_aBoss.aParts[3].PartsPos.y - pPlayer->pos.y)) / 3;

		if (fLengthAttack < 40.0f && g_bPurposeFlag[0] == false)
		{
			AttackHitPlayer(70);
			g_bPurposeFlag[0] = true;

			PlaySound(SOUND_LABEL_SE_FLUSHPUNCH_000);


			for (int nCntParticle = 0; nCntParticle < 50; nCntParticle++)
			{
				float fRandRot = float(rand() % 200 + 1) / 100.0f;
				float fRandSpeed = float(rand() % 20 + 1 - 10);
				float fRandYMove = float(rand() % 20 + 1 - 10);
				SetParticle(PARTICLETEX_NORMAL, 60, 20.0f, 0.3f, pPlayer->pos, D3DXVECTOR3(sinf(D3DX_PI * fRandRot) * fRandSpeed, fRandYMove, cosf(D3DX_PI * fRandRot) * fRandSpeed),
					D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 0.0f, PARTICLETEX_NORMAL,0);
			}
		}
	}

	if (g_aBoss.nCntAttackTime == 350)
	{
		g_aBoss.bUnderAttack = false;
		g_aBoss.nCntAttackTime = 0;
	}

	g_aBoss.move.x += (0.0f - g_aBoss.move.x) * 0.1f;
	g_aBoss.move.z += (0.0f - g_aBoss.move.z) * 0.1f;
}

//============================================================================================================================================================================================================
//ボス攻撃２：スラストスタブ
//============================================================================================================================================================================================================
void BossAttack2_ThrustStub(void)
{
	Player* pPlayer = GetPlayer();	//プレイヤーの情報を取得

	g_aBoss.nCntAttackTime++;

	if (g_aBoss.nCntAttackTime == 1)
	{
		g_aBoss.move = NULL_VECTOR3;
		g_bPurposeFlag[0] = false;
	}

	if (g_aBoss.nCntAttackTime == 15)
	{
		g_aBoss.state = BOSSSTATE_ATTACK;
		g_aBoss.fSaveAim = g_aBoss.fVaim;
		g_aBoss.rot.y = g_aBoss.fVaim + D3DX_PI;
	}

	if (g_aBoss.nCntAttackTime >= 81 && g_aBoss.nCntAttackTime <= 135)
	{
		float fRandPos = float(rand() % 500 + 1);
		float fRandY = float(rand() % 100 * 1);
		float fRandRot = float(rand() % 150 - 75) / 100;
		SetAttackbillboard(ATTACKBILLBOARD00_STUB, 60, 40.0f, 40.0f,
		D3DXVECTOR3(sinf(g_aBoss.fSaveAim + fRandRot) * fRandPos + g_aBoss.pos.x, fRandY + g_aBoss.pos.y, cosf(g_aBoss.fSaveAim + fRandRot) * fRandPos + g_aBoss.pos.z), NULL_VECTOR3, NULL_VECTOR3,
		0, 0.0f);
	}

	if (g_aBoss.nCntAttackTime == 170)
	{
		g_aBoss.fSaveAim = g_aBoss.fVaim;
	}

	if (g_aBoss.nCntAttackTime >= 200 && g_aBoss.nCntAttackTime <= 220)
	{
		g_aBoss.move = D3DXVECTOR3(sinf(g_aBoss.fSaveAim) * 50.0f,0.0f,cosf(g_aBoss.fSaveAim) * 50.0f);
	}

	if (g_aBoss.nCntAttackTime >= 200 && g_aBoss.nCntAttackTime <= 250)
	{
		g_aBoss.rot.y = g_aBoss.fVaim + D3DX_PI;

		if (g_aBoss.pos.x >= pPlayer->pos.x - 150.0f &&
			g_aBoss.pos.x <= pPlayer->pos.x + 150.0f &&
			g_aBoss.pos.z >= pPlayer->pos.z - 150.0f &&
			g_aBoss.pos.z <= pPlayer->pos.z + 150.0f)
		{
			g_aBoss.move = NULL_VECTOR3;
			g_bPurposeFlag[0] = true;
		}
		if (g_bPurposeFlag[0] == true)
		{
			g_aBoss.move = NULL_VECTOR3;
			g_aBoss.pos = D3DXVECTOR3(sinf(pPlayer->rot.y) * 100.0f + pPlayer->pos.x, 0.0f, cosf(pPlayer->rot.y) * 100.0f + pPlayer->pos.z);
		}
	}

	if (g_aBoss.nCntAttackTime == 255)
	{
		g_aBoss.state = BOSSSTATE_ATTACK;
		g_aBoss.fSaveAim = g_aBoss.fVaim;
		g_aBoss.rot.y = g_aBoss.fVaim + D3DX_PI;
	}

	if (g_aBoss.nCntAttackTime >= 291 && g_aBoss.nCntAttackTime <= 345)
	{
		float fRandPos = float(rand() % 500 + 1);
		float fRandY = float(rand() % 100 * 1);
		float fRandRot = float(rand() % 150 - 75) / 100;

		SetAttackbillboard(ATTACKBILLBOARD00_STUB, 60, 40.0f, 40.0f,
			D3DXVECTOR3(sinf(g_aBoss.fSaveAim + fRandRot) * fRandPos + g_aBoss.pos.x, fRandY + g_aBoss.pos.y, cosf(g_aBoss.fSaveAim + fRandRot) * fRandPos + g_aBoss.pos.z), NULL_VECTOR3, NULL_VECTOR3,
			0, 0.0f);
	}

	if (g_aBoss.nCntAttackTime == 340)
	{
		g_aBoss.bUnderAttack = false;
		g_aBoss.nCntAttackTime = 0;
	}

	g_aBoss.move.x += (0.0f - g_aBoss.move.x) * 0.1f;
	g_aBoss.move.z += (0.0f - g_aBoss.move.z) * 0.1f;
}

//============================================================================================================================================================================================================
//ボスの攻撃３：バードストライク
//============================================================================================================================================================================================================
void BossAttack3_BirdStrike(void)
{
	Player* pPlayer = GetPlayer();	//プレイヤーの情報を取得

	g_aBoss.nCntAttackTime++;

	if (g_aBoss.nCntAttackTime == 1)
	{
		g_aBoss.move = NULL_VECTOR3;
		g_bPurposeFlag[0] = false;
	}

	//==========================
	//３次元自機狙い処理
	//==========================
	g_aBoss.fVXaim3D = pPlayer->pos.x - g_aBoss.pos.x;
	g_aBoss.fVYaim3D = pPlayer->pos.y - g_aBoss.pos.y;
	g_aBoss.fVZaim3D = pPlayer->pos.z - g_aBoss.pos.z;

	D3DXVECTOR3 BulletMove = NULL_VECTOR3;

	g_aBoss.fVLaim3D = sqrtf((g_aBoss.fVXaim3D * g_aBoss.fVXaim3D) + (g_aBoss.fVYaim3D * g_aBoss.fVYaim3D)
		+ (g_aBoss.fVZaim3D * g_aBoss.fVZaim3D));

	BulletMove.x = g_aBoss.fVXaim3D / g_aBoss.fVLaim3D * 30.0f;
	BulletMove.y = g_aBoss.fVYaim3D / g_aBoss.fVLaim3D * 30.0f;
	BulletMove.z = g_aBoss.fVZaim3D / g_aBoss.fVLaim3D * 30.0f;

	if (g_aBoss.nCntAttackTime == 30)
	{
		g_aBoss.move.y = 30.0f;
	}

	if (g_aBoss.nCntAttackTime >= 50 && g_aBoss.nCntAttackTime <= 200)
	{
		g_aBoss.move = NULL_VECTOR3;

		if (g_nDelayBoss % 3 == 0)
		{
			float fRandRot = float(rand() % 200 + 1) / 100.0f;
			float fRandSpeed = float(rand() % 20 - 10);
		    
			SetBullet(BULLETTYPE_BOUND, 200, 20.0f, 20.0f, g_aBoss.pos,
				D3DXVECTOR3(sinf(D3DX_PI * fRandRot) * fRandSpeed, 5.0f, cosf(D3DX_PI * fRandRot) * fRandSpeed),
				NULL_VECTOR3, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0, 0.0f);
		}
	}

	if (g_aBoss.nCntAttackTime >= 201 && g_aBoss.nCntAttackTime <= 320)
	{
		if (g_aBoss.nCntAttackTime == 231)
		{
			g_aBoss.SaveMove3D = BulletMove;//この時点での３次元ベクトルを保存
		}

		g_aBoss.rot.y = g_aBoss.fVaim + D3DX_PI;
		g_aBoss.rot.x += 0.1f;
		g_aBoss.rot.z += 0.1f;


		g_aBoss.move = NULL_VECTOR3;
	}

	if (g_aBoss.nCntAttackTime >= 301 && g_aBoss.nCntAttackTime <= 321)
	{
		g_aBoss.move = g_aBoss.SaveMove3D;//プレイヤーに向かって突っ込む
	}

	if (g_aBoss.pos.y <= 0.0f && g_aBoss.nCntAttackTime >= 150 && g_bPurposeFlag[0] == false)
	{
		g_aBoss.move = NULL_VECTOR3;

		g_bPurposeFlag[0] = true;

		PlaySound(SOUND_LABEL_SE_BIRDSTRIKE_000);
		PlaySound(SOUND_LABEL_SE_THUNDER_000);
		for (int nCntThunder = 0; nCntThunder < 16; nCntThunder++)
		{
			float fRot = (2.0f / 16.0f) * nCntThunder;
			SetAttackModel(ATTACKMODEL06_THUNDER, 500, g_aBoss.pos,
				D3DXVECTOR3(sinf(D3DX_PI * fRot) * 10.0f, 0.0f, cosf(D3DX_PI * fRot) * 10.0f),
				NULL_VECTOR3,D3DXVECTOR3(1.5f,0.5f,0.5f), fRot, 0.0f, 0.0f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
		}
	}

	if (g_bPurposeFlag[0] == true)
	{
		g_aBoss.move = NULL_VECTOR3;
	}

	if (g_aBoss.nCntAttackTime == 400)
	{
		g_aBoss.bUnderAttack = false;
		g_aBoss.nCntAttackTime = 0;
		g_aBoss.rot = NULL_VECTOR3;
	}
}


//============================================================================================================================================================================================================
//ボス攻撃４：ボムレイン
//============================================================================================================================================================================================================
void BossAttack4_BombRain(void)
{
	Player* pPlayer = GetPlayer();	//プレイヤーの情報を取得

	g_aBoss.nCntAttackTime++;

	if (g_aBoss.nCntAttackTime == 1)
	{
		g_aBoss.move = NULL_VECTOR3;//移動量を０に
		g_bPurposeFlag[0] = false;//汎用的に使うフラグ
		g_aBoss.state = BOSSSTATE_ATTACK;//攻撃状態
	}


	if (g_aBoss.nCntAttackTime >= 150)
	{
		g_aBoss.state = BOSSSTATE_MOVE;
		g_aBoss.rot.y = g_aBoss.fVaim + D3DX_PI;
		
		if (g_aBoss.nCntAttackTime == 151)
		{
			g_aBoss.fRotMove = g_aBoss.fVaim;
		}

		if (g_aBoss.fRotMove < g_aBoss.fVaim)
		{
			g_aBoss.fRotMove += 0.05f;
		}
		else
		{
			g_aBoss.fRotMove -= 0.05f;

		}

		g_aBoss.move.x = sinf(g_aBoss.fRotMove) * 5.0f;
		g_aBoss.move.z = cosf(g_aBoss.fRotMove) * 5.0f;
		
		if (g_aBoss.fRotMove >= g_aBoss.fVaim - 0.3f && g_aBoss.fRotMove <= g_aBoss.fVaim + 0.3f)
		{
			g_aBoss.move.x = sinf(g_aBoss.fRotMove) * 10.0f;
			g_aBoss.move.z = cosf(g_aBoss.fRotMove) * 10.0f;
		}
	}

	float fRandLength = float(rand() % 1000 + 1);
	float fRandRot = float(rand() % 200 + 1) / 100;
	float fRandPosY = float(rand() % 500 + 400);

	if (g_aBoss.nCntAttackTime % 25 == 0)
	{
		SetAttackModel(ATTACKMODEL03_BOMB,300, D3DXVECTOR3(pPlayer->pos.x,300.0f, pPlayer->pos.z), NULL_VECTOR3, NULL_VECTOR3,D3DXVECTOR3(1.0f, 1.0f, 1.0f),0.0f,0.0f,0.0f,NULL_VECTOR3,0);

		SetAttackModel(ATTACKMODEL03_BOMB,300, D3DXVECTOR3(pPlayer->pos.x + sinf(D3DX_PI * fRandRot) * fRandLength, fRandPosY, pPlayer->pos.z + cosf(D3DX_PI * fRandRot) * fRandLength), NULL_VECTOR3, NULL_VECTOR3, D3DXVECTOR3(1.0f, 1.0f, 1.0f), 0.0f, 0.0f, 0.0f, NULL_VECTOR3, 0);
	}

	if (g_aBoss.nCntAttackTime == 1000)
	{
		g_bPurposeFlag[0] = false;//汎用的に使うフラグ
		g_aBoss.bUnderAttack = false;//攻撃しているかどうか
		g_aBoss.nCntAttackTime = 0;
	}
}

//============================================================================================================================================================================================================
//ボス攻撃５：クウェルジアース
//============================================================================================================================================================================================================
void BossAttack5_QuellTheEarth(void)
{
	g_aBoss.nCntAttackTime++;


	if (g_aBoss.nCntAttackTime == 1)
	{
		g_aBoss.move = NULL_VECTOR3;//移動量を０に
		g_bPurposeFlag[0] = false;//汎用的に使うフラグ
		g_nPurposeCnt = 0;//汎用カウント
	}

	if (g_aBoss.nCntAttackTime == 100)
	{
		g_aBoss.state = BOSSSTATE_ATTACK;
	}

	if (g_aBoss.nCntAttackTime == 180)
	{
		g_aBoss.rot.y = g_aBoss.fVaim + D3DX_PI;

		PlaySound(SOUND_LABEL_SE_SHOCKWAVE_000);

		SetAttackModel(ATTACKMODEL04_SHOCKWAVE,400,g_aBoss.pos,D3DXVECTOR3(sinf(g_aBoss.fVaim) * 12.0f,0.0f,cosf(g_aBoss.fVaim) * 12.0f),D3DXVECTOR3(g_aBoss.rot.x,g_aBoss.rot.y,g_aBoss.rot.z),D3DXVECTOR3(2.0f,2.0f,2.0f),0.0f,0.0f,0.0f,NULL_VECTOR3,ATTACKMODELBEHAVIOR00_NORMAL);
		SetAttackModel(ATTACKMODEL04_SHOCKWAVE, 400, g_aBoss.pos, D3DXVECTOR3(sinf(g_aBoss.fVaim + 0.2f) * 10.0f, 0.0f, cosf(g_aBoss.fVaim + 0.2f) * 10.0f), D3DXVECTOR3(g_aBoss.rot.x,g_aBoss.rot.y + 0.2f,g_aBoss.rot.z), D3DXVECTOR3(2.0f, 2.0f, 2.0f), 0.0f, 0.0f, 0.0f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
		SetAttackModel(ATTACKMODEL04_SHOCKWAVE, 400, g_aBoss.pos, D3DXVECTOR3(sinf(g_aBoss.fVaim - 0.2f) * 10.0f, 0.0f, cosf(g_aBoss.fVaim - 0.2f) * 10.0f), D3DXVECTOR3(g_aBoss.rot.x,g_aBoss.rot.y - 0.2f,g_aBoss.rot.z), D3DXVECTOR3(2.0f, 2.0f, 2.0f), 0.0f, 0.0f, 0.0f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
		SetAttackModel(ATTACKMODEL04_SHOCKWAVE, 400, g_aBoss.pos, D3DXVECTOR3(sinf(g_aBoss.fVaim + 0.4f) * 8.0f, 0.0f, cosf(g_aBoss.fVaim + 0.4f) * 8.0f), D3DXVECTOR3(g_aBoss.rot.x,g_aBoss.rot.y + 0.4f,g_aBoss.rot.z), D3DXVECTOR3(2.0f, 2.0f, 2.0f), 0.0f, 0.0f, 0.0f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
		SetAttackModel(ATTACKMODEL04_SHOCKWAVE, 400, g_aBoss.pos, D3DXVECTOR3(sinf(g_aBoss.fVaim - 0.4f) * 8.0f, 0.0f, cosf(g_aBoss.fVaim - 0.4f) * 8.0f), D3DXVECTOR3(g_aBoss.rot.x,g_aBoss.rot.y - 0.4f,g_aBoss.rot.z), D3DXVECTOR3(2.0f, 2.0f, 2.0f), 0.0f, 0.0f, 0.0f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
		SetAttackModel(ATTACKMODEL04_SHOCKWAVE, 400, g_aBoss.pos, D3DXVECTOR3(sinf(g_aBoss.fVaim + 0.6f) * 6.0f, 0.0f, cosf(g_aBoss.fVaim + 0.6f) * 6.0f), D3DXVECTOR3(g_aBoss.rot.x,g_aBoss.rot.y + 0.6f,g_aBoss.rot.z), D3DXVECTOR3(2.0f, 2.0f, 2.0f), 0.0f, 0.0f, 0.0f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
		SetAttackModel(ATTACKMODEL04_SHOCKWAVE, 400, g_aBoss.pos, D3DXVECTOR3(sinf(g_aBoss.fVaim - 0.6f) * 6.0f, 0.0f, cosf(g_aBoss.fVaim - 0.6f) * 6.0f), D3DXVECTOR3(g_aBoss.rot.x,g_aBoss.rot.y - 0.6f,g_aBoss.rot.z), D3DXVECTOR3(2.0f, 2.0f, 2.0f), 0.0f, 0.0f, 0.0f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
	}

	if (g_aBoss.nCntAttackTime == 270)
	{
		g_aBoss.state = BOSSSTATE_ATTACK;
		g_aBoss.rot.y = g_aBoss.fVaim + D3DX_PI;

		PlaySound(SOUND_LABEL_SE_SHOCKWAVE_000);

		SetAttackModel(ATTACKMODEL04_SHOCKWAVE, 400, g_aBoss.pos, D3DXVECTOR3(sinf(g_aBoss.fVaim) * 12.0f, 0.0f, cosf(g_aBoss.fVaim) * 212.0f), g_aBoss.rot, D3DXVECTOR3(2.0f, 2.0f, 2.0f), 0.0f, 0.0f, 0.0f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
		SetAttackModel(ATTACKMODEL04_SHOCKWAVE, 400, g_aBoss.pos, D3DXVECTOR3(sinf(g_aBoss.fVaim + 0.1f) * 10.0f, 0.0f, cosf(g_aBoss.fVaim + 0.1f) * 10.0f), g_aBoss.rot, D3DXVECTOR3(2.0f, 2.0f, 2.0f), 0.0f, 0.0f, 0.0f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
		SetAttackModel(ATTACKMODEL04_SHOCKWAVE, 400, g_aBoss.pos, D3DXVECTOR3(sinf(g_aBoss.fVaim - 0.1f) * 10.0f, 0.0f, cosf(g_aBoss.fVaim - 0.1f) * 10.0f), g_aBoss.rot, D3DXVECTOR3(2.0f, 2.0f, 2.0f), 0.0f, 0.0f, 0.0f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
		SetAttackModel(ATTACKMODEL04_SHOCKWAVE, 400, g_aBoss.pos, D3DXVECTOR3(sinf(g_aBoss.fVaim + 0.3f) * 8.0f, 0.0f, cosf(g_aBoss.fVaim + 0.3f) * 8.0f), g_aBoss.rot, D3DXVECTOR3(2.0f, 2.0f, 2.0f), 0.0f, 0.0f, 0.0f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
		SetAttackModel(ATTACKMODEL04_SHOCKWAVE, 400, g_aBoss.pos, D3DXVECTOR3(sinf(g_aBoss.fVaim - 0.3f) * 8.0f, 0.0f, cosf(g_aBoss.fVaim - 0.3f) * 8.0f), g_aBoss.rot, D3DXVECTOR3(2.0f, 2.0f, 2.0f), 0.0f, 0.0f, 0.0f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
		SetAttackModel(ATTACKMODEL04_SHOCKWAVE, 400, g_aBoss.pos, D3DXVECTOR3(sinf(g_aBoss.fVaim + 0.5f) * 6.0f, 0.0f, cosf(g_aBoss.fVaim + 0.5f) * 6.0f), g_aBoss.rot, D3DXVECTOR3(2.0f, 2.0f, 2.0f), 0.0f, 0.0f, 0.0f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
		SetAttackModel(ATTACKMODEL04_SHOCKWAVE, 400, g_aBoss.pos, D3DXVECTOR3(sinf(g_aBoss.fVaim - 0.5f) * 6.0f, 0.0f, cosf(g_aBoss.fVaim - 0.5f) * 6.0f), g_aBoss.rot, D3DXVECTOR3(2.0f, 2.0f, 2.0f), 0.0f, 0.0f, 0.0f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
	}

	if (g_aBoss.nCntAttackTime == 360)
	{
		g_aBoss.state = BOSSSTATE_ATTACK;
		g_aBoss.rot.y = g_aBoss.fVaim + D3DX_PI;

		PlaySound(SOUND_LABEL_SE_SHOCKWAVE_000);

		SetAttackModel(ATTACKMODEL04_SHOCKWAVE, 400, g_aBoss.pos, D3DXVECTOR3(sinf(g_aBoss.fVaim) * 12.0f, 0.0f, cosf(g_aBoss.fVaim) * 12.0f), D3DXVECTOR3(g_aBoss.rot.x, g_aBoss.rot.y, g_aBoss.rot.z), D3DXVECTOR3(2.0f, 2.0f, 2.0f), 0.0f, 0.0f, 20.0f, NULL_VECTOR3, ATTACKMODELBEHAVIOR01_ANOTHER);
		SetAttackModel(ATTACKMODEL04_SHOCKWAVE, 400, g_aBoss.pos, D3DXVECTOR3(sinf(g_aBoss.fVaim + 0.2f) * 10.0f, 0.0f, cosf(g_aBoss.fVaim + 0.2f) * 10.0f), D3DXVECTOR3(g_aBoss.rot.x, g_aBoss.rot.y + 0.2f, g_aBoss.rot.z), D3DXVECTOR3(2.0f, 2.0f, 2.0f), 0.0f, 0.0f, 16.0f, NULL_VECTOR3, ATTACKMODELBEHAVIOR01_ANOTHER);
		SetAttackModel(ATTACKMODEL04_SHOCKWAVE, 400, g_aBoss.pos, D3DXVECTOR3(sinf(g_aBoss.fVaim - 0.2f) * 10.0f, 0.0f, cosf(g_aBoss.fVaim - 0.2f) * 10.0f), D3DXVECTOR3(g_aBoss.rot.x, g_aBoss.rot.y - 0.2f, g_aBoss.rot.z), D3DXVECTOR3(2.0f, 2.0f, 2.0f), 0.0f, 0.0f, 16.0f, NULL_VECTOR3, ATTACKMODELBEHAVIOR01_ANOTHER);
		SetAttackModel(ATTACKMODEL04_SHOCKWAVE, 400, g_aBoss.pos, D3DXVECTOR3(sinf(g_aBoss.fVaim + 0.4f) * 8.0f, 0.0f, cosf(g_aBoss.fVaim + 0.4f) * 8.0f), D3DXVECTOR3(g_aBoss.rot.x, g_aBoss.rot.y + 0.4f, g_aBoss.rot.z), D3DXVECTOR3(2.0f, 2.0f, 2.0f), 0.0f, 0.0f, 12.0f, NULL_VECTOR3, ATTACKMODELBEHAVIOR01_ANOTHER);
		SetAttackModel(ATTACKMODEL04_SHOCKWAVE, 400, g_aBoss.pos, D3DXVECTOR3(sinf(g_aBoss.fVaim - 0.4f) * 8.0f, 0.0f, cosf(g_aBoss.fVaim - 0.4f) * 8.0f), D3DXVECTOR3(g_aBoss.rot.x, g_aBoss.rot.y - 0.4f, g_aBoss.rot.z), D3DXVECTOR3(2.0f, 2.0f, 2.0f), 0.0f, 0.0f, 12.0f, NULL_VECTOR3, ATTACKMODELBEHAVIOR01_ANOTHER);
		SetAttackModel(ATTACKMODEL04_SHOCKWAVE, 400, g_aBoss.pos, D3DXVECTOR3(sinf(g_aBoss.fVaim + 0.6f) * 6.0f, 0.0f, cosf(g_aBoss.fVaim + 0.6f) * 6.0f), D3DXVECTOR3(g_aBoss.rot.x, g_aBoss.rot.y + 0.6f, g_aBoss.rot.z), D3DXVECTOR3(2.0f, 2.0f, 2.0f), 0.0f, 0.0f, 8.0f, NULL_VECTOR3, ATTACKMODELBEHAVIOR01_ANOTHER);
		SetAttackModel(ATTACKMODEL04_SHOCKWAVE, 400, g_aBoss.pos, D3DXVECTOR3(sinf(g_aBoss.fVaim - 0.6f) * 6.0f, 0.0f, cosf(g_aBoss.fVaim - 0.6f) * 6.0f), D3DXVECTOR3(g_aBoss.rot.x, g_aBoss.rot.y - 0.6f, g_aBoss.rot.z), D3DXVECTOR3(2.0f, 2.0f, 2.0f), 0.0f, 0.0f, 8.0f, NULL_VECTOR3, ATTACKMODELBEHAVIOR01_ANOTHER);
	}

	if (g_aBoss.nCntAttackTime == 450)
	{
		g_aBoss.state = BOSSSTATE_ATTACK;
		g_aBoss.rot.y = g_aBoss.fVaim + D3DX_PI;

		PlaySound(SOUND_LABEL_SE_SHOCKWAVE_000);

		SetAttackModel(ATTACKMODEL04_SHOCKWAVE, 400, g_aBoss.pos, D3DXVECTOR3(sinf(g_aBoss.fVaim) * 12.0f, 0.0f, cosf(g_aBoss.fVaim) * 12.0f), D3DXVECTOR3(g_aBoss.rot.x, g_aBoss.rot.y, g_aBoss.rot.z), D3DXVECTOR3(2.0f, 2.0f, 2.0f), 0.0f, 0.0f, 20.0f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
		SetAttackModel(ATTACKMODEL04_SHOCKWAVE, 400, g_aBoss.pos, D3DXVECTOR3(sinf(g_aBoss.fVaim + 0.2f) * 12.0f, 0.0f, cosf(g_aBoss.fVaim + 0.2f) * 12.0f), D3DXVECTOR3(g_aBoss.rot.x, g_aBoss.rot.y + 0.2f, g_aBoss.rot.z), D3DXVECTOR3(2.0f, 2.0f, 2.0f), 0.0f, 0.0f, 20.0f, NULL_VECTOR3, ATTACKMODELBEHAVIOR01_ANOTHER);
		SetAttackModel(ATTACKMODEL04_SHOCKWAVE, 400, g_aBoss.pos, D3DXVECTOR3(sinf(g_aBoss.fVaim - 0.2f) * 12.0f, 0.0f, cosf(g_aBoss.fVaim - 0.2f) * 12.0f), D3DXVECTOR3(g_aBoss.rot.x, g_aBoss.rot.y - 0.2f, g_aBoss.rot.z), D3DXVECTOR3(2.0f, 2.0f, 2.0f), 0.0f, 0.0f, 20.0f, NULL_VECTOR3, ATTACKMODELBEHAVIOR01_ANOTHER);
		SetAttackModel(ATTACKMODEL04_SHOCKWAVE, 400, g_aBoss.pos, D3DXVECTOR3(sinf(g_aBoss.fVaim + 0.4f) * 12.0f, 0.0f, cosf(g_aBoss.fVaim + 0.4f) * 12.0f), D3DXVECTOR3(g_aBoss.rot.x, g_aBoss.rot.y + 0.4f, g_aBoss.rot.z), D3DXVECTOR3(2.0f, 2.0f, 2.0f), 0.0f, 0.0f, 20.0f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
		SetAttackModel(ATTACKMODEL04_SHOCKWAVE, 400, g_aBoss.pos, D3DXVECTOR3(sinf(g_aBoss.fVaim - 0.4f) * 12.0f, 0.0f, cosf(g_aBoss.fVaim - 0.4f) * 12.0f), D3DXVECTOR3(g_aBoss.rot.x, g_aBoss.rot.y - 0.4f, g_aBoss.rot.z), D3DXVECTOR3(2.0f, 2.0f, 2.0f), 0.0f, 0.0f, 20.0f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
		SetAttackModel(ATTACKMODEL04_SHOCKWAVE, 400, g_aBoss.pos, D3DXVECTOR3(sinf(g_aBoss.fVaim + 0.6f) * 12.0f, 0.0f, cosf(g_aBoss.fVaim + 0.6f) * 12.0f), D3DXVECTOR3(g_aBoss.rot.x, g_aBoss.rot.y + 0.6f, g_aBoss.rot.z), D3DXVECTOR3(2.0f, 2.0f, 2.0f), 0.0f, 0.0f, 20.0f, NULL_VECTOR3, ATTACKMODELBEHAVIOR01_ANOTHER);
		SetAttackModel(ATTACKMODEL04_SHOCKWAVE, 400, g_aBoss.pos, D3DXVECTOR3(sinf(g_aBoss.fVaim - 0.6f) * 12.0f, 0.0f, cosf(g_aBoss.fVaim - 0.6f) * 12.0f), D3DXVECTOR3(g_aBoss.rot.x, g_aBoss.rot.y - 0.6f, g_aBoss.rot.z), D3DXVECTOR3(2.0f, 2.0f, 2.0f), 0.0f, 0.0f, 20.0f, NULL_VECTOR3, ATTACKMODELBEHAVIOR01_ANOTHER);
	}

	if (g_aBoss.nCntAttackTime >= 530 && g_aBoss.nCntAttackTime <= 560)
	{
		g_aBoss.state = BOSSSTATE_MOVE;
		g_aBoss.move.x = sinf(g_aBoss.fVaim) * 20.0f;
		g_aBoss.move.z = cosf(g_aBoss.fVaim) * 20.0f;
	}

	if (g_aBoss.nCntAttackTime == 530)
	{
		g_aBoss.move.y = 50.0f;
	}

	if (g_aBoss.nCntAttackTime >= 531 && g_aBoss.nCntAttackTime <= 700 && g_aBoss.pos.y <= 0.0f)
	{
		g_bPurposeFlag[0] = true;
	}

	if (g_bPurposeFlag[0] == true)
	{
		g_nPurposeCnt++;
	}

	if (g_nPurposeCnt == 50)
	{
		PlaySound(SOUND_LABEL_SE_BIRDSTRIKE_000);
		
		SetAttackModel(ATTACKMODEL02_SPHERE, 100, D3DXVECTOR3(sinf(D3DX_PI * 0.0f) * 200.0f + g_aBoss.pos.x, 0.0f, cosf(D3DX_PI * 0.0f) * 200.0f + g_aBoss.pos.z), NULL_VECTOR3, NULL_VECTOR3,
			NULL_VECTOR3, 0.0f, 0.0f, 0.1f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
		SetAttackModel(ATTACKMODEL02_SPHERE, 100, D3DXVECTOR3(sinf(D3DX_PI * 0.5f) * 200.0f + g_aBoss.pos.x, 0.0f, cosf(D3DX_PI * 0.5f) * 200.0f + g_aBoss.pos.z), NULL_VECTOR3, NULL_VECTOR3,
			NULL_VECTOR3, 0.0f, 0.0f, 0.1f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
		SetAttackModel(ATTACKMODEL02_SPHERE, 100, D3DXVECTOR3(sinf(D3DX_PI * 1.0f) * 200.0f + g_aBoss.pos.x, 0.0f, cosf(D3DX_PI * 1.0f) * 200.0f + g_aBoss.pos.z), NULL_VECTOR3, NULL_VECTOR3,
			NULL_VECTOR3, 0.0f, 0.0f, 0.1f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
		SetAttackModel(ATTACKMODEL02_SPHERE, 100, D3DXVECTOR3(sinf(D3DX_PI * 1.5f) * 200.0f + g_aBoss.pos.x, 0.0f, cosf(D3DX_PI * 1.5f) * 200.0f + g_aBoss.pos.z), NULL_VECTOR3, NULL_VECTOR3,
			NULL_VECTOR3, 0.0f, 0.0f, 0.1f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
	}
	if (g_nPurposeCnt == 120)
	{
		PlaySound(SOUND_LABEL_SE_BIRDSTRIKE_000);

		SetAttackModel(ATTACKMODEL02_SPHERE, 100, D3DXVECTOR3(sinf(D3DX_PI * 0.25f) * 400.0f + g_aBoss.pos.x, 0.0f, cosf(D3DX_PI * 0.25f) * 400.0f + g_aBoss.pos.z), NULL_VECTOR3, NULL_VECTOR3,
			NULL_VECTOR3, 0.0f, 0.0f, 0.2f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
		SetAttackModel(ATTACKMODEL02_SPHERE, 100, D3DXVECTOR3(sinf(D3DX_PI * 0.75f) * 400.0f + g_aBoss.pos.x, 0.0f, cosf(D3DX_PI * 0.75f) * 400.0f + g_aBoss.pos.z), NULL_VECTOR3, NULL_VECTOR3,
			NULL_VECTOR3, 0.0f, 0.0f, 0.2f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
		SetAttackModel(ATTACKMODEL02_SPHERE, 100, D3DXVECTOR3(sinf(D3DX_PI * 1.25f) * 400.0f + g_aBoss.pos.x, 0.0f, cosf(D3DX_PI * 1.25f) * 400.0f + g_aBoss.pos.z), NULL_VECTOR3, NULL_VECTOR3,
			NULL_VECTOR3, 0.0f, 0.0f, 0.2f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
		SetAttackModel(ATTACKMODEL02_SPHERE, 100, D3DXVECTOR3(sinf(D3DX_PI * 1.75f) * 400.0f + g_aBoss.pos.x, 0.0f, cosf(D3DX_PI * 1.75f) * 400.0f + g_aBoss.pos.z), NULL_VECTOR3, NULL_VECTOR3,
			NULL_VECTOR3, 0.0f, 0.0f, 0.2f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
	}
	if (g_nPurposeCnt == 190)
	{
		PlaySound(SOUND_LABEL_SE_BIRDSTRIKE_000);

		SetAttackModel(ATTACKMODEL02_SPHERE, 100, D3DXVECTOR3(sinf(D3DX_PI * 0.0f) * 600.0f + g_aBoss.pos.x, 0.0f, cosf(D3DX_PI * 0.0f) * 600.0f + g_aBoss.pos.z), NULL_VECTOR3, NULL_VECTOR3,
			NULL_VECTOR3, 0.0f, 0.0f, 0.3f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
		SetAttackModel(ATTACKMODEL02_SPHERE, 100, D3DXVECTOR3(sinf(D3DX_PI * 0.5f) * 600.0f + g_aBoss.pos.x, 0.0f, cosf(D3DX_PI * 0.5f) * 600.0f + g_aBoss.pos.z), NULL_VECTOR3, NULL_VECTOR3,
			NULL_VECTOR3, 0.0f, 0.0f, 0.3f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
		SetAttackModel(ATTACKMODEL02_SPHERE, 100, D3DXVECTOR3(sinf(D3DX_PI * 1.0f) * 600.0f + g_aBoss.pos.x, 0.0f, cosf(D3DX_PI * 1.0f) * 600.0f + g_aBoss.pos.z), NULL_VECTOR3, NULL_VECTOR3,
			NULL_VECTOR3, 0.0f, 0.0f, 0.3f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
		SetAttackModel(ATTACKMODEL02_SPHERE, 100, D3DXVECTOR3(sinf(D3DX_PI * 1.5f) * 600.0f + g_aBoss.pos.x, 0.0f, cosf(D3DX_PI * 1.5f) * 600.0f + g_aBoss.pos.z), NULL_VECTOR3, NULL_VECTOR3,
			NULL_VECTOR3, 0.0f, 0.0f, 0.3f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
	}
	if (g_nPurposeCnt == 260)
	{
		PlaySound(SOUND_LABEL_SE_BIRDSTRIKE_000);

		SetAttackModel(ATTACKMODEL02_SPHERE, 100, D3DXVECTOR3(sinf(D3DX_PI * 0.25f) * 800.0f + g_aBoss.pos.x, 0.0f, cosf(D3DX_PI * 0.25f) * 800.0f + g_aBoss.pos.z), NULL_VECTOR3, NULL_VECTOR3,
			NULL_VECTOR3, 0.0f, 0.0f, 0.4f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
		SetAttackModel(ATTACKMODEL02_SPHERE, 100, D3DXVECTOR3(sinf(D3DX_PI * 0.75f) * 800.0f + g_aBoss.pos.x, 0.0f, cosf(D3DX_PI * 0.75f) * 800.0f + g_aBoss.pos.z), NULL_VECTOR3, NULL_VECTOR3,
			NULL_VECTOR3, 0.0f, 0.0f, 0.4f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
		SetAttackModel(ATTACKMODEL02_SPHERE, 100, D3DXVECTOR3(sinf(D3DX_PI * 1.25f) * 800.0f + g_aBoss.pos.x, 0.0f, cosf(D3DX_PI * 1.25f) * 800.0f + g_aBoss.pos.z), NULL_VECTOR3, NULL_VECTOR3,
			NULL_VECTOR3, 0.0f, 0.0f, 0.4f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
		SetAttackModel(ATTACKMODEL02_SPHERE, 100, D3DXVECTOR3(sinf(D3DX_PI * 1.75f) * 800.0f + g_aBoss.pos.x, 0.0f, cosf(D3DX_PI * 1.75f) * 800.0f + g_aBoss.pos.z), NULL_VECTOR3, NULL_VECTOR3,
			NULL_VECTOR3, 0.0f, 0.0f, 0.4f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
	}

	if (g_aBoss.nCntAttackTime == 1100)
	{
		g_bPurposeFlag[0] = false;//汎用的に使うフラグ
		g_aBoss.bUnderAttack = false;//攻撃しているかどうか
		g_aBoss.nCntAttackTime = 0;
		g_nPurposeCnt = 0;//汎用カウント
	}

	g_aBoss.move.x += (0.0f - g_aBoss.move.x) * 0.1f;
	g_aBoss.move.z += (0.0f - g_aBoss.move.z) * 0.1f;
}

//============================================================================================================================================================================================================
//ボス攻撃７：スレンダートルネード
//============================================================================================================================================================================================================
void BossAttack7_SlanderTornado(void)
{
	Player* pPlayer = GetPlayer();	//プレイヤーの情報を取得
	int nLottery = rand() % 2;

	g_aBoss.nCntAttackTime++;

	if (g_aBoss.nCntAttackTime == 1)
	{
		g_aBoss.move = NULL_VECTOR3;//移動量を０に
		g_bPurposeFlag[0] = false;//汎用的に使うフラグ０
		g_bPurposeFlag[1] = false;//汎用的に使うフラグ１
		g_bPurposeFlag[2] = false;//汎用的に使うフラグ２
 		g_nPurposeCnt = 0;//汎用カウント
	}

	if (g_aBoss.nCntAttackTime == 100)
	{
		g_aBoss.state = BOSSSTATE_ATTACK;

		PlaySound(SOUND_LABEL_SE_WIND_000);

		for (int nCntAttack = 0; nCntAttack < 8; nCntAttack++)
		{
			float fRot = (2.0f / 8.0f) * float(nCntAttack);
			SetAttackModel(ATTACKMODEL05_TORNADO, 300, g_aBoss.pos, NULL_VECTOR3, NULL_VECTOR3, D3DXVECTOR3(2.0f, 2.0f, 2.0f),
				fRot,0.002f * float(nCntAttack),0.1f, g_aBoss.pos, ATTACKMODELBEHAVIOR00_NORMAL);
		}
	}

	if (g_aBoss.nCntAttackTime == 200)
	{
		if(nLottery == 0)
		{
			g_bPurposeFlag[0] = false;
		}
		else
		{
			g_bPurposeFlag[0] = true;
		}
	}

	float fXVaim = (pPlayer->pos.x + 500.0f) - g_aBoss.pos.x;
	float fZVaim = pPlayer->pos.z - g_aBoss.pos.z;
	float fVaim = atan2f(fXVaim, fZVaim);

	float fXVaim2 = (pPlayer->pos.x - 500.0f) - g_aBoss.pos.x;
	float fZVaim2 = pPlayer->pos.z - g_aBoss.pos.z;
	float fVaim2 = atan2f(fXVaim2, fZVaim2);

	if (g_aBoss.nCntAttackTime >= 200 && g_aBoss.nCntAttackTime <= 400)
	{
		g_aBoss.state = BOSSSTATE_MOVE;

		if (g_bPurposeFlag[0] == false)
		{
		
			g_aBoss.move.x = sinf(fVaim) * 30.0f;
			g_aBoss.move.z = cosf(fVaim) * 30.0f;
		}
		else
		{
			g_aBoss.move.x = sinf(fVaim2) * 30.0f;
			g_aBoss.move.z = cosf(fVaim2) * 30.0f;
		}
	}

	if (g_aBoss.nCntAttackTime >= 350 && g_aBoss.nCntAttackTime <= 400)
	{
		if (g_bPurposeFlag[0] == false)
		{
			g_aBoss.rot.y = g_aBoss.fVaim + D3DX_PI;
			g_aBoss.state = BOSSSTATE_ATTACK;

			if (g_aBoss.pos.x >= pPlayer->pos.x + 500.0f - 50.0f && g_aBoss.pos.x <= pPlayer->pos.x + 500.0f + 50.0f &&
				g_aBoss.pos.z >= pPlayer->pos.z - 50.0f && g_aBoss.pos.z <= pPlayer->pos.z + 50.0f)
			{
				g_aBoss.move = NULL_VECTOR3;
				g_bPurposeFlag[1] = true;
			}

			if (g_bPurposeFlag[1] == true && g_bPurposeFlag[2] == false)
			{
				PlaySound(SOUND_LABEL_SE_WIND_000);


				SetAttackModel(ATTACKMODEL05_TORNADO, 300, D3DXVECTOR3(g_aBoss.pos.x, g_aBoss.pos.y, g_aBoss.pos.z),
					D3DXVECTOR3(sinf(D3DX_PI * 1.5f) * 4.0f, 0.0f, cosf(D3DX_PI * 1.5f) * 4.0f), NULL_VECTOR3, D3DXVECTOR3(2.0f,1.0f, 1.0f),
					0.0f, 0.0f, 0.1f, g_aBoss.pos, ATTACKMODELBEHAVIOR01_ANOTHER);

				for (int nCntAttack = 0; nCntAttack < 8; nCntAttack++)
				{
					if (nCntAttack % 3 == 0)
					{
						SetAttackModel(ATTACKMODEL05_TORNADO, 300, D3DXVECTOR3(g_aBoss.pos.x, g_aBoss.pos.y, g_aBoss.pos.z + 100.0f + (100.0f * nCntAttack)),
							D3DXVECTOR3(sinf(D3DX_PI * 1.5f) * 4.0f, 0.0f, cosf(D3DX_PI * 1.5f) * 4.0f), NULL_VECTOR3, D3DXVECTOR3(2.0f, 1.0f, 1.0f),
							0.0f, 0.002f * float(nCntAttack), 0.1f, g_aBoss.pos, ATTACKMODELBEHAVIOR01_ANOTHER);
					}
					else
					{
						SetAttackModel(ATTACKMODEL05_TORNADO, 300, D3DXVECTOR3(g_aBoss.pos.x, g_aBoss.pos.y, g_aBoss.pos.z - 100.0f - (100.0f * nCntAttack)),
							D3DXVECTOR3(sinf(D3DX_PI * 1.5f) * 4.0f, 0.0f, cosf(D3DX_PI * 1.5f) * 4.0f), NULL_VECTOR3, D3DXVECTOR3(2.0f, 1.0f, 1.0f),
							0.0f, 0.002f * float(nCntAttack), 0.1f, g_aBoss.pos, ATTACKMODELBEHAVIOR01_ANOTHER);
					}
				}

				g_bPurposeFlag[2] = true;
			}
		}
		else
		{
			g_aBoss.rot.y = g_aBoss.fVaim + D3DX_PI;

			g_aBoss.state = BOSSSTATE_ATTACK;

			if (g_aBoss.pos.x >= pPlayer->pos.x - 500.0f - 50.0f && g_aBoss.pos.x <= pPlayer->pos.x - 500.0f + 50.0f &&
				g_aBoss.pos.z >= pPlayer->pos.z - 50.0f && g_aBoss.pos.z <= pPlayer->pos.z + 50.0f)
			{
				g_aBoss.move = NULL_VECTOR3;
				g_bPurposeFlag[1] = true;
			}

			if (g_bPurposeFlag[1] == true && g_bPurposeFlag[2] == false)
			{
				PlaySound(SOUND_LABEL_SE_WIND_000);

				SetAttackModel(ATTACKMODEL05_TORNADO, 300, D3DXVECTOR3(g_aBoss.pos.x, g_aBoss.pos.y, g_aBoss.pos.z),
					D3DXVECTOR3(sinf(D3DX_PI * 0.5f) * 4.0f, 0.0f, cosf(D3DX_PI * 0.5f) * 4.0f), NULL_VECTOR3, D3DXVECTOR3(2.0f, 1.0f, 1.0f),
					0.0f, 0.0f, 0.1f, g_aBoss.pos, ATTACKMODELBEHAVIOR01_ANOTHER);

				for (int nCntAttack = 0; nCntAttack < 8; nCntAttack++)
				{
					if (nCntAttack % 3 == 0)
					{
						SetAttackModel(ATTACKMODEL05_TORNADO, 300, D3DXVECTOR3(g_aBoss.pos.x, g_aBoss.pos.y, g_aBoss.pos.z + 100.0f + (100.0f * nCntAttack)),
							D3DXVECTOR3(sinf(D3DX_PI * 0.5f) * 4.0f, 0.0f, cosf(D3DX_PI * 0.5f) * 4.0f), NULL_VECTOR3, D3DXVECTOR3(2.0f, 1.0f, 1.0f),
							0.0f, 0.002f * float(nCntAttack), 0.1f, g_aBoss.pos, ATTACKMODELBEHAVIOR01_ANOTHER);
					}
					else
					{
						SetAttackModel(ATTACKMODEL05_TORNADO, 300, D3DXVECTOR3(g_aBoss.pos.x, g_aBoss.pos.y, g_aBoss.pos.z - 100.0f - (100.0f * nCntAttack)),
							D3DXVECTOR3(sinf(D3DX_PI * 0.5f) * 4.0f, 0.0f, cosf(D3DX_PI * 0.5f) * 4.0f), NULL_VECTOR3, D3DXVECTOR3(2.0f, 1.0f, 1.0f),
							0.0f, 0.002f * float(nCntAttack), 0.1f, g_aBoss.pos, ATTACKMODELBEHAVIOR01_ANOTHER);
					}
				}

				g_bPurposeFlag[2] = true;
			}
		}
	}

	if (g_aBoss.nCntAttackTime == 400)
	{
		g_bPurposeFlag[0] = false;//汎用的に使うフラグ
		g_bPurposeFlag[1] = false;//汎用的に使うフラグ１
		g_bPurposeFlag[2] = false;//汎用的に使うフラグ２
	}

	float fZVaim3 = (pPlayer->pos.z + 500.0f) - g_aBoss.pos.z;
	float fXVaim3 = pPlayer->pos.x - g_aBoss.pos.x;
	float fVaim3 = atan2f(fXVaim3, fZVaim3);

	float fZVaim4 = (pPlayer->pos.z - 500.0f) - g_aBoss.pos.z;
	float fXVaim4 = pPlayer->pos.x - g_aBoss.pos.x;
	float fVaim4 = atan2f(fXVaim4, fZVaim4);

	if (g_aBoss.nCntAttackTime == 470)
	{
		if (nLottery == 0)
		{
			g_bPurposeFlag[0] = false;
		}
		else
		{
			g_bPurposeFlag[0] = true;
		}
	}

	if (g_aBoss.nCntAttackTime >= 470 && g_aBoss.nCntAttackTime <= 670)
	{
		g_aBoss.state = BOSSSTATE_MOVE;

		if (g_bPurposeFlag[0] == false)
		{

			g_aBoss.move.x = sinf(fVaim3) * 40.0f;
			g_aBoss.move.z = cosf(fVaim3) * 40.0f;
		}
		else
		{
			g_aBoss.move.x = sinf(fVaim4) * 40.0f;
			g_aBoss.move.z = cosf(fVaim4) * 40.0f;
		}
	}

	if (g_aBoss.nCntAttackTime >= 470 && g_aBoss.nCntAttackTime <= 670)
	{
		if (g_bPurposeFlag[0] == false)
		{
			g_aBoss.rot.y = g_aBoss.fVaim + D3DX_PI;
			g_aBoss.state = BOSSSTATE_ATTACK;

			if (g_aBoss.pos.x >= pPlayer->pos.x - 50.0f && g_aBoss.pos.x <= pPlayer->pos.x + 50.0f &&
				g_aBoss.pos.z >= pPlayer->pos.z + 500.0f - 50.0f && g_aBoss.pos.z <= pPlayer->pos.z + 500.0f + 50.0f)
			{
				g_aBoss.move = NULL_VECTOR3;
				g_bPurposeFlag[1] = true;
			}

			if (g_bPurposeFlag[1] == true && g_bPurposeFlag[2] == false)
			{
				PlaySound(SOUND_LABEL_SE_WIND_000);

				SetAttackModel(ATTACKMODEL05_TORNADO, 300, D3DXVECTOR3(g_aBoss.pos.x, g_aBoss.pos.y, g_aBoss.pos.z),
					D3DXVECTOR3(sinf(D3DX_PI) * 4.0f, 0.0f, cosf(D3DX_PI) * 4.0f), NULL_VECTOR3, D3DXVECTOR3(2.0f, 1.0f, 1.0f),
					0.0f, 0.0f, 0.1f, g_aBoss.pos, ATTACKMODELBEHAVIOR01_ANOTHER);

				for (int nCntAttack = 0; nCntAttack < 8; nCntAttack++)
				{
					if (nCntAttack % 3 == 0)
					{
						SetAttackModel(ATTACKMODEL05_TORNADO, 300, D3DXVECTOR3(g_aBoss.pos.x + 100.0f + (100.0f * nCntAttack), g_aBoss.pos.y, g_aBoss.pos.z),
							D3DXVECTOR3(sinf(D3DX_PI + 0.2f) * 4.0f, 0.0f, cosf(D3DX_PI + 0.2f) * 4.0f), NULL_VECTOR3, D3DXVECTOR3(2.0f, 1.0f, 1.0f),
							0.0f, 0.002f * float(nCntAttack), 0.1f, g_aBoss.pos, ATTACKMODELBEHAVIOR01_ANOTHER);
					}
					else
					{
						SetAttackModel(ATTACKMODEL05_TORNADO, 300, D3DXVECTOR3(g_aBoss.pos.x - 100.0f - (100.0f * nCntAttack), g_aBoss.pos.y, g_aBoss.pos.z),
							D3DXVECTOR3(sinf(D3DX_PI - 0.2f) * 4.0f, 0.0f, cosf(D3DX_PI - 0.2f) * 4.0f), NULL_VECTOR3, D3DXVECTOR3(2.0f, 1.0f, 1.0f),
							0.0f, 0.002f * float(nCntAttack), 0.1f, g_aBoss.pos, ATTACKMODELBEHAVIOR01_ANOTHER);
					}
				}

				g_bPurposeFlag[2] = true;
			}
		}
		else
		{
			g_aBoss.rot.y = g_aBoss.fVaim + D3DX_PI;

			g_aBoss.state = BOSSSTATE_ATTACK;

			if (g_aBoss.pos.x >= pPlayer->pos.x - 50.0f && g_aBoss.pos.x <= pPlayer->pos.x + 50.0f &&
				g_aBoss.pos.z >= pPlayer->pos.z - 500.0f - 50.0f && g_aBoss.pos.z <= pPlayer->pos.z - 500.0f + 50.0f)
			{
				g_aBoss.move = NULL_VECTOR3;
				g_bPurposeFlag[1] = true;
			}

			if (g_bPurposeFlag[1] == true && g_bPurposeFlag[2] == false)
			{
				PlaySound(SOUND_LABEL_SE_WIND_000);

				SetAttackModel(ATTACKMODEL05_TORNADO, 300, D3DXVECTOR3(g_aBoss.pos.x, g_aBoss.pos.y, g_aBoss.pos.z),
					D3DXVECTOR3(sinf(0.0f) * 4.0f, 0.0f, cosf(0.0f) * 4.0f), NULL_VECTOR3, D3DXVECTOR3(2.0f, 1.0f, 1.0f),
					0.0f, 0.0f, 0.1f, g_aBoss.pos, ATTACKMODELBEHAVIOR01_ANOTHER);

				for (int nCntAttack = 0; nCntAttack < 8; nCntAttack++)
				{
					if (nCntAttack % 3 == 0)
					{
						SetAttackModel(ATTACKMODEL05_TORNADO, 300, D3DXVECTOR3(g_aBoss.pos.x + 100.0f + (100.0f * nCntAttack), g_aBoss.pos.y, g_aBoss.pos.z),
							D3DXVECTOR3(sinf(0.0f + 0.2f) * 4.0f, 0.0f, cosf(0.0f + 0.2f) * 4.0f), NULL_VECTOR3, D3DXVECTOR3(2.0f, 1.0f, 1.0f),
							0.0f, 0.002f * float(nCntAttack), 0.1f, g_aBoss.pos, ATTACKMODELBEHAVIOR01_ANOTHER);
					}
					else
					{
						SetAttackModel(ATTACKMODEL05_TORNADO, 300, D3DXVECTOR3(g_aBoss.pos.x - 100.0f - (100.0f * nCntAttack), g_aBoss.pos.y, g_aBoss.pos.z),
							D3DXVECTOR3(sinf(0.0f - 0.2f) * 4.0f, 0.0f, cosf(0.0f - 0.2f) * 4.0f), NULL_VECTOR3, D3DXVECTOR3(2.0f, 1.0f, 1.0f),
							0.0f, 0.002f * float(nCntAttack), 0.1f, g_aBoss.pos, ATTACKMODELBEHAVIOR01_ANOTHER);
					}
				}

				g_bPurposeFlag[2] = true;
			}
		}
	}

	if (g_aBoss.nCntAttackTime == 680)
	{
		g_bPurposeFlag[0] = false;//汎用的に使うフラグ
		g_bPurposeFlag[1] = false;//汎用的に使うフラグ１
		g_bPurposeFlag[2] = false;//汎用的に使うフラグ２

		g_aBoss.bUnderAttack = false;//攻撃しているかどうか
		g_aBoss.nCntAttackTime = 0;
		g_nPurposeCnt = 0;//汎用カウント
	}

	g_aBoss.move.x += (0.0f - g_aBoss.move.x) * 0.1f;
	g_aBoss.move.z += (0.0f - g_aBoss.move.z) * 0.1f;
}

//============================================================================================================================================================================================================
//ボス攻撃６：バーニングチキン
//============================================================================================================================================================================================================
void BossAttack6_BurningChicken(void)//ボス攻撃６：バーニングチキン
{
	g_aBoss.nCntAttackTime++;

	if (g_aBoss.nCntAttackTime == 1)
	{
		g_aBoss.move = NULL_VECTOR3;//移動量を０に
		g_bPurposeFlag[0] = false;//汎用的に使うフラグ０
		g_bPurposeFlag[1] = false;//汎用的に使うフラグ１
		g_bPurposeFlag[2] = false;//汎用的に使うフラグ２
		g_nPurposeCnt = 0;//汎用カウント
		g_aBoss.state = BOSSSTATE_ATTACK;
	}

	if (g_aBoss.nCntAttackTime % 15 == 0 && g_aBoss.nCntAttackTime <= 200)
	{
		PlaySound(SOUND_LABEL_SE_FIRE_000);

		SetAttackbillboard(ATTACKBILLBORAD02_FIRE, 300, 40.0f, 100.0f,
			D3DXVECTOR3(sinf(g_aBoss.fVaim + 0.4f) * (100.0f * float(g_nPurposeCnt)) + g_aBoss.pos.x,50.0f, cosf(g_aBoss.fVaim + 0.4f) * (100.0f * float(g_nPurposeCnt)) + g_aBoss.pos.z), NULL_VECTOR3, NULL_VECTOR3, 0, 0.0f);

		SetAttackbillboard(ATTACKBILLBORAD02_FIRE, 300, 40.0f, 100.0f,
			D3DXVECTOR3(sinf(g_aBoss.fVaim - 0.4f) * (100.0f * float(g_nPurposeCnt)) + g_aBoss.pos.x, 50.0f, cosf(g_aBoss.fVaim - 0.4f) * (100.0f * float(g_nPurposeCnt)) + g_aBoss.pos.z), NULL_VECTOR3, NULL_VECTOR3, 0, 0.0f);

		g_nPurposeCnt++;
	}

	if (g_aBoss.nCntAttackTime >= 220 && g_aBoss.nCntAttackTime <= 320)
	{
		g_aBoss.rot.y = g_aBoss.fVaim + D3DX_PI;
		g_aBoss.state = BOSSSTATE_MOVE;
	}

	if (g_aBoss.nCntAttackTime >= 350 && g_aBoss.nCntAttackTime <= 500)
	{
		g_aBoss.move.x = sinf(g_aBoss.rot.y + D3DX_PI) * 15.0f;
		g_aBoss.move.z = cosf(g_aBoss.rot.y + D3DX_PI) * 15.0f;

		if (g_aBoss.nCntAttackTime % 40 == 0)
		{
			SetBullet(BULLETTYPE_FIRE, 300, 0.0f, 0.0f, g_aBoss.pos, D3DXVECTOR3(sinf(g_aBoss.rot.y + (D3DX_PI * 1.5f)) * 7.0f, 0.0f, cosf(g_aBoss.rot.y + (D3DX_PI * 1.5f)) * 7.0f),
				NULL_VECTOR3, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0, 0.0f);

			SetBullet(BULLETTYPE_FIRE, 300, 0.0f, 0.0f, g_aBoss.pos, D3DXVECTOR3(sinf(g_aBoss.rot.y + (D3DX_PI * 0.5f)) * 7.0f, 0.0f, cosf(g_aBoss.rot.y + (D3DX_PI * 0.5f)) * 7.0f),
				NULL_VECTOR3, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0, 0.0f);
		}
	}

	if (g_aBoss.nCntAttackTime == 500)
	{
		g_bPurposeFlag[0] = false;//汎用的に使うフラグ
		g_bPurposeFlag[1] = false;//汎用的に使うフラグ１
		g_bPurposeFlag[2] = false;//汎用的に使うフラグ２

		g_aBoss.bUnderAttack = false;//攻撃しているかどうか
		g_aBoss.nCntAttackTime = 0;
		g_nPurposeCnt = 0;//汎用カウント

		g_aBoss.move = NULL_VECTOR3;
	}
}

//=====================================
//ボス攻撃８：スターダストメテオ
//=====================================
void BossAttack8_StardustMeteo(void)
{
	Player* pPlayer = GetPlayer();

	g_aBoss.nCntAttackTime++;

	int nLottery = rand() % 3;

	if (g_aBoss.nCntAttackTime == 1)
	{
		g_aBoss.move = NULL_VECTOR3;//移動量を０に
		g_bPurposeFlag[0] = false;//汎用的に使うフラグ０
		g_bPurposeFlag[1] = false;//汎用的に使うフラグ１
		g_bPurposeFlag[2] = false;//汎用的に使うフラグ２
		g_nPurposeCnt = 0;//汎用カウント
		g_aBoss.state = BOSSSTATE_ATTACK;
	}

	float fRandRot = float(rand() % 200 + 1) / 100;
	float fRandLength = float(rand() % 700 + 1);

	if (g_aBoss.nCntAttackTime >= 50 && g_aBoss.nCntAttackTime <= 150)
	{
		g_aBoss.move.y = 10.0f;
	}

	if (g_aBoss.nCntAttackTime >= 150 && g_aBoss.nCntAttackTime <= 800)
	{
		g_aBoss.move = NULL_VECTOR3;

		if (g_aBoss.nCntAttackTime % 20 == 0)
		{
			PlaySound(SOUND_LABEL_SE_FALLINGMETEO_000);
			D3DXVECTOR3 MeteoMove = NULL_VECTOR3;
			if (nLottery == 0)
			{
				//==========================
				//３次元自機狙い処理
				//==========================
				g_aBoss.fVXaim3D = pPlayer->pos.x - g_aBoss.pos.x;
				g_aBoss.fVYaim3D = pPlayer->pos.y - g_aBoss.pos.y;
				g_aBoss.fVZaim3D = pPlayer->pos.z - g_aBoss.pos.z;


				g_aBoss.fVLaim3D = sqrtf((g_aBoss.fVXaim3D * g_aBoss.fVXaim3D) + (g_aBoss.fVYaim3D * g_aBoss.fVYaim3D)
					+ (g_aBoss.fVZaim3D * g_aBoss.fVZaim3D));

				MeteoMove.y = g_aBoss.fVYaim3D / g_aBoss.fVLaim3D * 20.0f;
				MeteoMove.x = g_aBoss.fVXaim3D / g_aBoss.fVLaim3D * 20.0f;
				MeteoMove.z = g_aBoss.fVZaim3D / g_aBoss.fVLaim3D * 20.0f;

				//===============================
				//メテオ発射
				//===============================
				SetAttackModel(ATTACKMODEL07_METEO, 1000, g_aBoss.pos, MeteoMove, NULL_VECTOR3, D3DXVECTOR3(0.2f, 0.2f, 0.2f), 0.0f, 0.0f, 0.0f,
					pPlayer->pos, ATTACKMODELBEHAVIOR00_NORMAL);
			}
			else if (nLottery == 1)
			{
				//================================================================
				//３次元自機狙い処理（プレイヤーの向いている方向を狙う)
				//================================================================
				g_aBoss.fVXaim3D = (pPlayer->pos.x + sinf(pPlayer->rot.y + D3DX_PI) * 700.0f) - g_aBoss.pos.x;
				g_aBoss.fVYaim3D = pPlayer->pos.y - g_aBoss.pos.y;
				g_aBoss.fVZaim3D = (pPlayer->pos.z + cosf(pPlayer->rot.y + D3DX_PI) * 700.0f) - g_aBoss.pos.z;

				g_aBoss.fVLaim3D = sqrtf((g_aBoss.fVXaim3D * g_aBoss.fVXaim3D) + (g_aBoss.fVYaim3D * g_aBoss.fVYaim3D)
					+ (g_aBoss.fVZaim3D * g_aBoss.fVZaim3D));

				MeteoMove.x = g_aBoss.fVXaim3D / g_aBoss.fVLaim3D * 10.0f;
				MeteoMove.y = g_aBoss.fVYaim3D / g_aBoss.fVLaim3D * 10.0f;
				MeteoMove.z = g_aBoss.fVZaim3D / g_aBoss.fVLaim3D * 10.0f;

				//===============================
				//メテオ発射
				//===============================
				SetAttackModel(ATTACKMODEL07_METEO, 1000, g_aBoss.pos, MeteoMove, NULL_VECTOR3, D3DXVECTOR3(0.4f, 0.4f, 0.4f), 0.0f, 0.0f, 0.0f,
					D3DXVECTOR3((pPlayer->pos.x + sinf(pPlayer->rot.y + D3DX_PI) * 700.0f),
						pPlayer->pos.y,
						(pPlayer->pos.z + cosf(pPlayer->rot.y + D3DX_PI) * 700.0f)), ATTACKMODELBEHAVIOR00_NORMAL);
			}
			else if (nLottery == 2)
			{

				//================================================================
				//３次元自機狙い処理（プレイヤーの向いている方向を狙う)
				//================================================================
				g_aBoss.fVXaim3D = (pPlayer->pos.x + sinf(D3DX_PI * fRandRot) * fRandLength) - g_aBoss.pos.x;
				g_aBoss.fVYaim3D = pPlayer->pos.y - g_aBoss.pos.y;
				g_aBoss.fVZaim3D = (pPlayer->pos.z + cosf(D3DX_PI * fRandRot) * fRandLength) - g_aBoss.pos.z;

				g_aBoss.fVLaim3D = sqrtf((g_aBoss.fVXaim3D * g_aBoss.fVXaim3D) + (g_aBoss.fVYaim3D * g_aBoss.fVYaim3D)
					+ (g_aBoss.fVZaim3D * g_aBoss.fVZaim3D));

				MeteoMove.x = g_aBoss.fVXaim3D / g_aBoss.fVLaim3D * 15.0f;
				MeteoMove.y = g_aBoss.fVYaim3D / g_aBoss.fVLaim3D * 15.0f;
				MeteoMove.z = g_aBoss.fVZaim3D / g_aBoss.fVLaim3D * 15.0f;

				//===============================
				//メテオ発射
				//===============================
				SetAttackModel(ATTACKMODEL07_METEO, 1000, g_aBoss.pos, MeteoMove, NULL_VECTOR3, D3DXVECTOR3(0.2f, 0.2f, 0.2f), 0.0f, 0.0f, 0.0f,
					D3DXVECTOR3((pPlayer->pos.x + sinf(D3DX_PI * fRandRot) * fRandLength),
						pPlayer->pos.y,
						(pPlayer->pos.z + cosf(D3DX_PI * fRandRot) * fRandLength)), ATTACKMODELBEHAVIOR00_NORMAL);
			}
		}
	}

	if (g_aBoss.nCntAttackTime == 900)
	{
		g_bPurposeFlag[0] = false;//汎用的に使うフラグ
		g_bPurposeFlag[1] = false;//汎用的に使うフラグ１
		g_bPurposeFlag[2] = false;//汎用的に使うフラグ２

		g_aBoss.bUnderAttack = false;//攻撃しているかどうか
		g_aBoss.nCntAttackTime = 0;
		g_nPurposeCnt = 0;//汎用カウント

		g_aBoss.move = NULL_VECTOR3;
	}

}

//====================================
//ボス攻撃９：コラプスレーザー
//====================================
void BossAttack9_CollapseLaser(void)
{
	g_aBoss.nCntAttackTime++;

	if (g_aBoss.nCntAttackTime == 1)
	{
		g_aBoss.move = NULL_VECTOR3;//移動量を０に
		g_bPurposeFlag[0] = false;//汎用的に使うフラグ０
		g_bPurposeFlag[1] = false;//汎用的に使うフラグ１
		g_bPurposeFlag[2] = false;//汎用的に使うフラグ２
		g_nPurposeCnt = 0;//汎用カウント
		g_aBoss.state = BOSSSTATE_ATTACK;
		g_aBoss.fSaveAim = g_aBoss.rot.y - D3DX_PI;
		g_aBoss.bCurveLaser = false;//レーザーをカーブさせる向きを決める
		g_aBoss.fRotMove = 0.0f;//回転する移動量

		PlaySound(SOUND_LABEL_SE_POWERCHARGE_000);
	}
	
	if (g_aBoss.nCntAttackTime >= 2 && g_aBoss.nCntAttackTime <= 250)
	{
		float fRandPosX = float(rand() % 1500 - 750) + g_aBoss.pos.x;
		float fRandPosY = float(rand() % 1500 - 750) + g_aBoss.pos.y;
		float fRandPosZ = float(rand() % 1500 - 750) + g_aBoss.pos.z;

		float fColR = float(rand() % 100 + 1) / 100.0f;
		float fColG = float(rand() % 100 + 1) / 100.0f;
		float fColB = float(rand() % 100 + 1) / 100.0f;
		D3DXVECTOR3 BulletMove = NULL_VECTOR3;

		g_aBoss.fVXaim3D = g_aBoss.pos.x - fRandPosX;
		g_aBoss.fVYaim3D = g_aBoss.pos.y - fRandPosY;
		g_aBoss.fVZaim3D = g_aBoss.pos.z - fRandPosZ;

		g_aBoss.fVLaim3D = sqrtf((g_aBoss.fVXaim3D * g_aBoss.fVXaim3D) + (g_aBoss.fVYaim3D * g_aBoss.fVYaim3D)
			+ (g_aBoss.fVZaim3D * g_aBoss.fVZaim3D));


		BulletMove.x = g_aBoss.fVXaim3D / g_aBoss.fVLaim3D * 10.0f;
		BulletMove.y = g_aBoss.fVYaim3D / g_aBoss.fVLaim3D * 10.0f;
		BulletMove.z = g_aBoss.fVZaim3D / g_aBoss.fVLaim3D * 10.0f;

		SetEmitterEffect(EMITTERTYPE_GATHERING, 0, 300, 30, 30.0f, 0.6f, D3DXVECTOR3(fRandPosX, fRandPosY, fRandPosZ), BulletMove, D3DXCOLOR(fColR, fColG, fColB, 1.0f),
			0.0f, 0, 0.0f, D3DXVECTOR3(g_aBoss.pos.x, g_aBoss.pos.y + g_aBoss.Size.y / 2.0f, g_aBoss.pos.z));


	}
	

	if (g_aBoss.nCntAttackTime == 300)
	{
		StopSound(SOUND_LABEL_SE_POWERCHARGE_000);
		PlaySound(SOUND_LABEL_SE_RASER_000);
		SetAttackModel(ATTACKMODEL09_LASER, 750, g_aBoss.aParts[1].PartsPos, NULL_VECTOR3, D3DXVECTOR3(0.0f,g_aBoss.fSaveAim, 0.0f), D3DXVECTOR3(1.2f,1.2f,4.0f),
			0.0f, 0.0f, 0.0f, NULL_VECTOR3, ATTACKMODELBEHAVIOR00_NORMAL);
	}

	if (g_aBoss.nCntAttackTime == 950)
	{
		g_bPurposeFlag[0] = false;//汎用的に使うフラグ
		g_bPurposeFlag[1] = false;//汎用的に使うフラグ１
		g_bPurposeFlag[2] = false;//汎用的に使うフラグ２

		g_aBoss.bUnderAttack = false;//攻撃しているかどうか
		g_aBoss.nCntAttackTime = 0;
		g_nPurposeCnt = 0;//汎用カウント
		g_aBoss.fSaveAim = 0.0f;

		g_aBoss.move = NULL_VECTOR3;
	}
}


//======================================
//ボスを逃げさせる
//======================================
void RunAwayBoss(void)
{
	g_aBoss.nCntAttackTime++;

	if (g_aBoss.nCntAttackTime == 1)
	{
		g_aBoss.move = NULL_VECTOR3;//移動量を０に
		g_bPurposeFlag[0] = false;//汎用的に使うフラグ０
		g_bPurposeFlag[1] = false;//汎用的に使うフラグ１
		g_bPurposeFlag[2] = false;//汎用的に使うフラグ２
		g_nPurposeCnt = 0;//汎用カウント
		g_aBoss.state = BOSSSTATE_ATTACK;
		g_aBoss.fSaveAim = g_aBoss.rot.y - D3DX_PI;
		g_aBoss.bCurveLaser = false;//レーザーをカーブさせる向きを決める
		g_aBoss.fRotMove = 0.0f;//回転する移動量
	}
  
	if (g_aBoss.nCntAttackTime >= 2 && g_aBoss.nCntAttackTime <= 300)
	{
		g_aBoss.move = NULL_VECTOR3;
		//g_aBoss.pos.y += 10.0f;
	}

	if (g_aBoss.nCntAttackTime >= 301)
	{
		if (g_aBoss.nCntAttackTime == 305)
		{
			for (int nCntEnemy = 0; nCntEnemy < 10; nCntEnemy++)
			{
				float fLength = float(rand() % 500);
				float fRot = float(rand() % 200 + 1) / 100.0f;
				int nType = rand() % 4;
				SetEnemy(D3DXVECTOR3(g_aBoss.pos.x + sinf(D3DX_PI * fRot) * fLength, 0.0f, g_aBoss.pos.z + cosf(D3DX_PI * fRot) * fLength),
					NULL_VECTOR3,
					nType, true);//敵をセットした数分、関数の先で、ボスが召喚した数をカウントしている
			}
		}
		g_aBoss.move = NULL_VECTOR3;

		if (g_aBoss.nCntSummonEnemy == 0 && g_aBoss.nCntAttackTime >= 310)
		{//召喚した敵の数が０になったら
			g_bPurposeFlag[0] = false;//汎用的に使うフラグ
			g_bPurposeFlag[1] = false;//汎用的に使うフラグ１
			g_bPurposeFlag[2] = false;//汎用的に使うフラグ２

			g_aBoss.bUnderAttack = false;//攻撃しているかどうか
			g_aBoss.bRunAwayBoss = false;//ボスの逃げる状態を解除
			g_aBoss.nCntAttackTime = 0;
			g_nPurposeCnt = 0;//汎用カウント
			g_aBoss.fSaveAim = 0.0f;
			
		}
	}

	PrintDebugProc("ボスが召喚した敵の数：%d", g_aBoss.nCntSummonEnemy);
}

//============================================================================================================================================================================================================
//ボスのマトリックスを計算する
//============================================================================================================================================================================================================
void CaluculateMatrixBoss(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスへのポインタと取得
	D3DXMATRIX mtxRot, mtxTrans, mtxParent;//計算用マトリックス
	D3DMATERIAL9 matDef;//現在のマテリアル保存用

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	//使用している場合
	if (g_aBoss.bUse == true)
	{
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aBoss.mtxWorld);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aBoss.rot.y, g_aBoss.rot.x, g_aBoss.rot.z);
		D3DXMatrixMultiply(&g_aBoss.mtxWorld, &g_aBoss.mtxWorld, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_aBoss.pos.x, g_aBoss.pos.y, g_aBoss.pos.z);
		D3DXMatrixMultiply(&g_aBoss.mtxWorld, &g_aBoss.mtxWorld, &mtxTrans);

		for (int nCntParts = 0; nCntParts < g_aBoss.nNumParts; nCntParts++)
		{
			//各パーツモデルのワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aBoss.aParts[nCntParts].mtxWorld);

			//各パーツモデルの向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot,
				g_aBoss.aParts[nCntParts].rot.y,
				g_aBoss.aParts[nCntParts].rot.x,
				g_aBoss.aParts[nCntParts].rot.z);

			D3DXMatrixMultiply(&g_aBoss.aParts[nCntParts].mtxWorld, &g_aBoss.aParts[nCntParts].mtxWorld, &mtxRot);

			//各パーツモデルの位置を反映
			D3DXMatrixTranslation(&mtxTrans,
				g_aBoss.aParts[nCntParts].pos.x,
				g_aBoss.aParts[nCntParts].pos.y,
				g_aBoss.aParts[nCntParts].pos.z);

			D3DXMatrixMultiply(&g_aBoss.aParts[nCntParts].mtxWorld, &g_aBoss.aParts[nCntParts].mtxWorld, &mtxTrans);

			//親番号の保存
			int nParent = g_aBoss.aParts[nCntParts].nParent;

			//親のマトリックスを反映
			if (nParent != -1)
			{//親の番号を持つ場合
				mtxParent = g_aBoss.aParts[nParent].mtxWorld;//親モデルのマトリックス
			}
			else
			{//親の番号を持たない場合
				mtxParent = g_aBoss.mtxWorld;//プレイヤーのマトリックス
			}

			//親のマトリックスを掛け合わせる
			D3DXMatrixMultiply(&g_aBoss.aParts[nCntParts].mtxWorld, &g_aBoss.aParts[nCntParts].mtxWorld, &mtxParent);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aBoss.aParts[nCntParts].mtxWorld);

			D3DXVECTOR3 PosZero = NULL_VECTOR3;

			D3DXVec3TransformCoord(&g_aBoss.aParts[nCntParts].PartsPos, &PosZero, &g_aBoss.aParts[nCntParts].mtxWorld);
		}
	}
}
