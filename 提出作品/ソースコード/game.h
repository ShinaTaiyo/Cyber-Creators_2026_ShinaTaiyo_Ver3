//===================================================================================================================
//
// �Q�O�Q�T�N�U���W���FOBB����[game.h]
// Author:ShinaTaiyo
//
//===================================================================================================================

//*******************************************************************************************************************
// �Q�d�C���N���[�h�h�~
//*******************************************************************************************************************
#ifndef _GAME_H_  
#define _GAME_H_

//*******************************************************************************************************************
// �C���N���[�h
//*******************************************************************************************************************
#include "main.h"
#include "scene.h"

//*******************************************************************************************************************
// �O���錾
//*******************************************************************************************************************
class CPhaseManager; // �t�F�[�Y�}�l�[�W���[
class CStageManager; // �X�e�[�W�}�l�[�W���[
class CPauseManager; // �|�[�Y�}�l�[�W���[
class CTutorial;     // �`���[�g���A��
class CScore;        // �X�R�A
class CCombo;        // �R���{
class CPlayer;       // �v���C���[
class CAttackEnemy;  // �G

//*******************************************************************************************************************
// �Q�[���N���X
//*******************************************************************************************************************
class CGame : public CScene
{
public:
	// === ����֐� ===

	CGame(bool bUseGamePad); // �R���X�g���N�^
	~CGame() override;       // �f�X�g���N�^

	// === ���C�t�T�C�N�������o�֐� ===

	HRESULT Init() override; // ����������
	void Uninit() override;  // �I������
	void Update() override;  // �X�V����

	// == �����o�֐� ===

	// �Q�[�����N���A�������ǂ���
	static void SetGameClear(bool bClear); // �ݒ�
	static const bool& GetGameClear();     // �擾

	static CPlayer* GetPlayer(); // �v���C���[�ւ̃|�C���^�擾
	static CPhaseManager* GetPhaseManager(); // �t�F�[�Y�}�l�[�W���[�擾
	static CStageManager* GetStageManager(); // �X�e�[�W�}�l�[�W���[���擾����
	static CTutorial* GetTutorial(); // �`���[�g���A��
	static CScore* GetScore(); // �X�R�A�ւ̃|�C���^�擾����
	static CCombo* GetCombo(); // �R���{�ւ̃|�C���^�擾����
	static const bool& GetPauseFlag(); // �|�[�Y�����ǂ������擾����
	static void SetChengePause(); // �|�[�Y��ԕύX�t���O��ON�ɂ���
private:
	// === �ÓI�����o�ϐ� ===

	static constexpr bool s_bUSETUTORIAL = true; // �`���[�g���A�����g�p���邩�ǂ���
	static int s_nPhaseNum;   // �t�F�[�Y�ԍ�
	static bool 
		s_bGameClear,   // �Q�[�����N���A�������ǂ���
	    s_bPAUSE,       // �|�[�Y���邩�ǂ���
	    s_bCHENGEPAUSE; // �|�[�Y��ԕύX�t���O
	static CPlayer* m_pPlayer;  // �v���C���[
	static CStageManager* m_pStageManager; // �X�e�[�W�}�l�[�W���[
	static CPhaseManager* m_pPhaseManager; // �t�F�[�Y�}�l�[�W���[
	static CTutorial* m_pTutorial; // �`���[�g���A��
	static CScore * s_pSCORE; // �X�R�A
	static CCombo* s_pCOMBO;  // �R���{

	// === �����o�ϐ� ===

	CPauseManager* m_pPauseManager; // �|�[�Y�}�l�[�W���[
	CAttackEnemy* m_pAttackEnemy; // �G�̍U��
	// === �����o�֐� ===

	void PauseProcess(); // �|�[�Y����
};
#endif