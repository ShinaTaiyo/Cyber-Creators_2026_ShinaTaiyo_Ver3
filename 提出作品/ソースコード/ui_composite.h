//==================================================================================================================
//
//�Q�O�Q�T�N�R���X���F�R���|�W�b�g�p�^�[�����g�p��UI�ɋ@�\����������[ui_composite.h]
//Author:ShinaTaiyo
//
//==================================================================================================================

#ifndef _UI_COMPOSITE_H_  
#define _UI_COMPOSITE_H_

//======================
//�C���N���[�h
//======================
#include "main.h"
#include "object2d.h"
#include "number.h"
#include "gauge.h"
//==================================================================================================================

//======================
//�O���錾
//======================
class CUi;
//==================================================================================================================

//======================
//�N���X
//======================

//�R���|�[�l���g
class CUIComposite_Component
{
public:

	//�@�\�̎��
	enum class UIFUNCTION
	{
		NONE = 0,//�Ȃ�
		NUMERIC, //�����ێ�
		GAUGE,   //�Q�[�W�ێ�
		MAX,     //�ő�
	};

	//����
	CUIComposite_Component(CUi * pUi);         //�R���X�g���N�^
	virtual ~CUIComposite_Component();         //�f�X�g���N�^
	virtual void Update(CUi* pUi) = 0;         //�X�V����
	virtual void Draw(CUi* pUi) = 0;           //�`�揈��

	//��
	void SetFunction(UIFUNCTION Function) { m_Function = Function; }//�@�\�̎�ނ�ݒ�
	UIFUNCTION GetFunction() { return m_Function; }                 //�@�\�̎�ނ��擾

private:
	UIFUNCTION m_Function = UIFUNCTION::NONE;  //UI�̋@�\�̎��
};

//�����ێ�
class CUIComposite_Numeric : public CUIComposite_Component
{
public:
	//����
	CUIComposite_Numeric(CUi* pUi, int nValue, float fWidth, float fHeight);      //�R���X�g���N�^
	~CUIComposite_Numeric() override;     //�f�X�g���N�^
	void Update(CUi* pUi) override;       //�X�V����
	void Draw(CUi* pUI) override;         //�`�揈��

	//��
	void SetValue(int nValue, CUi* pUi);  //���l��ݒ肷��
	int GetValue() { return m_nValue; };  //���l���擾����
private:
	static constexpr int s_nMAX_DIGIT = 8; //�ő包��
	static const int s_nMAX_VALUE;        //�ő吔�l
	vector<CNumber*> m_VecNum;            //�����̓��I�z��
	int m_nValue = 0;                     //���l
	int m_nDigit = 0;                     //����
	float m_fWidth = 0.0f;                //������l
	float m_fHeight = 0.0f;               //������l
};

//�Q�[�W�ێ�
class CUIComposite_Gauge : public CUIComposite_Component
{
public:
	//����
	CUIComposite_Gauge(CUi * pUi,D3DXVECTOR3 GaugePos, D3DXCOLOR Col, CObject2D::POLYGONTYPE PolygonType, CGauge::GAUGETYPE GaugeType, float fMaxWidth, float fMaxHeight, int nValue, int nMaxValue);//�R���X�g���N�^
	~CUIComposite_Gauge() override;//�f�X�g���N�^
	void Update(CUi* pUi) override;//�X�V����
	void Draw(CUi* pUi) override;  //�`�揈��

	//��
	CGauge* GetGauge() { return m_pGauge; } //�Q�[�W���擾
private:
	CGauge* m_pGauge = nullptr;             //�Q�[�W

};

//�R���e�i
class CUIComposite_Container : public CUIComposite_Component
{
public:
	//����
	CUIComposite_Container(CUi* pUi);;                                   //�R���X�g���N�^
	~CUIComposite_Container() override;                                  //�f�X�g���N�^
	void Update(CUi * pUi) override;                                     //�X�V����
	void Draw(CUi * pUi) override;                                       //�`�揈��
														                 
	//��											                     
	void Add(CUIComposite_Component* pComponent);                        //�@�\�̒ǉ�
	template<typename T>
	T* GetChildren()                                                     //�w�肵���@�\���擾����
	{
		for (auto& child : m_Children)
		{
			if (auto CastChild = dynamic_cast<T*>(child))//�w�肵���@�\�ɃL���X�g���Č^�����݂��Ă�����l������B�l�������true�A�Ȃ�������false�Ȃ̂ő��v
			{
				return CastChild;
			}
		}
		return nullptr;//�w�肵���@�\�����݂��Ȃ�����
	};                                                    
private:
	vector<CUIComposite_Component*> m_Children;//�������Ă���@�\
};

//==================================================================================================================

#endif