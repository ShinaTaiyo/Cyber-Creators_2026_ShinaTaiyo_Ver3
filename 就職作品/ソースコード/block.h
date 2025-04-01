//====================================================
//
//７月１０日：C++で基盤づくり[block.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _BLOCK_H_  
#define _BLOCK_H_

//======================
//インクルード
//======================
#include "main.h"
#include "objectX.h"
//==========================================

//===================================
//マップのテキストファイルのマクロ
//===================================
#define MAP00_SENTRAL_TXT "data\\TEXTFILE\\Map\\Sentral.txt"

//==========================================

//==========================================
//ブロッククラス
//==========================================
class CBlock : public CObjectX
{
public:
	//===========================
	//ブロックの種類
	//===========================
    enum class BLOCKTYPE
	{
		NORMAL = 0,    //普通ブロック
		WATER,         //水ブロック
		RENGA,         //レンガブロック
		BLACK,         //黒ブロック
		MAX
	};
	//======================================================================

	//===========================
	//マップの種類
	//===========================
	typedef enum
	{
		MAP00_SENTRAL = 0,
		MAP_MAX
	}MAPTYPE;
	//======================================================================

	CBlock(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::BLOCK, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_X);                 //コンストラクタ
	~CBlock() override;       //デストラクタ
	HRESULT Init() override;  //初期化処理
	void Uninit() override;   //終了処理
	void Update() override;   //更新処理
	void Draw() override;     //描画処理
	void SetDeath() override; //死亡フラグを設定
	static CBlock * Create(BLOCKTYPE type, int nLife,D3DXVECTOR3 pos,D3DXVECTOR3 rot,D3DXVECTOR3 scale,bool bSwapVtxXZ);//ブロックを生成
	void SetBlockType(BLOCKTYPE Type);                                                                                  //ブロックのタイプを設定する
	static int GetNumFile() { return m_nNumFile; }                                                                      //ファイル数を取得する
	BLOCKTYPE GetType();//ブロックの種類を取得する
	BLOCKTYPE GetBlockType() { return m_type; }                                                                         //ブロックのタイプを取得する
	bool GetbCollision() { return m_bCollision; }                                                                       //判定可能かどうかを取得する     

    //==========================================================
	//エディタ関係
	//==========================================================
	//関数
	void SaveInfoTxt(fstream& WritingFile) override;                                                                    //テキストファイルに情報を保存するための関数
	void LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff, CObject* pObj) override;        //テキストファイルから情報を読み込むための関数
	CObject* ManagerChengeObject(bool bAim) override;                                                                   //ステージマネージャーに変更したオブジェクトを渡す
	CObject* ManagerSaveObject() override;                                                                              //ステージマネージャーに今のオブジェクトを保存する
	//=================================================================================================================
private:
	//======================================
	//静的メンバ
	//======================================
	static const char* m_BLOCK_FILENAME[static_cast<int>(BLOCKTYPE::MAX)];                                              //ブロックのファイルネーム
	static const float m_fBLOCKCORRECTIONCOLLISION;                                                                     //判定に余裕を持たせる量
	static int m_nNumFile;                                                                                              //ファイル数を格納する
	//======================================================================================================================

	//======================================
	//基本系
	//======================================
	BLOCKTYPE m_type;                                                              //ブロックの種類
	bool m_bCollision;                                                             //当たり判定をするかどうか
	//======================================================================================================================
};

#endif
