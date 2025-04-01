//====================================================
//
//５月２０日：C++で基盤づくり[object2d.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _OBJECT2D_H_  
#define _OBJECT2D_H_

//======================
//インクルード
//======================
#include "main.h"
#include "object.h"
#include "renderer.h"
//==========================================

//======================
//マクロ定義
//======================
#define MAX_2DOBJECT (10)

//==========================================
//オブジェクト２Dクラス
//==========================================
class CObject2D : public CObject
{
public:

	//======================================
	//ポリゴンの設定タイプ
	//======================================
	enum class POLYGONTYPE
	{
	    NORMAL = 0,   //普通に出すポリゴン
	    SENTERROLLING,//中心点を軸に回転できるポリゴン
		DOWNSENTERROLLING,//中心点が下の回転できるポリゴン
	    DOWN,         //中心点が下側
	    LEFT,         //中心点が左側
	    RIGHT,        //中心点が右側
		MAX
	};
	//======================================================================================================

	CObject2D(int nPri = 6, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::NONE, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_2D);     //描画順設定用コンストラクタ
	~CObject2D() override;       //デストラクタ
	HRESULT Init() override;     //初期化処理
	void Uninit() override;      //終了処理
	void Update() override;      //更新処理
	void Draw() override;        //描画処理
	void SetDeath() override;    //死亡フラグ設定処理
	void BindTexture(LPDIRECT3DTEXTURE9 pTex);  //テクスチャを割り当てる

	//================================
	//回転系
	//================================
	D3DXVECTOR3& GetRot() { return m_rot; }                                 //向きを取得
	void SetRot(D3DXVECTOR3 Rot) { m_rot = Rot; }                           //向きを設定
	void SetPolygonRotSpeed(float fSpeed) { m_fPolygonRotSpeed = fSpeed; }  //回転速度を設定
	float& GetPolygonRotSpeed() { return m_fPolygonRotSpeed; }              //回転速度
	//========================================================================================================


	//===============
	//取得系
	//===============
	D3DXVECTOR3& GetPos() { return m_pos; }             //位置取得用
	D3DXVECTOR3 GetSupportPos() { return m_SupportPos; }//召喚位置を取得
	float GetWidth() { return m_fWidth; }               //横幅を取得
	float GetMaxWidth() { return m_fMaxWidth; }         //最大横幅を取得
	float GetHeight() { return m_fHeight; }             //高さを取得
	float GetMaxHeight() { return m_fMaxHeight; }       //最大高さを取得する
	D3DXCOLOR GetColor() { return m_col; }              //色合いを取得する
	//=======================================================================================================

	//===============
	//普通系
	//===============
	void SetUseFloating(float fFloatingRot,float fFloatingAddSpeed,float fLimitSpeed,bool bUseFloating)
	{
		m_fFloatingRot = fFloatingRot;          //浮く向き
		m_fFloatingAddSpeed = fFloatingAddSpeed;//加算する速度
		m_fFloatingLimitSpeed = fLimitSpeed;    //浮遊速度のリミット
		m_bUseFloating = bUseFloating;          //浮遊を使用するかどうか
	}
	//=======================================================================================================

	//移動量
	D3DXVECTOR3& GetMove() { return m_Move; }
	void SetMove(D3DXVECTOR3 move) { m_Move = move; }

	//ポリゴンの種類
	POLYGONTYPE GetPolygonType() { return m_PolygonType; }
	void SetPolygonType(POLYGONTYPE Type) { m_PolygonType = Type; }

	//===============
	//サイズ系
	//===============
	void SetScale(D3DXVECTOR2 Scale) { m_Scale = Scale; }      //拡大率を設定する
	void SetUseScale(bool bUse) { m_bUseScale = bUse; }        //拡大率を使用するかどうか

	//拡大率の加算量設定
	void SetUseAddScale(D3DXVECTOR2 AddScale, bool bUse) { m_AddScale = AddScale; m_bUseAddScale = bUse; m_bUseScale = true; }
	const D3DXVECTOR2& GetAddScale() const { return m_AddScale; }//拡大率の加算量を取得
	D3DXVECTOR2& GetScale() { return m_Scale; }//拡大率を取得
	//=======================================================================================================

