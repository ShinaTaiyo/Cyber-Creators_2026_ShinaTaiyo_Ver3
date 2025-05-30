//========================================================
//
//１０月１２日：プレイヤーにコマンド技をつけたい[Attack.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#include "main.h"
#include "Attack.h"
#include "input.h"
#include "explosion.h"
#include "enemy.h"
#include "player.h"
#include "health.h"
#include <time.h>
#include "effect.h"
#include "sound.h"
#include "particle.h"
#include "damage.h"
#include "block.h"
#include "bullet.h"


#define NUM_ATTACK (ATTACKTYPE_MAX)//攻撃の種類数
#define BIGAttack_LV1 (40.0f)
#define BIGAttack_LV2 (150.0f)//大きい攻撃LV24
#define AttackSCALE_NOVA (100.0f)

#define AttackSIZE_SCATTERING (30.0f)


//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureAttack[NUM_ATTACK] = {};//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffAttack = NULL;//攻撃の情報

//====================================
//斜めの当たり判定に使う処理
//====================================
D3DXVECTOR2 relative_position[MAX_ATTACK];
D3DXVECTOR2 transform_pos[MAX_ATTACK];


Attack g_aAttack[MAX_ATTACK];
//float g_fXMove;//X座標の移動
//float g_fYMove;//Y座標の移動

float g_fCurveAttack;
float g_fEnemy9Attack;
int g_nRandumAttack2_B;
float g_fRandumResultAttack2_B;//ランダムで攻撃を撃つ方向が決まる。

int g_nDelayAttack;

//int g_nNumEnemy[MAX_ENEMY];//エネミーの数を数える変数
D3DXVECTOR3 g_posAttack;//位置
D3DXVECTOR3 g_moveAttack;//移動量
//D3DXVECTOR3 g_rotPl;//向き

float g_fRandumScatteringAttack_Rot;//ばらまき拡散攻撃の処理
float g_fRandumScatternigAttack_Speed;//ばらまき拡散攻撃のスピード


float g_fXVaimAttack[MAX_ATTACK];
float g_fYVaimAttack[MAX_ATTACK];
float g_VaimAttack[MAX_ATTACK];

//int g_nHitCount;//プレイヤーが被攻撃した回数をカウントする変数
//
//int g_nAnimatioinCounter_Slash[MAX_ATTACK];//アニメーションを次のテクスチャにするカウンター
//float g_fAnimationPattern_Slash[MAX_ATTACK];//アニメーションのテクスチャを管理する変数

//攻撃の初期化処理
void InitAttack(void)
{

	//srand((unsigned int)time(NULL));//ゲームを開始した時間毎に乱数の種を設定

	g_moveAttack = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//移動量を初期化する

	g_fCurveAttack = 0.0f;
	g_fEnemy9Attack = 0.0f;

	g_nDelayAttack = 0;

	g_fRandumScatteringAttack_Rot = 0.0f;
	g_fRandumScatternigAttack_Speed = 0.0f;

	/*g_nHitCount = 0;*/

	//g_nDelayAttack;

	LPDIRECT3DDEVICE9 pDevice;
	int	nCntAttack;


	int nCntEnemy;
	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み（攻撃のテクスチャを読み込む）
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\AttackMotion_Punch.png",
		&g_pTextureAttack[ATTACKTYPE_PUNCH]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\AttackMotion_Slash_000.png",
		&g_pTextureAttack[ATTACKTYPE_SLASH_E]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\AttackMotion_IceExplosion.png",
		&g_pTextureAttack[ATTACKTYPE_ICE_EXPLOSION]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\AttackMotion_Thunder.png",
		&g_pTextureAttack[ATTACKTYPE_THUNDER_E4]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\AttackMotion_Bomu.png",
		&g_pTextureAttack[ATTACKTYPE_BOMU]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\AttackMotion_Needle.png",
		&g_pTextureAttack[ATTACKTYPE_NEEDLE]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\AttackMotion_Claw.png",
		&g_pTextureAttack[ATTACKTYPE_CLAW]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\AttackMotion_VacuumBlade.png",
		&g_pTextureAttack[ATTACKTYPE_VACUUMBLADE]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\AttackMotion_FallingBlock.png",
		&g_pTextureAttack[ATTACKTYPE_FALLINGBLOCK]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\AttackMotion_Beam.png",
		&g_pTextureAttack[ATTACKTYPE_BEAM]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\AttackMotion_IcicleClaw.png",
		&g_pTextureAttack[ATTACKTYPE_ICICLECLAW]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\AttackMotion_Absolute_blizzard.png",
		&g_pTextureAttack[ATTACKTYPE_ABSOLUTE_BLIZZARD]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\AttackMotion_Fire.png",
		&g_pTextureAttack[ATTACKTYPE_FIRE]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\AttackMotion_ShakingSword.png",
		&g_pTextureAttack[ATTACKTYPE_SHAKINGSWORD]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\AttackMotion_FlashBlade.png",
		&g_pTextureAttack[ATTACKTYPE_FLASHBLADE]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\AttackMotion_Thunder_002.png",
		&g_pTextureAttack[ATTACKTYPE_THUNDER_2]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\FinalBeam.png",
		&g_pTextureAttack[ATTACKTYPE_FINALBEAM]);
	//g_posAttack = D3DXVECTOR3(600.0f, 300.0f, 0.0f);//位置を初期化する


	//for (nCntAttack = 0; nCntAttack < MAX_CIRCLEAttack; nCntAttack++)
	//{//円形各散攻撃の初期化
	//	g_fCircleAttack_B[nCntAttack] = 0;
	//}

	//攻撃情報の初期化
	for (nCntAttack = 0; nCntAttack < MAX_ATTACK; nCntAttack++)
	{
		g_aAttack[nCntAttack].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//位置
		g_aAttack[nCntAttack].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//前の位置
		g_aAttack[nCntAttack].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//攻撃の動き
		g_aAttack[nCntAttack].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//向き
		g_aAttack[nCntAttack].fLength = 0.0f;//対角線の長さ
		g_aAttack[nCntAttack].fAngle = 0.0f;//対角線の角度
		g_aAttack[nCntAttack].fWidth = 0.0f;//幅
		g_aAttack[nCntAttack].fHeight = 0.0f;//高さ
		g_aAttack[nCntAttack].nLife = 0;//攻撃時間
		g_aAttack[nCntAttack].bUse = false;//使用していない状態にする
		g_aAttack[nCntAttack].type = ATTACKTYPE_PUNCH;//攻撃の種類
		g_aAttack[nCntAttack].moveType = MOVETYPE_NORMAL;//攻撃の動き方の種類
		g_aAttack[nCntAttack].nTime = 0;//攻撃が始まってからの時間を計測する。
		g_aAttack[nCntAttack].nCntAnim = 0;//アニメーションカウンター
		g_aAttack[nCntAttack].nPatternAnim = 0;//アニメーションパターン
		g_aAttack[nCntAttack].nPatternAnimY = 0;//アニメーションが２段以上の時に弾を移す処理
		g_aAttack[nCntAttack].nHitCount = 0;//一回の攻撃でヒットする回数の制限
		g_aAttack[nCntAttack].nNumEnemy = 0;
		g_aAttack[nCntAttack].bIsLanding = false;//地面についているフラグ
		g_aAttack[nCntAttack].bIsWalling = false;//壁に当たっているか
		g_aAttack[nCntAttack].fGravity = 0.0f;//重力補正値
		g_aAttack[nCntAttack].nColorChange = 0;//色を変える
		g_aAttack[nCntAttack].nCntUseTime = 0;//攻撃を使っている時間をカウントする
		g_aAttack[nCntAttack].fXVaim = 0.0f;
		g_aAttack[nCntAttack].fYVaim = 0.0f;
		g_aAttack[nCntAttack].fVaim = 0.0f;
		g_aAttack[nCntAttack].fEffectiveScale = 0.0f;//攻撃の効果範囲を決める
		g_aAttack[nCntAttack].fRotSpeed = 0.0f;//回転速度
	}

	//for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	//{
	//	g_nNumEnemy[nCntEnemy] = -1;//初期化
	//}
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ATTACK,//４は必要な頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffAttack,
		NULL);

	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffAttack->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntAttack = 0; nCntAttack < MAX_ATTACK; nCntAttack++)
	{
		//対角線の長さを算出する
        g_aAttack[nCntAttack].fLength = sqrtf(g_aAttack[nCntAttack].fWidth * g_aAttack[nCntAttack].fWidth + (g_aAttack[nCntAttack].fHeight * 2.0f) * (g_aAttack[nCntAttack].fHeight * 2.0f)) / 2;
        
        //対角線の角度を算出する
        g_aAttack[nCntAttack].fAngle = atan2f(g_aAttack[nCntAttack].fWidth, (g_aAttack[nCntAttack].fHeight * 2.0f));
        
        //頂点座標の設定
        pVtx[0].pos.y = g_aAttack[nCntAttack].pos.y + cosf(g_aAttack[nCntAttack].rot.z - (D3DX_PI - g_aAttack[nCntAttack].fAngle)) * g_aAttack[nCntAttack].fLength;
        pVtx[0].pos.x = g_aAttack[nCntAttack].pos.x + sinf(g_aAttack[nCntAttack].rot.z - (D3DX_PI - g_aAttack[nCntAttack].fAngle)) * g_aAttack[nCntAttack].fLength;
        pVtx[0].pos.z = 0.0f;
        
        pVtx[1].pos.x = g_aAttack[nCntAttack].pos.x + sinf(g_aAttack[nCntAttack].rot.z + (D3DX_PI - g_aAttack[nCntAttack].fAngle)) * g_aAttack[nCntAttack].fLength;
        pVtx[1].pos.y = g_aAttack[nCntAttack].pos.y + cosf(g_aAttack[nCntAttack].rot.z + (D3DX_PI - g_aAttack[nCntAttack].fAngle)) * g_aAttack[nCntAttack].fLength;
        pVtx[1].pos.z = 0.0f;
        
        pVtx[2].pos.x = g_aAttack[nCntAttack].pos.x + sinf(g_aAttack[nCntAttack].rot.z + -(D3DX_PI * 0.5f)) * g_aAttack[nCntAttack].fWidth / 2;
        pVtx[2].pos.y = g_aAttack[nCntAttack].pos.y + cosf(g_aAttack[nCntAttack].rot.z + -(D3DX_PI * 0.5f)) * g_aAttack[nCntAttack].fWidth / 2;
        pVtx[2].pos.z = 0.0f;
        
        pVtx[3].pos.x = g_aAttack[nCntAttack].pos.x + sinf(g_aAttack[nCntAttack].rot.z + (D3DX_PI * 0.5f)) * g_aAttack[nCntAttack].fWidth / 2;
        pVtx[3].pos.y = g_aAttack[nCntAttack].pos.y + cosf(g_aAttack[nCntAttack].rot.z + (D3DX_PI * 0.5f)) * g_aAttack[nCntAttack].fWidth / 2;
        pVtx[3].pos.z = 0.0f;
        
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
	g_pVtxBuffAttack->Unlock();
}

