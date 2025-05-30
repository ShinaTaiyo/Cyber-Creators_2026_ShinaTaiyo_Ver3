//===============================================================================
//
//１１月１７日：ワイヤーを作る[wire.h]
//Author:ShinaTaiyo
//
//===============================================================================

#ifndef _WIRE_H_
#define _WIRE_H_

//==========================================
//インクルード
//==========================================
#include "main.h"
#include "meshcylinder.h"
#include "wire_head.h"
//===============================================================================

//==========================================
//前方宣言
//==========================================
class CPlayer;

class CWire : public CMeshCylinder
{
public:
	enum class WIRETYPE
	{//ワイヤーの種類
		NORMAL = 0,
		ROPE,
		MAX
	};

	CWire(WIRETYPE WireType,float fRadius,float fHeight,int nNumDivsionXZ,D3DXVECTOR3 Pos,D3DXVECTOR3 Rot,int nNumDivisionY,int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::WIRE, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_3D);//コンストラクタ
	~CWire() override;//デストラクタ
	HRESULT Init() override; //初期化処理
	void Uninit() override;  //終了処理
	void Update() override;  //更新処理
	void Draw() override;    //描画処理
	void SetDeath() override;//死亡フラグ設定処理
	static CWire* Create(WIRETYPE Type,D3DXVECTOR3 Pos, D3DXVECTOR3 Rot,float fRadius, float fHeight,int nNumDivisionXZ,int nNumDivisionY);

	//ワイヤーの頭取得
	CWireHead* GetWireHead() { return m_pWireHead; }

	//プレイヤーのポインタ設定
	void SetPlayerPointer(CPlayer* pPlayer) {m_pPlayer = pPlayer; }

	//更新するかどうか
	void SetUseUpdate(bool bUse) { m_bUseUpdate = bUse; }
protected:
	static const string s_WIRE_FILENAME[static_cast<int>(WIRETYPE::MAX)];
private:
	WIRETYPE m_Type;//ワイヤーの種類
	CPlayer* m_pPlayer;//プレイヤーのポインタのインスタンス
	CWireHead* m_pWireHead;//ワイヤーの頭
	bool m_bUseUpdate;//更新するかどうか
};
#endif // !_WIRE_H_