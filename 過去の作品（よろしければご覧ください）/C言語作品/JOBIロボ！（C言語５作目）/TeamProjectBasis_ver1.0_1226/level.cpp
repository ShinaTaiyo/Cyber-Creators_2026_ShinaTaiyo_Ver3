//=====================================================================================================================
//
//�P�Q���Q�U���F�`�[������̊�ՂÂ���[Level.cpp]
//Author:ShinaTaiyo
//
// 
// ���ӁFfRWidth,fRHeight�́A���S�_����̔��a�ő傫���A����Ȃǂ�����Ă��܂��B�������̏������g���Ȃ�A�����Ӊ������B
// 
//======================================================================================================================

#include "level.h"
#include "main.h"
#include "sound.h"
#include "player.h"
#include "battery.h"

#define LEVEL_WIDTH (60.0f)
#define LEVEL_HEIGHT (30.0f)

#define LEVELNUM_WIDTH (20.0f)
#define LEVELNUM_HEIGHT (40.0f)
#define LEVELDIGIT (2)//�X�N���b�v�̐��\���̌���

#define LEVELUPDISPLAY_WIDTH (25.0f)
#define LEVELUPDISPLAY_HEIGHT (10.0f)

//=================================
//���x���t���[���\���p
//=================================
LPDIRECT3DTEXTURE9 g_pTextureLevel[NUM_LEVEL] = {};//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLevel = NULL;//���x���̏��
Level g_aLevel[NUM_LEVEL];//���x���\���p

//=================================
//���x�����l�\���p
//=================================
LPDIRECT3DTEXTURE9 g_pTextureLevelNum = NULL;//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLevelNum = NULL;//���x�����l�̏��
LevelNum g_aLevelNum[NUM_LEVEL][LEVEL_DIGIT];//���x�����l�̍\����

//=================================
//���x���A�b�v�\���p
//==================================
LPDIRECT3DTEXTURE9 g_pTextureLevelUpDisplay = NULL;//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLevelUpDisplay = NULL;//���_���
LevelUpDisplay g_aLevelUpDisplay[MAX_LEVELUPDISPLAY];//���x���A�b�v�\���̍\����
int g_nUseLevelUpDisplay;//���x���A�b�v�f�B�X�v���C���g�p���Ă��鐔���J�E���g����

LPDIRECT3DTEXTURE9 g_pTextureLevelUpDisplay2D = NULL;//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLevelUpDisplay2D = NULL;//���_���
LevelUpDisplay g_aLevelUpDisplay2D[MAX_LEVELUPDISPLAY];//���x���A�b�v�\���̍\����
int g_nUseLevelUpDisplay2D[NUM_LEVEL];//���x����\������X�e�[�^�X�̐����A�g�p����Ă��郌�x���A�b�v����\������


