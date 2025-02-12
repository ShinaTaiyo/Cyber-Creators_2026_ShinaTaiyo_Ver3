//=============================================================================================
//
//�U���T���F�r���{�[�h���o��[billboard.cpp]
//Author:ShinaTaiyo
//
//=============================================================================================

//==================================================
//�C���N���[�h
//==================================================
#include "billboard.h"
#include "manager.h"
#include "renderer.h"
#include "effect.h"
#include "block.h"
#include "enemy.h"
#include "game.h"
#include "scene.h"
#include "model.h"
#include "objectX.h"
//=============================================================================================

//==================================================
//�ÓI�����o������
//==================================================

//=============================================================================================

CBillboard::CBillboard(int nPriority) : CObject(nPriority)
{
	m_pTexture = nullptr;
	m_pVtxBuff = nullptr;
}

//==================================================
//�f�X�g���N�^
//==================================================
CBillboard::~CBillboard()
{
	m_mtxWorld = {};
	m_pMtxParent = nullptr;
}
//=============================================================================================

//==================================================
//����������
//==================================================
HRESULT CBillboard::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//�f�o�C�X�ւ̃|�C���^�Ǝ擾

	m_nTextureIndex = 0;
	m_Col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);//�F����
	m_fWidth = 0.0f;                          //����
	m_fFormarWidth = 0.0f;                    //���̉���
	m_fHeight = 0.0f;                         //����
	m_fFormarHeight = 0.0f;                   //���̍���

	m_Pos = NULL_VECTOR3;                     //�ʒu
	m_PosOld = NULL_VECTOR3;                  //1f�O�̈ʒu
	m_SupportPos = NULL_VECTOR3;              //�����ʒu
	m_Rot = NULL_VECTOR3;                     //����
	m_Move = NULL_VECTOR3;                    //�ړ���
	m_mtxWorld = {};                          //�}�g���b�N�X���[���h
	m_nTextureIndex = 0;                      //�e�N�X�`���ԍ�

	m_nAnimaionPattern = 0;                   //�A�j���[�V�����p�^�[��
	m_nAnimationChange = 0;                   //�A�j���[�V������ς���J�E���g��
	m_fAnimationSplit = 0.0f;                 //�ʒu����������̃A�j���[�V�����͈̔�
	m_nMaxAnimationPattern = 0;               //�A�j���[�V�����p�^�[���ő吔
	m_nAnimationCnt = 0;                      //�A�j���[�V�����J�E���g

	m_fScale = 1.0f;                          //�g�嗦
	m_bScaling = false;                       //�g�k�����邩�ǂ���
	m_fAddScale = 0.0f;                       //�g���

	m_fCurveSpeed = 0.0f;                     //�J�[�u���鑬��
	m_bUseCurve = false;                      //�J�[�u�����邩�ǂ���
	m_fSupportCurveLength = 0.0f;             //���_�����ɉ�]�����鋗��
	m_bUseLengthCurve = false;                //���_�����ɉ�]�����邩�ǂ���
	m_fAddCurveLength = 0.0f;
	m_fSpeed = 0.0f;                          //����
	m_fAddRot = 0.0f;                         //�����̕␳
	m_fStartRot = 0.0f;                       //�ŏ��̌���

	m_bSetEffect = false;                     //�G�t�F�N�g���g�p���邩�ǂ���
	m_SetEffectCol = NORMAL_COL;              //�ݒ肷��G�t�F�N�g�̐F����
	m_nSetEffectLife = 0;                     //�ݒ肷��G�t�F�N�g�̗̑�
	m_SetEffectSize = D3DXVECTOR2(0.0f, 0.0f);//�ݒ肷��G�t�F�N�g�̃T�C�Y

	m_bMtxChild = false;                      //�q�}�g���b�N�X�ɂ��邩�ǂ���
	m_pMtxParent = nullptr;                   //�e�̓G�ւ̃|�C���^
	m_TransPos = NULL_VECTOR3;                //�q�}�g���b�N�X���g�p���Ă���ꍇ�̃��[���h���W

	m_bUseGravity = false;                    //�d�͂��g�p���邩�ǂ���
	m_fGravityPower = 0.0f;                   //�d�͂̑傫��
	m_fAddGravity = 0.0f;                     //���Z����d��

	m_bUseHorming = false;                    //�z�[�~���O���g�p���邩�ǂ���
	m_fRotMove = 0.0f;                        //�����ւ̈ړ���
	m_nCntTime = 0;                           //�o�����Ă���̎��Ԃ��J�E���g����

	m_bUseAddSpeed = false;                   //�������g�p���邩�ǂ���
	m_bMultiplication = false;                //���x����Z���邩�ǂ���
	m_fAddSpeed = 0.0f;                       //�����x

	m_nLife = 0;                              //�̗�
	m_nMaxLife = 0;                           //�ő�̗�

	//============================================
	//�`��֌W
	//============================================
	m_bDraw = true;                           //�`�悷�邩�ǂ���
	//==============================================================================================

	//============================================
	//���_�o�b�t�@�̐���
	//============================================
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,//���_�̐�
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);
	//==============================================================================================

	VERTEX_3D* pVtx;
	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

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

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	return S_OK;
}
//=============================================================================================

