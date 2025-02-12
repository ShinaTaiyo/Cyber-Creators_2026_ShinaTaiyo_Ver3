//====================================================
//
//�V���P�U���F�g���b�v����[trap.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _TRAP_H_  
#define _TRAP_H_

//======================
//�C���N���[�h
//======================
#include "main.h"
#include "objectX.h"
//==========================================

//======================
//�O���錾
//======================
class CBlock;

//==========================================

//==========================================
//�g���b�v�N���X
//==========================================
class CTrap : public CObjectX
{
public:
	//===========================
	//�g���b�v�̎��
	//===========================
	typedef enum
	{
		TRAPTYPE00_NEEDLEBALL = 0,
		TRAPTYPE01_CANNON,
		TRAPTYPE02_THORNCLUB,
		TRAPTYPE_MAX
	}TRAPTYPE;
	//======================================================================

	//===========================
	//�����蔻��̎��
	//===========================
	typedef enum
	{
		COLLISIONTYPE00_SQUARE = 0,
		COLLISIONTYPE01_RECTANGLESQUARE,
		COLLISIOINTYPE_MAX
	}COLLISIONTYPE;

	CTrap();                    //�R���X�g���N�^
	~CTrap() override;          //�f�X�g���N�^
	HRESULT Init() override;    //����������
	void Uninit() override;     //�I������
	void Update() override;     //�X�V����
	void Draw() override;       //�`�揈��
	void SetDeath() override;   //���S�t���O��ݒ�
	static CTrap* Create(TRAPTYPE type, D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, D3DXVECTOR3 Scale, int nRotType,int nSubType);//�g���b�v�𐶐��i��{�����j
	static int GetNumFile() { return m_nNumFile; }           //�t�@�C���̐����擾����
	static const int* GetSubTypeNum() { return &m_nTRAPSUBTYPENUM[0]; }            //�T�u�^�C�v�̂O�Ԗڂ��擾����
	int GetSubType() { return m_nSubType; }                                        //�T�u�^�C�v���擾
	void SetInfo(float fStartRot,float fSpeed, float fRotSpeed,float fMoveLength,float fTurnSpeed,int nInterval,float fAddSpeed); //����ݒ�

	float& GetStartRot() { return m_fStartRot; }                                   //��]�J�n�ʒu���擾
	float& GetSpeed() { return m_fSpeed; }                                         //�������擾
	float& GetMoveLength() { return m_fMoveLength; }                               //�ړ��������擾
	float& GetRotSpeed() { return m_fRotSpeed; }                                   //��]���x���擾
	float& GetTurnSpeed() {return m_fTurnSpeed; }                                  //U�^�[������X�s�[�h���擾  
	int& GetInterval() { return m_nInterval; }                                     //�l�X�ȃC���^�[�o�����擾
	float& GetAddSpeed() { return m_fAddSpeed; }                                   //�����x
	COLLISIONTYPE GetCollisionType() { return m_CollisionType; }                   //�����蔻��̎�ނ��擾����

	TRAPTYPE GetTrapType() { return m_type; }                                      //�g���b�v�̎�ނ��擾����
private:													                       
	static const int m_nTRAPSUBTYPENUM[TRAPTYPE_MAX];                              //�g���b�v�̃T�u�^�C�v
	TRAPTYPE m_type;                                                               //�G�̎��
	COLLISIONTYPE m_CollisionType;                                                 //�����蔻��̎��
	bool m_bUse;                                                                   //�g�p���
	bool m_bUseMove;                                                               //�ړ��ʂ��g�p���邩�ǂ���
	bool m_bSpeedSwitch;                                                           //�f������ς���t���O
	bool m_bSpeedSwitch2;                                                          //��L���̂Q
	bool m_bTurn;                                                                  //�^�[��������ς���t���O
	int m_nSubType;                                                                //�T�u�^�C�v
	int m_nCntTime;                                                                //�o�����Ă���̃t���[�����𑪂�
	static const char* m_TRAPFILENAME[TRAPTYPE_MAX];                               //�A�C�e���̃t�@�C���̖��O
	static int m_nNumFile;                                                         //�t�@�C���̐�
															                       
	float m_fStartRot;                                                             //�J�n�ʒu
	float m_fSpeed;                                                                //����
	float m_fMoveLength;                                                           //�ړ�����
	float m_fRotSpeed;                                                             //��]�X�s�[�h
	float m_fTurnSpeed;                                                            //�^�[������X�s�[�h
	float m_fAddSpeed;                                                             //�����x
	int m_nCntSetTime;                                                             //�l�X�Ȏ��Ԃ�ݒ�
	int m_nInterval;                                                               //�l�X�ȃC���^�[�o��

	void Collision(void);                                                          //�����蔻����s��
	void NeedleBallProcess(void);                                                  //�g�Q�{�[���̏���
	void CannonProcess(void);                                                      //��C�̏���
	void ThornClubProcess();                                                       //�g�Q����_�̏���

	void DispInfo() override;                                                      //���\������
};

#endif
