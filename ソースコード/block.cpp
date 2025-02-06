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
	//===========================
	//X�I�u�W�F�N�g������
	//===========================
	CObjectX::Init();
	//=========================================
	m_type = BLOCKTYPE::NORMAL;   //�u���b�N�̎��
	m_bCollision = true;           //�����蔻������邩�ǂ���
	return S_OK;
}
//=================================================

//=========================
//�I������
//=========================
void CBlock::Uninit()
{
	CObjectX::Uninit();//X�I�u�W�F�N�g�I������
}
//=================================================

//=========================
//�X�V����
//=========================
void CBlock::Update()
{
	//========================================
    //X�I�u�W�F�N�g�̍X�V����
    //========================================
	CObjectX::Update();
	//===========================================================================================

	Collision();//�ڐG������s��
}
//=================================================

//===================================================
//�`�揈��
//===================================================
void CBlock::Draw()
{
	CObjectX::Draw();
}
//===========================================================================================

//===================================================
//���S�t���O��ݒ�
//===================================================
void CBlock::SetDeath()
{
	CObject::SetDeath();
}
//===========================================================================================

//==================================
//�u���b�N��������
//==================================
CBlock* CBlock::Create(BLOCKTYPE type, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, bool bSwapVtxXZ)
{
	CBlock* pBlock = DBG_NEW CBlock;                                                               //�u���b�N�𐶐�
	bool bSuccess = pBlock->CObject::GetCreateSuccess();
	int nIdx = 0;//���f�����̃C���f�b�N�X
	if (bSuccess == true)
	{
		if (pBlock != nullptr)
		{
			pBlock->Init();                               //����������
			pBlock->GetRotInfo().SetRot(rot);                          //������ݒ�
			pBlock->GetSizeInfo().SetUseSwapVtxXZ(bSwapVtxXZ);          //���_XZ�����ւ���
			pBlock->SetUseDeath(true);                    //���S�t���O�𔭓����邩�ǂ�����ݒ肷��
			pBlock->m_type = type;                        //�u���b�N�̎��
			pBlock->GetLifeInfo().SetLife(nLife);                       //�u���b�N�̗̑�
			pBlock->GetLifeInfo().SetMaxLife(nLife);                    //�u���b�N�̗̑�
			pBlock->GetLifeInfo().SetAutoSubLife(false);                //�����I�ɑ̗͂����炷���ǂ���
			pBlock->GetPosInfo().SetSupportPos(pos);                   //�ݒu�ʒu
			pBlock->GetDrawInfo().SetUseShadow(false);
			pBlock->GetDrawInfo().SetUseDraw(true);          //�`�悷��
			pBlock->GetPosInfo().SetPos(pos);                //�I�u�W�F�N�gX�̈ʒu��ݒ�
			pBlock->GetPosInfo().SetPosOld(pos);                       //1f�O�̈ʒu��ݒ�
			pBlock->CObject::SetType(CObject::TYPE::BLOCK);//�I�u�W�F�N�g�̎�ނ����߂�
			pBlock->GetSizeInfo().SetScale(scale);                      //�I�u�W�F�N�g�̊g�嗦
			pBlock->GetSizeInfo().SetFormarScale(scale);                //���̊g�嗦��ݒ肷��
			nIdx = CManager::GetObjectXInfo()->Regist(m_BLOCK_FILENAME[(int)(type)]);
			pBlock->CObjectX::BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(nIdx),
				CManager::GetObjectXInfo()->GetBuffMat(nIdx),
				CManager::GetObjectXInfo()->GetdwNumMat(nIdx),
				CManager::GetObjectXInfo()->GetTexture(nIdx),
				CManager::GetObjectXInfo()->GetColorValue(nIdx));                     //���f���������蓖�Ă�

			pBlock->CObjectX::SetObjXType(CObjectX::OBJECTXTYPE_BLOCK);               //�I�u�W�F�N�gX�̃^�C�v��ݒ�
			pBlock->CObjectX::SetTypeNum((int)(type));                                //�I�u�W�F�N�gX���Ƃ̃^�C�v�ԍ���ݒ�
			pBlock->SetSize();                                                        //X�I�u�W�F�N�g�̃T�C�Y��ݒ肷��
			pBlock->SetManagerObjectType(CObject::MANAGEROBJECTTYPE::BLOCK);           //�}�l�[�W���[�ŌĂяo�����̎�ނ�ݒ�
			pBlock->GetSizeInfo().ActiveSwapVtxMaxMin();
		}
	}
	else
	{//�I�u�W�F�N�g�ɋ󂫂��Ȃ������̂Ŕj������
		delete pBlock;
		pBlock = nullptr;
		return nullptr;
	}
	return pBlock;
}
//================================================================================================

