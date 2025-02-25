//==================================================================================
//
//６月５日：ビルボードを作成する[billboard.h]
//Author:ShinaTaiyo
//
//===================================================================================
#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

//===================================================================================
//インクルード
//===================================================================================
#include "main.h"
#include "object.h"

//===================================================================================

//===================================================================================
//前方宣言
//===================================================================================
class CEnemy;
class CObjectX;

//===================================================================================
//ビルボードクラス
//===================================================================================
class CBillboard : public CObject
{
public:
	CBillboard(int nPri = 6, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::NONE, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_BILLBOARD);                                              //コンストラクタ
	~CBillboard();                                                              //デストラクタ
	HRESULT Init(void) override;                                                //初期化処理
	void Uninit(void) override;                                                 //終了処理
	void Update(void) override;                                                 //更新処理
	void Draw(void) override;                                                   //描画処理
	void SetDeath() override;                                                   //死亡フラグ設定処理
	void bindTexture(LPDIRECT3DTEXTURE9 pTexture);                              //テクスチャを割り当てる

	bool CollisionSquare(D3DXVECTOR3 pos,D3DXVECTOR3 VtxMax,D3DXVECTOR3 VtxMin);//正方形の当たり判定を取る

	//================================================
	//座標関係
	//================================================
	D3DXVECTOR3 & GetPos() { return m_Pos; }                                    //位置を取得する
	D3DXVECTOR3& GetPosOld() { return m_PosOld; }                               //1f前の位置を取得
	void SetPos(D3DXVECTOR3 pos) { m_Pos = pos; }                               //位置を設定する
	void SetSupportPos(D3DXVECTOR3 Pos) { m_SupportPos = Pos; }                 //召喚位置を設定する
	D3DXVECTOR3& GetSupportPos() { return m_SupportPos; }                       //召喚位置を取得する
	void SetMove(D3DXVECTOR3 Move) { m_Move = Move; }                           //移動量を設定する
	D3DXVECTOR3& GetMove() { return m_Move; }                                   //移動量を取得する
	//=======================================================================================================

	//================================================
	//サイズ関係
	//================================================
	float &GetWidth() { return m_fWidth; }                                      //横幅を取得する
	float &GetHeight() { return m_fHeight; }                                    //高さを取得する
	float& GetFormarWidth() { return m_fFormarWidth; }                          //元の横幅を取得する
	float& GetFormarHeight() { return m_fFormarHeight; }                        //元の高さを取得する
	void SetSize(float fWidth, float fHeight);                                  //サイズを設定する
	void SetFormarSize(float fFormarWidth, float fFormarHeight);                //元の大きさを設定する
	void SetScale(D3DXVECTOR3 Scale) { m_Scale = Scale; }                       //拡大率を設定
	const D3DXVECTOR3& GetScale() const { return m_Scale; }                     //拡大率を取得
	void SetUseAddScale(bool bUse, D3DXVECTOR3 AddScale) { m_AddScale = AddScale; m_bUseAddScale = bUse; }            //加算拡大率
	const D3DXVECTOR3& GetAddScale() const { return m_AddScale; }               //加算拡大率を取得
	//=======================================================================================================


	void SetColor(D3DXCOLOR col);                                                             //色合いを設定する
	D3DXCOLOR GetColor() { return m_Col; }                                                    //色を取得する
	void SetTextureIndex(int nIdx) { m_nTextureIndex = nIdx; }                                //テクスチャ番号をセットする
	int GetTextureIndex() { return m_nTextureIndex; }                                         //テクスチャ番号を取得する
	void SetAnimInfo(int nMaxAnimationPattern, int nAnimationChange,D3DXCOLOR col,bool bAnim);//アニメーション関係の設定
	void SetAnim(int nAnim) { m_nAnimaionPattern = nAnim; }                                   //アニメーション番号を設定
	D3DXMATRIX& GetMtxWorld() { return m_mtxWorld; }                                          //ワールドマトリックスを取得する
	LPDIRECT3DVERTEXBUFFER9& GetVtxBuff() { return m_pVtxBuff; }                              //頂点バッファを取得する
	LPDIRECT3DTEXTURE9& GetTexture() { return m_pTexture; }                                   //テクスチャを取得する

	void SetUseGravity(float fGravity);                                                       //重力を設定
	void SetAddGravity(float fAddGravity) { m_fAddGravity = fAddGravity; };                   //加算する重力を設定
	void SetUseHorming(float fSpeed);                                                         //ホーミングするかどうかを設定
	void SetUseAddSpeed(bool bUseAddSpeed, bool bMultiplication, float fAddSpeed);            //加速するかどうかを設定

	//====================================================
	//カーブ関係
	//====================================================
	void SetUseLengthCurve(bool bUse, float fStartRot, float fCurveSpeed, float fAddCurveLength,float fSupportLength);
    //====================================================================================================================

	//====================================================
	//回転関係
	//====================================================
	void SetUsePolygonRot(bool bUse, float fRotPower) { m_bUsePolygonRot = bUse; m_fPolygonRotPower = fRotPower; }//ポリゴンを回転させるかどうかを設定
	//====================================================================================================================

