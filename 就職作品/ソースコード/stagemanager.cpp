//==========================================================
//
//�V���P�V���F�I�u�W�F�N�g�̃��X�g�Ǘ�������[stagemanager.cpp]
//Author:ShinaTaiyo
//
//==========================================================

//================================
//�C���N���[�h
//================================
#include "stagemanager.h"
#include "objectX.h"
#include "objectXInfo.h"
#include "block.h"
#include "input.h"
#include "manager.h"
#include "object.h"
#include "camera.h"
#include "calculation.h"
#include "bg3d.h"
#include "debugtext.h"
#include "bgModel.h"
#include "enemy.h"
#include <stdio.h>
#include <string.h>
//==========================================================

//================================
//�ÓI�����o�錾
//================================
const string CStageManager::m_aWORLDMAP_TXT[static_cast<int>(CStageManager::WORLDTYPE::MAX)] =
{
	"data\\TEXTFILE\\Ver2\\Stage01.txt",
	"data\\TEXTFILE\\Ver2\\Stage02.txt",
	"data\\TEXTFILE\\Ver2\\Stage03.txt",
};

const string CStageManager::m_aSAVE_FILENAME = "data\\TEXTFILE\\Ver2\\Stage03.txt";

//================================
//�R���X�g���N�^
//================================
CStageManager::CStageManager(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CObject(nPri,bUseintPri,type,ObjType),
m_nWorldIndex(0),m_pBg3D(nullptr), m_StgObjList(),m_SaveScale(D3DXVECTOR3(1.0f,1.0f,1.0f)),m_SavePos(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_SaveRot(D3DXVECTOR3(0.0f,0.0f,0.0f)),
m_SaveBeforeChoosePos(D3DXVECTOR3(0.0f,0.0f,0.0f)), m_nMapIndex(0), m_nMapNum(0), m_pChooseObject(nullptr),
m_ManagerMode(MANAGERMODE::ALREADYSTAGE),m_bChooseObject(false),m_bMakeMapMode(false),m_bUseSizeMove(false),
m_pState(nullptr),m_NowState(STATE::NEWOBJECT),m_SpawnPoint(D3DXVECTOR3(0.0f,0.0f,0.0f))
{

}
//==========================================================

//================================
//�f�X�g���N�^
//================================
CStageManager::~CStageManager()
{
	m_StgObjList.clear();//vector�̒��g���N���A�i�Y�ꂽ�Ƃ��΍�j
}
//==========================================================

//================================
//����������
//================================
HRESULT CStageManager::Init()
{
	//===========================
	//�����o�ϐ�
	//===========================
	m_nMapIndex = 0;                                     //�}�b�v�̃C���f�b�N�X
	m_nMapNum = 0;                                       //�}�b�v�̑���

	//=========================
	//�I���n
	//=========================
	m_pChooseObject = nullptr;                          //�I���I�u�W�F�N�g�ւ�
	//=======================================================================================
#ifdef _DEBUG
#endif // _DEBUG

	//===========================
	//�ړ����[�h�����߂�
	//===========================
	m_bUseSizeMove = false;//���݂̃I�u�W�F�N�g�̃T�C�Y���ړ����邩�ǂ���
	//=======================================================================================

	m_pState = DBG_NEW CStageManagerState_NewObject();//�V�����I�u�W�F�N�g�ҏW���[�h�ɂ���

	CObject::Init();                                  //�I�u�W�F�N�g����������

	SetUseDeath(false);                               //���S�t���O���g�p���Ȃ�
	if (CScene::GetMode() == CScene::MODE_EDIT)
	{//�G�f�B�b�g���[�h�Ȃ�
		LoadMapTxt(0);                                //�}�b�v�O��ǂݍ���
	}
	return S_OK;
}
//==========================================================

//================================
//�I������
//================================
void CStageManager::Uninit()
{
	CObject::Uninit();//�I�u�W�F�N�g�I������
}
//==========================================================

//================================
//�X�V����
//================================
void CStageManager::Update()
{

#ifdef _DEBUG

	if (CScene::GetMode() == CScene::MODE_EDIT)
	{//�G�f�B�b�g�V�[���Ȃ�

		MapChenge();  //�}�b�v��ς��鏈��

		ChengeState();//�X�e�[�g�ύX

		if (m_pState != nullptr)
		{//�X�e�[�g����
			m_pState->Process(this);
		}
		//===========================================================================================

		//===============================================
		//���\������
		//===============================================
		DispInfo();
		//===========================================================================================

		//===============================================
		//�I�u�W�F�N�g�X�V
		//===============================================
		CObject::Update();
		//===========================================================================================
	}

#endif // _DEBUG
}
//==========================================================

//================================
//�`�揈��
//================================
void CStageManager::Draw()
{
#ifdef _DEBUG

	if (CScene::GetMode() == CScene::MODE_EDIT)
	{//�G�f�B�b�g���[�h�Ȃ�
	    CObject::Draw();//�I�u�W�F�N�g�`�揈��
	}
#endif // _DEBUG

}
//============================================================================================================

//======================================================
//���S�t���O��ݒ�
//======================================================
void CStageManager::SetDeath()
{
	if (GetUseDeath() == true)
	{
		m_StgObjList.clear();//vector�̒��g���N���A

		if (m_pState != nullptr)
		{//�X�e�[�g����j��
			delete m_pState;
			m_pState = nullptr;
		}

		if (m_pBg3D != nullptr)
		{//�w�i��j��
			m_pBg3D->SetUseDeath(true);//���S�t���O���g�p��r��
			m_pBg3D->SetDeath();       //���S�t���O��ݒ肷��
			m_pBg3D = nullptr;         //�w�i�ւ̃|�C���^��������
		}
	}

	CObject::SetDeath();//�I�u�W�F�N�g�̎��S�t���O�ݒ菈��
}
//============================================================================================================

//============================================
//�}�b�v�����[�h���鏈��
//============================================
void CStageManager::LoadMapTxt(int nMapNum)
{
	ReleaseAll();//�S�ẴI�u�W�F�N�g�̎��S�t���O�𔭓�

	//vector�ɕۑ������������Z�b�g����
	m_StgObjList.clear();

	fstream ReadingFile;//�ǂݎ��p�t�@�C��
	string Reading_Buff;//�ǂݎ��p�t�@�C���̕�����

	//�}�b�v�ԍ��z��O�΍�
	if (nMapNum < 0)
	{
		nMapNum = static_cast<int>(WORLDTYPE::MAX) - 1;
	}
	if (nMapNum >= static_cast<int>(WORLDTYPE::MAX))
	{
		nMapNum = 0;
	}

	ReadingFile.open(m_aWORLDMAP_TXT[nMapNum], ios::in);//�ǂݎ�胂�[�h�ŊJ��

	while (!ReadingFile.eof())
	{
		CObject* pObj = nullptr;//�I�u�W�F�N�g�ւ̃|�C���^��������
		ReadingFile >> Reading_Buff;

		if (Reading_Buff == "PLAYER_SPAWNPOINT")
		{//�v���C���[�̃X�|�[���ʒu
			ReadingFile >> Reading_Buff;//�C�R�[��
			ReadingFile >> m_SpawnPoint.x;
			ReadingFile >> m_SpawnPoint.y;
			ReadingFile >> m_SpawnPoint.z;
		}
		else if (Reading_Buff == "SETBLOCK")
		{//�u���b�N
			pObj = CBlock::Create(CBlock::BLOCKTYPE::NORMAL, 1, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), false);//�u���b�N�𐶐�
		}
		else if (Reading_Buff == "SETBGMODEL")
		{//�w�i���f��
			pObj = CBgModel::Create(CBgModel::BGMODELTYPE::BILL_00, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), false);//�w�i���f���𐶐�
		}
		else if (Reading_Buff == "SETSHOTWEAKENEMY")
		{//�ˌ��Ɏア�G
			pObj = CShotWeakEnemy::Create(CShotWeakEnemy::SHOTWEAKENEMYTYPE::NORMAL, 1, 0, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));//�G�𐶐�
		}
		else if (Reading_Buff == "SETDIVEWEAKENEMY")
		{//�_�C�u�Ɏア�G
			pObj = CDiveWeakEnemy::Create(CDiveWeakEnemy::DIVEWEAKENEMYTYPE::NORMAL, 1, 0, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);//�_�C�u�Ɏア�G�𐶐�
		}
		else if (Reading_Buff == "SETIDLEENEMY")
		{//�������Ȃ��G
			pObj = CIdleEnemy::Create(CIdleEnemy::IDLEENEMYTYPE::NORMAL, 1, 0, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));;//�������Ȃ��G�𐶐�
		}

		if (pObj != nullptr)
		{//�I�u�W�F�N�g�ւ̃|�C���^�����݂��Ă�����
			pObj->LoadInfoTxt(ReadingFile, m_StgObjList, Reading_Buff, pObj);//�e�L�X�g�t�@�C���������ǂݍ���
		}
	}

	if (CScene::GetMode() == CScene::MODE_GAME)
	{//�Q�[���V�[���Ȃ�
		m_StgObjList.clear();//�X�e�[�W�I�u�W�F�N�g���X�g�ɕۑ�����K�v���Ȃ��̂ŃN���A����
	}

	ReadingFile.close();//�t�@�C�������
}
//============================================================================================================

