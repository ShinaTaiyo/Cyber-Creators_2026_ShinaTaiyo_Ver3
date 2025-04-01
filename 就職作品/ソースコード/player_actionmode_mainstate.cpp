//===================================================================================================================
//
//�Q�O�Q�T�N�Q���P�V���F�v���C���[�̃A�N�V�������[�h�̃��C����ԃN���X�����[player_actionmode_mainstate.cpp]
//Author:ShinaTaiyo
//
//===================================================================================================================

//===================================================================================================================
//�C���N���[�h
//===================================================================================================================
#include "player_actionmode_mainstate.h"
#include "player.h"
#include "enemy.h"
#include "collision.h"
#include "camera.h"
#include "manager.h"
#include "tutorial.h"
#include "game.h"
#include "input.h"
#include "sound.h"
#include "calculation.h"
//=========================================================================================================================================================

//*******************************************************************************************************************
//�X�[�p�[�N���X
//*******************************************************************************************************************

//===================================================================================================================
//�R���X�g���N�^
//===================================================================================================================
CPlayerActionMode::CPlayerActionMode()
{

}
//=========================================================================================================================================================

//===================================================================================================================
//�f�X�g���N�^
//===================================================================================================================
CPlayerActionMode::~CPlayerActionMode()
{

}
//=========================================================================================================================================================

//===================================================================================================================
//�����i�Ȃɂ��Ȃ�)
//===================================================================================================================
void CPlayerActionMode::Process(CPlayer* pPlayer)
{

}
//=========================================================================================================================================================

//*******************************************************************************************************************
//�ˌ��ړ��N���X
//*******************************************************************************************************************

//===================================================================================================================
//�R���X�g���N�^
//===================================================================================================================
CPlayerActionMode_ShotMove::CPlayerActionMode_ShotMove(CPlayer* pPlayer)
{
	CObjectX::MoveInfo& MoveInfo = pPlayer->GetMoveInfo();     //�ړ����
	CLockon* pLockOn = pPlayer->GetLockOn();                   //���b�N�I��
	pPlayer->ChengeAttackMode(DBG_NEW CPlayerAttack_Shot());   //�U�����[�h���ˌ����[�h�ɂ���
	pPlayer->ChengeMoveMode(DBG_NEW CPlayerMove_Normal());     //�ړ����[�h�𕁒ʂɂ���i�n�ʂœ����A�W�����v���ł���A������ł���)
	MoveInfo.SetUseInteria(true, CObjectX::GetNormalInertia());//�������g�p����
	MoveInfo.SetUseGravity(true, CObjectX::GetNormalGravity());//�d�͂��g�p����

	pLockOn->ChengeTexture(CLockon::TYPE::SHOT);               //�^�[�Q�b�g�̃e�N�X�`�����ˌ��d�l�ɕύX

	pPlayer->SetModeDisp(CUi::Create(CUi::UITYPE::ACTIONMODE_GUN,false,CObject2D::POLYGONTYPE::SENTERROLLING,100.0f, 100.0f, 1, false, D3DXVECTOR3(50.0f, 50.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)));//���[�h�\�����u�ˌ��v�ɕς���

}
//=========================================================================================================================================================

//===================================================================================================================
//�f�X�g���N�^
//===================================================================================================================
CPlayerActionMode_ShotMove::~CPlayerActionMode_ShotMove()
{

}
//=========================================================================================================================================================

//===================================================================================================================
//����
//===================================================================================================================
void CPlayerActionMode_ShotMove::Process(CPlayer* pPlayer)
{

}
//=========================================================================================================================================================

//*******************************************************************************************************************
//�_�C�u�����N���X
//*******************************************************************************************************************

