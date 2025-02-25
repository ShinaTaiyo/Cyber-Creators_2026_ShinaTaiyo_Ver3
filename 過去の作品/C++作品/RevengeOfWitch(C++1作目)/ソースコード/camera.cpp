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
#include "boss.h"
#include "input.h"
#include "object.h"
#include "game.h"
#include "edit.h"
//====================================================================================================

//====================================================================
//�ÓI�����o�錾
//====================================================================
const float CCamera::m_BESIDECAMERALENGTH = 570.0f;//�r�T�C�h�r���[�̃J�����̋���
//====================================================================================================

//====================================================================
//�R���X�g���N�^
//====================================================================
CCamera::CCamera() : m_SupportPos(NULL_VECTOR3),m_fLength(0.0f), m_fTurningRotSpeed(0.0f),m_fTurningSpeedY(0.0f)
{

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
	m_PosV = D3DXVECTOR3(0.0f, 200.0f,-400.0f);     //�x�_
	m_PosR = NULL_VECTOR3;                          //�����_
	m_VecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);         //������x�N�g���i�@���j
	m_Rot = NULL_VECTOR3;                           //����
	m_mtxProjection = {};                           //�v���W�F�N�V�����}�g���b�N�X
	m_mtxView = {};                                 //�r���[�}�g���b�N�X
	m_CameraType = (CAMERATYPE)(1);                 //�J�������[�h�̎��
	m_nShakeFrame = 0;                              //�J������h�炷�t���[����
	m_fShakePower = 0.0f;                           //�J������h�炷��
	m_DifferenceLength = NULL_VECTOR3;              //�ΏۂƂ̋���
	m_ModeTime = 0;                                 //���[�h�̎���
	m_ZoomSpeed = NULL_VECTOR3;                     //�Y�[�����鑬��
	return S_OK;
}
//====================================================================================================

//====================================================================
//�I������
//====================================================================
void CCamera::Uninit()
{

}
//====================================================================================================


//====================================================================
//�X�V����
//====================================================================
void CCamera::Update()
{
	//========================================
	//�J�����̈ʒu��ς���
	//========================================
	if (CManager::GetInputKeyboard()->GetPress(DIK_E) == true)
	{
		m_Rot.y += 0.02f;
	}
	else if (CManager::GetInputKeyboard()->GetPress(DIK_Q) == true)
	{
		m_Rot.y -= 0.02f;
	}

	//==========================oo==============
	//�J������h�炷
	//========================================
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_F1) == true)
	{
		SetShake(50, 0.2f);
	}

	//�J�����̒ʏ�̒����_��ݒ肵������
	NormalCameraSetPosR();

	//========================================
	//�J�������[�h�ɂ���Ď��_��ς���
	//========================================
#ifdef _DEBUG
	//if (CManager::GetInputKeyboard()->GetTrigger(DIK_Z) == true)
	//{
	//	m_ModeTime = 0;
	//	//�J�����^�C�v�̎�ނ�ς���
	//	if (m_CameraType == CAMERATYPE_BIRD)
	//	{
	//		m_CameraType = CAMERATYPE_BESIDE;
	//	}
	//	else if (m_CameraType == CAMERATYPE_BESIDE)
	//	{
	//		m_CameraType = CAMERATYPE_SIDE;
	//	}
	//	else if (m_CameraType == CAMERATYPE_SIDE)
	//	{
	//		m_CameraType = CAMERATYPE_BIRD;
	//	}
	//}
