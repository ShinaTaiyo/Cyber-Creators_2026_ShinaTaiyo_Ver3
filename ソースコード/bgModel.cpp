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
	CObjectX::Init();
	return S_OK;
}
//======================================================================================================================

//==================================================================
//�I������
//==================================================================
void CBgModel::Uninit()
{
	CObjectX::Uninit();
}
//======================================================================================================================

//==================================================================
//�X�V����
//==================================================================
void CBgModel::Update()
{
	CObjectX::Update();
}
//======================================================================================================================

//==================================================================
//�`�揈��
//==================================================================
void CBgModel::Draw()
{
	CObjectX::Draw();
}
//======================================================================================================================

//==================================================================
//���S�t���O�ݒ菈��
//==================================================================
void CBgModel::SetDeath()
{
	CObject::SetDeath();
}
//======================================================================================================================

//==================================================================
//��������
//==================================================================
CBgModel* CBgModel::Create(BGMODELTYPE bgModelType, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale, bool bSwapVtxXZ)
{
	CBgModel* pBgModel = DBG_NEW CBgModel();     //����

	pBgModel->Init();                        //����������
	pBgModel->SetBgModelType(bgModelType);   //�w�i���f���̎�ނ�ݒ肷��
	pBgModel->GetPosInfo().SetPos(pos);                   //�ʒu  
	pBgModel->GetPosInfo().SetSupportPos(pos);            //�x�_�ƂȂ�ʒu��ݒ�
	pBgModel->GetRotInfo().SetRot(rot);                   //����
	pBgModel->GetSizeInfo().SetScale(Scale);             //�g�嗦
	pBgModel->GetSizeInfo().SetUseSwapVtxXZ(bSwapVtxXZ);   //XZ���`�F���W����
	pBgModel->GetDrawInfo().SetUseShadow(false);
	//���f�����ݒ�
	int nIdx = CManager::GetObjectXInfo()->Regist(BGMODEL_FILENAME[static_cast<int>(bgModelType)]);

	//���f���������蓖�Ă�
	pBgModel->BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(nIdx),
		CManager::GetObjectXInfo()->GetBuffMat(nIdx),
		CManager::GetObjectXInfo()->GetdwNumMat(nIdx),
		CManager::GetObjectXInfo()->GetTexture(nIdx),
		CManager::GetObjectXInfo()->GetColorValue(nIdx));

	pBgModel->SetManagerObjectType(CObject::MANAGEROBJECTTYPE::BGMODEL);           //�}�l�[�W���[�ŌĂяo�����̎�ނ�ݒ�
	pBgModel->SetSize();//�T�C�Y��ݒ肷��

	if (bSwapVtxXZ == true)
	{
		D3DXVECTOR3 VtxMax = pBgModel->GetSizeInfo().GetOriginVtxMax();
		D3DXVECTOR3 VtxMin = pBgModel->GetSizeInfo().GetOriginVtxMin();

		VtxMax.x = pBgModel->GetSizeInfo().GetOriginVtxMax().z;
		VtxMax.z = pBgModel->GetSizeInfo().GetOriginVtxMax().x;
		VtxMin.x = pBgModel->GetSizeInfo().GetOriginVtxMin().z;
		VtxMin.z = pBgModel->GetSizeInfo().GetOriginVtxMin().x;
		pBgModel->GetSizeInfo().SetOriginVtxMax(VtxMax);
		pBgModel->GetSizeInfo().SetOriginVtxMin(VtxMin);
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
	WritingFile << "TYPE = " << static_cast<int>(m_Type) << endl;
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

//==================================================================
//�X�e�[�W�}�l�[�W���[�������ǂݍ���
//==================================================================
void CBgModel::LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff)
{
	int nType = 0;//���
	D3DXVECTOR3 Move = D3DXVECTOR3(0.0f,0.0f,0.0f);  //�ړ���
	D3DXVECTOR3 Pos = D3DXVECTOR3(0.0f,0.0f,0.0f);   //�ʒu
	D3DXVECTOR3 Scale = D3DXVECTOR3(0.0f,0.0f,0.0f); //�g�嗦
	D3DXVECTOR3 Rot = D3DXVECTOR3(0.0f,0.0f,0.0f);   //����
	BGMODELTYPE Type = {};            //�w�i���f���̎��
	bool bSwapVtxXZ = false;//XZ�̒��_��
	while (Buff != "END_SETBGMODEL")
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
	Type = BGMODELTYPE(nType);

	listSaveManager.push_back(CBgModel::Create(Type,Pos, Rot, Scale,bSwapVtxXZ));//vector�ɏ���ۑ�����

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
