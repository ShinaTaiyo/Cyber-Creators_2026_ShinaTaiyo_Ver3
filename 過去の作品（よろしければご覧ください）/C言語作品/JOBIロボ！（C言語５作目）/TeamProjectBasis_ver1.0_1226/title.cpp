//=========================================================
//
// �^�C�g����ʂ̏���
// Author : Ueda Kou
//
//=========================================================
#include "title.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "player.h"
#include "field.h"
#include "wall.h"
#include "camera.h"
#include "light.h"
#include "model.h"
#include "billboard.h"

//=========================================================
// �O���[�o���ϐ�
//=========================================================
//++++++++++++++++++++++++++++++++
// �^�C�g���S��
//++++++++++++++++++++++++++++++++
int g_nCntTitle;
int g_nSelect;
//++++++++++++++++++++++++++++++++
// �^�C�g�����S
//++++++++++++++++++++++++++++++++
LPDIRECT3DTEXTURE9 g_pTextureTitleLogo = {};	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitleLogo = NULL;	//���_�o�b�t�@�ւ̃|�C���^
TITLELOGO g_aTitleLogo;										//�\���̏��
//++++++++++++++++++++++++++++++++
// �^�C�g�����j���[
//++++++++++++++++++++++++++++++++
LPDIRECT3DTEXTURE9 g_pTextureTitleMenu[TITLEMENU_MAX] = {};	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitleMenu = NULL;	//���_�o�b�t�@�ւ̃|�C���^
TITLEMENU g_aTitleMenu[TITLEMENU_MAX];

//*******************************************************************
// �^�C�g���S�̏���
//*******************************************************************
//=========================================================
// ����������
//=========================================================
void InitTitle(void)
{
	g_nCntTitle = 0;//�o�ߎ���
	g_nSelect = 0;	//�I��
	
	InitTitleLogo();//���S
	InitTitleMenu();//���j���[
	InitPlayer();	//�v���C���[
	InitField();	//�t�B�[���h
	InitWall();		//��
	InitCamera();	//�J����
	InitLight();	//���C�g
	InitBG();		//�w�i
	InitModel();	//���f��

	LoadPlayer();	//�v���C���[�̃p�[�c���̓ǂݍ���	
	LoadWeapon();	//�v���C���[�̕���̓ǂݍ���
	LoadModeltxt();	//���f�����̓ǂݍ���(txt)

	PlaySound(SOUND_LABEL_BGM001);
}

//=========================================================
// �I������
//=========================================================
void UninitTitle(void)
{
	g_nCntTitle = 0;
	UninitTitleLogo();//���S
	UninitTitleMenu();//���j���[
	UninitPlayer();	//�v���C���[
	UninitField();	//�t�B�[���h
	UninitWall();		//��
	UninitCamera();	//�J����
	UninitLight();	//���C�g
	UninitBG();		//�w�i
	UninitModel();	//���f��

	
}

//=========================================================
// �X�V����
//=========================================================
void UpdateTitle(void)
{
	UpdateTitleLogo();	//���S
	UpdateTitleMenu();	//���j���[
	UpdatePlayer();		//�v���C���[
	UpdateField();		//�t�B�[���h
	UpdateWall();		//��
	UpdateCamera();		//�J����
	UpdateModel();		//���f��

	g_nCntTitle++;

	// ����!!(��������ɏ����������Ȃ�
	SelectTitle();		// ���C�����j���[
}

//=========================================================
// �`�揈��
//=========================================================
void DrawTitle(void)
{
	SetCamera();	//�J����
	DrawModel();	//���f��
	DrawPlayer();	//�v���C���[
	DrawField();	//�t�B�[���h
	DrawWall();		//��
	DrawTitleLogo();//���S
	DrawTitleMenu();//���j���[
}

