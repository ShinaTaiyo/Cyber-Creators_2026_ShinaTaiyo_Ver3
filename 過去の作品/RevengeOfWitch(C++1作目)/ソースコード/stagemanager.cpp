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
#include "effect.h"
#include "objectX.h"
#include "objectXInfo.h"
#include "input.h"
#include "manager.h"
#include "trap.h"
#include "object.h"
#include "camera.h"
#include "item.h"
#include "marker.h"
#include "particle.h"
#include "boss.h"
#include "debugtext.h"
#include "signboard.h"
#include "bg3d.h"
#include <stdio.h>
#include <string.h>
#include "calculation.h"
//==========================================================

//================================
//前方宣言
//================================
const char* CStageManager::m_apWORLDMAP_TXT[CStageManager::WORLDTYPE_MAX] =
{
	"data\\TEXTFILE\\Map\\EasyMap.txt",
	"data\\TEXTFILE\\Map\\NormalMap.txt",
	"data\\TEXTFILE\\Map\\BossRushMap.txt",
};

//================================
//コンストラクタ
//================================
CStageManager::CStageManager() : m_nWorldIndex(0),m_pBg3D(nullptr)
{
	for (int nCnt = 0; nCnt < m_nMAXMANAGEROBJECT; nCnt++)
	{
		m_apObjectList[nCnt] = nullptr;
	}

	for (int nCnt = 0; nCnt < m_nMAX_MAP; nCnt++)
	{
		for (int nCntWord = 0; nCntWord < m_nMAX_WORD; nCntWord++)
		{
			m_aMapFilePass[nCnt][nCntWord] = {};
		}
	}
}
//==========================================================

//================================
//デストラクタ
//================================
CStageManager::~CStageManager()
{

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
	m_nCntTime = 0;                                      //出現してからの時間をカウントする
	m_nManagerType = MANAGEROBJECT_BLOCK;                //マネージャーオブジェクトのタイプ番号
	m_nType = 0;                                         //オブジェクトごとのタイプ番号
	m_nMapIndex = 0;                                     //マップのインデックス
	m_nMapNum = 0;                                       //マップの総数
	m_nRotType = 0;                                      //向きの種類 
	m_Move = NULL_VECTOR3;                               //移動量
	m_bMove = NULL_VECTOR3;                              //移動しているかどうか
	m_Scale = ONE_VECTOR3;                               //拡大率
	m_Pos = NULL_VECTOR3;                                //位置
	m_SaveBeforeChoosePos = NULL_VECTOR3;                //選択処理をする前の位置を記憶する
	m_fSpeed = 5.0f;                                     //移動速度
 	m_pManagerObject = nullptr;                          //マネージャーに表示するオブジェクト
	m_nIndexObject = 0;                                  //オブジェクト番号
	m_ManagerMode = MANAGERMODE_ALREADYSTAGE;            //現在のステージマネーシャーのモード
	m_nWarpMapNum = 0;                                   //ワープマーカーのワープ先マップの設定
	m_WarpPos = NULL_VECTOR3;                            //ワープマーカーのワープ位置を設定
	m_nSetLife = 10;                                     //体力を設定する
	m_nSetSubType = 0;                                   //設定するサブタイプの番号  
	m_nMapCameraType = (int)(CCamera::CAMERATYPE_BESIDE);//マップごとのカメラの種類を設定

	//=========================
	//選択系
	//=========================
	m_pChooseObject = nullptr;                          //選択オブジェクトへの
	m_nIndexChooseObject = 0;                           //選択しているオブジェクトの番号
	m_bChooseObject = false;                            //オブジェクトを選択しているかどうか
	m_bMakeMapMode = false;                             //マップ制作モードかどうか
	//=======================================================================================
#ifdef _DEBUG

	if (CScene::GetMode() == CScene::MODE_EDIT)
	{
	    m_pManagerObject = CBlock::Create(CBlock::BLOCKTYPE00_NORMAL, 10, NULL_VECTOR3, NULL_VECTOR3, ONE_VECTOR3, CObjectX::ROTTYPE_NORMAL,m_nSetSubType);
		m_pManagerObject->SetUseDeath(false);
	    //m_pManagerObject = CModel::Create(CModel::MODELTYPE_BILL,NULL_VECTOR3, NULL_VECTOR3, NULL_VECTOR3, m_Scale, CObjectX::ROTTYPE_NORMAL);
	}
#endif // _DEBUG

	//===========================
	//移動モードを決める
	//===========================
	m_MoveMode = MOVEMODE00_XY;     //移動方法の種類
	m_FocusType = FOCUSTYPE_NORMAL;//カメラが追う位置の種類
	m_bUseSizeMove = false;//現在のオブジェクトのサイズ分移動するかどうか
	//=======================================================================================

	//===========================
	//オブジェクトX初期化処理
	//===========================
	CObject::Init();
	//=======================================================================================

	//===========================
	//3D背景を作る
	//===========================
	m_pBg3D = CBg3D::Create(CBg3D::BG3DTYPE_STAGE00, NULL_VECTOR3, 20, 2, 8000.0f, 8000.0f);

	//======================================
	//マップのファイルパスをロードする
	//======================================
	LoadMapFilePass(WORLDTYPE00_EASY);
	return S_OK;
}
//==========================================================

//================================
//終了処理
//================================
void CStageManager::Uninit()
{
	StageManagerObjectReleaseAll();
     
}
//==========================================================

//================================
//別枠の終了処理
//================================
void CStageManager::ExtraUninit()
{

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
		if (m_bChooseObject == false)
		{
			//===============================================
			//ステージマネージャーの色を変える
			//===============================================
			ColorChenge();
			//===========================================================================================

			//===============================================
			//体力を設定する
			//===============================================
			LifeSet();
			//===========================================================================================

			//===============================================
			//ステージマネージャーの位置を移動する
			//===============================================
			MoveManager();
			//===========================================================================================

			//===============================================
			//現在のブロックの種類を変更する
			//===============================================
			TypeChenge();
			//===========================================================================================

			//===============================================
			//拡大率をリセットする処理
			//===============================================
			ResetScale();
			//===========================================================================================

			//===============================================
			//ブロックを設定する
			//===============================================
			SetObjectX();
			//===========================================================================================

			////===============================================
			////ブロックを消す処理
			////===============================================
			//DeleteObjectX();
			////===========================================================================================

			//===============================================
			//マップを変える処理
			//===============================================
			MapChenge();
			//===========================================================================================

			//===============================================
			//オブジェクト全消去処理
			//===============================================
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_RSHIFT) == true)
			{
				StageManagerObjectReleaseAll();
			}
			//===========================================================================================
		}

		//===============================================
		//オブジェクト選択処理
		//===============================================
		ChooseObject();
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
	if (m_pManagerObject != nullptr)
	{
		m_pManagerObject->SetUseDeath(true);
		m_pManagerObject->SetDeath();
		m_pManagerObject = nullptr;
	}

	if (m_pBg3D != nullptr)
	{
		m_pBg3D->SetUseDeath(true);
		m_pBg3D->SetDeath();
		m_pBg3D = nullptr;
	}

	CObject::SetDeath();
}
//============================================================================================================

