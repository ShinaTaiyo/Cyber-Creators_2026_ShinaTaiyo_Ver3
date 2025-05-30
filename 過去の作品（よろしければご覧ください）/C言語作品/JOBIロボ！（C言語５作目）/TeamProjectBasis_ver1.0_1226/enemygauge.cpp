//========================================================
//
//�X���Q�U���F�n�b�J�\���̊�Ս��[EnemyGaugeFrame.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#include "enemy.h"
#include "enemygauge.h"
#include "boss.h"
//=====================================================================================
//�O���[�o���ϐ�
//=====================================================================================
LPDIRECT3DTEXTURE9 g_pTextureEnemyHP = NULL;		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemyHP = NULL;	//���_�o�b�t�@�ւ̃|�C���^
EnemyHP g_aEnemyHP[MAX_ENEMY];	//�\���̂̏��

LPDIRECT3DTEXTURE9 g_pTextureEnemyWeakDisp[NUM_ENEMYWEAKDISP];//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemyWeakDisp = NULL;	//���_�o�b�t�@�ւ̃|�C���^
EnemyWeakDisp g_aEnemyWeakDisp[MAX_ENEMYWEAKDISP];      //�\���̂̏��

//=====================================================================================
// ����������
//=====================================================================================
void InitEnemyGauge(void)
{
	VERTEX_3D* pVtx;							//���_���ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"",
		&g_pTextureEnemyHP);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_ENEMY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEnemyHP,
		NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEnemyHP->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemyHP = 0; nCntEnemyHP < MAX_ENEMY; nCntEnemyHP++)
	{
		//�\���̂̏��
		g_aEnemyHP[nCntEnemyHP].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu
		g_aEnemyHP[nCntEnemyHP].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);	//�F
		g_aEnemyHP[nCntEnemyHP].bUse = false;								//�g�p���

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-ENEMYHP_WIDTH / 2, ENEMYHP_HEIGHT / 2.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(ENEMYHP_WIDTH / 2, ENEMYHP_HEIGHT / 2.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-ENEMYHP_WIDTH / 2, -ENEMYHP_HEIGHT / 2.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(ENEMYHP_WIDTH / 2, -ENEMYHP_HEIGHT / 2.0f, 0.0f);

		//�@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		//���_�J���[�̐ݒ�
		pVtx[0].col = g_aEnemyHP[nCntEnemyHP].col;
		pVtx[1].col = g_aEnemyHP[nCntEnemyHP].col;
		pVtx[2].col = g_aEnemyHP[nCntEnemyHP].col;
		pVtx[3].col = g_aEnemyHP[nCntEnemyHP].col;

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEnemyHP->Unlock();
}


//===================================
//�G��_�\���̏�����
//===================================
void InitEnemyWeakDisp(void)
{
	VERTEX_3D* pVtx;							//���_���ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\WeakDisplay_Sword_000.png",
		&g_pTextureEnemyWeakDisp[ENEMYWEAKDISP00_SWORD]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\WeakDisplay_Boomerang_000.png",
		&g_pTextureEnemyWeakDisp[ENEMYWEAKDISP01_BOOMERANG]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\WeakDisplay_Speaker_000.png",
		&g_pTextureEnemyWeakDisp[ENEMYWEAKDISP02_SPEAKER]);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_ENEMYWEAKDISP,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEnemyWeakDisp,
		NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEnemyWeakDisp->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemyWeakDisp = 0; nCntEnemyWeakDisp < MAX_ENEMYWEAKDISP; nCntEnemyWeakDisp++)
	{
		//�\���̂̏��
		g_aEnemyWeakDisp[nCntEnemyWeakDisp].pos = NULL_VECTOR3;
		g_aEnemyWeakDisp[nCntEnemyWeakDisp].bUse = false;
		g_aEnemyWeakDisp[nCntEnemyWeakDisp].nType = 0;
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-ENEMYWEAKDISP_WIDTH / 2, ENEMYWEAKDISP_HEIGHT / 2.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(ENEMYWEAKDISP_WIDTH / 2, ENEMYWEAKDISP_HEIGHT / 2.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-ENEMYWEAKDISP_WIDTH / 2, -ENEMYWEAKDISP_HEIGHT / 2.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(ENEMYWEAKDISP_WIDTH / 2, -ENEMYWEAKDISP_HEIGHT / 2.0f, 0.0f);

		//�@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEnemyWeakDisp->Unlock();
}

//==============================
// �I������
//==============================
void UninitEnemyGauge(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureEnemyHP != NULL)
	{
		g_pTextureEnemyHP->Release();
		g_pTextureEnemyHP = NULL;
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffEnemyHP != NULL)
	{
		g_pVtxBuffEnemyHP->Release();
		g_pVtxBuffEnemyHP = NULL;
	}
}

