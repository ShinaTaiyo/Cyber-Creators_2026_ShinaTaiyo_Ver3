//====================================================
//
//�P�Q���P�X���F�C�x���g�}�l�[�W���[���쐬[number.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _NUMBER_H_  
#define _NUMBER_H_

//======================
//�C���N���[�h
//======================
#include "main.h"
#include "object2d.h"
//==========================================

//==========================================

//==========================================
//�i���o�[�N���X
//==========================================
class CNumber : public CObject2D
{
public:
	CNumber(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::NUMBER3D, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_BILLBOARD);//�R���X�g���N�^
	~CNumber() override;                                                    //�f�X�g���N�^
	HRESULT Init() override;                                                //����������
	void Uninit() override;                                                 //�I������
	void Update() override;                                                 //�X�V����
	void Draw() override;                                                   //�`�揈��
	void SetDeath() override;                                               //���S�t���O��ݒ�
	void SetNumber(int nNum) {m_nNumber = nNum; }                           //������ݒ�
	static CNumber* Create(D3DXVECTOR3 pos,float fWidth,float fHeight);     //��������
private:
	int m_nNumber;                                                          //�������ǂꂩ
};

#endif