//=========================================================
// �I������
//=========================================================
void SelectTitle(void)
{
	if (GetKeyboardTrigger(DIK_UP) == true || GetKeyboardTrigger(DIK_W) == true || GetJoypadTrigger(JOYKEY_UP))
	{// ����L�[�������ꂽ��
		g_nSelect--;
		if (g_nSelect < 0)
		{
			g_nSelect = TITLEMENU_MAX - 1;
		}

		PlaySound(SOUND_LABEL_SE_SELECT_000);

		SelectTitleMenu();
	}
	else if (GetKeyboardTrigger(DIK_DOWN) == true || GetKeyboardTrigger(DIK_S) == true || GetJoypadTrigger(JOYKEY_DOWN))
	{// �����L�[�������ꂽ��
		g_nSelect++;
		if (g_nSelect >= TITLEMENU_MAX)
		{
			g_nSelect = 0;
		}

		PlaySound(SOUND_LABEL_SE_SELECT_000);


		SelectTitleMenu();
	}

	//Enter�L�[ or START�{�^���������ꂽ�ꍇ
	if ((GetKeyboardTrigger(DIK_RETURN) == true && g_nCntTitle > TITLELOGO_TIME || GetJoypadTrigger(JOYKEY_START) == true || GetJoypadTrigger(JOYKEY_A) == true) && g_nCntTitle > TITLELOGO_TIME)
	{

		PlaySound(SOUND_LABEL_SE_DECIDE_000);

		switch (g_nSelect)
		{
		case TITLEMENU_START:
			StopSound(SOUND_LABEL_BGM001);
			SetFade(MODE_TUTORIAL);	//�`���[�g���A����ʂֈڍs
			break;
		case TITLEMENU_RANKING:
			SetFade(MODE_RANKING);	//�����L���O��ʂֈڍs
			break;
		case TITLEMENU_END:
			SetMode(MODE_END);	//�I����ʂֈڍs
			break;
		}

	}
}

//*******************************************************************
// �^�C�g�����S����
//*******************************************************************
//=========================================================
// ����������
//=========================================================
void InitTitleLogo(void)
{
	// ������
	{
		g_aTitleLogo.pos = TITLELOGO_POS;								// �ʒu������
		g_aTitleLogo.postarget = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu(�ڕW)������
		g_aTitleLogo.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0);			// ����������
		g_aTitleLogo.rottarget = D3DXVECTOR3(0.0f, 0.0f, 0.0);		// ����(�ڕW)������
		g_aTitleLogo.size = TITLELOGO_SIZE;								// �T�C�Y������
		g_aTitleLogo.fScale = 1.0f;									// �T�C�Y(�{��)������
		g_aTitleLogo.fScaletarget = 1.0f;							// �T�C�Y(�ڕW)������
		g_aTitleLogo.fAngle = atan2f(TITLELOGO_SIZE.x, TITLELOGO_SIZE.y);	// ���_�p�x
		g_aTitleLogo.fLeng = sqrtf((TITLELOGO_SIZE.x) * (TITLELOGO_SIZE.x) + (TITLELOGO_SIZE.y) * (TITLELOGO_SIZE.y)) * 0.5f;
		g_aTitleLogo.nArrivalType = TITLEROGO_ROLL;						// �o�����@������
	}

	VERTEX_2D* pVtx;						//���_���ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̎擾

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Title_bg_001.png"
		, &g_pTextureTitleLogo);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitleLogo,
		NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitleLogo->Lock(0, 0, (void**)&pVtx, 0);

	// �f�t�H���g�ݒ�

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_aTitleLogo.pos.x - (g_aTitleLogo.size.x * 0.5f) * g_aTitleLogo.fScale, g_aTitleLogo.pos.y - (g_aTitleLogo.size.y * 0.5f) * g_aTitleLogo.fScale, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aTitleLogo.pos.x + (g_aTitleLogo.size.x * 0.5f) * g_aTitleLogo.fScale, g_aTitleLogo.pos.y - (g_aTitleLogo.size.y * 0.5f) * g_aTitleLogo.fScale, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aTitleLogo.pos.x - (g_aTitleLogo.size.x * 0.5f) * g_aTitleLogo.fScale, g_aTitleLogo.pos.y + (g_aTitleLogo.size.y * 0.5f) * g_aTitleLogo.fScale, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aTitleLogo.pos.x + (g_aTitleLogo.size.x * 0.5f) * g_aTitleLogo.fScale, g_aTitleLogo.pos.y + (g_aTitleLogo.size.y * 0.5f) * g_aTitleLogo.fScale, 0.0f);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	
	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// �o�����@�ʏ���
	switch (g_aTitleLogo.nArrivalType)
	{
	case TITLELOGO_NORMAL:
		break;
	case TITLEROGO_ROLL:
		// ������]�o���Ȃ�ʒu��ύX����
		g_aTitleLogo.pos = TITLELOGO_ARR_ROLL_POS;
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aTitleLogo.pos.x - (g_aTitleLogo.size.x * 0.5f) * g_aTitleLogo.fScale, g_aTitleLogo.pos.y - (g_aTitleLogo.size.y * 0.5f) * g_aTitleLogo.fScale, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aTitleLogo.pos.x + (g_aTitleLogo.size.x * 0.5f) * g_aTitleLogo.fScale, g_aTitleLogo.pos.y - (g_aTitleLogo.size.y * 0.5f) * g_aTitleLogo.fScale, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aTitleLogo.pos.x - (g_aTitleLogo.size.x * 0.5f) * g_aTitleLogo.fScale, g_aTitleLogo.pos.y + (g_aTitleLogo.size.y * 0.5f) * g_aTitleLogo.fScale, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aTitleLogo.pos.x + (g_aTitleLogo.size.x * 0.5f) * g_aTitleLogo.fScale, g_aTitleLogo.pos.y + (g_aTitleLogo.size.y * 0.5f) * g_aTitleLogo.fScale, 0.0f);
		break;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTitleLogo->Unlock();
}