//============================================
//�}�b�v���Z�[�u���鏈��
//============================================
void CStageManager::SaveMapTxt(int nMapNum)
{
	fstream WritingFile;                                  //�t�@�C��
	string Writing_Buff;                                  //������

	WritingFile.open(m_aWORLDMAP_TXT[nMapNum], ios::out); //�����o�����[�h�Ńt�@�C�����J��	

	//�v���C���[�̃X�|�[���|�C���g��ݒ肷��i�����_��R�ʂ܂Łj
	WritingFile << "PLAYER_SPAWNPOINT = " << fixed << setprecision(3) <<m_SpawnPoint.x << " " <<
		fixed << setprecision(3) << m_SpawnPoint.y << " " <<
		fixed << setprecision(3) << m_SpawnPoint.z << " " << endl << endl;
	
	//�t�@�C���ɏ���ۑ�����
	for (list<CObject*>::iterator it = m_StgObjList.begin(); it != m_StgObjList.end();++it)
	{//�����܂ŌJ��Ԃ�
		if (&it != nullptr)
		{
			(*it)->SaveInfoTxt(WritingFile);//���������o��

			WritingFile << endl << endl;    //���s����
		}
	}

	WritingFile.close();                    //�t�@�C�������
						                    
	m_StgObjList.clear();                   //Vector�̒��g���N���A����
	ReleaseAll();                           //�S�Ă̎��S�t���O�𔭓�
}
//======================================================================================================================

