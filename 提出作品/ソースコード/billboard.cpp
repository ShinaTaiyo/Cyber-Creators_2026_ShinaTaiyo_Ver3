//===================================================================================================================
//
// ２０２５年６月１日：コードを綺麗にする[billboard.cpp]
// Author:ShinaTaiyo
//
//===================================================================================================================

//*******************************************************************************************************************
// インクルード
//*******************************************************************************************************************
#include "billboard.h" // 元ヘッダ
#include "manager.h"   // マネージャー
#include "renderer.h"  // レンダラー（デバイス取得など）

//===================================================================================================================
// 
// publicメンバ 
//
//===================================================================================================================

//===================================================================================================================
// コンストラクタ
//===================================================================================================================
CBillboard::CBillboard(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) :
	CObject(nPri, bUseintPri, type, ObjType) ,
    m_bAnimFlag(false),
	m_bDraw(true),
	m_bMultiplication(false),
    m_bUseAddSpeed(false),
	m_bUseGravity(false),
	m_bUsePolygonRot(false),
	m_fAddGravity(0.0f),
	m_fAddRot(0.0f),
	m_fAddSpeed(0.0f),
	m_fAnimationSplit(0.0f),
	m_fGravity(0.0f),
	m_fGravityPower(0.0f),
	m_fPolygonRotPower(0.0f),
	m_fStartRot(0.0f),
	m_nMaxAnimationPattern(0),
	m_nAnimationChange(0),
	m_nAnimationCnt(0),
	m_nCntTime(0),
	m_nLife(0),
	m_nMaxLife(0),
    m_nTextureIndex(0),
	m_nAnimaionPattern(0),
	m_Pos(D3DXVECTOR3(0.0f,0.0f,0.0f)),
	m_PosOld(D3DXVECTOR3(0.0f,0.0f,0.0f)),
	m_SupportPos(D3DXVECTOR3(0.0f,0.0f,0.0f)),
	m_Move(D3DXVECTOR3(0.0f,0.0f,0.0f)),
	m_Rot(D3DXVECTOR3(0.0f,0.0f,0.0f)),
	m_mtxWorld(),
    m_Col(D3DXCOLOR(1.0f,1.0f,1.0f,1.0f)),
	m_Scale(D3DXVECTOR3(1.0f,1.0f,1.0f)),
	m_fSpeed(0.0f),
	m_bUseAddScale(false),
	m_Size(D3DXVECTOR3(0.0f,0.0f,0.0f)),
	m_FormarSize(D3DXVECTOR3(0.0f,0.0f,0.0f))
{
	m_pTexture = nullptr;//テクスチャへのポインタを初期化
	m_pVtxBuff = nullptr;//頂点バッファへのポインタを初期化
}

//===================================================================================================================
// デストラクタ
//===================================================================================================================
CBillboard::~CBillboard()
{

}

//===================================================================================================================
// 初期化処理
//===================================================================================================================
HRESULT CBillboard::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//デバイスへのポインタと取得
	VERTEX_3D* pVtx;

	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4, // 頂点の数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定（初期位置）
	pVtx[0].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);

	// norの設定（初期設定）
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標の設定（初期設定）
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//===================================================================================================================
// 終了処理
//===================================================================================================================
void CBillboard::Uninit(void)
{
	// テクスチャのポインタを初期化（テクスチャクラスで管理しているので、解放しなくても大丈夫)
	if (m_pTexture != nullptr)
	{
		m_pTexture = nullptr;
	}

	// 頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
}

