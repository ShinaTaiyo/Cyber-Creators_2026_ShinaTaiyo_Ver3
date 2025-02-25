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
    enum class SOUND_LABEL
	{
		BGM_NOESIS,       //Noesis
		BGM_FINALBATTLE,  //FinalBattle
	    SE_SHOT_000,      //�ˌ��O
		SE_SHOT_001,      //�ˌ��P
	    SE_EXPLOSION_000, //�����O
		SE_DAMAGE_000,    //�_���[�W�O
		SE_LANDING_000,   //���n�O
		MAX,
	};
	//=================================================================================================================================

	//*****************************************************************************
	// �v���g�^�C�v�錾
	//*****************************************************************************
	//�����o�֐�
	CSound();                              //�R���X�g���N�^
	~CSound();                             //�f�X�g���N�^
	HRESULT Init(HWND hWnd);               //����������
	void Uninit();                         //�I������
	HRESULT PlaySoundB(SOUND_LABEL label); //�Z�O�����g�Đ�(�Đ����Ȃ��~)
	void Stop(SOUND_LABEL label);          //�Z�O�����g��~(���x���w��)
	void Stop();                           //�Z�O�����g��~(�S��)

private:
	// �T�E���h���̍\���̒�`
	typedef struct
	{
		const char* pFilename;	// �t�@�C����
		int nCntLoop;		    // ���[�v�J�E���g
	} SOUNDINFO;

	//�����o�֐�
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD* pChunkSize, DWORD* pChunkDataPosition);// �`�����N�̃`�F�b�N
	HRESULT ReadChunkData(HANDLE hFile, void* pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);// �`�����N�f�[�^�̓ǂݍ���

	//�����o�ϐ�
	IXAudio2* m_pXAudio2 = NULL;								                // XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	IXAudio2MasteringVoice* m_pMasteringVoice = NULL;			                // �}�X�^�[�{�C�X
	IXAudio2SourceVoice* m_apSourceVoice[static_cast<int>(SOUND_LABEL::MAX)];	// �\�[�X�{�C�X
	BYTE* m_apDataAudio[static_cast<int>(SOUND_LABEL::MAX)];					// �I�[�f�B�I�f�[�^
	DWORD m_aSizeAudio[static_cast<int>(SOUND_LABEL::MAX)];					    // �I�[�f�B�I�f�[�^�T�C�Y

	SOUNDINFO m_aSoundInfo[static_cast<int>(SOUND_LABEL::MAX)] =                // �T�E���h�̏��
		{
			{"data/BGM/TitleBgm_Noesis.wav", -1},                               //BGM_Noesis
			{"data/BGM/Waterflame_FinalBattle.wav", -1},                        //BGM_Waterframe_FinalBattle.wav
			{"data/SE/Shot_000.wav",  0},                                       //SE_�ˌ��O
			{"data/SE/Shot_001.wav",  0},                                       //SE_�ˌ��O
			{"data/SE/Explosion_000.wav",  0},                                  //SE_�����O
			{"data/SE/Damage_000.wav",  0},                                     //SE_�_���[�W�O
			{"data/SE/Landing_000.wav",  0},                                    //SE_���n
		};

};

#endif