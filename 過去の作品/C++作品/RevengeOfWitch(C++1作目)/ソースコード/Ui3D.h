//========================================================================
//
//�W���P�W���F���b�N�I�������[Ui3D.h]
//Author:ShinaTaiyo
//
//========================================================================

#ifndef _UI3D_H_  
#define _UI3D_H_

//======================
//�C���N���[�h
//======================
#include "main.h"
#include "billboard.h"

//==========================================

//======================
//�O���錾
//======================

//==========================================

//==========================================
//UI3D�N���X
//==========================================
class CUi3D : public CBillboard
{
public:
	//===========================
	//3DUi�̎��
	//===========================
	typedef enum
	{
		UI3DTYPE00_LOCKON = 0, //���b�N�I��
		UI3DTYPE01_WARNING,    //�x���\��
		UI3DTYPE_MAX
	}UI3DTYPE;
	//=========================================================================================================================

	//===========================
	//�v���g�^�C�v�錾
	//===========================
	CUi3D();                        //�R���X�g���N�^
	~CUi3D() override;              //�f�X�g���N�^
	HRESULT Init() override;        //����������
	void Uninit() override;         //�I������
	void Update() override;         //�X�V����
	void Draw() override;           //�`�揈��
	void SetDeath() override;       //���S�t���O��ݒ�
	static CUi3D* Create(UI3DTYPE type,int nLife,float fWidth, float fHeight, D3DXVECTOR3 pos, D3DXVECTOR3 move);//3DUi�𐶐�
	//=========================================================================================================================
private:
	static const char* m_apUI3D_FILENAME[UI3DTYPE_MAX];   //�t�@�C���l�[��
	UI3DTYPE m_type;                                      //3DUi�̎��
	bool m_bDrawSwitch;                                   //�`�悷�邩�ǂ���
};

#endif
