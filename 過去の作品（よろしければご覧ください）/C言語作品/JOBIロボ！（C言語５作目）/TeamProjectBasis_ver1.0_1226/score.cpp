//========================================================
//
//�X�R�A�̏���
//Author:ShinaTaiyo
//
//=========================================================
#include "main.h"
#include "score.h"
#include "input.h"
#include "player.h"
#include "result.h"

//=========================================================
//�O���[�o���ϐ�
//=========================================================
LPDIRECT3DTEXTURE9 g_pTextureScore= NULL;//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;//�e�̏��
Score g_aScore[MAX_SCORE];
D3DXVECTOR3 g_posScore;//�ʒu
D3DXVECTOR3 g_moveScore;//�ړ���
int g_nScore=0;
float g_fScoreScale=0;
int g_aPosTexU[MAX_SCORE];//�e���̐������i�[
int g_nSaveScore=0;
int g_mode;

//=========================================================
//����������
//=========================================================
void InitScore(void)
{
	g_nScore = 0;
	LPDIRECT3DDEVICE9 pDevice;
	int	nCntScore;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

    D3DXCreateTextureFromFile(pDevice,
	"data\\TEXTURE\\timer000.png",
	&g_pTextureScore);

	//�e�N�X�`���̓ǂݍ��݁i�X�R�A�̃e�N�X�`����ǂݍ��ށj
	for (nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{
		g_aPosTexU[nCntScore] = 0;
    }

	//�X�R�A���̏�����
	for (nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{
		g_aScore[nCntScore].pos = D3DXVECTOR3(SCORE_POSX - nCntScore * 40.0f, SCORE_POSY, 0.0f);
		g_aScore[nCntScore].bUse = false;//�g�p���Ă��Ȃ���Ԃɂ���
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_SCORE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL);

	VERTEX_2D* pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aScore[nCntScore].pos.x - 25.0f, g_aScore[nCntScore].pos.y - 25.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aScore[nCntScore].pos.x + 25.0f, g_aScore[nCntScore].pos.y - 25.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aScore[nCntScore].pos.x - 25.0f, g_aScore[nCntScore].pos.y + 25.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aScore[nCntScore].pos.x + 25.0f, g_aScore[nCntScore].pos.y + 25.0f, 0.0f);

		g_aScore[nCntScore].bUse = true;//�g�p���Ă����Ԃɂ���

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLOR�́A0.0f�`1.0f�̊Ԃ̐��l�ŐF��ς���B

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;//���_�f�[�^�̃|�C���^���S���i�߂�B
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffScore->Unlock();	
}

//=========================================================
//�I������
//=========================================================
void UninitScore(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}
}

//=========================================================
//�X�V����
//=========================================================
void UpdateScore(void)
{

}

//=========================================================
//�`�揈��
//=========================================================
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

	int nCntScore;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));//���_�o�b�t�@�ւ̃|�C���^�ƒ��_���̍\���̂̃T�C�Y

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureScore);

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntScore * 4, 2);
	}
}

//=========================================================
//�ݒ菈��
//=========================================================
void SetScore(int nScore)
{
	Player* pPlayer = GetPlayer();

	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();

	VERTEX_2D* pVtx;

	int nGetScore = 0;
	g_nScore += (int)((nScore * pPlayer->fCombo));
	nGetScore = g_nScore;//�X�R�A��ۑ�����
	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		g_aPosTexU[nCnt] = nGetScore % 10;
		nGetScore = nGetScore / 10;
	}

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{
		pVtx[0].tex = D3DXVECTOR2(0.0f + (0.1f * g_aPosTexU[nCntScore]), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f * g_aPosTexU[nCntScore]), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (0.1f * g_aPosTexU[nCntScore]), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * g_aPosTexU[nCntScore]), 1.0f);

		pVtx += 4;//���_�f�[�^�̃|�C���^���S���i�߂�B
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffScore->Unlock();
}

//=====================================
//�X�R�A���Z����
//=====================================
void AddScore(int nValue)
{
	SetScore(nValue);
}

//===================================
//�X�R�A�̎擾
//===================================
int GetScore(void)
{
	return g_nScore;
}