//============================
// ����������
//============================
void InitLevel(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

 	pDevice = GetDevice();

	VERTEX_2D* pVtx;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\LvDisplay_000_1.png",
		&g_pTextureLevel[0]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\LvDisplay_001.png",
		&g_pTextureLevel[1]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\LvDisplay_002.png",
		&g_pTextureLevel[2]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\LvDisplay_003.png",
		&g_pTextureLevel[3]);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_LEVEL,//�S�͕K�v�Ȓ��_��
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffLevel,
		NULL);

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffLevel->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntLevel = 0; nCntLevel < NUM_LEVEL; nCntLevel++)
	{
		g_aLevel[nCntLevel].nType = nCntLevel;
		g_aLevel[nCntLevel].pos = D3DXVECTOR3(80.0f + 250.0f * nCntLevel, 620.0f, 0.0f);
		g_aLevel[nCntLevel].nLevel = 0;


		pVtx[0].pos = D3DXVECTOR3(g_aLevel[nCntLevel].pos.x - LEVEL_WIDTH, g_aLevel[nCntLevel].pos.y - LEVEL_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aLevel[nCntLevel].pos.x + LEVEL_WIDTH, g_aLevel[nCntLevel].pos.y - LEVEL_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aLevel[nCntLevel].pos.x - LEVEL_WIDTH, g_aLevel[nCntLevel].pos.y + LEVEL_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aLevel[nCntLevel].pos.x + LEVEL_WIDTH, g_aLevel[nCntLevel].pos.y + LEVEL_HEIGHT, 0.0f);

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
    g_pVtxBuffLevel->Unlock();

	//==========================
	//���x�����l�̏���
	//==========================

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Lv_Num.png",
		&g_pTextureLevelNum);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_LEVEL * LEVELDIGIT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffLevelNum,
		NULL);

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffLevelNum->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntLevel = 0; nCntLevel < NUM_LEVEL; nCntLevel++)
	{
		for (int nCntDigit = 0; nCntDigit < LEVELDIGIT; nCntDigit++)
		{
			g_aLevelNum[nCntLevel][nCntDigit].pos = D3DXVECTOR3(200.0f + 250.0f * nCntLevel + 40.0f * -nCntDigit, 620.0f, 0.0f);

			pVtx[0].pos = D3DXVECTOR3(g_aLevelNum[nCntLevel][nCntDigit].pos.x - LEVELNUM_WIDTH, g_aLevelNum[nCntLevel][nCntDigit].pos.y - LEVELNUM_HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aLevelNum[nCntLevel][nCntDigit].pos.x + LEVELNUM_WIDTH, g_aLevelNum[nCntLevel][nCntDigit].pos.y - LEVELNUM_HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aLevelNum[nCntLevel][nCntDigit].pos.x - LEVELNUM_WIDTH, g_aLevelNum[nCntLevel][nCntDigit].pos.y + LEVELNUM_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aLevelNum[nCntLevel][nCntDigit].pos.x + LEVELNUM_WIDTH, g_aLevelNum[nCntLevel][nCntDigit].pos.y + LEVELNUM_HEIGHT, 0.0f);
															
			//rhw�̐ݒ�i�����ݒ�j
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			switch (nCntLevel)
			{
			case 0:
				//���_�J���[�̐ݒ�i�����ݒ�j
				pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				break;
			case 1:
				//���_�J���[�̐ݒ�i�����ݒ�j
				pVtx[0].col = D3DCOLOR_RGBA(0, 100, 255, 255);
				pVtx[1].col = D3DCOLOR_RGBA(0, 100, 255, 255);
				pVtx[2].col = D3DCOLOR_RGBA(0, 100, 255, 255);
				pVtx[3].col = D3DCOLOR_RGBA(0, 100, 255, 255);
				break;
			case 2:
				//���_�J���[�̐ݒ�i�����ݒ�j
				pVtx[0].col = D3DCOLOR_RGBA(255, 255, 0, 255);
				pVtx[1].col = D3DCOLOR_RGBA(255, 255, 0, 255);
				pVtx[2].col = D3DCOLOR_RGBA(255, 255, 0, 255);
				pVtx[3].col = D3DCOLOR_RGBA(255, 255, 0, 255);
				break;
			case 3:
				//���_�J���[�̐ݒ�i�����ݒ�j
				pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);
				pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
				pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
				pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);
				break;
			}
			//�e�N�X�`�����W�̐ݒ�i�����ݒ�j
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

			pVtx += 4;//���_���W�̃|�C���^���C���N�������g
		}
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffLevelNum->Unlock();
}

//==============================
// �I������
//==============================
void UninitLevel(void)
{
	//============================
	//���x���t���[���̏���
	//============================
	
	//�e�N�X�`���̔j��
	for (int nCntLevel = 0; nCntLevel < NUM_LEVEL; nCntLevel++)
	{
		if (g_pTextureLevel[nCntLevel] != NULL)
		{
			g_pTextureLevel[nCntLevel]->Release();
			g_pTextureLevel[nCntLevel] = NULL;
		}
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffLevel != NULL)
	{
		g_pVtxBuffLevel->Release();
		g_pVtxBuffLevel = NULL;
	}
  
	//==============================
	//���x�����l�̏���
	//==============================

	//�e�N�X�`���̔j��
	if (g_pTextureLevelNum != NULL)
	{
		g_pTextureLevelNum->Release();
		g_pTextureLevelNum = NULL;
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffLevel != NULL)
	{
		g_pVtxBuffLevel->Release();
		g_pVtxBuffLevel = NULL;
	}
}

