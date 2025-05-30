//========================================================
//
//�W���R�O���F�G�t�F�N�g������񌩒���[explosion.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

#include "main.h"

typedef enum
{
	EXPLOSIONTYPE_NORMAL=0,//���ʂ̔���
	EXPLOSIONTYPE_NOVA,//�v���C���[�̃X�L���u�m���@�u���X�g�v�̔���
	EXPLOSIONTYPE_E6,//�G�l�~�[�U�̍U���̔���
	EXPLOSIONTYPE_MAX
}EXPLOSION;//

//�v���g�^�C�v�錾
void InitExplosion(void);
void UninitExplosion(void);
void UpdateExplosion(void);
void DrawExplosion(void);
void SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col,int nType ,float fWidth,float fHeight);

#endif