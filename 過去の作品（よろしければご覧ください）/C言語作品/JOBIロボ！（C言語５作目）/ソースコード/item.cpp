//========================================================
//
//�P�Q���Q�U���F���f���ǂݍ���[Item.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#include "Item.h"
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "shadow.h"
#include "player.h"
#include <stdio.h>
#include <string.h>
#include "model.h"
#include "score.h"
#include "bullet.h"
#include "particle.h"
#include "mission.h"

//=============================
//�O���[�o���ϐ�
//=============================
int g_nNumItem = 0;//�ǂݍ��ރ��f���̍ő吔�����߂�
int g_nDelayItem = 0;
D3DXVECTOR3 g_posItem;//�ʒu
D3DXVECTOR3 g_rotItem;//����
D3DXMATRIX g_mtxWorldItem;//���[���h�}�g���b�N�X
Item g_aItem[MAX_ITEM];//���f�����̍\����
ItemMat g_aItemMat[NUM_ITEM];//���f���̃}�e���A���Ⓒ�_�̍\����

//=============================
//�L�����f�B�\��
//=============================
CandyDisp g_aCandyDisp;//�\����
LPDIRECT3DTEXTURE9 g_pTextureCandyDisp = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCandyDisp = NULL;

//=============================
//�L�����f�B�l�����\��
//=============================
CandyNum g_aCandyNum[CANDY_DIGIT];//�\����
LPDIRECT3DTEXTURE9 g_pTextureCandyNum = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCandyNum = NULL;

int g_nldxShadowItem = -1;//�Ώۂ̉e�̃C���f�b�N�X�i�ԍ��j

