//========================================================
//
//�W���R�O���F�G�t�F�N�g������񌩒���[bg.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _BG_H_
#define _BG_H

typedef enum
{
	BG_CENTRALCIRCLE,//�Z���g�����T�[�N���w�i
	BG_STAGE1,//�X�e�[�W�P�w�i
	BG_STAGE2,//�X�e�[�W�Q�w�i
	BG_STAGE3,//�X�e�[�W�R�w�i
	BG_STAGE4,//�X�e�[�W�S�w�i
	BG_LASTSTAGE,//���X�g�X�e�[�W�w�i
	BG_MAX
}BG;

//�v���g�^�C�v�錾
void InitBG(void);
void UninitBG(void);
void UpDateBG(void);
void DrawBG(void);

#endif
