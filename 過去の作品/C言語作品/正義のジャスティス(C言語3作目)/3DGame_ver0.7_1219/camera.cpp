//========================================================
//
//９月２６日：ハッカソンの基盤作り[Camera.cpp]
//Author:ShinaTaiyo
//
//=========================================================

#include "camera.h"
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "player.h"
//グローバル変数
//PDIRECT3DVERTEXBUFFER9 g_pVtxBuffCamera = NULL;//頂点バッファへのポインタ
//LPDIRECT3DTEXTURE9 g_pTextureCamera = NULL;//テクスチャへのポインタ
//D3DXVECTOR3 g_posCamera;//位置
//D3DXVECTOR3 g_rotCamera;//向き
//D3DXMATRIX g_mtxWorldCamera;//ワールドマトリックス(float4*4)

Camera g_Camera;//カメラの情報

float g_fCameraPosVX;//カメラのXの支点の位置を変える
float g_fCameraPosVY;//カメラのYの支点の位置を変える
float g_fCameraPosVZ;//カメラのZの支点の位置を変える

float g_fLength;//カメラとプレイヤーの距離
float g_fAngle;//カメラとプレイヤーの角度

//タイトル画面の初期化処理
void InitCamera(void)
{
	Player* pPlayer = GetPlayer();

	g_fLength = 0.0f;
	g_fAngle = 0.0f;
	g_fCameraPosVX = 0.0f;//カメラのXの支点の位置を変える
	g_fCameraPosVY = 350.0f;//カメラのYの支点の位置を変える
	g_fCameraPosVZ = 0.0f;//カメラのZの支点の位置を変える

	g_Camera.PosV = D3DXVECTOR3(pPlayer->pos.x, 200.0f,400.0f);//後ろから見るので、プレイヤーのXの初期地点を決めれば大丈夫（Y,Zは定数(視点からの距離)
	g_Camera.PosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Camera.targetPosV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//目的のカメラの視点
	g_Camera.targetPosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//目的のカメラの注視点
	g_Camera.VecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	g_Camera.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Camera.fXVaim = 0.0f;
	g_Camera.fZVaim = 0.0f;
	//posV.yとposV.zが注視点からの距離になるため（カメラは中止点の後ろから見るから）、posV.yとposV.zをsqrtfして,ちゃんとした距離を求める。
	g_Camera.fLength = 550.0f /*sqrtf(g_Camera.PosV.y * g_Camera.PosV.y + g_Camera.PosV.z * g_Camera.PosV.z) / 2*/;
	g_Camera.nMode = CAMERAMODE_REVOLUTION;//公転モードに初期化する
}

