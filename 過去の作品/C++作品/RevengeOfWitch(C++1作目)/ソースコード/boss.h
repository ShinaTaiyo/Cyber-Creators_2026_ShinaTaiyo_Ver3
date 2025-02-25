//====================================================================================
//
//�V���Q���F�{�X�P����������[boss.h]
//Author:ShinaTaiyo
//
//====================================================================================

#ifndef _BOSS_H_  
#define _BOSS_H_

//======================
//�C���N���[�h
//======================
#include "main.h"
#include "objectX.h"
#include "gauge.h"
#include "attack.h"
//==========================================

//==========================================
//�O���錾
//==========================================
class CObject;
class CBullet;
class CAIModel;

//==========================================
//�{�X�N���X
//==========================================
class CBoss : public CObjectX
{
public:
	//===========================
	//�{�X�̎��
	//===========================
	typedef enum
	{
		BOSSTYPE00_KINGSLIME = 0,
		BOSSTYPE01_DEATHENAGA,
		BOSSTYPE02_MRSTRONG,
		BOSSTYPE03_SNOWDEVIL_RILEY,
		BOSSTYPE04_DRAIYA,
		BOSSTYPE05_GABRIEL,
		BOSSTYPE_MAX
	}BOSSTYPE;
	//======================================================================
	CBoss();                                                                                                //�R���X�g���N�^
	~CBoss() override;                                                                                      //�f�X�g���N�^
	HRESULT Init() override;                                                                                //����������
	void Uninit() override;                                                                                 //�I������
	void Update() override;                                                                                 //�X�V����
	void Draw() override;                                                                                   //�`�揈��
	void SetDeath() override;                                                                               //���S�t���O��ݒ�
	void SetDamage(int nDamage, int nHitStopTime) override;                                                 //�_���[�W��^����
	static int GetNumFile() { return m_nNumFile; }                                                          //�t�@�C���̐����擾����
	static CBoss* Create(BOSSTYPE type, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 Scale);                      //�{�X�𐶐�
	static const int m_nMAX_DEFEATSTAGINGTIME = 180;                                                        //�|�����o�̎��Ԃ̍ő吔
	static CObject* SetCreateBoss(BOSSTYPE type, int nLife, D3DXVECTOR3 Pos, D3DXVECTOR3 Scale);                 //�{�X��ݒ肷��
protected:
	static const char* m_BOSS_FILENAME[BOSSTYPE_MAX];        //�{�X�̃t�@�C���l�[��
	void SetType(BOSSTYPE type) { m_type = type; }           //�{�X�̎�ނ�ݒ肷��

