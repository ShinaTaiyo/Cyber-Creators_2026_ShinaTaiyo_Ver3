//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//==============================================================================
//�T�E���h�N���X
//==============================================================================
class CSound
{
public:
	//==============================================================================
	// �T�E���h�ꗗ
    //==============================================================================
	typedef enum
	{
		SOUND_LABEL_BGM_NOESIS,       //Noesis
		SOUND_LABEL_BGM_DIFFICULTY000,//��΂𒴂���
		SOUND_LABEL_BGM_STAGE1,       //�X�e�[�W�Pbgm
		SOUND_LABEL_BGM_BOSS,         //BlackWingedWitch
		SOUND_LABEL_BGM_GHOSTBUTTLE,  //GhostBattle
		SOUND_LABEL_SE_DECISION_000,  //����{�^�����ʉ�
		SOUND_LABEL_SE_SELECT_000,    //�I���{�^�����ʉ�
		SOUND_LABEL_SE_THUNDER,       //��
		SOUND_LABEL_SE_FIRE,          //��
		SOUND_LABEL_SE_WARNING,       //�x��
		SOUND_LABEL_SE_PUNCH,         //�p���`
		SOUND_LABEL_SE_ICE000,        //�A�C�X�O
		SOUND_LABEL_SE_SLASH_000,     //�X���b�V���O
		SOUND_LABEL_SE_CLAW,          //��
		SOUND_LABEL_SE_VINE,          //��
		SOUND_LABEL_SE_HEAL,          //��
		SOUND_LABEL_SE_EXPLOSION_000, //����
		SOUND_LABEL_SE_DODGE_000,     //���
		SOUND_LABEL_SE_FIRE_002,      //���Q
		SOUND_LABEL_SE_PHYCOBURST_000,//�T�C�R�o�[�X�g
		SOUND_LABEL_SE_DAMAGE_000,    //�_���[�W
		SOUND_LABEL_SE_SHOT_000,      //���˂O
		SOUND_LABEL_SE_CHARGE_000,    //�`���[�W�O
		SOUND_LABEL_SE_SHOT_001,      //���˂P
		SOUND_LABEL_SE_SHOT_ROGKET,   //���P��������
		SOUND_LABEL_SE_SHOT_BURST,    //�o�[�X�g����
		SOUND_LABEL_SE_SHOT_RAIN,     //���C���V���b�g
		SOUND_LABEL_SE_JUMP_000,      //�W�����v
		SOUND_LABEL_SE_CHARGE_001,    //�`���[�W�P
		SOUND_LABEL_SE_COIN_000,      //�R�C���O
		SOUND_LABEL_SE_GETCLOWN_000,  //�����擾�O
		SOUND_LABEL_SE_GETACTION_000, //�A�N�V�����擾
		SOUND_LABEL_SE_GETMAGIC_000,  //���@�擾
		SOUND_LABEL_SE_GETSHOT_000,   //�V���b�g�擾
		SOUND_LABEL_SE_DAMAGE_001,    //�_���[�W�P
		SOUND_LABEL_SE_DECISION_001,  //����P
		SOUND_LABEL_SE_SHOT_002,      //���˂Q
		SOUND_LABEL_MAX,
	} SOUND_LABEL;
	//=================================================================================================================================

