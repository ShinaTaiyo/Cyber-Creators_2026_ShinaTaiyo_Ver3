//==============================================================================================================
//
//１０月２８日：ロックオンした場所に弾を撃ちたい[lockon.cpp]
//Author:ShinaTaiyo
//
//==============================================================================================================

//===============================================================
//インクルード
//===============================================================
#include "lockon.h"
#include "manager.h"
#include "camera.h"
#include "texture.h"
#include "collision.h"
#include "input.h"
#include "particle.h"
#include "particle2d.h"
#include "calculation.h"
#include "objectX.h"
#include "game.h"
#include "debugtext.h"
#include "player.h"
#include "ui.h"
//==============================================================================================================

//===============================================================
//静的メンバ宣言
//===============================================================

//テクスチャファイル名
const string CLockon::s_LOCKON_FILENAME[static_cast<int>(CLockon::TYPE::MAX)] =
{
	"data\\TEXTURE\\LockOn\\Target_000.png",
	"data\\TEXTURE\\LockOn\\DiveTarget_001.png",
	"data\\TEXTURE\\LockOn\\StuckShotTarget_000.png"
};

//===============================================================
//コンストラクタ
//===============================================================
CLockon::CLockon(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CObject2D(nPri,bUseintPri,type,ObjType),
m_LockOnPos(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_NowRay(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_FrontPos(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_EndState(ENDSTATE::NONE),m_NearRayColObjPos(D3DXVECTOR3(0.0f,0.0f,0.0f)),
m_bRayCollision(false),m_Type(TYPE::SHOT)
{

}
//==============================================================================================================

//===============================================================
//デストラクタ
//===============================================================
CLockon::~CLockon()
{

}
//==============================================================================================================

//===============================================================
//初期化処理
//===============================================================
HRESULT CLockon::Init()
{
	CObject2D::Init();//オブジェクト2D
	return S_OK;
}
//==============================================================================================================

//===============================================================
//終了処理
//===============================================================
void CLockon::Uninit()
{
	CObject2D::Uninit();//オブジェクト2D
}
//==============================================================================================================

//===============================================================
//更新処理
//===============================================================
void CLockon::Update()
{
	D3DXVECTOR3 Rot = GetRot();//向き
	Rot.z += 0.02f;//ポリゴンを回転させる
	SetRot(Rot);//向きを設定する

	//移動処理
	MoveProcess();

	//レイが狙っている奥の壁の判定位置をサーチする
	BackWallRayCollisionPosSearch();

	//レイを計算する
	CalcRay();

	//一番近いオブジェクトのレイが当たった位置を求める
	RayCollisionToObject();

	//オブジェクト2D更新処理
	CObject2D::Update();
}
//==============================================================================================================

//===============================================================
//描画処理
//===============================================================
void CLockon::Draw()
{
	CObject2D::Draw();//オブジェクト2Dの描画処理
}
//==============================================================================================================

//===============================================================
//死亡フラグ設定処理
//===============================================================
void CLockon::SetDeath()
{
	CObject2D::SetDeath();//オブジェクト2Dの死亡フラグ設定処理
}
//==============================================================================================================

//===============================================================
//生成処理
//===============================================================
CLockon* CLockon::Create(TYPE Type, D3DXVECTOR3 Pos, CObject2D::POLYGONTYPE PolygonType, float fWidth, float fHeight, D3DXCOLOR col)
{
	CTexture* pTexture = CManager::GetTexture();       //テクスチャクラス
	CLockon* pLockOn = DBG_NEW CLockon;                //ロックオン

	pLockOn->Init();                                   //初期化処理
	pLockOn->SetPos(Pos);                              //位置設定
	pLockOn->SetSupportPos(Pos);                       //支点位置設定
	pLockOn->SetAnimInfo(1, 1,false);                  //アニメーション情報を設定（必ず）
	pLockOn->SetPolygonType(PolygonType);              //ポリゴンの中心点の種類を設定する
	pLockOn->SetWidth(fWidth);                         //横幅を設定
	pLockOn->SetMaxWidth(fWidth);                      //最大横幅を設定
	pLockOn->SetHeight(fHeight);                       //高さを設定
	pLockOn->SetMaxHeight(fHeight);                    //最大高さを設定
	pLockOn->m_Type = Type;                            //種類を設定

	//テクスチャ設定
	pLockOn->SetTextureIndex(pTexture->Regist(s_LOCKON_FILENAME[static_cast<int>(Type)])); //テクスチャを登録
	pLockOn->BindTexture(pTexture->GetAddress(pLockOn->GetTextureIndex()));                //登録、指定したテクスチャ番号のアドレスを取得

	//体力を使用しない
	pLockOn->SetUseLife(false, 1, 1);

	//オブジェクトタイプ設定
	pLockOn->SetType(CObject::TYPE::LOCKON);


	return pLockOn;
}
//==============================================================================================================

//===============================================================
//テクスチャを変える
//===============================================================
void CLockon::ChengeTexture(TYPE Type)
{
	m_Type = Type;//種類を設定

	CTexture* pTexture = CManager::GetTexture();//テクスチャ管理クラスへのポインタ

	//変えたロックオンのテクスチャを大きく表示する演出
	CLockon* pLockOn = CLockon::Create(m_Type, GetPos(), GetPolygonType(), GetWidth(), GetHeight(), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	pLockOn->SetUseAddScale(D3DXVECTOR2(s_fLOCKONSTAGING_ADDSCALE, s_fLOCKONSTAGING_ADDSCALE), true);//演出用に変えたモードを拡大する
	pLockOn->SetUseLifeRatioColor(true);                                                             //体力の割合に応じて透明度を薄くする
	pLockOn->SetUseLife(true, s_nLOCKONSTAGING_LIFE, s_nLOCKONSTAGING_LIFE);                         //体力を使用する
	pLockOn->SetUseDeath(true);                                                                      //死亡フラグを使用する

	//テクスチャ設定
	SetTextureIndex(pTexture->Regist(s_LOCKON_FILENAME[static_cast<int>(m_Type)]));//テクスチャを登録
	BindTexture(pTexture->GetAddress(GetTextureIndex()));//登録、指定したテクスチャ番号のアドレスを取得

}
//==============================================================================================================

//===============================================================
//移動処理
//===============================================================
void CLockon::MoveProcess()
{

}
//==============================================================================================================

//===============================================================
//カーソルの先にあるオブジェクトをサーチ
//===============================================================
void CLockon::BackWallRayCollisionPosSearch()
{
	D3DXVECTOR3 Pos = GetPos();//位置

	CCalculation::CalcScreenToWorld(&m_LockOnPos,GetPos().x,GetPos().y, 1.0f,SCREEN_WIDTH,SCREEN_HEIGHT,CManager::GetCamera()->GetMtxView(),
		CManager::GetCamera()->GetMtxProjection()); //描画範囲の一番奥の位置
}
//==============================================================================================================

//===============================================================
//レイを計算する
//===============================================================
void CLockon::CalcRay()
{
	D3DXVECTOR3 FarPos = D3DXVECTOR3(0.0f,0.0f,0.0f); //奥
	CCamera* pCamera = CManager::GetCamera();         //カメラへのポインタを取得
	//============================================
	//カメラ手前と奥のワールド座標を求める
	//============================================
	CCalculation::CalcScreenToWorld(&m_FrontPos,GetPos().x,GetPos().y, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT,
		CManager::GetCamera()->GetMtxView(), CManager::GetCamera()->GetMtxProjection());//手前

	//デバッグ表示
	CManager::GetDebugText()->PrintDebugText("カメラ手前座標：%f %f %f\n", m_FrontPos.x, m_FrontPos.y, m_FrontPos.z);
	CManager::GetDebugText()->PrintDebugText("カメラ支点：%f %f %f\n", pCamera->GetPosV().x, pCamera->GetPosV().y, pCamera->GetPosV().z);

	CCalculation::CalcScreenToWorld(&FarPos,GetPos().x,GetPos().y, 1.0f, SCREEN_WIDTH, SCREEN_HEIGHT,
		CManager::GetCamera()->GetMtxView(), CManager::GetCamera()->GetMtxProjection());//奥
	//============================================================================================================================

	m_NowRay = FarPos - m_FrontPos;          //手前座標と奥座標のベクトルを求める
	D3DXVec3Normalize(&m_NowRay, &m_NowRay); //ベクトルを正規化する
}
//==============================================================================================================

//===============================================================
//レイがどのオブジェクトに当たっているかを計算する
//===============================================================
void CLockon::RayCollisionToObject()
{
	//*変数宣言
	bool bRayCollision = false;//レイが当たったかの状態をリセット
	vector<D3DXVECTOR3> VecCollisionSuccess;     //当たり判定が成功した位置のvector
	CPlayer* pPlayer = CGame::GetPlayer();

	//レイと一致した全てのオブジェクトを求め、中心点をVectorに保存
	for (int nCntPri = 0; nCntPri < CObject::m_nMAXPRIORITY; nCntPri++)
	{
		CObject* pObj = CObject::GetTopObject(nCntPri);//先頭オブジェクトを取得
		D3DXVECTOR3 CollisionStartPos = D3DXVECTOR3(0.0f,0.0f,0.0f);//衝突判定開始位置
		D3DXVECTOR3 CollisionEndPos = D3DXVECTOR3(0.0f,0.0f,0.0f);  //衝突判定終了位置
		while (pObj != nullptr)
		{
			CObject* pNext = pObj->GetNextObject();//次のオブジェクトのポインタを取得

			if (pObj->GetType() == CObject::TYPE::ENEMY || pObj->GetType() == CObject::TYPE::BLOCK || pObj->GetType() == CObject::TYPE::BGMODEL)
			{
				CObjectX* pObjX = dynamic_cast<CObjectX*>(pObj);
				//指定したモデルの位置
				bRayCollision = CCollision::RayIntersectsAABBCollisionPos(m_FrontPos, m_NowRay, pObjX->GetSizeInfo().GetVtxMin() + pObjX->GetPosInfo().GetPos(), pObjX->GetSizeInfo().GetVtxMax() + pObjX->GetPosInfo().GetPos(),
					CollisionStartPos);

				if (CCalculation::CalculationLength(m_FrontPos, CollisionStartPos) < CManager::GetCamera()->GetPosRToPosVLength())
				{//当たった距離がカメラの注視点と視点の距離よりも近ければ
					bRayCollision = false;//当たらなかったことにする
				}

				// レイが当たった
				if (bRayCollision == true)
				{
					D3DXVECTOR3 ScreenPos = CCalculation::CalcWorldToScreenNoViewport(pObjX->GetPosInfo().GetSenterPos(), *CManager::GetCamera()->GetMtxView(), *CManager::GetCamera()->GetMtxProjection(),
						float(SCREEN_WIDTH), float(SCREEN_HEIGHT));//対象のスクリーン座標を取得
					VecCollisionSuccess.push_back(CollisionStartPos);//当たり判定が成功したオブジェクトの判定開始位置を保存する
				}

			}

			pObj = pNext;//オブジェクトを次に進める
		}
	}

	D3DXVECTOR3 NearCollisionPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); //当たり判定が成功した位置の中で一番近い位置

	//レイの方向が一致したオブジェクトが存在したら、その中で一番距離が近いオブジェクトの中心点を求め、そこを目掛けた移動量を求める
	if (VecCollisionSuccess.size() != 0)
	{//狙っているオブジェクトの中心点に向かって撃つ
		float fLength = 0.0f;//距離
		float fMinLength = 0.0f;//一番近い距離格納用
		for (auto it = VecCollisionSuccess.begin(); it != VecCollisionSuccess.end(); ++it)
		{
			fLength = CCalculation::CalculationLength(m_FrontPos, *it);//レイの判定が成功したオブジェクトの位置とプレイヤーの中心点の距離を測る

			if (it == VecCollisionSuccess.begin())
			{
				fMinLength = fLength;
				NearCollisionPos = *it;
			}
			else
			{
				if (fLength < fMinLength)
				{//一番近い距離より近かったら
					NearCollisionPos = *it;//一番近いオブジェクトを格納
				}
			}
		}
		m_bRayCollision = true;//レイの当たり判定が成功
		m_NearRayColObjPos = NearCollisionPos;//レイが当たった一番近いオブジェクトに向かって撃つ
	}
	else
	{//狙っている方向の奥の壁に向かって撃つ
		m_bRayCollision = false;//レイの当たり判定が失敗
		m_NearRayColObjPos = m_LockOnPos;//レイが当たったオブジェクトがないので、奥の壁に向かって撃つ
	}
	//====================================================================================================================================================================

	//Vectorをクリア
	VecCollisionSuccess.clear();         //確保した配列を空にする
	VecCollisionSuccess.shrink_to_fit(); //現在のサイズに合わせてメモリ領域をコンパクトにする

}
//==============================================================================================================