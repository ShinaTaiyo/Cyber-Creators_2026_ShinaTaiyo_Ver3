//========================================================
//
//�P�O���P�X���F��l���\��[Treasure.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#include "main.h"
#include "bullet.h"
#include "explosion.h"
#include <stdlib.h>
#include <time.h>
#include "score.h"
#include "game.h"
#include "result.h"
#include "player.h"
#include "Treasure.h"
#include "sound.h"
#include "enemy.h"
#include "fade.h"
//�}�N����`
#include "input.h"
#define NUM_TREASURETEX (6)//��̎��
#define MAX_TREASURE (5000)

#define APPER_TREASURESIZE (50.0f)
#define PRESSSTART_TREASURESIZE (120.0f)
#define TREASURESIZE (20.0f)
#define ORBSIZE (20.0f)

#define TREASUREBOXSIZEX (80.0f)
#define TREASUREBOXSIZEY (30.0f)
//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_apTextureTreasure[NUM_TREASURETEX] = {};
//D3DXVECTOR3 g_rotPI;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTreasure = NULL;
//Enemy g_Treasure[MAX_ENEMY];//�G�̏��
//Result g_aResult;

Treasure g_Treasure[MAX_TREASURE];

//int g_nCountTime = 0;//���Ԃ�\���O���[�o���ϐ�

//=====================================================
//==========================
//�G�̏���������
//==========================
void InitTreasure(void)
{



	//srand((unsigned int)time(NULL));//�Q�[�����J�n�������Ԗ��ɗ����̎��ݒ�



	LPDIRECT3DDEVICE9 pDevice;
	int	nCntTreasure;



	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\GetTreasure.png",
		&g_apTextureTreasure[TREASURETYPE_BOX]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\ReafOrb.png",
		&g_apTextureTreasure[TREASURETYPE_REAF]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\IceOrb.png",
		&g_apTextureTreasure[TREASURETYPE_ICE]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\FireOrb.png",
		&g_apTextureTreasure[TREASURETYPE_FIRE]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\ThunderOrb.png",
		&g_apTextureTreasure[TREASURETYPE_THUNDER]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\LastOrb.png",
		&g_apTextureTreasure[TREASURETYPE_LAST]);


	//�G���̏�����
	for (nCntTreasure = 0; nCntTreasure < MAX_TREASURE; nCntTreasure++)
	{
		g_Treasure[nCntTreasure].nTreasureType = 0;
		g_Treasure[nCntTreasure].nLife = 0;
		g_Treasure[nCntTreasure].bUse = false;
		g_Treasure[nCntTreasure].bCountStartFlag = false;//���o���n�܂��Ă���̃J�E���g���J�n����t���O��錾����B
		g_Treasure[nCntTreasure].nTreasureCount = 0;//���o���n�܂��Ă���̃J�E���g������������B
		g_Treasure[nCntTreasure].fTreasurePatternCountY = 0.0;//�A�j���[�V������x�点��J�E���g
		g_Treasure[nCntTreasure].fTreasurePattern = 0.0f;//�A�j���[�V�����̎��
		g_Treasure[nCntTreasure].nAnimationRepeat = 0;//�A�j���[�V�������J��Ԃ��ϐ�
		g_Treasure[nCntTreasure].bDraw = true;//bUse��true���������Ƃɉ����A�X��bDraw��true���ǂ����ŕ`�悷�邩�ǂ��������߂�B
		g_Treasure[nCntTreasure].nCntEnemy = 0;
		g_Treasure[nCntTreasure].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Treasure[nCntTreasure].fColor_a = 0.0f;
		g_Treasure[nCntTreasure].bColorChange_a = false;
	}

	//g_Treasure->pos = D3DXVECTOR3(0.0f,0.0f, 0.0f);//�ʒu������������

	//�`���[�g���A�����̏�����
	//for (nCntTreasure = 0; nCntTreasure < NUM_TREASURETEX; nCntTreasure++)
	//{
	//	
	//}


	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TREASURE,// * NUM_TREASURETEX,//�S�͕K�v�Ȓ��_���B����g���摜����2�������A��̃|���S���ł��̂ŁA���_�S�ő��v�B
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTreasure,
		NULL);

	VERTEX_2D* pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffTreasure->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntTreasure = 0; nCntTreasure < MAX_TREASURE; nCntTreasure++)
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
	g_pVtxBuffTreasure->Unlock();
}

