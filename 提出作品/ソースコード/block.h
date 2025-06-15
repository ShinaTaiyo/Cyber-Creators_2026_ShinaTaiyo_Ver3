//===================================================================================================================
//
// �Q�O�Q�T�N�U���R���F�R�[�h���Y��ɂ���[block.h]
// Author:ShinaTaiyo
//
//===================================================================================================================

//*******************************************************************************************************************
// �Q�d�C���N���[�h�h�~
//*******************************************************************************************************************
#ifndef _BLOCK_H_  
#define _BLOCK_H_

//*******************************************************************************************************************
// �C���N���[�h
//*******************************************************************************************************************
#include "main.h"
#include "objectX.h"

//*******************************************************************************************************************
// �u���b�N�N���X
//*******************************************************************************************************************
class CBlock : public CObjectX
{
public:
	// === �񋓌^ ===

	// ���
	enum class TYPE
	{
		NORMAL = 0, // ���ʃu���b�N
		WATER,      // ���u���b�N
		RENGA,      // �����K�u���b�N
		BLACK,      // ���u���b�N
		MAX         // �ő�
	};

	// === ����֐� ===

	// �R���X�g���N�^
	CBlock(
		int nPri = 0, // �����Ō��߂�v���C�I���e�B
		bool bUseintPri = false, // �����Ńv���C�I���e�B�����߂邩�ǂ���
		CObject::TYPE type = CObject::TYPE::BLOCK, // �^�C�v
		CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_X // �I�u�W�F�N�g�^�C�v
	);
	~CBlock() override;       // �f�X�g���N�^

	// === ���C�t�T�C�N�������o�֐� ===

	HRESULT Init() override;  // ����������
	void Uninit() override;   // �I������
	void Update() override;   // �X�V����
	void Draw() override;     // �`�揈��
	void SetDeath() override; // ���S�t���O��ݒ�

	// === �����o�֐� ===

	void SetType(TYPE Type); // �u���b�N�̃^�C�v��ݒ肷��
	const TYPE & GetType();  // �u���b�N�̎�ނ��擾����
	const bool& GetCollision(); // ����\���ǂ������擾����     
	void SaveInfoTxt(fstream& WritingFile) override; // �e�L�X�g�t�@�C���ɏ���ۑ����邽�߂̊֐�

	// �e�L�X�g�t�@�C���������ǂݍ��ނ��߂̊֐�
	void LoadInfoTxt(
		fstream& LoadingFile, // �t�@�C���ǂݍ��ݗp
		list<CObject*>& listSaveManager, // �X�e�[�W�}�l�[�W���[�I�u�W�F�N�g���X�g
		string& Buff, // ������
		CObject* pObj // �ҏW�I�u�W�F�N�g�ւ̃|�C���^
	) override;

	CObject* ManagerChengeObject(bool bAim) override; // �X�e�[�W�}�l�[�W���[�ɕύX�����I�u�W�F�N�g��n��
	CObject* ManagerSaveObject() override; //�X�e�[�W�}�l�[�W���[�ɍ��̃I�u�W�F�N�g��ۑ�����

	// === �ÓI�����o�֐� ===

	// ����
	static CBlock * Create(
		TYPE type, int nLife,D3DXVECTOR3 pos,D3DXVECTOR3 rot,D3DXVECTOR3 scale,
		bool bSwapVtxXZ // ���_XZ�����ւ��邩�ǂ���
	);
private:
	// === �ÓI�����o�ϐ� ===

	static const string s_BlockModelFileName[static_cast<int>(TYPE::MAX)]; // �u���b�N�̃��f���t�@�C����
	static const float m_fBLOCKCORRECTIONCOLLISION; // ����ɗ]�T�����������

	// === �����o�ϐ� ===

	TYPE m_type;       // �u���b�N�̎��
};

#endif