//===============================================================
//�u���b�N�̃^�C�v���擾����
//===============================================================
CBlock::BLOCKTYPE CBlock::GetType()
{
	return m_type;
}
//=========================================================================================================================

//===============================================================
//�u���b�N�Ƃ̓����蔻����s��
//===============================================================
void CBlock::Collision()
{
	//==================================
	//�I�u�W�F�N�gX��������擾
	//==================================
	D3DXVECTOR3 ComparisonPos = D3DXVECTOR3(0.0f,0.0f,0.0f);                  //����Ώۂ̈ʒu���擾
	D3DXVECTOR3 ComparisonPosOld = D3DXVECTOR3(0.0f,0.0f,0.0f);               //����Ώۂ�1f�O�̈ʒu���擾
	D3DXVECTOR3 ComparisonVtxMax = D3DXVECTOR3(0.0f,0.0f,0.0f);               //����Ώۂ̍ő咸�_���擾
	D3DXVECTOR3 ComparisonVtxMin = D3DXVECTOR3(0.0f,0.0f,0.0f);               //����Ώۂ̍ŏ����_���擾
	D3DXVECTOR3 MySize = D3DXVECTOR3(0.0f,0.0f,0.0f);
	D3DXVECTOR3 BlockPos = GetPosInfo().GetPos();                          //�u���b�N�̈ʒu���擾
	D3DXVECTOR3 BlockVtxMax = GetSizeInfo().GetVtxMax();                    //�u���b�N�̍ő咸�_���擾
	D3DXVECTOR3 BlockVtxMin = GetSizeInfo().GetVtxMin();                    //�u���b�N�̍ŏ����_���擾
	//===========================================================

	//�I�u�W�F�N�g�擾
	CObject* pObj = nullptr;
	//==================================
	//�u���b�N�Ƃ̓����蔻��
	//==================================
	for (int nCntPriority = 0; nCntPriority < m_nMAXPRIORITY; nCntPriority++)
	{
		pObj = GetTopObject(nCntPriority);//�g�b�v�I�u�W�F�N�g���擾
		while (pObj != nullptr)
		{
			//���̃I�u�W�F�N�g���i�[
			CObject* pNext = pObj->GetNextObject();

			//��ނ̎擾�i�G�Ȃ瓖���蔻��j
			CObject::TYPE type = pObj->GetType();

			//�I�u�W�F�N�g�̎�ނ��G��������
			if (type == CObject::TYPE::PLAYER || type == CObject::TYPE::ENEMY)
			{
				ComparisonPos.x = (float)(floor(((CObjectX*)pObj)->GetPosInfo().GetPos().x));                   //�ʒu���擾
				ComparisonPos.y = (float)(floor(((CObjectX*)pObj)->GetPosInfo().GetPos().y));                   //�ʒu���擾
				ComparisonPos.z = (float)(floor(((CObjectX*)pObj)->GetPosInfo().GetPos().z));                   //�ʒu���擾]

				ComparisonVtxMax.x = (float)(floor(((CObjectX*)pObj)->GetSizeInfo().GetVtxMax().x));             //�ő咸�_���擾
				ComparisonVtxMax.y = (float)(floor(((CObjectX*)pObj)->GetSizeInfo().GetVtxMax().y));             //�ő咸�_���擾
				ComparisonVtxMax.z = (float)(floor(((CObjectX*)pObj)->GetSizeInfo().GetVtxMax().z));             //�ő咸�_���擾

				ComparisonVtxMin.x = (float)(floor(((CObjectX*)pObj)->GetSizeInfo().GetVtxMin().x));             //�ŏ����_���擾
				ComparisonVtxMin.y = (float)(floor(((CObjectX*)pObj)->GetSizeInfo().GetVtxMin().y));             //�ŏ����_���擾
				ComparisonVtxMin.z = (float)(floor(((CObjectX*)pObj)->GetSizeInfo().GetVtxMin().z));             //�ŏ����_���擾

				ComparisonPosOld.x = (float)(floor(((CObjectX*)pObj)->GetPosInfo().GetPosOld().x));             //1f�O�̈ʒu���擾
				ComparisonPosOld.y = (float)(floor(((CObjectX*)pObj)->GetPosInfo().GetPosOld().y));             //1f�O�̈ʒu���擾
				ComparisonPosOld.z = (float)(floor(((CObjectX*)pObj)->GetPosInfo().GetPosOld().z));             //1f�O�̈ʒu���擾

				MySize.x = (float)(floor(((CObjectX*)pObj)->GetSizeInfo().GetSize().x));
				MySize.y = (float)(floor(((CObjectX*)pObj)->GetSizeInfo().GetSize().y));
				MySize.z = (float)(floor(((CObjectX*)pObj)->GetSizeInfo().GetSize().z));
				//((CObjectX*)pObj)->SetLiftLanding(false);                     //���t�g�ɏ���Ă��Ȃ���Ԃɏ�����
				//��
				if (ComparisonPos.x + ComparisonVtxMax.x > BlockPos.x + BlockVtxMin.x
					&& ComparisonPos.x + ComparisonVtxMin.x < BlockPos.x + BlockVtxMax.x
					&& ComparisonPos.y + ComparisonVtxMin.y - m_fBLOCKCORRECTIONCOLLISION < BlockPos.y + BlockVtxMax.y
					&& ComparisonPosOld.y + ComparisonVtxMin.y + m_fBLOCKCORRECTIONCOLLISION >= BlockPos.y + BlockVtxMax.y
					&& ComparisonPos.z + ComparisonVtxMax.z > BlockPos.z + BlockVtxMin.z
					&& ComparisonPos.z + ComparisonVtxMin.z < BlockPos.z + BlockVtxMax.z)
				{//�Ώۂ̉��[�����f����+Y�ɓ����������̏���

				}
				if (ComparisonPos.x + ComparisonVtxMin.x - m_fBLOCKCORRECTIONCOLLISION <= BlockPos.x + BlockVtxMax.x &&
					ComparisonPos.x + ComparisonVtxMax.x + m_fBLOCKCORRECTIONCOLLISION >= BlockPos.x + BlockVtxMin.x &&
					ComparisonPos.y + ComparisonVtxMin.y <= BlockPos.y + BlockVtxMax.y &&
					ComparisonPos.y + ComparisonVtxMax.y >= BlockPos.y + BlockVtxMin.y &&
					ComparisonPos.z + ComparisonVtxMin.z - m_fBLOCKCORRECTIONCOLLISION <= BlockPos.z + BlockVtxMax.z &&
					ComparisonPos.z + ComparisonVtxMax.z + m_fBLOCKCORRECTIONCOLLISION >= BlockPos.z + BlockVtxMin.z)
				{//�u���b�N�ɓ���������

				}
			}
			//���X�g�����ɐi�߂�
			pObj = pNext;
		}
	}

}
//=========================================================================================================================

