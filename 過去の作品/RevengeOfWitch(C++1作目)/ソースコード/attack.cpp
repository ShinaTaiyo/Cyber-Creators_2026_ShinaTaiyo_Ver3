//=================================================
//
//７月２５日：敵のAIを実装[attack.cpp]
//Author:ShinaTaiyo
//
//=================================================

//==============================
//インクルード
//==============================
#include "attack.h"
#include "manager.h"
#include "game.h"
#include "edit.h"
#include "bullet.h"
#include "particle.h"
#include "emitter.h"
#include "texture.h"
#include "manager.h"
#include "objectXInfo.h"
#include "input.h"
#include "sound.h"
#include "billboard.h"
#include "calculation.h"
#include "collision.h"
#include "player.h"
#include "camera.h"
#include "boss.h"
#include <math.h>
//=================================================
//==============================
//静的メンバ初期化
//==============================
int CAttack::m_nNumFile = CAttack::ATTACKTYPE_MAX;                      //ファイルの数
const char* CAttack::m_ATTACK_FILENAME[CAttack::ATTACKTYPE_MAX] =
{ "data\\MODEL\\Attack\\Attack00_Fire.x",
 "data\\MODEL\\Attack\\Attack01_Meteo.x",
 "data\\MODEL\\Attack\\Attack02_Needle.x",
 "data\\MODEL\\Attack\\Attack03_Tornado.x",
 "data\\MODEL\\Attack\\Attack04_Beam.x",
 "data\\MODEL\\Attack\\Attack05_Ice.x",
 "data\\MODEL\\Attack\\Attack06_Thunder.x",
 "data\\MODEL\\Attack\\Attack07_MagicSword.x",
 "data\\MODEL\\Attack\\Attack08_VacuumBlade_2.x",
 "data\\MODEL\\Attack\\Attack09_Hammer.x",
 "data\\MODEL\\Attack\\Attack10_ShockWave.x",
 "data\\MODEL\\Attack\\Attack11_Explosion.x",
 "data\\MODEL\\Attack\\Attack12_Vine.x",
 "data\\MODEL\\Attack\\Attack13_CrossBomb.x",
 "data\\MODEL\\Attack\\Attack14_HellFire.x",
 "data\\MODEL\\Attack\\Attack15_BlackHole.x",
 "data\\MODEL\\Attack\\Attack16_PhycoBurst.x",
 "data\\MODEL\\Attack\\Attack17_Block.x",
 "data\\MODEL\\Attack\\Attack18_Thornclub.x",
 "data\\MODEL\\Attack\\Attack19_Senter_Thunder.x",
};
//=================================================

//==============================
//コンストラクタ
//==============================
CAttack::CAttack() : m_CollisionType(COLLISIONTYPE00_SQUARE),m_nSetEffectLife(0),m_SetEffectType(CEffect::EFFECTTYPE00_NORMAL),
m_TargetType(TARGETTYPE_ENEMY),m_bCollisionRelease(false),m_bDelaySetDeath(true),m_bDoCollision(false),m_bPatternFlag(false),
m_bUse(true),m_bUseAttackEffect(false),m_bUseGravity(false),m_bUseReflection(false),m_bUseSetEffect(false),m_bUseThinColor(false),m_fGravityPower(0.0f),m_nAttackCnt(0),
m_nCntTime(0),m_nPattern(0),m_nPatternTime(0),m_nPower(0),m_nSetAttackEffectLife(0),m_nSetHitStopTime(0),m_nSubType(0),m_type(ATTACKTYPE00_FIRE)
{

}
//=================================================

//==============================
//デストラクタ
//==============================
CAttack::~CAttack()
{

}
//=================================================

//==============================
//初期化処理
//==============================
HRESULT CAttack::Init()
{
	//===========================
	//２Dオブジェクト初期化
	//===========================
	if (FAILED(CObjectX::Init()))
	{
		return E_FAIL;
	}
	//====================================

	////===========================
	////初期化
	////===========================
	//m_bUse = true;                            //使用状態
	//m_CollisionType = COLLISIONTYPE00_SQUARE; //当たり判定の種類
	//m_nPower = 0;                             //ダメージ量
	//m_nCntTime = 0;                           //生成されてからの時間を計測する
	//m_bCollisionRelease = false;              //当たったときに消すかどうか
	//m_nAttackCnt = 0;                         //攻撃回数をカウントする
	//m_nSetHitStopTime = 0;                    //当たったときに割り当てるヒットストップ時間
	//m_bDoCollision = false;                   //当たり判定をするかどうか
	//m_bCollisionPossible = true;              //当たり判定が可能かどうか

	//m_bUseThinColor = false;                  //色を薄くしていくかどうか

	//m_bDelaySetDeath = true;                 //死亡フラグ発動を遅くするかどうか

	//m_PurposePos = NULL_VECTOR3;              //目的地代入用

	////====================================
	////反射関係
	////====================================
	//m_bUseReflection = false;
	////===============================================================================

	////====================================
	////エフェクト関係
	////====================================
	//m_bUseSetEffect = false;//エフェクトを使用するかどうか
	//m_nSetEffectLife = 0;//設定するエフェクトの体力
	//m_SetEffectSize = D3DXVECTOR2(0.0f, 0.0f);//設定するエフェクトのサイズ
	//m_SetEffectType = (CEffect::EFFECTTYPE)(0);//設定するエフェクトの種類
	//m_SetEffectColor = NORMAL_COL * 0.0f;//設定するエフェクトの色合い
	////===============================================================================

	////====================================
 //   //攻撃エフェクト関係
	////====================================
	//m_bUseAttackEffect = false;//攻撃エフェクトを使用するかどうか
	//m_nSetAttackEffectLife = 0;//設定する攻撃エフェクトの体力
	//m_SetAttackEffectCol = NORMAL_COL;//設定する攻撃エフェクトの色合い
	////===============================================================================

	////====================================
	////パターン関係
	////====================================
	//m_nPattern = 0;                     //攻撃のパターン
	//m_nPatternTime = 0;                 //攻撃パターン時間
	//m_bPatternFlag = false;             //パターンのフラグ
	////====================================

	if (GetUseInitialSound() == true)
	{
		switch (m_type)
		{
		case ATTACKTYPE00_FIRE:
			CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_FIRE);
			break;
		case ATTACKTYPE04_BEAM:
			CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PUNCH);
			break;
		case ATTACKTYPE06_THUNDER:
			CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_THUNDER);
			break;
		case ATTACKTYPE12_VINE:
			CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_VINE);
			break;
		case ATTACKTYPE13_CROSSBOMB:
			CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_FIRE_002);
			break;
		case ATTACKTYPE16_PHYCOBURST:
			CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PHYCOBURST_000);
			break;
		default:
			break;
		}
	}
	//====================================
	//パターン関係
	//====================================
	m_nSubType = 0;//動き方を決めるときなどに使う
	//===========================================================
	return S_OK;
}
//=================================================

//==============================
//終了処理
//==============================
void CAttack::Uninit()
{
	CObjectX::Uninit();
	//SetDeath();
	////Release();//オブジェクト破棄
}
//=================================================

//==============================
//更新処理
//==============================
void CAttack::Update()
{
	m_nCntTime++;                                                 //生成されてからの時間を計測する
	m_nPatternTime++;                                             //それぞれの動きの時間を計測する
	//==================================
	//オブジェクトXから情報を取得
	//==================================
	D3DXVECTOR3 PosOld = GetPosOld();                             //1f前の位置を設定
	D3DXVECTOR3& Move = GetMove();                                //移動量を設定
	D3DXVECTOR3 Size = GetSize();                                 //サイズを取得
	D3DXVECTOR3& Scale = GetScale();                               //拡大率を取得
	D3DXVECTOR3 & Pos = CObjectX::GetPos();                         //位置を取得
	D3DXVECTOR3 & Rot = GetRot();                                   //向きを取得
	D3DXVECTOR3 ParticlePos = CObjectX::GetPos();
	int& nLife = GetLife();                                       //体力を取得
	int nMaxLife = GetMaxLife();                                  //最大体力を取得
	float fVXaim = 0.0f;
	float fVYaim = 0.0f;
	float fVLaim = 0.0f;
	float fRatioLife = 0.0f;
	float fRot = (float)(rand() % 60 - 30) / 100;
	float fRandAddSpeed = (float)(rand() % 1000) / 1000;
	bool bCollision = false;//当たっているかどうか
	D3DXVECTOR3 RandMove = NULL_VECTOR3;                          //乱数移動量   
	CParticle* pParticle = nullptr;                               //パーティクル取得用  
	//===========================================================

	//==================================
	//エフェクトがオンになっていたら
	//==================================
	if (m_bUseSetEffect == true)
	{
		CEffect::Create(m_SetEffectType, m_nSetEffectLife, m_SetEffectSize.x, m_SetEffectSize.y, Pos, m_SetEffectColor);
	}

	//============================================
	//色を薄くしていくフラグがONになっていたら
	//============================================
	if (m_bUseThinColor == true)
	{
		fRatioLife = (float)(nLife) / (float)(nMaxLife);
		SetColorAlphaOnly(fRatioLife);//色のアルファ値を設定
	}
	//==========================================================

	//==================================
	//重力がONになっていたら
	//==================================
	if (m_bUseGravity == true)
	{
		Move.y += m_fGravityPower;
	}
	//==========================================================

	//==================================
	//Xオブジェクト更新
	//==================================
	CObjectX::Update();
	//===========================================================

	//==================================
	//攻撃エフェクト関係
	//==================================
	if (m_bUseAttackEffect == true)
	{
		if (GetUseParent() == false)
		{
			CAttackEffect::Create(m_type, 10, m_nSetAttackEffectLife, Pos, NULL_VECTOR3, Scale, Rot, NULL_VECTOR3, m_SetAttackEffectCol);
		}
		else
		{
			CAttackEffect::Create(m_type, 10, m_nSetAttackEffectLife,GetOffSetPos(), NULL_VECTOR3, Scale, GetParentRot(), NULL_VECTOR3, m_SetAttackEffectCol);
		}
	}

	//==================================
	//反射関係
	//==================================
	if (m_bUseReflection == true)
	{
		if ((GetSuccessBlockCollisionX() == true))
		{//Xベクトルを反転させる
			Move.x *= -1.0f;
		}
		if (GetSuccessBlockCollisionY() == true || GetPos().y + GetVtxMin().y < 0.0f || GetPos().y + GetVtxMax().y > 400.0f)
		{//Yベクトルを反転させる
			Move.y *= -1.0f;
		}
	}
	//===========================================================
	
	//==================================
	//当たり判定を行う
	//==================================
	if (CScene::GetMode() == CScene::MODE_GAME)
	{
		if (m_bDoCollision == true)
		{
			if (GetCntTime() > 1)
			{
				switch (m_CollisionType)
				{
				case COLLISIONTYPE00_SQUARE:
					Collision();
					break;
				case COLLISIONTYPE01_RECTANGLEXY:
					RectAngleCollisionXY();
					break;
				case COLLISIONTYPE02_TRUERECTANGLEXY:
					TrueRectAngleCollisionXY();
					break;
				case COLLISIONTYPE03_BALL:
					BallCollision();
					break;
				default:
					break;
				}
			}
		}
	}
	//===========================================================

	//死亡処理
	if (nLife < 1)
	{
		m_bUse = false;
	}


	//==================================
	//攻撃の破棄
	//==================================
	if (m_bDelaySetDeath == false)
	{
		if (m_bUse == false)
		{
			switch (m_type)
			{
			case ATTACKTYPE05_ICE:
				for (int nCnt = 0; nCnt < 30; nCnt++)
				{
					RandMove.x = float(rand() % 150 - 75) / 5;
					RandMove.y = float(rand() % 150 - 75) / 5;
					RandMove.z = float(rand() % 150 - 75) / 5;
					pParticle = CParticle::Create(CParticle::TYPE02_ICE, 50, 80.0f, 80.0f, Pos, RandMove, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), true);
					pParticle->SetGravity(-0.4f);
					pParticle = nullptr;
				}
				CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_ICE000);
				break;
			case ATTACKTYPE13_CROSSBOMB:
				CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_EXPLOSION_000);
				break;
			default:
				break;
			}

			SetDeath();//死亡フラグを設定
			//Uninit();//死亡フラグを設定
		}
	}
	if (m_bDelaySetDeath == true && m_bUse == false)
	{//死亡フラグを遅らせるフラグ発動時にオフにする（これにより、trueのオブジェクトは1f遅らせることができ、Collisionに、deathが発動した場合の処理を書かなくて良い。許容範囲。）
		m_bDelaySetDeath = false;
	}
	//===========================================================
}
//=================================================

//==============================
//描画処理
//==============================
void CAttack::Draw()
{
	CObjectX::Draw();
}
//=================================================

//==========================================================================
//死亡フラグを設定
//==========================================================================
void CAttack::SetDeath()
{
	CObject::SetDeath();
}
//==========================================================================================


