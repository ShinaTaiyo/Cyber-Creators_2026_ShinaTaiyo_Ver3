//===============================================================================
//
//�Q�O�Q�T�N�R���P�W���F�R���{����������[combo.h]
//Author:ShinaTaiyo
//
//===============================================================================

//========================================
//�Q�d�C���N���[�h�h�~
//========================================
#ifndef _COMBO_H_  
#define _COMBO_H_
//===============================================================================


//========================================
//�C���N���[�h
//========================================
#include "main.h"
#include "object.h"
#include "ui.h"
//===============================================================================

//========================================
//�R���{�N���X
//========================================
class CCombo : public CObject

{
public:
	//�����N�񋓌^
	enum class RANK
	{
		E = 0,
		D,
		C,
		B,
		A,
		S,
		SS,
		SSS,
		MAX
	};

	//�ʏ�
	CCombo(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::COMBO, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_NONE);//�R���X�g���N�^
	~CCombo() override;                                            //�f�X�g���N�^
	HRESULT Init() override;                                       //����������
	void Uninit() override;                                        //�I������
	void Update() override;                                        //�X�V����
	void Draw() override;                                          //�`�揈��
	void SetDeath() override;                                      //���S�t���O�ݒ菈��
	static CCombo* Create(D3DXVECTOR3 Pos);                        //��������

	//��
	void AddCombo(int nCombo);                                     //�R���{���Z
	void ResetCombo();                                             //�R���{���Z�b�g
	int GetComboValue() { return m_nCombo; }                       //�R���{���擾
private:
	//=================
	//�ÓI�����o
	//=================
	static constexpr float s_fCOMBODISP_WIDTH = 180.0f;      //�R���{�\���̉���
	static constexpr float s_fCOMBODISP_HEIGHT = 180.0f;     //�R���{�\���̍���
	static constexpr float s_fCOMBODISP_VALUEWIDTH = 45.0f;  //�R���{�\���̐��l�̉���
	static constexpr float s_fCOMBODISP_VALUEHEIGHT = 45.0f; //�R���{�\���̐��l�̍���
	static const D3DXVECTOR3 s_COMBODISP_POS;                //�R���{�\���̈ʒu
	//===========================================================

	//=================
	//�֐�
	//=================

	//===========================================================

	//=================
	//�����o�ϐ�
	//=================
	int m_nCombo = 0;             //�R���{��
	RANK m_Rank = RANK::E;        //E�����N
	float m_fMagnification = 0.0f;//�X�R�A�{��
	CUi* m_pComboDisp = nullptr;    //�R���{���\��UI
	//===========================================================
};
//===============================================================================

#endif