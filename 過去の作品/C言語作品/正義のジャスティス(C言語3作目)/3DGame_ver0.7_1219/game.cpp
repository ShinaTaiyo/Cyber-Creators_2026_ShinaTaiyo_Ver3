//========================================================
//
//�W���R�O���F�G�t�F�N�g������񌩒���[game.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#include "main.h"
#include "input.h"
#include "bg.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "tutorial.h"
#include "sound.h"
#include "fade.h"
#include <time.h>
#include "ranking.h"
#include "polygon.h"
#include "camera.h"
#include "light.h"
#include "field.h"
#include "wall.h"
#include "model.h"
#include "player.h"
#include "shadow.h"
#include <stdio.h>
#include "billboard.h"
#include "bullet.h"
#include "effect.h"
#include "emitter.h"
#include "meshfield.h"
#include "particle.h"
#include "meshcylinder.h"
#include "Block.h"
#include "edit.h"
#include "motion.h"
#include "enemy.h"
#include "editEnemy.h"
#include "AttackModel.h"
#include "timer.h"
#include "score.h"
#include "damage.h"
#include "SkillGage.h"
#include "SkillName.h"
#include "AttackBillBoard.h"
#include "health.h"
#include "combo.h"
#include "Summon.h"
#include "pause.h"
#include "item.h"
//�O���[�o���ϐ�
GAMESTATE g_GameState = GAMESTATE_NONE;//�Q�[���̏��

int g_nCounterGameState = 0;//��ԊǗ��J�E���^�[

int g_nCountTime;//�Q�[���J�n������o�������Ԃ��Ǘ�����ϐ�

bool g_bPause = false;//�|�[�Y�����ǂ���

int g_nChoosePauseCommand;//�|�[�Y���̎��ɑI�����郁�j���[��I������ϐ�

int g_nCountPauseMenu;//�|�[�Y��ʂ��J���Ă��鎞�Ԃ��J�E���g����ϐ�

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
	/*InitBG();
	InitFade();*/

	InitPause();
	//�G�����p
	InitSummon();

	InitShadow();
	InitField();
	//InitMeshField();
	//InitMeshCylinder();
	InitDamage();
	InitWall();
	InitModel();
	//InitBlock();
	InitEnemy();
	InitItem();
	InitPlayer();
	//InitBillboard();
	InitBullet();
	InitAttackModel();
	InitEffect();
	InitAttackBillBoard();
	InitParticle();
	InitEmitter();
	InitEdit();
	InitEditEnemy();
	InitMotion();
	InitCamera();
	InitLight();

	//UI
	InitTimer();
	InitScore();
	InitSkillGage();
	InitSkillName();
	InitHealth();
	InitCombo();
	InitFade();

	g_GameState = GAMESTATE_NORMAL;//�ʏ��Ԃɐݒ�

	g_nCounterGameState = 0;
	//�L�[�{�[�h�̏���������
	//InitKeyboard;

	PlaySound(SOUND_LABEL_BGM004);
}

//==================================
//�Q�[����ʂ̏I������
//==================================
void UninitGame(void)
{
	//UninitBG();
	//UninitFade();

	UninitPause();
	UninitShadow();
	UninitField();
	//UninitMeshField();
	//UninitMeshCylinder();
	UninitWall();
	UninitDamage();
	UninitModel();
	//UninitBlock();
	UninitEnemy();
	UninitItem();
	UninitPlayer();
	//UninitBillboard();
	UninitBullet();
	UninitAttackModel();
	UninitEffect();
	UninitAttackBillBoard();
	UninitParticle();
	UninitEmitter();
	UninitEdit();
	UninitEditEnemy();
	//UninitMotion();
	UninitCamera();
	UninitLight();

	//UI
	UninitTimer();
	UninitScore();
	UninitSkillGage();
	UninitSkillName();
	UninitHealth();
	UninitCombo();
	UninitFade();

}

