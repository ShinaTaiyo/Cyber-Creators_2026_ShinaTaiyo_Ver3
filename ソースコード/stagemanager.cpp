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

	m_pState = DBG_NEW CStageManagerState_NewObject();

	CObject::Init();

	SetUseDeath(false);
	if (CScene::GetMode() == CScene::MODE_EDIT)
	{
		LoadMapTxt(0);
	}
	return S_OK;
}
//==========================================================

//================================
//終了処理
//================================
void CStageManager::Uninit()
{
	CObject::Uninit();
}
//==========================================================

//================================
//更新処理
//================================
void CStageManager::Update()
{

#ifdef _DEBUG

	if (CScene::GetMode() == CScene::MODE_EDIT)
	{
		MapChenge();  //マップを変える処理

		ChengeState();//ステート変更

		//処理
		if (m_pState != nullptr)
		{
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
	{
        //オブジェクト描画
	    CObject::Draw();
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
			m_pBg3D->SetUseDeath(true);
			m_pBg3D->SetDeath();
			m_pBg3D = nullptr;
		}
	}

	CObject::SetDeath();
}
//============================================================================================================

//============================================
//マップをロードする処理
//============================================
void CStageManager::LoadMapTxt(int nMapNum)
{
	ReleaseAll();

	//vectorに保存した情報をリセットする
	m_StgObjList.clear();

	fstream ReadingFile;//読み取り用ファイル
	string Reading_Buff;//読み取り用ファイルの文字列

	//マップ番号が最大値や最低値を上回った場合
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
		ReadingFile >> Reading_Buff;

		if (Reading_Buff == "PLAYER_SPAWNPOINT")
		{
			ReadingFile >> Reading_Buff;//イコール
			ReadingFile >> m_SpawnPoint.x;
			ReadingFile >> m_SpawnPoint.y;
			ReadingFile >> m_SpawnPoint.z;
		}
		else if (Reading_Buff == "SETBLOCK")
		{
			CBlock::LoadInfoTxt(ReadingFile, m_StgObjList, Reading_Buff);
		}
		else if (Reading_Buff == "SETBGMODEL")
		{
			CBgModel::LoadInfoTxt(ReadingFile, m_StgObjList, Reading_Buff);
		}
		else if (Reading_Buff == "SETSHOTWEAKENEMY")
		{
			CShotWeakEnemy::LoadInfoTxt(ReadingFile, m_StgObjList, Reading_Buff);
		}
		else if (Reading_Buff == "SETDIVEWEAKENEMY")
		{
			CDiveWeakEnemy::LoadInfoTxt(ReadingFile, m_StgObjList, Reading_Buff);
		}
		else if (Reading_Buff == "SETIDLEENEMY")
		{
			CIdleEnemy::LoadInfoTxt(ReadingFile, m_StgObjList, Reading_Buff);
		}
	}

	if (CScene::GetMode() == CScene::MODE_GAME)
	{
		m_StgObjList.clear();
	}

	ReadingFile.close();//ファイルを閉じる
}
//============================================================================================================

//===========================================
//マップのファイルパスをロードする
//===========================================
void CStageManager::LoadMapFilePass(WORLDTYPE type)
{
	//char aString[512] = {};
	////===============================================================================================

	//m_nMapNum = 0;//マップ総数を初期化

	//FILE* pFileTxt = nullptr;
	//pFileTxt = fopen(m_apWORLDMAP_TXT[type], "r");

	//if (pFileTxt != nullptr)
	//{
	//	while (1)
	//	{
	//		fscanf(pFileTxt, "%s", &aString[0]);
	//		if (aString[0] == '#')
	//		{
	//			fgets(&aString[0], 100, pFileTxt);//行をスキップ
	//		}
	//		else if (strcmp(&aString[0], "FILENAME") == 0)
	//		{
	//			fscanf(pFileTxt, "%s", &aString[0]);//イコール読み取り用
	//			fscanf(pFileTxt, "%s", &m_aMapFilePass[m_nMapNum][0]);//ファイル名読み取り用
	//			m_nMapNum++;
	//		}
	//		else if (strcmp(&aString[0], "END_SCRIPT") == 0)
	//		{
	//			fclose(pFileTxt);
	//			break;
	//		}
	//	}

	//}

}
//============================================================================================================

