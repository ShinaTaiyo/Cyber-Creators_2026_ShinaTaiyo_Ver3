//====================================================
//
//�X���W���F�_���[�W������ [damage.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _DAMAGE_H_  
#define _DAMAGE_H_

//======================
//�C���N���[�h
//======================
#include "main.h"
#include "object.h"
//==========================================

//======================
//�O���錾
//======================
class CNumber3D;

//==========================================

//==========================================
//�_���[�W�N���X
//==========================================
class CDamage: public CObject
{
public:
	CDamage();                   //�R���X�g���N�^
	~CDamage() override;         //�f�X�g���N�^
	HRESULT Init() override;     //����������
	void Uninit() override;      //�I������
	void Update() override;      //�X�V����
	void Draw() override;        //�`�揈��
	void SetDeath() override;    //���S�t���O��ݒ�
	static CDamage* Create(int nDamage,D3DXVECTOR3 Pos,D3DXCOLOR col,float fWidth,float fHeight);//��������
private:
	static const int m_nMAX_DAMAGEDIGIT = 3;//�_���[�W�̍ő包��
	static const int m_nMAX_JUMPCNT = 4;    //�_���[�W�\�L���W�����v������ő吔

	void DamageDispProcess();                    //�_���[�W��\�����鏈��
	CNumber3D* m_pNumber3D[m_nMAX_DAMAGEDIGIT];  //�����ւ̃|�C���^
	D3DXVECTOR3 m_Pos;                           //�ʒu
	D3DXVECTOR3 m_JumpMove;                      //�W�����v�����鎞�̈ړ���

	int m_nJumpCnt;                              //�_���[�W�\�L���W�����v������J�E���g
	int m_nDamage;                               //�_���[�W
	int m_nDigit;                                //����

	float m_fWidth;                              //�����̉���
	float m_fHeight;                             //�����̍���
};

#endif
