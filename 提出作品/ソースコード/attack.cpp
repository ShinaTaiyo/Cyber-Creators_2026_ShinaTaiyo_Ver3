//======================================================================================================================
//
// ２０２５年４月１９日：コードを綺麗にする[attack.cpp]
//Author:ShinaTaiyo
//
//======================================================================================================================

//**********************************************************************************************************************
// インクルード
//**********************************************************************************************************************

#include "attack.h"      // 元ヘッダ
#include "damage.h"      // ダメージ
#include "manager.h"     // マネージャー
#include "enemy.h"       // 敵
#include "player.h"      // プレイヤー
#include "gauge.h"       // ゲージ
#include "objectXInfo.h" // オブジェクトX情報
#include "block.h"       // ブロック
#include "bgModel.h"     // 背景モデル
#include "particle.h"    // パーティクル
#include "game.h"        // ゲーム
#include "collision.h"   // コリジョン
#include "ui.h"          // UI
//**********************************************************************************************************************
// 静的メンバ宣言
//**********************************************************************************************************************

// 攻撃モデルファイル名
const string CAttack::ATTACK_FILENAME[static_cast<int>(CAttack::ATTACKTYPE::MAX)] =
{
	"data\\MODEL\\Attack\\Fire_000.x",
	"data\\MODEL\\Attack\\Explosion_000.x",
	"data\\MODEL\\Attack\\MagicSword_000.x",
};

//**********************************************************************************************************************
// 攻撃クラス
//**********************************************************************************************************************

//=====================================================================================================================
// 
// publicメンバ 
//
//=====================================================================================================================

//=====================================================================================================================
// コンストラクタ
//=====================================================================================================================
CAttack::CAttack(int nPower, int nSetHitStopTime, int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) :
	CObjectX(nPri, bUseintPri, type, ObjType),
	m_Type{ ATTACKTYPE::BULLET },
	m_nPower{nPower}, 
	m_HitStop{ {nSetHitStopTime} },
	m_bCollisionRelease{true},
	m_CollisionType{CAttack::COLLISIONTYPE::NONE},
	m_TargetType{CAttack::TARGETTYPE::NONE},
	m_bHitOtherThanLiving{false},
	m_bAutoCollision{true},
	m_bCollisionSuccess{false},
	m_bExtrusionCollision{false},
    m_BoundInfo{},
	m_CollisionObjType{CObject::TYPE::NONE}
{

}

//=====================================================================================================================
// デストラクタ
//=====================================================================================================================
CAttack::~CAttack()
{

}

//=====================================================================================================================
// 初期化処理
//=====================================================================================================================
HRESULT CAttack::Init()
{
	CObjectX::Init(); // オブジェクトX初期化
	return S_OK;
}

//=====================================================================================================================
// 終了処理
//=====================================================================================================================
void CAttack::Uninit()
{
	CObjectX::Uninit(); // オブジェクトX終了処理
}

//=====================================================================================================================
// 更新処理
//=====================================================================================================================
void CAttack::Update()
{
	// === 変数初期化 ===
	m_bCollisionSuccess = false; // 当たり判定が成功したかどうかをマイフレームリセット

	// === 処理 ===

	CObjectX::Update(); // オブジェクトX更新処理

	// 生きているものとの当たり判定を行う（プレイヤー、敵、ボスなど）
	if (m_bAutoCollision == true)
	{
		Collision();
	}

	// 生きているもの以外（背景やブロック）とも当たり判定を行う（オフの場合、生きていないモデルをすり抜ける）
	if (m_bHitOtherThanLiving == true)
	{
		CollisionBuilding();
	}
	// 生きているものとだけ当たり判定を行う（当たったら消える）
	else
	{
		// 押し出し判定を行う（建物に当たっても消えるわけじゃない）
		if (m_bExtrusionCollision == true)
		{
			ExtrusionCollisionProcess();

			// バウンドするならバウンドさせる
			if (m_BoundInfo.GetActive() == true)
			{
				m_BoundInfo.BoundProcess(this);
			}
		}
	}
}

//=====================================================================================================================
// 描画処理
//=====================================================================================================================
void CAttack::Draw()
{
	CObjectX::Draw(); // オブジェクトX描画処理
}

//=====================================================================================================================
// 死亡フラグ設定処理
//=====================================================================================================================
void CAttack::SetDeath()
{
	CObjectX::SetDeath(); // オブジェクトX死亡フラグ処理
}

