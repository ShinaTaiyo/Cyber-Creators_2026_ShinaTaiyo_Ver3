//========================================================
//
// �Q�[����ʂ̏���
// Author : ShinaTaiyo
//
//========================================================
#include "main.h"
#include "input.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "tutorial.h"
#include "sound.h"
#include "fade.h"
#include <time.h>
#include "ranking.h"
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
#include "edit.h"
#include "enemy.h"
#include "timer.h"
#include "score.h"
#include "Summon.h"
#include "pause.h"
#include "item.h"
#include "editcamera.h"
#include "editEnemy.h"
#include "scrap.h"
#include "debugproc.h"
#include "gauge.h"
#include "level.h"
#include "battery.h"
#include "bossgauge.h"
#include "enemygauge.h"
#include "attackmodel.h"
#include "boss.h"
#include "attackbillboard.h"
#include "ceiling.h"
#include "mission.h"
#include "missioncheck.h"
#include "UI.h"
#include "Rader.h"
#include "operate.h"
#include "Warning.h"
#include "combo.h"

//=======================================================================================================================================================================================================
// �O���[�o���ϐ�
//=======================================================================================================================================================================================================
GAMESTATE g_GameState = GAMESTATE_NONE;	//�Q�[���̏��
int g_nCountTime;						//�Q�[���J�n������o�������Ԃ��Ǘ�����ϐ�
int g_nChoosePauseCommand;				//�|�[�Y���j���[�̑I������
int g_nCntEnemy = 0;					//�G�̃p�[�c��
bool g_bPause = false;					//�|�[�Y�����ǂ���
bool g_bHouse = false;					//���у��{�n�E�X�����ǂ���
bool g_bEdit;							//�G�f�B�^�����ǂ���
bool g_bEditEnemy;						//�G�G�f�B�^�����ǂ���
//=======================================================================================================================================================================================================
// ����������
//=======================================================================================================================================================================================================
void InitGame(void)
{
	//�ϐ�
	g_nChoosePauseCommand = 0;		//�|�[�Y���j���[�̑I������
	g_nCountTime = 0;				//���Ԃ̏�����
	g_bPause = false;				//�|�[�Y�������
	g_bEdit = false;				//�G�f�B�^�������
	g_bEditEnemy = false;			//�G�G�f�B�^�������
	g_GameState = GAMESTATE_NORMAL;	//�ʏ��Ԃɐݒ�

	InitPause();			//�|�[�Y
	InitSummon();			//�G����
	InitShadow();			//�e
	InitField();			//�n��
	InitStagingField();     //���o�t�B�[���h
	InitItem();				//�A�C�e��
	InitPlayer();           //�v���C���[
	LoadPlayer();			//�v���C���[�̃p�[�c���̓ǂݍ���	
	LoadWeapon();			//�v���C���[�̕���̓ǂݍ���
	InitScrap();			//�X�N���b�v
	InitBullet();			//�o���b�g
	InitAttackbillboard();	//�U���r���{�[�h
	InitWarning();          //�x���\��
	InitEffect();			//�G�t�F�N�g
	InitParticle();			//�p�[�e�B�N��
	InitEmitter();			//�G�~�b�^�[
	InitEdit();				//���f���̃G�f�B�b�g
	InitEnemyEditor();      //�G�̃G�f�B�b�g
	InitCamera();			//�J����
	InitLight();			//���C�g
	InitEditCamera();		//�G�f�B�^�p�J����
	InitWall();				//��
	InitModel();            //���f��
	InitOutletDisp();       //�[�d��ʒu�\��
	InitAttackModel();		//�U�����f��
	InitEnemyGauge();		//�G�̗̑̓Q�[�W
	InitEnemyWeakDisp();    //�G�̎�_�\�L
	InitEnemy();			//�G
	InitBoss();				//�{�X
	InitCeiling();			//�V��

	//UI
	InitTimer();			//�^�C�}�[
	InitScore();			//�X�R�A
	InitGauge();			//�v���C���[�̊e�X�e�[�^�X���x���Q�[�W
	InitLevel();			//�v���C���[�̊e�X�e�[�^�X���x��
	InitLevelUpDisplay();	//�v���C���[�̃��x���A�b�v�\��
	InitLevelUpDisplay2D();	//�v���C���[�̃��x���A�b�v�\��
	InitCandyDisp();        //�L�����f�B�\��
	InitCandyNum();         //�L�����f�B���l
	InitBattery();			//�v���C���[�̗̑�
	InitMission();			//�~�b�V����
	InitBossGauge();		//�{�X�̗̑̓Q�[�W
	InitMissionCheck();		//�~�b�V�����̃`�F�b�N
	InitGraspBoss();        //�{�X�ʒu�c���\��
	InitRaderCom();			//���[�_�[
	InitUI();				//UI
	InitCombo();			//�R���{��

	ResetPlayerPos();	//�v���C���[�̈ʒu���Z�b�g

	//���̓ǂݍ���
	LoadEnemy();	//�G���̃p�[�c���̓ǂݍ���
	LoadModeltxt();	//���f�����̓ǂݍ���(txt)

//#ifdef _DEBUG	//Debug
//	LoadModeltxt();	//���f�����̓ǂݍ���(txt)
//#else			//Release
//	LoadModelbin();	//���f�����̓ǂݍ���(bin)
//#endif

	PlaySound(SOUND_LABEL_BGM013);
}