//====================================
//�`���[�g���A���̏I������
//====================================
void UninitTreasure(void)
{
	//�e�N�X�`���̔j��
	int nCntTreasure;
	for (nCntTreasure = 0; nCntTreasure < NUM_TREASURETEX; nCntTreasure++)
	{
		if (g_apTextureTreasure[nCntTreasure] != NULL)
		{
			g_apTextureTreasure[nCntTreasure]->Release();
			g_apTextureTreasure[nCntTreasure] = NULL;
		}
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffTreasure != NULL)
	{
		g_pVtxBuffTreasure->Release();
		g_pVtxBuffTreasure = NULL;
	}
}


//========================================
//�G�̍X�V����
//========================================
void UpdateTreasure(void)
{
	VERTEX_2D* pVtx;



	int nCntTreasure;


	Enemy* pEnemy = GetEnemy();


	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffTreasure->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntTreasure = 0; nCntTreasure < MAX_TREASURE; nCntTreasure++)
	{
		if (g_Treasure[nCntTreasure].bUse == true)
		{

			if (g_Treasure[nCntTreasure].nLife < 1)
			{
				g_Treasure[nCntTreasure].bUse = false;
			}

			g_Treasure[nCntTreasure].nTreasureCount++;//�o�����Ă���̎��Ԃ��J�E���g����

		

			if (g_Treasure[nCntTreasure].nTreasureType == TREASURETYPE_BOX)
			{
				pVtx[0].pos = D3DXVECTOR3(g_Treasure[nCntTreasure].pos.x - TREASUREBOXSIZEX, g_Treasure[nCntTreasure].pos.y - TREASUREBOXSIZEY, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_Treasure[nCntTreasure].pos.x + TREASUREBOXSIZEX, g_Treasure[nCntTreasure].pos.y - TREASUREBOXSIZEY, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_Treasure[nCntTreasure].pos.x - TREASUREBOXSIZEX, g_Treasure[nCntTreasure].pos.y + TREASUREBOXSIZEY, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_Treasure[nCntTreasure].pos.x + TREASUREBOXSIZEX, g_Treasure[nCntTreasure].pos.y + TREASUREBOXSIZEY, 0.0f);
			}
			if (g_Treasure[nCntTreasure].nTreasureType == TREASURETYPE_REAF ||
				g_Treasure[nCntTreasure].nTreasureType == TREASURETYPE_ICE ||
				g_Treasure[nCntTreasure].nTreasureType == TREASURETYPE_FIRE ||
				g_Treasure[nCntTreasure].nTreasureType == TREASURETYPE_THUNDER ||
				g_Treasure[nCntTreasure].nTreasureType == TREASURETYPE_LAST)
			{
				pVtx[0].pos = D3DXVECTOR3(g_Treasure[nCntTreasure].pos.x - ORBSIZE, g_Treasure[nCntTreasure].pos.y - ORBSIZE, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_Treasure[nCntTreasure].pos.x + ORBSIZE, g_Treasure[nCntTreasure].pos.y - ORBSIZE, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_Treasure[nCntTreasure].pos.x - ORBSIZE, g_Treasure[nCntTreasure].pos.y + ORBSIZE, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_Treasure[nCntTreasure].pos.x + ORBSIZE, g_Treasure[nCntTreasure].pos.y + ORBSIZE, 0.0f);
			}
		
			
		}
		pVtx += 4;//���_�f�[�^�̃|�C���^���S���i�߂�B
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTreasure->Unlock();


}


