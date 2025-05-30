//===================================================================================================================
//
// �Q�O�Q�T�N�T���Q�T���F�R�[�h���Y��ɂ���[bgModel.cpp]
// Author:ShinaTaiyo
//
//===================================================================================================================

//*******************************************************************************************************************
// �C���N���[�h
//*******************************************************************************************************************
#include "bgModel.h"     // �w�i���f��
#include "manager.h"     // �}�l�[�W���[ 
#include "objectX.h"     // �I�u�W�F�N�gX
#include "objectXInfo.h" // �I�u�W�F�N�gX���

//*******************************************************************************************************************
// �ÓI�����o�錾
//*******************************************************************************************************************

// ���f���t�@�C����
const string CBgModel::BGMODEL_FILENAME[static_cast<int>(CBgModel::TYPE::MAX)] =
{
	"data\\MODEL\\BgModel\\Bill_000.x",
	"data\\MODEL\\BgModel\\Tree_000.x",
	"data\\MODEL\\BgModel\\Bill_001.x",
	"data\\MODEL\\BgModel\\Bill_002.x",
	"data\\MODEL\\BgModel\\Bill_003.x",
	"data\\MODEL\\BgModel\\Bill_004.x",
	"data\\MODEL\\BgModel\\Chain_000.x",
	"data\\MODEL\\BgModel\\House_000.x",
	"data\\MODEL\\BgModel\\Grass_000.x",
	"data\\MODEL\\BgModel\\GrassGround_000.x",
	"data\\MODEL\\BgModel\\Log_000.x",
};

//===================================================================================================================
// 
// public�����o 
//
//===================================================================================================================

//===================================================================================================================
// �R���X�g���N�^
//===================================================================================================================
CBgModel::CBgModel(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) :
CObjectX(nPri,bUseintPri,type,ObjType),
m_Type(TYPE::BILL_00)
{
	SetManagerObjectType(CObject::MANAGEROBJECTTYPE::BGMODEL); // �X�e�[�W�}�l�[�W���[�ŌĂяo�����̎�ނ�ݒ�
}

//===================================================================================================================
// �f�X�g���N�^
//===================================================================================================================
CBgModel::~CBgModel()
{
	// �����Ȃ�
}

//===================================================================================================================
// ����������
//===================================================================================================================
HRESULT CBgModel::Init()
{
	CObjectX::Init(); //�I�u�W�F�N�gX����������
	return S_OK;
}

//===================================================================================================================
// �I������
//===================================================================================================================
void CBgModel::Uninit()
{
	CObjectX::Uninit(); // �I�u�W�F�N�gX�I������
}

//===================================================================================================================
// �X�V����
//===================================================================================================================
void CBgModel::Update()
{
	CObjectX::Update(); // �I�u�W�F�N�gX�X�V����
}

//===================================================================================================================
// �`�揈��
//===================================================================================================================
void CBgModel::Draw()
{
	CObjectX::Draw(); // �I�u�W�F�N�gX�`�揈��
}

//===================================================================================================================
// ���S�t���O�ݒ菈��
//===================================================================================================================
void CBgModel::SetDeath()
{
	CObjectX::SetDeath(); //�I�u�W�F�N�gX���S�t���O�ݒ菈��
}

