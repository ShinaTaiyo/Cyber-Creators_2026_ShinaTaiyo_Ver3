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
list<CPhaseManager::PhaseSaveInfo> CPhaseManager::s_PhaseList = {};//�t�F�[�Y��񃊃X�g
int CPhaseManager::s_nNowPhase = 0;                                //���݂̃t�F�[�Y
int CPhaseManager::s_MaxPhase = 0;                                 //�ő�t�F�[�Y
int CPhaseManager::s_nNowStage = 0;                                //���݂̃X�e�[�W
bool CPhaseManager::s_bStartFade = false;                          //�t�F�[�h���J�n���邩�ǂ���
//======================================================================================================================

//===============================================================
//�R���X�g���N�^
//===============================================================
CPhaseManager::CPhaseManager(int nNumStage, int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : 
	CObject(nPri, bUseintPri, type, ObjType)
{
	s_nNowPhase = 0;//���݂̃t�F�[�Y��������
	s_MaxPhase = 0; //�ő�t�F�[�Y��������
	s_nNowStage = 0;//���݂̃X�e�[�W��������

	if (nNumStage >= static_cast<int>(CStageManager::WORLDTYPE::MAX))
	{//�X�e�[�W����ݒ�
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
	CObject::Init();//�I�u�W�F�N�g�̏���������

	//�t�F�[�Y�ڍs�C�x���g�𐶐�
	CEventManager::Create(DBG_NEW CNowEvent_NextPhase(CUi::Create(CUi::UITYPE::STAGETEXT,false, CObject2D::POLYGONTYPE::SENTERROLLING, 200.0f, 100.0f, 100, false,
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
	CObject::Uninit();//�I�u�W�F�N�g�̏I������
}
//======================================================================================================================

//===============================================================
//�X�V����
//===============================================================
void CPhaseManager::Update()
{
	AdvancePhase();//�t�F�[�Y�ڍs����
	CObject::Update();//�I�u�W�F�N�g�̍X�V����
}
//======================================================================================================================

//===============================================================
//�`�揈��
//===============================================================
void CPhaseManager::Draw()
{
	CObject::Draw();//�I�u�W�F�N�g�̕`�揈��
}
//======================================================================================================================

//===============================================================
//���S�t���O�ݒ菈��
//===============================================================
void CPhaseManager::SetDeath()
{
	if (GetUseDeath() == true)
	{//���S�t���O���g�p����Ȃ�
		s_PhaseList.clear();//�t�F�[�Y�����N���A����
	}
	CObject::SetDeath();//�I�u�W�F�N�g�̎��S�t���O�ݒ菈��
}
//======================================================================================================================

//===============================================================
//�t�F�[�Y�}�l�[�W���[�ɏ���ݒ肷��
//===============================================================
void CPhaseManager::PushPhaseInfo(D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, D3DXVECTOR3 Scale, 
	int nLife, int nEnemyType, int nTypeNum, int nPhaseNum, float fNormalSpeed, float fSensingRange, 
	int nNumDivision, vector<CEnemy::MoveAiInfo> VecMoveAi)
{
	PhaseSaveInfo Info = {};                     //�t�F�[�Y���̏�����
	Info.Pos = Pos;                              //�ʒu
	Info.Rot = Rot;                              //����
	Info.Scale = Scale;                          //�g�嗦
	Info.nLife = nLife;                          //�̗�
	Info.nEnemyType = nEnemyType;                //�G�̃^�C�v
	Info.nTypeNum = nTypeNum;                    //�^�C�v�ԍ�
	Info.nPhaseNum = nPhaseNum;                  //�t�F�[�Y�ԍ�
	Info.VecMoveAi = VecMoveAi;                  //�ړ�AI�̓��I�z��
	Info.fNormalSpeed = fNormalSpeed;            //�ʏ푬�x
	Info.fSensingRange = fSensingRange;          //���G�͈�
	Info.nNumDivision = nNumDivision;            //�����

	//�t�F�[�Y�ő吔�𒴂��Ă�����X�V
	if (nPhaseNum > s_MaxPhase)
	{
		s_MaxPhase = nPhaseNum;                  //�ő�t�F�[�Y�����X�V
	}

	s_PhaseList.push_back(Info);                 //�t�F�[�Y���X�g�Ɋi�[
}
//======================================================================================================================

//===============================================================
//���̃t�F�[�Y�Ɉڍs���鏈��
//===============================================================
void CPhaseManager::AdvancePhase()
{
	if (s_nNowStage < static_cast<int>(CStageManager::WORLDTYPE::MAX))
	{
		if (CEnemy::GetNumEnemy() <= 0 && s_nNowPhase <= s_MaxPhase)
		{

			CEnemy::ENEMYTYPE EnemyType = {};//�G�^�C�v�i�[�p 
			CEnemy* pEnemy = nullptr;        //�G�ւ̃|�C���^
			vector<CAIModel*> VecMoveAi = {};//�ړ�AI�̓��I�z��

			//================================
			//�t�F�[�Y���𔽉f
			//================================
			for (auto it : s_PhaseList)
			{
				if (it.nPhaseNum == s_nNowPhase)
				{//�t�F�[�Y���X�g�Ɋi�[����Ă���G��񂪌��݂̃t�F�[�Y�ԍ��ƈ�v������
					EnemyType = static_cast<CEnemy::ENEMYTYPE>(it.nEnemyType);//�G�^�C�v���i�[

					//�ړ�AI���ݒ肳��Ă�����
					if (it.VecMoveAi.size() > 0)
					{//�G�̈ړ�AI�̓��I�z��̒��g�����݂��Ă�����
						for (const auto it2 : it.VecMoveAi)
						{//�ړ�AI�̓��I�z��̏������ƂɈړ�AI�𐶐�
							CAIModel* pAiModel = CAIModel::Create(CAIModel::AIMODELTYPE::MOVEPOINT, it2.Pos, it2.Rot, it2.Scale, nullptr);
							pAiModel->GetDrawInfo().SetUseDraw(false);//�`�悵�Ȃ�
							pAiModel->GetDrawInfo().SetUseShadow(false);//�e��`�悵�Ȃ�
							VecMoveAi.push_back(pAiModel);//���I�z��ɏ����i�[
						}
					}

					//�G�̎�ނɂ���Đ���������̂�ς���
					switch (EnemyType)
					{
					case CEnemy::ENEMYTYPE::SHOTWEAK://�ˌ��Ɏア�G
						pEnemy = CShotWeakEnemy::Create(static_cast<CShotWeakEnemy::SHOTWEAKENEMYTYPE>(it.nTypeNum), it.nLife, it.nPhaseNum, it.Pos, it.Rot, it.Scale);
						break;
					case CEnemy::ENEMYTYPE::DIVEWEAK://�_�C�u�Ɏア�G
						pEnemy = CDiveWeakEnemy::Create(static_cast<CDiveWeakEnemy::DIVEWEAKENEMYTYPE>(it.nTypeNum), it.nLife, it.nPhaseNum, it.Pos, it.Rot, it.Scale, it.nNumDivision);
						break;
					case CEnemy::ENEMYTYPE::IDLE:    //�������Ȃ��G
						pEnemy = CIdleEnemy::Create(static_cast<CIdleEnemy::IDLEENEMYTYPE>(it.nTypeNum), it.nLife, it.nPhaseNum, it.Pos, it.Rot, it.Scale);
						break;
					default:
						break;
					}

					pEnemy->SetNormalSpeed(it.fNormalSpeed);  //�ʏ푬�x��ݒ�
					pEnemy->SetSensingRange(it.fSensingRange);//���G�͈͂�ݒ�

					auto CopyVec = move(VecMoveAi);           //��L�̏�񂩂�i�[�����ړ�AI�̓��I�z���ݒ肳����ϐ��Ɉړ�
					pEnemy->SetVecMoveAiInfo(CopyVec);        //�ړ�AI�̓��I�z���ݒ�
				}

			}
			//====================================================================================================================================================

			s_nNowPhase++;//���݂̃t�F�[�Y�ԍ������ɐi�߂�                                    
			if (s_nNowPhase <= s_MaxPhase)
			{//�t�F�[�Y�����ő�𒴂��Ă��Ȃ���΃t�F�[�Y�ڍs�C�x���g�𐶐�
				CEventManager::Create(DBG_NEW CNowEvent_NextPhase(CUi::Create(CUi::UITYPE::PHASETEXT,false,CObject2D::POLYGONTYPE::SENTERROLLING, 200.0f, 100.0f, 100, false,
					D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(sinf(D3DX_PI * -0.5f) * 10.0f, cosf(D3DX_PI * -0.5f) * 10.0f, 0.0f),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)), s_nNowPhase, 80.0f, 80.0f));
			}

		}
		if (CEnemy::GetNumEnemy() <= 0 && s_nNowPhase == s_MaxPhase + 1 && s_bStartFade == false)
		{//�G�����Ȃ��Ȃ��Ă��āA���݂̃t�F�[�Y���ő�t�F�[�Y����Ȃ����

			s_PhaseList.clear();//���̃X�e�[�W�ɍs���̂Ńt�F�[�Y����������
			s_nNowPhase = 0;    //���̃X�e�[�W�ɍs���̂Ńt�F�[�Y���ŏ�����

			s_nNowStage++;      //�X�e�[�W�ԍ������ɐi�߂�

			if (s_nNowStage == static_cast<int>(CStageManager::WORLDTYPE::MAX))
			{//�z��I�ɃC���N�������g��ɃX�e�[�W���ɂȂ�����I���Ȃ̂ŁA���U���g�Ɉڍs
				CManager::GetSceneFade()->SetSceneFade(CFade::FADEMODE_IN, CScene::MODE_RESULT);
				CGame::SetGameClear(true);//�Q�[�����N���A�����I
			}
			else
			{//����ȊO�Ȃ玟�̃X�e�[�W���Ă�
				CGame::GetStageManager()->LoadMapTxt(s_nNowStage);//���̃X�e�[�W�����[�h����
				CGame::GetPlayer()->GetPosInfo().SetPos(CGame::GetStageManager()->GetSpawnPoint());
				CEventManager::Create(DBG_NEW CNowEvent_NextPhase(CUi::Create(CUi::UITYPE::STAGETEXT,false,CObject2D::POLYGONTYPE::SENTERROLLING, 200.0f, 100.0f, 100, false,
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
	CPhaseManager* pPhaseManager = DBG_NEW CPhaseManager(3);//�t�F�[�Y�}�l�[�W���[�𐶐�

	pPhaseManager->Init();                                  //����������

	return pPhaseManager;
}
//======================================================================================================================