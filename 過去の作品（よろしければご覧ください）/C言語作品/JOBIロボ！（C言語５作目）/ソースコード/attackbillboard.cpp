//========================================================
//
//�X���Q�U���F�n�b�J�\���̊�Ս��[Attackbillboard.cpp]
//Author:ShinaTaiyo
//
//=========================================================

#include "Attackbillboard.h"
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "shadow.h"
#include "effect.h"
#include "wall.h"
#include "particle.h"
#include "player.h"
#include "enemy.h"
#include "Attackbillboard.h"
#include "battery.h"
#include "attackmodel.h"
#define ATTACKBILLBOARD_SCALE (10.0f)
//�O���[�o���ϐ�
PDIRECT3DVERTEXBUFFER9 g_pVtxBuffAttackbillboard = NULL;//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureAttackbillboard[NUM_ATTACKBILLBOARD] = {};//�e�N�X�`���ւ̃|�C���^
D3DXVECTOR3 g_posAttackbillboard;//�ʒu
//D3DXVECTOR3 g_rotAttackbillboard;//����
D3DXMATRIX g_mtxWorldAttackbillboard;//���[���h�}�g���b�N�X(float4*4)

Attackbillboard g_aAttackbillboard[MAX_ATTACKBILLBOARD];//�e�̍\���̏��

int g_nDelayAttackbillboard = 0;

//�^�C�g����ʂ̏���������
void InitAttackbillboard(void)
{
	g_nDelayAttackbillboard = 0;

	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Attack\\piercing.png",
		&g_pTextureAttackbillboard[0]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Attack\\Thunder.png",
		&g_pTextureAttackbillboard[1]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Attack\\Fire.png",
		&g_pTextureAttackbillboard[2]);


	g_posAttackbillboard = D3DXVECTOR3(0.0f, 50.0f, 100.0f);//�ʒu�̏�����

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_ATTACKBILLBOARD,//���_�̐�
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffAttackbillboard,
		NULL);

	for (int nCntAttackbillboard = 0; nCntAttackbillboard < MAX_ATTACKBILLBOARD; nCntAttackbillboard++)
	{
		g_aAttackbillboard[nCntAttackbillboard].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�ʒu
		g_aAttackbillboard[nCntAttackbillboard].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�ړ���
		g_aAttackbillboard[nCntAttackbillboard].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//����
		g_aAttackbillboard[nCntAttackbillboard].Size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�傫��
		g_aAttackbillboard[nCntAttackbillboard].nLife = 0;//�̗�
		g_aAttackbillboard[nCntAttackbillboard].fRWidth = 0.0f;//���i���a�j
		g_aAttackbillboard[nCntAttackbillboard].fRHeight = 0.0f;//�����i���a�j
		g_aAttackbillboard[nCntAttackbillboard].nType = 0;//���
		g_aAttackbillboard[nCntAttackbillboard].bUse = false;//�g�p���Ă��邩�ǂ���
		g_aAttackbillboard[nCntAttackbillboard].nNumEnemy = 0;//���Ԃ̓G�������Ă��Ă��邩
		g_aAttackbillboard[nCntAttackbillboard].fLength = 0.0f;//����
		g_aAttackbillboard[nCntAttackbillboard].bOneFlag = false;
		g_aAttackbillboard[nCntAttackbillboard].bChangeFlag = false;//�l���㉺�����鎞�Ɏg��
		g_aAttackbillboard[nCntAttackbillboard].nMaxLife = 0;//�ő�̗�

		g_aAttackbillboard[nCntAttackbillboard].nAnimationCnt = 0;//�A�j���[�V�����J�E���g
		g_aAttackbillboard[nCntAttackbillboard].nAnimationPattern = 0;//�A�j���[�V�����p�^�[��
		g_aAttackbillboard[nCntAttackbillboard].nAnimationPatternH = 0;//�c�̃A�j���[�V�����p�^�[��
		g_aAttackbillboard[nCntAttackbillboard].nIdxShadow = -1;//�e�̃C���f�b�N�X
	}

	VERTEX_3D* pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffAttackbillboard->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntAttackbillboard = 0; nCntAttackbillboard < MAX_ATTACKBILLBOARD; nCntAttackbillboard++)
	{
		//���_���W�̐ݒ�i�����ʒu�j
		pVtx[0].pos = NULL_VECTOR3;
		pVtx[1].pos = NULL_VECTOR3;
		pVtx[2].pos = NULL_VECTOR3;
		pVtx[3].pos = NULL_VECTOR3;

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
	g_pVtxBuffAttackbillboard->Unlock();
}

