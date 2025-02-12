//==============================================================================================
//
//７月２日：ボスを実装する[boss.cpp]
//Author:ShinaTaiyo
//
//==============================================================================================

//=========================
//インクルード
//=========================
#include "bullet.h"
#include "manager.h"
#include "explosion.h"
#include "player.h"
#include "enemy.h"
#include "game.h"
#include "block.h"
#include "model.h"
#include "sound.h"
#include "calculation.h"
#include "collision.h"
#include "texture.h"
#include "particle.h"
#include "boss.h"
//=================================================

//=========================
//静的メンバ
//=========================
const char* CBullet::m_apBULLET_FILENAME[CBullet::BULLETTEX_MAX] =
{
	"data\\TEXTURE\\Bullet\\PlayerBullet00.png",
	"data\\TEXTURE\\Bullet\\EnemyBullet00.png",
	"data\\TEXTURE\\Bullet\\WaterBullet.png",
};
//========================================================

//=========================
//コンストラクタ
//=========================
CBullet::CBullet() : m_bBlockColliion(false),m_type(BULLETTYPE_PLAYER)
{

}
//=================================================

//=========================
//デストラクタ
//=========================
CBullet::~CBullet()
{

}
//=================================================

//=========================
//初期化処理
//=========================
HRESULT CBullet::Init()
{
	//===========================
	//ビルボード初期化
	//===========================
	CBillboard::Init();

	//==================================================================================

	m_bUse = false;                           //使用状態
	m_fGravity = 0.0f;                        //重力
	m_MoveType = MOVETYPE00_NORMAL;           //移動方法
	m_nPower = 0;                             //威力
								              
	m_fLength = 0.0f;                         //距離の補正

	m_bUseDivision = false;                   //分裂を使用するかどうか
	m_nNumDivision = 0;                       //分裂数
	m_nLimitDivision = 0;                     //分裂上限

	m_bUseBlockCollision = false;             //ブロックとの当たり判定を使用するかどうか

	//==========================
	//MP関連
	//==========================
	m_nHitAddMp = 0;                          //ヒットした時のMP変動値
	//==================================================================================

	//==========================
	//反射関連
	//==========================
	m_bUseReflection = false;                 //反射を使用するかどうか
	//==================================================================================

	if (m_type == BULLETTYPE_ENEMY)
	{
		CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_SHOT_002);
	}

	return S_OK;
}
//=================================================

//=========================
//終了処理
//=========================
void CBullet::Uninit()
{
	CBillboard::Uninit();
	//SetDeath();//死亡フラグ設定
}
//=================================================

//=========================
//更新処理
//=========================
void CBullet::Update()
{
	//=======================
	//変数宣言
	//=======================
	D3DXVECTOR3 & Pos = CBillboard::GetPos();                  //ビルボードの位置を取得する
	D3DXVECTOR3& PosOld = GetPosOld();                          //1f前の位置を取得
	D3DXVECTOR3& Move = GetMove();                             //ビルボードの移動量を取得する
	float &fWidth = CBillboard::GetWidth();                    //ビルボードの横幅を取得する
	float &fHeight = CBillboard::GetHeight();                  //ビルボードの高さを取得する
	bool bIsJumping = false;
	bool bIsLanding = false;
	bool bIsWalling = false;
	bool bIsLiftLanding = false;
	bool bSuccessCollisionX = false;
	bool bSuccessCollisionY = false;
	CObject* pSaveObj = nullptr;
	D3DXVECTOR3 ComparisonVtxMax = NULL_VECTOR3;             //相手の最大頂点代入用
	D3DXVECTOR3 ComparisonVtxMin = NULL_VECTOR3;             //相手の最小頂点代入用
	D3DXVECTOR3 ComparisonPos = NULL_VECTOR3;                //相手の位置の代入用

	CModel* pModel = nullptr;                                //モデルへのポインタ
	CBlock* pBlock = nullptr;                                //ブロックへのポインタ

	bool bCollision = false;                                 //当たり判定が発動したかどうか      

	int& nLife = GetLife();                                  //体力
	//===================================================
	m_fLength += 10.0f;

	if (nLife <= 0)
	{//体力が０になったら・・・
		m_bUse = false;
	}


	//========================================
	//ビルボードの更新処理
	//========================================
	CBillboard::Update();
	//===========================================================================================

	//========================================
	//弾の当たり判定
	//========================================
	Collision();
	//===========================================================================================

	//=======================================
	//反射
	//=======================================
	if (m_bUseReflection == true)
	{
		if (bSuccessCollisionX == true)
		{
			Move.x *= -1;
		}
		if (bSuccessCollisionY == true)
		{
			Move.y *= -1;
		}
	}
	//===========================================================================================

	//=======================================
	//弾のブロックとの当たり判定
	//=======================================
	if (m_bBlockColliion == true)
	{
		BlockCollisionProcess();
	}
	//===========================================================================================

	//=======================================
	//使用状態がオフになったら・・・
	//=======================================
	if (m_bUse == false)
	{
		//Uninit();
		SetDeath();
	}
	//===========================================================================================


}
//=================================================

