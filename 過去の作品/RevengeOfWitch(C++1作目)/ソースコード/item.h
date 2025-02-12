//====================================================
//
//７５日：回避アイテムを実装[item.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _ITEM_H_  
#define _ITEM_H_

//======================
//インクルード
//======================
#include "main.h"
#include "objectX.h"
//==========================================

//==========================================

//==========================================
//アイテムクラス
//==========================================
class CItem : public CObjectX
{
public:
	//===========================
	//アイテムの種類
	//===========================
	typedef enum
	{
		ITEMTYPE00_HEALHEART = 0,
		ITEMTYPE01_COIN,
		ITEMTYPE02_MAGICPORTION,
		ITEMTYPE03_CROWN,
		ITEMTYPE04_DOUBLEJUMPSHOES,
		ITEMTYPE05_DODGECRYSTAL,
		ITEMTYPE06_WALLJUMPCROW,
		ITEMTYPE07_FIREBALL_ENERGY,
		ITEMTYPE08_BEAMWHIP_ENERGY,
		ITEMTYPE09_ICE_ENERGY,
		ITEMTYPE10_THUNDER_ENERGY,
		ITEMTYPE11_MAGICSWORD_ENERGY,
		ITEMTYPE12_PLANTSHIELD_ENERGY,
		ITEMTYPE13_QUICKHEAL_ENERGY,
		ITEMTYPE14_CROSSBOMB_ENERGY,
		ITEMTYPE15_BLACKHOLE_ENERGY,
		ITEMTYPE16_PHYCOBURST_ENERGY,
		ITEMTYPE17_CHARGESHOT_GUN,
		ITEMTYPE18_RAINSHOT_GUN,
		ITEMTYPE19_BURSTSHOT_GUN,
		ITEMTYPE20_RAPIDSHOT_GUN,
		ITEMTYPE_MAX
	}ITEMTYPE;
	//======================================================================
	CItem();                    //コンストラクタ
	~CItem() override;          //デストラクタ
	HRESULT Init() override;    //初期化処理
	void Uninit() override;     //終了処理
	void Update() override;     //更新処理
	void Draw() override;       //描画処理
	void SetDeath() override;   //死亡フラグを設定
	static CItem* Create(ITEMTYPE type,D3DXVECTOR3 Pos,D3DXVECTOR3 Rot,D3DXVECTOR3 Scale,int nRotType);//敵を生成
	static int GetNumFile() { return m_nNumFile; }           //ファイルの数を取得する
	ITEMTYPE GetItemType() { return m_type; }                //アイテムの種類を取得する
private:
	void Collision(void);                                    //当たり判定を行う
	static bool NotSummonProcess(ITEMTYPE type);
	ITEMTYPE m_type;                                         //敵の種類
	bool m_bUse;                                             //使用状態
	int m_nCntTime;                                          //出現してからのフレーム数を測る
	static const char* m_ITEMFILENAME[ITEMTYPE_MAX];         //アイテムのファイルの名前
	static int m_nNumFile;                                   //ファイルの数
};

#endif
