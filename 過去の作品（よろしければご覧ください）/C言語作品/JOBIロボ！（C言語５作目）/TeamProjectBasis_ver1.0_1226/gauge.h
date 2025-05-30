//=========================================================
//
// UI�p�̃Q�[�W�\�L����
// Author : Shina Taiyo
//
//=========================================================
#ifndef _GAUGE_H_
#define _GAUGE_H_

#include "main.h"

#define NUM_GAUGEFRAME (4)             //�Q�[�W�̃t���[���̎��
#define MAX_GAUGEFRAME (4)             //�Q�[�W�t���[���̍ő吔
#define MAX_GAUGE (NUM_GAUGEFRAME)     //�Q�[�W�̍ő吔�i�Q�[�W�t���[���Ɠ������j

typedef enum
{
	SCRAPGAUGEFRAMETYPE00_TIBOROBO = 0,//���у��{�X�N���b�v�Q�[�W�t���[��
	SCRAPGAUGEFRAMETYPE01_SWORD,       //���X�N���b�v�Q�[�W�t���[��
	SCRAPGAUGEFRAMETYPE02_BOOMERANG,   //�u�[�������X�N���b�v�Q�[�W�t���[��
	SCRAPGAUGEFRAMETYPE03_SPEAKER,     //�X�s�[�J�[�X�N���b�v�Q�[�W�t���[��
	SCRAPGAUGEFRAMETYPE_MAX
}SCRAPGAUGEFRAMETYPE;                  //�X�N���b�v�Q�[�W�̎��

typedef struct
{
	bool bUse;                    //�g�p���Ă��邩�ǂ���
	int nType;                    //�Q�[�W�t���[���̕��ނ𑝂₷�P�[�X�ɔ����Aint�ɂ��Ƃ�
	D3DXVECTOR3 pos;              //�ʒu
	float fGaugeLength;           //�Q�[�W�̒���
}GaugeFrame;

typedef struct
{
	D3DXVECTOR3 pos;              //�ʒu
	float fGauge;                 //�Q�[�W�\�L
	float fGaugeLength;           //�Q�[�W�̒���
	float fNextLevelGauge;        //���̃��x���܂ł̃Q�[�W�̒l
	int nGaugeNum;              //�Q�[�W�̔ԍ�
}Gauge;


//=========================================================
// �v���g�^�C�v�錾
//=========================================================
void InitGauge(void);	                             //����������
void UninitGauge(void);	                             //�I������
void UpdateGauge(void);	                             //�X�V����
void DrawGauge(void);	                             //�`�揈��
void AddGauge(int nType,int nScrap,int nNextLvScrap);//�Q�[�W�𑝂₷����
#endif
