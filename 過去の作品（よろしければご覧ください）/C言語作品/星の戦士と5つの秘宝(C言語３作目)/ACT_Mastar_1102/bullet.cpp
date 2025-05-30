//========================================================
//
//８月３０日：エフェクトもう一回見直す[bullet.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#include "main.h"
#include "bullet.h"
#include "input.h"
#include "explosion.h"
#include "enemy.h"
#include "player.h"
#include "health.h"
#include <time.h>
#include "effect.h"
#include "sound.h"
#include "particle.h"
#include "health.h"
#include "block.h"
#include "Attack.h"
#include "health.h"
#include "staging.h"

#define NUM_BULLET (23)//弾の種類数
#define BIGBULLET_LV1 (40.0f)
#define BIGBULLET_LV2 (150.0f)//大きい弾LV24
#define BULLETSCALE_NOVA (100.0f)
#define FINALBALLSIZE (150.0f)
#define BULLETSIZE_SCATTERING (30.0f)


//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureBullet[NUM_BULLET] = {};//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;//弾の情報
Bullet g_aBullet[MAX_BULLET];
float g_fXMove;//X座標の移動
float g_fYMove;//Y座標の移動

float g_fCurveBullet;
float g_fEnemy9Bullet;

float g_fCircleBullet_B[MAX_CIRCLEBULLET];
int g_nRandumBullet2_B;
float g_fRandumResultBullet2_B;//ランダムで弾を撃つ方向が決まる。

int g_nDelayBullet;

int g_nNumEnemy[MAX_ENEMY];//エネミーの数を数える変数
D3DXVECTOR3 g_posBullet;//位置
D3DXVECTOR3 g_moveBullet;//移動量
D3DXVECTOR3 g_rotPl;//向き

float g_fRandumScatteringBullet_Rot;//ばらまき拡散弾の処理
float g_fRandumScatternigBullet_Speed;//ばらまき拡散弾のスピード

int g_nHitCount;//プレイヤーが被弾した回数をカウントする変数

//int g_nAnimatioinCounter_Slash[MAX_BULLET];//アニメーションを次のテクスチャにするカウンター
//float g_fAnimationPattern_Slash[MAX_BULLET];//アニメーションのテクスチャを管理する変数

//弾の初期化処理
void InitBullet(void)
{

	//srand((unsigned int)time(NULL));//ゲームを開始した時間毎に乱数の種を設定

	g_moveBullet = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//移動量を初期化する

	g_fCurveBullet = 0.0f;
	g_fEnemy9Bullet= 0.0f;

	g_nDelayBullet = 0;

	g_fRandumScatteringBullet_Rot = 0.0f;
	g_fRandumScatternigBullet_Speed = 0.0f;

	g_nHitCount=0;

	//g_nDelayBullet;

	LPDIRECT3DDEVICE9 pDevice;
	int	nCntBullet;


	int nCntEnemy;
	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み（弾のテクスチャを読み込む）
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\yellowgreenball_char.png",
		&g_pTextureBullet[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\redball_enemy.png",
		&g_pTextureBullet[1]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\brueball_char.png",
		&g_pTextureBullet[2]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Skill_Slash.png",
		&g_pTextureBullet[3]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\lightbrueball_enemy.png",
		&g_pTextureBullet[4]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\orangeball_enemy.png",
		&g_pTextureBullet[5]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\arrow_bullet_000.png",
		&g_pTextureBullet[6]);

		D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\brueball_char.png",
		&g_pTextureBullet[7]);

		D3DXCreateTextureFromFile(pDevice,
			"data\\TEXTURE\\purpleball_enemy.png",
			&g_pTextureBullet[8]);

		D3DXCreateTextureFromFile(pDevice,
			"data\\TEXTURE\\yellowgreenball_enemy.png",
			&g_pTextureBullet[9]);

		D3DXCreateTextureFromFile(pDevice,
			"data\\TEXTURE\\wind_bullet_000.png",
			&g_pTextureBullet[10]);

		D3DXCreateTextureFromFile(pDevice,
			"data\\TEXTURE\\Reflection_Bullet.png",
			&g_pTextureBullet[11]);

		D3DXCreateTextureFromFile(pDevice,
			"data\\TEXTURE\\NovaBall.png",
			&g_pTextureBullet[12]);

		D3DXCreateTextureFromFile(pDevice,
			"data\\TEXTURE\\Rockon_ball_000.png",
			&g_pTextureBullet[13]);

		D3DXCreateTextureFromFile(pDevice,
			"data\\TEXTURE\\BulletSeed.png",
			&g_pTextureBullet[14]);

		D3DXCreateTextureFromFile(pDevice,
			"data\\TEXTURE\\SnowBall.png",
			&g_pTextureBullet[15]);

		D3DXCreateTextureFromFile(pDevice,
			"data\\TEXTURE\\IcicleBullet.png",
			&g_pTextureBullet[16]);

		D3DXCreateTextureFromFile(pDevice,
			"data\\TEXTURE\\SpletBullet.png",
			&g_pTextureBullet[17]);

		D3DXCreateTextureFromFile(pDevice,
			"data\\TEXTURE\\JumpBullet.png",
			&g_pTextureBullet[18]);

		D3DXCreateTextureFromFile(pDevice,
			"data\\TEXTURE\\ExplosionBullet.png",
			&g_pTextureBullet[19]);

		D3DXCreateTextureFromFile(pDevice,
			"data\\TEXTURE\\FinalBall.png",
			&g_pTextureBullet[20]);
	//g_posBullet = D3DXVECTOR3(600.0f, 300.0f, 0.0f);//位置を初期化する


	for (nCntBullet = 0; nCntBullet < MAX_CIRCLEBULLET; nCntBullet++)
	{//円形各散弾の初期化
		g_fCircleBullet_B[nCntBullet] = 0;
	}

	//弾情報の初期化
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].nLife = 0;
		g_aBullet[nCntBullet].bUse = false;//使用していない状態にする
		g_aBullet[nCntBullet].type = BULLETTYPE_PLAYER;
		g_aBullet[nCntBullet].fCurveBulletX = 0;//それぞれの弾幕にカーブ量の数値を持たせる。
		g_aBullet[nCntBullet].fCurveBulletY = 0;//それぞれの弾幕にカーブ量の数値を持たせる。
		g_aBullet[nCntBullet].nAnimationCounter_Slash = 0;
		g_aBullet[nCntBullet].fAnimationPattern_Slash = 0.0f;
		g_aBullet[nCntBullet].bCurveFlag = false;
		g_aBullet[nCntBullet].fEnemy5Curve = 0.0f;//エネミー５の弾をカーブさせる変数
		g_aBullet[nCntBullet].nApperCount = 0;//エネミー５の弾をカーブさせる変数
		g_aBullet[nCntBullet].nNumber = 0;//エネミー５の順番
		g_aBullet[nCntBullet].nNumEnemy = -1;//どの敵が発射しているかを判別する変数
		g_aBullet[nCntBullet].bBulletReflectionX = false;//X方向に弾を反射させるフラグ
		g_aBullet[nCntBullet].bBulletReflectionY = false;//Y方向に弾を反射させるフラグ
		g_aBullet[nCntBullet].nHormingaim = 0;//ホーミングで狙っている敵を判別する変数
		g_aBullet[nCntBullet].HormingFlag = false;
		g_aBullet[nCntBullet].nHormingCount=0;
		g_aBullet[nCntBullet].fSpeed = 0.0f;//バレットのスピードを変える。
		g_aBullet[nCntBullet].fCurve = 0.0f;//バレットのカーブ値を変える。
		g_aBullet[nCntBullet].bCollisionBullet = false;//弾がブロックに衝突するかどうかのフラグ
		g_aBullet[nCntBullet].moveType = MOVETYPE_NORMAL;//弾の動き方を決める
		g_aBullet[nCntBullet].nAnimationCounter = 0;
		g_aBullet[nCntBullet].nAnimationPattern = 0;
		g_aBullet[nCntBullet].nCntTime = 0;
     }

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		g_nNumEnemy[nCntEnemy] = -1;//初期化
	}
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BULLET,//４は必要な頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL);

	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//頂点座標の設定(弾の大きさ）
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

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
	g_pVtxBuffBullet->Unlock();
}

