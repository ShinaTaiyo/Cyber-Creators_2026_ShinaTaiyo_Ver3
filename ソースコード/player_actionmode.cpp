//===================================================================================================================
//
//�P�P���P�O���F�v���C���[�̃A�N�V�������[�h�N���X�����[player_actionmode.cpp]
//Author:ShinaTaiyo
//
//===================================================================================================================

//===============================================================
//�C���N���[�h
//===============================================================
#include "player_actionmode.h"
#include "texture.h"
#include "calculation.h"
#include "debugtext.h"
#include "manager.h"
#include "collision.h"
#include "effect.h"
#include "enemy.h"
#include "meshorbit.h"
#include "attack.h"
#include "input.h"
#include "lockon.h"
#include "wire.h"
#include "player.h"
#include "game.h"
#include "tutorial.h"
#include "particle.h"
#include "sound.h"
#include "wire_head.h"
#include "camera.h"
//===================================================================================================================

//******************************************************************************************************************************************************
//�v���C���[�ړ��N���X
//******************************************************************************************************************************************************

//=====================================================================================================
//�R���X�g���N�^
//=====================================================================================================
CPlayerMove::CPlayerMove() : m_bIsLanding(false), m_bDodge(false)
{

}
//======================================================================================================================================================

//=====================================================================================================
//�f�X�g���N�^
//=====================================================================================================
CPlayerMove::~CPlayerMove()
{

}
//======================================================================================================================================================

//=====================================================================================================
//�ړ�����
//=====================================================================================================
void CPlayerMove::MoveProcess(CPlayer* pPlayer)
{
	if (m_bDodge == false)
	{//��𒆂Ȃ��΂ɒʏ�ړ��͂����Ȃ�
		const D3DXVECTOR3& Move = pPlayer->GetMoveInfo().GetMove();
		D3DXVECTOR3 AddMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		bool bMove = false;//�ړ����Ă��邩�ǂ���
		float fRotAim = 0.0f;

		bMove = CCalculation::CaluclationMove(true, AddMove, 10.0f, CCalculation::MOVEAIM_XZ, fRotAim);
		pPlayer->GetRotInfo().SetRot(D3DXVECTOR3(pPlayer->GetRotInfo().GetRot().x, fRotAim, pPlayer->GetRotInfo().GetRot().z));

		pPlayer->GetMoveInfo().SetUseInteria(true, CObjectX::GetNormalInertia());
		pPlayer->GetMoveInfo().SetUseGravity(true, CObjectX::GetNormalGravity());
		if (bMove == true)
		{
			pPlayer->SetNextMotion(1);//�ړ����[�V���������̃��[�V�����Ɏw�肷��
			pPlayer->GetMoveInfo().SetMove(AddMove + D3DXVECTOR3(0.0f, Move.y, 0.0f));
			CGame::GetTutorial()->SetSuccessCheck(CTutorial::CHECK::MOVE);
		}
		else
		{
			pPlayer->SetNextMotion(0);//�����Ă��Ȃ��̂ŁA�������[�V�����̃j���[�g�����ɐݒ�
		}
	}
}
//======================================================================================================================================================

//=====================================================================================================
//�W�����v����
//=====================================================================================================
void CPlayerMove::JumpProcess(CPlayer* pPlayer)
{
	if (pPlayer->GetLanding())
	{//�n�ʂɂ���Ȃ�W�����v
		pPlayer->GetMoveInfo().SetUseGravity(true, CObjectX::GetNormalGravity());
		if (CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY::A) || CManager::GetInputKeyboard()->GetTrigger(DIK_SPACE))
		{
			CGame::GetTutorial()->SetSuccessCheck(CTutorial::CHECK::JUMP);
			pPlayer->GetMoveInfo().SetMove(D3DXVECTOR3(pPlayer->GetMoveInfo().GetMove().x, 20.0f, pPlayer->GetMoveInfo().GetMove().z));
		}
	}
}
//======================================================================================================================================================

