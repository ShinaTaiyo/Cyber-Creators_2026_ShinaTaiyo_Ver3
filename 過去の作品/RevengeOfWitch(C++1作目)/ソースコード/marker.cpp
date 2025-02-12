//================================================================================
//
//６月２５日：ワープマーカーを実装[marker.cpp]
//Author:ShinaTaiyo
//
//================================================================================

//==============================
//インクルード
//==============================
#include "marker.h"
#include "manager.h"
#include "bullet.h"
#include "particle.h"
#include "texture.h"
#include "game.h"
#include "edit.h"
#include "manager.h"
#include "objectXInfo.h"
#include "stagemanager.h"
//=================================================

//==============================
//静的メンバ初期化
//==============================
int CMarker::m_nNumFile = CMarker::MARKERTYPE_MAX;//ファイルの数
const char* CMarker::m_MARKER_FILENAME[CMarker::MARKERTYPE_MAX] =
{ "data\\MODEL\\Marker\\WarpMarker_000.x"};//マーカーのモデルのパス
//=================================================

//==============================
//コンストラクタ
//==============================
CMarker::CMarker()
{

}
//=================================================

//==============================
//デストラクタ
//==============================
CMarker::~CMarker()
{

}
//=================================================

//==============================
//初期化処理
//==============================
HRESULT CMarker::Init()
{
	//===========================
	//２Dオブジェクト初期化
	//===========================
	if (FAILED(CObjectX::Init()))
	{
		return E_FAIL;
	}
	//====================================

	//===========================
	//クラスの変数初期化
	//===========================
	m_nCntTime = 0;          //出現してからの時間を測る
	m_Aim = NULL_VECTOR3;    //ベクトル
	m_fTotalAim = 0.0f;      //総合ベクトル
	m_nWarpMapNum = 0;       //ワープするマップの番号を格納する
	m_WarpPos = NULL_VECTOR3;//ワープする位置を決定する
	for (int nCnt = 0; nCnt < m_nWARPMAPDIVISION; nCnt++)
	{
		m_pWarpMapDispNum[nCnt] = nullptr;
	}
	//====================================

	return S_OK;
}
//=================================================

//==============================
//終了処理
//==============================
void CMarker::Uninit()
{
	CObjectX::Uninit();
}
//=================================================

//==============================
//更新処理
//==============================
void CMarker::Update()
{
	//==================================
	//オブジェクト２Dから情報を取得
	//==================================
	D3DXVECTOR3 Pos = CObjectX::GetPos();
	D3DXVECTOR3 Rot = CObjectX::GetRot();
	//===========================================================

	//==================================
	//座標更新
	//==================================
	Rot += ONE_VECTOR3 * 0.1f;
	SetRot(Rot);
	//===========================================================

	//==================================
	//Xオブジェクト更新
	//==================================
	CObjectX::Update();
	//===========================================================

	//==================================
	//当たり判定を取る
	//==================================
	Collision();
	//===========================================================
}
//=================================================

//==============================
//描画処理
//==============================
void CMarker::Draw()
{
	CObjectX::Draw();

	for (int nCnt = 0; nCnt < m_nWARPMAPDIVISION; nCnt++)
	{
		if (m_pWarpMapDispNum[nCnt] != nullptr)
		{
			m_pWarpMapDispNum[nCnt]->ExtraDraw();
		}
	}
}
//=================================================

//==============================
//死亡フラグを設定
//==============================
void CMarker::SetDeath()
{
	for (int nCnt = 0; nCnt < m_nWARPMAPDIVISION; nCnt++)
	{
		if (m_pWarpMapDispNum[nCnt] != nullptr)
		{
			m_pWarpMapDispNum[nCnt]->SetUseDeath(true);
			m_pWarpMapDispNum[nCnt]->SetDeath();
			m_pWarpMapDispNum[nCnt] = nullptr;
		}
	}
	CObject::SetDeath();
}
//=================================================

