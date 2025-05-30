//====================================================================================================================
//
//�X���V���FAI�p�̃��f�������[aimodel.cpp]
//Author:ShinaTaiyo
//
//====================================================================================================================

//===========================================================
//�C���N���[�h
//===========================================================
#include "manager.h"
#include "renderer.h"
#include "aimodel.h"
#include "objectXInfo.h"
//====================================================================================================================

//===========================================================
//�ÓI�����o
//===========================================================
const char* CAIModel::m_apAIMODEL_FILENAME[CAIModel::AIMODELTYPE_MAX] =
{
	"data\\MODEL\\AIModel\\AIModel00_Square.x",
	"data\\MODEL\\AIModel\\AIModel01_Ball.x",
};//���f���t�@�C����

//===========================================================
//�R���X�g���N�^
//===========================================================
CAIModel::CAIModel() : m_Type(AIMODELTYPE_SQUARE)
{

}
//====================================================================================================================

//===========================================================
//�f�X�g���N�^
//===========================================================
CAIModel::~CAIModel()
{
}
//====================================================================================================================

//===========================================================
//����������
//===========================================================
HRESULT CAIModel::Init()
{
	CObjectX::Init();
	return S_OK;
}
//====================================================================================================================

//===========================================================
//�I������
//===========================================================	
void CAIModel::Uninit()
{
	CObjectX::Uninit();
}
//====================================================================================================================

//===========================================================
//�X�V����
//===========================================================	
void CAIModel::Update()
{
	CObjectX::Update();
}
//====================================================================================================================

//===========================================================
//�`�揈��
//===========================================================	
void CAIModel::Draw()
{
	CObjectX::Draw();
}
//====================================================================================================================

//===========================================================
//�`�揈���i�I�u�W�F�N�g��Draw�ŌĂт����Ȃ��̂ŕʘg�j
//===========================================================	
void CAIModel::ExtraDraw()
{
	//CObjectX::Draw();
}
//====================================================================================================================

//===========================================================
//���S�t���O�ݒ菈��
//===========================================================	
void CAIModel::SetDeath()
{
	CObject::SetDeath();
}
//====================================================================================================================

CAIModel* CAIModel::Create(AIMODELTYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale)
{
	CAIModel* pAiModel = DBG_NEW CAIModel;   //�G�𐶐�
	bool bSuccess = pAiModel->CObject::GetCreateSuccess();
	int nIdx = 0;//���f���̃C���f�b�N�X
	if (bSuccess == true)
	{
		if (pAiModel != nullptr)
		{
			pAiModel->Init();         //����������
			pAiModel->SetUseDeath(false);                                                   //���S�t���O�𔭓����邩�ǂ�����ݒ肷��
			pAiModel->m_Type = type;  //�G�̎��
			pAiModel->SetLife(1);      //�̗�
			pAiModel->SetMaxLife(1);   //��̗�
			pAiModel->SetAutoSubLife(false);//�����I�ɑ̗͂����炷���ǂ���
			pAiModel->CObjectX::SetPos(pos);                                                                                  //�I�u�W�F�N�g�Q�c�̈ʒu��ݒ�
			pAiModel->SetPosOld(pos);                               //1f�O�̈ʒu��ݒ�
			pAiModel->SetUseDraw(false);
			pAiModel->SetSupportPos(pos);                                                                                     //�ݒu�ʒu
			pAiModel->CObject::SetType(CObject::TYPE_AIMODEL);                                                                //�I�u�W�F�N�g�̎�ނ����߂�
			pAiModel->CObjectX::SetScale(Scale);                                                                              //�I�u�W�F�N�g�̊g�嗦
			pAiModel->SetFormarScale(Scale);                                                                                  //���̊g�嗦��ݒ肷��
			nIdx = CManager::GetObjectXInfo()->Regist(CObjectX::OBJECTXTYPE_AIMODEL, m_apAIMODEL_FILENAME[(int)(type)]);
			pAiModel->CObjectX::BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(CObjectX::OBJECTXTYPE_AIMODEL, nIdx),
				CManager::GetObjectXInfo()->GetBuffMat(CObjectX::OBJECTXTYPE_AIMODEL, nIdx),
				CManager::GetObjectXInfo()->GetdwNumMat(CObjectX::OBJECTXTYPE_AIMODEL, nIdx),
				CManager::GetObjectXInfo()->GetTexture(CObjectX::OBJECTXTYPE_AIMODEL, nIdx),
				CManager::GetObjectXInfo()->GetColorValue(CObjectX::OBJECTXTYPE_AIMODEL, nIdx));                       //���f���������蓖�Ă�

			pAiModel->CObjectX::SetObjXType(CObjectX::OBJECTXTYPE_AIMODEL);                          //�I�u�W�F�N�gX�̃^�C�v��ݒ�
			pAiModel->CObjectX::SetTypeNum((int)(type));                                             //�I�u�W�F�N�gX���Ƃ̃^�C�v�ԍ���ݒ�
			pAiModel->SetSize();                                                                     //X�I�u�W�F�N�g�̃T�C�Y��ݒ肷��
			pAiModel->CObjectX::SetRotType(CObjectX::ROTTYPE::ROTTYPE_NORMAL);                       //�����̎�ނ�ݒ�
		}
		else
		{//�I�u�W�F�N�g�ɋ󂫂��Ȃ������̂Ŕj������
			delete pAiModel;
			pAiModel = nullptr;
			return nullptr;
		}
	}
	return pAiModel;
}
//====================================================================================================================