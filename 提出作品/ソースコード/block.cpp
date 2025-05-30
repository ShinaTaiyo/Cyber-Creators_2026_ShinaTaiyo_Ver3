//==========================================================================================================
//
//�V���P�Q���F�S�M�~�b�N����������[block.cpp]
//Author:ShinaTaiyo
//
//==========================================================================================================

//=========================
//�C���N���[�h
//=========================
#include "block.h"
#include "manager.h"
#include "objectXInfo.h"
#include "calculation.h"
#include "stagemanager.h"
#include "player.h"
#include "game.h"
#include "edit.h"
#include "input.h"
#include <stdio.h>
#include <string.h>
//=================================================

//=========================
//�ÓI�����o
//=========================
int CBlock::m_nNumFile = static_cast<int>(CBlock::BLOCKTYPE::MAX);                                              //�t�@�C�������i�[����
const float CBlock::m_fBLOCKCORRECTIONCOLLISION = 3.0f;      //1,2,3,4,5,6,7,8,9,10,11,12,13 
const char* CBlock::m_BLOCK_FILENAME[static_cast<int>(CBlock::BLOCKTYPE::MAX)] =
{ "data\\MODEL\\Block\\Block00_Normal.x",
  "data\\MODEL\\Block\\Block01_Water.x", 
  "data\\MODEL\\Block\\Renga_000.x", 
  "data\\MODEL\\Block\\BlackBlock_000.x", 
};                                                  //�u���b�N��X�t�@�C���ւ̃|�C���^
//========================================================

//=========================
//�R���X�g���N�^
//=========================
CBlock::CBlock(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CObjectX(nPri,bUseintPri,type,ObjType),
m_bCollision(false),m_type(BLOCKTYPE::NORMAL)
{

}
//=================================================

//=========================
//�f�X�g���N�^
//=========================
CBlock::~CBlock()
{

}
//=================================================

//=========================
//����������
//=========================
HRESULT CBlock::Init()
{
	CObjectX::Init();             //�I�u�W�F�N�gX�̏�����
	m_type = BLOCKTYPE::NORMAL;   //�u���b�N�̎��
	m_bCollision = true;          //�����蔻������邩�ǂ���
	return S_OK;
}
//=================================================

//=========================
//�I������
//=========================
void CBlock::Uninit()
{
	CObjectX::Uninit();//�I�u�W�F�N�gX�I������
}
//=================================================

//=========================
//�X�V����
//=========================
void CBlock::Update()
{
	CObjectX::Update();//�I�u�W�F�N�gX�X�V����
}
//=================================================

//===================================================
//�`�揈��
//===================================================
void CBlock::Draw()
{
	CObjectX::Draw();//�I�u�W�F�N�gX�`�揈��
}
//===========================================================================================

//===================================================
//���S�t���O��ݒ�
//===================================================
void CBlock::SetDeath()
{
	CObjectX::SetDeath();//�I�u�W�F�N�gX���S�t���O�ݒ菈��
}
//===========================================================================================

//==================================
//�u���b�N��������
//==================================
CBlock* CBlock::Create(BLOCKTYPE type, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, bool bSwapVtxXZ)
{
	CBlock* pBlock = DBG_NEW CBlock;                                                         //�u���b�N�𐶐�

	if (pBlock != nullptr)
	{
		pBlock->Init();                                                                      //����������
		pBlock->GetRotInfo().SetRot(rot);                                                    //������ݒ�
		pBlock->GetSizeInfo().SetUseSwapVtxXZ(bSwapVtxXZ);                                   //���_XZ�����ւ���
		pBlock->SetUseDeath(true);                                                           //���S�t���O�𔭓����邩�ǂ�����ݒ肷��
		pBlock->SetBlockType(type);                                                               //�u���b�N�̎��
		pBlock->GetLifeInfo().SetLife(nLife);                                                //�u���b�N�̗̑�
		pBlock->GetLifeInfo().SetMaxLife(nLife);                                             //�u���b�N�̍ő�̗�
		pBlock->GetLifeInfo().SetAutoSubLife(false);                                         //�����I�ɑ̗͂����炷���ǂ���
		pBlock->GetPosInfo().SetSupportPos(pos);                                             //�ݒu�ʒu
		pBlock->GetDrawInfo().SetUseShadow(false);                                           //�e��`�悵�Ȃ�
		pBlock->GetDrawInfo().SetUseDraw(true);                                              //�`�悷��
		pBlock->GetPosInfo().SetPos(pos);                                                    //�I�u�W�F�N�gX�̈ʒu��ݒ�
		pBlock->GetPosInfo().SetPosOld(pos);                                                 //1f�O�̈ʒu��ݒ�
		pBlock->CObject::SetType(CObject::TYPE::BLOCK);                                      //�I�u�W�F�N�g�̎�ނ����߂�
		pBlock->GetSizeInfo().SetScale(scale);                                               //�I�u�W�F�N�g�̊g�嗦
		pBlock->GetSizeInfo().SetFormarScale(scale);                                         //���̊g�嗦��ݒ肷��

		pBlock->CObjectX::SetObjXType(CObjectX::OBJECTXTYPE_BLOCK);                          //�I�u�W�F�N�gX�̃^�C�v��ݒ�
		pBlock->CObjectX::SetTypeNum((int)(type));                                           //�I�u�W�F�N�gX���Ƃ̃^�C�v�ԍ���ݒ�
		pBlock->SetManagerObjectType(CObject::MANAGEROBJECTTYPE::BLOCK);                     //�}�l�[�W���[�ŌĂяo�����̎�ނ�ݒ�
		pBlock->GetSizeInfo().ActiveSwapVtxMaxMin();                                         //�ő咸�_�ƍŏ����_�����ւ����悤�ɂ���
	}
	return pBlock;
}
//================================================================================================


//===============================================================
//�u���b�N�̃^�C�v��ݒ肷��
//===============================================================
void CBlock::SetBlockType(BLOCKTYPE Type)
{
	int nType = static_cast<int>(Type);//�^�C�v�ԍ����i�[
	if (nType < 0 || nType >= static_cast<int>(BLOCKTYPE::MAX))
	{//��O����
		assert("�u���b�N�Ŕz��O�A�N�Z�X�I");
	}
	else
	{
		Uninit();//�I�������i���f�����X�V����̂�)
		m_type = Type;//�^�C�v��ݒ�
		int nIdx = 0;                                                                            //���f�����̃C���f�b�N�X
		nIdx = CManager::GetObjectXInfo()->Regist(m_BLOCK_FILENAME[(int)(Type)]);            //���f������o�^���A���f���ԍ����擾
		CObjectX::BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(nIdx),         //���f������ݒ肷��
			CManager::GetObjectXInfo()->GetBuffMat(nIdx),
			CManager::GetObjectXInfo()->GetdwNumMat(nIdx),
			CManager::GetObjectXInfo()->GetTexture(nIdx),
			CManager::GetObjectXInfo()->GetColorValue(nIdx));

		SetSize();//�T�C�Y�ݒ�
	}
}
//================================================================================================

