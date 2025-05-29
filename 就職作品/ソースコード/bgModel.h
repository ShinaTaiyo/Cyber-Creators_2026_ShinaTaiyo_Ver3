//===================================================================================================================
//
// �Q�O�Q�T�N�T���Q�T���F�R�[�h���Y��ɂ���[bgModel.h]
// Author:ShinaTaiyo
//
//===================================================================================================================

//*******************************************************************************************************************
// �Q�d�C���N���[�h�h�~
//*******************************************************************************************************************
#ifndef _BGMODEL_H_  
#define _BGMODEL_H_

//*******************************************************************************************************************
// �C���N���[�h
//*******************************************************************************************************************
#include "main.h"    // ���C��
#include "objectX.h" // �I�u�W�F�N�gX

//*******************************************************************************************************************
// �w�i���f���N���X
//*******************************************************************************************************************
class CBgModel : public CObjectX
{
public:
	// === �񋓌^ ===

	// �w�i���f���^�C�v
	enum class TYPE
	{
		BILL_00 = 0, //�r���O
		TREE_00,  //��	
		BILL_01,  //�r���P
		BILL_02,  //�r���Q
		BILL_03,  //�r���R
		BILL_04,  //�r���S
		CHAIN_00, //�`�F�[��
		HOUSE_00, //�ƂO
		GRASS_00, //��
		GRASSGROUND_00, //���n��
		LOG_00, //�ۑ�
		MAX // �ő�
	};

	// === ����֐� ===

	// �R���X�g���N�^
	CBgModel(
		int nPri = 0,// �����Ō��߂�int�^�v���C�I���e�B
		bool bUseintPri = false, // �����Ńv���C�I���e�B�����߂邩�ǂ���
		CObject::TYPE type = CObject::TYPE::BGMODEL, // �^�C�v
		CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_X // �I�u�W�F�N�g�^�C�v
	);
	~CBgModel(); // �f�X�g���N�^

	// === ���C�t�T�C�N���֐� ===

	HRESULT Init() override;  // ����������
	void Uninit() override;   // �I������
	void Update() override;   // �X�V����
	void Draw() override;     // �`�揈��
	void SetDeath() override; // ���S�t���O��ݒ�

	// === �����o�֐� ===

	void SaveInfoTxt(fstream& WritingFile) override; // �e�L�X�g�t�@�C���ɏ���ۑ����邽�߂̊֐�

	 // �e�L�X�g�t�@�C���������ǂݍ��ނ��߂̊֐�
	void LoadInfoTxt(
		fstream& LoadingFile, // �ǂݍ��݃t�@�C���ւ̃|�C���^
		list<CObject*>& listSaveManager, // �ǂݍ��݃I�u�W�F�N�g�ۑ��p���X�g�ւ̃|�C���^
		string& Buff, // ������
		CObject* pObj // �I�u�W�F�N�g�ւ̃|�C���^
	) override;

	CObject* ManagerChengeObject(bool bAim) override; // �X�e�[�W�}�l�[�W���[�ɕύX�����I�u�W�F�N�g��n��
	CObject* ManagerSaveObject() override; // �X�e�[�W�}�l�[�W���[�ɍ��̃I�u�W�F�N�g��ۑ�����

	// �w�i���f���̎��
	const TYPE& GetType() const; // �擾
	void SetType(TYPE Type);     //�ݒ�

	// === �ÓI�����o�֐� ===

	// ��������
	static CBgModel* Create(
		TYPE bgModelType,  // �w�i���f���^�C�v
		D3DXVECTOR3 pos,   // �ʒu
		D3DXVECTOR3 rot,   // ����
		D3DXVECTOR3 Scale, // �g�嗦
		bool bSwapVtxXZ    // ���_XZ�����ւ��邩�ǂ���
	);
private:
	// === �ÓI�����o�ϐ� ===

	static const string BGMODEL_FILENAME[static_cast<int>(TYPE::MAX)]; // ���f���t�@�C���� 

	// === �����o�ϐ� ===

	TYPE m_Type; // ���
};
//==================================================================================================================================================
#endif