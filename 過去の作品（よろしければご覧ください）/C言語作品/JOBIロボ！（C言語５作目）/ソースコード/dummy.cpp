//================================================================================================================================================================================================================================
//
// �_�~�[�l�`�̏���
// Author : Atsumu Kuboichi
//
//================================================================================================================================================================================================================================
#include "dummy.h"
#include "player.h"
#include "particle.h"
#include "sound.h"
//================================================================================================================================================================================================================================
// �O���[�o���ϐ�
//================================================================================================================================================================================================================================
Dummy g_aDummy;	//�\���̏��

//================================================================================================================================================================================================================================
// ����������
//================================================================================================================================================================================================================================
void InitDummy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̎擾
	BYTE* pVtxBuff;							//���_�o�b�t�@�ւ̃|�C���^

	//�\���̂̏��
	g_aDummy.pos = D3DXVECTOR3(-120.0f, 0.0f, 120.0f);	//�ʒu
	g_aDummy.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����
	g_aDummy.nLife = DUMMY_LIFE;						//�̗�
	g_aDummy.nRespawn = (int)(60.0f * RESPAWN_TIME);	//���X�|�[������
	g_aDummy.bUse = true;								//�g�p���
	g_aDummy.nRespawnTime = 0;                          //���X�|�[�����Ԃ��J�E���g����

	for (int nCntAttack = 0; nCntAttack < PLAYERATTACK_MAX; nCntAttack++)
	{
		g_aDummy.bHitStop[nCntAttack] = false;
		g_aDummy.nHitStopTime[nCntAttack] = 0;
	}

	//X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX("data\\MODEL\\OBJECT\\dummy000\\dummy000.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&g_aDummy.pBuffMat, NULL, &g_aDummy.dwNumMat, &g_aDummy.pMesh);

	//���_���̎擾
	int nNumVtx = g_aDummy.pMesh->GetNumVertices();

	//���_�t�H�[�}�b�g�̃T�C�Y���擾
	DWORD sizeFVF = D3DXGetFVFVertexSize(g_aDummy.pMesh->GetFVF());

	//���_�o�b�t�@�̃��b�N
	g_aDummy.pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		//���_���W�̑��
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		//���f���̍ŏ��l�ƍő�l���擾
		if (vtx.x < g_aDummy.vtxMin.x)
		{//X���W�̍ŏ��l
			g_aDummy.vtxMin.x = vtx.x;
		}
		if (vtx.y < g_aDummy.vtxMin.y)
		{//Y���W�̍ŏ��l
			g_aDummy.vtxMin.y = vtx.y;
		}
		if (vtx.z < g_aDummy.vtxMin.z)
		{//Z���W�̍ŏ��l
			g_aDummy.vtxMin.z = vtx.z;
		}
		if (vtx.x > g_aDummy.vtxMax.x)
		{//X���W�̍ő�l
			g_aDummy.vtxMax.x = vtx.x;
		}
		if (vtx.y > g_aDummy.vtxMax.y)
		{//Y���W�̍ő�l
			g_aDummy.vtxMax.y = vtx.y;
		}
		if (vtx.z > g_aDummy.vtxMax.z)
		{//Z���W�̍ő�l
			g_aDummy.vtxMax.z = vtx.z;
		}

		//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
		pVtxBuff += sizeFVF;
	}

	g_aDummy.Size = g_aDummy.vtxMax - g_aDummy.vtxMin;

	//���_�o�b�t�@�̃A�����b�N
	g_aDummy.pMesh->UnlockVertexBuffer();
}

//================================================================================================================================================================================================================================
// �I������
//================================================================================================================================================================================================================================
void UninitDummy(void)
{
	//���b�V���̔j��
	if (g_aDummy.pMesh != NULL)
	{	
		g_aDummy.pMesh->Release();
		g_aDummy.pMesh = NULL;
	}
	//�}�e���A���̔j��
	if (g_aDummy.pBuffMat != NULL)
	{
		g_aDummy.pBuffMat->Release();
		g_aDummy.pBuffMat = NULL;
	}
}

//================================================================================================================================================================================================================================
// �X�V����
//================================================================================================================================================================================================================================
void UpdateDummy(void)
{
	if (g_aDummy.bUse == true)
	{
		HitStopDummy();
	}

	if (g_aDummy.bUse == false)
	{
		g_aDummy.nRespawnTime++;

		if (g_aDummy.nRespawnTime >= g_aDummy.nRespawn)
		{
			g_aDummy.bUse = true;
			g_aDummy.nRespawnTime = 0;
			g_aDummy.nLife = DUMMY_LIFE;
		}
	}
}

