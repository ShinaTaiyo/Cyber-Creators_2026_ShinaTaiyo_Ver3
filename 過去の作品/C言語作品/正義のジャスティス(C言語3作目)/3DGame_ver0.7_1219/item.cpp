//========================================================
//
//�P�P���V���F���f���ǂݍ���[Item.cpp]
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
#include "combo.h"
#include "SkillGage.h"
#include "particle.h"
//�O���[�o���ϐ�
int g_nNumItem = 0;//�ǂݍ��ރ��f���̍ő吔�����߂�
#define Item_SCALE (10.0f)
//#define SAVEItem "data\\TEXTFILE\\SaveItem.txt"

#define MAX_ITEMTEXTURE (32)
int g_nDelayItem = 0;

typedef struct
{
	LPD3DXMESH pMeshItem;//���b�V���i���_���j�ւ̃|�C���^�i���f���̎�ސ����j
	LPD3DXBUFFER pBuffMatItem;//�}�e���A���ւ̃|�C���^
	DWORD dwNumMatItem;//�}�e���A���̐�
	LPDIRECT3DTEXTURE9 apTextureItem[MAX_ITEMTEXTURE];//�e�N�X�`���ւ̃|�C���^
}ItemMat;

D3DXVECTOR3 g_posItem;//�ʒu
D3DXVECTOR3 g_rotItem;//����
D3DXMATRIX g_mtxWorldItem;//���[���h�}�g���b�N�X

Item g_aItem[MAX_ITEM];//���f�����̍\����

ItemMat g_aItemMat[NUM_ITEM];//���f���̃}�e���A���Ⓒ�_�̍\����

#define MAX_DELAY (50)//�S�̓I�ɍU�����x��������

