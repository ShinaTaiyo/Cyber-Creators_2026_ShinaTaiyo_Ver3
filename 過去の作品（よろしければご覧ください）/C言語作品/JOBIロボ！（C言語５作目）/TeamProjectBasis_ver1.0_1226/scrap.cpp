//=====================================================================================================================
//
//�P�Q���Q�U���F�`�[������̊�ՂÂ���[Scrap.cpp]
//Author:ShinaTaiyo
//
// 
// ���ӁFfRWidth,fRHeight�́A���S�_����̔��a�ő傫���A����Ȃǂ�����Ă��܂��B�������̏������g���Ȃ�A�����Ӊ������B
// 
//======================================================================================================================

#include "scrap.h"
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "shadow.h"
#include "effect.h"
#include "wall.h"
#include "particle.h"
#include "enemy.h"
#include "player.h"

#define Scrap_SCALE (10.0f)
#define SCRAPDIGIT (4)//�X�N���b�v�̐��\���̌���
#define SCRAPUIRADIUS (20.0f)//�X�N���b�v�̕\���̐����̑傫��
//�O���[�o���ϐ�

//====================================
//�X�N���b�v�r���{�[�h�\���p
//====================================
PDIRECT3DVERTEXBUFFER9 g_pVtxBuffScrap = NULL;//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureScrap[NUM_SCRAP] = {};//�e�N�X�`���ւ̃|�C���^
D3DXVECTOR3 g_posScrap;//�ʒu
D3DXMATRIX g_mtxWorldScrap;//���[���h�}�g���b�N�X(float4*4)
Scrap g_aScrap[MAX_SCRAP];//�X�N���b�v�̍\���̏��
int g_nDelayScrap = 0;//�X�N���b�v�̒x�点��J�E���g����
//============================================================================================


//=================================
//�X�N���b�v�Q�[�W�p
//=================================


//============================
//����������
//============================
void InitScrap(void)
{
	//================================
	//�X�N���b�v�r���{�[�h�̏���
	//================================
	g_nDelayScrap = 0;
	
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Scrap000_1.png",
		&g_pTextureScrap[SCRAPTYPE00_TIBIROBO]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Scrap_001.png",
		&g_pTextureScrap[SCRAPTYPE01_SWORD]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Scrap_002.png",
		&g_pTextureScrap[SCRAPTYPE02_BOOMERANG]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Scrap_003.png",
		&g_pTextureScrap[SCRAPTYPE03_SPEAKER]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Scrap_004.png",
		&g_pTextureScrap[SCRAPTYPE04_ALL]);

	g_posScrap = D3DXVECTOR3(0.0f, 50.0f, 100.0f);//�ʒu�̏�����

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_SCRAP,//���_�̐�
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScrap,
		NULL);

	for (int nCntScrap = 0; nCntScrap < MAX_SCRAP; nCntScrap++)
	{
		g_aScrap[nCntScrap].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�ʒu
		g_aScrap[nCntScrap].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�ړ���
		g_aScrap[nCntScrap].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//����
		g_aScrap[nCntScrap].nLife = 0;//�̗�
		g_aScrap[nCntScrap].fRWidth = 0.0f;//���i���a�j
		g_aScrap[nCntScrap].fRHeight = 0.0f;//�����i���a�j
		g_aScrap[nCntScrap].nType = 0;//���
		g_aScrap[nCntScrap].bUse = false;//�g�p���Ă��邩�ǂ���
		g_aScrap[nCntScrap].nIdxShadow = -1;//�e�̃C���f�b�N�X
		g_aScrap[nCntScrap].nAnimationCnt = 0;//�A�j���[�V�����J�E���g
		g_aScrap[nCntScrap].nAnimationPattern = 0;//�A�j���[�V�����p�^�[��
		g_aScrap[nCntScrap].fLength = 0.0f;//�����𑪂�B
		g_aScrap[nCntScrap].fXaim = 0.0f;//X�����̃x�N�g��
		g_aScrap[nCntScrap].fYaim = 0.0f;//Y�����̃x�N�g��
		g_aScrap[nCntScrap].fZaim = 0.0f;//Z�����̃x�N�g��
		g_aScrap[nCntScrap].fVXZaim = 0.0f;//X��Z�̑����x�N�g��
		g_aScrap[nCntScrap].fVYZaim = 0.0f;//Y��Z�̑����x�N�g��
		g_aScrap[nCntScrap].ScrapMoveType = SCRAPMOVETYPE_DIFFUSION;//�X�N���b�v�̈ړ��̎d��
		g_aScrap[nCntScrap].DecayMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�����p�ړ���

		//================================
		//�R�������@�_���ϐ�
		//================================
		g_aScrap[nCntScrap].fVXaim3D;
		g_aScrap[nCntScrap].fVYaim3D;
		g_aScrap[nCntScrap].fVZaim3D;
		g_aScrap[nCntScrap].fVLaim3D;

		//===============================================================================
	}

	VERTEX_3D* pVtx;
	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffScrap->Lock(0, 0, (void**)&pVtx, 0);


	for (int nCntScrap = 0; nCntScrap < MAX_SCRAP; nCntScrap++)
	{
		//���_���W�̐ݒ�i�����ʒu�j
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//nor�̐ݒ�i�����ݒ�j
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B

		//�e�N�X�`�����W�̐ݒ�i�����ݒ�j
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;//�|�C���^��i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffScrap->Unlock();
	//==================================================================================================================================================================================
}

