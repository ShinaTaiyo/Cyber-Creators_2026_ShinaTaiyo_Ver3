//===============================================================================
//
//２月２５日：スコアを実装する[score.h]
//Author:ShinaTaiyo
//
//===============================================================================

//========================================
//２重インクルード防止
//========================================
#ifndef _SCORE_H_  
#define _SCORE_H_
//===============================================================================


//========================================
//インクルード
//========================================
#include "main.h"
#include "object.h"
#include "number.h"
//===============================================================================

//========================================
//スコアクラス
//========================================
class CScore : public CObject

{
public:
	CScore(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::SCORE, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_NONE);//コンストラクタ
	~CScore() override;                                            //デストラクタ
	HRESULT Init() override;                                       //初期化処理
	void Uninit() override;                                        //終了処理
	void Update() override;                                        //更新処理
	void Draw() override;                                          //描画処理
	void SetDeath() override;                                      //死亡フラグ設定処理
	void SetScore(int nScore);                                     //スコアを設定する
	void AddScore(int nScore);                                     //スコアを加算する
	void SubScore(int nScore);                                     //スコアを減算する
	static CScore* Create(D3DXVECTOR3 Pos);                        //生成処理
	static constexpr float GetNumberWidth() { return s_fWIDTH; }   //数字の横幅を取得
	static constexpr float GetNumberHeight() { return s_fHEIGHT; } //数字の高さを取得
private:
	//=================
	//静的メンバ
	//=================
	static constexpr int s_nNUMDIGIT = 8;         //桁数
	static constexpr float s_fWIDTH = 40.0f;      //数字の横幅
	static constexpr float s_fHEIGHT = 40.0f;     //数字の高さ
	//===========================================================

	//=================
	//関数
	//=================
	void SetUpdateNumberDisp();                   //数字の表示を更新する
	//===========================================================
	
	//=================
	//メンバ変数
	//=================
	int m_nScore;                                 //スコア
	CNumber* m_pNumber[s_nNUMDIGIT];              //数字へのポインタ
	D3DXVECTOR3 m_Pos;                            //位置
	//===========================================================
};
//===============================================================================

#endif
