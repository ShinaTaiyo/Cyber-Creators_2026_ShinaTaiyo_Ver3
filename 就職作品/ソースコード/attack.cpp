//======================================================================================================================
//
//１０月２４日：攻撃用のクラスを作る[attack.cpp]
//Author:ShinaTaiyo
//
//======================================================================================================================

//==================================================================
//インクルード
//==================================================================
#include "attack.h"
#include "damage.h"
#include "manager.h"
#include "enemy.h"
#include "player.h"
#include"gauge.h"
#include "objectXInfo.h"
#include "block.h"
#include "bgModel.h"
#include "particle.h"
#include "game.h"
#include "collision.h"
//======================================================================================================================

//==================================================================
//前方宣言
//==================================================================
const string CAttack::ATTACK_FILENAME[static_cast<int>(CAttack::ATTACKTYPE::MAX)] =
{
	"data\\MODEL\\Attack\\Fire_000.x",
	"data\\MODEL\\Attack\\Explosion_000.x",
	"data\\MODEL\\Attack\\MagicSword_000.x",
};
//======================================================================================================================

//==================================================================
//コンストラクタ
//==================================================================
CAttack::CAttack(int nPower, int nSetHitStopTime, int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CObjectX(nPri, bUseintPri, type, ObjType),
m_Type(ATTACKTYPE::BULLET), m_nPower(nPower), m_HitStop({0,nSetHitStopTime}),m_bCollisionRelease(true),m_CollisionType(CAttack::COLLISIONTYPE::NONE),
m_TargetType(CAttack::TARGETTYPE::NONE),m_bHitOtherThanLiving(false),m_bAutoCollision(true),m_bCollisionSuccess(false),m_bExtrusionCollision(false),
m_BoundInfo(),m_CollisionObjType(CObject::TYPE::NONE)
{

}
//======================================================================================================================

//==================================================================
//デストラクタ
//==================================================================
CAttack::~CAttack()
{

}
//======================================================================================================================

//==================================================================
//初期化処理
//==================================================================
HRESULT CAttack::Init()
{
	CObjectX::Init();//オブジェクトX初期化
	return S_OK;
}
//======================================================================================================================

//==================================================================
//終了処理
//==================================================================
void CAttack::Uninit()
{
	CObjectX::Uninit();//オブジェクトX終了処理
}
//======================================================================================================================

//==================================================================
//更新処理
//==================================================================
void CAttack::Update()
{
	CObjectX::Update();//オブジェクトX更新処理

	m_bCollisionSuccess = false;//当たり判定が成功したかどうかをマイフレームリセット

	if (m_bAutoCollision == true)
	{//生きているものとの当たり判定を行う（プレイヤー、敵、ボスなど）
		Collision();
	}

	if (m_bHitOtherThanLiving == true)
	{//生きているもの以外（背景やブロック）とも当たり判定を行う
		HitOtherCollisionProcess();
	}
	else
	{//生きているものとだけ当たり判定を行う（当たったら消える）
		if (m_bExtrusionCollision == true)
		{//押し出し判定を行う（建物に当たっても消えるわけじゃない）
			ExtrusionCollisionProcess();

			if (m_BoundInfo.GetActive() == true)
			{//バウンドするならバウンドさせる
				m_BoundInfo.BoundProcess(this);
			}
		}
	}
}
//======================================================================================================================

//==================================================================
//描画処理
//==================================================================
void CAttack::Draw()
{
	CObjectX::Draw();//オブジェクトX描画処理
}
//======================================================================================================================

//==================================================================
//死亡フラグ設定処理
//==================================================================
void CAttack::SetDeath()
{
	CObjectX::SetDeath();//終了処理
}
//======================================================================================================================

//==================================================================
//当たり判定処理
//==================================================================
void CAttack::Collision()
{
	for (int nCntPri = 0; nCntPri < m_nMAXPRIORITY; nCntPri++)
	{//全てのリストを検索
		CObject* pObj = GetTopObject(nCntPri);//リストの先頭オブジェクトを取得
		while (pObj != nullptr)
		{
		    bool bNowCollision = false;//当たったかどうかを初期化
			CObject* pNext = pObj->GetNextObject();//次のオブジェクトを取得

			if (pObj->GetType() == CObject::TYPE::ENEMY && m_TargetType == TARGETTYPE::ENEMY)
			{//オブジェクトが敵で、攻撃が狙うオブジェクトが「敵」だった場合
				CEnemy* pEnemy = static_cast<CEnemy*>(pObj);//オブジェクトを敵にキャスト
				CollisionProcess(m_bCollisionSuccess, bNowCollision, pEnemy);//当たり判定の処理
				if (m_bCollisionSuccess == true)
				{//当たり判定が上記の処理で成功していたら
					m_CollisionObjType = CObject::TYPE::ENEMY;//当たったオブジェクトのタイプを格納する
					if (pEnemy->GetLifeInfo().GetLife() < 1)
					{//敵の体力がなくなっていたら攻撃が倒したオブジェクトのタイプを「敵」に設定
						pEnemy->SetDefeatAttack(m_Type);
					}
				}
			}
			else if (pObj->GetType() == CObject::TYPE::PLAYER && m_TargetType == TARGETTYPE::PLAYER)
			{//オブジェクトが「プレイヤー」で攻撃が狙うオブジェクトが「プレイヤー」だった場合
				CObjectX* pObjX = static_cast<CObjectX*>(pObj);//オブジェクトをオブジェクトXにキャスト
				CollisionProcess(m_bCollisionSuccess, bNowCollision, pObjX);//当たり判定の処理を行う

				if (m_bCollisionSuccess == true)
				{//上記の処理で当たり判定が発動していたら
					m_CollisionObjType = CObject::TYPE::PLAYER;//当たったオブジェクトのタイプを格納する
				}
			}

			pObj = pNext;//リストを次に進める
		}
	}

	if (m_bCollisionSuccess == true && GetCollisionRelease() == true)
	{//当たり判定が成功し、当たった時にオブジェクトを消すフラグがtrueなら、死亡フラグを発動する
		SetDeath();
	}

}
//======================================================================================================================

//==================================================================
//他のオブジェクトにも当てる処理
//==================================================================
void CAttack::HitOtherCollisionProcess()
{
	for (int nCntPri = 0; nCntPri < m_nMAXPRIORITY; nCntPri++)
	{//全てのリストを検索する
		CObject* pObj = GetTopObject(nCntPri);//先頭オブジェクトを取得する
		while (pObj != nullptr)
		{//オブジェクトがnullptrになるまで繰り返す
			bool bNowCollision = false;//現在当たったかどうかを初期化
			CObject* pNext = pObj->GetNextObject();//リストの次のオブジェクトを取得

			if (pObj->GetType() == CObject::TYPE::BGMODEL || pObj->GetType() == CObject::TYPE::BLOCK)
			{//オブジェクトのタイプが「背景モデル」か「ブロック」なら
				CObjectX* pObjX = static_cast<CObjectX*>(pObj);//オブジェクトを「オブジェクトX」にキャスト

				if (CCollision::CollisionSquare(GetPosInfo().GetPos(), GetSizeInfo().GetVtxMax(), GetSizeInfo().GetVtxMin(), pObjX->GetPosInfo().GetPos(), pObjX->GetSizeInfo().GetVtxMax(), pObjX->GetSizeInfo().GetVtxMin()))
				{//正方形の当たり判定を行う
					m_bCollisionSuccess = true;//当たり判定が成功したフラグをオンにする
					m_CollisionObjType = pObj->GetType();//当たったオブジェクトのタイプを格納する
				}
			}

			pObj = pNext;//リストを次に進める
		}
	}

	if (m_bCollisionSuccess == true && m_bCollisionRelease == true)
	{//当たり判定が成功し、当たった時に消すフラグがtrueになっていたら死亡フラグを発動する
		SetDeath();
	}

}
//======================================================================================================================

//==================================================================
//他のオブジェクトとの押し出し判定を行う処理
//==================================================================
void CAttack::ExtrusionCollisionProcess()
{
	CObjectX::CollisionInfo& CollisionInfo = GetCollisionInfo();//当たり判定情報を取得する
	CObjectX::CollisionInfo::State& CollisionState = CollisionInfo.GetState();//フラグをリセット
	CollisionState.SetLandingOld(CollisionState.GetLanding());     //1f前にオブジェクトが地面に乗っているかどうかを設定
	CollisionState.SetWallingOld(CollisionState.GetWalling());     //1f前にオブジェクトが壁にいるかどうかを設定
	CollisionState.ResetState();                                   //ステート
	CollisionInfo.GetSquareInfo().ResetPushOutFirstFlag();         //それぞれの軸の押し出し判定の優先フラグをリセット

	for (int nCntPri = 0; nCntPri < CObject::m_nMAXPRIORITY; nCntPri++)
	{//全てのリストを検索する
		CObject* pObj = CObject::GetTopObject(nCntPri);//リストの先頭オブジェクトを取得する

		while (pObj != nullptr)
		{//オブジェクトがnullptrになるまで繰り返す
			//次のオブジェクトを格納
			CObject* pNext = pObj->GetNextObject();//次のオブジェクトを取得する

			//種類の取得（敵なら当たり判定）
			CObject::TYPE type = pObj->GetType();

			if (type == CObject::TYPE::BLOCK || type == CObject::TYPE::BGMODEL)
			{//オブジェクトの種類が「ブロック」または「背景モデル」なら
				CObjectX* pObjX = static_cast<CObjectX*>(pObj);//オブジェクトXにダウンキャスト
				CCollision::ExtrusionCollisionSquarePushOutFirstDecide(this, pObjX);//正方形の押し出し判定のそれぞれの軸の順序の優先度を決める
			}
			//オブジェクトを次に進める
			pObj = pNext;
		}
	}
	//=======================================================================================

	//============================================================
	//押し出し判定開始
	//============================================================
	for (int nCntPri = 0; nCntPri < CObject::m_nMAXPRIORITY; nCntPri++)
	{//全てのリストを検索する
		CObject* pObj = CObject::GetTopObject(nCntPri);//リストの先頭オブジェクトを取得する

		while (pObj != nullptr)
		{//オブジェクトがnullptrになるまで繰り返す
			//次のオブジェクトを格納
			CObject* pNext = pObj->GetNextObject();

			//種類の取得（敵なら当たり判定）
			CObject::TYPE type = pObj->GetType();

			if (type == CObject::TYPE::BLOCK || type == CObject::TYPE::BGMODEL)
			{//オブジェクトの種類が「ブロック」または「背景モデル」なら
				CObjectX* pObjX = static_cast<CObjectX*>(pObj);//オブジェクトXにダウンキャスト

				CCollision::ResolveExtrusionCollisionSquare(this, pObjX);//正方形の押し出し判定をする
			}

			pObj = pNext;//オブジェクトを次に進める
		}
	}
	//=======================================================================================
}
//======================================================================================================================

//==================================================================
//当たり判定共通処理
//==================================================================
void CAttack::CollisionProcess(bool& bCollision, bool& bNowCollision, CObjectX* pObjX)
{
	switch (GetCollisionType())
	{//当たり判定の種類によって変える
	case CAttack::COLLISIONTYPE::SQUARE://正方形の当たり判定
		if (CCollision::CollisionSquare(GetPosInfo().GetPos(), GetSizeInfo().GetVtxMax(), GetSizeInfo().GetVtxMin(), pObjX->GetPosInfo().GetPos(), pObjX->GetSizeInfo().GetVtxMax(), pObjX->GetSizeInfo().GetVtxMin()) == true)
		{//正方形の当たり判定を行う
			bCollision = true;//当たった
			bNowCollision = true;//現在当たった
		}
		break;
	case CAttack::COLLISIONTYPE::RECTANGLE_XZ://XZ方向の斜めの当たり判定
		if (CCollision::RectAngleCollisionXZ(this, pObjX) == true)
		{//XZ方向の斜めの当たり判定を行う
			bCollision = true;//当たった
			bNowCollision = true;//現在当たった
		}
		break;
	default:
		break;
	}

	if (bNowCollision == true)
	{//当たったら
		pObjX->SetDamage(GetPower(), m_HitStop.nSetTime);//引数に設定されているオブジェクトにダメージを与える
		pObjX->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), m_HitStop.nSetTime, false, false,false);//引数に設定されているオブジェクトの色合いを赤くする
	}

}
//======================================================================================================================

