//========================================================
//
//�P�P���P�R���F�R�c��ԂōU���r���{�[�h�𔭎˂���[Attackbillboard.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _Attackbillboard_H_
#define _Attackbillboard_H_

#include "main.h"

#define MAX_ATTACKBILLBOARD (5000)//�U���r���{�[�h�̍ő吔
#define NUM_ATTACKBILLBOARD (3)//�U���r���{�[�h�̎�ސ�

typedef struct
{
	D3DXVECTOR3 pos;//�ʒu
	D3DXVECTOR3 move;//�ړ���
	D3DXVECTOR3 rot;//����
	D3DXVECTOR3 Size;//�傫��
	int nType;//���
	int nLife;//�̗�
	int nIdxShadow;//�e�̃C���f�b�N�X
	int nNumEnemy;//���Ԃ̓G�������Ă��Ă��邩
	float fRWidth;//���i���a�j
	float fRHeight;//�����i���a)
	float fLength;//����
	float fRotY;//Y��]
	bool bUse;//�g�p���Ă��邩�ǂ���
	bool bOneFlag;//��񂾂����������鏈���悤�t���O
	bool bChangeFlag;//�l���㉺�����鎞�Ɏg���t���O
	int nMaxLife;//�ő�̗�

	float fVXaim;//X�����x�N�g��
	float fVYaim;//Y�����x�N�g��
	float fVZaim;//Z�����x�N�g��
	float fVaim;//���������x�N�g��

	int nAnimationCnt;//�A�j���[�V�����J�E���g
	int nAnimationPattern;//�A�j���[�V�����p�^�[��
	int nAnimationPatternH;//�c�̃A�j���[�V�����p�^�[��
}Attackbillboard;

typedef enum
{
	ATTACKBILLBOARD00_STUB = 0,//���ʂ̃o���b�g
	ATTACKBILLBOARD01_THUNDER, //��
	ATTACKBILLBORAD02_FIRE,    //�t�@�C�A
	ATTACKBILLBOARDTYPE_MAX
}AttackbillboardTYPE;

//�v���g�^�C�v�錾
void InitAttackbillboard(void);
void UninitAttackbillboard(void);
void UpdateAttackbillboard(void);
void DrawAttackbillboard(void);
void SetAttackbillboard(int nType, int nLife, float fRWidth, float fRHeight, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, int nNumEnemy, float fLength);

Attackbillboard* GetAttackbillboard(void);//�U���r���{�[�h�\���̂̏����Q�b�g����

#endif
