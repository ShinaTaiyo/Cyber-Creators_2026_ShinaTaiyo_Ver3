//========================================================
//
//�W���R�O���F�G�t�F�N�g������񌩒���[game.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#include "game.h"
#include "bg.h"
#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "explosion.h"
#include "score.h"
#include "input.h"
#include "main.h"
#include "health.h"
#include "Gage.h"
#include "SkillName.h"
#include <stdlib.h>
#include <time.h>
#include "result.h"
#include "sound.h"
#include "effect.h"
#include "fade.h"
#include "staging.h"
#include "pause.h"
#include "particle.h"
#include "SkillGage.h"
#include "ranking.h"
#include "damage.h"
#include "wave.h"
#include "Difficulty.h"
#include "scorebonus.h"
#define MAX_WAVE (14) //�E�F�[�u��

//�O���[�o���ϐ�
GAMESTATE g_GameState = GAMESTATE_NONE;//�Q�[���̏��
int g_nCounterGameState = 0;//��ԊǗ��J�E���^�[
int g_nCountTime;//�Q�[���J�n������o�������Ԃ��Ǘ�����ϐ�

bool g_bWaveFlag[MAX_WAVE];//���ꂼ��̃E�F�[�u�J�n�̃t���O��錾����B

float g_fRandumEnemyPosX[MAX_ENEMY*2];
float g_fRandumEnemyPosY[MAX_ENEMY*2];

bool g_bPause = false;//�|�[�Y�����ǂ���

int g_nChoosePauseCommand;//�|�[�Y���̎��ɑI�����郁�j���[��I������ϐ�

int g_nCountPauseMenu;//�|�[�Y��ʂ��J���Ă��鎞�Ԃ��J�E���g����ϐ�

int g_nScoreBonustime[MAX_WAVE+1];
bool g_bScoreBonusTimeCountStart[MAX_WAVE+1];//���ꂼ��̃E�F�[�u���n�܂����Ƃ��ɁA�E�F�[�u�N���A���ԃX�R�A�{�[�i�X�̃J�E���g���X�^�[�g����B
//bool g_bResultFlag_gameover = false;
//===================================
//�Q�[����ʂ̏���������
//===================================
void InitGame(void)
{
	g_nChoosePauseCommand = 0;

	g_nCountPauseMenu = 0;

	g_nCountTime = 0;//���Ԃ̏�����

	g_bPause = false;//�|�[�Y�������

	int nCnt;
	InitBG();
	InitEffect();
	InitPlayer();
	InitHealth();
	InitGage();
	InitSkillName();
	InitSkillGage();
	InitDamage();
	InitEnemy();
	InitExplosion();
	InitBullet();
	InitWave();
	InitScore();
	InitScoreBonus();
	InitStaging();
	InitPause();
	InitParticle();
	InitFade();//�t�F�[�h�A�E�g��Ɉڍs���郂�[�h��ݒ�

	PlaySound(SOUND_LABEL_BGM001);

	for (nCnt = 0; nCnt < MAX_WAVE; nCnt++)
	{
		g_bWaveFlag[nCnt] = false;
	}

	for (nCnt = 0; nCnt < MAX_WAVE+1; nCnt++)
	{
		g_nScoreBonustime[nCnt] = 6000;//�ꕪ�̃X�R�A�{�[�i�X�܂ł̐������Ԃ�݂���i60f*100s)
		g_bScoreBonusTimeCountStart[nCnt] = false;//�E�F�[�u�J�n����true�ɂ��A�X�R�A�{�[�i�X�̃J�E���g���X�^�[�g����B
	}
	

	
	g_bScoreBonusTimeCountStart[0] = true;

	//SetEnemy(D3DXVECTOR3(150.0f, 100.0f, 0.0f), 0);//�G�̈ʒu�̏������B�E�̐����́A�G�̎�ސ�


	//�R�s�[�p=====================================================================================
	//       , D3DXVECTOR3((sinf(-D3DX_PI * 0.5f) * 5.0f), (cosf(-D3DX_PI * 0.5f) * 5.0f), 0.0f)          �G�𓮂���
	//       , D3DXVECTOR3(0.0f,0.0f,0.0f)                                                                �G�𓮂����Ȃ�
	//=============================================================================================



	////WAVE1============================
	///*SetEnemy(D3DXVECTOR3(300.0f, 100.0f, 0.0f),ENEMYTYPE_BOSS1_STARWARRIOR,1000.0f, D3DXVECTOR3((sinf(-D3DX_PI * 0.5f) * 5.0f), (cosf(-D3DX_PI * 0.5f) * 5.0f), 0.0f));*/
	//SetEnemy(D3DXVECTOR3(600.0f, 100.0f, 0.0f),ENEMYTYPE_DELBY,10.0f, D3DXVECTOR3((sinf(-D3DX_PI * 0.0f) * 5.0f), (cosf(-D3DX_PI * 0.0f) * 5.0f), 0.0f));
	//SetEnemy(D3DXVECTOR3(600.0f, 100.0f, 0.0f), ENEMYTYPE_DELBY, 30.0f, D3DXVECTOR3((sinf(-D3DX_PI * 0.5f) * 5.0f), (cosf(-D3DX_PI * 0.5f) * 5.0f), 0.0f));
	////SetEnemy(D3DXVECTOR3(SCREEN_WIDTH/2, 100.0f, 0.0f), ENEMYTYPE_BOSS1_STARWARRIOR,2000.0f, D3DXVECTOR3((sinf(-D3DX_PI * 0.0f) * 0.0f), (cosf(-D3DX_PI * 0.0f) * 0.0f), 0.0f));
	//SetEnemy(D3DXVECTOR3(750.0f, 100.0f, 0.0f), ENEMYTYPE_DELBY, 20.0f, D3DXVECTOR3((sinf(D3DX_PI * 0.5f) * 5.0f), (cosf(D3DX_PI * 0.5f) * 5.0f), 0.0f));
	//SetEnemy(D3DXVECTOR3(750.0f, 100.0f, 0.0f), ENEMYTYPE_DELBY, 20.0f, D3DXVECTOR3((sinf(-D3DX_PI * 0.5f) * 5.0f), (cosf(-D3DX_PI * 0.5f) * 5.0f), 0.0f));
	//////===================================================================================================================
	////TESTWAVE=========================

	//
	//==========================================================================================================================================================

	g_GameState = GAMESTATE_NORMAL;//�ʏ��Ԃɐݒ�
	g_nCounterGameState = 0;
	//�L�[�{�[�h�̏���������
	//InitKeyboard;
}

