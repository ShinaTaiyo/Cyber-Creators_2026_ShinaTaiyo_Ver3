//=========================================================================================================================
//
//６月１７日：画面遷移を実装する[Result.cpp]
//Author:ShinaTaiyo
//
//=========================================================================================================================

//=============================================================
//インクルード
//=============================================================
#include "Result.h"
#include "bg.h"
#include "field.h"
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "player.h"
//=========================================================================================================================

//=============================================================
//コンストラクタ
//=============================================================
CResult::CResult()
{

}
//=========================================================================================================================

//=============================================================
//デストラクタ
//=============================================================
CResult::~CResult()
{

}
//=========================================================================================================================

//=============================================================
//初期化処理
//=============================================================
HRESULT CResult::Init()
{
	CScene::Init();//シーン初期化処理
	//============================
	//各オブジェクト生成処理
	//============================
	CBg::Create(CBg::BG_RESULT);//リザルト背景を生成	
	//======================================================

		//PressEnter表示ロゴを生成
	m_pPressEnterLogo = CUi::Create(CUi::UITYPE_PRESSENTERORSTART_LOGO, CObject2D::POLYGONTYPE01_SENTERROLLING, 200.0f, 200.0f,
		SENTER_VECTOR3 + D3DXVECTOR3(0.0f, 200.0f, 0.0f), NULL_VECTOR3, NULL_VECTOR3, NORMAL_COL);
	m_pPressEnterLogo->SetUseDeath(false);//死亡フラグをfalseに
	m_pPressEnterLogo->SetUseBlinking(true, 60, 0.5f);//点滅させる
	return S_OK;
}
//=========================================================================================================================

//=============================================================
//終了処理
//=============================================================
void CResult::Uninit()
{
	CScene::Uninit();//シーン終了処理
    if (m_pPressEnterLogo != nullptr)
	{
		m_pPressEnterLogo->SetUseDeath(true);
		m_pPressEnterLogo->SetDeath();
		m_pPressEnterLogo = nullptr;
	}
}
//=========================================================================================================================

//=============================================================
//更新処理
//=============================================================
void CResult::Update()
{
	CScene::Update();//シーン更新処理

#ifdef _DEBUG
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) == true || CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_START) == true)
	{
		CManager::GetSceneFade()->SetSceneFade(CFade::FADEMODE_IN, CScene::MODE_EDIT);
	}
#else
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) == true || CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_START) == true)
	{
		CManager::GetSceneFade()->SetSceneFade(CFade::FADEMODE_IN, CScene::MODE_TITLE);
	}
#endif // _DEBUG
}
//=========================================================================================================================

//=============================================================
//描画処理
//=============================================================
void CResult::Draw()
{
	CScene::Draw();//シーン描画処理
}
//=========================================================================================================================