//===================================================
//
//�N���X�Ŋ�ՂÂ���[Manager.cpp]
//Author;ShinaTaiyo
//
//===================================================

//=======================
//�C���N���[�h
//=======================
#include "object.h"
#include "bg.h"
#include "player.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "sound.h"
#include "camera.h"
#include "light.h"
#include "object3d.h"
#include "texture.h"
#include "eventmanager.h"
#include "debugtext.h"
#include "objectXInfo.h"
#include "main.h"
#include "fade.h"
//===================================================

//=======================
//�ÓI�����o
//=======================
CRenderer* CManager::m_pRenderer = nullptr;              //�����_���[
CInputKeyboard* CManager::m_pInputKeyboard = nullptr;    //�L�[���͏��
CInputJoypad* CManager::m_pInputJoypad = nullptr;        //�W���C�p�b�h���͏��
CSound* CManager::m_pSound = nullptr;                    //�T�E���h���
CCamera* CManager::m_pCamera = nullptr;                  //�J����
CLight* CManager::m_pLight = nullptr;                    //���C�g
CTexture* CManager::m_pTexture = nullptr;                //�e�N�X�`�����
CObjectXInfo* CManager::m_pObjectXInfo = nullptr;        //�I�u�W�F�N�gX���
CScene* CManager::m_pScene = nullptr;                    //�V�[��
CSceneFade* CManager::m_pSceneFade = nullptr;            //�V�[���J�ڗp�t�F�[�h
CInputMouse* CManager::m_pInputMouse = nullptr;          //�}�E�X���͏��
CDebugText* CManager::m_pDebugText = nullptr;            //�f�o�b�O�\��
//===================================================

//=======================
//�R���X�g���N�^
//=======================
CManager::CManager()
{

}
//===================================================

//=======================
//�f�X�g���N�^
//=======================
CManager::~CManager()
{

}

//================================
//����������
//================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	//============================
	//�����_���[�̐���
	//============================
	m_pRenderer = DBG_NEW CRenderer;
	m_pRenderer->Init(hWnd, bWindow);

	//======================================================

	//============================
	//�L�[���͏��̐���
	//============================
	m_pInputKeyboard = DBG_NEW CInputKeyboard;
	m_pInputKeyboard->Init(hInstance, hWnd);

	//======================================================

	//============================
	//�W���C�p�b�h���͏��̐���
	//============================
	m_pInputJoypad = DBG_NEW CInputJoypad;
	m_pInputJoypad->Init(hInstance, hWnd);
	//======================================================

	//===================================
	//�}�E�X���͏��̐ݒ�
	//===================================
	m_pInputMouse = DBG_NEW CInputMouse;
	m_pInputMouse->Init(hInstance, hWnd);
	//======================================================

	//============================
	//�T�E���h�̐���
	//============================
	m_pSound = DBG_NEW CSound;
	m_pSound->Init(hWnd);
	//======================================================

	//============================
	//�J�����̐���
	//============================
	m_pCamera = DBG_NEW CCamera;
	m_pCamera->Init();
	//======================================================

	//============================
	//���C�g�̐���
	//============================
	m_pLight = DBG_NEW CLight;
	m_pLight->Init();
	//======================================================

	//============================
	//�e�N�X�`���N���X�̐���
	//============================
	m_pTexture = DBG_NEW CTexture;
	//======================================================


	//===============================
	//�I�u�W�F�N�gX���N���X�𐶐�
    //===============================
	m_pObjectXInfo = DBG_NEW CObjectXInfo;
	//======================================================

	//============================
	//�f�o�b�O�e�L�X�g�𐶐�
	//============================
	m_pDebugText = DBG_NEW CDebugText;
	m_pDebugText->Init();

	//===================================
	//���[�h�̐ݒ�
	//===================================
	SetMode(CScene::MODE_TITLE);
	m_pSceneFade = CSceneFade::Create();
	m_pSceneFade->SetIsStopUpdatePause(false);//�|�[�Y���ł��X�V����
	m_pSceneFade->SetSceneFade(CFade::FADEMODE_OUT, CScene::MODE_TITLE);
	//=====================================================

	//============================
	//�}�b�v����ݒ�
	//============================

	return S_OK;
}
//================================================================================

