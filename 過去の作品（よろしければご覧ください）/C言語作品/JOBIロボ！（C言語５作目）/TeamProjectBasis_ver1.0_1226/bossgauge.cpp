//========================================================
//
//�W���R�O���F�G�t�F�N�g������񌩒���[BossGaugeFrame.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#include "main.h"
#include "scrap.h"
#include "BossGauge.h"
#include "player.h"
#include "enemy.h"
#include "boss.h"

//�}�N����`
#define BOSSGAUGEFRAME_HEIGHT (70.0f)  //�Q�[�W�t���[���̒���
#define REDUCATIONBOSSGAUGE (2.0f) //�Q�[�W��Vtx�𐔒l������������B
#define BOSSGAUGEFRAME_WIDTH (600.0f) //�Q�[�W�̃t���[���̒���

//�O���[�o���ϐ�
#define BOSSGAUGE_HEIGHT (30.0f) //�Q�[�W�̍���
#define BOSSGAUGE_WIDTH (600.0f)//�Q�[�W�̒��� 

//====================================
//�Q�[�W�t���[��
//====================================
LPDIRECT3DTEXTURE9 g_pTextureBossGaugeFrame[NUM_BOSSGAUGEFRAME] = {};//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBossGaugeFrame = NULL;//�Q�[�W�t���[���̏��
BossGaugeFrame g_aBossGaugeFrame[MAX_BOSSGAUGEFRAME];

//====================================
//�Q�[�W
//====================================
LPDIRECT3DTEXTURE9 g_pTextureBossGauge = NULL;//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBossGauge = NULL;
BossGauge g_aBossGauge;//�Q�[�W���

