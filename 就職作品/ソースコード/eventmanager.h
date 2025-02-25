//====================================================
//
//�X���P�T���F�{�X�o���C�x���g�����[eventmanager.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _EVENTMANAGER_H_  
#define _EVENTMANAGER_H_

//======================
//�C���N���[�h
//======================
#include "main.h"
#include "object.h"
#include "ui.h"
//====================================================

//======================
//�O���錾
//======================
class CEventManager;
//==========================================

//=====================================================
//�C�x���g�X�e�[�g
//=====================================================

//�X�[�p�[�N���X
class CNowEvent
{
public:
	CNowEvent() {};                                        //�R���X�g���N�^
	virtual ~CNowEvent() {};                               //�f�X�g���N�^
	virtual void Process(CEventManager* pEventManager) {}; //����
};

//�t�F�[�Y�ڍs�N���X
class CNowEvent_NextPhase : public CNowEvent
{
public:
	CNowEvent_NextPhase(CUi * pUI,int nPhaseNum,float fValueWidth,float fValueHeight); //�R���X�g���N�^
	~CNowEvent_NextPhase() override;                                                   //�f�X�g���N�^
	void Process(CEventManager* pEventManager) override;                               //����
	static const int& GetNumNextPhaseEvent() { return s_nNumNextPhaseEvent; }          
private:
	CUi* m_PhaseText;                                                                  //�t�F�[�Y�̕���
	static int s_nNumNextPhaseEvent;                                                   //���̃t�F�[�Y�Ɉڍs����C�x���g�̐�
};

//�`���[�g���A���N���X
class CNowEvent_Tutorial : public CNowEvent
{
public:
	CNowEvent_Tutorial();                                                              //�R���X�g���N�^
	~CNowEvent_Tutorial();                                                             //�f�X�g���N�^
	void Process(CEventManager* pEventManager) override;							   //����
};

//===================================================================================================

//==========================================
//�C�x���g�}�l�[�W���[�N���X
//==========================================
class CEventManager : public CObject
{
public:
	//===================================================
    //�C�x���g�񋓌^
    //===================================================
	enum class EVENTTYPE
	{
		NONE = 0, //�Ȃ�
		NEXTPHASE,//�t�F�[�Y������
		TUTORIAL, //�`���[�g���A��
		MAX
	};
	//=========================================================================================

	//====================================================
    //�C�x���g�i�s���\����
    //====================================================
	struct EventProgressInfo
	{
		int nEventPattern = 0;     //�C�x���g�̃p�^�[��
		int nCntEventTime = 0;     //�C�x���g�̎��Ԃ𑪂�
		bool bEventProgress = false;   //�C�x���g���i�s�����ǂ���
		EVENTTYPE EventType = EVENTTYPE::NONE;//���݂̃C�x���g

		void NextPattern();      //�p�^�[�������ɐi�߂�
		void ResetPattern();     //�p�^�[�������Z�b�g����
	};
	//=========================================================================================
	CEventManager(CNowEvent * pNowEvent, int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::EVENTMANAGER, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_NONE);
	~CEventManager();        //�f�X�g���N�^
	HRESULT Init() override;          //����������
	void Uninit() override;           //�I������
	void Update() override;           //�X�V����
	void Draw() override;             //�`�揈��
	void SetDeath() override;         //���S�t���O�ݒ菈��

	void ChengeEvent(CNowEvent* pNowEvent);//�C�x���g��ς���

	static CEventManager* Create(CNowEvent * pNowEvent);//��������
	static const int GetEventManagerNum() { return s_nNumEventManager; }
	void SetEndEvent(bool bEnd);//�C�x���g���I��点��t���O��ݒ肷��
	EventProgressInfo & GetEventProgressInfo(){ return m_EventProgressInfo; }
private:
	EventProgressInfo m_EventProgressInfo;//�C�x���g�i�s���Ǘ�����ϐ�
	CNowEvent* m_pNowEvent;               //���݂̃C�x���g�̏��
	static int s_nNumEventManager;        //�C�x���g�}�l�[�W���[�̐�
};

#endif