void UninitEnemyWeakDisp(void)
{
	//�e�N�X�`���̔j��
	for (int nCntEnemyWeakDisp = 0; nCntEnemyWeakDisp < NUM_ENEMYWEAKDISP; nCntEnemyWeakDisp++)
	{
		if (g_pTextureEnemyWeakDisp[nCntEnemyWeakDisp] != NULL)
		{
			g_pTextureEnemyWeakDisp[nCntEnemyWeakDisp]->Release();
			g_pTextureEnemyWeakDisp[nCntEnemyWeakDisp] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffEnemyWeakDisp != NULL)
	{
		g_pVtxBuffEnemyWeakDisp->Release();
		g_pVtxBuffEnemyWeakDisp = NULL;
	}
}

//============================
// �X�V����
//============================
void UpdateEnemyGauge(void)
{
	VERTEX_3D* pVtx;			//���_���ւ̃|�C���^
	Enemy* pEnemy = GetEnemy();	//�G�̏��̎擾
	Boss* pBoss = GetBoss();    //�{�X�̏����擾

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEnemyHP->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemyHP = 0; nCntEnemyHP < MAX_ENEMY; nCntEnemyHP++, pEnemy++)
	{
		//�g�p����Ă���ꍇ
		if (g_aEnemyHP[nCntEnemyHP].bUse == true && pEnemy->bUse == true)
		{
			//�ʒu���X�V
			g_aEnemyHP[nCntEnemyHP].pos.x = pEnemy->pos.x;
			g_aEnemyHP[nCntEnemyHP].pos.y = pEnemy->pos.y + pEnemy->Size.y;
			g_aEnemyHP[nCntEnemyHP].pos.z = pEnemy->pos.z;

			//�Y������G�̗̑͂̍ő�l���擾
			int nMaxHP = pEnemy->nMaxHp;

			//���݂̓G�̗̑͂��犄�����v�Z
			float fWidth = (ENEMYHP_WIDTH / 2) * ((float)pEnemy->nHp / (float)nMaxHP) * 2.0f;

			//���_���W�̍X�V
			pVtx[0].pos = D3DXVECTOR3(-ENEMYHP_WIDTH / 2.0f, ENEMYHP_HEIGHT / 2.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(fWidth - ENEMYHP_WIDTH / 2.0f, ENEMYHP_HEIGHT / 2.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-ENEMYHP_WIDTH / 2.0f, -ENEMYHP_HEIGHT / 2.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(fWidth - ENEMYHP_WIDTH / 2.0f, -ENEMYHP_HEIGHT / 2.0f, 0.0f);

			//�c��̗͂ɉ����ĐF��ω�
			if (fWidth > ENEMYHP_WIDTH / 2.0f)
			{//51%�`
				g_aEnemyHP[nCntEnemyHP].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);	//�ΐF
			}
			else if (fWidth > ENEMYHP_WIDTH / 4.0f && fWidth <= ENEMYHP_WIDTH / 2.0f)
			{//26%�`50%
				g_aEnemyHP[nCntEnemyHP].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);	//���F
			}
			else
			{//�`25%
				g_aEnemyHP[nCntEnemyHP].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);	//�ԐF
			}

			//���_�J���[�̍X�V
			pVtx[0].col = g_aEnemyHP[nCntEnemyHP].col;
			pVtx[1].col = g_aEnemyHP[nCntEnemyHP].col;
			pVtx[2].col = g_aEnemyHP[nCntEnemyHP].col;
			pVtx[3].col = g_aEnemyHP[nCntEnemyHP].col;
		}

		//�G�̗̑͂�0�̏ꍇ
		if (pEnemy->nHp <= 0 || pBoss->bUse == true)
		{
			g_aEnemyHP[nCntEnemyHP].bUse = false;	//�g�p���Ȃ�
		}

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEnemyHP->Unlock();
}