//===========================
//�Q�[�W�̏���������
//===========================
void InitBossGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//===============================
	//�Q�[�W�t���[���̏���
	//===============================

	//�e�N�X�`��
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\BossGaugeFrame_000.png",
		&g_pTextureBossGaugeFrame[BOSSGAUGEFRAME00_HIDE]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\BossGaugeFrame_001.png",
		&g_pTextureBossGaugeFrame[BOSSGAUGEFRAME01_APPER]);

	for (int nCntBossGaugeFrame = 0; nCntBossGaugeFrame < MAX_BOSSGAUGEFRAME; nCntBossGaugeFrame++)
	{
		g_aBossGaugeFrame[nCntBossGaugeFrame].nType = 0;                          //���
		g_aBossGaugeFrame[nCntBossGaugeFrame].fBossGaugeLength = 0.0f;                //�Q�[�W�̒���
		g_aBossGaugeFrame[nCntBossGaugeFrame].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�ʒu
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BOSSGAUGEFRAME,//�S�͕K�v�Ȓ��_��
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBossGaugeFrame,
		NULL);

	VERTEX_2D* pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffBossGaugeFrame->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBossGaugeFrame = 0; nCntBossGaugeFrame < MAX_BOSSGAUGEFRAME; nCntBossGaugeFrame++)
	{
		g_aBossGaugeFrame[nCntBossGaugeFrame].pos = D3DXVECTOR3(BOSSGAUGE_POS_X - (BOSSGAUGEFRAME_WIDTH / 2.0f), BOSSGAUGE_POS_Y, 0.0f);//�ʒu�����߂�
		g_aBossGaugeFrame[nCntBossGaugeFrame].nType = nCntBossGaugeFrame;//��ނ����߂�

		switch (nCntBossGaugeFrame)
		{
		case BOSSGAUGEFRAME00_HIDE:
			g_aBossGaugeFrame[nCntBossGaugeFrame].bUse = true;
			break;
		case BOSSGAUGEFRAME01_APPER:
			g_aBossGaugeFrame[nCntBossGaugeFrame].bUse = false;
			break;
		}
	
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aBossGaugeFrame[nCntBossGaugeFrame].pos.x - REDUCATIONBOSSGAUGE, g_aBossGaugeFrame[nCntBossGaugeFrame].pos.y - REDUCATIONBOSSGAUGE, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aBossGaugeFrame[nCntBossGaugeFrame].pos.x + BOSSGAUGEFRAME_WIDTH + REDUCATIONBOSSGAUGE, g_aBossGaugeFrame[nCntBossGaugeFrame].pos.y - REDUCATIONBOSSGAUGE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aBossGaugeFrame[nCntBossGaugeFrame].pos.x - REDUCATIONBOSSGAUGE, g_aBossGaugeFrame[nCntBossGaugeFrame].pos.y + BOSSGAUGEFRAME_HEIGHT + REDUCATIONBOSSGAUGE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aBossGaugeFrame[nCntBossGaugeFrame].pos.x + BOSSGAUGEFRAME_WIDTH + REDUCATIONBOSSGAUGE, g_aBossGaugeFrame[nCntBossGaugeFrame].pos.y + BOSSGAUGEFRAME_HEIGHT + REDUCATIONBOSSGAUGE, 0.0f);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;//���_�f�[�^�̃|�C���^���S���i�߂�B
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBossGaugeFrame->Unlock();

	//===================================================
	//�Q�[�W�\�L����
	//===================================================
	g_aBossGauge.fBossGauge = 0.0f;                     //�Q�[�W
	g_aBossGauge.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ʒu
	g_aBossGauge.fBossGaugeLength = 0.0f;				//�Q�[�W�̑������
	g_aBossGauge.nBossGaugeNum = 0;						//�Q�[�W�̔ԍ�
	g_aBossGauge.bUse = false;							//�g�p���Ă��邩�ǂ���

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"",
		&g_pTextureBossGauge);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BOSSGAUGEFRAME,//�S�͕K�v�Ȓ��_��
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBossGauge,
		NULL);

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffBossGauge->Lock(0, 0, (void**)&pVtx, 0);

	g_aBossGauge.pos = D3DXVECTOR3(BOSSGAUGE_POS_X - ((BOSSGAUGE_WIDTH - 5.0f) / 2.0f), BOSSGAUGE_POS_Y + 39.0f, 0.0f);	//�ʒu

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_aBossGauge.pos.x, g_aBossGauge.pos.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aBossGauge.pos.x + (BOSSGAUGE_WIDTH - 5.0f), g_aBossGauge.pos.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aBossGauge.pos.x, g_aBossGauge.pos.y + BOSSGAUGE_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aBossGauge.pos.x + (BOSSGAUGE_WIDTH - 5.0f), g_aBossGauge.pos.y + BOSSGAUGE_HEIGHT, 0.0f);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBossGauge->Unlock();
}

//�Q�[�W�̏I������
void UninitBossGauge(void)
{
	//=====================================
	//�Q�[�W�t���[���̏���������
	//=====================================
	for (int nCntBossGaugeFrame = 0; nCntBossGaugeFrame < NUM_BOSSGAUGEFRAME; nCntBossGaugeFrame++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureBossGaugeFrame[nCntBossGaugeFrame] != NULL)
		{
			g_pTextureBossGaugeFrame[nCntBossGaugeFrame]->Release();
			g_pTextureBossGaugeFrame[nCntBossGaugeFrame] = NULL;
		}
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffBossGaugeFrame != NULL)
	{
		g_pVtxBuffBossGaugeFrame->Release();
		g_pVtxBuffBossGaugeFrame = NULL;
	}
	
	//======================================
	//�Q�[�W�̏I������
	//======================================

	//�e�N�X�`���̔j��
	if (g_pTextureBossGauge != NULL)
	{
		g_pTextureBossGauge->Release();
		g_pTextureBossGauge = NULL;
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffBossGauge != NULL)
	{
		g_pVtxBuffBossGauge->Release();
		g_pVtxBuffBossGauge = NULL;
	}
}