//==================================================
//�I������
//==================================================
void CBillboard::Uninit(void)
{
	//�e�N�X�`���̃|�C���^��������
	if (m_pTexture != nullptr)
	{
		m_pTexture = nullptr;
	}

	//���_�o�b�t�@�̔j��
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
}
//=============================================================================================

//==================================================
//�X�V����
//==================================================
void CBillboard::Update(void)
{
	VERTEX_3D* pVtx;
	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	m_nAnimationCnt++;
	m_nCntTime++;        //�o�����Ă���̎��Ԃ��J�E���g����
	m_nLife--;           //�̗͂����炵������

	//================================
    //�g�傪ON�ɂȂ��Ă�����
    //================================
	if (m_bScaling == true)
	{
		m_fScale += m_fAddScale;//�g�嗦�𑝂₷
		m_fWidth = m_fFormarWidth * m_fScale;
		m_fHeight = m_fFormarHeight * m_fScale;
	}
	//=====================================================

	//========================================
	//�J�[�u��ON��������
	//========================================
	if (m_bUseCurve == true)
	{
		m_fAddRot += m_fCurveSpeed;
		m_Pos.x += sinf(m_fAddRot + m_fStartRot) * m_fSpeed;
		m_Pos.y += cosf(m_fAddRot + m_fStartRot) * m_fSpeed;
	}
	//=============================================================================================

	//========================================
	//�x�_�����ɃJ�[�u��ON��������
	//========================================
	if (m_bUseLengthCurve == true)
	{
		m_fAddRot += m_fCurveSpeed;
		m_fSupportCurveLength += m_fAddCurveLength;
		m_Pos.x = sinf(m_fAddRot + m_fStartRot) * m_fSupportCurveLength + m_SupportPos.x;
		m_Pos.y = cosf(m_fAddRot + m_fStartRot) * m_fSupportCurveLength + m_SupportPos.y;
	}
	//=============================================================================================

	//================================
	//�G�t�F�N�g��ON�ɂȂ��Ă�����
	//================================
	if (m_bSetEffect == true)
	{
		if (m_bMtxChild == false)
		{
			CEffect::Create(CEffect::EFFECTTYPE00_NORMAL,m_nSetEffectLife, m_SetEffectSize.x, m_SetEffectSize.y, m_Pos, m_SetEffectCol);
		}
		else
		{//�����̃}�g���b�N�X�̈ʒu���Q�Ƃ��ăG�t�F�N�g���o����������
			CEffect::Create(CEffect::EFFECTTYPE00_NORMAL, m_nSetEffectLife, m_SetEffectSize.x, m_SetEffectSize.y, m_TransPos, m_SetEffectCol);
		}
	}
	//==============================================================================================

	//========================================
	//�d�͂�ON�ɂȂ��Ă�����
	//========================================
	if (m_bUseGravity == true)
	{
		m_fGravityPower += m_fAddGravity;
		m_Move.y += m_fGravityPower;
	}
	//==============================================================================================

	//========================================
	//�z�[�~���O��ON�ɂȂ��Ă�����
	//========================================
	if (m_bUseHorming == true)
	{
		HormingProcess();//�z�[�~���O�̏���
	}
	//==============================================================================================

	//========================================
	//�������g�p���邩�ǂ���
	//========================================
	if (m_bUseAddSpeed == true)
	{
		if (m_bMultiplication == false)
		{//��ZOFF
			m_Move += D3DXVECTOR3(m_fAddSpeed,m_fAddSpeed,0.0f);
		}
		else
		{//��ZON
			m_Move *= m_fAddSpeed;
		}
	}
	//==============================================================================================


	//========================================
	//�ړ��ʂ̍X�V
	//========================================
	m_PosOld = m_Pos;
	m_Pos += m_Move;
	//==============================================================================================

	//========================================
	//�e�I�u�W�F�N�gX�֌W
	//========================================
	if (m_pMtxParent != nullptr)
	{

	}
	//==============================================================================================

	//========================================
	//�|���S����]����
	//========================================
	if (m_bUsePolygonRot == true)
	{
		m_Rot.z += m_fPolygonRotPower;
	}
	//==============================================================================================

	if (m_bAnimFlag == true)
	{//�A�j���[�V����������Ȃ�
		if (m_nAnimationCnt % m_nAnimationChange == 0)
		{
			m_nAnimaionPattern++;

			if (m_nAnimaionPattern >= m_nMaxAnimationPattern)
			{
				m_nAnimaionPattern = 0;
			}
		}
	}

	float fLength = sqrtf(m_fWidth * m_fWidth + m_fHeight * m_fHeight) / 2.0f;
	float fAngle = atan2f(m_fWidth, m_fHeight);

	//���_���W�̐ݒ�
	pVtx[0].pos.x = sinf(m_Rot.z - fAngle) * fLength;//�ォ�獶�ɂS�T�x
	pVtx[0].pos.y = cosf(m_Rot.z - fAngle) * fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = sinf(m_Rot.z + fAngle) * fLength;//�ォ��E�ɂS�T�x
	pVtx[1].pos.y = cosf(m_Rot.z + fAngle) * fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = sinf(m_Rot.z - (D3DX_PI - fAngle)) * fLength;//�ォ�獶�ɂS�T�x
	pVtx[2].pos.y = cosf(m_Rot.z - (D3DX_PI - fAngle)) * fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = sinf(m_Rot.z + (D3DX_PI - fAngle)) * fLength;//�ォ��E�ɂS�T�x
	pVtx[3].pos.y = cosf(m_Rot.z + (D3DX_PI - fAngle)) * fLength;
	pVtx[3].pos.z = 0.0f;

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(m_fAnimationSplit * (float)(m_nAnimaionPattern), 0.0f);
	pVtx[1].tex = D3DXVECTOR2(m_fAnimationSplit + m_fAnimationSplit * (float)(m_nAnimaionPattern), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(m_fAnimationSplit * (float)(m_nAnimaionPattern), 1.0f);
	pVtx[3].tex = D3DXVECTOR2(m_fAnimationSplit + m_fAnimationSplit * (float)(m_nAnimaionPattern), 1.0f);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	if (m_nLife <= 0)
	{
		SetDeath();
	}

}
//=============================================================================================

//==================================================
//�`�揈��
//==================================================
void CBillboard::Draw(void)
{
	D3DXMATRIX mtxTrans,mtxRot;//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxParent = {};
	D3DXMATRIX mtxView;//�r���[�}�g���b�N�X�擾�p
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	if (m_bMtxChild == true && m_pMtxParent != nullptr)
	{
		mtxParent = *m_pMtxParent;
	}

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�r���[�}�g���b�N�X���擾
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//�|���S�����J�����ɑ΂��Đ��ʂɌ�����
	D3DXMatrixInverse(&m_mtxWorld, NULL, &mtxView);

	//�t�s������߂�
	m_mtxWorld._41 = 0.0f;
	m_mtxWorld._42 = 0.0f;
	m_mtxWorld._43 = 0.0f;


	if (m_pMtxParent == nullptr && m_bMtxChild == false)
	{//�e�ƂȂ郂�f�������Ȃ�������
		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, GetPos().x, GetPos().y, GetPos().z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);
	}
	else
	{//�e�ƂȂ郂�f����������
		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, GetPos().x, GetPos().y, GetPos().z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);//�����̃}�g���b�N�X�ƈʒu���������킹��

		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);//�e�̓G�̃}�g���b�N�X�Ƃ������킹��
	}
	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));//���_�o�b�t�@�ւ̃|�C���^�ƒ��_���̍\���̂̃T�C�Y

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	//================================================
	//�`��̒���
	//================================================
	//���C�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	////Z�̔�r���@�ύX�B�i���f���������Ă��B��Ȃ��Ȃ�j
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	////Z�o�b�t�@�ɏ������܂Ȃ��i�d�Ȃ���Ɉ�a�����Ȃ��Ȃ�j
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//�A���t�@�e�X�g��L��(�A���t�@�l���O���傫���ꍇ�ɕ`�悷��j
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//=======================================================================================================================================
	
	if (m_bDraw == true)
	{
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}

	D3DXVECTOR3 PosZero = D3DXVECTOR3(0.0f,0.0f,0.0f);
	D3DXVECTOR3 PosBullet;
	//if (m_bMtxChild == true)
	//{//�q�}�g���b�N�X���g�p����ꍇ�ɁA���[���h���W�𔻒肷��
		D3DXVec3TransformCoord(&PosBullet, &PosZero, &m_mtxWorld);//���̈����̃��[�J�����W�ɐ^�񒆂̈����̃��[���h���W��������
		m_TransPos = PosBullet;
	//}
	//================================================
	//�`��̒��������ɖ߂�
	//================================================

	//���C�g��L���ɖ߂�
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	////Z�̔�r���@�ύX�B
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	////Z�o�b�t�@�ɏ�������
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//�A���t�@�e�X�g�����ɖ߂�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//=======================================================================================================================================
}
//=============================================================================================