//===================================================================================================================
//�R���X�g���N�^
//===================================================================================================================
CPlayerActionMode_PrepDive::CPlayerActionMode_PrepDive(CPlayer* pPlayer)
{
	CObjectX::MoveInfo& MoveInfo = pPlayer->GetMoveInfo();      //�ړ����
	CLockon* pLockOn = pPlayer->GetLockOn();                    //���b�N�I��
	pPlayer->ChengeAttackMode(DBG_NEW CPlayerAttack_Dont());    //�U�����[�h���_�C�u���[�h�ɂ���
	pPlayer->ChengeMoveMode(DBG_NEW CPlayerMove_Normal());      //�ړ����[�h�𕁒ʂɂ���i�n�ʂœ����A�W�����v���ł���A������ł���)
	MoveInfo.SetUseInteria(true, CObjectX::GetNormalInertia()); //�������g�p����
	MoveInfo.SetUseGravity(true, CObjectX::GetNormalGravity()); //�d�͂��g�p����

	pLockOn->ChengeTexture(CLockon::TYPE::DIVE);                //�^�[�Q�b�g�̃e�N�X�`�����ˌ��d�l�ɕύX

	pPlayer->SetModeDisp(CUi::Create(CUi::UITYPE::ACTIONMODE_DIVE,false, CObject2D::POLYGONTYPE::SENTERROLLING, 100.0f, 100.0f, 1, false, D3DXVECTOR3(50.0f, 50.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)));//���[�h�\�����u�_�C�u�v�ɕς���
}
//=========================================================================================================================================================

//===================================================================================================================
//�f�X�g���N�^
//===================================================================================================================
CPlayerActionMode_PrepDive::~CPlayerActionMode_PrepDive()
{

}
//=========================================================================================================================================================

//===================================================================================================================
//����
//===================================================================================================================
void CPlayerActionMode_PrepDive::Process(CPlayer* pPlayer)
{
	//==================
	//�ϐ�
	//==================
	CWire* pWire = pPlayer->GetWire();                       //���C���[���擾
	CWireHead* pWireHead = pWire->GetWireHead();             //���C���[�̓����擾
	CLockon* pLockon = pPlayer->GetLockOn();                 //���b�N�I�����擾
	CInputMouse* pInputMouse = CManager::GetInputMouse();    //�}�E�X���擾
	CInputJoypad* pInputJoypad = CManager::GetInputJoypad(); //�W���C�p�b�h���擾
	//============================================================================
	pWireHead->GetPosInfo().SetPos(pPlayer->GetPosInfo().GetPos());//�_�C�u�������Ȃ̂Ń��C���[�w�b�h���v���C���[�̈ʒu�ɌŒ�

	if ((pInputJoypad->GetRT_Trigger() || pInputMouse->GetMouseLeftClickTrigger()) && pLockon->GetSuccessRayCollision())
	{//�W���C�p�b�h�̉E�g���K�[�{�^�����̓}�E�X�̉E�N���b�N�������Ă��āA���b�N�I���̃��C���������Ă�����
		pPlayer->ChengeActionMode(DBG_NEW CPlayerActionMode_WireShot(pPlayer));//���C���X�e�[�g�����C���[���˃X�e�[�g�ɕύX
	}

}
//=========================================================================================================================================================

//*******************************************************************************************************************
//���C���[���˃N���X
//*******************************************************************************************************************

