//====================================================================================
//
//７月２日：ボス１を実装する[boss.h]
//Author:ShinaTaiyo
//
//====================================================================================

#ifndef _BOSS_H_  
#define _BOSS_H_

//======================
//インクルード
//======================
#include "main.h"
#include "objectX.h"
#include "gauge.h"
#include "attack.h"
//==========================================

//==========================================
//前方宣言
//==========================================
class CObject;
class CBullet;
class CAIModel;

//==========================================
//ボスクラス
//==========================================
class CBoss : public CObjectX
{
public:
	//===========================
	//ボスの種類
	//===========================
	typedef enum
	{
		BOSSTYPE00_KINGSLIME = 0,
		BOSSTYPE01_DEATHENAGA,
		BOSSTYPE02_MRSTRONG,
		BOSSTYPE03_SNOWDEVIL_RILEY,
		BOSSTYPE04_DRAIYA,
		BOSSTYPE05_GABRIEL,
		BOSSTYPE_MAX
	}BOSSTYPE;
	//======================================================================
	CBoss();                                                                                                //コンストラクタ
	~CBoss() override;                                                                                      //デストラクタ
	HRESULT Init() override;                                                                                //初期化処理
	void Uninit() override;                                                                                 //終了処理
	void Update() override;                                                                                 //更新処理
	void Draw() override;                                                                                   //描画処理
	void SetDeath() override;                                                                               //死亡フラグを設定
	void SetDamage(int nDamage, int nHitStopTime) override;                                                 //ダメージを与える
	static int GetNumFile() { return m_nNumFile; }                                                          //ファイルの数を取得する
	static CBoss* Create(BOSSTYPE type, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 Scale);                      //ボスを生成
	static const int m_nMAX_DEFEATSTAGINGTIME = 180;                                                        //倒す演出の時間の最大数
	static CObject* SetCreateBoss(BOSSTYPE type, int nLife, D3DXVECTOR3 Pos, D3DXVECTOR3 Scale);                 //ボスを設定する
protected:
	static const char* m_BOSS_FILENAME[BOSSTYPE_MAX];        //ボスのファイルネーム
	void SetType(BOSSTYPE type) { m_type = type; }           //ボスの種類を設定する

	void SetBossHpGauge(int nLife);                          //ボスの体力ゲージを設定する
	int& GetCntTime() { return m_nCntTime; }                 //時間を取得する
	int& GetPatternTime() { return m_nPatternTime; }         //行動パターンの時間を取得
	int& GetSkill() { return m_nSkill; }                     //スキルを取得
	int& GetPattern() { return m_nPattern; }                  //現在のパターンを取得
	int& GetActionNum() { return m_nActionNum; }
         //行動回数を取得
	bool& GetAction() { return m_bAction; }                  //行動しているかどうかを取得
	bool& GetUseMovePattern() { return m_bUseMovePattern; }  //移動パターンを使用しているかどうかを取得
	int& GetMovePatternFrame() { return m_nMovePatternFrame; }//移動パターンフレームを取得
	void ResetPattern();                                     //ボスの行動パターンをリセットする
	virtual void MovePattern();                              //ボスの移動処理
	virtual void ActionPattern();                            //ボスの行動パターン
	D3DXVECTOR3& GetPurposePos() { return m_PurposePos; }    //目的の位置取得
	void SetPurposePos(D3DXVECTOR3 Pos) { m_PurposePos = Pos; }//目的の位置を設定
	void SetInsanity(bool bInsanity) { m_bInsanity = bInsanity; }//発狂を設定する
	bool& GetInsanity() { return m_bInsanity; }              //発狂しているかどうかを取得
	void SetSpecialAttack(bool bUse) { m_bSpecialAttack = bUse; }//必殺技を使うかどうかを設定
	bool& GetSpecialAttack() { return m_bSpecialAttack; }    //必殺技を使うかどうかを取得
	int& GetTurnCnt() { return m_nTurnCnt; }                 //ターン数をカウント
	void SetMovePatternNum(int nNum) { m_nMovePatternNum = nNum; }//移動パターン番号を設定
	int& GetMovePatternNum() { return m_nMovePatternNum; }   //移動パターン番号を取得する
	int& GetMaxMovePatternFrame() { return m_nMaxMovePatternFrame; }//移動パターンの最大フレーム数を取得する
	int& GetSubPattern() { return m_nSubPattern; }           //サブパターンを取得
	virtual void DecideMovePattern();                        //移動パターンを決める処理
	void SetSaveRot(float fRot) { m_fSaveRot = fRot; }       //保存する向きを設定する
	float& GetSaveRot() { return m_fSaveRot; }               //保存した向きを取得する
private:
	CGauge* m_pGauge;                                        //ゲージ
	void DefeatStaging();                                    //討伐演出
	void DispInfo() override;                                //情報表示処理
	void BossRushProcess();                                  //ボスラッシュの処理
	virtual void AdjustSize();                               //ボスのサイズを調整する
	//==============================
	//当たり判定
	//==============================
	void Collision();
	//====================================================================

