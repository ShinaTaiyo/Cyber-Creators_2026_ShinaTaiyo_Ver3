//====================================================
//
//�V���P���F���@�U�������[attack.h]
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
#include "effect.h"
//==========================================

//==========================================

//==========================================
//�U���N���X
//==========================================
class CAttack : public CObjectX
{
public:
	//===========================
	//�U���̎��
	//===========================
	typedef enum
	{
		ATTACKTYPE00_FIRE = 0,      //�U���O�F�t�@�C�A
		ATTACKTYPE01_METEO,         //�U���P�F���e�I
		ATTACKTYPE02_NEEDLE,        //�U���Q�F�j�[�h��
		ATTACKTYPE03_TORNADO,       //�U���R�F�g���l�[�h
		ATTACKTYPE04_BEAM,          //�U���S�F�r�[��
		ATTACKTYPE05_ICE,           //�U���T�F�A�C�X
		ATTACKTYPE06_THUNDER,       //�U���U�F�T���_�[
		ATTACKTYPE07_MAGICSWORD,    //�U���V�F�}�W�b�N�\�[�h
		ATTACKTYPE08_VACUUMBLADE,   //�U���W�F�^��n
		ATTACKTYPE09_HAMMER,        //�U���X�F�n���}�[
		ATTACKTYPE10_SHOCKWAVE,     //�U���P�O�F�Ռ��g
		ATTACKTYPE11_EXPLOSION,     //�U���P�P�F����
		ATTACKTYPE12_VINE,          //�U���P�Q�F��
		ATTACKTYPE13_CROSSBOMB,     //�U���P�R�F�N���X�{��
		ATTACKTYPE14_HELLFIRE,      //�U���P�S�F�Ɖ�
		ATTACKTYPE15_BLACKHOLE,     //�U���P�T�F�u���b�N�z�[��
		ATTACKTYPE16_PHYCOBURST,    //�U���P�U�F�T�C�R�o�[�X�g
		ATTACKTYPE17_BLOCK,         //�U���P�V�F�u���b�N
		ATTACKTYPE18_THORNCLUB,     //�U���P�W�F�g�Q����_
		ATTACKTYPE19_SENTERTHUNDER, //�U���P�X�F���S��
		ATTACKTYPE_MAX
	}ATTACKTYPE;
	//======================================================================

	//===========================
	//���Ă�G�̎��
	//===========================
	typedef enum
	{
		TARGETTYPE_ENEMY = 0,      //�G�ɓ��Ă�
		TARGETTYPE_PLAYER,         //�v���C���[�ɓ��Ă�
		TARGETTYPE_MAX
	}TARGETTYPE;
	//======================================================================

	//===========================
	//�����蔻��̎��
	//===========================
	typedef enum
	{
		COLLISIONTYPE00_SQUARE = 0,//�����`�̓����蔻��
		COLLISIONTYPE01_RECTANGLEXY,//�����`�̎΂߂��܂߂������蔻��
		COLLISIONTYPE02_TRUERECTANGLEXY,//�����Ƃ��������`XY�̓����蔻��
		COLLISIONTYPE03_BALL,           //�~�̓����蔻����s��
		COLLISIONTYPE_MAX
	}COLLISIONTYPE;
	//======================================================================

	CAttack();                  //�R���X�g���N�^
	~CAttack() override;        //�f�X�g���N�^
	HRESULT Init() override;    //����������
	void Uninit() override;     //�I������
	void Update() override;     //�X�V����
	void Draw() override;       //�`�揈��
	void SetDeath() override;   //���S�t���O
	static int GetNumFile() { return m_nNumFile; }//�t�@�C���̐����擾����
	static CAttack* Create(ATTACKTYPE type, int nDamage, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 Scale, TARGETTYPE TargetType, D3DXVECTOR3 Rot,
		COLLISIONTYPE CollisionType, D3DXVECTOR3 AddRot, int nHitStopTime);     //�U���𐶐�
	void SetPower(int nPower) { m_nPower = nPower; }         //�З͂�ݒ�
	void SetType(ATTACKTYPE type) { m_type = type; }         //�^�C�v��ݒ�
	void SetTargetType(TARGETTYPE TargetType) { m_TargetType = TargetType; }//�^�[�Q�b�g�^�C�v��ݒ�

	void SetCollisionType(COLLISIONTYPE CollisionType) { m_CollisionType = CollisionType; }//����^�C�v��ݒ�
	void SetCollisionRelease(bool bCollisionRelease) { m_bCollisionRelease = bCollisionRelease; }       //���������Ƃ��ɏ������ǂ�����ݒ肷��
	void SetDoCollision(bool bCollision) { m_bDoCollision = bCollision; }                              //�����蔻������邩�ǂ�����ݒ肷��
	void SetHitStopTime(int nHitStopTime) { m_nSetHitStopTime = nHitStopTime; }                        //�q�b�g�X�g�b�v���Ԃ�ݒ肷��

