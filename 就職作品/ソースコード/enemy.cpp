//============================================================================================================================================
//
//�P�P���Q�T���F�G�����[enemy.cpp]
//Author:ShinaTaiyo
//
//============================================================================================================================================

//====================================================================================
//�C���N���[�h
//====================================================================================
#include "enemy.h"
#include "manager.h"
#include "objectXInfo.h"
#include "game.h"
#include "calculation.h"
#include "debugtext.h"
#include "collision.h"
#include "damage.h"
#include "particle.h"
#include "phasemanager.h"
#include "input.h"
#include "effect.h"
#include "block.h"
#include "camera.h"
#include "attack.h"
//============================================================================================================================================

//====================================================================================
//�ÓI�����o�錾
//====================================================================================
//const string CEnemy::m_aENEMY_FILENAME[static_cast<int>(ENEMYTYPE::MAX)] =
//{
//	"data\\MODEL\\Enemy\\Normal\\noobSlime.x"
//};
int CEnemy::m_nNumEnemy = 0;

//====================================================================================
//�R���X�g���N�^
//====================================================================================
CEnemy::CEnemy(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CObjectX(nPri, bUseintPri, type, ObjType),
m_Type(ENEMYTYPE::SHOTWEAK), m_VecMoveAi(), m_MoveAiSavePos(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),m_nCntTime(0),m_nIdxMoveAi(0), m_nPhaseNum(0),m_pEnemyMove(DBG_NEW CEnemyMove_AI()),
m_fRotMove(0.0f),m_fSensingRange(0.0f),m_fNormalSpeed(0.0f),m_Pattern(),m_bCollisoinDetection(true),m_bActivateCollisionDetection(false),m_bCollisionWall(false),
m_DefeatAttackType(CAttack::ATTACKTYPE::EXPLOSION),m_nAttackCoolTime(0),m_bPossibleAttack(true),m_State(CEnemy::STATE::NORMAL),m_bStartLanding(false),m_nJumpCoolTime(0)
{
	m_nNumEnemy++;//�G�����J�E���g�A�b�v
}
//============================================================================================================================================

//====================================================================================
//�f�X�g���N�^
//====================================================================================
CEnemy::~CEnemy()
{
	m_nNumEnemy--;//�G�����J�E���g�_�E��
}
//============================================================================================================================================

//====================================================================================
//����������
//====================================================================================
HRESULT CEnemy::Init()
{
	CObjectX::Init();//�I�u�W�F�N�gX����������

	GetMoveInfo().SetUseGravity(true, CObjectX::GetNormalGravity());  //�d�͂��g�p����
	GetMoveInfo().SetUseUpdatePos(true);                              //�ʒu�̍X�V������
	return S_OK;
}
//============================================================================================================================================

//====================================================================================
//�I������
//====================================================================================
void CEnemy::Uninit()
{
	CObjectX::Uninit();//�I�u�W�F�N�gX�I������

	if (m_pEnemyMove != nullptr)
	{//�G�̈ړ���ԋ@�B��j������
		delete m_pEnemyMove;
		m_pEnemyMove = nullptr;
	}
}
//============================================================================================================================================

//====================================================================================
//�X�V����
//====================================================================================
void CEnemy::Update()
{
	if (CScene::GetMode() == CScene::MODE_GAME)
	{//�V�[�����u�Q�[���v��������
		const D3DXVECTOR3& Rot = GetRotInfo().GetRot();                           //����
		const D3DXVECTOR3& Pos = GetPosInfo().GetPos();                           //�ʒu
		const D3DXVECTOR3& PlayerPos = CGame::GetPlayer()->GetPosInfo().GetPos(); //�v���C���[�̈ʒu
		float fLength = CCalculation::CalculationLength(Pos, PlayerPos);          //����

		m_nCntTime++;                                                             //���Ԃ��J�E���g����
		m_nJumpCoolTime++;                                                        //�W�����v�܂ł̃N�[���^�C�����J�E���g����
		m_nAttackCoolTime++;                                                      //�N�[���^�C�����J�E���g����
		CObjectX::CollisionInfo& CollisionInfo = GetCollisionInfo();              //�����蔻������擾����
		CObjectX::CollisionInfo::State& CollisionState = CollisionInfo.GetState();//�����蔻���Ԃ��擾����

		if (CollisionState.GetLanding())
		{
			GetMoveInfo().SetMove(D3DXVECTOR3(GetMoveInfo().GetMove().x,0.0f, GetMoveInfo().GetMove().z));
		}

		m_pEnemyMove->Process(this);//�ړ������S�ʃX�e�[�g

		if (m_bPossibleAttack == true)
		{//�U�����\��������
			AttackProcess();//�U������
		}

		//�G�̈ʒu�̒���
		CollisionDetectionProcess();

		if (m_Type == ENEMYTYPE::DIVEWEAK && m_bStartLanding == true)
		{//�_�C�u�Ɏア�G�Ȃ�A�ŏ��ɏ�����n�ʂ��痎���Ȃ�
			CheckGrroundDistance_AdjustPos();
		}

	    CObjectX::Update();//�I�u�W�F�N�gX�X�V����

		if (CollisionState.GetLanding() && m_bStartLanding == false)
		{//�n�ʂɏ���Ă��邩�A�܂��n�ʂɏ���Ă��Ȃ�������
			m_bStartLanding = true;//�n�ʂɏ��߂ď�����t���O��true�ɂ���
		}

		CollisionProcess();//�����蔻�菈��

		DefeatStaging();  //�|���ꂽ�Ƃ��̉��o���s���i�h���N���X���Ƃɕς��j


		if (GetPosInfo().GetPos().y < -100.0f)
		{//��ԉ��̒n�ʂ��X�ɉ��ɂ����玀�S�t���O��ݒ肷��
			SetDeath();
		}

	}
	if (CScene::GetMode() == CScene::MODE_EDIT)
	{//�V�[�����u�G�f�B�b�g�v
		GetDrawInfo().ChengeColorProcess(this);//�G��I�񂾂Ƃ��ɐF��ς��鏈��

		for (auto it : m_VecMoveAi)
		{
			if (it->GetDrawInfo().GetUseDraw())
			{//�G�f�B�b�g���[�h�������S�Ă̈ړ�AI���f����`�悷��
				it->GetDrawInfo().SetUseDraw(false);
			}
		}
	}

}
//============================================================================================================================================

//====================================================================================
//�`�揈��
//====================================================================================
void CEnemy::Draw()
{
	CObjectX::Draw();//�I�u�W�F�N�gX�̕`�揈��

	for (auto pObj : m_VecMoveAi)
	{
		pObj->ExtraDraw();//AI���f���̕`��i�G�̕`�揈��)
	}
}
//============================================================================================================================================

//====================================================================================
//���S�t���O�ݒ菈��
//====================================================================================
void CEnemy::SetDeath()
{
	CObjectX::SetDeath();//�I�u�W�F�N�gX���S�t���O�ݒ菈��

	if (GetUseDeath() == true)
	{//�G�̎��S�t���O��true��������ړ�AI�̎��S�t���O���ݒ肷��
		for (auto pObj : m_VecMoveAi)
		{
			if (pObj != nullptr)
			{
				pObj->SetUseDeath(true);
				pObj->SetDeath();
				pObj = nullptr;
			}
		}
		m_VecMoveAi.clear();//�ړ�AI�̓��I�z����N���A����
	}
}
//============================================================================================================================================

//====================================================================================
//�e�L�X�g�t�@�C���ɏ����Z�[�u����
//====================================================================================
void CEnemy::SaveInfoTxt(fstream& WritingFile)
{
	WritingFile << "SETENEMY" << endl;//�ǂݍ��݊J�n�p�e�L�X�g

	WritingFile << "ENEMYTYPE = " << static_cast<int>(m_Type);//�G�̃^�C�v�ԍ��������o��
	switch (m_Type)
	{//�G�̃^�C�v�ԍ��ɉ����ĕ����Ń^�C�v���������o��
	case ENEMYTYPE::SHOTWEAK:
		WritingFile << " # SHOTWEAK" << endl;
		break;
	case ENEMYTYPE::DIVEWEAK:
		WritingFile << " # DIVEWEAK" << endl;
		break;
	case ENEMYTYPE::IDLE:
		WritingFile << " # IDLE" << endl;
		break;
	default:
		assert(false);//�z��O�̒l
		break;
	}

	WritingFile << "NORMALSPEED = " << m_fNormalSpeed << endl;   //�ʏ푬�x�������o��
	WritingFile << "SENSINGRANGE = " << m_fSensingRange << endl; //���G�͈͂������o��

	//�t�F�[�Y�ԍ���ݒ�
	WritingFile << "PHASENUM = " << m_nPhaseNum << endl;

	//�ړ�AI�̈ʒu��ۑ�
	WritingFile << "SETMOVEAI" << endl;

	int nCnt = 0;//�ړ�AI�̐��J�E���g�p
	for (auto it = m_VecMoveAi.begin(); it != m_VecMoveAi.end(); ++it)
	{
		WritingFile << "SETNUM = " << nCnt << endl;//�ړ�AI�̔ԍ��������o��

		WritingFile << "POS = " << fixed << setprecision(3) << (*it)->GetPosInfo().GetPos().x << " " <<
			fixed << setprecision(3) << (*it)->GetPosInfo().GetPos().y << " " <<
			fixed << setprecision(3) << (*it)->GetPosInfo().GetPos().z << " " << endl;//�ʒu
		WritingFile << "ROT = " << fixed << setprecision(3) << (*it)->GetRotInfo().GetRot().x << " " <<
			fixed << setprecision(3) << (*it)->GetRotInfo().GetRot().y << " " <<
			fixed << setprecision(3) << (*it)->GetRotInfo().GetRot().z << " " << endl;//����
		WritingFile << "SCALE = " << fixed << setprecision(3) << (*it)->GetSizeInfo().GetScale().x << " " <<
			fixed << setprecision(3) << (*it)->GetSizeInfo().GetScale().y << " " <<
			fixed << setprecision(3) << (*it)->GetSizeInfo().GetScale().z << " " << endl;//�g�嗦

		WritingFile << "END_SETNUM" << endl;

		nCnt++;//�J�E���g�A�b�v
	}
	WritingFile << "END_SETMOVEAI" << endl;//�ړ�AI�̏��̕ۑ����I������

	CObjectX::SaveInfoTxt(WritingFile);//�I�u�W�F�N�gX�̏��������o��

	WritingFile << "END_SETENEMY" << endl;//�ǂݍ��݊J�n�p�e�L�X�g
}
//============================================================================================================================================

//====================================================================================
//�e�L�X�g�t�@�C������������[�h����
//====================================================================================
//void CEnemy::LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff)
//{
//	//int nType = 0;//���
//	//int nLife = 0;//�̗�
//	//D3DXVECTOR3 Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  //�ړ���
//	//D3DXVECTOR3 Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   //�ʒu
//	//D3DXVECTOR3 Scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f); //�g�嗦
//	//D3DXVECTOR3 Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   //����
//	//ENEMYTYPE Type = {};                               //�u���b�N�̎��
//	//while (Buff != "END_SETBLOCK")
//	//{
//	//	LoadingFile >> Buff;//�P���ǂݍ���
//	//	if (Buff == "#")
//	//	{
//	//		getline(LoadingFile, Buff);
//	//	}
//	//	else if (Buff == "TYPE")
//	//	{
//	//		LoadingFile >> Buff;//�C�R�[��
//	//		LoadingFile >> nType;      //���
//	//	}
//	//	else if (Buff == "LIFE")
//	//	{
//	//		LoadingFile >> Buff;//�C�R�[��
//	//		LoadingFile >> nLife;      //�̗�
//	//	}
//	//	else if (Buff == "MOVE")
//	//	{
//	//		LoadingFile >> Buff;//�C�R�[��
//	//		LoadingFile >> Move.x;      //�ړ���X
//	//		LoadingFile >> Move.y;      //�ړ���Y
//	//		LoadingFile >> Move.z;      //�ړ���Z
//	//	}
//	//	else if (Buff == "POS")
//	//	{
//	//		LoadingFile >> Buff;//�C�R�[��
//	//		LoadingFile >> Pos.x;      //�ʒuX
//	//		LoadingFile >> Pos.y;      //�ʒuY
//	//		LoadingFile >> Pos.z;      //�ʒuZ
//	//	}
//	//	else if (Buff == "ROT")
//	//	{
//	//		LoadingFile >> Buff;//�C�R�[��
//	//		LoadingFile >> Rot.x;      //�ʒuX
//	//		LoadingFile >> Rot.y;      //�ʒuY
//	//		LoadingFile >> Rot.z;      //�ʒuZ
//	//	}
//	//	else if (Buff == "SCALE")
//	//	{
//	//		LoadingFile >> Buff;//�C�R�[��
//	//		LoadingFile >> Scale.x;      //�g�嗦X
//	//		LoadingFile >> Scale.y;      //�g�嗦Y
//	//		LoadingFile >> Scale.z;      //�g�嗦Z
//	//	}
//	//}
//	//Type = static_cast<ENEMYTYPE>(nType);
//
//	//
//
//	//listSaveManager.push_back(CEnemy::Create(Type, nLife, Pos, Rot, Scale));//vector�ɏ���ۑ�����
//
//}
////============================================================================================================================================

//====================================================================================
//�e�L�X�g�t�@�C������������[�h����
//====================================================================================
void CEnemy::LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff, CObject* pObj)
{
	int nPhaseNum = 0;                                        //�t�F�[�Y�ԍ�
	float fNormalSpeed = 0.0f;                                //�ʏ푬�x
	float fSensingRange = 0.0f;                               //���G�͈�
	vector<CAIModel*> VecMoveAi = {};                         //�ړ�AI�̓��I�z��
	vector<MoveAiInfo> VecMoveAiInfo = {};                    //�ړ�AI���̓��I�z��
	int nEnemyType = 0;
	ENEMYTYPE EnemyType = ENEMYTYPE::SHOTWEAK;                //�G�^�C�v
	int nCntMoveAi = 0;                                       //�ړ�AI�̐����J�E���g
	D3DXVECTOR3 MoveAiPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    //�ړ�AI�̈ʒu
	D3DXVECTOR3 MoveAiRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    //�ړ�AI�̌���
	D3DXVECTOR3 MoveAiScale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  //�ړ�AI�̊g�嗦
	while (Buff != "END_SETENEMY")
	{//������END_SETENEMY�ɂȂ�܂ŌJ��Ԃ�
		LoadingFile >> Buff;//�P���ǂݍ���
		if (Buff == "#")
		{//#�Ȃ炻�̍s���X�L�b�v
			getline(LoadingFile, Buff);
		}
		else if (Buff == "ENEMYTYPE")
		{//�G�^�C�v
			LoadingFile >> Buff;                             //�C�R�[��
			LoadingFile >> nEnemyType;                       //�t�F�[�Y�ԍ�
			EnemyType = static_cast<ENEMYTYPE>(nEnemyType);  //�G�^�C�v�ɃL���X�g
			SetEnemyType(EnemyType);                         //�G�^�C�v��ݒ�
		}
		else if (Buff == "PHASENUM")
		{//�t�F�[�Y�ԍ�
			LoadingFile >> Buff;                             //�C�R�[��
			LoadingFile >> nPhaseNum;                        //�t�F�[�Y�ԍ�
			SetPhaseNum(nPhaseNum);                          //�t�F�[�Y�ԍ��ݒ�
		}
		else if (Buff == "NORMALSPEED")
		{//�ʏ�ړ����x
			LoadingFile >> Buff;                             //�C�R�[��
			LoadingFile >> fNormalSpeed;                     //�ʏ푬�x
			SetNormalSpeed(fNormalSpeed);                    //�ʏ푬�x�ݒ�
		}
		else if (Buff == "SENSINGRANGE")
		{//���G�͈�
			LoadingFile >> Buff;                             //�C�R�[��
			LoadingFile >> fSensingRange;                    //���G�͈�
			SetSensingRange(fSensingRange);                  //���G�͈͂�ݒ�
		}
		else if (Buff == "SETMOVEAI")
		{//�ړ�AI
			while (1)
			{
				LoadingFile >> Buff;
				if (Buff == "SETNUM")
				{//���ڂ�
					LoadingFile >> Buff;                     //�C�R�[��
					LoadingFile >> nCntMoveAi;               //�ԍ�
					while (1)
					{
						LoadingFile >> Buff;

						if (Buff == "POS")
						{
							LoadingFile >> Buff;//�C�R�[��
							LoadingFile >> MoveAiPos.x;      //�ʒuX
							LoadingFile >> MoveAiPos.y;      //�ʒuY
							LoadingFile >> MoveAiPos.z;      //�ʒuZ
						}
						else if (Buff == "ROT")
						{
							LoadingFile >> Buff;//�C�R�[��
							LoadingFile >> MoveAiRot.x;      //�ʒuX
							LoadingFile >> MoveAiRot.y;      //�ʒuY
							LoadingFile >> MoveAiRot.z;      //�ʒuZ
						}
						else if (Buff == "SCALE")
						{
							LoadingFile >> Buff;//�C�R�[��
							LoadingFile >> MoveAiScale.x;      //�g�嗦X
							LoadingFile >> MoveAiScale.y;      //�g�嗦Y
							LoadingFile >> MoveAiScale.z;      //�g�嗦Z
						}
						else if (Buff == "END_SETNUM")
						{
							if (CScene::GetMode() == CScene::MODE_EDIT)
							{//�G�f�B�b�g���[�h�̎��͕��ʂɑS�Ă̈ړ�AI���o���������ߒ��ړ��I�z��Ɋi�[
								CAIModel* pAiModel = CAIModel::Create(CAIModel::AIMODELTYPE::MOVEPOINT, MoveAiPos, MoveAiRot, MoveAiScale, nullptr);//AI���f���̐���
								pAiModel->GetDrawInfo().SetUseDraw(false);  //�`�悵�Ȃ�
								pAiModel->GetDrawInfo().SetUseShadow(false);//�e��`�悵�Ȃ�
								VecMoveAi.push_back(pAiModel);              //�ړ�AI�̓��I�z��Ɋi�[
							}
							else if (CScene::GetMode() == CScene::MODE_GAME)
							{//�Q�[�����[�h�̂Ƃ��͂܂��Ă΂�Ă��Ȃ��G�̏ꍇ�A�ړ�AI���̂𑶍݂��������Ȃ������f�[�^�������i�[���A�Ă΂ꂽ�Ƃ���PhaseManager�ɕۑ������f�[�^���g�p���Ĉړ�AI����������
								MoveAiInfo Info = {};          //�ړ�AI���
								Info.Pos = MoveAiPos;          //�ʒu
								Info.Rot = MoveAiRot;          //����
								Info.Scale = MoveAiScale;      //�g�嗦
								VecMoveAiInfo.push_back(Info); //���I�z��Ɋi�[
							}
							break;
						}
					}
				}
				else if (Buff == "END_SETMOVEAI")
				{//�ړ�AI�̏��̏����o���I��
					break;
				}
			}
		}
		else if (Buff == "SETOBJECTX")
		{//�I�u�W�F�N�gX�̏���ǂݍ���
			CObjectX::LoadInfoTxt(LoadingFile, listSaveManager, Buff, pObj);
		}
	}

	if (CScene::GetMode() == CScene::MODE_GAME)
	{//�Q�[�����[�h��������
		//�t�F�[�Y�}�l�[�W���[�ɏ����i�[�B�����t�F�[�Y�ɓ�������t�F�[�Y�}�l�[�W���[����G�����������
		CGame::GetPhaseManager()->PushPhaseInfo(GetPosInfo().GetPos(), GetRotInfo().GetRot(), GetSizeInfo().GetScale(), GetLifeInfo().GetMaxLife(),
			static_cast<int>(EnemyType), 0, nPhaseNum, fNormalSpeed, fSensingRange, 3, VecMoveAiInfo);
	}
	else if (CScene::GetMode() == CScene::MODE_EDIT)
	{//�G�f�B�b�g���[�h��������
		SetVecMoveAiInfo(VecMoveAi);//�ړ�AI����ݒ肷��
	}
}
//============================================================================================================================================