//=============================================================================プレイヤー攻撃クラス=============================================================================

//==================================================================
//コンストラクタ
//==================================================================
CAttackPlayer::CAttackPlayer(int nPower, int nSetHitStopTime) : CAttack(nPower,nSetHitStopTime)
{

}
//======================================================================================================================

//==================================================================
//デストラクタ
//==================================================================
CAttackPlayer::~CAttackPlayer()
{

}
//======================================================================================================================

//==================================================================
//初期化処理
//==================================================================
HRESULT CAttackPlayer::Init()
{
	CAttack::Init();//攻撃の初期化処理
	return S_OK;
}
//======================================================================================================================

//==================================================================
//終了処理
//==================================================================
void CAttackPlayer::Uninit()
{
	CAttack::Uninit();//攻撃の終了処理
}
//======================================================================================================================

//==================================================================
//更新処理
//==================================================================
void CAttackPlayer::Update()
{
	CAttack::Update();//攻撃の更新処理

	BulletCollisionProcess();//弾が当たった時の処理
}
//======================================================================================================================

//==================================================================
//描画処理
//==================================================================
void CAttackPlayer::Draw()
{
	CAttack::Draw();//攻撃の描画処理
}
//======================================================================================================================

//==================================================================
//死亡フラグ設定処理
//==================================================================
void CAttackPlayer::SetDeath()
{
	CAttack::SetDeath();//攻撃の死亡フラグ設定処理
}
//======================================================================================================================