//===================================================================================================================
//�R���X�g���N�^
//===================================================================================================================
CPlayerActionMode_WireShot::CPlayerActionMode_WireShot(CPlayer* pPlayer)
{
	//==================
    //�ϐ�
    //==================
	CWire* pWire = pPlayer->GetWire();//���C���[���擾
	CWireHead* pWireHead = pWire->GetWireHead();//���C���[�̓����擾
	CLockon* pLockon = pPlayer->GetLockOn();//���b�N�I�����擾

	//���C���[�̓�
	CObjectX::MoveInfo& WireHeadMoveInfo = pWireHead->GetMoveInfo();//���C���[�̓��̈ړ������擾
	CObjectX::PosInfo& WireHeadPosInfo = pWireHead->GetPosInfo();//���C���[�̓��̈ʒu�����擾
	CObjectX::RotInfo& WireHeadRotInfo = pWireHead->GetRotInfo();//���C���[�̓��̌��������擾
	CObjectX::DrawInfo& WireHeadDrawInfo = pWireHead->GetDrawInfo();//���C���[�̓��̕`������擾

	//�v���C���[
	CObjectX::PosInfo& PlayerPosInfo = pPlayer->GetPosInfo();//�v���C���[�̈ʒu�����擾
	CObjectX::MoveInfo& PlayerMoveInfo = pPlayer->GetMoveInfo();//�v���C���[�̈ړ������擾
	const D3DXVECTOR3& PlayerPos = PlayerPosInfo.GetPos();   //�v���C���[�̈ʒu���擾

	const D3DXVECTOR3& WireHeadPos = WireHeadPosInfo.GetPos();//���C���[�̓��̈ʒu
	const D3DXVECTOR3& LockOnRayCollisionPos = pLockon->GetNearRayColObjPos();//���b�N�I���̃��C������������ԋ߂��I�u�W�F�N�g�̏Փˈʒu���擾����
	//============================================================================

	//�T�u�X�e�[�g�ݒ�
	pPlayer->ChengeMoveMode(DBG_NEW CPlayerMove_Dont());//�ړ������Ȃ�
	pPlayer->ChengeAttackMode(DBG_NEW CPlayerAttack_Dont());//�U�������Ȃ�

	//===============================
	//���C���[�����ˏ���
	//===============================

	D3DXVECTOR3 MathRot = LockOnRayCollisionPos - PlayerPos;//���C�����������ʒu�ƃv���C���[�̈ʒu�̃x�N�g�������߂�
	D3DXVec3Normalize(&MathRot, &MathRot);                  //��L�ŋ��߂��x�N�g���𐳋K������

	//���K�������x�N�g�������Ƃ�Yaw��Pitch�����߂�
	float fYaw = atan2f(MathRot.x, MathRot.z);//Z�����ɂ���
	float fPitch = atan2f(MathRot.y, sqrtf(powf(MathRot.x, 2) + powf(MathRot.z, 2)));//XZ���ʂ̒��������ɂ��ċ��߂�
	//fPitch *= -1.0f;//�s�b�`�������Ɂi�E����W�A��O���ɌX���̂ŁA������)

	WireHeadMoveInfo.SetMove(CCalculation::Calculation3DVec(WireHeadPos, LockOnRayCollisionPos, s_fWIREHEAD_SHOTSPEED));//���C���[�̓������b�N�I���̃��C�̏Փˈʒu�ɔ��˂���
	pWireHead->ResetCoolTime();                                                                                         //���C���[�̓���������܂ł̃N�[���^�C�������Z�b�g
	WireHeadMoveInfo.SetUseInteria(false, CObjectX::GetNormalInertia());                                                //���C���[�̓��̊������I�t�ɂ���
	WireHeadMoveInfo.SetUseGravity(false, 1.0f);                                                                        //���C���[�̓��̏d�͂��I�t�ɂ���
	WireHeadRotInfo.SetRot(D3DXVECTOR3(D3DX_PI * 0.5f - fPitch, fYaw, 0.0f));                                           //�O�����pitch�𒲐��i�O�̎���������Ă���A�O�������������O�Ɖ��肷�邽�߁A
	                                                                                                                    //��L�̏�����Pitch�����߂�Ƃ��A�l�𑫂��Ă���̂ł��łɏ�������^�[�����I�����
																														// ����ȏ�l�𑝂₷�Ɖ��������n�߂�̂�Pitch���t�ɂ��A�����������������悤�ɂ���
	                                                                                                                    //+ D3DX_PI * 0.5f�AYaw�͉�����Ȃ̂ł��̂܂܂ő��v
	WireHeadDrawInfo.SetUseDraw(true);                                                                                  //���C���[�̓��̕`��𕜊�������

	//���C���[
	pWire->SetUseDraw(true);                                                                                            //���C���[�̕`��𕜊�������
	pWire->SetUseUpdate(true);                                                                                          //���C���[�̍X�V�𕜊�������

	//�v���C���[
	PlayerMoveInfo.SetUseInteria(false, CObjectX::GetNormalInertia());                                                  //�������g�p���Ȃ�
	PlayerMoveInfo.SetUseGravity(false, CObjectX::GetNormalGravity());                                                  //�d�͂��g�p���Ȃ�
	//============================================================================

}
//=========================================================================================================================================================