//弾の終了処理
void UninitBullet(void)
{
	//テクスチャの破棄
	int nCnt;
	for (nCnt = 0; nCnt < NUM_BULLET; nCnt++)
	{
		if (g_pTextureBullet[nCnt] != NULL)
		{
			g_pTextureBullet[nCnt]->Release();
			g_pTextureBullet[nCnt] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//弾の更新処理
void UpdateBullet(void)
{
	int nCntBullet3;

	g_nRandumBullet2_B = rand() % 100 + 1;//円形各散弾の発射方向の調整用

	g_fRandumResultBullet2_B = 0.01f * g_nRandumBullet2_B;//円形拡散弾の方向調整結果
	//g_fRandumResultBullet2_B = 0.01f;//円形拡散弾の方向調整結果


	g_fRandumScatteringBullet_Rot= float((rand)() % 629 - 314) / 100.0f;//弾を発射する方向を決める

    

	g_nDelayBullet++;

	Player* pPlayer = GetPlayer();
	//g_aBullet.fCurveBulletX += 0.01f;
	//g_fRotBulletY += 0.02f;
	//
	//LPDIRECT3DDEVICE9 pDevice;
	int	nCntBullet;
	//g_fEnemy9Bullet += 0.0001f;

	//Health* pHealth = GetHealth();

	Enemy* pEnemy = GetEnemy();

	VERTEX_2D* pVtx;

	
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{//弾が使用されている
			int nCntEnemy;
			/*SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 0.5f, 0.5f), 3.0f, 50);*/


			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			//========================================================================
			//スキル２のテクスチャアニメーション
			//========================================================================
			if (g_aBullet[nCntBullet].type == BULLETTYPE_SKILL2_SLASH)
			{
				g_aBullet[nCntBullet].nAnimationCounter_Slash++;//テクスチャアニメーションの処理

				if (g_aBullet[nCntBullet].nAnimationCounter_Slash % 3 == 0)
				{
					g_aBullet[nCntBullet].fAnimationPattern_Slash += 0.2f;//テクスチャアニメーションを進める。
				}
				if (g_aBullet[nCntBullet].fAnimationPattern_Slash >= 5)
				{
					g_aBullet[nCntBullet].fAnimationPattern_Slash = 0.0f;//テクスチャのアニメーションを一つ目に戻す。
				}
				/*if (g_aBullet[nCntBullet].bUse == false)
				{
					g_aBullet[nCntBullet].fAnimationPattern_Slash = 0.0f;
				}*/
				//スキル２テクスチャ座標（アニメーション）の設定
				pVtx[0].tex = D3DXVECTOR2(0.0f + g_aBullet[nCntBullet].fAnimationPattern_Slash, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.2f + g_aBullet[nCntBullet].fAnimationPattern_Slash, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f + g_aBullet[nCntBullet].fAnimationPattern_Slash, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.2f + g_aBullet[nCntBullet].fAnimationPattern_Slash, 1.0f);

				/*if (g_aBullet[nCntBullet].bUse == false)
				{
					pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
					pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
					pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
					pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);
				}*/
			}
			//=============================================================================================================

			g_aBullet[nCntBullet].fCurveBulletX += 0.017f;//それぞれの弾幕にカーブ量の数値を持たせる。
			g_aBullet[nCntBullet].fCurveBulletY -= 0.00f;//それぞれの弾幕にカーブ量の数値を持たせる。

			g_aBullet[nCntBullet].nLife -= 1;//表示する時間（寿命）を減らす。




			//=======================================================
			//弾の大きさ、爆発、アニメーションなどの処理
			//=======================================================

				//特に変更を加えない弾の大きさ
			pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - 10.0f, g_aBullet[nCntBullet].pos.y - 10.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + 10.0f, g_aBullet[nCntBullet].pos.y - 10.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - 10.0f, g_aBullet[nCntBullet].pos.y + 10.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + 10.0f, g_aBullet[nCntBullet].pos.y + 10.0f, 0.0f);

			if (g_aBullet[nCntBullet].type == BULLETTYPE_SKILL1)
			{//スキル１の弾の大きさを設定
				pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - BIGBULLET_LV1, g_aBullet[nCntBullet].pos.y - BIGBULLET_LV1, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + BIGBULLET_LV1, g_aBullet[nCntBullet].pos.y - BIGBULLET_LV1, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - BIGBULLET_LV1, g_aBullet[nCntBullet].pos.y + BIGBULLET_LV1, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + BIGBULLET_LV1, g_aBullet[nCntBullet].pos.y + BIGBULLET_LV1, 0.0f);
			}

			if (g_aBullet[nCntBullet].type == BULLETTYPE_SKILL2_SLASH)
			{//スキル２の弾の大きさを設定


				pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - BIGBULLET_LV2, g_aBullet[nCntBullet].pos.y - (BIGBULLET_LV2 - 50.0f), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + BIGBULLET_LV2, g_aBullet[nCntBullet].pos.y - (BIGBULLET_LV2 - 50.0f), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - BIGBULLET_LV2, g_aBullet[nCntBullet].pos.y + (BIGBULLET_LV2 - 50.0f), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + BIGBULLET_LV2, g_aBullet[nCntBullet].pos.y + (BIGBULLET_LV2 - 50.0f), 0.0f);


			}
			if (g_aBullet[nCntBullet].type == BULLETTYPE_SCATTERING)
			{//スキル２の弾の大きさを設定


				pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - BULLETSIZE_SCATTERING, g_aBullet[nCntBullet].pos.y - (BULLETSIZE_SCATTERING), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + BULLETSIZE_SCATTERING, g_aBullet[nCntBullet].pos.y - (BULLETSIZE_SCATTERING), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - BULLETSIZE_SCATTERING, g_aBullet[nCntBullet].pos.y + (BULLETSIZE_SCATTERING), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + BULLETSIZE_SCATTERING, g_aBullet[nCntBullet].pos.y + (BULLETSIZE_SCATTERING), 0.0f);


			}
			if (g_aBullet[nCntBullet].type == BULLETTYPE_NOVA)
			{//スキル２の弾の大きさを設定


				pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - BULLETSCALE_NOVA, g_aBullet[nCntBullet].pos.y - (BULLETSCALE_NOVA), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + BULLETSCALE_NOVA, g_aBullet[nCntBullet].pos.y - (BULLETSCALE_NOVA), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - BULLETSCALE_NOVA, g_aBullet[nCntBullet].pos.y + (BULLETSCALE_NOVA), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + BULLETSCALE_NOVA, g_aBullet[nCntBullet].pos.y + (BULLETSCALE_NOVA), 0.0f);


			}
			//=============================================================================================================================================

			//==============================================
			//特定の条件下で弾を消す処理
			//==============================================
			if ((g_aBullet[nCntBullet].pos.x >= SCREEN_WIDTH) || (g_aBullet[nCntBullet].pos.x <= 0) ||
				(g_aBullet[nCntBullet].pos.y >= SCREEN_HEIGHT) || (g_aBullet[nCntBullet].pos.y <= 0))//角にぶつかった
			{
				/*if (g_aBullet[nCntBullet].type == BULLETTYPE_PLAYER ||
					g_aBullet[nCntBullet].type == BULLETTYPE_ENEMY ||
					g_aBullet[nCntBullet].type == BULLETTYPE_SKILL1 ||
					g_aBullet[nCntBullet].type == BULLETTYPE_ROT ||
					g_aBullet[nCntBullet].type == BULLETTYPE_REAFY ||
					g_aBullet[nCntBullet].type == BULLETTYPE_ENEMY9 ||
					g_aBullet[nCntBullet].type == ENEMYTYPE_BOLBY||
					g_aBullet[nCntBullet].type==BULLETTYPE_BANDY)*/
				if (g_aBullet[nCntBullet].type != BULLETTYPE_ARROW)
				{
					if (g_aBullet[nCntBullet].type != BULLETTYPE_REFLECTION)
					{
						if (g_aBullet[nCntBullet].type != BULLETTYPE_SATELLITE)
						{
							if (g_aBullet[nCntBullet].type != BULLETTYPE_SEED || g_aBullet[nCntBullet] .type != BULLETTYPE_JUMPING)
							{
								SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), EXPLOSIONTYPE_NORMAL, 10.0f, 10.0f);

								if (g_aBullet[nCntBullet].type == BULLETTYPE_NOVA)
								{
									SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), EXPLOSIONTYPE_NOVA, 400.0f, 400.0f);
								}
								g_aBullet[nCntBullet].bUse = false;//使用していない状態にする。
							}
						}
					}
				}


			}

			Block* pBlock = GetBlock();
			
			if (g_aBullet[nCntBullet].bCollisionBullet == true)
			{//ブロックに当たるかどうかのフラグがtrueの時
				for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++, pBlock++)
				{
					if (pBlock->bUse == true)
					{
						if (g_aBullet[nCntBullet].pos.x >= pBlock->pos.x - pBlock->fRadius && g_aBullet[nCntBullet].pos.x <= pBlock->pos.x + pBlock->fRadius &&
							g_aBullet[nCntBullet].pos.y >= pBlock->pos.y - pBlock->fRadius && g_aBullet[nCntBullet].pos.y <= pBlock->pos.y + pBlock->fRadius)
						{//プレイヤーに弾が当たった

							//PlaySound(SOUND_LABEL_SE_DAMAGE_000);

							SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), EXPLOSIONTYPE_NORMAL, 10.0f, 10.0f);
							//敵の弾を使用していない状態にする
							g_aBullet[nCntBullet].bUse = false;//弾を使用していない状態にする

						}
					}
				}
			}

			
			//===================================================================================================================================================


			//=============================================================
			//当たり判定、ヒット時の威力、カーブなどの処理
			//===============================================================
			if (g_aBullet[nCntBullet].type == BULLETTYPE_PLAYER)
			{//プレイヤーの弾
				
				Enemy* pEnemy = GetEnemy();
				for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				{
					if (pEnemy->bUse == true)
					{//敵が使用されている
						
					    if (g_aBullet[nCntBullet].pos.x >= pEnemy->pos.x - pEnemy->fWidth /2 && g_aBullet[nCntBullet].pos.x <= pEnemy->pos.x + pEnemy->fWidth / 2 &&
							g_aBullet[nCntBullet].pos.y >= pEnemy->pos.y - pEnemy->fHeight && g_aBullet[nCntBullet].pos.y <= pEnemy->pos.y)
						{//敵と弾が当たった
							HitEnemy(nCntEnemy,5);

							SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), EXPLOSIONTYPE_NORMAL,10.0f,10.0f);
							//pEnemy->bUse = false;//敵を使用していない状態にする
							g_aBullet[nCntBullet].bUse = false;//弾を使用していない状態にする

						}
					}

				}
			}
			if (g_aBullet[nCntBullet].type == BULLETTYPE_ROT)
			{//プレイヤーの弾

				Enemy* pEnemy = GetEnemy();
				for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				{
					if (pEnemy->bUse == true)
					{//敵が使用されている
						if (g_aBullet[nCntBullet].pos.x >= pEnemy->pos.x - pEnemy->fWidth / 2 && g_aBullet[nCntBullet].pos.x <= pEnemy->pos.x + pEnemy->fWidth / 2 &&
							g_aBullet[nCntBullet].pos.y >= pEnemy->pos.y - pEnemy->fHeight && g_aBullet[nCntBullet].pos.y <= pEnemy->pos.y)
						{//敵と弾が当たった
							HitEnemy(nCntEnemy, 20.0f);

							SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), EXPLOSIONTYPE_NORMAL,10.0f,10.0f);
							//pEnemy->bUse = false;//敵を使用していない状態にする
							g_aBullet[nCntBullet].bUse = false;//弾を使用していない状態にする

						}
					}
				}
			}
		
			else if (g_aBullet[nCntBullet].type == BULLETTYPE_ENEMY)
			{//敵の弾(敵全体に適用）

				g_aBullet[nCntBullet].nCntTime++;

				if (g_aBullet[nCntBullet].nCntTime == 1)
				{
					PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_001);
				}
				Player* pPlayer = GetPlayer();
				/*for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pPlayer++)
				{*/

				SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 10.0f, 30, EFFECTSCALE_NORMAL);
				if (g_aBullet[nCntBullet].pos.x >= pPlayer->pos.x - pPlayer->fWidth&& g_aBullet[nCntBullet].pos.x <= pPlayer->pos.x + pPlayer->fWidth &&
					g_aBullet[nCntBullet].pos.y >= pPlayer->pos.y - pPlayer->fHeight && g_aBullet[nCntBullet].pos.y <= pPlayer->pos.y)
				{//プレイヤーに弾が当たった

					//PlaySound(SOUND_LABEL_SE_DAMAGE_000);

				SetExplosion(pPlayer->pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),EXPLOSIONTYPE_NORMAL,10.0f,10.0f);
					//敵の弾を使用していない状態にする
					g_aBullet[nCntBullet].bUse = false;//弾を使用していない状態にする
					DamageHealth(5);
				}
				/*}*/
			}
			if (g_aBullet[nCntBullet].type == BULLETTYPE_REAFY)
			{//エネミー５の弾
				Player* pPlayer = GetPlayer();
				/*	g_aBullet[nCntBullet].move += D3DXVECTOR3((sinf(-D3DX_PI *g_aBullet[nCntBullet].fCurveBulletX) * 0.1f), (cosf(-D3DX_PI *
						g_aBullet[nCntBullet].fCurveBulletX) * 0.1f), 0.0f);*/

				SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 0.0f, 1.0f, 0.5f), 15.0f, 300, EFFECTSCALE_NORMAL);

				g_aBullet[nCntBullet].fCurveBulletX += 0.001f;/*
				g_aBullet[nCntBullet].fCurveBulletY += 0.02f;*/
				/*	g_fCurveBullet += 0.1f;*/

					//if (g_aBullet[nCntBullet].fCurveBulletX >= 0.3f)
					//{
					//	g_aBullet[nCntBullet].bCurveFlag == true;
					//}
					//if (g_aBullet[nCntBullet].bCurveFlag == true)
					//{
					//	g_aBullet[nCntBullet].fCurveBulletX -= 0.01f;
					//	g_aBullet[nCntBullet].move.x += (cosf(D3DX_PI * g_aBullet[nCntBullet].fCurveBulletX) * 0.1f);
					//}
					//if (g_aBullet[nCntBullet].fCurveBulletX <= -0.3f)
					//{
					//	g_aBullet[nCntBullet].bCurveFlag == false;
					//}
					//if (g_aBullet[nCntBullet].bCurveFlag == false)
					//{
					//	g_aBullet[nCntBullet].fCurveBulletX += 0.01f;//カーブ用処理
					//	g_aBullet[nCntBullet].move.x += (sinf(D3DX_PI * g_aBullet[nCntBullet].fCurveBulletX) * 0.1f);
					//}


				/*g_aBullet[nCntBullet].move.x += (sinf(D3DX_PI * g_aBullet[nCntBullet].fCurveBulletX) * 0.1f);*/

				/*for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pPlayer++)
				{*/

				/*	g_aBullet[nCntBullet].move.x += (sinf(-D3DX_PI * g_fCurveBullet) * 5.0f);///面白すぎたので後でほかの敵に実装する！！！！！！！！！！！！！！
						g_aBullet[nCntBullet].move.y += (sinf(-D3DX_PI * 0.0f) * 0.002f);*/
						/*g_aBullet[nCntBullet].move.x = 0.0f;*/
						//int nCntBullet3;
						//for (nCntBullet3 = 0; nCntBullet3 < MAX_CIRCLEBULLET; nCntBullet3++)
						//{
						//	g_fCircleBullet_B[nCntBullet3] = (1.0f / 5.0f) * nCntBullet3;

					/*	for (nCntBullet3 = 0; nCntBullet3 < MAX_CIRCLEBULLET_2; nCntBullet3++)
						{*/
						/*	g_nDelayBullet[nCntBullet3]++;*/
							//	g_fCircleBullet[nCntBullet3] = (1.0f / 5.0f) * nCntBullet3;
				g_aBullet[nCntBullet].nApperCount++;

				//if (g_aBullet[nCntBullet].nApperCount == 1)
				//{
				//	//g_aBullet[nCntBullet].fEnemy5Curve = (D3DX_PI / 5.0f) * (nCntBullet + g_fRandumResultBullet2_B);
				//	g_aBullet[nCntBullet].fEnemy5Curve = (D3DX_PI / 5.0f) * g_aBullet[nCntBullet].nNumber + g_fRandumResultBullet2_B;
				//}


				//パターン１======================================
				/*if (g_nDelayBullet % 5== 0)
				{
					g_aBullet[nCntBullet].fEnemy5Curve += ((D3DX_PI / 15000.0f) * g_aBullet[nCntBullet].nApperCount);
				}*/
				//======================================================================================================
				//パターン２===========================================================

				if (g_aBullet[nCntBullet].nApperCount == 1)
				{
					g_aBullet[nCntBullet].fEnemy5Curve = (D3DX_PI / 8.0f) * g_aBullet[nCntBullet].nNumber + g_fRandumResultBullet2_B;
				}

				if (g_nDelayBullet % 5 == 0)
				{
					g_aBullet[nCntBullet].fEnemy5Curve += ((D3DX_PI / 50.0f) * 0.5f);
				}
				//===========================================================================


				g_aBullet[nCntBullet].move.x = (sinf(g_aBullet[nCntBullet].fEnemy5Curve) * 5.5f);
				g_aBullet[nCntBullet].move.y = (cosf(g_aBullet[nCntBullet].fEnemy5Curve) * 5.5f);



				/*	g_aBullet[nCntBullet].move.x += (sinf(g_fCircleBullet_B[nCntBullet3] + g_fRandumResultBullet2_B) * 0.1f);
					g_aBullet[nCntBullet].move.y += (cosf(g_fCircleBullet_B[nCntBullet3] + g_fRandumResultBullet2_B) * 0.1f);*/

				if (g_aBullet[nCntBullet].pos.x >= pPlayer->pos.x - pPlayer->fWidth && g_aBullet[nCntBullet].pos.x <= pPlayer->pos.x + pPlayer->fWidth &&
					g_aBullet[nCntBullet].pos.y >= pPlayer->pos.y - pPlayer->fHeight && g_aBullet[nCntBullet].pos.y <= pPlayer->pos.y)
				{//プレイヤーに弾が当たった
					SetExplosion(pPlayer->pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), EXPLOSIONTYPE_NORMAL, 10.0f, 10.0f);
					//敵の弾を使用していない状態にする
					g_aBullet[nCntBullet].bUse = false;//弾を使用していない状態にする
					//UpdateHealth(1.0f);//追加予定（仮定）
					DamageHealth(10);
					//pHealth -= 1;//なぜか２ダメージ入ってしまうので、１回復する。
				}
				/*}*/
			}

			if (g_aBullet[nCntBullet].type == BULLETTYPE_SATELLITE)
			{//エネミー５の弾
				Player* pPlayer = GetPlayer();
				SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 0.0f, 1.0f, 0.5f), 15.0f, 300, EFFECTSCALE_NORMAL);

				g_aBullet[nCntBullet].fCurveBulletX += 0.001f;
				g_aBullet[nCntBullet].nApperCount++;
				
				g_aBullet[nCntBullet].fSpeed = (g_aBullet[nCntBullet].nApperCount * 0.1f);
				g_aBullet[nCntBullet].fCurve -= 1.7f;
		
					if (g_aBullet[nCntBullet].nApperCount == 1)
					{
						g_aBullet[nCntBullet].fEnemy5Curve = (D3DX_PI / 8.0f) * g_aBullet[nCntBullet].nNumber + g_fRandumResultBullet2_B;
					}
					




				//パターン２===========================================================
				
					g_aBullet[nCntBullet].fEnemy5Curve += ((D3DX_PI / g_aBullet[nCntBullet].fCurve));
				
				//===========================================================================


				g_aBullet[nCntBullet].move.x = (sinf(g_aBullet[nCntBullet].fEnemy5Curve) * g_aBullet[nCntBullet].fSpeed);
				g_aBullet[nCntBullet].move.y = (cosf(g_aBullet[nCntBullet].fEnemy5Curve) * g_aBullet[nCntBullet].fSpeed);



				

				if (g_aBullet[nCntBullet].pos.x >= pPlayer->pos.x - pPlayer->fWidth && g_aBullet[nCntBullet].pos.x <= pPlayer->pos.x + pPlayer->fWidth &&
					g_aBullet[nCntBullet].pos.y >= pPlayer->pos.y - pPlayer->fHeight && g_aBullet[nCntBullet].pos.y <= pPlayer->pos.y)
				{//プレイヤーに弾が当たった
					SetExplosion(pPlayer->pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),EXPLOSIONTYPE_NORMAL,10.0f,10.0f);
					//敵の弾を使用していない状態にする
					g_aBullet[nCntBullet].bUse = false;//弾を使用していない状態にする
					DamageHealth(15);//プレイヤーに15ダメージを与える。
					
				}
				/*}*/
			}
			if (g_aBullet[nCntBullet].type == BULLETTYPE_ENEMY9)
			{

				SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 0.5f, 0.5f), 15.0f, 300,EFFECTSCALE_NORMAL);

				g_aBullet[nCntBullet].fBulletMoveX_Enemy9 += 0.01f;
				g_aBullet[nCntBullet].fBulletMoveY_Enemy9 += 0.015f;

				///*	g_aBullet[nCntBullet].move.x = 0.0f;*/
				//	g_aBullet[nCntBullet].move.x += (sinf(-D3DX_PI * g_aBullet[nCntBullet].fBulletMoveX_Enemy9) * 0.1f);
				//	g_aBullet[nCntBullet].move.y += (cosf(-D3DX_PI * g_aBullet[nCntBullet].fCurveBulletX) * 0.1f);

				g_aBullet[nCntBullet].move.x += (sinf(-D3DX_PI * 0.0f) * 0.5f);
				g_aBullet[nCntBullet].move.y += (cosf(-D3DX_PI * g_aBullet[nCntBullet].fBulletMoveY_Enemy9) * 0.5f);
				
				

				if (g_aBullet[nCntBullet].pos.x >= pPlayer->pos.x - 5.0f && g_aBullet[nCntBullet].pos.x <= pPlayer->pos.x + 5.0f &&
					g_aBullet[nCntBullet].pos.y >= pPlayer->pos.y - 5.0f && g_aBullet[nCntBullet].pos.y <= pPlayer->pos.y + 5.0f)
				{//プレイヤーに弾が当たった
					SetExplosion(pPlayer->pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),EXPLOSIONTYPE_NORMAL,10.0f,10.0f);
					//敵の弾を使用していない状態にする
					g_aBullet[nCntBullet].bUse = false;//弾を使用していない状態にする
					//UpdateHealth(1.0f);//追加予定（仮定）
					HitPlayer(1);//プレイヤーに1ダメージを与える。
					//pHealth -= 1;//なぜか２ダメージ入ってしまうので、１回復する。
				}
			}
			if (g_aBullet[nCntBullet].type == BULLETTYPE_BOLBY)
			{//敵の弾(敵全体に適用）
				
				SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f),15.0f,300,EFFECTSCALE_NORMAL);
				Player* pPlayer = GetPlayer();
				Enemy* pEnemy = GetEnemy();
				
					g_aBullet[nCntBullet].fXVaim = pPlayer->pos.x - pEnemy[g_aBullet[nCntBullet].nNumEnemy].pos.x;//X座標へのベクトル
					g_aBullet[nCntBullet].fYVaim = pPlayer->pos.y - pEnemy[g_aBullet[nCntBullet].nNumEnemy].pos.y;//Y座標へのベクトル

					g_aBullet[nCntBullet].fShotVectorX = (g_aBullet[nCntBullet].fXVaim - g_aBullet[nCntBullet].fVaim);
					g_aBullet[nCntBullet].fShotVectorY = (g_aBullet[nCntBullet].fYVaim - g_aBullet[nCntBullet].fVaim);
				


				/*g_aBullet[nCntBullet].move.x += ((D3DX_PI * g_aBullet[nCntBullet].fShotVectorX) * 0.002f);
				g_aBullet[nCntBullet].move.y+= ((D3DX_PI * g_aBullet[nCntBullet].fShotVectorY) * 0.002f);*/

				g_aBullet[nCntBullet].move.x += ((D3DX_PI *g_aBullet[nCntBullet].fShotVectorX*0.00001f));
				g_aBullet[nCntBullet].move.y += ((D3DX_PI * g_aBullet[nCntBullet].fShotVectorY*0.00001f));
				//SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((D3DX_PI* g_fXVaim[nCntEnemy]) * 0.002f, (D3DX_PI* g_fYVaim[nCntEnemy]) * 0.002f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0);//敵の弾だから敵同士で当たらない

				if (g_aBullet[nCntBullet].pos.x >= pPlayer->pos.x - 5.0f && g_aBullet[nCntBullet].pos.x <= pPlayer->pos.x + 5.0f &&
					g_aBullet[nCntBullet].pos.y >= pPlayer->pos.y - 5.0f && g_aBullet[nCntBullet].pos.y <= pPlayer->pos.y + 5.0f)
				{//プレイヤーに弾が当たった
					SetExplosion(pPlayer->pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),EXPLOSIONTYPE_NORMAL,10.0f,10.0f);
					//敵の弾を使用していない状態にする
					g_aBullet[nCntBullet].bUse = false;//弾を使用していない状態にする
					//UpdateHealth(1.0f);//追加予定（仮定）
					HitPlayer(1);
				}
				/*}*/
			}
			if (g_aBullet[nCntBullet].type == BULLETTYPE_BANDY)
			{//敵の弾(敵全体に適用）

				SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.5f), 15.0f, 300,EFFECTSCALE_NORMAL);
				
				if (g_aBullet[nCntBullet].pos.x >= pPlayer->pos.x - pPlayer->fWidth && g_aBullet[nCntBullet].pos.x <= pPlayer->pos.x + pPlayer->fWidth &&
					g_aBullet[nCntBullet].pos.y >= pPlayer->pos.y - pPlayer->fHeight && g_aBullet[nCntBullet].pos.y <= pPlayer->pos.y)
				{//プレイヤーに弾が当たった
					SetExplosion(pPlayer->pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),EXPLOSIONTYPE_NORMAL,10.0f,10.0f);
					//敵の弾を使用していない状態にする
					g_aBullet[nCntBullet].bUse = false;//弾を使用していない状態にする
					//UpdateHealth(1.0f);//追加予定（仮定）
					DamageHealth(5);
					
				}
			}
			if (g_aBullet[nCntBullet].type == BULLETTYPE_ROCKON)
			{//更に自機狙い弾を発射

				g_aBullet[nCntBullet].nCntTime++;

				if (g_aBullet[nCntBullet].nCntTime == 1)
				{
					PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_005);
				}

				SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 0.5f, 0.0f, 0.5f), 15.0f, 300, EFFECTSCALE_NORMAL);

				g_aBullet[nCntBullet].fXVaim= pPlayer->pos.x - g_aBullet[nCntBullet].pos.x;//X座標へのベクトル
				g_aBullet[nCntBullet].fYVaim= pPlayer->pos.y - g_aBullet[nCntBullet].pos.y;//Y座標へのベクトル


				g_aBullet[nCntBullet].fVaim= atan2(g_aBullet[nCntBullet].fXVaim, g_aBullet[nCntBullet].fYVaim);

				if (g_nDelayBullet % 3 == 0)
				{
					/*PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_007);*/
					SetBullet(g_aBullet[nCntBullet].pos, D3DXVECTOR3(sinf(g_aBullet[nCntBullet].fVaim) * 15.0f, cosf(g_aBullet[nCntBullet].fVaim) * 15.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1,false,MOVETYPE_NORMAL);
					SetBullet(g_aBullet[nCntBullet].pos, D3DXVECTOR3(sinf(g_aBullet[nCntBullet].fVaim + 0.25f) * 15.0f, cosf(g_aBullet[nCntBullet].fVaim + 0.25f) * 15.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1,false, MOVETYPE_NORMAL);
					SetBullet(g_aBullet[nCntBullet].pos, D3DXVECTOR3(sinf(g_aBullet[nCntBullet].fVaim - 0.25f) * 15.0f, cosf(g_aBullet[nCntBullet].fVaim - 0.25f) * 15.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1,false, MOVETYPE_NORMAL);
				/*	SetBullet(g_aBullet[nCntBullet].pos, D3DXVECTOR3(sinf(g_aBullet[nCntBullet].fVaim + 0.3f) * 15.0f, cosf(g_aBullet[nCntBullet].fVaim + 0.3f) * 15.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1, false, MOVETYPE_NORMAL);
					SetBullet(g_aBullet[nCntBullet].pos, D3DXVECTOR3(sinf(g_aBullet[nCntBullet].fVaim - 0.3f) * 15.0f, cosf(g_aBullet[nCntBullet].fVaim - 0.3f) * 15.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1, false, MOVETYPE_NORMAL);
					SetBullet(g_aBullet[nCntBullet].pos, D3DXVECTOR3(sinf(g_aBullet[nCntBullet].fVaim + 0.45f) * 15.0f, cosf(g_aBullet[nCntBullet].fVaim + 0.45f) * 15.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1, false, MOVETYPE_NORMAL);
					SetBullet(g_aBullet[nCntBullet].pos, D3DXVECTOR3(sinf(g_aBullet[nCntBullet].fVaim - 0.45f) * 15.0f, cosf(g_aBullet[nCntBullet].fVaim - 0.45f) * 15.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1, false, MOVETYPE_NORMAL);
					SetBullet(g_aBullet[nCntBullet].pos, D3DXVECTOR3(sinf(g_aBullet[nCntBullet].fVaim + 0.6f) * 15.0f, cosf(g_aBullet[nCntBullet].fVaim + 0.6f) * 15.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1, false, MOVETYPE_NORMAL);
					SetBullet(g_aBullet[nCntBullet].pos, D3DXVECTOR3(sinf(g_aBullet[nCntBullet].fVaim - 0.6f) * 15.0f, cosf(g_aBullet[nCntBullet].fVaim - 0.6f) * 15.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1, false, MOVETYPE_NORMAL);
					SetBullet(g_aBullet[nCntBullet].pos, D3DXVECTOR3(sinf(g_aBullet[nCntBullet].fVaim + 0.75f) * 15.0f, cosf(g_aBullet[nCntBullet].fVaim + 0.75f) * 15.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1, false, MOVETYPE_NORMAL);
					SetBullet(g_aBullet[nCntBullet].pos, D3DXVECTOR3(sinf(g_aBullet[nCntBullet].fVaim - 0.75f) * 15.0f, cosf(g_aBullet[nCntBullet].fVaim - 0.75f) * 15.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1, false, MOVETYPE_NORMAL);
					SetBullet(g_aBullet[nCntBullet].pos, D3DXVECTOR3(sinf(g_aBullet[nCntBullet].fVaim + 0.9f) * 15.0f, cosf(g_aBullet[nCntBullet].fVaim + 0.9f) * 15.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1, false, MOVETYPE_NORMAL);
					SetBullet(g_aBullet[nCntBullet].pos, D3DXVECTOR3(sinf(g_aBullet[nCntBullet].fVaim - 0.9f) * 15.0f, cosf(g_aBullet[nCntBullet].fVaim - 0.9f) * 15.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1, false, MOVETYPE_NORMAL);*/

				}
		
				

				if (g_aBullet[nCntBullet].pos.x >= pPlayer->pos.x - pPlayer->fWidth&& g_aBullet[nCntBullet].pos.x <= pPlayer->pos.x + pPlayer->fWidth &&
					g_aBullet[nCntBullet].pos.y >= pPlayer->pos.y - pPlayer->fHeight && g_aBullet[nCntBullet].pos.y <= pPlayer->pos.y)
				{//プレイヤーに弾が当たった
					SetExplosion(pPlayer->pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),EXPLOSIONTYPE_NORMAL,10.0f,10.0f);
					//敵の弾を使用していない状態にする
					g_aBullet[nCntBullet].bUse = false;//弾を使用していない状態にする
					//UpdateHealth(1.0f);//追加予定（仮定）
					DamageHealth(4);
				}
			}
			if (g_aBullet[nCntBullet].type == BULLETTYPE_SCATTERING)
			{//発射した弾から、さらにばらまく。
				float fRandumScattering_Rot= float((rand)() % 629 - 314) / 100.0f;//弾を発射する方向を決める
				float fRandumScattering_Speed= float((rand)() % 100+1)/ 10.0f;//弾を発射した時のスピードを決める。
				int nCntBullet_Scattering;
				SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(0.9f, 0.5f, 0.5f, 0.5f), 50.0f, 100, EFFECTSCALE_NORMAL);

				
					if (g_nDelayBullet % 20 == 0)
					{
						SetBullet(g_aBullet[nCntBullet].pos, D3DXVECTOR3((sinf(-D3DX_PI * (fRandumScattering_Rot)) * fRandumScattering_Speed), (cosf(-D3DX_PI * (fRandumScattering_Rot)) * fRandumScattering_Speed), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, -1, -1, -1, false, MOVETYPE_NORMAL);
					}
				

				

				if (g_aBullet[nCntBullet].pos.x >= pPlayer->pos.x - 5.0f && g_aBullet[nCntBullet].pos.x <= pPlayer->pos.x + 5.0f &&
					g_aBullet[nCntBullet].pos.y >= pPlayer->pos.y - 5.0f && g_aBullet[nCntBullet].pos.y <= pPlayer->pos.y + 5.0f)
				{//プレイヤーに弾が当たった
					SetExplosion(pPlayer->pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),EXPLOSIONTYPE_NORMAL,10.0f,10.0f);
					//敵の弾を使用していない状態にする
					g_aBullet[nCntBullet].bUse = false;//弾を使用していない状態にする
					//UpdateHealth(1.0f);//追加予定（仮定）
					HitPlayer(1);
				}
			}
			if (g_aBullet[nCntBullet].type == BULLETTYPE_REVERSE_CURVE)
			{//エネミー５の弾
				Player* pPlayer = GetPlayer();
				/*	g_aBullet[nCntBullet].move += D3DXVECTOR3((sinf(-D3DX_PI *g_aBullet[nCntBullet].fCurveBulletX) * 0.1f), (cosf(-D3DX_PI *
						g_aBullet[nCntBullet].fCurveBulletX) * 0.1f), 0.0f);*/

				SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 0.0f, 1.0f, 0.5f), 15.0f, 300, EFFECTSCALE_NORMAL);

				g_aBullet[nCntBullet].fCurveBulletX += 0.001f;/*
				g_aBullet[nCntBullet].fCurveBulletY += 0.02f;*/
				/*	g_fCurveBullet += 0.1f;*/

					//if (g_aBullet[nCntBullet].fCurveBulletX >= 0.3f)
					//{
					//	g_aBullet[nCntBullet].bCurveFlag == true;
					//}
					//if (g_aBullet[nCntBullet].bCurveFlag == true)
					//{
					//	g_aBullet[nCntBullet].fCurveBulletX -= 0.01f;
					//	g_aBullet[nCntBullet].move.x += (cosf(D3DX_PI * g_aBullet[nCntBullet].fCurveBulletX) * 0.1f);
					//}
					//if (g_aBullet[nCntBullet].fCurveBulletX <= -0.3f)
					//{
					//	g_aBullet[nCntBullet].bCurveFlag == false;
					//}
					//if (g_aBullet[nCntBullet].bCurveFlag == false)
					//{
					//	g_aBullet[nCntBullet].fCurveBulletX += 0.01f;//カーブ用処理
					//	g_aBullet[nCntBullet].move.x += (sinf(D3DX_PI * g_aBullet[nCntBullet].fCurveBulletX) * 0.1f);
					//}


				/*g_aBullet[nCntBullet].move.x += (sinf(D3DX_PI * g_aBullet[nCntBullet].fCurveBulletX) * 0.1f);*/

				/*for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pPlayer++)
				{*/

				/*	g_aBullet[nCntBullet].move.x += (sinf(-D3DX_PI * g_fCurveBullet) * 5.0f);///面白すぎたので後でほかの敵に実装する！！！！！！！！！！！！！！
						g_aBullet[nCntBullet].move.y += (sinf(-D3DX_PI * 0.0f) * 0.002f);*/
						/*g_aBullet[nCntBullet].move.x = 0.0f;*/
						//int nCntBullet3;
						//for (nCntBullet3 = 0; nCntBullet3 < MAX_CIRCLEBULLET; nCntBullet3++)
						//{
						//	g_fCircleBullet_B[nCntBullet3] = (1.0f / 5.0f) * nCntBullet3;

					/*	for (nCntBullet3 = 0; nCntBullet3 < MAX_CIRCLEBULLET_2; nCntBullet3++)
						{*/
						/*	g_nDelayBullet[nCntBullet3]++;*/
							//	g_fCircleBullet[nCntBullet3] = (1.0f / 5.0f) * nCntBullet3;
				g_aBullet[nCntBullet].nApperCount++;

				//if (g_aBullet[nCntBullet].nApperCount == 1)
				//{
				//	//g_aBullet[nCntBullet].fEnemy5Curve = (D3DX_PI / 5.0f) * (nCntBullet + g_fRandumResultBullet2_B);
				//	g_aBullet[nCntBullet].fEnemy5Curve = (D3DX_PI / 5.0f) * g_aBullet[nCntBullet].nNumber + g_fRandumResultBullet2_B;
				//}


				//パターン１======================================
				/*if (g_nDelayBullet % 5== 0)
				{
					g_aBullet[nCntBullet].fEnemy5Curve += ((D3DX_PI / 15000.0f) * g_aBullet[nCntBullet].nApperCount);
				}*/
				//======================================================================================================
				//パターン２===========================================================

					if (g_aBullet[nCntBullet].nApperCount == 1)
					{
						g_aBullet[nCntBullet].fEnemy5Curve = (D3DX_PI / 8.0f) * g_aBullet[nCntBullet].nNumber + g_fRandumResultBullet2_B;
					}

				if (g_nDelayBullet % 5 == 0)
				{
					g_aBullet[nCntBullet].fEnemy5Curve -= ((D3DX_PI / 50.0f) * 0.5f);
				}
				//===========================================================================

				
					g_aBullet[nCntBullet].move.x = (sinf(g_aBullet[nCntBullet].fEnemy5Curve) * 3.5f);
					g_aBullet[nCntBullet].move.y = (cosf(g_aBullet[nCntBullet].fEnemy5Curve) * 3.5f);
				


				/*	g_aBullet[nCntBullet].move.x += (sinf(g_fCircleBullet_B[nCntBullet3] + g_fRandumResultBullet2_B) * 0.1f);
					g_aBullet[nCntBullet].move.y += (cosf(g_fCircleBullet_B[nCntBullet3] + g_fRandumResultBullet2_B) * 0.1f);*/

					if (g_aBullet[nCntBullet].pos.x >= pPlayer->pos.x - pPlayer->fWidth && g_aBullet[nCntBullet].pos.x <= pPlayer->pos.x + pPlayer->fWidth &&
						g_aBullet[nCntBullet].pos.y >= pPlayer->pos.y - pPlayer->fHeight && g_aBullet[nCntBullet].pos.y <= pPlayer->pos.y)
					{//プレイヤーに弾が当たった
						SetExplosion(pPlayer->pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), EXPLOSIONTYPE_NORMAL, 10.0f, 10.0f);
						//敵の弾を使用していない状態にする
						g_aBullet[nCntBullet].bUse = false;//弾を使用していない状態にする
						//UpdateHealth(1.0f);//追加予定（仮定）
						DamageHealth(10);
						//pHealth -= 1;//なぜか２ダメージ入ってしまうので、１回復する。
					}
				/*}*/
			}
			if (g_aBullet[nCntBullet].type == BULLETTYPE_RIGHT_WIND)
			{//敵の弾(敵全体に適用）

				
				g_aBullet[nCntBullet].move.x += (sinf(D3DX_PI * 0.5f) * 0.09f);


				SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(0.2f, 0.6f, 0.1f, 0.5f), 15.0f, 300, EFFECTSCALE_NORMAL);

				if (g_aBullet[nCntBullet].pos.x >= pPlayer->pos.x - 5.0f && g_aBullet[nCntBullet].pos.x <= pPlayer->pos.x + 5.0f &&
					g_aBullet[nCntBullet].pos.y >= pPlayer->pos.y - 5.0f && g_aBullet[nCntBullet].pos.y <= pPlayer->pos.y + 5.0f)
				{//プレイヤーに弾が当たった
					SetExplosion(pPlayer->pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),EXPLOSIONTYPE_NORMAL,10.0f,10.0f);
					//敵の弾を使用していない状態にする
					g_aBullet[nCntBullet].bUse = false;//弾を使用していない状態にする
					//UpdateHealth(1.0f);//追加予定（仮定）
					HitPlayer(1);
				}
			}
			if (g_aBullet[nCntBullet].type == BULLETTYPE_LEFT_WIND)
			{//敵の弾(敵全体に適用）


				g_aBullet[nCntBullet].move.x += (sinf(-D3DX_PI * 0.5f) * 0.09f);


				SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(0.2f, 0.6f, 0.1f, 0.5f), 15.0f, 300, EFFECTSCALE_NORMAL);

				if (g_aBullet[nCntBullet].pos.x >= pPlayer->pos.x - 5.0f && g_aBullet[nCntBullet].pos.x <= pPlayer->pos.x + 5.0f &&
					g_aBullet[nCntBullet].pos.y >= pPlayer->pos.y - 5.0f && g_aBullet[nCntBullet].pos.y <= pPlayer->pos.y + 5.0f)
				{//プレイヤーに弾が当たった
					SetExplosion(pPlayer->pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),EXPLOSIONTYPE_NORMAL,10.0f,10.0f);
					//敵の弾を使用していない状態にする
					g_aBullet[nCntBullet].bUse = false;//弾を使用していない状態にする
					//UpdateHealth(1.0f);//追加予定（仮定）
					HitPlayer(1);
				}
			}
			if (g_aBullet[nCntBullet].type == BULLETTYPE_UPWIND)
			{//敵の弾(敵全体に適用）


				g_aBullet[nCntBullet].move.y += (cosf(-D3DX_PI * 1.0f) * 0.09f);


				SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(0.2f, 0.6f, 0.1f, 0.5f), 15.0f, 300, EFFECTSCALE_NORMAL);

				if (g_aBullet[nCntBullet].pos.x >= pPlayer->pos.x - 5.0f && g_aBullet[nCntBullet].pos.x <= pPlayer->pos.x + 5.0f &&
					g_aBullet[nCntBullet].pos.y >= pPlayer->pos.y - 5.0f && g_aBullet[nCntBullet].pos.y <= pPlayer->pos.y + 5.0f)
				{//プレイヤーに弾が当たった
					SetExplosion(pPlayer->pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),EXPLOSIONTYPE_NORMAL,10.0f,10.0f);
					//敵の弾を使用していない状態にする
					g_aBullet[nCntBullet].bUse = false;//弾を使用していない状態にする
					//UpdateHealth(1.0f);//追加予定（仮定）
					HitPlayer(1);
				}
			}
			if (g_aBullet[nCntBullet].type == BULLETTYPE_DOWNWIND)
			{//敵の弾(敵全体に適用）


				g_aBullet[nCntBullet].move.y += (cosf(-D3DX_PI * 0.0f) * 0.09f);


				SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(0.2f, 0.6f, 0.1f, 0.5f), 15.0f, 300, EFFECTSCALE_NORMAL);

				if (g_aBullet[nCntBullet].pos.x >= pPlayer->pos.x - 5.0f && g_aBullet[nCntBullet].pos.x <= pPlayer->pos.x + 5.0f &&
					g_aBullet[nCntBullet].pos.y >= pPlayer->pos.y - 5.0f && g_aBullet[nCntBullet].pos.y <= pPlayer->pos.y + 5.0f)
				{//プレイヤーに弾が当たった
					SetExplosion(pPlayer->pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),EXPLOSIONTYPE_NORMAL,10.0f,10.0f);
					//敵の弾を使用していない状態にする
					g_aBullet[nCntBullet].bUse = false;//弾を使用していない状態にする
					//UpdateHealth(1.0f);//追加予定（仮定）
					HitPlayer(1);
				}
			}
			if (g_aBullet[nCntBullet].type == BULLETTYPE_SNOW)
			{
				SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f), 10.0f, 70, EFFECTSCALE_NORMAL);
				if (g_aBullet[nCntBullet].pos.x >= pPlayer->pos.x - pPlayer->fWidth &&
					g_aBullet[nCntBullet].pos.x <= pPlayer->pos.x + pPlayer->fWidth &&
					g_aBullet[nCntBullet].pos.y >= pPlayer->pos.y - pPlayer->fHeight &&
					g_aBullet[nCntBullet].pos.y <= pPlayer->pos.y)
				{
					DamageHealth(7);
					SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), EXPLOSIONTYPE_NORMAL, 10.0f, 10.0f);
				}
			}
			if (g_aBullet[nCntBullet].type == BULLETTYPE_REFLECTION)
			{//スキル３（Xアロー）

				SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f), 10.0f, 30, EFFECTSCALE_NORMAL);
				
				if (g_aBullet[nCntBullet].pos.x >= SCREEN_WIDTH || g_aBullet[nCntBullet].pos.x <= 0.0f)
				{
					if (g_aBullet[nCntBullet].bBulletReflectionX == false)
					{
						g_aBullet[nCntBullet].bBulletReflectionX = true;
					}
					else if (g_aBullet[nCntBullet].bBulletReflectionX == true)
					{
						g_aBullet[nCntBullet].bBulletReflectionX = false;
					}
				}
				if (g_aBullet[nCntBullet].pos.y >= SCREEN_HEIGHT || g_aBullet[nCntBullet].pos.y <= 0.0f)
				{
					if (g_aBullet[nCntBullet].bBulletReflectionY == false)
					{
						g_aBullet[nCntBullet].bBulletReflectionY = true;
					}
					else if (g_aBullet[nCntBullet].bBulletReflectionY == true)
					{
						g_aBullet[nCntBullet].bBulletReflectionY = false;
					}
				}



				if (g_aBullet[nCntBullet].bBulletReflectionX == true)
				{
					g_aBullet[nCntBullet].pos.x -= g_aBullet[nCntBullet].move.x;
				}
				if (g_aBullet[nCntBullet].bBulletReflectionX == false)
				{
					g_aBullet[nCntBullet].pos.x += g_aBullet[nCntBullet].move.x;
				}
				if (g_aBullet[nCntBullet].bBulletReflectionY == true)
				{
					g_aBullet[nCntBullet].pos.y -= g_aBullet[nCntBullet].move.y;
				}
				if (g_aBullet[nCntBullet].bBulletReflectionY == false)
				{
					g_aBullet[nCntBullet].pos.y += g_aBullet[nCntBullet].move.y;
				}

			
					g_aBullet[nCntBullet].move += g_aBullet[nCntBullet].move * 0.002f;
				

					//当たり判定
				if (g_aBullet[nCntBullet].pos.x >= pPlayer->pos.x - 5.0f && g_aBullet[nCntBullet].pos.x <= pPlayer->pos.x + 5.0f &&
					g_aBullet[nCntBullet].pos.y >= pPlayer->pos.y - 5.0f && g_aBullet[nCntBullet].pos.y <= pPlayer->pos.y + 5.0f)
				{//プレイヤーに弾が当たった
					SetExplosion(pPlayer->pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),EXPLOSIONTYPE_NORMAL,10.0f,10.0f);
					//敵の弾を使用していない状態にする
					g_aBullet[nCntBullet].bUse = false;//弾を使用していない状態にする
					//UpdateHealth(1.0f);//追加予定（仮定）
					HitPlayer(1);
				}
			}
			if (g_aBullet[nCntBullet].type == BULLETTYPE_SKILL1)
			{//プレイヤーのスキル１
				Enemy* pEnemy = GetEnemy();
				for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				{
					if (pEnemy->bUse == true)
					{//敵が使用されている
						
							if (g_aBullet[nCntBullet].pos.x >= pEnemy->pos.x - pEnemy->fWidth / 2 - BIGBULLET_LV1 && g_aBullet[nCntBullet].pos.x <= pEnemy->pos.x + pEnemy->fWidth / 2 + BIGBULLET_LV1 &&
								g_aBullet[nCntBullet].pos.y >= pEnemy->pos.y - pEnemy->fHeight - BIGBULLET_LV1 && g_aBullet[nCntBullet].pos.y <= pEnemy->pos.y + BIGBULLET_LV1)
							{//敵と弾が当たった
								HitEnemy(nCntEnemy, 40.0f);//威力５０！超高威力弾

								SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), EXPLOSIONTYPE_NORMAL,10.0f,10.0f);
								//pEnemy->bUse = false;//敵を使用していない状態にする
								g_aBullet[nCntBullet].bUse = false;//弾を使用していない状態にする

							}
						
					}
				}
			}
			if (g_aBullet[nCntBullet].type == BULLETTYPE_SKILL2_SLASH)
			{//プレイヤーのスキル2
				Enemy* pEnemy = GetEnemy();
				
			
				for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				{
					if (pEnemy->bUse == true)
					{//敵が使用されている
							if (g_aBullet[nCntBullet].pos.x >= pEnemy->pos.x - pEnemy->fWidth / 2 - BIGBULLET_LV2 && g_aBullet[nCntBullet].pos.x <= pEnemy->pos.x + pEnemy->fWidth / 2 + BIGBULLET_LV2 &&
								g_aBullet[nCntBullet].pos.y >= pEnemy->pos.y - pEnemy->fHeight - BIGBULLET_LV2 && g_aBullet[nCntBullet].pos.y <= pEnemy->pos.y + BIGBULLET_LV2)
							{//敵と弾が当たった
								HitEnemy(nCntEnemy, 2.0f);//威力１００（物理攻撃）

								SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), EXPLOSIONTYPE_NORMAL,10.0f,10.0f);

								/*	if (g_aBullet[nCntBullet].bUse == false)
									{
										g_aBullet[nCntBullet].fAnimationPattern_Slash = 0.0f;
										g_aBullet[nCntBullet].nAnimationCounter_Slash = 0;
									}*/
									//pEnemy->bUse = false;//敵を使用していない状態にする
									//g_aBullet[nCntBullet].bUse = false;//弾を使用していない状態にする
									//g_aBullet[nCntBullet].bUse = false;//弾を使用していない状態にする

							}
					}
				}
			}
			if (g_aBullet[nCntBullet].type == BULLETTYPE_ARROW)
			{//スキル３（Xアロー）
				g_aBullet[nCntBullet].nCntTime++;

				if (g_aBullet[nCntBullet].nCntTime == 1)
				{
					PlaySound(SOUND_LABEL_SE_ARROWSHOT_000);
				}
				SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(0.2f, 1.0f, 1.0f, 0.5f),50.0f, 70,EFFECTSCALE_NORMAL);

				if (g_aBullet[nCntBullet].pos.x >= SCREEN_WIDTH || g_aBullet[nCntBullet].pos.x <= 0.0f)
				{

					if (g_aBullet[nCntBullet].bBulletReflectionX == false)
					{
						g_aBullet[nCntBullet].bBulletReflectionX= true;
					}
					else if (g_aBullet[nCntBullet].bBulletReflectionX == true)
					{
						g_aBullet[nCntBullet].bBulletReflectionX = false;
					}
				}
				if (g_aBullet[nCntBullet].pos.y >= SCREEN_HEIGHT || g_aBullet[nCntBullet].pos.y <= 0.0f)
				{
					if (g_aBullet[nCntBullet].bBulletReflectionY == false)
					{
						g_aBullet[nCntBullet].bBulletReflectionY = true;
					}
					else if (g_aBullet[nCntBullet].bBulletReflectionY == true)
					{
						g_aBullet[nCntBullet].bBulletReflectionY = false;
					}
				}



				if (g_aBullet[nCntBullet].bBulletReflectionX == true)
				{
					g_aBullet[nCntBullet].pos.x -= g_aBullet[nCntBullet].move.x;
				}
				if (g_aBullet[nCntBullet].bBulletReflectionX == false)
				{
					g_aBullet[nCntBullet].pos.x += g_aBullet[nCntBullet].move.x;
				}
				if (g_aBullet[nCntBullet].bBulletReflectionY == true)
				{
					g_aBullet[nCntBullet].pos.y -= g_aBullet[nCntBullet].move.y;
				}
				if (g_aBullet[nCntBullet].bBulletReflectionY == false)
				{
					g_aBullet[nCntBullet].pos.y += g_aBullet[nCntBullet].move.y;
				}
				

				Enemy* pEnemy = GetEnemy();
				//for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				//{
				//	if (pEnemy->bUse == true)
				//	{//敵が使用されている
				//		
						if (g_aBullet[nCntBullet].pos.x >= pPlayer->pos.x - pPlayer->fWidth && g_aBullet[nCntBullet].pos.x <= pPlayer->pos.x + pPlayer->fWidth&&
							g_aBullet[nCntBullet].pos.y >= pPlayer->pos.y - pPlayer->fHeight&& g_aBullet[nCntBullet].pos.y <= pPlayer->pos.y)
						{//敵と弾が当たった
						/*	HitEnemy(nCntEnemy,15.0f);*/
							DamageHealth(15);

							SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), EXPLOSIONTYPE_NORMAL,10.0f,10.0f);
							//pEnemy->bUse = false;//敵を使用していない状態にする
							g_aBullet[nCntBullet].bUse = false;//弾を使用していない状態にする

						}
				/*	
					}
				}*/
			}
			if (g_aBullet[nCntBullet].type == BULLETTYPE_FINALBALL)
			{//ラスボスの必殺技の弾
				g_aBullet[nCntBullet].nCntTime++;
				if (g_aBullet[nCntBullet].nCntTime == 1)
				{
					PlaySound(SOUND_LABEL_SE_FINALBALL);
				}
				if (g_aBullet[nCntBullet].nCntTime %100 == 0)
				{
					PlaySound(SOUND_LABEL_SE_FINALBALL);
				}
				pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - FINALBALLSIZE, g_aBullet[nCntBullet].pos.y - FINALBALLSIZE, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + FINALBALLSIZE, g_aBullet[nCntBullet].pos.y - FINALBALLSIZE, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - FINALBALLSIZE, g_aBullet[nCntBullet].pos.y + FINALBALLSIZE, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + FINALBALLSIZE, g_aBullet[nCntBullet].pos.y + FINALBALLSIZE, 0.0f);

				if (g_nDelayBullet % 13 == 0)
				{
					SetBullet(g_aBullet[nCntBullet].pos, D3DXVECTOR3(sinf(-D3DX_PI * (0.01f * g_nDelayBullet)) * 0.08f, cosf(-D3DX_PI * (g_nDelayBullet)) * 0.08f, 0.0f),
						GENERALLY_BULLETLIFE, BULLETTYPE_ICICLE, -1, -1, -1, false, MOVETYPE_NORMAL);

					SetBullet(g_aBullet[nCntBullet].pos, D3DXVECTOR3(sinf(-D3DX_PI * (-0.01f * g_nDelayBullet)) * 0.08f, cosf(-D3DX_PI * (0.01f * g_nDelayBullet)) * 0.08f, 0.0f),
						GENERALLY_BULLETLIFE, BULLETTYPE_ICICLE, -1, -1, -1, false, MOVETYPE_NORMAL);
				}

				if (g_aBullet[nCntBullet].nAnimationCounter % 5 == 0)
				{
					g_aBullet[nCntBullet].nAnimationPattern++;

				}
				if (g_aBullet[nCntBullet].nAnimationPattern >= 5)
				{
					g_aBullet[nCntBullet].nAnimationPattern = 0;
				}
				pVtx[0].tex = D3DXVECTOR2(0.0f + 0.2f * g_aBullet[nCntBullet].nAnimationPattern, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.2f + 0.2f * g_aBullet[nCntBullet].nAnimationPattern, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f + 0.2f * g_aBullet[nCntBullet].nAnimationPattern, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.2f + 0.2f * g_aBullet[nCntBullet].nAnimationPattern, 1.0f);
				float fRandumRot = float((rand)() % 100) / 100;

				float fBullet_FinalBoss[20];
				if (g_aBullet[nCntBullet].pos.x >= SCREEN_WIDTH - FINALBALLSIZE || g_aBullet[nCntBullet].pos.x <= 0.0f + FINALBALLSIZE)
				{
					for (int nCnt = 0; nCnt < 20; nCnt++)
					{
						fBullet_FinalBoss[nCnt] = (1.0f / 10) * nCnt;
						SetBullet(g_aBullet[nCntBullet].pos, D3DXVECTOR3(sinf(-D3DX_PI * (fRandumRot + fBullet_FinalBoss[nCnt])) * 5.0f, cosf(-D3DX_PI * (fRandumRot + fBullet_FinalBoss[nCnt])) * 5.0f, 0.0f),
							GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, -1, -1, -1, false, MOVETYPE_NORMAL);
					}
					float fData = 0.0f;
						SetAttack(g_aBullet[nCntBullet].pos,
							D3DXVECTOR3(0.0f, 0.0f, 0.0f), 
							ATTACKTYPE_FINALBEAM,
							-1, -1, -1, MOVETYPE_NORMAL, 50.0f, 1280.0f, 20, D3DX_PI * 1.0f, -fData);

						SetAttack(g_aBullet[nCntBullet].pos,
							D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							ATTACKTYPE_FINALBEAM,
							-1, -1, -1, MOVETYPE_NORMAL, 50.0f, 1280.0f, 20, D3DX_PI * 0.75f, -fData);

						SetAttack(g_aBullet[nCntBullet].pos,
							D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							ATTACKTYPE_FINALBEAM,
							-1, -1, -1, MOVETYPE_NORMAL, 50.0f, 1280.0f, 20, D3DX_PI * 0.5f, -fData);

						SetAttack(g_aBullet[nCntBullet].pos,
							D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							ATTACKTYPE_FINALBEAM,
							-1, -1, -1, MOVETYPE_NORMAL, 50.0f, 1280.0f, 20, D3DX_PI * 0.25f, -fData);

						SetAttack(g_aBullet[nCntBullet].pos,
							D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							ATTACKTYPE_FINALBEAM,
							-1, -1, -1, MOVETYPE_NORMAL, 50.0f, 1280.0f, 20, D3DX_PI * 0.0f, -fData);

						SetAttack(g_aBullet[nCntBullet].pos,
							D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							ATTACKTYPE_FINALBEAM,
							-1, -1, -1, MOVETYPE_NORMAL, 50.0f, 1280.0f, 20, -D3DX_PI * 0.5f, -fData);

						SetAttack(g_aBullet[nCntBullet].pos,
							D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							ATTACKTYPE_FINALBEAM,
							-1, -1, -1, MOVETYPE_NORMAL, 50.0f, 1280.0f, 20, -D3DX_PI * 0.75f, -fData);

						SetAttack(g_aBullet[nCntBullet].pos,
							D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							ATTACKTYPE_FINALBEAM,
							-1, -1, -1, MOVETYPE_NORMAL, 50.0f, 1280.0f, 20, -D3DX_PI * 0.25f, -fData);

					if (g_aBullet[nCntBullet].bBulletReflectionX == false)
					{
						g_aBullet[nCntBullet].bBulletReflectionX = true;
					}
					else if (g_aBullet[nCntBullet].bBulletReflectionX == true)
					{
						g_aBullet[nCntBullet].bBulletReflectionX = false;
					}
				}
				if (g_aBullet[nCntBullet].pos.y >= SCREEN_HEIGHT - FINALBALLSIZE || g_aBullet[nCntBullet].pos.y <= 0.0f + FINALBALLSIZE)
				{
					float fData = 0.0f;

					for (int nCnt = 0; nCnt < 20; nCnt++)
					{
						fBullet_FinalBoss[nCnt] = (1.0f / 10) * nCnt;
						SetBullet(g_aBullet[nCntBullet].pos, D3DXVECTOR3(sinf(-D3DX_PI * (fRandumRot + fBullet_FinalBoss[nCnt])) * 5.0f, cosf(-D3DX_PI * (fRandumRot + fBullet_FinalBoss[nCnt])) * 5.0f, 0.0f),
							GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, -1, -1, -1, false, MOVETYPE_NORMAL);
					}
				
					SetAttack(g_aBullet[nCntBullet].pos,
						D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						ATTACKTYPE_FINALBEAM,
						-1, -1, -1, MOVETYPE_NORMAL, 50.0f, 1280.0f, 20, D3DX_PI * 1.0f, -fData);

					SetAttack(g_aBullet[nCntBullet].pos,
						D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						ATTACKTYPE_FINALBEAM,
						-1, -1, -1, MOVETYPE_NORMAL, 50.0f, 1280.0f, 20, D3DX_PI * 0.75f, -fData);

					SetAttack(g_aBullet[nCntBullet].pos,
						D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						ATTACKTYPE_FINALBEAM,
						-1, -1, -1, MOVETYPE_NORMAL, 50.0f, 1280.0f, 20, D3DX_PI * 0.5f, -fData);

					SetAttack(g_aBullet[nCntBullet].pos,
						D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						ATTACKTYPE_FINALBEAM,
						-1, -1, -1, MOVETYPE_NORMAL, 50.0f, 1280.0f, 20, D3DX_PI * 0.25f, -fData);

					SetAttack(g_aBullet[nCntBullet].pos,
						D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						ATTACKTYPE_FINALBEAM,
						-1, -1, -1, MOVETYPE_NORMAL, 50.0f, 1280.0f, 20, D3DX_PI * 0.0f, -fData);

					SetAttack(g_aBullet[nCntBullet].pos,
						D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						ATTACKTYPE_FINALBEAM,
						-1, -1, -1, MOVETYPE_NORMAL, 50.0f, 1280.0f, 20, -D3DX_PI * 0.5f, -fData);

					SetAttack(g_aBullet[nCntBullet].pos,
						D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						ATTACKTYPE_FINALBEAM,
						-1, -1, -1, MOVETYPE_NORMAL, 50.0f, 1280.0f, 20, -D3DX_PI * 0.75f, -fData);

					SetAttack(g_aBullet[nCntBullet].pos,
						D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						ATTACKTYPE_FINALBEAM,
						-1, -1, -1, MOVETYPE_NORMAL, 50.0f, 1280.0f, 20, -D3DX_PI * 0.25f, -fData);

					if (g_aBullet[nCntBullet].bBulletReflectionY == false)
					{
						g_aBullet[nCntBullet].bBulletReflectionY = true;
					}
					else if (g_aBullet[nCntBullet].bBulletReflectionY == true)
					{
						g_aBullet[nCntBullet].bBulletReflectionY = false;
					}
				}



				if (g_aBullet[nCntBullet].bBulletReflectionX == true)
				{
					g_aBullet[nCntBullet].pos.x -= g_aBullet[nCntBullet].move.x;
				}
				if (g_aBullet[nCntBullet].bBulletReflectionX == false)
				{
					g_aBullet[nCntBullet].pos.x += g_aBullet[nCntBullet].move.x;
				}
				if (g_aBullet[nCntBullet].bBulletReflectionY == true)
				{
					g_aBullet[nCntBullet].pos.y -= g_aBullet[nCntBullet].move.y;
				}
				if (g_aBullet[nCntBullet].bBulletReflectionY == false)
				{
					g_aBullet[nCntBullet].pos.y += g_aBullet[nCntBullet].move.y;
				}


				Enemy* pEnemy = GetEnemy();
				//for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				//{
				//	if (pEnemy->bUse == true)
				//	{//敵が使用されている
				//		
				if (pPlayer->pos.x >= g_aBullet[nCntBullet].pos.x - FINALBALLSIZE - pPlayer->fWidth && pPlayer->pos.x <= g_aBullet[nCntBullet].pos.x + FINALBALLSIZE + pPlayer->fWidth &&
					pPlayer->pos.y >= g_aBullet[nCntBullet].pos.y - FINALBALLSIZE - pPlayer->fHeight && pPlayer->pos.y <= g_aBullet[nCntBullet].pos.y + FINALBALLSIZE)
				{//敵と弾が当たった
				/*	HitEnemy(nCntEnemy,15.0f);*/
					DamageHealth(10);

					SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), EXPLOSIONTYPE_NORMAL, 10.0f, 10.0f);
					//pEnemy->bUse = false;//敵を使用していない状態にする
					//g_aBullet[nCntBullet].bUse = false;//弾を使用していない状態にする

				}
				/*
					}
				}*/
			}
			if (g_aBullet[nCntBullet].type == BULLETTYPE_NOVA)
			{//プレイヤーのスキル5

				g_aBullet[nCntBullet].nCntTime++;
				if (g_aBullet[nCntBullet].nCntTime == 1)
				{
					PlaySound(SOUND_LABEL_SE_FINALBALL);
				}
				Enemy* pEnemy = GetEnemy();
				float fRandumColorR;
				float fRandumColorG;
				float fRandumColorB;
				float fRandumColorA;
				fRandumColorR = float((rand)() % 100) / 10.0f;
				fRandumColorG = float((rand)() % 100) / 10.0f;
				fRandumColorB = float((rand)() % 100) / 10.0f;
			/*	fRandumSpeed = float((rand)() % 100) / 5.0f;*/
				SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(fRandumColorR,fRandumColorG,fRandumColorB, 0.5f), 50.0f, 300, EFFECTSCALE_NORMAL);
				//for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				//{
				//	if (pEnemy->bUse == true)
				//	{//敵が使用されている
				//		
							if (g_aBullet[nCntBullet].pos.x >= pPlayer->pos.x - pPlayer->fWidth - 120.0f && g_aBullet[nCntBullet].pos.x <= pPlayer->pos.x + pPlayer->fWidth + 120.0f &&
								g_aBullet[nCntBullet].pos.y >= pPlayer->pos.y - pPlayer->fHeight && g_aBullet[nCntBullet].pos.y <= pPlayer->pos.y + 120.0f)
							{//敵と弾が当たった
								//HitEnemy(nCntEnemy,100.0f);//威力１００（物理攻撃）

								DamageHealth(100);

								SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), EXPLOSIONTYPE_NOVA,400.0f,400.0f);
								g_aBullet[nCntBullet].bUse = false;//弾を使用していない状態にする
							}
						
				/*	}
				}*/
			}
			if (g_aBullet[nCntBullet].type == BULLETTYPE_SEED)
			{
				g_aBullet[nCntBullet].nCntTime++;
				if (g_aBullet[nCntBullet].nCntTime == 1)
				{
					PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_006);
				}

			
				float fVector[12];

				float fRandumRot;
				float fRandumSpeed;
				fRandumRot = float((rand)() % 200) / 100.0f;
				fRandumSpeed = float((rand)() % 100) / 5.0f;
				if (g_aBullet[nCntBullet].nLife == 0)
				{
					for (int nCntCircleBullet = 0; nCntCircleBullet < 12; nCntCircleBullet++)
					{
						fVector[nCntCircleBullet] = (2.0f / 12.0f) * nCntCircleBullet;
                        
						SetBullet(g_aBullet[nCntBullet].pos, D3DXVECTOR3(sinf(-D3DX_PI * (fVector[nCntCircleBullet] + fRandumRot)) * 10.0f,
							cosf(-D3DX_PI * (fVector[nCntCircleBullet] + fRandumRot)) * 10.0f, 0.0f),
							200, BULLETTYPE_ENEMY, -1, -1, -1, false, MOVETYPE_GRAVITY);
					}
				}
			}
			if (g_aBullet[nCntBullet].type == BULLETTYPE_JUMPING)
			{
				g_aBullet[nCntBullet].nCntTime++;
				if (g_aBullet[nCntBullet].nCntTime == 1)
				{
					PlaySound(SOUND_LABEL_SE_SHOT);
				}
				float fVector[12];

				float fRandumRot;
				float fRandumSpeed;
 				fRandumRot = float((rand)() % 200) / 100.0f;
				fRandumSpeed = float((rand)() % 100) / 5.0f;
				SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 10.0f, 30, EFFECTSCALE_NORMAL);
				if (g_aBullet[nCntBullet].pos.x >= pPlayer->pos.x - pPlayer->fWidth && g_aBullet[nCntBullet].pos.x <= pPlayer->pos.x + pPlayer->fWidth &&
					g_aBullet[nCntBullet].pos.y >= pPlayer->pos.y - pPlayer->fHeight && g_aBullet[nCntBullet].pos.y <= pPlayer->pos.y)
				{//プレイヤーに弾が当たった

				/*	PlaySound(SOUND_LABEL_SE_DAMAGE_000);*/

					SetExplosion(pPlayer->pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), EXPLOSIONTYPE_NORMAL, 10.0f, 10.0f);
					//敵の弾を使用していない状態にする
					g_aBullet[nCntBullet].bUse = false;//弾を使用していない状態にする
					DamageHealth(5);
				}

				if (g_aBullet[nCntBullet].pos.y >= 665.0f)
				{
					g_aBullet[nCntBullet].move.y = -15.0f;
				}
			}
			if (g_aBullet[nCntBullet].type == BULLETTYPE_ICICLE)
			{
				g_aBullet[nCntBullet].nCntTime++;
				if (g_aBullet[nCntBullet].nCntTime == 1)
				{
					PlaySound(SOUND_LABEL_SE_JUMPINGBULLET);
				}
				/*float fVector[12];*/

				float fRandumRot;
				float fRandumSpeed;
				fRandumRot = float((rand)() % 200) / 100.0f;
				fRandumSpeed = float((rand)() % 100) / 5.0f;

				SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(0.5f,0.5f,0.0f, 0.5f), 15.0f, 300, EFFECTSCALE_NORMAL);

				pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - 10.0f, g_aBullet[nCntBullet].pos.y - 10.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + 10.0f, g_aBullet[nCntBullet].pos.y - 10.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - 10.0f, g_aBullet[nCntBullet].pos.y + 10.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + 10.0f, g_aBullet[nCntBullet].pos.y + 10.0f, 0.0f);

				if (g_aBullet[nCntBullet].pos.x >= pPlayer->pos.x - pPlayer->fWidth - 10.0f &&
					g_aBullet[nCntBullet].pos.x <= pPlayer->pos.x + pPlayer->fWidth + 10.0f &&
					g_aBullet[nCntBullet].pos.y >= pPlayer->pos.y - pPlayer->fHeight - 10.0f &&
					g_aBullet[nCntBullet].pos.y <= pPlayer->pos.y + 10.0f)
				{
					DamageHealth(7);
					SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), EXPLOSIONTYPE_NORMAL, 10.0f, 10.0f);
					g_aBullet[nCntBullet].bUse = false;
				}
				//少しずつ加速する弾
				g_aBullet[nCntBullet].move += g_aBullet[nCntBullet].move / 50;

				
			}
			if (g_aBullet[nCntBullet].type == BULLETTYPE_SPLET)
			{
				g_aBullet[nCntBullet].nCntTime++;
				if (g_aBullet[nCntBullet].nCntTime == 1)
				{
					PlaySound(SOUND_LABEL_SE_SPLETBULLET);
				}
				/*float fVector[12];*/

				float fRandumRot;
				float fRandumSpeed;
			

				pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - 30.0f, g_aBullet[nCntBullet].pos.y - 30.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + 30.0f, g_aBullet[nCntBullet].pos.y - 30.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - 30.0f, g_aBullet[nCntBullet].pos.y + 30.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + 30.0f, g_aBullet[nCntBullet].pos.y + 30.0f, 0.0f);

				if (g_aBullet[nCntBullet].pos.x >= pPlayer->pos.x - pPlayer->fWidth - 30.0f &&
					g_aBullet[nCntBullet].pos.x <= pPlayer->pos.x + pPlayer->fWidth + 30.0f &&
					g_aBullet[nCntBullet].pos.y >= pPlayer->pos.y - pPlayer->fHeight - 30.0f &&
					g_aBullet[nCntBullet].pos.y <= pPlayer->pos.y + 30.0f)
				{
					DamageHealth(24);
					SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), EXPLOSIONTYPE_NORMAL, 10.0f, 10.0f);
					g_aBullet[nCntBullet].bUse = false;

					
				}
			
				if (g_aBullet[nCntBullet].pos.x >= SCREEN_WIDTH - 20.0f||
					g_aBullet[nCntBullet].pos.x <= 0.0f + 20.0f||
					g_aBullet[nCntBullet].pos.y >= SCREEN_HEIGHT - 20.0f ||
					g_aBullet[nCntBullet].pos.y <= 0.0f + 20.0f)
				{
					SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), EXPLOSIONTYPE_NORMAL, 10.0f, 10.0f);
					g_aBullet[nCntBullet].bUse = false;


				
					for (int nCnt = 0; nCnt < 15; nCnt++)
					{
						fRandumRot = float((rand)() % 200) / 100.0f;
						fRandumSpeed = float((rand)() % 100) / 20.0f;

						SetBullet(g_aBullet[nCntBullet].pos, D3DXVECTOR3(sinf(-D3DX_PI * fRandumRot) * fRandumSpeed, cosf(-D3DX_PI * fRandumRot) * fRandumSpeed, 0.0f),
							600, BULLETTYPE_ENEMY, -1, -1, -1, false, MOVETYPE_NORMAL);
					}
				}

			}
			if (g_aBullet[nCntBullet].type == BULLETTYPE_EXPLOSION)
			{
				g_aBullet[nCntBullet].nCntTime++;
				if (g_aBullet[nCntBullet].nCntTime == 1)
				{
					PlaySound(SOUND_LABEL_SE_FINALBALL);
				}
				/*float fVector[12];*/

				float fRandumRot;
				float fRandumSpeed;


				pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - 30.0f, g_aBullet[nCntBullet].pos.y - 30.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + 30.0f, g_aBullet[nCntBullet].pos.y - 30.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - 30.0f, g_aBullet[nCntBullet].pos.y + 30.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + 30.0f, g_aBullet[nCntBullet].pos.y + 30.0f, 0.0f);

				/*if (g_aBullet[nCntBullet].pos.x >= pPlayer->pos.x - pPlayer->fWidth - 30.0f &&
					g_aBullet[nCntBullet].pos.x <= pPlayer->pos.x + pPlayer->fWidth + 30.0f &&
					g_aBullet[nCntBullet].pos.y >= pPlayer->pos.y - pPlayer->fHeight - 30.0f &&
					g_aBullet[nCntBullet].pos.y <= pPlayer->pos.y + 30.0f)
				{
					DamageHealth(40);
					SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), EXPLOSIONTYPE_NORMAL, 10.0f, 10.0f);
					g_aBullet[nCntBullet].bUse = false;


				}*/


				if (g_nDelayBullet % 10 == 0)
				{
					SetStaging(STAGINGTYPE_EXPLOSIONWARNING, -1, g_aBullet->pos, 60, ATTACKTYPE_PUNCH, -1.0f, 50.0f, -1.0f, -1.0f, -1.0f,- 1.0f);
				}

				if (g_aBullet[nCntBullet].pos.x >= SCREEN_WIDTH - 20.0f ||
					g_aBullet[nCntBullet].pos.x <= 0.0f + 20.0f ||
					g_aBullet[nCntBullet].pos.y >= SCREEN_HEIGHT - 20.0f ||
					g_aBullet[nCntBullet].pos.y <= 0.0f + 20.0f)
				{
					SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), EXPLOSIONTYPE_NORMAL, 10.0f, 10.0f);
					g_aBullet[nCntBullet].bUse = false;



					/*for (int nCnt = 0; nCnt < 15; nCnt++)
					{
						fRandumRot = float((rand)() % 200) / 100.0f;
						fRandumSpeed = float((rand)() % 100) / 20.0f;

						SetBullet(g_aBullet[nCntBullet].pos, D3DXVECTOR3(sinf(-D3DX_PI * fRandumRot) * fRandumSpeed, cosf(-D3DX_PI * fRandumRot) * fRandumSpeed, 0.0f),
							600, BULLETTYPE_ENEMY, -1, -1, -1, false, MOVETYPE_NORMAL);
					}*/
				}

			}
			if (g_aBullet[nCntBullet].type == BULLETTYPE_HORMING)
			{//ホーミング弾

				g_aBullet[nCntBullet].nHormingCount++;

				SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(0.2f, 1.0f, 1.0f, 0.5f), 20.0f, 30,EFFECTSCALE_NORMAL);

				//g_aBullet[nCntBullet].fRotMove = atan2f(g_aBullet[nCntBullet].move.y, g_aBullet[nCntBullet].move.x);//現在の移動方向（角度）
				//g_aBullet[nCntBullet].fRotDest = atan2f(pEnemy->pos.y -g_aBullet[nCntBullet].pos.y, pEnemy->pos.x - g_aBullet[nCntBullet]. pos.x);//プレイヤーから敵への向き//　　　　　＜＜＜＜＜NovaBrast候補＞＞＞＞＞
				//g_aBullet[nCntBullet].fRotDiff = g_aBullet[nCntBullet].fRotDest - g_aBullet[nCntBullet].fRotMove;//目的地までの移動方向の差分

				//g_aBullet[nCntBullet].fRotMove += g_aBullet[nCntBullet].fRotDiff* 5.0f;//移動方向（角度補正）
				//g_aBullet[nCntBullet].move.x = sinf(g_aBullet[nCntBullet].fRotMove) * g_aBullet[nCntBullet].fRotMove;
				//g_aBullet[nCntBullet].move.y = cosf(g_aBullet[nCntBullet].fRotMove) * g_aBullet[nCntBullet].fRotMove;
			
				Enemy* pEnemy = GetEnemy();
				

			/*	if (g_aBullet[nCntBullet].nHormingCount >=50)
				{
				g_aBullet[nCntBullet].HormingFlag =true;
				}*/
				if (g_aBullet[nCntBullet].HormingFlag == false)
				{
					for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
					{
						if (pEnemy->bUse == false && g_aBullet[nCntBullet].nHormingaim == nCntEnemy)
						{
							g_aBullet[nCntBullet].bUse = false;
							SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), EXPLOSIONTYPE_NORMAL,10.0f,10.0f);
						}
						if (pEnemy->bUse == true && g_aBullet[nCntBullet].nHormingaim == nCntEnemy)
						{
							g_aBullet[nCntBullet].fRotMove = atan2f(cosf(D3DX_PI * (0.2f * g_aBullet[nCntBullet].nNumber)) * 100.0f, sinf(D3DX_PI * (0.2f * g_aBullet[nCntBullet].nNumber)) * 100.0f);//現在の移動方向（角度]]）
							g_aBullet[nCntBullet].fRotDest = atan2f(pEnemy->pos.y - g_aBullet[nCntBullet].pos.y, pEnemy->pos.x - g_aBullet[nCntBullet].pos.x);//プレイヤーから敵への向き

						}

					}



					//g_aBullet[nCntBullet].fRotMove = atan2f(sinf(D3DX_PI * (0.1 * g_aBullet[nCntBullet].nNumber)) * 1.0f, cosf(D3DX_PI * (0.1f * g_aBullet[nCntBullet].nNumber)) * 1.0f);//現在の移動方向（角度）
					g_aBullet[nCntBullet].fRotDiff = g_aBullet[nCntBullet].fRotDest - g_aBullet[nCntBullet].fRotMove;//目的地までの移動方向の差分

					g_aBullet[nCntBullet].fRotDiff = -g_aBullet[nCntBullet].fRotDest - g_aBullet[nCntBullet].fRotMove + 6.28f;//角度差分の調整

					g_aBullet[nCntBullet].fRotMove += g_aBullet[nCntBullet].fRotDiff * 10.0f;//移動方向（角度補正）

					g_aBullet[nCntBullet].fRotMove += g_aBullet[nCntBullet].fRotDiff - 6.28f;//moveの角度修正

					g_aBullet[nCntBullet].move.x = sinf(g_aBullet[nCntBullet].fRotMove) * 5.0f;
					g_aBullet[nCntBullet].move.y = cosf(g_aBullet[nCntBullet].fRotMove) * 5.0f;
				}
				
				/*	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
					{
						if (pEnemy->bUse == true)
						{*/
						/*	g_aBullet[nCntBullet].pos.y += g_aBullet[nCntBullet].move.y;
							g_aBullet[nCntBullet].pos.x += g_aBullet[nCntBullet].move.x;*/
					/*	}
					}*/
				g_aBullet[nCntBullet].fRotMove = atan2f(sinf(D3DX_PI*(0.1*g_aBullet[nCntBullet].nNumber))*1.0f,cosf(D3DX_PI*(0.1f*g_aBullet[nCntBullet].nNumber))*1.0f);//現在の移動方向（角度）
				
				g_aBullet[nCntBullet].fRotDest = atan2f(pEnemy->pos.y - g_aBullet[nCntBullet].pos.y, pEnemy->pos.x - g_aBullet[nCntBullet].pos.x);//プレイヤーから敵への向き
				
				g_aBullet[nCntBullet].fRotDiff = g_aBullet[nCntBullet].fRotDest - g_aBullet[nCntBullet].fRotMove;//目的地までの移動方向の差分

				g_aBullet[nCntBullet].fRotDiff = -g_aBullet[nCntBullet].fRotDest - g_aBullet[nCntBullet].fRotMove+6.28f;//角度差分の調整

				g_aBullet[nCntBullet].fRotMove += g_aBullet[nCntBullet].fRotDiff * 10.0f;//移動方向（角度補正）

				g_aBullet[nCntBullet].fRotMove += g_aBullet[nCntBullet].fRotDiff-6.28f;//moveの角度修正

				g_aBullet[nCntBullet].move.x = sinf(g_aBullet[nCntBullet].fRotMove) *10.0f;
				g_aBullet[nCntBullet].move.y = cosf(g_aBullet[nCntBullet].fRotMove) *10.0f;

				if (pEnemy->bUse == false)
				{
					pEnemy++;
				}
			/*	Enemy* pEnemy = GetEnemy();*/