	BOSSTYPE m_type;                                         //ボスの種類
	D3DXVECTOR3 m_Aim;                                       //ベクトル
	float m_fTotalAim;                                       //総合したベクトル
	bool m_bUse;                                             //使用状態
	int m_nCntTime;                                          //出現してからのフレーム数を測る
	int m_nDefeatStagingTime;                                //倒す演出の時間

	//========================================
	//移動パターン関係
	//========================================
	bool m_bUseMovePattern;                               //移動パターンを発動するかどうか
	int m_nMaxMovePatternFrame;                           //移動パターンの最大フレーム数
	int m_nMovePatternFrame;                              //移動状態のフレーム数をカウントする
	int m_nMovePatternNum;                                   //移動パターンの番号
	//==========================================================================================================================

	//========================================
	//行動パターン関係
	//========================================
	int m_nPatternTime;                                      //行動パターンの時間
	int m_nSkill;                                            //スキル
	int m_nPattern;                                          //行動パターン
	int m_nSubPattern;                                       //サブパターン（行動パターンの中で更に分割）
	int m_nActionNum;                                        //行動回数
	bool m_bAction;                                          //行動しているかどうか
	float m_fSaveRot;                                        //向きを保存しておく
	D3DXVECTOR3 m_PurposePos;                                //目的の位置保存用
	//==========================================================================================================================

	int m_nTurnCnt;                                          //ターン数をカウント
	bool m_bInsanity;                                        //発狂しているかどうか
	bool m_bSpecialAttack;                                   //必殺技を使うかどうか
	static int m_nNumFile;                                   //読み込んだファイルの数
};
//====================================================================================================

//==========================================
//キングスライムクラス
//==========================================
class CKingSlime : public CBoss
{
public:
	CKingSlime(); //コンストラクタ
	~CKingSlime() override;//デストラクタ
	HRESULT Init() override; //初期化処理
	void Uninit() override;  //終了処理
	void Update() override;  //更新処理
	void Draw() override;    //描画処理
	void SetDeath() override;//死亡フラグを設定
	static CKingSlime* Create(BOSSTYPE type, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 Scale);//ボスを生成
private:
	static const int m_nMAX_MOVEPATTERNFRAME = 100;          //移動パターンの最大フレーム

	void MovePattern() override;                             //ボスの移動パターン
	void ActionPattern() override;                           //ボスの行動パターン
	void Action0();                                          //行動０
	void Action1();                                          //行動１
	void Action2();                                          //行動２
	void Action3();                                          //行動３
	void Action4();                                          //行動４
};
//====================================================================================================

//==========================================
//デスエナガクラス
//==========================================
class CDeathEnaga : public CBoss
{
public:
	CDeathEnaga(); //コンストラクタ
	~CDeathEnaga() override;//デストラクタ
	HRESULT Init() override; //初期化処理
	void Uninit() override;  //終了処理
	void Update() override;  //更新処理
	void Draw() override;    //描画処理
	void SetDeath() override;//死亡フラグを設定
	static CDeathEnaga* Create(BOSSTYPE type, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 Scale);//ボスを生成
private:
	static const int m_nMAX_TWINTORNADO = 8;                 //ツイントルネード最大数
	float m_fMoveLength;                                     //移動する距離
	static const int m_nMAX_MOVEPOINT = 6;                   //移動位置の最大数
	static const D3DXVECTOR3 m_MOVEPOINT[m_nMAX_MOVEPOINT];  //移動位置