//============================================
//マップをロードする処理
//============================================
void CStageManager::LoadMapTxt(int nMapNum)
{
	//==================
	//変数
	//==================
	char aString[128] = {};
	m_nMapIndex = nMapNum;//マップのインデックスを設定
	FILE* pFile = nullptr;//ファイルポインタを宣言

	//=================================================
	//現在のモードによってロードするファイルを変える
	//=================================================
	if (m_ManagerMode == MANAGERMODE_ALREADYSTAGE)
	{
		pFile = fopen(&m_aMapFilePass[m_nMapIndex][0], "r");
	}
	else
	{
		pFile = fopen(SAVE_TXT, "r");
	}
	//=====================================================================================================================

	int nManagerType = 0;                  //マネージャーオブジェクトの種類
	int nType = 0;                         //オブジェクトごとの種類
	int nRotType = 0;                      //向きの種類
	int nSubType = 0;                      //サブタイプ
	D3DXVECTOR3 pos = NULL_VECTOR3;        //位置
	D3DXVECTOR3 WarpPos = NULL_VECTOR3;
	int nWarpMap = 0;//ワープ先のマップ番号
	int nLife = 0;//体力
	D3DXVECTOR3 Scale = NULL_VECTOR3;      //拡大率
	D3DXVECTOR3 Rot = NULL_VECTOR3;        //向き
	CObject* pObject = nullptr;            //オブジェクトへのポインタ
	//======================================================

	//ReleaseAll();
	if (CScene::GetMode() == CScene::MODE_EDIT)
	{
		StageManagerObjectReleaseAll();
	}


	if (pFile != nullptr)
	{
		while (1)
		{

			fscanf(pFile, "%s", &aString[0]);

			if (aString[0] == '#')
			{
				fgets(&aString[0], 100, pFile);//行をスキップ
			}
			else if (strcmp(&aString[0], "SETCAMERATYPE") == 0)
			{
				fscanf(pFile, "%s", &aString[0]);  //イコール読み取り
				fscanf(pFile, "%d", &m_nMapCameraType);//カメラの種類を取得
				CManager::GetCamera()->SetCameraType((CCamera::CAMERATYPE)(m_nMapCameraType));//取得したカメラの種類に設定
			}
			else if (strcmp(&aString[0], "SETOBJECT") == 0)
			{
				while (1)
				{
					fscanf(pFile, "%s", &aString[0]);
					if (aString[0] == '#')
					{
						fgets(&aString[0], 100, pFile);//行をスキップ
					}
					else if (strcmp(&aString[0], "MANAGERTYPE") == 0)
					{
						fscanf(pFile, "%s", &aString[0]);  //イコール読み取り
						fscanf(pFile, "%d", &nManagerType);//マネージャーオブジェクトのタイプ取得
					}
					else if (strcmp(&aString[0], "TYPE") == 0)
					{
						fscanf(pFile, "%s", &aString[0]);  //イコール読み取り
						fscanf(pFile, "%d", &nType);       //オブジェクトごとのタイプ取得
					}
					else if (strcmp(&aString[0], "ROTTYPE") == 0)
					{
						fscanf(pFile, "%s", &aString[0]);  //イコール読み取り
						fscanf(pFile, "%d", &nRotType);    //オブジェクトごとのタイプ取得
					}
					else if (strcmp(&aString[0], "POS") == 0)
					{
						fscanf(pFile, "%s", &aString[0]);  //イコール読み取り
						fscanf(pFile, "%f", &pos.x);       //X座標読み取り
						fscanf(pFile, "%f", &pos.y);       //Y座標読み取り
						fscanf(pFile, "%f", &pos.z);       //Z座標読み取り
					}
					else if (strcmp(&aString[0], "SCALE") == 0)
					{
						fscanf(pFile, "%s", &aString[0]);  //イコール読み取り
						fscanf(pFile, "%f", &Scale.x);       //X座標読み取り
						fscanf(pFile, "%f", &Scale.y);       //Y座標読み取り
						fscanf(pFile, "%f", &Scale.z);       //Z座標読み取り
					}
					else if (strcmp(&aString[0], "ROT") == 0)
					{
						fscanf(pFile, "%s", &aString[0]);  //イコール読み取り
						fscanf(pFile, "%f", &Rot.x);       //X座標読み取り
						fscanf(pFile, "%f", &Rot.y);       //Y座標読み取り
						fscanf(pFile, "%f", &Rot.z);       //Z座標読み取り
					}
					else if (strcmp(&aString[0], "LIFE") == 0)
					{
						fscanf(pFile, "%s", &aString[0]);  //イコール読み取り
						fscanf(pFile, "%d", &nLife);       //Z座標読み取り
					}
					else if (strcmp(&aString[0], "SUBTYPE") == 0)
					{
						fscanf(pFile, "%s", &aString[0]);  //イコール読み取り
						fscanf(pFile, "%d", &nSubType);    //サブタイプ
					}
					else if (strcmp(&aString[0], "END_SETOBJECT") == 0)
					{//Xオブジェクトを設定
						if (CScene::GetMode() == CScene::MODE_EDIT)
						{//エディットモードなら、オブジェクトをリストに保存し管理する
							switch (nManagerType)
							{
								case (int)(MANAGEROBJECT_BLOCK) :
									pObject = CBlock::Create((CBlock::BLOCKTYPE)(nType), 10, pos, Rot, Scale, CObjectX::ROTTYPE_NORMAL, nSubType);
									break;
									case (int)(MANAGEROBJECT_MODEL) :
										pObject = CModel::Create((CModel::MODELTYPE)(nType), pos, Rot, NULL_VECTOR3, Scale, CObjectX::ROTTYPE_NORMAL);
										break;
										case (int)(MANAGEROBJECT_ENEMY) :
											pObject = CEnemy::Create((CEnemy::ENEMYTYPE)(nType), nLife, pos, NULL_VECTOR3, Scale,NULL_VECTOR3,NULL_VECTOR3,ONE_VECTOR3);//生成処理
											break;
											case (int)(MANAGEROBJECT_ITEM) :
												pObject = CItem::Create((CItem::ITEMTYPE)(nType), pos, Rot, Scale, CObjectX::ROTTYPE_NORMAL);//生成処理
												break;
												case (int)(MANAGEROBJECT_MARKER) :
													pObject = CMarker::Create((CMarker::MARKERTYPE)(nType), pos, Scale, NULL_VECTOR3, 0);
													break;
													case (int)(MANAGEROBJECT_BOSS) :
														pObject = CBoss::SetCreateBoss((CBoss::BOSSTYPE)(nType),nLife,pos,Scale);
														break;
														case (int)(MANAGEROBJECT_SIGNBOARD):
															pObject = CSignBoard::Create(CSignBoard::SIGNBOARDTYPE(nType),
																(CSignBoard::SIGNBOARDMANUALTYPE)(nSubType),pos,Scale,Rot);													default:
														break;
							}

							//#ifdef _DEBUG
							if (pObject != nullptr)
							{	
								if (pObject->GetType() == CObject::TYPE_BOSS)
								{
									//
									int nCnt = 0;
								}

								pObject->SetStageManagerObjNum(m_nIndexObject);//ステージマネージャーで管理するオブジェクトの番号を設定
								pObject->SetStageManagerObj();                 //ステージマネージャーで管理するオブジェクトに設定	
								m_nIndexObject++;
							}
							//#endif // _DEBUG
						}
						else if (CScene::GetMode() == CScene::MODE_GAME)
						{//ゲームモードなら、オブジェクトを生成するだけで、リストに保存しない
							switch (nManagerType)
							{
								case (int)(MANAGEROBJECT_BLOCK) :
									CBlock::Create((CBlock::BLOCKTYPE)(nType), 10, pos, Rot, Scale, CObjectX::ROTTYPE_NORMAL, nSubType);
									break;
									case (int)(MANAGEROBJECT_MODEL) :
										CModel::Create((CModel::MODELTYPE)(nType), pos, Rot, NULL_VECTOR3, Scale, CObjectX::ROTTYPE_NORMAL);
										break;
										case (int)(MANAGEROBJECT_ENEMY) :
											CEnemy::Create((CEnemy::ENEMYTYPE)(nType), nLife, pos, NULL_VECTOR3, Scale,NULL_VECTOR3,NULL_VECTOR3,ONE_VECTOR3);//生成処理
											break;
											case (int)(MANAGEROBJECT_ITEM) :
												CItem::Create((CItem::ITEMTYPE)(nType), pos, Rot, Scale, CObjectX::ROTTYPE_NORMAL);//生成処理
												break;
												case (int)(MANAGEROBJECT_MARKER) :
													CMarker::Create((CMarker::MARKERTYPE)(nType), pos, Scale, NULL_VECTOR3, 0);
													break;
													case (int)(MANAGEROBJECT_BOSS) :
														CBoss::SetCreateBoss((CBoss::BOSSTYPE)(nType), nLife, pos, Scale);
														break;
														case (int)(MANAGEROBJECT_SIGNBOARD) : CSignBoard::Create(CSignBoard::SIGNBOARDTYPE(nType), 
															(CSignBoard::SIGNBOARDMANUALTYPE)(nSubType), pos, Scale, Rot);
															break;
													default:
														break;
							}
						}
						break;
					}

				}
			}
			else if (strcmp(&aString[0], "SETMARKER") == 0)
			{
				while (1)
				{
					fscanf(pFile, "%s", &aString[0]);
					if (aString[0] == '#')
					{
						fgets(&aString[0], 100, pFile);//行をスキップ
					}
					else if (strcmp(&aString[0], "MANAGERTYPE") == 0)
					{
						fscanf(pFile, "%s", &aString[0]);  //イコール読み取り
						fscanf(pFile, "%d", &nManagerType);//マネージャーオブジェクトのタイプ取得
					}
					else if (strcmp(&aString[0], "TYPE") == 0)
					{
						fscanf(pFile, "%s", &aString[0]);  //イコール読み取り
						fscanf(pFile, "%d", &nType);       //オブジェクトごとのタイプ取得
					}
					else if (strcmp(&aString[0], "POS") == 0)
					{
						fscanf(pFile, "%s", &aString[0]);  //イコール読み取り
						fscanf(pFile, "%f", &pos.x);       //X座標読み取り
						fscanf(pFile, "%f", &pos.y);       //Y座標読み取り
						fscanf(pFile, "%f", &pos.z);       //Z座標読み取り
					}
					else if (strcmp(&aString[0], "SCALE") == 0)
					{
						fscanf(pFile, "%s", &aString[0]);  //イコール読み取り
						fscanf(pFile, "%f", &Scale.x);       //X座標読み取り
						fscanf(pFile, "%f", &Scale.y);       //Y座標読み取り
						fscanf(pFile, "%f", &Scale.z);       //Z座標読み取り
					}
					else if (strcmp(&aString[0], "ROT") == 0)
					{
						fscanf(pFile, "%s", &aString[0]);  //イコール読み取り
						fscanf(pFile, "%f", &Rot.x);       //X座標読み取り
						fscanf(pFile, "%f", &Rot.y);       //Y座標読み取り
						fscanf(pFile, "%f", &Rot.z);       //Z座標読み取り
					}
					else if (strcmp(&aString[0], "WARPPOS") == 0)
					{
						fscanf(pFile, "%s", &aString[0]);  //イコール読み取り
						fscanf(pFile, "%f", &WarpPos.x);       //X座標読み取り
						fscanf(pFile, "%f", &WarpPos.y);       //Y座標読み取り
						fscanf(pFile, "%f", &WarpPos.z);       //Z座標読み取り
					}
					else if (strcmp(&aString[0], "WARPMAPINDEX") == 0)
					{
						fscanf(pFile, "%s", &aString[0]);  //イコール読み取り
						fscanf(pFile, "%d", &nWarpMap);
					}
					else if (strcmp(&aString[0], "SUBTYPE") == 0)
					{
						fscanf(pFile, "%s", &aString[0]);  //イコール読み取り
						fscanf(pFile, "%d", &nSubType);    //サブタイプ
					}
					else if (strcmp(&aString[0], "END_SETMARKER") == 0)
					{//Xオブジェクトを設定
						if (CScene::GetMode() == CScene::MODE_EDIT)
						{//エディットモードなら、オブジェクトをリストに保存し管理する
							pObject = CMarker::Create((CMarker::MARKERTYPE)(nType), pos, Scale, WarpPos, nWarpMap);

							if (pObject != nullptr)
							{
								pObject->SetStageManagerObj();                 //ステージマネージャーで管理するオブジェクトに設定	
								m_nIndexObject++;
							}
						}
						else if (CScene::GetMode() == CScene::MODE_GAME)
						{//ゲームモードなら、オブジェクトを生成するだけで、リストに保存しない
							int n = 0;
							CMarker::Create((CMarker::MARKERTYPE)(nType), pos, Scale, WarpPos, nWarpMap);
						}
						break;
					}
				}
			}
			else if (strcmp(&aString[0], "SETTRAP") == 0)
			{
			    LoadTrapTxt(pFile);
			}
			else if (strcmp(&aString[0], "SETENEMY") == 0)
			{
			    LoadEnemyTxt(pFile);
			}
			else if (strcmp(&aString[0], "SETBLOCK") == 0)
			{
			    LoadBlockTxt(pFile);
			}
			else if (strcmp(&aString[0], "END_SCRIPT") == 0)
			{
				break;
				fclose(pFile);
			}
		}
	}
}
//============================================================================================================

//===========================================
//マップのファイルパスをロードする
//===========================================
void CStageManager::LoadMapFilePass(WORLDTYPE type)
{
	char aString[512] = {};
	//===============================================================================================

	switch (type)
	{
	case WORLDTYPE00_EASY:
		m_pBg3D->SetBg3DType(CBg3D::BG3DTYPE_STAGE00);
		break;
	case WORLDTYPE01_NORMAL:
		m_pBg3D->SetBg3DType(CBg3D::BG3DTYPE_STAGE01);
		break;
	case WORLDTYPE02_BOSSRUSH:
		m_pBg3D->SetBg3DType(CBg3D::BG3DTYPE_STAGE00);
	default:
		break;
	}

	m_nMapNum = 0;//マップ総数を初期化

	FILE* pFileTxt = nullptr;
	pFileTxt = fopen(m_apWORLDMAP_TXT[type], "r");

	if (pFileTxt != nullptr)
	{
		while (1)
		{
			fscanf(pFileTxt, "%s", &aString[0]);
			if (aString[0] == '#')
			{
				fgets(&aString[0], 100, pFileTxt);//行をスキップ
			}
			else if (strcmp(&aString[0], "FILENAME") == 0)
			{
				fscanf(pFileTxt, "%s", &aString[0]);//イコール読み取り用
				fscanf(pFileTxt, "%s", &m_aMapFilePass[m_nMapNum][0]);//ファイル名読み取り用
				m_nMapNum++;
			}
			else if (strcmp(&aString[0], "END_SCRIPT") == 0)
			{
				fclose(pFileTxt);
				break;
			}
		}

	}

}
//============================================================================================================

