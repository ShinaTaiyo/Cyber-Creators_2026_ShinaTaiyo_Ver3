//====================================================
//
//�P�O���Q�S���F�U���N���X����������[attack.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _ATTACK_H_  
#define _ATTACK_H_

//======================
//�C���N���[�h
//======================
#include "main.h"
#include "objectX.h"
//==========================================

//===========================================
//�U���N���X
//===========================================
class CAttack : public CObjectX
{
public:
	enum class ATTACKTYPE
	{//���
		BULLET = 0,
		EXPLOSION,
		MAGICSWORD,
		MAX
	};

	enum class COLLISIONTYPE
	{//����̎��
		NONE = 0,
		SQUARE,
		RECTANGLE_XZ,
		MAX
	};

	enum class TARGETTYPE
	{//�_���I�u�W�F�N�g
		NONE = 0,
		PLAYER,
		ENEMY,
		MAX
	};

	//�q�b�g�X�g�b�v���
	struct HitStop
	{
		int nCntTime;
		const int nSetTime;
	};

	//�o�E���h���
	struct BoundInfo
	{
		//�֐�
		void SetActive(bool bCopyActive, D3DXVECTOR3 CopyPower, bool bCopyGravity, float fCopyGravity)
		{
			bActive = bCopyActive; Power = CopyPower; bGravity = bCopyGravity; fGravity = fCopyGravity;
		};//�o�E���h�����邩�ǂ���
		void BoundProcess(CAttack * pAttack);//�o�E���h����
		const D3DXVECTOR3& GetPower() const { return Power; }//�o�E���h�͂��擾
		const bool& GetActive() const { return bActive; }    //�o�E���h���邩�ǂ������擾
	private:
		D3DXVECTOR3 Power = { 0.0f,0.0f,0.0f }; //�o�E���h��
		bool bActive = false;//�o�E���h���邩�ǂ���
		bool bGravity = false;//�o�E���h���ɏd�͂�ON�ɂ��邩�ǂ���
		float fGravity = 0.0f;//�o�E���h���ɏd�͂�ON�ɂ���ꍇ�ɐݒ肷��d��
	};

	CAttack(int nPower,int nSetHitStopTime,int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::ATTACK, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_X);                  //�R���X�g���N�^
	~CAttack();                 //�f�X�g���N�^
	HRESULT Init() override;    //����������
	void Uninit() override;     //�I������
	void Update() override;     //�X�V����
	void Draw() override;       //�`�揈��
	void SetDeath() override;   //���S�t���O��ݒ�
	void SetCollisionRelease(bool bUse) { m_bCollisionRelease = bUse; }//�����������ɃI�u�W�F�N�g���������ǂ���
	const HitStop & GetHitStop() const { return m_HitStop; }//���G��ԏ����擾����
	const int& GetPower() const { return m_nPower; }//�З͂��擾����
	const bool& GetCollisionRelease() const{ return m_bCollisionRelease; }//�����������ɏ������ǂ������擾����

	//����^�C�v��ݒ肷��
	void SetCollisionType(COLLISIONTYPE Type) { m_CollisionType = Type; }//�����蔻��̎�ނ�ݒ肷��
	const COLLISIONTYPE GetCollisionType() const { return m_CollisionType; }

	//�^�[�Q�b�g�^�C�v��ݒ肷��
	void SetTargetType(TARGETTYPE Type) { m_TargetType = Type; }
	const TARGETTYPE GetTargetType() const { return m_TargetType; }

	//�����Ă���I�u�W�F�N�g�ȊO�ɂ������邩�ǂ�����ݒ�
	void SetHitOtherThanLibing(bool bHit) { m_bHitOtherThanLiving = bHit; }
	const bool& GetHitOtherThanLibing() const { return m_bHitOtherThanLiving; }

	//�����蔻����U���N���X�ɔC���邩�ǂ���
	void SetAutoCollision(bool bAuto) { m_bAutoCollision = bAuto; }
	const bool& GetAutoCollision() { return m_bAutoCollision; }

	//�����o��������g�����ǂ���
	void SetExtrusionCollisioin(bool bUse) { m_bExtrusionCollision = bUse; }
	const bool& GetExtrusionCollisioin() { return m_bExtrusionCollision; }