	void SetBossHpGauge(int nLife);                          //�{�X�̗̑̓Q�[�W��ݒ肷��
	int& GetCntTime() { return m_nCntTime; }                 //���Ԃ��擾����
	int& GetPatternTime() { return m_nPatternTime; }         //�s���p�^�[���̎��Ԃ��擾
	int& GetSkill() { return m_nSkill; }                     //�X�L�����擾
	int& GetPattern() { return m_nPattern; }                  //���݂̃p�^�[�����擾
	int& GetActionNum() { return m_nActionNum; }
         //�s���񐔂��擾
	bool& GetAction() { return m_bAction; }                  //�s�����Ă��邩�ǂ������擾
	bool& GetUseMovePattern() { return m_bUseMovePattern; }  //�ړ��p�^�[�����g�p���Ă��邩�ǂ������擾
	int& GetMovePatternFrame() { return m_nMovePatternFrame; }//�ړ��p�^�[���t���[�����擾
	void ResetPattern();                                     //�{�X�̍s���p�^�[�������Z�b�g����
	virtual void MovePattern();                              //�{�X�̈ړ�����
	virtual void ActionPattern();                            //�{�X�̍s���p�^�[��
	D3DXVECTOR3& GetPurposePos() { return m_PurposePos; }    //�ړI�̈ʒu�擾
	void SetPurposePos(D3DXVECTOR3 Pos) { m_PurposePos = Pos; }//�ړI�̈ʒu��ݒ�
	void SetInsanity(bool bInsanity) { m_bInsanity = bInsanity; }//������ݒ肷��
	bool& GetInsanity() { return m_bInsanity; }              //�������Ă��邩�ǂ������擾
	void SetSpecialAttack(bool bUse) { m_bSpecialAttack = bUse; }//�K�E�Z���g�����ǂ�����ݒ�
	bool& GetSpecialAttack() { return m_bSpecialAttack; }    //�K�E�Z���g�����ǂ������擾
	int& GetTurnCnt() { return m_nTurnCnt; }                 //�^�[�������J�E���g
	void SetMovePatternNum(int nNum) { m_nMovePatternNum = nNum; }//�ړ��p�^�[���ԍ���ݒ�
	int& GetMovePatternNum() { return m_nMovePatternNum; }   //�ړ��p�^�[���ԍ����擾����
	int& GetMaxMovePatternFrame() { return m_nMaxMovePatternFrame; }//�ړ��p�^�[���̍ő�t���[�������擾����
	int& GetSubPattern() { return m_nSubPattern; }           //�T�u�p�^�[�����擾
	virtual void DecideMovePattern();                        //�ړ��p�^�[�������߂鏈��
	void SetSaveRot(float fRot) { m_fSaveRot = fRot; }       //�ۑ����������ݒ肷��
	float& GetSaveRot() { return m_fSaveRot; }               //�ۑ������������擾����
private:
	CGauge* m_pGauge;                                        //�Q�[�W
	void DefeatStaging();                                    //�������o
	void DispInfo() override;                                //���\������
	void BossRushProcess();                                  //�{�X���b�V���̏���
	virtual void AdjustSize();                               //�{�X�̃T�C�Y�𒲐�����
	//==============================
	//�����蔻��
	//==============================
	void Collision();
	//====================================================================

	BOSSTYPE m_type;                                         //�{�X�̎��
	D3DXVECTOR3 m_Aim;                                       //�x�N�g��
	float m_fTotalAim;                                       //���������x�N�g��
	bool m_bUse;                                             //�g�p���
	int m_nCntTime;                                          //�o�����Ă���̃t���[�����𑪂�
	int m_nDefeatStagingTime;                                //�|�����o�̎���

	//========================================
	//�ړ��p�^�[���֌W
	//========================================
	bool m_bUseMovePattern;                               //�ړ��p�^�[���𔭓����邩�ǂ���
	int m_nMaxMovePatternFrame;                           //�ړ��p�^�[���̍ő�t���[����
	int m_nMovePatternFrame;                              //�ړ���Ԃ̃t���[�������J�E���g����
	int m_nMovePatternNum;                                   //�ړ��p�^�[���̔ԍ�
	//==========================================================================================================================

	//========================================
	//�s���p�^�[���֌W
	//========================================
	int m_nPatternTime;                                      //�s���p�^�[���̎���
	int m_nSkill;                                            //�X�L��
	int m_nPattern;                                          //�s���p�^�[��
	int m_nSubPattern;                                       //�T�u�p�^�[���i�s���p�^�[���̒��ōX�ɕ����j
	int m_nActionNum;                                        //�s����
	bool m_bAction;                                          //�s�����Ă��邩�ǂ���
	float m_fSaveRot;                                        //������ۑ����Ă���
	D3DXVECTOR3 m_PurposePos;                                //�ړI�̈ʒu�ۑ��p
	//==========================================================================================================================

	int m_nTurnCnt;                                          //�^�[�������J�E���g
	bool m_bInsanity;                                        //�������Ă��邩�ǂ���
	bool m_bSpecialAttack;                                   //�K�E�Z���g�����ǂ���
	static int m_nNumFile;                                   //�ǂݍ��񂾃t�@�C���̐�
};
//====================================================================================================

