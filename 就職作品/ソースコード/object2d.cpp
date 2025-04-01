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
#include "debugtext.h"
#include "calculation.h"
//================================================================================

//========================
//静的メンバ変数
//========================

//================================================================================

//========================
//コンストラクタ
//========================
CObject2D::CObject2D(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CObject(nPri,bUseintPri,type,ObjType),
m_nCntBlinkingFrame(0),m_nMaxBlinkingFrame(0),m_bBlinkingAim(false),m_fLimitBlinkingRatio(0.0f),
m_nLife(0),m_nMaxLife(0),m_fRatioLife(0.0f),m_bUseLifeRatioColor(false),m_bUseDraw(true),m_PolygonType(POLYGONTYPE::NORMAL),m_bAnimFlag(false),m_bUse(false),m_bUseBlinking(false),
m_bUseFloating(false),m_bUseLife(false),m_bUseScale(false),m_fAngle(0.0f),m_fAnimationSplit(0.0f),m_fFloatingAddSpeed(0.0f),m_fFloatingLimitSpeed(0.0f),m_fFloatingRot(0.0f),
m_fFloatingSpeed(0.0f),m_fHeight(0.0f),m_fLength(0.0f),m_fMaxHeight(0.0f),m_fMaxWidth(0.0f),m_fPolygonRotSpeed(0.0f),m_fWidth(0.0f),m_nAnimaionPattern(0),m_nAnimationChange(0),
m_nAnimationCnt(0),m_nMaxAnimationPattern(0),m_nTextureIndex(0),m_pos(D3DXVECTOR3(0.0f,0.0f,0.0f)), m_Move(D3DXVECTOR3(0.0f,0.0f,0.0f)), m_SupportPos(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_col(D3DXCOLOR(1.0f,1.0f,1.0f,1.0f)),
m_pVtxBuff(nullptr),m_pTexture(nullptr),m_Scale(D3DXVECTOR3(1.0f,1.0f,1.0f)),m_rot(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_AddScale(D3DXVECTOR2(0.0f,0.0f)),m_bUseAddScale(false)
{
	m_pTexture = nullptr;
	m_pVtxBuff = nullptr;

	SetObjectType(CObject::OBJECTTYPE::OBJECTTYPE_2D);
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

	m_pos = D3DXVECTOR3(SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2,0.0f);//位置（中心）
	m_Move = D3DXVECTOR3(0.0f,0.0f,0.0f);                        //移動量
	m_col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);                      //色合い
	m_SupportPos = D3DXVECTOR3(0.0f,0.0f,0.0f);                  //支点
	m_rot = D3DXVECTOR3(0.0f,0.0f,0.0f);                         //向き
	m_Scale = D3DXVECTOR2(1.0f, 1.0f);                           //拡大率

	VERTEX_2D* pVtx;                                             //2D頂点情報
	
	CRenderer* pRenderer = CManager::GetRenderer();              //レンダラークラスを取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();          //デバイスを取得

	
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);


	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//位置
	pVtx[0].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);

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

	CObject::Init();
	
	return S_OK;
}
//================================================================================