//=========================================
//�G�̕`�揈��
//=========================================
void DrawTreasure(void)
{

	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

	Enemy* pEnemy = GetEnemy();

	int nCntTreasure;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTreasure, 0, sizeof(VERTEX_2D));//���_�o�b�t�@�ւ̃|�C���^�ƒ��_���̍\���̂̃T�C�Y

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�

	for (nCntTreasure = 0; nCntTreasure < MAX_TREASURE; nCntTreasure++)
	{
		if (g_Treasure[nCntTreasure].bUse == true)
		{
			if (g_Treasure[nCntTreasure].bDraw == true)
			{
				if (g_Treasure[nCntTreasure].nTreasureType == TREASURETYPE_BOX)
				{
					pDevice->SetTexture(0, g_apTextureTreasure[TREASURETYPE_BOX]);
				}
				if (g_Treasure[nCntTreasure].nTreasureType == TREASURETYPE_REAF)
				{
					pDevice->SetTexture(0, g_apTextureTreasure[TREASURETYPE_REAF]);
				}
				if (g_Treasure[nCntTreasure].nTreasureType == TREASURETYPE_ICE)
				{
					pDevice->SetTexture(0, g_apTextureTreasure[TREASURETYPE_ICE]);
				}
				if (g_Treasure[nCntTreasure].nTreasureType == TREASURETYPE_FIRE)
				{
					pDevice->SetTexture(0, g_apTextureTreasure[TREASURETYPE_FIRE]);
				}
				if (g_Treasure[nCntTreasure].nTreasureType == TREASURETYPE_THUNDER)
				{
					pDevice->SetTexture(0, g_apTextureTreasure[TREASURETYPE_THUNDER]);
				}
				if (g_Treasure[nCntTreasure].nTreasureType == TREASURETYPE_LAST)
				{
					pDevice->SetTexture(0, g_apTextureTreasure[TREASURETYPE_LAST]);
				}
				
				//�|���S���̕`��
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTreasure * 4, 2);//0�́A�`�悷��ŏ��̒��_�C���f�b�N�X�ŁA2�́A�`�悷��v���~�e�B�u��

				//&g_pVtxBuffPolygon[0],//���_���̐擪�A�h���X
				//sizeof(VERTEX_2D));//���_���\���̂̃T�C�Y
			}
		}
	}


}

void SetTreasure(TREASURETYPE type, int nCntEnemy, D3DXVECTOR3 pos)
{
	int nCntTreasure;

	VERTEX_2D* pVtx;

	/*Enemy* pEnemy = GetEnemy();*/

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffTreasure->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntTreasure = 0; nCntTreasure < MAX_TREASURE; nCntTreasure++)
	{
		if (g_Treasure[nCntTreasure].bUse == false)
		{
			g_Treasure[nCntTreasure].pos = pos;//SetEnemy�̎��ɁA�G�̈ʒu��������ꂽ�ہA�ʒu�̈����������SetTreasure��pos�ɑ�����邱�Ƃɂ��A
			g_Treasure[nCntTreasure].nLife = 100;
			g_Treasure[nCntTreasure].nTreasureType = type;//���o�̎�ނ��Z�b�g����B
			g_Treasure[nCntTreasure].bUse = true;
			g_Treasure[nCntTreasure].nTreasureCount = 0;
			g_Treasure[nCntTreasure].bCountStartFlag = true;
			g_Treasure[nCntTreasure].fTreasurePatternCountY = 0.0f;
			g_Treasure[nCntTreasure].fTreasurePattern = 0.0f;
			g_Treasure[nCntTreasure].nAnimationRepeat = 0;
			g_Treasure[nCntTreasure].bDraw = true;
			g_Treasure[nCntTreasure].nCntEnemy = nCntEnemy;
			g_Treasure[nCntTreasure].fColor_a = 1.0f;
			g_Treasure[nCntTreasure].bColorChange_a = false;
			break;
		}
		pVtx += 4;
	}
	g_pVtxBuffTreasure->Unlock();
}
