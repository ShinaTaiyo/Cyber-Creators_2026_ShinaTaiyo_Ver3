//===================================================================================================================
//
// ２０２５年６月１日：コードを綺麗にする[bgModel.h]
// Author:ShinaTaiyo
//
//===================================================================================================================

//*******************************************************************************************************************
// ２重インクルード防止
//*******************************************************************************************************************
#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

//*******************************************************************************************************************
// インクルード
//*******************************************************************************************************************
#include "main.h"
#include "object.h"

//*******************************************************************************************************************
// ビルボードクラス
//*******************************************************************************************************************
class CBillboard : public CObject
{
public:
	// === 特殊関数 ===

	// コンストラクタ
	CBillboard(
		int nPri = 6, // 自分で決めるプライオリティ
		bool bUseintPri = false, // 自分でプライオリティを決めるかどうか
		CObject::TYPE type = CObject::TYPE::NONE, // タイプ
		CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_BILLBOARD // オブジェクトタイプ
	);

	~CBillboard(); // デストラクタ
     
	// === ライフサイクルメンバ関数 ===

	HRESULT Init(void) override; //  初期化処理
	void Uninit(void) override;  //  終了処理
	void Update(void) override;  //  更新処理
	void Draw(void) override;    //  描画処理
	void SetDeath() override;    //  死亡フラグ設定処理

	// === メンバ関数 ===

	void bindTexture(LPDIRECT3DTEXTURE9 pTexture); // テクスチャを割り当てる

	// 位置
	const D3DXVECTOR3& GetPos() const; // 取得
	void SetPos(D3DXVECTOR3 pos);      // 設定

	// 1f前の位置
	const D3DXVECTOR3& GetPosOld() const; // 取得
	void SetPosOld(D3DXVECTOR3 PosOld);   // 設定

	// 召喚位置
	const D3DXVECTOR3& GetSupportPos() const; // 取得
	void SetSupportPos(D3DXVECTOR3 Pos);      // 設定

	// 移動量
	void SetMove(D3DXVECTOR3 Move) { m_Move = Move; } // 設定
	D3DXVECTOR3& GetMove() { return m_Move; }         // 取得

	// サイズ
	const D3DXVECTOR3& GetSize() const; // 取得
	void SetSize(D3DXVECTOR3 Size);     // 設定

	// 元のサイズ
	const D3DXVECTOR3& GetFormarSize() const;   // 取得
	void SetFormarSize(D3DXVECTOR3 FormarSize); // 設定

	// 拡大率
	void SetScale(D3DXVECTOR3 Scale);    // 設定
	const D3DXVECTOR3& GetScale() const; // 取得

	// 加算拡大率
	void SetUseAddScale(bool bUse, D3DXVECTOR3 AddScale); // 設定
	const D3DXVECTOR3& GetAddScale() const; // 取得

	// 色合い
	void SetColor(D3DXCOLOR col);       // 設定
	const D3DXCOLOR & GetColor() const; // 取得

	// テクスチャ番号
	void SetTextureIndex(int nIdx);      // 設定
	const int & GetTextureIndex() const; // 取得
	void SetAnimInfo(int nMaxAnimationPattern, int nAnimationChange,D3DXCOLOR col,bool bAnim); // アニメーション関係設定
	void SetAnim(int nAnim) { m_nAnimaionPattern = nAnim; } // アニメーションパターン設定
	D3DXMATRIX& GetMtxWorld() { return m_mtxWorld; } // ワールドマトリックス取得
	LPDIRECT3DVERTEXBUFFER9& GetVtxBuff() { return m_pVtxBuff; } // 頂点バッファ取得
	LPDIRECT3DTEXTURE9& GetTexture() { return m_pTexture; }      // テクスチャ取得

	void SetUseGravity(float fGravity); // 重力を設定
	void SetAddGravity(float fAddGravity); // 加算重力設定
	void SetUseAddSpeed(bool bUseAddSpeed, bool bMultiplication, float fAddSpeed); // 加速するかどうか設定
	void SetUsePolygonRot(bool bUse, float fRotPower); // ポリゴンを回転させるかどうかを設定

	// 描画するかどうか
	const bool& GetUseDraw() const; // 取得
	void SetUseDraw(bool bDraw);    // 設定

	// 体力
	const int& GetLife() const; // 取得
	void SetLife(int nLife);    // 設定

	// 最大体力
	const int& GetMaxLife() const; // 取得
	void SetMaxLife(int nLife);    // 設定
private:	
	// === メンバ変数 ===

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff; // 頂点バッファへのポインタ

	LPDIRECT3DTEXTURE9 m_pTexture;      // テクスチャへのポインタ

	D3DXVECTOR3
		m_Pos,    // 位置
		m_PosOld, // 1f前の位置
		m_Move,   // 移動量
		m_Rot,    // 向き!
		m_Scale,  // 拡大率
		m_AddScale,   // 加算拡大率
		m_SupportPos, // 召喚位置を設定
		m_Size,       // サイズ
		m_FormarSize; // 元のサイズ

	D3DXMATRIX m_mtxWorld; // ワールド変換行列

	D3DXCOLOR m_Col; // 色合い

	int
		m_nLife,    // 体力
		m_nMaxLife, // 最大体力
		m_nAnimationCnt,    //アニメーションカウント
		m_nAnimaionPattern, // アニメーションパターン
		m_nMaxAnimationPattern, // アニメーションパターン最大数
		m_nAnimationChange,     // アニメーションを変えるカウント数
		m_nTextureIndex, // テクスチャ番号!
		m_nCntTime;      // 出現してからの時間カウント

	float
		m_fSpeed,           // 速度
		m_fAnimationSplit,  //アニメーション１分割当たりの値
		m_fAddRot,          // 向きの補正
		m_fStartRot,        // 最初の向き
		m_fPolygonRotPower, // ポリゴンの回転量
		m_fGravityPower, // 重力の大きさ
		m_fAddGravity,   // 加算する重力
		m_fGravity,      // 重力
		m_fAddSpeed;     // 加速度

	bool 
        m_bDraw,           // 描画するかどうか
        m_bAnimFlag,       // アニメーションをするかどうか
        m_bUseGravity,     // 重力を使用するかどうか
		m_bUseAddScale,    // 加算拡大を使用するかどうか
        m_bUseAddSpeed,    // 加速を使用するかどうか
        m_bUsePolygonRot,  // ポリゴンを回転させるかどうか
        m_bMultiplication; // 速度を乗算させるかどうか
};


#endif