//=================================
//�Q�[����ʂ̍X�V����
//=================================
void UpdateGame(void)
{

	

	/*Result* pResult;*/
	int nData;

	int nCnt;

	bool bTimeUpFlag = false;


	if (GetKeyboardTrigger(DIK_P) == true||GetJoyPadTrigger(JOYKEY_BACK)==true)
	{//�|�[�Y�L�[�������ꂽ
		g_bPause = g_bPause ? false : true;
	}

	if (g_bPause == false)
	{
		g_nCountPauseMenu = 0;

		g_nCountTime++;//�Q�[���J�n������̎��Ԃ��v������B

		

	/*	UpDateBG();
		UpdateFade();*/

		//�G����
		UpdateSummon();

		//�Q�[��
		UpdateField();
		//UpdateMeshField();
		//UpdateMeshCylinder();
		UpdateWall();
		UpdateDamage();
		UpdateModel();
		//UpdateBlock();
		UpdateEnemy();
		UpdateItem();
		UpdatePlayer();
		UpdateShadow();
		//UpdateBillboard();
		UpdateBullet();
		UpdateAttackModel();
		UpdateEffect();
		UpdateAttackBillBoard();
		UpdateParticle();
		UpdateEmitter();
		UpdateEdit();
		UpdateEditEnemy();
		UpdateMotion();
		UpdateCamera();
		UpdateLight();

		//UI
		UpdateTimer();
		UpdateScore();
		UpdateSkillGage();
		UpdateSkillName();
		UpdateHealth();
		UpdateCombo();
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
		if (GetKeyboardTrigger(DIK_S) == true || GetJoyPadTrigger(JOYKEY_DOWN) == true)
		{
			StopSound(SOUND_LABEL_SE_SELECT_000);
			g_nChoosePauseCommand += 1;//���ɑI�����ړ�����B
		}
		if (GetKeyboardTrigger(DIK_W) == true || GetJoyPadTrigger(JOYKEY_UP) == true)
		{
			StopSound(SOUND_LABEL_SE_SELECT_000);
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

			if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoyPadTrigger(JOYKEY_START) == true)
			{
				g_bPause = false;
			/*	StopSound(SOUND_LABEL_BGM004);*/
			}
		}
		if (g_nChoosePauseCommand == 1)
		{//���g���C�R�}���h��I��
			SetEnablePause(PAUSE_MENU_RETRY);
			if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoyPadTrigger(JOYKEY_START) == true)
			{
				SetMode(MODE_GAME);
				StopSound(SOUND_LABEL_BGM004);
			}
		}
		if (g_nChoosePauseCommand == 2)
		{//�^�C�g����ʂɖ߂�R�}���h��I��
			SetEnablePause(PAUSE_MENU_QUIT);
			if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoyPadTrigger(JOYKEY_START) == true)
			{
				SetMode(MODE_TITLE);
				StopSound(SOUND_LABEL_BGM004);
			}
		}
	}

	bTimeUpFlag = CheckTime();

	if (bTimeUpFlag == true)
	{//�^�C�}�[���O�ɂȂ�����A�Q�[�����I��������B
		g_GameState = GAMESTATE_END;
	}
	//���̑�
	bool bDeathFlag = bPlayerDeath();
	if (bDeathFlag == true)
	{
		g_GameState = GAMESTATE_END;
	}

	if (GetKeyboardTrigger(DIK_0) == true /*|| GetJoyPadTrigger(JOYKEY_START) == true*/)
	{//�Q�[�������I���{�^��
		g_GameState = GAMESTATE_END;
	}

	switch (g_GameState)
	{
	case GAMESTATE_NORMAL://�ʏ���
		break;
	case GAMESTATE_END:
		g_nCounterGameState++;
		if (g_nCounterGameState >= 150)
		{
			StopSound(SOUND_LABEL_BGM004);
				SetFade(MODE_RESULT);
		}
		break;
	}
	
}

//======================================
//�Q�[����ʂ̕`�揈��
//======================================
void DrawGame(void)
{
	//DrawBG();
	if (g_bPause == true)
	{//�|�[�Y��
		//�|�[�Y�̕`�揈��
		DrawPause();
	}

	DrawField();
	//DrawMeshField();
	//DrawMeshCylinder();
	DrawWall();
	DrawDamage();
	DrawModel();
	//DrawBlock();
	DrawEnemy();
	DrawItem();
	DrawPlayer();
	DrawShadow();
	DrawEmitter();
	//DrawBillboard();
	DrawBullet();
	DrawAttackModel();
	DrawEffect();
	DrawAttackBillBoard();
	DrawParticle();
	DrawEdit();
	DrawEditEnemy();
	SetCamera();

    //UI
	DrawTimer();
	DrawScore();
	DrawSkillGage();
	DrawSkillName();
	DrawHealth();
	DrawCombo();
	DrawFade();

}

//============================================
//�Q�[���̏�Ԃ̐ݒ�
//============================================
void SetGameState(GAMESTATE state)
{
	g_GameState = state;
	
}

//==============================================
//�Q�[���̏�Ԃ̎擾
//==============================================
GAMESTATE GetGameState(void)
{
	return g_GameState;
}
