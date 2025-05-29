//========================================================
//
//�W���R�O���F�G�t�F�N�g������񌩒���[player.cpp]
//Author:ShinaTaiyo
//
//=========================================================

#ifndef _PLAYER_H_//���̃}�N����`������Ă��Ȃ�������
#define _PLAYER_H_//�Q�d�C���N���[�h�h�~�̃}�N����`

#define PLAYER_WIDTH (18.0f)//�e�N�X�`���̉���
#define PLAYER_HEIGHT (40.0f)//�e�N�X�`���̏c��


//�v���C���[�̏��
typedef enum
{
	PLAYERSTATE_APPER = 0,//�o����ԁi�_�Łj
    PLAYERSTATE_NORMAL,//�ʏ���
	PLAYERSTATE_DAMAGE,//�_���[�W���
	PLAYERSTATE_DEATH,//���S���
	PLAYERSTATE_MAX
}PLAYERSTATE;

//�v���C���[�̃��[�V����
typedef enum
{
	PLAYERMOTION_STAND = 0,//�ҋ@���[�V����
	PLAYERMOTION_MOVE,//�ړ����[�V����
	PLAYERMOTION_PUNCH,//�p���`���[�V����
	PLAYERMOTION_MAX
}PLAYERMOTION;

typedef enum
{
	INERTIA_NORMAL = 0,//���ʂ̊���
	INERTIA_ICE,//�X�u���b�N�̏�ɂ���Ƃ��̊����̋���
	INERTIA_MAX
}INERTIA;


//�v���C���[�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;//�ʒu
	D3DXVECTOR3 PosBeside;//���̒��S�_�i�����蔻��j
	D3DXVECTOR3 posOld;//�O��̈ʒu
	D3DXVECTOR3 rot;//����
	PLAYERSTATE state;//�v���C���[�̏��
	D3DXVECTOR3 move;//�ړ���
	int nCounterAnim;//�J�E���^�[
	int nPatternAnim;//�p�^�[���ԍ�
	int nDirectionMove;//����
	int nCounterState;//��ԑJ�ڃJ�E���^�[
	int nMotionType;//�g�p���Ă��郂�[�V�����̎��
	int nDashCount;//�_�b�V����Ԃɂ��邽�߂̃R�}���h�����܂ł̎��Ԃ��J�E���g����
	int nMotionTypeOld;//�O�Ɏg�p���Ă��郂�[�V������ۑ�����
	int nMotionTime;//���[�V�������g�p���Ă��鎞�ԁB���[�V���������I�����邩�̔���Ŏg��
	int nCntDashButtonL;//�������Ƀ_�b�V�����邽�߂̃{�^�����������񐔂��J�E���g
	int nCntDashButtonR;//�E�����Ƀ_�b�V�����邽�߂̃{�^�����������񐔂��J�E���g
	int nInertia;//���������߂�
	bool bIsJumping;//�W�����v�����ǂ���
	bool bDisp;//�v���C���[�̕\����ON�AOFF������B
	bool bInvincibility;//�v���C���[�𖳓G��Ԃɂ��邩�ǂ���
	bool bUse;//�g�p���Ă��邩�ǂ���
	bool bIsLanding;//�n�ʂ̏�ɂ��邩�ǂ���
	int nHitCount;//�v���C���[����e�����񐔂��J�E���g����ϐ�
	int nSkillUseCount;//�v���C���[���X�L�����g�p�����񐔂��J�E���g����ϐ�
	bool OperationFlag;//������\�ɂ���t���O
	float fGravity;//�d�͂��W�����v���Ԃ������قǑ��₷�B
	float fWidth;//��
	float fHeight;//����
	bool bIsWalling;//�ǂɓ������Ă���Ƃ����t���O
	bool g_bTouchingBlock;//�u���b�N�ɐG��Ă���Ɣ�������t���O
	bool bDirection;//���E�ǂ���������Ă��邩���A�t���O�Ŕ��肷��
	bool bUseMotion;//���[�V�������g�p���Ă��邩�ǂ����̃t���O
	bool bDashL;//�������ւ̃_�b�V����Ԃ��ǂ����̃t���O
	bool bDashR;//�E�����ւ̃_�b�V����Ԃ��ǂ����̃t���O
	bool bAutoMoveR;//�E�����֎����I�Ɉړ����邩�ǂ����̃t���O
	bool bAutoMoveL;//�������֎����I�Ɉړ����邩�ǂ����̃t���O
}Player;

//�v���g�^�C�v�錾
void InitPlayer(void);
void UninitPlayer(void);
void UpDatePlayer(void);
void DrawPlayer(void);
Player* GetPlayer(void);
void HitPlayer(int nDamage);
void SetPlayer(D3DXVECTOR3 pos);
#endif