//===================================================================================================================
// 更新処理
//===================================================================================================================
void CBillboard::Update(void)
{
	VERTEX_3D* pVtx; // 頂点情報へのポインタ

	// === 処理に使用する情報の宣言、初期化 ====

	m_nAnimationCnt++; // アニメーションカウントをインクリメント
	m_nCntTime++;      // 出現してからのフレーム数をカウントする
	m_nLife -= 1 * static_cast<int>(GetDeltaTimeScale(this) + 0.1f); // 体力を減らし続ける（deltatimeとtimescaleを計算して１以下なら体力を減らさない)

	// 拡大率を加算する
	if (m_bUseAddScale == true)
	{
		m_Scale += m_AddScale;
	}

	// 重力を加算し、現在の重力分Y座標を加算する
	if (m_bUseGravity == true)
	{
		m_fGravityPower += m_fAddGravity * GetDeltaTimeScale(this);
		m_Move.y += m_fGravityPower * GetDeltaTimeScale(this);
	}

	// 加速するなら
	if (m_bUseAddSpeed == true)
	{
		// 乗算OFF
		if (m_bMultiplication == false)
		{
			m_Move += D3DXVECTOR3(m_fAddSpeed,m_fAddSpeed,0.0f) * GetDeltaTimeScale(this);
		}
		// 乗算ON
		else
		{
			m_Move *= powf(m_fAddSpeed,GetDeltaTimeScale(this));//指数関数的減速
		}
	}

	m_PosOld = m_Pos; //1f前の位置
	m_Pos += m_Move * GetDeltaTimeScale(this); // 位置の更新

	// 向きを加算する
	if (m_bUsePolygonRot == true)
	{
		m_Rot.z += m_fPolygonRotPower * GetDeltaTimeScale(this);
	}

	// アニメーションをするなら
	if (m_bAnimFlag == true)
	{
		// アニメーションカウントがアニメーションを変更するフレーム数に達したら
		if (m_nAnimationCnt % m_nAnimationChange == 0)
		{
			m_nAnimaionPattern++;//アニメーションパターンをインクリメント

			// アニメーションパターンが最大アニメーションパターンに達したらリセットする
			if (m_nAnimaionPattern >= m_nMaxAnimationPattern)
			{
				m_nAnimaionPattern = 0;
			}
		}
	}

	float fLength = sqrtf(m_Size.x * m_Size.x + m_Size.y * m_Size.y) / 2.0f; // 中心点からポリゴンの角との距離を取る
	float fAngle = atan2f(m_Size.x, m_Size.y); // 中心点から角への角度を求める

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos.x = sinf(m_Rot.z - fAngle) * fLength; // 上から左に４５度
	pVtx[0].pos.y = cosf(m_Rot.z - fAngle) * fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = sinf(m_Rot.z + fAngle) * fLength; // 上から右に４５度
	pVtx[1].pos.y = cosf(m_Rot.z + fAngle) * fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = sinf(m_Rot.z - (D3DX_PI - fAngle)) * fLength; // 上から左に４５度
	pVtx[2].pos.y = cosf(m_Rot.z - (D3DX_PI - fAngle)) * fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = sinf(m_Rot.z + (D3DX_PI - fAngle)) * fLength; // 上から右に４５度
	pVtx[3].pos.y = cosf(m_Rot.z + (D3DX_PI - fAngle)) * fLength;
	pVtx[3].pos.z = 0.0f;

	// 頂点カラーの設定
	pVtx[0].col = m_Col;
	pVtx[1].col = m_Col;
	pVtx[2].col = m_Col;
	pVtx[3].col = m_Col;

	// テクスチャ座標の設定（アニメーション分割数×現在のアニメーションパターン)
	pVtx[0].tex = D3DXVECTOR2(m_fAnimationSplit * (float)(m_nAnimaionPattern), 0.0f);
	pVtx[1].tex = D3DXVECTOR2(m_fAnimationSplit + m_fAnimationSplit * (float)(m_nAnimaionPattern), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(m_fAnimationSplit * (float)(m_nAnimaionPattern), 1.0f);
	pVtx[3].tex = D3DXVECTOR2(m_fAnimationSplit + m_fAnimationSplit * (float)(m_nAnimaionPattern), 1.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	// 体力が０になったら死亡フラグを発動する
	if (m_nLife <= 0)
	{
		SetDeath();
	}

}

//===================================================================================================================
// 描画処理
//===================================================================================================================
void CBillboard::Draw(void)
{
	D3DXMATRIX mtxTrans,mtxRot,mtxScale; //計算用マトリックス
	D3DXMATRIX mtxView; // ビューマトリックス
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice(); // デバイスへのポインタ

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

	// 拡大率を加算するならスケールを設定
	//if (m_bUseAddScale == true)
	//{
	//	D3DXMatrixScaling(&mtxScale, m_Scale.x, m_Scale.y, m_Scale.z);
	//	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);
	//}

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, GetPos().x, GetPos().y, GetPos().z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	//================================================
	//描画の調整
	//================================================
	
	// ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Zの比較方法変更。（モデルがあっても隠れなくなる）
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	// Zバッファに書き込まない（重なり方に違和感がなくなる）
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// アルファテストを有効(アルファ値が０より大きい場合に描画する）
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	
	// ポリゴンの描画
	if (m_bDraw == true)
	{
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}

	//================================================
	//描画の調整を元に戻す
	//================================================

	// ライトを有効に戻す
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// Zの比較方法変更。
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	// Zバッファに書き込む
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	
	// アルファテスト無効に戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//===================================================================================================================
// 死亡フラグ設定処理
//===================================================================================================================
void CBillboard::SetDeath()
{
	CObject::SetDeath(); // オブジェクト死亡フラグ設定処理
}

//===================================================================================================================
// テクスチャを割り当てる
//===================================================================================================================
void CBillboard::bindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;//指定したアドレスを保存
}

//===================================================================================================================
// 位置取得
//===================================================================================================================
const D3DXVECTOR3& CBillboard::GetPos() const
{
	return m_Pos;
}

//===================================================================================================================
// 位置設定
//===================================================================================================================
void CBillboard::SetPos(D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//===================================================================================================================
// 1f前の位置取得
//===================================================================================================================
const D3DXVECTOR3& CBillboard::GetPosOld() const
{
	return m_PosOld;
}

//===================================================================================================================
// 1f前の位置設定
//===================================================================================================================
void CBillboard::SetPosOld(D3DXVECTOR3 PosOld)
{
	m_PosOld = PosOld;
}

//===================================================================================================================
// 召喚位置取得
//===================================================================================================================
const D3DXVECTOR3& CBillboard::GetSupportPos() const
{
	return m_SupportPos;
}

//===================================================================================================================
// 召喚位置設定
//===================================================================================================================
void CBillboard::SetSupportPos(D3DXVECTOR3 Pos)
{
	m_SupportPos = Pos;
}

//===================================================================================================================
// サイズ取得
//===================================================================================================================
const D3DXVECTOR3& CBillboard::GetSize() const
{
	return m_Size;
}

//===================================================================================================================
// サイズ設定
//===================================================================================================================
void CBillboard::SetSize(D3DXVECTOR3 Size)
{
	m_Size = Size;
}

//===================================================================================================================
// 元のサイズ取得
//===================================================================================================================
const D3DXVECTOR3& CBillboard::GetFormarSize() const
{
	return m_FormarSize;
}

//===================================================================================================================
// 元のサイズ設定
//===================================================================================================================
void CBillboard::SetFormarSize(D3DXVECTOR3 FormarSize)
{
	m_FormarSize = FormarSize;
}

//===================================================================================================================
// 拡大率設定
//===================================================================================================================
void CBillboard::SetScale(D3DXVECTOR3 Scale)
{
	m_Scale = Scale;
}

//===================================================================================================================
// 拡大率取得
//===================================================================================================================
const D3DXVECTOR3& CBillboard::GetScale() const
{
	return m_Scale;
}

//===================================================================================================================
// 加算拡大率設定
//===================================================================================================================
void CBillboard::SetUseAddScale(bool bUse, D3DXVECTOR3 AddScale)
{
	m_bUseAddScale = bUse;
	m_AddScale - AddScale;
}

//===================================================================================================================
// 加算拡大率取得
//===================================================================================================================
const D3DXVECTOR3& CBillboard::GetAddScale() const
{
	return m_AddScale;
}

//===================================================================================================================
// 色合い設定
//===================================================================================================================
void CBillboard::SetColor(D3DXCOLOR col)
{
	m_Col = col;
}

//===================================================================================================================
// 色合い取得
//===================================================================================================================
const D3DXCOLOR & CBillboard::GetColor() const
{
	return m_Col;
}

//===================================================================================================================
// テクスチャ番号設定
//===================================================================================================================
void CBillboard::SetTextureIndex(int nIdx)
{
	m_nTextureIndex = nIdx;
}

//===================================================================================================================
// テクスチャ番号取得
//===================================================================================================================
const int& CBillboard::GetTextureIndex() const
{
	return m_nTextureIndex;
}

//===================================================================================================================
// アニメーション設定
//===================================================================================================================
void CBillboard::SetAnimInfo(int nMaxAnimationPattern, int nAnimationChange, D3DXCOLOR col, bool bAnim)
{
	m_Col = col;         // 色合い
	m_nMaxAnimationPattern = nMaxAnimationPattern; // アニメーションパターン最大値
	m_bAnimFlag = bAnim; // アニメーションをするかどうか
	m_fAnimationSplit = (float)(1.0f / m_nMaxAnimationPattern); // １分割当たりのアニメーション範囲（分割数)
	m_nAnimationChange = nAnimationChange; // アニメーションを次に進めるカウント数
}

//===================================================================================================================
// 重力設定
//===================================================================================================================
void CBillboard::SetUseGravity(float fGravity)
{
	m_fGravityPower = fGravity;
	m_bUseGravity = true;      
}

//===================================================================================================================
// 加算重力設定
//===================================================================================================================
void CBillboard::SetAddGravity(float fAddGravity)
{
	m_fAddGravity = fAddGravity;
}

//===================================================================================================================
// 加速設定
//===================================================================================================================
void CBillboard::SetUseAddSpeed(bool bUseAddSpeed, bool bMultiplication, float fAddSpeed)
{
	m_bUseAddSpeed = bUseAddSpeed;       // 加算速度を使用するかどうか
	m_bMultiplication = bMultiplication; // 乗算速度を使用するかどうか
	m_fAddSpeed = fAddSpeed;             // 加算速度を設定する
}

//===================================================================================================================
// ポリゴン回転設定
//===================================================================================================================
void CBillboard::SetUsePolygonRot(bool bUse, float fRotPower)
{
	m_bUsePolygonRot = bUse; // 使用するかどうか
	m_fPolygonRotPower = fRotPower; // 回転力
}

//===================================================================================================================
// 描画するかどうか取得
//===================================================================================================================
const bool& CBillboard::GetUseDraw() const
{
	return m_bDraw;
}

//===================================================================================================================
// 描画するかどうか設定
//===================================================================================================================
void CBillboard::SetUseDraw(bool bDraw)
{
	m_bDraw = bDraw;
}

//===================================================================================================================
// 体力取得
//===================================================================================================================
const int& CBillboard::GetLife() const
{
	return m_nLife;
}

//===================================================================================================================
// 体力設定
//===================================================================================================================
void CBillboard::SetLife(int nLife)
{
	m_nLife = nLife;
}

//===================================================================================================================
// 最大体力取得
//===================================================================================================================
const int& CBillboard::GetMaxLife() const
{
	return m_nMaxLife;
}

//===================================================================================================================
// 最大体力設定
//===================================================================================================================
void CBillboard::SetMaxLife(int nLife)
{
	m_nMaxLife = nLife;
}
