//====================================================
//
//�V���P�X���F�G��AI����[enemy.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _ENEMY_H_  
#define _ENEMY_H_

//======================
//�C���N���[�h
//======================
#include "main.h"
#include "objectX.h"
#include "aimodel.h"
//==========================================

//======================
//�O���錾
//======================
class CModel;

//==========================================
//�G�l�~�[�N���X
//==========================================
class CEnemy : public CObjectX
{
public:
	//===========================
	//�G�̎��
	//===========================
	typedef enum
	{
		ENEMYTYPE00_SLIME = 0,
		ENEMYTYPE01_FIRESLIME,
		ENEMYTYPE02_SWORDSLIME,
		ENEMYTYPE03_HAMMERSLIME,
		ENEMYTYPE04_ORANGESLIME,
		ENEMYTYPE05_THUNDERSLIME,
		ENEMYTYPE06_SCANINGSLIME,
		ENEMYTYPE07_NEEDLESLIME,
		ENEMYTYPE08_ENGELSLIME,
		ENEMYTYPE09_DARKSLIME,
		ENEMYTYPE10_RAINBOWSLIME,
		ENEMYTYPE_MAX
	}ENEMYTYPE;
	//======================================================================
	CEnemy();                   //�R���X�g���N�^
	~CEnemy() override;         //�f�X�g���N�^
	HRESULT Init() override;    //����������
	void Uninit() override;     //�I������
	void Update() override;     //�X�V����
	void Draw() override;       //�`�揈��
	void SetDamage(int nDamage, int nHitStopTime) override;  //�_���[�W��^���鏈��
	void SetDeath() override;   //���S�t���O��ݒ�
	static int GetNumFile() { return m_nNumFile; }//�t�@�C���̐����擾����
	static CEnemy * Create(ENEMYTYPE type,int nLife,D3DXVECTOR3 pos, D3DXVECTOR3 move,D3DXVECTOR3 Scale,
		D3DXVECTOR3 MoveLemgthAIPos,D3DXVECTOR3 MoveLengthAIRot,D3DXVECTOR3 MoveLengthAIScale);//�G�𐶐�

	//==============================================
	//�G��AI���f��
	//==============================================
	CAIModel* GetEnemyMoveRangeAIModel() { return m_pMoveRangeAI; }

private:
	static const char* m_ENEMY_FILENAME[ENEMYTYPE_MAX];           //�u���b�N�̃t�@�C���l�[��
	static const int m_nMAXENEMY01BULLET;
	static int m_nNumFile;                                        //�ǂݍ��񂾃t�@�C���̐�

	//==============================
	//�G�O�̐ÓI�����o
	//==============================
	static constexpr float m_fENEMY00_MOVESTARTLENGTH = 500.0f;
	static constexpr float m_fENEMY00_NEARACTIONPATTERNLENGTH = 150.0f;//�߂��̎��̃p�^�[���𔭓����鋗��
	//==============================================================================================================

	//==============================
	//�G�P�̐ÓI�����o
	//==============================
	static constexpr float m_fENEMY01_NORMALSPEED = 2.0f;//�G�P�̕��ʂ̑��x
	static const int m_nENEMY01_ACTION0_FRAME = 40;//�G�P�̍s���O�̃t���[����
	//==============================================================================================================
	
	//==============================
	//�G�Q�̐ÓI�����o
	//==============================
	static constexpr float m_fENEMY02_JUMPPOWER = 16.0f;//�G�Q�̃W�����v��
	static constexpr float m_fENEMY02_ACTION1_SPEED = 5.0f;//�G�Q�̃A�N�V�����Q�̑���
	//==============================================================================================================

	//==============================
	//�G�S�̐ÓI�����o
	//==============================
	static constexpr float m_fENEMY04_ACTION2THREEWAYAIM = 0.8f;//�A�N�V�����Q�̒e�̔��ˊp�x�Ԋu
	//==============================================================================================================

