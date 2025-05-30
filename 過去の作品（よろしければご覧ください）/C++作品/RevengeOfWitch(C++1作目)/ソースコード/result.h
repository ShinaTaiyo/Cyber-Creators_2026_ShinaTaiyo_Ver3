//===============================================================================
//
//�U���P�V���F��ʑJ�ڂ���������[result.h]
//Author:ShinaTaiyo
//
//===============================================================================

#ifndef _RESULT_H_  
#define _RESULT_H_


//========================================
//�C���N���[�h
//========================================
#include "scene.h"
#include "ui.h"
//===============================================================================

//========================================
//�Q�[���N���X
//========================================
class CResult : public CScene
{
public:
	CResult();                           //�R���X�g���N�^
	~CResult() override;                 //�f�X�g���N�^
	HRESULT Init() override;            //����������
	void Uninit() override;             //�I������
	void Update() override;             //�X�V����
	void Draw() override;               //�`�揈��
private:
	CUi* m_pPressEnterLogo;             //PressEnter�\���p
};
#endif