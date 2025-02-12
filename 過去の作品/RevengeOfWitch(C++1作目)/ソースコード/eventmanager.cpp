//======================================================================================================================================
//
//�X���P�T���F�{�X�o�ꉉ�o�����[eventmanager.cpp]
//Author:ShinaTaiyo
//
//======================================================================================================================================

//=====================================================================
//�C���N���[�h
//=====================================================================
#include "eventmanager.h"
#include "camera.h"
#include "boss.h"
#include "renderer.h"
#include "object.h"
#include "sound.h"
#include "fade.h"
#include "debugtext.h"
#include "manager.h"
#include "calculation.h"
#include "particle2d.h"
#include "ui.h"
//======================================================================================================================================

//=====================================================================
//�R���X�g���N�^
//=====================================================================
CEventManager::CEventManager() : m_nEventPattern(0),m_nCntEventTime(0),m_NowEventType(EVENTTYPE_NONE),m_bEventProgress(false),
m_pMagicCircle(nullptr),m_pBossBattleText(nullptr),m_pDirectionFade(nullptr), m_pBossCutIn(nullptr),m_pBossNameUi(nullptr),
m_BossAppearType(BOSSAPPEAR_KINGSLIME)
{

}
//======================================================================================================================================

//=====================================================================
//�f�X�g���N�^
//=====================================================================
CEventManager::~CEventManager()
{

}
//======================================================================================================================================

//=====================================================================
//����������
//=====================================================================
HRESULT CEventManager::Init()
{
	CObject::Init();
	return S_OK;
}
//======================================================================================================================================

//=====================================================================
//�I������
//=====================================================================
void CEventManager::Uninit()
{
	CObject::Uninit();
}
//======================================================================================================================================

//=====================================================================
//�X�V����
//=====================================================================
void CEventManager::Update()
{
	
	if (m_bEventProgress == true)
	{
		m_nCntEventTime++;
		switch (m_NowEventType)
		{
		case EVENTTYPE_BOSSAPPEAR:
			BossAppearEventProcess();
			break;
		default:
			break;
		}
	}

	CObject::Update();
}
//======================================================================================================================================

//=====================================================================
//�`�揈��
//=====================================================================
void CEventManager::Draw()
{
	CObject::Draw();
}
//======================================================================================================================================

//=====================================================================
//���S�t���O�ݒ菈��
//=====================================================================
void CEventManager::SetDeath()
{

	CObject::SetDeath();
}
//======================================================================================================================================

