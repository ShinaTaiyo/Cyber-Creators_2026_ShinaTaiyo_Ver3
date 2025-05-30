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
	SOUND_LABEL_BGM000 = 0,		                //BGM0
	SOUND_LABEL_BGM001,			                //BGM1
	SOUND_LABEL_BGM002,			                //BGM2
	SOUND_LABEL_BGM003,			                //BGM3
	SOUND_LABEL_BGM004,                         //BGM4 mement
	SOUND_LABEL_BGM005,                         //BGM5 yaiba
	SOUND_LABEL_BGM006,			                //BGM6_stage1
	SOUND_LABEL_BGM007,			                //BGM7_stage2
	SOUND_LABEL_BGM008,			                //BGM8_stage3
	SOUND_LABEL_BGM009,                         //BGM9_stage4
	SOUND_LABEL_BGM010,                         //BGM10_stage5
	SOUND_LABEL_BGM011,                         //BGM11_home
	SOUND_LABEL_BGM012,                         //BGM12_���A��΁I�H
	SOUND_LABEL_BGM013,                         //BGM13_game
	SOUND_LABEL_BGM014,                         //BGM14_result
	SOUND_LABEL_BGM015,                         //BGM15_BossBgm0
	SOUND_LABEL_SE_TIBIROBOWALK,                //SE00_���у��{������
	SOUND_LABEL_SE_SWORDSWING_000,              //SE01_���у��{������U��
	SOUND_LABEL_SE_SWORDSLASH_000,              //SE02_���у��{�����Ŏa��
	SOUND_LABEL_SE_SPEAKERWAVE_000,             //SE03_���у��{���X�s�[�J�[�U��������
	SOUND_LABEL_SE_BOOMERANGTHROW_000,          //SE04_���у��{���u�[�������𓊂���
	SOUND_LABEL_SE_HITBOOMERANG_000  ,          //SE05_�u�[���������q�b�g����
	SOUND_LABEL_SE_SPEAKERWAVEHIT_000,          //SE06_�X�s�[�J�[�U�����q�b�g����
	SOUND_LABEL_SE_SELECT_000,                  //SE07_�I��
	SOUND_LABEL_SE_BATTERYCAUTION_000,          //SE08_�o�b�e���[�c�ʂ���Ȃ�
	SOUND_LABEL_SE_STATUSBUFF_000,              //SE09_�X�e�[�^�X�A�b�v
	SOUND_LABEL_SE_CLAWSLASH_000,               //SE10_�܂Ŏa��
	SOUND_LABEL_SE_BOSSAPPEAR_000,              //SE11_�{�X�o��
	SOUND_LABEL_SE_DEFEATENEMY_000,             //SE12_�G��|����
	SOUND_LABEL_SE_DECIDE_000,                  //SE13_���肵��
	SOUND_LABEL_SE_BATTERYCHARGE_000,           //SE14_�o�b�e���[���`���[�W����
	SOUND_LABEL_SE_SCRAPCOLLECT_000,            //SE15_�X�N���b�v���l������
	SOUND_LABEL_SE_DODGE_000,                   //SE16_���
	SOUND_LABEL_SE_CRITICAL_000,                //SE17_�N���e�B�J��
	SOUND_LABEL_SE_CHARGINGCOMPLETION_000,      //SE18_�[�d����
	SOUND_LABEL_SE_GETCANDY_000,                //SE19_���l��
	SOUND_LABEL_SE_RASER_000,                   //SE20_���[�U�[
	SOUND_LABEL_SE_POWERCHARGE_000,             //SE21_�͂����߂�
	SOUND_LABEL_SE_FASTMOVE_000,                //SE22_�����ړ�
	SOUND_LABEL_SE_FIRE_000,                    //SE23_��
	SOUND_LABEL_SE_WIND_000,                    //SE24_��
	SOUND_LABEL_SE_DAMAGE_000,					//SE25_��e
	SOUND_LABEL_SE_FLUSHPUNCH_000,              //SE26_�t���b�V���p���`
	SOUND_LABEL_SE_BIRDSTRIKE_000,				//SE27_�o�[�h�X�g���C�N
	SOUND_LABEL_SE_THUNDER_000,                 //SE28_�T���_�[
	SOUND_LABEL_SE_FALLINGMETEO_000,            //SE29_���e�I���~��
	SOUND_LABEL_SE_BURSTMETEO_000,              //SE30_���e�I���o�[�X�g����
	SOUND_LABEL_SE_SHOCKWAVE_000,               //SE31_�Ռ��g





	SOUND_LABEL_MAX
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