//=====================================================================================================================
// 判定時に破棄するかどうか設定
//=====================================================================================================================
void CAttack::SetCollisionRelease(bool bUse)
{
	m_bCollisionRelease = bUse;
}

//=====================================================================================================================
// 判定時に破棄するかどうか取得
//=====================================================================================================================
const bool& CAttack::GetCollisionRelease() const
{
	return m_bCollisionRelease;
}

//=====================================================================================================================
// 無敵状態情報を取得
//=====================================================================================================================
const CAttack::HitStop& CAttack::GetHitStop()
{
	return m_HitStop;
}

//=====================================================================================================================
// 威力取得
//=====================================================================================================================
const int& CAttack::GetPower() const
{
	return m_nPower;
}

//=====================================================================================================================
// 判定タイプ設定
//=====================================================================================================================
void CAttack::SetCollisionType(COLLISIONTYPE Type)
{
	m_CollisionType = Type;
}

//=====================================================================================================================
// 判定タイプ取得
//=====================================================================================================================
const CAttack::COLLISIONTYPE CAttack::GetCollisionType() const
{
	return m_CollisionType;
}

//=====================================================================================================================
// 当たるオブジェクトタイプ設定
//=====================================================================================================================
void CAttack::SetTargetType(TARGETTYPE Type)
{
	m_TargetType = Type;
}

//=====================================================================================================================
// 当たるオブジェクトタイプ取得
//=====================================================================================================================
const CAttack::TARGETTYPE CAttack::GetTargetType() const
{
	return m_TargetType;
}

//=====================================================================================================================
// 生きているオブジェクト以外にも当たるかどうか設定
//=====================================================================================================================
void CAttack::SetHitOtherThanLibing(bool bHit)
{
	m_bHitOtherThanLiving = bHit;
}

//=====================================================================================================================
// 生きているオブジェクト以外にも当たるかどうか取得
//=====================================================================================================================
const bool& CAttack::GetHitOtherThanLibing() const
{
	return m_bHitOtherThanLiving;
}

//=====================================================================================================================
// 当たり判定を攻撃クラスに任せて”自動的に”判定するかどうか設定
//=====================================================================================================================
void CAttack::SetAutoCollision(bool bAuto)
{
	m_bAutoCollision = bAuto;
}

//=====================================================================================================================
// 当たり判定を攻撃クラスに任せて”自動的に”判定するかどうか取得
//=====================================================================================================================
const bool& CAttack::GetAutoCollision()
{
	return m_bAutoCollision;
}

//=====================================================================================================================
// 押し出し判定を行うかどうか設定
//=====================================================================================================================
void CAttack::SetExtrusionCollision(bool bUse)
{
	m_bExtrusionCollision = bUse;
}

//=====================================================================================================================
// 押し出し判定を行うかどうか取得
//=====================================================================================================================
const bool& CAttack::GetExtrusionCollision()
{
	return m_bExtrusionCollision;
}

//=====================================================================================================================
// 当たったオブジェクトのタイプ取得
//=====================================================================================================================
const CObject::TYPE& CAttack::GetCollisionObjType()
{
	return m_CollisionObjType;
}

//=====================================================================================================================
// バウンド情報取得
//=====================================================================================================================
CAttack::BoundInfo& CAttack::GetBoundInfo()
{
	return m_BoundInfo;
}

//=====================================================================================================================
// 
// protectedメンバ 
//
//=====================================================================================================================


//=====================================================================================================================
// 攻撃タイプ設定
//=====================================================================================================================
void CAttack::SetAttackType(ATTACKTYPE AttackType)
{
	m_Type = AttackType;
}

//=====================================================================================================================
// 攻撃タイプ取得
//=====================================================================================================================
const CAttack::ATTACKTYPE& CAttack::GetAttackType() const
{
	return m_Type;
}

//=====================================================================================================================
// 判定が成功したかどうか
//=====================================================================================================================
const bool& CAttack::GetCollisionSuccess() const
{
	return m_bCollisionSuccess;
}

//====================================================================================================================
// 
// privateメンバ 
//
//====================================================================================================================

