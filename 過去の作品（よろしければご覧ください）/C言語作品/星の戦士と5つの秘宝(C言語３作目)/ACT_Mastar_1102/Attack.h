//========================================================
//
//�P�O���P�Q���F�v���C���[�ɃR�}���h�Z��������[Attack.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _ATTACK_H_
#define _ATTACK_H_

#include "main.h"


#define MAX_ATTACK (4096)//�U���̐��̍ő吔
#define GENERALLY_ATTACK_LIFE (1500)//���ɕύX�������Ȃ��Ƃ��́A�W���̍U���\������
//�e�̎��
typedef enum
{
	ATTACKTYPE_PUNCH=0,//�A�r���e�B�u�m�[�}���v�̎��̃v���C���[�̍U���Z���̂P
	ATTACKTYPE_SLASH_E,//�a���U��(�G�j
	ATTACKTYPE_ICE_EXPLOSION,//�X�����U���i�G�j
	ATTACKTYPE_THUNDER_E4,//���U���i�G�S�j
	ATTACKTYPE_BOMU,//���e�U���i�{���X���C���j
	ATTACKTYPE_NEEDLE,//�Ƃ��U���i�ԃX���C���j
	ATTACKTYPE_CLAW,//�܍U���i�L�X���C���j
	ATTACKTYPE_VACUUMBLADE,//�^��n�i�L�X���C���j
	ATTACKTYPE_FALLINGBLOCK,//�u���b�N���Ƃ��i�΃X���C���j
	ATTACKTYPE_BEAM,//�r�[���U���i���@�g���X���C���j
	ATTACKTYPE_ICICLECLAW,//�A�C�V�N���N���[�i�{�X�Q�j
	ATTACKTYPE_ABSOLUTE_BLIZZARD,//�A�u�\�����[�g�u���U�[�h�i�{�X�Q�j
	ATTACKTYPE_FIRE,//���U���i�{�X�R�j
	ATTACKTYPE_SHAKINGSWORD,//���U���i�{�X�R�j
	ATTACKTYPE_FLASHBLADE,//�u�n�M �i�{�X�R�j
	ATTACKTYPE_THUNDER_2,//���U���Q�i�{�X�S�j
	ATTACKTYPE_FINALBEAM,//�r�[���U���i���X�{�X�j
	ATTACKTYPE_MAX
}ATTACKTYPE;

//�U���̈ړ��^�C�v
typedef enum
{
	MOVETYPE_NORMAL = 0,//���ʂɓ���
	MOVETYPE_GRAVITY,//�U���ɏd�͂���
	MOVETYPE_MAX
}MOVETYPE;


//�e�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;//�ʒu
	D3DXVECTOR3 posOld;//�O�̈ʒu
	D3DXVECTOR3 move;//�ړ���
	D3DXVECTOR3 rot;//����
	ATTACKTYPE type;//�U���Z�̎��
	MOVETYPE moveType;//�U���̓����̎��
	float fLength;//�Ίp���̒���
	float fAngle;//�Ίp���̊p�x
	float fWidth;//��
	float fHeight;//����
	float fGravity;//�d�͕␳�l
	float fXVaim;//���@�_��������Ƃ��̏���
	float fYVaim;//���@�_��������Ƃ��̏���
	float fVaim;//���@�_��������Ƃ��̏���
	float fEffectiveScale;//�U���̌��ʔ͈͂����߂�i�K�v�ɉ����Ďg�p�j
	float fRotSpeed;//��]������U���̉�]���x�����߂�
	int nLife;//�����i�ǂ̂��炢�e�����ł�����
	int nCntUseTime;//�U�����g���Ă��鎞�Ԃ��J�E���g����
	int nColorChange;//�F��ς���ϐ�
	int nTime;//�U�����n�܂��Ă���̎��Ԃ��v������
	int nCntAnim;//�A�j���[�V�����J�E���^�[
	int nPatternAnim;//�A�j���[�V�����p�^�[��
	int nPatternAnimY;//�A�j���[�V��������i�ȏ�̎��ɁA�e���ڂ�����
	int nHitCount;//���̍U���ōU�����q�b�g����񐔂����߂�
	int nNumEnemy;//�ǂ̓G���U�������Ă��邩�𔻒肷��
	bool bUse;//�U�������Ă��邩�ǂ���
	bool bIsLanding;//�n�ʂɂ��Ă���t���O
	bool bIsWalling;//�ǂɓ������Ă��邩�ǂ����̃t���O
}Attack;

//�v���g�^�C�v�錾
void InitAttack(void);
void UninitAttack(void);
void UpdateAttack(void);
void DrawAttack(void);
void SetAttack(D3DXVECTOR3 pos, D3DXVECTOR3 move, ATTACKTYPE type, int nNum, int nNumEnemy, int nAimingEnemy,MOVETYPE moveType,
	float Width,float Height,int nLife,float rot,float fRotSpeed);//pos�́u���ˈʒu�v�Amove�́u�ړ��ʁv,damage�́u�e�̍U���́vnNumEney�́A���Ԗڂ̓G���U�����Ă��邩?

Attack* GetAttack(void);//�e�\���̂̏����擾����B

#endif

