//=============================================================================================
//
//６月５日：ビルボードを出す[billboard.cpp]
//Author:ShinaTaiyo
//
//=============================================================================================

//==================================================
//インクルード
//==================================================
#include "billboard.h"
#include "manager.h"
#include "renderer.h"
#include "effect.h"
#include "block.h"
#include "enemy.h"
#include "game.h"
#include "scene.h"
#include "model.h"
#include "objectX.h"
//=============================================================================================

//==================================================
//静的メンバ初期化
//==================================================

//=============================================================================================

CBillboard::CBillboard(int nPriority) : CObject(nPriority)
{
	m_pTexture = nullptr;
	m_pVtxBuff = nullptr;
}

//==================================================
//デストラクタ
//==================================================
CBillboard::~CBillboard()
{
	m_mtxWorld = {};
	m_pMtxParent = nullptr;
}
//=============================================================================================

//==================================================
//初期化処理
//==================================================
HRESULT CBillboard::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//デバイスへのポインタと取得

	m_nTextureIndex = 0;
	m_Col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);//色合い
	m_fWidth = 0.0f;                          //横幅
	m_fFormarWidth = 0.0f;                    //元の横幅
	m_fHeight = 0.0f;                         //高さ
	m_fFormarHeight = 0.0f;                   //元の高さ

	m_Pos = NULL_VECTOR3;                     //位置
	m_PosOld = NULL_VECTOR3;                  //1f前の位置
	m_SupportPos = NULL_VECTOR3;              //召喚位置
	m_Rot = NULL_VECTOR3;                     //向き
	m_Move = NULL_VECTOR3;                    //移動量
	m_mtxWorld = {};                          //マトリックスワールド
	m_nTextureIndex = 0;                      //テクスチャ番号

	m_nAnimaionPattern = 0;                   //アニメーションパターン
	m_nAnimationChange = 0;                   //アニメーションを変えるカウント数
	m_fAnimationSplit = 0.0f;                 //位置分割当たりのアニメーションの範囲
	m_nMaxAnimationPattern = 0;               //アニメーションパターン最大数
	m_nAnimationCnt = 0;                      //アニメーションカウント

	m_fScale = 1.0f;                          //拡大率
	m_bScaling = false;                       //拡縮をするかどうか
	m_fAddScale = 0.0f;                       //拡大量

	m_fCurveSpeed = 0.0f;                     //カーブする速さ
	m_bUseCurve = false;                      //カーブさせるかどうか
	m_fSupportCurveLength = 0.0f;             //視点を軸に回転させる距離
	m_bUseLengthCurve = false;                //視点を軸に回転させるかどうか
	m_fAddCurveLength = 0.0f;
	m_fSpeed = 0.0f;                          //速さ
	m_fAddRot = 0.0f;                         //向きの補正
	m_fStartRot = 0.0f;                       //最初の向き

	m_bSetEffect = false;                     //エフェクトを使用するかどうか
	m_SetEffectCol = NORMAL_COL;              //設定するエフェクトの色合い
	m_nSetEffectLife = 0;                     //設定するエフェクトの体力
	m_SetEffectSize = D3DXVECTOR2(0.0f, 0.0f);//設定するエフェクトのサイズ

	m_bMtxChild = false;                      //子マトリックスにするかどうか
	m_pMtxParent = nullptr;                   //親の敵へのポインタ
	m_TransPos = NULL_VECTOR3;                //子マトリックスを使用している場合のワールド座標

	m_bUseGravity = false;                    //重力を使用するかどうか
	m_fGravityPower = 0.0f;                   //重力の大きさ
	m_fAddGravity = 0.0f;                     //加算する重力

	m_bUseHorming = false;                    //ホーミングを使用するかどうか
	m_fRotMove = 0.0f;                        //向きへの移動量
	m_nCntTime = 0;                           //出現してからの時間をカウントする

	m_bUseAddSpeed = false;                   //加速を使用するかどうか
	m_bMultiplication = false;                //速度を乗算するかどうか
	m_fAddSpeed = 0.0f;                       //加速度

	m_nLife = 0;                              //体力
	m_nMaxLife = 0;                           //最大体力

	//============================================
	//描画関係
	//============================================
	m_bDraw = true;                           //描画するかどうか
	//==============================================================================================

	//============================================
	//頂点バッファの生成
	//============================================
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,//頂点の数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);
	//==============================================================================================

	VERTEX_3D* pVtx;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定（初期位置）
	pVtx[0].pos = NULL_VECTOR3;
	pVtx[1].pos = NULL_VECTOR3;
	pVtx[2].pos = NULL_VECTOR3;
	pVtx[3].pos = NULL_VECTOR3;

	//norの設定（初期設定）
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。

	//テクスチャ座標の設定（初期設定）
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}
//=============================================================================================

