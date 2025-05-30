//==============================================
//
//�V���P�W���F�e�N�X�`���̉�][polygon2.cpp]
//Author:ShinaTaiyo
//
//==============================================

#include "main.h"
#include "polygon2.h"//�쐬����polygon2.h���C���N���[�h����


//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTexturePolygon_2 = NULL;//�e�N�X�`���ւ̃|�C���^
//VERTEX_2D g_aVertex[4];//���_�����i�[�i�l�p�`����肽���j
PDIRECT3DVERTEXBUFFER9 g_pVtxBuffPolygon_2 = NULL;//���_�o�b�t�@�ւ̃|�C���^

bool g_bTurnPoint_2 = false;//�E�C���h�E�̒[�ɃL���������B�����ꍇ�A���������邽�߂̃t���O
bool g_bTurnPoint2_2 = false;
float g_fXmove_2 = 0;//X�����ɓ����������̕ϐ�
float g_fYmove_2 = 0;//Y�����ɓ����������̕ϐ�
bool g_bColorFlag_2 = true;//�F��ς��鎞�̐��l�̑������Ǘ�����ϐ�
//float g_fEnemyMove1[4];

int g_nColorCount1_2 = 0, g_nColorCount2_2 = 0, g_nColorCount3_2 = 0, g_nColorCount4_2 = 0;//�펞�F��ς�������ϐ�


//=======================================================
//�|���S���̏���������
//=======================================================
void InitPolygon_2(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	//D3DXCreateTextureFromFile(pDevice,
	//	"data\\bg100.jpg",
	//	&g_pTexturePolygon_2);

	/*D3DXCreateTextureFromFile(pDevice,
		"data\\sword_kirby.png",
		&g_pTexturePolygon_2);*/

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D_2) * 4,//���_�̐�
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPolygon_2,
		NULL);

	VERTEX_2D_2* pVtx_2;
	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPolygon_2->Lock(0, 0, (void**)&pVtx_2, 0);

	//���_���W�̐ݒ�i�����ʒu�j
	pVtx_2[0].pos_2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx_2[1].pos_2 = D3DXVECTOR3(SCREEN_WIDTH,0.0f, 0.0f);
	pVtx_2[2].pos_2 = D3DXVECTOR3(0.0f,SCREEN_HEIGHT, 0.0f);
	pVtx_2[3].pos_2 = D3DXVECTOR3(SCREEN_WIDTH,SCREEN_HEIGHT, 0.0f);




	//rhw�̐ݒ�i�����ݒ�j
	pVtx_2[0].rhw_2 = 1.0f;
	pVtx_2[1].rhw_2 = 1.0f;
	pVtx_2[2].rhw_2 = 1.0f;
	pVtx_2[3].rhw_2 = 1.0f;
		
	 
	//���_�J���[�̐ݒ�i�����ݒ�j
	pVtx_2[0].col_2 = D3DCOLOR_RGBA(0, 0, 0, 0);
	pVtx_2[1].col_2 = D3DCOLOR_RGBA(0, 0, 0, 0);
	pVtx_2[2].col_2 = D3DCOLOR_RGBA(0, 0, 0, 0);
	pVtx_2[3].col_2 = D3DCOLOR_RGBA(0, 0, 0, 0);


	//�e�N�X�`�����W�̐ݒ�i�����ݒ�j
	pVtx_2[0].tex_2 = D3DXVECTOR2(0.0f, 0.0f);
	pVtx_2[1].tex_2 = D3DXVECTOR2(1.0f, 0.0f);
	pVtx_2[2].tex_2 = D3DXVECTOR2(0.0f, 1.0f);
	pVtx_2[3].tex_2 = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPolygon_2->Unlock();
}