//====================================================================================
//�e�L�X�g����������[�h����
//====================================================================================
//CObject* CEnemy::ManagerChengeObject(bool bAim)
//{
//	int nNewType = static_cast<int>(m_Type);
//    ENEMYTYPE NewType = {};
//	//=======================================
//	//��ނ�ς���
//	//=======================================
//	if (bAim == true)
//	{
//		nNewType++;
//	}
//	else
//	{
//		nNewType--;
//	}
//	if (nNewType >= static_cast<int>(ENEMYTYPE::MAX))
//	{
//		nNewType = 0;
//	}
//	if (nNewType < 0)
//	{
//		nNewType = static_cast<int>(ENEMYTYPE::MAX) - 1;
//	}
//	//======================================================================================
//
//	//=======================================
//	//�ݒ肷��
//	//=======================================
//	NewType = ENEMYTYPE(nNewType);
//	//======================================================================================
//
//	//=======================================
//	//���S�t���O��ݒ肷��
//	//=======================================
//	SetUseDeath(true);
//	SetDeath();
//	//======================================================================================
//
//	return CEnemy::Create(NewType,GetLife,GetPos(), GetRot(), GetScale());//���������I�u�W�F�N�g��Ԃ�
//}
////============================================================================================================================================

//====================================================================================
//�X�e�[�W�}�l�[�W���[�ɃZ�[�u����
//====================================================================================
CObject* CEnemy::ManagerSaveObject()
{
	return nullptr;
}
//============================================================================================================================================

//====================================================================================
//�X�e�[�W�}�l�[�W���[���瑀�삷��
//====================================================================================
void CEnemy::ManagerChooseControlInfo()
{
	//�X�e�[�W�}�l�[�W���[�ɑI������Ă��鎞�����\������
	for (auto it : m_VecMoveAi)
	{
		it->GetDrawInfo().SetUseDraw(true);
	}

	if (CObject::GetType() == CObject::TYPE::ENEMY)
	{//�I�u�W�F�N�g�̃^�C�v���G��������
		CManager::GetDebugText()->PrintDebugText("�G���Ă΂�Ă���I\n");
	}

	SetMoveAiPoint();//�ړ�AI�̐ݒ���s��

	PhaseNumDecision();//�t�F�[�Y�ԍ��̌�����s��

	EditNormalSpeed();//�ʏ�ړ����x��ҏW

	EditSensingRange();//���G�͈͂�ҏW

	CObjectX::ManagerChooseControlInfo();//�I�u�W�F�N�gX�̏��𑀍삷��
}
//============================================================================================================================================

//====================================================================================
//�ړ�AI�̏���ݒ肷��
//====================================================================================
void CEnemy::SetVecMoveAiInfo(vector<CAIModel*> vec)
{
	m_VecMoveAi = vec;//AI���f���̓��I�z���ݒ�
}
//============================================================================================================================================

//====================================================================================
//�v���C���[��ǂ������鏈��
//====================================================================================
void CEnemy::ChasePlayer()
{
	const D3DXVECTOR3& PlayerPos = CGame::GetPlayer()->GetPosInfo().GetPos();//�v���C���[�̈ʒu
	const D3DXVECTOR3& Pos = GetPosInfo().GetPos();                          //�G�̈ʒu
	float fLengthPlayer = CCalculation::CalculationLength(PlayerPos, Pos);   //�v���C���[�Ƃ̋��������߂�
	D3DXVECTOR3 Aim = PlayerPos - Pos;                                       //�x�N�g�������߂�
	D3DXVec3Normalize(&Aim, &Aim);                                           //���߂��x�N�g���𐳋K��        
	float fRot = atan2f(Aim.x, Aim.z);                                       //�x�N�g���̊p�x�����߂�

	D3DXVECTOR3 Move = CCalculation::HormingVecRotXZ(m_fRotMove, GetPosInfo().GetPos(), CGame::GetPlayer()->GetPosInfo().GetSenterPos(), 0.1f, m_fNormalSpeed);//�v���C���[�̈ʒu�Ɍ������ăz�[�~���O���Ɉړ�����
	GetRotInfo().SetRot(D3DXVECTOR3(GetRotInfo().GetRot().x, m_fRotMove, GetRotInfo().GetRot().z));//���X�Ƀv���C���[�Ɍ��������킹�Ă���
	GetMoveInfo().SetMove(D3DXVECTOR3(Move.x, GetMoveInfo().GetMove().y, Move.z));                 //��L�̏����ŋ��߂��ړ��ʂ�ݒ�
}
//============================================================================================================================================

//====================================================================================
//�G���|���ꂽ�Ƃ��̉��o
//====================================================================================
void CEnemy::DefeatStaging()
{
	CGame::GetCombo()->AddCombo(1);//�R���{���{�P
	CGame::GetScore()->AddScore(100);//�X�R�A�{�P�O�O
}
//============================================================================================================================================

//====================================================================================
//�����蔻��̏������s��
//====================================================================================
void CEnemy::CollisionProcess()
{
	CObjectX::CollisionInfo& CollisionInfo = GetCollisionInfo();               //�����蔻������擾����
	CObjectX::CollisionInfo::State& CollisionState = CollisionInfo.GetState(); //�����蔻���Ԃ��擾����
	CollisionInfo.GetSquareInfo().ResetPushOutFirstFlag();                     //���ꂼ��̎��̉����o������̗D��t���O�����Z�b�g
	CollisionState.SetLanding(false);                                          //�n�ʂɏ���Ă��邩�ǂ��������Z�b�g 
	for (int nCntPri = 0; nCntPri < CObject::m_nMAXPRIORITY; nCntPri++)
	{//�I�u�W�F�N�g���X�g����������
		CObject* pObj = CObject::GetTopObject(nCntPri);//���X�g�̐擪�I�u�W�F�N�g���擾����

		while (pObj != nullptr)
		{//�I�u�W�F�N�g��nullptr�ɂȂ�܂ŌJ��Ԃ�
			CObject* pNext = pObj->GetNextObject();//���̃I�u�W�F�N�g���擾

			CObject::TYPE type = pObj->GetType();  //�I�u�W�F�N�g�̎�ނ��擾

			if (type == CObject::TYPE::BLOCK || type == CObject::TYPE::BGMODEL)
			{//�I�u�W�F�N�g�̎�ނ��u���b�N�A���͔w�i���f����������
				CObjectX* pObjX = static_cast<CObjectX*>(pObj);//�I�u�W�F�N�gX�Ƀ_�E���L���X�g
				CCollision::ExtrusionCollisionSquarePushOutFirstDecide(this, pObjX);//�����`�̉����o������̂��ꂼ��̎��̏����̗D��x�����߂�
			}
			//�I�u�W�F�N�g�����ɐi�߂�
			pObj = pNext;
		}
	}
	//=======================================================================================

	//============================================================
	//�����o������J�n
	//============================================================
	for (int nCntPri = 0; nCntPri < CObject::m_nMAXPRIORITY; nCntPri++)
	{
		CObject* pObj = CObject::GetTopObject(nCntPri);

		while (pObj != nullptr)
		{
			//���̃I�u�W�F�N�g���i�[
			CObject* pNext = pObj->GetNextObject();

			//��ނ̎擾�i�G�Ȃ瓖���蔻��j
			CObject::TYPE type = pObj->GetType();

			if (type == CObject::TYPE::BLOCK || type == CObject::TYPE::BGMODEL)
			{//�I�u�W�F�N�g�̃^�C�v���u�u���b�N�v�܂��́u�w�i���f���v��������
				CObjectX* pObjX = static_cast<CObjectX*>(pObj);//�I�u�W�F�N�gX�Ƀ_�E���L���X�g

				CCollision::ResolveExtrusionCollisionSquare(this, pObjX);//��L�̃��X�g�����œ����蔻��̗D��x�����߂��̂ŁA�����`�̉����o�����������
			}

			pObj = pNext;
		}
	}
	//=======================================================================================

}
//============================================================================================================================================

//====================================================================================
//�ړ�AI�̔z�u������
//====================================================================================
void CEnemy::SetMoveAiPoint()
{
	float fX = 0;//X���̃x�N�g��
	float fZ = 0;//Z���̃x�N�g��
	bool bMove = false;//XZ���ʂ̈ړ������Ă��邩�ǂ���
	bool bMoveY = false;//Y�����̈ړ������Ă��邩�ǂ���
	float fAddPosY = 0.0f;//Y���W�̈ʒu�����炷�l
	auto Input = CManager::GetInputKeyboard();//�L�[���͏��ւ̃|�C���^
	float fCameraRot = CManager::GetCamera()->GetRot().y;//�J�����̌������擾
	if (Input->GetPress(DIK_LSHIFT))
	{//LSHIFT�L�[�������Ă��邩�ǂ���
		if (Input->GetPress(DIK_I))
		{//I�L�[�������Ă��邩�ǂ���
			fAddPosY = 5.0f;//Y�ʒu�����Z����
			bMoveY = true;//Y�����̈ړ������Ă���
		}
		else if (Input->GetPress(DIK_K))
		{//K�L�[�������Ă�����
			fAddPosY = -5.0f;//Y�ʒu�����Z����
			bMoveY = true;//Y�����̈ړ������Ă���
		}
	}
	else
	{//LSHIFT�L�[�������Ă��Ȃ��Ȃ�
		if (Input->GetPress(DIK_I))
		{//I�L�[�������Ă�����
			fZ = 1;//Z�x�N�g�������߂�
			bMove = true;//XZ���ʂ̈ړ������Ă���
		}
		else if (Input->GetPress(DIK_K))
		{//K�L�[�������Ă�����
			fZ = -1;//Z�x�N�g�������߂�
			bMove = true;//XZ���ʂ̈ړ������Ă���
		}
	}
	if (Input->GetPress(DIK_L))
	{//L�L�[�������Ă�����
		fX = 1;//X�x�N�g�������߂�
		bMove = true;//XZ���ʂ̈ړ������Ă���
	}
	else if (Input->GetPress(DIK_J))
	{//J�L�[�������Ă�����
		fX = -1;//X�x�N�g�������߂�
		bMove = true;//XZ���ʂ̈ړ������Ă���
	}

	GetMoveInfo().SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));//�G�̈ړ��ʂ��O�ɂ���
	CManager::GetDebugText()->PrintDebugText("�ړ�AI�̐ݒu�ʒu�̈ړ��FIKLJ\n");//�f�o�b�O�\��

	if (bMove == true)
	{//XZ����
		float fRot = atan2f(fX, fZ);//�x�N�g���̊p�x�����߂�
		m_MoveAiSavePos += D3DXVECTOR3(sinf(fCameraRot + fRot) * 5.0f, 0.0f, cosf(fCameraRot + fRot) * 5.0f);//�J�����̌�������Ƃ����e�]�˂̌���
	}

	if (bMoveY == true)
	{//Y�����̈ړ������Ă�����
		m_MoveAiSavePos += D3DXVECTOR3(0.0f, fAddPosY, 0.0f);
	}

	D3DXVECTOR3 NowPos = m_MoveAiSavePos + GetPosInfo().GetPos();//�ݒ肷��ړ�AI�̌��݂̈ʒu��G�̈ʒu����Ɍ��߂�

	//�f�o�b�O�\��
	CManager::GetDebugText()->PrintDebugText("�ړ�AI�̈ʒu�F%f %f %f\n", NowPos.x,NowPos.y,NowPos.z);
	CManager::GetDebugText()->PrintDebugText("�ړ�AI��ۑ��FO\n");

	//�p�[�e�B�N��������
	CParticle::SummonParticle(CParticle::TYPE::TYPE00_NORMAL, 1, 30, 40.0f, 40.0f, 100, 10, false, NowPos, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), true);

	if (Input->GetPress(DIK_LSHIFT))
	{//LSHIFT�L�[�������Ă�����
		if (m_VecMoveAi.size() > 0)
		{//�ړ�AI�̐���0���傫�����
			if (Input->GetTrigger(DIK_O))
			{//O�L�[�������Ă�����
				auto it = m_VecMoveAi.end() - 1;//��Ԍ��̃C�e���[�^���擾
				(*it)->SetUseDeath(true);       //���S�t���O���g�p����
				(*it)->SetDeath();              //���S�t���O��ݒ肷��
				m_VecMoveAi.pop_back();         //��Ԍ��̃I�u�W�F�N�g��j��
			}
		}
	}
	else
	{//LSHIFT�L�[�������Ă��Ȃ����
		if (Input->GetTrigger(DIK_O))
		{//O�L�[�������Ă����猻�݂�AI���f�������ʒu��AI���f����ݒ肵���I�z��Ɋi�[
			m_VecMoveAi.push_back(CAIModel::Create(CAIModel::AIMODELTYPE::MOVEPOINT, m_MoveAiSavePos + GetPosInfo().GetPos(), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(3.0f, 3.0f, 3.0f), nullptr));
		}
	}
}
//============================================================================================================================================

//====================================================================================
//AI�ړ�����
//====================================================================================
void CEnemy::AIMoveProcess()
{
	if (CScene::GetMode() == CScene::MODE_GAME)
	{//�V�[�����u�Q�[���v�Ȃ�
		//================
	    //�g�p����ϐ�
		//================
		CObjectX::RotInfo& RotInfo = GetRotInfo();//���������擾
		const D3DXVECTOR3& Rot = RotInfo.GetRot();//�������擾
		float fAddRotY = 0.0f;//Y�����̉��Z����
		float fLengthPlayer = CCalculation::CalculationLength(CGame::GetPlayer()->GetPosInfo().GetPos(), GetPosInfo().GetPos());//�v���C���[�Ƃ̋���
		//=========================================================================================================================================

		if (m_VecMoveAi.size() > 0)
		{//�ړ�AI�̐���0���傫�����
			auto it = m_VecMoveAi.begin();//�ŏ��̃|�C���^���擾

			advance(it, m_nIdxMoveAi);//�w�肵�Ă���ԍ��܂Ői�߂�

			float fLength = CCalculation::CalculationLength(GetPosInfo().GetPos(), (*it)->GetPosInfo().GetPos());//�����𑪂�

			//���ݖړI�n�ɂ��Ă���ړ�AI�ւ̊p�x�����߂�
			float fRot = atan2f((*it)->GetPosInfo().GetPos().x - GetPosInfo().GetPos().x, (*it)->GetPosInfo().GetPos().z - GetPosInfo().GetPos().z);

			//��L�ŋ��߂��p�x��G�̌����Ă�������Ƃ���
			RotInfo.SetRot(D3DXVECTOR3(Rot.x,CCalculation::CalculationCollectionRot2D(Rot.y, fRot, 0.1f, false),Rot.z));//���������X�ɖړI�n�֍��킹�Ă���

			//���݌����Ă�������ֈړ�����
			GetMoveInfo().SetMove(D3DXVECTOR3(sinf(fRot) * m_fNormalSpeed, GetMoveInfo().GetMove().y, cosf(fRot) * m_fNormalSpeed));

			if (fLength < m_fNormalSpeed + 50.0f)
			{//�������ʏ�ړ����x + 50.0f���߂��Ȃ��
				m_nIdxMoveAi++;//�ړI�n�����̈ʒu�ɕύX
			}

			int nSize = m_VecMoveAi.size();
			if (m_nIdxMoveAi >= nSize)
			{//�ړI�n���ړ�AI�̓��I�z��̃T�C�Y�𒴂�����
				m_nIdxMoveAi = 0;//0�ɖ߂�
			}
			if (m_nIdxMoveAi < 0)
			{//�ړI�n��0�ȉ��ɂȂ�����
				m_nIdxMoveAi = nSize - 1;//���I�z��̈�Ԍ����w��
			}

		}
		if (fLengthPlayer < m_fSensingRange)
		{//�v���C���[�����G�͈͂ɓ�������o�g�����[�h�ɕύX
			ChengeMove(DBG_NEW CEnemyMove_Battle());
		}
	}
}
//============================================================================================================================================


//====================================================================================
//�o�g���ړ�����
//====================================================================================
void CEnemy::BattleMoveProcess()
{
	float fLengthPlayer = CCalculation::CalculationLength(CGame::GetPlayer()->GetPosInfo().GetPos(), GetPosInfo().GetPos());//�v���C���[�Ƃ̋��������߂�

	ChasePlayer();//�v���C���[��ǂ������鏈��

	if (fLengthPlayer > m_fSensingRange + 300.0f)
	{//���������G�͈͂�+300.0f��艓���Ȃ�����AI���[�h�ɖ߂��Ĝp�j������
		ChengeMove(DBG_NEW CEnemyMove_AI());
	}
}
//============================================================================================================================================


//====================================================================================
//�ړ���Ԃ�ύX����
//====================================================================================
void CEnemy::ChengeMove(CEnemyMove* pEnemyMove)
{
	//�j������
	if (m_pEnemyMove != nullptr)
	{
		delete m_pEnemyMove;
		m_pEnemyMove = nullptr;
	}

	//�������
	m_pEnemyMove = pEnemyMove;
}
//============================================================================================================================================

