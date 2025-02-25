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
#include "bg3d.h"
#include "field.h"
#include "player.h"
#include "main.h"
#include "eventmanager.h"
#include "timer.h"
#include "difficulty.h"
//===============================================================================

//========================================
//�Q�[���N���X
//========================================
class CGame : public CScene
{
public:
	CGame();                                                               //�R���X�g���N�^
	~CGame() override;                                                     //�f�X�g���N�^
	HRESULT Init() override;                                               //����������
	void Uninit() override;                                                //�I������
	void Update() override;                                                //�X�V����
	static CPlayer* GetPlayer() { return m_pPlayer; }                      //�v���C���[���擾����
	static CStageManager* GetStageManager() { return m_pStageManager; }    //�X�e�[�W�}�l�[�W���[���擾����
	static void SetDifficulty(CDifficulty::DIFFICULTYTYPE DifficultyType) { m_DifficultyType = DifficultyType; }//��Փx��ݒ肷��
	static CDifficulty::DIFFICULTYTYPE GetDifficulty() { return m_DifficultyType; }//��Փx���擾����
private:
	static CStageManager* m_pStageManager;                                 //�X�e�[�W�}�l�[�W���[
	static CPlayer* m_pPlayer;                                             //�v���C���[
	static CBg3D* m_pBg3D;                                                 //3D�w�i
	static CField* m_pField;                                               //�t�B�[���h
	static CTimer* m_pTimer;                                               //�^�C�}�[
	static CEventManager* m_pEventManager;                                 //�C�x���g�}�l�[�W���[
	static CDifficulty::DIFFICULTYTYPE m_DifficultyType;                   //��Փx

};
#endif