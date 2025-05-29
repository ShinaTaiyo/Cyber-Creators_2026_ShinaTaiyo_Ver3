//========================================================
//
//�P�P���V���FX�t�@�C����ǂݍ���[Item.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _Item_H_
#define _Item_H_

#include "main.h"

#define NUM_ITEM (4)//�G�̎��
#define MAX_ITEM (1024)//�ǂݍ��ޓG�̍ő吔
#define ITEMSET "data\\TEXTFILE\\Item.txt"

typedef struct
{
	D3DXVECTOR3 pos;//�ʒu
	D3DXVECTOR3 posOld;//1F�O�̈ʒu
	D3DXVECTOR3 move;//�ړ���
	D3DXVECTOR3 rot;//����
	D3DXVECTOR3 vtxMinItem;//���ꂼ��̍��W�̓G�̈ʒu�̍ŏ��l
	D3DXVECTOR3 vtxMaxItem;//���ꂼ��̍��W�̓G�̈ʒu�̍ő�l

	int nNumVtx;//���_��
	DWORD sizeFVF;//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE* pVtxBuff;//���_�o�b�t�@�ւ̃|�C���^
	int nType;//���
	int nIdxShadow;//�e�̃C���f�b�N�X
	bool bUse;//�g�p���Ă��邩�ǂ���
	bool bIsLanding;//�n�ʂɂ��邩�ǂ���

	
	D3DXVECTOR3 Size;//�G�̑傫��
}Item;

typedef enum
{
	ITEM00_HEALHART = 0,//�̗͉񕜃A�C�e��
	ITEM01_ENERGYBOOST,//MP�񕜃A�C�e��
	ITEM02_POWFULSWORD,//�U���͏㏸�A�C�e��
	ITEM03_CAPSULEBARRIER,//�U�������A�C�e��
	ITEM_MAX
}ITEMTYPE;

//�v���g�^�C�v�錾
void InitItem(void);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);
void SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);
int GetNumItem(void);//�ǂݍ��ޓG�̍ő吔���擾����
Item* GetItem(void);//�G�\���̂̏����擾����


#endif

