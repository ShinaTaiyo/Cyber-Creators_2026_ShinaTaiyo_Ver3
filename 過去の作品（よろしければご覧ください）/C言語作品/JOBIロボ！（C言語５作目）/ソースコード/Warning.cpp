//========================================================
//
//�R���P�S���F�{�X�̍U���̎��Ɍx���\�����o������[Warning.cpp]
//Author:ShinaTaiyo
//
//=========================================================

#include "Warning.h"
#include "main.h"
#include "boss.h"

//�O���[�o���ϐ�
PDIRECT3DVERTEXBUFFER9 g_pVtxBuffWarning = NULL;//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureWarning = NULL;//�e�N�X�`���ւ̃|�C���^
Warning g_aWarning[MAX_WARNING];//�\����
int g_nDelayWarning = 0;//�f�B���C

//=================================
//�x���\���̏���������
//=================================
void InitWarning(void)
{
	g_nDelayWarning = 0;
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Warning_000.png",
		&g_pTextureWarning);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_WARNING,//���_�̐�
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffWarning,
		NULL);

	for (int nCntWarning = 0; nCntWarning < MAX_WARNING; nCntWarning++)
	{
		g_aWarning[nCntWarning].bUse = true;//�d�l���
		g_aWarning[nCntWarning].pos = NULL_VECTOR3;//�ʒu
		g_aWarning[nCntWarning].fRWidth = 0.0f;//����
		g_aWarning[nCntWarning].fRHeight = 0.0f;//����
		g_aWarning[nCntWarning].mtxWorld = {};//�}�g���b�N�X���[���h
		g_aWarning[nCntWarning].nLife = 0;//�̗�
		g_aWarning[nCntWarning].Size = NULL_VECTOR3;//�T�C�Y
	
	}

	VERTEX_3D* pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffWarning->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWarning = 0; nCntWarning < MAX_WARNING; nCntWarning++)
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
	g_pVtxBuffWarning->Unlock();
}

//==============================
//�x���\���̏I������
//==============================
void UninitWarning(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureWarning != NULL)
	{
		g_pTextureWarning->Release();
		g_pTextureWarning = NULL;
	}
	
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffWarning != NULL)
	{
		g_pVtxBuffWarning->Release();
		g_pVtxBuffWarning = NULL;
	}
}
//============================
//�x���\���̍X�V����
//============================
void UpdateWarning(void)
{
	Boss* pBoss = GetBoss();

	g_nDelayWarning++;//�f�B���C

	VERTEX_3D* pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffWarning->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWarning = 0; nCntWarning < MAX_WARNING; nCntWarning++)
	{
		if (g_aWarning[nCntWarning].bUse == true)
		{
			g_aWarning[nCntWarning].nLife--;

			//===========================
			//10f���ƂɐF��ς���
			//===========================
			if (g_nDelayWarning % 10 == 0)
			{
				g_aWarning[nCntWarning].bColChange = g_aWarning[nCntWarning].bColChange ? false : true;
			}
			//=========================================================================================================


			if (g_aWarning[nCntWarning].bColChange == false)
			{
				//���_�J���[�̐ݒ�
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}
			else
			{
				//���_�J���[�̐ݒ�
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

			}

			//====================
			//�ʒu�̐ݒ�
			//====================
			g_aWarning[nCntWarning].pos = D3DXVECTOR3(pBoss->pos.x, pBoss->pos.y + pBoss->Size.y + 120.0f, pBoss->pos.z);

			//==================================================================================================================

			if (g_aWarning[nCntWarning].nLife <= 0)
			{
				g_aWarning[nCntWarning].bUse = false;
			}
		}

		pVtx += 4;
	}
	
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffWarning->Unlock();
}



//================================
// �x���\���̕`�揈��
//================================
void DrawWarning(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	D3DXMATRIX mtxTrans;//�v�Z�p�}�g���b�N�X

	D3DXMATRIX mtxView;//�r���[�}�g���b�N�X�擾�p

	for (int nCntWarning = 0; nCntWarning < MAX_WARNING; nCntWarning++)
	{
		if (g_aWarning[nCntWarning].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aWarning[nCntWarning].mtxWorld);

			//�r���[�}�g���b�N�X���擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//�|���S�����J�����ɑ΂��Đ��ʂɌ�����
			D3DXMatrixInverse(&g_aWarning[nCntWarning].mtxWorld, NULL, &mtxView);

			//�t�s������߂�
			g_aWarning[nCntWarning].mtxWorld._41 = 0.0f;
			g_aWarning[nCntWarning].mtxWorld._42 = 0.0f;
			g_aWarning[nCntWarning].mtxWorld._43 = 0.0f;

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aWarning[nCntWarning].pos.x, g_aWarning[nCntWarning].pos.y, g_aWarning[nCntWarning].pos.z);
			D3DXMatrixMultiply(&g_aWarning[nCntWarning].mtxWorld, &g_aWarning[nCntWarning].mtxWorld, &mtxTrans);

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
			pDevice->SetTransform(D3DTS_WORLD, &g_aWarning[nCntWarning].mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0,g_pVtxBuffWarning, 0, sizeof(VERTEX_3D));//���_�o�b�t�@�ւ̃|�C���^�ƒ��_���̍\���̂̃T�C�Y

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0,g_pTextureWarning);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntWarning * 4, 2);//0�́A�`�悷��ŏ��̒��_�C���f�b�N�X�ŁA2�́A�`�悷��v���~�e�B�u

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

//================================
// �ݒ菈��
//================================
void SetWarning(int nLife, float fRWidth, float fRHeight, D3DXVECTOR3 pos)
{
	VERTEX_3D* pVtx;
	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffWarning->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWarning = 0; nCntWarning < MAX_WARNING; nCntWarning++)
	{
		if (g_aWarning[nCntWarning].bUse == false)
		{
			g_aWarning[nCntWarning].bUse = true;//�g�p���
			g_aWarning[nCntWarning].pos = pos;//�ʒu
			g_aWarning[nCntWarning].fRWidth = fRWidth;//����
			g_aWarning[nCntWarning].fRHeight = fRHeight;//����
			g_aWarning[nCntWarning].nLife = nLife;//�̗�

			//���_���W�̐ݒ�i�����ʒu�j
			pVtx[0].pos = D3DXVECTOR3(-fRWidth, fRHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(fRWidth, fRHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-fRWidth, -fRHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(fRWidth, -fRHeight, 0.0f);

			break;
		}
		pVtx += 4;
	}
	

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffWarning->Unlock();
}

//==============================
//�\���̂̎擾
//==============================
Warning* GetWarning(void)
{
	return &g_aWarning[0];
}