//==============================
// ����������
//==============================
void InitItem(void)
{
	g_nNumItem = 2;

	g_nDelayItem = 0;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�ւ̃|�C���^�Ǝ擾

	g_posItem = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotItem = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	for (int nCntMatItem = 0; nCntMatItem < NUM_ITEM; nCntMatItem++)
	{
		g_aItemMat[nCntMatItem].dwNumMatItem = {};
		g_aItemMat[nCntMatItem].pBuffMatItem = {};
		g_aItemMat[nCntMatItem].pMeshItem = {};

		for (int nCntTexture = 0; nCntTexture < MAX_ITEMTEXTURE; nCntTexture++)
		{
			g_aItemMat[nCntMatItem].apTextureItem[nCntTexture] = {};//��̃��f���Ɋ֘A����e�N�X�`���̏�����
		}
	}
	for (int nCnt = 0; nCnt < MAX_ITEM; nCnt++)
	{
		g_aItem[nCnt].bUse = false;//�g�p���Ă��邩�ǂ���
		g_aItem[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�ʒu
		g_aItem[nCnt].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//1F�O�̈ʒu
		g_aItem[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�ړ���
		g_aItem[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//����
		g_aItem[nCnt].nIdxShadow = -1;
		g_aItem[nCnt].nType = -1;
		g_aItem[nCnt].vtxMinItem = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//���ꂼ��̍��W�̃��f���̈ʒu�̍ŏ��l
		g_aItem[nCnt].vtxMaxItem = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//���ꂼ��̍��W�̃��f���̈ʒu�̍ő�l
		g_aItem[nCnt].nNumVtx = 0;//���_��
		g_aItem[nCnt].sizeFVF;//���_�t�H�[�}�b�g�̃T�C�Y
		g_aItem[nCnt].pVtxBuff;//���_�o�b�t�@�ւ̃|�C���^
		g_aItem[nCnt].Size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�G�̑傫��
		g_aItem[nCnt].bIsLanding = false;//�n�ʂɂ��Ă��邩�ǂ���
		g_aItem[nCnt].nNumLandingModel = 0;//�ǂ̃��f���̏�ɏ���Ă��邩
	}

	//���f���̓ǂݍ���
	D3DXLoadMeshFromX("data\\MODEL\\Item\\Candy_000.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_aItemMat[ITEM00_CANDY].pBuffMatItem,
		NULL,
		&g_aItemMat[ITEM00_CANDY].dwNumMatItem,
		&g_aItemMat[ITEM00_CANDY].pMeshItem);

	D3DXMATERIAL* pMat;//�}�e���A���ւ̃|�C���^

	for (int nCntItemSET = 0; nCntItemSET < NUM_ITEM; nCntItemSET++)
	{
		//�}�e���A���ւ̃|�C���^���擾�i�Z�b�g�������f���Ɠ����^�C�v�j
		pMat = (D3DXMATERIAL*)g_aItemMat[nCntItemSET].pBuffMatItem->GetBufferPointer();
		int nNumTexture = 0;
		for (int nCntMat = 0; nCntMat < (int)g_aItemMat[nCntItemSET].dwNumMatItem; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{//�e�N�X�`���t�@�C�������݂���
					//�e�N�X�`���̓ǂݍ���
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_aItemMat[nCntItemSET].apTextureItem[nNumTexture]);
				nNumTexture++;
			}
		}
	}
}

//==============================
// �I������
//==============================
void UninitItem(void)
{
	for (int nCnt = 0; nCnt < NUM_ITEM; nCnt++)
	{
		//���b�V���̔j��
		if (g_aItemMat[nCnt].pMeshItem != NULL)
		{
			g_aItemMat[nCnt].pMeshItem->Release();
			g_aItemMat[nCnt].pMeshItem = NULL;
		}
		//�}�e���A���̔j��
		if (g_aItemMat[nCnt].pBuffMatItem != NULL)
		{
			g_aItemMat[nCnt].pBuffMatItem->Release();
			g_aItemMat[nCnt].pBuffMatItem = NULL;
		}
	}
}
//============================
// �X�V����
//============================
void UpdateItem(void)
{
	Player* pPlayer = GetPlayer();

	g_nDelayItem++;

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse == true)
		{
			Mission* pMission = GetMission();

			g_aItem[nCntItem].bIsLanding = bCollisionModel(&g_aItem[nCntItem].pos, &g_aItem[nCntItem].posOld, g_aItem[nCntItem].Size, g_aItem[nCntItem].nNumLandingModel);

			if (g_aItem[nCntItem].bIsLanding == false)
			{
				g_aItem[nCntItem].move.y = -2.0f;//�d�͏���
			}

			if (g_aItem[nCntItem].pos.y <= 0.0f)
			{
				g_aItem[nCntItem].pos.y = 0.0f;
				g_aItem[nCntItem].move.y = 0.0f;
			}

			switch (g_aItem[nCntItem].nType)
			{
			case ITEM00_CANDY:
				if (g_aItem[nCntItem].pos.x + g_aItem[nCntItem].Size.x / 2 >= pPlayer->pos.x - pPlayer->Size.x / 2 &&
					g_aItem[nCntItem].pos.x - g_aItem[nCntItem].Size.x / 2 <= pPlayer->pos.x + pPlayer->Size.x / 2 &&
					g_aItem[nCntItem].pos.y + g_aItem[nCntItem].Size.y >= pPlayer->pos.y &&
					g_aItem[nCntItem].pos.y <= pPlayer->pos.y + pPlayer->Size.y &&
					g_aItem[nCntItem].pos.z + g_aItem[nCntItem].Size.z / 2 >= pPlayer->pos.z - pPlayer->Size.z / 2 &&
					g_aItem[nCntItem].pos.z - g_aItem[nCntItem].Size.z / 2 <= pPlayer->pos.z + pPlayer->Size.z / 2)
				{
					PlaySound(SOUND_LABEL_SE_GETCANDY_000);
					pMission->aMission0.nGetCandyCount++;
					AddCandyNum(1);
					g_aItem[nCntItem].bUse = false;
				}

				break;
			}

			if (g_aItem[nCntItem].bUse == false)
			{
				/*	PlaySound(SOUND_LABEL_SE_DEFEAT_Item);*/
			}
		}
	}
}

