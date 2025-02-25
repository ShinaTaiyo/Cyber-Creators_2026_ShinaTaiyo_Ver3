//========================================================
//
//�W���R�O���F�G�t�F�N�g������񌩒���[Warp.cpp]
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
#include "sound.h"
#include "fade.h"
#include "health.h"
#include "warp.h"
#include "map.h"
#include "input.h"
//�}�N����`
#define NUM_WarpTEX (10)//���[�v�̎��
#define FRAME_WarpSIZE (20.0f) //�����[�v�̑傫��



//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_apTextureWarp = NULL;
//D3DXVECTOR3 g_rotPI;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWarp = NULL;
//Enemy g_Warp[MAX_ENEMY];//�G�̏��
//Result g_aResult;

Warp g_aWarp[MAX_WARP];

//int g_nCountTime = 0;//���Ԃ�\���O���[�o���ϐ�

//=====================================================
//==========================
//���[�v�̏���������
//==========================
void InitWarp(void)
{
	//srand((unsigned int)time(NULL));//�Q�[�����J�n�������Ԗ��ɗ����̎��ݒ�
	LPDIRECT3DDEVICE9 pDevice;
	int	nCntWarp;
	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\WarpHole_000.png",
		&g_apTextureWarp);

	

	//���[�v���̏�����
	for (nCntWarp = 0; nCntWarp < MAX_WARP; nCntWarp++)
	{
		g_aWarp[nCntWarp].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//���[�v�̈ʒu
		g_aWarp[nCntWarp].PlayerPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//���[�v������v���C���[�̈ʒu
		g_aWarp[nCntWarp].nRoadMapType = 0;//���[�v�̎��
		g_aWarp[nCntWarp].nLife = 0;//���[�v�̃��C�t
		g_aWarp[nCntWarp].bUse = false;//���[�v���g���Ă��邩����
		g_aWarp[nCntWarp].bDraw = false;//bUse��true���������Ƃɉ����A�X��bDraw��true���ǂ����ŕ`�悷�邩�ǂ��������߂�B
		g_aWarp[nCntWarp].bMapChange = false;//�}�b�v��ς���Ƃ��ɌĂяo���t���O
		g_aWarp[nCntWarp].fXsize = 0.0f;//X�����̃T�C�Y
		g_aWarp[nCntWarp].fYsize = 0.0f;//Y�����̃T�C�Y
		g_aWarp[nCntWarp].fRadius = 0.0f;//���a�ŃT�C�Y�����߂�ꍇ�̃T�C�Y
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_WARP,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffWarp,
		NULL);

	VERTEX_2D* pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffWarp->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntWarp = 0; nCntWarp < MAX_WARP; nCntWarp++)
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
	g_pVtxBuffWarp->Unlock();
}

//====================================
//�`���[�g���A���̏I������
//====================================
void UninitWarp(void)
{
	//�e�N�X�`���̔j��
	int nCntWarp;
	if (g_apTextureWarp != NULL)
		{
			g_apTextureWarp->Release();
			g_apTextureWarp = NULL;
		}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffWarp != NULL)
	{
		g_pVtxBuffWarp->Release();
		g_pVtxBuffWarp = NULL;
	}
}


