//==================================================================================
//
//�U���R���F�RD�I�u�W�F�N�g�N���X���쐬����[3Dobject.h]
//Author:ShinaTaiyo
//
//===================================================================================
#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

//===================================================================================
//�C���N���[�h
//===================================================================================
#include "main.h"
#include "object.h"

//===================================================================================

//===================================================================================
//3D�I�u�W�F�N�g�N���X
//===================================================================================
class CObject3D : public CObject
{
public:
	CObject3D(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::NONE, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_3D);                                                           //�R���X�g���N�^
	~CObject3D();                                                          //�f�X�g���N�^
	HRESULT Init(void) override;                                           //����������
	void Uninit(void) override;                                            //�I������
	void Update(void) override;                                            //�X�V����
	void Draw(void) override;                                              //�`�揈��
	void SetDeath() override;                                              //���S�t���O�ݒ菈��
	void bindTexture(LPDIRECT3DTEXTURE9 pTexture);                         //�e�N�X�`�������蓖�Ă�

	void SetWidth(float fWidth) { m_fWidth = fWidth; }                     //����
	void SetHeight(float fHeight) { m_fHeight = fHeight; }                 //����
	void SetRot(D3DXVECTOR3 Rot) { m_Rot = Rot; }                          //����
	void SetPos(D3DXVECTOR3 Pos) { m_Pos = Pos; }                          //�ʒu��ݒ�
	void SetTextureIndex(int nIdx) { m_nTextureIndex = nIdx; }             //�e�N�X�`���ԍ����Z�b�g����
	int GetTextureIndex() { return m_nTextureIndex; }                      //�e�N�X�`���ԍ����擾����

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;                                    //���_�o�b�t�@�ւ̃|�C���^!
	LPDIRECT3DTEXTURE9 m_pTexture;                                         //�e�N�X�`���ւ̃|�C���^"!
	D3DXVECTOR3 m_Pos;                                                     //�ʒu!
	D3DXVECTOR3 m_Rot;                                                     //����!
	D3DXMATRIX m_mtxWorld;                                                 //���[���h�ϊ��s��!
	float m_fWidth;                                                        //����!
	float m_fHeight;                                                       //����!
	int m_nTextureIndex;                                                   //�e�N�X�`���ԍ��i�S�ẴI�u�W�F�N�g3D�h���N���X�������Ă���j!
};
//===================================================================================


#endif