//============================================
//マップをセーブする処理
//============================================
void CStageManager::SaveMapTxt(int nMapNum)
{
	fstream WritingFile;    //ファイル
	string Writing_Buff;    //文字列
	CObject::TYPE Type = {};//オブジェクト種類

	WritingFile.open(m_aWORLDMAP_TXT[nMapNum], ios::out);//読み取りモードでファイルを開く	
	//プレイヤーのスポーンポイントを設定する
	WritingFile << "PLAYER_SPAWNPOINT = " << fixed << setprecision(3) <<m_SpawnPoint.x << " " <<
		fixed << setprecision(3) << m_SpawnPoint.y << " " <<
		fixed << setprecision(3) << m_SpawnPoint.z << " " << endl << endl;
	
	//ファイルに情報を保存する
	for (list<CObject*>::iterator it = m_StgObjList.begin(); it != m_StgObjList.end();++it)
	{//末尾まで繰り返す
		if (&it != nullptr)
		{
			Type = (*it)->GetType();
			(*it)->SaveInfoTxt(WritingFile);

			WritingFile << endl << endl;//改行処理
		}
	}

	WritingFile.close();//ファイルを閉じる

	m_StgObjList.clear();//Vectorの中身をクリアする
	ReleaseAll();        //全ての死亡フラグを発動
}
//======================================================================================================================

//==========================================================
//マップを変更する
//==========================================================
void CStageManager::MapChenge()
{
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_F3) == true && m_ManagerMode == MANAGERMODE::ALREADYSTAGE)
	{
		SaveMapTxt(m_nMapIndex);//現在のマップ情報をセーブする
		m_nMapIndex++;
		if (m_nMapIndex >= static_cast<int>(WORLDTYPE::MAX))
		{
			m_nMapIndex = 0;
		}

		LoadMapTxt(m_nMapIndex);//番号のマップを読み込む
	}
	else if (CManager::GetInputKeyboard()->GetTrigger(DIK_F2) == true && m_ManagerMode == MANAGERMODE::ALREADYSTAGE)
	{
		SaveMapTxt(m_nMapIndex);//現在のマップ情報をセーブする
		m_nMapIndex--;
		if (m_nMapIndex < 0)
		{
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
	CStageManager* pStageManager = DBG_NEW CStageManager;                                                                        //プレイヤーを生成

	bool bSuccess = pStageManager->CObject::GetCreateSuccess();
	if (bSuccess == true)
	{//空きがあったので生成開始
		if (pStageManager != nullptr)
		{
			pStageManager->Init();                                                                                           //初期化処理
			pStageManager->SetUseDeath(false);                                                                        //死亡フラグを発動するかどうかを設定する
			pStageManager->CObject::SetType(CObject::TYPE::STAGEMANAGER);                                                     //オブジェクトの種類を決める   
		}
	}
	else
	{//オブジェクトに空きがなかったので破棄する
		delete pStageManager;
		pStageManager = nullptr;
	}

	return pStageManager;
}
//================================================================================

//===========================================================
//情報表示処理
//===========================================================
void CStageManager::DispInfo()
{
	char aChooseString[32] = {};//選択状態かどうかを記述
	char aCameraTypeName[CCamera::CAMERATYPE_MAX][100] = {};
	char aMapModeString[32] = {};
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
	bool bStart = false;//ステート変更開始フラグ
	int nNowState = static_cast<int>(m_NowState);//現在のステートを代入
	auto input = CManager::GetInputKeyboard();//

	if (input->GetTrigger(DIK_8))
	{
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
		case CStageManager::STATE::NEWOBJECT:
			m_pState = DBG_NEW CStageManagerState_NewObject();
			break;
		case CStageManager::STATE::PLACEDOBJECT:
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
	m_pManagerObject = CBlock::Create(CBlock::BLOCKTYPE::NORMAL,10, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f),false);
	m_pManagerObject->SetUseDeath(false);
}
//=======================================================================================================================

//===========================================================
//デストラクタ
//===========================================================
CStageManagerState_NewObject::~CStageManagerState_NewObject()
{
	if (m_pManagerObject != nullptr)
	{
		m_pManagerObject->SetUseDeath(true);
		m_pManagerObject->SetDeath();
		m_pManagerObject = nullptr;
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
		m_pManagerObject->ManagerChooseControlInfo();

		if (m_pManagerObject->GetObjectType() == CObject::OBJECTTYPE::OBJECTTYPE_X)
		{
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_P) == true)
			{
				pStageManager->SetSpawnPoint(static_cast<CObjectX*>(m_pManagerObject)->GetPosInfo().GetPos());
			}
		}

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
	CObject* pObj = nullptr;
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
	list <CObject*>& StgObjList = pStageManager->GetStgObjList();
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) == true)
	{//オブジェクトをVectorの先頭に保存する
		StgObjList.push_back(m_pManagerObject->ManagerSaveObject());
	}
}
//=======================================================================================================================