//====================================================================================
//���C�������������ǂ����𔻒肷��
//====================================================================================
void CEnemy::RayCollision()
{
	if (CScene::GetMode() == CScene::MODE_GAME)
	{//�V�[�����u�Q�[���v��������
		CObjectX* pObjX = nullptr;//�I�u�W�F�N�gX�ւ̃|�C���^��������
		D3DXVECTOR3 Ray = CCalculation::RadToVec(D3DXVECTOR3(GetRotInfo().GetRot().x, GetRotInfo().GetRot().y,0.0f) - D3DXVECTOR3(D3DX_PI * 0.5f,0.0f,0.0f));//��O���Ƀ��C���΂�

		for (int nCntPri = 0; nCntPri < m_nMAXPRIORITY; nCntPri++)
		{//�I�u�W�F�N�g���X�g����������
			CObject* pObj = GetTopObject(nCntPri);//���X�g�̐擪�I�u�W�F�N�g���擾����

			while (pObj != nullptr)
			{//�I�u�W�F�N�g��nullptr�ɂȂ�܂ŌJ��Ԃ�
				CObject* pNext = pObj->GetNextObject();
				D3DXVECTOR3 CollisionPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//���C��������ʒu�i�[�p
				if (pObj->GetType() == CObject::TYPE::BGMODEL || pObj->GetType() == CObject::TYPE::BLOCK)
				{//�I�u�W�F�N�g�̃^�C�v���u�w�i���f���v���́u�u���b�N�v�Ȃ�
					CObjectX * pComObjX = static_cast<CObjectX*>(pObj);
					if (CCollision::RayIntersectsAABBCollisionPos(GetPosInfo().GetSenterPos(), Ray, pComObjX->GetPosInfo().GetPos() + pComObjX->GetSizeInfo().GetVtxMin(), pComObjX->GetPosInfo().GetPos() + pComObjX->GetSizeInfo().GetVtxMax(),
						CollisionPos))
					{//���C��AABB�ɓ������Ă�����
						float fLength = sqrtf(powf(CollisionPos.x - GetPosInfo().GetSenterPos().x, 2) +
							powf(CollisionPos.y - GetPosInfo().GetSenterPos().y, 2) + powf(CollisionPos.z - GetPosInfo().GetSenterPos().z,2));//���C�����������ʒu�܂ł̋������v�Z����
						if (fLength < GetSizeInfo().GetSize().x + 50.0f)
						{//�G��X�����̃T�C�Y+50.0f�������������
							//�G�̃I�u�W�F�N�g�̃T�C�Y�̏������苗�����������Ȃ���
							pObjX = pComObjX;//�߂��I�u�W�F�N�g���i�[
						}
					}
				}

				pObj = pNext;//���X�g�����ɐi�߂�
			}
		}

		if (pObjX != nullptr)
		{//���������I�u�W�F�N�g���������ꍇ�A�v���C���[�Ƃ̃x�N�g���̓��ς��Ƃ�A�I�u�W�F�N�g���悯��܂ŉE���ɓ������荶���ɓ������肷��
			D3DXVECTOR3 ObjectAim = pObjX->GetPosInfo().GetPos() - GetPosInfo().GetPos();//���������I�u�W�F�N�g�ƓG�̃x�N�g�������߂�
			D3DXVec3Normalize(&ObjectAim, &ObjectAim);//��L�ŋ��߂��x�N�g���𐳋K������
			const D3DXVECTOR3& PlayerPos = CGame::GetPlayer()->GetPosInfo().GetPos();//�v���C���[�̈ʒu
			const D3DXVECTOR3& Pos = GetPosInfo().GetPos();//�G�̈ʒu

			float fDot = CCalculation::DetermineSide3D(Pos, ObjectAim, D3DXVECTOR3(0.0f, 1.0f, 0.0f), PlayerPos);//���������I�u�W�F�N�g�ɑ΂��ăv���C���[���ǂ��瑤�ɂ��邩���v�Z����
			D3DXVECTOR3 Aim = PlayerPos - Pos;//�v���C���[�ƓG�̃x�N�g�������
			D3DXVec3Normalize(&Aim, &Aim);//�x�N�g���𐳋K������
			float fRot = atan2f(Aim.x, Aim.z);//XZ���ʂ̃x�N�g���̊p�x�����߂�

			if (fDot > 0.0f)
			{//�x�N�g���ɑ΂��ăv���C���[���E���ɂ�����
				ChengeMove(DBG_NEW CEnemyMove_DodgeWall(this, D3DXVECTOR3(sinf(fRot - D3DX_PI * 0.5f) * m_fNormalSpeed, GetMoveInfo().GetMove().y, cosf(fRot - D3DX_PI * 0.5f) * m_fNormalSpeed)));
			}
			else if (fDot < 0.0f)
			{//�x�N�g���ɑ΂��ăv���C���[�������ɂ�����
				ChengeMove(DBG_NEW CEnemyMove_DodgeWall(this, D3DXVECTOR3(sinf(fRot + D3DX_PI * 0.5f) * m_fNormalSpeed, GetMoveInfo().GetMove().y, cosf(fRot + D3DX_PI * 0.5f) * m_fNormalSpeed)));
			}
		}
	}
}
//============================================================================================================================================

//====================================================================================
//�U���p�^�[�����I������
//====================================================================================
void CEnemy::EndAttackPattern()
{
	m_Pattern.nPattern = 0;        //�p�^�[���ԍ������Z�b�g
	m_Pattern.nPatternTime = 0;    //�p�^�[�����Ԃ����Z�b�g
	m_Pattern.bAction = false;     //�s�����Ă��邩�ǂ��������Z�b�g 
	m_Pattern.nSubPattern = 0;     //�T�u�p�^�[���ԍ������Z�b�g
	m_nAttackCoolTime = 0;         //�N�[���^�C�������Z�b�g
	GetMoveInfo().SetUseGravity(true, GetNormalGravity());  //�d�͂��g�p����
	GetMoveInfo().SetUseInteria(false, GetNormalInertia()); //�������g�p���Ȃ�
}
//============================================================================================================================================


//====================================================================================
//�t�F�[�Y�ԍ������肷��
//====================================================================================
void CEnemy::PhaseNumDecision()
{
	if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == false)
	{
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_4))
		{//�o���t�F�[�Y�ԍ����C���N�������g
			m_nPhaseNum++;
		}
	}
	else
	{
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_4))
		{//�o���t�F�[�Y�ԍ����f�N�������g
			m_nPhaseNum--;
		}
	}

	//�f�o�b�O�\��
	CManager::GetDebugText()->PrintDebugText("�t�F�[�Y�ԍ��̕ύX�F%d\n", m_nPhaseNum);
}
//============================================================================================================================================


//====================================================================================
//�U������
//====================================================================================
void CEnemy::AttackProcess()
{

}
//============================================================================================================================================

//====================================================================================
//�Փ˔��菈��
//====================================================================================
void CEnemy::CollisionDetectionProcess()
{
	D3DXVECTOR3 MyPos = GetPosInfo().GetPos();        //�����̈ʒu
	D3DXVECTOR3 MyVtxMax = GetSizeInfo().GetVtxMax(); //�����̍ő咸�_

	//�������g�̈ʒu��X�̍ő咸�_�Ƃ̋��������
	float fMyCornarDistance = CCalculation::CalculationLength(MyPos,MyPos + D3DXVECTOR3(MyVtxMax.x, 0.0f, 0.0f));

	for (int nCntPri = 0; nCntPri < m_nMAXPRIORITY; nCntPri++)
	{//�I�u�W�F�N�g���X�g����������
		CObject* pObj = GetTopObject(nCntPri);//���X�g�̐擪�I�u�W�F�N�g���擾����
		while (pObj != nullptr)
		{//�I�u�W�F�N�g��nullptr�ɂȂ�܂ŌJ��Ԃ�
			CObject* pNext = pObj->GetNextObject();
			if (pObj->GetType() == CObject::TYPE::ENEMY || pObj->GetType() == CObject::TYPE::PLAYER)
			{//�I�u�W�F�N�g�̃^�C�v���G���̓v���C���[��������
				CObjectX* pObjX = static_cast<CObjectX*>(pObj);//�I�u�W�F�N�gX�ɃL���X�g
				if (pObjX != this)
				{//�I�u�W�F�N�gX�������ȊO�Ȃ�
					D3DXVECTOR3 AimVec = D3DXVECTOR3(pObjX->GetPosInfo().GetPos().x, 0.0f, pObjX->GetPosInfo().GetPos().z) -
						D3DXVECTOR3(MyPos.x, 0.0f, MyPos.z);//�G�Ƒ����XZ�����̃x�N�g�������
					D3DXVec3Normalize(&AimVec, &AimVec);//XZ�����̃x�N�g���𐳋K��
					const D3DXVECTOR3& ComPos = pObjX->GetPosInfo().GetPos();                                            //����̈ʒu
					const D3DXVECTOR3& ComVtxMaxX = ComPos + D3DXVECTOR3(pObjX->GetSizeInfo().GetVtxMax().x, 0.0f, 0.0f);//�����X�̍ő咸�_�̈ʒu
					float fLength = CCalculation::CalculationLength(MyPos, pObjX->GetPosInfo().GetPos());//�G���̓v���C���[�Ƃ̋������Ƃ�

					float fComCornarDistance = CCalculation::CalculationLength(ComPos, ComVtxMaxX);//����̈ʒu�Ƒ����X�̍ő咸�_�Ƃ̋��������

					float fTotalLength = (fComCornarDistance + fMyCornarDistance);//�����Ƒ���̒��S�_����̋����𑫂�
					if (fLength < fTotalLength &&
						GetPosInfo().GetPos().y + GetSizeInfo().GetVtxMax().y >= pObjX->GetPosInfo().GetPos().y + pObjX->GetSizeInfo().GetVtxMin().y &&
						GetPosInfo().GetPos().y + GetSizeInfo().GetVtxMin().y <= pObjX->GetPosInfo().GetPos().y + pObjX->GetSizeInfo().GetVtxMax().y)
					{//���݂̑���Ƃ̋����������Ƒ���̒��S�_����̋�����菬�������
						//�ǂꂾ���߂荞��ł邩�ɂ���ĕK�v��XZ�ړ��ʂ𑫂��i�x�N�g���̋t���Ɉړ��ʂ𑫂�)
						GetMoveInfo().SetMove(D3DXVECTOR3(GetMoveInfo().GetMove().x -AimVec.x * (fTotalLength - fLength) * 1.0f,GetMoveInfo().GetMove().y, GetMoveInfo().GetMove().z - AimVec.z * (fTotalLength - fLength) * 1.0f));//�U�����̓��������D��I�ɂ��̈ړ��ʂ����蓖�Ă�
					}

					
				}
			}

			pObj = pNext;//���X�g�����ɐi�߂�
		}
	}
}
//============================================================================================================================================

//====================================================================================
//�Փ˔��菈��
//====================================================================================
void CEnemy::CheckGrroundDistance_AdjustPos()
{
	CObject* pObjBgModel = GetTopObject(static_cast<int>(CObject::TYPE::BGMODEL));//�w�i���f���̃��X�g�̐擪���擾
	CObject* pObjBlock = GetTopObject(static_cast<int>(CObject::TYPE::BLOCK));    //�u���b�N�̃��X�g�̐擪���擾
	CObjectX::PosInfo& PosInfo = GetPosInfo();                                    //�ʒu���
	CObjectX::SizeInfo& SizeInfo = GetSizeInfo();                                 //�T�C�Y���
	CObjectX::MoveInfo& MoveInfo = GetMoveInfo();                                 //�ړ����
	const D3DXVECTOR3& Pos = PosInfo.GetPos();                                    //�ʒu
	const D3DXVECTOR3& PosOld = PosInfo.GetPosOld();                              //1f�O�̈ʒu
	const D3DXVECTOR3& VtxMin = SizeInfo.GetVtxMin();                             //�ŏ����_
	const D3DXVECTOR3& VtxMax = SizeInfo.GetVtxMax();                             //�ő咸�_
	const D3DXVECTOR3& Move = MoveInfo.GetMove();                                 //�ړ���
	D3DXVECTOR3 Dir = D3DXVECTOR3(0.0f, -1.0f, 0.0f);                             //�������x�N�g�����`
	D3DXVECTOR3 OriginPos = Pos + D3DXVECTOR3(0.0f, VtxMin.y, 0.0f);              //���肷��ʒu�i�I�u�W�F�N�g�̒�ʁj
	D3DXVECTOR3 RayCollisionPos = { 0.0f,0.0f,0.0f };                             //���C�̔���ꏊ�v�Z�p
	bool bCollision = false;                                                      //�Փ˂������ǂ���
	float fTotalBestNearLength = 0.0f;                                            //���C�����������I�u�W�F�N�g�̋����̒��ň�ԋ߂����������߂�
	int nCntRayCollosionObj = 0;                                                  //���C�����������I�u�W�F�N�g�̐����J�E���g����
	//�w�i���f�����X�g
	while (pObjBgModel != nullptr)
	{
		CObject* pNextBgMoodel = pObjBgModel->GetNextObject();
		CBgModel* pBgModel = static_cast<CBgModel*>(pObjBgModel);                 //�w�i���f���ɃL���X�g
		CObjectX::PosInfo BgModelPosInfo = pBgModel->GetPosInfo();                //�w�i���f���̈ʒu���
		CObjectX::SizeInfo BgModelSizeInfo = pBgModel->GetSizeInfo();             //�w�i���f���̃T�C�Y���
		const D3DXVECTOR3& BgModelPos = BgModelPosInfo.GetPos();                  //�w�i���f���̈ʒu
		const D3DXVECTOR3& BgModelPosOld = BgModelPosInfo.GetPosOld();            //1f�O�̈ʒu
		const D3DXVECTOR3& BgModelVtxMax = BgModelSizeInfo.GetVtxMax();           //�w�i���f���̍ŏ����_
		const D3DXVECTOR3& BgModelVtxMin = BgModelSizeInfo.GetVtxMin();           //�w�i���f���̍ő咸
		if (CCollision::RayIntersectsAABBCollisionPos(OriginPos, Dir, BgModelPos + BgModelVtxMin, BgModelPos + BgModelVtxMax, RayCollisionPos))
		{//���C���w�i���f���ɓ������Ă�����
			float fLength = sqrtf(powf(RayCollisionPos.x - OriginPos.x, 2) +
				powf(RayCollisionPos.y - OriginPos.y, 2) +
				powf(RayCollisionPos.z - OriginPos.z, 2));//���C�����������ꏊ�Ƃ̋���

			if (nCntRayCollosionObj == 0)
			{//�ŏ��Ƀ��C�����������I�u�W�F�N�g���J�E���g���A��ԋ߂������Ƃ���
				nCntRayCollosionObj++;
				fTotalBestNearLength = fLength;
			}
			else
			{//2�߈ȍ~�ɓ��������I�u�W�F�N�g�̏ꍇ�A��ԋ߂������Ɣ�r���čX�V����
				if (fLength < fTotalBestNearLength)
				{
					fTotalBestNearLength = fLength;
				}
			}
		}
		pObjBgModel = pNextBgMoodel;//���X�g�����ɐi�߂�
	}

	//�u���b�N���X�g
	while (pObjBlock != nullptr)
	{
		CObject* pNextBlock = pObjBlock->GetNextObject();             //���̃I�u�W�F�N�g���擾
		CBlock* pBlock = static_cast<CBlock*>(pObjBlock);             //�u���b�N�ɃL���X�g
		CObjectX::PosInfo BlockPosInfo = pBlock->GetPosInfo();        //�u���b�N�̈ʒu���
		CObjectX::SizeInfo BlockSizeInfo = pBlock->GetSizeInfo();     //�u���b�N�̃T�C�Y���
		const D3DXVECTOR3& BlockPos = BlockPosInfo.GetPos();          //�u���b�N�̈ʒu
		const D3DXVECTOR3& BlockPosOld = BlockPosInfo.GetPosOld();    //1f�O�̈ʒu
		const D3DXVECTOR3& BlockVtxMax = BlockSizeInfo.GetVtxMax();   //�u���b�N�̍ŏ����_
		const D3DXVECTOR3& BlockVtxMin = BlockSizeInfo.GetVtxMin();   //�u���b�N�̍ő咸
		if (CCollision::RayIntersectsAABBCollisionPos(OriginPos, Dir, BlockPos + BlockVtxMin, BlockPos + BlockVtxMax, RayCollisionPos))
		{//�u���b�N�Ƀ��C���������Ă�����
			float fLength = sqrtf(powf(RayCollisionPos.x - OriginPos.x, 2) +
				powf(RayCollisionPos.y - OriginPos.y, 2) +
				powf(RayCollisionPos.z - OriginPos.z, 2));//���C�����������ꏊ�Ƃ̋���

			if (nCntRayCollosionObj == 0)
			{//�ŏ��Ƀ��C�����������I�u�W�F�N�g���J�E���g���A��ԋ߂������Ƃ���
				nCntRayCollosionObj++;
				fTotalBestNearLength = fLength;
			}
			else
			{//2�߈ȍ~�ɓ��������I�u�W�F�N�g�̏ꍇ�A��ԋ߂������Ɣ�r���čX�V����
				if (fLength < fTotalBestNearLength)
				{
					fTotalBestNearLength = fLength;
				}
			}
		}

		pObjBlock = pNextBlock;
	}


	if (fTotalBestNearLength > 50.0f)
	{//��ԋ߂�������50.0f���傫���ꍇ�A���ɕ����Ă���Ƃ݂Ȃ��A�I�u�W�F�N�g�̏�Ɉʒu��߂��������s��
		PosInfo.SetPos(PosOld);        //�ʒu��ݒ�
		PosInfo.SetPosOld(Pos - Move); //1f�O�̈ʒu��ݒ�
		CManager::GetDebugText()->PrintDebugText("������̂Ŗ߂�\n");//�f�o�b�O�\��
	}

}

//====================================================================================
//�ʏ�ړ����x��ҏW
//====================================================================================
void CEnemy::EditNormalSpeed()
{
	CInputKeyboard * pInput = CManager::GetInputKeyboard();

	if (pInput->GetPress(DIK_LCONTROL) == true)
	{//L�R���g���[���L�[�������Ȃ���
		if (pInput->GetPress(DIK_LSHIFT) == true)
		{//�V�t�g�L�[�������Ȃ���E�E�E
			if (pInput->GetPress(DIK_5) == true)
			{
				m_fNormalSpeed -= 0.1f;
			}
		}
		else if (pInput->GetPress(DIK_5) == true)
		{
			m_fNormalSpeed += 0.1f;
		}
	}
	else
	{//L�R���g���[���L�[�������Ă��Ȃ�
		if (pInput->GetPress(DIK_LSHIFT) == true)
		{//�V�t�g�L�[�������Ȃ���E�E�E
			if (pInput->GetTrigger(DIK_5) == true)
			{
				m_fNormalSpeed -= 0.1f;
			}
		}
		else if (pInput->GetTrigger(DIK_5) == true)
		{
			m_fNormalSpeed += 0.1f;
		}
	}

	//�f�o�b�O�\��
	CManager::GetDebugText()->PrintDebugText("�ʏ�ړ����x�ύX�i�T�j�F%f\n",m_fNormalSpeed);
}
//============================================================================================================================================

//====================================================================================
//�ʏ�ړ����x��ҏW
//====================================================================================
void CEnemy::EditSensingRange()
{
	CInputKeyboard* pInput = CManager::GetInputKeyboard();//�L�[���͏��
	CObjectX::PosInfo& PosInfo = GetPosInfo();            //�ʒu���
	const D3DXVECTOR3& Pos = PosInfo.GetPos();            //�ʒu
	if (pInput->GetPress(DIK_LCONTROL) == true)
	{//L�R���g���[���L�[�������Ȃ���
		if (pInput->GetPress(DIK_LSHIFT) == true)
		{//�V�t�g�L�[�������Ȃ���E�E�E
			if (pInput->GetPress(DIK_6) == true)
			{//���G�͈͂���������
				m_fSensingRange -= 10.0f;
			}
		}
		else if (pInput->GetPress(DIK_6) == true)
		{//���G�͈͂��L������
			m_fSensingRange += 10.0f;
		}
	}
	else
	{//L�R���g���[���L�[�������Ă��Ȃ�
		if (pInput->GetPress(DIK_LSHIFT) == true)
		{//�V�t�g�L�[�������Ȃ���E�E�E
			if (pInput->GetTrigger(DIK_6) == true)
			{//���G�͈͂���������
				m_fSensingRange -= 10.0f;
			}
		}
		else if (pInput->GetTrigger(DIK_6) == true)
		{//���G�͈͂��L������
			m_fSensingRange += 10.0f;
		}
	}
	float fRadXZ = static_cast<float>(rand() % 628 - 314) * 0.01f;//XZ�����̊p�x(Yaw)�����߂�
	float fRadY = static_cast<float>(rand() % 628 - 314) * 0.01f; //Y�����̊p�x(Pitch)�����߂�
	D3DXVECTOR3 RandPos = { 0.0f,0.0f,0.0f };

	RandPos.x = m_fSensingRange * sinf(fRadXZ) * cosf(fRadY);//X�����̒����ɑ΂��āA���̑��ʂɏo�����߂ɁAY�����̍����̔�i�������ʂ̔�Ƃ��Ă�����j
	RandPos.z = m_fSensingRange * cosf(fRadXZ) * cosf(fRadY);//Z�����̒����ɑ΂��āA���̑��ʂɏo�����߂ɁAY�����̍����̔�i�������ʂ̔�Ƃ��Ă�����j
	RandPos.y = m_fSensingRange * sinf(fRadY);               //Y�����̍��������߂�

	//���G�͈͂�\�����߂Ƀp�[�e�B�N�������G�����̈ʒu�Ƀ����_���ŏo��
	CParticle::Create(CParticle::TYPE00_NORMAL, 200, 30.0f, 30.0f,Pos + RandPos,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), true);

	//�f�o�b�O�\��
	CManager::GetDebugText()->PrintDebugText("���G�͈͕ύX�i�U�j�F%f\n", m_fSensingRange);
}
//============================================================================================================================================

