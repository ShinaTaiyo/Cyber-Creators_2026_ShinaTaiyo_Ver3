
//�W���R�O���F�G�t�F�N�g������񌩒���[pause.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _CHARACTER_H_
#define _CHACACTER_H_

#include "main.h"

//�}�N����`
#define MAX_CHARACTER_TEX (2)

//�񋓌^��`
typedef enum
{
	CHACACTER_TYPE_LIGHT=0,
	CHACACTER_TYPE_HEAVY,
	CHARACTER_TYPE_MAX
}CHARACTER_TYPE;

//�\���̒�`
typedef struct
{
	int nType;//�Ăяo���R�}���h�̎�ނ����߂�ϐ�
	D3DXVECTOR3 pos;//���S���W��\���B
	bool bChooseCharacter;
}Character;

typedef struct
{
	int nCharacterNum;
}CharacterNum;

//�v���g�^�C�v�錾
void InitChacacter(void);
void UninitChacacter(void);
void UpdateCharacter(void);
void DrawCharacter(void);
CharacterNum *GetCharacterNum(void);
#endif


#pragma once