//=========================
//描画処理
//=========================
void CBullet::Draw()
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	//================================================
	//描画の調整
	//================================================
	//ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//Zの比較方法変更。（モデルがあっても隠れなくなる）
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	////Zバッファに書き込まない（重なり方に違和感がなくなる）
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//アルファテストを有効(アルファ値が０より大きい場合に描画する）
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//=======================================================================================================================================
	
	CBillboard::Draw();

	//================================================
	//描画の調整を元に戻す
	//================================================

	//ライトを有効に戻す
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	////Zの比較方法変更。
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	////Zバッファに書き込む
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//アルファテスト無効に戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//=======================================================================================================================================
}
//=======================================================================================================================================

//===============================================================
//死亡フラグを設定
//===============================================================
void CBullet::SetDeath()
{
	CObject::SetDeath();
}
//=======================================================================================================================================

//=====================================================
//バレット生成処理
//=====================================================
CBullet* CBullet::Create(BULLETTYPE type, BULLETTEX tex, int nLife, int nPower, float fWidth, float fHeight, D3DXVECTOR3 pos, D3DXVECTOR3 move)
{
	CBullet* pBullet = DBG_NEW CBullet;                             //弾を生成
	CTexture* pTextureClass = CManager::GetTexture();           //テクスチャクラスを取得
	bool bSuccess = pBullet->CObject::GetCreateSuccess();       //生成が成功したかどうかを取得する
	if (bSuccess == true)
	{//生成が成功したら
		if (pBullet != nullptr)
		{
			pBullet->m_type = type;                           //弾の種類
			pBullet->Init();                                  //初期化処理
			pBullet->SetUseDeath(true);                 //死亡フラグを発動するかどうかを設定する
			pBullet->m_MoveType = MOVETYPE00_NORMAL;          //普通の移動方法
			pBullet->SetLife(nLife);                          //体力
			pBullet->SetMaxLife(nLife);                       //最大体力
			pBullet->SetMove(move);                           //移動量の設定
			pBullet->m_bUse = true;                           //使用状態
			pBullet->m_nPower = nPower;                       //威力
			pBullet->SetTextureIndex(pTextureClass->Regist(m_apBULLET_FILENAME[tex]));

			pBullet->CBillboard::bindTexture(pTextureClass->GetAddress(pBullet->GetTextureIndex()));     //テクスチャをセットする　
			pBullet->CBillboard::SetPos(pos);                                                            //オブジェクト２Ｄの位置を設定
			pBullet->SetSupportPos(pos);                                                                 //召喚位置を設定
			pBullet->CBillboard::SetSize(fWidth, fHeight);                                               //ポリゴンのサイズを設定
			pBullet->CBillboard::SetFormarSize(fWidth, fHeight);                                         //ポリゴンのサイズを設定
			pBullet->CObject::SetType(CObject::TYPE_BULLET);                                             //オブジェクトの種類を決める
			pBullet->SetAnimInfo(1,1,NORMAL_COL,false);                                                  //アニメーション情報を設定

		}
	}
	else
	{//オブジェクトに空きがなかったので破棄する
		delete pBullet;
		pBullet = nullptr;
		return nullptr;
	}
	return pBullet;
}
//==================================================================================================================================================