//============================
// �X�V����
//============================
void UpdateLevel(void)
{


	
}

//================================
// �`�揈��
//================================
void DrawLevel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̎擾

	//===========================
	//���x���t���[���̏���
	//===========================

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffLevel, 0, sizeof(VERTEX_2D));//���_�o�b�t�@�ւ̃|�C���^�ƒ��_���̍\���̂̃T�C�Y

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntLevel = 0; nCntLevel < NUM_LEVEL; nCntLevel++)
	{
		pDevice->SetTexture(0, g_pTextureLevel[g_aLevel[nCntLevel].nType]);
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntLevel * 4, 2);//0�́A�`�悷��ŏ��̒��_�C���f�b�N�X�ŁA2�́A�`�悷��v���~�e�B�u��
	}

	//=============================
	//���x�����l�̏���
	//=============================

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffLevelNum, 0, sizeof(VERTEX_2D));//���_�o�b�t�@�ւ̃|�C���^�ƒ��_���̍\���̂̃T�C�Y

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntLevelNum = 0; nCntLevelNum < NUM_LEVEL * LEVELDIGIT; nCntLevelNum++)
	{
		pDevice->SetTexture(0, g_pTextureLevelNum);
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntLevelNum * 4, 2);
	}

}

//===================================
//���x���A�b�v�\���̏�����
//===================================
void InitLevelUpDisplay(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

	pDevice = GetDevice();

	VERTEX_3D* pVtx;

	g_nUseLevelUpDisplay = 0;//�g�p���Ă��郌�x���A�b�v�\���̐����J�E���g����

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\LvUp000.png",
		&g_pTextureLevelUpDisplay);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_LEVELUPDISPLAY,//�S�͕K�v�Ȓ��_��
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffLevelUpDisplay,
		NULL);

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffLevelUpDisplay->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntLevel = 0; nCntLevel < MAX_LEVELUPDISPLAY; nCntLevel++)
	{
		g_aLevelUpDisplay[nCntLevel].bUse = false;
		g_aLevelUpDisplay[nCntLevel].col = {};
		g_aLevelUpDisplay[nCntLevel].pos = NULL_VECTOR3;
		g_aLevelUpDisplay[nCntLevel].nLife = 0;

		pVtx[0].pos = NULL_VECTOR3;
		pVtx[1].pos = NULL_VECTOR3;
		pVtx[2].pos = NULL_VECTOR3;
		pVtx[3].pos = NULL_VECTOR3;

		//nor�̐ݒ�i�����ݒ�j
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

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
	g_pVtxBuffLevelUpDisplay->Unlock();
}

//===================================
//���x���A�b�v�\���̏I������
//===================================
void UninitLevelUpDisplay(void)
{

	if (g_pTextureLevelUpDisplay != NULL)
	{
		g_pTextureLevelUpDisplay->Release();
		g_pTextureLevelUpDisplay = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffLevelUpDisplay != NULL)
	{
		g_pVtxBuffLevelUpDisplay->Release();
		g_pVtxBuffLevelUpDisplay = NULL;
	}

}

