//========================================================
//
//�P�O���R�P�X���F��l������[Treasure.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _TREASURE_H_
#define _TREASURE_H

#include "main.h"
//�v���g�^�C�v�錾
void InitTreasure(void);
void UninitTreasure(void);
void UpdateTreasure(void);
void DrawTreasure(void);

//�\���̒�`
typedef struct
{
	int nTreasureType;//�X�L���̃^�C�v�����߂�ϐ�
	D3DXVECTOR3 pos;
	bool bUse;//����g���Ă��邩�ǂ���
	int type;//��̎��
	int nLife;//�󂪏�����܂ł̎���
	int nTreasureCount;//�󂪎n�܂��Ă���̃J�E���g���J�n����B
	float fTreasurePattern;//�A�j���[�V�����̎��
	float fTreasurePatternCountY;//�A�j���[�V������Y���W�����炷�J�E���g
	bool bCountStartFlag;//�󂪎n�܂��Ă���̃J�E���g���J�n����t���O��錾����B
	int nAnimationRepeat;//�A�j���[�V���������s�[�g����ϐ�
	bool bDraw;//�`�悷�邩�ǂ�����bUse�ȊO�ł���ɔ��肷��t���O
	int nCntEnemy;//n�Ԗڂ̓G�̂���ʒu�𔻒肷��
	float fColor_a;//�F�̃A���t�@�l�𑀍삷��ϐ�
	bool bColorChange_a;//�F�̃A���t�@�l���C���N�������g���邩�A�f�N�������g���邩�����߂�t���O
}Treasure;



//�񋓌^��`
typedef enum
{
	TREASURETYPE_BOX = 0,//��l���\���p
	TREASURETYPE_REAF,//���̕�
	TREASURETYPE_ICE,//�X�̕�
	TREASURETYPE_FIRE,//���̕�
	TREASURETYPE_THUNDER,//���̕�
	TREASURETYPE_LAST,//�T�C�S�m�^�J��
	TreasureTYPE_MAX
}TREASURETYPE;

void SetTreasure(TREASURETYPE type, int nCntEnemy, D3DXVECTOR3 pos);

//Treasure* GetTreasure(void);
#endif