//=====================================================================
//�{�X�o��C�x���g�̏���
//=====================================================================
void CEventManager::BossAppearEventProcess()
{
	CObject* pObj = nullptr;
	CBoss* pBoss = nullptr;
	D3DXVECTOR3 BossPos = NULL_VECTOR3;
	CCamera* pCamera = CManager::GetCamera();//�J�����ւ̃|�C���^

	float fCameraLength = 0.0f;    //�J�����̒����_���񋗗�
	float fCemeraSpeedY = 0.0f;    //�J������Y�����̈ړ���
	float fCameraAddLength = 0.0f; //�J�����ƒ����_�̋����̉��Z��
	float fCameraRotSpeed = 0.0f;  //�J�����̉�]���x

	float fRandRot = 0.0f;//��������
	switch (m_nEventPattern)
	{
	case 0:
		if (m_nCntEventTime == 1)
		{
			CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_WARNING);
			m_pDirectionFade = CDirectionFade::Create();
			m_pDirectionFade->SetFade(CFade::FADEMODE_IN);
			m_pMagicCircle = CUi::Create(CUi::UITYPE_MAGICCIRCLE, CObject2D::POLYGONTYPE01_SENTERROLLING, 700.0f, 700.0f,
				SENTER_VECTOR3, NULL_VECTOR3, NULL_VECTOR3, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));//���@�w�𐶐�
			m_pMagicCircle->SetPolygonRotSpeed(0.1f);//���@�w����]������
			m_pMagicCircle->SetUseBlinking(true, 30, 0.0f);//�_�ł�����
			m_pMagicCircle->SetUseDeath(false);//���S�t���O�𔭓������Ȃ�

			m_pBossBattleText = CUi::Create(CUi::UITYPE_BOSSBATTLE_TEXT, CObject2D::POLYGONTYPE01_SENTERROLLING, 800.0f, 200.0f,
				D3DXVECTOR3(SCREEN_WIDTH + 300.0f, SENTER_VECTOR3.y, 0.0f),D3DXVECTOR3(-20.0f,0.0f,0.0f), NULL_VECTOR3, NORMAL_COL);
			m_pBossBattleText->SetPolygonRotSpeed(-0.4f);
			m_pBossBattleText->SetUseUiEffect(true, 10, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		}

		if (m_nCntEventTime < 120)
		{
			fRandRot = CCalculation::CalculationRandVecXY();
			CParticle2D::Create(SENTER_VECTOR3, D3DXVECTOR3(sinf(fRandRot) * 20.0f, cosf(fRandRot) * 20.0f, 0.0f), NULL_VECTOR3, CObject2D::POLYGONTYPE01_SENTERROLLING,
				60, 50.0f, 50.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			if (m_pBossBattleText->GetPos().x >= SENTER_VECTOR3.x - 20.0f && m_pBossBattleText->GetPos().x <= SENTER_VECTOR3.x + 20.0f)
			{//���S�ɂ��ǂ蒅�����������蓮����
				m_pBossBattleText->SetPolygonRotSpeed(-0.01f);
				m_pBossBattleText->SetMove(D3DXVECTOR3(-0.1f, 0.0f, 0.0f));
			}
		}
		if (m_nCntEventTime == 120)
		{
			//�Ăѓ�����
			m_pBossBattleText->SetPolygonRotSpeed(-0.4f);
			m_pBossBattleText->SetMove(D3DXVECTOR3(-20.0f, 0.0f, 0.0f));

			m_pDirectionFade->SetFade(CFade::FADEMODE_OUT);
			if (m_pMagicCircle != nullptr)
			{
				m_pMagicCircle->SetUseDeath(true);
				m_pMagicCircle->SetDeath();
				m_pMagicCircle = nullptr;
			}
		}

		if (m_nCntEventTime == 180)
		{
			CManager::GetSound()->StopSound(CSound::SOUND_LABEL_SE_WARNING);
			NextPattern();
		}
		break;
	case 1:
		if (m_nCntEventTime == 1)
		{
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
						pBoss = (CBoss*)pObj;

						BossPos = pBoss->GetPos();//�{�X�̒��S�_���擾����
						break;
					}
					//���X�g�����ɐi�߂�
					pObj = pNext;
				}
			}

			CManager::GetCamera()->SetCameraType(CCamera::CAMERATYPE_TURNINGXZ);
			CManager::GetCamera()->SetTurningInfo(1000.0f, -0.001f,BossPos,BossPos.y + 1500.0f, -0.01f, 0.1f);
		}

		if (m_nCntEventTime == 60)
		{
			NextPattern();//�p�^�[�������ɐi�߂�
		}
		break;
	case 2:
		fCemeraSpeedY = pCamera->GetTurningSpeedY();
		fCemeraSpeedY -= 0.01f * m_nCntEventTime;
		pCamera->SetTurningSpeedY(fCemeraSpeedY);//Y�̈ړ��ʂ�����������

		pCamera->SetAddLength(-7.0f);//�����_�Ƃ̋����𗣂��Ă���

		fCameraRotSpeed = pCamera->GetTurningRotSpeed();
		if (m_nCntEventTime % 3 == 0)
		{
			fCameraRotSpeed += 0.007f;
			pCamera->SetTurningRotSpeed(fCameraRotSpeed);
		}

		if (m_nCntEventTime == 89)
		{
			NextPattern();
		}
		break;
	case 3:
		pCamera->SetTurningSpeedY(-0.3f);
		pCamera->SetTurningRotSpeed(0.002f);
		pCamera->SetAddLength(-0.02f);

		if (m_nCntEventTime == 60)
		{
			NextPattern();
		}
		break;
	case 4:
		if (m_nCntEventTime == 1)
		{
			switch (m_BossAppearType)
			{
			case BOSSAPPEAR_KINGSLIME:
				m_pBossCutIn = CUi::Create(CUi::UITYPE_CUTIN_KINGSLIME, CObject2D::POLYGONTYPE01_SENTERROLLING, 500.0f, 500.0f,
					D3DXVECTOR3(0.0f - 250.0f, SENTER_VECTOR3.y + 200.0f, 0.0f), D3DXVECTOR3(30.0f, 0.0f, 0.0f), NULL_VECTOR3, NORMAL_COL);

				m_pBossNameUi = CUi::Create(CUi::UITYPE_KINGSLIME_TEXT, CObject2D::POLYGONTYPE01_SENTERROLLING, 1000.0f, 250.0f,
					D3DXVECTOR3(SCREEN_WIDTH + 500.0f, SENTER_VECTOR3.y - 200.0f, 0.0f), D3DXVECTOR3(-30.0f, 0.0f, 0.0f), NULL_VECTOR3, NORMAL_COL);
				break;
			case BOSSAPPEAR_DEATHENAGA:
				m_pBossCutIn = CUi::Create(CUi::UITYPE_CUTIN_DEATHENAGA, CObject2D::POLYGONTYPE01_SENTERROLLING, 500.0f, 500.0f,
					D3DXVECTOR3(0.0f - 250.0f, SENTER_VECTOR3.y + 200.0f, 0.0f), D3DXVECTOR3(30.0f, 0.0f, 0.0f), NULL_VECTOR3, NORMAL_COL);

				m_pBossNameUi = CUi::Create(CUi::UITYPE_DEATHENAGA_TEXT, CObject2D::POLYGONTYPE01_SENTERROLLING, 1000.0f, 250.0f,
					D3DXVECTOR3(SCREEN_WIDTH + 500.0f, SENTER_VECTOR3.y - 200.0f, 0.0f), D3DXVECTOR3(-30.0f, 0.0f, 0.0f), NULL_VECTOR3, NORMAL_COL);
				break;
			case BOSSAPPEAR_MRSTRONG:
				m_pBossCutIn = CUi::Create(CUi::UITYPE_CUTIN_MRSTRONG, CObject2D::POLYGONTYPE01_SENTERROLLING, 500.0f, 500.0f,
					D3DXVECTOR3(0.0f - 250.0f, SENTER_VECTOR3.y + 200.0f, 0.0f), D3DXVECTOR3(30.0f, 0.0f, 0.0f), NULL_VECTOR3, NORMAL_COL);

				m_pBossNameUi = CUi::Create(CUi::UITYPE_MRSTRONG_TEXT, CObject2D::POLYGONTYPE01_SENTERROLLING, 1000.0f, 250.0f,
					D3DXVECTOR3(SCREEN_WIDTH + 500.0f, SENTER_VECTOR3.y - 200.0f, 0.0f), D3DXVECTOR3(-30.0f, 0.0f, 0.0f), NULL_VECTOR3, NORMAL_COL);
				break;
			case BOSSAPPEAR_RILEY:
				m_pBossCutIn = CUi::Create(CUi::UITYPE_CUTIN_RILEY, CObject2D::POLYGONTYPE01_SENTERROLLING, 500.0f, 500.0f,
					D3DXVECTOR3(0.0f - 250.0f, SENTER_VECTOR3.y + 200.0f, 0.0f), D3DXVECTOR3(30.0f, 0.0f, 0.0f), NULL_VECTOR3, NORMAL_COL);

				m_pBossNameUi = CUi::Create(CUi::UITYPE_RILEY_TEXT, CObject2D::POLYGONTYPE01_SENTERROLLING, 1000.0f, 250.0f,
					D3DXVECTOR3(SCREEN_WIDTH + 500.0f, SENTER_VECTOR3.y - 200.0f, 0.0f), D3DXVECTOR3(-30.0f, 0.0f, 0.0f), NULL_VECTOR3, NORMAL_COL);
				break;
			case BOSSAPPEAR_DRAIYA:
				m_pBossCutIn = CUi::Create(CUi::UITYPE_CUTIN_DRAIYA, CObject2D::POLYGONTYPE01_SENTERROLLING, 500.0f, 500.0f,
					D3DXVECTOR3(0.0f - 250.0f, SENTER_VECTOR3.y + 200.0f, 0.0f), D3DXVECTOR3(30.0f, 0.0f, 0.0f), NULL_VECTOR3, NORMAL_COL);

				m_pBossNameUi = CUi::Create(CUi::UITYPE_DRAIYA_TEXT, CObject2D::POLYGONTYPE01_SENTERROLLING, 1000.0f, 250.0f,
					D3DXVECTOR3(SCREEN_WIDTH + 500.0f, SENTER_VECTOR3.y - 200.0f, 0.0f), D3DXVECTOR3(-30.0f, 0.0f, 0.0f), NULL_VECTOR3, NORMAL_COL);
				break;
			case BOSSAPPEAR_GABRIEL:
				m_pBossCutIn = CUi::Create(CUi::UITYPE_CUTIN_GABRIEL, CObject2D::POLYGONTYPE01_SENTERROLLING, 500.0f, 500.0f,
					D3DXVECTOR3(0.0f - 250.0f, SENTER_VECTOR3.y + 200.0f, 0.0f), D3DXVECTOR3(30.0f, 0.0f, 0.0f), NULL_VECTOR3, NORMAL_COL);

				m_pBossNameUi = CUi::Create(CUi::UITYPE_GABRIEL_TEXT, CObject2D::POLYGONTYPE01_SENTERROLLING, 1000.0f, 250.0f,
					D3DXVECTOR3(SCREEN_WIDTH + 500.0f, SENTER_VECTOR3.y - 200.0f, 0.0f), D3DXVECTOR3(-30.0f, 0.0f, 0.0f), NULL_VECTOR3, NORMAL_COL);
				break;
			default:
				break;
			}
		}

		if (m_pBossCutIn->GetPos().x >= SENTER_VECTOR3.x + 200.0f - 20.0f && m_pBossCutIn->GetPos().x <= SENTER_VECTOR3.x + 200.0f + 20.0f)
		{
			m_pBossCutIn->SetUseUiEffect(true, 90, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			m_pBossCutIn->SetMove(D3DXVECTOR3(1.0f, 0.0f, 0.0f));

			m_pBossNameUi->SetMove(D3DXVECTOR3(-1.0f, 0.0f, 0.0f));
			NextPattern();
		}
		break;
	case 5:
		if (m_nCntEventTime == 120)
		{
			m_pBossCutIn->SetMove(D3DXVECTOR3(30.0f, 0.0f, 0.0f));
			m_pBossNameUi->SetMove(D3DXVECTOR3(-30.0f, 0.0f, 0.0f));
			NextPattern();
		}
		break;
	case 6:
		if (m_nCntEventTime == 60)
		{//�C�x���g�I��
			ResetPattern();
			CManager::GetCamera()->SetCameraType(CCamera::CAMERATYPE_BOSSBATTLE);

			SetDeathOnly();//�{�̃I�u�W�F�N�g�ȊO������
		}
		break;
	default:
		break;
	}

	CManager::GetDebugText()->PrintDebugText("���݂̃p�^�[���F%d�A���݂̃p�^�[�����ԁF%d\n", m_nEventPattern, m_nCntEventTime);

}
//======================================================================================================================================