//================================================================================================================================================================================================================================
// �`�揈��
//================================================================================================================================================================================================================================
void DrawDummy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;			//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;					//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL* pMat;						//�}�e���A���̃f�[�^�ւ̃|�C���^

	//�g�p����Ă���ꍇ
	if (g_aDummy.bUse == true)
	{
		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aDummy.mtxWorld);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aDummy.rot.y, g_aDummy.rot.x, g_aDummy.rot.z);
		D3DXMatrixMultiply(&g_aDummy.mtxWorld, &g_aDummy.mtxWorld, &mtxRot);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_aDummy.pos.x, g_aDummy.pos.y, g_aDummy.pos.z);
		D3DXMatrixMultiply(&g_aDummy.mtxWorld, &g_aDummy.mtxWorld, &mtxTrans);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_aDummy.mtxWorld);

		//���݂̃}�e���A�����擾
		pDevice->GetMaterial(&matDef);

		//�}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)g_aDummy.pBuffMat->GetBufferPointer();

		for (int nCntDummy2 = 0; nCntDummy2 < (int)g_aDummy.dwNumMat; nCntDummy2++)
		{
			//�}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[nCntDummy2].MatD3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, NULL);

			//���f��(�p�[�c)�̕`��
			g_aDummy.pMesh->DrawSubset(nCntDummy2);
		}

		//�ۑ����Ă����}�e���A����߂�
		pDevice->SetMaterial(&matDef);
	}
}

//===================================
//�\���̂̏����擾
//===================================
Dummy* GetDummy(void)
{
	return &g_aDummy;
}


//===================================
//�_�~�[�Ƀ_���[�W��^����
//===================================
void DamageDummy(int nDamage, PLAYERATTACK PlayerAttack)
{
	if (g_aDummy.bHitStop[PlayerAttack] == false)//�q�b�g�X�g�b�v��Ԃ���Ȃ����
	{
		g_aDummy.nLife -= nDamage;//�̗͂����炷
		g_aDummy.bHitStop[PlayerAttack] = true;  //�_���[�W��^�����U���ɑ΂��ăq�b�g�X�g�b�v��Ԃɂ���

		for (int nCntParticle = 0; nCntParticle < 100; nCntParticle++)
		{
			float fRot = float(rand() % 200 + 1) / 100;
			float fYmove = float(rand() % 200 - 100) / 10;
			float fSpeed = float(rand() % 200 - 100) / 10;

			SetParticle(PARTICLETYPE_NORMAL, 300, 10.0f, 0.6f, g_aDummy.pos,
				D3DXVECTOR3(sinf(-D3DX_PI * fRot) * fSpeed, fYmove, cosf(-D3DX_PI * fRot) * fSpeed),
				D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 0.0f, PARTICLETEX_NORMAL,0);
		}

		//========================
		//���ʉ�����
		//========================
		if (PlayerAttack == PLAYERATTACK00_SWORD)
		{
			PlaySound(SOUND_LABEL_SE_SWORDSLASH_000);
		}
		if (PlayerAttack == PLAYERATTACK01_BOOMERANG)
		{
			PlaySound(SOUND_LABEL_SE_HITBOOMERANG_000);
		}
		if (PlayerAttack == PLAYERATTACK02_SPEAKER)
		{
			PlaySound(SOUND_LABEL_SE_SPEAKERWAVEHIT_000);
		}

		if (g_aDummy.nLife <= 0)
		{
			g_aDummy.bUse = false;
		}
	}
}

//================================
//�q�b�g�X�g�b�v���s��
//================================
void HitStopDummy(void)
{
	if (g_aDummy.bUse == true)
	{
		for (int nCntAttack = 0; nCntAttack < PLAYERATTACK_MAX; nCntAttack++)
		{
			if (g_aDummy.bHitStop[nCntAttack] == true)
			{
				g_aDummy.nHitStopTime[nCntAttack]++;
				switch (nCntAttack)
				{
				case PLAYERATTACK00_SWORD:

					if (g_aDummy.nHitStopTime[nCntAttack] == 20)
					{
						g_aDummy.bHitStop[nCntAttack] = false;
						g_aDummy.nHitStopTime[nCntAttack] = 0;
					}

					break;
				case PLAYERATTACK01_BOOMERANG:
					if (g_aDummy.nHitStopTime[nCntAttack] == 10)
					{
						g_aDummy.bHitStop[nCntAttack] = false;
						g_aDummy.nHitStopTime[nCntAttack] = 0;

					}
					break;
				case PLAYERATTACK02_SPEAKER:
					if (g_aDummy.nHitStopTime[nCntAttack] == 1)
					{
						g_aDummy.bHitStop[nCntAttack] = false;
						g_aDummy.nHitStopTime[nCntAttack] = 0;

					}
					break;
				}
			}
		}
	}
}
