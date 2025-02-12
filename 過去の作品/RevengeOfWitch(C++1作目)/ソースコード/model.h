//====================================================
//
//６月５日：モデルの当たり判定を行う[model.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _MODEL_H_  
#define _MODEL_H_

//======================
//インクルード
//======================
#include "main.h"
#include "object2d.h"
#include "objectX.h"
//==========================================

//===========================================
//モデルクラス
//===========================================
class CModel: public CObjectX
{
public:
	//=============================================
	//モデル種類列挙型
	//=============================================
	typedef enum
	{
		MODELTYPE_BILL = 0,        //ビル[0]
		MODELTYPE_TV,              //テレビ[1]
		MODELTYPE_SWITCH,          //ニンテンドースイッチ[2]
		MODELTYPE_RING,            //リング[3]
		MODELTYPE_MAX
	}MODELTYPE;
	//========================================================================================
	CModel();                  //コンストラクタ
	~CModel();                 //デストラクタ
	HRESULT Init() override;    //初期化処理
	void Uninit() override;     //終了処理
	void Update() override;     //更新処理
	void Draw() override;       //描画処理
	void SetDeath() override;   //死亡フラグを設定
	static CModel* Create(MODELTYPE type,D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXVECTOR3 Scale,CObjectX::ROTTYPE RotType);
	static int GetNumFile() { return m_nNumFile; }                                                                 //ファイル数を取得する
private:

	static const char* m_BLOCK_FILENAME[MODELTYPE_MAX];                                    //ブロックのファイルネーム
	MODELTYPE m_Type;                                                                      //モデル種類
	static int m_nNumFile;                                                                 //ファイル数を格納する
};
//==============================================================================================================================================================

#endif
