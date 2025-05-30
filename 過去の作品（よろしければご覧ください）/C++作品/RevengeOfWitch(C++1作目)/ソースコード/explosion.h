//====================================================
//
//�T���Q�S���FC++�Ŋ�ՂÂ���[explosion.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _EXPLOSION_H_  
#define _EXPLOSION_H_

//======================
//�C���N���[�h
//======================
#include "main.h"
#include "object2d.h"
//==========================================

//==========================================

//==========================================
//�����N���X
//==========================================
class CExplosion : public CObject2D
{
public:
	CExplosion();                                                               //�R���X�g���N�^
	~CExplosion() override;                                                     //�f�X�g���N�^
	HRESULT Init() override;                                                    //����������
	void Uninit() override;                                                     //�I������
	void Update() override;                                                     //�X�V����
	void Draw() override;                                                       //�`�揈��
	void SetDeath() override;                                                   //���S�t���O��ݒ�
	static void Create(int nLife, float fWidth, float fHeight, D3DXVECTOR3 pos);//�o���b�g�𐶐�
private:
	int m_nLife;                                                                                                  //�̗�
	bool m_bUse;                                                                                                  //�g�p���
};

#endif
