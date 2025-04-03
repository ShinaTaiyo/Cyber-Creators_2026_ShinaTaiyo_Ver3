//==========================================================
//
//７月１７日：オブジェクトのリスト管理を実装[stagemanager.cpp]
//Author:ShinaTaiyo
//
//==========================================================

//================================
//インクルード
//================================
#include "stagemanager.h"
#include "objectX.h"
#include "objectXInfo.h"
#include "block.h"
#include "input.h"
#include "manager.h"
#include "object.h"
#include "camera.h"
#include "calculation.h"
#include "bg3d.h"
#include "debugtext.h"
#include "bgModel.h"
#include "enemy.h"
#include <stdio.h>
#include <string.h>
//==========================================================

//================================
//静的メンバ宣言
//================================
const string CStageManager::m_aWORLDMAP_TXT[static_cast<int>(CStageManager::WORLDTYPE::MAX)] =
{
	"data\\TEXTFILE\\Ver2\\Stage01.txt",
	"data\\TEXTFILE\\Ver2\\Stage02.txt",
	"data\\TEXTFILE\\Ver2\\Stage03.txt",
};

const string CStageManager::m_aSAVE_FILENAME = "data\\TEXTFILE\\Ver2\\Stage03.txt";

//================================
//コンストラクタ
//================================
CStageManager::CStageManager(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CObject(nPri,bUseintPri,type,ObjType),
m_nWorldIndex(0),m_pBg3D(nullptr), m_StgObjList(),m_SaveScale(D3DXVECTOR3(1.0f,1.0f,1.0f)),m_SavePos(D3DXVECTOR3(0.0f,0.0f,0.0f)),m_SaveRot(D3DXVECTOR3(0.0f,0.0f,0.0f)),
m_SaveBeforeChoosePos(D3DXVECTOR3(0.0f,0.0f,0.0f)), m_nMapIndex(0), m_nMapNum(0), m_pChooseObject(nullptr),
m_ManagerMode(MANAGERMODE::ALREADYSTAGE),m_bChooseObject(false),m_bMakeMapMode(false),m_bUseSizeMove(false),
m_pState(nullptr),m_NowState(STATE::NEWOBJECT),m_SpawnPoint(D3DXVECTOR3(0.0f,0.0f,0.0f))
{

}
//==========================================================

//================================
//デストラクタ
//================================
CStageManager::~CStageManager()
{
	m_StgObjList.clear();//vectorの中身をクリア（忘れたとき対策）
}
//==========================================================

//================================
//初期化処理
//================================
HRESULT CStageManager::Init()
{
	//===========================
	//メンバ変数
	//===========================
	m_nMapIndex = 0;                                     //マップのインデックス
	m_nMapNum = 0;                                       //マップの総数

	//=========================
	//選択系
	//=========================
	m_pChooseObject = nullptr;                          //選択オブジェクトへの
	//=======================================================================================
#ifdef _DEBUG
#endif // _DEBUG

	//===========================
	//移動モードを決める
	//===========================
	m_bUseSizeMove = false;//現在のオブジェクトのサイズ分移動するかどうか
	//=======================================================================================

	m_pState = DBG_NEW CStageManagerState_NewObject();//新しいオブジェクト編集モードにする

	CObject::Init();                                  //オブジェクト初期化処理

	SetUseDeath(false);                               //死亡フラグを使用しない
	if (CScene::GetMode() == CScene::MODE_EDIT)
	{//エディットモードなら
		LoadMapTxt(0);                                //マップ０を読み込む
	}
	return S_OK;
}
//==========================================================

//================================
//終了処理
//================================
void CStageManager::Uninit()
{
	CObject::Uninit();//オブジェクト終了処理
}
//==========================================================

//================================
//更新処理
//================================
void CStageManager::Update()
{

#ifdef _DEBUG

	if (CScene::GetMode() == CScene::MODE_EDIT)
	{//エディットシーンなら

		MapChenge();  //マップを変える処理

		ChengeState();//ステート変更

		if (m_pState != nullptr)
		{//ステート処理
			m_pState->Process(this);
		}
		//===========================================================================================

		//===============================================
		//情報表示処理
		//===============================================
		DispInfo();
		//===========================================================================================

		//===============================================
		//オブジェクト更新
		//===============================================
		CObject::Update();
		//===========================================================================================
	}

#endif // _DEBUG
}
//==========================================================

//================================
//描画処理
//================================
void CStageManager::Draw()
{
#ifdef _DEBUG

	if (CScene::GetMode() == CScene::MODE_EDIT)
	{//エディットモードなら
	    CObject::Draw();//オブジェクト描画処理
	}
#endif // _DEBUG

}
//============================================================================================================

//======================================================
//死亡フラグを設定
//======================================================
void CStageManager::SetDeath()
{
	if (GetUseDeath() == true)
	{
		m_StgObjList.clear();//vectorの中身をクリア

		if (m_pState != nullptr)
		{//ステート情報を破棄
			delete m_pState;
			m_pState = nullptr;
		}

		if (m_pBg3D != nullptr)
		{//背景を破棄
			m_pBg3D->SetUseDeath(true);//死亡フラグを使用すrう
			m_pBg3D->SetDeath();       //死亡フラグを設定する
			m_pBg3D = nullptr;         //背景へのポインタを初期化
		}
	}

	CObject::SetDeath();//オブジェクトの死亡フラグ設定処理
}
//============================================================================================================

//============================================
//マップをロードする処理
//============================================
void CStageManager::LoadMapTxt(int nMapNum)
{
	ReleaseAll();//全てのオブジェクトの死亡フラグを発動

	//vectorに保存した情報をリセットする
	m_StgObjList.clear();

	fstream ReadingFile;//読み取り用ファイル
	string Reading_Buff;//読み取り用ファイルの文字列

	//マップ番号配列外対策
	if (nMapNum < 0)
	{
		nMapNum = static_cast<int>(WORLDTYPE::MAX) - 1;
	}
	if (nMapNum >= static_cast<int>(WORLDTYPE::MAX))
	{
		nMapNum = 0;
	}

	ReadingFile.open(m_aWORLDMAP_TXT[nMapNum], ios::in);//読み取りモードで開く

	while (!ReadingFile.eof())
	{
		CObject* pObj = nullptr;//オブジェクトへのポインタを初期化
		ReadingFile >> Reading_Buff;

		if (Reading_Buff == "PLAYER_SPAWNPOINT")
		{//プレイヤーのスポーン位置
			ReadingFile >> Reading_Buff;//イコール
			ReadingFile >> m_SpawnPoint.x;
			ReadingFile >> m_SpawnPoint.y;
			ReadingFile >> m_SpawnPoint.z;
		}
		else if (Reading_Buff == "SETBLOCK")
		{//ブロック
			pObj = CBlock::Create(CBlock::BLOCKTYPE::NORMAL, 1, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), false);//ブロックを生成
		}
		else if (Reading_Buff == "SETBGMODEL")
		{//背景モデル
			pObj = CBgModel::Create(CBgModel::BGMODELTYPE::BILL_00, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), false);//背景モデルを生成
		}
		else if (Reading_Buff == "SETSHOTWEAKENEMY")
		{//射撃に弱い敵
			pObj = CShotWeakEnemy::Create(CShotWeakEnemy::SHOTWEAKENEMYTYPE::NORMAL, 1, 0, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));//敵を生成
		}
		else if (Reading_Buff == "SETDIVEWEAKENEMY")
		{//ダイブに弱い敵
			pObj = CDiveWeakEnemy::Create(CDiveWeakEnemy::DIVEWEAKENEMYTYPE::NORMAL, 1, 0, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);//ダイブに弱い敵を生成
		}
		else if (Reading_Buff == "SETIDLEENEMY")
		{//何もしない敵
			pObj = CIdleEnemy::Create(CIdleEnemy::IDLEENEMYTYPE::NORMAL, 1, 0, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));;//何もしない敵を生成
		}

		if (pObj != nullptr)
		{//オブジェクトへのポインタが存在していたら
			pObj->LoadInfoTxt(ReadingFile, m_StgObjList, Reading_Buff, pObj);//テキストファイルから情報を読み込む
		}
	}

	if (CScene::GetMode() == CScene::MODE_GAME)
	{//ゲームシーンなら
		m_StgObjList.clear();//ステージオブジェクトリストに保存する必要がないのでクリアする
	}

	ReadingFile.close();//ファイルを閉じる
}
//============================================================================================================

