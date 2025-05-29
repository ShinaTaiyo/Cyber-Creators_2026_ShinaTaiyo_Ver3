//========================================================
//
//�W���R�O���F�G�t�F�N�g������񌩒���[Block.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#include "main.h"
#include <stdlib.h>
#include <time.h>
#include "score.h"
#include "game.h"
#include "result.h"
#include "player.h"
#include "block.h"
#include "sound.h"
#include "fade.h"
#include "health.h"
//�}�N����`
#include "input.h"
#define NUM_BLOCKTEX (14)//�u���b�N�̎��
#define FRAME_BLOCKSIZE (20.0f) //���u���b�N�̑傫��



//�O���[�o���ϐ�
bool g_bIsLanding;

int g_nCntBlockTime;//�u���b�N���o�����Ă���̎��Ԃ𑪂�

float g_fWingMove;//�H�u���b�N���㉺�ɓ�����

LPDIRECT3DTEXTURE9 g_apTextureBlock[NUM_BLOCKTEX] = {};
//D3DXVECTOR3 g_rotPI;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBlock = NULL;
//Enemy g_Block[MAX_ENEMY];//�G�̏��
//Result g_aResult;

Block g_aBlock[MAX_BLOCK];

//int g_nCountTime = 0;//���Ԃ�\���O���[�o���ϐ�

//=====================================================
//==========================
//�u���b�N�̏���������
//==========================
void InitBlock(void)
{
	//srand((unsigned int)time(NULL));//�Q�[�����J�n�������Ԗ��ɗ����̎��ݒ�
	LPDIRECT3DDEVICE9 pDevice;
	int	nCntBlock;
	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Block_000.png",
		&g_apTextureBlock[BLOCKTYPE_NORMAL]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\GrassBlock_001.png",
		&g_apTextureBlock[BLOCKTYPE_GRASS]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\GrayRenga_000.png",
		&g_apTextureBlock[BLOCKTYPE_RENGA]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\FrameBlock_001.png",
		&g_apTextureBlock[BLOCKTYPE_FRAME]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\WaterBlock_000.png",
		&g_apTextureBlock[BLOCKTYPE_WATER]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\SandBlock_000.png",
		&g_apTextureBlock[BLOCKTYPE_SAND]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\WingBlock_000.png",
		&g_apTextureBlock[BLOCKTYPE_WING]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\StoneBrock_001.png",
		&g_apTextureBlock[BLOCKTYPE_STONE]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\LineBlock_000.png",
		&g_apTextureBlock[BLOCKTYPE_LINE]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\IceBlock_000.png",
		&g_apTextureBlock[BLOCKTYPE_ICE]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\ArrowBlock.png",
		&g_apTextureBlock[BLOCKTYPE_ARROW]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\AutoMoveBlock_Right.png",
		&g_apTextureBlock[BLOCKTYPE_AUTOMOVE_RIGHT]);


	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\AutoMoveBlock_Left.png",
		&g_apTextureBlock[BLOCKTYPE_AUTOMOVE_LEFT]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\BlinkingBlock.png",
		&g_apTextureBlock[BLOCKTYPE_BLINKING]);

	g_fWingMove = 0.0f;

	//�u���b�N���̏�����
	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		g_aBlock[nCntBlock].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�u���b�N�̈ʒu
		g_aBlock[nCntBlock].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�u���b�N�̈ʒu
		g_aBlock[nCntBlock].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//����
		g_aBlock[nCntBlock].nType= 0;//�u���b�N�̎��
		g_aBlock[nCntBlock].nLife = 0;//�u���b�N�̃��C�t
		g_aBlock[nCntBlock].bUse = false;//�u���b�N���g���Ă��邩����
		g_aBlock[nCntBlock].bMoveWay = false;//�ړ����������߂�t���O
		g_aBlock[nCntBlock].bDraw = false;//bUse��true���������Ƃɉ����A�X��bDraw��true���ǂ����ŕ`�悷�邩�ǂ��������߂�B
		g_aBlock[nCntBlock].fXsize = 0.0f;//X�����̃T�C�Y
		g_aBlock[nCntBlock].fYsize = 0.0f;//Y�����̃T�C�Y
		g_aBlock[nCntBlock].fRadius = 0.0f;//���a�ŃT�C�Y�����߂�ꍇ�̃T�C�Y
		g_aBlock[nCntBlock].nCollisionUse = 0;//����bCollisionBlock���Ă�ł��邩
		g_aBlock[nCntBlock].fWingMove = 0.0f;//�u���b�N������瓮����
	}

	g_nCntBlockTime = 0;//�u���b�N���o�����Ă���̎��Ԃ𑪂�

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BLOCK,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBlock,
		NULL);

	VERTEX_2D* pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		//���_���W�̐ݒ�(�G�̑傫���j
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;//���_�f�[�^�̃|�C���^���S���i�߂�B
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBlock->Unlock();
}