	//*****************************************************************************
	// �v���g�^�C�v�錾
	//*****************************************************************************
	CSound();                            //�R���X�g���N�^
	~CSound();                           //�f�X�g���N�^
	HRESULT InitSound(HWND hWnd);        //����������
	void UninitSound(void);              //�I������
	HRESULT PlaySound(SOUND_LABEL label);//�T�E���h�Đ�����
	void StopSound(SOUND_LABEL label);   //�T�E���h�X�g�b�v����
	void StopSound(void);                //�T�E���h�X�g�b�v�����i�I�[�o�[���[�h�j
	//=================================================================================================================================

private:
	//*****************************************************************************
	// �v���g�^�C�v�錾
	//*****************************************************************************
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD* pChunkSize, DWORD* pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void* pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);
	//=================================================================================================================================

		//*****************************************************************************
        // �T�E���h���̍\���̒�`
        //*****************************************************************************
		typedef struct
		{
			const char* pFilename;	// �t�@�C����
			int nCntLoop;		// ���[�v�J�E���g
		} SOUNDINFO;

		//*****************************************************************************
		// �O���[�o���ϐ�
		//*****************************************************************************
		IXAudio2* m_pXAudio2 = NULL;								// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
		IXAudio2MasteringVoice* m_pMasteringVoice = NULL;			// �}�X�^�[�{�C�X
		IXAudio2SourceVoice* m_apSourceVoice[SOUND_LABEL_MAX] = {};	// �\�[�X�{�C�X
		BYTE* m_apDataAudio[SOUND_LABEL_MAX] = {};					// �I�[�f�B�I�f�[�^
		DWORD m_aSizeAudio[SOUND_LABEL_MAX] = {};					// �I�[�f�B�I�f�[�^�T�C�Y

		// �T�E���h�̏��
		SOUNDINFO m_aSoundInfo[SOUND_LABEL_MAX] =
		{
			{"data/BGM/TitleBgm_Noesis.wav", -1},   //Noesis
			{"data/BGM/DifficultyBgm_000.wav", -1}, //��΂𒴂���
			{"data/BGM/stage1.wav", -1},            //EasyBgm
			{"data/BGM/bgm013_BlackWingedWitch.wav", -1},//BlackWingedWitch
			{"data/BGM/GhostButtle.wav", -1},       //GhostButtle
			{"data/SE/Decision_000.wav", 0},        //����L�[�̌��ʉ�
			{"data/SE/Select_000.wav", 0},          //�I���L�[�̌��ʉ�
			{"data/SE/Thunder.wav", 0},             //���̌��ʉ�
			{"data/SE/Fire.wav", 0},                //���̌��ʉ�
			{"data/SE/Warning_000.wav", 0},         //�x���\���̌��ʉ�
			{"data/SE/Punch.wav", 0},               //�p���`�̌��ʉ�
			{"data/SE/IceExplosion.wav", 0},        //�X�̌��ʉ�
			{"data/SE/Slash_000.wav", 0},           //�a���̌��ʉ�
			{"data/SE/Claw.wav", 0},                //�܂̌��ʉ�
			{"data/SE/Vine.wav", 0},                //��̌��ʉ�
			{"data/SE/Heal_000.wav", 0},            //�񕜂̌��ʉ�
			{"data/SE/Explosion_000.wav", 0},       //�����̌��ʉ�
			{"data/SE/Dodge.wav", 0},               //����̌��ʉ�
			{"data/SE/Fire_002.wav", 0},            //���Q�̌��ʉ�
			{"data/SE/PhycoBurst_001.wav", 0},      //�T�C�R�o�[�X�g�̌��ʉ�
			{"data/SE/damage_000.wav", 0},          //�_���[�W�̌��ʉ�
			{"data/SE/Shot_000.wav", 0},            //���˂O�̌��ʉ�
			{"data/SE/Charge_000.wav",0},           //�`���[�W�O�̌��ʉ�
			{"data/SE/Shot_001.wav",0},             //���˂P�̌��ʉ�
			{"data/SE/Shot_Rocket.wav",0},          //���P�������˂̌��ʉ�
			{"data/SE/BurstShot.wav",0},            //�o�[�X�g���˂̌��ʉ�
			{"data/SE/RainShot_000.wav",0},         //���C���V���b�g�̌��ʉ�
			{"data/SE/Jump.wav",0},                 //�W�����v�̌��ʉ�
			{"data/SE/Charge_001.wav",-1},          //�`���[�W�̌��ʉ��P
			{"data/SE/Coin_000.wav",0},             //�R�C���O
			{"data/SE/GetClown.wav",0},             //�����擾�O
			{"data/SE/GetAction.wav",0},            //�A�N�V�����擾�O
			{"data/SE/GetMagic.wav",0},             //���@�擾�O
			{"data/SE/GetShot.wav",0},              //�V���b�g�擾�O
			{"data/SE/Damage_001.wav",0},           //�_���[�W�P
			{"data/SE/Decision_001.wav",0},         //����P
			{"data/SE/Shot_003.wav",0},             //���˂R
		};

};

#endif