//********************************************************************************************************************************************
//�ˌ��Ɏア�G�N���X
//********************************************************************************************************************************************

//====================================================================================
//�ÓI�����o�ϐ��錾
//====================================================================================
const string CShotWeakEnemy::s_aSHOTWEAKENEMY_FILENAME[static_cast<int>(SHOTWEAKENEMYTYPE::MAX)] =
{
	"data\\MODEL\\Enemy\\ShotWeak\\noobSlime.x"
};
const float CShotWeakEnemy::s_fATTACKSTART_LENGTH = 450.0f;      //�U���J�n����
const float CShotWeakEnemy::s_fNORMAL_SENSIINGRANGE = 1050.0f;   //�ʏ튴�m�˒�
const int CShotWeakEnemy::s_nATTACK_COOLTIME = 60;               //�U���̃N�[���^�C��
//============================================================================================================================================

//====================================================================================
//�R���X�g���N�^
//====================================================================================
CShotWeakEnemy::CShotWeakEnemy(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CEnemy(nPri,bUseintPri,type,ObjType),
m_ShotWeakEnemyType(SHOTWEAKENEMYTYPE::NORMAL),m_pMagicSword(nullptr), m_SaveAimPos(D3DXVECTOR3(0.0f,0.0f,0.0f))
{

}
//============================================================================================================================================

//====================================================================================
//�f�X�g���N�^
//====================================================================================
CShotWeakEnemy::~CShotWeakEnemy()
{

}
//============================================================================================================================================

//====================================================================================
//����������
//====================================================================================
HRESULT CShotWeakEnemy::Init()
{
	CEnemy::Init();//�G�̏���������

	float fRatioRot = static_cast<float>(rand() % 200 - 100) / 100;   //���̍ŏ��̌����������_���Ō��߂�
	bool bAim = rand() % 2;                                           //�������v���X���}�C�i�X�����߂�
	m_pMagicSword = CAttackEnemy::Create(CAttack::ATTACKTYPE::MAGICSWORD, CAttack::TARGETTYPE::PLAYER, CAttack::COLLISIONTYPE::RECTANGLE_XZ,
		false,false,1, 60, 200, GetPosInfo().GetPos(), D3DXVECTOR3(0.0f,D3DX_PI * fRatioRot, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));//���𐶐�
	m_pMagicSword->SetUseDeath(false);//���̎��S�t���O���I�t�ɂ���

	//�����񂷕��������߂�
	if (bAim == false)
	{
		m_pMagicSword->GetRotInfo().SetUseAddRot(true, D3DXVECTOR3(0.0f,0.1f, 0.0f));
	}
	else
	{
		m_pMagicSword->GetRotInfo().SetUseAddRot(true, D3DXVECTOR3(0.0f,-0.1f, 0.0f));

	}

	SetEnemyType(CEnemy::ENEMYTYPE::SHOTWEAK);//�G�^�C�v��ݒ�
	return S_OK;
}
//============================================================================================================================================

//====================================================================================
//�I������
//====================================================================================
void CShotWeakEnemy::Uninit()
{
	CEnemy::Uninit();//�G�̏I������
}
//============================================================================================================================================

//====================================================================================
//�X�V����
//====================================================================================
void CShotWeakEnemy::Update()
{
	CEnemy::Update();//�G�̍X�V����

	if (m_pMagicSword != nullptr)
	{//�������݂��Ă�����
		m_pMagicSword->GetPosInfo().SetPos(GetPosInfo().GetPos());//���̒��S����ɓG�̈ʒu�ɌŒ�
	}

	SwordCollision();//���̓����蔻����s��
}
//============================================================================================================================================

//====================================================================================
//�`�揈��
//====================================================================================
void CShotWeakEnemy::Draw()
{
	CEnemy::Draw();//�G�̕`�揈��
}
//============================================================================================================================================

//====================================================================================
//���S�t���O�ݒ菈��
//====================================================================================
void CShotWeakEnemy::SetDeath()
{
	CEnemy::SetDeath();//�G�̎��S�t���O��ݒ�
	if (GetUseDeath() == true)
	{//�G���S�t���O���g�p����Ȃ�
		if (m_pMagicSword != nullptr)
		{//�������݂��Ă�����
			m_pMagicSword->SetUseDeath(true);//���̎��S�t���O���g�p����
			m_pMagicSword->SetDeath();       //���S�t���O��ݒ肷��
		}
	}
}
//============================================================================================================================================

//====================================================================================
//��������
//====================================================================================
CShotWeakEnemy* CShotWeakEnemy::Create(SHOTWEAKENEMYTYPE Type, int nLife, int nPhaseNum, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale)
{
	CShotWeakEnemy* pShotWeakEnemy = DBG_NEW CShotWeakEnemy;                             //�ˌ��Ɏア�G�̐���

	pShotWeakEnemy->Init();                                                              //��������
	int nType = static_cast<int>(Type);//�^�C�v�ԍ����i�[
	if (nType < 0 || nType >= static_cast<int>(SHOTWEAKENEMYTYPE::MAX))
	{
		assert("�z��O�A�N�Z�X�I(CShotWeakEnemy)");
	}
	else
	{//��O����
		int nIdx = CManager::GetObjectXInfo()->Regist(s_aSHOTWEAKENEMY_FILENAME[nType]); //���f������o�^���ԍ����擾
		pShotWeakEnemy->BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(nIdx),           //���f������ݒ肷��
			CManager::GetObjectXInfo()->GetBuffMat(nIdx),
			CManager::GetObjectXInfo()->GetdwNumMat(nIdx),
			CManager::GetObjectXInfo()->GetTexture(nIdx),
			CManager::GetObjectXInfo()->GetColorValue(nIdx));
	}
	pShotWeakEnemy->SetPhaseNum(nPhaseNum);                                              //�t�F�[�Y�ԍ���ݒ肷��
	pShotWeakEnemy->m_ShotWeakEnemyType = Type;                                          //�ˌ��Ɏア�G�̎��
	pShotWeakEnemy->GetLifeInfo().SetLife(nLife);                                        //�̗�
	pShotWeakEnemy->GetLifeInfo().SetMaxLife(nLife);                                     //�ő�̗�
	pShotWeakEnemy->GetLifeInfo().SetAutoSubLife(false);                                 //�����I�ɑ̗͂����炷���ǂ���
	pShotWeakEnemy->GetPosInfo().SetPos(pos);                                            //�ʒu
	pShotWeakEnemy->GetPosInfo().SetSupportPos(pos);                                     //�x�_�ʒu
	pShotWeakEnemy->GetRotInfo().SetRot(rot);                                            //����
	pShotWeakEnemy->GetSizeInfo().SetScale(Scale);                                       //�g�嗦
	pShotWeakEnemy->GetSizeInfo().SetFormarScale(Scale);                                 //���̊g�嗦��ݒ�
	pShotWeakEnemy->SetSensingRange(1250.0f);                                            //���m�˒�
	pShotWeakEnemy->SetNormalSpeed(10.0f);                                               //�ʏ�ړ����x
	pShotWeakEnemy->GetMoveInfo().SetUseInteria(false, GetNormalInertia());              //�������g�p���Ȃ�
	if (CScene::GetMode() == CScene::MODE_GAME)
	{//�Q�[�����[�h�̎����������o��
		pShotWeakEnemy->m_pMagicSword->GetSizeInfo().SetScale(Scale * 0.5f);
	}
	pShotWeakEnemy->GetLifeInfo().SetAutoDeath(true);                                    //�̗͂��O�ɂȂ����玀�S�t���O��ݒ肷��

	pShotWeakEnemy->SetSize();                                                           //���f���T�C�Y��ݒ�
	pShotWeakEnemy->SetManagerObjectType(CObject::MANAGEROBJECTTYPE::SHOTWEAKENEMY);     //�}�l�[�W���[�ŌĂяo�����̎�ނ�ݒ�
	return pShotWeakEnemy;
}
//============================================================================================================================================

//====================================================================================
//�e�L�X�g�t�@�C���ɏ���ۑ�����
//====================================================================================
void CShotWeakEnemy::SaveInfoTxt(fstream& WritingFile)
{
	WritingFile << "SETSHOTWEAKENEMY" << endl;//�ˌ��Ɏア�G�̏��������o��
	WritingFile << "SHOTWEAKENEMYTYPE = " << static_cast<int>(m_ShotWeakEnemyType);//�ˌ��Ɏア�G�̃^�C�v�ԍ���ݒ�
	switch (m_ShotWeakEnemyType)
	{//�ˌ��Ɏア�G�̃^�C�v���������o��
	case SHOTWEAKENEMYTYPE::NORMAL://����
		WritingFile << " # NORMAL" << endl;
		break;
	default:
		assert(false);
		break;
	}

	CEnemy::SaveInfoTxt(WritingFile);//�G�̏��������o��

	WritingFile << "END_SETSHOTWEAKENEMY" << endl;//�ˌ��Ɏア�G�̏��̐ݒ���I��
}
//============================================================================================================================================

//====================================================================================
//�e�L�X�g�t�@�C���������ǂݍ���
//====================================================================================
//void CShotWeakEnemy::LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff)
//{
//	int nShotWeakEnemyType = 0;                               //�V���b�g�Ɏア�G�^�C�v   
//	int nLife = 0;                                            //�̗�
//	int nPhaseNum = 0;                                        //�t�F�[�Y�ԍ�
//	int nDivisionNum = 0;                                     //�����
//	D3DXVECTOR3 Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);         //�ړ���
//	D3DXVECTOR3 Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);          //�ʒu
//	D3DXVECTOR3 Scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);        //�g�嗦
//	D3DXVECTOR3 Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);          //����
//	SHOTWEAKENEMYTYPE ShotWeakEnemyType = {};                 //�w�i���f���̎��
//	ENEMYTYPE EnemyType = {};                                 //�G�̃^�C�v
//													          
//	vector<CAIModel*> VecMoveAi = {};                         //�ړ�AI�̓��I�z��
//	vector<MoveAiInfo> VecMoveAiInfo = {};                    //�ړ�AI���̓��I�z��
//													          
//	int nCntMoveAi = 0;                                       //�ړ�AI�̐����J�E���g
//	D3DXVECTOR3 MoveAiPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    //�ړ�AI�̈ʒu
//	D3DXVECTOR3 MoveAiRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    //�ړ�AI�̌���
//	D3DXVECTOR3 MoveAiScale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  //�ړ�AI�̊g�嗦
//
//	float fNormalSpeed = 0.0f;                                //�ʏ푬�x
//	float fSensingRange = 0.0f;                               //���G����
//
//	while (Buff != "END_SETSHOTWEAKENEMY")
//	{//������END_SETSHOTWEAKENEMY�ɂȂ�܂ŌJ��Ԃ�
//		LoadingFile >> Buff;//�P���ǂݍ���
//		if (Buff == "#")
//		{//#�Ȃ炻�̍s���X�L�b�v
//			getline(LoadingFile, Buff);
//		}
//		else if (Buff == "SHOTWEAKENEMYTYPE")
//		{//�ˌ��Ɏア�G�̎��
//			LoadingFile >> Buff;//�C�R�[��
//			LoadingFile >> nShotWeakEnemyType;
//		}
//		else if (Buff == "ENEMYTYPE")
//		{//�G�̃^�C�v
//			LoadingFile >> Buff;//�C�R�[��
//			LoadingFile >> nType;
//		}
//		else if (Buff == "LIFE")
//		{//�̗�
//			LoadingFile >> Buff;//�C�R�[��
//			LoadingFile >> nLife;
//		}
//		else if (Buff == "POS")
//		{//�ʒu
//			LoadingFile >> Buff;//�C�R�[��
//			LoadingFile >> Pos.x;      //�ʒuX
//			LoadingFile >> Pos.y;      //�ʒuY
//			LoadingFile >> Pos.z;      //�ʒuZ
//		}
//		else if (Buff == "ROT")
//		{//����
//			LoadingFile >> Buff;//�C�R�[��
//			LoadingFile >> Rot.x;      //�ʒuX
//			LoadingFile >> Rot.y;      //�ʒuY
//			LoadingFile >> Rot.z;      //�ʒuZ
//		}
//		else if (Buff == "SCALE")
//		{//�g�嗦
//			LoadingFile >> Buff;//�C�R�[��
//			LoadingFile >> Scale.x;      //�g�嗦X
//			LoadingFile >> Scale.y;      //�g�嗦Y
//			LoadingFile >> Scale.z;      //�g�嗦Z
//		}
//		else if (Buff == "PHASENUM")
//		{//�t�F�[�Y�ԍ�
//			LoadingFile >> Buff;//�C�R�[��
//			LoadingFile >> nPhaseNum;//�t�F�[�Y�ԍ�
//		}
//		else if (Buff == "NORMALSPEED")
//		{//�ʏ�ړ����x
//			LoadingFile >> Buff;//�C�R�[��
//			LoadingFile >> fNormalSpeed;//�ʏ푬�x
//		}
//		else if (Buff == "SENSINGRANGE")
//		{//���G�͈�
//			LoadingFile >> Buff;//�C�R�[��
//			LoadingFile >> fSensingRange;
//		}
//	}
//
//	ShotWeakEnemyType = static_cast<SHOTWEAKENEMYTYPE>(nShotWeakEnemyType);//�ˌ��Ɏア�G�̃^�C�v���i�[
//	EnemyType = static_cast<ENEMYTYPE>(nType);                             //�G�̃^�C�v���i�[
//	if (CScene::GetMode() == CScene::MODE_EDIT)
//	{//�G�f�B�b�g���[�h��������
//		CShotWeakEnemy* pShotWeakEnemy = CShotWeakEnemy::Create(ShotWeakEnemyType,nLife,nPhaseNum,Pos,Rot,Scale);//�ˌ��Ɏア�G�𐶐�
//		pShotWeakEnemy->SetVecMoveAiInfo(VecMoveAi);    //�ړ�AI�̓��I�z���ݒ�
//		pShotWeakEnemy->SetNormalSpeed(fNormalSpeed);   //�ʏ�ړ����x��ݒ�
//		pShotWeakEnemy->SetSensingRange(fSensingRange); //���G�͈͂�ݒ�
//		listSaveManager.push_back(pShotWeakEnemy);      //vector�ɏ���ۑ�����
//
//	}
//	else if (CScene::GetMode() == CScene::MODE_GAME)
//	{//�Q�[�����[�h��������
//
//		//�t�F�[�Y�}�l�[�W���[�ɏ����i�[�B�����t�F�[�Y�ɓ�������t�F�[�Y�}�l�[�W���[����G�����������
//		CGame::GetPhaseManager()->PushPhaseInfo(Pos, Rot, Scale, nLife, static_cast<int>(EnemyType), nShotWeakEnemyType, nPhaseNum,fNormalSpeed,fSensingRange,0,VecMoveAiInfo);
//	}
//
//}
////============================================================================================================================================

//====================================================================================
//�e�L�X�g�t�@�C���������ǂݍ���
//====================================================================================
void CShotWeakEnemy::LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff, CObject* pObj)
{
	int nShotWeakEnemyType = 0;                                            //�ˌ��Ɏア�G�^�C�v   
	SHOTWEAKENEMYTYPE ShotWeakEnemyType = {};                              //�ˌ��Ɏア�G�^�C�v
	CShotWeakEnemy* pShotWeakEnemy = dynamic_cast<CShotWeakEnemy*>(pObj);  //�ˌ��Ɏア�G�Ƀ_�E���L���X�g
	
	if (pShotWeakEnemy != nullptr)
	{
		while (Buff != "END_SETSHOTWEAKENEMY")
		{//������END_SETSHOTWEAKENEMY�ɂȂ�܂ŌJ��Ԃ�
			LoadingFile >> Buff;//�P���ǂݍ���
			if (Buff == "#")
			{//#�Ȃ炻�̍s���X�L�b�v
				getline(LoadingFile, Buff);
			}
			else if (Buff == "SHOTWEAKENEMYTYPE")
			{//�ˌ��Ɏア�G�̎��
				LoadingFile >> Buff;//�C�R�[��
				LoadingFile >> nShotWeakEnemyType;
				ShotWeakEnemyType = static_cast<SHOTWEAKENEMYTYPE>(nShotWeakEnemyType);//�ˌ��Ɏア�G�̃^�C�v���i�[
				SetShotWeakEnemyType(ShotWeakEnemyType);               //�ˌ��Ɏア�G�^�C�v��ݒ�
			}
			else if (Buff == "SETENEMY")
			{//�G�̏���ݒ�
				CEnemy::LoadInfoTxt(LoadingFile, listSaveManager, Buff, pObj);
			}
		}

		if (CScene::GetMode() == CScene::MODE_EDIT)
		{//�G�f�B�b�g���[�h��������
			listSaveManager.push_back(this);      //�X�e�[�W�}�l�[�W���[�ɏ���ۑ�����
		}
		else if (CScene::GetMode() == CScene::MODE_GAME)
		{//�Q�[�����[�h�ł̓t�F�[�Y�}�l�[�W���[�ŏ�������̂ň�U����
			SetUseDeath(true);
			SetDeath();
		}
	}
	else
	{//��O����
		assert("�ˌ��Ɏア�G�ւ̃|�C���^�����݂��Ȃ�");
	}
}

//====================================================================================
//�X�e�[�W�}�l�[�W���[�̃I�u�W�F�N�g���`�F���W����
//====================================================================================
CObject* CShotWeakEnemy::ManagerChengeObject(bool bAim)
{
	int nNewType = int(m_ShotWeakEnemyType);//�ˌ��Ɏア�G�^�C�v��int�^�ɃL���X�g
	SHOTWEAKENEMYTYPE NewType = {};         //�V�����ˌ��Ɏア�G�^�C�v�i�[�p
	//=======================================
	//��ނ�ς���
	//=======================================
	if (bAim == true)
	{
		nNewType++;
	}
	else
	{
		nNewType--;
	}
	if (nNewType >= static_cast<int>(SHOTWEAKENEMYTYPE::MAX))
	{
		nNewType = 0;
	}
	if (nNewType < 0)
	{
		nNewType = static_cast<int>(SHOTWEAKENEMYTYPE::MAX) - 1;
	}
	//======================================================================================

	//=======================================
	//�ݒ肷��
	//=======================================
	NewType = SHOTWEAKENEMYTYPE(nNewType);
	//======================================================================================

	//=======================================
	//���S�t���O��ݒ肷��
	//=======================================
	SetUseDeath(true);//���S�t���O���g�p����
	SetDeath();       //���S�t���O��ݒ肷��
	//======================================================================================

	return CShotWeakEnemy::Create(NewType, GetLifeInfo().GetMaxLife(), GetPhaseNum(), GetPosInfo().GetPos(), GetRotInfo().GetRot(), GetSizeInfo().GetScale());//���������I�u�W�F�N�g��Ԃ�
}
//============================================================================================================================================

//====================================================================================
//�X�e�[�W�}�l�[�W���[�Ɍ��݂̃I�u�W�F�N�g��ۑ�����
//====================================================================================
CObject* CShotWeakEnemy::ManagerSaveObject()
{
	auto& Vec = GetVecAiModelInfo();//���ݔz�u���Ă���G�̈ړ�AI�����擾
	auto Vec2 = move(Vec);          //���ݔz�u���Ă���G�̈ړ�AI����ϐ��Ɉړ�
	CShotWeakEnemy* pShotWeakEnemy = CShotWeakEnemy::Create(m_ShotWeakEnemyType, GetLifeInfo().GetMaxLife(), GetPhaseNum(), GetPosInfo().GetPos(), GetRotInfo().GetRot(), GetSizeInfo().GetScale());//���������I�u�W�F�N�g��Ԃ�
	pShotWeakEnemy->SetVecMoveAiInfo(Vec2);            //�ړ�AI����ݒ�
	pShotWeakEnemy->SetNormalSpeed(GetNormalSpeed());  //���݂̓G�̒ʏ푬�x��ۑ�����
	pShotWeakEnemy->SetSensingRange(GetSensingRange());//���݂̓G�̍��G�͈͂�ۑ�����
	return pShotWeakEnemy;
}
//============================================================================================================================================