//=====================================================
//当たり判定を取る
//=====================================================
void CBullet::Collision()
{
	//=================================
	//ビルボードから情報を取得
	//==================================
	D3DXVECTOR3 Pos = NULL_VECTOR3;                    //オブジェクトの位置を取得)
	D3DXVECTOR3 Size = NULL_VECTOR3;                           //オブジェクトのサイズ
	Size.x = GetWidth();                                       //横幅を取得
	Size.y = GetHeight();                                       //高さを取得

	if (GetUseMtxChild() == true)
	{//子マトリックスを使っているなら
		Pos = GetMtxTransPos();
	}
	else
	{
		Pos = GetPos();
	}
	D3DXVECTOR3 ComparisonPos = NULL_VECTOR3;                  //プレイヤーの位置代入用
	D3DXVECTOR3 ComparisonVtxMax = NULL_VECTOR3;               //判定対象の最大頂点
	D3DXVECTOR3 ComparisonVtxMin = NULL_VECTOR3;               //判定対象の最小頂点
	CObjectX* pObjX = nullptr;                                 //オブジェクトX
	CObject* pObj = nullptr;                                   //オブジェクト格納用
	bool bCollision = false;                                   //当たり判定をするかどうか
	//===========================================================

	//==================================
	//弾の当たり判定
	//==================================
	for (int nCntPriority = 0; nCntPriority < m_nMAXPRIORITY; nCntPriority++)
	{

		pObj = GetTopObject(nCntPriority);//トップオブジェクトを取得
		while (pObj != nullptr)
		{
			//次のオブジェクトを格納
			CObject* pNext = pObj->GetNextObject();

			//種類の取得（敵なら当たり判定）
			CObject::TYPE type = pObj->GetType();

			if (type == CObject::TYPE_ENEMY || type == CObject::TYPE_PLAYER || type == CObject::TYPE_BOSS || type == CObject::TYPE_BLOCK)
			{
				pObjX = (CObjectX*)pObj;

				ComparisonPos = pObjX->GetPos();
				ComparisonVtxMax = pObjX->GetVtxMax();
				ComparisonVtxMin = pObjX->GetVtxMin();
			}
			else
			{
				//リストを次に進める
				pObj = pNext;
				continue;
			}

			float fLength = CCalculation::CalculationLength(pObjX->GetPos(), Pos);

			bCollision = false;
			if (fLength < Size.x * 3.0f)
			{
				bCollision = CCollision::CollisionSquare(Pos, D3DXVECTOR3(Size.x, Size.y, Size.x) * 0.5f, D3DXVECTOR3(-Size.x, -Size.y, -Size.x) * 0.5f,
					ComparisonPos, ComparisonVtxMax, ComparisonVtxMin);
			}
			if (bCollision == true)
			{//弾がプレイヤーに当たったら
				switch (m_type)
				{
				case BULLETTYPE_ENEMY:
					if (CScene::GetMode() == CScene::MODE_GAME && type == CObject::TYPE::TYPE_PLAYER)
					{
						if (CGame::GetPlayer()->GetDodgeState() == false)
						{
							HitAddMpProcess();
							CGame::GetPlayer()->SetDamage(m_nPower, 1);
							m_bUse = false;//使用状態をオフにする
						}
					}
					break;
				case BULLETTYPE_PLAYER:
					if (CScene::GetMode() == CScene::MODE_GAME)
					{
						if (type == CObject::TYPE::TYPE_ENEMY)
						{//敵へダメージを与える
							pObjX->SetDamage(m_nPower, 0);
							HitAddMpProcess();
							if (m_bUseDivision == true)
							{
								int nRadius = (int)(pObjX->GetSize().x);
								DivisionBulletProcess(nRadius);
							}
							m_bUse = false;//使用状態をオフにする
						}
						else if (type == CObject::TYPE::TYPE_BOSS)
						{//ボスへダメージを与える
							pObjX->SetDamage(m_nPower, 0);
							HitAddMpProcess();
							if (m_bUseDivision == true)
							{
								int nRadius = (int)(pObjX->GetSize().x);

								DivisionBulletProcess(nRadius);
							}
							m_bUse = false;//使用状態をオフにする
						}
					}
					break;
				default:
					break;
				}
			}
			//リストを次に進める
			pObj = pNext;
		}
	}

}
//================================================================================================================

