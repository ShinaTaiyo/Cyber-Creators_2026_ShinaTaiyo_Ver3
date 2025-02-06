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
	{
		STAGE01 = 0,
		STAGE02,
		STAGE03,
		MAX
	};

	CStageManager(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::STAGEMANAGER, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_NONE);  //コンストラクタ
	~CStageManager(); //デストラクタ
	HRESULT Init() override;   //初期化処理
	void Uninit() override;     //終了処理
	void Update() override;    //更新処理
	void Draw() override;      //描画処理
	void SetDeath() override;  //死亡フラグを設定
	void LoadMapTxt(int nMapNum);               //マップをテキストファイルからロードする
	void LoadMapFilePass(WORLDTYPE type);       //マップのファイルパスをロードする
	void SaveMapTxt(int nMapNum);               //マップをテキストファイルにセーブする
	int GetMapIndex() { return m_nMapIndex; }   //現在のマップ番号を取得する
	void MapChenge();                            //マップを変更する

	void SetSpawnPoint(D3DXVECTOR3 Pos) { m_SpawnPoint = Pos; }//スポーン位置を設定する
	const D3DXVECTOR3& GetSpawnPoint() const { return m_SpawnPoint; }//スポーン位置を取得する

	list <CObject*> & GetStgObjList() { return m_StgObjList; }//管理オブジェクトリストを取得する
	const D3DXVECTOR3& GetSavePos() const { return m_SavePos; }
	void SetSavePos(D3DXVECTOR3 Pos) { m_SavePos = Pos; }
	const D3DXVECTOR3& GetSaveRot() const { return m_SaveRot; }
	void SetSaveRot(D3DXVECTOR3 Rot) { m_SaveRot = Rot; }
	const D3DXVECTOR3& GetSaveScale() const { return m_SaveScale; }
	void SetSaveScale(D3DXVECTOR3 Scale) { m_SaveScale = Scale; }
	static CStageManager* Create();             //生成処理
private:

	//*列挙型
	enum class MANAGERMODE
	{
		ALREADYSTAGE = 0,//既にあるステージを編集するモード
		NEWSTAGE,        //新しくステージを作るモード
		MAX
	};

	enum class STATE
	{
		NEWOBJECT = 0,
		PLACEDOBJECT,
		MAX
	};

	//*静的メンバ変数

	static const int m_nMAX_MAP = 20;
	static const int m_nMAX_WORD = 126;
	static const string m_aWORLDMAP_TXT[static_cast<int>(WORLDTYPE::MAX)];
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
	CBg3D* m_pBg3D;//背景へのポインタ
	//========================================================================================

	//===================
	//ステート
	//===================
	CStageManagerState* m_pState;
	STATE m_NowState;
	//========================================================================================

	//*関数
	
	void ResetScale();  //拡大率をリセットする処理
	void DispInfo();    //現在のマップエディタの情報を表示

	void ChengeState();
};

//ステージマネージャー状態スーパークラス
class CStageManagerState
{
public:
	CStageManagerState() {}; //コンストラクタ
	virtual ~CStageManagerState() {};//デストラクタ
	virtual void Process(CStageManager* pStageManager) {};//処理
};

//新しいオブジェクトの配置を行う状態
class CStageManagerState_NewObject : public CStageManagerState
{
public:
	CStageManagerState_NewObject();
	~CStageManagerState_NewObject() override;
	void Process(CStageManager* pStageManager) override;//処理
private:
	CObject* m_pManagerObject;//設置予定オブジェクトへのポインタ

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
	CStageManagerState_PlacedObject();
	~CStageManagerState_PlacedObject() override;
	void Process(CStageManager* pStageManager) override;
private:
	void ChooseObject(CStageManager * pStageManager);

	void ChengeIdx(CStageManager* pStageManager);
	void DeleteObject(list<CObject*>::iterator & it,list<CObject*> & StgObjList);
	int m_nChooseObjIdx;//現在選んでいるオブジェクトの番号
};

#endif