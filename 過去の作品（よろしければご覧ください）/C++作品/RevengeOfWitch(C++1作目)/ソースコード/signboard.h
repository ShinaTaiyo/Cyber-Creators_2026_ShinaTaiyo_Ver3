//====================================================
//
//９月３日：立て看板実装[signboard.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _SIGNBOARD_H_  
#define _SIGNBOARD_H_

//======================
//インクルード
//======================
#include "main.h"
#include "objectX.h"
#include "enemy.h"
//==========================================

//==========================================

//==========================================
//サインボードクラス
//==========================================
class CSignBoard : public CObjectX
{
public:
	//===========================
	//説明の種類
	//===========================
	typedef enum
	{
		SIGNBOARDMANUALTYPE00_MOVE = 0,     //立て看板説明書：移動
		SIGNBOARDMANUALTYPE01_JUNP,         //立て看板説明書：ジャンプ
		SIGNBOARDMANUALTYPE02_DASH,         //立て看板説明書：ダッシュ
		SIGNBOARDMANUALTYPE03_WALLJUMP,     //立て看板説明書：壁キック
		SIGNBOARDMANUALTYPE04_SHOT,         //立て看板説明書：ショット
		SIGNBOARDMANUALTYPE05_CHARGESHOT,   //立て看板説明書：チャージショット
		SIGNBOARDMANUALTYPE06_RAINSHOT,     //立て看板説明書：レインショット
		SIGNBOARDMANUALTYPE07_BURSTSHOT,    //立て看板説明書：バーストショット
		SIGNBOARDMANUALTYPE08_RAPIDSHOT,    //立て看板説明書：ラピッドショット
		SIGNBOARDMANUALTYPE09_MAGIC,        //立て看板説明書：魔法
		SIGNBOARDMANUALTYPE10_MAGICCHENGE,  //立て看板説明書：魔法チェンジ
		SIGNBOARDMANUALTYPE11_DOUBLEJUMP,   //立て看板説明書：２段ジャンプ
		SIGNBOARDMANUALTYPE_MAX
	}SIGNBOARDMANUALTYPE;
	//======================================================================

	//===========================
	//説明の種類
	//===========================
	typedef enum
	{
		SIGNBOARDTYPE00_NORMAL = 0,     //普通の立て看板
		SIGNBOARDTYPE_MAX
	}SIGNBOARDTYPE;
	//======================================================================

	//==========================
	//立て看板の種類
	//==========================
	CSignBoard();                   //コンストラクタ
	~CSignBoard() override;         //デストラクタ
	HRESULT Init() override;    //初期化処理
	void Uninit() override;     //終了処理
	void Update() override;     //更新処理
	void Draw() override;       //描画処理
	void SetDeath() override;   //死亡フラグを設定
	static int* GetSubTypeNum() { return &m_SIGNBOARDSUBTYPENUM[0]; }//それぞれのオブジェクトのサブタイプを取得する
	static int GetNumFile() { return m_nNumFile; }//ファイルの数を取得する
	static CSignBoard* Create(SIGNBOARDTYPE Type, SIGNBOARDMANUALTYPE ManualType, D3DXVECTOR3 pos, D3DXVECTOR3 Scale, D3DXVECTOR3 Rot);//敵を生成
	void SetManualType(SIGNBOARDMANUALTYPE type) { m_ManualType = type; }//説明の種類を設定する
	int GetSubType() { return m_nSubType; }
private:
	SIGNBOARDMANUALTYPE m_ManualType;     //説明の種類
	SIGNBOARDTYPE m_SignBoardType;        //立て看板の種類
	static const int m_nNumFile = 1;                //モデルファイルの総数
	static const char* m_apSIGNBOARD_TEXTURENAME[SIGNBOARDMANUALTYPE_MAX];//立て看板の説明のテクスチャの総数
	static const char* m_apSIGNBOARD_TEXTURENAME_SUCCESS[SIGNBOARDMANUALTYPE_MAX];//立て看板の説明が実行された後のテクスチャ
	static const char* m_apSIGNBOARD_FILENAME[SIGNBOARDTYPE_MAX];//立て看板のファイル名
	static int m_SIGNBOARDSUBTYPENUM[SIGNBOARDTYPE_MAX];//立て看板のサブタイプ

	int m_nSubType;
	bool m_bSuccessJudge;//求めた操作が成功したかどうか

	void ManualShotProcess();//移動説明看板の処理
	void ManualJumpProcess();//ジャンプ説明看板の処理
	void ManualDashProcess();//ダッシュ説明看板の処理
	void SummonTestEnemy();  //試す用の敵を召喚する処理
};

#endif