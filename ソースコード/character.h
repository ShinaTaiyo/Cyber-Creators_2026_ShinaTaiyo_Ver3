//===========================================================================
//
//�Q�O�Q�T�N�P���W���F���[�V�������Ǘ��N���X����[motionData.h]
//Author:ShinaTaiyo
//
//===========================================================================

//=====================================
//�C���N���[�h
//=====================================
#include "main.h"
#include "objectX.h"
#include "modelparts.h"
//========================================================================

//=====================================
//�L�����N�^�[�N���X
//=====================================
class CCharacter : public CObjectX
{
public:
	struct KEY
	{//�L�[���
		D3DXVECTOR3 Pos = { 0.0f,0.0f,0.0f };//�ʒu
		D3DXVECTOR3 Rot = { 0.0f,0.0f,0.0f };//����
	};

	struct KEYSET
	{//�L�[�Z�b�g���
		int nFrame = 0;    //�t���[����
		vector<KEY> VecKey;//�e���f���p�[�c�̃L�[���
	};

	struct MOTION
	{//���[�V�������
		bool bLoop = false;       //���[�v�����邩�ǂ���
		int nNumKey = 0;          //�L�[�̑���
		vector<KEYSET> VecKeySet; //�L�[�Z�b�g���
	};

	struct CHARACTERMOTIONDATA
	{//�L�����N�^�[���Ƃ̃��[�V�������
		int nNumMotion = 0;       //���[�V��������
		string FileName = "";     //���[�V�����t�@�C����
		vector<MOTION> VecMotion; //���[�V�������
	};

	CCharacter(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::NONE, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_X); //�R���X�g���N�^                                                          //�R���X�g���N�^
	~CCharacter() override;                                              //�f�X�g���N�^
	HRESULT Init() override;                                             //����������
	void Uninit() override;                                              //�I������
	void Update() override;                                              //�X�V����
	void Draw() override;                                                //�`�揈��
	void SetDeath() override;                                            //���S�t���O�ݒ菈��
	void SetNextMotion(int nNext);                                       //���̃��[�V������ݒ肷��
	void SetDamage(int nDamage, int nHitStopTime) override;              //�_���[�W��^����
	static int RegistMotion(string String,CCharacter * pCharacter);      //���[�V�������̃t�@�C���p�X���w�肵�A���[�V�������̃C���f�b�N�X�ƃ��f���p�[�c���擾
	void SetSize() override;                                             //�T�C�Y��ݒ肷��
	vector<CModelParts*>& GetVecModelParts() { return m_VecModelParts; } //���f���p�[�c�̓��I�z����擾
protected:
	void MotionProcess();                                                //���[�V�����������s���i�ʒu�̍X�V�Ƃ�����Ȃ��̂ŁA�h���N���X�̍X�V�̍Ō�ŌĂт����̂�public�ɒu���j
private:
	//=======================
	//�ÓI�����o
	//=======================
	static vector<CHARACTERMOTIONDATA> s_VecMotionInfo;            //���[�V�������̓��I�z��
	static constexpr float s_fMotionBrend_Speed = 0.4f;            //���[�V�����u�����h�̑��x��ݒ�
	static constexpr float s_fMotionBrend_CheckDefference = 0.1f;  //���[�V�����u�����h�̂���Ƃ��̖ړI�̒l�܂ł̍��������̒l�ȉ��Ȃ犮��
	//===================================================================================

	//=======================
	//�\����
	//=======================
	struct NowMotionInfo
	{//���݂̃��[�V�������
		int nNowMotion = 0;          //���݂̃��[�V�����ԍ�
		int nNextMotion = 0;         //���̃��[�V����
		int nMotionOld = 0;          //1f�O�̃��[�V����
							         
		int nCntFrame = 0;           //���݂̃t���[����
		int nCntKey = 0;             //���݂̃L�[���J�E���g����
		bool bNowBrending = false;   //���[�V�����u�����h�����ǂ���
	};
	//===================================================================================

	//=======================
	//�ϐ�
	//=======================
	int m_nIdxCharacter;                   //�L�����N�^�[���[�V�������̃C���f�b�N�X
	NowMotionInfo m_NowMotionInfo;         //���[�V������ԏ��
	vector <CModelParts*> m_VecModelParts; //���f���p�[�c�̓��I�z��
	//===================================================================================

	//=======================
	//�֐�
	//=======================

	//�ÓI�����o
	static void SaveMotion(string MotionFileName);                            //���[�V��������ۑ�����
	static void LoadModelParts(string MotionFileName, CCharacter* pCharacter);//���f���p�[�c��ǂݍ���
	//===================================================================================
};
//============================================================================================================
#pragma once
