//========================================================
//
//�W���R�O���F�G�t�F�N�g������񌩒���[bullet.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#include "main.h"
#include "bullet.h"
#include "input.h"
#include "explosion.h"
#include "enemy.h"
#include "player.h"
#include "health.h"
#include <time.h>
#include "effect.h"
#include "sound.h"
#include "particle.h"
#include "health.h"
#include "block.h"
#include "Attack.h"
#include "health.h"
#include "staging.h"

#define NUM_BULLET (23)//�e�̎�ސ�
#define BIGBULLET_LV1 (40.0f)
#define BIGBULLET_LV2 (150.0f)//�傫���eLV24
#define BULLETSCALE_NOVA (100.0f)
#define FINALBALLSIZE (150.0f)
#define BULLETSIZE_SCATTERING (30.0f)


//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureBullet[NUM_BULLET] = {};//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;//�e�̏��
Bullet g_aBullet[MAX_BULLET];
float g_fXMove;//X���W�̈ړ�
float g_fYMove;//Y���W�̈ړ�

float g_fCurveBullet;
float g_fEnemy9Bullet;

float g_fCircleBullet_B[MAX_CIRCLEBULLET];
int g_nRandumBullet2_B;
float g_fRandumResultBullet2_B;//�����_���Œe�������������܂�B

int g_nDelayBullet;

int g_nNumEnemy[MAX_ENEMY];//�G�l�~�[�̐��𐔂���ϐ�
D3DXVECTOR3 g_posBullet;//�ʒu
D3DXVECTOR3 g_moveBullet;//�ړ���
D3DXVECTOR3 g_rotPl;//����

float g_fRandumScatteringBullet_Rot;//�΂�܂��g�U�e�̏���
float g_fRandumScatternigBullet_Speed;//�΂�܂��g�U�e�̃X�s�[�h

int g_nHitCount;//�v���C���[����e�����񐔂��J�E���g����ϐ�

//int g_nAnimatioinCounter_Slash[MAX_BULLET];//�A�j���[�V���������̃e�N�X�`���ɂ���J�E���^�[
//float g_fAnimationPattern_Slash[MAX_BULLET];//�A�j���[�V�����̃e�N�X�`�����Ǘ�����ϐ�

//�e�̏���������
void InitBullet(void)
{

	//srand((unsigned int)time(NULL));//�Q�[�����J�n�������Ԗ��ɗ����̎��ݒ�

	g_moveBullet = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�ړ��ʂ�����������

	g_fCurveBullet = 0.0f;
	g_fEnemy9Bullet= 0.0f;

	g_nDelayBullet = 0;

	g_fRandumScatteringBullet_Rot = 0.0f;
	g_fRandumScatternigBullet_Speed = 0.0f;

	g_nHitCount=0;

	//g_nDelayBullet;

	LPDIRECT3DDEVICE9 pDevice;
	int	nCntBullet;


	int nCntEnemy;
	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ��݁i�e�̃e�N�X�`����ǂݍ��ށj
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\yellowgreenball_char.png",
		&g_pTextureBullet[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\redball_enemy.png",
		&g_pTextureBullet[1]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\brueball_char.png",
		&g_pTextureBullet[2]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Skill_Slash.png",
		&g_pTextureBullet[3]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\lightbrueball_enemy.png",
		&g_pTextureBullet[4]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\orangeball_enemy.png",
		&g_pTextureBullet[5]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\arrow_bullet_000.png",
		&g_pTextureBullet[6]);

		D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\brueball_char.png",
		&g_pTextureBullet[7]);

		D3DXCreateTextureFromFile(pDevice,
			"data\\TEXTURE\\purpleball_enemy.png",
			&g_pTextureBullet[8]);

		D3DXCreateTextureFromFile(pDevice,
			"data\\TEXTURE\\yellowgreenball_enemy.png",
			&g_pTextureBullet[9]);

		D3DXCreateTextureFromFile(pDevice,
			"data\\TEXTURE\\wind_bullet_000.png",
			&g_pTextureBullet[10]);

		D3DXCreateTextureFromFile(pDevice,
			"data\\TEXTURE\\Reflection_Bullet.png",
			&g_pTextureBullet[11]);

		D3DXCreateTextureFromFile(pDevice,
			"data\\TEXTURE\\NovaBall.png",
			&g_pTextureBullet[12]);

		D3DXCreateTextureFromFile(pDevice,
			"data\\TEXTURE\\Rockon_ball_000.png",
			&g_pTextureBullet[13]);

		D3DXCreateTextureFromFile(pDevice,
			"data\\TEXTURE\\BulletSeed.png",
			&g_pTextureBullet[14]);

		D3DXCreateTextureFromFile(pDevice,
			"data\\TEXTURE\\SnowBall.png",
			&g_pTextureBullet[15]);

		D3DXCreateTextureFromFile(pDevice,
			"data\\TEXTURE\\IcicleBullet.png",
			&g_pTextureBullet[16]);

		D3DXCreateTextureFromFile(pDevice,
			"data\\TEXTURE\\SpletBullet.png",
			&g_pTextureBullet[17]);

		D3DXCreateTextureFromFile(pDevice,
			"data\\TEXTURE\\JumpBullet.png",
			&g_pTextureBullet[18]);

		D3DXCreateTextureFromFile(pDevice,
			"data\\TEXTURE\\ExplosionBullet.png",
			&g_pTextureBullet[19]);

		D3DXCreateTextureFromFile(pDevice,
			"data\\TEXTURE\\FinalBall.png",
			&g_pTextureBullet[20]);
	//g_posBullet = D3DXVECTOR3(600.0f, 300.0f, 0.0f);//�ʒu������������


	for (nCntBullet = 0; nCntBullet < MAX_CIRCLEBULLET; nCntBullet++)
	{//�~�`�e�U�e�̏�����
		g_fCircleBullet_B[nCntBullet] = 0;
	}

	//�e���̏�����
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].nLife = 0;
		g_aBullet[nCntBullet].bUse = false;//�g�p���Ă��Ȃ���Ԃɂ���
		g_aBullet[nCntBullet].type = BULLETTYPE_PLAYER;
		g_aBullet[nCntBullet].fCurveBulletX = 0;//���ꂼ��̒e���ɃJ�[�u�ʂ̐��l����������B
		g_aBullet[nCntBullet].fCurveBulletY = 0;//���ꂼ��̒e���ɃJ�[�u�ʂ̐��l����������B
		g_aBullet[nCntBullet].nAnimationCounter_Slash = 0;
		g_aBullet[nCntBullet].fAnimationPattern_Slash = 0.0f;
		g_aBullet[nCntBullet].bCurveFlag = false;
		g_aBullet[nCntBullet].fEnemy5Curve = 0.0f;//�G�l�~�[�T�̒e���J�[�u������ϐ�
		g_aBullet[nCntBullet].nApperCount = 0;//�G�l�~�[�T�̒e���J�[�u������ϐ�
		g_aBullet[nCntBullet].nNumber = 0;//�G�l�~�[�T�̏���
		g_aBullet[nCntBullet].nNumEnemy = -1;//�ǂ̓G�����˂��Ă��邩�𔻕ʂ���ϐ�
		g_aBullet[nCntBullet].bBulletReflectionX = false;//X�����ɒe�𔽎˂�����t���O
		g_aBullet[nCntBullet].bBulletReflectionY = false;//Y�����ɒe�𔽎˂�����t���O
		g_aBullet[nCntBullet].nHormingaim = 0;//�z�[�~���O�ő_���Ă���G�𔻕ʂ���ϐ�
		g_aBullet[nCntBullet].HormingFlag = false;
		g_aBullet[nCntBullet].nHormingCount=0;
		g_aBullet[nCntBullet].fSpeed = 0.0f;//�o���b�g�̃X�s�[�h��ς���B
		g_aBullet[nCntBullet].fCurve = 0.0f;//�o���b�g�̃J�[�u�l��ς���B
		g_aBullet[nCntBullet].bCollisionBullet = false;//�e���u���b�N�ɏՓ˂��邩�ǂ����̃t���O
		g_aBullet[nCntBullet].moveType = MOVETYPE_NORMAL;//�e�̓����������߂�
		g_aBullet[nCntBullet].nAnimationCounter = 0;
		g_aBullet[nCntBullet].nAnimationPattern = 0;
		g_aBullet[nCntBullet].nCntTime = 0;
     }

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		g_nNumEnemy[nCntEnemy] = -1;//������
	}
	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BULLET,//�S�͕K�v�Ȓ��_��
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL);

	VERTEX_2D* pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//���_���W�̐ݒ�(�e�̑傫���j
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

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
	g_pVtxBuffBullet->Unlock();
}