//攻撃の終了処理
void UninitAttack(void)
{
	//テクスチャの破棄
	int nCnt;
	for (nCnt = 0; nCnt < NUM_ATTACK; nCnt++)
	{
		if (g_pTextureAttack[nCnt] != NULL)
		{
			g_pTextureAttack[nCnt]->Release();
			g_pTextureAttack[nCnt] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffAttack != NULL)
	{
		g_pVtxBuffAttack->Release();
		g_pVtxBuffAttack = NULL;
	}
}

//攻撃の更新処理
void UpdateAttack(void)
{
	int nCntAttack3;

	//g_nRandumAttack2_B = rand() % 100 + 1;//円形各散攻撃の発射方向の調整用

	//g_fRandumResultAttack2_B = 0.01f * g_nRandumAttack2_B;//円形拡散攻撃の方向調整結果
	////g_fRandumResultAttack2_B = 0.01f;//円形拡散攻撃の方向調整結果


	//g_fRandumScatteringAttack_Rot = float((rand)() % 629 - 314) / 100.0f;//攻撃を発射する方向を決める



	g_nDelayAttack++;//攻撃判定を遅らせるカウント

	Player* pPlayer = GetPlayer();
	//g_aAttack.fCurveAttackX += 0.01f;
	//g_fRotAttackY += 0.02f;
	//
	//LPDIRECT3DDEVICE9 pDevice;
	int	nCntAttack;
	//g_fEnemy9Attack += 0.0001f;

	//Health* pHealth = GetHealth();

	Enemy* pEnemy = GetEnemy();

	VERTEX_2D* pVtx;


	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffAttack->Lock(0, 0, (void**)&pVtx, 0);



	for (nCntAttack = 0; nCntAttack < MAX_ATTACK; nCntAttack++)
	{
		if (g_aAttack[nCntAttack].bUse == true)
		{//攻撃が使用されている
			int nCntEnemy;
			g_aAttack[nCntAttack].nLife -= 1;//表示する時間（寿命）を減らし続ける


			//=====================================
			//中心点決定処理
			//=====================================

		//対角線の長さを算出する
			g_aAttack[nCntAttack].fLength = sqrtf(g_aAttack[nCntAttack].fWidth * g_aAttack[nCntAttack].fWidth + (g_aAttack[nCntAttack].fHeight * 2.0f) * (g_aAttack[nCntAttack].fHeight * 2.0f)) / 2;

			//対角線の角度を算出する
			g_aAttack[nCntAttack].fAngle = atan2f(g_aAttack[nCntAttack].fWidth, (g_aAttack[nCntAttack].fHeight * 2.0f));

			//頂点座標の設定
			pVtx[0].pos.y = g_aAttack[nCntAttack].pos.y + cosf(g_aAttack[nCntAttack].rot.z - (D3DX_PI - g_aAttack[nCntAttack].fAngle)) * g_aAttack[nCntAttack].fLength;
			pVtx[0].pos.x = g_aAttack[nCntAttack].pos.x + sinf(g_aAttack[nCntAttack].rot.z - (D3DX_PI - g_aAttack[nCntAttack].fAngle)) * g_aAttack[nCntAttack].fLength;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aAttack[nCntAttack].pos.x + sinf(g_aAttack[nCntAttack].rot.z + (D3DX_PI - g_aAttack[nCntAttack].fAngle)) * g_aAttack[nCntAttack].fLength;
			pVtx[1].pos.y = g_aAttack[nCntAttack].pos.y + cosf(g_aAttack[nCntAttack].rot.z + (D3DX_PI - g_aAttack[nCntAttack].fAngle)) * g_aAttack[nCntAttack].fLength;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aAttack[nCntAttack].pos.x + sinf(g_aAttack[nCntAttack].rot.z - (D3DX_PI * 0.5f)) * g_aAttack[nCntAttack].fWidth / 2;
			pVtx[2].pos.y = g_aAttack[nCntAttack].pos.y + cosf(g_aAttack[nCntAttack].rot.z - (D3DX_PI * 0.5f)) * g_aAttack[nCntAttack].fWidth / 2;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aAttack[nCntAttack].pos.x + sinf(g_aAttack[nCntAttack].rot.z + (D3DX_PI * 0.5f)) * g_aAttack[nCntAttack].fWidth / 2;
			pVtx[3].pos.y = g_aAttack[nCntAttack].pos.y + cosf(g_aAttack[nCntAttack].rot.z + (D3DX_PI * 0.5f)) * g_aAttack[nCntAttack].fWidth / 2;
			pVtx[3].pos.z = 0.0f;


			//=========================================
			//斜めの当たり判定を使う場合の処理
			// ========================================


			// 矩形の中心を原点とした相対座標を作成する
			relative_position[nCntAttack] = D3DXVECTOR2(pVtx[0].pos.x - g_aAttack[nCntAttack].pos.x, pVtx[0].pos.y - g_aAttack[nCntAttack].pos.y);

			// 相対座標に対して矩形の回転を打ち消す逆行列を掛ける
			transform_pos[nCntAttack] = D3DXVECTOR2(
				cosf(g_aAttack[nCntAttack].rot.z) * relative_position[nCntAttack].x + sinf(g_aAttack[nCntAttack].rot.z) * relative_position[nCntAttack].y,
				-sinf(g_aAttack[nCntAttack].rot.z) * relative_position[nCntAttack].x + cosf(g_aAttack[nCntAttack].rot.z) * relative_position[nCntAttack].y
			);



			//===============================================================================================================================================================

			//=================================================
			//角にぶつかったら攻撃を消す
			//=================================================
			//if ((g_aAttack[nCntAttack].pos.x >= SCREEN_WIDTH) || (g_aAttack[nCntAttack].pos.x <= 0) ||
			//	(g_aAttack[nCntAttack].pos.y >= SCREEN_HEIGHT) || (g_aAttack[nCntAttack].pos.y <= 0))
			//{
			//	g_aAttack[nCntAttack].bUse = false;//使用していない状態にする。
			//}



			//==============================================================================================================================


			//=============================================================
			//主な攻撃処理
			//=============================================================
			if (g_aAttack[nCntAttack].type == ATTACKTYPE_PUNCH)
			{//アビリティ：ノーマル、技：パンチ

				Enemy* pEnemy = GetEnemy();

				g_aAttack[nCntAttack].nCntAnim++;//アニメーションカウンターを進める

				if (g_aAttack[nCntAttack].nCntAnim % 5 == 0)
				{
					g_aAttack[nCntAttack].nPatternAnim++;

				}
				if (g_aAttack[nCntAttack].nPatternAnim >= 4)
				{
					g_aAttack[nCntAttack].nPatternAnim = 0;
				}

				//テクスチャ座標の設定
				if (pPlayer->bDirection == false)
				{//右向いていたら
					pVtx[0].tex = D3DXVECTOR2(0.0f + 0.25f * g_aAttack[nCntAttack].nPatternAnim, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(0.25f + 0.25f * g_aAttack[nCntAttack].nPatternAnim, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(0.0f + 0.25f * g_aAttack[nCntAttack].nPatternAnim, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(0.25f + 0.25f * g_aAttack[nCntAttack].nPatternAnim, 1.0f);

					g_aAttack[nCntAttack].pos.y = pPlayer->pos.y;
					g_aAttack[nCntAttack].pos.x = pPlayer->pos.x + PLAYER_WIDTH + 30.0f;
				}
				else if (pPlayer->bDirection == true)
				{//左向いていたら
					pVtx[1].tex = D3DXVECTOR2(0.0f + 0.25f * g_aAttack[nCntAttack].nPatternAnim, 0.0f);
					pVtx[0].tex = D3DXVECTOR2(0.25f + 0.25f * g_aAttack[nCntAttack].nPatternAnim, 0.0f);
					pVtx[3].tex = D3DXVECTOR2(0.0f + 0.25f * g_aAttack[nCntAttack].nPatternAnim, 1.0f);
					pVtx[2].tex = D3DXVECTOR2(0.25f + 0.25f * g_aAttack[nCntAttack].nPatternAnim, 1.0f);

					g_aAttack[nCntAttack].pos.y = pPlayer->pos.y;
					g_aAttack[nCntAttack].pos.x = pPlayer->pos.x - PLAYER_WIDTH - 30.0f;
				}


				for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				{
					if (pEnemy->bUse == true)
					{//敵が使用されている
						if (g_aAttack[nCntAttack].pos.x >= pEnemy->pos.x - 50.0f
							&& g_aAttack[nCntAttack].pos.x <= pEnemy->pos.x + 50.0f
							&& g_aAttack[nCntAttack].pos.y - g_aAttack[nCntAttack].fHeight / 2 >= pEnemy->pos.y - pEnemy->fHeight - 50.0f
							&& g_aAttack[nCntAttack].pos.y - g_aAttack[nCntAttack].fHeight / 2 <= pEnemy->pos.y - pEnemy->fHeight + 50.0f)
						{//敵と弾が当たった（敵の中心座標）（攻撃の中心座標）
							if (g_aAttack[nCntAttack].nHitCount <= 0)
							{//ヒット数が一回以下なら
								HitEnemy(nCntEnemy, 15);
							}

							g_aAttack[nCntAttack].nHitCount++;//ヒット数のカウントを１増やす
						}
					}

				}
			}
			else if (g_aAttack[nCntAttack].type == ATTACKTYPE_SLASH_E)
			{//エネミー２の技「スラッシュ」

				Enemy* pEnemy = GetEnemy();
				Player* pPlayer = GetPlayer();
				g_aAttack[nCntAttack].nCntAnim++;//アニメーションカウンターを進める

				g_aAttack[nCntAttack].nCntUseTime++;//攻撃を使っている時間をカウントする
				if (g_aAttack[nCntAttack].nCntUseTime == 1)
				{
					PlaySound(SOUND_LABEL_SE_MINISLASH);
				}


				if (g_aAttack[nCntAttack].nCntAnim % 5 == 0)
				{
					g_aAttack[nCntAttack].nPatternAnim++;

				}
				if (g_aAttack[nCntAttack].nPatternAnim >= 5)
				{
					g_aAttack[nCntAttack].nPatternAnim = 0;
				}

				//テクスチャ座標の設定



				for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				{
					if (pEnemy->bUse == true)
					{//敵が使用されている

						if (nCntEnemy == g_aAttack[nCntAttack].nNumEnemy)
						{
							if (pEnemy->bDirection == false)
							{//右向いている
								pVtx[0].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.0f);
								pVtx[1].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.0f);
								pVtx[2].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 1.0f);
								pVtx[3].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 1.0f);

								//攻撃の座標を敵の少し右側にずらす
								g_aAttack[nCntAttack].pos.y = pEnemy->pos.y;
								g_aAttack[nCntAttack].pos.x = pEnemy->pos.x + pEnemy->fWidth + 20.0f;
							}
							else if (pEnemy->bDirection == true)
							{//左向いている
								pVtx[1].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.0f);
								pVtx[0].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.0f);
								pVtx[3].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 1.0f);
								pVtx[2].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 1.0f);

								//攻撃の座標を敵の少し左側にずらす
								g_aAttack[nCntAttack].pos.y = pEnemy->pos.y;
								g_aAttack[nCntAttack].pos.x = pEnemy->pos.x - pEnemy->fWidth - 20.0f;
							}
						}

					}

				}


				if (pPlayer->pos.x >= g_aAttack[nCntAttack].pos.x - g_aAttack[nCntAttack].fWidth / 2 &&
					pPlayer->pos.x <= g_aAttack[nCntAttack].pos.x + g_aAttack[nCntAttack].fWidth / 2 &&
					pPlayer->pos.y - pPlayer->fHeight / 2 >= g_aAttack[nCntAttack].pos.y - g_aAttack[nCntAttack].fHeight - pPlayer->fHeight / 2 &&
					pPlayer->pos.y - pPlayer->fHeight / 2 <= g_aAttack[nCntAttack].pos.y + pPlayer->fHeight / 2)
				{//プレイヤーに攻撃が当たった(プレイヤーの位置がどこにあるかで計算）
					if (g_nDelayAttack % 5 == 0)
					{
						if (g_aAttack[nCntAttack].nHitCount <= 0)
						{//ヒット数が一回以下なら
							DamageHealth(8);
						}
					}
					g_aAttack[nCntAttack].nHitCount++;//ヒット数のカウントを１増やす
				}
			}
			else if (g_aAttack[nCntAttack].type == ATTACKTYPE_ICE_EXPLOSION)
			{//エネミー３の技「氷爆発」

			g_aAttack[nCntAttack].nCntUseTime++;//攻撃を使っている時間をカウントする
			if (g_aAttack[nCntAttack].nCntUseTime == 1)
			{
				PlaySound(SOUND_LABEL_SE_ICE_EXPLOSION);
			}

				Enemy* pEnemy = GetEnemy();
				Player* pPlayer = GetPlayer();
				g_aAttack[nCntAttack].nCntAnim++;//アニメーションカウンターを進める

				if (g_aAttack[nCntAttack].nCntAnim % 2 == 0)
				{
					g_aAttack[nCntAttack].nPatternAnim++;

				}
				if (g_aAttack[nCntAttack].nPatternAnim >= 8)
				{
					g_aAttack[nCntAttack].nPatternAnim = 0;
					g_aAttack[nCntAttack].nPatternAnimY++;//アニメーションの段を次の段へ移動
				}
				if (g_aAttack[nCntAttack].nPatternAnim >= 6 && g_aAttack[nCntAttack].nPatternAnimY >= 1)
				{//アニメーションを最初の位置へ
					g_aAttack[nCntAttack].nPatternAnim = 0;
					g_aAttack[nCntAttack].nPatternAnimY = 0;
				}

				//テクスチャ座標の設定




				pVtx[0].tex = D3DXVECTOR2(0.0f + 0.125f * g_aAttack[nCntAttack].nPatternAnim, 0.0f + 0.5f * g_aAttack[nCntAttack].nPatternAnimY);
				pVtx[1].tex = D3DXVECTOR2(0.125f + 0.125f * g_aAttack[nCntAttack].nPatternAnim, 0.0f + 0.5f * g_aAttack[nCntAttack].nPatternAnimY);
				pVtx[2].tex = D3DXVECTOR2(0.0f + 0.125f * g_aAttack[nCntAttack].nPatternAnim, 0.5f + 0.5f * g_aAttack[nCntAttack].nPatternAnimY);
				pVtx[3].tex = D3DXVECTOR2(0.125f + 0.125f * g_aAttack[nCntAttack].nPatternAnim, 0.5f + 0.5f * g_aAttack[nCntAttack].nPatternAnimY);




				if (pPlayer->pos.x >= g_aAttack[nCntAttack].pos.x - g_aAttack[nCntAttack].fWidth / 2 &&
					pPlayer->pos.x <= g_aAttack[nCntAttack].pos.x + g_aAttack[nCntAttack].fWidth / 2 &&
					pPlayer->pos.y - pPlayer->fHeight / 2 >= g_aAttack[nCntAttack].pos.y - g_aAttack[nCntAttack].fHeight &&
					pPlayer->pos.y - pPlayer->fHeight / 2 <= g_aAttack[nCntAttack].pos.y)
				{//プレイヤーに攻撃が当たった(プレイヤーの中心がどの位置にあるかで計算）
					if (g_nDelayAttack % 1 == 0)
					{
						if (g_aAttack[nCntAttack].nHitCount <= 0)
						{//ヒット数が一回以下なら
							DamageHealth(18);

						}
					}
					g_aAttack[nCntAttack].nHitCount++;//ヒット数のカウントを１増やす
				}
			}
			else if (g_aAttack[nCntAttack].type == ATTACKTYPE_THUNDER_E4)
			{//エネミー４の技「サンダー」
			g_aAttack[nCntAttack].nCntUseTime++;//攻撃を使っている時間をカウントする
			if (g_aAttack[nCntAttack].nCntUseTime == 1)
			{
				PlaySound(SOUND_LABEL_SE_THUNDER);
			}

				Enemy* pEnemy = GetEnemy();
				Player* pPlayer = GetPlayer();
				g_aAttack[nCntAttack].nCntAnim++;//アニメーションカウンターを進める

				if (g_aAttack[nCntAttack].nCntAnim % 5 == 0)
				{
					g_aAttack[nCntAttack].nPatternAnim++;

				}
				if (g_aAttack[nCntAttack].nPatternAnim >= 5)
				{
					g_aAttack[nCntAttack].nPatternAnim = 0;
				}

				//テクスチャ座標の設定



				for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				{
					if (pEnemy->bUse == true)
					{//敵が使用されている

						if (nCntEnemy == g_aAttack[nCntAttack].nNumEnemy)
						{
							if (pEnemy->bDirection == false)
							{//右向いている
								pVtx[0].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.0f);
								pVtx[1].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.0f);
								pVtx[2].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 1.0f);
								pVtx[3].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 1.0f);

								////攻撃の座標は攻撃の下端
								//g_aAttack[nCntAttack].pos.y = pEnemy->pos.y + g_aAttack[nCntAttack].fHeight / 2;
								//g_aAttack[nCntAttack].pos.x = pEnemy->pos.x;
							}
							else if (pEnemy->bDirection == true)
							{//左向いている
								pVtx[1].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.0f);
								pVtx[0].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.0f);
								pVtx[3].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 1.0f);
								pVtx[2].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 1.0f);

								//攻撃の座標は攻撃の下端
							/*	g_aAttack[nCntAttack].pos.y = pEnemy->pos.y +
									g_aAttack[nCntAttack].fHeight / 2;
								g_aAttack[nCntAttack].pos.x = pEnemy->pos.x;*/
							}
						}

					}

				}


				if (pPlayer->pos.x >= g_aAttack[nCntAttack].pos.x - g_aAttack[nCntAttack].fWidth / 2 - 10.0f &&
					pPlayer->pos.x <= g_aAttack[nCntAttack].pos.x + g_aAttack[nCntAttack].fWidth / 2 + 10.0f &&
					pPlayer->pos.y - pPlayer->fHeight / 2 >= g_aAttack[nCntAttack].pos.y - g_aAttack[nCntAttack].fHeight &&
					pPlayer->pos.y - pPlayer->fHeight / 2 <= g_aAttack[nCntAttack].pos.y + g_aAttack[nCntAttack].fHeight)
				{//プレイヤーに攻撃が当たった(プレイヤーの中心がどの位置にあるかで計算）
					if (g_nDelayAttack % 1 == 0)
					{
						if (g_aAttack[nCntAttack].nHitCount <= 0)
						{//ヒット数が一回以下なら
							DamageHealth(12);

						}
					}
					g_aAttack[nCntAttack].nHitCount++;//ヒット数のカウントを１増やす
				}

			}

			else if (g_aAttack[nCntAttack].type == ATTACKTYPE_BOMU)
			{//エネミー６の技「ボム」
				Enemy* pEnemy = GetEnemy();
				Player* pPlayer = GetPlayer();
				g_aAttack[nCntAttack].nCntAnim++;//アニメーションカウンターを進める
				g_aAttack[nCntAttack].nColorChange++;//色を変え続ける
				if (g_aAttack[nCntAttack].nCntAnim % 3 == 0)
				{
					g_aAttack[nCntAttack].nPatternAnim++;

				}
				if (g_aAttack[nCntAttack].nPatternAnim >= 5)
				{
					g_aAttack[nCntAttack].nPatternAnim = 0;
				}

				//テクスチャ座標の設定

				//頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f - (0.01f * g_aAttack[nCntAttack].nColorChange), 1.0f - (0.01f * g_aAttack[nCntAttack].nColorChange), 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f - (0.01f * g_aAttack[nCntAttack].nColorChange), 1.0f - (0.01f * g_aAttack[nCntAttack].nColorChange), 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f - (0.01f * g_aAttack[nCntAttack].nColorChange), 1.0f - (0.01f * g_aAttack[nCntAttack].nColorChange), 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f - (0.01f * g_aAttack[nCntAttack].nColorChange), 1.0f - (0.01f * g_aAttack[nCntAttack].nColorChange), 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。

				pVtx[0].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 1.0f);


				if (g_aAttack[nCntAttack].nLife <= 0)
				{//ライフが０以下になったら

					SetExplosion(D3DXVECTOR3(g_aAttack[nCntAttack].pos.x, g_aAttack[nCntAttack].pos.y - g_aAttack[nCntAttack].fHeight / 2.0f, 0.0f),
						D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
						EXPLOSIONTYPE_E6, 100.0f, 100.0f);

				}


			}
			else if (g_aAttack[nCntAttack].type == ATTACKTYPE_NEEDLE)
			{//エネミー7の技「とげ攻撃」


			g_aAttack[nCntAttack].nCntUseTime++;//攻撃を使っている時間をカウントする
			if (g_aAttack[nCntAttack].nCntUseTime == 1)
			{
				PlaySound(SOUND_LABEL_SE_STAB);
			}

				Enemy* pEnemy = GetEnemy();
				Player* pPlayer = GetPlayer();
				g_aAttack[nCntAttack].nCntAnim++;//アニメーションカウンターを進める

				if (g_aAttack[nCntAttack].nCntAnim % 4 == 0)
				{
					g_aAttack[nCntAttack].nPatternAnim++;

				}
				if (g_aAttack[nCntAttack].nPatternAnim >= 5)
				{
					g_aAttack[nCntAttack].nPatternAnim = 0;
					g_aAttack[nCntAttack].nPatternAnimY++;//アニメーションの段を次の段へ移動


					if (g_aAttack[nCntAttack].nPatternAnimY >= 2)
					{//アニメーションを最初の位置へ
						g_aAttack[nCntAttack].nPatternAnim = 0;
						g_aAttack[nCntAttack].nPatternAnimY = 0;
						g_aAttack[nCntAttack].bUse = false;//使用していない状態にする
					}
				}




				//テクスチャ座標の設定



				for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				{
					if (pEnemy->bUse == true)
					{//敵が使用されている

						if (nCntEnemy == g_aAttack[nCntAttack].nNumEnemy)
						{
							if (pEnemy->bDirection == false)
							{//右向いている
								pVtx[0].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.0f + 0.5f * g_aAttack[nCntAttack].nPatternAnimY);
								pVtx[1].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.0f + 0.5f * g_aAttack[nCntAttack].nPatternAnimY);
								pVtx[2].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.5f + 0.5f * g_aAttack[nCntAttack].nPatternAnimY);
								pVtx[3].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.5f + 0.5f * g_aAttack[nCntAttack].nPatternAnimY);
							}
							else if (pEnemy->bDirection == true)
							{//左向いている
								pVtx[1].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.0f + 0.5f * g_aAttack[nCntAttack].nPatternAnimY);
								pVtx[0].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.0f + 0.5f * g_aAttack[nCntAttack].nPatternAnimY);
								pVtx[3].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.5f + 0.5f * g_aAttack[nCntAttack].nPatternAnimY);
								pVtx[2].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.5f + 0.5f * g_aAttack[nCntAttack].nPatternAnimY);
							}


						}

					}

				}


				if (pPlayer->pos.x >= g_aAttack[nCntAttack].pos.x - g_aAttack[nCntAttack].fWidth / 2 + 20.0f &&
					pPlayer->pos.x <= g_aAttack[nCntAttack].pos.x + g_aAttack[nCntAttack].fWidth / 2 - 20.0f &&
					pPlayer->pos.y - pPlayer->fHeight / 2 >= g_aAttack[nCntAttack].pos.y - g_aAttack[nCntAttack].fHeight + 20.0f &&
					pPlayer->pos.y - pPlayer->fHeight / 2 <= g_aAttack[nCntAttack].pos.y - 20.0f)
				{//プレイヤーに攻撃が当たった(プレイヤーの中心がどの位置にあるかで計算）
					if (g_nDelayAttack % 1 == 0)
					{
						if (g_aAttack[nCntAttack].nHitCount <= 0)
						{//ヒット数が一回以下なら
							DamageHealth(10);

						}
					}
					g_aAttack[nCntAttack].nHitCount++;//ヒット数のカウントを１増やす
				}


			}
			else if (g_aAttack[nCntAttack].type == ATTACKTYPE_VACUUMBLADE)
			{//エネミー8の技「真空刃」

			g_aAttack[nCntAttack].nCntUseTime++;//攻撃を使っている時間をカウントする
			if (g_aAttack[nCntAttack].nCntUseTime == 1)
			{
				PlaySound(SOUND_LABEL_SE_FLASHBLADE);
			}

				g_aAttack[nCntAttack].nCntUseTime++;//攻撃を使っている時間をカウントする
				Enemy* pEnemy = GetEnemy();
				Player* pPlayer = GetPlayer();
				g_aAttack[nCntAttack].nCntAnim++;//アニメーションカウンターを進める

				if (g_aAttack[nCntAttack].nCntAnim % 5 == 0)
				{
					g_aAttack[nCntAttack].nPatternAnim++;

				}
				if (g_aAttack[nCntAttack].nPatternAnim >= 5)
				{
					g_aAttack[nCntAttack].nPatternAnim = 0;
				}





				//テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 1.0f);



				for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				{
					if (pEnemy->bUse == true)
					{//敵が使用されている

						if (nCntEnemy == g_aAttack[nCntAttack].nNumEnemy)
						{
							if (pEnemy->bDirection == false && g_aAttack[nCntAttack].nCntUseTime <= 4)
							{//右向いている

								pEnemy->move.x = 30.0f;
							}
							else if (pEnemy->bDirection == true && g_aAttack[nCntAttack].nCntUseTime <= 4)
							{//左向いている

								pEnemy->move.x = -30.0f;
							}



						}

					}

				}


				if (pPlayer->pos.x >= g_aAttack[nCntAttack].pos.x - g_aAttack[nCntAttack].fWidth / 2 - 10.0f &&
					pPlayer->pos.x <= g_aAttack[nCntAttack].pos.x + g_aAttack[nCntAttack].fWidth / 2 + 10.0f &&
					pPlayer->pos.y - pPlayer->fHeight / 2 >= g_aAttack[nCntAttack].pos.y - g_aAttack[nCntAttack].fHeight + 10.0f &&
					pPlayer->pos.y - pPlayer->fHeight / 2 <= g_aAttack[nCntAttack].pos.y - 10.0f)
				{//プレイヤーに攻撃が当たった(プレイヤーの中心がどの位置にあるかで計算）
					if (g_nDelayAttack % 1 == 0)
					{
						if (g_aAttack[nCntAttack].nHitCount <= 0)
						{//ヒット数が一回以下なら
							DamageHealth(25);

						}
					}
					g_aAttack[nCntAttack].nHitCount++;//ヒット数のカウントを１増やす
				}
			}
			else if (g_aAttack[nCntAttack].type == ATTACKTYPE_CLAW)
			{//エネミー8の技「爪攻撃」
				int nCntSaveEnemy = 0;

				g_aAttack[nCntAttack].nCntUseTime++;//攻撃を使っている時間をカウントする
				if (g_aAttack[nCntAttack].nCntUseTime == 1)
				{
					PlaySound(SOUND_LABEL_SE_CLAW);
				}


				Enemy* pEnemy = GetEnemy();
				Player* pPlayer = GetPlayer();
				g_aAttack[nCntAttack].nCntAnim++;//アニメーションカウンターを進める

				if (g_aAttack[nCntAttack].nCntAnim % 3 == 0)
				{
					g_aAttack[nCntAttack].nPatternAnim++;

				}
				if (g_aAttack[nCntAttack].nPatternAnim >= 5)
				{
					g_aAttack[nCntAttack].nPatternAnim = 0;
				}



				//テクスチャ座標の設定



				for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				{
					if (pEnemy->bUse == true)
					{//敵が使用されている

						if (nCntEnemy == g_aAttack[nCntAttack].nNumEnemy)
						{
							nCntSaveEnemy = nCntEnemy;
							if (pEnemy->bDirection == false)
							{//右向いている
								pVtx[0].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.0f);
								pVtx[1].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.0f);
								pVtx[2].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 1.0f);
								pVtx[3].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 1.0f);

								//攻撃の座標は敵の下端の右側
								g_aAttack[nCntAttack].pos.y = pEnemy->pos.y;
								g_aAttack[nCntAttack].pos.x = pEnemy->pos.x + pEnemy->fWidth + 20.0f;

								pEnemy->move.x = 3.0f;
							}
							else if (pEnemy->bDirection == true)
							{//左向いている
								pVtx[1].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.0f);
								pVtx[0].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.0f);
								pVtx[3].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 1.0f);
								pVtx[2].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 1.0f);
								//攻撃の座標は敵の下端の左側
								g_aAttack[nCntAttack].pos.y = pEnemy->pos.y;
								g_aAttack[nCntAttack].pos.x = pEnemy->pos.x - pEnemy->fWidth - 20.0f;

								pEnemy->move.x = -3.0f;
							}

							if (g_aAttack[nCntAttack].nLife == 1)
							{
								g_aAttack[nCntAttack].bUse == false;
								g_fXVaimAttack[nCntAttack] = pPlayer->pos.x - pPlayer->pos.x;//X座標へのベクトル
								g_fYVaimAttack[nCntAttack] = pPlayer->pos.y - pPlayer->pos.y;//Y座標へのベクトル
								g_VaimAttack[nCntAttack] = atan2(g_fXVaimAttack[nCntAttack], g_fYVaimAttack[nCntAttack]);//弾を撃つ方向の決定

								float fXVaim;
								float fYVaim;
								float fVaim;

								fXVaim = pPlayer->pos.x - pEnemy->pos.x;//X座標へのベクトル
								fYVaim = pPlayer->pos.y - pEnemy->pos.y;//Y座標へのベクトル
								fVaim = atan2(fXVaim, fYVaim);//攻撃を撃つ方向の決定




								SetAttack(D3DXVECTOR3(pEnemy->pos.x, pEnemy->pos.y - pEnemy->fHeight / 2, 0.0f),
									D3DXVECTOR3(sinf(fVaim) * 8.0f, cosf(fVaim) * 8.0f, 0.0f),
									ATTACKTYPE_VACUUMBLADE, -1, nCntSaveEnemy, -1, MOVETYPE_NORMAL,
									120.0f, 120.0f, 300, 0.0f,-1);

							}
						}

					}

				}


				if (pPlayer->pos.x >= g_aAttack[nCntAttack].pos.x - g_aAttack[nCntAttack].fWidth / 2 - 10.0f &&
					pPlayer->pos.x <= g_aAttack[nCntAttack].pos.x + g_aAttack[nCntAttack].fWidth / 2 + 10.0f &&
					pPlayer->pos.y - pPlayer->fHeight / 2 >= g_aAttack[nCntAttack].pos.y - g_aAttack[nCntAttack].fHeight + 10.0f &&
					pPlayer->pos.y - pPlayer->fHeight / 2 <= g_aAttack[nCntAttack].pos.y - 10.0f)
				{//プレイヤーに攻撃が当たった(プレイヤーの中心がどの位置にあるかで計算）
					if (g_nDelayAttack % 1 == 0)
					{
						if (g_aAttack[nCntAttack].nHitCount <= 0)
						{//ヒット数が一回以下なら
							DamageHealth(8);

						}
					}
					g_aAttack[nCntAttack].nHitCount++;//ヒット数のカウントを１増やす
				}

				if (g_aAttack[nCntAttack].nLife < 1)
				{
					g_aAttack[nCntAttack].bUse = false;
				}
			}
			else if (g_aAttack[nCntAttack].type == ATTACKTYPE_FALLINGBLOCK)
			{//エネミー9の技「ブロック落とし攻撃」
				int nCntSaveEnemy = 0;
				g_aAttack[nCntAttack].nCntUseTime++;
				if (g_aAttack[nCntAttack].nCntUseTime == 1)
				{
					PlaySound(SOUND_LABEL_SE_FALLINGBLOCK);
				}

				Enemy* pEnemy = GetEnemy();
				Player* pPlayer = GetPlayer();
				g_aAttack[nCntAttack].nCntAnim++;//アニメーションカウンターを進める

				if (g_aAttack[nCntAttack].nCntAnim % 3 == 0)
				{
					g_aAttack[nCntAttack].nPatternAnim++;

				}
				if (g_aAttack[nCntAttack].nPatternAnim >= 5)
				{
					g_aAttack[nCntAttack].nPatternAnim = 0;
				}



				//テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 1.0f);




				if (pPlayer->pos.x >= g_aAttack[nCntAttack].pos.x - g_aAttack[nCntAttack].fWidth / 2 &&
					pPlayer->pos.x <= g_aAttack[nCntAttack].pos.x + g_aAttack[nCntAttack].fWidth / 2 &&
					pPlayer->pos.y - pPlayer->fHeight / 2 >= g_aAttack[nCntAttack].pos.y - g_aAttack[nCntAttack].fHeight &&
					pPlayer->pos.y - pPlayer->fHeight / 2 <= g_aAttack[nCntAttack].pos.y)
				{//プレイヤーに攻撃が当たった(プレイヤーの中心がどの位置にあるかで計算）
					if (g_nDelayAttack % 1 == 0)
					{
						if (g_aAttack[nCntAttack].nHitCount <= 0)
						{//ヒット数が一回以下なら
							DamageHealth(15);

						}
					}
					g_aAttack[nCntAttack].nHitCount++;//ヒット数のカウントを１増やす
				}
			}
			else if (g_aAttack[nCntAttack].type == ATTACKTYPE_BEAM ||
			g_aAttack[nCntAttack].type == ATTACKTYPE_FLASHBLADE ||
			g_aAttack[nCntAttack].type == ATTACKTYPE_THUNDER_2 ||
			g_aAttack[nCntAttack].type == ATTACKTYPE_FINALBEAM)
			{//エネミー１０の技「ビーム攻撃」
			g_aAttack[nCntAttack].nCntUseTime++;//攻撃を使っている時間をカウントする
				Enemy* pEnemy = GetEnemy();
				Player* pPlayer = GetPlayer();
				float fData = 0.0f;
				if (g_aAttack[nCntAttack].type == ATTACKTYPE_BEAM)
				{
					fData = 0.04f;
					if (g_aAttack[nCntAttack].nCntUseTime == 1)
					{
						PlaySound(SOUND_LABEL_SE_BEAM);
					}
				}
				else if (g_aAttack[nCntAttack].type == ATTACKTYPE_FLASHBLADE)
				{
					fData = 0.1f;
					if (g_aAttack[nCntAttack].nCntUseTime == 1)
					{
						PlaySound(SOUND_LABEL_SE_SLASH_000);
					}
				}
				else if (g_aAttack[nCntAttack].type == ATTACKTYPE_THUNDER_2)
				{
					if (g_aAttack[nCntAttack].nCntUseTime == 1)
					{
						PlaySound(SOUND_LABEL_SE_THUNDER2);
					}
					fData = 0.07f;
				}
				else if (g_aAttack[nCntAttack].type == ATTACKTYPE_FINALBEAM)
				{
					fData = 0.05f;
					if (g_aAttack[nCntAttack].nCntUseTime == 1)
					{
						PlaySound(SOUND_LABEL_SE_BEAM);
					}
				}

				g_aAttack[nCntAttack].nCntAnim++;//アニメーションカウンターを進める

				if (g_aAttack[nCntAttack].nCntAnim % 1 == 0)
				{
					g_aAttack[nCntAttack].nPatternAnim++;

				}
				if (g_aAttack[nCntAttack].nPatternAnim >= 5)
				{
					g_aAttack[nCntAttack].nPatternAnim = 0;
					g_aAttack[nCntAttack].nPatternAnimY++;//アニメーションの段を次の段へ移動


					if (g_aAttack[nCntAttack].nPatternAnimY >= 2)
					{//アニメーションを最初の位置へ
						g_aAttack[nCntAttack].nPatternAnim = 0;
						g_aAttack[nCntAttack].nPatternAnimY = 0;
						//g_aAttack[nCntAttack].bUse = false;//使用していない状態にする
					}
				}

				//テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.0f + 0.5f * g_aAttack[nCntAttack].nPatternAnimY);
				pVtx[1].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.0f + 0.5f * g_aAttack[nCntAttack].nPatternAnimY);
				pVtx[2].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.5f + 0.5f * g_aAttack[nCntAttack].nPatternAnimY);
				pVtx[3].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.5f + 0.5f * g_aAttack[nCntAttack].nPatternAnimY);



				g_aAttack[nCntAttack].rot.z += g_aAttack[nCntAttack].fRotSpeed;


				if (g_aAttack[nCntAttack].rot.z > D3DX_PI)
				{
					g_aAttack[nCntAttack].rot.z -= D3DX_PI * 2.0f;
				}
				else if (g_aAttack[nCntAttack].rot.z < -D3DX_PI)
				{
					g_aAttack[nCntAttack].rot.z += D3DX_PI * 2.0f;
				}
				//========================================
				//攻撃方向へのベクトル
				//========================================

			//============
			//前の処理
			//============
			/*	if (pPlayer->pos.x < g_aAttack[nCntAttack].pos.x + g_aAttack[nCntAttack].fWidth / 2 &&
					pPlayer->pos.x > g_aAttack[nCntAttack].pos.x - g_aAttack[nCntAttack].fWidth / 2 &&
					pPlayer->pos.y - pPlayer->fHeight / 2 >= g_aAttack[nCntAttack].pos.y - g_aAttack[nCntAttack].fHeight &&
					pPlayer->pos.y - pPlayer->fHeight / 2 <= g_aAttack[nCntAttack].pos.y)*/
			//========================================================================================
				//========================================
				//攻撃からプレイヤーへのベクトル
				//========================================
	            g_aAttack[nCntAttack].fXVaim = pPlayer->pos.x - g_aAttack[nCntAttack].pos.x;//X座標へのベクトル
				g_aAttack[nCntAttack].fYVaim = pPlayer->pos.y - g_aAttack[nCntAttack].pos.y;//Y座標へのベクトル
				g_aAttack[nCntAttack].fVaim = atan2f(g_aAttack[nCntAttack].fXVaim, g_aAttack[nCntAttack].fYVaim);//プレイヤーへの角度g_aAttack[nCntAttack].fYVaim
				//=========================================================================================================================

				if (g_aAttack[nCntAttack].rot.z >= 0.0f && g_aAttack[nCntAttack].rot.z <= D3DX_PI)
				{
					if (g_aAttack[nCntAttack].fVaim > g_aAttack[nCntAttack].rot.z - 3.14f - fData &&
						g_aAttack[nCntAttack].fVaim < g_aAttack[nCntAttack].rot.z - 3.14f + fData)
					{//プレイヤーに攻撃が当たった(プレイヤーの中 transform_pos[nCntAttack].x心がどの位置にあるかで計算）

						if (g_nDelayAttack % 1 == 0)
						{
							if (g_aAttack[nCntAttack].type != ATTACKTYPE_FINALBEAM)
							{
								if (g_aAttack[nCntAttack].nHitCount <= 0)
								{//ヒット数が一回以下なら
									DamageHealth(30);

								}
							}
							else
							{
								if (g_aAttack[nCntAttack].nHitCount <= 0)
								{//ヒット数が一回以下なら
									DamageHealth(10);

								}
							}
						}
						if (g_aAttack[nCntAttack].type != ATTACKTYPE_FINALBEAM)
						{
							g_aAttack[nCntAttack].nHitCount++;//ヒット数のカウントを１増やす
						}
					}
				}
				else if (g_aAttack[nCntAttack].rot.z >= -D3DX_PI && g_aAttack[nCntAttack].rot.z <= 0.0f)
				{
					if (g_aAttack[nCntAttack].fVaim > g_aAttack[nCntAttack].rot.z + 3.14f - fData &&
						g_aAttack[nCntAttack].fVaim < g_aAttack[nCntAttack].rot.z + 3.14f + fData)
					{//プレイヤーに攻撃が当たった(プレイヤーの中 transform_pos[nCntAttack].x心がどの位置にあるかで計算）

						if (g_nDelayAttack % 1 == 0)
						{
							if (g_aAttack[nCntAttack].nHitCount <= 0)
							{//ヒット数が一回以下なら
								DamageHealth(30);

							}
						}
						if (g_aAttack[nCntAttack].type != ATTACKTYPE_FINALBEAM)
						{
							g_aAttack[nCntAttack].nHitCount++;//ヒット数のカウントを１増やす
						}
					}
				}
							
			}
			else if (g_aAttack[nCntAttack].type == ATTACKTYPE_SHAKINGSWORD)
			{//ボス３の技「剣攻撃」、

			g_aAttack[nCntAttack].nCntUseTime++;
			if (g_aAttack[nCntAttack].nCntUseTime == 1)
			{
				PlaySound(SOUND_LABEL_SE_SHAKINGSWORD);
			}


				Enemy* pEnemy = GetEnemy();
				Player* pPlayer = GetPlayer();
				g_aAttack[nCntAttack].nCntAnim++;//アニメーションカウンターを進める

				if (g_aAttack[nCntAttack].nCntAnim % 3 == 0)
				{
					g_aAttack[nCntAttack].nPatternAnim++;

				}
				if (g_aAttack[nCntAttack].nPatternAnim >= 5)
				{
					g_aAttack[nCntAttack].nPatternAnim = 0;
				}

				//テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 1.0f);


				if (g_aAttack[nCntAttack].nNumEnemy == 1)
				{
					g_aAttack->move.x = sinf(D3DX_PI * g_aAttack[nCntAttack].nCntAnim) * 10.0f;
				}

				g_aAttack[nCntAttack].rot.z += g_aAttack[nCntAttack].fRotSpeed;



				if (g_aAttack[nCntAttack].rot.z > D3DX_PI)
				{
					g_aAttack[nCntAttack].rot.z -= D3DX_PI * 2.0f;
				}
				else if (g_aAttack[nCntAttack].rot.z < -D3DX_PI)
				{
					g_aAttack[nCntAttack].rot.z += D3DX_PI * 2.0f;
				}

				//========================================
				//攻撃方向へのベクトル
				//========================================

			//============
			//前の処理
			//============
			/*	if (pPlayer->pos.x < g_aAttack[nCntAttack].pos.x + g_aAttack[nCntAttack].fWidth / 2 &&
					pPlayer->pos.x > g_aAttack[nCntAttack].pos.x - g_aAttack[nCntAttack].fWidth / 2 &&
					pPlayer->pos.y - pPlayer->fHeight / 2 >= g_aAttack[nCntAttack].pos.y - g_aAttack[nCntAttack].fHeight &&
					pPlayer->pos.y - pPlayer->fHeight / 2 <= g_aAttack[nCntAttack].pos.y)*/
					//========================================================================================
						//========================================
						//攻撃からプレイヤーへのベクトル
						//========================================
				g_aAttack[nCntAttack].fXVaim = pPlayer->pos.x - g_aAttack[nCntAttack].pos.x;//X座標へのベクトル
				g_aAttack[nCntAttack].fYVaim = pPlayer->pos.y - g_aAttack[nCntAttack].pos.y;//Y座標へのベクトル
				g_aAttack[nCntAttack].fVaim = atan2f(g_aAttack[nCntAttack].fXVaim, g_aAttack[nCntAttack].fYVaim);//プレイヤーへの角度g_aAttack[nCntAttack].fYVaim
				//=========================================================================================================================

				if (pPlayer->pos.x >= g_aAttack[nCntAttack].pos.x - g_aAttack[nCntAttack].fHeight &&
					pPlayer->pos.x <= g_aAttack[nCntAttack].pos.x + g_aAttack[nCntAttack].fHeight &&
					pPlayer->pos.y >= g_aAttack[nCntAttack].pos.y - g_aAttack[nCntAttack].fHeight &&
					pPlayer->pos.y <= g_aAttack[nCntAttack].pos.y + g_aAttack[nCntAttack].fHeight)
				{

					if (g_aAttack[nCntAttack].rot.z >= 0.0f && g_aAttack[nCntAttack].rot.z <= D3DX_PI)
					{
						if (g_aAttack[nCntAttack].fVaim > g_aAttack[nCntAttack].rot.z - 3.14f - 0.1f &&
							g_aAttack[nCntAttack].fVaim < g_aAttack[nCntAttack].rot.z - 3.14f + 0.1f)
						{//プレイヤーに攻撃が当たった(プレイヤーの中 transform_pos[nCntAttack].x心がどの位置にあるかで計算）

							if (g_nDelayAttack % 1 == 0)
							{
								if (g_aAttack[nCntAttack].nHitCount <= 10)
								{//ヒット数が一回以下なら
									DamageHealth(20);

								}
							}
							g_aAttack[nCntAttack].nHitCount++;//ヒット数のカウントを１増やす
						}
					}
					else if (g_aAttack[nCntAttack].rot.z >= -D3DX_PI && g_aAttack[nCntAttack].rot.z <= 0.0f)
					{
						if (g_aAttack[nCntAttack].fVaim > g_aAttack[nCntAttack].rot.z + 3.14f - 0.1f &&
							g_aAttack[nCntAttack].fVaim < g_aAttack[nCntAttack].rot.z + 3.14f + 0.1f)
						{//プレイヤーに攻撃が当たった(プレイヤーの中 transform_pos[nCntAttack].x心がどの位置にあるかで計算）

							if (g_nDelayAttack % 1 == 0)
							{
								if (g_aAttack[nCntAttack].nHitCount <= 10)
								{//ヒット数が一回以下なら
									DamageHealth(20);

								}
							}
							g_aAttack[nCntAttack].nHitCount++;//ヒット数のカウントを１増やす
						}
					}

				}
			}
			else if (g_aAttack[nCntAttack].type == ATTACKTYPE_ICICLECLAW)
			{//エネミー１１の技「アイシクルクロー」
			g_aAttack[nCntAttack].nCntUseTime++;
			if (g_aAttack[nCntAttack].nCntUseTime == 1)
			{
				PlaySound(SOUND_LABEL_SE_ICICLECLAW);
			}
			Enemy* pEnemy = GetEnemy();
			Player* pPlayer = GetPlayer();
			g_aAttack[nCntAttack].nCntAnim++;//アニメーションカウンターを進める

			if (g_aAttack[nCntAttack].nCntAnim % 3 == 0)
			{
				g_aAttack[nCntAttack].nPatternAnim++;

			}
			if (g_aAttack[nCntAttack].nPatternAnim >= 5)
			{
				g_aAttack[nCntAttack].nPatternAnim = 0;
				g_aAttack[nCntAttack].nPatternAnimY++;//アニメーションの段を次の段へ移動


				if (g_aAttack[nCntAttack].nPatternAnimY >= 2)
				{//アニメーションを最初の位置へ
					g_aAttack[nCntAttack].nPatternAnim = 0;
					g_aAttack[nCntAttack].nPatternAnimY = 0;
					//g_aAttack[nCntAttack].bUse = false;//使用していない状態にする
				}
			}

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.0f + 0.5f * g_aAttack[nCntAttack].nPatternAnimY);
			pVtx[1].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.0f + 0.5f * g_aAttack[nCntAttack].nPatternAnimY);
			pVtx[2].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.5f + 0.5f * g_aAttack[nCntAttack].nPatternAnimY);
			pVtx[3].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.5f + 0.5f * g_aAttack[nCntAttack].nPatternAnimY);



			/*	g_aAttack[nCntAttack].rot.z -= 0.01f;*/


			if (g_aAttack[nCntAttack].rot.z > D3DX_PI)
			{
				g_aAttack[nCntAttack].rot.z -= D3DX_PI * 2.0f;
			}
			else if (g_aAttack[nCntAttack].rot.z < -D3DX_PI)
			{
				g_aAttack[nCntAttack].rot.z += D3DX_PI * 2.0f;
			}
			//========================================
			//攻撃方向へのベクトル
			//========================================

		//============
		//前の処理
		//============
		/*	if (pPlayer->pos.x < g_aAttack[nCntAttack].pos.x + g_aAttack[nCntAttack].fWidth / 2 &&
				pPlayer->pos.x > g_aAttack[nCntAttack].pos.x - g_aAttack[nCntAttack].fWidth / 2 &&
				pPlayer->pos.y - pPlayer->fHeight / 2 >= g_aAttack[nCntAttack].pos.y - g_aAttack[nCntAttack].fHeight &&
				pPlayer->pos.y - pPlayer->fHeight / 2 <= g_aAttack[nCntAttack].pos.y)*/
				//========================================================================================
					//========================================
					//攻撃からプレイヤーへのベクトル
					//========================================
			g_aAttack[nCntAttack].fXVaim = pPlayer->pos.x - g_aAttack[nCntAttack].pos.x;//X座標へのベクトル
			g_aAttack[nCntAttack].fYVaim = pPlayer->pos.y - g_aAttack[nCntAttack].pos.y;//Y座標へのベクトル
			g_aAttack[nCntAttack].fVaim = atan2f(g_aAttack[nCntAttack].fXVaim, g_aAttack[nCntAttack].fYVaim);//プレイヤーへの角度g_aAttack[nCntAttack].fYVaim
			//=========================================================================================================================

			if (pPlayer->pos.x >= g_aAttack[nCntAttack].pos.x - g_aAttack[nCntAttack].fHeight &&
				pPlayer->pos.x <= g_aAttack[nCntAttack].pos.x + g_aAttack[nCntAttack].fHeight &&
				pPlayer->pos.y >= g_aAttack[nCntAttack].pos.y - g_aAttack[nCntAttack].fHeight &&
				pPlayer->pos.y <= g_aAttack[nCntAttack].pos.y + g_aAttack[nCntAttack].fHeight)
			{

				if (g_aAttack[nCntAttack].rot.z >= 0.0f && g_aAttack[nCntAttack].rot.z <= D3DX_PI)
				{
					if (g_aAttack[nCntAttack].fVaim > g_aAttack[nCntAttack].rot.z - 3.14f - 0.1f &&
						g_aAttack[nCntAttack].fVaim < g_aAttack[nCntAttack].rot.z - 3.14f + 0.1f)
					{//プレイヤーに攻撃が当たった(プレイヤーの中 transform_pos[nCntAttack].x心がどの位置にあるかで計算）

						if (g_nDelayAttack % 1 == 0)
						{
							if (g_aAttack[nCntAttack].nHitCount <= 0)
							{//ヒット数が一回以下なら
								DamageHealth(20);

							}
						}
						g_aAttack[nCntAttack].nHitCount++;//ヒット数のカウントを１増やす
					}
				}
				else if (g_aAttack[nCntAttack].rot.z >= -D3DX_PI && g_aAttack[nCntAttack].rot.z <= 0.0f)
				{
					if (g_aAttack[nCntAttack].fVaim > g_aAttack[nCntAttack].rot.z + 3.14f - 0.1f &&
						g_aAttack[nCntAttack].fVaim < g_aAttack[nCntAttack].rot.z + 3.14f + 0.1f)
					{//プレイヤーに攻撃が当たった(プレイヤーの中 transform_pos[nCntAttack].x心がどの位置にあるかで計算）

						if (g_nDelayAttack % 1 == 0)
						{
							if (g_aAttack[nCntAttack].nHitCount <= 0)
							{//ヒット数が一回以下なら
								DamageHealth(20);

							}
						}
						g_aAttack[nCntAttack].nHitCount++;//ヒット数のカウントを１増やす
					}
				}

			}
			}
			else if (g_aAttack[nCntAttack].type == ATTACKTYPE_ABSOLUTE_BLIZZARD)
			{//ボス２の技「アブソリュート・ブリザード」

			g_aAttack[nCntAttack].nCntUseTime++;
			if (g_aAttack[nCntAttack].nCntUseTime == 1)
			{
				PlaySound(SOUND_LABEL_SE_ABSOLUTEBLIZARD);
			}

				Enemy* pEnemy = GetEnemy();
				Player* pPlayer = GetPlayer();
				g_aAttack[nCntAttack].nCntAnim++;//アニメーションカウンターを進める

				if (g_aAttack[nCntAttack].nCntAnim % 1 == 0)
				{
					g_aAttack[nCntAttack].nPatternAnim++;

				}
				if (g_aAttack[nCntAttack].nPatternAnim >= 5)
				{
					g_aAttack[nCntAttack].nPatternAnim = 0;
					g_aAttack[nCntAttack].nPatternAnimY++;//アニメーションの段を次の段へ移動


					if (g_aAttack[nCntAttack].nPatternAnimY >= 2)
					{//アニメーションを最初の位置へ
						g_aAttack[nCntAttack].nPatternAnim = 0;
						g_aAttack[nCntAttack].nPatternAnimY = 0;
						//g_aAttack[nCntAttack].bUse = false;//使用していない状態にする
					}
				}
				//テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.0f + 0.5f * g_aAttack[nCntAttack].nPatternAnimY);
				pVtx[1].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.0f + 0.5f * g_aAttack[nCntAttack].nPatternAnimY);
				pVtx[2].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.5f + 0.5f * g_aAttack[nCntAttack].nPatternAnimY);
				pVtx[3].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.5f + 0.5f * g_aAttack[nCntAttack].nPatternAnimY);



				if (pPlayer->pos.x >= g_aAttack[nCntAttack].pos.x - g_aAttack[nCntAttack].fWidth / 2 - pPlayer->fWidth &&
					pPlayer->pos.x <= g_aAttack[nCntAttack].pos.x + g_aAttack[nCntAttack].fWidth / 2 + pPlayer->fWidth &&
					pPlayer->pos.y - pPlayer->fHeight / 2 >= g_aAttack[nCntAttack].pos.y - g_aAttack[nCntAttack].fHeight / 2 &&
					pPlayer->pos.y - pPlayer->fHeight / 2 <= g_aAttack[nCntAttack].pos.y + pPlayer->fHeight / 2)
				{//プレイヤーに攻撃が当たった(プレイヤーの中心がどの位置にあるかで計算）
					if (g_nDelayAttack % 1 == 0)
					{
						if (g_aAttack[nCntAttack].nHitCount <= 0)
						{//ヒット数が一回以下なら
							DamageHealth(150);

						}
					}
					g_aAttack[nCntAttack].nHitCount++;//ヒット数のカウントを１増やす
				}


			}
			else if (g_aAttack[nCntAttack].type == ATTACKTYPE_FIRE)
			{//ボス３の技「炎攻撃」
				int nCntSaveEnemy = 0;
				g_aAttack[nCntAttack].nCntUseTime++;
				if (g_aAttack[nCntAttack].nCntUseTime == 1)
				{
					PlaySound(SOUND_LABEL_SE_FIRE);
				}

				if (g_aAttack[nCntAttack].rot.z == 0.2f)
				{
					if (g_aAttack[nCntAttack].pos.y >= 675.0f && g_aAttack[nCntAttack].nHitCount == 0)
					{
						SetAttack(g_aAttack[nCntAttack].pos,
							D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							ATTACKTYPE_FIRE,
							-1, -1, -1, MOVETYPE_NORMAL, 70.0f, 200.0f, 80, 0.0f, -1);

						g_aAttack[nCntAttack].nHitCount++;
					}
				}

				Enemy* pEnemy = GetEnemy();
				Player* pPlayer = GetPlayer();
				g_aAttack[nCntAttack].nCntAnim++;//アニメーションカウンターを進める

				if (g_aAttack[nCntAttack].nCntAnim % 3 == 0)
				{
					g_aAttack[nCntAttack].nPatternAnim++;

				}
				if (g_aAttack[nCntAttack].nPatternAnim >= 5)
				{
					g_aAttack[nCntAttack].nPatternAnim = 0;
				}

				float fRandumRot;
				float fRandumSpeed;
				fRandumRot = float((rand)() % 100 + 51) / 100.0f;
				fRandumSpeed = float((rand)() % 100) / 5.0f;

				if (g_aAttack[nCntAttack].rot.z == 0.1f)
				{
					if (g_nDelayAttack % 8 == 0)
					{
						SetBullet(g_aAttack[nCntAttack].pos, D3DXVECTOR3(sinf(-D3DX_PI * fRandumRot) * 10.0f, cosf(-D3DX_PI * fRandumRot) * 10.0f, 0.0f),
							500, BULLETTYPE_ENEMY, -1, -1, -1, true, MOVETYPE_GRAVITY);
					}
				}


				//テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.2f + 0.2f * g_aAttack[nCntAttack].nPatternAnim, 1.0f);




				if (pPlayer->pos.x >= g_aAttack[nCntAttack].pos.x - g_aAttack[nCntAttack].fWidth / 2 - pPlayer->fWidth &&
					pPlayer->pos.x <= g_aAttack[nCntAttack].pos.x + g_aAttack[nCntAttack].fWidth / 2 &&
					pPlayer->pos.y - pPlayer->fHeight / 2 >= g_aAttack[nCntAttack].pos.y - g_aAttack[nCntAttack].fHeight &&
					pPlayer->pos.y - pPlayer->fHeight / 2 <= g_aAttack[nCntAttack].pos.y + pPlayer->fHeight / 2)
				{//プレイヤーに攻撃が当たった(プレイヤーの中心がどの位置にあるかで計算）
					if (g_nDelayAttack % 1 == 0)
					{
						if (g_aAttack[nCntAttack].nHitCount <= 0)
						{//ヒット数が一回以下なら
							DamageHealth(1);

						}
					}
					//g_aAttack[nCntAttack].nHitCount++;//ヒット数のカウントを１増やす
				}
			}
			//===============================
			//攻撃の動き方を決める処理
			//===============================

			bool b = false;
			//////移動量を更新（減衰させる）
			//g_aAttack[nCntAttack].move.x += (0.0f - g_aAttack[nCntAttack].move.x) * 0.5f;
			////g_aPlayer.move.y += (0.0f - g_aPlayer.move.y) * 0.5f;

			if (g_aAttack[nCntAttack].moveType == MOVETYPE_GRAVITY)
			{

				g_aAttack[nCntAttack].bIsLanding = bCollisionBlock(&g_aAttack[nCntAttack].pos,
					&g_aAttack[nCntAttack].posOld,
					&g_aAttack[nCntAttack].move,
					g_aAttack[nCntAttack].fWidth,
					g_aAttack[nCntAttack].fHeight,
					&g_aAttack[nCntAttack].fGravity,
					COLLISIONTYPE_LENGTH, &b, COLLISIONUSE_ATTACK,
					&g_aAttack[nCntAttack].bIsWalling);
			}
			if (g_aAttack[nCntAttack].moveType == MOVETYPE_GRAVITY && g_aAttack[nCntAttack].bIsLanding == true)
			{
				g_aAttack[nCntAttack].move.y = 0.0f;
			}
			else if (g_aAttack[nCntAttack].moveType == MOVETYPE_GRAVITY && g_aAttack[nCntAttack].bIsLanding == false)
			{
				g_aAttack[nCntAttack].move.y += 2.0f;
			}

			g_aAttack[nCntAttack].posOld.x = g_aAttack[nCntAttack].pos.x;
			g_aAttack[nCntAttack].posOld.y = g_aAttack[nCntAttack].pos.y;

			g_aAttack[nCntAttack].pos.y += g_aAttack[nCntAttack].move.y;
			g_aAttack[nCntAttack].pos.x += g_aAttack[nCntAttack].move.x;


			//==========================================================================

			if (g_aAttack[nCntAttack].nLife <= 0)
			{//表示時間が０以下になったら（攻撃の寿命が尽きた）(種類は全て）

				g_aAttack[nCntAttack].bUse = false;//使用していない状態にする
			}

		

			//============================================================================================================================================================
		}


		pVtx += 4;
	}


	//頂点バッファをアンロックする
	g_pVtxBuffAttack->Unlock();
}

