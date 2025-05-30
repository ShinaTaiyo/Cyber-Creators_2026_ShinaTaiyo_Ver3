//========================================================
//
//�W���R�O���F�G�t�F�N�g������񌩒���[GaugeFrame.cpp]
//Author:ShinaTaiyo
//
//=========================================================

#include "main.h"
#include "scrap.h"
#include "gauge.h"
#include "player.h"

//�}�N����`
#define GAUGEFRAME_HEIGHT (40.0f)  //�Q�[�W�t���[���̒���
#define REDUCATIONGAUGE (2.0f) //�Q�[�W��Vtx�𐔒l������������B
#define GAUGEFRAME_WIDTH (200.0f + (REDUCATIONGAUGE * 2)) //�Q�[�W�̃t���[���̒���
//�O���[�o���ϐ�

//====================================
//�Q�[�W�t���[��
//====================================
LPDIRECT3DTEXTURE9 g_pTextureGaugeFrame[NUM_GAUGEFRAME] = {};//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGaugeFrame = NULL;//�Q�[�W�t���[���̏��
GaugeFrame g_aGaugeFrame[MAX_GAUGEFRAME];
//==============================================================================================

//====================================
//�Q�[�W
//====================================
LPDIRECT3DTEXTURE9 g_pTextureGauge = NULL;//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGauge = NULL;

Gauge g_aGauge[MAX_GAUGE];                     //�Q�[�W���
//==============================================================================================

//===========================
//�Q�[�W�̏���������
//===========================
void InitGauge(void)
{


	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//===============================
	//�Q�[�W�t���[���̏���
	//===============================

	//�e�N�X�`��
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\LvUpGaugeFrame_000.png",
		&g_pTextureGaugeFrame[SCRAPGAUGEFRAMETYPE00_TIBOROBO]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\LvUpGaugeFrame_001.png",
		&g_pTextureGaugeFrame[SCRAPGAUGEFRAMETYPE01_SWORD]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\LvUpGaugeFrame_002.png",
		&g_pTextureGaugeFrame[SCRAPGAUGEFRAMETYPE02_BOOMERANG]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\LvUpGaugeFrame_003.png",
		&g_pTextureGaugeFrame[SCRAPGAUGEFRAMETYPE03_SPEAKER]);
	
	for (int nCntGaugeFrame = 0; nCntGaugeFrame < MAX_GAUGEFRAME; nCntGaugeFrame++)
	{
		g_aGaugeFrame[nCntGaugeFrame].nType = 0;                          //���
		g_aGaugeFrame[nCntGaugeFrame].fGaugeLength = 0.0f;                //�Q�[�W�̒���
		g_aGaugeFrame[nCntGaugeFrame].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�ʒu
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_GAUGEFRAME,//�S�͕K�v�Ȓ��_��
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffGaugeFrame,
		NULL);

	VERTEX_2D* pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffGaugeFrame->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntGaugeFrame = 0; nCntGaugeFrame < MAX_GAUGEFRAME; nCntGaugeFrame++)
	{
		g_aGaugeFrame[nCntGaugeFrame].pos = D3DXVECTOR3(30.0f + 250.0f * nCntGaugeFrame, 660.0f, 0.0f);//�ʒu�����߂�
		g_aGaugeFrame[nCntGaugeFrame].nType = nCntGaugeFrame;//��ނ����߂�

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aGaugeFrame[nCntGaugeFrame].pos.x, g_aGaugeFrame[nCntGaugeFrame].pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aGaugeFrame[nCntGaugeFrame].pos.x + GAUGEFRAME_WIDTH,g_aGaugeFrame[nCntGaugeFrame].pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aGaugeFrame[nCntGaugeFrame].pos.x, g_aGaugeFrame[nCntGaugeFrame].pos.y + GAUGEFRAME_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aGaugeFrame[nCntGaugeFrame].pos.x + GAUGEFRAME_WIDTH, g_aGaugeFrame[nCntGaugeFrame].pos.y + GAUGEFRAME_HEIGHT, 0.0f);


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
	g_pVtxBuffGaugeFrame->Unlock();

	//==========================================================================================================================

	//===================================================
	//�Q�[�W�\�L����
	//===================================================

	for (int nCntGauge = 0; nCntGauge < MAX_GAUGEFRAME; nCntGauge++)
	{
		g_aGauge[nCntGauge].fGauge = 0.0f;                      //�Q�[�W
		g_aGauge[nCntGauge].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�ʒu
		g_aGauge[nCntGauge].fGaugeLength = 0.0f;      //�Q�[�W�̑������
		g_aGauge[nCntGauge].nGaugeNum = 0;            //�Q�[�W�̔ԍ�
	}

	//(�e�N�X�`���͎��ۂɂ͎g��Ȃ����A���܂ł̏������ł��Ȃ�A�|���S�����o�����߂ɕK�v�I�j
	D3DXCreateTextureFromFile(pDevice,
		"ffffff",
		&g_pTextureGauge);


	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_GAUGEFRAME,//�S�͕K�v�Ȓ��_��
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffGauge,
		NULL);

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffGauge->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntGauge = 0; nCntGauge < MAX_GAUGEFRAME; nCntGauge++)
	{
		g_aGauge[nCntGauge].pos = D3DXVECTOR3(30.0f + 250.0f * nCntGauge, 660.0f, 0.0f);//�ʒu�����߂�
		g_aGauge[nCntGauge].nGaugeNum = nCntGauge;
		D3DXCOLOR col;

		switch (nCntGauge)
		{
		case 0:
			col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			break;
		case 1:
			col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
			break;
		case 2:
			col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
			break;
		case 3:
			col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			break;
		default:
			break;
		}

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aGauge[nCntGauge].pos.x + REDUCATIONGAUGE, g_aGauge[nCntGauge].pos.y + REDUCATIONGAUGE, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aGauge[nCntGauge].pos.x + GAUGEFRAME_WIDTH - REDUCATIONGAUGE, g_aGauge[nCntGauge].pos.y + REDUCATIONGAUGE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aGauge[nCntGauge].pos.x + REDUCATIONGAUGE, g_aGauge[nCntGauge].pos.y + GAUGEFRAME_HEIGHT - REDUCATIONGAUGE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aGauge[nCntGauge].pos.x + GAUGEFRAME_WIDTH - REDUCATIONGAUGE, g_aGauge[nCntGauge].pos.y - REDUCATIONGAUGE, 0.0f);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = col;
		pVtx[1].col = col;
		pVtx[2].col = col;
		pVtx[3].col = col;

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx += 4;//���_�f�[�^�̃|�C���^���S���i�߂�B
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffGauge->Unlock();
}

