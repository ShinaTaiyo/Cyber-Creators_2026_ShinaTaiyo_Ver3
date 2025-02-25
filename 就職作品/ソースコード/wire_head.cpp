//===================================================================================================================================
//
//�P�P���Q�P���F���C���[�����[wire_head.cpp]
//Author:ShinaTaiyo
//
//===================================================================================================================================

//=================================================================
//�C���N���[�h
//=================================================================
#include "wire_head.h"
#include "renderer.h"
#include "game.h"
#include "objectXInfo.h"
#include "manager.h"
#include "debugtext.h"
#include "collision.h"
//===================================================================================================================================

//=================================================================
//�R���X�g���N�^
//=================================================================
CWireHead::CWireHead(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CObjectX(nPri, bUseintPri, type, ObjType),
m_bCollision(false),m_nCoolTime(0), m_CollisionObjType(CObject::TYPE::NONE)
{

}
//===================================================================================================================================

//=================================================================
//�f�X�g���N�^
//=================================================================
CWireHead::~CWireHead()
{

}
//===================================================================================================================================

//=================================================================
//����������
//=================================================================
HRESULT CWireHead::Init()
{
	CObjectX::Init();//�I�u�W�F�N�gX����������
	return S_OK;
}
//===================================================================================================================================

//=================================================================
//�I������
//=================================================================
void CWireHead::Uninit()
{
	CObjectX::Uninit();//�I�u�W�F�N�gX�I������
}
//===================================================================================================================================

//=================================================================
//�X�V����
//=================================================================
void CWireHead::Update()
{
	CObjectX::Update();//�I�u�W�F�N�gX�X�V����

	CManager::GetDebugText()->PrintDebugText("���C���[�w�b�h�̈ʒu�F%f %f %f\n", GetPosInfo().GetPos().x, GetPosInfo().GetPos().y, GetPosInfo().GetPos().z);

	if (m_nCoolTime == 0)
	{//�O�����œ�����Ȃ��悤�ɂ��邽�߂ɃN�[���^�C����ݒ�i�O���Z����j
	    m_bCollision = CollisionSquare();
	}
	else
	{
		m_bCollision = false;//�����蔻����s���܂ł̃N�[���^�C�����܂�����̂œ������Ă��Ȃ�
		m_nCoolTime--;       //�N�[���^�C�������炷
	}
}
//===================================================================================================================================

//=================================================================
//�`�揈��
//=================================================================
void CWireHead::Draw()
{
	CObjectX::Draw();//�I�u�W�F�N�gX�`�揈��
}
//===================================================================================================================================

//=================================================================
//���S�t���O�ݒ菈��
//=================================================================
void CWireHead::SetDeath()
{
	CObjectX::SetDeath();//�I�u�W�F�N�gX���S�t���O�ݒ菈��
}
//===================================================================================================================================

//=================================================================
//��������
//=================================================================
CWireHead* CWireHead::Create(D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, D3DXVECTOR3 Move, D3DXVECTOR3 Scale, int nLife)
{
	CWireHead* pWireHead = DBG_NEW CWireHead();                //���C���[�̓��𐶐�
	pWireHead->Init();                                         //����������
	pWireHead->GetPosInfo().SetPos(Pos);		               //�ʒu�̐ݒ�
	pWireHead->GetPosInfo().SetSupportPos(Pos);	               //�x�_�ʒu�̐ݒ�
	pWireHead->GetRotInfo().SetRot(Rot);		               //�����̐ݒ�
	pWireHead->GetMoveInfo().SetMove(Move);		               //�ړ��ʂ̐ݒ�
	pWireHead->GetSizeInfo().SetScale(Scale);	               //�g�嗦�̐ݒ�
	pWireHead->GetLifeInfo().SetLife(nLife);	               //�̗͂̐ݒ�
	pWireHead->GetLifeInfo().SetMaxLife(nLife);	               //�ő�̗͂̐ݒ�
	pWireHead->GetLifeInfo().SetAutoSubLife(true);             //�̗͂������I�Ɍ��炷
	

	int nIdx = CManager::GetObjectXInfo()->Regist("data\\MODEL\\Wire_Head\\Wire_Head_000.x");//���f������o�^

    //���f���������蓖�Ă�
	pWireHead->BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(nIdx),
		CManager::GetObjectXInfo()->GetBuffMat(nIdx),
		CManager::GetObjectXInfo()->GetdwNumMat(nIdx),
		CManager::GetObjectXInfo()->GetTexture(nIdx),
		CManager::GetObjectXInfo()->GetColorValue(nIdx));

	pWireHead->SetSize();//���f���̃T�C�Y��ݒ肷��

	return pWireHead;
}
//===================================================================================================================================

//=================================================================
//�����`�̓����蔻����s��
//=================================================================
bool CWireHead::CollisionSquare()
{
	for (int nCntPri = 0; nCntPri < m_nMAXPRIORITY; nCntPri++)
	{//�I�u�W�F�N�g���X�g����������
		CObject* pObj = GetTopObject(nCntPri);//���X�g�̐擪�I�u�W�F�N�g���擾����
		while (pObj != nullptr)
		{//�I�u�W�F�N�g��nullptr�ɂȂ�܂ŌJ��Ԃ�
			CObject* pNext = pObj->GetNextObject();//���̃I�u�W�F�N�g���擾
			CObject::TYPE type = pObj->GetType();//�I�u�W�F�N�g�̎�ނ��擾
			if (type == CObject::TYPE::BGMODEL || type == CObject::TYPE::ENEMY || type == CObject::TYPE::BLOCK)
			{//��ނ��w�i���f�����͓G���̓u���b�N��������
				CObjectX* pObjX = static_cast<CObjectX*>(pObj);//�I�u�W�F�N�gX�ɃL���X�g
				if (CCollision::CollisionSquare(GetPosInfo().GetPos(), GetSizeInfo().GetVtxMax(), GetSizeInfo().GetVtxMin(), pObjX->GetPosInfo().GetPos(), pObjX->GetSizeInfo().GetVtxMax(), pObjX->GetSizeInfo().GetVtxMin()) == true)
				{//�����`�̓����蔻����s��
					m_CollisionObjType = type;//�Փ˂����̂ŁA�Փ˂����I�u�W�F�N�g�̃^�C�v���i�[
					return true;              //�Փˍς݂Ȃ̂ł����p�ς�
				}
			}
			pObj = pNext;//���X�g�����ɐi�߂�
		}
	}
	return false;
}
//===================================================================================================================================