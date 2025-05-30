//========================================================
//
//�X���P�V���F��Փx�I������[Difficulty.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _DIFFICULTY_H_
#define _DIFFICULTY_H_

#include "main.h"

//�}�N����`
#define MAX_DIFFICULTY_TEX (5)
#define MAX_WAVELENGTH_TEX (4)

//�񋓌^��`
typedef enum
{
	DIFFICULTY_BG=0,
	DIFFICULTY_EASY,
	DIFFICULTY_NORMAL,
	DIFFICULTY_HARD,
	DIFFICULTY_VERYHARD,
	WAVELENGTH_SHORT,
	WAVELENGTH_NORMAL,
	WAVELENGTH_LONG,
	WAVELENGTH_VERYLONG,
	DIFFICULTY_MAX
}DIFFICULTY;

//�\���̒�`
typedef struct
{
	int nNumDifficulty;//�I�ԓ�Փx�̎�ނ����߂�ϐ�
	D3DXVECTOR3 pos;//���S���W��\���B
	bool bChooseDifficulty;//�I�񂾃R�}���h�����邭�Ȃ�悤�ɂ���t���O
}Difficulty;

typedef struct
{
	int nWaveLength;//�E�F�[�u�̒��������߂�ϐ�
	int nDifficulty;//��Փx�����߂�ϐ�
	int nDecideCount;//���肵����
}GameCustom;

//�v���g�^�C�v�錾
void InitDifficulty(void);
void UninitDifficulty(void);
void UpdateDifficulty(void);
void DrawDifficulty(void);
GameCustom *GetGameCustom(void);

#endif