//==============================================================
//MPを割り当てる処理
//==============================================================
void CBullet::HitAddMpProcess()
{
	if (m_nHitAddMp != 0)
	{//値が０以外なら
		if (CGame::GetPlayer() != nullptr)
		{
			CGame::GetPlayer()->SetMp(m_nHitAddMp);//MPを変動させる
		}
	}
}
//================================================================================================================

//==============================================================
//分裂弾の処理
//==============================================================
void CBullet::DivisionBulletProcess(int nRadius)
{
	float fRandRot = 0.0f;
	CBullet* pBullet = nullptr;
	D3DXVECTOR3 Pos = GetPos();
	float fWidth = GetWidth();
	float fHeight = GetHeight();
	D3DXVECTOR3 RandPos = NULL_VECTOR3;
	fRandRot = float(rand() % 628) / 100;

	for (int nCnt = 0; nCnt < m_nNumDivision; nCnt++)
	{
		fRandRot = float(rand() % 628) / 100;
		RandPos.x = float(rand() % (nRadius * 8) - (nRadius * 4));
		RandPos.y = float(rand() % (nRadius * 8) - (nRadius * 4));
		pBullet = CBullet::Create(m_type, CBullet::BULLETTEX_PLAYER, 120,5,10.0f,10.0f, Pos + RandPos, D3DXVECTOR3(sinf(fRandRot) * 2.5f, cosf(fRandRot) * 2.5f, 0.0f));
		pBullet->SetUseAddSpeed(true, false, 0.01f);
		pBullet->SetHitAddMp(1);
		if (m_nLimitDivision > 0)
		{
			pBullet->SetUseDivision(true, 3, m_nLimitDivision - 1);
		}
	}
}
//================================================================================================================

//==============================================================
//ブロックとの当たり判定
//==============================================================
void CBullet::BlockCollisionProcess()
{
	CObject* pObj = nullptr;
	CBlock* pBlock = nullptr;
	bool bCollision = false;
	D3DXVECTOR3 Pos = GetPos();
	D3DXVECTOR3 VtxMax = D3DXVECTOR3(GetWidth() * 0.5f, GetHeight() * 0.5f, GetWidth() * 0.5f);
	D3DXVECTOR3 VtxMin = D3DXVECTOR3(-GetWidth() * 0.5f, -GetHeight() * 0.5f, -GetWidth() * 0.5f);
	//==================================
    //トラップの当たり判定
    //==================================
	for (int nCntPriority = 0; nCntPriority < m_nMAXPRIORITY; nCntPriority++)
	{
		pObj = GetTopObject(nCntPriority);//トップオブジェクトを取得
		while (pObj != nullptr)
		{
			//次のオブジェクトを格納
			CObject* pNext = pObj->GetNextObject();

			//種類の取得（敵なら当たり判定）
			CObject::TYPE type = pObj->GetType();

			if (type == CObject::TYPE_BLOCK)
			{
				pBlock = (CBlock*)pObj;

				bCollision = CCollision::CollisionSquare(Pos, VtxMax, VtxMin, pBlock->GetPos(), pBlock->GetVtxMax(), pBlock->GetVtxMin());

				if (bCollision == true)
				{
					SetDeath();
				}
			}
			//リストを次に進める
			pObj = pNext;
		}
	}
}
//================================================================================================================