//========================================================
//
// �G�t�F�N�g�̏���
// Author:ShinaTaiyo
//
//=========================================================
#include "effect.h"
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "shadow.h"
#include "attackmodel.h"

//==============================
// �O���[�o���ϐ�
//==============================
PDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureEffect = NULL;//�e�N�X�`���ւ̃|�C���^
D3DXVECTOR3 g_posEffect;//�ʒu
Effect g_aEffect[MAX_EFFECT];//�e�̍\���̏��

//==============================
// ����������
//==============================
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\shadow000.jpg",
		&g_pTextureEffect);

	g_posEffect = D3DXVECTOR3(0.0f, 50.0f, 100.0f);//�ʒu�̏�����

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_EFFECT,//���_�̐�
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�ʒu
		g_aEffect[nCntEffect].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�ړ���
		g_aEffect[nCntEffect].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//����
		g_aEffect[nCntEffect].nLife = 0;//�̗�
		g_aEffect[nCntEffect].fRWidth = 0.0f;//���i���a�j
		g_aEffect[nCntEffect].fRHeight = 0.0f;//�����i���a�j
		g_aEffect[nCntEffect].bUse = false;//�g�p���Ă��邩�ǂ���
		g_aEffect[nCntEffect].nIdxShadow = -1;//�e�̃C���f�b�N�X
		g_aEffect[nCntEffect].fWidth = 0.0f;//����
		g_aEffect[nCntEffect].fHeight = 0.0f;//����
		g_aEffect[nCntEffect].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);//�F
		g_aEffect[nCntEffect].fRedution = 0.0f;//�G�t�F�N�g������������X�s�[�h
		g_aEffect[nCntEffect].fRotMove = 0.0f;//���������̐��l�����߂�
		g_aEffect[nCntEffect].nMaxLife = 0;//�̗͍ő�l
		g_aEffect[nCntEffect].nNumAttackModel = 0;//�G�t�F�N�g��ǂ񂾍U�����f���̔ԍ�
	}

	VERTEX_3D* pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		//���_���W�̐ݒ�i�����ʒu�j
		pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fWidth, g_aEffect[nCntEffect].fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fWidth, g_aEffect[nCntEffect].fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fWidth, -g_aEffect[nCntEffect].fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fWidth, -g_aEffect[nCntEffect].fHeight, 0.0f);

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
	g_pVtxBuffEffect->Unlock();
}

//==============================
// �I������
//==============================
void UninitEffect(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureEffect != NULL)
	{
		g_pTextureEffect->Release();
		g_pTextureEffect = NULL;
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}

//============================
// �X�V����
//============================
void UpdateEffect(void)
{
	VERTEX_3D* pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse)
		{
			g_aEffect[nCntEffect].col.a = (float)(g_aEffect[nCntEffect].nLife) / (float)(g_aEffect[nCntEffect].nMaxLife);
			g_aEffect[nCntEffect].nLife--;

			float fdiameter = g_aEffect[nCntEffect].fWidth / g_aEffect[nCntEffect].fHeight;

			g_aEffect[nCntEffect].fWidth -= g_aEffect[nCntEffect].fRedution * fdiameter;
			g_aEffect[nCntEffect].fHeight -= g_aEffect[nCntEffect].fRedution;
			g_aEffect[nCntEffect].pos += g_aEffect[nCntEffect].move;

			//���_���W�̐ݒ�i�����ʒu�j
			pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fWidth, g_aEffect[nCntEffect].fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fWidth, g_aEffect[nCntEffect].fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fWidth, -g_aEffect[nCntEffect].fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fWidth, -g_aEffect[nCntEffect].fHeight, 0.0f);

			//nor�̐ݒ�i�����ݒ�j
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

			//���_�J���[�̐ݒ�
			pVtx[0].col = g_aEffect[nCntEffect].col;//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
			pVtx[1].col = g_aEffect[nCntEffect].col;//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
			pVtx[2].col = g_aEffect[nCntEffect].col;//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
			pVtx[3].col = g_aEffect[nCntEffect].col;//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B

			if (g_aEffect[nCntEffect].fWidth <= 0.0f || 
				g_aEffect[nCntEffect].fHeight <= 0.0f || 
				g_aEffect[nCntEffect].col.a <= 0.0f || 
				g_aEffect[nCntEffect].nLife <= 0)
			{
				g_aEffect[nCntEffect].bUse = false;//�̗͂��O�ɂȂ�����false�ɂ���
			}
		}

		pVtx += 4;//�|�C���^��i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEffect->Unlock();
}

