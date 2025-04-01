//====================================================
//
//�T���Q�O���FC++�Ŋ�ՂÂ���[object2d.h]
//Author:ShinaTaiyo
//
//====================================================

//========================
//�C���N���[�h
//========================
#include "object2d.h"
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "debugtext.h"
#include "calculation.h"
//================================================================================

//========================
//�ÓI�����o�ϐ�
//========================

//================================================================================

//========================
//�R���X�g���N�^
//========================
CObject2D::CObject2D(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CObject(nPri,bUseintPri,type,ObjType),
m_nCntBlinkingFrame(0),m_nMaxBlinkingFrame(0),m_bBlinkingAim(false),m_fLimitBlinkingRatio(0.0f),
m_nLife(0),m_nMaxLife(0),m_fRatioLife(0.0f),m_bUseLifeRatioColor(false),m_bUseDraw(true),m_PolygonType(POLYGONTYPE::NORMAL),m_bAnimFlag(false),m_bUse(false),m_bUseBlinking(false),
m_bUseFloating(false),m_bUseLife(false),m_bUseScale(false),m_fAngle(0.0f),m_fAnimationSplit(0.0f),m_fFloatingAddSpeed(0.0f),m_fFloatingLimitSpeed(0.0f),m_fFloatingRot(0.0f),
m_fFloatingSpeed(0.0f),m_fHeight(0.0f),m_fLength(0.0f),m_fMaxHeight(0.0f),m_fMaxWidth(0.0f),m_fPolygonRotSpeed(0.0f),m_fWidth(0.0f),m_nAnimaionPattern(0),m_nAnimationChange(0),
m_nAnimationCnt(0),m_nMaxAnimationPattern(0),m_nTextureIndex(0),m_pos(D3DXVECTOR3(0.0f,0.0f,0.0f)), m_Move(D3DXVECTOR3(0.0f,0.0f,0.0f)), m_SupportPos(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_col(D3DXCOLOR(1.0f,1.0f,1.0f,1.0f)),
m_pVtxBuff(nullptr),m_pTexture(nullptr),m_Scale(D3DXVECTOR3(1.0f,1.0f,1.0f)),m_rot(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_AddScale(D3DXVECTOR2(0.0f,0.0f)),m_bUseAddScale(false)
{
	m_pTexture = nullptr;
	m_pVtxBuff = nullptr;

	SetObjectType(CObject::OBJECTTYPE::OBJECTTYPE_2D);
}
//================================================================================

//========================
//�f�X�g���N�^
//========================
CObject2D::~CObject2D()
{
}
//================================================================================


//========================
//�����ݒ�
//========================
HRESULT CObject2D::Init()
{
	m_bUse = true;          //�g�p���

	m_pos = D3DXVECTOR3(SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2,0.0f);//�ʒu�i���S�j
	m_Move = D3DXVECTOR3(0.0f,0.0f,0.0f);                        //�ړ���
	m_col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);                      //�F����
	m_SupportPos = D3DXVECTOR3(0.0f,0.0f,0.0f);                  //�x�_
	m_rot = D3DXVECTOR3(0.0f,0.0f,0.0f);                         //����
	m_Scale = D3DXVECTOR2(1.0f, 1.0f);                           //�g�嗦

	VERTEX_2D* pVtx;                                             //2D���_���
	
	CRenderer* pRenderer = CManager::GetRenderer();              //�����_���[�N���X���擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();          //�f�o�C�X���擾

	
	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);


	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�ʒu
	pVtx[0].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	CObject::Init();
	
	return S_OK;
}
//================================================================================


//========================
//�I������
//========================
void CObject2D::Uninit()
{
	//============================
	//���_�o�b�t�@�̔j��
	//============================
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
	//==================================================

	//============================
	//�e�N�X�`���̔j��
	//============================
	if (m_pTexture != nullptr)
	{
		m_pTexture = nullptr;
	}
	//==================================================

	CObject::Uninit();//�I�u�W�F�N�g�̏I������
}
//================================================================================


//========================
//�X�V����
//========================
void CObject2D::Update()
{
	VERTEX_2D* pVtx;
	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�̗͂����炷����
	SubLifeProcess();

	//�g�嗦�̏���
	ScaleProcess();

	//���V�̏���
	FloatingProcess();

	//�_�ł̏���
	BlinkingProcess();

	m_nAnimationCnt++;
	if (m_bAnimFlag == true)
	{//�A�j���[�V����������Ȃ�
		if (m_nAnimationCnt % m_nAnimationChange == 0)
		{//�A�j���[�V������ς���t���[�����ɒB������A�j���[�V�����p�^�[���𑝂₷
			m_nAnimaionPattern++;

			if (m_nAnimaionPattern >= m_nMaxAnimationPattern)
			{//�ő�ɒB�����̂łO�ɖ߂�
				m_nAnimaionPattern = 0;
			}
		}
	}

	m_rot.z += m_fPolygonRotSpeed;//�|���S������]������

	//=============================================
	//�|���S���̏o��������ނɂ���Č��߂�
	//=============================================
	switch (m_PolygonType)
	{
		//���ʃ|���S��
	case POLYGONTYPE::NORMAL:
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y - m_fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y - m_fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y + m_fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y + m_fHeight, 0.0f);
		break;
		//��]�|���S��
	case POLYGONTYPE::SENTERROLLING:
		m_fLength = sqrtf(m_fWidth * m_fWidth + m_fHeight * m_fHeight) / 2.0f;
		m_fAngle = atan2f(m_fWidth, m_fHeight);
		//���_���W�̐ݒ�
		pVtx[0].pos.x = m_pos.x + sinf(m_rot.z - (D3DX_PI - m_fAngle)) * m_fLength;//�ォ�獶�ɂS�T�x
		pVtx[0].pos.y = m_pos.y + cosf(m_rot.z - (D3DX_PI - m_fAngle)) * m_fLength;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = m_pos.x + sinf(m_rot.z + (D3DX_PI - m_fAngle)) * m_fLength;//�ォ��E�ɂS�T�x
		pVtx[1].pos.y = m_pos.y + cosf(m_rot.z + (D3DX_PI - m_fAngle)) * m_fLength;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = m_pos.x + sinf(m_rot.z - m_fAngle) * m_fLength;//�����獶�ɂS�T�x
		pVtx[2].pos.y = m_pos.y + cosf(m_rot.z - m_fAngle) * m_fLength;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = m_pos.x + sinf(m_rot.z + m_fAngle) * m_fLength;//������E�ɂS�T�x
		pVtx[3].pos.y = m_pos.y + cosf(m_rot.z + m_fAngle) * m_fLength;
		pVtx[3].pos.z = 0.0f;
		break;
	case POLYGONTYPE::DOWNSENTERROLLING:
		//���_���W�̐ݒ�
		m_fLength = sqrtf(m_fWidth * m_fWidth + (m_fHeight * 2.0f) * (m_fHeight * 2.0f)) / 2;

		//�Ίp���̊p�x���Z�o����
		m_fAngle = atan2f(m_fWidth,m_fHeight * 2.0f);

		//���_���W�̐ݒ�
		pVtx[0].pos.x = m_pos.x + sinf(m_rot.z - (D3DX_PI - m_fAngle)) * m_fLength;
		pVtx[0].pos.y = m_pos.y + cosf(m_rot.z - (D3DX_PI - m_fAngle)) * m_fLength;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = m_pos.x + sinf(m_rot.z + (D3DX_PI - m_fAngle)) * m_fLength;
		pVtx[1].pos.y = m_pos.y + cosf(m_rot.z + (D3DX_PI - m_fAngle)) * m_fLength;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = m_pos.x + sinf(m_rot.z - (D3DX_PI * 0.5f)) * m_fWidth / 2;
		pVtx[2].pos.y = m_pos.y + cosf(m_rot.z - (D3DX_PI * 0.5f)) * m_fWidth / 2;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = m_pos.x + sinf(m_rot.z + (D3DX_PI * 0.5f)) * m_fWidth / 2;
		pVtx[3].pos.y = m_pos.y + cosf(m_rot.z + (D3DX_PI * 0.5f)) * m_fWidth / 2;
		pVtx[3].pos.z = 0.0f;
		break;
	case POLYGONTYPE::DOWN://���S�_����
		pVtx[0].pos.x = m_pos.x - m_fWidth;
		pVtx[0].pos.y = m_pos.y - m_fHeight;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = m_pos.x + m_fWidth;
		pVtx[1].pos.y = m_pos.y - m_fHeight;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = m_pos.x - m_fWidth;
		pVtx[2].pos.y = m_pos.y;//���_�������ɂ��邽��
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = m_pos.x + m_fWidth;
		pVtx[3].pos.y = m_pos.y;//���_�������ɂ��邽��
		pVtx[3].pos.z = 0.0f;
		break;
	case POLYGONTYPE::LEFT://���S�_������
		pVtx[0].pos = D3DXVECTOR3(m_pos.x, m_pos.y - m_fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y - m_fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x, m_pos.y + m_fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y + m_fHeight, 0.0f);
		break;
	case POLYGONTYPE::RIGHT://���S�_������
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y - m_fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x, m_pos.y - m_fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y + m_fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x, m_pos.y + m_fHeight, 0.0f);
		break;
	default:
		break;
	}
	//========================================================================================================================================
	
	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(m_fAnimationSplit * (float)(m_nAnimaionPattern), 0.0f);
	pVtx[1].tex = D3DXVECTOR2(m_fAnimationSplit + m_fAnimationSplit * (float)(m_nAnimaionPattern), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(m_fAnimationSplit * (float)(m_nAnimaionPattern), 1.0f);
	pVtx[3].tex = D3DXVECTOR2(m_fAnimationSplit + m_fAnimationSplit * (float)(m_nAnimaionPattern), 1.0f);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	//�ʒu�̍X�V
	m_pos += m_Move;

	CObject::Update();
}
//================================================================================


