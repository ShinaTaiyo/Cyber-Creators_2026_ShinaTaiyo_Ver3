//========================================================
//
//�P�O���X���F�A�N�V������UI�쐬[Ability.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _ABILITY_H_
#define _ABILITY_H_

#include "main.h"

//�A�r���e�B�񋓌^��`
typedef enum
{
	ABILITYTYPE_NORMAL=0,
	ABILITYTYPE_BULLET,
	ABILITYTYPE_MAX
}ABILITYTYPE;

//�A�r���e�B�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;//�ʒu
	int nAbilityType;//�A�r���e�B�̎��
	bool bUse;//�A�r���e�B�\�������悤���Ă��邩�ǂ���
}Ability;

//�v���g�^�C�v�錾
void InitAbility(void);
void UninitAbility(void);
void UpdateAbility(void);
void DrawAbility(void);
void SetAbility(ABILITYTYPE type);//�A�r���e�B�ݒ菈��
Ability * GetAbility(void);//�A�r���e�B�����擾

#endif