//========================================
//�|���S���̏I������
//========================================
void UninitPolygon_2(void)
{
	//�e�N�X�`���̔j��
	if (g_pTexturePolygon_2 != NULL)
	{
		g_pTexturePolygon_2->Release();
		g_pTexturePolygon_2 = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffPolygon_2 != NULL)
	{
		g_pVtxBuffPolygon_2->Release();
		g_pVtxBuffPolygon_2 = NULL;
	}
}
//==========================================================
//�|���S���̍X�V����
//==========================================================
void UpDatePolygon_2(void)//�P�t���[�����Ƃɏ��������B
{
	//float fEnemyMove1[4];
	//float *pMove;

	//pMove = fEnemyMove1;
 //   
	//pMove[0] = 0.0f + g_fXmove_2;
	//fEnemyMove1[1] = 1280.0f + g_fXmove_2;
	//fEnemyMove1[2] = 0.0f + g_fYmove_2;
	//fEnemyMove1[3] = 720.0f + g_fYmove_2;


	//

	//if (fEnemyMove1[1] >= SCREEN_WIDTH)//��ʉE�[�֍s�����ꍇ
	//{
	//	fEnemyMove1[1] = SCREEN_WIDTH;
	//}
	//if (fEnemyMove1[0] == 0)//��ʍ��[�֍s�����ꍇ
	//{
	//	fEnemyMove1[0] = 0;
	//}

	//if (fEnemyMove1[2] <= 0)//��ʏ�ւ������ꍇ
	//{
	//	fEnemyMove1[2] = 0;
	//}
	//if (fEnemyMove1[3] == SCREEN_HEIGHT)//��ʉ��ւ������ꍇ
	//{
	//	fEnemyMove1[3] = SCREEN_HEIGHT;
	//}
	//


	//if (g_nColorCount1_2 >= 255)
	//{
	//	g_bColorFlag_2 = true;
	//}
	//if (g_nColorCount1_2 <= 0)
	//{
	//	g_bColorFlag_2 = false;
	//}

	//if (g_bColorFlag_2 == true)
	//{
	//	g_nColorCount1_2 -= 10;
	//	g_nColorCount2_2 -= 10;
	//	g_nColorCount3_2 -= 10;
	//	g_nColorCount4_2 -= 10;
	//}
	//if (g_bColorFlag_2 == false)
	//{
	//	g_nColorCount1_2 += 10;
	//	g_nColorCount2_2 += 10;
	//	g_nColorCount3_2 += 10;
	//	g_nColorCount4_2 += 10;
	//}

	//VERTEX_2D_2* pVtx_2;
	////���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	//g_pVtxBuffPolygon_2->Lock(0, 0, (void**)&pVtx_2, 0);

	////���_���W�̐ݒ�
	//pVtx_2[0].pos_2 = D3DXVECTOR3(fEnemyMove1[0], fEnemyMove1[2], 0.0f);
	//pVtx_2[1].pos_2 = D3DXVECTOR3(fEnemyMove1[1], fEnemyMove1[2], 0.0f);
	//pVtx_2[2].pos_2 = D3DXVECTOR3(fEnemyMove1[0], fEnemyMove1[3], 0.0f);
	//pVtx_2[3].pos_2 = D3DXVECTOR3(fEnemyMove1[1], fEnemyMove1[3], 0.0f);

	////rhw�̐ݒ�
	//pVtx_2[0].rhw_2 = 1.0f;
	//pVtx_2[1].rhw_2 = 1.0f;
	//pVtx_2[2].rhw_2 = 1.0f;
	//pVtx_2[3].rhw_2 = 1.0f;
	//
	////���_�J���[�̐ݒ�
	///*pVtx_2[0].col_2 = D3DCOLOR_RGBA(g_nColorCount1_2, 255, 255, 255);
	//pVtx_2[1].col_2 = D3DCOLOR_RGBA(255, g_nColorCount2_2, 255, 255);
	//pVtx_2[2].col_2 = D3DCOLOR_RGBA(255, 255, g_nColorCount3_2, 255);
	//pVtx_2[3].col_2 = D3DCOLOR_RGBA(g_nColorCount4_2, 255, g_nColorCount4_2, 255);*/

	//pVtx_2[0].col_2 = D3DCOLOR_RGBA(255, 255, 255, 255);
	//pVtx_2[1].col_2 = D3DCOLOR_RGBA(255, 255, 255, 255);
	//pVtx_2[2].col_2 = D3DCOLOR_RGBA(255, 255, 255, 255);
	//pVtx_2[3].col_2 = D3DCOLOR_RGBA(255, 255, 255, 255);


	////�e�N�X�`�����W�̐ݒ�
	//pVtx_2[0].tex_2 = D3DXVECTOR2(0.0f, 0.0f);
	//pVtx_2[1].tex_2 = D3DXVECTOR2(1.0f, 0.0f);
	//pVtx_2[2].tex_2 = D3DXVECTOR2(0.0f, 1.0f);
	//pVtx_2[3].tex_2 = D3DXVECTOR2(1.0f, 1.0f);

	////���_�o�b�t�@���A�����b�N����
	//g_pVtxBuffPolygon_2->Unlock();
}
//==========================================================
//�|���S���̕`�揈��
//==========================================================
void DrawPolygon_2(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPolygon_2, 0, sizeof(VERTEX_2D_2));//���_�o�b�t�@�ւ̃|�C���^�ƒ��_���̍\���̂̃T�C�Y

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTexturePolygon_2);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);//0�́A�`�悷��ŏ��̒��_�C���f�b�N�X�ŁA2�́A�`�悷��v���~�e�B�u��
		//&g_pVtxBuffPolygon[0],//���_���̐擪�A�h���X
		//sizeof(VERTEX_2D));//���_���\���̂̃T�C�Y
}
