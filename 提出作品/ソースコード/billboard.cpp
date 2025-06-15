//===================================================================================================================
//
// �Q�O�Q�T�N�U���P���F�R�[�h���Y��ɂ���[billboard.cpp]
// Author:ShinaTaiyo
//
//===================================================================================================================

//*******************************************************************************************************************
// �C���N���[�h
//*******************************************************************************************************************
#include "billboard.h" // ���w�b�_
#include "manager.h"   // �}�l�[�W���[
#include "renderer.h"  // �����_���[�i�f�o�C�X�擾�Ȃǁj

//===================================================================================================================
// 
// public�����o 
//
//===================================================================================================================

//===================================================================================================================
// �R���X�g���N�^
//===================================================================================================================
CBillboard::CBillboard(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) :
	CObject(nPri, bUseintPri, type, ObjType) ,
    m_bAnimFlag(false),
	m_bDraw(true),
	m_bMultiplication(false),
    m_bUseAddSpeed(false),
	m_bUseGravity(false),
	m_bUsePolygonRot(false),
	m_fAddGravity(0.0f),
	m_fAddRot(0.0f),
	m_fAddSpeed(0.0f),
	m_fAnimationSplit(0.0f),
	m_fGravity(0.0f),
	m_fGravityPower(0.0f),
	m_fPolygonRotPower(0.0f),
	m_fStartRot(0.0f),
	m_nMaxAnimationPattern(0),
	m_nAnimationChange(0),
	m_nAnimationCnt(0),
	m_nCntTime(0),
	m_nLife(0),
	m_nMaxLife(0),
    m_nTextureIndex(0),
	m_nAnimaionPattern(0),
	m_Pos(D3DXVECTOR3(0.0f,0.0f,0.0f)),
	m_PosOld(D3DXVECTOR3(0.0f,0.0f,0.0f)),
	m_SupportPos(D3DXVECTOR3(0.0f,0.0f,0.0f)),
	m_Move(D3DXVECTOR3(0.0f,0.0f,0.0f)),
	m_Rot(D3DXVECTOR3(0.0f,0.0f,0.0f)),
	m_mtxWorld(),
    m_Col(D3DXCOLOR(1.0f,1.0f,1.0f,1.0f)),
	m_Scale(D3DXVECTOR3(1.0f,1.0f,1.0f)),
	m_fSpeed(0.0f),
	m_bUseAddScale(false),
	m_Size(D3DXVECTOR3(0.0f,0.0f,0.0f)),
	m_FormarSize(D3DXVECTOR3(0.0f,0.0f,0.0f))
{
	m_pTexture = nullptr;//�e�N�X�`���ւ̃|�C���^��������
	m_pVtxBuff = nullptr;//���_�o�b�t�@�ւ̃|�C���^��������
}

//===================================================================================================================
// �f�X�g���N�^
//===================================================================================================================
CBillboard::~CBillboard()
{

}

//===================================================================================================================
// ����������
//===================================================================================================================
HRESULT CBillboard::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//�f�o�C�X�ւ̃|�C���^�Ǝ擾
	VERTEX_3D* pVtx;

	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4, // ���_�̐�
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�i�����ʒu�j
	pVtx[0].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);

	// nor�̐ݒ�i�����ݒ�j
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	// ���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W�̐ݒ�i�����ݒ�j
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	return S_OK;
}

//===================================================================================================================
// �I������
//===================================================================================================================
void CBillboard::Uninit(void)
{
	// �e�N�X�`���̃|�C���^���������i�e�N�X�`���N���X�ŊǗ����Ă���̂ŁA������Ȃ��Ă����v)
	if (m_pTexture != nullptr)
	{
		m_pTexture = nullptr;
	}

	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
}

