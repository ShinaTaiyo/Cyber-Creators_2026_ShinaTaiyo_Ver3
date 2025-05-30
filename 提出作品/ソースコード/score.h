//===============================================================================
//
//�Q���Q�T���F�X�R�A����������[score.h]
//Author:ShinaTaiyo
//
//===============================================================================

//========================================
//�Q�d�C���N���[�h�h�~
//========================================
#ifndef _SCORE_H_  
#define _SCORE_H_
//===============================================================================


//========================================
//�C���N���[�h
//========================================
#include "main.h"
#include "object.h"
#include "number.h"
//===============================================================================

//========================================
//�X�R�A�N���X
//========================================
class CScore : public CObject

{
public:
	CScore(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::SCORE, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_NONE);//�R���X�g���N�^
	~CScore() override;                                            //�f�X�g���N�^
	HRESULT Init() override;                                       //����������
	void Uninit() override;                                        //�I������
	void Update() override;                                        //�X�V����
	void Draw() override;                                          //�`�揈��
	void SetDeath() override;                                      //���S�t���O�ݒ菈��
	void SetScore(int nScore);                                     //�X�R�A��ݒ肷��
	void AddScore(int nScore);                                     //�X�R�A�����Z����
	void SubScore(int nScore);                                     //�X�R�A�����Z����
	static CScore* Create(D3DXVECTOR3 Pos);                        //��������
	static constexpr float GetNumberWidth() { return s_fWIDTH; }   //�����̉������擾
	static constexpr float GetNumberHeight() { return s_fHEIGHT; } //�����̍������擾
private:
	//=================
	//�ÓI�����o
	//=================
	static constexpr int s_nNUMDIGIT = 8;         //����
	static constexpr float s_fWIDTH = 40.0f;      //�����̉���
	static constexpr float s_fHEIGHT = 40.0f;     //�����̍���
	//===========================================================

	//=================
	//�֐�
	//=================
	void SetUpdateNumberDisp();                   //�����̕\�����X�V����
	//===========================================================
	
	//=================
	//�����o�ϐ�
	//=================
	int m_nScore;                                 //�X�R�A
	CNumber* m_pNumber[s_nNUMDIGIT];              //�����ւ̃|�C���^
	D3DXVECTOR3 m_Pos;                            //�ʒu
	//===========================================================
};
//===============================================================================

#endif