//============================================
//マップをセーブする処理
//============================================
void CStageManager::SaveMapTxt(int nMapNum)
{
	D3DXVECTOR3 Pos = NULL_VECTOR3;
	D3DXVECTOR3 WarpPos = NULL_VECTOR3;
	D3DXVECTOR3 Scale = NULL_VECTOR3;
	D3DXVECTOR3 Rot = NULL_VECTOR3;
	int nWarpMap = 0;//ワープ先のマップ番号
	int nManagerType;//オブジェクトXのタイプ
	int nRotType; //向きの種類
	int nType = 0;//オブジェクトXごとのタイプ
	D3DXVECTOR3 move = NULL_VECTOR3;
	float fWidth = 0.0f;
	float fHeight = 0.0f;
	int nLife = 0;        //体力
	m_nMapIndex = nMapNum;
	int nSubType = 0;     //サブタイプ
	FILE* pFile = nullptr;
	CObject* pObj = nullptr;//オブジェクト取得用

	//=================================================
	//現在のモードによってセーブするファイルを変える
	//=================================================
	if (m_ManagerMode == MANAGERMODE_ALREADYSTAGE)
	{
		pFile = fopen(&m_aMapFilePass[m_nMapIndex][0], "w");
	}
	else
	{
		pFile = fopen(SAVE_TXT, "w");
	}
	//=====================================================================================================================

	if (pFile != nullptr)
	{
		fprintf(pFile, "SETCAMERATYPE = %d\n\n",m_nMapCameraType);//カメラの種類を設定

		pObj = GetTopStageManagerObject();//トップオブジェクトを取得

		while (pObj != nullptr)
		{
			//次のオブジェクトを格納
			CObject* pNext = pObj->GetNextStageManagerObject();

			//種類の取得（敵なら当たり判定）
			CObject::TYPE type = pObj->GetType();

			//==============================================
			//オブジェクト情報を書き出す
			//==============================================
			nLife = ((CObjectX*)pObj)->GetLife();
			nManagerType = ((CObjectX*)pObj)->GetManagerType(); //Xオブジェクトのタイプを取得
			nType = ((CObjectX*)pObj)->GetTypeNum();            //Xオブジェクトごとのタイプを取得
			nRotType = ((CObjectX*)pObj)->GetRotType();         //向きの種類を取得する
			Pos = ((CObjectX*)pObj)->GetSupportPos();           //位置を書き出す
			Scale = ((CObjectX*)pObj)->GetScale();				//拡大率を書き出す
			Rot = ((CObjectX*)pObj)->GetRot();					//向きを書き出す

			//サブタイプ取得
			switch (nManagerType)
			{
			case MANAGEROBJECT_BLOCK:
				nSubType = ((CBlock*)pObj)->GetSubType();
				break;
			case MANAGEROBJECT_TRAP:
				nSubType = ((CTrap*)pObj)->GetSubType();
				break;
			case MANAGEROBJECT_SIGNBOARD:
				nSubType = ((CSignBoard*)pObj)->GetSubType();
				break;
			default:
				nSubType = 0;
				break;
			}

			if (nManagerType == MANAGEROBJECT_MARKER)
			{
				WarpPos = ((CMarker*)pObj)->GetWarpPos();
				nWarpMap = ((CMarker*)pObj)->GetWarpMap();

				fprintf(pFile, "SETMARKER\n");
				fprintf(pFile, "MANAGERTYPE = %d\n", nManagerType);
				fprintf(pFile, "TYPE = %d\n", nType);
				fprintf(pFile, "POS = %.3f %.3f %.3f\n", Pos.x, Pos.y, Pos.z);
				fprintf(pFile, "SCALE = %.3f %.3f %.3f\n", Scale.x, Scale.y, Scale.z);
				fprintf(pFile, "ROT = %.3f %.3f %.3f\n", Rot.x, Rot.y, Rot.z);
				fprintf(pFile, "WARPPOS = %.3f %.3f %.3f\n", WarpPos.x, WarpPos.y, WarpPos.z);
				fprintf(pFile, "WARPMAPINDEX = %d\n", nWarpMap);
				fprintf(pFile, "SUBTYPE = %d\n", nSubType);
				fprintf(pFile, "END_SETMARKER\n\n");
			}
			else if (nManagerType == MANAGEROBJECT_TRAP)
			{//トラップ情報を保存
				SaveTrapTxt(pFile, pObj);
			}
			else if (nManagerType == MANAGEROBJECT_BLOCK)
			{//ブロック情報を保存
				SaveBlockTxt(pFile, pObj);
			}
			else if (nManagerType == MANAGEROBJECT_ENEMY)
			{
				SaveEnemyTxt(pFile, pObj);
			}
			else
			{//特に特殊効果がない場合
				fprintf(pFile, "SETOBJECT\n");
				fprintf(pFile, "MANAGERTYPE = %d\n", nManagerType);
				fprintf(pFile, "TYPE = %d\n", nType);
				fprintf(pFile, "ROTTYPE = %d\n", nRotType);
				fprintf(pFile, "POS = %.3f %.3f %.3f\n", Pos.x, Pos.y, Pos.z);
				fprintf(pFile, "SCALE = %.3f %.3f %.3f\n", Scale.x, Scale.y, Scale.z);
				fprintf(pFile, "ROT = %.3f %.3f %.3f\n", Rot.x, Rot.y, Rot.z);
				fprintf(pFile, "LIFE = %d\n", nLife);
				fprintf(pFile, "SUBTYPE = %d\n", nSubType);
				fprintf(pFile, "END_SETOBJECT\n\n");
			}

			pObj = pNext;//オブジェクトを次に進める
		}
		//========================================================================
		fprintf(pFile, "END_SCRIPT");
		fclose(pFile);
	}
	//================================================================================

}
//======================================================================================================================

//==========================================================
//マップをバイナリファイルから読み込む
//==========================================================
void CStageManager::LoadMapBin(int nMapNum)
{
	//======================================
	//変数宣言
	//======================================
	D3DXVECTOR3 Pos = NULL_VECTOR3;
	D3DXVECTOR3 Scale = NULL_VECTOR3;
	D3DXVECTOR3 Rot = NULL_VECTOR3;
	int nType = 0;            //オブジェクトXごとのタイプ
	int nManagerType = 0;        //オブジェクトXのタイプ
	int nRotType = 0;         //向きのタイプ
	D3DXVECTOR3 move = NULL_VECTOR3;
	float fWidth = 0.0f;
	float fHeight = 0.0f;
	int nData = 0;
	char aString[512] = {};
	char aStringMap[100] = {};
	CObject* pObject = nullptr;
	//===============================================================================================
	StageManagerObjectReleaseAll();
	FILE* pFile = nullptr;
	pFile = fopen(&m_aMapFilePass[nMapNum][0], "rb");
	//pFile = fopen(&m_aMapFilePass[0][0], "rb");
	if (pFile != nullptr)
	{
		//==============================================
		//オブジェクトの情報を読み込む
		//==============================================
		fread(&m_nIndexObject, sizeof(int), 1, pFile);
		if (m_nIndexObject > 0)
		{
			for (int nCnt = 0; nCnt < m_nIndexObject; nCnt++)
			{//オブジェクトを設定
				fread(&nManagerType, sizeof(int), 1, pFile);         //Xオブジェクトのタイプを読み込む
				fread(&nType, sizeof(int), 1, pFile);             //Xオブジェクトごとのタイプを読み込む
				fread(&nRotType, sizeof(int), 1, pFile);          //向きの種類を読み込む
				fread(&Pos, sizeof(D3DXVECTOR3), 1, pFile);       //位置を読み込む
				fread(&Scale, sizeof(D3DXVECTOR3), 1, pFile);     //拡大率を読み込む
				fread(&Rot, sizeof(D3DXVECTOR3), 1, pFile);       //向きを読み込む

				switch (nManagerType)
				{
					case (int)(MANAGEROBJECT_BLOCK) :
						pObject = CBlock::Create(CBlock::BLOCKTYPE(nType), 10, Pos, Rot, Scale, (CObjectX::ROTTYPE)(nRotType), m_nSetSubType);//生成処理
						break;
						case (int)(MANAGEROBJECT_MODEL) :
							pObject = CModel::Create((CModel::MODELTYPE)(nType), Pos, Rot, NULL_VECTOR3, Scale, (CObjectX::ROTTYPE)(nRotType));//生成処理
							break;
								case (int)(MANAGEROBJECT_ENEMY) :
									pObject = CEnemy::Create((CEnemy::ENEMYTYPE)(nType), 10, Pos, NULL_VECTOR3, Scale,NULL_VECTOR3,NULL_VECTOR3,ONE_VECTOR3);//生成処理
									break;
									case (int)(MANAGEROBJECT_ITEM) :
										pObject = CItem::Create((CItem::ITEMTYPE)(nType), Pos, Rot, Scale, CObjectX::ROTTYPE_NORMAL);//生成処理
										break;
										case (int)(MANAGEROBJECT_MARKER) :
											pObject = CMarker::Create((CMarker::MARKERTYPE)(nType),Pos,Scale, NULL_VECTOR3, 0);
											break;
											case (int)(MANAGEROBJECT_BOSS) :
												pObject = CBoss::SetCreateBoss((CBoss::BOSSTYPE)(nType),10, Pos, Scale);
												break;


								default:
									break;
				}

				if (pObject != nullptr)
				{
					m_apObjectList[nCnt] = pObject;
				}
			}
		}
		//========================================================================
		fclose(pFile);
	}
}
	//======================================================================================================================