//==========================================================
//�}�b�v��ύX����
//==========================================================
void CStageManager::MapChenge()
{
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard();//�L�[���͏��ւ̃|�C���^
	if (pInputKeyboard->GetTrigger(DIK_F3) == true && m_ManagerMode == MANAGERMODE::ALREADYSTAGE)
	{//F3�L�[�������Ă��āA�����̃X�e�[�W�ҏW���[�h��������
		SaveMapTxt(m_nMapIndex);//���݂̃}�b�v�����Z�[�u����
		m_nMapIndex++;
		if (m_nMapIndex >= static_cast<int>(WORLDTYPE::MAX))
		{//�z��O�΍�
			m_nMapIndex = 0;
		}

		LoadMapTxt(m_nMapIndex);//�ԍ��̃}�b�v��ǂݍ���
	}
	else if (pInputKeyboard->GetTrigger(DIK_F2) == true && m_ManagerMode == MANAGERMODE::ALREADYSTAGE)
	{//F2�L�[�������Ă��āA�����̃X�e�[�W�ҏW���[�h��������
		SaveMapTxt(m_nMapIndex);//���݂̃}�b�v�����Z�[�u����
		m_nMapIndex--;
		if (m_nMapIndex < 0)
		{//�z��O�΍�
			m_nMapIndex = static_cast<int>(WORLDTYPE::MAX) - 1;
		}
		LoadMapTxt(m_nMapIndex);//�ԍ��̃}�b�v��ǂݍ���
	}
}
//======================================================================================================================