//====================================
//�`���[�g���A���̏I������
//====================================
void UninitBlock(void)
{
	//�e�N�X�`���̔j��
	int nCntBlock;
	for (nCntBlock = 0; nCntBlock < NUM_BLOCKTEX; nCntBlock++)
	{
		if (g_apTextureBlock[nCntBlock] != NULL)
		{
			g_apTextureBlock[nCntBlock]->Release();
			g_apTextureBlock[nCntBlock] = NULL;
		}
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffBlock != NULL)
	{
		g_pVtxBuffBlock->Release();
		g_pVtxBuffBlock = NULL;
	}
}


//========================================
//�G�̍X�V����
//========================================
void UpdateBlock(void)
{
	VERTEX_2D* pVtx;
	int nCntBlock;
	Player* pPlayer = GetPlayer();
	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	//g_fWingMove += 0.01f;

	g_nCntBlockTime++;//�Q�[�����n�܂��Ă���̎��Ԃ𑪂�

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if( g_aBlock[nCntBlock].bUse == true)
		{
			g_aBlock[nCntBlock].nLife--;
			g_aBlock[nCntBlock].fWingMove += 0.01f;
				//�u���b�N�̈ʒu�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x - g_aBlock[nCntBlock].fRadius, g_aBlock[nCntBlock].pos.y - g_aBlock[nCntBlock].fRadius, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fRadius, g_aBlock[nCntBlock].pos.y - g_aBlock[nCntBlock].fRadius, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x - g_aBlock[nCntBlock].fRadius, g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fRadius, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fRadius, g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fRadius, 0.0f);



				if (g_aBlock[nCntBlock].nType == BLOCKTYPE_WING)
				{
					g_aBlock[nCntBlock].move.y = cosf(D3DX_PI * g_aBlock[nCntBlock].fWingMove ) * 5.0f;
				}

				if (g_aBlock[nCntBlock].nType == BLOCKTYPE_ARROW)
				{
				
					g_aBlock[nCntBlock].move.x = sinf(D3DX_PI * g_aBlock[nCntBlock].fWingMove) * 5.0f;
				}

				if (g_aBlock[nCntBlock].nType == BLOCKTYPE_BLINKING && g_nCntBlockTime % 60 == 0)
				{
					g_aBlock[nCntBlock].bDraw = g_aBlock[nCntBlock].bDraw ? false : true;
				}

				if (g_aBlock[nCntBlock].nType == BLOCKTYPE_WATER)
				{
					if (pPlayer->pos.x >= g_aBlock[nCntBlock].pos.x - g_aBlock[nCntBlock].fRadius - pPlayer->fWidth &&
						pPlayer->pos.x <= g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fRadius + pPlayer->fWidth &&
						pPlayer->pos.y >= g_aBlock[nCntBlock].pos.y - g_aBlock[nCntBlock].fRadius &&
						pPlayer->pos.y <= g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fRadius + pPlayer->fHeight)
					{
						if (GetKeyboardPress(DIK_SPACE) == true || GetJoypadPress(JOYKEY_A) == true)
						{
							pPlayer->move.y = -10.0f;
						}
					}
				}
				
				
				g_aBlock[nCntBlock].pos.x += g_aBlock[nCntBlock].move.x;
				g_aBlock[nCntBlock].pos.y += g_aBlock[nCntBlock].move.y;

				if (g_aBlock[nCntBlock].nLife == 0)
				{
					g_aBlock[nCntBlock].bUse = false;
				}

		}
		pVtx += 4;//���_�f�[�^�̃|�C���^���S���i�߂�B
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBlock->Unlock();


}