//==========================================================================
//重力を設定
//==========================================================================
void CAttack::SetUseGravity(float fGravityPower)
{
	m_bUseGravity = true;
	m_fGravityPower = fGravityPower;
}
//===================================================================================================================================================

//==========================================================================
//生成処理
//==========================================================================
CAttack* CAttack::Create(ATTACKTYPE type, int nDamage, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 Scale, TARGETTYPE TargetType, D3DXVECTOR3 Rot,
	COLLISIONTYPE CollisionType, D3DXVECTOR3 AddRot, int nHitStopTime)
{
	CAttack* pAttack = DBG_NEW CAttack;   //敵を生成
	bool bSuccess = pAttack->CObject::GetCreateSuccess();
	int nIdx = 0;//モデルのインデックス取得用
	if (bSuccess == true)
	{
		if (pAttack != nullptr)
		{
			pAttack->m_type = type;  //弾の種類
			pAttack->Init();         //初期化処理
			pAttack->SetHitStopTime(nHitStopTime);//当たったときのヒットストップ時間を設定する
			pAttack->SetUseBlockCollision(false);
			pAttack->SetUseInteria(false);//慣性を使用しない
			pAttack->m_bUse = true;  //使用状態
			pAttack->SetMove(move);  //移動量の設定
			pAttack->SetLife(nLife); //体力を設定
			pAttack->SetMaxLife(nLife);//最大体力を設定
			pAttack->SetAutoSubLife(true);              //自動的に体力を減らすかどうか
			pAttack->m_TargetType = TargetType;//誰に当てるかを設定
			pAttack->m_CollisionType = CollisionType;//当たり判定の種類
			pAttack->m_nPower = nDamage;               //ダメージ量
			pAttack->SetUseAddRot(true,AddRot);                 //加算する向きの量
			pAttack->SetDoCollision(true);              //当たり判定をするかどうかを設定
			pAttack->CObjectX::SetPos(pos);             //オブジェクトXの位置を設定
			pAttack->SetRot(Rot);                       //向きを設定
			pAttack->SetPosOld(pos);                                                                                         //1f前の位置を設定
			pAttack->SetSupportPos(pos);           
			pAttack->CObject::SetType(CObject::TYPE_ATTACK);                                                                  //オブジェクトの種類を決める
			pAttack->SetScale(Scale);                                                                              //オブジェクトの拡大率
			pAttack->SetFormarScale(Scale);                                       //元の拡大率を設定する
			pAttack->SetUseDeath(true);//死亡フラグを発動するかどうかを設定する
			nIdx = CManager::GetObjectXInfo()->Regist(CObjectX::OBJECTXTYPE_ATTACK, m_ATTACK_FILENAME[(int)(type)]);
			pAttack->CObjectX::BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(CObjectX::OBJECTXTYPE_ATTACK,nIdx),
				CManager::GetObjectXInfo()->GetBuffMat(CObjectX::OBJECTXTYPE_ATTACK, nIdx),
				CManager::GetObjectXInfo()->GetdwNumMat(CObjectX::OBJECTXTYPE_ATTACK, nIdx),
				CManager::GetObjectXInfo()->GetTexture(CObjectX::OBJECTXTYPE_ATTACK, nIdx),
				CManager::GetObjectXInfo()->GetColorValue(CObjectX::OBJECTXTYPE_ATTACK, nIdx));                       //モデル情報を割り当てる

			pAttack->CObjectX::SetObjXType(CObjectX::OBJECTXTYPE_ATTACK);                            //オブジェクトXのタイプを設定
			pAttack->CObjectX::SetTypeNum((int)(type));                                             //オブジェクトXごとのタイプ番号を設定
			pAttack->SetSize();                                                                     //Xオブジェクトのサイズを設定する
			pAttack->CObjectX::SetRotType(CObjectX::ROTTYPE::ROTTYPE_NORMAL);                       //向きの種類を設定
		}
		else
		{//オブジェクトに空きがなかったので破棄する
			delete pAttack;
			pAttack = nullptr;
			return nullptr;
		}
	}
	return pAttack;
}
//===================================================================================================================================================

//==========================================================================
//色を薄くしていくかどうかを決める
//==========================================================================
void CAttack::SetUseThinColor(bool bUse)
{
	m_bUseThinColor = bUse;
}
//===================================================================================================================================================

//=====================================================================
//当たり判定を取る
//=====================================================================
void CAttack::Collision()
{
	//==================================
	//オブジェクトXから情報を取得
	//==================================
	D3DXVECTOR3 Pos = GetPos();                      //オブジェクトの位置を取得
	D3DXVECTOR3 Size = GetSize();                    //オブジェクトのサイズを取得
	D3DXVECTOR3 VtxMax = GetVtxMax();
	D3DXVECTOR3 VtxMin = GetVtxMin();
	D3DXVECTOR3 ComparisonPos = NULL_VECTOR3;                  //プレイヤーの位置代入用
	D3DXVECTOR3 ComparisonVtxMax = NULL_VECTOR3;               //判定対象の最大頂点
	D3DXVECTOR3 ComparisonVtxMin = NULL_VECTOR3;               //判定対象の最小頂点
	CPlayer* pPlayer = nullptr;                                //プレイヤークラス取得用
	CEnemy* pEnemy = nullptr;                                  //敵クラス取得用 
	CBoss* pBoss = nullptr;                                    //ボス取得用
	CObject* pObj = nullptr;
	//===========================================================

	float fSpeedX = 0.0f;
	float fSpeedY = 0.0f;
	float fSpeedZ = 0.0f;

	//==================================
	//攻撃の当たり判定
	//==================================
	if (m_TargetType == TARGETTYPE_PLAYER)
	{//ターゲットがプレイヤーなら
		ComparisonPos = CGame::GetPlayer()->GetPos();                   //位置を取得
		ComparisonVtxMax = CGame::GetPlayer()->GetVtxMax();             //最大頂点を取得
		ComparisonVtxMin = CGame::GetPlayer()->GetVtxMin();             //最小頂点を取得
		if (Pos.x + VtxMin.x / 2 <= ComparisonPos.x + ComparisonVtxMax.x &&
			Pos.x + VtxMax.x / 2 >= ComparisonPos.x + ComparisonVtxMin.x &&
			Pos.y + VtxMin.y <= ComparisonPos.y + ComparisonVtxMax.y &&
			Pos.y + VtxMax.y >= ComparisonPos.y + ComparisonVtxMin.y &&
			Pos.z + VtxMin.z / 2 <= ComparisonPos.z + ComparisonVtxMax.x &&
			Pos.z + VtxMax.z / 2 >= ComparisonPos.z + ComparisonVtxMin.x)
		{//攻撃に当たったら
			CGame::GetPlayer()->SetDamage(m_nPower,1);
			if (m_bCollisionRelease == true)
			{
				m_bUse = false;//使用状態をオフにする
			}
		}
	}
	else if (m_TargetType == TARGETTYPE_ENEMY)
	{//ターゲットが敵なら
		for (int nCntPriority = 0; nCntPriority < m_nMAXPRIORITY; nCntPriority++)
		{

			pObj = GetTopObject(nCntPriority);//トップオブジェクトを取得
			while (pObj != nullptr)
			{
				//次のオブジェクトを格納
				CObject* pNext = pObj->GetNextObject();

				//種類の取得（敵なら当たり判定）
				CObject::TYPE type = pObj->GetType();
				switch (type)
				{
				case CObject::TYPE_ENEMY:
					pEnemy = (CEnemy*)pObj;                             //オブジェクトを敵の型にダウンキャスト
					ComparisonPos = pEnemy->GetPos();                   //位置を取得
					ComparisonVtxMax = pEnemy->GetVtxMax();             //最大頂点を取得
					ComparisonVtxMin = pEnemy->GetVtxMin();             //最小頂点を取得
					break;
				case CObject::TYPE_BOSS:
					pBoss = (CBoss*)pObj;                               //オブジェクトをボスの型にダウンキャスト
					ComparisonPos = pBoss->GetPos();                   //位置を取得
					ComparisonVtxMax = pBoss->GetVtxMax();             //最大頂点を取得
					ComparisonVtxMin = pBoss->GetVtxMin();             //最小頂点を取得
					break;
				default:
					pObj = pNext;
					continue;
					break;
				}

				if (Pos.x + VtxMin.x / 2 <= ComparisonPos.x + ComparisonVtxMax.x &&
					Pos.x + VtxMax.x / 2 >= ComparisonPos.x + ComparisonVtxMin.x &&
					Pos.y + VtxMin.y <= ComparisonPos.y + ComparisonVtxMax.y &&
					Pos.y + VtxMax.y >= ComparisonPos.y + ComparisonVtxMin.y &&
					Pos.z + VtxMin.z / 2 <= ComparisonPos.z + ComparisonVtxMax.x &&
					Pos.z + VtxMax.z / 2 >= ComparisonPos.z + ComparisonVtxMin.x)
				{//攻撃に当たったら
					switch (type)
					{
					case CObject::TYPE_ENEMY:
						if (pEnemy->GetHitStop() == false)
						{
							pEnemy->SetDamage(m_nPower,m_nSetHitStopTime);//ダメージを与える
							if (m_bCollisionRelease == true)
							{
								m_bUse = false;//使用状態をオフにする
							}
						}
						break;
					case CObject::TYPE_BOSS:
						if (pBoss->GetHitStop() == false)
						{
							pBoss->SetDamage(m_nPower,m_nSetHitStopTime);
							if (m_bCollisionRelease == true)
							{
								m_bUse = false;//使用状態をオフにする
							}
						}
						break;
					default:
						continue;
						break;
					}
				}
				//リストを次に進める
				pObj = pNext;
			}
		}
	}

}
//===================================================================================================================================================

