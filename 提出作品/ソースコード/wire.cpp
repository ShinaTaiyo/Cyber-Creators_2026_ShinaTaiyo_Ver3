//===================================================================================================================
//
// ２０２５年６月１６日：計算用クラスを名前空間に変更[wire.cpp]
// Author:ShinaTaiyo
//
//===================================================================================================================

//*******************************************************************************************************************
// インクルード
//*******************************************************************************************************************
#include "wire.h"      // ワイヤー
#include "texture.h"   // テクスチャ
#include "renderer.h"  // レンダラー
#include "wire_head.h" // ワイヤーヘッド
#include "player.h"    // プレイヤー
#include "manager.h"   // マネージャー
#include "debugtext.h" // デバッグ表示
#include "calculation.h" // 計算用

//*******************************************************************************************************************
// 静的メンバ宣言
//*******************************************************************************************************************

// ワイヤーのテクスチャファイル名
const string CWire::s_WIRE_FILENAME[static_cast<int>(CWire::WIRETYPE::MAX)] =
{
	"data\\TEXTURE\\Wire\\LakeField_000.png",
	"data\\TEXTURE\\Wire\\Rope_000.png"
};

//=====================================================================================================================
// 
// publicメンバ 
//
//=====================================================================================================================

//=====================================================================================================================
// コンストラクタ
//=====================================================================================================================
CWire::CWire(WIRETYPE WireType, float fRadius, float fHeight,int nNumDivsionXZ,
	D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, 
	int nNumDivisionY, int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : 
	CMeshCylinder(fRadius,fHeight,nNumDivsionXZ,nNumDivisionY,Pos,Rot,nPri,bUseintPri,type,ObjType),
	m_Type(WireType),
	m_bUseUpdate(true),
	m_pWireHead(nullptr),
	m_pPlayer(nullptr)
{
	// 何もなし
}

//=====================================================================================================================
// デストラクタ
//=====================================================================================================================
CWire::~CWire()
{
	// 何もなし
}

//=====================================================================================================================
// 初期化処理
//=====================================================================================================================
HRESULT CWire::Init()
{
	//ワイヤーヘッドを生成
	m_pWireHead = CWireHead::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 120);//生成処理
	m_pWireHead->GetLifeInfo().SetAutoSubLife(false);//体力を自動的に減らさない
	m_pWireHead->SetUseDeath(false);                 //死亡フラグを使用しない
	m_pWireHead->GetDrawInfo().SetUseDraw(true);     //描画をする

	CMeshCylinder::Init();//メッシュシリンダーの初期化処理

	return S_OK;
}

//=====================================================================================================================
// 終了処理
//=====================================================================================================================
void CWire::Uninit()
{
	CMeshCylinder::Uninit(); // メッシュシリンダーの終了処理
}

//=====================================================================================================================
// 更新処理
//=====================================================================================================================
void CWire::Update()
{
	GenerateCylindricalMeshOnWire(); // ワイヤー上にメッシュを展開

	CMeshCylinder::Update(); // メッシュシリンダーの更新処理
}

//=====================================================================================================================
// 描画処理
//=====================================================================================================================
void CWire::Draw()
{
	LPDIRECT3DDEVICE9 pDevice; // デバイスへのポインタ

	// デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;          // 計算用マトリックス
	D3DXMATRIX& mtxWorld = GetMtxWorld(); // ワールドマトリックス
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBufferPointer(); // 頂点バッファ
	LPDIRECT3DINDEXBUFFER9 pIdxBuff = GetIdxBufferPointer();  // インデックスバッファ
	LPDIRECT3DTEXTURE9 pTexture = GetTexture(); // テクスチャへのポインタ
	const int& nNumPolygon = GetNumPolygon();   // ポリゴン数
	const int& nNumVtx = GetNumVtx(); // 頂点数

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,0.0f,0.0f,0.0f);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans,0.0f,0.0f,0.0f);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	//両面を描画する
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, pVtxBuff, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(pIdxBuff);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, pTexture);

	// 描画するなら
	if (GetUseDraw() == true)
	{
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, nNumVtx, 0, nNumPolygon);
	}

	//片面だけ描画する
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//=====================================================================================================================
// 死亡フラグ設定処理
//=====================================================================================================================
void CWire::SetDeath()
{
	// 死亡フラグを使用するなら
	if (GetUseDeath())
	{
		// ワイヤーの頭終了
		if (m_pWireHead != nullptr)
		{
			m_pWireHead->SetUseDeath(true); // 死亡フラグを使用する
			m_pWireHead->SetDeath();        // 死亡フラグを設定する
			m_pWireHead = nullptr;          // ポインタを初期化
		}

		// プレイヤー終了
		if (m_pPlayer != nullptr)
		{
			m_pPlayer = nullptr; //プレイヤーへのポインタ初期化（ワイヤーからは破棄しません。）
		}
	}

	CMeshCylinder::SetDeath(); // メッシュシリンダーの死亡フラグ設定処理
}