	int m_nPurposeMovePoint;                                 //移動先の番号
	void MovePattern() override;                             //ボスの移動パターン
	void ActionPattern() override;                           //ボスの行動パターン

	void Action0();                                          //行動０
	void Action1();                                          //行動１

	CAttackEnemy* m_apTwinTornado[m_nMAX_TWINTORNADO];       //ツイントルネード個数
	void Action2();                                          //行動２

	void Action3();                                          //行動３

	void Action4();                                          //行動４
};
//====================================================================================================

//==========================================
//ミスターストロングクラス
//==========================================
class CMrStrong : public CBoss
{
public:
	CMrStrong(); //コンストラクタ
	~CMrStrong() override;//デストラクタ
	HRESULT Init() override; //初期化処理
	void Uninit() override;  //終了処理
	void Update() override;  //更新処理
	void Draw() override;    //描画処理
	void SetDeath() override;//死亡フラグを設定
	static CMrStrong* Create(BOSSTYPE type, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 Scale);//ボスを生成
private:
	void MovePattern() override;                             //ボスの移動パターン
	void DecideMovePattern() override;                       //移動パターンを決める
	void AdjustSize() override;                              //ボスのサイズを調整する
	void MovePattern0();                                     //移動０
	void MovePattern1();                                     //移動１

	void ActionPattern() override;                           //ボスの行動パターン
	void Action0();                                          //行動０
	void Action1();                                          //行動１
	void Action2();                                          //行動２

	CAttackEnemy* m_pBurstMeteo;                             //バーストメテオへのポインタ
	void Action3();                                          //行動３

	void Action4();                                          //行動４

	void Action5();                                          //行動５（ヘルフォール）
};
//====================================================================================================

//==========================================
//ライリークラス
//==========================================
class CRiley : public CBoss
{
public:
	CRiley(); //コンストラクタ
	~CRiley() override;//デストラクタ
	HRESULT Init() override; //初期化処理
	void Uninit() override;  //終了処理
	void Update() override;  //更新処理
	void Draw() override;    //描画処理
	void SetDeath() override;//死亡フラグを設定
	static CRiley* Create(BOSSTYPE type, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 Scale);//ボスを生成
private:
	static const int m_nMAX_MOVEPATTERN_0_FRAME = 100;           //移動パターン０のフレーム数
	static const int m_nMAX_MOVEPATTERN_1_FRAME = 100;           //移動パターン１のフレーム数
	static const int m_nMAX_REFLECTIONSNOWBURST = 3;             //反射雪攻撃の総数
	static constexpr float m_fMOVEPATTERNLENGTH = 200.0f;    //移動パターンを分岐させる基準の距離 
	static const int m_nMAX_ACTION03_ATTACKNUM = 5;          //アクション３の雪玉の召喚数
	static const int m_nMAX_ACTION2BULLETNUM = 8;                //アクション２の破裂時発射するバレット数
	static const int m_nMAX_ICE_EIGE = 12;                   //アイスエイジ召喚数
	void MovePattern() override;                             //ボスの移動パターン
	void MovePattern0();                                     //移動０
	void MovePattern1();                                     //移動１
	void DecideMovePattern() override;                       //移動パターンを決める
	void AdjustSize() override;                              //ボスのサイズを調整する

	void ActionPattern() override;                           //ボスの行動パターン
	void Action0();                                          //行動０
	void Action1();                                          //行動１
	void Action2();                                          //行動２
	void Action3();                                          //行動３
	void Action4();                                          //行動４（アイスエイジ）
	void Action5();                                          //行動５（無想転生）

	bool m_bSwitchScale;                                     //拡大率を変えるフラグ
	CRileyAttack* m_pSnowRamming;                            //雪体当たり
	CRileyAttack* m_pReflectionSnowBurst[m_nMAX_REFLECTIONSNOWBURST];                  //反射雪バースト
};
//====================================================================================================

//==========================================
//ドライヤクラス
//==========================================
class CDraiya : public CBoss
{
public:
	CDraiya(); //コンストラクタ
	~CDraiya() override;//デストラクタ
	HRESULT Init() override; //初期化処理
	void Uninit() override;  //終了処理
	void Update() override;  //更新処理
	void Draw() override;    //描画処理
	void SetDeath() override;//死亡フラグを設定
	static CDraiya* Create(BOSSTYPE type, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 Scale);//ボスを生成
private:
	static const int m_nNUMSPECIALATTACK = 10;