//==============================
//�^�C�g����ʂ̏I������
//==============================
void UninitAttackbillboard(void)
{
	//�e�N�X�`���̔j��
	for (int nCntAttackbillboard = 0; nCntAttackbillboard < NUM_ATTACKBILLBOARD; nCntAttackbillboard++)
	{
		if (g_pTextureAttackbillboard[nCntAttackbillboard] != NULL)
		{
			g_pTextureAttackbillboard[nCntAttackbillboard]->Release();
			g_pTextureAttackbillboard[nCntAttackbillboard] = NULL;
		}
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffAttackbillboard != NULL)
	{
		g_pVtxBuffAttackbillboard->Release();
		g_pVtxBuffAttackbillboard = NULL;
	}
}
//============================
//�^�C�g����ʂ̍X�V����
//============================
void UpdateAttackbillboard(void)
{
	VERTEX_3D* pVtx;
	Player* pPlayer = GetPlayer();

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffAttackbillboard->Lock(0, 0, (void**)&pVtx, 0);

	g_nDelayAttackbillboard++;

	for (int nCntAttackbillboard = 0; nCntAttackbillboard < MAX_ATTACKBILLBOARD; nCntAttackbillboard++)
	{
		if (g_aAttackbillboard[nCntAttackbillboard].bUse == true)
		{
			g_aAttackbillboard[nCntAttackbillboard].nAnimationCnt++;

		    //=========================================
		    // �G�ƃv���C���[�̋��������B
		    //=========================================
		    float fLength = sqrtf((g_aAttackbillboard[nCntAttackbillboard].pos.x - pPlayer->pos.x) * (g_aAttackbillboard[nCntAttackbillboard].pos.x - pPlayer->pos.x) +
				(g_aAttackbillboard[nCntAttackbillboard].pos.z - pPlayer->pos.z) * (g_aAttackbillboard[nCntAttackbillboard].pos.z - pPlayer->pos.z)) / 2;

			g_aAttackbillboard[nCntAttackbillboard].fVXaim = pPlayer->pos.x - g_aAttackbillboard[nCntAttackbillboard].pos.x;
			g_aAttackbillboard[nCntAttackbillboard].fVYaim = pPlayer->pos.y - g_aAttackbillboard[nCntAttackbillboard].pos.y;
			g_aAttackbillboard[nCntAttackbillboard].fVZaim = pPlayer->pos.z - g_aAttackbillboard[nCntAttackbillboard].pos.z;

			//=========================================
			//�ėp�I�Ɏg�������l
			//=========================================
			float fPurposeRandRot = float(rand() % 200 + 1) / 100.0f;
			float fPurposeRandSpeed = float(rand() % 20 + 1);

			switch (g_aAttackbillboard[nCntAttackbillboard].nType)
			{
			case ATTACKBILLBOARD00_STUB:
				//==========================
				//�A�j���[�V��������
				//==========================
				if (g_aAttackbillboard[nCntAttackbillboard].nAnimationCnt % 5 == 0)
				{
					g_aAttackbillboard[nCntAttackbillboard].nAnimationPattern++;

					if (g_aAttackbillboard[nCntAttackbillboard].nAnimationPattern == 4)
					{
						g_aAttackbillboard[nCntAttackbillboard].nAnimationPattern = 0;
					}
				}

				//�e�N�X�`�����W�̐ݒ�i�����ݒ�j
				pVtx[0].tex = D3DXVECTOR2(0.0f + 0.25f * g_aAttackbillboard[nCntAttackbillboard].nAnimationPattern,0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.25f + 0.25f * g_aAttackbillboard[nCntAttackbillboard].nAnimationPattern,0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f + 0.25f * g_aAttackbillboard[nCntAttackbillboard].nAnimationPattern,1.0f );
				pVtx[3].tex = D3DXVECTOR2(0.25f + 0.25f * g_aAttackbillboard[nCntAttackbillboard].nAnimationPattern,1.0f);

				if (g_aAttackbillboard[nCntAttackbillboard].pos.y + g_aAttackbillboard[nCntAttackbillboard].fRHeight >= pPlayer->pos.y &&
					g_aAttackbillboard[nCntAttackbillboard].pos.y - g_aAttackbillboard[nCntAttackbillboard].fRHeight <= pPlayer->pos.y + pPlayer->Size.y &&
					fLength < g_aAttackbillboard[nCntAttackbillboard].fRWidth)
				{
					for (int nCntParticle = 0; nCntParticle < 10; nCntParticle++)
					{
						float fRandRot = float(rand() % 200 + 1) / 10;
						float fRandSpeed = float(rand() % 20 + 1);
						float fRandMove = float(rand() % 40 - 20) / 2;

						SetParticle(PARTICLETYPE_NORMAL, 200, 10.0f, 0.4f,pPlayer->pos,
							D3DXVECTOR3(sinf(-D3DX_PI * fRandRot) * fRandSpeed, fRandMove, cosf(-D3DX_PI * fRandRot) * fRandSpeed),
							D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 0.0f, PARTICLETEX_NORMAL,0);
					}

					AttackHitPlayer(30);
				}


				break;

			case ATTACKBILLBORAD02_FIRE:

				if (g_nDelayAttackbillboard % 10 == 0)
				{
					SetParticle(PARTICLETYPE_FIRE, 60, 5.0f, 0.05f, g_aAttackbillboard[nCntAttackbillboard].pos,
						D3DXVECTOR3(sinf(D3DX_PI * fPurposeRandRot) * fPurposeRandSpeed, fPurposeRandSpeed, cosf(D3DX_PI * fPurposeRandRot) * fPurposeRandSpeed),
						D3DXCOLOR(1.0f, 0.647f, 0.0f, 1.0f), 0.0f, PARTICLETEX_NORMAL,0);
				}
				//==========================
				//�A�j���[�V��������
				//==========================
				if (g_aAttackbillboard[nCntAttackbillboard].nAnimationCnt % 4 == 0)
				{
					g_aAttackbillboard[nCntAttackbillboard].nAnimationPattern++;

					if (g_aAttackbillboard[nCntAttackbillboard].nAnimationPattern == 8)
					{
						g_aAttackbillboard[nCntAttackbillboard].nAnimationPattern = 0;
						g_aAttackbillboard[nCntAttackbillboard].nAnimationPatternH++;

						if (g_aAttackbillboard[nCntAttackbillboard].nAnimationPatternH == 2)
						{
							g_aAttackbillboard[nCntAttackbillboard].nAnimationPatternH = 0;
						}
					}
				}

				//�e�N�X�`�����W�̐ݒ�i�����ݒ�j
				pVtx[0].tex = D3DXVECTOR2(0.0f + 0.125f * g_aAttackbillboard[nCntAttackbillboard].nAnimationPattern, 0.0f + 0.5f * g_aAttackbillboard[nCntAttackbillboard].nAnimationPatternH);
				pVtx[1].tex = D3DXVECTOR2(0.125f + 0.125f * g_aAttackbillboard[nCntAttackbillboard].nAnimationPattern, 0.0f + 0.5f * g_aAttackbillboard[nCntAttackbillboard].nAnimationPatternH);
				pVtx[2].tex = D3DXVECTOR2(0.0f + 0.125f * g_aAttackbillboard[nCntAttackbillboard].nAnimationPattern, 0.5f + 0.5f * g_aAttackbillboard[nCntAttackbillboard].nAnimationPatternH);
				pVtx[3].tex = D3DXVECTOR2(0.125f + 0.125f * g_aAttackbillboard[nCntAttackbillboard].nAnimationPattern, 0.5f + 0.5f * g_aAttackbillboard[nCntAttackbillboard].nAnimationPatternH);

				if (g_aAttackbillboard[nCntAttackbillboard].pos.y + g_aAttackbillboard[nCntAttackbillboard].fRHeight >= pPlayer->pos.y &&
					g_aAttackbillboard[nCntAttackbillboard].pos.y - g_aAttackbillboard[nCntAttackbillboard].fRHeight <= pPlayer->pos.y + pPlayer->Size.y &&
					fLength < g_aAttackbillboard[nCntAttackbillboard].fRWidth / 2.0f)
				{
					for (int nCntParticle = 0; nCntParticle < 10; nCntParticle++)
					{
						float fRandRot = float(rand() % 200 + 1) / 10;
						float fRandSpeed = float(rand() % 20 + 1);
						float fRandMove = float(rand() % 40 - 20) / 2;

						SetParticle(PARTICLETYPE_NORMAL, 60, 6.0f, 0.4f, pPlayer->pos,
							D3DXVECTOR3(sinf(-D3DX_PI * fRandRot) * fRandSpeed, fRandMove, cosf(-D3DX_PI * fRandRot) * fRandSpeed),
							D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 0.0f, PARTICLETEX_NORMAL,0);
					}
					AttackHitPlayer(25);
				}

				break;
			}

			g_aAttackbillboard[nCntAttackbillboard].nLife -= 1;

			g_aAttackbillboard[nCntAttackbillboard].pos += g_aAttackbillboard[nCntAttackbillboard].move;

			if (g_aAttackbillboard[nCntAttackbillboard].nLife <= 0)
			{
				g_aAttackbillboard[nCntAttackbillboard].bUse = false;//�̗͂��O�ɂȂ�����false�ɂ���
			}

			SetPositionShadow(g_aAttackbillboard[nCntAttackbillboard].nIdxShadow, D3DXVECTOR3(g_aAttackbillboard[nCntAttackbillboard].pos.x, g_aAttackbillboard[nCntAttackbillboard].pos.y - g_aAttackbillboard[nCntAttackbillboard].fRHeight, g_aAttackbillboard[nCntAttackbillboard].pos.z));

			if (g_aAttackbillboard[nCntAttackbillboard].bUse == false)
			{
				KillShadow(g_aAttackbillboard[nCntAttackbillboard].nIdxShadow);
			}
		}

		pVtx += 4;//�|�C���^��i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffAttackbillboard->Unlock();
}



//================================
// �`�揈��
//================================
void DrawAttackbillboard(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	for (int nCntAttackbillboard = 0; nCntAttackbillboard < MAX_ATTACKBILLBOARD; nCntAttackbillboard++)
	{
		if (g_aAttackbillboard[nCntAttackbillboard].bUse == true)
		{
			if (g_aAttackbillboard[nCntAttackbillboard].pos.y >= 0.0f)
			{
				D3DXMATRIX mtxTrans;//�v�Z�p�}�g���b�N�X

				D3DXMATRIX mtxView;//�r���[�}�g���b�N�X�擾�p

				//���[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&g_mtxWorldAttackbillboard);

				//�r���[�}�g���b�N�X���擾
				pDevice->GetTransform(D3DTS_VIEW, &mtxView);

				//�|���S�����J�����ɑ΂��Đ��ʂɌ�����
				D3DXMatrixInverse(&g_mtxWorldAttackbillboard, NULL, &mtxView);

				//�t�s������߂�
				g_mtxWorldAttackbillboard._41 = 0.0f;
				g_mtxWorldAttackbillboard._42 = 0.0f;
				g_mtxWorldAttackbillboard._43 = 0.0f;

				//�ʒu�𔽉f
				D3DXMatrixTranslation(&mtxTrans, g_aAttackbillboard[nCntAttackbillboard].pos.x, g_aAttackbillboard[nCntAttackbillboard].pos.y, g_aAttackbillboard[nCntAttackbillboard].pos.z);
				D3DXMatrixMultiply(&g_mtxWorldAttackbillboard, &g_mtxWorldAttackbillboard, &mtxTrans);

				//===============================================================
				//�`��̒���
				//==============================================================
				//���C�g�𖳌��ɂ���
				pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

				//�A���t�@�e�X�g��L��
				pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
				pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
				pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

				//���[���h�}�g���b�N�X�̐ݒ�
				pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldAttackbillboard);

				//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
				pDevice->SetStreamSource(0, g_pVtxBuffAttackbillboard, 0, sizeof(VERTEX_3D));//���_�o�b�t�@�ւ̃|�C���^�ƒ��_���̍\���̂̃T�C�Y

				//���_�t�H�[�}�b�g�̐ݒ�
				pDevice->SetFVF(FVF_VERTEX_3D);

				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_pTextureAttackbillboard[g_aAttackbillboard[nCntAttackbillboard].nType]);

				//�|���S���̕`��
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntAttackbillboard * 4, 2);//0�́A�`�悷��ŏ��̒��_�C���f�b�N�X�ŁA2�́A�`�悷��v���~�e�B�u

				//============================
				//�`��̒��������Ƃɖ߂�
				//============================

				//���C�g��L���ɖ߂�
				pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

				//�A���t�@�e�X�g�𖳌��ɖ߂�
				pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
			}
		}
	}
}

