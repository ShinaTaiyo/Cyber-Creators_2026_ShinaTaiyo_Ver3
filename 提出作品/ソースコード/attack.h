//===================================================================================================================
//
// �Q�O�Q�T�N�T���P�W���F�R�[�h�����₷������[attack.h]
// Author:ShinaTaiyo
//
//===================================================================================================================

//*******************************************************************************************************************
// �Q�d�C���N���[�h�h�~
//*******************************************************************************************************************
#ifndef _ATTACK_H_  
#define _ATTACK_H_

//*******************************************************************************************************************
// �C���N���[�h
//*******************************************************************************************************************
#include "main.h"    // ���C��
#include "objectX.h" // �I�u�W�F�N�gX

//===========================================
// �U���N���X
//===========================================
class CAttack : public CObjectX
{
public:
	// === �񋓌^ ===

	// �U���^�C�v
	enum class ATTACKTYPE
	{
		BULLET = 0, // �e
		EXPLOSION,  // ����
		MAGICSWORD, // ���@�̌�
		MAX         // �ő�
	};

	// ����^�C�v
	enum class COLLISIONTYPE
	{
		NONE = 0,     // �Ȃ�
		SQUARE,       // �����` 
		RECTANGLE_XZ, // ��]�����`XZ
		OBBTOOBB,     // OBB���m
		MAX
	};

	// ������I�u�W�F�N�g�^�C�v
	enum class TARGETTYPE
	{
		NONE = 0, // �Ȃ�
		PLAYER,   // �v���C���[
		ENEMY,    // �G
		MAX       // �ő�
	};

	// === �\���� ===

	// ���G��ԏ��i���G��Ԃ��q�b�g�X�g�b�v���Ɗ��Ⴂ���Ă��܂����B��X�����܂��B�j
	struct HitStop
	{
		const int nSetTime; // �����������ɗ^���閳�G��ԃt���[����
	};

	// �U�����o�E���h������ꍇ�̏��i�S�Ă̍U�����o�E���h����킯�Ȃ��̂ŁA��ɊǗ����@���C���\��j�i�f�t�H���g��public�Ȃ����œ��ɃN���X�ƕς��Ȃ��̂ŏC���\��j
	struct BoundInfo
	{
		// === �����o�֐� ===

		// �o�E���h�����邩�ǂ����ݒ�
		void SetActive (
			bool bCopyActive,      // �o�E���h�����邩�ǂ����@�@
			D3DXVECTOR3 CopyPower, // �o�E���h������� 
			bool bCopyGravity,     // �d�͂�t���邩�ǂ���
			float fCopyGravity)    // �d��
		{
			bActive = bCopyActive; Power = CopyPower; bGravity = bCopyGravity; fGravity = fCopyGravity;
		};
		void BoundProcess(CAttack * pAttack); // �o�E���h����
		const D3DXVECTOR3& GetPower() const { return Power; } // �o�E���h�͂��擾
		const bool& GetActive() const { return bActive; }     // �o�E���h���邩�ǂ������擾
	private:
		// === �����o�ϐ� ===

		bool bActive = false;  // �o�E���h���邩�ǂ���
		bool bGravity = false; // �o�E���h���ɏd�͂�ON�ɂ��邩�ǂ���
		float fGravity = 0.0f; // �o�E���h���ɏd�͂�ON�ɂ���ꍇ�ɐݒ肷��d��
		D3DXVECTOR3 Power = { 0.0f,0.0f,0.0f }; // �o�E���h��
	};

	// === ����֐� ===
	
	// �R���X�g���N�^
	CAttack(
		int nPower, // �З�
		int nSetHitStopTime, // ���̍U���Ŕ�e��������ɗ^���閳�G����
		int nPri = 0, // �����Ō��߂�v���C�I���e�B
		bool bUseintPri = false, // �v���C�I���e�B�������Ō��߂邩�ǂ����i�ʏ�̓^�C�v�ʂŐݒ�j
		CObject::TYPE type = CObject::TYPE::ATTACK, // �^�C�v
		CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_X // �I�u�W�F�N�g�^�C�v
	);
	~CAttack();	// �f�X�g���N�^

	// === ���C�t�T�C�N�������o�֐� ===
	HRESULT Init() override;  // ����������
	void Uninit() override;   // �I������
	void Update() override;   // �X�V����
	void Draw() override;     // �`�揈��
	void SetDeath() override; // ���S�t���O��ݒ�

	// === �����o�֐� ===

	// �����������ɃI�u�W�F�N�g��j�����邩�ǂ���
	void SetCollisionRelease(bool bUse);     // �ݒ�
	const bool& GetCollisionRelease() const; // �擾

	const HitStop& GetHitStop(); // ���G��ԏ��擾

	const int& GetPower() const; // �З͎擾

	// ����^�C�v
	void SetCollisionType(COLLISIONTYPE Type);    // �ݒ�
	const COLLISIONTYPE GetCollisionType() const; // �擾

	// �^�[�Q�b�g�^�C�v
	void SetTargetType(TARGETTYPE Type);    // �ݒ�
	const TARGETTYPE GetTargetType() const; // �擾

	// �����Ă���I�u�W�F�N�g�ȊO�ɂ������邩�ǂ���
	void SetHitOtherThanLibing(bool bHit);     // �ݒ�
	const bool& GetHitOtherThanLibing() const; // �擾

	// �����蔻����U���N���X�ɔC���邩�ǂ���
	void SetAutoCollision(bool bAuto); // �ݒ�
	const bool& GetAutoCollision();    // �擾

	// �����o��������g�����ǂ���
	void SetExtrusionCollision(bool bUse); // �ݒ�
	const bool& GetExtrusionCollision();   // �擾

