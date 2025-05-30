//=======================================================================
//
// �^�C�}�[�̏���
// Author : ShinaTaiyo
//
//=======================================================================

//=======================================================================
//include
//=======================================================================
#include "timer.h"//�^�C�}�[

//=======================================================================
//�O���[�o���ϐ�
//=======================================================================
LPDIRECT3DTEXTURE9 g_pTextureTimer = NULL;      //�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTimer = NULL;	//���_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_posTimer;							//�^�C�}�[�̈ʒu
Timer g_aTimer[MAX_TIMER];						//�^�C�}�[�̏��
int g_MathTimer = 0;							//���݂̃^�C�}�[
int g_KeepNumTimer = 0;							//����ۑ�����p
int g_nTimer[MAX_TIMER] = {};					//�^�C�}�[�̊e���ۑ��p
float g_fanimTimer = 0.0f;						//�^�C�}�[�̃e�N�X�`�����W
int g_nDelayTimer = 0;							//�t���[�����J�E���g

//=======================================================================
//�^�C�}�[�̏���������
//=======================================================================
void InitTimer(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntTimer; //�����J�E���g����

	g_nDelayTimer = 0;

	g_fanimTimer = 0.0f;//�^�C�}�[�̃e�N�X�`�����W

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\timer000.png", &g_pTextureTimer);

	//�X�R�A�̈ʒu�̏�����
	g_aTimer[0].pos = D3DXVECTOR3(TIMER_POSX, TIMER_POSY, 0.0f);
	g_aTimer[1].pos = D3DXVECTOR3(TIMER_POSX - 40.0f, TIMER_POSY, 0.0f);
	g_aTimer[2].pos = D3DXVECTOR3(TIMER_POSX - 80.0f, TIMER_POSY, 0.0f);

	//�^�C�}�[�̏��̏�����
	for (nCntTimer = 0; nCntTimer < MAX_TIMER; nCntTimer++)
	{
		g_aTimer[nCntTimer].bUse = true;                       //�g�p���Ă��Ȃ����
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TIMER, D3DUSAGE_WRITEONLY,FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffTimer, NULL);

#ifdef _DEBUG
	g_MathTimer = MAX_TIME;	//���݂̃^�C�}�[
#else
	g_MathTimer = MAX_TIME;	//���݂̃^�C�}�[
#endif // _DEBUG

	//���_���̃|�C���^
	VERTEX_2D* pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);

	g_KeepNumTimer = g_MathTimer;

	for (nCntTimer = 0; nCntTimer < MAX_TIMER; nCntTimer++)
	{
		g_nTimer[nCntTimer] = g_KeepNumTimer % 10;
		g_KeepNumTimer /= 10;

		switch (g_nTimer[nCntTimer])
		{
		case 0:
			g_fanimTimer = 0.0f;//�^�C�}�[�̃e�N�X�`�����W
			break;
		case 1:
			g_fanimTimer = 0.1f;//�^�C�}�[�̃e�N�X�`�����W
			break;
		case 2:
			g_fanimTimer = 0.2f;//�^�C�}�[�̃e�N�X�`�����W
			break;
		case 3:
			g_fanimTimer = 0.3f;//�^�C�}�[�̃e�N�X�`�����W
			break;
		case 4:
			g_fanimTimer = 0.4f;//�^�C�}�[�̃e�N�X�`�����W
			break;
		case 5:
			g_fanimTimer = 0.5f;//�^�C�}�[�̃e�N�X�`�����W
			break;
		case 6:
			g_fanimTimer = 0.6f;//�^�C�}�[�̃e�N�X�`�����W
			break;
		case 7:
			g_fanimTimer = 0.7f;//�^�C�}�[�̃e�N�X�`�����W
			break;
		case 8:
			g_fanimTimer = 0.8f;//�^�C�}�[�̃e�N�X�`�����W
			break;
		case 9:
			g_fanimTimer = 0.9f;//�^�C�}�[�̃e�N�X�`�����W
			break;
		}

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aTimer[nCntTimer].pos.x - 25.0f, g_aTimer[nCntTimer].pos.y - 25.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aTimer[nCntTimer].pos.x + 25.0f, g_aTimer[nCntTimer].pos.y - 25.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aTimer[nCntTimer].pos.x - 25.0f, g_aTimer[nCntTimer].pos.y + 25.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aTimer[nCntTimer].pos.x + 25.0f, g_aTimer[nCntTimer].pos.y + 25.0f, 0.0f);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(g_fanimTimer, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(g_fanimTimer + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(g_fanimTimer, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(g_fanimTimer + 0.1f, 1.0f);

		//���_�f�[�^�̃|�C���^��4���i�߂�
		pVtx += 4;
	}

	//���_�o�b�t�@�̃A�����b�N
	g_pVtxBuffTimer->Unlock();
}

//=======================================================================
//�^�C�}�[�̏I������
//=======================================================================
void UninitTimer(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureTimer != NULL)
	{
		g_pTextureTimer->Release();
		g_pTextureTimer = NULL;
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffTimer != NULL)
	{
		g_pVtxBuffTimer->Release();
		g_pVtxBuffTimer = NULL;
	}
}

