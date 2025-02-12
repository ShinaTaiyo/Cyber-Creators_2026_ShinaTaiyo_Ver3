//====================================================
//
//�U���Q�P���FC++�Ŋ�ՂÂ���[bg3d.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _BG3D_H_  
#define _BG3D_H_

//======================
//�C���N���[�h
//======================
#include "main.h"
#include "billboard.h"
#include "meshcylinder.h"
//==========================================

//==========================================

//==========================================
//�RD�w�i�N���X
//==========================================
class CBg3D : public CMeshCylinder
{
public:
	//===========================
	//�e�̎��
	//===========================
	typedef enum
	{
		BG3DTYPE_STAGE00 = 0, //�X�e�[�W�O�w�i
		BG3DTYPE_STAGE01,     //�X�e�[�W�P�w�i
		BULLETTYPE_MAX
	}BG3DTYPE;
	//======================================================================
	CBg3D();              //�R���X�g���N�^
	~CBg3D() override;    //�f�X�g���N�^
	HRESULT Init() override;//����������
	void Uninit() override; //�I������
	void ExtraUninit() override;//�ʘg�̏I������
	void Update() override; //�X�V����
	void Draw() override;   //�`�揈��
	void SetDeath() override;//���S�t���O��ݒ�
	static CBg3D * Create(BG3DTYPE type,D3DXVECTOR3 Pos,int nNumBlockXZ,int nNumBlockY,float fRadius,float fHeight);//3D�w�i�𐶐�
	void SetBg3DType(BG3DTYPE type);

private:
	BG3DTYPE m_type;      //�e�̎��
};

#endif