//==============================================
//�X�e�[�W�}�l�[�W���[��������
//==============================================
CStageManager* CStageManager::Create()
{
	CStageManager* pStageManager = DBG_NEW CStageManager;        //�X�e�[�W�}�l�[�W���[�𐶐�
	pStageManager->Init();                                       //����������
	pStageManager->SetUseDeath(false);                           //���S�t���O���g�p���Ȃ�
	pStageManager->CObject::SetType(CObject::TYPE::STAGEMANAGER);//�I�u�W�F�N�g�̎�ނ����߂�   
	return pStageManager;
}
//================================================================================

//===========================================================
//���\������
//===========================================================
void CStageManager::DispInfo()
{
	char aChooseString[32] = {};//�I����Ԃ��ǂ������L�q
	char aCameraTypeName[CCamera::CAMERATYPE_MAX][100] = {};//�}�b�v�t�@�C�������i�[�p
	char aMapModeString[32] = {};                           //�}�b�v�ҏW���[�h�����i�[�p

	//�}�b�v�̃J�������[�h��
	strcpy(&aCameraTypeName[CCamera::CAMERATYPE_BIRD][0], "�o�[�h�r���[");
	strcpy(&aCameraTypeName[CCamera::CAMERATYPE_BESIDE][0], "�r�T�C�h�r���[");
	strcpy(&aCameraTypeName[CCamera::CAMERATYPE_SIDE][0], "�T�C�h�r���[");


	if (m_bChooseObject == false)
	{
		strcpy(&aChooseString[0], "�I�����Ă��Ȃ�");
	}
	else
	{
		strcpy(&aChooseString[0], "�I�𒆁E�E�E");
	}

	if (m_ManagerMode == MANAGERMODE::ALREADYSTAGE)
	{
		strcpy(&aMapModeString[0], "�����̃X�e�[�W��ҏW");
	}
	else
	{
		strcpy(&aMapModeString[0], "�V�����X�e�[�W��ҏW");
	}

	CManager::GetDebugText()->PrintDebugText("//=================================\n");
	CManager::GetDebugText()->PrintDebugText("//�}�b�v�G�f�B�^�̏��\n");
	CManager::GetDebugText()->PrintDebugText("//=================================\n");
	CManager::GetDebugText()->PrintDebugText("���݂̃X�e�[�W�}�l�[�W���[�Ǘ��I�u�W�F�N�g�̐��F%d\n", m_StgObjList.size());
	CManager::GetDebugText()->PrintDebugText("���݂̃}�b�v�ԍ�(F2�AF3�ŕύX�j�F%d\n", m_nMapIndex);
	CManager::GetDebugText()->PrintDebugText("���݂̃}�b�v���[�h�iF4�j�F%s\n",&aMapModeString[0]);
	CManager::GetDebugText()->PrintDebugText("�X�|�[���|�C���g�iP�j�F%f %f %f\n",m_SpawnPoint.x,m_SpawnPoint.y,m_SpawnPoint.z);
	CManager::GetDebugText()->PrintDebugText("�X�e�[�g�ύX : 8\n");
	CManager::GetDebugText()->PrintDebugText("�F�����̃I�u�W�F�N�g�������FBACKSPACE\n");
	CManager::GetDebugText()->PrintDebugText("//=================================\n");

}
//=======================================================================================================================

