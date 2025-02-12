//====================================================
//
//�T���Q�V���FC++�Ŋ�ՂÂ���[effect.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _EFFECT_H_  
#define _EFFECT_H_

//======================
//�C���N���[�h
//======================
#include "main.h"
#include "billboard.h"
//==========================================

//==========================================

//==========================================
//�G�t�F�N�g�N���X
//==========================================
class CEffect : public CBillboard
{
public:
	typedef enum
	{
		EFFECTTYPE00_NORMAL = 0,//���ʂ̃G�t�F�N�g
		EFFECTTYPE01_THUNDER,   //���G�t�F�N�g
		EFFECTTYPE_MAX
	}EFFECTTYPE;

	CEffect();              //�R���X�g���N�^
	~CEffect() override;    //�f�X�g���N�^
	HRESULT Init() override;//����������
	void Uninit() override; //�I������
	void Update() override; //�X�V����
	void Draw() override;   //�`�揈��
	void SetDeath() override;//���S�t���O��ݒ�
	static void Unload();   //�e�N�X�`���̃A�����[�h
	static void Create(EFFECTTYPE type,int nLife, float fWidth, float fHeight,D3DXVECTOR3 pos,D3DXCOLOR col);//�G�t�F�N�g�𐶐�
private:
	EFFECTTYPE m_Type;        //�G�t�F�N�g�̎��
	bool m_bUse;              //�g�p���
	float m_fReductionWidth;  //�����k���X�s�[�h
	float m_fReductionHeight; //�����k���X�s�[�h
	static  LPDIRECT3DTEXTURE9 m_pTextureTemp;//�e�N�X�`���̏��
	static const char* m_aEFFECT_FILENAME[EFFECTTYPE_MAX];//�G�t�F�N�g�̃t�@�C����
};
//====================================================================================================================================
#endif
