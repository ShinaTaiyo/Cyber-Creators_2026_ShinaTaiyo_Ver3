//====================================================
//
//９月８日：ダメージを実装 [damage.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _DAMAGE_H_  
#define _DAMAGE_H_

//======================
//インクルード
//======================
#include "main.h"
#include "object.h"
//==========================================

//======================
//前方宣言
//======================
class CNumber3D;

//==========================================

//==========================================
//ダメージクラス
//==========================================
class CDamage: public CObject
{
public:
	CDamage();                   //コンストラクタ
	~CDamage() override;         //デストラクタ
	HRESULT Init() override;     //初期化処理
	void Uninit() override;      //終了処理
	void Update() override;      //更新処理
	void Draw() override;        //描画処理
	void SetDeath() override;    //死亡フラグを設定
	static CDamage* Create(int nDamage,D3DXVECTOR3 Pos,D3DXCOLOR col,float fWidth,float fHeight);//生成処理
private:
	static const int m_nMAX_DAMAGEDIGIT = 3;//ダメージの最大桁数
	static const int m_nMAX_JUMPCNT = 4;    //ダメージ表記をジャンプさせる最大数

	void DamageDispProcess();                    //ダメージを表示する処理
	CNumber3D* m_pNumber3D[m_nMAX_DAMAGEDIGIT];  //数字へのポインタ
	D3DXVECTOR3 m_Pos;                           //位置
	D3DXVECTOR3 m_JumpMove;                      //ジャンプさせる時の移動量

	int m_nJumpCnt;                              //ダメージ表記をジャンプさせるカウント
	int m_nDamage;                               //ダメージ
	int m_nDigit;                                //桁数

	float m_fWidth;                              //数字の横幅
	float m_fHeight;                             //数字の高さ
};

#endif