//==============================
//�^�C�g����ʂ̏I������
//==============================
void UninitScrap(void)
{
	//�e�N�X�`���̔j��
	for (int nCntScrap = 0; nCntScrap < NUM_SCRAP; nCntScrap++)
	{
		if (g_pTextureScrap[nCntScrap] != NULL)
		{
			g_pTextureScrap[nCntScrap]->Release();
			g_pTextureScrap[nCntScrap] = NULL;
		}
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffScrap != NULL)
	{
		g_pVtxBuffScrap->Release();
		g_pVtxBuffScrap = NULL;
	}
}
//============================
//�^�C�g����ʂ̍X�V����
//============================
void UpdateScrap(void)
{


	//======================================================================
	//���G���g���Ȃ����̑��ȏ����B�����ʒu�ɃX�N���b�v�𗐐��ŏ���
	//======================================================================
	/*if (g_nDelayScrap % 10 == 0)
	{
		int nSummonScrap = rand() % NUM_SCRAP;
		float fRandumRot = float(rand() % 200 + 1) / 100;
		float fRandumSpeed = float(rand() % 100 - 50) / 10;
		float fRandumMove = float(rand() % 21 - 10);
		float fRandumLength = float(rand() % 550 + 200);
		SetScrap(nSummonScrap, GENERALLYSCRAPLIFE, SCRAPRADIUSSIZE, SCRAPRADIUSSIZE,
			D3DXVECTOR3(0.0f,SCRAPRADIUSSIZE,0.0f)
			,NULL_VECTOR3,
			D3DXVECTOR3(sinf(-D3DX_PI * fRandumRot) * fRandumSpeed, 0.0f, cosf(-D3DX_PI * fRandumRot) + fRandumSpeed));
	}*/
	//=============================================================================

	//=======================================
	//�X�N���b�v�r���{�[�h����
	//=======================================
	VERTEX_3D* pVtx;
	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffScrap->Lock(0, 0, (void**)&pVtx, 0);

	Player* pPlayer = GetPlayer();

	for (int nCntScrap = 0; nCntScrap < MAX_SCRAP; nCntScrap++)
	{
		if (g_aScrap[nCntScrap].bUse == true)
		{
			g_aScrap[nCntScrap].nLife -= 1;//�̗͂����炵������B
			g_aScrap[nCntScrap].nAnimationCnt++;//�A�j���[�V�����J�E���g

			if (g_aScrap[nCntScrap].nLife <= 0)
			{
				g_aScrap[nCntScrap].bUse = false;//�̗͂��O�ȉ��ɂȂ�����false�ɂ���
			}

			//=================================================
			//�X�N���b�v�l������
			//=================================================
			if (g_aScrap[nCntScrap].pos.x + g_aScrap[nCntScrap].fRWidth >= pPlayer->pos.x - pPlayer->Size.x / 2 &&
				g_aScrap[nCntScrap].pos.x - g_aScrap[nCntScrap].fRWidth <= pPlayer->pos.x + pPlayer->Size.x / 2 &&
				g_aScrap[nCntScrap].pos.y + g_aScrap[nCntScrap].fRHeight >= pPlayer->pos.y &&
				g_aScrap[nCntScrap].pos.y - g_aScrap[nCntScrap].fRHeight <= pPlayer->pos.y + pPlayer->Size.y &&
				g_aScrap[nCntScrap].pos.z + g_aScrap[nCntScrap].fRWidth >= pPlayer->pos.z - pPlayer->Size.z / 2 &&
				g_aScrap[nCntScrap].pos.z - g_aScrap[nCntScrap].fRWidth <= pPlayer->pos.z + pPlayer->Size.z / 2)
			{
				switch (g_aScrap[nCntScrap].nType)
				{
				case SCRAPTYPE00_TIBIROBO:
					AddScrapPlayer(SCRAPTYPE00_TIBIROBO,1);//���у��{�����X�N���b�v�l���{�P
					break;
				case SCRAPTYPE01_SWORD:
					AddScrapPlayer(SCRAPTYPE01_SWORD, 1);//���U�������X�N���b�v�l���{�P
					break;
				case SCRAPTYPE02_BOOMERANG:
					AddScrapPlayer(SCRAPTYPE02_BOOMERANG, 1);//�u�[�������U�������X�N���b�v�l���{�P
					break;
				case SCRAPTYPE03_SPEAKER:
					AddScrapPlayer(SCRAPTYPE03_SPEAKER, 1);//�X�s�[�J�[�U�������X�N���b�v�l���{�P
					break;
				case SCRAPTYPE04_ALL:
					AddScrapPlayer(SCRAPTYPE04_ALL, 1);//�S�X�e�[�^�X�����X�N���b�v�l���{�P
					break;
				}

				g_aScrap[nCntScrap].bUse = false;
			}
			//======================================================================================================================================


			//================================================
			//�e�N�X�`���A�j���[�V��������
			//================================================
			if (g_aScrap[nCntScrap].nAnimationCnt % 5 == 0)
			{
				g_aScrap[nCntScrap].nAnimationPattern++;

				if (g_aScrap[nCntScrap].nAnimationPattern >= 5)
				{
					g_aScrap[nCntScrap].nAnimationPattern = 0;
				}
			}
			pVtx[0].tex = D3DXVECTOR2(0.0f + 0.2f * g_aScrap[nCntScrap].nAnimationPattern, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.2f + 0.2f * g_aScrap[nCntScrap].nAnimationPattern, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + 0.2f * g_aScrap[nCntScrap].nAnimationPattern, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.2f + 0.2f * g_aScrap[nCntScrap].nAnimationPattern, 1.0f);
			//=========================================================================================================

			//===================================================
		    //�ړ�����
			//===================================================
		
			switch (g_aScrap[nCntScrap].ScrapMoveType)
			{
			    //=========================================
				//�g�U�����iMOVETYPE_DIFFUSION)
				//=========================================
			case SCRAPMOVETYPE_DIFFUSION:

				if (g_aScrap[nCntScrap].move.x >= -0.01f && g_aScrap[nCntScrap].move.x <= 0.01f &&
					g_aScrap[nCntScrap].move.y >= -0.01f && g_aScrap[nCntScrap].move.y <= 0.01f &&
					g_aScrap[nCntScrap].move.z >= -0.01f && g_aScrap[nCntScrap].move.z <= 0.01f)
				{
					g_aScrap[nCntScrap].ScrapMoveType = SCRAPMOVETYPE_NORMAL;//�ړ��ʂ��O�ɋ߂Â����̂ŁA�ړ��^�C�v��ύX
				}

				//�ړ��ʂ̌���
				g_aScrap[nCntScrap].DecayMove.x = (0.0f - g_aScrap[nCntScrap].move.x) * 0.03f;
				g_aScrap[nCntScrap].DecayMove.y = (0.0f - g_aScrap[nCntScrap].move.y) * 0.03f;
				g_aScrap[nCntScrap].DecayMove.z = (0.0f - g_aScrap[nCntScrap].move.z) * 0.03f;

				
				g_aScrap[nCntScrap].move += g_aScrap[nCntScrap].DecayMove;//�����Âړ��ʂ����炷

			
				break;
				//===================================================================================================================
				

				//========================================
				//���@�_������(MOVETYPE_NORMAL)
				//========================================
			case SCRAPMOVETYPE_NORMAL:
				
				//==========================
	            //�R�������@�_������
	            //==========================
				g_aScrap[nCntScrap].fVXaim3D = pPlayer->pos.x - g_aScrap[nCntScrap].pos.x;
				g_aScrap[nCntScrap].fVYaim3D = pPlayer->pos.y - g_aScrap[nCntScrap].pos.y;
				g_aScrap[nCntScrap].fVZaim3D = pPlayer->pos.z - g_aScrap[nCntScrap].pos.z;

				D3DXVECTOR3 BulletMove = NULL_VECTOR3;

				g_aScrap[nCntScrap].fVLaim3D = sqrtf((g_aScrap[nCntScrap].fVXaim3D * g_aScrap[nCntScrap].fVXaim3D) + 
					(g_aScrap[nCntScrap].fVYaim3D * g_aScrap[nCntScrap].fVYaim3D)
					+ (g_aScrap[nCntScrap].fVZaim3D * g_aScrap[nCntScrap].fVZaim3D));

				BulletMove.x = g_aScrap[nCntScrap].fVXaim3D / g_aScrap[nCntScrap].fVLaim3D * 1.0f;
				BulletMove.y = g_aScrap[nCntScrap].fVYaim3D / g_aScrap[nCntScrap].fVLaim3D * 1.0f;
				BulletMove.z = g_aScrap[nCntScrap].fVZaim3D / g_aScrap[nCntScrap].fVLaim3D * 1.0f;

				//===========================================================================================================

				//==================================================================
		        //�v���C���[�̃X�N���b�v�z���͈͂ɓ�������f�����������
		        //==================================================================
				float fLength = sqrtf((pPlayer->pos.x - g_aScrap[nCntScrap].pos.x) * (pPlayer->pos.x - g_aScrap[nCntScrap].pos.x) +
					(pPlayer->pos.y - g_aScrap[nCntScrap].pos.y) * (pPlayer->pos.y - g_aScrap[nCntScrap].pos.y) +
					(pPlayer->pos.z - g_aScrap[nCntScrap].pos.z) * (pPlayer->pos.z - g_aScrap[nCntScrap].pos.z)) / 3.0f;

				if (fLength < pPlayer->fSuctionScrapRange)
				{
					BulletMove.x = g_aScrap[nCntScrap].fVXaim3D / g_aScrap[nCntScrap].fVLaim3D * 15.0f;
					BulletMove.y = g_aScrap[nCntScrap].fVYaim3D / g_aScrap[nCntScrap].fVLaim3D * 15.0f;
					BulletMove.z = g_aScrap[nCntScrap].fVZaim3D / g_aScrap[nCntScrap].fVLaim3D * 15.0f;
				}

				g_aScrap[nCntScrap].move = BulletMove;
				//==================================================================================================================================


				break;
				//=========================================================================================================

				
			}


		

			//�ړ�
			g_aScrap[nCntScrap].pos += g_aScrap[nCntScrap].move;

		}
	

		pVtx += 4;//�|�C���^��i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffScrap->Unlock();

	//================================================================================================================================================================================
}



//================================
//�`�揈��
//================================
void DrawScrap(void)
{

	//=========================================================
	//�X�N���b�v�r���{�[�h����
    //=========================================================
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	for (int nCntScrap = 0; nCntScrap < MAX_SCRAP; nCntScrap++)
	{
		if (g_aScrap[nCntScrap].bUse == true)
		{
			D3DXMATRIX mtxTrans;//�v�Z�p�}�g���b�N�X

			D3DXMATRIX mtxView;//�r���[�}�g���b�N�X�擾�p

			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_mtxWorldScrap);

			//�r���[�}�g���b�N�X���擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//�|���S�����J�����ɑ΂��Đ��ʂɌ�����
			D3DXMatrixInverse(&g_mtxWorldScrap, NULL, &mtxView);

			//�t�s������߂�
			g_mtxWorldScrap._41 = 0.0f;
			g_mtxWorldScrap._42 = 0.0f;
			g_mtxWorldScrap._43 = 0.0f;

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aScrap[nCntScrap].pos.x, g_aScrap[nCntScrap].pos.y, g_aScrap[nCntScrap].pos.z);
			D3DXMatrixMultiply(&g_mtxWorldScrap, &g_mtxWorldScrap, &mtxTrans);

			//=============================
			//�`��̒���
			//=============================
			//���C�g�𖳌��ɂ���
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			

			//�A���t�@�e�X�g��L��
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

			//===============================================================

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldScrap);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffScrap, 0, sizeof(VERTEX_3D));//���_�o�b�t�@�ւ̃|�C���^�ƒ��_���̍\���̂̃T�C�Y

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureScrap[g_aScrap[nCntScrap].nType]);
				
			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntScrap * 4, 2);//0�́A�`�悷��ŏ��̒��_�C���f�b�N�X�ŁA2�́A�`�悷��v���~�e�B�u

			//============================
			//�`��̒��������Ƃɖ߂�
			//============================

			//���C�g��L���ɖ߂�
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);


			//�A���t�@�e�X�g�𖳌��ɖ߂�
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

			//=========================================================
		}
	}
}