//===================================================================================================================
// ��������
//===================================================================================================================
CBgModel* CBgModel::Create(TYPE bgModelType, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale, bool bSwapVtxXZ)
{
	// === �ϐ� ===

	CBgModel* pBgModel = DBG_NEW CBgModel(); // ����
	CObjectX::PosInfo& SelfPosInfo = pBgModel->GetPosInfo(); // �ʒu���
	CObjectX::RotInfo& SelfRotInfo = pBgModel->GetRotInfo(); // �������
	CObjectX::SizeInfo& SelfSizeInfo = pBgModel->GetSizeInfo(); // �T�C�Y���
	CObjectX::DrawInfo& SelfDrawInfo = pBgModel->GetDrawInfo(); // �`����
	CObjectX::LifeInfo& SelfLifeInfo = pBgModel->GetLifeInfo(); // �̗͏��

	// === ���� ===

	pBgModel->Init(); // ����������
	pBgModel->SetType(bgModelType); // �w�i���f���̎�ނ�ݒ肷��
	SelfPosInfo.SetPos(pos);      // �ʒu��ݒ�  
	SelfSizeInfo.SetScale(Scale); // �g�嗦��ݒ�
	SelfRotInfo.SetRot(rot);      // ������ݒ�
	SelfPosInfo.SetSupportPos(pos); // �x�_�ƂȂ�ʒu��ݒ�
	SelfSizeInfo.SetUseSwapVtxXZ(bSwapVtxXZ); // XZ�̒��_�̃T�C�Y�����ւ��邩�ǂ���
	SelfDrawInfo.SetUseShadow(false);   // �e���g�p���Ȃ�
	SelfLifeInfo.SetAutoSubLife(false); // �̗͂������I�Ɍ��炳�Ȃ�
	SelfLifeInfo.SetMaxLife(10); // �ő�̗�
	SelfLifeInfo.SetLife(10);    // �̗�

	// XZ�̃T�C�Y�����ւ���Ȃ�
	if (bSwapVtxXZ == true)
	{
		// === �ϐ� ===

		D3DXVECTOR3 VtxMax = SelfSizeInfo.GetOriginVtxMax(); // ���̍ő咸�_���擾
		D3DXVECTOR3 VtxMin = SelfSizeInfo.GetOriginVtxMin(); // ���̍ŏ����_���擾

		// === ���� ===

		VtxMax.x = SelfSizeInfo.GetOriginVtxMax().z; // �ő咸�_��X��Z��
		VtxMax.z = SelfSizeInfo.GetOriginVtxMax().x; // �ő咸�_��Z��X��
		VtxMin.x = SelfSizeInfo.GetOriginVtxMin().z; // �ŏ����_��X��Z��
		VtxMin.z = SelfSizeInfo.GetOriginVtxMin().x; // �ŏ����_��Z��X��
		SelfSizeInfo.SetOriginVtxMax(VtxMax); // ���̍ő咸�_�����ݐݒ肵�����ɍX�V
		SelfSizeInfo.SetOriginVtxMin(VtxMin); // ���̍ŏ����_�����ݐݒ肵�����ɍX�V
	}
	return pBgModel;
}

//===================================================================================================================
// �X�e�[�W�}�l�[�W���[�ɏ���ۑ����鏈��
//===================================================================================================================
void CBgModel::SaveInfoTxt(fstream& WritingFile)
{
	WritingFile << "SETBGMODEL" << endl;
	WritingFile << "TYPE = " << static_cast<int>(m_Type);

	// �^�C�v�ɉ����Ė��O���L�q
	switch (m_Type)
	{
	case TYPE::BILL_00:
		WritingFile << " # BILL00" << endl;
		break;
	case TYPE::TREE_00:
		WritingFile << " # TREE00" << endl;
		break;
	default:
		break;
	}

	CObjectX::SaveInfoTxt(WritingFile); // �I�u�W�F�N�gX�̏��ۑ�����

	WritingFile << "END_SETBGMODEL" << endl;

}