//=======================================================================================================================================================================================================
// �I������
//=======================================================================================================================================================================================================
void UninitGame(void)
{

		//�Q�[���I�u�W�F�N�g
	    UninitLight();			//���C�g
		UninitCeiling();		//�V��
		UninitField();		    //�t�B�[���h
		UninitStagingField();   //���o�t�B�[���h
		UninitWall();			//��
		UninitEnemy();		    //�G
		UninitBoss();			//�{�X	
		UninitEmitter();		//�G�~�b�^�[
		UninitEnemyGauge();	    //�G�̗̑̓Q�[�W
		UninitEnemyWeakDisp();  //�G�̎�_�\�L
		UninitEditCamera();     //�G�f�B�^�p�J�����̐ݒ�
		UninitEdit();		    //�G�f�B�^
		//�Q�[���I�u�W�F�N�g
	    UninitCamera();			//�Q�[���J�����̐ݒ�
	    UninitAttackbillboard();//�U���r���{�[�h
	    UninitParticle();		//�p�[�e�B�N��
	    UninitBullet();			//�e
	    UninitOutletDisp();     //�[�d��ʒu�\��
	    UninitScrap();			//�X�N���b�v
	    UninitItem();			//�A�C�e��
	    UninitModel();			//���f��
	    UninitPlayer();			//�v���C���[
	    UninitAttackModel();	//�U�����f��
		UninitWarning();        //�x���\��
	    UninitShadow();			//�e
	    UninitEffect();			//�G�t�F�N�g
	    UninitEnemyEditor();        //�G�G�f�B�^
 	    UninitSummon();             //���� 

		//UI
		UninitCandyDisp();			//�L�����f�B�\��
		UninitCandyNum();			//�L�����f�B���l
		UninitGauge();				//�v���C���[�̊e�X�e�[�^�X���x���Q�[�W
		UninitLevel();				//�v���C���[�̊e�X�e�[�^�X���x��
		UninitLevelUpDisplay();		//���x���A�b�v�\��
		UninitLevelUpDisplay2D();	//���x���A�b�v�\��
		UninitBattery();			//�v���C���[�̗̑�
		UninitMission();			//�~�b�V����
		UninitMissionCheck();		//�~�b�V�����̃`�F�b�N
		UninitBossGauge();			//�{�X�̗̑̓Q�[�W
		UninitRaderCom();			//���[�_�[
		UninitGraspBoss();			//�{�X�ʒu�c���\��
		UninitUI();					//UI
		UninitCombo();              //�R���{
		UninitTimer();				//�^�C�}�[
		UninitPause();				//�|�[�Y���
}

