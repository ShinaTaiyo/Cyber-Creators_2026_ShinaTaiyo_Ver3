//========================================================
//
//８月３０日：エフェクトもう一回見直す[pause.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#include "main.h"
#include "enemy.h"
#include "bullet.h"
#include "explosion.h"
#include <stdlib.h>
#include <time.h>
#include "score.h"
#include "game.h"
#include "result.h"
#include "player.h"
#include "sound.h"
#include "particle.h"
#include "Gage.h"
#include "staging.h"
#include "damage.h"
#include "wave.h"
#include "Difficulty.h"
//マクロ定義
#include "input.h"
#define NUM_ENEMY (13)//敵の種類
#define MAX_ENEMYBULLET (10000)
#define MAX_CIRCLEBULLET_2 (10)//円形各散弾の弾数
#define MAX_CIRCLEBULLET_E10 (8)
#define BOSS1_SIZE (120.0f)


#define ENEMY_WIDTH (30.0f)
#define ENEMY_HEIGHT (30.0f)

#define ENEMY8BULLET (30)

#define NUM_BOSS_PATTERN (7)//ボスの技の種類の数

//グローバル変数
LPDIRECT3DTEXTURE9 g_apTextureEnemy[NUM_ENEMY] = {};
D3DXVECTOR3 g_rotPI;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;
Enemy g_aEnemy[MAX_ENEMY];//敵の情報
Result g_aResult;

Defeat g_aDefeat;
int g_nNumEnemy = 0;//敵の総数

//int g_nCountTime = 0;//時間を表すグローバル変数

int g_nDelay = 0;
int g_nBulletShift = 0;//弾の軌道をずらす変数

int g_nRandumBullet1=0;
float g_fRandumResultBullet = 0.0f;
int g_nRandumBullet2 = 0.0f;
float g_fRandumResultBullet2 = 0.0f;

int g_nRandumBullet3 = 0;
float g_fRandumResultBullet3 = 0.0f;

float g_fXmoveEnemy[MAX_ENEMY];

bool g_bResultFlag = false;

int g_nCntEnemyParticle[MAX_ENEMY];
bool g_bSetParticle[MAX_ENEMY];

//==================================================
float g_fBulletCount = 0;//弾の方向調整用変数
bool g_bBulletDirectionFlag = false;
//==================================================
// 円形各散弾の処理用変数
float g_fCircleBullet[MAX_CIRCLEBULLET];
float g_fCircleBullet_E5[MAX_CIRCLEBULLET_2];
//=================================
//カーブ拡散弾の処理用変数
//=================================
float g_fCurveBullet[MAX_CIRCLEBULLET_2];
//=============================================

float g_fLengthEnemy;
float g_fAngleEnemy;

int g_nDefeatCount;

//=====================================================
//自機狙い弾の処理
//=====================================================
float g_fXVaim[MAX_ENEMY];
float g_fYVaim[MAX_ENEMY];
float g_Vaim[MAX_ENEMY];


float g_ShotVectorX[MAX_ENEMY];
float g_ShotVectorY[MAX_ENEMY];

//===============================
//エネミー５の処理
//===============================
int g_nE5_CurveCount;
int g_nCurveBullet_E5[MAX_CIRCLEBULLET_2];

//=====================================================
//エネミー７の処理
//=====================================================
float g_fRolingBulletX_E7;
float g_fRolingBulletY_E7;

//======================================================
//エネミー８の処理
//======================================================
float g_fSpeedBullet_E8[ENEMY8BULLET];

float g_fCircleBullet_E8[ENEMY8BULLET];

//======================================================
//エネミー９の処理
//======================================================
int g_nSet_SpeedBullet_E9;//rand関数にあてはめる用
float g_fSpeedBullet_E9;//弾のスピードをランダムにする。

//エネミー１０の処理
int g_nCIrcleBullet_E10[MAX_CIRCLEBULLET_E10];


bool g_bClearFlag;

//============================================
//ボス１の処理
//============================================
int g_nBoss1TechiniquePattern;//出してくる弾幕を選択させる変数
int g_nCountTechiniqueTime;//選んだ技を出す時間をカウントする変数
bool g_bBoss1Flag;

float g_fRotBullet_W6;

bool g_bNohitFlag;
//=====================================================
//==========================
//敵の初期化処理
//==========================
void InitEnemy(void)
{
	GameCustom* pGameCustom = GetGameCustom();
	int nA = 0;
	
	g_nBulletShift = 0;
	
	g_bClearFlag = false;

	g_bNohitFlag = false;

	g_bBoss1Flag=false;

	g_nBoss1TechiniquePattern = 0;//初期化
	g_nCountTechiniqueTime = 0;//初期化

	g_fRotBullet_W6 = 0.0f;

	g_nE5_CurveCount = 0;

	//g_aDefeat.nDefeatCount = 0;//現在のウェーブを０にする。

	g_aDefeat.DefeatEnemyCount = 0;//敵撃破数の初期化
	int nCntBullet4;
	
	g_fSpeedBullet_E8;


		//srand((unsigned int)time(NULL));//ゲームを開始した時間毎に乱数の種を設定

	g_aResult.bResultFlag = false;
	int nCntBullet;
	
	for (nCntBullet = 0; nCntBullet < MAX_CIRCLEBULLET; nCntBullet++)
	{//円形各散弾の初期化
		g_fCircleBullet[nCntBullet] = 0;
	}


	for (nCntBullet4 = 0; nCntBullet4 < ENEMY8BULLET; nCntBullet4++)
	{
		//g_fCircleBullet_E8[nCntBullet4] = (1.0f / (ENEMY8BULLET / 2)) * nCntBullet4;

		g_fSpeedBullet_E8[nCntBullet4] = 0;


		//SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * g_fCircleBullet_E8[nCntBullet4]) * g_fSpeedBullet_E8[nCntBullet4]), (cosf(-D3DX_PI * g_fCircleBullet_E8[nCntBullet4]) * g_fSpeedBullet_E8[nCntBullet4]), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY);//敵の弾だから敵同士で当たらない	
	}


	g_aEnemy->nEnemyCount = NUM_ENEMY;

	LPDIRECT3DDEVICE9 pDevice;
	int	nCntEnemy;

	g_nNumEnemy = MAX_ENEMY;//敵の総数の初期化


	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み（敵のテクスチャを読み込む）
	D3DXCreateTextureFromFile(pDevice,
		"data\\enemy1_delby.png",
		&g_apTextureEnemy[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\enemy2_pundy.png",
		&g_apTextureEnemy[1]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\enemy3_smiley.png",
		&g_apTextureEnemy[2]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\enemy4_squby.png",
		&g_apTextureEnemy[3]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\enemy5_reafy.png",
		&g_apTextureEnemy[4]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\enemy6_honoby.png",
		&g_apTextureEnemy[5]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\enemy7_mizuby.png",
		&g_apTextureEnemy[6]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\enemy8_bomuby.png",
		&g_apTextureEnemy[7]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\enemy9_winby.png",
		&g_apTextureEnemy[8]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\enemy10_bolby.png",
		&g_apTextureEnemy[9]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\Enemy11_bandy.png",
		&g_apTextureEnemy[10]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\boss1_starwarrior.png",
		&g_apTextureEnemy[11]);
	//g_aEnemy->pos = D3DXVECTOR3(0.0f,0.0f, 0.0f);//位置を初期化する

	//敵情報の初期化
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		g_nCntEnemyParticle[nCntEnemy] = 0;
		g_bSetParticle[nCntEnemy] = false;

		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].nType = 0;//それぞれのエネミーに敵の種類（nType)を割り振っている。
		g_aEnemy[nCntEnemy].bUse = false;//使用していない状態にする
		g_aEnemy[nCntEnemy].State = ENEMYSTATE_NORMAL;//敵の状態を普通の状態に初期化
		g_aEnemy[nCntEnemy].nCounterState = 0;//状態を変える時間を表すカウンター
		g_aEnemy[nCntEnemy].fLife = 0.0f;//寿命の初期化
		g_aEnemy[nCntEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_fXmoveEnemy[nCntEnemy] = 0;

		g_aEnemy[nCntEnemy].bEnemyMoveX = false;
		g_aEnemy[nCntEnemy].bEnemyMoveY= false;

		g_aEnemy[nCntEnemy].bSetEnemyUse = false;
		g_aEnemy[nCntEnemy].bDefeatFlag = false;
		g_aEnemy[nCntEnemy].bBossSummonFlag = false;//ボス召喚フラグ
		g_aEnemy[nCntEnemy].fBossMove = 0.0f;
		g_aEnemy[nCntEnemy].bBossMoveFlag = false;

		g_aEnemy[nCntEnemy].nCountMoveInterval = 0;
		g_aEnemy[nCntEnemy].nMoveTime= 0;

		g_aEnemy[nCntEnemy].fRotMove = 0.0f;

		g_aEnemy[nCntEnemy].bAttackInterval = false;

		g_aEnemy[nCntEnemy].nAttarkIntervalCount = 0;

		g_aEnemy[nCntEnemy].nApperCount = 0;

		g_aEnemy[nCntEnemy].bEnemyCountminusFlag = false;
	}


	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ENEMY,//４は必要な頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEnemy,
		NULL);

	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//頂点座標の設定(敵の大きさ）
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(80.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 80.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(80.0f, 80.0f, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;//頂点データのポインタを４つ分進める。
	}
	//頂点バッファをアンロックする
	g_pVtxBuffEnemy->Unlock();
}

