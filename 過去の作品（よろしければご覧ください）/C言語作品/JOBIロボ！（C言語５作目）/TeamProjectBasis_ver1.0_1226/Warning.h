//========================================================
//
//�R���P�S���F�{�X�̍U�������O�Ɍx�����o�������B[Warning.h]
//Author:ShinaTaiyo
// 
//
//=========================================================
#ifndef _WARNING_H_
#define _WARNING_H_

#include "main.h"

#define MAX_WARNING (32)//�U���r���{�[�h�̍ő吔

typedef struct
{
	D3DXVECTOR3 pos;//�ʒu
	D3DXVECTOR3 Size;//�傫��
	D3DXMATRIX mtxWorld;//�}�g���b�N�X���[���h
	int nLife;//�̗�
	float fRWidth;//���i���a�j
	float fRHeight;//�����i���a)
	bool bColChange;//�Q��ނ̐F��ς�������t���O
	bool bUse;//�g�p���Ă��邩�ǂ���
}Warning;

//�v���g�^�C�v�錾
void InitWarning(void);
void UninitWarning(void);
void UpdateWarning(void);
void DrawWarning(void);
void SetWarning(int nLife, float fRWidth, float fRHeight, D3DXVECTOR3 pos);

Warning * GetWarning (void);//�U���r���{�[�h�\���̂̏����Q�b�g����

#endif