int g_nldxShadowItem = -1;//�Ώۂ̉e�̃C���f�b�N�X�i�ԍ��j
//�^�C�g����ʂ̏���������
void InitItem(void)
{

	//for (int nCntItem = 0; nCntItem < NUM_ITEM; nCntItem++)
	//{
	//	g_dwNumMatItem[nCntItem] = 0;//�}�e���A���̐�
	//}
	//g_nldxShadow = 0;//�Ώۂ̉e�̃C���f�b�N�X

	g_nNumItem = 2;

	g_nDelayItem = 0;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�ւ̃|�C���^�Ǝ擾

	g_posItem = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotItem = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//g_bIsLanding = false;//�n�ʂɂ��邩�ǂ���



	

	


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
	}

	//���f���̓ǂݍ���
	D3DXLoadMeshFromX("data\\MODEL\\Item\\Item00_HealHart.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_aItemMat[ITEM00_HEALHART].pBuffMatItem,
		NULL,
		&g_aItemMat[ITEM00_HEALHART].dwNumMatItem,
		&g_aItemMat[ITEM00_HEALHART].pMeshItem);

	//���f���̓ǂݍ���
	D3DXLoadMeshFromX("data\\MODEL\\Item\\Item01_EnergyBoost.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_aItemMat[ITEM01_ENERGYBOOST].pBuffMatItem,
		NULL,
		&g_aItemMat[ITEM01_ENERGYBOOST].dwNumMatItem,
		&g_aItemMat[ITEM01_ENERGYBOOST].pMeshItem);


	//���f���̓ǂݍ���
	D3DXLoadMeshFromX("data\\MODEL\\Item\\Item02_PowerFullSword.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_aItemMat[ITEM02_POWFULSWORD].pBuffMatItem,
		NULL,
		&g_aItemMat[ITEM02_POWFULSWORD].dwNumMatItem,
		&g_aItemMat[ITEM02_POWFULSWORD].pMeshItem);


	//���f���̓ǂݍ���
	D3DXLoadMeshFromX("data\\MODEL\\Item\\Item03_CapsuleBarrier.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_aItemMat[ITEM03_CAPSULEBARRIER].pBuffMatItem,
		NULL,
		&g_aItemMat[ITEM03_CAPSULEBARRIER].dwNumMatItem,
		&g_aItemMat[ITEM03_CAPSULEBARRIER].pMeshItem);


	

	//for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	//{//�S�Ẵ��f���Ɋ��蓖�Ă�
	//	for (int nCntMat = 0; nCntMat < g_nNumItem; nCntMat++)
	//	{//��ނ����蓖�Ă�for��
	//		if (g_aItem[nCntItem].nType == nCntMat)
	//		{//���f���̎�ނ���v

	//			//���_���̎擾
	//			g_aItem[nCntItem].nNumVtx = g_aItemMat[nCntMat].pMeshItem->GetNumVertices();

	//			//���_�t�H�[�}�b�g�̃T�C�Y���擾
	//			g_aItem[nCntItem].sizeFVF = D3DXGetFVFVertexSize(g_aItemMat[nCntMat].pMeshItem->GetFVF());

	//			//���_�o�b�t�@�̃��b�N
	//			g_aItemMat[nCntMat].pMeshItem->LockVertexBuffer(D3DLOCK_READONLY, (void**)&g_aItem[nCntItem].pVtxBuff);


	//			for (int nCntVtx = 0; nCntVtx < g_aItem[nCntItem].nNumVtx; nCntVtx++)
	//			{
	//				//���_���W�̑��
	//				D3DXVECTOR3 vtx = *(D3DXVECTOR3*)g_aItem[nCntItem].pVtxBuff;

	//				//====================================================
	//				//���_���W���r���ă��f���̍ŏ��l�ő�l���擾
	//				//====================================================
	//				if (vtx.x > g_aItem[nCntItem].vtxMaxItem.x)
	//				{
	//					g_aItem[nCntItem].vtxMaxItem.x = vtx.x;
	//				}
	//				else if (vtx.x < g_aItem[nCntItem].vtxMinItem.x)
	//				{//����ǂݍ��񂾒��_���A��ԏ��������_��菬�����ꍇ
	//					g_aItem[nCntItem].vtxMinItem.x = vtx.x;
	//				}
	//				else if (vtx.y > g_aItem[nCntItem].vtxMaxItem.y)
	//				{//����ǂݍ��񂾒��_���A��ԏ��������_��菬�����ꍇ
	//					g_aItem[nCntItem].vtxMaxItem.y = vtx.y;
	//				}
	//				else if (vtx.y < g_aItem[nCntItem].vtxMinItem.y)
	//				{//����ǂݍ��񂾒��_���A��ԏ��������_��菬�����ꍇ
	//					g_aItem[nCntItem].vtxMinItem.y = vtx.y;
	//				}
	//				else if (vtx.z > g_aItem[nCntItem].vtxMaxItem.z)
	//				{//����ǂݍ��񂾒��_���A��ԏ��������_��菬�����ꍇ
	//					g_aItem[nCntItem].vtxMaxItem.z = vtx.z;
	//				}
	//				else if (vtx.z < g_aItem[nCntItem].vtxMinItem.z)
	//				{//����ǂݍ��񂾒��_���A��ԏ��������_��菬�����ꍇ
	//					g_aItem[nCntItem].vtxMinItem.z = vtx.z;
	//				}
	//				//========================================================================

	//				//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
	//				g_aItem[nCntItem].pVtxBuff += g_aItem[nCntItem].sizeFVF;

	//			}

	//			//���_�o�b�t�@�̃A�����b�N
	//			g_aItemMat[nCntMat].pMeshItem->UnlockVertexBuffer();

	//			//��ޓ��肵�����A�����^�C�v��for���񂷕K�v�Ȃ��̂ŁAbreak
	//			break;

	//		}
	//	}
	//}
	//���_���̎擾



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
//�^�C�g����ʂ̏I������
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
//���f���̍X�V����
//============================
void UpdateItem(void)
{
	Player* pPlayer = GetPlayer();

	g_nDelayItem++;

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse == true)
		{
			g_aItem[nCntItem].bIsLanding = bCollisionModel(&g_aItem[nCntItem].pos, &g_aItem[nCntItem].posOld, &g_aItem[nCntItem].move, g_aItem[nCntItem].Size);

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
			case ITEM00_HEALHART:
				if (pPlayer->pos.x + pPlayer->Size.x / 2 >= g_aItem[nCntItem].pos.x - g_aItem[nCntItem].Size.x / 2 &&
					pPlayer->pos.x - pPlayer->Size.x / 2 <= g_aItem[nCntItem].pos.x + g_aItem[nCntItem].Size.x / 2 &&
					pPlayer->pos.y + pPlayer->Size.y >= g_aItem[nCntItem].pos.y - g_aItem[nCntItem].Size.y &&
					pPlayer->pos.y <= g_aItem[nCntItem].pos.y + g_aItem[nCntItem].Size.y &&
					pPlayer->pos.z + pPlayer->Size.z / 2 >= g_aItem[nCntItem].pos.z - g_aItem[nCntItem].Size.z / 2 &&
					pPlayer->pos.z - pPlayer->Size.z / 2 <= g_aItem[nCntItem].pos.z + g_aItem[nCntItem].Size.z / 2)
						{
							pPlayer->nHp += 300;//�̗͂��R�O�O�񕜂���
							g_aItem[nCntItem].bUse = false;
							PlaySound(SOUND_LABEL_SE_HEAL_000);
						}
				break;
			case ITEM01_ENERGYBOOST:
				if (pPlayer->pos.x + pPlayer->Size.x / 2 >= g_aItem[nCntItem].pos.x - g_aItem[nCntItem].Size.x / 2 &&
					pPlayer->pos.x - pPlayer->Size.x / 2 <= g_aItem[nCntItem].pos.x + g_aItem[nCntItem].Size.x / 2 &&
					pPlayer->pos.y + pPlayer->Size.y >= g_aItem[nCntItem].pos.y - g_aItem[nCntItem].Size.y &&
					pPlayer->pos.y <= g_aItem[nCntItem].pos.y + g_aItem[nCntItem].Size.y &&
					pPlayer->pos.z + pPlayer->Size.z / 2 >= g_aItem[nCntItem].pos.z - g_aItem[nCntItem].Size.z / 2 &&
					pPlayer->pos.z - pPlayer->Size.z / 2 <= g_aItem[nCntItem].pos.z + g_aItem[nCntItem].Size.z / 2)
				{
					AddSkillGage(300.0f);
					g_aItem[nCntItem].bUse = false;
					PlaySound(SOUND_LABEL_SE_ORBGET);
				}
				break;
			case ITEM02_POWFULSWORD:
				if (pPlayer->pos.x + pPlayer->Size.x / 2 >= g_aItem[nCntItem].pos.x - g_aItem[nCntItem].Size.x / 2 &&
					pPlayer->pos.x - pPlayer->Size.x / 2 <= g_aItem[nCntItem].pos.x + g_aItem[nCntItem].Size.x / 2 &&
					pPlayer->pos.y + pPlayer->Size.y >= g_aItem[nCntItem].pos.y - g_aItem[nCntItem].Size.y &&
					pPlayer->pos.y <= g_aItem[nCntItem].pos.y + g_aItem[nCntItem].Size.y &&
					pPlayer->pos.z + pPlayer->Size.z / 2 >= g_aItem[nCntItem].pos.z - g_aItem[nCntItem].Size.z / 2 &&
					pPlayer->pos.z - pPlayer->Size.z / 2 <= g_aItem[nCntItem].pos.z + g_aItem[nCntItem].Size.z / 2)
				{
					pPlayer->nAttackUpStateCnt += 20;//�����̊ԍU���͂Q�{
					g_aItem[nCntItem].bUse = false;
					PlaySound(SOUND_LABEL_SE_SPECIALATTACK);
				}
				break;
			case ITEM03_CAPSULEBARRIER:
				if (pPlayer->pos.x + pPlayer->Size.x / 2 >= g_aItem[nCntItem].pos.x - g_aItem[nCntItem].Size.x / 2 &&
					pPlayer->pos.x - pPlayer->Size.x / 2 <= g_aItem[nCntItem].pos.x + g_aItem[nCntItem].Size.x / 2 &&
					pPlayer->pos.y + pPlayer->Size.y >= g_aItem[nCntItem].pos.y - g_aItem[nCntItem].Size.y &&
					pPlayer->pos.y <= g_aItem[nCntItem].pos.y + g_aItem[nCntItem].Size.y &&
					pPlayer->pos.z + pPlayer->Size.z / 2 >= g_aItem[nCntItem].pos.z - g_aItem[nCntItem].Size.z / 2 &&
					pPlayer->pos.z - pPlayer->Size.z / 2 <= g_aItem[nCntItem].pos.z + g_aItem[nCntItem].Size.z / 2)
				{
					pPlayer->nBarrierCount += 5;//�T�񂾂��_���[�W�𖳌��ɂ���o���A��t�^�B
					g_aItem[nCntItem].bUse = false;
					PlaySound(SOUND_LABEL_SE_BARRIER);
				}
				break;
			}
			float fRandumRot = float(rand() % 200 + 1) / 100;
			float fRandumSpeed = float(rand() % 100 + 30) / 10;
			float fRandumMove = float(rand() % 21 - 10);
			float fRandumLength = float(rand() % 550 + 200);

			float fRandumColorR = float(rand() % 100 + 1) / 100;
			float fRandumColorG = float(rand() % 100 + 1) / 100;
			float fRandumColorB = float(rand() % 100 + 1) / 100;
			SetParticle(PARTICLETYPE_NORMAL, 200, 30.0f, 0.5f, g_aItem[nCntItem].pos,
				D3DXVECTOR3(sinf(-D3DX_PI * fRandumRot) * fRandumSpeed, fRandumMove, cosf(-D3DX_PI * fRandumRot) * fRandumSpeed),
				D3DXCOLOR(fRandumColorR, fRandumColorG, fRandumColorB, 1.0f), 0.0f, PARTICLETEX_NORMAL);

			SetPositionShadow(g_aItem[nCntItem].nIdxShadow, D3DXVECTOR3(g_aItem[nCntItem].pos.x, g_aItem[nCntItem].pos.y - g_aItem[nCntItem].Size.y / 2, g_aItem[nCntItem].pos.z), -1, nCntItem, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));


			if (g_aItem[nCntItem].bUse == false)
			{

				/*	PlaySound(SOUND_LABEL_SE_DEFEAT_Item);*/
				KillShadow(g_aItem[nCntItem].nIdxShadow);
			}


		}
	}



}

