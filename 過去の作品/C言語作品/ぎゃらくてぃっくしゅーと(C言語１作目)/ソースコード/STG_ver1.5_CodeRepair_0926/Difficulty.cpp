//========================================================
//
//�W���R�O���F�G�t�F�N�g������񌩒���[pause.cpp]
//Author:ShinaTaiyo
//
//=========================================================

#include "result.h"
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "Difficulty.h"
#include "enemy.h"

#define DIFFICULTY_SIZE_X (100.0f)
#define DIFFICULTY_SIZE_Y (50.0f)

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureDifficulty[MAX_DIFFICULTY_TEX+MAX_WAVELENGTH_TEX] = {};//�e�N�X�`���ւ̃|�C���^
//VERTEX_2D g_aVertex[4];//���_�����i�[�i�l�p�`����肽���j
PDIRECT3DVERTEXBUFFER9 g_pVtxBuffDifficulty = NULL;//���_�o�b�t�@�ւ̃|�C���^

Difficulty g_aDifficulty[MAX_DIFFICULTY_TEX+MAX_WAVELENGTH_TEX];

int g_nChooseDifficulty;//��Փx�I�����s���ϐ�
int g_nCntShine;//�I�񂾓�Փx�����点��ϐ�

int g_nCntDifficultyTime;

GameCustom g_aGameCustom;//�Q�[���̓�Փx�Ⓑ�����J�X�^������

//���U���g��ʂ̏���������
void InitDifficulty(void)
{
	int nCntDifficulty;

	g_nCntShine = 1;
	g_nChooseDifficulty = 1;
	g_aGameCustom.nDecideCount = 0;//�I�����[�h�����Ԃɍs��

	g_nCntDifficultyTime = 0;

	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

	g_aGameCustom.nDifficulty = 1;
	g_aGameCustom.nWaveLength = 1;
	g_aGameCustom.nDecideCount = 0;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\ChooseDifficulty_bg_001.png",
		&g_pTextureDifficulty[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\Difficulty_Easy_000.png",
		&g_pTextureDifficulty[1]);


	D3DXCreateTextureFromFile(pDevice,
		"data\\Difficulty_Normal_000.png",
		&g_pTextureDifficulty[2]);


	D3DXCreateTextureFromFile(pDevice,
		"data\\Difficulty_Hard_000.png",
		&g_pTextureDifficulty[3]);


	D3DXCreateTextureFromFile(pDevice,
		"data\\Difficulty_VeryHard_000.png",
		&g_pTextureDifficulty[4]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\ShortMode_000.png",
		&g_pTextureDifficulty[5]);


	D3DXCreateTextureFromFile(pDevice,
		"data\\NormalMode_000.png",
		&g_pTextureDifficulty[6]);


	D3DXCreateTextureFromFile(pDevice,
		"data\\LongMode_000.png",
		&g_pTextureDifficulty[7]);


	D3DXCreateTextureFromFile(pDevice,
		"data\\VeryLongMode.png",
		&g_pTextureDifficulty[8]);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * (MAX_DIFFICULTY_TEX+MAX_WAVELENGTH_TEX),//���_�̐��i��Փx�̃e�N�X�`��*�E�F�[�u�̒����̃e�N�X�`���j
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffDifficulty,
		NULL);

	for (nCntDifficulty = 1; nCntDifficulty < MAX_DIFFICULTY_TEX; nCntDifficulty++)
	{//��Փx�I�𕔕��̕����̈ʒu����
		g_aDifficulty[nCntDifficulty].pos = D3DXVECTOR3(SCREEN_WIDTH / 3, 144.0f * nCntDifficulty, 0.0f);
	}
	for (nCntDifficulty = 5; nCntDifficulty < (MAX_DIFFICULTY_TEX + MAX_WAVELENGTH_TEX); nCntDifficulty++)
	{//�E�F�[�u�̒����I�𕔕��̕����̈ʒu����
		g_aDifficulty[nCntDifficulty].pos= D3DXVECTOR3(SCREEN_WIDTH / 2, 144.0f * (nCntDifficulty-4), 0.0f);
	}
	VERTEX_2D* pVtx;
	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffDifficulty->Lock(0, 0, (void**)&pVtx, 0);



	for (nCntDifficulty = 0; nCntDifficulty < (MAX_DIFFICULTY_TEX+MAX_WAVELENGTH_TEX); nCntDifficulty++)
	{
		//���_���W�̐ݒ�i�����ʒu�j
		g_aDifficulty[nCntDifficulty].nNumDifficulty = nCntDifficulty;//�ǂݍ��ރe�N�X�`���ɔԍ�������B
		g_aDifficulty[nCntDifficulty].bChooseDifficulty = false;
		if (g_aDifficulty->nNumDifficulty == DIFFICULTY_BG)
		{//��ʑS�̂ɕ\������e�N�X�`��
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
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);


			//�e�N�X�`�����W�̐ݒ�i�����ݒ�j
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
		else
		{//�R�}���h�̈ʒu���̏�����
			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);




			//rhw�̐ݒ�i�����ݒ�j
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;


			//���_�J���[�̐ݒ�i�����ݒ�j
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);


			//�e�N�X�`�����W�̐ݒ�i�����ݒ�j
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		pVtx += 4;//���_���W�̃|�C���^���C���N�������g
	}



	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffDifficulty->Unlock();
}