//=====================================================================================================================
//当たり判定処理
//=====================================================================================================================
void CAttack::Collision()
{
	// === 攻撃を他のオブジェクトに判定させるためリストを検索 ===

	for (int nCntPri = 0; nCntPri < m_nMAXPRIORITY; nCntPri++)
	{
		CObject* pObj = GetTopObject(nCntPri); // リストの先頭オブジェクトを取得
		while (pObj != nullptr)
		{
			// === 変数 ===

		    bool bNowCollision = false; // 当たったかどうかを初期化
			CObject* pNext = pObj->GetNextObject(); // 次のオブジェクトを取得

			// === 処理 ===

			// オブジェクトが敵で、攻撃が狙うオブジェクトが「敵」だった場合
			if (pObj->GetType() == CObject::TYPE::ENEMY && m_TargetType == TARGETTYPE::ENEMY)
			{
				// === 変数 ===

				CEnemy* pEnemy = static_cast<CEnemy*>(pObj);//オブジェクトを敵にキャスト

				// === 処理 ===

				CollisionProcess(m_bCollisionSuccess, bNowCollision, pEnemy); // 敵との当たり判定を行う

				// 当たり判定が上記の処理で成功していたら
				if (m_bCollisionSuccess == true)
				{
					m_CollisionObjType = CObject::TYPE::ENEMY; // 当たったオブジェクトのタイプを敵とする

					// 敵の体力がなくなっていたら攻撃が倒したオブジェクトのタイプを「敵」に設定
					if (pEnemy->GetLifeInfo().GetLife() < 1)
					{
						pEnemy->SetDefeatAttack(m_Type); // 倒したオブジェクトのタイプを「敵」とする
					}
				}
			}
			// オブジェクトが「プレイヤー」で攻撃が狙うオブジェクトが「プレイヤー」だった場合
			else if (pObj->GetType() == CObject::TYPE::PLAYER && m_TargetType == TARGETTYPE::PLAYER)
			{
				// === 変数 ===

				CObjectX* pObjX = static_cast<CObjectX*>(pObj);//オブジェクトをオブジェクトXにキャスト

				// === 処理 ===

				CollisionProcess(m_bCollisionSuccess, bNowCollision, pObjX); // 当たり判定の処理を行う

				// 上記の処理で当たり判定が発動していたら
				if (m_bCollisionSuccess == true)
				{
					m_CollisionObjType = CObject::TYPE::PLAYER;// 当たったオブジェクトのタイプを「プレイヤー」とする
				}
			}

			pObj = pNext;//リストを次に進める
		}
	}

	// === 条件によって破棄する処理 ===

	// 当たり判定が成功し、当たった時にオブジェクトを消すフラグがtrueなら、死亡フラグを発動する
	if (m_bCollisionSuccess == true && GetCollisionRelease() == true)
	{
		SetDeath();
	}

}

//=====================================================================================================================
//他のオブジェクトにも当てる処理
//=====================================================================================================================
void CAttack::CollisionBuilding()
{
	// === 変数 ===
	CObjectX::PosInfo& SelfPosInfo = GetPosInfo();     // 位置情報
	CObjectX::SizeInfo& SelfSizeInfo = GetSizeInfo();  // サイズ情報
	const D3DXVECTOR3& SelfPos = SelfPosInfo.GetPos(); // 位置
	const D3DXVECTOR3& SelfVtxMax = SelfSizeInfo.GetVtxMax(); // 最大頂点
	const D3DXVECTOR3& SelfVtxMin = SelfSizeInfo.GetVtxMin(); // 最小頂点


	// === 建物系オブジェクトをリストから検索する ===
	for (int nCntPri = 0; nCntPri < m_nMAXPRIORITY; nCntPri++)
	{
		// === 変数 ===
		CObject* pObj = GetTopObject(nCntPri);//先頭オブジェクトを取得する

		// === 処理 ===

		// オブジェクトが存在していたら
		while (pObj != nullptr)
		{
			// === 変数 ===

			bool bNowCollision = false; // 現在当たったかどうか
			CObject* pNext = pObj->GetNextObject(); // 次のオブジェクト
			CObjectX* pObjX = nullptr; // オブジェクトXへのポインタ

			// === 処理 ===

			// オブジェクトのタイプが「背景モデル」か「ブロック」以外なら処理をスキップ
			if (pObj->GetType() != CObject::TYPE::BGMODEL && pObj->GetType() != CObject::TYPE::BLOCK)
			{
				pObj = pNext; // リストを次に進める
				continue;
			}

			pObjX = static_cast<CObjectX*>(pObj); // オブジェクトを「オブジェクトX」にキャスト

			// オブジェクトXが存在しないので処理をスキップ
			if (pObjX == nullptr)
			{
				pObj = pNext;
				continue;
			}

			// === 変数 ===

			CObjectX::PosInfo&
				OtherPosInfo = pObjX->GetPosInfo();      // 位置情報
			CObjectX::SizeInfo& 
				OtherSizeInfo = pObjX->GetSizeInfo();   // サイズ情報

			const D3DXVECTOR3&
				OtherPos = OtherPosInfo.GetPos(),        // 位置
				OtherVtxMax = OtherSizeInfo.GetVtxMax(), // 最大頂点
				OtherVtxMin = OtherSizeInfo.GetVtxMin(); // 最小頂点

			// === 処理 ===

			// 正方形の当たり判定を行う
			if (CCollision::CollisionSquare(SelfPos,SelfVtxMax,SelfVtxMin,OtherPos,OtherVtxMax,OtherVtxMin))
			{
				m_bCollisionSuccess = true; // 当たり判定が成功したフラグをオンにする
				m_CollisionObjType = pObj->GetType(); // 当たったオブジェクトのタイプを格納する
			}

			pObj = pNext; // リストを次に進める
		}
	}

	// === オブジェクトを消す処理 ===

	// 当たり判定が成功し、当たった時に消すフラグがtrueになっていたら死亡フラグを発動する
	if (m_bCollisionSuccess == true && m_bCollisionRelease == true)
	{
		SetDeath();
	}

}