//==========================================
//�L���O�X���C���N���X
//==========================================
class CKingSlime : public CBoss
{
public:
	CKingSlime(); //�R���X�g���N�^
	~CKingSlime() override;//�f�X�g���N�^
	HRESULT Init() override; //����������
	void Uninit() override;  //�I������
	void Update() override;  //�X�V����
	void Draw() override;    //�`�揈��
	void SetDeath() override;//���S�t���O��ݒ�
	static CKingSlime* Create(BOSSTYPE type, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 Scale);//�{�X�𐶐�
private:
	static const int m_nMAX_MOVEPATTERNFRAME = 100;          //�ړ��p�^�[���̍ő�t���[��

	void MovePattern() override;                             //�{�X�̈ړ��p�^�[��
	void ActionPattern() override;                           //�{�X�̍s���p�^�[��
	void Action0();                                          //�s���O
	void Action1();                                          //�s���P
	void Action2();                                          //�s���Q
	void Action3();                                          //�s���R
	void Action4();                                          //�s���S
};
//====================================================================================================

//==========================================
//�f�X�G�i�K�N���X
//==========================================
class CDeathEnaga : public CBoss
{
public:
	CDeathEnaga(); //�R���X�g���N�^
	~CDeathEnaga() override;//�f�X�g���N�^
	HRESULT Init() override; //����������
	void Uninit() override;  //�I������
	void Update() override;  //�X�V����
	void Draw() override;    //�`�揈��
	void SetDeath() override;//���S�t���O��ݒ�
	static CDeathEnaga* Create(BOSSTYPE type, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 Scale);//�{�X�𐶐�
private:
	static const int m_nMAX_TWINTORNADO = 8;                 //�c�C���g���l�[�h�ő吔
	float m_fMoveLength;                                     //�ړ����鋗��
	static const int m_nMAX_MOVEPOINT = 6;                   //�ړ��ʒu�̍ő吔
	static const D3DXVECTOR3 m_MOVEPOINT[m_nMAX_MOVEPOINT];  //�ړ��ʒu

	int m_nPurposeMovePoint;                                 //�ړ���̔ԍ�
	void MovePattern() override;                             //�{�X�̈ړ��p�^�[��
	void ActionPattern() override;                           //�{�X�̍s���p�^�[��

	void Action0();                                          //�s���O
	void Action1();                                          //�s���P

	CAttackEnemy* m_apTwinTornado[m_nMAX_TWINTORNADO];       //�c�C���g���l�[�h��
	void Action2();                                          //�s���Q

	void Action3();                                          //�s���R

	void Action4();                                          //�s���S
};
//====================================================================================================

//==========================================
//�~�X�^�[�X�g�����O�N���X
//==========================================
class CMrStrong : public CBoss
{
public:
	CMrStrong(); //�R���X�g���N�^
	~CMrStrong() override;//�f�X�g���N�^
	HRESULT Init() override; //����������
	void Uninit() override;  //�I������
	void Update() override;  //�X�V����
	void Draw() override;    //�`�揈��
	void SetDeath() override;//���S�t���O��ݒ�
	static CMrStrong* Create(BOSSTYPE type, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 Scale);//�{�X�𐶐�
private:
	void MovePattern() override;                             //�{�X�̈ړ��p�^�[��
	void DecideMovePattern() override;                       //�ړ��p�^�[�������߂�
	void AdjustSize() override;                              //�{�X�̃T�C�Y�𒲐�����
	void MovePattern0();                                     //�ړ��O
	void MovePattern1();                                     //�ړ��P

	void ActionPattern() override;                           //�{�X�̍s���p�^�[��
	void Action0();                                          //�s���O
	void Action1();                                          //�s���P
	void Action2();                                          //�s���Q

	CAttackEnemy* m_pBurstMeteo;                             //�o�[�X�g���e�I�ւ̃|�C���^
	void Action3();                                          //�s���R

	void Action4();                                          //�s���S