//===================================
//���x���A�b�v�\���̍X�V����
//===================================
void UpdateLevelUpDisplay(void)
{
	VERTEX_3D* pVtx;

	Player* pPlayer = GetPlayer();

	int nCntUseDisplay = 0;//���x���A�b�v�\�����g�p���Ă��鐔

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffLevelUpDisplay->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntLevelUpDisplay = 0; nCntLevelUpDisplay < MAX_LEVELUPDISPLAY; nCntLevelUpDisplay++)
	{
		if (g_aLevelUpDisplay[nCntLevelUpDisplay].bUse == true)
		{
			nCntUseDisplay++;

			g_aLevelUpDisplay[nCntLevelUpDisplay].nLife -= 1;

			g_aLevelUpDisplay[nCntLevelUpDisplay].pos = D3DXVECTOR3(pPlayer->pos.x, pPlayer->pos.y + (pPlayer->Size.y * 2.0f) + ((LEVELUPDISPLAY_HEIGHT * 2.0f) * (float)(nCntUseDisplay)), pPlayer->pos.z);
		
			if (g_aLevelUpDisplay[nCntLevelUpDisplay].nLife <= 0)
			{
				g_nUseLevelUpDisplay--;//���x���A�b�v�\�����g�p���Ă��鐔���J�E���g����

				g_aLevelUpDisplay[nCntLevelUpDisplay].bUse = false;
			}
		}
	}

	g_pVtxBuffLevelUpDisplay->Unlock();

}

//===================================
//���x���A�b�v�\���̕`�揈��
//===================================
void DrawLevelUpDisplay(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	for (int nCntLevelUpDisplay = 0; nCntLevelUpDisplay < MAX_LEVELUPDISPLAY; nCntLevelUpDisplay++)
	{
		if (g_aLevelUpDisplay[nCntLevelUpDisplay].bUse == true)
		{
			D3DXMATRIX mtxTrans;//�v�Z�p�}�g���b�N�X

			D3DXMATRIX mtxView;//�r���[�}�g���b�N�X�擾�p

			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aLevelUpDisplay[nCntLevelUpDisplay].mtxWorld);

			//�r���[�}�g���b�N�X���擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//�|���S�����J�����ɑ΂��Đ��ʂɌ�����
			D3DXMatrixInverse(&g_aLevelUpDisplay[nCntLevelUpDisplay].mtxWorld, NULL, &mtxView);

			//�t�s������߂�
			g_aLevelUpDisplay[nCntLevelUpDisplay].mtxWorld._41 = 0.0f;
			g_aLevelUpDisplay[nCntLevelUpDisplay].mtxWorld._42 = 0.0f;
			g_aLevelUpDisplay[nCntLevelUpDisplay].mtxWorld._43 = 0.0f;

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aLevelUpDisplay[nCntLevelUpDisplay].pos.x, g_aLevelUpDisplay[nCntLevelUpDisplay].pos.y, g_aLevelUpDisplay[nCntLevelUpDisplay].pos.z);
			D3DXMatrixMultiply(&g_aLevelUpDisplay[nCntLevelUpDisplay].mtxWorld, &g_aLevelUpDisplay[nCntLevelUpDisplay].mtxWorld, &mtxTrans);

			//===============================================================
			//�`��̒���
			//==============================================================
			//���C�g�𖳌��ɂ���
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			//Z�̔�r���@�ύX�B
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

			//Z�o�b�t�@�ɏ������܂Ȃ�
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

			//�A���t�@�e�X�g��L��
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aLevelUpDisplay[nCntLevelUpDisplay].mtxWorld);


			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffLevelUpDisplay, 0, sizeof(VERTEX_3D));//���_�o�b�t�@�ւ̃|�C���^�ƒ��_���̍\���̂̃T�C�Y

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureLevelUpDisplay);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntLevelUpDisplay * 4, 2);//0�́A�`�悷��ŏ��̒��_�C���f�b�N�X�ŁA2�́A�`�悷��v���~�e�B�u

			//============================
			//�`��̒��������Ƃɖ߂�
			//============================

			//���C�g��L���ɖ߂�
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

			//Z�̔�r���@�ύX�B
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

			//Z�o�b�t�@�ɏ�������
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

			//�A���t�@�e�X�g�𖳌��ɖ߂�
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

		}
	}

}

//=======================================================
//���x���\���̂̏����擾
//=======================================================
Level* GetLevel(void)//�X�N���b�v�\���̂̏����Q�b�g����
{
	return &g_aLevel[0];
}

