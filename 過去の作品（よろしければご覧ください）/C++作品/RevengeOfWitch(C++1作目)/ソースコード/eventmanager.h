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
//====================================================

//======================
//�O���錾
//======================
class CUi;
class CDirectionFade;

//==========================================

//==========================================
//�C�x���g�}�l�[�W���[�N���X
//==========================================
class CEventManager : public CObject
{
public:
	//===================================================
	//�C�x���g�񋓌^
	//===================================================
	typedef enum
	{
		EVENTTYPE_NONE = 0,      //�C�x���g�Ȃ�
		EVENTTYPE_BOSSAPPEAR,    //�{�X�o��C�x���g
		EVENTTYPE_MAX
	}EVENTTYPE;
	//=========================================================================================

	//===================================================
	//�{�X�o�����
	//===================================================
	typedef enum
	{
		BOSSAPPEAR_KINGSLIME = 0,//�L���O�X���C�����o��
		BOSSAPPEAR_DEATHENAGA,   //�f�X�G�i�K���o��
		BOSSAPPEAR_MRSTRONG,     //Mr�X�g�����O���o��
		BOSSAPPEAR_RILEY,        //���C���[���o��
		BOSSAPPEAR_DRAIYA,       //�h���C�����o��
		BOSSAPPEAR_GABRIEL,      //�K�u���G�����o��
		BOSSAPPEAR_MAX
	}BOSSAPPEARTYPE;

	CEventManager();
	~CEventManager();        //�f�X�g���N�^
	HRESULT Init() override;          //����������
	void Uninit() override;           //�I������
	void Update() override;           //�X�V����
	void Draw() override;             //�`�揈��
	void SetDeath() override;         //���S�t���O�ݒ菈��
	void SetEvent(EVENTTYPE type) { m_NowEventType = type; m_bEventProgress = true;}//�C�x���g��ݒ肷��
	CEventManager* Create(EVENTTYPE type);//��������

	void SetBossAppearEvent(BOSSAPPEARTYPE BossAppearType) { m_NowEventType = EVENTTYPE_BOSSAPPEAR; m_BossAppearType = BossAppearType; m_bEventProgress = true; }
	EVENTTYPE GetEventType() { return m_NowEventType; }//���݂̃C�x���g���擾����
private:
	EVENTTYPE m_NowEventType;//���݂̃C�x���g
	int m_nEventPattern;      //�C�x���g�̃p�^�[��
	int m_nCntEventTime;     //�C�x���g�̎��Ԃ𑪂�
	bool m_bEventProgress;   //�C�x���g���i�s�����ǂ���
	void SetDeathOnly();     //�{�̃I�u�W�F�N�g�ȊO����������
	void NextPattern();      //�p�^�[�������ɐi�߂�
	void ResetPattern();     //�p�^�[�������Z�b�g����

	void BossAppearEventProcess();//�{�X�o��C�x���g�̏���
	CUi* m_pMagicCircle;          //���@�w�ւ̃|�C���^
	CUi* m_pBossBattleText;       //�{�X�o�g���̃e�L�X�gUi�ւ̃|�C���^
	CUi* m_pBossCutIn;            //�{�X�̃J�b�g�C��
	CUi* m_pBossNameUi;           //�{�X�̖��O��UI
	CDirectionFade* m_pDirectionFade;//���o�p�t�F�[�h

	BOSSAPPEARTYPE m_BossAppearType;//�o��������{�X�̎��

};

#endif
