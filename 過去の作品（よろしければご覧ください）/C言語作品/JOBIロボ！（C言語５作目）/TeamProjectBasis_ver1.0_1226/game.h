//========================================================
//
//�P�Q���Q�U���F�`�[������̊�Ս쐬[game.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"
#include "enemy.h"

//�Q�[���̏��
typedef enum
{
	GAMESTATE_NONE=0,//�������Ă��Ȃ����
	GAMESTATE_NORMAL,//�ʏ��ԁi�Q�[���i�s���j
	GAMESTATE_END,//�I����ԁi�G�S�ŁA�v���C���[���ŁA�^�C���I�[�o�[�Ȃǁj
	GAMESTATE_MAX
}GAMESTATE;

//�v���g�^�C�v�錾
void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);
void SetGameState(GAMESTATE state);
void ChangeStage(void);//�X�e�[�W�؂�ւ�����
GAMESTATE GetGameState(void);//�����Q�b�g����B
bool GetEditMode(void);//�G�f�B�^�����ǂ����̔�����擾
bool GetEditEnemyMode(void);//�G�G�f�B�^�����ǂ����̔�����擾
bool GetHouse(void);//���у��{�n�E�X�����ǂ����̔�����擾
bool GetPause(void);//�|�[�Y�����ǂ����̔�����擾
#endif