//==================================================
//終了処理
//==================================================
void CBillboard::Uninit(void)
{
	//テクスチャのポインタを初期化
	if (m_pTexture != nullptr)
	{
		m_pTexture = nullptr;
	}

	//頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
}
//=============================================================================================

//==================================================
//更新処理
//==================================================
void CBillboard::Update(void)
{
	VERTEX_3D* pVtx;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	m_nAnimationCnt++;
	m_nCntTime++;        //出現してからの時間をカウントする
	m_nLife--;           //体力を減らし続ける

	//================================
    //拡大がONになっていたら
    //================================
	if (m_bScaling == true)
	{
		m_fScale += m_fAddScale;//拡大率を増やす
		m_fWidth = m_fFormarWidth * m_fScale;
		m_fHeight = m_fFormarHeight * m_fScale;
	}
	//=====================================================

	//========================================
	//カーブがONだったら
	//========================================
	if (m_bUseCurve == true)
	{
		m_fAddRot += m_fCurveSpeed;
		m_Pos.x += sinf(m_fAddRot + m_fStartRot) * m_fSpeed;
		m_Pos.y += cosf(m_fAddRot + m_fStartRot) * m_fSpeed;
	}
	//=============================================================================================

	//========================================
	//支点を軸にカーブがONだったら
	//========================================
	if (m_bUseLengthCurve == true)
	{
		m_fAddRot += m_fCurveSpeed;
		m_fSupportCurveLength += m_fAddCurveLength;
		m_Pos.x = sinf(m_fAddRot + m_fStartRot) * m_fSupportCurveLength + m_SupportPos.x;
		m_Pos.y = cosf(m_fAddRot + m_fStartRot) * m_fSupportCurveLength + m_SupportPos.y;
	}
	//=============================================================================================

	//================================
	//エフェクトがONになっていたら
	//================================
	if (m_bSetEffect == true)
	{
		if (m_bMtxChild == false)
		{
			CEffect::Create(CEffect::EFFECTTYPE00_NORMAL,m_nSetEffectLife, m_SetEffectSize.x, m_SetEffectSize.y, m_Pos, m_SetEffectCol);
		}
		else
		{//自分のマトリックスの位置を参照してエフェクトを出したいため
			CEffect::Create(CEffect::EFFECTTYPE00_NORMAL, m_nSetEffectLife, m_SetEffectSize.x, m_SetEffectSize.y, m_TransPos, m_SetEffectCol);
		}
	}
	//==============================================================================================

	//========================================
	//重力がONになっていたら
	//========================================
	if (m_bUseGravity == true)
	{
		m_fGravityPower += m_fAddGravity;
		m_Move.y += m_fGravityPower;
	}
	//==============================================================================================

	//========================================
	//ホーミングがONになっていたら
	//========================================
	if (m_bUseHorming == true)
	{
		HormingProcess();//ホーミングの処理
	}
	//==============================================================================================

	//========================================
	//加速を使用するかどうか
	//========================================
	if (m_bUseAddSpeed == true)
	{
		if (m_bMultiplication == false)
		{//乗算OFF
			m_Move += D3DXVECTOR3(m_fAddSpeed,m_fAddSpeed,0.0f);
		}
		else
		{//乗算ON
			m_Move *= m_fAddSpeed;
		}
	}
	//==============================================================================================


	//========================================
	//移動量の更新
	//========================================
	m_PosOld = m_Pos;
	m_Pos += m_Move;
	//==============================================================================================

	//========================================
	//親オブジェクトX関係
	//========================================
	if (m_pMtxParent != nullptr)
	{

	}
	//==============================================================================================

	//========================================
	//ポリゴン回転処理
	//========================================
	if (m_bUsePolygonRot == true)
	{
		m_Rot.z += m_fPolygonRotPower;
	}
	//==============================================================================================

	if (m_bAnimFlag == true)
	{//アニメーションをするなら
		if (m_nAnimationCnt % m_nAnimationChange == 0)
		{
			m_nAnimaionPattern++;

			if (m_nAnimaionPattern >= m_nMaxAnimationPattern)
			{
				m_nAnimaionPattern = 0;
			}
		}
	}

	float fLength = sqrtf(m_fWidth * m_fWidth + m_fHeight * m_fHeight) / 2.0f;
	float fAngle = atan2f(m_fWidth, m_fHeight);

	//頂点座標の設定
	pVtx[0].pos.x = sinf(m_Rot.z - fAngle) * fLength;//上から左に４５度
	pVtx[0].pos.y = cosf(m_Rot.z - fAngle) * fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = sinf(m_Rot.z + fAngle) * fLength;//上から右に４５度
	pVtx[1].pos.y = cosf(m_Rot.z + fAngle) * fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = sinf(m_Rot.z - (D3DX_PI - fAngle)) * fLength;//上から左に４５度
	pVtx[2].pos.y = cosf(m_Rot.z - (D3DX_PI - fAngle)) * fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = sinf(m_Rot.z + (D3DX_PI - fAngle)) * fLength;//上から右に４５度
	pVtx[3].pos.y = cosf(m_Rot.z + (D3DX_PI - fAngle)) * fLength;
	pVtx[3].pos.z = 0.0f;

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(m_fAnimationSplit * (float)(m_nAnimaionPattern), 0.0f);
	pVtx[1].tex = D3DXVECTOR2(m_fAnimationSplit + m_fAnimationSplit * (float)(m_nAnimaionPattern), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(m_fAnimationSplit * (float)(m_nAnimaionPattern), 1.0f);
	pVtx[3].tex = D3DXVECTOR2(m_fAnimationSplit + m_fAnimationSplit * (float)(m_nAnimaionPattern), 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	if (m_nLife <= 0)
	{
		SetDeath();
	}

}
//=============================================================================================

//==================================================
//描画処理
//==================================================
void CBillboard::Draw(void)
{
	D3DXMATRIX mtxTrans,mtxRot;//計算用マトリックス
	D3DXMATRIX mtxParent = {};
	D3DXMATRIX mtxView;//ビューマトリックス取得用
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	if (m_bMtxChild == true && m_pMtxParent != nullptr)
	{
		mtxParent = *m_pMtxParent;
	}

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//ビューマトリックスを取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//ポリゴンをカメラに対して正面に向ける
	D3DXMatrixInverse(&m_mtxWorld, NULL, &mtxView);

	//逆行列を求める
	m_mtxWorld._41 = 0.0f;
	m_mtxWorld._42 = 0.0f;
	m_mtxWorld._43 = 0.0f;


	if (m_pMtxParent == nullptr && m_bMtxChild == false)
	{//親となるモデルがいなかったら
		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, GetPos().x, GetPos().y, GetPos().z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);
	}
	else
	{//親となるモデルがいたら
		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, GetPos().x, GetPos().y, GetPos().z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);//自分のマトリックスと位置をかけ合わせる

		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);//親の敵のマトリックスとかけ合わせる
	}
	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	//================================================
	//描画の調整
	//================================================
	//ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	////Zの比較方法変更。（モデルがあっても隠れなくなる）
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	////Zバッファに書き込まない（重なり方に違和感がなくなる）
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//アルファテストを有効(アルファ値が０より大きい場合に描画する）
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//=======================================================================================================================================
	
	if (m_bDraw == true)
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}

	D3DXVECTOR3 PosZero = D3DXVECTOR3(0.0f,0.0f,0.0f);
	D3DXVECTOR3 PosBullet;
	//if (m_bMtxChild == true)
	//{//子マトリックスを使用する場合に、ワールド座標を判定する
		D3DXVec3TransformCoord(&PosBullet, &PosZero, &m_mtxWorld);//左の引数のローカル座標に真ん中の引数のワールド座標を代入する
		m_TransPos = PosBullet;
	//}
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
//=============================================================================================

