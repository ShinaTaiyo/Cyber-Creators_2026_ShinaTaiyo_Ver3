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
#include "game.h"
#include "scene.h"
#include "objectX.h"
//=============================================================================================

//==================================================
//静的メンバ初期化
//==================================================

//=============================================================================================

//==================================================
//コンストラクタ
//==================================================
CBillboard::CBillboard(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CObject(nPri, bUseintPri, type, ObjType) ,
m_bAnimFlag(false),m_bDraw(false),m_bMultiplication(false),
m_bUseAddSpeed(false),m_bUseCurve(false),m_bUseGravity(false),m_bUseHorming(false),m_bUseLengthCurve(false),m_bUsePolygonRot(false),
m_fAddCurveLength(0.0f),m_fAddGravity(0.0f),m_fAddRot(0.0f),m_fAddSpeed(0.0f),m_fAnimationSplit(0.0f),m_fCurveSpeed(0.0f),m_fFormarHeight(0.0f),
m_fFormarWidth(0.0f),m_fGravity(0.0f),m_fGravityPower(0.0f),m_fHeight(0.0f),m_fPolygonRotPower(0.0f),m_fRotMove(0.0f),m_fStartRot(0.0f),
m_fSupportCurveLength(0.0f),m_fWidth(0.0f),m_nMaxAnimationPattern(0),m_nAnimationChange(0),m_nAnimationCnt(0),m_nCntTime(0),m_nLife(0),m_nMaxLife(0),
m_nTextureIndex(0),m_nAnimaionPattern(0), m_nCntBlinkingFrame(0), m_nMaxBlinkingFrame(0), m_bBlinkingAim(false), m_bUseBlinking(false), 
m_fLimitBlinkingRatio(0.0f),m_Pos(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_PosOld(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_SupportPos(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_Move(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_Rot(D3DXVECTOR3(0.0f,0.0f,0.0f)), m_mtxWorld(),
m_Col(D3DXCOLOR(1.0f,1.0f,1.0f,1.0f)),m_Scale(D3DXVECTOR3(1.0f,1.0f,1.0f)),m_fSpeed(0.0f),m_bUseAddScale(false)
{
	m_pTexture = nullptr;//テクスチャへのポインタを初期化
	m_pVtxBuff = nullptr;//頂点バッファへのポインタを初期化
}
//=============================================================================================

//==================================================
//デストラクタ
//==================================================
CBillboard::~CBillboard()
{
	m_mtxWorld = {};//ワールドマトリックスを初期化
}
//=============================================================================================

//==================================================
//初期化処理
//==================================================
HRESULT CBillboard::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//デバイスへのポインタと取得

	m_Col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);               //色合い
	 
	m_Pos = D3DXVECTOR3(0.0f,0.0f,0.0f);                     //位置
	m_PosOld = D3DXVECTOR3(0.0f,0.0f,0.0f);                  //1f前の位置
	m_SupportPos = D3DXVECTOR3(0.0f,0.0f,0.0f);              //召喚位置
	m_Rot = D3DXVECTOR3(0.0f,0.0f,0.0f);                     //向き
	m_Move = D3DXVECTOR3(0.0f,0.0f,0.0f);                    //移動量
	m_mtxWorld = {};                                         //マトリックスワールド
	m_bDraw = true;                                          //基本的には描画する

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
	pVtx[0].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);

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
	//テクスチャのポインタを初期化（テクスチャクラスで管理しているので、解放しなくても大丈夫)
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

	//========================================
	//支点を軸にカーブがONだったら
	//========================================
	if (m_bUseLengthCurve == true)
	{
		m_fAddRot += m_fCurveSpeed;//向きをカーブ速度分加算する
		m_fSupportCurveLength += m_fAddCurveLength;//カーブする距離を加算する
		m_Pos.x = sinf(m_fAddRot + m_fStartRot) * m_fSupportCurveLength + m_SupportPos.x;
		m_Pos.y = cosf(m_fAddRot + m_fStartRot) * m_fSupportCurveLength + m_SupportPos.y;
	}
	//=============================================================================================

	//========================================
	//加算拡大率がONになっていたら
	//========================================
	if (m_bUseAddScale == true)
	{//拡大率を加算する
		m_Scale += m_AddScale;
	}
	//=============================================================================================

	//========================================
	//重力がONになっていたら
	//========================================
	if (m_bUseGravity == true)
	{//重力を加算し、現在の重力分Y座標を加算する
		m_fGravityPower += m_fAddGravity;
		m_Move.y += m_fGravityPower;
	}
	//==============================================================================================

	//点滅の処理
	BlinkingProcess();

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
	m_PosOld = m_Pos;//1f前の位置
	m_Pos += m_Move; //位置の更新
	//==============================================================================================

	//========================================
	//ポリゴン回転処理
	//========================================
	if (m_bUsePolygonRot == true)
	{//向きを加算する
		m_Rot.z += m_fPolygonRotPower;
	}
	//==============================================================================================

	if (m_bAnimFlag == true)
	{//アニメーションをするなら
		if (m_nAnimationCnt % m_nAnimationChange == 0)
		{//アニメーションカウントがアニメーションを変更するフレーム数に達したら
			m_nAnimaionPattern++;//アニメーションパターンをインクリメント

			if (m_nAnimaionPattern >= m_nMaxAnimationPattern)
			{//アニメーションパターンが最大アニメーションパターンに達したらリセットする
				m_nAnimaionPattern = 0;
			}
		}
	}

	float fLength = sqrtf(m_fWidth * m_fWidth + m_fHeight * m_fHeight) / 2.0f;//中心点からポリゴンの角との距離を取る
	float fAngle = atan2f(m_fWidth, m_fHeight);                               //中心点から角への角度を求める

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

	//頂点カラーの設定
	pVtx[0].col = m_Col;
	pVtx[1].col = m_Col;
	pVtx[2].col = m_Col;
	pVtx[3].col = m_Col;

	//テクスチャ座標の設定（アニメーション分割数×現在のアニメーションパターン
	pVtx[0].tex = D3DXVECTOR2(m_fAnimationSplit * (float)(m_nAnimaionPattern), 0.0f);
	pVtx[1].tex = D3DXVECTOR2(m_fAnimationSplit + m_fAnimationSplit * (float)(m_nAnimaionPattern), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(m_fAnimationSplit * (float)(m_nAnimaionPattern), 1.0f);
	pVtx[3].tex = D3DXVECTOR2(m_fAnimationSplit + m_fAnimationSplit * (float)(m_nAnimaionPattern), 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	if (m_nLife <= 0)
	{//体力が０になったら死亡フラグを発動する
		SetDeath();
	}

}
//=============================================================================================

//==================================================
//描画処理
//==================================================
void CBillboard::Draw(void)
{
	D3DXMATRIX mtxTrans,mtxRot,mtxScale;//計算用マトリックス
	D3DXMATRIX mtxParent = {};
	D3DXMATRIX mtxView;//ビューマトリックス取得用
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

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

	if (m_bUseAddScale == true)
	{
		//拡大率を設定
		D3DXMatrixScaling(&mtxScale, m_Scale.x, m_Scale.y, m_Scale.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);
	}
	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, GetPos().x, GetPos().y, GetPos().z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

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
//死亡フラグ設定処理
//==================================================
void CBillboard::SetDeath()
{
	CObject::SetDeath();
}
//=============================================================================================

//==================================================
//テクスチャを割り当てる処理
//==================================================
void CBillboard::bindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;//指定したアドレスを保存
}
//============================================================================================================================================

//==================================================
//正方形の当たり判定を取る
//==================================================
bool CBillboard::CollisionSquare(D3DXVECTOR3 pos, D3DXVECTOR3 VtxMax, D3DXVECTOR3 VtxMin)
{
	D3DXVECTOR3 ComparisonPos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	D3DXVECTOR3 ComparisonVtxMin = D3DXVECTOR3(0.0f,0.0f,0.0f);
	D3DXVECTOR3 ComparisonVtxMax = D3DXVECTOR3(0.0f,0.0f,0.0f);

	ComparisonPos.x = (float)(floor(pos.x));//小数点を切り捨てた位置を代入
	ComparisonPos.y = (float)(floor(pos.y));//小数点を切り捨てた位置を代入
	ComparisonPos.z = (float)(floor(pos.z));//小数点を切り捨てた位置を代入

	ComparisonVtxMax.x = (float)(floor(VtxMax.x));//小数点を切り捨てた最大頂点を代入する
	ComparisonVtxMax.y = (float)(floor(VtxMax.y));//小数点を切り捨てた最大頂点を代入する
	ComparisonVtxMax.z = (float)(floor(VtxMax.z));//小数点を切り捨てた最大頂点を代入する

	ComparisonVtxMin.x = (float)(floor(VtxMin.x));//小数点を切り捨てた最小頂点を代入する
	ComparisonVtxMin.y = (float)(floor(VtxMin.y));//小数点を切り捨てた最小頂点を代入する
	ComparisonVtxMin.z = (float)(floor(VtxMin.z));//小数点を切り捨てた最小頂点を代入する


	D3DXVECTOR2 MySize = D3DXVECTOR2(0.0f,0.0f);//ポリゴンのサイズ代入用

	//==================================
	//判定用にキャストする
	//==================================
	MySize.x = (float)(floor(m_fWidth));//小数点を切り捨てた横幅を代入する
	MySize.y = (float)(floor(m_fHeight));//小数点を切り捨てた高さを代入する
	//==========================================================================================================

	bool bCollision = false;                         //地面に着地しているか
	
	if (m_Pos.x + MySize.x / 2 >= ComparisonPos.x + ComparisonVtxMin.x
		&& m_Pos.x - MySize.x / 2 <= ComparisonPos.x + ComparisonVtxMax.x
		&& m_Pos.y + MySize.y / 2 >= ComparisonPos.y + ComparisonVtxMin.y
		&& m_Pos.y - MySize.y / 2 <= ComparisonPos.y + ComparisonVtxMax.y
		&& m_Pos.z + MySize.x / 2 >= ComparisonPos.z + ComparisonVtxMin.z
		&& m_Pos.z - MySize.x / 2 <= ComparisonPos.z + ComparisonVtxMax.z)
	{//当たり判定を行う
		bCollision = true;
	}
	return bCollision;
}
//============================================================================================================================================

//==================================================
//サイズを設定する
//==================================================
void CBillboard::SetSize(float fWidth, float fHeight)
{
	m_fWidth = fWidth;//横幅
	m_fHeight = fHeight;//高さ
}
//============================================================================================================================================

//==========================================================================
//元のサイズを設定する
//==========================================================================
void CBillboard::SetFormarSize(float fFormarWidth, float fFormarHeight)
{
	m_fFormarWidth = fFormarWidth;//基準の横幅
	m_fFormarHeight = fFormarHeight;//基準の高さ
}
//============================================================================================================================================

//==================================================
//色合いを設定
//==================================================
void CBillboard::SetColor(D3DXCOLOR col)
{
	m_Col = col;//色合いを設定
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
	m_fAnimationSplit = (float)(1.0f / m_nMaxAnimationPattern);//１分割当たりのアニメーション範囲（分割数)
}
//================================================================================================================

//===============================================================
//基準点を軸に回転するカーブを設定
//===============================================================
void CBillboard::SetUseLengthCurve(bool bUse, float fStartRot, float fCurveSpeed, float fAddCurveLength, float fSupportLength)
{
	m_bUseLengthCurve = bUse;                //使用するかどうか
	m_fStartRot = fStartRot;                 //最初の向き
	m_fCurveSpeed = fCurveSpeed;             //カーブ速度
	m_fAddCurveLength = fAddCurveLength;     //加算する距離
	m_fSupportCurveLength = fSupportLength;  //基準となる最初の基準点からの距離
}
//================================================================================================================

//===============================================================
//重力を設定
//===============================================================
void CBillboard::SetUseGravity(float fGravity)
{
	m_fGravityPower = fGravity;//重力の大きさ
	m_bUseGravity = true;      //重力を使用する
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
	m_bUseAddSpeed = bUseAddSpeed;       //加算速度を使用するかどうか
	m_bMultiplication = bMultiplication; //乗算速度を使用するかどうか
	m_fAddSpeed = fAddSpeed;             //加算速度を設定する
}
//================================================================================================================

//===============================================================
//ホーミングの処理
//===============================================================
void CBillboard::HormingProcess()
{
	float fRotDest = 0.0f;                               //目的の位置への角度
	float fRotDiff = 0.0f;                               //現在の角度と目的の角度の差分
	float fVXaim = 0.0f;                                 //Xベクトル
	float fVYaim = 0.0f;                                 //Yベクトル
	D3DXVECTOR3 TargetPos = D3DXVECTOR3(0.0f,0.0f,0.0f); //ターゲットの位置
	//===================================
	//XZベクトルを出す
	//===================================
	if (CScene::GetMode() == CScene::MODE_GAME)
	{//シーンがゲームなら
		TargetPos = CGame::GetPlayer()->GetPosInfo().GetPos(); //プレイヤーの位置をターゲットとする
		fVXaim = TargetPos.x - m_Pos.x;                        //ターゲットの位置とビルボードの位置のXベクトルを求める
		fVYaim = TargetPos.y - m_Pos.y;                        //ターゲットの位置とビルボードの位置のYベクトルを求める

		fRotDest = atan2f(fVXaim, fVYaim);                     //XとYのベクトルを元に角度を求める

		//==========================================================================================================================================================

		//現在の移動方向の角度と、目的の移動方向の角度の差分を求める
		fRotDiff = fRotDest - m_fRotMove;

		//===============================
		//角度差分の修正
		//===============================
		
		//向きの差分の調整(3.14を超えたら近い向きに補正しジンバルロックを回避)
		if (fRotDiff > D3DX_PI)
		{
			fRotDiff -= D3DX_PI * 2;
		}
		else if (fRotDiff < -D3DX_PI)
		{
			fRotDiff += D3DX_PI * 2;
		}

		//==============================================================================================
		m_fRotMove += fRotDiff * 0.05f;//移動方向の補正

		//移動量の更新
		m_Move.x = sinf(m_fRotMove) * m_fSpeed;
		m_Move.y = cosf(m_fRotMove) * m_fSpeed;
	}
}
//================================================================================================================

//===================================================================
//点滅処理
//===================================================================
void CBillboard::BlinkingProcess()
{
	if (m_bUseBlinking == true)
	{//点滅させるなら
		if (m_bBlinkingAim == false)
		{//falseならインクリメント
			m_nCntBlinkingFrame++;
		}
		else
		{//trueならデクリメント
			m_nCntBlinkingFrame--;
		}

		if (m_nCntBlinkingFrame >= m_nMaxBlinkingFrame || m_nCntBlinkingFrame <= 0)
		{//点滅フレームのカウントが最大以上、又は０以下になったら点滅方向を逆にする
			m_bBlinkingAim = m_bBlinkingAim ? false : true;
		}

		float fRatioBlinkingFrame = 0.0f;//点滅フレームの割合
		fRatioBlinkingFrame = float(m_nCntBlinkingFrame) / float(m_nMaxBlinkingFrame);//点滅フレーム
		m_Col.a = fRatioBlinkingFrame;//透明度を上記で求めた割合

		VERTEX_3D* pVtx;//3D頂点情報
		//頂点バッファをロックし、頂点情報へのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		//頂点カラーの設定
		pVtx[0].col = m_Col;
		pVtx[1].col = m_Col;
		pVtx[2].col = m_Col;
		pVtx[3].col = m_Col;

		//頂点バッファをアンロックする
		m_pVtxBuff->Unlock();

		if (m_fLimitBlinkingRatio > fRatioBlinkingFrame)
		{//点滅する透明度の最低値を下回ったらカウントする値を逆にする
			m_bBlinkingAim = false;
		}
	}
}
//================================================================================================================