//====================================================================================
//�o�g���ړ�����
//====================================================================================
void CShotWeakEnemy::BattleMoveProcess()
{
	CEnemy::ChasePlayer();//�v���C���[��ǂ������鏈��
	const D3DXVECTOR3& Move = GetMoveInfo().GetMove();//�ړ��ʂ��擾

	//�f�o�b�O�\��
	CManager::GetDebugText()->PrintDebugText("�ˌ��Ɏア�G�̈ړ��ʁF%f %f %f\n", Move.x, Move.y, Move.z);

	RayCollisionJumpOverOnHit();//���C�������������ɃW�����v�����邽�߂̏���
}
//============================================================================================================================================

//====================================================================================
//�U������
//====================================================================================
void CShotWeakEnemy::AttackProcess()
{
	float fLength = CCalculation::CalculationLength(GetPosInfo().GetPos(), CGame::GetPlayer()->GetPosInfo().GetPos()); //�����ƃv���C���[�̋���
	float fLengthY = CGame::GetPlayer()->GetPosInfo().GetPos().y - GetPosInfo().GetPos().y;                            //�����ƃv���C���[��Y���̋���
	const bool& bAction = GetAction();                                                                                 //�U����Ԃ��ǂ���
	const int& nPatternTime = GetPatternTime();                                                                        //�U���p�^�[���ɓ����Ă���̎��Ԃ��擾����
	const int& nPattern = GetPattern();                                                                                //�p�^�[���ԍ����擾
																													   
	if (fLength < s_fATTACKSTART_LENGTH && bAction == false && fLengthY < 400.0f && GetAttackCoolTime() > s_nATTACK_COOLTIME)
	{//�U�����J�n����Ă��Ȃ���΁i�N�[���^�C�����ƂɍU�����s���B�^�[�����o�g���݂����ȓ����ɂȂ�j
		ChengeMove(DBG_NEW CEnemyMove_None());//AI�ړ��ƍU�����������ւ���
		SetAction(true);                      //�s���J�n
	}

	if (bAction == true)
	{//�s�����J�n����Ă�����
		switch (nPattern)
		{
		case 0://�~�܂��ēːi���������߂�
			GetMoveInfo().SetMove(D3DXVECTOR3(0.0f, GetMoveInfo().GetMove().y, 0.0f));
			if (nPatternTime == 15)
			{//���̃p�^�[���ɓ����Ă���15�t���[����
				SetPattern(nPattern + 1);//�p�^�[���ԍ������ɐi�߂�
				SetPatternTime(0);       //�p�^�[�����Ԃ����Z�b�g
				m_SaveAimPos = CGame::GetPlayer()->GetPosInfo().GetPos();//�ړI�n�����̎��_�ł̃v���C���[�̈ʒu�ɐݒ�
			}
			break;
		case 1://�`���[�W����

			//�p�[�e�B�N��������(�����Ɍ������ē���)
			CParticle::SummonChargeParticle(CParticle::TYPE::TYPE00_NORMAL, 1, 45, 5.0f, 20.0f, 20.0f, GetSizeInfo().GetSize().x, 100, 10, false,
				GetPosInfo().GetSenterPos(), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), true);
			if (nPatternTime == 45)
			{//���̃p�^�[���ɓ����Ă���S�T�t���[����A�_���������ɓːi����
				GetMoveInfo().SetUseInteria(true, 0.05f);//�������g�p���A���Ȃ�キ����

				GetMoveInfo().SetMove(CCalculation::Calculation3DVec(GetPosInfo().GetPos(),m_SaveAimPos, 30.0f));//�p�^�[���O�ő_�����ʒu�ɓːi
				SetPattern(nPattern + 1);//�p�^�[���ԍ������ɐi�߂�
				SetPatternTime(0);       //�p�^�[�����Ԃ����Z�b�g����
			}
			break;
		case 2:
			if (nPatternTime == 60)
			{//���̃p�^�[���ɓ����ĂU�O�t���[����A�U���p�^�[�����I������
				GetMoveInfo().SetUseInteria(false, GetNormalInertia());//�������I�t�ɖ߂�
			    EndAttackPattern();                                    //�U���p�^�[�����I������
				ChengeMove(DBG_NEW CEnemyMove_AI());                   //AI�ړ������ɕς���
			}
			break;
		default:
			break;
		}
		SetPatternTime(nPatternTime + 1);//�p�^�[�����Ԃ��J�E���g����
	}
}
//============================================================================================================================================

//====================================================================================
//���̓����蔻����s��
//====================================================================================
void CShotWeakEnemy::SwordCollision()
{
	if (m_pMagicSword != nullptr && CGame::GetPlayer() != nullptr)
	{//���ƃv���C���[�����݂��Ă�����
		if (CCollision::RectAngleCollisionXZ(m_pMagicSword, CGame::GetPlayer()))
		{//XZ�����̎΂߂̓����蔻����s��

			//XZ�����̃x�N�g�������߂�
			D3DXVECTOR3 Aim = D3DXVECTOR3(CGame::GetPlayer()->GetPosInfo().GetPos().x, 0.0f, CGame::GetPlayer()->GetPosInfo().GetPos().z) - D3DXVECTOR3(GetPosInfo().GetPos().x, 0.0f, GetPosInfo().GetPos().z);

			//�v���C���[�Ƀ_���[�W��^����
			CGame::GetPlayer()->SetDamage(5, 45);

			//�x�N�g���𐳋K��
			D3DXVec3Normalize(&Aim, &Aim);

			//�v���C���[�Ƀm�b�N�o�b�N��Ԃ�t�^���A��L�ŋ��߂��x�N�g���Ɍ������Đ�����΂�
			CGame::GetPlayer()->ChengeAbnormalState(DBG_NEW CPlayerAbnormalState_KnockBack(CGame::GetPlayer(), Aim * 100.0f, 0.1f));
		}
	}
}
//============================================================================================================================================

//====================================================================================
//�|���ꂽ�Ƃ��̉��o
//====================================================================================
void CShotWeakEnemy::DefeatStaging()
{
	if (GetLifeInfo().GetLife() < 1)
	{//�̗͂��Ȃ��Ȃ��Ă�����
		for (int nCnt = 0; nCnt < 60; nCnt++)
		{//�p�[�e�B�N��������
			CParticle* pParticle = CParticle::Create(CParticle::TYPE::TYPE00_NORMAL, 120, 80.0f, 80.0f, GetPosInfo().GetPos(), CCalculation::Rand3DVec(200, 10),
				D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), true);
			pParticle->SetUseAddSpeed(true, true, 0.9f);//����������������
		}

		CEnemy::DefeatStaging();//��ꏈ��
	}
}
//============================================================================================================================================

//====================================================================================
//���C���I�u�W�F�N�g�ɓ����������ɃW�����v�����邽�߂̏���
//====================================================================================
void CShotWeakEnemy::RayCollisionJumpOverOnHit()
{
	//=======================
    //�G�̕ϐ�
    //=======================
	CObjectX::PosInfo& PosInfo = GetPosInfo();              //�ʒu�����擾
	CObjectX::SizeInfo& SizeInfo = GetSizeInfo();           //�T�C�Y�����擾
	CObjectX::MoveInfo& MoveInfo = GetMoveInfo();           //�ړ������擾
	CObjectX::RotInfo& RotInfo = GetRotInfo();              //���������擾

	const D3DXVECTOR3& Pos = PosInfo.GetPos();              //�ʒu
	const D3DXVECTOR3& SenterPos = PosInfo.GetSenterPos();  //���S�ʒu
	const D3DXVECTOR3& Rot = RotInfo.GetRot();              //����
	const D3DXVECTOR3& VtxMax = SizeInfo.GetVtxMax();       //�ő咸�_
	const D3DXVECTOR3& VtxMin = SizeInfo.GetVtxMin();       //�ŏ����_
	const D3DXVECTOR3& Size = SizeInfo.GetSize();           //�T�C�Y
	D3DXVECTOR3 RayOrigin = SenterPos + D3DXVECTOR3(sinf(Rot.y) * (Size.x / 2), 0.0f, cosf(Rot.y) * (Size.x / 2));//���C�̎x�_�͌����Ă�������̎����̒[

	D3DXVECTOR3 RayDir = RayOrigin - SenterPos;             //���C�̃x�N�g�������߂�
	D3DXVec3Normalize(&RayDir, &RayDir);                    //�x�N�g���𐳋K��
	D3DXVECTOR3 RayCollisionPos = { 0.0f,0.0f,0.0f };       //���C�����������Ƃ���i�[�p
	float fLength = 0.0f;                                   //�����i�[�p
	//=====================================================================================

	for (int nCntPri = 0; nCntPri < CObject::m_nMAXPRIORITY; nCntPri++)
	{//�I�u�W�F�N�g���X�g������
		if (nCntPri == static_cast<int>(CObject::TYPE::BLOCK) || nCntPri == static_cast<int>(CObject::TYPE::BGMODEL))
		{//���X�g���u���b�N���͔w�i���f����������
			CObject* pObj = CObject::GetTopObject(nCntPri);//�擪�A�h���X���擾
			while (pObj != nullptr)
			{//�I�u�W�F�N�g��nullptr�ɂȂ�܂ŌJ��Ԃ�
				CObject* pNext = pObj->GetNextObject();//���̃I�u�W�F�N�g���擾

				CObjectX* pObjX = static_cast<CObjectX*>(pObj);//�I�u�W�F�N�gX�ɃL���X�g

				//=======================
				//��r�Ώۂ̕ϐ�
				//=======================
				CObjectX::PosInfo& ComPosInfo = pObjX->GetPosInfo();   //�ʒu�����擾
				CObjectX::RotInfo& ComRotInfo = pObjX->GetRotInfo();   //���������擾
				CObjectX::SizeInfo& ComSizeInfo = pObjX->GetSizeInfo();//�T�C�Y�����擾

				const D3DXVECTOR3& ComPos = ComPosInfo.GetPos();       //�ʒu
				const D3DXVECTOR3& ComRot = ComRotInfo.GetRot();       //����
				const D3DXVECTOR3& ComVtxMax = ComSizeInfo.GetVtxMax();//�ő咸�_
				const D3DXVECTOR3& ComVtxMin = ComSizeInfo.GetVtxMin();//�ŏ����_
				const D3DXVECTOR3& ComSize = ComSizeInfo.GetSize();    //�T�C�Y
				//=====================================================================================

				if (CCollision::RayIntersectsAABBCollisionPos(RayOrigin, RayDir, ComPos + ComVtxMin, ComPos + ComVtxMax, RayCollisionPos))
				{//���C���I�u�W�F�N�g�ɓ������Ă�����
					fLength = CCalculation::CalculationLength(RayOrigin, RayCollisionPos);//���C�̎x�X�ƃ��C�����������ʒu���o��
					if (fLength < 100.0f)
					{
						float fLengthY = (ComPos.y + ComVtxMax.y) - Pos.y;//Y���̋��������A�ڕW�̃W�����v���������߂�

						if (GetJumpCoolTime() > 200)
						{//�W�����v�N�[���^�C����200f�ȏ�ɂȂ�����
							ChengeMove(DBG_NEW CEnemyMove_OverJumpObj(this,fLengthY));//�ړ���Ԃ��u�ǂ𒴂���v�ɂ���
						}
						//���̃I�u�W�F�N�g�̔�r�͂�������Ȃ��̂�return����
						return;
					}
				}

				pObj = pNext;
			}
		}
	}
}
//============================================================================================================================================

//********************************************************************************************************************************************
//�_�C�u�Ɏア�G�N���X
//********************************************************************************************************************************************

//====================================================================================
//�ÓI�����o�ϐ��錾
//====================================================================================
const string CDiveWeakEnemy::s_aDIVEWEAKENEMY_FILENAME[static_cast<int>(CDiveWeakEnemy::DIVEWEAKENEMYTYPE::MAX)] =
{
	"data\\MODEL\\Enemy\\DiveWeak\\angrySlime.x"
};
const int CDiveWeakEnemy::s_nATTACK_FREQUENCY = 105;     //�U���p�x
const float CDiveWeakEnemy::s_fSENSINGRANGE = 2000.0f;   //���G�͈�
const float CDiveWeakEnemy::s_fNORMAL_SPEED = 3.0f;      //�ʏ�ړ����x
//====================================================================================
//�R���X�g���N�^
//====================================================================================
CDiveWeakEnemy::CDiveWeakEnemy(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CEnemy(nPri, bUseintPri, type, ObjType),
m_DiveWeakEnemyType(DIVEWEAKENEMYTYPE::NORMAL),m_nDivisionNum(0),m_bStartDeath(false)
{

}
//============================================================================================================================================

//====================================================================================
//�f�X�g���N�^
//====================================================================================
CDiveWeakEnemy::~CDiveWeakEnemy()
{

}
//============================================================================================================================================

//====================================================================================
//����������
//====================================================================================
HRESULT CDiveWeakEnemy::Init()
{
	CEnemy::Init();                           //�G�̏���������
	SetEnemyType(CEnemy::ENEMYTYPE::DIVEWEAK);//�G�^�C�v��ݒ�
	return S_OK;
}
//============================================================================================================================================

//====================================================================================
//�I������
//====================================================================================
void CDiveWeakEnemy::Uninit()
{
	CEnemy::Uninit();//�G�̏I������
}
//============================================================================================================================================

//====================================================================================
//�X�V����
//====================================================================================
void CDiveWeakEnemy::Update()
{
	CEnemy::Update();//�G�̍X�V����
}
//============================================================================================================================================

//====================================================================================
//�`�揈��
//====================================================================================
void CDiveWeakEnemy::Draw()
{
	CEnemy::Draw();//�G�̕`�揈��
}
//============================================================================================================================================

//====================================================================================
//���S�t���O�ݒ菈��
//====================================================================================
void CDiveWeakEnemy::SetDeath()
{
	if (m_nDivisionNum > 0 && GetDefeatAttackType() == CAttack::ATTACKTYPE::BULLET && GetLifeInfo().GetLife() < 1 && m_bStartDeath == false)
	{ //����񐔂��O��ȏ�œ|���ꂽ�U���̃^�C�v���u�e�v�ő̗͂��Ȃ��Ȃ��Ă�����
		float fPosX = static_cast<float>(rand() % 30 - 15);//X���̏����ʒu
		float fPosZ = static_cast<float>(rand() % 30 - 15);//Z���̏����ʒu
		m_nDivisionNum--;                                  //����񐔌���
		if (m_nDivisionNum == 1)
		{//�c�蕪��񐔂��P�̏ꍇ�A�����ԓG�������Ă��܂��Ă���̂ŁA�������Ȃ��G�ɕ��􂳂���
			CIdleEnemy* pIdleEnemy = CIdleEnemy::Create(CIdleEnemy::IDLEENEMYTYPE::NORMAL, GetLifeInfo().GetMaxLife() / 2, 0, GetPosInfo().GetPos() + D3DXVECTOR3(fPosZ, 100.0f, fPosX), GetRotInfo().GetRot(), GetSizeInfo().GetScale() / 2);
			pIdleEnemy->SetSensingRange(9999.0f);//���G�͈͂�9999�ɐݒ�
			pIdleEnemy = CIdleEnemy::Create(CIdleEnemy::IDLEENEMYTYPE::NORMAL, GetLifeInfo().GetMaxLife() / 2, 0, GetPosInfo().GetPos() + D3DXVECTOR3(fPosZ, 100.0f, fPosX), GetRotInfo().GetRot(), GetSizeInfo().GetScale() / 2);
			pIdleEnemy->SetSensingRange(9999.0f);//���G�͈͂�9999�ɐݒ�
		}
		else
		{//�c�蕪��񐔂��܂������̂ŁA����Ȃɕ��􂵂Ă��Ȃ��Ƃ݂Ȃ��A�_�C�u�Ɏア�G�ɕ��􂳂���
			CDiveWeakEnemy* pDiveWeakEnemy = CDiveWeakEnemy::Create(DIVEWEAKENEMYTYPE::NORMAL, GetLifeInfo().GetMaxLife() / 2, 0, GetPosInfo().GetPos() + D3DXVECTOR3(fPosX, 100.0f, fPosZ), GetRotInfo().GetRot(), GetSizeInfo().GetScale() / 2, m_nDivisionNum);
			pDiveWeakEnemy->SetSensingRange(9999.0f);//���G�͈͂�9999�ɐݒ�
			pDiveWeakEnemy->SetDivisionNum(m_nDivisionNum);//����񐔂�ݒ�
			pDiveWeakEnemy = CDiveWeakEnemy::Create(DIVEWEAKENEMYTYPE::NORMAL, GetLifeInfo().GetMaxLife() / 2, 0, GetPosInfo().GetPos() + D3DXVECTOR3(fPosZ, 100.0f, fPosX), GetRotInfo().GetRot(), GetSizeInfo().GetScale() / 2, m_nDivisionNum);
			pDiveWeakEnemy->SetSensingRange(9999.0f);//���G�͈͂�9999�ɐݒ�
			pDiveWeakEnemy->SetDivisionNum(m_nDivisionNum);//����񐔂�ݒ�

			//�p�[�e�B�N��������
			CParticle::SummonParticle(CParticle::TYPE::TYPE00_NORMAL, 30, 50, 30.0f, 30.0f, 100, 10, true, GetPosInfo().GetPos(), D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), true);
		}
	}

	CEnemy::SetDeath();//�G�̎��S�t���O��ݒ�
}
//============================================================================================================================================

//====================================================================================
//��������
//====================================================================================
CDiveWeakEnemy* CDiveWeakEnemy::Create(DIVEWEAKENEMYTYPE Type, int nLife, int nPhaseNum, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale, int nDivisionNum)
{
	CDiveWeakEnemy* pDiveWeakEnemy = DBG_NEW CDiveWeakEnemy;//�_�C�u�Ɏア�G�𐶐�

	pDiveWeakEnemy->Init();                                                                    //����������
	int nType = static_cast<int>(Type);//�^�C�v�ԍ����i�[
	if (nType < 0 || nType >= static_cast<int>(DIVEWEAKENEMYTYPE::MAX))
	{//��O����
		assert("�z��O�A�N�Z�X�I(CDiveWeakEnemy)");
	}
	else
	{
		//����o�^�A�ǂݍ���
		int nIdx = CManager::GetObjectXInfo()->Regist(s_aDIVEWEAKENEMY_FILENAME[nType]);
		pDiveWeakEnemy->BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(nIdx),
			CManager::GetObjectXInfo()->GetBuffMat(nIdx),
			CManager::GetObjectXInfo()->GetdwNumMat(nIdx),
			CManager::GetObjectXInfo()->GetTexture(nIdx),
			CManager::GetObjectXInfo()->GetColorValue(nIdx));
	}
	pDiveWeakEnemy->SetPhaseNum(nPhaseNum);                                                    //�t�F�[�Y�ԍ���ݒ肷��
	pDiveWeakEnemy->m_DiveWeakEnemyType = Type;                                                //�_�C�u�Ɏア�G�̎�ނ�ݒ�
	pDiveWeakEnemy->GetLifeInfo().SetLife(nLife);                                              //�̗�
	pDiveWeakEnemy->GetLifeInfo().SetMaxLife(nLife);                                           //�ő�̗�
	pDiveWeakEnemy->GetPosInfo().SetPos(pos);                                                  //�ʒu
	pDiveWeakEnemy->GetPosInfo().SetSupportPos(pos);                                           //�x�_�ʒu
	pDiveWeakEnemy->GetRotInfo().SetRot(rot);                                                  //����
	pDiveWeakEnemy->GetSizeInfo().SetScale(Scale);                                             //�g�嗦
	pDiveWeakEnemy->GetSizeInfo().SetFormarScale(Scale);                                       //���̊g�嗦��ݒ�
	pDiveWeakEnemy->SetSensingRange(550.0f);                                                   //���m�˒�
	pDiveWeakEnemy->SetNormalSpeed(s_fNORMAL_SPEED);                                           //�ʏ�ړ����x
	pDiveWeakEnemy->GetMoveInfo().SetUseInteria(false, GetNormalInertia());                    //�������g�p���Ȃ�

	//��������Ă���̎��Ԃ������_���ɐݒ�i��������Ă���̎��Ԃ����ɒB������e�𔭎˂���̂Ń����_�������������邽��
	pDiveWeakEnemy->SetCntTime(rand() % 100 + 1);                

	pDiveWeakEnemy->SetDivisionNum(3);                                                         //����񐔂�ݒ�
	pDiveWeakEnemy->GetLifeInfo().SetAutoDeath(true);                                          //�̗͂��O�ɂȂ����玩���I�Ɏ��S�t���O��ݒ肷��

	pDiveWeakEnemy->SetSize();                                                                 //���f���T�C�Y��ݒ�
	pDiveWeakEnemy->GetLifeInfo().SetAutoSubLife(false);                                       //�����I�ɑ̗͂����炷���ǂ���
	pDiveWeakEnemy->SetManagerObjectType(CObject::MANAGEROBJECTTYPE::DIVEWEAKENEMY);           //�}�l�[�W���[�ŌĂяo�����̎�ނ�ݒ�
	return pDiveWeakEnemy;
}
//============================================================================================================================================