//=====================================================================================================
//�������
//=====================================================================================================
void CPlayerMove::DodgeProcess(CPlayer* pPlayer)
{
	CInputJoypad* pInput = CManager::GetInputJoypad();
	if ((pInput->GetLT_Trigger() || CManager::GetInputKeyboard()->GetTrigger(DIK_LSHIFT)) && m_bDodge == false)
	{
		m_bDodge = true;
		pPlayer->GetMoveInfo().SetMove(D3DXVECTOR3(pPlayer->GetMoveInfo().GetMove().x * s_fACCELL_PARAM, pPlayer->GetMoveInfo().GetMove().y, pPlayer->GetMoveInfo().GetMove().z * s_fACCELL_PARAM));
		pPlayer->GetMoveInfo().SetUseInteria(true, 0.1f);
		CGame::GetTutorial()->SetSuccessCheck(CTutorial::CHECK::DASH);
	}
	float fAverageSpeed = (fabsf(pPlayer->GetMoveInfo().GetMove().x) + fabsf(pPlayer->GetMoveInfo().GetMove().z)) / 2;

	if (fAverageSpeed < 0.51f && m_bDodge == true)
	{
		m_bDodge = false;
		pPlayer->GetMoveInfo().SetUseInteria(true, CObjectX::GetNormalInertia());
	}
}

//======================================================================================================================================================

//******************************************************************************************************************************************************
//�v���C���[���ʈړ��N���X
//******************************************************************************************************************************************************

//=====================================================================================================
//�R���X�g���N�^
//=====================================================================================================
CPlayerMove_Normal::CPlayerMove_Normal()
{

}
//======================================================================================================================================================

//=====================================================================================================
//�f�X�g���N�^
//=====================================================================================================
CPlayerMove_Normal::~CPlayerMove_Normal()
{
}
//======================================================================================================================================================

//=====================================================================================================
//�ړ�����
//=====================================================================================================
void CPlayerMove_Normal::MoveProcess(CPlayer* pPlayer)
{
	CPlayerMove::MoveProcess(pPlayer);

	CPlayerMove::DodgeProcess(pPlayer);

	CPlayerMove::JumpProcess(pPlayer);
}
//======================================================================================================================================================


//******************************************************************************************************************************************************
//�v���C���[�_�C�u�ړ��N���X
//******************************************************************************************************************************************************

//=====================================================================================================
//�R���X�g���N�^
//=====================================================================================================
CPlayerMove_PrepDive::CPlayerMove_PrepDive(CPlayer* pPlayer)
{

}
//======================================================================================================================================================

//=====================================================================================================
//�f�X�g���N�^
//=====================================================================================================
CPlayerMove_PrepDive::~CPlayerMove_PrepDive()
{

}
//======================================================================================================================================================

//=====================================================================================================
//�ړ�����
//=====================================================================================================
void CPlayerMove_PrepDive::MoveProcess(CPlayer* pPlayer)
{
	CPlayerMove::MoveProcess(pPlayer);//�ʏ�ړ�

	CPlayerMove::DodgeProcess(pPlayer);//����ړ�

	CPlayerMove::JumpProcess(pPlayer);//�W�����v�ړ�
}
//======================================================================================================================================================

//******************************************************************************************************************************************************
//�v���C���[�_�C�u�ړ��N���X
//******************************************************************************************************************************************************

//=====================================================================================================
//�ÓI�����o�錾
//=====================================================================================================

//=====================================================================================================
//�R���X�g���N�^
//=====================================================================================================
CPlayerMove_Dive::CPlayerMove_Dive() : m_DiveMove(D3DXVECTOR3(0.0f,0.0f,0.0f))
{

}
//======================================================================================================================================================

//=====================================================================================================
//�f�X�g���N�^
//=====================================================================================================
CPlayerMove_Dive::~CPlayerMove_Dive()
{

}
//======================================================================================================================================================

