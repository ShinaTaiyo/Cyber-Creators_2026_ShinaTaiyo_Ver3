
//========================================================
//
//�P�O���P�S���F�}�b�v�ǂݍ���[warp.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _WARP_H_
#define _WARP_H

#include "main.h"

#define MAX_WARP (5000)
#define NORMAL_WARPSIZE (20.0f)//���ʂ̃u���b�N�̃T�C�Y



//bool CollisionWarp(void);

//�\���̒�`
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 PlayerPos;//�v���C���[�����[�v������ʒu
	bool bUse;//���o���g���Ă��邩�ǂ���
	int nRoadMapType;//�Ăяo���}�b�v�̎��
	int nLife;//���o��������܂ł̎���
	float fXsize;//X�����̑傫��
	float fYsize;//Y�����̑傫��
	float fRadius;//���a�ő傫�������߂Ă���ꍇ�̑傫��
	bool bDraw;//�`�悷�邩�ǂ�����bUse�ȊO�ł���ɔ��肷��t���O
	bool bMapChange;//�}�b�v���`�F���W����Ƃ��ɌĂяo���t���O
}Warp;




//�v���g�^�C�v�錾
void InitWarp(void);
void UninitWarp(void);
void UpdateWarp(void);
void DrawWarp(void);
void SetWarp(int nRoadMapType, D3DXVECTOR3 pos,D3DXVECTOR3 PlayerPos);
void ResetWarp(void);//���[�v����U�S������
Warp* GetWarp(void);
//Warp* GetWarp(void);
#endif


