//========================================================
//
//�P�O���P�T���F�}�b�v���Ăяo������[game.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#include <stdio.h>
#include <string.h>
#include "game.h"
#include "bg.h"
#include "block.h"
#include "fade.h"
#include "pause.h"
#include "sound.h"
#include "input.h"
#include "player.h"
#include "Item.h"
#include "character.h"
#include "health.h"
#include "Ability.h"
#include "camera.h"
#include "Attack.h"
#include "enemy.h"
#include "damage.h"
#include "explosion.h"
#include "map.h"
#include "warp.h"
#include "bullet.h"
#include "effect.h"
#include "Treasure.h"
#include "Gage.h"
#include "staging.h"
#include "timer.h"
#include "score.h"
#include "main.h"
#include "particle.h"
#include "special.h"
//�O���[�o���ϐ�
GAMESTATE g_GameState = GAMESTATE_NONE;//�Q�[���̏��

int g_nCounterGameState = 0;//��ԊǗ��J�E���^�[

int g_nCountTime;//�Q�[���J�n������o�������Ԃ��Ǘ�����ϐ�

bool g_bPause = false;//�|�[�Y�����ǂ���

int g_nChoosePauseCommand;//�|�[�Y���̎��ɑI�����郁�j���[��I������ϐ�

int g_nCountPauseMenu;//�|�[�Y��ʂ��J���Ă��鎞�Ԃ��J�E���g����ϐ�

PAUSETYPE g_aStopUpdate;//�A�b�v�f�[�g���~�߂���ŁA����̏���������

bool g_bClearPause;//�Q�[���N���A���ɓ���̃A�b�v�f�[�g���~�߂�

bool g_bTimeOver;//���Ԑ؂ꂩ�ǂ����𔻒肷��

bool g_bUseTimer;//�^�C�}�[�̎��Ԃ����炷���ǂ����̃t���O
//int g_nMapBlockData[MAX_MAPHEIGHT][MAX_MAPWIDTH];//�}�b�v�̃u���b�N�̃f�[�^��ۑ�����

typedef struct
{
	float x;
	float y;
	BLOCKTYPE type;
}SetBlockInfo;//�Z�b�g����u���b�N�̏������\����

SetBlockInfo g_aSetBlockInfo[MAX_BLOCK] =
{
	{100,200,BLOCKTYPE_NORMAL},
	{200,300,BLOCKTYPE_NORMAL},
};