//=====================================================================================================
//�ړ�����
//=====================================================================================================
void CPlayerMove_Dive::MoveProcess(CPlayer* pPlayer)
{
	CWireHead* pWireHead = pPlayer->GetWire()->GetWireHead();
	bool bInput = CManager::GetInputJoypad()->GetRT_Press();

	if (CManager::GetInputJoypad()->GetRT_Press() || CManager::GetInputMouse()->GetMouseLeftClickPress())
	{
		bInput = true;
	}

	pPlayer->GetMoveInfo().SetMove(CCalculation::Calculation3DVec(pPlayer->GetPosInfo().GetPos(),pWireHead->GetPosInfo().GetPos(),s_fDIVEMOVE));//�ړI�n�ɒB����܂ő_��������

	if (pPlayer->IsDamaged())
	{//�_�C�u�ړ����Ƀ_���[�W���󂯂���ˌ����[�h�̏�����Ԃɖ߂��i�_�C�u�U���͋��͂ȍU���Ȃ̂ŁA�m�[���X�N�œˌ����������Ȃ�����j
		pPlayer->SetInitialActionMode(CPlayer::ACTIONMODE::SHOT);
	}

	//CCamera* pCamera = CManager::GetCamera();
	//if (CCalculation::CalculationLength(pPlayer->GetPosInfo().GetPos(), pWireHead->GetPosInfo().GetPos()) < s_fCOLLISIONDIVEMOVELENGTH)
	//{//�_�C�u���ɔ��肵����ړ����[�h�ƍU�����[�h��ʏ�ɖ߂�
	//	if(bInput == true && pWireHead->GetCollisionObjType() != CObject::TYPE::ENEMY)
	//	{//�����t�����_�C�u�i�������C���[�w�b�h���Փ˂����I�u�W�F�N�g���G�̏ꍇ�A�G�ɂ͈����t�������Ȃ��̂ŁA�U���Ɉڍs����
	//		//pPlayer->SetRot(pPlayer->GetRot());
	//		CGame::GetTutorial()->SetSuccessCheck(CTutorial::CHECK::STUCKWALL);
	//		pPlayer->ChengeMoveMode(DBG_NEW CPlayerMove_Stuck(pPlayer));
	//		pPlayer->ChengeAttackMode(DBG_NEW CPlayerAttack_Dont());
	//	}
	//	else
	//	{//�U�����ˌ����[�h
	//		pPlayer->ChengeAttackMode(DBG_NEW CPlayerAttack_Dive());
	//		pPlayer->GetWire()->SetUseDraw(false);
	//		pPlayer->ChengeMoveMode(DBG_NEW CPlayerMove_PrepDive(pPlayer));
	//		pPlayer->GetRotInfo().SetRot(D3DXVECTOR3(0.0f, pCamera->GetRot().y, 0.0f));
	//	}
	//}
}
//======================================================================================================================================================

//******************************************************************************************************************************************************
//�����t���ړ��N���X
//******************************************************************************************************************************************************

//=====================================================================================================
//�R���X�g���N�^�i���C���[���ˊJ�n�t���O�́A�f�t�H���g�Ŕ��ˉ\�Ȃ̂�true�ŏ�����)
//=====================================================================================================
CPlayerMove_Stuck::CPlayerMove_Stuck(CPlayer* pPlayer) : m_NowPos(pPlayer->GetPosInfo().GetPos()),m_bStartWireShot(true)
{
	CCamera* pCamera = CManager::GetCamera();
	CWireHead* pWireHead = pPlayer->GetWire()->GetWireHead();
	pPlayer->GetPosInfo().SetPos(pPlayer->GetPosInfo().GetPos() - pPlayer->GetMoveInfo().GetMove());
	//pCamera->SetRot(D3DXVECTOR3(-pWireHead->GetRot().x,pWireHead->GetRot().y + D3DX_PI,0.0f));//�J�����̌������Œ肵�����C���[�w�b�h�̋t���ɁI

	//==========================
	//�J�����̌��������߂�
	//==========================
	D3DXVECTOR3 ComRot = pWireHead->GetPosInfo().GetPos() - pPlayer->GetPosInfo().GetPos();
	D3DXVec3Normalize(&ComRot, &ComRot);
	float fYaw = atan2f(ComRot.x, ComRot.z);
	float fPitch = atan2f(ComRot.y, sqrtf(powf(ComRot.x, 2) + powf(ComRot.z, 2)));
	D3DXVECTOR3 ResultRot = D3DXVECTOR3(-fPitch - D3DX_PI * 0.5f, fYaw, 0.0f);//�J�����̌����𒲐�����i�O��������ɂ���j

	CSound* pSound = CManager::GetSound();

	pSound->PlaySoundB(CSound::SOUND_LABEL::SE_LANDING_000);

	if (pCamera->GetRot().x > ResultRot.x - 0.5f && pCamera->GetRot().x < ResultRot.x + 0.5f &&
		pCamera->GetRot().y > ResultRot.y - 0.5f && pCamera->GetRot().y < ResultRot.y + 0.5f)
	{//���݂̃J�����̌������ړI�̌����ɋ߂�������_�C�u��ɍ��킹��
		//CParticle::SummonParticle(CParticle::TYPE::TYPE00_NORMAL, 10, 60, 30.0f, 30.0f, 100, 10, false, pPlayer->GetSenterPos(), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), true);
		pCamera->ChengeState(DBG_NEW CCameraState_TurnAround(D3DXVECTOR3(-pWireHead->GetRotInfo().GetRot().x, pWireHead->GetRotInfo().GetRot().y + D3DX_PI, 0.0f), 0.15f));
	}
	//==============================================================================================
}
//======================================================================================================================================================