//�e�̏I������
void UninitBullet(void)
{
	//�e�N�X�`���̔j��
	int nCnt;
	for (nCnt = 0; nCnt < NUM_BULLET; nCnt++)
	{
		if (g_pTextureBullet[nCnt] != NULL)
		{
			g_pTextureBullet[nCnt]->Release();
			g_pTextureBullet[nCnt] = NULL;
		}
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//�e�̍X�V����
void UpdateBullet(void)
{
	int nCntBullet3;

	g_nRandumBullet2_B = rand() % 100 + 1;//�~�`�e�U�e�̔��˕����̒����p

	g_fRandumResultBullet2_B = 0.01f * g_nRandumBullet2_B;//�~�`�g�U�e�̕�����������
	//g_fRandumResultBullet2_B = 0.01f;//�~�`�g�U�e�̕�����������


	g_fRandumScatteringBullet_Rot= float((rand)() % 629 - 314) / 100.0f;//�e�𔭎˂�����������߂�

    

	g_nDelayBullet++;

	Player* pPlayer = GetPlayer();
	//g_aBullet.fCurveBulletX += 0.01f;
	//g_fRotBulletY += 0.02f;
	//
	//LPDIRECT3DDEVICE9 pDevice;
	int	nCntBullet;
	//g_fEnemy9Bullet += 0.0001f;

	//Health* pHealth = GetHealth();

	Enemy* pEnemy = GetEnemy();

	VERTEX_2D* pVtx;

	
	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{//�e���g�p����Ă���
			int nCntEnemy;
			/*SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 0.5f, 0.5f), 3.0f, 50);*/


			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			//========================================================================
			//�X�L���Q�̃e�N�X�`���A�j���[�V����
			//========================================================================
			if (g_aBullet[nCntBullet].type == BULLETTYPE_SKILL2_SLASH)
			{
				g_aBullet[nCntBullet].nAnimationCounter_Slash++;//�e�N�X�`���A�j���[�V�����̏���

				if (g_aBullet[nCntBullet].nAnimationCounter_Slash % 3 == 0)
				{
					g_aBullet[nCntBullet].fAnimationPattern_Slash += 0.2f;//�e�N�X�`���A�j���[�V������i�߂�B
				}
				if (g_aBullet[nCntBullet].fAnimationPattern_Slash >= 5)
				{
					g_aBullet[nCntBullet].fAnimationPattern_Slash = 0.0f;//�e�N�X�`���̃A�j���[�V��������ڂɖ߂��B
				}
				/*if (g_aBullet[nCntBullet].bUse == false)
				{
					g_aBullet[nCntBullet].fAnimationPattern_Slash = 0.0f;
				}*/
				//�X�L���Q�e�N�X�`�����W�i�A�j���[�V�����j�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2(0.0f + g_aBullet[nCntBullet].fAnimationPattern_Slash, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.2f + g_aBullet[nCntBullet].fAnimationPattern_Slash, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f + g_aBullet[nCntBullet].fAnimationPattern_Slash, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.2f + g_aBullet[nCntBullet].fAnimationPattern_Slash, 1.0f);

				/*if (g_aBullet[nCntBullet].bUse == false)
				{
					pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
					pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
					pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
					pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);
				}*/
			}
			//=============================================================================================================

			g_aBullet[nCntBullet].fCurveBulletX += 0.017f;//���ꂼ��̒e���ɃJ�[�u�ʂ̐��l����������B
			g_aBullet[nCntBullet].fCurveBulletY -= 0.00f;//���ꂼ��̒e���ɃJ�[�u�ʂ̐��l����������B

			g_aBullet[nCntBullet].nLife -= 1;//�\�����鎞�ԁi�����j�����炷�B




			//=======================================================
			//�e�̑傫���A�����A�A�j���[�V�����Ȃǂ̏���
			//=======================================================

				//���ɕύX�������Ȃ��e�̑傫��
			pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - 10.0f, g_aBullet[nCntBullet].pos.y - 10.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + 10.0f, g_aBullet[nCntBullet].pos.y - 10.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - 10.0f, g_aBullet[nCntBullet].pos.y + 10.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + 10.0f, g_aBullet[nCntBullet].pos.y + 10.0f, 0.0f);

			if (g_aBullet[nCntBullet].type == BULLETTYPE_SKILL1)
			{//�X�L���P�̒e�̑傫����ݒ�
				pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - BIGBULLET_LV1, g_aBullet[nCntBullet].pos.y - BIGBULLET_LV1, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + BIGBULLET_LV1, g_aBullet[nCntBullet].pos.y - BIGBULLET_LV1, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - BIGBULLET_LV1, g_aBullet[nCntBullet].pos.y + BIGBULLET_LV1, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + BIGBULLET_LV1, g_aBullet[nCntBullet].pos.y + BIGBULLET_LV1, 0.0f);
			}

			if (g_aBullet[nCntBullet].type == BULLETTYPE_SKILL2_SLASH)
			{//�X�L���Q�̒e�̑傫����ݒ�


				pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - BIGBULLET_LV2, g_aBullet[nCntBullet].pos.y - (BIGBULLET_LV2 - 50.0f), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + BIGBULLET_LV2, g_aBullet[nCntBullet].pos.y - (BIGBULLET_LV2 - 50.0f), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - BIGBULLET_LV2, g_aBullet[nCntBullet].pos.y + (BIGBULLET_LV2 - 50.0f), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + BIGBULLET_LV2, g_aBullet[nCntBullet].pos.y + (BIGBULLET_LV2 - 50.0f), 0.0f);


			}
			if (g_aBullet[nCntBullet].type == BULLETTYPE_SCATTERING)
			{//�X�L���Q�̒e�̑傫����ݒ�


				pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - BULLETSIZE_SCATTERING, g_aBullet[nCntBullet].pos.y - (BULLETSIZE_SCATTERING), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + BULLETSIZE_SCATTERING, g_aBullet[nCntBullet].pos.y - (BULLETSIZE_SCATTERING), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - BULLETSIZE_SCATTERING, g_aBullet[nCntBullet].pos.y + (BULLETSIZE_SCATTERING), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + BULLETSIZE_SCATTERING, g_aBullet[nCntBullet].pos.y + (BULLETSIZE_SCATTERING), 0.0f);


			}
			if (g_aBullet[nCntBullet].type == BULLETTYPE_NOVA)
			{//�X�L���Q�̒e�̑傫����ݒ�


				pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - BULLETSCALE_NOVA, g_aBullet[nCntBullet].pos.y - (BULLETSCALE_NOVA), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + BULLETSCALE_NOVA, g_aBullet[nCntBullet].pos.y - (BULLETSCALE_NOVA), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - BULLETSCALE_NOVA, g_aBullet[nCntBullet].pos.y + (BULLETSCALE_NOVA), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + BULLETSCALE_NOVA, g_aBullet[nCntBullet].pos.y + (BULLETSCALE_NOVA), 0.0f);


			}
			//=============================================================================================================================================

			//==============================================
			//����̏������Œe����������
			//==============================================
			if ((g_aBullet[nCntBullet].pos.x >= SCREEN_WIDTH) || (g_aBullet[nCntBullet].pos.x <= 0) ||
				(g_aBullet[nCntBullet].pos.y >= SCREEN_HEIGHT) || (g_aBullet[nCntBullet].pos.y <= 0))//�p�ɂԂ�����
			{
				/*if (g_aBullet[nCntBullet].type == BULLETTYPE_PLAYER ||
					g_aBullet[nCntBullet].type == BULLETTYPE_ENEMY ||
					g_aBullet[nCntBullet].type == BULLETTYPE_SKILL1 ||
					g_aBullet[nCntBullet].type == BULLETTYPE_ROT ||
					g_aBullet[nCntBullet].type == BULLETTYPE_REAFY ||
					g_aBullet[nCntBullet].type == BULLETTYPE_ENEMY9 ||
					g_aBullet[nCntBullet].type == ENEMYTYPE_BOLBY||
					g_aBullet[nCntBullet].type==BULLETTYPE_BANDY)*/
				if (g_aBullet[nCntBullet].type != BULLETTYPE_ARROW)
				{
					if (g_aBullet[nCntBullet].type != BULLETTYPE_REFLECTION)
					{
						if (g_aBullet[nCntBullet].type != BULLETTYPE_SATELLITE)
						{
							if (g_aBullet[nCntBullet].type != BULLETTYPE_SEED || g_aBullet[nCntBullet] .type != BULLETTYPE_JUMPING)
							{
								SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), EXPLOSIONTYPE_NORMAL, 10.0f, 10.0f);

								if (g_aBullet[nCntBullet].type == BULLETTYPE_NOVA)
								{
									SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), EXPLOSIONTYPE_NOVA, 400.0f, 400.0f);
								}
								g_aBullet[nCntBullet].bUse = false;//�g�p���Ă��Ȃ���Ԃɂ���B
							}
						}
					}
				}


			}

			Block* pBlock = GetBlock();
			
			if (g_aBullet[nCntBullet].bCollisionBullet == true)
			{//�u���b�N�ɓ����邩�ǂ����̃t���O��true�̎�
				for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++, pBlock++)
				{
					if (pBlock->bUse == true)
					{
						if (g_aBullet[nCntBullet].pos.x >= pBlock->pos.x - pBlock->fRadius && g_aBullet[nCntBullet].pos.x <= pBlock->pos.x + pBlock->fRadius &&
							g_aBullet[nCntBullet].pos.y >= pBlock->pos.y - pBlock->fRadius && g_aBullet[nCntBullet].pos.y <= pBlock->pos.y + pBlock->fRadius)
						{//�v���C���[�ɒe����������

							//PlaySound(SOUND_LABEL_SE_DAMAGE_000);

							SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), EXPLOSIONTYPE_NORMAL, 10.0f, 10.0f);
							//�G�̒e���g�p���Ă��Ȃ���Ԃɂ���
							g_aBullet[nCntBullet].bUse = false;//�e���g�p���Ă��Ȃ���Ԃɂ���

						}
					}
				}
			}

			
			//===================================================================================================================================================


			//=============================================================
			//�����蔻��A�q�b�g���̈З́A�J�[�u�Ȃǂ̏���
			//===============================================================
			if (g_aBullet[nCntBullet].type == BULLETTYPE_PLAYER)
			{//�v���C���[�̒e
				
				Enemy* pEnemy = GetEnemy();
				for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				{
					if (pEnemy->bUse == true)
					{//�G���g�p����Ă���
						
					    if (g_aBullet[nCntBullet].pos.x >= pEnemy->pos.x - pEnemy->fWidth /2 && g_aBullet[nCntBullet].pos.x <= pEnemy->pos.x + pEnemy->fWidth / 2 &&
							g_aBullet[nCntBullet].pos.y >= pEnemy->pos.y - pEnemy->fHeight && g_aBullet[nCntBullet].pos.y <= pEnemy->pos.y)
						{//�G�ƒe����������
							HitEnemy(nCntEnemy,5);

							SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), EXPLOSIONTYPE_NORMAL,10.0f,10.0f);
							//pEnemy->bUse = false;//�G���g�p���Ă��Ȃ���Ԃɂ���
							g_aBullet[nCntBullet].bUse = false;//�e���g�p���Ă��Ȃ���Ԃɂ���

						}
					}

				}
			}
			if (g_aBullet[nCntBullet].type == BULLETTYPE_ROT)
			{//�v���C���[�̒e

				Enemy* pEnemy = GetEnemy();
				for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				{
					if (pEnemy->bUse == true)
					{//�G���g�p����Ă���
						if (g_aBullet[nCntBullet].pos.x >= pEnemy->pos.x - pEnemy->fWidth / 2 && g_aBullet[nCntBullet].pos.x <= pEnemy->pos.x + pEnemy->fWidth / 2 &&
							g_aBullet[nCntBullet].pos.y >= pEnemy->pos.y - pEnemy->fHeight && g_aBullet[nCntBullet].pos.y <= pEnemy->pos.y)
						{//�G�ƒe����������
							HitEnemy(nCntEnemy, 20.0f);

							SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), EXPLOSIONTYPE_NORMAL,10.0f,10.0f);
							//pEnemy->bUse = false;//�G���g�p���Ă��Ȃ���Ԃɂ���
							g_aBullet[nCntBullet].bUse = false;//�e���g�p���Ă��Ȃ���Ԃɂ���

						}
					}
				}
			}
		
			else if (g_aBullet[nCntBullet].type == BULLETTYPE_ENEMY)
			{//�G�̒e(�G�S�̂ɓK�p�j

				g_aBullet[nCntBullet].nCntTime++;

				if (g_aBullet[nCntBullet].nCntTime == 1)
				{
					PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_001);
				}
				Player* pPlayer = GetPlayer();
				/*for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pPlayer++)
				{*/

				SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 10.0f, 30, EFFECTSCALE_NORMAL);
				if (g_aBullet[nCntBullet].pos.x >= pPlayer->pos.x - pPlayer->fWidth&& g_aBullet[nCntBullet].pos.x <= pPlayer->pos.x + pPlayer->fWidth &&
					g_aBullet[nCntBullet].pos.y >= pPlayer->pos.y - pPlayer->fHeight && g_aBullet[nCntBullet].pos.y <= pPlayer->pos.y)
				{//�v���C���[�ɒe����������

					//PlaySound(SOUND_LABEL_SE_DAMAGE_000);

				SetExplosion(pPlayer->pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),EXPLOSIONTYPE_NORMAL,10.0f,10.0f);
					//�G�̒e���g�p���Ă��Ȃ���Ԃɂ���
					g_aBullet[nCntBullet].bUse = false;//�e���g�p���Ă��Ȃ���Ԃɂ���
					DamageHealth(5);
				}
				/*}*/
			}
			if (g_aBullet[nCntBullet].type == BULLETTYPE_REAFY)
			{//�G�l�~�[�T�̒e
				Player* pPlayer = GetPlayer();
				/*	g_aBullet[nCntBullet].move += D3DXVECTOR3((sinf(-D3DX_PI *g_aBullet[nCntBullet].fCurveBulletX) * 0.1f), (cosf(-D3DX_PI *
						g_aBullet[nCntBullet].fCurveBulletX) * 0.1f), 0.0f);*/

				SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 0.0f, 1.0f, 0.5f), 15.0f, 300, EFFECTSCALE_NORMAL);

				g_aBullet[nCntBullet].fCurveBulletX += 0.001f;/*
				g_aBullet[nCntBullet].fCurveBulletY += 0.02f;*/
				/*	g_fCurveBullet += 0.1f;*/

					//if (g_aBullet[nCntBullet].fCurveBulletX >= 0.3f)
					//{
					//	g_aBullet[nCntBullet].bCurveFlag == true;
					//}
					//if (g_aBullet[nCntBullet].bCurveFlag == true)
					//{
					//	g_aBullet[nCntBullet].fCurveBulletX -= 0.01f;
					//	g_aBullet[nCntBullet].move.x += (cosf(D3DX_PI * g_aBullet[nCntBullet].fCurveBulletX) * 0.1f);
					//}
					//if (g_aBullet[nCntBullet].fCurveBulletX <= -0.3f)
					//{
					//	g_aBullet[nCntBullet].bCurveFlag == false;
					//}
					//if (g_aBullet[nCntBullet].bCurveFlag == false)
					//{
					//	g_aBullet[nCntBullet].fCurveBulletX += 0.01f;//�J�[�u�p����
					//	g_aBullet[nCntBullet].move.x += (sinf(D3DX_PI * g_aBullet[nCntBullet].fCurveBulletX) * 0.1f);
					//}


				/*g_aBullet[nCntBullet].move.x += (sinf(D3DX_PI * g_aBullet[nCntBullet].fCurveBulletX) * 0.1f);*/

				/*for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pPlayer++)
				{*/

				/*	g_aBullet[nCntBullet].move.x += (sinf(-D3DX_PI * g_fCurveBullet) * 5.0f);///�ʔ��������̂Ō�łق��̓G�Ɏ�������I�I�I�I�I�I�I�I�I�I�I�I�I�I
						g_aBullet[nCntBullet].move.y += (sinf(-D3DX_PI * 0.0f) * 0.002f);*/
						/*g_aBullet[nCntBullet].move.x = 0.0f;*/
						//int nCntBullet3;
						//for (nCntBullet3 = 0; nCntBullet3 < MAX_CIRCLEBULLET; nCntBullet3++)
						//{
						//	g_fCircleBullet_B[nCntBullet3] = (1.0f / 5.0f) * nCntBullet3;

					/*	for (nCntBullet3 = 0; nCntBullet3 < MAX_CIRCLEBULLET_2; nCntBullet3++)
						{*/
						/*	g_nDelayBullet[nCntBullet3]++;*/
							//	g_fCircleBullet[nCntBullet3] = (1.0f / 5.0f) * nCntBullet3;
				g_aBullet[nCntBullet].nApperCount++;

				//if (g_aBullet[nCntBullet].nApperCount == 1)
				//{
				//	//g_aBullet[nCntBullet].fEnemy5Curve = (D3DX_PI / 5.0f) * (nCntBullet + g_fRandumResultBullet2_B);
				//	g_aBullet[nCntBullet].fEnemy5Curve = (D3DX_PI / 5.0f) * g_aBullet[nCntBullet].nNumber + g_fRandumResultBullet2_B;
				//}


				//�p�^�[���P======================================
				/*if (g_nDelayBullet % 5== 0)
				{
					g_aBullet[nCntBullet].fEnemy5Curve += ((D3DX_PI / 15000.0f) * g_aBullet[nCntBullet].nApperCount);
				}*/
				//======================================================================================================
				//�p�^�[���Q===========================================================

				if (g_aBullet[nCntBullet].nApperCount == 1)
				{
					g_aBullet[nCntBullet].fEnemy5Curve = (D3DX_PI / 8.0f) * g_aBullet[nCntBullet].nNumber + g_fRandumResultBullet2_B;
				}

				if (g_nDelayBullet % 5 == 0)
				{
					g_aBullet[nCntBullet].fEnemy5Curve += ((D3DX_PI / 50.0f) * 0.5f);
				}
				//===========================================================================


				g_aBullet[nCntBullet].move.x = (sinf(g_aBullet[nCntBullet].fEnemy5Curve) * 5.5f);
				g_aBullet[nCntBullet].move.y = (cosf(g_aBullet[nCntBullet].fEnemy5Curve) * 5.5f);



				/*	g_aBullet[nCntBullet].move.x += (sinf(g_fCircleBullet_B[nCntBullet3] + g_fRandumResultBullet2_B) * 0.1f);
					g_aBullet[nCntBullet].move.y += (cosf(g_fCircleBullet_B[nCntBullet3] + g_fRandumResultBullet2_B) * 0.1f);*/

				if (g_aBullet[nCntBullet].pos.x >= pPlayer->pos.x - pPlayer->fWidth && g_aBullet[nCntBullet].pos.x <= pPlayer->pos.x + pPlayer->fWidth &&
					g_aBullet[nCntBullet].pos.y >= pPlayer->pos.y - pPlayer->fHeight && g_aBullet[nCntBullet].pos.y <= pPlayer->pos.y)
				{//�v���C���[�ɒe����������
					SetExplosion(pPlayer->pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), EXPLOSIONTYPE_NORMAL, 10.0f, 10.0f);
					//�G�̒e���g�p���Ă��Ȃ���Ԃɂ���
					g_aBullet[nCntBullet].bUse = false;//�e���g�p���Ă��Ȃ���Ԃɂ���
					//UpdateHealth(1.0f);//�ǉ��\��i����j
					DamageHealth(10);
					//pHealth -= 1;//�Ȃ����Q�_���[�W�����Ă��܂��̂ŁA�P�񕜂���B
				}
				/*}*/
			}

			if (g_aBullet[nCntBullet].type == BULLETTYPE_SATELLITE)
			{//�G�l�~�[�T�̒e
				Player* pPlayer = GetPlayer();
				SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 0.0f, 1.0f, 0.5f), 15.0f, 300, EFFECTSCALE_NORMAL);

				g_aBullet[nCntBullet].fCurveBulletX += 0.001f;
				g_aBullet[nCntBullet].nApperCount++;
				
				g_aBullet[nCntBullet].fSpeed = (g_aBullet[nCntBullet].nApperCount * 0.1f);
				g_aBullet[nCntBullet].fCurve -= 1.7f;
		
					if (g_aBullet[nCntBullet].nApperCount == 1)
					{
						g_aBullet[nCntBullet].fEnemy5Curve = (D3DX_PI / 8.0f) * g_aBullet[nCntBullet].nNumber + g_fRandumResultBullet2_B;
					}
					




				//�p�^�[���Q===========================================================
				
					g_aBullet[nCntBullet].fEnemy5Curve += ((D3DX_PI / g_aBullet[nCntBullet].fCurve));
				
				//===========================================================================


				g_aBullet[nCntBullet].move.x = (sinf(g_aBullet[nCntBullet].fEnemy5Curve) * g_aBullet[nCntBullet].fSpeed);
				g_aBullet[nCntBullet].move.y = (cosf(g_aBullet[nCntBullet].fEnemy5Curve) * g_aBullet[nCntBullet].fSpeed);



				

				if (g_aBullet[nCntBullet].pos.x >= pPlayer->pos.x - pPlayer->fWidth && g_aBullet[nCntBullet].pos.x <= pPlayer->pos.x + pPlayer->fWidth &&
					g_aBullet[nCntBullet].pos.y >= pPlayer->pos.y - pPlayer->fHeight && g_aBullet[nCntBullet].pos.y <= pPlayer->pos.y)
				{//�v���C���[�ɒe����������
					SetExplosion(pPlayer->pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),EXPLOSIONTYPE_NORMAL,10.0f,10.0f);
					//�G�̒e���g�p���Ă��Ȃ���Ԃɂ���
					g_aBullet[nCntBullet].bUse = false;//�e���g�p���Ă��Ȃ���Ԃɂ���
					DamageHealth(15);//�v���C���[��15�_���[�W��^����B
					
				}
				/*}*/
			}
			if (g_aBullet[nCntBullet].type == BULLETTYPE_ENEMY9)
			{

				SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 0.5f, 0.5f), 15.0f, 300,EFFECTSCALE_NORMAL);

				g_aBullet[nCntBullet].fBulletMoveX_Enemy9 += 0.01f;
				g_aBullet[nCntBullet].fBulletMoveY_Enemy9 += 0.015f;

				///*	g_aBullet[nCntBullet].move.x = 0.0f;*/
				//	g_aBullet[nCntBullet].move.x += (sinf(-D3DX_PI * g_aBullet[nCntBullet].fBulletMoveX_Enemy9) * 0.1f);
				//	g_aBullet[nCntBullet].move.y += (cosf(-D3DX_PI * g_aBullet[nCntBullet].fCurveBulletX) * 0.1f);

				g_aBullet[nCntBullet].move.x += (sinf(-D3DX_PI * 0.0f) * 0.5f);
				g_aBullet[nCntBullet].move.y += (cosf(-D3DX_PI * g_aBullet[nCntBullet].fBulletMoveY_Enemy9) * 0.5f);
				
				

				if (g_aBullet[nCntBullet].pos.x >= pPlayer->pos.x - 5.0f && g_aBullet[nCntBullet].pos.x <= pPlayer->pos.x + 5.0f &&
					g_aBullet[nCntBullet].pos.y >= pPlayer->pos.y - 5.0f && g_aBullet[nCntBullet].pos.y <= pPlayer->pos.y + 5.0f)
				{//�v���C���[�ɒe����������
					SetExplosion(pPlayer->pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),EXPLOSIONTYPE_NORMAL,10.0f,10.0f);
					//�G�̒e���g�p���Ă��Ȃ���Ԃɂ���
					g_aBullet[nCntBullet].bUse = false;//�e���g�p���Ă��Ȃ���Ԃɂ���
					//UpdateHealth(1.0f);//�ǉ��\��i����j
					HitPlayer(1);//�v���C���[��1�_���[�W��^����B
					//pHealth -= 1;//�Ȃ����Q�_���[�W�����Ă��܂��̂ŁA�P�񕜂���B
				}
			}
			if (g_aBullet[nCntBullet].type == BULLETTYPE_BOLBY)
			{//�G�̒e(�G�S�̂ɓK�p�j
				
				SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f),15.0f,300,EFFECTSCALE_NORMAL);
				Player* pPlayer = GetPlayer();
				Enemy* pEnemy = GetEnemy();
				
					g_aBullet[nCntBullet].fXVaim = pPlayer->pos.x - pEnemy[g_aBullet[nCntBullet].nNumEnemy].pos.x;//X���W�ւ̃x�N�g��
					g_aBullet[nCntBullet].fYVaim = pPlayer->pos.y - pEnemy[g_aBullet[nCntBullet].nNumEnemy].pos.y;//Y���W�ւ̃x�N�g��

					g_aBullet[nCntBullet].fShotVectorX = (g_aBullet[nCntBullet].fXVaim - g_aBullet[nCntBullet].fVaim);
					g_aBullet[nCntBullet].fShotVectorY = (g_aBullet[nCntBullet].fYVaim - g_aBullet[nCntBullet].fVaim);
				


				/*g_aBullet[nCntBullet].move.x += ((D3DX_PI * g_aBullet[nCntBullet].fShotVectorX) * 0.002f);
				g_aBullet[nCntBullet].move.y+= ((D3DX_PI * g_aBullet[nCntBullet].fShotVectorY) * 0.002f);*/

				g_aBullet[nCntBullet].move.x += ((D3DX_PI *g_aBullet[nCntBullet].fShotVectorX*0.00001f));
				g_aBullet[nCntBullet].move.y += ((D3DX_PI * g_aBullet[nCntBullet].fShotVectorY*0.00001f));
				//SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3((D3DX_PI* g_fXVaim[nCntEnemy]) * 0.002f, (D3DX_PI* g_fYVaim[nCntEnemy]) * 0.002f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0);//�G�̒e������G���m�œ�����Ȃ�

				if (g_aBullet[nCntBullet].pos.x >= pPlayer->pos.x - 5.0f && g_aBullet[nCntBullet].pos.x <= pPlayer->pos.x + 5.0f &&
					g_aBullet[nCntBullet].pos.y >= pPlayer->pos.y - 5.0f && g_aBullet[nCntBullet].pos.y <= pPlayer->pos.y + 5.0f)
				{//�v���C���[�ɒe����������
					SetExplosion(pPlayer->pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),EXPLOSIONTYPE_NORMAL,10.0f,10.0f);
					//�G�̒e���g�p���Ă��Ȃ���Ԃɂ���
					g_aBullet[nCntBullet].bUse = false;//�e���g�p���Ă��Ȃ���Ԃɂ���
					//UpdateHealth(1.0f);//�ǉ��\��i����j
					HitPlayer(1);
				}
				/*}*/
			}
			if (g_aBullet[nCntBullet].type == BULLETTYPE_BANDY)
			{//�G�̒e(�G�S�̂ɓK�p�j

				SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.5f), 15.0f, 300,EFFECTSCALE_NORMAL);
				
				if (g_aBullet[nCntBullet].pos.x >= pPlayer->pos.x - pPlayer->fWidth && g_aBullet[nCntBullet].pos.x <= pPlayer->pos.x + pPlayer->fWidth &&
					g_aBullet[nCntBullet].pos.y >= pPlayer->pos.y - pPlayer->fHeight && g_aBullet[nCntBullet].pos.y <= pPlayer->pos.y)
				{//�v���C���[�ɒe����������
					SetExplosion(pPlayer->pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),EXPLOSIONTYPE_NORMAL,10.0f,10.0f);
					//�G�̒e���g�p���Ă��Ȃ���Ԃɂ���
					g_aBullet[nCntBullet].bUse = false;//�e���g�p���Ă��Ȃ���Ԃɂ���
					//UpdateHealth(1.0f);//�ǉ��\��i����j
					DamageHealth(5);
					
				}
			}
			if (g_aBullet[nCntBullet].type == BULLETTYPE_ROCKON)
			{//�X�Ɏ��@�_���e�𔭎�

				g_aBullet[nCntBullet].nCntTime++;

				if (g_aBullet[nCntBullet].nCntTime == 1)
				{
					PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_005);
				}

				SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 0.5f, 0.0f, 0.5f), 15.0f, 300, EFFECTSCALE_NORMAL);

				g_aBullet[nCntBullet].fXVaim= pPlayer->pos.x - g_aBullet[nCntBullet].pos.x;//X���W�ւ̃x�N�g��
				g_aBullet[nCntBullet].fYVaim= pPlayer->pos.y - g_aBullet[nCntBullet].pos.y;//Y���W�ւ̃x�N�g��


				g_aBullet[nCntBullet].fVaim= atan2(g_aBullet[nCntBullet].fXVaim, g_aBullet[nCntBullet].fYVaim);

				if (g_nDelayBullet % 3 == 0)
				{
					/*PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_007);*/
					SetBullet(g_aBullet[nCntBullet].pos, D3DXVECTOR3(sinf(g_aBullet[nCntBullet].fVaim) * 15.0f, cosf(g_aBullet[nCntBullet].fVaim) * 15.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1,false,MOVETYPE_NORMAL);
					SetBullet(g_aBullet[nCntBullet].pos, D3DXVECTOR3(sinf(g_aBullet[nCntBullet].fVaim + 0.25f) * 15.0f, cosf(g_aBullet[nCntBullet].fVaim + 0.25f) * 15.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1,false, MOVETYPE_NORMAL);
					SetBullet(g_aBullet[nCntBullet].pos, D3DXVECTOR3(sinf(g_aBullet[nCntBullet].fVaim - 0.25f) * 15.0f, cosf(g_aBullet[nCntBullet].fVaim - 0.25f) * 15.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1,false, MOVETYPE_NORMAL);
				/*	SetBullet(g_aBullet[nCntBullet].pos, D3DXVECTOR3(sinf(g_aBullet[nCntBullet].fVaim + 0.3f) * 15.0f, cosf(g_aBullet[nCntBullet].fVaim + 0.3f) * 15.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1, false, MOVETYPE_NORMAL);
					SetBullet(g_aBullet[nCntBullet].pos, D3DXVECTOR3(sinf(g_aBullet[nCntBullet].fVaim - 0.3f) * 15.0f, cosf(g_aBullet[nCntBullet].fVaim - 0.3f) * 15.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1, false, MOVETYPE_NORMAL);
					SetBullet(g_aBullet[nCntBullet].pos, D3DXVECTOR3(sinf(g_aBullet[nCntBullet].fVaim + 0.45f) * 15.0f, cosf(g_aBullet[nCntBullet].fVaim + 0.45f) * 15.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1, false, MOVETYPE_NORMAL);
					SetBullet(g_aBullet[nCntBullet].pos, D3DXVECTOR3(sinf(g_aBullet[nCntBullet].fVaim - 0.45f) * 15.0f, cosf(g_aBullet[nCntBullet].fVaim - 0.45f) * 15.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1, false, MOVETYPE_NORMAL);
					SetBullet(g_aBullet[nCntBullet].pos, D3DXVECTOR3(sinf(g_aBullet[nCntBullet].fVaim + 0.6f) * 15.0f, cosf(g_aBullet[nCntBullet].fVaim + 0.6f) * 15.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1, false, MOVETYPE_NORMAL);
					SetBullet(g_aBullet[nCntBullet].pos, D3DXVECTOR3(sinf(g_aBullet[nCntBullet].fVaim - 0.6f) * 15.0f, cosf(g_aBullet[nCntBullet].fVaim - 0.6f) * 15.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1, false, MOVETYPE_NORMAL);
					SetBullet(g_aBullet[nCntBullet].pos, D3DXVECTOR3(sinf(g_aBullet[nCntBullet].fVaim + 0.75f) * 15.0f, cosf(g_aBullet[nCntBullet].fVaim + 0.75f) * 15.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1, false, MOVETYPE_NORMAL);
					SetBullet(g_aBullet[nCntBullet].pos, D3DXVECTOR3(sinf(g_aBullet[nCntBullet].fVaim - 0.75f) * 15.0f, cosf(g_aBullet[nCntBullet].fVaim - 0.75f) * 15.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1, false, MOVETYPE_NORMAL);
					SetBullet(g_aBullet[nCntBullet].pos, D3DXVECTOR3(sinf(g_aBullet[nCntBullet].fVaim + 0.9f) * 15.0f, cosf(g_aBullet[nCntBullet].fVaim + 0.9f) * 15.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1, false, MOVETYPE_NORMAL);
					SetBullet(g_aBullet[nCntBullet].pos, D3DXVECTOR3(sinf(g_aBullet[nCntBullet].fVaim - 0.9f) * 15.0f, cosf(g_aBullet[nCntBullet].fVaim - 0.9f) * 15.0f, 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_BANDY, 6, -1, -1, false, MOVETYPE_NORMAL);*/

				}
		
				

				if (g_aBullet[nCntBullet].pos.x >= pPlayer->pos.x - pPlayer->fWidth&& g_aBullet[nCntBullet].pos.x <= pPlayer->pos.x + pPlayer->fWidth &&
					g_aBullet[nCntBullet].pos.y >= pPlayer->pos.y - pPlayer->fHeight && g_aBullet[nCntBullet].pos.y <= pPlayer->pos.y)
				{//�v���C���[�ɒe����������
					SetExplosion(pPlayer->pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),EXPLOSIONTYPE_NORMAL,10.0f,10.0f);
					//�G�̒e���g�p���Ă��Ȃ���Ԃɂ���
					g_aBullet[nCntBullet].bUse = false;//�e���g�p���Ă��Ȃ���Ԃɂ���
					//UpdateHealth(1.0f);//�ǉ��\��i����j
					DamageHealth(4);
				}
			}
			if (g_aBullet[nCntBullet].type == BULLETTYPE_SCATTERING)
			{//���˂����e����A����ɂ΂�܂��B
				float fRandumScattering_Rot= float((rand)() % 629 - 314) / 100.0f;//�e�𔭎˂�����������߂�
				float fRandumScattering_Speed= float((rand)() % 100+1)/ 10.0f;//�e�𔭎˂������̃X�s�[�h�����߂�B
				int nCntBullet_Scattering;
				SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(0.9f, 0.5f, 0.5f, 0.5f), 50.0f, 100, EFFECTSCALE_NORMAL);

				
					if (g_nDelayBullet % 20 == 0)
					{
						SetBullet(g_aBullet[nCntBullet].pos, D3DXVECTOR3((sinf(-D3DX_PI * (fRandumScattering_Rot)) * fRandumScattering_Speed), (cosf(-D3DX_PI * (fRandumScattering_Rot)) * fRandumScattering_Speed), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, -1, -1, -1, false, MOVETYPE_NORMAL);
					}
				

				

				if (g_aBullet[nCntBullet].pos.x >= pPlayer->pos.x - 5.0f && g_aBullet[nCntBullet].pos.x <= pPlayer->pos.x + 5.0f &&
					g_aBullet[nCntBullet].pos.y >= pPlayer->pos.y - 5.0f && g_aBullet[nCntBullet].pos.y <= pPlayer->pos.y + 5.0f)
				{//�v���C���[�ɒe����������
					SetExplosion(pPlayer->pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),EXPLOSIONTYPE_NORMAL,10.0f,10.0f);
					//�G�̒e���g�p���Ă��Ȃ���Ԃɂ���
					g_aBullet[nCntBullet].bUse = false;//�e���g�p���Ă��Ȃ���Ԃɂ���
					//UpdateHealth(1.0f);//�ǉ��\��i����j
					HitPlayer(1);
				}
			}
			if (g_aBullet[nCntBullet].type == BULLETTYPE_REVERSE_CURVE)
			{//�G�l�~�[�T�̒e
				Player* pPlayer = GetPlayer();
				/*	g_aBullet[nCntBullet].move += D3DXVECTOR3((sinf(-D3DX_PI *g_aBullet[nCntBullet].fCurveBulletX) * 0.1f), (cosf(-D3DX_PI *
						g_aBullet[nCntBullet].fCurveBulletX) * 0.1f), 0.0f);*/

				SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 0.0f, 1.0f, 0.5f), 15.0f, 300, EFFECTSCALE_NORMAL);

				g_aBullet[nCntBullet].fCurveBulletX += 0.001f;/*
				g_aBullet[nCntBullet].fCurveBulletY += 0.02f;*/
				/*	g_fCurveBullet += 0.1f;*/

					//if (g_aBullet[nCntBullet].fCurveBulletX >= 0.3f)
					//{
					//	g_aBullet[nCntBullet].bCurveFlag == true;
					//}
					//if (g_aBullet[nCntBullet].bCurveFlag == true)
					//{
					//	g_aBullet[nCntBullet].fCurveBulletX -= 0.01f;
					//	g_aBullet[nCntBullet].move.x += (cosf(D3DX_PI * g_aBullet[nCntBullet].fCurveBulletX) * 0.1f);
					//}
					//if (g_aBullet[nCntBullet].fCurveBulletX <= -0.3f)
					//{
					//	g_aBullet[nCntBullet].bCurveFlag == false;
					//}
					//if (g_aBullet[nCntBullet].bCurveFlag == false)
					//{
					//	g_aBullet[nCntBullet].fCurveBulletX += 0.01f;//�J�[�u�p����
					//	g_aBullet[nCntBullet].move.x += (sinf(D3DX_PI * g_aBullet[nCntBullet].fCurveBulletX) * 0.1f);
					//}


				/*g_aBullet[nCntBullet].move.x += (sinf(D3DX_PI * g_aBullet[nCntBullet].fCurveBulletX) * 0.1f);*/

				/*for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pPlayer++)
				{*/

				/*	g_aBullet[nCntBullet].move.x += (sinf(-D3DX_PI * g_fCurveBullet) * 5.0f);///�ʔ��������̂Ō�łق��̓G�Ɏ�������I�I�I�I�I�I�I�I�I�I�I�I�I�I
						g_aBullet[nCntBullet].move.y += (sinf(-D3DX_PI * 0.0f) * 0.002f);*/
						/*g_aBullet[nCntBullet].move.x = 0.0f;*/
						//int nCntBullet3;
						//for (nCntBullet3 = 0; nCntBullet3 < MAX_CIRCLEBULLET; nCntBullet3++)
						//{
						//	g_fCircleBullet_B[nCntBullet3] = (1.0f / 5.0f) * nCntBullet3;

					/*	for (nCntBullet3 = 0; nCntBullet3 < MAX_CIRCLEBULLET_2; nCntBullet3++)
						{*/
						/*	g_nDelayBullet[nCntBullet3]++;*/
							//	g_fCircleBullet[nCntBullet3] = (1.0f / 5.0f) * nCntBullet3;
				g_aBullet[nCntBullet].nApperCount++;

				//if (g_aBullet[nCntBullet].nApperCount == 1)
				//{
				//	//g_aBullet[nCntBullet].fEnemy5Curve = (D3DX_PI / 5.0f) * (nCntBullet + g_fRandumResultBullet2_B);
				//	g_aBullet[nCntBullet].fEnemy5Curve = (D3DX_PI / 5.0f) * g_aBullet[nCntBullet].nNumber + g_fRandumResultBullet2_B;
				//}


				//�p�^�[���P======================================
				/*if (g_nDelayBullet % 5== 0)
				{
					g_aBullet[nCntBullet].fEnemy5Curve += ((D3DX_PI / 15000.0f) * g_aBullet[nCntBullet].nApperCount);
				}*/
				//======================================================================================================
				//�p�^�[���Q===========================================================

					if (g_aBullet[nCntBullet].nApperCount == 1)
					{
						g_aBullet[nCntBullet].fEnemy5Curve = (D3DX_PI / 8.0f) * g_aBullet[nCntBullet].nNumber + g_fRandumResultBullet2_B;
					}

				if (g_nDelayBullet % 5 == 0)
				{
					g_aBullet[nCntBullet].fEnemy5Curve -= ((D3DX_PI / 50.0f) * 0.5f);
				}
				//===========================================================================

				
					g_aBullet[nCntBullet].move.x = (sinf(g_aBullet[nCntBullet].fEnemy5Curve) * 3.5f);
					g_aBullet[nCntBullet].move.y = (cosf(g_aBullet[nCntBullet].fEnemy5Curve) * 3.5f);
				


				/*	g_aBullet[nCntBullet].move.x += (sinf(g_fCircleBullet_B[nCntBullet3] + g_fRandumResultBullet2_B) * 0.1f);
					g_aBullet[nCntBullet].move.y += (cosf(g_fCircleBullet_B[nCntBullet3] + g_fRandumResultBullet2_B) * 0.1f);*/

					if (g_aBullet[nCntBullet].pos.x >= pPlayer->pos.x - pPlayer->fWidth && g_aBullet[nCntBullet].pos.x <= pPlayer->pos.x + pPlayer->fWidth &&
						g_aBullet[nCntBullet].pos.y >= pPlayer->pos.y - pPlayer->fHeight && g_aBullet[nCntBullet].pos.y <= pPlayer->pos.y)
					{//�v���C���[�ɒe����������
						SetExplosion(pPlayer->pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), EXPLOSIONTYPE_NORMAL, 10.0f, 10.0f);
						//�G�̒e���g�p���Ă��Ȃ���Ԃɂ���
						g_aBullet[nCntBullet].bUse = false;//�e���g�p���Ă��Ȃ���Ԃɂ���
						//UpdateHealth(1.0f);//�ǉ��\��i����j
						DamageHealth(10);
						//pHealth -= 1;//�Ȃ����Q�_���[�W�����Ă��܂��̂ŁA�P�񕜂���B
					}
				/*}*/
			}
			if (g_aBullet[nCntBullet].type == BULLETTYPE_RIGHT_WIND)
			{//�G�̒e(�G�S�̂ɓK�p�j

				
				g_aBullet[nCntBullet].move.x += (sinf(D3DX_PI * 0.5f) * 0.09f);


				SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(0.2f, 0.6f, 0.1f, 0.5f), 15.0f, 300, EFFECTSCALE_NORMAL);

				if (g_aBullet[nCntBullet].pos.x >= pPlayer->pos.x - 5.0f && g_aBullet[nCntBullet].pos.x <= pPlayer->pos.x + 5.0f &&
					g_aBullet[nCntBullet].pos.y >= pPlayer->pos.y - 5.0f && g_aBullet[nCntBullet].pos.y <= pPlayer->pos.y + 5.0f)
				{//�v���C���[�ɒe����������
					SetExplosion(pPlayer->pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),EXPLOSIONTYPE_NORMAL,10.0f,10.0f);
					//�G�̒e���g�p���Ă��Ȃ���Ԃɂ���
					g_aBullet[nCntBullet].bUse = false;//�e���g�p���Ă��Ȃ���Ԃɂ���
					//UpdateHealth(1.0f);//�ǉ��\��i����j
					HitPlayer(1);
				}
			}
			if (g_aBullet[nCntBullet].type == BULLETTYPE_LEFT_WIND)
			{//�G�̒e(�G�S�̂ɓK�p�j


				g_aBullet[nCntBullet].move.x += (sinf(-D3DX_PI * 0.5f) * 0.09f);


				SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(0.2f, 0.6f, 0.1f, 0.5f), 15.0f, 300, EFFECTSCALE_NORMAL);

				if (g_aBullet[nCntBullet].pos.x >= pPlayer->pos.x - 5.0f && g_aBullet[nCntBullet].pos.x <= pPlayer->pos.x + 5.0f &&
					g_aBullet[nCntBullet].pos.y >= pPlayer->pos.y - 5.0f && g_aBullet[nCntBullet].pos.y <= pPlayer->pos.y + 5.0f)
				{//�v���C���[�ɒe����������
					SetExplosion(pPlayer->pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),EXPLOSIONTYPE_NORMAL,10.0f,10.0f);
					//�G�̒e���g�p���Ă��Ȃ���Ԃɂ���
					g_aBullet[nCntBullet].bUse = false;//�e���g�p���Ă��Ȃ���Ԃɂ���
					//UpdateHealth(1.0f);//�ǉ��\��i����j
					HitPlayer(1);
				}
			}
			if (g_aBullet[nCntBullet].type == BULLETTYPE_UPWIND)
			{//�G�̒e(�G�S�̂ɓK�p�j


				g_aBullet[nCntBullet].move.y += (cosf(-D3DX_PI * 1.0f) * 0.09f);


				SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(0.2f, 0.6f, 0.1f, 0.5f), 15.0f, 300, EFFECTSCALE_NORMAL);

				if (g_aBullet[nCntBullet].pos.x >= pPlayer->pos.x - 5.0f && g_aBullet[nCntBullet].pos.x <= pPlayer->pos.x + 5.0f &&
					g_aBullet[nCntBullet].pos.y >= pPlayer->pos.y - 5.0f && g_aBullet[nCntBullet].pos.y <= pPlayer->pos.y + 5.0f)
				{//�v���C���[�ɒe����������
					SetExplosion(pPlayer->pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),EXPLOSIONTYPE_NORMAL,10.0f,10.0f);
					//�G�̒e���g�p���Ă��Ȃ���Ԃɂ���
					g_aBullet[nCntBullet].bUse = false;//�e���g�p���Ă��Ȃ���Ԃɂ���
					//UpdateHealth(1.0f);//�ǉ��\��i����j
					HitPlayer(1);
				}
			}
			if (g_aBullet[nCntBullet].type == BULLETTYPE_DOWNWIND)
			{//�G�̒e(�G�S�̂ɓK�p�j


				g_aBullet[nCntBullet].move.y += (cosf(-D3DX_PI * 0.0f) * 0.09f);


				SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(0.2f, 0.6f, 0.1f, 0.5f), 15.0f, 300, EFFECTSCALE_NORMAL);

				if (g_aBullet[nCntBullet].pos.x >= pPlayer->pos.x - 5.0f && g_aBullet[nCntBullet].pos.x <= pPlayer->pos.x + 5.0f &&
					g_aBullet[nCntBullet].pos.y >= pPlayer->pos.y - 5.0f && g_aBullet[nCntBullet].pos.y <= pPlayer->pos.y + 5.0f)
				{//�v���C���[�ɒe����������
					SetExplosion(pPlayer->pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),EXPLOSIONTYPE_NORMAL,10.0f,10.0f);
					//�G�̒e���g�p���Ă��Ȃ���Ԃɂ���
					g_aBullet[nCntBullet].bUse = false;//�e���g�p���Ă��Ȃ���Ԃɂ���
					//UpdateHealth(1.0f);//�ǉ��\��i����j
					HitPlayer(1);
				}
			}
			if (g_aBullet[nCntBullet].type == BULLETTYPE_SNOW)
			{
				SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f), 10.0f, 70, EFFECTSCALE_NORMAL);
				if (g_aBullet[nCntBullet].pos.x >= pPlayer->pos.x - pPlayer->fWidth &&
					g_aBullet[nCntBullet].pos.x <= pPlayer->pos.x + pPlayer->fWidth &&
					g_aBullet[nCntBullet].pos.y >= pPlayer->pos.y - pPlayer->fHeight &&
					g_aBullet[nCntBullet].pos.y <= pPlayer->pos.y)
				{
					DamageHealth(7);
					SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), EXPLOSIONTYPE_NORMAL, 10.0f, 10.0f);
				}
			}
			if (g_aBullet[nCntBullet].type == BULLETTYPE_REFLECTION)
			{//�X�L���R�iX�A���[�j

				SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f), 10.0f, 30, EFFECTSCALE_NORMAL);
				
				if (g_aBullet[nCntBullet].pos.x >= SCREEN_WIDTH || g_aBullet[nCntBullet].pos.x <= 0.0f)
				{
					if (g_aBullet[nCntBullet].bBulletReflectionX == false)
					{
						g_aBullet[nCntBullet].bBulletReflectionX = true;
					}
					else if (g_aBullet[nCntBullet].bBulletReflectionX == true)
					{
						g_aBullet[nCntBullet].bBulletReflectionX = false;
					}
				}
				if (g_aBullet[nCntBullet].pos.y >= SCREEN_HEIGHT || g_aBullet[nCntBullet].pos.y <= 0.0f)
				{
					if (g_aBullet[nCntBullet].bBulletReflectionY == false)
					{
						g_aBullet[nCntBullet].bBulletReflectionY = true;
					}
					else if (g_aBullet[nCntBullet].bBulletReflectionY == true)
					{
						g_aBullet[nCntBullet].bBulletReflectionY = false;
					}
				}



				if (g_aBullet[nCntBullet].bBulletReflectionX == true)
				{
					g_aBullet[nCntBullet].pos.x -= g_aBullet[nCntBullet].move.x;
				}
				if (g_aBullet[nCntBullet].bBulletReflectionX == false)
				{
					g_aBullet[nCntBullet].pos.x += g_aBullet[nCntBullet].move.x;
				}
				if (g_aBullet[nCntBullet].bBulletReflectionY == true)
				{
					g_aBullet[nCntBullet].pos.y -= g_aBullet[nCntBullet].move.y;
				}
				if (g_aBullet[nCntBullet].bBulletReflectionY == false)
				{
					g_aBullet[nCntBullet].pos.y += g_aBullet[nCntBullet].move.y;
				}

			
					g_aBullet[nCntBullet].move += g_aBullet[nCntBullet].move * 0.002f;
				

					//�����蔻��
				if (g_aBullet[nCntBullet].pos.x >= pPlayer->pos.x - 5.0f && g_aBullet[nCntBullet].pos.x <= pPlayer->pos.x + 5.0f &&
					g_aBullet[nCntBullet].pos.y >= pPlayer->pos.y - 5.0f && g_aBullet[nCntBullet].pos.y <= pPlayer->pos.y + 5.0f)
				{//�v���C���[�ɒe����������
					SetExplosion(pPlayer->pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),EXPLOSIONTYPE_NORMAL,10.0f,10.0f);
					//�G�̒e���g�p���Ă��Ȃ���Ԃɂ���
					g_aBullet[nCntBullet].bUse = false;//�e���g�p���Ă��Ȃ���Ԃɂ���
					//UpdateHealth(1.0f);//�ǉ��\��i����j
					HitPlayer(1);
				}
			}
			if (g_aBullet[nCntBullet].type == BULLETTYPE_SKILL1)
			{//�v���C���[�̃X�L���P
				Enemy* pEnemy = GetEnemy();
				for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				{
					if (pEnemy->bUse == true)
					{//�G���g�p����Ă���
						
							if (g_aBullet[nCntBullet].pos.x >= pEnemy->pos.x - pEnemy->fWidth / 2 - BIGBULLET_LV1 && g_aBullet[nCntBullet].pos.x <= pEnemy->pos.x + pEnemy->fWidth / 2 + BIGBULLET_LV1 &&
								g_aBullet[nCntBullet].pos.y >= pEnemy->pos.y - pEnemy->fHeight - BIGBULLET_LV1 && g_aBullet[nCntBullet].pos.y <= pEnemy->pos.y + BIGBULLET_LV1)
							{//�G�ƒe����������
								HitEnemy(nCntEnemy, 40.0f);//�З͂T�O�I�����З͒e

								SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), EXPLOSIONTYPE_NORMAL,10.0f,10.0f);
								//pEnemy->bUse = false;//�G���g�p���Ă��Ȃ���Ԃɂ���
								g_aBullet[nCntBullet].bUse = false;//�e���g�p���Ă��Ȃ���Ԃɂ���

							}
						
					}
				}
			}
			if (g_aBullet[nCntBullet].type == BULLETTYPE_SKILL2_SLASH)
			{//�v���C���[�̃X�L��2
				Enemy* pEnemy = GetEnemy();
				
			
				for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				{
					if (pEnemy->bUse == true)
					{//�G���g�p����Ă���
							if (g_aBullet[nCntBullet].pos.x >= pEnemy->pos.x - pEnemy->fWidth / 2 - BIGBULLET_LV2 && g_aBullet[nCntBullet].pos.x <= pEnemy->pos.x + pEnemy->fWidth / 2 + BIGBULLET_LV2 &&
								g_aBullet[nCntBullet].pos.y >= pEnemy->pos.y - pEnemy->fHeight - BIGBULLET_LV2 && g_aBullet[nCntBullet].pos.y <= pEnemy->pos.y + BIGBULLET_LV2)
							{//�G�ƒe����������
								HitEnemy(nCntEnemy, 2.0f);//�З͂P�O�O�i�����U���j

								SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), EXPLOSIONTYPE_NORMAL,10.0f,10.0f);

								/*	if (g_aBullet[nCntBullet].bUse == false)
									{
										g_aBullet[nCntBullet].fAnimationPattern_Slash = 0.0f;
										g_aBullet[nCntBullet].nAnimationCounter_Slash = 0;
									}*/
									//pEnemy->bUse = false;//�G���g�p���Ă��Ȃ���Ԃɂ���
									//g_aBullet[nCntBullet].bUse = false;//�e���g�p���Ă��Ȃ���Ԃɂ���
									//g_aBullet[nCntBullet].bUse = false;//�e���g�p���Ă��Ȃ���Ԃɂ���

							}
					}
				}
			}
			if (g_aBullet[nCntBullet].type == BULLETTYPE_ARROW)
			{//�X�L���R�iX�A���[�j
				g_aBullet[nCntBullet].nCntTime++;

				if (g_aBullet[nCntBullet].nCntTime == 1)
				{
					PlaySound(SOUND_LABEL_SE_ARROWSHOT_000);
				}
				SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(0.2f, 1.0f, 1.0f, 0.5f),50.0f, 70,EFFECTSCALE_NORMAL);

				if (g_aBullet[nCntBullet].pos.x >= SCREEN_WIDTH || g_aBullet[nCntBullet].pos.x <= 0.0f)
				{

					if (g_aBullet[nCntBullet].bBulletReflectionX == false)
					{
						g_aBullet[nCntBullet].bBulletReflectionX= true;
					}
					else if (g_aBullet[nCntBullet].bBulletReflectionX == true)
					{
						g_aBullet[nCntBullet].bBulletReflectionX = false;
					}
				}
				if (g_aBullet[nCntBullet].pos.y >= SCREEN_HEIGHT || g_aBullet[nCntBullet].pos.y <= 0.0f)
				{
					if (g_aBullet[nCntBullet].bBulletReflectionY == false)
					{
						g_aBullet[nCntBullet].bBulletReflectionY = true;
					}
					else if (g_aBullet[nCntBullet].bBulletReflectionY == true)
					{
						g_aBullet[nCntBullet].bBulletReflectionY = false;
					}
				}



				if (g_aBullet[nCntBullet].bBulletReflectionX == true)
				{
					g_aBullet[nCntBullet].pos.x -= g_aBullet[nCntBullet].move.x;
				}
				if (g_aBullet[nCntBullet].bBulletReflectionX == false)
				{
					g_aBullet[nCntBullet].pos.x += g_aBullet[nCntBullet].move.x;
				}
				if (g_aBullet[nCntBullet].bBulletReflectionY == true)
				{
					g_aBullet[nCntBullet].pos.y -= g_aBullet[nCntBullet].move.y;
				}
				if (g_aBullet[nCntBullet].bBulletReflectionY == false)
				{
					g_aBullet[nCntBullet].pos.y += g_aBullet[nCntBullet].move.y;
				}
				

				Enemy* pEnemy = GetEnemy();
				//for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				//{
				//	if (pEnemy->bUse == true)
				//	{//�G���g�p����Ă���
				//		
						if (g_aBullet[nCntBullet].pos.x >= pPlayer->pos.x - pPlayer->fWidth && g_aBullet[nCntBullet].pos.x <= pPlayer->pos.x + pPlayer->fWidth&&
							g_aBullet[nCntBullet].pos.y >= pPlayer->pos.y - pPlayer->fHeight&& g_aBullet[nCntBullet].pos.y <= pPlayer->pos.y)
						{//�G�ƒe����������
						/*	HitEnemy(nCntEnemy,15.0f);*/
							DamageHealth(15);

							SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), EXPLOSIONTYPE_NORMAL,10.0f,10.0f);
							//pEnemy->bUse = false;//�G���g�p���Ă��Ȃ���Ԃɂ���
							g_aBullet[nCntBullet].bUse = false;//�e���g�p���Ă��Ȃ���Ԃɂ���

						}
				/*	
					}
				}*/
			}
			if (g_aBullet[nCntBullet].type == BULLETTYPE_FINALBALL)
			{//���X�{�X�̕K�E�Z�̒e
				g_aBullet[nCntBullet].nCntTime++;
				if (g_aBullet[nCntBullet].nCntTime == 1)
				{
					PlaySound(SOUND_LABEL_SE_FINALBALL);
				}
				if (g_aBullet[nCntBullet].nCntTime %100 == 0)
				{
					PlaySound(SOUND_LABEL_SE_FINALBALL);
				}
				pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - FINALBALLSIZE, g_aBullet[nCntBullet].pos.y - FINALBALLSIZE, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + FINALBALLSIZE, g_aBullet[nCntBullet].pos.y - FINALBALLSIZE, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - FINALBALLSIZE, g_aBullet[nCntBullet].pos.y + FINALBALLSIZE, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + FINALBALLSIZE, g_aBullet[nCntBullet].pos.y + FINALBALLSIZE, 0.0f);

				if (g_nDelayBullet % 13 == 0)
				{
					SetBullet(g_aBullet[nCntBullet].pos, D3DXVECTOR3(sinf(-D3DX_PI * (0.01f * g_nDelayBullet)) * 0.08f, cosf(-D3DX_PI * (g_nDelayBullet)) * 0.08f, 0.0f),
						GENERALLY_BULLETLIFE, BULLETTYPE_ICICLE, -1, -1, -1, false, MOVETYPE_NORMAL);

					SetBullet(g_aBullet[nCntBullet].pos, D3DXVECTOR3(sinf(-D3DX_PI * (-0.01f * g_nDelayBullet)) * 0.08f, cosf(-D3DX_PI * (0.01f * g_nDelayBullet)) * 0.08f, 0.0f),
						GENERALLY_BULLETLIFE, BULLETTYPE_ICICLE, -1, -1, -1, false, MOVETYPE_NORMAL);
				}

				if (g_aBullet[nCntBullet].nAnimationCounter % 5 == 0)
				{
					g_aBullet[nCntBullet].nAnimationPattern++;

				}
				if (g_aBullet[nCntBullet].nAnimationPattern >= 5)
				{
					g_aBullet[nCntBullet].nAnimationPattern = 0;
				}
				pVtx[0].tex = D3DXVECTOR2(0.0f + 0.2f * g_aBullet[nCntBullet].nAnimationPattern, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.2f + 0.2f * g_aBullet[nCntBullet].nAnimationPattern, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f + 0.2f * g_aBullet[nCntBullet].nAnimationPattern, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.2f + 0.2f * g_aBullet[nCntBullet].nAnimationPattern, 1.0f);
				float fRandumRot = float((rand)() % 100) / 100;

				float fBullet_FinalBoss[20];
				if (g_aBullet[nCntBullet].pos.x >= SCREEN_WIDTH - FINALBALLSIZE || g_aBullet[nCntBullet].pos.x <= 0.0f + FINALBALLSIZE)
				{
					for (int nCnt = 0; nCnt < 20; nCnt++)
					{
						fBullet_FinalBoss[nCnt] = (1.0f / 10) * nCnt;
						SetBullet(g_aBullet[nCntBullet].pos, D3DXVECTOR3(sinf(-D3DX_PI * (fRandumRot + fBullet_FinalBoss[nCnt])) * 5.0f, cosf(-D3DX_PI * (fRandumRot + fBullet_FinalBoss[nCnt])) * 5.0f, 0.0f),
							GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, -1, -1, -1, false, MOVETYPE_NORMAL);
					}
					float fData = 0.0f;
						SetAttack(g_aBullet[nCntBullet].pos,
							D3DXVECTOR3(0.0f, 0.0f, 0.0f), 
							ATTACKTYPE_FINALBEAM,
							-1, -1, -1, MOVETYPE_NORMAL, 50.0f, 1280.0f, 20, D3DX_PI * 1.0f, -fData);

						SetAttack(g_aBullet[nCntBullet].pos,
							D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							ATTACKTYPE_FINALBEAM,
							-1, -1, -1, MOVETYPE_NORMAL, 50.0f, 1280.0f, 20, D3DX_PI * 0.75f, -fData);

						SetAttack(g_aBullet[nCntBullet].pos,
							D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							ATTACKTYPE_FINALBEAM,
							-1, -1, -1, MOVETYPE_NORMAL, 50.0f, 1280.0f, 20, D3DX_PI * 0.5f, -fData);

						SetAttack(g_aBullet[nCntBullet].pos,
							D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							ATTACKTYPE_FINALBEAM,
							-1, -1, -1, MOVETYPE_NORMAL, 50.0f, 1280.0f, 20, D3DX_PI * 0.25f, -fData);

						SetAttack(g_aBullet[nCntBullet].pos,
							D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							ATTACKTYPE_FINALBEAM,
							-1, -1, -1, MOVETYPE_NORMAL, 50.0f, 1280.0f, 20, D3DX_PI * 0.0f, -fData);

						SetAttack(g_aBullet[nCntBullet].pos,
							D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							ATTACKTYPE_FINALBEAM,
							-1, -1, -1, MOVETYPE_NORMAL, 50.0f, 1280.0f, 20, -D3DX_PI * 0.5f, -fData);

						SetAttack(g_aBullet[nCntBullet].pos,
							D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							ATTACKTYPE_FINALBEAM,
							-1, -1, -1, MOVETYPE_NORMAL, 50.0f, 1280.0f, 20, -D3DX_PI * 0.75f, -fData);

						SetAttack(g_aBullet[nCntBullet].pos,
							D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							ATTACKTYPE_FINALBEAM,
							-1, -1, -1, MOVETYPE_NORMAL, 50.0f, 1280.0f, 20, -D3DX_PI * 0.25f, -fData);

					if (g_aBullet[nCntBullet].bBulletReflectionX == false)
					{
						g_aBullet[nCntBullet].bBulletReflectionX = true;
					}
					else if (g_aBullet[nCntBullet].bBulletReflectionX == true)
					{
						g_aBullet[nCntBullet].bBulletReflectionX = false;
					}
				}
				if (g_aBullet[nCntBullet].pos.y >= SCREEN_HEIGHT - FINALBALLSIZE || g_aBullet[nCntBullet].pos.y <= 0.0f + FINALBALLSIZE)
				{
					float fData = 0.0f;

					for (int nCnt = 0; nCnt < 20; nCnt++)
					{
						fBullet_FinalBoss[nCnt] = (1.0f / 10) * nCnt;
						SetBullet(g_aBullet[nCntBullet].pos, D3DXVECTOR3(sinf(-D3DX_PI * (fRandumRot + fBullet_FinalBoss[nCnt])) * 5.0f, cosf(-D3DX_PI * (fRandumRot + fBullet_FinalBoss[nCnt])) * 5.0f, 0.0f),
							GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, -1, -1, -1, false, MOVETYPE_NORMAL);
					}
				
					SetAttack(g_aBullet[nCntBullet].pos,
						D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						ATTACKTYPE_FINALBEAM,
						-1, -1, -1, MOVETYPE_NORMAL, 50.0f, 1280.0f, 20, D3DX_PI * 1.0f, -fData);

					SetAttack(g_aBullet[nCntBullet].pos,
						D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						ATTACKTYPE_FINALBEAM,
						-1, -1, -1, MOVETYPE_NORMAL, 50.0f, 1280.0f, 20, D3DX_PI * 0.75f, -fData);

					SetAttack(g_aBullet[nCntBullet].pos,
						D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						ATTACKTYPE_FINALBEAM,
						-1, -1, -1, MOVETYPE_NORMAL, 50.0f, 1280.0f, 20, D3DX_PI * 0.5f, -fData);

					SetAttack(g_aBullet[nCntBullet].pos,
						D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						ATTACKTYPE_FINALBEAM,
						-1, -1, -1, MOVETYPE_NORMAL, 50.0f, 1280.0f, 20, D3DX_PI * 0.25f, -fData);

					SetAttack(g_aBullet[nCntBullet].pos,
						D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						ATTACKTYPE_FINALBEAM,
						-1, -1, -1, MOVETYPE_NORMAL, 50.0f, 1280.0f, 20, D3DX_PI * 0.0f, -fData);

					SetAttack(g_aBullet[nCntBullet].pos,
						D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						ATTACKTYPE_FINALBEAM,
						-1, -1, -1, MOVETYPE_NORMAL, 50.0f, 1280.0f, 20, -D3DX_PI * 0.5f, -fData);

					SetAttack(g_aBullet[nCntBullet].pos,
						D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						ATTACKTYPE_FINALBEAM,
						-1, -1, -1, MOVETYPE_NORMAL, 50.0f, 1280.0f, 20, -D3DX_PI * 0.75f, -fData);

					SetAttack(g_aBullet[nCntBullet].pos,
						D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						ATTACKTYPE_FINALBEAM,
						-1, -1, -1, MOVETYPE_NORMAL, 50.0f, 1280.0f, 20, -D3DX_PI * 0.25f, -fData);

					if (g_aBullet[nCntBullet].bBulletReflectionY == false)
					{
						g_aBullet[nCntBullet].bBulletReflectionY = true;
					}
					else if (g_aBullet[nCntBullet].bBulletReflectionY == true)
					{
						g_aBullet[nCntBullet].bBulletReflectionY = false;
					}
				}



				if (g_aBullet[nCntBullet].bBulletReflectionX == true)
				{
					g_aBullet[nCntBullet].pos.x -= g_aBullet[nCntBullet].move.x;
				}
				if (g_aBullet[nCntBullet].bBulletReflectionX == false)
				{
					g_aBullet[nCntBullet].pos.x += g_aBullet[nCntBullet].move.x;
				}
				if (g_aBullet[nCntBullet].bBulletReflectionY == true)
				{
					g_aBullet[nCntBullet].pos.y -= g_aBullet[nCntBullet].move.y;
				}
				if (g_aBullet[nCntBullet].bBulletReflectionY == false)
				{
					g_aBullet[nCntBullet].pos.y += g_aBullet[nCntBullet].move.y;
				}


				Enemy* pEnemy = GetEnemy();
				//for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				//{
				//	if (pEnemy->bUse == true)
				//	{//�G���g�p����Ă���
				//		
				if (pPlayer->pos.x >= g_aBullet[nCntBullet].pos.x - FINALBALLSIZE - pPlayer->fWidth && pPlayer->pos.x <= g_aBullet[nCntBullet].pos.x + FINALBALLSIZE + pPlayer->fWidth &&
					pPlayer->pos.y >= g_aBullet[nCntBullet].pos.y - FINALBALLSIZE - pPlayer->fHeight && pPlayer->pos.y <= g_aBullet[nCntBullet].pos.y + FINALBALLSIZE)
				{//�G�ƒe����������
				/*	HitEnemy(nCntEnemy,15.0f);*/
					DamageHealth(10);

					SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), EXPLOSIONTYPE_NORMAL, 10.0f, 10.0f);
					//pEnemy->bUse = false;//�G���g�p���Ă��Ȃ���Ԃɂ���
					//g_aBullet[nCntBullet].bUse = false;//�e���g�p���Ă��Ȃ���Ԃɂ���

				}
				/*
					}
				}*/
			}
			if (g_aBullet[nCntBullet].type == BULLETTYPE_NOVA)
			{//�v���C���[�̃X�L��5

				g_aBullet[nCntBullet].nCntTime++;
				if (g_aBullet[nCntBullet].nCntTime == 1)
				{
					PlaySound(SOUND_LABEL_SE_FINALBALL);
				}
				Enemy* pEnemy = GetEnemy();
				float fRandumColorR;
				float fRandumColorG;
				float fRandumColorB;
				float fRandumColorA;
				fRandumColorR = float((rand)() % 100) / 10.0f;
				fRandumColorG = float((rand)() % 100) / 10.0f;
				fRandumColorB = float((rand)() % 100) / 10.0f;
			/*	fRandumSpeed = float((rand)() % 100) / 5.0f;*/
				SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(fRandumColorR,fRandumColorG,fRandumColorB, 0.5f), 50.0f, 300, EFFECTSCALE_NORMAL);
				//for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				//{
				//	if (pEnemy->bUse == true)
				//	{//�G���g�p����Ă���
				//		
							if (g_aBullet[nCntBullet].pos.x >= pPlayer->pos.x - pPlayer->fWidth - 120.0f && g_aBullet[nCntBullet].pos.x <= pPlayer->pos.x + pPlayer->fWidth + 120.0f &&
								g_aBullet[nCntBullet].pos.y >= pPlayer->pos.y - pPlayer->fHeight && g_aBullet[nCntBullet].pos.y <= pPlayer->pos.y + 120.0f)
							{//�G�ƒe����������
								//HitEnemy(nCntEnemy,100.0f);//�З͂P�O�O�i�����U���j

								DamageHealth(100);

								SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), EXPLOSIONTYPE_NOVA,400.0f,400.0f);
								g_aBullet[nCntBullet].bUse = false;//�e���g�p���Ă��Ȃ���Ԃɂ���
							}
						
				/*	}
				}*/
			}
			if (g_aBullet[nCntBullet].type == BULLETTYPE_SEED)
			{
				g_aBullet[nCntBullet].nCntTime++;
				if (g_aBullet[nCntBullet].nCntTime == 1)
				{
					PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_006);
				}

			
				float fVector[12];

				float fRandumRot;
				float fRandumSpeed;
				fRandumRot = float((rand)() % 200) / 100.0f;
				fRandumSpeed = float((rand)() % 100) / 5.0f;
				if (g_aBullet[nCntBullet].nLife == 0)
				{
					for (int nCntCircleBullet = 0; nCntCircleBullet < 12; nCntCircleBullet++)
					{
						fVector[nCntCircleBullet] = (2.0f / 12.0f) * nCntCircleBullet;
                        
						SetBullet(g_aBullet[nCntBullet].pos, D3DXVECTOR3(sinf(-D3DX_PI * (fVector[nCntCircleBullet] + fRandumRot)) * 10.0f,
							cosf(-D3DX_PI * (fVector[nCntCircleBullet] + fRandumRot)) * 10.0f, 0.0f),
							200, BULLETTYPE_ENEMY, -1, -1, -1, false, MOVETYPE_GRAVITY);
					}
				}
			}
			if (g_aBullet[nCntBullet].type == BULLETTYPE_JUMPING)
			{
				g_aBullet[nCntBullet].nCntTime++;
				if (g_aBullet[nCntBullet].nCntTime == 1)
				{
					PlaySound(SOUND_LABEL_SE_SHOT);
				}
				float fVector[12];

				float fRandumRot;
				float fRandumSpeed;
 				fRandumRot = float((rand)() % 200) / 100.0f;
				fRandumSpeed = float((rand)() % 100) / 5.0f;
				SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 10.0f, 30, EFFECTSCALE_NORMAL);
				if (g_aBullet[nCntBullet].pos.x >= pPlayer->pos.x - pPlayer->fWidth && g_aBullet[nCntBullet].pos.x <= pPlayer->pos.x + pPlayer->fWidth &&
					g_aBullet[nCntBullet].pos.y >= pPlayer->pos.y - pPlayer->fHeight && g_aBullet[nCntBullet].pos.y <= pPlayer->pos.y)
				{//�v���C���[�ɒe����������

				/*	PlaySound(SOUND_LABEL_SE_DAMAGE_000);*/

					SetExplosion(pPlayer->pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), EXPLOSIONTYPE_NORMAL, 10.0f, 10.0f);
					//�G�̒e���g�p���Ă��Ȃ���Ԃɂ���
					g_aBullet[nCntBullet].bUse = false;//�e���g�p���Ă��Ȃ���Ԃɂ���
					DamageHealth(5);
				}

				if (g_aBullet[nCntBullet].pos.y >= 665.0f)
				{
					g_aBullet[nCntBullet].move.y = -15.0f;
				}
			}
			if (g_aBullet[nCntBullet].type == BULLETTYPE_ICICLE)
			{
				g_aBullet[nCntBullet].nCntTime++;
				if (g_aBullet[nCntBullet].nCntTime == 1)
				{
					PlaySound(SOUND_LABEL_SE_JUMPINGBULLET);
				}
				/*float fVector[12];*/

				float fRandumRot;
				float fRandumSpeed;
				fRandumRot = float((rand)() % 200) / 100.0f;
				fRandumSpeed = float((rand)() % 100) / 5.0f;

				SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(0.5f,0.5f,0.0f, 0.5f), 15.0f, 300, EFFECTSCALE_NORMAL);

				pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - 10.0f, g_aBullet[nCntBullet].pos.y - 10.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + 10.0f, g_aBullet[nCntBullet].pos.y - 10.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - 10.0f, g_aBullet[nCntBullet].pos.y + 10.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + 10.0f, g_aBullet[nCntBullet].pos.y + 10.0f, 0.0f);

				if (g_aBullet[nCntBullet].pos.x >= pPlayer->pos.x - pPlayer->fWidth - 10.0f &&
					g_aBullet[nCntBullet].pos.x <= pPlayer->pos.x + pPlayer->fWidth + 10.0f &&
					g_aBullet[nCntBullet].pos.y >= pPlayer->pos.y - pPlayer->fHeight - 10.0f &&
					g_aBullet[nCntBullet].pos.y <= pPlayer->pos.y + 10.0f)
				{
					DamageHealth(7);
					SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), EXPLOSIONTYPE_NORMAL, 10.0f, 10.0f);
					g_aBullet[nCntBullet].bUse = false;
				}
				//��������������e
				g_aBullet[nCntBullet].move += g_aBullet[nCntBullet].move / 50;

				
			}
			if (g_aBullet[nCntBullet].type == BULLETTYPE_SPLET)
			{
				g_aBullet[nCntBullet].nCntTime++;
				if (g_aBullet[nCntBullet].nCntTime == 1)
				{
					PlaySound(SOUND_LABEL_SE_SPLETBULLET);
				}
				/*float fVector[12];*/

				float fRandumRot;
				float fRandumSpeed;
			

				pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - 30.0f, g_aBullet[nCntBullet].pos.y - 30.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + 30.0f, g_aBullet[nCntBullet].pos.y - 30.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - 30.0f, g_aBullet[nCntBullet].pos.y + 30.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + 30.0f, g_aBullet[nCntBullet].pos.y + 30.0f, 0.0f);

				if (g_aBullet[nCntBullet].pos.x >= pPlayer->pos.x - pPlayer->fWidth - 30.0f &&
					g_aBullet[nCntBullet].pos.x <= pPlayer->pos.x + pPlayer->fWidth + 30.0f &&
					g_aBullet[nCntBullet].pos.y >= pPlayer->pos.y - pPlayer->fHeight - 30.0f &&
					g_aBullet[nCntBullet].pos.y <= pPlayer->pos.y + 30.0f)
				{
					DamageHealth(24);
					SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), EXPLOSIONTYPE_NORMAL, 10.0f, 10.0f);
					g_aBullet[nCntBullet].bUse = false;

					
				}
			
				if (g_aBullet[nCntBullet].pos.x >= SCREEN_WIDTH - 20.0f||
					g_aBullet[nCntBullet].pos.x <= 0.0f + 20.0f||
					g_aBullet[nCntBullet].pos.y >= SCREEN_HEIGHT - 20.0f ||
					g_aBullet[nCntBullet].pos.y <= 0.0f + 20.0f)
				{
					SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), EXPLOSIONTYPE_NORMAL, 10.0f, 10.0f);
					g_aBullet[nCntBullet].bUse = false;


				
					for (int nCnt = 0; nCnt < 15; nCnt++)
					{
						fRandumRot = float((rand)() % 200) / 100.0f;
						fRandumSpeed = float((rand)() % 100) / 20.0f;

						SetBullet(g_aBullet[nCntBullet].pos, D3DXVECTOR3(sinf(-D3DX_PI * fRandumRot) * fRandumSpeed, cosf(-D3DX_PI * fRandumRot) * fRandumSpeed, 0.0f),
							600, BULLETTYPE_ENEMY, -1, -1, -1, false, MOVETYPE_NORMAL);
					}
				}

			}
			if (g_aBullet[nCntBullet].type == BULLETTYPE_EXPLOSION)
			{
				g_aBullet[nCntBullet].nCntTime++;
				if (g_aBullet[nCntBullet].nCntTime == 1)
				{
					PlaySound(SOUND_LABEL_SE_FINALBALL);
				}
				/*float fVector[12];*/

				float fRandumRot;
				float fRandumSpeed;


				pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - 30.0f, g_aBullet[nCntBullet].pos.y - 30.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + 30.0f, g_aBullet[nCntBullet].pos.y - 30.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - 30.0f, g_aBullet[nCntBullet].pos.y + 30.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + 30.0f, g_aBullet[nCntBullet].pos.y + 30.0f, 0.0f);

				/*if (g_aBullet[nCntBullet].pos.x >= pPlayer->pos.x - pPlayer->fWidth - 30.0f &&
					g_aBullet[nCntBullet].pos.x <= pPlayer->pos.x + pPlayer->fWidth + 30.0f &&
					g_aBullet[nCntBullet].pos.y >= pPlayer->pos.y - pPlayer->fHeight - 30.0f &&
					g_aBullet[nCntBullet].pos.y <= pPlayer->pos.y + 30.0f)
				{
					DamageHealth(40);
					SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), EXPLOSIONTYPE_NORMAL, 10.0f, 10.0f);
					g_aBullet[nCntBullet].bUse = false;


				}*/


				if (g_nDelayBullet % 10 == 0)
				{
					SetStaging(STAGINGTYPE_EXPLOSIONWARNING, -1, g_aBullet->pos, 60, ATTACKTYPE_PUNCH, -1.0f, 50.0f, -1.0f, -1.0f, -1.0f,- 1.0f);
				}

				if (g_aBullet[nCntBullet].pos.x >= SCREEN_WIDTH - 20.0f ||
					g_aBullet[nCntBullet].pos.x <= 0.0f + 20.0f ||
					g_aBullet[nCntBullet].pos.y >= SCREEN_HEIGHT - 20.0f ||
					g_aBullet[nCntBullet].pos.y <= 0.0f + 20.0f)
				{
					SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), EXPLOSIONTYPE_NORMAL, 10.0f, 10.0f);
					g_aBullet[nCntBullet].bUse = false;



					/*for (int nCnt = 0; nCnt < 15; nCnt++)
					{
						fRandumRot = float((rand)() % 200) / 100.0f;
						fRandumSpeed = float((rand)() % 100) / 20.0f;

						SetBullet(g_aBullet[nCntBullet].pos, D3DXVECTOR3(sinf(-D3DX_PI * fRandumRot) * fRandumSpeed, cosf(-D3DX_PI * fRandumRot) * fRandumSpeed, 0.0f),
							600, BULLETTYPE_ENEMY, -1, -1, -1, false, MOVETYPE_NORMAL);
					}*/
				}

			}
			if (g_aBullet[nCntBullet].type == BULLETTYPE_HORMING)
			{//�z�[�~���O�e

				g_aBullet[nCntBullet].nHormingCount++;

				SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(0.2f, 1.0f, 1.0f, 0.5f), 20.0f, 30,EFFECTSCALE_NORMAL);

				//g_aBullet[nCntBullet].fRotMove = atan2f(g_aBullet[nCntBullet].move.y, g_aBullet[nCntBullet].move.x);//���݂̈ړ������i�p�x�j
				//g_aBullet[nCntBullet].fRotDest = atan2f(pEnemy->pos.y -g_aBullet[nCntBullet].pos.y, pEnemy->pos.x - g_aBullet[nCntBullet]. pos.x);//�v���C���[����G�ւ̌���//�@�@�@�@�@����������NovaBrast��⁄��������
				//g_aBullet[nCntBullet].fRotDiff = g_aBullet[nCntBullet].fRotDest - g_aBullet[nCntBullet].fRotMove;//�ړI�n�܂ł̈ړ������̍���

				//g_aBullet[nCntBullet].fRotMove += g_aBullet[nCntBullet].fRotDiff* 5.0f;//�ړ������i�p�x�␳�j
				//g_aBullet[nCntBullet].move.x = sinf(g_aBullet[nCntBullet].fRotMove) * g_aBullet[nCntBullet].fRotMove;
				//g_aBullet[nCntBullet].move.y = cosf(g_aBullet[nCntBullet].fRotMove) * g_aBullet[nCntBullet].fRotMove;
			
				Enemy* pEnemy = GetEnemy();
				

			/*	if (g_aBullet[nCntBullet].nHormingCount >=50)
				{
				g_aBullet[nCntBullet].HormingFlag =true;
				}*/
				if (g_aBullet[nCntBullet].HormingFlag == false)
				{
					for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
					{
						if (pEnemy->bUse == false && g_aBullet[nCntBullet].nHormingaim == nCntEnemy)
						{
							g_aBullet[nCntBullet].bUse = false;
							SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), EXPLOSIONTYPE_NORMAL,10.0f,10.0f);
						}
						if (pEnemy->bUse == true && g_aBullet[nCntBullet].nHormingaim == nCntEnemy)
						{
							g_aBullet[nCntBullet].fRotMove = atan2f(cosf(D3DX_PI * (0.2f * g_aBullet[nCntBullet].nNumber)) * 100.0f, sinf(D3DX_PI * (0.2f * g_aBullet[nCntBullet].nNumber)) * 100.0f);//���݂̈ړ������i�p�x]]�j
							g_aBullet[nCntBullet].fRotDest = atan2f(pEnemy->pos.y - g_aBullet[nCntBullet].pos.y, pEnemy->pos.x - g_aBullet[nCntBullet].pos.x);//�v���C���[����G�ւ̌���

						}

					}



					//g_aBullet[nCntBullet].fRotMove = atan2f(sinf(D3DX_PI * (0.1 * g_aBullet[nCntBullet].nNumber)) * 1.0f, cosf(D3DX_PI * (0.1f * g_aBullet[nCntBullet].nNumber)) * 1.0f);//���݂̈ړ������i�p�x�j
					g_aBullet[nCntBullet].fRotDiff = g_aBullet[nCntBullet].fRotDest - g_aBullet[nCntBullet].fRotMove;//�ړI�n�܂ł̈ړ������̍���

					g_aBullet[nCntBullet].fRotDiff = -g_aBullet[nCntBullet].fRotDest - g_aBullet[nCntBullet].fRotMove + 6.28f;//�p�x�����̒���

					g_aBullet[nCntBullet].fRotMove += g_aBullet[nCntBullet].fRotDiff * 10.0f;//�ړ������i�p�x�␳�j

					g_aBullet[nCntBullet].fRotMove += g_aBullet[nCntBullet].fRotDiff - 6.28f;//move�̊p�x�C��

					g_aBullet[nCntBullet].move.x = sinf(g_aBullet[nCntBullet].fRotMove) * 5.0f;
					g_aBullet[nCntBullet].move.y = cosf(g_aBullet[nCntBullet].fRotMove) * 5.0f;
				}
				
				/*	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
					{
						if (pEnemy->bUse == true)
						{*/
						/*	g_aBullet[nCntBullet].pos.y += g_aBullet[nCntBullet].move.y;
							g_aBullet[nCntBullet].pos.x += g_aBullet[nCntBullet].move.x;*/
					/*	}
					}*/
				g_aBullet[nCntBullet].fRotMove = atan2f(sinf(D3DX_PI*(0.1*g_aBullet[nCntBullet].nNumber))*1.0f,cosf(D3DX_PI*(0.1f*g_aBullet[nCntBullet].nNumber))*1.0f);//���݂̈ړ������i�p�x�j
				
				g_aBullet[nCntBullet].fRotDest = atan2f(pEnemy->pos.y - g_aBullet[nCntBullet].pos.y, pEnemy->pos.x - g_aBullet[nCntBullet].pos.x);//�v���C���[����G�ւ̌���
				
				g_aBullet[nCntBullet].fRotDiff = g_aBullet[nCntBullet].fRotDest - g_aBullet[nCntBullet].fRotMove;//�ړI�n�܂ł̈ړ������̍���

				g_aBullet[nCntBullet].fRotDiff = -g_aBullet[nCntBullet].fRotDest - g_aBullet[nCntBullet].fRotMove+6.28f;//�p�x�����̒���

				g_aBullet[nCntBullet].fRotMove += g_aBullet[nCntBullet].fRotDiff * 10.0f;//�ړ������i�p�x�␳�j

				g_aBullet[nCntBullet].fRotMove += g_aBullet[nCntBullet].fRotDiff-6.28f;//move�̊p�x�C��

				g_aBullet[nCntBullet].move.x = sinf(g_aBullet[nCntBullet].fRotMove) *10.0f;
				g_aBullet[nCntBullet].move.y = cosf(g_aBullet[nCntBullet].fRotMove) *10.0f;

				if (pEnemy->bUse == false)
				{
					pEnemy++;
				}
			/*	Enemy* pEnemy = GetEnemy();*/