void UpdateEnemyWeakDisp(void)
{

	VERTEX_3D* pVtx;			//���_���ւ̃|�C���^
	Enemy* pEnemy = GetEnemy();	//�G�̏��̎擾

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEnemyWeakDisp->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemyWeakDisp = 0; nCntEnemyWeakDisp < MAX_ENEMYWEAKDISP; nCntEnemyWeakDisp++, pEnemy++)
	{
		//�g�p����Ă���ꍇ
		if (g_aEnemyWeakDisp[nCntEnemyWeakDisp].bUse == true && pEnemy->bUse == true)
		{
			//�ʒu���X�V
			g_aEnemyWeakDisp[nCntEnemyWeakDisp].pos.x = pEnemy->pos.x;
			g_aEnemyWeakDisp[nCntEnemyWeakDisp].pos.y = pEnemy->pos.y + pEnemy->Size.y + 20.0f;
			g_aEnemyWeakDisp[nCntEnemyWeakDisp].pos.z = pEnemy->pos.z;
		
		}

		if (pEnemy->bUse == false)
		{
			g_aEnemyWeakDisp[nCntEnemyWeakDisp].bUse = false;
		}
		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEnemyWeakDisp->Unlock();
}

//================================
// �`�揈��
//================================
void DrawEnemyGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾
	D3DXMATRIX mtxTrans;						//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;							//�r���[�}�g���b�N�X�擾�p

	for (int nCntEnemyHP = 0; nCntEnemyHP < MAX_ENEMY; nCntEnemyHP++)
	{
		//�g�p���Ă���ꍇ
		if (g_aEnemyHP[nCntEnemyHP].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aEnemyHP[nCntEnemyHP].mtxWorld);

			//�r���[�}�g���b�N�X���擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//�|���S�����J�����ɑ΂��Đ��ʂɌ�����
			D3DXMatrixInverse(&g_aEnemyHP[nCntEnemyHP].mtxWorld, NULL, &mtxView);	//�t�s������߂�
			g_aEnemyHP[nCntEnemyHP].mtxWorld._41 = 0.0f;
			g_aEnemyHP[nCntEnemyHP].mtxWorld._42 = 0.0f;
			g_aEnemyHP[nCntEnemyHP].mtxWorld._43 = 0.0f;

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aEnemyHP[nCntEnemyHP].pos.x, g_aEnemyHP[nCntEnemyHP].pos.y, g_aEnemyHP[nCntEnemyHP].pos.z);
			D3DXMatrixMultiply(&g_aEnemyHP[nCntEnemyHP].mtxWorld, &g_aEnemyHP[nCntEnemyHP].mtxWorld, &mtxTrans);

			//���C�g�𖳌��ɂ���
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			////Z�̔�r���@�ύX�B
			//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

			////Z�o�b�t�@�ɏ������܂Ȃ�
			//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

			//�A���t�@�e�X�g��L��
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);



			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aEnemyHP[nCntEnemyHP].mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffEnemyHP, 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureEnemyHP);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEnemyHP * 4, 2);

			//============================
			//�`��̒��������Ƃɖ߂�
			//============================

			//���C�g��L���ɖ߂�
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

			////Z�̔�r���@�ύX�B
			//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

			////Z�o�b�t�@�ɏ�������
			//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

			//�A���t�@�e�X�g�𖳌��ɖ߂�
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		}
	}
}