//=====================================================================================================
//�f�X�g���N�^
//=====================================================================================================
CPlayerMove_Stuck::~CPlayerMove_Stuck()
{

}
//======================================================================================================================================================

//=====================================================================================================
//�ړ�����
//=====================================================================================================
void CPlayerMove_Stuck::MoveProcess(CPlayer* pPlayer)
{
	pPlayer->GetMoveInfo().SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//*�ϐ��錾
	CCamera* pCamera = CManager::GetCamera(); // �J�����ւ̃|�C���^
	CWireHead* pWireHead = pPlayer->GetWire()->GetWireHead(); // ���C���[�w�b�h�ւ̃|�C���^
	D3DXVECTOR3 WireHeadRot = pWireHead->GetRotInfo().GetRot(); // ���C���[�w�b�h�̌����i�I�C���[�p�j
	CWire* pWire = pPlayer->GetWire();
	CLockon* pLockon = pPlayer->GetLockOn();//���b�N�I���ւ̃|�C���^
	CInputJoypad* pInputJoypad = CManager::GetInputJoypad();//�W���C�p�b�h���͏����擾
	CInputMouse* pInputMouse = CManager::GetInputMouse();//�}�E�X���͏����擾
	pWireHead->GetPosInfo().SetPos(pPlayer->GetPosInfo().GetPos());//�_�C�u�������Ȃ̂Ń��C���[�w�b�h���v���C���[�̈ʒu�ɌŒ�

	if (pInputJoypad->GetTrigger(CInputJoypad::JOYKEY::B) || pInputMouse->GetMouseMiddleClickTrigger())
	{//�����t���Ȃ���ˌ�����
		m_bStartWireShot = m_bStartWireShot ? false : true;//�t���O��ONOFF��ς���
		if (m_bStartWireShot == false)
		{//m_bStartWireShot��true�ɂ����Ƃ��ɃX�e�[�g���ς��Ȃ��悤�ɂ��邽�߁Afalse�̎������X�e�[�g��ς���
			pPlayer->ChengeAttackMode(DBG_NEW CPlayerAttack_StackShot(pPlayer));
		}
	}

	if ((pInputJoypad->GetRT_Trigger() || pInputMouse->GetMouseLeftClickTrigger()) && pLockon->GetSuccessRayCollision() && m_bStartWireShot)
	{//���C���[���ˈړ����[�h�Ƀ`�F���W
		pPlayer->ChengeAttackMode(DBG_NEW CPlayerAttack_Dont());//�����I�ɍU���s�ɂ���i�_�C�u�����܂ł̏��������邽�߁j
	}

}
//======================================================================================================================================================

//******************************************************************************************************************************************************
//�v���C���[�ړ��ł��Ȃ��N���X
//******************************************************************************************************************************************************

//=====================================================================================================
//�R���X�g���N�^
//=====================================================================================================
CPlayerMove_Dont::CPlayerMove_Dont()
{

}
//======================================================================================================================================================

//=====================================================================================================
//�f�X�g���N�^
//=====================================================================================================
CPlayerMove_Dont::~CPlayerMove_Dont()
{

}
//======================================================================================================================================================

