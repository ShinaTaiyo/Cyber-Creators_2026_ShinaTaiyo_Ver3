//====================================================
//
//C++�Ŋ�ՂÂ���
//Author:ShinaTaiyo
//
//====================================================

#ifndef _RENDERER_H_  
#define _RENDERER_H_

//======================
//�C���N���[�h
//======================
#include "d3dx9.h"//�`�揈���ɕK�v
#include "main.h"
//==========================================

//==========================================
//�����_���[�N���X
//==========================================
class CRenderer
{
public:
	//================================
    //���[�h�̗񋓌^
    //================================
	typedef enum
	{
		MODE_TITLE = 0,        //�^�C�g�����
		MODE_TUTORIAL,         //�`���[�g���A����ʂ��Ăяo��
		MODE_CHOOSE_DIFFICULTY,//��Փx�I�����[�h���Ăяo���B
		MODE_GAME,             //�Q�[�����
		MODE_RESULT,           //���U���g���
		MODE_RANKING,          //�����L���O���
		MODE_MAX
	}MODE;
	//====================================================================================

	CRenderer();                             //�R���X�g���N�^
	~CRenderer();                            //�f�X�g���N�^
	HRESULT Init(HWND hWnd, BOOL bWindow);   //����������
	void Uninit();                           //�I������
	void Update();                           //�X�V����
	void Draw();                             //�`�揈��
	void SetMode(MODE mode);                 //���[�h�̐ݒ�
	LPDIRECT3DDEVICE9 GetDevice();           //�f�o�C�X�̎擾
	static const int m_nMAXDEBUG;            //�f�o�b�O�\���̐�
private:
	LPD3DXFONT m_pFont;                      //�t�H���g�ւ̃|�C���^!
	LPDIRECT3D9 m_pD3D;                      //Direct3D!
	LPDIRECT3DDEVICE9 m_pD3DDevice;          //Direct3D�f�o�C�X!
	MODE m_mode;                             //���݂̃��[�h!
	void DrawFPS();                          //�f�o�b�O�\������

};

#endif