	void Action5();                                          //�s���T�i�w���t�H�[���j
};
//====================================================================================================

//==========================================
//���C���[�N���X
//==========================================
class CRiley : public CBoss
{
public:
	CRiley(); //�R���X�g���N�^
	~CRiley() override;//�f�X�g���N�^
	HRESULT Init() override; //����������
	void Uninit() override;  //�I������
	void Update() override;  //�X�V����
	void Draw() override;    //�`�揈��
	void SetDeath() override;//���S�t���O��ݒ�
	static CRiley* Create(BOSSTYPE type, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 Scale);//�{�X�𐶐�
private:
	static const int m_nMAX_MOVEPATTERN_0_FRAME = 100;           //�ړ��p�^�[���O�̃t���[����
	static const int m_nMAX_MOVEPATTERN_1_FRAME = 100;           //�ړ��p�^�[���P�̃t���[����
	static const int m_nMAX_REFLECTIONSNOWBURST = 3;             //���ː�U���̑���
	static constexpr float m_fMOVEPATTERNLENGTH = 200.0f;    //�ړ��p�^�[���𕪊򂳂����̋��� 
	static const int m_nMAX_ACTION03_ATTACKNUM = 5;          //�A�N�V�����R�̐�ʂ̏�����
	static const int m_nMAX_ACTION2BULLETNUM = 8;                //�A�N�V�����Q�̔j�􎞔��˂���o���b�g��
	static const int m_nMAX_ICE_EIGE = 12;                   //�A�C�X�G�C�W������
	void MovePattern() override;                             //�{�X�̈ړ��p�^�[��
	void MovePattern0();                                     //�ړ��O
	void MovePattern1();                                     //�ړ��P
	void DecideMovePattern() override;                       //�ړ��p�^�[�������߂�
	void AdjustSize() override;                              //�{�X�̃T�C�Y�𒲐�����

	void ActionPattern() override;                           //�{�X�̍s���p�^�[��
	void Action0();                                          //�s���O
	void Action1();                                          //�s���P
	void Action2();                                          //�s���Q
	void Action3();                                          //�s���R
	void Action4();                                          //�s���S�i�A�C�X�G�C�W�j
	void Action5();                                          //�s���T�i���z�]���j

	bool m_bSwitchScale;                                     //�g�嗦��ς���t���O
	CRileyAttack* m_pSnowRamming;                            //��̓�����
	CRileyAttack* m_pReflectionSnowBurst[m_nMAX_REFLECTIONSNOWBURST];                  //���ː�o�[�X�g
};
//====================================================================================================

//==========================================
//�h���C���N���X
//==========================================
class CDraiya : public CBoss
{
public:
	CDraiya(); //�R���X�g���N�^
	~CDraiya() override;//�f�X�g���N�^
	HRESULT Init() override; //����������
	void Uninit() override;  //�I������
	void Update() override;  //�X�V����
	void Draw() override;    //�`�揈��
	void SetDeath() override;//���S�t���O��ݒ�
	static CDraiya* Create(BOSSTYPE type, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 Scale);//�{�X�𐶐�
private:
	static const int m_nNUMSPECIALATTACK = 10;

