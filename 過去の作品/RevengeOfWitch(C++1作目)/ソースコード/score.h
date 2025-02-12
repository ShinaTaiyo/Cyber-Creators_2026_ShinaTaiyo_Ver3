//====================================================
//
//�X���S���F�X�R�A������ [score.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _SCORE_H_  
#define _SCORE_H_

//======================
//�C���N���[�h
//======================
#include "main.h"
#include "object2d.h"
//==========================================

//======================
//�O���錾
//======================
class CNumber;

//==========================================

//==========================================
//�X�R�A�N���X
//==========================================
class CScore : public CObject
{
public:
	CScore();                   //�R���X�g���N�^
	~CScore() override;         //�f�X�g���N�^
	HRESULT Init() override;     //����������
	void Uninit() override;      //�I������
	void Update() override;      //�X�V����
	void Draw() override;        //�`�揈��
	void SetDeath() override;    //���S�t���O��ݒ�
	static CScore* Create(int nInitialScore);//��������
	void AddScore(int nScore);
private:
	static const int m_nMAX_SCOREDIGIT = 5;//�X�R�A�̍ő包��
	CNumber* m_pNumber[m_nMAX_SCOREDIGIT];//�����ւ̃|�C���^
	int m_nScore;//�X�R�A
};

#endif
