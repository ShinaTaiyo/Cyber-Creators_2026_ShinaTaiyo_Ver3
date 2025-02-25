//========================================================
//
//�P�O���Q�S���F�{�X�Q�̕K�E�Z�����[staging.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#include "main.h"
#include "bullet.h"
#include "explosion.h"
#include <stdlib.h>
#include <time.h>
#include "score.h"
#include "game.h"
#include "result.h"
#include "player.h"
#include "staging.h"
#include "sound.h"
#include "enemy.h"
#include "fade.h"
//�}�N����`
#include "input.h"
#define NUM_STAGINGTEX (20)//���o�̎��
#define MAX_STAGING (5000)

#define SCOREBONUS_SIZE (300.0f)
#define APPER_STAGINGSIZE (50.0f)
#define PRESSSTART_STAGINGSIZE (120.0f)

#define SHIELD_SIZE (100.0f)


//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_apTextureStaging[NUM_STAGINGTEX] = {};
//D3DXVECTOR3 g_rotPI;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffStaging = NULL;
//Enemy g_Staging[MAX_ENEMY];//�G�̏��
//Result g_aResult;

Staging g_Staging[MAX_STAGING];

//int g_nCountTime = 0;//���Ԃ�\���O���[�o���ϐ�

//=====================================================
//==========================
//�G�̏���������
//==========================
void InitStaging(void)
{



	//srand((unsigned int)time(NULL));//�Q�[�����J�n�������Ԗ��ɗ����̎��ݒ�



	LPDIRECT3DDEVICE9 pDevice;
	int	nCntStaging;



	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\re_warning!_000.png",
		&g_apTextureStaging[STAGINGTYPE_WARNING]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\appearance_notice.png",
		&g_apTextureStaging[STAGINGTYPE_APPER]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\bullet_warning_000.png",
		&g_apTextureStaging[STAGINGTYPE_BULLETWARNING]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\PressStart_000.png",
		&g_apTextureStaging[STAGINGTYPE_PRESSSTART]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\PressEnter_or_start!_000.png",
		&g_apTextureStaging[STAGINGTYPE_PRESSENTER]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\Shield_000.png",
		&g_apTextureStaging[STAGINGTYPE_SHIELD]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\Normal_ClearBonus.png",
		&g_apTextureStaging[STAGINGTYPE_NORMAL_CLEARBONUS]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\Hard_ClearBonus.png",
		&g_apTextureStaging[STAGINGTYPE_HARD_CLEARBONUS]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\VeryHard_ClearBonus.png",
		&g_apTextureStaging[STAGINGTYPE_VERYHARD_CLEARBONUS]);


	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\rankingBG.png",
		&g_apTextureStaging[STAGINGTYPE_RANKING_BG]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\SpecialAttack_Boss1.png",
	    &g_apTextureStaging[STAGINGTYPE_BOSS1_SPECIAL_ATTACK]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Staging_WarningAttack.png",
		&g_apTextureStaging[STAGINGTYPE_WARNING_ATTACK]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\NormalWarning.png",
		&g_apTextureStaging[STAGINGTYPE_NORMAL_WARNINGS]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\SpecialAttack_Boss2.png",
		&g_apTextureStaging[STAGINGTYPE_BOSS2_SPECIAL_ATTACK]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Explosion_Warning.png",
		&g_apTextureStaging[STAGINGTYPE_EXPLOSIONWARNING]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\SpecialAttack_Boss3.png",
		&g_apTextureStaging[STAGINGTYPE_BOSS3_SPECIAL_ATTACK]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\SpecialAttack_Boss4.png",
		&g_apTextureStaging[STAGINGTYPE_BOSS4_SPECIAL_ATTACK]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\SpecialAttack_FinalBoss.png",
		&g_apTextureStaging[STAGINGTYPE_FINALBOSS_SPECIAL_ATTACK]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Staging_Safe.png",
		&g_apTextureStaging[STAGINGTYPE_SAFE]);







	//�G���̏�����
	for (nCntStaging = 0; nCntStaging < MAX_STAGING; nCntStaging++)
	{
		g_Staging[nCntStaging].nStagingType = 0;
		g_Staging[nCntStaging].nLife = 0;
		g_Staging[nCntStaging].bUse = false;
		g_Staging[nCntStaging].bCountStartFlag = false;//���o���n�܂��Ă���̃J�E���g���J�n����t���O��錾����B
		g_Staging[nCntStaging].nStagingCount = 0;//���o���n�܂��Ă���̃J�E���g������������B
		g_Staging[nCntStaging].fStagingPatternCountY = 0.0;//�A�j���[�V������x�点��J�E���g
		g_Staging[nCntStaging].fStagingPattern = 0.0f;//�A�j���[�V�����̎��
		g_Staging[nCntStaging].nAnimationRepeat = 0;//�A�j���[�V�������J��Ԃ��ϐ�
		g_Staging[nCntStaging].bDraw = true;//bUse��true���������Ƃɉ����A�X��bDraw��true���ǂ����ŕ`�悷�邩�ǂ��������߂�B
		g_Staging[nCntStaging].nCntEnemy = 0;
		g_Staging[nCntStaging].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Staging[nCntStaging].fColor_a = 0.0f;
		g_Staging[nCntStaging].bColorChange_a=false;
		g_Staging[nCntStaging].nAtackSummon = 0;//���o�������ꏊ�ɍU������������
		g_Staging[nCntStaging].rot = 0.0f;//��]
		g_Staging[nCntStaging].fScale = 0.0f;//�傫��
		g_Staging[nCntStaging].fAeffective_Scale = 0.0f;//���������U���̌��ʔ͈�
		g_Staging[nCntStaging].fA_Width = 0.0f;//���������U���̕�
		g_Staging[nCntStaging].fA_Height = 0.0f;//���������U���̍���
		g_Staging[nCntStaging].nA_Life = 0.0f;//���������U���̃��C�t
		g_Staging[nCntStaging].fA_rotSpeed = 0.0f;//��������U���̉�]���x
		g_Staging[nCntStaging].nCntTime = 0;//���Ԃ��v������
	}

	//g_Staging->pos = D3DXVECTOR3(0.0f,0.0f, 0.0f);//�ʒu������������

	//�`���[�g���A�����̏�����
	//for (nCntStaging = 0; nCntStaging < NUM_STAGINGTEX; nCntStaging++)
	//{
	//	
	//}


	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4*MAX_STAGING,// * NUM_STAGINGTEX,//�S�͕K�v�Ȓ��_���B����g���摜����2�������A��̃|���S���ł��̂ŁA���_�S�ő��v�B
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffStaging,
		NULL);

	VERTEX_2D* pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffStaging->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntStaging = 0; nCntStaging < MAX_STAGING; nCntStaging++)
	{
		//���_���W�̐ݒ�(�G�̑傫���j
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f,0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f,0.0f, 0.0f);

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
	g_pVtxBuffStaging->Unlock();
}