//===========================
//���x�����グ��
//===========================
void AddLevel(int nType)//���x�����グ��B
{
	Battery* pBattery = GetBattery();

	PlaySound(SOUND_LABEL_SE_STATUSBUFF_000);

	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	VERTEX_2D* pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffLevelNum->Lock(0, 0, (void**)&pVtx, 0);
	g_aLevel[nType].nLevel += 1;//���x����1�グ��

	switch (nType)
	{
	case 0:
		SetLevelUpDisplay(D3DXVECTOR3(g_aLevel[nType].pos.x + LEVEL_WIDTH, g_aLevel[nType].pos.y - LEVEL_HEIGHT - LEVELUPDISPLAY_HEIGHT, g_aLevel[nType].pos.z),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 60);

		SetLevelUpDisplay2D(D3DXVECTOR3(g_aLevel[nType].pos.x + LEVEL_WIDTH, g_aLevel[nType].pos.y - LEVEL_HEIGHT - LEVELUPDISPLAY_HEIGHT * 4.0f, g_aLevel[nType].pos.z),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 60,nType);
		break;

	case 1:
		SetLevelUpDisplay(D3DXVECTOR3(g_aLevel[nType].pos.x + LEVEL_WIDTH, g_aLevel[nType].pos.y - LEVEL_HEIGHT - LEVELUPDISPLAY_HEIGHT, g_aLevel[nType].pos.z),
			D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), 60);
		SetLevelUpDisplay2D(D3DXVECTOR3(g_aLevel[nType].pos.x + LEVEL_WIDTH, g_aLevel[nType].pos.y - LEVEL_HEIGHT - LEVELUPDISPLAY_HEIGHT * 4.0f, g_aLevel[nType].pos.z),
			D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), 60, nType);
		break;

	case 2:
		SetLevelUpDisplay(D3DXVECTOR3(g_aLevel[nType].pos.x + LEVEL_WIDTH, g_aLevel[nType].pos.y - LEVEL_HEIGHT - LEVELUPDISPLAY_HEIGHT, g_aLevel[nType].pos.z),
			D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 60);

		SetLevelUpDisplay2D(D3DXVECTOR3(g_aLevel[nType].pos.x + LEVEL_WIDTH, g_aLevel[nType].pos.y - LEVEL_HEIGHT - LEVELUPDISPLAY_HEIGHT * 4.0f, g_aLevel[nType].pos.z),
			D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 60, nType);
		break;

	case 3:
		SetLevelUpDisplay(D3DXVECTOR3(g_aLevel[nType].pos.x + LEVEL_WIDTH, g_aLevel[nType].pos.y - LEVEL_HEIGHT - LEVELUPDISPLAY_HEIGHT, g_aLevel[nType].pos.z),
			D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 60);

		SetLevelUpDisplay2D(D3DXVECTOR3(g_aLevel[nType].pos.x + LEVEL_WIDTH, g_aLevel[nType].pos.y - LEVEL_HEIGHT - LEVELUPDISPLAY_HEIGHT * 4.0f, g_aLevel[nType].pos.z),
			D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 60, nType);
		break;

	default:
		break;
	}

	int nGetLevel = g_aLevel[nType].nLevel;//�X�R�A��ۑ�����
	int nMatchType = 0;//�Ă΂ꂽ��ނƈ�v�������ǂ���
	while (1)
	{
		for (int nCntDigit = 0; nCntDigit < LEVELDIGIT; nCntDigit++)
		{
			if (nMatchType == nType)
			{
				

				g_aLevelNum[nType][nCntDigit].fPosTexU = (float)(nGetLevel % 10);
				nGetLevel /= 10;

				pVtx[0].tex = D3DXVECTOR2(0.0f + (0.1f * g_aLevelNum[nType][nCntDigit].fPosTexU), 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f * g_aLevelNum[nType][nCntDigit].fPosTexU), 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f + (0.1f * g_aLevelNum[nType][nCntDigit].fPosTexU), 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * g_aLevelNum[nType][nCntDigit].fPosTexU), 1.0f);
			}
			pVtx += 4;//���_�f�[�^�̃|�C���^���S���i�߂�B
		}

		if (nMatchType == nType)
		{
			if (nType == 0)
			{
				pBattery->nBattery += 100;
			}
			break;
		}
		nMatchType++;
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffLevelNum->Unlock();
}