//�Q�[�W�̍X�V����
void UpdateBossGauge(void)
{
	Boss* pBoss = GetBoss();//�{�X�̍\���̂̏����擾
	VERTEX_2D* pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffBossGauge->Lock(0, 0, (void**)&pVtx, 0);

	if (g_aBossGauge.bUse == true)
	{
			g_aBossGauge.fBossGauge = (((BOSSGAUGE_WIDTH - 5.0f)) / (float)(pBoss->nMaxHp)) * (float)(pBoss->nMaxHp - pBoss->nHp);
		
			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aBossGauge.pos.x, g_aBossGauge.pos.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBossGauge.pos.x + (BOSSGAUGE_WIDTH - 5.0f) - g_aBossGauge.fBossGauge, g_aBossGauge.pos.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBossGauge.pos.x, g_aBossGauge.pos.y + BOSSGAUGE_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBossGauge.pos.x + (BOSSGAUGE_WIDTH - 5.0f) - g_aBossGauge.fBossGauge, g_aBossGauge.pos.y + BOSSGAUGE_HEIGHT, 0.0f);
		
			float fColor = (1.0f / (float)(pBoss->nMaxHp)) * (float)(pBoss->nHp);
		
			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, fColor, fColor, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, fColor, fColor, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, fColor, fColor, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, fColor, fColor, 1.0f);
		
			if (g_aBossGauge.fBossGauge >= (BOSSGAUGE_WIDTH - 5.0f))
			{
				g_aBossGauge.bUse = false;
				g_aBossGauge.nEnemyNum = -1;
				g_aBossGaugeFrame[BOSSGAUGEFRAME01_APPER].bUse = false;
				g_aBossGaugeFrame[BOSSGAUGEFRAME00_HIDE].bUse = true;
			}
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBossGauge->Unlock();
}

//�Q�[�W�̕`�揈��
void DrawBossGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBossGaugeFrame, 0, sizeof(VERTEX_2D));//���_�o�b�t�@�ւ̃|�C���^�ƒ��_���̍\���̂̃T�C�Y

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//================================================
	//�Q�[�W�t���[���̕`�揈��
	//================================================

	for (int nCntBossGaugeFrame = 0; nCntBossGaugeFrame < MAX_BOSSGAUGEFRAME; nCntBossGaugeFrame++)
	{
		if (g_aBossGauge.bUse == true && g_aBossGaugeFrame[nCntBossGaugeFrame].bUse == true)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureBossGaugeFrame[g_aBossGaugeFrame[nCntBossGaugeFrame].nType]);

			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,//0�́A�`�悷��ŏ��̒��_�C���f�b�N�X�ŁA2�́A�`�悷��v���~�e�B�u��
				nCntBossGaugeFrame * 4,
				2);
		}
	}

	//================================================
	//�Q�[�W�̕`�揈��
	//================================================

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBossGauge, 0, sizeof(VERTEX_2D));//���_�o�b�t�@�ւ̃|�C���^�ƒ��_���̍\���̂̃T�C�Y

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_aBossGauge.bUse == true)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureBossGauge);

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,//0�́A�`�悷��ŏ��̒��_�C���f�b�N�X�ŁA2�́A�`�悷��v���~�e�B�u��
			0,
			2);
	}
}

//=============================================================================
//�{�X�̃Q�[�W�Z�b�g����(�{�X���o������)
//=============================================================================
void SetBossGauge(int nEnemyNum)//�{�X�̃Q�[�W���o�������鏈��
{
	if (g_aBossGaugeFrame[BOSSGAUGEFRAME01_APPER].bUse == false && g_aBossGauge.bUse == false)
	{
		for (int nCntBossGauge = 0; nCntBossGauge < MAX_BOSSGAUGEFRAME; nCntBossGauge++)
		{
			switch (nCntBossGauge)
			{
				//�{�X���o�������̂ŁA�{�X���B��Ă��鎞�̃Q�[�W��j��
			case BOSSGAUGEFRAME00_HIDE:
				g_aBossGaugeFrame[nCntBossGauge].bUse = false;
				break;

				//�{�X�o���p�̃Q�[�W���Z�b�g
			case BOSSGAUGEFRAME01_APPER:
				g_aBossGaugeFrame[nCntBossGauge].bUse = true;
				break;
			}
		}

		g_aBossGauge.bUse = true;
		g_aBossGauge.nEnemyNum = nEnemyNum;
	}
}
