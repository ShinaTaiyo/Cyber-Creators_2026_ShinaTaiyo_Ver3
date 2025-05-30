//========================================================
//
//�W���R�O���F�G�t�F�N�g������񌩒���[Item.cpp]
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
#include "Item.h"
#include "sound.h"
#include "health.h"
#include "Treasure.h"
#include "fade.h"
#include "timer.h"
#include "enemy.h"
//�}�N����`
#include "input.h"
#define NUM_ITEMTEX (6)//�A�C�e���̎��
#define MAX_Item (5000)
#define ITEM_SIZE (50.0f)

#define SCOREBONUS_SIZE (300.0f)
#define APPER_ItemSIZE (20.0f)
#define PRESSSTART_ItemSIZE (20.0f)

#define SHIELD_SIZE (100.0f)


//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_apTextureItem[NUM_ITEMTEX] = {};
//D3DXVECTOR3 g_rotPI;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffItem = NULL;
//Enemy g_Item[MAX_ENEMY];//�G�̏��
//Result g_aResult;

Item g_Item[MAX_Item];
ItemEffect g_aItemEffect;

//int g_nCountTime = 0;//���Ԃ�\���O���[�o���ϐ�

//=====================================================
//==========================
//�G�̏���������
//==========================
void InitItem(void)
{



	//srand((unsigned int)time(NULL));//�Q�[�����J�n�������Ԗ��ɗ����̎��ݒ�



	LPDIRECT3DDEVICE9 pDevice;
	int	nCntItem;



	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Heal_Item_000.png",
		&g_apTextureItem[ITEMTYPE_HEAL]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\ReafOrb.png",
		&g_apTextureItem[ITEMTYPE_REAFORB]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\IceOrb.png",
		&g_apTextureItem[ITEMTYPE_ICEORB]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\FireOrb.png",
		&g_apTextureItem[ITEMTYPE_FIREORB]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\ThunderOrb.png",
		&g_apTextureItem[ITEMTYPE_THUNDERORB]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\LastOrb.png",
		&g_apTextureItem[ITEMTYPE_LASTORB]);

	
	//�G���̏�����
	for (nCntItem = 0; nCntItem < MAX_Item; nCntItem++)
	{
		
		g_Item[nCntItem].bUse = false;
		g_Item[nCntItem].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Item[nCntItem].HealPoint = 0;
	}

	//g_Item->pos = D3DXVECTOR3(0.0f,0.0f, 0.0f);//�ʒu������������

	//�`���[�g���A�����̏�����
	//for (nCntItem = 0; nCntItem < NUM_ITEMTEX; nCntItem++)
	//{
	//	
	//}


	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4*MAX_Item,// * NUM_ITEMTEX,//�S�͕K�v�Ȓ��_���B����g���摜����2�������A��̃|���S���ł��̂ŁA���_�S�ő��v�B
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffItem,
		NULL);

	VERTEX_2D* pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntItem = 0; nCntItem < MAX_Item; nCntItem++)
	{
		//���_���W�̐ݒ�(�G�̑傫���j
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f,0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f,0.0f, 0.0f);

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
	g_pVtxBuffItem->Unlock();
}

//====================================
//�`���[�g���A���̏I������
//====================================
void UninitItem(void)
{
	//�e�N�X�`���̔j��
	int nCntItem;
	for (nCntItem = 0; nCntItem < NUM_ITEMTEX; nCntItem++)
	{
		if (g_apTextureItem[nCntItem] != NULL)
		{
			g_apTextureItem[nCntItem]->Release();
			g_apTextureItem[nCntItem] = NULL;
		}
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffItem != NULL)
	{
		g_pVtxBuffItem->Release();
		g_pVtxBuffItem = NULL;
	}
}