//攻撃の描画処理
void DrawAttack(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	int nCntAttack;

	Enemy* pEnemy = GetEnemy();
	Player* pPlayer = GetPlayer();

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffAttack, 0, sizeof(VERTEX_2D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	for (nCntAttack = 0; nCntAttack < MAX_ATTACK; nCntAttack++)
	{
		if (g_aAttack[nCntAttack].bUse == true)
		{
			if (g_aAttack[nCntAttack].type ==ATTACKTYPE_PUNCH)
			{
				pDevice->SetTexture(0, g_pTextureAttack[ATTACKTYPE_PUNCH]);
			}
			if (g_aAttack[nCntAttack].type == ATTACKTYPE_SLASH_E)
			{
				pDevice->SetTexture(0, g_pTextureAttack[ATTACKTYPE_SLASH_E]);
			}
			if (g_aAttack[nCntAttack].type == ATTACKTYPE_ICE_EXPLOSION)
			{
				pDevice->SetTexture(0, g_pTextureAttack[ATTACKTYPE_ICE_EXPLOSION]);
			}
			if (g_aAttack[nCntAttack].type == ATTACKTYPE_THUNDER_E4)
			{
				pDevice->SetTexture(0, g_pTextureAttack[ATTACKTYPE_THUNDER_E4]);
			}
			if (g_aAttack[nCntAttack].type == ATTACKTYPE_BOMU)
			{
				pDevice->SetTexture(0, g_pTextureAttack[ATTACKTYPE_BOMU]);
			}
			if (g_aAttack[nCntAttack].type == ATTACKTYPE_NEEDLE)
			{
				pDevice->SetTexture(0, g_pTextureAttack[ATTACKTYPE_NEEDLE]);
			}
			if (g_aAttack[nCntAttack].type == ATTACKTYPE_CLAW)
			{
				pDevice->SetTexture(0, g_pTextureAttack[ATTACKTYPE_CLAW]);
			}
			if (g_aAttack[nCntAttack].type == ATTACKTYPE_VACUUMBLADE)
			{
				pDevice->SetTexture(0, g_pTextureAttack[ATTACKTYPE_VACUUMBLADE]);
			}
			if (g_aAttack[nCntAttack].type == ATTACKTYPE_FALLINGBLOCK)
			{
				pDevice->SetTexture(0, g_pTextureAttack[ATTACKTYPE_FALLINGBLOCK]);
			}
			if (g_aAttack[nCntAttack].type == ATTACKTYPE_BEAM)
			{
				pDevice->SetTexture(0, g_pTextureAttack[ATTACKTYPE_BEAM]);
			}
			if (g_aAttack[nCntAttack].type == ATTACKTYPE_ICICLECLAW)
			{
				pDevice->SetTexture(0, g_pTextureAttack[ATTACKTYPE_ICICLECLAW]);
			}
			if (g_aAttack[nCntAttack].type == ATTACKTYPE_ABSOLUTE_BLIZZARD)
			{
				pDevice->SetTexture(0, g_pTextureAttack[ATTACKTYPE_ABSOLUTE_BLIZZARD]);
			}
			if (g_aAttack[nCntAttack].type == ATTACKTYPE_FIRE)
			{
				pDevice->SetTexture(0, g_pTextureAttack[ATTACKTYPE_FIRE]);
			}
			if (g_aAttack[nCntAttack].type == ATTACKTYPE_SHAKINGSWORD)
			{
				pDevice->SetTexture(0, g_pTextureAttack[ATTACKTYPE_SHAKINGSWORD]);
			}
			if (g_aAttack[nCntAttack].type == ATTACKTYPE_FLASHBLADE)
			{
				pDevice->SetTexture(0, g_pTextureAttack[ATTACKTYPE_FLASHBLADE]);
			}
			if (g_aAttack[nCntAttack].type == ATTACKTYPE_THUNDER_2)
			{
				pDevice->SetTexture(0, g_pTextureAttack[ATTACKTYPE_THUNDER_2]);
			}
			if (g_aAttack[nCntAttack].type == ATTACKTYPE_FINALBEAM)
			{
				pDevice->SetTexture(0, g_pTextureAttack[ATTACKTYPE_FINALBEAM]);
			}
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,//0は、描画する最初の頂点インデックスで、2は、描画するプリミティブ数
				nCntAttack * 4,
				2);
		}

	}


}

