//========================================================
//
//�W���R�O���F�G�t�F�N�g������񌩒���[staging.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _STAGING_H_
#define _STAGING_H

#include "main.h"
//�v���g�^�C�v�錾
void InitStaging(void);
void UninitStaging(void);
void UpdateStaging(void);
void DrawStaging(void);

//�\���̒�`
typedef struct
{
	int nStagingType;//�X�L���̃^�C�v�����߂�ϐ�
	D3DXVECTOR3 pos;
	bool bUse;//���o���g���Ă��邩�ǂ���
	int type;//���o�̎��
	int nLife;//���o��������܂ł̎���
	int nStagingCount;//���o���n�܂��Ă���̃J�E���g���J�n����B
	float fStagingPattern;//�A�j���[�V�����̎��
	float fStagingPatternCountY;//�A�j���[�V������Y���W�����炷�J�E���g
	bool bCountStartFlag;//���o���n�܂��Ă���̃J�E���g���J�n����t���O��錾����B
	int nAnimationRepeat;//�A�j���[�V���������s�[�g����ϐ�
	bool bDraw;//�`�悷�邩�ǂ�����bUse�ȊO�ł���ɔ��肷��t���O
	int nCntEnemy;//n�Ԗڂ̓G�̂���ʒu�𔻒肷��
	float fColor_a;//�F�̃A���t�@�l�𑀍삷��ϐ�
	bool bColorChange_a;//�F�̃A���t�@�l���C���N�������g���邩�A�f�N�������g���邩�����߂�t���O
}Staging;



//�񋓌^��`
typedef enum
{
	STAGINGTYPE_WARNING = 0,//���[���������ꂽ��ʂ��Ăяo���B
	STAGINGTYPE_APPER,//�G�o���ʒu��\�����鉉�o
    STAGINGTYPE_BULLETWARNING,//����̒e���o����\������
	STAGINGTYPE_PRESSSTART,//�uPressStart�v�̕�����\������B
	STAGINGTYPE_PRESSENTER,//�uPressEnter�v�̕�����\������B
	STAGINGTYPE_SHIELD,//�u�X�L���u�C�[�W�X�V�[���h�v�̑���Ɏg���񋓌^��`
	STAGINGTYPE_NORMAL_CLEARBONUS,//�m�[�}�����[�h�̃N���A�{�[�i�X�\�L
	STAGINGTYPE_HARD_CLEARBONUS,//�n�[�h���[�h�̃N���A�{�[�i�X�\�L
	STAGINGTYPE_VERYHARD_CLEARBONUS,//�x���[�n�[�h�̃N���A�{�[�i�X�\�L
	STAGINGTYPE_RANKING_BG,//�����L���O�̔w�i�������ň�U�\��������B
	STAGINGTYPE_MAX
}STAGINGTYPE;

void SetStaging(STAGINGTYPE type,int nCntEnemy,D3DXVECTOR3 pos);

//Staging* GetStaging(void);
#endif

