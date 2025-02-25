//========================================================
//
//�W���R�O���F�G�t�F�N�g������񌩒���[pause.cpp]
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
#include "Gage.h"
#include "staging.h"
#include "damage.h"
#include "wave.h"
#include "Difficulty.h"
//�}�N����`
#include "input.h"
#define NUM_ENEMY (13)//�G�̎��
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
//=====================================================
//==========================
//�G�̏���������
//==========================
void InitEnemy(void)
{
	GameCustom* pGameCustom = GetGameCustom();
	int nA = 0;
	
	g_nBulletShift = 0;
	
	g_bClearFlag = false;

	g_bNohitFlag = false;

	g_bBoss1Flag=false;

	g_nBoss1TechiniquePattern = 0;//������
	g_nCountTechiniqueTime = 0;//������

	g_fRotBullet_W6 = 0.0f;

	g_nE5_CurveCount = 0;

	//g_aDefeat.nDefeatCount = 0;//���݂̃E�F�[�u���O�ɂ���B

	g_aDefeat.DefeatEnemyCount = 0;//�G���j���̏�����
	int nCntBullet4;
	
	g_fSpeedBullet_E8;


		//srand((unsigned int)time(NULL));//�Q�[�����J�n�������Ԗ��ɗ����̎��ݒ�

	g_aResult.bResultFlag = false;
	int nCntBullet;
	
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

	g_nNumEnemy = MAX_ENEMY;//�G�̑����̏�����


	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ��݁i�G�̃e�N�X�`����ǂݍ��ށj
	D3DXCreateTextureFromFile(pDevice,
		"data\\enemy1_delby.png",
		&g_apTextureEnemy[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\enemy2_pundy.png",
		&g_apTextureEnemy[1]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\enemy3_smiley.png",
		&g_apTextureEnemy[2]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\enemy4_squby.png",
		&g_apTextureEnemy[3]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\enemy5_reafy.png",
		&g_apTextureEnemy[4]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\enemy6_honoby.png",
		&g_apTextureEnemy[5]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\enemy7_mizuby.png",
		&g_apTextureEnemy[6]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\enemy8_bomuby.png",
		&g_apTextureEnemy[7]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\enemy9_winby.png",
		&g_apTextureEnemy[8]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\enemy10_bolby.png",
		&g_apTextureEnemy[9]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\Enemy11_bandy.png",
		&g_apTextureEnemy[10]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\boss1_starwarrior.png",
		&g_apTextureEnemy[11]);
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
	GameCustom* pGameCustom = GetGameCustom();

	//LPDIRECT3DDEVICE9 pDevice;
	int	nCntBullet;
	int nCntEnemy;
	int nCntBullet2;
	int nCntBullet3;
	int nRandBullet2;

	/*g_aEnemy[nCntEnemy].fRotMove;*/
	VERTEX_2D* pVtx;

	Bullet* pBullet = GetBullet();//�e�\���̂̏����Q�b�g����B

	Player* pPlayer = GetPlayer();//�v���C���[�\���̂̏����Q�b�g����B

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

		pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 30.0f, g_aEnemy[nCntEnemy].pos.y - 30.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 30.0f, g_aEnemy[nCntEnemy].pos.y - 30.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 30.0f, g_aEnemy[nCntEnemy].pos.y + 30.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 30.0f, g_aEnemy[nCntEnemy].pos.y + 30.0f, 0.0f);





		if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS1_STARWARRIOR)
		{
			pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - BOSS1_SIZE, g_aEnemy[nCntEnemy].pos.y - BOSS1_SIZE, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + BOSS1_SIZE, g_aEnemy[nCntEnemy].pos.y - BOSS1_SIZE, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - BOSS1_SIZE, g_aEnemy[nCntEnemy].pos.y + BOSS1_SIZE, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + BOSS1_SIZE, g_aEnemy[nCntEnemy].pos.y + BOSS1_SIZE, 0.0f);
		}


		if (g_aEnemy[nCntEnemy].bUse == true)
		{

			g_aEnemy[nCntEnemy].nApperCount++;//�G�̏o���ʒu�����肵�Ă���o������܂ł̎��Ԃ��J�E���g����B

			/*if (g_bSetParticle[nCntEnemy] == true)
			{
				g_nCntEnemyParticle[nCntEnemy]++;

				SetParticle(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 25, 100);

				if (g_nCntEnemyParticle[nCntEnemy] >= 30)
				{
					g_nCntEnemyParticle[nCntEnemy] = 0;
					g_bSetParticle[nCntEnemy] = false;
				}
			}*/

			/*if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS1_STARWARRIOR)
			{
				if (g_bBoss1Flag == false)
				{
					g_bBoss1Flag = true;
					SetGage(nCntEnemy, GAGETYPE_BOSS1HP);
				}
			}*/
			if (g_aEnemy[nCntEnemy].nApperCount >= 100)
			{

				if (g_aEnemy[nCntEnemy].bUse == true && g_aEnemy[nCntEnemy].nType == ENEMYTYPE_DELBY)
				{//�G�l�~�[�O���g���Ă�����E�E�E
					switch (pGameCustom->nDifficulty)
					{
					case 1://��ՓxEASY
						if (g_nDelay % 80 == 0)
						{
							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_000);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * 0.0f) * 5.0f), (cosf(-D3DX_PI * 0.0f) * 5.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);
						}
						break;
					case 2://��ՓxNormal
						if (g_nDelay % 60 == 0)
						{
							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_000);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * 0.0f) * 5.0f), (cosf(-D3DX_PI * 0.0f) * 5.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);
						}
						break;
					case 3://��ՓxHard
						if (g_nDelay % 20 == 0)
						{
							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_000);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * 0.0f) * 5.0f), (cosf(-D3DX_PI * 0.0f) * 5.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);
						}
						break;
					case 4://��ՓxHard
						if (g_nDelay % 10 == 0)
						{
							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_000);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * 0.0f) * 5.0f), (cosf(-D3DX_PI * 0.0f) * 5.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);
						}
						break;
					}
					



				}

				if (g_aEnemy[nCntEnemy].bUse == true && g_aEnemy[nCntEnemy].nType == ENEMYTYPE_PUNDY)
				{//�G�l�~�[�Q�̍U��
					
					switch (pGameCustom->nDifficulty)
					{
					case 1://��ՓxEasy
						if (g_nDelay % 30 == 0)
						{
							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_000);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * g_fRandumResultBullet) * 5.0f), (cosf(-D3DX_PI * g_fRandumResultBullet) * 5.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�
						}
						break;
					case 2://��ՓxNormal
						if (g_nDelay % 15 == 0)
						{
							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_000);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * g_fRandumResultBullet) * 5.0f), (cosf(-D3DX_PI * g_fRandumResultBullet) * 5.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�
						}
						break;
					case 3://��ՓxHard
						if (g_nDelay %6  == 0)
						{
							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_000);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * g_fRandumResultBullet) * 5.0f), (cosf(-D3DX_PI * g_fRandumResultBullet) * 5.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�
						}
						break;
					case 4://��ՓxVeryHard
						if (g_nDelay % 2 == 0)
						{
							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_000);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * g_fRandumResultBullet) * 5.0f), (cosf(-D3DX_PI * g_fRandumResultBullet) * 5.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * g_fRandumResultBullet) * 2.0f), (cosf(-D3DX_PI * g_fRandumResultBullet) * 2.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�
						}
						break;
					}
					//SetBullet(g_aEnemy[1].pos, D3DXVECTOR3((sinf(-D3DX_PI * g_fRandumResultBullet) * 10.0f), (cosf(-D3DX_PI * g_fRandumResultBullet) * 10.0f), 0.0f), 10, BULLETTYPE_ENEMY);//�G�̒e������G���m�œ�����Ȃ�
					

				}

				if (g_aEnemy[nCntEnemy].bUse == true &&g_aEnemy[nCntEnemy].nType == ENEMYTYPE_SMILY)
				{//�G�l�~�[�R�̍U��
				
					if (g_bBulletDirectionFlag == false)
					{
						g_fBulletCount += 0.09f;
						if (g_nBulletShift % 4 == 0)
						{
							g_fBulletCount += 0.04f;
						}
					}



					if (g_bBulletDirectionFlag == true)
					{
						g_fBulletCount -= 0.09f;
						if (g_nBulletShift % 3 == 0)
						{
							g_fBulletCount -= 0.03f;
						}
					}

					if (g_fBulletCount >= 0.5f)
					{
						g_bBulletDirectionFlag = true;
					}
					if (g_fBulletCount <= -0.5f)
					{
						g_bBulletDirectionFlag = false;
					}

					switch (pGameCustom->nDifficulty)
					{
					case 1://��ՓxEasy
						if (g_nDelay % 45 == 0)
						{
							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_000);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * (0.0f + g_fBulletCount)) * 3.0f), (cosf(-D3DX_PI * (0.0f + g_fBulletCount)) * 4.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�

						}
						break;
					case 2://��ՓxNormal
						if (g_nDelay % 15 == 0)
						{
							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_000);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * (0.0f + g_fBulletCount)) * 3.0f), (cosf(-D3DX_PI * (0.0f + g_fBulletCount)) * 4.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�

						}
						break;
					case 3://��ՓxHard
						if (g_nDelay % 7 == 0)
						{
							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_000);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * (0.0f + g_fBulletCount)) * 3.0f), (cosf(-D3DX_PI * (0.0f + g_fBulletCount)) * 4.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�

						}
						break;
					case 4://��ՓxVeryHard
						if (g_nDelay % 3 == 0)
						{
							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_000);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * (0.0f + g_fBulletCount)) * 3.0f), (cosf(-D3DX_PI * (0.0f + g_fBulletCount)) * 4.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(D3DX_PI * (1.0f + g_fBulletCount)) * 3.0f), (cosf(D3DX_PI * (1.0f + g_fBulletCount)) * 4.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�


						}
						break;
					}
				


				}

				if (g_aEnemy[nCntEnemy].bUse == true && g_aEnemy[nCntEnemy].nType == ENEMYTYPE_SQUDY)
				{//�G�l�~�[�S�̍U���I�~�`�e�U�e
					


					int nCntBullet_E4;
					float fCircleBullet_E4_easy[8];
					float fCircleBullet_E4_normal[12];
					float fCircleBullet_E4_hard[16];
					float fCircleBullet_E4_veryhard[24];
					float fRotBullet_E4;
					fRotBullet_E4= float((rand)() % 200)/ 100.0f;

					switch (pGameCustom->nDifficulty)
					{
					case 1://Easy
						if (g_nDelay % 50 == 0)
						{
							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_004);

							for (nCntBullet_E4 = 0; nCntBullet_E4 < 8; nCntBullet_E4++)

							{
								fCircleBullet_E4_easy[nCntBullet_E4] = (1.0f / 4.0f) * nCntBullet_E4;

								SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * (fCircleBullet_E4_easy[nCntBullet_E4] + fRotBullet_E4)) * 4.0f), (cosf(-D3DX_PI * (fCircleBullet_E4_easy[nCntBullet_E4] + fRotBullet_E4)) * 4.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);

							}
						}
						break;
					case 2://Normal
						if (g_nDelay % 45 == 0)
						{
							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_004);

							for (nCntBullet_E4 = 0; nCntBullet_E4 < 12; nCntBullet_E4++)

							{
								fCircleBullet_E4_normal[nCntBullet_E4] = (1.0f / 6.0f) * nCntBullet_E4;

								SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * (fCircleBullet_E4_normal[nCntBullet_E4] + fRotBullet_E4)) * 4.0f), (cosf(-D3DX_PI * (fCircleBullet_E4_normal[nCntBullet_E4] + fRotBullet_E4)) * 4.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);

							}
						}
						break;
					case 3://Hard
						if (g_nDelay % 40 == 0)
						{
							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_004);

							for (nCntBullet_E4 = 0; nCntBullet_E4 < 16; nCntBullet_E4++)

							{
								fCircleBullet_E4_hard[nCntBullet_E4] = (1.0f / 8.0f) * nCntBullet_E4;

								SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * (fCircleBullet_E4_hard[nCntBullet_E4] + fRotBullet_E4)) * 4.0f), (cosf(-D3DX_PI * (fCircleBullet_E4_hard[nCntBullet_E4] + fRotBullet_E4)) * 4.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);

							}
						}
						break;
					case 4://VeryHard
						if (g_nDelay % 30 == 0)
						{
							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_004);

							for (nCntBullet_E4 = 0; nCntBullet_E4 < 24; nCntBullet_E4++)

							{
								fCircleBullet_E4_veryhard[nCntBullet_E4] = (1.0f / 12.0f) * nCntBullet_E4;

								SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * (fCircleBullet_E4_veryhard[nCntBullet_E4] + fRotBullet_E4)) * 4.0f), (cosf(-D3DX_PI * (fCircleBullet_E4_veryhard[nCntBullet_E4] + fRotBullet_E4)) * 4.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);

							}
						}
						break;

					}



					
				}

				if (g_aEnemy[nCntEnemy].bUse == true && g_nDelay % 60 == 0 && g_aEnemy[nCntEnemy].nType == ENEMYTYPE_REAFY)
				{//�G�l�~�[�T�̍U��(SetBullet)
				//	SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * 0.0f) * 5.0f), (cosf(-D3DX_PI * 0.0f) * 5.0f), 0.0f), 10, BULLETTYPE_REAFY);//�G�̒e������G���m�œ�����Ȃ�
					int nCntBullet_E5;
					float fCircleBullet_E5_easy[4];
					float fCircleBullet_E5_normal[12];
					float fCircleBullet_E5_hard[16];
					float fCircleBullet_E5_veryhard[24];
					switch (pGameCustom->nDifficulty)
					{
						
					case 1:
						PlaySound(SOUND_LABEL_SE_ARROWSHOT_000);
						for (nCntBullet_E5 = 0; nCntBullet_E5 < 4; nCntBullet_E5++)
						{
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_REAFY, nCntBullet_E5, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�

						}
						break;
					case 2:
						PlaySound(SOUND_LABEL_SE_ARROWSHOT_000);
						for (nCntBullet_E5 = 0; nCntBullet_E5 < 8; nCntBullet_E5++)
						{
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_REAFY, nCntBullet_E5, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�

						}
						break;
					case 3:
						PlaySound(SOUND_LABEL_SE_ARROWSHOT_000);
						for (nCntBullet_E5 = 0; nCntBullet_E5 < 14; nCntBullet_E5++)
						{
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_REAFY, nCntBullet_E5, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�

						}
						break;
					case 4:
						PlaySound(SOUND_LABEL_SE_ARROWSHOT_000);
						for (nCntBullet_E5 = 0; nCntBullet_E5 < 20; nCntBullet_E5++)
						{
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_REAFY, nCntBullet_E5, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�

						}
						break;

					}
				


				}

				


				if (g_aEnemy[nCntEnemy].bUse == true && g_nDelay % 60 == 0 && g_aEnemy[nCntEnemy].nType == ENEMYTYPE_HONOBY)
				{//�G�l�~�[6���g���Ă�����E�E�E
				


					g_fXVaim[nCntEnemy] = pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x;//X���W�ւ̃x�N�g��
					g_fYVaim[nCntEnemy] = pPlayer->pos.y - g_aEnemy[nCntEnemy].pos.y;//Y���W�ւ̃x�N�g��
				//g_Vaim[nCntEnemy] = sqrt((g_fXVaim[nCntEnemy] * g_fXVaim[nCntEnemy]) - (g_fYVaim[nCntEnemy] * g_fYVaim[nCntEnemy]));*���z�Ƃ͈�������A�ʔ��������̂ŃL�[�v���Ă���
					/*g_Vaim[nCntEnemy] = sqrtf((g_fXVaim[nCntEnemy] * g_fXVaim[nCntEnemy])+(g_fYVaim[nCntEnemy] * g_fYVaim[nCntEnemy]));*/

					g_Vaim[nCntEnemy] = atan2(g_fXVaim[nCntEnemy], g_fYVaim[nCntEnemy]);

					switch (pGameCustom->nDifficulty)
					{
					case 1://Easy
						PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_003);
						SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((D3DX_PI * g_fXVaim[nCntEnemy]) * 0.002f, (D3DX_PI * g_fYVaim[nCntEnemy]) * 0.002f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�
						break;
					case 2://Normal
						PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_003);
						SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((D3DX_PI * g_fXVaim[nCntEnemy]) * 0.004f, (D3DX_PI * g_fYVaim[nCntEnemy]) * 0.004f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�
						break;
					case 3://Hard
						PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_003);
						SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((D3DX_PI * g_fXVaim[nCntEnemy]) * 0.002f, (D3DX_PI * g_fYVaim[nCntEnemy]) * 0.002f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);
						SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((D3DX_PI* g_fXVaim[nCntEnemy]) * 0.004f, (D3DX_PI* g_fYVaim[nCntEnemy]) * 0.004f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);
						break;
					case 4://VeryHard
						PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_003);
						SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((D3DX_PI * g_fXVaim[nCntEnemy]) * 0.004f, (D3DX_PI * g_fYVaim[nCntEnemy]) * 0.004f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_SCATTERING, 0, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�
						break;
					}

					
					
				}

				if (g_aEnemy[nCntEnemy].bUse == true &&g_aEnemy[nCntEnemy].nType == ENEMYTYPE_MIZUBY)
				{//�G�l�~�[7���g���Ă�����E�E�E

					switch (pGameCustom->nDifficulty)
					{
					case 1://Easy
						if (g_nDelay % 54 == 0)
						{
							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_000);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * 0.0f + g_fRolingBulletX_E7) * 5.0f), (cosf(-D3DX_PI * 0.0f + g_fRolingBulletY_E7) * 5.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * 1.0f + g_fRolingBulletX_E7) * 5.0f), (cosf(-D3DX_PI * 1.0f + g_fRolingBulletY_E7) * 5.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * 0.0f - g_fRolingBulletX_E7) * 5.0f), (cosf(-D3DX_PI * 0.0f - g_fRolingBulletY_E7) * 5.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * 1.0f - g_fRolingBulletX_E7) * 5.0f), (cosf(-D3DX_PI * 1.0f - g_fRolingBulletY_E7) * 5.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�
						}
						break;
					case 2://Easy
						if (g_nDelay % 23== 0)
						{
							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_000);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * 0.0f + g_fRolingBulletX_E7) * 5.0f), (cosf(-D3DX_PI * 0.0f + g_fRolingBulletY_E7) * 5.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * 1.0f + g_fRolingBulletX_E7) * 5.0f), (cosf(-D3DX_PI * 1.0f + g_fRolingBulletY_E7) * 5.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * 0.0f - g_fRolingBulletX_E7) * 5.0f), (cosf(-D3DX_PI * 0.0f - g_fRolingBulletY_E7) * 5.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * 1.0f - g_fRolingBulletX_E7) * 5.0f), (cosf(-D3DX_PI * 1.0f - g_fRolingBulletY_E7) * 5.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�
						}
						break;
					case 3://Easy
						if (g_nDelay % 11 == 0)
						{
							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_000);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * 0.0f + g_fRolingBulletX_E7) * 5.0f), (cosf(-D3DX_PI * 0.0f + g_fRolingBulletY_E7) * 5.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * 1.0f + g_fRolingBulletX_E7) * 5.0f), (cosf(-D3DX_PI * 1.0f + g_fRolingBulletY_E7) * 5.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * 0.0f - g_fRolingBulletX_E7) * 5.0f), (cosf(-D3DX_PI * 0.0f - g_fRolingBulletY_E7) * 5.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * 1.0f - g_fRolingBulletX_E7) * 5.0f), (cosf(-D3DX_PI * 1.0f - g_fRolingBulletY_E7) * 5.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�
						}
						break;
					case 4://Easy
						if (g_nDelay % 6 == 0)
						{
							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_000);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * 0.0f + g_fRolingBulletX_E7) * 5.0f), (cosf(-D3DX_PI * 0.0f + g_fRolingBulletY_E7) * 5.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * 1.0f + g_fRolingBulletX_E7) * 5.0f), (cosf(-D3DX_PI * 1.0f + g_fRolingBulletY_E7) * 5.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * 0.0f - g_fRolingBulletX_E7) * 5.0f), (cosf(-D3DX_PI * 0.0f - g_fRolingBulletY_E7) * 5.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * 1.0f - g_fRolingBulletX_E7) * 5.0f), (cosf(-D3DX_PI * 1.0f - g_fRolingBulletY_E7) * 5.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�
						}
						break;
					}
					


				}

				if (g_aEnemy[nCntEnemy].bUse == true && g_nDelay % 5 == 0 && g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOMUBY)
				{//�G�l�~�[8���g���Ă�����E�E�E
					//SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * 0.0f) * 5.0f), (cosf(-D3DX_PI * 0.0f) * 5.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY);//�G�̒e������G���m�œ�����Ȃ�




				}
				if (g_aEnemy[nCntEnemy].bUse == true&& g_aEnemy[nCntEnemy].nType == ENEMYTYPE_WINBY)
				{//�G�l�~�[9���g���Ă�����E�E�E
					
					/*g_aEnemy[nCntEnemy].rot += D3DXVECTOR3(3.0f, 3.0f, 0.0f);*/

					g_rotPI.z += 0.1f;

					if (g_nDelay % 9 == 0)
					{
						g_rotPI.z += 0.019f;
					}
					switch (pGameCustom->nDifficulty)
					{
					case 1://Easy
						if (g_nDelay % 80 == 0)
						{
							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_006);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * g_rotPI.z) * g_fSpeedBullet_E9), (cosf(-D3DX_PI * g_rotPI.z) * g_fSpeedBullet_E9), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY9, 0, nCntEnemy, -1);//

						}
							break;
					case 2://Normal
						if (g_nDelay % 60 == 0)
						{
							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_006);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * g_rotPI.z) * g_fSpeedBullet_E9), (cosf(-D3DX_PI * g_rotPI.z) * g_fSpeedBullet_E9), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY9, 0, nCntEnemy, -1);//
						}

							break;
						
					case 3://Hard
						if (g_nDelay % 30 == 0)
						{
							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_006);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * g_rotPI.z) * g_fSpeedBullet_E9), (cosf(-D3DX_PI * g_rotPI.z) * g_fSpeedBullet_E9), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY9, 0, nCntEnemy, -1);//

						}
							break;
					case 4://VeryHard
						if (g_nDelay % 14 == 0)
						{
							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_006);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * g_rotPI.z) * g_fSpeedBullet_E9), (cosf(-D3DX_PI * g_rotPI.z) * g_fSpeedBullet_E9), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY9, 0, nCntEnemy, -1);//

						}
							break;
					}



				}


				if (g_aEnemy[nCntEnemy].bUse == true &&g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOLBY)
				{
					int nCircleBullet[4];
				
				
					switch (pGameCustom->nDifficulty)
					{
					case 1://Easy
						if (g_nDelay % 90 == 0)
						{
							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_003);
							for (nCntBullet2 = 0; nCntBullet2 < MAX_CIRCLEBULLET_E10; nCntBullet2++)
							{


								g_fCircleBullet[nCntBullet2] = (1.0f / 4.0f) * nCntBullet2;

								if (nCntBullet2 % 2 == 0)
								{
									SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * g_fCircleBullet[nCntBullet2] + g_fRandumResultBullet2) * 1.0f), (cosf(-D3DX_PI * g_fCircleBullet[nCntBullet2] + g_fRandumResultBullet2) * 1.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BOLBY, 0, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�		}
								}
								else
								{
									//SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * g_fCircleBullet[nCntBullet2] + g_fRandumResultBullet2) * 3.0f), (cosf(-D3DX_PI * g_fCircleBullet[nCntBullet2] + g_fRandumResultBullet2) * 3.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BOLBY, 0, nCntEnemy, -1);
								}
							}
						}
						break;
					case 2://Normal
						if (g_nDelay % 50 == 0)
						{
							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_003);
							for (nCntBullet2 = 0; nCntBullet2 < MAX_CIRCLEBULLET_E10; nCntBullet2++)
							{


								g_fCircleBullet[nCntBullet2] = (1.0f / 4.0f) * nCntBullet2;

								if (nCntBullet2 % 2 == 0)
								{
									SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * g_fCircleBullet[nCntBullet2] + g_fRandumResultBullet2) * 1.0f), (cosf(-D3DX_PI * g_fCircleBullet[nCntBullet2] + g_fRandumResultBullet2) * 1.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BOLBY, 0, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�		}
								}
								else
								{
									//SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * g_fCircleBullet[nCntBullet2] + g_fRandumResultBullet2) * 3.0f), (cosf(-D3DX_PI * g_fCircleBullet[nCntBullet2] + g_fRandumResultBullet2) * 3.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BOLBY, 0, nCntEnemy, -1);
								}
							}
						}
						break;
					case 3://Hard
						if (g_nDelay % 40 == 0)
						{
							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_003);
							for (nCntBullet2 = 0; nCntBullet2 < MAX_CIRCLEBULLET_E10; nCntBullet2++)
							{


								g_fCircleBullet[nCntBullet2] = (1.0f / 4.0f) * nCntBullet2;

								if (nCntBullet2 % 2 == 0)
								{
									SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * g_fCircleBullet[nCntBullet2] + g_fRandumResultBullet2) * 1.0f), (cosf(-D3DX_PI * g_fCircleBullet[nCntBullet2] + g_fRandumResultBullet2) * 1.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BOLBY, 0, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�		}
								}
								else
								{
									SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * g_fCircleBullet[nCntBullet2] + g_fRandumResultBullet2) * 3.0f), (cosf(-D3DX_PI * g_fCircleBullet[nCntBullet2] + g_fRandumResultBullet2) * 3.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BOLBY, 0, nCntEnemy, -1);
								}
							}
						}
						break;
					case 4://VeryHard
						if (g_nDelay % 30 == 0)
						{
							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_003);
							for (nCntBullet2 = 0; nCntBullet2 < 12; nCntBullet2++)
							{


								g_fCircleBullet[nCntBullet2] = (1.0f / 6.0f) * nCntBullet2;

								if (nCntBullet2 % 2 == 0)
								{
									SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * g_fCircleBullet[nCntBullet2] + g_fRandumResultBullet2) * 1.0f), (cosf(-D3DX_PI * g_fCircleBullet[nCntBullet2] + g_fRandumResultBullet2) * 1.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BOLBY, 0, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�		}
								}
								else
								{
									SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * g_fCircleBullet[nCntBullet2] + g_fRandumResultBullet2) * 3.0f), (cosf(-D3DX_PI * g_fCircleBullet[nCntBullet2] + g_fRandumResultBullet2) * 3.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BOLBY, 0, nCntEnemy, -1);
								}
							}
						}
						break;
					}

					//for (nCntBullet2 = 0; nCntBullet2 < MAX_CIRCLEBULLET_E10; nCntBullet2++)
					//{

					//	
					//	g_fCircleBullet[nCntBullet2] = (1.0f / 4.0f) * nCntBullet2;

					//	if (nCntBullet2 % 2 == 0)
					//	{
					//		SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * g_fCircleBullet[nCntBullet2] + g_fRandumResultBullet2) * 1.0f), (cosf(-D3DX_PI * g_fCircleBullet[nCntBullet2] + g_fRandumResultBullet2) * 1.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BOLBY, 0, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�		}
					//	}
					//	else
					//	{
					//		SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * g_fCircleBullet[nCntBullet2] + g_fRandumResultBullet2) * 3.0f), (cosf(-D3DX_PI * g_fCircleBullet[nCntBullet2] + g_fRandumResultBullet2) * 3.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BOLBY, 0, nCntEnemy, -1);
					//	}
					//}
				}

				if (g_aEnemy[nCntEnemy].bUse == true && g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BANDY)
				{//�G�l�~�[11���g���Ă�����E�E�E
					


					g_fXVaim[nCntEnemy] = pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x;//X���W�ւ̃x�N�g��
					g_fYVaim[nCntEnemy] = pPlayer->pos.y - g_aEnemy[nCntEnemy].pos.y;//Y���W�ւ̃x�N�g��
				//g_Vaim[nCntEnemy] = sqrt((g_fXVaim[nCntEnemy] * g_fXVaim[nCntEnemy]) - (g_fYVaim[nCntEnemy] * g_fYVaim[nCntEnemy]));*���z�Ƃ͈�������A�ʔ��������̂ŃL�[�v���Ă���
					/*g_Vaim[nCntEnemy] = sqrtf((g_fXVaim[nCntEnemy] * g_fXVaim[nCntEnemy])+(g_fYVaim[nCntEnemy] * g_fYVaim[nCntEnemy]));*/

					g_Vaim[nCntEnemy] = atan2(g_fXVaim[nCntEnemy], g_fYVaim[nCntEnemy]);

					switch (pGameCustom->nDifficulty)
					{
					case 1://Easy
						if (g_nDelay % 50 == 0)
						{
							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_007);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy]) * 3.0f, cosf(g_Vaim[nCntEnemy]) * 3.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy] + 0.4f) * 3.0f, cosf(g_Vaim[nCntEnemy] + 0.4f) * 3.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy] - 0.4f) * 3.0f, cosf(g_Vaim[nCntEnemy] - 0.4f) * 3.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);
						}
						break;
					case 2://Normal
						if (g_nDelay % 30 == 0)
						{
							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_007);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy]) * 3.0f, cosf(g_Vaim[nCntEnemy]) * 3.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy] + 0.4f) * 3.0f, cosf(g_Vaim[nCntEnemy] + 0.4f) * 3.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy] - 0.4f) * 3.0f, cosf(g_Vaim[nCntEnemy] - 0.4f) * 3.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);
						}
						break;
					case 3://Hard
						if (g_nDelay % 30 == 0)

						{
							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_007);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy]) * 3.0f, cosf(g_Vaim[nCntEnemy]) * 3.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);
					SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy] + 0.4f) * 3.0f, cosf(g_Vaim[nCntEnemy] + 0.4f) * 3.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);
					SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy] - 0.4f) * 3.0f, cosf(g_Vaim[nCntEnemy] - 0.4f) * 3.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);
					SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy] + 0.8f) * 3.0f, cosf(g_Vaim[nCntEnemy] + 0.8f) * 3.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);
					SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy] - 0.8f) * 3.0f, cosf(g_Vaim[nCntEnemy] - 0.8f) * 3.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);

						}
						break;
					case 4://VeryHard
						if (g_nDelay % 22 == 0)

						{
							PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_007);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy]) * 3.0f, cosf(g_Vaim[nCntEnemy]) * 3.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy] + 0.4f) * 3.0f, cosf(g_Vaim[nCntEnemy] + 0.4f) * 3.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy] - 0.4f) * 3.0f, cosf(g_Vaim[nCntEnemy] - 0.4f) * 3.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy] + 0.8f) * 3.0f, cosf(g_Vaim[nCntEnemy] + 0.8f) * 3.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy] - 0.8f) * 3.0f, cosf(g_Vaim[nCntEnemy] - 0.8f) * 3.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy] + 0.2f) * 3.3f, cosf(g_Vaim[nCntEnemy] + 0.2f) * 3.3f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy] - 0.2f) * 3.3f, cosf(g_Vaim[nCntEnemy] - 0.2f) * 3.3f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy] + 0.6f) * 3.3f, cosf(g_Vaim[nCntEnemy] + 0.6f) * 3.3f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);
							SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy] - 0.6f) * 3.3f, cosf(g_Vaim[nCntEnemy] - 0.6f) * 3.3f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);

						}
						break;
					}

					//SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((D3DX_PI * g_fXVaim[nCntEnemy]) * 0.004f, (D3DX_PI * g_fYVaim[nCntEnemy]) * 0.004f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy);//�G�̒e������G���m�œ�����Ȃ�
					
				}



				if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS1_STARWARRIOR)
				{//�{�X���g�p���ꂽ��
					g_aEnemy[nCntEnemy].nCountMoveInterval++;
					/*			g_aEnemy[nCntEnemy].nMoveTime++;*/

					if (g_aEnemy[nCntEnemy].bAttackInterval == false)
					{
						g_aEnemy[nCntEnemy].nAttarkIntervalCount++;

						if (g_aEnemy[nCntEnemy].nAttarkIntervalCount >= 80)
						{
							g_aEnemy[nCntEnemy].bAttackInterval = true;
							g_aEnemy[nCntEnemy].nAttarkIntervalCount = 0;
						}
					}


					if (g_aEnemy[nCntEnemy].bAttackInterval == true)
					{
						g_nCountTechiniqueTime++;
					}

					if (g_nCountTechiniqueTime == 1)
					{
						g_nBoss1TechiniquePattern = rand() % 8 + 1;//�{�X�̋Z�̎�ނ̔ԍ������߂�
					}



					if (g_aEnemy[nCntEnemy].bBossSummonFlag == false)
					{
						g_aEnemy[nCntEnemy].bBossSummonFlag = true;
						StopSound(SOUND_LABEL_BGM001);

						PlaySound(SOUND_LABEL_BGM003);
						SetGage(nCntEnemy, GAGETYPE_BOSSHP_FRAME);//���Ԗڂ̓G���𔻕ʂ���K�v�͂Ȃ��̂ŁA-1�ԖڂƐݒ肵�Ă����B
						SetGage(nCntEnemy, GAGETYPE_BOSS1HP);

					}


					/*	if (nCountMoveTime>= 300)
						{
							g_nCountTechiniqueTime = 0;
						}*/


					if (g_aEnemy[nCntEnemy].nCountMoveInterval >= 300)
					{
						g_aEnemy[nCntEnemy].fRotMove = float((rand)() % 629 - 314) / 100.0f;
						g_aEnemy[nCntEnemy].nCountMoveInterval = 0;//���ɓ����܂ł̎��Ԃ��������B
						g_aEnemy[nCntEnemy].bBossMoveFlag = true;//�{�X�𓮂���
					}

					if (pGameCustom->nDifficulty != 4)
					{
						if (g_aEnemy[nCntEnemy].bBossMoveFlag == true)
						{
							g_aEnemy[nCntEnemy].nMoveTime++;
							g_aEnemy[nCntEnemy].move = D3DXVECTOR3((sinf(-D3DX_PI * g_aEnemy[nCntEnemy].fRotMove) * 2.0f), (cosf(-D3DX_PI * g_aEnemy[nCntEnemy].fRotMove) * 2.0f), 0.0f);
						}
					}
					else
					{
						if (g_aEnemy[nCntEnemy].bBossMoveFlag == true)
						{
							g_aEnemy[nCntEnemy].nMoveTime++;
							g_aEnemy[nCntEnemy].move = D3DXVECTOR3((sinf(-D3DX_PI * g_aEnemy[nCntEnemy].fRotMove) * 3.0f), (cosf(-D3DX_PI * g_aEnemy[nCntEnemy].fRotMove) * 3.0f), 0.0f);
						}
					}
					if (g_aEnemy[nCntEnemy].nMoveTime >= 100)
					{
						g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�����I������̂ŁAmove�̒l���������B
						g_aEnemy[nCntEnemy].nMoveTime = 0;
						g_aEnemy[nCntEnemy].bBossMoveFlag = false;

					}
					//if (g_aEnemy[nCntEnemy].bBossMoveFlag == true)
					//{
					///*	g_aEnemy[nCntEnemy].bBossMoveFlag = false;*/
					//}

					if (g_aEnemy[nCntEnemy].bAttackInterval == true)
					{//�U����ԂɂȂ�����E�E�E
						if (g_nBoss1TechiniquePattern == 1)
						{
							g_nCountTechiniqueTime++;
							g_fXVaim[nCntEnemy] = pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x;//X���W�ւ̃x�N�g��
							g_fYVaim[nCntEnemy] = pPlayer->pos.y - g_aEnemy[nCntEnemy].pos.y;//Y���W�ւ̃x�N�g��
						

							g_Vaim[nCntEnemy] = atan2(g_fXVaim[nCntEnemy], g_fYVaim[nCntEnemy]);

							switch (pGameCustom->nDifficulty)
							{
							case 1://Easy
								if (g_nDelay % 15 == 0)
								{
									PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_007);
									SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy]) * 8.0f, cosf(g_Vaim[nCntEnemy]) * 8.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);
									SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy] + 0.4f) * 7.0f, cosf(g_Vaim[nCntEnemy] + 0.4f) * 7.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);
									SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy] - 0.4f) * 6.0f, cosf(g_Vaim[nCntEnemy] - 0.4f) * 6.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);
								}
								break;
							case 2://Normal
								if (g_nDelay % 15==0)
								{
									PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_007);
									SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy]) * 8.0f, cosf(g_Vaim[nCntEnemy]) * 8.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);//�G�̒e������G���m�œ�����Ȃ�
									SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy] + 0.2f) * 7.0f, cosf(g_Vaim[nCntEnemy] + 0.2f) * 7.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);//�G�̒e������G���m�œ�����Ȃ�
									SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy] - 0.2f) * 6.0f, cosf(g_Vaim[nCntEnemy] - 0.2f) * 6.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);//�G�̒e������G���m�œ�����Ȃ�
									SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy] + 0.4f) * 5.0f, cosf(g_Vaim[nCntEnemy] + 0.4) * 5.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);//�G�̒e������G���m�œ�����Ȃ�
									SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy] - 0.4f) * 4.0f, cosf(g_Vaim[nCntEnemy] - 0.4f) * 4.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);//�G�̒e������G���m�œ�����Ȃ�
								}
								break;
							case 3://Hard
								if (g_nDelay % 5 == 0)
								{
									PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_007);
									SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy]) * 8.0f, cosf(g_Vaim[nCntEnemy]) * 8.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);
									SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy] + 0.2f) * 7.0f, cosf(g_Vaim[nCntEnemy] + 0.2f) * 7.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);
									SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy] - 0.2f) * 6.0f, cosf(g_Vaim[nCntEnemy] - 0.2f) * 6.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);
									SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy] + 0.4f) * 5.0f, cosf(g_Vaim[nCntEnemy] + 0.4) * 5.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);
									SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy] - 0.4f) * 4.0f, cosf(g_Vaim[nCntEnemy] - 0.4f) * 4.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);
									SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy] + 0.6f) * 3.0f, cosf(g_Vaim[nCntEnemy] + 0.6f) * 3.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);
									SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(g_Vaim[nCntEnemy] - 0.6f) * 2.0f, cosf(g_Vaim[nCntEnemy] - 0.6f) * 2.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1);
								}
								break;
							case 4://Veryhard

								if (g_nDelay %50 == 0)
								{
									PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_007);
										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(D3DX_PI * 0.5) * 10.0f, cosf(D3DX_PI * 0.5) * 10.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ROCKON, 6, -1, -1);
										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(D3DX_PI * 1.5) * 10.0f, cosf(D3DX_PI * 1.5) * 10.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ROCKON, 6, -1, -1);
									
								}
								break;
							}

							
							if (g_nCountTechiniqueTime >= 150)
							{
								g_nCountTechiniqueTime = 0;
								g_aEnemy[nCntEnemy].bAttackInterval = false;
							}
						}

						if (g_aEnemy[nCntEnemy].bUse == true && g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS1_STARWARRIOR && g_nBoss1TechiniquePattern == 2)
						{//�G�l�~�[12�̍U��



						
							int nCntBullet_E4;
							float fCircleBullet_E4_easy[8];
							float fCircleBullet_E4_normal[12];
							float fCircleBullet_E4_hard[16];
							float fCircleBullet_E4_veryhard[100];
							float fRotBullet_E4;
							fRotBullet_E4 = float((rand)() % 200) / 100.0f;

							switch (pGameCustom->nDifficulty)
							{
							case 1://Easy
								if (g_nDelay % 40 == 0)
								{
									PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_004);

									for (nCntBullet_E4 = 0; nCntBullet_E4 < 8; nCntBullet_E4++)

									{
										fCircleBullet_E4_easy[nCntBullet_E4] = (1.0f / 4.0f) * nCntBullet_E4;

										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * (fCircleBullet_E4_easy[nCntBullet_E4] + fRotBullet_E4)) * 4.0f), (cosf(-D3DX_PI * (fCircleBullet_E4_easy[nCntBullet_E4] + fRotBullet_E4)) * 4.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);
										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * (fCircleBullet_E4_easy[nCntBullet_E4] + fRotBullet_E4)) * 8.0f), (cosf(-D3DX_PI * (fCircleBullet_E4_easy[nCntBullet_E4] + fRotBullet_E4)) * 8.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);

									}
								}
								break;
							case 2://Normal
								if (g_nDelay % 25 == 0)
								{
									PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_004);

									for (nCntBullet_E4 = 0; nCntBullet_E4 < 12; nCntBullet_E4++)

									{
										fCircleBullet_E4_normal[nCntBullet_E4] = (1.0f / 6.0f) * nCntBullet_E4;

										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * (fCircleBullet_E4_normal[nCntBullet_E4] + fRotBullet_E4)) * 4.0f), (cosf(-D3DX_PI * (fCircleBullet_E4_normal[nCntBullet_E4] + fRotBullet_E4)) * 4.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);
										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * (fCircleBullet_E4_normal[nCntBullet_E4] + fRotBullet_E4)) * 8.0f), (cosf(-D3DX_PI * (fCircleBullet_E4_normal[nCntBullet_E4] + fRotBullet_E4)) * 8.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);

									}
								}
								break;
							case 3://Hard
								if (g_nDelay % 15 == 0)
								{
									PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_004);

									for (nCntBullet_E4 = 0; nCntBullet_E4 < 16; nCntBullet_E4++)

									{
										fCircleBullet_E4_hard[nCntBullet_E4] = (1.0f / 8.0f) * nCntBullet_E4;

										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * (fCircleBullet_E4_hard[nCntBullet_E4] + fRotBullet_E4)) * 4.0f), (cosf(-D3DX_PI * (fCircleBullet_E4_hard[nCntBullet_E4] + fRotBullet_E4)) * 4.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);
										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * (fCircleBullet_E4_hard[nCntBullet_E4] + fRotBullet_E4)) * 8.0f), (cosf(-D3DX_PI * (fCircleBullet_E4_hard[nCntBullet_E4] + fRotBullet_E4)) * 8.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);

									}
								}
								break;
							case 4://VeryHard
								if (g_nDelay % 70 == 0)
								{
									PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_004);

									for (nCntBullet_E4 = 0; nCntBullet_E4 < 100; nCntBullet_E4++)

									{
										fCircleBullet_E4_veryhard[nCntBullet_E4] = (1.0f / 50.0f) * nCntBullet_E4;
										//SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * (fCircleBullet_E4_veryhard[nCntBullet_E4] + fRotBullet_E4)) * 2.0f), (cosf(-D3DX_PI * (fCircleBullet_E4_veryhard[nCntBullet_E4] + fRotBullet_E4)) * 2.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);
										
										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * (fCircleBullet_E4_veryhard[nCntBullet_E4] + fRotBullet_E4)) *4.0f), (cosf(-D3DX_PI * (fCircleBullet_E4_veryhard[nCntBullet_E4] + fRotBullet_E4)) * 4.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);
										//SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * (fCircleBullet_E4_veryhard[nCntBullet_E4] + fRotBullet_E4)) * 7.0f), (cosf(-D3DX_PI * (fCircleBullet_E4_veryhard[nCntBullet_E4] + fRotBullet_E4)) * 7.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);
										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * (fCircleBullet_E4_veryhard[nCntBullet_E4] + fRotBullet_E4)) * 8.0f), (cosf(-D3DX_PI * (fCircleBullet_E4_veryhard[nCntBullet_E4] + fRotBullet_E4)) * 8.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);
										
									
									}
								}
								break;

							}


							if (g_nCountTechiniqueTime >= 150)
							{
								g_nCountTechiniqueTime = 0;
								g_aEnemy[nCntEnemy].bAttackInterval = false;
							}

							//SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((D3DX_PI * g_fXVaim[nCntEnemy]) * 0.004f, (D3DX_PI * g_fYVaim[nCntEnemy]) * 0.004f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy);//�G�̒e������G���m�œ�����Ȃ�
						}
						if (g_aEnemy[nCntEnemy].bUse == true &&g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS1_STARWARRIOR && g_nBoss1TechiniquePattern == 3)
						{//�G�l�~�[12�̍U��(SetBullet)
						//	SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * 0.0f) * 5.0f), (cosf(-D3DX_PI * 0.0f) * 5.0f), 0.0f), 10, BULLETTYPE_REAFY);//�G�̒e������G���m�œ�����Ȃ�
							int nCntBullet_E5;
							float fCircleBullet_E5_easy[4];
							float fCircleBullet_E5_normal[12];
							float fCircleBullet_E5_hard[16];
							float fCircleBullet_E5_veryhard[24];
							switch (pGameCustom->nDifficulty)
							{
								
							case 1:
								if (g_nDelay % 30 == 0)
								{
									PlaySound(SOUND_LABEL_SE_ARROWSHOT_000);
									for (nCntBullet_E5 = 0; nCntBullet_E5 < 4; nCntBullet_E5++)
									{
										
										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_REAFY, nCntBullet_E5, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�
										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_REVERSE_CURVE, nCntBullet_E5, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�

									}
								}
								break;
							case 2:
								if (g_nDelay % 20 == 0)
								{
									PlaySound(SOUND_LABEL_SE_ARROWSHOT_000);
									for (nCntBullet_E5 = 0; nCntBullet_E5 < 8; nCntBullet_E5++)
									{
										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_REAFY, nCntBullet_E5, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�
										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_REVERSE_CURVE, nCntBullet_E5, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�

									}
								}
								break;
							case 3:
								if (g_nDelay % 20 == 0)
								{
									PlaySound(SOUND_LABEL_SE_ARROWSHOT_000);
									for (nCntBullet_E5 = 0; nCntBullet_E5 < 14; nCntBullet_E5++)
									{
										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_REAFY, nCntBullet_E5, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�
										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_REVERSE_CURVE, nCntBullet_E5, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�

									}
								}
								break;
							case 4:
								if (g_nDelay % 15 == 0)
								{
									PlaySound(SOUND_LABEL_SE_ARROWSHOT_000);
									for (nCntBullet_E5 = 0; nCntBullet_E5 < 20; nCntBullet_E5++)
									{
										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_REAFY, nCntBullet_E5, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�
										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_REVERSE_CURVE, nCntBullet_E5, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�

									}
								}
								break;

							}
							if (g_nCountTechiniqueTime >= 300)
							{
								g_nCountTechiniqueTime = 0;
								g_aEnemy[nCntEnemy].bAttackInterval = false;
							}
						}
						if (g_aEnemy[nCntEnemy].bUse == true &&g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS1_STARWARRIOR && g_nBoss1TechiniquePattern == 4)
						{//�G�l�~�[12�̍U��


							float fCircleBullet_E12[4];
							int nCntBullet_E12;
							float fCircleBullet_E12_easy[2];
							float fCircleBullet_E12_normal[4];
							float fCircleBullet_E12_hard[4];
							float fCircleBullet_E12_veryhard[4];



							switch (pGameCustom->nDifficulty)
							{
							case 1://Easy
								if (g_nDelay % 75 == 0)
								{
							      PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_001);
									for (nCntBullet2 = 0; nCntBullet2 < 2; nCntBullet2++)
									{
										fCircleBullet_E12_easy[nCntBullet2] = 1.0f* nCntBullet2;


										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * fCircleBullet_E12_easy[nCntBullet2] + g_fRandumResultBullet2) * fRandumSpeed_E12), (cosf(-D3DX_PI * fCircleBullet_E12_easy[nCntBullet2] + g_fRandumResultBullet2) * fRandumSpeed_E12), 0.0f), GENERALLY_BULLETLIFE / 5, BULLETTYPE_SCATTERING, 0, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�		}

									}
								}
								break;
							case 2://Normal
								if (g_nDelay % 50 == 0)
								{
									PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_001);
									for (nCntBullet2 = 0; nCntBullet2 < 4 ;nCntBullet2++)
									{
										fCircleBullet_E12_normal[nCntBullet2] = (1.0f/2.0f) * nCntBullet2;


										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * fCircleBullet_E12_normal[nCntBullet2] + g_fRandumResultBullet2) * fRandumSpeed_E12), (cosf(-D3DX_PI * fCircleBullet_E12_normal[nCntBullet2] + g_fRandumResultBullet2) * fRandumSpeed_E12), 0.0f), GENERALLY_BULLETLIFE / 5, BULLETTYPE_SCATTERING, 0, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�		}

									}
								}
								break;
							case 3://Hard
								if (g_nDelay % 30 == 0)
								{
									PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_001);
									for (nCntBullet2 = 0; nCntBullet2 < 4; nCntBullet2++)
									{
										fCircleBullet_E12_normal[nCntBullet2] = (1.0f / 2.0f) * nCntBullet2;


										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * fCircleBullet_E12_normal[nCntBullet2] + g_fRandumResultBullet2) * fRandumSpeed_E12), (cosf(-D3DX_PI * fCircleBullet_E12_normal[nCntBullet2] + g_fRandumResultBullet2) * fRandumSpeed_E12), 0.0f), GENERALLY_BULLETLIFE / 6, BULLETTYPE_SCATTERING, 0, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�		}

									}
								}
								break;
							case 4://VeryHard
								if (g_nDelay % 40 == 0)
								{
									PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_001);
									for (nCntBullet2 = 0; nCntBullet2 < 4; nCntBullet2++)
									{
										fCircleBullet_E12_veryhard[nCntBullet2] = (1.0f / 2.0f) * nCntBullet2;


										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * fCircleBullet_E12_veryhard[nCntBullet2] + g_fRandumResultBullet2) * fRandumSpeed_E12), (cosf(-D3DX_PI * fCircleBullet_E12_veryhard[nCntBullet2] + g_fRandumResultBullet2) * fRandumSpeed_E12), 0.0f), GENERALLY_BULLETLIFE / 5, BULLETTYPE_SCATTERING, 0, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�		}

									}
								}
								break;
							}


							//for (nCntBullet2 = 0; nCntBullet2 < 4; nCntBullet2++)
							//{
							//	fCircleBullet_E12[nCntBullet2] = (1.0f / 2.0f) * nCntBullet2;


							//	SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * fCircleBullet_E12[nCntBullet2] + g_fRandumResultBullet2) * fRandumSpeed_E12), (cosf(-D3DX_PI * fCircleBullet_E12[nCntBullet2] + g_fRandumResultBullet2) * fRandumSpeed_E12), 0.0f), GENERALLY_BULLETLIFE/4, BULLETTYPE_SCATTERING, 0, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�		}

							//}

							if (g_nCountTechiniqueTime >= 150)
							{
								g_nCountTechiniqueTime = 0;
								g_aEnemy[nCntEnemy].bAttackInterval = false;
							}

							//SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((D3DX_PI * g_fXVaim[nCntEnemy]) * 0.004f, (D3DX_PI * g_fYVaim[nCntEnemy]) * 0.004f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy);//�G�̒e������G���m�œ�����Ȃ�
						}
						if (g_aEnemy[nCntEnemy].bUse == true &&g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS1_STARWARRIOR && g_nBoss1TechiniquePattern == 5)
						{//�G�l�~�[12�̍U�� �i�_�u���E�C���h�j
							float fCircleBullet_E12[4];
							float fRotMove;
							float fRandumSpeed_E12_W5;


							switch (pGameCustom->nDifficulty)
							{
							case 1://Easy
								if (g_nDelay % 100 == 0)
								{
									PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_008);
									for (nCntBullet2 = 0; nCntBullet2 < 5; nCntBullet2++)
									{

										fRotMove = float((rand)() % 100 + 1) / 100.0f;//�����̕����ɒe�𔭎˂���B(0.0f�`1.0f)
										fRandumSpeed_E12_W5 = float((rand)() % 150 + 1) / 10.0f;//�����̕����ɒe�𔭎˂���B(0.0f�`1.0f)
										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * fRotMove) * fRandumSpeed_E12_W5), (cosf(-D3DX_PI * fRotMove) * fRandumSpeed_E12_W5), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_RIGHT_WIND, 0, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�		}

									}

									for (nCntBullet2 = 0; nCntBullet2 < 5; nCntBullet2++)
									{

										fRotMove = float((rand)() % 100 + 1) / 100.0f;//�����̕����ɒe�𔭎˂���B(0.0f�`1.0f)
										fRandumSpeed_E12_W5 = float((rand)() % 150 + 1) / 10.0f;//�����̕����ɒe�𔭎˂���B(0.0f�`1.0f)
										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(D3DX_PI * fRotMove) * fRandumSpeed_E12_W5), (cosf(D3DX_PI * fRotMove) * fRandumSpeed_E12_W5), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_LEFT_WIND, 0, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�		}

									}
								}
							break;
							case 2://Normal
								if (g_nDelay % 80 == 0)
								{
									PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_008);
									for (nCntBullet2 = 0; nCntBullet2 < 15; nCntBullet2++)
									{

										fRotMove = float((rand)() % 100 + 1) / 100.0f;//�����̕����ɒe�𔭎˂���B(0.0f�`1.0f)
										fRandumSpeed_E12_W5 = float((rand)() % 150 + 1) / 10.0f;//�����̕����ɒe�𔭎˂���B(0.0f�`1.0f)
										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * fRotMove) * fRandumSpeed_E12_W5), (cosf(-D3DX_PI * fRotMove) * fRandumSpeed_E12_W5), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_RIGHT_WIND, 0, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�		}

									}

									for (nCntBullet2 = 0; nCntBullet2 < 15; nCntBullet2++)
									{

										fRotMove = float((rand)() % 100 + 1) / 100.0f;//�����̕����ɒe�𔭎˂���B(0.0f�`1.0f)
										fRandumSpeed_E12_W5 = float((rand)() % 150 + 1) / 10.0f;//�����̕����ɒe�𔭎˂���B(0.0f�`1.0f)
										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(D3DX_PI * fRotMove) * fRandumSpeed_E12_W5), (cosf(D3DX_PI * fRotMove) * fRandumSpeed_E12_W5), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_LEFT_WIND, 0, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�		}

									}
								}
								break;
							case 3://Hard
								if (g_nDelay % 60 == 0)
								{
									PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_008);
									for (nCntBullet2 = 0; nCntBullet2 < 30; nCntBullet2++)
									{

										fRotMove = float((rand)() % 100 + 1) / 100.0f;//�����̕����ɒe�𔭎˂���B(0.0f�`1.0f)
										fRandumSpeed_E12_W5 = float((rand)() % 150 + 1) / 10.0f;//�����̕����ɒe�𔭎˂���B(0.0f�`1.0f)
										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * fRotMove) * fRandumSpeed_E12_W5), (cosf(-D3DX_PI * fRotMove) * fRandumSpeed_E12_W5), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_RIGHT_WIND, 0, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�		}

									}

									for (nCntBullet2 = 0; nCntBullet2 < 30; nCntBullet2++)
									{

										fRotMove = float((rand)() % 100 + 1) / 100.0f;//�����̕����ɒe�𔭎˂���B(0.0f�`1.0f)
										fRandumSpeed_E12_W5 = float((rand)() % 150 + 1) / 10.0f;//�����̕����ɒe�𔭎˂���B(0.0f�`1.0f)
										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(D3DX_PI * fRotMove) * fRandumSpeed_E12_W5), (cosf(D3DX_PI * fRotMove) * fRandumSpeed_E12_W5), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_LEFT_WIND, 0, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�		}

									}
								}
								break;
							case 4://VeryHard
								if (g_nDelay % 75 == 0)
								{
									PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_008);
									for (nCntBullet2 = 0; nCntBullet2 < 40; nCntBullet2++)
									{

										fRotMove = float((rand)() % 100 + 1) / 100.0f;//�����̕����ɒe�𔭎˂���B(0.0f�`1.0f)
										fRandumSpeed_E12_W5 = float((rand)() % 150 + 1) / 10.0f;//�����̕����ɒe�𔭎˂���B(0.0f�`1.0f)
										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * fRotMove) * fRandumSpeed_E12_W5), (cosf(-D3DX_PI * fRotMove) * fRandumSpeed_E12_W5), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_RIGHT_WIND, 0, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�		}

									}

									for (nCntBullet2 = 0; nCntBullet2 < 40; nCntBullet2++)
									{

										fRotMove = float((rand)() % 100 + 1) / 100.0f;//�����̕����ɒe�𔭎˂���B(0.0f�`1.0f)
										fRandumSpeed_E12_W5 = float((rand)() % 150 + 1) / 10.0f;//�����̕����ɒe�𔭎˂���B(0.0f�`1.0f)
										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(D3DX_PI * fRotMove) * fRandumSpeed_E12_W5), (cosf(D3DX_PI * fRotMove) * fRandumSpeed_E12_W5), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_LEFT_WIND, 0, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�		}

									}
									for (nCntBullet2 = 0; nCntBullet2 < 40; nCntBullet2++)
									{

										fRotMove = float((rand)() % 150 + 51) / 100.0f;//�����̕����ɒe�𔭎˂���B(0.0f�`1.0f)
										fRandumSpeed_E12_W5 = float((rand)() % 150 + 1) / 10.0f;//�����̕����ɒe�𔭎˂���B(0.0f�`1.0f)
										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(D3DX_PI * fRotMove) * fRandumSpeed_E12_W5), (cosf(D3DX_PI * fRotMove) * fRandumSpeed_E12_W5), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_DOWNWIND, 0, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�		}

									}
									for (nCntBullet2 = 0; nCntBullet2 < 40; nCntBullet2++)
									{

										fRotMove = float((rand)() % 150 + 51) / 100.0f;//�����̕����ɒe�𔭎˂���B(0.0f�`1.0f)
										fRandumSpeed_E12_W5 = float((rand)() % 150 + 1) / 10.0f;//�����̕����ɒe�𔭎˂���B(0.0f�`1.0f)
										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * fRotMove) * fRandumSpeed_E12_W5), (cosf(-D3DX_PI * fRotMove) * fRandumSpeed_E12_W5), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_UPWIND, 0, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�		}

									}
								}
								break;
							}

							//for (nCntBullet2 = 0; nCntBullet2 < 30; nCntBullet2++)
							//{

							//	fRotMove = float((rand)() % 100 + 1) / 100.0f;//�����̕����ɒe�𔭎˂���B(0.0f�`1.0f)
							//	fRandumSpeed_E12_W5 = float((rand)() % 150 + 1) / 10.0f;//�����̕����ɒe�𔭎˂���B(0.0f�`1.0f)
							//	SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * fRotMove) * fRandumSpeed_E12_W5), (cosf(-D3DX_PI * fRotMove) * fRandumSpeed_E12_W5), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_RIGHT_WIND, 0, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�		}

							//}

							//for (nCntBullet2 = 0; nCntBullet2 < 30; nCntBullet2++)
							//{

							//	fRotMove = float((rand)() % 100 + 1) / 100.0f;//�����̕����ɒe�𔭎˂���B(0.0f�`1.0f)
							//	fRandumSpeed_E12_W5 = float((rand)() % 150 + 1) / 10.0f;//�����̕����ɒe�𔭎˂���B(0.0f�`1.0f)
							//	SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(D3DX_PI * fRotMove) * fRandumSpeed_E12_W5), (cosf(D3DX_PI * fRotMove) * fRandumSpeed_E12_W5), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_LEFT_WIND, 0, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�		}

							//}
							if (g_nCountTechiniqueTime >= 150)
							{
								g_nCountTechiniqueTime = 0;
								g_aEnemy[nCntEnemy].bAttackInterval = false;
							}

							//SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((D3DX_PI * g_fXVaim[nCntEnemy]) * 0.004f, (D3DX_PI * g_fYVaim[nCntEnemy]) * 0.004f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy);//�G�̒e������G���m�œ�����Ȃ�
						}
						if (g_aEnemy[nCntEnemy].bUse == true && g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS1_STARWARRIOR && g_nBoss1TechiniquePattern == 6)
						{//�G�l�~�[12�̍U�� �i���˒e�j

							float fCircleBullet_easy[4];
							float fCircleBullet_normal[6];
							float fCircleBullet_hard[6];
							float fCircleBullet_veryhard[6];


							switch (pGameCustom->nDifficulty)
							{
							case 1:
								if (g_nDelay % 35 == 0)
								{
									PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_001);

									for (nCntBullet2 = 0; nCntBullet2 < 4; nCntBullet2++)
									{
										fCircleBullet_easy[nCntBullet2] = (1.0f / 2.0f) * nCntBullet2;

										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * (fCircleBullet_easy[nCntBullet2] + g_fRotBullet_W6)) * 3.0f), (cosf(-D3DX_PI * (fCircleBullet_easy[nCntBullet2] + g_fRotBullet_W6)) * 3.0f), 0.0f), GENERALLY_BULLETLIFE / 5, BULLETTYPE_REFLECTION, 0, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�		}

									}
								}
								break;
							case 2:
								if (g_nDelay % 30 == 0)
								{
									PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_001);

									for (nCntBullet2 = 0; nCntBullet2 < 6; nCntBullet2++)
									{
										fCircleBullet_normal[nCntBullet2] = (1.0f / 3.0f) * nCntBullet2;

										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * (fCircleBullet_normal[nCntBullet2] + g_fRotBullet_W6)) * 3.0f), (cosf(-D3DX_PI * (fCircleBullet_normal[nCntBullet2] + g_fRotBullet_W6)) * 3.0f), 0.0f), GENERALLY_BULLETLIFE / 5, BULLETTYPE_REFLECTION, 0, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�		}

									}
								}
								break;
							case 3:
								if (g_nDelay % 17 == 0)
								{
									PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_001);

									for (nCntBullet2 = 0; nCntBullet2 < 6; nCntBullet2++)
									{
										fCircleBullet_hard[nCntBullet2] = (1.0f / 3.0f) * nCntBullet2;

										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * (fCircleBullet_hard[nCntBullet2] + g_fRotBullet_W6)) * 3.0f), (cosf(-D3DX_PI * (fCircleBullet_hard[nCntBullet2] + g_fRotBullet_W6)) * 3.0f), 0.0f), GENERALLY_BULLETLIFE / 5, BULLETTYPE_REFLECTION, 0, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�		}

									}
								}
								break;
							case 4:
								if (g_nDelay % 10 == 0)
								{
									PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_001);

									for (nCntBullet2 = 0; nCntBullet2 < 6; nCntBullet2++)
									{
										fCircleBullet_veryhard[nCntBullet2] = (1.0f / 3.0f) * nCntBullet2;

										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * (fCircleBullet_veryhard[nCntBullet2] + g_fRotBullet_W6)) * 3.0f), (cosf(-D3DX_PI * (fCircleBullet_veryhard[nCntBullet2] + g_fRotBullet_W6)) * 3.0f), 0.0f), GENERALLY_BULLETLIFE / 5, BULLETTYPE_REFLECTION, 0, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�		}

									}
								}
								break;
							}
							/*PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_001);*/

							//for (nCntBullet2 = 0; nCntBullet2 < 6; nCntBullet2++)
							//{
							//	fCircleBullet[nCntBullet2] = (1.0f / 3.0f) * nCntBullet2;

							//	SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * (fCircleBullet[nCntBullet2] + g_fRotBullet_W6)) * 3.0f), (cosf(-D3DX_PI * (fCircleBullet[nCntBullet2] + g_fRotBullet_W6)) * 3.0f), 0.0f), GENERALLY_BULLETLIFE / 5, BULLETTYPE_REFLECTION, 0, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�		}

							//}
							if (g_nCountTechiniqueTime >= 150)
							{
								g_nCountTechiniqueTime = 0;
								g_aEnemy[nCntEnemy].bAttackInterval = false;
							}

							//SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((D3DX_PI * g_fXVaim[nCntEnemy]) * 0.004f, (D3DX_PI * g_fYVaim[nCntEnemy]) * 0.004f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy);//�G�̒e������G���m�œ�����Ȃ�
						}
						if (g_aEnemy[nCntEnemy].bUse == true &&g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS1_STARWARRIOR && g_nBoss1TechiniquePattern == 7)
						{//�G�l�~�[12�̍U�� (�~�X�g�o�[�X�g�j

							float fCircleBullet[6];
							float fRandumPosX;
							float fRandumPosY;
							
							fRandumPosX= float((rand)() % SCREEN_WIDTH+1);
							fRandumPosY= float((rand)() % SCREEN_HEIGHT+1);
							switch (pGameCustom->nDifficulty)
							{
							case 1://Easy
								if (g_nDelay % 60 == 0)
								{
									PlaySound(SOUND_LABEL_SE_WARNING_001);
									SetStaging(STAGINGTYPE_BULLETWARNING, -1, D3DXVECTOR3(fRandumPosX, fRandumPosY, 0.0f));
								}
								break;
							case 2://Normal
								if (g_nDelay % 35 == 0)
								{
									PlaySound(SOUND_LABEL_SE_WARNING_001);
									SetStaging(STAGINGTYPE_BULLETWARNING, -1, D3DXVECTOR3(fRandumPosX, fRandumPosY, 0.0f));
								}
								break;
							case 3://Hard
								if (g_nDelay % 17== 0)
								{
									PlaySound(SOUND_LABEL_SE_WARNING_001);
									SetStaging(STAGINGTYPE_BULLETWARNING, -1, D3DXVECTOR3(fRandumPosX, fRandumPosY, 0.0f));
								}
								break;
							case 4://Hard
								if (g_nDelay % 15 == 0)
								{
									PlaySound(SOUND_LABEL_SE_WARNING_001);
									SetStaging(STAGINGTYPE_BULLETWARNING, -1, D3DXVECTOR3(fRandumPosX, fRandumPosY, 0.0f));
								}
								if (g_nDelay % 30 == 0)
								{
									PlaySound(SOUND_LABEL_SE_WARNING_001);
									SetStaging(STAGINGTYPE_BULLETWARNING, -1, D3DXVECTOR3(pPlayer->pos.x,pPlayer->pos.y, 0.0f));
								}
								break;
							}
							
						
							if (g_nCountTechiniqueTime >= 400)
							{
								g_nCountTechiniqueTime = 0;
								g_aEnemy[nCntEnemy].bAttackInterval = false;
							}

							//SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((D3DX_PI * g_fXVaim[nCntEnemy]) * 0.004f, (D3DX_PI * g_fYVaim[nCntEnemy]) * 0.004f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy);//�G�̒e������G���m�œ�����Ȃ�
						}
						if (g_aEnemy[nCntEnemy].bUse == true && g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS1_STARWARRIOR && g_nBoss1TechiniquePattern == 8)
						{//�G�l�~�[12�̍U��(�q���e�j
						//	SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * 0.0f) * 5.0f), (cosf(-D3DX_PI * 0.0f) * 5.0f), 0.0f), 10, BULLETTYPE_REAFY);//�G�̒e������G���m�œ�����Ȃ�

							

							int nCntBullet_E5;
							float fCircleBullet_E5_easy[4];
							float fCircleBullet_E5_normal[12];
							float fCircleBullet_E5_hard[16];
							float fCircleBullet_E5_veryhard[24];
							switch (pGameCustom->nDifficulty)
							{

							case 1:
								if (g_nDelay % 60 == 0)
								{
									PlaySound(SOUND_LABEL_SE_ARROWSHOT_000);
									for (nCntBullet_E5 = 0; nCntBullet_E5 < 4; nCntBullet_E5++)
									{

										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), GENERALLY_BULLETLIFE-1200, BULLETTYPE_SATELLITE, nCntBullet_E5, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�
										
									}
								}
								break;
							case 2:
								if (g_nDelay %50 == 0)
								{
									PlaySound(SOUND_LABEL_SE_ARROWSHOT_000);
									for (nCntBullet_E5 = 0; nCntBullet_E5 < 8; nCntBullet_E5++)
									{
										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), GENERALLY_BULLETLIFE-1200, BULLETTYPE_SATELLITE, nCntBullet_E5, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�
										
									}
								}
								break;
							case 3:
								if (g_nDelay % 25 == 0)
								{
									PlaySound(SOUND_LABEL_SE_ARROWSHOT_000);
									for (nCntBullet_E5 = 0; nCntBullet_E5 < 14; nCntBullet_E5++)
									{
										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), GENERALLY_BULLETLIFE-1200, BULLETTYPE_SATELLITE, nCntBullet_E5, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�
										
									}
								}
								break;
							case 4:
								if (g_nDelay % 15 == 0)
								{
									PlaySound(SOUND_LABEL_SE_ARROWSHOT_000);
									for (nCntBullet_E5 = 0; nCntBullet_E5 < 20; nCntBullet_E5++)
									{
										SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), GENERALLY_BULLETLIFE-1200, BULLETTYPE_SATELLITE, nCntBullet_E5, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�
										
									}
								}
								break;

							}
							if (g_nCountTechiniqueTime >= 150)
							{
								g_nCountTechiniqueTime = 0;
								g_aEnemy[nCntEnemy].bAttackInterval = false;
							}
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

				


				//==============================
				//�G�l�~�[�̓���
				//==============================
				
			    if (g_aEnemy[nCntEnemy].pos.x > SCREEN_WIDTH || g_aEnemy[nCntEnemy].pos.x < 0.0f)
				{
					if (g_aEnemy[nCntEnemy].bEnemyMoveX == false)
					{
						g_aEnemy[nCntEnemy].bEnemyMoveX = true;
					}
					else if (g_aEnemy[nCntEnemy].bEnemyMoveX == true)
					{
						g_aEnemy[nCntEnemy].bEnemyMoveX = false;
					}
				}

				if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS1_STARWARRIOR)
				{

					if (g_aEnemy[nCntEnemy].pos.y > SCREEN_HEIGHT / 3 || g_aEnemy[nCntEnemy].pos.y < 20.0f)
					{

						if (g_aEnemy[nCntEnemy].bEnemyMoveY == false)
						{
							g_aEnemy[nCntEnemy].bEnemyMoveY = true;
						}
						else if (g_aEnemy[nCntEnemy].bEnemyMoveY == true)
						{
							g_aEnemy[nCntEnemy].bEnemyMoveY = false;
						}
					}
				}
				else if (g_aEnemy[nCntEnemy].pos.y > SCREEN_HEIGHT || g_aEnemy[nCntEnemy].pos.y < 0.0f)
				{
					if (g_aEnemy[nCntEnemy].bEnemyMoveY == false)
					{
						g_aEnemy[nCntEnemy].bEnemyMoveY = true;
					}
					else if (g_aEnemy[nCntEnemy].bEnemyMoveY == true)
					{
						g_aEnemy[nCntEnemy].bEnemyMoveY = false;
					}
				}


				if (g_aEnemy[nCntEnemy].bEnemyMoveX == true)
				{
					g_aEnemy[nCntEnemy].pos.x -= g_aEnemy[nCntEnemy].move.x;
				}
				if (g_aEnemy[nCntEnemy].bEnemyMoveX == false)
				{
					g_aEnemy[nCntEnemy].pos.x += g_aEnemy[nCntEnemy].move.x;
				}
				if (g_aEnemy[nCntEnemy].bEnemyMoveY == true)
				{
					g_aEnemy[nCntEnemy].pos.y -= g_aEnemy[nCntEnemy].move.y;
				}
				if (g_aEnemy[nCntEnemy].bEnemyMoveY == false)
				{
					g_aEnemy[nCntEnemy].pos.y += g_aEnemy[nCntEnemy].move.y;
				}
			

				if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOMUBY|| g_aEnemy[nCntEnemy].nType == ENEMYTYPE_MINIBOMUBY)
				{


					float fangle[MAX_ENEMY];

					fangle[nCntEnemy] = atan2(pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x, pPlayer->pos.y - g_aEnemy[nCntEnemy].pos.y);

					g_aEnemy[nCntEnemy].move = D3DXVECTOR3((pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x) * 0.01f, (pPlayer->pos.y - g_aEnemy[nCntEnemy].pos.y) * 0.01f, 0.0f);//���S���W�𓮂���
				}
			}
		}
	
		/*if (g_aEnemy[nCntEnemy].fLife <= 1.0f)
		{
			g_aEnemy[nCntEnemy].bUse = false;
		}*/
		
	

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
			if (g_aEnemy[nCntEnemy].nApperCount >= 100)
			{

				if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_DELBY)
				{
					pDevice->SetTexture(0, g_apTextureEnemy[ENEMYTYPE_DELBY]);
				}
				if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_PUNDY)
				{
					pDevice->SetTexture(0, g_apTextureEnemy[ENEMYTYPE_PUNDY]);
				}
				if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_SMILY)
				{
					pDevice->SetTexture(0, g_apTextureEnemy[ENEMYTYPE_SMILY]);
				}
				if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_SQUDY)
				{
					pDevice->SetTexture(0, g_apTextureEnemy[ENEMYTYPE_SQUDY]);
				}
				if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_REAFY)
				{
					pDevice->SetTexture(0, g_apTextureEnemy[ENEMYTYPE_REAFY]);
				}
				if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_HONOBY)
				{
					pDevice->SetTexture(0, g_apTextureEnemy[ENEMYTYPE_HONOBY]);
				}
				if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_MIZUBY)
				{
					pDevice->SetTexture(0, g_apTextureEnemy[ENEMYTYPE_MIZUBY]);
				}
				if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOMUBY|| g_aEnemy[nCntEnemy].nType == ENEMYTYPE_MINIBOMUBY)
				{
					pDevice->SetTexture(0, g_apTextureEnemy[ENEMYTYPE_BOMUBY]);
				}
				if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_WINBY)
				{
					pDevice->SetTexture(0, g_apTextureEnemy[ENEMYTYPE_WINBY]);
				}
				if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOLBY)
				{
					pDevice->SetTexture(0, g_apTextureEnemy[ENEMYTYPE_BOLBY]);
				}
				if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BANDY)
				{
					pDevice->SetTexture(0, g_apTextureEnemy[ENEMYTYPE_BANDY]);
				}
				if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS1_STARWARRIOR)
				{
					pDevice->SetTexture(0, g_apTextureEnemy[ENEMYTYPE_BOSS1_STARWARRIOR]);
				}
				/*else
				{
					pDevice->SetTexture(0, g_apTextureEnemy[nCntEnemy]);
				}*/

				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,//0�́A�`�悷��ŏ��̒��_�C���f�b�N�X�ŁA2�́A�`�悷��v���~�e�B�u��
					nCntEnemy * 4,
					2);
			}
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
			//pVtx += (nCntEnemy * 4);
			pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 30.0f, g_aEnemy[nCntEnemy].pos.y - 30.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 30.0f, g_aEnemy[nCntEnemy].pos.y - 30.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 30.0f, g_aEnemy[nCntEnemy].pos.y + 30.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 30.0f, g_aEnemy[nCntEnemy].pos.y + 30.0f, 0.0f);
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

			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//�ԐF�ɂ���
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//�ԐF�ɂ���
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//�ԐF�ɂ���
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//�ԐF�ɂ���

			SetStaging(STAGINGTYPE_APPER, nCntEnemy,pos);//�G���o������\��̈ʒu�ɓG���o������\��������B�B


			g_aDefeat.DefeatEnemyCount++;//��ɂ���c��G�l�~�[�����Z�b�g���ꂽ���v���X����B

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
void HitEnemy(int nCntEnemy,float fDamage)
{
	int nCnt;
	GameCustom* pGameCustom = GetGameCustom();
	ResultType* pResultType = GetResultType();//���U���g�̎�ނ̍\���̂̃A�h���X�Ə����擾
	Result* pResult = GetResult();
	VERTEX_2D* pVtx;
	Player* pPlayer = GetPlayer();
	g_aEnemy[nCntEnemy].fLife -= fDamage;
	if (g_aEnemy[nCntEnemy].fLife <= 0.0f)
	{//�G�̗̑͂��O�ɂȂ�����E�E�E

         		PlaySound(SOUND_LABEL_SE_DEFEAT_ENEMY);

		SetExplosion(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), EXPLOSIONTYPE_NORMAL);//�G������ʒu�Ŕ���������B

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

		if (fDamage <= 1 && fDamage >= 0)
		{
			SetDamage(fDamage, 100, D3DXVECTOR3((sinf(-D3DX_PI * fRandumRot) * fRandumSpeed), (cosf(-D3DX_PI * fRandumRot) * fRandumSpeed), 0.0f), g_aEnemy[nCntEnemy].pos, 10.0f);

		}
		else if (fDamage <= 5 && fDamage >= 2)
		{
			SetDamage(fDamage, 100, D3DXVECTOR3((sinf(-D3DX_PI * fRandumRot) * fRandumSpeed), (cosf(-D3DX_PI * fRandumRot) * fRandumSpeed), 0.0f), g_aEnemy[nCntEnemy].pos, 15.0f);
		}
		else if (fDamage >= 15 && fDamage <= 50)
		{
			SetDamage(fDamage, 100, D3DXVECTOR3((sinf(-D3DX_PI * fRandumRot) * fRandumSpeed), (cosf(-D3DX_PI * fRandumRot) * fRandumSpeed), 0.0f), g_aEnemy[nCntEnemy].pos, 40.0f);
		}
		else if (fDamage >= 70)
		{
			SetDamage(fDamage, 100, D3DXVECTOR3((sinf(-D3DX_PI * fRandumRot) * fRandumSpeed), (cosf(-D3DX_PI * fRandumRot) * fRandumSpeed), 0.0f), g_aEnemy[nCntEnemy].pos, 80.0f);
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

		if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_MINIBOMUBY)
		{
			int nCntBullet_E8;
			float fSpeedBullet_E8;
			float fCircleBullet_E8_easy[10];
			float fRotBullet_E8;
			fRotBullet_E8 = float((rand)() % 200) / 100.0f;
			PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_001);

			int nCntBullet4;
			for (nCntBullet4 = 0; nCntBullet4 < 10; nCntBullet4++)
			{
				fCircleBullet_E8_easy[nCntBullet4] = (1.0f / (10 / 2)) * nCntBullet4;

				fSpeedBullet_E8 = rand() % 5 + 1;


				SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * fCircleBullet_E8_easy[nCntBullet4]) * fSpeedBullet_E8), (cosf(-D3DX_PI * fCircleBullet_E8_easy[nCntBullet4]) * fSpeedBullet_E8), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�	

			}
		}


		if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOMUBY)
		{
			int nCntBullet_E8;
			float fSpeedBullet_E8;
			float fCircleBullet_E8_easy[10];
			float fCircleBullet_E8_normal[30];
			float fCircleBullet_E8_hard[50];
			float fCircleBullet_E8_veryhard[100];
			float fRotBullet_E8;
			fRotBullet_E8 = float((rand)() % 200) / 100.0f;
			

			int nCntBullet4;
			switch (pGameCustom->nDifficulty)
			{
			case 1://Easy
				PlaySound(SOUND_LABEL_SE_EXPLOSION_ENEMY_000);
				for (nCntBullet4 = 0; nCntBullet4 < 10; nCntBullet4++)
				{
					fCircleBullet_E8_easy[nCntBullet4]= (1.0f / (10 / 2)) * nCntBullet4;

					fSpeedBullet_E8 = rand() % 5 + 1;

					
					SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * fCircleBullet_E8_easy[nCntBullet4]) * fSpeedBullet_E8), (cosf(-D3DX_PI * fCircleBullet_E8_easy[nCntBullet4]) * fSpeedBullet_E8), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�	

				}
				break;
			case 2://Normal
				PlaySound(SOUND_LABEL_SE_EXPLOSION_ENEMY_000);
				for (nCntBullet4 = 0; nCntBullet4 < 30; nCntBullet4++)
				{
					fCircleBullet_E8_normal[nCntBullet4] = (1.0f / (30 / 2)) * nCntBullet4;

					fSpeedBullet_E8 = rand() % 5 + 1;


					SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * fCircleBullet_E8_normal[nCntBullet4]) * fSpeedBullet_E8), (cosf(-D3DX_PI * fCircleBullet_E8_normal[nCntBullet4]) * fSpeedBullet_E8), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�	

				}
				break;
			case 3://Hard
				PlaySound(SOUND_LABEL_SE_EXPLOSION_ENEMY_000);
				for (nCntBullet4 = 0; nCntBullet4 < 50; nCntBullet4++)
				{
					fCircleBullet_E8_hard[nCntBullet4] = (1.0f / (50 / 2)) * nCntBullet4;

					fSpeedBullet_E8 = rand() % 5 + 1;


					SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * fCircleBullet_E8_hard[nCntBullet4]) * fSpeedBullet_E8), (cosf(-D3DX_PI * fCircleBullet_E8_hard[nCntBullet4]) * fSpeedBullet_E8), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�	

				}
				break;
			case 4://Hard
				PlaySound(SOUND_LABEL_SE_EXPLOSION_ENEMY_000);
				for (nCntBullet4 = 0; nCntBullet4 < 100; nCntBullet4++)
				{
					fCircleBullet_E8_veryhard[nCntBullet4] = (1.0f / (100 / 2)) * nCntBullet4;

					fSpeedBullet_E8 = rand() % 6+ 1;


					SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((sinf(-D3DX_PI * fCircleBullet_E8_veryhard[nCntBullet4]) * fSpeedBullet_E8), (cosf(-D3DX_PI * fCircleBullet_E8_veryhard[nCntBullet4]) * fSpeedBullet_E8), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, nCntEnemy, -1);//�G�̒e������G���m�œ�����Ȃ�	

				}
				break;
			}
			
		}
			g_nNumEnemy--;//�G��l�ɂ���񂾂��G�̑����J�E���g�_�E��
		
		


		/*if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_DELBY)
		{
			AddScore(2000);
		}*/
		if (g_aEnemy[nCntEnemy].nType == ENEMYTYPE_BOSS1_STARWARRIOR)
		{
			AddScore(2000000);

			for (int nCnt = 0; nCnt < 500; nCnt++)
			{
				SetParticle(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.8f), 100.0f, 1000);
			}
			if (g_bClearFlag == false)
			{

				/*if (pPlayer->nHitCount == 0)
				{
					if (g_bNohitFlag == false)
					{
						g_bNohitFlag = true;
						AddScore(3000000);
					}

				}*/

			
				//���[�h�ݒ�
				pResult->bResultFlag = true;//���U���g��ʂ��Ăяo���t���O��true�ɂ���B
				pResultType->nResultType = RESULTTYPE_CLEAR;//�Q�[���N���A�p���U���g���Ăяo���B
				SetGameState(GAMESTATE_END);
				g_bClearFlag = true;
			}
		}
		AddScore(500);
		if (g_nNumEnemy <= 0)
		{//�G�̑������O�ȉ��ɂȂ�����
			//���[�h�ݒ�

		}
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
	if (fDamage <= 1&&fDamage>=0)
	{
		SetDamage(fDamage, 100, D3DXVECTOR3((sinf(-D3DX_PI * fRandumRot) * fRandumSpeed), (cosf(-D3DX_PI * fRandumRot) * fRandumSpeed), 0.0f), g_aEnemy[nCntEnemy].pos, 10.0f);

	}
	else if (fDamage <= 5 && fDamage >= 2)
	{
		SetDamage(fDamage, 100, D3DXVECTOR3((sinf(-D3DX_PI * fRandumRot)* fRandumSpeed), (cosf(-D3DX_PI * fRandumRot)* fRandumSpeed), 0.0f), g_aEnemy[nCntEnemy].pos, 15.0f);
	}
	else if (fDamage >= 15 && fDamage <= 50)
	{
		SetDamage(fDamage, 100, D3DXVECTOR3((sinf(-D3DX_PI * fRandumRot) * fRandumSpeed), (cosf(-D3DX_PI * fRandumRot) * fRandumSpeed), 0.0f), g_aEnemy[nCntEnemy].pos, 40.0f);
	}
	else if (fDamage >= 70)
	{
		SetDamage(fDamage, 100, D3DXVECTOR3((sinf(-D3DX_PI * fRandumRot) * fRandumSpeed), (cosf(-D3DX_PI * fRandumRot) * fRandumSpeed), 0.0f), g_aEnemy[nCntEnemy].pos, 80.0f);
	}
	//=======================================================================================================================================================================================


		for (int nCnt = 0; nCnt < 5; nCnt++)
		{
			SetParticle(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 0.5f, 0.0f, 0.8f), 20.0f, 100);
		}

		if (g_aEnemy[nCntEnemy].nType != ENEMYTYPE_BOSS1_STARWARRIOR)
		{
			PlaySound(SOUND_LABEL_SE_HIT);
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