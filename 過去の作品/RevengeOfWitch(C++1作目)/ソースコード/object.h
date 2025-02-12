//====================================================
//
//７月１６日：オブジェクトのリスト管理[Object.cpp]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _OBJECT_H_  
#define _OBJECT_H_

//======================
//インクルード
//======================
#include "main.h"

//==========================================

//==========================================
//オブジェクトクラス
//==========================================
class CObject
{
public:
	//===========================
	//オブジェクト種類列挙型
	//===========================
	typedef enum
	{
		TYPE_NONE,            //タイプなし
		TYPE_PLAYER,          //プレイヤー3
		TYPE_ENEMY,           //敵2
		TYPE_BULLET,          //弾4
		TYPE_EXPLOSION,       //爆発1
		TYPE_EFFECT,          //エフェクト1
		TYPE_PARTICLE,        //パーティクル1
		TYPE_BLOCK,           //ブロック0
		TYPE_ITEM,            //アイテム2
		TYPE_MODEL,           //モデル
		TYPE_BG,              //背景
		TYPE_FIELD,           //床
		TYPE_STAGEMANAGER,    //ステージマネージャー
		TYPE_GAUGE,           //ゲージ2
		TYPE_NUMBER,          //数字
		TYPE_FADE,            //フェード4
		TYPE_BG3D,            //3D背景
		TYPE_MARKER,          //マーカー
		TYPE_NUMBER3D,        //3D数字
		TYPE_MAGICWINDOW,     //魔法ウインドウ
		TYPE_MAGICDISP,       //魔法表示
		TYPE_ATTACK,          //攻撃
		TYPE_BOSS,            //ボス
		TYPE_EMITTER,         //エミッター
		TYPE_TRAP,            //トラップ
		TYPE_MESHORBIT,       //メッシュ軌跡
		TYPE_TIMER,           //タイマー
		TYPE_SCORE,           //スコア      
		TYPE_AIMODEL,         //AIモデル
		TYPE_DAMAGE,          //ダメージ
		TYPE_UI,              //UI
		TYPE_UI3D,            //UI3D
		TYPE_PARTICLE2D,      //パーティクル2D
		TYPE_MAX
	}TYPE;
	//===========================================

	//===========================
	//オブジェクト分類列挙型
	//===========================
	typedef enum
	{
		OBJECTTYPE_2D = 0,
		OBJECTTYPE_3D,
		OBJECTTYPE_BILLBOARD,
		OBJECTTYPE_X,
	}OBJECTTYPE;

	CObject(int nPriority = 2);//描画優先設定(５月２８日New!：デフォルト引数（呼び出し時に引数設定しなければ３が代入されて処理される
	virtual ~CObject();        //デストラクタ
	virtual HRESULT Init() = 0;//初期化処理
	virtual void Uninit() = 0; //終了処理
	virtual void ExtraUninit();//別枠の終了処理
	virtual void Update() = 0; //更新処理
	virtual void Draw() = 0;   //描画処理
	bool GetUseDeath() { return m_bUseDeath; }//死亡フラグを発動するか決めるフラグを取得
	static void ReleaseAll();  //全オブジェクト開放
	static void UpdateAll();   //全オブジェクト更新
	static void DrawAll();     //全オブジェクト描画
	virtual void SetDeath();                                              //死亡フラグを設定
	//=====================
	//取得系
	//=====================
	static int GetNumAll();                              //全オブジェクトの総数を取得する
	bool GetCreateSuccess() { return m_bCreateSuccess; } //オブジェクトの生成に成功したかどうか
	int GetId() { return m_nID; }                        //現在のIDを取得する
	static CObject* GetCObject(int nIdx,int nPriority);  //オブジェクト取得
	TYPE GetType();                                      //タイプ取得
	//==============================================================================================
	void SetType(TYPE type);                             //タイプ設定
	void SetStageManagerObjNum(int nNum) { m_nStageManagerObjNum = nNum; }            //ステージマネージャーで管理されているオブジェクトの番号を設定
	int GetStageManagerObjNum() { return m_nStageManagerObjNum; }                     //ステージマネージャーで管理されているオブジェクトの番号を取得する
	static const int m_nMAXOBJECT = 1024;                                             //オブジェクト最大数
	static const int m_nMAXPRIORITY = 9;                                              //描画順最大数
	static void ReleaseProcess();                                                     //リストの破棄をする処理
	void Release();                                                                   //自分自身の開放
	void SetUseDeath(bool bDeath) { m_bUseDeath = bDeath; }                           //死亡フラグを発動するかどうかを設定する
	static void SetReleaseAll() { m_bActivationReleaseAll = true; }                   //ReleaseAllを発動させるフラグを設定する
	void SetObjectType(OBJECTTYPE type) { m_ObjectType = type; }                      //オブジェクトの分類を決める
	OBJECTTYPE GetObjectType() { return m_ObjectType; }                               //オブジェクトの分類を取得する
	CObject* GetNextObject() {return m_pNext; }                                       //次のオブジェクトを取得する
	CObject* GetPrevObject() { return m_pPrev; }                                      //前のオブジェクトを取得する

