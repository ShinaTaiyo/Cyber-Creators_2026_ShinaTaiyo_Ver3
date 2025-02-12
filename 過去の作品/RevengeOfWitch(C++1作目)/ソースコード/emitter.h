//=======================================================================================================
//
//�V���X���F�G�~�b�^�[����[emittter.h]
//Author:ShinaTaiyo
//
//=======================================================================================================

#ifndef _EMITTER_H_
#define _EMITTER_H_

//======================
//�C���N���[�h
//======================
#include "main.h"
#include "billboard.h"
//==========================================

//======================
//�O���錾
//======================
class CEffect;

//==========================================

//==========================================
//�G�~�b�^�[�N���X
//==========================================
class CEmitter : public CBillboard
{
public:
	CEmitter();              //�R���X�g���N�^
	~CEmitter() override;    //�f�X�g���N�^
	HRESULT Init() override; //����������
	void Uninit() override;  //�I������
	void Update() override;  //�X�V����
	void Draw() override;    //�`�揈��
	void SetDeath() override;//���S�t���O��ݒ�
	static CEmitter * Create(int nEffectType,int nLife,int nSetLife,float fLength, float fWidth, float fHeight,D3DXVECTOR3 pos,D3DXVECTOR3 move,D3DXCOLOR col);//�G�~�b�^�[�𐶐�
	void SetUseRotate(bool bUse, float fLength, D3DXVECTOR3 Rot, D3DXVECTOR3 AddRot)
	{m_bUseRotate = bUse; m_fLength = fLength; m_Rot = Rot; m_AddRot = AddRot;}//��]���g�p���邩�ǂ���
private:
	D3DXVECTOR3 m_AddRot;       //���Z��]��

	int m_nEffectType;          //�ݒ肷��G�t�F�N�g�̎��
	bool m_bUse;                //�g�p���
	
	bool m_bUseRotate;          //��]���g�p���邩�ǂ���
	float m_fLength;            //�����v�Z�p
	D3DXVECTOR3 m_Rot;          //����

	float m_fSetEffectWidth;    //�ݒ肷��G�t�F�N�g�̉���
	float m_fSetEffectHeight;   //�ݒ肷��G�t�F�N�g�̍���
	int m_nSetEffectLife;       //�ݒ肷��G�t�F�N�g�̗̑�
	D3DXCOLOR m_SetEffectColor; //�ݒ肷��G�t�F�N�g�̐F����
};
//====================================================================================================================================
#endif