//===============================================================
//�u���b�N�Ƃ̓����蔻����s��
//===============================================================
void CBlock::CollisionSquare(CObjectX* pObjX)
{
	//D3DXVECTOR3 ComparisonPos = D3DXVECTOR3(0.0f,0.0f,0.0f);        //��r�p�ʒu
	//D3DXVECTOR3 ComparisonPosOld = D3DXVECTOR3(0.0f,0.0f,0.0f);     //��r�p1f�O�̈ʒu
	//D3DXVECTOR3 ComparisonVtxMax = D3DXVECTOR3(0.0f,0.0f,0.0f);     //��r�p�ō����_
	//D3DXVECTOR3 ComparisonVtxMin = D3DXVECTOR3(0.0f,0.0f,0.0f);     //��r�p�Œᒸ�_
	//D3DXVECTOR3 ComparisonSize = D3DXVECTOR3(0.0f,0.0f,0.0f);       //��r�T�C�Y
	//D3DXVECTOR3 ComparisonMove = D3DXVECTOR3(0.0f,0.0f,0.0f);       //��r�ړ���
	////==========================================================================================================
	//bool& bIsLanding = pObjX->GetLanding();
	//bool bIsLandingOld = pObjX->GetLanding();//1f�O�̒��n���
	//bIsLanding = false;
	//bool& bIsWalling = pObjX->GetWalling();          //�ǂɂ������Ă��邩�ǂ���
	//bIsWalling = false;

	//bool bCollision = true;//�����蔻������邩�ǂ���

	//D3DXVECTOR3 Scale = D3DXVECTOR3(0.0f,0.0f,0.0f);

	////====================================================================
	////����J�n
	////====================================================================
	//for (int nCntPriority = 0; nCntPriority < m_nMAXPRIORITY; nCntPriority++)
	//{
	//	CObject * pObj = GetTopObject(nCntPriority);//�g�b�v�I�u�W�F�N�g���擾
	//	while (pObj != nullptr)
	//	{
	//		//���̃I�u�W�F�N�g���i�[
	//		CObject* pNext = pObj->GetNextObject();

	//		//��ނ̎擾�i�G�Ȃ瓖���蔻��j
	//		CObject::TYPE type = pObj->GetType();
	//		if (type == CObject::TYPE_BLOCK)
	//		{
	//			CBlock* pBlock = (CBlock*)pObj;

	//			float fLength = CCalculation::CalculationLength(pObjX->GetPos(), pBlock->GetPos());
	//			Scale = pBlock->GetScale();
	//			if (Scale.x >= Scale.y - 0.1f && Scale.x <= Scale.y + 0.1f)
	//			{//�g�嗦���قƂ�Ǔ����Ȃ�
	//				if (fLength > pBlock->GetSize().x * 3.0f)
	//				{
	//					bCollision = false;
	//				}
	//				else
	//				{
	//					bCollision = true;
	//				}
	//			}
	//			else
	//			{
	//				bCollision = true;
	//			}

	//			if (pBlock->m_bCollision == true && bCollision == true)
	//			{
	//				if (bIsLandingOld == true)
	//				{
	//					//�㉺
	//					pBlock->ExtrusionCollisionY(pObjX, pBlock);

	//					//���E
	//					pBlock->ExtrusionCollisionX(pObjX, pBlock);
	//					
	//				}
	//				else
	//				{
	//					//���E
	//					pBlock->ExtrusionCollisionX(pObjX, pBlock);

	//					//�㉺
	//					pBlock->ExtrusionCollisionY(pObjX, pBlock);
	//				}
	//			}
	//		}
	//		//���X�g�����ɐi�߂�
	//		pObj = pNext;
	//	}
	//}
}
//=========================================================================================================================