//==================================================
//テクスチャを割り当てる処理
//==================================================
void CBillboard::bindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}
//============================================================================================================================================

//==================================================
//正方形の当たり判定を取る
//==================================================
bool CBillboard::CollisionSquare(D3DXVECTOR3 pos, D3DXVECTOR3 VtxMax, D3DXVECTOR3 VtxMin)
{
	D3DXVECTOR3 ComparisonPos = NULL_VECTOR3;
	D3DXVECTOR3 ComparisonVtxMin = NULL_VECTOR3;
	D3DXVECTOR3 ComparisonVtxMax = NULL_VECTOR3;

	ComparisonPos.x = (float)(floor(pos.x));
	ComparisonPos.y = (float)(floor(pos.y));
	ComparisonPos.z = (float)(floor(pos.z));

	ComparisonVtxMax.x = (float)(floor(VtxMax.x));
	ComparisonVtxMax.y = (float)(floor(VtxMax.y));
	ComparisonVtxMax.z = (float)(floor(VtxMax.z));

	ComparisonVtxMin.x = (float)(floor(VtxMin.x));
	ComparisonVtxMin.y = (float)(floor(VtxMin.y));
	ComparisonVtxMin.z = (float)(floor(VtxMin.z));


	D3DXVECTOR2 MySize = D3DXVECTOR2(0.0f,0.0f);

	//==================================
	//判定用にキャストする
	//==================================
	MySize.x = (float)(floor(m_fWidth));
	MySize.y = (float)(floor(m_fHeight));
	//==========================================================================================================

	bool bCollision = false;                         //地面に着地しているか
	float fAdjust = 0.001f;
	
	if (m_Pos.x + MySize.x / 2 >= ComparisonPos.x + ComparisonVtxMin.x
		&& m_Pos.x - MySize.x / 2 <= ComparisonPos.x + ComparisonVtxMax.x
		&& m_Pos.y + MySize.y / 2 >= ComparisonPos.y + ComparisonVtxMin.y
		&& m_Pos.y - MySize.y / 2 <= ComparisonPos.y + ComparisonVtxMax.y
		&& m_Pos.z + MySize.x / 2 >= ComparisonPos.z + ComparisonVtxMin.z
		&& m_Pos.z - MySize.x / 2 <= ComparisonPos.z + ComparisonVtxMax.z)
	{//対象の下端がモデルの+Yに当たった時の処理
		bCollision = true;
	}
	return bCollision;
}

