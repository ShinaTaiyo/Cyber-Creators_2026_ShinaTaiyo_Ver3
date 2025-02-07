//===============================================================================
//
//�U���P�V���F��ʑJ�ڂ���������[game.h]
//Author:ShinaTaiyo
//
//===============================================================================

#ifndef _GAME_H_  
#define _GAME_H_


//========================================
//�C���N���[�h
//========================================
#include "scene.h"
#include "stagemanager.h"
#include "bgModel.h"
#include "player.h"
#include "main.h"
#include "phasemanager.h"
#include "eventmanager.h"
#include "tutorial.h"
//===============================================================================

//========================================
//�Q�[���N���X
//========================================
class CGame : public CScene
{
public:
	CGame(bool bUseGamePad);                                                               //�R���X�g���N�^
	~CGame() override;                                                     //�f�X�g���N�^
	HRESULT Init() override;                                               //����������
	void Uninit() override;                                                //�I������
	void Update() override;                                                //�X�V����
	static CPlayer* GetPlayer() { return m_pPlayer; }                      //�v���C���[���擾����
	static void SetGameClear(bool bClear) { s_bGameClear = bClear; }       //�Q�[�����N���A�������ǂ�����ݒ�
	static const bool& GetGameClear() { return s_bGameClear; }             //�Q�[�����N���A�������ǂ������擾����
	static CPhaseManager* GetPhaseManager() { return m_pPhaseManager; }    //�t�F�[�Y�}�l�[�W���[���擾����
	static CStageManager* GetStageManager() { return m_pStageManager; }    //�X�e�[�W�}�l�[�W���[���擾����
	static CTutorial* GetTutorial() { return m_pTutorial; }                //�`���[�g���A��
private:
	static CPlayer* m_pPlayer;                                             //�v���C���[
	static CStageManager* m_pStageManager;                                 //�X�e�[�W�}�l�[�W���[
	static CPhaseManager* m_pPhaseManager;                                 //�t�F�[�Y�}�l�[�W���[
	static CTutorial* m_pTutorial;                                         //�`���[�g���A��
	static int s_nPhaseNum;                                                //�t�F�[�Y�ԍ�
	static bool s_bGameClear;                                              //�Q�[�����N���A�������ǂ���

};
#endif