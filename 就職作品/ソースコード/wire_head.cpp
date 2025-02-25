//===================================================================================================================================
//
//１１月２１日：ワイヤーを作る[wire_head.cpp]
//Author:ShinaTaiyo
//
//===================================================================================================================================

//=================================================================
//インクルード
//=================================================================
#include "wire_head.h"
#include "renderer.h"
#include "game.h"
#include "objectXInfo.h"
#include "manager.h"
#include "debugtext.h"
#include "collision.h"
//===================================================================================================================================

//=================================================================
//コンストラクタ
//=================================================================
CWireHead::CWireHead(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CObjectX(nPri, bUseintPri, type, ObjType),
m_bCollision(false),m_nCoolTime(0), m_CollisionObjType(CObject::TYPE::NONE)
{

}
//===================================================================================================================================

//=================================================================
//デストラクタ
//=================================================================
CWireHead::~CWireHead()
{

}
//===================================================================================================================================

//=================================================================
//初期化処理
//=================================================================
HRESULT CWireHead::Init()
{
	CObjectX::Init();//オブジェクトX初期化処理
	return S_OK;
}
//===================================================================================================================================

//=================================================================
//終了処理
//=================================================================
void CWireHead::Uninit()
{
	CObjectX::Uninit();//オブジェクトX終了処理
}
//===================================================================================================================================

//=================================================================
//更新処理
//=================================================================
void CWireHead::Update()
{
	CObjectX::Update();//オブジェクトX更新処理

	CManager::GetDebugText()->PrintDebugText("ワイヤーヘッドの位置：%f %f %f\n", GetPosInfo().GetPos().x, GetPosInfo().GetPos().y, GetPosInfo().GetPos().z);

	if (m_nCoolTime == 0)
	{//０距離で当たらないようにするためにクールタイムを設定（０除算回避）
	    m_bCollision = CollisionSquare();
	}
	else
	{
		m_bCollision = false;//当たり判定を行うまでのクールタイムがまだあるので当たっていない
		m_nCoolTime--;       //クールタイムを減らす
	}
}
//===================================================================================================================================

//=================================================================
//描画処理
//=================================================================
void CWireHead::Draw()
{
	CObjectX::Draw();//オブジェクトX描画処理
}
//===================================================================================================================================

//=================================================================
//死亡フラグ設定処理
//=================================================================
void CWireHead::SetDeath()
{
	CObjectX::SetDeath();//オブジェクトX死亡フラグ設定処理
}
//===================================================================================================================================

//=================================================================
//生成処理
//=================================================================
CWireHead* CWireHead::Create(D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, D3DXVECTOR3 Move, D3DXVECTOR3 Scale, int nLife)
{
	CWireHead* pWireHead = DBG_NEW CWireHead();                //ワイヤーの頭を生成
	pWireHead->Init();                                         //初期化処理
	pWireHead->GetPosInfo().SetPos(Pos);		               //位置の設定
	pWireHead->GetPosInfo().SetSupportPos(Pos);	               //支点位置の設定
	pWireHead->GetRotInfo().SetRot(Rot);		               //向きの設定
	pWireHead->GetMoveInfo().SetMove(Move);		               //移動量の設定
	pWireHead->GetSizeInfo().SetScale(Scale);	               //拡大率の設定
	pWireHead->GetLifeInfo().SetLife(nLife);	               //体力の設定
	pWireHead->GetLifeInfo().SetMaxLife(nLife);	               //最大体力の設定
	pWireHead->GetLifeInfo().SetAutoSubLife(true);             //体力を自動的に減らす
	

	int nIdx = CManager::GetObjectXInfo()->Regist("data\\MODEL\\Wire_Head\\Wire_Head_000.x");//モデル情報を登録

    //モデル情報を割り当てる
	pWireHead->BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(nIdx),
		CManager::GetObjectXInfo()->GetBuffMat(nIdx),
		CManager::GetObjectXInfo()->GetdwNumMat(nIdx),
		CManager::GetObjectXInfo()->GetTexture(nIdx),
		CManager::GetObjectXInfo()->GetColorValue(nIdx));

	pWireHead->SetSize();//モデルのサイズを設定する

	return pWireHead;
}
//===================================================================================================================================

//=================================================================
//正方形の当たり判定を行う
//=================================================================
bool CWireHead::CollisionSquare()
{
	for (int nCntPri = 0; nCntPri < m_nMAXPRIORITY; nCntPri++)
	{//オブジェクトリストを検索する
		CObject* pObj = GetTopObject(nCntPri);//リストの先頭オブジェクトを取得する
		while (pObj != nullptr)
		{//オブジェクトがnullptrになるまで繰り返す
			CObject* pNext = pObj->GetNextObject();//次のオブジェクトを取得
			CObject::TYPE type = pObj->GetType();//オブジェクトの種類を取得
			if (type == CObject::TYPE::BGMODEL || type == CObject::TYPE::ENEMY || type == CObject::TYPE::BLOCK)
			{//種類が背景モデル又は敵又はブロックだったら
				CObjectX* pObjX = static_cast<CObjectX*>(pObj);//オブジェクトXにキャスト
				if (CCollision::CollisionSquare(GetPosInfo().GetPos(), GetSizeInfo().GetVtxMax(), GetSizeInfo().GetVtxMin(), pObjX->GetPosInfo().GetPos(), pObjX->GetSizeInfo().GetVtxMax(), pObjX->GetSizeInfo().GetVtxMin()) == true)
				{//正方形の当たり判定を行う
					m_CollisionObjType = type;//衝突したので、衝突したオブジェクトのタイプを格納
					return true;              //衝突済みなのでもう用済み
				}
			}
			pObj = pNext;//リストを次に進める
		}
	}
	return false;
}
//===================================================================================================================================