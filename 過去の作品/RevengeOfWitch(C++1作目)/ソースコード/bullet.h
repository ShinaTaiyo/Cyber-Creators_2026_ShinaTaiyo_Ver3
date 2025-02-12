//====================================================
//
//５月２２日：C++で基盤づくり[bullet.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _BULLET_H_  
#define _BULLET_H_

//======================
//インクルード
//======================
#include "main.h"
#include "billboard.h"
#include "effect.h"

//==========================================

//======================
//前方宣言
//======================
class CEnemy;

//==========================================

//==========================================
//バレットクラス
//==========================================
class CBullet : public CBillboard
{
public:
	//===========================
	//弾の種類
	//===========================
	typedef enum
	{
		BULLETTYPE_PLAYER = 0, //プレイヤーの弾
		BULLETTYPE_ENEMY,      //敵の弾
		BULLETTYPE_MAX
	}BULLETTYPE;
	//======================================================================

	//===========================
	//弾のテクスチャの種類
	//===========================
	typedef enum
	{
		BULLETTEX_PLAYER = 0,//プレイヤーの通常弾
		BULLETTEX_ENEMY,     //敵の通常弾
		BULLETTEX_RAIN,      //雨のテクスチャ
		BULLETTEX_MAX
	}BULLETTEX;

	//===========================
	//動き方の種類
	//===========================
	typedef enum
	{
		MOVETYPE00_NORMAL = 0,//普通に動く
		MOVETYPE01_GRAVITY,   //重力がつく
		MOVETYPE_MAX
	}MOVETYPE;
	//======================================================================

	CBullet();              //コンストラクタ
	~CBullet() override;    //デストラクタ
	HRESULT Init() override;//初期化処理
	void Uninit() override; //終了処理
	void Update() override; //更新処理
	void Draw() override;   //描画処理
	void SetDeath() override;//死亡フラグを設定
	static CBullet * Create(BULLETTYPE type,BULLETTEX tex,int nLife,int nPower,float fWidth,float fHeight,D3DXVECTOR3 pos,D3DXVECTOR3 move);//バレットを生成
	void Collision();       //当たり判定を取る
	void SetHitAddMp(int nAddMp) { m_nHitAddMp = nAddMp; }//MP変動値を設定する
	
	void SetUseDivision(bool bUse, int nNum, int nLimit) { m_bUseDivision = bUse; m_nNumDivision = nNum; m_nLimitDivision = nLimit; }
	BULLETTYPE GetBulletType() { return m_type; }

	void SetUseReflection(bool bUse) { m_bUseReflection = bUse; }//反射を使用するかどうかを設定

	void SetBlockCollision(bool bCollision) { m_bBlockColliion = bCollision; }
	bool& GetBlockCollision() { return m_bBlockColliion; }
private:																							             
	BULLETTYPE m_type;      //弾の種類
	MOVETYPE m_MoveType;    //動き方の種類
	int m_nPower;           //威力

	bool m_bUse;            //使用状態
	float m_fGravity;       //重力
	float m_fLength;        //距離

	bool m_bUseDivision;    //分裂をさせるかどうか
	bool m_bBlockColliion;  //ブロックに当たるかどうか
	int m_nNumDivision;     //分裂数
	int m_nLimitDivision;   //分裂回数の上限を決める

	bool m_bUseReflection; //反射を使用するかどうか

	bool m_bUseBlockCollision;//ブロックとの当たり判定を使用するかどうか

	//===========================================================
	//MP関連
	//===========================================================
	int m_nHitAddMp;        //ヒットしたときにMPを変動させる値
	void HitAddMpProcess(); //MPを割り当てる処理
	//============================================================================================
	static const char* m_apBULLET_FILENAME[BULLETTEX_MAX];

	void DivisionBulletProcess(int nRadius);//分裂弾の処理

	void BlockCollisionProcess();//ブロックとの当たり判定を行う処理

};



#endif