//========================
//�`�揈��
//========================
void CObject2D::Draw()
{
	if (m_bUseDraw == true)
	{
		//�����_���[�N���X�̏����擾
		CRenderer* pRenderer = CManager::GetRenderer();
		//�f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));//���_�o�b�t�@�ւ̃|�C���^�ƒ��_���̍\���̂̃T�C�Y
		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);
		//�e�N�X�`���̐ݒ�h
		pDevice->SetTexture(0, m_pTexture);

		//�A���t�@�e�X�g��L��(�A���t�@�l���O���傫���ꍇ�ɕ`�悷��j
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		//�A���t�@�e�X�g�����ɖ߂�
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	}

	CObject::Draw();//�I�u�W�F�N�g�̕`�揈��
}
//================================================================================

//========================
//���S�t���O�ݒ菈��
//========================
void CObject2D::SetDeath()
{
	CObject::SetDeath();//�I�u�W�F�N�g�̎��S�t���O�ݒ菈��
}
//================================================================================

//=====================================
//�e�N�X�`�����蓖�ď���
//=====================================
void CObject2D::BindTexture(LPDIRECT3DTEXTURE9 pTex)
{
 	m_pTexture = pTex;//�e�N�X�`���A�h���X���i�[
}
//================================================================================

//=====================================
//�F��ݒ肷��
//=====================================
void CObject2D::SetColor(D3DXCOLOR col, bool bAlphaOnly, float fAlpha)
{
	if (bAlphaOnly == false)
	{//�S�̂̐F��ݒ�
		m_col = col;
	}
	else
	{//�����x�����ݒ�
		m_col.a = fAlpha;
	}
}

