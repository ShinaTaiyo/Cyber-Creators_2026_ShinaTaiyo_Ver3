//========================================================
//
//�W���R�O���F�p�[�e�B�N�����[particle.h]
//Author:ShinaTaiyo
//
//=========================================================

#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "main.h"


#define MAX_PARTICLE (4096)
//typedef enum
//{
//	EFFECTSCALE_NORMAL = 0,
//	EFFECTSCALE_FAST,
//	EFFECTSCALE_MAX
//}EFFECTSCALE;



//�v���g�^�C�v�錾
void InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);
void SetParticle(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, int nLife);//pos�́u���ˈʒu�v�Amove�́u�ړ��ʁv,damage�́u�e�̍U���́v

#endif