//================================
//�^�C�g����ʂ̕`�揈��
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

			for (int nCntItemSET = 0; nCntItemSET < NUM_ITEM; nCntItemSET++)
			{//�ǂݍ��ރ��f���̎�ސ���for�����񂵁A�����^�C�v�̃}�e���A���̃|�C���^��A�A
			//�p�[�c�̕`��A�}�e���A���̐ݒ������B
				if (g_aItem[nCntItem].nType == nCntItemSET)
				{
					//�}�e���A���ւ̃|�C���^���擾�i�Z�b�g�������f���Ɠ����^�C�v�j
					pMat = (D3DXMATERIAL*)g_aItemMat[nCntItemSET].pBuffMatItem->GetBufferPointer();

					int nNumTexture = 0;
					//�Z�b�g���ꂽ���f���ƃ^�C�v���������̂̃��f���̃}�e���A���̐����ǂݍ��ށB
					for (int nCntMat = 0; nCntMat < (int)g_aItemMat[nCntItemSET].dwNumMatItem; nCntMat++)
					{
						//�}�e���A���̐ݒ�
						pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

						if (pMat[nCntMat].pTextureFilename != NULL)
						{
							//�e�N�X�`���̐ݒ�
							pDevice->SetTexture(0, g_aItemMat[nCntItemSET].apTextureItem[nNumTexture]);
							nNumTexture++;
						}
						else
						{
							//�e�N�X�`���̐ݒ�
							pDevice->SetTexture(0, g_aItemMat[nCntItemSET].apTextureItem[nNumTexture]);
							nNumTexture++;
						}
						//���f���i�p�[�c�j�̕`��
						g_aItemMat[nCntItemSET].pMeshItem->DrawSubset(nCntMat);
					}
					break;
				}
			}

			//�ۑ����Ă����}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

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
			g_aItem[nCntItem].nIdxShadow = SetShadow();


			//=====================================
			//���f���̈ʒu�̍ő�ŏ��l���擾����
			//=====================================
			for (int nCntMat = 0; nCntMat < g_nNumItem; nCntMat++)
			{//��ނ����蓖�Ă�for��
				if (g_aItem[nCntItem].nType == nCntMat)
				{//���f���̎�ނ���v

					//���_���̎擾
					g_aItem[nCntItem].nNumVtx = g_aItemMat[nCntMat].pMeshItem->GetNumVertices();

					//���_�t�H�[�}�b�g�̃T�C�Y���擾
					g_aItem[nCntItem].sizeFVF = D3DXGetFVFVertexSize(g_aItemMat[nCntMat].pMeshItem->GetFVF());

					//���_�o�b�t�@�̃��b�N
					g_aItemMat[nCntMat].pMeshItem->LockVertexBuffer(D3DLOCK_READONLY, (void**)&g_aItem[nCntItem].pVtxBuff);


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
						//========================================================================

						//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
						g_aItem[nCntItem].pVtxBuff += g_aItem[nCntItem].sizeFVF;

					}

					//���_�o�b�t�@�̃A�����b�N
					g_aItemMat[nCntMat].pMeshItem->UnlockVertexBuffer();

					//��ޓ��肵�����A�����^�C�v��for���񂷕K�v�Ȃ��̂ŁAbreak
					break;

				}
			}
			//============================================================================================================================

			g_aItem[nCntItem].Size = g_aItem[nCntItem].vtxMaxItem - g_aItem[nCntItem].vtxMinItem;

			
			
			//===========================================================
			//g_aItem[nCntItem].nIdxShadow = SetShadow();//�e�̃C���f�b�N�X��ݒ�
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