//==================================
//���x���A�b�v�\���̐ݒ�
//==================================
void SetLevelUpDisplay(D3DXVECTOR3 pos, D3DXCOLOR col, int nLife)
{
	VERTEX_3D* pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffLevelUpDisplay->Lock(0, 0, (void**)&pVtx, 0);

	g_nUseLevelUpDisplay++;//���x���A�b�v�\�����g�p���Ă��鐔���J�E���g����

	for (int nCntLevelUpDisplay = 0; nCntLevelUpDisplay < MAX_LEVELUPDISPLAY; nCntLevelUpDisplay++)
	{
		if (g_aLevelUpDisplay[nCntLevelUpDisplay].bUse == false)
		{
			g_aLevelUpDisplay[nCntLevelUpDisplay].bUse = true;
			g_aLevelUpDisplay[nCntLevelUpDisplay].col = col;
			g_aLevelUpDisplay[nCntLevelUpDisplay].nLife = nLife;
			g_aLevelUpDisplay[nCntLevelUpDisplay].pos = pos;

			//���_���W�̐ݒ�i�����ʒu�j
			pVtx[0].pos = D3DXVECTOR3(-LEVELUPDISPLAY_WIDTH, LEVELUPDISPLAY_HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(LEVELUPDISPLAY_WIDTH, LEVELUPDISPLAY_HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-LEVELUPDISPLAY_WIDTH, -LEVELUPDISPLAY_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(LEVELUPDISPLAY_WIDTH, -LEVELUPDISPLAY_HEIGHT, 0.0f);

			//���_�J���[�̐ݒ�i�����ݒ�j
			pVtx[0].col = col;
			pVtx[1].col = col;
			pVtx[2].col = col;
			pVtx[3].col = col;
			break;
		}
		pVtx += 4;
	}

	g_pVtxBuffLevelUpDisplay->Unlock();
}

//=================================================================================================

//===========================================
//2D���x���A�b�v�\���̏���������
//===========================================
void InitLevelUpDisplay2D(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();

	VERTEX_2D* pVtx;

	for (int nCntLevel = 0; nCntLevel < NUM_LEVEL; nCntLevel++)
	{
		g_nUseLevelUpDisplay2D[nCntLevel] = 0;
	}
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\LvUp000.png",
		&g_pTextureLevelUpDisplay2D);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_LEVELUPDISPLAY,//�S�͕K�v�Ȓ��_��
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffLevelUpDisplay2D,
		NULL);

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffLevelUpDisplay2D->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntLevel = 0; nCntLevel < MAX_LEVELUPDISPLAY; nCntLevel++)
	{
		g_aLevelUpDisplay2D[nCntLevel].bUse = false;//�g�p���
		g_aLevelUpDisplay2D[nCntLevel].col = {};//�F
		g_aLevelUpDisplay2D[nCntLevel].pos = NULL_VECTOR3;//�ʒu
		g_aLevelUpDisplay2D[nCntLevel].nLife = 0;//�̗�

		pVtx[0].pos = NULL_VECTOR3;
		pVtx[1].pos = NULL_VECTOR3;
		pVtx[2].pos = NULL_VECTOR3;
		pVtx[3].pos = NULL_VECTOR3;

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
	g_pVtxBuffLevelUpDisplay2D->Unlock();
}

void UninitLevelUpDisplay2D(void)
{
	if (g_pTextureLevelUpDisplay2D != NULL)
	{
		g_pTextureLevelUpDisplay2D->Release();
		g_pTextureLevelUpDisplay2D = NULL;
	}

	if (g_pVtxBuffLevelUpDisplay2D != NULL)
	{
		g_pVtxBuffLevelUpDisplay2D->Release();
		g_pVtxBuffLevelUpDisplay2D = NULL;
	}
}