	void SetUseThinColor(bool bUse);                        //�F�𔖂����Ă������ǂ���

	void SetUseGravity(float fGravityPower);                //�d�͂��g�p���邩�ǂ���

	void SetUseEffect(CEffect::EFFECTTYPE type, bool bUse, D3DXCOLOR col, int nLife, D3DXVECTOR2 Size) {
		m_SetEffectType = type; m_bUseSetEffect = bUse; m_SetEffectColor = col; m_nSetEffectLife = nLife; m_SetEffectSize = Size;
	} //�G�t�F�N�g���g�p���邩�ǂ���

	bool GetbUse() { return m_bUse; }                      //�g�p��Ԃ��擾
	void SetDelayDeath(bool bDelayDeath) { m_bDelaySetDeath = bDelayDeath; }//���S�t���O��x�点�邩�ǂ�����ݒ�
	bool GetDelayDeath() { return m_bDelaySetDeath; }

	void SetPurposePos(D3DXVECTOR3 PurposePos) { m_PurposePos = PurposePos; }//�ړI�n��ݒ肷��

	//=================================================
	//���ˊ֌W
	//=================================================
	void SetUseReflection(bool bUse) {m_bUseReflection = bUse;}//���˂����邩�ǂ���
	D3DXVECTOR3& GetPurposePos() { return m_PurposePos; }//�ړI�n���擾����
	//===================================================================================

	//=================================================
	//�U���G�t�F�N�g�֌W
	//=================================================
	void SetUseAttackEffect(bool bUse, int nLife, D3DXCOLOR col) { m_bUseAttackEffect = bUse; m_nSetAttackEffectLife = nLife; m_SetAttackEffectCol = col; }
	//===================================================================================

	//=================================================
	//�T�u�^�C�v
	//=================================================
	int& GetSubType() { return m_nSubType; }
	void SetSubType(int nSubType) { m_nSubType = nSubType; }
	//===================================================================================

	//=================================================
	//�^�C�v
	//=================================================
	ATTACKTYPE GetAttackType() { return m_type; }
	void SetAttackType(ATTACKTYPE type) { m_type = type; }
	//===================================================================================
protected:
	int& GetCntTime() { return m_nCntTime; }                //���Ԃ��擾����
	//============================================
	//�p�^�[���֌W
	//============================================
	int& GetPattern() { return m_nPattern; }                //�U���p�^�[�����擾
	int& GetPatternTime() { return m_nPatternTime; }        //�U���p�^�[������
	bool& GetPatternFlag() { return m_bPatternFlag; }       //�p�^�[���̃t���O
	//======================================================================================

	static const char* m_ATTACK_FILENAME[ATTACKTYPE_MAX];   //�U���̃t�@�C���l�[��
	int m_nAttackCnt;                                       //�U����
private:
	ATTACKTYPE m_type;                                      //�U���̎��
	D3DXVECTOR3 m_PurposePos;                               //�ړI�n����p                            
	int m_nCntTime;                                         //��������Ă���̎��Ԃ��v������
	int m_nSetHitStopTime;                                  //���������Ƃ��ɑΏۂɐݒ肷��q�b�g�X�g�b�v����
	int m_nPower;                                           //�З͂�ݒ�
	bool m_bCollisionRelease;                               //���������Ƃ��ɏ������ǂ���
	bool m_bUse;                                            //�g�p���
	TARGETTYPE m_TargetType;                                //���Ă�I�u�W�F�N�g�̎��
	COLLISIONTYPE m_CollisionType;                          //�����蔻��̎��
	static int m_nNumFile;                                  //�ǂݍ��񂾃t�@�C���̐�
	bool m_bDoCollision;                                    //�����蔻������邩�ǂ���
	int m_nSubType;                                         //���������߂邽�߂̃^�C�v

	bool m_bUseGravity;                                     //�d�͂��g�p���邩�ǂ���
	float m_fGravityPower;                                  //�d�͂̑傫��

	bool m_bUseThinColor;                                   //�F�𔖂����Ă������ǂ���

	bool m_bDelaySetDeath;                                  //���S�O���O������x�����邩�ǂ���

	//======================================
	//���ˊ֌W
	//======================================
	bool m_bUseReflection;
	//===================================================================================

	//======================================
	//�G�t�F�N�g�֌W
	//======================================
	bool m_bUseSetEffect;                                   //�G�t�F�N�g���g�����ǂ���
	int m_nSetEffectLife;                                   //�ݒ肷��G�t�F�N�g�̗̑�
	CEffect::EFFECTTYPE m_SetEffectType;                    //�ݒ肷��G�t�F�N�g�̎��
	D3DXCOLOR m_SetEffectColor;                             //�ݒ肷��G�t�F�N�g�̐F����
	D3DXVECTOR2 m_SetEffectSize;                            //�ݒ肷��G�t�F�N�g�̑傫��
	//==============================================================================================