//============================================
//マップをセーブする処理
//============================================
void CStageManager::SaveMapTxt(int nMapNum)
{
	fstream WritingFile;                                  //ファイル
	string Writing_Buff;                                  //文字列

	WritingFile.open(m_aWORLDMAP_TXT[nMapNum], ios::out); //書き出しモードでファイルを開く	

	//プレイヤーのスポーンポイントを設定する（小数点第３位まで）
	WritingFile << "PLAYER_SPAWNPOINT = " << fixed << setprecision(3) <<m_SpawnPoint.x << " " <<
		fixed << setprecision(3) << m_SpawnPoint.y << " " <<
		fixed << setprecision(3) << m_SpawnPoint.z << " " << endl << endl;
	
	//ファイルに情報を保存する
	for (list<CObject*>::iterator it = m_StgObjList.begin(); it != m_StgObjList.end();++it)
	{//末尾まで繰り返す
		if (&it != nullptr)
		{
			(*it)->SaveInfoTxt(WritingFile);//情報を書き出す

			WritingFile << endl << endl;    //改行処理
		}
	}

	WritingFile.close();                    //ファイルを閉じる
						                    
	m_StgObjList.clear();                   //Vectorの中身をクリアする
	ReleaseAll();                           //全ての死亡フラグを発動
}
//======================================================================================================================