//=====================================
//�Q�c�I�u�W�F�N�g�̏���ݒ�
//=====================================
void CObject2D::SetAnimInfo(int nMaxAnimationPattern, int nAnimationChange, bool bAnim)
{

	m_nMaxAnimationPattern = nMaxAnimationPattern;             //�A�j���[�V�����̃p�^�[���̍ő吔
	m_nAnimationChange = nAnimationChange;                     //�A�j���[�V�����̍��W��ς���t���[����
	m_fAnimationSplit = 1.0f / m_nMaxAnimationPattern;         //�P����������̃A�j���[�V�����͈�
	m_bAnimFlag = bAnim;                                       //�g�p���
}
//================================================================================

//=====================================
//�e�N�X�`������������
//=====================================
void CObject2D::InitTextureOnly()
{
	if (m_pTexture != nullptr)
	{//�e�N�X�`���ւ̃|�C���^�����݂��Ă�����
		m_pTexture = nullptr;//�|�C���^��������
	}
}
//================================================================================

//======================================================
//�g�嗦�̏���
//======================================================
void CObject2D::ScaleProcess()
{	
	if (m_bUseAddScale == true)
	{
		m_Scale += m_AddScale;//�g�嗦�����Z����
	}

	if (m_bUseScale == true)
	{//�g�嗦���g�p���邩�ǂ���
		m_fWidth = m_fMaxWidth * m_Scale.x;
		m_fHeight = m_fMaxHeight * m_Scale.y;
	}
}
//================================================================================

