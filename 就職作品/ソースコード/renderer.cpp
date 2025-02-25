//==========================================
//
//�T���Q�O���F��Ղ̃N���X����
//Author:ShinaTaiyo
//
//==========================================

//===========================
//�C���N���[�h
//===========================
#include "renderer.h"
#include "object.h"
#include "object2d.h"
#include "player.h"
#include "bg.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "manager.h"
#include "camera.h"
#include "scene.h"
#include "game.h"
#include "debugtext.h"
#include "edit.h"
//==========================================

//===========================
//�ÓI�����o
//===========================
const int CRenderer::m_nMAXDEBUG = 2;//�f�o�b�O�\���ő吔
//==========================================

//===========================
//�R���X�g���N�^
//===========================
CRenderer::CRenderer() : m_pFont(),m_pD3D(),m_pD3DDevice(),m_mode(MODE_TITLE)
{
}
//==============================================================================

//===========================
//�f�X�g���N�^
//===========================
CRenderer::~CRenderer()
{

}
//================================================================================

//======================================================================
//����������
//======================================================================
HRESULT CRenderer::Init(HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;//�f�B�X�v���C���[�h
	D3DPRESENT_PARAMETERS d3dpp;//�v���[���e�[�V�����p�����[�^

	m_pD3DDevice = NULL;

	//DirectX3D�I�u�W�F�N�g�̐���
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}
	//���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}
	//�f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));//�p�����[�^�̃[���N���A
	d3dpp.BackBufferWidth = SCREEN_WIDTH;//�Q�[����ʃT�C�Y�i���j
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;//�Q�[����ʃT�C�Y�i�����j
	d3dpp.BackBufferFormat = d3ddm.Format;//�o�b�N�o�b�t�@�̌`��
	d3dpp.BackBufferCount = 1;//�o�b�N�o�b�t�@�̐�
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;//�_�u���o�b�t�@�̐؂�ւ��i�f���M���ɓ����j
	d3dpp.EnableAutoDepthStencil = TRUE;//�f�v�X�o�b�t�@�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;//�f�v�X�o�b�t�@�Ƃ���16bit���g��
	d3dpp.Windowed = bWindow;//�E�C���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;//���t���b�V�����[�g
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;//�C���^�[�o��

	//Direct3D�f�o�C�X�̐����i�`�揈���ƒ��_�������n�[�h�E�F�A�ōs���j
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&m_pD3DDevice)))
	{
		//Direct3D�f�o�C�X�̐����i�`�揈���̓n�[�h�E�F�A�A���_������CPU�ōs��
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&m_pD3DDevice)))
		{
			//Direct3D�f�o�C�X�̐����i�`�揈���ƒ��_������CPU�ōs���j
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&m_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	//�����_�[�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//�T���v���[�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//�e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);


	//�f�o�b�N�\���p�t�H���g�̐���
	D3DXCreateFont(m_pD3DDevice, 18, 0, 0, 0,
		FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,
		"terminal", &m_pFont);

	srand((unsigned int)time(NULL));//�Q�[�����J�n�������Ԗ��ɗ����̎��ݒ�

	////���[�h�̐ݒ�
	SetMode(m_mode);
	return S_OK;
}
//===================================================================================================================================

//=============================
//�I������
//=============================
void CRenderer::Uninit()
{
	//�f�o�b�N�\���p�t�H���g�̔j��
	if (m_pFont != NULL)
	{
		m_pFont->Release();
		m_pFont = NULL;
	}


	//Direct3D�f�o�C�X�̔j��
	if (m_pD3DDevice != NULL)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}

	//Direct3D�I�u�W�F�N�g�̔j��
	if (m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}
}
//===============================================================================

//=============================
//�X�V����
//=============================
void CRenderer::Update()
{
	CObject::UpdateAll();//�S�ẴI�u�W�F�N�g�̍X�V�������s��
}
//================================================================================

//=============================
//�`�揈��
//=============================
void CRenderer::Draw()
{
	//��ʃN���A�i�o�b�N�o�b�t�@��Z�o�b�t�@�̃N���A)
	m_pD3DDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(255,255,255,255), 1.0f, 0);

 	CCamera* pCamera = CManager::GetCamera();//�J�����ւ̃|�C���^
	
	//�`��J�n
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{//�`��J�n�����������ꍇ

		//�J�����̕`��
		pCamera->SetCamera();

		//FPS�̕\��
		CObject::DrawAll();//�S�ẴI�u�W�F�N�g�̕`�揈��

		//�f�o�b�O�\��
		CManager::GetDebugText()->Draw();

		//FPS�\��
		DrawFPS();

		//�`��I��
		m_pD3DDevice->EndScene();
	}

	//�o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);

}
//=========================================================================================================

//=====================================
//�f�o�b�O�\������
//=====================================
void CRenderer::DrawFPS()
{

	char aMode[64] = {};
	int nNumObject = CObject::GetNumAll();
    switch (CScene::GetMode())
    {
    case CScene::MODE_TITLE:
    	strcpy(&aMode[0], "�^�C�g��");
    	break;
    case CScene::MODE_GAME:
    	strcpy(&aMode[0], "�Q�[��");
    	break;
    case CScene::MODE_RESULT:
    	strcpy(&aMode[0], "���U���g");
    	break;
    case CScene::MODE_EDIT:
    	strcpy(&aMode[0], "�G�f�B�b�g");
    	break;
    default:
    	break;
    }

	//������ɑ��
	CManager::GetDebugText()->PrintDebugText("�y�f�o�b�O�\���z�yF1�z�yFPS�z %d\n", GetFPS());
	CManager::GetDebugText()->PrintDebugText("���݂̃��[�h�F%s\n",&aMode[0]);
	CManager::GetDebugText()->PrintDebugText("���݂̃I�u�W�F�N�g�����F%d\n", nNumObject);
}
//=========================================================================================================


//===========================================
//���[�h�̐ݒ�
//===========================================
void CRenderer::SetMode(MODE mode)
{
	m_mode = mode;//���[�h�̐ݒ�
}
//=================================================================================

//============================================
//�f�o�C�X�̎擾
//============================================
LPDIRECT3DDEVICE9 CRenderer::GetDevice()
{
	return m_pD3DDevice;
}
//==================================================================================================