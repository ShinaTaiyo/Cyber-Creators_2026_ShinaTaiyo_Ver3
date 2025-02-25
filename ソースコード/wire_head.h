//===============================================================================
//
//１１月２１日：ワイヤーを作る[wire_head.h]
//Author:ShinaTaiyo
//
//===============================================================================

#ifndef _WIREHEAD_H_
#define _WIREHEAD_H_

//==========================================
//インクルード
//==========================================
#include "main.h"
#include "objectX.h"
//===============================================================================

class CWireHead : public CObjectX
{
public:
	CWireHead(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::WIREHEAD, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_X);//コンストラクタ
	~CWireHead();                                                                                           //デストラクタ
	HRESULT Init() override;                                                                                //初期化処理
	void Uninit() override;                                                                                 //終了処理
	void Update() override;                                                                                 //更新処理
	void Draw() override;                                                                                   //描画処理
	void SetDeath() override;                                                                               //死亡フラグ設定処理
	static CWireHead* Create(D3DXVECTOR3 Pos,D3DXVECTOR3 Rot,D3DXVECTOR3 Move,D3DXVECTOR3 Scale,int nLife); //生成処理
	const bool& GetSuccessCollision() const { return m_bCollision; }                                        //当たり判定が成功したかどうか
	const CObject::TYPE& GetCollisionObjType() const { return m_CollisionObjType; }                         //衝突したオブジェクトの種類を取得
	void ResetCoolTime() { m_nCoolTime = s_nMAX_COOLTIME; m_bCollision = false; }                           //当たり判定を行うまでのクールタイムをリセットする
private:
	static constexpr int s_nMAX_COOLTIME = 1;                                                               //当たり判定を行うクールタイム最大値

	bool CollisionSquare();                                                                                 //正方形の当たり判定を行う
	bool m_bCollision;                                                                                      //当たり判定が成功したかどうか
	int m_nCoolTime;                                                                                        //クールタイムを設定
	CObject::TYPE m_CollisionObjType;                                                                       //衝突したオブジェクトのタイプを格納
};
#endif // !_WIRE_H_