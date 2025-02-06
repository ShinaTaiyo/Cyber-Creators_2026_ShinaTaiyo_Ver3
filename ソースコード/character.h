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
//�N���X
//=====================================
class CCharacter : public CObjectX
{
public:
	struct KEY
	{
		D3DXVECTOR3 Pos = { 0.0f,0.0f,0.0f };//�ʒu
		D3DXVECTOR3 Rot = { 0.0f,0.0f,0.0f };//����
	};

	struct KEYSET
	{
		int nFrame = 0;    //�t���[����
		vector<KEY> VecKey;//�e���f���p�[�c�̃L�[���
	};

	struct MOTION
	{
		bool bLoop = false;       //���[�v�����邩�ǂ���
		int nNumKey = 0;          //�L�[�̑���
		vector<KEYSET> VecKeySet; //�L�[�Z�b�g���
	};

	struct CHARACTERMOTIONDATA
	{
		int nNumMotion = 0;       //���[�V��������
		string FileName = "";     //���[�V�����t�@�C����
		vector<MOTION> VecMotion; //���[�V�������
	};

	CCharacter(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::NONE, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_X);                                                           //�R���X�g���N�^
	~CCharacter() override; //�f�X�g���N�^
	HRESULT Init() override;//����������
	void Uninit() override; //�I������
	void Update() override; //�X�V����
	void Draw() override;   //�`�揈��
	void SetDeath() override;//���S�t���O�ݒ菈��
	void SetNextMotion(int nNext);
	void SetDamage(int nDamage, int nHitStopTime) override; //�_���[�W��^����
	static int RegistMotion(string String,CCharacter * pCharacter);//���[�V�������̃t�@�C���p�X���w�肵�A���[�V�������̃C���f�b�N�X�ƃ��f���p�[�c���擾
	void SetSize() override;
	vector<CModelParts*>& GetVecModelParts() { return m_VecModelParts; }//���f���p�[�c�̓��I�z����擾
protected:
	void MotionProcess();//���[�V�����������s���i�ʒu�̍X�V�Ƃ�����Ȃ��̂ŁA�h���N���X�̍X�V�̍Ō�ŌĂт����̂�public�ɒu���j
private:
	//=======================
	//�ÓI�����o
	//=======================
	static vector<CHARACTERMOTIONDATA> s_VecMotionInfo;//���[�V�������̃x�N�^�[
	static constexpr float s_fMotionBrend_Speed = 0.4f;//���[�V�����u�����h�̑��x��ݒ�
	static constexpr float s_fMotionBrend_CheckDefference = 0.1f;//���[�V�����u�����h�̂���Ƃ��̖ړI�̒l�܂ł̍��������̒l�ȉ��Ȃ犮��
	//===================================================================================

	//=======================
	//�\����
	//=======================
	struct NowMotionInfo
	{
		int nNowMotion = 0;//���݂̃��[�V�����ԍ�
		int nNextMotion = 0;//���̃��[�V����
		int nMotionOld = 0;//1f�O�̃��[�V����

		int nCntFrame = 0; //���݂̃t���[����
		int nCntKey = 0;   //���݂̃L�[���J�E���g����
		bool bNowBrending = false;//���[�V�����u�����h�����ǂ���
	};
	//===================================================================================

	//=======================
	//�ϐ�
	//=======================
	int m_nIdxCharacter;//�L�����N�^�[���[�V�������̃C���f�b�N�X
	NowMotionInfo m_NowMotionInfo;//���[�V������ԏ��
	vector <CModelParts*> m_VecModelParts;//���f���p�[�c�̃x�N�^�[
	//===================================================================================

	//=======================
	//�֐�
	//=======================
	void MotionInfoChengeProcess();//���[�V���������X�V���鏈��

	//�ÓI�����o
	static void SaveMotion(string MotionFileName);//���[�V��������ۑ�����
	static void LoadModelParts(string MotionFileName, CCharacter* pCharacter);//���f���p�[�c��ǂݍ���
	//===================================================================================
};
//============================================================================================================
#pragma once
