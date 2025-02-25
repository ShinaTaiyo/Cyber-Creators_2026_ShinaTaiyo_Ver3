//====================================================
//
//１０月２８日：狙った方向に弾を発射したい[lockon.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _LOCKON_H_  
#define _LOCKON_H_

//======================
//インクルード
//======================
#include "main.h"
#include "object2d.h"
#include "lockon.h"
//==========================================

//==========================================
//ロックオンクラス
//==========================================
class CLockon : public CObject2D
{
public:
	enum class ENDSTATE
	{
		NONE = 0,
		RIGHTEND,
		LEFTEND,
		UPEND,
		DOWNEND,
		MAX
	};

	enum class TYPE
	{//種類
		SHOT = 0,
		DIVE,
		STUCKSHOT,
		MAX
	};

	CLockon(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::LOCKON, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_2D);//描画順設定用コンストラクタ
	~CLockon() override;                                                                                                          //デストラクタ
	HRESULT Init() override;                                                                                                      //初期化処理
	void Uninit() override;                                                                                                       //終了処理
	void Update() override;                                                                                                       //更新処理
	void Draw() override;                                                                                                         //描画処理
	void SetDeath() override;                                                                                                     //死亡フラグ設定処理
	static CLockon* Create(TYPE Type,D3DXVECTOR3 Pos,CObject2D::POLYGONTYPE PolygonType,float fWidth,float fHeight,D3DXCOLOR col);//生成処理
	void ChengeTexture(TYPE Type);                                                                                                //テクスチャを変える
	const D3DXVECTOR3& GetLockOnPos() const { return m_LockOnPos; }                                                               //狙っている位置を取得する
	const D3DXVECTOR3& GetNowRay() const { return m_NowRay; }                                                                     //現在のレイを取得する
	const D3DXVECTOR3& GetFrontPos() const { return m_FrontPos; }                                                                 //手前の位置を取得する
	const D3DXVECTOR3& GetNearRayColObjPos() const { return m_NearRayColObjPos; }                                                 //レイが当たった一番近いオブジェクトの衝突位置を求める
	const ENDSTATE& GetEndState() const { return m_EndState; }                                                                   //ロックオンが画面内のどの端に当たっているかを取得する
	const bool& GetSuccessRayCollision() const { return m_bRayCollision; }                                                       //レイの当たり判定が成功したかどうか
private:
	//=======================================
	//静的メンバ
	//=======================================
	static constexpr float m_fNORMAL_LOCKONMOVE = 20.0f;               //ロックオンの移動量
	static constexpr float s_fLOCKONSTAGING_ADDSCALE = 0.5f;           //ロックオン演出の加算拡大率
	static constexpr int s_nLOCKONSTAGING_LIFE = 20;                   //ロックオン演出の体力
	static const string s_LOCKON_FILENAME[static_cast<int>(TYPE::MAX)];//テクスチャ名
	//==========================================================================================================

	//=======================================
	//変数
	//=======================================
	D3DXVECTOR3 m_LockOnPos;       //狙っている位置
	D3DXVECTOR3 m_NowRay;          //現在のレイ
	D3DXVECTOR3 m_FrontPos;        //手前の位置
	ENDSTATE m_EndState;           //どの端にいるか
	TYPE m_Type;                   //種類
	D3DXVECTOR3 m_NearRayColObjPos;//一番近いオブジェクトのレイが当たっている位置を求める
	bool m_bRayCollision;          //レイが当たっているかどうか
	//==========================================================================================================

	//=======================================
	//プロトタイプ宣言
	//=======================================
	void MoveProcess();//移動処理
	void BackWallRayCollisionPosSearch();//レイが当たっている奥の壁の位置をサーチ
	void CalcRay();//レイを計算する
	void RayCollisionToObject();//どのオブジェクトにレイが当たっているかを求める	
	//==========================================================================================================


};

#endif