//==================================================================
//生成処理
//==================================================================
CAttackPlayer* CAttackPlayer::Create(ATTACKTYPE AttackType, TARGETTYPE TargetType, COLLISIONTYPE CollisionType,
	bool bHitOtherThanLiving, bool bAutoCollision, int nPower, int nSetHitStopTime, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXVECTOR3 Scale)
{
	CAttackPlayer* pAttackPlayer = nullptr;                                         //プレイヤーの攻撃へのポインタを初期化
	pAttackPlayer = DBG_NEW CAttackPlayer(nPower,nSetHitStopTime);                  //生成
	pAttackPlayer->Init();                                                          //初期化処理
	pAttackPlayer->SetTargetType(TargetType);                                       //ターゲットタイプを設定
	pAttackPlayer->SetCollisionType(CollisionType);                                 //判定タイプを設定
	pAttackPlayer->SetType(CObject::TYPE::ATTACK);                                  //オブジェクトごとのタイプを設定する
	pAttackPlayer->SetAttackType(AttackType);                                       //攻撃の種類を設定する
	pAttackPlayer->GetLifeInfo().SetLife(nLife);                                    //体力を設定
	pAttackPlayer->GetLifeInfo().SetMaxLife(nLife);                                 //最大体力を設定
	pAttackPlayer->GetPosInfo().SetPos(pos);                                        //位置  
	pAttackPlayer->GetPosInfo().SetSupportPos(pos);                                 //支点位置
	pAttackPlayer->GetRotInfo().SetRot(rot);                                        //向き
	pAttackPlayer->GetMoveInfo().SetMove(move);                                     //移動量
	pAttackPlayer->GetSizeInfo().SetScale(Scale);                                   //拡大率
	pAttackPlayer->GetLifeInfo().SetAutoSubLife(true);                              //体力を使用する
	pAttackPlayer->GetLifeInfo().SetAutoDeath(true);                                //体力が０になったときに自動的に死亡フラグを発動する 
	pAttackPlayer->GetMoveInfo().SetUseInteria(false, CObjectX::GetNormalInertia());//慣性を使用しない
	pAttackPlayer->GetMoveInfo().SetUseGravity(false,1.0f);                         //重力を使用しない
	pAttackPlayer->SetHitOtherThanLibing(bHitOtherThanLiving);                      //背景オブジェクトとも当たり判定を行うかどうか
	pAttackPlayer->SetAutoCollision(bAutoCollision);                                //攻撃の当たり判定を攻撃クラスに任せるかどうか

	//モデル情報設定
	int nIdx = CManager::GetObjectXInfo()->Regist(ATTACK_FILENAME[static_cast<int>(AttackType)]);

	//モデル情報を割り当てる
	pAttackPlayer->BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(nIdx),
		CManager::GetObjectXInfo()->GetBuffMat(nIdx),
		CManager::GetObjectXInfo()->GetdwNumMat(nIdx),
		CManager::GetObjectXInfo()->GetTexture(nIdx),
		CManager::GetObjectXInfo()->GetColorValue(nIdx));

	pAttackPlayer->SetSize();//サイズを設定する
	return pAttackPlayer;
}
//======================================================================================================================

