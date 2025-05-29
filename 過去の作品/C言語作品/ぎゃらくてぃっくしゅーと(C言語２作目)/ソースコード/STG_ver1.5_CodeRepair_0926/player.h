//========================================================
//
//�W���R�O���F�G�t�F�N�g������񌩒���[player.cpp]
//Author:ShinaTaiyo
//
//=========================================================

#ifndef _PLAYER_H_//���̃}�N����`������Ă��Ȃ�������
#define _PLAYER_H_//�Q�d�C���N���[�h�h�~�̃}�N����`

//�v���C���[�̏��
typedef enum
{
	PLAYERSTATE_APPER = 0,//�o����ԁi�_�Łj
    PLAYERSTATE_NORMAL,//�ʏ���
	PLAYERSTATE_DAMAGE,//�_���[�W���
	PLAYERSTATE_DEATH,//���S���
	PLAYERSTATE_MAX
}PLAYERSTATE;

//�v���C���[�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;//�ʒu
	D3DXVECTOR3 rot;//����
	D3DXVECTOR3 move;//�ړ���
	PLAYERSTATE state;//�v���C���[�̏��
	int nCounterState;//��ԑJ�ڃJ�E���^�[
	bool bDisp;//�v���C���[�̕\����ON�AOFF������B
	bool bInvincibility;//�v���C���[�𖳓G��Ԃɂ��邩�ǂ���
	int nHitCount;//�v���C���[����e�����񐔂��J�E���g����ϐ�
	int nSkillUseCount;//�v���C���[���X�L�����g�p�����񐔂��J�E���g����ϐ�
	bool OperationFlag;//������\�ɂ���t���O
}Player;

//�v���g�^�C�v�錾
void InitPlayer(void);
void UninitPlayer(void);
void UpDatePlayer(void);
void DrawPlayer(void);
Player* GetPlayer(void);
void HitPlayer(int nDamage);
#endif