/*
				/*/
				if (g_aBullet[nCntBullet].bUse == true)
				{
					Enemy* pEnemy = GetEnemy();
					for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
					{

						if (pEnemy->bUse == true)
						{//�G���g�p����Ă���
							if (g_aBullet[nCntBullet].pos.x >= pEnemy->pos.x - pEnemy->fWidth / 2 - 20.0f && g_aBullet[nCntBullet].pos.x <= pEnemy->pos.x + pEnemy->fWidth / 2 + 20.0f &&
								g_aBullet[nCntBullet].pos.y >= pEnemy->pos.y - pEnemy->fHeight - 20.0f && g_aBullet[nCntBullet].pos.y <= pEnemy->pos.y + 20.0f)
							{//�G�ƒe����������
								HitEnemy(nCntEnemy, 1.0f);

								SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), EXPLOSIONTYPE_NORMAL,10.0f,10.0f);
								//pEnemy->bUse = false;//�G���g�p���Ă��Ȃ���Ԃɂ���
								g_aBullet[nCntBullet].bUse = false;//�e���g�p���Ă��Ȃ���Ԃɂ���

							}
						}
					
					}
				}
			}
			//���_���W�̐ݒ�(�e�̑傫���j
			if (g_aBullet[nCntBullet].moveType == MOVETYPE_NORMAL)
			{
				if (g_aBullet[nCntBullet].type != BULLETTYPE_ARROW)
				{
					if (g_aBullet[nCntBullet].type != BULLETTYPE_FINALBALL)
					{
						if (g_aBullet[nCntBullet].type >= BULLETTYPE_PLAYER || g_aBullet[nCntBullet].type <= BULLETTYPE_BANDY)
						{
							g_aBullet[nCntBullet].pos.y += g_aBullet[nCntBullet].move.y;
							g_aBullet[nCntBullet].pos.x += g_aBullet[nCntBullet].move.x;
						}
						if (g_aBullet[nCntBullet].type == BULLETTYPE_HORMING || g_aBullet[nCntBullet].type == BULLETTYPE_SNOW ||
							g_aBullet[nCntBullet].type == BULLETTYPE_ICICLE ||
							g_aBullet[nCntBullet].type == BULLETTYPE_SPLET ||
							g_aBullet[nCntBullet].type == BULLETTYPE_EXPLOSION)
						{
							g_aBullet[nCntBullet].pos.y += g_aBullet[nCntBullet].move.y;
							g_aBullet[nCntBullet].pos.x += g_aBullet[nCntBullet].move.x;
						}
						if (g_aBullet[nCntBullet].type == BULLETTYPE_SCATTERING || g_aBullet[nCntBullet].type == BULLETTYPE_ROCKON || g_aBullet[nCntBullet].type == BULLETTYPE_SATELLITE)
						{
							g_aBullet[nCntBullet].pos.y += g_aBullet[nCntBullet].move.y;
							g_aBullet[nCntBullet].pos.x += g_aBullet[nCntBullet].move.x;
						}
						if (g_aBullet[nCntBullet].type == BULLETTYPE_REVERSE_CURVE || g_aBullet[nCntBullet].type == BULLETTYPE_LEFT_WIND || g_aBullet[nCntBullet].type == BULLETTYPE_RIGHT_WIND || g_aBullet[nCntBullet].type == BULLETTYPE_NOVA
							|| g_aBullet[nCntBullet].type == BULLETTYPE_DOWNWIND || g_aBullet[nCntBullet].type == BULLETTYPE_UPWIND)
						{
							g_aBullet[nCntBullet].pos.y += g_aBullet[nCntBullet].move.y;
							g_aBullet[nCntBullet].pos.x += g_aBullet[nCntBullet].move.x;
						}
					}
				}
			}
			else if (g_aBullet[nCntBullet].moveType == MOVETYPE_GRAVITY)
			{
				g_aBullet[nCntBullet].move.y += 0.5f;
				g_aBullet[nCntBullet].pos.y += g_aBullet[nCntBullet].move.y;
				g_aBullet[nCntBullet].pos.x += g_aBullet[nCntBullet].move.x;
			}

			if (g_aBullet[nCntBullet].nLife <= 0)
			{//�\�����Ԃ��O�ȉ��ɂȂ�����i�e�̎������s�����j(��ނ͑S�āj
				/*g_aBullet[nCntBullet].bUse = false;*/
				//�����̐ݒ�
				SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), EXPLOSIONTYPE_NORMAL, 10.0f, 10.0f);

				g_aBullet[nCntBullet].bUse = false;//�g�p���Ă��Ȃ���Ԃɂ���
			}

			if (g_aBullet[nCntBullet].type == BULLETTYPE_SNOW)
			{
				if (g_aBullet[nCntBullet].pos.x >= pPlayer->pos.x - pPlayer->fWidth - 10.0f &&
					g_aBullet[nCntBullet].pos.x <= pPlayer->pos.x + pPlayer->fWidth + 10.0f &&
					g_aBullet[nCntBullet].pos.y >= pPlayer->pos.y - pPlayer->fHeight - 10.0f &&
					g_aBullet[nCntBullet].pos.y <= pPlayer->pos.y + 10.0f)
				{
					g_aBullet[nCntBullet].bUse = false;
					DamageHealth(4);
				}
				if (g_aBullet[nCntBullet].bUse == false)
				{
					SetAttack(g_aBullet[nCntBullet].pos,
						D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						ATTACKTYPE_ICE_EXPLOSION,
						-1, nCntBullet, -1, MOVETYPE_NORMAL, 150.0f, 150.0f, 28, 0.0f, -1);
				}
			}
		
			//============================================================================================================================================
		}
		

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBullet->Unlock();
}