//=========================================
//�G�̕`�揈��
//=========================================
void DrawBlock(void)
{

	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

	int nCntBlock;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBlock, 0, sizeof(VERTEX_2D));//���_�o�b�t�@�ւ̃|�C���^�ƒ��_���̍\���̂̃T�C�Y

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
	
		if (g_aBlock[nCntBlock].bUse == true)
		{
			if (g_aBlock[nCntBlock].bDraw == true)
			{
				if (g_aBlock[nCntBlock].nType == BLOCKTYPE_NORMAL)
				{
					pDevice->SetTexture(0, g_apTextureBlock[BLOCKTYPE_NORMAL]);
				}
				else if (g_aBlock[nCntBlock].nType == BLOCKTYPE_GRASS)
				{
					pDevice->SetTexture(0, g_apTextureBlock[BLOCKTYPE_GRASS]);
				}
				else if (g_aBlock[nCntBlock].nType == BLOCKTYPE_RENGA)
				{
					pDevice->SetTexture(0, g_apTextureBlock[BLOCKTYPE_RENGA]);
				}
				else if (g_aBlock[nCntBlock].nType == BLOCKTYPE_FRAME)
				{
					pDevice->SetTexture(0, g_apTextureBlock[BLOCKTYPE_FRAME]);
				}
				else if (g_aBlock[nCntBlock].nType == BLOCKTYPE_WATER)
				{
					pDevice->SetTexture(0, g_apTextureBlock[BLOCKTYPE_WATER]);
				}
				else if (g_aBlock[nCntBlock].nType == BLOCKTYPE_SAND)
				{
					pDevice->SetTexture(0, g_apTextureBlock[BLOCKTYPE_SAND]);
				}
				else if (g_aBlock[nCntBlock].nType == BLOCKTYPE_WING)
				{
					pDevice->SetTexture(0, g_apTextureBlock[BLOCKTYPE_WING]);
				}
				else if (g_aBlock[nCntBlock].nType == BLOCKTYPE_STONE)
				{
					pDevice->SetTexture(0, g_apTextureBlock[BLOCKTYPE_STONE]);
				}
				else if (g_aBlock[nCntBlock].nType == BLOCKTYPE_LINE)
				{
					pDevice->SetTexture(0, g_apTextureBlock[BLOCKTYPE_LINE]);
				}
				else if (g_aBlock[nCntBlock].nType == BLOCKTYPE_ICE)
				{
					pDevice->SetTexture(0, g_apTextureBlock[BLOCKTYPE_ICE]);
				}
				else if (g_aBlock[nCntBlock].nType == BLOCKTYPE_ARROW)
				{
					pDevice->SetTexture(0, g_apTextureBlock[BLOCKTYPE_ARROW]);
				}
				else if (g_aBlock[nCntBlock].nType == BLOCKTYPE_AUTOMOVE_RIGHT)
				{
					pDevice->SetTexture(0, g_apTextureBlock[BLOCKTYPE_AUTOMOVE_RIGHT]);
				}
				else if (g_aBlock[nCntBlock].nType == BLOCKTYPE_AUTOMOVE_LEFT)
				{
					pDevice->SetTexture(0, g_apTextureBlock[BLOCKTYPE_AUTOMOVE_LEFT]);
				}
				else if (g_aBlock[nCntBlock].nType == BLOCKTYPE_BLINKING)
				{
					pDevice->SetTexture(0, g_apTextureBlock[BLOCKTYPE_BLINKING]);
				}
				//�|���S���̕`��
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBlock * 4, 2);//0�́A�`�悷��ŏ��̒��_�C���f�b�N�X�ŁA2�́A�`�悷��v���~�e�B�u��

				//&g_pVtxBuffPolygon[0],//���_���̐擪�A�h���X
				//sizeof(VERTEX_2D));//���_���\���̂̃T�C�Y
			}
		}
	}


}

