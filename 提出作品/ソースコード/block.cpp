//===================================================================================================================
//
// �Q�O�Q�T�N�U���T���F�R�[�h���Y��ɂ���[block.cpp]
// Author:ShinaTaiyo
//
//===================================================================================================================

//*******************************************************************************************************************
// �C���N���[�h
//*******************************************************************************************************************
#include "block.h"         // ���w�b�_
#include "manager.h"       // �}�l�[�W���[�Ǘ��I�u�W�F�N�g�̎擾
#include "objectXInfo.h"   // ���f�����̎擾

//*******************************************************************************************************************
// �ÓI�����o�錾
//*******************************************************************************************************************

// ���f���t�@�C����
const string CBlock::s_BlockModelFileName[static_cast<int>(CBlock::TYPE::MAX)] =
{ "data\\MODEL\\Block\\Block00_Normal.x",
  "data\\MODEL\\Block\\Block01_Water.x", 
  "data\\MODEL\\Block\\Renga_000.x", 
  "data\\MODEL\\Block\\BlackBlock_000.x", 
};

//===================================================================================================================
// 
// public�����o 
//
//===================================================================================================================

//===================================================================================================================
// �R���X�g���N�^
//===================================================================================================================
CBlock::CBlock(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) :
	CObjectX(nPri,bUseintPri,type,ObjType),
	m_type{TYPE::NORMAL}
{
	// �����Ȃ�
}

//===================================================================================================================
// �f�X�g���N�^
//===================================================================================================================
CBlock::~CBlock()
{
	// �����Ȃ�
}

//===================================================================================================================
// ����������
//===================================================================================================================
HRESULT CBlock::Init()
{
	CObjectX::Init(); // �I�u�W�F�N�gX�̏�����
	return S_OK;
}

//===================================================================================================================
// �I������
//===================================================================================================================
void CBlock::Uninit()
{
	CObjectX::Uninit(); // �I�u�W�F�N�gX�I������
}

//===================================================================================================================
// �X�V����
//===================================================================================================================
void CBlock::Update()
{
	CObjectX::Update(); // �I�u�W�F�N�gX�X�V����
}

//===================================================================================================================
// �`�揈��
//===================================================================================================================
void CBlock::Draw()
{
	CObjectX::Draw(); // �I�u�W�F�N�gX�`�揈��
}

//===================================================================================================================
// ���S�t���O�ݒ菈��
//===================================================================================================================
void CBlock::SetDeath()
{
	CObjectX::SetDeath(); //�I�u�W�F�N�gX���S�t���O�ݒ菈��
}