//==================================================
//サイズを設定する
//==================================================
void CBillboard::SetSize(float fWidth, float fHeight)
{
	m_fWidth = fWidth;
	m_fHeight = fHeight;
}
//============================================================================================================================================

//==========================================================================
//元のサイズを設定する
//==========================================================================
void CBillboard::SetFormarSize(float fFormarWidth, float fFormarHeight)
{
	m_fFormarWidth = fFormarWidth;
	m_fFormarHeight = fFormarHeight;
}
//============================================================================================================================================

//==================================================
//サイズを設定する
//==================================================
void CBillboard::SetColor(D3DXCOLOR col)
{
	VERTEX_3D* pVtx;
	m_Col = col;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定（初期位置）
	pVtx[0].col = m_Col;
	pVtx[1].col = m_Col;
	pVtx[2].col = m_Col;
	pVtx[3].col = m_Col;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

}
//============================================================================================================================================

//=============================================================================================================
//アニメーション関係の設定
//=============================================================================================================
void CBillboard::SetAnimInfo(int nMaxAnimationPattern, int nAnimationChange, D3DXCOLOR col, bool bAnim)
{
	m_nMaxAnimationPattern = nMaxAnimationPattern;             //アニメーションパターン最大値
	m_nAnimationChange = nAnimationChange;                     //アニメーションを次に進めるカウント数
	m_Col = col;                                               //色合い
	m_bAnimFlag = bAnim;                                       //アニメーションをするかどうか
	m_fAnimationSplit = (float)(1.0f / m_nMaxAnimationPattern);//１分割当たりのアニメーション範囲

	VERTEX_3D* pVtx;
    //頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点カラーの設定
	pVtx[0].col = m_Col;
	pVtx[1].col = m_Col;
	pVtx[2].col = m_Col;
	pVtx[3].col = m_Col;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

}
//================================================================================================================

//===============================================================
//拡縮の設定
//===============================================================
void CBillboard::SetScaling(float fAddScale)
{
	m_bScaling = true;//拡縮をするかどうか
	m_fAddScale = fAddScale;//拡縮量
}
//================================================================================================================

//===============================================================
//カーブを設定
//===============================================================
void CBillboard::SetUseCurve(float fRot, float fSpeed, float fCurveSpeed)
{
	m_bUseCurve = true;
	m_fSpeed = fSpeed;
	m_fStartRot = fRot;
	m_fCurveSpeed = fCurveSpeed;
}
//================================================================================================================

