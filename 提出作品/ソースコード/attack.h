//===================================================================================================================
//
// ２０２５年５月１８日：コードを見やすくする[attack.h]
// Author:ShinaTaiyo
//
//===================================================================================================================

//*******************************************************************************************************************
// ２重インクルード防止
//*******************************************************************************************************************
#ifndef _ATTACK_H_  
#define _ATTACK_H_

//*******************************************************************************************************************
// インクルード
//*******************************************************************************************************************
#include "main.h"    // メイン
#include "objectX.h" // オブジェクトX

//===========================================
// 攻撃クラス
//===========================================
class CAttack : public CObjectX
{
public:
	// === 列挙型 ===

	// 攻撃タイプ
	enum class ATTACKTYPE
	{
		BULLET = 0, // 弾
		EXPLOSION,  // 爆発
		MAGICSWORD, // 魔法の剣
		MAX         // 最大
	};

	// 判定タイプ
	enum class COLLISIONTYPE
	{
		NONE = 0,     // なし
		SQUARE,       // 正方形 
		RECTANGLE_XZ, // 回転正方形XZ
		OBBTOOBB,     // OBB同士
		MAX
	};

	// 当たるオブジェクトタイプ
	enum class TARGETTYPE
	{
		NONE = 0, // なし
		PLAYER,   // プレイヤー
		ENEMY,    // 敵
		MAX       // 最大
	};

	// === 構造体 ===

	// 無敵状態情報（無敵状態をヒットストップだと勘違いしていました。後々直します。）
	struct HitStop
	{
		const int nSetTime; // 当たった時に与える無敵状態フレーム数
	};

	// 攻撃をバウンドさせる場合の情報（全ての攻撃がバウンドするわけないので、後に管理方法を修正予定）（デフォルトがpublicなだけで特にクラスと変わらないので修正予定）
	struct BoundInfo
	{
		// === メンバ関数 ===

		// バウンドさせるかどうか設定
		void SetActive (
			bool bCopyActive,      // バウンドさせるかどうか　　
			D3DXVECTOR3 CopyPower, // バウンドさせる力 
			bool bCopyGravity,     // 重力を付けるかどうか
			float fCopyGravity)    // 重力
		{
			bActive = bCopyActive; Power = CopyPower; bGravity = bCopyGravity; fGravity = fCopyGravity;
		};
		void BoundProcess(CAttack * pAttack); // バウンド処理
		const D3DXVECTOR3& GetPower() const { return Power; } // バウンド力を取得
		const bool& GetActive() const { return bActive; }     // バウンドするかどうかを取得
	private:
		// === メンバ変数 ===

		bool bActive = false;  // バウンドするかどうか
		bool bGravity = false; // バウンド時に重力をONにするかどうか
		float fGravity = 0.0f; // バウンド時に重力をONにする場合に設定する重力
		D3DXVECTOR3 Power = { 0.0f,0.0f,0.0f }; // バウンド力
	};

	// === 特殊関数 ===
	
	// コンストラクタ
	CAttack(
		int nPower, // 威力
		int nSetHitStopTime, // この攻撃で被弾した相手に与える無敵時間
		int nPri = 0, // 自分で決めるプライオリティ
		bool bUseintPri = false, // プライオリティを自分で決めるかどうか（通常はタイプ別で設定）
		CObject::TYPE type = CObject::TYPE::ATTACK, // タイプ
		CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_X // オブジェクトタイプ
	);
	~CAttack();	// デストラクタ

	// === ライフサイクルメンバ関数 ===
	HRESULT Init() override;  // 初期化処理
	void Uninit() override;   // 終了処理
	void Update() override;   // 更新処理
	void Draw() override;     // 描画処理
	void SetDeath() override; // 死亡フラグを設定

	// === メンバ関数 ===

	// 当たった時にオブジェクトを破棄するかどうか
	void SetCollisionRelease(bool bUse);     // 設定
	const bool& GetCollisionRelease() const; // 取得

	const HitStop& GetHitStop(); // 無敵状態情報取得

	const int& GetPower() const; // 威力取得

	// 判定タイプ
	void SetCollisionType(COLLISIONTYPE Type);    // 設定
	const COLLISIONTYPE GetCollisionType() const; // 取得

	// ターゲットタイプ
	void SetTargetType(TARGETTYPE Type);    // 設定
	const TARGETTYPE GetTargetType() const; // 取得

	// 生きているオブジェクト以外にも当たるかどうか
	void SetHitOtherThanLibing(bool bHit);     // 設定
	const bool& GetHitOtherThanLibing() const; // 取得

	// 当たり判定を攻撃クラスに任せるかどうか
	void SetAutoCollision(bool bAuto); // 設定
	const bool& GetAutoCollision();    // 取得

	// 押し出し判定を使うかどうか
	void SetExtrusionCollision(bool bUse); // 設定
	const bool& GetExtrusionCollision();   // 取得

	const CObject::TYPE& GetCollisionObjType(); // 当たったオブジェクトのタイプを取得

