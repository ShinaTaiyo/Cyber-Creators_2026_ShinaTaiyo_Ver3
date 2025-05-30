//========================================================
//
//�~�b�V�����̏����Ɋւ���w�b�_�[�t�@�C��
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _MISSION_H_
#define _MISSION_H_

#include "main.h"

//=========================================================
//�}�N����`
//=========================================================
#define MAX_MISSION				(5)		//�~�b�V�����\���̍ő吔
#define MISSION_POS_Y			(180.0f)//�~�b�V�����\����Y���W
#define MISSION_DISPLAY_TIME	(5)		//�~�b�V�����\������(�b)
#define MISSION_MOVE			(15.0f)	//�\�����ړ���
#define NUM_CANDY				(5)		//���̐�
#define NEED_KILLENEMY			(150)	//�K�v�ȓG���j��	

//=========================================================
//�e�~�b�V����
//=========================================================
typedef enum
{
	MISSION00_COLLECTCANDY = 0,//�����W�߂�
	MISSION01_DEFEATENEMY30,   //�G���R�O�̓|��
	MISSION02_DONTFALLBELOW70, //�̗͂T�O����������
	MISSION03_BOSSDEFEAT70SEC, //�{�X���P�O�O�b�ȓ��ɓ|��
	MISSION04_HEALBIND_3,      //�񕜂R��ȓ�����
	MISSION_MENU_MAX
}MISSION_MENU;

//=========================================================
//�~�b�V����0�\����
//=========================================================
typedef struct
{
	bool bMissionClear;	//�~�b�V�������N���A�������ǂ���
	bool bMissionUISet;	//�~�b�V����UI���\���ς݂��ǂ���
	bool bMissionReward;//�~�b�V�����̕�V���󂯎�������ǂ���
	int nGetCandyCount;	//�L�����f�B�l����
}Mission0;

//=========================================================
//�~�b�V����1�\����
//=========================================================
typedef struct
{
	bool bMissionClear;		//�~�b�V�������N���A�������ǂ���
	bool bMissionUISet;	//�~�b�V����UI���\���ς݂��ǂ���
	bool bMissionReward;	//�~�b�V�����̕�V���󂯎�������ǂ���
	int nDefeatEnemyCount;	//�G��|������
}Mission1;

//=========================================================
//�~�b�V����2�\����
//=========================================================
typedef struct
{
	bool bMissionClear;	//�~�b�V�������N���A�������ǂ���
	bool bMissionUISet;	//�~�b�V����UI���\���ς݂��ǂ���
	int nKeepHp;		//�v���C���[�̌��݂̗̑͂�ۑ�����
	bool bTimeUpFlag;	//���ԂɂȂ�����A��񂾂��������s��
}Mission2;

//=========================================================
//�~�b�V����3�\����
//=========================================================
typedef struct
{
	bool bMissionClear;			//�~�b�V�������N���A�������ǂ���
	bool bMissionUISet;	//�~�b�V����UI���\���ς݂��ǂ���
	bool bMissionReward;		//�~�b�V�����̕�V���󂯎�������ǂ���
	int nAfterBossApperCntTime;	//�{�X���o�����Ă���̎���
}Mission3;

//=========================================================
//�~�b�V����4�\����
//=========================================================
typedef struct
{
	bool bMissionClear;	//�~�b�V�������N���A�������ǂ���
	bool bMissionUISet;	//�~�b�V����UI���\���ς݂��ǂ���
	int nHealCount;		//�񕜂����񐔂��J�E���g����
	int nHealTime;		//�񕜂����Ă��鎞�Ԃ��J�E���g����
}Mission4;

//=========================================================
//�~�b�V�����S�̂̍\����
//=========================================================
typedef struct
{
	Mission0 aMission0;//�����W�߂�
	Mission1 aMission1;//�G���R�O�̓|��
	Mission2 aMission2;//�̗͂T�O����������
	Mission3 aMission3;//�{�X���P�O�O�b�ȓ��ɓ|��
	Mission4 aMission4;//�񕜂R��ȓ�����
}Mission;

//=========================================================
//�~�b�V�����N���A�\���̍\����
//=========================================================
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	int nDisplayTime;	//�\������
	int nType;			//���
	float fWidth;		//����
	float fHeight;		//����
	bool bUse;			//�g�p���Ă��邩�ǂ���
}MissionUI;

//=========================================================
//�e��ނ̃e�N�X�`���t�@�C��
//=========================================================
static const char* MISSION_TEXTURE[MAX_MISSION] =
{
	"data\\TEXTURE\\Mission000.png",//�����W�߂�	
	"data\\TEXTURE\\Mission001.png",//�G���R�O�̓|��	
	"data\\TEXTURE\\Mission002.png",//�̗͂T�O����������	
	"data\\TEXTURE\\Mission003.png",//�{�X���P�O�O�b�ȓ��ɓ|��	
	"data\\TEXTURE\\Mission004.png",//�񕜂R��ȓ�����	
};

//=========================================================
//�v���g�^�C�v�錾
//=========================================================
void InitMission(void);		//����������
void UninitMission(void);	//�I������
void UpdateMission(void);	//�X�V����
void DrawMission(void);		//�`�揈��
Mission* GetMission(void);	//���\���̂̎擾
void SetMission(int nType, float fWidth, float fHeight);//�ݒ菈��
void JudgeClear(void);		//�~�b�V�����̃N���A���菈��

#endif