//=====================================================================================================
//�ړ�����
//=====================================================================================================
void CPlayerMove_Dont::MoveProcess(CPlayer* pPlayer)
{
	pPlayer->GetMoveInfo().SetMove(D3DXVECTOR3(0.0f, pPlayer->GetMoveInfo().GetMove().y, 0.0f));
}
//======================================================================================================================================================

//******************************************************************************************************************************************************
//�v���C���[�U���ł���N���X
//******************************************************************************************************************************************************

//=====================================================================================================
//�ÓI�����o�錾
//=====================================================================================================
const float CPlayerAttack_Shot::s_fNORMAL_SHOTSPEED = 50.0f;

//=====================================================================================================
//�R���X�g���N�^
//=====================================================================================================
CPlayerAttack_Shot::CPlayerAttack_Shot()
{

}
//======================================================================================================================================================

//=====================================================================================================
//�f�X�g���N�^
//=====================================================================================================
CPlayerAttack_Shot::~CPlayerAttack_Shot()
{

}
//======================================================================================================================================================

//=====================================================================================================
//�U������
//=====================================================================================================
void CPlayerAttack_Shot::AttackProcess(CPlayer* pPlayer)
{
	CLockon* pLockon = pPlayer->GetLockOn();
	D3DXVECTOR3 ShotPos = pPlayer->GetPosInfo().GetPos() + D3DXVECTOR3(0.0f, pPlayer->GetSizeInfo().GetVtxMax().y, 0.0f);
	D3DXVECTOR3 Move = CCalculation::Calculation3DVec(ShotPos, pLockon->GetNearRayColObjPos(), s_fNORMAL_SHOTSPEED);
	CAttackPlayer* pAttackPlayer = nullptr;//�v���C���[�U���ւ̃|�C���^
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_J) == true || CManager::GetInputJoypad()->GetRT_Repeat(4) == true ||
		CManager::GetInputMouse()->GetMouseLeftClickRepeat(4) == true)
	{
		pAttackPlayer = CAttackPlayer::Create(CAttack::ATTACKTYPE::BULLET,CAttack::TARGETTYPE::ENEMY,CAttack::COLLISIONTYPE::SQUARE,true,true,3,0,45,ShotPos, pPlayer->GetRotInfo().GetRot(), Move, D3DXVECTOR3(1.0f, 1.0f, 1.0f));
		pAttackPlayer->GetMoveInfo().SetUseInteria(false, CObjectX::GetNormalInertia());
		pAttackPlayer->GetLifeInfo().SetAutoSubLife(true);
		pAttackPlayer->SetHitOtherThanLibing(true);

		CManager::GetSound()->PlaySoundB(CSound::SOUND_LABEL::SE_SHOT_001);//�ˌ����ʉ����o��
		CGame::GetTutorial()->SetSuccessCheck(CTutorial::CHECK::SHOT);		
	}

	if (CManager::GetInputJoypad()->GetRT_Press())
	{
		pPlayer->SetNextMotion(2);//�U���{�^���������Ă������A���̃��[�V�����͍U�����[�V�����ɂȂ�
	}
}
//======================================================================================================================================================

//******************************************************************************************************************************************************
//�v���C���[�U���ł��Ȃ��N���X
//******************************************************************************************************************************************************

//=====================================================================================================
//�R���X�g���N�^
//=====================================================================================================
CPlayerAttack_Dont::CPlayerAttack_Dont()
{

}
//======================================================================================================================================================

//=====================================================================================================
//�f�X�g���N�^
//=====================================================================================================
CPlayerAttack_Dont::~CPlayerAttack_Dont()
{

}
//======================================================================================================================================================

//=====================================================================================================
//�U������
//=====================================================================================================
void CPlayerAttack_Dont::AttackProcess(CPlayer* pPlayer)
{
	//
}
//======================================================================================================================================================

//******************************************************************************************************************************************************
//�v���C���[�_�C�u�U���N���X
//******************************************************************************************************************************************************

//=====================================================================================================
//�R���X�g���N�^
//=====================================================================================================
CPlayerAttack_Dive::CPlayerAttack_Dive()
{

}
//======================================================================================================================================================