//===========================================================
//��Ԃ�ς���
//===========================================================
void CStageManager::ChengeState()
{
	bool bStart = false;                            //�X�e�[�g�ύX�J�n�t���O
	int nNowState = static_cast<int>(m_NowState);   //���݂̃X�e�[�g����
	auto input = CManager::GetInputKeyboard();      //�L�[���͏��ւ̃|�C���^

	if (input->GetTrigger(DIK_8))
	{//8�L�[�������Ă�����
		nNowState += (input->GetPress(DIK_LSHIFT) ? -1 : 1);//if ? true : false�i���� ? True�Ȃ� : False�Ȃ�j
		bStart = true;
	}

	if (bStart)
	{
		//�j��
		if (m_pState != nullptr)
		{
			delete m_pState;
			m_pState = nullptr;
		}

		// �͈͐���
		nNowState = (nNowState + static_cast<int>(STATE::MAX)) % static_cast<int>(STATE::MAX);
		m_NowState = static_cast<STATE>(nNowState);

		// �X�e�[�g����
		switch (m_NowState)
		{
		case CStageManager::STATE::NEWOBJECT://�V�����I�u�W�F�N�g
			m_pState = DBG_NEW CStageManagerState_NewObject();
			break;
		case CStageManager::STATE::PLACEDOBJECT://�����̃I�u�W�F�N�g
			m_pState = DBG_NEW CStageManagerState_PlacedObject();
			break;
		default:
			assert("�z��O�̒l�������Ă���");
			break;
		}
	}
}
//=======================================================================================================================

//===========================================================
//�g�嗦�����Z�b�g���鏈��
//===========================================================
void CStageManager::ResetScale()
{
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_V) == true)
	{
		m_SaveScale = D3DXVECTOR3(1.0f,1.0f,1.0f);
	}
}
//=======================================================================================================================

//***********************************************************************************************************************
//�V�����I�u�W�F�N�g�z�u��ԃN���X
//***********************************************************************************************************************

//===========================================================
//�R���X�g���N�^
//===========================================================
CStageManagerState_NewObject::CStageManagerState_NewObject() : m_pManagerObject(nullptr)
{
	//�u���b�N�𐶐�
	m_pManagerObject = CBlock::Create(CBlock::BLOCKTYPE::NORMAL,10, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f),false);
	m_pManagerObject->SetUseDeath(false);//���S�t���O���g�p���Ȃ�
}
//=======================================================================================================================

//===========================================================
//�f�X�g���N�^
//===========================================================
CStageManagerState_NewObject::~CStageManagerState_NewObject()
{
	if (m_pManagerObject != nullptr)
	{//���݂��Ă�����
		m_pManagerObject->SetUseDeath(true);//���S�t���O���g�p����
		m_pManagerObject->SetDeath();       //���S�t���O��ݒ肷��
		m_pManagerObject = nullptr;         //�|�C���^��������
	}
}
//=======================================================================================================================

//===========================================================
//����
//===========================================================
void CStageManagerState_NewObject::Process(CStageManager* pStageManager)
{
	//�X�e�[�W�}�l�[�W���[������ɃA�N�Z�X���đ��삷��
	if (m_pManagerObject != nullptr)
	{
		m_pManagerObject->ManagerChooseControlInfo();//�I�u�W�F�N�g�N���X�ɏ���ҏW���邽�߂̊֐����e�h���N���X��override����Ă���̂ŕҏW���邱�Ƃ��ł���

		if (m_pManagerObject->GetObjectType() == CObject::OBJECTTYPE::OBJECTTYPE_X)
		{//�I�u�W�F�N�g�^�C�v���u�I�u�W�F�N�gX�v��������
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_P) == true)
			{//P�L�[�������Ă�����
				//�v���C���[�̃X�|�[���|�C���g��ݒ�
				pStageManager->SetSpawnPoint(static_cast<CObjectX*>(m_pManagerObject)->GetPosInfo().GetPos());
			}
		}

		//�����_�����ݕҏW���Ă���I�u�W�F�N�g�ɌŒ�
		CManager::GetCamera()->SetPosR(static_cast<CObjectX*>(m_pManagerObject)->GetPosInfo().GetPos());

		//���݂̃u���b�N�̎�ނ�ύX����
		TypeChenge(pStageManager);

		//�u���b�N��ݒ肷��
		SetObjectX(pStageManager);

		//�I�u�W�F�N�g������
		DeleteManagerObject(pStageManager);
	}
}
//=======================================================================================================================