//=======================================================================================================================================================================================================
// �X�V����
//=======================================================================================================================================================================================================
void UpdateGame(void)
{
	bool bTimeUpFlag = false;
	bTimeUpFlag = CheckTime();	//���Ԑ؂ꂩ�ǂ����̔�����擾
	Camera* pCamera = GetCamera();
	EnemyEditor* pEnemyEditor = GetEnemyEditor();
	if (GetTimer() <= 0)
	{//�^�C�}�[���O�ɂȂ�����A�Q�[�����I��������B
		/*g_GameState = GAMESTATE_END;*/
		SetFade(MODE_RESULT);
		StopSound(SOUND_LABEL_BGM013);
		StopSound(SOUND_LABEL_BGM015);

	}

	if (GetKeyboardTrigger(DIK_P) || GetJoypadTrigger(JOYKEY_START))
	{//�|�[�Y�L�[�������ꂽ
		g_bPause = g_bPause ? false : true;	//�|�[�Y��ʕ\���E��\���̐؂�ւ�
	}

	//�|�[�Y���ł͂Ȃ��ꍇ
	if (g_bPause == false)
	{
		g_nCountTime++;//�Q�[���J�n������̎��Ԃ��v������B

#ifdef _DEBUG
		if (GetKeyboardTrigger(DIK_F4) == true)
		{//F4�L�[�������ꂽ
			//�I�u�W�F�N�g�G�f�B�^�؂�ւ�
			g_bEdit = g_bEdit ? false : true;
		}
		if (GetKeyboardTrigger(DIK_F5) == true)
		{//F5�L�[�������ꂽ
			//�G�l�~�[�G�f�B�^�؂�ւ�
			g_bEditEnemy = g_bEditEnemy ? false : true;

			if (g_bEditEnemy == true)
			{
				pEnemyEditor->nIdxRaderDisp = SetRaderDisplay(RADERDISPLAY04_EDIT);
			}
			else if (g_bEditEnemy == false)
			{
				KillRaderDisplay(pEnemyEditor->nIdxRaderDisp);
			}
		}
		if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoypadTrigger(JOYKEY_BACK) == true)
		{//Enter�L�[ or START�������ꂽ�ꍇ
			//���U���g��ʂ�
			SetFade(MODE_RESULT);
		}
#endif // _DEBUG

		UpdateField();	//�t�B�[���h
		UpdateStagingField();//���o�t�B�[���h
		UpdateWall();	//��
		UpdateLight();	//���C�g

		//�Q�[���t�B�[���h
		if (g_bHouse == false)
		{
			//�Q�[����
			if (g_bEdit == false)
			{
				if (g_bEditEnemy == false)
				{
					g_nCountTime++;//�Q�[���J�n������̎��Ԃ��v������B

					UpdateCamera();	//�J����

					//�Q�[���I�u�W�F�N�g
					UpdateModel();			//���f��
					UpdateAttackModel();	//�U�����f��
					UpdateBossGauge();		//�{�X�̗̑̓Q�[�W
					UpdateItem();			//�A�C�e��
					UpdateScrap();			//�X�N���b�v
					UpdateBullet();			//�e
					UpdateAttackbillboard();//�U���r���{�[�h
					UpdateWarning();        //�x���\��
					UpdateShadow();			//�e
					UpdateEffect();			//�G�t�F�N�g
					UpdateParticle();		//�p�[�e�B�N��
					UpdateEmitter();		//�G�~�b�^�[
					UpdateEnemyGauge();		//�G�̗̑̓Q�[�W
					UpdateEnemyWeakDisp();  //�G�̎�_�\�L
					UpdateOutletDisp();     //�[�d��ʒu�\��
					UpdatePlayer();	        //�v���C���[
					UpdateBoss();			//�{�X
					
					if (pCamera->nMode == CAMERAMODE_REVOLUTION)
					{
						UpdateSummon();	//�G����

						//UI
						UpdateEnemy();	//�G(���Վ��_�̎��͓����Ȃ��j
						UpdateTimer();	//�G(���Վ��_�̎��͓����Ȃ��j
					}

					UpdateCandyDisp();      //�L�����f�B�\��
					UpdateCandyNum();       //�L�����f�B���l
					UpdateScore();			//�X�R�A
					UpdateGauge();			//�v���C���[�̊e�X�e�[�^�X���x���Q�[�W
					UpdateLevel();			//�v���C���[�̊e�X�e�[�^�X���x��
					UpdateLevelUpDisplay(); //���x���A�b�v�\��
					UpdateLevelUpDisplay2D(); //���x���A�b�v�\��
					UpdateMission();		//�~�b�V����
					UpdateMissionCheck();	//�~�b�V�����̃`�F�b�N
					UpdateBossGauge();		//�{�X�̗̑̓Q�[�W
					UpdateBattery();		//�v���C���[�̗̑�
					UpdateRaderCom();       //���[�_�[
					UpdateGraspBoss();      //�{�X�ʒu�c���\��
					UpdateUI();				//�Q�[�����UI
					UpdateCombo();			//�R���{��
				}
			}

			//�G�f�B�^��
			else
			{
				if (GetKeyboardTrigger(DIK_F6) == true)
				{//F6�L�[�������ꂽ
					//�X�e�[�W�؂�ւ�����
					ChangeStage();
				}

				UpdateEdit();		//���f���G�f�B�^
				UpdateEditCamera();	//�G�f�B�^�p�J����
			}

			if (g_bEditEnemy == true)
			{
				UpdateCamera();
				UpdateEnemyEditor();
				SetPositionRaderDisPlay(pEnemyEditor->nIdxRaderDisp, pEnemyEditor->pos, 0.0f);
				UpdateRaderCom();       //���[�_�[(�G�G�f�B�^�̎�������)

			}
		}

		//�`���[�g���A���t�B�[���h
		else
		{
			//�Q�[����
			if (g_bEdit == false)
			{
				g_nCountTime++;//�Q�[���J�n������̎��Ԃ��v������B

				//�Q�[���I�u�W�F�N�g
				UpdateCamera();	//�Q�[���J����
				UpdateModel();	//���f��
				UpdatePlayer();	//�v���C���[
				UpdateShadow();	//�e

				//UI
				UpdateTimer();	//�^�C�}�[
				UpdateScore();	//�X�R�A
			}

			//�G�f�B�^��
			else
			{
				UpdateEdit();		//�G�f�B�^
				UpdateEditCamera();	//�G�f�B�^�p�J����
			}
		}
	}

	//�|�[�Y���̏ꍇ
	else
	{
		UpdatePause();	//�|�[�Y���
		if (GetKeyboardTrigger(DIK_S) == true || GetJoypadTrigger(JOYKEY_DOWN) == true)
		{
			g_nChoosePauseCommand++;//���ɑI�����ړ�����B
		}
		if (GetKeyboardTrigger(DIK_W) == true || GetJoypadTrigger(JOYKEY_UP) == true)
		{
			g_nChoosePauseCommand--;//��ɑI�����ړ�����
		}

		if (g_nChoosePauseCommand < 0)
		{//�I���ꏊ�������ֈړ�
			g_nChoosePauseCommand = 2;
		}
		if (g_nChoosePauseCommand > 2)
		{//�I���ꏊ���㑤�ֈړ�
			g_nChoosePauseCommand = 0;
		}

		if (g_nChoosePauseCommand == PAUSE_MENU_CONTINUE)
		{//�R���e�B�j���[�R�}���h��I��
			SetEnablePause(PAUSE_MENU_CONTINUE);

			if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoypadTrigger(JOYKEY_A) == true)
			{
				g_bPause = false;
			/*	StopSound(SOUND_LABEL_BGM004);*/

				StopSound(SOUND_LABEL_BGM013);
				StopSound(SOUND_LABEL_BGM015);
			}
		}
		else if (g_nChoosePauseCommand == PAUSE_MENU_RETRY)
		{//���g���C�R�}���h��I��
			SetEnablePause(PAUSE_MENU_RETRY);
			if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoypadTrigger(JOYKEY_A) == true)
			{
				SetFade(MODE_GAME);
				StopSound(SOUND_LABEL_BGM013);
				StopSound(SOUND_LABEL_BGM015);
			}
		}
		else if (g_nChoosePauseCommand == PAUSE_MENU_QUIT)
		{//�^�C�g����ʂɖ߂�R�}���h��I��
			SetEnablePause(PAUSE_MENU_QUIT);
			if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoypadTrigger(JOYKEY_A) == true)
			{
				SetFade(MODE_TITLE);
				StopSound(SOUND_LABEL_BGM013);
				StopSound(SOUND_LABEL_BGM015);
			}
		}
	}
}