//======================================================
//���V�̏���
//======================================================
void CObject2D::FloatingProcess()
{
	if (m_bUseFloating == true)
	{
		float fAbsSpeed = 0.0f;//���Z���x�i�[�p
		fAbsSpeed = fabsf(m_fFloatingSpeed);

		if (fAbsSpeed > m_fFloatingLimitSpeed)
		{//���V���x�̍ő�ɒB������ړ��������t�ɂ���
			m_fFloatingAddSpeed *= -1;
		}

      	m_fFloatingSpeed += m_fFloatingAddSpeed;//���x�𑝂₵������
		m_Move.x = sinf(m_fFloatingRot) * m_fFloatingSpeed;
		m_Move.y = cosf(m_fFloatingRot) * m_fFloatingSpeed;
	}
}
//====================================================================================================

//======================================================
//�_�ŏ���
//======================================================
void CObject2D::BlinkingProcess()
{
	if (m_bUseBlinking == true)
	{//�_�ł��邩�ǂ���

		//�_�Ńt���[���ƍő�_�Ńt���[���̊��������A���̒l���㉺�����ē_�ł�\��

		if (m_bBlinkingAim == false)
		{
			m_nCntBlinkingFrame++;
		}
		else
		{
			m_nCntBlinkingFrame--;
		}

		if (m_nCntBlinkingFrame >= m_nMaxBlinkingFrame || m_nCntBlinkingFrame <= 0)
		{
			m_bBlinkingAim = m_bBlinkingAim ? false : true;
		}

		float fRatioLife = 0.0f;
		fRatioLife = float(m_nCntBlinkingFrame) / float(m_nMaxBlinkingFrame);
		m_col.a = fRatioLife;

		if (m_fLimitBlinkingRatio > fRatioLife)
		{
			m_bBlinkingAim = false;
		}
	}
}
//====================================================================================================

//======================================================
//�̗͂����炷����
//======================================================
void CObject2D::SubLifeProcess()
{
	if (m_bUseLife == true)
	{//�̗͂��g�p����
		m_nLife--;//�̗͂������I�Ɍ��炷
		m_fRatioLife = float(m_nLife) / float(m_nMaxLife);//�̗͂̊������v�Z

		if (m_bUseLifeRatioColor == true)
		{//�̗͊����ɉ����ĐF������ς��邩�ǂ���
			LifeRatioColorProcess();
		}

		if (m_nLife <= 0)
		{//���S�t���O��ݒ�
			SetDeath();
		}
	}
}
//====================================================================================================

//======================================================
//�̗͂̊����ŐF�����̓����x��ς��鏈��
//======================================================
void CObject2D::LifeRatioColorProcess()
{
	SetColor(D3DXCOLOR(1.0f,1.0f,1.0f,1.0f), true, m_fRatioLife);
}
//====================================================================================================