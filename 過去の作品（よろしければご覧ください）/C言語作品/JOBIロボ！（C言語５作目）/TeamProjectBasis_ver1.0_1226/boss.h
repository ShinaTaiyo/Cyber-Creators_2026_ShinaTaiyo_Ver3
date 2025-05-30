//================================================================================================================================================================================================================================
//
// �{�X�̏����Ɋւ���w�b�_�[�t�@�C��
// Author : Atsumu Kuboichi
//
//================================================================================================================================================================================================================================
#ifndef _BOSS_H_
#define _BOSS_H_

#include "main.h"
#include "player.h"
#include "attackmodel.h"

//================================================================================================================================================================================================================================
// �}�N����`
//================================================================================================================================================================================================================================
#define MAX_BOSSPARTS		(14)				//�p�[�c�̍ő吔
#define MAX_BOSSMOTION		(5)					//���[�V������
#define MAX_BOSSKEYSET		(100)				//���[�V�����̃L�[�̍ő吔
#define MAX_BOSSMAT			(100)				//�}�e���A���̍ő吔
#define BOSSSET				"data\\BOSSSET.txt"	//�{�X�̏��txt�t�@�C��
#define NUM_BOSSATTACK		(10)                 //�{�X�̍U���̎��
#define MAX_PURPOSEFLAG		(3)					//�ėp�t���O�̍ő吔
#define BOSS_SAMMON_TIME	(120)				//�{�X�o������

//================================================================================================================================================================================================================================
// �\����
//================================================================================================================================================================================================================================

//=========================================
// ���
//=========================================
typedef enum
{
	BOSSSTATE_NEUTRAL,//�j���[�g����
	BOSSSTATE_MOVE,//�ړ�
	BOSSSTATE_ATTACK,//�U��
	BOSSSTATE_MAX
}BOSSSTATE;

//=========================================
//�U���̎��
//=========================================
typedef enum
{
	BOSSATTACK00_ATOMICPRESS = 0,  //�U���O�F�A�g�~�b�N�v���X
	BOSSATTACK01_FLASHPUNCH,       //�U���P�F�t���b�V���p���`
	BOSSATTACK02_THRUSTSTUB,       //�U���Q�F�X���X�g�X�^�u
	BOSSATTACK03_BIRDSTRIKE,       //�U���R�F�o�[�h�X�g���C�N
	BOSSATTACK04_BOMBRAIN,         //�U���S�F�{�����C��
	BOSSATTACK05_QUELLTHEEARTH,    //�U���T�F�N�E�F���W�A�[�X
	BOSSATTACK06_BURNINGCHICKEN,   //�U���U�F�o�[�j���O�`�L��
	BOSSATTACK07_SLANDERTORNADO,   //�U���V�F�X�����_�[�g���l�[�h
	BOSSATTACK08_STARDUSTMETEO,    //�U���W�F�X�^�[�_�X�g���e�I
	BOSSATTACK09_COLLAPSELASER,    //�U���X�F�R���v�X���[�U�[
	BOSSATTACK_MAX
}BOSSATTACK;

//=========================================
// �L�[�̏��\����
//=========================================
typedef struct
{
	D3DXVECTOR3 pos;//�ʒu
	D3DXVECTOR3 rot;//����
}BossKey;

//=========================================
// �L�[�Z�b�g�̏��\����
//=========================================
typedef struct
{
	BossKey key[MAX_BOSSPARTS];
	int nMaxFrame;
}BossKeySet;

//=========================================
// ���[�V�����̏��\����
//=========================================
typedef struct
{
	int nCntKey;//�L�[���J�E���^
	int nCntFrame;//�t���[�����J�E���^
	int nMaxKey;//�e���[�V�����̍ő�L�[��
	int nLoop;//�e���[�V���������[�v���邩�ǂ���
	BossKeySet keyset[MAX_BOSSKEYSET];//�p�[�c�̏��
}BossMotionSet;

//=========================================
// �e�p�[�c�̏��\����
//=========================================
typedef struct
{
	LPD3DXMESH pMesh;						//���b�V���̒��_���ւ̃|�C���^
	LPD3DXBUFFER pBuffMat;					//�}�e���A���ւ̃|�C���^
	DWORD dwNumMat;							//�}�e���A���̐�
	D3DXMATRIX mtxWorld;					//���[���h�}�g���b�N�X
	D3DCOLORVALUE Diffuse[MAX_BOSSMAT];		//�F����
	D3DXVECTOR3 pos;						//�ʒu
	D3DXVECTOR3 rot;						//����
	D3DXVECTOR3 vtxMin;//���ꂼ��̍��W�̓G�̈ʒu�̍ŏ��l
	D3DXVECTOR3 vtxMax;//���ꂼ��̍��W�̓G�̈ʒu�̍ő�l
	D3DXVECTOR3 PartsPos;//�p�[�c���Ƃ̃}�g���b�N�X�̈ʒu
	int nParent;							//�e�̔ԍ�
}BossParts;//�p�[�c���Ƃ̓���

