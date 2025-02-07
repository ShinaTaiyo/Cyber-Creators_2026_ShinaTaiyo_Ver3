//=====================================================================================
//
//�X���P�O���F�^�C�g�������[ui.h]
//Author:ShinaTaiyo
//
//=====================================================================================1

#ifndef _UI_H_  
#define _UI_H_

//======================
//�C���N���[�h
//======================
#include "main.h"
#include "object2d.h"
#include "number.h"
#include "gauge.h"
//==========================================

//======================
//�O���錾
//======================
class CObject2D;
class CUi;

//==========================================
//UI�X�e�[�g
//==========================================
class CUiState
{
public:
	//UI�̏�ԗ񋓌^
	enum class UISTATE
	{
		NONE = 0,//�Ȃ�
		NUMERIC, //�����ێ�
		GAUGE,   //�Q�[�W�ێ�
		MAX
	};
	CUiState();                 //�R���X�g���N�^
	virtual ~CUiState();        //�f�X�g���N�^
	virtual void Process(CUi* pUi);//����      
	void SetUiState(UISTATE pUiState) { m_pUiState = pUiState; }
	UISTATE GetUiState() { return m_pUiState; }//UI�̏�Ԃ��擾����
private:
	UISTATE m_pUiState = UISTATE::NONE;//UI�̏�Ԃ��i�[
};
//=======================================================================================

//==========================================
//UI�X�e�[�g�F�����ێ�
//==========================================
class CUiState_Numeric : public CUiState
{
public:
	CUiState_Numeric(CUi* pUi, int nValue, float fWidth, float fHeight);//�R���X�g���N�^
	~CUiState_Numeric() override;//�f�X�g���N�^
	void Process(CUi* pUi) override;//����
	void SetValue(int nValue,CUi * pUi);//���l��ݒ肷��
	int GetValue() { return m_nValue; };
private:
	vector<CNumber*> m_VecNum;//�����̃x�N�^�[
	int m_nValue = 0;         //���l
	float m_fWidth = 0.0f;           //������l
	float m_fHeight = 0.0f;          //������l
};
//=======================================================================================

//==========================================
//UI�X�e�[�g�F�Q�[�W�ێ�
//==========================================
class CUiState_Gauge : public CUiState
{
public:
	CUiState_Gauge(D3DXVECTOR3 GaugePos,D3DXCOLOR Col,CObject2D::POLYGONTYPE PolygonType,CGauge::GAUGETYPE GaugeType,float fMaxWidth, float fMaxHeight, int nValue, int nMaxValue);//�R���X�g���N�^
	~CUiState_Gauge() override;//�f�X�g���N�^
	void Process(CUi* pUi);//����
	CGauge* GetGauge() { return m_pGauge;}//�Q�[�W���擾
private:
	CGauge* m_pGauge = nullptr;//�Q�[�W
};
//=======================================================================================

//==========================================
//UI�N���X
//==========================================
class CUi : public CObject2D
{
public:
	enum class UITYPE
	{
		LOCKON = 0,      //���b�N�I��
		ACTIONMODEWINDOW,//�A�N�V�������[�h�E�C���h�E
	    ACTIONMODE_GUN,  //�V���b�g���[�hUI
		ACTIONMODE_DIVE, //�_�C�u���[�hUI
		PHASETEXT,       //�t�F�[�Y����
		STAGETEXT,       //�X�e�[�W����
	    TARGET_000,      //�^�[�Q�b�g
		POSSIBLEDIVENUMTEXT_000,//�_�C�u�\�񐔕���
		TUTORIAL_TEX,        //�`���[�g���A���p�e�N�X�`��
		CHECKMARK_000,       //�`�F�b�N�}�[�N
		TITLELOGO_000,       //�^�C�g�����S
		PRESSENTER_000,      //�G���^�[�L�[������
		DIVEGAUGEFRAME_000,  //�_�C�u�Q�[�W�̃t���[��
		GAMECLEAR_000,       //�Q�[���N���A�̕����̃e�N�X�`��
		LOSE_000,            //�����̕����̃e�N�X�`��
		MAX
	};

	typedef enum
	{
		UIMOVETYPE_NORMAL = 0,//���ʂ̓���
		UIMOVETYPE_FLOATING,  //���V������
		UIMOVETYPE_MAX
	}UIMOVETYPE;

	CUi(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::UI, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_2D);                      //�R���X�g���N�^
	~CUi() override;             //�f�X�g���N�^
	HRESULT Init() override;    //����������
	void Uninit() override;     //�I������
	void Update() override;     //�X�V����
	void Draw() override;       //�`�揈��
	void SetDeath() override;   //���S�t���O��ݒ�
	static CUi * Create(UITYPE type,CObject2D::POLYGONTYPE PolygonType,float fWidth, float fHeight,int nLife,bool bUseLife,D3DXVECTOR3 Pos,D3DXVECTOR3 Move,D3DXVECTOR3 Rot,D3DXCOLOR col);//�w�i�̐���
	void SetUiType(UITYPE type);
	void SetUseUiEffect(bool bUse, int nSetEffectLife, D3DXCOLOR col) { m_bUseUiEffect = bUse; m_nSetUiEffectLife = nSetEffectLife; m_SetUiEffectColor = col; }
	UITYPE GetUiType() { return m_Type; }//UI�̎�ނ��擾
	void SetNumericState(int nValue, float fWidth, float fHeight);//������Ԃ�ݒ肷��

	CUiState* GetUiState(CUiState::UISTATE UiState);//�w�肵��UI�̏�Ԃ��擾����
	void PushUiState(CUiState* pUiState) { m_VecUiState.push_back(pUiState);}//�V����UI�̏�Ԃ𓮓I�z��Ɋi�[
protected:
	static const string UI_FILENAME[int(UITYPE::MAX)];//UI�̃e�N�X�`���t�@�C����
private:
	UITYPE m_Type;                 //�w�i�̎��
	UIMOVETYPE m_MoveType;         //UI�̓������̎��
	bool m_bUseUiEffect;           //UI�ŃG�t�F�N�g���o�����ǂ���

	D3DXCOLOR m_SetUiEffectColor;  //UI�ŏo���G�t�F�N�g�̐F�����ݒ�p
	int m_nSetUiEffectLife;        //UI�̃G�t�F�N�g�̗̑͐ݒ�p
	vector<CUiState*> m_VecUiState;//UI�̏�ԍ\����
};
//=======================================================================================

//==========================================
//UI�G�t�F�N�g�N���X
//==========================================
class CUiEffect : public CUi
{
public:
	CUiEffect();                //�R���X�g���N�^
	virtual ~CUiEffect();       //�f�X�g���N�^
	HRESULT Init() override;    //����������
	void Uninit() override;     //�I������
	void Update() override;     //�X�V����
	void Draw() override;       //�`�揈��
	void SetDeath() override;   //���S�t���O��ݒ�
	static CUiEffect* Create(UITYPE type, CObject2D::POLYGONTYPE PolygonType, float fWidth, float fHeight,int nLife,D3DXVECTOR3 Pos, D3DXVECTOR3 Move, D3DXVECTOR3 Rot, D3DXCOLOR col);//�w�i�̐���
private:
};
//=======================================================================================
#endif