//====================================
//敵の終了処理
//====================================
void UninitEnemy(void)
{
	//テクスチャの破棄
	int nCntEnemy;
	for (nCntEnemy = 0; nCntEnemy < NUM_ENEMY; nCntEnemy++)
	{
		if (g_apTextureEnemy[nCntEnemy] != NULL)
		{
			g_apTextureEnemy[nCntEnemy]->Release();
			g_apTextureEnemy[nCntEnemy] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffEnemy != NULL)
	{
		g_pVtxBuffEnemy->Release();
		g_pVtxBuffEnemy = NULL;
	}
}


//========================================
//敵の更新処理
//========================================
void UpdateEnemy(void)
{
	GameCustom* pGameCustom = GetGameCustom();

	//LPDIRECT3DDEVICE9 pDevice;
	int	nCntBullet;
	int nCntEnemy;
	int nCntBullet2;
	int nCntBullet3;
	int nRandBullet2;

	/*g_aEnemy[nCntEnemy].fRotMove;*/
	VERTEX_2D* pVtx;

	Bullet* pBullet = GetBullet();//弾構造体の情報をゲットする。

	Player* pPlayer = GetPlayer();//プレイヤー構造体の情報をゲットする。

	g_nE5_CurveCount++;//エネミー５の弾カーブさせる用の変数。カウントスタート

	g_fRotBullet_W6 += 0.01f;//ボス１の技６の弾の発射方向をずらし続ける処理

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);
	//srand((unsigned int)time(NULL));

	g_nRandumBullet1 = rand() % 200 + 1;//0～100の値を設定(乱数弾）

	g_nRandumBullet2 = rand() % 100 + 1;//円形各散弾の発射方向の調整用

	g_nRandumBullet3 = rand() % 100 + 1;//円形各散弾の発射方向の調整用

	g_nSet_SpeedBullet_E9 = rand() % 50 + 1;//エネミー９の弾のスピード調整用

	

	g_fRandumResultBullet = 0.01f * g_nRandumBullet1;//0～2.0の間で値を変動させる。

	g_fRandumResultBullet2 = 0.01f * g_nRandumBullet2;//円形拡散弾の方向調整結果

	g_fRandumResultBullet3 = 0.01f * g_nRandumBullet3;

	g_fSpeedBullet_E9 = 0.1f * g_nSet_SpeedBullet_E9;//エネミー９の弾のスピードを確定
	//=========================================
	//エネミー7弾幕処理
	//=========================================
	g_fRolingBulletX_E7 += 0.01f;
	g_fRolingBulletY_E7 += 0.01f;

	if (g_nDelay % 9 == 0)
	{
		g_fRolingBulletX_E7 += 0.05f;
		g_fRolingBulletY_E7 += 0.03f;
	}
	//=============================================

	//========================================================================================================================================================================================================================================
	g_nDelay++;
	g_nBulletShift++;



	float fRandumSpeed_E12;

	fRandumSpeed_E12 = float((rand)() % 100 + 1) / 10.0f;//弾を発射した時のスピードを決める。

	//for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	//{
	//	if (g_aEnemy[nCntEnemy].bUse == true && g_aEnemy[nCntEnemy].nType == ENEMYTYPE_DELBY)
	//	{
	//		g_fXmoveEnemy[nCntEnemy]++;
	//	}
	//}
	//pBullet[pBullet->nCntBullet_2].fCurveBulletX += 0.002f;//X方向にカーブさせる
	//pBullet[pBullet->nCntBullet_2].fCurveBulletY += 0.001f;//X方向にカーブさせる

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{

		pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 30.0f, g_aEnemy[nCntEnemy].pos.y - 30.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 30.0f, g_aEnemy[nCntEnemy].pos.y - 30.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 30.0f, g_aEnemy[nCntEnemy].pos.y + 30.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 30.0f, g_aEnemy[nCntEnemy].pos.y + 30.0f, 0.0f);





		if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS1_STARWARRIOR)
		{
			pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - BOSS1_SIZE, g_aEnemy[nCntEnemy].pos.y - BOSS1_SIZE, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + BOSS1_SIZE, g_aEnemy[nCntEnemy].pos.y - BOSS1_SIZE, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - BOSS1_SIZE, g_aEnemy[nCntEnemy].pos.y + BOSS1_SIZE, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + BOSS1_SIZE, g_aEnemy[nCntEnemy].pos.y + BOSS1_SIZE, 0.0f);
		}


		if (g_aEnemy[nCntEnemy].bUse == true)
		{

			g_aEnemy[nCntEnemy].nApperCount++;//敵の出現位置を決定してから出現するまでの時間をカウントする。

			/*if (g_bSetParticle[nCntEnemy] == true)
			{
				g_nCntEnemyParticle[nCntEnemy]++;

				SetParticle(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 25, 100);

				if (g_nCntEnemyParticle[nCntEnemy] >= 30)
				{
					g_nCntEnemyParticle[nCntEnemy] = 0;
					g_bSetParticle[nCntEnemy] = false;
				}
			}*/

			/*if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS1_STARWARRIOR)
			{
				if (g_bBoss1Flag == false)
				{
					g_bBoss1Flag = true;
					SetGage(nCntEnemy, GAGETYPE_BOSS1HP);
				}
			}*/
			if (g_aEnemy[nCntEnemy].nApperCount >= 100)
			{

				if (g_aEnemy[nCntEnemy].bUse == true && g_aEnemy[nCntEnemy].nType == ENEMYTYPE_DELBY)
				{//エネミー０が使われていたら・・・
					switch (pGameCustom->nDifficulty)
					{
					case 1://難易度EASY
						if (g_nDelay % 80 == 0)
						{
							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_000);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * 0.0f) * 5.0f), (cosf(-D3DX_PI * 0.0f) * 5.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);
						}
						break;
					case 2://難易度Normal
						if (g_nDelay % 60 == 0)
						{
							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_000);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * 0.0f) * 5.0f), (cosf(-D3DX_PI * 0.0f) * 5.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);
						}
						break;
					case 3://難易度Hard
						if (g_nDelay % 20 == 0)
						{
							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_000);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * 0.0f) * 5.0f), (cosf(-D3DX_PI * 0.0f) * 5.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);
						}
						break;
					case 4://難易度Hard
						if (g_nDelay % 10 == 0)
						{
							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_000);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * 0.0f) * 5.0f), (cosf(-D3DX_PI * 0.0f) * 5.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);
						}
						break;
					}
					



				}

				if (g_aEnemy[nCntEnemy].bUse == true && g_aEnemy[nCntEnemy].nType == ENEMYTYPE_PUNDY)
				{//エネミー２の攻撃
					
					switch (pGameCustom->nDifficulty)
					{
					case 1://難易度Easy
						if (g_nDelay % 30 == 0)
						{
							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_000);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * g_fRandumResultBullet) * 5.0f), (cosf(-D3DX_PI * g_fRandumResultBullet) * 5.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);//敵の弾だから敵同士で当たらない
						}
						break;
					case 2://難易度Normal
						if (g_nDelay % 15 == 0)
						{
							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_000);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * g_fRandumResultBullet) * 5.0f), (cosf(-D3DX_PI * g_fRandumResultBullet) * 5.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);//敵の弾だから敵同士で当たらない
						}
						break;
					case 3://難易度Hard
						if (g_nDelay %6  == 0)
						{
							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_000);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * g_fRandumResultBullet) * 5.0f), (cosf(-D3DX_PI * g_fRandumResultBullet) * 5.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);//敵の弾だから敵同士で当たらない
						}
						break;
					case 4://難易度VeryHard
						if (g_nDelay % 2 == 0)
						{
							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_000);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * g_fRandumResultBullet) * 5.0f), (cosf(-D3DX_PI * g_fRandumResultBullet) * 5.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * g_fRandumResultBullet) * 2.0f), (cosf(-D3DX_PI * g_fRandumResultBullet) * 2.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);//敵の弾だから敵同士で当たらない
						}
						break;
					}
					//SetBullet(g_aEnemy[1].pos, D3DXVECTOR3((sinf(-D3DX_PI * g_fRandumResultBullet) * 10.0f), (cosf(-D3DX_PI * g_fRandumResultBullet) * 10.0f), 0.0f), 10, BULLETTYPE_ENEMY);//敵の弾だから敵同士で当たらない
					

				}

				if (g_aEnemy[nCntEnemy].bUse == true &&g_aEnemy[nCntEnemy].nType == ENEMYTYPE_SMILY)
				{//エネミー３の攻撃
				
					if (g_bBulletDirectionFlag == false)
					{
						g_fBulletCount += 0.09f;
						if (g_nBulletShift % 4 == 0)
						{
							g_fBulletCount += 0.04f;
						}
					}



					if (g_bBulletDirectionFlag == true)
					{
						g_fBulletCount -= 0.09f;
						if (g_nBulletShift % 3 == 0)
						{
							g_fBulletCount -= 0.03f;
						}
					}

					if (g_fBulletCount >= 0.5f)
					{
						g_bBulletDirectionFlag = true;
					}
					if (g_fBulletCount <= -0.5f)
					{
						g_bBulletDirectionFlag = false;
					}

					switch (pGameCustom->nDifficulty)
					{
					case 1://難易度Easy
						if (g_nDelay % 45 == 0)
						{
							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_000);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * (0.0f + g_fBulletCount)) * 3.0f), (cosf(-D3DX_PI * (0.0f + g_fBulletCount)) * 4.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);//敵の弾だから敵同士で当たらない

						}
						break;
					case 2://難易度Normal
						if (g_nDelay % 15 == 0)
						{
							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_000);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * (0.0f + g_fBulletCount)) * 3.0f), (cosf(-D3DX_PI * (0.0f + g_fBulletCount)) * 4.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);//敵の弾だから敵同士で当たらない

						}
						break;
					case 3://難易度Hard
						if (g_nDelay % 7 == 0)
						{
							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_000);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * (0.0f + g_fBulletCount)) * 3.0f), (cosf(-D3DX_PI * (0.0f + g_fBulletCount)) * 4.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);//敵の弾だから敵同士で当たらない

						}
						break;
					case 4://難易度VeryHard
						if (g_nDelay % 3 == 0)
						{
							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_000);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * (0.0f + g_fBulletCount)) * 3.0f), (cosf(-D3DX_PI * (0.0f + g_fBulletCount)) * 4.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);//敵の弾だから敵同士で当たらない
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(D3DX_PI * (1.0f + g_fBulletCount)) * 3.0f), (cosf(D3DX_PI * (1.0f + g_fBulletCount)) * 4.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);//敵の弾だから敵同士で当たらない


						}
						break;
					}
				


				}

				if (g_aEnemy[nCntEnemy].bUse == true && g_aEnemy[nCntEnemy].nType == ENEMYTYPE_SQUDY)
				{//エネミー４の攻撃！円形各散弾
					


					int nCntBullet_E4;
					float fCircleBullet_E4_easy[8];
					float fCircleBullet_E4_normal[12];
					float fCircleBullet_E4_hard[16];
					float fCircleBullet_E4_veryhard[24];
					float fRotBullet_E4;
					fRotBullet_E4= float((rand)() % 200)/ 100.0f;

					switch (pGameCustom->nDifficulty)
					{
					case 1://Easy
						if (g_nDelay % 50 == 0)
						{
							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_004);

							for (nCntBullet_E4 = 0; nCntBullet_E4 < 8; nCntBullet_E4++)

							{
								fCircleBullet_E4_easy[nCntBullet_E4] = (1.0f / 4.0f) * nCntBullet_E4;

								SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * (fCircleBullet_E4_easy[nCntBullet_E4] + fRotBullet_E4)) * 4.0f), (cosf(-D3DX_PI * (fCircleBullet_E4_easy[nCntBullet_E4] + fRotBullet_E4)) * 4.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);

							}
						}
						break;
					case 2://Normal
						if (g_nDelay % 45 == 0)
						{
							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_004);

							for (nCntBullet_E4 = 0; nCntBullet_E4 < 12; nCntBullet_E4++)

							{
								fCircleBullet_E4_normal[nCntBullet_E4] = (1.0f / 6.0f) * nCntBullet_E4;

								SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * (fCircleBullet_E4_normal[nCntBullet_E4] + fRotBullet_E4)) * 4.0f), (cosf(-D3DX_PI * (fCircleBullet_E4_normal[nCntBullet_E4] + fRotBullet_E4)) * 4.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);

							}
						}
						break;
					case 3://Hard
						if (g_nDelay % 40 == 0)
						{
							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_004);

							for (nCntBullet_E4 = 0; nCntBullet_E4 < 16; nCntBullet_E4++)

							{
								fCircleBullet_E4_hard[nCntBullet_E4] = (1.0f / 8.0f) * nCntBullet_E4;

								SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * (fCircleBullet_E4_hard[nCntBullet_E4] + fRotBullet_E4)) * 4.0f), (cosf(-D3DX_PI * (fCircleBullet_E4_hard[nCntBullet_E4] + fRotBullet_E4)) * 4.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);

							}
						}
						break;
					case 4://VeryHard
						if (g_nDelay % 30 == 0)
						{
							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_004);

							for (nCntBullet_E4 = 0; nCntBullet_E4 < 24; nCntBullet_E4++)

							{
								fCircleBullet_E4_veryhard[nCntBullet_E4] = (1.0f / 12.0f) * nCntBullet_E4;

								SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * (fCircleBullet_E4_veryhard[nCntBullet_E4] + fRotBullet_E4)) * 4.0f), (cosf(-D3DX_PI * (fCircleBullet_E4_veryhard[nCntBullet_E4] + fRotBullet_E4)) * 4.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);

							}
						}
						break;

					}



					
				}

				if (g_aEnemy[nCntEnemy].bUse == true && g_nDelay % 60 == 0 && g_aEnemy[nCntEnemy].nType == ENEMYTYPE_REAFY)
				{//エネミー５の攻撃(SetBullet)
				//	SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * 0.0f) * 5.0f), (cosf(-D3DX_PI * 0.0f) * 5.0f), 0.0f), 10, BULLETTYPE_REAFY);//敵の弾だから敵同士で当たらない
					int nCntBullet_E5;
					float fCircleBullet_E5_easy[4];
					float fCircleBullet_E5_normal[12];
					float fCircleBullet_E5_hard[16];
					float fCircleBullet_E5_veryhard[24];
					switch (pGameCustom->nDifficulty)
					{
						
					case 1:
						PlaySound(SOUND_LABEL_SE_ARROWSHOT_000);
						for (nCntBullet_E5 = 0; nCntBullet_E5 < 4; nCntBullet_E5++)
						{
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_REAFY, nCntBullet_E5, nCntEnemy, -1);//敵の弾だから敵同士で当たらない

						}
						break;
					case 2:
						PlaySound(SOUND_LABEL_SE_ARROWSHOT_000);
						for (nCntBullet_E5 = 0; nCntBullet_E5 < 8; nCntBullet_E5++)
						{
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_REAFY, nCntBullet_E5, nCntEnemy, -1);//敵の弾だから敵同士で当たらない

						}
						break;
					case 3:
						PlaySound(SOUND_LABEL_SE_ARROWSHOT_000);
						for (nCntBullet_E5 = 0; nCntBullet_E5 < 14; nCntBullet_E5++)
						{
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_REAFY, nCntBullet_E5, nCntEnemy, -1);//敵の弾だから敵同士で当たらない

						}
						break;
					case 4:
						PlaySound(SOUND_LABEL_SE_ARROWSHOT_000);
						for (nCntBullet_E5 = 0; nCntBullet_E5 < 20; nCntBullet_E5++)
						{
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_REAFY, nCntBullet_E5, nCntEnemy, -1);//敵の弾だから敵同士で当たらない

						}
						break;

					}
				


				}

				


				if (g_aEnemy[nCntEnemy].bUse == true && g_nDelay % 60 == 0 && g_aEnemy[nCntEnemy].nType == ENEMYTYPE_HONOBY)
				{//エネミー6が使われていたら・・・
				


					g_fXVaim[nCntEnemy] = pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x;//X座標へのベクトル
					g_fYVaim[nCntEnemy] = pPlayer->pos.y - g_aEnemy[nCntEnemy].pos.y;//Y座標へのベクトル
				//g_Vaim[nCntEnemy] = sqrt((g_fXVaim[nCntEnemy] * g_fXVaim[nCntEnemy]) - (g_fYVaim[nCntEnemy] * g_fYVaim[nCntEnemy]));*理想とは違ったが、面白かったのでキープしておく
					/*g_Vaim[nCntEnemy] = sqrtf((g_fXVaim[nCntEnemy] * g_fXVaim[nCntEnemy])+(g_fYVaim[nCntEnemy] * g_fYVaim[nCntEnemy]));*/

					g_Vaim[nCntEnemy] = atan2(g_fXVaim[nCntEnemy], g_fYVaim[nCntEnemy]);

					switch (pGameCustom->nDifficulty)
					{
					case 1://Easy
						PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_003);
						SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((D3DX_PI * g_fXVaim[nCntEnemy]) * 0.002f, (D3DX_PI * g_fYVaim[nCntEnemy]) * 0.002f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);//敵の弾だから敵同士で当たらない
						break;
					case 2://Normal
						PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_003);
						SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((D3DX_PI * g_fXVaim[nCntEnemy]) * 0.004f, (D3DX_PI * g_fYVaim[nCntEnemy]) * 0.004f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);//敵の弾だから敵同士で当たらない
						break;
					case 3://Hard
						PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_003);
						SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((D3DX_PI * g_fXVaim[nCntEnemy]) * 0.002f, (D3DX_PI * g_fYVaim[nCntEnemy]) * 0.002f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);
						SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((D3DX_PI* g_fXVaim[nCntEnemy]) * 0.004f, (D3DX_PI* g_fYVaim[nCntEnemy]) * 0.004f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);
						break;
					case 4://VeryHard
						PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_003);
						SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((D3DX_PI * g_fXVaim[nCntEnemy]) * 0.004f, (D3DX_PI * g_fYVaim[nCntEnemy]) * 0.004f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_SCATTERING, 0, nCntEnemy, -1);//敵の弾だから敵同士で当たらない
						break;
					}

					
					
				}

				if (g_aEnemy[nCntEnemy].bUse == true &&g_aEnemy[nCntEnemy].nType == ENEMYTYPE_MIZUBY)
				{//エネミー7が使われていたら・・・

					switch (pGameCustom->nDifficulty)
					{
					case 1://Easy
						if (g_nDelay % 54 == 0)
						{
							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_000);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * 0.0f + g_fRolingBulletX_E7) * 5.0f), (cosf(-D3DX_PI * 0.0f + g_fRolingBulletY_E7) * 5.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);//敵の弾だから敵同士で当たらない
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * 1.0f + g_fRolingBulletX_E7) * 5.0f), (cosf(-D3DX_PI * 1.0f + g_fRolingBulletY_E7) * 5.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);//敵の弾だから敵同士で当たらない
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * 0.0f - g_fRolingBulletX_E7) * 5.0f), (cosf(-D3DX_PI * 0.0f - g_fRolingBulletY_E7) * 5.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);//敵の弾だから敵同士で当たらない
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * 1.0f - g_fRolingBulletX_E7) * 5.0f), (cosf(-D3DX_PI * 1.0f - g_fRolingBulletY_E7) * 5.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);//敵の弾だから敵同士で当たらない
						}
						break;
					case 2://Easy
						if (g_nDelay % 23== 0)
						{
							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_000);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * 0.0f + g_fRolingBulletX_E7) * 5.0f), (cosf(-D3DX_PI * 0.0f + g_fRolingBulletY_E7) * 5.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);//敵の弾だから敵同士で当たらない
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * 1.0f + g_fRolingBulletX_E7) * 5.0f), (cosf(-D3DX_PI * 1.0f + g_fRolingBulletY_E7) * 5.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);//敵の弾だから敵同士で当たらない
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * 0.0f - g_fRolingBulletX_E7) * 5.0f), (cosf(-D3DX_PI * 0.0f - g_fRolingBulletY_E7) * 5.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);//敵の弾だから敵同士で当たらない
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * 1.0f - g_fRolingBulletX_E7) * 5.0f), (cosf(-D3DX_PI * 1.0f - g_fRolingBulletY_E7) * 5.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);//敵の弾だから敵同士で当たらない
						}
						break;
					case 3://Easy
						if (g_nDelay % 11 == 0)
						{
							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_000);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * 0.0f + g_fRolingBulletX_E7) * 5.0f), (cosf(-D3DX_PI * 0.0f + g_fRolingBulletY_E7) * 5.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);//敵の弾だから敵同士で当たらない
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * 1.0f + g_fRolingBulletX_E7) * 5.0f), (cosf(-D3DX_PI * 1.0f + g_fRolingBulletY_E7) * 5.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);//敵の弾だから敵同士で当たらない
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * 0.0f - g_fRolingBulletX_E7) * 5.0f), (cosf(-D3DX_PI * 0.0f - g_fRolingBulletY_E7) * 5.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);//敵の弾だから敵同士で当たらない
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * 1.0f - g_fRolingBulletX_E7) * 5.0f), (cosf(-D3DX_PI * 1.0f - g_fRolingBulletY_E7) * 5.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);//敵の弾だから敵同士で当たらない
						}
						break;
					case 4://Easy
						if (g_nDelay % 6 == 0)
						{
							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_000);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * 0.0f + g_fRolingBulletX_E7) * 5.0f), (cosf(-D3DX_PI * 0.0f + g_fRolingBulletY_E7) * 5.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);//敵の弾だから敵同士で当たらない
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * 1.0f + g_fRolingBulletX_E7) * 5.0f), (cosf(-D3DX_PI * 1.0f + g_fRolingBulletY_E7) * 5.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);//敵の弾だから敵同士で当たらない
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * 0.0f - g_fRolingBulletX_E7) * 5.0f), (cosf(-D3DX_PI * 0.0f - g_fRolingBulletY_E7) * 5.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);//敵の弾だから敵同士で当たらない
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * 1.0f - g_fRolingBulletX_E7) * 5.0f), (cosf(-D3DX_PI * 1.0f - g_fRolingBulletY_E7) * 5.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);//敵の弾だから敵同士で当たらない
						}
						break;
					}
					


				}

				if (g_aEnemy[nCntEnemy].bUse == true && g_nDelay % 5 == 0 && g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOMUBY)
				{//エネミー8が使われていたら・・・
					//SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * 0.0f) * 5.0f), (cosf(-D3DX_PI * 0.0f) * 5.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY);//敵の弾だから敵同士で当たらない




				}
				if (g_aEnemy[nCntEnemy].bUse == true&& g_aEnemy[nCntEnemy].nType == ENEMYTYPE_WINBY)
				{//エネミー9が使われていたら・・・
					
					/*g_aEnemy[nCntEnemy].rot += D3DXVECTOR3(3.0f, 3.0f, 0.0f);*/

					g_rotPI.z += 0.1f;

					if (g_nDelay % 9 == 0)
					{
						g_rotPI.z += 0.019f;
					}
					switch (pGameCustom->nDifficulty)
					{
					case 1://Easy
						if (g_nDelay % 80 == 0)
						{
							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_006);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * g_rotPI.z) * g_fSpeedBullet_E9), (cosf(-D3DX_PI * g_rotPI.z) * g_fSpeedBullet_E9), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY9, 0, nCntEnemy, -1);//

						}
							break;
					case 2://Normal
						if (g_nDelay % 60 == 0)
						{
							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_006);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * g_rotPI.z) * g_fSpeedBullet_E9), (cosf(-D3DX_PI * g_rotPI.z) * g_fSpeedBullet_E9), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY9, 0, nCntEnemy, -1);//
						}

							break;
						
					case 3://Hard
						if (g_nDelay % 30 == 0)
						{
							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_006);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * g_rotPI.z) * g_fSpeedBullet_E9), (cosf(-D3DX_PI * g_rotPI.z) * g_fSpeedBullet_E9), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY9, 0, nCntEnemy, -1);//

						}
							break;
					case 4://VeryHard
						if (g_nDelay % 14 == 0)
						{
							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_006);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * g_rotPI.z) * g_fSpeedBullet_E9), (cosf(-D3DX_PI * g_rotPI.z) * g_fSpeedBullet_E9), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY9, 0, nCntEnemy, -1);//

						}
							break;
					}



				}


				if (g_aEnemy[nCntEnemy].bUse == true &&g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOLBY)
				{
					int nCircleBullet[4];
				
				
					switch (pGameCustom->nDifficulty)
					{
					case 1://Easy
						if (g_nDelay % 90 == 0)
						{
							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_003);
							for (nCntBullet2 = 0; nCntBullet2 < MAX_CIRCLEBULLET_E10; nCntBullet2++)
							{


								g_fCircleBullet[nCntBullet2] = (1.0f / 4.0f) * nCntBullet2;

								if (nCntBullet2 % 2 == 0)
								{
									SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * g_fCircleBullet[nCntBullet2] + g_fRandumResultBullet2) * 1.0f), (cosf(-D3DX_PI * g_fCircleBullet[nCntBullet2] + g_fRandumResultBullet2) * 1.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BOLBY, 0, nCntEnemy, -1);//敵の弾だから敵同士で当たらない		}
								}
								else
								{
									//SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * g_fCircleBullet[nCntBullet2] + g_fRandumResultBullet2) * 3.0f), (cosf(-D3DX_PI * g_fCircleBullet[nCntBullet2] + g_fRandumResultBullet2) * 3.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BOLBY, 0, nCntEnemy, -1);
								}
							}
						}
						break;
					case 2://Normal
						if (g_nDelay % 50 == 0)
						{
							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_003);
							for (nCntBullet2 = 0; nCntBullet2 < MAX_CIRCLEBULLET_E10; nCntBullet2++)
							{


								g_fCircleBullet[nCntBullet2] = (1.0f / 4.0f) * nCntBullet2;

								if (nCntBullet2 % 2 == 0)
								{
									SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * g_fCircleBullet[nCntBullet2] + g_fRandumResultBullet2) * 1.0f), (cosf(-D3DX_PI * g_fCircleBullet[nCntBullet2] + g_fRandumResultBullet2) * 1.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BOLBY, 0, nCntEnemy, -1);//敵の弾だから敵同士で当たらない		}
								}
								else
								{
									//SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * g_fCircleBullet[nCntBullet2] + g_fRandumResultBullet2) * 3.0f), (cosf(-D3DX_PI * g_fCircleBullet[nCntBullet2] + g_fRandumResultBullet2) * 3.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BOLBY, 0, nCntEnemy, -1);
								}
							}
						}
						break;
					case 3://Hard
						if (g_nDelay % 40 == 0)
						{
							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_003);
							for (nCntBullet2 = 0; nCntBullet2 < MAX_CIRCLEBULLET_E10; nCntBullet2++)
							{


								g_fCircleBullet[nCntBullet2] = (1.0f / 4.0f) * nCntBullet2;

								if (nCntBullet2 % 2 == 0)
								{
									SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * g_fCircleBullet[nCntBullet2] + g_fRandumResultBullet2) * 1.0f), (cosf(-D3DX_PI * g_fCircleBullet[nCntBullet2] + g_fRandumResultBullet2) * 1.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BOLBY, 0, nCntEnemy, -1);//敵の弾だから敵同士で当たらない		}
								}
								else
								{
									SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * g_fCircleBullet[nCntBullet2] + g_fRandumResultBullet2) * 3.0f), (cosf(-D3DX_PI * g_fCircleBullet[nCntBullet2] + g_fRandumResultBullet2) * 3.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BOLBY, 0, nCntEnemy, -1);
								}
							}
						}
						break;
					case 4://VeryHard
						if (g_nDelay % 30 == 0)
						{
							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_003);
							for (nCntBullet2 = 0; nCntBullet2 < 12; nCntBullet2++)
							{


								g_fCircleBullet[nCntBullet2] = (1.0f / 6.0f) * nCntBullet2;

								if (nCntBullet2 % 2 == 0)
								{
									SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * g_fCircleBullet[nCntBullet2] + g_fRandumResultBullet2) * 1.0f), (cosf(-D3DX_PI * g_fCircleBullet[nCntBullet2] + g_fRandumResultBullet2) * 1.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BOLBY, 0, nCntEnemy, -1);//敵の弾だから敵同士で当たらない		}
								}
								else
								{
									SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * g_fCircleBullet[nCntBullet2] + g_fRandumResultBullet2) * 3.0f), (cosf(-D3DX_PI * g_fCircleBullet[nCntBullet2] + g_fRandumResultBullet2) * 3.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BOLBY, 0, nCntEnemy, -1);
								}
							}
						}
						break;
					}

					//for (nCntBullet2 = 0; nCntBullet2 < MAX_CIRCLEBULLET_E10; nCntBullet2++)
					//{

					//	
					//	g_fCircleBullet[nCntBullet2] = (1.0f / 4.0f) * nCntBullet2;

					//	if (nCntBullet2 % 2 == 0)
					//	{
					//		SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * g_fCircleBullet[nCntBullet2] + g_fRandumResultBullet2) * 1.0f), (cosf(-D3DX_PI * g_fCircleBullet[nCntBullet2] + g_fRandumResultBullet2) * 1.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BOLBY, 0, nCntEnemy, -1);//敵の弾だから敵同士で当たらない		}
					//	}
					//	else
					//	{
					//		SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * g_fCircleBullet[nCntBullet2] + g_fRandumResultBullet2) * 3.0f), (cosf(-D3DX_PI * g_fCircleBullet[nCntBullet2] + g_fRandumResultBullet2) * 3.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BOLBY, 0, nCntEnemy, -1);
					//	}
					//}
				}

				if (g_aEnemy[nCntEnemy].bUse == true && g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BANDY)
				{//エネミー11が使われていたら・・・
					


					g_fXVaim[nCntEnemy] = pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x;//X座標へのベクトル
					g_fYVaim[nCntEnemy] = pPlayer->pos.y - g_aEnemy[nCntEnemy].pos.y;//Y座標へのベクトル
				//g_Vaim[nCntEnemy] = sqrt((g_fXVaim[nCntEnemy] * g_fXVaim[nCntEnemy]) - (g_fYVaim[nCntEnemy] * g_fYVaim[nCntEnemy]));*理想とは違ったが、面白かったのでキープしておく
					/*g_Vaim[nCntEnemy] = sqrtf((g_fXVaim[nCntEnemy] * g_fXVaim[nCntEnemy])+(g_fYVaim[nCntEnemy] * g_fYVaim[nCntEnemy]));*/

					g_Vaim[nCntEnemy] = atan2(g_fXVaim[nCntEnemy], g_fYVaim[nCntEnemy]);

					switch (pGameCustom->nDifficulty)
					{
					case 1://Easy
						if (g_nDelay % 50 == 0)
						{
							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_007);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy]) * 3.0f, cosf(g_Vaim[nCntEnemy]) * 3.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy] + 0.4f) * 3.0f, cosf(g_Vaim[nCntEnemy] + 0.4f) * 3.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy] - 0.4f) * 3.0f, cosf(g_Vaim[nCntEnemy] - 0.4f) * 3.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);
						}
						break;
					case 2://Normal
						if (g_nDelay % 30 == 0)
						{
							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_007);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy]) * 3.0f, cosf(g_Vaim[nCntEnemy]) * 3.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy] + 0.4f) * 3.0f, cosf(g_Vaim[nCntEnemy] + 0.4f) * 3.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy] - 0.4f) * 3.0f, cosf(g_Vaim[nCntEnemy] - 0.4f) * 3.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);
						}
						break;
					case 3://Hard
						if (g_nDelay % 30 == 0)

						{
							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_007);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy]) * 3.0f, cosf(g_Vaim[nCntEnemy]) * 3.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);
					SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy] + 0.4f) * 3.0f, cosf(g_Vaim[nCntEnemy] + 0.4f) * 3.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);
					SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy] - 0.4f) * 3.0f, cosf(g_Vaim[nCntEnemy] - 0.4f) * 3.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);
					SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy] + 0.8f) * 3.0f, cosf(g_Vaim[nCntEnemy] + 0.8f) * 3.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);
					SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy] - 0.8f) * 3.0f, cosf(g_Vaim[nCntEnemy] - 0.8f) * 3.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);

						}
						break;
					case 4://VeryHard
						if (g_nDelay % 22 == 0)

						{
							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_007);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy]) * 3.0f, cosf(g_Vaim[nCntEnemy]) * 3.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy] + 0.4f) * 3.0f, cosf(g_Vaim[nCntEnemy] + 0.4f) * 3.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy] - 0.4f) * 3.0f, cosf(g_Vaim[nCntEnemy] - 0.4f) * 3.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy] + 0.8f) * 3.0f, cosf(g_Vaim[nCntEnemy] + 0.8f) * 3.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy] - 0.8f) * 3.0f, cosf(g_Vaim[nCntEnemy] - 0.8f) * 3.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy] + 0.2f) * 3.3f, cosf(g_Vaim[nCntEnemy] + 0.2f) * 3.3f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy] - 0.2f) * 3.3f, cosf(g_Vaim[nCntEnemy] - 0.2f) * 3.3f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy] + 0.6f) * 3.3f, cosf(g_Vaim[nCntEnemy] + 0.6f) * 3.3f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy] - 0.6f) * 3.3f, cosf(g_Vaim[nCntEnemy] - 0.6f) * 3.3f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);

						}
						break;
					}

					//SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((D3DX_PI * g_fXVaim[nCntEnemy]) * 0.004f, (D3DX_PI * g_fYVaim[nCntEnemy]) * 0.004f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy);//敵の弾だから敵同士で当たらない
					
				}



				if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS1_STARWARRIOR)
				{//ボスが使用されたら
					g_aEnemy[nCntEnemy].nCountMoveInterval++;
					/*			g_aEnemy[nCntEnemy].nMoveTime++;*/

					if (g_aEnemy[nCntEnemy].bAttackInterval == false)
					{
						g_aEnemy[nCntEnemy].nAttarkIntervalCount++;

						if (g_aEnemy[nCntEnemy].nAttarkIntervalCount >= 80)
						{
							g_aEnemy[nCntEnemy].bAttackInterval = true;
							g_aEnemy[nCntEnemy].nAttarkIntervalCount = 0;
						}
					}


					if (g_aEnemy[nCntEnemy].bAttackInterval == true)
					{
						g_nCountTechiniqueTime++;
					}

					if (g_nCountTechiniqueTime == 1)
					{
						g_nBoss1TechiniquePattern = rand() % 8 + 1;//ボスの技の種類の番号を決める
					}



					if (g_aEnemy[nCntEnemy].bBossSummonFlag == false)
					{
						g_aEnemy[nCntEnemy].bBossSummonFlag = true;
						StopSound(SOUND_LABEL_BGM001);

						PlaySound(SOUND_LABEL_BGM003);
						SetGage(nCntEnemy, GAGETYPE_BOSSHP_FRAME);//何番目の敵かを判別する必要はないので、-1番目と設定しておく。
						SetGage(nCntEnemy, GAGETYPE_BOSS1HP);

					}


					/*	if (nCountMoveTime>= 300)
						{
							g_nCountTechiniqueTime = 0;
						}*/


					if (g_aEnemy[nCntEnemy].nCountMoveInterval >= 300)
					{
						g_aEnemy[nCntEnemy].fRotMove = float((rand)() % 629 - 314) / 100.0f;
						g_aEnemy[nCntEnemy].nCountMoveInterval = 0;//次に動くまでの時間を初期化。
						g_aEnemy[nCntEnemy].bBossMoveFlag = true;//ボスを動かす
					}

					if (pGameCustom->nDifficulty != 4)
					{
						if (g_aEnemy[nCntEnemy].bBossMoveFlag == true)
						{
							g_aEnemy[nCntEnemy].nMoveTime++;
							g_aEnemy[nCntEnemy].move = D3DXVECTOR3((sinf(-D3DX_PI * g_aEnemy[nCntEnemy].fRotMove) * 2.0f), (cosf(-D3DX_PI * g_aEnemy[nCntEnemy].fRotMove) * 2.0f), 0.0f);
						}
					}
					else
					{
						if (g_aEnemy[nCntEnemy].bBossMoveFlag == true)
						{
							g_aEnemy[nCntEnemy].nMoveTime++;
							g_aEnemy[nCntEnemy].move = D3DXVECTOR3((sinf(-D3DX_PI * g_aEnemy[nCntEnemy].fRotMove) * 3.0f), (cosf(-D3DX_PI * g_aEnemy[nCntEnemy].fRotMove) * 3.0f), 0.0f);
						}
					}
					if (g_aEnemy[nCntEnemy].nMoveTime >= 100)
					{
						g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//動き終わったので、moveの値を初期化。
						g_aEnemy[nCntEnemy].nMoveTime = 0;
						g_aEnemy[nCntEnemy].bBossMoveFlag = false;

					}
					//if (g_aEnemy[nCntEnemy].bBossMoveFlag == true)
					//{
					///*	g_aEnemy[nCntEnemy].bBossMoveFlag = false;*/
					//}

					if (g_aEnemy[nCntEnemy].bAttackInterval == true)
					{//攻撃状態になったら・・・
						if (g_nBoss1TechiniquePattern == 1)
						{
							g_nCountTechiniqueTime++;
							g_fXVaim[nCntEnemy] = pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x;//X座標へのベクトル
							g_fYVaim[nCntEnemy] = pPlayer->pos.y - g_aEnemy[nCntEnemy].pos.y;//Y座標へのベクトル
						

							g_Vaim[nCntEnemy] = atan2(g_fXVaim[nCntEnemy], g_fYVaim[nCntEnemy]);

							switch (pGameCustom->nDifficulty)
							{
							case 1://Easy
								if (g_nDelay % 15 == 0)
								{
									PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_007);
									SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy]) * 8.0f, cosf(g_Vaim[nCntEnemy]) * 8.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);
									SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy] + 0.4f) * 7.0f, cosf(g_Vaim[nCntEnemy] + 0.4f) * 7.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);
									SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy] - 0.4f) * 6.0f, cosf(g_Vaim[nCntEnemy] - 0.4f) * 6.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);
								}
								break;
							case 2://Normal
								if (g_nDelay % 15==0)
								{
									PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_007);
									SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy]) * 8.0f, cosf(g_Vaim[nCntEnemy]) * 8.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);//敵の弾だから敵同士で当たらない
									SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy] + 0.2f) * 7.0f, cosf(g_Vaim[nCntEnemy] + 0.2f) * 7.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);//敵の弾だから敵同士で当たらない
									SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy] - 0.2f) * 6.0f, cosf(g_Vaim[nCntEnemy] - 0.2f) * 6.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);//敵の弾だから敵同士で当たらない
									SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy] + 0.4f) * 5.0f, cosf(g_Vaim[nCntEnemy] + 0.4) * 5.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);//敵の弾だから敵同士で当たらない
									SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy] - 0.4f) * 4.0f, cosf(g_Vaim[nCntEnemy] - 0.4f) * 4.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);//敵の弾だから敵同士で当たらない
								}
								break;
							case 3://Hard
								if (g_nDelay % 5 == 0)
								{
									PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_007);
									SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy]) * 8.0f, cosf(g_Vaim[nCntEnemy]) * 8.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);
									SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy] + 0.2f) * 7.0f, cosf(g_Vaim[nCntEnemy] + 0.2f) * 7.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);
									SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy] - 0.2f) * 6.0f, cosf(g_Vaim[nCntEnemy] - 0.2f) * 6.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);
									SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy] + 0.4f) * 5.0f, cosf(g_Vaim[nCntEnemy] + 0.4) * 5.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);
									SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy] - 0.4f) * 4.0f, cosf(g_Vaim[nCntEnemy] - 0.4f) * 4.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);
									SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy] + 0.6f) * 3.0f, cosf(g_Vaim[nCntEnemy] + 0.6f) * 3.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);
									SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy] - 0.6f) * 2.0f, cosf(g_Vaim[nCntEnemy] - 0.6f) * 2.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);
								}
								break;
							case 4://Veryhard

								if (g_nDelay %50 == 0)
								{
									PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_007);
										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(D3DX_PI * 0.5) * 10.0f, cosf(D3DX_PI * 0.5) * 10.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ROCKON, 6, -1, -1);
										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(D3DX_PI * 1.5) * 10.0f, cosf(D3DX_PI * 1.5) * 10.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ROCKON, 6, -1, -1);
									
								}
								break;
							}

							
							if (g_nCountTechiniqueTime >= 150)
							{
								g_nCountTechiniqueTime = 0;
								g_aEnemy[nCntEnemy].bAttackInterval = false;
							}
						}

						if (g_aEnemy[nCntEnemy].bUse == true && g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS1_STARWARRIOR && g_nBoss1TechiniquePattern == 2)
						{//エネミー12の攻撃



						
							int nCntBullet_E4;
							float fCircleBullet_E4_easy[8];
							float fCircleBullet_E4_normal[12];
							float fCircleBullet_E4_hard[16];
							float fCircleBullet_E4_veryhard[100];
							float fRotBullet_E4;
							fRotBullet_E4 = float((rand)() % 200) / 100.0f;

							switch (pGameCustom->nDifficulty)
							{
							case 1://Easy
								if (g_nDelay % 40 == 0)
								{
									PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_004);

									for (nCntBullet_E4 = 0; nCntBullet_E4 < 8; nCntBullet_E4++)

									{
										fCircleBullet_E4_easy[nCntBullet_E4] = (1.0f / 4.0f) * nCntBullet_E4;

										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * (fCircleBullet_E4_easy[nCntBullet_E4] + fRotBullet_E4)) * 4.0f), (cosf(-D3DX_PI * (fCircleBullet_E4_easy[nCntBullet_E4] + fRotBullet_E4)) * 4.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);
										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * (fCircleBullet_E4_easy[nCntBullet_E4] + fRotBullet_E4)) * 8.0f), (cosf(-D3DX_PI * (fCircleBullet_E4_easy[nCntBullet_E4] + fRotBullet_E4)) * 8.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);

									}
								}
								break;
							case 2://Normal
								if (g_nDelay % 25 == 0)
								{
									PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_004);

									for (nCntBullet_E4 = 0; nCntBullet_E4 < 12; nCntBullet_E4++)

									{
										fCircleBullet_E4_normal[nCntBullet_E4] = (1.0f / 6.0f) * nCntBullet_E4;

										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * (fCircleBullet_E4_normal[nCntBullet_E4] + fRotBullet_E4)) * 4.0f), (cosf(-D3DX_PI * (fCircleBullet_E4_normal[nCntBullet_E4] + fRotBullet_E4)) * 4.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);
										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * (fCircleBullet_E4_normal[nCntBullet_E4] + fRotBullet_E4)) * 8.0f), (cosf(-D3DX_PI * (fCircleBullet_E4_normal[nCntBullet_E4] + fRotBullet_E4)) * 8.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);

									}
								}
								break;
							case 3://Hard
								if (g_nDelay % 15 == 0)
								{
									PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_004);

									for (nCntBullet_E4 = 0; nCntBullet_E4 < 16; nCntBullet_E4++)

									{
										fCircleBullet_E4_hard[nCntBullet_E4] = (1.0f / 8.0f) * nCntBullet_E4;

										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * (fCircleBullet_E4_hard[nCntBullet_E4] + fRotBullet_E4)) * 4.0f), (cosf(-D3DX_PI * (fCircleBullet_E4_hard[nCntBullet_E4] + fRotBullet_E4)) * 4.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);
										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * (fCircleBullet_E4_hard[nCntBullet_E4] + fRotBullet_E4)) * 8.0f), (cosf(-D3DX_PI * (fCircleBullet_E4_hard[nCntBullet_E4] + fRotBullet_E4)) * 8.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);

									}
								}
								break;
							case 4://VeryHard
								if (g_nDelay % 70 == 0)
								{
									PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_004);

									for (nCntBullet_E4 = 0; nCntBullet_E4 < 100; nCntBullet_E4++)

									{
										fCircleBullet_E4_veryhard[nCntBullet_E4] = (1.0f / 50.0f) * nCntBullet_E4;
										//SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * (fCircleBullet_E4_veryhard[nCntBullet_E4] + fRotBullet_E4)) * 2.0f), (cosf(-D3DX_PI * (fCircleBullet_E4_veryhard[nCntBullet_E4] + fRotBullet_E4)) * 2.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);
										
										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * (fCircleBullet_E4_veryhard[nCntBullet_E4] + fRotBullet_E4)) *4.0f), (cosf(-D3DX_PI * (fCircleBullet_E4_veryhard[nCntBullet_E4] + fRotBullet_E4)) * 4.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);
										//SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * (fCircleBullet_E4_veryhard[nCntBullet_E4] + fRotBullet_E4)) * 7.0f), (cosf(-D3DX_PI * (fCircleBullet_E4_veryhard[nCntBullet_E4] + fRotBullet_E4)) * 7.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);
										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * (fCircleBullet_E4_veryhard[nCntBullet_E4] + fRotBullet_E4)) * 8.0f), (cosf(-D3DX_PI * (fCircleBullet_E4_veryhard[nCntBullet_E4] + fRotBullet_E4)) * 8.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);
										
									
									}
								}
								break;

							}


							if (g_nCountTechiniqueTime >= 150)
							{
								g_nCountTechiniqueTime = 0;
								g_aEnemy[nCntEnemy].bAttackInterval = false;
							}

							//SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((D3DX_PI * g_fXVaim[nCntEnemy]) * 0.004f, (D3DX_PI * g_fYVaim[nCntEnemy]) * 0.004f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy);//敵の弾だから敵同士で当たらない
						}
						if (g_aEnemy[nCntEnemy].bUse == true &&g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS1_STARWARRIOR && g_nBoss1TechiniquePattern == 3)
						{//エネミー12の攻撃(SetBullet)
						//	SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * 0.0f) * 5.0f), (cosf(-D3DX_PI * 0.0f) * 5.0f), 0.0f), 10, BULLETTYPE_REAFY);//敵の弾だから敵同士で当たらない
							int nCntBullet_E5;
							float fCircleBullet_E5_easy[4];
							float fCircleBullet_E5_normal[12];
							float fCircleBullet_E5_hard[16];
							float fCircleBullet_E5_veryhard[24];
							switch (pGameCustom->nDifficulty)
							{
								
							case 1:
								if (g_nDelay % 30 == 0)
								{
									PlaySound(SOUND_LABEL_SE_ARROWSHOT_000);
									for (nCntBullet_E5 = 0; nCntBullet_E5 < 4; nCntBullet_E5++)
									{
										
										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_REAFY, nCntBullet_E5, nCntEnemy, -1);//敵の弾だから敵同士で当たらない
										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_REVERSE_CURVE, nCntBullet_E5, nCntEnemy, -1);//敵の弾だから敵同士で当たらない

									}
								}
								break;
							case 2:
								if (g_nDelay % 20 == 0)
								{
									PlaySound(SOUND_LABEL_SE_ARROWSHOT_000);
									for (nCntBullet_E5 = 0; nCntBullet_E5 < 8; nCntBullet_E5++)
									{
										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_REAFY, nCntBullet_E5, nCntEnemy, -1);//敵の弾だから敵同士で当たらない
										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_REVERSE_CURVE, nCntBullet_E5, nCntEnemy, -1);//敵の弾だから敵同士で当たらない

									}
								}
								break;
							case 3:
								if (g_nDelay % 20 == 0)
								{
									PlaySound(SOUND_LABEL_SE_ARROWSHOT_000);
									for (nCntBullet_E5 = 0; nCntBullet_E5 < 14; nCntBullet_E5++)
									{
										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_REAFY, nCntBullet_E5, nCntEnemy, -1);//敵の弾だから敵同士で当たらない
										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_REVERSE_CURVE, nCntBullet_E5, nCntEnemy, -1);//敵の弾だから敵同士で当たらない

									}
								}
								break;
							case 4:
								if (g_nDelay % 15 == 0)
								{
									PlaySound(SOUND_LABEL_SE_ARROWSHOT_000);
									for (nCntBullet_E5 = 0; nCntBullet_E5 < 20; nCntBullet_E5++)
									{
										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_REAFY, nCntBullet_E5, nCntEnemy, -1);//敵の弾だから敵同士で当たらない
										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_REVERSE_CURVE, nCntBullet_E5, nCntEnemy, -1);//敵の弾だから敵同士で当たらない

									}
								}
								break;

							}
							if (g_nCountTechiniqueTime >= 300)
							{
								g_nCountTechiniqueTime = 0;
								g_aEnemy[nCntEnemy].bAttackInterval = false;
							}
						}
						if (g_aEnemy[nCntEnemy].bUse == true &&g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS1_STARWARRIOR && g_nBoss1TechiniquePattern == 4)
						{//エネミー12の攻撃


							float fCircleBullet_E12[4];
							int nCntBullet_E12;
							float fCircleBullet_E12_easy[2];
							float fCircleBullet_E12_normal[4];
							float fCircleBullet_E12_hard[4];
							float fCircleBullet_E12_veryhard[4];



							switch (pGameCustom->nDifficulty)
							{
							case 1://Easy
								if (g_nDelay % 75 == 0)
								{
							      PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_001);
									for (nCntBullet2 = 0; nCntBullet2 < 2; nCntBullet2++)
									{
										fCircleBullet_E12_easy[nCntBullet2] = 1.0f* nCntBullet2;


										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * fCircleBullet_E12_easy[nCntBullet2] + g_fRandumResultBullet2) * fRandumSpeed_E12), (cosf(-D3DX_PI * fCircleBullet_E12_easy[nCntBullet2] + g_fRandumResultBullet2) * fRandumSpeed_E12), 0.0f), GENERALLY_BULLETLIFE / 5, BULLETTYPE_SCATTERING, 0, nCntEnemy, -1);//敵の弾だから敵同士で当たらない		}

									}
								}
								break;
							case 2://Normal
								if (g_nDelay % 50 == 0)
								{
									PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_001);
									for (nCntBullet2 = 0; nCntBullet2 < 4 ;nCntBullet2++)
									{
										fCircleBullet_E12_normal[nCntBullet2] = (1.0f/2.0f) * nCntBullet2;


										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * fCircleBullet_E12_normal[nCntBullet2] + g_fRandumResultBullet2) * fRandumSpeed_E12), (cosf(-D3DX_PI * fCircleBullet_E12_normal[nCntBullet2] + g_fRandumResultBullet2) * fRandumSpeed_E12), 0.0f), GENERALLY_BULLETLIFE / 5, BULLETTYPE_SCATTERING, 0, nCntEnemy, -1);//敵の弾だから敵同士で当たらない		}

									}
								}
								break;
							case 3://Hard
								if (g_nDelay % 30 == 0)
								{
									PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_001);
									for (nCntBullet2 = 0; nCntBullet2 < 4; nCntBullet2++)
									{
										fCircleBullet_E12_normal[nCntBullet2] = (1.0f / 2.0f) * nCntBullet2;


										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * fCircleBullet_E12_normal[nCntBullet2] + g_fRandumResultBullet2) * fRandumSpeed_E12), (cosf(-D3DX_PI * fCircleBullet_E12_normal[nCntBullet2] + g_fRandumResultBullet2) * fRandumSpeed_E12), 0.0f), GENERALLY_BULLETLIFE / 6, BULLETTYPE_SCATTERING, 0, nCntEnemy, -1);//敵の弾だから敵同士で当たらない		}

									}
								}
								break;
							case 4://VeryHard
								if (g_nDelay % 40 == 0)
								{
									PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_001);
									for (nCntBullet2 = 0; nCntBullet2 < 4; nCntBullet2++)
									{
										fCircleBullet_E12_veryhard[nCntBullet2] = (1.0f / 2.0f) * nCntBullet2;


										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * fCircleBullet_E12_veryhard[nCntBullet2] + g_fRandumResultBullet2) * fRandumSpeed_E12), (cosf(-D3DX_PI * fCircleBullet_E12_veryhard[nCntBullet2] + g_fRandumResultBullet2) * fRandumSpeed_E12), 0.0f), GENERALLY_BULLETLIFE / 5, BULLETTYPE_SCATTERING, 0, nCntEnemy, -1);//敵の弾だから敵同士で当たらない		}

									}
								}
								break;
							}


							//for (nCntBullet2 = 0; nCntBullet2 < 4; nCntBullet2++)
							//{
							//	fCircleBullet_E12[nCntBullet2] = (1.0f / 2.0f) * nCntBullet2;


							//	SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * fCircleBullet_E12[nCntBullet2] + g_fRandumResultBullet2) * fRandumSpeed_E12), (cosf(-D3DX_PI * fCircleBullet_E12[nCntBullet2] + g_fRandumResultBullet2) * fRandumSpeed_E12), 0.0f), GENERALLY_BULLETLIFE/4, BULLETTYPE_SCATTERING, 0, nCntEnemy, -1);//敵の弾だから敵同士で当たらない		}

							//}

							if (g_nCountTechiniqueTime >= 150)
							{
								g_nCountTechiniqueTime = 0;
								g_aEnemy[nCntEnemy].bAttackInterval = false;
							}

							//SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((D3DX_PI * g_fXVaim[nCntEnemy]) * 0.004f, (D3DX_PI * g_fYVaim[nCntEnemy]) * 0.004f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy);//敵の弾だから敵同士で当たらない
						}
						if (g_aEnemy[nCntEnemy].bUse == true &&g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS1_STARWARRIOR && g_nBoss1TechiniquePattern == 5)
						{//エネミー12の攻撃 （ダブルウインド）
							float fCircleBullet_E12[4];
							float fRotMove;
							float fRandumSpeed_E12_W5;


							switch (pGameCustom->nDifficulty)
							{
							case 1://Easy
								if (g_nDelay % 100 == 0)
								{
									PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_008);
									for (nCntBullet2 = 0; nCntBullet2 < 5; nCntBullet2++)
									{

										fRotMove = float((rand)() % 100 + 1) / 100.0f;//半分の方向に弾を発射する。(0.0f～1.0f)
										fRandumSpeed_E12_W5 = float((rand)() % 150 + 1) / 10.0f;//半分の方向に弾を発射する。(0.0f～1.0f)
										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * fRotMove) * fRandumSpeed_E12_W5), (cosf(-D3DX_PI * fRotMove) * fRandumSpeed_E12_W5), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_RIGHT_WIND, 0, nCntEnemy, -1);//敵の弾だから敵同士で当たらない		}

									}

									for (nCntBullet2 = 0; nCntBullet2 < 5; nCntBullet2++)
									{

										fRotMove = float((rand)() % 100 + 1) / 100.0f;//半分の方向に弾を発射する。(0.0f～1.0f)
										fRandumSpeed_E12_W5 = float((rand)() % 150 + 1) / 10.0f;//半分の方向に弾を発射する。(0.0f～1.0f)
										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(D3DX_PI * fRotMove) * fRandumSpeed_E12_W5), (cosf(D3DX_PI * fRotMove) * fRandumSpeed_E12_W5), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_LEFT_WIND, 0, nCntEnemy, -1);//敵の弾だから敵同士で当たらない		}

									}
								}
							break;
							case 2://Normal
								if (g_nDelay % 80 == 0)
								{
									PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_008);
									for (nCntBullet2 = 0; nCntBullet2 < 15; nCntBullet2++)
									{

										fRotMove = float((rand)() % 100 + 1) / 100.0f;//半分の方向に弾を発射する。(0.0f～1.0f)
										fRandumSpeed_E12_W5 = float((rand)() % 150 + 1) / 10.0f;//半分の方向に弾を発射する。(0.0f～1.0f)
										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * fRotMove) * fRandumSpeed_E12_W5), (cosf(-D3DX_PI * fRotMove) * fRandumSpeed_E12_W5), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_RIGHT_WIND, 0, nCntEnemy, -1);//敵の弾だから敵同士で当たらない		}

									}

									for (nCntBullet2 = 0; nCntBullet2 < 15; nCntBullet2++)
									{

										fRotMove = float((rand)() % 100 + 1) / 100.0f;//半分の方向に弾を発射する。(0.0f～1.0f)
										fRandumSpeed_E12_W5 = float((rand)() % 150 + 1) / 10.0f;//半分の方向に弾を発射する。(0.0f～1.0f)
										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(D3DX_PI * fRotMove) * fRandumSpeed_E12_W5), (cosf(D3DX_PI * fRotMove) * fRandumSpeed_E12_W5), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_LEFT_WIND, 0, nCntEnemy, -1);//敵の弾だから敵同士で当たらない		}

									}
								}
								break;
							case 3://Hard
								if (g_nDelay % 60 == 0)
								{
									PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_008);
									for (nCntBullet2 = 0; nCntBullet2 < 30; nCntBullet2++)
									{

										fRotMove = float((rand)() % 100 + 1) / 100.0f;//半分の方向に弾を発射する。(0.0f～1.0f)
										fRandumSpeed_E12_W5 = float((rand)() % 150 + 1) / 10.0f;//半分の方向に弾を発射する。(0.0f～1.0f)
										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * fRotMove) * fRandumSpeed_E12_W5), (cosf(-D3DX_PI * fRotMove) * fRandumSpeed_E12_W5), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_RIGHT_WIND, 0, nCntEnemy, -1);//敵の弾だから敵同士で当たらない		}

									}

									for (nCntBullet2 = 0; nCntBullet2 < 30; nCntBullet2++)
									{

										fRotMove = float((rand)() % 100 + 1) / 100.0f;//半分の方向に弾を発射する。(0.0f～1.0f)
										fRandumSpeed_E12_W5 = float((rand)() % 150 + 1) / 10.0f;//半分の方向に弾を発射する。(0.0f～1.0f)
										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(D3DX_PI * fRotMove) * fRandumSpeed_E12_W5), (cosf(D3DX_PI * fRotMove) * fRandumSpeed_E12_W5), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_LEFT_WIND, 0, nCntEnemy, -1);//敵の弾だから敵同士で当たらない		}

									}
								}
								break;
							case 4://VeryHard
								if (g_nDelay % 75 == 0)
								{
									PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_008);
									for (nCntBullet2 = 0; nCntBullet2 < 40; nCntBullet2++)
									{

										fRotMove = float((rand)() % 100 + 1) / 100.0f;//半分の方向に弾を発射する。(0.0f～1.0f)
										fRandumSpeed_E12_W5 = float((rand)() % 150 + 1) / 10.0f;//半分の方向に弾を発射する。(0.0f～1.0f)
										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * fRotMove) * fRandumSpeed_E12_W5), (cosf(-D3DX_PI * fRotMove) * fRandumSpeed_E12_W5), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_RIGHT_WIND, 0, nCntEnemy, -1);//敵の弾だから敵同士で当たらない		}

									}

									for (nCntBullet2 = 0; nCntBullet2 < 40; nCntBullet2++)
									{

										fRotMove = float((rand)() % 100 + 1) / 100.0f;//半分の方向に弾を発射する。(0.0f～1.0f)
										fRandumSpeed_E12_W5 = float((rand)() % 150 + 1) / 10.0f;//半分の方向に弾を発射する。(0.0f～1.0f)
										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(D3DX_PI * fRotMove) * fRandumSpeed_E12_W5), (cosf(D3DX_PI * fRotMove) * fRandumSpeed_E12_W5), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_LEFT_WIND, 0, nCntEnemy, -1);//敵の弾だから敵同士で当たらない		}

									}
									for (nCntBullet2 = 0; nCntBullet2 < 40; nCntBullet2++)
									{

										fRotMove = float((rand)() % 150 + 51) / 100.0f;//半分の方向に弾を発射する。(0.0f～1.0f)
										fRandumSpeed_E12_W5 = float((rand)() % 150 + 1) / 10.0f;//半分の方向に弾を発射する。(0.0f～1.0f)
										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(D3DX_PI * fRotMove) * fRandumSpeed_E12_W5), (cosf(D3DX_PI * fRotMove) * fRandumSpeed_E12_W5), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_DOWNWIND, 0, nCntEnemy, -1);//敵の弾だから敵同士で当たらない		}

									}
									for (nCntBullet2 = 0; nCntBullet2 < 40; nCntBullet2++)
									{

										fRotMove = float((rand)() % 150 + 51) / 100.0f;//半分の方向に弾を発射する。(0.0f～1.0f)
										fRandumSpeed_E12_W5 = float((rand)() % 150 + 1) / 10.0f;//半分の方向に弾を発射する。(0.0f～1.0f)
										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * fRotMove) * fRandumSpeed_E12_W5), (cosf(-D3DX_PI * fRotMove) * fRandumSpeed_E12_W5), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_UPWIND, 0, nCntEnemy, -1);//敵の弾だから敵同士で当たらない		}

									}
								}
								break;
							}

							//for (nCntBullet2 = 0; nCntBullet2 < 30; nCntBullet2++)
							//{

							//	fRotMove = float((rand)() % 100 + 1) / 100.0f;//半分の方向に弾を発射する。(0.0f～1.0f)
							//	fRandumSpeed_E12_W5 = float((rand)() % 150 + 1) / 10.0f;//半分の方向に弾を発射する。(0.0f～1.0f)
							//	SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * fRotMove) * fRandumSpeed_E12_W5), (cosf(-D3DX_PI * fRotMove) * fRandumSpeed_E12_W5), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_RIGHT_WIND, 0, nCntEnemy, -1);//敵の弾だから敵同士で当たらない		}

							//}

							//for (nCntBullet2 = 0; nCntBullet2 < 30; nCntBullet2++)
							//{

							//	fRotMove = float((rand)() % 100 + 1) / 100.0f;//半分の方向に弾を発射する。(0.0f～1.0f)
							//	fRandumSpeed_E12_W5 = float((rand)() % 150 + 1) / 10.0f;//半分の方向に弾を発射する。(0.0f～1.0f)
							//	SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(D3DX_PI * fRotMove) * fRandumSpeed_E12_W5), (cosf(D3DX_PI * fRotMove) * fRandumSpeed_E12_W5), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_LEFT_WIND, 0, nCntEnemy, -1);//敵の弾だから敵同士で当たらない		}

							//}
							if (g_nCountTechiniqueTime >= 150)
							{
								g_nCountTechiniqueTime = 0;
								g_aEnemy[nCntEnemy].bAttackInterval = false;
							}

							//SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((D3DX_PI * g_fXVaim[nCntEnemy]) * 0.004f, (D3DX_PI * g_fYVaim[nCntEnemy]) * 0.004f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy);//敵の弾だから敵同士で当たらない
						}
						if (g_aEnemy[nCntEnemy].bUse == true && g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS1_STARWARRIOR && g_nBoss1TechiniquePattern == 6)
						{//エネミー12の攻撃 （反射弾）

							float fCircleBullet_easy[4];
							float fCircleBullet_normal[6];
							float fCircleBullet_hard[6];
							float fCircleBullet_veryhard[6];


							switch (pGameCustom->nDifficulty)
							{
							case 1:
								if (g_nDelay % 35 == 0)
								{
									PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_001);

									for (nCntBullet2 = 0; nCntBullet2 < 4; nCntBullet2++)
									{
										fCircleBullet_easy[nCntBullet2] = (1.0f / 2.0f) * nCntBullet2;

										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * (fCircleBullet_easy[nCntBullet2] + g_fRotBullet_W6)) * 3.0f), (cosf(-D3DX_PI * (fCircleBullet_easy[nCntBullet2] + g_fRotBullet_W6)) * 3.0f), 0.0f), GENERALLY_BULLETLIFE / 5, BULLETTYPE_REFLECTION, 0, nCntEnemy, -1);//敵の弾だから敵同士で当たらない		}

									}
								}
								break;
							case 2:
								if (g_nDelay % 30 == 0)
								{
									PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_001);

									for (nCntBullet2 = 0; nCntBullet2 < 6; nCntBullet2++)
									{
										fCircleBullet_normal[nCntBullet2] = (1.0f / 3.0f) * nCntBullet2;

										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * (fCircleBullet_normal[nCntBullet2] + g_fRotBullet_W6)) * 3.0f), (cosf(-D3DX_PI * (fCircleBullet_normal[nCntBullet2] + g_fRotBullet_W6)) * 3.0f), 0.0f), GENERALLY_BULLETLIFE / 5, BULLETTYPE_REFLECTION, 0, nCntEnemy, -1);//敵の弾だから敵同士で当たらない		}

									}
								}
								break;
							case 3:
								if (g_nDelay % 17 == 0)
								{
									PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_001);

									for (nCntBullet2 = 0; nCntBullet2 < 6; nCntBullet2++)
									{
										fCircleBullet_hard[nCntBullet2] = (1.0f / 3.0f) * nCntBullet2;

										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * (fCircleBullet_hard[nCntBullet2] + g_fRotBullet_W6)) * 3.0f), (cosf(-D3DX_PI * (fCircleBullet_hard[nCntBullet2] + g_fRotBullet_W6)) * 3.0f), 0.0f), GENERALLY_BULLETLIFE / 5, BULLETTYPE_REFLECTION, 0, nCntEnemy, -1);//敵の弾だから敵同士で当たらない		}

									}
								}
								break;
							case 4:
								if (g_nDelay % 10 == 0)
								{
									PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_001);

									for (nCntBullet2 = 0; nCntBullet2 < 6; nCntBullet2++)
									{
										fCircleBullet_veryhard[nCntBullet2] = (1.0f / 3.0f) * nCntBullet2;

										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * (fCircleBullet_veryhard[nCntBullet2] + g_fRotBullet_W6)) * 3.0f), (cosf(-D3DX_PI * (fCircleBullet_veryhard[nCntBullet2] + g_fRotBullet_W6)) * 3.0f), 0.0f), GENERALLY_BULLETLIFE / 5, BULLETTYPE_REFLECTION, 0, nCntEnemy, -1);//敵の弾だから敵同士で当たらない		}

									}
								}
								break;
							}
							/*PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_001);*/

							//for (nCntBullet2 = 0; nCntBullet2 < 6; nCntBullet2++)
							//{
							//	fCircleBullet[nCntBullet2] = (1.0f / 3.0f) * nCntBullet2;

							//	SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * (fCircleBullet[nCntBullet2] + g_fRotBullet_W6)) * 3.0f), (cosf(-D3DX_PI * (fCircleBullet[nCntBullet2] + g_fRotBullet_W6)) * 3.0f), 0.0f), GENERALLY_BULLETLIFE / 5, BULLETTYPE_REFLECTION, 0, nCntEnemy, -1);//敵の弾だから敵同士で当たらない		}

							//}
							if (g_nCountTechiniqueTime >= 150)
							{
								g_nCountTechiniqueTime = 0;
								g_aEnemy[nCntEnemy].bAttackInterval = false;
							}

							//SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((D3DX_PI * g_fXVaim[nCntEnemy]) * 0.004f, (D3DX_PI * g_fYVaim[nCntEnemy]) * 0.004f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy);//敵の弾だから敵同士で当たらない
						}
						if (g_aEnemy[nCntEnemy].bUse == true &&g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS1_STARWARRIOR && g_nBoss1TechiniquePattern == 7)
						{//エネミー12の攻撃 (ミストバースト）

							float fCircleBullet[6];
							float fRandumPosX;
							float fRandumPosY;
							
							fRandumPosX= float((rand)() % SCREEN_WIDTH+1);
							fRandumPosY= float((rand)() % SCREEN_HEIGHT+1);
							switch (pGameCustom->nDifficulty)
							{
							case 1://Easy
								if (g_nDelay % 60 == 0)
								{
									PlaySound(SOUND_LABEL_SE_WARNING_001);
									SetStaging(STAGINGTYPE_BULLETWARNING, -1, D3DXVECTOR3(fRandumPosX, fRandumPosY, 0.0f));
								}
								break;
							case 2://Normal
								if (g_nDelay % 35 == 0)
								{
									PlaySound(SOUND_LABEL_SE_WARNING_001);
									SetStaging(STAGINGTYPE_BULLETWARNING, -1, D3DXVECTOR3(fRandumPosX, fRandumPosY, 0.0f));
								}
								break;
							case 3://Hard
								if (g_nDelay % 17== 0)
								{
									PlaySound(SOUND_LABEL_SE_WARNING_001);
									SetStaging(STAGINGTYPE_BULLETWARNING, -1, D3DXVECTOR3(fRandumPosX, fRandumPosY, 0.0f));
								}
								break;
							case 4://Hard
								if (g_nDelay % 15 == 0)
								{
									PlaySound(SOUND_LABEL_SE_WARNING_001);
									SetStaging(STAGINGTYPE_BULLETWARNING, -1, D3DXVECTOR3(fRandumPosX, fRandumPosY, 0.0f));
								}
								if (g_nDelay % 30 == 0)
								{
									PlaySound(SOUND_LABEL_SE_WARNING_001);
									SetStaging(STAGINGTYPE_BULLETWARNING, -1, D3DXVECTOR3(pPlayer->pos.x,pPlayer->pos.y, 0.0f));
								}
								break;
							}
							
						
							if (g_nCountTechiniqueTime >= 400)
							{
								g_nCountTechiniqueTime = 0;
								g_aEnemy[nCntEnemy].bAttackInterval = false;
							}

							//SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((D3DX_PI * g_fXVaim[nCntEnemy]) * 0.004f, (D3DX_PI * g_fYVaim[nCntEnemy]) * 0.004f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy);//敵の弾だから敵同士で当たらない
						}
						if (g_aEnemy[nCntEnemy].bUse == true && g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS1_STARWARRIOR && g_nBoss1TechiniquePattern == 8)
						{//エネミー12の攻撃(衛星弾）
						//	SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * 0.0f) * 5.0f), (cosf(-D3DX_PI * 0.0f) * 5.0f), 0.0f), 10, BULLETTYPE_REAFY);//敵の弾だから敵同士で当たらない

							

							int nCntBullet_E5;
							float fCircleBullet_E5_easy[4];
							float fCircleBullet_E5_normal[12];
							float fCircleBullet_E5_hard[16];
							float fCircleBullet_E5_veryhard[24];
							switch (pGameCustom->nDifficulty)
							{

							case 1:
								if (g_nDelay % 60 == 0)
								{
									PlaySound(SOUND_LABEL_SE_ARROWSHOT_000);
									for (nCntBullet_E5 = 0; nCntBullet_E5 < 4; nCntBullet_E5++)
									{

										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), GENERALLY_BULLETLIFE-1200, BULLETTYPE_SATELLITE, nCntBullet_E5, nCntEnemy, -1);//敵の弾だから敵同士で当たらない
										
									}
								}
								break;
							case 2:
								if (g_nDelay %50 == 0)
								{
									PlaySound(SOUND_LABEL_SE_ARROWSHOT_000);
									for (nCntBullet_E5 = 0; nCntBullet_E5 < 8; nCntBullet_E5++)
									{
										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), GENERALLY_BULLETLIFE-1200, BULLETTYPE_SATELLITE, nCntBullet_E5, nCntEnemy, -1);//敵の弾だから敵同士で当たらない
										
									}
								}
								break;
							case 3:
								if (g_nDelay % 25 == 0)
								{
									PlaySound(SOUND_LABEL_SE_ARROWSHOT_000);
									for (nCntBullet_E5 = 0; nCntBullet_E5 < 14; nCntBullet_E5++)
									{
										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), GENERALLY_BULLETLIFE-1200, BULLETTYPE_SATELLITE, nCntBullet_E5, nCntEnemy, -1);//敵の弾だから敵同士で当たらない
										
									}
								}
								break;
							case 4:
								if (g_nDelay % 15 == 0)
								{
									PlaySound(SOUND_LABEL_SE_ARROWSHOT_000);
									for (nCntBullet_E5 = 0; nCntBullet_E5 < 20; nCntBullet_E5++)
									{
										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), GENERALLY_BULLETLIFE-1200, BULLETTYPE_SATELLITE, nCntBullet_E5, nCntEnemy, -1);//敵の弾だから敵同士で当たらない
										
									}
								}
								break;

							}
							if (g_nCountTechiniqueTime >= 150)
							{
								g_nCountTechiniqueTime = 0;
								g_aEnemy[nCntEnemy].bAttackInterval = false;
							}
						}
					}
				}


			

				switch (g_aEnemy[nCntEnemy].State)
				{
				case ENEMYSTATE_NORMAL:
					if (g_nDelay % 60 == 0)
					{
						//SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * 0.0f) * 5.0f), (cosf(-D3DX_PI * 0.0f) * 5.0f), 0.0f), 10, BULLETTYPE_ENEMY);//敵の弾だから敵同士で当たらない
					}
					break;
				case ENEMYSTATE_DAMAGE:
					g_aEnemy[nCntEnemy].nCounterState--;
					if (g_aEnemy[nCntEnemy].nCounterState <= 0)
					{
						g_aEnemy[nCntEnemy].State = ENEMYSTATE_DAMAGE;
						//頂点カラーの設定
						pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//通常色に戻す。
						pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//通常色に戻す。
						pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//通常色に戻す。
						pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//通常色に戻す。
					}
					break;
				}

				


				//==============================
				//エネミーの動き
				//==============================
				
			    if (g_aEnemy[nCntEnemy].pos.x > SCREEN_WIDTH || g_aEnemy[nCntEnemy].pos.x < 0.0f)
				{
					if (g_aEnemy[nCntEnemy].bEnemyMoveX == false)
					{
						g_aEnemy[nCntEnemy].bEnemyMoveX = true;
					}
					else if (g_aEnemy[nCntEnemy].bEnemyMoveX == true)
					{
						g_aEnemy[nCntEnemy].bEnemyMoveX = false;
					}
				}

				if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS1_STARWARRIOR)
				{

					if (g_aEnemy[nCntEnemy].pos.y > SCREEN_HEIGHT / 3 || g_aEnemy[nCntEnemy].pos.y < 20.0f)
					{

						if (g_aEnemy[nCntEnemy].bEnemyMoveY == false)
						{
							g_aEnemy[nCntEnemy].bEnemyMoveY = true;
						}
						else if (g_aEnemy[nCntEnemy].bEnemyMoveY == true)
						{
							g_aEnemy[nCntEnemy].bEnemyMoveY = false;
						}
					}
				}
				else if (g_aEnemy[nCntEnemy].pos.y > SCREEN_HEIGHT || g_aEnemy[nCntEnemy].pos.y < 0.0f)
				{
					if (g_aEnemy[nCntEnemy].bEnemyMoveY == false)
					{
						g_aEnemy[nCntEnemy].bEnemyMoveY = true;
					}
					else if (g_aEnemy[nCntEnemy].bEnemyMoveY == true)
					{
						g_aEnemy[nCntEnemy].bEnemyMoveY = false;
					}
				}


				if (g_aEnemy[nCntEnemy].bEnemyMoveX == true)
				{
					g_aEnemy[nCntEnemy].pos.x -= g_aEnemy[nCntEnemy].move.x;
				}
				if (g_aEnemy[nCntEnemy].bEnemyMoveX == false)
				{
					g_aEnemy[nCntEnemy].pos.x += g_aEnemy[nCntEnemy].move.x;
				}
				if (g_aEnemy[nCntEnemy].bEnemyMoveY == true)
				{
					g_aEnemy[nCntEnemy].pos.y -= g_aEnemy[nCntEnemy].move.y;
				}
				if (g_aEnemy[nCntEnemy].bEnemyMoveY == false)
				{
					g_aEnemy[nCntEnemy].pos.y += g_aEnemy[nCntEnemy].move.y;
				}
			

				if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOMUBY|| g_aEnemy[nCntEnemy].nType == ENEMYTYPE_MINIBOMUBY)
				{


					float fangle[MAX_ENEMY];

					fangle[nCntEnemy] = atan2(pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x, pPlayer->pos.y - g_aEnemy[nCntEnemy].pos.y);

					g_aEnemy[nCntEnemy].move = D3DXVECTOR3((pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x) * 0.01f, (pPlayer->pos.y - g_aEnemy[nCntEnemy].pos.y) * 0.01f, 0.0f);//中心座標を動かす
				}
			}
		}
	
		/*if (g_aEnemy[nCntEnemy].fLife <= 1.0f)
		{
			g_aEnemy[nCntEnemy].bUse = false;
		}*/
		
	

		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffEnemy->Unlock();

	/*if (g_aResult.bResultFlag == true)
	{
		SetMode(MODE_RESULT);
	}*/
}