//======================================================================
//XY方向の斜めを含めた判定を行う
//======================================================================
void CAttack::RectAngleCollisionXY()
{
	D3DXVECTOR3 FourVtxPos[4] = {};   //４頂点の位置
	D3DXVECTOR3 FourVtxRotPos[4] = {};//回転した４頂点の位置
	
	D3DXVECTOR3 Pos = GetPos();//位置
	D3DXVECTOR3 ComparisonPos = NULL_VECTOR3;//比較対象の位置
	D3DXVECTOR3 ComparisonSize = NULL_VECTOR3;//比較対象のサイズ
	D3DXVECTOR3 ComparisonVtxMax = NULL_VECTOR3;
	D3DXVECTOR3 ComparisonVtxMin = NULL_VECTOR3;
	D3DXVECTOR3 Rot = GetRot();//向き
	D3DXVECTOR3 VtxMax = GetVtxMax();//最大頂点
	D3DXVECTOR3 VtxMin = GetVtxMin();//最小頂点
	float fSpeedX = 0.0f;
	float fSpeedY = 0.0f;
	float fSpeedZ = 0.0f;

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		ComparisonPos = CGame::GetPlayer()->GetPos();
		ComparisonSize = CGame::GetPlayer()->GetSize();
		ComparisonVtxMax = CGame::GetPlayer()->GetVtxMax();
		ComparisonVtxMin = CGame::GetPlayer()->GetVtxMin();
		switch (nCnt)
		{
		case 0://左上
			ComparisonPos.x += ComparisonVtxMin.x;
			ComparisonPos.y += ComparisonVtxMax.y;
			break;
		case 1://右上
			ComparisonPos.x += ComparisonVtxMax.x;
			ComparisonPos.y += ComparisonVtxMax.y;
			break;
		case 2://左下
			ComparisonPos.x += ComparisonVtxMin.x;
			break;
		case 3://右下
			ComparisonPos.x += ComparisonVtxMax.x;
			break;
		default:
			break;
		}

		//=================================
		//計算に使う４頂点の位置
		//=================================
		if (CScene::GetMode() == CScene::MODE_GAME)
		{
			//左上頂点
			FourVtxPos[0] = D3DXVECTOR3(Pos.x + VtxMin.x, Pos.y + VtxMax.y, 0.0f);

			//右上頂点
			FourVtxPos[1] = D3DXVECTOR3(Pos.x + VtxMax.x, Pos.y + VtxMax.y, 0.0f);

			//左下頂点
			FourVtxPos[2] = D3DXVECTOR3(Pos.x + VtxMin.x, Pos.y + VtxMin.y, 0.0f);

			//右下頂点
			FourVtxPos[3] = D3DXVECTOR3(Pos.x + VtxMax.x, Pos.y + VtxMin.y, 0.0f);

			//=============================================================================================================================================================================

			//==================================
			//中心点と４頂点の距離を求める
			//==================================
			float fLength[4] = {};
			for (int nCnVtx = 0; nCnVtx < 4; nCnVtx++)
			{
				fLength[nCnVtx] = sqrtf(powf((FourVtxPos[nCnVtx].x - Pos.x), 2) + powf((FourVtxPos[nCnVtx].y - Pos.y), 2));
			}
			//=============================================================================================================================================================================

			//==================================
			//中心点から４頂点の角度を求める
			//==================================
			float fVaim[4] = {};
			for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
			{
				fVaim[nCntVtx] = atan2f(FourVtxPos[nCntVtx].x - Pos.x, FourVtxPos[nCntVtx].y - Pos.y);
			}
			//=============================================================================================================================================================================

			//==================================
			//回転頂点の位置を求める
			//==================================
			for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
			{
				//判定を合わせるため、Rotに-を付ける
				FourVtxRotPos[nCntVtx] = D3DXVECTOR3(sinf(-Rot.z + fVaim[nCntVtx]) * fLength[nCntVtx] + Pos.x, cosf(-Rot.z + fVaim[nCntVtx]) * fLength[nCntVtx] + Pos.y, 0.0f);
			}
			//=============================================================================================================================================================================

			//===============================================================
			//矩形の各辺の始点と終点のベクトルを用意する（ベクトルA）
			//===============================================================
			float fVecAX[4] = {};
			float fVecAY[4] = {};

			fVecAX[0] = FourVtxRotPos[1].x - FourVtxRotPos[0].x;
			fVecAX[1] = FourVtxRotPos[3].x - FourVtxRotPos[1].x;
			fVecAX[2] = FourVtxRotPos[2].x - FourVtxRotPos[3].x;
			fVecAX[3] = FourVtxRotPos[0].x - FourVtxRotPos[2].x;

			fVecAY[0] = FourVtxRotPos[1].y - FourVtxRotPos[0].y;
			fVecAY[1] = FourVtxRotPos[3].y - FourVtxRotPos[1].y;
			fVecAY[2] = FourVtxRotPos[2].y - FourVtxRotPos[3].y;
			fVecAY[3] = FourVtxRotPos[0].y - FourVtxRotPos[2].y;

			float fCheckAim[4] = {};
			for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
			{
				fCheckAim[nCntVtx] = atan2f(fVecAX[nCntVtx], fVecAY[nCntVtx]);
			}
			//=============================================================================================================================================================================

			//=============================================================================================
			//ターゲットとなる点（プレイヤー、エネミーなど）と４頂点のベクトルを用意する（ベクトルB）
			//=============================================================================================
			float fVecBX[4] = {};
			float fVecBY[4] = {};


			fVecBX[0] = ComparisonPos.x - FourVtxRotPos[0].x;
			fVecBX[1] = ComparisonPos.x - FourVtxRotPos[1].x;
			fVecBX[2] = ComparisonPos.x - FourVtxRotPos[3].x;
			fVecBX[3] = ComparisonPos.x - FourVtxRotPos[2].x;

			fVecBY[0] = ComparisonPos.y - FourVtxRotPos[0].y;
			fVecBY[1] = ComparisonPos.y - FourVtxRotPos[1].y;
			fVecBY[2] = ComparisonPos.y - FourVtxRotPos[3].y;
			fVecBY[3] = ComparisonPos.y - FourVtxRotPos[2].y;

			float fCheckAim2[4] = {};

			for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
			{
				fCheckAim2[nCntVtx] = atan2f(fVecBX[nCntVtx], fVecBY[nCntVtx]);
			}

			//========================================================
			//上二つで作ったベクトルの外積を計算する
			//========================================================
			float fCross[4] = {};
			for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
			{
				fCross[nCntVtx] = (fVecAX[nCntVtx] * fVecBY[nCntVtx]) - (fVecBX[nCntVtx] * fVecAY[nCntVtx]);
			}

			//========================================================
			//上下判定をする
			//========================================================
			int nCntCheck = 0;//判定が何個成功しているか確認する

			for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
			{
				if (fCross[nCntVtx] <= 0.0f)
				{//値が正なら矩形の内側にある。
					nCntCheck++;
				}
			}

			if (nCntCheck >= 4)
			{//全ての外積の値が正の値（矩形の中にある
				if (ComparisonPos.z + ComparisonSize.z / 2 >= Pos.z + VtxMin.z &&
					ComparisonPos.z - ComparisonSize.z / 2 <= Pos.z + VtxMax.z && m_TargetType == TARGETTYPE_PLAYER)
				{//矩形内に入ったときのプレイヤーとモデルの中心点との距離分押し出す
					if (m_bCollisionRelease == true)
					{
						m_bUse = false;//使用状態をオフにする
					}
					CGame::GetPlayer()->SetDamage(m_nPower,m_nSetHitStopTime);
					for (int nCnt = 0; nCnt < 20; nCnt++)
					{
						fSpeedX = float(rand() % 200 - 100) / 10;
						fSpeedY = float(rand() % 200 - 100) / 10;
						fSpeedZ = float(rand() % 200 - 100) / 10;
						CParticle::Create(CParticle::TYPE01_FIRE, 30, 20.0f, 20.0f, CGame::GetPlayer()->GetPos(), D3DXVECTOR3(fSpeedX, fSpeedY, fSpeedZ), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), true);
					}
					break;
				}
			}
		}
	}
}

//======================================================================
//正しいXY方向の回転矩形同士の当たり判定
//======================================================================
void CAttack::TrueRectAngleCollisionXY()
{

	//オブジェクト
	D3DXVECTOR3 Pos = GetPos();//位置
	D3DXVECTOR3 Rot = GetRot();//向き
	D3DXVECTOR3 VtxMax = GetVtxMax();//最大頂点
	D3DXVECTOR3 VtxMin = GetVtxMin();//最小頂点
	if (GetUseParent() == true)
	{//親がいたら
		Pos = GetOffSetPos();//オフセットの位置を取得

		Rot = GetParentRot() + GetRot();

		float fSpeedX = float(rand() % 200 - 100) / 10;
		float fSpeedY = float(rand() % 200 - 100) / 10;
		float fSpeedZ = float(rand() % 200 - 100) / 10;
		CParticle::Create(CParticle::TYPE01_FIRE, 30, 20.0f, 20.0f, Pos, D3DXVECTOR3(fSpeedX, fSpeedY, fSpeedZ), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), true);

	}

	D3DXVECTOR2 FourVtxPos[4] = {};   //４頂点の位置
	D3DXVECTOR2 FourVtxRotPos[4] = {};//回転した４頂点の位置
	D3DXVECTOR3 ComparisonPos = NULL_VECTOR3;//比較対象の位置
	D3DXVECTOR2 FourComparisonVtxPos[4] = {};    //比較用４頂点
	D3DXVECTOR3 ComparisonSize = NULL_VECTOR3;//比較対象のサイズ
	D3DXVECTOR3 ComparisonVtxMax = NULL_VECTOR3;
	D3DXVECTOR3 ComparisonVtxMin = NULL_VECTOR3;
	CObject* pObj = nullptr;//オブジェクト格納用
	bool bCollision = true;
	bool bEnd = false;        //ループを終わらせるフラグ
	CEnemy* pEnemy = nullptr;//敵へのポインタ
	CBoss* pBoss = nullptr;  //ボスへのポインタ
	CObjectX* pObjX = nullptr;//オブジェクトXへのポインタ
	CBullet* pBullet = nullptr;//弾へのポインタ

	if (m_TargetType == TARGETTYPE_PLAYER)
	{
		bCollision = CCollision::RectAngleCollisionXY(Pos, VtxMax, VtxMin, Rot,
			CGame::GetPlayer()->GetPos(), CGame::GetPlayer()->GetVtxMax(), CGame::GetPlayer()->GetVtxMin(), CGame::GetPlayer()->GetRot());

		if (bCollision == true)
		{
			CGame::GetPlayer()->SetDamage(1, 10);
			if (m_bCollisionRelease == true)
			{
				SetDeath();
			}
		}
	}
	else if (m_TargetType == TARGETTYPE_ENEMY)
	{
		for (int nCntPriority = 0; nCntPriority < m_nMAXPRIORITY; nCntPriority++)
		{

			pObj = GetTopObject(nCntPriority);//トップオブジェクトを取得
			while (pObj != nullptr)
			{
				//次のオブジェクトを格納
				CObject* pNext = pObj->GetNextObject();

				//種類の取得（敵なら当たり判定）
				CObject::TYPE type = pObj->GetType();
				bCollision = true;

				if (type == CObject::TYPE_ENEMY || type == CObject::TYPE_BOSS)
				{
					pObjX = (CObjectX*)pObj;

					bCollision = CCollision::RectAngleCollisionXY(Pos, VtxMax, VtxMin, Rot,
						pObjX->GetPos(), pObjX->GetVtxMax(), pObjX->GetVtxMin(), pObjX->GetRot());

					if (bCollision == true)
					{
						pObjX->SetDamage(m_nPower, m_nSetHitStopTime);
						if (m_bCollisionRelease == true)
						{
							//SetDeath();
							//SetDelayDeath(true);
							m_bUse = false;
						}
					}
				}
				else if (type == CObject::TYPE_BULLET)
				{
					if (m_type == ATTACKTYPE12_VINE)
					{
						pBullet = (CBullet*)pObj;
						bCollision = CCollision::RectAngleCollisionXY(Pos, VtxMax, VtxMin, Rot,
							pBullet->GetPos(),D3DXVECTOR3(pBullet->GetWidth(),pBullet->GetHeight(),pBullet->GetWidth()) * 0.5f,
							D3DXVECTOR3(-pBullet->GetWidth(), -pBullet->GetHeight(), -pBullet->GetWidth()) * 0.5f,NULL_VECTOR3);

						if (bCollision == true && pBullet->GetBulletType() == CBullet::BULLETTYPE_ENEMY)
						{
							pBullet->SetDeath();
							if (m_bCollisionRelease == true)
							{
								SetDeath();
							}
						}
					}
				}


				if (bEnd == true)
				{//ループ終了（当たったら消える攻撃が判定されたとき
					break;
				}
				//リストを次に進める
				pObj = pNext;
			}
		}
	}

}
//===================================================================================================================================================

//=====================================================================
//球の当たり判定を行う
//=====================================================================
void CAttack::BallCollision()
{
	CObject* pObj = nullptr;//オブジェクトへのポインタ
	CObjectX* pObjX = nullptr;//オブジェクトXへのポインタ
	bool bCollision = false;//当たったかどうか
	for (int nCntPriority = 0; nCntPriority < m_nMAXPRIORITY; nCntPriority++)
	{
		pObj = GetTopObject(nCntPriority);//トップオブジェクトを取得
		while (pObj != nullptr)
		{
			CObject* pNext = pObj->GetNextObject();
			CObject::TYPE type = pObj->GetType();
			if (type == CObject::TYPE_PLAYER || type == CObject::TYPE_ENEMY || type == CObject::TYPE_BOSS)
			{
				pObjX = (CObjectX*)pObj;

				bCollision = CCollision::CollisionBall(GetSenterPos(), GetSize(), pObjX->GetSenterPos(), pObjX->GetSize());

				if (bCollision == true)
				{
					switch (m_TargetType)
					{
					case TARGETTYPE_ENEMY:
						if (type == CObject::TYPE_BOSS || type == CObject::TYPE_ENEMY)
						{
							pObjX->SetDamage(m_nPower, m_nSetHitStopTime);
						}
						break;
					case TARGETTYPE_PLAYER:
						if (type == CObject::TYPE_PLAYER)
						{
							pObjX->SetDamage(m_nPower, m_nSetHitStopTime);
						}
						break;
					default:
						break;
					}
				}
			}
			else
			{
				pObj = pNext;
				continue;
			}
			pObj = pNext;//次の進める
		}
	}
}
//===================================================================================================================================================

//===================================================================<<<プレイヤー攻撃クラス>>>===========================================================

//=====================================================================
//コンストラクタ
//=====================================================================
CAttackPlayer::CAttackPlayer()
{

}
//===================================================================================================================================================

//=====================================================================
//デストラクタ
//=====================================================================
CAttackPlayer::~CAttackPlayer()
{
}
//===================================================================================================================================================

//=====================================================================
//初期化処理
//=====================================================================
HRESULT CAttackPlayer::Init()
{
	CAttack::Init();               //攻撃の初期化
	m_fCorrectionRot = 0.0f;       //補正用向き
	m_nVineDelBulletNum = 0;       //つるが消す弾の数

	m_SaveScale = NULL_VECTOR3;    //拡大率保存用
	m_TotalRot = NULL_VECTOR3;     //総合的向き
	m_bVineDelStart = false;       //つるを消す処理開始フラグ

	m_bCrossBombStaging = false;   //クロスボムの演出フラグ
	m_nCrossBombStagingCnt = 0;    //クロスボムの演出時間をカウントする

	m_nBlackHoleDefeatCnt = 0;     //ブラックホールで倒した敵の数をカウントする
	return S_OK;
}
//===================================================================================================================================================

//=====================================================================
//終了処理
//=====================================================================
void CAttackPlayer::Uninit()
{
	CAttack::Uninit();//攻撃の終了処理
}
//===================================================================================================================================================

