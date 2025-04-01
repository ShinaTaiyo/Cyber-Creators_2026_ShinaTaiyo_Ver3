//===================================================
//
//クラスで基盤づくり[Manager.cpp]
//Author;ShinaTaiyo
//
//===================================================

//=======================
//インクルード
//=======================
#include "object.h"
#include "bg.h"
#include "player.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "sound.h"
#include "camera.h"
#include "light.h"
#include "object3d.h"
#include "texture.h"
#include "eventmanager.h"
#include "debugtext.h"
#include "objectXInfo.h"
#include "main.h"
#include "fade.h"
//===================================================

//=======================
//静的メンバ
//=======================
CRenderer* CManager::m_pRenderer = nullptr;              //レンダラー
CInputKeyboard* CManager::m_pInputKeyboard = nullptr;    //キー入力情報
CInputJoypad* CManager::m_pInputJoypad = nullptr;        //ジョイパッド入力情報
CSound* CManager::m_pSound = nullptr;                    //サウンド情報
CCamera* CManager::m_pCamera = nullptr;                  //カメラ
CLight* CManager::m_pLight = nullptr;                    //ライト
CTexture* CManager::m_pTexture = nullptr;                //テクスチャ情報
CObjectXInfo* CManager::m_pObjectXInfo = nullptr;        //オブジェクトX情報
CScene* CManager::m_pScene = nullptr;                    //シーン
CSceneFade* CManager::m_pSceneFade = nullptr;            //シーン遷移用フェード
CInputMouse* CManager::m_pInputMouse = nullptr;          //マウス入力情報
CDebugText* CManager::m_pDebugText = nullptr;            //デバッグ表示
//===================================================

//=======================
//コンストラクタ
//=======================
CManager::CManager()
{

}
//===================================================

//=======================
//デストラクタ
//=======================
CManager::~CManager()
{

}

//================================
//初期化処理
//================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	//============================
	//レンダラーの生成
	//============================
	m_pRenderer = DBG_NEW CRenderer;
	m_pRenderer->Init(hWnd, bWindow);

	//======================================================

	//============================
	//キー入力情報の生成
	//============================
	m_pInputKeyboard = DBG_NEW CInputKeyboard;
	m_pInputKeyboard->Init(hInstance, hWnd);

	//======================================================

	//============================
	//ジョイパッド入力情報の生成
	//============================
	m_pInputJoypad = DBG_NEW CInputJoypad;
	m_pInputJoypad->Init(hInstance, hWnd);
	//======================================================

	//===================================
	//マウス入力情報の設定
	//===================================
	m_pInputMouse = DBG_NEW CInputMouse;
	m_pInputMouse->Init(hInstance, hWnd);
	//======================================================

	//============================
	//サウンドの生成
	//============================
	m_pSound = DBG_NEW CSound;
	m_pSound->Init(hWnd);
	//======================================================

	//============================
	//カメラの生成
	//============================
	m_pCamera = DBG_NEW CCamera;
	m_pCamera->Init();
	//======================================================

	//============================
	//ライトの生成
	//============================
	m_pLight = DBG_NEW CLight;
	m_pLight->Init();
	//======================================================

	//============================
	//テクスチャクラスの生成
	//============================
	m_pTexture = DBG_NEW CTexture;
	//======================================================


	//===============================
	//オブジェクトX情報クラスを生成
    //===============================
	m_pObjectXInfo = DBG_NEW CObjectXInfo;
	//======================================================

	//============================
	//デバッグテキストを生成
	//============================
	m_pDebugText = DBG_NEW CDebugText;
	m_pDebugText->Init();

	//===================================
	//モードの設定
	//===================================
	SetMode(CScene::MODE_TITLE);
	m_pSceneFade = CSceneFade::Create();
	m_pSceneFade->SetIsStopUpdatePause(false);//ポーズ中でも更新する
	m_pSceneFade->SetSceneFade(CFade::FADEMODE_OUT, CScene::MODE_TITLE);
	//=====================================================

	//============================
	//マップ情報を設定
	//============================

	return S_OK;
}
//================================================================================

