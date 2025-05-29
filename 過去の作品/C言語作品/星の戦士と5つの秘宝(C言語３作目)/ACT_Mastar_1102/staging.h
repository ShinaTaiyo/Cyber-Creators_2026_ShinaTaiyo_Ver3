//========================================================
//
//�W���R�O���F�G�t�F�N�g������񌩒���[staging.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _STAGING_H_
#define _STAGING_H

#include "main.h"
#include "Attack.h"
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
	int nAtackSummon;//���o�𔭓��������ꏊ�ɍU���Z����������
	bool bUse;//���o���g���Ă��邩�ǂ���
	float fScale;//�傫��
	int nA_Life;//���������U���̃��C�t
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
	float rot;//��]�p
	float fAeffective_Scale;//���������U���̌��ʔ͈�
	float fA_Width;//���������U���̑傫��
	float fA_Height;//���������U���̍���
	float fA_rotSpeed;//��������U���̉�]���x
	int nCntTime;//���Ԃ��v������
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
	STAGINGTYPE_BOSS1_SPECIAL_ATTACK,//�{�X�P�̕K�E�Z���o
	STAGINGTYPE_WARNING_ATTACK,//���o�ꏊ�ɍU�����������鏈��
	STAGINGTYPE_NORMAL_WARNINGS,//���ʂ̊댯���o�B�ǉ����ʂ͂Ȃ�
	STAGINGTYPE_NORMAL_WARNINGS_RELATED,//�ǉ����ʂȂ��̊댯���o���A�������Ă���G�Ɗ֘A�����ē�����
	STAGINGTYPE_BOSS2_SPECIAL_ATTACK,//�{�X�Q�̕K�E�Z���o
	STAGINGTYPE_EXPLOSIONWARNING,//�����x��
	STAGINGTYPE_BOSS3_SPECIAL_ATTACK,//�{�X�R�̕K�E�Z���o
	STAGINGTYPE_BOSS4_SPECIAL_ATTACK,//�{�X�S�K�E�Z����
	STAGINGTYPE_FINALBOSS_SPECIAL_ATTACK,//���X�{�X�̕K�E�Z���o
	STAGINGTYPE_SAFE,//���S�ȃG���A�̕\���p
	STAGINGTYPE_MAX
}STAGINGTYPE;

void SetStaging(STAGINGTYPE type,int nCntEnemy,D3DXVECTOR3 pos,int nLife,ATTACKTYPE Atype,float Arot,float fScale,float fA_rotSpeed,float fA_Width,
	float fA_Height,int nA_Life);

//Staging* GetStaging(void);
#endif