void UpdateLevelUpDisplay2D(void)
{
	for (int nCntLevel = 0; nCntLevel < MAX_LEVELUPDISPLAY; nCntLevel++)
	{
		if (g_aLevelUpDisplay2D[nCntLevel].bUse == true)
		{
			g_aLevelUpDisplay2D[nCntLevel].nLife--;

			if (g_aLevelUpDisplay2D[nCntLevel].nLife <= 0)
			{
				g_aLevelUpDisplay2D[nCntLevel].bUse = false;
			}
		}
	}
}

void DrawLevelUpDisplay2D(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̎擾
	pDevice->SetStreamSource(0, g_pVtxBuffLevelUpDisplay2D, 0, sizeof(VERTEX_2D));//���_�o�b�t�@�ւ̃|�C���^�ƒ��_���̍\���̂̃T�C�Y
	pDevice->SetFVF(FVF_VERTEX_2D);
	

	for (int nCntLevel = 0; nCntLevel < MAX_LEVELUPDISPLAY; nCntLevel++)
	{
		if (g_aLevelUpDisplay2D[nCntLevel].bUse == true)
		{
			pDevice->SetTexture(0, g_pTextureLevelUpDisplay2D);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntLevel * 4, 2);//0�́A�`�悷��ŏ��̒��_�C���f�b�N�X�ŁA2�́A�`�悷��v���~�e�B�u��
		}
	}
}

void SetLevelUpDisplay2D(D3DXVECTOR3 pos, D3DXCOLOR col, int nLife, int nType)
{
	VERTEX_2D* pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffLevelUpDisplay2D->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntLevelUpDisplay2D = 0; nCntLevelUpDisplay2D < MAX_LEVELUPDISPLAY; nCntLevelUpDisplay2D++)
	{
		if (g_aLevelUpDisplay[nCntLevelUpDisplay2D].bUse == false)
		{
			g_aLevelUpDisplay2D[nCntLevelUpDisplay2D].bUse = true;
			g_aLevelUpDisplay2D[nCntLevelUpDisplay2D].col = col;
			g_aLevelUpDisplay2D[nCntLevelUpDisplay2D].nLife = nLife;
			g_aLevelUpDisplay2D[nCntLevelUpDisplay2D].pos = pos;
			g_aLevelUpDisplay2D[nCntLevelUpDisplay2D].nType = nType;

			//���_���W�̐ݒ�i�����ʒu�j
			pVtx[0].pos = D3DXVECTOR3(g_aLevelUpDisplay2D[nCntLevelUpDisplay2D].pos.x - LEVELUPDISPLAY_WIDTH * 3.0f, g_aLevelUpDisplay2D[nCntLevelUpDisplay2D].pos.y - LEVELUPDISPLAY_HEIGHT * 3.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aLevelUpDisplay2D[nCntLevelUpDisplay2D].pos.x + LEVELUPDISPLAY_WIDTH * 3.0f, g_aLevelUpDisplay2D[nCntLevelUpDisplay2D].pos.y - LEVELUPDISPLAY_HEIGHT * 3.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aLevelUpDisplay2D[nCntLevelUpDisplay2D].pos.x - LEVELUPDISPLAY_WIDTH * 3.0f, g_aLevelUpDisplay2D[nCntLevelUpDisplay2D].pos.y + LEVELUPDISPLAY_HEIGHT * 3.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aLevelUpDisplay2D[nCntLevelUpDisplay2D].pos.x + LEVELUPDISPLAY_WIDTH * 3.0f, g_aLevelUpDisplay2D[nCntLevelUpDisplay2D].pos.y + LEVELUPDISPLAY_HEIGHT * 3.0f, 0.0f);

			//���_�J���[�̐ݒ�i�����ݒ�j
			pVtx[0].col = col;
			pVtx[1].col = col;
			pVtx[2].col = col;
			pVtx[3].col = col;
			break;
		}
		pVtx += 4;
	}

	g_pVtxBuffLevelUpDisplay->Unlock();
}


