//========================================================
//
//�W���R�O���F�G�t�F�N�g������񌩒���[game.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"


//�Q�[���̏��
typedef enum
{
	GAMESTATE_NONE=0,//�������Ă��Ȃ����
	GAMESTATE_NORMAL,//�ʏ��ԁi�Q�[���i�s���j
	GAMESTATE_END,//�I����ԁi�G�S�ŁA�v���C���[���ŁA�^�C���I�[�o�[�Ȃǁj
	GAMESTATE_MAX
}GAMESTATE;

typedef enum
{
	PAUSETYPE_STAGING = 0,//�Q�[���̃A�b�v�f�[�g���~�߂āA���o��������B
	PAUSETYPE_PAUSE,//���ʂ̃|�[�Y����
	PAUSETYPE_UNLOCK,//�|�[�Y��Ԃ���������
	PAUSETYPE_MAX
}PAUSETYPE;

//�v���g�^�C�v�錾
void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);
void SetGameState(GAMESTATE state);
void SetPause(PAUSETYPE type);
GAMESTATE GetGameState(void);//�����Q�b�g����B
bool * GetUseTimer(void);

#endif