//==========================================================
//マップを変更する
//==========================================================
void CStageManager::MapChenge()
{
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard();//キー入力情報へのポインタ
	if (pInputKeyboard->GetTrigger(DIK_F3) == true && m_ManagerMode == MANAGERMODE::ALREADYSTAGE)
	{//F3キーを押していて、既存のステージ編集モードだったら
		SaveMapTxt(m_nMapIndex);//現在のマップ情報をセーブする
		m_nMapIndex++;
		if (m_nMapIndex >= static_cast<int>(WORLDTYPE::MAX))
		{//配列外対策
			m_nMapIndex = 0;
		}

		LoadMapTxt(m_nMapIndex);//番号のマップを読み込む
	}
	else if (pInputKeyboard->GetTrigger(DIK_F2) == true && m_ManagerMode == MANAGERMODE::ALREADYSTAGE)
	{//F2キーを押していて、既存のステージ編集モードだったら
		SaveMapTxt(m_nMapIndex);//現在のマップ情報をセーブする
		m_nMapIndex--;
		if (m_nMapIndex < 0)
		{//配列外対策
			m_nMapIndex = static_cast<int>(WORLDTYPE::MAX) - 1;
		}
		LoadMapTxt(m_nMapIndex);//番号のマップを読み込む
	}
}
//======================================================================================================================

//==============================================
//ステージマネージャー生成処理
//==============================================
CStageManager* CStageManager::Create()
{
	CStageManager* pStageManager = DBG_NEW CStageManager;        //ステージマネージャーを生成
	pStageManager->Init();                                       //初期化処理
	pStageManager->SetUseDeath(false);                           //死亡フラグを使用しない
	pStageManager->CObject::SetType(CObject::TYPE::STAGEMANAGER);//オブジェクトの種類を決める   
	return pStageManager;
}
//================================================================================