	//======================================
	//�p�^�[���֌W
	//======================================
	int m_nPattern;                                         //�p�^�[��
	int m_nPatternTime;                                     //���ꂼ��̃p�^�[���ł̎���
	bool m_bPatternFlag;                                    //��x������������t���O
	//==============================================================================================

	//======================================
	//�U���G�t�F�N�g�֌W
	//======================================
	bool m_bUseAttackEffect;//�U���G�t�F�N�g���������邩�ǂ���
	int m_nSetAttackEffectLife;//�U���G�t�F�N�g�̗̑͂�ݒ肷��
	D3DXCOLOR m_SetAttackEffectCol;//�U���G�t�F�N�g�̐F������ݒ肷��
	//==============================================================================================

	//======================================
	//�����蔻��֌W
	//======================================
	void Collision();                                       //�����蔻������
	void RectAngleCollisionXY();                            //XY�����̎΂߂��܂߂�������s��
	void TrueRectAngleCollisionXY();                        //������XY�����̉�]��`���m�̓����蔻��
	void BallCollision();                                   //���̓����蔻����s��
};
//==============================================================================================================================================

//===========================================================
//�v���C���[�U���N���X
//===========================================================
class CAttackPlayer : public CAttack
{
public:
	CAttackPlayer();            //�R���X�g���N�^
	~CAttackPlayer() override;  //�f�X�g���N�^
	HRESULT Init() override;    //����������
	void Uninit() override;     //�I������
	void Update() override;     //�X�V����
	void Draw() override;       //�`�揈��
	void SetDeath() override;   //���S�t���O�ݒ菈��
	int& GetVineDelBulletNum() { return m_nVineDelBulletNum; }
	void SetVineDelBulletNum(int nNum) { m_nVineDelBulletNum = nNum; }
	static CAttackPlayer* Create(ATTACKTYPE type, int nDamage, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 Scale,D3DXVECTOR3 Rot,
		COLLISIONTYPE CollisionType, D3DXVECTOR3 AddRot,bool bCollisionRelease,int nHitStopTime);//�U���𐶐�
private:
	float m_fCorrectionRot;//�����p�̌���
	D3DXVECTOR3 m_SaveScale;//�g�嗦�ۑ��p
	D3DXVECTOR3 m_TotalRot;     //�����I�Ȍ������v�Z����

	bool m_bVineDelStart;       //������������J�n�t���O
	int m_nVineDelBulletNum;    //�邪������e�̐�
	void VineProcess();         //��̏���

	bool m_bCrossBombStaging;   //�N���X�{���̉��o�𔭓�����t���O
	int m_nCrossBombStagingCnt; //�N���X�{���̉��o�̎��Ԃ��J�E���g����
	void CrossBombProcess();    //�N���X�{���̏���

	int m_nBlackHoleDefeatCnt;  //�u���b�N�z�[���œ|�����G�̐����J�E���g����
	void BlackHoleProcess();    //�u���b�N�z�[���̏���

	void PhycoBurstProcess();   //�T�C�R�o�[�X�g�̏���
};
//==============================================================================================================================================

//===========================================================
//�G�l�~�[�U���N���X
//===========================================================
class CAttackEnemy : public CAttack
{
public:
	CAttackEnemy();             //�R���X�g���N�^
	~CAttackEnemy() override;   //�f�X�g���N�^
	HRESULT Init() override;    //����������
	void Uninit() override;     //�I������
	void Update() override;     //�X�V����
	void Draw() override;       //�`�揈��
	void SetDeath() override;   //���S�t���O�ݒ菈��
	static CAttackEnemy* Create(ATTACKTYPE type, int nDamage, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 Scale, D3DXVECTOR3 Rot,
		COLLISIONTYPE CollisionType, D3DXVECTOR3 AddRot, bool bCollisionRelease,int nHitStopTime);//�U���𐶐�
private:

};
//==============================================================================================================================================


//===========================================================
//�U���G�t�F�N�g�N���X
//===========================================================
class CAttackEffect : public CAttack
{
public:
	CAttackEffect();             //�R���X�g���N�^
	~CAttackEffect() override;   //�f�X�g���N�^
	HRESULT Init() override;     //����������
	void Uninit() override;      //�I������
	void Update() override;      //�X�V����
	void Draw() override;        //�`�揈��
	void SetDeath() override;   //���S�t���O�ݒ菈��
	static CAttackEffect* Create(ATTACKTYPE type, int nDamage, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 Scale, D3DXVECTOR3 Rot, D3DXVECTOR3 AddRot,D3DXCOLOR col);//�U���𐶐�
private:
	D3DXVECTOR3 m_MaxScale;//�g�嗦�ő�l
	D3DXCOLOR m_Col;       //�F����
};
//==============================================================================================================================================

