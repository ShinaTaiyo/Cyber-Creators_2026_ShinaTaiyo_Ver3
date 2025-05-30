//===============================================================================
//
//�V���P�P���F�I�u�W�F�N�g��Release���@��ύX[edit.h]
//Author:ShinaTaiyo
//
//===============================================================================

#ifndef _EDIT_H_  
#define _EDIT_H_


//========================================
//�C���N���[�h
//========================================
#include "scene.h"
#include "stagemanager.h"
#include "player.h"
#include "bg3d.h"
#include "field.h"
//===============================================================================

//========================================
//�G�f�B�b�g�N���X
//========================================
class CEdit : public CScene
{
public:
	CEdit();                           //�R���X�g���N�^
	~CEdit() override;                 //�f�X�g���N�^
	HRESULT Init() override;           //����������
	void Uninit() override;            //�I������
	void Update() override;            //�X�V����
	static CPlayer* GetPlayer() { return m_pPlayer; }           //�v���C���[���擾����
	static CStageManager* GetStageManager() { return m_pStageManager; }    //�X�e�[�W�}�l�[�W���[���擾����
private:
	static CStageManager* m_pStageManager;                                 //�X�e�[�W�}�l�[�W���[
	static CPlayer* m_pPlayer;                                             //�v���C���[
	static CBg3D* m_pBg3D;                                                 //3D�w�i
	static CField* m_pField;                                               //�t�B�[���h
};
#endif