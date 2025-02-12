//====================================================
//
//�U���R�O���F���@������[magic.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _MAGIC_H_  
#define _MAGIC_H_

//======================
//�C���N���[�h
//======================
#include "main.h"
#include "ui.h"
#include "object2d.h"
#include "objectX.h"
//==========================================

//==========================================

//==========================================
//���@�\���N���X
//==========================================
class CMagicDisp : public CObject2D
{
public:
	typedef enum
	{
		MAGICDISP00_FIRE = 0,   //���@�\���O�F�t�@�C�A
		MAGICDISP01_BEAM,       //���@�\���P�F�r�[��
		MAGICDISP02_ICE,        //���@�\���Q�F�A�C�X
		MAGICDISP03_THUNDER,    //���@�\���R�F�T���_�[
		MAGICDISP04_SWORD,      //���@�\���S�F��
		MAGICDISP05_PLANTSHIELD,//���@�\���T�F�v�����g�V�[���h
		MAGICDISP06_QUICKHEAL,  //���@�\���U�F�N�C�b�N�q�[��
		MAGICDISP07_CROSSBOMB,  //���@�\���V�F�N���X�{��
		MAGICDISP08_BLACKHOLE,  //���@�\���W�F�u���b�N�z�[��
		MAGICDISP09_PHYCOBURST, //���@�\���X�F�T�C�R�o�[�X�g
		MAGICDISP_MAX
	}MAGICDISPTYPE;
	CMagicDisp();                                                            //�R���X�g���N�^
	~CMagicDisp();                                                           //�f�X�g���N�^
	HRESULT Init() override;                                                 //����������
	void Uninit() override;                                                  //�I������
	void ExtraUninit() override;                                             //�ʘg�̏I������
	void Update() override;                                                  //�X�V����
	void Draw() override;                                                    //�`�揈��
	void SetDeath() override;                                                //���S�t���O��ݒ�
	static CMagicDisp* Create(MAGICDISPTYPE Type,float fWidth, float fHeight, D3DXVECTOR3 pos); //�Q�[�W�𐶐�
	void SetMagicDispType(MAGICDISPTYPE Type);                               //���@�\���̎�ނ�ς���
	MAGICDISPTYPE GetMagicDispType() { return m_Type; }                      //���݂̖��@�\�����擾����

private:
	MAGICDISPTYPE m_Type;                                                    //���@�\���̎��
	static const char* m_MAGICDISP_FILENAME[MAGICDISP_MAX];                  //���@�\���̃t�@�C���l�[��
	CUi* m_pLongPushUi;                                                      //�������̕���UI�̕\��
	
	//==============================
	//���@���X�g
	//==============================
	//static CMagicDisp* m_pMagicDisp_Top;                  //�X�e�[�W�}�l�[�W���[�ŊǗ����Ă���擪�I�u�W�F�N�g�ւ̃|�C���^
	//static CMagicDisp* m_pMagicDisp_Cur;                  //�X�e�[�W�}�l�[�W���[�ŊǗ����Ă��Ō���I�u�W�F�N�g�ւ̃|�C���^
	//CMagicDisp* m_pMagicDisp_Next;                        //�X�e�[�W�}�l�[�W���[�ŊǗ����Ă��鎟�̃I�u�W�F�N�g�ւ̃|�C���^
	//CMagicDisp* m_pMagicDisp_Prev;                        //�X�e�[�W�}�l�[�W���[�ŊǗ����Ă���O�̃I�u�W�F�N�g�ւ̃|�C���^
	//bool m_bChooseState;                                    //�I�΂�Ă����Ԃɂ���
	//================================================================================================
};
//============================================================================================================

//==========================================
//���@�E�C���h�E�N���X
//==========================================
class CMagicWindow : public CObject2D
{
public:
	CMagicWindow();             //�R���X�g���N�^
	~CMagicWindow() override;   //�f�X�g���N�^
	HRESULT Init() override;    //����������
	void Uninit() override;     //�I������
	void ExtraUninit() override;//�ʘg�̏I������
	void Update() override;     //�X�V����
	void Draw() override;       //�`�揈��
	void SetDeath() override;   //���S�t���O��ݒ�
	static CMagicWindow* Create(float fWidth, float fHeight, D3DXVECTOR3 pos);//���@�E�C���h�E�𐶐�
	int m_nHaveMagic;             //���ݎ����Ă��閂�@
private:
};
//==========================================================================================================================

#endif
