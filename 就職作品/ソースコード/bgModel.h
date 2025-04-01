//====================================================
//
//�P�O���Q�S���F�U���N���X����������[attack.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _BGMODEL_H_  
#define _BGMODEL_H_

//======================
//�C���N���[�h
//======================
#include "main.h"
#include "objectX.h"
//==========================================

//===========================================
//�w�i���f���N���X
//===========================================
class CBgModel : public CObjectX
{
public:
	enum class BGMODELTYPE
	{//�w�i���f���̎��
		BILL_00 = 0,//�r���O
		TREE_00,    //��	
		BILL_01,    //�r���P
		BILL_02,    //�r���Q
		BILL_03,    //�r���R
		BILL_04,    //�r���S
		CHAIN_00,   //�`�F�[��
		HOUSE_00,   //�ƂO
		GRASS_00,   //��
		GRASSGROUND_00,//���n��
		LOG_00,     //�ۑ�
		MAX
	};

	CBgModel(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::BGMODEL, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_X);//�R���X�g���N�^
	~CBgModel();                                                                                                             //�f�X�g���N�^
	HRESULT Init() override;                                                                                                 //����������
	void Uninit() override;                                                                                                  //�I������
	void Update() override;                                                                                                  //�X�V����
	void Draw() override;                                                                                                    //�`�揈��
	void SetDeath() override;                                                                                                //���S�t���O��ݒ�
	static CBgModel* Create(BGMODELTYPE bgModelType,D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale,bool bSwapVtxXZ);    //��������

	//==========================================================
	//�G�f�B�^�֌W
	//==========================================================
	//�֐�
	void SaveInfoTxt(fstream& WritingFile) override;                                                               //�e�L�X�g�t�@�C���ɏ���ۑ����邽�߂̊֐�
	void LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff, CObject* pObj) override; //�e�L�X�g�t�@�C���������ǂݍ��ނ��߂̊֐�
	CObject* ManagerChengeObject(bool bAim) override;                                                              //�X�e�[�W�}�l�[�W���[�ɕύX�����I�u�W�F�N�g��n��
	CObject* ManagerSaveObject() override;                                                                         //�X�e�[�W�}�l�[�W���[�ɍ��̃I�u�W�F�N�g��ۑ�����
	//=================================================================================================================
protected:
	void SetBgModelType(BGMODELTYPE Type);                                                         //�w�i���f���̎�ނ�ݒ肷��
	const BGMODELTYPE& GetBgModelType() const { return m_Type; }                                   //�w�i���f���̎�ނ��擾����
	static const string BGMODEL_FILENAME[static_cast<int>(BGMODELTYPE::MAX)];                      //�w�i���f���̃t�@�C���� 
private:
	BGMODELTYPE m_Type;                                                                            //�w�i���f���̎��
};
//==================================================================================================================================================
#endif