//=======================================================================
//�^�C�}�[�̍X�V����
//=======================================================================
void UpdateTimer(void)
{
	int nCntTimer;
	VERTEX_2D* pVtx;  //���_���̃|�C���^

	g_nDelayTimer++;

	if (g_nDelayTimer % 60 == 0)
	{
		SubTimer(1);//�U�O�t���[�����ƂɃ^�C�}�[���P�b���炷
	}

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);

	g_KeepNumTimer = g_MathTimer;

	for (nCntTimer = 0; nCntTimer < MAX_TIMER; nCntTimer++)
	{
		g_nTimer[nCntTimer] = g_KeepNumTimer % 10;
		g_KeepNumTimer /= 10;

		switch (g_nTimer[nCntTimer])
		{
		case 0:
			g_fanimTimer = 0.0f;//�^�C�}�[�̃e�N�X�`�����W
			break;
		case 1:
			g_fanimTimer = 0.1f;//�^�C�}�[�̃e�N�X�`�����W
			break;
		case 2:
			g_fanimTimer = 0.2f;//�^�C�}�[�̃e�N�X�`�����W
			break;
		case 3:
			g_fanimTimer = 0.3f;//�^�C�}�[�̃e�N�X�`�����W
			break;
		case 4:
			g_fanimTimer = 0.4f;//�^�C�}�[�̃e�N�X�`�����W
			break;
		case 5:
			g_fanimTimer = 0.5f;//�^�C�}�[�̃e�N�X�`�����W
			break;
		case 6:
			g_fanimTimer = 0.6f;//�^�C�}�[�̃e�N�X�`�����W
			break;
		case 7:
			g_fanimTimer = 0.7f;//�^�C�}�[�̃e�N�X�`�����W
			break;
		case 8:
			g_fanimTimer = 0.8f;//�^�C�}�[�̃e�N�X�`�����W
			break;
		case 9:
			g_fanimTimer = 0.9f;//�^�C�}�[�̃e�N�X�`�����W
			break;
		}

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(g_fanimTimer, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(g_fanimTimer + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(g_fanimTimer, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(g_fanimTimer + 0.1f, 1.0f);

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTimer->Unlock();
}

//=======================================================================
//�^�C�}�[�̕`�揈��
//=======================================================================
void DrawTimer(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntTimer;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTimer, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureTimer);

	for (nCntTimer = 0; nCntTimer < MAX_TIMER; nCntTimer++)
	{
		if (g_aTimer[nCntTimer].bUse == true)
		{//�X�R�A���g�p����Ă���
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTimer * 4, 2);
		}
	}
}

//=======================================================================
//�^�C�}�[�̌��Z����
//=======================================================================
void SubTimer(int nValue)
{
	//�^�C�}�[����b���炷
	g_MathTimer -= nValue;
}

//=======================================================================
//�^�C�}�[��0�ɂȂ������ǂ����𒲂ׂ鏈��
//=======================================================================
bool CheckTime(void)
{
	if (g_MathTimer <= 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//===================================
//���݂̎c�莞�Ԃ��擾����
//===================================
int GetTimer(void)//���݂̎c�莞�Ԃ��擾����
{
	return g_MathTimer;
}
