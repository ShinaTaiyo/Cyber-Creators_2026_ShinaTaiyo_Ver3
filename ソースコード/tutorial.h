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
	{
		MOVE = 0,
		SHOT,
		CAMERACONTROLL,
		DASH,
		JUMP,
		MODECHENGE,
		TURNAROUND,
		TAKEDIVE,
		STUCKWALL,
		MAX
	};

	CTutorial(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::TUTORIAL, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_NONE);//�R���X�g���N�^
	~CTutorial();//�f�X�g���N�^
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void SetDeath() override;

	static CTutorial* Create();

	const bool& GetSuccess() const { return m_bSuccess; }
	void SetSuccess(bool bSuccess) { m_bSuccess = bSuccess; }

	void SetDrawReset();

	void SetSuccessCheck(CHECK Type)
	{
		if (m_bSuccess == false)
		{
			m_pCheckPoint[static_cast<int>(Type)]->SetUseDraw(true);
		}
	}

private:
	CUi* m_pTutorialTex;

	CUi* m_pCheckPoint[static_cast<int>(CHECK::MAX)];//�����������ǂ���

	bool m_bSuccess;//�����������ǂ���

};

#endif // !_TUTORIAL_H_