//===============================================================
//�u���b�N�̃^�C�v���擾����
//===============================================================
CBlock::BLOCKTYPE CBlock::GetType()
{
	return m_type;//�u���b�N�̃^�C�v���擾����
}
//=========================================================================================================================

//=======================================================================
//�K�v�ȏ����e�L�X�g�t�@�C���ɕۑ�����
//=======================================================================
void CBlock::SaveInfoTxt(fstream& WritingFile)
{
	WritingFile << "SETBLOCK" << endl;
	WritingFile << "TYPE = " << static_cast<int>(m_type);
	switch (m_type)
	{
	case BLOCKTYPE::NORMAL:
		WritingFile << " # NORMALBLOCK" << endl;
		break;
	case BLOCKTYPE::WATER:
		WritingFile << " # WATERBLOCK" << endl;
		break;
	case BLOCKTYPE::RENGA:
		WritingFile << " # RENGABLOCK" << endl;
		break;
	case BLOCKTYPE::BLACK:
		WritingFile << " # BLACKBLOCK" << endl;
		break;
	default:
		break;
	}

	CObjectX::SaveInfoTxt(WritingFile);

	WritingFile << "END_SETBLOCK" << endl;
}
//=========================================================================================================================

//=======================================================================
//�X�e�[�W�}�l�[�W���[�ɕύX�����I�u�W�F�N�g��Ԃ�
//=======================================================================
CObject* CBlock::ManagerChengeObject(bool bAim)
{
	int nNewType = int(m_type);//�C���N�������g��f�N�������g������������int�ɃL���X�g
	BLOCKTYPE NewType = BLOCKTYPE::NORMAL;//�V�����u���b�N�̎�ނ�������

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
	if (nNewType >= static_cast<int>(BLOCKTYPE::MAX))
	{
		nNewType = static_cast<int>(BLOCKTYPE::NORMAL);
	}
	if (nNewType < 0)
	{
		nNewType = static_cast<int>(BLOCKTYPE::MAX) - 1;
	}
	//======================================================================================

	//=======================================
	//�ݒ肷��
	//=======================================
	NewType = BLOCKTYPE(nNewType);//��L�ŕύX�����^�C�v��V�����u���b�N�̃^�C�v�ɃL���X�g
	//======================================================================================

	//=======================================
	//���S�t���O��ݒ肷��
	//=======================================
	SetUseDeath(true);//���S�t���O�𔭓����邩�ǂ�����true1�ɂ���
	SetDeath();       //���S�t���O��ݒ肷��
	//======================================================================================

	return CBlock::Create(NewType, GetLifeInfo().GetMaxLife(), GetPosInfo().GetPos(), GetRotInfo().GetRot(), GetSizeInfo().GetScale(), GetSizeInfo().GetUseSwapVtxXZ());//���������I�u�W�F�N�g��Ԃ�
}
//=========================================================================================================================