//===============================
//敵の生成処理
//===============================
CMarker* CMarker::Create(MARKERTYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 Scale, D3DXVECTOR3 WarpPos, int nMapNum)
{
	CMarker* pMarker = DBG_NEW CMarker;   //敵を生成
	CNumber3D* pNumber3D = nullptr;//数値取得用
	bool bSuccess = pMarker->CObject::GetCreateSuccess();
	int nIdx = 0;//モデルのインデックス
	if (bSuccess == true)
	{
		if (pMarker != nullptr)
		{
			pMarker->Init();         //初期化処理
			pMarker->SetUseDeath(true);                                                                  //死亡フラグを発動するかどうかを設定する
			pMarker->SetUseBlockCollision(false);
			pMarker->m_type = type;  //弾の種類
			pMarker->SetUseGravity(false);
			pMarker->m_nWarpMapNum = nMapNum;//ワープするマップの番号を格納する
			pMarker->m_WarpPos = WarpPos;    //ワープする位置を決定する
			pMarker->CObjectX::SetPos(pos);                                                                                  //オブジェクト２Ｄの位置を設定
			pMarker->SetPosOld(pos);                                                                                         //1f前の位置を設定
			pMarker->SetSupportPos(pos);                                                                                     //設置位置
			pMarker->CObject::SetType(CObject::TYPE_MARKER);                                                                  //オブジェクトの種類を決める
			pMarker->CObjectX::SetScale(Scale);                                                                              //オブジェクトの拡大率
			pMarker->SetFormarScale(Scale);                                                                                  //元の拡大率を設定する
			pMarker->SetAutoSubLife(false);//自動敵に体力を減らすかどうか
			nIdx = CManager::GetObjectXInfo()->Regist(CObjectX::OBJECTXTYPE_MARKER, m_MARKER_FILENAME[0]);
			pMarker->CObjectX::BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(CObjectX::OBJECTXTYPE_MARKER, nIdx),
				CManager::GetObjectXInfo()->GetBuffMat(CObjectX::OBJECTXTYPE_MARKER, nIdx),
				CManager::GetObjectXInfo()->GetdwNumMat(CObjectX::OBJECTXTYPE_MARKER, nIdx),
				CManager::GetObjectXInfo()->GetTexture(CObjectX::OBJECTXTYPE_MARKER, nIdx),
				CManager::GetObjectXInfo()->GetColorValue(CObjectX::OBJECTXTYPE_MARKER, nIdx));                       //モデル情報を割り当てる

			pMarker->CObjectX::SetObjXType(CObjectX::OBJECTXTYPE_MARKER);                                                    //オブジェクトXのタイプを設定
			pMarker->CObjectX::SetTypeNum((int)(type));                                                                      //オブジェクトXごとのタイプ番号を設定
			pMarker->SetSize();                                                                                              //Xオブジェクトのサイズを設定する
			pMarker->CObjectX::SetRotType(CObjectX::ROTTYPE::ROTTYPE_NORMAL);                                                //向きの種類を設定
			pMarker->SetManagerType((int)(CStageManager::MANAGEROBJECT_MARKER));                                             //マネージャーで呼び出す時の種類を設定

			if (type == MARKERTYPE00_WARP)
			{
				int nCopy = pMarker->m_nWarpMapNum;
				int nNum = 0;
				for (int nCnt = 0; nCnt < m_nWARPMAPDIVISION; nCnt++)
				{
					pMarker->m_pWarpMapDispNum[nCnt] = CNumber3D::Create(40.0f, 40.0f,
						D3DXVECTOR3(pMarker->GetPos().x - 40.0f * nCnt, pMarker->GetPos().y + 50.0f, pMarker->GetPos().z),
						NULL_VECTOR3,
						NORMAL_COL);
					nNum = nCopy % 10;
					pMarker->m_pWarpMapDispNum[nCnt]->SetAnim(nNum);
					nCopy /= 10;
				}
			}
		}
		else
		{//オブジェクトに空きがなかったので破棄する
			delete pMarker;
			pMarker = nullptr;
			return nullptr;
		}
	}
	return pMarker;
}
//=====================================================================================================================================