//====================================
//�`���[�g���A���̏I������
//====================================
void UninitStaging(void)
{
	//�e�N�X�`���̔j��
	int nCntStaging;
	for (nCntStaging = 0; nCntStaging < NUM_STAGINGTEX; nCntStaging++)
	{
		if (g_apTextureStaging[nCntStaging] != NULL)
		{
			g_apTextureStaging[nCntStaging]->Release();
			g_apTextureStaging[nCntStaging] = NULL;
		}
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffStaging != NULL)
	{
		g_pVtxBuffStaging->Release();
		g_pVtxBuffStaging = NULL;
	}
}


//========================================
//�G�̍X�V����
//========================================
void UpdateStaging(void)
{
	VERTEX_2D* pVtx;



	int nCntStaging;


	Enemy* pEnemy = GetEnemy();


	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffStaging->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntStaging = 0; nCntStaging < MAX_STAGING; nCntStaging++)
	{
		if (g_Staging[nCntStaging].bUse == true)
		{
			g_Staging[nCntStaging].nLife--;

			

			g_Staging[nCntStaging].nStagingCount++;


			if (g_Staging[nCntStaging].nLife <= 0)
			{
				g_Staging[nCntStaging].bUse = false;
			}

			if (g_Staging[nCntStaging].nStagingType == STAGINGTYPE_WARNING)
			{


				//���_���W�̐ݒ�(�G�̑傫���j
				pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

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

				if (g_Staging[nCntStaging].nStagingCount % 5 == 0)
				{//�A�j���[�V�������E�ɐi�߂�B
					g_Staging[nCntStaging].fStagingPattern += 1.0f;
				}

				if (g_Staging[nCntStaging].fStagingPattern >= 4.0f && g_Staging[nCntStaging].fStagingPatternCountY >= 0.5f)
				{
					g_Staging[nCntStaging].nAnimationRepeat++;
					g_Staging[nCntStaging].fStagingPattern = 0.0f;
					g_Staging[nCntStaging].fStagingPatternCountY = 0.0f;

				}
				else if (g_Staging[nCntStaging].fStagingPattern >= 4.0f)
				{//�A�j���[�V���������̒i�Ɉڂ��B
					g_Staging[nCntStaging].fStagingPattern = 0.0f;
					g_Staging[nCntStaging].fStagingPatternCountY += 0.5f;
				}

				if (g_Staging[nCntStaging].nAnimationRepeat >= 3)
				{
					g_Staging[nCntStaging].bUse = false;
				}
				pVtx[0].tex = D3DXVECTOR2(0.0f + (1.0f / 4.0f) * g_Staging[nCntStaging].fStagingPattern, g_Staging[nCntStaging].fStagingPatternCountY);
				pVtx[1].tex = D3DXVECTOR2((1.0f / 4.0f) + (1.0f / 4.0f) * g_Staging[nCntStaging].fStagingPattern, g_Staging[nCntStaging].fStagingPatternCountY);
				pVtx[2].tex = D3DXVECTOR2(0.0f + (1.0f / 4.0f) * g_Staging[nCntStaging].fStagingPattern, 0.5f + g_Staging[nCntStaging].fStagingPatternCountY);
				pVtx[3].tex = D3DXVECTOR2((1.0f / 4.0f) + (1.0f / 4.0f) * g_Staging[nCntStaging].fStagingPattern, 0.5f + g_Staging[nCntStaging].fStagingPatternCountY);
			}
			if (g_Staging[nCntStaging].nStagingType == STAGINGTYPE_BOSS1_SPECIAL_ATTACK ||
				g_Staging[nCntStaging].nStagingType == STAGINGTYPE_BOSS2_SPECIAL_ATTACK ||
				g_Staging[nCntStaging].nStagingType == STAGINGTYPE_BOSS3_SPECIAL_ATTACK ||
				g_Staging[nCntStaging].nStagingType == STAGINGTYPE_BOSS4_SPECIAL_ATTACK ||
				g_Staging[nCntStaging].nStagingType == STAGINGTYPE_FINALBOSS_SPECIAL_ATTACK)
			{
				if (g_Staging[nCntStaging].nStagingCount == 1)
				{//�|�[�Y��Ԃɂ��āA���o���������s��
					SetPause(PAUSETYPE_STAGING);
				}
				//���_���W�̐ݒ�(�G�̑傫���j
				pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

				//rhw�̐ݒ�
				pVtx[0].rhw = 1.0f;
				pVtx[1].rhw = 1.0f;
				pVtx[2].rhw = 1.0f;
				pVtx[3].rhw = 1.0f;

				//���_�J���[�̐ݒ�
		
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
				
				if (g_Staging[nCntStaging].nLife == 0)
				{
					g_Staging[nCntStaging].bUse = false;
					SetPause(PAUSETYPE_UNLOCK);//�|�[�Y��ԉ���
				}
			}
			if (g_Staging[nCntStaging].nStagingType == STAGINGTYPE_APPER)
			{



				pVtx[0].pos = D3DXVECTOR3(g_Staging[nCntStaging].pos.x - APPER_STAGINGSIZE, g_Staging[nCntStaging].pos.y - APPER_STAGINGSIZE, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_Staging[nCntStaging].pos.x + APPER_STAGINGSIZE, g_Staging[nCntStaging].pos.y - APPER_STAGINGSIZE, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_Staging[nCntStaging].pos.x - APPER_STAGINGSIZE, g_Staging[nCntStaging].pos.y + APPER_STAGINGSIZE, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_Staging[nCntStaging].pos.x + APPER_STAGINGSIZE, g_Staging[nCntStaging].pos.y + APPER_STAGINGSIZE, 0.0f);

				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

				if (g_Staging[nCntStaging].nStagingCount % 5 == 0)
				{
					g_Staging[nCntStaging].bDraw = false;
				}
				else
				{
					g_Staging[nCntStaging].bDraw = true;
				}
				if (g_Staging[nCntStaging].nStagingCount >= 100)
				{
					g_Staging[nCntStaging].bUse = false;
				}



			}
			if (g_Staging[nCntStaging].nStagingType == STAGINGTYPE_BULLETWARNING)
			{



				pVtx[0].pos = D3DXVECTOR3(g_Staging[nCntStaging].pos.x - APPER_STAGINGSIZE, g_Staging[nCntStaging].pos.y - APPER_STAGINGSIZE, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_Staging[nCntStaging].pos.x + APPER_STAGINGSIZE, g_Staging[nCntStaging].pos.y - APPER_STAGINGSIZE, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_Staging[nCntStaging].pos.x - APPER_STAGINGSIZE, g_Staging[nCntStaging].pos.y + APPER_STAGINGSIZE, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_Staging[nCntStaging].pos.x + APPER_STAGINGSIZE, g_Staging[nCntStaging].pos.y + APPER_STAGINGSIZE, 0.0f);

				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

				if (g_Staging[nCntStaging].nStagingCount % 5 == 0)
				{
					g_Staging[nCntStaging].bDraw = false;
				}
				else
				{
					g_Staging[nCntStaging].bDraw = true;
				}
				if (g_Staging[nCntStaging].nStagingCount >= 150)
				{
					PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_004);
					g_Staging[nCntStaging].bUse = false;

					int nCnt;
					for (nCnt = 0; nCnt < 8; nCnt++)
					{
						SetBullet(g_Staging[nCntStaging].pos, D3DXVECTOR3((sinf(-D3DX_PI * (0.25f * nCnt)) * 5.0f), (cosf(-D3DX_PI * (0.25 * nCnt)) * 5.0f), 0.0f), GENERALLY_BULLETLIFE, BULLETTYPE_ENEMY, 0, -1, -1, false, MOVETYPE_NORMAL);

					}
				}



			}
			if (g_Staging[nCntStaging].nStagingType == STAGINGTYPE_NORMAL_WARNINGS ||
				g_Staging[nCntStaging].nStagingType == STAGINGTYPE_SAFE)
			{

				//g_Staging[nCntStaging].nCntTime++;//���Ԃ��v������
				//if (g_Staging[nCntStaging].nCntTime == 1)
				//{
				//	PlaySound(SOUND_LABEL_SE_WARNING_001);
				//}


				pVtx[0].pos = D3DXVECTOR3(g_Staging[nCntStaging].pos.x - g_Staging[nCntStaging].fScale, g_Staging[nCntStaging].pos.y - g_Staging[nCntStaging].fScale, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_Staging[nCntStaging].pos.x + g_Staging[nCntStaging].fScale, g_Staging[nCntStaging].pos.y - g_Staging[nCntStaging].fScale, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_Staging[nCntStaging].pos.x - g_Staging[nCntStaging].fScale, g_Staging[nCntStaging].pos.y + g_Staging[nCntStaging].fScale, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_Staging[nCntStaging].pos.x + g_Staging[nCntStaging].fScale, g_Staging[nCntStaging].pos.y + g_Staging[nCntStaging].fScale, 0.0f);

				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

				if (g_Staging[nCntStaging].nStagingCount % 5 == 0)
				{
					g_Staging[nCntStaging].bDraw = false;
				}
				else
				{
					g_Staging[nCntStaging].bDraw = true;
				}

			}
			if (g_Staging[nCntStaging].nStagingType == STAGINGTYPE_NORMAL_WARNINGS_RELATED)
			{
				Enemy* pEnemy = GetEnemy();


				pVtx[0].pos = D3DXVECTOR3(g_Staging[nCntStaging].pos.x - g_Staging[nCntStaging].fScale, g_Staging[nCntStaging].pos.y - g_Staging[nCntStaging].fScale, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_Staging[nCntStaging].pos.x + g_Staging[nCntStaging].fScale, g_Staging[nCntStaging].pos.y - g_Staging[nCntStaging].fScale, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_Staging[nCntStaging].pos.x - g_Staging[nCntStaging].fScale, g_Staging[nCntStaging].pos.y + g_Staging[nCntStaging].fScale, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_Staging[nCntStaging].pos.x + g_Staging[nCntStaging].fScale, g_Staging[nCntStaging].pos.y + g_Staging[nCntStaging].fScale, 0.0f);

				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

				if (g_Staging[nCntStaging].nStagingCount % 5 == 0)
				{
					g_Staging[nCntStaging].bDraw = false;
				}
				else
				{
					g_Staging[nCntStaging].bDraw = true;
				}

				for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++,pEnemy++)
				{
					if (pEnemy->bUse == true && g_Staging[nCntStaging].nCntEnemy == nCntEnemy)
					{
						g_Staging[nCntStaging].pos.x = pEnemy->pos.x;
						g_Staging[nCntStaging].pos.y = pEnemy->pos.y;
					}
				}

			}
			if (g_Staging[nCntStaging].nStagingType == STAGINGTYPE_WARNING_ATTACK)
			{

				//g_Staging[nCntStaging].nCntTime++;//���Ԃ��v������
				//if (g_Staging[nCntStaging].nCntTime == 1)
				//{
				//	PlaySound(SOUND_LABEL_SE_WARNING_001);
				//}

				pVtx[0].pos = D3DXVECTOR3(g_Staging[nCntStaging].pos.x - APPER_STAGINGSIZE, g_Staging[nCntStaging].pos.y - APPER_STAGINGSIZE, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_Staging[nCntStaging].pos.x + APPER_STAGINGSIZE, g_Staging[nCntStaging].pos.y - APPER_STAGINGSIZE, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_Staging[nCntStaging].pos.x - APPER_STAGINGSIZE, g_Staging[nCntStaging].pos.y + APPER_STAGINGSIZE, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_Staging[nCntStaging].pos.x + APPER_STAGINGSIZE, g_Staging[nCntStaging].pos.y + APPER_STAGINGSIZE, 0.0f);

				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

				if (g_Staging[nCntStaging].nStagingCount % 5 == 0)
				{
					g_Staging[nCntStaging].bDraw = false;
				}
				else
				{
					g_Staging[nCntStaging].bDraw = true;
				}
				if (g_Staging[nCntStaging].nStagingCount >= 150)
				{
			/*		PlaySound(SOUND_LABEL_SE_SHOT_ENEMY_004);*/
					g_Staging[nCntStaging].bUse = false;

					int nCnt;

					if (g_Staging->nAtackSummon == ATTACKTYPE_NEEDLE)
					{
						SetAttack(D3DXVECTOR3(g_Staging[nCntStaging].pos.x, g_Staging[nCntStaging].pos.y, 0.0f),
							D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							ATTACKTYPE_NEEDLE,
							-1, g_Staging[nCntStaging].nCntEnemy, -1, MOVETYPE_NORMAL, 100.0f, 100.0f, 100, g_Staging[nCntStaging].rot, -1);
					}
					

				}

				//==================================
				//�X������������
				//==================================
				if (g_Staging[nCntStaging].nAtackSummon == ATTACKTYPE_ICE_EXPLOSION && g_Staging[nCntStaging].bUse == false)
				{
					SetAttack(D3DXVECTOR3(g_Staging[nCntStaging].pos.x, g_Staging[nCntStaging].pos.y + 100.0f, 0.0f),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						ATTACKTYPE_ICE_EXPLOSION,
						-1, g_Staging[nCntStaging].nCntEnemy, -1, MOVETYPE_NORMAL,
						g_Staging[nCntStaging].fScale, g_Staging[nCntStaging].fScale,g_Staging[nCntStaging].nA_Life, g_Staging[nCntStaging].rot, -1);
				}

				//==================================
				//�u�A�C�V�N���N���[�v��������
				//==================================
				if (g_Staging[nCntStaging].nAtackSummon == ATTACKTYPE_ICICLECLAW && g_Staging[nCntStaging].bUse == false)
				{
					SetAttack(D3DXVECTOR3(g_Staging[nCntStaging].pos.x, g_Staging[nCntStaging].pos.y, 0.0f),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						ATTACKTYPE_ICICLECLAW,
						-1, g_Staging[nCntStaging].nCntEnemy, -1, MOVETYPE_NORMAL,
						g_Staging[nCntStaging].fA_Width, g_Staging[nCntStaging].fA_Height, g_Staging[nCntStaging].nA_Life, g_Staging[nCntStaging].rot, -1);
				}

				//==================================
				//�u�t�@�C�A�v��������
				//==================================
				if (g_Staging[nCntStaging].nAtackSummon == ATTACKTYPE_FIRE && g_Staging[nCntStaging].bUse == false)
				{
					SetAttack(D3DXVECTOR3(g_Staging[nCntStaging].pos.x, g_Staging[nCntStaging].pos.y, 0.0f),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						ATTACKTYPE_FIRE,
						-1
						, g_Staging[nCntStaging].nCntEnemy, -1, MOVETYPE_NORMAL,
						g_Staging[nCntStaging].fA_Width, g_Staging[nCntStaging].fA_Height, g_Staging[nCntStaging].nA_Life, g_Staging[nCntStaging].rot, -1);
				}
				//==================================
				//���U����������
				//==================================
				if (g_Staging[nCntStaging].nAtackSummon == ATTACKTYPE_SHAKINGSWORD && g_Staging[nCntStaging].bUse == false)
				{
					SetAttack(D3DXVECTOR3(g_Staging[nCntStaging].pos.x, g_Staging[nCntStaging].pos.y, 0.0f),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						ATTACKTYPE_SHAKINGSWORD,
						-1
						, g_Staging[nCntStaging].nCntEnemy, -1, MOVETYPE_NORMAL,
						g_Staging[nCntStaging].fA_Width, g_Staging[nCntStaging].fA_Height, g_Staging[nCntStaging].nA_Life, g_Staging[nCntStaging].rot,g_Staging[nCntStaging].fA_rotSpeed);
				}

				//==================================
				//�u�n�M��������
				//==================================
				if (g_Staging[nCntStaging].nAtackSummon == ATTACKTYPE_FLASHBLADE && g_Staging[nCntStaging].bUse == false)
				{
					SetAttack(D3DXVECTOR3(g_Staging[nCntStaging].pos.x, g_Staging[nCntStaging].pos.y, 0.0f),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						ATTACKTYPE_FLASHBLADE,
						-1,
						g_Staging[nCntStaging].nCntEnemy, -1, MOVETYPE_NORMAL,
						g_Staging[nCntStaging].fA_Width, g_Staging[nCntStaging].fA_Height, g_Staging[nCntStaging].nA_Life, g_Staging[nCntStaging].rot, g_Staging[nCntStaging].fA_rotSpeed);
				}

				//==================================
				//�T���_�[��������
				//==================================
				if (g_Staging[nCntStaging].nAtackSummon == ATTACKTYPE_THUNDER_E4 && g_Staging[nCntStaging].bUse == false)
				{
					SetAttack(D3DXVECTOR3(g_Staging[nCntStaging].pos.x, g_Staging[nCntStaging].pos.y, 0.0f),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						ATTACKTYPE_THUNDER_E4,
						-1,
						g_Staging[nCntStaging].nCntEnemy, -1, MOVETYPE_NORMAL,
						g_Staging[nCntStaging].fA_Width, g_Staging[nCntStaging].fA_Height, g_Staging[nCntStaging].nA_Life, g_Staging[nCntStaging].rot, g_Staging[nCntStaging].fA_rotSpeed);
				}


				//==================================
				//�T���_�[�Q��������
				//==================================
				if (g_Staging[nCntStaging].nAtackSummon == ATTACKTYPE_THUNDER_2&& g_Staging[nCntStaging].bUse == false)
				{
					SetAttack(D3DXVECTOR3(g_Staging[nCntStaging].pos.x, g_Staging[nCntStaging].pos.y, 0.0f),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						ATTACKTYPE_THUNDER_2,
						-1,
						g_Staging[nCntStaging].nCntEnemy, -1, MOVETYPE_NORMAL,
						g_Staging[nCntStaging].fA_Width, g_Staging[nCntStaging].fA_Height, g_Staging[nCntStaging].nA_Life, g_Staging[nCntStaging].rot, g_Staging[nCntStaging].fA_rotSpeed);
				}

				if (g_Staging[nCntStaging].nAtackSummon == ATTACKTYPE_BEAM && g_Staging[nCntStaging].bUse == false)
				{
					SetAttack(D3DXVECTOR3(g_Staging[nCntStaging].pos.x, g_Staging[nCntStaging].pos.y, 0.0f),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						ATTACKTYPE_BEAM,
						-1,
						g_Staging[nCntStaging].nCntEnemy, -1, MOVETYPE_NORMAL,
						g_Staging[nCntStaging].fA_Width, g_Staging[nCntStaging].fA_Height, g_Staging[nCntStaging].nA_Life, g_Staging[nCntStaging].rot, g_Staging[nCntStaging].fA_rotSpeed);
				}

				if (g_Staging[nCntStaging].nAtackSummon == ATTACKTYPE_FINALBEAM && g_Staging[nCntStaging].bUse == false)
				{
					SetAttack(D3DXVECTOR3(g_Staging[nCntStaging].pos.x, g_Staging[nCntStaging].pos.y, 0.0f),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						ATTACKTYPE_FINALBEAM,
						-1,
						g_Staging[nCntStaging].nCntEnemy, -1, MOVETYPE_NORMAL,
						g_Staging[nCntStaging].fA_Width, g_Staging[nCntStaging].fA_Height, g_Staging[nCntStaging].nA_Life, g_Staging[nCntStaging].rot, g_Staging[nCntStaging].fA_rotSpeed);
				}
			}
			if (g_Staging[nCntStaging].nStagingType == STAGINGTYPE_EXPLOSIONWARNING)
			{
				//g_Staging[nCntStaging].nCntTime++;//���Ԃ��v������
				//if (g_Staging[nCntStaging].nCntTime == 1)
				//{
				//	PlaySound(SOUND_LABEL_SE_WARNING_001);
				//}

				pVtx[0].pos = D3DXVECTOR3(g_Staging[nCntStaging].pos.x - g_Staging[nCntStaging].fScale, g_Staging[nCntStaging].pos.y - g_Staging[nCntStaging].fScale, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_Staging[nCntStaging].pos.x + g_Staging[nCntStaging].fScale, g_Staging[nCntStaging].pos.y - g_Staging[nCntStaging].fScale, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_Staging[nCntStaging].pos.x - g_Staging[nCntStaging].fScale, g_Staging[nCntStaging].pos.y + g_Staging[nCntStaging].fScale, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_Staging[nCntStaging].pos.x + g_Staging[nCntStaging].fScale, g_Staging[nCntStaging].pos.y + g_Staging[nCntStaging].fScale, 0.0f);

				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

				if (g_Staging[nCntStaging].nStagingCount % 5 == 0)
				{
					g_Staging[nCntStaging].bDraw = false;
				}
				else
				{
					g_Staging[nCntStaging].bDraw = true;
				}

				if (g_Staging[nCntStaging].bUse == false)
				{
					SetExplosion(g_Staging[nCntStaging].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), EXPLOSIONTYPE_E6, g_Staging[nCntStaging].fScale, g_Staging[nCntStaging].fScale);
				}
			}
			if (g_Staging[nCntStaging].nStagingType == STAGINGTYPE_PRESSSTART || g_Staging[nCntStaging].nStagingType == STAGINGTYPE_PRESSENTER)

			{



				pVtx[0].pos = D3DXVECTOR3(g_Staging[nCntStaging].pos.x - PRESSSTART_STAGINGSIZE, g_Staging[nCntStaging].pos.y - PRESSSTART_STAGINGSIZE, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_Staging[nCntStaging].pos.x + PRESSSTART_STAGINGSIZE, g_Staging[nCntStaging].pos.y - PRESSSTART_STAGINGSIZE, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_Staging[nCntStaging].pos.x - PRESSSTART_STAGINGSIZE, g_Staging[nCntStaging].pos.y + PRESSSTART_STAGINGSIZE, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_Staging[nCntStaging].pos.x + PRESSSTART_STAGINGSIZE, g_Staging[nCntStaging].pos.y + PRESSSTART_STAGINGSIZE, 0.0f);

				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);



				//===============================================================
				//�A���t�@�l�̃C���N�������g�A�f�N�������g�̏���
				//===============================================================
				if (g_Staging[nCntStaging].bColorChange_a == false)
				{
					g_Staging[nCntStaging].fColor_a += 0.02f;
				}
				else
				{
					g_Staging[nCntStaging].fColor_a -= 0.02f;
				}
				//================================================================

				//================================================================
				//�A���t�@�l��ς���t���O�̐ݒ菈��
				//================================================================
				if (g_Staging[nCntStaging].fColor_a >= 1.0f)
				{
					g_Staging[nCntStaging].bColorChange_a = true;
				}
				if (g_Staging[nCntStaging].fColor_a <= 0.0f)
				{
					g_Staging[nCntStaging].bColorChange_a = false;
				}
				//================================================================
				//���_�J���[�̐ݒ�
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Staging[nCntStaging].fColor_a);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Staging[nCntStaging].fColor_a);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Staging[nCntStaging].fColor_a);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Staging[nCntStaging].fColor_a);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
			}
			if (g_Staging[nCntStaging].nStagingType == STAGINGTYPE_SHIELD)
			{

				g_Staging[nCntStaging].nLife--;

				int nCntBullet;

				pVtx[0].pos = D3DXVECTOR3(g_Staging[nCntStaging].pos.x - SHIELD_SIZE, g_Staging[nCntStaging].pos.y - SHIELD_SIZE, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_Staging[nCntStaging].pos.x + SHIELD_SIZE, g_Staging[nCntStaging].pos.y - SHIELD_SIZE, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_Staging[nCntStaging].pos.x - SHIELD_SIZE, g_Staging[nCntStaging].pos.y + SHIELD_SIZE, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_Staging[nCntStaging].pos.x + SHIELD_SIZE, g_Staging[nCntStaging].pos.y + SHIELD_SIZE, 0.0f);

				Bullet* pBullet = GetBullet();

				for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++, pBullet++)
				{
					if (pBullet->bUse == true)
					{
						if (pBullet->type == BULLETTYPE_BANDY
							|| pBullet->type == BULLETTYPE_BOLBY
							|| pBullet->type == BULLETTYPE_DOWNWIND
							|| pBullet->type == BULLETTYPE_ENEMY
							|| pBullet->type == BULLETTYPE_ENEMY9
							|| pBullet->type == BULLETTYPE_LEFT_WIND
							|| pBullet->type == BULLETTYPE_REAFY
							|| pBullet->type == BULLETTYPE_REFLECTION
							|| pBullet->type == BULLETTYPE_REVERSE_CURVE
							|| pBullet->type == BULLETTYPE_RIGHT_WIND
							|| pBullet->type == BULLETTYPE_ROCKON
							|| pBullet->type == BULLETTYPE_SCATTERING
							|| pBullet->type == BULLETTYPE_UPWIND
							|| pBullet->type == BULLETTYPE_SATELLITE)
						{
							if (pBullet->pos.x >= g_Staging[nCntStaging].pos.x - SHIELD_SIZE && pBullet->pos.x <= g_Staging[nCntStaging].pos.x + SHIELD_SIZE &&
								pBullet->pos.y >= g_Staging[nCntStaging].pos.y - SHIELD_SIZE && pBullet->pos.y <= g_Staging[nCntStaging].pos.y + SHIELD_SIZE)
							{
								pBullet->bUse = false;

							}
						}
					}
				}



				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);




			}
			if (g_Staging[nCntStaging].nStagingType == STAGINGTYPE_NORMAL_CLEARBONUS
				|| g_Staging[nCntStaging].nStagingType == STAGINGTYPE_HARD_CLEARBONUS
				|| g_Staging[nCntStaging].nStagingType == STAGINGTYPE_VERYHARD_CLEARBONUS)
			{
				pVtx[0].pos = D3DXVECTOR3(g_Staging[nCntStaging].pos.x - SCOREBONUS_SIZE, g_Staging[nCntStaging].pos.y - SCOREBONUS_SIZE, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_Staging[nCntStaging].pos.x + SCOREBONUS_SIZE, g_Staging[nCntStaging].pos.y - SCOREBONUS_SIZE, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_Staging[nCntStaging].pos.x - SCOREBONUS_SIZE, g_Staging[nCntStaging].pos.y + SCOREBONUS_SIZE, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_Staging[nCntStaging].pos.x + SCOREBONUS_SIZE, g_Staging[nCntStaging].pos.y + SCOREBONUS_SIZE, 0.0f);

				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}
			if (g_Staging[nCntStaging].nStagingType == STAGINGTYPE_RANKING_BG)
			{
				pVtx[0].pos = D3DXVECTOR3(g_Staging[nCntStaging].pos.x - SCREEN_WIDTH / 2, g_Staging[nCntStaging].pos.y - SCREEN_HEIGHT / 2, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_Staging[nCntStaging].pos.x + SCREEN_WIDTH / 2, g_Staging[nCntStaging].pos.y - SCREEN_HEIGHT / 2, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_Staging[nCntStaging].pos.x - SCREEN_WIDTH / 2, g_Staging[nCntStaging].pos.y + SCREEN_HEIGHT / 2, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_Staging[nCntStaging].pos.x + SCREEN_WIDTH / 2, g_Staging[nCntStaging].pos.y + SCREEN_HEIGHT / 2, 0.0f);

				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}

			
		}
		pVtx += 4;//���_�f�[�^�̃|�C���^���S���i�߂�B
	}
	//���_�o�b�t�@���A�����b�N����

	g_pVtxBuffStaging->Unlock();

	
}


