//========================================================
//
//�P�Q���Q�V���F���x������������s���B[Level.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _LEVEL_H_
#define _LEVEL_H_

#include "main.h"

#define NUM_LEVEL (4)//���x��UI�̎�ސ�
#define LEVEL_DIGIT (2)//���x�����l�̌���
#define LEVELRADIUSSIZE (20.0f)//���x���̔��a�Q�Ƃ̑傫��
#define MAX_LEVELUPDISPLAY (10)//���x���A�b�v�\���̍ő吔

typedef struct
{
	D3DXVECTOR3 pos;//�ʒu
	int nType;//���
	int nLevel;//���݂̃��x��
}Level;

typedef struct
{
	D3DXVECTOR3 pos;//�ʒu
	float fPosTexU;//�����̕\���p
}LevelNum;

typedef struct
{
	D3DXVECTOR3 pos;//�ʒu
	D3DXCOLOR col;  //�F
	bool bUse;      //�g�p���
	int nLife;      //�̗�
	int nType;      //���
	D3DXMATRIX mtxWorld;//�}�g���b�N�X���[���h
}LevelUpDisplay;



//�v���g�^�C�v�錾
void InitLevel(void);
void UninitLevel(void);
void UpdateLevel(void);
void DrawLevel(void);

void InitLevelUpDisplay(void);
void UninitLevelUpDisplay(void);
void UpdateLevelUpDisplay(void);
void DrawLevelUpDisplay(void);
void SetLevelUpDisplay(D3DXVECTOR3 pos, D3DXCOLOR col,int nLife);

void InitLevelUpDisplay2D(void);
void UninitLevelUpDisplay2D(void);
void UpdateLevelUpDisplay2D(void);
void DrawLevelUpDisplay2D(void);
void SetLevelUpDisplay2D(D3DXVECTOR3 pos, D3DXCOLOR col, int nLife,int nType);

Level* GetLevel(void);//�e�\���̂̏����Q�b�g����
void AddLevel(int nType);//���x�����グ��B
#endif