void SetBlock(int ntype, D3DXVECTOR3 pos, int nLife)
{
	int nCntBlock;

	VERTEX_2D* pVtx;

	/*Enemy* pEnemy = GetEnemy();*/

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾

	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == false)
		{
			g_aBlock[nCntBlock].pos = pos;//SetEnemy�̎��ɁA�G�̈ʒu��������ꂽ�ہA�ʒu�̈����������SetBlock��pos�ɑ�����邱�Ƃɂ��A
			g_aBlock[nCntBlock].nLife = nLife;//���C�t�i�\�����ԁj
			g_aBlock[nCntBlock].nType = ntype;//�u���b�N�̎�ނ��Z�b�g����B
			g_aBlock[nCntBlock].nCollisionUse = 0;//����bCollisionBlock���Ă�ł��邩
			g_aBlock[nCntBlock].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//����
			g_aBlock[nCntBlock].bDraw = true;//�`�悷�邩�ǂ���
			g_aBlock[nCntBlock].fWingMove = 0.0f;//�u���b�N������瓮����
			//=====================================================
			//�u���b�N�̑傫���ݒ菈��
			//=====================================================

			//�����ύX�������Ȃ��ꍇ�̃u���b�N�̃T�C�Y
			g_aBlock[nCntBlock].fRadius = NORMAL_BLOCKSIZE;



			if (g_aBlock[nCntBlock].nType == BLOCKTYPE_NORMAL)
			{//�u���b�N�̃^�C�v���u�m�[�}���v
				g_aBlock[nCntBlock].fRadius = NORMAL_BLOCKSIZE;//���a�ł����߂�ꍇ�̑傫��
			}
			if (g_aBlock[nCntBlock].nType == BLOCKTYPE_FRAME)
			{
				g_aBlock[nCntBlock].fRadius = FRAME_BLOCKSIZE;//���a�ł����߂�ꍇ�̑傫��
			}

			if (g_aBlock[nCntBlock].nType >= BLOCKTYPE_NORMAL && g_aBlock[nCntBlock].nType <= NUM_BLOCKTEX - 1)
			{
				g_aBlock[nCntBlock].bUse = true;//�g�p���Ă��邩�ǂ���
			}
			else
			{//�u���b�N�̎�ނ̒�����Ȃ�������g�p���Ȃ�
				g_aBlock[nCntBlock].bUse = false;
			}
			break;
		}
		pVtx += 4;
	}
	g_pVtxBuffBlock->Unlock();
}