////=======================================================================
////�e�L�X�g�t�@�C������ǂݍ��񂾏���ۑ����鏈��
////=======================================================================
//void CBlock::LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff)
//{
//	int nType = 0;                                  //���
//	int nLife = 0;                                  //�̗�
//	D3DXVECTOR3 Move = D3DXVECTOR3(0.0f,0.0f,0.0f); //�ړ���
//	D3DXVECTOR3 Pos = D3DXVECTOR3(0.0f,0.0f,0.0f);  //�ʒu
//	D3DXVECTOR3 Scale = D3DXVECTOR3(0.0f,0.0f,0.0f);//�g�嗦
//	D3DXVECTOR3 Rot = D3DXVECTOR3(0.0f,0.0f,0.0f);  //����
//	BLOCKTYPE Type = BLOCKTYPE::NORMAL;             //�u���b�N�̎��
//	bool bSwapVtxXZ = false;                        //XZ�̒��_�����ւ��邩�ǂ���
//	while (Buff != "END_SETBLOCK")
//	{
//		LoadingFile >> Buff;           //�P���ǂݍ���
//		if (Buff == "#")
//		{//�s���X�L�b�v
//			getline(LoadingFile, Buff);
//		}
//		else if (Buff == "TYPE")
//		{
//			LoadingFile >> Buff;       //�C�R�[��
//			LoadingFile >> nType;      //���
//		}
//		else if (Buff == "LIFE")
//		{
//			LoadingFile >> Buff;       //�C�R�[��
//			LoadingFile >> nLife;      //�̗�
//		}
//		else if (Buff == "MOVE")
//		{
//			LoadingFile >> Buff;       //�C�R�[��
//			LoadingFile >> Move.x;     //�ړ���X
//			LoadingFile >> Move.y;     //�ړ���Y
//			LoadingFile >> Move.z;     //�ړ���Z
//		}
//		else if (Buff == "POS")
//		{
//			LoadingFile >> Buff;       //�C�R�[��
//			LoadingFile >> Pos.x;      //�ʒuX
//			LoadingFile >> Pos.y;      //�ʒuY
//			LoadingFile >> Pos.z;      //�ʒuZ
//		}
//		else if (Buff == "ROT")
//		{
//			LoadingFile >> Buff;       //�C�R�[��
//			LoadingFile >> Rot.x;      //�ʒuX
//			LoadingFile >> Rot.y;      //�ʒuY
//			LoadingFile >> Rot.z;      //�ʒuZ
//		}
//		else if (Buff == "SCALE")
//		{
//			LoadingFile >> Buff;       //�C�R�[��
//			LoadingFile >> Scale.x;    //�g�嗦X
//			LoadingFile >> Scale.y;    //�g�嗦Y
//			LoadingFile >> Scale.z;    //�g�嗦Z
//		}
//		else if (Buff == "SWAPVTXXZ")
//		{
//			LoadingFile >> Buff;       //�C�R�[��
//			LoadingFile >> bSwapVtxXZ; //XZ�̒��_�����ւ��邩�ǂ���
//		}
//	}
//	Type = BLOCKTYPE(nType);           //�u���b�N�̎�ނ�ݒ�
//
//	listSaveManager.push_back(CBlock::Create(Type, nLife, Pos, Rot, Scale, bSwapVtxXZ));//�X�e�[�W�I�u�W�F�N�g���X�g�ɏ���ۑ�����
//}
////=========================================================================================================================

//=======================================================================
//�e�L�X�g�t�@�C������ǂݍ��񂾏���ۑ����鏈��
//=======================================================================
void CBlock::LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff, CObject* pObj)
{
	int nType = 0;                                  //���
	BLOCKTYPE Type = BLOCKTYPE::NORMAL;             //�u���b�N�̎��
	while (Buff != "END_SETBLOCK")
	{
		LoadingFile >> Buff;           //�P���ǂݍ���
		if (Buff == "#")
		{//�s���X�L�b�v
			getline(LoadingFile, Buff);
		}
		else if (Buff == "TYPE")
		{
			LoadingFile >> Buff;       //�C�R�[��
			LoadingFile >> nType;      //���
			Type = BLOCKTYPE(nType);   //�u���b�N�̎�ނ�ݒ�
			SetBlockType(Type);        //�u���b�N�̃^�C�v��ݒ�
		}
		else if (Buff == "SETOBJECTX")
		{//�I�u�W�F�N�gX�̏���ݒ肷��
			CObjectX::LoadInfoTxt(LoadingFile, listSaveManager, Buff, this);
		}
	}
	listSaveManager.push_back(this);   //�X�e�[�W�I�u�W�F�N�g���X�g�ɏ���ۑ�����

}
//=========================================================================================================================

//=======================================================================
//�X�e�[�W�}�l�[�W���[�ɃZ�[�u����
//=======================================================================
CObject* CBlock::ManagerSaveObject()
{
	return CBlock::Create(m_type, GetLifeInfo().GetMaxLife(), GetPosInfo().GetPos(), GetRotInfo().GetRot(), GetSizeInfo().GetScale(),GetSizeInfo().GetUseSwapVtxXZ());//���������I�u�W�F�N�g��Ԃ�
}
//=========================================================================================================================