//=========================================================================================================================
//
//６月１７日：画面遷移を実装する[title.cpp]
//Author:ShinaTaiyo
//
//=========================================================================================================================

//=============================================================
//インクルード
//=============================================================
#include "title.h"
#include "bg3d.h"
#include "bgModel.h"
#include "manager.h"
#include "renderer.h"
#include "sound.h"
#include "fade.h"
#include "attack.h"
#include "camera.h"
#include "input.h"
//=========================================================================================================================

//=============================================================
//静的メンバ宣言
//=============================================================
CPlayer* CTitle::m_pPlayer = nullptr;
CBg3D* CTitle::m_pBg3D = nullptr;

//=============================================================
//コンストラクタ
//=============================================================
CTitle::CTitle() : m_bMoveSwitch(true)
{
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI , 0.0f), D3DXVECTOR3(0.0f, -5.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	m_pPlayer->GetMoveInfo().SetUseGravity(false, CObjectX::GetNormalGravity());
	m_pPlayer->GetMoveInfo().SetUseInteria(false, CObjectX::GetNormalInertia());
	m_pPlayer->GetMoveInfo().SetUseUpdatePos(true);

	m_pBg3D = CBg3D::Create(CBg3D::BG3DTYPE::SKY, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(500.0f, 500.0f, 500.0f));
	m_pBg3D->SetUseDeath(false);

	m_pTitleLogo = CUi::Create(CUi::UITYPE::TITLELOGO_000, false, CObject2D::POLYGONTYPE::SENTERROLLING, 1000.0f, 500.0f, 10, false, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 110.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_pTitleLogo->SetUseDeath(false);
}
//=========================================================================================================================

//=============================================================
//デストラクタ
//=============================================================
CTitle::~CTitle()
{

}
//=========================================================================================================================

//=============================================================
//初期化処理
//=============================================================
HRESULT CTitle::Init()
{
	CScene::Init();//シーン初期化処理

	//タイトル背景を生成
	CManager::GetCamera()->SetRot(D3DXVECTOR3(-D3DX_PI * 0.5f, 0.0f, 0.0f));//カメラの向きを設定

	CUi* pUi = CUi::Create(CUi::UITYPE::PRESSENTER_000, false, CObject2D::POLYGONTYPE::SENTERROLLING, 200.0f, 200.0f, 100, false, D3DXVECTOR3(SCREEN_WIDTH - 200.0f,
		SCREEN_HEIGHT - 100.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));//エンターを押したらスタートというUIを生成
	pUi->SetUseBlinking(true, 45, 0.0f);//UIを点滅させる
	pUi->SetUseDeath(true);             //死亡フラグを使用する
	CObjectX::SetCommonDraw(false);     //影を描画しない

	CManager::GetSound()->PlaySoundB(CSound::SOUND_LABEL::BGM_NOESIS);//タイトルBGMを流す
	return S_OK;
}
//=========================================================================================================================

//=============================================================
//終了処理
//=============================================================
void CTitle::Uninit()
{
	CScene::Uninit();//シーン終了処理

	if (m_pPlayer != nullptr)
	{//プレイヤーの破棄
		m_pPlayer->SetUseDeath(true);//死亡フラグを使用する
		m_pPlayer->SetDeath();//死亡フラグを設定
		m_pPlayer = nullptr;  //ポインタを初期化
	}

	if (m_pBg3D != nullptr)
	{//3D背景の破棄
		m_pBg3D->SetUseDeath(true);//死亡フラグを使用する
		m_pBg3D->SetDeath();       //死亡フラグを設定する
		m_pBg3D = nullptr;         //ポインタを初期化
	}

	if (m_pTitleLogo != nullptr)
	{//タイトルロゴの破棄
		m_pTitleLogo->SetUseDeath(true);//死亡フラグを使用する
		m_pTitleLogo->SetDeath();       //死亡フラグを設定する
		m_pTitleLogo = nullptr;         //ポインタを初期化
	}

	CManager::GetSound()->Stop();//全ての音を止める
}
//=========================================================================================================================

//=============================================================
//更新処理
//=============================================================
void CTitle::Update()
{
	CScene::Update();//シーン更新処理

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) == true || CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY::START) == true)
	{//ENTERキー又はジョイパッドのSTARTボタンを押していたら
		CManager::GetSceneFade()->SetSceneFade(CFade::FADEMODE_IN, CScene::MODE_GAME);//フェードインしてゲームモードに
	}

	if (m_pPlayer != nullptr && m_pBg3D != nullptr)
	{//プレイヤーと3D背景が存在していたら
		m_pBg3D->GetPosInfo().SetPos(m_pPlayer->GetPosInfo().GetPos());//3D背景の中心をプレイヤーに設定

		PlayerProcess();//プレイヤーの処理
	}
}
//=========================================================================================================================

//=============================================================
//描画処理
//=============================================================
void CTitle::Draw()
{
	CScene::Draw();//シーン描画処理
}
//=========================================================================================================================

//=============================================================
//プレイヤーの処理
//=============================================================
void CTitle::PlayerProcess()
{
	if (m_bMoveSwitch == false)
	{//フラグをオンオフしてプレイヤーを上下に動かす
		m_pPlayer->GetMoveInfo().SetMove(D3DXVECTOR3(m_pPlayer->GetMoveInfo().GetMove().x,m_pPlayer->GetMoveInfo().GetMove().y + 0.1f,m_pPlayer->GetMoveInfo().GetMove().z));

		if (m_pPlayer->GetMoveInfo().GetMove().y >= 5.0f)
		{
			m_bMoveSwitch = m_bMoveSwitch ? false : true;
		}

	}
	else
	{
		m_pPlayer->GetMoveInfo().SetMove(D3DXVECTOR3(m_pPlayer->GetMoveInfo().GetMove().x, m_pPlayer->GetMoveInfo().GetMove().y - 0.1f, m_pPlayer->GetMoveInfo().GetMove().z));

		if (m_pPlayer->GetMoveInfo().GetMove().y <= -5.0f)
		{
			m_bMoveSwitch = m_bMoveSwitch ? false : true;
		}
	}

	//指定した位置からランダムに演出用の攻撃を召喚
	float fPosX = static_cast<float>(rand() % 1500 - 750);
	float fPosY = static_cast<float>(rand() % 1500 - 750);

	//生成
	CAttackPlayer::Create(CAttack::ATTACKTYPE::BULLET, CAttack::TARGETTYPE::ENEMY, CAttack::COLLISIONTYPE::SQUARE, false, true, 3, 0,300,D3DXVECTOR3(fPosX,fPosY,3000.0f),
		D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(0.0f,0.0f,-20.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));
}
//=========================================================================================================================