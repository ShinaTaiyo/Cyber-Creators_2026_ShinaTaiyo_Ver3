//====================================================
//
//５月２０日：C++で基盤づくり[object2d.h]
//Author:ShinaTaiyo
//
//====================================================

//========================
//インクルード
//========================
#include "object2d.h"
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "calculation.h"
//================================================================================

//========================
//静的メンバ変数
//========================

//================================================================================

//========================
//コンストラクタ
//========================
CObject2D::CObject2D(int nPriority) : CObject(nPriority),m_nCntBlinkingFrame(0),m_nMaxBlinkingFrame(0),m_bBlinkingAim(false),m_fLimitBlinkingRatio(0.0f),
m_nLife(0),m_nMaxLife(0),m_fRatioLife(0.0f),m_bUseLifeRatioColor(false),m_bUseDraw(true)
{
	m_pTexture = nullptr;
	m_pVtxBuff = nullptr;
}
//================================================================================

//========================
//デストラクタ
//========================
CObject2D::~CObject2D()
{
}
//================================================================================


//========================
//初期設定
//========================
HRESULT CObject2D::Init()
{
	m_bUse = true;          //使用状態
	m_nAnimaionPattern = 0; //アニメーションパターン
	m_nMaxAnimationPattern = 0;//最大アニメーションパターン
	m_nAnimationCnt = 0;    //アニメーションカウント
	m_nAnimationChange = 0; //アニメーションを変えるカウント数
	m_fAnimationSplit = 0.0f;//アニメーション１分割当たりの値
	m_nTextureIndex = 0;     //テクスチャ番号
	m_fWidth = 0.0f;       //横幅
	m_fHeight = 0.0f;      //高さ	
	m_fMaxWidth = 0.0f;    //最大横幅
	m_fMaxHeight = 0.0f;   //最大高さ
	m_fLength = 0.0f;      //４頂点との距離
	m_fAngle = 0.0f;
	m_bAnimFlag = false;   //アニメーションをするかどうか
	m_pos = SENTER_VECTOR3;//位置（中心）
	m_Move = NULL_VECTOR3;//移動量
	m_col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);//色合い
	m_SupportPos = NULL_VECTOR3;

	//=======================================
	//回転系
	//=======================================
	m_fPolygonRotSpeed = 0.0f;//回転速度
	m_rot = NULL_VECTOR3;
	//===============================================================================================

	//=======================================
	//サイズ系
	//=======================================
	m_Scale = D3DXVECTOR2(1.0f, 1.0f);
	m_bUseScale = false;//拡大率を使用するかどうか
	//===============================================================================================

	//=======================================
	//浮遊系
	//=======================================
	m_fFloatingRot = 0.0f;//普通向き
	m_fFloatingSpeed = 0.0f;//普通速度
	m_fFloatingAddSpeed = 0.0f;//普通加速度
	m_bUseFloating = false;//浮遊を使用するかどうか
	m_fFloatingLimitSpeed = 0.0f;
	//===============================================================================================

	VERTEX_2D* pVtx;
	
	CRenderer* pRenderer = CManager::GetRenderer();     //レンダラークラスを取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice(); //デバイスを取得

	
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);


	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = NULL_VECTOR3;
	pVtx[1].pos = NULL_VECTOR3;
	pVtx[2].pos = NULL_VECTOR3;
	pVtx[3].pos = NULL_VECTOR3;

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
	
	return S_OK;
}
//================================================================================


//========================
//終了処理
//========================
void CObject2D::Uninit()
{
	int nID = GetId();
	//============================
	//頂点バッファの破棄
	//============================
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	//==================================================

	//============================
	//テクスチャの破棄
	//============================
	if (m_pTexture != nullptr)
	{
		//m_pTexture->Release(); //同じアドレスのテクスチャを共有しているので、アンロードだけで良い？
		m_pTexture = nullptr;
	}
}
//================================================================================