//===================================
//�Q�[����ʂ̏���������
//===================================
void InitGame(void)
{
	g_nChoosePauseCommand = 0;

	g_bTimeOver = false;

	g_bUseTimer = true;

	g_nCountPauseMenu = 0;

	g_nCountTime = 0;//���Ԃ̏�����

	g_bPause = false;//�|�[�Y�������

	Player* pPlayer = GetPlayer();

	int nCnt = 0;
	int nCntWidth;//���̃f�[�^��ǂݎ��
	int nCntHeight;//�c��̃f�[�^��ǂݎ��

	
	InitBG();
	InitBlock();
	InitEffect();
	InitParticle();
	InitBullet();
	InitExplosion();
	InitItem();
	InitEnemy();
	InitAttack();
	InitPlayer();
	InitDamage();
	InitStaging();
	InitSpecial();
	InitHealth();
	InitScore();
	InitAbility();
	InitTreasure();
	InitWarp();
	InitGage();
	InitTimer();
	InitMap();
	InitFade();
	InitCamera();
	InitPause();

	g_GameState = GAMESTATE_NORMAL;//�ʏ��Ԃɐݒ�

	g_nCounterGameState = 0;
	//�L�[�{�[�h�̏���������
	//InitKeyboard;

	SetMap(MAPTYPE_8);
	DamageHealth(0);
	/*SetItem(ITEMTYPE_HEAL, D3DXVECTOR3(180.0f, 680.0f, 0.0f),1000);*/
	SetTreasure(TREASURETYPE_BOX, -1, D3DXVECTOR3(1200.0f, 30.0f, 0.0f));
	SetPlayer(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
	//SetItem(ITEMTYPE_REAFORB, D3DXVECTOR3(200.0f, 220.0f, 0.0f));
}


//==================================
//�Q�[����ʂ̏I������
//==================================
void UninitGame(void)
{
	UninitBG();
	UninitBlock();
	UninitEffect();
	UninitParticle();
	UninitBullet();
	UninitExplosion();
	UninitItem();
	UninitEnemy();
	UninitAttack();
	UninitPlayer();
	UninitDamage();
	UninitStaging();
	UninitSpecial();
	UninitHealth();
	UninitScore();
	UninitAbility();
	UninitTreasure();
	UninitWarp();
	UninitGage();
	UninitTimer();
	UninitPause();
	UninitFade();
	UninitCamera();
}

//=================================
//�Q�[����ʂ̍X�V����
//=================================
void UpdateGame(void)
{
	Player* pPlayer = GetPlayer();
	
	g_nCountTime++;

	CharacterNum* pCharacterNum = GetCharacterNum();

	/*Result* pResult;*/
	int nData;

	int nCnt;

	int nRandPos= 0;
	int nRandType = 0;
	int nRandItemPos = 0;

	int nRandItemSet = 0;

	if (g_aStopUpdate == PAUSETYPE_UNLOCK)
	{//�|�[�Y��Ԃ�����
		g_bPause = false;
	}

	if (GetKeyboardTrigger(DIK_P) == true||GetJoyPadTrigger(JOYKEY_BACK)==true)
	{//�|�[�Y�L�[�������ꂽ
		g_bPause = g_bPause ? false : true;
		g_aStopUpdate = PAUSETYPE_PAUSE;
	}
		if (g_bPause == false)
		{
			if (g_bUseTimer == true)
			{
				if (g_nCountTime % 60 == 0)
				{
					SubTimer(1);//�^�C�}�[�̎��Ԃ����炷
				}
			}
			
			UpDateBG();
			UpdateBlock();
			UpdateEffect();
			UpdateParticle();
			UpdateBullet();
			UpdateExplosion();
			UpdateItem();
			UpdateEnemy();
			UpdateAttack();
			UpDatePlayer();
			UpdateDamage();
			UpdateStaging();
			UpdateSpecial();
			UpdateHealth();
			UpdateScore();
			UpdateAbility();
			UpdateTreasure();
			UpdateWarp();
			UpdateGage();
			UpdateTimer();
			UpdateFade();
			UpdateCamera();
			
			

		}
		if (g_bPause == true)
		{//�|�[�Y��
		//�|�[�Y�̍X�V����

			SetEnablePause(g_nChoosePauseCommand);

			UpdatePause();

			if (g_aStopUpdate == PAUSETYPE_PAUSE)
			{
				if (GetKeyboardTrigger(DIK_S) == true || GetJoyPadTrigger(JOYKEY_DOWN) == true)
				{
					g_nChoosePauseCommand++;
				}
				else if (GetKeyboardTrigger(DIK_W) == true || GetJoyPadTrigger(JOYKEY_UP) == true)
				{
					g_nChoosePauseCommand--;
				}

				if (g_nChoosePauseCommand == 4)
				{
					g_nChoosePauseCommand = 1;
				}
				if (g_nChoosePauseCommand == 0)
				{
					g_nChoosePauseCommand = 3;
				}

				if (g_nChoosePauseCommand == 1)
				{
					if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoyPadTrigger(JOYKEY_START) == true)
					{
						g_bPause = false;
					}
				}
				else if (g_nChoosePauseCommand == 2)
				{
					if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoyPadTrigger(JOYKEY_START) == true)
					{
						SetMode(MODE_GAME);
					}
				}
				else if (g_nChoosePauseCommand == 3)
				{
					if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoyPadTrigger(JOYKEY_START) == true)
					{
						SetMode(MODE_TITLE);
					}
				}

				
			}
			else if (g_aStopUpdate == PAUSETYPE_STAGING)
			{//���o�������s��
				UpdateSpecial();
			}
		}
	
		g_bTimeOver = CheckTime();

	//========================
	//�����I���{�^��
	//========================
	if (GetKeyboardTrigger(DIK_E) == true)
	{
		SetGameState(GAMESTATE_END);
	}
	if (g_bTimeOver == true)
	{
		SetGameState(GAMESTATE_END);
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
			StopSound(SOUND_LABEL_BGM000);
			StopSound(SOUND_LABEL_BGM001);
			StopSound(SOUND_LABEL_BGM002);
			StopSound(SOUND_LABEL_BGM003);
			StopSound(SOUND_LABEL_BGM004);
			StopSound(SOUND_LABEL_BGM005);
			StopSound(SOUND_LABEL_BGM006);
			StopSound(SOUND_LABEL_BGM007);
			StopSound(SOUND_LABEL_BGM008);
			StopSound(SOUND_LABEL_BGM009);
			StopSound(SOUND_LABEL_BGM010);
			StopSound(SOUND_LABEL_BGM011);

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
	DrawBlock();
	DrawEffect();
	DrawParticle();
	DrawBullet();
	DrawExplosion();
	DrawItem();
	DrawEnemy();
	DrawAttack();
	DrawPlayer();
	DrawDamage();
	DrawStaging();
	DrawHealth();
	DrawScore();
	DrawAbility();
	DrawTreasure();
	DrawWarp();
	DrawGage();
	DrawTimer();
	DrawFade();
	DrawCamera();
	if (g_bPause == true)
	{//�|�[�Y��
		//�|�[�Y�̕`�揈��
	
			if (g_aStopUpdate == PAUSETYPE_PAUSE)
			{
				DrawPause();
			}
			else if (g_aStopUpdate == PAUSETYPE_STAGING)
			{//���o�������s��
				DrawSpecial();
			}
	}
}

//============================================
//�Q�[���̏�Ԃ̐ݒ�
//============================================
void SetGameState(GAMESTATE state)
{
	g_GameState = state;
	
}

//============================================
//�|�[�Y�ݒ菈��
void SetPause(PAUSETYPE type)
{
	g_aStopUpdate = type;
	g_bPause = true;
}

//==============================================
//�Q�[���̏�Ԃ̎擾
//==============================================
GAMESTATE GetGameState(void)
{
	return g_GameState;
}

//==================================
//�^�C�}�[�̎��Ԃ����炷���ǂ���
//==================================
bool* GetUseTimer(void)
{
	return &g_bUseTimer;
}