//=====================================================================================================
//�f�X�g���N�^
//=====================================================================================================
CPlayerAttack_Dive::~CPlayerAttack_Dive()
{

}
//======================================================================================================================================================

//=====================================================================================================
//�U������
//=====================================================================================================
void CPlayerAttack_Dive::AttackProcess(CPlayer* pPlayer)
{
	CUiState_Gauge* pUiState_Gauge = dynamic_cast<CUiState_Gauge*>(CGame::GetPlayer()->GetDiveGaugeFrame()->GetUiState(CUiState::UISTATE::GAUGE));//UI�̃Q�[�W�����擾
	if (pUiState_Gauge != nullptr)
	{
		CGauge* pDiveGauge = pUiState_Gauge->GetGauge();//�_�C�u�Q�[�W���擾����

		CUi* pDivePossibleNum = pPlayer->GetDivePossibleNum();//�_�C�u�\�񐔂�UI���擾����
		CWireHead* pWireHead = pPlayer->GetWire()->GetWireHead();//���C���[�̓����擾����
		CUiState_Numeric* pUiState_Numeric = dynamic_cast<CUiState_Numeric*>(pDivePossibleNum->GetUiState(CUiState::UISTATE::NUMERIC));//UI�̐��������擾����
		if (pUiState_Numeric->GetValue() > 0)
		{//�_�C�u�Q�[�W�����܂��Ă����甚���U���𔭓�
			CAttackPlayer* pAttackPlayer = CAttackPlayer::Create(CAttack::ATTACKTYPE::EXPLOSION, CAttack::TARGETTYPE::ENEMY, CAttack::COLLISIONTYPE::SQUARE, false, true, 50, 30, 100, pPlayer->GetPosInfo().GetPos(), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.1f, 0.1f, 0.1f),
				D3DXVECTOR3(1.0f, 1.0f, 1.0f));
			pAttackPlayer->GetSizeInfo().SetUseAddScale(D3DXVECTOR3(0.4f, 0.4f, 0.4f), true);
			pAttackPlayer->SetColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 2, false, false, false);
			pAttackPlayer->GetLifeInfo().SetUseRatioLifeAlpha(true);
			pAttackPlayer->SetCollisionRelease(false);
			CGame::GetTutorial()->SetSuccessCheck(CTutorial::CHECK::TAKEDIVE);
			pUiState_Numeric->SetValue(pUiState_Numeric->GetValue() - 1, pDivePossibleNum);
			CManager::GetSound()->PlaySoundB(CSound::SOUND_LABEL::SE_EXPLOSION_000);
			//�ړI�̌����܂ŏ����Â������i�J�����̑O��-D3DX_PI * 0.5f,�v���C���[�̓f�t�H���g�̌������قȂ�̂ŁARot.y + D3DX_PI)
			CManager::GetCamera()->ChengeState(DBG_NEW CCameraState_TurnAround(D3DXVECTOR3(-D3DX_PI * 0.5f, pPlayer->GetRotInfo().GetRot().y + D3DX_PI, 0.0f), 0.1f));

			//���������������̂ŁA�J�����ƒ����_�̋�������莞�ԕς���
			CManager::GetCamera()->ChengeLengthState(DBG_NEW CCameraLengthState_Gradually(300.0f, 0.1f, 60));
		}
		pWireHead->GetDrawInfo().SetUseDraw(false);//���C���[�̓��̕`����I�t�ɂ���
		pPlayer->ChengeAttackMode(DBG_NEW CPlayerAttack_Dont());//�������I������̂ŁA�U����Ԃ��u�Ȃ��v�ɖ߂�
	}

}
//======================================================================================================================================================

//******************************************************************************************************************************************************
//�v���C���[���C���[���˒��N���X
//******************************************************************************************************************************************************

//******************************************************************************************************************************************************
//�U����ԁF�����t���ˌ�
//******************************************************************************************************************************************************

