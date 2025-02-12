//===============================================================================
//
//�X���Q�O���F��Փx�I������������[difficulty.h]
//Author:ShinaTaiyo
//
//===============================================================================

#ifndef _DIFFICULTY_H_  
#define _DIFFICULTY_H_


//========================================
//�C���N���[�h
//========================================
#include "scene.h"
#include "ui.h"
//===============================================================================

//========================================
//��Փx�N���X
//========================================
class CDifficulty : public CScene
{
public:
	CDifficulty();                           //�R���X�g���N�^
	~CDifficulty() override;                 //�f�X�g���N�^
	HRESULT Init() override;            //����������
	void Uninit() override;             //�I������
	void Update() override;             //�X�V����
	void Draw() override;               //�`�揈��

	typedef enum
	{
		DIFFICULTYTYPE_EASY = 0,//��Փx�F�ȒP
		DIFFICULTYTYPE_NORMAL,  //��Փx�F����
		DIFFICULTYTYPE_BOSSRUSH,//��Փx�F�{�X���b�V��
		DIFFICULTYTYPE_MAX
	}DIFFICULTYTYPE;

	static const int& GetDifficultyNum() { return s_nChooseDifficultyNum; }//��Փx�ԍ����擾
private:
	CUi* m_pDifficultyUi[DIFFICULTYTYPE_MAX];
	static int s_nChooseDifficultyNum;//�I��ł����Փx�̔ԍ�

	void ChooseDifficulty();//��Փx�I������
	void TextStaging(bool bChoose);     //�e�L�X�gUI�̉��o
};
#endif