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
	CObjectX::Init();

	GetMoveInfo().SetUseGravity(true, CObjectX::GetNormalGravity());
	GetMoveInfo().SetUseUpdatePos(true);
	return S_OK;
}
//============================================================================================================================================

//====================================================================================
//�I������
//====================================================================================
void CEnemy::Uninit()
{
	CObjectX::Uninit();

	if (m_pEnemyMove != nullptr)
	{
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
	{
		const D3DXVECTOR3& Rot = GetRotInfo().GetRot();
		const D3DXVECTOR3& Pos = GetPosInfo().GetPos();
		const D3DXVECTOR3& PlayerPos = CGame::GetPlayer()->GetPosInfo().GetPos();
		//float fElevaRot = atan2f(PlayerPos.y - Pos.y, sqrtf(powf(PlayerPos.x - Pos.x, 2) + powf(PlayerPos.z - Pos.z, 2)));

		float fLength = CCalculation::CalculationLength(Pos, PlayerPos);

		m_nCntTime++;//���Ԃ��J�E���g����
		m_nJumpCoolTime++;  //�W�����v�܂ł̃N�[���^�C�����J�E���g����
		m_nAttackCoolTime++;//�N�[���^�C�����J�E���g����

		if (GetLanding() == true)
		{
			GetMoveInfo().SetMove(D3DXVECTOR3(GetMoveInfo().GetMove().x,0.0f, GetMoveInfo().GetMove().z));
		}

		m_pEnemyMove->Process(this);//�ړ������S�ʃX�e�[�g

		if (m_bPossibleAttack == true)
		{
			AttackProcess();//�U������
		}

		//�G�̈ʒu�̒���
		CollisionDetectionProcess();

		if (m_Type == ENEMYTYPE::DIVEWEAK && m_bStartLanding == true)
		{//�_�C�u�Ɏア�G�Ȃ�A�ŏ��ɏ�����n�ʂ��痎���Ȃ�
			CheckGrroundDistance_AdjustPos();
		}

	    CObjectX::Update();

		if (GetLanding() == true && m_bStartLanding == false)
		{
			m_bStartLanding = true;
		}

		CollisionProcess();//�����蔻�菈��

		DefeatStaging();//�|���ꂽ�Ƃ��̉��o���s���i�h���N���X���Ƃɕς��j


		if (GetPosInfo().GetPos().y < -100.0f)
		{
			SetDeath();
		}

	}
	if (CScene::GetMode() == CScene::MODE_EDIT)
	{
		GetDrawInfo().ChengeColorProcess(this);

		for (auto it : m_VecMoveAi)
		{
			if (it->GetDrawInfo().GetUseDraw())
			{
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
	CObjectX::Draw();

	for (auto pObj : m_VecMoveAi)
	{
		pObj->ExtraDraw();
	}
}
//============================================================================================================================================

//====================================================================================
//���S�t���O�ݒ菈��
//====================================================================================
void CEnemy::SetDeath()
{
	CObjectX::SetDeath();

	if (GetUseDeath() == true)
	{
		for (auto pObj : m_VecMoveAi)
		{
			if (pObj != nullptr)
			{
				pObj->SetUseDeath(true);
				pObj->SetDeath();
				pObj = nullptr;
			}
		}
		m_VecMoveAi.clear();
	}
}
//============================================================================================================================================

//====================================================================================
//��������
//====================================================================================
//CEnemy* CEnemy::Create(ENEMYTYPE Type, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale)
//{
//	CEnemy* pEnemy = DBG_NEW CEnemy;
//
//	pEnemy->Init();            //����������
//	int nIdx = CManager::GetObjectXInfo()->Regist(m_aENEMY_FILENAME[int(Type)]);
//	pEnemy->BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(nIdx),
//		CManager::GetObjectXInfo()->GetBuffMat(nIdx),
//		CManager::GetObjectXInfo()->GetdwNumMat(nIdx),
//		CManager::GetObjectXInfo()->GetTexture(nIdx),
//		CManager::GetObjectXInfo()->GetColorValue(nIdx));
//
//	pEnemy->SetType(CObject::TYPE::ENEMY);//�I�u�W�F�N�g�^�C�v��ݒ�
//	pEnemy->SetEnemyType(Type);//�G�̎�ނ�ݒ�
//	pEnemy->SetLife(nLife);    //�̗�
//	pEnemy->SetMaxLife(nLife); //�ő�̗�
//	pEnemy->SetPos(pos);       //�ʒu
//	pEnemy->SetSupportPos(pos);//�x�_�ʒu
//	pEnemy->SetRot(rot);       //����
//	pEnemy->SetScale(Scale);   //�g�嗦
//	pEnemy->SetFormarScale(Scale);//���̊g�嗦��ݒ�
//	pEnemy->SetSize();//���f���T�C�Y��ݒ�
//	pEnemy->SetAutoSubLife(false);//�����I�ɑ̗͂����炷���ǂ���
//
//	return pEnemy;
//}
////============================================================================================================================================

//====================================================================================
//�e�L�X�g�t�@�C���ɏ����Z�[�u����
//====================================================================================
void CEnemy::SaveInfoTxt(fstream& WritingFile)
{
	WritingFile << "ENEMYTYPE = " << static_cast<int>(m_Type);
	switch (m_Type)
	{
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
		assert(false);
		break;
	}

	WritingFile << "NORMALSPEED = " << m_fNormalSpeed << endl;
	WritingFile << "SENSINGRANGE = " << m_fSensingRange << endl;

	//�t�F�[�Y�ԍ���ݒ�
	WritingFile << "PHASENUM = " << m_nPhaseNum << endl;

	//�ړ�AI�̈ʒu��ۑ�
	WritingFile << "SETMOVEAI" << endl;

	int nCnt = 0;
	for (auto it = m_VecMoveAi.begin(); it != m_VecMoveAi.end(); ++it)
	{
		WritingFile << "SETNUM = " << nCnt << endl;

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
	WritingFile << "END_SETMOVEAI" << endl;

	CObjectX::SaveInfoTxt(WritingFile);
}
//============================================================================================================================================

//====================================================================================
//�e�L�X�g�t�@�C������������[�h����
//====================================================================================
void CEnemy::LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff)
{
	//int nType = 0;//���
	//int nLife = 0;//�̗�
	//D3DXVECTOR3 Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  //�ړ���
	//D3DXVECTOR3 Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   //�ʒu
	//D3DXVECTOR3 Scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f); //�g�嗦
	//D3DXVECTOR3 Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   //����
	//ENEMYTYPE Type = {};                               //�u���b�N�̎��
	//while (Buff != "END_SETBLOCK")
	//{
	//	LoadingFile >> Buff;//�P���ǂݍ���
	//	if (Buff == "#")
	//	{
	//		getline(LoadingFile, Buff);
	//	}
	//	else if (Buff == "TYPE")
	//	{
	//		LoadingFile >> Buff;//�C�R�[��
	//		LoadingFile >> nType;      //���
	//	}
	//	else if (Buff == "LIFE")
	//	{
	//		LoadingFile >> Buff;//�C�R�[��
	//		LoadingFile >> nLife;      //�̗�
	//	}
	//	else if (Buff == "MOVE")
	//	{
	//		LoadingFile >> Buff;//�C�R�[��
	//		LoadingFile >> Move.x;      //�ړ���X
	//		LoadingFile >> Move.y;      //�ړ���Y
	//		LoadingFile >> Move.z;      //�ړ���Z
	//	}
	//	else if (Buff == "POS")
	//	{
	//		LoadingFile >> Buff;//�C�R�[��
	//		LoadingFile >> Pos.x;      //�ʒuX
	//		LoadingFile >> Pos.y;      //�ʒuY
	//		LoadingFile >> Pos.z;      //�ʒuZ
	//	}
	//	else if (Buff == "ROT")
	//	{
	//		LoadingFile >> Buff;//�C�R�[��
	//		LoadingFile >> Rot.x;      //�ʒuX
	//		LoadingFile >> Rot.y;      //�ʒuY
	//		LoadingFile >> Rot.z;      //�ʒuZ
	//	}
	//	else if (Buff == "SCALE")
	//	{
	//		LoadingFile >> Buff;//�C�R�[��
	//		LoadingFile >> Scale.x;      //�g�嗦X
	//		LoadingFile >> Scale.y;      //�g�嗦Y
	//		LoadingFile >> Scale.z;      //�g�嗦Z
	//	}
	//}
	//Type = static_cast<ENEMYTYPE>(nType);

	//

	//listSaveManager.push_back(CEnemy::Create(Type, nLife, Pos, Rot, Scale));//vector�ɏ���ۑ�����

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
	{
		CManager::GetDebugText()->PrintDebugText("�G���Ă΂�Ă���I\n");
	}

	SetMoveAiPoint();//�ړ�AI�̐ݒ���s��

	PhaseNumDecision();//�t�F�[�Y�ԍ��̌�����s��

	EditNormalSpeed();//�ʏ�ړ����x��ҏW

	EditSensingRange();//���G�͈͂�ҏW

	CObjectX::ManagerChooseControlInfo();
}
//============================================================================================================================================

//====================================================================================
//�ړ�AI�̏���ݒ肷��
//====================================================================================
void CEnemy::SetVecMoveAiInfo(vector<CAIModel*> vec)
{
	m_VecMoveAi = vec;
}
//============================================================================================================================================

//====================================================================================
//�v���C���[��ǂ������鏈��
//====================================================================================
void CEnemy::ChasePlayer()
{
	float fLengthPlayer = CCalculation::CalculationLength(CGame::GetPlayer()->GetPosInfo().GetPos(), GetPosInfo().GetPos());
	const D3DXVECTOR3& PlayerPos = CGame::GetPlayer()->GetPosInfo().GetPos();
	const D3DXVECTOR3& Pos = GetPosInfo().GetPos();
	D3DXVECTOR3 Aim = PlayerPos - Pos;
	D3DXVec3Normalize(&Aim, &Aim);
	float fRot = atan2f(Aim.x, Aim.z);

	D3DXVECTOR3 Move = CCalculation::HormingVecRotXZ(m_fRotMove, GetPosInfo().GetPos(), CGame::GetPlayer()->GetPosInfo().GetSenterPos(), 0.1f, m_fNormalSpeed);
	GetRotInfo().SetRot(D3DXVECTOR3(GetRotInfo().GetRot().x, m_fRotMove, GetRotInfo().GetRot().z));
	GetMoveInfo().SetMove(D3DXVECTOR3(Move.x, GetMoveInfo().GetMove().y, Move.z));
}
//============================================================================================================================================

//====================================================================================
//�G���|���ꂽ�Ƃ��̉��o
//====================================================================================
void CEnemy::DefeatStaging()
{

}
//============================================================================================================================================

//====================================================================================
//�����蔻��̏������s��
//====================================================================================
void CEnemy::CollisionProcess()
{
	GetCollisionInfo().GetSquareInfo().ResetPushOutFirstFlag();//���ꂼ��̎��̉����o������̗D��t���O�����Z�b�g
	SetIsLanding(false);
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
			{
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
			{
				CObjectX* pObjX = static_cast<CObjectX*>(pObj);//�I�u�W�F�N�gX�Ƀ_�E���L���X�g

				CCollision::ResolveExtrusionCollisionSquare(this, pObjX);//�����`�̉����o�����������
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
	float fX = 0;
	float fZ = 0;
	bool bMove = false;
	bool bMoveY = false;
	float fAddPosY = 0.0f;
	auto Input = CManager::GetInputKeyboard();
	float fCameraRot = CManager::GetCamera()->GetRot().y;
	if (Input->GetPress(DIK_LSHIFT))
	{
		if (Input->GetPress(DIK_I))
		{
			fAddPosY = 5.0f;
			bMoveY = true;
		}
		else if (Input->GetPress(DIK_K))
		{
			fAddPosY = -5.0f;
			bMoveY = true;
		}
	}
	else
	{
		if (Input->GetPress(DIK_I))
		{
			fZ = 1;
			bMove = true;
		}
		else if (Input->GetPress(DIK_K))
		{
			fZ = -1;
			bMove = true;
		}
	}
	if (Input->GetPress(DIK_L))
	{
		fX = 1;
		bMove = true;
	}
	else if (Input->GetPress(DIK_J))
	{
		fX = -1;
		bMove = true;
	}

	GetMoveInfo().SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	CManager::GetDebugText()->PrintDebugText("�ړ�AI�̐ݒu�ʒu�̈ړ��FIKLJ\n");

	if (bMove == true)
	{
		float fRot = atan2f(fX, fZ);
		m_MoveAiSavePos += D3DXVECTOR3(sinf(fCameraRot + fRot) * 5.0f, 0.0f, cosf(fCameraRot + fRot) * 5.0f);
	}

	if (bMoveY == true)
	{
		m_MoveAiSavePos += D3DXVECTOR3(0.0f, fAddPosY, 0.0f);
	}

	D3DXVECTOR3 NowPos = m_MoveAiSavePos + GetPosInfo().GetPos();

	CManager::GetDebugText()->PrintDebugText("�ړ�AI�̈ʒu�F%f %f %f\n", NowPos.x,NowPos.y,NowPos.z);
	CManager::GetDebugText()->PrintDebugText("�ړ�AI��ۑ��FO\n");

	CParticle::SummonParticle(CParticle::TYPE::TYPE00_NORMAL, 1, 30, 40.0f, 40.0f, 100, 10, false, NowPos, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), true);

	if (Input->GetPress(DIK_LSHIFT))
	{
		if (m_VecMoveAi.size() > 0)
		{
			if (Input->GetTrigger(DIK_O))
			{
				auto it = m_VecMoveAi.end() - 1;
				(*it)->SetUseDeath(true);
				(*it)->SetDeath();
				m_VecMoveAi.pop_back();
			}
		}
	}
	else
	{
		if (Input->GetTrigger(DIK_O))
		{
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
	{
		//================
	    //�g�p����ϐ�
		//================
		CObjectX::RotInfo& RotInfo = GetRotInfo();//���������擾
		const D3DXVECTOR3& Rot = RotInfo.GetRot();//�������擾
		float fAddRotY = 0.0f;//Y�����̉��Z����
		float fLengthPlayer = CCalculation::CalculationLength(CGame::GetPlayer()->GetPosInfo().GetPos(), GetPosInfo().GetPos());//�v���C���[�Ƃ̋���
		//=========================================================================================================================================

		if (m_VecMoveAi.size() > 0)
		{
			auto it = m_VecMoveAi.begin();//�ŏ��̃|�C���^���擾

			advance(it, m_nIdxMoveAi);//�w�肵�Ă���ԍ��܂Ői�߂�

			float fLength = CCalculation::CalculationLength(GetPosInfo().GetPos(), (*it)->GetPosInfo().GetPos());//�����𑪂�
			float fRot = atan2f((*it)->GetPosInfo().GetPos().x - GetPosInfo().GetPos().x, (*it)->GetPosInfo().GetPos().z - GetPosInfo().GetPos().z);

			RotInfo.SetRot(D3DXVECTOR3(Rot.x,CCalculation::CalculationCollectionRot2D(Rot.y, fRot, 0.1f, false),Rot.z));//���������X�ɖړI�n�֍��킹�Ă���

			GetMoveInfo().SetMove(D3DXVECTOR3(sinf(fRot) * m_fNormalSpeed, GetMoveInfo().GetMove().y, cosf(fRot) * m_fNormalSpeed));

			if (fLength < m_fNormalSpeed + 50.0f)
			{
				m_nIdxMoveAi++;//�ړI�n�����̈ʒu�ɕύX
			}

			int nSize = m_VecMoveAi.size();
			if (m_nIdxMoveAi >= nSize)
			{
				m_nIdxMoveAi = 0;
			}
			if (m_nIdxMoveAi < 0)
			{
				m_nIdxMoveAi = nSize - 1;
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
	float fLengthPlayer = CCalculation::CalculationLength(CGame::GetPlayer()->GetPosInfo().GetPos(), GetPosInfo().GetPos());

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
	{
		CObjectX* pObjX = nullptr;
		D3DXVECTOR3 Ray = CCalculation::RadToVec(D3DXVECTOR3(GetRotInfo().GetRot().x, GetRotInfo().GetRot().y,0.0f) - D3DXVECTOR3(D3DX_PI * 0.5f,0.0f,0.0f));//��O���Ƀ��C���΂�

		for (int nCntPri = 0; nCntPri < m_nMAXPRIORITY; nCntPri++)
		{
			CObject* pObj = GetTopObject(nCntPri);

			while (pObj != nullptr)
			{
				CObject* pNext = pObj->GetNextObject();
				D3DXVECTOR3 CollisionPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				if (pObj->GetType() == CObject::TYPE::BGMODEL || pObj->GetType() == CObject::TYPE::BLOCK)
				{
					CObjectX * pComObjX = static_cast<CObjectX*>(pObj);
					if (CCollision::RayIntersectsAABBCollisionPos(GetPosInfo().GetSenterPos(), Ray, pComObjX->GetPosInfo().GetPos() + pComObjX->GetSizeInfo().GetVtxMin(), pComObjX->GetPosInfo().GetPos() + pComObjX->GetSizeInfo().GetVtxMax(),
						CollisionPos))
					{
						float fLength = sqrtf(powf(CollisionPos.x - GetPosInfo().GetSenterPos().x, 2) +
							powf(CollisionPos.y - GetPosInfo().GetSenterPos().y, 2) + powf(CollisionPos.z - GetPosInfo().GetSenterPos().z,2));
						if (fLength < GetSizeInfo().GetSize().x + 50.0f)
						{
							//�G�̃I�u�W�F�N�g�̃T�C�Y�̏������苗�����������Ȃ���
							pObjX = pComObjX;
						}
					}
				}

				pObj = pNext;
			}
		}

		if (pObjX != nullptr)
		{//���������I�u�W�F�N�g���������ꍇ�A�v���C���[�Ƃ̃x�N�g���̓��ς��Ƃ�A�I�u�W�F�N�g���悯��܂ŉE���ɓ������荶���ɓ������肷��
			D3DXVECTOR3 ObjectAim = pObjX->GetPosInfo().GetPos() - GetPosInfo().GetPos();
			D3DXVec3Normalize(&ObjectAim, &ObjectAim);

			float fDot = CCalculation::DetermineSide3D(GetPosInfo().GetPos(), ObjectAim, D3DXVECTOR3(0.0f, 1.0f, 0.0f), CGame::GetPlayer()->GetPosInfo().GetPos());
			const D3DXVECTOR3& PlayerPos = CGame::GetPlayer()->GetPosInfo().GetPos();
			const D3DXVECTOR3& Pos = GetPosInfo().GetPos();
			D3DXVECTOR3 Aim = PlayerPos - Pos;
			D3DXVec3Normalize(&Aim, &Aim);
			float fRot = atan2f(Aim.x, Aim.z);

			if (fDot > 0.0f)
			{//�x�N�g���ɑ΂��ăv���C���[���E���ɂ�����
				//SetMove(D3DXVECTOR3(sinf(fRot - D3DX_PI * 0.5f) * m_fNormalSpeed, GetMove().y, cosf(fRot - D3DX_PI * 0.5f) * m_fNormalSpeed));
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
	m_Pattern.nPattern = 0;
	m_Pattern.nPatternTime = 0; 
	m_Pattern.bAction = false;
	m_Pattern.nSubPattern = 0;
	m_nAttackCoolTime = 0;
	GetMoveInfo().SetUseGravity(true, GetNormalGravity());
	GetMoveInfo().SetUseInteria(false, GetNormalInertia());
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
		{
			m_nPhaseNum++;
		}
	}
	else
	{
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_4))
		{
			m_nPhaseNum--;
		}
	}
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
	for (int nCntPri = 0; nCntPri < m_nMAXPRIORITY; nCntPri++)
	{
		CObject* pObj = GetTopObject(nCntPri);
		while (pObj != nullptr)
		{
			CObject* pNext = pObj->GetNextObject();
			if (pObj->GetType() == CObject::TYPE::ENEMY || pObj->GetType() == CObject::TYPE::PLAYER)
			{
				CObjectX* pObjX = static_cast<CObjectX*>(pObj);
				if (pObjX != this)
				{
					D3DXVECTOR3 AimVec = D3DXVECTOR3(pObjX->GetPosInfo().GetPos().x, 0.0f, pObjX->GetPosInfo().GetPos().z) -
						D3DXVECTOR3(GetPosInfo().GetPos().x, 0.0f, GetPosInfo().GetPos().z);//XZ���������̃x�N�g�������
					float fLength = CCalculation::CalculationLength(GetPosInfo().GetPos(), pObjX->GetPosInfo().GetPos());
					const D3DXVECTOR3& ComPos = pObjX->GetPosInfo().GetPos();
					const D3DXVECTOR3& ComVtxMax = ComPos + D3DXVECTOR3(pObjX->GetSizeInfo().GetVtxMax().x, 0.0f, 0.0f);
					D3DXVec3Normalize(&AimVec, &AimVec);

					float fComCornarDistance = CCalculation::CalculationLength(ComPos,ComVtxMax);
					float fMyCornarDistance = CCalculation::CalculationLength(GetPosInfo().GetPos(), GetPosInfo().GetPos() + D3DXVECTOR3(GetSizeInfo().GetVtxMax().x, 0.0f, 0.0f));
					float fTotalLength = (fComCornarDistance + fMyCornarDistance);
					if (fLength < fTotalLength &&
						GetPosInfo().GetPos().y + GetSizeInfo().GetVtxMax().y >= pObjX->GetPosInfo().GetPos().y + pObjX->GetSizeInfo().GetVtxMin().y &&
						GetPosInfo().GetPos().y + GetSizeInfo().GetVtxMin().y <= pObjX->GetPosInfo().GetPos().y + pObjX->GetSizeInfo().GetVtxMax().y)
					{
						GetMoveInfo().SetMove(D3DXVECTOR3(GetMoveInfo().GetMove().x -AimVec.x * (fTotalLength - fLength) * 1.0f,GetMoveInfo().GetMove().y, GetMoveInfo().GetMove().z - AimVec.z * (fTotalLength - fLength) * 1.0f));//�U�����̓��������D��I�ɂ��̈ړ��ʂ����蓖�Ă�
					}

					
				}
			}

			pObj = pNext;
		}
	}
}

//====================================================================================
//�Փ˔��菈��
//====================================================================================
void CEnemy::CheckGrroundDistance_AdjustPos()
{
	CObject* pObjBgModel = GetTopObject(static_cast<int>(CObject::TYPE::BGMODEL));//�w�i���f���̃��X�g�̐擪���擾
	CObject* pObjBlock = GetTopObject(static_cast<int>(CObject::TYPE::BLOCK));    //�u���b�N�̃��X�g�̐擪���擾
	CObjectX::PosInfo& PosInfo = GetPosInfo();//�ʒu���
	CObjectX::SizeInfo& SizeInfo = GetSizeInfo();//�T�C�Y���
	CObjectX::MoveInfo& MoveInfo = GetMoveInfo();//�ړ����
	const D3DXVECTOR3& Pos = PosInfo.GetPos();//�ʒu
	const D3DXVECTOR3& PosOld = PosInfo.GetPosOld();//1f�O�̈ʒu
	const D3DXVECTOR3& VtxMin = SizeInfo.GetVtxMin();//�ŏ����_
	const D3DXVECTOR3& VtxMax = SizeInfo.GetVtxMax();//�ő咸�_
	const D3DXVECTOR3& Move = MoveInfo.GetMove();//�ړ���
	D3DXVECTOR3 Dir = D3DXVECTOR3(0.0f, -1.0f, 0.0f);//�������x�N�g�����`
	D3DXVECTOR3 OriginPos = Pos + D3DXVECTOR3(0.0f, VtxMin.y, 0.0f);//���肷��ʒu�i�I�u�W�F�N�g�̒�ʁj
	D3DXVECTOR3 RayCollisionPos = { 0.0f,0.0f,0.0f };//���C�̔���ꏊ�v�Z�p
	bool bCollision = false;
	float fTotalBestNearLength = 0.0f;//���C�����������I�u�W�F�N�g�̋����̒��ň�ԋ߂����������߂�
	int nCntRayCollosionObj = 0;//���C�����������I�u�W�F�N�g�̐����J�E���g����
	//�w�i���f�����X�g
	while (pObjBgModel != nullptr)
	{
		CObject* pNextBgMoodel = pObjBgModel->GetNextObject();
		CBgModel* pBgModel = static_cast<CBgModel*>(pObjBgModel);//�w�i���f���ɃL���X�g
		CObjectX::PosInfo BgModelPosInfo = pBgModel->GetPosInfo();//�w�i���f���̈ʒu���
		CObjectX::SizeInfo BgModelSizeInfo = pBgModel->GetSizeInfo();//�w�i���f���̃T�C�Y���
		const D3DXVECTOR3& BgModelPos = BgModelPosInfo.GetPos();//�w�i���f���̈ʒu
		const D3DXVECTOR3& BgModelPosOld = BgModelPosInfo.GetPosOld();//1f�O�̈ʒu
		const D3DXVECTOR3& BgModelVtxMax = BgModelSizeInfo.GetVtxMax();//�w�i���f���̍ŏ����_
		const D3DXVECTOR3& BgModelVtxMin = BgModelSizeInfo.GetVtxMin();//�w�i���f���̍ő咸
		if (CCollision::RayIntersectsAABBCollisionPos(OriginPos, Dir, BgModelPos + BgModelVtxMin, BgModelPos + BgModelVtxMax, RayCollisionPos))
		{
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
		pObjBgModel = pNextBgMoodel;
	}

	//�u���b�N���X�g
	while (pObjBlock != nullptr)
	{
		CObject* pNextBlock = pObjBlock->GetNextObject();
		CBlock* pBlock = static_cast<CBlock*>(pObjBlock);//�u���b�N�ɃL���X�g
		CObjectX::PosInfo BlockPosInfo = pBlock->GetPosInfo();//�u���b�N�̈ʒu���
		CObjectX::SizeInfo BlockSizeInfo = pBlock->GetSizeInfo();//�u���b�N�̃T�C�Y���
		const D3DXVECTOR3& BlockPos = BlockPosInfo.GetPos();//�u���b�N�̈ʒu
		const D3DXVECTOR3& BlockPosOld = BlockPosInfo.GetPosOld();//1f�O�̈ʒu
		const D3DXVECTOR3& BlockVtxMax = BlockSizeInfo.GetVtxMax();//�u���b�N�̍ŏ����_
		const D3DXVECTOR3& BlockVtxMin = BlockSizeInfo.GetVtxMin();//�u���b�N�̍ő咸
		if (CCollision::RayIntersectsAABBCollisionPos(OriginPos, Dir, BlockPos + BlockVtxMin, BlockPos + BlockVtxMax, RayCollisionPos))
		{
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
		PosInfo.SetPos(PosOld);
		PosInfo.SetPosOld(Pos - Move);
		CManager::GetDebugText()->PrintDebugText("������̂Ŗ߂�\n");
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

	CManager::GetDebugText()->PrintDebugText("�ʏ�ړ����x�ύX�i�T�j�F%f\n",m_fNormalSpeed);
}
//============================================================================================================================================

//====================================================================================
//�ʏ�ړ����x��ҏW
//====================================================================================
void CEnemy::EditSensingRange()
{
	CInputKeyboard* pInput = CManager::GetInputKeyboard();
	CObjectX::PosInfo& PosInfo = GetPosInfo();
	const D3DXVECTOR3& Pos = PosInfo.GetPos();
	if (pInput->GetPress(DIK_LCONTROL) == true)
	{//L�R���g���[���L�[�������Ȃ���
		if (pInput->GetPress(DIK_LSHIFT) == true)
		{//�V�t�g�L�[�������Ȃ���E�E�E
			if (pInput->GetPress(DIK_6) == true)
			{
				m_fSensingRange -= 10.0f;
			}
		}
		else if (pInput->GetPress(DIK_6) == true)
		{
			m_fSensingRange += 10.0f;
		}
	}
	else
	{//L�R���g���[���L�[�������Ă��Ȃ�
		if (pInput->GetPress(DIK_LSHIFT) == true)
		{//�V�t�g�L�[�������Ȃ���E�E�E
			if (pInput->GetTrigger(DIK_6) == true)
			{
				m_fSensingRange -= 10.0f;
			}
		}
		else if (pInput->GetTrigger(DIK_6) == true)
		{
			m_fSensingRange += 10.0f;
		}
	}
	float fRadXZ = static_cast<float>(rand() % 628 - 314) * 0.01f;//XZ�����̊p�x(Yaw)�����߂�
	float fRadY = static_cast<float>(rand() % 628 - 314) * 0.01f; //Y�����̊p�x(Pitch)�����߂�
	D3DXVECTOR3 RandPos = { 0.0f,0.0f,0.0f };

	RandPos.x = m_fSensingRange * sinf(fRadXZ) * cosf(fRadY);//X�����̒����ɑ΂��āA���̑��ʂɏo�����߂ɁAY�����̍����̔�i�������ʂ̔�Ƃ��Ă�����j
	RandPos.z = m_fSensingRange * cosf(fRadXZ) * cosf(fRadY);//Z�����̒����ɑ΂��āA���̑��ʂɏo�����߂ɁAY�����̍����̔�i�������ʂ̔�Ƃ��Ă�����j
	RandPos.y = m_fSensingRange * sinf(fRadY);//Y�����̍��������߂�

	//���G�͈͂�\�����߂Ƀp�[�e�B�N�������G�����̈ʒu�Ƀ����_���ŏo��
	CParticle::Create(CParticle::TYPE00_NORMAL, 200, 30.0f, 30.0f,Pos + RandPos,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), true);

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
const float CShotWeakEnemy::s_fATTACKSTART_LENGTH = 450.0f;//�U���J�n����
const float CShotWeakEnemy::s_fNORMAL_SENSIINGRANGE = 1050.0f;//�ʏ튴�m�˒�
const int CShotWeakEnemy::s_nATTACK_COOLTIME = 60;//�U���̃N�[���^�C��
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
	CEnemy::Init();

	float fRatioRot = static_cast<float>(rand() % 200 - 100) / 100;
	bool bAim = rand() % 2;
	m_pMagicSword = CAttackEnemy::Create(CAttack::ATTACKTYPE::MAGICSWORD, CAttack::TARGETTYPE::PLAYER, CAttack::COLLISIONTYPE::RECTANGLE_XZ,
		false,false,1, 60, 200, GetPosInfo().GetPos(), D3DXVECTOR3(0.0f,D3DX_PI * fRatioRot, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	m_pMagicSword->SetUseDeath(false);

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
	CEnemy::Uninit();
}
//============================================================================================================================================

//====================================================================================
//�X�V����
//====================================================================================
void CShotWeakEnemy::Update()
{
	CEnemy::Update();
	if (m_pMagicSword != nullptr)
	{
		m_pMagicSword->GetPosInfo().SetPos(GetPosInfo().GetPos());
	}
	SwordCollision();//���̓����蔻����s��
}
//============================================================================================================================================

//====================================================================================
//�`�揈��
//====================================================================================
void CShotWeakEnemy::Draw()
{
	CEnemy::Draw();
}
//============================================================================================================================================

//====================================================================================
//���S�t���O�ݒ菈��
//====================================================================================
void CShotWeakEnemy::SetDeath()
{
	CEnemy::SetDeath();
	if (GetUseDeath() == true)
	{
		if (m_pMagicSword != nullptr)
		{
			m_pMagicSword->SetUseDeath(true);
			m_pMagicSword->SetDeath();
		}
	}
}
//============================================================================================================================================

//====================================================================================
//��������
//====================================================================================
CShotWeakEnemy* CShotWeakEnemy::Create(SHOTWEAKENEMYTYPE Type, int nLife, int nPhaseNum, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale)
{
	CShotWeakEnemy* pShotWeakEnemy = DBG_NEW CShotWeakEnemy;

	pShotWeakEnemy->Init();            //����������
	int nIdx = CManager::GetObjectXInfo()->Regist(s_aSHOTWEAKENEMY_FILENAME[int(Type)]);
	pShotWeakEnemy->BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(nIdx),
		CManager::GetObjectXInfo()->GetBuffMat(nIdx),
		CManager::GetObjectXInfo()->GetdwNumMat(nIdx),
		CManager::GetObjectXInfo()->GetTexture(nIdx),
		CManager::GetObjectXInfo()->GetColorValue(nIdx));

	pShotWeakEnemy->SetPhaseNum(nPhaseNum);//�t�F�[�Y�ԍ���ݒ肷��
	pShotWeakEnemy->m_ShotWeakEnemyType = Type;
	pShotWeakEnemy->GetLifeInfo().SetLife(nLife);    //�̗�
	pShotWeakEnemy->GetLifeInfo().SetMaxLife(nLife); //�ő�̗�
	pShotWeakEnemy->GetLifeInfo().SetAutoSubLife(false);
	pShotWeakEnemy->GetPosInfo().SetPos(pos);       //�ʒu
	pShotWeakEnemy->GetPosInfo().SetSupportPos(pos);//�x�_�ʒu
	pShotWeakEnemy->GetRotInfo().SetRot(rot);       //����
	pShotWeakEnemy->GetSizeInfo().SetScale(Scale);   //�g�嗦
	pShotWeakEnemy->GetSizeInfo().SetFormarScale(Scale);//���̊g�嗦��ݒ�
	pShotWeakEnemy->SetSensingRange(1250.0f);//���m�˒�
	pShotWeakEnemy->SetNormalSpeed(10.0f);//�ʏ�ړ����x
	pShotWeakEnemy->GetMoveInfo().SetUseInteria(false, GetNormalInertia());
	if (CScene::GetMode() == CScene::MODE_GAME)
	{//�Q�[�����[�h�̎����������o��
		pShotWeakEnemy->m_pMagicSword->GetSizeInfo().SetScale(Scale * 0.5f);
	}
	pShotWeakEnemy->GetLifeInfo().SetAutoDeath(true);

	pShotWeakEnemy->SetSize();//���f���T�C�Y��ݒ�
	pShotWeakEnemy->SetManagerObjectType(CObject::MANAGEROBJECTTYPE::SHOTWEAKENEMY);           //�}�l�[�W���[�ŌĂяo�����̎�ނ�ݒ�
	return pShotWeakEnemy;
}
//============================================================================================================================================

//====================================================================================
//�e�L�X�g�t�@�C���ɏ���ۑ�����
//====================================================================================
void CShotWeakEnemy::SaveInfoTxt(fstream& WritingFile)
{
	WritingFile << "SETSHOTWEAKENEMY" << endl;
	WritingFile << "SHOTWEAKENEMYTYPE = " << static_cast<int>(m_ShotWeakEnemyType);
	switch (m_ShotWeakEnemyType)
	{
	case SHOTWEAKENEMYTYPE::NORMAL:
		WritingFile << " # NORMAL" << endl;
		break;
	default:
		assert(false);
		break;
	}

	CEnemy::SaveInfoTxt(WritingFile);

	WritingFile << "END_SETSHOTWEAKENEMY" << endl;
}
//============================================================================================================================================

//====================================================================================
//�e�L�X�g�t�@�C���������ǂݍ���
//====================================================================================
void CShotWeakEnemy::LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff)
{
	int nType = 0;                                     //���
	int nShotWeakEnemyType = 0;                        //�V���b�g�Ɏア�G�^�C�v   
	int nLife = 0;                                     //�̗�
	int nPhaseNum = 0;                                 //�t�F�[�Y�ԍ�
	int nDivisionNum = 0;                              //�����
	D3DXVECTOR3 Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  //�ړ���
	D3DXVECTOR3 Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   //�ʒu
	D3DXVECTOR3 Scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f); //�g�嗦
	D3DXVECTOR3 Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   //����
	SHOTWEAKENEMYTYPE ShotWeakEnemyType = {};          //�w�i���f���̎��
	ENEMYTYPE EnemyType = {};

	vector<CAIModel*> VecMoveAi = {};
	vector<MoveAiInfo> VecMoveAiInfo = {};

	int nCntMoveAi = 0;
	D3DXVECTOR3 MoveAiPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 MoveAiRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 MoveAiScale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	float fNormalSpeed = 0.0f; //�ʏ푬�x
	float fSensingRange = 0.0f;//���G����

	while (Buff != "END_SETSHOTWEAKENEMY")
	{
		LoadingFile >> Buff;//�P���ǂݍ���
		if (Buff == "#")
		{
			getline(LoadingFile, Buff);
		}
		else if (Buff == "SHOTWEAKENEMYTYPE")
		{
			LoadingFile >> Buff;//�C�R�[��
			LoadingFile >> nShotWeakEnemyType;
		}
		else if (Buff == "ENEMYTYPE")
		{
			LoadingFile >> Buff;//�C�R�[��
			LoadingFile >> nType;
		}
		else if (Buff == "LIFE")
		{
			LoadingFile >> Buff;//�C�R�[��
			LoadingFile >> nLife;
		}
		else if (Buff == "POS")
		{
			LoadingFile >> Buff;//�C�R�[��
			LoadingFile >> Pos.x;      //�ʒuX
			LoadingFile >> Pos.y;      //�ʒuY
			LoadingFile >> Pos.z;      //�ʒuZ
		}
		else if (Buff == "ROT")
		{
			LoadingFile >> Buff;//�C�R�[��
			LoadingFile >> Rot.x;      //�ʒuX
			LoadingFile >> Rot.y;      //�ʒuY
			LoadingFile >> Rot.z;      //�ʒuZ
		}
		else if (Buff == "SCALE")
		{
			LoadingFile >> Buff;//�C�R�[��
			LoadingFile >> Scale.x;      //�g�嗦X
			LoadingFile >> Scale.y;      //�g�嗦Y
			LoadingFile >> Scale.z;      //�g�嗦Z
		}
		else if (Buff == "PHASENUM")
		{
			LoadingFile >> Buff;//�C�R�[��
			LoadingFile >> nPhaseNum;//�t�F�[�Y�ԍ�
		}
		else if (Buff == "NORMALSPEED")
		{
			LoadingFile >> Buff;//�C�R�[��
			LoadingFile >> fNormalSpeed;//�ʏ푬�x
		}
		else if (Buff == "SENSINGRANGE")
		{
			LoadingFile >> Buff;//�C�R�[��
			LoadingFile >> fSensingRange;
		}
		else if (Buff == "SETMOVEAI")
		{
			while (1)
			{
				LoadingFile >> Buff;
				if (Buff == "SETNUM")
				{
					LoadingFile >> Buff;//�C�R�[��
					LoadingFile >> nCntMoveAi;//�ԍ�
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
							{
								CAIModel* pAiModel = CAIModel::Create(CAIModel::AIMODELTYPE::MOVEPOINT, MoveAiPos, MoveAiRot, MoveAiScale, nullptr);
								pAiModel->GetDrawInfo().SetUseDraw(false);
								pAiModel->GetDrawInfo().SetUseShadow(false);
								VecMoveAi.push_back(pAiModel);
							}
							else if (CScene::GetMode() == CScene::MODE_GAME)
							{
								MoveAiInfo Info = {};
								Info.Pos = MoveAiPos;
								Info.Rot = MoveAiRot;
								Info.Scale = MoveAiScale;
								VecMoveAiInfo.push_back(Info);
							}
							break;
						}
					}
				}
				else if (Buff == "END_SETMOVEAI")
				{
					break;
				}
			}
		}
	}
	ShotWeakEnemyType = static_cast<SHOTWEAKENEMYTYPE>(nShotWeakEnemyType);
	EnemyType = static_cast<ENEMYTYPE>(nType);
	if (CScene::GetMode() == CScene::MODE_EDIT)
	{
		CShotWeakEnemy* pShotWeakEnemy = CShotWeakEnemy::Create(ShotWeakEnemyType,nLife,nPhaseNum,Pos,Rot,Scale);
		pShotWeakEnemy->SetVecMoveAiInfo(VecMoveAi);
		pShotWeakEnemy->SetNormalSpeed(fNormalSpeed);
		pShotWeakEnemy->SetSensingRange(fSensingRange);
		listSaveManager.push_back(pShotWeakEnemy);      //vector�ɏ���ۑ�����

	}
	else if (CScene::GetMode() == CScene::MODE_GAME)
	{
		CGame::GetPhaseManager()->PushPhaseInfo(Pos, Rot, Scale, nLife, static_cast<int>(EnemyType), nShotWeakEnemyType, nPhaseNum,fNormalSpeed,fSensingRange,0,VecMoveAiInfo);
	}

}
//============================================================================================================================================

//====================================================================================
//�X�e�[�W�}�l�[�W���[�̃I�u�W�F�N�g���`�F���W����
//====================================================================================
CObject* CShotWeakEnemy::ManagerChengeObject(bool bAim)
{
	int nNewType = int(m_ShotWeakEnemyType);
	SHOTWEAKENEMYTYPE NewType = {};
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
	SetUseDeath(true);
	SetDeath();
	//======================================================================================

	return CShotWeakEnemy::Create(NewType, GetLifeInfo().GetMaxLife(), GetPhaseNum(), GetPosInfo().GetPos(), GetRotInfo().GetRot(), GetSizeInfo().GetScale());//���������I�u�W�F�N�g��Ԃ�
}
//============================================================================================================================================

//====================================================================================
//�X�e�[�W�}�l�[�W���[�Ɍ��݂̃I�u�W�F�N�g��ۑ�����
//====================================================================================
CObject* CShotWeakEnemy::ManagerSaveObject()
{
	auto& Vec = GetVecAiModelInfo();
	auto Vec2 = move(Vec);
	CShotWeakEnemy* pShotWeakEnemy = CShotWeakEnemy::Create(m_ShotWeakEnemyType, GetLifeInfo().GetMaxLife(), GetPhaseNum(), GetPosInfo().GetPos(), GetRotInfo().GetRot(), GetSizeInfo().GetScale());//���������I�u�W�F�N�g��Ԃ�
	pShotWeakEnemy->SetVecMoveAiInfo(Vec2);
	pShotWeakEnemy->SetNormalSpeed(GetNormalSpeed());//���݂̓G�̒ʏ푬�x��ۑ�����
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
	CManager::GetDebugText()->PrintDebugText("�ˌ��Ɏア�G�̈ړ��ʁF%f %f %f\n", Move.x, Move.y, Move.z);

	RayCollisionJumpOverOnHit();//���C�������������ɃW�����v�����邽�߂̏���
}
//============================================================================================================================================

//====================================================================================
//�U������
//====================================================================================
void CShotWeakEnemy::AttackProcess()
{
	float fLength = CCalculation::CalculationLength(GetPosInfo().GetPos(), CGame::GetPlayer()->GetPosInfo().GetPos());
	float fLengthY = CGame::GetPlayer()->GetPosInfo().GetPos().y - GetPosInfo().GetPos().y;
	const bool& bAction = GetAction();
	const int& nPatternTime = GetPatternTime();
	const int& nPattern = GetPattern();

	if (fLength < s_fATTACKSTART_LENGTH && bAction == false && fLengthY < 400.0f && GetAttackCoolTime() > s_nATTACK_COOLTIME)
	{//�U�����J�n����Ă��Ȃ���΁i�N�[���^�C�����ƂɍU�����s���B�^�[�����o�g���݂����ȓ����ɂȂ�j
		ChengeMove(DBG_NEW CEnemyMove_None());//AI�ړ��ƍU�����������ւ���
		SetAction(true);
	}

	if (bAction == true)
	{
		switch (nPattern)
		{
		case 0://�~�܂��ēːi���������߂�
			GetMoveInfo().SetMove(D3DXVECTOR3(0.0f, GetMoveInfo().GetMove().y, 0.0f));
			if (nPatternTime == 15)
			{
				SetPattern(nPattern + 1);
				SetPatternTime(0);
				m_SaveAimPos = CGame::GetPlayer()->GetPosInfo().GetPos();
			}
			break;
		case 1://�`���[�W����
			CParticle::SummonChargeParticle(CParticle::TYPE::TYPE00_NORMAL, 1, 45, 5.0f, 20.0f, 20.0f, GetSizeInfo().GetSize().x, 100, 10, false,
				GetPosInfo().GetSenterPos(), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), true);

			if (nPatternTime == 45)
			{//�_���������ɓːi����
				GetMoveInfo().SetUseInteria(true, 0.05f);

				GetMoveInfo().SetMove(CCalculation::Calculation3DVec(GetPosInfo().GetPos(),m_SaveAimPos, 30.0f));
				SetPattern(nPattern + 1);
				SetPatternTime(0);
			}
			break;
		case 2:
			if (nPatternTime == 60)
			{//�U���p�^�[�����I������
				GetMoveInfo().SetUseInteria(false, GetNormalInertia());//�������I�t�ɖ߂�
			    EndAttackPattern();//�U���p�^�[�����I������
				ChengeMove(DBG_NEW CEnemyMove_AI());//AI�ړ������ɕς���
			}
			break;
		default:
			break;
		}
		SetPatternTime(nPatternTime + 1);
	}
}
//============================================================================================================================================

//====================================================================================
//���̓����蔻����s��
//====================================================================================
void CShotWeakEnemy::SwordCollision()
{
	if (m_pMagicSword != nullptr && CGame::GetPlayer() != nullptr)
	{

		if (CCollision::RectAngleCollisionXZ(m_pMagicSword, CGame::GetPlayer()))
		{
			D3DXVECTOR3 Aim = D3DXVECTOR3(CGame::GetPlayer()->GetPosInfo().GetPos().x, 0.0f, CGame::GetPlayer()->GetPosInfo().GetPos().z) - D3DXVECTOR3(GetPosInfo().GetPos().x, 0.0f, GetPosInfo().GetPos().z);

			CGame::GetPlayer()->SetDamage(5, 45);

			D3DXVec3Normalize(&Aim, &Aim);

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
	{
		for (int nCnt = 0; nCnt < 60; nCnt++)
		{
			CParticle* pParticle = CParticle::Create(CParticle::TYPE::TYPE00_NORMAL, 120, 80.0f, 80.0f, GetPosInfo().GetPos(), CCalculation::Rand3DVec(200, 10),
				D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), true);
			pParticle->SetUseAddSpeed(true, true, 0.9f);
		}
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
	CObjectX::PosInfo& PosInfo = GetPosInfo();//�ʒu�����擾
	CObjectX::SizeInfo& SizeInfo = GetSizeInfo();//�T�C�Y�����擾
	CObjectX::MoveInfo& MoveInfo = GetMoveInfo();//�ړ������擾
	CObjectX::RotInfo& RotInfo = GetRotInfo();//���������擾

	const D3DXVECTOR3& Pos = PosInfo.GetPos();//�ʒu
	const D3DXVECTOR3& SenterPos = PosInfo.GetSenterPos();//���S�ʒu
	const D3DXVECTOR3& Rot = RotInfo.GetRot();//����
	const D3DXVECTOR3& VtxMax = SizeInfo.GetVtxMax();//�ő咸�_
	const D3DXVECTOR3& VtxMin = SizeInfo.GetVtxMin();//�ŏ����_
	const D3DXVECTOR3& Size = SizeInfo.GetSize();//�T�C�Y
	D3DXVECTOR3 RayOrigin = SenterPos + D3DXVECTOR3(sinf(Rot.y) * (Size.x / 2), 0.0f, cosf(Rot.y) * (Size.x / 2));//���C�̎��_�����߂�

	D3DXVECTOR3 RayDir = RayOrigin - SenterPos;
	D3DXVec3Normalize(&RayDir, &RayDir);
	D3DXVECTOR3 RayCollisionPos = { 0.0f,0.0f,0.0f };//���C�����������Ƃ���i�[�p
	float fLength = 0.0f;//�����i�[�p
	//=====================================================================================

	for (int nCntPri = 0; nCntPri < CObject::m_nMAXPRIORITY; nCntPri++)
	{
		if (nCntPri == static_cast<int>(CObject::TYPE::BLOCK) || nCntPri == static_cast<int>(CObject::TYPE::BGMODEL))
		{
			CObject* pObj = CObject::GetTopObject(nCntPri);//�擪�A�h���X���擾
			while (pObj != nullptr)
			{
				CObject* pNext = pObj->GetNextObject();

				CObjectX* pObjX = static_cast<CObjectX*>(pObj);

				//=======================
				//��r�Ώۂ̕ϐ�
				//=======================
				CObjectX::PosInfo& ComPosInfo = pObjX->GetPosInfo();//�ʒu�����擾
				CObjectX::RotInfo& ComRotInfo = pObjX->GetRotInfo();//���������擾
				CObjectX::SizeInfo& ComSizeInfo = pObjX->GetSizeInfo();//�T�C�Y�����擾

				const D3DXVECTOR3& ComPos = ComPosInfo.GetPos();//�ʒu
				const D3DXVECTOR3& ComRot = ComRotInfo.GetRot();//����
				const D3DXVECTOR3& ComVtxMax = ComSizeInfo.GetVtxMax();//�ő咸�_
				const D3DXVECTOR3& ComVtxMin = ComSizeInfo.GetVtxMin();//�ŏ����_
				const D3DXVECTOR3& ComSize = ComSizeInfo.GetSize();//�T�C�Y
				//=====================================================================================

				if (CCollision::RayIntersectsAABBCollisionPos(RayOrigin, RayDir, ComPos + ComVtxMin, ComPos + ComVtxMax, RayCollisionPos))
				{
					fLength = CCalculation::CalculationLength(RayOrigin, RayCollisionPos);//���C�̎x�X�ƃ��C�����������ʒu���o��
					if (fLength < 100.0f)
					{
						float fLengthY = (ComPos.y + ComVtxMax.y) - Pos.y;//Y���̋��������A�ڕW�̃W�����v���������߂�

						if (GetJumpCoolTime() > 200)
						{
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
const int CDiveWeakEnemy::s_nATTACK_FREQUENCY = 105;//�U���p�x
const float CDiveWeakEnemy::s_fSENSINGRANGE = 2000.0f;
const float CDiveWeakEnemy::s_fNORMAL_SPEED = 3.0f;
//====================================================================================
//�R���X�g���N�^
//====================================================================================
CDiveWeakEnemy::CDiveWeakEnemy(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CEnemy(nPri, bUseintPri, type, ObjType),
m_DiveWeakEnemyType(DIVEWEAKENEMYTYPE::NORMAL),m_nDivisionNum(0)
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
	CEnemy::Init();
	SetEnemyType(CEnemy::ENEMYTYPE::DIVEWEAK);//�G�^�C�v��ݒ�
	return S_OK;
}
//============================================================================================================================================

//====================================================================================
//�I������
//====================================================================================
void CDiveWeakEnemy::Uninit()
{
	CEnemy::Uninit();
}
//============================================================================================================================================

//====================================================================================
//�X�V����
//====================================================================================
void CDiveWeakEnemy::Update()
{
	CEnemy::Update();
}
//============================================================================================================================================

//====================================================================================
//�`�揈��
//====================================================================================
void CDiveWeakEnemy::Draw()
{
	CEnemy::Draw();
}
//============================================================================================================================================

//====================================================================================
//���S�t���O�ݒ菈��
//====================================================================================
void CDiveWeakEnemy::SetDeath()
{
	if (m_nDivisionNum > 0 && GetDefeatAttackType() == CAttack::ATTACKTYPE::BULLET && GetLifeInfo().GetLife() < 1 && m_bStartDeath == false)
	{ 
		m_bStartDeath = true;
		float fPosX = static_cast<float>(rand() % 30 - 15);
		float fPosZ = static_cast<float>(rand() % 30 - 15);
		m_nDivisionNum--;
		if (m_nDivisionNum == 1)
		{//�c�蕪��񐔂��P�̏ꍇ�A�����ԓG�������Ă��܂��Ă���̂ŁA�������Ȃ��G�ɕ��􂳂���
			CIdleEnemy* pIdleEnemy = CIdleEnemy::Create(CIdleEnemy::IDLEENEMYTYPE::NORMAL, GetLifeInfo().GetMaxLife() / 2, 0, GetPosInfo().GetPos() + D3DXVECTOR3(fPosZ, 100.0f, fPosX), GetRotInfo().GetRot(), GetSizeInfo().GetScale() / 2);
			pIdleEnemy->SetSensingRange(9999.0f);
			pIdleEnemy = CIdleEnemy::Create(CIdleEnemy::IDLEENEMYTYPE::NORMAL, GetLifeInfo().GetMaxLife() / 2, 0, GetPosInfo().GetPos() + D3DXVECTOR3(fPosZ, 100.0f, fPosX), GetRotInfo().GetRot(), GetSizeInfo().GetScale() / 2);
			pIdleEnemy->SetSensingRange(9999.0f);
		}
		else
		{//�c�蕪��񐔂��܂������̂ŁA����Ȃɕ��􂵂Ă��Ȃ��Ƃ݂Ȃ��A�_�C�u�Ɏア�G�ɕ��􂳂���
			CDiveWeakEnemy* pDiveWeakEnemy = CDiveWeakEnemy::Create(DIVEWEAKENEMYTYPE::NORMAL, GetLifeInfo().GetMaxLife() / 2, 0, GetPosInfo().GetPos() + D3DXVECTOR3(fPosX, 100.0f, fPosZ), GetRotInfo().GetRot(), GetSizeInfo().GetScale() / 2, m_nDivisionNum);
			pDiveWeakEnemy->SetSensingRange(9999.0f);
			pDiveWeakEnemy->SetDivisionNum(m_nDivisionNum);
			pDiveWeakEnemy = CDiveWeakEnemy::Create(DIVEWEAKENEMYTYPE::NORMAL, GetLifeInfo().GetMaxLife() / 2, 0, GetPosInfo().GetPos() + D3DXVECTOR3(fPosZ, 100.0f, fPosX), GetRotInfo().GetRot(), GetSizeInfo().GetScale() / 2, m_nDivisionNum);
			pDiveWeakEnemy->SetSensingRange(9999.0f);
			pDiveWeakEnemy->SetDivisionNum(m_nDivisionNum);
			CParticle::SummonParticle(CParticle::TYPE::TYPE00_NORMAL, 30, 50, 30.0f, 30.0f, 100, 10, true, GetPosInfo().GetPos(), D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), true);
		}
	}

	CEnemy::SetDeath();
}
//============================================================================================================================================

//====================================================================================
//��������
//====================================================================================
CDiveWeakEnemy* CDiveWeakEnemy::Create(DIVEWEAKENEMYTYPE Type, int nLife, int nPhaseNum, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale, int nDivisionNum)
{
	CDiveWeakEnemy* pDiveWeakEnemy = DBG_NEW CDiveWeakEnemy;

	pDiveWeakEnemy->Init();            //����������
	int nIdx = CManager::GetObjectXInfo()->Regist(s_aDIVEWEAKENEMY_FILENAME[int(Type)]);
	pDiveWeakEnemy->BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(nIdx),
		CManager::GetObjectXInfo()->GetBuffMat(nIdx),
		CManager::GetObjectXInfo()->GetdwNumMat(nIdx),
		CManager::GetObjectXInfo()->GetTexture(nIdx),
		CManager::GetObjectXInfo()->GetColorValue(nIdx));

	pDiveWeakEnemy->SetPhaseNum(nPhaseNum);//�t�F�[�Y�ԍ���ݒ肷��
	pDiveWeakEnemy->m_DiveWeakEnemyType = Type;
	pDiveWeakEnemy->GetLifeInfo().SetLife(nLife);    //�̗�
	pDiveWeakEnemy->GetLifeInfo().SetMaxLife(nLife); //�ő�̗�
	pDiveWeakEnemy->GetPosInfo().SetPos(pos);       //�ʒu
	pDiveWeakEnemy->GetPosInfo().SetSupportPos(pos);//�x�_�ʒu
	pDiveWeakEnemy->GetRotInfo().SetRot(rot);       //����
	pDiveWeakEnemy->GetSizeInfo().SetScale(Scale);   //�g�嗦
	pDiveWeakEnemy->GetSizeInfo().SetFormarScale(Scale);//���̊g�嗦��ݒ�
	pDiveWeakEnemy->SetSensingRange(550.0f);//���m�˒�
	pDiveWeakEnemy->SetNormalSpeed(s_fNORMAL_SPEED);//�ʏ�ړ����x
	pDiveWeakEnemy->GetMoveInfo().SetUseInteria(false, GetNormalInertia());
	pDiveWeakEnemy->SetCntTime(rand() % 100 + 1);
	pDiveWeakEnemy->SetDivisionNum(3);
	pDiveWeakEnemy->GetLifeInfo().SetAutoDeath(true);

	pDiveWeakEnemy->SetSize();//���f���T�C�Y��ݒ�
	pDiveWeakEnemy->GetLifeInfo().SetAutoSubLife(false);//�����I�ɑ̗͂����炷���ǂ���
	pDiveWeakEnemy->SetManagerObjectType(CObject::MANAGEROBJECTTYPE::DIVEWEAKENEMY);           //�}�l�[�W���[�ŌĂяo�����̎�ނ�ݒ�
	return pDiveWeakEnemy;
}
//============================================================================================================================================

//====================================================================================
//�e�L�X�g�t�@�C���ɏ���ۑ�����
//====================================================================================
void CDiveWeakEnemy::SaveInfoTxt(fstream& WritingFile)
{
	WritingFile << "SETDIVEWEAKENEMY" << endl;
	WritingFile << "DIVEWEAKENEMYTYPE = " << static_cast<int>(m_DiveWeakEnemyType);
	switch (m_DiveWeakEnemyType)
	{
	case DIVEWEAKENEMYTYPE::NORMAL:
		WritingFile << " # NORMAL" << endl;
		break;
	default:
		assert(false);
		break;
	}

	WritingFile << "DIVISIONNUM = " << m_nDivisionNum << endl;

	CEnemy::SaveInfoTxt(WritingFile);

	WritingFile << "END_SETDIVEWEAKENEMY" << endl;
}
//============================================================================================================================================

//====================================================================================
//�e�L�X�g�t�@�C���������ǂݍ���
//====================================================================================
void CDiveWeakEnemy::LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff)
{
	int nType = 0;                                     //���
	int nDiveWeakEnemyType = 0;                        //�_�C�u�Ɏア�G�^�C�v   
	int nLife = 0;                                     //�̗�
	int nPhaseNum = 0;                                 //�t�F�[�Y�ԍ�
	int nDivisionNum = 0;                              //�����
 	D3DXVECTOR3 Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  //�ړ���
	D3DXVECTOR3 Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   //�ʒu
	D3DXVECTOR3 Scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f); //�g�嗦
	D3DXVECTOR3 Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   //����
    DIVEWEAKENEMYTYPE DiveWeakEnemyType = {};          //�w�i���f���̎��
	ENEMYTYPE EnemyType = {};

	vector<CAIModel*> VecMoveAi = {};
	vector<MoveAiInfo> VecMoveAiInfo = {};

	int nCntMoveAi = 0;
	D3DXVECTOR3 MoveAiPos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	D3DXVECTOR3 MoveAiRot = D3DXVECTOR3(0.0f,0.0f,0.0f);
	D3DXVECTOR3 MoveAiScale = D3DXVECTOR3(0.0f,0.0f,0.0f);

	float fNormalSpeed = 0.0f; //�ʏ푬�x
	float fSensingRange = 0.0f;//���G����
	while (Buff != "END_SETDIVEWEAKENEMY")
	{
		LoadingFile >> Buff;//�P���ǂݍ���
		if (Buff == "#")
		{
			getline(LoadingFile, Buff);
		}
		else if (Buff == "DIVEWEAKENEMYTYPE")
		{
			LoadingFile >> Buff;//�C�R�[��
			LoadingFile >> nDiveWeakEnemyType;
		}
		else if (Buff == "ENEMYTYPE")
		{
			LoadingFile >> Buff;//�C�R�[��
			LoadingFile >> nType;
		}
		else if (Buff == "LIFE")
		{
			LoadingFile >> Buff;//�C�R�[��
			LoadingFile >> nLife;
		}
		else if (Buff == "POS")
		{
			LoadingFile >> Buff;//�C�R�[��
			LoadingFile >> Pos.x;      //�ʒuX
			LoadingFile >> Pos.y;      //�ʒuY
			LoadingFile >> Pos.z;      //�ʒuZ
		}
		else if (Buff == "ROT")
		{
			LoadingFile >> Buff;//�C�R�[��
			LoadingFile >> Rot.x;      //�ʒuX
			LoadingFile >> Rot.y;      //�ʒuY
			LoadingFile >> Rot.z;      //�ʒuZ
		}
		else if (Buff == "SCALE")
		{
			LoadingFile >> Buff;//�C�R�[��
			LoadingFile >> Scale.x;      //�g�嗦X
			LoadingFile >> Scale.y;      //�g�嗦Y
			LoadingFile >> Scale.z;      //�g�嗦Z
		}
		else if (Buff == "PHASENUM")
		{
			LoadingFile >> Buff;      //�C�R�[��
			LoadingFile >> nPhaseNum; //�t�F�[�Y�ԍ�
		}
		else if (Buff == "NORMALSPEED")
		{
			LoadingFile >> Buff;//�C�R�[��
			LoadingFile >> fNormalSpeed;//�ʏ푬�x
		}
		else if (Buff == "SENSINGRANGE")
		{
			LoadingFile >> Buff;//�C�R�[��
			LoadingFile >> fSensingRange;
		}
		else if (Buff == "DIVISIONNUM")
		{
			LoadingFile >> Buff;//�C�R�[��
			LoadingFile >> nDivisionNum;//�����
		}
		else if (Buff == "SETMOVEAI")
		{
			while (1)
			{
				LoadingFile >> Buff;
				if (Buff == "SETNUM")
				{
					LoadingFile >> Buff;//�C�R�[��
					LoadingFile >> nCntMoveAi;//�ԍ�
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
							{
								CAIModel* pAiModel = CAIModel::Create(CAIModel::AIMODELTYPE::MOVEPOINT, MoveAiPos, MoveAiRot, MoveAiScale, nullptr);
								pAiModel->GetDrawInfo().SetUseDraw(true);
								pAiModel->GetDrawInfo().SetUseShadow(true);
								VecMoveAi.push_back(pAiModel);
							}
							else if (CScene::GetMode() == CScene::MODE_GAME)
							{
								MoveAiInfo Info = {};
								Info.Pos = MoveAiPos;
								Info.Rot = MoveAiRot;
								Info.Scale = MoveAiScale;
								VecMoveAiInfo.push_back(Info);
							}
							break;
						}
					}
				}
				else if (Buff == "END_SETMOVEAI")
				{
					break;
				}
			}
		}
	}

	DiveWeakEnemyType = static_cast<DIVEWEAKENEMYTYPE>(nDiveWeakEnemyType);
	EnemyType = static_cast<ENEMYTYPE>(nType);
	if (CScene::GetMode() == CScene::MODE_EDIT)
	{
		CDiveWeakEnemy* pDiveWeakEnemy = CDiveWeakEnemy::Create(DiveWeakEnemyType, nLife, nPhaseNum, Pos, Rot, Scale,3);
		pDiveWeakEnemy->GetDrawInfo().SetUseDraw(true);
		pDiveWeakEnemy->GetDrawInfo().SetUseShadow(true);
		pDiveWeakEnemy->SetVecMoveAiInfo(VecMoveAi);
		pDiveWeakEnemy->SetNormalSpeed(fNormalSpeed);
		pDiveWeakEnemy->SetSensingRange(fSensingRange);
		pDiveWeakEnemy->GetMoveInfo().SetUseUpdatePos(true);
		listSaveManager.push_back(pDiveWeakEnemy);      //vector�ɏ���ۑ�����
	}
	else if (CScene::GetMode() == CScene::MODE_GAME)
	{
		CGame::GetPhaseManager()->PushPhaseInfo(Pos, Rot, Scale, nLife, static_cast<int>(EnemyType), nDiveWeakEnemyType, nPhaseNum,fNormalSpeed,fSensingRange,3,VecMoveAiInfo);
	}
}
//============================================================================================================================================

//====================================================================================
//�X�e�[�W�}�l�[�W���[�̃I�u�W�F�N�g���`�F���W����
//====================================================================================
CObject* CDiveWeakEnemy::ManagerChengeObject(bool bAim)
{
	int nNewType = int(m_DiveWeakEnemyType);
	DIVEWEAKENEMYTYPE NewType = {};
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
	SetUseDeath(true);
	SetDeath();
	//======================================================================================

	return CDiveWeakEnemy::Create(NewType, GetLifeInfo().GetLife(),GetPhaseNum(), GetPosInfo().GetPos(), GetRotInfo().GetRot(), GetSizeInfo().GetScale(),GetDivisionNum());//���������I�u�W�F�N�g��Ԃ�
}
//============================================================================================================================================

//====================================================================================
//�X�e�[�W�}�l�[�W���[�Ɍ��݂̃I�u�W�F�N�g��ۑ�����
//====================================================================================
CObject* CDiveWeakEnemy::ManagerSaveObject()
{
	auto& Vec = GetVecAiModelInfo();
	auto Vec2 = move(Vec);
	CDiveWeakEnemy * pDiveWeakEnemy = CDiveWeakEnemy::Create(m_DiveWeakEnemyType, GetLifeInfo().GetMaxLife(),GetPhaseNum(), GetPosInfo().GetPos(), GetRotInfo().GetRot(), GetSizeInfo().GetScale(),GetDivisionNum());//���������I�u�W�F�N�g��Ԃ�
	pDiveWeakEnemy->SetSensingRange(GetSensingRange());//���݂̓G�̍��G�͈͂�ۑ�����
	pDiveWeakEnemy->SetNormalSpeed(GetNormalSpeed());//���݂̓G�̒ʏ푬�x��ۑ�����
	pDiveWeakEnemy->SetVecMoveAiInfo(Vec2);
	return pDiveWeakEnemy;//���������I�u�W�F�N�g��Ԃ�
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
		ChengeMove(DBG_NEW CEnemyMove_None());//�U���������́A�ړ�������̂ŁA�ʏ�ړ������͂����Ȃ��B
		SetAction(true);//�U�����J�n
	}
	if (bAction == true)
	{
		//==============
		//�g�p�ϐ�
		//==============
		CObjectX::RotInfo& RotInfo = GetRotInfo();        //���������擾
		CObjectX::PosInfo& PosInfo = GetPosInfo();        //�ʒu�����擾
		CObjectX::SizeInfo& SizeInfo = GetSizeInfo();     //�T�C�Y�����擾
		CObjectX::MoveInfo& MoveInfo = GetMoveInfo();     //�ړ������擾
		CPlayer* pPlayer = CGame::GetPlayer();            //�v���C���[�̃|�C���^���擾
		CObjectX::PosInfo& PlayerPosInfo = pPlayer->GetPosInfo();//�v���C���[�̈ʒu�����擾
		const D3DXVECTOR3& Pos = PosInfo.GetPos();        //�ʒu���擾
		const D3DXVECTOR3& Move = MoveInfo.GetMove();     //�ړ��ʂ��擾
		const D3DXVECTOR3& PlayerPos = PlayerPosInfo.Pos;//�v���C���[�̈ʒu���擾
		D3DXVECTOR3 Rot = RotInfo.GetRot();               //����
		const float & fNormalSpeed = GetNormalSpeed();    //�ʏ�ړ����x
		float fRotAim = atan2f(PlayerPos.x - Pos.x, PlayerPos.z - Pos.z);//Z��������Ƀv���C���[�ւ̊p�x�i�ړI�̊p�x�j���v�Z����
		//========================================================================================================

		RotInfo.SetRot(D3DXVECTOR3(Rot.x, CCalculation::CalculationCollectionRot2D(Rot.y, fRotAim, 0.007f, false), Rot.z));//�������v���C���[�֒����������킹�Ă���
		MoveInfo.SetMove(D3DXVECTOR3(sinf(fRotAim) * fNormalSpeed, Move.y, cosf(fRotAim) * fNormalSpeed));//�v���C���[�Ɍ������Ĉړ�������

		if (GetCntTime() % s_nATTACK_FREQUENCY == 0)
		{//�U���𔭎�

		    //==============
		    //�g�p�ϐ�
		    //==============
			const D3DXVECTOR3& VtxMax = SizeInfo.GetVtxMax(); //�ő咸�_���擾
			D3DXVECTOR3 Aim = { 0.0f,0.0f,0.0f };             //�_������
			D3DXVECTOR3 ShotPos = Pos + D3DXVECTOR3(0.0f, VtxMax.y, 0.0f);//���ˈʒu
			D3DXVECTOR3 AddRot = { 0.0f,0.0f,0.0f };//���Z�������
			D3DXVECTOR2 YawPitch = CCalculation::VectorToYawPitch(ShotPos, PlayerPos);//�ړI�n�ւ̊p�x(Yaw��Pitch)���擾
			//========================================================================================================

			//Yaw�̕␳
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
		    
		    //Pitch�̕␳
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
		    	1, 60, 200, ShotPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), Aim, GetSizeInfo().GetScale() * 0.5f);
		    
		    pAttackEnemy->GetBoundInfo().SetActive(true, D3DXVECTOR3(0.0f, 10.0f, 0.0f), true, 0.5f);//�o�E���h������
		    pAttackEnemy->SetExtrusionCollisioin(true);//�����o��������s���A
		    pAttackEnemy->SetHitOtherThanLibing(false);//�G��v���C���[�ȊO�Ƃ̓����蔻��͍s��Ȃ�
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
	{
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
	{
		for (int nCnt = 0; nCnt < 60; nCnt++)
		{
			CParticle* pParticle = CParticle::Create(CParticle::TYPE::TYPE00_NORMAL, 120, 80.0f, 80.0f, GetPosInfo().GetPos(), CCalculation::Rand3DVec(200, 10),
				D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), true);
			pParticle->SetUseAddSpeed(true, true, 0.9f);
		}
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
	pEnemy->BattleMoveProcess();
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
	D3DXVECTOR3 ScreenPos = D3DXVECTOR3(0.0f, 0.0f,0.0f);
	ScreenPos = CCalculation::CalcWorldToScreenNoViewport(pEnemy->GetPosInfo().GetSenterPos(), *CManager::GetCamera()->GetMtxView(), *CManager::GetCamera()->GetMtxProjection(),
		SCREEN_WIDTH,SCREEN_HEIGHT);

	pEnemy->SetState(CEnemy::STATE::FRIGHTENDED);//������Ԃɂ���

	m_pLockOn = CUi::Create(CUi::UITYPE::TARGET_000, CObject2D::POLYGONTYPE::SENTERROLLING, 200.0f, 200.0f, 100, true, D3DXVECTOR3(ScreenPos.x, ScreenPos.y, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));

	pEnemy->SetAction(false);
	pEnemy->SetPossibleAttack(false);//�U����s�\�ɂ���

	m_pLockOn->SetPolygonRotSpeed(0.1f);
	m_pLockOn->SetUseAddScale(D3DXVECTOR2(-0.01f, -0.01f), true);
	m_pLockOn->SetUseDeath(false);
}
//============================================================================================================================================

//====================================================================================
//�f�X�g���N�^
//====================================================================================
CEnemyMove_Frightened::~CEnemyMove_Frightened()
{
	if (m_pLockOn != nullptr)
	{
		m_pLockOn->SetUseDeath(true);
		m_pLockOn->SetDeath();
		m_pLockOn = nullptr;
	}
}
//============================================================================================================================================

//====================================================================================
//����
//====================================================================================
void CEnemyMove_Frightened::Process(CEnemy* pEnemy)
{
	float fX = static_cast<float>(rand() % 50 - 25);
	float fY = static_cast<float>(rand() % 50 - 25);
	float fZ = static_cast<float>(rand() % 50 - 25);
	pEnemy->GetPosInfo().SetPos(m_StopPos + D3DXVECTOR3(fX, fY, fZ));//�k��������
	pEnemy->GetMoveInfo().SetMove(D3DXVECTOR3(0.0f,0.0f,0.0f));
	m_nStateTime--;
	pEnemy->EndAttackPattern();//������Ԃ̎��͍s���s�\�ɂ������̂ŁA�U���p�^�[�����I�������Â���

	if (m_pLockOn != nullptr)
	{
		D3DXVECTOR3 ScreenPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		ScreenPos = CCalculation::CalcWorldToScreenNoViewport(pEnemy->GetPosInfo().GetSenterPos(), *CManager::GetCamera()->GetMtxView(), *CManager::GetCamera()->GetMtxProjection(),
			SCREEN_WIDTH, SCREEN_HEIGHT);
		m_pLockOn->SetPos(ScreenPos);//������Ԃɂ����G�̃X�N���[�����W��UI��\��
		if (m_pLockOn->GetScale().x < 0.0f && m_pLockOn->GetScale().y < 0.0f)
		{
			m_pLockOn->SetUseDeath(true);
			m_pLockOn->SetDeath();
			m_pLockOn = nullptr;
		}
	}
	if (m_nStateTime < 1)
	{
		pEnemy->SetPossibleAttack(true);//�U�����\�ɖ߂�
		pEnemy->GetPosInfo().SetPos(m_StopPos);
		pEnemy->SetState(CEnemy::STATE::NORMAL);//��Ԉُ�𕁒ʂɖ߂�
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
	m_DodgeMove = DodgeMove;
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
	pEnemy->GetMoveInfo().SetMove(m_DodgeMove);

	if (pEnemy->GetCollisionWall() == false)
	{
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
	CEnemy::Init();//����������
	return S_OK;
}
//============================================================================================================================================

//====================================================================================
//�I������
//====================================================================================
void CIdleEnemy::Uninit()
{
	CEnemy::Uninit();//�I������
}
//============================================================================================================================================

//====================================================================================
//�X�V����
//====================================================================================
void CIdleEnemy::Update()
{
	CEnemy::Update();//�X�V����
}
//============================================================================================================================================

//====================================================================================
//�`�揈��
//====================================================================================
void CIdleEnemy::Draw()
{
	CEnemy::Draw();//�`�揈��
}
//============================================================================================================================================

//====================================================================================
//���S�t���O�ݒ菈��
//====================================================================================
void CIdleEnemy::SetDeath()
{
	CEnemy::SetDeath();//���S�t���O�ݒ菈��
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

	//���f������o�^�A�ǂݍ���
	int nIdx = CManager::GetObjectXInfo()->Regist(s_aIDLEENEMY_FILENAME[int(Type)]);
	pIdleEnemy->BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(nIdx),
		CManager::GetObjectXInfo()->GetBuffMat(nIdx),
		CManager::GetObjectXInfo()->GetdwNumMat(nIdx),
		CManager::GetObjectXInfo()->GetTexture(nIdx),
		CManager::GetObjectXInfo()->GetColorValue(nIdx));

	//�X�e�[�^�X��ݒ�
	pIdleEnemy->SetPhaseNum(nPhaseNum);               //�t�F�[�Y�ԍ���ݒ肷��
	pIdleEnemy->SetEnemyType(CEnemy::ENEMYTYPE::IDLE);//�G�̎�ނ�ݒ肷��
	pIdleEnemy->m_IdleEnemyType = Type;               //�������Ȃ��G�̃^�C�v��ݒ肷��
	pIdleEnemy->GetLifeInfo().SetLife(nLife);         //�̗�
	pIdleEnemy->GetLifeInfo().SetMaxLife(nLife);      //�ő�̗�
	pIdleEnemy->GetPosInfo().SetPos(pos);             //�ʒu
	pIdleEnemy->GetPosInfo().SetSupportPos(pos);      //�x�_�ʒu
	pIdleEnemy->GetRotInfo().SetRot(rot);             //����
	pIdleEnemy->GetSizeInfo().SetScale(Scale);        //�g�嗦
	pIdleEnemy->GetSizeInfo().SetFormarScale(Scale);  //���̊g�嗦��ݒ�
	pIdleEnemy->SetSensingRange(550.0f);              //���m�˒�
	pIdleEnemy->SetNormalSpeed(3.0f);                //�ʏ�ړ����x
	pIdleEnemy->GetMoveInfo().SetUseInteria(false, GetNormalInertia());//������ݒ�i�ʏ�l�j
	pIdleEnemy->SetCntTime(rand() % 100 + 1);         //�U���^�C�~���O�����炷
	pIdleEnemy->GetLifeInfo().SetAutoDeath(true);     //�̗͂��O�ɂȂ����Ƃ��Ɏ��S�t���O�𔭓�
	pIdleEnemy->GetLifeInfo().SetAutoSubLife(false);  //�̗͂����炵�����Ȃ�
	pIdleEnemy->SetSize();                            //���f���T�C�Y��ݒ�
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

//====================================================================================
//��񃍁[�h����
//===================================================================================
void CIdleEnemy::LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff)
{
	int nType = 0;                                           //���
	int nIdleEnemyType = 0;                                  //�������Ȃ��G�^�C�v   
	int nLife = 0;                                           //�̗�
	int nPhaseNum = 0;                                       //�t�F�[�Y�ԍ�
	D3DXVECTOR3 Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);        //�ړ���
	D3DXVECTOR3 Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);         //�ʒu
	D3DXVECTOR3 Scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);       //�g�嗦
	D3DXVECTOR3 Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);         //����
	IDLEENEMYTYPE IdleEnemyType = IDLEENEMYTYPE::NORMAL;     //�������Ȃ��G�̎��
	ENEMYTYPE EnemyType = {};                                //�G�̕���
														     
	vector<CAIModel*> VecMoveAi = {};                        //�ړ�AI�̓��I�z��
	vector<MoveAiInfo> VecMoveAiInfo = {};                   //�ړ�AI���̓��I�z��
														     
	int nCntMoveAi = 0;                                      //�ړ�AI�̐�
	D3DXVECTOR3 MoveAiPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   //�ړ�AI�̈ʒu
	D3DXVECTOR3 MoveAiRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   //�ړ�AI�̌���
	D3DXVECTOR3 MoveAiScale = D3DXVECTOR3(0.0f, 0.0f, 0.0f); //�ړ�AI�̊g�嗦

	float fNormalSpeed = 0.0f;                               //�ʏ푬�x
	float fSensingRange = 0.0f;                              //���G�����i�U�����[�h�ɓ��鋗��)
	while (Buff != "END_SETIDLEENEMY")
	{
		LoadingFile >> Buff;//�P���ǂݍ���
		if (Buff == "#")
		{
			getline(LoadingFile, Buff);
		}
		else if (Buff == "IDLEENEMYTYPE")
		{
			LoadingFile >> Buff;//�C�R�[��
			LoadingFile >> nIdleEnemyType;//�������Ȃ��G�^�C�v��ǂݍ���
		}
		else if (Buff == "ENEMYTYPE")
		{
			LoadingFile >> Buff;//�C�R�[��
			LoadingFile >> nType;//�G�̕��ނ�ǂݍ���
		}
		else if (Buff == "LIFE")
		{
			LoadingFile >> Buff;//�C�R�[��
			LoadingFile >> nLife;//�̗͂�ǂݍ���
		}
		else if (Buff == "POS")
		{
			LoadingFile >> Buff;//�C�R�[��
			LoadingFile >> Pos.x;      //�ʒuX
			LoadingFile >> Pos.y;      //�ʒuY
			LoadingFile >> Pos.z;      //�ʒuZ
		}
		else if (Buff == "ROT")
		{
			LoadingFile >> Buff;//�C�R�[��
			LoadingFile >> Rot.x;      //����X
			LoadingFile >> Rot.y;      //����Y
			LoadingFile >> Rot.z;      //����Z
		}
		else if (Buff == "SCALE")
		{
			LoadingFile >> Buff;//�C�R�[��
			LoadingFile >> Scale.x;      //�g�嗦X
			LoadingFile >> Scale.y;      //�g�嗦Y
			LoadingFile >> Scale.z;      //�g�嗦Z
		}
		else if (Buff == "PHASENUM")
		{
			LoadingFile >> Buff;      //�C�R�[��
			LoadingFile >> nPhaseNum; //�t�F�[�Y�ԍ�
		}
		else if (Buff == "NORMALSPEED")
		{
			LoadingFile >> Buff;//�C�R�[��
			LoadingFile >> fNormalSpeed;//�ʏ푬�x
		}
		else if (Buff == "SENSINGRANGE")
		{
			LoadingFile >> Buff;//�C�R�[��
			LoadingFile >> fSensingRange;//���G�͈͂�ǂݍ���
		}
		else if (Buff == "SETMOVEAI")
		{//�ړ�AI�̏���ǂݍ���
			while (1)
			{
				LoadingFile >> Buff;
				if (Buff == "SETNUM")
				{
					LoadingFile >> Buff;//�C�R�[��
					LoadingFile >> nCntMoveAi;//�ԍ�
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
							LoadingFile >> MoveAiRot.x;      //����X
							LoadingFile >> MoveAiRot.y;      //����Y
							LoadingFile >> MoveAiRot.z;      //����Z
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
							{//�G�f�B�b�g���[�h�̏ꍇ�i�G�f�B�b�g���[�h�ł͍ŏ�����S�Ă̓G�ƈړ�AI��\���������̂ŁA��������j
								CAIModel* pAiModel = CAIModel::Create(CAIModel::AIMODELTYPE::MOVEPOINT, MoveAiPos, MoveAiRot, MoveAiScale, nullptr);//����
								pAiModel->GetDrawInfo().SetUseDraw(true);//�`�������
								pAiModel->GetDrawInfo().SetUseShadow(true);//�e��`�悷��
								VecMoveAi.push_back(pAiModel);
							}
							else if (CScene::GetMode() == CScene::MODE_GAME)
							{//�Q�[�����[�h�̏ꍇ�i�Q�[�����[�h�̓t�F�[�Y���Ői�ނ̂ŁA�ŏ�����ړ�AI�𐶐��������̂ŁA�t�F�[�Y���Ɋi�[����)
								MoveAiInfo Info = {};//�ړ�AI�̏���������
								Info.Pos = MoveAiPos;//�ړ�AI�̈ʒu
								Info.Rot = MoveAiRot;//�ړ�AI�̌���
								Info.Scale = MoveAiScale;//�ړ�AI�̊g�嗦
								VecMoveAiInfo.push_back(Info);//�ړ�AI���̓��I�z��Ɋi�[
							}
							break;
						}
					}
				}
				else if (Buff == "END_SETMOVEAI")
				{//�ړ�AI�̓ǂݍ��݂��I������
					break;
				}
			}
		}
	}

	IdleEnemyType = static_cast<IDLEENEMYTYPE>(nIdleEnemyType);//int�^�œǂݍ��񂾉������Ȃ��G�̃^�C�v�ԍ���񋓌^�ɃL���X�g���Ċi�[
	EnemyType = static_cast<ENEMYTYPE>(nType);                 //�G�̕��ނ�񋓌^�ɃL���X�g

	if (CScene::GetMode() == CScene::MODE_EDIT)
	{//�G�f�B�b�g���[�h�̏ꍇ�i�t�F�[�Y���͓K�p���Ȃ��̂ŁA�ŏ�����S�Ă̏��𐶐��j
		CIdleEnemy* pIdleEnemy = CIdleEnemy::Create(IdleEnemyType, nLife, nPhaseNum, Pos, Rot, Scale);//����
		pIdleEnemy->GetDrawInfo().SetUseDraw(true);        //�`�悷��
		pIdleEnemy->GetDrawInfo().SetUseShadow(true);      //�e��`�悷��
		pIdleEnemy->SetVecMoveAiInfo(VecMoveAi);           //�ړ�AI����ݒ�
		pIdleEnemy->SetNormalSpeed(3.0f);          //�ʏ푬�x��ݒ�
		pIdleEnemy->SetSensingRange(fSensingRange);        //���G�͈͂�ݒ�
		pIdleEnemy->GetMoveInfo().SetUseUpdatePos(true);   //�ʒu�̍X�V���s��
		listSaveManager.push_back(pIdleEnemy);             //vector�ɏ���ۑ�����
	}
	else if (CScene::GetMode() == CScene::MODE_GAME)
	{//�Q�[�����[�h�Ȃ�i�t�F�[�Y���ŃQ�[���������̂ŁA�t�F�[�Y���ɓG�̏����i�[�j
		CGame::GetPhaseManager()->PushPhaseInfo(Pos, Rot, Scale, nLife, static_cast<int>(EnemyType),nIdleEnemyType, nPhaseNum,3.0f, fSensingRange, 3, VecMoveAiInfo);
	}
}
//============================================================================================================================================

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
	SetUseDeath(true);
	SetDeath();
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
	{
		CEnemy::AIMoveProcess();
	}
}
//============================================================================================================================================

//====================================================================================
//���j���o
//===================================================================================
void CIdleEnemy::DefeatStaging()
{
	if (GetLifeInfo().GetLife() < 1 && GetUseDeath() == true)
	{
		for (int nCnt = 0; nCnt < 60; nCnt++)
		{
			CParticle* pParticle = CParticle::Create(CParticle::TYPE::TYPE00_NORMAL, 120, 80.0f, 80.0f, GetPosInfo().GetPos(), CCalculation::Rand3DVec(200, 10),
				D3DXCOLOR(0.678f, 1.0f, 0.184f, 1.0f), true);
			pParticle->SetUseAddSpeed(true, true, 0.9f);
		}
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
	CObjectX::PosInfo& PosInfo = pEnemy->GetPosInfo();
	const D3DXVECTOR3& Pos = PosInfo.GetPos();
	const D3DXVECTOR3& SenterPos = PosInfo.GetSenterPos();
	m_nCntTime++;

	CParticle::SummonChargeParticle(CParticle::TYPE::TYPE00_NORMAL, 1, 60, 5.0f, 40.0f, 40.0f, 200.0f, 100, 10, true, SenterPos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), true);

	if (m_nCntTime == 100)
	{
		CObjectX::MoveInfo& MoveInfo = pEnemy->GetMoveInfo();

		const D3DXVECTOR3& Move = MoveInfo.GetMove();

		MoveInfo.SetMove(D3DXVECTOR3(Move.x,CCalculation::GetInitialVelocityHeight(m_fGoalHeight,pEnemy->GetMoveInfo().GetGravity()) + 5.0f,Move.z));
		pEnemy->ResetJumpCoolTime();//���ɃW�����v�U���𔭓�����܂ł̃N�[���^�C�������Z�b�g����
		pEnemy->ChengeMove(DBG_NEW CEnemyMove_Battle());
	}
}
//============================================================================================================================================