//========================================
//�G�̍X�V����
//========================================
void UpdateWarp(void)
{
	VERTEX_2D* pVtx;
	int nCntWarp;
	Player* pPlayer = GetPlayer();
	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffWarp->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntWarp = 0; nCntWarp < MAX_WARP; nCntWarp++)
	{
		if (g_aWarp[nCntWarp].bUse == true)
		{
			//���[�v�̈ʒu�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aWarp[nCntWarp].pos.x - g_aWarp[nCntWarp].fRadius, g_aWarp[nCntWarp].pos.y - g_aWarp[nCntWarp].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aWarp[nCntWarp].pos.x + g_aWarp[nCntWarp].fRadius, g_aWarp[nCntWarp].pos.y - g_aWarp[nCntWarp].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aWarp[nCntWarp].pos.x - g_aWarp[nCntWarp].fRadius, g_aWarp[nCntWarp].pos.y + g_aWarp[nCntWarp].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aWarp[nCntWarp].pos.x + g_aWarp[nCntWarp].fRadius, g_aWarp[nCntWarp].pos.y + g_aWarp[nCntWarp].fRadius, 0.0f);



			if (pPlayer->pos.x >= g_aWarp[nCntWarp].pos.x - g_aWarp[nCntWarp].fRadius
				&& pPlayer->pos.x <= g_aWarp[nCntWarp].pos.x + g_aWarp[nCntWarp].fRadius
				&& pPlayer->pos.y - PLAYER_HEIGHT / 2 >= g_aWarp[nCntWarp].pos.y - g_aWarp[nCntWarp].fRadius
				&& pPlayer->pos.y - PLAYER_HEIGHT / 2 <= g_aWarp[nCntWarp].pos.y + g_aWarp[nCntWarp].fRadius)
			{//�v���C���[�̒��S�Ŕ���B�}�b�v���Ăяo��
				if (g_aWarp[nCntWarp].bMapChange == true)
				{
					ResetWarp();
					g_aWarp[nCntWarp].bMapChange = false;//�}�b�v���Ăяo�����̂ŁA�t���O��off�ɂ���
					g_aWarp[nCntWarp].bUse = false;
					pPlayer->pos = g_aWarp[nCntWarp].PlayerPos;//�v���C���[�����[�v������
					pPlayer->posOld = g_aWarp[nCntWarp].PlayerPos;//�v���C���[�����[�v������
					SetMap(g_aWarp[nCntWarp].nRoadMapType);

				}
			}
		}
		pVtx += 4;//���_�f�[�^�̃|�C���^���S���i�߂�B
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffWarp->Unlock();


}


//=========================================
//�G�̕`�揈��
//=========================================
void DrawWarp(void)
{

	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

	int nCntWarp;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffWarp, 0, sizeof(VERTEX_2D));//���_�o�b�t�@�ւ̃|�C���^�ƒ��_���̍\���̂̃T�C�Y

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�

	for (nCntWarp = 0; nCntWarp < MAX_WARP; nCntWarp++)
	{

		if (g_aWarp[nCntWarp].bUse == true)
		{
			if (g_aWarp[nCntWarp].bDraw == true)
			{
				pDevice->SetTexture(0, g_apTextureWarp);
				
				//�|���S���̕`��
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntWarp * 4, 2);//0�́A�`�悷��ŏ��̒��_�C���f�b�N�X�ŁA2�́A�`�悷��v���~�e�B�u��

				//&g_pVtxBuffPolygon[0],//���_���̐擪�A�h���X
				//sizeof(VERTEX_2D));//���_���\���̂̃T�C�Y
			}
		}
	}


}

void ResetWarp(void)//���[�v����U�S������
{

	for (int nCntWarp = 0; nCntWarp < MAX_WARP; nCntWarp++)
	{
		if (g_aWarp[nCntWarp].bUse == true)
		{
			g_aWarp[nCntWarp].bUse = false;//���[�v���g���Ă��邩�ǂ���
		}

	}
}


void SetWarp(int nRoadMapType, D3DXVECTOR3 pos, D3DXVECTOR3 PlayerPos)
{
	int nCntWarp;

	VERTEX_2D* pVtx;

	Player* pPlayer = GetPlayer();
	/*Enemy* pEnemy = GetEnemy();*/



	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffWarp->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntWarp = 0; nCntWarp < MAX_WARP; nCntWarp++)
	{
		if (g_aWarp[nCntWarp].bUse == false)
		{
			g_aWarp[nCntWarp].pos = pos;
			g_aWarp[nCntWarp].PlayerPos = PlayerPos;//�v���C���[�����[�v������ʒu
			g_aWarp[nCntWarp].nRoadMapType = nRoadMapType;//���[�v�̎��
			g_aWarp[nCntWarp].nLife = 0;//���[�v�̃��C�t
			g_aWarp[nCntWarp].bUse = true;//���[�v���g���Ă��邩����
			g_aWarp[nCntWarp].bDraw = true;//bUse��true���������Ƃɉ����A�X��bDraw��true���ǂ����ŕ`�悷�邩�ǂ��������߂�B
			g_aWarp[nCntWarp].bMapChange = true;//�}�b�v��ς���Ƃ��ɌĂяo���t���O
			g_aWarp[nCntWarp].fRadius = 20.0f;//���a�ŃT�C�Y�����߂�ꍇ�̃T�C�Y
			break;
		}
		pVtx += 4;
	}
	g_pVtxBuffWarp->Unlock();
}

Warp* GetWarp(void)
{
	return &g_aWarp[0];
}