//===========================================================
//末尾のオブジェクトを消去する
//===========================================================
void CStageManagerState_NewObject::DeleteManagerObject(CStageManager* pStageManager)
{
	list <CObject*>& StgObjList = pStageManager->GetStgObjList();

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_BACKSPACE) == true)
	{
		if (StgObjList.size() > 0)
		{
			auto it = prev(StgObjList.end());//末尾の要素のイテレータを取得

			(*it)->SetUseDeath(true);
			(*it)->SetDeath();

			StgObjList.pop_back();//末尾の要素を削除する
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
	int nNumType = static_cast<int>(m_pManagerObject->GetManagerObjectType());
	CObject::MANAGEROBJECTTYPE ManagerObjectType = CObject::MANAGEROBJECTTYPE::NONE;

	//=====================================================================
	//オブジェクトXの種類を変更する
	//=====================================================================
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_2) == true)
	{
		nNumType++;
		bChengeFlag = true;//発動
	}
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_1) == true)
	{
		nNumType--;
		bChengeFlag = true;//発動
	}

	if (bChengeFlag == true)
	{
		ReleaseObject(pStageManager);
		if (nNumType >= static_cast<int>(CObject::MANAGEROBJECTTYPE::MAX))
		{
			nNumType = static_cast<int>(CObject::MANAGEROBJECTTYPE::NONE) + 1;
		}
		if (nNumType < 1)
		{
			nNumType = static_cast<int>(CObject::MANAGEROBJECTTYPE::MAX) - 1;
		}

		ManagerObjectType = CObject::MANAGEROBJECTTYPE(nNumType);
	}
	//=====================================================================================================================================

	//オブジェクトの種類を変える
	switch (ManagerObjectType)
	{
	case  CObject::MANAGEROBJECTTYPE::BLOCK:
		m_pManagerObject = CBlock::Create(CBlock::BLOCKTYPE::NORMAL, 10, pStageManager->GetSavePos(),pStageManager->GetSaveRot(),pStageManager->GetSaveScale(),false);
		break;
	case CObject::MANAGEROBJECTTYPE::BGMODEL:
		m_pManagerObject = CBgModel::Create(CBgModel::BGMODELTYPE::BILL_00, pStageManager->GetSavePos(), pStageManager->GetSaveRot(), pStageManager->GetSaveScale(),false);
		break;
	case CObject::MANAGEROBJECTTYPE::SHOTWEAKENEMY:
		m_pManagerObject = CShotWeakEnemy::Create(CShotWeakEnemy::SHOTWEAKENEMYTYPE::NORMAL, 10, 0, pStageManager->GetSavePos(), pStageManager->GetSaveRot(), pStageManager->GetSaveScale());
		break;
	case CObject::MANAGEROBJECTTYPE::DIVEWEAKENEMY:
		m_pManagerObject = CDiveWeakEnemy::Create(CDiveWeakEnemy::DIVEWEAKENEMYTYPE::NORMAL, 10, 0, pStageManager->GetSavePos(), pStageManager->GetSaveRot(), pStageManager->GetSaveScale(),0);
		break;
	case CObject::MANAGEROBJECTTYPE::IDLEENEMY:
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
	case CObject::OBJECTTYPE::OBJECTTYPE_X:
		pStageManager->SetSavePos(static_cast<CObjectX*>(m_pManagerObject)->GetPosInfo().GetPos());
		pStageManager->SetSaveRot(static_cast<CObjectX*>(m_pManagerObject)->GetRotInfo().GetRot());
		pStageManager->SetSaveScale(static_cast<CObjectX*>(m_pManagerObject)->GetSizeInfo().GetScale());
		break;
	default:
		break;
	}
	if (m_pManagerObject != nullptr)
	{
		m_pManagerObject->SetUseDeath(true);
		m_pManagerObject->SetDeath();
		m_pManagerObject = nullptr;
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
	list<CObject*> & StgObjList = pStageManager->GetStgObjList();

	auto it = StgObjList.begin();//最初のポインタを取得
	advance(it, m_nChooseObjIdx);//現在のポインタから見た選んでいる番号まで移動する

	(*it)->ManagerChooseControlInfo();

	DeleteObject(it, StgObjList);
}
//=======================================================================================================================

//===========================================================
//選択オブジェクト番号を変える
//===========================================================
void CStageManagerState_PlacedObject::ChengeIdx(CStageManager* pStageManager)
{
	list<CObject*>& StgObjList = pStageManager->GetStgObjList();
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_RIGHT) == true)
	{
		m_nChooseObjIdx++;
	}
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_LEFT) == true)
	{
		m_nChooseObjIdx--;
	}

	int nSize = StgObjList.size();
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
	auto Input = CManager::GetInputKeyboard();
	if (Input->GetTrigger(DIK_BACKSPACE))
	{
		(*it)->SetUseDeath(true);
		(*it)->SetDeath();
		StgObjList.erase(it);
	}
}
//=======================================================================================================================