/*
				/*/
				if (g_aBullet[nCntBullet].bUse == true)
				{
					Enemy* pEnemy = GetEnemy();
					for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
					{

						if (pEnemy->bUse == true)
						{//敵が使用されている
							if (g_aBullet[nCntBullet].pos.x >= pEnemy->pos.x - pEnemy->fWidth / 2 - 20.0f && g_aBullet[nCntBullet].pos.x <= pEnemy->pos.x + pEnemy->fWidth / 2 + 20.0f &&
								g_aBullet[nCntBullet].pos.y >= pEnemy->pos.y - pEnemy->fHeight - 20.0f && g_aBullet[nCntBullet].pos.y <= pEnemy->pos.y + 20.0f)
							{//敵と弾が当たった
								HitEnemy(nCntEnemy, 1.0f);

								SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), EXPLOSIONTYPE_NORMAL,10.0f,10.0f);
								//pEnemy->bUse = false;//敵を使用していない状態にする
								g_aBullet[nCntBullet].bUse = false;//弾を使用していない状態にする

							}
						}
					
					}
				}
			}
			//頂点座標の設定(弾の大きさ）
			if (g_aBullet[nCntBullet].moveType == MOVETYPE_NORMAL)
			{
				if (g_aBullet[nCntBullet].type != BULLETTYPE_ARROW)
				{
					if (g_aBullet[nCntBullet].type != BULLETTYPE_FINALBALL)
					{
						if (g_aBullet[nCntBullet].type >= BULLETTYPE_PLAYER || g_aBullet[nCntBullet].type <= BULLETTYPE_BANDY)
						{
							g_aBullet[nCntBullet].pos.y += g_aBullet[nCntBullet].move.y;
							g_aBullet[nCntBullet].pos.x += g_aBullet[nCntBullet].move.x;
						}
						if (g_aBullet[nCntBullet].type == BULLETTYPE_HORMING || g_aBullet[nCntBullet].type == BULLETTYPE_SNOW ||
							g_aBullet[nCntBullet].type == BULLETTYPE_ICICLE ||
							g_aBullet[nCntBullet].type == BULLETTYPE_SPLET ||
							g_aBullet[nCntBullet].type == BULLETTYPE_EXPLOSION)
						{
							g_aBullet[nCntBullet].pos.y += g_aBullet[nCntBullet].move.y;
							g_aBullet[nCntBullet].pos.x += g_aBullet[nCntBullet].move.x;
						}
						if (g_aBullet[nCntBullet].type == BULLETTYPE_SCATTERING || g_aBullet[nCntBullet].type == BULLETTYPE_ROCKON || g_aBullet[nCntBullet].type == BULLETTYPE_SATELLITE)
						{
							g_aBullet[nCntBullet].pos.y += g_aBullet[nCntBullet].move.y;
							g_aBullet[nCntBullet].pos.x += g_aBullet[nCntBullet].move.x;
						}
						if (g_aBullet[nCntBullet].type == BULLETTYPE_REVERSE_CURVE || g_aBullet[nCntBullet].type == BULLETTYPE_LEFT_WIND || g_aBullet[nCntBullet].type == BULLETTYPE_RIGHT_WIND || g_aBullet[nCntBullet].type == BULLETTYPE_NOVA
							|| g_aBullet[nCntBullet].type == BULLETTYPE_DOWNWIND || g_aBullet[nCntBullet].type == BULLETTYPE_UPWIND)
						{
							g_aBullet[nCntBullet].pos.y += g_aBullet[nCntBullet].move.y;
							g_aBullet[nCntBullet].pos.x += g_aBullet[nCntBullet].move.x;
						}
					}
				}
			}
			else if (g_aBullet[nCntBullet].moveType == MOVETYPE_GRAVITY)
			{
				g_aBullet[nCntBullet].move.y += 0.5f;
				g_aBullet[nCntBullet].pos.y += g_aBullet[nCntBullet].move.y;
				g_aBullet[nCntBullet].pos.x += g_aBullet[nCntBullet].move.x;
			}

			if (g_aBullet[nCntBullet].nLife <= 0)
			{//表示時間が０以下になったら（弾の寿命が尽きた）(種類は全て）
				/*g_aBullet[nCntBullet].bUse = false;*/
				//爆発の設定
				SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), EXPLOSIONTYPE_NORMAL, 10.0f, 10.0f);

				g_aBullet[nCntBullet].bUse = false;//使用していない状態にする
			}

			if (g_aBullet[nCntBullet].type == BULLETTYPE_SNOW)
			{
				if (g_aBullet[nCntBullet].pos.x >= pPlayer->pos.x - pPlayer->fWidth - 10.0f &&
					g_aBullet[nCntBullet].pos.x <= pPlayer->pos.x + pPlayer->fWidth + 10.0f &&
					g_aBullet[nCntBullet].pos.y >= pPlayer->pos.y - pPlayer->fHeight - 10.0f &&
					g_aBullet[nCntBullet].pos.y <= pPlayer->pos.y + 10.0f)
				{
					g_aBullet[nCntBullet].bUse = false;
					DamageHealth(4);
				}
				if (g_aBullet[nCntBullet].bUse == false)
				{
					SetAttack(g_aBullet[nCntBullet].pos,
						D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						ATTACKTYPE_ICE_EXPLOSION,
						-1, nCntBullet, -1, MOVETYPE_NORMAL, 150.0f, 150.0f, 28, 0.0f, -1);
				}
			}
		
			//============================================================================================================================================
		}
		

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();
}