//=====================================================================================================================
// 他のオブジェクトとの押し出し判定を行う処理
//=====================================================================================================================
void CAttack::ExtrusionCollisionProcess()
{
	// === 変数 ===

	CObjectX::CollisionInfo& CollisionInfo = GetCollisionInfo(); // 当たり判定情報
	CObjectX::CollisionInfo::State& CollisionState = CollisionInfo.GetState(); // 判定状態
	CollisionState.SetLandingOld(CollisionState.GetLanding()); // 1f前にオブジェクトが地面に乗っているかどうかを設定
	CollisionState.SetWallingOld(CollisionState.GetWalling()); // 1f前にオブジェクトが壁にいるかどうかを設定
	CollisionInfo.GetSquareInfo().ResetPushOutFirstFlag();     // それぞれの軸の押し出し判定の優先フラグをリセット
	CollisionState.ResetState(); // 判定状態リセット

	// === 押し出し判定を行うオブジェクトをリストから検索し、押し出し優先度を決める ===

	// リスト検索
	for (int nCntPri = 0; nCntPri < CObject::m_nMAXPRIORITY; nCntPri++)
	{
		// === 変数 ===

		CObject* pObj = CObject::GetTopObject(nCntPri);//リストの先頭オブジェクトを取得する

		// === 処理 ===

		// オブジェクトが存在するなら繰り返す
		while (pObj != nullptr)
		{
			// === 変数 ===

			CObject* pNext = pObj->GetNextObject(); // 次のオブジェクト
			CObject::TYPE type = pObj->GetType();   // 種類の取得（敵なら当たり判定）

			// === 処理 ===

			// オブジェクトの種類が「ブロック」と「背景モデル」以外なら処理をスキップ
			if (type != CObject::TYPE::BLOCK && type != CObject::TYPE::BGMODEL)
			{
				// オブジェクトを次に進める
				pObj = pNext;
				continue;
			}

			// === 変数 ===

			CObjectX* pObjX = static_cast<CObjectX*>(pObj); // オブジェクトXにダウンキャスト

			// === 処理 ===

			// オブジェクトXが存在していないなら処理をスキップ
			if (pObjX == nullptr)
			{
				// オブジェクトを次に進める
				pObj = pNext;
				continue;
			}

			CCollision::ExtrusionCollisionSquarePushOutFirstDecide(this, pObjX); // 正方形の押し出し判定のそれぞれの軸の順序の優先度を決める
			
			// オブジェクトを次に進める
			pObj = pNext;
		}
	}

	// === 押し出し判定を行うオブジェクトをリストから検索し、押し出し判定開始 ===

	// リスト検索
	for (int nCntPri = 0; nCntPri < CObject::m_nMAXPRIORITY; nCntPri++)
	{
		// === 変数 ===

		CObject* pObj = CObject::GetTopObject(nCntPri); //リストの先頭オブジェクト

		// === 処理 ===

		// オブジェクトが存在するなら繰り返す
		while (pObj != nullptr)
		{
			// === 変数 ===

			CObject* pNext = pObj->GetNextObject(); // 次のオブジェクトを格納
			CObject::TYPE type = pObj->GetType();   // 種類の取得

			// === 処理 ===

			// オブジェクトの種類が「ブロック」と「背景モデル」以外なら処理をスキップ
			if (type != CObject::TYPE::BLOCK && type != CObject::TYPE::BGMODEL)
			{
				// オブジェクトを次に進める
				pObj = pNext;
				continue;
			}

			// === 変数 ===

			CObjectX* pObjX = static_cast<CObjectX*>(pObj); // オブジェクトXにダウンキャスト

			// === 処理 ===

			// オブジェクトXが存在していないなら処理をスキップ
			if (pObjX == nullptr)
			{
				// オブジェクトを次に進める
				pObj = pNext;
				continue;
			}

			CCollision::ResolveExtrusionCollisionSquare(this, pObjX); // 正方形の押し出し判定をする

			pObj = pNext;//オブジェクトを次に進める
		}
	}
}