//===================================================================================================================
//�f�X�g���N�^
//===================================================================================================================
CPlayerActionMode_WireShot::~CPlayerActionMode_WireShot()
{

}
//=========================================================================================================================================================

//===================================================================================================================
//����
//===================================================================================================================
void CPlayerActionMode_WireShot::Process(CPlayer* pPlayer)
{
	//=================
	//�ϐ��錾
	//=================
    CWire* pWire = pPlayer->GetWire();                              //���C���[���擾
    CWireHead* pWireHead = pWire->GetWireHead();                    //���C���[�̓����擾
	
	//�v���C���[
	CObjectX::MoveInfo& PlayerMoveInfo = pPlayer->GetMoveInfo();    //�v���C���[�̈ړ������擾
	CObjectX::PosInfo& PlayerPosInfo = pPlayer->GetPosInfo();       //�v���C���[�̈ʒu�����擾
	const D3DXVECTOR3& PlayerPos = PlayerPosInfo.GetPos();          //�v���C���[�̈ʒu

	//���C���[�̓�
	CObjectX::PosInfo& WireHeadPosInfo = pWireHead->GetPosInfo();   //���C���[�̓��̈ʒu�����擾
	const D3DXVECTOR3& WireHeadPos = WireHeadPosInfo.GetPos();      //���C���[�̓��̈ʒu
	CObjectX::MoveInfo& WireHeadMoveInfo = pWireHead->GetMoveInfo();//���C���[�̓��̈ړ������擾
    
    FrightenedEnemy(pPlayer);                                       //���̏����̓r���ő_�����G�͋�����
    
	PlayerMoveInfo.SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));          //���C���[���˒��̓v���C���[�̓������~�߂�

	if (pWireHead->GetSuccessCollision())
	{//���C���[���ǂꂩ�̃I�u�W�F�N�g�ɓ���������
		DecisionCameraRot(pPlayer);                                 //�J�����̌��������߂�
		WireHeadMoveInfo.SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));    //���C���[�w�b�h�̈ړ����~�߂�
		//���C���X�e�[�g���u�_�C�u�ړ��v	�ɕύX
		pPlayer->ChengeActionMode(DBG_NEW CPlayerActionMode_DiveMove(CCalculation::Calculation3DVec(PlayerPos,WireHeadPos,s_fWIREHEAD_SHOTSPEED * 0.5f),pPlayer));
		pPlayer->SetSuccessCollision(false);                        //�����Ԃ��m�����
	}

}
//=========================================================================================================================================================