//==============================
//���U���g��ʂ̏I������
//==============================
void UninitDifficulty(void)
{
	int nCntDifficulty;
	//�e�N�X�`���̔j��
	for (nCntDifficulty = 0; nCntDifficulty < (MAX_DIFFICULTY_TEX+MAX_WAVELENGTH_TEX); nCntDifficulty++)
	{
		if (g_pTextureDifficulty[nCntDifficulty] != NULL)
		{
			g_pTextureDifficulty[nCntDifficulty]->Release();
			g_pTextureDifficulty[nCntDifficulty] = NULL;
		}
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffDifficulty != NULL)
	{
		g_pVtxBuffDifficulty->Release();
		g_pVtxBuffDifficulty = NULL;
	}
}

//===================================
//���U���g��ʂ̍X�V����
//===================================
void UpdateDifficulty(void)
{
	int nCntDifficulty;
	VERTEX_2D* pVtx;

	g_nCntDifficultyTime++;


	if (g_nCntDifficultyTime == 1)
	{
		SetEnemy(D3DXVECTOR3(1000.0f, 150.0f, 0.0f), ENEMYTYPE_DELBY, 100000.0f, D3DXVECTOR3(0.0f,0.0f,0.0f));
		SetEnemy(D3DXVECTOR3(920.0f, 200.0f, 0.0f), ENEMYTYPE_PUNDY, 100000.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	//========================================================================================
	//�I��ł����Փx�����点�鏈��
	//========================================================================================
	if (g_aGameCustom.nDecideCount == 0)
	{
		for (nCntDifficulty = 0; nCntDifficulty < MAX_DIFFICULTY_TEX; nCntDifficulty++)
		{//�Z�b�g����邽�тɃ|�[�Y�̖��邭�\������t���O��false�ɂ���
			g_aDifficulty[nCntDifficulty].bChooseDifficulty = false;
		}
		//�O�Ԗڂ́A�w�i�ł��邽�߁A�I�����Ȃ��悤�ɒ�������B
		if (GetKeyboardTrigger(DIK_S) == true||GetJoyPadTrigger(JOYKEY_DOWN)==true)
		{
			PlaySound(SOUND_LABEL_SE_SELECT_000);
			g_nCntShine++;//�I���ꏊ�����ɂ����
			g_aGameCustom.nDifficulty++;//��Փx�̕ϓ�
		}
		if (GetKeyboardTrigger(DIK_W) == true||GetJoyPadTrigger(JOYKEY_UP)==true)
		{
			PlaySound(SOUND_LABEL_SE_SELECT_000);
			g_nCntShine--;//�I���ꏊ����ɂ����
			g_aGameCustom.nDifficulty--;//��Փx�̕ϓ�
		}
		if (g_nCntShine >= 5)
		{
			g_nCntShine = 1;//��ԉ������ԏ�ɑI���ꏊ���ړ�
			g_aGameCustom.nDifficulty = 1;//��Փx�̕ϓ�
		}
		if (g_nCntShine <= 0)
		{
			g_nCntShine = 4;//��ԏォ���ԉ��ɑI���ꏊ���ړ�
			g_aGameCustom.nDifficulty = 4;//��Փx�̕ϓ�
		}
		g_aDifficulty[g_nCntShine].bChooseDifficulty = true;
		if (GetKeyboardTrigger(DIK_RETURN) == true||GetJoyPadTrigger(JOYKEY_START)==true)
		{
			PlaySound(SOUND_LABEL_SE_DECISION_000);
			g_nCntShine = 5;
			g_aGameCustom.nDecideCount++;//��Փx�I�����I�����A�E�F�[�u�̒����I���Ɉړ�����B
			
		}
	}
	else if (g_aGameCustom.nDecideCount == 1)
	{
		for (nCntDifficulty = 5; nCntDifficulty < (MAX_DIFFICULTY_TEX+MAX_WAVELENGTH_TEX); nCntDifficulty++)
		{//�Z�b�g����邽�тɃ|�[�Y�̖��邭�\������t���O��false�ɂ���
			g_aDifficulty[nCntDifficulty].bChooseDifficulty = false;
		}
		//�O�Ԗڂ́A�w�i�ł��邽�߁A�I�����Ȃ��悤�ɒ�������B
		if (GetKeyboardTrigger(DIK_S) == true || GetJoyPadTrigger(JOYKEY_DOWN) == true)
		{
			PlaySound(SOUND_LABEL_SE_SELECT_000);
			g_nCntShine++;//�I���ꏊ�����ɂ����
			g_aGameCustom.nWaveLength++;//�E�F�[�u�̒����̕ϓ�
		}
		if (GetKeyboardTrigger(DIK_W) == true || GetJoyPadTrigger(JOYKEY_UP) == true)
		{
			PlaySound(SOUND_LABEL_SE_SELECT_000);
			g_nCntShine--;//�I���ꏊ����ɂ����
			g_aGameCustom.nWaveLength--;//�E�F�[�u�̒����̕ϓ�
		}
		if (g_nCntShine >= 7)
		{
			g_nCntShine = 5;//��ԉ������ԏ�ɑI���ꏊ���ړ�
			g_aGameCustom.nWaveLength = 1;//�E�F�[�u�̒����̕ϓ�
		}
		if (g_nCntShine <= 4)
		{
			g_nCntShine = 6;//��ԏォ���ԉ��ɑI���ꏊ���ړ�
			g_aGameCustom.nWaveLength = 2;//�E�F�[�u�̒����̕ϓ�
		}
		g_aDifficulty[g_nCntShine].bChooseDifficulty = true;

		if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoyPadTrigger(JOYKEY_START) == true)
		{
			StopSound(SOUND_LABEL_BGM000);
			PlaySound(SOUND_LABEL_SE_DECISION_000);
			g_aGameCustom.nDecideCount++;//�E�F�[�u�̒����I�����I�����A�Q�[�����[�h��
			SetFade(MODE_GAME);
		}
	}
	//===========================================================================================


	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffDifficulty->Lock(0, 0, (void**)&pVtx, 0);

	//============================================================
	//��Փx�I���̏���
	//============================================================
	for (nCntDifficulty = 0; nCntDifficulty < MAX_DIFFICULTY_TEX; nCntDifficulty++)
	{
		//���_���W�̐ݒ�i�����ʒu�j

		if (g_aDifficulty[nCntDifficulty].nNumDifficulty >= DIFFICULTY_EASY && g_aDifficulty[nCntDifficulty].nNumDifficulty <= DIFFICULTY_VERYHARD)
		{//�|�[�Y��ʃe�N�X�`���ȊO�̃R�}���h�̈ʒu�����肷��B


			pVtx[0].pos = D3DXVECTOR3(g_aDifficulty[nCntDifficulty].pos.x - DIFFICULTY_SIZE_X, g_aDifficulty[nCntDifficulty].pos.y - DIFFICULTY_SIZE_Y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aDifficulty[nCntDifficulty].pos.x + DIFFICULTY_SIZE_X, g_aDifficulty[nCntDifficulty].pos.y - DIFFICULTY_SIZE_Y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aDifficulty[nCntDifficulty].pos.x - DIFFICULTY_SIZE_X, g_aDifficulty[nCntDifficulty].pos.y + DIFFICULTY_SIZE_Y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aDifficulty[nCntDifficulty].pos.x + DIFFICULTY_SIZE_X, g_aDifficulty[nCntDifficulty].pos.y + DIFFICULTY_SIZE_Y, 0.0f);

			//���_�J���[�̐ݒ�i�����ݒ�j
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 100);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 100);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 100);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 100);
		}
		if (g_aDifficulty[nCntDifficulty].bChooseDifficulty == true)//�I��ł����Փx�̕��������𖾂邭����B����ȊO�̓t���O��false�ɂ��邱�Ƃɂ��A��Փx�\�L�̕����𔖂�����B
		{
			//���_�J���[�̐ݒ�i�I�����Ă���R�}���h�����邭�Ȃ�j
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		}
		pVtx += 4;//���_���W�̃|�C���^���C���N�������g
	}
	//==========================================================================================================================================================================================

	//========================================
	//�E�F�[�u�̒����̏���
	//========================================
	for (nCntDifficulty = 5; nCntDifficulty < (MAX_DIFFICULTY_TEX+MAX_WAVELENGTH_TEX); nCntDifficulty++)
	{
		//���_���W�̐ݒ�i�����ʒu�j

		//if (g_aDifficulty[nCntDifficulty].nNumDifficulty >= DIFFICULTY_EASY && g_aDifficulty[nCntDifficulty].nNumDifficulty <= DIFFICULTY_VERYHARD)
		//{//�|�[�Y��ʃe�N�X�`���ȊO�̃R�}���h�̈ʒu�����肷��B


			pVtx[0].pos = D3DXVECTOR3(g_aDifficulty[nCntDifficulty].pos.x - DIFFICULTY_SIZE_X, g_aDifficulty[nCntDifficulty].pos.y - DIFFICULTY_SIZE_Y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aDifficulty[nCntDifficulty].pos.x + DIFFICULTY_SIZE_X, g_aDifficulty[nCntDifficulty].pos.y - DIFFICULTY_SIZE_Y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aDifficulty[nCntDifficulty].pos.x - DIFFICULTY_SIZE_X, g_aDifficulty[nCntDifficulty].pos.y + DIFFICULTY_SIZE_Y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aDifficulty[nCntDifficulty].pos.x + DIFFICULTY_SIZE_X, g_aDifficulty[nCntDifficulty].pos.y + DIFFICULTY_SIZE_Y, 0.0f);

			//���_�J���[�̐ݒ�i�����ݒ�j
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 100);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 100);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 100);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 100);
		//}
		if (g_aDifficulty[nCntDifficulty].bChooseDifficulty == true)//�I��ł����Փx�̕��������𖾂邭����B����ȊO�̓t���O��false�ɂ��邱�Ƃɂ��A��Փx�\�L�̕����𔖂�����B
		{
			//���_�J���[�̐ݒ�i�I�����Ă���R�}���h�����邭�Ȃ�j
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		}
		pVtx += 4;//���_���W�̃|�C���^���C���N�������g
	}
	//=================================================================================================================================================================================

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffDifficulty->Unlock();
}