	//==============================
	//点滅関係
	//==============================
	void SetUseBlinking(bool bUse, int nMaxBlinkingFrame, float fLimitBlinkingRatio) {
		m_bUseBlinking = bUse;                                   //点滅を使用するかどうか
		m_nMaxBlinkingFrame = nMaxBlinkingFrame;                 //点滅最大フレーム
		m_fLimitBlinkingRatio = fLimitBlinkingRatio;             //点滅させる濃さのリミット
	}
	//==============================================================================================================================================================

	//====================================================
	//描画関係
	//====================================================
	void SetUseDraw(bool bDraw) { m_bDraw = bDraw; }                            //描画するかどうかを設定する
	bool& GetUseDraw() { return m_bDraw; }                                      //描画するかどうかを取得する
	//====================================================================================================================

	void SetLife(int nLife) { m_nLife = nLife; }                                //体力を設定
	void SetMaxLife(int nLife) { m_nMaxLife = nLife; }                          //最大体力を設定

	int& GetLife() { return m_nLife; }                                          //体力を取得
	int& GetMaxLife() { return m_nMaxLife; }                                    //最大体力を取得
private:			
	static const int m_nMAX_MAT = 50;                                           //マテリアルの最大数
	void HormingProcess();                                                      //ホーミングの処理

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;                                         //頂点バッファへのポインタ!
	LPDIRECT3DTEXTURE9 m_pTexture;                                              //テクスチャへのポインタ!

	//=======================================================
	//座標系
	//=======================================================
	D3DXVECTOR3 m_Pos;                                                          //位置!
	D3DXVECTOR3 m_PosOld;                                                       //1f前の位置!
	D3DXVECTOR3 m_SupportPos;                                                   //召喚位置を設定!
	D3DXVECTOR3 m_Move;                                                         //移動量!
	D3DXVECTOR3 m_Rot;                                                          //向き!
	float m_fSpeed;
    //==========================================================================================

	D3DXMATRIX m_mtxWorld;                                                      //ワールド変換行列!
	D3DXCOLOR m_Col;                                                            //色合い!

	//=======================================================
	//サイズ関係
	//=======================================================
	float m_fWidth;                                                             //横幅!
	float m_fFormarWidth;                                                       //元の横幅!
	float m_fHeight;                                                            //高さ!
	float m_fFormarHeight;                                                      //元の高さ!
	int m_nLife;                                                                //体力!
	int m_nMaxLife;                                                             //最大体力!
	D3DXVECTOR3 m_Scale;                                                        //拡大率

	D3DXVECTOR3 m_AddScale;                                                     //加算拡大率
	bool m_bUseAddScale;                                                        //加算拡大率を使用するかどうか
	//==========================================================================================

   
	//=======================================================
	//アニメーション関係
	//=======================================================
	float m_fAnimationSplit;                                                    //アニメーション１分割当たりの値!
	int m_nAnimaionPattern;                                                     //アニメーションパターン!
	int m_nAnimationCnt;                                                        //アニメーションカウント!
	int m_nMaxAnimationPattern;                                                 //アニメーションパターンの最大数!
	int m_nAnimationChange;                                                     //アニメーションを変えるカウント数!
	bool m_bAnimFlag;                                                           //アニメーションをするかどうか!
	int m_nTextureIndex;                                                        //テクスチャ番号!
	int m_nCntTime;          //出現してからの時間をカウントする!
	//==========================================================================================

	//========================================
	//カーブ系
	//========================================
	bool m_bUseCurve;           //カーブ弾にするかどうか!
	bool m_bUseLengthCurve;     //視点を軸にカーブするかどうか!
	float m_fAddCurveLength;    //支点からの距離を伸ばす値!
	float m_fCurveSpeed;        //カーブするスピード!
	float m_fSupportCurveLength;//視点を軸に回転する距離を決める!
	float m_fAddRot;            //向きの補正!
	float m_fStartRot;          //最初の向き!
	//=======================================================================================================

	//========================================
	//回転関係
	//========================================
	bool m_bUsePolygonRot;   //ポリゴンを回転させるかどうか!
	float m_fPolygonRotPower;//ポリゴンの回転量!
	//=======================================================================================================


	//========================================
	//描画関係
	//========================================
	bool m_bDraw;               //描画するかどうか!
	//=======================================================================================================

	//====================================================
	//点滅関係
	//====================================================
	int m_nCntBlinkingFrame;     //点滅用のカウント!
	int m_nMaxBlinkingFrame;     //点滅用の最大カウント数!
	bool m_bBlinkingAim;         //カウントを増やすか減らすかを決める!
	bool m_bUseBlinking;         //点滅させるかどうか!
	float m_fLimitBlinkingRatio; //点滅させる色の濃さのリミット!
	void BlinkingProcess();      //点滅させる処理
	//====================================================================================================================

	//===============================
	//重力系
	//===============================
	bool m_bUseGravity;      //重力を使用するかどうか!
	float m_fGravityPower;   //重力の大きさ!
	float m_fAddGravity;     //加算する重力!
	float m_fGravity;        //重力!
	//================================================================================

	//===============================
	//ホーミング系
	//===============================
	bool m_bUseHorming;      //ホーミングを使用するかどうか!
	float m_fRotMove;        //向きへの移動量!
	//================================================================================

	//===============================
	//加速系
	//===============================
	bool m_bUseAddSpeed;     //加速を使用するかどうか!
	float m_fAddSpeed;       //加速度!
	bool m_bMultiplication;  //速度を乗算させるかどうか!
	//================================================================================

};
//===================================================================================


#endif