	const CObject::TYPE& GetCollisionObjType(); // ���������I�u�W�F�N�g�̃^�C�v���擾

	BoundInfo& GetBoundInfo(); // �o�E���h�����擾

	// �U���̎��
	void SetAttackType(ATTACKTYPE AttackType); // �ݒ�
	const ATTACKTYPE& GetAttackType() const;   // �擾
protected:
	// === �ÓI�����o�ϐ� ===

	static const string ATTACK_FILENAME[static_cast<int>(ATTACKTYPE::MAX)]; //�U�����f���̃t�@�C����

	const bool& GetCollisionSuccess() const;   //�����蔻�肪�����������ǂ����擾
private:
	// === �����o�ϐ� ===

	int m_nPower; // �U����

	bool 
		m_bCollisionRelease,   // �Փˎ��ɏ������ǂ���
        m_bHitOtherThanLiving, // �����Ă���I�u�W�F�N�g�ȊO�ɂ������邩�ǂ���
	    m_bAutoCollision,      // �����蔻����U���N���X�ɔC���邩�ǂ���
	    m_bExtrusionCollision, // �����o��������g�����ǂ���
		m_bCollisionSuccess;   // ���肪�����������ǂ���

	ATTACKTYPE m_Type;       // �^�C�v
	TARGETTYPE m_TargetType; // �_���I�u�W�F�N�g�̎��
	COLLISIONTYPE m_CollisionType;    // ����^�C�v
	CObject::TYPE m_CollisionObjType; // ���������I�u�W�F�N�g�̎��

	HitStop m_HitStop;     // �q�b�g�X�g�b�v
	BoundInfo m_BoundInfo; // �o�E���h���

	// === �����o�֐� ===

	void Collision(); // �����蔻����s������

	// ���菈��
	void CollisionProcess(
		bool& bCollision,     // �������Ă��邩�ǂ���
		bool & bNowCollision, // ���ݓ������Ă��邩�ǂ���
		CObjectX * pObjX      // �I�u�W�F�N�gX�̃|�C���^
	);
	void CollisionBuilding();         // �����I�u�W�F�N�g�Ɣ��肷�鏈��
	void ExtrusionCollisionProcess(); // �����o��������s������

};

//===========================================
// �v���C���[�U���N���X
//===========================================
class CAttackPlayer : public CAttack
{
public:
	// === ����֐� ===

	// �R���X�g���N�^
	CAttackPlayer(
		int nPower,         // �З�
		int nSetHitStopTime // ��e�I�u�W�F�N�g�ɗ^���閳�G����
	);
	~CAttackPlayer() override; // �f�X�g���N�^

	// === ���C�t�T�C�N�������o�֐� ===

	HRESULT Init() override;   // ����������
	void Uninit() override;    // �I������
	void Update() override;    // �X�V����
	void Draw() override;      // �`�揈��
	void SetDeath() override;  // ���S�t���O��ݒ�

	// === �ÓI�����o�֐� ===

	// ��������
	static CAttackPlayer* Create(
		ATTACKTYPE AttackType, // �U���^�C�v
		TARGETTYPE TargetType, // ������I�u�W�F�N�g�^�C�v
		COLLISIONTYPE CollisionType, // ����^�C�v
		bool bHitOtherThanLiving, // �����Ă���I�u�W�F�N�g�ȊO�ɓ����邩�ǂ���
		bool bAutoCollision,      // ������N���X�ɔC���邩�ǂ���(false�̏ꍇ�A�Ăяo�����N���X�����肷��j
		int nPower, // �З�                  
		int nSetHitStopTime, // ���������I�u�W�F�N�g�ɗ^���閳�G��ԃt���[��
		int nLife, // �̗�
		D3DXVECTOR3 pos,    // �ʒu
		D3DXVECTOR3 rot,    // ����
		D3DXVECTOR3 move,   // �ړ���
		D3DXVECTOR3 Scale); // �g�嗦
private:
	// === �����o�֐� ===
	void BulletCollisionProcess(); // �e�������������̏���
};

//===========================================
// �G�U���N���X
//===========================================
class CAttackEnemy : public CAttack
{
public:
	// === ����֐� ===

	//�R���X�g���N�^
	CAttackEnemy(
		int nPower,
		int nSetHitStopTime
	);

	~CAttackEnemy() override; // �f�X�g���N�^

	// === ���C�t�T�C�N���֐� ===

	HRESULT Init() override;  // ����������
	void Uninit() override;   // �I������
	void Update() override;   // �X�V����
	void Draw() override;     // �`�揈��
	void SetDeath() override; // ���S�t���O��ݒ�

	// === �ÓI�����o�֐� ===

	// ��������
	static CAttackEnemy* Create(
		ATTACKTYPE AttackType, // �U���^�C�v
		TARGETTYPE TargetType, // ������I�u�W�F�N�g�^�C�v
		COLLISIONTYPE CollisionType, // ����^�C�v
		bool bHitOtherThanLiving, // �����Ă���I�u�W�F�N�g�ȊO�ɂ������邩�ǂ���
		bool bAutoCollision,      // ������N���X�ɔC���邩�ǂ����ifalse�̏ꍇ�A�Ăяo�����N���X��������Ǘ��j
		int nPower, // �З�
		int nSetHitStopTime, // ���������I�u�W�F�N�g�ɗ^���閳�G��ԃt���[���� 
		int nLife, // �̗�
		D3DXVECTOR3 pos,    // �ʒu
		D3DXVECTOR3 rot,    // ����
		D3DXVECTOR3 move,   // �ړ���
		D3DXVECTOR3 Scale   // �g�嗦
	);
};
//==================================================================================================================================================
#endif
