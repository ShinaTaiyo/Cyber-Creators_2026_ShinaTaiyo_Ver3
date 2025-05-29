//========================================================
//
//�P�P���V���FX�t�@�C����ǂݍ���[EDITENEMY.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _EDITENEMY_H_
#define _EDITENEMY_H_

#include "main.h"

#define NUM_EDITENEMY (100)//���f���̎��
#define MAX_EDITENEMY (100)//�ǂݍ��ރ��f���̍ő吔

typedef struct
{
	D3DXVECTOR3 pos;//�ʒu
	D3DXVECTOR3 rot;//����
	int nType;//���
	int nIdxShadow;//�e�̃C���f�b�N�X
	bool bUse;//�g�p���Ă��邩�ǂ���
}EditEnemy;

typedef enum
{
	EDITENEMYTYPE_BILL = 0,//�r��
	EDITENEMYTYPE_BRIDDGE,//��
	EDITENEMYTYPE_MAX,//�ő吔
}EDITENEMYTYPE;

//�v���g�^�C�v�錾
void InitEditEnemy(void);
void UninitEditEnemy(void);
void UpdateEditEnemy(void);
void DrawEditEnemy(void);
void SetEditEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);

#endif


#pragma once
