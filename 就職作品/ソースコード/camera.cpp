//===================================================================
//
//�U���R���F3D�I�u�W�F�N�g��`�悷��[Camera.cpp]
//Author:ShinaTaiyo
//
//===================================================================

//===================================================================
//�C���N���[�h
//===================================================================
#include "camera.h"
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "object.h"
#include "particle.h"
#include "game.h"
#include "debugtext.h"
#include "edit.h"
#include "calculation.h"
#include "collision.h"
#include "object.h"
#include "objectX.h"
#include "title.h"
#include "tutorial.h"
//====================================================================================================

//====================================================================
//�ÓI�����o�錾
//====================================================================
const float CCamera::m_BESIDECAMERALENGTH = 570.0f;//�r�T�C�h�r���[�̃J�����̋���
const float CCamera::s_fINITIAL_LENGTH = 200.0f;   //�J�����Ƃ̍ŏ��̋���
bool CCamera::s_bCAMERACONTROLLMOUSE = false;      //�f�o�b�O���ɃJ�������}�E�X�œ��������ǂ���
int CCamera::s_nSENSITIVITYLEVEL = 10;              //�J�����̊��x���x��
//====================================================================================================

//====================================================================
//�R���X�g���N�^
//====================================================================
CCamera::CCamera() : m_SupportPos(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_fLength(s_fINITIAL_LENGTH), m_fTurningRotSpeed(0.0f),m_fTurningSpeedY(0.0f),m_PosV(D3DXVECTOR3(0.0f,0.0f,0.0f)),
m_PosR(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_VecU(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_Rot(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_mtxProjection(),m_mtxView(),m_CameraType(CAMERATYPE_BIRD),m_DifferenceLength(D3DXVECTOR3(0.0f,0.0f,0.0f)),
m_ZoomSpeed(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_nShakeFrame(0),m_ModeTime(0),m_fShakePower(0.0f),m_fAddLength(0.0f),m_AddPosR(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_AddPosV(D3DXVECTOR3(0.0f,0.0f,0.0f))
,m_State(CAMERASTATE::NORMAL),m_pCameraState(DBG_NEW CCameraState_Normal()),m_pCameraLengthState(DBG_NEW CCameraLengthState())
{
#ifdef _DEBUG
	if (s_bDEBUGCAMERACONTROLLMOUSE)
	{//�f�o�b�O���̓J�������}�E�X�ő��삷�邩�ǂ��������߂邱�Ƃ��ł�����
		s_bCAMERACONTROLLMOUSE = true;
	}
	else
	{//�����[�X���͕K���J�������}�E�X�ő���ł���悤�ɂ���
		s_bCAMERACONTROLLMOUSE = false;
	}
#else
	//�K���J�������}�E�X�ő���ł���悤�ɂ���
	s_bCAMERACONTROLLMOUSE = true;
#endif // _DEBUG

}
//====================================================================================================

//====================================================================
//�f�X�g���N�^
//====================================================================
CCamera::~CCamera()
{

}
//====================================================================================================

//====================================================================
//����������
//====================================================================
HRESULT CCamera::Init()
{
	m_PosV = D3DXVECTOR3(0.0f, 200.0f,-350.0f);       //�x�_
	m_PosR = D3DXVECTOR3(0.0f,0.0f,0.0f);             //�����_
	m_VecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);           //������x�N�g���i�@���j
	m_Rot = D3DXVECTOR3(-D3DX_PI * 0.5f,0.0f,0.0f);   //����
	m_mtxProjection = {};                             //�v���W�F�N�V�����}�g���b�N�X
	m_mtxView = {};                                   //�r���[�}�g���b�N�X
	m_CameraType = CAMERATYPE_BIRD;                   //�J�������[�h�̎��
	m_nShakeFrame = 0;                                //�J������h�炷�t���[����
	m_fShakePower = 0.0f;                             //�J������h�炷��
	m_DifferenceLength = D3DXVECTOR3(0.0f,0.0f,0.0f); //�ΏۂƂ̋���
	m_ModeTime = 0;                                   //���[�h�̎���
	m_ZoomSpeed = D3DXVECTOR3(0.0f,0.0f,0.0f);        //�Y�[�����鑬��

	fstream ReadingFile;//�t�@�C��
	string ReadingBuff; //����
	ReadingFile.open("data\\TEXTFILE\\CameraSensitivity.txt");//�J�������x�����̃t�@�C����ǂݍ���

	if (ReadingFile.good())
	{//�t�@�C��������ɊJ���Ă�����
		while (ReadingFile >> ReadingBuff)
		{//������̓ǂݍ��݂����s����܂�
			if (ReadingBuff == "CAMERASENSITIVITYLEVEL")
			{
				ReadingFile >> ReadingBuff;//�C�R�[��
				ReadingFile >> s_nSENSITIVITYLEVEL;//���x���x����ǂݍ���
			}
		}
	}
	ReadingFile.close();//�t�@�C�������
	return S_OK;
}
//====================================================================================================

//====================================================================
//�I������
//====================================================================
void CCamera::Uninit()
{
	if (m_pCameraState != nullptr)
	{//�J�����̏�ԋ@�B��j��
		delete m_pCameraState;
		m_pCameraState = nullptr;
	}

	if (m_pCameraLengthState != nullptr)
	{//�J�����̋����̏�ԋ@�B��j��
		delete m_pCameraLengthState;
		m_pCameraLengthState = nullptr;
	}

	fstream WritingFile;//�����o���p�t�@�C��
	WritingFile.open("data\\TEXTFILE\\CameraSensitivity.txt", ios::out);

	if (WritingFile.good())
	{//�t�@�C���𐳏�ɊJ������
		WritingFile << "CAMERASENSITIVITYLEVEL = " << s_nSENSITIVITYLEVEL << endl;//�J�����̊��x���x���������o��
	}

	WritingFile.close();//�t�@�C�������
}
//====================================================================================================


//====================================================================
//�X�V����
//====================================================================
void CCamera::Update()
{
	m_pCameraState->Process(this);      //�J�����̊p�x�Ɋւ��鏈��
	m_pCameraLengthState->Process(this);//�J�����̋����Ɋւ��鏈��
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard();//�L�[���͏��
	CDebugText* pDebugText = CManager::GetDebugText();//�f�o�b�O���

	pDebugText->PrintDebugText("�J�������}�E�X�ő��삷�邩�ǂ����iL�V�t�g�������Ȃ���M): %d\n", s_bCAMERACONTROLLMOUSE);

	if (pInputKeyboard->GetPress(DIK_LSHIFT))
	{
		if (pInputKeyboard->GetTrigger(DIK_M))
		{
			s_bCAMERACONTROLLMOUSE = s_bCAMERACONTROLLMOUSE ? false : true;//�J�������}�E�X�ő��삷�邩�̃t���O��ONOFF��ς���
		}
	}

	//Pitch�͑O���ɔ͈͂𐧌�
	if (m_Rot.x < -D3DX_PI + 0.01f)
	{
		m_Rot.x = -D3DX_PI + 0.01f;
	}
	if (m_Rot.x > -0.01f)
	{
		m_Rot.x = -0.01f;
	}

	

	//�W���o�����b�N���������
	m_Rot.x = CCalculation::CorrectionRot(m_Rot.x);
	m_Rot.y = CCalculation::CorrectionRot(m_Rot.y);
	m_Rot.z = CCalculation::CorrectionRot(m_Rot.z);

	//===============================================================================

	//�f�o�b�O�\��
	CManager::GetDebugText()->PrintDebugText("�J�����̌����F%f %f %f\n", m_Rot.x, m_Rot.y, m_Rot.z);
	CManager::GetDebugText()->PrintDebugText("�J�����̎��_�F%f %f %f\n", m_PosV.x, m_PosV.y, m_PosV.z);
	CManager::GetDebugText()->PrintDebugText("�J�����̒����_�F%f %f %f\n", m_PosR.x, m_PosR.y, m_PosR.z);
	CManager::GetDebugText()->PrintDebugText("�J�����Ƃ̋����F%f\n",m_fAddLength);

	//�J�����̒ʏ�̒����_��ݒ肵������
	NormalCameraMove();

	if (CScene::GetMode() == CScene::MODE_GAME)
	{//�V�[�����Q�[���̎��̂ݍs������
		if (m_PosV.y < 0.0f)
		{//���_���O�ȉ��Ȃ�S�Ẳe��`�悵�Ȃ�
			CObjectX::SetCommonDraw(false);
		}
		else
		{//����ȊO�Ȃ�S�Ẳe��`��
			CObjectX::SetCommonDraw(true);
		}
	}
	MakeTransparent();//�J�����ƒ��~�_�Əd�Ȃ����I�u�W�F�N�g�𓧖��ɂ��鏈��

	//=================================================================================================================================

}
//====================================================================================================


//====================================================================
//�J�����ݒ菈��
//====================================================================
void CCamera::SetCamera()
{
	LPDIRECT3DDEVICE9 pDevice;                       //�f�o�C�X�ւ̃|�C���^
	pDevice = CManager::GetRenderer()->GetDevice();  //�f�o�C�X�̎擾

	//�v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProjection);

	if (m_CameraType == CAMERATYPE_BIRD || m_CameraType == CAMERATYPE_BESIDE)
	{
		// �v���W�F�N�V�����}�g���b�N�X���쐬�i�������e�s��j���@�����̃I�u�W�F�N�g��������������
		//�P�F�s��A�Q�F����p�A�R�F�A�X�y�N�g��A�S�F�߃N���b�v�ʁA�T�F���N���b�v��
		//  �߃N���b�v�ʁF�O�A���N���b�v�ʁF�P
		// �N���b�v��Ԃ��O�ɂ�����͕̂`�悵�Ȃ��悤�ɂ�����A�g���~���O���s���A
		//�`��R�X�g������
		D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
			D3DXToRadian(45.0f),
			(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
			10.0f,
			30000.0f);
	}
	else if (m_CameraType == CAMERATYPE_SIDE)
	{
		//���s���e�i���ˉe�j
		D3DXMatrixOrthoLH(
			&m_mtxProjection,
			(float)SCREEN_WIDTH / 2,
			(float)SCREEN_HEIGHT / 2,
			10.0f,
			2500.0f);
	}
	//�v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	//�r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxView);

	//==========================
	//�J������h�炷����
	//==========================
	D3DXVECTOR3 adjust = D3DXVECTOR3(0.0f,0.0f,0.0f);//�h��̕␳
	D3DXVECTOR3 ResultPosV = m_PosV;//���ʓI�Ȏx�_
	D3DXVECTOR3 ResultPosR = m_PosR;//���ʓI�Ȓ����_
	if (m_nShakeFrame > 0)
	{
		m_nShakeFrame--;//�ݒ肳�ꂽ�J������h�炷�t���[�������h�炷

		//�J������h�炷�͂��傫���قǋ����h�炷
		adjust.x += (float)(rand() % 100 - 50) * m_fShakePower;
		adjust.y += (float)(rand() % 100 - 50) * m_fShakePower;

		ResultPosV += adjust;
		ResultPosR += adjust;
	}
	//========================================================================================
	//�r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&m_mtxView,
		&ResultPosV,
		&ResultPosR,
		&m_VecU);

	//�r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW,&m_mtxView);

}
//====================================================================================================

//====================================================================
//�J�����ݒ菈��
//====================================================================
void CCamera::SetShake(int nShakeFrame, float fShakePower)
{
	m_nShakeFrame = nShakeFrame;//�J������h�炷�t���[����
	m_fShakePower = fShakePower;//�J������h�炷��
}
//====================================================================================================

//====================================================================
//�J�����̏�Ԃ�ς���
//====================================================================
void CCamera::ChengeState(CCameraState* pCameraState)
{
	delete m_pCameraState;//�J�����̏�Ԃ�j��
	m_pCameraState = pCameraState;//�V�����J�����̏��
}
//====================================================================================================

//====================================================================
//�J�����̋����̏�Ԃ�ς���
//====================================================================
void CCamera::ChengeLengthState(CCameraLengthState* pCameraLengthState)
{
	delete m_pCameraLengthState;//�J�����̋����̏�Ԃ�j��
	m_pCameraLengthState = pCameraLengthState;//�V�����J�����̋����̏��
}
//====================================================================================================

//====================================================================
//���ʂ̃J�����̈ʒu��ݒ肵������
//====================================================================
void CCamera::NormalCameraMove()
{
	D3DXVECTOR3 RotVec = CCalculation::RadToVec(m_Rot);
	switch (CScene::GetMode())
	{
	case CScene::MODE_TITLE://�V�[�����^�C�g���Ȃ�
		m_PosR = CTitle::GetPlayer()->GetPosInfo().GetPos() + D3DXVECTOR3(100.0f,75.0f,0.0f);//�v���C���[�������ɐݒ肷�邽�߂ɃJ�������v���C���[�̉E���ɐݒ�
		m_PosV = m_PosR + RotVec * m_fLength;//�����_����̌����̃x�N�g���ɋ����������Ď��_��ݒ�
		break;
	case CScene::MODE_GAME://�ʏ�̓v���C���[�̈ʒu�����ɓ���(�J�����̌������g�����񂷂�j
		if (CGame::GetPlayer() != nullptr)
		{
			m_PosR = CGame::GetPlayer()->GetPosInfo().GetPos() + D3DXVECTOR3(sinf(m_Rot.y + D3DX_PI * 0.5f) * 30.0f, 60.0f,cosf(m_Rot.y + D3DX_PI * 0.5f) * 30.0f) + m_AddPosR;//FPS���ɃJ�������猩���L�����N�^�[�̈ʒu�����ɂ��炷���ߒl�����Z����
			m_PosV = m_PosR + RotVec * m_fLength;//�����_����̌����̃x�N�g���ɋ����������Ď��_��ݒ�
		}
		break;
	case CScene::MODE_EDIT://�V�[�����G�f�B�b�g��������
		m_PosV = m_PosR + RotVec * (m_fLength);//�����_����̌����̃x�N�g���ɋ����������Ď��_��ݒ�
		break;
	default:
		break;
	}
}
//====================================================================================================

//====================================================================
//�������߂��I�u�W�F�N�g�𓧖��ɂ��鏈��
//====================================================================
void CCamera::MakeTransparent()
{
	if (CScene::GetMode() != CScene::MODE_EDIT)
	{//�V�[�����u�G�f�B�b�g�v�Ȃ�
		D3DXVECTOR3 Ray = m_PosR - m_PosV;                          //�����_�Ǝx�X�̃x�N�g�������A���C�Ƃ���
		D3DXVec3Normalize(&Ray, &Ray);                              //��L�ŋ��߂����C�𐳋K������
		D3DXVECTOR3 RayCollisionPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//���C�����������ʒu����p
		float fLength = 0.0f;                                       //���C�����������ꏊ�̋���
		for (int nCntPri = 0; nCntPri < CObject::m_nMAXPRIORITY; nCntPri++)
		{//�I�u�W�F�N�g���X�g����������
			CObject* pObj = CObject::GetTopObject(nCntPri);//�e���X�g�̐擪�I�u�W�F�N�g���擾����

			while (pObj != nullptr)
			{//�I�u�W�F�N�g��nullptr�ɂȂ�܂Ō�������
				CObject* pNext = pObj->GetNextObject();//���̃I�u�W�F�N�g���擾����
				CObject::TYPE Type = pObj->GetType();//�I�u�W�F�N�g�̎�ނ��擾����
				if (Type == CObject::TYPE::BGMODEL || Type == CObject::TYPE::BLOCK)
				{
					CObjectX* pObjX = static_cast<CObjectX*>(pObj);//�I�u�W�F�N�gX�ɃL���X�g����
					if (CCollision::RayIntersectsAABBCollisionPos(m_PosV, Ray, pObjX->GetPosInfo().GetPos() + pObjX->GetSizeInfo().GetVtxMin(), pObjX->GetPosInfo().GetPos() + pObjX->GetSizeInfo().GetVtxMax(), RayCollisionPos))
					{//���C���������Ă�����

						fLength = CCalculation::CalculationLength(m_PosV, RayCollisionPos);//���C�����������ʒu�Ƌ��������߂�

						if (m_fLength > fLength)
						{//�J�����̋��������I�u�W�F�N�g�ƃ��C�����������ʒu���߂��ꍇ�͓����ɂ���
							pObjX->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f), 3, true, true,false);
						}
					}
				}

				pObj = pNext;//���X�g��i�߂�
			}
		}
	}
}
//====================================================================================================

//���ȍ~�X�e�[�g�N���X

//****************************************************************************************************
//�J����������������ԃN���X
//****************************************************************************************************

//=================================================================
//�R���X�g���N�^
//=================================================================
CCameraState_TurnAround::CCameraState_TurnAround(D3DXVECTOR3 AimRot, float fAdjustTurnSpeed) : m_AimRot(AimRot),m_fAdjustTurnSpeed(fAdjustTurnSpeed)
{

}
//===================================================================================================================================================

//=================================================================
//�f�X�g���N�^
//=================================================================
CCameraState_TurnAround::~CCameraState_TurnAround()
{

}
//===================================================================================================================================================

//=================================================================
//����
//=================================================================
void CCameraState_TurnAround::Process(CCamera* pCamera)
{
	const D3DXVECTOR3 & NowRot = pCamera->GetRot();//�J�����̌������擾����

	//�����̍��������߂�
	float fRotDiffX = m_AimRot.x - NowRot.x;
	float fRotDiffY = m_AimRot.y - NowRot.y;

	//�����̍�����␳�i�W���o�����b�N�����)
	fRotDiffX = CCalculation::CorrectionRot(fRotDiffX);
	fRotDiffY = CCalculation::CorrectionRot(fRotDiffY);

	CManager::GetDebugText()->PrintDebugText("�J�����̌����̍���:%f\n");//�f�o�b�O�\��

	//���Z��������̗ʂ�␳���ċ��߂�
	float fAddRotX = fRotDiffX * m_fAdjustTurnSpeed;
	float fAddRotY = fRotDiffY * m_fAdjustTurnSpeed;

	//�W���o�����b�N�����
	fAddRotX = CCalculation::CorrectionRot(fAddRotX);
	fAddRotY = CCalculation::CorrectionRot(fAddRotY);

	//������ݒ肷��
	pCamera->SetRot(D3DXVECTOR3(pCamera->GetRot().x + fAddRotX, pCamera->GetRot().y + fAddRotY, pCamera->GetRot().z));

	//�����̐�Βl��0.01f�ȉ��Ȃ�
	if (fabsf(fAddRotX) < 0.005f && fabsf(fAddRotY) < 0.005f)//Yaw��Pitch�̌������Z�ʂ�0.01f�ȉ��ɂȂ�����X�e�[�g��߂�
	{//Yaw��Pitch�̉��Z����l���ǂ����0.005f�ȉ��Ȃ�������ς��؂����Ɖ��肵�A��Ԃ𕁒ʂɖ߂�
		pCamera->ChengeState(DBG_NEW CCameraState_Normal());
	}
}
//===================================================================================================================================================

//****************************************************************************************************
//���ʂ̏��
//****************************************************************************************************

//=================================================================
//�ÓI�����o�錾
//=================================================================
const float CCameraState_Normal::s_fNORMAL_AROUNDROTSPEED = 0.02f;//�J�����̒ʏ��]���x
//===================================================================================================================================================


//=================================================================
//�R���X�g���N�^
//=================================================================
CCameraState_Normal::CCameraState_Normal()
{

}
//===================================================================================================================================================

//=================================================================
//�f�X�g���N�^
//=================================================================
CCameraState_Normal::~CCameraState_Normal()
{

}
//===================================================================================================================================================

//=================================================================
//����
//=================================================================
void CCameraState_Normal::Process(CCamera* pCamera)
{
	//========================================
    //�J�����̌�����ς���
    //========================================
	if (CScene::GetMode() != CScene::MODE_TITLE && CScene::GetMode() != CScene::MODE_RESULT)
	{//�V�[�����^�C�g���ƃ��U���g�ȊO�Ȃ�
		if (CManager::GetInputKeyboard()->GetPress(DIK_Q))
		{//�J���������X�ɉE�Ɍ�������
			pCamera->SetRot(pCamera->GetRot() + D3DXVECTOR3(0.0f, -s_fNORMAL_AROUNDROTSPEED, 0.0f));
		}
		if (CManager::GetInputKeyboard()->GetPress(DIK_E))
		{//�J���������X�ɍ��Ɍ�������
			if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT))
			{//LSHIFT�������Ȃ���E�L�[��������Yaw�����Z�b�g
				pCamera->SetRot(D3DXVECTOR3(pCamera->GetRot().x, 0.0f, pCamera->GetRot().z));
			}
			else
			{//���ʂɃJ���������X�ɍ��Ɍ�������
				pCamera->SetRot(pCamera->GetRot() + D3DXVECTOR3(0.0f, s_fNORMAL_AROUNDROTSPEED, 0.0f));
			}
		}

		if (CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY::LB))
		{//�R���g���[���[��LB�{�^���������Ă�����
			pCamera->SetRot(pCamera->GetRot() + D3DXVECTOR3(0.0f, -s_fNORMAL_AROUNDROTSPEED, 0.0f));
		}
		if (CManager::GetInputJoypad()->GetPress(CInputJoypad::JOYKEY::RB))
		{//�R���g���[���[��RB�{�^���������Ă�����
			pCamera->SetRot(pCamera->GetRot() + D3DXVECTOR3(0.0f, s_fNORMAL_AROUNDROTSPEED, 0.0f));
		}

		//===========================
		//X�{�^���������Ă�����
		//===========================
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//�V�t�g�L�[�������Ȃ���E�E�E
			if (CManager::GetInputKeyboard()->GetPress(DIK_X) == true)
			{//�����_�̉��Z�ʂ����炷
				pCamera->SetAddPosR(pCamera->GetAddPosR() + D3DXVECTOR3(0.0f, -5.0f, 0.0f));
			}
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_X) == true)
		{//�����_�̉��Z�ʂ𑝂₷
			pCamera->SetAddPosR(pCamera->GetAddPosR() + D3DXVECTOR3(0.0f, 5.0f, 0.0f));
		}
		float fMagnification = 1.0f / CCamera::GetMaxSensitivityLevel();//���x�{�������߂�
		float fResultSensitivity = 0.0f;//���ʓI�Ȋ��x
		//=================================================
		//�Q�[���p�b�h�A���̓}�E�X�ŃJ�����̌�����ς���
		//=================================================
		if (CManager::GetInputJoypad()->GetRStickPress(16) == true)
		{//�E�X�e�B�b�N����͂��Ă�����
			if (CScene::GetMode() == CScene::MODE::MODE_GAME)
			{//�V�[�����u�Q�[���v��������
				CGame::GetTutorial()->SetSuccessCheck(CTutorial::CHECK::CAMERACONTROLL);
			}
			fResultSensitivity = s_fMAX_STICKSENSITIVITY * (fMagnification * CCamera::GetSensitivityLevel());//���x���x���ɉ����Ē���
			//�X�e�B�b�N�������������ɃJ�����̌�������������
			CManager::GetCamera()->SetRot(pCamera->GetRot() + D3DXVECTOR3(cosf(CManager::GetInputJoypad()->GetRStickAimRot() + D3DX_PI) * fResultSensitivity,
				sinf(CManager::GetInputJoypad()->GetRStickAimRot()) * fResultSensitivity, 0.0f));
		}

		if (CCamera::GetUseCameraControllMouse())
		{//�}�E�X�ł��J�����𑀍삷��Ȃ�
			float fAngle = 0.0f;    //�p�x
			float fAddYaw = 0.0f;   //Yaw
			float fAddPitch = 0.0f; //Pitch
			if (CScene::GetMode() == CScene::MODE_GAME || CScene::GetMode() == CScene::MODE_EDIT)
			{//�Q�[�����[�h�ƃG�f�B�b�g���[�h�̎������}�E�X���Œ�
				fResultSensitivity = s_fMAX_MOUSESENSITIVITY * (fMagnification * CCamera::GetSensitivityLevel());//���x���x���ɉ����Ē���
				if (CManager::GetInputMouse()->GetMouseMoveAngle(fAddYaw, fAddPitch, fResultSensitivity))
				{//�}�E�X���ړ�������������Yaw��Pitch�̉��Z�ʂ��擾����B�}�E�X�������Ă�����true
					if (CScene::GetMode() == CScene::MODE::MODE_GAME)
					{//�V�[�����u�Q�[���v��������
						CGame::GetTutorial()->SetSuccessCheck(CTutorial::CHECK::CAMERACONTROLL);//�J�����𓮂����`���[�g���A��������
					}
					//���������Z����
					CManager::GetCamera()->SetRot(pCamera->GetRot() + D3DXVECTOR3(fAddPitch,
						fAddYaw, 0.0f));
				}
			}
		}

		//===========================
		//C�{�^���������Ă�����
		//===========================
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//�V�t�g�L�[�������Ȃ���E�E�E
			if (CManager::GetInputKeyboard()->GetPress(DIK_C) == true)
			{//C�L�[������������
				pCamera->SetAddPosV(pCamera->GetAddPosV() + D3DXVECTOR3(0.0f, -5.0f, 0.0f));//���_�̈ʒu�̉��Z�ʂ����炷
			}
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_C) == true)
		{//�V�t�g�L�[�������Ă��Ȃ���C�L�[�������Ă�����
			pCamera->SetAddPosV(pCamera->GetAddPosV() + D3DXVECTOR3(0.0f, 5.0f, 0.0f));//���_�̈ʒu�̉��Z�ʂ𑝂₷
		}

		//===========================
		//V�{�^���������Ă�����
		//===========================
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//���V�t�g�L�[�������Ȃ���E�E�E
			if (CManager::GetInputKeyboard()->GetPress(DIK_V) == true)
			{//V�L�[�������Ă�����
				pCamera->SetLength(pCamera->GetLength() - 5.0f);//���_�ƒ����_�̋��������炷
			}
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_V) == true)
		{//���V�t�g�L�[�������Ă��Ȃ��AV�L�[�������Ă�����
			pCamera->SetLength(pCamera->GetLength() + 5.0f);//���_�ƒ����_�̋����𑝂₷
		}


		//========================================
		//�J������h�炷
		//========================================
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_F1) == true)
		{//F1�L�[�������Ă�����
			pCamera->SetShake(50, 0.2f);//�J������h�炷
		}

		//========================================
		//��������
		//========================================
		if (CScene::GetMode() == CScene::MODE::MODE_GAME)
		{//�V�[�����u�Q�[���v��������
			if (CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY::Y) || CManager::GetInputKeyboard()->GetTrigger(DIK_LCONTROL))
			{//�R���g���[���[��Y�L�[����������L�R���g���[���L�[�������Ă�����
				CGame::GetTutorial()->SetSuccessCheck(CTutorial::CHECK::TURNAROUND);//���������`���[�g���A��������

				pCamera->ChengeState(DBG_NEW CCameraState_TurnAround(pCamera->GetRot() + D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), 0.15f));//�w�肵�����������X�Ɍ�����Ԃɂ��A���݂̌����̌��ɐݒ�
			}
		}
	}
}
//===================================================================================================================================================