//=====================================================================
//�{�̈ȊO�̃I�u�W�F�N�g��S�ď�������
//=====================================================================
void CEventManager::SetDeathOnly()
{
	if (m_pMagicCircle != nullptr)
	{
		m_pMagicCircle->SetUseDeath(true);
		m_pMagicCircle->SetDeath();
		m_pMagicCircle = nullptr;
	}

	if (m_pDirectionFade != nullptr)
	{
		m_pDirectionFade->SetUseDeath(true);
		m_pDirectionFade->SetDeath();
		m_pDirectionFade = nullptr;
	}

	if (m_pBossBattleText != nullptr)
	{
		m_pBossBattleText->SetUseDeath(true);
		m_pBossBattleText->SetDeath();
		m_pBossBattleText = nullptr;
	}

	if (m_pBossCutIn != nullptr)
	{
		m_pBossCutIn->SetUseDeath(true);
		m_pBossCutIn->SetDeath();
		m_pBossCutIn = nullptr;
	}

	if (m_pBossNameUi != nullptr)
	{
		m_pBossNameUi->SetUseDeath(true);
		m_pBossNameUi->SetDeath();
		m_pBossNameUi = nullptr;
	}
}
//======================================================================================================================================

//=====================================================================
//�p�^�[�������ɐi�߂�
//=====================================================================
void CEventManager::NextPattern()
{
	m_nCntEventTime = 0;
	m_nEventPattern++;
}
//======================================================================================================================================

//=====================================================================
//�p�^�[�������Z�b�g����
//=====================================================================
void CEventManager::ResetPattern()
{
	m_nCntEventTime = 0;
	m_nEventPattern = 0;
	m_bEventProgress = false;
	m_NowEventType = EVENTTYPE_NONE;
}
//======================================================================================================================================

//=====================================================================
//�C�x���g�̐���
//=====================================================================
CEventManager* CEventManager::Create(EVENTTYPE type)
{
	CEventManager* pEventManager = DBG_NEW CEventManager;

	CEventManager::Init();//����������
	m_NowEventType = type;//�C�x���g�̎��

	return pEventManager;
}
//======================================================================================================================================