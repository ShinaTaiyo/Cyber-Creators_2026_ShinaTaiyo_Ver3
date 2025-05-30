//====================================================
//
//�T���Q�Q���FC++�Ŋ�ՂÂ���LV4[manager.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _MANAGER_H_  
#define _MANAGER_H_

//======================
//�C���N���[�h
//======================
#include "main.h"
#include "scene.h"
//==========================================

//======================
//�O���錾
//======================
class CRenderer;        //�����_���[�N���X�O���錾
class CInputKeyboard;   //�L�[�{�[�h�N���X��O���錾
class CInputJoypad;     //�W���C�p�b�h�N���X��O���錾
class CSound;           //�T�E���h�N���X��O���錾
class CCamera;          //�J�����N���X��O���錾
class CLight;           //���C�g�N���X��O���錾
class CTexture;         //�e�N�X�`���N���X��O���錾
class CObjectXInfo;     //�I�u�W�F�N�gX�N���X��O���錾
class CSceneFade;       //�t�F�[�h�N���X��O���錾
class CInputMouse;      //�}�E�X
class CDebugText;       //�f�o�b�O�����N���X
//==========================================

//==========================================
//�}�l�[�W���N���X
//==========================================
class CManager
{
public:
	CManager();                                                         //�R���X�g���N�^
	~CManager();                                                        //�f�X�g���N�^
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);         //����������
	void Uninit();                                                      //�I������
	void Update();                                                      //�X�V����
	void Draw();                                                        //�`�揈��
	static CRenderer* GetRenderer();                                    //�����_���[�N���X���擾
	static CInputKeyboard* GetInputKeyboard();                          //�L�[�{�[�h�N���X���擾
	static CInputJoypad* GetInputJoypad();                              //�W���C�p�b�h�N���X���擾
	static CSound* GetSound() { return m_pSound; }                      //�T�E���h�N���X���擾
	static CCamera* GetCamera() { return m_pCamera; }                   //�J�����N���X���擾
	static CLight* GetLight() { return m_pLight; }                      //���C�g�N���X���擾
	static CTexture* GetTexture() { return m_pTexture; }                //�e�N�X�`���N���X���擾
	static CObjectXInfo* GetObjectXInfo() { return m_pObjectXInfo; }    //�I�u�W�F�N�gX�N���X���擾����
	static CSceneFade* GetSceneFade() { return m_pSceneFade; }                         //�t�F�[�h�N���X���擾����
	static CScene* GetScene() { return m_pScene; }                      //�V�[���N���X���擾����
	static CInputMouse* GetInputMouse() { return m_pInputMouse; }       //�}�E�X���͏����擾����
	static CDebugText* GetDebugText() { return m_pDebugText; }          //�f�o�b�O�e�L�X�g���
	static void SetMode(CScene::MODE mode);                             //���[�h��ݒ肷��
private:
	static CRenderer* m_pRenderer;                                      //�����_���[
	static CInputKeyboard* m_pInputKeyboard;                            //�L�[�{�[�h����
	static CInputJoypad* m_pInputJoypad;                                //�W���C�p�b�h
	static CSound* m_pSound;                                            //�T�E���h            
	static CCamera* m_pCamera;                                          //�J�������
	static CLight* m_pLight;                                            //���C�g���
	static CTexture* m_pTexture;                                        //�e�N�X�`�����
	static CObjectXInfo* m_pObjectXInfo;                                //�I�u�W�F�N�gX���
	static CScene* m_pScene;                                            //�V�[�����
	static CSceneFade* m_pSceneFade;                                    //�t�F�[�h���	
	static CInputMouse* m_pInputMouse;                                  //�}�E�X���
	static CDebugText* m_pDebugText;                                    //�f�o�b�O�e�L�X�g���
}; 
//=======================================================================================


#endif