//===============================================================
//視点を軸に回転するカーブを設定
//===============================================================
void CBillboard::SetUseLengthCurve(bool bUse, float fStartRot, float fCurveSpeed, float fAddCurveLength)
{
	m_bUseLengthCurve = bUse;
	m_fStartRot = fStartRot;
	m_fCurveSpeed = fCurveSpeed;
	m_fAddCurveLength = fAddCurveLength;
}
//================================================================================================================

//===============================================================
//エフェクトの使用する場合の設定
//===============================================================
void CBillboard::SetUseEffect(int nSetEffectLife, D3DXVECTOR2 Size, D3DXCOLOR Col)
{
	m_bSetEffect = true;                //エフェクトをONにする
	m_nSetEffectLife = nSetEffectLife;  //設定するエフェクトの体力
	m_SetEffectSize = Size;             //設定するエフェクトのサイズ
	m_SetEffectCol = Col;               //設定するエフェクトの色合い
}
//================================================================================================================

//===============================================================
//子マトリックスを設定
//===============================================================
void CBillboard::SetUseMtxChild(D3DXMATRIX* pMtxParent, bool bUse)
{
	m_pMtxParent = pMtxParent;
	m_bMtxChild = true;
}
//================================================================================================================


//===============================================================
//重力を設定
//===============================================================
void CBillboard::SetUseGravity(float fGravity)
{
	m_fGravityPower = fGravity;
	m_bUseGravity = true;
}
//================================================================================================================

//===============================================================
//ホーミングを設定
//===============================================================
void CBillboard::SetUseHorming(float fSpeed)
{
	m_bUseHorming = true;//ホーミングを使用するかどうか
	m_fSpeed = fSpeed;   //速さ
}
//================================================================================================================

//===============================================================
//加速を設定
//===============================================================
void CBillboard::SetUseAddSpeed(bool bUseAddSpeed, bool bMultiplication, float fAddSpeed)
{
	m_bUseAddSpeed = bUseAddSpeed;
	m_bMultiplication = bMultiplication;
	m_fAddSpeed = fAddSpeed;
}
//================================================================================================================

//===============================================================
//ホーミングの処理
//===============================================================
void CBillboard::HormingProcess()
{
	float fRotDest = 0.0f;//目的の位置への角度
	float fRotDiff = 0.0f;//現在の角度と目的の角度の差分
	float fVXaim = 0.0f;  //Xベクトル
	float fVYaim = 0.0f;  //Yベクトル
	float fVLaim = 0.0f;  //総合ベクトル
	D3DXVECTOR3 TargetPos = NULL_VECTOR3;//ターゲットの位置
	//===================================
	//XZベクトルを出す
	//===================================
	if (CScene::GetMode() == CScene::MODE_GAME)
	{
		TargetPos = CGame::GetPlayer()->GetPos();
		fVXaim = TargetPos.x - m_Pos.x;
		fVYaim = TargetPos.y - m_Pos.y;

		fVLaim = atan2f(fVXaim, fVYaim);

		//==========================================================================================================================================================

		//if (g_aEnemy[nNumEnemy].nTime == 1)
		//{//敵が出現時に最初の軌道を決める（プレイヤーと敵のベクトルで計算)
		//	g_aEnemy[nNumEnemy].fRotMove = atan2f(g_aEnemy[nNumEnemy].fVXaim, g_aEnemy[nNumEnemy].fVZaim);
		//}

		//目的の位置への角度
		fRotDest = atan2f(TargetPos.x - m_Pos.x, TargetPos.y - m_Pos.y);

		//現在の移動方向の角度と、目的の移動方向の角度の差分を求める
		fRotDiff = fRotDest - m_fRotMove;

		//===============================
		//角度差分の修正
		//===============================
		if (m_fRotMove < 0.0f)
		{//現在の角度がマイナスなら
			if ((m_fRotMove + D3DX_PI) <= fRotDest)
			{//現在の角度 + D3DX_PIより目的の角度が大きかったら
				fRotDiff -= D3DX_PI * 2.0f;
			}
		}
		if (m_fRotMove > 0.0f)
		{//現在の角度がプラスなら
			if ((m_fRotMove - D3DX_PI) >= fRotDest)
			{//現在の角度 - D3DX_PIより目的の角度が小さかったら
				fRotDiff += D3DX_PI * 2.0f;
			}
		}
		//==============================================================================================
		m_fRotMove += fRotDiff * 0.05f;//移動方向（角度補正）

		m_Move.x = sinf(m_fRotMove) * m_fSpeed;
		m_Move.y = cosf(m_fRotMove) * m_fSpeed;
	}
}
//================================================================================================================