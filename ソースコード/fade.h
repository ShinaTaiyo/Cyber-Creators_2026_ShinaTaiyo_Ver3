//===============================================================================
//
//�U���P�V���F�t�F�[�h��������������[fade.h]
//Author:ShinaTaiyo
//
//===============================================================================

#ifndef _FADE_H_  
#define _FADE_H_


//========================================
//�C���N���[�h
//========================================
#include "main.h"
#include "object2d.h"
#include "manager.h"
//===============================================================================

//========================================
//�t�F�[�h�N���X
//========================================
class CFade : public CObject2D
{
public:
	typedef enum
	{//�t�F�[�h���[�h�񋓌^
		FADEMODE_NONE = 0,//�t�F�[�h�����Ă��Ȃ�
		FADEMODE_IN,  //�t�F�[�h�C��
		FADEMODE_OUT, //�t�F�[�h�A�E�g
		FADEMODE_MAX
	}FADEMODE;
	CFade(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::FADE, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_2D);//�R���X�g���N�^
	~CFade() override;                //�f�X�g���N�^
	HRESULT Init() override;          //����������
	void Uninit() override;           //�I������
	void Update() override;           //�X�V����
	void Draw() override;             //�`�揈��
	void SetDeath() override;         //���S�t���O��ݒ�
	void SetFade(FADEMODE FadeMode);  //�t�F�[�h���[�h��ݒ肷��
	static CFade* Create();           //��������
protected:
	void SetFadeCnt(int nFadeCnt) { m_nFadeCnt = nFadeCnt; }                      //�t�F�[�h��Ԃ̃t���[������ݒ肷��
	int& GetFadeCnt() { return m_nFadeCnt; }                                      //�t�F�[�h��Ԃ̃t���[�������擾����
	void SetMaxFadeCnt(int nMaxFadeCnt) { m_nMaxFadeCnt = nMaxFadeCnt; }          //�t�F�[�h��Ԃ̍ő�t���[������ݒ肷��
	int& GetMaxFadeCnt() { return m_nMaxFadeCnt; }                                //�t�F�[�h��Ԃ̍ő�t���[�������擾����
	void SetFadeMode(FADEMODE mode) { m_FadeMode = mode;}                         //�t�F�[�h���[�h��ݒ肷��
	void SetStartFade(bool bUse) { m_bStartFade = bUse; }                         //�t�F�[�h�J�n�t���O��ݒ肷��
	const bool GetStartFade() const { return m_bStartFade; }                      //�t�F�[�h���J�n���Ă��邩�ǂ������擾
private:
	int m_nFadeCnt;                   //�t�F�[�h�����鎞�̃J�E���g!
	int m_nMaxFadeCnt;                //�t�F�[�h�J�E���g�ő�l!
	float m_fAlpha;                   //�t�F�[�h�̃A���t�@�l!
	bool m_bStartFade;                //�t�F�[�h���J�n����t���O
	FADEMODE m_FadeMode;              //�t�F�[�h�̃��[�h!
};
//====================================================================================================================

//========================================
//�V�[���t�F�[�h�N���X
//========================================
class CSceneFade : public CFade
{
public:
	CSceneFade();                          //�R���X�g���N�^
	~CSceneFade() override;                //�f�X�g���N�^
	HRESULT Init() override;               //����������
	void Uninit() override;                //�I������
	void Update() override;                //�X�V����
	void Draw() override;                  //�`�揈��
	void SetDeath() override;              //���S�t���O��ݒ�
	static CSceneFade* Create();           //��������
	void SetSceneFade(FADEMODE FadeMode, CScene::MODE mode);  //�t�F�[�h�̃��[�h��ݒ肷��
private:
	CScene::MODE m_NextMode;               //���̃��[�h�����߂�  !
};
//====================================================================================================================
#endif