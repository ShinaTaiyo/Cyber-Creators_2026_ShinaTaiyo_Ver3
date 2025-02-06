//======================================================================================================================
//
//�P�P���Q�X���F�t�F�[�Y�}�l�[�W���[�����[phasemanager.cpp]
//Author:ShinaTaiyo
//
//======================================================================================================================

//===============================================================
//�C���N���[�h
//===============================================================
#include "phasemanager.h"
#include "aimodel.h"
#include "manager.h"
#include "stagemanager.h"
#include "game.h"
#include "fade.h"
//======================================================================================================================

//===============================================================
//�ÓI�����o�錾
//===============================================================
list<CPhaseManager::PhaseSaveInfo> CPhaseManager::s_PhaseList = {};
int CPhaseManager::s_nNowPhase = 0;
int CPhaseManager::s_MaxPhase = 0;
int CPhaseManager::s_nNowStage = 0;
bool CPhaseManager::s_bStartFade = false;
//======================================================================================================================

//===============================================================
//�R���X�g���N�^
//===============================================================
CPhaseManager::CPhaseManager(int nNumStage, int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : 
	CObject(nPri, bUseintPri, type, ObjType)
{
	s_nNowPhase = 0;
	s_MaxPhase = 0;
	s_nNowStage = 0;

	if (nNumStage >= static_cast<int>(CStageManager::WORLDTYPE::MAX))
	{
		nNumStage = static_cast<int>(CStageManager::WORLDTYPE::MAX) - 1;
	}
	s_bStartFade = false;
}
//======================================================================================================================

//===============================================================
//�f�X�g���N�^
//===============================================================
CPhaseManager::~CPhaseManager()
{

}
//======================================================================================================================

//===============================================================
//����������
//===============================================================
HRESULT CPhaseManager::Init()
{
	CObject::Init();
	CEventManager::Create(DBG_NEW CNowEvent_NextPhase(CUi::Create(CUi::UITYPE::STAGETEXT, CObject2D::POLYGONTYPE::SENTERROLLING, 200.0f, 100.0f, 100, false,
		D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT / 2 - 180.0f, 0.0f), D3DXVECTOR3(sinf(D3DX_PI * -0.5f) * 15.0f, cosf(D3DX_PI * -0.5f) * 15.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)), s_nNowStage, 80.0f, 80.0f));
	return S_OK;
}
//======================================================================================================================

//===============================================================
//�I������
//===============================================================
void CPhaseManager::Uninit()
{
	CObject::Uninit();
}
//======================================================================================================================

//===============================================================
//�X�V����
//===============================================================
void CPhaseManager::Update()
{
	AdvancePhase();
	CObject::Update();
}
//======================================================================================================================

//===============================================================
//�`�揈��
//===============================================================
void CPhaseManager::Draw()
{
	CObject::Draw();
}
//======================================================================================================================

//===============================================================
//���S�t���O�ݒ菈��
//===============================================================
void CPhaseManager::SetDeath()
{
	if (GetUseDeath() == true)
	{
		s_PhaseList.clear();
	}
	CObject::SetDeath();
}
//======================================================================================================================

//===============================================================
//�t�F�[�Y�}�l�[�W���[�ɏ���ݒ肷��
//===============================================================
void CPhaseManager::PushPhaseInfo(D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, D3DXVECTOR3 Scale, 
	int nLife, int nEnemyType, int nTypeNum, int nPhaseNum, float fNormalSpeed, float fSensingRange, 
	int nNumDivision, vector<CEnemy::MoveAiInfo> VecMoveAi)
{
	PhaseSaveInfo Info = {};
	Info.Pos = Pos;
	Info.Rot = Rot;
	Info.Scale = Scale;
	Info.nLife = nLife;
	Info.nEnemyType = nEnemyType;
	Info.nTypeNum = nTypeNum;
	Info.nPhaseNum = nPhaseNum;
	Info.VecMoveAi = VecMoveAi;
	Info.fNormalSpeed = fNormalSpeed;
	Info.fSensingRange = fSensingRange;
	Info.nNumDivision = nNumDivision;

	//�t�F�[�Y�ő吔�𒴂��Ă�����X�V
	if (nPhaseNum > s_MaxPhase)
	{
		s_MaxPhase = nPhaseNum;
	}

	s_PhaseList.push_back(Info);
}
//======================================================================================================================