//�e�̕`�揈��
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

	int nCntBullet;

	Enemy* pEnemy = GetEnemy();
	Player* pPlayer = GetPlayer();

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_2D));//���_�o�b�t�@�ւ̃|�C���^�ƒ��_���̍\���̂̃T�C�Y

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{
			if (g_aBullet[nCntBullet].type == BULLETTYPE_PLAYER || g_aBullet[nCntBullet].type == BULLETTYPE_ROT)
			{
				pDevice->SetTexture(0, g_pTextureBullet[0]);
			}
			else if (g_aBullet[nCntBullet].type == BULLETTYPE_ENEMY || g_aBullet[nCntBullet].type == BULLETTYPE_REAFY||g_aBullet[nCntBullet].type== BULLETTYPE_ENEMY9
				|| g_aBullet[nCntBullet].type == BULLETTYPE_SATELLITE)
			{
				pDevice->SetTexture(0, g_pTextureBullet[1]);
			}

			else if (g_aBullet[nCntBullet].type == BULLETTYPE_SKILL1)
			{
				pDevice->SetTexture(0, g_pTextureBullet[2]);
			}
			else if (g_aBullet[nCntBullet].type == BULLETTYPE_SKILL2_SLASH)
			{
				pDevice->SetTexture(0, g_pTextureBullet[3]);
			}
			else if (g_aBullet[nCntBullet].type == BULLETTYPE_BOLBY)
			{
				pDevice->SetTexture(0, g_pTextureBullet[4]);
			}
			else if (g_aBullet[nCntBullet].type == BULLETTYPE_BANDY)
			{
				pDevice->SetTexture(0, g_pTextureBullet[5]);
			}
			else if (g_aBullet[nCntBullet].type == BULLETTYPE_ARROW)
			{
				pDevice->SetTexture(0, g_pTextureBullet[6]);
			}
			else if (g_aBullet[nCntBullet].type == BULLETTYPE_HORMING)
			{
				pDevice->SetTexture(0, g_pTextureBullet[7]);
			}
			else if (g_aBullet[nCntBullet].type == BULLETTYPE_SCATTERING)
			{
				pDevice->SetTexture(0, g_pTextureBullet[8]);
			}
			else if (g_aBullet[nCntBullet].type == BULLETTYPE_REVERSE_CURVE)
			{
				pDevice->SetTexture(0, g_pTextureBullet[9]);
			}
			else if (g_aBullet[nCntBullet].type == BULLETTYPE_RIGHT_WIND||g_aBullet[nCntBullet].type==BULLETTYPE_LEFT_WIND
				||g_aBullet[nCntBullet].type==BULLETTYPE_DOWNWIND||g_aBullet[nCntBullet].type==BULLETTYPE_UPWIND)
			{
				pDevice->SetTexture(0, g_pTextureBullet[10]);
			}
			else if (g_aBullet[nCntBullet].type == BULLETTYPE_REFLECTION)
			{
				pDevice->SetTexture(0, g_pTextureBullet[11]);
			}
			else if (g_aBullet[nCntBullet].type == BULLETTYPE_NOVA)
			{
				pDevice->SetTexture(0, g_pTextureBullet[12]);
			}
			else if (g_aBullet[nCntBullet].type == BULLETTYPE_ROCKON)
			{
				pDevice->SetTexture(0, g_pTextureBullet[13]);
			}
			else if (g_aBullet[nCntBullet].type == BULLETTYPE_SEED)
			{
				pDevice->SetTexture(0, g_pTextureBullet[14]);
			}
			else if (g_aBullet[nCntBullet].type == BULLETTYPE_SNOW)
			{
				pDevice->SetTexture(0, g_pTextureBullet[15]);
			}
			else if (g_aBullet[nCntBullet].type == BULLETTYPE_ICICLE)
			{
				pDevice->SetTexture(0, g_pTextureBullet[16]);
			}
			else if (g_aBullet[nCntBullet].type == BULLETTYPE_SPLET)
			{
				pDevice->SetTexture(0, g_pTextureBullet[17]);
			}
			else if (g_aBullet[nCntBullet].type == BULLETTYPE_JUMPING)
			{
				pDevice->SetTexture(0, g_pTextureBullet[18]);
			}
			else if (g_aBullet[nCntBullet].type == BULLETTYPE_EXPLOSION)
			{
				pDevice->SetTexture(0, g_pTextureBullet[19]);
			}
			else if (g_aBullet[nCntBullet].type == BULLETTYPE_FINALBALL)
			{
				pDevice->SetTexture(0, g_pTextureBullet[20]);
			}
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,//0�́A�`�悷��ŏ��̒��_�C���f�b�N�X�ŁA2�́A�`�悷��v���~�e�B�u��
				nCntBullet * 4,
				2);
		}

	}


}