//=========================================
//敵の描画処理
//=========================================
void DrawEnemy(void)
{

	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	int nCntEnemy;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEnemy, 0, sizeof(VERTEX_2D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)//ポリゴンの描画
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{//敵が使用されている
			if (g_aEnemy[nCntEnemy].nApperCount >= 100)
			{

				if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_DELBY)
				{
					pDevice->SetTexture(0, g_apTextureEnemy[ENEMYTYPE_DELBY]);
				}
				if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_PUNDY)
				{
					pDevice->SetTexture(0, g_apTextureEnemy[ENEMYTYPE_PUNDY]);
				}
				if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_SMILY)
				{
					pDevice->SetTexture(0, g_apTextureEnemy[ENEMYTYPE_SMILY]);
				}
				if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_SQUDY)
				{
					pDevice->SetTexture(0, g_apTextureEnemy[ENEMYTYPE_SQUDY]);
				}
				if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_REAFY)
				{
					pDevice->SetTexture(0, g_apTextureEnemy[ENEMYTYPE_REAFY]);
				}
				if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_HONOBY)
				{
					pDevice->SetTexture(0, g_apTextureEnemy[ENEMYTYPE_HONOBY]);
				}
				if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_MIZUBY)
				{
					pDevice->SetTexture(0, g_apTextureEnemy[ENEMYTYPE_MIZUBY]);
				}
				if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOMUBY|| g_aEnemy[nCntEnemy].nType == ENEMYTYPE_MINIBOMUBY)
				{
					pDevice->SetTexture(0, g_apTextureEnemy[ENEMYTYPE_BOMUBY]);
				}
				if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_WINBY)
				{
					pDevice->SetTexture(0, g_apTextureEnemy[ENEMYTYPE_WINBY]);
				}
				if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOLBY)
				{
					pDevice->SetTexture(0, g_apTextureEnemy[ENEMYTYPE_BOLBY]);
				}
				if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BANDY)
				{
					pDevice->SetTexture(0, g_apTextureEnemy[ENEMYTYPE_BANDY]);
				}
				if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS1_STARWARRIOR)
				{
					pDevice->SetTexture(0, g_apTextureEnemy[ENEMYTYPE_BOSS1_STARWARRIOR]);
				}
				/*else
				{
					pDevice->SetTexture(0, g_apTextureEnemy[nCntEnemy]);
				}*/

				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,//0は、描画する最初の頂点インデックスで、2は、描画するプリミティブ数
					nCntEnemy * 4,
					2);
			}
		}
	}


}