//===================================================================================================================
// �X�V����
//===================================================================================================================
void CBillboard::Update(void)
{
	VERTEX_3D* pVtx; // ���_���ւ̃|�C���^

	// === �����Ɏg�p������̐錾�A������ ====

	m_nAnimationCnt++; // �A�j���[�V�����J�E���g���C���N�������g
	m_nCntTime++;      // �o�����Ă���̃t���[�������J�E���g����
	m_nLife -= 1 * static_cast<int>(GetDeltaTimeScale(this) + 0.1f); // �̗͂����炵������ideltatime��timescale���v�Z���ĂP�ȉ��Ȃ�̗͂����炳�Ȃ�)

	// �g�嗦�����Z����
	if (m_bUseAddScale == true)
	{
		m_Scale += m_AddScale;
	}

	// �d�͂����Z���A���݂̏d�͕�Y���W�����Z����
	if (m_bUseGravity == true)
	{
		m_fGravityPower += m_fAddGravity * GetDeltaTimeScale(this);
		m_Move.y += m_fGravityPower * GetDeltaTimeScale(this);
	}

	// ��������Ȃ�
	if (m_bUseAddSpeed == true)
	{
		// ��ZOFF
		if (m_bMultiplication == false)
		{
			m_Move += D3DXVECTOR3(m_fAddSpeed,m_fAddSpeed,0.0f) * GetDeltaTimeScale(this);
		}
		// ��ZON
		else
		{
			m_Move *= powf(m_fAddSpeed,GetDeltaTimeScale(this));//�w���֐��I����
		}
	}

	m_PosOld = m_Pos; //1f�O�̈ʒu
	m_Pos += m_Move * GetDeltaTimeScale(this); // �ʒu�̍X�V

	// ���������Z����
	if (m_bUsePolygonRot == true)
	{
		m_Rot.z += m_fPolygonRotPower * GetDeltaTimeScale(this);
	}

	// �A�j���[�V����������Ȃ�
	if (m_bAnimFlag == true)
	{
		// �A�j���[�V�����J�E���g���A�j���[�V������ύX����t���[�����ɒB������
		if (m_nAnimationCnt % m_nAnimationChange == 0)
		{
			m_nAnimaionPattern++;//�A�j���[�V�����p�^�[�����C���N�������g

			// �A�j���[�V�����p�^�[�����ő�A�j���[�V�����p�^�[���ɒB�����烊�Z�b�g����
			if (m_nAnimaionPattern >= m_nMaxAnimationPattern)
			{
				m_nAnimaionPattern = 0;
			}
		}
	}

	float fLength = sqrtf(m_Size.x * m_Size.x + m_Size.y * m_Size.y) / 2.0f; // ���S�_����|���S���̊p�Ƃ̋��������
	float fAngle = atan2f(m_Size.x, m_Size.y); // ���S�_����p�ւ̊p�x�����߂�

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos.x = sinf(m_Rot.z - fAngle) * fLength; // �ォ�獶�ɂS�T�x
	pVtx[0].pos.y = cosf(m_Rot.z - fAngle) * fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = sinf(m_Rot.z + fAngle) * fLength; // �ォ��E�ɂS�T�x
	pVtx[1].pos.y = cosf(m_Rot.z + fAngle) * fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = sinf(m_Rot.z - (D3DX_PI - fAngle)) * fLength; // �ォ�獶�ɂS�T�x
	pVtx[2].pos.y = cosf(m_Rot.z - (D3DX_PI - fAngle)) * fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = sinf(m_Rot.z + (D3DX_PI - fAngle)) * fLength; // �ォ��E�ɂS�T�x
	pVtx[3].pos.y = cosf(m_Rot.z + (D3DX_PI - fAngle)) * fLength;
	pVtx[3].pos.z = 0.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = m_Col;
	pVtx[1].col = m_Col;
	pVtx[2].col = m_Col;
	pVtx[3].col = m_Col;

	// �e�N�X�`�����W�̐ݒ�i�A�j���[�V�����������~���݂̃A�j���[�V�����p�^�[��)
	pVtx[0].tex = D3DXVECTOR2(m_fAnimationSplit * (float)(m_nAnimaionPattern), 0.0f);
	pVtx[1].tex = D3DXVECTOR2(m_fAnimationSplit + m_fAnimationSplit * (float)(m_nAnimaionPattern), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(m_fAnimationSplit * (float)(m_nAnimaionPattern), 1.0f);
	pVtx[3].tex = D3DXVECTOR2(m_fAnimationSplit + m_fAnimationSplit * (float)(m_nAnimaionPattern), 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	// �̗͂��O�ɂȂ����玀�S�t���O�𔭓�����
	if (m_nLife <= 0)
	{
		SetDeath();
	}

}

//===================================================================================================================
// �`�揈��
//===================================================================================================================
void CBillboard::Draw(void)
{
	D3DXMATRIX mtxTrans,mtxRot,mtxScale; //�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView; // �r���[�}�g���b�N�X
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice(); // �f�o�C�X�ւ̃|�C���^

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

	// �g�嗦�����Z����Ȃ�X�P�[����ݒ�
	//if (m_bUseAddScale == true)
	//{
	//	D3DXMatrixScaling(&mtxScale, m_Scale.x, m_Scale.y, m_Scale.z);
	//	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);
	//}

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, GetPos().x, GetPos().y, GetPos().z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));//���_�o�b�t�@�ւ̃|�C���^�ƒ��_���̍\���̂̃T�C�Y

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	//================================================
	//�`��̒���
	//================================================
	
	// ���C�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Z�̔�r���@�ύX�B�i���f���������Ă��B��Ȃ��Ȃ�j
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	// Z�o�b�t�@�ɏ������܂Ȃ��i�d�Ȃ���Ɉ�a�����Ȃ��Ȃ�j
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// �A���t�@�e�X�g��L��(�A���t�@�l���O���傫���ꍇ�ɕ`�悷��j
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	
	// �|���S���̕`��
	if (m_bDraw == true)
	{
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}

	//================================================
	//�`��̒��������ɖ߂�
	//================================================

	// ���C�g��L���ɖ߂�
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// Z�̔�r���@�ύX�B
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	// Z�o�b�t�@�ɏ�������
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	
	// �A���t�@�e�X�g�����ɖ߂�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//===================================================================================================================
// ���S�t���O�ݒ菈��
//===================================================================================================================
void CBillboard::SetDeath()
{
	CObject::SetDeath(); // �I�u�W�F�N�g���S�t���O�ݒ菈��
}

//===================================================================================================================
// �e�N�X�`�������蓖�Ă�
//===================================================================================================================
void CBillboard::bindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;//�w�肵���A�h���X��ۑ�
}