//=====================================================================
//更新処理
//=====================================================================
void CAttackPlayer::Update()
{
	if (CScene::GetMode() == CScene::MODE_GAME)
	{
		D3DXVECTOR3 Size = GetSize();                                 //サイズを取得
		D3DXVECTOR3 ParticlePos = CObjectX::GetPos();
		D3DXVECTOR3 & Rot = GetRot();                                   //向きを取得
		D3DXVECTOR3 & Move = GetMove();                                //移動量を設定
		D3DXVECTOR3 & Pos = GetPos();                                   //位置を取得
		D3DXVECTOR3 & Scale = GetScale();
		CEmitter* pEmitter = nullptr;                                 //エミッター
		CParticle* pParticle = nullptr;                               //パーティクル取得用
		float fRot = (float)(rand() % 60 - 30) / 100;
		float fRandAddSpeed = (float)(rand() % 1000) / 1000;
		float fRatioLife = (float)(GetLife()) / (float)(GetMaxLife());//最大体力との割合を求める
		bool bTurn = false;
		int& nCntTime = GetCntTime();
		int& nPattern = GetPattern();
		switch (GetAttackType())
		{
		case ATTACKTYPE00_FIRE:
			ParticlePos.x += (float)(rand() % (int)(Size.x) - (int)(Size.x / 2)) * 2;
			ParticlePos.y += (float)(rand() % (int)(Size.y) - (int)(Size.y / 2));
			pParticle = CParticle::Create(CParticle::TYPE00_NORMAL, 50, 40.0f, 40.0f,ParticlePos, D3DXVECTOR3(sinf(D3DX_PI * 0.5f + fRot) * -Move.x, cosf(D3DX_PI * 0.5f + fRot) * -Move.x, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), true);
			pParticle->SetAddSpeed(fRandAddSpeed);//加速量を設定
			pParticle = nullptr;
			break;
		case ATTACKTYPE04_BEAM:
			if (CScene::GetMode() == CScene::MODE_GAME)
			{
				Pos = CGame::GetPlayer()->GetPos();
			}
			pParticle = CParticle::Create(CParticle::TYPE01_FIRE,60, 30.0f, 30.0f, ParticlePos, D3DXVECTOR3(sinf(-Rot.z + fRot) * 10.0f, cosf(-Rot.z + fRot) * 10.0f, 0.0f),
				NORMAL_COL, true);
			pParticle = nullptr;
			break;
		case ATTACKTYPE05_ICE:
			Scale += ONE_VECTOR3 * 0.1f;
			break;
		case ATTACKTYPE06_THUNDER:
			if (fRatioLife > 0.5f)
			{
				Scale.y += 0.3f;
			}

			D3DXVECTOR3(0.0f, -0.37f, 0.0f);
			if (nCntTime % 30 == 0)
			{
			    pEmitter = CEmitter::Create((int)(CEffect::EFFECTTYPE01_THUNDER), 60, 20, 20.0f, 20.0f, 40.0f, Pos + D3DXVECTOR3(0.0f, Size.y / 2, 0.0f), 
					D3DXVECTOR3(0.0f, -12.0f, 0.0f),D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				pEmitter->SetUseRotate(true, 20.0f, NULL_VECTOR3, D3DXVECTOR3(0.0f, -0.37f, 0.0f));
			}
			else if (nCntTime % 15 == 0)
			{
				pEmitter = CEmitter::Create((int)(CEffect::EFFECTTYPE01_THUNDER), 60, 20, 20.0f, 20.0f, 40.0f, Pos + D3DXVECTOR3(0.0f, Size.y / 2, 0.0f),
					D3DXVECTOR3(0.0f, -12.0f, 0.0f),D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				pEmitter->SetUseRotate(true, 20.0f, NULL_VECTOR3, D3DXVECTOR3(0.0f, 0.37f, 0.0f));
			}
			Rot.y += 0.4f;
			break;
		case ATTACKTYPE07_MAGICSWORD:
			Pos = CGame::GetPlayer()->GetPos();
			Pos.y += CGame::GetPlayer()->GetSize().y / 2;
			//プレイヤーの向きによって攻撃方向を変える

			if (CGame::GetPlayer()->GetRot().y >= D3DX_PI)
			{
				Rot.z = -m_TotalRot.z;
				bTurn = true;
			}
			else
			{
				Rot.z = m_TotalRot.z;
				bTurn = false;
			}


			switch (nPattern)
			{
			case 0:
				SetDoCollision(false);
				m_TotalRot = NULL_VECTOR3;
				if (nCntTime == 1)
				{
					CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CLAW);
				}
				if ((CManager::GetInputKeyboard()->GetTrigger(DIK_M) == true || CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_B) == true)&&
					nCntTime >= 3)
				{
					nPattern++;
					m_fCorrectionRot = 0.3f;
				}
				break;
			case 1:
				SetDoCollision(true);
				m_TotalRot.z += m_fCorrectionRot;
				CAttackEffect::Create(CAttack::ATTACKTYPE07_MAGICSWORD, 0, 30, Pos, NULL_VECTOR3, ONE_VECTOR3 * 2, Rot, NULL_VECTOR3, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
				if (m_TotalRot.z >= D3DX_PI - (D3DX_PI * 0.5f))
				{
					if (m_nAttackCnt == 0)
					{
						CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_SLASH_000);
						if (bTurn == true)
						{
							CAttackPlayer::Create(CAttack::ATTACKTYPE08_VACUUMBLADE, 35, 100, Pos, D3DXVECTOR3(sinf(D3DX_PI * 0.5f) * 10.0f, 0.0f, cosf(D3DX_PI * 0.5f) * 10.0f), ONE_VECTOR3 * 2.0f,D3DXVECTOR3(0.0f,0.0f,D3DX_PI * 0.5f),
								CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, NULL_VECTOR3, true, 10);
						}
						else
						{
							CAttackPlayer::Create(CAttack::ATTACKTYPE08_VACUUMBLADE, 35, 100, Pos, D3DXVECTOR3(sinf(D3DX_PI * -0.5f) * 10.0f, 0.0f, cosf(D3DX_PI * -0.5f) * 10.0f), ONE_VECTOR3 * 2.0f, D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * -0.5f),
								CAttack::COLLISIONTYPE02_TRUERECTANGLEXY, NULL_VECTOR3, true, 10);
						}
						m_nAttackCnt++;
					}
					m_fCorrectionRot += (0.0f - m_fCorrectionRot) * 0.15f;
				}

				if (m_fCorrectionRot <= 0.001f)
				{
					m_nAttackCnt = 0;
					nPattern = 0;
					nCntTime = 0;
				}
				break;
			default:
				break;
			}
			break;
		case ATTACKTYPE08_VACUUMBLADE:
			CAttackEffect::Create(CAttack::ATTACKTYPE08_VACUUMBLADE, 0, 30, Pos, NULL_VECTOR3, ONE_VECTOR3 * 2, Rot, NULL_VECTOR3, NORMAL_COL);
			break;
		case ATTACKTYPE12_VINE:
			VineProcess();
			break;
		case ATTACKTYPE13_CROSSBOMB:
			CrossBombProcess();
			break;
		case ATTACKTYPE15_BLACKHOLE:
			BlackHoleProcess();
			break;
		case ATTACKTYPE16_PHYCOBURST:
			PhycoBurstProcess();
			break;
		}
		CAttack::Update();//攻撃の更新処理
	}
}
//===================================================================================================================================================

//=====================================================================
//描画処理
//=====================================================================
void CAttackPlayer::Draw()
{
	CAttack::Draw();
}
//===================================================================================================================================================

//=====================================================================
//死亡フラグ設定処理
//=====================================================================
void CAttackPlayer::SetDeath()
{
	CAttack::SetDeath();
}
//===================================================================================================================================================

//=====================================================================
//生成処理
//=====================================================================
CAttackPlayer* CAttackPlayer::Create(ATTACKTYPE type, int nDamage, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 Scale, D3DXVECTOR3 Rot,
	COLLISIONTYPE CollisionType, D3DXVECTOR3 AddRot, bool bCollisionRelease, int nHitStopTime)
{
	CAttackPlayer* pAttackPlayer = DBG_NEW CAttackPlayer;   //敵を生成
	bool bSuccess = pAttackPlayer->CObject::GetCreateSuccess();
	int nIdx = 0;//モデルのインデックス
	if (bSuccess == true)
	{
		if (pAttackPlayer != nullptr)
		{
			pAttackPlayer->SetAttackType(type);  //攻撃の種類
			pAttackPlayer->Init();         //初期化処理
			pAttackPlayer->SetHitStopTime(nHitStopTime);          //当たったときに割り当てるヒットストップ時間を設定する
			pAttackPlayer->SetUseBlockCollision(false);
			pAttackPlayer->SetCollisionRelease(bCollisionRelease);//当たった時に消すかどうかを設定する
			pAttackPlayer->SetDoCollision(true);                  //当たり判定をするかどうかを設定
			pAttackPlayer->SetUseInteria(false);//慣性を使用しない
			pAttackPlayer->SetFormarScale(Scale);                                       //元の拡大率を設定する
			pAttackPlayer->SetMove(move);  //移動量
			pAttackPlayer->SetLife(nLife);                    //体力を設定
			pAttackPlayer->SetMaxLife(nLife);                 //最大体力を設定
			pAttackPlayer->SetAutoSubLife(true);              //自動的に体力を減らすかどうか
			pAttackPlayer->SetTargetType(CAttack::TARGETTYPE_ENEMY);//誰に当てるかを設定
			pAttackPlayer->SetCollisionType(CollisionType);   //当たり判定の種類
			pAttackPlayer->SetPower(nDamage);                 //威力
			pAttackPlayer->SetUseAddRot(true, AddRot);                 //加算する向きの量
			pAttackPlayer->CObjectX::SetPos(pos);             //オブジェクトXの位置を設定
			pAttackPlayer->SetRot(Rot);                       //向きを設定
			pAttackPlayer->SetUseDeath(true);                 //死亡フラグを発動するかどうかを設定する
			pAttackPlayer->SetPosOld(pos);                                                                                         //1f前の位置を設定
			pAttackPlayer->SetSupportPos(pos);                                                                                     //設置位置
			pAttackPlayer->CObject::SetType(CObject::TYPE_ATTACK);                                                                  //オブジェクトの種類を決める
			pAttackPlayer->CObjectX::SetScale(Scale);                                                                              //オブジェクトの拡大率
			nIdx = CManager::GetObjectXInfo()->Regist(CObjectX::OBJECTXTYPE_ATTACK, CAttack::m_ATTACK_FILENAME[(int)(type)]);

			pAttackPlayer->CObjectX::BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(CObjectX::OBJECTXTYPE_ATTACK, nIdx),
				CManager::GetObjectXInfo()->GetBuffMat(CObjectX::OBJECTXTYPE_ATTACK, nIdx),
				CManager::GetObjectXInfo()->GetdwNumMat(CObjectX::OBJECTXTYPE_ATTACK, nIdx),
				CManager::GetObjectXInfo()->GetTexture(CObjectX::OBJECTXTYPE_ATTACK, nIdx),
				CManager::GetObjectXInfo()->GetColorValue(CObjectX::OBJECTXTYPE_ATTACK, nIdx));                       //モデル情報を割り当てる

			pAttackPlayer->CObjectX::SetObjXType(CObjectX::OBJECTXTYPE_ATTACK);                            //オブジェクトXのタイプを設定
			pAttackPlayer->CObjectX::SetTypeNum((int)(type));                                             //オブジェクトXごとのタイプ番号を設定
			pAttackPlayer->SetSize();                                                                     //Xオブジェクトのサイズを設定する
			pAttackPlayer->CObjectX::SetRotType(CObjectX::ROTTYPE::ROTTYPE_NORMAL);                       //向きの種類を設定
		}
		else
		{//オブジェクトに空きがなかったので破棄する
			delete pAttackPlayer;
			pAttackPlayer = nullptr;
			return nullptr;
		}
	}
	return pAttackPlayer;
}
//===================================================================================================================================================

//=====================================================================
//つるの処理
//=====================================================================
void CAttackPlayer::VineProcess()
{
	int& nCntTime = GetCntTime();
	D3DXVECTOR3& Scale = GetScale();
	float fRatioLife = 0.0f;
	if (nCntTime <= 30)
	{
		Scale.y += 0.01f;
	}
	else
	{
		if (nCntTime <= 45)
		{
			Scale.y += 0.2f;
		}

	}

	if (m_nVineDelBulletNum <= 0 && m_bVineDelStart == false)
	{//消せる弾数が０になったら
		SetLife(10);//体力を１０に設定し、下の処理を発動
		m_bVineDelStart = true;//つるの消去開始
	}

	if (GetLife() == 10)
	{
		m_SaveScale = Scale;
	}

	if (GetLife() < 10)
	{//体力が１０より下になったら拡大率を下げていく
		fRatioLife = (float)(GetLife()) / 10;
		Scale.y = m_SaveScale.y * fRatioLife;
	}
}
//===================================================================================================================================================

