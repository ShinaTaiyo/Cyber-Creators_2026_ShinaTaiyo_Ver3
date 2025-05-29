//========================================================
//
//�W���R�O���F�G�t�F�N�g������񌩒���[explosion.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#include "main.h"
#include "explosion.h"
#include "bullet.h"
#include "player.h"
#include "enemy.h"
#include "health.h"
#include "sound.h"


//�}�N����`
#define MAX_EXPLOSION (10000)//�����̍ő吔
#define EXPLOSION_TEXTURE "data\\TEXTURE\\explosion_002.png"
#define EXPLOSION_SCALE (20)
#define EXPLOSION_SCALE_NOVA (300.0f)//�X�L���u�m���@�u���X�g�v�̔����͈�
#define EXPLOSION_SCALE_E6 (100.0f)//�G�U�̍U���̔����̑傫��

//�����\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;//�ʒu
	D3DXCOLOR col;//�F
	int nCounterAnim;//�A�j���[�V�����J�E���^�[
	int nPatternAnim;//�A�j���[�V�����p�^�[��No
	float fPatternAnimY;//�e�N�X�`����Y���W�����炷�B
	bool bUse;//�g�p���Ă��邩�ǂ���
	int nType;//�����̃^�C�v�i��ށj
	float fWidth;//��
	float fHeight;//����
	int nCntTime;//����
	
}Explosion;

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureExplosion = NULL;//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL;//���_�o�b�t�@�ւ̃|�C���^
Explosion g_aExplosion[MAX_EXPLOSION];//�����̏��

//�����̏���������
void InitExplosion(void)
{
	
	LPDIRECT3DDEVICE9 pDevice;
	int	nCntExplosion;


	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ��݁i�e�̃e�N�X�`����ǂݍ��ށj
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\explosion_002.png",
		&g_pTextureExplosion);

	//�������̏�����
	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		/*g_aExplosion[nCntExplosion].pos = D3DXVECTOR3(600.0f, 300.0f, 0.0f); */
		g_aExplosion[nCntExplosion].bUse = false;//�g�p���Ă��Ȃ���Ԃɂ���
		g_aExplosion[nCntExplosion].fWidth = 0.0f;//��
		g_aExplosion[nCntExplosion].fHeight = 0.0f;//����
		g_aExplosion[nCntExplosion].nCntTime = 0;
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EXPLOSION,//�S�͕K�v�Ȓ��_��
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffExplosion,
		NULL);



	VERTEX_2D* pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		//���_���W�̐ݒ�(�e�̑傫���j
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(40.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 40.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(40.0f, 40.0f, 0.0f);

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

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.125f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 0.5f);
		pVtx[3].tex = D3DXVECTOR2(0.125f, 0.5f);

		pVtx += 4;//���_�f�[�^�̃|�C���^���S���i�߂�B
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffExplosion->Unlock();
}

//�����̏I������
void UninitExplosion(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureExplosion != NULL)
	{
		g_pTextureExplosion->Release();
		g_pTextureExplosion = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}
}

//�����̍X�V����
void UpdateExplosion(void)
{
	int nCntExplosion;
	int nCntEnemy;
	int nCntBullet;
	
	Bullet* pBullet = GetBullet();
	
	Player* pPlayer = GetPlayer();

	VERTEX_2D* pVtx;
	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	
	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION;nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == true)
		{//�������g�p����Ă���itrue)�̊Ԃ́A�����Ə��������
   			g_aExplosion[nCntExplosion].nCounterAnim++;//�J�E���^��i�߂ăp�^�[��No���X�V

			if (g_aExplosion[nCntExplosion].nCounterAnim%5==0)
			{
				g_aExplosion[nCntExplosion].nPatternAnim++;//�P�O���̂P�̑��x�ŃA�j���[�V������i�߂�
			}


			if (g_aExplosion[nCntExplosion].nType == EXPLOSIONTYPE_NOVA)
			{

	/*			for (nCntBullet = 0; nCntBullet < MAX_ENEMY; nCntBullet++, pBullet++)
				{
					
					if(pBullet->bUse==true)
					{*/
							if (pPlayer->pos.x >= g_aExplosion[nCntExplosion].pos.x - EXPLOSION_SCALE_NOVA && pPlayer->pos.x <= g_aExplosion[nCntExplosion].pos.x + EXPLOSION_SCALE_NOVA &&
								pPlayer->pos.y >= g_aExplosion[nCntExplosion].pos.y - EXPLOSION_SCALE_NOVA && pPlayer->pos.y <= g_aExplosion[nCntExplosion].pos.y + EXPLOSION_SCALE_NOVA)
							{//�G�ƒe����������
								pBullet->bUse = false;//�����͈͓��̒e�������B
								DamageHealth(10);
							}
						
						
				/*	}
				}*/
				//���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - EXPLOSION_SCALE_NOVA, g_aExplosion[nCntExplosion].pos.y - EXPLOSION_SCALE_NOVA, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + EXPLOSION_SCALE_NOVA, g_aExplosion[nCntExplosion].pos.y - EXPLOSION_SCALE_NOVA, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - EXPLOSION_SCALE_NOVA, g_aExplosion[nCntExplosion].pos.y + EXPLOSION_SCALE_NOVA, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + EXPLOSION_SCALE_NOVA, g_aExplosion[nCntExplosion].pos.y + EXPLOSION_SCALE_NOVA, 0.0f);

			}
            else if (g_aExplosion[nCntExplosion].nType == EXPLOSIONTYPE_E6)
			{
				Player* pPlayer = GetPlayer();
				g_aExplosion[nCntExplosion].nCntTime++;

				if (g_aExplosion[nCntExplosion].nCntTime == 1)
				{
					PlaySound(SOUND_LABEL_SE_EXPLOSION_ENEMY_000);
				}

				//���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - g_aExplosion[nCntExplosion].fWidth, g_aExplosion[nCntExplosion].pos.y - g_aExplosion[nCntExplosion].fHeight, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + g_aExplosion[nCntExplosion].fWidth, g_aExplosion[nCntExplosion].pos.y - g_aExplosion[nCntExplosion].fHeight, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - g_aExplosion[nCntExplosion].fWidth, g_aExplosion[nCntExplosion].pos.y + g_aExplosion[nCntExplosion].fHeight, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + g_aExplosion[nCntExplosion].fWidth, g_aExplosion[nCntExplosion].pos.y + g_aExplosion[nCntExplosion].fHeight, 0.0f);

				if (pPlayer->pos.x > g_aExplosion[nCntExplosion].pos.x - g_aExplosion[nCntExplosion].fWidth &&
					pPlayer->pos.x < g_aExplosion[nCntExplosion].pos.x + g_aExplosion[nCntExplosion].fWidth &&
					pPlayer->pos.y - pPlayer->fHeight / 2 > g_aExplosion[nCntExplosion].pos.y - g_aExplosion[nCntExplosion].fHeight &&
					pPlayer->pos.y - pPlayer->fHeight / 2 < g_aExplosion[nCntExplosion].pos.y + g_aExplosion[nCntExplosion].fHeight)
				{//�v���C���[�̒��S���ǂ̈ʒu�ɂ��邩�Ōv�Z

					DamageHealth(1);

				}
			}

			pVtx[0].tex = D3DXVECTOR2(g_aExplosion[nCntExplosion].nPatternAnim/ 8.0f, 0.0f+g_aExplosion[nCntExplosion].fPatternAnimY);
			pVtx[1].tex = D3DXVECTOR2((g_aExplosion[nCntExplosion].nPatternAnim + 1.0f) / 8.0f, 0.0f + g_aExplosion[nCntExplosion].fPatternAnimY);
			pVtx[2].tex = D3DXVECTOR2(g_aExplosion[nCntExplosion].nPatternAnim / 8.0f, 0.5f + g_aExplosion[nCntExplosion].fPatternAnimY);
			pVtx[3].tex = D3DXVECTOR2((g_aExplosion[nCntExplosion].nPatternAnim+ 1.0f) / 8.0f, 0.5f + g_aExplosion[nCntExplosion].fPatternAnimY);


			
			
			if (g_aExplosion[nCntExplosion].nPatternAnim>8)//
			{
				g_aExplosion[nCntExplosion].fPatternAnimY = 0.5f;
			}
			if (g_aExplosion[nCntExplosion].nPatternAnim > 8&&g_aExplosion[nCntExplosion].fPatternAnimY==0.5f)//
			{
				g_aExplosion[nCntExplosion].bUse = false;//�g�p���Ă��Ȃ���Ԃɂ���
			}

			

		}
		pVtx += 4;//
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffExplosion->Unlock();
}

//�e�̕`�揈��
void DrawExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�ւ̃|�C���^

	int nCntExplosion;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_2D));//���_�o�b�t�@�ւ̃|�C���^�ƒ��_���̍\���̂̃T�C�Y

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureExplosion);



	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == true)
		{//�e���g�p����Ă���
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,//0�́A�`�悷��ŏ��̒��_�C���f�b�N�X�ŁA2�́A�`�悷��v���~�e�B�u��
				nCntExplosion * 4,
				2);
		}

	}


}

