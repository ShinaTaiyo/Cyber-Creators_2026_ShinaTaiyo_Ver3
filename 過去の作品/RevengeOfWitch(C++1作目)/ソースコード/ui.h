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
//==========================================

//======================
//�O���錾
//======================
class CObject2D;

//==========================================
//UI�N���X
//==========================================
class CUi : public CObject2D
{
public:
	typedef enum
	{
		UITYPE_PLAYER2DILUST = 0,     //�v���C���[�C���X�g
		UITYPE_SLIMEILUST,            //�X���C���C���X�g
		UITYPE_MAGICCIRCLE,           //���@�w
		UITYPE_TITLELOGO,             //�^�C�g�����S
		UITYPE_PRESSENTER_LOGO,       //PressEnter���S
		UITYPE_PRESSENTERORSTART_LOGO,//PressEnterOrStart���S
		UITYPE_CHARACTERGAUGEFRAME,   //�L�����N�^�[�̃Q�[�W�t���[��
		UITYPE_FACENORMAL,            //���ʂ̊�
		UITYPE_FACEDAMAGE,            //�_���[�W���󂯂��Ƃ��̊�
		UITYPE_FACEGLAD,              //��������
		UITYPE_BOSSBATTLE_TEXT,       //�{�X�o�g�������\��
		UITYPE_CUTIN_KINGSLIME,       //�L���O�X���C���̃J�b�g�C��
		UITYPE_KINGSLIME_TEXT,        //�L���O�X���C���̕���
		UITYPE_CUTIN_DEATHENAGA,      //�f�X�G�i�K�̃J�b�g�C��
		UITYPE_DEATHENAGA_TEXT,       //�f�X�G�i�K�̕���
		UITYPE_CUTIN_MRSTRONG,        //Mr�X�g�����O�̃J�b�g�C��
		UITYPE_MRSTRONG_TEXT,         //Mr�X�g�����O�̕���
		UITYPE_CUTIN_RILEY,           //���C���[�̃J�b�g�C��
		UITYPE_RILEY_TEXT,            //���C���[�̕���
		UITYPE_CUTIN_DRAIYA,          //�h���C���̃J�b�g�C��
		UITYPE_DRAIYA_TEXT,           //�h���C���̕���
		UITYPE_CUTIN_GABRIEL,         //�K�u���G���̃J�b�g�C��
		UITYPE_GABRIEL_TEXT,          //�K�u���G���̕���
		UITYPE_EASY_TEXT,             //��ՓxEASY�̕���
		UITYPE_NORMAL_TEXT,           //��ՓxNORMAL�̕���
		UITYPE_CHOOSEDIFFICULTY_TEXT, //��Փx�I���̕���
		UITYPE_LONGPUSH_TEXT,         //�������̕���
		UITYPE_BOSSRUSH_TEXT,         //�{�X���b�V���̕���
		UITYPE_MAX
	}UITYPE;

	typedef enum
	{
		UIMOVETYPE_NORMAL = 0,//���ʂ̓���
		UIMOVETYPE_FLOATING,  //���V������
		UIMOVETYPE_MAX
	}UIMOVETYPE;

	CUi(int nPriority = 6);                      //�R���X�g���N�^
	virtual ~CUi();             //�f�X�g���N�^
	HRESULT Init() override;    //����������
	void Uninit() override;     //�I������
	void Update() override;     //�X�V����
	void Draw() override;       //�`�揈��
	void SetDeath() override;   //���S�t���O��ݒ�
	static CUi * Create(UITYPE type, CObject2D::POLYGONTYPE PolygonType,float fWidth, float fHeight,D3DXVECTOR3 Pos,D3DXVECTOR3 Move,D3DXVECTOR3 Rot,D3DXCOLOR col);//�w�i�̐���
	void SetUiType(UITYPE type);
	void SetUseUiEffect(bool bUse, int nSetEffectLife, D3DXCOLOR col) { m_bUseUiEffect = bUse; m_nSetUiEffectLife = nSetEffectLife; m_SetUiEffectColor = col; }
protected:
	UITYPE GetType() { return m_Type; }//UI�̎�ނ��擾
	static const char* m_apUI_FILENAME[UITYPE_MAX];//UI�̃e�N�X�`���t�@�C����
private:
	UITYPE m_Type;                 //�w�i�̎��
	UIMOVETYPE m_MoveType;         //UI�̓������̎��
	bool m_bUseUiEffect;           //UI�ŃG�t�F�N�g���o�����ǂ���

	D3DXCOLOR m_SetUiEffectColor;  //UI�ŏo���G�t�F�N�g�̐F�����ݒ�p
	int m_nSetUiEffectLife;        //UI�̃G�t�F�N�g�̗̑͐ݒ�p
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