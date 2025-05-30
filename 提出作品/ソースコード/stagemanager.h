//==========================================================================================
//
//７月１７日：配置ツールを作る[stagemanager.h]
//Author:ShinaTaiyo
//
//==========================================================================================

#ifndef _STAGEMANAGER_H_  
#define _STAGEMANAGER_H_

//============================
//インクルード
//============================
#include "main.h"
#include "object.h"
//========================================================

//============================
//前方宣言
//============================
class CObjectXInfo;
class CBg3D;
//======================
//マクロ定義
//======================
#define SAVE_BIN "data\\BINARYFILE\\Save.bin"
#define SAVE_TXT "data\\TEXTFILE\\stagemanager\\Save.txt"

//======================
//前方宣言
//======================
class CStageManagerState;

//============================
//ステージ管理クラス
//============================
class CStageManager : public CObject
{
public:
	enum class FOCUSTYPE
	{
		FOCUSTYPE_NORMAL = 0,//カメラが追う位置が普通
		FOCUSTYPE_WARPPOS,   //カメラがワープ先の位置を追う
	};

	enum class WORLDTYPE
	{//ワールド列挙型
		STAGE01 = 0,
		STAGE02,
		STAGE03,
		MAX
	};

	CStageManager(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::STAGEMANAGER, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_NONE);//コンストラクタ
	~CStageManager();                                                //デストラクタ
	HRESULT Init() override;                                         //初期化処理
	void Uninit() override;                                          //終了処理
	void Update() override;                                          //更新処理
	void Draw() override;                                            //描画処理
	void SetDeath() override;                                        //死亡フラグを設定
	void LoadMapTxt(int nMapNum);                                    //マップをテキストファイルからロードする
	void SaveMapTxt(int nMapNum);                                    //マップをテキストファイルにセーブする
	int GetMapIndex() { return m_nMapIndex; }                        //現在のマップ番号を取得する
	void MapChenge();                                                //マップを変更する

	void SetSpawnPoint(D3DXVECTOR3 Pos) { m_SpawnPoint = Pos; }      //スポーン位置を設定する
	const D3DXVECTOR3& GetSpawnPoint() const { return m_SpawnPoint; }//スポーン位置を取得する

	list <CObject*> & GetStgObjList() { return m_StgObjList; }       //管理オブジェクトリストを取得する
	const D3DXVECTOR3& GetSavePos() const { return m_SavePos; }      //破棄するオブジェクトの位置を取得
	void SetSavePos(D3DXVECTOR3 Pos) { m_SavePos = Pos; }            //破棄するオブジェクトの位置を設定
	const D3DXVECTOR3& GetSaveRot() const { return m_SaveRot; }      //破棄するオブジェクトの向きを取得
	void SetSaveRot(D3DXVECTOR3 Rot) { m_SaveRot = Rot; }            //破棄するオブジェクトの向きを設定
	const D3DXVECTOR3& GetSaveScale() const { return m_SaveScale; }  //破棄するオブジェクトの拡大率を取得
	void SetSaveScale(D3DXVECTOR3 Scale) { m_SaveScale = Scale; }    //破棄するオブジェクトの拡大率を設定
	static CStageManager* Create();                                  //生成処理
private:

	//*列挙型
	enum class MANAGERMODE
	{
		ALREADYSTAGE = 0,//既にあるステージを編集するモード
		NEWSTAGE,        //新しくステージを作るモード
		MAX
	};

	enum class STATE
	{//ステート名
		NEWOBJECT = 0,//新しいオブジェクト
		PLACEDOBJECT, //既存のオブジェクト
		MAX
	};

	//*静的メンバ変数
	static const int m_nMAX_MAP = 20;                                       //マップ最大数
	static const int m_nMAX_WORD = 126;                                     //文字列最大数宇
	static const string m_aWORLDMAP_TXT[static_cast<int>(WORLDTYPE::MAX)];  //ワールド名
	static const string m_aSAVE_FILENAME;//保存するファイル名  