//================================
// �ݒ菈��
//================================
void SetAttackbillboard(int nType, int nLife, float fRWidth, float fRHeight, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, int nNumEnemy, float fLength)
{
	VERTEX_3D* pVtx;
	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffAttackbillboard->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntAttackbillboard = 0; nCntAttackbillboard < MAX_ATTACKBILLBOARD; nCntAttackbillboard++)
	{
		if (g_aAttackbillboard[nCntAttackbillboard].bUse == false)
		{
			g_aAttackbillboard[nCntAttackbillboard].bUse = true;//�g�p���Ă��邩�ǂ���
			g_aAttackbillboard[nCntAttackbillboard].pos = pos;//�ʒu
			g_aAttackbillboard[nCntAttackbillboard].move = move;//�ړ���
			g_aAttackbillboard[nCntAttackbillboard].rot = rot;//����
			g_aAttackbillboard[nCntAttackbillboard].nLife = nLife;//�̗�
			g_aAttackbillboard[nCntAttackbillboard].fRWidth = fRWidth;//���i���a�j
			g_aAttackbillboard[nCntAttackbillboard].fRHeight = fRHeight;//�����i���a�j
			g_aAttackbillboard[nCntAttackbillboard].nType = nType;//���
			g_aAttackbillboard[nCntAttackbillboard].nNumEnemy = nNumEnemy;
			g_aAttackbillboard[nCntAttackbillboard].fLength = fLength;
			g_aAttackbillboard[nCntAttackbillboard].bOneFlag = true;//��x�������������鏈���p�t���O
			g_aAttackbillboard[nCntAttackbillboard].bChangeFlag = false;//�l���㉺�����鎞�Ɏg��
			g_aAttackbillboard[nCntAttackbillboard].nIdxShadow = SetShadow();
			g_aAttackbillboard[nCntAttackbillboard].nMaxLife = nLife;//�ő�̗�

			g_aAttackbillboard[nCntAttackbillboard].nAnimationCnt = 0;//�A�j���[�V�����J�E���g
			g_aAttackbillboard[nCntAttackbillboard].nAnimationPattern = 0;//�A�j���[�V�����p�^�[��
			g_aAttackbillboard[nCntAttackbillboard].nAnimationPatternH = 0;//�c�̃A�j���[�V�����p�^�[��

			//���_���W�̐ݒ�i�����ʒu�j
			pVtx[0].pos = D3DXVECTOR3(-g_aAttackbillboard[nCntAttackbillboard].fRWidth, g_aAttackbillboard[nCntAttackbillboard].fRHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aAttackbillboard[nCntAttackbillboard].fRWidth, g_aAttackbillboard[nCntAttackbillboard].fRHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aAttackbillboard[nCntAttackbillboard].fRWidth, -g_aAttackbillboard[nCntAttackbillboard].fRHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aAttackbillboard[nCntAttackbillboard].fRWidth, -g_aAttackbillboard[nCntAttackbillboard].fRHeight, 0.0f);

			g_aAttackbillboard[nCntAttackbillboard].Size = D3DXVECTOR3(g_aAttackbillboard[nCntAttackbillboard].fRWidth * 2, 0.0f, g_aAttackbillboard[nCntAttackbillboard].fRWidth * 2);//�傫���i�T�C�Y = ���a �~ 2�j 


			break;
		}
		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffAttackbillboard->Unlock();
}

Attackbillboard* GetAttackbillboard(void)//�e�\���̂̏����Q�b�g����
{
	return &g_aAttackbillboard[0];
}