//=====================================================================================================================
//当たり判定共通処理
//=====================================================================================================================
void CAttack::CollisionProcess(bool& bCollision, bool& bNowCollision, CObjectX* pObjX)
{
	// === 変数 ===

	// 自分
	CObjectX::PosInfo& SelfPosInfo = GetPosInfo();     // 自分の位置情報
	CObjectX::SizeInfo& SelfSizeInfo = GetSizeInfo();  // 自分のサイズ情報
	const D3DXVECTOR3& SelfPos = SelfPosInfo.GetPos(); // 自分の位置
	const D3DXVECTOR3& SelfVtxMax = SelfSizeInfo.GetVtxMax(); // 自分の最大頂点
	const D3DXVECTOR3& SelfVtxMin = SelfSizeInfo.GetVtxMin(); // 自分の最小頂点
	
	// 相手
	CObjectX::PosInfo& OtherPosInfo = pObjX->GetPosInfo();     // 相手の位置情報
	CObjectX::SizeInfo& OtherSizeInfo = pObjX->GetSizeInfo();  // 相手のサイズ情報
	const D3DXVECTOR3& OtherPos = OtherPosInfo.GetPos(); // 相手の位置
	const D3DXVECTOR3& OtherVtxMax = OtherSizeInfo.GetVtxMax(); // 相手の最大頂点
	const D3DXVECTOR3& OtherVtxMin = OtherSizeInfo.GetVtxMin(); // 相手の最小頂点

	// === 当たり判定の種類によってポインタとして渡されたオブジェクトとの判定方法を変更して処理 ===

	// 判定種類によって変更
	switch (GetCollisionType())
	{
		// 正方形の当たり判定
	case CAttack::COLLISIONTYPE::SQUARE:
		if (CCollision::CollisionSquare(SelfPos, SelfVtxMax, SelfVtxMin, OtherPos, OtherVtxMax, OtherVtxMin))
		{
			bCollision = true;    // 当たった
			bNowCollision = true; // 現在当たった
		}
		break;
		// XZ方向の斜めの当たり判定
	case CAttack::COLLISIONTYPE::RECTANGLE_XZ:
		if (CCollision::RectAngleCollisionXZ(this, pObjX))
		{
			bCollision = true;    // 当たった
			bNowCollision = true; // 現在当たった
		}
		break;
	case CAttack::COLLISIONTYPE::OBBTOOBB:
		if (CCollision::OBBToOBB(this, pObjX))
		{
			bCollision = true;
			bNowCollision = true;
		}
		break;
	default:
		break;
	}

	// 当たったならダメージを与える
	if (bNowCollision == true)
	{
		pObjX->SetDamage(GetPower(), m_HitStop.nSetTime); // 引数に設定されているオブジェクトにダメージを与える
		pObjX->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), m_HitStop.nSetTime + 1, false, false,false); // 引数に設定されているオブジェクトの色合いを赤くする
	}

}

//*********************************************************************************************************************
// プレイヤー攻撃クラス
//*********************************************************************************************************************

//=====================================================================================================================
// 
// publicメンバ 
//
//=====================================================================================================================

//=====================================================================================================================
// コンストラクタ
//=====================================================================================================================
CAttackPlayer::CAttackPlayer(int nPower, int nSetHitStopTime) :
	CAttack(nPower,nSetHitStopTime)
{

}

//=====================================================================================================================
// デストラクタ
//=====================================================================================================================
CAttackPlayer::~CAttackPlayer()
{

}