//==================================================================
//弾が当たった時の処理
//==================================================================
void CAttackPlayer::BulletCollisionProcess()
{
	if (CScene::GetMode() == CScene::MODE::MODE_GAME)
	{//ゲームシーンなら
		if (GetCollisionSuccess() == true && GetAttackType() == ATTACKTYPE::BULLET)
		{//当たり判定が成功していて、攻撃の種類が「弾」だったら
			CObjectX::PosInfo& PosInfo = GetPosInfo();//位置情報を取得
			const D3DXVECTOR3& Pos = PosInfo.GetPos();//位置
			//パーティクルを衝突位置に召喚する
			if (GetCollisionObjType() == CObject::TYPE::ENEMY)
			{//敵に当たっていたら
				CPlayer* pPlayer = CGame::GetPlayer();//プレイヤーへのポインタを取得
				CUi* pDiveGaugeFrameUi = pPlayer->GetDiveGaugeFrame();//ダイブゲージフレームへのポインタ
				//ダイブゲージの機能の取得
				CUIComposite_Container* pDiveGaugeFrameUiCompositeContainer = pDiveGaugeFrameUi->GetUiCompositeContainer();                //ゲージフレームのコンポジットパターンのコンテナを取得する
				CUIComposite_Gauge* pDiveGaugeUi_CompositeGauge = pDiveGaugeFrameUiCompositeContainer->GetChildren<CUIComposite_Gauge>();  //ゲージの機能を取得する
				if (pDiveGaugeUi_CompositeGauge != nullptr)
				{
					CGauge* pDiveGauge = pDiveGaugeUi_CompositeGauge->GetGauge();//ダイブゲージを取得する

					CUi* pDivePossibleNum = pPlayer->GetDivePossibleNum();//ダイブ可能回数のUIを取得

					//ダイブ可能回数のUIのコンポジットパターンから機能を取得
					CUIComposite_Container* pDivePossibleNumCompositeContainer = pDivePossibleNum->GetUiCompositeContainer();//ダイブ可能回数のコンポジットパターンのコンテナを取得する
					CUIComposite_Numeric* pDivePossibleNum_Numeric = pDivePossibleNumCompositeContainer->GetChildren<CUIComposite_Numeric>();//ダイブ可能回数の数値表示機能を取得する

					if (pDivePossibleNum_Numeric != nullptr)
					{//取得したUIの機能が数字だったら
						if (pDivePossibleNum_Numeric->GetValue() < CPlayer::GetMaxDiveNum())
						{//最大ダイブ可能回数に達していない場合はダイブ可能回数を＋１する
							pDiveGauge->SetParam(pDiveGauge->GetParam() + 1);
						}
					}
				}
				//パーティクルを召喚する
				CParticle::SummonParticle(CParticle::TYPE::TYPE00_NORMAL, 20, 30, 30.0f, 30.0f, 100, 10, true, Pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), true);
			}
			else
			{//それ以外なら

				//パーティクルを召喚する
				CParticle::SummonParticle(CParticle::TYPE::TYPE00_NORMAL, 20, 30, 30.0f, 30.0f, 100, 10, false, Pos, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), true);
			}
		}
	}
}
//======================================================================================================================