//================================
// �`�揈��
//================================
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	D3DXMATRIX mtxTrans;//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;//�r���[�}�g���b�N�X�擾�p

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aEffect[nCntEffect].mtxWorldEffect);

			//�r���[�}�g���b�N�X���擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//�|���S�����J�����ɑ΂��Đ��ʂɌ�����
			D3DXMatrixInverse(&g_aEffect[nCntEffect].mtxWorldEffect, NULL, &mtxView);

			//�t�s������߂�
			g_aEffect[nCntEffect].mtxWorldEffect._41 = 0.0f;
			g_aEffect[nCntEffect].mtxWorldEffect._42 = 0.0f;
			g_aEffect[nCntEffect].mtxWorldEffect._43 = 0.0f;

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aEffect[nCntEffect].pos.x, g_aEffect[nCntEffect].pos.y, g_aEffect[nCntEffect].pos.z);
			D3DXMatrixMultiply(&g_aEffect[nCntEffect].mtxWorldEffect, &g_aEffect[nCntEffect].mtxWorldEffect, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aEffect[nCntEffect].mtxWorldEffect);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_3D));//���_�o�b�t�@�ւ̃|�C���^�ƒ��_���̍\���̂̃T�C�Y

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//���C�g�𖳌��ɂ���
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			////Z�̔�r���@�ύX�B
			//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

			//Z�o�b�t�@�ɏ������܂Ȃ�
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

			//�A���t�@�e�X�g��L��(�A���t�@�l���O���傫���ꍇ�ɕ`�悷��j
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

			//a�u�����f�B���O�����Z�����ɐݒ�
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);


			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureEffect);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * 4, 2);//0�́A�`�悷��ŏ��̒��_�C���f�b�N�X�ŁA2�́A�`�悷��v���~�e�B�u

			//���C�g��L���ɖ߂�
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

			////Z�̔�r���@�ύX�B
			//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

			//Z�o�b�t�@�ɏ�������
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

			//�A���t�@�e�X�g�����ɖ߂�
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

			//a�u�����f�B���O�����ɖ߂�
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		}
	}
}

//================================
// �ݒ菈��
//================================
void SetEffect(int nType, int nLife, float fWidth, float fHeight, float fRedution, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRotMove, int nNumModel)
{
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (!g_aEffect[nCntEffect].bUse)
		{
			g_aEffect[nCntEffect].bUse = true;//�g�p����
			g_aEffect[nCntEffect].pos = pos;//�ʒu
			g_aEffect[nCntEffect].move = move;//�ړ���
			g_aEffect[nCntEffect].col = col;//�F
			g_aEffect[nCntEffect].fRotMove = fRotMove;//���������̐��l�����߂�
			g_aEffect[nCntEffect].nType = nType;//���
			g_aEffect[nCntEffect].nLife = nLife;//�̗�
			g_aEffect[nCntEffect].nMaxLife = nLife;//�̗͍ő�l
			g_aEffect[nCntEffect].nNumAttackModel = nNumModel;//�G�t�F�N�g��ǂ񂾍U�����f���̔ԍ�
			g_aEffect[nCntEffect].fWidth = fWidth;//����
			g_aEffect[nCntEffect].fHeight = fHeight;//����
			g_aEffect[nCntEffect].fRedution = fRedution;//�G�t�F�N�g������������X�s�[�h

			break;
		}
	}
}
