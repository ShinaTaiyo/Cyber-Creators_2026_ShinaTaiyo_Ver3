//====================================================
//
//�T���Q�Q���FC++�Ŋ�ՂÂ���[bullet.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _BULLET_H_  
#define _BULLET_H_

//======================
//�C���N���[�h
//======================
#include "main.h"
#include "billboard.h"
#include "effect.h"

//==========================================

//======================
//�O���錾
//======================
class CEnemy;

//==========================================

//==========================================
//�o���b�g�N���X
//==========================================
class CBullet : public CBillboard
{
public:
	//===========================
	//�e�̎��
	//===========================
	typedef enum
	{
		BULLETTYPE_PLAYER = 0, //�v���C���[�̒e
		BULLETTYPE_ENEMY,      //�G�̒e
		BULLETTYPE_MAX
	}BULLETTYPE;
	//======================================================================

	//===========================
	//�e�̃e�N�X�`���̎��
	//===========================
	typedef enum
	{
		BULLETTEX_PLAYER = 0,//�v���C���[�̒ʏ�e
		BULLETTEX_ENEMY,     //�G�̒ʏ�e
		BULLETTEX_RAIN,      //�J�̃e�N�X�`��
		BULLETTEX_MAX
	}BULLETTEX;

	//===========================
	//�������̎��
	//===========================
	typedef enum
	{
		MOVETYPE00_NORMAL = 0,//���ʂɓ���
		MOVETYPE01_GRAVITY,   //�d�͂���
		MOVETYPE_MAX
	}MOVETYPE;
	//======================================================================

	CBullet();              //�R���X�g���N�^
	~CBullet() override;    //�f�X�g���N�^
	HRESULT Init() override;//����������
	void Uninit() override; //�I������
	void Update() override; //�X�V����
	void Draw() override;   //�`�揈��
	void SetDeath() override;//���S�t���O��ݒ�
	static CBullet * Create(BULLETTYPE type,BULLETTEX tex,int nLife,int nPower,float fWidth,float fHeight,D3DXVECTOR3 pos,D3DXVECTOR3 move);//�o���b�g�𐶐�
	void Collision();       //�����蔻������
	void SetHitAddMp(int nAddMp) { m_nHitAddMp = nAddMp; }//MP�ϓ��l��ݒ肷��
	
	void SetUseDivision(bool bUse, int nNum, int nLimit) { m_bUseDivision = bUse; m_nNumDivision = nNum; m_nLimitDivision = nLimit; }
	BULLETTYPE GetBulletType() { return m_type; }

	void SetUseReflection(bool bUse) { m_bUseReflection = bUse; }//���˂��g�p���邩�ǂ�����ݒ�

	void SetBlockCollision(bool bCollision) { m_bBlockColliion = bCollision; }
	bool& GetBlockCollision() { return m_bBlockColliion; }
private:																							             
	BULLETTYPE m_type;      //�e�̎��
	MOVETYPE m_MoveType;    //�������̎��
	int m_nPower;           //�З�

	bool m_bUse;            //�g�p���
	float m_fGravity;       //�d��
	float m_fLength;        //����

	bool m_bUseDivision;    //����������邩�ǂ���
	bool m_bBlockColliion;  //�u���b�N�ɓ����邩�ǂ���
	int m_nNumDivision;     //������
	int m_nLimitDivision;   //����񐔂̏�������߂�

	bool m_bUseReflection; //���˂��g�p���邩�ǂ���

	bool m_bUseBlockCollision;//�u���b�N�Ƃ̓����蔻����g�p���邩�ǂ���

	//===========================================================
	//MP�֘A
	//===========================================================
	int m_nHitAddMp;        //�q�b�g�����Ƃ���MP��ϓ�������l
	void HitAddMpProcess(); //MP�����蓖�Ă鏈��
	//============================================================================================
	static const char* m_apBULLET_FILENAME[BULLETTEX_MAX];

	void DivisionBulletProcess(int nRadius);//�����e�̏���

	void BlockCollisionProcess();//�u���b�N�Ƃ̓����蔻����s������

};



#endif
