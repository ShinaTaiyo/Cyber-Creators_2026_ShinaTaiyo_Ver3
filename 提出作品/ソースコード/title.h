//===============================================================================
//
//�U���P�V���F��ʑJ�ڂ���������[title.h]
//Author:ShinaTaiyo
//
//===============================================================================

#ifndef _TITLE_H_  
#define _TITLE_H_


//========================================
//�C���N���[�h
//========================================
#include "scene.h"
#include "bg3d.h"
#include "player.h"
#include "ui.h"
//===============================================================================

//========================================
//�^�C�g���N���X
//========================================
class CTitle : public CScene
{
public:
	CTitle();                           //�R���X�g���N�^
	~CTitle() override;                 //�f�X�g���N�^
	HRESULT Init() override;            //����������
	void Uninit() override;             //�I������
	void Update() override;             //�X�V����
	void Draw() override;               //�`�揈��

	static CPlayer* GetPlayer() { return m_pPlayer; }
private:
	static CPlayer* m_pPlayer;                   //�v���C���[
	static CBg3D* m_pBg3D;                       //3D�w�i

	CUi* m_pTitleLogo;                           //�^�C�g�����S
	bool m_bMoveSwitch;                          //�ړ��X�C�b�`

    //====================================
	//�v���g�^�C�v�錾
	//====================================
	void PlayerProcess();                        //�v���C���[�̏���
};
#endif