#endif // _DEBUG

	if (m_CameraType == CAMERATYPE_BESIDE)
	{
		m_PosV = D3DXVECTOR3(sinf(m_Rot.y) * -m_BESIDECAMERALENGTH + m_PosR.x, m_PosR.y + 50.0f, cosf(m_Rot.y) * -m_BESIDECAMERALENGTH + m_PosR.z);
		//m_PosV = D3DXVECTOR3(-400.0f,)
	}
	else if (m_CameraType == CAMERATYPE_BOSSBATTLE)
	{
		m_Rot.y = 0.0f;
		m_PosV = D3DXVECTOR3(sinf(m_Rot.y) * -m_BESIDECAMERALENGTH + m_PosR.x, m_PosR.y + 50.0f, cosf(m_Rot.y) * -m_BESIDECAMERALENGTH + m_PosR.z);
	}
	else if (m_CameraType == CAMERATYPE_SIDE)
	{
		m_PosV = D3DXVECTOR3(sinf(m_Rot.y) * -100.0f + m_PosR.x, m_PosR.y, cosf(m_Rot.y) * -300.0f + m_PosR.z);
	}
	else if (m_CameraType == CAMERATYPE_BOSSDEFEAT)
	{
		BossDefeatCameraProcess();
	}
	else if (m_CameraType == CAMERATYPE_TURNINGXZ)
	{
		TurningCameraProcess();
	}
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

	if (m_CameraType == CAMERATYPE_BIRD || m_CameraType == CAMERATYPE_BESIDE || m_CameraType == CAMERATYPE_BOSSDEFEAT || m_CameraType == CAMERATYPE_BOSSBATTLE ||
		m_CameraType == CAMERATYPE_TURNINGXZ)
	{
		//�v���W�F�N�V�����}�g���b�N�X���쐬z
		D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
			D3DXToRadian(45.0f),
			(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
			10.0f,
			20000.0f);
	}
	else if (m_CameraType == CAMERATYPE_SIDE)
	{
		//���s���f�i���ˉe�j
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
	D3DXVECTOR3 adjust = NULL_VECTOR3;//�h��̕␳
	D3DXVECTOR3 ResultPosV = m_PosV;//���ʓI�Ȏx�_
	D3DXVECTOR3 ResultPosR = m_PosR;//���ʓI�Ȓ����_
	if (m_nShakeFrame > 0)
	{
		m_nShakeFrame--;
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
	m_nShakeFrame = nShakeFrame;
	m_fShakePower = fShakePower;
}
//====================================================================================================

//====================================================================
//���ʂ̃J�����̈ʒu��ݒ肵������
//====================================================================
void CCamera::NormalCameraSetPosR()
{
	D3DXVECTOR3 EditPos = NULL_VECTOR3;

	////===================================================
 //   //�J�����̎�ނɂ���āA�����_�̈ʒu��ς���
 //   //===================================================
	//if (CManager::GetCamera()->GetCameraType() == CCamera::CAMERATYPE_BIRD)
	//{
	//	CManager::GetCamera()->SetPosR(D3DXVECTOR3(GetPos().x, 0.0f, GetPos().z));
	//}
	//else if (CManager::GetCamera()->GetCameraType() == CCamera::CAMERATYPE_BESIDE)
	//{
	//	CManager::GetCamera()->SetPosR(D3DXVECTOR3(GetPos().x, 150.0f, GetPos().z));
	//	if (pos.y >= 150.0f)
	//	{
	//		m_bReturnCamera = false;
	//		CManager::GetCamera()->SetPosR(GetPos());
	//	}
	//	else
	//	{
	//		m_bReturnCamera = true;
	//	}
	//}
	//else if (CManager::GetCamera()->GetCameraType() == CCamera::CAMERATYPE_SIDE)
	//{
	//	CManager::GetCamera()->SetPosR(D3DXVECTOR3(GetPos().x, 100.0f, GetPos().z));
	//}
	//========================================================================================================================
	if (CManager::GetCamera()->GetCameraType() == CCamera::CAMERATYPE_BESIDE)
	{
		if (CScene::GetMode() == CScene::MODE_GAME)
		{//�Q�[�����[�h
			if (CGame::GetPlayer() != nullptr)
			{
				if (CGame::GetPlayer()->GetPos().y <= 150.0f)
				{//���݂���Y���W��150f�ȉ���������
					m_PosR.x = CGame::GetPlayer()->GetPos().x;
					m_PosR.y = 150.0f;
					m_PosR.z = CGame::GetPlayer()->GetPos().z;
				}
				else
				{//����ȏ�Ȃ�
					m_PosR = CGame::GetPlayer()->GetPos();
				}

				//m_PosR = D3DXVECTOR3(0.0f,150.0f,0.0f);
			}
		}
		else if (CScene::GetMode() == CScene::MODE_EDIT)
		{//�G�f�B�b�g���[�h
			if (CEdit::GetStageManager()->GetFocusType() == CStageManager::FOCUSTYPE_NORMAL)
			{
				EditPos = CEdit::GetStageManager()->GetPos();
			}
			else
			{
				EditPos = CEdit::GetStageManager()->GetWarpPos();
			}

			if (EditPos.y <= 150.0f)
			{
				m_PosR.y = 150.0f;
			}
			else
			{
				m_PosR.y = EditPos.y;
			}
			m_PosR.x = EditPos.x;
		}
	}
	else if (m_CameraType == CAMERATYPE_BOSSBATTLE)
	{
		m_PosR = D3DXVECTOR3(0.0f,150.0f,0.0f);
	}
}
//====================================================================================================

//====================================================================
//�{�X��|�����Ƃ��̃J�����̏������s��
//====================================================================
void CCamera::BossDefeatCameraProcess()
{
	float fVXaim = 0.0f;//X�����x�N�g��
	float fVYaim = 0.0f;//Y�����x�N�g��
	float fVZaim = 0.0f;//Z�����x�N�g��
	float fVLaim = 0.0f;//�����x�N�g��
	float fLength = 0.0f;
	CObject* pObj = nullptr;
	D3DXVECTOR3 ComparisonPos = NULL_VECTOR3;

	m_ModeTime++;
	//m_PosR = D3DXVECTOR3(0.0f,100.0f,0.0f);
	//==================================
	//�A�C�e���̓����蔻��
	//==================================
	for (int nCntPriority = 0; nCntPriority < CObject::m_nMAXPRIORITY; nCntPriority++)
	{
		pObj = CObject::GetTopObject(nCntPriority);//�g�b�v�I�u�W�F�N�g���擾
		while (pObj != nullptr)
		{
			//���̃I�u�W�F�N�g���i�[
			CObject* pNext = pObj->GetNextObject();

			//��ނ̎擾�i�{�X�Ȃ�J�����Y�[���j
			CObject::TYPE type = pObj->GetType();

			if (type == CObject::TYPE_BOSS)
			{
				ComparisonPos = ((CBoss*)pObj)->GetPos();
				if (m_ModeTime == 1)
				{
					m_DifferenceLength = ComparisonPos - m_PosV;
					m_ZoomSpeed;


					//==========================
					//�R�����x�N�g�������
					//==========================
					fVXaim = ComparisonPos.x - m_PosV.x;
					fVYaim = ComparisonPos.y - m_PosV.y;
					fVZaim = ComparisonPos.z - m_PosV.z;



					fVLaim = sqrtf((fVXaim * fVXaim) + (fVYaim * fVYaim)
						+ (fVZaim * fVZaim));

					fLength = fVLaim / CBoss::m_nMAX_DEFEATSTAGINGTIME;
					m_ZoomSpeed.x = fVXaim / fVLaim * fLength;
					m_ZoomSpeed.y = fVYaim / fVLaim * fLength;
					m_ZoomSpeed.z = fVZaim / fVLaim * fLength;

				}

				m_PosR = ComparisonPos;//�{�X�ɒ����_���Œ�
				m_PosV += m_ZoomSpeed;
				break;
			}
			//���X�g�����ɐi�߂�
			pObj = pNext;
		}
	}
}
//====================================================================================================

//====================================================================
//�J�����̐��񏈗����s��
//====================================================================
void CCamera::TurningCameraProcess()
{
	//�������
	m_Rot.y += m_fTurningRotSpeed;

	//�x�_Y�̈ʒu
	m_PosV.y += m_fTurningSpeedY;

	//�������X�V����
	m_fLength += m_fAddLength;

	//�J�����̐��񏈗�
	m_PosV.x = sinf(m_Rot.y) * m_fLength + m_PosR.x;
	m_PosV.z = cosf(m_Rot.y) * m_fLength + m_PosR.z;
}
//====================================================================================================