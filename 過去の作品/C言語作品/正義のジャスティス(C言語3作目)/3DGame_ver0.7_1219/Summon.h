//========================================================
//
//�P�P���V���FX�t�@�C����ǂݍ���[Summon.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _SUMMON_H_
#define _SUMMON_H_

#include "main.h"

#define NUM_SUMMON (100)//�G�̎��
#define MAX_SUMMON (1024)//�ǂݍ��ޓG�̍ő吔
#define SUMMONSET "data\\TEXTFILE\\Summon.txt"


//�v���g�^�C�v�錾
void InitSummon(void);
void UpdateSummon(void);
void SetSummon(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);

#endif
