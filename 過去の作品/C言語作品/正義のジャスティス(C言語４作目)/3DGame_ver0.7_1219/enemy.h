//========================================================
//
//�P�P���V���FX�t�@�C����ǂݍ���[Enemy.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"

#define NUM_Enemy (100)//�G�̎��
#define MAX_ENEMY (1024)//�ǂݍ��ޓG�̍ő吔
#define ENEMYSET "data\\TEXTFILE\\Enemy.txt"

typedef struct
{
	D3DXVECTOR3 pos;//�ʒu
	D3DXVECTOR3 posOld;//1F�O�̈ʒu
	D3DXVECTOR3 move;//�ړ���
	D3DXVECTOR3 rot;//����
	D3DXVECTOR3 vtxMinEnemy;//���ꂼ��̍��W�̓G�̈ʒu�̍ŏ��l
	D3DXVECTOR3 vtxMaxEnemy;//���ꂼ��̍��W�̓G�̈ʒu�̍ő�l
	
	int nNumVtx;//���_��
	DWORD sizeFVF;//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE* pVtxBuff;//���_�o�b�t�@�ւ̃|�C���^
	int nType;//���
	int nIdxShadow;//�e�̃C���f�b�N�X
	bool bUse;//�g�p���Ă��邩�ǂ���
	bool bIsLanding;//�n�ʂɂ��邩�ǂ���

	int nHp;//�̗�
	int nAttack;//�U����
	int nDefeatScore;//�|�����Ƃ��ɂ��炦��X�R�A
	float fSpeed;//����

	float fVXaim;//X�����̃x�N�g��
	float fVYaim;//Y�����̃x�N�g��
	float fVZaim;//Z�����̃x�N�g��
	float fVaim;//���������x�N�g��
	float fVaimY1;//��������Y�����̃x�N�g��
	float fVaimY2;
	float fVaim3;

	float fPenduram;//�U��q�̂悤�ɒe�𔭎˂�����ϐ�
	bool bPenduramFlag;

	D3DXVECTOR3 Size;//�G�̑傫��
}Enemy;

typedef enum
{
	ENEMY00_NOOBSLIME = 0,//�ア�X���C��
	ENEMY01_ANGRYSLIME,//�{��X���C��
	ENEMY02_SWORDSLIME,//���X���C��
	ENEMY03_HAMMERSLIME,//�n���}�[�X���C��
	ENEMY04_ORANGESLIME,//�I�����W�X���C��
	ENEMY05_GREENSLIME,//�O���[���X���C��
	ENEMY06_WHITESLIME,//�z���C�g�X���C��
	ENEMY07_TURQUOISSLIME,//�^�[�R�C�Y�X���C��
	ENEMY08_ENGELSLIME,//�G���W�F���X���C��
	ENEMY09_DARKSLIME,//�_�[�N�X���C��
	ENEMY10_RAINBOWSLIME,//���C���{�[�X���C��
	ENEMY11_TREASURESLIME,//�g���W���[�X���C��
	ENEMY_MAX
}ENEMYTYPE;

//�v���g�^�C�v�錾
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);
int GetNumEnemy(void);//�ǂݍ��ޓG�̍ő吔���擾����
Enemy* GetEnemy(void);//�G�\���̂̏����擾����

void SetDamageEnemy(int nDamage,int nNumEnemy);

#endif