//===============================================================
//�w�艺�u���b�N�̏�ɏ���Ă���ꍇ�A�ʒu��␳����
//===============================================================
void CBlock::LandingCorrection(D3DXVECTOR3& Pos, CObject* pSaveObj,D3DXVECTOR3 VtxMin)
{
	CObject::TYPE type = (CObject::TYPE)(0);
	D3DXVECTOR3 ComparisonPos = D3DXVECTOR3(0.0f,0.0f,0.0f);        //��r�p�ʒu
	D3DXVECTOR3 ComparisonPosOld = D3DXVECTOR3(0.0f,0.0f,0.0f);     //��r�p1f�O�̈ʒu
	D3DXVECTOR3 ComparisonVtxMax = D3DXVECTOR3(0.0f,0.0f,0.0f);     //��r�p�ō����_
	D3DXVECTOR3 ComparisonVtxMin = D3DXVECTOR3(0.0f,0.0f,0.0f);     //��r�p�Œᒸ�_
	D3DXVECTOR3 ComparisonSize = D3DXVECTOR3(0.0f,0.0f,0.0f);       //�T�C�Y
	D3DXVECTOR3 ComparisonMove = D3DXVECTOR3(0.0f,0.0f,0.0f);

	D3DXVECTOR3 MyVtxMin = D3DXVECTOR3(0.0f,0.0f,0.0f);
	MyVtxMin.x = (float)(floor(VtxMin.x));
	MyVtxMin.y = (float)(floor(VtxMin.y));
	MyVtxMin.z = (float)(floor(VtxMin.z));

	if (pSaveObj != nullptr)
	{
		//��ނ̎擾
		type = pSaveObj->GetType();
		if (type == CObject::TYPE::BLOCK)
		{
			CBlock* pBlock = (CBlock*)pSaveObj;

			ComparisonPos.x = (float)(floor(pBlock->GetPosInfo().GetPos().x));
			ComparisonPos.y = (float)(floor(pBlock->GetPosInfo().GetPos().y));
			ComparisonPos.z = (float)(floor(pBlock->GetPosInfo().GetPos().z));

			ComparisonPosOld.x = (float)(floor(pBlock->GetPosInfo().GetPosOld().x));
			ComparisonPosOld.y = (float)(floor(pBlock->GetPosInfo().GetPosOld().y));
			ComparisonPosOld.z = (float)(floor(pBlock->GetPosInfo().GetPosOld().z));

			ComparisonVtxMax.x = (float)(floor(pBlock->GetSizeInfo().GetVtxMax().x));
			ComparisonVtxMax.y = (float)(floor(pBlock->GetSizeInfo().GetVtxMax().y));
			ComparisonVtxMax.z = (float)(floor(pBlock->GetSizeInfo().GetVtxMax().z));

			ComparisonVtxMin.x = (float)(floor(pBlock->GetSizeInfo().GetVtxMin().x));
			ComparisonVtxMin.y = (float)(floor(pBlock->GetSizeInfo().GetVtxMin().y));
			ComparisonVtxMin.z = (float)(floor(pBlock->GetSizeInfo().GetVtxMin().z));

			ComparisonSize.x = (float)(floor(pBlock->GetSizeInfo().GetSize().x));
			ComparisonSize.y = (float)(floor(pBlock->GetSizeInfo().GetSize().y));
			ComparisonSize.z = (float)(floor(pBlock->GetSizeInfo().GetSize().z));
			 

			float fPosY = fabsf(MyVtxMin.y);
			Pos.y = ComparisonPos.y + ComparisonVtxMax.y + fPosY;
		}
	}

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
	int nNewType = int(m_type);
	BLOCKTYPE NewType = BLOCKTYPE::NORMAL;

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
	NewType = BLOCKTYPE(nNewType);
	//======================================================================================

	//=======================================
	//���S�t���O��ݒ肷��
	//=======================================
	SetUseDeath(true);
	SetDeath();
	//======================================================================================

	return CBlock::Create(NewType, GetLifeInfo().GetMaxLife(), GetPosInfo().GetPos(), GetRotInfo().GetRot(), GetSizeInfo().GetScale(), GetSizeInfo().GetUseSwapVtxXZ());//���������I�u�W�F�N�g��Ԃ�
}
//=========================================================================================================================

