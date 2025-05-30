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
#include "calculation.h"
//================================================================================

//========================
//�ÓI�����o�ϐ�
//========================

//================================================================================

//========================
//�R���X�g���N�^
//========================
CObject2D::CObject2D(int nPriority) : CObject(nPriority),m_nCntBlinkingFrame(0),m_nMaxBlinkingFrame(0),m_bBlinkingAim(false),m_fLimitBlinkingRatio(0.0f),
m_nLife(0),m_nMaxLife(0),m_fRatioLife(0.0f),m_bUseLifeRatioColor(false),m_bUseDraw(true)
{
	m_pTexture = nullptr;
	m_pVtxBuff = nullptr;
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
	m_nAnimaionPattern = 0; //�A�j���[�V�����p�^�[��
	m_nMaxAnimationPattern = 0;//�ő�A�j���[�V�����p�^�[��
	m_nAnimationCnt = 0;    //�A�j���[�V�����J�E���g
	m_nAnimationChange = 0; //�A�j���[�V������ς���J�E���g��
	m_fAnimationSplit = 0.0f;//�A�j���[�V�����P����������̒l
	m_nTextureIndex = 0;     //�e�N�X�`���ԍ�
	m_fWidth = 0.0f;       //����
	m_fHeight = 0.0f;      //����	
	m_fMaxWidth = 0.0f;    //�ő剡��
	m_fMaxHeight = 0.0f;   //�ő卂��
	m_fLength = 0.0f;      //�S���_�Ƃ̋���
	m_fAngle = 0.0f;
	m_bAnimFlag = false;   //�A�j���[�V���������邩�ǂ���
	m_pos = SENTER_VECTOR3;//�ʒu�i���S�j
	m_Move = NULL_VECTOR3;//�ړ���
	m_col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);//�F����
	m_SupportPos = NULL_VECTOR3;

	//=======================================
	//��]�n
	//=======================================
	m_fPolygonRotSpeed = 0.0f;//��]���x
	m_rot = NULL_VECTOR3;
	//===============================================================================================

	//=======================================
	//�T�C�Y�n
	//=======================================
	m_Scale = D3DXVECTOR2(1.0f, 1.0f);
	m_bUseScale = false;//�g�嗦���g�p���邩�ǂ���
	//===============================================================================================

	//=======================================
	//���V�n
	//=======================================
	m_fFloatingRot = 0.0f;//���ʌ���
	m_fFloatingSpeed = 0.0f;//���ʑ��x
	m_fFloatingAddSpeed = 0.0f;//���ʉ����x
	m_bUseFloating = false;//���V���g�p���邩�ǂ���
	m_fFloatingLimitSpeed = 0.0f;
	//===============================================================================================

	VERTEX_2D* pVtx;
	
	CRenderer* pRenderer = CManager::GetRenderer();     //�����_���[�N���X���擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice(); //�f�o�C�X���擾

	
	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);


	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = NULL_VECTOR3;
	pVtx[1].pos = NULL_VECTOR3;
	pVtx[2].pos = NULL_VECTOR3;
	pVtx[3].pos = NULL_VECTOR3;

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
	
	return S_OK;
}
//================================================================================


//========================
//�I������
//========================
void CObject2D::Uninit()
{
	int nID = GetId();
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
		//m_pTexture->Release(); //�����A�h���X�̃e�N�X�`�������L���Ă���̂ŁA�A�����[�h�����ŗǂ��H
		m_pTexture = nullptr;
	}
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
		{
			m_nAnimaionPattern++;

			if (m_nAnimaionPattern >= m_nMaxAnimationPattern)
			{
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
	case POLYGONTYPE00_NORMAL:
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y - m_fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y - m_fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y + m_fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y + m_fHeight, 0.0f);
		break;
		//��]�|���S��
	case POLYGONTYPE01_SENTERROLLING:
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
	case POLYGONTYPE02_DOWN://���S�_����
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
	case POLYGONTYPE03_LEFT://���S�_������
		pVtx[0].pos = D3DXVECTOR3(m_pos.x, m_pos.y - m_fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y - m_fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x, m_pos.y + m_fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y + m_fHeight, 0.0f);
		break;
	case POLYGONTYPE04_RIGHT://���S�_������
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
}
//================================================================================

//=====================================
//�e�N�X�`�����蓖�ď���
//=====================================
void CObject2D::BindTexture(LPDIRECT3DTEXTURE9 pTex)
{
 	int nID = GetId();//ID���擾����
 	m_pTexture = pTex;
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
void CObject2D::SetInfo(int nMaxAnimationPattern, int nAnimationChange, float fWidth, float fHeight, D3DXCOLOR col, POLYGONTYPE PolygonType, bool bAnim)
{
	m_nMaxAnimationPattern = nMaxAnimationPattern;             //�A�j���[�V�����̃p�^�[���̍ő吔
	m_nAnimationChange = nAnimationChange;                     //�A�j���[�V�����̍��W��ς���t���[����
	m_fAnimationSplit = (float)(1.0f / m_nMaxAnimationPattern);//�P����������̃A�j���[�V�����͈�
	m_fWidth = fWidth;                                         //����
	m_fMaxWidth = fWidth;                                      //�ő剡��
	m_fHeight = fHeight;                                       //����
	m_fMaxHeight = fHeight;                                    //�ő卂��
	m_PolygonType = PolygonType;                               //�|���S���̏o�����̎�ނ����߂�
	m_col = col;                                               //�F�̐ݒ�
	m_bAnimFlag = bAnim;                                       //�g�p���
}
//================================================================================

//======================================================
//�g�嗦�̏���
//======================================================
void CObject2D::ScaleProcess()
{
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

		float fLength = CCalculation::CalculationLength(m_SupportPos, m_pos);

		float fAbsSpeed = 0.0f;
		fAbsSpeed = fabsf(m_fFloatingSpeed);

		if (fAbsSpeed > m_fFloatingLimitSpeed)
		{
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
	{
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
	{
		m_nLife--;
		m_fRatioLife = float(m_nLife) / float(m_nMaxLife);

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
//�̗͂����炷����
//======================================================
void CObject2D::LifeRatioColorProcess()
{
	SetColor(NORMAL_COL, true, m_fRatioLife);
}
//====================================================================================================