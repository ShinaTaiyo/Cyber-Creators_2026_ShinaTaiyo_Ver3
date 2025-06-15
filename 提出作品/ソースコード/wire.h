//===================================================================================================================
//
// �Q�O�Q�T�N�U���P�U���F�v�Z�p�N���X�𖼑O��Ԃɂ���[bg.h]
// Author:ShinaTaiyo
//
//===================================================================================================================

//*******************************************************************************************************************
// �Q�d�C���N���[�h�h�~
//*******************************************************************************************************************
#ifndef _WIRE_H_
#define _WIRE_H_

//*******************************************************************************************************************
// �C���N���[�h
//*******************************************************************************************************************
#include "main.h"
#include "meshcylinder.h"

//*******************************************************************************************************************
// �O���錾
//*******************************************************************************************************************
class CPlayer;   // �v���C���[
class CWireHead; // ���C���[�w�b�h

//*******************************************************************************************************************
// ���C���[�N���X
//*******************************************************************************************************************
class CWire : public CMeshCylinder
{
public:
	// === �񋓌^ ===

	// ���C���[���
	enum class WIRETYPE
	{
		NORMAL = 0,
		ROPE,
		MAX
	};

	// === ����֐� ===

	// �R���X�g���N�^
	CWire(
		WIRETYPE WireType, // ���C���[���
		float fRadius,     // ���a 
		float fHeight,     // ����
		int nNumDivsionXZ, // XZ������
		D3DXVECTOR3 Pos,   // �ʒu
		D3DXVECTOR3 Rot,   // ����
		int nNumDivisionY, // �c������
		int nPri = 0,      // �����Ō��߂�v���C�I���e�B
		bool bUseintPri = false, // �v���C�I���e�B�������Ō��߂邩�ǂ���
		CObject::TYPE type = CObject::TYPE::WIRE, // �^�C�v
		CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_3D // �I�u�W�F�N�g�^�C�v
	);

	~CWire() override; // �f�X�g���N�^

	// === ���C�t�T�C�N�������o�֐� ===

	HRESULT Init() override;  // ����������
	void Uninit() override;   // �I������
	void Update() override;   // �X�V����
	void Draw() override;     // �`�揈��
	void SetDeath() override; // ���S�t���O�ݒ菈��

	// === �����o�֐� ===

	// ���C���[�̓��擾
	CWireHead* GetWireHead();

	// �v���C���[�̃|�C���^�ݒ�
	void SetPlayerPointer(CPlayer* pPlayer);

	// �X�V���邩�ǂ���
	void SetUseUpdate(bool bUse);

	// === �ÓI�����o�֐� ===

	// ��������
	static CWire* Create(
		WIRETYPE Type,   // ���C���[���
		D3DXVECTOR3 Pos, // �ʒu 
		D3DXVECTOR3 Rot, // ����
		float fRadius,   // ���a 
		float fHeight,   // ����
		int nNumDivisionXZ, // XZ������ 
		int nNumDivisionY   // Y������
	);

protected:
	static const string s_WIRE_FILENAME[static_cast<int>(WIRETYPE::MAX)]; // ���C���[�e�N�X�`���t�@�C����
private:
	// === �����o�ϐ� ===

	WIRETYPE m_Type;    // ���C���[�̎��
	CPlayer* m_pPlayer; // �v���C���[�ւ̃|�C���^
	bool m_bUseUpdate;  // �X�V���邩�ǂ���
	CWireHead* m_pWireHead; // ���C���[�̓�

	// === �����o�֐� ===

	void GenerateCylindricalMeshOnWire(); // ���C���[��Ƀ��b�V����W�J
};
#endif // !_WIRE_H_