//==================================
//�Q�[����ʂ̏I������
//==================================
void UninitGame(void)
{
	UninitBG();
	UninitEffect();
	UninitPlayer();
	UninitHealth();
	UninitGage();
	UninitSkillName();
	UninitSkillGage();
	UninitDamage();
	UninitEnemy();
	UninitExplosion();
	UninitWave();
	UninitBullet();
	UninitScore();
	UninitScoreBonus();
	UninitStaging();
	UninitPause();
	UninitParticle();
	UninitFade();
	//InitKeyboard();
}

//=================================
//�Q�[����ʂ̍X�V����
//=================================
void UpdateGame(void)
{

	ResultType* pResultType = GetResultType();

	/*Result* pResult;*/
	int nData;

	int nCnt;

	if (GetKeyboardTrigger(DIK_P) == true||GetJoyPadTrigger(JOYKEY_BACK)==true)
	{//�|�[�Y�L�[�������ꂽ
		g_bPause = g_bPause ? false : true;
	}

	if (g_bPause == false)
	{
		g_nCountPauseMenu = 0;

		g_nCountTime++;//�Q�[���J�n������̎��Ԃ��v������B

	/*	if (g_nCountTime == 1)
		{
			srand((unsigned int)time(NULL));
		}*/

		for (nCnt = 0; nCnt < MAX_ENEMY * 2; nCnt++)
		{
			g_fRandumEnemyPosX[nCnt] = rand() % SCREEN_WIDTH + 1;//0�`100�̒l��ݒ�(�����e�j
			g_fRandumEnemyPosY[nCnt] = rand() % SCREEN_HEIGHT / 2 + 1;
		}

		for (nCnt = 0; nCnt < MAX_WAVE + 1; nCnt++)
		{
			if (g_bScoreBonusTimeCountStart[nCnt] == true)
			{
				g_nScoreBonustime[nCnt]--;//nCnt�E�F�[�u�ڂ̃X�R�A�{�[�i�X�̃J�E���g���X�^�[�g
			}
		}

		/*g_nScoreAdd++;*/
		/*UpDateBG();
		UpdateBullet();
		UpDatePlayer();
		UpdateExplosion();
		UpdateEnemy();
		UpdateScore();*/

		UpDateBG();
		UpdateEffect();
		UpDatePlayer();
		UpdateHealth(0);//�_���[�W���󂯂��񐔁i�O��j�������Ƃ��ēn���B
		UpdateGage();
		UpdateSkillName();
		UpdateSkillGage();
		UpdateDamage();
		UpdateEnemy();
		UpdateExplosion();
		UpdateWave();
		UpdateBullet();
		UpdateScore();
		UpdateScoreBonus();
		UpdateStaging();
		UpdateParticle();
		UpdateFade();

	}

	if (g_bPause == true)
	{//�|�[�Y��
	//�|�[�Y�̍X�V����
		UpdatePause();
		g_nCountPauseMenu++;
		if (g_nCountPauseMenu == 1)
		{//�|�[�Y��ʂ̑I���ʒu�̏����ʒu��ݒ�B
			g_nChoosePauseCommand = 0;
		}
		if (GetKeyboardTrigger(DIK_S) == true||GetJoyPadTrigger(JOYKEY_DOWN)==true)
		{
			g_nChoosePauseCommand += 1;//���ɑI�����ړ�����B
		}
		if (GetKeyboardTrigger(DIK_W) == true||GetJoyPadTrigger(JOYKEY_UP)==true)
		{
			g_nChoosePauseCommand -= 1;//��ɑI�����ړ�����
		}

		if (g_nChoosePauseCommand == -1)
		{//�I���ꏊ�������ֈړ�
			g_nChoosePauseCommand = 2;
		}
		if (g_nChoosePauseCommand == 3)
		{//�I���ꏊ���㑤�ֈړ�
			g_nChoosePauseCommand = 0;
		}

		if (g_nChoosePauseCommand == 0)
		{//�R���e�B�j���[�R�}���h��I��
			SetEnablePause(PAUSE_MENU_CONTINUE);

			if (GetKeyboardTrigger(DIK_RETURN) == true||GetJoyPadTrigger(JOYKEY_START)==true)
			{
				StopSound(SOUND_LABEL_BGM001);
				StopSound(SOUND_LABEL_BGM003);
				g_bPause = false;
			}
		}
		if (g_nChoosePauseCommand == 1)
		{//���g���C�R�}���h��I��
			SetEnablePause(PAUSE_MENU_RETRY);
			if (GetKeyboardTrigger(DIK_RETURN) == true||GetJoyPadTrigger(JOYKEY_START)==true)
			{
				StopSound(SOUND_LABEL_BGM001);
				StopSound(SOUND_LABEL_BGM003);
				SetMode(MODE_GAME);
			}
		}
		if (g_nChoosePauseCommand == 2)
		{//�^�C�g����ʂɖ߂�R�}���h��I��
			SetEnablePause(PAUSE_MENU_QUIT);
			if (GetKeyboardTrigger(DIK_RETURN) == true||GetJoyPadTrigger(JOYKEY_START)==true)
			{
				StopSound(SOUND_LABEL_BGM001);
				StopSound(SOUND_LABEL_BGM003);
				SetMode(MODE_TITLE);
			}
		}
	}

	Enemy *pEnemy=GetEnemy();

	Defeat* pDefeat = GetDefeat();

	


	switch (g_GameState)
	{
	case GAMESTATE_NORMAL://�ʏ���
		break;
	case GAMESTATE_END:
		g_nCounterGameState++;
		if (g_nCounterGameState >= 150)
		{
			
		

			//��ʁi���[�h�j�̐ݒ�
			Result* pResult = GetResult();
			Result_2* pResult_2 = GetResult_2();
			if (pResult->bResultFlag == true||pResult_2->bResultFlag2==true)
			{
				
				pResultType->bResultMode = true;

				StopSound(SOUND_LABEL_BGM001);
				StopSound(SOUND_LABEL_BGM003);





				SetMode(MODE_RESULT);
			}
		}
		break;
	}
	
}

//======================================
//�Q�[����ʂ̕`�揈��
//======================================
void DrawGame(void)
{
	DrawBG();
	DrawEffect();
	DrawPlayer();
	DrawHealth();
	DrawGage();
	DrawSkillName();
	DrawSkillGage();
	DrawDamage();
	DrawEnemy();
	DrawExplosion();
	DrawWave();
	DrawBullet();
	DrawScore();
	DrawScoreBonus();
	DrawStaging();
	DrawParticle();
	DrawFade();
	if (g_bPause == true)
	{//�|�[�Y��
		//�|�[�Y�̕`�揈��
		DrawPause();
	}
}

//============================================
//�Q�[���̏�Ԃ̐ݒ�
//============================================
void SetGameState(GAMESTATE state)
{
	g_GameState = state;
	/*g_nCounterGameState = 0;*/
}

//==============================================
//�Q�[���̏�Ԃ̎擾
//==============================================
GAMESTATE GetGameState(void)
{
	return g_GameState;
}
