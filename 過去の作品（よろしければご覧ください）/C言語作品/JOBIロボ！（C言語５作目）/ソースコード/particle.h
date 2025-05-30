//========================================================
//
//�P�P���P�R���F�R�c��ԂŃG�t�F�N�g�𔭎˂���[Particle.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _Particle_H_
#define _Particle_H_

#include "main.h"

#define MAX_PARTICLE (50000)//�p�[�e�B�N���̍ő吔
#define NUM_PARTICLE (2)//�p�[�e�B�N���̎�ސ�
#define Particle_SCALE (10.0f)

typedef struct
{
	D3DXVECTOR3 pos;//�ʒu
	D3DXVECTOR3 move;//�ړ���
	D3DXVECTOR3 rot;//����
	D3DXCOLOR col;//�F
	int nType;//���
	int nTexType;//�p�[�e�B�N���̃e�N�X�`���̎��
	int nLife;//�̗�
	int nMaxLife;//�ő�̗�
	int nIdxShadow;//�e�̃C���f�b�N�X
	float fRWidth;//���i���a�j
	float fRHeight;//�����i���a�j
	float fScale;//�G�t�F�N�g�̑傫��
	float fRedution;//�G�t�F�N�g�̑傫��������������X�s�[�h
	float fRotMove;//���������̐��l�����߂�ϐ�
	bool bUse;//�g�p���Ă��邩�ǂ���
	float fSpeed;//����
	int nAnimationPattern;
	int nAnimaitonCount;
	int nNumModel;
	//======================================
    //�R�������@�_���e�̎����Ɏg���ϐ�
    //======================================
	float fVXaim3D;
	float fVYaim3D;
	float fVZaim3D;
	float fVLaim3D;

	D3DXVECTOR3 SaveMove3D;//�ړ��ʂ̕ۑ�������@
	//==============================================================================================
}Particle;

typedef enum
{
	PARTICLETYPE_NORMAL = 0,//���ʂɔ��
	PARTICLETYPE_GRAVITY,//�d�͂������Ĕ��
	PARTICLETYPE_FIRE,   //�΂̕��̂悤�ɁA�������㏸�ʂ�������
	PARTICLETYPE_METEO,  //���e�I�����鎞�ɁA�ʒu��\������p�[�e�B�N��
	PARTICLETYPE_CHARGE, //�p���[���`���[�W���Ă��镗
	PARTICLETYPE_MAX
}PARTICLETYPE;

typedef enum
{
 PARTICLETEX_NORMAL,//���ʂ̃p�[�e�B�N��
 PARTICLETEX_CHAINFIRE,//��l���̋Z�F�`�F�C���t�@�C�A
 PARTICLETEX_MAX
}PARTICLETEX;

//�v���g�^�C�v�錾
void InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);
void SetParticle(int nType, int nLife, float fScale, float fRedution, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRotMove,int nTexType,int nNumModel);

#endif