	static const int m_nMAX_MOVEPATTERN_0_FRAME = 150;           //移動パターン０のフレーム数
	static constexpr float m_fACTION1_PARABORAJUMPSPEED = 25.0f;//行動１の放物線ジャンプのスピード
	static const int m_nMAX_ACTION2THUNDERNUM = 5;           //行動２のサンダーを発射する回数
	static const int m_nMAX_ACTION2THUNDER_SPAN = 60;        //行動２のサンダーを発射する間隔
	static constexpr float m_fACTION0_ATTACKSPEED = 15.0f;   //行動０の攻撃の速度
	static const int m_nMAX_ACTION03_RAMMING_NUM = 7;        //行動３の体当たり回数
	static constexpr float m_fACTION4_SUMMONTHUNDERLENGTH = 900.0f;//行動４の雷を召喚する距離
	static constexpr float m_fACTION4_THUNDERSPEED = 25.0f;  //行動４の雷の速さ
	static const int m_nACTION4_THUNDERSUMMONSPAN = 60;      //行動４の雷を召喚する速さ
	static constexpr float m_fACTION4_THUNDERSCALE_Y = 5.0f; //行動４の雷のY方向の拡大率
	static const int m_nACTION4_THUNDERSHOTNUM = 2;          //行動４の雷の召喚回数
	static constexpr float m_fACTION4_BACKMOCELENGTH = 400.0f;//行動４の奥へ移動する距離
	void MovePattern() override;                             //ボスの移動パターン
	void MovePattern0();                                     //移動０
	void DecideMovePattern() override;                       //移動パターンを決める
	void AdjustSize() override;                              //ボスのサイズを調整する

	void ActionPattern() override;                           //ボスの行動パターン
	void Action0();                                          //行動０
	void Action1();                                          //行動１
	void Action2();                                          //行動２
	void Action3();                                          //行動３
	void Action4();                                          //行動３
	CDraiyaAttack* m_pAnnihilationThunder;                   //殲滅雷
	bool m_bAnnihilationThunderShot[m_nMAX_ACTION2THUNDERNUM];//殲滅雷の発射方向のフラグ

	D3DXVECTOR3 m_RammingSavePos[m_nMAX_ACTION03_RAMMING_NUM];//体当たりの開始位置
	float m_fSaveRammingRot[m_nMAX_ACTION03_RAMMING_NUM];       //体当たりの飛ぶ方向を保存する

	CDraiyaAttack* m_pSpecialThunder[m_nNUMSPECIALATTACK];
	int m_nNumThunder;                                        //雷の数を格納する
};
//====================================================================================================

//==========================================
//ガブリエルクラス
//==========================================
class CGabriel : public CBoss
{
public:
	CGabriel();              //コンストラクタ
	~CGabriel() override;    //デストラクタ
	HRESULT Init() override; //初期化処理
	void Uninit() override;  //終了処理
	void Update() override;  //更新処理
	void Draw() override;    //描画処理
	void SetDeath() override;//死亡フラグを設定
	static CGabriel* Create(BOSSTYPE type, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 Scale);//ボスを生成
private:
	static const int m_nMAX_MOVEPATTERN0_FRAME = 120;        //ボスの移動パターン０のフレーム数
	static const int m_nMAX_ACTION0BULLETNUM = 4;            //行動０の弾の数
	void MovePattern() override;                             //ボスの移動パターン
	void MovePattern0();                                     //移動０
	void DecideMovePattern() override;                       //移動パターンを決める
	void AdjustSize() override;                              //ボスのサイズを調整する

	void ActionPattern() override;                           //ボスの行動パターン
	void Action0();                                          //行動０
	void Action1();                                          //行動１

	float m_fMoveLength;                                     //移動するときの距離
	float m_fMoveRot;                                        //移動するときの向き
	D3DXVECTOR3 m_SaveMoveFulcrumPos;                        //移動するときの支点となる位置
	bool m_bMoveAim;                                         //移動方向を決める

	CBullet* m_pCircleBullet[m_nMAX_ACTION0BULLETNUM];       //弾へのポインタ]
	CAIModel* m_pSubAiModel;                                 //AI用１

};
//====================================================================================================

#endif