//*****************************************************************
//�J�����̋�����ς����Ԃ̃N���X
//*****************************************************************

//=================================================================
//�R���X�g���N�^
//=================================================================
CCameraLengthState::CCameraLengthState()
{

}
//===================================================================================================================================================

//=================================================================
//�f�X�g���N�^
//=================================================================
CCameraLengthState::~CCameraLengthState()
{

}
//===================================================================================================================================================

//=================================================================
//����
//=================================================================
void CCameraLengthState::Process(CCamera* pCamera)
{

}
//===================================================================================================================================================

//*****************************************************************
//�J�����̋�����ړI�̈ʒu�ɏ��X�ɕς����Ԃ̃N���X
//*****************************************************************

//=================================================================
//�R���X�g���N�^
//=================================================================
CCameraLengthState_Gradually::CCameraLengthState_Gradually(float fLength, float fAdjustAddSpeed, int nChengeFrame) : m_fAimLength(fLength),m_fAdjustAddSpeed(fAdjustAddSpeed),
m_nChengeLengthFrame(nChengeFrame),m_bNowAddLength(true),m_nChengeFrameCnt(0),m_bStartReturnLength(false)
{

}
//===================================================================================================================================================

//=================================================================
//�f�X�g���N�^
//=================================================================
CCameraLengthState_Gradually::~CCameraLengthState_Gradually()
{

}
//===================================================================================================================================================

