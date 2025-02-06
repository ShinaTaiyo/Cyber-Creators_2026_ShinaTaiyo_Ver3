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
	CObjectX::Init();
	return S_OK;
}
//===================================================================================================================================

//=================================================================
//終了処理
//=================================================================
void CWireHead::Uninit()
{
	CObjectX::Uninit();
}
//===================================================================================================================================

//=================================================================
//更新処理
//=================================================================
void CWireHead::Update()
{
	CObjectX::Update();

	CManager::GetDebugText()->PrintDebugText("ワイヤーヘッドの位置：%f %f %f\n", GetPosInfo().GetPos().x, GetPosInfo().GetPos().y, GetPosInfo().GetPos().z);

	if (m_nCoolTime == 0)
	{//０距離で当たらないようにするためにクールタイムを設定（０除算回避）
	    m_bCollision = CollisionSquare();
	}
	else
	{
		m_bCollision = false;
		m_nCoolTime--;
	}
}
//===================================================================================================================================

//=================================================================
//描画処理
//=================================================================
void CWireHead::Draw()
{
	CObjectX::Draw();
}
//===================================================================================================================================

//=================================================================
//死亡フラグ設定処理
//=================================================================
void CWireHead::SetDeath()
{
	CObjectX::SetDeath();
}
//===================================================================================================================================

//=================================================================
//生成処理
//=================================================================
CWireHead* CWireHead::Create(D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, D3DXVECTOR3 Move, D3DXVECTOR3 Scale, int nLife)
{
	CWireHead* pWireHead = DBG_NEW CWireHead();
	pWireHead->Init();
	pWireHead->GetPosInfo().SetPos(Pos);
	pWireHead->GetPosInfo().SetSupportPos(Pos);
	pWireHead->GetRotInfo().SetRot(Rot);
	pWireHead->GetMoveInfo().SetMove(Move);
	pWireHead->GetSizeInfo().SetScale(Scale);
	pWireHead->GetLifeInfo().SetLife(nLife);
	pWireHead->GetLifeInfo().SetMaxLife(nLife);
	pWireHead->GetLifeInfo().SetAutoSubLife(true);
	//モデル情報設定
	int nIdx = CManager::GetObjectXInfo()->Regist("data\\MODEL\\Wire_Head\\Wire_Head_000.x");

	//モデル情報を割り当てる
	pWireHead->BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(nIdx),
		CManager::GetObjectXInfo()->GetBuffMat(nIdx),
		CManager::GetObjectXInfo()->GetdwNumMat(nIdx),
		CManager::GetObjectXInfo()->GetTexture(nIdx),
		CManager::GetObjectXInfo()->GetColorValue(nIdx));

	pWireHead->SetSize();

	return pWireHead;
}
//===================================================================================================================================

//=================================================================
//正方形の当たり判定を行う
//=================================================================
bool CWireHead::CollisionSquare()
{
	for (int nCntPri = 0; nCntPri < m_nMAXPRIORITY; nCntPri++)
	{
		CObject* pObj = GetTopObject(nCntPri);
		while (pObj != nullptr)
		{
			CObject* pNext = pObj->GetNextObject();
			CObject::OBJECTTYPE ObjType = pObj->GetObjectType();
			CObject::TYPE type = pObj->GetType();
			if (type == CObject::TYPE::BGMODEL || type == CObject::TYPE::ENEMY || type == CObject::TYPE::BLOCK)
			{
				CObjectX* pObjX = static_cast<CObjectX*>(pObj);
				if (CCollision::CollisionSquare(GetPosInfo().GetPos(), GetSizeInfo().GetVtxMax(), GetSizeInfo().GetVtxMin(), pObjX->GetPosInfo().GetPos(), pObjX->GetSizeInfo().GetVtxMax(), pObjX->GetSizeInfo().GetVtxMin()) == true)
				{
					CManager::GetDebugText()->PrintDebugText("ワイヤーヘッドの当たり判定成功！\n");
					m_CollisionObjType = type;//衝突したので、衝突したオブジェクトのタイプを格納
					return true;
				}
			}
			pObj = pNext;
		}
	}

	CManager::GetDebugText()->PrintDebugText("正方形の当たり判定失敗！\n");

	return false;
}
//===================================================================================================================================