//===================================================================================================================
// �X�e�[�W�}�l�[�W���[�������ǂݍ��ޏ���
//===================================================================================================================
void CBgModel::LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff, CObject* pObj)
{
	// === �ϐ� ===

	int nType = 0; // ���
	CBgModel::TYPE Type = {}; // �w�i���f���̎��

	// === ���� ===

	// END_SETBGMODEL�܂ŌJ��Ԃ�
	while (Buff != "END_SETBGMODEL")
	{
		// === ���� ===

		LoadingFile >> Buff; // �P���ǂݍ���

		// #����������s��ǂݍ��݃X�L�b�v
		if (Buff == "#")
		{
			getline(LoadingFile, Buff);
		}
		// �^�C�v��ݒ�
		else if (Buff == "TYPE")
		{
			LoadingFile >> Buff;  // �C�R�[��
			LoadingFile >> nType; // ���
			Type = static_cast<CBgModel::TYPE>(nType); // �w�i���f���̃^�C�v�ɃL���X�g
			CBgModel::SetType(Type); // �w�i���f���̃^�C�v��ݒ�
		}
		// �I�u�W�F�N�gX���̐ݒ�
		else if (Buff == "SETOBJECTX")
		{
			CObjectX::LoadInfoTxt(LoadingFile, listSaveManager, Buff, this);
		}
	}

	listSaveManager.push_back(this); // �X�e�[�W�}�l�[�W���[�̃I�u�W�F�N�g���X�g�ɏ���o�^����
}

//===================================================================================================================
// �X�e�[�W�}�l�[�W���[�Ǘ��I�u�W�F�N�g�̎�ނ�ς���
//===================================================================================================================
CObject* CBgModel::ManagerChengeObject(bool bAim)
{
	// === �ϐ� ===

	int nNewType = int(m_Type); // �V�����^�C�v
	CBgModel::TYPE NewType = {};// �w�i���f���̃^�C�v

	// === ���� ===

	// true�Ȃ�^�C�v�ԍ����C���N�������g
	if (bAim == true)
	{
		nNewType++;
	}
	// false�Ȃ�^�C�v�ԍ����f�N�������g
	else
	{
		nNewType--;
	}

	// �͈͐���
	if (nNewType >= static_cast<int>(CBgModel::TYPE::MAX))
	{
		nNewType = 0;
	}
	if (nNewType < 0)
	{
		nNewType = static_cast<int>(CBgModel::TYPE::MAX) - 1;
	}

	NewType = static_cast<CBgModel::TYPE>(nNewType); // ��L�ŕύX�����^�C�v�ɐݒ�

	// ��L�ŕύX�����^�C�v�ɂ���̂Ŕj�����ĐV�����^�C�v�̃I�u�W�F�N�g��Ԃ�
	SetUseDeath(true); // ���S�t���O���g�p����
	SetDeath();        // ���S�t���O��ݒ�
	return CBgModel::Create(NewType, GetPosInfo().GetPos(), GetRotInfo().GetRot(), GetSizeInfo().GetScale(),GetSizeInfo().GetUseSwapVtxXZ());
}

//===================================================================================================================
// �X�e�[�W�}�l�[�W���[�ɃI�u�W�F�N�g��ۑ�����
//===================================================================================================================
CObject* CBgModel::ManagerSaveObject()
{
	return CBgModel::Create(m_Type, GetPosInfo().GetPos(), GetRotInfo().GetRot(), GetSizeInfo().GetScale(), GetSizeInfo().GetUseSwapVtxXZ());//���������I�u�W�F�N�g��Ԃ�
}

//===================================================================================================================
// ��ނ��擾
//===================================================================================================================
const CBgModel::TYPE& CBgModel::GetType() const
{
	return m_Type;
}

//===================================================================================================================
// ��ނ�ݒ�
//===================================================================================================================
void CBgModel::SetType(TYPE Type)
{
	// === ���� ===

	Uninit(); // ���݂̃��f�������I������

	// === �ϐ� ===
	
	//���f�����ݒ�
	int nIdx = CManager::GetObjectXInfo()->Regist(BGMODEL_FILENAME[static_cast<int>(Type)]);
	m_Type = Type; // �^�C�v�ԍ����i�[

	// === ���� ===

	// ���f���������蓖�Ă�
	BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(nIdx),
		CManager::GetObjectXInfo()->GetBuffMat(nIdx),
		CManager::GetObjectXInfo()->GetdwNumMat(nIdx),
		CManager::GetObjectXInfo()->GetTexture(nIdx),
		CManager::GetObjectXInfo()->GetColorValue(nIdx));

	SetSize(); // �T�C�Y��ݒ肷��
}