//=======================
//�I������
//=======================
void CManager::Uninit()
{	
	//================================
	//�V�[��
	//================================
	if (m_pScene != nullptr)
	{
		m_pScene->Uninit();
		delete m_pScene;
		m_pScene = nullptr;
	}

	//================================
	//�����_���[�̊J��
	//================================
	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = nullptr;
	}
	//===============================================

	//================================
	//�L�[���͏��̊J��
	//================================
	if (m_pInputKeyboard != nullptr)
	{
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = nullptr;
	}
	//===============================================

	//================================
	//�W���C�p�b�h���͏��̊J��
	//================================
	if (m_pInputJoypad != nullptr)
	{
		m_pInputJoypad->Uninit();
		delete m_pInputJoypad;
		m_pInputJoypad = nullptr;
	}
	//===============================================
	
	//================================
	//�}�E�X���͏��̊J��
	//================================
	if (m_pInputMouse != nullptr)
	{
		m_pInputMouse->Uninit();
		delete m_pInputMouse;
		m_pInputMouse = nullptr;
	}
	//===============================================

	//================================
	//�T�E���h�̊J��
	//================================
	if (m_pSound != nullptr)
	{
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = nullptr;
	}
	//===============================================

	//================================
	//�J����
	//================================
	if (m_pCamera != nullptr)
	{
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = nullptr;
	}
	//===============================================

	//================================
	//���C�g
	//================================
	if (m_pLight != nullptr)
	{
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = nullptr;
	}
	//===============================================
	
	//================================
	//�e�N�X�`��
	//================================
	if (m_pTexture != nullptr)
	{
		m_pTexture->Unload();
		delete m_pTexture;
		m_pTexture = nullptr;
	}
	//===============================================

	//================================
	//�I�u�W�F�N�gX���
	//================================
	if (m_pObjectXInfo != nullptr)
	{
		m_pObjectXInfo->Unload();
		delete m_pObjectXInfo;
		m_pObjectXInfo = nullptr;
	}
	//===============================================

	//================================
	//�t�F�[�h���
	//================================
	if (m_pSceneFade != nullptr)
	{
		m_pSceneFade->SetUseDeath(true);
		m_pSceneFade->SetDeath();
		m_pSceneFade = nullptr;
	}
	//===============================================

	//================================
	//�f�o�b�O�e�L�X�g���
	//================================
	if (m_pDebugText != nullptr)
	{
		m_pDebugText->Uninit();
		delete m_pDebugText;
		m_pDebugText = nullptr;
	}
	//===============================================

	CObject::ReleaseAll();    //�I�u�W�F�N�g�̊J��

	//�Ō�Ɏc�����I�u�W�F�N�g�������őS���j��
	CObject::ReleaseProcess();//���X�g�̔j�������鏈��

	CObject::ReleaseAll();    //�I�u�W�F�N�g�̊J��

	//�Ō�Ɏc�����I�u�W�F�N�g�������őS���j��
	CObject::ReleaseProcess();//���X�g�̔j�������鏈��
}
//===================================================

//=======================
//�X�V����
//=======================
void CManager::Update()
{
	m_pRenderer->Update();     //�����_���[
	m_pInputKeyboard->Update();//�L�[����
	m_pInputJoypad->Update();  //�W���C�p�b�h����
	m_pInputMouse->Update();   //�}�E�X����
	m_pCamera->Update();       //�J����
	m_pLight->Update();        //���C�g
	if (m_pScene != nullptr)
	{
		m_pScene->Update();        //�V�[���̍X�V����
	}
}
//===================================================

//=======================
//�`�揈��
//=======================
void CManager::Draw()
{
	m_pRenderer->Draw();         //�����_���[

	if (m_pScene != nullptr)
	{
		m_pScene->Draw();        //�V�[���̍X�V����
	}
}
//===================================================

//=================================
//�����_���[�N���X���擾����
//=================================
CRenderer* CManager::GetRenderer()
{
	return m_pRenderer;
}
//===================================================

//=================================
//�L�[�{�[�h�N���X���擾����
//=================================
CInputKeyboard* CManager::GetInputKeyboard()
{
	return m_pInputKeyboard;
}
//===================================================

//=================================
//�W���C�p�b�h�N���X���擾����
//=================================
CInputJoypad* CManager::GetInputJoypad()
{
	return m_pInputJoypad;
}
//====================================================

//===========================================
//���[�h
//===========================================
void CManager::SetMode(CScene::MODE mode)
{

	if (m_pScene != nullptr)
	{//�V�[�������݂��Ă�����

		//�V�[����j��
		m_pScene->Uninit();
		delete m_pScene;
		m_pScene = nullptr;
	}
	
	//�V�����V�[���𐶐�
	m_pScene = CScene::Create(mode);
}