//===================================================================================================================
//�G�����������鏈��
//===================================================================================================================
void CPlayerActionMode_WireShot::FrightenedEnemy(CPlayer* pPlayer)
{
	CObject* pObj = CObject::GetTopObject(static_cast<int>(CObject::TYPE::ENEMY));    //�G�̃��X�g�̐擪�I�u�W�F�N�g���擾
	CLockon* pLockOn = pPlayer->GetLockOn();                                          //���b�N�I�����擾
   const D3DXVECTOR3 & FrontPos = pLockOn->GetFrontPos();                             //���b�N�I���̎�O���W���擾
   const D3DXVECTOR3& Ray = pLockOn->GetNowRay();                                     //���b�N�I���̃��C���擾
   D3DXVECTOR3 CollisionPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);                          //���C�����������ʒu�i�[�p
   while (pObj != nullptr)
   {//�I�u�W�F�N�g��nullptr�ɂȂ�܂Ō�������

	   CObject* pNext = pObj->GetNextObject();//���̃I�u�W�F�N�g���擾

	   CEnemy* pEnemy = static_cast<CEnemy*>(pObj);//�I�u�W�F�N�g��G�ɃL���X�g
	   if (pEnemy->GetEnemyType() == CEnemy::ENEMYTYPE::DIVEWEAK)
	   {//�_�C�u�Ɏア�G��������������B������Ԃ���Ȃ��Ƃ��ɋ�����Ԃɂ���
		   if (pEnemy->GetState() != CEnemy::STATE::FRIGHTENDED && CCollision::RayIntersectsAABBCollisionPos(FrontPos, Ray, pEnemy->GetPosInfo().GetPos() + pEnemy->GetSizeInfo().GetVtxMin(), pEnemy->GetPosInfo().GetPos() + pEnemy->GetSizeInfo().GetVtxMax(), CollisionPos))
		   {//�G��������Ԃ���Ȃ������C���������Ă�����
			   pEnemy->ChengeMove(DBG_NEW CEnemyMove_Frightened(pEnemy, pEnemy->GetPosInfo().GetPos(), 90));//�P�D�T�b�ԋ�����Ԃɂ���
		   }
	   }
	   pObj = pNext;//���X�g�����ɐi�߂�
   }

}
//=========================================================================================================================================================

//===================================================================================================================
//�J�����̌��������肷��i���C���[�̓����I�u�W�F�N�g�ɓ����������Ɍ��Ă���J�����̌����ɉ����ĕ␳����j
//===================================================================================================================
void CPlayerActionMode_WireShot::DecisionCameraRot(CPlayer* pPlayer)
{
	//===================
	//�ϐ�
	//===================
	CWire* pWire = pPlayer->GetWire();                //���C���[�ւ̃|�C���^
    CWireHead* pWireHead = pWire->GetWireHead();      //���C���[�̓��ւ̃|�C���^

	//�J����
    CCamera* pCamera = CManager::GetCamera();         //�J�������擾
	const D3DXVECTOR3& CameraRot = pCamera->GetRot(); //�J�����̌���
    //============================================================

    //==========================
    //�J�����̌��������߂�
    //==========================
    D3DXVECTOR3 ComRot = pWireHead->GetPosInfo().GetPos() - pPlayer->GetPosInfo().GetPos();//���C���[�̓��ƃv���C���[�̃x�N�g�������߂�
    D3DXVec3Normalize(&ComRot, &ComRot);                                                   //��L�ŋ��߂��x�N�g���𐳋K������

	//���K���x�N�g�������ƂɃ��C���[�̓��ƃv���C���[�̌��������߂�
    float fYaw = atan2f(ComRot.x, ComRot.z);                                               //Z����������Yaw�����߂�
    float fPitch = atan2f(ComRot.y, sqrtf(powf(ComRot.x, 2) + powf(ComRot.z, 2)));         //XZ���ʂ̒���������Pitch�����߂�
    D3DXVECTOR3 ResultRot = D3DXVECTOR3(-fPitch - D3DX_PI * 0.5f, fYaw, 0.0f);             //�J�����̌����𒲐�����i�O��������ɂ��邽��-�������āA���C���[�̓��͏�������Ă���̂ŁA
	                                                                                       //�O�����-D3DX_PI * 0.5f
    if (CameraRot.x > ResultRot.x - 0.5f && CameraRot.x < ResultRot.x + 0.5f &&
		CameraRot.y > ResultRot.y - 0.5f && CameraRot.y < ResultRot.y + 0.5f)
    {//���݂̃J�����̌������ړI�̌����ɋ߂�������_�C�u��ɍ��킹��
    	pCamera->ChengeState(DBG_NEW CCameraState_TurnAround(D3DXVECTOR3(-fPitch - D3DX_PI * 0.5f, fYaw, 0.0f), 0.1f));
    }
    //==============================================================================================

}
//=========================================================================================================================================================

//*******************************************************************************************************************
//�_�C�u�ړ��N���X
//*******************************************************************************************************************

