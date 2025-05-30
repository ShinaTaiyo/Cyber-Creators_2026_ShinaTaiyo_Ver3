//========================================================
//
//�P�P���V���FX�t�@�C����ǂݍ���[Item.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _Item_H_
#define _Item_H_

#include "main.h"

#define NUM_ITEM (1)//�A�C�e���̎��
#define MAX_ITEM (1024)//�ǂݍ��ޓG�̍ő吔
#define ITEMSET "data\\TEXTFILE\\Item.txt"

#define CANDYDISP_WIDTH (85.0f)//�L�����f�B�\���̉���
#define CANDYDISP_HEIGHT (35.0f)//�L�����f�B�\���̍���

#define CANDY_DIGIT (1) //�\�����錅��
#define CANDYNUM_HEIGHT (35.0f) //�L�����f�B���l�̍���
#define CANDYNUM_WIDTH (CANDYNUM_HEIGHT * 0.6f)  //�L�����f�B���l�̉���

#define Item_SCALE (10.0f)
#define MAX_ITEMTEXTURE (32)

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
	int nNumLandingModel;//�ǂ̃��f���̏�ɏ���Ă��邩
	
	D3DXVECTOR3 Size;//�G�̑傫��
}Item;

//=============================
//�A�C�e��
//=============================
typedef struct
{
	LPD3DXMESH pMeshItem;//���b�V���i���_���j�ւ̃|�C���^�i���f���̎�ސ����j
	LPD3DXBUFFER pBuffMatItem;//�}�e���A���ւ̃|�C���^
	DWORD dwNumMatItem;//�}�e���A���̐�
	LPDIRECT3DTEXTURE9 apTextureItem[MAX_ITEMTEXTURE];//�e�N�X�`���ւ̃|�C���^
}ItemMat;

typedef struct
{
	bool bUse;      //�g�p���
	D3DXVECTOR3 pos;//�ʒu
	int nGetCandy;  //�L�����f�B�l����
}CandyDisp;

typedef struct
{
	bool bUse;      //�g�p���
	float fPosTexU; //�e�N�X�`�����W
	D3DXVECTOR3 pos;//�ʒu
}CandyNum;

typedef enum
{
	ITEM00_CANDY = 0,//�A�C�e���O�A�L�����f�B
	ITEM_MAX
}ITEMTYPE;

//�v���g�^�C�v�錾

//�A�C�e��
void InitItem(void);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);

//�L�����f�B�\��
void InitCandyDisp(void);
void UninitCandyDisp(void);
void UpdateCandyDisp(void);
void DrawCandyDisp(void);

//�L�����f�B�l�����\��
void InitCandyNum(void);
void UninitCandyNum(void);
void UpdateCandyNum(void);
void DrawCandyNum(void);
void AddCandyNum(int nCandy);

void SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);
int GetNumItem(void);//�ǂݍ��ޓG�̍ő吔���擾����
Item* GetItem(void);//�G�\���̂̏����擾����

#endif