//=========================================================
// �I������
//=========================================================
void UninitTitleLogo(void)
{
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffTitleLogo != NULL)
	{
		g_pVtxBuffTitleLogo->Release();
		g_pVtxBuffTitleLogo = NULL;
	}
	//�e�N�X�`���̔j��
	if (g_pTextureTitleLogo != NULL)
	{
		g_pTextureTitleLogo->Release();
		g_pTextureTitleLogo = NULL;
	}
}

//=========================================================
// �X�V����
//=========================================================
void UpdateTitleLogo(void)
{
	VERTEX_2D* pVtx;						//���_���ւ̃|�C���^
	if ((GetKeyboardTrigger(DIK_RETURN) == true || GetJoypadTrigger(JOYKEY_START) == true || GetJoypadTrigger(JOYKEY_A)) && g_nCntTitle < TITLELOGO_TIME)
	{// �^�C�g���A�j���[�V�����������I��
		g_nCntTitle = TITLELOGO_TIME - 1;

		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffTitleLogo->Lock(0, 0, (void**)&pVtx, 0);

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(TITLELOGO_RED_POS.x - (g_aTitleLogo.size.x * 0.5f) * TITLELOGO_RED_SCALE, TITLELOGO_RED_POS.y - (g_aTitleLogo.size.y * 0.5f) * TITLELOGO_RED_SCALE, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(TITLELOGO_RED_POS.x + (g_aTitleLogo.size.x * 0.5f) * TITLELOGO_RED_SCALE, TITLELOGO_RED_POS.y - (g_aTitleLogo.size.y * 0.5f) * TITLELOGO_RED_SCALE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(TITLELOGO_RED_POS.x - (g_aTitleLogo.size.x * 0.5f) * TITLELOGO_RED_SCALE, TITLELOGO_RED_POS.y + (g_aTitleLogo.size.y * 0.5f) * TITLELOGO_RED_SCALE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(TITLELOGO_RED_POS.x + (g_aTitleLogo.size.x * 0.5f) * TITLELOGO_RED_SCALE, TITLELOGO_RED_POS.y + (g_aTitleLogo.size.y * 0.5f) * TITLELOGO_RED_SCALE, 0.0f);
		//���_�o�b�t�@���A�����b�N����
		g_pVtxBuffTitleLogo->Unlock();
	}
	else
	{// �^�C�g���A�j���[�V����
		// �o�����@
		if (g_nCntTitle <= TITLELOGO_ARR_TIME)
		{
			switch (g_aTitleLogo.nArrivalType)
			{
			case TITLELOGO_NORMAL:
				break;
			case TITLEROGO_ROLL:
				{
			
					if (g_nCntTitle == 0)	// �ڕW���ݒ�
					{
						g_aTitleLogo.rottarget = g_aTitleLogo.rot - (TITLELOGO_ARR_ROLL_ROT) / TITLELOGO_RED_TIME;
						// �ڕW�ʒu = ���ݍ��W�ƖڕW���W�̍��� / �ύX�ɂ����鎞��
						g_aTitleLogo.postarget.x = (g_aTitleLogo.pos.x - TITLELOGO_POS.x) / TITLELOGO_RED_TIME;
						g_aTitleLogo.postarget.y = (g_aTitleLogo.pos.y - TITLELOGO_POS.y) / TITLELOGO_RED_TIME;
						g_aTitleLogo.postarget.z = 0.0f;
					}
					else
					{
						g_aTitleLogo.pos -= g_aTitleLogo.postarget;
						g_aTitleLogo.rot.z += g_aTitleLogo.rottarget.z;
					}

					//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
					g_pVtxBuffTitleLogo->Lock(0, 0, (void**)&pVtx, 0);

					//���_���W�̐ݒ�
					pVtx[0].pos = D3DXVECTOR3(g_aTitleLogo.pos.x + sinf(g_aTitleLogo.rot.z - (D3DX_PI - g_aTitleLogo.fAngle)) * g_aTitleLogo.fLeng * g_aTitleLogo.fScale, g_aTitleLogo.pos.y + cosf(g_aTitleLogo.rot.z - (D3DX_PI - g_aTitleLogo.fAngle)) * g_aTitleLogo.fLeng * g_aTitleLogo.fScale, 0.0f);
					pVtx[1].pos = D3DXVECTOR3(g_aTitleLogo.pos.x + sinf(g_aTitleLogo.rot.z + (D3DX_PI - g_aTitleLogo.fAngle)) * g_aTitleLogo.fLeng * g_aTitleLogo.fScale, g_aTitleLogo.pos.y + cosf(g_aTitleLogo.rot.z + (D3DX_PI - g_aTitleLogo.fAngle)) * g_aTitleLogo.fLeng * g_aTitleLogo.fScale, 0.0f);
					pVtx[2].pos = D3DXVECTOR3(g_aTitleLogo.pos.x + sinf(g_aTitleLogo.rot.z - (g_aTitleLogo.fAngle)) * g_aTitleLogo.fLeng * g_aTitleLogo.fScale, g_aTitleLogo.pos.y + cosf(g_aTitleLogo.rot.z - (g_aTitleLogo.fAngle)) * g_aTitleLogo.fLeng * g_aTitleLogo.fScale, 0.0f);
					pVtx[3].pos = D3DXVECTOR3(g_aTitleLogo.pos.x + sinf(g_aTitleLogo.rot.z + (g_aTitleLogo.fAngle)) * g_aTitleLogo.fLeng * g_aTitleLogo.fScale, g_aTitleLogo.pos.y + cosf(g_aTitleLogo.rot.z + (g_aTitleLogo.fAngle)) * g_aTitleLogo.fLeng * g_aTitleLogo.fScale, 0.0f);
					//���_�o�b�t�@���A�����b�N����
					g_pVtxBuffTitleLogo->Unlock();
				}
				break;
			}
		}
		// �^�C�g���k��
		else if (g_nCntTitle >= TITLELOGO_ARR_TIME + TITLELOGO_RED_START && g_nCntTitle < TITLELOGO_ARR_TIME + TITLELOGO_RED_START + TITLELOGO_RED_TIME)
		{
			if (g_nCntTitle == TITLELOGO_ARR_TIME + TITLELOGO_RED_START)	// �ڕW���ݒ�
			{
				// �ڕW�T�C�Y = ���ݔ{���ƖڕW�{���̍��� / �ύX�ɂ����鎞��
				g_aTitleLogo.fScaletarget = (g_aTitleLogo.fScale - TITLELOGO_RED_SCALE) / TITLELOGO_RED_TIME;
				// �ڕW�ʒu = ���ݍ��W�ƖڕW���W�̍��� / �ύX�ɂ����鎞��
				g_aTitleLogo.postarget.x = (g_aTitleLogo.pos.x - TITLELOGO_RED_POS.x) / TITLELOGO_RED_TIME;
				g_aTitleLogo.postarget.y = (g_aTitleLogo.pos.y - TITLELOGO_RED_POS.y) / TITLELOGO_RED_TIME;
				g_aTitleLogo.postarget.z = 0.0f;
			}
			else
			{
				g_aTitleLogo.fScale -= g_aTitleLogo.fScaletarget;
				g_aTitleLogo.pos -= g_aTitleLogo.postarget;
			}

			//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
			g_pVtxBuffTitleLogo->Lock(0, 0, (void**)&pVtx, 0);

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aTitleLogo.pos.x - (g_aTitleLogo.size.x * 0.5f) * g_aTitleLogo.fScale, g_aTitleLogo.pos.y - (g_aTitleLogo.size.y * 0.5f) * g_aTitleLogo.fScale, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aTitleLogo.pos.x + (g_aTitleLogo.size.x * 0.5f) * g_aTitleLogo.fScale, g_aTitleLogo.pos.y - (g_aTitleLogo.size.y * 0.5f) * g_aTitleLogo.fScale, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aTitleLogo.pos.x - (g_aTitleLogo.size.x * 0.5f) * g_aTitleLogo.fScale, g_aTitleLogo.pos.y + (g_aTitleLogo.size.y * 0.5f) * g_aTitleLogo.fScale, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aTitleLogo.pos.x + (g_aTitleLogo.size.x * 0.5f) * g_aTitleLogo.fScale, g_aTitleLogo.pos.y + (g_aTitleLogo.size.y * 0.5f) * g_aTitleLogo.fScale, 0.0f);
			//���_�o�b�t�@���A�����b�N����
			g_pVtxBuffTitleLogo->Unlock();
		}
	}
}

//=========================================================
// �`�揈��
//=========================================================
void DrawTitleLogo(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();						//�f�o�C�X�̎擾
	pDevice->SetStreamSource(0, g_pVtxBuffTitleLogo, 0, sizeof(VERTEX_2D));//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);									//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetTexture(0, g_pTextureTitleLogo);				//�e�N�X�`���̐ݒ�
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);				//�|���S���̕`��
}

//*******************************************************************
// �^�C�g���I������
//*******************************************************************
//=========================================================
// ����������
//=========================================================
void InitTitleMenu(void)
{
	// ������
	for (int nCnt = 0; nCnt < TITLEMENU_MAX; nCnt++)
	{
		g_aTitleMenu[nCnt].pos.x = TITLEMENU_POS.x ;														// �ʒu������
		g_aTitleMenu[nCnt].pos.y = TITLEMENU_POS.y + (TITLEMENU_SIZE .y * nCnt) + (TITLEMENU_FALL * nCnt);	// �ʒu������
		g_aTitleMenu[nCnt].pos.z = TITLEMENU_POS.z;															// �ʒu������
		g_aTitleMenu[nCnt].size = TITLEMENU_SIZE;															// �T�C�Y������
		g_aTitleMenu[nCnt].col = TITLEMENU_COL_NOTSELECT;													// �F������
		g_aTitleMenu[nCnt].col.a = 0.0f;																	// �F������
		g_aTitleMenu[nCnt].fColaTarget = 0.0f;																// �Fa(�ڕW)������
		g_aTitleMenu[nCnt].nType = nCnt;																	// ��ފ��蓖��
		g_aTitleMenu[nCnt].bUse = false;																	// ��\���ɂ���
	}
	g_aTitleMenu[0].col = TITLEMENU_COL_SELECT;		// �I������
	g_aTitleMenu[0].col.a = 0.0f;					// �I������

	VERTEX_2D* pVtx;						//���_���ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̎擾

	for (int nCnt = 0; nCnt < TITLEMENU_MAX; nCnt++)
	{
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(
			pDevice,
			cTITLEMENU_TEXTURE_FILENAME[nCnt],
			&g_pTextureTitleMenu[nCnt]);
	}
	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * TITLEMENU_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitleMenu,
		NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitleMenu->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < TITLEMENU_MAX; nCnt++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aTitleMenu[nCnt].pos.x - (g_aTitleMenu[nCnt].size.x * 0.5f), g_aTitleMenu[nCnt].pos.y - (g_aTitleMenu[nCnt].size.y * 0.5f), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aTitleMenu[nCnt].pos.x + (g_aTitleMenu[nCnt].size.x * 0.5f), g_aTitleMenu[nCnt].pos.y - (g_aTitleMenu[nCnt].size.y * 0.5f), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aTitleMenu[nCnt].pos.x - (g_aTitleMenu[nCnt].size.x * 0.5f), g_aTitleMenu[nCnt].pos.y + (g_aTitleMenu[nCnt].size.y * 0.5f), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aTitleMenu[nCnt].pos.x + (g_aTitleMenu[nCnt].size.x * 0.5f), g_aTitleMenu[nCnt].pos.y + (g_aTitleMenu[nCnt].size.y * 0.5f), 0.0f);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = g_aTitleMenu[nCnt].col;
		pVtx[1].col = g_aTitleMenu[nCnt].col;
		pVtx[2].col = g_aTitleMenu[nCnt].col;
		pVtx[3].col = g_aTitleMenu[nCnt].col;

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTitleMenu->Unlock();
}
//=========================================================
// �I������
//=========================================================
void UninitTitleMenu(void)
{
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffTitleMenu != NULL)
	{
		g_pVtxBuffTitleMenu->Release();
		g_pVtxBuffTitleMenu = NULL;
	}
	//�e�N�X�`���̔j��
	for (int nCnt = 0; nCnt < TITLEMENU_MAX; nCnt++)
	{
		if (g_pTextureTitleMenu[nCnt] != NULL)
		{
			g_pTextureTitleMenu[nCnt]->Release();
			g_pTextureTitleMenu[nCnt] = NULL;
		}
	}
}
//=========================================================
// �X�V����
//=========================================================
void UpdateTitleMenu(void)
{
	VERTEX_2D* pVtx;//���_���ւ̃|�C���^

	if (g_nCntTitle >= TITLELOGO_TIME)
	{
		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffTitleMenu->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCnt = 0; nCnt < TITLEMENU_MAX; nCnt++)
		{
			if (g_nCntTitle < TITLELOGO_TIME + TITLEMENU_TIME)
			{
				if (g_nCntTitle == TITLELOGO_TIME)
				{
					g_aTitleMenu[nCnt].bUse = true;
					g_aTitleMenu[nCnt].fColaTarget = (g_aTitleMenu[nCnt].col.a - TITLEMENU_COL_A) / TITLEMENU_TIME;
				}
				else
				{
					g_aTitleMenu[nCnt].col.a -= g_aTitleMenu[nCnt].fColaTarget;
				}
				//���_�J���[�̐ݒ�
				pVtx[0].col = g_aTitleMenu[nCnt].col;
				pVtx[1].col = g_aTitleMenu[nCnt].col;
				pVtx[2].col = g_aTitleMenu[nCnt].col;
				pVtx[3].col = g_aTitleMenu[nCnt].col;

				pVtx += 4;

			}
		}
		//���_�o�b�t�@���A�����b�N����
		g_pVtxBuffTitleMenu->Unlock();
	}
}

