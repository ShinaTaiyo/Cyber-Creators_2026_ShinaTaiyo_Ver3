//========================================================
//
//８月３０日：エフェクトもう一回見直す[player.cpp]
//Author:ShinaTaiyo
//
//=========================================================


#include "main.h"
#include "player.h"//作成したpolygon.hをインクルードする
#include "input.h"
#include "score.h"
#include "sound.h"
#include <time.h>
#include "tutorial.h"
#include "block.h"
#include "health.h"
#include "Attack.h"
#include "Ability.h"
#include "bullet.h"

#define PLAYER_MOVE (7.5f)//プレイヤー移動速度
#define NUM_PLAYER_MOTION (3)//プレイヤーのモーションの種類

//グローバル変数
Player g_aPlayer;//プレイヤーの構造体

LPDIRECT3DTEXTURE9 g_pTexturePolygon[NUM_PLAYER_MOTION] = {};//テクスチャへのポインタ
//VERTEX_2D g_aVertex[4];//頂点情報を格納（四角形を作りたい）
PDIRECT3DVERTEXBUFFER9 g_pVtxBuffPolygon = NULL;//頂点バッファへのポインタ



int g_nCounterAnimPlayer;//アニメーションカウンター
int g_nPatternAnimPlayer;//移動量

float g_fSpeedUp = 0;

int g_nData = 1;

float g_fRegBullet = 0;//弾の方向調整用変数


D3DXVECTOR3 g_StartPos;//プレイヤーの初期位置をセーブする。

//D3DXVECTOR3 g_aPlayer.pos;//位置
//D3DXVECTOR3 g_aPlayer.move;//移動量
//D3DXVECTOR3 g_aPlayer.rot;//向き
float g_fLengthPlayer;//対角線の長さ
float g_fAnglePlayer;//対角線の角度
float g_fDiagonalMovex;//斜め移動用変数
float g_fDiagonalMovey;//斜め移動用変数
int g_nMotionMode;//動作モードの設定用変数


float g_fScale=1.0f;//拡大率を管理する変数(初期値1.0f)


bool g_bTurnPoint=false;//ウインドウの端にキャラが到達した場合、往復させるためのフラグ
bool g_bTurnPoint2 = false;
float g_fXmove = 0;//X方向に動かす処理の変数
float g_fYmove = 0;//Y方向に動かす処理の変数

int g_nAnimation = 0;
int g_nAnimationDelay = 0;//アニメーションの遅延を行う変数
float g_nAnimationColumn = 0;//アニメーションを読み込む列を表す変数
int g_nAnimationCount = 0;

float g_fRoling=0;//回転用変数

int g_nStayCount = 0;
bool g_bStayFlag = false;