//===================================================================
//当たり判定を取る
//===================================================================
void CMarker::Collision()
{
	//==================================
    //オブジェクトXから情報を取得
    //==================================
	D3DXVECTOR3 Pos = CObjectX::GetPos();                      //オブジェクトの位置を取得
	D3DXVECTOR3 Size = CObjectX::GetSize();                    //オブジェクトのサイズを取得
	D3DXVECTOR3 VtxMax = GetVtxMax();                          //オブジェクトの最大頂点を取得する
	D3DXVECTOR3 VtxMin = GetVtxMin();                          //オブジェクトの最小頂点を取得する
	D3DXVECTOR3 ComparisonPos = NULL_VECTOR3;                  //プレイヤーの位置代入用
	D3DXVECTOR3 ComparisonVtxMax = NULL_VECTOR3;               //判定対象の最大頂点
	D3DXVECTOR3 ComparisonVtxMin = NULL_VECTOR3;               //判定対象の最小頂点
	CPlayer* pPlayer = nullptr;                                //プレイヤークラス取得用
	int nMapNum = 0;                                           //ワープマーカーに保存されたマップ番号格納用（ReleaseAllが呼ばれるので、保存した番号が消えてしまうから）
	D3DXVECTOR3 WarpPos = NULL_VECTOR3;                        //ワープマーカーに保存されたマップ番号格納用（ReleaseAllが呼ばれるので、保存した番号が消えてしまうから）
	CObject* pObj = nullptr;                                   //オブジェクト格納用
	//===========================================================

	//==================================
	//アイテムの当たり判定
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

			//オブジェクトの種類が敵だったら
			switch (type)
			{
			case CObject::TYPE::TYPE_PLAYER:
				pPlayer = (CPlayer*)pObj;                            //オブジェクトを敵の型にダウンキャスト
				ComparisonPos = pPlayer->GetPos();                   //位置を取得
				ComparisonVtxMax = pPlayer->GetVtxMax();             //最大頂点を取得
				ComparisonVtxMin = pPlayer->GetVtxMin();             //最小頂点を取得
				if (Pos.x + VtxMin.x  <= ComparisonPos.x + ComparisonVtxMax.x &&
					Pos.x + VtxMax.x  >= ComparisonPos.x + ComparisonVtxMin.x &&
					Pos.y + VtxMin.y  <= ComparisonPos.y + ComparisonVtxMax.y &&
					Pos.y + VtxMax.y  >= ComparisonPos.y + ComparisonVtxMin.y &&
					Pos.z + VtxMin.z  <= ComparisonPos.z + ComparisonVtxMax.x &&
					Pos.z + VtxMax.z  >= ComparisonPos.z + ComparisonVtxMin.x)
				{//ワープマーカーに当たったら
					if (m_type == MARKERTYPE00_WARP)
					{
						nMapNum = m_nWarpMapNum;                                //ワープ先のマップ番号を格納
						WarpPos = m_WarpPos;                                    //ワープ先の位置を格納
						if (CScene::GetMode() == CScene::MODE_GAME)
						{
							CGame::GetPlayer()->SetPos(WarpPos);              //ワープマーカーに記録された位置にワープ
							CGame::GetPlayer()->SetPosOld(WarpPos);       
							CGame::GetPlayer()->SetReSpawnPos(WarpPos); //セーブポイントを設定する
							CGame::GetPlayer()->ResetRespawnCnt();
							ReleaseAll();
							CGame::GetStageManager()->LoadMapTxt(nMapNum);//ワープマーカーに記録されたマップ番号を読み込む
						}
						else if (CScene::GetMode() == CScene::MODE_EDIT)
						{
							CEdit::GetPlayer()->SetPos(WarpPos);              //ワープマーカーに記録された位置にワープ
							CEdit::GetPlayer()->SetPosOld(WarpPos);
							CEdit::GetStageManager()->SaveMapTxt(CEdit::GetStageManager()->GetMapIndex());//今のマップの情報を保存する
							CEdit::GetStageManager()->LoadMapTxt(nMapNum);//ワープマーカーに記録されたマップ番号を読み込む
						}
					}
				}
				break;
			default:
				//リストを次に進める
				pObj = pNext;
				continue;
				break;
			}
			//リストを次に進める
			pObj = pNext;
		}
	}

}
//=====================================================================================================================================

//===================================================================
//数字を消す
//===================================================================
void CMarker::DeleteNum()
{

}
//=====================================================================================================================================