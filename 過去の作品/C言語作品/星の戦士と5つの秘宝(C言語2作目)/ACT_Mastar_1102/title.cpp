//========================================================
//
//�X���Q�U���F�n�b�J�\���̊�Ս��[title.cpp]
//Author:ShinaTaiyo
//
//=========================================================

#include "title.h"
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
//�O���[�o���ϐ�

#define NUM_DRAWTITLE (3)

LPDIRECT3DTEXTURE9 g_pTextureTitle[NUM_DRAWTITLE] = {};//�e�N�X�`���ւ̃|�C���^
//VERTEX_2D g_aVertex[4];//���_�����i�[�i�l�p�`����肽���j
PDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;//���_�o�b�t�@�ւ̃|�C���^

int g_nColorChange;

bool bFadeFlag;//�^�C�g���t�F�[�h�̃t���O

int g_nCntTime2;

int g_nChooseCommand;//�R�}���h�I��p

//�^�C�g����ʂ̏���������
void InitTitle(void)
{
	g_nChooseCommand = 0;//�R�}���h�I��p

	bFadeFlag = false;


	g_nCntTime2 = 0;

	g_nColorChange = 255;//�J���[�̏������i���F�j

	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\ACT_Title.png",
		&g_pTextureTitle[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\ChooseNormalMode.png",
		&g_pTextureTitle[1]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\ChooseShortMode.png",
		&g_pTextureTitle[2]);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 2,//���_�̐�
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitle,
		NULL);

	//�^�C�g�����BGM�̐ݒ�
	PlaySound(SOUND_LABEL_BGM000);

	VERTEX_2D* pVtx;
	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		//���_���W�̐ݒ�i�����ʒu�j
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

		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTitle->Unlock();
}

//==============================
//�^�C�g����ʂ̏I������
//==============================
void UninitTitle(void)
{
	//�e�N�X�`���̔j��
	for (int nCnt = 0; nCnt < NUM_DRAWTITLE; nCnt++)
	{
		if (g_pTextureTitle[nCnt] != NULL)
		{
			g_pTextureTitle[nCnt]->Release();
			g_pTextureTitle[nCnt] = NULL;
		}
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}
}
//============================
//�^�C�g����ʂ̍X�V����
//============================
void UpdateTitle(void)
{
	g_nCntTime2++;

	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	VERTEX_2D* pVtx;
	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);


	/*if (g_nCntTime2 == 900)
	{
		SetFade(MODE_RANKING);
	}*/

	if (GetKeyboardTrigger(DIK_W) == true || GetJoyPadTrigger(JOYKEY_UP)==true)
	{
		g_nChooseCommand--;
		PlaySound(SOUND_LABEL_SE_SELECT_000);
	}
	if (GetKeyboardTrigger(DIK_S) == true || GetJoyPadTrigger(JOYKEY_DOWN) == true)
	{
		g_nChooseCommand++;
		PlaySound(SOUND_LABEL_SE_SELECT_000);
	}

	if (g_nChooseCommand <= -1)
	{
		g_nChooseCommand = 1;
	}
	if (g_nChooseCommand >= 2)
	{
		g_nChooseCommand = 0;
	}


	//==========================================
	//�^�C�g���Ƃ��������Ȃ珈��������
	//==========================================
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		switch (nCnt)
		{
		case 0:
			//���_���W�̐ݒ�i�����ʒu�j
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
			break;
		case 1:
			//���_���W�̐ݒ�i�����ʒu�j
			pVtx[0].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 -150.0f,SCREEN_HEIGHT - 300.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 + 150.0f, SCREEN_HEIGHT - 300.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 - 150.0f, SCREEN_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 + 150.0f, SCREEN_HEIGHT, 0.0f);

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
			break;
		}
		

		pVtx += 4;
	}

    //=====================================================================

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTitle->Unlock();

	//========================================
	//��ʑJ�ڏ���
	//========================================
	if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoyPadTrigger(JOYKEY_START) == true)
	{//����L�[�iENTER�L�[�j�������ꂽ
		//bFadeFlag = true;//�t�F�[�h������t���O��true�ɂ���B
		SetFade(MODE_TUTORIAL);//�`���[�g���A����ʂ��Ăяo���B
		PlaySound(SOUND_LABEL_SE_DECISION_000);
	}
	//===========================================================================================
}



//================================
//�^�C�g����ʂ̕`�揈��
//================================
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

		//�f�o�C�X�̎擾
		pDevice = GetDevice();

		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffTitle, 0, sizeof(VERTEX_2D));//���_�o�b�t�@�ւ̃|�C���^�ƒ��_���̍\���̂̃T�C�Y

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		//�e�N�X�`���̐ݒ�
		for (int nCnt = 0; nCnt < 2; nCnt++)
		{
			switch (nCnt)
			{
			case 0:
			pDevice->SetTexture(0, g_pTextureTitle[0]);
				break;
			case 1:
				if (g_nChooseCommand == 0)
				{
					pDevice->SetTexture(0, g_pTextureTitle[1]);
				}
				else if (g_nChooseCommand == 1)
				{
					pDevice->SetTexture(0, g_pTextureTitle[2]);
				}
				break;
			}
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,nCnt *4, 2);//0�́A�`�悷��ŏ��̒��_�C���f�b�N�X�ŁA2�́A�`�悷��v���~�e�B�u
		}
		//�|���S���̕`��
}

int* GetChooseCommand(void)
{
	return &g_nChooseCommand;
}
