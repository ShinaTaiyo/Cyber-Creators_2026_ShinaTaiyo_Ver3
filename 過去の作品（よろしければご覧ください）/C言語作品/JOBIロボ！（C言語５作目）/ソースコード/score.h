//========================================================
//
//�X�R�A�̏����Ɋւ���w�b�_�[�t�@�C��
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _SCORE_H_
#define _SCORE_H_

#include "main.h"

//=========================================================
//�}�N����`
//=========================================================
#define MAX_SCORE	(6)		//�ő吔
#define	SCORE_POSX	(420.0f)//X���W
#define	SCORE_POSY	(80.0f)	//Y���W

//=========================================================
//�\���̂̒�`
//=========================================================
typedef struct
{
	D3DXVECTOR3 pos;//�ʒu
	bool bUse;//�g�p���Ă��邩�ǂ���
}Score;

//=========================================================
//�v���g�^�C�v�錾
//=========================================================
void InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);
void SetScore(int nScore);
void AddScore(int nValue);
int GetScore(void);

#endif
