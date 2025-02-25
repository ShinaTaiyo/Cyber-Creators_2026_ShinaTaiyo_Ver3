//========================================================
//
//�P�O���Q�O���F�{�X�P�����[enemy.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#include "main.h"
#include "enemy.h"
#include "bullet.h"
#include "explosion.h"
#include <stdlib.h>
#include <time.h>
#include "score.h"
#include "game.h"
#include "result.h"
#include "player.h"
#include "sound.h"
#include "particle.h"
#include "staging.h"
#include "damage.h"
#include "input.h"
#include "block.h"
#include "Attack.h"
#include "Gage.h"
#include "Item.h"
#include "warp.h"
#include "map.h"
#include "health.h"
#include "title.h"
#include "special.h"
//#include "Gage.h"
//#include "wave.h"
//#include "Difficulty.h"
//�}�N����`
#define NUM_ENEMY (ENEMYTYPE_MAX)//�G�̎��
#define MAX_ENEMYBULLET (10000)
#define MAX_CIRCLEBULLET_2 (10)//�~�`�e�U�e�̒e��
#define MAX_CIRCLEBULLET_E10 (8)
#define BOSS1_SIZE (120.0f)


#define ENEMY_WIDTH (30.0f)
#define ENEMY_HEIGHT (30.0f)

#define ENEMY8BULLET (30)

#define NUM_BOSS_PATTERN (7)//�{�X�̋Z�̎�ނ̐�

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_apTextureEnemy[NUM_ENEMY] = {};
D3DXVECTOR3 g_rotPI;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;
Enemy g_aEnemy[MAX_ENEMY];//�G�̏��
Result g_aResult;

Defeat g_aDefeat;
int g_nNumEnemy = 0;//�G�̑���

//int g_nCountTime = 0;//���Ԃ�\���O���[�o���ϐ�

int g_nDelay = 0;
int g_nBulletShift = 0;//�e�̋O�������炷�ϐ�

int g_nRandumBullet1=0;
float g_fRandumResultBullet = 0.0f;
int g_nRandumBullet2 = 0.0f;
float g_fRandumResultBullet2 = 0.0f;

int g_nRandumBullet3 = 0;
float g_fRandumResultBullet3 = 0.0f;

float g_fXmoveEnemy[MAX_ENEMY];

bool g_bResultFlag = false;

int g_nCntEnemyParticle[MAX_ENEMY];
bool g_bSetParticle[MAX_ENEMY];

//==================================================
float g_fBulletCount = 0;//�e�̕��������p�ϐ�
bool g_bBulletDirectionFlag = false;
//==================================================
// �~�`�e�U�e�̏����p�ϐ�
float g_fCircleBullet[MAX_CIRCLEBULLET];
float g_fCircleBullet_E5[MAX_CIRCLEBULLET_2];
//=================================
//�J�[�u�g�U�e�̏����p�ϐ�
//=================================
float g_fCurveBullet[MAX_CIRCLEBULLET_2];
//=============================================

float g_fLengthEnemy;
float g_fAngleEnemy;

int g_nDefeatCount;

//=====================================================
//���@�_���e�̏���
//=====================================================
float g_fXVaim[MAX_ENEMY];
float g_fYVaim[MAX_ENEMY];
float g_Vaim[MAX_ENEMY];


float g_ShotVectorX[MAX_ENEMY];
float g_ShotVectorY[MAX_ENEMY];

//===============================
//�G�l�~�[�T�̏���
//===============================
int g_nE5_CurveCount;
int g_nCurveBullet_E5[MAX_CIRCLEBULLET_2];

//=====================================================
//�G�l�~�[�V�̏���
//=====================================================
float g_fRolingBulletX_E7;
float g_fRolingBulletY_E7;

//======================================================
//�G�l�~�[�W�̏���
//======================================================
float g_fSpeedBullet_E8[ENEMY8BULLET];

float g_fCircleBullet_E8[ENEMY8BULLET];

//======================================================
//�G�l�~�[�X�̏���
//======================================================
int g_nSet_SpeedBullet_E9;//rand�֐��ɂ��Ă͂߂�p
float g_fSpeedBullet_E9;//�e�̃X�s�[�h�������_���ɂ���B

//�G�l�~�[�P�O�̏���
int g_nCIrcleBullet_E10[MAX_CIRCLEBULLET_E10];


bool g_bClearFlag;

//============================================
//�{�X�P�̏���
//============================================
int g_nBoss1TechiniquePattern;//�o���Ă���e����I��������ϐ�
int g_nCountTechiniqueTime;//�I�񂾋Z���o�����Ԃ��J�E���g����ϐ�
bool g_bBoss1Flag;

float g_fRotBullet_W6;

bool g_bNohitFlag;


float g_fRot;

float g_fSaveSpeed;

float g_fRolingBullet[100];

int g_nCntAttack;
int g_nCntAttack2;
int g_nCntAttack3;

bool g_bAttackFlag;


//=====================================================
//==========================
//�G�̏���������
//==========================
void InitEnemy(void)
{
	//GameCustom* pGameCustom = GetGameCustom();
	int nA = 0;
	bool bAttackFlag = false;

	g_nCntAttack = 0;
	g_nCntAttack2 = 0;

	g_nBulletShift = 0;
	
	g_bClearFlag = false;

	g_bNohitFlag = false;

	g_bBoss1Flag=false;

	g_fSaveSpeed = 0;

	g_nBoss1TechiniquePattern = 0;//������
	g_nCountTechiniqueTime = 0;//������

	g_fRotBullet_W6 = 0.0f;

	g_nE5_CurveCount = 0;

	//g_aDefeat.nDefeatCount = 0;//���݂̃E�F�[�u���O�ɂ���B

	g_aDefeat.DefeatEnemyCount = 0;//�G���j���̏�����
	int nCntBullet4;
	
	g_fSpeedBullet_E8;

	g_fRot = 0.0f;

		//srand((unsigned int)time(NULL));//�Q�[�����J�n�������Ԗ��ɗ����̎��ݒ�

	g_aResult.bResultFlag = false;
	int nCntBullet;
	

	for (int nCnt = 0; nCnt < 100; nCnt++)
	{
		g_fRolingBullet[nCnt] = 0.0f;
	}
	for (nCntBullet = 0; nCntBullet < MAX_CIRCLEBULLET; nCntBullet++)
	{//�~�`�e�U�e�̏�����
		g_fCircleBullet[nCntBullet] = 0;
	}


	for (nCntBullet4 = 0; nCntBullet4 < ENEMY8BULLET; nCntBullet4++)
	{
		//g_fCircleBullet_E8[nCntBullet4] = (1.0f / (ENEMY8BULLET / 2)) * nCntBullet4;

		g_fSpeedBullet_E8[nCntBullet4] = 0;


		//SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * g_fCircleBullet_E8[nCntBullet4]) * g_fSpeedBullet_E8[nCntBullet4]), (cosf(-D3DX_PI * g_fCircleBullet_E8[nCntBullet4]) * g_fSpeedBullet_E8[nCntBullet4]), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY);//�G�̒e������G���m�œ�����Ȃ�	
	}


	g_aEnemy->nEnemyCount = NUM_ENEMY;

	LPDIRECT3DDEVICE9 pDevice;
	int	nCntEnemy;

	g_nNumEnemy = 0;//�G���j���̏�����


	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ��݁i�G�̃e�N�X�`����ǂݍ��ށj
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Enemy0_MINIPUNI.png",
		&g_apTextureEnemy[ENEMYTYPE0_MINIPUNI]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Enemy1_HONOPUNI.png",
		&g_apTextureEnemy[ENEMYTYPE1_HONOPUNI]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\enemy2_SODOPUNI.png",
		&g_apTextureEnemy[ENEMYTYPE2_SODOPUNI]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Enemy3_ICESLIME.png",
		&g_apTextureEnemy[ENEMYTYPE3_ICESLIME]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Enemy4_BILIBILISLIME.png",
		&g_apTextureEnemy[ENEMYTYPE4_BILIBILISLIME]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Enemy5_SNIPERSLIME.png",
		&g_apTextureEnemy[ENEMYTYPE5_SNIPERSLIME]);


	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Enemy6_BOMUSLIME.png",
		&g_apTextureEnemy[ENEMYTYPE6_BOMUSLIME]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Enemy7_FLOWERSLIME.png",
		&g_apTextureEnemy[ENEMYTYPE7_FLOWERSLIME]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Enemy8_CATSLIME.png",
		&g_apTextureEnemy[ENEMYTYPE8_CATSLIME]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Enemy9_STONESLIME.png",
		&g_apTextureEnemy[ENEMYTYPE9_STONESLIME]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Enemy10_WITCHSLIME.png",
		&g_apTextureEnemy[ENEMYTYPE10_WITCHSLIME]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Boss1_StandingMotion.png",
		&g_apTextureEnemy[ENEMYTYPE_BOSS1_STAND]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Boss1_RunMotion.png",
		&g_apTextureEnemy[ENEMYTYPE_BOSS1_RUN]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Boss1_AttackMotion.png",
		&g_apTextureEnemy[ENEMYTYPE_BOSS1_ATTACK]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Boss2_StandingMotion.png",
		&g_apTextureEnemy[ENEMYTYPE_BOSS2_STAND]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Boss2_RunMotion.png",
		&g_apTextureEnemy[ENEMYTYPE_BOSS2_RUN]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Boss2_AttackMotion.png",
		&g_apTextureEnemy[ENEMYTYPE_BOSS2_ATTACK]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Boss3_StandingMotion.png",
		&g_apTextureEnemy[ENEMYTYPE_BOSS3_STAND]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Boss3_RunMotion.png",
		&g_apTextureEnemy[ENEMYTYPE_BOSS3_RUN]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Boss3_AttackMotion.png",
		&g_apTextureEnemy[ENEMYTYPE_BOSS3_ATTACK]);


	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Boss4_StandingMotion.png",
		&g_apTextureEnemy[ENEMYTYPE_BOSS4_STAND]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Boss4_RunMotion.png",
		&g_apTextureEnemy[ENEMYTYPE_BOSS4_RUN]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Boss4_AttackMotion.png",
		&g_apTextureEnemy[ENEMYTYPE_BOSS4_ATTACK]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\FinalBoss_StandingMotion.png",
		&g_apTextureEnemy[ENEMYTYPE_FINALBOSS_STAND]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\FinalBoss_RunMotion.png",
		&g_apTextureEnemy[ENEMYTYPE_FINALBOSS_RUN]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\FinalBoss_AttackMotion.png",
		&g_apTextureEnemy[ENEMYTYPE_FINALBOSS_ATTACK]);
	//g_aEnemy->pos = D3DXVECTOR3(0.0f,0.0f, 0.0f);//�ʒu������������

	//�G���̏�����
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		g_nCntEnemyParticle[nCntEnemy] = 0;
		g_bSetParticle[nCntEnemy] = false;

		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].nType = 0;//���ꂼ��̃G�l�~�[�ɓG�̎�ށinType)������U���Ă���B
		g_aEnemy[nCntEnemy].bUse = false;//�g�p���Ă��Ȃ���Ԃɂ���
		g_aEnemy[nCntEnemy].State = ENEMYSTATE_NORMAL;//�G�̏�Ԃ𕁒ʂ̏�Ԃɏ�����
		g_aEnemy[nCntEnemy].nCounterState = 0;//��Ԃ�ς��鎞�Ԃ�\���J�E���^�[
		g_aEnemy[nCntEnemy].fLife = 0.0f;//�����̏�����
		g_aEnemy[nCntEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_fXmoveEnemy[nCntEnemy] = 0;
		g_aEnemy[nCntEnemy].bEnemyMoveX = false;
		g_aEnemy[nCntEnemy].bEnemyMoveY= false;
		g_aEnemy[nCntEnemy].bSetEnemyUse = false;
		g_aEnemy[nCntEnemy].bDefeatFlag = false;
		g_aEnemy[nCntEnemy].bBossSummonFlag = false;//�{�X�����t���O
		g_aEnemy[nCntEnemy].fBossMove = 0.0f;
		g_aEnemy[nCntEnemy].bBossMoveFlag = false;
		g_aEnemy[nCntEnemy].nCountMoveInterval = 0;
		g_aEnemy[nCntEnemy].nMoveTime= 0;
		g_aEnemy[nCntEnemy].fRotMove = 0.0f;
		g_aEnemy[nCntEnemy].bAttackInterval = false;
		g_aEnemy[nCntEnemy].nAttarkIntervalCount = 0;
		g_aEnemy[nCntEnemy].nApperCount = 0;
		g_aEnemy[nCntEnemy].bEnemyCountminusFlag = false;
		g_aEnemy[nCntEnemy].fWidth = 0.0f;//�G�̕�
		g_aEnemy[nCntEnemy].fHeight = 0.0f;//�G�̍���
		g_aEnemy[nCntEnemy].nCntAnim = 0;//�A�j���[�V�����J�E���^�[
		g_aEnemy[nCntEnemy].nPatternAnim = 0;//�A�j���[�V�����p�^�[��
		g_aEnemy[nCntEnemy].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].bIsLanding = false;//�n�ʂɂ��Ă��邩�ǂ����̏���
		g_aEnemy[nCntEnemy].bIsWalling = false;//�ǂɓ������Ă��邩�ǂ����̏���
		g_aEnemy[nCntEnemy].fGravity = 0.0f;//�n�ʂɂ��Ă��邩�ǂ����̏���
		g_aEnemy[nCntEnemy].bDirection = false;//���E�ǂ���������Ă���̂��𔻒肷��t���O
		g_aEnemy[nCntEnemy].fXVaim = 0.0f;//���@�_��������Ƃ��̏���
		g_aEnemy[nCntEnemy].fYVaim = 0.0f;//���@�_��������Ƃ��̏���
		g_aEnemy[nCntEnemy].fVaim = 0.0f;//���@�_��������Ƃ��̏���
		g_aEnemy[nCntEnemy].nCntJumpTime = 0;//�W�����v���鎞�Ԃ��J�E���g����
		g_aEnemy[nCntEnemy].nTypeOld = 0;//1f�O�̃^�C�v
		g_aEnemy[nCntEnemy].nAttackCount = 0;//�s�����Ƃ̍U���񐔂��J�E���g����
		g_aEnemy[nCntEnemy].bMoveWay = false;//�������������߂�t���O
		g_aEnemy[nCntEnemy].bMovePattern = false;//���̏u�Ԃɓ���������ތ��߂�t���O
		g_aEnemy[nCntEnemy].fMaxHp = 0.0f;//�ő�̗͂�ۑ�����
		g_aEnemy[nCntEnemy].bSpecialStaging = false;//�K�E�Z�̉��o���������ǂ����𔻒肷��t���O
		g_aEnemy[nCntEnemy].bGravity = false;//�d�͂������邩�ǂ����̃t���O
	}


	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ENEMY,//�S�͕K�v�Ȓ��_��
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEnemy,
		NULL);

	VERTEX_2D* pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//���_���W�̐ݒ�(�G�̑傫���j
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(80.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 80.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(80.0f, 80.0f, 0.0f);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;//���_�f�[�^�̃|�C���^���S���i�߂�B
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEnemy->Unlock();
}

//====================================
//�G�̏I������
//====================================
void UninitEnemy(void)
{
	//�e�N�X�`���̔j��
	int nCntEnemy;
	for (nCntEnemy = 0; nCntEnemy < NUM_ENEMY; nCntEnemy++)
	{
		if (g_apTextureEnemy[nCntEnemy] != NULL)
		{
			g_apTextureEnemy[nCntEnemy]->Release();
			g_apTextureEnemy[nCntEnemy] = NULL;
		}
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffEnemy != NULL)
	{
		g_pVtxBuffEnemy->Release();
		g_pVtxBuffEnemy = NULL;
	}
}


//========================================
//�G�̍X�V����
//========================================
void UpdateEnemy(void)
{
	//GameCustom* pGameCustom = GetGameCustom();

	//LPDIRECT3DDEVICE9 pDevice;
	int	nCntBullet;
	int nCntEnemy;
	int nCntBullet2;
	int nCntBullet3;
	int nRandBullet2;

	g_fRot += 0.01f;

	/*g_aEnemy[nCntEnemy].fRotMove;*/
	VERTEX_2D* pVtx;

	Bullet* pBullet = GetBullet();//�e�\���̂̏����Q�b�g����B

	Player* pPlayer = GetPlayer();//�v���C���[�\���̂̏����Q�b�g����B

	Map_h_DefeatBoss* pDefeatBoss = GetDefeatBoss();

	g_nE5_CurveCount++;//�G�l�~�[�T�̒e�J�[�u������p�̕ϐ��B�J�E���g�X�^�[�g

	g_fRotBullet_W6 += 0.01f;//�{�X�P�̋Z�U�̒e�̔��˕��������炵�����鏈��

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);
	//srand((unsigned int)time(NULL));

	g_nRandumBullet1 = rand() % 200 + 1;//0�`100�̒l��ݒ�(�����e�j

	g_nRandumBullet2 = rand() % 100 + 1;//�~�`�e�U�e�̔��˕����̒����p

	g_nRandumBullet3 = rand() % 100 + 1;//�~�`�e�U�e�̔��˕����̒����p

	g_nSet_SpeedBullet_E9 = rand() % 50 + 1;//�G�l�~�[�X�̒e�̃X�s�[�h�����p

	

	g_fRandumResultBullet = 0.01f * g_nRandumBullet1;//0�`2.0�̊ԂŒl��ϓ�������B

	g_fRandumResultBullet2 = 0.01f * g_nRandumBullet2;//�~�`�g�U�e�̕�����������

	g_fRandumResultBullet3 = 0.01f * g_nRandumBullet3;

	g_fSpeedBullet_E9 = 0.1f * g_nSet_SpeedBullet_E9;//�G�l�~�[�X�̒e�̃X�s�[�h���m��
	//=========================================
	//�G�l�~�[7�e������
	//=========================================
	g_fRolingBulletX_E7 += 0.01f;
	g_fRolingBulletY_E7 += 0.01f;

	if (g_nDelay % 9 == 0)
	{
		g_fRolingBulletX_E7 += 0.05f;
		g_fRolingBulletY_E7 += 0.03f;
	}
	//=============================================

	//========================================================================================================================================================================================================================================
	g_nDelay++;
	g_nBulletShift++;



	float fRandumSpeed_E12;

	fRandumSpeed_E12 = float((rand)() % 100 + 1) / 10.0f;//�e�𔭎˂������̃X�s�[�h�����߂�B




	//for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	//{
	//	if (g_aEnemy[nCntEnemy].bUse == true && g_aEnemy[nCntEnemy].nType == ENEMYTYPE_DELBY)
	//	{
	//		g_fXmoveEnemy[nCntEnemy]++;
	//	}
	//}
	//pBullet[pBullet->nCntBullet_2].fCurveBulletX += 0.002f;//X�����ɃJ�[�u������
	//pBullet[pBullet->nCntBullet_2].fCurveBulletY += 0.001f;//X�����ɃJ�[�u������

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{

		


		if (g_aEnemy[nCntEnemy].bUse == true)
		{

			g_aEnemy[nCntEnemy].bIsLanding = false;//�n�ʂɂ��Ă��邩�ǂ����̏���

			g_aEnemy[nCntEnemy].bIsWalling = false;//�ǂɓ������Ă��邩�ǂ����̏���

			if (g_aEnemy[nCntEnemy].bGravity == true)
			{
				g_aEnemy[nCntEnemy].bIsLanding = bCollisionBlock(&g_aEnemy[nCntEnemy].pos, &g_aEnemy[nCntEnemy].posOld, &g_aEnemy[nCntEnemy].move, g_aEnemy[nCntEnemy].fWidth, g_aEnemy[nCntEnemy].fHeight, &g_aEnemy[nCntEnemy].fGravity, COLLISIONTYPE_RADIUS, &g_aEnemy[nCntEnemy].bMoveWay, COLLISIONUSE_ENEMY, &g_aEnemy[nCntEnemy].bIsWalling);//�ǂɓ������Ă��邩�ǂ����̏���);
			}



			/*if (g_aEnemy[nCntEnemy].nApperCount >= 100)
			{*/


			//���_�����[�̒��S�ɐݒ�
			pVtx[0].pos.x = g_aEnemy[nCntEnemy].pos.x - g_aEnemy[nCntEnemy].fWidth;
			pVtx[0].pos.y = g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].fHeight;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].fWidth;
			pVtx[1].pos.y = g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].fHeight;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aEnemy[nCntEnemy].pos.x - g_aEnemy[nCntEnemy].fWidth;
			pVtx[2].pos.y = g_aEnemy[nCntEnemy].pos.y;//���_�������ɂ��邽��
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].fWidth;
			pVtx[3].pos.y = g_aEnemy[nCntEnemy].pos.y;//���_�������ɂ��邽��
			pVtx[3].pos.z = 0.0f;

			//===================================================
			//�G�̌������菈��
			//===================================================
			if (pPlayer->pos.x > g_aEnemy[nCntEnemy].pos.x)
			{//�v���C���[���G�̈ʒu���E���Ȃ�

				g_aEnemy[nCntEnemy].bDirection = false;//false�ŁA�E�������Ă�����

			}
			else
			{//�v���C���[���G�̈ʒu��荶���Ȃ�
				g_aEnemy[nCntEnemy].bDirection = true;//true�ŁA���������Ă�����
			}
			//==========================================================================================

			if (g_aEnemy[nCntEnemy].bUse == true && g_aEnemy[nCntEnemy].nType == ENEMYTYPE0_MINIPUNI)
			{//�G�l�~�[�O���g���Ă�����E�E�E



				g_aEnemy[nCntEnemy].nCntAnim++;//�A�j���[�V�����J�E���^�[

				if (g_aEnemy[nCntEnemy].nCntAnim % 10 == 0)
				{//�A�j���[�V�����̑��x
					g_aEnemy[nCntEnemy].nPatternAnim++;
				}

				if (g_aEnemy[nCntEnemy].nPatternAnim >= 2)
				{//�A�j���[�V�������ŏ��ɖ߂�
					g_aEnemy[nCntEnemy].nPatternAnim = 0;
				}



				//�e�N�X�`���A�j���[�V����
				pVtx[0].tex = D3DXVECTOR2(0.0f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.5f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.5f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 1.0f);
			}

			if (g_aEnemy[nCntEnemy].bUse == true && g_aEnemy[nCntEnemy].nType == ENEMYTYPE1_HONOPUNI)
			{//�G�l�~�[�P�̍U��
				//if (g_nDelay % 60 == 0)
				//{
				//	PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_000);
				//	SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * g_fRandumResultBullet) * 5.0f), (cosf(-D3DX_PI * g_fRandumResultBullet) * 5.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�
				//}
				if (g_nDelay % 80 == 0)
				{
					if (g_aEnemy[nCntEnemy].bDirection == true)
					{
						PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_000);
						SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].fHeight / 2, 0.0f)
							, D3DXVECTOR3((sinf(-D3DX_PI * 0.5f) * 5.0f), (cosf(-D3DX_PI * 0.5f) * 5.0f), 0.0f)
							, GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1, false, MOVETYPE_NORMAL);//�G�̒e������G���m�œ�����Ȃ�
					}
					else if (g_aEnemy[nCntEnemy].bDirection == false)
					{
						PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_000);
						SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].fHeight / 2, 0.0f)
							, D3DXVECTOR3((sinf(-D3DX_PI * 1.5f) * 5.0f), (cosf(-D3DX_PI * 1.5f) * 5.0f), 0.0f)
							, GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1, false, MOVETYPE_NORMAL);//�G�̒e������G���m�œ�����Ȃ�
					}
				}

				g_aEnemy[nCntEnemy].nCntAnim++;//�A�j���[�V�����J�E���^�[

				if (g_aEnemy[nCntEnemy].nCntAnim % 10 == 0)
				{//�A�j���[�V�����̑��x
					g_aEnemy[nCntEnemy].nPatternAnim++;
				}

				if (g_aEnemy[nCntEnemy].nPatternAnim >= 2)
				{//�A�j���[�V�������ŏ��ɖ߂�
					g_aEnemy[nCntEnemy].nPatternAnim = 0;
				}

				//�e�N�X�`���A�j���[�V����
				if (g_aEnemy[nCntEnemy].bDirection == true)
				{
					pVtx[0].tex = D3DXVECTOR2(0.0f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(0.5f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(0.0f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(0.5f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 1.0f);
				}
				else
				{
					pVtx[1].tex = D3DXVECTOR2(0.0f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 0.0f);
					pVtx[0].tex = D3DXVECTOR2(0.5f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 0.0f);
					pVtx[3].tex = D3DXVECTOR2(0.0f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 1.0f);
					pVtx[2].tex = D3DXVECTOR2(0.5f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 1.0f);
				}
			}

			if (g_aEnemy[nCntEnemy].bUse == true && g_aEnemy[nCntEnemy].nType == ENEMYTYPE2_SODOPUNI)
			{//�G�l�~�[�Q�i�\�h�v�j�j
				//�ߋ��̃o���b�g����
				//if (g_bBulletDirectionFlag == false)
				//{
				//	g_fBulletCount += 0.09f;
				//	if (g_nBulletShift % 4 == 0)
				//	{
				//		g_fBulletCount += 0.04f;
				//	}
				//}
				//if (g_bBulletDirectionFlag == true)
				//{
				//	g_fBulletCount -= 0.09f;
				//	if (g_nBulletShift % 3 == 0)
				//	{
				//		g_fBulletCount -= 0.03f;
				//	}
				//}
				//if (g_fBulletCount >= 0.5f)
				//{
				//	g_bBulletDirectionFlag = true;
				//}
				//if (g_fBulletCount <= -0.5f)
				//{
				//	g_bBulletDirectionFlag = false;
				//}
				//if (g_nDelay % 15 == 0)
				//{
				//	PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_000);
				//	SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * (0.0f + g_fBulletCount)) * 3.0f), (cosf(-D3DX_PI * (0.0f + g_fBulletCount)) * 4.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�

				g_aEnemy[nCntEnemy].nCntAnim++;//�A�j���[�V�����J�E���^�[

				if (g_aEnemy[nCntEnemy].nCntAnim % 5 == 0)
				{//�A�j���[�V�����̑��x
					g_aEnemy[nCntEnemy].nPatternAnim++;
				}

				if (g_aEnemy[nCntEnemy].nPatternAnim >= 2)
				{//�A�j���[�V�������ŏ��ɖ߂�
					g_aEnemy[nCntEnemy].nPatternAnim = 0;
				}

				//�e�N�X�`���A�j���[�V����
				if (g_aEnemy[nCntEnemy].bDirection == false)
				{
				pVtx[0].tex = D3DXVECTOR2(0.0f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.5f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.5f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 1.0f);
				}
				else
				{
					pVtx[1].tex = D3DXVECTOR2(0.0f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 0.0f);
					pVtx[0].tex = D3DXVECTOR2(0.5f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 0.0f);
					pVtx[3].tex = D3DXVECTOR2(0.0f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 1.0f);
					pVtx[2].tex = D3DXVECTOR2(0.5f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 1.0f);
				}

				if (pPlayer->pos.x >= g_aEnemy[nCntEnemy].pos.x - 400.0f && pPlayer->pos.x <= g_aEnemy[nCntEnemy].pos.x + 400.0f &&
					pPlayer->pos.y - pPlayer->fHeight / 2 >= g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].fHeight &&
					pPlayer->pos.y - pPlayer->fHeight / 2 <= g_aEnemy[nCntEnemy].pos.y)
				{//�v���C���[���߂��ɗ�����s���J�n

					if (g_aEnemy[nCntEnemy].bDirection == false)
					{//���������Ă���
						g_aEnemy[nCntEnemy].move.x = 3.0f;
					}
					else
					{//�E�������Ă���
						g_aEnemy[nCntEnemy].move.x = -3.0f;
					}

				}
				else
				{
					g_aEnemy[nCntEnemy].move.x = 0.0f;
				}

				if (pPlayer->pos.x >= g_aEnemy[nCntEnemy].pos.x - 200.0f && pPlayer->pos.x <= g_aEnemy[nCntEnemy].pos.x + 200.0f &&
					pPlayer->pos.y - pPlayer->fHeight / 2 >= g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].fHeight - 30.0f &&
					pPlayer->pos.y - pPlayer->fHeight / 2 <= g_aEnemy[nCntEnemy].pos.y + 300.0f)
				{//�v���C���[�ɐڋ߂�����U�����J�n
					if (g_nDelay % 60 == 0)
					{
						SetAttack(g_aEnemy[nCntEnemy].pos,
							D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							ATTACKTYPE_SLASH_E,
							-1, nCntEnemy, -1, MOVETYPE_NORMAL, 60.0f, 80.0f, 20, 0.0f, -1);
					}
				}

				if (pPlayer->pos.x >= g_aEnemy[nCntEnemy].pos.x - 20.0f && pPlayer->pos.x <= g_aEnemy[nCntEnemy].pos.x + 20.0f &&
					pPlayer->pos.y - pPlayer->fHeight / 2 >= g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].fHeight - 20.0f &&
					pPlayer->pos.y - pPlayer->fHeight / 2 <= g_aEnemy[nCntEnemy].pos.y + 20.0f)
				{//���ȏ�ڋ߂����瓮�����~�߂�
					g_aEnemy[nCntEnemy].move.x = 0.0f;
				}
				
			}
			if (g_aEnemy[nCntEnemy].bUse == true && g_aEnemy[nCntEnemy].nType == ENEMYTYPE3_ICESLIME)
			{//�G�l�~�[�R�̍U��
				//=================================================================
				//�A�j���[�V��������
				//=================================================================
				g_aEnemy[nCntEnemy].nCntAnim++;//�A�j���[�V�����J�E���^�[
				if (g_aEnemy[nCntEnemy].nCntAnim % 10 == 0)
				{//�A�j���[�V�����̑��x
					g_aEnemy[nCntEnemy].nPatternAnim++;
				}

				if (g_aEnemy[nCntEnemy].nPatternAnim >= 2)
				{//�A�j���[�V�������ŏ��ɖ߂�
					g_aEnemy[nCntEnemy].nPatternAnim = 0;
				}

				//�e�N�X�`���A�j���[�V����
				if (g_aEnemy[nCntEnemy].bDirection == false)
				{//�E�������Ă���
					pVtx[0].tex = D3DXVECTOR2(0.0f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(0.5f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(0.0f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(0.5f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 1.0f);
				}
				else
				{//���������Ă���
					pVtx[1].tex = D3DXVECTOR2(0.0f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 0.0f);
					pVtx[0].tex = D3DXVECTOR2(0.5f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 0.0f);
					pVtx[3].tex = D3DXVECTOR2(0.0f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 1.0f);
					pVtx[2].tex = D3DXVECTOR2(0.5f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 1.0f);
				}
				//============================================================================================================

				//==============================================================
				//�U������
				//==============================================================
				
				if (pPlayer->pos.x <= g_aEnemy[nCntEnemy].pos.x - 351.0f || pPlayer->pos.x >= g_aEnemy[nCntEnemy].pos.x + 351.0f ||
					pPlayer->pos.y - pPlayer->fHeight / 2 <= g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].fHeight - 251.0f||
					pPlayer->pos.y - pPlayer->fHeight / 2 >= g_aEnemy[nCntEnemy].pos.y + 251.0f)
				{
					int nCntBullet_E4;
					float fCircleBullet_E4_normal[4];
					float fRotBullet_E4;
					fRotBullet_E4 = float((rand)() % 200) / 100.0f;

					PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_004);

					if (g_nDelay % 60 == 0)
					{
						for (nCntBullet_E4 = 0; nCntBullet_E4 < 4; nCntBullet_E4++)

						{
							fCircleBullet_E4_normal[nCntBullet_E4] = (1.0f / 2.0f) * nCntBullet_E4;

							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x,g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].fHeight / 2, 0.0f),
								D3DXVECTOR3((sinf(-D3DX_PI * (fCircleBullet_E4_normal[nCntBullet_E4] + fRotBullet_E4)) * 4.0f), (cosf(-D3DX_PI * (fCircleBullet_E4_normal[nCntBullet_E4] + fRotBullet_E4)) * 4.0f), 0.0f),
								GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1,true, MOVETYPE_NORMAL);

						}
					}
				}
				else if (pPlayer->pos.x >= g_aEnemy[nCntEnemy].pos.x - 250.0f && pPlayer->pos.x <= g_aEnemy[nCntEnemy].pos.x + 250.0f &&
					pPlayer->pos.y - pPlayer->fHeight / 2 >= g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].fHeight - 120.0f &&
					pPlayer->pos.y - pPlayer->fHeight / 2 <= g_aEnemy[nCntEnemy].pos.y + 120.0f)
				{//�v���C���[���߂��ɗ�����s���J�n(�G�̒��S���W����v�Z�j

					if (g_aEnemy[nCntEnemy].bDirection == false)
					{//���������Ă���
						g_aEnemy[nCntEnemy].move.x = 1.0f;
					}
					else
					{//�E�������Ă���
						g_aEnemy[nCntEnemy].move.x = -1.0f;
					}

					if (g_nDelay % 90 == 0)
					{
						SetAttack(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y + g_aEnemy[nCntEnemy].fHeight,0.0f),
							D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							ATTACKTYPE_ICE_EXPLOSION,
							-1, nCntEnemy, -1,MOVETYPE_NORMAL,250.0f,250.0f,28, 0.0f,-1);
					}
				}
				else
				{
					g_aEnemy[nCntEnemy].move.x = 0.0f;
				}

			
				//=====================================================================================================================================

			}

			if (g_aEnemy[nCntEnemy].bUse == true && g_aEnemy[nCntEnemy].nType == ENEMYTYPE4_BILIBILISLIME)
			{//�G�l�~�[�S�̍U��
				int nCntBullet_E5;
				float fCircleBullet_E5_normal[12];
				///*if (g_nDelay % 60 == 0)*/
				//{
				//	//PlaySound(SOUND_LABEL_SE_ARROWSHOT_000);
				//	//for (nCntBullet_E5 = 0; nCntBullet_E5 < 8; nCntBullet_E5++)
				//	//{
				//	//	SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_REAFY, nCntBullet_E5, nCntEnemy, -1,false);//�G�̒e������G���m�œ�����Ȃ�

				//	//}
				//}

				//=================================================================
				//�A�j���[�V��������
				//=================================================================
				g_aEnemy[nCntEnemy].nCntAnim++;//�A�j���[�V�����J�E���^�[
				if (g_aEnemy[nCntEnemy].nCntAnim % 10 == 0)
				{//�A�j���[�V�����̑��x
					g_aEnemy[nCntEnemy].nPatternAnim++;
				}

				if (g_aEnemy[nCntEnemy].nPatternAnim >= 2)
				{//�A�j���[�V�������ŏ��ɖ߂�
					g_aEnemy[nCntEnemy].nPatternAnim = 0;
				}

				//�e�N�X�`���A�j���[�V����
				if (g_aEnemy[nCntEnemy].bDirection == false)
				{//�E�������Ă���
					pVtx[0].tex = D3DXVECTOR2(0.0f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(0.5f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(0.0f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(0.5f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 1.0f);
				}
				else
				{//���������Ă���
					pVtx[1].tex = D3DXVECTOR2(0.0f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 0.0f);
					pVtx[0].tex = D3DXVECTOR2(0.5f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 0.0f);
					pVtx[3].tex = D3DXVECTOR2(0.0f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 1.0f);
					pVtx[2].tex = D3DXVECTOR2(0.5f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 1.0f);
				}

				//==============================================================
				//�U������
				//==============================================================

				/*if (pPlayer->pos.x <= g_aEnemy[nCntEnemy].pos.x - 351.0f || pPlayer->pos.x >= g_aEnemy[nCntEnemy].pos.x + 351.0f ||
					pPlayer->pos.y - pPlayer->fHeight / 2 <= g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].fHeight - 251.0f ||
					pPlayer->pos.y - pPlayer->fHeight / 2 >= g_aEnemy[nCntEnemy].pos.y + 251.0f)
				{
					int nCntBullet_E4;
					float fCircleBullet_E4_normal[4];
					float fRotBullet_E4;
					fRotBullet_E4 = float((rand)() % 200) / 100.0f;

					PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_004);

					if (g_nDelay % 60 == 0)
					{
						for (nCntBullet_E4 = 0; nCntBullet_E4 < 4; nCntBullet_E4++)

						{
							fCircleBullet_E4_normal[nCntBullet_E4] = (1.0f / 2.0f) * nCntBullet_E4;

							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].fHeight / 2, 0.0f),
								D3DXVECTOR3((sinf(-D3DX_PI * (fCircleBullet_E4_normal[nCntBullet_E4] + fRotBullet_E4)) * 4.0f), (cosf(-D3DX_PI * (fCircleBullet_E4_normal[nCntBullet_E4] + fRotBullet_E4)) * 4.0f), 0.0f),
								GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1, true);

						}
					}
				}*/
			   if (pPlayer->pos.x >= g_aEnemy[nCntEnemy].pos.x - 200.0f && pPlayer->pos.x <= g_aEnemy[nCntEnemy].pos.x + 200.0f &&
					pPlayer->pos.y - pPlayer->fHeight / 2 >= g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].fHeight - 1000.0f &&
					pPlayer->pos.y - pPlayer->fHeight / 2 <= g_aEnemy[nCntEnemy].pos.y + 1000.0f)
				{//�v���C���[���߂��ɗ�����s���J�n(�G�̒��S���W����v�Z�j

					if (g_aEnemy[nCntEnemy].bDirection == false)
					{//���������Ă���
						g_aEnemy[nCntEnemy].nMoveTime++;
						g_aEnemy[nCntEnemy].move.x = 1.5f;
					}
					else
					{//�E�������Ă���
						g_aEnemy[nCntEnemy].nMoveTime++;
						g_aEnemy[nCntEnemy].move.x = -1.5f;
					}

					

					if (g_nDelay % 50 == 0)
					{
						SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy,D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x,g_aEnemy[nCntEnemy].pos.y + 900.0f,0.0f),
							100, ATTACKTYPE_THUNDER_E4, D3DX_PI * 0.0f, 64.0f, 0.0f, 80.0f, 1800.0f, 20);
					}
				}
				else
				{
				   g_aEnemy[nCntEnemy].nMoveTime = 0;
					g_aEnemy[nCntEnemy].move.x = 0.0f;
				}

			   /*if (g_aEnemy[nCntEnemy].nMoveTime >= 1 && g_aEnemy[nCntEnemy].nMoveTime <= 2)
			   {
				   g_aEnemy[nCntEnemy].move.y = -20.0f;
			   }*/

			}

			if (g_aEnemy[nCntEnemy].bUse == true && g_aEnemy[nCntEnemy].nType == ENEMYTYPE5_SNIPERSLIME)
			{//�G�l�~�[�T�̍U��
				//g_fXVaim[nCntEnemy] = pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x;//X���W�ւ̃x�N�g��
				//g_fYVaim[nCntEnemy] = pPlayer->pos.y - g_aEnemy[nCntEnemy].pos.y;//Y���W�ւ̃x�N�g��
				//g_Vaim[nCntEnemy] = atan2(g_fXVaim[nCntEnemy], g_fYVaim[nCntEnemy]);//�e���������m��

				//if (g_nDelay % 60 == 0)
				//{
				//	PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_003);
				//	SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((D3DX_PI * g_fXVaim[nCntEnemy]) * 0.004f, (D3DX_PI * g_fYVaim[nCntEnemy]) * 0.004f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1,false);//�G�̒e������G���m�œ�����Ȃ�	
				//}



				g_fXVaim[nCntEnemy] = pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x;//X���W�ւ̃x�N�g��
				g_fYVaim[nCntEnemy] = pPlayer->pos.y - g_aEnemy[nCntEnemy].pos.y;//Y���W�ւ̃x�N�g��
				g_Vaim[nCntEnemy] = atan2(g_fXVaim[nCntEnemy], g_fYVaim[nCntEnemy]);//�e���������̌���


				g_aEnemy[nCntEnemy].nCntAnim++;//�A�j���[�V�����J�E���^�[

				if (g_aEnemy[nCntEnemy].nCntAnim % 4 == 0)
				{//�A�j���[�V�����̑��x
					g_aEnemy[nCntEnemy].nPatternAnim++;
				}

				if (g_aEnemy[nCntEnemy].nPatternAnim >= 2)
				{//�A�j���[�V�������ŏ��ɖ߂�
					g_aEnemy[nCntEnemy].nPatternAnim = 0;
				}

				//�e�N�X�`���A�j���[�V����
				if (g_aEnemy[nCntEnemy].bDirection == false)
				{
					pVtx[0].tex = D3DXVECTOR2(0.0f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(0.5f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(0.0f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(0.5f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 1.0f);
				}
				else
				{
					pVtx[1].tex = D3DXVECTOR2(0.0f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 0.0f);
					pVtx[0].tex = D3DXVECTOR2(0.5f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 0.0f);
					pVtx[3].tex = D3DXVECTOR2(0.0f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 1.0f);
					pVtx[2].tex = D3DXVECTOR2(0.5f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 1.0f);
				}


			
				
				

					if (g_nDelay % 120 == 0)
					{
						PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_007);
						SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy]) * 3.0f, cosf(g_Vaim[nCntEnemy]) * 3.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1, false, MOVETYPE_NORMAL);
						SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy] + 0.5f) * 3.0f, cosf(g_Vaim[nCntEnemy] + 0.5f) * 3.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1, false, MOVETYPE_NORMAL);
						SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy] - 0.5f) * 3.0f, cosf(g_Vaim[nCntEnemy] - 0.5f) * 3.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1, false, MOVETYPE_NORMAL);
					}
				

			}

			if (g_aEnemy[nCntEnemy].bUse == true && g_aEnemy[nCntEnemy].nType == ENEMYTYPE6_BOMUSLIME)
			{//�G�l�~�[�U�̍U��

				//if (g_nDelay % 60 == 0)
				//{
				//	PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_000);
				//	SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * 0.0f + g_fRolingBulletX_E7) * 5.0f), (cosf(-D3DX_PI * 0.0f + g_fRolingBulletY_E7) * 5.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1,false);//�G�̒e������G���m�œ�����Ȃ�
				//	SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * 1.0f + g_fRolingBulletX_E7) * 5.0f), (cosf(-D3DX_PI * 1.0f + g_fRolingBulletY_E7) * 5.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1,false);//�G�̒e������G���m�œ�����Ȃ�
				//	SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * 0.0f - g_fRolingBulletX_E7) * 5.0f), (cosf(-D3DX_PI * 0.0f - g_fRolingBulletY_E7) * 5.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1,false);//�G�̒e������G���m�œ�����Ȃ�
				//	SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * 1.0f - g_fRolingBulletX_E7) * 5.0f), (cosf(-D3DX_PI * 1.0f - g_fRolingBulletY_E7) * 5.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1,false);//�G�̒e������G���m�œ�����Ȃ�
				//}

			


				g_aEnemy[nCntEnemy].nCntAnim++;//�A�j���[�V�����J�E���^�[

				if (g_aEnemy[nCntEnemy].nCntAnim % 4 == 0)
				{//�A�j���[�V�����̑��x
					g_aEnemy[nCntEnemy].nPatternAnim++;
				}

				if (g_aEnemy[nCntEnemy].nPatternAnim >= 2)
				{//�A�j���[�V�������ŏ��ɖ߂�
					g_aEnemy[nCntEnemy].nPatternAnim = 0;
				}

				//�e�N�X�`���A�j���[�V����
				if (g_aEnemy[nCntEnemy].bDirection == false)
				{//�E�������Ă���
					pVtx[0].tex = D3DXVECTOR2(0.0f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(0.5f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(0.0f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(0.5f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 1.0f);
				}
				else
				{//���������Ă���
					pVtx[1].tex = D3DXVECTOR2(0.0f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 0.0f);
					pVtx[0].tex = D3DXVECTOR2(0.5f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 0.0f);
					pVtx[3].tex = D3DXVECTOR2(0.0f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 1.0f);
					pVtx[2].tex = D3DXVECTOR2(0.5f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 1.0f);
				}

				if (pPlayer->move.y >= 5)
				{
					g_aEnemy[nCntEnemy].move.y = -10;
				}

				//==============================================================
				//�U������
				//==============================================================
			

				if (pPlayer->pos.x <= g_aEnemy[nCntEnemy].pos.x - 50.0f || pPlayer->pos.x >= g_aEnemy[nCntEnemy].pos.x + 50.0f ||
					pPlayer->pos.y - pPlayer->fHeight / 2 <= g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].fHeight - 50.0f ||
					pPlayer->pos.y - pPlayer->fHeight / 2 >= g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].fHeight + 50.0f)
				{//�v���C���[���߂��ɗ�����s���J�n(�G�̒��S���W����v�Z�j

					if (g_aEnemy[nCntEnemy].bDirection == false)
					{//�E�������Ă���
						float fRotBullet_E6_R = float((rand)() % 100 + 100) / 100.0f;
						if (g_nDelay % 80 == 0)
						{
							SetAttack(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].fHeight / 2,0.0f),
								D3DXVECTOR3(sinf(-D3DX_PI * fRotBullet_E6_R) * 8.0f, cosf(-D3DX_PI* fRotBullet_E6_R) * 8.0f,0.0f),
								ATTACKTYPE_BOMU,
								-1, nCntEnemy, -1, MOVETYPE_GRAVITY,140.0f, 140.0f, 200, 0.0f, -1);
						}
					}
					else
					{//���������Ă���
						if (g_nDelay % 80 == 0)
						{
							float fRotBullet_E6_L = float((rand)() % 100) / 100.0f;
						
								SetAttack(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].fHeight / 2, 0.0f),
									D3DXVECTOR3(sinf(-D3DX_PI * fRotBullet_E6_L) * 8.0f, cosf(-D3DX_PI * fRotBullet_E6_L) * 8.0f, 0.0f),
									ATTACKTYPE_BOMU,
									-1, nCntEnemy, -1, MOVETYPE_GRAVITY,140.0f,140.0f,200, 0.0f, -1);
							
						}
					
					}

				
				}
				


				//=====================================================================================================================================

			}

			if (g_aEnemy[nCntEnemy].bUse == true && g_aEnemy[nCntEnemy].nType == ENEMYTYPE7_FLOWERSLIME)
			{//�G�l�~�[�V���g���Ă�����E�E�E
				//g_rotPI.z += 0.1f;

				//if (g_nDelay % 9 == 0)
				//{
				//	g_rotPI.z += 0.019f;
				//}

				//if (g_nDelay % 60 == 0)
				//{
				//	PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_006);
				//	SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * g_rotPI.z) * g_fSpeedBullet_E9), (cosf(-D3DX_PI * g_rotPI.z) * g_fSpeedBullet_E9), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY9, 0, nCntEnemy, -1,false);//
				//}

				g_aEnemy[nCntEnemy].nCntAnim++;//�A�j���[�V�����J�E���^�[

				if (g_aEnemy[nCntEnemy].nCntAnim % 4 == 0)
				{//�A�j���[�V�����̑��x
					g_aEnemy[nCntEnemy].nPatternAnim++;
				}

				if (g_aEnemy[nCntEnemy].nPatternAnim >= 2)
				{//�A�j���[�V�������ŏ��ɖ߂�
					g_aEnemy[nCntEnemy].nPatternAnim = 0;
				}

				//�e�N�X�`���A�j���[�V����
				if (g_aEnemy[nCntEnemy].bDirection == false)
				{//�E�������Ă���
					pVtx[0].tex = D3DXVECTOR2(0.0f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(0.5f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(0.0f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(0.5f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 1.0f);
				}
				else
				{//���������Ă���
					pVtx[1].tex = D3DXVECTOR2(0.0f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 0.0f);
					pVtx[0].tex = D3DXVECTOR2(0.5f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 0.0f);
					pVtx[3].tex = D3DXVECTOR2(0.0f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 1.0f);
					pVtx[2].tex = D3DXVECTOR2(0.5f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 1.0f);
				}

				

				//===============================================
				//�U������
				//===============================================
				//if (pPlayer->pos.x >= g_aEnemy[nCntEnemy].pos.x - 400.0f && pPlayer->pos.x <= g_aEnemy[nCntEnemy].pos.x + 400.0f &&
				//	pPlayer->pos.y - pPlayer->fHeight / 2 >= g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].fHeight &&
				//	pPlayer->pos.y - pPlayer->fHeight / 2 <= g_aEnemy[nCntEnemy].pos.y)
				//{//�v���C���[���߂��ɗ�����s���J�n

				//	//if (g_aEnemy[nCntEnemy].bDirection == false)
				//	//{//���������Ă���
				//	//	g_aEnemy[nCntEnemy].move.x = 3.0f;
				//	//}
				//	//else
				//	//{//�E�������Ă���
				//	//	g_aEnemy[nCntEnemy].move.x = -3.0f;
				//	//}

				//}
				if (pPlayer->pos.x >= g_aEnemy[nCntEnemy].pos.x - 200.0f && pPlayer->pos.x <= g_aEnemy[nCntEnemy].pos.x + 200.0f &&
					pPlayer->pos.y - pPlayer->fHeight / 2 >= g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].fHeight - 150.0f &&
					pPlayer->pos.y - pPlayer->fHeight / 2 <= g_aEnemy[nCntEnemy].pos.y + 150.0f)
				{//�v���C���[�ɐڋ߂�����U�����J�n�i�v���C���[�̒��S�_���ǂ��ɂ��邩�j

					if (g_nDelay % 60 == 0)
					{

						//SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy,D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 150.0f,g_aEnemy[nCntEnemy].pos.y,0.0f), 30, ATTACKTYPE_NEEDLE, 0.0f, -1, -1,
						//	-1, -1, 60);

						//SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 150.0f, g_aEnemy[nCntEnemy].pos.y, 0.0f),30, ATTACKTYPE_NEEDLE, 0.0f, -1, -1,
						//	-1, -1, 60);


						SetAttack(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 150.0f, g_aEnemy[nCntEnemy].pos.y, 0.0f),
							D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							ATTACKTYPE_NEEDLE,
							-1, nCntEnemy, -1, MOVETYPE_NORMAL,100.0f,100.0f,200, 0.0f, -1);

						SetAttack(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 150.0f, g_aEnemy[nCntEnemy].pos.y, 0.0f),
							D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							ATTACKTYPE_NEEDLE,
							-1, nCntEnemy, -1, MOVETYPE_NORMAL, 100.0f, 100.0f, 200, 0.0f, -1);
					}
				}
				//========================================================================================================================

			}
			else if (g_aEnemy[nCntEnemy].bUse == true && g_aEnemy[nCntEnemy].nType == ENEMYTYPE8_CATSLIME)
			{//�G�l�~�[�R�̍U��
				//=================================================================
				//�A�j���[�V��������
				//=================================================================
				g_aEnemy[nCntEnemy].nCntAnim++;//�A�j���[�V�����J�E���^�[
				if (g_aEnemy[nCntEnemy].nCntAnim % 7 == 0)
				{//�A�j���[�V�����̑��x
					g_aEnemy[nCntEnemy].nPatternAnim++;
				}

				if (g_aEnemy[nCntEnemy].nPatternAnim >= 2)
				{//�A�j���[�V�������ŏ��ɖ߂�
					g_aEnemy[nCntEnemy].nPatternAnim = 0;
				}

				//�e�N�X�`���A�j���[�V����
				if (g_aEnemy[nCntEnemy].bDirection == false)
				{//�E�������Ă���
					pVtx[0].tex = D3DXVECTOR2(0.0f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(0.5f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(0.0f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(0.5f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 1.0f);
				}
				else
				{//���������Ă���
					pVtx[1].tex = D3DXVECTOR2(0.0f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 0.0f);
					pVtx[0].tex = D3DXVECTOR2(0.5f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 0.0f);
					pVtx[3].tex = D3DXVECTOR2(0.0f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 1.0f);
					pVtx[2].tex = D3DXVECTOR2(0.5f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 1.0f);
				}
				//============================================================================================================

				//==============================================================
				//�U������
				//==============================================================
				if (pPlayer->pos.x >= g_aEnemy[nCntEnemy].pos.x - 350.0f && pPlayer->pos.x <= g_aEnemy[nCntEnemy].pos.x + 350.0f &&
					pPlayer->pos.y - pPlayer->fHeight / 2 >= g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].fHeight - 100.0f &&
					pPlayer->pos.y - pPlayer->fHeight / 2 <= g_aEnemy[nCntEnemy].pos.y + 100.0f)
				{//�v���C���[���߂��ɗ�����s���J�n(�G�̒��S���W����v�Z�j

					//if (g_aEnemy[nCntEnemy].bDirection == false)
					//{//���������Ă���
					//	g_aEnemy[nCntEnemy].move.x = 1.0f;
					//}
					//else
					//{//�E�������Ă���
					//	g_aEnemy[nCntEnemy].move.x = -1.0f;
					//}

					if (g_nDelay % 210 == 0)
					{
						SetAttack(g_aEnemy[nCntEnemy].pos,
							D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							ATTACKTYPE_CLAW,
							-1, nCntEnemy, -1, MOVETYPE_NORMAL,60.0f,60.0f,100, 0.0f, -1);
					}
				}
				


				//=====================================================================================================================================

			}

			if (g_aEnemy[nCntEnemy].bUse == true && g_aEnemy[nCntEnemy].nType == ENEMYTYPE9_STONESLIME)
			{//�G�l�~�[�X���g���Ă�����
				int nCircleBullet[4];
				//if (g_nDelay % 50 == 0)
				//{
				//	PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_003);
				//	for (nCntBullet2 = 0; nCntBullet2 < MAX_CIRCLEBULLET_E10; nCntBullet2++)
				//	{


				//		g_fCircleBullet[nCntBullet2] = (1.0f / 4.0f) * nCntBullet2;

				//		if (nCntBullet2 % 2 == 0)
				//		{
				//			SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * g_fCircleBullet[nCntBullet2] + g_fRandumResultBullet2) * 1.0f), (cosf(-D3DX_PI * g_fCircleBullet[nCntBullet2] + g_fRandumResultBullet2) * 1.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BOLBY, 0, nCntEnemy, -1, false);//�G�̒e������G���m�œ�����Ȃ�		}
				//		}
				//		else
				//		{
				//			//SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * g_fCircleBullet[nCntBullet2] + g_fRandumResultBullet2) * 3.0f), (cosf(-D3DX_PI * g_fCircleBullet[nCntBullet2] + g_fRandumResultBullet2) * 3.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BOLBY, 0, nCntEnemy, -1);
				//		}
				//	}
				//}

				//=================================================================
				//�A�j���[�V��������
				//=================================================================
				g_aEnemy[nCntEnemy].nCntAnim++;//�A�j���[�V�����J�E���^�[
				if (g_aEnemy[nCntEnemy].nCntAnim % 7 == 0)
				{//�A�j���[�V�����̑��x
					g_aEnemy[nCntEnemy].nPatternAnim++;
				}

				if (g_aEnemy[nCntEnemy].nPatternAnim >= 2)
				{//�A�j���[�V�������ŏ��ɖ߂�
					g_aEnemy[nCntEnemy].nPatternAnim = 0;
				}

				//�e�N�X�`���A�j���[�V����
				if (g_aEnemy[nCntEnemy].bDirection == false)
				{//�E�������Ă���
					pVtx[0].tex = D3DXVECTOR2(0.0f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(0.5f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(0.0f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(0.5f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 1.0f);
				}
				else
				{//���������Ă���
					pVtx[1].tex = D3DXVECTOR2(0.0f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 0.0f);
					pVtx[0].tex = D3DXVECTOR2(0.5f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 0.0f);
					pVtx[3].tex = D3DXVECTOR2(0.0f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 1.0f);
					pVtx[2].tex = D3DXVECTOR2(0.5f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 1.0f);
				}

				

			//==============================================================
			//�U������
			//==============================================================
				
				if (pPlayer->pos.x <= g_aEnemy[nCntEnemy].pos.x - 251.0f || pPlayer->pos.x >= g_aEnemy[nCntEnemy].pos.x + 251.0f ||
					pPlayer->pos.y - pPlayer->fHeight / 2 <= g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].fHeight - 151.0f ||
					pPlayer->pos.y - pPlayer->fHeight / 2 >= g_aEnemy[nCntEnemy].pos.y + 151.0f)
				{//�v���C���[�̒��S�_���ǂ͈͓̔��ɂ��邩
					
					int nCntBullet_E4;
					float fCircleBullet_E4_normal[4];
					float fRotBullet_E4;
					fRotBullet_E4 = float((rand)() % 200) / 100.0f;

					PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_004);

					if (g_nDelay % 60 == 0)
					{
						for (nCntBullet_E4 = 0; nCntBullet_E4 < 4; nCntBullet_E4++)

						{
							fCircleBullet_E4_normal[nCntBullet_E4] = (1.0f / 2.0f) * nCntBullet_E4;

							
								PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_000);
								SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * g_fRandumResultBullet) * 7.0f), (cosf(-D3DX_PI * g_fRandumResultBullet) * 7.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1, false, MOVETYPE_NORMAL);//�G�̒e������G���m�œ�����Ȃ�
							
						}
					}
				}
				else if (pPlayer->pos.x >= g_aEnemy[nCntEnemy].pos.x - 250.0f && pPlayer->pos.x <= g_aEnemy[nCntEnemy].pos.x + 250.0f &&
					pPlayer->pos.y - pPlayer->fHeight / 2 >= g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].fHeight - 120.0f &&
					pPlayer->pos.y - pPlayer->fHeight / 2 <= g_aEnemy[nCntEnemy].pos.y + 120.0f)
				{//�v���C���[���߂��ɗ�����s���J�n(�G�̒��S���W����v�Z�j
					g_aEnemy[nCntEnemy].nCntJumpTime++;
					if (g_aEnemy[nCntEnemy].bDirection == false)
					{//���������Ă���
						g_aEnemy[nCntEnemy].move.x = 12.0f;
					}
					else
					{//�E�������Ă���
						g_aEnemy[nCntEnemy].move.x = -12.0f;
					}
					


					if (g_aEnemy[nCntEnemy].bIsLanding == true)
					{
						g_aEnemy[nCntEnemy].move.y = -10.0f;
					}
			

					float fRandFall;//�ǂ��̈ʒu�ɗ��Ƃ����������_���Ō��߂�
					float fRandRadius;//�ǂ��̈ʒu�ɗ��Ƃ����������_���Ō��߂�
					float fResultPos;//���ʓI�ɂǂ��ɗ��Ƃ���
					fRandFall = float((rand)() % 300 + 1);
					fRandRadius = float((rand)() % 2 + 1);
					
					if (fRandRadius == 1)
					{
						fResultPos = -fRandFall;
					}
					else
					{
						fResultPos = fRandFall;
					}

					if (g_nDelay % 60 == 0)
					{
						SetAttack(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + fResultPos, g_aEnemy[nCntEnemy].pos.y - 500.0f, 0.0f),
							D3DXVECTOR3(0.0f, 10.0f, 0.0f),
							ATTACKTYPE_FALLINGBLOCK,
							-1, nCntEnemy, -1, MOVETYPE_NORMAL,100.0f,100.0f,200, 0.0f, -1);
					}
				}
				else
				{
					g_aEnemy[nCntEnemy].move.x = 0.0f;
				}

				


				
				
				//============================================================================================================
			} 

			if (g_aEnemy[nCntEnemy].bUse == true && g_aEnemy[nCntEnemy].nType == ENEMYTYPE10_WITCHSLIME)
			{//�G�l�~�[�P�O���g���Ă�����E�E�E



				//g_fXVaim[nCntEnemy] = pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x;//X���W�ւ̃x�N�g��
				//g_fYVaim[nCntEnemy] = pPlayer->pos.y - g_aEnemy[nCntEnemy].pos.y;//Y���W�ւ̃x�N�g��
				//g_Vaim[nCntEnemy] = atan2(g_fXVaim[nCntEnemy], g_fYVaim[nCntEnemy]);//�e���������̌���

				//if (g_nDelay % 60 == 0)
				//{
				//	PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_007);
				//	SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy]) * 3.0f, cosf(g_Vaim[nCntEnemy]) * 3.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1, false);
				//	SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy] + 0.4f) * 3.0f, cosf(g_Vaim[nCntEnemy] + 0.4f) * 3.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1, false);
				//	SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy] - 0.4f) * 3.0f, cosf(g_Vaim[nCntEnemy] - 0.4f) * 3.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1, false);
				//}


				//=================================================================
				//�A�j���[�V��������
				//=================================================================
				g_aEnemy[nCntEnemy].nCntAnim++;//�A�j���[�V�����J�E���^�[
				if (g_aEnemy[nCntEnemy].nCntAnim % 7 == 0)
				{//�A�j���[�V�����̑��x
					g_aEnemy[nCntEnemy].nPatternAnim++;
				}

				if (g_aEnemy[nCntEnemy].nPatternAnim >= 2)
				{//�A�j���[�V�������ŏ��ɖ߂�
					g_aEnemy[nCntEnemy].nPatternAnim = 0;
				}

				//�e�N�X�`���A�j���[�V����
				if (g_aEnemy[nCntEnemy].bDirection == false)
				{//�E�������Ă���
					pVtx[0].tex = D3DXVECTOR2(0.0f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(0.5f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(0.0f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(0.5f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 1.0f);
				}
				else
				{//���������Ă���
					pVtx[1].tex = D3DXVECTOR2(0.0f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 0.0f);
					pVtx[0].tex = D3DXVECTOR2(0.5f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 0.0f);
					pVtx[3].tex = D3DXVECTOR2(0.0f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 1.0f);
					pVtx[2].tex = D3DXVECTOR2(0.5f + 0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 1.0f);
				}

				if (pPlayer->pos.x >= g_aEnemy[nCntEnemy].pos.x - 1280.0f && pPlayer->pos.x <= g_aEnemy[nCntEnemy].pos.x + 1280.0f &&
					pPlayer->pos.y - pPlayer->fHeight / 2 >= g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].fHeight - 120.0f &&
					pPlayer->pos.y - pPlayer->fHeight / 2 <= g_aEnemy[nCntEnemy].pos.y + 120.0f)
				{//�v���C���[���߂��ɗ�����s���J�n(�G�̒��S���W����v�Z�j
				

				
					if (g_aEnemy[nCntEnemy].bDirection == false)
					{//�E�������Ă���
						if (g_nDelay % 100 == 0)
						{
							
						

							SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, g_aEnemy[nCntEnemy].pos,
								80, ATTACKTYPE_BEAM, -D3DX_PI * 0.5f, 50.0f, 0.0f, 50.0f, 1280.0f, 15);
						}
					}
					if (g_aEnemy[nCntEnemy].bDirection == true)
					{//�E�������Ă���
						if (g_nDelay % 100 == 0)
						{
							SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, g_aEnemy[nCntEnemy].pos,
								80, ATTACKTYPE_BEAM, D3DX_PI * 0.5f, 50.0f, 0.0f, 50.0f, 1280.0f, 15);
						}
					}
				}
				//============================================================================================================

			}
			//==============================================================
			//�{�X�P�̍s������
			//==============================================================
			if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS1_STAND ||
				g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS1_RUN || 
				g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS1_ATTACK)
			{//�{�X���g�p���ꂽ��


			
				g_aEnemy[nCntEnemy].nCountMoveInterval++;
				g_aEnemy[nCntEnemy].nApperCount++;//�G���o�����Ă���̎��Ԃ��J�E���g����B

				if (g_aEnemy[nCntEnemy].nCountMoveInterval == 1)
				{
					StopSound(SOUND_LABEL_BGM006);
					PlaySound(SOUND_LABEL_BGM004);
					SetGage(nCntEnemy, GAGETYPE_BOSS1HP);
					SetGage(-1, GAGETYPE_BOSSHP_FRAME);
				}

				//================================
				//�`�ԕω�����
				//================================
				if (g_aEnemy[nCntEnemy].fLife <= g_aEnemy[nCntEnemy].fMaxHp / 4)
				{//�c��HP���ő�HP�̂S���̂P�ɂȂ�����
					if (g_aEnemy[nCntEnemy].bSpecialStaging == false)
					{
						PlaySound(SOUND_LABEL_SE_SPECIALATTACK);
						g_aEnemy[nCntEnemy].bSpecialAttack = true;
						g_aEnemy[nCntEnemy].bSpecialStaging = true;
					/*	SetStaging(STAGINGTYPE_BOSS1_SPECIAL_ATTACK, -1, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 180,ATTACKTYPE_PUNCH,0.0f,-1,-1,
							-1,-1,-1);*/
						SetSpecial(SPECIALTYPE_BOSS1_SPECIAL_ATTACK, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 150);
						
						g_aEnemy[nCntEnemy].nApperCount = 0;
					}
				}
			//==============================================================
			//�{�X�P�̍s������
			//==============================================================


				//=============
				//�p�^�[���P
				//=============
				if (g_aEnemy[nCntEnemy].bSpecialAttack == false)
				{
					g_fXVaim[nCntEnemy] = pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x;//X���W�ւ̃x�N�g��
					g_fYVaim[nCntEnemy] = pPlayer->pos.y - g_aEnemy[nCntEnemy].pos.y;//Y���W�ւ̃x�N�g��
					g_Vaim[nCntEnemy] = atan2(g_fXVaim[nCntEnemy], g_fYVaim[nCntEnemy]);//�e���������̌���

					if (g_aEnemy[nCntEnemy].nApperCount >= 0 && g_aEnemy[nCntEnemy].nApperCount <= 300)
					{
						g_aEnemy[nCntEnemy].nType = ENEMYTYPE_BOSS1_RUN;
						if (g_aEnemy[nCntEnemy].bDirection == false)
						{//���������Ă���
							g_aEnemy[nCntEnemy].move.x = 5.0f;
						}
						else
						{//�E�������Ă���
							g_aEnemy[nCntEnemy].move.x = -5.0f;
						}

						if (g_nDelay % 60 == 0)
						{
							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_007);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy]) * 10.0f, cosf(g_Vaim[nCntEnemy]) * 10.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1, false, MOVETYPE_NORMAL);
						}
					}
					//===================

					//=============
					//�p�^�[��2
					//=============
					if (g_aEnemy[nCntEnemy].nApperCount >= 301 && g_aEnemy[nCntEnemy].nApperCount <= 500)
					{
						g_aEnemy[nCntEnemy].nType = ENEMYTYPE_BOSS1_ATTACK;
						if (g_aEnemy[nCntEnemy].bDirection == false)
						{//���������Ă���
							g_aEnemy[nCntEnemy].move.x = 10.0f;
						}
						else
						{//�E�������Ă���
							g_aEnemy[nCntEnemy].move.x = -10.0f;
						}

						SetAttack(g_aEnemy[nCntEnemy].pos,
							D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							ATTACKTYPE_SLASH_E,
							-1, nCntEnemy, -1, MOVETYPE_NORMAL, 100.0f, 120.0f, 20,0.0f, -1);
					}
					//===================

					//=============
					//�p�^�[��3
					//=============
					if (g_aEnemy[nCntEnemy].nApperCount == 501)
					{
						g_aEnemy[nCntEnemy].nType = ENEMYTYPE_BOSS1_STAND;

						g_aEnemy[nCntEnemy].move.y = -30.0f;


					}

					if (g_aEnemy[nCntEnemy].nApperCount >= 502 && g_aEnemy[nCntEnemy].nApperCount <= 560)
					{
						g_aEnemy[nCntEnemy].nType = ENEMYTYPE_BOSS1_STAND;

						if (g_nDelay % 3 == 0)
						{
							SetBullet(g_aEnemy[nCntEnemy].pos,
								D3DXVECTOR3(sinf(-D3DX_PI * 0.5f) * 6.0f, cosf(-D3DX_PI * 0.5f) * 6.0f, 0.0f),
								300, BULLETTYPE_ENEMY, -1, -1, -1, true, MOVETYPE_NORMAL);

							SetBullet(g_aEnemy[nCntEnemy].pos,
								D3DXVECTOR3(sinf(-D3DX_PI * 1.5f) * 6.0f, cosf(-D3DX_PI * 1.5f) * 6.0f, 0.0f),
								300, BULLETTYPE_ENEMY, -1, -1, -1, true, MOVETYPE_NORMAL);
						}

					}
					//===================

					//=============
					//�p�^�[��4
					//=============
					if (g_aEnemy[nCntEnemy].nApperCount == 561)
					{
						g_aEnemy[nCntEnemy].nType = ENEMYTYPE_BOSS1_STAND;

						g_aEnemy[nCntEnemy].pos.x = pPlayer->pos.x;
					}


					if (g_aEnemy[nCntEnemy].nApperCount >= 562 && g_aEnemy[nCntEnemy].nApperCount <= 700)
					{
						g_aEnemy[nCntEnemy].nType = ENEMYTYPE_BOSS1_ATTACK;

						if (g_aEnemy[nCntEnemy].bIsLanding == true && g_aEnemy[nCntEnemy].nAttackCount == 0)
						{//��񂾂�����	

							SetExplosion(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), EXPLOSIONTYPE_E6, 500.0f, 500.0f);
							g_aEnemy[nCntEnemy].nAttackCount++;///�U���񐔂��J�E��
						}
					}

					//===================

					//=============
					//�p�^�[��5
					//=============

					if (g_aEnemy[nCntEnemy].nApperCount >= 801 && g_aEnemy[nCntEnemy].nApperCount <= 1200)
					{
						
						//======================================================================
						//�ǁA���̓u���b�N�ɓ��������ꍇ�ɁA�ړ�����������t�ɂ��鏈��
						//======================================================================
						if (g_aEnemy[nCntEnemy].nApperCount == 801)
						{
							g_aEnemy[nCntEnemy].nAttackCount = 0;

							if (g_aEnemy[nCntEnemy].bDirection == false)
							{//�E�������Ă���
								g_aEnemy[nCntEnemy].bMovePattern = false;

								g_fSaveSpeed = 15.0f;
							}
							else
							{//���������Ă���
								g_aEnemy[nCntEnemy].bMovePattern = true;

								g_fSaveSpeed = -15.0f;
							}
						}
						g_aEnemy[nCntEnemy].nType = ENEMYTYPE_BOSS1_RUN;

						if (g_aEnemy[nCntEnemy].bMoveWay == false)
						{
							g_aEnemy[nCntEnemy].move.x = g_fSaveSpeed;
						}
						else
						{
							g_aEnemy[nCntEnemy].move.x = -g_fSaveSpeed;
						}
						//=========
						//�U������
						//=========
						if (g_nDelay % 60 == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].fHeight / 2, 0.0f),
								D3DXVECTOR3(sinf(-D3DX_PI * 1.0f) * 23.0f, cosf(-D3DX_PI * 1.0f) * 23.0f, 0.0f),
								60, BULLETTYPE_SEED, -1, -1, -1, false, MOVETYPE_GRAVITY);
						}
						//===================
					}
					//================
					//�p�^�[���U
					//================
					if (g_aEnemy[nCntEnemy].nApperCount >= 1201 && g_aEnemy[nCntEnemy].nApperCount <= 1600)
					{
						g_aEnemy[nCntEnemy].nType = ENEMYTYPE_BOSS1_ATTACK;//���[�V�����I��

						//=============================================
						//�ړ�����
						//=============================================
						if (g_aEnemy[nCntEnemy].bDirection == false)
						{//���������Ă���
							g_aEnemy[nCntEnemy].move.x = 3.0f;
						}
						else
						{//�E�������Ă���
							g_aEnemy[nCntEnemy].move.x = -3.0f;
						}
						//==============================================================

						if (pPlayer->pos.x >= g_aEnemy[nCntEnemy].pos.x - 300.0f && pPlayer->pos.x <= g_aEnemy[nCntEnemy].pos.x + 300.0f &&
							pPlayer->pos.y - pPlayer->fHeight / 2 >= g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].fHeight - 300.0f &&
							pPlayer->pos.y - pPlayer->fHeight / 2 <= g_aEnemy[nCntEnemy].pos.y + 300.0f)
						{//�v���C���[�ɐڋ߂�����U�����J�n�i�v���C���[�̒��S�_���ǂ͈͓̔��ɂ��邩�j

							if (g_nDelay % 30 == 0)
							{
								SetAttack(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 150.0f, g_aEnemy[nCntEnemy].pos.y, 0.0f),
									D3DXVECTOR3(0.0f, 0.0f, 0.0f),
									ATTACKTYPE_NEEDLE,
									-1, nCntEnemy, -1, MOVETYPE_NORMAL, 100.0f, 100.0f, 200, 0.0f, -1);

								SetStaging(STAGINGTYPE_NORMAL_WARNINGS, nCntEnemy, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 150.0f, g_aEnemy[nCntEnemy].pos.y, 0.0f), 30, ATTACKTYPE_NEEDLE, 0.0f, 80.0f, -1,
									-1, -1, 40);

								SetAttack(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 150.0f, g_aEnemy[nCntEnemy].pos.y, 0.0f),
									D3DXVECTOR3(0.0f, 0.0f, 0.0f),
									ATTACKTYPE_NEEDLE,
									-1, nCntEnemy, -1, MOVETYPE_NORMAL, 100.0f, 100.0f, 200, 0.0f,-1);

								SetStaging(STAGINGTYPE_NORMAL_WARNINGS, nCntEnemy, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 150.0f, g_aEnemy[nCntEnemy].pos.y, 0.0f), 30, ATTACKTYPE_NEEDLE, 0.0f, 80.0f, -1,
									-1, -1, 40);

								SetAttack(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 450.0f, g_aEnemy[nCntEnemy].pos.y, 0.0f),
									D3DXVECTOR3(0.0f, 0.0f, 0.0f),
									ATTACKTYPE_NEEDLE,
									-1, nCntEnemy, -1, MOVETYPE_NORMAL, 100.0f, 150.0f, 200, 0.0f, -1);

								SetStaging(STAGINGTYPE_NORMAL_WARNINGS, nCntEnemy, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 450.0f, g_aEnemy[nCntEnemy].pos.y, 0.0f), 30, ATTACKTYPE_NEEDLE, 0.0f, 80.0f, -1,
									-1, -1, 40);

								SetAttack(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 450.0f, g_aEnemy[nCntEnemy].pos.y, 0.0f),
									D3DXVECTOR3(0.0f, 0.0f, 0.0f),
									ATTACKTYPE_NEEDLE,
									-1, nCntEnemy, -1, MOVETYPE_NORMAL, 100.0f, 150.0f, 200, 0.0f, -1);

								SetStaging(STAGINGTYPE_NORMAL_WARNINGS, nCntEnemy, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 450.0f, g_aEnemy[nCntEnemy].pos.y, 0.0f), 30, ATTACKTYPE_NEEDLE, 0.0f, 80.0f, -1,
									-1, -1, 40);

								SetAttack(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos),
									D3DXVECTOR3(0.0f, 0.0f, 0.0f),
									ATTACKTYPE_NEEDLE,
									-1, nCntEnemy, -1, MOVETYPE_NORMAL, 200.0f, 150.0f, 200, 0.0f, -1);

								SetStaging(STAGINGTYPE_NORMAL_WARNINGS, nCntEnemy,g_aEnemy[nCntEnemy].pos, 30, ATTACKTYPE_NEEDLE, 0.0f, 80.0f, -1,
									-1, -1, 40);
							}
						}
						else
						{//��̍U�������͈͊O

							if (g_nDelay % 30 == 0)
							{
								SetStaging(STAGINGTYPE_NORMAL_WARNINGS, nCntEnemy, pPlayer->pos, 30, ATTACKTYPE_NEEDLE, 0.0f,80.0f, -1,
									-1, -1, 40);

								SetAttack(D3DXVECTOR3(pPlayer->pos.x, pPlayer->pos.y - 400.0f, 0.0f),
									D3DXVECTOR3(0.0f, 20.0f, 0.0f),
									ATTACKTYPE_FALLINGBLOCK,
									-1, nCntEnemy, -1, MOVETYPE_NORMAL, 100.0f, 150.0f, 200, 0.0f, -1);
							}
						}

					}

					if (g_aEnemy[nCntEnemy].nApperCount == 1601)
					{
						g_aEnemy[nCntEnemy].nApperCount = 0;
					}
				}
				//�ڍs���[�v
				//======================================================================
				
				//===============================
				//�{�X�P�K�E�Z����
				//===============================
				if (g_aEnemy[nCntEnemy].bSpecialAttack == true)
				{
					g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 150.0f, 0.0f);

					//====================
					//�p�^�[���P
					//====================
					if (g_aEnemy[nCntEnemy].nApperCount >= 0 && g_aEnemy[nCntEnemy].nApperCount <= 600)
					{
						if (g_nDelay % 10 == 0)
						{
						
							SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, pPlayer->pos, 150, ATTACKTYPE_NEEDLE,0.0f,-1,-1,
								-1,-1, 60);
					     
						}
					}
					//==================================================================================================

					//=====================
					//�p�^�[���Q
					//=====================
					if (g_aEnemy[nCntEnemy].nApperCount == 680)
					{
						
							SetStaging(STAGINGTYPE_NORMAL_WARNINGS,
								nCntEnemy,D3DXVECTOR3(SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2,0.0f), 100, ATTACKTYPE_NEEDLE, -D3DX_PI * 1.5f,300,-1,
								-1,-1, 60);
					

						
					}

					if (g_aEnemy[nCntEnemy].nApperCount >= 780 && g_aEnemy[nCntEnemy].nApperCount >= 880)
					{
							SetAttack(D3DXVECTOR3(SCREEN_WIDTH / 2,SCREEN_HEIGHT, 0.0f),
							D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							ATTACKTYPE_NEEDLE,
							-1,nCntEnemy, -1, MOVETYPE_NORMAL, 800.0f, 720.0f, 200,0.0f,120);
					}

					

					//================================
					//�ʏ��Ԃɖ߂�
					//================================
					if (g_aEnemy[nCntEnemy].nApperCount == 1000)
					{
						g_aEnemy[nCntEnemy].bSpecialAttack = false;
						g_aEnemy[nCntEnemy].nApperCount = 0;
					}


					//===============================================================================================

				}
				//>\==================================================================================================================================================
				//=======================================
				//�e�N�X�`���A�j���[�V��������
				//=======================================
				if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS1_ATTACK)
				{
					g_aEnemy[nCntEnemy].nCntAnim++;//�A�j���[�V�����J�E���^�[
					if (g_aEnemy[nCntEnemy].nCntAnim >= 5)
					{//�A�j���[�V�����̃X�s�[�h��������
						g_aEnemy[nCntEnemy].nPatternAnim++;
						g_aEnemy[nCntEnemy].nCntAnim = 0;
					}

					if (g_aEnemy[nCntEnemy].nPatternAnim >= 5)
					{//���[�v�p����
						g_aEnemy[nCntEnemy].nPatternAnim = 0;
					}

					if (g_aEnemy[nCntEnemy].bDirection == false)
					{
						pVtx[0].tex = D3DXVECTOR2(0.0f + g_aEnemy[nCntEnemy].nPatternAnim * 0.2f, 0.0f);
						pVtx[1].tex = D3DXVECTOR2(0.2f + g_aEnemy[nCntEnemy].nPatternAnim * 0.2f, 0.0f);
						pVtx[2].tex = D3DXVECTOR2(0.0f + g_aEnemy[nCntEnemy].nPatternAnim * 0.2f, 1.0f);
						pVtx[3].tex = D3DXVECTOR2(0.2f + g_aEnemy[nCntEnemy].nPatternAnim * 0.2f, 1.0f);
					}
					else if (g_aEnemy[nCntEnemy].bDirection == true)
					{
						pVtx[1].tex = D3DXVECTOR2(0.0f + g_aEnemy[nCntEnemy].nPatternAnim * 0.2f, 0.0f);
						pVtx[0].tex = D3DXVECTOR2(0.2f + g_aEnemy[nCntEnemy].nPatternAnim * 0.2f, 0.0f);
						pVtx[3].tex = D3DXVECTOR2(0.0f + g_aEnemy[nCntEnemy].nPatternAnim * 0.2f, 1.0f);
						pVtx[2].tex = D3DXVECTOR2(0.2f + g_aEnemy[nCntEnemy].nPatternAnim * 0.2f, 1.0f);
					}
				}
				else if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS1_RUN)
				{//�ړ����[�V����
					g_aEnemy[nCntEnemy].nCntAnim++;//�A�j���[�V�����J�E���^�[
					if (g_aEnemy[nCntEnemy].nCntAnim >= 5)
					{
						g_aEnemy[nCntEnemy].nPatternAnim++;
						g_aEnemy[nCntEnemy].nCntAnim = 0;
					}

					if (g_aEnemy[nCntEnemy].nPatternAnim >= 8)
					{
						g_aEnemy[nCntEnemy].nPatternAnim = 0;
					}

					if (g_aEnemy[nCntEnemy].bDirection == false)
					{
						pVtx[0].tex = D3DXVECTOR2(0.0f + g_aEnemy[nCntEnemy].nPatternAnim * 0.125f, 0.0f);
						pVtx[1].tex = D3DXVECTOR2(0.125f + g_aEnemy[nCntEnemy].nPatternAnim * 0.125f, 0.0f);
						pVtx[2].tex = D3DXVECTOR2(0.0f + g_aEnemy[nCntEnemy].nPatternAnim * 0.125f, 1.0f);
						pVtx[3].tex = D3DXVECTOR2(0.125f + g_aEnemy[nCntEnemy].nPatternAnim * 0.125f, 1.0f);
					}
					else if (g_aEnemy[nCntEnemy].bDirection == true)
					{
						pVtx[1].tex = D3DXVECTOR2(0.0f + g_aEnemy[nCntEnemy].nPatternAnim * 0.125f, 0.0f);
						pVtx[0].tex = D3DXVECTOR2(0.125f + g_aEnemy[nCntEnemy].nPatternAnim * 0.125f, 0.0f);
						pVtx[3].tex = D3DXVECTOR2(0.0f + g_aEnemy[nCntEnemy].nPatternAnim * 0.125f, 1.0f);
						pVtx[2].tex = D3DXVECTOR2(0.125f + g_aEnemy[nCntEnemy].nPatternAnim * 0.125f, 1.0f);
					}
				}
				else if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS1_STAND)
				{//�ҋ@���[�V����
					g_aEnemy[nCntEnemy].nCntAnim++;//�A�j���[�V�����J�E���^�[
					if (g_aEnemy[nCntEnemy].nCntAnim >= 10)
					{
						g_aEnemy[nCntEnemy].nPatternAnim++;
						g_aEnemy[nCntEnemy].nCntAnim = 0;
					}

					if (g_aEnemy[nCntEnemy].nPatternAnim >= 4)
					{
						g_aEnemy[nCntEnemy].nPatternAnim = 0;
					}

					if (g_aEnemy[nCntEnemy].bDirection == false)
					{
						pVtx[0].tex = D3DXVECTOR2(0.0f + g_aEnemy[nCntEnemy].nPatternAnim * 0.25f, 0.0f);
						pVtx[1].tex = D3DXVECTOR2(0.25f + g_aEnemy[nCntEnemy].nPatternAnim * 0.25f, 0.0f);
						pVtx[2].tex = D3DXVECTOR2(0.0f + g_aEnemy[nCntEnemy].nPatternAnim * 0.25f, 1.0f);
						pVtx[3].tex = D3DXVECTOR2(0.25f + g_aEnemy[nCntEnemy].nPatternAnim * 0.25f, 1.0f);
					}
					else if (g_aEnemy[nCntEnemy].bDirection == true)
					{
						pVtx[1].tex = D3DXVECTOR2(0.0f + g_aEnemy[nCntEnemy].nPatternAnim * 0.25f, 0.0f);
						pVtx[0].tex = D3DXVECTOR2(0.25f + g_aEnemy[nCntEnemy].nPatternAnim * 0.25f, 0.0f);
						pVtx[3].tex = D3DXVECTOR2(0.0f + g_aEnemy[nCntEnemy].nPatternAnim * 0.25f, 1.0f);
						pVtx[2].tex = D3DXVECTOR2(0.25f + g_aEnemy[nCntEnemy].nPatternAnim * 0.25f, 1.0f);
					}
				}
			

				/*			g_aEnemy[nCntEnemy].nMoveTime++;*/

				//if (g_aEnemy[nCntEnemy].bAttackInterval == false)
				//{
				//	g_aEnemy[nCntEnemy].nAttarkIntervalCount++;

				//	if (g_aEnemy[nCntEnemy].nAttarkIntervalCount >= 80)
				//	{
				//		g_aEnemy[nCntEnemy].bAttackInterval = true;
				//		g_aEnemy[nCntEnemy].nAttarkIntervalCount = 0;
				//	}
				//}

				//if (g_aEnemy[nCntEnemy].bAttackInterval == true)
				//{
				//	g_nCountTechiniqueTime++;
				//}

				//if (g_nCountTechiniqueTime == 1)
				//{
				//	g_nBoss1TechiniquePattern = rand() % 8 + 1;//�{�X�̋Z�̎�ނ̔ԍ������߂�
				//}

				//if (g_aEnemy[nCntEnemy].bBossSummonFlag == false)
				//{
				//	g_aEnemy[nCntEnemy].bBossSummonFlag = true;
				//	StopSound(SOUND_LABEL_BGM001);

				//	PlaySound(SOUND_LABEL_BGM003);
				//}
				//if (g_aEnemy[nCntEnemy].nCountMoveInterval >= 300)
				//{
				//	g_aEnemy[nCntEnemy].fRotMove = float((rand)() % 629 - 314) / 100.0f;
				//	g_aEnemy[nCntEnemy].nCountMoveInterval = 0;//���ɓ����܂ł̎��Ԃ��������B
				//	g_aEnemy[nCntEnemy].bBossMoveFlag = true;//�{�X�𓮂���
				//}
				//if (g_aEnemy[nCntEnemy].nMoveTime >= 100)
				//{
				//	g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�����I������̂ŁAmove�̒l���������B
				//	g_aEnemy[nCntEnemy].nMoveTime = 0;
				//	g_aEnemy[nCntEnemy].bBossMoveFlag = false;

				//}
				//if (g_aEnemy[nCntEnemy].bAttackInterval == true)
				//{//�U����ԂɂȂ�����E�E�E
				//	if (g_nBoss1TechiniquePattern == 1)
				//	{
				//		g_nCountTechiniqueTime++;
				//		g_fXVaim[nCntEnemy] = pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x;//X���W�ւ̃x�N�g��
				//		g_fYVaim[nCntEnemy] = pPlayer->pos.y - g_aEnemy[nCntEnemy].pos.y;//Y���W�ւ̃x�N�g��
				//		g_Vaim[nCntEnemy] = atan2(g_fXVaim[nCntEnemy], g_fYVaim[nCntEnemy]);

				//		if (g_nDelay % 15 == 0)
				//		{
				//			PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_007);
				//			SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy]) * 8.0f, cosf(g_Vaim[nCntEnemy]) * 8.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1, false);//�G�̒e������G���m�œ�����Ȃ�
				//			SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy] + 0.2f) * 7.0f, cosf(g_Vaim[nCntEnemy] + 0.2f) * 7.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1, false);//�G�̒e������G���m�œ�����Ȃ�
				//			SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy] - 0.2f) * 6.0f, cosf(g_Vaim[nCntEnemy] - 0.2f) * 6.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1, false);//�G�̒e������G���m�œ�����Ȃ�
				//			SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy] + 0.4f) * 5.0f, cosf(g_Vaim[nCntEnemy] + 0.4) * 5.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1, false);//�G�̒e������G���m�œ�����Ȃ�
				//			SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy] - 0.4f) * 4.0f, cosf(g_Vaim[nCntEnemy] - 0.4f) * 4.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1, false);//�G�̒e������G���m�œ�����Ȃ�
				//		}

				//		if (g_nCountTechiniqueTime >= 150)
				//		{
				//			g_nCountTechiniqueTime = 0;
				//			g_aEnemy[nCntEnemy].bAttackInterval = false;
				//		}
				//	}

				//	if (g_aEnemy[nCntEnemy].bUse == true && g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS1_STARWARRIOR && g_nBoss1TechiniquePattern == 2)
				//	{//�G�l�~�[12�̍U��

				//		int nCntBullet_E4;
				//		float fCircleBullet_E4_normal[12];
				//		float fRotBullet_E4;
				//		fRotBullet_E4 = float((rand)() % 200) / 100.0f;

				//		if (g_nDelay % 25 == 0)
				//		{
				//			PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_004);

				//			for (nCntBullet_E4 = 0; nCntBullet_E4 < 12; nCntBullet_E4++)

				//			{
				//				fCircleBullet_E4_normal[nCntBullet_E4] = (1.0f / 6.0f) * nCntBullet_E4;

				//				SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * (fCircleBullet_E4_normal[nCntBullet_E4] + fRotBullet_E4)) * 4.0f), (cosf(-D3DX_PI * (fCircleBullet_E4_normal[nCntBullet_E4] + fRotBullet_E4)) * 4.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1, false);
				//				SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * (fCircleBullet_E4_normal[nCntBullet_E4] + fRotBullet_E4)) * 8.0f), (cosf(-D3DX_PI * (fCircleBullet_E4_normal[nCntBullet_E4] + fRotBullet_E4)) * 8.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1, false);

				//			}
				//		}

				//		if (g_nCountTechiniqueTime >= 150)
				//		{
				//			g_nCountTechiniqueTime = 0;
				//			g_aEnemy[nCntEnemy].bAttackInterval = false;
				//		}

				//		//SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((D3DX_PI * g_fXVaim[nCntEnemy]) * 0.004f, (D3DX_PI * g_fYVaim[nCntEnemy]) * 0.004f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy);//�G�̒e������G���m�œ�����Ȃ�
				//	}
				//	if (g_aEnemy[nCntEnemy].bUse == true && g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS1_STARWARRIOR && g_nBoss1TechiniquePattern == 3)
				//	{//�G�l�~�[12�̍U��(SetBullet)

				//		int nCntBullet_E5;
				//		float fCircleBullet_E5_normal[12];

				//		if (g_nDelay % 20 == 0)
				//		{
				//			PlaySound(SOUND_LABEL_SE_ARROWSHOT_000);
				//			for (nCntBullet_E5 = 0; nCntBullet_E5 < 8; nCntBullet_E5++)
				//			{
				//				SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_REAFY, nCntBullet_E5, nCntEnemy, -1, false);//�G�̒e������G���m�œ�����Ȃ�
				//				SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_REVERSE_CURVE, nCntBullet_E5, nCntEnemy, -1, false);//�G�̒e������G���m�œ�����Ȃ�

				//			}
				//		}

				//		if (g_nCountTechiniqueTime >= 300)
				//		{
				//			g_nCountTechiniqueTime = 0;
				//			g_aEnemy[nCntEnemy].bAttackInterval = false;
				//		}
				//	}
				//	if (g_aEnemy[nCntEnemy].bUse == true && g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS1_STARWARRIOR && g_nBoss1TechiniquePattern == 4)
				//	{//�G�l�~�[12�̍U��

				//		float fCircleBullet_E12[4];
				//		int nCntBullet_E12;
				//		float fCircleBullet_E12_easy[2];
				//		float fCircleBullet_E12_normal[4];
				//		float fCircleBullet_E12_hard[4];
				//		float fCircleBullet_E12_veryhard[4];

				//		if (g_nDelay % 50 == 0)
				//		{
				//			PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_001);
				//			for (nCntBullet2 = 0; nCntBullet2 < 4; nCntBullet2++)
				//			{
				//				fCircleBullet_E12_normal[nCntBullet2] = (1.0f / 2.0f) * nCntBullet2;


				//				SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * fCircleBullet_E12_normal[nCntBullet2] + g_fRandumResultBullet2) * fRandumSpeed_E12), (cosf(-D3DX_PI * fCircleBullet_E12_normal[nCntBullet2] + g_fRandumResultBullet2) * fRandumSpeed_E12), 0.0f), GENERALLY_BULLETLIFE / 5, BULLETTYPE_SCATTERING, 0, nCntEnemy, -1, false);//�G�̒e������G���m�œ�����Ȃ�		}

				//			}
				//		}

				//		if (g_nCountTechiniqueTime >= 150)
				//		{
				//			g_nCountTechiniqueTime = 0;
				//			g_aEnemy[nCntEnemy].bAttackInterval = false;
				//		}

				//		//SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((D3DX_PI * g_fXVaim[nCntEnemy]) * 0.004f, (D3DX_PI * g_fYVaim[nCntEnemy]) * 0.004f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy);//�G�̒e������G���m�œ�����Ȃ�
				//	}
				//	if (g_aEnemy[nCntEnemy].bUse == true && g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS1_STARWARRIOR && g_nBoss1TechiniquePattern == 5)
				//	{//�G�l�~�[12�̍U�� �i�_�u���E�C���h�j
				//		float fCircleBullet_E12[4];
				//		float fRotMove;
				//		float fRandumSpeed_E12_W5;

				//		if (g_nDelay % 80 == 0)
				//		{
				//			PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_008);
				//			for (nCntBullet2 = 0; nCntBullet2 < 15; nCntBullet2++)
				//			{

				//				fRotMove = float((rand)() % 100 + 1) / 100.0f;//�����̕����ɒe�𔭎˂���B(0.0f�`1.0f)
				//				fRandumSpeed_E12_W5 = float((rand)() % 150 + 1) / 10.0f;//�����̕����ɒe�𔭎˂���B(0.0f�`1.0f)
				//				SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * fRotMove) * fRandumSpeed_E12_W5), (cosf(-D3DX_PI * fRotMove) * fRandumSpeed_E12_W5), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_RIGHT_WIND, 0, nCntEnemy, -1, false);//�G�̒e������G���m�œ�����Ȃ�		}

				//			}

				//			for (nCntBullet2 = 0; nCntBullet2 < 15; nCntBullet2++)
				//			{

				//				fRotMove = float((rand)() % 100 + 1) / 100.0f;//�����̕����ɒe�𔭎˂���B(0.0f�`1.0f)
				//				fRandumSpeed_E12_W5 = float((rand)() % 150 + 1) / 10.0f;//�����̕����ɒe�𔭎˂���B(0.0f�`1.0f)
				//				SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(D3DX_PI * fRotMove) * fRandumSpeed_E12_W5), (cosf(D3DX_PI * fRotMove) * fRandumSpeed_E12_W5), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_LEFT_WIND, 0, nCntEnemy, -1, false);//�G�̒e������G���m�œ�����Ȃ�		}

				//			}
				//		}

				//		if (g_nCountTechiniqueTime >= 150)
				//		{
				//			g_nCountTechiniqueTime = 0;
				//			g_aEnemy[nCntEnemy].bAttackInterval = false;
				//		}


				//	}
				//	if (g_aEnemy[nCntEnemy].bUse == true && g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS1_STARWARRIOR && g_nBoss1TechiniquePattern == 6)
				//	{//�G�l�~�[12�̍U�� �i���˒e�j

				//		float fCircleBullet_easy[4];
				//		float fCircleBullet_normal[6];
				//		float fCircleBullet_hard[6];
				//		float fCircleBullet_veryhard[6];

				//		if (g_nDelay % 30 == 0)
				//		{
				//			PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_001);

				//			for (nCntBullet2 = 0; nCntBullet2 < 6; nCntBullet2++)
				//			{
				//				fCircleBullet_normal[nCntBullet2] = (1.0f / 3.0f) * nCntBullet2;

				//				SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * (fCircleBullet_normal[nCntBullet2] + g_fRotBullet_W6)) * 3.0f), (cosf(-D3DX_PI * (fCircleBullet_normal[nCntBullet2] + g_fRotBullet_W6)) * 3.0f), 0.0f), GENERALLY_BULLETLIFE / 5, BULLETTYPE_REFLECTION, 0, nCntEnemy, -1, false);//�G�̒e������G���m�œ�����Ȃ�		}

				//			}
				//		}

				//		if (g_nCountTechiniqueTime >= 150)
				//		{
				//			g_nCountTechiniqueTime = 0;
				//			g_aEnemy[nCntEnemy].bAttackInterval = false;
				//		}


				//	}
				//	if (g_aEnemy[nCntEnemy].bUse == true && g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS1_STARWARRIOR && g_nBoss1TechiniquePattern == 7)
				//	{//�G�l�~�[12�̍U�� (�~�X�g�o�[�X�g�j

				//		float fCircleBullet[6];
				//		float fRandumPosX;
				//		float fRandumPosY;

				//		fRandumPosX = float((rand)() % SCREEN_WIDTH + 1);
				//		fRandumPosY = float((rand)() % SCREEN_HEIGHT + 1);

				//		if (g_nDelay % 35 == 0)
				//		{
				//			PlaySound(SOUND_LABEL_SE_WARNING_001);
				//			SetStaging(STAGINGTYPE_BULLETWARNING, -1, D3DXVECTOR3(fRandumPosX, fRandumPosY, 0.0f));
				//		}

				//		if (g_nCountTechiniqueTime >= 400)
				//		{
				//			g_nCountTechiniqueTime = 0;
				//			g_aEnemy[nCntEnemy].bAttackInterval = false;
				//		}

				//		//SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((D3DX_PI * g_fXVaim[nCntEnemy]) * 0.004f, (D3DX_PI * g_fYVaim[nCntEnemy]) * 0.004f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy);//�G�̒e������G���m�œ�����Ȃ�
				//	}
				//	if (g_aEnemy[nCntEnemy].bUse == true && g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS1_STARWARRIOR && g_nBoss1TechiniquePattern == 8)
				//	{//�G�l�~�[12�̍U��(�q���e�j
				//	//	SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * 0.0f) * 5.0f), (cosf(-D3DX_PI * 0.0f) * 5.0f), 0.0f), 10, BULLETTYPE_REAFY);//�G�̒e������G���m�œ�����Ȃ�

				//		int nCntBullet_E5;
				//		float fCircleBullet_E5_easy[4];
				//		float fCircleBullet_E5_normal[12];
				//		float fCircleBullet_E5_hard[16];
				//		float fCircleBullet_E5_veryhard[24];

				//		if (g_nDelay % 50 == 0)
				//		{
				//			PlaySound(SOUND_LABEL_SE_ARROWSHOT_000);
				//			for (nCntBullet_E5 = 0; nCntBullet_E5 < 8; nCntBullet_E5++)
				//			{
				//				SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), GENERALLY_BULLETLIFE - 1200, BULLETTYPE_SATELLITE, nCntBullet_E5, nCntEnemy, -1, false);//�G�̒e������G���m�œ�����Ȃ�

				//			}
				//		}

				//		if (g_nCountTechiniqueTime >= 150)
				//		{
				//			g_nCountTechiniqueTime = 0;
				//			g_aEnemy[nCntEnemy].bAttackInterval = false;
				//		}
				//	}
				//}
			}
			//=========================================================
			//�{�X�Q�̏���
			//=========================================================
			if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS2_STAND ||
				g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS2_RUN ||
				g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS2_ATTACK)
			{//�{�X���g�p���ꂽ��



				g_aEnemy[nCntEnemy].nCountMoveInterval++;
				g_aEnemy[nCntEnemy].nApperCount++;//�G���o�����Ă���̎��Ԃ��J�E���g����B

				if (g_aEnemy[nCntEnemy].nCountMoveInterval == 1)
				{
					StopSound(SOUND_LABEL_BGM007);
					PlaySound(SOUND_LABEL_BGM004);
					SetGage(nCntEnemy, GAGETYPE_BOSS1HP);
					SetGage(-1, GAGETYPE_BOSSHP_FRAME);
				}
			

				//================================
				//�`�ԕω�����
				//================================
				if (g_aEnemy[nCntEnemy].fLife <= g_aEnemy[nCntEnemy].fMaxHp / 4)
				{//�c��HP���ő�HP�̂S���̂P�ɂȂ�����
					if (g_aEnemy[nCntEnemy].bSpecialStaging == false)
					{
						PlaySound(SOUND_LABEL_SE_SPECIALATTACK);
						g_aEnemy[nCntEnemy].bSpecialAttack = true;
						g_aEnemy[nCntEnemy].bSpecialStaging = true;
						/*SetStaging(STAGINGTYPE_BOSS2_SPECIAL_ATTACK, -1, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 180, ATTACKTYPE_PUNCH, 0.0f, -1, - 1,
							-1,-1, 150);*/
						SetSpecial(SPECIALTYPE_BOSS2_SPECIAL_ATTACK, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 150);
						g_aEnemy[nCntEnemy].nApperCount = 0;
					}
				}

				//============================================
				//�ʏ�s�������i�p�^�[���P�`�W�j
				//============================================
				if (g_aEnemy[nCntEnemy].bSpecialAttack == false)
				{


					//=========================
					//�p�^�[���P
					//=========================
					if (g_aEnemy[nCntEnemy].nApperCount >= 100 && g_aEnemy[nCntEnemy].nApperCount <= 600)
					{
						/*g_aEnemy[nCntEnemy].nType = MOTIONTYPE*/
						if (g_aEnemy[nCntEnemy].bDirection == false)
						{//���������Ă���
							g_aEnemy[nCntEnemy].move.x = 10.0f;
						}
						else
						{//�E�������Ă���
							g_aEnemy[nCntEnemy].move.x = -10.0f;
						}

						if (g_aEnemy[nCntEnemy].bIsLanding == true)
						{
							g_aEnemy[nCntEnemy].move.y = -20.0f;
						}

						int nCntBullet_Boss2;
						float fCircleBullet_Boss2[12];
						float fRotBullet_Boss2;
						fRotBullet_Boss2 = float((rand)() % 200) / 100.0f;

						PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_004);

						if (g_nDelay % 90 == 0)
						{
							for (nCntBullet_Boss2 = 0; nCntBullet_Boss2 < 12; nCntBullet_Boss2++)

							{
								fCircleBullet_Boss2[nCntBullet_Boss2] = (1.0f / 6.0f) * nCntBullet_Boss2;

								SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].fHeight / 2, 0.0f),
									D3DXVECTOR3((sinf(-D3DX_PI * (fCircleBullet_Boss2[nCntBullet_Boss2] + fRotBullet_Boss2)) * 4.0f), (cosf(-D3DX_PI * (fCircleBullet_Boss2[nCntBullet_Boss2] + fRotBullet_Boss2)) * 4.0f), 0.0f),
									GENERALLY_BULLETLIFE, BULLETTYPE_SNOW, 0, nCntEnemy, -1, true, MOVETYPE_NORMAL);

							}
						}
					}
					//==================================
					//�p�^�[���Q
					//==================================
					if (g_aEnemy[nCntEnemy].nApperCount == 700)
					{

						g_fXVaim[nCntEnemy] = 300.0f - g_aEnemy[nCntEnemy].pos.x;//X���W�ւ̃x�N�g��
						g_fYVaim[nCntEnemy] = 360.0f - g_aEnemy[nCntEnemy].pos.y;//Y���W�ւ̃x�N�g��
						g_Vaim[nCntEnemy] = atan2(g_fXVaim[nCntEnemy], g_fYVaim[nCntEnemy]);//�e���������̌���


						g_aEnemy[nCntEnemy].bGravity = false;//�d��Off
						g_aEnemy[nCntEnemy].move.x = sinf(g_Vaim[nCntEnemy]) * 15.0f;
						g_aEnemy[nCntEnemy].move.y = cosf(g_Vaim[nCntEnemy]) * 15.0f;

					
					}
					//============
					//�����P
					//============
					if (g_aEnemy[nCntEnemy].nApperCount >= 701 && g_aEnemy[nCntEnemy].nApperCount <= 880)
					{//�ړI�̈ʒu�Ɉړ������鏈��
						if (g_aEnemy[nCntEnemy].pos.x >= 270.0f && g_aEnemy[nCntEnemy].pos.y >= 330.0f &&
							g_aEnemy[nCntEnemy].pos.x <= 330.0f && g_aEnemy[nCntEnemy].pos.y <= 390.0f)
						{
							g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						}
					}

					if (g_aEnemy[nCntEnemy].nApperCount == 801)
					{
						SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 200.0f, g_aEnemy[nCntEnemy].pos.y, 0.0f), 60, ATTACKTYPE_ICE_EXPLOSION, 0.0f, 200.0f, -1, -1, -1, 20);
						SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 200.0f, g_aEnemy[nCntEnemy].pos.y, 0.0f), 60, ATTACKTYPE_ICE_EXPLOSION, 0.0f, 200.0f, -1, -1, -1, 20);
						SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y - 250.0f, 0.0f), 60, ATTACKTYPE_ICE_EXPLOSION, 0.0f, 200.0f, -1, -1, -1, 20);
						SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y + 250.0f, 0.0f), 60, ATTACKTYPE_ICE_EXPLOSION, 0.0f, 200.0f, -1, -1, -1, 30);
						SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 200.0f, g_aEnemy[nCntEnemy].pos.y - 200.0f, 0.0f), 60, ATTACKTYPE_ICE_EXPLOSION, 0.0f, 200.0f, -1, -1, -1, 20);
						SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 200.0f, g_aEnemy[nCntEnemy].pos.y + 200.0f, 0.0f), 60, ATTACKTYPE_ICE_EXPLOSION, 0.0f, 200.0f, -1, -1, -1, 20);
						SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 200.0f, g_aEnemy[nCntEnemy].pos.y + 200.0f, 0.0f), 60, ATTACKTYPE_ICE_EXPLOSION, 0.0f, 200.0f, -1, -1, -1, 20);
						SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 200.0f, g_aEnemy[nCntEnemy].pos.y - 200.0f, 0.0f), 60, ATTACKTYPE_ICE_EXPLOSION, 0.0f, 200.0f, -1, -1, -1, 20);
					}

					//=============
					//�����Q
					//=============

					if (g_aEnemy[nCntEnemy].nApperCount == 880)
					{

						g_fXVaim[nCntEnemy] = 900.0f - g_aEnemy[nCntEnemy].pos.x;//X���W�ւ̃x�N�g��
						g_fYVaim[nCntEnemy] = 360.0f - g_aEnemy[nCntEnemy].pos.y;//Y���W�ւ̃x�N�g��
						g_Vaim[nCntEnemy] = atan2(g_fXVaim[nCntEnemy], g_fYVaim[nCntEnemy]);//�e���������̌���


						g_aEnemy[nCntEnemy].bGravity = false;//�d��Off
						g_aEnemy[nCntEnemy].move.x = sinf(g_Vaim[nCntEnemy]) * 15.0f;
						g_aEnemy[nCntEnemy].move.y = cosf(g_Vaim[nCntEnemy]) * 15.0f;

						//g_fXVaim[nCntEnemy] = pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x;//X���W�ւ̃x�N�g��
						//g_fYVaim[nCntEnemy] = pPlayer->pos.y - g_aEnemy[nCntEnemy].pos.y;//Y���W�ւ̃x�N�g��
						//g_Vaim[nCntEnemy] = atan2(g_fXVaim[nCntEnemy], g_fYVaim[nCntEnemy]);//�e���������̌���
								/*SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy]) * 10.0f, cosf(g_Vaim[nCntEnemy]) * 10.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1, false, MOVETYPE_NORMAL);
							*/
					}
					if (g_aEnemy[nCntEnemy].nApperCount >= 881 && g_aEnemy[nCntEnemy].nApperCount <= 951)
					{//�ړI�̈ʒu�Ɉړ������鏈��
						if (g_aEnemy[nCntEnemy].pos.x >= 870.0f && g_aEnemy[nCntEnemy].pos.y >= 330.0f &&
							g_aEnemy[nCntEnemy].pos.x <= 930.0f && g_aEnemy[nCntEnemy].pos.y <= 390.0f)
						{
							g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						}
					}

					if (g_aEnemy[nCntEnemy].nApperCount == 952)
					{
						SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 200.0f, g_aEnemy[nCntEnemy].pos.y, 0.0f), 60, ATTACKTYPE_ICE_EXPLOSION, 0.0f, 200.0f, -1, -1, -1, 20);
						SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 200.0f, g_aEnemy[nCntEnemy].pos.y, 0.0f), 60, ATTACKTYPE_ICE_EXPLOSION, 0.0f, 200.0f, -1, -1, -1, 20);
						SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y - 250.0f, 0.0f), 60, ATTACKTYPE_ICE_EXPLOSION, 0.0f, 200.0f, -1, -1, -1, 20);
						SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y + 250.0f, 0.0f), 60, ATTACKTYPE_ICE_EXPLOSION, 0.0f, 200.0f, -1, -1, -1, 20);
						SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 200.0f, g_aEnemy[nCntEnemy].pos.y - 200.0f, 0.0f), 60, ATTACKTYPE_ICE_EXPLOSION, 0.0f, 200.0f, -1, -1, -1, 20);
						SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 200.0f, g_aEnemy[nCntEnemy].pos.y + 200.0f, 0.0f), 60, ATTACKTYPE_ICE_EXPLOSION, 0.0f, 200.0f, -1, -1, -1, 20);
						SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 200.0f, g_aEnemy[nCntEnemy].pos.y + 200.0f, 0.0f), 60, ATTACKTYPE_ICE_EXPLOSION, 0.0f, 200.0f, -1, -1, -1, 20);
						SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 200.0f, g_aEnemy[nCntEnemy].pos.y - 200.0f, 0.0f), 60, ATTACKTYPE_ICE_EXPLOSION, 0.0f, 200.0f, -1, -1, -1, 20);
					}

					//=============
					//�����R
					//=============

					if (g_aEnemy[nCntEnemy].nApperCount >= 1001 && g_aEnemy[nCntEnemy].nApperCount <= 1101)
					{

						g_fXVaim[nCntEnemy] = pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x;//X���W�ւ̃x�N�g��
						g_fYVaim[nCntEnemy] = pPlayer->pos.y - g_aEnemy[nCntEnemy].pos.y;//Y���W�ւ̃x�N�g��
						g_Vaim[nCntEnemy] = atan2(g_fXVaim[nCntEnemy], g_fYVaim[nCntEnemy]);//�e���������̌���


						g_aEnemy[nCntEnemy].bGravity = false;//�d��Off
						g_aEnemy[nCntEnemy].move.x = sinf(g_Vaim[nCntEnemy]) * 15.0f;
						g_aEnemy[nCntEnemy].move.y = cosf(g_Vaim[nCntEnemy]) * 15.0f;

						//g_fXVaim[nCntEnemy] = pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x;//X���W�ւ̃x�N�g��
						//g_fYVaim[nCntEnemy] = pPlayer->pos.y - g_aEnemy[nCntEnemy].pos.y;//Y���W�ւ̃x�N�g��
						//g_Vaim[nCntEnemy] = atan2(g_fXVaim[nCntEnemy], g_fYVaim[nCntEnemy]);//�e���������̌���
								/*SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy]) * 10.0f, cosf(g_Vaim[nCntEnemy]) * 10.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1, false, MOVETYPE_NORMAL);
							*/
					}
					if (g_aEnemy[nCntEnemy].nApperCount >= 1001 && g_aEnemy[nCntEnemy].nApperCount <= 1101)
					{//�ړI�̈ʒu�Ɉړ������鏈��
						if (g_aEnemy[nCntEnemy].pos.x >= pPlayer->pos.x - 30.0f && pPlayer->pos.y >= -30.0f &&
							g_aEnemy[nCntEnemy].pos.x <= pPlayer->pos.x + 30.0f && pPlayer->pos.y <= +30.0f)
						{
							g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						}
					}

					if (g_aEnemy[nCntEnemy].nApperCount == 1102)
					{
						g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						g_aEnemy[nCntEnemy].nType = ENEMYTYPE_BOSS2_ATTACK;
						g_aEnemy[nCntEnemy].bGravity = true;
						SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 200.0f, g_aEnemy[nCntEnemy].pos.y, 0.0f), 60, ATTACKTYPE_ICE_EXPLOSION, 0.0f, 200.0f, -1, -1, -1, 60);
						SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 200.0f, g_aEnemy[nCntEnemy].pos.y, 0.0f), 60, ATTACKTYPE_ICE_EXPLOSION, 0.0f, 200.0f, -1, -1, -1, 60);
						SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y - 250.0f, 0.0f), 60, ATTACKTYPE_ICE_EXPLOSION, 0.0f, 200.0f, -1, -1, -1, 60);
						SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y + 250.0f, 0.0f), 60, ATTACKTYPE_ICE_EXPLOSION, 0.0f, 200.0f, -1, -1, -1, 60);
						SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 200.0f, g_aEnemy[nCntEnemy].pos.y - 200.0f, 0.0f), 60, ATTACKTYPE_ICE_EXPLOSION, 0.0f, 200.0f, -1, -1, -1, 60);
						SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 200.0f, g_aEnemy[nCntEnemy].pos.y + 200.0f, 0.0f), 60, ATTACKTYPE_ICE_EXPLOSION, 0.0f, 200.0f, -1, -1, -1, 60);
						SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 200.0f, g_aEnemy[nCntEnemy].pos.y + 200.0f, 0.0f), 60, ATTACKTYPE_ICE_EXPLOSION, 0.0f, 200.0f, -1, -1, -1, 60);
						SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 200.0f, g_aEnemy[nCntEnemy].pos.y - 200.0f, 0.0f), 60, ATTACKTYPE_ICE_EXPLOSION, 0.0f, 200.0f, -1, -1, -1, 60);
					}

					if (g_aEnemy[nCntEnemy].nApperCount == 1162)
					{
						for (int nCntBullet = 0; nCntBullet < 10; nCntBullet++)
						{
							
								SetStaging(STAGINGTYPE_NORMAL_WARNINGS, nCntEnemy, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, 0.0f), 150, ATTACKTYPE_NEEDLE, 0.0f, 80.0f, -1,
									-1, -1, 30);
								SetStaging(STAGINGTYPE_NORMAL_WARNINGS, nCntEnemy, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 50.0f, g_aEnemy[nCntEnemy].pos.y, 0.0f), 150, ATTACKTYPE_NEEDLE, 0.0f, 80.0f, -1,
									-1, -1, 30);
								SetStaging(STAGINGTYPE_NORMAL_WARNINGS, nCntEnemy, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 50.0f, g_aEnemy[nCntEnemy].pos.y, 0.0f), 150, ATTACKTYPE_NEEDLE, 0.0f, 80.0f, -1,
									-1, -1, 30);
							

							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y - 10.0f * nCntBullet, 0.0f),
								D3DXVECTOR3(sinf(-D3DX_PI * 0.5f) * 10.0f, cosf(-D3DX_PI * 0.5f) * 10.0f, 0.0f),
								300, BULLETTYPE_SNOW, -1, -1, -1, false, MOVETYPE_NORMAL);

							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y - 10.0f * nCntBullet, 0.0f),
								D3DXVECTOR3(sinf(-D3DX_PI * 1.5f) * 10.0f, cosf(-D3DX_PI * 1.5f) * 10.0f, 0.0f),
								300, BULLETTYPE_SNOW, -1, -1, -1, false, MOVETYPE_NORMAL);

							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y - 10.0f * nCntBullet, 0.0f),
								D3DXVECTOR3(sinf(-D3DX_PI * 0.5f) * 7.0f, cosf(-D3DX_PI * 0.5f) * 7.0f, 0.0f),
								300, BULLETTYPE_SNOW, -1, -1, -1, false, MOVETYPE_NORMAL);

							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y - 10.0f * nCntBullet, 0.0f),
								D3DXVECTOR3(sinf(-D3DX_PI * 1.5f) * 7.0f, cosf(-D3DX_PI * 1.5f) * 7.0f, 0.0f),
								300, BULLETTYPE_SNOW, -1, -1, -1, false, MOVETYPE_NORMAL);

							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y - 100.0f - 10.0f * nCntBullet, 0.0f),
								D3DXVECTOR3(sinf(-D3DX_PI * 0.5f) * 4.0f, cosf(-D3DX_PI * 0.5f) * 4.0f, 0.0f),
								300, BULLETTYPE_SNOW, -1, -1, -1, false, MOVETYPE_NORMAL);

							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y - 100.0f - 10.0f * nCntBullet, 0.0f),
								D3DXVECTOR3(sinf(-D3DX_PI * 1.5f) * 4.0f, cosf(-D3DX_PI * 1.5f) * 4.0f, 0.0f),
								300, BULLETTYPE_SNOW, -1, -1, -1, false, MOVETYPE_NORMAL);
						}
					}
					//================================
					//�p�^�[���R
					//================================

					if (g_aEnemy[nCntEnemy].nApperCount >= 1250 && g_aEnemy[nCntEnemy].nApperCount <= 1880)
					{
						g_aEnemy[nCntEnemy].nType = ENEMYTYPE_BOSS2_STAND;

						int nCntBullet_Boss2;
						float fCircleBullet_Boss2[2];
						float fRotBullet_Boss2;
						fRotBullet_Boss2 = float((rand)() % 200) / 100.0f;
						if (g_aEnemy[nCntEnemy].bDirection == false)
						{
							g_aEnemy[nCntEnemy].move.x = 2.0f;
						}
						else if (g_aEnemy[nCntEnemy].bDirection == true)
						{
							g_aEnemy[nCntEnemy].move.x = -2.0f;
						}

						if (g_nDelay % 30 == 0)
						{
							SetStaging(STAGINGTYPE_NORMAL_WARNINGS, nCntEnemy, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, 0.0f), 150, ATTACKTYPE_NEEDLE, 0.0f, 80.0f, -1,
								-1, -1, 30);
							SetStaging(STAGINGTYPE_NORMAL_WARNINGS, nCntEnemy, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 50.0f, g_aEnemy[nCntEnemy].pos.y, 0.0f), 150, ATTACKTYPE_NEEDLE, 0.0f, 80.0f, -1,
								-1, -1, 30);
							SetStaging(STAGINGTYPE_NORMAL_WARNINGS, nCntEnemy, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 50.0f, g_aEnemy[nCntEnemy].pos.y, 0.0f), 150, ATTACKTYPE_NEEDLE, 0.0f, 80.0f, -1,
								-1, -1, 30);
						}

						//================
						//�s���P
						//================
						PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_004);
						if (g_aEnemy[nCntEnemy].nApperCount >= 1400 && g_aEnemy[nCntEnemy].nApperCount <= 1430)
							for (nCntBullet_Boss2 = 0; nCntBullet_Boss2 < 2; nCntBullet_Boss2++)

							{
								fCircleBullet_Boss2[nCntBullet_Boss2] = (1.0f / 1.0f) * nCntBullet_Boss2;

								SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].fHeight / 2, 0.0f),
									D3DXVECTOR3((sinf(-D3DX_PI * (fCircleBullet_Boss2[nCntBullet_Boss2] + fRotBullet_Boss2)) * 0.1f), (cosf(-D3DX_PI * (fCircleBullet_Boss2[nCntBullet_Boss2] + fRotBullet_Boss2)) * 0.1f), 0.0f),
									GENERALLY_BULLETLIFE, BULLETTYPE_ICICLE, 0, nCntEnemy, -1, true, MOVETYPE_NORMAL);

							}
						//======================================================

						//================
						//�s���Q
						//================
						PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_004);
						if (g_aEnemy[nCntEnemy].nApperCount >= 1650 && g_aEnemy[nCntEnemy].nApperCount <= 1680)
							for (nCntBullet_Boss2 = 0; nCntBullet_Boss2 < 2; nCntBullet_Boss2++)

							{
								fCircleBullet_Boss2[nCntBullet_Boss2] = (1.0f / 1.0f) * nCntBullet_Boss2;

								SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].fHeight / 2, 0.0f),
									D3DXVECTOR3((sinf(-D3DX_PI * (fCircleBullet_Boss2[nCntBullet_Boss2] + fRotBullet_Boss2)) * 0.1f), (cosf(-D3DX_PI * (fCircleBullet_Boss2[nCntBullet_Boss2] + fRotBullet_Boss2)) * 0.1f), 0.0f),
									GENERALLY_BULLETLIFE, BULLETTYPE_ICICLE, 0, nCntEnemy, -1, true, MOVETYPE_NORMAL);

							}
						//======================================================

						//================
						//�s���R
						//================
						PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_004);
						if (g_aEnemy[nCntEnemy].nApperCount >= 1850 && g_aEnemy[nCntEnemy].nApperCount <= 1880)
							for (nCntBullet_Boss2 = 0; nCntBullet_Boss2 < 2; nCntBullet_Boss2++)

							{
								fCircleBullet_Boss2[nCntBullet_Boss2] = (1.0f / 1.0f) * nCntBullet_Boss2;

								SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].fHeight / 2, 0.0f),
									D3DXVECTOR3((sinf(-D3DX_PI * (fCircleBullet_Boss2[nCntBullet_Boss2] + fRotBullet_Boss2)) * 0.1f), (cosf(-D3DX_PI * (fCircleBullet_Boss2[nCntBullet_Boss2] + fRotBullet_Boss2)) * 0.1f), 0.0f),
									GENERALLY_BULLETLIFE, BULLETTYPE_ICICLE, 0, nCntEnemy, -1, true, MOVETYPE_NORMAL);

							}
						//======================================================
					}
					//===================
					//�s���S
					//===================
					if (g_aEnemy[nCntEnemy].nApperCount >= 1881 && g_aEnemy[nCntEnemy].nApperCount <= 2100)
					{
						if (g_aEnemy[nCntEnemy].bDirection == false)
						{
							g_aEnemy[nCntEnemy].move.x = 8.0f;
						}
						else if (g_aEnemy[nCntEnemy].bDirection == true)
						{
							g_aEnemy[nCntEnemy].move.x = -8.0f;
						}
						if (g_aEnemy[nCntEnemy].nApperCount == 1881)
						{
							SetStaging(STAGINGTYPE_NORMAL_WARNINGS_RELATED, -1, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 100.0f, g_aEnemy[nCntEnemy].pos.y, 0.0f), 60, ATTACKTYPE_PUNCH, -1, 100.0f, -1, -1, -1, -1);
							SetStaging(STAGINGTYPE_NORMAL_WARNINGS_RELATED, -1, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 100.0f, g_aEnemy[nCntEnemy].pos.y, 0.0f), 60, ATTACKTYPE_PUNCH, -1, 100.0f, -1, -1, -1, -1);
						}
					}

					if (g_aEnemy[nCntEnemy].nApperCount == 1950)
					{

						SetStaging(STAGINGTYPE_NORMAL_WARNINGS_RELATED, -1, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 100.0f, g_aEnemy[nCntEnemy].pos.y, 0.0f), 60, ATTACKTYPE_PUNCH, -1, 100.0f, -1, -1, -1, 30);
						SetStaging(STAGINGTYPE_NORMAL_WARNINGS_RELATED, -1, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 100.0f, g_aEnemy[nCntEnemy].pos.y, 0.0f), 60, ATTACKTYPE_PUNCH, -1, 100.0f, -1, -1, -1, 30);
						SetAttack(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 100.0f, g_aEnemy[nCntEnemy].pos.y, 0.0f),
							D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							ATTACKTYPE_ICE_EXPLOSION,
							-1, nCntEnemy, -1, MOVETYPE_NORMAL, 150.0f, 150.0f, 50, 0.0f, -1);
						SetAttack(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 100.0f, g_aEnemy[nCntEnemy].pos.y, 0.0f),
							D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							ATTACKTYPE_ICE_EXPLOSION,
							-1, nCntEnemy, -1, MOVETYPE_NORMAL, 150.0f, 150.0f, 50, 0.0f, -1);
					}

					if (g_aEnemy[nCntEnemy].nApperCount == 2020)
					{

						SetStaging(STAGINGTYPE_NORMAL_WARNINGS_RELATED, -1, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 100.0f, g_aEnemy[nCntEnemy].pos.y, 0.0f), 60, ATTACKTYPE_PUNCH, -1, 100.0f, -1, -1, -1, 30);
						SetStaging(STAGINGTYPE_NORMAL_WARNINGS_RELATED, -1, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 100.0f, g_aEnemy[nCntEnemy].pos.y, 0.0f), 60, ATTACKTYPE_PUNCH, -1, 100.0f, -1, -1, -1, 30);
						SetAttack(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 100.0f, g_aEnemy[nCntEnemy].pos.y, 0.0f),
							D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							ATTACKTYPE_ICE_EXPLOSION,
							-1, nCntEnemy, -1, MOVETYPE_NORMAL, 150.0f, 150.0f, 50, 0.0f, -1);
						SetAttack(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 100.0f, g_aEnemy[nCntEnemy].pos.y, 0.0f),
							D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							ATTACKTYPE_ICE_EXPLOSION,
							-1, nCntEnemy, -1, MOVETYPE_NORMAL, 150.0f, 150.0f, 50, 0.0f, -1);
					}

					if (g_aEnemy[nCntEnemy].nApperCount == 2090)
					{
						SetAttack(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 100.0f, g_aEnemy[nCntEnemy].pos.y, 0.0f),
							D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							ATTACKTYPE_ICE_EXPLOSION,
							-1, nCntEnemy, -1, MOVETYPE_NORMAL, 150.0f, 150.0f, 50, 0.0f, -1);
						SetAttack(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 100.0f, g_aEnemy[nCntEnemy].pos.y, 0.0f),
							D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							ATTACKTYPE_ICE_EXPLOSION,
							-1, nCntEnemy, -1, MOVETYPE_NORMAL, 150.0f, 150.0f, 50, 0.0f, -1);
					}

					//====================
					//�p�^�[���S
					//====================
					if (g_aEnemy[nCntEnemy].nApperCount == 2150)
					{

						g_fXVaim[nCntEnemy] = SCREEN_WIDTH / 2 - g_aEnemy[nCntEnemy].pos.x;//X���W�ւ̃x�N�g��
						g_fYVaim[nCntEnemy] = SCREEN_HEIGHT / 2 - g_aEnemy[nCntEnemy].pos.y;//Y���W�ւ̃x�N�g��
						g_Vaim[nCntEnemy] = atan2(g_fXVaim[nCntEnemy], g_fYVaim[nCntEnemy]);//�x�N�g���̌���


						g_aEnemy[nCntEnemy].bGravity = false;//�d��Off
						g_aEnemy[nCntEnemy].move.x = sinf(g_Vaim[nCntEnemy]) * 15.0f;
						g_aEnemy[nCntEnemy].move.y = cosf(g_Vaim[nCntEnemy]) * 15.0f;

						//g_fXVaim[nCntEnemy] = pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x;//X���W�ւ̃x�N�g��
						//g_fYVaim[nCntEnemy] = pPlayer->pos.y - g_aEnemy[nCntEnemy].pos.y;//Y���W�ւ̃x�N�g��
						//g_Vaim[nCntEnemy] = atan2(g_fXVaim[nCntEnemy], g_fYVaim[nCntEnemy]);//�e���������̌���
								/*SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy]) * 10.0f, cosf(g_Vaim[nCntEnemy]) * 10.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1, false, MOVETYPE_NORMAL);
							*/
					}
					//============
					//�s���P
					//============
					if (g_aEnemy[nCntEnemy].nApperCount >= 2151 && g_aEnemy[nCntEnemy].nApperCount <= 2211)
					{//�ړI�̈ʒu�Ɉړ������鏈��
						if (g_aEnemy[nCntEnemy].pos.x >= SCREEN_WIDTH / 2 - 20.0f && g_aEnemy[nCntEnemy].pos.y >= SCREEN_HEIGHT / 2 - 20.0f &&
							g_aEnemy[nCntEnemy].pos.x <= SCREEN_WIDTH / 2 + 20.0f && g_aEnemy[nCntEnemy].pos.y <= SCREEN_HEIGHT / 2 + 20.0f)
						{
							g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						}
					}

					if (g_aEnemy[nCntEnemy].nApperCount == 2221)
					{
						for (int nCnt = 0; nCnt < 8; nCnt++)
						{
							SetStaging(STAGINGTYPE_WARNING_ATTACK,
								nCntEnemy,
								D3DXVECTOR3(160 * nCnt, 680.0f, 0.0f),
								60, ATTACKTYPE_ICE_EXPLOSION, 0.0f, 200.0f, -1, -1, -1, 20);
						}
					}

					if (g_aEnemy[nCntEnemy].nApperCount == 2300)
					{
						for (int nCnt = 0; nCnt < 16; nCnt++)
						{
							SetStaging(STAGINGTYPE_WARNING_ATTACK,
								nCntEnemy,
								D3DXVECTOR3(80.0f + 160.0f * nCnt, 680.0f, 0.0f),
								60, ATTACKTYPE_ICE_EXPLOSION, 0.0f, 200.0f, -1, -1, -1, 20);
						}
						/*	for (int nCnt = 0; nCnt < 4; nCnt++)
							{
								SetStaging(STAGINGTYPE_WARNING_ATTACK,
									nCntEnemy,
									D3DXVECTOR3(320.0f * nCnt, 680.0f, 0.0f),
									60, ATTACKTYPE_ICE_EXPLOSION, 0.0f, 200.0f);
							}*/

					}

					if (g_aEnemy[nCntEnemy].nApperCount == 2450)
					{
						g_aEnemy[nCntEnemy].nType = ENEMYTYPE_BOSS2_ATTACK;

						for (int nCnt = 0; nCnt < 16; nCnt++)
						{
							SetStaging(STAGINGTYPE_WARNING_ATTACK,
								nCntEnemy,
								D3DXVECTOR3(80.0f * nCnt, 680.0f, 0.0f),
								60, ATTACKTYPE_ICE_EXPLOSION, 0.0f, 200.0f, -1, -1, -1, 25);
						}
						for (int nCnt = 0; nCnt < 4; nCnt++)
						{
							SetStaging(STAGINGTYPE_WARNING_ATTACK,
								nCntEnemy,
								D3DXVECTOR3(100.0f + 320.0f * nCnt, 400.0f, 0.0f),
								60, ATTACKTYPE_ICE_EXPLOSION, 0.0f, 220.0f, -1, -1, -1, 90);
						}

					}



					//==========================
					//�p�^�[���T
					//==========================
					if (g_aEnemy[nCntEnemy].nApperCount == 2550)
					{
						g_aEnemy[nCntEnemy].bGravity = true;

						g_aEnemy[nCntEnemy].nAttackCount = 0;

						if (g_aEnemy[nCntEnemy].bDirection == false)
						{//�E�������Ă���
							g_aEnemy[nCntEnemy].bMovePattern = false;

							g_fSaveSpeed = 10.0f;
						}
						else
						{//���������Ă���
							g_aEnemy[nCntEnemy].bMovePattern = true;

							g_fSaveSpeed = -10.0f;
						}
					}

					if (g_aEnemy[nCntEnemy].nApperCount >= 2551 && g_aEnemy[nCntEnemy].nApperCount <= 3000)
					{
						g_aEnemy[nCntEnemy].nType = ENEMYTYPE_BOSS2_ATTACK;

						if (g_aEnemy[nCntEnemy].bMoveWay == false)
						{
							g_aEnemy[nCntEnemy].move.x = g_fSaveSpeed;
						}
						else
						{
							g_aEnemy[nCntEnemy].move.x = -g_fSaveSpeed;
						}


						if (g_aEnemy[nCntEnemy].nApperCount == 2600)
						{

							SetStaging(STAGINGTYPE_WARNING_ATTACK,
								nCntEnemy,
								D3DXVECTOR3(pPlayer->pos.x - 200.0f, pPlayer->pos.y - 200.0f, 0.0f),
								60, ATTACKTYPE_ICICLECLAW, -D3DX_PI * 0.75f, 200.0f, -1, 50.0f, 200.0f, 30);

							SetStaging(STAGINGTYPE_WARNING_ATTACK,
								nCntEnemy,
								D3DXVECTOR3(pPlayer->pos.x + 200.0f, pPlayer->pos.y - 200.0f, 0.0f),
								60, ATTACKTYPE_ICICLECLAW, D3DX_PI * 0.75f, 200.0f, -1, 50.0f, 200.0f, 30);

							SetStaging(STAGINGTYPE_WARNING_ATTACK,
								nCntEnemy,
								D3DXVECTOR3(pPlayer->pos.x - 200.0f, pPlayer->pos.y + 100.0f, 0.0f),
								60, ATTACKTYPE_ICICLECLAW, -D3DX_PI * 0.25f, 200.0f, -1, 50.0f, 200.0f, 30);

							SetStaging(STAGINGTYPE_WARNING_ATTACK,
								nCntEnemy,
								D3DXVECTOR3(pPlayer->pos.x + 200.0f, pPlayer->pos.y + 100.0f, 0.0f),
								60, ATTACKTYPE_ICICLECLAW, D3DX_PI * 0.25f, 200.0f, -1, 50.0f, 200.0f, 30);
						}

						if (g_aEnemy[nCntEnemy].nApperCount == 2660)
						{

							SetStaging(STAGINGTYPE_WARNING_ATTACK,
								nCntEnemy,
								D3DXVECTOR3(pPlayer->pos.x - 200.0f, pPlayer->pos.y - 200.0f, 0.0f),
								60, ATTACKTYPE_ICICLECLAW, -D3DX_PI * 0.75f, 200.0f, -1, 50.0f, 200.0f, 30);

							SetStaging(STAGINGTYPE_WARNING_ATTACK,
								nCntEnemy,
								D3DXVECTOR3(pPlayer->pos.x + 200.0f, pPlayer->pos.y - 200.0f, 0.0f),
								60, ATTACKTYPE_ICICLECLAW, D3DX_PI * 0.75f, 200.0f, -1, 50.0f, 200.0f, 30);

							SetStaging(STAGINGTYPE_WARNING_ATTACK,
								nCntEnemy,
								D3DXVECTOR3(pPlayer->pos.x - 200.0f, pPlayer->pos.y + 100.0f, 0.0f),
								60, ATTACKTYPE_ICICLECLAW, -D3DX_PI * 0.25f, 200.0f, -1, 50.0f, 200.0f, 30);

							SetStaging(STAGINGTYPE_WARNING_ATTACK,
								nCntEnemy,
								D3DXVECTOR3(pPlayer->pos.x + 200.0f, pPlayer->pos.y + 100.0f, 0.0f),
								60, ATTACKTYPE_ICICLECLAW, D3DX_PI * 0.25f, 200.0f, -1, 50.0f, 200.0f, 30);
						}

						if (g_aEnemy[nCntEnemy].nApperCount == 2720)
						{

							SetStaging(STAGINGTYPE_WARNING_ATTACK,
								nCntEnemy,
								D3DXVECTOR3(pPlayer->pos.x - 200.0f, pPlayer->pos.y - 200.0f, 0.0f),
								60, ATTACKTYPE_ICICLECLAW, -D3DX_PI * 0.75f, 200.0f, -1, 50.0f, 200.0f, 30);

							SetStaging(STAGINGTYPE_WARNING_ATTACK,
								nCntEnemy,
								D3DXVECTOR3(pPlayer->pos.x + 200.0f, pPlayer->pos.y - 200.0f, 0.0f),
								60, ATTACKTYPE_ICICLECLAW, D3DX_PI * 0.75f, 200.0f, -1, 50.0f, 200.0f, 30);

							SetStaging(STAGINGTYPE_WARNING_ATTACK,
								nCntEnemy,
								D3DXVECTOR3(pPlayer->pos.x - 200.0f, pPlayer->pos.y + 100.0f, 0.0f),
								60, ATTACKTYPE_ICICLECLAW, -D3DX_PI * 0.25f, 200.0f, -1, 50.0f, 200.0f, 30);

							SetStaging(STAGINGTYPE_WARNING_ATTACK,
								nCntEnemy,
								D3DXVECTOR3(pPlayer->pos.x + 200.0f, pPlayer->pos.y + 100.0f, 0.0f),
								60, ATTACKTYPE_ICICLECLAW, D3DX_PI * 0.25f, 200.0f, -1, 50.0f, 200.0f, 30);
						}

						if (g_aEnemy[nCntEnemy].nApperCount == 2780)
						{

							SetStaging(STAGINGTYPE_WARNING_ATTACK,
								nCntEnemy,
								D3DXVECTOR3(pPlayer->pos.x - 200.0f, pPlayer->pos.y - 200.0f, 0.0f),
								60, ATTACKTYPE_ICICLECLAW, -D3DX_PI * 0.75f, 200.0f, -1, 50.0f, 200.0f, 30);

							SetStaging(STAGINGTYPE_WARNING_ATTACK,
								nCntEnemy,
								D3DXVECTOR3(pPlayer->pos.x + 200.0f, pPlayer->pos.y - 200.0f, 0.0f),
								60, ATTACKTYPE_ICICLECLAW, D3DX_PI * 0.75f, 200.0f, -1, 50.0f, 200.0f, 30);

							SetStaging(STAGINGTYPE_WARNING_ATTACK,
								nCntEnemy,
								D3DXVECTOR3(pPlayer->pos.x - 200.0f, pPlayer->pos.y + 100.0f, 0.0f),
								60, ATTACKTYPE_ICICLECLAW, -D3DX_PI * 0.25f, 200.0f, -1, 50.0f, 200.0f, 30);

							SetStaging(STAGINGTYPE_WARNING_ATTACK,
								nCntEnemy,
								D3DXVECTOR3(pPlayer->pos.x + 200.0f, pPlayer->pos.y + 100.0f, 0.0f),
								60, ATTACKTYPE_ICICLECLAW, D3DX_PI * 0.25f, 200.0f, -1, 50.0f, 200.0f, 30);
						}

						if (g_aEnemy[nCntEnemy].nApperCount == 2820)
						{


							SetStaging(STAGINGTYPE_WARNING_ATTACK,
								nCntEnemy,
								D3DXVECTOR3(pPlayer->pos.x - 200.0f, pPlayer->pos.y, 0.0f),
								60, ATTACKTYPE_ICICLECLAW, -D3DX_PI * 0.5f, 200.0f, -1, 50.0f, 200.0f, 30);

							SetStaging(STAGINGTYPE_WARNING_ATTACK,
								nCntEnemy,
								D3DXVECTOR3(pPlayer->pos.x + 200.0f, pPlayer->pos.y, 0.0f),
								60, ATTACKTYPE_ICICLECLAW, D3DX_PI * 0.5f, 200.0f, -1, 50.0f, 200.0f, 30);

							SetStaging(STAGINGTYPE_WARNING_ATTACK,
								nCntEnemy,
								D3DXVECTOR3(pPlayer->pos.x, pPlayer->pos.y - 100.0f, 0.0f),
								60, ATTACKTYPE_ICICLECLAW, -D3DX_PI * 1.0f, 200.0f, -1, 50.0f, 200.0f, 30);

							SetStaging(STAGINGTYPE_WARNING_ATTACK,
								nCntEnemy,
								D3DXVECTOR3(pPlayer->pos.x, pPlayer->pos.y + 100.0f, 0.0f),
								60, ATTACKTYPE_ICICLECLAW, D3DX_PI * 0.0f, 200.0f, -1, 50.0f, 200.0f, 30);

						}


					}

					//===============================
					//�p�^�[���U
					//===============================
					if (g_aEnemy[nCntEnemy].nApperCount == 3000)
					{
						g_aEnemy[nCntEnemy].bGravity = false;

						float fRotBullet_Boss2 = float((rand)() % 200 - 100) / 100.0f;

						g_aEnemy[nCntEnemy].move = D3DXVECTOR3(sinf(-D3DX_PI * fRotBullet_Boss2) * 15.0f, cosf(-D3DX_PI * fRotBullet_Boss2) * 15.0f, 0.0f);
					}

					if (g_aEnemy[nCntEnemy].nApperCount >= 3001 && g_aEnemy[nCntEnemy].nApperCount <= 3500)
					{
						if (g_nDelay % 15 == 0)
						{
							for (int nCnt = 0; nCnt < 4; nCnt++)
							{
								SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].fHeight / 2, 0.0f),
									D3DXVECTOR3(sinf(-D3DX_PI * (0.5f * nCnt)) * 0.2f, cosf(-D3DX_PI * (0.5f * nCnt)) * 0.2f, 0.0f),
									GENERALLY_BULLETLIFE, BULLETTYPE_ICICLE, 0, nCntEnemy, -1, true, MOVETYPE_NORMAL);
							}
						}
					}

					//===============================
					//�p�^�[���V
					//===============================
					if (g_aEnemy[nCntEnemy].nApperCount == 3501)
					{
						g_aEnemy[nCntEnemy].bGravity = true;
						g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
						g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					}

					if (g_aEnemy[nCntEnemy].nApperCount == 3550)
					{
						SetStaging(STAGINGTYPE_WARNING_ATTACK,
							nCntEnemy,
							D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y - 100.0f, 0.0f),
							60, ATTACKTYPE_ICE_EXPLOSION, 0.0f, 100.0f, -1, 100.0f, 100.0f, 30);
					}

					if (g_aEnemy[nCntEnemy].nApperCount == 3570)
					{
						SetStaging(STAGINGTYPE_WARNING_ATTACK,
							nCntEnemy,
							D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 50.0f, g_aEnemy[nCntEnemy].pos.y - 100.0f, 0.0f),
							60, ATTACKTYPE_ICE_EXPLOSION, 0.0f, 100.0f, -1, 150.0f, 150.0f, 30);

						SetStaging(STAGINGTYPE_WARNING_ATTACK,
							nCntEnemy,
							D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 50.0f, g_aEnemy[nCntEnemy].pos.y - 100.0f, 0.0f),
							60, ATTACKTYPE_ICE_EXPLOSION, 0.0f, 100.0f, -1, 150.0f, 150.0f, 30);
					}

					if (g_aEnemy[nCntEnemy].nApperCount == 3590)
					{
						SetStaging(STAGINGTYPE_WARNING_ATTACK,
							nCntEnemy,
							D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 100.0f, g_aEnemy[nCntEnemy].pos.y - 100.0f, 0.0f),
							60, ATTACKTYPE_ICE_EXPLOSION, 0.0f, 100.0f, -1, 200.0f, 200.0f, 30);

						SetStaging(STAGINGTYPE_WARNING_ATTACK,
							nCntEnemy,
							D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 100.0f, g_aEnemy[nCntEnemy].pos.y - 100.0f, 0.0f),
							60, ATTACKTYPE_ICE_EXPLOSION, 0.0f, 100.0f, -1, 200.0f, 200.0f, 30);
					}

					if (g_aEnemy[nCntEnemy].nApperCount == 3610)
					{
						SetStaging(STAGINGTYPE_WARNING_ATTACK,
							nCntEnemy,
							D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 150.0f, g_aEnemy[nCntEnemy].pos.y - 100.0f, 0.0f),
							60, ATTACKTYPE_ICE_EXPLOSION, 0.0f, 100.0f, -1, 250.0f, 250.0f, 30);

						SetStaging(STAGINGTYPE_WARNING_ATTACK,
							nCntEnemy,
							D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 150.0f, g_aEnemy[nCntEnemy].pos.y - 100.0f, 0.0f),
							60, ATTACKTYPE_ICE_EXPLOSION, 0.0f, 100.0f, -1, 250.0f, 250.0f, 30);
					}

					if (g_aEnemy[nCntEnemy].nApperCount == 3630)
					{
						SetStaging(STAGINGTYPE_WARNING_ATTACK,
							nCntEnemy,
							D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 200.0f, g_aEnemy[nCntEnemy].pos.y - 100.0f, 0.0f),
							60, ATTACKTYPE_ICE_EXPLOSION, 0.0f, 100.0f, -1, 300.0f, 300.0f, 30);

						SetStaging(STAGINGTYPE_WARNING_ATTACK,
							nCntEnemy,
							D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 200.0f, g_aEnemy[nCntEnemy].pos.y - 100.0f, 0.0f),
							60, ATTACKTYPE_ICE_EXPLOSION, 0.0f, 100.0f, -1, 300.0f, 300.0f, 30);
					}

					if (g_aEnemy[nCntEnemy].nApperCount == 3650)
					{
						SetStaging(STAGINGTYPE_WARNING_ATTACK,
							nCntEnemy,
							D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 350.0f, g_aEnemy[nCntEnemy].pos.y - 100.0f, 0.0f),
							60, ATTACKTYPE_ICE_EXPLOSION, 0.0f, 100.0f, -1, 350.0f, 350.0f, 30);

						SetStaging(STAGINGTYPE_WARNING_ATTACK,
							nCntEnemy,
							D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 350.0f, g_aEnemy[nCntEnemy].pos.y - 100.0f, 0.0f),
							60, ATTACKTYPE_ICE_EXPLOSION, 0.0f, 100.0f, -1, 350.0f, 350.0f, 30);
					}

					if (g_aEnemy[nCntEnemy].nApperCount == 3670)
					{
						SetStaging(STAGINGTYPE_WARNING_ATTACK,
							nCntEnemy,
							D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 500.0f, g_aEnemy[nCntEnemy].pos.y - 100.0f, 0.0f),
							60, ATTACKTYPE_ICE_EXPLOSION, 0.0f, 100.0f, -1, 250.0f, 250.0f, 30);

						SetStaging(STAGINGTYPE_WARNING_ATTACK,
							nCntEnemy,
							D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 500.0f, g_aEnemy[nCntEnemy].pos.y - 100.0f, 0.0f),
							60, ATTACKTYPE_ICE_EXPLOSION, 0.0f, 100.0f, -1, 250.0f, 250.0f, 30);
					}

					if (g_aEnemy[nCntEnemy].nApperCount == 3690)
					{
						SetStaging(STAGINGTYPE_WARNING_ATTACK,
							nCntEnemy,
							D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 650.0f, g_aEnemy[nCntEnemy].pos.y - 100.0f, 0.0f),
							60, ATTACKTYPE_ICE_EXPLOSION, 0.0f, 100.0f, -1, 200.0f, 200.0f, 30);

						SetStaging(STAGINGTYPE_WARNING_ATTACK,
							nCntEnemy,
							D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 650.0f, g_aEnemy[nCntEnemy].pos.y - 100.0f, 0.0f),
							60, ATTACKTYPE_ICE_EXPLOSION, 0.0f, 100.0f, -1, 200.0f, 200.0f, 30);
					}

					if (g_aEnemy[nCntEnemy].nApperCount == 3710)
					{
						SetStaging(STAGINGTYPE_WARNING_ATTACK,
							nCntEnemy,
							D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 800.0f, g_aEnemy[nCntEnemy].pos.y - 100.0f, 0.0f),
							60, ATTACKTYPE_ICE_EXPLOSION, 0.0f, 100.0f, -1, 150.0f, 150.0f, 30);

						SetStaging(STAGINGTYPE_WARNING_ATTACK,
							nCntEnemy,
							D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 800.0f, g_aEnemy[nCntEnemy].pos.y - 100.0f, 0.0f),
							60, ATTACKTYPE_ICE_EXPLOSION, 0.0f, 100.0f, -1, 150.0f, 150.0f, 30);
					}

					if (g_aEnemy[nCntEnemy].nApperCount == 3730)
					{
						SetStaging(STAGINGTYPE_WARNING_ATTACK,
							nCntEnemy,
							D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 950.0f, g_aEnemy[nCntEnemy].pos.y - 100.0f, 0.0f),
							60, ATTACKTYPE_ICE_EXPLOSION, 0.0f, 100.0f, -1, 100.0f, 100.0f, 30);

						SetStaging(STAGINGTYPE_WARNING_ATTACK,
							nCntEnemy,
							D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 950.0f, g_aEnemy[nCntEnemy].pos.y - 100.0f, 0.0f),
							60, ATTACKTYPE_ICE_EXPLOSION, 0.0f, 100.0f, -1, 100.0f, 100.0f, 30);
					}

					//================================
					//�p�^�[���W
					//================================

					if (g_aEnemy[nCntEnemy].nApperCount >= 3800 && g_aEnemy[nCntEnemy].nApperCount <= 4300)
					{

						if (g_nDelay % 50 == 0)
						{
							/*	float fCircleBullet[6];*/
							float fRandumPosX;
							float fRandumPosY;

							fRandumPosX = float((rand)() % SCREEN_WIDTH + 1);
							fRandumPosY = float((rand)() % SCREEN_HEIGHT + 1);


							PlaySound(SOUND_LABEL_SE_WARNING_001);
							SetStaging(STAGINGTYPE_BULLETWARNING, -1, D3DXVECTOR3(fRandumPosX, fRandumPosY, 0.0f), 180, ATTACKTYPE_PUNCH, 0.0f, 20.0f, -1.0f, -1.0f, -1.0f, -1.0f);

						}

						if (g_aEnemy[nCntEnemy].bDirection == false)
						{
							g_aEnemy[nCntEnemy].move.x = 5.0f;
						}
						else if (g_aEnemy[nCntEnemy].bDirection == true)
						{
							g_aEnemy[nCntEnemy].move.x = -5.0f;
						}
						if (pPlayer->pos.x >= g_aEnemy[nCntEnemy].pos.x - 200.0f && pPlayer->pos.x <= g_aEnemy[nCntEnemy].pos.x + 200.0f &&
							pPlayer->pos.y - pPlayer->fHeight / 2 >= g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].fHeight - 200.0f &&
							pPlayer->pos.y - pPlayer->fHeight / 2 <= g_aEnemy[nCntEnemy].pos.y + 200.0f)
						{
							if (g_nDelay % 30 == 0)
							{
								SetStaging(STAGINGTYPE_WARNING_ATTACK,
									nCntEnemy,
									D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 100.0f, g_aEnemy[nCntEnemy].pos.y - 720.0f, 0.0f),
									60, ATTACKTYPE_ICICLECLAW, D3DX_PI * 1.0f, 100.0f, -1, 50.0f, 600.0f, 530);

								SetStaging(STAGINGTYPE_WARNING_ATTACK,
									nCntEnemy,
									D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 100.0f, g_aEnemy[nCntEnemy].pos.y - 720.0f, 0.0f),
									60, ATTACKTYPE_ICICLECLAW, D3DX_PI * 1.0f, 100.0f, -1, 50.0f, 600.0f, 530);
							}
						}
						else
						{//��L�̍U���͈͊O

							if (g_nDelay % 45 == 0)
							{
								SetStaging(STAGINGTYPE_WARNING_ATTACK,
									nCntEnemy,
									D3DXVECTOR3(pPlayer->pos.x, pPlayer->pos.y, 0.0f),
									60, ATTACKTYPE_ICE_EXPLOSION, 0.0f, 150.0f, -1, 100.0f, 100.0f, 20);

								SetStaging(STAGINGTYPE_WARNING_ATTACK,
									nCntEnemy,
									D3DXVECTOR3(pPlayer->pos.x - 150.0f, pPlayer->pos.y - 150.0f, 0.0f),
									60, ATTACKTYPE_ICE_EXPLOSION, 0.0f, 150.0f, -1, 100.0f, 100.0f, 20);

								SetStaging(STAGINGTYPE_WARNING_ATTACK,
									nCntEnemy,
									D3DXVECTOR3(pPlayer->pos.x + 150.0f, pPlayer->pos.y - 150.0f, 0.0f),
									60, ATTACKTYPE_ICE_EXPLOSION, 0.0f, 150.0f, -1, 100.0f, 100.0f, 20);
							}

						}
					}
					//====================
					//�ȍ~���[�v
					//====================
					if (g_aEnemy[nCntEnemy].nApperCount >= 4400)
					{
						g_aEnemy[nCntEnemy].nApperCount = 0;

						g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					}

				}
		    //============================================
			//�{�X�Q�K�E�Z�����i�p�^�[���P�`�R�j
			//============================================
						if (g_aEnemy[nCntEnemy].bSpecialAttack == true)
						{
							//==================================
							//�p�^�[���P
							//==================================
							if (g_aEnemy[nCntEnemy].nApperCount == 1)
							{
								g_aEnemy[nCntEnemy].nType = ENEMYTYPE_BOSS2_ATTACK;//�U�����[�V�����ɕύX
								g_aEnemy[nCntEnemy].bGravity = false;//�d�͂̌��ʂ���������
								g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
								g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
							}

							if (g_aEnemy[nCntEnemy].nApperCount >= 50 && g_aEnemy[nCntEnemy].nApperCount <= 600)
							{
								float fRandumPosX;
								float fRandumPosY;
								int nRandumRot;
								nRandumRot = rand() % 2 + 1;
								fRandumPosX = float((rand)() % SCREEN_WIDTH + 1);

								if (g_nDelay % 2 == 0)
								{
									if (nRandumRot == 1)
									{
										SetBullet(D3DXVECTOR3(fRandumPosX, 5.0f, 0.0f), D3DXVECTOR3(sinf(-D3DX_PI * 0.2f) * 0.1f, cosf(-D3DX_PI * 0.2f) * 0.1f, 0.0f),
											GENERALLY_BULLETLIFE, BULLETTYPE_ICICLE, -1, -1, -1, false, MOVETYPE_NORMAL);
									}
									else
									{
										SetBullet(D3DXVECTOR3(fRandumPosX, 5.0f, 0.0f), D3DXVECTOR3(sinf(-D3DX_PI * -0.2f) * 0.1f, cosf(-D3DX_PI * -0.2f) * 0.1f, 0.0f),
											GENERALLY_BULLETLIFE, BULLETTYPE_ICICLE, -1, -1, -1, false, MOVETYPE_NORMAL);
									}
								}

							}

							//========================================
							//�p�^�[���Q
							//========================================
							if (g_aEnemy[nCntEnemy].nApperCount >= 601 && g_aEnemy[nCntEnemy].nApperCount <= 1200)
							{

								//==========================
								//�s���P
								//==========================
								if (g_aEnemy[nCntEnemy].nApperCount == 660)
								{
									for (int nCnt = 0; nCnt < 5; nCnt++)
									{
										SetStaging(STAGINGTYPE_WARNING_ATTACK,
											nCntEnemy,
											D3DXVECTOR3(256.0f * nCnt ,SCREEN_HEIGHT - 100.0f, 0.0f),
											60, ATTACKTYPE_ICE_EXPLOSION, 0.0f, 100.0f, -1, 200.0f, 200.0f,10);
									}
									for (int nCnt = 0; nCnt < 5; nCnt++)
									{
										SetStaging(STAGINGTYPE_WARNING_ATTACK,
											nCntEnemy,
											D3DXVECTOR3(128.0f + 256.0f * nCnt,SCREEN_HEIGHT - 300.0f, 0.0f),
											60, ATTACKTYPE_ICE_EXPLOSION, 0.0f, 100.0f, -1, 200.0f, 200.0f, 10);
									}
								}
								if (g_aEnemy[nCntEnemy].nApperCount == 780)
								{
									for (int nCnt = 0; nCnt < 5; nCnt++)
									{
										SetStaging(STAGINGTYPE_WARNING_ATTACK,
											nCntEnemy,
											D3DXVECTOR3(256.0f * nCnt, SCREEN_HEIGHT - 100.0f, 0.0f),
											60, ATTACKTYPE_ICE_EXPLOSION, 0.0f, 100.0f, -1, 200.0f, 200.0f, 10);
									}
									for (int nCnt = 0; nCnt < 5; nCnt++)
									{
										SetStaging(STAGINGTYPE_WARNING_ATTACK,
											nCntEnemy,
											D3DXVECTOR3(128.0f + 256.0f * nCnt, SCREEN_HEIGHT - 300.0f, 0.0f),
											60, ATTACKTYPE_ICE_EXPLOSION, 0.0f, 100.0f, -1, 200.0f, 200.0f, 10);
									}
								}
								if (g_aEnemy[nCntEnemy].nApperCount == 900)
								{
									for (int nCnt = 0; nCnt < 5; nCnt++)
									{
										SetStaging(STAGINGTYPE_WARNING_ATTACK,
											nCntEnemy,
											D3DXVECTOR3(256.0f * nCnt, SCREEN_HEIGHT - 100.0f, 0.0f),
											60, ATTACKTYPE_ICE_EXPLOSION, 0.0f, 100.0f, -1, 200.0f, 200.0f, 10);
									}
									for (int nCnt = 0; nCnt < 5; nCnt++)
									{
										SetStaging(STAGINGTYPE_WARNING_ATTACK,
											nCntEnemy,
											D3DXVECTOR3(128.0f + 256.0f * nCnt, SCREEN_HEIGHT - 300.0f, 0.0f),
											60, ATTACKTYPE_ICE_EXPLOSION, 0.0f, 100.0f, -1, 200.0f, 200.0f, 10);
									}
								}
								if (g_aEnemy[nCntEnemy].nApperCount == 1020)
								{
									for (int nCnt = 0; nCnt < 5; nCnt++)
									{
										SetStaging(STAGINGTYPE_WARNING_ATTACK,
											nCntEnemy,
											D3DXVECTOR3(256.0f * nCnt, SCREEN_HEIGHT - 100.0f, 0.0f),
											60, ATTACKTYPE_ICE_EXPLOSION, 0.0f, 100.0f, -1, 200.0f, 200.0f, 10);
									}
									for (int nCnt = 0; nCnt < 5; nCnt++)
									{
										SetStaging(STAGINGTYPE_WARNING_ATTACK,
											nCntEnemy,
											D3DXVECTOR3(128.0f + 256.0f * nCnt, SCREEN_HEIGHT - 300.0f, 0.0f),
											60, ATTACKTYPE_ICE_EXPLOSION, 0.0f, 100.0f, -1, 200.0f, 200.0f, 10);
									}
								}
								if (g_aEnemy[nCntEnemy].nApperCount == 1140)
								{
									for (int nCnt = 0; nCnt < 5; nCnt++)
									{
										SetStaging(STAGINGTYPE_WARNING_ATTACK,
											nCntEnemy,
											D3DXVECTOR3(256.0f * nCnt, SCREEN_HEIGHT -100.0f, 0.0f),
											60, ATTACKTYPE_ICE_EXPLOSION, 0.0f, 100.0f, -1, 200.0f, 200.0f, 10);
									}
									for (int nCnt = 0; nCnt < 5; nCnt++)
									{
										SetStaging(STAGINGTYPE_WARNING_ATTACK,
											nCntEnemy,
											D3DXVECTOR3(128.0f + 256.0f * nCnt, SCREEN_HEIGHT - 300.0f, 0.0f),
											60, ATTACKTYPE_ICE_EXPLOSION, 0.0f, 100.0f, -1, 200.0f, 200.0f, 10);
									}
								}
								//==========================
								//�s���Q
								//==========================
								if (g_aEnemy[nCntEnemy].nApperCount == 655)
								{
									SetStaging(STAGINGTYPE_WARNING_ATTACK,
								nCntEnemy,
								D3DXVECTOR3(pPlayer->pos.x - 200.0f, pPlayer->pos.y - 50.0f, 0.0f),
								60, ATTACKTYPE_ICICLECLAW, -D3DX_PI * 0.5f, 200.0f, -1, 50.0f, 200.0f, 30);

							SetStaging(STAGINGTYPE_WARNING_ATTACK,
								nCntEnemy,
								D3DXVECTOR3(pPlayer->pos.x + 200.0f, pPlayer->pos.y - 50.0f, 0.0f),
								60, ATTACKTYPE_ICICLECLAW, D3DX_PI * 0.5f, 200.0f, -1, 50.0f, 200.0f, 30);

							SetStaging(STAGINGTYPE_WARNING_ATTACK,
								nCntEnemy,
								D3DXVECTOR3(pPlayer->pos.x, pPlayer->pos.y - 100.0f - 50.0f, 0.0f),
								60, ATTACKTYPE_ICICLECLAW, -D3DX_PI * 1.0f, 200.0f, -1, 50.0f, 200.0f, 30);

							SetStaging(STAGINGTYPE_WARNING_ATTACK,
								nCntEnemy,
								D3DXVECTOR3(pPlayer->pos.x, pPlayer->pos.y + 100.0f - 50.0f, 0.0f),
								60, ATTACKTYPE_ICICLECLAW, D3DX_PI * 0.0f, 200.0f, -1, 50.0f, 200.0f, 30);

							SetStaging(STAGINGTYPE_WARNING_ATTACK,
								nCntEnemy,
								D3DXVECTOR3(pPlayer->pos.x - 200.0f, pPlayer->pos.y + 50.0f, 0.0f),
								60, ATTACKTYPE_ICICLECLAW, -D3DX_PI * 0.5f, 200.0f, -1, 50.0f, 200.0f, 30);

							SetStaging(STAGINGTYPE_WARNING_ATTACK,
								nCntEnemy,
								D3DXVECTOR3(pPlayer->pos.x + 200.0f, pPlayer->pos.y + 50.0f, 0.0f),
								60, ATTACKTYPE_ICICLECLAW, D3DX_PI * 0.5f, 200.0f, -1, 50.0f, 200.0f, 30);

							SetStaging(STAGINGTYPE_WARNING_ATTACK,
								nCntEnemy,
								D3DXVECTOR3(pPlayer->pos.x, pPlayer->pos.y - 100.0f + 50.0f, 0.0f),
								60, ATTACKTYPE_ICICLECLAW, -D3DX_PI * 1.0f, 200.0f, -1, 50.0f, 200.0f, 30);

							SetStaging(STAGINGTYPE_WARNING_ATTACK,
								nCntEnemy,
								D3DXVECTOR3(pPlayer->pos.x, pPlayer->pos.y + 100.0f + 50.0f, 0.0f),
								60, ATTACKTYPE_ICICLECLAW, D3DX_PI * 0.0f, 200.0f, -1, 50.0f, 200.0f, 30);

								}

								if (g_aEnemy[nCntEnemy].nApperCount == 750)
								{
									for (int nCnt = 0; nCnt < 3; nCnt++)
									{
										SetStaging(STAGINGTYPE_WARNING_ATTACK,
											nCntEnemy,
											D3DXVECTOR3(pPlayer->pos.x - 200.0f, pPlayer->pos.y - 50.0f * nCnt, 0.0f),
											60, ATTACKTYPE_ICICLECLAW, -D3DX_PI * 0.5f, 200.0f, -1, 50.0f, 200.0f, 15);

										SetStaging(STAGINGTYPE_WARNING_ATTACK,
											nCntEnemy,
											D3DXVECTOR3(pPlayer->pos.x + 200.0f, pPlayer->pos.y - 50.0f * nCnt, 0.0f),
											60, ATTACKTYPE_ICICLECLAW, D3DX_PI * 0.5f, 200.0f, -1, 50.0f, 200.0f, 15);
									}

								}

								if (g_aEnemy[nCntEnemy].nApperCount == 860)
								{
									int nCntBullet_Boss2;
									float fCircleBullet_Boss2[10];
									float fRotBullet_Boss2;
									fRotBullet_Boss2 = float((rand)() % 200) / 100.0f;

									PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_004);

								
										for (nCntBullet_Boss2 = 0; nCntBullet_Boss2 < 10; nCntBullet_Boss2++)

										{
											fCircleBullet_Boss2[nCntBullet_Boss2] = (1.0f / 5.0f) * nCntBullet_Boss2;

											SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].fHeight / 2, 0.0f),
												D3DXVECTOR3((sinf(-D3DX_PI * (fCircleBullet_Boss2[nCntBullet_Boss2] + fRotBullet_Boss2)) * 4.0f), (cosf(-D3DX_PI * (fCircleBullet_Boss2[nCntBullet_Boss2] + fRotBullet_Boss2)) * 4.0f), 0.0f),
												GENERALLY_BULLETLIFE, BULLETTYPE_SNOW, 0, nCntEnemy, -1, true, MOVETYPE_NORMAL);

											SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].fHeight / 2, 0.0f),
												D3DXVECTOR3((sinf(-D3DX_PI * (fCircleBullet_Boss2[nCntBullet_Boss2] + fRotBullet_Boss2)) * 3.0f), (cosf(-D3DX_PI * (fCircleBullet_Boss2[nCntBullet_Boss2] + fRotBullet_Boss2)) * 3.0f), 0.0f),
												GENERALLY_BULLETLIFE, BULLETTYPE_SNOW, 0, nCntEnemy, -1, true, MOVETYPE_NORMAL);

											SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].fHeight / 2, 0.0f),
												D3DXVECTOR3((sinf(-D3DX_PI * (fCircleBullet_Boss2[nCntBullet_Boss2] + fRotBullet_Boss2)) * 2.0f), (cosf(-D3DX_PI * (fCircleBullet_Boss2[nCntBullet_Boss2] + fRotBullet_Boss2)) * 2.0f), 0.0f),
												GENERALLY_BULLETLIFE, BULLETTYPE_SNOW, 0, nCntEnemy, -1, true, MOVETYPE_NORMAL);

										}
									
								}

								if (g_aEnemy[nCntEnemy].nApperCount >= 1000 && g_aEnemy[nCntEnemy].nApperCount <= 1100)
								{
									g_fXVaim[nCntEnemy] = pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x;//X���W�ւ̃x�N�g��
									g_fYVaim[nCntEnemy] = pPlayer->pos.y - g_aEnemy[nCntEnemy].pos.y;//Y���W�ւ̃x�N�g��
									g_Vaim[nCntEnemy] = atan2(g_fXVaim[nCntEnemy], g_fYVaim[nCntEnemy]);//�x�N�g���̌���

									SetStaging(STAGINGTYPE_WARNING_ATTACK,
										nCntEnemy,
										g_aEnemy[nCntEnemy].pos,
										60, ATTACKTYPE_ICICLECLAW,g_Vaim[nCntEnemy] + D3DX_PI, 200.0f, -1, 50.0f, 700.0f, 20);

									if (g_nDelay % 5 == 0)
									{
										SetStaging(STAGINGTYPE_NORMAL_WARNINGS,
											nCntEnemy,
											pPlayer->pos,
											100, ATTACKTYPE_ICICLECLAW, -g_Vaim[nCntEnemy], 100.0f, -1, 50.0f, 700.0f, 20);
									}
								
								}


							}

							//=================================
							//�p�^�[���R
							//=================================
							if (g_aEnemy[nCntEnemy].nApperCount >= 1300 && g_aEnemy[nCntEnemy].nApperCount <= 1800)
							{
								if (g_aEnemy[nCntEnemy].nApperCount == 1350)
								{
									SetStaging(STAGINGTYPE_NORMAL_WARNINGS,
										nCntEnemy,
										D3DXVECTOR3(SCREEN_WIDTH / 2 - 300.0f, SCREEN_HEIGHT / 2, 0.0f),
										120, ATTACKTYPE_ICICLECLAW, -D3DX_PI * 1.0f, 500.0f, -1, 50.0f, 380.0f, 40);
									for (int nCnt = 0; nCnt < 20; nCnt++)
									{
										
										if (nCnt >= 0 && nCnt <= 9)
										{
											SetStaging(STAGINGTYPE_WARNING_ATTACK,
												nCntEnemy,
												D3DXVECTOR3(64.0f * nCnt, SCREEN_HEIGHT / 2, 0.0f),
												120, ATTACKTYPE_ICICLECLAW, -D3DX_PI * 1.0f, 200.0f, -1, 50.0f, 380.0f, 40);
										}
										else
										{
											SetStaging(STAGINGTYPE_WARNING_ATTACK,
												nCntEnemy,
												D3DXVECTOR3(64.0f * nCnt, SCREEN_HEIGHT / 2, 0.0f),
												120, ATTACKTYPE_ICICLECLAW, -D3DX_PI * 0.0f, 200.0f, -1, 50.0f, 380.0f, 40);
										}
									}
								}

								if (g_aEnemy[nCntEnemy].nApperCount == 1600)
								{

									SetStaging(STAGINGTYPE_NORMAL_WARNINGS,
										nCntEnemy,
										D3DXVECTOR3(SCREEN_WIDTH / 2 + 300.0f, SCREEN_HEIGHT / 2, 0.0f),
										120, ATTACKTYPE_ICICLECLAW, -D3DX_PI * 1.0f, 500.0f, -1, 50.0f, 380.0f, 40);
									for (int nCnt = 0; nCnt < 20; nCnt++)
									{
										if (nCnt >= 0 && nCnt <= 9)
										{
											SetStaging(STAGINGTYPE_WARNING_ATTACK,
												nCntEnemy,
												D3DXVECTOR3(64.0f * nCnt, SCREEN_HEIGHT / 2, 0.0f),
												120, ATTACKTYPE_ICICLECLAW, -D3DX_PI * 0.0f, 200.0f, -1, 50.0f, 380.0f, 40);
										}
										else
										{
											SetStaging(STAGINGTYPE_WARNING_ATTACK,
												nCntEnemy,
												D3DXVECTOR3(64.0f * nCnt, SCREEN_HEIGHT / 2, 0.0f),
												120, ATTACKTYPE_ICICLECLAW, -D3DX_PI * 1.0f, 200.0f, -1, 50.0f, 380.0f, 40);
										}
									}
								}

								if (g_aEnemy[nCntEnemy].nApperCount == 1780)
								{
									SetStaging(STAGINGTYPE_NORMAL_WARNINGS,
										nCntEnemy,
										D3DXVECTOR3(SCREEN_WIDTH / 2 + 350.0f, SCREEN_HEIGHT / 2, 0.0f),
										150, ATTACKTYPE_ICICLECLAW, -D3DX_PI * 1.0f, 400.0f, -1, 50.0f, 380.0f, 40);

									SetStaging(STAGINGTYPE_NORMAL_WARNINGS,
										nCntEnemy,
										D3DXVECTOR3(SCREEN_WIDTH / 2 - 350.0f, SCREEN_HEIGHT / 2, 0.0f),
										150, ATTACKTYPE_ICICLECLAW, -D3DX_PI * 1.0f, 400.0f, -1, 50.0f, 380.0f, 40);
								}

								
							}

							if (g_aEnemy[nCntEnemy].nApperCount == 1900)
							{
								SetAttack(D3DXVECTOR3(SCREEN_WIDTH / 2 - 400.0f,SCREEN_HEIGHT, 0.0f),
									D3DXVECTOR3(0.0f, 0.0f, 0.0f),
									ATTACKTYPE_ABSOLUTE_BLIZZARD,
									-1, nCntEnemy, -1, MOVETYPE_NORMAL, 600.0f, 800.0f, 240, 0.0f, -1);

								SetAttack(D3DXVECTOR3(SCREEN_WIDTH / 2 + 400.0f, SCREEN_HEIGHT, 0.0f),
									D3DXVECTOR3(0.0f, 0.0f, 0.0f),
									ATTACKTYPE_ABSOLUTE_BLIZZARD,
									-1, nCntEnemy, -1, MOVETYPE_NORMAL, 600.0f, 800.0f, 240, 0.0f, -1);
							}

					
							//============================
							//�ʏ��Ԃɖ߂�
							//============================
							if (g_aEnemy[nCntEnemy].nApperCount == 2201)
							{
								g_aEnemy[nCntEnemy].bSpecialAttack = false;
								g_aEnemy[nCntEnemy].nApperCount = 0;
								g_aEnemy[nCntEnemy].bGravity = true;
							}		
								  
						}
							



						
					//====================================================================================================================
				
			//===============================}==========
			//�e�N�X�`���A�j���[�V��������
			//=======================================
					if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS2_ATTACK)
					{
						g_aEnemy[nCntEnemy].nCntAnim++;//�A�j���[�V�����J�E���^�[
						if (g_aEnemy[nCntEnemy].nCntAnim >= 5)
						{//�A�j���[�V�����̃X�s�[�h��������
							g_aEnemy[nCntEnemy].nPatternAnim++;
							g_aEnemy[nCntEnemy].nCntAnim = 0;
						}

						if (g_aEnemy[nCntEnemy].nPatternAnim >= 5)
						{//���[�v�p����
							g_aEnemy[nCntEnemy].nPatternAnim = 0;
						}

						if (g_aEnemy[nCntEnemy].bDirection == false)
						{
							pVtx[0].tex = D3DXVECTOR2(0.0f + g_aEnemy[nCntEnemy].nPatternAnim * 0.2f, 0.0f);
							pVtx[1].tex = D3DXVECTOR2(0.2f + g_aEnemy[nCntEnemy].nPatternAnim * 0.2f, 0.0f);
							pVtx[2].tex = D3DXVECTOR2(0.0f + g_aEnemy[nCntEnemy].nPatternAnim * 0.2f, 1.0f);
							pVtx[3].tex = D3DXVECTOR2(0.2f + g_aEnemy[nCntEnemy].nPatternAnim * 0.2f, 1.0f);
						}
						else if (g_aEnemy[nCntEnemy].bDirection == true)
						{
							pVtx[1].tex = D3DXVECTOR2(0.0f + g_aEnemy[nCntEnemy].nPatternAnim * 0.2f, 0.0f);
							pVtx[0].tex = D3DXVECTOR2(0.2f + g_aEnemy[nCntEnemy].nPatternAnim * 0.2f, 0.0f);
							pVtx[3].tex = D3DXVECTOR2(0.0f + g_aEnemy[nCntEnemy].nPatternAnim * 0.2f, 1.0f);
							pVtx[2].tex = D3DXVECTOR2(0.2f + g_aEnemy[nCntEnemy].nPatternAnim * 0.2f, 1.0f);
						}
					}
					else if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS2_RUN)
					{//�ړ����[�V����
						g_aEnemy[nCntEnemy].nCntAnim++;//�A�j���[�V�����J�E���^�[
						if (g_aEnemy[nCntEnemy].nCntAnim >= 5)
						{
							g_aEnemy[nCntEnemy].nPatternAnim++;
							g_aEnemy[nCntEnemy].nCntAnim = 0;
						}

						if (g_aEnemy[nCntEnemy].nPatternAnim >= 8)
						{
							g_aEnemy[nCntEnemy].nPatternAnim = 0;
						}

						if (g_aEnemy[nCntEnemy].bDirection == false)
						{
							pVtx[0].tex = D3DXVECTOR2(0.0f + g_aEnemy[nCntEnemy].nPatternAnim * 0.125f, 0.0f);
							pVtx[1].tex = D3DXVECTOR2(0.125f + g_aEnemy[nCntEnemy].nPatternAnim * 0.125f, 0.0f);
							pVtx[2].tex = D3DXVECTOR2(0.0f + g_aEnemy[nCntEnemy].nPatternAnim * 0.125f, 1.0f);
							pVtx[3].tex = D3DXVECTOR2(0.125f + g_aEnemy[nCntEnemy].nPatternAnim * 0.125f, 1.0f);
						}
						else if (g_aEnemy[nCntEnemy].bDirection == true)
						{
							pVtx[1].tex = D3DXVECTOR2(0.0f + g_aEnemy[nCntEnemy].nPatternAnim * 0.125f, 0.0f);
							pVtx[0].tex = D3DXVECTOR2(0.125f + g_aEnemy[nCntEnemy].nPatternAnim * 0.125f, 0.0f);
							pVtx[3].tex = D3DXVECTOR2(0.0f + g_aEnemy[nCntEnemy].nPatternAnim * 0.125f, 1.0f);
							pVtx[2].tex = D3DXVECTOR2(0.125f + g_aEnemy[nCntEnemy].nPatternAnim * 0.125f, 1.0f);
						}
					}
					else if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS2_STAND)
					{//�ҋ@���[�V����
						g_aEnemy[nCntEnemy].nCntAnim++;//�A�j���[�V�����J�E���^�[
						if (g_aEnemy[nCntEnemy].nCntAnim >= 10)
						{
							g_aEnemy[nCntEnemy].nPatternAnim++;
							g_aEnemy[nCntEnemy].nCntAnim = 0;
						}

						if (g_aEnemy[nCntEnemy].nPatternAnim >= 4)
						{
							g_aEnemy[nCntEnemy].nPatternAnim = 0;
						}

						if (g_aEnemy[nCntEnemy].bDirection == false)
						{
							pVtx[0].tex = D3DXVECTOR2(0.0f + g_aEnemy[nCntEnemy].nPatternAnim * 0.25f, 0.0f);
							pVtx[1].tex = D3DXVECTOR2(0.25f + g_aEnemy[nCntEnemy].nPatternAnim * 0.25f, 0.0f);
							pVtx[2].tex = D3DXVECTOR2(0.0f + g_aEnemy[nCntEnemy].nPatternAnim * 0.25f, 1.0f);
							pVtx[3].tex = D3DXVECTOR2(0.25f + g_aEnemy[nCntEnemy].nPatternAnim * 0.25f, 1.0f);
						}
						else if (g_aEnemy[nCntEnemy].bDirection == true)
						{
							pVtx[1].tex = D3DXVECTOR2(0.0f + g_aEnemy[nCntEnemy].nPatternAnim * 0.25f, 0.0f);
							pVtx[0].tex = D3DXVECTOR2(0.25f + g_aEnemy[nCntEnemy].nPatternAnim * 0.25f, 0.0f);
							pVtx[3].tex = D3DXVECTOR2(0.0f + g_aEnemy[nCntEnemy].nPatternAnim * 0.25f, 1.0f);
							pVtx[2].tex = D3DXVECTOR2(0.25f + g_aEnemy[nCntEnemy].nPatternAnim * 0.25f, 1.0f);
						}
					}

			
				
			}
			//=========================================================
			//�{�X�R�̏���
			//=========================================================
			if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS3_STAND ||
				g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS3_RUN ||
				g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS3_ATTACK)
			{
				g_aEnemy[nCntEnemy].nCountMoveInterval++;
				g_aEnemy[nCntEnemy].nApperCount++;//�G���o�����Ă���̎��Ԃ��J�E���g����B

				if (g_aEnemy[nCntEnemy].nCountMoveInterval == 1)
				{
					StopSound(SOUND_LABEL_BGM008);
					PlaySound(SOUND_LABEL_BGM004);
					SetGage(nCntEnemy, GAGETYPE_BOSS1HP);
					SetGage(-1, GAGETYPE_BOSSHP_FRAME);
				}

			
				//================================
				//�`�ԕω�����
				//================================
				if (g_aEnemy[nCntEnemy].fLife <= g_aEnemy[nCntEnemy].fMaxHp / 4)
				{//�c��HP���ő�HP�̂S���̂P�ɂȂ�����
					if (g_aEnemy[nCntEnemy].bSpecialStaging == false)
					{
						PlaySound(SOUND_LABEL_SE_SPECIALATTACK);
						g_aEnemy[nCntEnemy].bSpecialAttack = true;
						g_aEnemy[nCntEnemy].bSpecialStaging = true;
						/*SetStaging(STAGINGTYPE_BOSS3_SPECIAL_ATTACK, -1, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 180, ATTACKTYPE_PUNCH, 0.0f, -1, -1,
							-1, -1, 150);*/
						SetSpecial(SPECIALTYPE_BOSS3_SPECIAL_ATTACK, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 150);
						g_aEnemy[nCntEnemy].nApperCount = 0;
					}
				}

				//===============================================
				//�ʏ�U�������i�p�^�[���P�`�V�j
				//===============================================

				//================
				//�p�^�[���P
				//================


				if (g_aEnemy[nCntEnemy].bSpecialAttack == false)
				{
					if (g_aEnemy[nCntEnemy].nApperCount >= 100 && g_aEnemy[nCntEnemy].nApperCount <= 200)
					{
						if (g_nDelay % 10 == 0)
						{
							SetStaging(STAGINGTYPE_WARNING_ATTACK, -1, pPlayer->pos, 60, ATTACKTYPE_FIRE, 0.0f, 50, -1,
								60.0f, 60.0f, 500);
						}
					}


					if (g_aEnemy[nCntEnemy].nApperCount >= 201 && g_aEnemy[nCntEnemy].nApperCount <= 700)
					{
						if (g_aEnemy[nCntEnemy].bDirection == false)
						{
							g_aEnemy[nCntEnemy].move.x += 4.0f;
						}
						else
						{
							g_aEnemy[nCntEnemy].move.x -= 4.0f;
						}
						if (pPlayer->pos.x >= g_aEnemy[nCntEnemy].pos.x - 250.0f && pPlayer->pos.x <= g_aEnemy[nCntEnemy].pos.x + 250.0f &&
							pPlayer->pos.y - pPlayer->fHeight / 2 >= g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].fHeight - 250.0f &&
							pPlayer->pos.y - pPlayer->fHeight / 2 <= g_aEnemy[nCntEnemy].pos.y + 250.0f)
						{
							if (g_nDelay % 30 == 0)
							{
								g_aEnemy[nCntEnemy].nType = ENEMYTYPE_BOSS3_ATTACK;
								if (g_aEnemy[nCntEnemy].bDirection == false)
								{
								
									if (g_nDelay % 60 == 0)
									{
										SetStaging(STAGINGTYPE_WARNING_ATTACK, -1, g_aEnemy[nCntEnemy].pos, 40, ATTACKTYPE_SHAKINGSWORD, 0.0f, 50,-0.1f,
											50.0f, 300.0f, 30);
									}
								}
								else
								{
									g_aEnemy[nCntEnemy].move.x -= 6.0f;

									if (g_nDelay % 60 == 0)
									{
										SetStaging(STAGINGTYPE_WARNING_ATTACK, -1, g_aEnemy[nCntEnemy].pos, 40, ATTACKTYPE_SHAKINGSWORD, 0.0f, 50,0.1f,
											50.0f, 300.0f, 30);
									}
								}

							}
						}
						else
						{
							float fRotBullet_E6_R = float((rand)() % 200 + 100) / 100.0f;
							if (g_nDelay % 20 == 0)
							{
								SetAttack(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].fHeight / 2, 0.0f),
									D3DXVECTOR3(sinf(-D3DX_PI * fRotBullet_E6_R) * 10.0f, cosf(-D3DX_PI * fRotBullet_E6_R) * 10.0f, 0.0f),
									ATTACKTYPE_BOMU,
									-1, nCntEnemy, -1, MOVETYPE_GRAVITY, 140.0f, 140.0f, 200, 0.0f, -1);
							}
						}



					}

					//===================
					//�p�^�[���Q
					//===================

					if (g_aEnemy[nCntEnemy].nApperCount == 780)
					{
						g_aEnemy[nCntEnemy].nType = ENEMYTYPE_BOSS3_ATTACK;

						g_fXVaim[nCntEnemy] = SCREEN_WIDTH / 2 - g_aEnemy[nCntEnemy].pos.x;//X���W�ւ̃x�N�g��
						g_fYVaim[nCntEnemy] = SCREEN_HEIGHT / 2 - g_aEnemy[nCntEnemy].pos.y;//Y���W�ւ̃x�N�g��
						g_Vaim[nCntEnemy] = atan2(g_fXVaim[nCntEnemy], g_fYVaim[nCntEnemy]);//�e���������̌���


						g_aEnemy[nCntEnemy].bGravity = false;//�d��Off
						g_aEnemy[nCntEnemy].move.x = sinf(g_Vaim[nCntEnemy]) * 15.0f;
						g_aEnemy[nCntEnemy].move.y = cosf(g_Vaim[nCntEnemy]) * 15.0f;
					}
					if (g_aEnemy[nCntEnemy].nApperCount >= 781 && g_aEnemy[nCntEnemy].nApperCount <= 880)
					{
						if (g_aEnemy[nCntEnemy].pos.x >= SCREEN_WIDTH / 2 - 20.0f && g_aEnemy[nCntEnemy].pos.y >= SCREEN_HEIGHT / 2 - 20.0f &&
							g_aEnemy[nCntEnemy].pos.x <= SCREEN_WIDTH / 2 + 20.0f && g_aEnemy[nCntEnemy].pos.y <= SCREEN_HEIGHT / 2 + 20.0f)
						{
							g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						}
					}

					if (g_aEnemy[nCntEnemy].nApperCount == 950)
					{
						int nCntBullet_E5;
						float fCircleBullet_E5_normal[16];

						
							PlaySound(SOUND_LABEL_SE_ARROWSHOT_000);
							for (nCntBullet_E5 = 0; nCntBullet_E5 < 16; nCntBullet_E5++)
							{
								SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_REAFY, nCntBullet_E5, nCntEnemy, -1, false,MOVETYPE_NORMAL);//�G�̒e������G���m�œ�����Ȃ�
							}
						
					}

					if (g_aEnemy[nCntEnemy].nApperCount == 1030)
					{
						int nCntBullet_E5;
						float fCircleBullet_E5_normal[16];


						PlaySound(SOUND_LABEL_SE_ARROWSHOT_000);
						for (nCntBullet_E5 = 0; nCntBullet_E5 < 16; nCntBullet_E5++)
						{
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_REVERSE_CURVE, nCntBullet_E5, nCntEnemy, -1, false, MOVETYPE_NORMAL);//�G�̒e������G���m�œ�����Ȃ�

						}

					}

					//============================
					//�p�^�[���R
					//============================

					if (g_aEnemy[nCntEnemy].nApperCount == 1040)
					{
						g_aEnemy[nCntEnemy].bGravity = true;//�d�̓I��


					}

					if (g_aEnemy[nCntEnemy].nApperCount >= 1100 && g_aEnemy[nCntEnemy].nApperCount <= 1470)
					{
						
						g_fXVaim[nCntEnemy] = pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x;//X���W�ւ̃x�N�g��
						g_fYVaim[nCntEnemy] = pPlayer->pos.y - g_aEnemy[nCntEnemy].pos.y;//Y���W�ւ̃x�N�g��
						g_Vaim[nCntEnemy] = atan2(g_fXVaim[nCntEnemy], g_fYVaim[nCntEnemy]);//�e���������̌���

			
						g_aEnemy[nCntEnemy].bGravity = true;//�d��Off
					

						if (g_aEnemy[nCntEnemy].nApperCount == 1140)
						{
							SetStaging(STAGINGTYPE_WARNING_ATTACK, -1,pPlayer->pos, 40, ATTACKTYPE_FLASHBLADE,-D3DX_PI * 0.5f, 50.0f, 0.0f,
								50.0f, 1200.0f, 30);
						}

						if (g_aEnemy[nCntEnemy].nApperCount == 1170)
						{
					
							if (g_aEnemy[nCntEnemy].bDirection == false)
							{
								g_aEnemy[nCntEnemy].pos.x = SCREEN_WIDTH - 1;
							}
							else if (g_aEnemy[nCntEnemy].bDirection == true)
							{
								g_aEnemy[nCntEnemy].pos.x = 0.0f;
							}

							
						}

						if (g_aEnemy[nCntEnemy].nApperCount == 1171)
						{
							for (int nCnt = 0; nCnt < 5; nCnt++)
							{
								SetAttack(g_aEnemy[nCntEnemy].pos,
									D3DXVECTOR3(sinf(g_Vaim[nCntEnemy]) * 1.0f * nCnt, cosf(g_Vaim[nCntEnemy]) * 1.0f * nCnt, 0.0f),
									ATTACKTYPE_FIRE,
									-1, nCntEnemy, -1, MOVETYPE_NORMAL, 50.0f, 50.0f, 240, 0.0f, -1);
							}
						}


						if (g_aEnemy[nCntEnemy].nApperCount == 1250)
						{
							if (g_aEnemy[nCntEnemy].bDirection == false)
							{
								SetStaging(STAGINGTYPE_WARNING_ATTACK, -1,D3DXVECTOR3(pPlayer->pos.x,SCREEN_HEIGHT - 50.0f,0.0f), 40, ATTACKTYPE_FLASHBLADE, -D3DX_PI * 0.5f, 50.0f, 0.0f,
									50.0f, 1200.0f, 30);
							}
							else
							{
								SetStaging(STAGINGTYPE_WARNING_ATTACK, -1, D3DXVECTOR3(pPlayer->pos.x, SCREEN_HEIGHT - 50.0f, 0.0f), 40, ATTACKTYPE_FLASHBLADE, D3DX_PI * 0.5f, 50.0f, 0.0f,
									50.0f, 1200.0f, 30);
							}
						}

						if (g_aEnemy[nCntEnemy].nApperCount == 1280)
						{


							if (g_aEnemy[nCntEnemy].bDirection == false)
							{
								g_aEnemy[nCntEnemy].pos.x = SCREEN_WIDTH;
							}
							else if (g_aEnemy[nCntEnemy].bDirection == true)
							{
								g_aEnemy[nCntEnemy].pos.x = 0.0f;
							}

							
						}
						if (g_aEnemy[nCntEnemy].nApperCount == 1281)
						{
							for (int nCnt = 0; nCnt < 5; nCnt++)
							{
								SetAttack(g_aEnemy[nCntEnemy].pos,
									D3DXVECTOR3(sinf(g_Vaim[nCntEnemy]) * 1.0f * nCnt, cosf(g_Vaim[nCntEnemy]) * 1.0f * nCnt, 0.0f),
									ATTACKTYPE_FIRE,
									-1, nCntEnemy, -1, MOVETYPE_NORMAL, 50.0f, 50.0f, 240, 0.0f, -1);
							}
						}

						if (g_aEnemy[nCntEnemy].nApperCount == 1310)
						{
							if (g_aEnemy[nCntEnemy].bDirection == false)
							{
								SetStaging(STAGINGTYPE_WARNING_ATTACK, -1, pPlayer->pos, 40, ATTACKTYPE_FLASHBLADE, -D3DX_PI * 0.5f, 50.0f, 0.0f,
									50.0f, 1200.0f, 30);
							}
							else
							{
								SetStaging(STAGINGTYPE_WARNING_ATTACK, -1, pPlayer->pos, 40, ATTACKTYPE_FLASHBLADE, D3DX_PI * 0.5f, 50.0f, 0.0f,
									50.0f, 1200.0f, 30);
							}
						}

						if (g_aEnemy[nCntEnemy].nApperCount == 1340)
						{


							if (g_aEnemy[nCntEnemy].bDirection == false)
							{
								g_aEnemy[nCntEnemy].pos.x = SCREEN_WIDTH;
							}
							else if (g_aEnemy[nCntEnemy].bDirection == true)
							{
								g_aEnemy[nCntEnemy].pos.x = 0.0f;
							}

						
						}
						if (g_aEnemy[nCntEnemy].nApperCount == 1341)
						{
							for (int nCnt = 0; nCnt < 5; nCnt++)
							{
								SetAttack(g_aEnemy[nCntEnemy].pos,
									D3DXVECTOR3(sinf(g_Vaim[nCntEnemy]) * 1.0f * nCnt, cosf(g_Vaim[nCntEnemy]) * 1.0f * nCnt, 0.0f),
									ATTACKTYPE_FIRE,
									-1, nCntEnemy, -1, MOVETYPE_NORMAL, 50.0f, 50.0f, 240, 0.0f, -1);
							}
						}

						if (g_aEnemy[nCntEnemy].nApperCount == 1370)
						{
							if (g_aEnemy[nCntEnemy].bDirection == false)
							{
								SetStaging(STAGINGTYPE_WARNING_ATTACK, -1, pPlayer->pos, 40, ATTACKTYPE_FLASHBLADE, -D3DX_PI * 0.5f, 50.0f, 0.0f,
									50.0f, 1200.0f, 30);
							}
							else
							{
								SetStaging(STAGINGTYPE_WARNING_ATTACK, -1, pPlayer->pos, 40, ATTACKTYPE_FLASHBLADE, D3DX_PI * 0.5f, 50.0f, 0.0f,
									50.0f, 1200.0f, 30);
							}
						}

						if (g_aEnemy[nCntEnemy].nApperCount == 1400)
						{


							if (g_aEnemy[nCntEnemy].bDirection == false)
							{
								g_aEnemy[nCntEnemy].pos.x = SCREEN_WIDTH;
							}
							else if (g_aEnemy[nCntEnemy].bDirection == true)
							{
								g_aEnemy[nCntEnemy].pos.x = 0.0f;
							}

						
						}
						if (g_aEnemy[nCntEnemy].nApperCount == 1401)
						{
							for (int nCnt = 0; nCnt < 5; nCnt++)
							{
								SetAttack(g_aEnemy[nCntEnemy].pos,
									D3DXVECTOR3(sinf(g_Vaim[nCntEnemy]) * 1.0f * nCnt, cosf(g_Vaim[nCntEnemy]) * 1.0f * nCnt, 0.0f),
									ATTACKTYPE_FIRE,
									-1, nCntEnemy, -1, MOVETYPE_NORMAL, 50.0f, 50.0f, 240, 0.0f, -1);
							}

							g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						}
					}

					//============================
					//�p�^�[���S
					//============================
					if (g_aEnemy[nCntEnemy].nApperCount >= 1500 && g_aEnemy[nCntEnemy].nApperCount <= 1650)
					{
						g_aEnemy[nCntEnemy].nType = ENEMYTYPE_BOSS3_RUN;
						g_fXVaim[nCntEnemy] = pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x;//X���W�ւ̃x�N�g��
						g_fYVaim[nCntEnemy] = pPlayer->pos.y - g_aEnemy[nCntEnemy].pos.y;//Y���W�ւ̃x�N�g��
						g_Vaim[nCntEnemy] = atan2(g_fXVaim[nCntEnemy], g_fYVaim[nCntEnemy]);//�e���������̌���
						g_aEnemy[nCntEnemy].bGravity = true;
						if (g_aEnemy[nCntEnemy].bDirection == false)
						{
							g_aEnemy[nCntEnemy].move.x += 7.0f;
						}
						else
						{
							g_aEnemy[nCntEnemy].move.x -= 7.0f;
						}

						if (g_nDelay % 30 == 0)
						{
							SetAttack(g_aEnemy[nCntEnemy].pos,
								D3DXVECTOR3(sinf(g_Vaim[nCntEnemy]) * 10.0f, cosf(g_Vaim[nCntEnemy]) * 10.0f, 0.0f),
								ATTACKTYPE_VACUUMBLADE,
								-1, nCntEnemy, -1, MOVETYPE_NORMAL, 70.0f, 70.0f, 1000, 0.0f, -1);
						}
					}

					if (g_aEnemy[nCntEnemy].nApperCount == 1651)
					{
						g_aEnemy[nCntEnemy].move.y = -20.0f;
					}

					if (g_aEnemy[nCntEnemy].nApperCount == 1670)
					{
						SetAttack(g_aEnemy[nCntEnemy].pos,
							D3DXVECTOR3(sinf(-D3DX_PI * 0.9f) * 30.0f, cosf(-D3DX_PI * 0.9f) * 30.0f, 0.0f),
							ATTACKTYPE_FIRE,
							-1, nCntEnemy, -1, MOVETYPE_GRAVITY, 70.0f, 70.0f, 1000, 0.1f, -1);

						SetAttack(g_aEnemy[nCntEnemy].pos,
							D3DXVECTOR3(sinf(-D3DX_PI * -0.9f) * 30.0f, cosf(-D3DX_PI * -0.9f) * 30.0f, 0.0f),
							ATTACKTYPE_FIRE,
							-1, nCntEnemy, -1, MOVETYPE_GRAVITY, 70.0f, 70.0f, 1000, 0.1f, -1);

						SetAttack(g_aEnemy[nCntEnemy].pos,
							D3DXVECTOR3(sinf(-D3DX_PI * 0.8f) * 30.0f, cosf(-D3DX_PI * 0.8f) * 30.0f, 0.0f),
							ATTACKTYPE_FIRE,
							-1, nCntEnemy, -1, MOVETYPE_GRAVITY, 70.0f, 70.0f, 1000, 0.1f, -1);

						SetAttack(g_aEnemy[nCntEnemy].pos,
							D3DXVECTOR3(sinf(-D3DX_PI * -0.8f) * 30.0f, cosf(-D3DX_PI * -0.8f) * 30.0f, 0.0f),
							ATTACKTYPE_FIRE,
							-1, nCntEnemy, -1, MOVETYPE_GRAVITY, 70.0f, 70.0f, 1000, 0.1f, -1);
					}

					if (g_aEnemy[nCntEnemy].nApperCount == 1690)
					{
						g_fXVaim[nCntEnemy] = pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x;//X���W�ւ̃x�N�g��
						g_fYVaim[nCntEnemy] = pPlayer->pos.y - g_aEnemy[nCntEnemy].pos.y;//Y���W�ւ̃x�N�g��
						g_Vaim[nCntEnemy] = atan2(g_fXVaim[nCntEnemy], g_fYVaim[nCntEnemy]);//�e���������̌���
						g_aEnemy[nCntEnemy].bGravity = true;
					
						g_aEnemy[nCntEnemy].move.x = sinf(g_Vaim[nCntEnemy]) * 15.0f;

						for (int nCnt = 0; nCnt < 20; nCnt++)
						{
							for (int nCnt2 = 0; nCnt < 2; nCnt++)
							{
								SetStaging(STAGINGTYPE_NORMAL_WARNINGS, -1, D3DXVECTOR3(64.0f * nCnt ,660.0f - 36.0f * nCnt2,0.0f) , 100, ATTACKTYPE_FLASHBLADE, -D3DX_PI * 0.5f, 50.0f, 0.0f,
									50.0f, 1200.0f, 30);
							}
						}
					}

					if (g_aEnemy[nCntEnemy].nApperCount >= 1691 && g_aEnemy[nCntEnemy].nApperCount <= 1800)
					{
						if (g_aEnemy[nCntEnemy].pos.y == 660.0f)
						{
							for (int nCnt = 0; nCnt < 20; nCnt++)
							{
								SetAttack(D3DXVECTOR3(64.0f * nCnt,SCREEN_HEIGHT,0.0f),
									D3DXVECTOR3(sinf(-D3DX_PI * 1.0f) * 18.0f, cosf(-D3DX_PI * 1.0f) * 18.0f, 0.0f),
									ATTACKTYPE_FIRE,
									-1, nCntEnemy, -1, MOVETYPE_GRAVITY, 70.0f, 70.0f, 30, 0.0f, -1);
							}
						}
					}

					//=============================
					//�p�^�[���T
					//=============================

					if (g_aEnemy[nCntEnemy].nApperCount == 1850)
					{
						g_aEnemy[nCntEnemy].nType = ENEMYTYPE_BOSS3_ATTACK;
						g_fXVaim[nCntEnemy] = 1100.0f - g_aEnemy[nCntEnemy].pos.x;//X���W�ւ̃x�N�g��
						g_fYVaim[nCntEnemy] = 180.0f - g_aEnemy[nCntEnemy].pos.y;//Y���W�ւ̃x�N�g��
						g_Vaim[nCntEnemy] = atan2(g_fXVaim[nCntEnemy], g_fYVaim[nCntEnemy]);//�e���������̌���
						g_aEnemy[nCntEnemy].bGravity = false;

						g_aEnemy[nCntEnemy].move = D3DXVECTOR3(sinf(g_Vaim[nCntEnemy]) * 25.0f, cosf(g_Vaim[nCntEnemy]) * 25.0f, 0.0f);
					}

					if (g_aEnemy[nCntEnemy].nApperCount >= 1851 && g_aEnemy[nCntEnemy].nApperCount <= 2000)
					{
						if (g_aEnemy[nCntEnemy].pos.x >= 1070.0f && g_aEnemy[nCntEnemy].pos.y >= 150.0f &&
							g_aEnemy[nCntEnemy].pos.x <= 1130.0f && g_aEnemy[nCntEnemy].pos.y <= 210.0f)
						{
							g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						}
					}
					
					if (g_aEnemy[nCntEnemy].nApperCount >= 2001 && g_aEnemy[nCntEnemy].nApperCount <= 2150)
					{
						if (g_aEnemy[nCntEnemy].nApperCount == 2010)
						{
							g_fRolingBullet[0] = 0.0f;
						}
						if (g_nDelay % 5 == 0)
						{
							g_fRolingBullet[0] += 0.04f;

							/*float fData = (g_aEnemy[nCntEnemy].nApperCount - 2000) / 100;*/
							SetAttack(g_aEnemy[nCntEnemy].pos,
								D3DXVECTOR3(sinf(-D3DX_PI * g_fRolingBullet[0]) * 12.0f, cosf(-D3DX_PI * g_fRolingBullet[0]) * 12.0f, 0.0f),
								ATTACKTYPE_FIRE,
								-1, nCntEnemy, -1, MOVETYPE_NORMAL, 70.0f, 70.0f, 480, 0.2f, -1);
						}
					}

					if (g_aEnemy[nCntEnemy].nApperCount == 2200)
					{
						g_aEnemy[nCntEnemy].nType = ENEMYTYPE_BOSS3_ATTACK;
						g_fXVaim[nCntEnemy] = 100.0f - g_aEnemy[nCntEnemy].pos.x;//X���W�ւ̃x�N�g��
						g_fYVaim[nCntEnemy] = 180.0f - g_aEnemy[nCntEnemy].pos.y;//Y���W�ւ̃x�N�g��
						g_Vaim[nCntEnemy] = atan2(g_fXVaim[nCntEnemy], g_fYVaim[nCntEnemy]);//�e���������̌���
						g_aEnemy[nCntEnemy].bGravity = false;

						g_aEnemy[nCntEnemy].move = D3DXVECTOR3(sinf(g_Vaim[nCntEnemy]) * 25.0f, cosf(g_Vaim[nCntEnemy]) * 25.0f, 0.0f);
					}

					if (g_aEnemy[nCntEnemy].nApperCount >= 2221 && g_aEnemy[nCntEnemy].nApperCount <= 2320)
					{
						if (g_aEnemy[nCntEnemy].pos.x >= 70.0f && g_aEnemy[nCntEnemy].pos.y >= 150.0f &&
							g_aEnemy[nCntEnemy].pos.x <= 130.0f && g_aEnemy[nCntEnemy].pos.y <= 210.0f)
						{
							g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						}
					}

					if (g_aEnemy[nCntEnemy].nApperCount >= 2321 && g_aEnemy[nCntEnemy].nApperCount <= 2470)
					{
						if (g_aEnemy[nCntEnemy].nApperCount == 2330)
						{
							g_fRolingBullet[0] = 0.0f;
						}
						if (g_nDelay % 5 == 0)
						{
							g_fRolingBullet[0] += -0.04f;

							/*float fData = (g_aEnemy[nCntEnemy].nApperCount - 2000) / 100;*/
							SetAttack(g_aEnemy[nCntEnemy].pos,
								D3DXVECTOR3(sinf(-D3DX_PI * g_fRolingBullet[0]) * 12.0f, cosf(-D3DX_PI * g_fRolingBullet[0]) * 12.0f, 0.0f),
								ATTACKTYPE_FIRE,
								-1, nCntEnemy, -1, MOVETYPE_NORMAL, 70.0f, 70.0f, 480, 0.2f, -1);
						}
					}

					//=========================
					//�p�^�[���U
					//=========================
					if (g_aEnemy[nCntEnemy].nApperCount >= 2501 && g_aEnemy[nCntEnemy].nApperCount <= 3100)
					{
						
						if (g_nDelay % 10 == 0)
						{
							g_aEnemy[nCntEnemy].nType = ENEMYTYPE_BOSS3_ATTACK;
							g_fXVaim[nCntEnemy] = pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x;//X���W�ւ̃x�N�g��
							g_fYVaim[nCntEnemy] = pPlayer->pos.y - g_aEnemy[nCntEnemy].pos.y;//Y���W�ւ̃x�N�g��
							g_Vaim[nCntEnemy] = atan2(g_fXVaim[nCntEnemy], g_fYVaim[nCntEnemy]);//�e���������̌���
						}
						else if (g_nDelay % 15 == 0)
						{
							g_aEnemy[nCntEnemy].pos = pPlayer->pos;

							SetStaging(STAGINGTYPE_WARNING_ATTACK, -1, pPlayer->pos, 120, ATTACKTYPE_FLASHBLADE, -D3DX_PI * g_Vaim[nCntEnemy], 50.0f, 0.0f,
								50.0f, 400.0f, 50);
						}
					}

					if (g_aEnemy[nCntEnemy].nApperCount == 3200)
					{
						g_aEnemy[nCntEnemy].bGravity = true;
						SetStaging(STAGINGTYPE_WARNING_ATTACK, -1,D3DXVECTOR3(pPlayer->pos.x - 180.0f,pPlayer->pos.y,0.0f), 20, ATTACKTYPE_FIRE,0.0f, 50.0f, 0.0f,
							50.0f, 700.0f, 400);

						SetStaging(STAGINGTYPE_WARNING_ATTACK, -1, D3DXVECTOR3(pPlayer->pos.x + 180.0f, pPlayer->pos.y, 0.0f), 20, ATTACKTYPE_FIRE, 0.0f, 50.0f, 0.0f,
							50.0f, 700.0f, 400);

						g_aEnemy[nCntEnemy].bGravity = true;
						SetStaging(STAGINGTYPE_WARNING_ATTACK, -1, D3DXVECTOR3(pPlayer->pos.x - 180.0f, pPlayer->pos.y, 0.0f), 20, ATTACKTYPE_FIRE, 3.14f, 50.0f, 0.0f,
							50.0f, 700.0f, 400);

						SetStaging(STAGINGTYPE_WARNING_ATTACK, -1, D3DXVECTOR3(pPlayer->pos.x + 180.0f, pPlayer->pos.y, 0.0f), 20, ATTACKTYPE_FIRE, 3.14f, 50.0f, 0.0f,
							50.0f, 700.0f, 400);
					}


					if (g_aEnemy[nCntEnemy].nApperCount >= 3251 && g_aEnemy[nCntEnemy].nApperCount <= 3550)
					{
						int nCntBullet_E4;
						float fCircleBullet_E4_normal[4];
						float fRotBullet_E4;
						fRotBullet_E4 = float((rand)() % 200) / 100.0f;
						int nData = rand() % 2 + 1;
						if (g_nDelay % 10 == 0)
						{
							if (nData == 1)
							{
								SetAttack(g_aEnemy[nCntEnemy].pos,
									D3DXVECTOR3(sinf(-D3DX_PI * fRotBullet_E4) * 5.0f, cosf(-D3DX_PI * fRotBullet_E4) * 12.0f, 0.0f),
									ATTACKTYPE_SHAKINGSWORD,
									-1, nCntEnemy, -1, MOVETYPE_NORMAL, 70.0f, 140.0f, 480, 0.0f, 0.1f);
							}
							else
							{
								SetAttack(g_aEnemy[nCntEnemy].pos,
									D3DXVECTOR3(sinf(-D3DX_PI * fRotBullet_E4) * 5.0f, cosf(-D3DX_PI * fRotBullet_E4) * 12.0f, 0.0f),
									ATTACKTYPE_SHAKINGSWORD,
									-1, nCntEnemy, -1, MOVETYPE_NORMAL, 70.0f, 140.0f, 480, 0.0f, -0.1f);
							}
						}
					}

					//==============================
					//�p�^�[���V
					//==============================

					if (g_aEnemy[nCntEnemy].nApperCount >= 3800 && g_aEnemy[nCntEnemy].nApperCount <= 4500)
					{
						g_aEnemy[nCntEnemy].nType = ENEMYTYPE_BOSS3_RUN;
						if (g_aEnemy[nCntEnemy].bDirection == false)
						{//���������Ă���
							g_aEnemy[nCntEnemy].move.x = 5.0f;
						}
						else
						{//�E�������Ă���
							g_aEnemy[nCntEnemy].move.x = -5.0f;
						}

						if (g_aEnemy[nCntEnemy].bIsLanding == true)
						{
							g_aEnemy[nCntEnemy].move.y = -15.0f;
						}
						g_aEnemy[nCntEnemy].bGravity = true;
						if (pPlayer->pos.x >= g_aEnemy[nCntEnemy].pos.x - 150.0f && pPlayer->pos.x <= g_aEnemy[nCntEnemy].pos.x + 150.0f &&
							pPlayer->pos.y - pPlayer->fHeight / 2 >= g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].fHeight - 150.0f &&
							pPlayer->pos.y - pPlayer->fHeight / 2 <= g_aEnemy[nCntEnemy].pos.y + 150.0f)
						{
							if (g_nDelay % 40 == 0)
							{
								g_aEnemy[nCntEnemy].nType = ENEMYTYPE_BOSS3_ATTACK;

								SetStaging(STAGINGTYPE_EXPLOSIONWARNING, -1, D3DXVECTOR3(pPlayer->pos.x - 120.0f, pPlayer->pos.y, 0.0f),
									150, ATTACKTYPE_PUNCH, -1, 50.0f, -1, -1, -1, -1);

								SetStaging(STAGINGTYPE_EXPLOSIONWARNING, -1, D3DXVECTOR3(pPlayer->pos.x + 120.0f, pPlayer->pos.y, 0.0f),
									150, ATTACKTYPE_PUNCH, -1, 50.0f, -1, -1, -1, -1);

								SetStaging(STAGINGTYPE_EXPLOSIONWARNING, -1, D3DXVECTOR3(pPlayer->pos.x, pPlayer->pos.y - 120.0f, 0.0f),
									150, ATTACKTYPE_PUNCH, -1, 50.0f, -1, -1, -1, -1);

								SetStaging(STAGINGTYPE_EXPLOSIONWARNING, -1, D3DXVECTOR3(pPlayer->pos.x, pPlayer->pos.y + 120.0f, 0.0f),
									150, ATTACKTYPE_PUNCH, -1, 50.0f, -1, -1, -1, -1);

							}
							else if (g_nDelay % 20 == 0)
							{
								SetStaging(STAGINGTYPE_EXPLOSIONWARNING, -1, D3DXVECTOR3(pPlayer->pos.x - 120.0f, pPlayer->pos.y - 120.0f, 0.0f),
									150, ATTACKTYPE_PUNCH, -1, 50.0f, -1, -1, -1, -1);

								SetStaging(STAGINGTYPE_EXPLOSIONWARNING, -1, D3DXVECTOR3(pPlayer->pos.x + 120.0f, pPlayer->pos.y -120.0f, 0.0f),
									150, ATTACKTYPE_PUNCH, -1, 50.0f, -1, -1, -1, -1);

								SetStaging(STAGINGTYPE_EXPLOSIONWARNING, -1, D3DXVECTOR3(pPlayer->pos.x - 120.0f, pPlayer->pos.y + 120.0f, 0.0f),
									150, ATTACKTYPE_PUNCH, -1, 50.0f, -1, -1, -1, -1);

								SetStaging(STAGINGTYPE_EXPLOSIONWARNING, -1, D3DXVECTOR3(pPlayer->pos.x + 120.0f, pPlayer->pos.y + 120.0f, 0.0f),
									150, ATTACKTYPE_PUNCH, -1, 50.0f, -1, -1, -1, -1);
							}
						}
						else
						{
							float fRotBullet_E6_R = float((rand)() % 200 + 100) / 100.0f;
							if (g_nDelay % 100 == 0)
							{

								SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(-D3DX_PI * fRotBullet_E6_R) * 3.0f, cosf(-D3DX_PI * fRotBullet_E6_R) * 3.0f, 0.0f),
									600, BULLETTYPE_SPLET, -1, -1, -1, false, MOVETYPE_NORMAL);
							}
						}
					}

					if (g_aEnemy[nCntEnemy].nApperCount >= 4501 && g_aEnemy[nCntEnemy].nApperCount <= 4800)
					{
						g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						g_aEnemy[nCntEnemy].bGravity = true;
					}

					//=====================
					//�ȍ~���[�v
					//=====================

					if (g_aEnemy[nCntEnemy].nApperCount == 4801)
					{
						g_aEnemy[nCntEnemy].nApperCount = 0;
					}

				}
				
				//====================================
				//�{�X�R�K�E�Z����
				//====================================
				if (g_aEnemy[nCntEnemy].bSpecialAttack == true)
				{
				
					if (g_aEnemy[nCntEnemy].nApperCount == 1)
					{
						g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						g_aEnemy[nCntEnemy].bGravity = false;
						g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);

						SetStaging(STAGINGTYPE_NORMAL_WARNINGS, -1, D3DXVECTOR3(SCREEN_WIDTH / 2 + 250.0f, SCREEN_HEIGHT / 2, 0.0f),
							100, ATTACKTYPE_PUNCH, -1.0f, 400, -1.0f, -1.0f, -1.0f, -1);

						SetStaging(STAGINGTYPE_NORMAL_WARNINGS, -1, D3DXVECTOR3(SCREEN_WIDTH / 2 - 250.0f, SCREEN_HEIGHT / 2, 0.0f),
							100, ATTACKTYPE_PUNCH, -1.0f, 400, -1.0f, -1.0f, -1.0f, -1);
					}

					if (g_aEnemy[nCntEnemy].nApperCount == 100)
					{
						SetAttack(D3DXVECTOR3(SCREEN_WIDTH,SCREEN_HEIGHT / 2,0.0f),
							D3DXVECTOR3(sinf(-D3DX_PI * 0.5f) * 2.5f, cosf(-D3DX_PI * 0.5f) * 4.5f, 0.0f),
							ATTACKTYPE_SHAKINGSWORD,
							-1, nCntEnemy, -1, MOVETYPE_NORMAL, 90.0f, 450.0f, 1000, 0.0f, 0.05f);

						SetAttack(D3DXVECTOR3(0.0f, SCREEN_HEIGHT / 2, 0.0f),
							D3DXVECTOR3(sinf(-D3DX_PI * 1.5f) * 2.5f, cosf(-D3DX_PI * 1.5f) * 4.5f, 0.0f),
							ATTACKTYPE_SHAKINGSWORD,
							-1, nCntEnemy, -1, MOVETYPE_NORMAL, 90.0f, 450.0f, 1000, 0.0f, -0.05f);
					}

					

					if (g_aEnemy[nCntEnemy].nApperCount == 400)
					{
						SetStaging(STAGINGTYPE_NORMAL_WARNINGS, -1, D3DXVECTOR3(SCREEN_WIDTH / 2 + 250.0f, SCREEN_HEIGHT / 2 + 200.0f, 0.0f),
							100, ATTACKTYPE_PUNCH, -1.0f, 300, -1.0f, -1.0f, -1.0f, -1);

						SetStaging(STAGINGTYPE_NORMAL_WARNINGS, -1, D3DXVECTOR3(SCREEN_WIDTH / 2 - 250.0f, SCREEN_HEIGHT / 2 - 200.0f, 0.0f),
							100, ATTACKTYPE_PUNCH, -1.0f, 300, -1.0f, -1.0f, -1.0f, -1);
					}

					if (g_aEnemy[nCntEnemy].nApperCount == 550)
					{
						SetAttack(D3DXVECTOR3(SCREEN_WIDTH / 2 + 250.0f, SCREEN_HEIGHT / 2 + 360.0f, 0.0f),
							D3DXVECTOR3(sinf(-D3DX_PI * 1.0f) * 3.5f, cosf(-D3DX_PI * 1.0f) * 3.5f, 0.0f),
							ATTACKTYPE_SHAKINGSWORD,
							-1, nCntEnemy, -1, MOVETYPE_NORMAL, 90.0f, 450.0f, 1000,D3DX_PI * -0.5f , 0.05f);

						SetAttack(D3DXVECTOR3(SCREEN_WIDTH / 2 - 250.0f, SCREEN_HEIGHT / 2 - 360.0f, 0.0f),
							D3DXVECTOR3(sinf(-D3DX_PI * 0.0f) * 5.5f, cosf(-D3DX_PI * 0.0f) * 5.5f, 0.0f),
							ATTACKTYPE_SHAKINGSWORD,
							-1, nCntEnemy, -1, MOVETYPE_NORMAL, 90.0f, 450.0f, 1000, D3DX_PI * 0.5f, -0.05f);
					}

					if (g_aEnemy[nCntEnemy].nApperCount == 700)
					{
						SetStaging(STAGINGTYPE_NORMAL_WARNINGS, -1, D3DXVECTOR3(SCREEN_WIDTH / 2 + 250.0f, SCREEN_HEIGHT / 2 - 300.0f, 0.0f),
							100, ATTACKTYPE_PUNCH, -1.0f, 300, -1.0f, -1.0f, -1.0f, -1);

						SetStaging(STAGINGTYPE_NORMAL_WARNINGS, -1, D3DXVECTOR3(SCREEN_WIDTH / 2 - 250.0f, SCREEN_HEIGHT / 2 - 300.0f, 0.0f),
							100, ATTACKTYPE_PUNCH, -1.0f, 300, -1.0f, -1.0f, -1.0f, -1);
					}

					if (g_aEnemy[nCntEnemy].nApperCount == 800)
					{
						SetAttack(D3DXVECTOR3(SCREEN_WIDTH / 2 + 250.0f, SCREEN_HEIGHT / 2 - 360.0f, 0.0f),
							D3DXVECTOR3(sinf(-D3DX_PI * 0.0f) * 4.5f, cosf(-D3DX_PI * 0.0f) * 4.5f, 0.0f),
							ATTACKTYPE_SHAKINGSWORD,
							1, 1, -1, MOVETYPE_NORMAL, 90.0f, 440.0f, 1000, D3DX_PI * 0.5f, 0.05f);

						SetAttack(D3DXVECTOR3(SCREEN_WIDTH / 2 - 250.0f, SCREEN_HEIGHT / 2 - 360.0f, 0.0f),
							D3DXVECTOR3(sinf(-D3DX_PI * 0.0f) * 4.5f, cosf(-D3DX_PI * 0.0f) * 4.5f, 0.0f),
							ATTACKTYPE_SHAKINGSWORD,
							1, 1, -1, MOVETYPE_NORMAL, 90.0f, 440.0f, 1000, D3DX_PI * -0.5f, -0.05f);
					}

					if (g_aEnemy[nCntEnemy].nApperCount >= 1100 && g_aEnemy[nCntEnemy].nApperCount <= 1800)
					{
						if (g_nDelay % 40 == 0)
						{
							SetAttack(D3DXVECTOR3(0.0f, pPlayer->pos.y - 10.0f, 0.0f),
								D3DXVECTOR3(sinf(-D3DX_PI * 1.5f) * 12.0f, cosf(-D3DX_PI * 1.5f) * 12.0f, 0.0f),
								ATTACKTYPE_FIRE,
								-1, nCntEnemy, -1, MOVETYPE_NORMAL, 70.0f, 70.0f, 480, 0.2f, -1);
							
							SetAttack(D3DXVECTOR3(SCREEN_WIDTH, pPlayer->pos.y - 10.0f, 0.0f),
								D3DXVECTOR3(sinf(-D3DX_PI * 0.5f) * 12.0f, cosf(-D3DX_PI * 0.5f) * 12.0f, 0.0f),
								ATTACKTYPE_FIRE,
								-1, nCntEnemy, -1, MOVETYPE_NORMAL, 70.0f, 70.0f, 480, 0.2f, -1);
						}
						if (g_aEnemy[nCntEnemy].nApperCount == 1350)
						{
							SetStaging(STAGINGTYPE_WARNING_ATTACK, -1, D3DXVECTOR3(pPlayer->pos.x, 0.0f, 0.0f),
								100, ATTACKTYPE_FIRE, -1.0f, 300, -1.0f, 50.0f, 120.0f, 600);
						}


						if (g_aEnemy[nCntEnemy].nApperCount == 1600)
						{
							SetAttack(D3DXVECTOR3(pPlayer->pos.x - 300.0f, 0.0f, 0.0f),
								D3DXVECTOR3(0.0f,0.0f, 0.0f),
								ATTACKTYPE_FIRE,
								-1, nCntEnemy, -1, MOVETYPE_GRAVITY, 70.0f, 70.0f, 480, 0.0f, -1);

							SetAttack(D3DXVECTOR3(pPlayer->pos.x + 300.0f,0.0f, 0.0f),
								D3DXVECTOR3(0.0f,0.0f, 0.0f),
								ATTACKTYPE_FIRE,
								-1, nCntEnemy, -1, MOVETYPE_GRAVITY, 70.0f, 140.0f, 480, 0.0f, -1);
						}

						if (g_aEnemy[nCntEnemy].nApperCount == 1650)
						{
							SetAttack(D3DXVECTOR3(pPlayer->pos.x - 200.0f, 0.0f, 0.0f),
								D3DXVECTOR3(0.0f, 0.0f, 0.0f),
								ATTACKTYPE_FIRE,
								-1, nCntEnemy, -1, MOVETYPE_GRAVITY, 70.0f, 140.0f, 480, 0.0f, -1);

							SetAttack(D3DXVECTOR3(pPlayer->pos.x + 200.0f, 0.0f, 0.0f),
								D3DXVECTOR3(0.0f, 0.0f, 0.0f),
								ATTACKTYPE_FIRE,
								-1, nCntEnemy, -1, MOVETYPE_GRAVITY, 70.0f, 140.0f, 480, 0.0f, -1);
						}

						if (g_aEnemy[nCntEnemy].nApperCount == 1700)
						{
							SetAttack(D3DXVECTOR3(pPlayer->pos.x - 100.0f, 0.0f, 0.0f),
								D3DXVECTOR3(0.0f, 0.0f, 0.0f),
								ATTACKTYPE_FIRE,
								-1, nCntEnemy, -1, MOVETYPE_GRAVITY, 70.0f, 140.0f, 480, 0.0f, -1);

							SetAttack(D3DXVECTOR3(pPlayer->pos.x + 100.0f, 0.0f, 0.0f),
								D3DXVECTOR3(0.0f, 0.0f, 0.0f),
								ATTACKTYPE_FIRE,
								-1, nCntEnemy, -1, MOVETYPE_GRAVITY, 70.0f, 140.0f, 480, 0.0f, -1);
						}
					
					}


					if (g_aEnemy[nCntEnemy].nApperCount >= 1750 && g_aEnemy[nCntEnemy].nApperCount <= 1900)
					{
					    
						if (g_nDelay % 10 == 0)
						{
							SetStaging(STAGINGTYPE_WARNING_ATTACK, -1, D3DXVECTOR3(pPlayer->pos.x, 100.0f, 0.0f),
								100, ATTACKTYPE_FLASHBLADE, -D3DX_PI * 1.0f, 50.0f, 0.0f, 80.0f, 700.0f, 30);
						}
					}

					if (g_aEnemy[nCntEnemy].nApperCount == 1950)
					{
						SetStaging(STAGINGTYPE_NORMAL_WARNINGS, -1, D3DXVECTOR3(SCREEN_WIDTH / 2 + 250.0f, SCREEN_HEIGHT / 2 - 300.0f, 0.0f),
							100, ATTACKTYPE_PUNCH, -1.0f, 300, -1.0f, -1.0f, -1.0f, -1);
					}

					if (g_aEnemy[nCntEnemy].nApperCount == 2050)
					{
						SetAttack(D3DXVECTOR3(SCREEN_WIDTH / 2 + 250.0f, SCREEN_HEIGHT / 2 - 360.0f, 0.0f),
							D3DXVECTOR3(sinf(-D3DX_PI * 0.0f) * 4.5f, cosf(-D3DX_PI * 0.0f) * 4.5f, 0.0f),
							ATTACKTYPE_SHAKINGSWORD,
							1, 1, -1, MOVETYPE_NORMAL, 90.0f, 720.0f,40, D3DX_PI * 0.0f, -0.08f);
					}

					if (g_aEnemy[nCntEnemy].nApperCount == 2100)
					{
						SetStaging(STAGINGTYPE_NORMAL_WARNINGS, -1, D3DXVECTOR3(SCREEN_WIDTH / 2 - 250.0f, SCREEN_HEIGHT / 2 - 300.0f, 0.0f),
							100, ATTACKTYPE_PUNCH, -1.0f, 300, -1.0f, -1.0f, -1.0f, -1);
					}

					if (g_aEnemy[nCntEnemy].nApperCount == 2200)
					{
						SetAttack(D3DXVECTOR3(SCREEN_WIDTH / 2 - 250.0f, SCREEN_HEIGHT / 2 - 360.0f, 0.0f),
							D3DXVECTOR3(sinf(-D3DX_PI * 0.0f) * 4.5f, cosf(-D3DX_PI * 0.0f) * 4.5f, 0.0f),
							ATTACKTYPE_SHAKINGSWORD,
							1, 1, -1, MOVETYPE_NORMAL, 90.0f, 720.0f, 40, D3DX_PI * 0.0f, 0.08f);
					}

					if (g_aEnemy[nCntEnemy].nApperCount == 2300)
					{
						SetStaging(STAGINGTYPE_NORMAL_WARNINGS, -1, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
							150, ATTACKTYPE_PUNCH, -1.0f, 500.0f, -1.0f, -1.0f, -1.0f, -1);
					}

					if (g_aEnemy[nCntEnemy].nApperCount >= 2450 && g_aEnemy[nCntEnemy].nApperCount <= 2950)
					{
						g_fRolingBullet[1] += 0.013f;
						g_fRolingBullet[2] += -0.013f;
						if (g_aEnemy[nCntEnemy].nApperCount == 2451)
						{
							SetAttack(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
								D3DXVECTOR3(0.0f,0.0f,0.0f),
								ATTACKTYPE_SHAKINGSWORD,
								1, 1, -1, MOVETYPE_NORMAL, 120.0f, 500.0f, 400, D3DX_PI * 0.0f, 0.35f);
						}

						if (g_nDelay % 5 == 0)
						{
							SetBullet(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f)
								, D3DXVECTOR3((sinf(-D3DX_PI * g_fRolingBullet[1]) * 5.0f), (cosf(-D3DX_PI * g_fRolingBullet[1]) * 5.0f), 0.0f)
								, GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1, false, MOVETYPE_NORMAL);//�G�̒e������G���m�œ�����Ȃ�

							SetBullet(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f)
								, D3DXVECTOR3((sinf(-D3DX_PI * (g_fRolingBullet[1] + 0.5f)) * 5.0f), (cosf(-D3DX_PI * (g_fRolingBullet[1] + 0.5f)) * 5.0f), 0.0f)
								, GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1, false, MOVETYPE_NORMAL);//�G�̒e������G���m�œ�����Ȃ�

							SetBullet(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f)
								, D3DXVECTOR3((sinf(-D3DX_PI * (g_fRolingBullet[1] + 1.0f)) * 5.0f), (cosf(-D3DX_PI * (g_fRolingBullet[1] + 1.0f)) * 5.0f), 0.0f)
								, GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1, false, MOVETYPE_NORMAL);//�G�̒e������G���m�œ�����Ȃ�

							SetBullet(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f)
								, D3DXVECTOR3((sinf(-D3DX_PI * (g_fRolingBullet[1] + 1.5f)) * 5.0f), (cosf(-D3DX_PI * (g_fRolingBullet[1] + 1.5f)) * 5.0f), 0.0f)
								, GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1, false, MOVETYPE_NORMAL);//�G�̒e������G���m�œ�����Ȃ�

							SetBullet(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f)
								, D3DXVECTOR3((sinf(-D3DX_PI * g_fRolingBullet[2]) * 5.0f), (cosf(-D3DX_PI * g_fRolingBullet[2]) * 5.0f), 0.0f)
								, GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1, false, MOVETYPE_NORMAL);//�G�̒e������G���m�œ�����Ȃ�

							SetBullet(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f)
								, D3DXVECTOR3((sinf(-D3DX_PI * (g_fRolingBullet[2] + 0.5f)) * 5.0f), (cosf(-D3DX_PI * (g_fRolingBullet[2] + 0.5f)) * 5.0f), 0.0f)
								, GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1, false, MOVETYPE_NORMAL);//�G�̒e������G���m�œ�����Ȃ�

							SetBullet(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f)
								, D3DXVECTOR3((sinf(-D3DX_PI * (g_fRolingBullet[2] + 1.0f)) * 5.0f), (cosf(-D3DX_PI * (g_fRolingBullet[2] + 1.0f)) * 5.0f), 0.0f)
								, GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1, false, MOVETYPE_NORMAL);//�G�̒e������G���m�œ�����Ȃ�

							SetBullet(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f)
								, D3DXVECTOR3((sinf(-D3DX_PI * (g_fRolingBullet[2] + 1.5f)) * 5.0f), (cosf(-D3DX_PI * (g_fRolingBullet[2] + 1.5f)) * 5.0f), 0.0f)
								, GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1, false, MOVETYPE_NORMAL);//�G�̒e������G���m�œ�����Ȃ�

						}

						

					}
					//====================
					//�ʏ�U���֖߂�
					//====================

					if (g_aEnemy[nCntEnemy].nApperCount == 3000)
					{
						g_aEnemy[nCntEnemy].nApperCount = 0;
						g_aEnemy[nCntEnemy].bSpecialAttack = false;
						g_aEnemy[nCntEnemy].bGravity = true;
					}
				}




				//===============================}==========
				//�e�N�X�`���A�j���[�V��������
				//=======================================
				if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS3_ATTACK)
				{
					g_aEnemy[nCntEnemy].nCntAnim++;//�A�j���[�V�����J�E���^�[
					if (g_aEnemy[nCntEnemy].nCntAnim >= 5)
					{//�A�j���[�V�����̃X�s�[�h��������
						g_aEnemy[nCntEnemy].nPatternAnim++;
						g_aEnemy[nCntEnemy].nCntAnim = 0;
					}

					if (g_aEnemy[nCntEnemy].nPatternAnim >= 5)
					{//���[�v�p����
						g_aEnemy[nCntEnemy].nPatternAnim = 0;
					}

					if (g_aEnemy[nCntEnemy].bDirection == false)
					{
						pVtx[0].tex = D3DXVECTOR2(0.0f + g_aEnemy[nCntEnemy].nPatternAnim * 0.2f, 0.0f);
						pVtx[1].tex = D3DXVECTOR2(0.2f + g_aEnemy[nCntEnemy].nPatternAnim * 0.2f, 0.0f);
						pVtx[2].tex = D3DXVECTOR2(0.0f + g_aEnemy[nCntEnemy].nPatternAnim * 0.2f, 1.0f);
						pVtx[3].tex = D3DXVECTOR2(0.2f + g_aEnemy[nCntEnemy].nPatternAnim * 0.2f, 1.0f);
					}
					else if (g_aEnemy[nCntEnemy].bDirection == true)
					{
						pVtx[1].tex = D3DXVECTOR2(0.0f + g_aEnemy[nCntEnemy].nPatternAnim * 0.2f, 0.0f);
						pVtx[0].tex = D3DXVECTOR2(0.2f + g_aEnemy[nCntEnemy].nPatternAnim * 0.2f, 0.0f);
						pVtx[3].tex = D3DXVECTOR2(0.0f + g_aEnemy[nCntEnemy].nPatternAnim * 0.2f, 1.0f);
						pVtx[2].tex = D3DXVECTOR2(0.2f + g_aEnemy[nCntEnemy].nPatternAnim * 0.2f, 1.0f);
					}
				}
				else if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS3_RUN)
				{//�ړ����[�V����
					g_aEnemy[nCntEnemy].nCntAnim++;//�A�j���[�V�����J�E���^�[
					if (g_aEnemy[nCntEnemy].nCntAnim >= 5)
					{
						g_aEnemy[nCntEnemy].nPatternAnim++;
						g_aEnemy[nCntEnemy].nCntAnim = 0;
					}

					if (g_aEnemy[nCntEnemy].nPatternAnim >= 8)
					{
						g_aEnemy[nCntEnemy].nPatternAnim = 0;
					}

					if (g_aEnemy[nCntEnemy].bDirection == false)
					{
						pVtx[0].tex = D3DXVECTOR2(0.0f + g_aEnemy[nCntEnemy].nPatternAnim * 0.125f, 0.0f);
						pVtx[1].tex = D3DXVECTOR2(0.125f + g_aEnemy[nCntEnemy].nPatternAnim * 0.125f, 0.0f);
						pVtx[2].tex = D3DXVECTOR2(0.0f + g_aEnemy[nCntEnemy].nPatternAnim * 0.125f, 1.0f);
						pVtx[3].tex = D3DXVECTOR2(0.125f + g_aEnemy[nCntEnemy].nPatternAnim * 0.125f, 1.0f);
					}
					else if (g_aEnemy[nCntEnemy].bDirection == true)
					{
						pVtx[1].tex = D3DXVECTOR2(0.0f + g_aEnemy[nCntEnemy].nPatternAnim * 0.125f, 0.0f);
						pVtx[0].tex = D3DXVECTOR2(0.125f + g_aEnemy[nCntEnemy].nPatternAnim * 0.125f, 0.0f);
						pVtx[3].tex = D3DXVECTOR2(0.0f + g_aEnemy[nCntEnemy].nPatternAnim * 0.125f, 1.0f);
						pVtx[2].tex = D3DXVECTOR2(0.125f + g_aEnemy[nCntEnemy].nPatternAnim * 0.125f, 1.0f);
					}
				}
				else if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS3_STAND)
				{//�ҋ@���[�V����
					g_aEnemy[nCntEnemy].nCntAnim++;//�A�j���[�V�����J�E���^�[
					if (g_aEnemy[nCntEnemy].nCntAnim >= 10)
					{
						g_aEnemy[nCntEnemy].nPatternAnim++;
						g_aEnemy[nCntEnemy].nCntAnim = 0;
					}

					if (g_aEnemy[nCntEnemy].nPatternAnim >= 4)
					{
						g_aEnemy[nCntEnemy].nPatternAnim = 0;
					}

					if (g_aEnemy[nCntEnemy].bDirection == false)
					{
						pVtx[0].tex = D3DXVECTOR2(0.0f + g_aEnemy[nCntEnemy].nPatternAnim * 0.25f, 0.0f);
						pVtx[1].tex = D3DXVECTOR2(0.25f + g_aEnemy[nCntEnemy].nPatternAnim * 0.25f, 0.0f);
						pVtx[2].tex = D3DXVECTOR2(0.0f + g_aEnemy[nCntEnemy].nPatternAnim * 0.25f, 1.0f);
						pVtx[3].tex = D3DXVECTOR2(0.25f + g_aEnemy[nCntEnemy].nPatternAnim * 0.25f, 1.0f);
					}
					else if (g_aEnemy[nCntEnemy].bDirection == true)
					{
						pVtx[1].tex = D3DXVECTOR2(0.0f + g_aEnemy[nCntEnemy].nPatternAnim * 0.25f, 0.0f);
						pVtx[0].tex = D3DXVECTOR2(0.25f + g_aEnemy[nCntEnemy].nPatternAnim * 0.25f, 0.0f);
						pVtx[3].tex = D3DXVECTOR2(0.0f + g_aEnemy[nCntEnemy].nPatternAnim * 0.25f, 1.0f);
						pVtx[2].tex = D3DXVECTOR2(0.25f + g_aEnemy[nCntEnemy].nPatternAnim * 0.25f, 1.0f);
					}
				}



			}
			//=====================================================================================================================================================
			//=========================================================
			//�{�X�S�̏���
			//=========================================================
			if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS4_STAND ||
				g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS4_RUN ||
				g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS4_ATTACK)
			{
				g_aEnemy[nCntEnemy].nCountMoveInterval++;
				g_aEnemy[nCntEnemy].nApperCount++;//�G���o�����Ă���̎��Ԃ��J�E���g����B

				if (g_aEnemy[nCntEnemy].nCountMoveInterval == 1)
				{
					PlaySound(SOUND_LABEL_SE_SPECIALATTACK);
					StopSound(SOUND_LABEL_BGM009);
					PlaySound(SOUND_LABEL_BGM004);
					SetGage(nCntEnemy, GAGETYPE_BOSS1HP);
					SetGage(-1, GAGETYPE_BOSSHP_FRAME);
					g_nCntAttack = 0;
				}


				//================================
				//�`�ԕω�����
				//================================
				if (g_aEnemy[nCntEnemy].fLife <= g_aEnemy[nCntEnemy].fMaxHp / 4)
				{//�c��HP���ő�HP�̂S���̂P�ɂȂ�����
					if (g_aEnemy[nCntEnemy].bSpecialStaging == false)
					{
						g_aEnemy[nCntEnemy].bSpecialAttack = true;
						g_aEnemy[nCntEnemy].bSpecialStaging = true;
						/*SetStaging(STAGINGTYPE_BOSS4_SPECIAL_ATTACK, -1, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 180, ATTACKTYPE_PUNCH, 0.0f, -1, -1,
							-1, -1, 150);*/
						SetSpecial(SPECIALTYPE_BOSS4_SPECIAL_ATTACK, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 150);
						g_aEnemy[nCntEnemy].nApperCount = 0;
						g_nCntAttack = 0;
					}
				}
				//================================
				//�ʏ�U���i�p�^�[���P�`�U�j
				//================================
				if (g_aEnemy[nCntEnemy].bSpecialAttack == false)
				{
					//======================
					//�p�^�[���P
					//======================

					if (g_aEnemy[nCntEnemy].nApperCount >= 1 && g_aEnemy[nCntEnemy].nApperCount <= 100)
					{
						g_aEnemy[nCntEnemy].bGravity = true;

						if (g_aEnemy[nCntEnemy].nApperCount == 1)
						{
							g_aEnemy[nCntEnemy].nType = ENEMYTYPE_BOSS4_ATTACK;
							g_nCntAttack = 0;
						}
					
						if (g_nDelay % 10 == 0)
						{
							SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 120.0f * g_nCntAttack,SCREEN_HEIGHT - 40.0f, 0.0f),
								100, ATTACKTYPE_THUNDER_E4, D3DX_PI * 0.0f, 50.0f, 0.0f, 50.0f, 700.0f, 40);

							SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 120.0f * g_nCntAttack, SCREEN_HEIGHT - 40.0f, 0.0f),
								100, ATTACKTYPE_THUNDER_E4, D3DX_PI * 0.0f, 50.0f, 0.0f, 50.0f, 700.0f, 40);
							g_nCntAttack++;
						}
					}

					if (g_aEnemy[nCntEnemy].nApperCount >= 101 && g_aEnemy[nCntEnemy].nApperCount <= 200)
					{
						g_aEnemy[nCntEnemy].bGravity = true;

						if (g_aEnemy[nCntEnemy].nApperCount == 101)
						{
							g_nCntAttack = 0;
						}

						if (g_nDelay % 10 == 0)
						{
							SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 60.0f - 120.0f * g_nCntAttack, SCREEN_HEIGHT - 40.0f, 0.0f),
								100, ATTACKTYPE_THUNDER_E4, D3DX_PI * 0.0f, 50.0f, 0.0f, 50.0f, 700.0f, 40);

							SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 60.0f + 120.0f * g_nCntAttack, SCREEN_HEIGHT - 40.0f, 0.0f),
								100, ATTACKTYPE_THUNDER_E4, D3DX_PI * 0.0f, 50.0f, 0.0f, 50.0f, 700.0f, 40);
							g_nCntAttack++;
						}
					}

				
					if (g_aEnemy[nCntEnemy].nApperCount == 250)
					{
						g_aEnemy[nCntEnemy].nType = ENEMYTYPE_BOSS4_ATTACK;
						g_fXVaim[nCntEnemy] = 0.0f - g_aEnemy[nCntEnemy].pos.x;//X���W�ւ̃x�N�g��
						g_fYVaim[nCntEnemy] = 180.0f - g_aEnemy[nCntEnemy].pos.y;//Y���W�ւ̃x�N�g��
						g_Vaim[nCntEnemy] = atan2(g_fXVaim[nCntEnemy], g_fYVaim[nCntEnemy]);//�e���������̌���
						g_aEnemy[nCntEnemy].bGravity = false;

						g_nCntAttack = 0;
						g_aEnemy[nCntEnemy].move = D3DXVECTOR3(sinf(g_Vaim[nCntEnemy]) * 20.0f, cosf(g_Vaim[nCntEnemy]) * 20.0f, 0.0f);
					}

					if (g_aEnemy[nCntEnemy].nApperCount >= 251 && g_aEnemy[nCntEnemy].nApperCount <= 371)
					{
						if (g_aEnemy[nCntEnemy].pos.x >=  - 20.0f && g_aEnemy[nCntEnemy].pos.y > 160.0f &&
							g_aEnemy[nCntEnemy].pos.x <= 20.0f && g_aEnemy[nCntEnemy].pos.y <= 200.0f)
						{
							g_aEnemy[nCntEnemy].move = D3DXVECTOR3(10.0f, 0.0f, 0.0f);

						
						}
						if (g_aEnemy[nCntEnemy].pos.x >= 0.0f && g_aEnemy[nCntEnemy].pos.x <= 1000.0f)
						{
							if (g_nDelay % 5 == 0)
							{
								g_nCntAttack++;
								SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, SCREEN_HEIGHT - 40.0f, 0.0f),
									100, ATTACKTYPE_THUNDER_E4, D3DX_PI * 0.0f, 50.0f, 0.0f, 50.0f, 700.0f, 40);
							}
						}
						
					}

					if (g_aEnemy[nCntEnemy].nApperCount >= 402 && g_aEnemy[nCntEnemy].nApperCount <= 530)
					{
						if (g_aEnemy[nCntEnemy].pos.x >= 280.0f && g_aEnemy[nCntEnemy].pos.x <= 1280.0f)
						{
							if (g_nDelay % 5 == 0)
							{
								g_nCntAttack++;
								SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, SCREEN_HEIGHT - 40.0f, 0.0f),
									100, ATTACKTYPE_THUNDER_E4, D3DX_PI * 0.0f, 50.0f, 0.0f, 50.0f, 700.0f, 40);
							}
						}

					}

					if (g_aEnemy[nCntEnemy].nApperCount == 531)
					{
						g_nCntAttack = 0;
						g_aEnemy[nCntEnemy].bGravity = true;
					}

					if (g_aEnemy[nCntEnemy].nApperCount == 650)
					{
						SetStaging(STAGINGTYPE_NORMAL_WARNINGS, nCntEnemy,g_aEnemy[nCntEnemy].pos,
							100, ATTACKTYPE_THUNDER_2, -D3DX_PI * 0.0f, 150.0f, 0.0f, 400.0f, 1280.0f, 60);
					}
					//========================
					//�p�^�[���Q
					//========================
					if (g_aEnemy[nCntEnemy].nApperCount == 701)
					{
						g_aEnemy[nCntEnemy].bGravity = false;
						if (g_aEnemy[nCntEnemy].bDirection == false)
						{
							g_aEnemy[nCntEnemy].move = D3DXVECTOR3(20.0f,-20.0f, 0.0f);
						}
						else
						{
							g_aEnemy[nCntEnemy].move = D3DXVECTOR3(-20.0f,-20.0f, 0.0f);
						}
					}

					if (g_aEnemy[nCntEnemy].nApperCount >= 702 && g_aEnemy[nCntEnemy].nApperCount <= 800)
					{
						if (g_aEnemy[nCntEnemy].pos.x >= pPlayer->pos.x - 20.0f && g_aEnemy[nCntEnemy].pos.x <= pPlayer->pos.x + 20.0f&& g_nCntAttack == 0)
						{
							g_nCntAttack++;
							g_aEnemy[nCntEnemy].bGravity = true;
							g_aEnemy[nCntEnemy].move.y = 20.0f;

							for (int nCnt = 0; nCnt < 20; nCnt++)
							{
								SetStaging(STAGINGTYPE_NORMAL_WARNINGS, nCntEnemy, D3DXVECTOR3(64.0f + 64.0f * nCnt,665.0f, 0.0f),
									50, ATTACKTYPE_THUNDER_E4, D3DX_PI * 0.0f, 64.0f, 0.0f, 50.0f, 700.0f, 40);
							}

							for (int nCnt = 0; nCnt < 20; nCnt++)
							{
								SetStaging(STAGINGTYPE_NORMAL_WARNINGS, nCntEnemy, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x,64.0f + 64.0f * nCnt, 0.0f),
									50, ATTACKTYPE_THUNDER_E4, D3DX_PI * 0.0f, 64.0f, 0.0f, 50.0f, 700.0f, 40);
							}
						}
					}

					if (g_aEnemy[nCntEnemy].nApperCount >= 702 && g_aEnemy[nCntEnemy].nApperCount <= 900 && g_nCntAttack == 1)
					{
						if (g_aEnemy[nCntEnemy].pos.y >= 655.0f && g_nCntAttack == 1)
						{
							g_nCntAttack = 0;
							SetAttack(g_aEnemy[nCntEnemy].pos,
								D3DXVECTOR3(0.0f, 0.0f, 0.0f),
								ATTACKTYPE_THUNDER_2,
								-1, -1, -1, MOVETYPE_NORMAL, 80.0f, 1000.0f,53, D3DX_PI * 0.0f, 0.0f);

							SetAttack(g_aEnemy[nCntEnemy].pos,
								D3DXVECTOR3(0.0f, 0.0f, 0.0f),
								ATTACKTYPE_THUNDER_2,
								-1, -1, -1, MOVETYPE_NORMAL, 80.0f, 1000.0f, 30, D3DX_PI * 0.5f, 0.0f);
							SetAttack(g_aEnemy[nCntEnemy].pos,
								D3DXVECTOR3(0.0f, 0.0f, 0.0f),
								ATTACKTYPE_THUNDER_2,
								-1, -1, -1, MOVETYPE_NORMAL, 80.0f, 1000.0f, 30, -D3DX_PI * 0.5f, 0.0f);
						}
					}
					if (g_aEnemy[nCntEnemy].nApperCount >= 650 && g_aEnemy[nCntEnemy].nApperCount <= 1500)
					{
						if (g_nDelay % 10 == 0)
						{
							SetStaging(STAGINGTYPE_NORMAL_WARNINGS, nCntEnemy, g_aEnemy[nCntEnemy].pos,
								100, ATTACKTYPE_THUNDER_2, -D3DX_PI * 0.0f, 150.0f, 0.0f, 400.0f, 1280.0f, 60);
						}
					}
					if (g_aEnemy[nCntEnemy].nApperCount == 901)
					{
						g_aEnemy[nCntEnemy].bGravity = false;
						if (g_aEnemy[nCntEnemy].bDirection == false)
						{
							g_aEnemy[nCntEnemy].move = D3DXVECTOR3(20.0f, -20.0f, 0.0f);
						}
						else
						{
							g_aEnemy[nCntEnemy].move = D3DXVECTOR3(-20.0f, -20.0f, 0.0f);
						}
					}

					if (g_aEnemy[nCntEnemy].nApperCount >= 902 && g_aEnemy[nCntEnemy].nApperCount <= 1000)
					{
						if (g_aEnemy[nCntEnemy].pos.x >= pPlayer->pos.x - 20.0f && g_aEnemy[nCntEnemy].pos.x <= pPlayer->pos.x + 20.0f && g_nCntAttack == 0)
						{
							g_nCntAttack++;
							g_aEnemy[nCntEnemy].bGravity = true;
							g_aEnemy[nCntEnemy].move.y = 20.0f;

							for (int nCnt = 0; nCnt < 20; nCnt++)
							{
								SetStaging(STAGINGTYPE_NORMAL_WARNINGS, nCntEnemy, D3DXVECTOR3(64.0f + 64.0f * nCnt, 665.0f, 0.0f),
									50, ATTACKTYPE_THUNDER_E4, D3DX_PI * 0.0f, 64.0f, 0.0f, 50.0f, 700.0f, 40);
							}

							for (int nCnt = 0; nCnt < 20; nCnt++)
							{
								SetStaging(STAGINGTYPE_NORMAL_WARNINGS, nCntEnemy, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, 64.0f + 64.0f * nCnt, 0.0f),
									50, ATTACKTYPE_THUNDER_E4, D3DX_PI * 0.0f, 64.0f, 0.0f, 50.0f, 700.0f, 40);
							}
						}
					}

					if (g_aEnemy[nCntEnemy].nApperCount >= 902 && g_aEnemy[nCntEnemy].nApperCount <= 1100 && g_nCntAttack == 1)
					{
						if (g_aEnemy[nCntEnemy].pos.y >= 655.0f && g_nCntAttack == 1)
						{
							g_nCntAttack = 0;
							SetAttack(g_aEnemy[nCntEnemy].pos,
								D3DXVECTOR3(0.0f, 0.0f, 0.0f),
								ATTACKTYPE_THUNDER_2,
								-1, -1, -1, MOVETYPE_NORMAL, 80.0f, 1000.0f, 30, D3DX_PI * 0.0f, 0.0f);

							SetAttack(g_aEnemy[nCntEnemy].pos,
								D3DXVECTOR3(0.0f, 0.0f, 0.0f),
								ATTACKTYPE_THUNDER_2,
								-1, -1, -1, MOVETYPE_NORMAL, 80.0f, 1000.0f, 30, D3DX_PI * 0.5f, 0.0f);
							SetAttack(g_aEnemy[nCntEnemy].pos,
								D3DXVECTOR3(0.0f, 0.0f, 0.0f),
								ATTACKTYPE_THUNDER_2,
								-1, -1, -1, MOVETYPE_NORMAL, 80.0f, 1000.0f, 30, -D3DX_PI * 0.5f, 0.0f);
						}
					}

					if (g_aEnemy[nCntEnemy].nApperCount ==1101)
					{
						g_aEnemy[nCntEnemy].bGravity = false;
						if (g_aEnemy[nCntEnemy].bDirection == false)
						{
							g_aEnemy[nCntEnemy].move = D3DXVECTOR3(20.0f, -20.0f, 0.0f);
						}
						else
						{
							g_aEnemy[nCntEnemy].move = D3DXVECTOR3(-20.0f, -20.0f, 0.0f);
						}
					}

					if (g_aEnemy[nCntEnemy].nApperCount >= 1102 && g_aEnemy[nCntEnemy].nApperCount <= 1200)
					{
						if (g_aEnemy[nCntEnemy].pos.x >= pPlayer->pos.x - 20.0f && g_aEnemy[nCntEnemy].pos.x <= pPlayer->pos.x + 20.0f && g_nCntAttack == 0)
						{
							g_nCntAttack++;
							g_aEnemy[nCntEnemy].bGravity = true;
							g_aEnemy[nCntEnemy].move.y = 20.0f;

							for (int nCnt = 0; nCnt < 20; nCnt++)
							{
								SetStaging(STAGINGTYPE_NORMAL_WARNINGS, nCntEnemy, D3DXVECTOR3(64.0f + 64.0f * nCnt, 665.0f, 0.0f),
									50, ATTACKTYPE_THUNDER_E4, D3DX_PI * 0.0f, 64.0f, 0.0f, 50.0f, 700.0f, 40);
							}

							for (int nCnt = 0; nCnt < 20; nCnt++)
							{
								SetStaging(STAGINGTYPE_NORMAL_WARNINGS, nCntEnemy, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, 64.0f + 64.0f * nCnt, 0.0f),
									50, ATTACKTYPE_THUNDER_E4, D3DX_PI * 0.0f, 64.0f, 0.0f, 50.0f, 700.0f, 40);
							}
						}
					}

					if (g_aEnemy[nCntEnemy].nApperCount >= 1200 && g_aEnemy[nCntEnemy].nApperCount <= 1450 && g_nCntAttack == 1)
					{
						if (g_aEnemy[nCntEnemy].pos.y >= 655.0f && g_nCntAttack == 1)
						{
							g_nCntAttack = 0;
							SetAttack(g_aEnemy[nCntEnemy].pos,
								D3DXVECTOR3(0.0f, 0.0f, 0.0f),
								ATTACKTYPE_THUNDER_2,
								-1, -1, -1, MOVETYPE_NORMAL, 80.0f, 1000.0f, 30, D3DX_PI * 0.0f, 0.0f);

							SetAttack(g_aEnemy[nCntEnemy].pos,
								D3DXVECTOR3(0.0f, 0.0f, 0.0f),
								ATTACKTYPE_THUNDER_2,
								-1, -1, -1, MOVETYPE_NORMAL, 80.0f, 1000.0f, 30, D3DX_PI * 0.5f, 0.0f);
							SetAttack(g_aEnemy[nCntEnemy].pos,
								D3DXVECTOR3(0.0f, 0.0f, 0.0f),
								ATTACKTYPE_THUNDER_2,
								-1, -1, -1, MOVETYPE_NORMAL, 80.0f, 1000.0f, 30, -D3DX_PI * 0.5f, 0.0f);
						}
					}

					//===========================
					//�p�^�[���R
					//===========================
					if (g_aEnemy[nCntEnemy].nApperCount >= 1450 && g_aEnemy[nCntEnemy].nApperCount <= 1950)
					{
						g_aEnemy[nCntEnemy].nType = ENEMYTYPE_BOSS4_RUN;
						if (g_aEnemy[nCntEnemy].bDirection == false)
						{//���������Ă���
							g_aEnemy[nCntEnemy].move.x = 6.0f;
						}
						else
						{//�E�������Ă���
							g_aEnemy[nCntEnemy].move.x = -6.0f;
						}

						g_aEnemy[nCntEnemy].bGravity = true;
						if (pPlayer->pos.x >= g_aEnemy[nCntEnemy].pos.x - 250.0f && pPlayer->pos.x <= g_aEnemy[nCntEnemy].pos.x + 250.0f &&
							pPlayer->pos.y - pPlayer->fHeight / 2 >= g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].fHeight - 250.0f &&
							pPlayer->pos.y - pPlayer->fHeight / 2 <= g_aEnemy[nCntEnemy].pos.y + 250.0f)
						{
							if (g_nDelay % 70 == 0)
							{
							
								if (g_aEnemy[nCntEnemy].bDirection == false)
								{//���������Ă���
									g_aEnemy[nCntEnemy].move.x = 300.0f;
								}
								else
								{//�E�������Ă���
									g_aEnemy[nCntEnemy].move.x = -300.0f;
								}

								int nCntBullet_E4;
								float fCircleBullet_E4_normal[8];
								float fRotBullet_E4;
								fRotBullet_E4 = float((rand)() % 200) / 100.0f;

								PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_004);

									for (nCntBullet_E4 = 0; nCntBullet_E4 < 8; nCntBullet_E4++)

									{
										fCircleBullet_E4_normal[nCntBullet_E4] = (1.0f / 4.0f) * nCntBullet_E4;

										SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].fHeight / 2, 0.0f),
											D3DXVECTOR3((sinf(-D3DX_PI * (fCircleBullet_E4_normal[nCntBullet_E4] + fRotBullet_E4)) * 2.0f), (cosf(-D3DX_PI * (fCircleBullet_E4_normal[nCntBullet_E4] + fRotBullet_E4)) * 2.0f), 0.0f),
											GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1, true, MOVETYPE_NORMAL);

									}
									SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, D3DXVECTOR3(pPlayer->pos.x,665.0f, 0.0f),
										100, ATTACKTYPE_THUNDER_E4, D3DX_PI * 0.0f, 64.0f, 0.0f, 50.0f, 700.0f, 40);

									SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, D3DXVECTOR3(pPlayer->pos.x - 400.0f, 665.0f, 0.0f),
										100, ATTACKTYPE_THUNDER_E4, D3DX_PI * 0.0f, 64.0f, 0.0f, 50.0f, 700.0f, 40);

									SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, D3DXVECTOR3(pPlayer->pos.x + 400.0f, 665.0f, 0.0f),
										100, ATTACKTYPE_THUNDER_E4, D3DX_PI * 0.0f, 64.0f, 0.0f, 50.0f, 700.0f, 40);
							}
						
						}
						else
						{
							float fRotBullet_E6_R = float((rand)() % 200 + 100) / 100.0f;
							if (g_nDelay % 40 == 0)
							{
								for(int nCnt = 0;nCnt < 5;nCnt++)
								SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(-D3DX_PI * (0.6f + 0.2f * nCnt)) * 18.0f, cosf(-D3DX_PI * (0.6f + 0.2f * nCnt)) * 18.0f, 0.0f),
									600, BULLETTYPE_ENEMY, -1, -1, -1, false, MOVETYPE_GRAVITY);
							}
						}
					}

					//==============================
					//�p�^�[���S
					//==============================

					if (g_aEnemy[nCntEnemy].nApperCount >= 2000 && g_aEnemy[nCntEnemy].nApperCount <= 2600)
					{
						g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						g_aEnemy[nCntEnemy].nType = ENEMYTYPE_BOSS4_ATTACK;
						float fRandumPosX;
						float fRandumPosY;
						fRandumPosX = float((rand)() % SCREEN_WIDTH + 1);
						fRandumPosY = float((rand)() % SCREEN_HEIGHT + 1);
						int nRandPattern = rand() % 2 + 1;
						SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 50.0f, 665.0f, 0.0f),
							50, ATTACKTYPE_THUNDER_E4, D3DX_PI * 0.0f, 64.0f, 0.0f, 50.0f, 700.0f, 40);

						SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 50.0f, 665.0f, 0.0f),
							50, ATTACKTYPE_THUNDER_E4, D3DX_PI * 0.0f, 64.0f, 0.0f, 50.0f, 700.0f, 40);

						if (g_nDelay % 30 == 0)
						{
							if (nRandPattern == 1)
							{
								SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, D3DXVECTOR3(fRandumPosX, fRandumPosY, 0.0f),
									50, ATTACKTYPE_THUNDER_2, D3DX_PI * 0.0f, 64.0f, 0.0f, 50.0f, 1280.0f, 20);

								SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, D3DXVECTOR3(fRandumPosX, fRandumPosY, 0.0f),
									50, ATTACKTYPE_THUNDER_2, D3DX_PI * 0.5f, 64.0f, 0.0f, 50.0f, 1280.0f, 20);

								SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, D3DXVECTOR3(fRandumPosX, fRandumPosY, 0.0f),
									50, ATTACKTYPE_THUNDER_2, D3DX_PI * 1.0f, 64.0f, 0.0f, 50.0f, 1280.0f, 20);

								SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, D3DXVECTOR3(fRandumPosX, fRandumPosY, 0.0f),
									50, ATTACKTYPE_THUNDER_2, -D3DX_PI * 0.5f, 64.0f, 0.0f, 50.0f, 1280.0f, 20);

								SetStaging(STAGINGTYPE_NORMAL_WARNINGS, nCntEnemy, D3DXVECTOR3(fRandumPosX - 100.0f, fRandumPosY, 0.0f),
									50, ATTACKTYPE_THUNDER_2, D3DX_PI * 0.0f, 64.0f, 0.0f, 50.0f, 1280.0f, 20);

								SetStaging(STAGINGTYPE_NORMAL_WARNINGS, nCntEnemy, D3DXVECTOR3(fRandumPosX + 100.0f, fRandumPosY, 0.0f),
									50, ATTACKTYPE_THUNDER_2, D3DX_PI * 0.5f, 64.0f, 0.0f, 50.0f, 1280.0f, 20);

								SetStaging(STAGINGTYPE_NORMAL_WARNINGS, nCntEnemy, D3DXVECTOR3(fRandumPosX, fRandumPosY - 100.0f, 0.0f),
									50, ATTACKTYPE_THUNDER_2, D3DX_PI * 1.0f, 64.0f, 0.0f, 50.0f, 1280.0f, 20);

								SetStaging(STAGINGTYPE_NORMAL_WARNINGS, nCntEnemy, D3DXVECTOR3(fRandumPosX, fRandumPosY + 100.0f, 0.0f),
									50, ATTACKTYPE_THUNDER_2, -D3DX_PI * 0.5f, 64.0f, 0.0f, 50.0f, 1280.0f, 20);
							}
							else if (nRandPattern == 2)
							{
								SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, D3DXVECTOR3(fRandumPosX, fRandumPosY, 0.0f),
									50, ATTACKTYPE_THUNDER_2, D3DX_PI * 0.25f, 64.0f, 0.0f, 50.0f, 1280.0f,20);

								SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, D3DXVECTOR3(fRandumPosX, fRandumPosY, 0.0f),
									50, ATTACKTYPE_THUNDER_2, D3DX_PI * 0.75f, 64.0f, 0.0f, 50.0f, 1280.0f, 20);

								SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, D3DXVECTOR3(fRandumPosX, fRandumPosY, 0.0f),
									50, ATTACKTYPE_THUNDER_2, -D3DX_PI * 0.25f, 64.0f, 0.0f, 50.0f, 1280.0f, 20);

								SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, D3DXVECTOR3(fRandumPosX, fRandumPosY, 0.0f),
									50, ATTACKTYPE_THUNDER_2, -D3DX_PI * 0.75f, 64.0f, 0.0f, 50.0f, 1280.0f, 20);

								SetStaging(STAGINGTYPE_NORMAL_WARNINGS, nCntEnemy, D3DXVECTOR3(fRandumPosX - 100.0f, fRandumPosY - 100.0f, 0.0f),
									50, ATTACKTYPE_THUNDER_2, D3DX_PI * 0.0f, 64.0f, 0.0f, 50.0f, 1280.0f, 20);

								SetStaging(STAGINGTYPE_NORMAL_WARNINGS, nCntEnemy, D3DXVECTOR3(fRandumPosX + 100.0f, fRandumPosY - 100.0f, 0.0f),
									50, ATTACKTYPE_THUNDER_2, D3DX_PI * 0.5f, 64.0f, 0.0f, 50.0f, 1280.0f, 20);

								SetStaging(STAGINGTYPE_NORMAL_WARNINGS, nCntEnemy, D3DXVECTOR3(fRandumPosX - 100.0f, fRandumPosY + 100.0f, 0.0f),
									50, ATTACKTYPE_THUNDER_2, D3DX_PI * 1.0f, 64.0f, 0.0f, 50.0f, 1280.0f, 20);

								SetStaging(STAGINGTYPE_NORMAL_WARNINGS, nCntEnemy, D3DXVECTOR3(fRandumPosX + 100.0f, fRandumPosY + 100.0f, 0.0f),
									50, ATTACKTYPE_THUNDER_2, -D3DX_PI * 0.5f, 64.0f, 0.0f, 50.0f, 1280.0f, 20);
							}
						}
					}

					//================================
					//�p�^�[���T
					//================================
					if (g_aEnemy[nCntEnemy].nApperCount == 2700)
					{
						g_nCntAttack = 0;
						g_aEnemy[nCntEnemy].bGravity = true;
					}
					if (g_aEnemy[nCntEnemy].nApperCount >= 2700 && g_aEnemy[nCntEnemy].nApperCount <= 3500)
					{
						if (g_nCntAttack == 0)
						{
							if (g_aEnemy[nCntEnemy].bDirection == false)
							{
								g_aEnemy[nCntEnemy].move.x = 12.0f;
							}
							else
							{
								g_aEnemy[nCntEnemy].move.x = -12.0f;
							}

							if (pPlayer->pos.x >= g_aEnemy[nCntEnemy].pos.x - 100.0f && pPlayer->pos.x <= g_aEnemy[nCntEnemy].pos.x + 100.0f &&
								pPlayer->pos.y - pPlayer->fHeight / 2 >= g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].fHeight - 100.0f &&
								pPlayer->pos.y - pPlayer->fHeight / 2 <= g_aEnemy[nCntEnemy].pos.y + 100.0f)
							{
								
									g_nCntAttack++;
									
									if (g_aEnemy[nCntEnemy].bDirection == false)
									{
										g_aEnemy[nCntEnemy].move.x += 120.0f;
										SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 50.0f, 665.0f, 0.0f),
											50, ATTACKTYPE_FLASHBLADE, -D3DX_PI * 0.25f, 64.0f, 0.0f, 50.0f, 300.0f, 40);

										SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 50.0f, 665.0f, 0.0f),
											50, ATTACKTYPE_FLASHBLADE, -D3DX_PI * 0.5f, 64.0f, 0.0f, 50.0f, 300.0f, 40);

										SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 50.0f, 665.0f, 0.0f),
											50, ATTACKTYPE_FLASHBLADE, -D3DX_PI * 0.75f, 64.0f, 0.0f, 50.0f, 300.0f, 40);
									}
									else
									{
										g_aEnemy[nCntEnemy].move.x += -120.0f;
										SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 50.0f, 665.0f, 0.0f),
											50, ATTACKTYPE_FLASHBLADE, D3DX_PI * 0.25f, 64.0f, 0.0f, 50.0f, 300.0f, 40);

										SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 50.0f, 665.0f, 0.0f),
											50, ATTACKTYPE_FLASHBLADE, D3DX_PI * 0.5f, 64.0f, 0.0f, 50.0f, 300.0f, 40);

										SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 50.0f, 665.0f, 0.0f),
											50, ATTACKTYPE_FLASHBLADE, D3DX_PI * 0.75f, 64.0f, 0.0f, 50.0f, 300.0f, 40);
									}
								
								
							}
						}

						if (g_nCntAttack >= 1 && g_nCntAttack <= 5)
						{
							if (g_nDelay % 15 == 0)
							{
								g_nCntAttack++;
								SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy,pPlayer->pos,
									50, ATTACKTYPE_THUNDER_E4,0.0f, 64.0f, 0.0f, 50.0f, 300.0f, 40);
							}
						}


						if (g_nCntAttack == 6)
						{
							if (g_nDelay % 30 == 0)
							{
								g_nCntAttack++;
								g_aEnemy[nCntEnemy].move.y = -40.0f;
							}
						}

						if (g_nCntAttack == 7)
						{

						

								SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x,g_aEnemy[nCntEnemy].pos.y - 500.0f,0.0f),
									D3DXVECTOR3((sinf(-D3DX_PI * 0.95f) * 40.0f), (cosf(-D3DX_PI * 0.95f) * 40.0f), 0.0f),
									GENERALLY_BULLETLIFE, BULLETTYPE_ROCKON, 0, nCntEnemy, -1, true, MOVETYPE_NORMAL);

								SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y - 500.0f, 0.0f),
									D3DXVECTOR3((sinf(-D3DX_PI * 1.05f) * 40.0f), (cosf(-D3DX_PI * 1.5f) * 40.0f), 0.0f),
									GENERALLY_BULLETLIFE, BULLETTYPE_ROCKON, 0, nCntEnemy, -1, true, MOVETYPE_NORMAL);

								g_nCntAttack = 0;
							
						}

					

					}

					if (g_aEnemy[nCntEnemy].nApperCount == 3650)
					{
						g_aEnemy[nCntEnemy].bGravity = true;
						g_nCntAttack = 0;
						g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, -30.0f, 0.0f);
					}

					//=============================
					//�p�^�[���U
					//=============================

					if (g_aEnemy[nCntEnemy].nApperCount == 3700)
					{
						for (int nCnt = 0; nCnt < 20; nCnt++)
						{
							SetStaging(STAGINGTYPE_NORMAL_WARNINGS, nCntEnemy, D3DXVECTOR3(64.0f + 64.0f * nCnt, 665.0f, 0.0f),
								120, ATTACKTYPE_THUNDER_E4, D3DX_PI * 0.0f, 64.0f, 0.0f, 50.0f, 1200.0f, 40);
						}
					}
					if (g_aEnemy[nCntEnemy].nApperCount >= 3700 && g_aEnemy[nCntEnemy].nApperCount <= 4300)
					{
						if (g_aEnemy[nCntEnemy].bDirection == false)
						{//���������Ă���
							g_aEnemy[nCntEnemy].move.x = 5.0f;
						}
						else
						{//�E�������Ă���
							g_aEnemy[nCntEnemy].move.x = -5.0f;
						}

						if (g_aEnemy[nCntEnemy].bIsLanding == true)
						{
							g_aEnemy[nCntEnemy].move.y = -15.0f;
						}

						if (g_aEnemy[nCntEnemy].pos.y == 665.0f)
						{
							g_nCntAttack++;
							SetAttack(g_aEnemy[nCntEnemy].pos,
								D3DXVECTOR3(0.0f, 0.0f, 0.0f),
								ATTACKTYPE_THUNDER_2,
								-1, -1, -1, MOVETYPE_NORMAL, 50.0f, 1500.0f, 30, D3DX_PI * 0.5f, 0.0f);

							SetAttack(g_aEnemy[nCntEnemy].pos,
								D3DXVECTOR3(0.0f, 0.0f, 0.0f),
								ATTACKTYPE_THUNDER_2,
								-1, -1, -1, MOVETYPE_NORMAL, 50.0f, 1500.0f, 30, -D3DX_PI * 0.5f, 0.0f);
						}

						if (g_nCntAttack == 6)
						{
							g_nCntAttack = 0;

							for (int nCnt = 0; nCnt < 6; nCnt++)
							{
								SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y - 100.0f, 0.0f),
									D3DXVECTOR3((sinf(-D3DX_PI * 0.9f) * (10.0f +3.0f *nCnt)), (cosf(-D3DX_PI * 0.9f) * (10.0f + 3.0f * nCnt)), 0.0f),
									GENERALLY_BULLETLIFE, BULLETTYPE_JUMPING, 0, nCntEnemy, -1, true, MOVETYPE_GRAVITY);

								SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y - 100.0f, 0.0f),
									D3DXVECTOR3((sinf(-D3DX_PI * 1.1f) * (10.0f + 3.0f * nCnt)), (cosf(-D3DX_PI * 1.1f) * (10.0f + 3.0f * nCnt)), 0.0f),
									GENERALLY_BULLETLIFE, BULLETTYPE_JUMPING, 0, nCntEnemy, -1, true, MOVETYPE_GRAVITY);
							}
						}

					}


					//==============================
					//�ȍ~���[�v
					//==============================
					if (g_aEnemy[nCntEnemy].nApperCount == 4301)
					{
						g_aEnemy[nCntEnemy].nApperCount = 0;
						g_nCntAttack = 0;
						g_aEnemy[nCntEnemy].bGravity = true;
					}

				}

				//================================================
				//�{�X�S�K�E�Z����
				//================================================
				if (g_aEnemy[nCntEnemy].bSpecialAttack == true)
				{
					//=====================
					//�p�^�[���P
					//=====================

					if (g_aEnemy[nCntEnemy].nApperCount == 1)
					{
						g_aEnemy[nCntEnemy].bGravity = false;
						g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
					}

					if (g_aEnemy[nCntEnemy].nApperCount >= 50 && g_aEnemy[nCntEnemy].nApperCount <= 500)
					{
						float fRandumPosX;
						float fRandumPosY;
						fRandumPosX = float((rand)() % SCREEN_WIDTH + 1);
						fRandumPosY = float((rand)() % SCREEN_HEIGHT + 1);

						if (g_nDelay % 5 == 0)
						{
							SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, D3DXVECTOR3(fRandumPosX, 665.0f, 0.0f),
								50, ATTACKTYPE_THUNDER_E4, D3DX_PI * 0.0f, 64.0f, 0.0f, 50.0f, 1200.0f, 40);
						}
					}

					if (g_aEnemy[nCntEnemy].nApperCount == 600)
					{
						for (int nCnt = 0; nCnt < 4; nCnt++)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 400.0f, g_aEnemy[nCntEnemy].pos.y - 100.0f, 0.0f),
								D3DXVECTOR3((sinf(-D3DX_PI * 0.5f) * (2.0f * nCnt)), (cosf(-D3DX_PI * 0.5f) * (2.0f * nCnt)), 0.0f),
								GENERALLY_BULLETLIFE, BULLETTYPE_JUMPING, 0, nCntEnemy, -1, true, MOVETYPE_GRAVITY);

							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x -
								400.0f, g_aEnemy[nCntEnemy].pos.y - 100.0f, 0.0f),
								D3DXVECTOR3((sinf(-D3DX_PI * 1.5f) * (2.0f * nCnt)), (cosf(-D3DX_PI * 1.5f) * (2.0f * nCnt)), 0.0f),
								GENERALLY_BULLETLIFE, BULLETTYPE_JUMPING, 0, nCntEnemy, -1, true, MOVETYPE_GRAVITY);
						}
					}

					if (g_aEnemy[nCntEnemy].nApperCount == 650)
					{
						SetStaging(STAGINGTYPE_NORMAL_WARNINGS, nCntEnemy, D3DXVECTOR3(64.0f, 665.0f, 0.0f),
							120, ATTACKTYPE_THUNDER_E4, D3DX_PI * 0.0f, 64.0f, 0.0f, 50.0f, 1200.0f, 40);

						for (int nCnt = 0; nCnt < 17; nCnt++)
						{
							SetStaging(STAGINGTYPE_NORMAL_WARNINGS, nCntEnemy, D3DXVECTOR3(350.0f + 64.0f * nCnt, 665.0f, 0.0f),
								120, ATTACKTYPE_THUNDER_E4, D3DX_PI * 0.0f, 64.0f, 0.0f, 50.0f, 1200.0f, 40);
						}
					}

					if (g_aEnemy[nCntEnemy].nApperCount == 770)
					{
						SetAttack(D3DXVECTOR3(0.0f,720.0f,0.0f),
							D3DXVECTOR3(4.0f,0.0f, 0.0f),
							ATTACKTYPE_THUNDER_2,
							-1, -1, -1, MOVETYPE_NORMAL, 50.0f, 1500.0f,240, D3DX_PI * 0.0f, 0.0f);

						SetAttack(D3DXVECTOR3(230.0f, 720.0f, 0.0f),
							D3DXVECTOR3(4.0f, 0.0f, 0.0f),
							ATTACKTYPE_THUNDER_2,
							-1, -1, -1, MOVETYPE_NORMAL, 50.0f, 1500.0f,240, D3DX_PI * 0.0f, 0.0f);
					}

					//===========================
					//�p�^�[���R
					//===========================
					if (g_aEnemy[nCntEnemy].nApperCount == 1020)
					{
						float fRandumPosX;
						float fRandumPosY;
						fRandumPosX = float((rand)() % SCREEN_WIDTH + 1);
						fRandumPosY = float((rand)() % 520 + 1);

						for (int nCnt = 0; nCnt < 11; nCnt++)
						{
							SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, D3DXVECTOR3(fRandumPosX,fRandumPosY, 0.0f),
								120, ATTACKTYPE_THUNDER_2, D3DX_PI * (0.1f * nCnt), 64.0f, 0.0f, 70.0f, 1280.0f,100);

							SetStaging(STAGINGTYPE_NORMAL_WARNINGS, nCntEnemy, D3DXVECTOR3(fRandumPosX + sinf(D3DX_PI *nCnt) * 100.0f , fRandumPosY + cosf(D3DX_PI * nCnt * 100.0f), 0.0f),
								120, ATTACKTYPE_THUNDER_2, D3DX_PI* (0.1f *nCnt), 64.0f, 0.0f, 70.0f, 1280.0f, 100);
						}

						for (int nCnt = 0; nCnt < 11; nCnt++)
						{
							SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, D3DXVECTOR3(fRandumPosX, fRandumPosY, 0.0f),
								120, ATTACKTYPE_THUNDER_2, -D3DX_PI * (0.1f * nCnt), 64.0f, 0.0f, 70.0f, 1280.0f, 100);



							SetStaging(STAGINGTYPE_NORMAL_WARNINGS, nCntEnemy, D3DXVECTOR3(fRandumPosX + sinf(-D3DX_PI * nCnt) * 100.0f, fRandumPosY + cosf(-D3DX_PI * nCnt * 100.0f), 0.0f),
								120, ATTACKTYPE_THUNDER_2, D3DX_PI* (0.1f * nCnt), 64.0f, 0.0f, 70.0f, 1280.0f, 100);
						}


					}

					if (g_aEnemy[nCntEnemy].nApperCount == 1300)
					{
						for (int nCnt = 0; nCnt < 11; nCnt++)
						{
							SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, D3DXVECTOR3(128.0f * nCnt, 690.0f, 0.0f),
								50, ATTACKTYPE_THUNDER_2, -D3DX_PI * 0.0f, 64.0f, 0.0f, 70.0f, 1280.0f,25);
						}
					}

					if (g_aEnemy[nCntEnemy].nApperCount == 1400)
					{
						for (int nCnt = 0; nCnt < 11; nCnt++)
						{
							SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, D3DXVECTOR3(-64.0f+128.0f* nCnt, 690.0f, 0.0f),
								50, ATTACKTYPE_THUNDER_2, -D3DX_PI * 0.0f, 64.0f, 0.0f, 70.0f, 1280.0f, 25);
						}
					}

					if (g_aEnemy[nCntEnemy].nApperCount == 1500)
					{
						for (int nCnt = 0; nCnt < 11; nCnt++)
						{
							SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, D3DXVECTOR3(128.0f * nCnt, 690.0f, 0.0f),
								30, ATTACKTYPE_THUNDER_2, -D3DX_PI * 0.0f, 64.0f, 0.0f, 70.0f, 1280.0f, 25);
						}
					}

					if (g_aEnemy[nCntEnemy].nApperCount == 1600)
					{
						for (int nCnt = 0; nCnt < 11; nCnt++)
						{
							SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, D3DXVECTOR3(-64.0f + 128.0f * nCnt, 690.0f, 0.0f),
								30, ATTACKTYPE_THUNDER_2, -D3DX_PI * 0.0f, 64.0f, 0.0f, 70.0f, 1280.0f, 35);
						}
					}

					if (g_aEnemy[nCntEnemy].nApperCount == 1680)
					{
							SetStaging(STAGINGTYPE_NORMAL_WARNINGS, nCntEnemy, D3DXVECTOR3(SCREEN_WIDTH / 2 - 30.0f,690.0f, 0.0f),
								100, ATTACKTYPE_THUNDER_2, -D3DX_PI * 0.0f, 64.0f, 0.0f, 70.0f, 1280.0f, 35);

							SetStaging(STAGINGTYPE_NORMAL_WARNINGS, nCntEnemy, D3DXVECTOR3(SCREEN_WIDTH / 2 + 30.0f, 690.0f, 0.0f),
								100, ATTACKTYPE_THUNDER_2, -D3DX_PI * 0.0f, 64.0f, 0.0f, 70.0f, 1280.0f, 35);
					}

					if (g_aEnemy[nCntEnemy].nApperCount == 1780)
					{
						SetAttack(D3DXVECTOR3(SCREEN_WIDTH / 2 - 30.0f, 690.0f, 0.0f),
							D3DXVECTOR3(7.0f, 0.0f, 0.0f),
							ATTACKTYPE_THUNDER_2,
							-1, -1, -1, MOVETYPE_NORMAL, 50.0f, 1500.0f, 90, D3DX_PI * 0.0f, 0.0f);

						SetAttack(D3DXVECTOR3(SCREEN_WIDTH / 2 + 30.0f, 690.0f, 0.0f),
							D3DXVECTOR3(-7.0f, 0.0f, 0.0f),
							ATTACKTYPE_THUNDER_2,
							-1, -1, -1, MOVETYPE_NORMAL, 50.0f, 1500.0f, 90, D3DX_PI * 0.0f, 0.0f);
					}

					if (g_aEnemy[nCntEnemy].nApperCount == 1850)
					{
						for (int nCnt = 0; nCnt < 11; nCnt++)
						{
							SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, D3DXVECTOR3(-64.0f + 128.0f * nCnt, 690.0f, 0.0f),
								80, ATTACKTYPE_THUNDER_2, -D3DX_PI * 0.0f, 64.0f, 0.0f, 70.0f, 1280.0f, 180);
						}

						for (int nCnt = 0; nCnt < 3; nCnt++)
						{
							SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, D3DXVECTOR3(SCREEN_WIDTH,280.0f * nCnt, 0.0f),
								80, ATTACKTYPE_THUNDER_2, D3DX_PI * 0.5f, 64.0f, 0.0f, 70.0f, 1280.0f, 180);
						}
					}

					if (g_aEnemy[nCntEnemy].nApperCount == 2030)
					{
						for (int nCnt = 0; nCnt < 11; nCnt++)
						{
							SetBullet(g_aEnemy[nCntEnemy].pos,
								D3DXVECTOR3((sinf(-D3DX_PI * (0.5f + (0.1f * nCnt))) * 5.0f), cosf(-D3DX_PI * (0.5f + (0.1f * nCnt)) * 5.0f), 0.0f),
								GENERALLY_BULLETLIFE, BULLETTYPE_SPLET, 0, nCntEnemy, -1, true, MOVETYPE_NORMAL);
						}
					}

					if (g_aEnemy[nCntEnemy].nApperCount == 2150)
					{
						SetStaging(STAGINGTYPE_NORMAL_WARNINGS, nCntEnemy, D3DXVECTOR3(SCREEN_WIDTH / 2 - 300.0f,SCREEN_HEIGHT / 2, 0.0f),
							100, ATTACKTYPE_THUNDER_2, -D3DX_PI * 0.0f,300.0f, 0.0f, 70.0f, 1280.0f,60);

						SetStaging(STAGINGTYPE_NORMAL_WARNINGS, nCntEnemy, D3DXVECTOR3(SCREEN_WIDTH / 2 + 300.0f,SCREEN_HEIGHT / 2, 0.0f),
							100, ATTACKTYPE_THUNDER_2, -D3DX_PI * 0.0f,300.0f, 0.0f, 300.0f, 1280.0f,60);
					}

					if (g_aEnemy[nCntEnemy].nApperCount == 2250)
					{
						SetAttack(D3DXVECTOR3(SCREEN_WIDTH / 2 - 500.0f, 690.0f, 0.0f),
							D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							ATTACKTYPE_THUNDER_2,
							-1, -1, -1, MOVETYPE_NORMAL,500.0f, 1500.0f, 60, D3DX_PI * 0.0f, 0.0f);

						SetAttack(D3DXVECTOR3(SCREEN_WIDTH / 2 + 500.0f, 690.0f, 0.0f),
							D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							ATTACKTYPE_THUNDER_2,
							-1, -1, -1, MOVETYPE_NORMAL, 500.0f, 1500.0f, 60, D3DX_PI * 0.0f, 0.0f);

						SetStaging(STAGINGTYPE_NORMAL_WARNINGS, nCntEnemy, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
							110, ATTACKTYPE_THUNDER_2, -D3DX_PI * 0.0f, 300.0f, 0.0f, 300.0f, 1280.0f, 60);
					}

					if (g_aEnemy[nCntEnemy].nApperCount == 2360)
					{

						SetAttack(D3DXVECTOR3(SCREEN_WIDTH / 2, 690.0f, 0.0f),
							D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							ATTACKTYPE_THUNDER_2,
							-1, -1, -1, MOVETYPE_NORMAL, 600.0f, 1500.0f, 60, D3DX_PI * 0.0f, 0.0f);
					}

					if (g_aEnemy[nCntEnemy].nApperCount == 2470)
					{
						SetStaging(STAGINGTYPE_NORMAL_WARNINGS, nCntEnemy, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
							110, ATTACKTYPE_THUNDER_2, -D3DX_PI * 0.0f, 300.0f, 0.0f, 400.0f, 1280.0f, 60);

						SetStaging(STAGINGTYPE_NORMAL_WARNINGS, nCntEnemy, D3DXVECTOR3(SCREEN_WIDTH / 2 - 450.0f, SCREEN_HEIGHT / 2, 0.0f),
							110, ATTACKTYPE_THUNDER_2, -D3DX_PI * 0.0f, 300.0f, 0.0f, 300.0f, 1280.0f, 60);

						SetStaging(STAGINGTYPE_NORMAL_WARNINGS, nCntEnemy, D3DXVECTOR3(SCREEN_WIDTH / 2 + 450.0f, SCREEN_HEIGHT / 2, 0.0f),
							110, ATTACKTYPE_THUNDER_2, -D3DX_PI * 0.0f, 300.0f, 0.0f, 300.0f, 1280.0f, 60);
					}

					if (g_aEnemy[nCntEnemy].nApperCount >= 2580 && g_aEnemy[nCntEnemy].nApperCount <= 2700)
					{

						SetAttack(D3DXVECTOR3(SCREEN_WIDTH / 2, 695.0f, 0.0f),
							D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							ATTACKTYPE_THUNDER_2,
							-1, -1, -1, MOVETYPE_NORMAL, 400.0f, 1500.0f, 200, D3DX_PI * 0.0f, 0.0f);

						SetAttack(D3DXVECTOR3(SCREEN_WIDTH / 2 - 450.0f, 695.0f, 0.0f),
							D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							ATTACKTYPE_THUNDER_2,
							-1, -1, -1, MOVETYPE_NORMAL, 300.0f, 1500.0f, 200, D3DX_PI * 0.0f, 0.0f);

						SetAttack(D3DXVECTOR3(SCREEN_WIDTH / 2 + 450.0f, 695.0f, 0.0f),
							D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							ATTACKTYPE_THUNDER_2,
							-1, -1, -1, MOVETYPE_NORMAL, 300.0f, 1500.0f, 200, D3DX_PI * 0.0f, 0.0f);
					}
					

					//=======================
					//�ʏ�U���֖߂�
					//=======================
					if (g_aEnemy[nCntEnemy].nApperCount == 2780)
					{
						g_nCntAttack = 0;
						g_aEnemy[nCntEnemy].bGravity = true;
						g_aEnemy[nCntEnemy].bSpecialAttack = false;
						g_aEnemy[nCntEnemy].nApperCount = 0;
					}

				}

				//=======================================
				//�e�N�X�`���A�j���[�V��������
				//=======================================
				if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS4_ATTACK)
				{
					g_aEnemy[nCntEnemy].nCntAnim++;//�A�j���[�V�����J�E���^�[
					if (g_aEnemy[nCntEnemy].nCntAnim >= 5)
					{//�A�j���[�V�����̃X�s�[�h��������
						g_aEnemy[nCntEnemy].nPatternAnim++;
						g_aEnemy[nCntEnemy].nCntAnim = 0;
					}

					if (g_aEnemy[nCntEnemy].nPatternAnim >= 5)
					{//���[�v�p����
						g_aEnemy[nCntEnemy].nPatternAnim = 0;
					}

					if (g_aEnemy[nCntEnemy].bDirection == false)
					{
						pVtx[0].tex = D3DXVECTOR2(0.0f + g_aEnemy[nCntEnemy].nPatternAnim * 0.2f, 0.0f);
						pVtx[1].tex = D3DXVECTOR2(0.2f + g_aEnemy[nCntEnemy].nPatternAnim * 0.2f, 0.0f);
						pVtx[2].tex = D3DXVECTOR2(0.0f + g_aEnemy[nCntEnemy].nPatternAnim * 0.2f, 1.0f);
						pVtx[3].tex = D3DXVECTOR2(0.2f + g_aEnemy[nCntEnemy].nPatternAnim * 0.2f, 1.0f);
					}
					else if (g_aEnemy[nCntEnemy].bDirection == true)
					{
						pVtx[1].tex = D3DXVECTOR2(0.0f + g_aEnemy[nCntEnemy].nPatternAnim * 0.2f, 0.0f);
						pVtx[0].tex = D3DXVECTOR2(0.2f + g_aEnemy[nCntEnemy].nPatternAnim * 0.2f, 0.0f);
						pVtx[3].tex = D3DXVECTOR2(0.0f + g_aEnemy[nCntEnemy].nPatternAnim * 0.2f, 1.0f);
						pVtx[2].tex = D3DXVECTOR2(0.2f + g_aEnemy[nCntEnemy].nPatternAnim * 0.2f, 1.0f);
					}
				}
				else if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS4_RUN)
				{//�ړ����[�V����
					g_aEnemy[nCntEnemy].nCntAnim++;//�A�j���[�V�����J�E���^�[
					if (g_aEnemy[nCntEnemy].nCntAnim >= 5)
					{
						g_aEnemy[nCntEnemy].nPatternAnim++;
						g_aEnemy[nCntEnemy].nCntAnim = 0;
					}

					if (g_aEnemy[nCntEnemy].nPatternAnim >= 8)
					{
						g_aEnemy[nCntEnemy].nPatternAnim = 0;
					}

					if (g_aEnemy[nCntEnemy].bDirection == false)
					{
						pVtx[0].tex = D3DXVECTOR2(0.0f + g_aEnemy[nCntEnemy].nPatternAnim * 0.125f, 0.0f);
						pVtx[1].tex = D3DXVECTOR2(0.125f + g_aEnemy[nCntEnemy].nPatternAnim * 0.125f, 0.0f);
						pVtx[2].tex = D3DXVECTOR2(0.0f + g_aEnemy[nCntEnemy].nPatternAnim * 0.125f, 1.0f);
						pVtx[3].tex = D3DXVECTOR2(0.125f + g_aEnemy[nCntEnemy].nPatternAnim * 0.125f, 1.0f);
					}
					else if (g_aEnemy[nCntEnemy].bDirection == true)
					{
						pVtx[1].tex = D3DXVECTOR2(0.0f + g_aEnemy[nCntEnemy].nPatternAnim * 0.125f, 0.0f);
						pVtx[0].tex = D3DXVECTOR2(0.125f + g_aEnemy[nCntEnemy].nPatternAnim * 0.125f, 0.0f);
						pVtx[3].tex = D3DXVECTOR2(0.0f + g_aEnemy[nCntEnemy].nPatternAnim * 0.125f, 1.0f);
						pVtx[2].tex = D3DXVECTOR2(0.125f + g_aEnemy[nCntEnemy].nPatternAnim * 0.125f, 1.0f);
					}
				}
				else if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS4_STAND)
				{//�ҋ@���[�V����
					g_aEnemy[nCntEnemy].nCntAnim++;//�A�j���[�V�����J�E���^�[
					if (g_aEnemy[nCntEnemy].nCntAnim >= 10)
					{
						g_aEnemy[nCntEnemy].nPatternAnim++;
						g_aEnemy[nCntEnemy].nCntAnim = 0;
					}

					if (g_aEnemy[nCntEnemy].nPatternAnim >= 4)
					{
						g_aEnemy[nCntEnemy].nPatternAnim = 0;
					}

					if (g_aEnemy[nCntEnemy].bDirection == false)
					{
						pVtx[0].tex = D3DXVECTOR2(0.0f + g_aEnemy[nCntEnemy].nPatternAnim * 0.25f, 0.0f);
						pVtx[1].tex = D3DXVECTOR2(0.25f + g_aEnemy[nCntEnemy].nPatternAnim * 0.25f, 0.0f);
						pVtx[2].tex = D3DXVECTOR2(0.0f + g_aEnemy[nCntEnemy].nPatternAnim * 0.25f, 1.0f);
						pVtx[3].tex = D3DXVECTOR2(0.25f + g_aEnemy[nCntEnemy].nPatternAnim * 0.25f, 1.0f);
					}
					else if (g_aEnemy[nCntEnemy].bDirection == true)
					{
						pVtx[1].tex = D3DXVECTOR2(0.0f + g_aEnemy[nCntEnemy].nPatternAnim * 0.25f, 0.0f);
						pVtx[0].tex = D3DXVECTOR2(0.25f + g_aEnemy[nCntEnemy].nPatternAnim * 0.25f, 0.0f);
						pVtx[3].tex = D3DXVECTOR2(0.0f + g_aEnemy[nCntEnemy].nPatternAnim * 0.25f, 1.0f);
						pVtx[2].tex = D3DXVECTOR2(0.25f + g_aEnemy[nCntEnemy].nPatternAnim * 0.25f, 1.0f);
					}
				}
			}

			//================================
			//���X�{�X�̏���
			//================================
			if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_FINALBOSS_STAND ||
				g_aEnemy[nCntEnemy].nType == ENEMYTYPE_FINALBOSS_RUN ||
				g_aEnemy[nCntEnemy].nType == ENEMYTYPE_FINALBOSS_ATTACK)
			{
				g_aEnemy[nCntEnemy].nCountMoveInterval++;
				g_aEnemy[nCntEnemy].nApperCount++;//�G���o�����Ă���̎��Ԃ��J�E���g����B

				if (g_aEnemy[nCntEnemy].nCountMoveInterval == 1)
				{
					PlaySound(SOUND_LABEL_SE_SPECIALATTACK);
					StopSound(SOUND_LABEL_BGM010);
					SetGage(nCntEnemy, GAGETYPE_BOSS1HP);
					SetGage(-1, GAGETYPE_BOSSHP_FRAME);
					PlaySound(SOUND_LABEL_BGM005);
					g_nCntAttack = 0;
				}


				//================================
				//�`�ԕω�����
				//================================
				if (g_aEnemy[nCntEnemy].fLife <= g_aEnemy[nCntEnemy].fMaxHp / 4)
				{//�c��HP���ő�HP�̂S���̂P�ɂȂ�����
					if (g_aEnemy[nCntEnemy].bSpecialStaging == false)
					{
						g_aEnemy[nCntEnemy].bSpecialAttack = true;
						g_aEnemy[nCntEnemy].bSpecialStaging = true;
						/*SetStaging(STAGINGTYPE_FINALBOSS_SPECIAL_ATTACK, -1, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 180, ATTACKTYPE_PUNCH, 0.0f, -1, -1,
							-1, -1, 150);*/
						SetSpecial(SPECIALTYPE_FINALBOSS_SPECIAL_ATTACK, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 150);
						g_aEnemy[nCntEnemy].nApperCount = 0;
						g_nCntAttack = 0;
						g_nCntAttack2 = 0;
						g_nCntAttack3 = 0;
						g_nCntAttack = 0;
					}
				}

				//=========================================
				//�ʏ�U�������i�p�^�[���P�`�W�j
				//=========================================
				if (g_aEnemy[nCntEnemy].bSpecialAttack == false)
				{
					
					if (g_aEnemy[nCntEnemy].nApperCount == 1)
					{
						g_aEnemy[nCntEnemy].bGravity = true;
						g_nCntAttack = 0;
					}
				/*	g_aEnemy[nCntEnemy].bGravity = true;*/
					//======================
					//�p�^�[���P
					//======================
					if (g_aEnemy[nCntEnemy].nApperCount >= 2 && g_aEnemy[nCntEnemy].nApperCount <= 400)
					{
						g_aEnemy[nCntEnemy].nType = ENEMYTYPE_FINALBOSS_ATTACK;
						g_aEnemy[nCntEnemy].bGravity = false;
						g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 120.0f, 0.0f);

						if (g_aEnemy[nCntEnemy].nApperCount == 30)
						{
							SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, g_aEnemy[nCntEnemy].pos,
								110, ATTACKTYPE_FINALBEAM, -D3DX_PI * 0.25f, 50.0f, 0.05f, 50.0f, 1280.0f, 400);
						/*	SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, g_aEnemy[nCntEnemy].pos,
								110, ATTACKTYPE_FINALBEAM, -D3DX_PI * 0.75f, 50.0f, 0.05f, 50.0f, 1280.0f, 400);*/

							SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, g_aEnemy[nCntEnemy].pos,
								110, ATTACKTYPE_FINALBEAM, D3DX_PI * 0.75f, 50.0f, 0.05f, 50.0f, 1280.0f, 400);

							//SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, g_aEnemy[nCntEnemy].pos,
							//	110, ATTACKTYPE_FINALBEAM, D3DX_PI * 0.75f, 50.0f, 0.05f, 50.0f, 1280.0f, 400);
						}
					}

					if (g_aEnemy[nCntEnemy].nApperCount >= 450 && g_aEnemy[nCntEnemy].nApperCount <= 850)
					{
						if (g_aEnemy[nCntEnemy].nApperCount == 480)
						{
							SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, g_aEnemy[nCntEnemy].pos,
								110, ATTACKTYPE_FINALBEAM, -D3DX_PI * 0.75f, 50.0f, -0.05f, 50.0f, 1280.0f, 400);
							/*	SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, g_aEnemy[nCntEnemy].pos,
									110, ATTACKTYPE_FINALBEAM, -D3DX_PI * 0.75f, 50.0f, 0.05f, 50.0f, 1280.0f, 400);*/

							SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, g_aEnemy[nCntEnemy].pos,
								110, ATTACKTYPE_FINALBEAM, D3DX_PI * 0.25f, 50.0f, -0.05f, 50.0f, 1280.0f, 400);
						}

						if (g_aEnemy[nCntEnemy].nApperCount == 520)
						{
							for (int nCnt = 0; nCnt < 20; nCnt++)
							{
								SetStaging(STAGINGTYPE_EXPLOSIONWARNING, nCntEnemy, D3DXVECTOR3(pPlayer->pos.x+ sinf(-D3DX_PI * (0.1f * nCnt)) * 300.0f,
									pPlayer->pos.y + cosf(-D3DX_PI * (0.1f * nCnt)) * 300.0f, 0.0f), 60, ATTACKTYPE_PUNCH, -1.0f, 60.0f, -1.0f, 50.0f, 50.0f, 100);
							}
						}

						if (g_aEnemy[nCntEnemy].nApperCount == 670)
						{
							for (int nCnt = 0; nCnt < 20; nCnt++)
							{
								SetStaging(STAGINGTYPE_EXPLOSIONWARNING, nCntEnemy, D3DXVECTOR3(pPlayer->pos.x - 350.0f,
									36.0f * nCnt, 0.0f), 60, ATTACKTYPE_PUNCH, -1.0f, 60.0f, -1.0f, 50.0f, 50.0f, 100);

								SetStaging(STAGINGTYPE_EXPLOSIONWARNING, nCntEnemy, D3DXVECTOR3(pPlayer->pos.x + 350.0f,
									36.0f * nCnt, 0.0f), 60, ATTACKTYPE_PUNCH, -1.0f, 60.0f, -1.0f, 50.0f, 50.0f, 100);
							}
						}

						if (g_aEnemy[nCntEnemy].nApperCount == 790)
						{
							for (int nCnt = 0; nCnt < 20; nCnt++)
							{
								SetStaging(STAGINGTYPE_EXPLOSIONWARNING, nCntEnemy, D3DXVECTOR3(pPlayer->pos.x - 350.0f,
									36.0f * nCnt, 0.0f), 60, ATTACKTYPE_PUNCH, -1.0f, 60.0f, -1.0f, 50.0f, 50.0f, 100);

								SetStaging(STAGINGTYPE_EXPLOSIONWARNING, nCntEnemy, D3DXVECTOR3(pPlayer->pos.x + 350.0f,
									36.0f * nCnt, 0.0f), 60, ATTACKTYPE_PUNCH, -1.0f, 60.0f, -1.0f, 50.0f, 50.0f, 100);
							}
						}
					}

					if (g_aEnemy[nCntEnemy].nApperCount == 920)
					{
						for (int nCnt = 0; nCnt < 20; nCnt++)
						{
							SetStaging(STAGINGTYPE_EXPLOSIONWARNING, nCntEnemy, D3DXVECTOR3(64.0f * nCnt,
								SCREEN_HEIGHT, 0.0f), 60, ATTACKTYPE_PUNCH, -1.0f, 50.0f, -1.0f, 50.0f, 50.0f, 100);

							SetStaging(STAGINGTYPE_EXPLOSIONWARNING, nCntEnemy, D3DXVECTOR3(64.0f * nCnt,
								SCREEN_HEIGHT - 350.0f, 0.0f), 60, ATTACKTYPE_PUNCH, -1.0f, 60.0f, -1.0f, 50.0f, 50.0f, 100);
						}
					}

					if (g_aEnemy[nCntEnemy].nApperCount >= 1000 && g_aEnemy[nCntEnemy].nApperCount <= 1250)
					{
						g_aEnemy[nCntEnemy].bGravity = true;
						if (g_aEnemy[nCntEnemy].nApperCount == 1001)
						{
							g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, -30.0f, 0.0f);
						}
					
						float fRandumRot;
						float fRandumSpeed;
						float fRandumPosX;
						float fRandumPosY;
						fRandumRot = float((rand)() % 200 + 1) / 100.0f;
						fRandumSpeed = float((rand)() % 50) / 5.0f;
						fRandumPosX = float((rand)() % SCREEN_WIDTH + 1);
						fRandumPosY = float((rand)() % 360 + 1);
						SetBullet(g_aEnemy[nCntEnemy].pos,
							D3DXVECTOR3((sinf(-D3DX_PI * fRandumRot) * fRandumSpeed), (cosf(-D3DX_PI * fRandumRot) * fRandumSpeed), 0.0f),
							GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1, true, MOVETYPE_GRAVITY);

						if (g_aEnemy[nCntEnemy].pos.y <= 0.0f)
						{
							g_aEnemy[nCntEnemy].pos.x = pPlayer->pos.x;
						}
					}

					if (g_aEnemy[nCntEnemy].nApperCount >= 1251 && g_aEnemy[nCntEnemy].nApperCount <= 1600)
					{
						g_aEnemy[nCntEnemy].nType = ENEMYTYPE_FINALBOSS_RUN;
						float fRandumRot;
						float fRandumSpeed;
						float fRandumPosX;
						float fRandumPosY;
						fRandumRot = float((rand)() % 200 + 1) / 100.0f;
						fRandumSpeed = float((rand)() % 50) / 5.0f;
						fRandumPosX = float((rand)() % SCREEN_WIDTH + 1);
						fRandumPosY = float((rand)() % 360 + 1);
						if (g_nDelay % 70 == 0)
						{
							SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy,D3DXVECTOR3(fRandumPosX, fRandumPosY, 0.0f), 60, ATTACKTYPE_FINALBEAM,D3DX_PI * 0.5f, 50.0f, 0.025f, 50.0f,1280.0f,50);

							SetStaging(STAGINGTYPE_WARNING_ATTACK, nCntEnemy, D3DXVECTOR3(fRandumPosX, fRandumPosY, 0.0f), 60, ATTACKTYPE_FINALBEAM, -D3DX_PI * 0.5f, 50.0f, -0.025f, 50.0f, 1280.0f, 50);

							g_nCntAttack++;

						}

						if (g_nCntAttack == 1)
						{
							g_aEnemy[nCntEnemy].bGravity = false;
							g_aEnemy[nCntEnemy].move.y = 0.0f;
							if (g_aEnemy[nCntEnemy].bDirection == false)
							{
								g_aEnemy[nCntEnemy].move.x = 20.0f;
							}
							else
							{
								g_aEnemy[nCntEnemy].move.x = -20.0f;
							}
							g_nCntAttack++;
						}
						
						/*if (g_nDelay % 30 == 0 && g_nCntAttack >= 2 && g_nCntAttack <= 10)
						{
							SetAttack(g_aEnemy[nCntEnemy].pos,
								D3DXVECTOR3(0.0f, 0.0f, 0.0f),
								ATTACKTYPE_THUNDER_2,
								-1, -1, -1, MOVETYPE_NORMAL, 50.0f, 1500.0f, 60, D3DX_PI * 0.0f,0.0f);

							g_nCntAttack++;
						}*/

						if (g_aEnemy[nCntEnemy].pos.x >= SCREEN_WIDTH - 30.0f || g_aEnemy[nCntEnemy].pos.x <= 0.0f + 30.0f)
						{
							/*if (g_nCntAttack == 10)
							{*/
								float fRandumRot;
								float fRandumSpeed;
								fRandumRot = float((rand)() % 200 + 1) / 100.0f;
								fRandumSpeed = float((rand)() % 50) / 5.0f;

								for (int nCnt = 0; nCnt < 20; nCnt++)
								{
									SetBullet(g_aEnemy[nCntEnemy].pos,
										D3DXVECTOR3((sinf(-D3DX_PI * fRandumRot) * fRandumSpeed), (cosf(-D3DX_PI * fRandumRot) * fRandumSpeed), 0.0f),
										300, BULLETTYPE_JUMPING, 0, nCntEnemy, -1, false, MOVETYPE_GRAVITY);

									//SetBullet(g_aEnemy[nCntEnemy].pos,
									//	D3DXVECTOR3((sinf(-D3DX_PI * fRandumRot) * fRandumSpeed), (cosf(-D3DX_PI * fRandumRot) * fRandumSpeed), 0.0f),
									//	GENERALLY_BULLETLIFE, BULLETTYPE_JUMPING, 0, nCntEnemy, -1, false, MOVETYPE_GRAVITY);
								}
							//}
							g_nCntAttack = 0;
						
						}
					}

					//=====================
					//�p�^�[���Q
					//=====================

					if (g_aEnemy[nCntEnemy].nApperCount == 1680)
					{
						g_aEnemy[nCntEnemy].nType = ENEMYTYPE_FINALBOSS_RUN;
						g_nCntAttack = 0;
						g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						g_aEnemy[nCntEnemy].bGravity = true;
					}

					if (g_aEnemy[nCntEnemy].nApperCount >= 1750 && g_aEnemy[nCntEnemy].nApperCount <= 1850)
					{
						if (g_aEnemy[nCntEnemy].bDirection == false)
						{
							g_aEnemy[nCntEnemy].move.x = 10.0f;
						}
						else
						{
							g_aEnemy[nCntEnemy].move.x = -10.0f;
						}
					}

					if (g_aEnemy[nCntEnemy].nApperCount == 1860)
					{
						SetStaging(STAGINGTYPE_WARNING_ATTACK,
							nCntEnemy, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, 0.0f), 
							60, ATTACKTYPE_SHAKINGSWORD, D3DX_PI * 0.0f,200.0f, 0.5f, 50.0f, 300.0f, 100);

						for (int nCnt = 0; nCnt < 5; nCnt++)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y - 200.0f, 0.0f),
								D3DXVECTOR3((sinf(-D3DX_PI * (0.8f + 0.1f * nCnt)) * 15.0f), (cosf(-D3DX_PI * (0.7f + 0.1f * nCnt)) * 12.0f), 0.0f),
								300, BULLETTYPE_SNOW, 0, nCntEnemy, -1, true, MOVETYPE_GRAVITY);
						}

						for (int nCnt = 0; nCnt < 7; nCnt++)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y - 200.0f, 0.0f),
								D3DXVECTOR3((sinf(-D3DX_PI * (0.7f + 0.1f * nCnt)) * 20.0f), (cosf(-D3DX_PI * (0.5f + 0.1f * nCnt)) * 12.0f), 0.0f),
								300, BULLETTYPE_SNOW, 0, nCntEnemy, -1, true, MOVETYPE_GRAVITY);
						}
					}

					if (g_aEnemy[nCntEnemy].nApperCount >= 1950 && g_aEnemy[nCntEnemy].nApperCount <= 2050)
					{
						if (g_aEnemy[nCntEnemy].bDirection == false)
						{
							g_aEnemy[nCntEnemy].move.x = 14.0f;
						}
						else
						{
							g_aEnemy[nCntEnemy].move.x = -14.0f;
						}
					}


					if (g_aEnemy[nCntEnemy].nApperCount == 2070)
					{
						SetStaging(STAGINGTYPE_WARNING_ATTACK,
							nCntEnemy, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, 0.0f),
							60, ATTACKTYPE_SHAKINGSWORD, D3DX_PI * 0.0f, 200.0f, 0.5f, 50.0f, 300.0f, 100);

						for (int nCnt = 0; nCnt < 7; nCnt++)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y - 200.0f, 0.0f),
								D3DXVECTOR3((sinf(-D3DX_PI * (0.7f + 0.1f * nCnt)) * 15.0f), (cosf(-D3DX_PI * (0.7f + 0.1f * nCnt)) * 12.0f), 0.0f),
								300, BULLETTYPE_SNOW, 0, nCntEnemy, -1, true, MOVETYPE_GRAVITY);
						}

						for (int nCnt = 0; nCnt < 9; nCnt++)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y - 200.0f, 0.0f),
								D3DXVECTOR3((sinf(-D3DX_PI * (0.5f + 0.1f * nCnt)) * 20.0f), (cosf(-D3DX_PI * (0.5f + 0.1f * nCnt)) * 12.0f), 0.0f),
								300, BULLETTYPE_SNOW, 0, nCntEnemy, -1, true, MOVETYPE_GRAVITY);
						}
					}


					if (g_aEnemy[nCntEnemy].nApperCount >= 2150 && g_aEnemy[nCntEnemy].nApperCount <= 2300)
					{
						if (g_aEnemy[nCntEnemy].bDirection == false)
						{
							g_aEnemy[nCntEnemy].move.x = 18.0f;
						}
						else
						{
							g_aEnemy[nCntEnemy].move.x = -18.0f;
						}
						float fBullet[8];
						float fRandumRot;
						float fRandumSpeed;
						fRandumRot = float((rand)() % 200 + 1) / 100.0f;
					/*	fRandumSpeed = float((rand)() % 50) / 5.0f;*/
						if (g_nDelay % 10 == 0)
						{
							for (int nCnt = 0; nCnt < 8; nCnt++)
							{
								fBullet[nCnt] = (1.0f / 4.0f) * nCnt;

								SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].fHeight / 2 - 500.0f, 0.0f),
									D3DXVECTOR3((sinf(-D3DX_PI * fBullet[nCnt]) * 0.5f), (cosf(-D3DX_PI * fBullet[nCnt]) * 0.5f), 0.0f),
									300, BULLETTYPE_ICICLE, 0, nCntEnemy, -1, true, MOVETYPE_NORMAL);
							}
						}
					}

					if (g_aEnemy[nCntEnemy].nApperCount == 2320)
					{
						SetStaging(STAGINGTYPE_WARNING_ATTACK,
							nCntEnemy, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, 0.0f),
							60, ATTACKTYPE_SHAKINGSWORD, D3DX_PI * 0.0f, 200.0f, 0.5f, 50.0f, 300.0f, 100);

						for (int nCnt = 0; nCnt < 7; nCnt++)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x,g_aEnemy[nCntEnemy].pos.y - 200.0f,0.0f),
								D3DXVECTOR3((sinf(-D3DX_PI * (0.7f + 0.1f * nCnt)) * 15.0f), (cosf(-D3DX_PI * (0.7f + 0.1f * nCnt)) * 12.0f), 0.0f),
								300, BULLETTYPE_SNOW, 0, nCntEnemy, -1, true, MOVETYPE_GRAVITY);
						}

						for (int nCnt = 0; nCnt < 9; nCnt++)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y - 200.0f, 0.0f),
								D3DXVECTOR3((sinf(-D3DX_PI * (0.5f + 0.1f * nCnt)) * 20.0f), (cosf(-D3DX_PI * (0.5f + 0.1f * nCnt)) * 12.0f), 0.0f),
								300, BULLETTYPE_SNOW, 0, nCntEnemy, -1, true, MOVETYPE_GRAVITY);
						}
					}

					//=========================
					//�p�^�[���R
					//=========================

					if (g_aEnemy[nCntEnemy].nApperCount == 2360)
					{
						g_nCntAttack = 0;
						g_aEnemy[nCntEnemy].nType = ENEMYTYPE_FINALBOSS_STAND;
						g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						g_aEnemy[nCntEnemy].bGravity = false;
					}

					if (g_aEnemy[nCntEnemy].nApperCount >= 2600 && g_aEnemy[nCntEnemy].nApperCount <= 3000)
					{
						/*g_aEnemy[nCntEnemy].nType = MOTIONTYPE*/
						//if (g_aEnemy[nCntEnemy].bDirection == false)
						//{//���������Ă���
						//	g_aEnemy[nCntEnemy].move.x = 12.0f;
						//}
						//else
						//{//�E�������Ă���
						//	g_aEnemy[nCntEnemy].move.x = -12.0f;
						//}

						//if (g_aEnemy[nCntEnemy].bIsLanding == true)
						//{
						//	g_aEnemy[nCntEnemy].move.y = -20.0f;
						//}

						float fRandumPosX;
						float fRandumPosY;

						fRandumPosX = float((rand)() % SCREEN_WIDTH + 1);
						fRandumPosY = float((rand)() % SCREEN_HEIGHT + 1);

						g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(fRandumPosX, fRandumPosY, 0.0f);

						int nCntBullet_Boss2;
						float fCircleBullet_Boss2[12];
						float fRotBullet_Boss2;
						fRotBullet_Boss2 = float((rand)() % 200) / 100.0f;



						PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_004);
						g_fXVaim[nCntEnemy] = pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x;//X���W�ւ̃x�N�g��
						g_fYVaim[nCntEnemy] = pPlayer->pos.y- g_aEnemy[nCntEnemy].pos.y;//Y���W�ւ̃x�N�g��
						g_Vaim[nCntEnemy] = atan2(g_fXVaim[nCntEnemy], g_fYVaim[nCntEnemy]);//�e���������̌���
						g_aEnemy[nCntEnemy].bGravity = false;

						g_nCntAttack = 0;
						g_aEnemy[nCntEnemy].move = D3DXVECTOR3(sinf(g_Vaim[nCntEnemy]) * 20.0f, cosf(g_Vaim[nCntEnemy]) * 20.0f, 0.0f);

						if (g_nDelay % 3 == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].fHeight / 2, 0.0f),
								D3DXVECTOR3(sinf(g_Vaim[nCntEnemy]) * 10.0f, cosf(g_Vaim[nCntEnemy]) * 10.0f, 0.0f),
								300, BULLETTYPE_EXPLOSION, 0, nCntEnemy, -1, true, MOVETYPE_NORMAL);
						}
					}

					if (g_aEnemy[nCntEnemy].nApperCount == 3001)
					{
						g_nCntAttack = 0;
						g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						g_aEnemy[nCntEnemy].bGravity = true;
						g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);

						for (int nCnt = 0; nCnt < 5; nCnt++)
						{
							SetBullet(D3DXVECTOR3(SCREEN_WIDTH / 2 - 100.0f * nCnt,10.0f + 50.0f * nCnt,0.0f),
								D3DXVECTOR3(sinf(-D3DX_PI * 0.25f) * 10.0f, cosf(-D3DX_PI * 0.25f) * 10.0f, 0.0f),
								300, BULLETTYPE_EXPLOSION, 0, nCntEnemy, -1, true, MOVETYPE_NORMAL);

							SetBullet(D3DXVECTOR3(SCREEN_WIDTH / 2 - 100.0f * nCnt, 10.0f + 50.0f * nCnt, 0.0f),
								D3DXVECTOR3(sinf(-D3DX_PI * 1.75f) * 10.0f, cosf(-D3DX_PI * 1.75f) * 10.0f, 0.0f),
								300, BULLETTYPE_EXPLOSION, 0, nCntEnemy, -1, true, MOVETYPE_NORMAL);
						}

						SetStaging(STAGINGTYPE_NORMAL_WARNINGS,
							nCntEnemy, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 100.0f, g_aEnemy[nCntEnemy].pos.y, 0.0f),
							100, ATTACKTYPE_FIRE, D3DX_PI * 0.0f, 50.0f, 0.5f, 50.0f, 100.0f, 300);

						SetStaging(STAGINGTYPE_NORMAL_WARNINGS,
							nCntEnemy, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x +100.0f, g_aEnemy[nCntEnemy].pos.y, 0.0f),
							100, ATTACKTYPE_FIRE, D3DX_PI * 0.0f, 50.0f, 0.5f, 50.0f, 100.0f, 300);
					}

					//===========================
					//�p�^�[���S
					//===========================

					if (g_aEnemy[nCntEnemy].nApperCount == 3150)
					{
						g_aEnemy[nCntEnemy].nType = ENEMYTYPE_FINALBOSS_RUN;
						SetAttack(D3DXVECTOR3(SCREEN_WIDTH / 2 - 30.0f, 690.0f, 0.0f),
							D3DXVECTOR3(5.0f, 0.0f, 0.0f),
							ATTACKTYPE_THUNDER_2,
							-1, -1, -1, MOVETYPE_NORMAL, 50.0f, 1500.0f, 50, D3DX_PI * 0.0f, 0.0f);

						SetAttack(D3DXVECTOR3(SCREEN_WIDTH / 2 + 30.0f, 690.0f, 0.0f),
							D3DXVECTOR3(-5.0f, 0.0f, 0.0f),
							ATTACKTYPE_THUNDER_2,
							-1, -1, -1, MOVETYPE_NORMAL, 50.0f, 1500.0f, 50, D3DX_PI * 0.0f, 0.0f);


						for (int nCnt = 0; nCnt < 5; nCnt++)
						{
						
								SetStaging(STAGINGTYPE_WARNING_ATTACK,
									nCntEnemy, D3DXVECTOR3(pPlayer->pos.x - 100.0f * nCnt ,pPlayer->pos.y, 0.0f),
									60, ATTACKTYPE_ICICLECLAW, D3DX_PI * 0.0f, 50.0f, 0.5f, 50.0f, 150.0f, 40);

								SetStaging(STAGINGTYPE_WARNING_ATTACK,
									nCntEnemy, D3DXVECTOR3(pPlayer->pos.x + 100.0f * nCnt, pPlayer->pos.y, 0.0f),
									60, ATTACKTYPE_ICICLECLAW, D3DX_PI * 0.0f, 50.0f, 0.5f, 50.0f, 150.0f, 40);
							
						}
					

					}
						if (g_aEnemy[nCntEnemy].nApperCount >= 3300 && g_aEnemy[nCntEnemy].nApperCount <= 3700)
						{
							/*g_aEnemy[nCntEnemy].nType = MOTIONTYPE*/
							if (g_aEnemy[nCntEnemy].bDirection == false)
							{//���������Ă���
								g_aEnemy[nCntEnemy].move.x = 11.0f;
							}
							else
							{//�E�������Ă���
								g_aEnemy[nCntEnemy].move.x = -11.0f;
							}

							if (g_aEnemy[nCntEnemy].bIsLanding == true)
							{
								g_aEnemy[nCntEnemy].move.y = -25.0f;
							}

							int nCntBullet_Boss2;
							float fCircleBullet_Boss2[12];
							float fRotBullet_Boss2;
							fRotBullet_Boss2 = float((rand)() % 200) / 100.0f;

							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_004);

						
								int nCntBullet_E5;
										float fCircleBullet_E5_easy[4];
										float fCircleBullet_E5_normal[12];
										float fCircleBullet_E5_hard[16];
										float fCircleBullet_E5_veryhard[24];

										if (g_aEnemy[nCntEnemy].pos.y <= 100.0f && g_aEnemy[nCntEnemy].pos.y >= 0.0f && g_nCntAttack <= 2)
										{
											PlaySound(SOUND_LABEL_SE_ARROWSHOT_000);
											for (nCntBullet_E5 = 0; nCntBullet_E5 < 16; nCntBullet_E5++)
											{
												SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), GENERALLY_BULLETLIFE - 1200, BULLETTYPE_SATELLITE, nCntBullet_E5, nCntEnemy, -1, false,MOVETYPE_NORMAL);//�G�̒e������G���m�œ�����Ȃ�

											}
											g_nCntAttack++;
										}

										if (g_aEnemy[nCntEnemy].bIsLanding == true)
										{
											g_nCntAttack = 0;
										}
							
						}

						if (g_aEnemy[nCntEnemy].nApperCount == 3800)
						{
							g_nCntAttack = 0;
							g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
							g_aEnemy[nCntEnemy].bGravity = false;

							SetStaging(STAGINGTYPE_WARNING_ATTACK,
								nCntEnemy, D3DXVECTOR3(SCREEN_WIDTH / 2 - 500.0f,SCREEN_HEIGHT / 2, 0.0f),
								60, ATTACKTYPE_FINALBEAM, D3DX_PI * 0.0f, 50.0f, -0.04f, 50.0f, 480.0f, 80);

							SetStaging(STAGINGTYPE_WARNING_ATTACK,
								nCntEnemy, D3DXVECTOR3(SCREEN_WIDTH  / 2 + 500.0f, SCREEN_HEIGHT / 2, 0.0f),
								60, ATTACKTYPE_FINALBEAM, D3DX_PI * 0.0f, 50.0f, 0.04f, 50.0f, 480.0f, 80);

							SetStaging(STAGINGTYPE_NORMAL_WARNINGS,
								nCntEnemy, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
								60, ATTACKTYPE_FINALBEAM, D3DX_PI * 0.0f, 500.0f, 0.04f, 50.0f, 360.0f, 60);

						}
					

						//===========================
						//�p�^�[���T
						//===========================
						if (g_aEnemy[nCntEnemy].nApperCount == 3900)
						{
							g_aEnemy[nCntEnemy].nType = ENEMYTYPE_FINALBOSS_ATTACK;
							g_nCntAttack = 0;
							g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
							g_aEnemy[nCntEnemy].bGravity = true;
							g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(SCREEN_WIDTH,620.0f,0.0f);
							pPlayer->pos = D3DXVECTOR3(0.0f, 620.0f, 0.0f);
						}

						if (g_aEnemy[nCntEnemy].nApperCount >= 3950 && g_aEnemy[nCntEnemy].nApperCount <= 4600)
						{

							g_fXVaim[nCntEnemy] = pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x;//X���W�ւ̃x�N�g��
							g_fYVaim[nCntEnemy] = pPlayer->pos.y - g_aEnemy[nCntEnemy].pos.y;//Y���W�ւ̃x�N�g��
							g_Vaim[nCntEnemy] = atan2(g_fXVaim[nCntEnemy], g_fYVaim[nCntEnemy]);//�e���������̌���
							if (g_nDelay % 30 == 0)
							{
								/*SetStaging(STAGINGTYPE_NORMAL_WARNINGS,
									nCntEnemy, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 100.0f, g_aEnemy[nCntEnemy].pos.y, 0.0f),
									250, ATTACKTYPE_FIRE, D3DX_PI * 0.0f, 50.0f, 0.5f, 50.0f, 100.0f, 300);*/

								int nData = rand() % 3 + 1;

								if (g_nCntAttack == 0)
								{
									SetAttack(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, 0.0f),
										D3DXVECTOR3(-10.0f, 0.0f, 0.0f),
										ATTACKTYPE_ICICLECLAW,
										-1, -1, -1, MOVETYPE_NORMAL, 50.0f, 120.0f, 500, D3DX_PI * 0.0f, 0.0f);
								}
								else if (g_nCntAttack == 1)
								{
									SetAttack(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y - 120.0f, 0.0f),
										D3DXVECTOR3(-10.0f, 0.0f, 0.0f),
										ATTACKTYPE_ICICLECLAW,
										-1, -1, -1, MOVETYPE_NORMAL, 50.0f, 120.0f, 500, D3DX_PI * 1.0f, 0.0f);
								}
								else if (g_nCntAttack == 2)
								{
									SetAttack(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y - 120.0f, 0.0f),
										D3DXVECTOR3(-10.0f, 0.0f, 0.0f),
										ATTACKTYPE_ICICLECLAW,
										-1, -1, -1, MOVETYPE_NORMAL, 50.0f, 120.0f, 500, -D3DX_PI * 0.25f, 0.0f);

									SetAttack(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y - 120.0f, 0.0f),
										D3DXVECTOR3(-15.0f, 0.0f, 0.0f),
										ATTACKTYPE_ICICLECLAW,
										-1, -1, -1, MOVETYPE_NORMAL, 50.0f, 120.0f, 500, -D3DX_PI * 0.75f, 0.0f);
								}
								else if (g_nCntAttack == 3)
								{
									SetAttack(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y - 120.0f, 0.0f),
										D3DXVECTOR3(-10.0f, 0.0f, 0.0f),
										ATTACKTYPE_ICICLECLAW,
										-1, -1, -1, MOVETYPE_NORMAL, 50.0f, 120.0f, 500, -D3DX_PI * 0.0f, 0.0f);

									SetAttack(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y - 120.0f, 0.0f),
										D3DXVECTOR3(-15.0f, 0.0f, 0.0f),
										ATTACKTYPE_ICICLECLAW,
										-1, -1, -1, MOVETYPE_NORMAL, 50.0f, 120.0f, 500, -D3DX_PI * 0.0f, 0.0f);
								}
								else if (g_nCntAttack == 4)
								{
									SetAttack(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y - 120.0f, 0.0f),
										D3DXVECTOR3(sinf(g_Vaim[nCntEnemy]) * 12.0f, cosf(g_Vaim[nCntEnemy]) * 12.0f, 0.0f),
										ATTACKTYPE_SHAKINGSWORD,
										-1, -1, -1, MOVETYPE_NORMAL, 20.0f, 150.0f, 500, -D3DX_PI * 0.0f, 0.2f);


									SetAttack(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y - 120.0f, 0.0f),
										D3DXVECTOR3(sinf(g_Vaim[nCntEnemy]) * 6.0f, cosf(g_Vaim[nCntEnemy]) * 6.0f, 0.0f),
										ATTACKTYPE_SHAKINGSWORD,
										-1, -1, -1, MOVETYPE_NORMAL, 20.0f, 150.0f, 500, -D3DX_PI * 0.0f, 0.2f);
								}
								else if (g_nCntAttack == 5)
								{
									SetAttack(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y - 120.0f, 0.0f),
										D3DXVECTOR3(sinf(g_Vaim[nCntEnemy]) * 12.0f, cosf(g_Vaim[nCntEnemy]) * 12.0f, 0.0f),
										ATTACKTYPE_SHAKINGSWORD,
										-1, -1, -1, MOVETYPE_NORMAL, 20.0f, 100.0f, 500, -D3DX_PI * 0.0f, 0.2f);


									SetAttack(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y - 120.0f, 0.0f),
										D3DXVECTOR3(sinf(g_Vaim[nCntEnemy]) * 6.0f, cosf(g_Vaim[nCntEnemy]) * 6.0f, 0.0f),
										ATTACKTYPE_SHAKINGSWORD,
										-1, -1, -1, MOVETYPE_NORMAL, 20.0f, 100.0f, 500, -D3DX_PI * 0.0f, 0.2f);
								}
								else if (g_nCntAttack == 6)
								{

									SetBullet(D3DXVECTOR3(pPlayer->pos.x, pPlayer->pos.y - 300.0f, 0.0f),
										D3DXVECTOR3(sinf(-D3DX_PI * 0.0f) * 4.0f, cosf(-D3DX_PI * 0.0f) * 4.0f, 0.0f),
										300, BULLETTYPE_EXPLOSION, 0, nCntEnemy, -1, true, MOVETYPE_NORMAL);

									SetBullet(D3DXVECTOR3(pPlayer->pos.x - 300.0f, pPlayer->pos.y - 300.0f, 0.0f),
										D3DXVECTOR3(sinf(-D3DX_PI * 1.75f) * 4.0f, cosf(-D3DX_PI * 1.75f) * 4.0f, 0.0f),
										300, BULLETTYPE_EXPLOSION, 0, nCntEnemy, -1, true, MOVETYPE_NORMAL);

									SetBullet(D3DXVECTOR3(pPlayer->pos.x + 300.0f, pPlayer->pos.y - 300.0f, 0.0f),
										D3DXVECTOR3(sinf(-D3DX_PI * 0.25f) * 4.0f, cosf(-D3DX_PI * 0.25f) * 4.0f, 0.0f),
										300, BULLETTYPE_EXPLOSION, 0, nCntEnemy, -1, true, MOVETYPE_NORMAL);

								}
								else if (g_nCntAttack == 7)
								{
									for (int nCnt = 0; nCnt < 7; nCnt++)
									{
										SetStaging(STAGINGTYPE_WARNING_ATTACK,
											nCntEnemy, D3DXVECTOR3(pPlayer->pos.x - 200.0f + 50.0f * nCnt, pPlayer->pos.y, 0.0f),
											80, ATTACKTYPE_THUNDER_E4, D3DX_PI * 0.0f, 50.0f, 0.0f, 50.0f, 900.0f, 50);
									}
								}
								else if (g_nCntAttack == 8)
								{
									for (int nCnt = 0; nCnt < 7; nCnt++)
									{
										SetStaging(STAGINGTYPE_WARNING_ATTACK,
											nCntEnemy, D3DXVECTOR3(pPlayer->pos.x- 150.0f + 30.0f * nCnt, pPlayer->pos.y, 0.0f),
											80, ATTACKTYPE_THUNDER_E4, D3DX_PI * 0.0f, 50.0f, 0.0f, 50.0f, 900.0f, 50);
									}
								}
								else if (g_nCntAttack == 9)
								{
									SetStaging(STAGINGTYPE_WARNING_ATTACK,
										nCntEnemy, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 500.0f, g_aEnemy[nCntEnemy].pos.y - 300.0f, 0.0f),
										100, ATTACKTYPE_FINALBEAM, D3DX_PI * 1.0f, 500.0f, 0.05f, 50.0f, 0.0f, 30);

									SetStaging(STAGINGTYPE_WARNING_ATTACK,
										nCntEnemy, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 100.0f, g_aEnemy[nCntEnemy].pos.y - 50.0f, 0.0f),
										100, ATTACKTYPE_FINALBEAM, D3DX_PI * 1.0f, 50.0f, -0.05f, 50.0f, 1280.0f, 30);

									g_nCntAttack = 0;
								}
								g_nCntAttack++;
							}
						}

						//=====================
						//�p�^�[���U
						//=====================
						if (g_aEnemy[nCntEnemy].nApperCount == 4601)
						{
							g_nCntAttack = 0;
							g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
							g_aEnemy[nCntEnemy].bGravity = true;
							/*g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(SCREEN_WIDTH, 620.0f, 0.0f);*/
						/*	pPlayer->pos = D3DXVECTOR3(0.0f, 620.0f, 0.0f);*/
						}

						if (g_aEnemy[nCntEnemy].nApperCount >= 4801 && g_aEnemy[nCntEnemy].nApperCount <= 5600)
						{
					


							int nCntBullet_Boss5;
							float fCircleBullet_Boss5[12];
							float fRotBullet_Boss5;
							float fRotAttack;
							float fRotBullet2;
							float fRandumPosX;
							float fRandumPosY;
							fRotBullet_Boss5 = float((rand)() % 100) / 100.0f;
							fRotBullet2 = float((rand)() % 100) / 100.0f;
							fRotAttack = float((rand)() % 628 - 314) / 100.0f;
							fRandumPosX = float((rand)() % SCREEN_WIDTH + 1);
							fRandumPosY = float((rand)() % 360 + 1);

							g_fXVaim[nCntEnemy] = pPlayer->pos.x - fRandumPosX;//X���W�ւ̃x�N�g��
							g_fYVaim[nCntEnemy] = pPlayer->pos.y - fRandumPosY;//Y���W�ւ̃x�N�g��
							g_Vaim[nCntEnemy] = atan2(g_fXVaim[nCntEnemy], g_fYVaim[nCntEnemy]);//�x�N�g���̌���
						/*	int nData = rand() % 2 + 1;*/
							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_004);


							if (g_nDelay % 50 == 0)
							{

								/*	SetStaging(STAGINGTYPE_WARNING_ATTACK,
										nCntEnemy,
										g_aEnemy[nCntEnemy].pos,
										60, ATTACKTYPE_ICICLECLAW, g_Vaim[nCntEnemy] + D3DX_PI, 200.0f, -1, 50.0f, 700.0f, 20);*/
							}
						
								g_aEnemy[nCntEnemy].nType = ENEMYTYPE_FINALBOSS_ATTACK;//���[�V�����I��

								//=============================================
								//�ړ�����
								//=============================================
								if (g_aEnemy[nCntEnemy].bDirection == false)
								{//���������Ă���
									g_aEnemy[nCntEnemy].move.x = 7.0f;
								}
								else
								{//�E�������Ă���
									g_aEnemy[nCntEnemy].move.x = -7.0f;
								}
								//==============================================================

								if (pPlayer->pos.x >= g_aEnemy[nCntEnemy].pos.x - 300.0f && pPlayer->pos.x <= g_aEnemy[nCntEnemy].pos.x + 300.0f &&
									pPlayer->pos.y - pPlayer->fHeight / 2 >= g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].fHeight - 300.0f &&
									pPlayer->pos.y - pPlayer->fHeight / 2 <= g_aEnemy[nCntEnemy].pos.y + 300.0f)
								{//�v���C���[�ɐڋ߂�����U�����J�n�i�v���C���[�̒��S�_���ǂ͈͓̔��ɂ��邩�j

									if (g_nDelay % 60 == 0)
									{
										SetStaging(STAGINGTYPE_NORMAL_WARNINGS,
											nCntEnemy,
											g_aEnemy[nCntEnemy].pos,
											200, ATTACKTYPE_SLASH_E, -g_Vaim[nCntEnemy], 100.0f, -1, 50.0f, 120.0f, 200);

										if (g_aEnemy[nCntEnemy].bDirection == false)
										{
											SetAttack(g_aEnemy[nCntEnemy].pos,
												D3DXVECTOR3(sinf(-D3DX_PI* -fRotBullet_Boss5) * 3.0f,cosf(-D3DX_PI* -fRotBullet_Boss5) * 3.0f,0.0f),
													ATTACKTYPE_FLASHBLADE,
													-1, nCntEnemy, -1, MOVETYPE_NORMAL, 100.0f, 150.0f, 200,g_Vaim[nCntEnemy],0.0f);
										}
										else
										{
											SetAttack(g_aEnemy[nCntEnemy].pos,
												D3DXVECTOR3(sinf(-D3DX_PI * fRotBullet_Boss5) * 3.0f, cosf(-D3DX_PI * fRotBullet_Boss5) * 3.0f, 0.0f),
												ATTACKTYPE_FLASHBLADE,
												-1, nCntEnemy, -1, MOVETYPE_NORMAL, 100.0f, 150.0f, 200,fRotBullet_Boss5,0.0f);
										}
									}
								}
								else
								{//��̍U�������͈͊O

									if (g_nDelay % 50 == 0)
									{
										SetBullet(D3DXVECTOR3(pPlayer->pos.x, g_aEnemy[nCntEnemy].pos.y - pPlayer->fHeight / 2 -100.0f, 0.0f),
											D3DXVECTOR3(sinf(-D3DX_PI * 1.0f) * 23.0f, cosf(-D3DX_PI * 1.0f) * 23.0f, 0.0f),
											50, BULLETTYPE_SEED, -1, -1, -1, false, MOVETYPE_GRAVITY);
									}

								}

							

							if (g_nDelay % 60 == 0)
							{
								SetStaging(STAGINGTYPE_WARNING_ATTACK,
									nCntEnemy,
									D3DXVECTOR3(fRandumPosX,fRandumPosY,0.0f),
									60, ATTACKTYPE_FINALBEAM, g_Vaim[nCntEnemy] + D3DX_PI, 200.0f,0.0f, 100.0f, 1200.0f, 20);

								for (int nCnt = 0; nCnt < 15; nCnt++)
								{
									SetStaging(STAGINGTYPE_NORMAL_WARNINGS,
										nCntEnemy, D3DXVECTOR3(fRandumPosX + sinf(g_Vaim[nCntEnemy]) * 100.0f * nCnt, fRandumPosY + cosf(g_Vaim[nCntEnemy]) * 100.0f * nCnt, 0.0f),
										150, ATTACKTYPE_FINALBEAM,-g_Vaim[nCntEnemy] , 50.0f, 0.0f, 50.0f, 1280.0f, 30);
								}
							}

						}


						//=====================
						//�p�^�[���V
						//=====================
						if (g_aEnemy[nCntEnemy].nApperCount == 5701)
						{
							g_aEnemy[nCntEnemy].nType = ENEMYTYPE_FINALBOSS_STAND;
							g_nCntAttack = 0;
							g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
							g_aEnemy[nCntEnemy].bGravity = true;
							/*g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(SCREEN_WIDTH, 620.0f, 0.0f);*/
						/*	pPlayer->pos = D3DXVECTOR3(0.0f, 620.0f, 0.0f);*/
						}
						
						if (g_aEnemy[nCntEnemy].nApperCount == 6000)
						{
							g_nCntAttack = 0;
							g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
							g_aEnemy[nCntEnemy].bGravity = false;
							g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2, 0.0f);
						}

						if (g_aEnemy[nCntEnemy].nApperCount >= 6001 && g_aEnemy[nCntEnemy].nApperCount <= 7200)
						{
						
							float fRandumRot;
							fRandumRot = float((rand)() % 200) / 100.0f;
							if (g_nDelay % 120 == 0 )
							{
								if (g_nCntAttack == 0)
								{
									for (int nCnt = 0; nCnt < 10; nCnt++)
									{
										int nShotr = rand() % 4 + 1;
										float fRotBullet_Boss5;
										float fSpeed;
										fRotBullet_Boss5 = float((rand)() % 100) / 1000.0f;
										fSpeed = float((rand)() % 100) / 5.0f;

										if (nShotr == 1)
										{
											SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * (0.25f + fRotBullet_Boss5)) * (5.0f + fSpeed)), (cosf(-D3DX_PI * (0.25f + fRotBullet_Boss5)) * (5.0f + fSpeed)), 0.0f), 240, BULLETTYPE_ARROW, 0, -1, -1, false, MOVETYPE_NORMAL);
										}
										else if (nShotr == 2)
										{
											SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * (0.75f + fRotBullet_Boss5)) * (5.0f + fSpeed)), (cosf(-D3DX_PI * (0.75f + fRotBullet_Boss5)) * (5.0f + fSpeed)), 0.0f), 240, BULLETTYPE_ARROW, 0, -1, -1, false, MOVETYPE_NORMAL);
										}
										else if (nShotr == 3)
										{
											SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * (1.25f + fRotBullet_Boss5)) * (5.0f + fSpeed)), (cosf(-D3DX_PI * (1.25f + fRotBullet_Boss5)) * (5.0f + fSpeed)), 0.0f), 10, BULLETTYPE_ARROW, 0, -1, -1, false, MOVETYPE_NORMAL);
										}
										else if (nShotr == 4)
										{
											SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * (1.75f + fRotBullet_Boss5)) * (5.0f + fSpeed)), (cosf(-D3DX_PI * (1.75f + fRotBullet_Boss5)) * (5.0f + fSpeed)), 0.0f), 120, BULLETTYPE_ARROW, 0, -1, -1, false, MOVETYPE_NORMAL);
										}

									}
										g_nCntAttack++;
								}

								if (g_nCntAttack == 1)
								{
									SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * fRandumRot) * 4.0f), (cosf(-D3DX_PI * fRandumRot) * 4.0f), 0.0f), 300, BULLETTYPE_NOVA, 0, -1, -1, false, MOVETYPE_NORMAL);

									g_nCntAttack = 0;
								}
							}
						}

						if (g_aEnemy[nCntEnemy].nApperCount == 7301)
						{
							g_nCntAttack = 0;
							g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
							g_aEnemy[nCntEnemy].bGravity = true;
						/*	g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);*/
						}

						//====================
						//�p�^�[���W
						//====================
						if (g_aEnemy[nCntEnemy].nApperCount == 7550)
						{
							g_aEnemy[nCntEnemy].nType = ENEMYTYPE_FINALBOSS_ATTACK;
							g_aEnemy[nCntEnemy].bGravity = false;

							g_aEnemy[nCntEnemy].nAttackCount = 0;

							if (g_aEnemy[nCntEnemy].bDirection == false)
							{//�E�������Ă���
								g_aEnemy[nCntEnemy].bMovePattern = false;

								g_fSaveSpeed = 20.0f;
							}
							else
							{//���������Ă���
								g_aEnemy[nCntEnemy].bMovePattern = true;

								g_fSaveSpeed = -20.0f;
							}
						}
						if (g_aEnemy[nCntEnemy].nApperCount >= 7560 && g_aEnemy[nCntEnemy].nApperCount <= 8800)
						{
							g_aEnemy[nCntEnemy].nType = ENEMYTYPE_FINALBOSS_RUN;

							if (g_aEnemy[nCntEnemy].bMoveWay == false)
							{
								g_aEnemy[nCntEnemy].move.x = g_fSaveSpeed;
							}
							else
							{
								g_aEnemy[nCntEnemy].move.x = -g_fSaveSpeed;
							}
							float fBullet[8];
							if (g_nDelay % 60 == 0)
							{

								for (int nCnt = 0; nCnt < 8; nCnt++)
								{
									fBullet[nCnt] = (1.0f / 4.0f) * nCnt;

									SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].fHeight / 2 - 500.0f, 0.0f),
										D3DXVECTOR3((sinf(-D3DX_PI * fBullet[nCnt]) * 0.5f), (cosf(-D3DX_PI * fBullet[nCnt]) * 0.5f), 0.0f),
										300, BULLETTYPE_ICICLE, 0, nCntEnemy, -1, true, MOVETYPE_NORMAL);
								}
							}

							if (g_nDelay % 10 == 0)
							{
								
								

								if (g_nCntAttack >= 0 && g_nCntAttack <= 18)
								{
									SetStaging(STAGINGTYPE_WARNING_ATTACK,
										nCntEnemy,
										D3DXVECTOR3(pPlayer->pos.x - 500.0f + 20.0f * g_nCntAttack, pPlayer->pos.y - 300.0f, 0.0f),
										60, ATTACKTYPE_FLASHBLADE, D3DX_PI * 1.0f, 200.0f, 0.0f, 100.0f, 600.0f, 20);

									SetStaging(STAGINGTYPE_WARNING_ATTACK,
										nCntEnemy,
										D3DXVECTOR3(pPlayer->pos.x + 500.0f - 20.0f * g_nCntAttack, pPlayer->pos.y - 300.0f, 0.0f),
										60, ATTACKTYPE_FLASHBLADE, D3DX_PI * 1.0f, 200.0f, 0.0f, 100.0f, 600.0f, 20);

								}

								if (g_nCntAttack >= 27 && g_nCntAttack <= 32)
								{
									SetBullet(D3DXVECTOR3(pPlayer->pos.x,pPlayer->pos.y - 400.0f,0.0f), 
										D3DXVECTOR3(0.0f,0.0f, 0.0f), 240, BULLETTYPE_SPLET, 0, -1, -1, false, MOVETYPE_GRAVITY);
								}

								if (g_nCntAttack == 36)
								{
									SetStaging(STAGINGTYPE_WARNING_ATTACK,
										nCntEnemy,
										D3DXVECTOR3(pPlayer->pos.x + 300.0f, pPlayer->pos.y - 300.0f, 0.0f),
										60, ATTACKTYPE_FINALBEAM, D3DX_PI * 1.0f, 200.0f, -0.01f, 50.0f, 1200.0f, 60);

									SetStaging(STAGINGTYPE_WARNING_ATTACK,
										nCntEnemy,
										D3DXVECTOR3(pPlayer->pos.x - 300.0f, pPlayer->pos.y - 300.0f, 0.0f),
										60, ATTACKTYPE_FINALBEAM, D3DX_PI * 1.0f, 200.0f, 0.01f, 50.0f, 1200.0f, 60);

									g_nCntAttack = 0;
								}
									g_nCntAttack++;
							}
						}

						//=======================
						//�ȍ~���[�v
						//=======================
						if (g_aEnemy[nCntEnemy].nApperCount == 8801)
						{
							g_aEnemy[nCntEnemy].nApperCount = 0;
						}

					
				}
			/*	g_aEnemy[nCntEnemy].bSpecialAttack = true;*/
				//================================
				//���X�{�X�K�E�Z����
				//================================
		/*		g_aEnemy[nCntEnemy].bSpecialAttack = true;*/
				if (g_aEnemy[nCntEnemy].bSpecialAttack == true)
				{
					if (g_aEnemy[nCntEnemy].nApperCount == 1)
					{
						g_aEnemy[nCntEnemy].nType = ENEMYTYPE_FINALBOSS_ATTACK;
						g_nCntAttack = 0;
						g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						g_aEnemy[nCntEnemy].bGravity = false;
						g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);

						/*g_aEnemy[nCntEnemy].nApperCount = 5600;*/
					}

					if (g_aEnemy[nCntEnemy].nApperCount == 100)
					{
						for (int nCnt = 0; nCnt < 4; nCnt++)
						{
							SetBlock(BLOCKTYPE_RENGA, D3DXVECTOR3(SCREEN_WIDTH / 2 - 300.0f, SCREEN_HEIGHT - 144.0f - 144.0f * nCnt,0.0f), 1500);
							SetBlock(BLOCKTYPE_RENGA, D3DXVECTOR3(SCREEN_WIDTH / 2 + 300.0f, SCREEN_HEIGHT - 144.0f - 144.0f * nCnt,0.0f), 1500);

						}
						SetBlock(BLOCKTYPE_RENGA, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 144.0f - 144.0f * 3, 0.0f), 1500);
						SetBlock(BLOCKTYPE_RENGA, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 1500);
						SetBlock(BLOCKTYPE_RENGA, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 144.0f, 0.0f), 1500);

					}

					if (g_aEnemy[nCntEnemy].nApperCount >= 200 && g_aEnemy[nCntEnemy].nApperCount <= 2800)
					{

					


						if (g_aEnemy[nCntEnemy].nApperCount == 201)
						{
							for (int nCnt = 0; nCnt < 13; nCnt++)
							{
								for (int nCnt2 = 0; nCnt2 < 8; nCnt2++)
								{
									SetStaging(STAGINGTYPE_NORMAL_WARNINGS,
										nCntEnemy, D3DXVECTOR3(128.0f * nCnt,SCREEN_HEIGHT - (72.0f * nCnt2), 0.0f),
										150, ATTACKTYPE_FINALBEAM, 0.0f, 50.0f, 0.0f, 50.0f, 1280.0f, 30);
								}
							}

						

						
						}

						if (g_nDelay % 5 == 0)
						{
							if (g_nCntAttack >= 10 && g_nCntAttack <= 23)
							{
								SetStaging(STAGINGTYPE_WARNING_ATTACK,
									nCntEnemy,
									D3DXVECTOR3(0.0f, SCREEN_HEIGHT - (40.0f * g_nCntAttack2), 0.0f),
									40, ATTACKTYPE_FINALBEAM, -D3DX_PI * 0.5f, 200.0f, 0.00f, 80.0f, 1200.0f, 300);

								SetStaging(STAGINGTYPE_WARNING_ATTACK,
									nCntEnemy,
									D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT - (40.0f * g_nCntAttack2), 0.0f),
									40, ATTACKTYPE_FINALBEAM, D3DX_PI * 0.5f, 200.0f, 0.00f, 80.0f, 1200.0f, 300);

								g_nCntAttack2++;
							}

							
							if (g_nCntAttack == 40)
							{
								Attack* pAttack = GetAttack();

								for (int nCntAttack = 0; nCntAttack < MAX_ATTACK; nCntAttack++, pAttack++)
								{
									if (pAttack->bUse == true)
									{
										pAttack->bUse = false;
									}
								}

								g_nCntAttack2 = 0;

							}
							
							if (g_nCntAttack == 45)
							{

								for (int nCnt = 0; nCnt < 13; nCnt++)
								{
									for (int nCnt2 = 0; nCnt2 < 8; nCnt2++)
									{
										SetStaging(STAGINGTYPE_NORMAL_WARNINGS,
											nCntEnemy, D3DXVECTOR3(128.0f * nCnt,72.0f * nCnt2, 0.0f),
											60, ATTACKTYPE_FINALBEAM, 0.0f, 50.0f, 0.0f, 50.0f, 1280.0f, 30);
									}
								}


								for (int nCnt = 0; nCnt < 13; nCnt++)
								{
									SetStaging(STAGINGTYPE_WARNING_ATTACK,
										nCntEnemy,
										D3DXVECTOR3(0.0f,40.0f * nCnt, 0.0f),
										30, ATTACKTYPE_FINALBEAM, -D3DX_PI * 0.5f, 200.0f, 0.00f, 80.0f, 1200.0f, 300);

									SetStaging(STAGINGTYPE_WARNING_ATTACK,
										nCntEnemy,
										D3DXVECTOR3(SCREEN_WIDTH,40.0f * nCnt, 0.0f),
										30, ATTACKTYPE_FINALBEAM, D3DX_PI * 0.5f, 200.0f, 0.00f, 80.0f, 1200.0f, 300);
								}
							}

							if (g_nCntAttack == 60)
							{
								Attack* pAttack = GetAttack();

								for (int nCntAttack = 0; nCntAttack < MAX_ATTACK; nCntAttack++, pAttack++)
								{
									if (pAttack->bUse == true)
									{
										pAttack->bUse = false;
									}
								}
							}

							if (g_nCntAttack == 66)
							{
								for (int nCnt = 0; nCnt < 15; nCnt++)
								{
									for (int nCnt2 = 0; nCnt2 < 6; nCnt2++)
									{
										SetStaging(STAGINGTYPE_NORMAL_WARNINGS,
											nCntEnemy, D3DXVECTOR3(100.0f * nCnt, SCREEN_HEIGHT -(100.0f * nCnt2), 0.0f),
											100, ATTACKTYPE_FINALBEAM, 0.0f, 100.0f, 0.0f, 50.0f, 1280.0f, 30);
									}
								}

								for (int nCnt = 0; nCnt < 9; nCnt++)
								{
									for (int nCnt2 = 0; nCnt2 < 4; nCnt2++)
									{
										SetStaging(STAGINGTYPE_NORMAL_WARNINGS,
											nCntEnemy, D3DXVECTOR3(100.0f * nCnt, 0.0f + 100.0f * nCnt2, 0.0f),
											100, ATTACKTYPE_FINALBEAM, 0.0f, 100.0f, 0.0f, 50.0f, 1280.0f, 30);
									}
								}

							}
							if (g_nCntAttack >= 70 && g_nCntAttack <= 94)
							{
								SetStaging(STAGINGTYPE_WARNING_ATTACK,
									nCntEnemy,
									D3DXVECTOR3(0.0f + 40.0f * g_nCntAttack2,0.0f, 0.0f),
									40, ATTACKTYPE_FINALBEAM, -D3DX_PI * 1.0f, 200.0f, 0.00f, 80.0f, 1280.0f, 300);
								g_nCntAttack2++;
							}
							if (g_nCntAttack >= 66 && g_nCntAttack <= 78)
							{
								SetStaging(STAGINGTYPE_WARNING_ATTACK,
									nCntEnemy,
									D3DXVECTOR3(0.0f, SCREEN_HEIGHT - (40.0f * g_nCntAttack3), 0.0f),
									40, ATTACKTYPE_FINALBEAM, D3DX_PI * -0.5f, 200.0f, 0.00f, 80.0f, 1280.0f, 300);
								g_nCntAttack3++;
							}
							if (g_nCntAttack == 106)
							{
								Attack* pAttack = GetAttack();

								for (int nCntAttack = 0; nCntAttack < MAX_ATTACK; nCntAttack++, pAttack++)
								{
									if (pAttack->bUse == true)
									{
										pAttack->bUse = false;
									}
								}
								g_nCntAttack2 = 0;
								g_nCntAttack3 = 0;
							}
						

							if (g_nCntAttack == 115)
							{
								for (int nCnt = 0; nCnt < 15; nCnt++)
								{
									for (int nCnt2 = 0; nCnt2 < 6; nCnt2++)
									{
										SetStaging(STAGINGTYPE_NORMAL_WARNINGS,
											nCntEnemy, D3DXVECTOR3(SCREEN_WIDTH - 100.0f * nCnt, 0.0f + (100.0f * nCnt2), 0.0f),
											100, ATTACKTYPE_FINALBEAM, 0.0f, 100.0f, 0.0f, 50.0f, 1280.0f, 30);
									}
								}

								for (int nCnt = 0; nCnt < 9; nCnt++)
								{
									for (int nCnt2 = 0; nCnt2 < 4; nCnt2++)
									{
										SetStaging(STAGINGTYPE_NORMAL_WARNINGS,
											nCntEnemy, D3DXVECTOR3(SCREEN_WIDTH -100.0f * nCnt,SCREEN_HEIGHT - 100.0f * nCnt2, 0.0f),
											100, ATTACKTYPE_FINALBEAM, 0.0f, 100.0f, 0.0f, 50.0f, 1280.0f, 30);
									}
								}

							}
							if (g_nCntAttack >= 120 && g_nCntAttack <= 144)
							{
								SetStaging(STAGINGTYPE_WARNING_ATTACK,
									nCntEnemy,
									D3DXVECTOR3(SCREEN_WIDTH - 40.0f * g_nCntAttack2, 0.0f, 0.0f),
									40, ATTACKTYPE_FINALBEAM, -D3DX_PI * 1.0f, 200.0f, 0.00f, 80.0f, 1280.0f, 300);
								g_nCntAttack2++;
							}
							if (g_nCntAttack >= 120 && g_nCntAttack <= 132)
							{
								SetStaging(STAGINGTYPE_WARNING_ATTACK,
									nCntEnemy,
									D3DXVECTOR3(SCREEN_WIDTH,0.0f + (40.0f * g_nCntAttack3), 0.0f),
									40, ATTACKTYPE_FINALBEAM, D3DX_PI * 0.5f, 200.0f, 0.00f, 80.0f, 1280.0f, 300);
								g_nCntAttack3++;
							}

							if (g_nCntAttack == 170)
							{
								Attack* pAttack = GetAttack();

								for (int nCntAttack = 0; nCntAttack < MAX_ATTACK; nCntAttack++, pAttack++)
								{
									if (pAttack->bUse == true)
									{
										pAttack->bUse = false;
									}
								}
								g_nCntAttack2 = 0;
								g_nCntAttack3 = 0;
							}

							if (g_nCntAttack >= 180 && g_nCntAttack <= 198)
							{
							/*	for (int nCnt = 0; nCnt < 15; nCnt++)
								{*/
										SetStaging(STAGINGTYPE_WARNING_ATTACK,
											nCntEnemy, D3DXVECTOR3(SCREEN_WIDTH - (50.0f * g_nCntAttack2),SCREEN_HEIGHT, 0.0f),
											500, ATTACKTYPE_FINALBEAM, -D3DX_PI * 0.25f, 100.0f, 0.0f, 50.0f, 1280.0f, 500);

										SetStaging(STAGINGTYPE_WARNING_ATTACK,
											nCntEnemy, D3DXVECTOR3(0.0f + (50.0f * g_nCntAttack2),0.0f, 0.0f),
											500, ATTACKTYPE_FINALBEAM, D3DX_PI * 0.75f, 100.0f, 0.0f, 50.0f, 1280.0f, 500);
									
								//}
							g_nCntAttack2++;
							}

							if (g_nCntAttack == 248)
							{
								Attack* pAttack = GetAttack();

								for (int nCntAttack = 0; nCntAttack < MAX_ATTACK; nCntAttack++, pAttack++)
								{
									if (pAttack->bUse == true)
									{
										pAttack->bUse = false;
									}
								}
								g_nCntAttack2 = 0;
								g_nCntAttack3 = 0;
							}

							if (g_nCntAttack >= 252 && g_nCntAttack <= 270)
							{
								/*	for (int nCnt = 0; nCnt < 15; nCnt++)
									{*/
								SetStaging(STAGINGTYPE_WARNING_ATTACK,
									nCntEnemy, D3DXVECTOR3(0.0f + (50.0f * g_nCntAttack2),SCREEN_HEIGHT, 0.0f),
									500, ATTACKTYPE_FINALBEAM, D3DX_PI * 0.25f, 100.0f, 0.0f, 50.0f, 1280.0f, 500);

								SetStaging(STAGINGTYPE_WARNING_ATTACK,
									nCntEnemy, D3DXVECTOR3(SCREEN_WIDTH - (50.0f * g_nCntAttack2),0.0f, 0.0f),
									500, ATTACKTYPE_FINALBEAM, -D3DX_PI * 0.75f, 100.0f, 0.0f, 50.0f, 1280.0f, 500);

								//}
								g_nCntAttack2++;
							}

							if (g_nCntAttack == 310)
							{
								Attack* pAttack = GetAttack();

								for (int nCntAttack = 0; nCntAttack < MAX_ATTACK; nCntAttack++, pAttack++)
								{
									if (pAttack->bUse == true)
									{
										pAttack->bUse = false;
									}
								}
								g_nCntAttack2 = 0;
								g_nCntAttack3 = 0;
							}

							if (g_nCntAttack == 320)
							{
								SetStaging(STAGINGTYPE_SAFE,
									nCntEnemy, pPlayer->pos,
									70, ATTACKTYPE_FINALBEAM, D3DX_PI * 0.25f, 100.0f, 0.0f, 50.0f, 1280.0f, 500);
							}

							if (g_nCntAttack >= 320 && g_nCntAttack <= 510 && g_bAttackFlag == true)
							{
								if (pPlayer->pos.x <= 0.0f + pPlayer->fWidth || pPlayer->pos.x >= SCREEN_WIDTH - pPlayer->fWidth)
								{
									Attack* pAttack = GetAttack();

									for (int nCntAttack = 0; nCntAttack < MAX_ATTACK; nCntAttack++, pAttack++)
									{
										if (pAttack->bUse == true)
										{
											pAttack->bUse = false;
										}
									}

									g_bAttackFlag = false;
								}
								
							}
							if (g_nCntAttack == 340)
							{
								g_bAttackFlag = true;
								for (int nCnt = 0; nCnt < 20; nCnt++)
								{
									/*	SetStaging(STAGINGTYPE_WARNING_ATTACK,
											nCntEnemy, D3DXVECTOR3(pPlayer->pos.x - 100.0f + (50.0f * nCnt),pPlayer->pos.y - 300.0f, 0.0f),
											500, ATTACKTYPE_FINALBEAM, D3DX_PI * 0.25f, 100.0f, 0.0f, 50.0f, 1280.0f, 500);*/

									SetAttack(D3DXVECTOR3(pPlayer->pos.x - 50.0f + (200.0f * nCnt), pPlayer->pos.y - 450.0f, 0.0f),
										D3DXVECTOR3(sinf(-D3DX_PI * 0.5f) * 1.0f, cosf(-D3DX_PI * 0.5f) * 1.0f, 0.0f),
										ATTACKTYPE_FINALBEAM,
										-1, nCntEnemy, -1, MOVETYPE_NORMAL, 50.0f, 1280.0f, 500, D3DX_PI * 0.75f, 0.0f);

									SetAttack(D3DXVECTOR3(pPlayer->pos.x + 50.0f - (200.0f * nCnt), pPlayer->pos.y - 450.0f, 0.0f),
										D3DXVECTOR3(sinf(-D3DX_PI * 0.5f) * 1.0f, cosf(-D3DX_PI * 0.5f) * 1.0f, 0.0f),
										ATTACKTYPE_FINALBEAM,
										-1, nCntEnemy, -1, MOVETYPE_NORMAL, 50.0f, 1280.0f, 500, -D3DX_PI * 0.75f, 0.0f);
								}
							}

							if (g_nCntAttack == 400)
							{
								Attack* pAttack = GetAttack();

								for (int nCntAttack = 0; nCntAttack < MAX_ATTACK; nCntAttack++, pAttack++)
								{
									if (pAttack->bUse == true)
									{
										pAttack->bUse = false;
									}
								}
								g_nCntAttack2 = 0;
								g_nCntAttack3 = 0;
							}

							if (g_nCntAttack == 400)
							{
								SetStaging(STAGINGTYPE_SAFE,
									nCntEnemy, pPlayer->pos,
									70, ATTACKTYPE_FINALBEAM, D3DX_PI * 0.25f, 100.0f, 0.0f, 50.0f, 1280.0f, 500);
							}

							if (g_nCntAttack == 420)
							{
								g_bAttackFlag = true;
								for (int nCnt = 0; nCnt < 20; nCnt++)
								{
									/*	SetStaging(STAGINGTYPE_WARNING_ATTACK,
											nCntEnemy, D3DXVECTOR3(pPlayer->pos.x - 100.0f + (50.0f * nCnt),pPlayer->pos.y - 300.0f, 0.0f),
											500, ATTACKTYPE_FINALBEAM, D3DX_PI * 0.25f, 100.0f, 0.0f, 50.0f, 1280.0f, 500);*/

									SetAttack(D3DXVECTOR3(pPlayer->pos.x - 50.0f + (200.0f * nCnt), pPlayer->pos.y - 450.0f, 0.0f),
										D3DXVECTOR3(sinf(-D3DX_PI * 1.5f) * 2.0f, cosf(-D3DX_PI * 1.5f) * 2.0f, 0.0f),
										ATTACKTYPE_FINALBEAM,
										-1, nCntEnemy, -1, MOVETYPE_NORMAL, 50.0f, 1280.0f, 500, D3DX_PI * 0.75f, 0.0f);

									SetAttack(D3DXVECTOR3(pPlayer->pos.x + 50.0f - (200.0f * nCnt), pPlayer->pos.y - 450.0f, 0.0f),
										D3DXVECTOR3(sinf(-D3DX_PI * 1.5f) * 2.0f, cosf(-D3DX_PI * 1.5f) * 2.0f, 0.0f),
										ATTACKTYPE_FINALBEAM,
										-1, nCntEnemy, -1, MOVETYPE_NORMAL, 50.0f, 1280.0f, 500, -D3DX_PI * 0.75f, 0.0f);
								}
							}

							if (g_nCntAttack == 450)
							{
								Attack* pAttack = GetAttack();

								for (int nCntAttack = 0; nCntAttack < MAX_ATTACK; nCntAttack++, pAttack++)
								{
									if (pAttack->bUse == true)
									{
										pAttack->bUse = false;
									}
								}
								g_nCntAttack2 = 0;
								g_nCntAttack3 = 0;
							}

							if (g_nCntAttack == 450)
							{
								SetStaging(STAGINGTYPE_SAFE,
									nCntEnemy, pPlayer->pos,
									70, ATTACKTYPE_FINALBEAM, D3DX_PI * 0.25f, 100.0f, 0.0f, 50.0f, 1280.0f, 500);
							}

							if (g_nCntAttack == 470)
							{
								g_bAttackFlag = true;
								for (int nCnt = 0; nCnt < 20; nCnt++)
								{
									/*	SetStaging(STAGINGTYPE_WARNING_ATTACK,
											nCntEnemy, D3DXVECTOR3(pPlayer->pos.x - 100.0f + (50.0f * nCnt),pPlayer->pos.y - 300.0f, 0.0f),
											500, ATTACKTYPE_FINALBEAM, D3DX_PI * 0.25f, 100.0f, 0.0f, 50.0f, 1280.0f, 500);*/

									SetAttack(D3DXVECTOR3(pPlayer->pos.x - 50.0f + (200.0f * nCnt), pPlayer->pos.y - 450.0f, 0.0f),
										D3DXVECTOR3(sinf(-D3DX_PI * 0.5f) * 4.0f, cosf(-D3DX_PI * 0.5f) * 4.0f, 0.0f),
										ATTACKTYPE_FINALBEAM,
										-1, nCntEnemy, -1, MOVETYPE_NORMAL, 50.0f, 1280.0f, 500, D3DX_PI * 0.75f, 0.0f);

									SetAttack(D3DXVECTOR3(pPlayer->pos.x + 50.0f - (200.0f * nCnt), pPlayer->pos.y - 450.0f, 0.0f),
										D3DXVECTOR3(sinf(-D3DX_PI * 0.5f) * 4.0f, cosf(-D3DX_PI * 0.5f) * 4.0f, 0.0f),
										ATTACKTYPE_FINALBEAM,
										-1, nCntEnemy, -1, MOVETYPE_NORMAL, 50.0f, 1280.0f, 500, -D3DX_PI * 0.75f, 0.0f);
								}
							}

							if (g_nCntAttack == 510)
							{
								Attack* pAttack = GetAttack();

								for (int nCntAttack = 0; nCntAttack < MAX_ATTACK; nCntAttack++, pAttack++)
								{
									if (pAttack->bUse == true)
									{
										pAttack->bUse = false;
									}
								}
								g_nCntAttack2 = 0;
								g_nCntAttack3 = 0;
							}
							

							g_nCntAttack++;
						}
					}

					if (g_aEnemy[nCntEnemy].nApperCount == 3000)
					{
						for (int nCnt = 0; nCnt < 4; nCnt++)
						{
							SetBlock(BLOCKTYPE_RENGA, D3DXVECTOR3(SCREEN_WIDTH / 2 - 300.0f, SCREEN_HEIGHT - 144.0f - 144.0f * nCnt, 0.0f), 1500);
							SetBlock(BLOCKTYPE_RENGA, D3DXVECTOR3(SCREEN_WIDTH / 2 + 300.0f, SCREEN_HEIGHT - 144.0f - 144.0f * nCnt, 0.0f), 1500);

						}
						SetBlock(BLOCKTYPE_RENGA, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 144.0f - 144.0f * 3, 0.0f), 1500);
						SetBlock(BLOCKTYPE_RENGA, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 1500);
						SetBlock(BLOCKTYPE_RENGA, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 144.0f, 0.0f), 1500);


						for (int nCnt = 0; nCnt < 13; nCnt++)
						{
							/*for (int nCnt2 = 0; nCnt2 < 8; nCnt2++)
							{*/
							SetStaging(STAGINGTYPE_NORMAL_WARNINGS,
								nCntEnemy, D3DXVECTOR3(0.0f + (128.0f * nCnt), SCREEN_HEIGHT / 2, 0.0f),
								150, ATTACKTYPE_FINALBEAM, 0.0f, 50.0f, 0.0f, 50.0f, 1280.0f, 30);

							SetStaging(STAGINGTYPE_NORMAL_WARNINGS,
								nCntEnemy, D3DXVECTOR3(SCREEN_WIDTH / 2, 0.0f + (72.0f * nCnt), 0.0f),
								150, ATTACKTYPE_FINALBEAM, 0.0f, 50.0f, 0.0f, 50.0f, 1280.0f, 30);

							SetStaging(STAGINGTYPE_NORMAL_WARNINGS,
								nCntEnemy, D3DXVECTOR3(0.0f + (128.0f * nCnt), 0.0f + (72.0f * nCnt), 0.0f),
								150, ATTACKTYPE_FINALBEAM, 0.0f, 50.0f, 0.0f, 50.0f, 1280.0f, 30);

							SetStaging(STAGINGTYPE_NORMAL_WARNINGS,
								nCntEnemy, D3DXVECTOR3(0.0f + (128.0f * nCnt), SCREEN_HEIGHT - (72.0f * nCnt), 0.0f),
								150, ATTACKTYPE_FINALBEAM, 0.0f, 50.0f, 0.0f, 50.0f, 1280.0f, 30);


							//	}
						}

					}

					if (g_aEnemy[nCntEnemy].nApperCount == 3200)
					{
						float fData = 0.015f;

						SetAttack(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
							D3DXVECTOR3(0.0f, 0.0f, 0.0f), 
							ATTACKTYPE_FINALBEAM,
							-1, nCntEnemy, -1, MOVETYPE_NORMAL, 50.0f, 1280.0f, 1000, D3DX_PI * 1.0f, -fData);

						SetAttack(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
							D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							ATTACKTYPE_FINALBEAM,
							-1, nCntEnemy, -1, MOVETYPE_NORMAL, 50.0f, 1280.0f, 1000, D3DX_PI * 0.75f, -fData);

						SetAttack(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
							D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							ATTACKTYPE_FINALBEAM,
							-1, nCntEnemy, -1, MOVETYPE_NORMAL, 50.0f, 1280.0f, 1000, D3DX_PI * 0.5f, -fData);

						SetAttack(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
							D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							ATTACKTYPE_FINALBEAM,
							-1, nCntEnemy, -1, MOVETYPE_NORMAL, 50.0f, 1280.0f, 1000, D3DX_PI * 0.25f, -fData);

						SetAttack(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
							D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							ATTACKTYPE_FINALBEAM,
							-1, nCntEnemy, -1, MOVETYPE_NORMAL, 50.0f, 1280.0f, 1000, D3DX_PI * 0.0f, -fData);

						SetAttack(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
							D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							ATTACKTYPE_FINALBEAM,
							-1, nCntEnemy, -1, MOVETYPE_NORMAL, 50.0f, 1280.0f, 1000, -D3DX_PI * 0.5f, -fData);

						SetAttack(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
							D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							ATTACKTYPE_FINALBEAM,
							-1, nCntEnemy, -1, MOVETYPE_NORMAL, 50.0f, 1280.0f, 1000, -D3DX_PI * 0.75f, -fData);

						SetAttack(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
							D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							ATTACKTYPE_FINALBEAM,
							-1, nCntEnemy, -1, MOVETYPE_NORMAL, 50.0f, 1280.0f, 1000, -D3DX_PI * 0.25f, -fData);
					}

					if (g_aEnemy[nCntEnemy].nApperCount == 4300)
					{
						SetStaging(STAGINGTYPE_NORMAL_WARNINGS,
							nCntEnemy, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
							150, ATTACKTYPE_FINALBEAM, 0.0f, 400.0f, 0.0f, 50.0f, 1280.0f, 30);

					}

					if (g_aEnemy[nCntEnemy].nApperCount >= 4500 && g_aEnemy[nCntEnemy].nApperCount <= 4800)
					{
						int nCntBullet_Boss5;
						float fCircleBullet_Boss5[12];
						float fRotBullet_Boss5;
						float fRotAttack;
						float fRotBullet2;
						float fRandumPosX;
						float fRandumPosY;
						fRotBullet_Boss5 = float((rand)() % 100) / 100.0f;
						fRotBullet2 = float((rand)() % 100) / 100.0f;
						fRotAttack = float((rand)() % 628 - 314) / 100.0f;
						fRandumPosX = float((rand)() % SCREEN_WIDTH + 1);
						fRandumPosY = float((rand)() % 360 + 1);

						g_fXVaim[nCntEnemy] = pPlayer->pos.x - fRandumPosX;//X���W�ւ̃x�N�g��
						g_fYVaim[nCntEnemy] = pPlayer->pos.y - fRandumPosY;//Y���W�ւ̃x�N�g��
						g_Vaim[nCntEnemy] = atan2(g_fXVaim[nCntEnemy], g_fYVaim[nCntEnemy]);//�x�N�g���̌���

						SetStaging(STAGINGTYPE_NORMAL_WARNINGS,
							nCntEnemy,
							pPlayer->pos,
							60, ATTACKTYPE_FINALBEAM, g_Vaim[nCntEnemy] + D3DX_PI, 50.0f, 0.0f, 100.0f, 1200.0f, 150);
						SetStaging(STAGINGTYPE_WARNING_ATTACK,
							nCntEnemy,
							D3DXVECTOR3(fRandumPosX, fRandumPosY, 0.0f),
							60, ATTACKTYPE_FINALBEAM, g_Vaim[nCntEnemy] + D3DX_PI, 200.0f, 0.0f, 100.0f,1200.0f,150);
					}

				/*	if (g_aEnemy[nCntEnemy].nApperCount == 5400)
					{
						for (int nCnt = 0; nCnt < 4; nCnt++)
						{
							SetBlock(BLOCKTYPE_RENGA, D3DXVECTOR3(SCREEN_WIDTH / 2 - 300.0f, SCREEN_HEIGHT - 144.0f - 144.0f * nCnt, 0.0f), 1500);
							SetBlock(BLOCKTYPE_RENGA, D3DXVECTOR3(SCREEN_WIDTH / 2 + 300.0f, SCREEN_HEIGHT - 144.0f - 144.0f * nCnt, 0.0f), 1500);

						}
						SetBlock(BLOCKTYPE_RENGA, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 144.0f - 144.0f * 3, 0.0f), 6000);
						SetBlock(BLOCKTYPE_RENGA, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 6000);
						SetBlock(BLOCKTYPE_RENGA, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 144.0f, 0.0f), 6000);
					}*/

					if (g_aEnemy[nCntEnemy].nApperCount == 5700)
					{
					/*	for (int nCnt = 0; nCnt < 4; nCnt++)
						{
							SetBlock(BLOCKTYPE_RENGA, D3DXVECTOR3(SCREEN_WIDTH / 2 - 300.0f, SCREEN_HEIGHT - 144.0f - 144.0f * nCnt, 0.0f), 6000);
							SetBlock(BLOCKTYPE_RENGA, D3DXVECTOR3(SCREEN_WIDTH / 2 + 300.0f, SCREEN_HEIGHT - 144.0f - 144.0f * nCnt, 0.0f), 6000);

						}
						SetBlock(BLOCKTYPE_RENGA, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 144.0f - 144.0f * 3, 0.0f), 6000);
						SetBlock(BLOCKTYPE_RENGA, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 6000);
						SetBlock(BLOCKTYPE_RENGA, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 144.0f, 0.0f), 6000);*/
					/*	for (int nCnt = 0; nCnt < 3; nCnt++)
						{*/
							float fRandumRot = float((rand)() % 200 + 1) / 100;

							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].fHeight / 2, 0.0f),
								D3DXVECTOR3((sinf(-D3DX_PI * fRandumRot) * 6.0f), (cosf(-D3DX_PI * fRandumRot) * 6.0f), 0.0f),
								3500, BULLETTYPE_FINALBALL, 0, nCntEnemy, -1, false, MOVETYPE_NORMAL);
						//}
					}
					if (g_aEnemy[nCntEnemy].nApperCount == 7500)
					{
						float fRandumRot = float((rand)() % 200 + 1) / 100;

						SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].fHeight / 2, 0.0f),
							D3DXVECTOR3((sinf(-D3DX_PI * fRandumRot) * 6.0f), (cosf(-D3DX_PI * fRandumRot) * 6.0f), 0.0f),
							1700, BULLETTYPE_FINALBALL, 0, nCntEnemy, -1, false, MOVETYPE_NORMAL);
						//}
					}
					if (g_aEnemy[nCntEnemy].nApperCount == 9200)
					{
						Attack* pAttack = GetAttack();

						for (int nCntAttack = 0; nCntAttack < MAX_ATTACK; nCntAttack++, pAttack++)
						{
							if (pAttack->bUse == true)
							{
								pAttack->bUse = false;
							}
						}
						g_nCntAttack2 = 0;
						g_nCntAttack3 = 0;
						g_nCntAttack = 0;
						g_aEnemy[nCntEnemy].nApperCount = 0;
						g_aEnemy[nCntEnemy].bSpecialAttack = false;
					}


				}

				//=======================================
				//�e�N�X�`���A�j���[�V��������
				//=======================================
				if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_FINALBOSS_ATTACK)
				{
					g_aEnemy[nCntEnemy].nCntAnim++;//�A�j���[�V�����J�E���^�[
					if (g_aEnemy[nCntEnemy].nCntAnim >= 5)
					{//�A�j���[�V�����̃X�s�[�h��������
						g_aEnemy[nCntEnemy].nPatternAnim++;
						g_aEnemy[nCntEnemy].nCntAnim = 0;
					}

					if (g_aEnemy[nCntEnemy].nPatternAnim >= 5)
					{//���[�v�p����
						g_aEnemy[nCntEnemy].nPatternAnim = 0;
					}

					if (g_aEnemy[nCntEnemy].bDirection == false)
					{
						pVtx[0].tex = D3DXVECTOR2(0.0f + g_aEnemy[nCntEnemy].nPatternAnim * 0.2f, 0.0f);
						pVtx[1].tex = D3DXVECTOR2(0.2f + g_aEnemy[nCntEnemy].nPatternAnim * 0.2f, 0.0f);
						pVtx[2].tex = D3DXVECTOR2(0.0f + g_aEnemy[nCntEnemy].nPatternAnim * 0.2f, 1.0f);
						pVtx[3].tex = D3DXVECTOR2(0.2f + g_aEnemy[nCntEnemy].nPatternAnim * 0.2f, 1.0f);
					}
					else if (g_aEnemy[nCntEnemy].bDirection == true)
					{
						pVtx[1].tex = D3DXVECTOR2(0.0f + g_aEnemy[nCntEnemy].nPatternAnim * 0.2f, 0.0f);
						pVtx[0].tex = D3DXVECTOR2(0.2f + g_aEnemy[nCntEnemy].nPatternAnim * 0.2f, 0.0f);
						pVtx[3].tex = D3DXVECTOR2(0.0f + g_aEnemy[nCntEnemy].nPatternAnim * 0.2f, 1.0f);
						pVtx[2].tex = D3DXVECTOR2(0.2f + g_aEnemy[nCntEnemy].nPatternAnim * 0.2f, 1.0f);
					}
				}
				else if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_FINALBOSS_RUN)
				{//�ړ����[�V����
					g_aEnemy[nCntEnemy].nCntAnim++;//�A�j���[�V�����J�E���^�[
					if (g_aEnemy[nCntEnemy].nCntAnim >= 5)
					{
						g_aEnemy[nCntEnemy].nPatternAnim++;
						g_aEnemy[nCntEnemy].nCntAnim = 0;
					}

					if (g_aEnemy[nCntEnemy].nPatternAnim >= 8)
					{
						g_aEnemy[nCntEnemy].nPatternAnim = 0;
					}

					if (g_aEnemy[nCntEnemy].bDirection == false)
					{
						pVtx[0].tex = D3DXVECTOR2(0.0f + g_aEnemy[nCntEnemy].nPatternAnim * 0.125f, 0.0f);
						pVtx[1].tex = D3DXVECTOR2(0.125f + g_aEnemy[nCntEnemy].nPatternAnim * 0.125f, 0.0f);
						pVtx[2].tex = D3DXVECTOR2(0.0f + g_aEnemy[nCntEnemy].nPatternAnim * 0.125f, 1.0f);
						pVtx[3].tex = D3DXVECTOR2(0.125f + g_aEnemy[nCntEnemy].nPatternAnim * 0.125f, 1.0f);
					}
					else if (g_aEnemy[nCntEnemy].bDirection == true)
					{
						pVtx[1].tex = D3DXVECTOR2(0.0f + g_aEnemy[nCntEnemy].nPatternAnim * 0.125f, 0.0f);
						pVtx[0].tex = D3DXVECTOR2(0.125f + g_aEnemy[nCntEnemy].nPatternAnim * 0.125f, 0.0f);
						pVtx[3].tex = D3DXVECTOR2(0.0f + g_aEnemy[nCntEnemy].nPatternAnim * 0.125f, 1.0f);
						pVtx[2].tex = D3DXVECTOR2(0.125f + g_aEnemy[nCntEnemy].nPatternAnim * 0.125f, 1.0f);
					}
				}
				else if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_FINALBOSS_STAND)
				{//�ҋ@���[�V����
					g_aEnemy[nCntEnemy].nCntAnim++;//�A�j���[�V�����J�E���^�[
					if (g_aEnemy[nCntEnemy].nCntAnim >= 10)
					{
						g_aEnemy[nCntEnemy].nPatternAnim++;
						g_aEnemy[nCntEnemy].nCntAnim = 0;
					}

					if (g_aEnemy[nCntEnemy].nPatternAnim >= 4)
					{
						g_aEnemy[nCntEnemy].nPatternAnim = 0;
					}

					if (g_aEnemy[nCntEnemy].bDirection == false)
					{
						pVtx[0].tex = D3DXVECTOR2(0.0f + g_aEnemy[nCntEnemy].nPatternAnim * 0.25f, 0.0f);
						pVtx[1].tex = D3DXVECTOR2(0.25f + g_aEnemy[nCntEnemy].nPatternAnim * 0.25f, 0.0f);
						pVtx[2].tex = D3DXVECTOR2(0.0f + g_aEnemy[nCntEnemy].nPatternAnim * 0.25f, 1.0f);
						pVtx[3].tex = D3DXVECTOR2(0.25f + g_aEnemy[nCntEnemy].nPatternAnim * 0.25f, 1.0f);
					}
					else if (g_aEnemy[nCntEnemy].bDirection == true)
					{
						pVtx[1].tex = D3DXVECTOR2(0.0f + g_aEnemy[nCntEnemy].nPatternAnim * 0.25f, 0.0f);
						pVtx[0].tex = D3DXVECTOR2(0.25f + g_aEnemy[nCntEnemy].nPatternAnim * 0.25f, 0.0f);
						pVtx[3].tex = D3DXVECTOR2(0.0f + g_aEnemy[nCntEnemy].nPatternAnim * 0.25f, 1.0f);
						pVtx[2].tex = D3DXVECTOR2(0.25f + g_aEnemy[nCntEnemy].nPatternAnim * 0.25f, 1.0f);
					}
				}
			}
				switch (g_aEnemy[nCntEnemy].State)
				{
				case ENEMYSTATE_NORMAL:
					if (g_nDelay % 60 == 0)
					{
						//SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * 0.0f) * 5.0f), (cosf(-D3DX_PI * 0.0f) * 5.0f), 0.0f), 10, BULLETTYPE_ENEMY);//�G�̒e������G���m�œ�����Ȃ�
					}
					break;
				case ENEMYSTATE_DAMAGE:
					g_aEnemy[nCntEnemy].nCounterState--;
					if (g_aEnemy[nCntEnemy].nCounterState <= 0)
					{
						g_aEnemy[nCntEnemy].State = ENEMYSTATE_DAMAGE;
						//���_�J���[�̐ݒ�
						pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//�ʏ�F�ɖ߂��B
						pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//�ʏ�F�ɖ߂��B
						pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//�ʏ�F�ɖ߂��B
						pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//�ʏ�F�ɖ߂��B
					}
					break;
				}




				//===============================================
				//�G�l�~�[�̓����i�A�N�V�����o�[�W����
				//===============================================
				if (g_aEnemy[nCntEnemy].bGravity == false)
				{
					if (g_aEnemy[nCntEnemy].pos.x > SCREEN_WIDTH || g_aEnemy[nCntEnemy].pos.x < 0.0f)
					{
						if (g_aEnemy[nCntEnemy].pos.x > SCREEN_WIDTH)
						{
							g_aEnemy[nCntEnemy].pos.x = SCREEN_WIDTH - 10.0f;
						}
						else if (g_aEnemy[nCntEnemy].pos.x < 0.0f)
						{
							g_aEnemy[nCntEnemy].pos.x = 0.0f + 10.0f;
						}

						if (g_aEnemy[nCntEnemy].bEnemyMoveX == false)
						{
							g_aEnemy[nCntEnemy].bEnemyMoveX = true;
						}
						else if (g_aEnemy[nCntEnemy].bEnemyMoveX == true)
						{
							g_aEnemy[nCntEnemy].bEnemyMoveX = false;
						}
					}
					if (g_aEnemy[nCntEnemy].pos.y > SCREEN_HEIGHT || g_aEnemy[nCntEnemy].pos.y < 0.0f)
					{
						if (g_aEnemy[nCntEnemy].pos.y > SCREEN_HEIGHT)
						{
							g_aEnemy[nCntEnemy].pos.y = SCREEN_HEIGHT - 10.0f;
						}
						else if (g_aEnemy[nCntEnemy].pos.y < 0.0f)
						{
							g_aEnemy[nCntEnemy].pos.y = 0.0f + 10.0f;
						}

						if (g_aEnemy[nCntEnemy].bEnemyMoveY == false)
						{
							g_aEnemy[nCntEnemy].bEnemyMoveY = true;
						}
						else if (g_aEnemy[nCntEnemy].bEnemyMoveY == true)
						{
							g_aEnemy[nCntEnemy].bEnemyMoveY = false;
						}
					}

					//==============================
					//�X�N���[���Z�[�o�[����
					//==============================

					if (g_aEnemy[nCntEnemy].bEnemyMoveX == false)
					{
						g_aEnemy[nCntEnemy].pos.x += g_aEnemy[nCntEnemy].move.x;
					}
					if (g_aEnemy[nCntEnemy].bEnemyMoveY == false)
					{
						g_aEnemy[nCntEnemy].pos.y += g_aEnemy[nCntEnemy].move.y;
					}
					if (g_aEnemy[nCntEnemy].bEnemyMoveX == true)
					{
						g_aEnemy[nCntEnemy].pos.x -= g_aEnemy[nCntEnemy].move.x;
					}
					if (g_aEnemy[nCntEnemy].bEnemyMoveY == true)
					{
						g_aEnemy[nCntEnemy].pos.y -= g_aEnemy[nCntEnemy].move.y;
					}
				}
					//==========================================
					//�d�͏���
					//==========================================

				if (g_aEnemy[nCntEnemy].bGravity == true)
				{
					if (g_aEnemy[nCntEnemy].bIsLanding == false)
					{//�n�ʂɂ��Ȃ�
						g_aEnemy[nCntEnemy].move.y += 0.5f;
					}

					if (g_aEnemy[nCntEnemy].move.y >= 20.0f)
					{
						g_aEnemy[nCntEnemy].move.y = 20.0f;
					}

					g_aEnemy[nCntEnemy].move.x += (0.0f - g_aEnemy[nCntEnemy].move.x) * 0.5f;



					//�����O�̍��W��������
					g_aEnemy[nCntEnemy].posOld.x = g_aEnemy[nCntEnemy].pos.x;

					g_aEnemy[nCntEnemy].posOld.y = g_aEnemy[nCntEnemy].pos.y;


					//�ړ�����
					g_aEnemy[nCntEnemy].pos.x += g_aEnemy[nCntEnemy].move.x;
					g_aEnemy[nCntEnemy].pos.y += g_aEnemy[nCntEnemy].move.y;
				}
			
		}
			
		int* pGameMode = GetChooseCommand();
	
		if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS1_ATTACK ||
			g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS1_STAND ||
			g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS1_RUN)
		{
			if (g_aEnemy[nCntEnemy].fLife <= 0.0f && g_aEnemy[nCntEnemy].bDefeatFlag == false)
			{
				StopSound(SOUND_LABEL_BGM004);

				SetItem(ITEMTYPE_REAFORB, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 60.0f, 0.0f),0);
				g_aEnemy[nCntEnemy].bDefeatFlag = true;//�G��|�ꂽ�����
				
				SetWarp(MAPTYPE_8,D3DXVECTOR3(SCREEN_WIDTH / 2,SCREEN_HEIGHT - 150.0f,0.0f),D3DXVECTOR3(SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2,0.0f));

				pDefeatBoss->bDefeatBoss1 = true;//�{�X�P���j�t���O��true�ɂ���B
				if (*pGameMode == 1)
				{
				SetGameState(GAMESTATE_END);
				}
			}
		}


		if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS2_ATTACK ||
			g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS2_STAND ||
			g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS2_RUN)
		{
			if (g_aEnemy[nCntEnemy].fLife <= 0.0f && g_aEnemy[nCntEnemy].bDefeatFlag == false)
			{
				StopSound(SOUND_LABEL_BGM004);
					//SetGameState(GAMESTATE_END);
				SetItem(ITEMTYPE_ICEORB, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 60.0f, 0.0f),0);
				g_aEnemy[nCntEnemy].bDefeatFlag = true;//�G��|�ꂽ�����
				SetWarp(MAPTYPE_8, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 150.0f, 0.0f), D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
				pDefeatBoss->bDefeatBoss2 = true;//�{�X�Q���j�t���O��true�ɂ���B

				if (*pGameMode == 1)
				{
					SetGameState(GAMESTATE_END);
				}
			}
		}

		if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS3_ATTACK ||
			g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS3_STAND ||
			g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS3_RUN)
		{
			if (g_aEnemy[nCntEnemy].fLife <= 0.0f && g_aEnemy[nCntEnemy].bDefeatFlag == false)
			{
				StopSound(SOUND_LABEL_BGM004);
					//SetGameState(GAMESTATE_END);
				SetItem(ITEMTYPE_FIREORB, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 60.0f, 0.0f),0);
				g_aEnemy[nCntEnemy].bDefeatFlag = true;//�G��|�ꂽ�����
				SetWarp(MAPTYPE_8, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 150.0f, 0.0f), D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
				pDefeatBoss->bDefeatBoss3 = true;//�{�X�R���j�t���O��true�ɂ���B

				if (*pGameMode == 1)
				{
					SetGameState(GAMESTATE_END);
				}
			}
		}

		if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS4_ATTACK ||
			g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS4_STAND ||
			g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS4_RUN)
		{
			if (g_aEnemy[nCntEnemy].fLife <= 0.0f && g_aEnemy[nCntEnemy].bDefeatFlag == false)
			{
				StopSound(SOUND_LABEL_BGM004);
					//SetGameState(GAMESTATE_END);
				SetItem(ITEMTYPE_THUNDERORB, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 60.0f, 0.0f),0);
				g_aEnemy[nCntEnemy].bDefeatFlag = true;//�G��|�ꂽ�����
				SetWarp(MAPTYPE_8, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 150.0f, 0.0f), D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
				pDefeatBoss->bDefeatBoss4 = true;//�{�X�S���j�t���O��true�ɂ���B

				if (*pGameMode == 1)
				{
					SetGameState(GAMESTATE_END);
				}
			}
		}

		if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_FINALBOSS_ATTACK ||
			g_aEnemy[nCntEnemy].nType == ENEMYTYPE_FINALBOSS_STAND ||
			g_aEnemy[nCntEnemy].nType == ENEMYTYPE_FINALBOSS_RUN)
		{
			if (g_aEnemy[nCntEnemy].fLife <= 0.0f && g_aEnemy[nCntEnemy].bDefeatFlag == false)
			{
				StopSound(SOUND_LABEL_BGM005);
				//SetGameState(GAMESTATE_END);
				SetItem(ITEMTYPE_LASTORB, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 60.0f, 0.0f),0);
				g_aEnemy[nCntEnemy].bDefeatFlag = true;//�G��|�ꂽ�����
			/*	SetWarp(MAPTYPE_8, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 150.0f, 0.0f), D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));*/
				//pDefeatBoss->bDefeatBoss4 = true;//�{�X�S���j�t���O��true�ɂ���B

				if (*pGameMode == 1)
				{
					SetGameState(GAMESTATE_END);
				}
			}
		}
	
		
	

		pVtx += 4;
	}
	
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEnemy->Unlock();

	/*if (g_aResult.bResultFlag == true)
	{
		SetMode(MODE_RESULT);
	}*/
}

//=========================================
//�G�̕`�揈��
//=========================================
void DrawEnemy(void)
{

	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

	int nCntEnemy;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffEnemy, 0, sizeof(VERTEX_2D));//���_�o�b�t�@�ւ̃|�C���^�ƒ��_���̍\���̂̃T�C�Y

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)//�|���S���̕`��
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{//�G���g�p����Ă���
			/*if (g_aEnemy[nCntEnemy].nApperCount >= 100)
			{*/

				if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE0_MINIPUNI)
				{
					pDevice->SetTexture(0, g_apTextureEnemy[ENEMYTYPE0_MINIPUNI]);
				}
				if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE1_HONOPUNI)
				{
					pDevice->SetTexture(0, g_apTextureEnemy[ENEMYTYPE1_HONOPUNI]);
				}
				if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE2_SODOPUNI)
				{
					pDevice->SetTexture(0, g_apTextureEnemy[ENEMYTYPE2_SODOPUNI]);
				}
				if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE3_ICESLIME)
				{
					pDevice->SetTexture(0, g_apTextureEnemy[ENEMYTYPE3_ICESLIME]);
				}
				if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE4_BILIBILISLIME)
				{
					pDevice->SetTexture(0, g_apTextureEnemy[ENEMYTYPE4_BILIBILISLIME]);
				}
				if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE5_SNIPERSLIME)
				{
					pDevice->SetTexture(0, g_apTextureEnemy[ENEMYTYPE5_SNIPERSLIME]);
				}
				if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE6_BOMUSLIME)
				{
					pDevice->SetTexture(0, g_apTextureEnemy[ENEMYTYPE6_BOMUSLIME]);
				}
				if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE7_FLOWERSLIME)
				{
					pDevice->SetTexture(0, g_apTextureEnemy[ENEMYTYPE7_FLOWERSLIME]);
				}
				if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE8_CATSLIME)
				{
					pDevice->SetTexture(0, g_apTextureEnemy[ENEMYTYPE8_CATSLIME]);
				}
				if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE9_STONESLIME)
				{
					pDevice->SetTexture(0, g_apTextureEnemy[ENEMYTYPE9_STONESLIME]);
				}
				if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE10_WITCHSLIME)
				{
					pDevice->SetTexture(0, g_apTextureEnemy[ENEMYTYPE10_WITCHSLIME]);
				}
				if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BANDY)
				{
					pDevice->SetTexture(0, g_apTextureEnemy[ENEMYTYPE_BANDY]);
				}
				if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS1_STAND)
				{
					pDevice->SetTexture(0, g_apTextureEnemy[ENEMYTYPE_BOSS1_STAND]);
				}
				if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS1_RUN)
				{
					pDevice->SetTexture(0, g_apTextureEnemy[ENEMYTYPE_BOSS1_RUN]);
				}
				if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS1_ATTACK)
				{
					pDevice->SetTexture(0, g_apTextureEnemy[ENEMYTYPE_BOSS1_ATTACK]);
				}
				if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS2_STAND)
				{
					pDevice->SetTexture(0, g_apTextureEnemy[ENEMYTYPE_BOSS2_STAND]);
				}
				if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS2_RUN)
				{
					pDevice->SetTexture(0, g_apTextureEnemy[ENEMYTYPE_BOSS2_RUN]);
				}
				if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS2_ATTACK)
				{
					pDevice->SetTexture(0, g_apTextureEnemy[ENEMYTYPE_BOSS2_ATTACK]);
				}
				if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS3_STAND)
				{
					pDevice->SetTexture(0, g_apTextureEnemy[ENEMYTYPE_BOSS3_STAND]);
				}
				if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS3_RUN)
				{
					pDevice->SetTexture(0, g_apTextureEnemy[ENEMYTYPE_BOSS3_RUN]);
				}
				if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS3_ATTACK)
				{
					pDevice->SetTexture(0, g_apTextureEnemy[ENEMYTYPE_BOSS3_ATTACK]);
				}
				if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS4_STAND)
				{
					pDevice->SetTexture(0, g_apTextureEnemy[ENEMYTYPE_BOSS4_STAND]);
				}
				if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS4_RUN)
				{
					pDevice->SetTexture(0, g_apTextureEnemy[ENEMYTYPE_BOSS4_RUN]);
				}
				if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS4_ATTACK)
				{
					pDevice->SetTexture(0, g_apTextureEnemy[ENEMYTYPE_BOSS4_ATTACK]);
				}
				if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_FINALBOSS_STAND)
				{
					pDevice->SetTexture(0, g_apTextureEnemy[ENEMYTYPE_FINALBOSS_STAND]);
				}
				if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_FINALBOSS_RUN)
				{
					pDevice->SetTexture(0, g_apTextureEnemy[ENEMYTYPE_FINALBOSS_RUN]);
				}
				if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_FINALBOSS_ATTACK)
				{
					pDevice->SetTexture(0, g_apTextureEnemy[ENEMYTYPE_FINALBOSS_ATTACK]);
				}
				/*else
				{
					pDevice->SetTexture(0, g_apTextureEnemy[nCntEnemy]);
				}*/

				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,//0�́A�`�悷��ŏ��̒��_�C���f�b�N�X�ŁA2�́A�`�悷��v���~�e�B�u��
					nCntEnemy * 4,
					2);
			//}
		}
	}


}

//=================================
//�G�̐ݒ菈��
//=================================
void SetEnemy(D3DXVECTOR3 pos, int nType, float fLife, D3DXVECTOR3 move)
{

	int nCntEnemy;
	VERTEX_2D* pVtx;
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == false)
		{//�G���g�p����Ă��Ȃ�
			g_nCntEnemyParticle[nCntEnemy] = 0;
			g_bSetParticle[nCntEnemy] = false;

			g_aEnemy[nCntEnemy].pos = pos;
			g_aEnemy[nCntEnemy].posOld = pos;
		
			//pVtx += (nCntEnemy * 4);
		/*	pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 30.0f, g_aEnemy[nCntEnemy].pos.y - 30.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 30.0f, g_aEnemy[nCntEnemy].pos.y - 30.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 30.0f, g_aEnemy[nCntEnemy].pos.y + 30.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 30.0f, g_aEnemy[nCntEnemy].pos.y + 30.0f, 0.0f);*/
			g_aEnemy[nCntEnemy].nType = nType;
			g_aEnemy[nCntEnemy].bUse = true;//�t���O��true�ɂ��A�G���o�������Ƃ��ɕ\�������悤�ɂ���B
			g_aEnemy[nCntEnemy].nCounterState = 100;//��Ԃ�ς��鎞�Ԃ�\���J�E���^�[
			g_aEnemy[nCntEnemy].fLife = fLife;//�̗�
			g_aEnemy[nCntEnemy].State = ENEMYSTATE_NORMAL;
			g_aEnemy[nCntEnemy].bSetEnemyUse = true;
			g_aEnemy[nCntEnemy].bDefeatFlag = false;
			g_aEnemy[nCntEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aEnemy[nCntEnemy].move = move;//�ړ���
			g_aEnemy[nCntEnemy].nCountMoveInterval = 0;
			g_aEnemy[nCntEnemy].nMoveTime = 0;
			g_aEnemy[nCntEnemy].bAttackInterval = false;
			g_aEnemy[nCntEnemy].nAttarkIntervalCount = 0;
			g_aEnemy[nCntEnemy].bBossSummonFlag = false;//�{�X�����t���O
			g_aEnemy[nCntEnemy].fBossMove = 0.0f;
			g_aEnemy[nCntEnemy].bBossMoveFlag = false;
			g_aEnemy[nCntEnemy].nApperCount = 0;
			g_aEnemy[nCntEnemy].bEnemyCountminusFlag = false;
			g_aEnemy[nCntEnemy].nCntAnim = 0;//�A�j���[�V�����J�E���^�[
			g_aEnemy[nCntEnemy].nPatternAnim = 0;//�A�j���[�V�����p�^�[��
			g_aEnemy[nCntEnemy].bEnemyMoveX = false;
			g_aEnemy[nCntEnemy].bEnemyMoveY = false;
			g_aEnemy[nCntEnemy].bIsLanding = false;//�n�ʂɂ��Ă��邩�ǂ����̏���
			g_aEnemy[nCntEnemy].fGravity = 0.0f;//�n�ʂɂ��Ă��邩�ǂ����̏���
			g_aEnemy[nCntEnemy].bDirection = false;//���E�ǂ���������Ă���̂��𔻒肷��t���O
			g_aEnemy[nCntEnemy].fXVaim = 0.0f;//���@�_��������Ƃ��̏���
			g_aEnemy[nCntEnemy].fYVaim = 0.0f;//���@�_��������Ƃ��̏���
			g_aEnemy[nCntEnemy].fVaim = 0.0f;//���@�_��������Ƃ��̏���
			g_aEnemy[nCntEnemy].nCntJumpTime = 0;//�W�����v���鎞�Ԃ��J�E���g����
			g_aEnemy[nCntEnemy].nTypeOld = 0;//1f�O�̃^�C�v
			g_aEnemy[nCntEnemy].nAttackCount = 0;//�s�����Ƃ̍U���񐔂��J�E���g����
			g_aEnemy[nCntEnemy].bMoveWay = false;//�������������߂�t���O
			g_aEnemy[nCntEnemy].bMovePattern = false;//���̏u�Ԃɓ���������ތ��߂�t���O
			g_aEnemy[nCntEnemy].bSpecialAttack = false;//�K�E�Z���g���Ă��邩�ǂ��������߂�t���O
			g_aEnemy[nCntEnemy].bSpecialStaging = false;//�K�E�Z�̉��o���������ǂ����𔻒肷��t���O
			g_aEnemy[nCntEnemy].bIsWalling = false;//�ǂɓ������Ă��邩�ǂ����̏���
			g_aEnemy[nCntEnemy].bGravity = true;//�d�͂������邩�ǂ����̃t���O


			switch (g_aEnemy[nCntEnemy].nType)
			{
				//=================
				//�U�R
				//=================
			case ENEMYTYPE0_MINIPUNI:
				g_aEnemy[nCntEnemy].fLife = 30.0f;//�G�̗̑�
				g_aEnemy[nCntEnemy].fWidth = 20.0f;//�G�̕�
				g_aEnemy[nCntEnemy].fHeight = 40.0f;//�G�̍���
				break;
			case ENEMYTYPE1_HONOPUNI:
				g_aEnemy[nCntEnemy].fLife = 50.0f;//�G�̗̑�
				g_aEnemy[nCntEnemy].fWidth = 30.0f;//�G�̕�
				g_aEnemy[nCntEnemy].fHeight = 60.0f;//�G�̍���
				break;
			case ENEMYTYPE2_SODOPUNI:
				g_aEnemy[nCntEnemy].fLife = 100.0f;//�G�̗̑�
				g_aEnemy[nCntEnemy].fWidth = 30.0f;//�G�̕�
				g_aEnemy[nCntEnemy].fHeight = 60.0f;//�G�̍���
				break;
			case ENEMYTYPE3_ICESLIME:
				g_aEnemy[nCntEnemy].fLife = 100.0f;//�G�̗̑�
				g_aEnemy[nCntEnemy].fWidth = 40.0f;//�G�̕�
				g_aEnemy[nCntEnemy].fHeight = 80.0f;//�G�̍���
				break;
			case ENEMYTYPE4_BILIBILISLIME:
				g_aEnemy[nCntEnemy].fLife = 100.0f;//�G�̗̑�
				g_aEnemy[nCntEnemy].fWidth = 40.0f;//�G�̕�
				g_aEnemy[nCntEnemy].fHeight = 80.0f;//�G�̍���
				break;
			case ENEMYTYPE5_SNIPERSLIME:
				g_aEnemy[nCntEnemy].fLife = 100.0f;//�G�̗̑�
				g_aEnemy[nCntEnemy].fWidth = 30.0f;//�G�̕�
				g_aEnemy[nCntEnemy].fHeight = 60.0f;//�G�̍���
				break;
			case ENEMYTYPE6_BOMUSLIME:
				g_aEnemy[nCntEnemy].fLife = 120.0f;//�G�̗̑�
				g_aEnemy[nCntEnemy].fWidth = 30.0f;//�G�̕��i���a�j
				g_aEnemy[nCntEnemy].fHeight = 60.0f;//�G�̍����i���a�j
				break;
			case ENEMYTYPE7_FLOWERSLIME:
				g_aEnemy[nCntEnemy].fLife = 150.0f;//�G�̗̑�
				g_aEnemy[nCntEnemy].fWidth = 40.0f;//�G�̕��i���a�j
				g_aEnemy[nCntEnemy].fHeight = 80.0f;//�G�̍����i���a�j
				break;
			case ENEMYTYPE8_CATSLIME:
				g_aEnemy[nCntEnemy].fLife = 150.0f;//�G�̗̑�
				g_aEnemy[nCntEnemy].fWidth = 40.0f;//�G�̕��i���a�j
				g_aEnemy[nCntEnemy].fHeight = 80.0f;//�G�̍����i���a�j
				break;
			case ENEMYTYPE9_STONESLIME:
				g_aEnemy[nCntEnemy].fLife = 200.0f;//�G�̗̑�
				g_aEnemy[nCntEnemy].fWidth = 50.0f;//�G�̕��i���a�j
				g_aEnemy[nCntEnemy].fHeight = 100.0f;//�G�̍����i���a�j
				break;
			case ENEMYTYPE10_WITCHSLIME:
				g_aEnemy[nCntEnemy].fLife = 250.0f;//�G�̗̑�
				g_aEnemy[nCntEnemy].fWidth = 50.0f;//�G�̕��i���a�j
				g_aEnemy[nCntEnemy].fHeight = 100.0f;//�G�̍����i���a�j
				break;
				//========================================================

				//=============
				//�{�X
				//============
			case ENEMYTYPE_BOSS1_STAND:
				g_aEnemy[nCntEnemy].fLife = 1500.0f;//�G�̗̑�
				g_aEnemy[nCntEnemy].fMaxHp = g_aEnemy[nCntEnemy].fLife;
				g_aEnemy[nCntEnemy].fWidth = 50.0f;//�G�̕��i���a�j
				g_aEnemy[nCntEnemy].fHeight = 100.0f;//�G�̍����i���a�j
				break;
			case ENEMYTYPE_BOSS2_STAND:
				g_aEnemy[nCntEnemy].fLife = 2000.0f;//�G�̗̑�
				g_aEnemy[nCntEnemy].fMaxHp = g_aEnemy[nCntEnemy].fLife;
				g_aEnemy[nCntEnemy].fWidth = 50.0f;//�G�̕��i���a�j
				g_aEnemy[nCntEnemy].fHeight = 100.0f;//�G�̍����i���a�j
				break;
			case ENEMYTYPE_BOSS3_STAND:
				g_aEnemy[nCntEnemy].fLife = 2000.0f;//�G�̗̑�
				g_aEnemy[nCntEnemy].fMaxHp = g_aEnemy[nCntEnemy].fLife;
				g_aEnemy[nCntEnemy].fWidth = 50.0f;//�G�̕��i���a�j
				g_aEnemy[nCntEnemy].fHeight = 100.0f;//�G�̍����i���a�j
				break;
			case ENEMYTYPE_BOSS4_STAND:
				g_aEnemy[nCntEnemy].fLife = 2500.0f;//�G�̗̑�
				g_aEnemy[nCntEnemy].fMaxHp = g_aEnemy[nCntEnemy].fLife;
				g_aEnemy[nCntEnemy].fWidth = 50.0f;//�G�̕��i���a�j
				g_aEnemy[nCntEnemy].fHeight = 100.0f;//�G�̍����i���a�j
				break;
			case ENEMYTYPE_FINALBOSS_STAND:
				g_aEnemy[nCntEnemy].fLife = 5000.0f;//�G�̗̑�
				g_aEnemy[nCntEnemy].fMaxHp = g_aEnemy[nCntEnemy].fLife;
				g_aEnemy[nCntEnemy].fWidth = 50.0f;//�G�̕��i���a�j
				g_aEnemy[nCntEnemy].fHeight = 100.0f;//�G�̍����i���a�j
				break;

			}

			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//SetStaging(STAGINGTYPE_APPER, nCntEnemy,pos);//�G���o������\��̈ʒu�ɓG���o������\��������B�B

	//g_aDefeat.DefeatEnemyCount++;//��ɂ���c��G�l�~�[�����Z�b�g���ꂽ���v���X����B

			//g_nNumEnemy++;//�G�̑����J�E���g�A�b�v
			break;
		
		}
		pVtx += 4;
	}
	g_pVtxBuffEnemy->Unlock();
}

//====================================
//�G�̃q�b�g����
//====================================
void HitEnemy(int nCntEnemy,int nDamage)
{
	int nCnt;
	/*GameCustom* pGameCustom = GetGameCustom();*/
	ResultType* pResultType = GetResultType();//���U���g�̎�ނ̍\���̂̃A�h���X�Ə����擾
	Result* pResult = GetResult();
	VERTEX_2D* pVtx;
	Player* pPlayer = GetPlayer();
	g_aEnemy[nCntEnemy].fLife -= nDamage;
	if (g_aEnemy[nCntEnemy].fLife <= 0.0f)
	{//�G�̗̑͂��O�ɂȂ�����E�E�E

		if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE0_MINIPUNI)
		{
			AddScore(100);
		}
		else if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE1_HONOPUNI)
		{
			AddScore(150);
		}
		else if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE2_SODOPUNI)
		{
			AddScore(250);
		}
		else if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE3_ICESLIME)
		{
			AddScore(300);
		}
		else if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE4_BILIBILISLIME)
		{
			AddScore(300);
		}
		else if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE5_SNIPERSLIME)
		{
			AddScore(350);
		}
		else if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE6_BOMUSLIME)
		{
			AddScore(350);
		}
		else if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE5_SNIPERSLIME)
		{
			AddScore(350);
		}
		else if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE6_BOMUSLIME)
		{
			AddScore(350);
		}
		else if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE7_FLOWERSLIME)
		{
			AddScore(400);
		}
		else if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE8_CATSLIME)
		{
			AddScore(500);
		}
		else if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE9_STONESLIME)
		{
			AddScore(1000);
		}
		else if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE10_WITCHSLIME)
		{
			AddScore(1500);
		}
		else if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS1_STAND ||
			g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS1_RUN ||
			g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS1_ATTACK ||
			g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS2_STAND ||
			g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS2_RUN ||
			g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS2_ATTACK ||
			g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS3_STAND ||
			g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS3_RUN ||
			g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS3_ATTACK ||
			g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS4_STAND ||
			g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS4_RUN ||
			g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS4_ATTACK || 
			g_aEnemy[nCntEnemy].nType == ENEMYTYPE_FINALBOSS_STAND ||
			g_aEnemy[nCntEnemy].nType == ENEMYTYPE_FINALBOSS_RUN ||
			g_aEnemy[nCntEnemy].nType == ENEMYTYPE_FINALBOSS_ATTACK)
		{
			int nData = GetHealth();
			AddScore(100 * nData);
		}

         		PlaySound(SOUND_LABEL_SE_DEFEAT_ENEMY);

		//
				SetExplosion(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), EXPLOSIONTYPE_NORMAL,20.0f,20.0f);//�G������ʒu�Ŕ���������B

		int nNomissScore;
		int	nNotSkillUseBonus;
		g_aEnemy[nCntEnemy].bUse = false;

		

	//===================================
	//�_���[�W�\�L����
	//===================================
		float fRandumRot;
		float fRandumSpeed;
		fRandumRot = float((rand)() % 629 - 314) / 100.0f;
		fRandumSpeed = float((rand)() % 30) / 10.0f;

		if (nDamage <= 5 && nDamage >= 0)
		{
			SetDamage(nDamage, 100, D3DXVECTOR3((sinf(-D3DX_PI * fRandumRot) * fRandumSpeed), (cosf(-D3DX_PI * fRandumRot) * fRandumSpeed), 0.0f), g_aEnemy[nCntEnemy].pos, 10.0f);
		}
		else if (nDamage <= 200 && nDamage >= 15)
		{
			SetDamage(nDamage, 100, D3DXVECTOR3((sinf(-D3DX_PI * fRandumRot) * fRandumSpeed), (cosf(-D3DX_PI * fRandumRot) * fRandumSpeed), 0.0f), g_aEnemy[nCntEnemy].pos, 15.0f);
		}
		else if (nDamage >= 300 && nDamage <= 201)
		{
			SetDamage(nDamage, 100, D3DXVECTOR3((sinf(-D3DX_PI * fRandumRot) * fRandumSpeed), (cosf(-D3DX_PI * fRandumRot) * fRandumSpeed), 0.0f), g_aEnemy[nCntEnemy].pos, 40.0f);
		}
		else if (nDamage >= 301)
		{
			SetDamage(nDamage, 100, D3DXVECTOR3((sinf(-D3DX_PI * fRandumRot) * fRandumSpeed), (cosf(-D3DX_PI * fRandumRot) * fRandumSpeed), 0.0f), g_aEnemy[nCntEnemy].pos, 80.0f);
		}
		//=======================================================================================================================================================================================


		if (g_aEnemy[nCntEnemy].bUse== false)
		{
			if (g_aEnemy[nCntEnemy].bEnemyCountminusFlag == false)
			{
				g_aEnemy[nCntEnemy].bEnemyCountminusFlag = true;//�G�̑����J�E���g�_�E�������̂�true�i�ρj
				if (g_aDefeat.DefeatEnemyCount > 0)
				{
					g_aDefeat.DefeatEnemyCount--;//�G�̎c�萔���}�C�i�X����B
			    }
			}
		}

		


		//if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOMUBY)
		//{
		//	int nCntBullet_E8;
		//	float fSpeedBullet_E8;
		//	float fCircleBullet_E8_easy[10];
		//	float fCircleBullet_E8_normal[30];
		//	float fCircleBullet_E8_hard[50];
		//	float fCircleBullet_E8_veryhard[100];
		//	float fRotBullet_E8;
		//	fRotBullet_E8 = float((rand)() % 200) / 100.0f;
		//	

		//	int nCntBullet4;
		//		PlaySound(SOUND_LABEL_SE_EXPLOSION_ENEMY_000);
		//		for (nCntBullet4 = 0; nCntBullet4 < 30; nCntBullet4++)
		//		{
		//			fCircleBullet_E8_normal[nCntBullet4] = (1.0f / (30 / 2)) * nCntBullet4;

		//			fSpeedBullet_E8 = rand() % 5 + 1;


		//			SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * fCircleBullet_E8_normal[nCntBullet4]) * fSpeedBullet_E8), (cosf(-D3DX_PI * fCircleBullet_E8_normal[nCntBullet4]) * fSpeedBullet_E8), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1, false);//�G�̒e������G���m�œ�����Ȃ�	

		//		}
		//		
		//	
		//	
		//}
			g_nNumEnemy++;//�G��l�ɂ���񂾂��G�̑����J�E���g�_�E��
		
		


		/*if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_DELBY)
		{
			AddScore(2000);
		}*/
		
	}
	else
	{//�G�Ƀ_���[�W��^������

	float fRandumRot;
	float fRandumSpeed;
	fRandumRot = float((rand)() % 629 - 314) / 100.0f;
	fRandumSpeed = float((rand)() % 30) / 10.0f;

	//===================================
	//�_���[�W�\�L����
	//===================================
	if (nDamage <= 5 && nDamage >= 0)
	{
		SetDamage(nDamage, 100, D3DXVECTOR3((sinf(-D3DX_PI * fRandumRot) * fRandumSpeed), (cosf(-D3DX_PI * fRandumRot) * fRandumSpeed), 0.0f), g_aEnemy[nCntEnemy].pos, 10.0f);
	}
	else if (nDamage <= 200 && nDamage >= 15)
	{
		SetDamage(nDamage, 100, D3DXVECTOR3((sinf(-D3DX_PI * fRandumRot) * fRandumSpeed), (cosf(-D3DX_PI * fRandumRot) * fRandumSpeed), 0.0f), g_aEnemy[nCntEnemy].pos, 15.0f);
	}
	else if (nDamage >= 300 && nDamage <= 201)
	{
		SetDamage(nDamage, 100, D3DXVECTOR3((sinf(-D3DX_PI * fRandumRot) * fRandumSpeed), (cosf(-D3DX_PI * fRandumRot) * fRandumSpeed), 0.0f), g_aEnemy[nCntEnemy].pos, 40.0f);
	}
	else if (nDamage >= 301)
	{
		SetDamage(nDamage, 100, D3DXVECTOR3((sinf(-D3DX_PI * fRandumRot) * fRandumSpeed), (cosf(-D3DX_PI * fRandumRot) * fRandumSpeed), 0.0f), g_aEnemy[nCntEnemy].pos, 80.0f);
	}
	//=======================================================================================================================================================================================


		for (int nCnt = 0; nCnt < 5; nCnt++)
		{
			//SetParticle(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 0.5f, 0.0f, 0.8f), 20.0f, 100);
		}

		
		g_bSetParticle[nCntEnemy] = true;

		float fRandumDamageMove = float((rand)() % 629 - 314) / 100.0f;

	/*	SetDamage(fDamage, 30, D3DXVECTOR3((sinf(-D3DX_PI * fRandumDamageMove) * 3.0f), (cosf(-D3DX_PI * fRandumDamageMove) * 3.0f), 0.0f),D3DXVECTOR3(g_aEnemy[nCntEnemy].pos));*/

		g_aEnemy[nCntEnemy].State = ENEMYSTATE_DAMAGE;//�q�b�g���Ƀ_���[�W��Ԃɂ���
		g_aEnemy[nCntEnemy].nCounterState = 5;//�_���[�W��Ԃ�ۂ���
		//���_�J���[�̐ݒ�

		/*VERTEX_2D* pVtx;*/
		/*nDamage = 1;*/


		g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (4 * nCntEnemy);
		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);//�ԐF�ɂ���
		pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);//�ԐF�ɂ���
		pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);//�ԐF�ɂ���
		pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);//�ԐF�ɂ���
		AddScore(10);//�G�Ƀ_���[�W��^������X�R�A�����Z����
		g_pVtxBuffEnemy->Unlock();
	}

	
}
//==========================
//�G�̎擾
//==========================
Enemy* GetEnemy(void)
{
	return &g_aEnemy[0];//�G�̏��̐擪�A�h���X��Ԃ��B
}

//=======================================
//���U���g��ʂ��Ăяo���t���O�̎擾
//=======================================
Result* GetResult(void)
{
	return &g_aResult;
}

//=========================================
//�E�F�[�u�\���̂̎擾
//=========================================
Defeat* GetDefeat(void)
{
	return &g_aDefeat;
}

int DefeatEnemy(void)//�|�����G�̐�
{
	return g_nNumEnemy;
}