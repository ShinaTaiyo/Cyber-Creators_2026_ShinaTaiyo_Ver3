//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// �T�E���h�ꗗ
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM000 = 0,		// BGM0
	SOUND_LABEL_BGM001,			// BGM1
	SOUND_LABEL_BGM002,			// BGM2
	SOUND_LABEL_BGM003,			// BGM3
	SOUND_LABEL_SE_SHOT,		// �e���ˉ�
	SOUND_LABEL_SE_HIT,			// �q�b�g��
	SOUND_LABEL_SE_DEFEAT_ENEMY,// ������
	SOUND_LABEL_SE_EXPLOSION_ENEMY_000,// �G������
	SOUND_LABEL_SE_SHOT_ENEMY_000,//�G�̒e���ˉ��i�P�e���j
	SOUND_LABEL_SE_SHOT_ENEMY_001,//�G�̒e���ˉ��i�y��j
	SOUND_LABEL_SE_SHOT_ENEMY_002,//�G�̒e���ˉ��i�h�b�J���j
	SOUND_LABEL_SE_SHOT_ENEMY_003,//�G�̒e���ˉ��i�o�[���I�j
	SOUND_LABEL_SE_SHOT_ENEMY_004,//�G�̒e���ˉ��i�s���b�I�j
	SOUND_LABEL_SE_SHOT_ENEMY_005,//�G�̒e���ˉ��i�s�V�����I�j
	SOUND_LABEL_SE_SHOT_ENEMY_006,//�G�̒e���ˉ��i�s�b�I�j
	SOUND_LABEL_SE_SHOT_ENEMY_007,//�G�̒e���ˉ��i�V���b�g�K���j
	SOUND_LABEL_SE_SHOT_ENEMY_008,//�G�̒e���ˉ��i���̉��j
	SOUND_LABEL_SE_DECISION_000,//����{�^�����ʉ�
	SOUND_LABEL_SE_SELECT_000,//�I���{�^�����ʉ�
	SOUND_LABEL_SE_SLASH_000,//�a�����ʉ�
	SOUND_LABEL_SE_HEAL_000,//�񕜌��ʉ�
	SOUND_LABEL_SE_ARROWSHOT_000,//�|���ˉ�
	SOUND_LABEL_SE_WARNING_000,//�x����
	SOUND_LABEL_SE_WARNING_001,//�e���x����
	SOUND_LABEL_SE_DAMAGE_000,//�v���C���[��e��
	SOUND_LABEL_SE_SHIELD_000,//�V�[���h�W�J��
	SOUND_LABEL_SE_APPLAUSE_000,//�X�R�A�����N�C�����̔��艹
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
