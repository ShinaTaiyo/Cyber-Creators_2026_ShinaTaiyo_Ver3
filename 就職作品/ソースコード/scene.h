//===============================================================================
//
//�U���P�V���F��ʑJ�ڂ���������
//Author:ShinaTaiyo
//
//===============================================================================

#ifndef _SCENE_H_  
#define _SCENE_H_


//========================================
//�C���N���[�h
//========================================
#include "main.h"
//===============================================================================

//========================================
//�V�[���N���X
//========================================
class CScene
{
public:
	//��ʂ̎��
	typedef enum
	{
		MODE_TITLE = 0,                       //�^�C�g�����
		MODE_GAME,                            //�Q�[�����
		MODE_RESULT,                          //���U���g���
		MODE_EDIT,                            //�G�f�B�b�g���
		MODE_MAX					          
	}MODE;							          
									          
	CScene();                                 //�R���X�g���N�^
	virtual ~CScene();                        //�f�X�g���N�^
	virtual HRESULT Init();                   //����������
	virtual void Uninit();                    //�I������
	virtual void Update();                    //�X�V����
	virtual void Draw();                      //�`�揈��
	static CScene* Create(MODE mode);         //��������
	static MODE GetMode() { return m_Mode; }  //���݂̃��[�h���擾����
private:							   
	static MODE m_Mode;                       //���݂̃��[�h
};
#endif