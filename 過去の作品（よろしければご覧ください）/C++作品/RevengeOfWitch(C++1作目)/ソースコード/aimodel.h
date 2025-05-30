//====================================================
//
//９月７日：AI用のモデルを作る[aimodel.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _AIMODEL_H_  
#define _AIMODEL_H_

//======================
//インクルード
//======================
#include "main.h"
#include "objectX.h"
//==========================================

//===========================================
//AIモデルクラス
//===========================================
class CAIModel : public CObjectX
{
public:
	typedef enum
	{
		AIMODELTYPE_SQUARE = 0,//四角形
		AIMODELTYPE_BALL,      //円
		AIMODELTYPE_MAX
	}AIMODELTYPE;

	CAIModel();                 //コンストラクタ
	~CAIModel();                //デストラクタ
	HRESULT Init() override;    //初期化処理
	void Uninit() override;     //終了処理
	void Update() override;     //更新処理
	void Draw() override;       //描画処理
	void ExtraDraw();           //描画処理
	void SetDeath() override;   //死亡フラグを設定
	static CAIModel* Create(AIMODELTYPE type,D3DXVECTOR3 pos, D3DXVECTOR3 rot,D3DXVECTOR3 Scale);
private:
	AIMODELTYPE m_Type;
	static const char* m_apAIMODEL_FILENAME[AIMODELTYPE_MAX];
};
//==============================================================================================================================================================

#endif
