//========================================================
//
//�P�P���P�R���F�R�c��ԂŃG�~�b�^�[�𔭎˂���[Emitter.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _EMITTER_H_
#define _EMITTER_H_

#include "main.h"

#define MAX_EMITTER (50000)//�G�~�b�^�[�̍ő吔
#define NUM_EMITTER (3)//�G�~�b�^�[�̎�ސ�

typedef enum
{
	EMITTERTYPE_EFFECT = 0,//�G�~�b�^�[�ŃG�t�F�N�g���Ăяo��
	EMITTERTYPE_PARTICLE,//�G�~�b�^�[�Ńp�[�e�B�N�����Ăяo��
	EMITTERTYPE_CURVE,//�J�[�u������G�~�b�^�[���Ăяo��
	EMITTERTYPE_TORNADO,//�g���l�[�h�G�~�b�^�[
	EMITTERTYPE_ROTATION,//���߂��ʒu�̎������葱����G�~�b�^�[
	EMITTERTYPE_GATHERING,//���߂��ʒu�ɏW�܂�G�~�b�^�[
	EMITTERTYPE_ACCELEHORMING,//�����^�̃z�[�~���O
	EMITTERTYPE_MAX
}EMITTERTYPE;

typedef enum
{
	EMITTERTARGET_ATTACKMODEL = 0,//�U�����f���𒆐S�ɓ�����
	EMITTERTARGET_ENEMY,          //�G�𒆐S�ɓ�����
	EMITTERTARGET_BOSS,           //�{�X�𒆐S�ɓ�����
	EMITTERTARGET_MAX
}EMITTERTARGET;

typedef struct
{
	D3DXVECTOR3 pos;//�ʒu
	D3DXVECTOR3 SupportPos;//���߂��ʒu�ŉ����������Ƃ��Ɏg��
	D3DXVECTOR3 move;//�ړ���
	D3DXVECTOR3 AddMove;//�ړ��ʂ�ǉ�����
	D3DXVECTOR3 rot;//����
	D3DXCOLOR col;//�F
	EMITTERTYPE EmitterType;//�G�~�b�^�[�ŌĂяo�����̎��
	int nType;//�ǂ݂����G�t�F�N�g��p�[�e�B�N���̎��
	int nTargetType;//�ǂ̃��f���𒆐S�ɃG�~�b�^�[���o����
	int nLife;//�̗�
	int nSummonLife;//�Ăяo�������̗̑͂�ݒ肷��
	int nNumAttack;//�G�~�b�^�[���Ăяo���Ă���U���̔ԍ����i�[����
	D3DXVECTOR3 SummonMove;//�Ăяo�������̈ړ��ʂ�ݒ肷��
	int nIdxShadow;//�e�̃C���f�b�N�X
	float fRWidth;//���i���a�j
	float fRHeight;//�����i���a�j
	float fScale;//�G�~�b�^�[�̑傫��
	float fRedution;//�G�~�b�^�[�̑傫��������������X�s�[�h
	float fRotMove;//�i�s���������߂�
	bool bUse;//�g�p���Ă��邩�ǂ���
	float fLength;//����
	int nCntTime;//�G�~�b�^�[���o�����Ă���̎��Ԃ��J�E���g����

	//======================================
	//�R�������@�_���e�̎����Ɏg���ϐ�
	//======================================
	float fVXaim3D;
	float fVYaim3D;
	float fVZaim3D;
	float fVLaim3D;
}Emitter;

//�v���g�^�C�v�錾
void InitEmitter(void);
void UninitEmitter(void);
void UpdateEmitter(void);
void DrawEmitter(void);
void SetEmitterEffect(int nType,int nTargetType, int nLife, int nSummonLife, float fScale, float fRedution, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRotMove,int nNumAttack,float fLength,D3DXVECTOR3 SupportPos);

#endif