void DrawEnemyWeakDisp(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾
	D3DXMATRIX mtxTrans;						//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;							//�r���[�}�g���b�N�X�擾�p

	for (int nCntEnemyWeakDisp = 0; nCntEnemyWeakDisp < MAX_ENEMYWEAKDISP; nCntEnemyWeakDisp++)
	{
		//�g�p���Ă���ꍇ
		if (g_aEnemyWeakDisp[nCntEnemyWeakDisp].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aEnemyWeakDisp[nCntEnemyWeakDisp].mtxWorld);

			//�r���[�}�g���b�N�X���擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//�|���S�����J�����ɑ΂��Đ��ʂɌ�����
			D3DXMatrixInverse(&g_aEnemyWeakDisp[nCntEnemyWeakDisp].mtxWorld, NULL, &mtxView);	//�t�s������߂�
			g_aEnemyWeakDisp[nCntEnemyWeakDisp].mtxWorld._41 = 0.0f;
			g_aEnemyWeakDisp[nCntEnemyWeakDisp].mtxWorld._42 = 0.0f;
			g_aEnemyWeakDisp[nCntEnemyWeakDisp].mtxWorld._43 = 0.0f;

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aEnemyWeakDisp[nCntEnemyWeakDisp].pos.x, g_aEnemyWeakDisp[nCntEnemyWeakDisp].pos.y, g_aEnemyWeakDisp[nCntEnemyWeakDisp].pos.z);
			D3DXMatrixMultiply(&g_aEnemyWeakDisp[nCntEnemyWeakDisp].mtxWorld, &g_aEnemyWeakDisp[nCntEnemyWeakDisp].mtxWorld, &mtxTrans);

			//===============================================================
			//�`��̒���
			//==============================================================
			//���C�g�𖳌��ɂ���
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			////Z�̔�r���@�ύX�B
			//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

			////Z�o�b�t�@�ɏ������܂Ȃ�
			//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

			//�A���t�@�e�X�g��L��
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

			//===============================================================

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aEnemyWeakDisp[nCntEnemyWeakDisp].mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffEnemyWeakDisp, 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureEnemyWeakDisp[g_aEnemyWeakDisp[nCntEnemyWeakDisp].nType]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEnemyWeakDisp * 4, 2);

			//============================
			//�`��̒��������Ƃɖ߂�
			//============================

			 //���C�g��L���ɖ߂�
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

			////Z�̔�r���@�ύX�B
			//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

			////Z�o�b�t�@�ɏ�������
			//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

			//�A���t�@�e�X�g�𖳌��ɖ߂�
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		}
	}
}

//======================================================
// �G�̃Q�[�W���Z�b�g����
//======================================================
void SetEnemyGauge(int nNumEnemy, D3DXVECTOR3 pos)
{
	//�g�p����Ă��Ȃ��ꍇ
	if (g_aEnemyHP[nNumEnemy].bUse == false)
	{
		g_aEnemyHP[nNumEnemy].pos = pos;	//�ʒu
		g_aEnemyHP[nNumEnemy].bUse = true;	//�g�p����
	}
}

//=======================================================
// �G�̎�_�\�L���Z�b�g����
void SetEnemyWeakDisp(int nType, int nNumEnemy, D3DXVECTOR3 pos)
{
	if (g_aEnemyWeakDisp[nNumEnemy].bUse == false)
	{
		g_aEnemyWeakDisp[nNumEnemy].bUse = true;
		g_aEnemyWeakDisp[nNumEnemy].pos = pos;
		g_aEnemyWeakDisp[nNumEnemy].nType = nType;
	}
}

//======================================================
// �\���̂̏��̎擾
//======================================================
EnemyHP* GetEnemyGauge(void)
{
	return &g_aEnemyHP[0];
}

EnemyWeakDisp* GetEnemyWeakDisp(void)
{
	return &g_aEnemyWeakDisp[0];
}