//=======================================================
//ポリゴンの初期化処理
//=======================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	Tutorial* pTutorial = GetTutorial(); 
//GameCustom * pGameCustom = GetGameCustom();

	
	//プレイヤー構造体の情報の初期化==========================================
	g_aPlayer.state = PLAYERSTATE_NORMAL;
	g_aPlayer.nHitCount = 0;
	g_nMotionMode = 7;
	g_aPlayer.bInvincibility = false;//無敵フラグの初期化（off)
	g_aPlayer.bDisp = true;//プレイヤーを表示するかどうかのフラグをonにする。
	g_aPlayer.fGravity = 0.0f;//重力の値を初期化
	g_nCounterAnimPlayer = 0;//カウンターを初期化する
	g_nPatternAnimPlayer = 0;//パターンNoを初期化する
	g_aPlayer.move = D3DXVECTOR3(0.0f,0.0f,0.0f);//移動量を初期化する
	g_aPlayer.rot = D3DXVECTOR3(0.0f, 0.0f,0.0f);//向きを初期化する（ｚ座標を変えることにより、向きを変えることができる）
	g_aPlayer.nCounterAnim = 0; //アニメーションカウンター
	g_aPlayer.nPatternAnim = 0;//アニメーションパターン
	g_aPlayer.nDirectionMove = 0;//向きを判定
	g_aPlayer.bIsJumping = false;//ジャンプしているかどうか
	g_aPlayer.bUse = true;//使用しているかどうか
	g_aPlayer.bIsLanding = false;//ブロックに乗っているかどうか
	g_aPlayer.bIsWalling = false;//壁に当たっているかどうか
	g_aPlayer.g_bTouchingBlock = false;//ブロックに触れていたら発動するフラグ。スリップダメージ処理などで使う
	g_aPlayer.bDirection = false;//右に向いていることにする
	g_aPlayer.nMotionType = PLAYERMOTION_STAND;//使用しているプレイヤーのモーションの種類
	g_aPlayer.nMotionTypeOld = PLAYERMOTION_STAND;//前に使用しているモーションを保存する
	//g_aPlayer.pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 80.0f, 0.0f);//位置を初期化する
	g_aPlayer.pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 80.0f, 0.0f);//位置を初期化する
	g_aPlayer.posOld = g_aPlayer.pos;
	g_aPlayer.bUseMotion = false;//モーションを使用しているかどうかのフラグ
	g_aPlayer.nMotionTime = 0;//モーションを使用している時間を測る
	g_aPlayer.fWidth = PLAYER_WIDTH;//幅
	g_aPlayer.fHeight = PLAYER_HEIGHT;//高さ
	g_aPlayer.bDashL = false;//左方向へのダッシュ状態かどうか
	g_aPlayer.bDashR = false;//右方向へのダッシュ状態かどうか
	g_aPlayer.nDashCount = 0;//ダッシュ状態になるまでの時間をカウントする
	g_aPlayer.nDashCount = 0;//ダッシュするためのボタンを押した回数をカウント
	g_aPlayer.nCntDashButtonL = 0;
	g_aPlayer.nCntDashButtonR = 0;
	g_aPlayer.nInertia = 0;//慣性を調整する
	g_aPlayer.bAutoMoveR = false;//右方向へ自動的に移動するかどうかのフラグ
	g_aPlayer.bAutoMoveL = false;//左方向へ自動的に移動するかどうかのフラグ
	//===============================================================================
	//デバイスの取得
	pDevice = GetDevice();
	
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\ACT_Player_StandMotion.png",
		&g_pTexturePolygon[PLAYERMOTION_STAND]);//待機モーション

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\ACT_Player_RunMotion_000.png",
		&g_pTexturePolygon[PLAYERMOTION_MOVE]);//移動モーション

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\ACT_PlayerMotion_Punch_000.png",
		&g_pTexturePolygon[PLAYERMOTION_PUNCH]);//移動モーション


	int nMode;

	nMode = GetMode();
	

	 g_nStayCount = 0;
	 g_bStayFlag = false;


	//if (nMode >= 2)
	//{
	//	g_aPlayer.pos = D3DXVECTOR3(SCREEN_WIDTH/2,SCREEN_HEIGHT, 0.0f);//位置を初期化する
	//}

	g_StartPos = g_aPlayer.pos;
	//srand((unsigned int)time(NULL));//ゲームを開始した時間毎に乱数の種を設定

	//対角線の長さを算出する
	g_fLengthPlayer = sqrtf(PLAYER_WIDTH * PLAYER_WIDTH + PLAYER_HEIGHT * PLAYER_HEIGHT) / 2.0f;//プレイヤーの幅2乗＋プレイヤーの高さ２乗/２

	//対角線の角度を算出する
	g_fAnglePlayer = atan2f(PLAYER_WIDTH,PLAYER_HEIGHT);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPolygon,
		NULL);
    
	VERTEX_2D* pVtx;//頂点情報へのポインタ
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);




	//頂点座標の設定
	//pVtx[0].pos = D3DXVECTOR3(500.0f, 150.0f, 0.0f);//X座標左
 //   pVtx[1].pos = D3DXVECTOR3(650.0f, 150.0f, 0.0f);//X座標右
	//pVtx[2].pos = D3DXVECTOR3(500.0f, 300.0f, 0.0f);//Y座標上
	//pVtx[3].pos = D3DXVECTOR3(650.0f, 300.0f, 0.0f);//Y座標下

	/*pVtx[0].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.rot.z  + g_fAnglePlayer*-0.75) * g_fLengthPlayer;
	pVtx[0].pos.y = g_aPlayer.pos.y +cosf(g_aPlayer.rot.z + g_fAnglePlayer*-0.75) * g_fLengthPlayer;
	pVtx[0].pos.z= 0.0f;

	pVtx[1].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.rot.z + g_fAnglePlayer*0.75) * g_fLengthPlayer;
	pVtx[1].pos.y = g_aPlayer.pos.y + cosf(g_aPlayer.rot.z + g_fAnglePlayer*-0.75) * g_fLengthPlayer;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.rot.z + g_fAnglePlayer*-0.25) * g_fLengthPlayer;
	pVtx[2].pos.y = g_aPlayer.pos.y + cosf(g_aPlayer.rot.z + g_fAnglePlayer*+0.25) * g_fLengthPlayer;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.rot.z + g_fAnglePlayer*0.25) * g_fLengthPlayer;
	pVtx[3].pos.y = g_aPlayer.pos.y + cosf(g_aPlayer.rot.z + g_fAnglePlayer*0.25) * g_fLengthPlayer;
	pVtx[3].pos.z = 0.0f;*/


	/*if (pTutorial->bTutorialUse == true)
	{
		    pVtx[0].pos=
			pVtx[1].pos=
			pVtx[2].pos=
			pVtx[3].pos=
	}*/

	//頂点座標の設定
	pVtx[0].pos.x = g_aPlayer.pos.x - PLAYER_WIDTH;
	pVtx[0].pos.y = g_aPlayer.pos.y - PLAYER_HEIGHT;
	pVtx[0].pos.z = 0.0f;
				
	pVtx[1].pos.x = g_aPlayer.pos.x + PLAYER_WIDTH;
	pVtx[1].pos.y = g_aPlayer.pos.y - PLAYER_HEIGHT;
	pVtx[1].pos.z = 0.0f;
					
	pVtx[2].pos.x = g_aPlayer.pos.x - PLAYER_WIDTH;
	pVtx[2].pos.y = g_aPlayer.pos.y;
	pVtx[2].pos.z = 0.0f;
					
	pVtx[3].pos.x = g_aPlayer.pos.x + PLAYER_WIDTH;
	pVtx[3].pos.y = g_aPlayer.pos.y;
	pVtx[3].pos.z = 0.0f;

	//g_aPlayer.PosBeside = D3DXVECTOR3(g_aPlayer.pos.x - PLAYER_WIDTH, g_aPlayer.pos.y - PLAYER_HEIGHT / 2, 0.0f);//プレイヤーの右横側に、更に中点を追加

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255,255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255,255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255,255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255,255);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.25f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.25f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffPolygon->Unlock();
}