	//==============================
	//ステージマネージャー関係
	//==============================
	CObject* GetNextStageManagerObject() { return m_pStageManagerObj_Next; }          //ステージマネージャーで管理しているオブジェクトの次のオブジェクトを取得する
	CObject* GetPrevStageManagerObject() { return m_pStageManagerObj_Prev; }          //ステージマネージャーで管理しているオブジェクトの前のオブジェクトを取得する
	static CObject* GetTopStageManagerObject() { return m_pStageManagerObj_Top; }     //ステージマネージャーで管理しているオブジェクトのトップオブジェクトを取得する
	static CObject* GetCurStageManagerObject() { return m_pStageManagerObj_Cur; }     //ステージマネージャーで管理しているオブジェクトのカレントオブジェクトを取得する
	void SetStageManagerObj();                                                        //ステージマネージャーで管理するオブジェクトを設定

	static int GetTotalStageManagerObjNum() { return m_nNumStageManagerObject; }      //ステージマネージャーで管理しているオブジェクトの総数を取得
	static void StageManagerObjectReleaseAll();                                            //全てのマネージャー管理のオブジェクトを消す    
	//==============================================================================================

	//==============================
	//リスト関係
	//==============================
	static CObject* GetTopObject(int nPriority){ return m_pTop[nPriority];}           //トップオブジェクトを取得する
	static CObject* GetCurrentObject(int nPriority) { return m_pCur[nPriority];}      //カレントオブジェクトを取得する
	void SetStageManagerChoose(bool bChoose) { m_bStageManagerChooseState = bChoose; }//ステージマネージャーが選択しているかどうか
	bool GetStageManagerChoose() { return m_bStageManagerChooseState; }               //ステージマネージャーが選択しているかどうかを取得する
	//==============================================================================================

	//=================================================
	//フレーム関係
	//=================================================
	int GetCntFrame() { return m_nCntFrame; }                                         //出現してからのフレーム数をカウントする
	//================================================================================================

	//=================================================
	//サウンド関係
	//=================================================
	void SetUseInitialSound(bool bUse) { m_bUseInitialSound = bUse; }//初期化時にサウンドを再生するかどうか
	bool GetUseInitialSound() { return m_bUseInitialSound; }         //初期化時にサウンドをさいせいするかどうかを取得
	//================================================================================================
protected:
private:
	void ReleaseStageManagerObj();                             //ステージマネージャーで管理しているオブジェクトを破棄
	bool m_bDeath;                                           //死亡フラグ
	bool m_bUseDeath;                                        //死亡フラグを発動するかどうか

	static CObject* m_apObject[m_nMAXPRIORITY][m_nMAXOBJECT];//オブジェクト管理
	static bool m_bActivationReleaseAll;                     //ReleaseAllを発動するかどうか

	//===============================================
	//リスト管理
	//===============================================
	static CObject* m_pTop[m_nMAXPRIORITY];                  //先頭のオブジェクトのポインタ
	static CObject* m_pCur[m_nMAXPRIORITY];                  //現在（最後尾）のポインタ
	CObject* m_pNext;                                        //次のオブジェクトへのポインタ
	CObject* m_pPrev;                                        //前のオブジェクトへのポインタ
	//===============================================================================================

	//===============================================
	//ステージマネージャー関係
	//===============================================
	static CObject* m_pStageManagerObj_Top;                  //ステージマネージャーで管理している先頭オブジェクトへのポインタ
	static CObject* m_pStageManagerObj_Cur;                  //ステージマネージャーで管理してい最後尾オブジェクトへのポインタ
	CObject* m_pStageManagerObj_Next;                        //ステージマネージャーで管理している次のオブジェクトへのポインタ
	CObject* m_pStageManagerObj_Prev;                        //ステージマネージャーで管理している前のオブジェクトへのポインタ
	bool m_bIsStageManagerObj;                               //ステージマネージャーで管理されているオブジェクトかどうか
	int m_nStageManagerObjNum;                               //ステージマネージャーで管理されているオブジェクトの番号
	bool m_bStageManagerChooseState;                         //ステージマネージャーが選択しているかどうか

	static int m_nNumStageManagerObject;                     //ステージマネージャーで管理されているオブジェクトの数
	//================================================================================================

	TYPE m_type;                                             //オブジェクト種類
	OBJECTTYPE m_ObjectType;                                 //オブジェクト分類
	static int m_nNumAll;                                    //オブジェクト総数
	int m_nID;                                               //自分自身のID
	int m_nPriority;                                         //描画順を送る
	bool m_bCreateSuccess;                                   //オブジェクトの生成が成功したかどうか

	//=================================================
	//フレーム関係
	//=================================================
	int m_nCntFrame;                                         //出現してからのフレーム数をカウントする
	//================================================================================================

	//=================================================
	//サウンド関係
	//=================================================
	bool m_bUseInitialSound;//初期化時にサウンドを再生するかどうか
	//================================================================================================
};

#endif