	const CObject::TYPE& GetCollisionObjType() { return m_CollisionObjType; }//���������I�u�W�F�N�g�̃^�C�v���擾����

	//�\����
	BoundInfo& GetBoundInfo() { return m_BoundInfo; }//�o�E���h�����擾

protected:
	void SetAttackType(ATTACKTYPE AttackType) { m_Type = AttackType;}//�U���̎�ނ�ݒ肷��
	const ATTACKTYPE & GetAttackType() const { return m_Type; }      //�U���̎�ނ��擾����
	static const string ATTACK_FILENAME[static_cast<int>(ATTACKTYPE::MAX)];//�U�����f���̃t�@�C���� 
	const bool& GetCollisionSuccess() const { return m_bCollisionSuccess; }//�����蔻�肪�����������ǂ������擾
private:

	//================================================
	//�ϐ��錾
	//================================================
	int m_nPower;            //�U����

	bool m_bCollisionRelease;//�Փˎ��ɏ������ǂ���

	bool m_bHitOtherThanLiving;//�����Ă���I�u�W�F�N�g�ȊO�ɂ������邩�ǂ���
	bool m_bAutoCollision;     //�����蔻����U���N���X�ɔC���邩�ǂ���

	bool m_bExtrusionCollision;        //�����o��������g�����ǂ���
	bool m_bCollisionSuccess; //���肪�����������ǂ���

	HitStop m_HitStop;//�q�b�g�X�g�b�v
	ATTACKTYPE m_Type;//�^�C�v
	COLLISIONTYPE m_CollisionType;//����^�C�v
	CObject::TYPE m_CollisionObjType;//���������I�u�W�F�N�g�̎��
	TARGETTYPE m_TargetType;//�_���I�u�W�F�N�g�̎��
	BoundInfo m_BoundInfo;  //�o�E���h���
	//==========================================================================================

	//================================================
	//�v���g�^�C�v�錾
	//================================================
	void Collision();//�����蔻����s������
	void CollisionProcess(bool& bCollision,bool & bNowCollision,CObjectX * pObjX);//�����蔻��̏������s��
	void HitOtherCollisionProcess();//���̃I�u�W�F�N�g�Ƃ����肷�鏈��
	void ExtrusionCollisionProcess();//���̃I�u�W�F�N�g�Ƃ̉����o��������s������
	//==========================================================================================
};
//==================================================================================================================================================

//===========================================
//�v���C���[�U���N���X
//===========================================
class CAttackPlayer : public CAttack
{
public:
	CAttackPlayer(int nPower,int nSetHitStopTime);      //�R���X�g���N�^
	~CAttackPlayer() override;                 //�f�X�g���N�^
	HRESULT Init() override;          //����������
	void Uninit() override;           //�I������
	void Update() override;           //�X�V����
	void Draw() override;             //�`�揈��
	void SetDeath() override;         //���S�t���O��ݒ�
	static CAttackPlayer* Create(ATTACKTYPE AttackType,TARGETTYPE TargetType,COLLISIONTYPE CollisionType,bool bHitOtherThanLiving,bool bAutoCollision,int nPower,int nSetHitStopTime,int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXVECTOR3 Scale);//��������
private:
	void BulletCollisionProcess();//�e�������������̏���
};
//==================================================================================================================================================

//===========================================
//�G�U���N���X
//===========================================
class CAttackEnemy : public CAttack
{
public:
	CAttackEnemy(int nPower,int nSetHitStopTime);         //�R���X�g���N�^
	~CAttackEnemy() override;                  //�f�X�g���N�^
	HRESULT Init() override;          //����������
	void Uninit() override;           //�I������
	void Update() override;           //�X�V����
	void Draw() override;             //�`�揈��
	void SetDeath() override;         //���S�t���O��ݒ�
	static CAttackEnemy* Create(ATTACKTYPE AttackType, TARGETTYPE TargetType, COLLISIONTYPE CollisionType,bool bHitOtherThanLiving,bool bAutoCollision, int nPower, int nSetHitStopTime, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXVECTOR3 Scale);//��������
private:
};
//==================================================================================================================================================
#endif