//==========================================================
//マップをバイナリファイルに書き出す
//==========================================================
void CStageManager::SaveMapBin()
{
	//======================================
	//変数宣言
	//======================================
	D3DXVECTOR3 Pos = NULL_VECTOR3;
	D3DXVECTOR3 Scale = NULL_VECTOR3;
	D3DXVECTOR3 Rot = NULL_VECTOR3;
	int nType = 0;             //オブジェクトXごとのタイプ
	int nManagerType = 0;             //オブジェクトXのタイプ
	int nRotType = 0;              //オブジェクトXの向きのタイプ
	D3DXVECTOR3 move = NULL_VECTOR3;
	float fWidth = 0.0f;
	float fHeight = 0.0f;
	FILE* pFile = fopen(SAVE_BIN, "wb");
	//===============================================================================================
	if (pFile != nullptr)
	{

		fwrite(&m_nIndexObject, sizeof(int),1, pFile);
		//==============================================
        //ブロックのXオブジェクトの情報を書き出す
        //==============================================
		for (int nCnt = 0; nCnt < m_nIndexObject; nCnt++)
		{//ブロックを設定
			if (m_apObjectList[nCnt] != nullptr)
			{
				nManagerType = ((CObjectX*)m_apObjectList[nCnt])->GetManagerType();  //Xオブジェクトのタイプを取得
				nType = (int)(((CObjectX*)m_apObjectList[nCnt])->GetTypeNum());       //Xオブジェクトごとのタイプを取得
				nRotType = ((CObjectX*)m_apObjectList[nCnt])->GetRotType();                //向きの種類を取得する
				Pos = ((CObjectX*)m_apObjectList[nCnt])->GetPos();                    //位置を書き出す
				Scale = ((CObjectX*)m_apObjectList[nCnt])->GetScale();				//拡大率を書き出す
				Rot = ((CObjectX*)m_apObjectList[nCnt])->GetRot();					//向きを書き出す

				fwrite(&nManagerType, sizeof(int), 1, pFile);              //Xオブジェクトのタイプを書き出す
				fwrite(&nType, sizeof(int), 1, pFile);                  //Xオブジェクトごとのタイプを書き出す
				fwrite(&nRotType, sizeof(int), 1, pFile);               //向きの種類を書き出す
				fwrite(&Pos, sizeof(D3DXVECTOR3), 1, pFile);            //位置を書き出す
				fwrite(&Scale, sizeof(D3DXVECTOR3), 1, pFile);          //拡大率を書き出す
				fwrite(&Rot, sizeof(D3DXVECTOR3), 1, pFile);            //向きを書き出す
			}
		}
		//========================================================================
		fclose(pFile);
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
			pStageManager->CObject::SetType(CObject::TYPE_STAGEMANAGER);                                                     //オブジェクトの種類を決める   
			pStageManager->m_nType = 0;																	                     //前の位置
			pStageManager->m_nManagerType = MANAGEROBJECT_BLOCK;																					 //前の位置
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

//============================================
//ステージマネージャーを移動させる処理
//============================================
void CStageManager::MoveManager()
{
	//=========================================
	//変数宣言
	//=========================================
	
	float fSpeedX = float(rand() % 100 - 50) / 10;
	float fSpeedY = float(rand() % 100 - 50) / 10;
	float fSpeedZ = float(rand() % 100 - 50) / 10;

	float fRandRot = 0.0f;

	D3DXVECTOR3 MoveSize = ((CObjectX*)m_pManagerObject)->GetSize();//移動するサイズを求める

	CParticle::Create(CParticle::TYPE00_NORMAL, 45, 20.0f, 20.0f, m_WarpPos, D3DXVECTOR3(fSpeedX, fSpeedY, fSpeedZ), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f),true);
	CEffect::Create(CEffect::EFFECTTYPE00_NORMAL,30, 20.0f, 20.0f, m_WarpPos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard();//キーボードクラスの取得
	float fMoveX = 0.0f;                                          //X方向の移動量
	float fMoveZ = 0.0f;                                          //Z方向の移動量

	//=================================
	//移動タイプを切り替える
	//=================================
	if (pInputKeyboard->GetTrigger(DIK_MINUS) == true)
	{
		if (m_MoveMode == MOVEMODE00_XY)
		{
			m_MoveMode = MOVEMODE01_XZ;
		}
		else if (m_MoveMode == MOVEMODE01_XZ)
		{
			m_MoveMode = MOVEMODE00_XY;
		}
	}

	if (pInputKeyboard->GetTrigger(DIK_C) == true)
	{//サイズ分動かすかどうか
		m_bUseSizeMove = m_bUseSizeMove ? false : true;
	}

	//==================================================================================================

	//=================================
	//位置を戻す
	//=================================
	if (pInputKeyboard->GetTrigger(DIK_PERIOD))
	{
		m_Pos.z = 0.0f;
		m_Pos.y = 0.0f;
	}
	//==================================================================================================

	//=================================
	//移動方向を決める
	//=================================
	if (pInputKeyboard->GetPress(DIK_UP) == true)
	{
		fMoveZ = 1.0f;
	}
	else if (pInputKeyboard->GetPress(DIK_DOWN) == true)
	{
		fMoveZ = -1.0f;
	}

	if (pInputKeyboard->GetPress(DIK_RIGHT) == true)
	{
		fMoveX = 1.0f;
	}
	else if (pInputKeyboard->GetPress(DIK_LEFT) == true)
	{
		fMoveX = -1.0f;
	}
	//==================================================================================================

	//============================
	// 移動ボタンを押していたら
	//============================
	if (fMoveX != 0.0f || fMoveZ != 0.0f)
	{
		m_bMove = true;//移動状態
	}
	else
	{
		m_bMove = false;//待機状態
		m_Move = NULL_VECTOR3;
	}


	if (m_bMove == true)
	{//移動していたら
		if (m_bUseSizeMove == true)
		{
			m_Move = SizeMoveProcess(fMoveX, fMoveZ, MoveSize);
		}
		else
		{
			m_Move = NotSizeMoveProcess(fMoveX, fMoveZ);
		}
	}
	//==================================================================================================

	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{
		//ワープ位置移動処理
		m_WarpPos += m_Move;
		m_FocusType = FOCUSTYPE_WARPPOS;//ワープ先の位置に集中する
	}
	else
	{   //マネージャー移動処理
		m_Pos += m_Move;
		m_FocusType = FOCUSTYPE_NORMAL;//オブジェクトの位置に集中する
	    m_Pos.x = (float)(floor(m_Pos.x));
		m_Pos.y = (float)(floor(m_Pos.y));
		m_Pos.z = (float)(floor(m_Pos.z));

		fRandRot = CCalculation::CalculationRandVecXY();//乱数ベクトルを取得する

		//現在の位置にパーティクルを召喚する
		CParticle::Create(CParticle::TYPE00_NORMAL, 30, 10.0f, 10.0f, m_Pos, D3DXVECTOR3(sinf(fRandRot) * 5.0f, cosf(fRandRot) * 5.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), true);

	    //位置を設定する
	    ((CObjectX*)m_pManagerObject)->SetPos(m_Pos);
	}

}
//===========================================================================================

//=================================================
//体力を設定する
//=================================================
void CStageManager::LifeSet()
{
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_F8) == true)
	{
		m_nSetLife += 10;
	}
	else if (CManager::GetInputKeyboard()->GetTrigger(DIK_F7) == true)
	{
		m_nSetLife -= 10;
	}
}
//===========================================================================================

//=================================================
//選択ブロックの種類を変える
//=================================================
void CStageManager::TypeChenge()
{
	//=========================================
    //変数宣言
    //=========================================

	//それぞれのオブジェクトタイプのファイル数を取得する
	int nNumFile[MANAGEROBJECT_MAX] = { CBlock::GetNumFile(),
		CModel::GetNumFile(),
	    CEnemy::GetNumFile(),
	    CItem::GetNumFile(),
	    CMarker::GetNumFile(),
	    CBoss::GetNumFile(),
	    CTrap::GetNumFile(),
	    CSignBoard::GetNumFile()};

	const int* pNumSubType = nullptr;//それぞれのオブジェクトのサブタイプを取得する

	//=====================================================================
    //オブジェクトXの種類を移動する
    //=====================================================================
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_2) == true)
	{
		m_nManagerType++;
		m_nSetSubType = 0;//サブタイプ番号をリセット
		if (m_nManagerType >= (int)(MANAGEROBJECT_MAX))
		{
			m_nManagerType = 0;
		}
		m_nType = 0;//オブジェクト別タイプ番号を０に戻す
		ChengeObject();//オブジェクトの種類を変える
	}
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_1) == true)
	{
		m_nManagerType--;
		m_nSetSubType = 0;//サブタイプ番号をリセット
		if (m_nManagerType < 0)
		{
			m_nManagerType = (int)(MANAGEROBJECT_MAX) - 1;
		}
		m_nType = 0;//オブジェクト別タイプ番号を０に戻す
		ChengeObject();//オブジェクトの種類を変える
	}
	//=====================================================================================================================================

	//=====================================================================
	//オブジェクトごとの種類を移動する
	//=====================================================================
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_0) == true)
	{
		m_nType++;
		if (m_nType >= nNumFile[m_nManagerType])
		{
			m_nType = 0;
		}
		m_nSetSubType = 0;//サブタイプ番号をリセット
		ChengeObject();//オブジェクトの種類を変える
	}
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_9) == true)
	{
		m_nType--;
		m_nSetSubType = 0;//サブタイプ番号をリセット
		if (m_nType < 0)
		{
			m_nType = nNumFile[m_nManagerType] - 1;
		}
		ChengeObject();//オブジェクトの種類を変える
	}
	//=====================================================================================================================================

	//=====================================================================
	//サブタイプを設定する
	//=====================================================================

	//サブタイプ取得
	switch (m_nManagerType)
	{
	case MANAGEROBJECT_BLOCK:
		pNumSubType = CBlock::GetSubTypeNum();
		break;
	case MANAGEROBJECT_TRAP:
		pNumSubType = CTrap::GetSubTypeNum();
		break;
	case MANAGEROBJECT_SIGNBOARD:
		pNumSubType = CSignBoard::GetSubTypeNum();
		break;
	default:
		pNumSubType = nullptr;
		m_nSetSubType = 0;//サブタイプ番号をリセット
		break;
	}

	if (pNumSubType != nullptr)
	{//サブタイプを設定する
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_F9) == true)
		{
			m_nSetSubType++;
			if (m_nSetSubType >= pNumSubType[m_nType])
			{
				m_nSetSubType = 0;
			}
		}
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_F10) == true)
		{
			m_nSetSubType--;
			if (m_nSetSubType < 0)
			{
				m_nSetSubType = nNumFile[m_nType] - 1;
			}
		}
	}


	//=====================================================================================================================================

	//=====================================================================
	//拡大率を変える
	//=====================================================================
	if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
	{//Y軸縮小
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_R) == true)
		{
			m_Scale.x -= 1.0f;
		}
	}
	else
	{//Y軸拡大
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_R) == true)
		{
			m_Scale.x += 1.0f;
		}
	}

	if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
	{//Y軸縮小
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_T) == true)
		{
			m_Scale.y -= 1.0f;
		}
	}
	else
	{//Y軸拡大
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_T) == true)
		{
			m_Scale.y += 1.0f;
		}
	}
	if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
	{//Z軸縮小
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_Y) == true)
		{
			m_Scale.z -= 1.0f;
		}
	}
	else
	{//Z軸拡大
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_Y) == true)
		{
			m_Scale.z += 1.0f;
		}
	}
	if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
	{//等比縮小
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_U) == true)
		{
			m_Scale -= ONE_VECTOR3;
		}
	}
	else
	{//等比拡大
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_U) == true)
		{
			m_Scale += ONE_VECTOR3;
		}
	}

	((CObjectX*)m_pManagerObject)->SetScale(m_Scale);
	//=====================================================================================================================================
	
	//======================================================================
	//向きの種類を変える
	//======================================================================
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_6) == true)
	{
		if (m_nRotType == (int)(CObjectX::ROTTYPE_NORMAL))
		{
			m_nRotType = (int)(CObjectX::ROTTYPE_Z);
		}
		else
		{
			m_nRotType = (int)(CObjectX::ROTTYPE_NORMAL);
		}
	}
	((CObjectX*)m_pManagerObject)->SetRotType((CObjectX::ROTTYPE)(m_nRotType));
    //=======================================================================================================================================

	//=======================================================================
	//ワープ先マップのインデックスを変える
	//=======================================================================
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_F6) == true)
	{
		m_nWarpMapNum++;
		if (m_nWarpMapNum >= m_nMapNum)
		{
			m_nWarpMapNum = 0;
		}
	}
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_F5) == true)
	{
		m_nWarpMapNum--;
		if (m_nWarpMapNum < 0)
		{
			m_nWarpMapNum = m_nMapNum - 1;
		}
	}
	//=======================================================================================================================================

	//=======================================================================
	//マップのカメラの種類を変える
	//=======================================================================
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_F11) == true)
	{
		m_nMapCameraType++;
		if (m_nMapCameraType >= (int)(CCamera::CAMERATYPE_MAX))
		{
			m_nMapCameraType = 0;
		}
	}
	//=======================================================================================================================================

}
//===========================================================================================

