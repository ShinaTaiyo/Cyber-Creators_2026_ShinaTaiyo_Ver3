//====================================================
//
//�X���P�T���F�{�X�o�ꉉ�o������[particle2d.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _PARTICLE2D_H_  
#define _PARTICLE2D_H_

//======================
//�C���N���[�h
//======================
#include "main.h"
#include "object2d.h"
//==========================================

//==========================================

//==========================================
//�p�[�e�B�N���QD�N���X
//==========================================
class CParticle2D : public CObject2D
{
public:
	CParticle2D(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::PARTICLE2D, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_2D);//�R���X�g���N�^
	~CParticle2D() override;     //�f�X�g���N�^
	HRESULT Init() override;     //����������
	void Uninit() override;      //�I������
	void Update() override;      //�X�V����
	void Draw() override;        //�`�揈��
	void SetDeath() override;    //���S�t���O��ݒ�
	static CParticle2D* Create(D3DXVECTOR3 pos,D3DXVECTOR3 Move,D3DXVECTOR3 Rot,CObject2D::POLYGONTYPE PolygonType,int nLife, float fWidth, float fHeight,D3DXCOLOR col);//��������
private:
};

#endif
#pragma once
