//========================================================
//
//�W���R�O���F�G�t�F�N�g������񌩒���[Gage.cpp]
//Author:ShinaTaiyo
//
//=========================================================

#ifndef _GAGE_H_
#define _GAGE_H



typedef enum
{
	GAGETYPE_BOSS1HP=0,
	GAGETYPE_BOSSHP_FRAME,//�{�X��HP�Q�[�W�̘g�g��
	GAGETYPE_MAX
}GAGETYPE;

//�\���̒�`
typedef struct
{
	int nGageCount;
	float fGage;//�Q�[�W�̒�����ς���ϐ�
	int nType;//�Q�[�W�̎��
	bool bUse;//�g�p���Ă��邩�ǂ���
	int nSubjectEnemy;//HP��\������G
	int nCountBoss1;//�{�X�P���o�����Ă���̎��Ԃ��v������B
	float fBoss1MaxHp;//�{�X�P���o�����Ă���{�X�P��Hp��ۑ�����B


}Gage;


//�v���g�^�C�v�錾
void InitGage(void);
void UninitGage(void);
void UpdateGage(void);
void DrawGage(void);

void SetGage(int nCntEnemy, int nType);

Gage* GetGage(void);//�Q�[�W�\���̂̏����Q�b�g����B
#endif