//=====================================================================================================================
// ワイヤーヘッドへのポインタ取得
//=====================================================================================================================
CWireHead* CWire::GetWireHead()
{
	return m_pWireHead;
}

//=====================================================================================================================
// プレイヤーへのポインタ設定
//=====================================================================================================================
void CWire::SetPlayerPointer(CPlayer* pPlayer)
{
	m_pPlayer = pPlayer;
}

//=====================================================================================================================
// 更新するかどうか設定
//=====================================================================================================================
void CWire::SetUseUpdate(bool bUse)
{
	m_bUseUpdate = bUse;
}

//=====================================================================================================================
// 生成処理
//=====================================================================================================================
CWire* CWire::Create(WIRETYPE Type, D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, float fRadius, float fHeight, int nNumDivisionXZ, int nNumDivisionY)
{
	CTexture* pTexture = CManager::GetTexture(); // テクスチャ情報へのポインタ
	CWire* pWire = DBG_NEW CWire(Type, fRadius, fHeight, nNumDivisionXZ, Pos, Rot, nNumDivisionY); // インスタンス生成
	pWire->Init(); // 初期化処理  
	pWire->SetTextureIndex(pTexture->Regist(s_WIRE_FILENAME[static_cast<int>(Type)])); // テクスチャを登録し、テクスチャ番号を設定
	pWire->BindTexture(pTexture->GetAddress(pWire->GetTextureIndex())); // テクスチャを割り当てる
	return pWire;
}