//====================================================================================
//�e�L�X�g�t�@�C���ɏ���ۑ�����
//====================================================================================
void CDiveWeakEnemy::SaveInfoTxt(fstream& WritingFile)
{
	WritingFile << "SETDIVEWEAKENEMY" << endl;//�_�C�u�Ɏア�G�̏���ݒ�
	WritingFile << "DIVEWEAKENEMYTYPE = " << static_cast<int>(m_DiveWeakEnemyType);//�_�C�u�Ɏア�G�̃^�C�v�ԍ��������o��
	switch (m_DiveWeakEnemyType)
	{//�_�C�u�Ɏア�G�̃^�C�v���������o��
	case DIVEWEAKENEMYTYPE::NORMAL:
		WritingFile << " # NORMAL" << endl;//����
		break;
	default:
		assert(false);
		break;
	}

	WritingFile << "DIVISIONNUM = " << m_nDivisionNum << endl; //����񐔂������o��

	CEnemy::SaveInfoTxt(WritingFile);                          //�G�̏��������o��

	WritingFile << "END_SETDIVEWEAKENEMY" << endl;             //�_�C�u�Ɏア�G�̏����o�����I������
}
//============================================================================================================================================

////====================================================================================
////�e�L�X�g�t�@�C���������ǂݍ���
////====================================================================================
//void CDiveWeakEnemy::LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff)
//{
//	int nType = 0;                                           //���
//	int nDiveWeakEnemyType = 0;                              //�_�C�u�Ɏア�G�^�C�v   
//	int nLife = 0;                                           //�̗�
//	int nPhaseNum = 0;                                       //�t�F�[�Y�ԍ�
//	int nDivisionNum = 0;                                    //�����
// 	D3DXVECTOR3 Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);        //�ړ���
//	D3DXVECTOR3 Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);         //�ʒu
//	D3DXVECTOR3 Scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);       //�g�嗦
//	D3DXVECTOR3 Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);         //����
//    DIVEWEAKENEMYTYPE DiveWeakEnemyType = {};                //�w�i���f���̎��
//	ENEMYTYPE EnemyType = {};                                //�G�^�C�v
//														     
//	vector<CAIModel*> VecMoveAi = {};                        //AI���f���̓��I�z��
//	vector<MoveAiInfo> VecMoveAiInfo = {};                   //AI���f�����̓��I�z��
//
//	int nCntMoveAi = 0;                                      //�ړ�AI�̐��J�E���g�p
//	D3DXVECTOR3 MoveAiPos = D3DXVECTOR3(0.0f,0.0f,0.0f);     //�ړ�AI�̈ʒu
//	D3DXVECTOR3 MoveAiRot = D3DXVECTOR3(0.0f,0.0f,0.0f);     //�ړ�AI�̌���
//	D3DXVECTOR3 MoveAiScale = D3DXVECTOR3(0.0f,0.0f,0.0f);   //�ړ�AI�̊g�嗦
//
//	float fNormalSpeed = 0.0f;                               //�ʏ푬�x
//	float fSensingRange = 0.0f;                              //���G����
//	while (Buff != "END_SETDIVEWEAKENEMY")
//	{//�_�C�u�Ɏア�G�̐ݒ肪�I���܂ŌJ��Ԃ�
//		LoadingFile >> Buff;//�P���ǂݍ���
//		if (Buff == "#")
//		{//�s�̃X�L�b�v
//			getline(LoadingFile, Buff);
//		}
//		else if (Buff == "DIVEWEAKENEMYTYPE")
//		{//�_�C�u�Ɏア�G�̃^�C�v
//			LoadingFile >> Buff;//�C�R�[��
//			LoadingFile >> nDiveWeakEnemyType;
//		}
//		else if (Buff == "ENEMYTYPE")
//		{//�G�̃^�C�v
//			LoadingFile >> Buff;//�C�R�[��
//			LoadingFile >> nType;
//		}
//		else if (Buff == "LIFE")
//		{//�̗�
//			LoadingFile >> Buff;//�C�R�[��
//			LoadingFile >> nLife;
//		}
//		else if (Buff == "POS")
//		{//�ʒu
//			LoadingFile >> Buff;//�C�R�[��
//			LoadingFile >> Pos.x;      //�ʒuX
//			LoadingFile >> Pos.y;      //�ʒuY
//			LoadingFile >> Pos.z;      //�ʒuZ
//		}
//		else if (Buff == "ROT")
//		{//����
//			LoadingFile >> Buff;//�C�R�[��
//			LoadingFile >> Rot.x;      //�ʒuX
//			LoadingFile >> Rot.y;      //�ʒuY
//			LoadingFile >> Rot.z;      //�ʒuZ
//		}
//		else if (Buff == "SCALE")
//		{//�g�嗦
//			LoadingFile >> Buff;//�C�R�[��
//			LoadingFile >> Scale.x;      //�g�嗦X
//			LoadingFile >> Scale.y;      //�g�嗦Y
//			LoadingFile >> Scale.z;      //�g�嗦Z
//		}
//		else if (Buff == "PHASENUM")
//		{//�t�F�[�Y�ԍ�
//			LoadingFile >> Buff;      //�C�R�[��
//			LoadingFile >> nPhaseNum; //�t�F�[�Y�ԍ�
//		}
//		else if (Buff == "NORMALSPEED")
//		{//�ʏ�ړ����x
//			LoadingFile >> Buff;//�C�R�[��
//			LoadingFile >> fNormalSpeed;//�ʏ푬�x
//		}
//		else if (Buff == "SENSINGRANGE")
//		{//���G�͈�
//			LoadingFile >> Buff;//�C�R�[��
//			LoadingFile >> fSensingRange;
//		}
//		else if (Buff == "DIVISIONNUM")
//		{//�����
//			LoadingFile >> Buff;//�C�R�[��
//			LoadingFile >> nDivisionNum;//�����
//		}
//		else if (Buff == "SETMOVEAI")
//		{//�ړ�AI�̐ݒ�
//			while (1)
//			{
//				LoadingFile >> Buff;
//				if (Buff == "SETNUM")
//				{//��
//					LoadingFile >> Buff;//�C�R�[��
//					LoadingFile >> nCntMoveAi;//�ԍ�
//					while (1)
//					{
//						LoadingFile >> Buff;
//
//						if (Buff == "POS")
//						{//�ʒu
//							LoadingFile >> Buff;//�C�R�[��
//							LoadingFile >> MoveAiPos.x;      //�ʒuX
//							LoadingFile >> MoveAiPos.y;      //�ʒuY
//							LoadingFile >> MoveAiPos.z;      //�ʒuZ
//						}
//						else if (Buff == "ROT")
//						{//����
//							LoadingFile >> Buff;//�C�R�[��
//							LoadingFile >> MoveAiRot.x;      //�ʒuX
//							LoadingFile >> MoveAiRot.y;      //�ʒuY
//							LoadingFile >> MoveAiRot.z;      //�ʒuZ
//						}
//						else if (Buff == "SCALE")
//						{//�g�嗦
//							LoadingFile >> Buff;//�C�R�[��
//							LoadingFile >> MoveAiScale.x;      //�g�嗦X
//							LoadingFile >> MoveAiScale.y;      //�g�嗦Y
//							LoadingFile >> MoveAiScale.z;      //�g�嗦Z
//						}
//						else if (Buff == "END_SETNUM")
//						{//�ړ�AI�̐ݒ�
//							if (CScene::GetMode() == CScene::MODE_EDIT)
//							{//�G�f�B�b�g���[�h�̎��͕��ʂɑS�Ă̈ړ�AI���o���������ߒ��ړ��I�z��Ɋi�[
//								CAIModel* pAiModel = CAIModel::Create(CAIModel::AIMODELTYPE::MOVEPOINT, MoveAiPos, MoveAiRot, MoveAiScale, nullptr);//AI���f���̐���
//								pAiModel->GetDrawInfo().SetUseDraw(false);  //�`�悵�Ȃ�
//								pAiModel->GetDrawInfo().SetUseShadow(false);//�e��`�悵�Ȃ�
//								VecMoveAi.push_back(pAiModel);              //�ړ�AI�̓��I�z��Ɋi�[
//							}
//							else if (CScene::GetMode() == CScene::MODE_GAME)
//							{//�Q�[�����[�h�̂Ƃ��͂܂��Ă΂�Ă��Ȃ��G�̏ꍇ�A�ړ�AI���̂𑶍݂��������Ȃ������f�[�^�������i�[���A�Ă΂ꂽ�Ƃ���PhaseManager�ɕۑ������f�[�^���g�p���Ĉړ�AI����������
//								MoveAiInfo Info = {};          //�ړ�AI���
//								Info.Pos = MoveAiPos;          //�ʒu
//								Info.Rot = MoveAiRot;          //����
//								Info.Scale = MoveAiScale;      //�g�嗦
//								VecMoveAiInfo.push_back(Info); //���I�z��Ɋi�[
//							}
//							break;
//						}
//					}
//				}
//				else if (Buff == "END_SETMOVEAI")
//				{//�ړ�AI�̐ݒ�I��
//					break;
//				}
//			}
//		}
//	}
//
//	DiveWeakEnemyType = static_cast<DIVEWEAKENEMYTYPE>(nDiveWeakEnemyType);//�_�C�u�Ɏア�G�̃^�C�v
//	EnemyType = static_cast<ENEMYTYPE>(nType);                             //�G�̃^�C�v
//	if (CScene::GetMode() == CScene::MODE_EDIT)
//	{//�G�f�B�b�g���[�h�Ȃ�
//		CDiveWeakEnemy* pDiveWeakEnemy = CDiveWeakEnemy::Create(DiveWeakEnemyType, nLife, nPhaseNum, Pos, Rot, Scale,3);//��������
//		pDiveWeakEnemy->GetDrawInfo().SetUseDraw(true);                                                                 //�`�悷��
//		pDiveWeakEnemy->GetDrawInfo().SetUseShadow(true);                                                               //�e��`�悷��
//		pDiveWeakEnemy->SetVecMoveAiInfo(VecMoveAi);                                                                    //�ړ�AI�̓��I�z���ݒ�
//		pDiveWeakEnemy->SetNormalSpeed(fNormalSpeed);                                                                   //�ʏ�ړ����x��ݒ�
//		pDiveWeakEnemy->SetSensingRange(fSensingRange);                                                                 //���G�͈͂�ݒ�
//		pDiveWeakEnemy->GetMoveInfo().SetUseUpdatePos(true);                                                            //�ʒu���X�V����
//		listSaveManager.push_back(pDiveWeakEnemy);                                                                      //�X�e�[�W�}�l�[�W���[�ɏ���ۑ�����
//	}
//	else if (CScene::GetMode() == CScene::MODE_GAME)
//	{//�Q�[�����[�h�Ȃ�t�F�[�Y�}�l�[�W���[�ɏ���ۑ�
//		CGame::GetPhaseManager()->PushPhaseInfo(Pos, Rot, Scale, nLife, static_cast<int>(EnemyType), nDiveWeakEnemyType, nPhaseNum,fNormalSpeed,fSensingRange,3,VecMoveAiInfo);
//	}
//}
////============================================================================================================================================

//====================================================================================
//�e�L�X�g�t�@�C���������ǂݍ���
//====================================================================================
void CDiveWeakEnemy::LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff, CObject* pObj)
{
	int nDiveWeakEnemyType = 0;                                            //�_�C�u�Ɏア�G�^�C�v   
	int nDivisionNum = 0;                                                  //�����
	DIVEWEAKENEMYTYPE DiveWeakEnemyType = {};                              //�_�C�u�Ɏア�G�^�C�v
	
	while (Buff != "END_SETDIVEWEAKENEMY")
	{//������END_SETSHOTWEAKENEMY�ɂȂ�܂ŌJ��Ԃ�
		LoadingFile >> Buff;//�P���ǂݍ���
		if (Buff == "#")
		{//#�Ȃ炻�̍s���X�L�b�v
			getline(LoadingFile, Buff);
		}
		else if (Buff == "DIVEWEAKENEMYTYPE")
		{//�ˌ��Ɏア�G�̎��
			LoadingFile >> Buff;//�C�R�[��
			LoadingFile >> nDiveWeakEnemyType;
			DiveWeakEnemyType = static_cast<DIVEWEAKENEMYTYPE>(nDiveWeakEnemyType);//�_�C�u�Ɏア�G�̃^�C�v���i�[
			SetDiveWeakEnemyType(DiveWeakEnemyType);//�_�C�u�Ɏア�G�̃^�C�v��ݒ�
		}
		else if (Buff == "DIVISIONNUM")
		{//�����
			LoadingFile >> Buff;         //�C�R�[��
			LoadingFile >> nDivisionNum; //�����
			SetDivisionNum(nDivisionNum);//����񐔂�ݒ�
		}
		else if (Buff == "SETENEMY")
		{//�G�̏���ݒ�
			CEnemy::LoadInfoTxt(LoadingFile, listSaveManager, Buff, pObj);
		}
	}

	if (CScene::GetMode() == CScene::MODE_EDIT)
	{//�G�f�B�b�g���[�h��������
		listSaveManager.push_back(this);//�X�e�[�W�}�l�[�W���[�ɏ���ۑ�����
	}
	else if (CScene::GetMode() == CScene::MODE_GAME)
	{//�Q�[�����[�h�ł̓t�F�[�Y�}�l�[�W���[�ŏ�������̂ň�U����
		SetUseDeath(true);
		SetDeath();
	}
}

//====================================================================================
//�X�e�[�W�}�l�[�W���[�̃I�u�W�F�N�g���`�F���W����
//====================================================================================
CObject* CDiveWeakEnemy::ManagerChengeObject(bool bAim)
{
	int nNewType = int(m_DiveWeakEnemyType);  //���݂̃^�C�v��int�ɃL���X�g
	DIVEWEAKENEMYTYPE NewType = {};           //�V�����^�C�v�i�[�p
	//=======================================
	//��ނ�ς���
	//=======================================
	if (bAim == true)
	{
		nNewType++;
	}
	else
	{
		nNewType--;
	}
	if (nNewType >= static_cast<int>(DIVEWEAKENEMYTYPE::MAX))
	{
		nNewType = 0;
	}
	if (nNewType < 0)
	{
		nNewType = static_cast<int>(DIVEWEAKENEMYTYPE::MAX) - 1;
	}
	//======================================================================================

	//=======================================
	//�ݒ肷��
	//=======================================
	NewType = static_cast<DIVEWEAKENEMYTYPE>(nNewType);
	//======================================================================================

	//=======================================
	//���S�t���O��ݒ肷��
	//=======================================
	SetUseDeath(true);//���S�t���O���g�p����
	SetDeath();       //���S�t���O��ݒ肷��
	//======================================================================================

	return CDiveWeakEnemy::Create(NewType, GetLifeInfo().GetLife(),GetPhaseNum(), GetPosInfo().GetPos(), GetRotInfo().GetRot(), GetSizeInfo().GetScale(),GetDivisionNum());//���������I�u�W�F�N�g��Ԃ�
}
//============================================================================================================================================

//====================================================================================
//�X�e�[�W�}�l�[�W���[�Ɍ��݂̃I�u�W�F�N�g��ۑ�����
//====================================================================================
CObject* CDiveWeakEnemy::ManagerSaveObject()
{
	auto& Vec = GetVecAiModelInfo();//�X�e�[�W�}�l�[�W���[�ő��삵�Ă���G��AI���f�����̓��I�z����擾
	auto Vec2 = move(Vec);          //�X�e�[�W�}�l�[�W���[�ő��삵�Ă������AI���f�����͂�������Ȃ��̂ŁA��������G�̂��߂Ɉړ������Ă���
	CDiveWeakEnemy * pDiveWeakEnemy = CDiveWeakEnemy::Create(m_DiveWeakEnemyType, GetLifeInfo().GetMaxLife(),GetPhaseNum(), GetPosInfo().GetPos(), GetRotInfo().GetRot(), GetSizeInfo().GetScale(),GetDivisionNum());//���������I�u�W�F�N�g��Ԃ�
	pDiveWeakEnemy->SetSensingRange(GetSensingRange());//���݂̓G�̍��G�͈͂�ۑ�����
	pDiveWeakEnemy->SetNormalSpeed(GetNormalSpeed());  //���݂̓G�̒ʏ푬�x��ۑ�����
	pDiveWeakEnemy->SetVecMoveAiInfo(Vec2);            //�ړ�AI���̓��I�z���ݒ�
	return pDiveWeakEnemy;                             //���������I�u�W�F�N�g��Ԃ�
}
//============================================================================================================================================

//====================================================================================
//�X�e�[�W�}�l�[�W���[���瑀�삷��
//====================================================================================
void CDiveWeakEnemy::ManagerChooseControlInfo()
{
	CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();

	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{//L�R���g���[���L�[�������Ȃ���
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//�V�t�g�L�[�������Ȃ���E�E�E
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_U) == true)
			{
				m_nDivisionNum -= 1;
			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_U) == true)
		{
			m_nDivisionNum += 1;
		}
	}
	else
	{//L�R���g���[���L�[�������Ă��Ȃ�
		if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
		{//�V�t�g�L�[�������Ȃ���E�E�E
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_U) == true)
			{
				m_nDivisionNum -= 1;
			}
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_U) == true)
		{
			m_nDivisionNum += 1;
		}
	}

	CManager::GetDebugText()->PrintDebugText("�����(U)�F%d\n", m_nDivisionNum);

	CEnemy::ManagerChooseControlInfo();
}
//============================================================================================================================================

//====================================================================================
//�o�g���ړ�����
//====================================================================================
void CDiveWeakEnemy::BattleMoveProcess()
{
	CEnemy::BattleMoveProcess();
}
//============================================================================================================================================