//===================================================================================================================
//�R���X�g���N�^
//===================================================================================================================
CPlayerActionMode_DiveMove::CPlayerActionMode_DiveMove(D3DXVECTOR3 Move, CPlayer* pPlayer)
{
	CObjectX::MoveInfo& PlayerMoveInfo = pPlayer->GetMoveInfo();      //�v���C���[�̈ړ����
	PlayerMoveInfo.SetMove(Move);                                     //�v���C���[�Ɉړ��ʂ�ݒ�
	PlayerMoveInfo.SetUseInteria(false, CObjectX::GetNormalInertia());//�������g�p���Ȃ�
	PlayerMoveInfo.SetUseGravity(false, CObjectX::GetNormalGravity());//�d�͂��g�p���Ȃ�

	pPlayer->ChengeMoveMode(DBG_NEW CPlayerMove_Dive());              //�ړ����@���u�_�C�u�v�ɕύX�i���C���[�̓����Ђ�����_���Ĕ��)
	pPlayer->ChengeAttackMode(DBG_NEW CPlayerAttack_Dont());          //�U�����@���u�Ȃ��v�ɕύX
}
//=========================================================================================================================================================

//===================================================================================================================
//�f�X�g���N�^
//===================================================================================================================
CPlayerActionMode_DiveMove::~CPlayerActionMode_DiveMove()
{

}
//=========================================================================================================================================================

//===================================================================================================================
//����
//===================================================================================================================
void CPlayerActionMode_DiveMove::Process(CPlayer* pPlayer)
{
	//====================
	//�ϐ��錾
	//====================
	bool bInput = false;//�w��̃{�^���������Ă��邩�ǂ���

	//���C���[�̓�
	CWire* pWire = pPlayer->GetWire();                             //���C���[�ւ̃|�C���^
	CWireHead* pWireHead = pWire->GetWireHead();                   //���C���[�̓�
	CObjectX::PosInfo& WireHeadPosInfo = pWireHead->GetPosInfo();  //���C���[�̈ʒu���
	const D3DXVECTOR3& WireHeadPos = WireHeadPosInfo.GetPos();     //���C���[�̓��̈ʒu

	//�v���C���[
	CObjectX::PosInfo& PlayerPosInfo = pPlayer->GetPosInfo();      //�ʒu���
	CObjectX::RotInfo& PlayerRotInfo = pPlayer->GetRotInfo();      //�������
	const D3DXVECTOR3& PlayerRot = PlayerRotInfo.GetRot();         //����
	const D3DXVECTOR3& PlayerPos = PlayerPosInfo.GetPos();         //�ʒu

	CCamera* pCamera = CManager::GetCamera();                      //�J�����ւ̃|�C���^
	CInputJoypad* pInputJoypad = CManager::GetInputJoypad();       //�W���C�p�b�h���͏��ւ̃|�C���^
	CInputMouse* pInputMouse = CManager::GetInputMouse();          //�}�E�X���͏��ւ̃|�C���^
	//========================================================================

	if (pInputJoypad->GetRT_Press() || pInputMouse->GetMouseLeftClickPress())
	{//�w�肳�ꂽ�{�^������������true�ɂ���
		bInput = true;
	}

	if (CCalculation::CalculationLength(PlayerPos,WireHeadPos) < s_fCOLLISIONSTARTLENGTH)
	{//�_�C�u���ɔ��肵����ړ����[�h�ƍU�����[�h��ʏ�ɖ߂�
		if (bInput == true && pWireHead->GetCollisionObjType() != CObject::TYPE::ENEMY)
		{//�����t�����_�C�u�i�������C���[�w�b�h���Փ˂����I�u�W�F�N�g���G�̏ꍇ�A�G�ɂ͈����t�������Ȃ��̂ŁA�U���Ɉڍs����
			CGame::GetTutorial()->SetSuccessCheck(CTutorial::CHECK::STUCKWALL);      //�ǂɈ����t���`���[�g���A��������
			pPlayer->ChengeActionMode(DBG_NEW CPlayerActionMode_Stuck(pPlayer));     //���C���X�e�[�g���u�����t���v�ɕύX
		}
		else
		{//�U�����_�C�u���[�h
			pWire->SetUseDraw(false);                                                //���C���[�̓���`�悵�Ȃ�
			PlayerRotInfo.SetRot(D3DXVECTOR3(0.0f, pCamera->GetRot().y, 0.0f));      //�������J�����̌����ɐݒ�
			pPlayer->ChengeActionMode(DBG_NEW CPlayerActionMode_DiveAttack(pPlayer));//���C���X�e�[�g���u�_�C�u�U���v�ɕύX
		}
	}
}
//=========================================================================================================================================================