//===================================================================================================================
// �ʒu�擾
//===================================================================================================================
const D3DXVECTOR3& CBillboard::GetPos() const
{
	return m_Pos;
}

//===================================================================================================================
// �ʒu�ݒ�
//===================================================================================================================
void CBillboard::SetPos(D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//===================================================================================================================
// 1f�O�̈ʒu�擾
//===================================================================================================================
const D3DXVECTOR3& CBillboard::GetPosOld() const
{
	return m_PosOld;
}

//===================================================================================================================
// 1f�O�̈ʒu�ݒ�
//===================================================================================================================
void CBillboard::SetPosOld(D3DXVECTOR3 PosOld)
{
	m_PosOld = PosOld;
}

//===================================================================================================================
// �����ʒu�擾
//===================================================================================================================
const D3DXVECTOR3& CBillboard::GetSupportPos() const
{
	return m_SupportPos;
}

//===================================================================================================================
// �����ʒu�ݒ�
//===================================================================================================================
void CBillboard::SetSupportPos(D3DXVECTOR3 Pos)
{
	m_SupportPos = Pos;
}

//===================================================================================================================
// �T�C�Y�擾
//===================================================================================================================
const D3DXVECTOR3& CBillboard::GetSize() const
{
	return m_Size;
}

//===================================================================================================================
// �T�C�Y�ݒ�
//===================================================================================================================
void CBillboard::SetSize(D3DXVECTOR3 Size)
{
	m_Size = Size;
}

//===================================================================================================================
// ���̃T�C�Y�擾
//===================================================================================================================
const D3DXVECTOR3& CBillboard::GetFormarSize() const
{
	return m_FormarSize;
}

//===================================================================================================================
// ���̃T�C�Y�ݒ�
//===================================================================================================================
void CBillboard::SetFormarSize(D3DXVECTOR3 FormarSize)
{
	m_FormarSize = FormarSize;
}

//===================================================================================================================
// �g�嗦�ݒ�
//===================================================================================================================
void CBillboard::SetScale(D3DXVECTOR3 Scale)
{
	m_Scale = Scale;
}

//===================================================================================================================
// �g�嗦�擾
//===================================================================================================================
const D3DXVECTOR3& CBillboard::GetScale() const
{
	return m_Scale;
}

//===================================================================================================================
// ���Z�g�嗦�ݒ�
//===================================================================================================================
void CBillboard::SetUseAddScale(bool bUse, D3DXVECTOR3 AddScale)
{
	m_bUseAddScale = bUse;
	m_AddScale - AddScale;
}

//===================================================================================================================
// ���Z�g�嗦�擾
//===================================================================================================================
const D3DXVECTOR3& CBillboard::GetAddScale() const
{
	return m_AddScale;
}

//===================================================================================================================
// �F�����ݒ�
//===================================================================================================================
void CBillboard::SetColor(D3DXCOLOR col)
{
	m_Col = col;
}

//===================================================================================================================
// �F�����擾
//===================================================================================================================
const D3DXCOLOR & CBillboard::GetColor() const
{
	return m_Col;
}

//===================================================================================================================
// �e�N�X�`���ԍ��ݒ�
//===================================================================================================================
void CBillboard::SetTextureIndex(int nIdx)
{
	m_nTextureIndex = nIdx;
}

//===================================================================================================================
// �e�N�X�`���ԍ��擾
//===================================================================================================================
const int& CBillboard::GetTextureIndex() const
{
	return m_nTextureIndex;
}

//===================================================================================================================
// �A�j���[�V�����ݒ�
//===================================================================================================================
void CBillboard::SetAnimInfo(int nMaxAnimationPattern, int nAnimationChange, D3DXCOLOR col, bool bAnim)
{
	m_Col = col;         // �F����
	m_nMaxAnimationPattern = nMaxAnimationPattern; // �A�j���[�V�����p�^�[���ő�l
	m_bAnimFlag = bAnim; // �A�j���[�V���������邩�ǂ���
	m_fAnimationSplit = (float)(1.0f / m_nMaxAnimationPattern); // �P����������̃A�j���[�V�����͈́i������)
	m_nAnimationChange = nAnimationChange; // �A�j���[�V���������ɐi�߂�J�E���g��
}

//===================================================================================================================
// �d�͐ݒ�
//===================================================================================================================
void CBillboard::SetUseGravity(float fGravity)
{
	m_fGravityPower = fGravity;
	m_bUseGravity = true;      
}

//===================================================================================================================
// ���Z�d�͐ݒ�
//===================================================================================================================
void CBillboard::SetAddGravity(float fAddGravity)
{
	m_fAddGravity = fAddGravity;
}

//===================================================================================================================
// �����ݒ�
//===================================================================================================================
void CBillboard::SetUseAddSpeed(bool bUseAddSpeed, bool bMultiplication, float fAddSpeed)
{
	m_bUseAddSpeed = bUseAddSpeed;       // ���Z���x���g�p���邩�ǂ���
	m_bMultiplication = bMultiplication; // ��Z���x���g�p���邩�ǂ���
	m_fAddSpeed = fAddSpeed;             // ���Z���x��ݒ肷��
}

//===================================================================================================================
// �|���S����]�ݒ�
//===================================================================================================================
void CBillboard::SetUsePolygonRot(bool bUse, float fRotPower)
{
	m_bUsePolygonRot = bUse; // �g�p���邩�ǂ���
	m_fPolygonRotPower = fRotPower; // ��]��
}

//===================================================================================================================
// �`�悷�邩�ǂ����擾
//===================================================================================================================
const bool& CBillboard::GetUseDraw() const
{
	return m_bDraw;
}

//===================================================================================================================
// �`�悷�邩�ǂ����ݒ�
//===================================================================================================================
void CBillboard::SetUseDraw(bool bDraw)
{
	m_bDraw = bDraw;
}

//===================================================================================================================
// �̗͎擾
//===================================================================================================================
const int& CBillboard::GetLife() const
{
	return m_nLife;
}

//===================================================================================================================
// �̗͐ݒ�
//===================================================================================================================
void CBillboard::SetLife(int nLife)
{
	m_nLife = nLife;
}

//===================================================================================================================
// �ő�̗͎擾
//===================================================================================================================
const int& CBillboard::GetMaxLife() const
{
	return m_nMaxLife;
}

//===================================================================================================================
// �ő�̗͐ݒ�
//===================================================================================================================
void CBillboard::SetMaxLife(int nLife)
{
	m_nMaxLife = nLife;
}