//================================
// �`�揈��
//================================
void DrawItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();;//�f�o�C�X�ւ̃|�C���^�Ǝ擾
	D3DXMATRIX mtxRot, mtxTrans;//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL* pMat;//�}�e���A���f�[�^�ւ̃|�C���^

	//���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_mtxWorldItem);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aItem[nCntItem].rot.y, g_aItem[nCntItem].rot.x, g_aItem[nCntItem].rot.z);

			D3DXMatrixMultiply(&g_mtxWorldItem, &g_mtxWorldItem, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aItem[nCntItem].pos.x, g_aItem[nCntItem].pos.y, g_aItem[nCntItem].pos.z);
			D3DXMatrixMultiply(&g_mtxWorldItem, &g_mtxWorldItem, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldItem);
		
		    //�}�e���A���ւ̃|�C���^���擾�i�Z�b�g�������f���Ɠ����^�C�v�j
		    pMat = (D3DXMATERIAL*)g_aItemMat[g_aItem[nCntItem].nType].pBuffMatItem->GetBufferPointer();
		    
		    int nNumTexture = 0;
		    //�Z�b�g���ꂽ���f���ƃ^�C�v���������̂̃��f���̃}�e���A���̐����ǂݍ��ށB
		    for (int nCntMat = 0; nCntMat < (int)g_aItemMat[g_aItem[nCntItem].nType].dwNumMatItem; nCntMat++)
		    {
		    	//�}�e���A���̐ݒ�
		    	pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
		    
		    	if (pMat[nCntMat].pTextureFilename != NULL)
		    	{
		    		//�e�N�X�`���̐ݒ�
		    		pDevice->SetTexture(0, g_aItemMat[g_aItem[nCntItem].nType].apTextureItem[nNumTexture]);
		    		nNumTexture++;
		    	}
		    	else
		    	{
		    		//�e�N�X�`���̐ݒ�
		    		pDevice->SetTexture(0, g_aItemMat[g_aItem[nCntItem].nType].apTextureItem[nNumTexture]);
		    		nNumTexture++;
		    	}
		    	//���f���i�p�[�c�j�̕`��
		    	g_aItemMat[g_aItem[nCntItem].nType].pMeshItem->DrawSubset(nCntMat);
		    }
		    
			//�ۑ����Ă����}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//====================================
// �L�����f�B�\���̏���������
//====================================
void InitCandyDisp(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^
	pDevice = GetDevice();
	VERTEX_2D* pVtx;
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CandyDisplay_000.png",
		&g_pTextureCandyDisp);


	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffCandyDisp,
		NULL);

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffCandyDisp->Lock(0, 0, (void**)&pVtx, 0);

	g_aCandyDisp.bUse = false;//�g�p���
	g_aCandyDisp.nGetCandy = 0;//�L�����f�B�l����
	g_aCandyDisp.pos = D3DXVECTOR3(1130.0f, 490.0f, 0.0f);//�ʒu

	pVtx[0].pos = D3DXVECTOR3(g_aCandyDisp.pos.x - CANDYDISP_WIDTH, g_aCandyDisp.pos.y - CANDYDISP_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aCandyDisp.pos.x + CANDYDISP_WIDTH, g_aCandyDisp.pos.y - CANDYDISP_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aCandyDisp.pos.x - CANDYDISP_WIDTH, g_aCandyDisp.pos.y + CANDYDISP_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aCandyDisp.pos.x + CANDYDISP_WIDTH, g_aCandyDisp.pos.y + CANDYDISP_HEIGHT, 0.0f);

	//rhw�̐ݒ�i�����ݒ�j
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�i�����ݒ�j
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`�����W�̐ݒ�i�����ݒ�j
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffCandyDisp->Unlock();
}

//===================================
// �L�����f�B�\���̏I������
//===================================
void UninitCandyDisp(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureCandyDisp != NULL)
	{
		g_pTextureCandyDisp->Release();
		g_pTextureCandyDisp = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffCandyDisp != NULL)
	{
		g_pVtxBuffCandyDisp->Release();
		g_pVtxBuffCandyDisp = NULL;
	}
}

//==================================
// �L�����f�B�\���̍X�V����
//==================================
void UpdateCandyDisp(void)
{
}

//==================================
// �L�����f�B�\���̕`�揈��
//==================================
void DrawCandyDisp(void)
{
	LPDIRECT3DDEVICE9 pDevice;

    //�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffCandyDisp, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetTexture(0,g_pTextureCandyDisp);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,0, 2);
	
}

