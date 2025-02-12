//=======================================================================================
//
//�U���S���F���f����`�悷��[field.h]
//Author:ShinaTaiyo
//
//=======================================================================================
#ifndef _FIELD_H_
#define _FIELD_H_

//==============================================
//�C���N���[�h
//==============================================
#include "main.h"
#include "object.h"
#include "object3d.h"
//=======================================================================================

//=======================================================
//�t�B�[���h�N���X
//=======================================================
class CField : public CObject3D
{
public:
	CField();                                                                       //�R���X�g���N�^
	~CField() override;                                                             //�f�X�g���N�^
	HRESULT Init() override;                                                        //����������
	void Uninit() override;                                                         //�I������
	void ExtraUninit() override;                                                    //�ʘg�̏I������
	void Update() override;                                                         //�X�V����
	void Draw() override;                                                           //�`�揈��
	void SetDeath() override;                                                       //���S�t���O��ݒ�
	static void Unload();                                                                  //�A�����[�h����
	static CField* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight);  //��������
private:
	bool m_bUse;                                                                    //�g�p���Ă��邩�ǂ���
	static LPDIRECT3DTEXTURE9 m_pTextureTemp;                                       //�e�N�X�`��
};
//============================================================================================================

#endif