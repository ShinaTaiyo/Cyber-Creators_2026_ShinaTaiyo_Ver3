//===================================================================================================================
//
//１１月１７日：ワイヤーを作る[wire.cpp]
//Author:ShinaTaiyo
//
//===================================================================================================================

//===============================================================
//インクルード
//===============================================================
#include "wire.h"
#include "texture.h"
#include "renderer.h"
#include "player.h"
#include "manager.h"
#include "particle.h"
#include "debugtext.h"
#include "calculation.h"
//===================================================================================================================

//===============================================================
//デストラクタ
//===============================================================
const string CWire::s_WIRE_FILENAME[static_cast<int>(CWire::WIRETYPE::MAX)] =
{
	"data\\TEXTURE\\Wire\\LakeField_000.png",
	"data\\TEXTURE\\Wire\\Rope_000.png"
};
//===================================================================================================================

//===============================================================
//コンストラクタ
//===============================================================
CWire::CWire(WIRETYPE WireType, float fRadius, float fHeight,int nNumDivsionXZ, 
	D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, 
	int nNumDivisionY, int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CMeshCylinder(fRadius,fHeight,nNumDivsionXZ,nNumDivisionY,
		Pos,Rot,
		nPri,bUseintPri,type,ObjType),m_Type(WireType),m_bUseUpdate(true),m_pWireHead(nullptr),m_pPlayer(nullptr)
{}
//===================================================================================================================

//===============================================================
//デストラクタ
//===============================================================
CWire::~CWire()
{

}
//===================================================================================================================

//===============================================================
//初期化処理
//===============================================================
HRESULT CWire::Init()
{
	CMeshCylinder::Init();//メッシュシリンダーの初期化処理

	//ワイヤーヘッドを生成
	m_pWireHead = CWireHead::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 120);//生成処理
	m_pWireHead->GetLifeInfo().SetAutoSubLife(false);//体力を自動的に減らさない
	m_pWireHead->SetUseDeath(false);                 //死亡フラグを使用しない
	m_pWireHead->GetDrawInfo().SetUseDraw(true);     //描画をする

	return S_OK;
}
//===================================================================================================================

//===============================================================
//終了処理
//===============================================================
void CWire::Uninit()
{
	CMeshCylinder::Uninit();//メッシュシリンダーの終了処理
}
//===================================================================================================================

//===============================================================
//更新処理
//===============================================================
void CWire::Update()
{
	CMeshCylinder::Update();                                                                                             //メッシュシリンダーの更新処理
	const D3DXVECTOR3& Pos = GetPos();                                                                                   //位置
	const int& nNumDivisionXZ = GetNumDivisionXZ();                                                                      //XZ分割数
	const int& nNumDivisionY = GetNumDivisionY();                                                                        //Y分割数
	const int& nNumVtx = GetNumVtx();                                                                                    //頂点数
	const int& nNumIdx = GetNumIdx();                                                                                    //インデックス数
	const float& fRadius = GetRadius();                                                                                  //半径
	const float& fHeight = GetHeight();                                                                                  //高さ
	float fLength = CCalculation::CalculationLength(m_pWireHead->GetPosInfo().GetPos(),m_pPlayer->GetPosInfo().GetPos());//ワイヤーの頭とプレイヤーの距離を求める
	SetHeight(fLength);                                                                                                  //上の行で求めた距離を設定する
	D3DXVECTOR3 Dir = m_pWireHead->GetPosInfo().GetPos() - m_pPlayer->GetPosInfo().GetPos();                             //ワイヤーの頭とプレイヤーのベクトルを求める
	D3DXVec3Normalize(&Dir, &Dir);                                                                                       //ベクトルを正規化する
	VERTEX_3D* pVtx;                                                                                                     //3D頂点情報へのポインタ
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBufferPointer();                                                            //頂点バッファへのポインタを取得
	//頂点バッファをロックし、頂点情報へのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 Up = { 0.0f,1.0f,0.0f };//基準となる上方向ベクトル（外積の処理で使う）

	if (std::abs(Dir.x * Up.x + Dir.y * Up.y + Dir.z * Up.z) > 0.99f) {
		Up = { 1.0f, 0.0f, 0.0f }; // 平行に近い場合は別の基準(X方向)
	}

	if (m_bUseUpdate == true && m_pPlayer != nullptr)
	{
		int nCntArray = 0;                                        //配列数をカウント
		float fRatioXZ = 0.0f;                                    //XZ方向のカウント数の割合
		float fRatioY = 0.0f;                                     //Y方向のカウント数の割合
		D3DXVECTOR3 V1 = { 0.0f,0.0f,0.0f };                      //ベクトル１
		D3DXVECTOR3 V2 = { 0.0f,0.0f,0.0f };                      //ベクトル２
		float fMeasureAngle = 0.0f;                               //角度計算用
		float fMeasureX = 0.0f;		                              //X方向の角度せ
		float fMeasureY = 0.0f;		                              

		//円を展開する平面を求める基準ベクトルを計算する
		D3DXVec3Cross(&V1, &Dir, &Up);//X方向
		D3DXVec3Normalize(&V1, &V1);//ベクトル１を正規化
		D3DXVec3Cross(&V2,&Dir,&V1);//Y方向
		D3DXVec3Normalize(&V2, &V2);//ベクトル２を正規化

		//頂点の位置の設定を開始
		for (int nCntVtxY = 0; nCntVtxY < nNumDivisionY; nCntVtxY++)
		{//Y方向のUVはそのまま使う
			fRatioY = (1.0f / (nNumDivisionY - 1)) * nCntVtxY;
			for (int nCntVtxXZ = 0; nCntVtxXZ < nNumDivisionXZ + 1; nCntVtxXZ++)
			{//X方向のUVは重なる頂点があるので、+ 1
				fRatioXZ = (1.0f / (nNumDivisionXZ)) * nCntVtxXZ;
				if (nCntVtxY == 0 && nCntVtxXZ == 0)
				{//最初の周なので底面の中心点を設定する
					pVtx[nCntArray].pos = m_pPlayer->GetPosInfo().GetPos();
					nCntArray++;
				}

				if (nCntVtxY == 0)
				{//最初の周で基準点を決める（9,8,7,6,5,4,3,2,1)
					fMeasureAngle = (D3DX_PI * 2) * fRatioXZ;//角度の割合を計算する
					fMeasureX = fRadius * sinf(fMeasureAngle);//上を正としているので、sinfはX
					fMeasureY = fRadius * cosf(fMeasureAngle);//上を正としているので、cosfはY
					pVtx[nCntArray].pos = m_pPlayer->GetPosInfo().GetPos() + V1 * fMeasureX + V2 * fMeasureY;//sinfとcosfで平面のX方向とY方向に進む比を求めたので合っている
				}
				else
				{//基準点に対して軌跡風に頂点を代入していく(18 = 27)
					//円を目的のベクトルを基準に平面に展開し基準点は求まったので、あとはそのベクトルに向かって値を足していくだけ
					D3DXVECTOR3 AdjustPos = Dir * fLength * fRatioY;
					pVtx[nCntArray].pos = pVtx[1 + nCntVtxXZ].pos + AdjustPos;
				}


				//配列カウント
				nCntArray++;

				if (nCntArray >= nNumVtx)
				{//配列外チェック
					assert("配列外アクセス");
				}

				if (nCntArray < 0 || nCntArray >= nNumVtx)
				{//配列外アクセスチェック
					assert(false);
				}

				if (nCntVtxY == nNumDivisionY - 1 && nCntVtxXZ == nNumDivisionXZ)
				{//最後の頂点なので、中心点をワイヤーの頭に設定
					pVtx[nCntArray].pos = m_pWireHead->GetPosInfo().GetPos();//底面の中心に位置を設定
				}
			}
		}
	}

	//頂点バッファをアンロックする 
	pVtxBuff->Unlock();

}
//===================================================================================================================