//=====================================================================================================================
// 初期化処理
//=====================================================================================================================
HRESULT CAttackPlayer::Init()
{
	CAttack::Init(); // 攻撃の初期化処理
	return S_OK;
}

//=====================================================================================================================
// 終了処理
//=====================================================================================================================
void CAttackPlayer::Uninit()
{
	CAttack::Uninit(); // 攻撃の終了処理
}

//=====================================================================================================================
// 更新処理
//=====================================================================================================================
void CAttackPlayer::Update()
{
	CAttack::Update(); // 攻撃の更新処理

	BulletCollisionProcess();//弾が当たった時の処理
}

//=====================================================================================================================
// 描画処理
//=====================================================================================================================
void CAttackPlayer::Draw()
{
	CAttack::Draw(); // 攻撃の描画処理
}

//=====================================================================================================================
// 死亡フラグ設定処理
//=====================================================================================================================
void CAttackPlayer::SetDeath()
{
	CAttack::SetDeath(); // 攻撃の死亡フラグ設定処理
}

//=====================================================================================================================
// 生成処理
//=====================================================================================================================
CAttackPlayer* CAttackPlayer::Create(ATTACKTYPE AttackType, TARGETTYPE TargetType, COLLISIONTYPE CollisionType,
	bool bHitOtherThanLiving, bool bAutoCollision, int nPower, int nSetHitStopTime, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXVECTOR3 Scale)
{
	// === 変数 ===

	CAttackPlayer* pAttackPlayer = DBG_NEW CAttackPlayer(nPower, nSetHitStopTime); // プレイヤーの攻撃へのポインタ

	// === 情報設定処理 ===

	pAttackPlayer->Init(); //初期化処理
	pAttackPlayer->SetTargetType(TargetType);       // ターゲットタイプを設定
	pAttackPlayer->SetCollisionType(CollisionType); // 判定タイプを設定
	pAttackPlayer->SetType(CObject::TYPE::ATTACK);  // オブジェクトごとのタイプを設定する
	pAttackPlayer->SetAttackType(AttackType);       // 攻撃の種類を設定する
	pAttackPlayer->GetLifeInfo().SetLife(nLife);    // 体力を設定
	pAttackPlayer->GetLifeInfo().SetMaxLife(nLife); // 最大体力を設定
	pAttackPlayer->GetPosInfo().SetPos(pos);        // 位置  
	pAttackPlayer->GetPosInfo().SetSupportPos(pos); // 支点位置
	pAttackPlayer->GetRotInfo().SetRot(rot);        // 向き
	pAttackPlayer->GetMoveInfo().SetMove(move);     // 移動量
	pAttackPlayer->GetSizeInfo().SetScale(Scale);   // 拡大率
	pAttackPlayer->GetLifeInfo().SetAutoSubLife(true); // 体力を使用する
	pAttackPlayer->GetLifeInfo().SetAutoDeath(true);   // 体力が０になったときに自動的に死亡フラグを発動する 
	pAttackPlayer->GetMoveInfo().SetUseInteria(false, CObjectX::GetNormalInertia()); // 慣性を使用しない
	pAttackPlayer->GetMoveInfo().SetUseGravity(false,1.0f);    // 重力を使用しない
	pAttackPlayer->SetHitOtherThanLibing(bHitOtherThanLiving); // 背景オブジェクトとも当たり判定を行うかどうか
	pAttackPlayer->SetAutoCollision(bAutoCollision); //攻撃の当たり判定を攻撃クラスに任せるかどうか

	// モデル情報設定
	int nIdx = CManager::GetObjectXInfo()->Regist(ATTACK_FILENAME[static_cast<int>(AttackType)]); // 指定パスのインデックス取得

	// 上記で取得したインデックスのモデル情報を割り当てる
	pAttackPlayer->BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(nIdx),
		CManager::GetObjectXInfo()->GetBuffMat(nIdx),
		CManager::GetObjectXInfo()->GetdwNumMat(nIdx),
		CManager::GetObjectXInfo()->GetTexture(nIdx),
		CManager::GetObjectXInfo()->GetColorValue(nIdx));

	pAttackPlayer->SetSize(); // 割り当てられたモデル情報を基にサイズを設定する

	return pAttackPlayer;
}

//=====================================================================================================================
// 
// privateメンバ 
//
//=====================================================================================================================

