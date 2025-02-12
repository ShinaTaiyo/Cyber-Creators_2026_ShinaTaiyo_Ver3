//====================================================
//
//７月１６日：トラップ実装[trap.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _TRAP_H_  
#define _TRAP_H_

//======================
//インクルード
//======================
#include "main.h"
#include "objectX.h"
//==========================================

//======================
//前方宣言
//======================
class CBlock;

//==========================================

//==========================================
//トラップクラス
//==========================================
class CTrap : public CObjectX
{
public:
	//===========================
	//トラップの種類
	//===========================
	typedef enum
	{
		TRAPTYPE00_NEEDLEBALL = 0,
		TRAPTYPE01_CANNON,
		TRAPTYPE02_THORNCLUB,
		TRAPTYPE_MAX
	}TRAPTYPE;
	//======================================================================

	//===========================
	//当たり判定の種類
	//===========================
	typedef enum
	{
		COLLISIONTYPE00_SQUARE = 0,
		COLLISIONTYPE01_RECTANGLESQUARE,
		COLLISIOINTYPE_MAX
	}COLLISIONTYPE;

	CTrap();                    //コンストラクタ
	~CTrap() override;          //デストラクタ
	HRESULT Init() override;    //初期化処理
	void Uninit() override;     //終了処理
	void Update() override;     //更新処理
	void Draw() override;       //描画処理
	void SetDeath() override;   //死亡フラグを設定
	static CTrap* Create(TRAPTYPE type, D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, D3DXVECTOR3 Scale, int nRotType,int nSubType);//トラップを生成（基本挙動）
	static int GetNumFile() { return m_nNumFile; }           //ファイルの数を取得する
	static const int* GetSubTypeNum() { return &m_nTRAPSUBTYPENUM[0]; }            //サブタイプの０番目を取得する
	int GetSubType() { return m_nSubType; }                                        //サブタイプを取得
	void SetInfo(float fStartRot,float fSpeed, float fRotSpeed,float fMoveLength,float fTurnSpeed,int nInterval,float fAddSpeed); //情報を設定

	float& GetStartRot() { return m_fStartRot; }                                   //回転開始位置を取得
	float& GetSpeed() { return m_fSpeed; }                                         //速さを取得
	float& GetMoveLength() { return m_fMoveLength; }                               //移動距離を取得
	float& GetRotSpeed() { return m_fRotSpeed; }                                   //回転速度を取得
	float& GetTurnSpeed() {return m_fTurnSpeed; }                                  //Uターンするスピードを取得  
	int& GetInterval() { return m_nInterval; }                                     //様々なインターバルを取得
	float& GetAddSpeed() { return m_fAddSpeed; }                                   //加速度
	COLLISIONTYPE GetCollisionType() { return m_CollisionType; }                   //当たり判定の種類を取得する

	TRAPTYPE GetTrapType() { return m_type; }                                      //トラップの種類を取得する
private:													                       
	static const int m_nTRAPSUBTYPENUM[TRAPTYPE_MAX];                              //トラップのサブタイプ
	TRAPTYPE m_type;                                                               //敵の種類
	COLLISIONTYPE m_CollisionType;                                                 //当たり判定の種類
	bool m_bUse;                                                                   //使用状態
	bool m_bUseMove;                                                               //移動量を使用するかどうか
	bool m_bSpeedSwitch;                                                           //素早さを変えるフラグ
	bool m_bSpeedSwitch2;                                                          //上記その２
	bool m_bTurn;                                                                  //ターン方向を変えるフラグ
	int m_nSubType;                                                                //サブタイプ
	int m_nCntTime;                                                                //出現してからのフレーム数を測る
	static const char* m_TRAPFILENAME[TRAPTYPE_MAX];                               //アイテムのファイルの名前
	static int m_nNumFile;                                                         //ファイルの数
															                       
	float m_fStartRot;                                                             //開始位置
	float m_fSpeed;                                                                //速さ
	float m_fMoveLength;                                                           //移動距離
	float m_fRotSpeed;                                                             //回転スピード
	float m_fTurnSpeed;                                                            //ターンするスピード
	float m_fAddSpeed;                                                             //加速度
	int m_nCntSetTime;                                                             //様々な時間を設定
	int m_nInterval;                                                               //様々なインターバル

	void Collision(void);                                                          //当たり判定を行う
	void NeedleBallProcess(void);                                                  //トゲボールの処理
	void CannonProcess(void);                                                      //大砲の処理
	void ThornClubProcess();                                                       //トゲこん棒の処理

	void DispInfo() override;                                                      //情報表示処理
};

#endif
