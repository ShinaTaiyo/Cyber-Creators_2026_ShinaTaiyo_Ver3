//========================================================
//
//�W���R�O���F�G�t�F�N�g������񌩒���[effect.cpp]
//Author:ShinaTaiyo
//
//=========================================================

#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "main.h"

typedef enum
{
	EFFECTSCALE_NORMAL=0,
	EFFECTSCALE_FAST,
	EFFECTSCALE_MAX
}EFFECTSCALE;

//�v���g�^�C�v�錾
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void SetEffect(D3DXVECTOR3 pos,D3DXCOLOR col,float fRadius, int nLife,int nType);//pos�́u���ˈʒu�v�Amove�́u�ړ��ʁv,damage�́u�e�̍U���́v

#endif

