//=======================================================================================================================================================================================================================
//
// �`���[�g���A����ʂ̏���
// Author : Atsumu Kuboichi
//
//=======================================================================================================================================================================================================================
#include "tutorial.h"
#include "field.h"
#include "camera.h"
#include "wall.h"
#include "player.h"
#include "shadow.h"
#include "input.h"
#include "fade.h"
#include "attackmodel.h"
#include "bullet.h"
#include "operate.h"
#include "check.h"
#include "sound.h"
#include "model.h"
#include "rule.h"
#include "dummy.h"
#include "billboard.h"
#include "particle.h"
#include "effect.h"
#include "light.h"

//=======================================================================================================================================================================================================================
// �O���[�o���ϐ�
//=======================================================================================================================================================================================================================
bool bLoad = false;	//�}�b�v��񂪍X�V�ς݂��ǂ���

//=======================================================================================================================================================================================================================
// ����������
//=======================================================================================================================================================================================================================
void InitTutorial(void)
{
	InitShadow();		//�e
	InitCamera();		//�J����
	InitLight();		//���C�g
	InitField();        //�t�B�[���h
	InitWall();         //��
	InitModel();        //���f��
	LoadModeltxt();		//���f�����̓ǂݍ���(txt)

	InitPlayer();       //�v���C���[
	LoadPlayer();		//�v���C���[�̃p�[�c���̓ǂݍ���	
	LoadWeapon();		//�v���C���[�̕���̓ǂݍ���
	InitAttackModel();	//�U�����o
	InitBullet();		//�e
	InitOperate();		//�`���[�g���A���pUI
	InitDummy();		//�_�~�[�l�`
	InitParticle();     //�p�[�e�B�N��
	InitBillboard();	//�r���{�[�h
	InitEffect();		//�G�t�F�N�g
	InitRule();			//���[��������ʂ̏���������
	ResetPlayerPos();	//�v���C���[�̈ʒu���Z�b�g

	//���f���̍\���̏����擾
	Model* pModel = GetModel();

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++, pModel++)
	{
		if (pModel->nUse == 1)
		{//�@�B�̏�ɐݒ�
			SetBillboard(D3DXVECTOR3(pModel->pos.x, pModel->vtxMax.y, pModel->pos.z), pModel->nUse, 0, 200.0f, 50.0f);//�r���{�[�h�̐ݒ�
		}
	}

	PlaySound(SOUND_LABEL_BGM012);
}

//=======================================================================================================================================================================================================================
// �I������
//=======================================================================================================================================================================================================================
void UninitTutorial(void)
{
	UninitShadow();		//�e
	UninitCamera();		//�J����
	UninitField();        //�t�B�[���h
	UninitWall();         //��
	UninitModel();        //���f��
	UninitPlayer();       //�v���C���[
	UninitAttackModel();	//�U�����o
	UninitBullet();		//�e
	UninitOperate();		//�`���[�g���A���pUI
	UninitDummy();		//�_�~�[�l�`
	UninitParticle();     //�p�[�e�B�N��
	UninitBillboard();	//�r���{�[�h
	UninitEffect();		//�G�t�F�N�g
	UninitRule();			//���[��������ʂ̏���������
}

//=======================================================================================================================================================================================================================
// �X�V����
//=======================================================================================================================================================================================================================
void UpdateTutorial(void)
{
	Model* pModel = GetModel();			//���f���̍\���̂̏����擾
	Player* pPlayer = GetPlayer();		//�v���C���[�̍\���̂̏����擾
	Rule* pRuleScreen = GetRuleScreen();//���[��������ʂ̍\���̂̏��̎擾

	if (bLoad)
	{//�}�b�v��񂪍X�V����Ă���ꍇ
		if (!pRuleScreen->bUse)
		{//���[��������ʂ��g�p���Ă��Ȃ��ꍇ
			UpdateCamera();		//�J����
			UpdateField();		//�t�B�[���h
			UpdateWall();		//��
			UpdatePlayer();		//�v���C���[
			UpdateShadow();		//�e
			UpdateAttackModel();//�U�����o
			UpdateBullet();		//�e
			UpdateOperate();	//�`���[�g���A���pUI
			UpdateModel();		//���f��
			UpdateDummy();		//�_�~�[�l�`
			UpdateBillboard();	//�r���{�[�h
			UpdateParticle();   //�p�[�e�B�N��
			UpdateEffect();		//�G�t�F�N�g

			for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++, pModel++)
			{
				if (pPlayer->pos.x >= pModel->pos.x + pModel->vtxMin.x && pPlayer->pos.x <= pModel->pos.x + pModel->vtxMax.x
					&& pPlayer->pos.z >= pModel->pos.z + (pModel->vtxMin.z - 50.0f) && pPlayer->pos.z <= pModel->pos.z + pModel->vtxMax.z
					&& pModel->nType == 23 && GetJoypadTrigger(JOYKEY_A))
				{//�v���C���[���@�B�̑O��A�{�^���������ꂽ�ꍇ
					InitRule();//���[����ʂ̏���������
					pRuleScreen->bUse = true;//���[��������ʂ�\��
				}
			}

			if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(JOYKEY_START))
			{//Enter�L�[ or START�{�^���������ꂽ�ꍇ
				PlaySound(SOUND_LABEL_SE_DECIDE_000);
				StopSound(SOUND_LABEL_BGM012);
				//�Q�[����ʂ�
				SetFade(MODE_GAME);
			}
		}
		else
		{//���[��������ʂ��g�p���Ă���ꍇ
			UpdateRule();	//���[���������
		}
	}
	else
	{//�}�b�v��񂪍X�V����Ă��Ȃ��ꍇ
		UpdateRule();//���[���������

		if (!pRuleScreen->bUse)
		{//�ŏ��̃��[��������ʂ������ꍇ
			bLoad = true;
		}
	}
}

//=======================================================================================================================================================================================================================
// �`�揈��
//=======================================================================================================================================================================================================================
void DrawTutorial(void)
{
	Rule* pRuleScreen = GetRuleScreen();//���[��������ʂ̍\���̂̏��̎擾

	if (bLoad)
	{//�}�b�v��񂪍X�V����Ă���ꍇ
		SetCamera();		//�J�����̃Z�b�g
		DrawModel();		//���f��
		DrawDummy();		//�_�~�[�l�`
		DrawPlayer();		//�v���C���[
		DrawField();		//�t�B�[���h
		DrawWall();			//��
		DrawShadow();		//�e
		DrawBillboard();	//�r���{�[�h
		DrawAttackModel();	//�U�����o
		DrawEffect();		//�G�t�F�N�g
		DrawBullet();		//�e
		DrawParticle();     //�p�[�e�B�N��
		DrawOperate();		//�`���[�g���A���pUI
	}
	
	if (pRuleScreen->bUse)
	{//���[��������ʂ��g�p���Ă���ꍇ
		DrawRule();	//���[���������
	}
}