//================================
//�^�C�g����ʂ̕`�揈��
//================================
void DrawDifficulty(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

	int nCntDifficulty;
	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffDifficulty, 0, sizeof(VERTEX_2D));//���_�o�b�t�@�ւ̃|�C���^�ƒ��_���̍\���̂̃T�C�Y

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);


	for (nCntDifficulty = 0; nCntDifficulty < (MAX_DIFFICULTY_TEX+MAX_WAVELENGTH_TEX); nCntDifficulty++)
	{
		//�e�N�X�`���̐ݒ�
		if (g_aDifficulty[nCntDifficulty].nNumDifficulty == DIFFICULTY_BG)
		{
			pDevice->SetTexture(0, g_pTextureDifficulty[DIFFICULTY_BG]);
		}
		if (g_aDifficulty[nCntDifficulty].nNumDifficulty == DIFFICULTY_EASY)
		{
			pDevice->SetTexture(0, g_pTextureDifficulty[DIFFICULTY_EASY]);
		}
		if (g_aDifficulty[nCntDifficulty].nNumDifficulty == DIFFICULTY_NORMAL)
		{
			pDevice->SetTexture(0, g_pTextureDifficulty[DIFFICULTY_NORMAL]);
		}
		if (g_aDifficulty[nCntDifficulty].nNumDifficulty == DIFFICULTY_HARD)
		{
			pDevice->SetTexture(0, g_pTextureDifficulty[DIFFICULTY_HARD]);
		}
		if (g_aDifficulty[nCntDifficulty].nNumDifficulty == DIFFICULTY_VERYHARD)
		{
			pDevice->SetTexture(0, g_pTextureDifficulty[DIFFICULTY_VERYHARD]);
		}
		if (g_aDifficulty[nCntDifficulty].nNumDifficulty == WAVELENGTH_SHORT)
		{
			pDevice->SetTexture(0, g_pTextureDifficulty[WAVELENGTH_SHORT]);
		}
		if (g_aDifficulty[nCntDifficulty].nNumDifficulty == WAVELENGTH_NORMAL)
		{
			pDevice->SetTexture(0, g_pTextureDifficulty[WAVELENGTH_NORMAL]);
		}
		if (g_aDifficulty[nCntDifficulty].nNumDifficulty == WAVELENGTH_LONG)
		{
			pDevice->SetTexture(0, g_pTextureDifficulty[WAVELENGTH_LONG]);
		}
		if (g_aDifficulty[nCntDifficulty].nNumDifficulty == WAVELENGTH_VERYLONG)
		{
			pDevice->SetTexture(0, g_pTextureDifficulty[WAVELENGTH_VERYLONG]);
		}
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntDifficulty * 4, 2);//0�́A�`�悷��ŏ��̒��_�C���f�b�N�X�ŁA2�́A�`�悷��v���~�e�B�u��
			//&g_pVtxBuffPolygon[0],//���_���̐擪�A�h���X
			//sizeof(VERTEX_2D));//���_���\���̂̃T�C�Y
	}
}

//=============================================
//�Q�[���̃��[�h�J�X�^���̍\���̂��Q�b�g����B
//=============================================
GameCustom *GetGameCustom(void)
{
	return &g_aGameCustom;
}

	