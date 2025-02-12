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
	typedef enum
	{
		SOUND_LABEL_BGM_NOESIS,       //Noesis
		SOUND_LABEL_BGM_DIFFICULTY000,//戦火を超えて
		SOUND_LABEL_BGM_STAGE1,       //ステージ１bgm
		SOUND_LABEL_BGM_BOSS,         //BlackWingedWitch
		SOUND_LABEL_BGM_GHOSTBUTTLE,  //GhostBattle
		SOUND_LABEL_SE_DECISION_000,  //決定ボタン効果音
		SOUND_LABEL_SE_SELECT_000,    //選択ボタン効果音
		SOUND_LABEL_SE_THUNDER,       //雷
		SOUND_LABEL_SE_FIRE,          //炎
		SOUND_LABEL_SE_WARNING,       //警告
		SOUND_LABEL_SE_PUNCH,         //パンチ
		SOUND_LABEL_SE_ICE000,        //アイス０
		SOUND_LABEL_SE_SLASH_000,     //スラッシュ０
		SOUND_LABEL_SE_CLAW,          //爪
		SOUND_LABEL_SE_VINE,          //つる
		SOUND_LABEL_SE_HEAL,          //回復
		SOUND_LABEL_SE_EXPLOSION_000, //爆発
		SOUND_LABEL_SE_DODGE_000,     //回避
		SOUND_LABEL_SE_FIRE_002,      //炎２
		SOUND_LABEL_SE_PHYCOBURST_000,//サイコバースト
		SOUND_LABEL_SE_DAMAGE_000,    //ダメージ
		SOUND_LABEL_SE_SHOT_000,      //発射０
		SOUND_LABEL_SE_CHARGE_000,    //チャージ０
		SOUND_LABEL_SE_SHOT_001,      //発射１
		SOUND_LABEL_SE_SHOT_ROGKET,   //ロケラン発射
		SOUND_LABEL_SE_SHOT_BURST,    //バースト発射
		SOUND_LABEL_SE_SHOT_RAIN,     //レインショット
		SOUND_LABEL_SE_JUMP_000,      //ジャンプ
		SOUND_LABEL_SE_CHARGE_001,    //チャージ１
		SOUND_LABEL_SE_COIN_000,      //コイン０
		SOUND_LABEL_SE_GETCLOWN_000,  //王冠取得０
		SOUND_LABEL_SE_GETACTION_000, //アクション取得
		SOUND_LABEL_SE_GETMAGIC_000,  //魔法取得
		SOUND_LABEL_SE_GETSHOT_000,   //ショット取得
		SOUND_LABEL_SE_DAMAGE_001,    //ダメージ１
		SOUND_LABEL_SE_DECISION_001,  //決定１
		SOUND_LABEL_SE_SHOT_002,      //発射２
		SOUND_LABEL_MAX,
	} SOUND_LABEL;
	//=================================================================================================================================

	//*****************************************************************************
	// プロトタイプ宣言
	//*****************************************************************************
	CSound();                            //コンストラクタ
	~CSound();                           //デストラクタ
	HRESULT InitSound(HWND hWnd);        //初期化処理
	void UninitSound(void);              //終了処理
	HRESULT PlaySound(SOUND_LABEL label);//サウンド再生処理
	void StopSound(SOUND_LABEL label);   //サウンドストップ処理
	void StopSound(void);                //サウンドストップ処理（オーバーロード）
	//=================================================================================================================================

private:
	//*****************************************************************************
	// プロトタイプ宣言
	//*****************************************************************************
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD* pChunkSize, DWORD* pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void* pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);
	//=================================================================================================================================

		//*****************************************************************************
        // サウンド情報の構造体定義
        //*****************************************************************************
		typedef struct
		{
			const char* pFilename;	// ファイル名
			int nCntLoop;		// ループカウント
		} SOUNDINFO;

		//*****************************************************************************
		// グローバル変数
		//*****************************************************************************
		IXAudio2* m_pXAudio2 = NULL;								// XAudio2オブジェクトへのインターフェイス
		IXAudio2MasteringVoice* m_pMasteringVoice = NULL;			// マスターボイス
		IXAudio2SourceVoice* m_apSourceVoice[SOUND_LABEL_MAX] = {};	// ソースボイス
		BYTE* m_apDataAudio[SOUND_LABEL_MAX] = {};					// オーディオデータ
		DWORD m_aSizeAudio[SOUND_LABEL_MAX] = {};					// オーディオデータサイズ

		// サウンドの情報
		SOUNDINFO m_aSoundInfo[SOUND_LABEL_MAX] =
		{
			{"data/BGM/TitleBgm_Noesis.wav", -1},   //Noesis
			{"data/BGM/DifficultyBgm_000.wav", -1}, //戦火を超えて
			{"data/BGM/stage1.wav", -1},            //EasyBgm
			{"data/BGM/bgm013_BlackWingedWitch.wav", -1},//BlackWingedWitch
			{"data/BGM/GhostButtle.wav", -1},       //GhostButtle
			{"data/SE/Decision_000.wav", 0},        //決定キーの効果音
			{"data/SE/Select_000.wav", 0},          //選択キーの効果音
			{"data/SE/Thunder.wav", 0},             //雷の効果音
			{"data/SE/Fire.wav", 0},                //炎の効果音
			{"data/SE/Warning_000.wav", 0},         //警告表示の効果音
			{"data/SE/Punch.wav", 0},               //パンチの効果音
			{"data/SE/IceExplosion.wav", 0},        //氷の効果音
			{"data/SE/Slash_000.wav", 0},           //斬撃の効果音
			{"data/SE/Claw.wav", 0},                //爪の効果音
			{"data/SE/Vine.wav", 0},                //つるの効果音
			{"data/SE/Heal_000.wav", 0},            //回復の効果音
			{"data/SE/Explosion_000.wav", 0},       //爆発の効果音
			{"data/SE/Dodge.wav", 0},               //回避の効果音
			{"data/SE/Fire_002.wav", 0},            //炎２の効果音
			{"data/SE/PhycoBurst_001.wav", 0},      //サイコバーストの効果音
			{"data/SE/damage_000.wav", 0},          //ダメージの効果音
			{"data/SE/Shot_000.wav", 0},            //発射０の効果音
			{"data/SE/Charge_000.wav",0},           //チャージ０の効果音
			{"data/SE/Shot_001.wav",0},             //発射１の効果音
			{"data/SE/Shot_Rocket.wav",0},          //ロケラン発射の効果音
			{"data/SE/BurstShot.wav",0},            //バースト発射の効果音
			{"data/SE/RainShot_000.wav",0},         //レインショットの効果音
			{"data/SE/Jump.wav",0},                 //ジャンプの効果音
			{"data/SE/Charge_001.wav",-1},          //チャージの効果音１
			{"data/SE/Coin_000.wav",0},             //コイン０
			{"data/SE/GetClown.wav",0},             //王冠取得０
			{"data/SE/GetAction.wav",0},            //アクション取得０
			{"data/SE/GetMagic.wav",0},             //魔法取得０
			{"data/SE/GetShot.wav",0},              //ショット取得０
			{"data/SE/Damage_001.wav",0},           //ダメージ１
			{"data/SE/Decision_001.wav",0},         //決定１
			{"data/SE/Shot_003.wav",0},             //発射３
		};

};

#endif