//================================
//�ݒ菈��
//================================
void SetScrap(int nType, int nLife, float fRWidth, float fRHeight, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 DiffusionMove)
{

	VERTEX_3D* pVtx;
	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffScrap->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntScrap = 0; nCntScrap < MAX_SCRAP; nCntScrap++)
	{
		if (g_aScrap[nCntScrap].bUse == false)
		{
			g_aScrap[nCntScrap].bUse = true;//�g�p���Ă��邩�ǂ���
			g_aScrap[nCntScrap].pos = pos;//�ʒu
			g_aScrap[nCntScrap].move = DiffusionMove;//�ړ���
			g_aScrap[nCntScrap].ScrapMoveType = SCRAPMOVETYPE_DIFFUSION;//�ŏ��͊g�U������悤�Ɉړ�������

			g_aScrap[nCntScrap].rot = rot;//����
			g_aScrap[nCntScrap].nLife = nLife;//�̗�
			g_aScrap[nCntScrap].fRWidth = fRWidth;//���i���a�j
			g_aScrap[nCntScrap].fRHeight = fRHeight;//�����i���a�j
			g_aScrap[nCntScrap].nType = nType;//���
			g_aScrap[nCntScrap].nAnimationCnt = 0;//�A�j���[�V�����J�E���g
			g_aScrap[nCntScrap].nAnimationPattern = 0;//�A�j���[�V�����p�^�[��
			g_aScrap[nCntScrap].fLength = 0.0f;
			g_aScrap[nCntScrap].fLength = 0.0f;//�����𑪂�B
			g_aScrap[nCntScrap].fXaim = 0.0f;//X�����̃x�N�g��
			g_aScrap[nCntScrap].fYaim = 0.0f;//Y�����̃x�N�g��
			g_aScrap[nCntScrap].fZaim = 0.0f;//Z�����̃x�N�g��
			g_aScrap[nCntScrap].fVXZaim = 0.0f;//X��Z�̑����x�N�g��
			g_aScrap[nCntScrap].fVYZaim = 0.0f;//Y��Z�̑����x�N�g��

			//�X�N���b�v�̒��_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-g_aScrap[nCntScrap].fRWidth, g_aScrap[nCntScrap].fRHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aScrap[nCntScrap].fRWidth, g_aScrap[nCntScrap].fRHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aScrap[nCntScrap].fRWidth, -g_aScrap[nCntScrap].fRHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aScrap[nCntScrap].fRWidth, -g_aScrap[nCntScrap].fRHeight, 0.0f);

			break;
		}
		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffScrap->Unlock();
}

Scrap* GetScrap(void)//�X�N���b�v�\���̂̏����Q�b�g����
{
	return &g_aScrap[0];
}
