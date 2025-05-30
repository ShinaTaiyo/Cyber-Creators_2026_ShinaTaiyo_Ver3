//========================================================
//
//�P�Q���Q�U���F�`�[������̊�Ս��[pause.cpp]
//Author:ShinaTaiyo
//
//=========================================================

#include "result.h"
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "pause.h"


#define COMMAND_SIZE_X (160.0f)
#define COMMAND_SIZE_Y (40.0f)

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTexturePauseBG = NULL;//�|�[�Y�w�i�e�N�X�`���ւ̃|�C���^
PDIRECT3DVERTEXBUFFER9 g_pVtxBuffPauseBG = NULL;//�|�[�Y�w�i�̒��_�o�b�t�@�ւ̃|�C���^

LPDIRECT3DTEXTURE9 g_pTexturePause[NUM_PAUSETEX] = {};//�e�N�X�`���ւ̃|�C���^
PDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;//���_�o�b�t�@�ւ̃|�C���^
Pause g_aPause[NUM_PAUSETEX];

//���U���g��ʂ̏���������
void InitPause(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//================================
	//�|�[�Y��ʔw�i�̏���������
	//================================
	

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\PauseBg_000.png",
		&g_pTexturePauseBG);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPauseBG,
		NULL);

	VERTEX_2D* pVtx;
	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPauseBG->Lock(0, 0, (void**)&pVtx, 0);
	
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//rhw�̐ݒ�i�����ݒ�j
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�i�����ݒ�j
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//�e�N�X�`�����W�̐ݒ�i�����ݒ�j
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPauseBG->Unlock();

	//================================
	//�|�[�Y�R�}���h����������
	//================================

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\continue_001.png",
		&g_pTexturePause[0]);


	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\retry_001.png",
		&g_pTexturePause[1]);


	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\quit_001.png",
		&g_pTexturePause[2]);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_PAUSETEX,//���_�̐�
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause,
		NULL);

	for (int nCntPause = 0; nCntPause < NUM_PAUSETEX; nCntPause++)
	{
		
		g_aPause[nCntPause].pos = D3DXVECTOR3(1060.0f, 350.0f + ((COMMAND_SIZE_Y * 2.0f) + 50.0f) * nCntPause, 0.0f);//�^�񒆂ɎO�i�I�v�V�����R�}���h��p�ӂ���B
	}
	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);



	for (int nCntPause = 0; nCntPause < NUM_PAUSETEX; nCntPause++)
	{
		//���_���W�̐ݒ�i�����ʒu�j
		g_aPause[nCntPause].nPauseMenu = nCntPause;//4�̃|���S���Ƀe�N�X�`�������蓖�Ă�B
		g_aPause[nCntPause].bChooseMenu = false;
		//�R�}���h�̈ʒu���̏�����
			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			//rhw�̐ݒ�i�����ݒ�j
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			//���_�J���[�̐ݒ�i�����ݒ�j
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			//�e�N�X�`�����W�̐ݒ�i�����ݒ�j
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		

		pVtx += 4;//���_���W�̃|�C���^���C���N�������g
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPause->Unlock();

	//==========================================================================================================
}

//==============================
//���U���g��ʂ̏I������
//==============================
void UninitPause(void)
{
	//�|�[�Y�w�i�̃e�N�X�`���̔j��
	if (g_pTexturePauseBG != NULL)
	{
		g_pTexturePauseBG->Release();
		g_pTexturePauseBG = NULL;
	}

	//�|�[�Y�w�i�̒��_�o�b�t�@�̔j��
	if (g_pVtxBuffPauseBG != NULL)
	{
		g_pVtxBuffPauseBG->Release();
		g_pVtxBuffPauseBG = NULL;
	}


	//�|�[�Y�R�}���h�̃e�N�X�`���̔j��
	for (int nCntPause = 0; nCntPause < NUM_PAUSETEX; nCntPause++)
	{
		if (g_pTexturePause[nCntPause] != NULL)
		{
			g_pTexturePause[nCntPause]->Release();
			g_pTexturePause[nCntPause] = NULL;
		}
	}
	//�|�[�Y�R�}���h�̒��_�o�b�t�@�̔j��
	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}
}

//===================================
//���U���g��ʂ̍X�V����
//===================================
void UpdatePause(void)
{
	int nCntPause;
	VERTEX_2D* pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntPause = 0; nCntPause < NUM_PAUSETEX; nCntPause++)
	{
		//���_���W�̐ݒ�i�����ʒu�j
		pVtx[0].pos = D3DXVECTOR3(g_aPause[nCntPause].pos.x- COMMAND_SIZE_X,g_aPause[nCntPause].pos.y-COMMAND_SIZE_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aPause[nCntPause].pos.x+ COMMAND_SIZE_X,g_aPause[nCntPause].pos.y-COMMAND_SIZE_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aPause[nCntPause].pos.x- COMMAND_SIZE_X,g_aPause[nCntPause].pos.y+COMMAND_SIZE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aPause[nCntPause].pos.x+ COMMAND_SIZE_X,g_aPause[nCntPause].pos.y+COMMAND_SIZE_Y, 0.0f);

		//���_�J���[�̐ݒ�i�����ݒ�j
		pVtx[0].col = D3DCOLOR_RGBA(255,255,255,255);
		pVtx[1].col = D3DCOLOR_RGBA(255,255,255,255);
		pVtx[2].col = D3DCOLOR_RGBA(255,255,255,255);
		pVtx[3].col = D3DCOLOR_RGBA(255,255,255,255);

		if (g_aPause[nCntPause].bChooseMenu == true)
		{//	�I�΂�Ă���R�}���h�������邭����
			pVtx[0].col = D3DCOLOR_RGBA(0, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(0, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(0, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(0, 255, 255, 255);
		}
		pVtx += 4;//���_���W�̃|�C���^���C���N�������g
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPause->Unlock();
}

//================================
//�^�C�g����ʂ̕`�揈��
//================================
void DrawPause(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPauseBG, 0, sizeof(VERTEX_2D));//���_�o�b�t�@�ւ̃|�C���^�ƒ��_���̍\���̂̃T�C�Y

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetTexture(0, g_pTexturePauseBG);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,0, 2);//0�́A�`�悷��ŏ��̒��_�C���f�b�N�X�ŁA2�́A�`�悷��v���~�e�B�u��

	//=======================================================================================================================

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));//���_�o�b�t�@�ւ̃|�C���^�ƒ��_���̍\���̂̃T�C�Y

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);


	for (int nCntPause = 0; nCntPause < NUM_PAUSETEX; nCntPause++)
	{
		//�e�N�X�`���̐ݒ�
		if (g_aPause[nCntPause].nPauseMenu == nCntPause)
		{
			pDevice->SetTexture(0, g_pTexturePause[nCntPause]);
		}
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPause * 4, 2);//0�́A�`�悷��ŏ��̒��_�C���f�b�N�X�ŁA2�́A�`�悷��v���~�e�B�u��
	}
}

//================================
//���U���g��ʕ����\���̂̎擾
//================================

void SetEnablePause(int nType)
{
	int nCntPause;
	for (nCntPause = 0; nCntPause < NUM_PAUSETEX; nCntPause++)
	{//�Z�b�g����邽�тɃ|�[�Y�̖��邭�\������t���O��false�ɂ���B
		g_aPause[nCntPause].bChooseMenu = false;
	}
	//�Z�b�g���ꂽ�Ƃ��ɁA�������ŌĂяo�����|�[�Y���j���[�����邭�\�������B
	g_aPause[nType].bChooseMenu = true;
}