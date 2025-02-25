//====================================================
//
//７月２１日：敵のAIを作る[particle.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _PARTICLE_H_  
#define _PARTICLE_H_

//======================
//インクルード
//======================
#include "main.h"
#include "billboard.h"
//==========================================

//==========================================

//==========================================
//パーティクルクラス
//==========================================
class CParticle : public CBillboard
{
public:
	typedef enum
	{
		TYPE00_NORMAL = 0,//普通のパーティクルのテクスチャ
		TYPE01_FIRE,      //炎のパーティクルのテクスチャ
		TYPE02_ICE,       //氷のパーティクルのテクスチャ
		TYPE03_THUNDER,   //氷のパーティクルのテクスチャ
		TYPE04_HEAL,      //回復のパーティクルのテクスチャ
		TYPE05_WING,      //羽のパーティクルのテクスチャ
		TYPE_MAX
	}TYPE;

	CParticle(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::PARTICLE, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_BILLBOARD);//コンストラクタ
	~CParticle() override;                  //デストラクタ
	HRESULT Init() override;                //初期化処理
	void Uninit() override;                 //終了処理
	void Update() override;                 //更新処理
	void Draw() override;                   //描画処理
	void SetDeath() override;               //死亡フラグを設定

	//詳細設定
	void SetAddSpeed(float fAddSpeed);      //加速パーティクルにする
	void SetGravity(float fGravity);        //重力パーティクルにする　
	static CParticle * Create(TYPE Type,int nLife, float fWidth, float fHeight,D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, bool bBrending);//パーティクルを生成
	static void SummonParticle(TYPE type,int nNum,int nLife, float fWidth, float fHeight,int nRandSpeed,int nDivision,bool bRandumCol,D3DXVECTOR3 Pos,D3DXCOLOR col, bool bBreanding);//ランダム方向に放出するパーティクルを出す
	static void SummonChargeParticle(TYPE type, int nNum, int nLife, float fInitialSpeed,float fWidth, float fHeight,float fSummonLength, int nRandSpeed, int nDivision, bool bRandumCol,D3DXVECTOR3 SupportPos, D3DXCOLOR col, bool bBreanding);//チャージするような挙動のパーティクルを出す
private:
	bool m_bBranding;                                  //アルファブレンディングをするかどうか
	TYPE m_Type;                                       //種類
	float m_fReductionWidth;                           //横幅縮小スピード
	float m_fReductionHeight;                          //高さ縮小スピード
							                           
	bool m_bAddSpeed;                                  //加速させるかどうか
	float m_fAddSpeed;                                 //加速度
							                           
	bool m_bUseChargePartilce;                         //チャージする挙動をするかどうか
	bool m_bGravity;                                   //重力をかけるかどうか
	float m_fGravity;                                  //重力

	D3DXVECTOR3 m_SupportPos;                          //支点となる位置

	static const char* m_apPARTICLE_FILENAME[TYPE_MAX];//パーティクルのファイル名
};
//====================================================================================================================================
#endif