//=========================================
// �G�̏��\����
//=========================================
typedef struct
{
	D3DXMATRIX mtxWorld;//���[���h�}�g���b�N�X
	D3DXVECTOR3 pos;//�ʒu
	D3DXVECTOR3 posOld;//1F�O�̈ʒu
	D3DXVECTOR3 move;//�ړ���
	D3DXVECTOR3 rot;//����
	D3DXVECTOR3 vtxMin;//���ꂼ��̍��W�̓G�̈ʒu�̍ŏ��l
	D3DXVECTOR3 vtxMax;//���ꂼ��̍��W�̓G�̈ʒu�̍ő�l
	D3DXVECTOR3 Size;//�G�̑傫��
	BOSSSTATE state;//���
	int nIdxShadow;//�e�̃C���f�b�N�X
	int nIdxRaderDisplay;//���[�_�[�f�B�X�v���C�̃C���f�b�N�X
	int nNumParts;//�p�[�c��
	int nHp;//�̗�
	int nMaxHp;//�̗͍ő�l
	int nAttack;//�U����
	int nDefeatScore;//�|�����Ƃ��ɂ��炦��X�R�A
	int nAttackTiming;//�U�����������^�C�~���O�𑪂�
	int nNumLandingModel;//�ǂ̃��f���̏�ɏ���Ă��邩
	int nPattern;//�p�^�[��
	int nCntAppearTime;//��������Ă���̎���
	int nRunAwayLottery;//�{�X���m���œ���������ϐ�
	int nCntSummonEnemy;//�{�X�����������G�̐����J�E���g����
	int nTotalDamage;//�_���[�W���v��
	int nDamageCnt;//�_���[�W�����J�E���g����
	int nCntUseAttack;//�U���������񐔂��J�E���g����

	float fRotMove;//�ꕔ�̏����ŁA���̕ϐ��̒l�̕����Ɉړ�������
	float fSpeed;//����
	float fVXaim;//X�����̃x�N�g��
	float fVYaim;//Y�����̃x�N�g��
	float fVZaim;//Z�����̃x�N�g��
	float fVaim;//���������x�N�g��
	float fVaimY1;//��������Y�����̃x�N�g��
	float fLength;//����
	float fMoveStartLength;//�ړ����J�n���鋗��
	float fAttackStartLength;//�U�����J�n���鋗��
	bool bHitFlag;//�U�����q�b�g�������ǂ��������߂�ϐ�
	bool bUnderAttack;//�U�������ǂ����𔻒肷��ϐ�
	bool bUse;//�g�p���Ă��邩�ǂ���
	bool bIsLanding;//�n�ʂɂ��邩�ǂ���
	bool bBossSummon;//�{�X�����t���O
	bool bBossAppear;//�{�X���o�����o�����ǂ���
	bool bCurveLaser;//���[�U�[���J�[�u��������������߂�
	bool bRunAwayBoss;//�t���O�������Ƀ{�X�𓦂�������

	BossMotionSet aMotionset[MAX_BOSSMOTION];//�e���[�V�������
	BossParts aParts[MAX_BOSSPARTS];//�e�p�[�c���

	//=========================
	//�U���̎󂯕t������
	//=========================
	bool bHitStop[PLAYERATTACK_MAX];//���̕ϐ���true�̊ԁA�U�����󂯕t���Ȃ��i�v���C���[�̍U���̎�ނ��Ƃɕ�����B)
	int nCoolTime[PLAYERATTACK_MAX];//bHitStop��false�ɂ���܂ł̃J�E���g�i�v���C���[�̍U���̎�ނ��Ƃɕ�����B)
	bool bHitStopBoomerang[MAX_ATTACKMODEL];//�u�[�������ɑ΂���q�b�g�X�g�b�v�̔��������
	int nCoolTimeBoomerang[MAX_ATTACKMODEL];//�u�[�������ɑ΂���q�b�g�X�g�b�v�̃J�E���g������
	
	//===============================
	//�{�X�U�������p
	//===============================
	int nCntAttackTime;//�U�������Ă��鎞��
	int nLotteryAttack;//�U���𒊑I����
	int nAttackCoolTime;//�U���̃N�[���^�C��
	float fSaveAim;//�x�N�g����ۑ�����
	
	//======================================
	//�R�������@�_���e�̎����Ɏg���ϐ�
	//======================================
	float fVXaim3D;
	float fVYaim3D;
	float fVZaim3D;
	float fVLaim3D;

	D3DXVECTOR3 SaveMove3D;//�ړ��ʂ̕ۑ�������@
}Boss;

