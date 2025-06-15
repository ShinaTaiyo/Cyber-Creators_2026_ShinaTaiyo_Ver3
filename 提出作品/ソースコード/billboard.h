//===================================================================================================================
//
// �Q�O�Q�T�N�U���P���F�R�[�h���Y��ɂ���[bgModel.h]
// Author:ShinaTaiyo
//
//===================================================================================================================

//*******************************************************************************************************************
// �Q�d�C���N���[�h�h�~
//*******************************************************************************************************************
#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

//*******************************************************************************************************************
// �C���N���[�h
//*******************************************************************************************************************
#include "main.h"
#include "object.h"

//*******************************************************************************************************************
// �r���{�[�h�N���X
//*******************************************************************************************************************
class CBillboard : public CObject
{
public:
	// === ����֐� ===

	// �R���X�g���N�^
	CBillboard(
		int nPri = 6, // �����Ō��߂�v���C�I���e�B
		bool bUseintPri = false, // �����Ńv���C�I���e�B�����߂邩�ǂ���
		CObject::TYPE type = CObject::TYPE::NONE, // �^�C�v
		CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_BILLBOARD // �I�u�W�F�N�g�^�C�v
	);

	~CBillboard(); // �f�X�g���N�^
     
	// === ���C�t�T�C�N�������o�֐� ===

	HRESULT Init(void) override; //  ����������
	void Uninit(void) override;  //  �I������
	void Update(void) override;  //  �X�V����
	void Draw(void) override;    //  �`�揈��
	void SetDeath() override;    //  ���S�t���O�ݒ菈��

	// === �����o�֐� ===

	void bindTexture(LPDIRECT3DTEXTURE9 pTexture); // �e�N�X�`�������蓖�Ă�

	// �ʒu
	const D3DXVECTOR3& GetPos() const; // �擾
	void SetPos(D3DXVECTOR3 pos);      // �ݒ�

	// 1f�O�̈ʒu
	const D3DXVECTOR3& GetPosOld() const; // �擾
	void SetPosOld(D3DXVECTOR3 PosOld);   // �ݒ�

	// �����ʒu
	const D3DXVECTOR3& GetSupportPos() const; // �擾
	void SetSupportPos(D3DXVECTOR3 Pos);      // �ݒ�

	// �ړ���
	void SetMove(D3DXVECTOR3 Move) { m_Move = Move; } // �ݒ�
	D3DXVECTOR3& GetMove() { return m_Move; }         // �擾

	// �T�C�Y
	const D3DXVECTOR3& GetSize() const; // �擾
	void SetSize(D3DXVECTOR3 Size);     // �ݒ�

	// ���̃T�C�Y
	const D3DXVECTOR3& GetFormarSize() const;   // �擾
	void SetFormarSize(D3DXVECTOR3 FormarSize); // �ݒ�

	// �g�嗦
	void SetScale(D3DXVECTOR3 Scale);    // �ݒ�
	const D3DXVECTOR3& GetScale() const; // �擾

	// ���Z�g�嗦
	void SetUseAddScale(bool bUse, D3DXVECTOR3 AddScale); // �ݒ�
	const D3DXVECTOR3& GetAddScale() const; // �擾

	// �F����
	void SetColor(D3DXCOLOR col);       // �ݒ�
	const D3DXCOLOR & GetColor() const; // �擾

	// �e�N�X�`���ԍ�
	void SetTextureIndex(int nIdx);      // �ݒ�
	const int & GetTextureIndex() const; // �擾
	void SetAnimInfo(int nMaxAnimationPattern, int nAnimationChange,D3DXCOLOR col,bool bAnim); // �A�j���[�V�����֌W�ݒ�
	void SetAnim(int nAnim) { m_nAnimaionPattern = nAnim; } // �A�j���[�V�����p�^�[���ݒ�
	D3DXMATRIX& GetMtxWorld() { return m_mtxWorld; } // ���[���h�}�g���b�N�X�擾
	LPDIRECT3DVERTEXBUFFER9& GetVtxBuff() { return m_pVtxBuff; } // ���_�o�b�t�@�擾
	LPDIRECT3DTEXTURE9& GetTexture() { return m_pTexture; }      // �e�N�X�`���擾

	void SetUseGravity(float fGravity); // �d�͂�ݒ�
	void SetAddGravity(float fAddGravity); // ���Z�d�͐ݒ�
	void SetUseAddSpeed(bool bUseAddSpeed, bool bMultiplication, float fAddSpeed); // �������邩�ǂ����ݒ�
	void SetUsePolygonRot(bool bUse, float fRotPower); // �|���S������]�����邩�ǂ�����ݒ�

	// �`�悷�邩�ǂ���
	const bool& GetUseDraw() const; // �擾
	void SetUseDraw(bool bDraw);    // �ݒ�

	// �̗�
	const int& GetLife() const; // �擾
	void SetLife(int nLife);    // �ݒ�

	// �ő�̗�
	const int& GetMaxLife() const; // �擾
	void SetMaxLife(int nLife);    // �ݒ�
private:	
	// === �����o�ϐ� ===

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff; // ���_�o�b�t�@�ւ̃|�C���^

	LPDIRECT3DTEXTURE9 m_pTexture;      // �e�N�X�`���ւ̃|�C���^

	D3DXVECTOR3
		m_Pos,    // �ʒu
		m_PosOld, // 1f�O�̈ʒu
		m_Move,   // �ړ���
		m_Rot,    // ����!
		m_Scale,  // �g�嗦
		m_AddScale,   // ���Z�g�嗦
		m_SupportPos, // �����ʒu��ݒ�
		m_Size,       // �T�C�Y
		m_FormarSize; // ���̃T�C�Y

	D3DXMATRIX m_mtxWorld; // ���[���h�ϊ��s��

	D3DXCOLOR m_Col; // �F����

	int
		m_nLife,    // �̗�
		m_nMaxLife, // �ő�̗�
		m_nAnimationCnt,    //�A�j���[�V�����J�E���g
		m_nAnimaionPattern, // �A�j���[�V�����p�^�[��
		m_nMaxAnimationPattern, // �A�j���[�V�����p�^�[���ő吔
		m_nAnimationChange,     // �A�j���[�V������ς���J�E���g��
		m_nTextureIndex, // �e�N�X�`���ԍ�!
		m_nCntTime;      // �o�����Ă���̎��ԃJ�E���g

	float
		m_fSpeed,           // ���x
		m_fAnimationSplit,  //�A�j���[�V�����P����������̒l
		m_fAddRot,          // �����̕␳
		m_fStartRot,        // �ŏ��̌���
		m_fPolygonRotPower, // �|���S���̉�]��
		m_fGravityPower, // �d�͂̑傫��
		m_fAddGravity,   // ���Z����d��
		m_fGravity,      // �d��
		m_fAddSpeed;     // �����x

	bool 
        m_bDraw,           // �`�悷�邩�ǂ���
        m_bAnimFlag,       // �A�j���[�V���������邩�ǂ���
        m_bUseGravity,     // �d�͂��g�p���邩�ǂ���
		m_bUseAddScale,    // ���Z�g����g�p���邩�ǂ���
        m_bUseAddSpeed,    // �������g�p���邩�ǂ���
        m_bUsePolygonRot,  // �|���S������]�����邩�ǂ���
        m_bMultiplication; // ���x����Z�����邩�ǂ���
};


#endif