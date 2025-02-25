//=====================================================================================
//
//�P�Q���Q�S���F�`���[�g���A�������[tutorial.h]
//Author:ShinaTaiyo
//
//=====================================================================================

#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_


//=============================================
//�C���N���[�h
//=============================================
#include "main.h"
#include "object.h"
#include "ui.h"
//=====================================================================================

//=============================================
//�O���錾
//=============================================

//=====================================================================================


//=============================================
//�`���[�g���A���N���X
//=============================================
class CTutorial : public CObject
{
public:
	enum class CHECK
	{//�`���[�g���A���`�F�b�N�p
		MOVE = 0,       //�ړ�
		SHOT,           //�ˌ�
		CAMERACONTROLL, //�J�����𑀍삵��
		DASH,           //�������
		JUMP,           //�W�����v����
		MODECHENGE,     //�A�N�V�������[�h��ύX
		TURNAROUND,     //��������
		TAKEDIVE,       //�_�C�u�U��������
		STUCKWALL,      //�ǂɈ����t��
		MAX
	};

	CTutorial(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::TUTORIAL, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_NONE);//�R���X�g���N�^
	~CTutorial();              //�f�X�g���N�^
	HRESULT Init() override;   //����������
	void Uninit() override;    //�I������
	void Update() override;    //�X�V����
	void Draw() override;      //�`�揈��
	void SetDeath() override;  //���S�t���O�ݒ菈��

	static CTutorial* Create();//��������

	//�`���[�g���A�����N���A�������ǂ���
	const bool& GetSuccess() const { return m_bSuccess; }
	void SetSuccess(bool bSuccess) { m_bSuccess = bSuccess; }

	void SetDrawReset();//�`���[�g���A���N���A�󋵂̕\�������Z�b�g

	void SetSuccessCheck(CHECK Type)
	{//�N���A�����`���[�g���A���Ƀ`�F�b�N������
		if (m_bSuccess == false)
		{
			m_pCheckPoint[static_cast<int>(Type)]->SetUseDraw(true);
		}
	}

private:
	CUi* m_pTutorialTex;                             //�`���[�g���A���e�N�X�`���ւ̃|�C���^

	CUi* m_pCheckPoint[static_cast<int>(CHECK::MAX)];//�����������ǂ���

	bool m_bSuccess;                                 //�����������ǂ���

};

#endif // !_TUTORIAL_H_