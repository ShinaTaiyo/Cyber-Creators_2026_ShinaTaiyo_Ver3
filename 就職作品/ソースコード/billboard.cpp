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
#include "game.h"
#include "scene.h"
#include "objectX.h"
//=============================================================================================

//==================================================
//�ÓI�����o������
//==================================================

//=============================================================================================

//==================================================
//�R���X�g���N�^
//==================================================
CBillboard::CBillboard(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CObject(nPri, bUseintPri, type, ObjType) ,
m_bAnimFlag(false),m_bDraw(false),m_bMultiplication(false),
m_bUseAddSpeed(false),m_bUseCurve(false),m_bUseGravity(false),m_bUseHorming(false),m_bUseLengthCurve(false),m_bUsePolygonRot(false),
m_fAddCurveLength(0.0f),m_fAddGravity(0.0f),m_fAddRot(0.0f),m_fAddSpeed(0.0f),m_fAnimationSplit(0.0f),m_fCurveSpeed(0.0f),m_fFormarHeight(0.0f),
m_fFormarWidth(0.0f),m_fGravity(0.0f),m_fGravityPower(0.0f),m_fHeight(0.0f),m_fPolygonRotPower(0.0f),m_fRotMove(0.0f),m_fStartRot(0.0f),
m_fSupportCurveLength(0.0f),m_fWidth(0.0f),m_nMaxAnimationPattern(0),m_nAnimationChange(0),m_nAnimationCnt(0),m_nCntTime(0),m_nLife(0),m_nMaxLife(0),
m_nTextureIndex(0),m_nAnimaionPattern(0), m_nCntBlinkingFrame(0), m_nMaxBlinkingFrame(0), m_bBlinkingAim(false), m_bUseBlinking(false), 
m_fLimitBlinkingRatio(0.0f),m_Pos(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_PosOld(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_SupportPos(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_Move(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_Rot(D3DXVECTOR3(0.0f,0.0f,0.0f)), m_mtxWorld(),
m_Col(D3DXCOLOR(1.0f,1.0f,1.0f,1.0f)),m_Scale(D3DXVECTOR3(1.0f,1.0f,1.0f)),m_fSpeed(0.0f),m_bUseAddScale(false)
{
	m_pTexture = nullptr;//�e�N�X�`���ւ̃|�C���^��������
	m_pVtxBuff = nullptr;//���_�o�b�t�@�ւ̃|�C���^��������
}
//=============================================================================================

//==================================================
//�f�X�g���N�^
//==================================================
CBillboard::~CBillboard()
{
	m_mtxWorld = {};//���[���h�}�g���b�N�X��������
}
//=============================================================================================

//==================================================
//����������
//==================================================
HRESULT CBillboard::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//�f�o�C�X�ւ̃|�C���^�Ǝ擾

	m_Col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);               //�F����
	 
	m_Pos = D3DXVECTOR3(0.0f,0.0f,0.0f);                     //�ʒu
	m_PosOld = D3DXVECTOR3(0.0f,0.0f,0.0f);                  //1f�O�̈ʒu
	m_SupportPos = D3DXVECTOR3(0.0f,0.0f,0.0f);              //�����ʒu
	m_Rot = D3DXVECTOR3(0.0f,0.0f,0.0f);                     //����
	m_Move = D3DXVECTOR3(0.0f,0.0f,0.0f);                    //�ړ���
	m_mtxWorld = {};                                         //�}�g���b�N�X���[���h
	m_bDraw = true;                                          //��{�I�ɂ͕`�悷��

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
	pVtx[0].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);

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
	//�e�N�X�`���̃|�C���^���������i�e�N�X�`���N���X�ŊǗ����Ă���̂ŁA������Ȃ��Ă����v)
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

	//========================================
	//�x�_�����ɃJ�[�u��ON��������
	//========================================
	if (m_bUseLengthCurve == true)
	{
		m_fAddRot += m_fCurveSpeed;//�������J�[�u���x�����Z����
		m_fSupportCurveLength += m_fAddCurveLength;//�J�[�u���鋗�������Z����
		m_Pos.x = sinf(m_fAddRot + m_fStartRot) * m_fSupportCurveLength + m_SupportPos.x;
		m_Pos.y = cosf(m_fAddRot + m_fStartRot) * m_fSupportCurveLength + m_SupportPos.y;
	}
	//=============================================================================================

	//========================================
	//���Z�g�嗦��ON�ɂȂ��Ă�����
	//========================================
	if (m_bUseAddScale == true)
	{//�g�嗦�����Z����
		m_Scale += m_AddScale;
	}
	//=============================================================================================

	//========================================
	//�d�͂�ON�ɂȂ��Ă�����
	//========================================
	if (m_bUseGravity == true)
	{//�d�͂����Z���A���݂̏d�͕�Y���W�����Z����
		m_fGravityPower += m_fAddGravity;
		m_Move.y += m_fGravityPower;
	}
	//==============================================================================================

	//�_�ł̏���
	BlinkingProcess();

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
	m_PosOld = m_Pos;//1f�O�̈ʒu
	m_Pos += m_Move; //�ʒu�̍X�V
	//==============================================================================================

	//========================================
	//�|���S����]����
	//========================================
	if (m_bUsePolygonRot == true)
	{//���������Z����
		m_Rot.z += m_fPolygonRotPower;
	}
	//==============================================================================================

	if (m_bAnimFlag == true)
	{//�A�j���[�V����������Ȃ�
		if (m_nAnimationCnt % m_nAnimationChange == 0)
		{//�A�j���[�V�����J�E���g���A�j���[�V������ύX����t���[�����ɒB������
			m_nAnimaionPattern++;//�A�j���[�V�����p�^�[�����C���N�������g

			if (m_nAnimaionPattern >= m_nMaxAnimationPattern)
			{//�A�j���[�V�����p�^�[�����ő�A�j���[�V�����p�^�[���ɒB�����烊�Z�b�g����
				m_nAnimaionPattern = 0;
			}
		}
	}

	float fLength = sqrtf(m_fWidth * m_fWidth + m_fHeight * m_fHeight) / 2.0f;//���S�_����|���S���̊p�Ƃ̋��������
	float fAngle = atan2f(m_fWidth, m_fHeight);                               //���S�_����p�ւ̊p�x�����߂�

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

	//���_�J���[�̐ݒ�
	pVtx[0].col = m_Col;
	pVtx[1].col = m_Col;
	pVtx[2].col = m_Col;
	pVtx[3].col = m_Col;

	//�e�N�X�`�����W�̐ݒ�i�A�j���[�V�����������~���݂̃A�j���[�V�����p�^�[��
	pVtx[0].tex = D3DXVECTOR2(m_fAnimationSplit * (float)(m_nAnimaionPattern), 0.0f);
	pVtx[1].tex = D3DXVECTOR2(m_fAnimationSplit + m_fAnimationSplit * (float)(m_nAnimaionPattern), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(m_fAnimationSplit * (float)(m_nAnimaionPattern), 1.0f);
	pVtx[3].tex = D3DXVECTOR2(m_fAnimationSplit + m_fAnimationSplit * (float)(m_nAnimaionPattern), 1.0f);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	if (m_nLife <= 0)
	{//�̗͂��O�ɂȂ����玀�S�t���O�𔭓�����
		SetDeath();
	}

}
//=============================================================================================

//==================================================
//�`�揈��
//==================================================
void CBillboard::Draw(void)
{
	D3DXMATRIX mtxTrans,mtxRot,mtxScale;//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxParent = {};
	D3DXMATRIX mtxView;//�r���[�}�g���b�N�X�擾�p
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

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

	if (m_bUseAddScale == true)
	{
		//�g�嗦��ݒ�
		D3DXMatrixScaling(&mtxScale, m_Scale.x, m_Scale.y, m_Scale.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);
	}
	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, GetPos().x, GetPos().y, GetPos().z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

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
//���S�t���O�ݒ菈��
//==================================================
void CBillboard::SetDeath()
{
	CObject::SetDeath();
}
//=============================================================================================

//==================================================
//�e�N�X�`�������蓖�Ă鏈��
//==================================================
void CBillboard::bindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;//�w�肵���A�h���X��ۑ�
}
//============================================================================================================================================

//==================================================
//�����`�̓����蔻������
//==================================================
bool CBillboard::CollisionSquare(D3DXVECTOR3 pos, D3DXVECTOR3 VtxMax, D3DXVECTOR3 VtxMin)
{
	D3DXVECTOR3 ComparisonPos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	D3DXVECTOR3 ComparisonVtxMin = D3DXVECTOR3(0.0f,0.0f,0.0f);
	D3DXVECTOR3 ComparisonVtxMax = D3DXVECTOR3(0.0f,0.0f,0.0f);

	ComparisonPos.x = (float)(floor(pos.x));//�����_��؂�̂Ă��ʒu����
	ComparisonPos.y = (float)(floor(pos.y));//�����_��؂�̂Ă��ʒu����
	ComparisonPos.z = (float)(floor(pos.z));//�����_��؂�̂Ă��ʒu����

	ComparisonVtxMax.x = (float)(floor(VtxMax.x));//�����_��؂�̂Ă��ő咸�_��������
	ComparisonVtxMax.y = (float)(floor(VtxMax.y));//�����_��؂�̂Ă��ő咸�_��������
	ComparisonVtxMax.z = (float)(floor(VtxMax.z));//�����_��؂�̂Ă��ő咸�_��������

	ComparisonVtxMin.x = (float)(floor(VtxMin.x));//�����_��؂�̂Ă��ŏ����_��������
	ComparisonVtxMin.y = (float)(floor(VtxMin.y));//�����_��؂�̂Ă��ŏ����_��������
	ComparisonVtxMin.z = (float)(floor(VtxMin.z));//�����_��؂�̂Ă��ŏ����_��������


	D3DXVECTOR2 MySize = D3DXVECTOR2(0.0f,0.0f);//�|���S���̃T�C�Y����p

	//==================================
	//����p�ɃL���X�g����
	//==================================
	MySize.x = (float)(floor(m_fWidth));//�����_��؂�̂Ă�������������
	MySize.y = (float)(floor(m_fHeight));//�����_��؂�̂Ă�������������
	//==========================================================================================================

	bool bCollision = false;                         //�n�ʂɒ��n���Ă��邩
	
	if (m_Pos.x + MySize.x / 2 >= ComparisonPos.x + ComparisonVtxMin.x
		&& m_Pos.x - MySize.x / 2 <= ComparisonPos.x + ComparisonVtxMax.x
		&& m_Pos.y + MySize.y / 2 >= ComparisonPos.y + ComparisonVtxMin.y
		&& m_Pos.y - MySize.y / 2 <= ComparisonPos.y + ComparisonVtxMax.y
		&& m_Pos.z + MySize.x / 2 >= ComparisonPos.z + ComparisonVtxMin.z
		&& m_Pos.z - MySize.x / 2 <= ComparisonPos.z + ComparisonVtxMax.z)
	{//�����蔻����s��
		bCollision = true;
	}
	return bCollision;
}
//============================================================================================================================================

//==================================================
//�T�C�Y��ݒ肷��
//==================================================
void CBillboard::SetSize(float fWidth, float fHeight)
{
	m_fWidth = fWidth;//����
	m_fHeight = fHeight;//����
}
//============================================================================================================================================

//==========================================================================
//���̃T�C�Y��ݒ肷��
//==========================================================================
void CBillboard::SetFormarSize(float fFormarWidth, float fFormarHeight)
{
	m_fFormarWidth = fFormarWidth;//��̉���
	m_fFormarHeight = fFormarHeight;//��̍���
}
//============================================================================================================================================

//==================================================
//�F������ݒ�
//==================================================
void CBillboard::SetColor(D3DXCOLOR col)
{
	m_Col = col;//�F������ݒ�
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
	m_fAnimationSplit = (float)(1.0f / m_nMaxAnimationPattern);//�P����������̃A�j���[�V�����͈́i������)
}
//================================================================================================================

//===============================================================
//��_�����ɉ�]����J�[�u��ݒ�
//===============================================================
void CBillboard::SetUseLengthCurve(bool bUse, float fStartRot, float fCurveSpeed, float fAddCurveLength, float fSupportLength)
{
	m_bUseLengthCurve = bUse;                //�g�p���邩�ǂ���
	m_fStartRot = fStartRot;                 //�ŏ��̌���
	m_fCurveSpeed = fCurveSpeed;             //�J�[�u���x
	m_fAddCurveLength = fAddCurveLength;     //���Z���鋗��
	m_fSupportCurveLength = fSupportLength;  //��ƂȂ�ŏ��̊�_����̋���
}
//================================================================================================================

//===============================================================
//�d�͂�ݒ�
//===============================================================
void CBillboard::SetUseGravity(float fGravity)
{
	m_fGravityPower = fGravity;//�d�͂̑傫��
	m_bUseGravity = true;      //�d�͂��g�p����
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
	m_bUseAddSpeed = bUseAddSpeed;       //���Z���x���g�p���邩�ǂ���
	m_bMultiplication = bMultiplication; //��Z���x���g�p���邩�ǂ���
	m_fAddSpeed = fAddSpeed;             //���Z���x��ݒ肷��
}
//================================================================================================================

//===============================================================
//�z�[�~���O�̏���
//===============================================================
void CBillboard::HormingProcess()
{
	float fRotDest = 0.0f;                               //�ړI�̈ʒu�ւ̊p�x
	float fRotDiff = 0.0f;                               //���݂̊p�x�ƖړI�̊p�x�̍���
	float fVXaim = 0.0f;                                 //X�x�N�g��
	float fVYaim = 0.0f;                                 //Y�x�N�g��
	D3DXVECTOR3 TargetPos = D3DXVECTOR3(0.0f,0.0f,0.0f); //�^�[�Q�b�g�̈ʒu
	//===================================
	//XZ�x�N�g�����o��
	//===================================
	if (CScene::GetMode() == CScene::MODE_GAME)
	{//�V�[�����Q�[���Ȃ�
		TargetPos = CGame::GetPlayer()->GetPosInfo().GetPos(); //�v���C���[�̈ʒu���^�[�Q�b�g�Ƃ���
		fVXaim = TargetPos.x - m_Pos.x;                        //�^�[�Q�b�g�̈ʒu�ƃr���{�[�h�̈ʒu��X�x�N�g�������߂�
		fVYaim = TargetPos.y - m_Pos.y;                        //�^�[�Q�b�g�̈ʒu�ƃr���{�[�h�̈ʒu��Y�x�N�g�������߂�

		fRotDest = atan2f(fVXaim, fVYaim);                     //X��Y�̃x�N�g�������Ɋp�x�����߂�

		//==========================================================================================================================================================

		//���݂̈ړ������̊p�x�ƁA�ړI�̈ړ������̊p�x�̍��������߂�
		fRotDiff = fRotDest - m_fRotMove;

		//===============================
		//�p�x�����̏C��
		//===============================
		
		//�����̍����̒���(3.14�𒴂�����߂������ɕ␳���W���o�����b�N�����)
		if (fRotDiff > D3DX_PI)
		{
			fRotDiff -= D3DX_PI * 2;
		}
		else if (fRotDiff < -D3DX_PI)
		{
			fRotDiff += D3DX_PI * 2;
		}

		//==============================================================================================
		m_fRotMove += fRotDiff * 0.05f;//�ړ������̕␳

		//�ړ��ʂ̍X�V
		m_Move.x = sinf(m_fRotMove) * m_fSpeed;
		m_Move.y = cosf(m_fRotMove) * m_fSpeed;
	}
}
//================================================================================================================

//===================================================================
//�_�ŏ���
//===================================================================
void CBillboard::BlinkingProcess()
{
	if (m_bUseBlinking == true)
	{//�_�ł�����Ȃ�
		if (m_bBlinkingAim == false)
		{//false�Ȃ�C���N�������g
			m_nCntBlinkingFrame++;
		}
		else
		{//true�Ȃ�f�N�������g
			m_nCntBlinkingFrame--;
		}

		if (m_nCntBlinkingFrame >= m_nMaxBlinkingFrame || m_nCntBlinkingFrame <= 0)
		{//�_�Ńt���[���̃J�E���g���ő�ȏ�A���͂O�ȉ��ɂȂ�����_�ŕ������t�ɂ���
			m_bBlinkingAim = m_bBlinkingAim ? false : true;
		}

		float fRatioBlinkingFrame = 0.0f;//�_�Ńt���[���̊���
		fRatioBlinkingFrame = float(m_nCntBlinkingFrame) / float(m_nMaxBlinkingFrame);//�_�Ńt���[��
		m_Col.a = fRatioBlinkingFrame;//�����x����L�ŋ��߂�����

		VERTEX_3D* pVtx;//3D���_���
		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		//���_�J���[�̐ݒ�
		pVtx[0].col = m_Col;
		pVtx[1].col = m_Col;
		pVtx[2].col = m_Col;
		pVtx[3].col = m_Col;

		//���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();

		if (m_fLimitBlinkingRatio > fRatioBlinkingFrame)
		{//�_�ł��铧���x�̍Œ�l�����������J�E���g����l���t�ɂ���
			m_bBlinkingAim = false;
		}
	}
}
//================================================================================================================