//=============================================================================敵攻撃クラス=============================================================================

//==================================================================
//コンストラクタ
//==================================================================
CAttackEnemy::CAttackEnemy(int nPower, int nSetHitStopTime) : CAttack(nPower,nSetHitStopTime)
{

}
//======================================================================================================================

//==================================================================
//デストラクタ
//==================================================================
CAttackEnemy::~CAttackEnemy()
{

}
//======================================================================================================================

//==================================================================
//初期化処理
//==================================================================
HRESULT CAttackEnemy::Init()
{
	CAttack::Init();//攻撃の初期化処理
	return S_OK;
}
//======================================================================================================================

//==================================================================
//終了処理
//==================================================================
void CAttackEnemy::Uninit()
{
	CAttack::Uninit();//攻撃の終了処理
}
//======================================================================================================================

//==================================================================
//更新処理
//==================================================================
void CAttackEnemy::Update()
{
	CAttack::Update();//攻撃の更新処理
}
//======================================================================================================================

//==================================================================
//描画処理
//==================================================================
void CAttackEnemy::Draw()
{
	CAttack::Draw();//攻撃の描画処理
}
//======================================================================================================================

//==================================================================
//死亡フラグ設定処理
//==================================================================
void CAttackEnemy::SetDeath()
{
	CAttack::SetDeath();//攻撃の死亡フラグ設定処理
}
//======================================================================================================================

