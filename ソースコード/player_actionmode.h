//======================================================================================
//
//�P�P���P�O���F�v���C���[�A�N�V�������[�h�N���X�����[player_actionmode.h]
//Author:ShinaTaiyo
//
//======================================================================================

//============================================
//�Q�d�C���N���[�h�h�~
//============================================
#ifndef _PLAYER_ACTIONMODE_H_
#define _PLAYER_ACTIONMODE_H_
//======================================================================================

//============================================
//�C���N���[�h
//============================================
#include "main.h"
#include "object2d.h"
//======================================================================================

//============================================
//�O���錾
//============================================
class CPlayer;

//�X�[�p�[�N���X
class CPlayerMove
{
public:
	CPlayerMove();                                              //�R���X�g���N�^
	virtual ~CPlayerMove();                                     //�f�X�g���N�^
	virtual void MoveProcess(CPlayer* pPlayer);                 //�ړ�����
	void JumpProcess(CPlayer* pPlayer);                         //�W�����v����
	void DodgeProcess(CPlayer* pPlayer);                        //�������
	const bool& GetDodge() const { return m_bDodge; }           //��𒆂��ǂ������擾
	void SetDodge(bool bDodge) { m_bDodge = bDodge; }           //������Ă��邩�ǂ�����ݒ�
	const bool& GetLanding() const { return m_bIsLanding; }     //�n�ʂɂ��邩�ǂ������擾
	void SetLanding(bool bLanding) { m_bIsLanding = bLanding; } //�n�ʂɂ��邩�ǂ�����ݒ�
private:
	//*�ÓI�����o�錾
	static constexpr float s_fACCELL_PARAM = 7.0f;              //��𑬓x

	//*�ϐ��錾
	bool m_bIsLanding;                                          //�n�ʂɂ��邩�ǂ���
	bool m_bDodge;                                              //������Ă��邩�ǂ���
};

//�ʏ�ړ��N���X
class CPlayerMove_Normal : public CPlayerMove
{
public:
	CPlayerMove_Normal();                                       //�R���X�g���N�^
	~CPlayerMove_Normal() override;                             //�f�X�g���N�^
	void MoveProcess(CPlayer* pPlayer) override;                //�ړ�����

};

//�_�C�u�ړ��N���X
class CPlayerMove_Dive : public CPlayerMove
{
public:
	CPlayerMove_Dive();                                         //�R���X�g���N�^
	~CPlayerMove_Dive() override;                               //�f�X�g���N�^
	void MoveProcess(CPlayer* pPlayer) override;                //�ړ�����
	void SetDiveMove(D3DXVECTOR3 Move) { m_DiveMove = Move; }   //�_�C�u���̈ړ��ʂ�ݒ�
private:
	static constexpr float s_fDIVEMOVE = 60.0f;                 //�_�C�u�ړ������C���[�w�b�h�Ɠ����鋗���i�_�C�u���̓��C���[�̓��Ɍ������Ĕ��)
	D3DXVECTOR3 m_DiveMove;                                     //�_�C�u�̈ړ���  
};

//�_�C�u�����ړ��N���X
class CPlayerMove_PrepDive : public CPlayerMove
{
public:
	CPlayerMove_PrepDive(CPlayer * pPlayer);                      //�R���X�g���N�^
	~CPlayerMove_PrepDive() override;                             //�f�X�g���N�^
	void MoveProcess(CPlayer* pPlayer) override;                  //�ړ�����
};

//�ړ��ł��Ȃ��N���X
class CPlayerMove_Dont : public CPlayerMove
{
public:
	CPlayerMove_Dont();                                           //�R���X�g���N�^
	~CPlayerMove_Dont() override;                                 //�f�X�g���N�^
	void MoveProcess(CPlayer* pPlayer) override;                  //�ړ�����
private:
};
//======================================================================================



//==============================================
//�U���N���X
//==============================================

//�X�[�p�[�N���X
class CPlayerAttack
{
public:
	CPlayerAttack() {};                             //�R���X�g���N�^
	virtual ~CPlayerAttack() {};                    //�f�X�g���N�^
	virtual void AttackProcess(CPlayer* pPlayer) {};//�U������
};

//�ˌ��N���X
class CPlayerAttack_Shot : public CPlayerAttack
{
public:
	CPlayerAttack_Shot();                         //�R���X�g���N�^
	~CPlayerAttack_Shot() override;               //�f�X�g���N�^
	void AttackProcess(CPlayer* pPlayer) override;//�U������
private:
	static const float s_fNORMAL_SHOTSPEED;       //�ʏ�̎ˌ����x
};

//�_�C�u�N���X
class CPlayerAttack_Dive : public CPlayerAttack
{
public:
	CPlayerAttack_Dive();                             //�R���X�g���N�^
	~CPlayerAttack_Dive() override;                   //�f�X�g���N�^
	void AttackProcess(CPlayer* pPlayer) override;    //�U������
};

//�U���ł��Ȃ��N���X
class CPlayerAttack_Dont : public CPlayerAttack
{
public:
	CPlayerAttack_Dont();                             //�R���X�g���N�^
	~CPlayerAttack_Dont() override;                   //�f�X�g���N�^
	void AttackProcess(CPlayer* pPlayer) override;    //�U������
};
//======================================================================================
#endif