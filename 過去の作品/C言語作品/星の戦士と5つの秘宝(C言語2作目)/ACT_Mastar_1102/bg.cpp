//========================================================
//
//�W���R�O���F�G�t�F�N�g������񌩒���[bg.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#include "main.h"
#include "bg.h"
#include "Item.h"
#include "character.h"
#include "map.h"
//�}�N����`
#define NUM_BG (BG_MAX)//�w�i�̐�

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_apTextureBG[NUM_BG] = {};//�e�N�X�`��3�����̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBG = NULL;//���_�o�b�t�@�ւ̃|�C���^
float g_aPosTexV;//�e�N�X�`�����W�̊J�n�ʒu�iV�l�j

float g_fColorChangeBg;

int g_nCntTime;

int g_nBG = 0;

int g_nUaeMap;

//============================
//�w�i�̏���������
//============================
void InitBG(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBG;

	g_nCntTime = 0;

	 g_nBG = 0;

	 g_nUaeMap = 0;

	g_fColorChangeBg = 1.0f;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\BG_CentralCircle.png",
		&g_apTextureBG[BG_CENTRALCIRCLE]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Grassland_000.jpg",
		&g_apTextureBG[BG_STAGE1]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\SnowFieldBG4.jpg",
		&g_apTextureBG[BG_STAGE2]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\BG_Volcano.jpg",
		&g_apTextureBG[BG_STAGE3]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\BG_OnCloud.jpg",
		&g_apTextureBG[BG_STAGE4]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\BG_FinalBattle.jpg",
		&g_apTextureBG[BG_LASTSTAGE]);

	/*D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Grassland_000.jpg",
		&g_apTextureBG[BG_STAGE1]);*/

	////====================================
	////�w�i�𕡐����g�������Ȃ�����g��
	////====================================
	//D3DXCreateTextureFromFile(pDevice,
	//	"data\\Texture\\inseki.png",
	//	&g_apTextureBG[1]);

	//D3DXCreateTextureFromFile(pDevice,
	//	"data\\Texture\\insekiyuka.png",
	//	&g_apTextureBG[2]);
	//==============================================================

	//�e�N�X�`�����W�̊J�n�ʒu�iV�l�j�̏�����
	//for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	//{
		g_aPosTexV = 0.0f;
	//}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_BG,//�S�͕K�v�Ȓ��_��
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBG,
		NULL);

	VERTEX_2D* pVtx;
	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);

	/*for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{*/
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH , 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f,SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH,SCREEN_HEIGHT, 0.0f);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
		pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
		pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
		pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(g_aPosTexV,0.0f);
		pVtx[1].tex = D3DXVECTOR2(g_aPosTexV+1.0f,0.0f);
		pVtx[2].tex = D3DXVECTOR2(g_aPosTexV,1.0f);
		pVtx[3].tex = D3DXVECTOR2(g_aPosTexV+1.0f,1.0f);

	//	pVtx += 4;//���_�f�[�^�̃|�C���^���S���i�߂�B
	//	
	//}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBG->Unlock();
}

//==========================
//�w�i�̏I������
//==========================
void UninitBG(void)
{
	//�e�N�X�`���̔j��
	int nCntBG;
	for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		if (g_apTextureBG[nCntBG] != NULL)
		{
			g_apTextureBG[nCntBG]->Release();
			g_apTextureBG[nCntBG] = NULL;
		}
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffBG != NULL)
	{
		g_pVtxBuffBG->Release();
		g_pVtxBuffBG = NULL;
	}
}