//==================================================================
//敵の攻撃を生成
//==================================================================
CAttackEnemy* CAttackEnemy::Create(ATTACKTYPE AttackType, TARGETTYPE TargetType, COLLISIONTYPE CollisionType,
	bool bHitOtherThanLiving, bool bAutoCollision, int nPower, int nSetHitStopTime, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXVECTOR3 Scale)
{
	CAttackEnemy* pAttackEnemy = DBG_NEW CAttackEnemy(nPower, nSetHitStopTime);

	pAttackEnemy->Init();                                       //初期化処理
	pAttackEnemy->SetType(CObject::TYPE::ATTACK);               //オブジェクトごとのタイプを設定する
	pAttackEnemy->SetAttackType(AttackType);                    //攻撃の種類を設定する
	pAttackEnemy->SetTargetType(TargetType);                    //ターゲットタイプを設定する
	pAttackEnemy->SetCollisionType(CollisionType);              //判定タイプを設定する
	pAttackEnemy->GetLifeInfo().SetLife(nLife);                 //体力を設定
	pAttackEnemy->GetLifeInfo().SetMaxLife(nLife);              //最大体力を設定
	pAttackEnemy->GetLifeInfo().SetAutoSubLife(true);           //体力を自動的に減らす
	pAttackEnemy->GetPosInfo().SetPos(pos);                     //位置  
	pAttackEnemy->GetRotInfo().SetRot(rot);                     //向き
	pAttackEnemy->GetMoveInfo().SetMove(move);                  //移動量
	pAttackEnemy->GetSizeInfo().SetScale(Scale);                //拡大率
	pAttackEnemy->GetMoveInfo().SetUseInteria(false, CObjectX::GetNormalInertia());//慣性を使用しない
	pAttackEnemy->GetLifeInfo().SetAutoDeath(true);                                //体力がなくなった時に自動的に死亡フラグを発動する
	pAttackEnemy->GetMoveInfo().SetUseGravity(false, 1.0f);                        //重力を使用する
	pAttackEnemy->SetHitOtherThanLibing(bHitOtherThanLiving);                      //背景オブジェクトとも当たり判定を発動するかどうか
	pAttackEnemy->SetAutoCollision(bAutoCollision);                                //攻撃クラスに判定を任せるかどうか
	//モデル情報設定
	int nIdx = CManager::GetObjectXInfo()->Regist(ATTACK_FILENAME[static_cast<int>(AttackType)]);

	//モデル情報を割り当てる
	pAttackEnemy->BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(nIdx),
		CManager::GetObjectXInfo()->GetBuffMat(nIdx),
		CManager::GetObjectXInfo()->GetdwNumMat(nIdx),
		CManager::GetObjectXInfo()->GetTexture(nIdx),
		CManager::GetObjectXInfo()->GetColorValue(nIdx));
	pAttackEnemy->SetSize();//オブジェクトのサイズを設定する
	return pAttackEnemy;
}
//======================================================================================================================

//==================================================================
//バウンド処理
//==================================================================
void CAttack::BoundInfo::BoundProcess(CAttack* pAttack)
{
	if (pAttack->GetCollisionInfo().GetState().GetLanding() == true)
	{//地面に乗っていたら

		//跳ねさせる
		pAttack->GetMoveInfo().SetMove(D3DXVECTOR3(pAttack->GetMoveInfo().GetMove().x, Power.y, pAttack->GetMoveInfo().GetMove().z));

		if (bGravity == true)
		{//重力をONにする場合
			pAttack->GetMoveInfo().SetUseGravity(true, fGravity);
		}
	}
}
//======================================================================================================================