	static const int m_nMAX_MOVEPATTERN_0_FRAME = 150;           //�ړ��p�^�[���O�̃t���[����
	static constexpr float m_fACTION1_PARABORAJUMPSPEED = 25.0f;//�s���P�̕������W�����v�̃X�s�[�h
	static const int m_nMAX_ACTION2THUNDERNUM = 5;           //�s���Q�̃T���_�[�𔭎˂����
	static const int m_nMAX_ACTION2THUNDER_SPAN = 60;        //�s���Q�̃T���_�[�𔭎˂���Ԋu
	static constexpr float m_fACTION0_ATTACKSPEED = 15.0f;   //�s���O�̍U���̑��x
	static const int m_nMAX_ACTION03_RAMMING_NUM = 7;        //�s���R�̑̓������
	static constexpr float m_fACTION4_SUMMONTHUNDERLENGTH = 900.0f;//�s���S�̗����������鋗��
	static constexpr float m_fACTION4_THUNDERSPEED = 25.0f;  //�s���S�̗��̑���
	static const int m_nACTION4_THUNDERSUMMONSPAN = 60;      //�s���S�̗����������鑬��
	static constexpr float m_fACTION4_THUNDERSCALE_Y = 5.0f; //�s���S�̗���Y�����̊g�嗦
	static const int m_nACTION4_THUNDERSHOTNUM = 2;          //�s���S�̗��̏�����
	static constexpr float m_fACTION4_BACKMOCELENGTH = 400.0f;//�s���S�̉��ֈړ����鋗��
	void MovePattern() override;                             //�{�X�̈ړ��p�^�[��
	void MovePattern0();                                     //�ړ��O
	void DecideMovePattern() override;                       //�ړ��p�^�[�������߂�
	void AdjustSize() override;                              //�{�X�̃T�C�Y�𒲐�����

	void ActionPattern() override;                           //�{�X�̍s���p�^�[��
	void Action0();                                          //�s���O
	void Action1();                                          //�s���P
	void Action2();                                          //�s���Q
	void Action3();                                          //�s���R
	void Action4();                                          //�s���R
	CDraiyaAttack* m_pAnnihilationThunder;                   //�r�ŗ�
	bool m_bAnnihilationThunderShot[m_nMAX_ACTION2THUNDERNUM];//�r�ŗ��̔��˕����̃t���O

	D3DXVECTOR3 m_RammingSavePos[m_nMAX_ACTION03_RAMMING_NUM];//�̓�����̊J�n�ʒu
	float m_fSaveRammingRot[m_nMAX_ACTION03_RAMMING_NUM];       //�̓�����̔�ԕ�����ۑ�����

	CDraiyaAttack* m_pSpecialThunder[m_nNUMSPECIALATTACK];
	int m_nNumThunder;                                        //���̐����i�[����
};
//====================================================================================================

//==========================================
//�K�u���G���N���X
//==========================================
class CGabriel : public CBoss
{
public:
	CGabriel();              //�R���X�g���N�^
	~CGabriel() override;    //�f�X�g���N�^
	HRESULT Init() override; //����������
	void Uninit() override;  //�I������
	void Update() override;  //�X�V����
	void Draw() override;    //�`�揈��
	void SetDeath() override;//���S�t���O��ݒ�
	static CGabriel* Create(BOSSTYPE type, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 Scale);//�{�X�𐶐�
private:
	static const int m_nMAX_MOVEPATTERN0_FRAME = 120;        //�{�X�̈ړ��p�^�[���O�̃t���[����
	static const int m_nMAX_ACTION0BULLETNUM = 4;            //�s���O�̒e�̐�
	void MovePattern() override;                             //�{�X�̈ړ��p�^�[��
	void MovePattern0();                                     //�ړ��O
	void DecideMovePattern() override;                       //�ړ��p�^�[�������߂�
	void AdjustSize() override;                              //�{�X�̃T�C�Y�𒲐�����

	void ActionPattern() override;                           //�{�X�̍s���p�^�[��
	void Action0();                                          //�s���O
	void Action1();                                          //�s���P

	float m_fMoveLength;                                     //�ړ�����Ƃ��̋���
	float m_fMoveRot;                                        //�ړ�����Ƃ��̌���
	D3DXVECTOR3 m_SaveMoveFulcrumPos;                        //�ړ�����Ƃ��̎x�_�ƂȂ�ʒu
	bool m_bMoveAim;                                         //�ړ����������߂�

	CBullet* m_pCircleBullet[m_nMAX_ACTION0BULLETNUM];       //�e�ւ̃|�C���^]
	CAIModel* m_pSubAiModel;                                 //AI�p�P

};
//====================================================================================================

#endif