//�Q�[�W�̏I������
void UninitGauge(void)
{

	//=====================================
	//�Q�[�W�t���[���̏���������
	//=====================================

	for (int nCntGaugeFrame = 0; nCntGaugeFrame < NUM_GAUGEFRAME; nCntGaugeFrame++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureGaugeFrame[nCntGaugeFrame] != NULL)
		{
			g_pTextureGaugeFrame[nCntGaugeFrame]->Release();
			g_pTextureGaugeFrame[nCntGaugeFrame] = NULL;
		}
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffGaugeFrame != NULL)
	{
		g_pVtxBuffGaugeFrame->Release();
		g_pVtxBuffGaugeFrame = NULL;
	}
	//================================================================================================================================

	//======================================
	//�Q�[�W�̏I������
	//======================================
	
	//�e�N�X�`���̔j��
	if (g_pTextureGauge != NULL)
	{
		g_pTextureGauge->Release();
		g_pTextureGauge = NULL;
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffGauge != NULL)
	{
		g_pVtxBuffGauge->Release();
		g_pVtxBuffGauge = NULL;
	}
	//=================================================================================================================================


}

//�Q�[�W�̍X�V����
void UpdateGauge(void)
{
	VERTEX_2D* pVtx;
	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffGauge->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntGauge = 0; nCntGauge < MAX_GAUGE; nCntGauge++)
	{
		
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aGauge[nCntGauge].pos.x + REDUCATIONGAUGE, g_aGauge[nCntGauge].pos.y + REDUCATIONGAUGE, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aGauge[nCntGauge].pos.x + g_aGauge[nCntGauge].fGauge - REDUCATIONGAUGE, g_aGauge[nCntGauge].pos.y + REDUCATIONGAUGE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aGauge[nCntGauge].pos.x + REDUCATIONGAUGE, g_aGauge[nCntGauge].pos.y + GAUGEFRAME_HEIGHT - REDUCATIONGAUGE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aGauge[nCntGauge].pos.x + g_aGauge[nCntGauge].fGauge - REDUCATIONGAUGE, g_aGauge[nCntGauge].pos.y + GAUGEFRAME_HEIGHT - REDUCATIONGAUGE, 0.0f);

		if (g_aGauge[nCntGauge].fGauge >= (GAUGEFRAME_WIDTH - 4.0f))
		{
			g_aGauge[nCntGauge].fGauge = 0.0f;
		}
		pVtx += 4;

	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffGauge->Unlock();

}

//�Q�[�W�̕`�揈��
void DrawGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffGaugeFrame, 0, sizeof(VERTEX_2D));//���_�o�b�t�@�ւ̃|�C���^�ƒ��_���̍\���̂̃T�C�Y

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//================================================
	//�Q�[�W�t���[���̕`�揈��
	//================================================

	for (int nCntGaugeFrame = 0; nCntGaugeFrame < MAX_GAUGEFRAME; nCntGaugeFrame++)
	{
	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureGaugeFrame[g_aGaugeFrame[nCntGaugeFrame].nType]);

	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,//0�́A�`�悷��ŏ��̒��_�C���f�b�N�X�ŁA2�́A�`�悷��v���~�e�B�u��
		nCntGaugeFrame * 4,
		2);
	}

	//==============================================================================================================================================================================

	//================================================
	//�Q�[�W�̕`�揈��
	//================================================

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffGauge, 0, sizeof(VERTEX_2D));//���_�o�b�t�@�ւ̃|�C���^�ƒ��_���̍\���̂̃T�C�Y

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntGauge = 0; nCntGauge < MAX_GAUGE; nCntGauge++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureGauge);

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,//0�́A�`�悷��ŏ��̒��_�C���f�b�N�X�ŁA2�́A�`�悷��v���~�e�B�u��
			nCntGauge * 4,
			2);
	}
	//===================================================================================================================================================================================


}

//===================================
//�Q�[�W���Z����
//===================================
void AddGauge(int nType, int nScrap, int nNextLvScrap)
{
	VERTEX_2D* pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffGauge->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntGauge = 0; nCntGauge < MAX_GAUGE; nCntGauge++)
	{
		if (g_aGauge[nCntGauge].nGaugeNum == nType)//��ނ���v������
		{
			g_aGauge[nCntGauge].fGaugeLength = (GAUGEFRAME_WIDTH - 4.0f) / nNextLvScrap;//�Q�[�W�t���[���̒������A���̃��x���܂ł̃X�N���b�v���Ŋ���A1�X�N���b�v�ɕt���ǂꂮ�炢�Q�[�W�������邩�ǂ������m���߂�B
			g_aGauge[nCntGauge].fGauge = g_aGauge[nCntGauge].fGaugeLength * nScrap;
			break;
		}
		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffGauge->Unlock();
}