//攻撃の設定処理
void SetAttack(D3DXVECTOR3 pos, D3DXVECTOR3 move, ATTACKTYPE type, int nNum, int nNumEnemy, int nAimingEnemy, MOVETYPE moveType,
	float Width, float Height, int nLife, float rot, float fRotSpeed)//posは「発射位置」、moveは「移動量」,damageは「弾の攻撃力」nNumEneyは、何番目の敵が攻撃しているか?
{
int nCntAttack;

	VERTEX_2D* pVtx;
	g_pVtxBuffAttack->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntAttack = 0; nCntAttack < MAX_ATTACK; nCntAttack++)
	{
	

		if (g_aAttack[nCntAttack].bUse == false)
		{//攻撃が使用されていない
		
			//=================================================
			//セットで設定する処理
			//=================================================
			g_aAttack[nCntAttack].pos = pos;//位置
			g_aAttack[nCntAttack].posOld = pos;//前の位置
			g_aAttack[nCntAttack].move = move;//動き
			g_aAttack[nCntAttack].bUse = true;//使用している状態にする
			g_aAttack[nCntAttack].type = type;//攻撃の種類（type)を代入する。
			g_aAttack[nCntAttack].nCntAnim = 0;//アニメーションカウンター
			g_aAttack[nCntAttack].nPatternAnim = 0;//アニメーションパターン
			g_aAttack[nCntAttack].nPatternAnimY = 0;//アニメーションが２段以上の時に弾を移す処理
			g_aAttack[nCntAttack].nHitCount = 0;//一回の攻撃でヒットする回数の制限
			g_aAttack[nCntAttack].nNumEnemy = nNumEnemy;//どの敵が攻撃しているかを判定する
			g_aAttack[nCntAttack].moveType = moveType;//攻撃の動き方の種類
			g_aAttack[nCntAttack].bIsLanding = false;//地面についているフラグ
			g_aAttack[nCntAttack].bIsWalling = false;//壁に当たっているか
			g_aAttack[nCntAttack].fGravity = 0.0f;//重力補正値
			g_aAttack[nCntAttack].nColorChange = 0;//色を変える
			g_aAttack[nCntAttack].nCntUseTime = 0;//攻撃を使っている時間をカウントする
			g_aAttack[nCntAttack].rot.z = 0.0f;
			g_aAttack[nCntAttack].nLife = nLife;//寿命
			g_aAttack[nCntAttack].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//向き
			g_aAttack[nCntAttack].fWidth = Width;//幅
			g_aAttack[nCntAttack].fHeight = Height;//高さ
			g_aAttack[nCntAttack].rot.z = rot;
			g_aAttack[nCntAttack].fXVaim = 0.0f;
			g_aAttack[nCntAttack].fYVaim = 0.0f;
			g_aAttack[nCntAttack].fVaim = 0.0f;
			g_aAttack[nCntAttack].fEffectiveScale = 0.0f;//攻撃の効果範囲を決める
			g_aAttack[nCntAttack].fRotSpeed = fRotSpeed;//回転速度
			//=================================================================================
		
			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。

			break;
		}
		pVtx += 4;
	}
	g_pVtxBuffAttack->Unlock();
}

//=====================================================
//攻撃構造体の情報を取得
//=====================================================
Attack* GetAttack(void)
{
	return &g_aAttack[0];//構造体の先頭アドレスを返す。
}