Block* GetBlock(void)
{
	return &g_aBlock[0];
}
bool bCollisionBlock(D3DXVECTOR3* pPos,
	D3DXVECTOR3* pPosOld,
	D3DXVECTOR3* pMove,
	float fWidth,
	float fHeight,
	float* fGravity,
	COLLISIONTYPE CollisionType,
	bool* bMoveWay,
	COLLISIONUSE C_use,
	bool* bIsWalling)
{
	Player* pPlayer = GetPlayer();
	g_bIsLanding = false;//���n���Ă��邩�ǂ���
	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		
		{//�u���b�N���g���Ă��邩�ǂ���fWidth
			if (g_aBlock[nCntBlock].nType != BLOCKTYPE_WATER)
			{
				if (g_aBlock[nCntBlock].bDraw == true)
				{//�`�悳��Ă��邩�ǂ���
					if (CollisionType == COLLISIONTYPE_RADIUS)
					{


						if (pPos->x - fWidth < 0.0f)
						{
							pPos->x = 0.0f + fWidth;
							if (*bMoveWay == false)
							{
								*bMoveWay = true;
							}
							else if (*bMoveWay == true)
							{
								*bMoveWay = false;
							}
						}
						else if (pPos->x + fWidth > SCREEN_WIDTH)
						{
							pPos->x = SCREEN_WIDTH - fWidth;
							if (*bMoveWay == false)
							{
								*bMoveWay = true;
							}
							else if (*bMoveWay == true)
							{
								*bMoveWay = false;
							}
						}

						if (pPos->x + fWidth >= g_aBlock[nCntBlock].pos.x - g_aBlock[nCntBlock].fRadius//�v���C���[�̉E�[���u���b�N�̍��[�������Ȃ�
							&& pPosOld->x + fWidth <= g_aBlock[nCntBlock].pos.x - 5.0f /*- g_aBlock[nCntBlock].fRadius*///1F�O�̃v���C���[�̉E�[���u���b�N�̉E�[���
							&& pPos->y - fHeight < g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fRadius//�v���C���[�̏�[���u���b�N�̉��[����Ȃ�
							&& pPos->y > g_aBlock[nCntBlock].pos.y - g_aBlock[nCntBlock].fRadius)//�v���C���[�̉��[���u���b�N�̏�[��艺�Ȃ�
						{//�v���C���[�̉E�[���u���b�N�̒��ɓ�������
							pPos->x = g_aBlock[nCntBlock].pos.x - g_aBlock[nCntBlock].fRadius - fWidth;
							pMove->x = -0.0f;

							*bIsWalling = true;
							if (*bMoveWay == false)
							{
								*bMoveWay = true;
							}
							else if (*bMoveWay == true)
							{
								*bMoveWay = false;
							}



							if (g_aBlock[nCntBlock].nType == BLOCKTYPE_FRAME && C_use == COLLISIONUSE_PLAYER)
							{
								DamageHealth(1);//�P�_���[�W��^��������
							}
						}
						if (pPos->x - fWidth < g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fRadius//�v���C���[�̍��[���u���b�N�̉E�[�������Ȃ�
							&& pPosOld->x - fWidth >= g_aBlock[nCntBlock].pos.x + 5.0f/*+ g_aBlock[nCntBlock].fRadius*///1F�O�̃v���C���[�̍��[���u���b�N�̉E�[���O���Ȃ�
							&& pPos->y - fHeight < g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fRadius//�v���C���[�̏�[���u���b�N�̉��[�������Ȃ�
							&& pPos->y > g_aBlock[nCntBlock].pos.y - g_aBlock[nCntBlock].fRadius)//�v���C���[�̉��[���u���b�N�̏�[�������Ȃ�
						{//�v���C���[�̍��[���u���b�N�̒��ɓ�������
							pPos->x = g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fRadius + fWidth;
							pMove->x = 0.0f;

							*bIsWalling = true;
							if (*bMoveWay == false)
							{
								*bMoveWay = true;
							}
							else if (*bMoveWay == true)
							{
								*bMoveWay = false;
							}

							if (g_aBlock[nCntBlock].nType == BLOCKTYPE_FRAME && C_use == COLLISIONUSE_PLAYER)
							{
								DamageHealth(1);//�P�_���[�W��^��������
							}
						}
						if (pPos->x + fWidth > g_aBlock[nCntBlock].pos.x - g_aBlock[nCntBlock].fRadius
							&& pPos->x - fWidth < g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fRadius
							&& pPos->y >= g_aBlock[nCntBlock].pos.y - g_aBlock[nCntBlock].fRadius
							&& pPosOld->y <= g_aBlock[nCntBlock].pos.y - 5.0f/*- g_aBlock[nCntBlock].fRadius*/)
						{//�v���C���[�̉��[���A�u���b�N�̏�[��艺�Ȃ�

							if (g_aBlock[nCntBlock].nType == BLOCKTYPE_ARROW && *bIsWalling == false)
							{
								pPos->x = g_aBlock[nCntBlock].pos.x;//�v���C���[�̈ʒu���u���b�N�̏�[�Ɉړ�������B
							}

							pPos->y = g_aBlock[nCntBlock].pos.y - g_aBlock[nCntBlock].fRadius;//�v���C���[�̈ʒu���u���b�N�̏�[�Ɉړ�������B
							pMove->y = 0.0f;//�d�͂�0�ɂ���
							*fGravity = 0.0f;//�d�͂̕␳�l��0�ɂ���@
							g_bIsLanding = true;//�n�ʂɂ���t���O�𗧂āA�W�����v���\�ɂ���

							if (g_aBlock[nCntBlock].nType == BLOCKTYPE_FRAME && C_use == COLLISIONUSE_PLAYER)
							{
								DamageHealth(1);//�P�_���[�W��^��������
							}
							else if (g_aBlock[nCntBlock].nType == BLOCKTYPE_ICE && g_aBlock[nCntBlock].nCollisionUse == COLLISIONUSE_PLAYER)
							{
								pPlayer->nInertia = INERTIA_ICE;//�X�u���b�N�̏�ɂ���̂ŁA���������߂�
							}
							else if (g_aBlock[nCntBlock].nType == BLOCKTYPE_AUTOMOVE_RIGHT && g_aBlock[nCntBlock].nCollisionUse == COLLISIONUSE_PLAYER)
							{
								pPlayer->bAutoMoveR = true;//�E�����֎����I�Ɉړ����邩
							}
							else if (g_aBlock[nCntBlock].nType == BLOCKTYPE_AUTOMOVE_LEFT && g_aBlock[nCntBlock].nCollisionUse == COLLISIONUSE_PLAYER)
							{
								pPlayer->bAutoMoveL = true;//�������֎����I�Ɉړ����邩
							}


						}
						if (pPos->x + fWidth > g_aBlock[nCntBlock].pos.x - g_aBlock[nCntBlock].fRadius
							&& pPos->x - fWidth < g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fRadius
							&& pPosOld->y - fHeight >= g_aBlock[nCntBlock].pos.y + 5.0f/* + g_aBlock[nCntBlock].fRadius*/
							&& pPos->y - fHeight <= g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fRadius)
						{//PosOld�̓����u���b�N�̉��[����ŁA�v���C���[��pos���u���b�N�̉��[��艺�Ȃ�E�E�E



							pPos->y = g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fRadius + fHeight + 6.0f;//�v���C���[�̈ʒu���u���b�N�̉��[�Ɉړ�������B
							pMove->y = 0.0f;//�d�͂��O�ɂ���
							if (g_aBlock[nCntBlock].nType == BLOCKTYPE_FRAME && C_use == COLLISIONUSE_PLAYER)
							{
								DamageHealth(1);//�P�_���[�W��^��������
							}
						}
					}
					else if (CollisionType == COLLISIONTYPE_LENGTH)
					{
						if (pPos->x + fWidth / 2 >= g_aBlock[nCntBlock].pos.x - g_aBlock[nCntBlock].fRadius//�v���C���[�̉E�[���u���b�N�̍��[�������Ȃ�
							&& pPosOld->x + fWidth / 2 <= g_aBlock[nCntBlock].pos.x - g_aBlock[nCntBlock].fRadius//1F�O�̃v���C���[�̉E�[���u���b�N�̉E�[���
							&& pPos->y - fHeight < g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fRadius//�v���C���[�̏�[���u���b�N�̉��[����Ȃ�
							&& pPos->y > g_aBlock[nCntBlock].pos.y - g_aBlock[nCntBlock].fRadius)//�v���C���[�̉��[���u���b�N�̏�[��艺�Ȃ�
						{//�v���C���[�̉E�[���u���b�N�̒��ɓ�������

							*bIsWalling = true;

							pPos->x = g_aBlock[nCntBlock].pos.x - g_aBlock[nCntBlock].fRadius - fWidth / 2;
							pMove->x = -0.0001f;


						}
						else if (pPos->x - fWidth / 2 <= g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fRadius//�v���C���[�̍��[���u���b�N�̉E�[�������Ȃ�
							&& pPosOld->x - fWidth / 2 >= g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fRadius//1F�O�̃v���C���[�̍��[���u���b�N�̉E�[���O���Ȃ�
							&& pPos->y - fHeight < g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fRadius//�v���C���[�̏�[���u���b�N�̉��[�������Ȃ�
							&& pPos->y > g_aBlock[nCntBlock].pos.y - g_aBlock[nCntBlock].fRadius)//�v���C���[�̉��[���u���b�N�̏�[�������Ȃ�
						{//�v���C���[�̍��[���u���b�N�̒��ɓ�������
							*bIsWalling = true;

							pPos->x = g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fRadius + fWidth / 2;
							pMove->x = 0.0001f;



						}
						else if (pPos->x + fWidth / 2 - 20.0f > g_aBlock[nCntBlock].pos.x - g_aBlock[nCntBlock].fRadius
							&& pPos->x - fWidth / 2 + 20.0f < g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fRadius
							&& pPos->y >= g_aBlock[nCntBlock].pos.y - g_aBlock[nCntBlock].fRadius
							&& pPosOld->y <= g_aBlock[nCntBlock].pos.y - g_aBlock[nCntBlock].fRadius)
						{//�v���C���[�̉��[���A�u���b�N�̏�[��艺�Ȃ�

							if (g_aBlock[nCntBlock].nType == BLOCKTYPE_ARROW && *bIsWalling == false)
							{
								pPos->x = g_aBlock[nCntBlock].pos.x;//�v���C���[�̈ʒu���u���b�N�̏�[�Ɉړ�������B

							}

							pPos->y = g_aBlock[nCntBlock].pos.y - g_aBlock[nCntBlock].fRadius;//�v���C���[�̈ʒu���u���b�N�̏�[�Ɉړ�������B
							pMove->y = 0.0f;//�d�͂�0�ɂ���
							*fGravity = 0.0f;//�d�͂̕␳�l��0�ɂ���@
							g_bIsLanding = true;//�n�ʂɂ���t���O�𗧂āA�W�����v���\�ɂ���

						}
						else if (pPos->x + fWidth / 2 - 20.0f > g_aBlock[nCntBlock].pos.x - g_aBlock[nCntBlock].fRadius
							&& pPos->x - fWidth / 2 + 20.0f < g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fRadius
							&& pPosOld->y - fHeight >= g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fRadius
							&& pPos->y - fHeight <= g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fRadius)
						{//PosOld�̓����u���b�N�̉��[����ŁA�v���C���[��pos���u���b�N�̉��[��艺�Ȃ�E�E�E
							pPos->y = g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fRadius + fHeight;//�v���C���[�̈ʒu���u���b�N�̉��[�Ɉړ�������B
							pMove->y = 0.0f;//�d�͂��O�ɂ���

						}
					}

				}
			}
		}
	}

		return g_bIsLanding;
	
}
