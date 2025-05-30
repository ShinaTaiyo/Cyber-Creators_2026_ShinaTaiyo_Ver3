//========================================================
//
//�P�P���V���FX�t�@�C����ǂݍ���[Summon.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _SUMMON_H_
#define _SUMMON_H_

#include "main.h"
#include "enemy.h"

#define NUM_SUMMON (100)//�G�̎��
#define MAX_SUMMON (1024)//�ǂݍ��ޓG�̍ő吔
#define SUMMONSET "data\\TEXTFILE\\Summon.txt"
#define MAX_SUMMONMAT (100)
typedef struct
{
	LPD3DXMESH pMesh;//���b�V���i���_���j�ւ̃|�C���^�i���f���̎�ސ����j
	LPD3DXBUFFER pBuffMat;//�}�e���A���ւ̃|�C���^
	DWORD dwNumMat;//�}�e���A���̐�

}SummonModelInfo;

typedef struct
{
	int nType;//���
	int nLayoutType;//�z�u�������@�̎��
	D3DXVECTOR3 pos;//�ʒu
	D3DXVECTOR3 rot;//����
	D3DXVECTOR3 move;//�ړ���
	D3DXMATRIX mtxWorld;//�}�g���b�N�X���[���h
	bool bUse;//�g�p���Ă��邩�ǂ���
	int nSummonTime;//��������
	int nNumSetEdit;//�G�f�B�^�[�Ŕz�u���ꂽ�G�̔ԍ�
	bool bRemote;//���u����̕ύX���󂯂邩�ǂ���
	D3DCOLORVALUE Diffuse[NUM_ENEMY][MAX_SUMMONMAT];//�F����
	D3DCOLORVALUE FormarDiffuse[NUM_ENEMY][MAX_SUMMONMAT];//�F����

}Summon;//�G�����\����

static const char* SUMMON_FILENAME[NUM_ENEMY] =
{
	"data\\MODEL\\OBJECT\\Cylinder\\Cylinder_000.x",
	"data\\MODEL\\OBJECT\\Cylinder\\Cylinder_001.x",
	"data\\MODEL\\OBJECT\\Cylinder\\Cylinder_002.x",
	"data\\MODEL\\OBJECT\\Cylinder\\Cylinder_003.x",
	"data\\MODEL\\OBJECT\\Cylinder\\Cylinder_004.x",
};


//�v���g�^�C�v�錾
void InitSummon(void);
void UninitSummon(void);
void UpdateSummon(void);
void DrawSummon(void);
void SetSummon(int nType, int nTime, int nNumSetEdit,int nLayoutType, bool bRemote, D3DXVECTOR3 pos, D3DXVECTOR3 rot);

Summon* GetSummon(void);//�\���̂̏����擾
#endif