//===========================================================
//�I�u�W�F�N�g�̎�ނ�ς���
//===========================================================
void CStageManagerState_NewObject::TypeChenge(CStageManager* pStageManager)
{
	CObject* pObj = nullptr;//�I�u�W�F�N�g�ւ̃|�C���^
	//=====================================================================
	//�I�u�W�F�N�g���Ƃ̎�ނ�ύX����
	//=====================================================================
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_0) == true)
	{
		pObj = m_pManagerObject->ManagerChengeObject(true);
		m_pManagerObject = pObj;
	}
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_9) == true)
	{
		pObj = m_pManagerObject->ManagerChengeObject(false);
		m_pManagerObject = pObj;
	}
	//=====================================================================================================================================

	//�I�u�W�F�N�g���Ƃ̃^�C�v��ς���
	ChengeObject(pStageManager);
}
//=======================================================================================================================

//===========================================================
//�I�u�W�F�N�g��ݒ肷��
//===========================================================
void CStageManagerState_NewObject::SetObjectX(CStageManager* pStageManager)
{
	list <CObject*>& StgObjList = pStageManager->GetStgObjList();//�X�e�[�W�I�u�W�F�N�g���X�g���擾
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) == true)
	{//�I�u�W�F�N�g�����X�g�ɕۑ�����
		StgObjList.push_back(m_pManagerObject->ManagerSaveObject());//�I�u�W�F�N�g�N���X��ManagerSaveObject������A�e�h���N���X��override���Ă���̂Ŕh���N���X���Ƃ̏���ۑ��ł���
	}
}
//=======================================================================================================================

//===========================================================
//�����̃I�u�W�F�N�g����������
//===========================================================
void CStageManagerState_NewObject::DeleteManagerObject(CStageManager* pStageManager)
{
	list <CObject*>& StgObjList = pStageManager->GetStgObjList();//�X�e�[�W�I�u�W�F�N�g���X�g���擾

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_BACKSPACE) == true)
	{//BACKSPACE�L�[�������Ă����疖���̗v�f���폜
		if (StgObjList.size() > 0)
		{
			auto it = prev(StgObjList.end());//�����̗v�f�̃C�e���[�^���擾

			(*it)->SetUseDeath(true);       //���S�t���O���g�p����
			(*it)->SetDeath();              //���S�t���O��ݒ肷��

			StgObjList.pop_back();          //�����̗v�f���폜����
		}
	}
}
//=======================================================================================================================