//*******************************************************************************************************************
//�_�C�u�U���N���X
//*******************************************************************************************************************

//===================================================================================================================
//�R���X�g���N�^
//===================================================================================================================
CPlayerActionMode_DiveAttack::CPlayerActionMode_DiveAttack(CPlayer* pPlayer)
{
	pPlayer->ChengeAttackMode(DBG_NEW CPlayerAttack_Dive());//�U�����u�_�C�u�v�ɂ���
}
//=========================================================================================================================================================

//===================================================================================================================
//�f�X�g���N�^
//===================================================================================================================
CPlayerActionMode_DiveAttack::~CPlayerActionMode_DiveAttack()
{

}
//=========================================================================================================================================================

//===================================================================================================================
//����
//===================================================================================================================
void CPlayerActionMode_DiveAttack::Process(CPlayer* pPlayer)
{
	pPlayer->ChengeActionMode(DBG_NEW CPlayerActionMode_PrepDive(pPlayer));//�R���X�g���N�^���ʂ�����Ƀ_�C�u�U���������I������̂ŁA�_�C�u�������[�h�ɖ߂�
}
//=========================================================================================================================================================

//*******************************************************************************************************************
//�����t���N���X
//*******************************************************************************************************************

//===================================================================================================================
//�R���X�g���N�^
//===================================================================================================================
CPlayerActionMode_Stuck::CPlayerActionMode_Stuck(CPlayer* pPlayer) : m_bStartWireShot(true)
{
	pPlayer->ChengeMoveMode(DBG_NEW CPlayerMove_Dont());                                            //�ړ����@���u�Ȃ��v�ɕύX
	pPlayer->ChengeAttackMode(DBG_NEW CPlayerAttack_Dont());                                        //�U�����@���u�Ȃ��v�ɕύX
															                                        
	CCamera* pCamera = CManager::GetCamera();                                                       //�J�������擾����
	CWireHead* pWireHead = pPlayer->GetWire()->GetWireHead();                                       //���C���[�̓������
	pPlayer->GetPosInfo().SetPos(pPlayer->GetPosInfo().GetPos() - pPlayer->GetMoveInfo().GetMove());//1f�O�̈ʒu�ɐݒ�

	//==========================
	//���C���[�w�b�h�ւ̌��������߂�
	//==========================
	D3DXVECTOR3 ComRot = pWireHead->GetPosInfo().GetPos() - pPlayer->GetPosInfo().GetPos();//���C���[�̓��ƃv���C���[�̈ʒu�̃x�N�g�������߂�
	D3DXVec3Normalize(&ComRot, &ComRot);                                                   //�x�N�g���𐳋K��
	float fYaw = atan2f(ComRot.x, ComRot.z);                                               //Yaw�����߂�
	float fPitch = atan2f(ComRot.y, sqrtf(powf(ComRot.x, 2) + powf(ComRot.z, 2)));         //Pitch�����߂�
	D3DXVECTOR3 ResultRot = D3DXVECTOR3(-fPitch - D3DX_PI * 0.5f, fYaw, 0.0f);             //�J�����̌����𒲐�����i�O��������ɂ���j
	CSound* pSound = CManager::GetSound();                                                 //�T�E���h�ւ̃|�C���^���擾
	pSound->PlaySoundB(CSound::SOUND_LABEL::SE_LANDING_000);                               //���n����炷

	if (pCamera->GetRot().x > ResultRot.x - 0.5f && pCamera->GetRot().x < ResultRot.x + 0.5f &&
		pCamera->GetRot().y > ResultRot.y - 0.5f && pCamera->GetRot().y < ResultRot.y + 0.5f)
	{//�����t�����Ƃ��ɕǂ������Ă�����ǂ̋t������������
		pCamera->ChengeState(DBG_NEW CCameraState_TurnAround(D3DXVECTOR3(-pWireHead->GetRotInfo().GetRot().x, pWireHead->GetRotInfo().GetRot().y + D3DX_PI, 0.0f), 0.15f));
	}
	//==============================================================================================

}
//=========================================================================================================================================================