//�e�̐ݒ菈��
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, BULLETTYPE type, int nNum, int nNumEnemy, int nAimingEnemy, bool bCollision
	, MOVETYPE moveType)//pos�́u���ˈʒu�v�Amove�́u�ړ��ʁv,damage�́u�e�̍U���́vnNumEney�́A���Ԗڂ̓G���U�����Ă��邩?
{
	int nCntBullet;

	VERTEX_2D* pVtx;
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet->nCntBullet_2 = nCntBullet;

		if (g_aBullet[nCntBullet].bUse == false)
		{//�e���g�p����Ă��Ȃ�
		/*	if (BULLETTYPE_ENEMY || BULLETTYPE_PLAYER || BULLETTYPE_SKILL1||BULLETTYPE_REAFY)
			{*/

			g_aBullet[nCntBullet].pos = pos;

			//���_���W���X�V
		/*	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(15.0f, 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, -15.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(15.0f, 15.0f, 0.0f);*/

		
				g_aBullet[nCntBullet].move = move;
				g_aBullet[nCntBullet].nLife = nLife;
				g_aBullet[nCntBullet].bUse = true;//�g�p���Ă����Ԃɂ���
				g_aBullet[nCntBullet].type = type;//�e�̎�ށitype)��������B
				g_aBullet[nCntBullet].moveType = moveType;
				g_aBullet[nCntBullet].fCurveBulletX = 0.0f;
				g_aBullet[nCntBullet].fCurveBulletY = 0.0f;
				g_aBullet[nCntBullet].nAnimationCounter_Slash = 0;
				g_aBullet[nCntBullet].fAnimationPattern_Slash = 0.0f;
				g_aBullet[nCntBullet].fBulletMoveX_Enemy9 = 0.0f;
				g_aBullet[nCntBullet].fBulletMoveY_Enemy9 = 0.0f;
				g_aBullet[nCntBullet].fBulletMove_Enemy9 = 0.0f;
				g_aBullet[nCntBullet].nApperCount = 0;//�G���o��������J�E���g���J�n����B
				g_aBullet[nCntBullet].fEnemy5Curve = 0.0f;
				g_aBullet[nCntBullet].nNumber = nNum;
				g_aBullet[nCntBullet].nNumEnemy = nNumEnemy;//�ǂ̓G�����˂��Ă���e���𔻕ʂ���ϐ��B
				g_aBullet[nCntBullet].nHormingaim = nAimingEnemy;
				g_aBullet[nCntBullet].HormingFlag = false;
				g_aBullet[nCntBullet].nHormingCount=0;
				g_aBullet[nCntBullet].fSpeed = 0.0f;//�o���b�g�̃X�s�[�h��ς���B
				g_aBullet[nCntBullet].fCurve = 300.0f;//�o���b�g�̃J�[�u�l��ς���B
				g_aBullet[nCntBullet].bCollisionBullet = bCollision;//�e���u���b�N�ɏՓ˂��邩�ǂ����̃t���O
				g_aBullet[nCntBullet].nAnimationCounter = 0;
				g_aBullet[nCntBullet].nAnimationPattern = 0;
				g_aBullet[nCntBullet].nCntTime = 0;

				break;
			//}
		}
		pVtx += 4;//�|�C���^���S���i�߂�
	}
	g_pVtxBuffBullet->Unlock();
}

//=====================================================
//�e�\���̂̏����擾
//=====================================================
Bullet* GetBullet(void)
{
	return &g_aBullet[0];//�\���̂̐擪�A�h���X��Ԃ��B
}