//===================================================================================================================
// ��������
//===================================================================================================================
CBlock* CBlock::Create(TYPE type, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, bool bSwapVtxXZ)
{
	CBlock* pBlock = DBG_NEW CBlock(); // �C���X�^���X�𐶐�

	// �C���X�^���X�̐����Ɏ��s
	if (pBlock == nullptr)
	{
		assert(false && "�u���b�N�̃C���X�^���X�̐����Ɏ��s�I");
	}

	// === �I�u�W�F�N�gX��{���擾 ===

	CObjectX::PosInfo& SelfPosInfo = pBlock->GetPosInfo(); // �ʒu���
	CObjectX::RotInfo& SelfRotInfo = pBlock->GetRotInfo(); // �������
	CObjectX::SizeInfo& SelfSizeInfo = pBlock->GetSizeInfo(); // �T�C�Y���
	CObjectX::DrawInfo& SelfDrawInfo = pBlock->GetDrawInfo(); // �`����
	CObjectX::LifeInfo& SelfLifeInfo = pBlock->GetLifeInfo(); // �̗͏��

	// === ��{���ݒ� ===

	pBlock->Init(); // ����������
	pBlock->CObject::SetType(CObject::TYPE::BLOCK); // �I�u�W�F�N�g�̎�ސݒ�
	pBlock->SetUseDeath(true); // ���S�t���O�𔭓����邩�ǂ�����ݒ�
	pBlock->SetType(type);   // �u���b�N�̎�ސݒ�
	SelfRotInfo.SetRot(rot); // ������ݒ�
	SelfPosInfo.SetSupportPos(pos); // �ݒu�ʒu�ݒ�
	SelfPosInfo.SetPos(pos);     // �ʒu�ݒ�
	SelfPosInfo.SetPosOld(pos);  //1f�O�̈ʒu��ݒ�
	SelfLifeInfo.SetLife(nLife); // �̗͐ݒ�
	SelfLifeInfo.SetMaxLife(nLife);     // �ő�̗͐ݒ�
	SelfLifeInfo.SetAutoSubLife(false); // �����I�ɑ̗͂����炷���ǂ����ݒ�
	SelfSizeInfo.SetUseSwapVtxXZ(bSwapVtxXZ); // ���_XZ�����ւ���
	SelfSizeInfo.SetScale(scale);       // �g�嗦�ݒ�
	SelfSizeInfo.SetFormarScale(scale); //���ƂȂ�g�嗦�ݒ�
	SelfDrawInfo.SetUseShadow(false); // �e��`�悷�邩�ǂ����ݒ�
	SelfDrawInfo.SetUseDraw(true);    //�`�悷�邩�ǂ����ݒ�													 
	pBlock->CObjectX::SetObjXType(CObjectX::OBJECTXTYPE_BLOCK);      // �I�u�W�F�N�gX�̃^�C�v��ݒ�
	pBlock->CObjectX::SetTypeNum((int)(type)); // �I�u�W�F�N�gX���Ƃ̃^�C�v�ԍ���ݒ�
	pBlock->GetSizeInfo().ActiveSwapVtxMaxMin(); // �ő咸�_�ƍŏ����_�����ւ����悤�ɂ���
	pBlock->SetManagerObjectType(CObject::MANAGEROBJECTTYPE::BLOCK); //�}�l�[�W���[�ŌĂяo�����̎�ނ�ݒ�
	return pBlock;
}

//===================================================================================================================
// �u���b�N�^�C�v�ݒ�
//===================================================================================================================
void CBlock::SetType(TYPE Type)
{
	int nType = static_cast<int>(Type); // �^�C�v�ԍ����i�[

	// �z��O�A�N�Z�X�`�F�b�N
	if (nType < 0 || nType >= static_cast<int>(TYPE::MAX))
	{
		// �A�T�[�g�i�ǂ��炩��false�Ȃ�assert���Ă΂��̂ŁA�E�ӂ̓R�����g�Ƃ��Ďg���܂��B�j
		assert(false && "�u���b�N�Ŕz��O�A�N�Z�X�I");
	}
	// �z��O�A�N�Z�X���Ȃ�����
	else
	{
		Uninit(); // �I�������i���f�����X�V����̂�)
		m_type = Type; // �^�C�v��ݒ�
		int nIdx = 0; // ���f���ԍ�       

		// ���f�����̓o�^�A���f���ԍ��̎擾
		nIdx = CManager::GetObjectXInfo()->Regist(s_BlockModelFileName[(int)(Type)]);   

		// ���f���������蓖�Ă�
		CObjectX::BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(nIdx),       
			CManager::GetObjectXInfo()->GetBuffMat(nIdx),
			CManager::GetObjectXInfo()->GetdwNumMat(nIdx),
			CManager::GetObjectXInfo()->GetTexture(nIdx),
			CManager::GetObjectXInfo()->GetColorValue(nIdx));

		SetSize(); // �T�C�Y�ݒ�
	}

}

//===================================================================================================================
// �u���b�N�^�C�v�擾
//===================================================================================================================
const CBlock::TYPE & CBlock::GetType()
{
	return m_type;
}

