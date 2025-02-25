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
	enum class EFFECTTYPE
	{
		NORMAL = 0,//���ʂ̃G�t�F�N�g
		MAX        //�ő�
	};

	CEffect(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::EFFECT, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_BILLBOARD);              //�R���X�g���N�^
	~CEffect() override;         //�f�X�g���N�^
	HRESULT Init() override;     //����������
	void Uninit() override;      //�I������
	void Update() override;      //�X�V����
	void Draw() override;        //�`�揈��
	void SetDeath() override;    //���S�t���O��ݒ�
	static CEffect * Create(EFFECTTYPE type,int nLife, float fWidth, float fHeight,D3DXVECTOR3 pos,D3DXCOLOR col);//�G�t�F�N�g�𐶐�
private:
	//===========================================
	//�ÓI�����o
	//===========================================
	static const string s_EffectFileName[static_cast<int>(EFFECTTYPE::MAX)];//�G�t�F�N�g�̃t�@�C����
	//=================================================================================================

	//===========================================
	//�ϐ�
	//===========================================
	EFFECTTYPE m_Type;                                                     //�G�t�F�N�g�̎��
	float m_fReductionWidth;                                               //�����k���X�s�[�h
	float m_fReductionHeight;                                              //�����k���X�s�[�h
	//=================================================================================================
};
//====================================================================================================================================
#endif
