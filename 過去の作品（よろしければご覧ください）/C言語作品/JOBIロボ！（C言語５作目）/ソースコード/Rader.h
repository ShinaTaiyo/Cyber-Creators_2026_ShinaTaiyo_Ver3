//========================================================
//
//�P�Q���Q�V���F�G���[�_�[����������s���B[RADER.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _RADER_H_
#define _RADER_H_

#include "main.h"

#define MAX_RADERDISPLAY (5000)//���[�_�[�ŕ\������ő吔
#define NUM_RADERDISPLAY (5)//���[�_�[�ŕ\�������ސ�
typedef enum
{
	RADERDISPLAY00_PLAYER = 0,//�v���C���[�̈ʒu��\��
	RADERDISPLAY01_ENEMY,     //�G�̈ʒu��\��
	RADERDISPLAY02_BOSS,      //�{�X�̈ʒu��\��
	RADERDISPLAY03_OUTLET,    //�[�d��̈ʒu��\��
	RADERDISPLAY04_EDIT,      //�G�f�B�^�̈ʒu��\��
	RADERDISPLAY_MAX
}RADERDISPLAYTYPE;

typedef struct
{
	D3DXVECTOR3 pos;//�ʒu
	D3DXVECTOR3 rot;//����
	D3DXMATRIX mtxWorld;

}Rader;//���[�_�[

typedef struct
{
	D3DXVECTOR3 UserPos;//���[�_�[�f�B�X�v���C���g�p���Ă���҂̈ʒu
	D3DXVECTOR3 pos;//�ʒu
	D3DXVECTOR3 rot;//����
	int nType;//���
	bool bUse;//�g�p���Ă��邩�ǂ���
	float size;//�傫��
	float fAngle;//�A���O��
	float fLength;//����
	float fWidth;//��
	float fHeight;//�c
}RaderDisplay;//���[�_�[�f�B�X�v���C

//�v���g�^�C�v�錾

//====================
//���[�_�[
//====================
void InitRader(void);
void UninitRader(void);
void UpdateRader(void);
void DrawRader(void);

//================================

//=========================
//���[�_�[�f�B�X�v���C
//=========================
void InitRaderDisplay(void);
void UninitRaderDisplay(void);
void UpdateRaderDisplay(void);
void DrawRaderDisplay(void);
int SetRaderDisplay(int nType);
void SetPositionRaderDisPlay(int nIdxRaderDisPlay, D3DXVECTOR3 pos,float fRotZ);
void KillRaderDisplay(int nIdxRaderDisplay);
//============================================================================


//========================
//����
//========================
void InitRaderCom(void);
void UninitRaderCom(void);
void UpdateRaderCom(void);
void DrawRaderCom(void);
//======================================

#endif
