//========================================================
//
//�W���R�O���F�G�t�F�N�g������񌩒���[score.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _WAVE_H_
#define _WAVE_H_

#include "main.h"


//�e�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;//�ʒu
	//D3DXVECTOR3 move;//�ړ���
	////int nLife;//�����i�ǂ̂��炢�e�����ł������j
	bool bUse;//�e���g�p���Ă��邩�ǂ���

	int nBonusCountStart;//�E�F�[�u���Ƃɑ����N���A����قǃX�R�A���オ��J�E���g�����B

	bool bWaveFlag;//�e�E�F�[�u���J�n�������ǂ����𔻒肷��t���O

	bool bBonusCountFlag;//�E�F�[�u�J�n���Ƀ{�[�i�X�J�E���g���X�^�[�g������t���O
}Wave;



//�v���g�^�C�v�錾
void InitWave(void);
void UninitWave(void);
void UpdateWave(void);
void DrawWave(void);
bool GetWaveFlag(void);//���݂����E�F�[�u�ڂ���ǂݍ��ށB
//void SetWave(int nScore);//pos�́u���ˈʒu�v�Amove�́u�ړ��ʁv


#endif

