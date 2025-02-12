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
	CBillboard(int nPriority = 1);                                              //コンストラクタ
	~CBillboard();                                                              //デストラクタ
	HRESULT Init(void) override;                                                //初期化処理
	void Uninit(void) override;                                                 //終了処理
	void Update(void) override;                                                 //更新処理
	void Draw(void) override;                                                   //描画処理
	void bindTexture(LPDIRECT3DTEXTURE9 pTexture);                              //テクスチャを割り当てる

	bool CollisionSquare(D3DXVECTOR3 pos,D3DXVECTOR3 VtxMax,D3DXVECTOR3 VtxMin);//正方形の当たり判定を取る

	D3DXVECTOR3 & GetPos() { return m_Pos; }                                    //位置を取得する
	D3DXVECTOR3& GetPosOld() { return m_PosOld; }                               //1f前の位置を取得
	void SetPos(D3DXVECTOR3 pos) { m_Pos = pos; }                               //位置を設定する
	void SetSupportPos(D3DXVECTOR3 Pos) { m_SupportPos = Pos; }                 //召喚位置を設定する
	D3DXVECTOR3& GetSupportPos() { return m_SupportPos; }                       //召喚位置を取得する
	void SetMove(D3DXVECTOR3 Move) { m_Move = Move; }                           //移動量を設定する
	D3DXVECTOR3& GetMove() { return m_Move; }                                   //移動量を取得する
	float &GetWidth() { return m_fWidth; }                                      //横幅を取得する
	float &GetHeight() { return m_fHeight; }                                    //高さを取得する
	float& GetFormarWidth() { return m_fFormarWidth; }                          //元の横幅を取得する
	float& GetFormarHeight() { return m_fFormarHeight; }                        //元の高さを取得する
	void SetSize(float fWidth, float fHeight);                                  //サイズを設定する
	void SetFormarSize(float fFormarWidth, float fFormarHeight);                //元の大きさを設定する
	void SetColor(D3DXCOLOR col);                                               //色合いを設定する
	D3DXCOLOR GetColor() { return m_Col; }                                      //色を取得する
	void SetTextureIndex(int nIdx) { m_nTextureIndex = nIdx; }//テクスチャ番号をセットする
	int GetTextureIndex() { return m_nTextureIndex; }         //テクスチャ番号を取得する
	void SetAnimInfo(int nMaxAnimationPattern, int nAnimationChange,D3DXCOLOR col,bool bAnim);//アニメーション関係の設定
	void SetAnim(int nAnim) { m_nAnimaionPattern = nAnim; }   //アニメーション番号を設定
	D3DXMATRIX& GetMtxWorld() { return m_mtxWorld; }          //ワールドマトリックスを取得する
	LPDIRECT3DVERTEXBUFFER9& GetVtxBuff() { return m_pVtxBuff; }//頂点バッファを取得する
	LPDIRECT3DTEXTURE9& GetTexture() { return m_pTexture; }     //テクスチャを取得する

	void SetScaling(float fAddScale);//拡大をするかどうか
	void SetUseEffect(int nSetEffectLife, D3DXVECTOR2 Size, D3DXCOLOR Col);//エフェクトを使用するかどうか
	void SetUseMtxChild(D3DXMATRIX* pMtxParent,bool bUse);//子マトリックスを設定
	void SetUseGravity(float fGravity); //重力を設定
	void SetAddGravity(float fAddGravity) { m_fAddGravity = fAddGravity; };//加算する重力を設定
	void SetUseHorming(float fSpeed);   //ホーミングを設定
	void SetUseAddSpeed(bool bUseAddSpeed, bool bMultiplication, float fAddSpeed);//加速を設定

	D3DXVECTOR3& GetMtxTransPos() {return m_TransPos; }                                //子マトリックスのワールド座標を取得する
	bool GetUseMtxChild() { return m_bMtxChild; }                               //子マトリックスを使っているかどうかを取得する    

	//====================================================
	//カーブ関係
	//====================================================
	void SetUseCurve(float fRot, float fSpeed, float fCurveSpeed);//カーブをするかどうか
	void SetUseLengthCurve(bool bUse, float fStartRot, float fCurveSpeed, float fAddCurveLength);
    //====================================================================================================================

	//====================================================
	//回転関係
	//====================================================
	void SetUsePolygonRot(bool bUse, float fRotPower) { m_bUsePolygonRot = bUse; m_fPolygonRotPower = fRotPower; }//ポリゴンを回転させるかどうかを設定
	//====================================================================================================================

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
	void HormingProcess();                                                      //ホーミングの処理
	static const int m_nMAX_MAT = 50;                                           //マテリアルの最大数
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;                                         //頂点バッファへのポインタ
	LPDIRECT3DTEXTURE9 m_pTexture;                                              //テクスチャへのポインタ
	D3DXVECTOR3 m_Pos;                                                          //位置
	D3DXVECTOR3 m_PosOld;                                                       //1f前の位置
	D3DXVECTOR3 m_SupportPos;                                                   //召喚位置を設定
	D3DXVECTOR3 m_Move;                                                         //移動量
	D3DXVECTOR3 m_Rot;                                                          //向き
	D3DXMATRIX m_mtxWorld;                                                      //ワールド変換行列
	D3DXCOLOR m_Col;                                                            //色合い
   
	float m_fWidth;                                                             //横幅
	float m_fFormarWidth;                                                       //元の横幅
	float m_fHeight;                                                            //高さ
	float m_fFormarHeight;                                                      //元の高さ

	int m_nLife;                                                                //体力
	int m_nMaxLife;                                                             //最大体力

	float m_fAnimationSplit;                                                    //アニメーション１分割当たりの値
	int m_nAnimaionPattern;                                                     //アニメーションパターン
	int m_nAnimationCnt;                                                        //アニメーションカウント
	int m_nMaxAnimationPattern;                                                 //アニメーションパターンの最大数
	int m_nAnimationChange;                                                     //アニメーションを変えるカウント数
	bool m_bAnimFlag;                                                           //アニメーションをするかどうか

	int m_nTextureIndex;                                                        //テクスチャ番号

	bool m_bScaling;        //拡縮をするかどうか  
	float m_fAddScale;      //拡大量を設定
	float m_fScale;         //拡大率

	bool m_bUse;            //使用状態

	float m_fSpeed;          //早さ

	int m_nCntTime;          //出現してからの時間をカウントする
	//========================================
	//カーブ系
	//========================================
	bool m_bUseCurve;           //カーブ弾にするかどうか
	bool m_bUseLengthCurve;     //視点を軸にカーブするかどうか
	float m_fAddCurveLength;    //支点からの距離を伸ばす値
	float m_fCurveSpeed;        //カーブするスピード
	float m_fSupportCurveLength;//視点を軸に回転する距離を決める
	float m_fAddRot;            //向きの補正
	float m_fStartRot;          //最初の向き
	//=======================================================================================================

	//========================================
	//回転関係
	//========================================
	bool m_bUsePolygonRot;   //ポリゴンを回転させるかどうか
	float m_fPolygonRotPower;//ポリゴンの回転量
	//=======================================================================================================


	//========================================
	//描画関係
	//========================================
	bool m_bDraw;               //描画するかどうか
	//=======================================================================================================

	bool m_bSetEffect;          //エフェクトを使用するかどうか
	D3DXCOLOR m_SetEffectCol;   //設定するエフェクトの色合い
	int m_nSetEffectLife;       //設定するエフェクトの体力
	D3DXVECTOR2 m_SetEffectSize;//設定するエフェクトのサイズ

	//===============================
	//親マトリックス系
	//===============================
	D3DXMATRIX* m_pMtxParent; //親マトリックスへのポインタ
	bool m_bMtxChild;        //子マトリックスにするかどうか
	D3DXVECTOR3 m_TransPos;  //子マトリックスにしている場合の位置
	//================================================================================

	//===============================
	//重力系
	//===============================
	bool m_bUseGravity;      //重力を使用するかどうか
	float m_fGravityPower;   //重力の大きさ
	float m_fAddGravity;     //加算する重力
	float m_fGravity;        //重力
	//================================================================================

	//===============================
	//ホーミング系
	//===============================
	bool m_bUseHorming;      //ホーミングを使用するかどうか
	float m_fRotMove;        //向きへの移動量
	//================================================================================

	//===============================
	//加速系
	//===============================
	bool m_bUseAddSpeed;     //加速を使用するかどうか
	float m_fAddSpeed;       //加速度
	bool m_bMultiplication;    //速度を乗算させるかどうか
	//================================================================================

};
//===================================================================================


#endif