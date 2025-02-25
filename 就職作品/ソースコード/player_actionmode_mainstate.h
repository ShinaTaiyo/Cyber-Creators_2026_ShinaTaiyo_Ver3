//======================================================================================
//
//�Q���P�V���F�v���C���[�A�N�V�������[�h�̃��C����ԃN���X�����[player_actionmode_mainstate.h]
//Author:ShinaTaiyo
//
//======================================================================================

//============================================
//�C���N���[�h
//============================================
#include "main.h"

//============================================
//�O���錾
//============================================
class CPlayer;
//======================================================================================

//============================================
//�Q�d�C���N���[�h�h�~
//============================================
#ifndef _PLAYER_ACTIONMODE_MAINSTATE_H_
#define _PLAYER_ACTIONMODE_MAINSTATE_H_
//======================================================================================

//�v���C���[�A�N�V�������C���X�e�[�g�N���X
class CPlayerActionMode
{
public:
	CPlayerActionMode();                    //�R���X�g���N�^
	virtual ~CPlayerActionMode();           //�f�X�g���N�^
	virtual void Process(CPlayer * pPlayer);//�����i�����Ȃ��j
};

//�v���C���[�A�N�V�����F�ˌ��ړ��N���X
class CPlayerActionMode_ShotMove : public CPlayerActionMode
{
public:
	CPlayerActionMode_ShotMove(CPlayer * pPlayer);//�R���X�g���N�^
	~CPlayerActionMode_ShotMove() override;       //�f�X�g���N�^
	void Process(CPlayer* pPlayer) override;      //����
};

//�v���C���[�A�N�V�����F�_�C�u�����N���X
class CPlayerActionMode_PrepDive : public CPlayerActionMode
{
public:
	CPlayerActionMode_PrepDive(CPlayer* pPlayer);//�R���X�g���N�^
	~CPlayerActionMode_PrepDive() override;      //�f�X�g���N�^
	void Process(CPlayer* pPlayer) override;     //����
};

//�v���C���[�A�N�V�����F���C���[���˃N���X
class CPlayerActionMode_WireShot : public CPlayerActionMode
{
public:
	CPlayerActionMode_WireShot(CPlayer* pPlayer);        //�R���X�g���N�^
	~CPlayerActionMode_WireShot() override;              //�f�X�g���N�^
	void Process(CPlayer* pPlayer) override;             //����
private:
	static constexpr float s_fWIREHEAD_SHOTSPEED = 60.0f;//���C���[�̓��𔭎˂��鑬�x
	void FrightenedEnemy(CPlayer* pPlayer);              //�G�����������鏈��
	void DecisionCameraRot(CPlayer* pPlayer);            //�J�����̌��������߂鏈��
};

//�v���C���[�A�N�V�����F�_�C�u�ړ��N���X
class CPlayerActionMode_DiveMove : public CPlayerActionMode
{
public:
	CPlayerActionMode_DiveMove(D3DXVECTOR3 Move,CPlayer * pPlayer);//�R���X�g���N�^
	~CPlayerActionMode_DiveMove() override;                        //�f�X�g���N�^
	void Process(CPlayer* pPlayer) override;                       //����
private:
	//================
	//�ÓI�����o�錾
	//================
	static constexpr float s_fCOLLISIONSTARTLENGTH = 120.0f;       //�v���C���[�����C���[�̓��ƏՓ˂��鋗��
	//===========================================================================================================
};

//�v���C���[�A�N�V�����F�_�C�u�A�^�b�N�N���X
class CPlayerActionMode_DiveAttack : public CPlayerActionMode
{
public:
	CPlayerActionMode_DiveAttack(CPlayer* pPlayer);//�R���X�g���N�^
	~CPlayerActionMode_DiveAttack() override;      //�f�X�g���N�^
	void Process(CPlayer* pPlayer) override;       //����
};

//�v���C���[�A�N�V�����F�����t���N���X
class CPlayerActionMode_Stuck : public CPlayerActionMode
{
public:
	CPlayerActionMode_Stuck(CPlayer* pPlayer);//�R���X�g���N�^
	~CPlayerActionMode_Stuck() override;      //�f�X�g���N�^
	void Process(CPlayer* pPlayer) override;  //����
private:
	bool m_bStartWireShot;                    //���C���[�̔��˂��J�n���邩�ǂ���
};

#endif