	//*変数

	//====================
	//基本系
	//====================
	D3DXVECTOR3 m_SaveRot;                      //向き
	D3DXVECTOR3 m_SavePos;                      //位置
	D3DXVECTOR3 m_SaveScale;                    //拡大率
	D3DXVECTOR3 m_SaveBeforeChoosePos;          //選択処理をする前のする位置
	MANAGERMODE m_ManagerMode;                  //現在のステージマネーシャーのモード
	D3DXVECTOR3 m_SpawnPoint;                   //スポーンポイント
	//========================================================================================

	//====================
	//番号
	//====================
	int m_nMapIndex;                            //マップのインデックス
	int m_nWorldIndex;                          //ワールドのインデックス
	int m_nMapNum;                              //マップの総数
	//========================================================================================
	
	//====================
	//選択系
	//====================
	CObject* m_pChooseObject;                   //選択オブジェクト
	bool m_bChooseObject;                       //オブジェクトを選択しているかどうか
	//========================================================================================

	//===================
	//モード関係
	//===================
	bool m_bMakeMapMode;                        //マップを作るモード
	bool m_bUseSizeMove;//現在のオブジェクトのサイズ分移動するかどうか
	//========================================================================================


	//===================
	//オブジェクトリスト
	//===================
	list <CObject*> m_StgObjList;//オブジェクトのリスト
	//========================================================================================

	//===================
	//3D背景
	//===================
	CBg3D* m_pBg3D;               //背景へのポインタ
	//========================================================================================

	//===================
	//ステート
	//===================
	CStageManagerState* m_pState;//ステート格納用
	STATE m_NowState;//現在の状態
	//========================================================================================

	//*関数
	
	void ResetScale();  //拡大率をリセットする処理
	void DispInfo();    //現在のマップエディタの情報を表示

	void ChengeState(); //ステートを変える
};

//ステージマネージャー状態スーパークラス
class CStageManagerState
{
public:
	CStageManagerState() {};                              //コンストラクタ
	virtual ~CStageManagerState() {};                     //デストラクタ
	virtual void Process(CStageManager* pStageManager) {};//処理
};

//新しいオブジェクトの配置を行う状態
class CStageManagerState_NewObject : public CStageManagerState
{
public:
	CStageManagerState_NewObject();                     //コンストラクタ
	~CStageManagerState_NewObject() override;           //デストラクタ
	void Process(CStageManager* pStageManager) override;//処理
private:
	CObject* m_pManagerObject;                                              //設置予定オブジェクトへのポインタ

	void TypeChenge(CStageManager* pStageManager);                          //オブジェクトXの種類を変える
	void SetObjectX(CStageManager* pStageManager);                          //オブジェクトXを設定する
	void DeleteManagerObject(CStageManager* pStageManager);                 //オブジェクトXを消す
	void ChengeObject(CStageManager * pStageManager);                       //オブジェクトの種類を消す
	void ReleaseObject(CStageManager * pStageManager);                      //オブジェクトをリリースする

};

//既存のオブジェクトの編集を行う状態
class CStageManagerState_PlacedObject : public CStageManagerState
{
public:
	CStageManagerState_PlacedObject();                                           //コンストラクタ
	~CStageManagerState_PlacedObject() override;                                 //デストラクタ
	void Process(CStageManager* pStageManager) override;                         //処理
private:
	void ChooseObject(CStageManager * pStageManager);                            //オブジェクトを選ぶ処理

	void ChengeIdx(CStageManager* pStageManager);                                //オブジェクトリストを指定アドレスまで進めるための番号を変える  
	void DeleteObject(list<CObject*>::iterator & it,list<CObject*> & StgObjList);//オブジェクトを破棄する
	int m_nChooseObjIdx;                                                         //現在選んでいるオブジェクトの番号
};

#endif