//===========================================================
//情報表示処理
//===========================================================
void CStageManager::DispInfo()
{
	char aChooseString[32] = {};//選択状態かどうかを記述
	char aCameraTypeName[CCamera::CAMERATYPE_MAX][100] = {};//マップファイル文字格納用
	char aMapModeString[32] = {};                           //マップ編集モード文字格納用

	//マップのカメラモード名
	strcpy(&aCameraTypeName[CCamera::CAMERATYPE_BIRD][0], "バードビュー");
	strcpy(&aCameraTypeName[CCamera::CAMERATYPE_BESIDE][0], "ビサイドビュー");
	strcpy(&aCameraTypeName[CCamera::CAMERATYPE_SIDE][0], "サイドビュー");


	if (m_bChooseObject == false)
	{
		strcpy(&aChooseString[0], "選択していない");
	}
	else
	{
		strcpy(&aChooseString[0], "選択中・・・");
	}

	if (m_ManagerMode == MANAGERMODE::ALREADYSTAGE)
	{
		strcpy(&aMapModeString[0], "既存のステージを編集");
	}
	else
	{
		strcpy(&aMapModeString[0], "新しいステージを編集");
	}

	CManager::GetDebugText()->PrintDebugText("//=================================\n");
	CManager::GetDebugText()->PrintDebugText("//マップエディタの情報\n");
	CManager::GetDebugText()->PrintDebugText("//=================================\n");
	CManager::GetDebugText()->PrintDebugText("現在のステージマネージャー管理オブジェクトの数：%d\n", m_StgObjList.size());
	CManager::GetDebugText()->PrintDebugText("現在のマップ番号(F2、F3で変更）：%d\n", m_nMapIndex);
	CManager::GetDebugText()->PrintDebugText("現在のマップモード（F4）：%s\n",&aMapModeString[0]);
	CManager::GetDebugText()->PrintDebugText("スポーンポイント（P）：%f %f %f\n",m_SpawnPoint.x,m_SpawnPoint.y,m_SpawnPoint.z);
	CManager::GetDebugText()->PrintDebugText("ステート変更 : 8\n");
	CManager::GetDebugText()->PrintDebugText("：末尾のオブジェクトを消去：BACKSPACE\n");
	CManager::GetDebugText()->PrintDebugText("//=================================\n");

}
//=======================================================================================================================

//===========================================================
//状態を変える
//===========================================================
void CStageManager::ChengeState()
{
	bool bStart = false;                            //ステート変更開始フラグ
	int nNowState = static_cast<int>(m_NowState);   //現在のステートを代入
	auto input = CManager::GetInputKeyboard();      //キー入力情報へのポインタ

	if (input->GetTrigger(DIK_8))
	{//8キーを押していたら
		nNowState += (input->GetPress(DIK_LSHIFT) ? -1 : 1);//if ? true : false（条件 ? Trueなら : Falseなら）
		bStart = true;
	}

	if (bStart)
	{
		//破棄
		if (m_pState != nullptr)
		{
			delete m_pState;
			m_pState = nullptr;
		}

		// 範囲制限
		nNowState = (nNowState + static_cast<int>(STATE::MAX)) % static_cast<int>(STATE::MAX);
		m_NowState = static_cast<STATE>(nNowState);

		// ステート生成
		switch (m_NowState)
		{
		case CStageManager::STATE::NEWOBJECT://新しいオブジェクト
			m_pState = DBG_NEW CStageManagerState_NewObject();
			break;
		case CStageManager::STATE::PLACEDOBJECT://既存のオブジェクト
			m_pState = DBG_NEW CStageManagerState_PlacedObject();
			break;
		default:
			assert("想定外の値が入っている");
			break;
		}
	}
}
//=======================================================================================================================

//===========================================================
//拡大率をリセットする処理
//===========================================================
void CStageManager::ResetScale()
{
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_V) == true)
	{
		m_SaveScale = D3DXVECTOR3(1.0f,1.0f,1.0f);
	}
}
//=======================================================================================================================

//***********************************************************************************************************************
//新しいオブジェクト配置状態クラス
//***********************************************************************************************************************