//==================================================
//Xオブジェクト設定処理
//==================================================
void CStageManager::SetObjectX()
{
	CObject* pObject = nullptr;
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) == true)
	{
		switch (m_nManagerType)
		{
			case (int)(MANAGEROBJECT_BLOCK) :
		    pObject = CBlock::Create((CBlock::BLOCKTYPE)(m_nType),m_nSetLife, m_Pos, NULL_VECTOR3, m_Scale, (CObjectX::ROTTYPE)(m_nRotType), m_nSetSubType);
		    break;
		    case (int)(MANAGEROBJECT_MODEL) :
		    pObject = CModel::Create((CModel::MODELTYPE)(m_nType),m_Pos, NULL_VECTOR3,NULL_VECTOR3,m_Scale,(CObjectX::ROTTYPE)(m_nRotType));
		    break;
			case (int)(MANAGEROBJECT_ENEMY) :
			pObject = CEnemy::Create((CEnemy::ENEMYTYPE)(m_nType), m_nSetLife,m_Pos,NULL_VECTOR3,m_Scale, m_Pos,NULL_VECTOR3,ONE_VECTOR3);
			break;
			case (int)(MANAGEROBJECT_ITEM) :
			pObject = CItem::Create((CItem::ITEMTYPE)(m_nType), m_Pos, NULL_VECTOR3, m_Scale, (CObjectX::ROTTYPE)(m_nRotType));//生成処理
			break;
			case (int)(MANAGEROBJECT_MARKER) :
			pObject = CMarker::Create((CMarker::MARKERTYPE)(m_nType),m_Pos,m_Scale, m_WarpPos, m_nWarpMapNum);
			break;
			case (int)(MANAGEROBJECT_BOSS) :
			pObject = CBoss::SetCreateBoss((CBoss::BOSSTYPE)(m_nType),m_nSetLife, m_Pos, m_Scale);
			break;
			case (int)(MANAGEROBJECT_TRAP) :
			pObject = CTrap::Create((CTrap::TRAPTYPE)(m_nType), m_Pos, NULL_VECTOR3, m_Scale, CObjectX::ROTTYPE_NORMAL,m_nSetSubType);//生成処理
			break;
			case (int)(MANAGEROBJECT_SIGNBOARD) :
				pObject = CSignBoard::Create(CSignBoard::SIGNBOARDTYPE(m_nType), CSignBoard::SIGNBOARDMANUALTYPE(m_nSetSubType),
					m_Pos, m_Scale, NULL_VECTOR3);
			break;
		    default:
		    break;
		}
		//if (pObject != nullptr && m_nIndexObject < m_nMAXOBJECT)
		//{//オブジェクト情報をリストに保存
		//	m_apObjectList[m_nIndexObject] = pObject;

		pObject->SetStageManagerObj();//ステージマネージャー管理オブジェクトに設定する
		m_nIndexObject++;//インデックスを増やす

		//}

	}

}
//===========================================================================================

//==================================================
//Xオブジェクト消去処理
//==================================================
void CStageManager::DeleteObjectX()
{
	//if (CManager::GetInputKeyboard()->GetTrigger(DIK_DELETE) == true)
	//{
	//	if (m_apObjectList[m_nIndexObject - 1] != nullptr && m_nIndexObject >= 1)
	//	{
	//		if (((CObjectX*)m_apObjectList[m_nIndexObject - 1])->GetObjXType() == CObjectX::OBJECTXTYPE_MARKER)
	//		{
	//			((CMarker*)m_apObjectList[m_nIndexObject - 1])->DeleteNum();
	//		}
	//		m_apObjectList[m_nIndexObject - 1]->SetDeath();    //ブロックリストの動的確保メモリを開放
	//		m_apObjectList[m_nIndexObject - 1] = nullptr;          //ブロックリストのポインタを初期化
	//		m_nIndexObject--;                                     //インデックスをデクリメント
	//	}
	//}
}
//=======================================================================================================================

//====================================================
//設置オブジェクトの種類を変える
//====================================================
void CStageManager::ChengeObject()
{
	//オブジェクトをリリースする
	ReleaseObject();

	switch (m_nManagerType)
	{
		case (int)(MANAGEROBJECT_BLOCK) :
			m_pManagerObject = CBlock::Create((CBlock::BLOCKTYPE)(m_nType), 10, m_Pos, NULL_VECTOR3, m_Scale, (CObjectX::ROTTYPE)(m_nRotType), m_nSetSubType);
			break;
			case (int)(MANAGEROBJECT_MODEL) :
				m_pManagerObject = CModel::Create((CModel::MODELTYPE)(m_nType), m_Pos, NULL_VECTOR3, NULL_VECTOR3, m_Scale, (CObjectX::ROTTYPE)(m_nRotType));
				break;
				case (int)(MANAGEROBJECT_ENEMY) :
					m_pManagerObject = CEnemy::Create((CEnemy::ENEMYTYPE)(m_nType), 10, m_Pos, NULL_VECTOR3, m_Scale, m_Pos, NULL_VECTOR3, ONE_VECTOR3);
					break;
					case (int)(MANAGEROBJECT_ITEM) :
						m_pManagerObject = CItem::Create((CItem::ITEMTYPE)(m_nType), m_Pos, NULL_VECTOR3, m_Scale, (CObjectX::ROTTYPE)(m_nRotType));//生成処理
						break;
						case (int)(MANAGEROBJECT_MARKER) :
							m_pManagerObject = CMarker::Create((CMarker::MARKERTYPE)(m_nType), m_Pos, m_Scale, NULL_VECTOR3, 0);
							break;
							case (int)(MANAGEROBJECT_BOSS) :
								m_pManagerObject = CBoss::SetCreateBoss((CBoss::BOSSTYPE)(m_nType), m_nSetLife, m_Pos, m_Scale);
								break;
								case (int)(MANAGEROBJECT_TRAP) :
									m_pManagerObject = CTrap::Create((CTrap::TRAPTYPE)(m_nType), m_Pos, NULL_VECTOR3, m_Scale, CObjectX::ROTTYPE_NORMAL, m_nSetSubType);//生成処理
									break;
									case (int)(MANAGEROBJECT_SIGNBOARD) :
										m_pManagerObject = CSignBoard::Create(CSignBoard::SIGNBOARDTYPE(m_nType), CSignBoard::SIGNBOARDMANUALTYPE(m_nSetSubType),
											m_Pos, m_Scale, NULL_VECTOR3);
										break;

									default:
										break;
	}
	if (m_pManagerObject != nullptr)
	{
		m_pManagerObject->SetUseDeath(false);
	}
}
//=======================================================================================================================

//==================================================================
//オブジェクトをリリースする
//==================================================================
void CStageManager::ReleaseObject()
{
	if (m_pManagerObject != nullptr)
	{
		m_pManagerObject->SetUseDeath(true);
		m_pManagerObject->SetDeath();
		m_pManagerObject = nullptr;
	}

}

//================================================================
//色合いを設定する
//================================================================
void CStageManager::ColorChenge()
{

}
//=======================================================================================================================

//===========================================================
//マップを変える
//===========================================================
void CStageManager::MapChenge()
{
	//===============================================================
	//モードを変える
	//===============================================================
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_F4) == true)
	{
		SaveMapTxt(m_nMapIndex);//現在のマップを保存する

		if (m_ManagerMode == MANAGERMODE_ALREADYSTAGE)
		{
			m_ManagerMode = MANAGERMODE_NEWSTAGE;
		}
		else
		{
			m_ManagerMode = MANAGERMODE_ALREADYSTAGE;
		}

		LoadMapTxt(m_nMapIndex);
	}
	//=======================================================================================================

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_F3) == true && m_ManagerMode == MANAGERMODE_ALREADYSTAGE)
	{
		SaveMapTxt(m_nMapIndex);//現在のマップ情報をセーブする
		m_nMapIndex++;
		if (m_nMapIndex >= m_nMapNum)
		{
			m_nMapIndex = 0;
		}

		LoadMapTxt(m_nMapIndex);//番号のマップを読み込む
	}
	else if (CManager::GetInputKeyboard()->GetTrigger(DIK_F2) == true && m_ManagerMode == MANAGERMODE_ALREADYSTAGE)
	{
		SaveMapTxt(m_nMapIndex);//現在のマップ情報をセーブする
		m_nMapIndex--;
		if (m_nMapIndex < 0)
		{
			m_nMapIndex = m_nMapNum - 1;
		}
		LoadMapTxt(m_nMapIndex);//番号のマップを読み込む
	}

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_P) == true && m_ManagerMode == MANAGERMODE_ALREADYSTAGE)
	{
		SaveMapTxt(m_nMapIndex);//現在のマップ情報をセーブする

		m_nMapIndex = 0;//ワールドを変更するのでマップ番号を初期化
		m_nMapNum = 0;  //マップ総数を初期化

		m_nWorldIndex++;//次のワールドを選択

		if (m_nWorldIndex == WORLDTYPE_MAX)
		{
			m_nWorldIndex = 0;
		}

		//マップ情報をロードする
		LoadMapFilePass(WORLDTYPE(m_nWorldIndex));

		//マップを読み込む
		LoadMapTxt(m_nMapIndex);//番号のマップを読み込む
	}

}
//=======================================================================================================================

