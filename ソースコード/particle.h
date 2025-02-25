//====================================================
//
//�V���Q�P���F�G��AI�����[particle.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _PARTICLE_H_  
#define _PARTICLE_H_

//======================
//�C���N���[�h
//======================
#include "main.h"
#include "billboard.h"
//==========================================

//==========================================

//==========================================
//�p�[�e�B�N���N���X
//==========================================
class CParticle : public CBillboard
{
public:
	typedef enum
	{
		TYPE00_NORMAL = 0,//���ʂ̃p�[�e�B�N���̃e�N�X�`��
		TYPE01_FIRE,      //���̃p�[�e�B�N���̃e�N�X�`��
		TYPE02_ICE,       //�X�̃p�[�e�B�N���̃e�N�X�`��
		TYPE03_THUNDER,   //�X�̃p�[�e�B�N���̃e�N�X�`��
		TYPE04_HEAL,      //�񕜂̃p�[�e�B�N���̃e�N�X�`��
		TYPE05_WING,      //�H�̃p�[�e�B�N���̃e�N�X�`��
		TYPE_MAX
	}TYPE;

	CParticle(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::PARTICLE, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_BILLBOARD);//�R���X�g���N�^
	~CParticle() override;                  //�f�X�g���N�^
	HRESULT Init() override;                //����������
	void Uninit() override;                 //�I������
	void Update() override;                 //�X�V����
	void Draw() override;                   //�`�揈��
	void SetDeath() override;               //���S�t���O��ݒ�

	//�ڍאݒ�
	void SetAddSpeed(float fAddSpeed);      //�����p�[�e�B�N���ɂ���
	void SetGravity(float fGravity);        //�d�̓p�[�e�B�N���ɂ���@
	static CParticle * Create(TYPE Type,int nLife, float fWidth, float fHeight,D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, bool bBrending);//�p�[�e�B�N���𐶐�
	static void SummonParticle(TYPE type,int nNum,int nLife, float fWidth, float fHeight,int nRandSpeed,int nDivision,bool bRandumCol,D3DXVECTOR3 Pos,D3DXCOLOR col, bool bBreanding);//�����_�������ɕ��o����p�[�e�B�N�����o��
	static void SummonChargeParticle(TYPE type, int nNum, int nLife, float fInitialSpeed,float fWidth, float fHeight,float fSummonLength, int nRandSpeed, int nDivision, bool bRandumCol,D3DXVECTOR3 SupportPos, D3DXCOLOR col, bool bBreanding);//�`���[�W����悤�ȋ����̃p�[�e�B�N�����o��
private:
	bool m_bBranding;                                  //�A���t�@�u�����f�B���O�����邩�ǂ���
	TYPE m_Type;                                       //���
	float m_fReductionWidth;                           //�����k���X�s�[�h
	float m_fReductionHeight;                          //�����k���X�s�[�h
							                           
	bool m_bAddSpeed;                                  //���������邩�ǂ���
	float m_fAddSpeed;                                 //�����x
							                           
	bool m_bUseChargePartilce;                         //�`���[�W���鋓�������邩�ǂ���
	bool m_bGravity;                                   //�d�͂������邩�ǂ���
	float m_fGravity;                                  //�d��

	D3DXVECTOR3 m_SupportPos;                          //�x�_�ƂȂ�ʒu

	static const char* m_apPARTICLE_FILENAME[TYPE_MAX];//�p�[�e�B�N���̃t�@�C����
};
//====================================================================================================================================
#endif