//===========================================================
//コンストラクタ
//===========================================================
CStageManagerState_NewObject::CStageManagerState_NewObject() : m_pManagerObject(nullptr)
{
	//ブロックを生成
	m_pManagerObject = CBlock::Create(CBlock::BLOCKTYPE::NORMAL,10, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f),false);
	m_pManagerObject->SetUseDeath(false);//死亡フラグを使用しない
}
//=======================================================================================================================

//===========================================================
//デストラクタ
//===========================================================
CStageManagerState_NewObject::~CStageManagerState_NewObject()
{
	if (m_pManagerObject != nullptr)
	{//存在していたら
		m_pManagerObject->SetUseDeath(true);//死亡フラグを使用する
		m_pManagerObject->SetDeath();       //死亡フラグを設定する
		m_pManagerObject = nullptr;         //ポインタを初期化
	}
}
//=======================================================================================================================

//===========================================================
//処理
//===========================================================
void CStageManagerState_NewObject::Process(CStageManager* pStageManager)
{
	//ステージマネージャーから情報にアクセスして操作する
	if (m_pManagerObject != nullptr)
	{
		m_pManagerObject->ManagerChooseControlInfo();//オブジェクトクラスに情報を編集するための関数が各派生クラスにoverrideされているので編集することができる

		if (m_pManagerObject->GetObjectType() == CObject::OBJECTTYPE::OBJECTTYPE_X)
		{//オブジェクトタイプが「オブジェクトX」だったら
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_P) == true)
			{//Pキーを押していたら
				//プレイヤーのスポーンポイントを設定
				pStageManager->SetSpawnPoint(static_cast<CObjectX*>(m_pManagerObject)->GetPosInfo().GetPos());
			}
		}

		//注視点を現在編集しているオブジェクトに固定
		CManager::GetCamera()->SetPosR(static_cast<CObjectX*>(m_pManagerObject)->GetPosInfo().GetPos());

		//現在のブロックの種類を変更する
		TypeChenge(pStageManager);

		//ブロックを設定する
		SetObjectX(pStageManager);

		//オブジェクトを消す
		DeleteManagerObject(pStageManager);
	}
}
//=======================================================================================================================

//===========================================================
//オブジェクトの種類を変える
//===========================================================
void CStageManagerState_NewObject::TypeChenge(CStageManager* pStageManager)
{
	CObject* pObj = nullptr;//オブジェクトへのポインタ
	//=====================================================================
	//オブジェクトごとの種類を変更する
	//=====================================================================
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_0) == true)
	{
		pObj = m_pManagerObject->ManagerChengeObject(true);
		m_pManagerObject = pObj;
	}
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_9) == true)
	{
		pObj = m_pManagerObject->ManagerChengeObject(false);
		m_pManagerObject = pObj;
	}
	//=====================================================================================================================================

	//オブジェクトごとのタイプを変える
	ChengeObject(pStageManager);
}
//=======================================================================================================================

//===========================================================
//オブジェクトを設定する
//===========================================================
void CStageManagerState_NewObject::SetObjectX(CStageManager* pStageManager)
{
	list <CObject*>& StgObjList = pStageManager->GetStgObjList();//ステージオブジェクトリストを取得
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) == true)
	{//オブジェクトをリストに保存する
		StgObjList.push_back(m_pManagerObject->ManagerSaveObject());//オブジェクトクラスにManagerSaveObjectがあり、各派生クラスがoverrideしているので派生クラスごとの情報を保存できる
	}
}
//=======================================================================================================================

//===========================================================
//末尾のオブジェクトを消去する
//===========================================================
void CStageManagerState_NewObject::DeleteManagerObject(CStageManager* pStageManager)
{
	list <CObject*>& StgObjList = pStageManager->GetStgObjList();//ステージオブジェクトリストを取得

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_BACKSPACE) == true)
	{//BACKSPACEキーを押していたら末尾の要素を削除
		if (StgObjList.size() > 0)
		{
			auto it = prev(StgObjList.end());//末尾の要素のイテレータを取得

			(*it)->SetUseDeath(true);       //死亡フラグを使用する
			(*it)->SetDeath();              //死亡フラグを設定する

			StgObjList.pop_back();          //末尾の要素を削除する
		}
	}
}
//=======================================================================================================================

