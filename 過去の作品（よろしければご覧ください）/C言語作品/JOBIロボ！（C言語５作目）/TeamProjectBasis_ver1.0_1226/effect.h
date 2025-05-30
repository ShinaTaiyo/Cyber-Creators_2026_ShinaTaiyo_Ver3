//========================================================
//
// �G�t�F�N�g�̏����Ɋւ���w�b�_�[�t�@�C��
// Author:ShinaTaiyo
//
//========================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "main.h"

//========================================================
// �}�N����`
//========================================================
#define MAX_EFFECT (50000)//�G�t�F�N�g�̍ő吔
#define NUM_EFFECT (1)//�G�t�F�N�g�̎�ސ�
#define EFFECT_SCALE (10.0f)//�G�t�F�N�g�̃X�P�[��

//========================================================
// �\����
//========================================================
typedef struct
{
	D3DXVECTOR3 pos;//�ʒu
	D3DXVECTOR3 move;//�ړ���
	D3DXVECTOR3 rot;//����
	D3DXCOLOR col;//�F
	D3DXMATRIX mtxWorldEffect;//���[���h�}�g���b�N�X(float4*4)
	int nType;//���
	int nLife;//�̗�
	int nMaxLife;//�̗͍ő�l
	int nNumAttackModel;//�G�t�F�N�g���Ă񂾍U�����f���̔ԍ�
	int nIdxShadow;//�e�̃C���f�b�N�X
	float fRWidth;//���i���a�j
	float fRHeight;//�����i���a�j
	float fWidth;//�G�t�F�N�g�̉���
	float fHeight;//�G�t�F�N�g�̍���
	float fRedution;//�G�t�F�N�g�̑傫��������������X�s�[�h
	float fRotMove;//���������̐��l�����߂�ϐ�
	bool bUse;//�g�p���Ă��邩�ǂ���
}Effect;

//========================================================
// ���
//========================================================
typedef enum
{
	EFFECTTYPE_NORMAL = 0,//���ʂ̃G�t�F�N�g
	EFFECTTYPE_ROLING,//���邮��G�t�F�N�g
	EFFECTTYPE_MAX,
}EFFECTTYPE;

//========================================================
// �v���g�^�C�v�錾
//========================================================
void InitEffect(void);//����������
void UninitEffect(void);//�I������
void UpdateEffect(void);//�X�V����
void DrawEffect(void);//�`�揈��
void SetEffect(int nType, int nLife, float fWidth, float fHeight, float fRedution, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col,float fRotMove,int nNumModel);//�ݒ菈��

#endif