//====================================================================================
//�U������
//====================================================================================
void CDiveWeakEnemy::AttackProcess()
{
	float fLength = CCalculation::CalculationLength(GetPosInfo().GetPos(), CGame::GetPlayer()->GetPosInfo().GetPos());
	const bool& bAction = GetAction();
	if (fLength < GetSensingRange() && bAction == false)
	{//���G�͈͂ɂ���ꍇ�A�U�����J�n����
		ChengeMove(DBG_NEW CEnemyMove_None());       //�U���������́A�ړ�������̂ŁA�ʏ�ړ������͂����Ȃ��B
		SetAction(true);                             //�U�����J�n
	}
	if (bAction == true)
	{//�U�����J�n���Ă���ꍇ
		//==============
		//�g�p�ϐ�
		//==============
		CObjectX::RotInfo& RotInfo = GetRotInfo();                          //���������擾
		CObjectX::PosInfo& PosInfo = GetPosInfo();                          //�ʒu�����擾
		CObjectX::SizeInfo& SizeInfo = GetSizeInfo();                       //�T�C�Y�����擾
		CObjectX::MoveInfo& MoveInfo = GetMoveInfo();                       //�ړ������擾
		CPlayer* pPlayer = CGame::GetPlayer();                              //�v���C���[�̃|�C���^���擾
		CObjectX::PosInfo& PlayerPosInfo = pPlayer->GetPosInfo();           //�v���C���[�̈ʒu�����擾
		const D3DXVECTOR3& Pos = PosInfo.GetPos();                          //�ʒu���擾
		const D3DXVECTOR3& Move = MoveInfo.GetMove();                       //�ړ��ʂ��擾
		const D3DXVECTOR3& PlayerPos = PlayerPosInfo.GetPos();              //�v���C���[�̈ʒu���擾
		D3DXVECTOR3 Rot = RotInfo.GetRot();                                 //����
		const float & fNormalSpeed = GetNormalSpeed();                      //�ʏ�ړ����x
		float fRotAim = atan2f(PlayerPos.x - Pos.x, PlayerPos.z - Pos.z);   //Z��������Ƀv���C���[�ւ̊p�x�i�ړI�̊p�x�j���v�Z����
		//========================================================================================================

		RotInfo.SetRot(D3DXVECTOR3(Rot.x, CCalculation::CalculationCollectionRot2D(Rot.y, fRotAim, 0.007f, false), Rot.z));//�������v���C���[�֒����������킹�Ă���
		MoveInfo.SetMove(D3DXVECTOR3(sinf(fRotAim) * fNormalSpeed, Move.y, cosf(fRotAim) * fNormalSpeed));                 //�v���C���[�Ɍ������Ĉړ�������

		if (GetCntTime() % s_nATTACK_FREQUENCY == 0)
		{//�U���𔭎�

		    //==============
		    //�g�p�ϐ�
		    //==============
			const D3DXVECTOR3& VtxMax = SizeInfo.GetVtxMax();                          //�ő咸�_���擾
			D3DXVECTOR3 Aim = { 0.0f,0.0f,0.0f };                                      //�_������
			D3DXVECTOR3 ShotPos = Pos + D3DXVECTOR3(0.0f, VtxMax.y, 0.0f);             //���ˈʒu
			D3DXVECTOR3 AddRot = { 0.0f,0.0f,0.0f };                                   //���Z�������
			D3DXVECTOR2 YawPitch = CCalculation::VectorToYawPitch(ShotPos, PlayerPos); //�ړI�n�ւ̊p�x(Yaw��Pitch)���擾
			//========================================================================================================

			//Yaw�̕␳�i�_��������ɐ���������)
			if (YawPitch.y > Rot.y + 0.5f)
			{
				YawPitch.y = Rot.y + 0.5f;
			}
			else if (YawPitch.y < Rot.y - 0.5f)
			{
				YawPitch.y = Rot.y - 0.5f;
			}

		    //���˕����v�Z
		    Aim.x = sinf(YawPitch.y) * 20.0f;//Z���������ɂ��Ă���̂ŁAsin��X�Ƃ���
		    Aim.z = cosf(YawPitch.y) * 20.0f;//Z���������ɂ��Ă���̂ŁBcos��Z�Ƃ���
		    
		    //Pitch�̕␳(�_��������ɐ���������)
		    if (YawPitch.x > 0.3f)
		    {
		    	YawPitch.x = 0.3f;
		    }
		    else if (YawPitch.x < -0.3f)
		    {
		    	YawPitch.x = -0.3f;
		    }
		    
		    Aim.y = sinf(YawPitch.x) * 20.0f;//�������ʂȂ̂ŁAsin��Y�Ƃ���
		    
		    CAttackEnemy* pAttackEnemy = CAttackEnemy::Create(CAttack::ATTACKTYPE::EXPLOSION, CAttack::TARGETTYPE::PLAYER, CAttack::COLLISIONTYPE::SQUARE, true, true,
		    	1, 60, 200, ShotPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), Aim, GetSizeInfo().GetScale() * 0.5f);//��������
		    
		    pAttackEnemy->GetBoundInfo().SetActive(true, D3DXVECTOR3(0.0f, 10.0f, 0.0f), true, 0.5f);     //�o�E���h������
		    pAttackEnemy->SetExtrusionCollisioin(true);                                                   //�����o��������s���A
		    pAttackEnemy->SetHitOtherThanLibing(false);                                                   //�G��v���C���[�ȊO�Ƃ̓����蔻��͍s��Ȃ�
		}
	}
}
//============================================================================================================================================

//====================================================================================
//AI�ړ�����
//====================================================================================
void CDiveWeakEnemy::AIMoveProcess()
{
	if (CScene::GetMode() == CScene::MODE_GAME)
	{//�Q�[�����[�h�Ȃ�AI�ړ�������
		CEnemy::AIMoveProcess();
	}
}
//============================================================================================================================================

//====================================================================================
//�|���ꂽ�Ƃ��̉��o
//====================================================================================
void CDiveWeakEnemy::DefeatStaging()
{
	if (GetLifeInfo().GetLife() < 1)
	{//�̗͂��Ȃ��Ȃ��Ă�����
		for (int nCnt = 0; nCnt < 60; nCnt++)
		{
			//�p�[�e�B�N��������
			CParticle* pParticle = CParticle::Create(CParticle::TYPE::TYPE00_NORMAL, 120, 80.0f, 80.0f, GetPosInfo().GetPos(), CCalculation::Rand3DVec(200, 10),
				D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), true);

			//�p�[�e�B�N��������������������
			pParticle->SetUseAddSpeed(true, true, 0.9f);
		}

		CEnemy::DefeatStaging();//��ꏈ��
	}
}
//============================================================================================================================================

//********************************************************************************************************************************************
//�G�ړ��^�C�v�N���X
//********************************************************************************************************************************************

//====================================================================================
//�R���X�g���N�^
//====================================================================================
CEnemyMove::CEnemyMove()
{

}
//============================================================================================================================================

//====================================================================================
//�f�X�g���N�^
//====================================================================================
CEnemyMove::~CEnemyMove()
{

}
//============================================================================================================================================

//====================================================================================
//����
//====================================================================================
void CEnemyMove::Process(CEnemy* pEnemy)
{

}
//============================================================================================================================================

//********************************************************************************************************************************************
//�G�ړ��^�C�v�FAI�N���X
//********************************************************************************************************************************************

//====================================================================================
//�R���X�g���N�^
//====================================================================================
CEnemyMove_AI::CEnemyMove_AI()
{

}
//============================================================================================================================================

//====================================================================================
//�f�X�g���N�^
//====================================================================================
CEnemyMove_AI::~CEnemyMove_AI()
{

}
//============================================================================================================================================

//====================================================================================
//����
//====================================================================================
void CEnemyMove_AI::Process(CEnemy* pEnemy)
{
	pEnemy->AIMoveProcess();//AI�ړ��������s��
}
//============================================================================================================================================

//********************************************************************************************************************************************
//�G�ړ��^�C�v�F�o�g���N���X
//********************************************************************************************************************************************

//====================================================================================
//�R���X�g���N�^
//====================================================================================
CEnemyMove_Battle::CEnemyMove_Battle()
{

}
//============================================================================================================================================

//====================================================================================
//�f�X�g���N�^
//====================================================================================
CEnemyMove_Battle::~CEnemyMove_Battle()
{

}
//============================================================================================================================================

//====================================================================================
//����
//====================================================================================
void CEnemyMove_Battle::Process(CEnemy* pEnemy)
{
	pEnemy->BattleMoveProcess();//�킢�ړ��������s��
}
//============================================================================================================================================

//********************************************************************************************************************************************
//�G�ړ��^�C�v�F�Ȃ��N���X
//********************************************************************************************************************************************

//====================================================================================
//�R���X�g���N�^
//====================================================================================
CEnemyMove_None::CEnemyMove_None()
{

}
//============================================================================================================================================

//====================================================================================
//�f�X�g���N�^
//====================================================================================
CEnemyMove_None::~CEnemyMove_None()
{

}
//============================================================================================================================================

//====================================================================================
//����
//====================================================================================
void CEnemyMove_None::Process(CEnemy* pEnemy)
{

}
//============================================================================================================================================

//************************************************************************************
//�����ړ��N���X
//************************************************************************************

//====================================================================================
//�R���X�g���N�^
//====================================================================================
CEnemyMove_Frightened::CEnemyMove_Frightened(CEnemy* pEnemy, D3DXVECTOR3 StopPos, int nStateTime) : m_StopPos(StopPos),m_nStateTime(nStateTime),m_pLockOn(nullptr)
{
	D3DXVECTOR3 ScreenPos = D3DXVECTOR3(0.0f, 0.0f,0.0f);//�X�N���[�����W�i�[�p

	//�G�̃X�N���[�����W���擾����
	ScreenPos = CCalculation::CalcWorldToScreenNoViewport(pEnemy->GetPosInfo().GetSenterPos(), *CManager::GetCamera()->GetMtxView(), *CManager::GetCamera()->GetMtxProjection(),
		SCREEN_WIDTH,SCREEN_HEIGHT);

	pEnemy->SetState(CEnemy::STATE::FRIGHTENDED);//������Ԃɂ���

	//���b�N�I���\������L�ŋ��߂��X�N���[�����W�ɏ�������
	m_pLockOn = CUi::Create(CUi::UITYPE::TARGET_000,false,CObject2D::POLYGONTYPE::SENTERROLLING, 200.0f, 200.0f, 100, true, D3DXVECTOR3(ScreenPos.x, ScreenPos.y, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));

	pEnemy->SetAction(false);        //�U�����I�t�ɂ���
	pEnemy->SetPossibleAttack(false);//�U����s�\�ɂ���
	 
	m_pLockOn->SetPolygonRotSpeed(0.1f);                         //���b�N�I���\���̉�]���x��ݒ�
	m_pLockOn->SetUseAddScale(D3DXVECTOR2(-0.01f, -0.01f), true);//���b�N�I���\�����������k������
	m_pLockOn->SetUseDeath(false);                               //���b�N�I���\���̎��S�t���O���g�p���Ȃ�
}
//============================================================================================================================================

//====================================================================================
//�f�X�g���N�^
//====================================================================================
CEnemyMove_Frightened::~CEnemyMove_Frightened()
{
	if (m_pLockOn != nullptr)
	{//���b�N�I���\�������݂��Ă�����
		m_pLockOn->SetUseDeath(true);//���S�t���O���g�p����
		m_pLockOn->SetDeath();       //���S�t���O��ݒ肷��
		m_pLockOn = nullptr;         //���b�N�I���\���̃|�C���^��������
	}
}
//============================================================================================================================================

