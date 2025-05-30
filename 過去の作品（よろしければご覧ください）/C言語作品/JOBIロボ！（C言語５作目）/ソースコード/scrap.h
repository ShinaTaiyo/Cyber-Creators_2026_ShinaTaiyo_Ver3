//========================================================
//
//�P�Q���Q�V���F�X�N���b�v����������s���B[scrap.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _SCRAP_H_
#define _SCRAP_H_

#include "main.h"

#define MAX_SCRAP (5000)//�X�N���b�v�̍ő吔
#define NUM_SCRAP (5)//�U���r���{�[�h�̎�ސ��i�ꉞ��ތォ�瑝�������p�ɁA���̃}�N���͎g���Ă����B�j
#define GENERALLYSCRAPLIFE (600)//�S�ẴX�N���b�v�̗̑�
#define SCRAPRADIUSSIZE (20.0f)//�X�N���b�v�̔��a�Q�Ƃ̑傫��
typedef enum
{
	SCRAPTYPE00_TIBIROBO = 0,//���у��{�����X�N���b�v
	SCRAPTYPE01_SWORD,       //���U�������X�N���b�v
	SCRAPTYPE02_BOOMERANG,   //�u�[�����������X�N���b�v
	SCRAPTYPE03_SPEAKER,     //�X�s�[�J�[�����X�N���b�v
	SCRAPTYPE04_ALL,          //�S�X�e�[�^�X�����X�N���b�v
	SCRAPTYPE_MAX
}SCRAPTYPE;

typedef enum
{
	SCRAPMOVETYPE_DIFFUSION = 0,//�g�U������Ƃ��̈ړ��̂�����
	SCRAPMOVETYPE_NORMAL,        //���ʂ̈ړ��̎d��
	SCRAPMOVETYPE_MAX
}SCRAPMOVETYPE;

typedef struct
{
	D3DXVECTOR3 pos;//�ʒu
	D3DXVECTOR3 move;//�ړ���
	D3DXVECTOR3 rot;//����

	D3DXVECTOR3 DecayMove;//����������ړ���
	int nType;//���
	int nLife;//�̗�
	int nIdxShadow;//�e�̃C���f�b�N�X
	float fRWidth;//���i���a�j
	float fRHeight;//�����i���a�j
	float fLength;//����
	bool bUse;//�g�p���Ă��邩�ǂ���
	int nAnimationPattern;
	int nAnimationCnt;

	float fXaim;//X�����̃x�N�g��
	float fYaim;//Y�����̃x�N�g��
	float fZaim;//Z�����̃x�N�g��

	float fVXZaim;//X��Z�̃x�N�g��
	float fVYZaim;//Y��Z�̃x�N�g��

	//==============================
	//�O�������@�_���ϐ�
	//==============================
	float fVXaim3D;
	float fVYaim3D;
	float fVZaim3D;
	float fVLaim3D;
	//==============================================================

	SCRAPMOVETYPE ScrapMoveType;//�X�N���b�v�̈ړ��̎d��
}Scrap;

typedef struct
{
	D3DXVECTOR3 pos;//�ʒu
	float fPosTexU;//�����̕\���p
}ScrapUI;



//�v���g�^�C�v�錾
void InitScrap(void);
void UninitScrap(void);
void UpdateScrap(void);
void DrawScrap(void);
void SetScrap(int nType, int nLife,float fRWidth, float fRHeight, D3DXVECTOR3 pos, D3DXVECTOR3 rot,D3DXVECTOR3 DiffusionMove);
Scrap* GetScrap(void);//�e�\���̂̏����Q�b�g����

#endif