//�����̐ݒ菈��
void SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col, int nType, float fWidth, float fHeight)
 {
	VERTEX_2D* pVtx;
 	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	int nCntExplosion;
	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == false)
		{//�������I������Ƃ�
			g_aExplosion[nCntExplosion].pos = pos;
			g_aExplosion[nCntExplosion].col = col;
			g_aExplosion[nCntExplosion].nCounterAnim = 0;//�A�j���[�V�����J�E���^�[�̃��Z�b�g
			g_aExplosion[nCntExplosion].nPatternAnim = 0;//�A�j���[�V�����p�^�[���i�A�j���[�V�����̒x���p�ϐ��j�̃��Z�b�g
			g_aExplosion[nCntExplosion].fPatternAnimY = 0.0f;
			g_aExplosion[nCntExplosion].bUse = true;//�����́Atrue�ł��A���˂��ꂽ�e��������ƁA��������̂ŁAtrue�ɏ��������Ă������Ƃ��K�v
			g_aExplosion[nCntExplosion].nType = nType;//�����̎��
			g_aExplosion[nCntExplosion].fWidth = fWidth;//�����̕�
			g_aExplosion[nCntExplosion].fHeight = fHeight;//�����̍���
			g_aExplosion[nCntExplosion].nCntTime = 0;
			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - EXPLOSION_SCALE, g_aExplosion[nCntExplosion].pos.y - EXPLOSION_SCALE, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + EXPLOSION_SCALE, g_aExplosion[nCntExplosion].pos.y - EXPLOSION_SCALE, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - EXPLOSION_SCALE, g_aExplosion[nCntExplosion].pos.y + EXPLOSION_SCALE, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + EXPLOSION_SCALE, g_aExplosion[nCntExplosion].pos.y + EXPLOSION_SCALE, 0.0f);

			//rhw�̐ݒ�
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;


			//���_�J���[�̐ݒ�
			pVtx[0].col = g_aExplosion[nCntExplosion].col;
			pVtx[1].col = g_aExplosion[nCntExplosion].col;
			pVtx[2].col = g_aExplosion[nCntExplosion].col;
			pVtx[3].col = g_aExplosion[nCntExplosion].col;

			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.125f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 0.5f);
			pVtx[3].tex = D3DXVECTOR2(0.125f, 0.5f);

			break;
		}
		pVtx += 4;
	}
	g_pVtxBuffExplosion->Unlock();
}