//==================================================
//�e�N�X�`�������蓖�Ă鏈��
//==================================================
void CBillboard::bindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}
//============================================================================================================================================

//==================================================
//�����`�̓����蔻������
//==================================================
bool CBillboard::CollisionSquare(D3DXVECTOR3 pos, D3DXVECTOR3 VtxMax, D3DXVECTOR3 VtxMin)
{
	D3DXVECTOR3 ComparisonPos = NULL_VECTOR3;
	D3DXVECTOR3 ComparisonVtxMin = NULL_VECTOR3;
	D3DXVECTOR3 ComparisonVtxMax = NULL_VECTOR3;

	ComparisonPos.x = (float)(floor(pos.x));
	ComparisonPos.y = (float)(floor(pos.y));
	ComparisonPos.z = (float)(floor(pos.z));

	ComparisonVtxMax.x = (float)(floor(VtxMax.x));
	ComparisonVtxMax.y = (float)(floor(VtxMax.y));
	ComparisonVtxMax.z = (float)(floor(VtxMax.z));

	ComparisonVtxMin.x = (float)(floor(VtxMin.x));
	ComparisonVtxMin.y = (float)(floor(VtxMin.y));
	ComparisonVtxMin.z = (float)(floor(VtxMin.z));


	D3DXVECTOR2 MySize = D3DXVECTOR2(0.0f,0.0f);

	//==================================
	//����p�ɃL���X�g����
	//==================================
	MySize.x = (float)(floor(m_fWidth));
	MySize.y = (float)(floor(m_fHeight));
	//==========================================================================================================

	bool bCollision = false;                         //�n�ʂɒ��n���Ă��邩
	float fAdjust = 0.001f;
	
	if (m_Pos.x + MySize.x / 2 >= ComparisonPos.x + ComparisonVtxMin.x
		&& m_Pos.x - MySize.x / 2 <= ComparisonPos.x + ComparisonVtxMax.x
		&& m_Pos.y + MySize.y / 2 >= ComparisonPos.y + ComparisonVtxMin.y
		&& m_Pos.y - MySize.y / 2 <= ComparisonPos.y + ComparisonVtxMax.y
		&& m_Pos.z + MySize.x / 2 >= ComparisonPos.z + ComparisonVtxMin.z
		&& m_Pos.z - MySize.x / 2 <= ComparisonPos.z + ComparisonVtxMax.z)
	{//�Ώۂ̉��[�����f����+Y�ɓ����������̏���
		bCollision = true;
	}
	return bCollision;
}