//===========================================================
//�I�u�W�F�N�g�̕ύX���s��
//===========================================================
void CStageManagerState_NewObject::ChengeObject(CStageManager* pStageManager)
{
	bool bChengeFlag = false;
	int nNumType = static_cast<int>(m_pManagerObject->GetManagerObjectType());      //�X�e�[�W�}�l�[�W���[�ҏW�I�u�W�F�N�g�^�C�v���擾(++�A--���g�������̂�int�ɃL���X�g)
	CObject::MANAGEROBJECTTYPE ManagerObjectType = CObject::MANAGEROBJECTTYPE::NONE;//�X�e�[�W�}�l�[�W���[�ҏW�I�u�W�F�N�g�^�C�v�i�[�p

	//=====================================================================
	//�I�u�W�F�N�gX�̎�ނ�ύX����
	//=====================================================================
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_2) == true)
	{//2�L�[�������Ă�����
		nNumType++;        //�^�C�v�ԍ���ς���
		bChengeFlag = true;//����
	}
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_1) == true)
	{//1�L�[�������Ă�����
		nNumType--;        //�^�C�v�ԍ���ς���
		bChengeFlag = true;//����
	}

	if (bChengeFlag == true)
	{//�^�C�v��ς���t���O���I���ɂȂ�����
		ReleaseObject(pStageManager);

		//�z��O�`�F�b�N
		if (nNumType >= static_cast<int>(CObject::MANAGEROBJECTTYPE::MAX))
		{
			nNumType = static_cast<int>(CObject::MANAGEROBJECTTYPE::NONE) + 1;
		}
		if (nNumType < 1)
		{
			nNumType = static_cast<int>(CObject::MANAGEROBJECTTYPE::MAX) - 1;
		}

		ManagerObjectType = CObject::MANAGEROBJECTTYPE(nNumType);//�V�����^�C�v�ɐݒ�
	}
	//=====================================================================================================================================

	//�I�u�W�F�N�g�̎�ނ�ς���
	switch (ManagerObjectType)
	{
	case  CObject::MANAGEROBJECTTYPE::BLOCK://�u���b�N
		m_pManagerObject = CBlock::Create(CBlock::BLOCKTYPE::NORMAL, 10, pStageManager->GetSavePos(),pStageManager->GetSaveRot(),pStageManager->GetSaveScale(),false);
		break;
	case CObject::MANAGEROBJECTTYPE::BGMODEL://�w�i���f��
		m_pManagerObject = CBgModel::Create(CBgModel::BGMODELTYPE::BILL_00, pStageManager->GetSavePos(), pStageManager->GetSaveRot(), pStageManager->GetSaveScale(),false);
		break;
	case CObject::MANAGEROBJECTTYPE::SHOTWEAKENEMY://�ˌ��Ɏア�G
		m_pManagerObject = CShotWeakEnemy::Create(CShotWeakEnemy::SHOTWEAKENEMYTYPE::NORMAL, 10, 0, pStageManager->GetSavePos(), pStageManager->GetSaveRot(), pStageManager->GetSaveScale());
		break;
	case CObject::MANAGEROBJECTTYPE::DIVEWEAKENEMY://�_�C�u�Ɏア�G
		m_pManagerObject = CDiveWeakEnemy::Create(CDiveWeakEnemy::DIVEWEAKENEMYTYPE::NORMAL, 10, 0, pStageManager->GetSavePos(), pStageManager->GetSaveRot(), pStageManager->GetSaveScale(),0);
		break;
	case CObject::MANAGEROBJECTTYPE::IDLEENEMY://�������Ȃ��G
		m_pManagerObject = CIdleEnemy::Create(CIdleEnemy::IDLEENEMYTYPE::NORMAL, 10, 0, pStageManager->GetSavePos(), pStageManager->GetSaveRot(), pStageManager->GetSaveScale());
		break;
	default:
		break;
	}

	if (m_pManagerObject != nullptr)
	{//���I��ł���I�u�W�F�N�g�����͔j������Ȃ�
		m_pManagerObject->SetUseDeath(false);
	}
}
//=======================================================================================================================

//===========================================================
//�����̃I�u�W�F�N�g�̔j�����s��
//===========================================================
void CStageManagerState_NewObject::ReleaseObject(CStageManager* pStageManager)
{
	//�Œ���̏���ۑ�����
	switch (m_pManagerObject->GetObjectType())
	{
	case CObject::OBJECTTYPE::OBJECTTYPE_X://�I�u�W�F�N�g�^�C�v���u�I�u�W�F�N�gX�v��������
		pStageManager->SetSavePos(static_cast<CObjectX*>(m_pManagerObject)->GetPosInfo().GetPos());
		pStageManager->SetSaveRot(static_cast<CObjectX*>(m_pManagerObject)->GetRotInfo().GetRot());
		pStageManager->SetSaveScale(static_cast<CObjectX*>(m_pManagerObject)->GetSizeInfo().GetScale());
		break;
	default:
		break;
	}
	if (m_pManagerObject != nullptr)
	{
		m_pManagerObject->SetUseDeath(true);//���S�t���O���g�p����
		m_pManagerObject->SetDeath();       //���S�t���O��ݒ肷��
		m_pManagerObject = nullptr;         //�|�C���^��������
	}
}
//=======================================================================================================================