//===========================================================
//情報表示処理
//===========================================================
void CStageManager::DispInfo()
{
	char aChooseString[100] = {};//選択状態かどうかを記述
	char aCameraTypeName[CCamera::CAMERATYPE_MAX][100] = {};
	char aMapModeString[100] = {};
	//マップのカメラモード名
	strcpy(&aCameraTypeName[CCamera::CAMERATYPE_BIRD][0], "サイドビュー");
	strcpy(&aCameraTypeName[CCamera::CAMERATYPE_BESIDE][0], "ビサイドビュー");
	strcpy(&aCameraTypeName[CCamera::CAMERATYPE_SIDE][0], "サイドビュー");
	strcpy(&aCameraTypeName[CCamera::CAMERATYPE_BOSSBATTLE][0], "ボスバトル");
	strcpy(&aCameraTypeName[CCamera::CAMERATYPE_BOSSDEFEAT][0], "ボス撃破");


	if (m_bChooseObject == false)
	{
		strcpy(&aChooseString[0], "選択していない");
	}
	else
	{
		strcpy(&aChooseString[0], "選択中・・・");
	}

	if (m_ManagerMode == MANAGERMODE_ALREADYSTAGE)
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
	CManager::GetDebugText()->PrintDebugText("ステージマネージャー管理オブジェクトの総数：%d\n",GetTotalStageManagerObjNum());
	CManager::GetDebugText()->PrintDebugText("現在のワールド：%s\n",&m_apWORLDMAP_TXT[m_nWorldIndex][0]);
	CManager::GetDebugText()->PrintDebugText("現在のマップ番号(F2、F3で変更）：%d\n", m_nMapIndex);
	CManager::GetDebugText()->PrintDebugText("現在のマップファイルパス：%s\n",&m_aMapFilePass[m_nMapIndex][0]);
	CManager::GetDebugText()->PrintDebugText("現在のワープ先（F5、F6で変更）：%d\n", m_nWarpMapNum);
	CManager::GetDebugText()->PrintDebugText("現在の体力（F7、F8で変更）：%d\n", m_nSetLife);
	CManager::GetDebugText()->PrintDebugText("現在のオブジェクトのサブタイプ（F9、F10で変更）：%d\n", m_nSetSubType);
	CManager::GetDebugText()->PrintDebugText("オブジェクトの位置： %f %f %f\n",m_Pos.x,m_Pos.y,m_Pos.z);
	CManager::GetDebugText()->PrintDebugText("オブジェクトの拡大率リセット：V\n");
	CManager::GetDebugText()->PrintDebugText("現在のワープ先の位置： %f %f %f\n",m_WarpPos.x, m_WarpPos.y, m_WarpPos.z);
	CManager::GetDebugText()->PrintDebugText("現在のマップモード（F4）：%s\n",&aMapModeString[0]);
	CManager::GetDebugText()->PrintDebugText("オブジェクトを選択（L）：%s\n",&aChooseString[0]);
	CManager::GetDebugText()->PrintDebugText("選択オブジェクト変更：（N、M）\n");
	CManager::GetDebugText()->PrintDebugText("選択オブジェクト消去：（DEL）\n");
	CManager::GetDebugText()->PrintDebugText("移動方法を変更：ALTキー\n");
	CManager::GetDebugText()->PrintDebugText("移動方向変更：（マイナスキー）\n");
	CManager::GetDebugText()->PrintDebugText("Z座標を原点に戻す：（ピリオドキー）\n");
	CManager::GetDebugText()->PrintDebugText("全管理オブジェクト消去：（LSHIFTキー）\n");
	CManager::GetDebugText()->PrintDebugText("//=================================\n");

}
//=======================================================================================================================

//===========================================================
//オブジェクト選択処理
//===========================================================
void CStageManager::ChooseObject()
{
	bool bChoose = false;                              //オブジェクトを選択しているかどうか
	int nChooseIdxObject = 0;                          //現在選択しているオブジェクトの番号

	if (GetTotalStageManagerObjNum() <= 0)
	{//ステージマネージャー管理オブジェクトがなくなった瞬間に選択状態を解除する。
		m_bChooseObject = false;
		m_pChooseObject = nullptr;
	}

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_L) == true && GetTotalStageManagerObjNum() > 0)
	{
		m_bChooseObject = m_bChooseObject ? false : true;

		if (m_bChooseObject == false)
		{
			m_pChooseObject->SetStageManagerChoose(false);     //選択しているオブジェクトの選択状態を解除
			m_Pos = m_SaveBeforeChoosePos;                     //保存していた位置に戻す
		}
		else
		{
			m_SaveBeforeChoosePos = m_Pos;                     //選択状態を発動する前のオブジェクトの位置を記憶する
			m_pChooseObject = GetCurStageManagerObject();      //最新のオブジェクトを選択
			m_pChooseObject->SetStageManagerChoose(false);     //選択しているオブジェクトの選択状態を解除
		}
	}
	
	if (m_pChooseObject != nullptr)
	{//選択オブジェクトが存在していたら
		if (m_bChooseObject == true)
		{
			//選択オブジェクト変更
			if (CManager::GetInputKeyboard()->GetTrigger(DIK_N) == true)
			{
				m_pChooseObject->SetStageManagerChoose(false);//現在選んでいるオブジェクトの選択状態を解除

				if (m_pChooseObject->GetPrevStageManagerObject() != nullptr)
				{//前のオブジェクトがあったら
					m_pChooseObject = m_pChooseObject->GetPrevStageManagerObject();//前のオブジェクトを選択
				}
				else
				{//前のオブジェクトがない
					m_pChooseObject = GetCurStageManagerObject();//最新オブジェクトを選択
				}
			}
			else if (CManager::GetInputKeyboard()->GetTrigger(DIK_M) == true)
			{
				m_pChooseObject->SetStageManagerChoose(false);//現在選んでいるオブジェクトの選択状態を解除

				if (m_pChooseObject->GetNextStageManagerObject() != nullptr)
				{//次のオブジェクトがある
					m_pChooseObject = m_pChooseObject->GetNextStageManagerObject();//次のオブジェクトを選択
				}
				else
				{//次のオブジェクトがない
					m_pChooseObject = GetTopStageManagerObject();//トップオブジェクトを選択
				}
			}

			if (m_pChooseObject != nullptr && m_nIndexObject >= 1)
			{//オブジェクトを選択状態にする
				m_Pos = ((CObjectX*)m_pChooseObject)->GetPos();//選択オブジェクトの位置に移動する
				m_pChooseObject->SetStageManagerChoose(true);
			}

			if (CManager::GetInputKeyboard()->GetTrigger(DIK_DELETE) == true)
			{//選択オブジェクトを破棄する
				m_pChooseObject->SetUseDeath(true);
				m_pChooseObject->SetDeath();

				//選択オブジェクトを変更
				if (m_pChooseObject->GetNextStageManagerObject() != nullptr)
				{//次のオブジェクトがある
					m_pChooseObject = m_pChooseObject->GetNextStageManagerObject();//次のオブジェクトを選択
				}
				else
				{//次のオブジェクトがない
					m_pChooseObject = GetTopStageManagerObject();//トップオブジェクトを選択
				}
			}
		}
	}
}
//=======================================================================================================================

//===========================================================
//トラップをテキストファイルに保存する
//===========================================================
void CStageManager::SaveTrapTxt(FILE* pFile, CObject* pObj)
{
	D3DXVECTOR3 Pos = NULL_VECTOR3;
	D3DXVECTOR3 WarpPos = NULL_VECTOR3;
	D3DXVECTOR3 Scale = NULL_VECTOR3;
	D3DXVECTOR3 Rot = NULL_VECTOR3;
	int nType = ((CObjectX*)pObj)->GetTypeNum();          //Xオブジェクトごとのタイプを取得
	int nSubType = ((CTrap*)pObj)->GetSubType();          //サブタイプ

	float& fSpeed = ((CTrap*)pObj)->GetSpeed();           //速さ
	float& fRotSpeed = ((CTrap*)pObj)->GetRotSpeed();     //回転速度
	float& fStartRot = ((CTrap*)pObj)->GetStartRot();     //回転開始向き
	float& fMoveLength = ((CTrap*)pObj)->GetMoveLength(); //移動距離
	float& fTurnSpeed = ((CTrap*)pObj)->GetTurnSpeed();   //Uターン開始速度
	int& nInterval = ((CTrap*)pObj)->GetInterval();       //様々なインターバル
	float& fAddSpeed = ((CTrap*)pObj)->GetAddSpeed();     //加速度を取得
	Pos = ((CObjectX*)pObj)->GetSupportPos();                    //位置を書き出す
	Scale = ((CObjectX*)pObj)->GetScale();				//拡大率を書き出す
	Rot = ((CObjectX*)pObj)->GetRot();					//向きを書き出す

	fprintf(pFile, "SETTRAP\n");
	fprintf(pFile, "TYPE = %d\n", nType);
	fprintf(pFile, "SUBTYPE = %d\n", nSubType);
	fprintf(pFile, "POS = %.3f %.3f %.3f\n", Pos.x, Pos.y, Pos.z);
	fprintf(pFile, "SCALE = %.3f %.3f %.3f\n", Scale.x, Scale.y, Scale.z);
	fprintf(pFile, "ROT = %.3f %.3f %.3f\n", Rot.x, Rot.y, Rot.z);
	fprintf(pFile, "SPEED = %.3f\n",fSpeed);
	fprintf(pFile, "ROTSPEED = %.3f\n",fRotSpeed);
	fprintf(pFile, "STARTROT = %.3f\n",fStartRot);
	fprintf(pFile, "MOVELENGTH = %.3f\n",fMoveLength);
	fprintf(pFile, "TURNSPEED = %.3f\n",fTurnSpeed);
	fprintf(pFile, "INTERVAL = %d\n", nInterval);
	fprintf(pFile, "ADDSPEED = %f\n", fAddSpeed);
	fprintf(pFile, "END_SETTRAP\n\n");

}
//=======================================================================================================================

//===========================================================
//ブロックをテキストファイルに保存する
//===========================================================
void CStageManager::SaveBlockTxt(FILE* pFile, CObject* pObj)
{
	D3DXVECTOR3 Pos = ((CObjectX*)pObj)->GetSupportPos(); //支点
	D3DXVECTOR3 Scale = ((CObjectX*)pObj)->GetScale();    //拡大率
	D3DXVECTOR3 Rot = ((CObjectX*)pObj)->GetRot();        //向き
	int nType = ((CObjectX*)pObj)->GetTypeNum();          //Xオブジェクトごとのタイプを取得
	int nSubType = ((CBlock*)pObj)->GetSubType();         //サブタイプ

	float fStartRot = ((CBlock*)pObj)->GetStartRot();     //回転開始位置
	float fRotSpeed = ((CBlock*)pObj)->GetRotSpeed();     //回転速度
	float fMoveLength = ((CBlock*)pObj)->GetMoveLength(); //移動距離
	float fSpeed = ((CBlock*)pObj)->GetSpeed();           //移動速度
	int nBlinkingSpeed = ((CBlock*)pObj)->GetBlinkSpeed();//点滅速度
	float fAddSpeed = ((CBlock*)pObj)->GetAddSpeed();     //加速度
	float fJumpPower = ((CBlock*)pObj)->GetJumpPower();   //ジャンプ力

	fprintf(pFile, "SETBLOCK\n");
	fprintf(pFile, "TYPE = %d\n", nType);
	fprintf(pFile, "SUBTYPE = %d\n", nSubType);
	fprintf(pFile, "POS = %.3f %.3f %.3f\n", Pos.x, Pos.y, Pos.z);
	fprintf(pFile, "SCALE = %.3f %.3f %.3f\n", Scale.x, Scale.y, Scale.z);
	fprintf(pFile, "ROT = %.3f %.3f %.3f\n", Rot.x, Rot.y, Rot.z);

	fprintf(pFile, "STARTROT = %.3f\n", fStartRot);
	fprintf(pFile, "ROTSPEED = %.3f\n", fRotSpeed);
	fprintf(pFile, "MOVELENGTH = %.3f\n", fMoveLength);
	fprintf(pFile, "SPEED = %.3f\n", fSpeed);
	fprintf(pFile, "BLINKINGSPEED = %d\n", nBlinkingSpeed);
	fprintf(pFile, "ADDSPEED = %.3f\n", fAddSpeed);
	fprintf(pFile, "JUMPPOWER = %.3f\n", fJumpPower);
	fprintf(pFile, "END_SETBLOCK\n\n");

}
//=======================================================================================================================

