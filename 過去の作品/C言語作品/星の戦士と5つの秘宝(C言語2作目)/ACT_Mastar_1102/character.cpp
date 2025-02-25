//========================================================
//
//�W���R�O���F�G�t�F�N�g������񌩒���[Character.cpp]
//Author:ShinaTaiyo
//
//=========================================================

#include "result.h"
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "character.h"


#define CHARACTER_SIZE_X (150.0f)
#define CHARACTER_SIZE_Y (150.0f)

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureCharacter[MAX_CHARACTER_TEX] = {};//�e�N�X�`���ւ̃|�C���^
//VERTEX_2D g_aVertex[4];//���_�����i�[�i�l�p�`����肽���j
PDIRECT3DVERTEXBUFFER9 g_pVtxBuffCharacter = NULL;//���_�o�b�t�@�ւ̃|�C���^

LPDIRECT3DTEXTURE9 g_pTextureCharacterBG = NULL;//�e�N�X�`���ւ̃|�C���^
//VERTEX_2D g_aVertex[4];//���_�����i�[�i�l�p�`����肽���j
PDIRECT3DVERTEXBUFFER9 g_pVtxBuffCharacterBG = NULL;//���_�o�b�t�@�ւ̃|�C���^


Character g_aCharacter[MAX_CHARACTER_TEX];

CharacterNum g_aCharacterNum;

int g_nChooseNum;

//���U���g��ʂ̏���������
void InitChacacter(void)
{
	int nCntCharacter;

	g_nChooseNum = 0;

	g_aCharacterNum.nCharacterNum = 0;


	VERTEX_2D* pVtx;

	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();


	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Select.png",
		&g_pTextureCharacterBG);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,//���_�̐�
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffCharacterBG,
		NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffCharacterBG->Lock(0, 0, (void**)&pVtx, 0);




	//�R�}���h�̈ʒu���̏�����
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

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffCharacterBG->Unlock();




	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\PlayerBULE.png",
		&g_pTextureCharacter[1]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\PlayerRED.png",
		&g_pTextureCharacter[0]);


	/*D3DXCreateTextureFromFile(pDevice,
		"data\\RETRY_indicate.png",
		&g_pTextureCharacter[2]);


	D3DXCreateTextureFromFile(pDevice,
		"data\\QUIT_indicate.png",
		&g_pTextureCharacter[3]);*/

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_CHARACTER_TEX,//���_�̐�
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffCharacter,
		NULL);

	for (nCntCharacter = 0; nCntCharacter < MAX_CHARACTER_TEX; nCntCharacter++)
	{

		g_aCharacter[nCntCharacter].pos = D3DXVECTOR3(SCREEN_WIDTH / 2-200.0f+(450.0f*nCntCharacter),SCREEN_HEIGHT/2, 0.0f);//�^�񒆂ɎO�i�I�v�V�����R�}���h��p�ӂ���B
		g_aCharacter[nCntCharacter].nType = nCntCharacter;//4�̃|���S���Ƀe�N�X�`�������蓖�Ă�B
		g_aCharacter[nCntCharacter].bChooseCharacter = false;
	}
	
	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffCharacter->Lock(0, 0, (void**)&pVtx, 0);



	for (nCntCharacter = 0; nCntCharacter < MAX_CHARACTER_TEX; nCntCharacter++)
	{
		//�R�}���h�̈ʒu���̏�����
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
		

		pVtx += 4;//���_���W�̃|�C���^���C���N�������g
	}



	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffCharacter->Unlock();

	
}

//==============================
//���U���g��ʂ̏I������
//==============================
void UninitChacacter(void)
{
	int nCntCharacter;
	//�e�N�X�`���̔j��
	for (nCntCharacter = 0; nCntCharacter < MAX_CHARACTER_TEX; nCntCharacter++)
	{
		if (g_pTextureCharacter[nCntCharacter] != NULL)
		{
			g_pTextureCharacter[nCntCharacter]->Release();
			g_pTextureCharacter[nCntCharacter] = NULL;
		}
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffCharacter != NULL)
	{
		g_pVtxBuffCharacter->Release();
		g_pVtxBuffCharacter = NULL;
	}

	if (g_pTextureCharacterBG != NULL)
	{					   
		g_pTextureCharacterBG->Release();
		g_pTextureCharacterBG = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffCharacterBG != NULL)
	{
		g_pVtxBuffCharacterBG->Release();
		g_pVtxBuffCharacterBG = NULL;
	}
}

