//========================================================
//
//�W���R�O���F�G�t�F�N�g������񌩒���[title.cpp]
//Author:ShinaTaiyo
//
//=========================================================

#include "title.h"
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "staging.h"
//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureTitle= NULL;//�e�N�X�`���ւ̃|�C���^
//VERTEX_2D g_aVertex[4];//���_�����i�[�i�l�p�`����肽���j
PDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;//���_�o�b�t�@�ւ̃|�C���^

int g_nColorChange;

bool bFadeFlag;//�^�C�g���t�F�[�h�̃t���O

int g_nCntTime;

//�^�C�g����ʂ̏���������
void InitTitle(void)
{


	bFadeFlag = false;

	g_nCntTime = 0;

	g_nColorChange = 255;//�J���[�̏������i���F�j

	//�uPressStart�v�̕\������
	


	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\gametitle.png",
		&g_pTextureTitle);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,//���_�̐�
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
	pVtx[0].col = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[1].col = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[2].col = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[3].col = D3DCOLOR_RGBA(255,255,255,255);


	//�e�N�X�`�����W�̐ݒ�i�����ݒ�j
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTitle->Unlock();
}

//==============================
//�^�C�g����ʂ̏I������
//==============================
void UninitTitle(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureTitle != NULL)
	{
		g_pTextureTitle->Release();
		g_pTextureTitle = NULL;
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
	g_nCntTime++;

	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	VERTEX_2D* pVtx;
	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	if (GetKeyboardTrigger(DIK_RETURN) == true||GetJoyPadTrigger(JOYKEY_START))
	{//����L�[�iENTER�L�[�j�������ꂽ
	
		PlaySound(SOUND_LABEL_SE_DECISION_000);
		//bFadeFlag = true;//�t�F�[�h������t���O��true�ɂ���B
		SetFade(MODE_TUTORIAL);//�`���[�g���A����ʂ��t�F�[�h�A�E�g��ɌĂяo��
		//SetMode(MODE_TUTORIAL);//�Q�[�����[�h���Ăяo��
	}

	if (g_nCntTime == 2)
	{
		SetStaging(STAGINGTYPE_PRESSSTART, -1, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 140.0f, 0.0f));
	}
	//if (bFadeFlag == true)
	//{
	//	g_nColorChange-=5;//�F�̒l�����炵�����ăt�F�[�h����B
	//}
	//if (bFadeFlag == false)
	//{
	//	g_nColorChange += 5;
	//	
	//}
	//if (g_nColorChange >= 255)
	//{
	//	g_nColorChange = 255;
	//}
	//���[�h�̐ݒ�i�Q�[����ʂɈڍs�j
	//pVtx[0].col = D3DCOLOR_RGBA(g_nColorChange, g_nColorChange,g_nColorChange,g_nColorChange);
	//pVtx[1].col = D3DCOLOR_RGBA(g_nColorChange, g_nColorChange,g_nColorChange,g_nColorChange);
	//pVtx[2].col = D3DCOLOR_RGBA(g_nColorChange, g_nColorChange,g_nColorChange,g_nColorChange);
	//pVtx[3].col = D3DCOLOR_RGBA(g_nColorChange, g_nColorChange,g_nColorChange,g_nColorChange);

	//���_�o�b�t�@���A�����b�N����
	//g_pVtxBuffTitle->Unlock();

	if (g_nColorChange <= 0)
	{//��ʂ����S�ɈÂ��Ȃ�����E�E�E
		
	}
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
		pDevice->SetTexture(0, g_pTextureTitle);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);//0�́A�`�悷��ŏ��̒��_�C���f�b�N�X�ŁA2�́A�`�悷��v���~�e�B�u��
			//&g_pVtxBuffPolygon[0],//���_���̐擪�A�h���X
			//sizeof(VERTEX_2D));//���_���\���̂̃T�C�Y
}