	BoundInfo& GetBoundInfo(); // バウンド情報を取得

	// 攻撃の種類
	void SetAttackType(ATTACKTYPE AttackType); // 設定
	const ATTACKTYPE& GetAttackType() const;   // 取得
protected:
	// === 静的メンバ変数 ===

	static const string ATTACK_FILENAME[static_cast<int>(ATTACKTYPE::MAX)]; //攻撃モデルのファイル名

	const bool& GetCollisionSuccess() const;   //当たり判定が成功したかどうか取得
private:
	// === メンバ変数 ===

	int m_nPower; // 攻撃力

	bool 
		m_bCollisionRelease,   // 衝突時に消すかどうか
        m_bHitOtherThanLiving, // 生きているオブジェクト以外にも当たるかどうか
	    m_bAutoCollision,      // 当たり判定を攻撃クラスに任せるかどうか
	    m_bExtrusionCollision, // 押し出し判定を使うかどうか
		m_bCollisionSuccess;   // 判定が成功したかどうか

	ATTACKTYPE m_Type;       // タイプ
	TARGETTYPE m_TargetType; // 狙うオブジェクトの種類
	COLLISIONTYPE m_CollisionType;    // 判定タイプ
	CObject::TYPE m_CollisionObjType; // 当たったオブジェクトの種類

	HitStop m_HitStop;     // ヒットストップ
	BoundInfo m_BoundInfo; // バウンド情報

	// === メンバ関数 ===

	void Collision(); // 当たり判定を行う処理

	// 判定処理
	void CollisionProcess(
		bool& bCollision,     // 当たっているかどうか
		bool & bNowCollision, // 現在当たっているかどうか
		CObjectX * pObjX      // オブジェクトXのポインタ
	);
	void CollisionBuilding();         // 建物オブジェクトと判定する処理
	void ExtrusionCollisionProcess(); // 押し出し判定を行う処理

};

//===========================================
// プレイヤー攻撃クラス
//===========================================
class CAttackPlayer : public CAttack
{
public:
	// === 特殊関数 ===

	// コンストラクタ
	CAttackPlayer(
		int nPower,         // 威力
		int nSetHitStopTime // 被弾オブジェクトに与える無敵時間
	);
	~CAttackPlayer() override; // デストラクタ

	// === ライフサイクルメンバ関数 ===

	HRESULT Init() override;   // 初期化処理
	void Uninit() override;    // 終了処理
	void Update() override;    // 更新処理
	void Draw() override;      // 描画処理
	void SetDeath() override;  // 死亡フラグを設定

	// === 静的メンバ関数 ===

	// 生成処理
	static CAttackPlayer* Create(
		ATTACKTYPE AttackType, // 攻撃タイプ
		TARGETTYPE TargetType, // 当たるオブジェクトタイプ
		COLLISIONTYPE CollisionType, // 判定タイプ
		bool bHitOtherThanLiving, // 生きているオブジェクト以外に当たるかどうか
		bool bAutoCollision,      // 判定をクラスに任せるかどうか(falseの場合、呼び出したクラスが判定する）
		int nPower, // 威力                  
		int nSetHitStopTime, // 当たったオブジェクトに与える無敵状態フレーム
		int nLife, // 体力
		D3DXVECTOR3 pos,    // 位置
		D3DXVECTOR3 rot,    // 向き
		D3DXVECTOR3 move,   // 移動量
		D3DXVECTOR3 Scale); // 拡大率
private:
	// === メンバ関数 ===
	void BulletCollisionProcess(); // 弾が当たった時の処理
};

//===========================================
// 敵攻撃クラス
//===========================================
class CAttackEnemy : public CAttack
{
public:
	// === 特殊関数 ===

	//コンストラクタ
	CAttackEnemy(
		int nPower,
		int nSetHitStopTime
	);

	~CAttackEnemy() override; // デストラクタ

	// === ライフサイクル関数 ===

	HRESULT Init() override;  // 初期化処理
	void Uninit() override;   // 終了処理
	void Update() override;   // 更新処理
	void Draw() override;     // 描画処理
	void SetDeath() override; // 死亡フラグを設定

	// === 静的メンバ関数 ===

	// 生成処理
	static CAttackEnemy* Create(
		ATTACKTYPE AttackType, // 攻撃タイプ
		TARGETTYPE TargetType, // 当たるオブジェクトタイプ
		COLLISIONTYPE CollisionType, // 判定タイプ
		bool bHitOtherThanLiving, // 生きているオブジェクト以外にも当たるかどうか
		bool bAutoCollision,      // 判定をクラスに任せるかどうか（falseの場合、呼び出し元クラスが判定を管理）
		int nPower, // 威力
		int nSetHitStopTime, // 当たったオブジェクトに与える無敵状態フレーム数 
		int nLife, // 体力
		D3DXVECTOR3 pos,    // 位置
		D3DXVECTOR3 rot,    // 向き
		D3DXVECTOR3 move,   // 移動量
		D3DXVECTOR3 Scale   // 拡大率
	);
};
//==================================================================================================================================================
#endif
