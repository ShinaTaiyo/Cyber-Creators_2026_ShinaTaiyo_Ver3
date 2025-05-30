//===================================================================================================================
//
// �Q�O�Q�T�N�T���Q�T���F�R�[�h���Y��ɂ���[bg.h]
// Author:ShinaTaiyo
//
//===================================================================================================================

//*******************************************************************************************************************
// �Q�d�C���N���[�h�h�~
//*******************************************************************************************************************
#ifndef _BG_H_  
#define _BG_H_

//*******************************************************************************************************************
// �C���N���[�h
//*******************************************************************************************************************
#include "main.h"     // ���C��
#include "object2d.h" // �I�u�W�F�N�g2D

//*******************************************************************************************************************
// �w�i�N���X
//*******************************************************************************************************************
class CBg :  public CObject2D
{
public:
	// === �񋓌^ ===

	// �w�i��ޗ񋓌^
	enum class TYPE
	{
		TITLE = 0,  // �^�C�g���w�i
		DIFFICULTY, // ��Փx�I��w�i
		RESULT,     // ���U���g�w�i
		MAX // �ő�
	}
	;
	// === ����֐� ===

	//�R���X�g���N�^
	CBg(
		int nPri = 0, // �����Ō��߂�int�^�v���C�I���e�B
		bool bUseintPri = false, // �v���C�I���e�B�������Ō��߂邩�ǂ���
		CObject::TYPE type = CObject::TYPE::BG, // �^�C�v
		CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_2D // �I�u�W�F�N�g�^�C�v
	);
	~CBg() override; // �f�X�g���N�^

	// === ���C�t�T�C�N�������o�֐� ===

	HRESULT Init() override;  // ����������
	void Uninit() override;   // �I������
	void Update() override;   // �X�V����
	void Draw() override;     // �`�揈��
	void SetDeath() override; // ���S�t���O��ݒ�

	// === �����o�֐� ===

	// �w�i�̎��
	const TYPE& GetType();   // �擾
	void SetType(TYPE type); // �ݒ�

	// ��]���x
	void SetRotationPower(float fPower); // �ݒ�
	float& GetRotationPower(); // �擾

	// �e�N�X�`�����ړ������邩�ǂ���
	void SetUseTexMove(
		bool bUse, // �g�p���邩�ǂ���
		D3DXVECTOR2 Pos, // �ʒu
		D3DXVECTOR2 Move // �ړ���
	);

	void SetPosTex(D3DXVECTOR2 Pos); // �e�N�X�`�����W��ݒ肷��
	void SetMoveTex(D3DXVECTOR2 Move); // �e�N�X�`���̈ړ��ʂ�ݒ肷��

	// === �ÓI�����o�֐� ===

	static void Create(TYPE type); // �w�i�̐���
private:
	// === �ÓI�����o�ϐ� ===

	static const std::string m_apBGFILE_NAME[static_cast<int>(TYPE::MAX)]; // �w�i�̃e�N�X�`���t�@�C����

	// === �����o�ϐ� ===

	bool m_bUseTexMove; // �w�i���ړ������邩�ǂ���
	float m_fPosTexU,   // �e�N�X�`�����W
	      m_fRotationPower;  // ��]��
	D3DXVECTOR2 m_PosTex,    // �e�N�X�`�����W
	            m_MoveTex;   // �e�N�X�`���ړ���
	TYPE m_Type; // �w�i�̎��
};

#endif
