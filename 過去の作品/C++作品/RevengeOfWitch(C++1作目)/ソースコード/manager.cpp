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
#include "bullet.h"
#include "renderer.h"
#include "manager.h"
#include "stagemanager.h"
#include "debugtext.h"
#include "input.h"
#include "explosion.h"
#include "enemy.h"
#include "effect.h"
#include "particle.h"
#include "block.h"
#include "item.h"
#include "sound.h"
#include "camera.h"
#include "light.h"
#include "object3d.h"
#include "field.h"
#include "model.h"
#include "texture.h"
#include "objectXInfo.h"
#include "number.h"
#include "main.h"
#include "fade.h"
#include "marker.h"
#include "boss.h"
#include "attack.h"
#include "eventmanager.h"
//===================================================

//=======================
//�ÓI�����o
//=======================
CRenderer* CManager::m_pRenderer = nullptr;
CInputKeyboard* CManager::m_pInputKeyboard = nullptr;
CInputJoypad* CManager::m_pInputJoypad = nullptr;
CSound* CManager::m_pSound = nullptr;
CCamera* CManager::m_pCamera = nullptr;
CLight* CManager::m_pLight = nullptr;
CTexture* CManager::m_pTexture = nullptr;
CObjectXInfo* CManager::m_pObjectXInfo = nullptr;
CScene* CManager::m_pScene = nullptr;
CSceneFade* CManager::m_pSceneFade = nullptr;
CDebugText* CManager::m_pDebugText = nullptr;
CEventManager* CManager::m_pEventManager = nullptr;
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
	m_pRenderer->Init(hInstance, hWnd, bWindow);

	//======================================================

	//============================
	//�L�[���͏��̐���
	//============================
	m_pInputKeyboard = new CInputKeyboard;
	m_pInputKeyboard->Init(hInstance, hWnd);

	//======================================================

	//============================
	//�W���C�p�b�h���͏��̐���
	//============================
	m_pInputJoypad = new CInputJoypad;
	m_pInputJoypad->Init(hInstance, hWnd);
	//======================================================

	//============================
	//�T�E���h�̐���
	//============================
	m_pSound = DBG_NEW CSound;
	m_pSound->InitSound(hWnd);
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

	//===================================
	//�f�o�b�O�v���V�[�W���N���X�𐶐�
	//===================================
	m_pDebugText = DBG_NEW CDebugText;
	m_pDebugText->Init();
	//======================================================

	//===================================
	//�C�x���g�}�l�[�W���[�N���X�𐶐�
	//===================================
	m_pEventManager = DBG_NEW CEventManager;
	m_pEventManager->Init();
	//=====================================================

	//===================================
	//���[�h�̐ݒ�
	//===================================
	m_pSceneFade = CSceneFade::Create();
	SetMode(CScene::MODE_TITLE);
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
	//m_pStageManager->SaveMapTxt();//�}�b�v�����Z�[�u����i��������Ă���j�����鏈�����J�n�j
	//CObject::ReleaseAll();    //�I�u�W�F�N�g�̊J��
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
	//�T�E���h�̊J��
	//================================
	if (m_pSound != nullptr)
	{
		m_pSound->UninitSound();
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
	//�f�o�b�O�v���V�[�W�����
	//================================
	if (m_pDebugText != nullptr)
	{
		m_pDebugText->Uninit();
		delete m_pDebugText;
		m_pDebugText = nullptr;
	}
	//===============================================

	//================================
	//�C�x���g�}�l�[�W���[���
	//================================
	if (m_pEventManager != nullptr)
	{
		m_pEventManager->SetUseDeath(true);
		m_pEventManager->SetDeath();
		m_pEventManager = nullptr;
	}
	//===============================================

	//================================
	//�e�e�N�X�`���AX�t�@�C���̔j������
	//================================
	CPlayer::Unload();          //�v���C���[�̃e�N�X�`���j��
	CEffect::Unload();          //�G�t�F�N�g�̃e�N�X�`���̔j��
	CField::Unload();           //�t�B�[���h�̃e�N�X�`���̔j��
	//===============================================

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
	m_pCamera->Update();       //�J����
	m_pLight->Update();        //���C�g
#ifdef _DEBUG
	m_pDebugText->Update();    //�f�o�b�O�e�L�X�g
#endif // _DEBUG


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
	if (m_pScene != nullptr)
	{
		m_pScene->Draw();        //�V�[���̍X�V����
	}

	m_pRenderer->Draw();         //�����_���[
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
	{
		m_pScene->Uninit();
		delete m_pScene;
		m_pScene = nullptr;
	}
	
	//����
	m_pScene = CScene::Create(mode);
}