//==================================================
//�T�C�Y��ݒ肷��
//==================================================
void CBillboard::SetSize(float fWidth, float fHeight)
{
	m_fWidth = fWidth;
	m_fHeight = fHeight;
}
//============================================================================================================================================

//==========================================================================
//���̃T�C�Y��ݒ肷��
//==========================================================================
void CBillboard::SetFormarSize(float fFormarWidth, float fFormarHeight)
{
	m_fFormarWidth = fFormarWidth;
	m_fFormarHeight = fFormarHeight;
}
//============================================================================================================================================

//==================================================
//�T�C�Y��ݒ肷��
//==================================================
void CBillboard::SetColor(D3DXCOLOR col)
{
	VERTEX_3D* pVtx;
	m_Col = col;
	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�i�����ʒu�j
	pVtx[0].col = m_Col;
	pVtx[1].col = m_Col;
	pVtx[2].col = m_Col;
	pVtx[3].col = m_Col;

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

}
//============================================================================================================================================

//=============================================================================================================
//�A�j���[�V�����֌W�̐ݒ�
//=============================================================================================================
void CBillboard::SetAnimInfo(int nMaxAnimationPattern, int nAnimationChange, D3DXCOLOR col, bool bAnim)
{
	m_nMaxAnimationPattern = nMaxAnimationPattern;             //�A�j���[�V�����p�^�[���ő�l
	m_nAnimationChange = nAnimationChange;                     //�A�j���[�V���������ɐi�߂�J�E���g��
	m_Col = col;                                               //�F����
	m_bAnimFlag = bAnim;                                       //�A�j���[�V���������邩�ǂ���
	m_fAnimationSplit = (float)(1.0f / m_nMaxAnimationPattern);//�P����������̃A�j���[�V�����͈�

	VERTEX_3D* pVtx;
    //���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_�J���[�̐ݒ�
	pVtx[0].col = m_Col;
	pVtx[1].col = m_Col;
	pVtx[2].col = m_Col;
	pVtx[3].col = m_Col;

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

}
//================================================================================================================