//=======================================
// �L�����f�B�l�����\���̏���������
//=======================================
void InitCandyNum(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^
	pDevice = GetDevice();
	VERTEX_2D* pVtx;
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Lv_Num.png",
		&g_pTextureCandyNum);


	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * CANDY_DIGIT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffCandyNum,
		NULL);

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffCandyNum->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntDigit = 0; nCntDigit < CANDY_DIGIT; nCntDigit++)
	{
		g_aCandyNum[nCntDigit].bUse = false;//�g�p���
		g_aCandyNum[nCntDigit].fPosTexU = 0.0f;//�e�N�X�`�����W
		g_aCandyNum[nCntDigit].pos = D3DXVECTOR3(g_aCandyDisp.pos.x + 105.0f - (CANDYNUM_WIDTH * 2.0f) * nCntDigit,g_aCandyDisp.pos.y,0.0f);

		pVtx[0].pos = D3DXVECTOR3(g_aCandyNum[nCntDigit].pos.x - CANDYNUM_WIDTH, g_aCandyNum[nCntDigit].pos.y - CANDYNUM_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aCandyNum[nCntDigit].pos.x + CANDYNUM_WIDTH, g_aCandyNum[nCntDigit].pos.y - CANDYNUM_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aCandyNum[nCntDigit].pos.x - CANDYNUM_WIDTH, g_aCandyNum[nCntDigit].pos.y + CANDYNUM_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aCandyNum[nCntDigit].pos.x + CANDYNUM_WIDTH, g_aCandyNum[nCntDigit].pos.y + CANDYNUM_HEIGHT, 0.0f);

		//rhw�̐ݒ�i�����ݒ�j
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�i�����ݒ�j
		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�i�����ݒ�j
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffCandyNum->Unlock();

}

//=======================================
// �L�����f�B���l�̏I������
//=======================================
void UninitCandyNum(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureCandyNum != NULL)
	{
		g_pTextureCandyNum->Release();
		g_pTextureCandyNum = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffCandyNum != NULL)
	{
		g_pVtxBuffCandyNum->Release();
		g_pVtxBuffCandyNum = NULL;
	}
}

//=======================================
// �L�����f�B���l�̍X�V����
//=======================================
void UpdateCandyNum(void)
{

}

//=======================================
// �L�����f�B���l�̕`�揈��
//=======================================
void DrawCandyNum(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffCandyNum, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetTexture(0, g_pTextureCandyNum);

	for (int nCntDigit = 0; nCntDigit < CANDY_DIGIT; nCntDigit++)
	{
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,nCntDigit * 4, 2);
	}
}

//===================================
// �L�����f�B���l�̉��Z����
//===================================
void AddCandyNum(int nCandy)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();

	VERTEX_2D* pVtx;

	g_aCandyDisp.nGetCandy += nCandy;
	int nGetCandy = g_aCandyDisp.nGetCandy;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffCandyNum->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < CANDY_DIGIT; nCnt++)
	{
		g_aCandyNum[nCnt].fPosTexU = (float)(nGetCandy % 10);
		nGetCandy = nGetCandy / 10;

		pVtx[0].tex = D3DXVECTOR2(0.0f + (0.1f * g_aCandyNum[nCnt].fPosTexU), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f * g_aCandyNum[nCnt].fPosTexU), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (0.1f * g_aCandyNum[nCnt].fPosTexU), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * g_aCandyNum[nCnt].fPosTexU), 1.0f);

		pVtx += 4;//���_�f�[�^�̃|�C���^���S���i�߂�B
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffCandyNum->Unlock();
}