//=====================================================================
//クロスボムの処理
//=====================================================================
void CAttackPlayer::CrossBombProcess()
{
	D3DXVECTOR3 Pos = GetPos();             //位置
	D3DXVECTOR3& Scale = GetScale();        //拡大率
	CAttackPlayer* pAttackPlayer = nullptr; //攻撃プレイヤー

	if (GetLife() % 20 == 0)
	{//演出発動
		m_bCrossBombStaging = true;
	}

	if (m_bCrossBombStaging == true)
	{//大きくして少し小さくなる挙動を繰り返す

		SetColor(CObjectX::COLORTYPE_RED, 2);

		m_nCrossBombStagingCnt++;
		if (m_nCrossBombStagingCnt < 8)
		{//拡大
			Scale += ONE_VECTOR3 * 0.05f;
		}
		else
		{//縮小
			Scale -= ONE_VECTOR3 * 0.05f;
		}

		if (m_nCrossBombStagingCnt >= 10)
		{//演出フラグをオフにする
			m_bCrossBombStaging = false;
			m_nCrossBombStagingCnt = 0;
		}
	}

	if (GetDelayDeath() == false)
	{
		pAttackPlayer = CAttackPlayer::Create(CAttack::ATTACKTYPE14_HELLFIRE, 70,100,NULL_VECTOR3, NULL_VECTOR3,D3DXVECTOR3(2.0f,8.0f,2.0f), NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY,
			D3DXVECTOR3(0.0f, 0.4f, 0.0f), false, 10);

		//オフセットを設定
		pAttackPlayer->SetParentInfo(Pos, D3DXVECTOR3(0.0f, 0.0f,D3DX_PI * 0.5f), ONE_VECTOR3,true);

		pAttackPlayer = CAttackPlayer::Create(CAttack::ATTACKTYPE14_HELLFIRE, 70,100, NULL_VECTOR3, NULL_VECTOR3, D3DXVECTOR3(2.0f, 8.0f, 2.0f), NULL_VECTOR3, CAttack::COLLISIONTYPE02_TRUERECTANGLEXY,
			D3DXVECTOR3(0.0f, 0.4f, 0.0f), false, 10);

		//オフセットを設定
		pAttackPlayer->SetParentInfo(Pos,NULL_VECTOR3, ONE_VECTOR3, true);

	}
}
//===================================================================================================================================================

//=====================================================================
//ブラックホールの処理
//=====================================================================
void CAttackPlayer::BlackHoleProcess()
{
	CObject* pObj = nullptr;
	CBullet* pBullet = nullptr;
	int& nCntTime = GetCntTime();//出現時間

	D3DXVECTOR3 MyPos = GetPos();            //攻撃の位置
	D3DXVECTOR3 MyVtxMax = GetVtxMax();      //攻撃の最大頂点
	D3DXVECTOR3 MyVtxMin = GetVtxMin();      //攻撃の最小頂点
	D3DXVECTOR3& MyScale = GetScale();       //攻撃の拡大率

	D3DXVECTOR3 ComparisonPos = NULL_VECTOR3;//比較用位置
	D3DXVECTOR3 ComparisonVtxMax = NULL_VECTOR3;//比較用最大頂点
	D3DXVECTOR3 ComparisonVtxMin = NULL_VECTOR3;//比較用最小頂点

	for (int nCntPriority = 0; nCntPriority < m_nMAXPRIORITY; nCntPriority++)
	{
		pObj = GetTopObject(nCntPriority);//トップオブジェクトを取得
		while (pObj != nullptr)
		{
			//次のオブジェクトを格納
			CObject* pNext = pObj->GetNextObject();

			//種類の取得（敵なら当たり判定）
			CObject::TYPE type = pObj->GetType();

			if (type == CObject::TYPE_BULLET)
			{
				pBullet = static_cast<CBullet*>(pObj);//バレットにキャスト
				ComparisonPos = pBullet->GetPos();
				ComparisonVtxMax = D3DXVECTOR3(pBullet->GetWidth(), pBullet->GetHeight(), pBullet->GetWidth());
				ComparisonVtxMin = D3DXVECTOR3(-pBullet->GetWidth(), -pBullet->GetHeight(), -pBullet->GetWidth());
			}
			else
			{
				pObj = pNext;//リストを次に進める
				continue;
			}
			pBullet->SetPos(ComparisonPos + CCalculation::Calculation3DVec(ComparisonPos, GetPos(), 20.0f));//弾を強制的にブラックホールに向けて移動させる
			if (MyPos.x + MyVtxMin.x <= ComparisonPos.x + ComparisonVtxMax.x &&
				MyPos.x + MyVtxMax.x >= ComparisonPos.x + ComparisonVtxMin.x &&
				MyPos.y + MyVtxMin.y <= ComparisonPos.y + ComparisonVtxMax.y &&
				MyPos.y + MyVtxMax.y >= ComparisonPos.y + ComparisonVtxMin.y &&
				MyPos.z + MyVtxMin.z <= ComparisonPos.z + ComparisonVtxMax.x &&
				MyPos.z + MyVtxMax.z >= ComparisonPos.z + ComparisonVtxMin.x)
			{//ワープマーカーに当たったら
				pBullet->SetUseDeath(true);
				pBullet->SetDeath();
				SetScale(GetScale() + ONE_VECTOR3 * 0.01f);//ブラックホールの拡大率を上げる
				m_nBlackHoleDefeatCnt++;//ブラックホールで吸い込んだ弾の数をインクリメント
			}
			//リストを次に進める
			pObj = pNext;
		}
	}

	if (GetLife() == 1)
	{
		for (int nCntDefeat = 0; nCntDefeat < m_nBlackHoleDefeatCnt; nCntDefeat++)
		{//弾を吸い込むほど派手なパーティクルになる
			CParticle::SummonParticle(CParticle::TYPE03_THUNDER,1,100, 40.0f, 40.0f, 200, 10, true, MyPos, NORMAL_COL, true);
		}	
		//弾を吸い込むほど威力アップ
	    CAttackPlayer* pAttackPlayer = CAttackPlayer::Create(CAttack::ATTACKTYPE11_EXPLOSION, m_nBlackHoleDefeatCnt * 2,m_nBlackHoleDefeatCnt, GetPos(), NULL_VECTOR3, ONE_VECTOR3,
				NULL_VECTOR3, CAttack::COLLISIONTYPE00_SQUARE, ONE_VECTOR3 * 0.1f, false, 20);//爆発発射
		pAttackPlayer->SetUseAddScale(ONE_VECTOR3 * 0.01f * static_cast<float>(m_nBlackHoleDefeatCnt), true);//吸い込むほど拡大速度上昇
		pAttackPlayer->SetUseAttackEffect(true, 120, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));//攻撃に軌跡をつける
	}


}
//===================================================================================================================================================

//=====================================================================
//サイコバーストの処理
//=====================================================================
void CAttackPlayer::PhycoBurstProcess()
{
	int nLife = GetLife();            //体力を取得
	int nMaxLife = GetMaxLife();      //最大体力を取得
	int nDiffrenceLife = nMaxLife - nLife;//最大体力との差分を求める
	float fRatio = 0.0f;              //割合
	CEmitter* pEmitter = nullptr;     //エミッターへのポインタ
	D3DXVECTOR3 Pos = GetPos();       //位置を取得
	D3DXVECTOR3 Move = NULL_VECTOR3;  //移動量
	D3DXVECTOR3& Scale = GetScale();   //拡大率を取得
	D3DXVECTOR3& Rot = GetRot();      //向きを取得
	D3DXCOLOR col = {};               //色合い
	D3DXVECTOR3& AddRot = GetAddRot();//向きの加算量を取得
	//オブジェクトの色合いを薄くしていく
	fRatio = (float)(nLife) / (float)(nMaxLife);
	SetColorAlphaOnly(fRatio);

	//拡大率を乗算する
	float fScale = 0.012f * (float)(nDiffrenceLife);
	Scale += ONE_VECTOR3 * fScale;

	AddRot += AddRot * 0.1f;

	if (nDiffrenceLife == 1)
	{//最初にエミッターを召喚

		for (int nCnt = 0; nCnt < 10; nCnt++)
		{
			//飛ぶ方向を決める
			Move.x = float(rand() % 100 - 50) / 100;
			Move.y = float(rand() % 100 - 50) / 100;
			Move.z = float(rand() % 100 - 50) / 100;

			//召喚オブジェクトの色合い
			col.r = float(rand() % 100) / 100;
			col.g = float(rand() % 100) / 100;
			col.b = float(rand() % 100) / 100;
			col.a = 1.0f;

			pEmitter = CEmitter::Create(CEffect::EFFECTTYPE00_NORMAL, nMaxLife - (nMaxLife - nLife), 20, 0.0f, 40.0f, 40.0f,NULL_VECTOR3, Move, col);
			pEmitter->SetUseMtxChild(&GetMatrixWorld(),true);
		}
	}

	if (nLife < 2)
	{
		SetSummonObjectDeath(true);//召喚したオブジェクトを消すフラグを発動
	}
}
//===================================================================================================================================================

//===================================================================<<<エネミー攻撃クラス>>>===========================================================

//=====================================================================
//コンストラクタ
//=====================================================================
CAttackEnemy::CAttackEnemy()
{

}
//===================================================================================================================================================

//=====================================================================
//デストラクタ
//=====================================================================
CAttackEnemy::~CAttackEnemy()
{

}
//===================================================================================================================================================

//=====================================================================
//初期化処理
//=====================================================================
HRESULT CAttackEnemy::Init()
{
	CAttack::Init();//攻撃の初期化処理
	return S_OK;
}
//===================================================================================================================================================

//=====================================================================
//終了処理
//=====================================================================
void CAttackEnemy::Uninit()
{
	CAttack::Uninit();//攻撃の終了処理
}
//===================================================================================================================================================

//=====================================================================
//更新処理
//=====================================================================
void CAttackEnemy::Update()
{

	D3DXVECTOR3& PosOld = GetPosOld();                             //1f前の位置を設定
	D3DXVECTOR3 Size = GetSize();                                 //サイズを取得
	D3DXVECTOR3& Scale = GetScale();                               //拡大率を取得
	D3DXVECTOR3& Pos = CObjectX::GetPos();                         //位置を取得
	D3DXVECTOR3& Rot = GetRot();                                   //向きを取得
	CParticle* pParticle = nullptr;                                //パーティクルへのポインタ
	float fRandRot = 0.0f;                                         //乱数向き
	switch (GetAttackType())
	{
	case ATTACKTYPE02_NEEDLE:
		break;
	case ATTACKTYPE05_ICE:
		if (Scale.x <= 4.0f)
		{
			Scale += (ONE_VECTOR3 * 0.1f);
		}
		break;
	case ATTACKTYPE10_SHOCKWAVE:
		fRandRot = float(rand() % 628) / 100;
		pParticle = CParticle::Create(CParticle::TYPE00_NORMAL, 45, 20.0f, 20.0f, Pos,
			D3DXVECTOR3(sinf(fRandRot) * 10.0f, 10.0f, cosf(fRandRot) * 10.0f), NORMAL_COL, true);

		pParticle->SetUseGravity(-0.8f);
		break;
	}

	CAttack::Update();//攻撃の更新処理
}
//===================================================================================================================================================

//=====================================================================
//描画処理
//=====================================================================
void CAttackEnemy::Draw()
{
	CAttack::Draw();//攻撃の描画処理
}
//===================================================================================================================================================

//=====================================================================
//死亡フラグ設定処理
//=====================================================================
void CAttackEnemy::SetDeath()
{
	CAttack::SetDeath();
}
//===================================================================================================================================================