//=====================================================================================================
//�R���X�g���N�^
//=====================================================================================================
CPlayerAttack_StackShot::CPlayerAttack_StackShot(CPlayer* pPlayer) : m_bDelayModeChengeFrame(true)
{
	//CLockon* pLockon = pPlayer->GetLockOn();//���b�N�I���^�[�Q�b�g���擾
	//pLockon->ChengeTexture(CLockon::TYPE::STUCKSHOT);//�����t���ˌ��p�̃^�[�Q�b�g�e�N�X�`���ɕς���
}
//======================================================================================================================================================

//=====================================================================================================
//�f�X�g���N�^
//=====================================================================================================
CPlayerAttack_StackShot::~CPlayerAttack_StackShot()
{

}
//======================================================================================================================================================

//=====================================================================================================
//�U������
//=====================================================================================================
void CPlayerAttack_StackShot::AttackProcess(CPlayer* pPlayer)
{
	//CLockon* pLockon = pPlayer->GetLockOn();
	//D3DXVECTOR3 ShotPos = pPlayer->GetPosInfo().GetPos() + D3DXVECTOR3(0.0f, pPlayer->GetSizeInfo().GetVtxMax().y, 0.0f);
	//D3DXVECTOR3 Move = CCalculation::Calculation3DVec(ShotPos, pLockon->GetNearRayColObjPos(),s_fNORMAL_SHOTSPEED);
	//CAttackPlayer* pAttackPlayer = nullptr;//�v���C���[�U���ւ̃|�C���^
	//CInputJoypad* pInputJoypad = CManager::GetInputJoypad();//�W���C�p�b�h���͏��ւ̃|�C���^
	//CInputMouse* pInputMouse = CManager::GetInputMouse();   //�}�E�X���͏��ւ̃|�C���^
	//if (pInputJoypad->GetRT_Repeat(s_nSHOT_FREQUENCY) == true ||
	//	pInputMouse->GetMouseLeftClickRepeat(s_nSHOT_FREQUENCY) == true)
	//{
	//	pAttackPlayer = CAttackPlayer::Create(CAttack::ATTACKTYPE::BULLET, CAttack::TARGETTYPE::ENEMY, CAttack::COLLISIONTYPE::SQUARE, true, true, 3, 0, 45, ShotPos, pPlayer->GetRotInfo().GetRot(), Move, D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	//	pAttackPlayer->GetMoveInfo().SetUseInteria(false, CObjectX::GetNormalInertia());
	//	pAttackPlayer->GetLifeInfo().SetAutoSubLife(true);
	//	pAttackPlayer->SetHitOtherThanLibing(true);

	//	CManager::GetSound()->PlaySoundB(CSound::SOUND_LABEL::SE_SHOT_001);//�ˌ����ʉ����o��
	//	CGame::GetTutorial()->SetSuccessCheck(CTutorial::CHECK::SHOT);
	//}
	//if (pInputJoypad->GetRT_Press() || pInputMouse->GetMouseLeftClickPress())
	//{
	//	pPlayer->SetNextMotion(2);//�U���{�^���������Ă������A���̃��[�V�����͍U�����[�V�����ɂȂ�
	//}

	//if ((pInputJoypad->GetTrigger(CInputJoypad::JOYKEY::B) || pInputMouse->GetMouseMiddleClickTrigger()) && m_bDelayModeChengeFrame == false)
	//{//�X�^�b�N�ړ����[�h�ł��̍U�����[�h��JOYKEY_B�{�^���ŕς��Ă���̂ŁA���̏����ɍŏ��Ɉڍs�����Ƃ���JOYKEY_B�{�^�����������Ă��܂��̂ŁA1�t���[���x�点��
	//	pPlayer->ChengeAttackMode(DBG_NEW CPlayerAttack_Dont());//�U�����Ȃ����[�h�ɍĂѕς���
	//	pLockon->ChengeTexture(CLockon::TYPE::DIVE);//�_�C�u�p�̃^�[�Q�b�g�e�N�X�`���ɖ߂�
	//}

	//if (m_bDelayModeChengeFrame == true)
	//{//1�t���[����̃��[�h�`�F���W�̓��͂�x�点���̂Ŗ�ڂ͏I���Bfalse�ɖ߂�
	//	m_bDelayModeChengeFrame = false;
	//}

}
//======================================================================================================================================================