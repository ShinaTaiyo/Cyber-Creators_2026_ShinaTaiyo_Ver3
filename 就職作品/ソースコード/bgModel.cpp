//======================================================================================================================
//
//�P�O���Q�S���F�U���p�̃N���X�����[BgModel.cpp]
//Author:ShinaTaiyo
//
//======================================================================================================================

//==================================================================
//�C���N���[�h
//==================================================================
#include "bgModel.h"
#include "manager.h"
#include "objectX.h"
#include "objectXInfo.h"
//======================================================================================================================


//==================================================================
//�O���錾
//==================================================================
const string CBgModel::BGMODEL_FILENAME[static_cast<int>(CBgModel::BGMODELTYPE::MAX)] =
{//�w�i���f���̃t�@�C����
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
//======================================================================================================================

//==================================================================
//�R���X�g���N�^
//==================================================================
CBgModel::CBgModel(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : m_Type(BGMODELTYPE::BILL_00),
CObjectX(nPri,bUseintPri,type,ObjType)
{

}
//======================================================================================================================

//==================================================================
//�f�X�g���N�^
//==================================================================
CBgModel::~CBgModel()
{

}
//======================================================================================================================

//==================================================================
//����������
//==================================================================
HRESULT CBgModel::Init()
{
	CObjectX::Init();//�I�u�W�F�N�gX�̏���������
	return S_OK;
}
//======================================================================================================================

//==================================================================
//�I������
//==================================================================
void CBgModel::Uninit()
{
	CObjectX::Uninit();//�I�u�W�F�N�gX�̏I������
}
//======================================================================================================================

//==================================================================
//�X�V����
//==================================================================
void CBgModel::Update()
{
	CObjectX::Update();//�I�u�W�F�N�gX�̍X�V����
}
//======================================================================================================================

//==================================================================
//�`�揈��
//==================================================================
void CBgModel::Draw()
{
	CObjectX::Draw();//�I�u�W�F�N�gX�̕`�揈��
}
//======================================================================================================================

//==================================================================
//���S�t���O�ݒ菈��
//==================================================================
void CBgModel::SetDeath()
{
	CObjectX::SetDeath();//���S�t���O�ݒ菈��
}
//======================================================================================================================

//==================================================================
//��������
//==================================================================
CBgModel* CBgModel::Create(BGMODELTYPE bgModelType, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale, bool bSwapVtxXZ)
{
	CBgModel* pBgModel = DBG_NEW CBgModel();              //����

	pBgModel->Init();                                     //����������
	pBgModel->SetBgModelType(bgModelType);                //�w�i���f���̎�ނ�ݒ肷��
	pBgModel->GetPosInfo().SetPos(pos);                   //�ʒu��ݒ�  
	pBgModel->GetPosInfo().SetSupportPos(pos);            //�x�_�ƂȂ�ʒu��ݒ�
	pBgModel->GetRotInfo().SetRot(rot);                   //������ݒ�
	pBgModel->GetSizeInfo().SetScale(Scale);              //�g�嗦��ݒ�
	pBgModel->GetSizeInfo().SetUseSwapVtxXZ(bSwapVtxXZ);  //XZ�̒��_�̃T�C�Y�����ւ��邩�ǂ���
	pBgModel->GetDrawInfo().SetUseShadow(false);          //�e���g�p���Ȃ�

	pBgModel->SetManagerObjectType(CObject::MANAGEROBJECTTYPE::BGMODEL);//�X�e�[�W�}�l�[�W���[�ŌĂяo�����̎�ނ�ݒ�

	if (bSwapVtxXZ == true)
	{//XZ�̃T�C�Y�����ւ���Ȃ�
		D3DXVECTOR3 VtxMax = pBgModel->GetSizeInfo().GetOriginVtxMax();//���̍ő咸�_���擾
		D3DXVECTOR3 VtxMin = pBgModel->GetSizeInfo().GetOriginVtxMin();//���̍ŏ����_���擾

		VtxMax.x = pBgModel->GetSizeInfo().GetOriginVtxMax().z;//�ő咸�_��X��Z��
		VtxMax.z = pBgModel->GetSizeInfo().GetOriginVtxMax().x;//�ő咸�_��Z��X��
		VtxMin.x = pBgModel->GetSizeInfo().GetOriginVtxMin().z;//�ŏ����_��X��Z��
		VtxMin.z = pBgModel->GetSizeInfo().GetOriginVtxMin().x;//�ŏ����_��Z��X��
		pBgModel->GetSizeInfo().SetOriginVtxMax(VtxMax);       //���̍ő咸�_�����ݐݒ肵�����ɍX�V
		pBgModel->GetSizeInfo().SetOriginVtxMin(VtxMin);       //���̍ŏ����_�����ݐݒ肵�����ɍX�V
	}
	return pBgModel;
}
//======================================================================================================================


//==================================================================
//�X�e�[�W�}�l�[�W���[�ɏ���ۑ�����
//==================================================================
void CBgModel::SaveInfoTxt(fstream& WritingFile)
{
	WritingFile << "SETBGMODEL" << endl;
	WritingFile << "TYPE = " << static_cast<int>(m_Type);
	switch (m_Type)
	{
	case BGMODELTYPE::BILL_00:
		WritingFile << " # BILL00" << endl;
		break;
	case BGMODELTYPE::TREE_00:
		WritingFile << " # TREE00" << endl;
		break;
	default:
		break;
	}

	CObjectX::SaveInfoTxt(WritingFile);

	WritingFile << "END_SETBGMODEL" << endl;

}
//======================================================================================================================

////==================================================================
////�X�e�[�W�}�l�[�W���[�������ǂݍ���
////==================================================================
//void CBgModel::LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff)
//{
//	int nType = 0;//���
//	D3DXVECTOR3 Move = D3DXVECTOR3(0.0f,0.0f,0.0f);  //�ړ���
//	D3DXVECTOR3 Pos = D3DXVECTOR3(0.0f,0.0f,0.0f);   //�ʒu
//	D3DXVECTOR3 Scale = D3DXVECTOR3(0.0f,0.0f,0.0f); //�g�嗦
//	D3DXVECTOR3 Rot = D3DXVECTOR3(0.0f,0.0f,0.0f);   //����
//	BGMODELTYPE Type = {};            //�w�i���f���̎��
//	bool bSwapVtxXZ = false;//XZ�̒��_��
//	while (Buff != "END_SETBGMODEL")
//	{
//		LoadingFile >> Buff;//�P���ǂݍ���
//		if (Buff == "#")
//		{
//			getline(LoadingFile, Buff);
//		}
//		else if (Buff == "TYPE")
//		{
//			LoadingFile >> Buff;//�C�R�[��
//			LoadingFile >> nType;      //���
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
//			LoadingFile >> Rot.x;      //�ʒuX
//			LoadingFile >> Rot.y;      //�ʒuY
//			LoadingFile >> Rot.z;      //�ʒuZ
//		}
//		else if (Buff == "SCALE")
//		{
//			LoadingFile >> Buff;//�C�R�[��
//			LoadingFile >> Scale.x;      //�g�嗦X
//			LoadingFile >> Scale.y;      //�g�嗦Y
//			LoadingFile >> Scale.z;      //�g�嗦Z
//		}
//		else if (Buff == "SWAPVTXXZ")
//		{
//			LoadingFile >> Buff;//�C�R�[��
//			LoadingFile >> bSwapVtxXZ;
//		}
//	}
//	Type = BGMODELTYPE(nType);
//
//	listSaveManager.push_back(CBgModel::Create(Type,Pos, Rot, Scale,bSwapVtxXZ));//vector�ɏ���ۑ�����
//
//}
////======================================================================================================================

//==================================================================
//�X�e�[�W�}�l�[�W���[�������ǂݍ���
//==================================================================
void CBgModel::LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff, CObject* pObj)
{
	int nType = 0;                                       //���
	BGMODELTYPE Type = {};                               //�w�i���f���̎��
	while (Buff != "END_SETBGMODEL")
	{
		LoadingFile >> Buff;//�P���ǂݍ���
		if (Buff == "#")
		{
			getline(LoadingFile, Buff);
		}
		else if (Buff == "TYPE")
		{
			LoadingFile >> Buff;                    //�C�R�[��
			LoadingFile >> nType;                   //���
			Type = static_cast<BGMODELTYPE>(nType); //�w�i���f���̃^�C�v�ɃL���X�g
			SetBgModelType(Type);                   //�w�i���f���̃^�C�v��ݒ�
		}
		else if (Buff == "SETOBJECTX")
		{//�I�u�W�F�N�gX�̏���ݒ肷��
			CObjectX::LoadInfoTxt(LoadingFile, listSaveManager, Buff, this);
		}
	}
	listSaveManager.push_back(this);//�X�e�[�W�}�l�[�W���[�̃I�u�W�F�N�g���X�g�ɏ���o�^����
}
//======================================================================================================================

//==================================================================
//��ނ�ς���
//==================================================================
CObject* CBgModel::ManagerChengeObject(bool bAim)
{
	int nNewType = int(m_Type);
	BGMODELTYPE NewType = {};
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
	if (nNewType >= static_cast<int>(BGMODELTYPE::MAX))
	{
		nNewType = 0;
	}
	if (nNewType < 0)
	{
		nNewType = static_cast<int>(BGMODELTYPE::MAX) - 1;
	}
	//======================================================================================

	//=======================================
	//�ݒ肷��
	//=======================================
	NewType = BGMODELTYPE(nNewType);
	//======================================================================================

	//=======================================
	//���S�t���O��ݒ肷��
	//=======================================
	SetUseDeath(true);
	SetDeath();
	//======================================================================================

	return CBgModel::Create(NewType, GetPosInfo().GetPos(), GetRotInfo().GetRot(), GetSizeInfo().GetScale(),GetSizeInfo().GetUseSwapVtxXZ());//���������I�u�W�F�N�g��Ԃ�
}
//======================================================================================================================

//==================================================================
//����ۑ�����
//==================================================================
CObject* CBgModel::ManagerSaveObject()
{
	return CBgModel::Create(m_Type, GetPosInfo().GetPos(), GetRotInfo().GetRot(), GetSizeInfo().GetScale(), GetSizeInfo().GetUseSwapVtxXZ());//���������I�u�W�F�N�g��Ԃ�
}
//======================================================================================================================

//==================================================================
//�w�i���f���̎�ނ�ݒ肷��
//==================================================================
void CBgModel::SetBgModelType(BGMODELTYPE Type)
{
	int nType = static_cast<int>(Type);//�^�C�v�ԍ����擾

	if (nType < 0 || nType >= static_cast<int>(BGMODELTYPE::MAX))
	{
		//��O����
		assert("�w�i���f���Ŕz��O�A�N�Z�X�I");
	}
	else
	{
		Uninit();
		//���f�����ݒ�
		int nIdx = CManager::GetObjectXInfo()->Regist(BGMODEL_FILENAME[static_cast<int>(nType)]);

		//���f���������蓖�Ă�
		BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(nIdx),
			CManager::GetObjectXInfo()->GetBuffMat(nIdx),
			CManager::GetObjectXInfo()->GetdwNumMat(nIdx),
			CManager::GetObjectXInfo()->GetTexture(nIdx),
			CManager::GetObjectXInfo()->GetColorValue(nIdx));

		SetSize();//�T�C�Y��ݒ肷��

	}
}
//======================================================================================================================