//========================
//終了処理
//========================
void CObject2D::Uninit()
{
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
		m_pTexture = nullptr;
	}
	//==================================================

	CObject::Uninit();//オブジェクトの終了処理
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
		{//アニメーションを変えるフレーム数に達したらアニメーションパターンを増やす
			m_nAnimaionPattern++;

			if (m_nAnimaionPattern >= m_nMaxAnimationPattern)
			{//最大に達したので０に戻す
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
	case POLYGONTYPE::NORMAL:
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y - m_fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y - m_fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y + m_fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y + m_fHeight, 0.0f);
		break;
		//回転ポリゴン
	case POLYGONTYPE::SENTERROLLING:
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
	case POLYGONTYPE::DOWNSENTERROLLING:
		//頂点座標の設定
		m_fLength = sqrtf(m_fWidth * m_fWidth + (m_fHeight * 2.0f) * (m_fHeight * 2.0f)) / 2;

		//対角線の角度を算出する
		m_fAngle = atan2f(m_fWidth,m_fHeight * 2.0f);

		//頂点座標の設定
		pVtx[0].pos.x = m_pos.x + sinf(m_rot.z - (D3DX_PI - m_fAngle)) * m_fLength;
		pVtx[0].pos.y = m_pos.y + cosf(m_rot.z - (D3DX_PI - m_fAngle)) * m_fLength;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = m_pos.x + sinf(m_rot.z + (D3DX_PI - m_fAngle)) * m_fLength;
		pVtx[1].pos.y = m_pos.y + cosf(m_rot.z + (D3DX_PI - m_fAngle)) * m_fLength;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = m_pos.x + sinf(m_rot.z - (D3DX_PI * 0.5f)) * m_fWidth / 2;
		pVtx[2].pos.y = m_pos.y + cosf(m_rot.z - (D3DX_PI * 0.5f)) * m_fWidth / 2;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = m_pos.x + sinf(m_rot.z + (D3DX_PI * 0.5f)) * m_fWidth / 2;
		pVtx[3].pos.y = m_pos.y + cosf(m_rot.z + (D3DX_PI * 0.5f)) * m_fWidth / 2;
		pVtx[3].pos.z = 0.0f;
		break;
	case POLYGONTYPE::DOWN://中心点が下
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
	case POLYGONTYPE::LEFT://中心点が左側
		pVtx[0].pos = D3DXVECTOR3(m_pos.x, m_pos.y - m_fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y - m_fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x, m_pos.y + m_fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y + m_fHeight, 0.0f);
		break;
	case POLYGONTYPE::RIGHT://中心点が左側
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

	CObject::Update();
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

	CObject::Draw();//オブジェクトの描画処理
}
//================================================================================

//========================
//死亡フラグ設定処理
//========================
void CObject2D::SetDeath()
{
	CObject::SetDeath();//オブジェクトの死亡フラグ設定処理
}
//================================================================================

//=====================================
//テクスチャ割り当て処理
//=====================================
void CObject2D::BindTexture(LPDIRECT3DTEXTURE9 pTex)
{
 	m_pTexture = pTex;//テクスチャアドレスを格納
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
void CObject2D::SetAnimInfo(int nMaxAnimationPattern, int nAnimationChange, bool bAnim)
{

	m_nMaxAnimationPattern = nMaxAnimationPattern;             //アニメーションのパターンの最大数
	m_nAnimationChange = nAnimationChange;                     //アニメーションの座標を変えるフレーム数
	m_fAnimationSplit = 1.0f / m_nMaxAnimationPattern;         //１分割当たりのアニメーション範囲
	m_bAnimFlag = bAnim;                                       //使用状態
}
//================================================================================

//=====================================
//テクスチャだけ初期化
//=====================================
void CObject2D::InitTextureOnly()
{
	if (m_pTexture != nullptr)
	{//テクスチャへのポインタが存在していたら
		m_pTexture = nullptr;//ポインタを初期化
	}
}
//================================================================================

//======================================================
//拡大率の処理
//======================================================
void CObject2D::ScaleProcess()
{	
	if (m_bUseAddScale == true)
	{
		m_Scale += m_AddScale;//拡大率を加算する
	}

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
		float fAbsSpeed = 0.0f;//加算速度格納用
		fAbsSpeed = fabsf(m_fFloatingSpeed);

		if (fAbsSpeed > m_fFloatingLimitSpeed)
		{//浮遊速度の最大に達したら移動方向を逆にする
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
	{//点滅するかどうか

		//点滅フレームと最大点滅フレームの割合を取り、その値を上下させて点滅を表現

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
	{//体力を使用する
		m_nLife--;//体力を自動的に減らす
		m_fRatioLife = float(m_nLife) / float(m_nMaxLife);//体力の割合を計算

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
//体力の割合で色合いの透明度を変える処理
//======================================================
void CObject2D::LifeRatioColorProcess()
{
	SetColor(D3DXCOLOR(1.0f,1.0f,1.0f,1.0f), true, m_fRatioLife);
}
//====================================================================================================