//===================================================================================================================
// �����e�L�X�g�t�@�C���ɕۑ�����
//===================================================================================================================
void CBlock::SaveInfoTxt(fstream& WritingFile)
{
	WritingFile << "SETBLOCK" << endl;
	WritingFile << "TYPE = " << static_cast<int>(m_type);

	// �^�C�v�ɂ���Ė��O���L�q
	switch (m_type)
	{
	case TYPE::NORMAL:
		WritingFile << " # NORMALBLOCK" << endl;
		break;
	case TYPE::WATER:
		WritingFile << " # WATERBLOCK" << endl;
		break;
	case TYPE::RENGA:
		WritingFile << " # RENGABLOCK" << endl;
		break;
	case TYPE::BLACK:
		WritingFile << " # BLACKBLOCK" << endl;
		break;
	default:
		break;
	}

	CObjectX::SaveInfoTxt(WritingFile); // �I�u�W�F�N�gX�̏����e�L�X�g�t�@�C���ɕۑ�����

	WritingFile << "END_SETBLOCK" << endl;
}

//===================================================================================================================
// �Ăяo�����Ƀ^�C�v��ύX�����I�u�W�F�N�g��Ԃ�
//===================================================================================================================
CObject* CBlock::ManagerChengeObject(bool bAim)
{
	int nNewType = int(m_type);  // �C���N�������g��f�N�������g������������int�ɃL���X�g
    TYPE NewType = TYPE::NORMAL; // �V�����u���b�N�̎�ނ�������

	nNewType += bAim ? 1 : -1; // �����̃t���O�ɉ����ă^�C�v���ړ�������

	// === �^�C�v�͈͐��� ====

	if (nNewType >= static_cast<int>(TYPE::MAX))
	{
		nNewType = static_cast<int>(TYPE::NORMAL);
	}
	if (nNewType < 0)
	{
		nNewType = static_cast<int>(TYPE::MAX) - 1;
	}

	// === �ύX���ݒ�J�n ===

	NewType = TYPE(nNewType); // ��L�ŕύX�����^�C�v��V�����u���b�N�̃^�C�v�ɃL���X�g
	SetUseDeath(true); // ���S�t���O���g�p����
	SetDeath();        // ���S�t���O��ݒ肷��

	return CBlock::Create(NewType, GetLifeInfo().GetMaxLife(), GetPosInfo().GetPos(), GetRotInfo().GetRot(), GetSizeInfo().GetScale(), GetSizeInfo().GetUseSwapVtxXZ());//���������I�u�W�F�N�g��Ԃ�
}

//===================================================================================================================
// �e�L�X�g�t�@�C���������ǂݍ��ޏ���
//===================================================================================================================
void CBlock::LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff, CObject* pObj)
{
	int nType = 0; // ��ށi�ǂݍ��ݗp)
	TYPE Type = TYPE::NORMAL; //�u���b�N�̎��

	while (Buff != "END_SETBLOCK")
	{
		LoadingFile >> Buff; // �P���ǂݍ���

		// �s���X�L�b�v
		if (Buff == "#")
		{
			getline(LoadingFile, Buff);
		}
		// �^�C�v�ǂݍ���
		else if (Buff == "TYPE")
		{
			LoadingFile >> Buff;  // �C�R�[��
			LoadingFile >> nType; // �^�C�v
			Type = TYPE(nType);   // �ǂݍ��񂾃^�C�v���u���b�N�̃^�C�v�ɃL���X�g
			SetType(Type);        // �u���b�N�^�C�v�ݒ�
		}
		// �I�u�W�F�N�gZ�̏���ǂݍ���
		else if (Buff == "SETOBJECTX")
		{
			CObjectX::LoadInfoTxt(LoadingFile, listSaveManager, Buff, this);
		}
	}
	listSaveManager.push_back(this); // �X�e�[�W�I�u�W�F�N�g���X�g�ɏ���ۑ�����

}

//===================================================================================================================
// �ۑ��������Ԃ�
//===================================================================================================================
CObject* CBlock::ManagerSaveObject()
{
	return CBlock::Create(m_type, GetLifeInfo().GetMaxLife(), GetPosInfo().GetPos(), GetRotInfo().GetRot(), GetSizeInfo().GetScale(),GetSizeInfo().GetUseSwapVtxXZ());//���������I�u�W�F�N�g��Ԃ�
}