//=======================
//終了処理
//=======================
void CManager::Uninit()
{	
	//================================
	//シーン
	//================================
	if (m_pScene != nullptr)
	{
		m_pScene->Uninit();
		delete m_pScene;
		m_pScene = nullptr;
	}

	//================================
	//レンダラーの開放
	//================================
	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = nullptr;
	}
	//===============================================

	//================================
	//キー入力情報の開放
	//================================
	if (m_pInputKeyboard != nullptr)
	{
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = nullptr;
	}
	//===============================================

	//================================
	//ジョイパッド入力情報の開放
	//================================
	if (m_pInputJoypad != nullptr)
	{
		m_pInputJoypad->Uninit();
		delete m_pInputJoypad;
		m_pInputJoypad = nullptr;
	}
	//===============================================
	
	//================================
	//マウス入力情報の開放
	//================================
	if (m_pInputMouse != nullptr)
	{
		m_pInputMouse->Uninit();
		delete m_pInputMouse;
		m_pInputMouse = nullptr;
	}
	//===============================================

	//================================
	//サウンドの開放
	//================================
	if (m_pSound != nullptr)
	{
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = nullptr;
	}
	//===============================================

	//================================
	//カメラ
	//================================
	if (m_pCamera != nullptr)
	{
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = nullptr;
	}
	//===============================================

	//================================
	//ライト
	//================================
	if (m_pLight != nullptr)
	{
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = nullptr;
	}
	//===============================================
	
	//================================
	//テクスチャ
	//================================
	if (m_pTexture != nullptr)
	{
		m_pTexture->Unload();
		delete m_pTexture;
		m_pTexture = nullptr;
	}
	//===============================================

	//================================
	//オブジェクトX情報
	//================================
	if (m_pObjectXInfo != nullptr)
	{
		m_pObjectXInfo->Unload();
		delete m_pObjectXInfo;
		m_pObjectXInfo = nullptr;
	}
	//===============================================

	//================================
	//フェード情報
	//================================
	if (m_pSceneFade != nullptr)
	{
		m_pSceneFade->SetUseDeath(true);
		m_pSceneFade->SetDeath();
		m_pSceneFade = nullptr;
	}
	//===============================================

	//================================
	//デバッグテキスト情報
	//================================
	if (m_pDebugText != nullptr)
	{
		m_pDebugText->Uninit();
		delete m_pDebugText;
		m_pDebugText = nullptr;
	}
	//===============================================

	CObject::ReleaseAll();    //オブジェクトの開放

	//最後に残ったオブジェクトをここで全部破棄
	CObject::ReleaseProcess();//リストの破棄をする処理

	CObject::ReleaseAll();    //オブジェクトの開放

	//最後に残ったオブジェクトをここで全部破棄
	CObject::ReleaseProcess();//リストの破棄をする処理
}
//===================================================

//=======================
//更新処理
//=======================
void CManager::Update()
{
	m_pRenderer->Update();     //レンダラー
	m_pInputKeyboard->Update();//キー入力
	m_pInputJoypad->Update();  //ジョイパッド入力
	m_pInputMouse->Update();   //マウス入力
	m_pCamera->Update();       //カメラ
	m_pLight->Update();        //ライト
	if (m_pScene != nullptr)
	{
		m_pScene->Update();        //シーンの更新処理
	}
}
//===================================================

//=======================
//描画処理
//=======================
void CManager::Draw()
{
	m_pRenderer->Draw();         //レンダラー

	if (m_pScene != nullptr)
	{
		m_pScene->Draw();        //シーンの更新処理
	}
}
//===================================================

//=================================
//レンダラークラスを取得する
//=================================
CRenderer* CManager::GetRenderer()
{
	return m_pRenderer;
}
//===================================================

//=================================
//キーボードクラスを取得する
//=================================
CInputKeyboard* CManager::GetInputKeyboard()
{
	return m_pInputKeyboard;
}
//===================================================

//=================================
//ジョイパッドクラスを取得する
//=================================
CInputJoypad* CManager::GetInputJoypad()
{
	return m_pInputJoypad;
}
//====================================================

//===========================================
//モード
//===========================================
void CManager::SetMode(CScene::MODE mode)
{

	if (m_pScene != nullptr)
	{//シーンが存在していたら

		//シーンを破棄
		m_pScene->Uninit();
		delete m_pScene;
		m_pScene = nullptr;
	}
	
	//新しいシーンを生成
	m_pScene = CScene::Create(mode);
}