//===============================================================
//描画処理
//===============================================================
void CWire::Draw()
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;//計算用マトリックス
	D3DXMATRIX& mtxWorld = GetMtxWorld();
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBufferPointer();
	LPDIRECT3DINDEXBUFFER9 pIdxBuff = GetIdxBufferPointer();
	LPDIRECT3DTEXTURE9 pTexture = GetTexture();
	const int& nNumPolygon = GetNumPolygon();
	const int& nNumVtx = GetNumVtx();

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
	pDevice->SetStreamSource(0, pVtxBuff, 0, sizeof(VERTEX_3D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(pIdxBuff);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, pTexture);

	if (GetUseDraw() == true)
	{
		//ポリゴンの描画
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, nNumVtx, 0, nNumPolygon);
	}
	//片面だけ描画する
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}
//===================================================================================================================

//===============================================================
//死亡フラグ設定処理
//===============================================================
void CWire::SetDeath()
{
	if (GetUseDeath())
	{//死亡フラグを使用するなら
		if (m_pWireHead != nullptr)
		{//ワイヤーの頭を破棄
			m_pWireHead->SetUseDeath(true);//死亡フラグを使用する
			m_pWireHead->SetDeath();       //死亡フラグを設定する
			m_pWireHead = nullptr;         //ポインタを初期化
		}
		if (m_pPlayer != nullptr)
		{//プレイヤーへのポインタを解除(ワイヤーからは開放しない）
			m_pPlayer = nullptr;
		}
	}
	CMeshCylinder::SetDeath();//メッシュシリンダーの死亡フラグ設定処理
}
//===================================================================================================================

//===============================================================
//生成処理
//===============================================================
CWire* CWire::Create(WIRETYPE Type, D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, float fRadius, float fHeight, int nNumDivisionXZ, int nNumDivisionY)
{
	CTexture* pTexture = CManager::GetTexture();                                                  //テクスチャ情報へのポインタ
	CWire* pWire = DBG_NEW CWire(Type, fRadius, fHeight, nNumDivisionXZ, Pos, Rot, nNumDivisionY);//ワイヤーの生成
	pWire->Init();                                                                                //初期化処理  
	pWire->SetTextureIndex(pTexture->Regist(s_WIRE_FILENAME[static_cast<int>(Type)]));            //テクスチャを登録し、テクスチャ番号を設定
	pWire->BindTexture(pTexture->GetAddress(pWire->GetTextureIndex()));                           //テクスチャを割り当てる

	return pWire;
}
//===================================================================================================================