//弾の描画処理
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	int nCntBullet;

	Enemy* pEnemy = GetEnemy();
	Player* pPlayer = GetPlayer();

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_2D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{
			if (g_aBullet[nCntBullet].type == BULLETTYPE_PLAYER || g_aBullet[nCntBullet].type == BULLETTYPE_ROT)
			{
				pDevice->SetTexture(0, g_pTextureBullet[0]);
			}
			else if (g_aBullet[nCntBullet].type == BULLETTYPE_ENEMY || g_aBullet[nCntBullet].type == BULLETTYPE_REAFY||g_aBullet[nCntBullet].type== BULLETTYPE_ENEMY9
				|| g_aBullet[nCntBullet].type == BULLETTYPE_SATELLITE)
			{
				pDevice->SetTexture(0, g_pTextureBullet[1]);
			}

			else if (g_aBullet[nCntBullet].type == BULLETTYPE_SKILL1)
			{
				pDevice->SetTexture(0, g_pTextureBullet[2]);
			}
			else if (g_aBullet[nCntBullet].type == BULLETTYPE_SKILL2_SLASH)
			{
				pDevice->SetTexture(0, g_pTextureBullet[3]);
			}
			else if (g_aBullet[nCntBullet].type == BULLETTYPE_BOLBY)
			{
				pDevice->SetTexture(0, g_pTextureBullet[4]);
			}
			else if (g_aBullet[nCntBullet].type == BULLETTYPE_BANDY)
			{
				pDevice->SetTexture(0, g_pTextureBullet[5]);
			}
			else if (g_aBullet[nCntBullet].type == BULLETTYPE_ARROW)
			{
				pDevice->SetTexture(0, g_pTextureBullet[6]);
			}
			else if (g_aBullet[nCntBullet].type == BULLETTYPE_HORMING)
			{
				pDevice->SetTexture(0, g_pTextureBullet[7]);
			}
			else if (g_aBullet[nCntBullet].type == BULLETTYPE_SCATTERING)
			{
				pDevice->SetTexture(0, g_pTextureBullet[8]);
			}
			else if (g_aBullet[nCntBullet].type == BULLETTYPE_REVERSE_CURVE)
			{
				pDevice->SetTexture(0, g_pTextureBullet[9]);
			}
			else if (g_aBullet[nCntBullet].type == BULLETTYPE_RIGHT_WIND||g_aBullet[nCntBullet].type==BULLETTYPE_LEFT_WIND
				||g_aBullet[nCntBullet].type==BULLETTYPE_DOWNWIND||g_aBullet[nCntBullet].type==BULLETTYPE_UPWIND)
			{
				pDevice->SetTexture(0, g_pTextureBullet[10]);
			}
			else if (g_aBullet[nCntBullet].type == BULLETTYPE_REFLECTION)
			{
				pDevice->SetTexture(0, g_pTextureBullet[11]);
			}
			else if (g_aBullet[nCntBullet].type == BULLETTYPE_NOVA)
			{
				pDevice->SetTexture(0, g_pTextureBullet[12]);
			}
			else if (g_aBullet[nCntBullet].type == BULLETTYPE_ROCKON)
			{
				pDevice->SetTexture(0, g_pTextureBullet[13]);
			}
			else if (g_aBullet[nCntBullet].type == BULLETTYPE_SEED)
			{
				pDevice->SetTexture(0, g_pTextureBullet[14]);
			}
			else if (g_aBullet[nCntBullet].type == BULLETTYPE_SNOW)
			{
				pDevice->SetTexture(0, g_pTextureBullet[15]);
			}
			else if (g_aBullet[nCntBullet].type == BULLETTYPE_ICICLE)
			{
				pDevice->SetTexture(0, g_pTextureBullet[16]);
			}
			else if (g_aBullet[nCntBullet].type == BULLETTYPE_SPLET)
			{
				pDevice->SetTexture(0, g_pTextureBullet[17]);
			}
			else if (g_aBullet[nCntBullet].type == BULLETTYPE_JUMPING)
			{
				pDevice->SetTexture(0, g_pTextureBullet[18]);
			}
			else if (g_aBullet[nCntBullet].type == BULLETTYPE_EXPLOSION)
			{
				pDevice->SetTexture(0, g_pTextureBullet[19]);
			}
			else if (g_aBullet[nCntBullet].type == BULLETTYPE_FINALBALL)
			{
				pDevice->SetTexture(0, g_pTextureBullet[20]);
			}
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,//0は、描画する最初の頂点インデックスで、2は、描画するプリミティブ数
				nCntBullet * 4,
				2);
		}

	}


}