typedef struct
{
	bool bUse;       //�g�p���Ă��邩�ǂ���
	D3DXVECTOR3 pos; //�ʒu
	D3DXVECTOR3 move;//�ړ���
	D3DXVECTOR3 rot; //����
	float fAnimationPattern;//�A�j���[�V�����p�^�[��
	float fAnimationCnt;//�A�j���[�V�����J�E���g
}GraspBoss;

//=========================================
// �e�p�[�cX�t�@�C��
//=========================================
static const char* BOSSPARTS_FILENAME[MAX_BOSSPARTS] =
{
	"data\\ENEMY\\karaagekun\\00_body.x",
	"data\\ENEMY\\karaagekun\\01_head.x",
	"data\\ENEMY\\karaagekun\\04_armL.x",
	"data\\ENEMY\\karaagekun\\03_handR.x",
	"data\\ENEMY\\karaagekun\\02_armR.x",
	"data\\ENEMY\\karaagekun\\05_handL.x",
	"data\\ENEMY\\karaagekun\\06_footR.x",
	"data\\ENEMY\\karaagekun\\07_legR.x",
	"data\\ENEMY\\karaagekun\\08_footL.x",
	"data\\ENEMY\\karaagekun\\09_legL.x",
	"data\\ENEMY\\karaagekun\\10_wingR.x",
	"data\\ENEMY\\karaagekun\\11_wingL.x",
	"data\\ENEMY\\karaagekun\\12_weapon.x",
	"data\\ENEMY\\karaagekun\\13_shield.x",
};

//================================================================================================================================================================================================================================
// �v���g�^�C�v�錾
//================================================================================================================================================================================================================================
void InitBoss(void);		            //����������
void InitGraspBoss(void);               //�{�X�ʒu�c���\���̏���������
void UninitBoss(void);		            //�I������
void UninitGraspBoss(void);             //�{�X�ʒu�c���\���̏I������
void UpdateBoss(void);		            //�X�V����
void UpdateGraspBoss(void);             //�{�X�̈ʒu�c���\���̍X�V����
void DrawBoss(void);		            //�`�揈��
void DrawGraspBoss(void);               //�{�X�̈ʒu�c���\���̍X�V����
void BossMotion(void);		            //���[�V��������
void LoadBossParts(void);	            //�p�[�c���ǂݍ��ݏ���
void LoadBossMotion(void);	            //���[�V�������ǂݍ��ݏ���
void SetBoss(D3DXVECTOR3 pos, int nHp);	//�{�X����������
void SetGraspBoss(void);                //�{�X�ʒu�c���\���̐ݒ�
void SetDamageBoss(int nDamage,PLAYERATTACK PlayerAttackNum,int nNumAttackModel);//�{�X�Ƀ_���[�W��^����
void HitStopBoss(void);                 //�{�X�ւ̍U���̃q�b�g�X�g�b�v����
void CaluculateMatrixBoss(void);        //�{�X�̃}�g���b�N�X���v�Z����
Boss* GetBoss(void);                    //�{�X�̏����擾����

//=============================
//�{�X�̍U�������֐�
//=============================
void BossAttack0_AtomicPress(void);   //�{�X�U���O�F�A�g�~�b�N�v���X
void BossAttack1_FlashPunch(void);    //�{�X�U���P�F�t���b�V���p���`
void BossAttack2_ThrustStub(void);    //�{�X�U���Q�F�X���X�g�X�^�u
void BossAttack3_BirdStrike(void);    //�{�X�U���R�F�o�[�h�X�g���C�N
void BossAttack4_BombRain(void);      //�{�X�U���S�F�{�����C��
void BossAttack5_QuellTheEarth(void); //�{�X�U���T�F�N�E�F���W�A�[�X
void BossAttack6_BurningChicken(void);//�{�X�U���U�F�o�[�j���O�`�L��
void BossAttack7_SlanderTornado(void);//�{�X�U���V�F�X�����_�[�g���l�[�h
void BossAttack8_StardustMeteo(void); //�{�X�U���W�F�X�^�[�_�X�g���e�I
void BossAttack9_CollapseLaser(void); //�{�X�U���X�F�R���v�X���[�U�[

void RunAwayBoss(void);               //�{�X�������鏈��

#endif