//========================
//更新処理
//========================
void CObject2D::Update()
{
	VERTEX_2D* pVtx;
	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//体力を減らす処理
	SubLifeProcess();

	//拡大率の処理
	ScaleProcess();

	//浮遊の処理
	FloatingProcess();

	//点滅の処理
	BlinkingProcess();

	m_nAnimationCnt++;
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

	m_rot.z += m_fPolygonRotSpeed;//ポリゴンを回転させる

	//=============================================
	//ポリゴンの出し方を種類によって決める
	//=============================================
	switch (m_PolygonType)
	{
		//普通ポリゴン
	case POLYGONTYPE00_NORMAL:
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y - m_fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y - m_fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y + m_fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y + m_fHeight, 0.0f);
		break;
		//回転ポリゴン
	case POLYGONTYPE01_SENTERROLLING:
		m_fLength = sqrtf(m_fWidth * m_fWidth + m_fHeight * m_fHeight) / 2.0f;
		m_fAngle = atan2f(m_fWidth, m_fHeight);
		//頂点座標の設定
		pVtx[0].pos.x = m_pos.x + sinf(m_rot.z - (D3DX_PI - m_fAngle)) * m_fLength;//上から左に４５度
		pVtx[0].pos.y = m_pos.y + cosf(m_rot.z - (D3DX_PI - m_fAngle)) * m_fLength;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = m_pos.x + sinf(m_rot.z + (D3DX_PI - m_fAngle)) * m_fLength;//上から右に４５度
		pVtx[1].pos.y = m_pos.y + cosf(m_rot.z + (D3DX_PI - m_fAngle)) * m_fLength;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = m_pos.x + sinf(m_rot.z - m_fAngle) * m_fLength;//下から左に４５度
		pVtx[2].pos.y = m_pos.y + cosf(m_rot.z - m_fAngle) * m_fLength;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = m_pos.x + sinf(m_rot.z + m_fAngle) * m_fLength;//下から右に４５度
		pVtx[3].pos.y = m_pos.y + cosf(m_rot.z + m_fAngle) * m_fLength;
		pVtx[3].pos.z = 0.0f;
		break;
	case POLYGONTYPE02_DOWN://中心点が下
		pVtx[0].pos.x = m_pos.x - m_fWidth;
		pVtx[0].pos.y = m_pos.y - m_fHeight;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = m_pos.x + m_fWidth;
		pVtx[1].pos.y = m_pos.y - m_fHeight;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = m_pos.x - m_fWidth;
		pVtx[2].pos.y = m_pos.y;//中点を下側につけるため
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = m_pos.x + m_fWidth;
		pVtx[3].pos.y = m_pos.y;//中点を下側につけるため
		pVtx[3].pos.z = 0.0f;
		break;
	case POLYGONTYPE03_LEFT://中心点が左側
		pVtx[0].pos = D3DXVECTOR3(m_pos.x, m_pos.y - m_fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y - m_fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x, m_pos.y + m_fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y + m_fHeight, 0.0f);
		break;
	case POLYGONTYPE04_RIGHT://中心点が左側
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y - m_fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x, m_pos.y - m_fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y + m_fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x, m_pos.y + m_fHeight, 0.0f);
		break;
	default:
		break;
	}
	//========================================================================================================================================
	
	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(m_fAnimationSplit * (float)(m_nAnimaionPattern), 0.0f);
	pVtx[1].tex = D3DXVECTOR2(m_fAnimationSplit + m_fAnimationSplit * (float)(m_nAnimaionPattern), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(m_fAnimationSplit * (float)(m_nAnimaionPattern), 1.0f);
	pVtx[3].tex = D3DXVECTOR2(m_fAnimationSplit + m_fAnimationSplit * (float)(m_nAnimaionPattern), 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	//位置の更新
	m_pos += m_Move;

}
//================================================================================


//========================
//描画処理
//========================
void CObject2D::Draw()
{
	if (m_bUseDraw == true)
	{

		//レンダラークラスの情報を取得
		CRenderer* pRenderer = CManager::GetRenderer();
		//デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));//頂点バッファへのポインタと頂点情報の構造体のサイズ
		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);
		//テクスチャの設定h
		pDevice->SetTexture(0, m_pTexture);

		//アルファテストを有効(アルファ値が０より大きい場合に描画する）
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		//アルファテスト無効に戻す
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	}
}
//================================================================================

//=====================================
//テクスチャ割り当て処理
//=====================================
void CObject2D::BindTexture(LPDIRECT3DTEXTURE9 pTex)
{
 	int nID = GetId();//IDを取得する
 	m_pTexture = pTex;
}
//================================================================================

