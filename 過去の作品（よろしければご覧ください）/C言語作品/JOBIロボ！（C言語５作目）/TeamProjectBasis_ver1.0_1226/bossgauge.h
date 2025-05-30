//=========================================================
//
// �{�X�̗̑̓Q�[�W�\�L����
// Author : Shina Taiyo
//
//=========================================================
#ifndef _BOSSGAUGE_H_
#define _BOSSGAUGE_H_

#include "main.h"

//=========================================================
// �}�N����`
//=========================================================
#define NUM_BOSSGAUGEFRAME	(2)					//�Q�[�W�̃t���[���̎��
#define MAX_BOSSGAUGEFRAME	(NUM_BOSSGAUGEFRAME)//�Q�[�W�̃t���[���̍ő吔
#define BOSSGAUGE_POS_X		(760.0f)			//X���W
#define BOSSGAUGE_POS_Y		(25.0f)				//Y���W

//=========================================================
// ���
//=========================================================
typedef enum
{
	BOSSGAUGEFRAME00_HIDE = 0,
	BOSSGAUGEFRAME01_APPER,
	BOSSGAUGEFRAME_MAX
}BOSSGAUGEFRAME;

//=========================================================
// �\����
//=========================================================

//�Q�[�W�̃t���[��
typedef struct
{
	bool bUse;				//�g�p���Ă��邩�ǂ���
	int nType;				//�Q�[�W�t���[���̕��ނ𑝂₷�P�[�X�ɔ����Aint�ɂ��Ƃ�
	D3DXVECTOR3 pos;		//�ʒu
	float fBossGaugeLength;	//�Q�[�W�̒���
}BossGaugeFrame;

//�̗̓Q�[�W
typedef struct
{
	bool bUse;                  //�g�p���Ă��邩�ǂ���
	D3DXVECTOR3 pos;            //�ʒu
	float fBossGauge;           //�Q�[�W�\�L
	float fBossGaugeLength;     //�Q�[�W�̒���
	float fNextLevelBossGauge;	//���̃��x���܂ł̃Q�[�W�̒l
	int nBossGaugeNum;          //�Q�[�W�̔ԍ�
	int nEnemyNum;              //���������G�̔ԍ�
}BossGauge;

//=========================================================
// �v���g�^�C�v�錾
//=========================================================
void InitBossGauge(void);			//����������
void UninitBossGauge(void);			//�I������
void UpdateBossGauge(void);			//�X�V����
void DrawBossGauge(void);			//�`�揈��
void SetBossGauge(int nEnemyNum);	//�{�X�̃Q�[�W���o�������鏈��

#endif