//===========================================================
//オブジェクトの変更を行う
//===========================================================
void CStageManagerState_NewObject::ChengeObject(CStageManager* pStageManager)
{
	bool bChengeFlag = false;
	int nNumType = static_cast<int>(m_pManagerObject->GetManagerObjectType());      //ステージマネージャー編集オブジェクトタイプを取得(++、--を使いたいのでintにキャスト)
	CObject::MANAGEROBJECTTYPE ManagerObjectType = CObject::MANAGEROBJECTTYPE::NONE;//ステージマネージャー編集オブジェクトタイプ格納用

	//=====================================================================
	//オブジェクトXの種類を変更する
	//=====================================================================
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_2) == true)
	{//2キーを押していたら
		nNumType++;        //タイプ番号を変える
		bChengeFlag = true;//発動
	}
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_1) == true)
	{//1キーを押していたら
		nNumType--;        //タイプ番号を変える
		bChengeFlag = true;//発動
	}

	if (bChengeFlag == true)
	{//タイプを変えるフラグがオンになったら
		ReleaseObject(pStageManager);

		//配列外チェック
		if (nNumType >= static_cast<int>(CObject::MANAGEROBJECTTYPE::MAX))
		{
			nNumType = static_cast<int>(CObject::MANAGEROBJECTTYPE::NONE) + 1;
		}
		if (nNumType < 1)
		{
			nNumType = static_cast<int>(CObject::MANAGEROBJECTTYPE::MAX) - 1;
		}

		ManagerObjectType = CObject::MANAGEROBJECTTYPE(nNumType);//新しいタイプに設定
	}
	//=====================================================================================================================================

	//オブジェクトの種類を変える
	switch (ManagerObjectType)
	{
	case  CObject::MANAGEROBJECTTYPE::BLOCK://ブロック
		m_pManagerObject = CBlock::Create(CBlock::BLOCKTYPE::NORMAL, 10, pStageManager->GetSavePos(),pStageManager->GetSaveRot(),pStageManager->GetSaveScale(),false);
		break;
	case CObject::MANAGEROBJECTTYPE::BGMODEL://背景モデル
		m_pManagerObject = CBgModel::Create(CBgModel::BGMODELTYPE::BILL_00, pStageManager->GetSavePos(), pStageManager->GetSaveRot(), pStageManager->GetSaveScale(),false);
		break;
	case CObject::MANAGEROBJECTTYPE::SHOTWEAKENEMY://射撃に弱い敵
		m_pManagerObject = CShotWeakEnemy::Create(CShotWeakEnemy::SHOTWEAKENEMYTYPE::NORMAL, 10, 0, pStageManager->GetSavePos(), pStageManager->GetSaveRot(), pStageManager->GetSaveScale());
		break;
	case CObject::MANAGEROBJECTTYPE::DIVEWEAKENEMY://ダイブに弱い敵
		m_pManagerObject = CDiveWeakEnemy::Create(CDiveWeakEnemy::DIVEWEAKENEMYTYPE::NORMAL, 10, 0, pStageManager->GetSavePos(), pStageManager->GetSaveRot(), pStageManager->GetSaveScale(),0);
		break;
	case CObject::MANAGEROBJECTTYPE::IDLEENEMY://何もしない敵
		m_pManagerObject = CIdleEnemy::Create(CIdleEnemy::IDLEENEMYTYPE::NORMAL, 10, 0, pStageManager->GetSavePos(), pStageManager->GetSaveRot(), pStageManager->GetSaveScale());
		break;
	default:
		break;
	}

	if (m_pManagerObject != nullptr)
	{//今選んでいるオブジェクトだけは破棄されない
		m_pManagerObject->SetUseDeath(false);
	}
}
//=======================================================================================================================