//===================================================================================================================
//�f�X�g���N�^
//===================================================================================================================
CPlayerActionMode_Stuck::~CPlayerActionMode_Stuck()
{

}
//=========================================================================================================================================================

//===================================================================================================================
//����
//===================================================================================================================
void CPlayerActionMode_Stuck::Process(CPlayer* pPlayer)
{
	pPlayer->GetMoveInfo().SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f)); //�����t���Ă���̂œ������~�߂�

	//*�ϐ��錾
	CCamera* pCamera = CManager::GetCamera();                      //�J�����ւ̃|�C���^
	CWire* pWire = pPlayer->GetWire();                             //���C���[�ւ̃|�C���^
	CWireHead* pWireHead = pWire->GetWireHead();                   //���C���[�w�b�h�ւ̃|�C���^
	CLockon* pLockon = pPlayer->GetLockOn();                       //���b�N�I���ւ̃|�C���^
	CInputJoypad* pInputJoypad = CManager::GetInputJoypad();       //�W���C�p�b�h���͏����擾
	CInputMouse* pInputMouse = CManager::GetInputMouse();          //�}�E�X���͏����擾
	pWireHead->GetPosInfo().SetPos(pPlayer->GetPosInfo().GetPos());//�_�C�u�������Ȃ̂Ń��C���[�w�b�h���v���C���[�̈ʒu�ɌŒ�

	if (pInputJoypad->GetTrigger(CInputJoypad::JOYKEY::B) || pInputMouse->GetMouseMiddleClickTrigger())
	{//�����t���Ȃ���ˌ�����
		m_bStartWireShot = m_bStartWireShot ? false : true;     //�t���O��ONOFF��ς���
		if (m_bStartWireShot == false)
		{//m_bStartWireShot��true�ɂ����Ƃ��ɃX�e�[�g���ς��Ȃ��悤�ɂ��邽�߁Afalse�̎������X�e�[�g��ς���
			pPlayer->ChengeAttackMode(DBG_NEW CPlayerAttack_Shot());//�U���X�e�[�g���u�ˌ��v�ɕύX����
			pLockon->ChengeTexture(CLockon::TYPE::STUCKSHOT);       //�^�[�Q�b�g�̃e�N�X�`�����Ђ����ˌ��d�l�ɕύX
		}
		else
		{
			pPlayer->ChengeAttackMode(DBG_NEW CPlayerAttack_Dont());//�U���X�e�[�g���u�Ȃ��v�ɕύX
			pLockon->ChengeTexture(CLockon::TYPE::DIVE);            //�^�[�Q�b�g�̃e�N�X�`�����_�C�u�d�l�ɖ߂�
		}
	}

	if ((pInputJoypad->GetRT_Trigger() || pInputMouse->GetMouseLeftClickTrigger()) && pLockon->GetSuccessRayCollision() && m_bStartWireShot)
	{//�W���C�p�b�h�́u�E�g���K�[�{�^���v���́u�}�E�X�̉E�N���b�N�{�^���v�������Ă��邩�u���b�N�I���̃��C���������Ă���v�����C���[���˂��ł���(true)�Ȃ�
		pPlayer->ChengeActionMode(DBG_NEW CPlayerActionMode_WireShot(pPlayer));//�A�N�V�������[�h�����C���[���˂ɕς���
	}

}
//=========================================================================================================================================================