//=====================================================================
//生成処理
//=====================================================================
CAttackEnemy* CAttackEnemy::Create(ATTACKTYPE type, int nDamage, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 Scale, D3DXVECTOR3 Rot,
	COLLISIONTYPE CollisionType, D3DXVECTOR3 AddRot, bool bCollisionRelease, int nHitStopTime)
{
	CAttackEnemy* pAttackEnemy = DBG_NEW CAttackEnemy;   //敵を生成
	bool bSuccess = pAttackEnemy->CObject::GetCreateSuccess();
	int nIdx = 0;//モデルのインデックス
	if (bSuccess == true)
	{
		if (pAttackEnemy != nullptr)
		{
			pAttackEnemy->SetAttackType(type);  //攻撃の種類
			pAttackEnemy->Init();         //初期化処理
			pAttackEnemy->SetHitStopTime(nHitStopTime);          //当たったときに割り当てるヒットストップ時間を設定する
			pAttackEnemy->SetUseBlockCollision(false);
			pAttackEnemy->SetCollisionRelease(bCollisionRelease);//当たった時に消すかどうかを設定する
			pAttackEnemy->SetUseInteria(false);//慣性を使用しない
			pAttackEnemy->SetMove(move);  //移動量
			pAttackEnemy->SetDoCollision(true);                  //当たり判定をするかどうかを設定
			pAttackEnemy->SetLife(nLife);                    //体力を設定
			pAttackEnemy->SetMaxLife(nLife);                 //最大体力を設定
			pAttackEnemy->SetAutoSubLife(true);              //自動的に体力を減らすかどうか
			pAttackEnemy->SetTargetType(CAttack::TARGETTYPE_PLAYER);//誰に当てるかを設定
			pAttackEnemy->SetCollisionType(CollisionType);   //当たり判定の種類
			pAttackEnemy->SetPower(nDamage);                 //威力
			pAttackEnemy->SetUseAddRot(true,AddRot);                 //加算する向きの量
			pAttackEnemy->CObjectX::SetPos(pos);             //オブジェクトXの位置を設定
			pAttackEnemy->SetRot(Rot);                       //向きを設定
			pAttackEnemy->SetPosOld(pos);                                                                                         //1f前の位置を設定
			pAttackEnemy->SetSupportPos(pos);                                                                                     //設置位置
			pAttackEnemy->CObject::SetType(CObject::TYPE_ATTACK);                                                                  //オブジェクトの種類を決める
			pAttackEnemy->CObjectX::SetScale(Scale);                                                                              //オブジェクトの拡大率
			pAttackEnemy->SetFormarScale(Scale);                                       //元の拡大率を設定する
			pAttackEnemy->SetUseDeath(true);                 //死亡フラグを発動するかどうかを設定する
			nIdx = CManager::GetObjectXInfo()->Regist(CObjectX::OBJECTXTYPE_ATTACK, CAttack::m_ATTACK_FILENAME[(int)(type)]);
			pAttackEnemy->CObjectX::BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(CObjectX::OBJECTXTYPE_ATTACK, nIdx),
				CManager::GetObjectXInfo()->GetBuffMat(CObjectX::OBJECTXTYPE_ATTACK, nIdx),
				CManager::GetObjectXInfo()->GetdwNumMat(CObjectX::OBJECTXTYPE_ATTACK, nIdx),
				CManager::GetObjectXInfo()->GetTexture(CObjectX::OBJECTXTYPE_ATTACK, nIdx),
				CManager::GetObjectXInfo()->GetColorValue(CObjectX::OBJECTXTYPE_ATTACK, nIdx));                       //モデル情報を割り当てる

			pAttackEnemy->CObjectX::SetObjXType(CObjectX::OBJECTXTYPE_ATTACK);                            //オブジェクトXのタイプを設定
			pAttackEnemy->CObjectX::SetTypeNum((int)(type));                                             //オブジェクトXごとのタイプ番号を設定
			pAttackEnemy->SetSize();                                                                     //Xオブジェクトのサイズを設定する
			pAttackEnemy->CObjectX::SetRotType(CObjectX::ROTTYPE::ROTTYPE_NORMAL);                       //向きの種類を設定
		}
		else
		{//オブジェクトに空きがなかったので破棄する
			delete pAttackEnemy;
			pAttackEnemy = nullptr;
			return nullptr;
		}
	}
	return pAttackEnemy;
}
//===================================================================================================================================================

//================================================================<<攻撃エフェクトクラス>>==================================================================

//==============================================================================
//コンストラクタ
//==============================================================================
CAttackEffect::CAttackEffect()
{

}
//===================================================================================================================================================

//==============================================================================
//デストラクタ
//==============================================================================
CAttackEffect::~CAttackEffect()
{

}
//===================================================================================================================================================

//==============================================================================
//初期化処理
//==============================================================================
HRESULT CAttackEffect::Init()
{
	m_MaxScale = NULL_VECTOR3;//拡大率の最大値
	m_Col = {};               //色合い
	CAttack::Init();          //攻撃の初期化処理
	return S_OK;
}
//===================================================================================================================================================

//==============================================================================
//終了処理
//==============================================================================
void CAttackEffect::Uninit()
{
	CAttack::Uninit();//攻撃の終了処理
}
//===================================================================================================================================================

//==============================================================================
//更新処理
//==============================================================================
void CAttackEffect::Update()
{
	float fRatioLife = (float)(GetLife()) / (float)(GetMaxLife());//残り体力の割合を求める
	m_Col.a = fRatioLife;//アルファ値を設定
	//D3DXVECTOR3 Scale = NULL_VECTOR3;
	//Scale = m_MaxScale * fRatioLife;
	SetOriginalColor(m_Col, 2);//色合いを設定する
	CAttack::Update();//攻撃の更新処理
}
//===================================================================================================================================================

//==============================================================================
//描画処理
//==============================================================================
void CAttackEffect::Draw()
{
	CAttack::Draw();//攻撃の描画処理
}
//===================================================================================================================================================

//=====================================================================
//死亡フラグ設定処理
//=====================================================================
void CAttackEffect::SetDeath()
{
	CAttack::SetDeath();
}
//===================================================================================================================================================

//==============================================================================
//生成処理
//==============================================================================
CAttackEffect* CAttackEffect::Create(ATTACKTYPE type, int nDamage, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 Scale, D3DXVECTOR3 Rot, D3DXVECTOR3 AddRot, D3DXCOLOR col)
{
	CAttackEffect* pAttackEffect = DBG_NEW CAttackEffect;   //敵を生成
	bool bSuccess = pAttackEffect->CObject::GetCreateSuccess();
	int nIdx = 0;//モデルのインデックス
	if (bSuccess == true)
	{
		if (pAttackEffect != nullptr)
		{
			pAttackEffect->SetAttackType(type);  //攻撃の種類
			pAttackEffect->SetUseInitialSound(false);//初期化時にサウンドを再生するかどうか
			pAttackEffect->Init();         //初期化処理
			pAttackEffect->SetUseShadow(false);//影を使用しない
			pAttackEffect->SetUseBlockCollision(false);
			pAttackEffect->SetUseInteria(false);//慣性を使用しない
			pAttackEffect->m_MaxScale = Scale;//拡大率の最大値
			pAttackEffect->m_Col = col;
			pAttackEffect->SetOriginalColor(col, 2);          //色合いを設定     
			pAttackEffect->SetMove(move);   //移動量
			pAttackEffect->SetDoCollision(false);                  //当たり判定をするかどうかを設定
			pAttackEffect->SetLife(nLife);                    //体力を設定
			pAttackEffect->SetMaxLife(nLife);                 //最大体力を設定
			pAttackEffect->SetAutoSubLife(true);              //自動的に体力を減らすかどうか
			pAttackEffect->SetTargetType(CAttack::TARGETTYPE_PLAYER);//誰に当てるかを設定
			pAttackEffect->SetPower(nDamage);                 //威力
			pAttackEffect->SetUseAddRot(true, AddRot);                 //加算する向きの量
			pAttackEffect->CObjectX::SetPos(pos);             //オブジェクトXの位置を設定
			pAttackEffect->SetRot(Rot);                       //向きを設定
			pAttackEffect->SetPosOld(pos);                                                                                         //1f前の位置を設定
			pAttackEffect->SetSupportPos(pos);                                                                                     //設置位置
			pAttackEffect->CObject::SetType(CObject::TYPE_ATTACK);                                                                  //オブジェクトの種類を決める
			pAttackEffect->CObjectX::SetScale(Scale);                                                                              //オブジェクトの拡大率
			pAttackEffect->SetFormarScale(Scale);                                       //元の拡大率を設定する
			pAttackEffect->SetUseDeath(true);                 //死亡フラグを発動するかどうかを設定する
			nIdx = CManager::GetObjectXInfo()->Regist(CObjectX::OBJECTXTYPE_ATTACK, CAttack::m_ATTACK_FILENAME[(int)(type)]);
			pAttackEffect->CObjectX::BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(CObjectX::OBJECTXTYPE_ATTACK, nIdx),
				CManager::GetObjectXInfo()->GetBuffMat(CObjectX::OBJECTXTYPE_ATTACK, nIdx),
				CManager::GetObjectXInfo()->GetdwNumMat(CObjectX::OBJECTXTYPE_ATTACK, nIdx),
				CManager::GetObjectXInfo()->GetTexture(CObjectX::OBJECTXTYPE_ATTACK, nIdx),
				CManager::GetObjectXInfo()->GetColorValue(CObjectX::OBJECTXTYPE_ATTACK, nIdx));                       //モデル情報を割り当てる

			pAttackEffect->CObjectX::SetObjXType(CObjectX::OBJECTXTYPE_ATTACK);                            //オブジェクトXのタイプを設定
			pAttackEffect->CObjectX::SetTypeNum((int)(type));                                             //オブジェクトXごとのタイプ番号を設定
			pAttackEffect->SetSize();                                                                     //Xオブジェクトのサイズを設定する
			pAttackEffect->CObjectX::SetRotType(CObjectX::ROTTYPE::ROTTYPE_NORMAL);                       //向きの種類を設定
		}
		else
		{//オブジェクトに空きがなかったので破棄する
			delete pAttackEffect;
			pAttackEffect = nullptr;
			return nullptr;
		}
	}
	return pAttackEffect;
}
//===================================================================================================================================================

//==============================================================================
//コンストラクタ
//==============================================================================
CMrStrongAttack::CMrStrongAttack()
{

}
//===================================================================================================================================================

//==============================================================================
//デストラクタ
//==============================================================================
CMrStrongAttack::~CMrStrongAttack()
{

}
//===================================================================================================================================================

//==============================================================================
//初期化処理
//==============================================================================
HRESULT CMrStrongAttack::Init()
{
	CAttack::Init();
	return S_OK;
}
//===================================================================================================================================================

//==============================================================================
//終了処理
//==============================================================================
void CMrStrongAttack::Uninit()
{
	CAttack::Uninit();
}
//===================================================================================================================================================

//==============================================================================
//更新処理
//==============================================================================
void CMrStrongAttack::Update()
{
	switch (GetAttackType())
	{
	case ATTACKTYPE17_BLOCK:
		BlockProcess();
		break;
	default:
		break;
	}

	CAttack::Update();
}
//===================================================================================================================================================

//==============================================================================
//描画処理
//==============================================================================
void CMrStrongAttack::Draw()
{
	CAttack::Draw();
}
//===================================================================================================================================================

//==============================================================================
//描画処理
//==============================================================================
void CMrStrongAttack::SetDeath()
{
	CAttack::SetDeath();
}
//===================================================================================================================================================

//==============================================================================
//生成処理
//==============================================================================
CMrStrongAttack* CMrStrongAttack::Create(ATTACKTYPE type, int nDamage, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 Scale, D3DXVECTOR3 Rot,
	COLLISIONTYPE CollisionType, D3DXVECTOR3 AddRot, bool bCollisionRelease, int nHitStopTime)
{
	CMrStrongAttack* pMrStrongAttack = DBG_NEW CMrStrongAttack;   //敵を生成
	bool bSuccess = pMrStrongAttack->CObject::GetCreateSuccess();
	int nIdx = 0;//モデルのインデックス
	if (bSuccess == true)
	{
		if (pMrStrongAttack != nullptr)
		{
			pMrStrongAttack->SetAttackType(type);  //攻撃の種類
			pMrStrongAttack->Init();         //初期化処理
			pMrStrongAttack->SetHitStopTime(nHitStopTime);          //当たったときに割り当てるヒットストップ時間を設定する
			pMrStrongAttack->SetUseBlockCollision(false);
			pMrStrongAttack->SetCollisionRelease(bCollisionRelease);//当たった時に消すかどうかを設定する
			pMrStrongAttack->SetUseInteria(false);//慣性を使用しない
			pMrStrongAttack->SetMove(move);  //移動量
			pMrStrongAttack->SetDoCollision(true);                  //当たり判定をするかどうかを設定
			pMrStrongAttack->SetLife(nLife);                    //体力を設定
			pMrStrongAttack->SetMaxLife(nLife);                 //最大体力を設定
			pMrStrongAttack->SetAutoSubLife(true);              //自動的に体力を減らすかどうか
			pMrStrongAttack->SetTargetType(CAttack::TARGETTYPE_PLAYER);//誰に当てるかを設定
			pMrStrongAttack->SetCollisionType(CollisionType);   //当たり判定の種類
			pMrStrongAttack->SetPower(nDamage);                 //威力
			pMrStrongAttack->SetUseAddRot(true, AddRot);                 //加算する向きの量
			pMrStrongAttack->CObjectX::SetPos(pos);             //オブジェクトXの位置を設定
			pMrStrongAttack->SetRot(Rot);                       //向きを設定
			pMrStrongAttack->SetPosOld(pos);                                                                                         //1f前の位置を設定
			pMrStrongAttack->SetSupportPos(pos);                                                                                     //設置位置
			pMrStrongAttack->CObject::SetType(CObject::TYPE_ATTACK);                                                                  //オブジェクトの種類を決める
			pMrStrongAttack->CObjectX::SetScale(Scale);                                                                              //オブジェクトの拡大率
			pMrStrongAttack->SetFormarScale(Scale);                                       //元の拡大率を設定する
			pMrStrongAttack->SetUseDeath(true);                 //死亡フラグを発動するかどうかを設定する
			nIdx = CManager::GetObjectXInfo()->Regist(CObjectX::OBJECTXTYPE_ATTACK, CAttack::m_ATTACK_FILENAME[(int)(type)]);
			pMrStrongAttack->CObjectX::BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(CObjectX::OBJECTXTYPE_ATTACK, nIdx),
				CManager::GetObjectXInfo()->GetBuffMat(CObjectX::OBJECTXTYPE_ATTACK, nIdx),
				CManager::GetObjectXInfo()->GetdwNumMat(CObjectX::OBJECTXTYPE_ATTACK, nIdx),
				CManager::GetObjectXInfo()->GetTexture(CObjectX::OBJECTXTYPE_ATTACK, nIdx),
				CManager::GetObjectXInfo()->GetColorValue(CObjectX::OBJECTXTYPE_ATTACK, nIdx));                       //モデル情報を割り当てる

			pMrStrongAttack->CObjectX::SetObjXType(CObjectX::OBJECTXTYPE_ATTACK);                            //オブジェクトXのタイプを設定
			pMrStrongAttack->CObjectX::SetTypeNum((int)(type));                                             //オブジェクトXごとのタイプ番号を設定
			pMrStrongAttack->SetSize();                                                                     //Xオブジェクトのサイズを設定する
			pMrStrongAttack->CObjectX::SetRotType(CObjectX::ROTTYPE::ROTTYPE_NORMAL);                       //向きの種類を設定
		}
		else
		{//オブジェクトに空きがなかったので破棄する
			delete pMrStrongAttack;
			pMrStrongAttack = nullptr;
			return nullptr;
		}
	}
	return pMrStrongAttack;
}
//===================================================================================================================================================