//==============================
//タイトル画面の終了処理
//==============================
void UninitCamera(void)
{
	
}
//============================
//タイトル画面の更新処理
//============================
void UpdateCamera(void)
{

	////対角線の長さを算出する
	//g_fLengthPlayer = sqrtf(PLAYER_WIDTH * PLAYER_WIDTH + PLAYER_HEIGHT * PLAYER_HEIGHT) / 2.0f;//プレイヤーの幅2乗＋プレイヤーの高さ２乗/２

	////対角線の角度を算出する
	//g_fAnglePlayer = atan2f(PLAYER_WIDTH, PLAYER_HEIGHT);
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

	Player* pPlayer = GetPlayer();

	
	//g_Camera.rot.y;

	/*g_Camera.PosV = D3DXVECTOR3(pPlayer->pos.x + g_fCameraPosVX, 200.0f + pPlayer->pos.y + g_fCameraPosVY,-200.0f + pPlayer->pos.z + g_fCameraPosVZ);
	g_Camera.PosR = D3DXVECTOR3(pPlayer->pos.x,pPlayer->pos.y,pPlayer->pos.z);
	g_Camera.VecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);*/


	//g_fXVaim = pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x;//X座標へのベクトル
	//g_fYVaim = pPlayer->pos.y - g_aEnemy[nCntEnemy].pos.y;//Y座標へのベクトル
	//g_Vaim[nCntEnemy] = atan2(g_fXVaim[nCntEnemy], g_fYVaim[nCntEnemy]);//弾を撃つ方向の決定

	//g_Camera;
	
//	g_Camera.PosV;
//
	//g_Camera.PosV = D3DXVECTOR3(sinf(-D3DX_PI * g_Camera.rot.y) * g_fLength, 200.0f, cosf(-D3DX_PI * g_Camera.rot.y) * g_fLength);
	//g_Camera.PosR = D3DXVECTOR3(0.0f,0.0f,0.0f);
	//g_Camera.VecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//if (GetKeyboardPress(DIK_RIGHT) == true)
	//{
	//	g_fCameraPosVX += 5.0f;
	//	//g_Camera.PosV.x++;
	//}
	//if (GetKeyboardPress(DIK_LEFT) == true)
	//{
	//	g_fCameraPosVX -= 5.0f;
	//	//g_Camera.PosV.x--;
	//}
	//if (GetKeyboardPress(DIK_UP) == true)
	//{
	//	g_fCameraPosVZ += 5.0f;
	//	//g_Camera.PosV.z++;
	//}
	//if (GetKeyboardPress(DIK_DOWN) == true)
	//{
	//	g_fCameraPosVZ -= 5.0f;
	//	//g_Camera.PosV.z--;
	//}
	if (GetKeyboardPress(DIK_O) == true)
	{
		g_fCameraPosVY += 5.0f;
	/*	g_Camera.PosV.y++;*/
	}
	if (GetKeyboardPress(DIK_L) == true)
	{
		g_fCameraPosVY -= 5.0f;
	/*	g_Camera.PosV.y--;*/
	}
	if (GetKeyboardPress(DIK_U) == true || GetJoypadPress(JOYKEY_R1) == true)
	{
	/*	g_Camera.rot.y += 0.1f;*/
		g_Camera.move.y += 0.1f;
	}
	if (GetKeyboardPress(DIK_J) == true || GetJoypadPress(JOYKEY_L1) == true)
	{
	/*	g_Camera.rot.y += -0.1f;*/
		g_Camera.move.y += -0.1f;
	}

	if (GetKeyboardTrigger(DIK_C) == true)
	{
		g_Camera.nMode++;
	}

	if (GetKeyboardPress(DIK_Z) == true)
	{
		g_Camera.fLength += 10.0f;
	}
	if (GetKeyboardPress(DIK_X) == true)
	{
		g_Camera.fLength -= 10.0f;
	}


	if (g_Camera.nMode == 2)
	{
		g_Camera.nMode = 0;
	}

	//////移動量を更新（減衰させる）
 //   g_Camera.move.x += (0.0f - g_Camera.rot.y) * 0.1f;

	//g_aPlayer.move.y += -2.5f;
	//g_aPlayer.move.y += (0.0f - g_aPlayer.move.y) * 0.5f;
	//g_aPlayer.move.z += (0.0f - g_aPlayer.move.z) * 0.5f;
	//g_aPlayer.move.y += (0.0f - g_aPlayer.move.y) * 0.5f;

	g_Camera.move.y += (0.0f - g_Camera.move.y) * 0.5f;

	g_Camera.rot.y += g_Camera.move.y;




	//float fLengthX = (g_Camera.PosR.x - g_fCameraPosVX);
	//float fLengthZ = (g_Camera.PosR.z - g_fCameraPosVZ);
	//g_Camera.fLength = sqrt(fLengthX * fLengthX + fLengthZ * fLengthZ) / 2;//距離を出す！

	if (g_Camera.nMode == CAMERAMODE_REVOLUTION)
	{
		g_Camera.targetPosR = D3DXVECTOR3(pPlayer->pos.x, pPlayer->pos.y, pPlayer->pos.z);//注視点
		g_Camera.targetPosV = D3DXVECTOR3(sinf(D3DX_PI + g_Camera.rot.y) * g_Camera.fLength + g_Camera.PosR.x,//回転させるということは、Zの位置だけでなく、Xの位置も
																									   //変わるということ。故に、この式を使う。視点は注視点からの距離
			200.0f + g_Camera.PosR.y + g_fCameraPosVY,
			cosf(D3DX_PI + g_Camera.rot.y) * g_Camera.fLength + g_Camera.PosR.z);//視点
		g_Camera.VecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);//上方向ベクトル


		//===============
		//慣性移動処理
		//===============
		g_Camera.PosR.x += (g_Camera.targetPosR.x - g_Camera.PosR.x) * 0.1f;
		g_Camera.PosR.y += (g_Camera.targetPosR.y - g_Camera.PosR.y) * 0.1f;
		g_Camera.PosR.z += (g_Camera.targetPosR.z - g_Camera.PosR.z) * 0.1f;

		g_Camera.PosV.x += (g_Camera.targetPosV.x - g_Camera.PosV.x) * 0.1f;
		g_Camera.PosV.y += (g_Camera.targetPosV.y - g_Camera.PosV.y) * 0.1f;
		g_Camera.PosV.z += (g_Camera.targetPosV.z - g_Camera.PosV.z) * 0.1f;
		//==========================================================================
	}
	
	if (g_Camera.nMode == CAMERAMODE_ROTATION)
	{
		g_Camera.PosR = D3DXVECTOR3(sinf(D3DX_PI + g_Camera.rot.y) * g_Camera.fLength + g_Camera.PosV.x,//回転させるということは、Zの位置だけでなく、Xの位置も
																										   //変わるということ。故に、この式を使う。視点は注視点からの距離
		    0.0f,
			cosf(D3DX_PI + g_Camera.rot.y) * g_Camera.fLength + g_Camera.PosV.z);//視点
		g_Camera.PosV = g_Camera.PosV;//回転させるということは、Zの位置だけでなく、Xの位置も
		g_Camera.VecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);//上方向ベクトル
	}

}



//================================
//タイトル画面の描画処理
//================================
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

		//デバイスの取得
	pDevice = GetDevice();

	//プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&g_Camera.mtxProjection);

	//プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(&g_Camera.mtxProjection,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		10.0f,
		4500.0f);

	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_Camera.mtxProjection);

	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&g_Camera.mtxView);

	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&g_Camera.mtxView,
		&g_Camera.PosV,
		&g_Camera.PosR,
		&g_Camera.VecU);

	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &g_Camera.mtxView);

	
}

//====================
//カメラ情報の取得
//====================
Camera* GetCamera(void)
{
	return &g_Camera;
}

//void SetPosCamera(D3DXVECTOR3 PosR, D3DXVECTOR3 PosV)
//{
//	g_Camera.targetPosR = PosR;
//	g_Camera.targetPosV = PosV;
//}