//=================================
//敵の設定処理
//=================================
void SetEnemy(D3DXVECTOR3 pos, int nType, float fLife, D3DXVECTOR3 move)
{

	int nCntEnemy;
	VERTEX_2D* pVtx;
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == false)
		{//敵が使用されていない
			g_nCntEnemyParticle[nCntEnemy] = 0;
			g_bSetParticle[nCntEnemy] = false;

			g_aEnemy[nCntEnemy].pos = pos;
			//pVtx += (nCntEnemy * 4);
			pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 30.0f, g_aEnemy[nCntEnemy].pos.y - 30.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 30.0f, g_aEnemy[nCntEnemy].pos.y - 30.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 30.0f, g_aEnemy[nCntEnemy].pos.y + 30.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 30.0f, g_aEnemy[nCntEnemy].pos.y + 30.0f, 0.0f);
			g_aEnemy[nCntEnemy].nType = nType;
			g_aEnemy[nCntEnemy].bUse = true;//フラグをtrueにし、敵が出現したときに表示されるようにする。
			g_aEnemy[nCntEnemy].nCounterState = 100;//状態を変える時間を表すカウンター
			g_aEnemy[nCntEnemy].fLife = fLife;//体力
			g_aEnemy[nCntEnemy].State = ENEMYSTATE_NORMAL;
			g_aEnemy[nCntEnemy].bSetEnemyUse = true;
			g_aEnemy[nCntEnemy].bDefeatFlag = false;
			g_aEnemy[nCntEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aEnemy[nCntEnemy].move = move;//移動量
			g_aEnemy[nCntEnemy].nCountMoveInterval = 0;
			g_aEnemy[nCntEnemy].nMoveTime = 0;

			g_aEnemy[nCntEnemy].bAttackInterval = false;

			g_aEnemy[nCntEnemy].nAttarkIntervalCount = 0;

			g_aEnemy[nCntEnemy].bBossSummonFlag = false;//ボス召喚フラグ
			g_aEnemy[nCntEnemy].fBossMove = 0.0f;
			g_aEnemy[nCntEnemy].bBossMoveFlag = false;

			g_aEnemy[nCntEnemy].nApperCount = 0;

			g_aEnemy[nCntEnemy].bEnemyCountminusFlag = false;

			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//赤色にする
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//赤色にする
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//赤色にする
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//赤色にする

			SetStaging(STAGINGTYPE_APPER, nCntEnemy,pos);//敵が出現する予定の位置に敵が出現する予告をする。。


			g_aDefeat.DefeatEnemyCount++;//場にいる残りエネミー数をセットされた分プラスする。

			//g_nNumEnemy++;//敵の総数カウントアップ
			break;
		}
		pVtx += 4;
	}
	g_pVtxBuffEnemy->Unlock();
}