//===============================================================
//���̃t�F�[�Y�Ɉڍs���鏈��
//===============================================================
void CPhaseManager::AdvancePhase()
{
	if (CEnemy::GetNumEnemy() <= 0 && s_nNowPhase <= s_MaxPhase)
	{

		CEnemy::ENEMYTYPE EnemyType = {};
		CEnemy* pEnemy = nullptr;
		vector<CAIModel*> VecMoveAi = {};//�ړ�AI��vector
		for (auto it : s_PhaseList)
		{
			if (it.nPhaseNum == s_nNowPhase)
			{
				EnemyType = static_cast<CEnemy::ENEMYTYPE>(it.nEnemyType);

				//�ړ�AI���ݒ肳��Ă�����
				if (it.VecMoveAi.size() > 0)
				{
					for (const auto it2 : it.VecMoveAi)
					{
						CAIModel* pAiModel = CAIModel::Create(CAIModel::AIMODELTYPE::MOVEPOINT, it2.Pos, it2.Rot, it2.Scale, nullptr);
						pAiModel->GetDrawInfo().SetUseDraw(false);
						pAiModel->GetDrawInfo().SetUseShadow(false);
						VecMoveAi.push_back(pAiModel);
					}
				}

				//�G�̎�ނɂ���Đ���������̂�ς���
				switch (EnemyType)
				{
				case CEnemy::ENEMYTYPE::SHOTWEAK:
					pEnemy = CShotWeakEnemy::Create(static_cast<CShotWeakEnemy::SHOTWEAKENEMYTYPE>(it.nTypeNum), it.nLife, it.nPhaseNum, it.Pos, it.Rot, it.Scale);
					break;
				case CEnemy::ENEMYTYPE::DIVEWEAK:
					pEnemy = CDiveWeakEnemy::Create(static_cast<CDiveWeakEnemy::DIVEWEAKENEMYTYPE>(it.nTypeNum), it.nLife, it.nPhaseNum, it.Pos, it.Rot, it.Scale, it.nNumDivision);
					break;
				case CEnemy::ENEMYTYPE::IDLE:
					pEnemy = CIdleEnemy::Create(static_cast<CIdleEnemy::IDLEENEMYTYPE>(it.nTypeNum), it.nLife, it.nPhaseNum, it.Pos, it.Rot, it.Scale);
					break;
				default:
					break;
				}

				pEnemy->SetNormalSpeed(it.fNormalSpeed);
				pEnemy->SetSensingRange(it.fSensingRange);

				auto CopyVec = move(VecMoveAi);
				//�ړ�AI��ݒ肷��
				pEnemy->SetVecMoveAiInfo(CopyVec);
			}

		}
		s_nNowPhase++;
		CEventManager::Create(DBG_NEW CNowEvent_NextPhase(CUi::Create(CUi::UITYPE::PHASETEXT, CObject2D::POLYGONTYPE::SENTERROLLING, 200.0f, 100.0f, 100, false,
			D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(sinf(D3DX_PI * -0.5f) * 10.0f, cosf(D3DX_PI * -0.5f) * 10.0f, 0.0f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)),s_nNowPhase, 80.0f, 80.0f));

	}

	if (s_nNowStage < static_cast<int>(CStageManager::WORLDTYPE::MAX))
	{
		if (CEnemy::GetNumEnemy() <= 0 && s_nNowPhase == s_MaxPhase + 1 && s_bStartFade == false)
		{
			s_PhaseList.clear();//���̃X�e�[�W�ɍs���̂Ńt�F�[�Y����������
			s_nNowPhase = 0;    //���̃X�e�[�W�ɍs���̂Ńt�F�[�Y���ŏ�����

			s_nNowStage++;      //�X�e�[�W�ԍ������ɐi�߂�

			if (s_nNowStage == static_cast<int>(CStageManager::WORLDTYPE::MAX))
			{//�z��I�ɃC���N�������g��ɃX�e�[�W���ɂȂ�����I���Ȃ̂ŁA���U���g�Ɉڍs
				CManager::GetSceneFade()->SetSceneFade(CFade::FADEMODE_IN, CScene::MODE_RESULT);
			}
			else
			{
				CGame::GetStageManager()->LoadMapTxt(s_nNowStage);//���̃X�e�[�W�����[�h����
				CGame::GetPlayer()->GetPosInfo().SetPos(CGame::GetStageManager()->GetSpawnPoint());
				CEventManager::Create(DBG_NEW CNowEvent_NextPhase(CUi::Create(CUi::UITYPE::STAGETEXT, CObject2D::POLYGONTYPE::SENTERROLLING, 200.0f, 100.0f, 100, false,
					D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT / 2 - 180.0f, 0.0f), D3DXVECTOR3(sinf(D3DX_PI * -0.5f) * 15.0f, cosf(D3DX_PI * -0.5f) * 15.0f, 0.0f),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)), s_nNowStage, 80.0f, 80.0f));
			}
		}
	}
}
//======================================================================================================================

//===============================================================
//��������
//===============================================================
CPhaseManager* CPhaseManager::Create()
{
	CPhaseManager* pPhaseManager = DBG_NEW CPhaseManager(3);

	pPhaseManager->Init();//����������

	return pPhaseManager;
}
//======================================================================================================================