//=======================================================================================================================================================================================================
// �`�揈��
//=======================================================================================================================================================================================================
void DrawGame(void)
{
	//�Q�[���t�B�[���h
	if (g_bHouse == false)
	{
		//�G�f�B�^��
		if (g_bEdit == true)
		{
			//�Q�[���I�u�W�F�N�g
			DrawCeiling();		//�V��
			DrawField();		//�t�B�[���h
			DrawStagingField(); //���o�t�B�[���h
			DrawWall();			//��
			DrawEnemy();		//�G
			DrawBoss();			//�{�X	
			DrawEmitter();		//�G�~�b�^�[
			DrawEnemyGauge();	//�G�̗̑̓Q�[�W
			DrawEnemyWeakDisp();//�G�̎�_�\�L
			SetEditCamera();//�G�f�B�^�p�J�����̐ݒ�
			DrawEdit();		//�G�f�B�^
		}
		//�Q�[����
		else
		{
			//�Q�[���I�u�W�F�N�g
			DrawCeiling();			//�V��
			DrawField();			//�t�B�[���h
			DrawStagingField();		//���o�t�B�[���h
			DrawWall();				//��
			SetCamera();			//�Q�[���J�����̐ݒ�
			DrawAttackbillboard();	//�U���r���{�[�h
			DrawParticle();			//�p�[�e�B�N��
			DrawEnemyGauge();		//�G�̗̑̓Q�[�W
			DrawEnemyWeakDisp();	//�G�̎�_�\�L
			DrawBullet();			//�e
			DrawOutletDisp();       //�[�d��ʒu�\��
			DrawScrap();			//�X�N���b�v
			DrawItem();				//�A�C�e��
			DrawModel();			//���f��
			DrawPlayer();			//�v���C���[
			DrawAttackModel();		//�U�����f��
			DrawWarning();          //�x���\��
			DrawEnemy();			//�G
			DrawBoss();				//�{�X	
			DrawShadow();			//�e
			DrawEffect();			//�G�t�F�N�g
			
			//UI
			DrawCandyDisp();		//�L�����f�B�\��
			DrawCandyNum();			//�L�����f�B���l
			DrawGauge();			//�v���C���[�̊e�X�e�[�^�X���x���Q�[�W
			DrawLevel();			//�v���C���[�̊e�X�e�[�^�X���x��
			DrawLevelUpDisplay();	//���x���A�b�v�\��
			DrawLevelUpDisplay2D();	//���x���A�b�v�\��

			DrawBattery();			//�v���C���[�̗̑�
			DrawMission();			//�~�b�V����
			DrawMissionCheck();		//�~�b�V�����̃`�F�b�N
			DrawBossGauge();		//�{�X�̗̑̓Q�[�W
			DrawRaderCom();			//���[�_�[
			DrawGraspBoss();		//�{�X�ʒu�c���\��
			DrawComboUI();			//�R���{UI
			DrawComboNum();			//�R���{�{��
			DrawUI();				//UI
			DrawTimer();			//�^�C�}�[
			DrawScore();			//�X�R�A

	
			//===========================
			//�G�G�f�B�^���g��
			//===========================
			if (g_bEditEnemy == true)
			{
				DrawEnemyEditor();
				DrawSummon();
			}

			//===============================================================
	
		}
	}

	//���у��{�n�E�X
	else
	{
		//�Q�[���I�u�W�F�N�g
		DrawField();	//�t�B�[���h
		DrawWall();		//��
		DrawPlayer();	//�v���C���[

		//�G�f�B�^��
		if (g_bEdit == true)
		{
			SetEditCamera();//�G�f�B�^�p�J�����̐ݒ�
			DrawEdit();		//�G�f�B�^
		}
		//�Q�[����
		else
		{
			SetCamera();//�Q�[���p�J�����̐ݒ�
			DrawModel();	//���f��

			//UI
			DrawTimer();//�^�C�}�[
			DrawScore();//�X�R�A
		}
	}

	//�|�[�Y��
	if (g_bPause == true)
	{
		DrawPause();//�|�[�Y���
	}

}

