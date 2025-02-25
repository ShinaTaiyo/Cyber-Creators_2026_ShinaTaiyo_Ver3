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
private:
	CUi* m_pPressEnterLogo;             //PressEnter�\���p
};
#endif