//=====================================================================================================================
// 弾が当たった時の処理
//=====================================================================================================================
void CAttackPlayer::BulletCollisionProcess()
{
	// ゲームシーンなら
	if (CScene::GetMode() == CScene::MODE::MODE_GAME)
	{
		// 当たり判定が成功していて、攻撃の種類が「弾」だったら
		if (GetCollisionSuccess() == true && GetAttackType() == ATTACKTYPE::BULLET)
		{
			// === 変数 ===

			CObjectX::PosInfo& PosInfo = GetPosInfo(); // 位置情報を取得
			const D3DXVECTOR3& Pos = PosInfo.GetPos(); // 位置

			// === 処理 ===

			// 敵に当たっていたら、パーティクルを衝突位置に召喚する
			if (GetCollisionObjType() == CObject::TYPE::ENEMY)
			{
				// === 変数 ===
				CPlayer* pPlayer = CGame::GetPlayer(); // プレイヤーへのポインタを取得
				CUi* pDiveGaugeFrameUi = pPlayer->GetDiveGaugeFrame(); // ダイブゲージフレームへのポインタ

				// ゲージフレームのコンポジットパターンのコンテナを取得する
				CUIComposite_Container* pDiveGaugeFrameUiCompositeContainer = pDiveGaugeFrameUi->GetUiCompositeContainer(); 

				// ゲージの機能を取得する
				CUIComposite_Gauge* pDiveGaugeUi_CompositeGauge = pDiveGaugeFrameUiCompositeContainer->GetChildren<CUIComposite_Gauge>();

				// === 処理 ===

				// ダイブゲージが存在していなかったら処理をスキップ
				if (pDiveGaugeUi_CompositeGauge == nullptr)
				{
					return;
				}

				// === 変数 ===

				CGauge* pDiveGauge = pDiveGaugeUi_CompositeGauge->GetGauge(); // ダイブゲージを取得する
				CUi* pDivePossibleNum = pPlayer->GetDivePossibleNum(); // ダイブ可能回数のUIを取得

				// ダイブ可能回数のコンポジットパターンのコンテナを取得する
				CUIComposite_Container* pDivePossibleNumCompositeContainer = pDivePossibleNum->GetUiCompositeContainer();

				// ダイブ可能回数の数値表示機能を取得する
				CUIComposite_Numeric* pDivePossibleNum_Numeric = pDivePossibleNumCompositeContainer->GetChildren<CUIComposite_Numeric>();

				// === 処理 ===

				// 取得したUIの機能に「数字」がなかったら処理をスキップ
				if (pDivePossibleNum_Numeric == nullptr)
				{
					return;
				}

				// 最大ダイブ可能回数に達していない場合はダイブ可能回数を＋１する
				if (pDivePossibleNum_Numeric->GetValue() < CPlayer::GetMaxDiveNum())
				{
					pDiveGauge->SetParam(pDiveGauge->GetParam() + 1);
				}
				
				// 衝突位置にパーティクルを召喚する
				CParticle::SummonParticle(CParticle::TYPE::TYPE00_NORMAL, 20, 30, 30.0f, 30.0f, 100, 10, true, Pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), true);
			}
			// それ以外なら
			else
			{
				// 衝突位置にパーティクルを召喚する
				CParticle::SummonParticle(CParticle::TYPE::TYPE00_NORMAL, 20, 30, 30.0f, 30.0f, 100, 10, false, Pos, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), true);
			}
		}
	}
}

//*********************************************************************************************************************
// 敵攻撃クラス
//*********************************************************************************************************************

//=====================================================================================================================
// 
// publicメンバ 
//
//=====================================================================================================================

//=====================================================================================================================
// コンストラクタ
//=====================================================================================================================
CAttackEnemy::CAttackEnemy(int nPower, int nSetHitStopTime) : CAttack(nPower,nSetHitStopTime)
{

}

//=====================================================================================================================
// デストラクタ
//=====================================================================================================================
CAttackEnemy::~CAttackEnemy()
{

}

//=====================================================================================================================
// 初期化処理
//=====================================================================================================================
HRESULT CAttackEnemy::Init()
{
	CAttack::Init(); // 攻撃の初期化処理
	return S_OK;
}

//=====================================================================================================================
//終了処理
//=====================================================================================================================
void CAttackEnemy::Uninit()
{
	CAttack::Uninit(); //攻撃の終了処理
}

//=====================================================================================================================
// 更新処理
//=====================================================================================================================
void CAttackEnemy::Update()
{
	CAttack::Update(); // 攻撃の更新処理
}