//========================================
//�G�̍X�V����
//========================================
void UpdateItem(void)
{
	VERTEX_2D* pVtx;

	int nCntItem;

	Player* pPlayer = GetPlayer();


	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntItem = 0; nCntItem < MAX_Item; nCntItem++)
	{
		if (g_Item[nCntItem].bUse == true)
		{
			//���_���W�̐ݒ�(�G�̑傫���j
			pVtx[0].pos = D3DXVECTOR3(g_Item[nCntItem].pos.x - ITEM_SIZE, g_Item[nCntItem].pos.y - ITEM_SIZE, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_Item[nCntItem].pos.x + ITEM_SIZE, g_Item[nCntItem].pos.y - ITEM_SIZE, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_Item[nCntItem].pos.x - ITEM_SIZE, g_Item[nCntItem].pos.y + ITEM_SIZE, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_Item[nCntItem].pos.x + ITEM_SIZE, g_Item[nCntItem].pos.y + ITEM_SIZE, 0.0f);
			if (g_Item[nCntItem].type == ITEMTYPE_HEAL)
			{



				if (pPlayer->pos.x >= g_Item[nCntItem].pos.x - ITEM_SIZE - pPlayer->fWidth &&
					pPlayer->pos.x <= g_Item[nCntItem].pos.x + ITEM_SIZE + pPlayer->fWidth &&
					pPlayer->pos.y - pPlayer->fHeight / 2 >= g_Item[nCntItem].pos.y - ITEM_SIZE - pPlayer->fHeight / 2 &&
					pPlayer->pos.y - pPlayer->fHeight / 2 <= g_Item[nCntItem].pos.y + ITEM_SIZE + pPlayer->fHeight / 2)
				{//�v���C���[�̒��S�_���ǂ̈ʒu�ɂ��邩
					g_Item[nCntItem].bUse = false;
					SetHealing(g_Item[nCntItem].HealPoint);
					PlaySound(SOUND_LABEL_SE_HEAL_000);
				}

			}
			else if (g_Item[nCntItem].type == ITEMTYPE_REAFORB ||
				g_Item[nCntItem].type == ITEMTYPE_ICEORB ||
				g_Item[nCntItem].type == ITEMTYPE_FIREORB ||
				g_Item[nCntItem].type == ITEMTYPE_THUNDERORB ||
				g_Item[nCntItem].type == ITEMTYPE_LASTORB)
			{
				if (pPlayer->pos.x >= g_Item[nCntItem].pos.x - ITEM_SIZE - pPlayer->fWidth &&
					pPlayer->pos.x <= g_Item[nCntItem].pos.x + ITEM_SIZE + pPlayer->fWidth &&
					pPlayer->pos.y - pPlayer->fHeight / 2 >= g_Item[nCntItem].pos.y - ITEM_SIZE - pPlayer->fHeight / 2 &&
					pPlayer->pos.y - pPlayer->fHeight / 2 <= g_Item[nCntItem].pos.y + ITEM_SIZE + pPlayer->fHeight / 2)
				{//�v���C���[�̒��S�_���ǂ̈ʒu�ɂ��邩

					PlaySound(SOUND_LABEL_SE_ORBGET);
					g_Item[nCntItem].bUse = false;

					if (g_Item[nCntItem].type == ITEMTYPE_REAFORB)
					{
						SetTreasure(TREASURETYPE_REAF, -1, D3DXVECTOR3(1145.0f, 50.0f, 0.0f));
					}
					if (g_Item[nCntItem].type == ITEMTYPE_FIREORB)
					{
						
						SetTreasure(TREASURETYPE_FIRE, -1, D3DXVECTOR3(1145.0f, 15.0f, 0.0f));
					}
					if (g_Item[nCntItem].type == ITEMTYPE_ICEORB)
					{
						SetTreasure(TREASURETYPE_ICE, -1, D3DXVECTOR3(1257.0f, 50.0f, 0.0f));
					}
					if (g_Item[nCntItem].type == ITEMTYPE_THUNDERORB)
					{
						SetTreasure(TREASURETYPE_THUNDER, -1, D3DXVECTOR3(1257.0f, 15.0f, 0.0f));
					}
					if (g_Item[nCntItem].type == ITEMTYPE_LASTORB)
					{
						SetTreasure(TREASURETYPE_LAST, -1, D3DXVECTOR3(1200.0f, 35.0f, 0.0f));
						AddScore(1000000);
						int nData = GetScore();
						int nTimer = GetTimer();
						int nDefeatEnemy = DefeatEnemy();
						AddScore((nData * (nTimer / 100)) * (1 + (nDefeatEnemy / 100)) );
						SetGameState(GAMESTATE_END);
					}
				}
			}
		}
		pVtx += 4;//���_�f�[�^�̃|�C���^���S���i�߂�B
	}
		//���_�o�b�t�@���A�����b�N����
		g_pVtxBuffItem->Unlock();

	
}


