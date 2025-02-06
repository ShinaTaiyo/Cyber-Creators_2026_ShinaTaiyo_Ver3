//=========================================================================================================================
//
//�U���P�V���F��ʑJ�ڂ���������[title.cpp]
//Author:ShinaTaiyo
//
//=========================================================================================================================

//=============================================================
//�C���N���[�h
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
//�ÓI�����o�錾
//=============================================================
CPlayer* CTitle::m_pPlayer = nullptr;
CBg3D* CTitle::m_pBg3D = nullptr;

//=============================================================
//�R���X�g���N�^
//=============================================================
CTitle::CTitle() : m_bMoveSwitch(true)
{
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI , 0.0f), D3DXVECTOR3(0.0f, -5.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	m_pPlayer->GetMoveInfo().SetUseGravity(false, CObjectX::GetNormalGravity());
	m_pPlayer->GetMoveInfo().SetUseInteria(false, CObjectX::GetNormalInertia());
	m_pPlayer->GetMoveInfo().SetUseUpdatePos(true);

	m_pBg3D = CBg3D::Create(CBg3D::BG3DTYPE::SKY, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(500.0f, 500.0f, 500.0f));
	m_pBg3D->SetUseDeath(false);

	m_pTitleLogo = CUi::Create(CUi::UITYPE::TITLELOGO_000, CObject2D::POLYGONTYPE::SENTERROLLING, 1000.0f, 500.0f, 10, false, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 110.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_pTitleLogo->SetUseDeath(false);
}
//=========================================================================================================================

//=============================================================
//�f�X�g���N�^
//=============================================================
CTitle::~CTitle()
{

}
//=========================================================================================================================

//=============================================================
//����������
//=============================================================
HRESULT CTitle::Init()
{
	CScene::Init();//�V�[������������

	//�^�C�g���w�i�𐶐�
	CManager::GetCamera()->SetRot(D3DXVECTOR3(-D3DX_PI * 0.5f, 0.0f, 0.0f));

	CUi* pUi = CUi::Create(CUi::UITYPE::PRESSENTER_000, CObject2D::POLYGONTYPE::SENTERROLLING, 200.0f, 200.0f, 100, false, D3DXVECTOR3(SCREEN_WIDTH - 200.0f,
		SCREEN_HEIGHT - 100.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	pUi->SetUseBlinking(true, 45, 0.0f);
	pUi->SetUseDeath(true);
	CObjectX::SetCommonDraw(false);

	CManager::GetSound()->PlaySoundB(CSound::SOUND_LABEL::BGM_NOESIS);//�^�C�g��BGM�𗬂�
	return S_OK;
}
//=========================================================================================================================

//=============================================================
//�I������
//=============================================================
void CTitle::Uninit()
{
	CScene::Uninit();//�V�[���I������

	if (m_pPlayer != nullptr)
	{
		m_pPlayer->SetUseDeath(true);
		m_pPlayer->SetDeath();
		m_pPlayer = nullptr;
	}

	if (m_pBg3D != nullptr)
	{
		m_pBg3D->SetUseDeath(true);
		m_pBg3D->SetDeath();
		m_pBg3D = nullptr;
	}

	if (m_pTitleLogo != nullptr)
	{
		m_pTitleLogo->SetUseDeath(true);
		m_pTitleLogo->SetDeath();
		m_pTitleLogo = nullptr;
	}

	CManager::GetSound()->Stop();//�S�Ẳ����~�߂�
}
//=========================================================================================================================

//=============================================================
//�X�V����
//=============================================================
void CTitle::Update()
{
	CScene::Update();//�V�[���X�V����

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) == true || CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY::START) == true)
	{
		CManager::GetSceneFade()->SetSceneFade(CFade::FADEMODE_IN, CScene::MODE_GAME);
	}

	if (m_pPlayer != nullptr && m_pBg3D != nullptr)
	{
		m_pBg3D->GetPosInfo().SetPos(m_pPlayer->GetPosInfo().GetPos());

		PlayerProcess();
	}
}
//=========================================================================================================================

//=============================================================
//�`�揈��
//=============================================================
void CTitle::Draw()
{
	CScene::Draw();//�V�[���`�揈��
}
//=========================================================================================================================

//=============================================================
//�v���C���[�̏���
//=============================================================
void CTitle::PlayerProcess()
{
	if (m_bMoveSwitch == false)
	{
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

	float fPosX = static_cast<float>(rand() % 1500 - 750);
	float fPosY = static_cast<float>(rand() % 1500 - 750);

	CAttackPlayer::Create(CAttack::ATTACKTYPE::BULLET, CAttack::TARGETTYPE::ENEMY, CAttack::COLLISIONTYPE::SQUARE, false, true, 3, 0,300,D3DXVECTOR3(fPosX,fPosY,3000.0f),
		D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(0.0f,0.0f,-20.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));
}
//=========================================================================================================================