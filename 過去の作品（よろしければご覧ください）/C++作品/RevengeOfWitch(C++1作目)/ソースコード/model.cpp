//===========================================================================================================
//
//�U���T���F���f���̓����蔻����s��[model.cpp]
//Author:ShinaTaiyo
//
//===========================================================================================================

//==========================================================
//�C���N���[�h
//==========================================================
#include "model.h"
#include "manager.h"
#include "renderer.h"
#include "objectXInfo.h"
#include "stagemanager.h"
//============================================================================================================

//==========================================================
//�ÓI�����o������
//==========================================================
int CModel::m_nNumFile = CModel::MODELTYPE_MAX;                                               //�t�@�C�������i�[����
const char* CModel::m_BLOCK_FILENAME[CModel::MODELTYPE_MAX] =
{ "data\\MODEL\\bill.x", 
  "data\\MODEL\\TV_000.x",
  "data\\MODEL\\NintendoSwitch_000.x",
  "data\\MODEL\\NormalModel\\Ring_000.x"};                                                    //�u���b�N��X�t�@�C���ւ̃|�C���^


//==========================================================
//�R���X�g���N�^
//==========================================================
CModel::CModel()
{

}
//============================================================================================================

//==========================================================
//�f�X�g���N�^
//==========================================================
CModel::~CModel()
{

}
//============================================================================================================

//==========================================================
//����������
//==========================================================
HRESULT CModel::Init()
{
	//===========================
	//X�I�u�W�F�N�g������
	//===========================
	CObjectX::Init();

	//=========================================

	m_Type = (MODELTYPE)(0);                       //���f�����
	return S_OK;
}
//============================================================================================================

//==========================================================
//�I������
//==========================================================
void CModel::Uninit()
{
	CObjectX::Uninit();//X�I�u�W�F�N�g�I������
}
//============================================================================================================

//==========================================================
//�X�V����
//==========================================================
void CModel::Update()
{
	CObjectX::Update();
}
//============================================================================================================

//==========================================================
//�`�揈��
//==========================================================
void CModel::Draw()
{
	CObjectX::Draw();
}
//============================================================================================================

//==========================================================
//���S�t���O��ݒ�
//==========================================================
void CModel::SetDeath()
{
	CObject::SetDeath();
}
//============================================================================================================

//==========================================================
//��������
//==========================================================
CModel* CModel::Create(MODELTYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXVECTOR3 Scale, CObjectX::ROTTYPE RotType)
{
	CModel* pModel = DBG_NEW CModel;                                                               //�u���b�N�𐶐�
	bool bSuccess = pModel->CObject::GetCreateSuccess();
	LPDIRECT3DTEXTURE9 pResultTexture[m_nMAXOBJECT] = {};                                        //�ݒ肳�ꂽ��ނ̔z��̃e�N�X�`������p
	int nIdx = 0;//���f���̃C���f�b�N�X
	if (bSuccess == true)
	{
		if (pModel != nullptr)
		{
			pModel->Init();                                                                                                  //����������
			pModel->SetUseDeath(true);                                                                  //���S�t���O�𔭓����邩�ǂ�����ݒ肷��
			pModel->SetAutoSubLife(false);
			pModel->SetUseBlockCollision(true);
			pModel->m_Type = type;                                                                                           //�u���b�N�̎��
			pModel->CObjectX::SetPos(pos);                                                                                   //�I�u�W�F�N�gX�̈ʒu��ݒ�
			pModel->SetPosOld(pos);                                                                                         //1f�O�̈ʒu��ݒ�
			pModel->SetSupportPos(pos);                                                                                     //�ݒu�ʒu
			pModel->CObjectX::SetScale(Scale);                                                                               //�I�u�W�F�N�g�̊g�嗦
			pModel->SetFormarScale(Scale);                                                                                  //���̊g�嗦��ݒ肷��
			nIdx = CManager::GetObjectXInfo()->Regist(CObjectX::OBJECTXTYPE_MODEL, m_BLOCK_FILENAME[(int)(type)]);
			pModel->CObjectX::BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(CObjectX::OBJECTXTYPE_MODEL, nIdx),
				CManager::GetObjectXInfo()->GetBuffMat(CObjectX::OBJECTXTYPE_MODEL, nIdx),
				CManager::GetObjectXInfo()->GetdwNumMat(CObjectX::OBJECTXTYPE_MODEL, nIdx),
				CManager::GetObjectXInfo()->GetTexture(CObjectX::OBJECTXTYPE_MODEL, nIdx),
				CManager::GetObjectXInfo()->GetColorValue(CObjectX::OBJECTXTYPE_MODEL, nIdx));           //���f���������蓖�Ă�

			pModel->SetType(CObject::TYPE_MODEL);//�^�C�v�ݒ�
			pModel->CObjectX::SetObjXType(CObjectX::OBJECTXTYPE_MODEL);                            //�I�u�W�F�N�gX�̃^�C�v��ݒ�
			pModel->CObjectX::SetTypeNum((int)(type));                                             //�I�u�W�F�N�gX���Ƃ̃^�C�v�ԍ���ݒ�
 			pModel->SetSize();      //X�I�u�W�F�N�g�̃T�C�Y��ݒ肷��
			pModel->CObjectX::SetRotType(RotType);                                                 //�����̎�ނ�ݒ�
			pModel->SetManagerType((int)(CStageManager::MANAGEROBJECT_MODEL));        //�}�l�[�W���[�ŌĂяo�����̎�ނ�ݒ�
		}
	}
	else
	{//�I�u�W�F�N�g�ɋ󂫂��Ȃ������̂Ŕj������
		delete pModel;
		pModel = nullptr;
		return nullptr;
	}
	return pModel;
}
//============================================================================================================