//=======================================================================
//�e�L�X�g�t�@�C������ǂݍ��񂾏���ۑ����鏈��
//=======================================================================
void CBlock::LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff)
{
	int nType = 0;//���
	int nLife = 0;//�̗�
	D3DXVECTOR3 Move = D3DXVECTOR3(0.0f,0.0f,0.0f);//�ړ���
	D3DXVECTOR3 Pos = D3DXVECTOR3(0.0f,0.0f,0.0f); //�ʒu
	D3DXVECTOR3 Scale = D3DXVECTOR3(0.0f,0.0f,0.0f);//�g�嗦
	D3DXVECTOR3 Rot = D3DXVECTOR3(0.0f,0.0f,0.0f); //����
	BLOCKTYPE Type = {};//�u���b�N�̎��
	bool bSwapVtxXZ = false;
	while (Buff != "END_SETBLOCK")
	{
		LoadingFile >> Buff;//�P���ǂݍ���
		if (Buff == "#")
		{
			getline(LoadingFile, Buff);
		}
		else if (Buff == "TYPE")
		{
			LoadingFile >> Buff;//�C�R�[��
			LoadingFile >> nType;      //���
		}
		else if (Buff == "LIFE")
		{
			LoadingFile >> Buff;//�C�R�[��
			LoadingFile >> nLife;      //�̗�
		}
		else if (Buff == "MOVE")
		{
			LoadingFile >> Buff;//�C�R�[��
			LoadingFile >> Move.x;      //�ړ���X
			LoadingFile >> Move.y;      //�ړ���Y
			LoadingFile >> Move.z;      //�ړ���Z
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
		else if (Buff == "SWAPVTXXZ")
		{
			LoadingFile >> Buff;//�C�R�[��
			LoadingFile >> bSwapVtxXZ;
		}
	}
	Type = BLOCKTYPE(nType);

	listSaveManager.push_back(CBlock::Create(Type, nLife, Pos, Rot, Scale, bSwapVtxXZ));//vector�ɏ���ۑ�����
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

//=======================================================================
//X�����̉����o��������s��
//=======================================================================
void CBlock::ExtrusionCollisionX(CObjectX* pMyObjX, CBlock* pBlock)
{
	//D3DXVECTOR3 MyPos = pMyObjX->GetPos();
	//const D3DXVECTOR3& Pos = pMyObjX->GetPos();              //�ʒu���擾
	//const D3DXVECTOR3& PosOld = pMyObjX->GetPosOld();        //1f�O�̈ʒu���擾
	//D3DXVECTOR3 MyVtxMax = D3DXVECTOR3(0.0f,0.0f,0.0f);             //�������g�̍ő咸�_
	//D3DXVECTOR3 MyVtxMin = D3DXVECTOR3(0.0f,0.0f,0.0f);             //�������g�̍ŏ����_

	//D3DXVECTOR3 ComPos = pBlock->GetPos();             //�u���b�N�̈ʒu
	//D3DXVECTOR3 ComVtxMax = pBlock->GetVtxMax();       //�u���b�N�̍ő咸�_
	//D3DXVECTOR3 ComVtxMin = pBlock->GetVtxMin();       //�u���b�N�̍ŏ����_

	//bool& bIsWalling = pMyObjX->GetWalling();          //�ǂɂ������Ă��邩�ǂ���
	////�����o���Ɏg���l�̏����_��؂�̂Ă�i�����ȍ��Ŏ��s���Ȃ����߁j
	//MyVtxMax.x = (float)(floor(pMyObjX->GetVtxMax().x));
	//MyVtxMax.y = (float)(floor(pMyObjX->GetVtxMax().y));
	//MyVtxMax.z = (float)(floor(pMyObjX->GetVtxMax().z));

	//MyVtxMin.x = (float)(floor(pMyObjX->GetVtxMin().x));
	//MyVtxMin.y = (float)(floor(pMyObjX->GetVtxMin().y));
	//MyVtxMin.z = (float)(floor(pMyObjX->GetVtxMin().z));

	//ComPos.x = float(floor(pBlock->GetPos().x));
	//ComPos.y = float(floor(pBlock->GetPos().y));
	//ComPos.z = float(floor(pBlock->GetPos().z));

	//ComVtxMax.x = (float)(floor(ComVtxMax.x));
	//ComVtxMax.y = (float)(floor(ComVtxMax.y));
	//ComVtxMax.z = (float)(floor(ComVtxMax.z));

	//ComVtxMin.x = (float)(floor(ComVtxMin.x));
	//ComVtxMin.y = (float)(floor(ComVtxMin.y));
	//ComVtxMin.z = (float)(floor(ComVtxMin.z));
	//// 					//��
	//if (Pos.x + MyVtxMax.x > ComPos.x + ComVtxMin.x
	//	&& PosOld.x + MyVtxMax.x <= ComPos.x + ComVtxMin.x
	//	&& Pos.y + MyVtxMax.y > ComPos.y + ComVtxMin.y
	//	&& Pos.y + MyVtxMin.y < ComPos.y + ComVtxMax.y
	//	&& Pos.z + MyVtxMax.z > ComPos.z + ComVtxMin.z
	//	&& Pos.z + MyVtxMin.z < ComPos.z + ComVtxMax.z)
	//{//�Ώۂ��u���b�N��-X�ɓ����������̏���
	//	//Pos.x = ComPos.x + ComVtxMin.x - MyVtxMax.x;
	//	bIsWalling = true;
	// 
	//}

	////�E
	//else if (Pos.x + MyVtxMin.x < ComPos.x + ComVtxMax.x
	//	&& PosOld.x + MyVtxMin.x >= ComPos.x + ComVtxMax.x
	//	&& Pos.y + MyVtxMax.y > ComPos.y + ComVtxMin.y
	//	&& Pos.y + MyVtxMin.y < ComPos.y + ComVtxMax.y
	//	&& Pos.z + MyVtxMax.z > ComPos.z + ComVtxMin.z
	//	&& Pos.z + MyVtxMin.z < ComPos.z + ComVtxMax.z)
	//{//�Ώۂ��u���b�N��+X�ɓ����������̏���
	//	//Pos.x = ComPos.x + ComVtxMax.x - MyVtxMin.x;
	//	bIsWalling = true;
	//}
}
//======================================================================================================================================

//=======================================================================
//Y�����̉����o��������s��
//=======================================================================
void CBlock::ExtrusionCollisionY(CObjectX* pMyObjX, CBlock* pBlock)
{
	//D3DXVECTOR3 MyVtxMax = D3DXVECTOR3(0.0f,0.0f,0.0f);             //�������g�̍ő咸�_
	//D3DXVECTOR3 MyVtxMin = D3DXVECTOR3(0.0f,0.0f,0.0f);             //�������g�̍ŏ����_

	//D3DXVECTOR3 ComPos = pBlock->GetPos();             //�u���b�N�̈ʒu
	//D3DXVECTOR3 ComVtxMax = pBlock->GetVtxMax();       //�u���b�N�̍ő咸�_
	//D3DXVECTOR3 ComVtxMin = pBlock->GetVtxMin();       //�u���b�N�̍ŏ����_[
	//const D3DXVECTOR3& ComMove = pBlock->GetMove();           //�u���b�N�̈ړ���
	//const D3DXVECTOR3& Pos = pMyObjX->GetPos();              //�ʒu���擾
	//const D3DXVECTOR3& Move = pMyObjX->GetMove();
	//D3DXVECTOR3 PosOld = pMyObjX->GetPosOld();        //1f�O�̈ʒu���擾

	//bool& bIsLanding = pMyObjX->GetLanding();          //�n�ʂɂ��邩�ǂ���

	//bool& bIsJumping = pMyObjX->GetJumping();          //�W�����v���Ă��邩�ǂ���

	////�����o�����s�΍�
	//MyVtxMax.x = (float)(floor(pMyObjX->GetVtxMax().x));
	//MyVtxMax.y = (float)(floor(pMyObjX->GetVtxMax().y));
	//MyVtxMax.z = (float)(floor(pMyObjX->GetVtxMax().z));

	//MyVtxMin.x = (float)(floor(pMyObjX->GetVtxMin().x));
	//MyVtxMin.y = (float)(floor(pMyObjX->GetVtxMin().y));
	//MyVtxMin.z = (float)(floor(pMyObjX->GetVtxMin().z));

	//ComPos.x = float(floor(pBlock->GetPos().x));
	//ComPos.y = float(floor(pBlock->GetPos().y));
	//ComPos.z = float(floor(pBlock->GetPos().z));

	//

	//ComVtxMax.x = (float)(floor(ComVtxMax.x));
	//ComVtxMax.y = (float)(floor(ComVtxMax.y));
	//ComVtxMax.z = (float)(floor(ComVtxMax.z));

	//ComVtxMin.x = (float)(floor(ComVtxMin.x));
	//ComVtxMin.y = (float)(floor(ComVtxMin.y));
	//ComVtxMin.z = (float)(floor(ComVtxMin.z));

	////��
	//if (Pos.x + MyVtxMax.x > ComPos.x + ComVtxMin.x
	//	&& Pos.x + MyVtxMin.x < ComPos.x + ComVtxMax.x
	//	&& Pos.y + MyVtxMin.y < ComPos.y + ComVtxMax.y
	//	&& PosOld.y + MyVtxMin.y - Move.y >= ComPos.y + ComVtxMax.y
	//	&& Pos.z + MyVtxMax.z > ComPos.z + ComVtxMin.z
	//	&& Pos.z + MyVtxMin.z < ComPos.z + ComVtxMax.z)
	//{//�Ώۂ̉��[�����f����+Y�ɓ����������̏���
	//	float fPosY = fabsf(MyVtxMin.y);
	//	//Pos.y = ComPos.y + ComVtxMax.y + fPosY;
	//	pMyObjX->SetPos(D3DXVECTOR3(Pos.x, Pos.y + ComVtxMax.y + fPosY, Pos.z));
	//	pMyObjX->SetPos(Pos + ComMove);
	//	bIsLanding = true;
	//}
	////��
	//else if (Pos.x + MyVtxMax.x > ComPos.x + ComVtxMin.x
	//	&& Pos.x + MyVtxMin.x < ComPos.x + ComVtxMax.x
	//	&& Pos.y + MyVtxMax.y > ComPos.y + ComVtxMin.y
	//	&& PosOld.y + MyVtxMax.y <= ComPos.y + ComVtxMin.y
	//	&& Pos.z + MyVtxMax.z > ComPos.z + ComVtxMin.z
	//	&& Pos.z + MyVtxMin.z < ComPos.z + ComVtxMax.z)
	//{//�Ώۂ̉��[�����f����+Y�ɓ����������̏���
	//	//Pos.y = ComPos.y + ComVtxMin.y - MyVtxMax.y;
	//	pMyObjX->SetPos(D3DXVECTOR3(Pos.x,ComPos.y + ComVtxMin.y - MyVtxMax.y, Pos.z));
	//	bIsJumping = false;

	//	if (pMyObjX->GetType() == CObject::TYPE_PLAYER)
	//	{
	//		//Move.y = 0.0f;
	//		pMyObjX->SetMove(D3DXVECTOR3(Move.x, 0.0f, Move.z));
	//	}
	//}
}
//======================================================================================================================================