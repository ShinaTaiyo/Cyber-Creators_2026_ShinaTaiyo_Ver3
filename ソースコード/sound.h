//=============================================================================
//
// サウンド処理 [sound.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//==============================================================================
//サウンドクラス
//==============================================================================
class CSound
{
public:
	//==============================================================================
	// サウンド一覧
    //==============================================================================
    enum class SOUND_LABEL
	{
		BGM_NOESIS,       //Noesis
		BGM_FINALBATTLE,  //FinalBattle
	    SE_SHOT_000,      //射撃０
		SE_SHOT_001,      //射撃１
	    SE_EXPLOSION_000, //爆発０
		SE_DAMAGE_000,    //ダメージ０
		SE_LANDING_000,   //着地０
		MAX,
	};
	//=================================================================================================================================

	//*****************************************************************************
	// プロトタイプ宣言
	//*****************************************************************************
	//メンバ関数
	CSound();                              //コンストラクタ
	~CSound();                             //デストラクタ
	HRESULT Init(HWND hWnd);               //初期化処理
	void Uninit();                         //終了処理
	HRESULT PlaySoundB(SOUND_LABEL label); //セグメント再生(再生中なら停止)
	void Stop(SOUND_LABEL label);          //セグメント停止(ラベル指定)
	void Stop();                           //セグメント停止(全て)

private:
	// サウンド情報の構造体定義
	typedef struct
	{
		const char* pFilename;	// ファイル名
		int nCntLoop;		    // ループカウント
	} SOUNDINFO;

	//メンバ関数
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD* pChunkSize, DWORD* pChunkDataPosition);// チャンクのチェック
	HRESULT ReadChunkData(HANDLE hFile, void* pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);// チャンクデータの読み込み

	//メンバ変数
	IXAudio2* m_pXAudio2 = NULL;								                // XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice* m_pMasteringVoice = NULL;			                // マスターボイス
	IXAudio2SourceVoice* m_apSourceVoice[static_cast<int>(SOUND_LABEL::MAX)];	// ソースボイス
	BYTE* m_apDataAudio[static_cast<int>(SOUND_LABEL::MAX)];					// オーディオデータ
	DWORD m_aSizeAudio[static_cast<int>(SOUND_LABEL::MAX)];					    // オーディオデータサイズ

	SOUNDINFO m_aSoundInfo[static_cast<int>(SOUND_LABEL::MAX)] =                // サウンドの情報
		{
			{"data/BGM/TitleBgm_Noesis.wav", -1},                               //BGM_Noesis
			{"data/BGM/Waterflame_FinalBattle.wav", -1},                        //BGM_Waterframe_FinalBattle.wav
			{"data/SE/Shot_000.wav",  0},                                       //SE_射撃０
			{"data/SE/Shot_001.wav",  0},                                       //SE_射撃０
			{"data/SE/Explosion_000.wav",  0},                                  //SE_爆発０
			{"data/SE/Damage_000.wav",  0},                                     //SE_ダメージ０
			{"data/SE/Landing_000.wav",  0},                                    //SE_着地
		};

};

#endif