//=======================================================================================================================================================================================================
// �X�e�[�W�؂�ւ�
//=======================================================================================================================================================================================================
void ChangeStage(void)
{
	g_bHouse = g_bHouse ? false : true;
}

//=======================================================================================================================================================================================================
// �Q�[���̏�Ԃ̐ݒ�
//=======================================================================================================================================================================================================
void SetGameState(GAMESTATE state)
{
	g_GameState = state;
}

//=======================================================================================================================================================================================================
// �Q�[���̏�Ԃ̎擾
//=======================================================================================================================================================================================================
GAMESTATE GetGameState(void)
{
	return g_GameState;
}

//=======================================================================================================================================================================================================
// �G�f�B�^�����ǂ����̔���̎擾
//=======================================================================================================================================================================================================
bool GetEditMode(void)
{
	return g_bEdit;
}

//=======================================================================================================================================================================================================
// ���у��{�n�E�X�����ǂ����̔���̎擾
//=======================================================================================================================================================================================================
bool GetHouse(void)
{
	return g_bHouse;
}

//=======================================================================================================================================================================================================
// �|�[�Y�����ǂ����̔�����擾
//=======================================================================================================================================================================================================
bool GetPause(void)
{
	return g_bPause;
}

//=======================================================================================================================================================================================================
// �G�G�f�B�^�����ǂ����̔���̎擾
//=======================================================================================================================================================================================================
bool GetEditEnemyMode(void)
{
	return g_bEditEnemy;
}