//===========================================================
//末尾のオブジェクトの破棄を行う
//===========================================================
void CStageManagerState_NewObject::ReleaseObject(CStageManager* pStageManager)
{
	//最低限の情報を保存する
	switch (m_pManagerObject->GetObjectType())
	{
	case CObject::OBJECTTYPE::OBJECTTYPE_X://オブジェクトタイプが「オブジェクトX」だったら
		pStageManager->SetSavePos(static_cast<CObjectX*>(m_pManagerObject)->GetPosInfo().GetPos());
		pStageManager->SetSaveRot(static_cast<CObjectX*>(m_pManagerObject)->GetRotInfo().GetRot());
		pStageManager->SetSaveScale(static_cast<CObjectX*>(m_pManagerObject)->GetSizeInfo().GetScale());
		break;
	default:
		break;
	}
	if (m_pManagerObject != nullptr)
	{
		m_pManagerObject->SetUseDeath(true);//死亡フラグを使用する
		m_pManagerObject->SetDeath();       //死亡フラグを設定する
		m_pManagerObject = nullptr;         //ポインタを初期化
	}
}
//=======================================================================================================================

//***********************************************************************************************************************
//既存のオブジェクト編集クラス
//***********************************************************************************************************************

//===========================================================
//コンストラクタ
//===========================================================
CStageManagerState_PlacedObject::CStageManagerState_PlacedObject() : m_nChooseObjIdx(0)
{

}
//=======================================================================================================================

//===========================================================
//デストラクタ
//===========================================================
CStageManagerState_PlacedObject::~CStageManagerState_PlacedObject()
{

}
//=======================================================================================================================

//===========================================================
//処理
//===========================================================
void CStageManagerState_PlacedObject::Process(CStageManager* pStageManager)
{
	ChengeIdx(pStageManager);   //選択オブジェクト番号の変更
	ChooseObject(pStageManager);//配置オブジェクトを選択する
}
//=======================================================================================================================

//===========================================================
//オブジェクト選択
//===========================================================
void CStageManagerState_PlacedObject::ChooseObject(CStageManager* pStageManager)
{
	list<CObject*> & StgObjList = pStageManager->GetStgObjList();//ステージマネージャーオブジェクトリストを取得

	auto it = StgObjList.begin();//最初のポインタを取得
	advance(it, m_nChooseObjIdx);//現在のポインタから見た選んでいる番号まで移動する

	(*it)->ManagerChooseControlInfo();//オブジェクトクラスに情報を編集するための関数が各派生クラスにoverrideされているので編集することができる

	DeleteObject(it, StgObjList);     //現在選択しているオブジェクトを破棄する
}
//=======================================================================================================================

//===========================================================
//選択オブジェクト番号を変える
//===========================================================
void CStageManagerState_PlacedObject::ChengeIdx(CStageManager* pStageManager)
{
	list<CObject*>& StgObjList = pStageManager->GetStgObjList();
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_RIGHT) == true)
	{//RIGHTキーを押していたら
		m_nChooseObjIdx++;
	}
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_LEFT) == true)
	{//LEFTキーを押していたら
		m_nChooseObjIdx--;
	}

	int nSize = StgObjList.size();//ステージマネージャーオブジェクトリストのサイズを取得

	//配列外対策
	if (m_nChooseObjIdx >= nSize)
	{
		m_nChooseObjIdx = 0;
	}
	if (m_nChooseObjIdx < 0)
	{
		m_nChooseObjIdx = nSize - 1;
	}
}
//=======================================================================================================================

//===========================================================
//選択オブジェクトを消去する
//===========================================================
void CStageManagerState_PlacedObject::DeleteObject(list<CObject*>::iterator& it, list<CObject*>& StgObjList)
{
	auto Input = CManager::GetInputKeyboard();//キー入力情報を取得
	if (Input->GetTrigger(DIK_BACKSPACE))
	{//BACKSPACEキーを押していたら
		(*it)->SetUseDeath(true);//死亡フラグを使用する
		(*it)->SetDeath();       //死亡フラグを設定する
		StgObjList.erase(it);    //イテレータで選んでいるオブジェクトを破棄する
	}
}
//=======================================================================================================================