//===================================
//���U���g��ʂ̍X�V����
//===================================
void UpdateCharacter(void)
{
	int nCntCharacter;
	VERTEX_2D* pVtx;

	if (GetKeyboardTrigger(DIK_RIGHT) == true)
	{
		g_nChooseNum = 1;
	}
	if (GetKeyboardTrigger(DIK_LEFT) == true)
	{
		g_nChooseNum = 0;
	}

	if (g_nChooseNum == 1)
	{
		g_aCharacterNum.nCharacterNum = CHACACTER_TYPE_HEAVY;//�d���L�����N�^�[���Z�b�g����
		g_aCharacter[CHACACTER_TYPE_HEAVY].bChooseCharacter = true;
		g_aCharacter[CHACACTER_TYPE_LIGHT].bChooseCharacter = false;
	}
	else
	{
		g_aCharacterNum.nCharacterNum = CHACACTER_TYPE_LIGHT;//�y���L�����N�^�[���Z�b�g����
		g_aCharacter[CHACACTER_TYPE_HEAVY].bChooseCharacter = false;
		g_aCharacter[CHACACTER_TYPE_LIGHT].bChooseCharacter = true;
	}

	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		SetFade(MODE_GAME);//�Q�[�����[�h���Ăяo��
	}

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffCharacter->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntCharacter = 0; nCntCharacter < MAX_CHARACTER_TEX; nCntCharacter++)
	{
	
		//�|�[�Y��ʃe�N�X�`���ȊO�̃R�}���h�̈ʒu�����肷��B


			pVtx[0].pos = D3DXVECTOR3(g_aCharacter[nCntCharacter].pos.x -CHARACTER_SIZE_X, g_aCharacter[nCntCharacter].pos.y - CHARACTER_SIZE_Y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aCharacter[nCntCharacter].pos.x +CHARACTER_SIZE_X, g_aCharacter[nCntCharacter].pos.y - CHARACTER_SIZE_Y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aCharacter[nCntCharacter].pos.x -CHARACTER_SIZE_X, g_aCharacter[nCntCharacter].pos.y + CHARACTER_SIZE_Y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aCharacter[nCntCharacter].pos.x +CHARACTER_SIZE_X, g_aCharacter[nCntCharacter].pos.y + CHARACTER_SIZE_Y, 0.0f);

			//���_�J���[�̐ݒ�i�����ݒ�j
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 100);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 100);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 100);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 100);
		
		if (g_aCharacter[nCntCharacter].bChooseCharacter == true)
		{
			//���_�J���[�̐ݒ�i�I�����Ă���L�����N�^�[�����邭�Ȃ�j
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		}
		pVtx += 4;//���_���W�̃|�C���^���C���N�������g
	}


	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffCharacter->Unlock();
}

//================================
//�^�C�g����ʂ̕`�揈��
//================================
void DrawCharacter(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

	int nCntCharacter;
	//�f�o�C�X�̎擾
	pDevice = GetDevice();


	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffCharacterBG, 0, sizeof(VERTEX_2D));//���_�o�b�t�@�ւ̃|�C���^�ƒ��_���̍\���̂̃T�C�Y

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetTexture(0, g_pTextureCharacterBG);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);//0�́A�`�悷��ŏ��̒��_�C���f�b�N�X�ŁA2�́A�`�悷��v���~�e�B�u��
		//&g_pVtxBuffPolygon[0],//���_���̐擪�A�h���X
		//sizeof(VERTEX_2D));//���_���\���̂̃T�C�Y



	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffCharacter, 0, sizeof(VERTEX_2D));//���_�o�b�t�@�ւ̃|�C���^�ƒ��_���̍\���̂̃T�C�Y

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);


	for (nCntCharacter = 0; nCntCharacter < MAX_CHARACTER_TEX; nCntCharacter++)
	{
		//�e�N�X�`���̐ݒ�
		if (g_aCharacter[nCntCharacter].nType == CHACACTER_TYPE_LIGHT)
		{
			pDevice->SetTexture(0, g_pTextureCharacter[CHACACTER_TYPE_LIGHT]);
		}
		if (g_aCharacter[nCntCharacter].nType == CHACACTER_TYPE_HEAVY)
		{
			pDevice->SetTexture(0, g_pTextureCharacter[CHACACTER_TYPE_HEAVY]);
		}
		
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntCharacter * 4, 2);//0�́A�`�悷��ŏ��̒��_�C���f�b�N�X�ŁA2�́A�`�悷��v���~�e�B�u��
			//&g_pVtxBuffPolygon[0],//���_���̐擪�A�h���X
			//sizeof(VERTEX_2D));//���_���\���̂̃T�C�Y
	}



	
	
}

CharacterNum* GetCharacterNum(void)
{
	return &g_aCharacterNum;
}