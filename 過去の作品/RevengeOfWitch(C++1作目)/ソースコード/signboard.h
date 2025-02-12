//====================================================
//
//�X���R���F���ĊŔ���[signboard.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _SIGNBOARD_H_  
#define _SIGNBOARD_H_

//======================
//�C���N���[�h
//======================
#include "main.h"
#include "objectX.h"
#include "enemy.h"
//==========================================

//==========================================

//==========================================
//�T�C���{�[�h�N���X
//==========================================
class CSignBoard : public CObjectX
{
public:
	//===========================
	//�����̎��
	//===========================
	typedef enum
	{
		SIGNBOARDMANUALTYPE00_MOVE = 0,     //���ĊŔ������F�ړ�
		SIGNBOARDMANUALTYPE01_JUNP,         //���ĊŔ������F�W�����v
		SIGNBOARDMANUALTYPE02_DASH,         //���ĊŔ������F�_�b�V��
		SIGNBOARDMANUALTYPE03_WALLJUMP,     //���ĊŔ������F�ǃL�b�N
		SIGNBOARDMANUALTYPE04_SHOT,         //���ĊŔ������F�V���b�g
		SIGNBOARDMANUALTYPE05_CHARGESHOT,   //���ĊŔ������F�`���[�W�V���b�g
		SIGNBOARDMANUALTYPE06_RAINSHOT,     //���ĊŔ������F���C���V���b�g
		SIGNBOARDMANUALTYPE07_BURSTSHOT,    //���ĊŔ������F�o�[�X�g�V���b�g
		SIGNBOARDMANUALTYPE08_RAPIDSHOT,    //���ĊŔ������F���s�b�h�V���b�g
		SIGNBOARDMANUALTYPE09_MAGIC,        //���ĊŔ������F���@
		SIGNBOARDMANUALTYPE10_MAGICCHENGE,  //���ĊŔ������F���@�`�F���W
		SIGNBOARDMANUALTYPE11_DOUBLEJUMP,   //���ĊŔ������F�Q�i�W�����v
		SIGNBOARDMANUALTYPE_MAX
	}SIGNBOARDMANUALTYPE;
	//======================================================================

	//===========================
	//�����̎��
	//===========================
	typedef enum
	{
		SIGNBOARDTYPE00_NORMAL = 0,     //���ʂ̗��ĊŔ�
		SIGNBOARDTYPE_MAX
	}SIGNBOARDTYPE;
	//======================================================================

	//==========================
	//���ĊŔ̎��
	//==========================
	CSignBoard();                   //�R���X�g���N�^
	~CSignBoard() override;         //�f�X�g���N�^
	HRESULT Init() override;    //����������
	void Uninit() override;     //�I������
	void Update() override;     //�X�V����
	void Draw() override;       //�`�揈��
	void SetDeath() override;   //���S�t���O��ݒ�
	static int* GetSubTypeNum() { return &m_SIGNBOARDSUBTYPENUM[0]; }//���ꂼ��̃I�u�W�F�N�g�̃T�u�^�C�v���擾����
	static int GetNumFile() { return m_nNumFile; }//�t�@�C���̐����擾����
	static CSignBoard* Create(SIGNBOARDTYPE Type, SIGNBOARDMANUALTYPE ManualType, D3DXVECTOR3 pos, D3DXVECTOR3 Scale, D3DXVECTOR3 Rot);//�G�𐶐�
	void SetManualType(SIGNBOARDMANUALTYPE type) { m_ManualType = type; }//�����̎�ނ�ݒ肷��
	int GetSubType() { return m_nSubType; }
private:
	SIGNBOARDMANUALTYPE m_ManualType;     //�����̎��
	SIGNBOARDTYPE m_SignBoardType;        //���ĊŔ̎��
	static const int m_nNumFile = 1;                //���f���t�@�C���̑���
	static const char* m_apSIGNBOARD_TEXTURENAME[SIGNBOARDMANUALTYPE_MAX];//���ĊŔ̐����̃e�N�X�`���̑���
	static const char* m_apSIGNBOARD_TEXTURENAME_SUCCESS[SIGNBOARDMANUALTYPE_MAX];//���ĊŔ̐��������s���ꂽ��̃e�N�X�`��
	static const char* m_apSIGNBOARD_FILENAME[SIGNBOARDTYPE_MAX];//���ĊŔ̃t�@�C����
	static int m_SIGNBOARDSUBTYPENUM[SIGNBOARDTYPE_MAX];//���ĊŔ̃T�u�^�C�v

	int m_nSubType;
	bool m_bSuccessJudge;//���߂����삪�����������ǂ���

	void ManualShotProcess();//�ړ������Ŕ̏���
	void ManualJumpProcess();//�W�����v�����Ŕ̏���
	void ManualDashProcess();//�_�b�V�������Ŕ̏���
	void SummonTestEnemy();  //�����p�̓G���������鏈��
};

#endif