//======================================================================
//
//�U���Q�T���F���[�v�}�[�J�[����������[number3d.h]
//Author:ShinaTaiyo
//
//======================================================================

#ifndef _NUMBER3D_H_  
#define _NUMBER3D_H_

//======================
//�C���N���[�h
//======================
#include "main.h"
#include "billboard.h"
//==========================================

//==========================================

//==========================================
//�i���o�[3D�N���X
//==========================================
class CNumber3D : public CBillboard
{
public:

	CNumber3D(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::NUMBER3D, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_BILLBOARD);//�R���X�g���N�^
	~CNumber3D() override;                                                                                  //�f�X�g���N�^
	HRESULT Init() override;                                                                                //����������
	void Uninit() override;                                                                                 //�I������
	void Update() override;                                                                                 //�X�V����
	void Draw() override;                                                                                   //�`�揈��
	void ExtraDraw();                                                                                       //�ʘg�̕`�揈��
	void SetDeath() override;                                                                               //���S�t���O��ݒ�
	static CNumber3D * Create(float fWidth, float fHeight, D3DXVECTOR3 pos, D3DXVECTOR3 move,D3DXCOLOR col);//��������
private:
};

#endif