//=================================================================
//����
//=================================================================
void CCameraLengthState_Gradually::Process(CCamera* pCamera)
{
	if (m_bNowAddLength == true)
	{//���݋��������Z���Ă���Ȃ�
		AddLengthProcess(pCamera);//�J�����Ƃ̋����𑝂₷����
	}
	else
	{//���݋��������Z���Ă��Ȃ��Ȃ�
		MaintainLengthFrameCntProcess();//�������ێ����鎞�Ԃ��J�E���g���鏈��
	}
	if (m_bStartReturnLength == true)
	{//�J�����Ƃ̋�����߂��Ȃ�
		ReturnLengthProcess(pCamera);//�J�����Ƃ̋����Ə�Ԃ�߂�����
	}
}
//===================================================================================================================================================

//=================================================================
//�J�����Ƃ̋����𑝂₷����
//=================================================================
void CCameraLengthState_Gradually::AddLengthProcess(CCamera* pCamera)
{
	float fLengthDiff = m_fAimLength - pCamera->GetLength();//�ړI�̋����ƌ��݂̃J�����̎��_�ƒ����_�̋����̍��������߂�
	float fAddLength = fLengthDiff * m_fAdjustAddSpeed;     //���Z���鋗�������߂�
	pCamera->SetLength(pCamera->GetLength() + fAddLength); //���������Z����

	if (fLengthDiff < 1.0f)
	{//�����̍�����1.0f�ȉ��Ȃ�
		m_bNowAddLength = false;//�����𑝂₷��Ԃ��I�t�ɂ���
	}
}
//===================================================================================================================================================

