//========================================================
//
// ���U���g�̃X�R�A�̏����Ɋւ���w�b�_�[�t�@�C��
// Author : Atsumu Kuboichi
//
//=========================================================
#ifndef _RESULTSCORE_H_
#define _RESULTSCORE_H_

#include "main.h"

//=========================================================
// �}�N����`
//=========================================================
#define MAX_RESULTSCORE	(6)	//����
#define NUM_RESULTSCORE	(3)	//�\������X�R�A�̎��
#define MAX_RESULTENEMY	(3)	//�G���j��
#define MAX_RESULTSCRAP	(4)	//�l���X�N���b�v��

//=========================================================
// �\����
//=========================================================
typedef struct
{
	D3DXVECTOR3 pos;//�ʒu
	float fWidth;	//����
	float fHeight;	//����
	bool bUse;		//�g�p��
}ResultScore;

//=========================================================
// �v���g�^�C�v�錾
//=========================================================
void InitResultScore(void);		//����������
void UninitResultScore(void);	//�I������
void UpdateResultScore(void);	//�X�V����
void DrawResultScore(void);		//�`�揈��

#endif
