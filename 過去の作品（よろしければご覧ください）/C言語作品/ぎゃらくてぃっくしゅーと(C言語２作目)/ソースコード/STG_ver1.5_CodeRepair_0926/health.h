//========================================================
//
//�W���R�O���F�G�t�F�N�g������񌩒���[health.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _HEALTH_H_
#define _HEALTH_H

//�v���g�^�C�v�錾
void InitHealth(void);
void UninitHealth(void);
void UpdateHealth(float fHealthDamage);
void DrawHealth(void);

typedef struct
{
	bool bResultFlag2;
}Result_2;

typedef struct
{
	float fHealth;
	D3DXVECTOR3 pos;//�ʒu
}Health;

Result_2* GetResult_2(void);
Health* GetHealth(void);
#endif

