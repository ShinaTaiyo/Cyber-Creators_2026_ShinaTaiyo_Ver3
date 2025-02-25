//=====================================================================================
//
//１２月２４日：チュートリアルを作る[tutorial.h]
//Author:ShinaTaiyo
//
//=====================================================================================

#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_


//=============================================
//インクルード
//=============================================
#include "main.h"
#include "object.h"
#include "ui.h"
//=====================================================================================

//=============================================
//前方宣言
//=============================================

//=====================================================================================


//=============================================
//チュートリアルクラス
//=============================================
class CTutorial : public CObject
{
public:
	enum class CHECK
	{//チュートリアルチェック用
		MOVE = 0,       //移動
		SHOT,           //射撃
		CAMERACONTROLL, //カメラを操作した
		DASH,           //回避した
		JUMP,           //ジャンプした
		MODECHENGE,     //アクションモードを変更
		TURNAROUND,     //後ろを向く
		TAKEDIVE,       //ダイブ攻撃をする
		STUCKWALL,      //壁に引っ付く
		MAX
	};

	CTutorial(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::TUTORIAL, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_NONE);//コンストラクタ
	~CTutorial();              //デストラクタ
	HRESULT Init() override;   //初期化処理
	void Uninit() override;    //終了処理
	void Update() override;    //更新処理
	void Draw() override;      //描画処理
	void SetDeath() override;  //死亡フラグ設定処理

	static CTutorial* Create();//生成処理

	//チュートリアルをクリアしたかどうか
	const bool& GetSuccess() const { return m_bSuccess; }
	void SetSuccess(bool bSuccess) { m_bSuccess = bSuccess; }

	void SetDrawReset();//チュートリアルクリア状況の表示をリセット

	void SetSuccessCheck(CHECK Type)
	{//クリアしたチュートリアルにチェックをつける
		if (m_bSuccess == false)
		{
			m_pCheckPoint[static_cast<int>(Type)]->SetUseDraw(true);
		}
	}

private:
	CUi* m_pTutorialTex;                             //チュートリアルテクスチャへのポインタ

	CUi* m_pCheckPoint[static_cast<int>(CHECK::MAX)];//完了したかどうか

	bool m_bSuccess;                                 //成功したかどうか

};

#endif // !_TUTORIAL_H_