//=================================================================
//�J�����Ƃ̋������ێ����鏈��
//=================================================================
void CCameraLengthState_Gradually::MaintainLengthFrameCntProcess()
{
	m_nChengeFrameCnt++;//�ς����J�����̋������ێ�����t���[�������J�E���g����

	if (m_nChengeFrameCnt > m_nChengeLengthFrame)
	{//�������ێ����鎞�Ԃ𒴂�����
		m_bStartReturnLength = true;//�J�����̋�����߂��t���O��true�ɂ���
	}
}
//===================================================================================================================================================

//=================================================================
//�J�����̋��������ɖ߂�����
//=================================================================
void CCameraLengthState_Gradually::ReturnLengthProcess(CCamera* pCamera)
{
	float fLengthDiff = CCamera::GetInitialLength() - pCamera->GetLength();//�J�����̍ŏ��̋����ƌ��݂̃J�����̋����̍��������߂�
	float fSubLength = fLengthDiff * 0.1f;                                 //������0.1f�{���炷
	pCamera->SetLength(pCamera->GetLength() + fSubLength);                 //���������Z����

	if (fabsf(fLengthDiff) < 1.0f)//��������Βl��1.0f��艺��������Ԃ�߂�
	{//���������ɖ߂����̂ŏ�Ԃ�����
		pCamera->ChengeLengthState(DBG_NEW CCameraLengthState());
	}
}
//===================================================================================================================================================