//=====================================
//色を設定する
//=====================================
void CObject2D::SetColor(D3DXCOLOR col, bool bAlphaOnly, float fAlpha)
{
	if (bAlphaOnly == false)
	{//全体の色を設定
		m_col = col;
	}
	else
	{//透明度だけ設定
		m_col.a = fAlpha;
	}
}

//=====================================
//２Ｄオブジェクトの情報を設定
//=====================================
void CObject2D::SetInfo(int nMaxAnimationPattern, int nAnimationChange, float fWidth, float fHeight, D3DXCOLOR col, POLYGONTYPE PolygonType, bool bAnim)
{
	m_nMaxAnimationPattern = nMaxAnimationPattern;             //アニメーションのパターンの最大数
	m_nAnimationChange = nAnimationChange;                     //アニメーションの座標を変えるフレーム数
	m_fAnimationSplit = (float)(1.0f / m_nMaxAnimationPattern);//１分割当たりのアニメーション範囲
	m_fWidth = fWidth;                                         //横幅
	m_fMaxWidth = fWidth;                                      //最大横幅
	m_fHeight = fHeight;                                       //立幅
	m_fMaxHeight = fHeight;                                    //最大高さ
	m_PolygonType = PolygonType;                               //ポリゴンの出し方の種類を決める
	m_col = col;                                               //色の設定
	m_bAnimFlag = bAnim;                                       //使用状態
}
//================================================================================

//======================================================
//拡大率の処理
//======================================================
void CObject2D::ScaleProcess()
{
	if (m_bUseScale == true)
	{//拡大率を使用するかどうか
		m_fWidth = m_fMaxWidth * m_Scale.x;
		m_fHeight = m_fMaxHeight * m_Scale.y;
	}
}
//================================================================================

//======================================================
//浮遊の処理
//======================================================
void CObject2D::FloatingProcess()
{
	if (m_bUseFloating == true)
	{

		float fLength = CCalculation::CalculationLength(m_SupportPos, m_pos);

		float fAbsSpeed = 0.0f;
		fAbsSpeed = fabsf(m_fFloatingSpeed);

		if (fAbsSpeed > m_fFloatingLimitSpeed)
		{
			m_fFloatingAddSpeed *= -1;
		}

      	m_fFloatingSpeed += m_fFloatingAddSpeed;//速度を増やし続ける
		m_Move.x = sinf(m_fFloatingRot) * m_fFloatingSpeed;
		m_Move.y = cosf(m_fFloatingRot) * m_fFloatingSpeed;
	}
}
//====================================================================================================

//======================================================
//点滅処理
//======================================================
void CObject2D::BlinkingProcess()
{
	if (m_bUseBlinking == true)
	{
		if (m_bBlinkingAim == false)
		{
			m_nCntBlinkingFrame++;
		}
		else
		{
			m_nCntBlinkingFrame--;
		}

		if (m_nCntBlinkingFrame >= m_nMaxBlinkingFrame || m_nCntBlinkingFrame <= 0)
		{
			m_bBlinkingAim = m_bBlinkingAim ? false : true;
		}

		float fRatioLife = 0.0f;
		fRatioLife = float(m_nCntBlinkingFrame) / float(m_nMaxBlinkingFrame);
		m_col.a = fRatioLife;

		if (m_fLimitBlinkingRatio > fRatioLife)
		{
			m_bBlinkingAim = false;
		}
	}
}
//====================================================================================================

//======================================================
//体力を減らす処理
//======================================================
void CObject2D::SubLifeProcess()
{
	if (m_bUseLife == true)
	{
		m_nLife--;
		m_fRatioLife = float(m_nLife) / float(m_nMaxLife);

		if (m_bUseLifeRatioColor == true)
		{//体力割合に応じて色合いを変えるかどうか
			LifeRatioColorProcess();
		}

		if (m_nLife <= 0)
		{//死亡フラグを設定
			SetDeath();
		}
	}
}
//====================================================================================================

//======================================================
//体力を減らす処理
//======================================================
void CObject2D::LifeRatioColorProcess()
{
	SetColor(NORMAL_COL, true, m_fRatioLife);
}
//====================================================================================================