//=========================================================
// �`�揈��
//=========================================================
void DrawTitleMenu(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̎擾
	for (int nCnt = 0; nCnt < TITLEMENU_MAX; nCnt++)
	{
		if (g_aTitleMenu[nCnt].bUse == true)
		{
			pDevice->SetStreamSource(0, g_pVtxBuffTitleMenu, 0, sizeof(VERTEX_2D));//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetFVF(FVF_VERTEX_2D);									//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetTexture(0, g_pTextureTitleMenu[nCnt]);				//�e�N�X�`���̐ݒ�
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCnt, 2);				//�|���S���̕`��
		}
	}
}
//=========================================================
// �I��������
//=========================================================
void SelectTitleMenu(void)
{
	VERTEX_2D* pVtx;						//���_���ւ̃|�C���^
	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitleMenu->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCnt = 0; nCnt < TITLEMENU_MAX; nCnt++)
	{
		if (g_nSelect == nCnt)
		{
			g_aTitleMenu[nCnt].col.r = TITLEMENU_COL_SELECT.r;
			g_aTitleMenu[nCnt].col.g = TITLEMENU_COL_SELECT.g;
			g_aTitleMenu[nCnt].col.b = TITLEMENU_COL_SELECT.b;
		}
		else
		{
			g_aTitleMenu[nCnt].col.r = TITLEMENU_COL_NOTSELECT.r;
			g_aTitleMenu[nCnt].col.g = TITLEMENU_COL_NOTSELECT.g;
			g_aTitleMenu[nCnt].col.b = TITLEMENU_COL_NOTSELECT.b;
		}
		//���_�J���[�̐ݒ�
		pVtx[0].col = g_aTitleMenu[nCnt].col;
		pVtx[1].col = g_aTitleMenu[nCnt].col;
		pVtx[2].col = g_aTitleMenu[nCnt].col;
		pVtx[3].col = g_aTitleMenu[nCnt].col;

		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTitleMenu->Unlock();
}