//===========================================================
// �A�C�e���̐ݒ菈��
//===========================================================
void SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse == false)
		{
			g_aItem[nCntItem].bUse = true;//�g�p���Ă��邩�ǂ���
			g_aItem[nCntItem].pos = pos;//�ʒu
			g_aItem[nCntItem].posOld = pos;//1F�O�̈ʒu
			g_aItem[nCntItem].rot = rot;//����
			g_aItem[nCntItem].nType = nType;//���
			g_aItem[nCntItem].vtxMaxItem = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aItem[nCntItem].vtxMinItem = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aItem[nCntItem].Size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aItem[nCntItem].nNumLandingModel = 0;//�ǂ̃��f���̏�ɏ���Ă��邩

			//=====================================
			//���f���̈ʒu�̍ő�ŏ��l���擾����
			//=====================================
		
			//���_���̎擾
			g_aItem[nCntItem].nNumVtx = g_aItemMat[g_aItem[nCntItem].nType].pMeshItem->GetNumVertices();

			//���_�t�H�[�}�b�g�̃T�C�Y���擾
			g_aItem[nCntItem].sizeFVF = D3DXGetFVFVertexSize(g_aItemMat[g_aItem[nCntItem].nType].pMeshItem->GetFVF());

			//���_�o�b�t�@�̃��b�N
			g_aItemMat[g_aItem[nCntItem].nType].pMeshItem->LockVertexBuffer(D3DLOCK_READONLY, (void**)&g_aItem[nCntItem].pVtxBuff);

			for (int nCntVtx = 0; nCntVtx < g_aItem[nCntItem].nNumVtx; nCntVtx++)
			{
				//���_���W�̑��
				D3DXVECTOR3 vtx = *(D3DXVECTOR3*)g_aItem[nCntItem].pVtxBuff;

				//====================================================
				//���_���W���r���ă��f���̍ŏ��l�ő�l���擾
				//====================================================
				if (vtx.x > g_aItem[nCntItem].vtxMaxItem.x)
				{
					g_aItem[nCntItem].vtxMaxItem.x = vtx.x;
				}
				else if (vtx.x < g_aItem[nCntItem].vtxMinItem.x)
				{//����ǂݍ��񂾒��_���A��ԏ��������_��菬�����ꍇ
					g_aItem[nCntItem].vtxMinItem.x = vtx.x;
				}
				else if (vtx.y > g_aItem[nCntItem].vtxMaxItem.y)
				{//����ǂݍ��񂾒��_���A��ԏ��������_��菬�����ꍇ
					g_aItem[nCntItem].vtxMaxItem.y = vtx.y;
				}
				else if (vtx.y < g_aItem[nCntItem].vtxMinItem.y)
				{//����ǂݍ��񂾒��_���A��ԏ��������_��菬�����ꍇ
					g_aItem[nCntItem].vtxMinItem.y = vtx.y;
				}
				else if (vtx.z > g_aItem[nCntItem].vtxMaxItem.z)
				{//����ǂݍ��񂾒��_���A��ԏ��������_��菬�����ꍇ
					g_aItem[nCntItem].vtxMaxItem.z = vtx.z;
				}
				else if (vtx.z < g_aItem[nCntItem].vtxMinItem.z)
				{//����ǂݍ��񂾒��_���A��ԏ��������_��菬�����ꍇ
					g_aItem[nCntItem].vtxMinItem.z = vtx.z;
				}
				
				//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
				g_aItem[nCntItem].pVtxBuff += g_aItem[nCntItem].sizeFVF;
			}

			//���_�o�b�t�@�̃A�����b�N
			g_aItemMat[g_aItem[nCntItem].nType].pMeshItem->UnlockVertexBuffer();

			g_aItem[nCntItem].Size = g_aItem[nCntItem].vtxMaxItem - g_aItem[nCntItem].vtxMinItem;

			break;
		}
	}
}

int GetNumItem(void)
{
	return g_nNumItem;//�ǂݍ��ރ��f���̍ő吔���擾����
}

Item* GetItem(void)//�G�\���̂̏����擾����
{
	return &g_aItem[0];
}
