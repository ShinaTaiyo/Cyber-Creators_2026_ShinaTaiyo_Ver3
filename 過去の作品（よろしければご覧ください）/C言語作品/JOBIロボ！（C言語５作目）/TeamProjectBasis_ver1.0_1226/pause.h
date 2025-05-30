//========================================================
//
//�W���R�O���F�G�t�F�N�g������񌩒���[pause.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

#include "main.h"

//�}�N����`
#define NUM_PAUSETEX (3)//�|�[�Y�Ŏg���R�}���h�̎�ސ�

//�񋓌^��`
typedef enum
{
	PAUSE_MENU_CONTINUE = 0,//�R���e�B�j���[�{�^���p�e�N�X�`��
	PAUSE_MENU_RETRY,//���g���C�{�^���p�e�N�X�`��
	PAUSE_MENU_QUIT,//�N�C�b�g�{�^���p�e�N�X�`��
	PAUSE_MENU_MAX
}PAUSE_MENU;

//�\���̒�`
typedef struct
{
	int nPauseMenu;//�Ăяo���R�}���h�̔ԍ������߂�ϐ�
	D3DXVECTOR3 pos;//���S���W��\���B
	bool bChooseMenu;//�I�񂾃R�}���h�����邭�Ȃ�悤�ɂ���t���O
}Pause;


//�v���g�^�C�v�錾
void InitPause(void);
void UninitPause(void);
void UpdatePause(void);
void DrawPause(void);

void SetEnablePause(int nType);
int GetPauseMenu(int nChooseMenu);
#endif
