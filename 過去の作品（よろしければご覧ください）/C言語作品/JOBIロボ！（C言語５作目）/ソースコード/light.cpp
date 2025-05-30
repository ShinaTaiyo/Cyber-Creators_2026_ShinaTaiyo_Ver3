//========================================================
//
//９月２６日：ハッカソンの基盤作り[Light.cpp]
//Author:ShinaTaiyo
//
//=========================================================

#include "light.h"
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
//グローバル変数
//PDIRECT3DVERTEXBUFFER9 g_pVtxBuffLight = NULL;//頂点バッファへのポインタ
//LPDIRECT3DTEXTURE9 g_pTextureLight = NULL;//テクスチャへのポインタ
//D3DXVECTOR3 g_posLight;//位置
//D3DXVECTOR3 g_rotLight;//向き
//D3DXMATRIX g_mtxWorldLight;//ワールドマトリックス(float4*4)

#define MAX_LIGHT (3) //ライトの最大数

D3DLIGHT9 g_light[MAX_LIGHT];//ライトの情報


//タイトル画面の初期化処理
void InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	D3DXVECTOR3 vecDir[MAX_LIGHT];//設定用方向ベクトル

	for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{
		//ライトの情報をクリアする
		ZeroMemory(&g_light[nCntLight], sizeof(D3DLIGHT9));

		//ライトの種類を設定
		g_light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

		if (GetMode() != MODE_TITLE)
		{
			switch (nCntLight)
			{

			case 0:
				//ライトの拡散光を設定
				g_light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				//ライトの方向を設定
				vecDir[nCntLight] = D3DXVECTOR3(0.22f, -0.97f, 0.54f);
				break;
			case 1:
				//ライトの拡散光を設定
				g_light[nCntLight].Diffuse = D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f);
				//ライトの方向を設定
				vecDir[nCntLight] = D3DXVECTOR3(-0.58f, 0.88f, -0.44f);
				break;
			case 2:
				//ライトの拡散光を設定
				g_light[nCntLight].Diffuse = D3DXCOLOR(0.15f, 0.15f, 0.15f, 1.0f);
				//ライトの方向を設定
				vecDir[nCntLight] = D3DXVECTOR3(0.89f, -0.11f, 0.54f);
				break;
			}
		}
		else
		{
			switch (nCntLight)
			{

			case 0:
				//ライトの拡散光を設定
				g_light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				//ライトの方向を設定
				vecDir[nCntLight] = D3DXVECTOR3(0.22f, -0.97f, -0.54f);
				break;
			case 1:
				//ライトの拡散光を設定
				g_light[nCntLight].Diffuse = D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f);
				//ライトの方向を設定
				vecDir[nCntLight] = D3DXVECTOR3(-0.38f, 0.88f, -0.44f);
				break;
			case 2:
				//ライトの拡散光を設定
				g_light[nCntLight].Diffuse = D3DXCOLOR(0.15f, 0.15f, 0.15f, 1.0f);
				//ライトの方向を設定
				vecDir[nCntLight] = D3DXVECTOR3(0.89f, -0.11f, -0.54f);
				break;
			}
		}
		
		D3DXVec3Normalize(&vecDir[nCntLight], &vecDir[nCntLight]);//ベクトルを正規化する（長さを1にする）
		g_light[nCntLight].Direction = vecDir[nCntLight];

		//ライトを設定する（設置）
		pDevice->SetLight(nCntLight, &g_light[nCntLight]);

		//ライトを有効にする（スイッチをオンにする）
		pDevice->LightEnable(nCntLight, TRUE);
	}
}

//==============================
//タイトル画面の終了処理
//==============================
void UninitLight(void)
{
	
}
//============================
//タイトル画面の更新処理
//============================
void UpdateLight(void)
{

}