//===========================================================
//敵の情報をテキストファイルに書き出す
//===========================================================
void CStageManager::SaveEnemyTxt(FILE* pFile, CObject* pObj)
{
	D3DXVECTOR3 Pos = ((CObjectX*)pObj)->GetSupportPos();           //位置;
	D3DXVECTOR3 Scale = ((CObjectX*)pObj)->GetScale();				//拡大率;
	D3DXVECTOR3 Rot = ((CObjectX*)pObj)->GetRot();					//向き;
	D3DXVECTOR3 MoveRangeAIPos = ((CEnemy*)pObj)->GetEnemyMoveRangeAIModel()->GetPos();//移動範囲AIの位置
	D3DXVECTOR3 MoveRangeAIScale = ((CEnemy*)pObj)->GetEnemyMoveRangeAIModel()->GetScale();//移動範囲AIの拡大率
	int nType = ((CObjectX*)pObj)->GetTypeNum();        //Xオブジェクトごとのタイプを取得
	int nLife = ((CObjectX*)pObj)->GetLife();           //体力


	fprintf(pFile, "SETENEMY\n");
	fprintf(pFile, "TYPE = %d\n", nType);
	fprintf(pFile, "LIFE = %d\n", nLife);
	fprintf(pFile, "POS = %.3f %.3f %.3f\n", Pos.x, Pos.y, Pos.z);
	fprintf(pFile, "SCALE = %.3f %.3f %.3f\n", Scale.x, Scale.y, Scale.z);
	fprintf(pFile, "ROT = %.3f %.3f %.3f\n", Rot.x, Rot.y, Rot.z);
	fprintf(pFile, "MOVERANGEAIPOS = %.3f %.3f %.3f\n", MoveRangeAIPos.x, MoveRangeAIPos.y, MoveRangeAIPos.z);
	fprintf(pFile, "MOVERANGEAISCALE = %.3f %.3f %.3f\n", MoveRangeAIScale.x, MoveRangeAIScale.y, MoveRangeAIScale.z);
	fprintf(pFile, "END_SETENEMY\n\n");
}
//=======================================================================================================================

//===========================================================
//トラップの情報をテキストファイルから読み込む
//===========================================================
void CStageManager::LoadTrapTxt(FILE* pFile)
{
	D3DXVECTOR3 Pos = NULL_VECTOR3;   //位置
	D3DXVECTOR3 Scale = NULL_VECTOR3; //拡大率
	D3DXVECTOR3 Rot = NULL_VECTOR3;   //向き
	int nType = 0;                    //Xオブジェクトごとのタイプを取得
	int nSubType = 0;                 //サブタイプ
	float fSpeed = 0.0f;              //速さ
	float fRotSpeed = 0.0f;           //回転速度
	float fStartRot = 0.0f;           //回転開始向き
	float fMoveLength = 0.0f;         //移動距離
	float fTurnSpeed = 0.0f;          //Uターン開始速度
	float fAddSpeed = 0.0f;           //加速度
	int nInterval = 0;                //インターバル
	char aString[128] = {};           //文字列
	CObject* pObject = nullptr;       //オブジェクトへのポインタ

	while (1)
	{
		fscanf(pFile, "%s", &aString[0]);
		if (aString[0] == '#')
		{
			fgets(&aString[0], 100, pFile);//行をスキップ
		}
		else if (strcmp(&aString[0], "TYPE") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);  //イコール読み取り
			fscanf(pFile, "%d", &nType);       //オブジェクトごとのタイプ取得
		}
		else if (strcmp(&aString[0], "SUBTYPE") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);  //イコール読み取り
			fscanf(pFile, "%d", &nSubType);    //サブタイプ
		}
		else if (strcmp(&aString[0], "POS") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);  //イコール読み取り
			fscanf(pFile, "%f", &Pos.x);       //X座標読み取り
			fscanf(pFile, "%f", &Pos.y);       //Y座標読み取り
			fscanf(pFile, "%f", &Pos.z);       //Z座標読み取り
		}
		else if (strcmp(&aString[0], "SCALE") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);  //イコール読み取り
			fscanf(pFile, "%f", &Scale.x);       //X座標読み取り
			fscanf(pFile, "%f", &Scale.y);       //Y座標読み取り
			fscanf(pFile, "%f", &Scale.z);       //Z座標読み取り
		}
		else if (strcmp(&aString[0], "ROT") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);  //イコール読み取り
			fscanf(pFile, "%f", &Rot.x);       //X座標読み取り
			fscanf(pFile, "%f", &Rot.y);       //Y座標読み取り
			fscanf(pFile, "%f", &Rot.z);       //Z座標読み取り
		}
		else if (strcmp(&aString[0], "SPEED") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);  //イコール読み取り
			fscanf(pFile, "%f", &fSpeed);      //移動速度読み取り
		}
		else if (strcmp(&aString[0], "ROTSPEED") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);  //イコール読み取り
			fscanf(pFile, "%f", &fRotSpeed);   //回転速度読み取り
		}
		else if (strcmp(&aString[0], "STARTROT") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);  //イコール読み取り
			fscanf(pFile, "%f", &fStartRot);   //回転開始向き読み取り
		}
		else if (strcmp(&aString[0], "MOVELENGTH") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);  //イコール読み取り
			fscanf(pFile, "%f", &fMoveLength); //移動距離読み取り
		}
		else if (strcmp(&aString[0], "TURNSPEED") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);  //イコール読み取り
			fscanf(pFile, "%f", &fTurnSpeed);  //Uターン開始速度読み取り
		}
		else if (strcmp(&aString[0], "INTERVAL") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);  //イコール読み取り
			fscanf(pFile, "%d", &nInterval);   //インターバル読み取り
		}
		else if (strcmp(&aString[0], "ADDSPEED") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);  //イコール読み取り
			fscanf(pFile, "%f", &fAddSpeed);   //インターバル読み取り
		}
		else if (strcmp(&aString[0], "END_SETTRAP") == 0)
		{//Xオブジェクトを設定
			if (CScene::GetMode() == CScene::MODE_EDIT)
			{//エディットモードなら、オブジェクトをリストに保存し管理する
				pObject = CTrap::Create((CTrap::TRAPTYPE)(nType), Pos, Rot, Scale, 0, nSubType);
				((CTrap*)pObject)->SetInfo(fStartRot, fSpeed, fRotSpeed, fMoveLength, fTurnSpeed,nInterval,fAddSpeed);//情報設定
				if (pObject != nullptr)
				{
					pObject->SetStageManagerObj();                                                 //ステージマネージャーで管理するオブジェクトに設定	
					m_nIndexObject++;
				}
			}
			else if (CScene::GetMode() == CScene::MODE_GAME)
			{//ゲームモードなら、オブジェクトを生成するだけで、リストに保存しない
				pObject = CTrap::Create((CTrap::TRAPTYPE)(nType), Pos, Rot, Scale, 0, nSubType);
				((CTrap*)pObject)->SetInfo(fStartRot, fSpeed, fRotSpeed, fMoveLength, fTurnSpeed,nInterval, fAddSpeed);//情報設定
			}
			break;
		}
	}

}
//=======================================================================================================================

//===========================================================
//トラップの情報をテキストファイルから読み込む
//===========================================================
void CStageManager::LoadBlockTxt(FILE* pFile)
{
	D3DXVECTOR3 Pos = NULL_VECTOR3;  //支点
	D3DXVECTOR3 Scale = NULL_VECTOR3;//拡大率
	D3DXVECTOR3 Rot = NULL_VECTOR3;  //向き
	int nType = 0;                   //Xオブジェクトごとのタイプを取得
	int nSubType = 0;                //サブタイプ

	float fStartRot = 0.0f;          //回転開始位置
	float fRotSpeed = 0.0f;          //回転速度
	float fMoveLength = 0.0f;        //移動距離
	float fSpeed = 0.0f;             //移動速度
	int nBlinkingSpeed = 0;          //点滅速度
	float fAddSpeed = 0.0f;          //加速度
	float fJumpPower = 0.0f;         //ジャンプ力

	char aString[128] = {};           //文字列
	CObject* pObject = nullptr;       //オブジェクトへのポインタ

	while (1)
	{
		fscanf(pFile, "%s", &aString[0]);
		if (aString[0] == '#')
		{
			fgets(&aString[0], 100, pFile);//行をスキップ
		}
		else if (strcmp(&aString[0], "TYPE") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);  //イコール読み取り
			fscanf(pFile, "%d", &nType);       //オブジェクトごとのタイプ取得
		}
		else if (strcmp(&aString[0], "SUBTYPE") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);  //イコール読み取り
			fscanf(pFile, "%d", &nSubType);    //サブタイプ
		}
		else if (strcmp(&aString[0], "POS") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);  //イコール読み取り
			fscanf(pFile, "%f", &Pos.x);       //X座標読み取り
			fscanf(pFile, "%f", &Pos.y);       //Y座標読み取り
			fscanf(pFile, "%f", &Pos.z);       //Z座標読み取り
		}
		else if (strcmp(&aString[0], "SCALE") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);  //イコール読み取り
			fscanf(pFile, "%f", &Scale.x);       //X座標読み取り
			fscanf(pFile, "%f", &Scale.y);       //Y座標読み取り
			fscanf(pFile, "%f", &Scale.z);       //Z座標読み取り
		}
		else if (strcmp(&aString[0], "ROT") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);  //イコール読み取り
			fscanf(pFile, "%f", &Rot.x);       //X座標読み取り
			fscanf(pFile, "%f", &Rot.y);       //Y座標読み取り
			fscanf(pFile, "%f", &Rot.z);       //Z座標読み取り
		}
		else if (strcmp(&aString[0], "STARTROT") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);  //イコール読み取り
			fscanf(pFile, "%f", &fStartRot);   //回転開始向き読み取り
		}
		else if (strcmp(&aString[0], "ROTSPEED") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);  //イコール読み取り
			fscanf(pFile, "%f", &fRotSpeed);   //回転速度読み取り
		}
		else if (strcmp(&aString[0], "MOVELENGTH") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);  //イコール読み取り
			fscanf(pFile, "%f", &fMoveLength); //移動距離読み取り
		}
		else if (strcmp(&aString[0], "SPEED") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);  //イコール読み取り
			fscanf(pFile, "%f", &fSpeed);      //移動速度読み取り
		}
		else if (strcmp(&aString[0], "BLINKINGSPEED") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);    //イコール読み取り
			fscanf(pFile, "%d", &nBlinkingSpeed);//点滅速度読み取り
		}
		else if (strcmp(&aString[0], "ADDSPEED") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);    //イコール読み取り
			fscanf(pFile, "%f", &fAddSpeed);     //加速度読み取り
		}
		else if (strcmp(&aString[0], "JUMPPOWER") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);    //イコール読み取り
			fscanf(pFile, "%f", &fJumpPower);     //ジャンプ力読み取り
		}
		else if (strcmp(&aString[0], "END_SETBLOCK") == 0)
		{//Xオブジェクトを設定
			if (CScene::GetMode() == CScene::MODE_EDIT)
			{//エディットモードなら、オブジェクトをリストに保存し管理する
				pObject = CBlock::Create((CBlock::BLOCKTYPE)(nType),10,Pos,Rot,Scale,CObjectX::ROTTYPE_NORMAL,nSubType);
				((CBlock*)pObject)->SetBlockInfo(fStartRot, fRotSpeed, fMoveLength, fSpeed, nBlinkingSpeed, fAddSpeed, fJumpPower);
				if (pObject != nullptr)
				{
					pObject->SetStageManagerObj();                                                 //ステージマネージャーで管理するオブジェクトに設定	
					m_nIndexObject++;
				}
			}
			else if (CScene::GetMode() == CScene::MODE_GAME)
			{//ゲームモードなら、オブジェクトを生成するだけで、リストに保存しない
				pObject = CBlock::Create((CBlock::BLOCKTYPE)(nType), 10, Pos, Rot, Scale, CObjectX::ROTTYPE_NORMAL, nSubType);
				((CBlock*)pObject)->SetBlockInfo(fStartRot, fRotSpeed, fMoveLength, fSpeed, nBlinkingSpeed, fAddSpeed, fJumpPower);
			}
			break;
		}
	}

}
//=======================================================================================================================

