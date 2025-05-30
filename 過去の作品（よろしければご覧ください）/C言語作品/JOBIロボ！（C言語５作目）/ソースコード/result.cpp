//=========================================================
//
// ���U���g��ʂ̏���
// Author : Atsumu Kuboichi
//
//=========================================================
#include "result.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "resultcheck.h"
#include "resultscore.h"
#include "score.h"

//=========================================================
// �O���[�o���ϐ�
//=========================================================

//=========================================================
// ����������
//=========================================================
void InitResult(void)
{
	InitBG();			//�w�i
	InitResultCheck();	//�~�b�V�����N���A�`�F�b�N
	InitResultScore();	//���U���g�\���X�R�A

	PlaySound(SOUND_LABEL_BGM014);
}

//=========================================================
// �I������
//=========================================================
void UninitResult(void)
{
	UninitResultCheck();//�~�b�V�����N���A�`�F�b�N
	UninitResultScore();//���U���g�\���X�R�A
	UninitBG();	        //�w�i
}

//=========================================================
// �X�V����
//=========================================================
void UpdateResult(void)
{
	UpdateBG();			//�w�i
	UpdateResultCheck();//�~�b�V�����N���A�`�F�b�N
	UpdateResultScore();//���U���g�\���X�R�A

	if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(JOYKEY_A))
	{//Enter�L�[ or Start�{�^���������ꂽ�ꍇ
		PlaySound(SOUND_LABEL_SE_DECIDE_000);

		SetFade(MODE_RANKING);	//�����L���O��ʂֈڍs
	}
}

//=========================================================
// �`�揈��
//=========================================================
void DrawResult(void)
{
	DrawBG();			//�w�i
	DrawResultCheck();	//�~�b�V�����N���A�`�F�b�N
	DrawResultScore();	//���U���g�\���X�R�A
}
