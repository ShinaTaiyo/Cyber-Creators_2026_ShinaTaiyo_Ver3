//====================================================================================
//
//７月２９日：階層構造を作る[modelparts.cpp]
//Author:ShinaTaiyo
//
//====================================================================================

//============================================
//インクルード
//============================================
#include "modelparts.h"
#include "manager.h"
#include "renderer.h"
#include "objectXInfo.h"
//====================================================================================

//============================================
//コンストラクタ
//============================================
CModelParts::CModelParts(int nPriority) : m_pMtxParent(),m_OffSetPos(NULL_VECTOR3),m_OffSetRot(NULL_VECTOR3)
{

}
//====================================================================================

//============================================
//デストラクタ
//============================================
CModelParts::~CModelParts()
{

}
//====================================================================================

//============================================
//初期化処理
//============================================
HRESULT CModelParts::Init()
{
	CObjectX::Init();//オブジェクトX初期化処理

	m_OffSetPos = NULL_VECTOR3;//オフセット位置
	m_OffSetSupportPos = NULL_VECTOR3;//最初のオフセットの位置
	m_OffSetRot = NULL_VECTOR3;//オフセット向き
	m_OffSetSupportRot = NULL_VECTOR3;//最初のオフセットの向き
	m_pMtxParentParts = {};  //親モデルパーツへのポインタ
	m_pMtxParent = {};       //親モデルへのポインタ
	m_WorldPos = NULL_VECTOR3; //ワールド座標
	return S_OK;
}
//====================================================================================

//============================================
//終了処理
//============================================
void CModelParts::Uninit()
{
	CObjectX::Uninit();
}
//====================================================================================

//============================================
//死亡フラグ設定処理
//============================================
void CModelParts::SetDeath()
{
	CObject::SetDeath();
}
//====================================================================================

//============================================
//更新処理
//============================================
void CModelParts::Update()
{
	CObjectX::Update();//オブジェクトX更新処理
}
//====================================================================================

//============================================
//描画処理
//============================================
void CModelParts::Draw()
{

}
//================================================================================================================================================

//====================================================================
//別枠の描画処理
//====================================================================
void CModelParts::ExtraDraw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice(); //デバイスへのポインタと取得
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;                            //計算用マトリックス
	D3DMATERIAL9 matDef;                                              //現在のマテリアル保存用
	D3DXMATERIAL* pMat;                                               //マテリアルデータへのポインタ
	D3DXMATRIX& mtxWorld = CObjectX::GetMatrixWorld();                //ワールドマトリックス取得
	D3DXMATRIX mtxParentParts = {};                                   //親パーツのワールドマトリックス
	D3DXMATRIX mtxParent = {};                                        //親パーツのワールドマトリックス
	D3DXVECTOR3 Scale = GetScale();                                   //拡大率を取得する
	if (m_pMtxParentParts != nullptr)
	{//パーツ同士の中に親がいたら
		mtxParentParts = *m_pMtxParentParts;
	}
	else
	{//パーツ同士に親がいない（親がオブジェクトなど）
		mtxParent = *m_pMtxParent;
	}

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	//大きさを反映
	D3DXMatrixScaling(&mtxScale, Scale.x, Scale.y, Scale.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_OffSetRot.y, m_OffSetRot.x, m_OffSetRot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_OffSetPos.x, m_OffSetPos.y, m_OffSetPos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	if (m_pMtxParentParts != nullptr)
	{
		//親のマトリックスとかけ合わせる
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxParentParts);
	}
	else
	{
		//最新のワールド変換行列を取得（プレイヤー、敵の行列など）
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxParent);
	}
	//if (CObjectX::GetObjectXInfo().Diffuse[0].a > 0.0f)
	//{
	//	//影の描画
	//	DrawShadow(m_Pos);
	//}

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	D3DXVECTOR3 PosZero = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVec3TransformCoord(&m_WorldPos, &PosZero, &mtxWorld);//左の引数のローカル座標に真ん中の引数のワールド座標を代入する

	//=======================================
	//描画の調整
	//=======================================

	//法線の長さを１にする。（スケールなどを使った時は、必ず使う。)
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
	//================================================================================================================

	//マテリアルへのポインタを取得
	pMat = (D3DXMATERIAL*)CObjectX::GetObjectXInfo().pBuffMat->GetBufferPointer();

	//==========================================================================
	//マテリアルの数分、テクスチャを読み込む。
	//==========================================================================
	for (int nCntMat = 0; nCntMat < (int)CObjectX::GetObjectXInfo().dwNumMat; nCntMat++)
	{
		//色合いの設定
		pMat[nCntMat].MatD3D.Diffuse = CObjectX::GetObjectXInfo().Diffuse[nCntMat];

		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//テクスチャを設定する
		pDevice->SetTexture(0, CObjectX::GetObjectXInfo().pTexture[nCntMat]);

		//モデル（パーツ）の描画
		CObjectX::GetObjectXInfo().pMesh->DrawSubset(nCntMat);
	}
	//================================================================================================================

	//=======================================
	//描画の調整を元に戻す
	//=======================================

	//法線の長さを１にする。（スケールなどを使った時は、必ず使う。)
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
	//================================================================================================================

	//保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);

}
//================================================================================================================================================

//=====================================================================================================================
//生成処理
//=====================================================================================================================
CModelParts* CModelParts::Create(char* aString, D3DXVECTOR3 OffsetPos, D3DXVECTOR3 OffSetRot)
{
	CModelParts* pModelParts = DBG_NEW CModelParts;                                                                             //プレイヤーを生成

	bool bSuccess = pModelParts->CObject::GetCreateSuccess();
	int nIdx = 0;//モデルのインデックス
	if (bSuccess == true)
	{
		if (pModelParts != nullptr)
		{
			pModelParts->Init();                             //初期化処理
			pModelParts->m_OffSetPos = OffsetPos;            //オフセット位置
			pModelParts->m_OffSetSupportPos = OffsetPos;     //最初の位置を設定
			pModelParts->m_OffSetRot = OffSetRot;            //オフセット向き
			pModelParts->m_OffSetSupportRot = OffSetRot;     //最初のオフセットの向きを設定
			pModelParts->SetScale(ONE_VECTOR3);              //拡大率を設定する
			pModelParts->SetUseDeath(false);                 //死亡フラグを発動するかどうかを設定する
			nIdx = CManager::GetObjectXInfo()->Regist(CObjectX::OBJECTXTYPE_PLAYER,aString);
			pModelParts->CObjectX::BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(CObjectX::OBJECTXTYPE_PLAYER, nIdx),
				CManager::GetObjectXInfo()->GetBuffMat(CObjectX::OBJECTXTYPE_PLAYER, nIdx),
				CManager::GetObjectXInfo()->GetdwNumMat(CObjectX::OBJECTXTYPE_PLAYER, nIdx),
				CManager::GetObjectXInfo()->GetTexture(CObjectX::OBJECTXTYPE_PLAYER, nIdx),
				CManager::GetObjectXInfo()->GetColorValue(CObjectX::OBJECTXTYPE_PLAYER, nIdx));                  //モデル情報を割り当てる

			pModelParts->SetRotType((CObjectX::ROTTYPE)(0));                                                        //向きの種類を設定
			pModelParts->SetSize();                                                                                 //Xオブジェクトのサイズを設定する
		}
	}
	else
	{//オブジェクトに空きがなかったので破棄する
		delete pModelParts;
		pModelParts = nullptr;
	}

	return pModelParts;
}
//================================================================================================================================================