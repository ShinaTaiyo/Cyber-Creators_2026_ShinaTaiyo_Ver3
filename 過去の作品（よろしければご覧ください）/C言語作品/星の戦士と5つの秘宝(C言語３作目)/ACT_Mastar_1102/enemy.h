//========================================================
//
//�P�O���P�T���F�V�����G�̒ǉ�[enemy.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"

//�}�N����`
#define MAX_ENEMY (51)//�G�̍ő吔
#define MAX_CIRCLEBULLET (20)//�~�`�e�U�e�̒e��
#define MAX_CIRCLEBULLET_2 (10)//�~�`�e�U�e�̒e��


//�񋓌^��`
//=======================================
//�G�̏��
typedef enum
{
	ENEMYSTATE_NORMAL = 0,
	ENEMYSTATE_DAMAGE,
	ENEMYSTATE_MAX
}ENEMYSTATE;

//�G�̎��
typedef enum
{
	ENEMYTYPE0_MINIPUNI = 0,
	ENEMYTYPE1_HONOPUNI,
	ENEMYTYPE2_SODOPUNI,
	ENEMYTYPE3_ICESLIME,
	ENEMYTYPE4_BILIBILISLIME,
	ENEMYTYPE5_SNIPERSLIME,
	ENEMYTYPE6_BOMUSLIME,
	ENEMYTYPE7_FLOWERSLIME,
	ENEMYTYPE8_CATSLIME,
	ENEMYTYPE9_STONESLIME,
	ENEMYTYPE10_WITCHSLIME,
	ENEMYTYPE_BANDY,
	ENEMYTYPE_BOSS1_STAND,
	ENEMYTYPE_BOSS1_RUN,
	ENEMYTYPE_BOSS1_ATTACK,
	ENEMYTYPE_BOSS2_STAND,
	ENEMYTYPE_BOSS2_RUN,
	ENEMYTYPE_BOSS2_ATTACK,
	ENEMYTYPE_BOSS3_STAND,
	ENEMYTYPE_BOSS3_RUN,
	ENEMYTYPE_BOSS3_ATTACK,
	ENEMYTYPE_BOSS4_STAND,
	ENEMYTYPE_BOSS4_RUN,
	ENEMYTYPE_BOSS4_ATTACK,
	ENEMYTYPE_FINALBOSS_STAND,
	ENEMYTYPE_FINALBOSS_RUN,
	ENEMYTYPE_FINALBOSS_ATTACK,
	ENEMYTYPE_MAX
}ENEMYTYPE;


//�G�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;//�ʒu
	D3DXVECTOR3 posOld;;//1f�O�̈ʒu
	D3DXVECTOR3 move;//�ړ���
	D3DXVECTOR3 rot;//����
	ENEMYSTATE State;//���
	int nType;//���
	int nTypeOld;//1f�O�̃^�C�v
	int nCounterState;//��ԊǗ��J�E���^�[
	int nEnemyCount;//�G�̍ő吔���J�E���g����ϐ�
	int nCountMoveInterval;//�G�������܂ł̎��Ԃ��v��
	int nMoveTime;//�������Ԃ��v��
	int nAttarkIntervalCount;//�G���U������܂ł̎��Ԃ��v������B
	int nApperCount;//�G���Z�b�g����Ă���o������܂ł̎��Ԃ��v������
	int nAttackCount;//�s�����Ƃ̍U���񐔂��J�E���g����
	int nCntAnim;//�A�j���[�V�����J�E���^�[
	int nPatternAnim;//�A�j���[�V�����p�^�[��
	int nCntJumpTime;//�W�����v���鎞�Ԃ��J�E���g����
	float fLife;//�̗�
	float fMaxHp;//�ő�̗͂�ۑ�����
	float fBossMove;//�{�X����莞�Ԃ��Ƃɓ������t���O�B
	float fRotMove;//�G�������_���ɓ��������̕��������߂�ϐ�
	float fEnemy5BulletCurve;//�G�l�~�[�T�̒e�̃J�[�u�̒l
	float fWidth;//�G�̕�
	float fHeight;//�G�̍���
	float fGravity;//�d�͕␳�l
	float fXVaim;//���@�_��������Ƃ��̏���
	float fYVaim;//���@�_��������Ƃ��̏���
	float fVaim;//���@�_��������Ƃ��̏���
	bool bUse;//�g�p���Ă��邩�ǂ���
	bool bIsLanding;//�n�ʂɂ��Ă��邩�ǂ����̏���
	bool bIsWalling;//�ǂɓ������Ă��邩�ǂ����̃t���O
	bool bEnemyMoveX;//X�����̃X�N���[���Z�[�o�[
	bool bEnemyMoveY;//Y�����̃X�N���[���Z�[�o�[
	bool bSetEnemyUse;//�G���Z�b�g�����Ƃ���true�ɂȂ�t���O
	bool bDefeatFlag;//�G���Z�b�g������A�G��|������false�ɂȂ�t���O
	bool bBossSummonFlag;//�{�X�����t���O
	bool bBossMoveFlag;//�t���O��true�̎��Ƀ{�X�𓮂����t���O�B
	bool bAttackInterval;//�G���U������C���^�[�o��������t���O
	bool bEnemyCountminusFlag;//�G������Ă��瑍�������炷�J�E���g����񂾂�����������t���O
	bool bDirection;//���E�ǂ���������Ă���̂����肷��t���O
	bool bMoveWay;//�������������߂�t���O
	bool bMovePattern;//���̏u�Ԃɓ��������Q��ޗp�ӂ��Č��߂�t���O
	bool bSpecialAttack;//�K�E�Z���g���Ă��邩�ǂ��������߂�t���O
	bool bSpecialStaging;//�K�E�Z�̉��o���������ǂ����𔻒肷��t���O
	bool bGravity;//�d�͂������邩�ǂ����̃t���O
}Enemy;

typedef struct
{
	bool bResultFlag;
}Result;

typedef struct
{
	//bool bWaveFlag;//�E�F�[�u���̃J�E���g
	int DefeatEnemyCount;//�|�����G�l�~�[���̃J�E���g
}Defeat;

//�v���g�^�C�v�錾
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void SetEnemy(D3DXVECTOR3 pos, int ntype,float fLife,D3DXVECTOR3 move);//nType�͓G�̎��
void HitEnemy(int nCntEnemy, int nDamage);//�unCntEnemy�v�́A�ǂ̓G�ɁH�A�unDamage�v�́A�ǂ̂��炢�_���[�W��^����H
Enemy* GetEnemy(void);

Result* GetResult(void);

Defeat* GetDefeat(void);//�E�F�[�u�����Ǘ�����\���̂��Q�b�g����B

int DefeatEnemy(void);//�|�����G�̐�
#endif
