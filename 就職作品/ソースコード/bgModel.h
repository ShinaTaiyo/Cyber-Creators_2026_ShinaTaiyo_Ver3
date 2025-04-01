//====================================================
//
//１０月２４日：攻撃クラスを実装する[attack.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _BGMODEL_H_  
#define _BGMODEL_H_

//======================
//インクルード
//======================
#include "main.h"
#include "objectX.h"
//==========================================

//===========================================
//背景モデルクラス
//===========================================
class CBgModel : public CObjectX
{
public:
	enum class BGMODELTYPE
	{//背景モデルの種類
		BILL_00 = 0,//ビル０
		TREE_00,    //木	
		BILL_01,    //ビル１
		BILL_02,    //ビル２
		BILL_03,    //ビル３
		BILL_04,    //ビル４
		CHAIN_00,   //チェーン
		HOUSE_00,   //家０
		GRASS_00,   //草
		GRASSGROUND_00,//草地面
		LOG_00,     //丸太
		MAX
	};

	CBgModel(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::BGMODEL, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_X);//コンストラクタ
	~CBgModel();                                                                                                             //デストラクタ
	HRESULT Init() override;                                                                                                 //初期化処理
	void Uninit() override;                                                                                                  //終了処理
	void Update() override;                                                                                                  //更新処理
	void Draw() override;                                                                                                    //描画処理
	void SetDeath() override;                                                                                                //死亡フラグを設定
	static CBgModel* Create(BGMODELTYPE bgModelType,D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale,bool bSwapVtxXZ);    //生成処理

	//==========================================================
	//エディタ関係
	//==========================================================
	//関数
	void SaveInfoTxt(fstream& WritingFile) override;                                                               //テキストファイルに情報を保存するための関数
	void LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff, CObject* pObj) override; //テキストファイルから情報を読み込むための関数
	CObject* ManagerChengeObject(bool bAim) override;                                                              //ステージマネージャーに変更したオブジェクトを渡す
	CObject* ManagerSaveObject() override;                                                                         //ステージマネージャーに今のオブジェクトを保存する
	//=================================================================================================================
protected:
	void SetBgModelType(BGMODELTYPE Type);                                                         //背景モデルの種類を設定する
	const BGMODELTYPE& GetBgModelType() const { return m_Type; }                                   //背景モデルの種類を取得する
	static const string BGMODEL_FILENAME[static_cast<int>(BGMODELTYPE::MAX)];                      //背景モデルのファイル名 
private:
	BGMODELTYPE m_Type;                                                                            //背景モデルの種類
};
//==================================================================================================================================================
#endif