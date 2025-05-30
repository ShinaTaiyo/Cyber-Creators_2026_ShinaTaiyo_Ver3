//========================================================
//
//�P�Q���Q�V���F�o�b�e���[����������s���B[Battery.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _Battery_H_
#define _Battery_H_

#include "main.h"

#define MAX_BATTERY (5000)//�o�b�e���[�̍ő吔
#define BATTERYDIGIT (4)//�o�b�e���[���l�̌���
#define BATTERYRADIUSSIZE (20.0f)//�o�b�e���[�̔��a�Q�Ƃ̑傫��
#define MAX_OUTLETDISP (30)//�[�d��ʒu�\���̍ő吔
#define OUTLETDISP_WIDTH (30.0f)
#define OUTLETDISP_HEIGHT (30.0f)
#define NUM_BATTERY (2)//�o�b�e���[�̎�ސ�

typedef enum
{
	BATTERYTYPE00_NOWBATTERY = 0,//���݂̃o�b�e���[
	BATTERYTYPE01_MAXBATTERY,    //�o�b�e���[�̍ő�l
	BATTERYTYPE_MAX
}BATTERYTYPE;

typedef struct
{
	D3DXVECTOR3 pos;//�ʒu
	float fPosTexU;//�����̕\���p
}BatteryNum;

typedef struct
{
	D3DXVECTOR3 pos;//�ʒu
	int nType;//���
	int nBattery;//���݂̃o�b�e���[
	bool bCautionBattery;//�o�b�e���[�����Ȃ����Ɍx�����o���ϐ�
	BatteryNum aBatteryNum[BATTERYDIGIT];//�o�b�e���[���l�̍\����
    int nAnimationPattern;//�A�j���[�V�����p�^�[��
	int nAnimationCnt;      //�A�j���[�V�����J�E���g
	bool bAnimationTurn;    //�A�j���[�V�������t�ɂ���
}Battery;

typedef struct
{
	D3DXVECTOR3 pos;//�ʒu
	bool bUse;//�g�p���
	D3DXVECTOR3 rot;//����
	D3DXMATRIX mtxWorld;//�}�g���b�N�X���[���h
	int nNumModel;//���Ԗڂ̃��f�����g�p���Ă��邩
}OutletDisp;


//�v���g�^�C�v�錾

//============================
//�o�b�e���[�\��
//============================
void InitBattery(void);
void UninitBattery(void);
void UpdateBattery(void);
void DrawBattery(void);
Battery* GetBattery(void);//�e�\���̂̏����Q�b�g����
void SubBattery(int nSubBattery);//�o�b�e���[���グ��B
//================================================================

//=============================
//�[�d��̈ʒu�̕\��
//=============================
void InitOutletDisp(void);
void UninitOutletDisp(void);
void UpdateOutletDisp(void);
void DrawOutletDisp(void);
void SetOutletDisp(D3DXVECTOR3 pos,int nNumModel);

//=============================
//�ő�o�b�e���[�\��
//=============================
void InitMaxBattery(void);
void UninitMaxBattery(void);
void UpdateMaxBattery(void);
void DrawMaxBattery(void);

#endif