//***********************************************************************************************************************
//�����̃I�u�W�F�N�g�ҏW�N���X
//***********************************************************************************************************************

//===========================================================
//�R���X�g���N�^
//===========================================================
CStageManagerState_PlacedObject::CStageManagerState_PlacedObject() : m_nChooseObjIdx(0)
{

}
//=======================================================================================================================

//===========================================================
//�f�X�g���N�^
//===========================================================
CStageManagerState_PlacedObject::~CStageManagerState_PlacedObject()
{

}
//=======================================================================================================================

//===========================================================
//����
//===========================================================
void CStageManagerState_PlacedObject::Process(CStageManager* pStageManager)
{
	ChengeIdx(pStageManager);   //�I���I�u�W�F�N�g�ԍ��̕ύX
	ChooseObject(pStageManager);//�z�u�I�u�W�F�N�g��I������
}
//=======================================================================================================================

//===========================================================
//�I�u�W�F�N�g�I��
//===========================================================
void CStageManagerState_PlacedObject::ChooseObject(CStageManager* pStageManager)
{
	list<CObject*> & StgObjList = pStageManager->GetStgObjList();//�X�e�[�W�}�l�[�W���[�I�u�W�F�N�g���X�g���擾

	auto it = StgObjList.begin();//�ŏ��̃|�C���^���擾
	advance(it, m_nChooseObjIdx);//���݂̃|�C���^���猩���I��ł���ԍ��܂ňړ�����

	(*it)->ManagerChooseControlInfo();//�I�u�W�F�N�g�N���X�ɏ���ҏW���邽�߂̊֐����e�h���N���X��override����Ă���̂ŕҏW���邱�Ƃ��ł���

	DeleteObject(it, StgObjList);     //���ݑI�����Ă���I�u�W�F�N�g��j������
}
//=======================================================================================================================

//===========================================================
//�I���I�u�W�F�N�g�ԍ���ς���
//===========================================================
void CStageManagerState_PlacedObject::ChengeIdx(CStageManager* pStageManager)
{
	list<CObject*>& StgObjList = pStageManager->GetStgObjList();
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_RIGHT) == true)
	{//RIGHT�L�[�������Ă�����
		m_nChooseObjIdx++;
	}
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_LEFT) == true)
	{//LEFT�L�[�������Ă�����
		m_nChooseObjIdx--;
	}

	int nSize = StgObjList.size();//�X�e�[�W�}�l�[�W���[�I�u�W�F�N�g���X�g�̃T�C�Y���擾

	//�z��O�΍�
	if (m_nChooseObjIdx >= nSize)
	{
		m_nChooseObjIdx = 0;
	}
	if (m_nChooseObjIdx < 0)
	{
		m_nChooseObjIdx = nSize - 1;
	}
}
//=======================================================================================================================

//===========================================================
//�I���I�u�W�F�N�g����������
//===========================================================
void CStageManagerState_PlacedObject::DeleteObject(list<CObject*>::iterator& it, list<CObject*>& StgObjList)
{
	auto Input = CManager::GetInputKeyboard();//�L�[���͏����擾
	if (Input->GetTrigger(DIK_BACKSPACE))
	{//BACKSPACE�L�[�������Ă�����
		(*it)->SetUseDeath(true);//���S�t���O���g�p����
		(*it)->SetDeath();       //���S�t���O��ݒ肷��
		StgObjList.erase(it);    //�C�e���[�^�őI��ł���I�u�W�F�N�g��j������
	}
}
//=======================================================================================================================