//========================================
//ポリゴンの終了処理
//========================================
void UninitPlayer(void)
{
	//テクスチャの破棄
	for (int nCnt = 0; nCnt < NUM_PLAYER_MOTION; nCnt++)
	{
		if (g_pTexturePolygon[nCnt]!= NULL)
		{
			g_pTexturePolygon[nCnt]->Release();
			g_pTexturePolygon[nCnt] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffPolygon != NULL)
	{
		g_pVtxBuffPolygon->Release();
		g_pVtxBuffPolygon = NULL;
	}
}
//==========================================================
//ポリゴンの更新処理
//==========================================================
void UpDatePlayer(void)//１フレームごとに処理される。
{
	//srand((unsigned int)time(NULL));//ゲームを開始した時間毎に乱数の種を設定




	Tutorial* pTutorial = GetTutorial();

	


	//g_nXarrowShot_r = rand() % 100 + 1;//スキル３（Xarrow）の弾の方向処理

	//g_fXarrowShot_r = 0.001f * g_nXarrowShot_r;//スキル３(Xarrow)の弾の方向処理

	//g_nXarrowShotSpeed_r = rand() % 30 + 1;

	//g_fXarrowShotSpeed_r = 1.0f * g_nXarrowShotSpeed_r;

	//g_nXarrowShotRot_r = rand() % 4 + 1;//スキル３の弾撃つ方向（４方向基準）の処理

	g_fRoling += 0.01f;//回転角度をインクリメントし続ける

		//対角線の長さを算出する(STG)
	//g_fLengthPlayer = sqrtf((PLAYER_WIDTH * PLAYER_WIDTH)*g_fScale + (PLAYER_HEIGHT * PLAYER_HEIGHT)*g_fScale) / 2.0f;//プレイヤーの幅2乗＋プレイヤーの高さ２乗/２*拡大率


	//対角線の長さを選出する(ACT)
	g_fLengthPlayer = sqrtf((PLAYER_WIDTH * PLAYER_WIDTH) * g_fScale + (PLAYER_HEIGHT * PLAYER_HEIGHT) * g_fScale) / 2.0f;//プレイヤーの幅2乗＋プレイヤーの高さ２乗/２*拡大率




	

	g_aPlayer.nDashCount++;

	//ブロックに着地しない限り、フラグをfalseにするために、ブロック処理の前に置く
	g_aPlayer.bIsLanding = false;

	//ブロックの横に当たっていない限り、フラグをfalseにするために、ブロック処理の前に置く
	g_aPlayer.bIsWalling = false;

	bool b = false;


	

	g_aPlayer.bAutoMoveR = false;//右方向へ自動的に移動するかどうかのフラグ
	g_aPlayer.bAutoMoveL = false;//左方向へ自動的に移動するかどうかのフラグ

	g_aPlayer.nInertia = INERTIA_NORMAL;//普通の慣性状態

	g_aPlayer.bIsLanding = bCollisionBlock(&g_aPlayer.pos, &g_aPlayer.posOld, &g_aPlayer.move, g_aPlayer.fWidth, g_aPlayer.fHeight, &g_aPlayer.fGravity, COLLISIONTYPE_RADIUS, &b,COLLISIONUSE_PLAYER,&g_aPlayer.bIsWalling);

	if (g_aPlayer.bAutoMoveR == true)
	{
		g_aPlayer.move.x += 6.0f;
	}
	if (g_aPlayer.bAutoMoveL == true)
	{
		g_aPlayer.move.x += -6.0f;
	}

	/*if (pGameCustom->nDecideCount >= 2)
	{*/
	if (GetKeyboardPress(DIK_A) == true || GetJoypadPress(JOYKEY_LEFT) == true)
	{//Aキーが押された

		if (g_aPlayer.bDashL == false)
		{
			//if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP))
			//{//Wキーが押された
			//	//左上移動

			//	g_aPlayer.pos.x += sinf(-D3DX_PI * 0.75) * PLAYER_MOVE;
			//	g_aPlayer.pos.y += cosf(-D3DX_PI * 0.75) * PLAYER_MOVE;
			//}
			//else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true)
			//{//Sキーが押された
			//	//左下移動
			//	g_aPlayer.pos.x += sinf(-D3DX_PI * 0.25) * PLAYER_MOVE;
			//	g_aPlayer.pos.y += cosf(-D3DX_PI * 0.25) * PLAYER_MOVE;
			//}
			g_aPlayer.bDirection = true;//左に向いていることにする
			g_aPlayer.nMotionType = PLAYERMOTION_MOVE;
			g_aPlayer.move.x += sinf(-D3DX_PI * 0.5) * PLAYER_MOVE;
			/*g_aPlayer.pos.y += cosf(-D3DX_PI * 0.5) * PLAYER_MOVE;*/
		}
	}
	else if (GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT) == true)
	{//Dキーが押された
		if (g_aPlayer.bDashR == false)
		{
			//if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true)
			//{//Wきーが押された
			//	//右上移動
			//	g_aPlayer.pos.x += sinf(D3DX_PI * 0.75) * PLAYER_MOVE;
			//	g_aPlayer.pos.y += cosf(D3DX_PI * 0.75) * PLAYER_MOVE;
			//}
			//else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true)
			//{//Sきーが押されたs
			//	//右下移動
			//	g_aPlayer.pos.x += sinf(D3DX_PI * 0.25) * PLAYER_MOVE;
			//	g_aPlayer.pos.y += cosf(D3DX_PI * 0.25) * PLAYER_MOVE;
			//}
			//else
			//{//右移動
			g_aPlayer.bDirection = false;//右に向いていることにする
			g_aPlayer.nMotionType = PLAYERMOTION_MOVE;
			g_aPlayer.move.x += sinf(D3DX_PI * 0.5) * PLAYER_MOVE;
			/*g_aPlayer.pos.y += cosf(D3DX_PI * 0.5) * PLAYER_MOVE;*/
			/*	}*/
		}
	}
	//else if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true)
	//{//Wキーが押された
	//	g_aPlayer.pos.x += sinf(D3DX_PI * 1.0f) * PLAYER_MOVE;
	//	g_aPlayer.pos.y += cosf(D3DX_PI * 1.0f) * PLAYER_MOVE;
	//}
	//else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true)
	//{//Sキーが押された
	//	g_aPlayer.pos.x += sinf(D3DX_PI * 0.0f) * PLAYER_MOVE;
	//	g_aPlayer.pos.y += cosf(D3DX_PI * 0.0f) * PLAYER_MOVE;
	//}

	
	//====================================
	//ダッシュ処理１
	//====================================
		if (GetKeyboardTrigger(DIK_A) == true || GetJoyPadTrigger(JOYKEY_LEFT) == true)
		{
			g_aPlayer.nCntDashButtonL++;
		}
		if (g_aPlayer.nCntDashButtonL >= 2)
		{
			g_aPlayer.bDashL = true;
		}

		if (GetKeyboardTrigger(DIK_D) == true || GetJoyPadTrigger(JOYKEY_RIGHT) == true)
		{
			g_aPlayer.nCntDashButtonR++;
		}
		if (g_aPlayer.nCntDashButtonR >= 2)
		{
			g_aPlayer.bDashR = true;
		}
	//=================================================================================================

	//==================================
	//ダッシュ状態
	//==================================
	if (GetKeyboardPress(DIK_A) == true || GetJoypadPress(JOYKEY_LEFT) == true)
	{//Aキーが押された

		if (g_aPlayer.bDashL == true)
		{
			//if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP))
			//{//Wキーが押された
			//	//左上移動

			//	g_aPlayer.pos.x += sinf(-D3DX_PI * 0.75) * PLAYER_MOVE;
			//	g_aPlayer.pos.y += cosf(-D3DX_PI * 0.75) * PLAYER_MOVE;
			//}
			//else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true)
			//{//Sキーが押された
			//	//左下移動
			//	g_aPlayer.pos.x += sinf(-D3DX_PI * 0.25) * PLAYER_MOVE;
			//	g_aPlayer.pos.y += cosf(-D3DX_PI * 0.25) * PLAYER_MOVE;
			//}
			g_aPlayer.bDirection = true;//左に向いていることにする
			g_aPlayer.nMotionType = PLAYERMOTION_MOVE;
			g_aPlayer.move.x += sinf(-D3DX_PI * 0.5) * PLAYER_MOVE * 2;
		/*	g_aPlayer.pos.y += cosf(-D3DX_PI * 0.5) * PLAYER_MOVE * 2;*/

			


		}
	}
	else if (GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT) == true)
	{//Dキーが押された

		if (g_aPlayer.bDashR == true)
		{
			//if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true)
			//{//Wきーが押された
			//	//右上移動
			//	g_aPlayer.pos.x += sinf(D3DX_PI * 0.75) * PLAYER_MOVE;
			//	g_aPlayer.pos.y += cosf(D3DX_PI * 0.75) * PLAYER_MOVE;
			//}
			//else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true)
			//{//Sきーが押されたs
			//	//右下移動
			//	g_aPlayer.pos.x += sinf(D3DX_PI * 0.25) * PLAYER_MOVE;
			//	g_aPlayer.pos.y += cosf(D3DX_PI * 0.25) * PLAYER_MOVE;
			//}
			//else
			//{//右移動
			g_aPlayer.bDirection = false;//右に向いていることにする
			g_aPlayer.nMotionType = PLAYERMOTION_MOVE;
			g_aPlayer.move.x += sinf(D3DX_PI * 0.5) * PLAYER_MOVE * 2;
			/*g_aPlayer.pos.y += cosf(D3DX_PI * 0.5) * PLAYER_MOVE * 2;*/
		}
	
	}

		//==============
		//ジャンプ処理=============================================================================================================
		//==============
     
	


		Block* pBlock = GetBlock();
		

		if (g_aPlayer.bIsLanding == true)
		{//地面にいる
			g_aPlayer.move.y = 0.0f;//地面にいるので、重力をかけない
			if (GetKeyboardTrigger(DIK_SPACE) == true || GetJoyPadTrigger(JOYKEY_A) == true)
			{//ジャンプ処理
				PlaySound(SOUND_LABEL_SE_JUMP);
				g_aPlayer.move.y = -10.0f;
			}

		}
		if (g_aPlayer.bIsLanding == false)
		{//地面にいない

			if (GetKeyboardPress(DIK_SPACE) == true || GetJoypadPress(JOYKEY_A) == true)
			{//長押しで高くジャンプするようにする
				g_aPlayer.move.y += 2.0f + (g_aPlayer.fGravity * 4.5f);
				g_aPlayer.fGravity += 0.01f;
				g_aPlayer.move.y -= 2.1f + g_aPlayer.fGravity;
			}
			else
			{
				g_aPlayer.move.y += 2.0f;
			}
		}
		if (g_aPlayer.move.y >= 20.0f)
		{
			g_aPlayer.move.y = 20.0f;
		}


	

		//少し前の座標を代入する
		g_aPlayer.posOld.x = g_aPlayer.pos.x;

		g_aPlayer.posOld.y = g_aPlayer.pos.y;

		if (g_aPlayer.nInertia == INERTIA_NORMAL)
		{
		////移動量を更新（減衰させる）
		g_aPlayer.move.x += (0.0f - g_aPlayer.move.x) * 0.5f;
		//g_aPlayer.move.y += (0.0f - g_aPlayer.move.y) * 0.5f;
		}
		else if (g_aPlayer.nInertia == INERTIA_ICE)
		{
			////移動量を更新（減衰させる）
			g_aPlayer.move.x += (0.0f - g_aPlayer.move.x) * 0.3f;
			//g_aPlayer.move.y += (0.0f - g_aPlayer.move.y) * 0.5f;
		}


		//移動処理
		g_aPlayer.pos.x += g_aPlayer.move.x;
		g_aPlayer.pos.y += g_aPlayer.move.y;

		
		
		//=============================================================
		//ダッシュ処理２
		//=============================================================
		if (g_aPlayer.move.x <= 0.1f && g_aPlayer.move.x >= -0.1f)
		{
			g_bStayFlag = true;
			g_aPlayer.nDashCount = 0;
			g_aPlayer.bDashL = false;
			g_aPlayer.nCntDashButtonL = 0;
			g_aPlayer.bDashR = false;
			g_aPlayer.nCntDashButtonR = 0;
		}
		//================================================================================
	
		//=========================================================================================================================
	

		//ACTの画面端処理=================================================================================================
		if (g_aPlayer.pos.x > SCREEN_WIDTH + PLAYER_WIDTH / 2)
		{//画面右端まで移動したら
			g_aPlayer.pos.x = 0.0f + PLAYER_WIDTH / 2;//反対側へ移動する。
			//g_fSpeedUp+=0.1;//移動速度を上昇させる
		}

		if (g_aPlayer.pos.x < 0.0f - PLAYER_WIDTH / 2)
		{//画面左端まで移動したら
			g_aPlayer.pos.x = SCREEN_WIDTH - PLAYER_WIDTH / 2;//反対側へ移動する
			//g_fSpeedUp+=0.1;//移動速度を上昇させる
		}

		if (g_aPlayer.pos.y < 0 + PLAYER_HEIGHT / 2)
		{//画面上端まで移動したら
			g_aPlayer.pos.y = 0 + PLAYER_HEIGHT / 2;//画面外へ出ていかないように止める
			//g_fSpeedUp+=0.1;//移動速度を上昇させる
		}

		if (g_aPlayer.pos.y > SCREEN_HEIGHT - PLAYER_HEIGHT / 2)//画面より下に移動したら
		{//画面下端まで移動したら
			g_aPlayer.pos.y = SCREEN_HEIGHT - PLAYER_HEIGHT / 2;//画面外へ出ていかないように止める（地面に位置を固定する）
			//g_fSpeedUp+=0.1;//移動速度を上昇させる
		}
	

		//================================================================================================================

	


		//STGの画面端処理=============================================================================================
		//if (g_aPlayer.pos.x >= SCREEN_WIDTH - PLAYER_WIDTH / 2)
		//{//画面右端まで移動したら
		//	g_aPlayer.pos.x = SCREEN_WIDTH - PLAYER_WIDTH / 2;//画面外へ出ていかないように止める
		//	//g_fSpeedUp+=0.1;//移動速度を上昇させる
		//}

		//if (g_aPlayer.pos.x <= 0 + PLAYER_WIDTH / 2)
		//{//画面左端まで移動したら
		//	g_aPlayer.pos.x = 0 + PLAYER_WIDTH / 2;//画面外へ出ていかないように止める
		//	//g_fSpeedUp+=0.1;//移動速度を上昇させる
		//}

		//if (g_aPlayer.pos.y <= 0 + PLAYER_HEIGHT / 2)
		//{//画面上端まで移動したら
		//	g_aPlayer.pos.y = 0 + PLAYER_HEIGHT / 2;//画面外へ出ていかないように止める
		//	//g_fSpeedUp+=0.1;//移動速度を上昇させる
		//}

		//if (g_aPlayer.pos.y >= SCREEN_HEIGHT - PLAYER_HEIGHT / 2)
		//{//画面下端まで移動したら
		//	g_aPlayer.pos.y = SCREEN_HEIGHT - PLAYER_HEIGHT / 2;//画面外へ出ていかないように止める
		//	//g_fSpeedUp+=0.1;//移動速度を上昇させる
		//}
		//================================================================================================================



		



		







		//================================================
		//拡大縮小
		//================================================
			//if (GetKeyboardPress(DIK_RIGHT) == true)//拡大を行う
			//{//右キーが押された。
			//	g_fScale+=0.1f;
			//}
			//else if (GetKeyboardPress(DIK_LEFT) == true)//縮小を行う
			//{//左キーが押された
			//	g_fScale -= 0.1f;
			//}
			//else if (GetKeyboardPress(DIK_R) == true)//拡大率を元に戻す
			//{//Rキーが押された
			//	g_fScale = 1.0f;
			//}
			//
			//if (g_fScale >= 5.0f)//拡大率が5倍以上になったとき
			//{
			//	g_fScale = 5.0f;//拡大率の最大値を５倍に固定
			//}
			//if (g_fScale <= 0.1f)//縮小率が１０分の１以下になったとき
			//{
			//	g_fScale = 0.1f;//縮小率の最低値を１０分の１に固定。
			//}
			//============================================

			//===================================================
			//弾発射処理
			//===================================================
		if (GetKeyboardPress(DIK_F1) == true)
		{//F1キーが押された
			g_nMotionMode = 1;//トリガー発射モードにチェンジ
		}
		if (GetKeyboardPress(DIK_F2) == true)
		{//F2キーが押された
			g_nMotionMode = 2;//プレス発射モードにチェンジ
		}
		if (GetKeyboardPress(DIK_F3) == true)
		{//F3キーが押された
			g_nMotionMode = 3;//プレス発射モードにチェンジ

		}
		if (GetKeyboardPress(DIK_F4) == true)
		{//F4キーが押された
			g_nMotionMode = 4;//プレス発射モードにチェンジ

		}
		if (GetKeyboardPress(DIK_F5) == true)
		{//F5キーが押された
			g_nMotionMode = 5;//プレス発射モードにチェンジ

		}
		if (GetKeyboardPress(DIK_F6) == true)
		{//F6キーが押された
			g_nMotionMode = 6;//プレス発射モードにチェンジ

		}
		if (GetKeyboardPress(DIK_F7) == true)
		{//F7キーが押された
			g_nMotionMode = 7;//プレス発射モードにチェンジ

		}

		
		
			//===============================================================================================================================================

		//====================================
		//プレイヤーモーション処理
		//====================================

		g_aPlayer.nMotionTypeOld = g_aPlayer.nMotionType;

		Ability* pAbility = GetAbility();

		if (GetKeyboardTrigger(DIK_J) == true || GetJoyPadTrigger(JOYKEY_X) == true)
		{
			if (pAbility->nAbilityType == ABILITYTYPE_NORMAL)
			{
				PlaySound(SOUND_LABEL_SE_PUNCH);

				g_aPlayer.nMotionType = PLAYERMOTION_PUNCH;
				g_aPlayer.bUseMotion = true;
				SetAttack(D3DXVECTOR3(g_aPlayer.pos.x, g_aPlayer.pos.y, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ATTACKTYPE_PUNCH, -1, -1, -1, MOVETYPE_NORMAL
					, 120.0f, 120.0f, 20, 0.0f, -1);
			}
		}
		if (GetKeyboardTrigger(DIK_J) == true || GetJoyPadTrigger(JOYKEY_X) == true)
		{
			if (pAbility->nAbilityType == ABILITYTYPE_BULLET)
			{
				PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_000);
				if (g_aPlayer.bDirection == false)
				{
					SetBullet(D3DXVECTOR3(g_aPlayer.pos.x, g_aPlayer.pos.y - g_aPlayer.fHeight / 2, 0.0f), D3DXVECTOR3(sinf(-D3DX_PI * 1.5f) * 8.0f, cosf(-D3DX_PI * 1.5f) * 8.0f, 0.0f),
						GENERALLY_BULLETLIFE, BULLETTYPE_PLAYER, -1, -1, -1, true, MOVETYPE_NORMAL);
				}
				else if (g_aPlayer.bDirection == true)
				{
					SetBullet(D3DXVECTOR3(g_aPlayer.pos.x, g_aPlayer.pos.y - g_aPlayer.fHeight / 2, 0.0f), D3DXVECTOR3(sinf(-D3DX_PI * 0.5f) * 8.0f, cosf(-D3DX_PI * 0.5f) * 8.0f, 0.0f),
						GENERALLY_BULLETLIFE, BULLETTYPE_PLAYER, -1, -1, -1, true, MOVETYPE_NORMAL);
				}
			}
		}

		if (g_aPlayer.pos.x == g_aPlayer.posOld.x)
		{//横の移動量が０になった時
			g_aPlayer.nMotionType = PLAYERMOTION_STAND;
		}
		//===========================================================================================

	VERTEX_2D* pVtx;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);
	//pVtx[0].pos.x = g_aPlayer.pos.x - 70.0f;
	//pVtx[0].pos.y = g_aPlayer.pos.y - 100.0f;

	//頂点座標の設定
	//pVtx[0].pos = D3DXVECTOR3(fEnemyMove1[0],fEnemyMove1[2], 0.0f);
	//pVtx[1].pos = D3DXVECTOR3(fEnemyMove1[1],fEnemyMove1[2], 0.0f);
	//pVtx[2].pos = D3DXVECTOR3(fEnemyMove1[0],fEnemyMove1[3], 0.0f);
	//pVtx[3].pos = D3DXVECTOR3(fEnemyMove1[1],fEnemyMove1[3], 0.0f);

	/*pVtx[0].pos = D3DXVECTOR3(g_aPlayer.pos.x+fData1,g_aPlayer.pos.y+fData3, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aPlayer.pos.x+fData2,g_aPlayer.pos.y+fData3, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aPlayer.pos.x+fData1,g_aPlayer.pos.y+fData4, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aPlayer.pos.x+fData2,g_aPlayer.pos.y+fData4, 0.0f);*/

	//pVtx[0].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.rot.z - (D3DX_PI - g_fAnglePlayer)) * g_fLengthPlayer;//上から左に４５度
	//pVtx[0].pos.y = g_aPlayer.pos.y + cosf(g_aPlayer.rot.z - (D3DX_PI - g_fAnglePlayer)) * g_fLengthPlayer;
	//pVtx[0].pos.z = 0.0f;

	//pVtx[1].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.rot.z + (D3DX_PI - g_fAnglePlayer)) * g_fLengthPlayer;//上から右に４５度
	//pVtx[1].pos.y = g_aPlayer.pos.y + cosf(g_aPlayer.rot.z + (D3DX_PI - g_fAnglePlayer)) * g_fLengthPlayer;
	//pVtx[1].pos.z = 0.0f;


	//pVtx[2].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.rot.z - g_fAnglePlayer) * g_fLengthPlayer;//下から左に４５度
	//pVtx[2].pos.y = g_aPlayer.pos.y + cosf(g_aPlayer.rot.z - g_fAnglePlayer) * g_fLengthPlayer;
	//pVtx[2].pos.z = 0.0f;

	//pVtx[3].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.rot.z + g_fAnglePlayer) * g_fLengthPlayer;//下から右に４５度
	//pVtx[3].pos.y = g_aPlayer.pos.y + cosf(g_aPlayer.rot.z + g_fAnglePlayer) * g_fLengthPlayer;
	//pVtx[3].pos.z = 0.0f;

	pVtx[0].pos.x = g_aPlayer.pos.x - PLAYER_WIDTH;
	pVtx[0].pos.y = g_aPlayer.pos.y - PLAYER_HEIGHT;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_aPlayer.pos.x + PLAYER_WIDTH;
	pVtx[1].pos.y = g_aPlayer.pos.y - PLAYER_HEIGHT;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_aPlayer.pos.x - PLAYER_WIDTH;
	pVtx[2].pos.y = g_aPlayer.pos.y;//中点を下側につけるため
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_aPlayer.pos.x + PLAYER_WIDTH;
	pVtx[3].pos.y = g_aPlayer.pos.y;//中点を下側につけるため
	pVtx[3].pos.z = 0.0f;

	g_aPlayer.PosBeside = D3DXVECTOR3(g_aPlayer.pos.x + PLAYER_WIDTH, g_aPlayer.pos.y - PLAYER_HEIGHT, 0.0f);//プレイヤーの右端に、更に中点を追加


	



	if (g_aPlayer.bUseMotion == true)
	{
		g_aPlayer.nMotionType = PLAYERMOTION_PUNCH;
	}
	
	if (g_aPlayer.nMotionType != g_aPlayer.nMotionTypeOld)
	{//前に使っていたモーションと、今のモーションが一致しない場合、アニメーションカウンターを初期化する。
		g_aPlayer.nCounterAnim = 0;
	}

	if (g_aPlayer.nMotionType == PLAYERMOTION_PUNCH)
	{
		g_aPlayer.nCounterAnim++;//アニメーションカウンター
		g_aPlayer.nMotionTime++;
		g_aPlayer.nMotionType = PLAYERMOTION_PUNCH;
		

		if (g_aPlayer.nCounterAnim >= 5)
		{//アニメーションのスピード調整処理
			g_aPlayer.nPatternAnim++;
			g_aPlayer.nCounterAnim = 0;
		}

		if (g_aPlayer.nPatternAnim >= 5)
		{//ループ用処理
			g_aPlayer.nPatternAnim = 0;
		}

		if (g_aPlayer.bDirection == false)
		{
			pVtx[0].tex = D3DXVECTOR2(0.0f + g_aPlayer.nPatternAnim * 0.2f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.2f + g_aPlayer.nPatternAnim * 0.2f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + g_aPlayer.nPatternAnim * 0.2f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.2f + g_aPlayer.nPatternAnim * 0.2f, 1.0f);
		}
		else if (g_aPlayer.bDirection == true)
		{
			pVtx[1].tex = D3DXVECTOR2(0.0f + g_aPlayer.nPatternAnim * 0.2f, 0.0f);
			pVtx[0].tex = D3DXVECTOR2(0.2f + g_aPlayer.nPatternAnim * 0.2f, 0.0f);
			pVtx[3].tex = D3DXVECTOR2(0.0f + g_aPlayer.nPatternAnim * 0.2f, 1.0f);
			pVtx[2].tex = D3DXVECTOR2(0.2f + g_aPlayer.nPatternAnim * 0.2f, 1.0f);
		}

		if (g_aPlayer.nMotionTime >= 25)
		{
			g_aPlayer.nMotionType = PLAYERMOTION_STAND;
			g_aPlayer.nPatternAnim = 0;
			g_aPlayer.nCounterAnim = 0;//アニメーションカウンター
			g_aPlayer.nMotionTime = 0;
			g_aPlayer.bUseMotion = false;
		}

	}
	else if (g_aPlayer.nMotionType == PLAYERMOTION_STAND)
	{//待機モーション
		g_aPlayer.nCounterAnim++;//アニメーションカウンター
		if (g_aPlayer.nCounterAnim >= 10)
		{
			g_aPlayer.nPatternAnim++;
			g_aPlayer.nCounterAnim = 0;
		}

		if (g_aPlayer.nPatternAnim >= 4)
		{
			g_aPlayer.nPatternAnim = 0;
		}

		if (g_aPlayer.bDirection == false)
		{
			pVtx[0].tex = D3DXVECTOR2(0.0f + g_aPlayer.nPatternAnim * 0.25f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.25f + g_aPlayer.nPatternAnim * 0.25f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + g_aPlayer.nPatternAnim * 0.25f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.25f + g_aPlayer.nPatternAnim * 0.25f, 1.0f);
		}
		else if (g_aPlayer.bDirection == true)
		{
			pVtx[1].tex = D3DXVECTOR2(0.0f + g_aPlayer.nPatternAnim * 0.25f, 0.0f);
			pVtx[0].tex = D3DXVECTOR2(0.25f + g_aPlayer.nPatternAnim * 0.25f, 0.0f);
			pVtx[3].tex = D3DXVECTOR2(0.0f + g_aPlayer.nPatternAnim * 0.25f, 1.0f);
			pVtx[2].tex = D3DXVECTOR2(0.25f + g_aPlayer.nPatternAnim * 0.25f, 1.0f);
		}
	}
	else if (g_aPlayer.nMotionType == PLAYERMOTION_MOVE)
	{//移動モーション
		g_aPlayer.nCounterAnim++;//アニメーションカウンター
		if (g_aPlayer.nCounterAnim >= 5)
		{
			g_aPlayer.nPatternAnim++;
			g_aPlayer.nCounterAnim = 0;
		}

		if (g_aPlayer.nPatternAnim >= 8)
		{
			g_aPlayer.nPatternAnim = 0;
		}

		if (g_aPlayer.bDirection == false)
		{
			pVtx[0].tex = D3DXVECTOR2(0.0f + g_aPlayer.nPatternAnim * 0.125f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.125f + g_aPlayer.nPatternAnim * 0.125f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + g_aPlayer.nPatternAnim * 0.125f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.125f + g_aPlayer.nPatternAnim * 0.125f, 1.0f);
		}
		else if (g_aPlayer.bDirection == true)
		{
			pVtx[1].tex = D3DXVECTOR2(0.0f + g_aPlayer.nPatternAnim * 0.125f, 0.0f);
			pVtx[0].tex = D3DXVECTOR2(0.125f + g_aPlayer.nPatternAnim * 0.125f, 0.0f);
			pVtx[3].tex = D3DXVECTOR2(0.0f + g_aPlayer.nPatternAnim * 0.125f, 1.0f);
			pVtx[2].tex = D3DXVECTOR2(0.125f + g_aPlayer.nPatternAnim * 0.125f, 1.0f);
		}
	}


	switch(g_aPlayer.state)
		{
	case PLAYERSTATE_NORMAL:
			break;
	case PLAYERSTATE_DAMAGE:
		g_aPlayer.nCounterState--;
		if (g_aPlayer.nCounterState >= 1)
		{
			g_aPlayer.bInvincibility = true;//ダメージ状態なら、無敵状態になる。

			if (g_aPlayer.nCounterState % 2 == 0)
			{
				g_aPlayer.bDisp = false;//プレイヤーの描画を消したり表したりすることにより、点滅を表現。
			}
			else
			{
				g_aPlayer.bDisp = true;//プレイヤーの描画を消したり表したりすることにより、点滅を表現。
				pVtx[0].col = D3DCOLOR_RGBA(255,0,0,255);
				pVtx[1].col = D3DCOLOR_RGBA(255,0,0,255);
				pVtx[2].col = D3DCOLOR_RGBA(255,0,0,255);
				pVtx[3].col = D3DCOLOR_RGBA(255,0,0,255);
			}

		}
		if (g_aPlayer.nCounterState <= 0)
		{
			g_aPlayer.bDisp = true;
			g_aPlayer.bInvincibility = false;//無敵状態解除。
			g_aPlayer.state = PLAYERSTATE_NORMAL;
			pVtx[0].col = D3DCOLOR_RGBA(255,255,255,255);
			pVtx[1].col = D3DCOLOR_RGBA(255,255,255,255);
			pVtx[2].col = D3DCOLOR_RGBA(255,255,255,255);
			pVtx[3].col = D3DCOLOR_RGBA(255,255,255,255);
		}
		break;
	case PLAYERSTATE_DEATH:
		g_aPlayer.nCounterState--;
		if (g_aPlayer.nCounterState <= 0)
		{
			return;//関数を終わらせるやつ
		}
		
		
	}/*if (pTutorial->bTutorialUse == true)
	{
		if (g_aPlayer.pos.x <= g_StartPos.x - 50.0f)
		{
			g_aPlayer.pos.x = g_StartPos.x - 50.0f;
		}
		if (g_aPlayer.pos.x >= g_StartPos.x + 150.0f)
		{
			g_aPlayer.pos.x = g_StartPos.x + 150.0f;
		}
		if (g_aPlayer.pos.y <= g_StartPos.y)
		{
			g_aPlayer.pos.y = g_StartPos.y;
		}
		if (g_aPlayer.pos.y >= g_StartPos.y + 140.0f)
		{
			g_aPlayer.pos.y = g_StartPos.y +140.0f;
		}
	}*/

	//if (g_nAnimationDelay == 5)//遅延
	//{// 遅延カウントが5に到達した
	//	g_nAnimation++;				// Uの座標をインクリメント
	//	g_nAnimationDelay = 0;		// 遅延カウントをリセット
	//}

	//if ((g_nAnimation+1) % 5 == 0)
	//{// 一番右のコマを描写した
	//	g_nAnimationCount++;		// Vの座標をインクリメント
	//}

	//if (g_nAnimationCount > 1)
	//{// 一番下
	//	g_nAnimationCount = 0;		// Vの座標をリセット
	//}

	//if (g_nAnimation >= 4)
	//{// 一番右
	//	g_nAnimation = 0;			// Uの座標をリセット
	//}

	//g_nAnimationDelay++;//アニメーションの速度を遅くする変数

	//頂点バッファをアンロックする
	g_pVtxBuffPolygon->Unlock();
}
//====================================================
//ポリゴンの描画処理
//==========================================================
	void DrawPlayer(void)
	{
		LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

		//デバイスの取得
		pDevice = GetDevice();

		if (g_aPlayer.bDisp == true)
		{
			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffPolygon, 0, sizeof(VERTEX_2D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_2D);

			//テクスチャの設定
			if (g_aPlayer.nMotionType == PLAYERMOTION_STAND)
			{
				pDevice->SetTexture(0, g_pTexturePolygon[PLAYERMOTION_STAND]);
			}
			else if (g_aPlayer.nMotionType == PLAYERMOTION_MOVE)
			{
				pDevice->SetTexture(0, g_pTexturePolygon[PLAYERMOTION_MOVE]);
			}
			else if (g_aPlayer.nMotionType == PLAYERMOTION_PUNCH)
			{
				pDevice->SetTexture(0, g_pTexturePolygon[PLAYERMOTION_PUNCH]);
			}

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);//0は、描画する最初の頂点インデックスで、2は、描画するプリミティブ数
				//&g_pVtxBuffPolygon[0],//頂点情報の先頭アドレス
				//sizeof(VERTEX_2D));//頂点情報構造体のサイズ
		}
	}

	//プレイヤーのヒット処理
	void HitPlayer(int nDamage)
	{
		int nCntDisp;
		VERTEX_2D* pVtx;
		//頂点バッファをロックし、頂点情報へのポインタを取得
		//g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

		////Health* pHealth = GetHealth();//プレイヤーの体力の情報を取得する。

		//pHealth->fHealth += nDamage;

		//g_aPlayer.nHitCount++;
		//if (pHealth->fHealth >= 3)
		//{//プレイヤーの体力がなくなったら...
		//	SetExplosion(g_aPlayer.pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f), EXPLOSIONTYPE_NORMAL);
		//	g_aPlayer.bDisp = false;
		//	g_aPlayer.state = PLAYERSTATE_DEATH;
		//	g_aPlayer.nCounterState = 60;
		//}
		//else
		//{
		//	g_aPlayer.state = PLAYERSTATE_DAMAGE;
		//	
		//	g_aPlayer.nCounterState = 100;
		//}
		////頂点バッファをアンロックする
		//g_pVtxBuffPolygon->Unlock();
	}
	//プレイヤーの取得
	Player* GetPlayer(void)
	{
		return &g_aPlayer;
		
	}

	void SetPlayer(D3DXVECTOR3 pos)
	{
		g_aPlayer.pos = pos;
		g_aPlayer.posOld = pos;
	}