//====================================
//敵のヒット処理
//====================================
void HitEnemy(int nCntEnemy,float fDamage)
{
	int nCnt;
	GameCustom* pGameCustom = GetGameCustom();
	ResultType* pResultType = GetResultType();//リザルトの種類の構造体のアドレスと情報を取得
	Result* pResult = GetResult();
	VERTEX_2D* pVtx;
	Player* pPlayer = GetPlayer();
	g_aEnemy[nCntEnemy].fLife -= fDamage;
	if (g_aEnemy[nCntEnemy].fLife <= 0.0f)
	{//敵の体力が０になったら・・・

         		PlaySound(SOUND_LABEL_SE_DEFEAT_ENEMY);

		SetExplosion(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), EXPLOSIONTYPE_NORMAL);//敵がいる位置で爆発させる。

		int nNomissScore;
		int	nNotSkillUseBonus;
		g_aEnemy[nCntEnemy].bUse = false;

		

	//===================================
	//ダメージ表記処理
	//===================================
		float fRandumRot;
		float fRandumSpeed;
		fRandumRot = float((rand)() % 629 - 314) / 100.0f;
		fRandumSpeed = float((rand)() % 30) / 10.0f;

		if (fDamage <= 1 && fDamage >= 0)
		{
			SetDamage(fDamage, 100, D3DXVECTOR3((sinf(-D3DX_PI * fRandumRot) * fRandumSpeed), (cosf(-D3DX_PI * fRandumRot) * fRandumSpeed), 0.0f), g_aEnemy[nCntEnemy].pos, 10.0f);

		}
		else if (fDamage <= 5 && fDamage >= 2)
		{
			SetDamage(fDamage, 100, D3DXVECTOR3((sinf(-D3DX_PI * fRandumRot) * fRandumSpeed), (cosf(-D3DX_PI * fRandumRot) * fRandumSpeed), 0.0f), g_aEnemy[nCntEnemy].pos, 15.0f);
		}
		else if (fDamage >= 15 && fDamage <= 50)
		{
			SetDamage(fDamage, 100, D3DXVECTOR3((sinf(-D3DX_PI * fRandumRot) * fRandumSpeed), (cosf(-D3DX_PI * fRandumRot) * fRandumSpeed), 0.0f), g_aEnemy[nCntEnemy].pos, 40.0f);
		}
		else if (fDamage >= 70)
		{
			SetDamage(fDamage, 100, D3DXVECTOR3((sinf(-D3DX_PI * fRandumRot) * fRandumSpeed), (cosf(-D3DX_PI * fRandumRot) * fRandumSpeed), 0.0f), g_aEnemy[nCntEnemy].pos, 80.0f);
		}
		//=======================================================================================================================================================================================


		if (g_aEnemy[nCntEnemy].bUse== false)
		{
			if (g_aEnemy[nCntEnemy].bEnemyCountminusFlag == false)
			{
				g_aEnemy[nCntEnemy].bEnemyCountminusFlag = true;//敵の総数カウントダウンしたのでtrue（済）
				if (g_aDefeat.DefeatEnemyCount > 0)
				{
					g_aDefeat.DefeatEnemyCount--;//敵の残り数をマイナスする。
			    }
			}
		}

		if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_MINIBOMUBY)
		{
			int nCntBullet_E8;
			float fSpeedBullet_E8;
			float fCircleBullet_E8_easy[10];
			float fRotBullet_E8;
			fRotBullet_E8 = float((rand)() % 200) / 100.0f;
			PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_001);

			int nCntBullet4;
			for (nCntBullet4 = 0; nCntBullet4 < 10; nCntBullet4++)
			{
				fCircleBullet_E8_easy[nCntBullet4] = (1.0f / (10 / 2)) * nCntBullet4;

				fSpeedBullet_E8 = rand() % 5 + 1;


				SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * fCircleBullet_E8_easy[nCntBullet4]) * fSpeedBullet_E8), (cosf(-D3DX_PI * fCircleBullet_E8_easy[nCntBullet4]) * fSpeedBullet_E8), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);//敵の弾だから敵同士で当たらない	

			}
		}


		if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOMUBY)
		{
			int nCntBullet_E8;
			float fSpeedBullet_E8;
			float fCircleBullet_E8_easy[10];
			float fCircleBullet_E8_normal[30];
			float fCircleBullet_E8_hard[50];
			float fCircleBullet_E8_veryhard[100];
			float fRotBullet_E8;
			fRotBullet_E8 = float((rand)() % 200) / 100.0f;
			

			int nCntBullet4;
			switch (pGameCustom->nDifficulty)
			{
			case 1://Easy
				PlaySound(SOUND_LABEL_SE_EXPLOSION_ENEMY_000);
				for (nCntBullet4 = 0; nCntBullet4 < 10; nCntBullet4++)
				{
					fCircleBullet_E8_easy[nCntBullet4]= (1.0f / (10 / 2)) * nCntBullet4;

					fSpeedBullet_E8 = rand() % 5 + 1;

					
					SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * fCircleBullet_E8_easy[nCntBullet4]) * fSpeedBullet_E8), (cosf(-D3DX_PI * fCircleBullet_E8_easy[nCntBullet4]) * fSpeedBullet_E8), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);//敵の弾だから敵同士で当たらない	

				}
				break;
			case 2://Normal
				PlaySound(SOUND_LABEL_SE_EXPLOSION_ENEMY_000);
				for (nCntBullet4 = 0; nCntBullet4 < 30; nCntBullet4++)
				{
					fCircleBullet_E8_normal[nCntBullet4] = (1.0f / (30 / 2)) * nCntBullet4;

					fSpeedBullet_E8 = rand() % 5 + 1;


					SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * fCircleBullet_E8_normal[nCntBullet4]) * fSpeedBullet_E8), (cosf(-D3DX_PI * fCircleBullet_E8_normal[nCntBullet4]) * fSpeedBullet_E8), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);//敵の弾だから敵同士で当たらない	

				}
				break;
			case 3://Hard
				PlaySound(SOUND_LABEL_SE_EXPLOSION_ENEMY_000);
				for (nCntBullet4 = 0; nCntBullet4 < 50; nCntBullet4++)
				{
					fCircleBullet_E8_hard[nCntBullet4] = (1.0f / (50 / 2)) * nCntBullet4;

					fSpeedBullet_E8 = rand() % 5 + 1;


					SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * fCircleBullet_E8_hard[nCntBullet4]) * fSpeedBullet_E8), (cosf(-D3DX_PI * fCircleBullet_E8_hard[nCntBullet4]) * fSpeedBullet_E8), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);//敵の弾だから敵同士で当たらない	

				}
				break;
			case 4://Hard
				PlaySound(SOUND_LABEL_SE_EXPLOSION_ENEMY_000);
				for (nCntBullet4 = 0; nCntBullet4 < 100; nCntBullet4++)
				{
					fCircleBullet_E8_veryhard[nCntBullet4] = (1.0f / (100 / 2)) * nCntBullet4;

					fSpeedBullet_E8 = rand() % 6+ 1;


					SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * fCircleBullet_E8_veryhard[nCntBullet4]) * fSpeedBullet_E8), (cosf(-D3DX_PI * fCircleBullet_E8_veryhard[nCntBullet4]) * fSpeedBullet_E8), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);//敵の弾だから敵同士で当たらない	

				}
				break;
			}
			
		}
			g_nNumEnemy--;//敵一人につき一回だけ敵の総数カウントダウン
		
		


		/*if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_DELBY)
		{
			AddScore(2000);
		}*/
		if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS1_STARWARRIOR)
		{
			AddScore(2000000);

			for (int nCnt = 0; nCnt < 500; nCnt++)
			{
				SetParticle(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.8f), 100.0f, 1000);
			}
			if (g_bClearFlag == false)
			{

				/*if (pPlayer->nHitCount == 0)
				{
					if (g_bNohitFlag == false)
					{
						g_bNohitFlag = true;
						AddScore(3000000);
					}

				}*/

			
				//モード設定
				pResult->bResultFlag = true;//リザルト画面を呼び出すフラグをtrueにする。
				pResultType->nResultType = RESULTTYPE_CLEAR;//ゲームクリア用リザルトを呼び出す。
				SetGameState(GAMESTATE_END);
				g_bClearFlag = true;
			}
		}
		AddScore(500);
		if (g_nNumEnemy <= 0)
		{//敵の総数が０以下になったら
			//モード設定

		}
	}
	else
	{//敵にダメージを与えたら

	float fRandumRot;
	float fRandumSpeed;
	fRandumRot = float((rand)() % 629 - 314) / 100.0f;
	fRandumSpeed = float((rand)() % 30) / 10.0f;

	//===================================
	//ダメージ表記処理
	//===================================
	if (fDamage <= 1&&fDamage>=0)
	{
		SetDamage(fDamage, 100, D3DXVECTOR3((sinf(-D3DX_PI * fRandumRot) * fRandumSpeed), (cosf(-D3DX_PI * fRandumRot) * fRandumSpeed), 0.0f), g_aEnemy[nCntEnemy].pos, 10.0f);

	}
	else if (fDamage <= 5 && fDamage >= 2)
	{
		SetDamage(fDamage, 100, D3DXVECTOR3((sinf(-D3DX_PI * fRandumRot)* fRandumSpeed), (cosf(-D3DX_PI * fRandumRot)* fRandumSpeed), 0.0f), g_aEnemy[nCntEnemy].pos, 15.0f);
	}
	else if (fDamage >= 15 && fDamage <= 50)
	{
		SetDamage(fDamage, 100, D3DXVECTOR3((sinf(-D3DX_PI * fRandumRot) * fRandumSpeed), (cosf(-D3DX_PI * fRandumRot) * fRandumSpeed), 0.0f), g_aEnemy[nCntEnemy].pos, 40.0f);
	}
	else if (fDamage >= 70)
	{
		SetDamage(fDamage, 100, D3DXVECTOR3((sinf(-D3DX_PI * fRandumRot) * fRandumSpeed), (cosf(-D3DX_PI * fRandumRot) * fRandumSpeed), 0.0f), g_aEnemy[nCntEnemy].pos, 80.0f);
	}
	//=======================================================================================================================================================================================


		for (int nCnt = 0; nCnt < 5; nCnt++)
		{
			SetParticle(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 0.5f, 0.0f, 0.8f), 20.0f, 100);
		}

		if (g_aEnemy[nCntEnemy].nType != ENEMYTYPE_BOSS1_STARWARRIOR)
		{
			PlaySound(SOUND_LABEL_SE_HIT);
		}
		g_bSetParticle[nCntEnemy] = true;

		float fRandumDamageMove = float((rand)() % 629 - 314) / 100.0f;

	/*	SetDamage(fDamage, 30, D3DXVECTOR3((sinf(-D3DX_PI * fRandumDamageMove) * 3.0f), (cosf(-D3DX_PI * fRandumDamageMove) * 3.0f), 0.0f),D3DXVECTOR3(g_aEnemy[nCntEnemy].pos));*/

		g_aEnemy[nCntEnemy].State = ENEMYSTATE_DAMAGE;//ヒット時にダメージ状態にする
		g_aEnemy[nCntEnemy].nCounterState = 5;//ダメージ状態を保つ時間
		//頂点カラーの設定

		/*VERTEX_2D* pVtx;*/
		/*nDamage = 1;*/


		g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (4 * nCntEnemy);
		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);//赤色にする
		pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);//赤色にする
		pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);//赤色にする
		pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);//赤色にする
		AddScore(10);//敵にダメージを与えたらスコアを加算する
		g_pVtxBuffEnemy->Unlock();
	}

	
}
//==========================
//敵の取得
//==========================
Enemy* GetEnemy(void)
{
	return &g_aEnemy[0];//敵の情報の先頭アドレスを返す。
}

//=======================================
//リザルト画面を呼び出すフラグの取得
//=======================================
Result* GetResult(void)
{
	return &g_aResult;
}

//=========================================
//ウェーブ構造体の取得
//=========================================
Defeat* GetDefeat(void)
{
	return &g_aDefeat;
}