//=========================================
//�G�̕`�揈��
//=========================================
void DrawItem(void)
{

	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

	int nCntItem;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffItem, 0, sizeof(VERTEX_2D));//���_�o�b�t�@�ւ̃|�C���^�ƒ��_���̍\���̂̃T�C�Y

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�

	for (nCntItem = 0; nCntItem < MAX_Item; nCntItem++)
	{
		if (g_Item[nCntItem].bUse == true)
		{
			
				if (g_Item[nCntItem].type == ITEMTYPE_HEAL)
				{
					pDevice->SetTexture(0, g_apTextureItem[ITEMTYPE_HEAL]);
				}
				if (g_Item[nCntItem].type == ITEMTYPE_REAFORB)
				{
					pDevice->SetTexture(0, g_apTextureItem[ITEMTYPE_REAFORB]);
				}
				if (g_Item[nCntItem].type == ITEMTYPE_ICEORB)
				{
					pDevice->SetTexture(0, g_apTextureItem[ITEMTYPE_ICEORB]);
				}
				if (g_Item[nCntItem].type == ITEMTYPE_FIREORB)
				{
					pDevice->SetTexture(0, g_apTextureItem[ITEMTYPE_FIREORB]);
				}
				if (g_Item[nCntItem].type == ITEMTYPE_THUNDERORB)
				{
					pDevice->SetTexture(0, g_apTextureItem[ITEMTYPE_THUNDERORB]);
				}
				if (g_Item[nCntItem].type == ITEMTYPE_LASTORB)
				{
					pDevice->SetTexture(0, g_apTextureItem[ITEMTYPE_LASTORB]);
				}
				
				//�|���S���̕`��
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntItem * 4, 2);//0�́A�`�悷��ŏ��̒��_�C���f�b�N�X�ŁA2�́A�`�悷��v���~�e�B�u��

				//&g_pVtxBuffPolygon[0],//���_���̐擪�A�h���X
				//sizeof(VERTEX_2D));//���_���\���̂̃T�C�Y
			
		}
	}


}

void SetItem(ITEMTYPE type, D3DXVECTOR3 pos, int HealPoint)
{
	int nCntItem;

	VERTEX_2D* pVtx;

	/*Enemy* pEnemy = GetEnemy();*/

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntItem = 0; nCntItem < MAX_Item; nCntItem++)
	{
		if (g_Item[nCntItem].bUse == false)
		{
			g_Item[nCntItem].pos = pos;//SetEnemy�̎��ɁA�G�̈ʒu��������ꂽ�ہA�ʒu�̈����������SetItem��pos�ɑ�����邱�Ƃɂ��A
			g_Item[nCntItem].type = type;//���o�̎�ނ��Z�b�g����B
			g_Item[nCntItem].bUse = true;
			g_Item[nCntItem].HealPoint = HealPoint;
			break;
		}
		pVtx += 4;
	}
	g_pVtxBuffItem->Unlock();
}

ItemEffect* GetItemEffect(void)
{
	return &g_aItemEffect;
}

Item* GetItem(void)
{
	return &g_Item[0];
}