//===========================================================
//敵の情報をテキストファイルから読み込む
//===========================================================
void CStageManager::LoadEnemyTxt(FILE* pFile)
{
	int nManagerType = 0;                  //マネージャーオブジェクトの種類
	int nType = 0;                         //オブジェクトごとの種類
	int nRotType = 0;                      //向きの種類
	int nSubType = 0;                      //サブタイプ
	D3DXVECTOR3 pos = NULL_VECTOR3;        //位置
	D3DXVECTOR3 WarpPos = NULL_VECTOR3;
	int nWarpMap = 0;//ワープ先のマップ番号
	int nLife = 0;//体力
	D3DXVECTOR3 Scale = NULL_VECTOR3;      //拡大率
	D3DXVECTOR3 Rot = NULL_VECTOR3;        //向き
	CObject* pObject = nullptr;            //オブジェクトへのポインタ
	char aString[128] = {};

	D3DXVECTOR3 MoveRangeAIPos = NULL_VECTOR3;   //移動範囲AIの位置
	D3DXVECTOR3 MoveRangeAIScale = NULL_VECTOR3; //移動範囲AIの拡大率

	while (1)
	{
		fscanf(pFile, "%s", &aString[0]);
		if (aString[0] == '#')
		{
			fgets(&aString[0], 100, pFile);//行をスキップ
		}
		else if (strcmp(&aString[0], "MANAGERTYPE") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);  //イコール読み取り
			fscanf(pFile, "%d", &nManagerType);//マネージャーオブジェクトのタイプ取得
		}
		else if (strcmp(&aString[0], "TYPE") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);  //イコール読み取り
			fscanf(pFile, "%d", &nType);       //オブジェクトごとのタイプ取得
		}
		else if (strcmp(&aString[0], "ROTTYPE") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);  //イコール読み取り
			fscanf(pFile, "%d", &nRotType);    //オブジェクトごとのタイプ取得
		}
		else if (strcmp(&aString[0], "POS") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);  //イコール読み取り
			fscanf(pFile, "%f", &pos.x);       //X座標読み取り
			fscanf(pFile, "%f", &pos.y);       //Y座標読み取り
			fscanf(pFile, "%f", &pos.z);       //Z座標読み取り
		}
		else if (strcmp(&aString[0], "SCALE") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);  //イコール読み取り
			fscanf(pFile, "%f", &Scale.x);       //X座標読み取り
			fscanf(pFile, "%f", &Scale.y);       //Y座標読み取り
			fscanf(pFile, "%f", &Scale.z);       //Z座標読み取り
		}
		else if (strcmp(&aString[0], "ROT") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);  //イコール読み取り
			fscanf(pFile, "%f", &Rot.x);       //X座標読み取り
			fscanf(pFile, "%f", &Rot.y);       //Y座標読み取り
			fscanf(pFile, "%f", &Rot.z);       //Z座標読み取り
		}
		else if (strcmp(&aString[0], "LIFE") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);  //イコール読み取り
			fscanf(pFile, "%d", &nLife);       //Z座標読み取り
		}
		else if (strcmp(&aString[0], "SUBTYPE") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);  //イコール読み取り
			fscanf(pFile, "%d", &nSubType);    //サブタイプ
		}
		else if (strcmp(&aString[0], "MOVERANGEAIPOS") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);             //イコール読み取り
			fscanf(pFile, "%f", &MoveRangeAIPos.x);       //X座標読み取り
			fscanf(pFile, "%f", &MoveRangeAIPos.y);       //Y座標読み取り
			fscanf(pFile, "%f", &MoveRangeAIPos.z);       //Z座標読み取り
		}
		else if (strcmp(&aString[0], "MOVERANGEAISCALE") == 0)
		{
			fscanf(pFile, "%s", &aString[0]);             //イコール読み取り
			fscanf(pFile, "%f", &MoveRangeAIScale.x);       //X座標読み取り
			fscanf(pFile, "%f", &MoveRangeAIScale.y);       //Y座標読み取り
			fscanf(pFile, "%f", &MoveRangeAIScale.z);       //Z座標読み取り
		}
		else if (strcmp(&aString[0], "END_SETENEMY") == 0)
		{//Xオブジェクトを設定
			if (CScene::GetMode() == CScene::MODE_EDIT)
			{//エディットモードなら、オブジェクトをリストに保存し管理する
				pObject = CEnemy::Create(CEnemy::ENEMYTYPE(nType), nLife, pos, NULL_VECTOR3, Scale, MoveRangeAIPos, NULL_VECTOR3, MoveRangeAIScale);
				if (pObject != nullptr)
				{
					pObject->SetStageManagerObj();                                                 //ステージマネージャーで管理するオブジェクトに設定	
					m_nIndexObject++;
				}
			}
			else if (CScene::GetMode() == CScene::MODE_GAME)
			{//ゲームモードなら、オブジェクトを生成するだけで、リストに保存しない
				CEnemy::Create(CEnemy::ENEMYTYPE(nType), nLife, pos, NULL_VECTOR3, Scale, MoveRangeAIPos, NULL_VECTOR3, MoveRangeAIScale);
			}
			break;
		}
	}
}
//=======================================================================================================================

//===========================================================
//サイズ分動かさないときの処理
//===========================================================
D3DXVECTOR3 CStageManager::NotSizeMoveProcess(float fMoveX, float fMoveY)
{
	D3DXVECTOR3 Move = NULL_VECTOR3;

	if (CManager::GetCamera()->GetCameraType() == CCamera::CAMERATYPE_BIRD)
	{
		Move.x = sinf(atan2f(fMoveX, fMoveY)) * m_fSpeed;
		Move.z = cosf(atan2f(fMoveX, fMoveY)) * m_fSpeed;
	}
	else
	{
		if (m_MoveMode == MOVEMODE00_XY)
		{
			Move.x = sinf(atan2f(fMoveX, fMoveY) + CManager::GetCamera()->GetRot().y) * m_fSpeed;
			Move.y = cosf(atan2f(fMoveX, fMoveY) + CManager::GetCamera()->GetRot().y) * m_fSpeed;
		}
		else if (m_MoveMode == MOVEMODE01_XZ)
		{
			Move.x = sinf(atan2f(fMoveX, fMoveY) + CManager::GetCamera()->GetRot().y) * m_fSpeed;
			Move.z = cosf(atan2f(fMoveX, fMoveY) + CManager::GetCamera()->GetRot().y) * m_fSpeed;
		}
	}
	return Move;
}
//=======================================================================================================================

//===========================================================
//サイズ分動かすときの処理
//===========================================================
D3DXVECTOR3 CStageManager::SizeMoveProcess(float fMoveX, float fMoveY,D3DXVECTOR3 Size)
{
	D3DXVECTOR3 ResultSize = NULL_VECTOR3;

	ResultSize.x = float(floor(Size.x));
	ResultSize.y = float(floor(Size.y));
	ResultSize.z = float(floor(Size.z));

	D3DXVECTOR3 Move = NULL_VECTOR3;
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_DOWN) == true ||
		CManager::GetInputKeyboard()->GetTrigger(DIK_UP) == true ||
		CManager::GetInputKeyboard()->GetTrigger(DIK_RIGHT) == true ||
		CManager::GetInputKeyboard()->GetTrigger(DIK_LEFT) == true)
	{
		if (m_MoveMode == MOVEMODE00_XY)
		{
			if (fMoveX != 0.0f)
			{
				Move.x = sinf(atan2f(fMoveX, 0.0f)) * ResultSize.x;
			}
			else if (fMoveY != 0.0f)
			{
				Move.y = cosf(atan2f(0.0f, fMoveY)) * ResultSize.x;
			}
		}
	}


	return Move;
}
//=======================================================================================================================

//===========================================================
//拡大率をリセットする処理
//===========================================================
void CStageManager::ResetScale()
{
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_V) == true)
	{
		m_Scale = ONE_VECTOR3;
	}
}
//=======================================================================================================================

//===========================================================
//全てのマネージャーオブジェクトを破棄する
//===========================================================
void CStageManager::AllReleaseObject()
{
	//CObject* pObj = nullptr;//オブジェクト取得用
	//pObj = GetTopStageManagerObject();

	//CObject::TYPE type;
	//while (pObj != nullptr)
	//{
	//	//次のオブジェクトを格納
	//	CObject* pNext = pObj->GetNextStageManagerObject();
	//	pObj->SetUseDeath(true);
	//	pObj->SetDeath();
	//	type = pObj->GetType();
	//	m_nIndexObject--;
	//	pObj = pNext;
	//}

	////for (int nCnt = 0; nCnt < m_nMAXMANAGEROBJECT; nCnt++)
	////{
	////	if (m_apObjectList[nCnt] != nullptr)
	////	{
	////		m_apObjectList[nCnt]->SetDeath();
	////		m_apObjectList[nCnt] = nullptr;
	////		m_nIndexObject--;
	////	}
	//}
}
//=======================================================================================================================