//==============================================================================
//ブロック攻撃の処理
//==============================================================================
void CMrStrongAttack::BlockProcess()
{
	float fVXaim = 0.0f;
	float fVYaim = 0.0f;
	float fVLaim = 0.0f;
	D3DXVECTOR3& Pos = GetPos();
	D3DXVECTOR3& Move = GetMove();
	D3DXVECTOR3 PurposePos = GetPurposePos();
	int& nPattern = GetPattern();//パターン
	int& nPatternTime = GetPatternTime();//パターン時間
	switch (nPattern)
	{
	case 0:
		fVXaim = PurposePos.x - Pos.x;
		fVYaim = PurposePos.y - Pos.y;
		fVLaim = atan2f(fVXaim, fVYaim);

		Move = D3DXVECTOR3(sinf(fVLaim) * 10.0f, cosf(fVLaim) * 10.0f, 0.0f);

		if (Pos.x > PurposePos.x - 10.0f && Pos.x < PurposePos.x + 10.0f &&
			Pos.y > PurposePos.y - 10.0f && Pos.y < PurposePos.y + 10.0f)
		{
			Pos = PurposePos;
			Move = NULL_VECTOR3;
			nPattern++;
			nPatternTime = 0;
		}
		break;
	case 1:
		if (nPatternTime == 60)
		{
			nPattern++;
			nPatternTime = 0;
		}
		break;
	case 2:
		if (nPatternTime == 1)
		{
			Move.y = 3.0f;
		}
		
		if (nPatternTime == 2)
		{//加速させる
			SetUseMultiSpeed(true, D3DXVECTOR3(1.0f, 1.03f, 1.0f));
			nPattern++;
			nPatternTime = 0;
		}
		break;
	case 3:
		if (Pos.y >= 1000.0f)
		{
			Pos.z = 0.0f;
			SetUseMultiSpeed(false, NULL_VECTOR3);
			//SetUseAddSpeed(true, D3DXVECTOR3(0.0f, 0.01f, 0.0f));
			Move.y = -20.0f;

			nPattern++;
			nPatternTime = 0;
		}
		break;
	case 4:
		CAttackEffect::Create(CAttack::ATTACKTYPE17_BLOCK, 0, 20, Pos, NULL_VECTOR3, GetScale(), GetRot(), NULL_VECTOR3, D3DXCOLOR(1.0f, 0.0f, 1.0f,1.0f));
		break;
	default:
		break;
	}	
}
//===================================================================================================================================================

//==============================================================================
//コンストラクタ
//==============================================================================
CRileyAttack::CRileyAttack()
{

}
//===================================================================================================================================================

//==============================================================================
//デストラクタ
//==============================================================================
CRileyAttack::~CRileyAttack()
{

}
//===================================================================================================================================================

//==============================================================================
//初期化処理
//==============================================================================
HRESULT CRileyAttack::Init()
{
	CAttack::Init();
	m_AttackMove = ATTACKMOVE_RILEY_00_NONE;
	return S_OK;
}
//===================================================================================================================================================

//==============================================================================
//終了処理
//==============================================================================
void CRileyAttack::Uninit()
{
	CAttack::Uninit();
}
//===================================================================================================================================================

//==============================================================================
//更新処理
//==============================================================================
void CRileyAttack::Update()
{
	switch (m_AttackMove)
	{
	case ATTACKMOVE_RILEY_01_ACTION03:
		Action03_Process();
		break;
	default:
		break;
	}

	CAttack::Update();
}
//===================================================================================================================================================

//==============================================================================
//描画処理
//==============================================================================
void CRileyAttack::Draw()
{
	CAttack::Draw();
}
//===================================================================================================================================================

//==============================================================================
//生成処理
//==============================================================================
CRileyAttack* CRileyAttack::Create(ATTACKTYPE type, int nDamage, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 Scale, D3DXVECTOR3 Rot,
	COLLISIONTYPE CollisionType, D3DXVECTOR3 AddRot, bool bCollisionRelease, int nHitStopTime)
{
	CRileyAttack* pRileyAttack = DBG_NEW CRileyAttack;   //敵を生成
	bool bSuccess = pRileyAttack->CObject::GetCreateSuccess();
	int nIdx = 0;//モデルのインデックス
	if (bSuccess == true)
	{
		if (pRileyAttack != nullptr)
		{
			pRileyAttack->SetAttackType(type);  //攻撃の種類
			pRileyAttack->Init();         //初期化処理
			pRileyAttack->SetUseBlockCollision(false);
			pRileyAttack->SetHitStopTime(nHitStopTime);          //当たったときに割り当てるヒットストップ時間を設定する
			pRileyAttack->SetCollisionRelease(bCollisionRelease);//当たった時に消すかどうかを設定する
			pRileyAttack->SetUseInteria(false);//慣性を使用しない
			pRileyAttack->SetMove(move);  //移動量
			pRileyAttack->SetDoCollision(true);                  //当たり判定をするかどうかを設定
			pRileyAttack->SetLife(nLife);                    //体力を設定
			pRileyAttack->SetMaxLife(nLife);                 //最大体力を設定
			pRileyAttack->SetAutoSubLife(true);              //自動的に体力を減らすかどうか
			pRileyAttack->SetTargetType(CAttack::TARGETTYPE_PLAYER);//誰に当てるかを設定
			pRileyAttack->SetCollisionType(CollisionType);   //当たり判定の種類
			pRileyAttack->SetPower(nDamage);                 //威力
			pRileyAttack->SetUseAddRot(true, AddRot);                 //加算する向きの量
			pRileyAttack->CObjectX::SetPos(pos);             //オブジェクトXの位置を設定
			pRileyAttack->SetRot(Rot);                       //向きを設定
			pRileyAttack->SetPosOld(pos);                                                                                         //1f前の位置を設定
			pRileyAttack->SetSupportPos(pos);                                                                                     //設置位置
			pRileyAttack->CObject::SetType(CObject::TYPE_ATTACK);                                                                  //オブジェクトの種類を決める
			pRileyAttack->CObjectX::SetScale(Scale);                                                                              //オブジェクトの拡大率
			pRileyAttack->SetFormarScale(Scale);                                       //元の拡大率を設定する
			pRileyAttack->SetUseDeath(true);                 //死亡フラグを発動するかどうかを設定する
			nIdx = CManager::GetObjectXInfo()->Regist(CObjectX::OBJECTXTYPE_ATTACK, CAttack::m_ATTACK_FILENAME[(int)(type)]);
			pRileyAttack->CObjectX::BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(CObjectX::OBJECTXTYPE_ATTACK, nIdx),
				CManager::GetObjectXInfo()->GetBuffMat(CObjectX::OBJECTXTYPE_ATTACK, nIdx),
				CManager::GetObjectXInfo()->GetdwNumMat(CObjectX::OBJECTXTYPE_ATTACK, nIdx),
				CManager::GetObjectXInfo()->GetTexture(CObjectX::OBJECTXTYPE_ATTACK, nIdx),
				CManager::GetObjectXInfo()->GetColorValue(CObjectX::OBJECTXTYPE_ATTACK, nIdx));                       //モデル情報を割り当てる

			pRileyAttack->CObjectX::SetObjXType(CObjectX::OBJECTXTYPE_ATTACK);                            //オブジェクトXのタイプを設定
			pRileyAttack->CObjectX::SetTypeNum((int)(type));                                             //オブジェクトXごとのタイプ番号を設定
			pRileyAttack->SetSize();                                                                     //Xオブジェクトのサイズを設定する
			pRileyAttack->CObjectX::SetRotType(CObjectX::ROTTYPE::ROTTYPE_NORMAL);                       //向きの種類を設定
		}
		else
		{//オブジェクトに空きがなかったので破棄する
			delete pRileyAttack;
			pRileyAttack = nullptr;
			return nullptr;
		}
	}
	return pRileyAttack;
}
//===================================================================================================================================================

//==============================================================================
//生成処理
//==============================================================================
void CRileyAttack::Action03_ExplosionProcess()
{
	int nLife = GetLife();
	int nMaxLife = GetMaxLife();
	float fRatioLife = 1.0f - float(nLife) / float(nMaxLife);
	D3DXVECTOR3& Move = GetMove();
	bool& bPatternFlag = GetPatternFlag();

	int& nPattern = GetPattern();
	int& nPatternTime = GetPatternTime();
	float fRandRot = 0.0f;//角度計算用
	switch (GetSubType())
	{
	case 0:
		SetOriginalColor(D3DXCOLOR(fRatioLife, 0.0f, 0.0f, 1.0f), 2);
		switch (nPattern)
		{
		case 0:
			if (fRatioLife > 0.5f)
			{
				CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 40, 45, 3.0f, 40.0f, 40.0f, 150, 100, 20, true, GetSenterPos(), NORMAL_COL, true);
				nPattern++;
				nPatternTime = 0;
			}
		case 1:
			if (fRatioLife > 0.7f)
			{
				nPattern++;
				nPatternTime = 0;
				SetUseEffect(CEffect::EFFECTTYPE00_NORMAL, true, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 30, D3DXVECTOR2(50.0f, 50.0f));
			}
			break;
		case 2:
			fRandRot = CCalculation::CalculationXYaim(GetSenterPos(), CGame::GetPlayer()->GetSenterPos());
			Move.x = sinf(fRandRot) * 25.0f;
			Move.y = cosf(fRandRot) * 25.0f;
			nPattern++;
			nPatternTime = 0;
			break;
		default:
			break;
		}
		break;
	case 1:
		SetOriginalColor(D3DXCOLOR(0.0f, fRatioLife, 0.0f, 1.0f), 2);
		switch (nPattern)
		{
		case 0:
			if (fRatioLife > 0.5f)
			{
				CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 40, 45, 3.0f, 40.0f, 40.0f, 150, 100, 20, true, GetSenterPos(), NORMAL_COL, true);
				nPattern++;
				nPatternTime = 0;
			}
		case 1:
			if (fRatioLife > 0.7f)
			{//色が濃くなってきたら
				nPattern++;
				nPatternTime = 0;
				SetUseEffect(CEffect::EFFECTTYPE00_NORMAL, true, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), 30, D3DXVECTOR2(50.0f, 50.0f));
			}
			break;
		case 2:
			if (GetSenterPos().x < CGame::GetPlayer()->GetSenterPos().x)
			{
				Move.x = 6.0f;
			}
			else
			{
				Move.x = -6.0f;
			}
			Move.y = -8.0f;
			SetUseReflection(true);
			nPattern++;
			nPatternTime = 0;
			break;
		default:
			break;
		}
		break;
	case 2:
		SetOriginalColor(D3DXCOLOR(0.0f, 0.0f, fRatioLife, 1.0f), 2);
		switch (nPattern)
		{
		case 0:
			if (fRatioLife > 0.5f)
			{
				CParticle::SummonChargeParticle(CParticle::TYPE00_NORMAL, 40, 45, 3.0f, 40.0f, 40.0f, 150, 100, 20, true, GetSenterPos(), NORMAL_COL, true);
				nPattern++;
				nPatternTime = 0;
			}
		case 1:
			if (fRatioLife > 0.7f)
			{//色が濃くなってきたら
				nPattern++;
				nPatternTime = 0;
			}
			break;
		case 2:
			for (int nCnt = 0; nCnt < 3; nCnt++)
			{
				fRandRot = CCalculation::CalculationRandVecXY();
				CBullet::Create(CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTEX_ENEMY, 140, 10, 30.0f, 30.0f, GetSenterPos(),
					D3DXVECTOR3(sinf(fRandRot) * 4.0f, cosf(fRandRot) * 4.0f, 0.0f));
			}
			nPattern++;
			nPatternTime = 0;
			SetUseDeath(true);
			SetDeath();
			break;
		default:
			break;
		}
		break;
		break;
	default:
		break;
	}
}
//===================================================================================================================================================