//=====================================================================================================================
// ワイヤー状にメッシュを展開する処理
//=====================================================================================================================
void CWire::GenerateCylindricalMeshOnWire()
{
	// === 処理に使用する情報を宣言、初期化 ===

	const D3DXVECTOR3& Pos = GetPos(); // 位置
	const int& nNumDivisionXZ = GetNumDivisionXZ(); // XZ分割数
	const int& nNumDivisionY = GetNumDivisionY(); // Y分割数
	const int& nNumVtx = GetNumVtx();   // 頂点数
	const int& nNumIdx = GetNumIdx();   // インデックス数
	const float& fRadius = GetRadius(); // 半径
	const float& fHeight = GetHeight(); // 高さ

	// === ワイヤー状にメッシュを展開する処理を開始 ===

	// ワイヤーの頭とプレイヤーの距離を求める
	float fLength = Calculation::Length::ToGoalPos(m_pWireHead->GetPosInfo().GetPos(), m_pPlayer->GetPosInfo().GetPos());
	SetHeight(fLength); // 上の行で求めた距離を設定する（高さ = 長さ)
	D3DXVECTOR3 Dir = m_pWireHead->GetPosInfo().GetPos() - m_pPlayer->GetPosInfo().GetPos(); // ワイヤーの頭とプレイヤーのベクトルを求める
	D3DXVec3Normalize(&Dir, &Dir); // ベクトルを正規化する
	VERTEX_3D* pVtx; // 3D頂点情報へのポインタ
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBufferPointer(); // 頂点バッファへのポインタを取得

	//頂点バッファをロックし、頂点情報へのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 Up = { 0.0f,1.0f,0.0f }; // 基準となる上方向ベクトル（外積の処理で使う）

	// 平行に近い場合は別の基準(X方向)（どちらの基準ベクトルでも結果に大きな差はなし）
	if (std::abs(Dir.x * Up.x + Dir.y * Up.y + Dir.z * Up.z) > 0.99f) {
		Up = { 1.0f, 0.0f, 0.0f };
	}

	// 更新するかつプレイヤーへのポインタが存在するならワイヤー上に頂点を展開する処理を開始
	if (m_bUseUpdate == true && m_pPlayer != nullptr)
	{
		// === 処理に使用する情報を宣言、初期化 ===
		int nCntArray = 0;     // 配列数をカウント
		float fRatioXZ = 0.0f; // XZ方向のカウント数の割合
		float fRatioY = 0.0f;  // Y方向のカウント数の割合
		D3DXVECTOR3 V1 = { 0.0f,0.0f,0.0f }; // ベクトル１
		D3DXVECTOR3 V2 = { 0.0f,0.0f,0.0f }; // ベクトル２
		float fMeasureAngle = 0.0f;  // 角度計算用
		float fMeasureX = 0.0f; // X方向の角度
		float fMeasureY = 0.0f; // Y方向の角度

		// 円を展開する平面を求める基準ベクトルを計算する
		D3DXVec3Cross(&V1, &Dir, &Up); // X方向
		D3DXVec3Normalize(&V1, &V1);   // ベクトル１を正規化
		D3DXVec3Cross(&V2, &Dir, &V1); // Y方向
		D3DXVec3Normalize(&V2, &V2);   // ベクトル２を正規化

		// Y方向の頂点
		for (int nCntVtxY = 0; nCntVtxY < nNumDivisionY; nCntVtxY++)
		{
			// Y方向頂点割合
			fRatioY = (1.0f / (nNumDivisionY - 1)) * nCntVtxY;

			// X方向の頂点（重なり部分があるので＋１）
			for (int nCntVtxXZ = 0; nCntVtxXZ < nNumDivisionXZ + 1; nCntVtxXZ++)
			{
				// X方向頂点割合
				fRatioXZ = (1.0f / (nNumDivisionXZ)) * nCntVtxXZ;

				// 最初の周なので底面の中心点を設定する
				if (nCntVtxY == 0 && nCntVtxXZ == 0)
				{
					pVtx[nCntArray].pos = m_pPlayer->GetPosInfo().GetPos();
					nCntArray++;
				}

				// 最初の周で基準点を決める（9,8,7,6,5,4,3,2,1)
				if (nCntVtxY == 0)
				{
					fMeasureAngle = (D3DX_PI * 2) * fRatioXZ;  // 角度の割合を計算する
					fMeasureX = fRadius * sinf(fMeasureAngle); // 上を正としているので、sinfはX
					fMeasureY = fRadius * cosf(fMeasureAngle); // 上を正としているので、cosfはY

					// 円周状の比に外積で求めたベクトルを掛けてワイヤーを表現
					pVtx[nCntArray].pos = m_pPlayer->GetPosInfo().GetPos() + V1 * fMeasureX + V2 * fMeasureY;
				}
				// 基準点に対して軌跡風に頂点を代入していく(18 = 27)
				else
				{
					// 円を目的のベクトルを基準に平面に展開し基準点は求まったので、あとはそのベクトルに向かって値を足していくだけ
					D3DXVECTOR3 AdjustPos = Dir * fLength * fRatioY;
					pVtx[nCntArray].pos = pVtx[1 + nCntVtxXZ].pos + AdjustPos;
				}

				//配列カウント
				nCntArray++;

				// 配列外チェック
				if (nCntArray >= nNumVtx)
				{
					assert(false && "配列外アクセス");
				}

				// 配列外チェック
				if (nCntArray < 0 || nCntArray >= nNumVtx)
				{
					assert(false && "配列外アクセス");
				}

				// 最後の頂点なので、中心点をワイヤーの頭に設定
				if (nCntVtxY == nNumDivisionY - 1 && nCntVtxXZ == nNumDivisionXZ)
				{
					pVtx[nCntArray].pos = m_pWireHead->GetPosInfo().GetPos(); // 底面の中心に位置を設定
				}
			}
		}
	}

	// 頂点バッファをアンロックする 
	pVtxBuff->Unlock();
}