//=====================================================================================================================
// 描画処理
//=====================================================================================================================
void CAttackEnemy::Draw()
{
	CAttack::Draw(); // 攻撃の描画処理
}

//=====================================================================================================================
// 死亡フラグ設定処理
//=====================================================================================================================
void CAttackEnemy::SetDeath()
{
	CAttack::SetDeath(); // 攻撃の死亡フラグ設定処理
}

//=====================================================================================================================
// 敵の攻撃を生成
//=====================================================================================================================
CAttackEnemy* CAttackEnemy::Create(ATTACKTYPE AttackType, TARGETTYPE TargetType, COLLISIONTYPE CollisionType,
	bool bHitOtherThanLiving, bool bAutoCollision, int nPower, int nSetHitStopTime, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXVECTOR3 Scale)
{
	// === 変数 ===

	CAttackEnemy* pAttackEnemy = DBG_NEW CAttackEnemy(nPower, nSetHitStopTime); // 敵へのポインタ

	// === 処理 ===

	pAttackEnemy->Init(); // 初期化処理
	pAttackEnemy->SetType(CObject::TYPE::ATTACK); // オブジェクトごとのタイプを設定する
	pAttackEnemy->SetAttackType(AttackType); // 攻撃の種類を設定する
	pAttackEnemy->SetTargetType(TargetType); // ターゲットタイプを設定する
	pAttackEnemy->SetCollisionType(CollisionType); // 判定タイプを設定する
	pAttackEnemy->GetLifeInfo().SetLife(nLife);    // 体力を設定
	pAttackEnemy->GetLifeInfo().SetMaxLife(nLife); // 最大体力を設定
	pAttackEnemy->GetLifeInfo().SetAutoSubLife(true); // 体力を自動的に減らす
	pAttackEnemy->GetPosInfo().SetPos(pos); // 位置  
	pAttackEnemy->GetRotInfo().SetRot(rot); // 向き
	pAttackEnemy->GetMoveInfo().SetMove(move);   // 移動量
	pAttackEnemy->GetSizeInfo().SetScale(Scale); // 拡大率
	pAttackEnemy->GetMoveInfo().SetUseInteria(false, CObjectX::GetNormalInertia()); // 慣性を使用しない
	pAttackEnemy->GetLifeInfo().SetAutoDeath(true); // 体力がなくなった時に自動的に死亡フラグを発動する
	pAttackEnemy->GetMoveInfo().SetUseGravity(false, 1.0f);   // 重力を使用する
	pAttackEnemy->SetHitOtherThanLibing(bHitOtherThanLiving); // 背景オブジェクトとも当たり判定を発動するかどうか
	pAttackEnemy->SetAutoCollision(bAutoCollision); // 攻撃クラスに判定を任せるかどうか

	// モデルパスからインデックスを取得
	int nIdx = CManager::GetObjectXInfo()->Regist(ATTACK_FILENAME[static_cast<int>(AttackType)]);

	// 上記で取得したインデックスからモデル情報を割り当てる
	pAttackEnemy->BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(nIdx),
		CManager::GetObjectXInfo()->GetBuffMat(nIdx),
		CManager::GetObjectXInfo()->GetdwNumMat(nIdx),
		CManager::GetObjectXInfo()->GetTexture(nIdx),
		CManager::GetObjectXInfo()->GetColorValue(nIdx));

	pAttackEnemy->SetSize(); // 割り当てられたモデル情報を基にオブジェクトのサイズを設定する
	return pAttackEnemy;
}

//*********************************************************************************************************************
// バウンド構造体
//*********************************************************************************************************************

//=====================================================================================================================
// 
// publicメンバ 
//
//=====================================================================================================================

//=====================================================================================================================
// バウンド処理
//=====================================================================================================================
void CAttack::BoundInfo::BoundProcess(CAttack* pAttack)
{
	// 地面に乗っていたら
	if (pAttack->GetCollisionInfo().GetState().GetLanding() == true)
	{
		// 跳ねさせる
		pAttack->GetMoveInfo().SetMove(D3DXVECTOR3(pAttack->GetMoveInfo().GetMove().x, Power.y, pAttack->GetMoveInfo().GetMove().z));

		// 重力がONの場合、攻撃自身にバウンド時の重力を設定
		if (bGravity == true)
		{
			pAttack->GetMoveInfo().SetUseGravity(true, fGravity);
		}
	}
}
