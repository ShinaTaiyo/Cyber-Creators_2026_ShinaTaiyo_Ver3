//========================================================
//
//�W���R�O���F�G�t�F�N�g������񌩒���[SkillName.h]
//Author:ShinaTaiyo
//
//=========================================================

#ifndef _SKILLGAGE_H_
#define _SKILLGAGE_H

#include "main.h"



//�v���g�^�C�v�錾
void InitSkillName(void);
void UninitSkillName(void);
void UpdateSkillName(void);
void DrawSkillName(void);

//�\���̒�`
typedef struct
{
	int nSkillType;//�X�L���̃^�C�v�����߂�ϐ�
	float fSkillPattern;//�g���X�L�������߂�����ϐ�
	D3DXVECTOR3 pos;//�ʒu
}Skill;



//�񋓌^��`
typedef enum
{
	SKILLTYPE_NORMAL= 0,//
	SKILLNAME_MAX
}SkillType;

Skill* GetSkill(void);
#endif