//===============================================================
//�g�k�̐ݒ�
//===============================================================
void CBillboard::SetScaling(float fAddScale)
{
	m_bScaling = true;//�g�k�����邩�ǂ���
	m_fAddScale = fAddScale;//�g�k��
}
//================================================================================================================

//===============================================================
//�J�[�u��ݒ�
//===============================================================
void CBillboard::SetUseCurve(float fRot, float fSpeed, float fCurveSpeed)
{
	m_bUseCurve = true;
	m_fSpeed = fSpeed;
	m_fStartRot = fRot;
	m_fCurveSpeed = fCurveSpeed;
}
//================================================================================================================

//===============================================================
//���_�����ɉ�]����J�[�u��ݒ�
//===============================================================
void CBillboard::SetUseLengthCurve(bool bUse, float fStartRot, float fCurveSpeed, float fAddCurveLength)
{
	m_bUseLengthCurve = bUse;
	m_fStartRot = fStartRot;
	m_fCurveSpeed = fCurveSpeed;
	m_fAddCurveLength = fAddCurveLength;
}
//================================================================================================================

//===============================================================
//�G�t�F�N�g�̎g�p����ꍇ�̐ݒ�
//===============================================================
void CBillboard::SetUseEffect(int nSetEffectLife, D3DXVECTOR2 Size, D3DXCOLOR Col)
{
	m_bSetEffect = true;                //�G�t�F�N�g��ON�ɂ���
	m_nSetEffectLife = nSetEffectLife;  //�ݒ肷��G�t�F�N�g�̗̑�
	m_SetEffectSize = Size;             //�ݒ肷��G�t�F�N�g�̃T�C�Y
	m_SetEffectCol = Col;               //�ݒ肷��G�t�F�N�g�̐F����
}
//================================================================================================================

