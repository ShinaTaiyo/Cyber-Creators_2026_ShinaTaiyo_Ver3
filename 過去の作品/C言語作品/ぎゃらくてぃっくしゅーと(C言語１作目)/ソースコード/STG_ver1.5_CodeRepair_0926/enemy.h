//========================================================
//
//�W���R�O���F�G�t�F�N�g������񌩒���[enemy.cpp]
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
	ENEMYTYPE_DELBY = 0,
	ENEMYTYPE_PUNDY,
	ENEMYTYPE_SMILY,
	ENEMYTYPE_SQUDY,
	ENEMYTYPE_REAFY,
	ENEMYTYPE_HONOBY,
	ENEMYTYPE_MIZUBY,
	ENEMYTYPE_BOMUBY,
	ENEMYTYPE_WINBY,
	ENEMYTYPE_BOLBY,
	ENEMYTYPE_BANDY,
	ENEMYTYPE_BOSS1_STARWARRIOR,
	ENEMYTYPE_MINIBOMUBY,
	ENEMYTYPE_MAX
}ENEMYTYPE;


//�G�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;//�ʒu
	D3DXVECTOR3 move;//�ړ���
	D3DXVECTOR3 rot;//����
	int nType;//���
	ENEMYSTATE State;//���
	int nCounterState;//��ԊǗ��J�E���^�[
	float fLife;//�̗�
	bool bUse;//�g�p���Ă��邩�ǂ���
	int nEnemyCount;//�G�̍ő吔���J�E���g����ϐ�
	bool bEnemyMoveX;//X�����̃X�N���[���Z�[�o�[
	bool bEnemyMoveY;//Y�����̃X�N���[���Z�[�o�[
	bool bSetEnemyUse;//�G���Z�b�g�����Ƃ���true�ɂȂ�t���O
	bool bDefeatFlag;//�G���Z�b�g������A�G��|������false�ɂȂ�t���O
	float fEnemy5BulletCurve;//�G�l�~�[�T�̒e�̃J�[�u�̒l

	bool bBossSummonFlag;//�{�X�����t���O
	float fBossMove;//�{�X����莞�Ԃ��Ƃɓ������t���O�B
	bool bBossMoveFlag;//�t���O��true�̎��Ƀ{�X�𓮂����t���O�B

	int nCountMoveInterval;//�G�������܂ł̎��Ԃ��v��
	int nMoveTime;//�������Ԃ��v��

	float fRotMove;//�G�������_���ɓ��������̕��������߂�ϐ�

	bool bAttackInterval;//�G���U������C���^�[�o��������t���O

	int nAttarkIntervalCount;//�G���U������܂ł̎��Ԃ��v������B

	int nApperCount;//�G���Z�b�g����Ă���o������܂ł̎��Ԃ��v������

	bool bEnemyCountminusFlag;//�G������Ă��瑍�������炷�J�E���g����񂾂�����������t���O
	//bool bResultFlag;
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
void HitEnemy(int nCntEnemy, float fDamage);//�unCntEnemy�v�́A�ǂ̓G�ɁH�A�unDamage�v�́A�ǂ̂��炢�_���[�W��^����H
Enemy* GetEnemy(void);

Result* GetResult(void);

Defeat* GetDefeat(void);//�E�F�[�u�����Ǘ�����\���̂��Q�b�g����B
#endif