//==============================================================================
//描画処理
//==============================================================================
void CRileyAttack::SetDeath()
{
	CAttack::SetDeath();
}
//===================================================================================================================================================

//==============================================================================
//行動３の処理
//==============================================================================
void CRileyAttack::Action03_Process()
{
	switch (GetAttackType())
	{
	case ATTACKTYPE11_EXPLOSION:
		Action03_ExplosionProcess();
		break;
	default:
		break;
	}
}
//===================================================================================================================================================


//==========================================================＜＜＜ドライヤ攻撃クラス＞＞＞=====================================================================================================================

//==============================================================================
//コンストラクタ
//==============================================================================
CDraiyaAttack::CDraiyaAttack()
{

}
//===================================================================================================================================================

//==============================================================================
//デストラクタ
//==============================================================================
CDraiyaAttack::~CDraiyaAttack()
{

}
//===================================================================================================================================================

//==============================================================================
//初期化処理
//==============================================================================
HRESULT CDraiyaAttack::Init()
{
	CAttack::Init();
	return S_OK;
}
//===================================================================================================================================================

//==============================================================================
//終了処理
//==============================================================================
void CDraiyaAttack::Uninit()
{
	CAttack::Uninit();
}
//===================================================================================================================================================

//==============================================================================
//更新処理
//==============================================================================
void CDraiyaAttack::Update()
{
	CAttack::Update();
}
//===================================================================================================================================================

//==============================================================================
//描画処理
//==============================================================================
void CDraiyaAttack::Draw()
{
	CAttack::Draw();
}
//===================================================================================================================================================

//==============================================================================
//死亡フラグ設定処理
//==============================================================================
void CDraiyaAttack::SetDeath()
{
	CAttack::SetDeath();
}
//===================================================================================================================================================

//==============================================================================
//生成処理
//==============================================================================
CDraiyaAttack* CDraiyaAttack::Create(ATTACKTYPE type, int nDamage, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 Scale, D3DXVECTOR3 Rot, COLLISIONTYPE CollisionType, D3DXVECTOR3 AddRot, bool bCollisionRelease, int nHitStopTime)
{
	CDraiyaAttack* pDraiyaAttack = DBG_NEW CDraiyaAttack;   //敵を生成
	bool bSuccess = pDraiyaAttack->CObject::GetCreateSuccess();
	int nIdx = 0;//モデルのインデックス
	if (bSuccess == true)
	{
		if (pDraiyaAttack != nullptr)
		{
			pDraiyaAttack->SetAttackType(type);  //攻撃の種類
			pDraiyaAttack->Init();         //初期化処理
			pDraiyaAttack->SetUseBlockCollision(false);
			pDraiyaAttack->SetHitStopTime(nHitStopTime);          //当たったときに割り当てるヒットストップ時間を設定する
			pDraiyaAttack->SetCollisionRelease(bCollisionRelease);//当たった時に消すかどうかを設定する
			pDraiyaAttack->SetUseInteria(false);//慣性を使用しない
			pDraiyaAttack->SetMove(move);  //移動量
			pDraiyaAttack->SetDoCollision(true);                  //当たり判定をするかどうかを設定
			pDraiyaAttack->SetLife(nLife);                    //体力を設定
			pDraiyaAttack->SetMaxLife(nLife);                 //最大体力を設定
			pDraiyaAttack->SetAutoSubLife(true);              //自動的に体力を減らすかどうか
			pDraiyaAttack->SetTargetType(CAttack::TARGETTYPE_PLAYER);//誰に当てるかを設定
			pDraiyaAttack->SetCollisionType(CollisionType);   //当たり判定の種類
			pDraiyaAttack->SetPower(nDamage);                 //威力
			pDraiyaAttack->SetUseAddRot(true, AddRot);                 //加算する向きの量
			pDraiyaAttack->CObjectX::SetPos(pos);             //オブジェクトXの位置を設定
			pDraiyaAttack->SetRot(Rot);                       //向きを設定
			pDraiyaAttack->SetPosOld(pos);                                                                                         //1f前の位置を設定
			pDraiyaAttack->SetSupportPos(pos);                                                                                     //設置位置
			pDraiyaAttack->CObject::SetType(CObject::TYPE_ATTACK);                                                                  //オブジェクトの種類を決める
			pDraiyaAttack->CObjectX::SetScale(Scale);                                                                              //オブジェクトの拡大率
			pDraiyaAttack->SetFormarScale(Scale);                                       //元の拡大率を設定する
			pDraiyaAttack->SetUseDeath(true);                 //死亡フラグを発動するかどうかを設定する
			nIdx = CManager::GetObjectXInfo()->Regist(CObjectX::OBJECTXTYPE_ATTACK, CAttack::m_ATTACK_FILENAME[(int)(type)]);
			pDraiyaAttack->CObjectX::BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(CObjectX::OBJECTXTYPE_ATTACK, nIdx),
				CManager::GetObjectXInfo()->GetBuffMat(CObjectX::OBJECTXTYPE_ATTACK, nIdx),
				CManager::GetObjectXInfo()->GetdwNumMat(CObjectX::OBJECTXTYPE_ATTACK, nIdx),
				CManager::GetObjectXInfo()->GetTexture(CObjectX::OBJECTXTYPE_ATTACK, nIdx),
				CManager::GetObjectXInfo()->GetColorValue(CObjectX::OBJECTXTYPE_ATTACK, nIdx));                       //モデル情報を割り当てる

			pDraiyaAttack->CObjectX::SetObjXType(CObjectX::OBJECTXTYPE_ATTACK);                            //オブジェクトXのタイプを設定
			pDraiyaAttack->CObjectX::SetTypeNum((int)(type));                                             //オブジェクトXごとのタイプ番号を設定
			pDraiyaAttack->SetSize();                                                                     //Xオブジェクトのサイズを設定する
			pDraiyaAttack->CObjectX::SetRotType(CObjectX::ROTTYPE::ROTTYPE_NORMAL);                       //向きの種類を設定
		}
		else
		{//オブジェクトに空きがなかったので破棄する
			delete pDraiyaAttack;
			pDraiyaAttack = nullptr;
			return nullptr;
		}
	}
	return pDraiyaAttack;
}
//===================================================================================================================================================

//==========================================================＜＜＜ガブリエル攻撃クラス＞＞＞=====================================================================================================================

//==============================================================================
//コンストラクタ
//==============================================================================
CGabrielAttack::CGabrielAttack() : m_AttackMove(GABRIELATTACKMOVE_00)
{

}
//===================================================================================================================================================

//==============================================================================
//デストラクタ
//==============================================================================
CGabrielAttack::~CGabrielAttack()
{

}
//===================================================================================================================================================

//==============================================================================
//初期化処理
//==============================================================================
HRESULT CGabrielAttack::Init()
{
	CAttack::Init();
	return S_OK;
}
//===================================================================================================================================================

//==============================================================================
//終了処理
//==============================================================================
void CGabrielAttack::Uninit()
{
	CAttack::Uninit();
}
//===================================================================================================================================================

//==============================================================================
//更新処理
//==============================================================================
void CGabrielAttack::Update()
{
	CAttack::Update();
}
//===================================================================================================================================================

//==============================================================================
//描画処理
//==============================================================================
void CGabrielAttack::Draw()
{
	CAttack::Draw();
}
//===================================================================================================================================================

//==============================================================================
//死亡フラグ設定処理
//==============================================================================
void CGabrielAttack::SetDeath()
{
	CAttack::SetDeath();
}
//===================================================================================================================================================

//==============================================================================
//生成処理
//==============================================================================
CGabrielAttack* CGabrielAttack::Create(ATTACKTYPE type, int nDamage, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 Scale, D3DXVECTOR3 Rot, COLLISIONTYPE CollisionType, D3DXVECTOR3 AddRot, bool bCollisionRelease, int nHitStopTime)
{
	CGabrielAttack* pGabrielAttack = DBG_NEW CGabrielAttack;   //敵を生成
	bool bSuccess = pGabrielAttack->CObject::GetCreateSuccess();
	int nIdx = 0;//モデルのインデックス
	if (bSuccess == true)
	{
		if (pGabrielAttack != nullptr)
		{
			pGabrielAttack->SetAttackType(type);  //攻撃の種類
			pGabrielAttack->Init();         //初期化処理
			pGabrielAttack->SetUseBlockCollision(false);
			pGabrielAttack->SetHitStopTime(nHitStopTime);          //当たったときに割り当てるヒットストップ時間を設定する
			pGabrielAttack->SetCollisionRelease(bCollisionRelease);//当たった時に消すかどうかを設定する
			pGabrielAttack->SetUseInteria(false);//慣性を使用しない
			pGabrielAttack->SetMove(move);  //移動量
			pGabrielAttack->SetDoCollision(true);                  //当たり判定をするかどうかを設定
			pGabrielAttack->SetLife(nLife);                    //体力を設定
			pGabrielAttack->SetMaxLife(nLife);                 //最大体力を設定
			pGabrielAttack->SetAutoSubLife(true);              //自動的に体力を減らすかどうか
			pGabrielAttack->SetTargetType(CAttack::TARGETTYPE_PLAYER);//誰に当てるかを設定
			pGabrielAttack->SetCollisionType(CollisionType);   //当たり判定の種類
			pGabrielAttack->SetPower(nDamage);                 //威力
			pGabrielAttack->SetUseAddRot(true, AddRot);                 //加算する向きの量
			pGabrielAttack->CObjectX::SetPos(pos);             //オブジェクトXの位置を設定
			pGabrielAttack->SetRot(Rot);                       //向きを設定
			pGabrielAttack->SetPosOld(pos);                                                                                         //1f前の位置を設定
			pGabrielAttack->SetSupportPos(pos);                                                                                     //設置位置
			pGabrielAttack->CObject::SetType(CObject::TYPE_ATTACK);                                                                  //オブジェクトの種類を決める
			pGabrielAttack->CObjectX::SetScale(Scale);                                                                              //オブジェクトの拡大率
			pGabrielAttack->SetFormarScale(Scale);                                       //元の拡大率を設定する
			pGabrielAttack->SetUseDeath(true);                 //死亡フラグを発動するかどうかを設定する
			nIdx = CManager::GetObjectXInfo()->Regist(CObjectX::OBJECTXTYPE_ATTACK, CAttack::m_ATTACK_FILENAME[(int)(type)]);
			pGabrielAttack->CObjectX::BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(CObjectX::OBJECTXTYPE_ATTACK, nIdx),
				CManager::GetObjectXInfo()->GetBuffMat(CObjectX::OBJECTXTYPE_ATTACK, nIdx),
				CManager::GetObjectXInfo()->GetdwNumMat(CObjectX::OBJECTXTYPE_ATTACK, nIdx),
				CManager::GetObjectXInfo()->GetTexture(CObjectX::OBJECTXTYPE_ATTACK, nIdx),
				CManager::GetObjectXInfo()->GetColorValue(CObjectX::OBJECTXTYPE_ATTACK, nIdx));                       //モデル情報を割り当てる

			pGabrielAttack->CObjectX::SetObjXType(CObjectX::OBJECTXTYPE_ATTACK);                            //オブジェクトXのタイプを設定
			pGabrielAttack->CObjectX::SetTypeNum((int)(type));                                             //オブジェクトXごとのタイプ番号を設定
			pGabrielAttack->SetSize();                                                                     //Xオブジェクトのサイズを設定する
			pGabrielAttack->CObjectX::SetRotType(CObjectX::ROTTYPE::ROTTYPE_NORMAL);                       //向きの種類を設定
		}
		else
		{//オブジェクトに空きがなかったので破棄する
			delete pGabrielAttack;
			pGabrielAttack = nullptr;
			return nullptr;
		}
	}
	return pGabrielAttack;
}
//===================================================================================================================================================

//==============================================================================
//攻撃１の全ての処理
//==============================================================================
void CGabrielAttack::Attack01Process()
{
	//D3DXVECTOR3 Pos = GetPos();

	//switch (GetAttackType())
	//{
	//case ATTACKTYPE01_METEO:
	//	if (Pos.y <= 0.0f)
	//	{
	//		for (int nCnt = 0; nCnt < m_nMAX_SUMMON_METEO_TORNADO; nCnt++)
	//		{
	//			
	//		}
	//	}
	//	break;
	//default:
	//	break;
	//}
}
//===================================================================================================================================================