//====================================================================================
//����
//====================================================================================
void CEnemyMove_Frightened::Process(CEnemy* pEnemy)
{
	//�����Ă���Ԃ͋����n�߂��ʒu�𒆐S�Ƀ����_���Ɉʒu��ݒ肷��
	float fX = static_cast<float>(rand() % 50 - 25);
	float fY = static_cast<float>(rand() % 50 - 25);
	float fZ = static_cast<float>(rand() % 50 - 25);
	pEnemy->GetPosInfo().SetPos(m_StopPos + D3DXVECTOR3(fX, fY, fZ)); //����������
	pEnemy->GetMoveInfo().SetMove(D3DXVECTOR3(0.0f,0.0f,0.0f));       //�ړ��ʂ�0�ɂ���
	m_nStateTime--;                                                   //���̏�ԂɂȂ��Ă��鎞�Ԃ��J�E���g�_�E������
	pEnemy->EndAttackPattern();                                       //������Ԃ̎��͍s���s�\�ɂ������̂ŁA�U���p�^�[�����I�������Â���

	if (m_pLockOn != nullptr)
	{
		D3DXVECTOR3 ScreenPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�X�N���[�����W�i�[�p

		//�G�̒��S�ʒu�̃X�N���[�����W���擾
		ScreenPos = CCalculation::CalcWorldToScreenNoViewport(pEnemy->GetPosInfo().GetSenterPos(), *CManager::GetCamera()->GetMtxView(), *CManager::GetCamera()->GetMtxProjection(),
			SCREEN_WIDTH, SCREEN_HEIGHT);

		m_pLockOn->SetPos(ScreenPos);//������Ԃɂ����G�̃X�N���[�����W��UI��\��
		if (m_pLockOn->GetScale().x < 0.0f && m_pLockOn->GetScale().y < 0.0f)
		{//�g�嗦���O�ȉ��ɂȂ�����
			m_pLockOn->SetUseDeath(true);//���S�t���O���g�p����
			m_pLockOn->SetDeath();       //���S�t���O��ݒ肷��
			m_pLockOn = nullptr;         //���b�N�I���\��������������
		}
	}

	if (m_nStateTime < 1)
	{//���̏�ԂɂȂ�c�莞�Ԃ��Ȃ��Ȃ�����
		pEnemy->SetPossibleAttack(true);            //�U�����\�ɖ߂�
		pEnemy->GetPosInfo().SetPos(m_StopPos);     //������ԂɂȂ����ʒu�ɐݒ�
		pEnemy->SetState(CEnemy::STATE::NORMAL);    //��Ԉُ�𕁒ʂɖ߂�
		pEnemy->ChengeMove(DBG_NEW CEnemyMove_AI());//AI�ړ������ɖ߂�
	}
}
//============================================================================================================================================

//************************************************************************************
//�ǉ���ړ��N���X
//************************************************************************************

//====================================================================================
//�R���X�g���N�^
//====================================================================================
CEnemyMove_DodgeWall::CEnemyMove_DodgeWall(CEnemy* pEnemy, D3DXVECTOR3 DodgeMove)
{
	m_DodgeMove = DodgeMove;//�ǂ��������ړ��ʂ�ݒ�
}
//============================================================================================================================================

//====================================================================================
//�f�X�g���N�^
//====================================================================================
CEnemyMove_DodgeWall::~CEnemyMove_DodgeWall()
{

}
//============================================================================================================================================

//====================================================================================
//����
//====================================================================================
void CEnemyMove_DodgeWall::Process(CEnemy* pEnemy)
{
	pEnemy->GetMoveInfo().SetMove(m_DodgeMove);//�ړ��ʂ�ݒ肷��

	if (pEnemy->GetCollisionWall() == false)
	{//�ǂɓ�����Ȃ��Ȃ��Ă�����U���ړ������ɖ߂�
		pEnemy->ChengeMove(DBG_NEW CEnemyMove_Battle());
	}
}
//============================================================================================================================================

//********************************************************************************************************************************************
//�G�ړ��^�C�v�F�Ȃ��N���X
//********************************************************************************************************************************************

//====================================================================================
//�ÓI�����o�錾
//====================================================================================
const string CIdleEnemy::s_aIDLEENEMY_FILENAME[static_cast<int>(CIdleEnemy::IDLEENEMYTYPE::MAX)] =
{
	"data\\MODEL\\Enemy\\GreenSlime_000.x"
};
//============================================================================================================================================

//====================================================================================
//�R���X�g���N�^
//====================================================================================
CIdleEnemy::CIdleEnemy(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType)
{

}
//============================================================================================================================================

//====================================================================================
//�f�X�g���N�^
//====================================================================================
CIdleEnemy::~CIdleEnemy()
{

}
//============================================================================================================================================

//====================================================================================
//����������
//====================================================================================
HRESULT CIdleEnemy::Init()
{
	CEnemy::Init();//�G�̏���������
	return S_OK;
}
//============================================================================================================================================

//====================================================================================
//�I������
//====================================================================================
void CIdleEnemy::Uninit()
{
	CEnemy::Uninit();//�G�̏I������
}
//============================================================================================================================================

//====================================================================================
//�X�V����
//====================================================================================
void CIdleEnemy::Update()
{
	CEnemy::Update();//�G�̍X�V����
}
//============================================================================================================================================

//====================================================================================
//�`�揈��
//====================================================================================
void CIdleEnemy::Draw()
{
	CEnemy::Draw();//�G�̕`�揈��
}
//============================================================================================================================================

//====================================================================================
//���S�t���O�ݒ菈��
//====================================================================================
void CIdleEnemy::SetDeath()
{
	CEnemy::SetDeath();//�G�̎��S�t���O�ݒ菈��
}
//============================================================================================================================================

//====================================================================================
//��������
//===================================================================================
CIdleEnemy* CIdleEnemy::Create(IDLEENEMYTYPE Type, int nLife, int nPhaseNum, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale)
{
	CIdleEnemy* pIdleEnemy = DBG_NEW CIdleEnemy();//�������Ȃ��G�̓��I���������m��

	//����������
	pIdleEnemy->Init();
	int nType = static_cast<int>(Type);//�^�C�v�ԍ����i�[
	if (nType < 0 || nType >= static_cast<int>(IDLEENEMYTYPE::MAX))
	{//��O����
		assert("�z��O�A�N�Z�X�I(CIdleEnemy)");
	}
	else
	{
		//����o�^�A�ǂݍ���
		int nIdx = CManager::GetObjectXInfo()->Regist(s_aIDLEENEMY_FILENAME[nType]);
		pIdleEnemy->BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(nIdx),
			CManager::GetObjectXInfo()->GetBuffMat(nIdx),
			CManager::GetObjectXInfo()->GetdwNumMat(nIdx),
			CManager::GetObjectXInfo()->GetTexture(nIdx),
			CManager::GetObjectXInfo()->GetColorValue(nIdx));
	}
	//�X�e�[�^�X��ݒ�
	pIdleEnemy->SetPhaseNum(nPhaseNum);                                     //�t�F�[�Y�ԍ���ݒ肷��
	pIdleEnemy->SetEnemyType(CEnemy::ENEMYTYPE::IDLE);                      //�G�̎�ނ�ݒ肷��
	pIdleEnemy->m_IdleEnemyType = Type;                                     //�������Ȃ��G�̃^�C�v��ݒ肷��
	pIdleEnemy->GetLifeInfo().SetLife(nLife);                               //�̗�
	pIdleEnemy->GetLifeInfo().SetMaxLife(nLife);                            //�ő�̗�
	pIdleEnemy->GetPosInfo().SetPos(pos);                                   //�ʒu
	pIdleEnemy->GetPosInfo().SetSupportPos(pos);                            //�x�_�ʒu
	pIdleEnemy->GetRotInfo().SetRot(rot);                                   //����
	pIdleEnemy->GetSizeInfo().SetScale(Scale);                              //�g�嗦
	pIdleEnemy->GetSizeInfo().SetFormarScale(Scale);                        //���̊g�嗦��ݒ�
	pIdleEnemy->SetSensingRange(550.0f);                                    //���m�˒�
	pIdleEnemy->SetNormalSpeed(3.0f);                                       //�ʏ�ړ����x
	pIdleEnemy->GetMoveInfo().SetUseInteria(false, GetNormalInertia());     //������ݒ�i�ʏ�l�j
	pIdleEnemy->SetCntTime(rand() % 100 + 1);                               //�U���^�C�~���O�����炷
	pIdleEnemy->GetLifeInfo().SetAutoDeath(true);                           //�̗͂��O�ɂȂ����Ƃ��Ɏ��S�t���O�𔭓�
	pIdleEnemy->GetLifeInfo().SetAutoSubLife(false);                        //�̗͂����炵�����Ȃ�
	pIdleEnemy->SetSize();                                                  //���f���T�C�Y��ݒ�
	pIdleEnemy->SetManagerObjectType(CObject::MANAGEROBJECTTYPE::IDLEENEMY);//�X�e�[�W�}�l�[�W���[�ŌĂяo�����̎�ނ�ݒ�

	return pIdleEnemy;
}
//============================================================================================================================================

//====================================================================================
//��񏑂��o������
//===================================================================================
void CIdleEnemy::SaveInfoTxt(fstream& WritingFile)
{
	WritingFile << "SETIDLEENEMY" << endl;//�������Ȃ��G�̓ǂݍ��݊J�n�p

	//�������Ȃ��G�̃^�C�v�������o��
	WritingFile << "IDLEENEMYTYPE = " << static_cast<int>(m_IdleEnemyType);
	switch (m_IdleEnemyType)
	{
	case IDLEENEMYTYPE::NORMAL://����
		WritingFile << " # NORMAL" << endl;
		break;
	default://�z��O�̒l�Ȃ̂ŃA�T�[�g�𔭓�
		assert(false);
		break;
	}

	CEnemy::SaveInfoTxt(WritingFile);//�G�̋��ʂ̏��������o��

	WritingFile << "END_SETIDLEENEMY" << endl;//�������Ȃ��G�̓ǂݍ��ݏI���p

}
//============================================================================================================================================

////====================================================================================
////��񃍁[�h����
////===================================================================================
//void CIdleEnemy::LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff)
//{
//	int nType = 0;                                           //���
//	int nIdleEnemyType = 0;                                  //�������Ȃ��G�^�C�v   
//	int nLife = 0;                                           //�̗�
//	int nPhaseNum = 0;                                       //�t�F�[�Y�ԍ�
//	D3DXVECTOR3 Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);        //�ړ���
//	D3DXVECTOR3 Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);         //�ʒu
//	D3DXVECTOR3 Scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);       //�g�嗦
//	D3DXVECTOR3 Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);         //����
//	IDLEENEMYTYPE IdleEnemyType = IDLEENEMYTYPE::NORMAL;     //�������Ȃ��G�̎��
//	ENEMYTYPE EnemyType = {};                                //�G�̕���
//														     
//	vector<CAIModel*> VecMoveAi = {};                        //�ړ�AI�̓��I�z��
//	vector<MoveAiInfo> VecMoveAiInfo = {};                   //�ړ�AI���̓��I�z��
//														     
//	int nCntMoveAi = 0;                                      //�ړ�AI�̐�
//	D3DXVECTOR3 MoveAiPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   //�ړ�AI�̈ʒu
//	D3DXVECTOR3 MoveAiRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   //�ړ�AI�̌���
//	D3DXVECTOR3 MoveAiScale = D3DXVECTOR3(0.0f, 0.0f, 0.0f); //�ړ�AI�̊g�嗦
//
//	float fNormalSpeed = 0.0f;                               //�ʏ푬�x
//	float fSensingRange = 0.0f;                              //���G�����i�U�����[�h�ɓ��鋗��)
//	while (Buff != "END_SETIDLEENEMY")
//	{
//		LoadingFile >> Buff;//�P���ǂݍ���
//		if (Buff == "#")
//		{
//			getline(LoadingFile, Buff);
//		}
//		else if (Buff == "IDLEENEMYTYPE")
//		{
//			LoadingFile >> Buff;//�C�R�[��
//			LoadingFile >> nIdleEnemyType;//�������Ȃ��G�^�C�v��ǂݍ���
//		}
//		else if (Buff == "ENEMYTYPE")
//		{
//			LoadingFile >> Buff;//�C�R�[��
//			LoadingFile >> nType;//�G�̕��ނ�ǂݍ���
//		}
//		else if (Buff == "LIFE")
//		{
//			LoadingFile >> Buff;//�C�R�[��
//			LoadingFile >> nLife;//�̗͂�ǂݍ���
//		}
//		else if (Buff == "POS")
//		{
//			LoadingFile >> Buff;//�C�R�[��
//			LoadingFile >> Pos.x;      //�ʒuX
//			LoadingFile >> Pos.y;      //�ʒuY
//			LoadingFile >> Pos.z;      //�ʒuZ
//		}
//		else if (Buff == "ROT")
//		{
//			LoadingFile >> Buff;//�C�R�[��
//			LoadingFile >> Rot.x;      //����X
//			LoadingFile >> Rot.y;      //����Y
//			LoadingFile >> Rot.z;      //����Z
//		}
//		else if (Buff == "SCALE")
//		{
//			LoadingFile >> Buff;//�C�R�[��
//			LoadingFile >> Scale.x;      //�g�嗦X
//			LoadingFile >> Scale.y;      //�g�嗦Y
//			LoadingFile >> Scale.z;      //�g�嗦Z
//		}
//		else if (Buff == "PHASENUM")
//		{
//			LoadingFile >> Buff;      //�C�R�[��
//			LoadingFile >> nPhaseNum; //�t�F�[�Y�ԍ�
//		}
//		else if (Buff == "NORMALSPEED")
//		{
//			LoadingFile >> Buff;//�C�R�[��
//			LoadingFile >> fNormalSpeed;//�ʏ푬�x
//		}
//		else if (Buff == "SENSINGRANGE")
//		{
//			LoadingFile >> Buff;//�C�R�[��
//			LoadingFile >> fSensingRange;//���G�͈͂�ǂݍ���
//		}
//		else if (Buff == "SETMOVEAI")
//		{//�ړ�AI�̏���ǂݍ���
//			while (1)
//			{
//				LoadingFile >> Buff;
//				if (Buff == "SETNUM")
//				{
//					LoadingFile >> Buff;//�C�R�[��
//					LoadingFile >> nCntMoveAi;//�ԍ�
//					while (1)
//					{
//						LoadingFile >> Buff;
//
//						if (Buff == "POS")
//						{
//							LoadingFile >> Buff;//�C�R�[��
//							LoadingFile >> MoveAiPos.x;      //�ʒuX
//							LoadingFile >> MoveAiPos.y;      //�ʒuY
//							LoadingFile >> MoveAiPos.z;      //�ʒuZ
//						}
//						else if (Buff == "ROT")
//						{
//							LoadingFile >> Buff;//�C�R�[��
//							LoadingFile >> MoveAiRot.x;      //����X
//							LoadingFile >> MoveAiRot.y;      //����Y
//							LoadingFile >> MoveAiRot.z;      //����Z
//						}
//						else if (Buff == "SCALE")
//						{
//							LoadingFile >> Buff;//�C�R�[��
//							LoadingFile >> MoveAiScale.x;      //�g�嗦X
//							LoadingFile >> MoveAiScale.y;      //�g�嗦Y
//							LoadingFile >> MoveAiScale.z;      //�g�嗦Z
//						}
//						else if (Buff == "END_SETNUM")
//						{
//							if (CScene::GetMode() == CScene::MODE_EDIT)
//							{//�G�f�B�b�g���[�h�̏ꍇ�i�G�f�B�b�g���[�h�ł͍ŏ�����S�Ă̓G�ƈړ�AI��\���������̂ŁA��������j
//								CAIModel* pAiModel = CAIModel::Create(CAIModel::AIMODELTYPE::MOVEPOINT, MoveAiPos, MoveAiRot, MoveAiScale, nullptr);//����
//								pAiModel->GetDrawInfo().SetUseDraw(true);//�`�������
//								pAiModel->GetDrawInfo().SetUseShadow(true);//�e��`�悷��
//								VecMoveAi.push_back(pAiModel);
//							}
//							else if (CScene::GetMode() == CScene::MODE_GAME)
//							{//�Q�[�����[�h�̏ꍇ�i�Q�[�����[�h�̓t�F�[�Y���Ői�ނ̂ŁA�ŏ�����ړ�AI�𐶐��������̂ŁA�t�F�[�Y���Ɋi�[����)
//								MoveAiInfo Info = {};//�ړ�AI�̏���������
//								Info.Pos = MoveAiPos;//�ړ�AI�̈ʒu
//								Info.Rot = MoveAiRot;//�ړ�AI�̌���
//								Info.Scale = MoveAiScale;//�ړ�AI�̊g�嗦
//								VecMoveAiInfo.push_back(Info);//�ړ�AI���̓��I�z��Ɋi�[
//							}
//							break;
//						}
//					}
//				}
//				else if (Buff == "END_SETMOVEAI")
//				{//�ړ�AI�̓ǂݍ��݂��I������
//					break;
//				}
//			}
//		}
//	}
//
//	IdleEnemyType = static_cast<IDLEENEMYTYPE>(nIdleEnemyType);//int�^�œǂݍ��񂾉������Ȃ��G�̃^�C�v�ԍ���񋓌^�ɃL���X�g���Ċi�[
//	EnemyType = static_cast<ENEMYTYPE>(nType);                 //�G�̕��ނ�񋓌^�ɃL���X�g
//
//	if (CScene::GetMode() == CScene::MODE_EDIT)
//	{//�G�f�B�b�g���[�h�̏ꍇ�i�t�F�[�Y���͓K�p���Ȃ��̂ŁA�ŏ�����S�Ă̏��𐶐��j
//		CIdleEnemy* pIdleEnemy = CIdleEnemy::Create(IdleEnemyType, nLife, nPhaseNum, Pos, Rot, Scale);//����
//		pIdleEnemy->GetDrawInfo().SetUseDraw(true);        //�`�悷��
//		pIdleEnemy->GetDrawInfo().SetUseShadow(true);      //�e��`�悷��
//		pIdleEnemy->SetVecMoveAiInfo(VecMoveAi);           //�ړ�AI����ݒ�
//		pIdleEnemy->SetNormalSpeed(3.0f);                  //�ʏ푬�x��ݒ�
//		pIdleEnemy->SetSensingRange(fSensingRange);        //���G�͈͂�ݒ�
//		pIdleEnemy->GetMoveInfo().SetUseUpdatePos(true);   //�ʒu�̍X�V���s��
//		listSaveManager.push_back(pIdleEnemy);             //vector�ɏ���ۑ�����
//	}
//	else if (CScene::GetMode() == CScene::MODE_GAME)
//	{//�Q�[�����[�h�Ȃ�i�t�F�[�Y���ŃQ�[���������̂ŁA�t�F�[�Y���ɓG�̏����i�[�j
//		CGame::GetPhaseManager()->PushPhaseInfo(Pos, Rot, Scale, nLife, static_cast<int>(EnemyType),nIdleEnemyType, nPhaseNum,3.0f, fSensingRange, 3, VecMoveAiInfo);
//	}
//}
////============================================================================================================================================

//====================================================================================
//�^�C�v�`�F���W����
//===================================================================================
CObject* CIdleEnemy::ManagerChengeObject(bool bAim)
{
	int nNewType = static_cast<int>(m_IdleEnemyType);//�������Ȃ��G�̃^�C�v�񋓌^��int�^�ɃL���X�g
	IDLEENEMYTYPE NewType = IDLEENEMYTYPE::NORMAL;   //�V�����������Ȃ��G�^�C�v�i�[�p


	//=======================================
	//��ނ�ς���
	//=======================================
	if (bAim == true)
	{//�C���N�������g����
		nNewType++;
	}
	else
	{//�f�N�������g����
		nNewType--;
	}

	if (nNewType >= static_cast<int>(IDLEENEMYTYPE::MAX))
	{//�^�C�v���ő�ɒB������
		nNewType = 0;
	}
	if (nNewType < 0)
	{//�^�C�v���O�����ɂȂ�����
		nNewType = static_cast<int>(IDLEENEMYTYPE::MAX) - 1;
	}
	//======================================================================================

	//=======================================
	//�ݒ肷��
	//=======================================
	NewType = static_cast<IDLEENEMYTYPE>(nNewType);
	//======================================================================================

	//=======================================
	//���S�t���O��ݒ肷��
	//=======================================
	SetUseDeath(true);//���S�t���O���g�p����
	SetDeath();       //���S�t���O��ݒ肷��
	//======================================================================================

	return CIdleEnemy::Create(NewType, GetLifeInfo().GetLife(), GetPhaseNum(), GetPosInfo().GetPos(), GetRotInfo().GetRot(), GetSizeInfo().GetScale());//���������I�u�W�F�N�g��Ԃ�
}
//============================================================================================================================================

//====================================================================================
//�X�e�[�W�}�l�[�W���[�ɏ���ۑ�����
//===================================================================================
CObject* CIdleEnemy::ManagerSaveObject()
{
	CIdleEnemy* pIdleEnemy = CIdleEnemy::Create(m_IdleEnemyType, GetLifeInfo().GetMaxLife(), GetPhaseNum(), GetPosInfo().GetPos(), GetRotInfo().GetRot(), GetSizeInfo().GetScale());//���������I�u�W�F�N�g��Ԃ�
	pIdleEnemy->SetSensingRange(GetSensingRange());//���݂̓G�̍��G�͈͂�ۑ�����
	pIdleEnemy->SetNormalSpeed(GetNormalSpeed());//���݂̓G�̒ʏ푬�x��ۑ�����
	return pIdleEnemy;//���������I�u�W�F�N�g��Ԃ�
}
//============================================================================================================================================

//====================================================================================
//�X�e�[�W�}�l�[�W���[������𑀍삷�鏈��
//===================================================================================
void CIdleEnemy::ManagerChooseControlInfo()
{
	CEnemy::ManagerChooseControlInfo();//�G�̏��𑀍삷��
}
//============================================================================================================================================

//====================================================================================
//�e�L�X�g�t�@�C���������ǂݍ���
//===================================================================================
void CIdleEnemy::LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff, CObject* pObj)
{
	int nIdleWeakEnemyType = 0;                                            //�������Ȃ��G�^�C�v   
	IDLEENEMYTYPE IdleWeakEnemyType = {};                                  //�������Ȃ��G�^�C�v
	CIdleEnemy* pIdleEnemy = dynamic_cast<CIdleEnemy*>(pObj);              //�������Ȃ��G�Ƀ_�E���L���X�g

	while (Buff != "END_SETIDLEENEMY")
	{//������END_SETSHOTWEAKENEMY�ɂȂ�܂ŌJ��Ԃ�
		LoadingFile >> Buff;//�P���ǂݍ���
		if (Buff == "#")
		{//#�Ȃ炻�̍s���X�L�b�v
			getline(LoadingFile, Buff);
		}
		else if (Buff == "IDLEENEMYTYPE")
		{//�ˌ��Ɏア�G�̎��
			LoadingFile >> Buff;//�C�R�[��
			LoadingFile >> nIdleWeakEnemyType;
			IdleWeakEnemyType = static_cast<IDLEENEMYTYPE>(nIdleWeakEnemyType);//�������Ȃ��G�̃^�C�v���i�[
			SetIdleEnemyType(IdleWeakEnemyType);                               //�������Ȃ��G�̃^�C�v��ݒ�
		}
		else if (Buff == "SETENEMY")
		{//�G�̏���ݒ�
			CEnemy::LoadInfoTxt(LoadingFile, listSaveManager, Buff, pObj);
		}
	}

	if (CScene::GetMode() == CScene::MODE_EDIT)
	{//�G�f�B�b�g���[�h��������
		listSaveManager.push_back(this);//�X�e�[�W�}�l�[�W���[�ɏ���ۑ�����
	}
	else if (CScene::GetMode() == CScene::MODE_GAME)
	{//�Q�[�����[�h�ł̓t�F�[�Y�}�l�[�W���[�ŏ�������̂ň�U����
		SetUseDeath(true);
		SetDeath();
	}
}
//============================================================================================================================================

//====================================================================================
//�퓬���[�h�̓���
//===================================================================================
void CIdleEnemy::BattleMoveProcess()
{
	CEnemy::BattleMoveProcess();//��{�I�ȓ���������
}
//============================================================================================================================================

//====================================================================================
//�U������
//===================================================================================
void CIdleEnemy::AttackProcess()
{
	//�u�������Ȃ��v�̂ŁA�U���͂Ȃ�
}
//============================================================================================================================================

//====================================================================================
//AI�ړ�����
//===================================================================================
void CIdleEnemy::AIMoveProcess()
{
	if (CScene::GetMode() == CScene::MODE_GAME)
	{//�Q�[�����[�h�Ȃ�
		CEnemy::AIMoveProcess();//AI�ړ�����
	}
}
//============================================================================================================================================

//====================================================================================
//���j���o
//===================================================================================
void CIdleEnemy::DefeatStaging()
{
	if (GetLifeInfo().GetLife() < 1 && GetUseDeath() == true)
	{//�̗͂��Ȃ��Ȃ��Ă��āA���S�t���O���g�p����Ȃ�
		for (int nCnt = 0; nCnt < 60; nCnt++)
		{//�p�[�e�B�N��������
			CParticle* pParticle = CParticle::Create(CParticle::TYPE::TYPE00_NORMAL, 120, 80.0f, 80.0f, GetPosInfo().GetPos(), CCalculation::Rand3DVec(200, 10),
				D3DXCOLOR(0.678f, 1.0f, 0.184f, 1.0f), true);
			pParticle->SetUseAddSpeed(true, true, 0.9f);//���X�Ɍ���������
		}

		CEnemy::DefeatStaging();//��ꏈ��
	}
}
//============================================================================================================================================

//************************************************************************************
//���j���o
//************************************************************************************

//====================================================================================
//�R���X�g���N�^
//===================================================================================
CEnemyMove_OverJumpObj::CEnemyMove_OverJumpObj(CEnemy* pEnemy, float fGoalheight)
{
	m_fGoalHeight = fGoalheight;//�ڕW�̃W�����v����
}
//============================================================================================================================================

//====================================================================================
//�f�X�g���N�^
//===================================================================================
CEnemyMove_OverJumpObj::~CEnemyMove_OverJumpObj()
{

}
//============================================================================================================================================

//====================================================================================
//����
//===================================================================================
void CEnemyMove_OverJumpObj::Process(CEnemy* pEnemy)
{
	CObjectX::PosInfo& PosInfo = pEnemy->GetPosInfo();     //�ʒu�����擾
	const D3DXVECTOR3& Pos = PosInfo.GetPos();             //�ʒu
	const D3DXVECTOR3& SenterPos = PosInfo.GetSenterPos(); //���S�ʒu
	m_nCntTime++;//���̏�ԂɂȂ��Ă���̎��Ԃ��J�E���g����

	//�p�[�e�B�N������������
	CParticle::SummonChargeParticle(CParticle::TYPE::TYPE00_NORMAL, 1, 60, 5.0f, 40.0f, 40.0f, 200.0f, 100, 10, true, SenterPos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), true);

	if (m_nCntTime == 100)
	{//100f��������
		CObjectX::MoveInfo& MoveInfo = pEnemy->GetMoveInfo();//�ړ������擾����

		const D3DXVECTOR3& Move = MoveInfo.GetMove();//�ړ���

		//�ړ��ʂ�ݒ�
		MoveInfo.SetMove(D3DXVECTOR3(Move.x,CCalculation::GetInitialVelocityHeight(m_fGoalHeight,pEnemy->GetMoveInfo().GetGravity()) + 5.0f,Move.z));
		pEnemy->ResetJumpCoolTime();                      //���ɃW�����v�𔭓�����܂ł̃N�[���^�C�������Z�b�g����
		pEnemy->ChengeMove(DBG_NEW CEnemyMove_Battle()); //�U���ړ������ɖ߂�
	}
}
//============================================================================================================================================