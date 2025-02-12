//===============================================================================
//
//９月２０日：難易度選択を実装する[difficulty.h]
//Author:ShinaTaiyo
//
//===============================================================================

#ifndef _DIFFICULTY_H_  
#define _DIFFICULTY_H_


//========================================
//インクルード
//========================================
#include "scene.h"
#include "ui.h"
//===============================================================================

//========================================
//難易度クラス
//========================================
class CDifficulty : public CScene
{
public:
	CDifficulty();                           //コンストラクタ
	~CDifficulty() override;                 //デストラクタ
	HRESULT Init() override;            //初期化処理
	void Uninit() override;             //終了処理
	void Update() override;             //更新処理
	void Draw() override;               //描画処理

	typedef enum
	{
		DIFFICULTYTYPE_EASY = 0,//難易度：簡単
		DIFFICULTYTYPE_NORMAL,  //難易度：普通
		DIFFICULTYTYPE_BOSSRUSH,//難易度：ボスラッシュ
		DIFFICULTYTYPE_MAX
	}DIFFICULTYTYPE;

	static const int& GetDifficultyNum() { return s_nChooseDifficultyNum; }//難易度番号を取得
private:
	CUi* m_pDifficultyUi[DIFFICULTYTYPE_MAX];
	static int s_nChooseDifficultyNum;//選んでいる難易度の番号

	void ChooseDifficulty();//難易度選択処理
	void TextStaging(bool bChoose);     //テキストUIの演出
};
#endif