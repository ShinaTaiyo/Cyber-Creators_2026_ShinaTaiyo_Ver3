//=======================================================================================================================================================================================================================
//
// カメラの処理
// Author : Atsumu Kuboichi
//
//=======================================================================================================================================================================================================================
#include "editcamera.h"
#include "player.h"
#include "game.h"
#include "input.h"
#include "edit.h"

//=======================================================================================================================================================================================================================
// グローバル変数
//=======================================================================================================================================================================================================================
EditCamera g_EditCamera;	//カメラの情報

//=======================================================================================================================================================================================================================
// 初期化処理
//=======================================================================================================================================================================================================================
void InitEditCamera(void)
{
	g_EditCamera.posV = D3DXVECTOR3(0.0f, EDITCAMERA_POSV_Y, EDITCAMERA_POSV_Z);	//視点位置
	g_EditCamera.posR = D3DXVECTOR3(0.0f, 140.0f, 0.0f);	//注視点位置
	g_EditCamera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		//ベクトル(上方向)
	g_EditCamera.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量
	g_EditCamera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き
	g_EditCamera.nTurn = 1;									//回転方法
}

//=======================================================================================================================================================================================================================
// 終了処理
//=======================================================================================================================================================================================================================
void UninitEditCamera(void)
{

}

//=======================================================================================================================================================================================================================
// 更新処理
//=======================================================================================================================================================================================================================
void UpdateEditCamera(void)
{
	//視点から注視点までの距離
	g_EditCamera.fLength = sqrtf(g_EditCamera.posV.y * g_EditCamera.posV.y + g_EditCamera.posV.z * g_EditCamera.posV.z) / 2.0f;

	//============================================================================
	// 回転
	//============================================================================
	if (GetKeyboardPress(DIK_RIGHT) == true)//→キーが押された場合
	{//右回転
		g_EditCamera.rot.y += EDITCAMERA_ROLL;
	}
	if (GetKeyboardPress(DIK_LEFT) == true)	//←キーが押された場合
	{//左回転
		g_EditCamera.rot.y -= EDITCAMERA_ROLL;
	}
	if (GetKeyboardPress(DIK_UP) == true) //↑キーが押された
	{//上回転
		g_EditCamera.rot.x += EDITCAMERA_ROLL / 2.0f;
	}
	if (GetKeyboardPress(DIK_DOWN) == true) //↓キーが押された
	{//下回転
		g_EditCamera.rot.x -= EDITCAMERA_ROLL / 2.0f;
	}

	//回転数値の調整
	if (g_EditCamera.rot.x > D3DX_PI * 0.5f)
	{
		g_EditCamera.rot.x = D3DX_PI * 0.5f - 0.01f;
	}
	if (g_EditCamera.rot.x < -D3DX_PI * 0.5f)
	{
		g_EditCamera.rot.x = -D3DX_PI * 0.5f + 0.01f;
	}

	//マウスの情報を取得
	DIMOUSESTATE pMouse = GetMouse();

	g_EditCamera.rot.x -= EDITCAMERA_ROLL * pMouse.lY * MOUSE_SINCE;//X軸回転
	g_EditCamera.rot.y += EDITCAMERA_ROLL * pMouse.lX * MOUSE_SINCE;//Y軸回転

	//============================================================================
	// 移動
	//============================================================================

	//上移動(Z軸)
	if (GetKeyboardPress(DIK_W) == true) //Wキーが押された
	{
		//左上移動(Z軸)
		if (GetKeyboardPress(DIK_A) == true) //Aキーが押された
		{
			//移動量を加算
			g_EditCamera.move.x += sinf(g_EditCamera.rot.y + D3DX_PI * -0.25f) * EDITCAMERA_SPEED;	//X軸
			g_EditCamera.move.z += cosf(g_EditCamera.rot.y + D3DX_PI * -0.25f) * EDITCAMERA_SPEED;	//Z軸
		}
		//右上移動(Z軸)
		else if (GetKeyboardPress(DIK_D) == true) //Dキーが押された
		{
			//移動量を加算
			g_EditCamera.move.x += sinf(g_EditCamera.rot.y + D3DX_PI * 0.25f) * EDITCAMERA_SPEED;	//X軸
			g_EditCamera.move.z += cosf(g_EditCamera.rot.y + D3DX_PI * 0.25f) * EDITCAMERA_SPEED;	//Z軸
		}
		//真上移動(Z軸)
		else
		{
			//移動量を加算
			g_EditCamera.move.x -= sinf(g_EditCamera.rot.y + D3DX_PI) * EDITCAMERA_SPEED;	//X軸
			g_EditCamera.move.z -= cosf(g_EditCamera.rot.y + D3DX_PI) * EDITCAMERA_SPEED;	//Z軸
		}
	}
	//下移動(Z軸)
	else if (GetKeyboardPress(DIK_S) == true) //Sキーが押された
	{
		//左下移動(Z軸)
		if (GetKeyboardPress(DIK_A) == true) //Aキーが押された
		{
			//移動量を加算
			g_EditCamera.move.x += sinf(g_EditCamera.rot.y + D3DX_PI * -0.75f) * EDITCAMERA_SPEED;	//X軸
			g_EditCamera.move.z += cosf(g_EditCamera.rot.y + D3DX_PI * -0.75f) * EDITCAMERA_SPEED;	//Z軸
		}
		//右下移動(Z軸)
		else if (GetKeyboardPress(DIK_D) == true) //Dキーが押された
		{
			//移動量を加算
			g_EditCamera.move.x += sinf(g_EditCamera.rot.y + D3DX_PI * 0.75f) * EDITCAMERA_SPEED;	//X軸
			g_EditCamera.move.z += cosf(g_EditCamera.rot.y + D3DX_PI * 0.75f) * EDITCAMERA_SPEED;	//Z軸
		}
		//真下移動(Z軸)
		else
		{
			//移動量を加算
			g_EditCamera.move.x += sinf(g_EditCamera.rot.y + D3DX_PI) * EDITCAMERA_SPEED;	//X軸
			g_EditCamera.move.z += cosf(g_EditCamera.rot.y + D3DX_PI) * EDITCAMERA_SPEED;	//Z軸
		}
	}
	//上移動(Y軸)
	else if (GetKeyboardPress(DIK_Z) == true) //Wキーが押された
	{
		//移動量を加算
		g_EditCamera.move.y += EDITCAMERA_SPEED;
	}
	//下移動(Y軸)
	else if (GetKeyboardPress(DIK_X) == true) //Sキーが押された
	{
		//移動量を加算
		g_EditCamera.move.y -= EDITCAMERA_SPEED;
	}
	//左移動
	else if (GetKeyboardPress(DIK_A) == true) //Aキーが押された
	{
		//移動量を加算
		g_EditCamera.move.x += sinf(g_EditCamera.rot.y + D3DX_PI / -2.0f) * EDITCAMERA_SPEED;	//X軸
		g_EditCamera.move.z += cosf(g_EditCamera.rot.y + D3DX_PI / -2.0f) * EDITCAMERA_SPEED;	//Z軸
	}
	//右移動
	else if (GetKeyboardPress(DIK_D) == true) //Dキーが押された
	{
		//移動量を加算
		g_EditCamera.move.x += sinf(g_EditCamera.rot.y + D3DX_PI / 2.0f) * EDITCAMERA_SPEED;	//X軸
		g_EditCamera.move.z += cosf(g_EditCamera.rot.y + D3DX_PI / 2.0f) * EDITCAMERA_SPEED;	//Z軸
	}

	//移動量の加算
	g_EditCamera.posV += g_EditCamera.move;

	//移動量を減衰
	g_EditCamera.move.x += (0.0f - g_EditCamera.move.x);
	g_EditCamera.move.y += (0.0f - g_EditCamera.move.y);
	g_EditCamera.move.z += (0.0f - g_EditCamera.move.z);

	//注視点の位置の更新
	g_EditCamera.posR.x = sinf(g_EditCamera.rot.y) * g_EditCamera.fLength + g_EditCamera.posV.x;
	g_EditCamera.posR.y = tanf(g_EditCamera.rot.x) * g_EditCamera.fLength + g_EditCamera.posV.y;
	g_EditCamera.posR.z = cosf(g_EditCamera.rot.y) * g_EditCamera.fLength + g_EditCamera.posV.z;
}

