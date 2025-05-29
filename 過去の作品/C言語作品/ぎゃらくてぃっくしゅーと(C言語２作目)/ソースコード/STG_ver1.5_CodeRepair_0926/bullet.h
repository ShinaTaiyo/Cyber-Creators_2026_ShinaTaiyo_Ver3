//========================================================
//
//�W���R�O���F�G�t�F�N�g������񌩒���[bullet.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"


#define MAX_BULLET (4096)//�e�̍ő吔
#define GENERALLY_BULLETLIFE (1500)//�ʏ�̒e�̃��C�t

//�e�̎��
typedef enum
{
	BULLETTYPE_PLAYER = 0,//�v���C���[�̒e
BULLETTYPE_ENEMY,
BULLETTYPE_SKILL1,
BULLETTYPE_REAFY,
BULLETTYPE_ROT,
BULLETTYPE_SKILL2_SLASH,
BULLETTYPE_ENEMY9,
BULLETTYPE_BOLBY,
BULLETTYPE_BANDY,
BULLETTYPE_ARROW,
BULLETTYPE_HORMING,//�z�[�~���O�V���b�g
BULLETTYPE_SCATTERING,//�΂�܂��g�U�e
BULLETTYPE_REVERSE_CURVE,//BULLETTYPE_REAFY�̋t�����̃J�[�u�̒e
BULLETTYPE_RIGHT_WIND,//�E�ɔ��˂���Ă��獶�ɂ��񂾂�ړ����Ă����e
BULLETTYPE_LEFT_WIND,//���ɔ��˂���Ă���E�ɂ��񂾂�ړ����Ă����e
BULLETTYPE_REFLECTION,//�ǂɂ�����Ɣ��˂���e
BULLETTYPE_NOVA,//�X�L���u�m���@�u���X�g�̒e�v
BULLETTYPE_ROCKON,//���̒e����X�Ƀv���C���[�ւ̎��@�_���̒e�����˂����B
BULLETTYPE_DOWNWIND,
BULLETTYPE_UPWIND,
BULLETTYPE_SATELLITE,//�q���e
BULLETTYPE_MAX
}BULLETTYPE;

//�e�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;//�ʒu
	D3DXVECTOR3 move;//�ړ���
	int nLife;//�����i�ǂ̂��炢�e�����ł������j
	BULLETTYPE type;//���
	bool bUse;//�e���g�p���Ă��邩�ǂ���
	float fCurveBulletX;
	float fCurveBulletY;
	int nCntBullet_2;
	int nAnimationCounter_Slash;//�a���A�j���[�V�����J�E���^�[
	float fAnimationPattern_Slash;//�a���A�j���[�V�����p�^�[��
	bool bCurveFlag;//�J�[�u�̕�����\���t���O
	float fEnemy5Curve;//�G�l�~�[�T�̒e���J�[�u������
	int nApperCount;//�G���o�������u�ԂɃJ�E���g���J�n����B
	float fBulletMove_Enemy9;//�G�l�~�[�X�̒e
	float fBulletMoveX_Enemy9;//�G�l�~�[�X��X���W�̒e
	float fBulletMoveY_Enemy9;//�G�l�~�[�X��Y���W�̒e
	int nNumEnemy;//�ǂ̃G�l�~�[�����˂��Ă���e���𔻕ʂ���
	float fXVaim;
	float fYVaim;
	float fShotVectorX;
	float fShotVectorY;
	float fVaim;
	int nNumber;	// �G�l�~�[�T�̔ԍ�
	bool bBulletReflectionX;//X�����ɔ��˂�����t���O
	bool bBulletReflectionY;//Y�����ɔ��˂�����t���O
	bool HormingFlag;
	float fRotMove;
	float fRotDest;
	float fRotDiff;
	int nHormingCount;
	int nHormingaim;//�z�[�~���O�̃^�[�Q�b�g�̃G�l�~�[�𔻕ʂ���B
	float fSpeed;//�o���b�g�̃X�s�[�h��ς���
	float fCurve;//�o���b�g�̃J�[�u�l��ς���
}Bullet;

//�v���g�^�C�v�錾
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move,int nLife,BULLETTYPE type, int nNum,int nNumEnemy,int nAimingEnemy);//pos�́u���ˈʒu�v�Amove�́u�ړ��ʁv,damage�́u�e�̍U���́vnNumEney�́A���Ԗڂ̓G���U�����Ă��邩?

Bullet* GetBullet(void);//�e�\���̂̏����擾����B

#endif
