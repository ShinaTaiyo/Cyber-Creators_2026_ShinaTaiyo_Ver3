//========================================================
//
//�W���R�O���F�G�t�F�N�g������񌩒���[Block.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _BLOCK_H_
#define _BLOCK_H

#include "main.h"

#define MAX_BLOCK (5000)
#define NORMAL_BLOCKSIZE (20.0f)//���ʂ̃u���b�N�̃T�C�Y



//bool CollisionBlock(void);

//�\���̒�`
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 posOld;//1f�O�̈ʒu
	D3DXVECTOR3 move;
	int nCollisionUse;//bCollisionBlock���Ă�ł��镨�𔻕ʂ���
	bool bUse;//���o���g���Ă��邩�ǂ���
	int nType;//���o�̎��
	int nLife;//���o��������܂ł̎���
	float fWingMove;//�u���b�N������瓮����
	float fXsize;//X�����̑傫��
	float fYsize;//Y�����̑傫��
	float fRadius;//���a�ő傫�������߂Ă���ꍇ�̑傫��
	bool bDraw;//�`�悷�邩�ǂ�����bUse�ȊO�ł���ɔ��肷��t���O
	bool bMoveWay;//�ړ�������ς���t���O
}Block;



//�񋓌^��`
typedef enum
{
	BLOCKTYPE_NORMAL = 0,//0,���ʂ̃u���b�N�i���ʂȂ��j
	BLOCKTYPE_GRASS,//1,���u���b�N�i���ʂȂ��j
	BLOCKTYPE_RENGA,//2,�����K�u���b�N�i���ʂȂ��j
	BLOCKTYPE_FRAME,//3,���u���b�N�i�X���b�v�_���[�W�j
	BLOCKTYPE_WATER,//4,���u���b�N�i���̒��ɂ���Ԃ͖����W�����v�J���j
	BLOCKTYPE_SAND,//5,���u���b�N�i����������ł����j
	BLOCKTYPE_WING,//6,�H�u���b�N�i�ӂ�ӂ킵�Ă���j
	BLOCKTYPE_STONE,//7,��u���b�N�i�ォ�痎���Ă��āA������ƂP�T�_���[�W���󂯂�B�j
	BLOCKTYPE_LINE,//8,���u���b�N�i���X�g�_���W�������C���u���b�N�j
	BLOCKTYPE_ICE,//9,�X�u���b�N�i������j
	BLOCKTYPE_ARROW,//10,���Ɉړ�����u���b�N
	BLOCKTYPE_AUTOMOVE_RIGHT,//11,����Ă���ԁA�����I�Ƀv���C���[���E�ɂ��炵�Ă����u���b�N
	BLOCKTYPE_AUTOMOVE_LEFT,//12,����Ă���ԁA�����I�Ƀv���C���[�����ɂ��炵�Ă����u���b�N
	BLOCKTYPE_BLINKING,//13,�_�Ńu���b�N�i�������茻�ꂽ�肷��u���b�N�j
	BLOCKTYPE_MAX
}BLOCKTYPE;

typedef enum
{
	COLLISIONUSE_PLAYER = 0,//�v���C���[��bCollisionBlock���Ă�ł���
	COLLISIONUSE_ENEMY,//�G��bCollisionBlock���Ă�ł���
	COLLISIONUSE_ATTACK,//�U����bCollisionBlock���Ă�ł���
	COLLISIONUSE_MAX
}COLLISIONUSE;

typedef enum
{
	COLLISIONTYPE_RADIUS = 0,//���a�Ŕ��肵�Ă���
	COLLISIONTYPE_LENGTH,//�Ίp���Ŕ��肵�Ă���
	COLLISIONTYPE_MAX
}COLLISIONTYPE;

//�v���g�^�C�v�錾
void InitBlock(void);
void UninitBlock(void);
void UpdateBlock(void);
void DrawBlock(void);
void SetBlock(int ntype, D3DXVECTOR3 pos, int nLife);
bool bCollisionBlock(D3DXVECTOR3* pPos,
	D3DXVECTOR3* pPosOld,
	D3DXVECTOR3* pMove,
	float fWidth,
	float fHeight,
	float *fGravity,
	COLLISIONTYPE CollisionType,
	bool *bMoveWay,
	COLLISIONUSE C_use,
	bool* bIsWalling);
Block* GetBlock(void);
//Block* GetBlock(void);
#endif