	//===============
	//座標系
	//===============
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos;}                                 //位置を設定
	void SetSupportPos(D3DXVECTOR3 pos) { m_SupportPos = pos; }                  //召喚位置を設定
	void SetWidth(float fWidth) { m_fWidth = fWidth; }                           //横幅を設定
	void SetMaxWidth(float fWidth) { m_fMaxWidth = fWidth; }                     //最大横幅を設定
	void SetHeight(float fHeight) { m_fHeight = fHeight; }                       //高さを設定
	void SetMaxHeight(float fHeight) { m_fMaxHeight = fHeight; }                 //最大高さを設定
	void SetColor(D3DXCOLOR col, bool bAlphaOnly,float fAlpha);                  //色を設定
	void SetAnimInfo(int nMaxAnimationPattern, int nAnimationChange,bool bAnim); //アニメーション関係の設定
	void SetAnim(int nAnim) { m_nAnimaionPattern = nAnim; }                      //アニメーション番号を設定
	void InitTextureOnly();                                                    //テクスチャだけ初期化
	void SetTextureIndex(int nIdx) { m_nTextureIndex = nIdx; }                   //テクスチャ番号をセットする
	int GetTextureIndex() { return m_nTextureIndex; }                            //テクスチャ番号を取得する
    //==============================================================================================================================================================

	//==============================
	//体力関係
	//==============================
	void SetUseLife(bool bUse, int nLife, int nMaxLife)
	{
		m_bUseLife = bUse;
		m_nLife = nLife;
		m_nMaxLife = nMaxLife;
	}//体力を使用するかどうか

	int& GetLife() { return m_nLife; }//体力を取得
	int& GetMaxLife() { return m_nMaxLife; }//最大体力を取得
	float& GetRatioLife() { return m_fRatioLife; }//体力の割合
	//==============================================================================================================================================================

	//==============================
	//点滅関係
	//==============================
	void SetUseBlinking(bool bUse, int nMaxBlinkingFrame,float fLimitBlinkingRatio) {
		m_bUseBlinking = bUse;                      //点滅を使用するかどうか
		m_nMaxBlinkingFrame = nMaxBlinkingFrame;    //点滅最大フレーム
		m_fLimitBlinkingRatio = fLimitBlinkingRatio;//点滅させる濃さのリミット
	}
	//==============================================================================================================================================================

	//==============================
	//体力割合関係
	//==============================
	void SetUseLifeRatioColor(bool bUse) { m_bUseLifeRatioColor = bUse; }
	//==============================================================================================================================================================

	//================================
	//描画系
	//================================
	//描画するかどうか設定、取得
	void SetUseDraw(bool bUse) { m_bUseDraw = bUse; }
	const bool& GetUseDraw() const { return m_bUseDraw; }
	//==============================================================================================================================================================
protected:
	LPDIRECT3DVERTEXBUFFER9& GetVertexBuffer() { return m_pVtxBuff; }
private:
	int m_nAnimaionPattern;              //アニメーションパターン!
	int m_nAnimationCnt;                 //アニメーションカウント!
	int m_nMaxAnimationPattern;          //アニメーションパターンの最大数!
	int m_nAnimationChange;              //アニメーションを変えるカウント数!
	bool m_bAnimFlag;                    //アニメーションをするかどうか!
	float m_fLength;                     //４頂点との距離!
	float m_fAngle;                       //!
	float m_fAnimationSplit;             //アニメーション１分割当たりの値!

	int m_nTextureIndex;                 //テクスチャ番号（全てのオブジェクト2D派生クラスが持っている）!
	D3DXVECTOR3 m_pos;                   //位置!
	D3DXVECTOR3 m_Move;                  //移動量!
	D3DXVECTOR3 m_SupportPos;            //召喚位置!
	D3DXCOLOR m_col;                     //色合い!
	float m_fWidth;                      //横幅!
	float m_fHeight;                     //高さ!
	float m_fMaxWidth;                   //最大横幅!
	float m_fMaxHeight;                  //最大高さ!
	bool m_bUse;                         //使用状態!
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;                //頂点バッファ!
	LPDIRECT3DTEXTURE9 m_pTexture;                     //テクスチャ!
	POLYGONTYPE m_PolygonType;                         //ポリゴンの種類!

	//================================
	//サイズ系
	//================================
	D3DXVECTOR2 m_Scale;     //拡大率!
	D3DXVECTOR2 m_AddScale;  //加算する拡大率
	bool m_bUseScale;        //拡大率を使用するかどうか!
	bool m_bUseAddScale;     //拡大率の加算をするかどうか
	void ScaleProcess();     //拡大率の処理
	//==============================================================================================================================================================

	//================================
	//浮遊系
	//================================
	float m_fFloatingRot;   //浮く挙動をさせる向き!
	float m_fFloatingSpeed; //浮く挙動の速度!
	float m_fFloatingAddSpeed;//浮遊の挙動をさせるときの加算速度!
	float m_fFloatingLimitSpeed;//浮遊速度のリミット!
	bool m_bUseFloating;    //浮く挙動を使用するかどうか!
	void FloatingProcess(); //浮く挙動の処理
	//==============================================================================================================================================================

	//================================
	//点滅処理
	//================================
	int m_nCntBlinkingFrame;   //点滅用のカウント!
	int m_nMaxBlinkingFrame;   //点滅用の最大カウント数!
	bool m_bBlinkingAim;       //カウントを増やすか減らすかを決める!
	bool m_bUseBlinking;       //点滅させるかどうか!
	float m_fLimitBlinkingRatio; //点滅させる色の濃さのリミット!
	void BlinkingProcess();//点滅させる処理
	//==============================================================================================================================================================

	//================================
	//体力系
	//================================
	int m_nLife;//体力!
	int m_nMaxLife;//最大体力!
	bool m_bUseLife;//体力を使用するかどうか!
	float m_fRatioLife;//体力の割合!
	void SubLifeProcess();//体力をへらす処理

	bool m_bUseLifeRatioColor;//体力割合によって色合いを変えるかどうか!
	void LifeRatioColorProcess();//体力割合によって色合いを変える処理
	//==============================================================================================================================================================

	//================================
	//回転系
	//================================
	D3DXVECTOR3 m_rot;                   //向き!
	float m_fPolygonRotSpeed;                   //回転速度!
	//==============================================================================================================================================================

	//================================
	//描画系
	//================================
	bool m_bUseDraw;//描画するかどうか
	//==============================================================================================================================================================

};

#endif
