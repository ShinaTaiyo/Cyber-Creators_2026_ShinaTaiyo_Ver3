//===============================================================================
//
//�Q�O�Q�T�N�R���R�P���F�|�[�Y��������������[game.h]
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
#include "pausemanager.h"
#include "score.h"
#include "combo.h"
#include "tutorial.h"
//===============================================================================

//========================================
//�Q�[���N���X
//========================================
class CGame : public CScene
{
public:
	CGame(bool bUseGamePad);                                               //�R���X�g���N�^
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
	static CScore* GetScore() { return s_pSCORE; }                         //�X�R�A�ւ̃|�C���^���擾����
	static CCombo* GetCombo() { return s_pCOMBO; }                         //�R���{�ւ̃|�C���^���擾����
	static const bool& GetPauseFlag() { return s_bPAUSE; }                 //�|�[�Y�����ǂ������擾����
	static void SetChengePause() { s_bCHENGEPAUSE = true; }                //�|�[�Y��ԕύX�t���O��ON�ɂ���
private:

	//===========================================
	//�ÓI�����o�ϐ�
	//===========================================
	//�ʏ�
	static constexpr bool s_bUSETUTORIAL = true;                           //�`���[�g���A�����g�p���邩�ǂ���
	static CPlayer* m_pPlayer;                                             //�v���C���[
	static CStageManager* m_pStageManager;                                 //�X�e�[�W�}�l�[�W���[
	static CPhaseManager* m_pPhaseManager;                                 //�t�F�[�Y�}�l�[�W���[
	static CTutorial* m_pTutorial;                                         //�`���[�g���A��
	static CScore * s_pSCORE;                                              //�X�R�A
	static CCombo* s_pCOMBO;                                               //�R���{
	static int s_nPhaseNum;                                                //�t�F�[�Y�ԍ�
	static bool s_bGameClear;                                              //�Q�[�����N���A�������ǂ���
	static bool s_bPAUSE;                                                  //�|�[�Y���邩�ǂ���
	static bool s_bCHENGEPAUSE;                                            //�|�[�Y��ԕύX�t���O
	//====================================================================================================================-

	//===========================================
	//�ϐ�
	//===========================================
	CPauseManager* m_pPauseManager;                                        //�|�[�Y�}�l�[�W���[
	//====================================================================================================================-

	//===========================================
	//�֐�
	//===========================================
	void PauseProcess();                                                   //�|�[�Y����
	//====================================================================================================================-

};
#endif