//弾の設定処理
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, BULLETTYPE type, int nNum, int nNumEnemy, int nAimingEnemy, bool bCollision
	, MOVETYPE moveType)//posは「発射位置」、moveは「移動量」,damageは「弾の攻撃力」nNumEneyは、何番目の敵が攻撃しているか?
{
	int nCntBullet;

	VERTEX_2D* pVtx;
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet->nCntBullet_2 = nCntBullet;

		if (g_aBullet[nCntBullet].bUse == false)
		{//弾が使用されていない
		/*	if (BULLETTYPE_ENEMY || BULLETTYPE_PLAYER || BULLETTYPE_SKILL1||BULLETTYPE_REAFY)
			{*/

			g_aBullet[nCntBullet].pos = pos;

			//頂点座標を更新
		/*	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(15.0f, 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, -15.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(15.0f, 15.0f, 0.0f);*/

		
				g_aBullet[nCntBullet].move = move;
				g_aBullet[nCntBullet].nLife = nLife;
				g_aBullet[nCntBullet].bUse = true;//使用している状態にする
				g_aBullet[nCntBullet].type = type;//弾の種類（type)を代入する。
				g_aBullet[nCntBullet].moveType = moveType;
				g_aBullet[nCntBullet].fCurveBulletX = 0.0f;
				g_aBullet[nCntBullet].fCurveBulletY = 0.0f;
				g_aBullet[nCntBullet].nAnimationCounter_Slash = 0;
				g_aBullet[nCntBullet].fAnimationPattern_Slash = 0.0f;
				g_aBullet[nCntBullet].fBulletMoveX_Enemy9 = 0.0f;
				g_aBullet[nCntBullet].fBulletMoveY_Enemy9 = 0.0f;
				g_aBullet[nCntBullet].fBulletMove_Enemy9 = 0.0f;
				g_aBullet[nCntBullet].nApperCount = 0;//敵が出現したらカウントを開始する。
				g_aBullet[nCntBullet].fEnemy5Curve = 0.0f;
				g_aBullet[nCntBullet].nNumber = nNum;
				g_aBullet[nCntBullet].nNumEnemy = nNumEnemy;//どの敵が発射している弾かを判別する変数。
				g_aBullet[nCntBullet].nHormingaim = nAimingEnemy;
				g_aBullet[nCntBullet].HormingFlag = false;
				g_aBullet[nCntBullet].nHormingCount=0;
				g_aBullet[nCntBullet].fSpeed = 0.0f;//バレットのスピードを変える。
				g_aBullet[nCntBullet].fCurve = 300.0f;//バレットのカーブ値を変える。
				g_aBullet[nCntBullet].bCollisionBullet = bCollision;//弾がブロックに衝突するかどうかのフラグ
				g_aBullet[nCntBullet].nAnimationCounter = 0;
				g_aBullet[nCntBullet].nAnimationPattern = 0;
				g_aBullet[nCntBullet].nCntTime = 0;

				break;
			//}
		}
		pVtx += 4;//ポインタを４つ分進める
	}
	g_pVtxBuffBullet->Unlock();
}

//=====================================================
//弾構造体の情報を取得
//=====================================================
Bullet* GetBullet(void)
{
	return &g_aBullet[0];//構造体の先頭アドレスを返す。
}