//=============================================
//
//スクリーン座標変換処理[camera.cpp]
//Author:ShinaTaiyo
//
//=============================================

#include "main.h"
#include "camera.h"



D3DXVECTOR2 g_CameraPos;

// カメラ

//=============================
//初期化処理
//=============================
void InitCamera(void)
{
	static D3DXVECTOR2 g_CameraPos{ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
}

//=============================
//終了処理
//=============================
void UninitCamera(void)
{

}

//=============================
//更新処理
//=============================
void UpdateCamera(void)
{
	// カメラ座標からスクリーン座標の原点に変換する
	D3DXVECTOR2 screen_origin_position =
	{
		g_CameraPos.x - SCREEN_WIDTH / 2.0f,
		g_CameraPos.y - SCREEN_HEIGHT / 2.0f
	};
}

//=============================
//描画処理
//=============================
void DrawCamera(void)
{
	// ワールド座標からスクリーン座標に変換する
	//D3DXVECTOR2 screen_position = (pos.X - screen_origin_position.X, pos.Y - screen_origin_position.Y);
}