//==================================
//�w�i�̍X�V����
//==================================
void UpDateBG(void)
{
	int nCntBG;

	CharacterNum* pCharacterNum = GetCharacterNum();


	ItemEffect* pItemEffect = GetItemEffect();

	int *pUseMap = GetUseMap();//�g���Ă���}�b�v���ǂꂩ���肷��

	//====================================
	//�w�i�����肷�鏈��
	//====================================
	if (*pUseMap == MAPTYPE_8)
	{//�u�Z���g�����T�[�N���v�̔w�i
		g_nBG = BG_CENTRALCIRCLE;
	}
	else if (*pUseMap >= MAPTYPE_0 && *pUseMap <= MAPTYPE_5)
	{//�u�X�e�[�W�P�v�̔w�i
		g_nBG = BG_STAGE1;
	}
	else if (*pUseMap >= MAPTYPE_6 && *pUseMap <= 7)
	{//�u�X�e�[�W�Q�v�̔w�i
		g_nBG = BG_STAGE2;
	}
	else if (*pUseMap >= MAPTYPE_9 && *pUseMap <= MAPTYPE_13)
	{//�u�X�e�[�W�Q�v�̔w�i
		g_nBG = BG_STAGE2;
	}
	else if (*pUseMap >= MAPTYPE_14 && *pUseMap <= MAPTYPE_22)
	{//�u�X�e�[�W3�v�̔w�i
		g_nBG = BG_STAGE3;
	}
	else if (*pUseMap >= MAPTYPE_23 && *pUseMap <= MAPTYPE_31)
	{//�u�X�e�[�W�S�v�̔w�i
		g_nBG = BG_STAGE4;

	}
	else if (*pUseMap >= MAPTYPE_32/* && *pUseMap <= MAPTYPE_31*/)
	{//�u���X�g�X�e�[�W�v�̔w�i
		g_nBG = BG_LASTSTAGE;

	}
	//====================================================================
	

	//else if(*pUseMap )

	//g_nCntTime++;

	if (g_nCntTime == 1)
	{
		if (pCharacterNum->nCharacterNum == 0)
		{
			pItemEffect->fmoveBg += 0.002f;
		}
	}

	VERTEX_2D* pVtx;

	

	g_fColorChangeBg = 1.0f;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);

	//for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	//{
	//
	//	switch (nCntBG)
	//	{
	//	case 0:
	//		/*g_aPosTexV[nCntBG] -= 0.0025f;*/ //�w�i�����������Ȃ�g���B

				//�e�N�X�`�����W�̐ݒ�
			g_aPosTexV += 0.001f;
			
			pVtx[0].col = D3DXCOLOR(g_fColorChangeBg, g_fColorChangeBg, g_fColorChangeBg, 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
			pVtx[1].col = D3DXCOLOR(g_fColorChangeBg, g_fColorChangeBg, g_fColorChangeBg, 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
			pVtx[2].col = D3DXCOLOR(g_fColorChangeBg, g_fColorChangeBg, g_fColorChangeBg, 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
			pVtx[3].col = D3DXCOLOR(g_fColorChangeBg, g_fColorChangeBg, g_fColorChangeBg, 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B


		////�e�N�X�`�����W�̐ݒ�
		//	pVtx[0].tex = D3DXVECTOR2(g_aPosTexV, 0.0f);
		//	pVtx[1].tex = D3DXVECTOR2(g_aPosTexV + 1.0f, 0.0f);
		//	pVtx[2].tex = D3DXVECTOR2(g_aPosTexV, 1.0f);
		//	pVtx[3].tex = D3DXVECTOR2(g_aPosTexV + 1.0f, 1.0f);

			/*break;*/


			//==================================
			//�g�������Ȃ�����g��
			//==================================

		//case 1:
		//	g_aPosTexV[nCntBG] += 0.002f;

		//	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
		//	pVtx[2].pos = D3DXVECTOR3(0.0f,80.0f, 0.0f);
		//	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 80.0f, 0.0f);

		//	pVtx[0].col = D3DXCOLOR(g_fColorChangeBg, g_fColorChangeBg, g_fColorChangeBg, 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
		//	pVtx[1].col = D3DXCOLOR(g_fColorChangeBg, g_fColorChangeBg, g_fColorChangeBg, 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
		//	pVtx[2].col = D3DXCOLOR(g_fColorChangeBg, g_fColorChangeBg, g_fColorChangeBg, 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
		//	pVtx[3].col = D3DXCOLOR(g_fColorChangeBg, g_fColorChangeBg, g_fColorChangeBg, 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B


		//	pVtx[0].tex = D3DXVECTOR2(g_aPosTexV[nCntBG]+pItemEffect->fmoveBg, 0.0f);
		//	pVtx[1].tex = D3DXVECTOR2(g_aPosTexV[nCntBG]+pItemEffect->fmoveBg + 1.0f, 0.0f);
		//	pVtx[2].tex = D3DXVECTOR2(g_aPosTexV[nCntBG]+pItemEffect->fmoveBg, 1.0f);
		//	pVtx[3].tex = D3DXVECTOR2(g_aPosTexV[nCntBG]+pItemEffect->fmoveBg + 1.0f, 1.0f);

		//	break;

		//case 2:
		//	g_aPosTexV[nCntBG] += 0.002f;

		//	pVtx[0].pos = D3DXVECTOR3(0.0f,SCREEN_HEIGHT-80.0f, 0.0f);
		//	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH,SCREEN_HEIGHT-80.0f, 0.0f);
		//	pVtx[2].pos = D3DXVECTOR3(0.0f,SCREEN_HEIGHT, 0.0f);
		//	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH,SCREEN_HEIGHT, 0.0f);

		//	pVtx[0].col = D3DXCOLOR(g_fColorChangeBg, g_fColorChangeBg, g_fColorChangeBg, 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
		//	pVtx[1].col = D3DXCOLOR(g_fColorChangeBg, g_fColorChangeBg, g_fColorChangeBg, 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
		//	pVtx[2].col = D3DXCOLOR(g_fColorChangeBg, g_fColorChangeBg, g_fColorChangeBg, 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
		//	pVtx[3].col = D3DXCOLOR(g_fColorChangeBg, g_fColorChangeBg, g_fColorChangeBg, 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B


		//	pVtx[0].tex = D3DXVECTOR2(g_aPosTexV[nCntBG], 0.0f);
		//	pVtx[1].tex = D3DXVECTOR2(g_aPosTexV[nCntBG] + 1.0f, 0.0f);
		//	pVtx[2].tex = D3DXVECTOR2(g_aPosTexV[nCntBG], 1.0f);
		//	pVtx[3].tex = D3DXVECTOR2(g_aPosTexV[nCntBG] + 1.0f, 1.0f);

		//	break;
	//	}

	//	//============================================================================================================================================

	//	pVtx += 4;
	//}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBG->Unlock();
}

//==================================
//�w�i�̕`�揈��
//==================================
void DrawBG(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBG;
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBG, 0, sizeof(VERTEX_2D));//���_�o�b�t�@�ւ̃|�C���^�ƒ��_���̍\���̂̃T�C�Y

		//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	//{
		//�e�N�X�`���̐ݒ�
	if (g_nBG == BG_CENTRALCIRCLE)
	{
		pDevice->SetTexture(0, g_apTextureBG[BG_CENTRALCIRCLE]);
	}
	else if (g_nBG == BG_STAGE1)
	{
		pDevice->SetTexture(0, g_apTextureBG[BG_STAGE1]);
	}
	else if (g_nBG == BG_STAGE2)
	{
		pDevice->SetTexture(0, g_apTextureBG[BG_STAGE2]);
	}
	else if (g_nBG == BG_STAGE3)
	{
		pDevice->SetTexture(0, g_apTextureBG[BG_STAGE3]);
	}
	else if (g_nBG == BG_STAGE4)
	{
		pDevice->SetTexture(0, g_apTextureBG[BG_STAGE4]);
	}
	else if (g_nBG == BG_LASTSTAGE)
	{
		pDevice->SetTexture(0, g_apTextureBG[BG_LASTSTAGE]);
	}
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			0,
			2);
	//}
}