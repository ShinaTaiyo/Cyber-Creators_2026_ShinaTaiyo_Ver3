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
#include "ui_composite.h"
#include "gauge.h"
//==========================================

//==========================================
//UI�N���X
//==========================================
class CUi : public CObject2D
{
public:
	enum class UITYPE
	{//UI�̎��
		LOCKON = 0,                      //���b�N�I��
		ACTIONMODEWINDOW,                //�A�N�V�������[�h�E�C���h�E
	    ACTIONMODE_GUN,                  //�V���b�g���[�hUI
		ACTIONMODE_DIVE,                 //�_�C�u���[�hUI
		PHASETEXT,                       //�t�F�[�Y����
		STAGETEXT,                       //�X�e�[�W����
	    TARGET_000,                      //�^�[�Q�b�g
		POSSIBLEDIVENUMTEXT_000,         //�_�C�u�\�񐔕���
		TUTORIAL_TEX,                    //�`���[�g���A���p�e�N�X�`��
		CHECKMARK_000,                   //�`�F�b�N�}�[�N
		TITLELOGO_000,                   //�^�C�g�����S
		PRESSENTER_000,                  //�G���^�[�L�[������
		DIVEGAUGEFRAME_000,              //�_�C�u�Q�[�W�̃t���[��
		GAMECLEAR_000,                   //�Q�[���N���A�̕����̃e�N�X�`��
		LOSE_000,                        //�����̕����̃e�N�X�`��
		COMBOTEXT_000,                   //�R���{�̕����̃e�N�X�`��
	    PAUSETEXT_000,                   //�|�[�Y�̕����̃e�N�X�`��
	    EXITTEXT_000,                    //EXIT�̕����̃e�N�X�`��
	    CONTINUETEXT_000,                //CONTINUE�̕����̃e�N�X�`��
	    CAMERASENSITIVITYTEXT_000,       //�J�������x�����̕����̃e�N�X�`��
	    RETURNTEXT_000,                  //�߂�̕����̃e�N�X�`��
		MAX
	};

	typedef enum
	{
		UIMOVETYPE_NORMAL = 0,//���ʂ̓���
		UIMOVETYPE_FLOATING,  //���V������
		UIMOVETYPE_MAX
	}UIMOVETYPE;

	CUi(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::UI, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_2D);//�R���X�g���N�^
	~CUi() override;                                                                                                                                     //�f�X�g���N�^
	HRESULT Init() override;                                                                                                                             //����������
	void Uninit() override;                                                                                                                              //�I������
	void Update() override;                                                                                                                              //�X�V����
	void Draw() override;                                                                                                                                //�`�揈��
	void SetDeath() override;                                                                                                                            //���S�t���O��ݒ�
	static CUi * Create(UITYPE type,bool bPauseUi,CObject2D::POLYGONTYPE PolygonType,float fWidth, float fHeight,int nLife,bool bUseLife,D3DXVECTOR3 Pos,D3DXVECTOR3 Move,D3DXVECTOR3 Rot,D3DXCOLOR col);//�w�i�̐���
	void SetUiType(UITYPE type);//UI�̎�ނ�ݒ�
	void SetUseUiEffect(bool bUse, int nSetEffectLife, D3DXCOLOR col) { m_bUseUiEffect = bUse; m_nSetUiEffectLife = nSetEffectLife; m_SetUiEffectColor = col; }//UI�ɃG�t�F�N�g��ݒ�
	UITYPE GetUiType() { return m_Type; }                                    //UI�̎�ނ��擾
	CUIComposite_Container* GetUiCompositeContainer() { return m_pUiCompositeContainer; }//UI�̋@�\���擾
protected:
	static const string UI_FILENAME[int(UITYPE::MAX)];                       //UI�̃e�N�X�`���t�@�C����
private:
	UITYPE m_Type;                                                           //�w�i�̎��
	UIMOVETYPE m_MoveType;                                                   //UI�̓������̎��
	bool m_bUseUiEffect;                                                     //UI�ŃG�t�F�N�g���o�����ǂ���
								                                             
	D3DXCOLOR m_SetUiEffectColor;                                            //UI�ŏo���G�t�F�N�g�̐F�����ݒ�p
	int m_nSetUiEffectLife;                                                  //UI�̃G�t�F�N�g�̗̑͐ݒ�p

	CUIComposite_Container * m_pUiCompositeContainer;                           //UI�̋@�\�������Ă���R���|�W�b�g�p�^�[��
};
//=======================================================================================
#endif