//=======================================================================================================================================================================================================================
// 設定処理
//=======================================================================================================================================================================================================================
void SetEditCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスの取得

	//プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&g_EditCamera.mtxProjection);

	//プロジェクションマトリックスを生成
	D3DXMatrixPerspectiveFovLH(&g_EditCamera.mtxProjection, D3DXToRadian(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 1.0f, 5000.0f);

	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_EditCamera.mtxProjection);

	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&g_EditCamera.mtxView);

	//ビューマトリックスの生成
	D3DXMatrixLookAtLH(&g_EditCamera.mtxView, &g_EditCamera.posV, &g_EditCamera.posR, &g_EditCamera.vecU);

	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &g_EditCamera.mtxView);
}

//=======================================================================================================================================================================================================================
// 対象のオブジェクトに移動
//=======================================================================================================================================================================================================================
void SetPosEditCamera(int nCnt)
{
	Edit* pEdit = GetEdit();//エディタの構造体の情報の取得

	//対象のオブジェクトまでカウントを進める
	for (int nCntCamera = 0; nCntCamera < nCnt; nCntCamera++)
	{
		pEdit++;
	}

	//視点の位置の更新
	g_EditCamera.posV.x = pEdit->pos.x;
	g_EditCamera.posV.y = pEdit->pos.y + 200.0f;
	g_EditCamera.posV.z = pEdit->pos.z - 400.0f;

	//注視点の位置の更新
	g_EditCamera.posR = pEdit->pos;

	//対象のオブジェクトからカメラの対角線の角度を求める
	float fAngle = atan2f((pEdit->pos.y - g_EditCamera.posV.y), (pEdit->pos.z - g_EditCamera.posV.z));

	//向きをリセット
	g_EditCamera.rot = D3DXVECTOR3(fAngle, 0.0f, 0.0f);
}

//=======================================================================================================================================================================================================================
// 情報の取得
//=======================================================================================================================================================================================================================
EditCamera* GetEditCamera(void)
{
	return &g_EditCamera;
}