//===============================================================
//�q�}�g���b�N�X��ݒ�
//===============================================================
void CBillboard::SetUseMtxChild(D3DXMATRIX* pMtxParent, bool bUse)
{
	m_pMtxParent = pMtxParent;
	m_bMtxChild = true;
}
//================================================================================================================


//===============================================================
//�d�͂�ݒ�
//===============================================================
void CBillboard::SetUseGravity(float fGravity)
{
	m_fGravityPower = fGravity;
	m_bUseGravity = true;
}
//================================================================================================================

//===============================================================
//�z�[�~���O��ݒ�
//===============================================================
void CBillboard::SetUseHorming(float fSpeed)
{
	m_bUseHorming = true;//�z�[�~���O���g�p���邩�ǂ���
	m_fSpeed = fSpeed;   //����
}
//================================================================================================================

//===============================================================
//������ݒ�
//===============================================================
void CBillboard::SetUseAddSpeed(bool bUseAddSpeed, bool bMultiplication, float fAddSpeed)
{
	m_bUseAddSpeed = bUseAddSpeed;
	m_bMultiplication = bMultiplication;
	m_fAddSpeed = fAddSpeed;
}
//================================================================================================================

//===============================================================
//�z�[�~���O�̏���
//===============================================================
void CBillboard::HormingProcess()
{
	float fRotDest = 0.0f;//�ړI�̈ʒu�ւ̊p�x
	float fRotDiff = 0.0f;//���݂̊p�x�ƖړI�̊p�x�̍���
	float fVXaim = 0.0f;  //X�x�N�g��
	float fVYaim = 0.0f;  //Y�x�N�g��
	float fVLaim = 0.0f;  //�����x�N�g��
	D3DXVECTOR3 TargetPos = NULL_VECTOR3;//�^�[�Q�b�g�̈ʒu
	//===================================
	//XZ�x�N�g�����o��
	//===================================
	if (CScene::GetMode() == CScene::MODE_GAME)
	{
		TargetPos = CGame::GetPlayer()->GetPos();
		fVXaim = TargetPos.x - m_Pos.x;
		fVYaim = TargetPos.y - m_Pos.y;

		fVLaim = atan2f(fVXaim, fVYaim);

		//==========================================================================================================================================================

		//if (g_aEnemy[nNumEnemy].nTime == 1)
		//{//�G���o�����ɍŏ��̋O�������߂�i�v���C���[�ƓG�̃x�N�g���Ōv�Z)
		//	g_aEnemy[nNumEnemy].fRotMove = atan2f(g_aEnemy[nNumEnemy].fVXaim, g_aEnemy[nNumEnemy].fVZaim);
		//}

		//�ړI�̈ʒu�ւ̊p�x
		fRotDest = atan2f(TargetPos.x - m_Pos.x, TargetPos.y - m_Pos.y);

		//���݂̈ړ������̊p�x�ƁA�ړI�̈ړ������̊p�x�̍��������߂�
		fRotDiff = fRotDest - m_fRotMove;

		//===============================
		//�p�x�����̏C��
		//===============================
		if (m_fRotMove < 0.0f)
		{//���݂̊p�x���}�C�i�X�Ȃ�
			if ((m_fRotMove + D3DX_PI) <= fRotDest)
			{//���݂̊p�x + D3DX_PI���ړI�̊p�x���傫��������
				fRotDiff -= D3DX_PI * 2.0f;
			}
		}
		if (m_fRotMove > 0.0f)
		{//���݂̊p�x���v���X�Ȃ�
			if ((m_fRotMove - D3DX_PI) >= fRotDest)
			{//���݂̊p�x - D3DX_PI���ړI�̊p�x��������������
				fRotDiff += D3DX_PI * 2.0f;
			}
		}
		//==============================================================================================
		m_fRotMove += fRotDiff * 0.05f;//�ړ������i�p�x�␳�j

		m_Move.x = sinf(m_fRotMove) * m_fSpeed;
		m_Move.y = cosf(m_fRotMove) * m_fSpeed;
	}
}
//================================================================================================================