	//==============================
	//�G�T�̐ÓI�����o
	//==============================
	static const int m_nMAX_ENEMY05_BULLETNUM = 3;//�e�̐�
	//==============================================================================================================

	//==============================
	//�����蔻��
	//==============================
	void Collision();
	//====================================================================

	void CalculationLength();//�v���C���[�Ƃ̋������v�Z����
	ENEMYTYPE m_type;       //�G�̎��
	D3DXVECTOR3 m_Aim;      //�x�N�g��
	CObject* m_pLandingObj;   //����Ă���I�u�W�F�N�g
	float m_fTotalAim;      //���������x�N�g��
	bool m_bUse;            //�g�p���
	bool m_bIsJumping;      //�W�����v�����ǂ���
	bool m_bIsWalling;      //�ǂɓ������Ă��邩�ǂ���
	bool m_bAttack;         //�U�������邩�ǂ���
	int m_nCntTime;         //�o�����Ă���̃t���[�����𑪂�

	int m_nCntAction;       //�A�N�V�����񐔃J�E���g�p
	int m_nPattern;         //�p�^�[��
	int m_nSubPattern;      //�T�u�̃p�^�[��
	int m_nPatternTime;     //�p�^�[���ɓ����Ă���̎��Ԃ��J�E���g����
	bool m_bPattern;        //�s���p�^�[�������s���邩�ǂ���
	bool m_bAction;         //�s�������邩�ǂ��� 

	float m_fLength;        //�v���C���[�Ƃ̋���

	float m_fShotRot;       //�o���b�g�Ȃǂ𔭎˂������

	float m_fGravityPower;                                  //�d�͂̑傫��

	bool m_bSpeedSwitch;    //�ړ�������ς���t���O

	void GravityProcess();                                  //�d�͂�������
	//=================================
	//�G�̍s���p�^�[��
	//=================================
	void Enemy00Pattern();                                 //�G�O�̃p�^�[��
	void Enemy01Pattern();                                 //�G�P�̃p�^�[��
	void Enemy02Pattern();                                 //�G�Q�̃p�^�[��
	void Enemy03Pattern();                                 //�G�R�̃p�^�[��
	void Enemy04Pattern();                                 //�G�S�̃p�^�[��
	void Enemy05Pattern();                                 //�G�T�̃p�^�[��
	void Enemy06Pattern();                                 //�G�U�̃p�^�[��
	void Enemy07Pattern();                                 //�G�V�̃p�^�[��
	void Enemy08Pattern();                                 //�G�W�̃p�^�[��
	void Enemy09Pattern();                                 //�G�X�̃p�^�[��
	void Enemy10Pattern();                                 //�G�P�O�̃p�^�[��
	void ResetPattern();                                   //�s���p�^�[���̃��Z�b�g
	void ApproachProcess(float fSpeed);                    //�ڋ߂��鏈��
	//====================================================================================================

	//================================
	//�G��AI�p���f��
	//================================
	CAIModel* m_pMoveRangeAI;//�ړ��͈͂����߂�AI
	void AdjustEnemyPos();//�G�̈ʒu�������s������
	//====================================================================================================

	//================================
	//�G�f�B�^�\���p
	//================================
	void DispInfo() override;//����\������
	void ChengeLifeProcess();//�̗͂�ς��鏈��
	void ChengeMoveLengthAIScale();//�ړ��͈�AI���f���͈̔͂�ς��鏈��
	void ChengeMoveLengthAIScaleX();//�ړ��͈�AI���f���̊g�嗦X��ς��鏈��
	void ChengeMoveLengthAIScaleY();//�ړ��͈�AI���f���̊g�嗦Y��ς��鏈��
	void ChengeMoveLengthAIScaleZ();//�ړ��͈�AI���f���̊g�嗦Z��ς��鏈��
	void ChengeMoveLengthAIPos();   //�ړ��͈�AI���f���̈ʒu��ς��鏈��


	//====================================================================================================

};

#endif
