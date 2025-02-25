//==========================================
//
//５月２０日：基盤のクラス分け
//Author:ShinaTaiyo
//
//==========================================

//===========================
//インクルード
//===========================
#include "renderer.h"
#include "object.h"
#include "object2d.h"
#include "player.h"
#include "bg.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "manager.h"
#include "camera.h"
#include "scene.h"
#include "game.h"
#include "debugtext.h"
#include "edit.h"
//==========================================

//===========================
//静的メンバ
//===========================
const int CRenderer::m_nMAXDEBUG = 2;//デバッグ表示最大数
//==========================================

//===========================
//コンストラクタ
//===========================
CRenderer::CRenderer() : m_pFont(),m_pD3D(),m_pD3DDevice(),m_mode(MODE_TITLE)
{
}
//==============================================================================

//===========================
//デストラクタ
//===========================
CRenderer::~CRenderer()
{

}
//================================================================================

//======================================================================
//初期化処理
//======================================================================
HRESULT CRenderer::Init(HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;//ディスプレイモード
	D3DPRESENT_PARAMETERS d3dpp;//プレゼンテーションパラメータ

	m_pD3DDevice = NULL;

	//DirectX3Dオブジェクトの生成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}
	//現在のディスプレイモードを取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}
	//デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));//パラメータのゼロクリア
	d3dpp.BackBufferWidth = SCREEN_WIDTH;//ゲーム画面サイズ（幅）
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;//ゲーム画面サイズ（高さ）
	d3dpp.BackBufferFormat = d3ddm.Format;//バックバッファの形式
	d3dpp.BackBufferCount = 1;//バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;//ダブルバッファの切り替え（映像信号に同期）
	d3dpp.EnableAutoDepthStencil = TRUE;//デプスバッファとステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;//デプスバッファとして16bitを使う
	d3dpp.Windowed = bWindow;//ウインドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;//リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;//インターバル

	//Direct3Dデバイスの生成（描画処理と頂点処理をハードウェアで行う）
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&m_pD3DDevice)))
	{
		//Direct3Dデバイスの生成（描画処理はハードウェア、頂点処理はCPUで行う
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&m_pD3DDevice)))
		{
			//Direct3Dデバイスの生成（描画処理と頂点処理をCPUで行う）
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&m_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	//レンダーステートの設定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//サンプラーステートの設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//テクスチャステージステートの設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);


	//デバック表示用フォントの生成
	D3DXCreateFont(m_pD3DDevice, 18, 0, 0, 0,
		FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,
		"terminal", &m_pFont);

	srand((unsigned int)time(NULL));//ゲームを開始した時間毎に乱数の種を設定

	////モードの設定
	SetMode(m_mode);
	return S_OK;
}
//===================================================================================================================================

//=============================
//終了処理
//=============================
void CRenderer::Uninit()
{
	//デバック表示用フォントの破棄
	if (m_pFont != NULL)
	{
		m_pFont->Release();
		m_pFont = NULL;
	}


	//Direct3Dデバイスの破棄
	if (m_pD3DDevice != NULL)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}

	//Direct3Dオブジェクトの破棄
	if (m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}
}
//===============================================================================

//=============================
//更新処理
//=============================
void CRenderer::Update()
{
	CObject::UpdateAll();//全てのオブジェクトの更新処理を行う
}
//================================================================================

//=============================
//描画処理
//=============================
void CRenderer::Draw()
{
	//画面クリア（バックバッファ＆Zバッファのクリア)
	m_pD3DDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(255,255,255,255), 1.0f, 0);

 	CCamera* pCamera = CManager::GetCamera();//カメラへのポインタ
	
	//描画開始
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{//描画開始が成功した場合

		//カメラの描画
		pCamera->SetCamera();

		//FPSの表示
		CObject::DrawAll();//全てのオブジェクトの描画処理

		//デバッグ表示
		CManager::GetDebugText()->Draw();

		//FPS表示
		DrawFPS();

		//描画終了
		m_pD3DDevice->EndScene();
	}

	//バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);

}
//=========================================================================================================

//=====================================
//デバッグ表示処理
//=====================================
void CRenderer::DrawFPS()
{

	char aMode[64] = {};
	int nNumObject = CObject::GetNumAll();
    switch (CScene::GetMode())
    {
    case CScene::MODE_TITLE:
    	strcpy(&aMode[0], "タイトル");
    	break;
    case CScene::MODE_GAME:
    	strcpy(&aMode[0], "ゲーム");
    	break;
    case CScene::MODE_RESULT:
    	strcpy(&aMode[0], "リザルト");
    	break;
    case CScene::MODE_EDIT:
    	strcpy(&aMode[0], "エディット");
    	break;
    default:
    	break;
    }

	//文字列に代入
	CManager::GetDebugText()->PrintDebugText("【デバッグ表示】【F1】【FPS】 %d\n", GetFPS());
	CManager::GetDebugText()->PrintDebugText("現在のモード：%s\n",&aMode[0]);
	CManager::GetDebugText()->PrintDebugText("現在のオブジェクト総数：%d\n", nNumObject);
}
//=========================================================================================================


//===========================================
//モードの設定
//===========================================
void CRenderer::SetMode(MODE mode)
{
	m_mode = mode;//モードの設定
}
//=================================================================================

//============================================
//デバイスの取得
//============================================
LPDIRECT3DDEVICE9 CRenderer::GetDevice()
{
	return m_pD3DDevice;
}
//==================================================================================================