//===========================================================
//�~�X�^�[�X�g�����O�U���N���X
//===========================================================
class CMrStrongAttack : public CAttack
{
public:
	CMrStrongAttack();             //�R���X�g���N�^
	~CMrStrongAttack() override;   //�f�X�g���N�^
	HRESULT Init() override;     //����������
	void Uninit() override;      //�I������
	void Update() override;      //�X�V����
	void Draw() override;        //�`�揈��
	void SetDeath() override;   //���S�t���O�ݒ菈��
	static CMrStrongAttack* Create(ATTACKTYPE type, int nDamage, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 Scale, D3DXVECTOR3 Rot,
		COLLISIONTYPE CollisionType, D3DXVECTOR3 AddRot, bool bCollisionRelease, int nHitStopTime);//�U���𐶐�
private:
	void BlockProcess();//�u���b�N�U���̏���
};
//==============================================================================================================================================

//===========================================================
//���C���[�U���N���X
//===========================================================
class CRileyAttack : public CAttack
{
public:
	typedef enum
	{
		ATTACKMOVE_RILEY_00_NONE = 0,
		ATTACKMOVE_RILEY_01_ACTION03,
		ATTACKMOVE_RILEY_MAX
	}ATTACKMOVE_RILEY;
	CRileyAttack();                    //�R���X�g���N�^
	~CRileyAttack() override;          //�f�X�g���N�^
	HRESULT Init() override;     //����������
	void Uninit() override;      //�I������
	void Update() override;      //�X�V����
	void Draw() override;        //�`�揈��
	void SetDeath() override;   //���S�t���O�ݒ菈��
	static CRileyAttack* Create(ATTACKTYPE type, int nDamage, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 Scale, D3DXVECTOR3 Rot,
		COLLISIONTYPE CollisionType, D3DXVECTOR3 AddRot, bool bCollisionRelease, int nHitStopTime);//�U���𐶐�
	void SetAttackMove(ATTACKMOVE_RILEY AttackMove) { m_AttackMove = AttackMove; }//�������̎�ނ����߂�

	static const int m_nMAX_ACTION03_SUBTYPE = 3;//�s���R�̍U���̃T�u�^�C�v�̍ő吔
private:
	ATTACKMOVE_RILEY m_AttackMove;
	void Action03_ExplosionProcess();
	void Action03_Process();
};
//==============================================================================================================================================

//===========================================================
//�h���C���U���N���X
//===========================================================
class CDraiyaAttack : public CAttack
{
public:
	CDraiyaAttack();                    //�R���X�g���N�^
	~CDraiyaAttack() override;          //�f�X�g���N�^
	HRESULT Init() override;     //����������
	void Uninit() override;      //�I������
	void Update() override;      //�X�V����
	void Draw() override;        //�`�揈��
	void SetDeath() override;   //���S�t���O�ݒ菈��
	static CDraiyaAttack* Create(ATTACKTYPE type, int nDamage, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 Scale, D3DXVECTOR3 Rot,
		COLLISIONTYPE CollisionType, D3DXVECTOR3 AddRot, bool bCollisionRelease, int nHitStopTime);//�U���𐶐�
private:
};
//==============================================================================================================================================

//===========================================================
//�K�u���G���U���N���X
//===========================================================
class CGabrielAttack : public CAttack
{
public:
	typedef enum
	{
		GABRIELATTACKMOVE_00 = 0,//�����O
		GABRIELATTACKMOVE_01,    //�����P
		GABRIELATTACKMOVE_02,    //�����Q
		GABRIELATTACKMOVE_03,    //�����R
		GABRIELATTACKMOVE_04,    //�����S
		GABRIELATTACKMOVE_05,    //�����T
		GABRIELATTACKMOVE_MAX
	}GABRIELATTACKMOVE;

	CGabrielAttack();             //�R���X�g���N�^
	~CGabrielAttack() override;   //�f�X�g���N�^
	HRESULT Init() override;     //����������
	void Uninit() override;      //�I������
	void Update() override;      //�X�V����
	void Draw() override;        //�`�揈��
	void SetDeath() override;   //���S�t���O�ݒ菈��
	static CGabrielAttack* Create(ATTACKTYPE type, int nDamage, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 Scale, D3DXVECTOR3 Rot,
		COLLISIONTYPE CollisionType, D3DXVECTOR3 AddRot, bool bCollisionRelease, int nHitStopTime);//�U���𐶐�
private:
	static const int m_nMAX_SUMMON_METEO_TORNADO = 8;
	GABRIELATTACKMOVE m_AttackMove;

	void Attack01Process();//�U���P�̏���
};
//==============================================================================================================================================

#endif