//=========================================
//�G�̕`�揈��
//=========================================
void DrawStaging(void)
{

	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

	Enemy* pEnemy = GetEnemy();

	int nCntStaging;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffStaging, 0, sizeof(VERTEX_2D));//���_�o�b�t�@�ւ̃|�C���^�ƒ��_���̍\���̂̃T�C�Y

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�

	for (nCntStaging = 0; nCntStaging < MAX_STAGING; nCntStaging++)
	{
		if (g_Staging[nCntStaging].bUse == true)
		{
			if (g_Staging[nCntStaging].bDraw == true)
			{
				if (g_Staging[nCntStaging].nStagingType == STAGINGTYPE_WARNING)
				{
					pDevice->SetTexture(0, g_apTextureStaging[STAGINGTYPE_WARNING]);
				}
				if (g_Staging[nCntStaging].nStagingType == STAGINGTYPE_APPER)
				{
					pDevice->SetTexture(0, g_apTextureStaging[STAGINGTYPE_APPER]);
				}
				if (g_Staging[nCntStaging].nStagingType ==STAGINGTYPE_BULLETWARNING)
				{
					pDevice->SetTexture(0, g_apTextureStaging[STAGINGTYPE_BULLETWARNING]);
				}
				if (g_Staging[nCntStaging].nStagingType == STAGINGTYPE_PRESSSTART)
				{
					pDevice->SetTexture(0, g_apTextureStaging[STAGINGTYPE_PRESSSTART]);
				}
				if (g_Staging[nCntStaging].nStagingType == STAGINGTYPE_PRESSENTER)
				{
					pDevice->SetTexture(0, g_apTextureStaging[STAGINGTYPE_PRESSENTER]);
				}
				if (g_Staging[nCntStaging].nStagingType == STAGINGTYPE_SHIELD)
				{
					pDevice->SetTexture(0, g_apTextureStaging[STAGINGTYPE_SHIELD]);
				}
				if (g_Staging[nCntStaging].nStagingType == STAGINGTYPE_NORMAL_CLEARBONUS)
				{
					pDevice->SetTexture(0, g_apTextureStaging[STAGINGTYPE_NORMAL_CLEARBONUS]);
				}
				if (g_Staging[nCntStaging].nStagingType == STAGINGTYPE_HARD_CLEARBONUS)
				{
					pDevice->SetTexture(0, g_apTextureStaging[STAGINGTYPE_HARD_CLEARBONUS]);
				}
				if (g_Staging[nCntStaging].nStagingType == STAGINGTYPE_VERYHARD_CLEARBONUS)
				{
					pDevice->SetTexture(0, g_apTextureStaging[STAGINGTYPE_VERYHARD_CLEARBONUS]);
				}
				if (g_Staging[nCntStaging].nStagingType == STAGINGTYPE_RANKING_BG)
				{
					pDevice->SetTexture(0, g_apTextureStaging[STAGINGTYPE_RANKING_BG]);
				}
				if (g_Staging[nCntStaging].nStagingType == STAGINGTYPE_BOSS1_SPECIAL_ATTACK)
				{
					pDevice->SetTexture(0, g_apTextureStaging[STAGINGTYPE_BOSS1_SPECIAL_ATTACK]);
				}
				if (g_Staging[nCntStaging].nStagingType == STAGINGTYPE_WARNING_ATTACK)
				{
					pDevice->SetTexture(0, g_apTextureStaging[STAGINGTYPE_WARNING_ATTACK]);
				}
				if (g_Staging[nCntStaging].nStagingType == STAGINGTYPE_NORMAL_WARNINGS || 
					g_Staging[nCntStaging].nStagingType == STAGINGTYPE_NORMAL_WARNINGS_RELATED)
				{
					pDevice->SetTexture(0, g_apTextureStaging[STAGINGTYPE_NORMAL_WARNINGS]);
				}
				if (g_Staging[nCntStaging].nStagingType == STAGINGTYPE_BOSS2_SPECIAL_ATTACK)
				{
					pDevice->SetTexture(0, g_apTextureStaging[STAGINGTYPE_BOSS2_SPECIAL_ATTACK]);
				}
				if (g_Staging[nCntStaging].nStagingType == STAGINGTYPE_EXPLOSIONWARNING)
				{
					pDevice->SetTexture(0, g_apTextureStaging[STAGINGTYPE_EXPLOSIONWARNING]);
				}
				if (g_Staging[nCntStaging].nStagingType == STAGINGTYPE_BOSS3_SPECIAL_ATTACK)
				{
					pDevice->SetTexture(0, g_apTextureStaging[STAGINGTYPE_BOSS3_SPECIAL_ATTACK]);
				}
				if (g_Staging[nCntStaging].nStagingType == STAGINGTYPE_BOSS4_SPECIAL_ATTACK)
				{
					pDevice->SetTexture(0, g_apTextureStaging[STAGINGTYPE_BOSS4_SPECIAL_ATTACK]);
				}
				if (g_Staging[nCntStaging].nStagingType == STAGINGTYPE_FINALBOSS_SPECIAL_ATTACK)
				{
					pDevice->SetTexture(0, g_apTextureStaging[STAGINGTYPE_FINALBOSS_SPECIAL_ATTACK]);
				}
				if (g_Staging[nCntStaging].nStagingType == STAGINGTYPE_SAFE)
				{
					pDevice->SetTexture(0, g_apTextureStaging[STAGINGTYPE_SAFE]);
				}
				//�|���S���̕`��
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntStaging * 4, 2);//0�́A�`�悷��ŏ��̒��_�C���f�b�N�X�ŁA2�́A�`�悷��v���~�e�B�u��

				//&g_pVtxBuffPolygon[0],//���_���̐擪�A�h���X
				//sizeof(VERTEX_2D));//���_���\���̂̃T�C�Y
			}
		}
	}


}
void SetStaging(STAGINGTYPE type, int nCntEnemy, D3DXVECTOR3 pos, int nLife, ATTACKTYPE Atype, float Arot, float fScale, float fA_rotSpeed, float fA_Width,
	float fA_Height, int nA_Life)
{
	int nCntStaging;

	VERTEX_2D* pVtx;

	/*Enemy* pEnemy = GetEnemy();*/

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffStaging->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntStaging = 0; nCntStaging < MAX_STAGING; nCntStaging++)
	{
		if (g_Staging[nCntStaging].bUse == false)
		{
		
			g_Staging[nCntStaging].pos = pos;//SetEnemy�̎��ɁA�G�̈ʒu��������ꂽ�ہA�ʒu�̈����������SetStaging��pos�ɑ�����邱�Ƃɂ��A
			g_Staging[nCntStaging].nLife = nLife;
			g_Staging[nCntStaging].nStagingType = type;//���o�̎�ނ��Z�b�g����B
			g_Staging[nCntStaging].bUse = true;
			g_Staging[nCntStaging].nStagingCount = 0;
			g_Staging[nCntStaging].bCountStartFlag = true;
			g_Staging[nCntStaging].fStagingPatternCountY = 0.0f;
			g_Staging[nCntStaging].fStagingPattern = 0.0f;
			g_Staging[nCntStaging].nAnimationRepeat = 0;
			g_Staging[nCntStaging].bDraw = true;
			g_Staging[nCntStaging].nCntEnemy = nCntEnemy;
			g_Staging[nCntStaging].fColor_a = 1.0f;
			g_Staging[nCntStaging].bColorChange_a = false;
			g_Staging[nCntStaging].nAtackSummon = Atype;//���o�������ꏊ�ɍU������������
			g_Staging[nCntStaging].rot = Arot;//��]
			g_Staging[nCntStaging].fScale = fScale;//�傫��
			g_Staging[nCntStaging].fA_Width = fA_Width;//���������U���̕�
			g_Staging[nCntStaging].fA_Height = fA_Height;//���������U���̍���
			g_Staging[nCntStaging].nA_Life = nA_Life;//���������U���̃��C�t
			g_Staging[nCntStaging].fA_rotSpeed = fA_rotSpeed;//��������U���̉�]���x
			g_Staging[nCntStaging].nCntTime = 0;//���Ԃ��v������

			break;
		}
		pVtx += 4;
	}
	g_pVtxBuffStaging->Unlock();
}
