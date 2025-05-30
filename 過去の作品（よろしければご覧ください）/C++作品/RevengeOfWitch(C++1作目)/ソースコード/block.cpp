//==========================================================================================================
//
//７月１２日：全ギミックを実装する[block.cpp]
//Author:ShinaTaiyo
//
//==========================================================================================================

//=========================
//インクルード
//=========================
#include "block.h"
#include "manager.h"
#include "explosion.h"
#include "enemy.h"
#include "effect.h"
#include "objectXInfo.h"
#include "calculation.h"
#include "stagemanager.h"
#include "player.h"
#include "game.h"
#include "edit.h"
#include "particle.h"
#include "input.h"
#include "debugtext.h"
#include <stdio.h>
#include <string.h>
//=================================================

//=========================
//静的メンバ
//=========================
int CBlock::m_nNumFile = CBlock::BLOCKTYPE_MAX;                                              //ファイル数を格納する
const int CBlock::m_BLOCKSUBTYPENUM[CBlock::BLOCKTYPE_MAX] = { 1//0
,1//1
,1//2
,1//3
,3//4 リフトブロック
,1//5
,1//6
,1//7
,1//8
,1//9
,1//10
,1//11
,2//12 矢印ブロック
,1//13
,1//14
,1//15
,1//16
,1};//17     //ブロックのサブタイプ
const float CBlock::m_fBLOCKCORRECTIONCOLLISION = 3.0f;      //1,2,3,4,5,6,7,8,9,10,11,12,13                            //ブロックに当たり判定の余裕を持たせる量
const char* CBlock::m_BLOCK_FILENAME[CBlock::BLOCKTYPE_MAX] =
{ "data\\MODEL\\Block\\Block00_Normal.x", 
  "data\\MODEL\\Block\\Block01_Water.x",
 "data\\MODEL\\Block\\Block02_Grass.x" ,
 "data\\MODEL\\Block\\Block03_LongRenga.x",
"data\\MODEL\\Block\\Block04_Rift.x",
"data\\MODEL\\Block\\Block05_Fire.x",                                                    
"data\\MODEL\\Block\\Block06_Jump.x",                                                    
"data\\MODEL\\Block\\Block07_Crack.x",                                                      
"data\\MODEL\\Block\\Block08_SukeSuke.x",                                                   
"data\\MODEL\\Block\\Block09_BlinkingRed.x",                                                
"data\\MODEL\\Block\\Block10_BlinkingBrue.x",
"data\\MODEL\\Block\\Block11_Ice.x",                                                    
"data\\MODEL\\Block\\Block12_Arrow.x",                                                  
"data\\MODEL\\Block\\Block13_Swamp.x",
"data\\MODEL\\Block\\Block14_Tornado.x",                                                
"data\\MODEL\\Block\\Block15_Fall.x",
"data\\MODEL\\Block\\Block16_Save.x",
"data\\MODEL\\Block\\Block17_ChainBlock.x",
};                                                  //ブロックのXファイルへのポインタ
//========================================================

//=========================
//コンストラクタ
//=========================
CBlock::CBlock() : CObjectX(2),m_FallBlock()
{

}
//=================================================

//=========================
//デストラクタ
//=========================
CBlock::~CBlock()
{

}
//=================================================

//=========================
//初期化処理
//=========================
HRESULT CBlock::Init()
{
	//===========================
	//Xオブジェクト初期化
	//===========================
	CObjectX::Init();
	//=========================================
	m_bMove = false;               //移動するかどうか
	m_bUse = false;                //使用状態
	m_bTurn = false;               //ターンフラグ
	m_type = BLOCKTYPE00_NORMAL;   //ブロックの種類
	m_fRot = 0.0f;                 //向きの補正値
	m_nCntTime = 0;                //生成されてからの時間をカウントする
	m_nSubType = 0;                //サブタイプ
	m_bCollision = true;           //当たり判定をするかどうか
	SetInertia(0.0f);              //慣性を設定
	m_fMoveLength = 200.0f;        //動く距離
	m_fRotSpeed = 0.1f;            //回転速度
	m_fSpeed = 5.0f;               //速度
	m_fStartRot = 0.0f;            //開始向き
	//===========================
	//点滅ブロックの情報
	//===========================
	m_BlinkingBlock.bBlinking = true;//点滅しているかどうか
	m_BlinkingBlock.nBlinkingSpeed = 50;//点滅速度
	//==================================================================

	//===========================
	//矢印ブロックの情報
	//===========================
	m_ArrowBlock.fAddSpeed = 2.0f;//加速度
	//==================================================================
	
	//===========================
	//ジャンプブロックの情報
	//===========================
	m_JumpBlock.fJumpPower = 25.0f;
	//==================================================================

	//===========================
	//フォールブロックの情報
	//===========================
	m_FallBlock.bFall = false;
	m_FallBlock.nReraiseCnt = 100;
	m_FallBlock.nRespawnCnt = m_nMAX_FALLBLOCKRESPAWNCNT;//ブロックが落ちたときに復活するまでのカウント
    //==================================================================

	//===========================
	//ブロック判定分け用
	//===========================
	m_bNearBlockX = false;
	m_bNearBlockY = false;
	//==================================================================

	return S_OK;
}
//=================================================

//=========================
//終了処理
//=========================
void CBlock::Uninit()
{
	CObjectX::Uninit();//Xオブジェクト終了処理
	//SetDeath();//死亡フラグ設定
}
//=================================================

//=========================
//更新処理
//=========================
void CBlock::Update()
{

	//=======================
	//変数宣言
	//=======================
	D3DXVECTOR3 &Pos = CObjectX::GetPos();
	D3DXVECTOR3 &PosOld = CObjectX::GetPosOld();
	D3DXVECTOR3& Rot = GetRot();
	D3DXVECTOR3& Move = GetMove();
	float fEnemyWidth = 0.0f;//敵の横幅代入用
	float fEnemyHeight = 0.0f;//敵の高さ代入用
	int nCntRideFrame = 0;    
	int& nLife = GetLife();//体力

	bool bJustTurn = false;
	//===================================================
	
	m_nCntTime++;//生成されてからの時間をカウントする

	GravityProcess();

	switch (m_type)
	{
	case BLOCKTYPE04_RIFT:
		LiftBlockPattern();
		break;
	case BLOCKTYPE09_BLINKINGRED:
		BlinkingBlockPattern();
		break;
	case BLOCKTYPE10_BLINKINGBRUE:
		BlinkingBlockPattern();
		break;
	case BLOCKTYPE12_ARROW:
		ArrowBlockPattern();
		break;
	case BLOCKTYPE15_FALL:
		FallBlockProcess();
		break;
	case BLOCKTYPE16_SAVE:
		Rot.y += 0.03f;
		break;
	default:
		break;
	}

	//========================================
    //Xオブジェクトの更新処理
    //========================================
	CObjectX::Update();
	//===========================================================================================

	Collision();//接触判定を行う

	if (GetStageManagerChoose() == true)
	{

	}

}
//=================================================

//===================================================
//描画処理
//===================================================
void CBlock::Draw()
{
	CObjectX::Draw();
}
//===========================================================================================

//===================================================
//死亡フラグを設定
//===================================================
void CBlock::SetDeath()
{
	CObject::SetDeath();
}
//===========================================================================================

//==================================
//ブロック生成処理
//==================================
CBlock* CBlock::Create(BLOCKTYPE type, int nLife, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, CObjectX::ROTTYPE RotType, int nSubType)
{
	CBlock* pBlock = DBG_NEW CBlock;                                                               //ブロックを生成
	bool bSuccess = pBlock->CObject::GetCreateSuccess();
	LPDIRECT3DTEXTURE9 pResultTexture[m_nMAXOBJECT] = {};                                        //設定された種類の配列のテクスチャ代入用
	int nIdx = 0;//モデル情報のインデックス
	if (bSuccess == true)
	{
		if (pBlock != nullptr)
		{
			pBlock->Init();                                                                                                    //初期化処理
			switch (type)
			{
			case CBlock::BLOCKTYPE04_RIFT:
				pBlock->SetMoveType(MOVETYPE_TRUE);
				pBlock->m_bMove = true;
				break;
			default:
				break;
			}
			pBlock->SetUseGravity(false);
			pBlock->SetUseDeath(true);                                                                                         //死亡フラグを発動するかどうかを設定する
			pBlock->m_type = type;                                                                                             //ブロックの種類
			pBlock->SetUseBlockCollision(false);
			pBlock->SetLife(nLife);                                                                                            //ブロックの体力
			pBlock->SetMaxLife(nLife);                                                                                            //ブロックの体力
			pBlock->SetAutoSubLife(false);              //自動的に体力を減らすかどうか
			pBlock->m_bUse = true;                                                                                             //使用状態
			pBlock->m_nSubType = nSubType;                                                                                     //サブタイプを設定
			pBlock->SetSupportPos(pos);                                                                                        //設置位置
			pBlock->CObjectX::SetPos(pos);                                                                                     //オブジェクトXの位置を設定
			pBlock->SetPosOld(pos);                                                                                            //1f前の位置を設定
			pBlock->CObject::SetType(CObject::TYPE_BLOCK);                                                                     //オブジェクトの種類を決める
			pBlock->SetScale(scale);                                                                                 //オブジェクトの拡大率
			pBlock->SetFormarScale(scale);                                                                                     //元の拡大率を設定する
			nIdx = CManager::GetObjectXInfo()->Regist(CObjectX::OBJECTXTYPE_BLOCK, m_BLOCK_FILENAME[(int)(type)]);
			pBlock->CObjectX::BindObjectXInfo(CManager::GetObjectXInfo()->GetMesh(CObjectX::OBJECTXTYPE_BLOCK,nIdx),
				CManager::GetObjectXInfo()->GetBuffMat(CObjectX::OBJECTXTYPE_BLOCK,nIdx),
				CManager::GetObjectXInfo()->GetdwNumMat(CObjectX::OBJECTXTYPE_BLOCK,nIdx),
				CManager::GetObjectXInfo()->GetTexture(CObjectX::OBJECTXTYPE_BLOCK,nIdx),
				CManager::GetObjectXInfo()->GetColorValue(CObjectX::OBJECTXTYPE_BLOCK,nIdx));                          //モデル情報を割り当てる

			pBlock->CObjectX::SetObjXType(CObjectX::OBJECTXTYPE_BLOCK);                                                        //オブジェクトXのタイプを設定
			pBlock->CObjectX::SetTypeNum((int)(type));                                                                         //オブジェクトXごとのタイプ番号を設定
			pBlock->SetSize();                                                                                                 //Xオブジェクトのサイズを設定する
			pBlock->CObjectX::SetRotType(RotType);                                                                             //向きの種類を設定
			pBlock->SetManagerType((int)(CStageManager::MANAGEROBJECT_BLOCK));        //マネージャーで呼び出す時の種類を設定

		}
	}
	else
	{//オブジェクトに空きがなかったので破棄する
		delete pBlock;
		pBlock = nullptr;
		return nullptr;
	}
	return pBlock;
}
//================================================================================================

//===============================================================
//ブロックを消す処理
//===============================================================
void CBlock::ReleaseBlock()
{
	Release();
}
//=========================================================================================================================

//===============================================================
//ブロックのタイプを取得する
//===============================================================
CBlock::BLOCKTYPE CBlock::GetType()
{
	return m_type;
}
//=========================================================================================================================

//===============================================================
//ブロックとの当たり判定を行う
//===============================================================
void CBlock::Collision()
{
	//==================================
	//オブジェクトXから情報を取得
	//==================================
	D3DXVECTOR3 ComparisonPos = NULL_VECTOR3;                  //判定対象の位置を取得
	D3DXVECTOR3 ComparisonPosOld = NULL_VECTOR3;               //判定対象の1f前の位置を取得
	D3DXVECTOR3 ComparisonVtxMax = NULL_VECTOR3;               //判定対象の最大頂点を取得
	D3DXVECTOR3 ComparisonVtxMin = NULL_VECTOR3;               //判定対象の最小頂点を取得
	D3DXVECTOR3 MySize = NULL_VECTOR3;
	D3DXVECTOR3 BlockPos = GetPos();                          //ブロックの位置を取得
	D3DXVECTOR3 BlockVtxMax = GetVtxMax();                    //ブロックの最大頂点を取得
	D3DXVECTOR3 BlockVtxMin = GetVtxMin();                    //ブロックの最小頂点を取得
	//===========================================================

	//オブジェクト取得
	CObject* pObj = nullptr;
	//==================================
	//ブロックとの当たり判定
	//==================================
	for (int nCntPriority = 0; nCntPriority < m_nMAXPRIORITY; nCntPriority++)
	{
		pObj = GetTopObject(nCntPriority);//トップオブジェクトを取得
		while (pObj != nullptr)
		{
			//次のオブジェクトを格納
			CObject* pNext = pObj->GetNextObject();

			//種類の取得（敵なら当たり判定）
			CObject::TYPE type = pObj->GetType();

			//オブジェクトの種類が敵だったら
			if (type == CObject::TYPE_PLAYER || type == CObject::TYPE_ENEMY || type == CObject::TYPE_BOSS)
			{
				ComparisonPos.x = (float)(floor(((CObjectX*)pObj)->GetPos().x));                   //位置を取得
				ComparisonPos.y = (float)(floor(((CObjectX*)pObj)->GetPos().y));                   //位置を取得
				ComparisonPos.z = (float)(floor(((CObjectX*)pObj)->GetPos().z));                   //位置を取得]

				ComparisonVtxMax.x = (float)(floor(((CObjectX*)pObj)->GetVtxMax().x));             //最大頂点を取得
				ComparisonVtxMax.y = (float)(floor(((CObjectX*)pObj)->GetVtxMax().y));             //最大頂点を取得
				ComparisonVtxMax.z = (float)(floor(((CObjectX*)pObj)->GetVtxMax().z));             //最大頂点を取得

				ComparisonVtxMin.x = (float)(floor(((CObjectX*)pObj)->GetVtxMin().x));             //最小頂点を取得
				ComparisonVtxMin.y = (float)(floor(((CObjectX*)pObj)->GetVtxMin().y));             //最小頂点を取得
				ComparisonVtxMin.z = (float)(floor(((CObjectX*)pObj)->GetVtxMin().z));             //最小頂点を取得

				ComparisonPosOld.x = (float)(floor(((CObjectX*)pObj)->GetPosOld().x));             //1f前の位置を取得
				ComparisonPosOld.y = (float)(floor(((CObjectX*)pObj)->GetPosOld().y));             //1f前の位置を取得
				ComparisonPosOld.z = (float)(floor(((CObjectX*)pObj)->GetPosOld().z));             //1f前の位置を取得

				MySize.x = (float)(floor(((CObjectX*)pObj)->GetSize().x));
				MySize.y = (float)(floor(((CObjectX*)pObj)->GetSize().y));
				MySize.z = (float)(floor(((CObjectX*)pObj)->GetSize().z));
				bool& bIsComparisonLanding = ((CObjectX*)pObj)->GetLanding();                         //判定対象がブロックに乗っているかどうか
				D3DXVECTOR3& ComparisonMove = ((CObjectX*)pObj)->GetMove();                           //移動量を取得
				//((CObjectX*)pObj)->SetLiftLanding(false);                     //リフトに乗っていない状態に初期化
				//上
				if (ComparisonPos.x + ComparisonVtxMax.x > BlockPos.x + BlockVtxMin.x
					&& ComparisonPos.x + ComparisonVtxMin.x < BlockPos.x + BlockVtxMax.x
					&& ComparisonPos.y + ComparisonVtxMin.y - m_fBLOCKCORRECTIONCOLLISION < BlockPos.y + BlockVtxMax.y
					&& ComparisonPosOld.y + ComparisonVtxMin.y + m_fBLOCKCORRECTIONCOLLISION >= BlockPos.y + BlockVtxMax.y
					&& ComparisonPos.z + ComparisonVtxMax.z > BlockPos.z + BlockVtxMin.z
					&& ComparisonPos.z + ComparisonVtxMin.z < BlockPos.z + BlockVtxMax.z)
				{//対象の下端がモデルの+Yに当たった時の処理
					switch (GetType())
					{
					case BLOCKTYPE06_JUMP:
						ComparisonMove.y = m_JumpBlock.fJumpPower;
						bIsComparisonLanding = false;//乗ってない状態にする
						break;
					case BLOCKTYPE11_ICE:
						if (type == CObject::TYPE_PLAYER)
						{
							((CPlayer*)pObj)->SetPlayerSlideInertia(0.05f);
						}
						break;
					case BLOCKTYPE12_ARROW:
						if (m_nSubType == 0)
						{
							ComparisonMove.x += m_ArrowBlock.fAddSpeed;
							if (type == CObject::TYPE_PLAYER)
							{
								if (((CPlayer*)pObj)->GetMove().x > 5.0f)
								{
									((CPlayer*)pObj)->SetPlayerSlideInertia(0.2f);
								}
							}
						}
						else if (m_nSubType == 1)
						{
							ComparisonMove.x += -m_ArrowBlock.fAddSpeed;
							if (type == CObject::TYPE_PLAYER)
							{
								if (((CPlayer*)pObj)->GetMove().x < -5.0f)
								{
									((CPlayer*)pObj)->SetPlayerSlideInertia(0.2f);
								}
							}
						}
						break;
					case BLOCKTYPE13_SWAMP:
						if (type == CObject::TYPE_PLAYER)
						{
							((CPlayer*)pObj)->SetDecelerationState(true);
						}
						break;
					default:
						break;
					}

				}
				if (ComparisonPos.x + ComparisonVtxMin.x - m_fBLOCKCORRECTIONCOLLISION <= BlockPos.x + BlockVtxMax.x &&
					ComparisonPos.x + ComparisonVtxMax.x + m_fBLOCKCORRECTIONCOLLISION >= BlockPos.x + BlockVtxMin.x &&
					ComparisonPos.y + ComparisonVtxMin.y <= BlockPos.y + BlockVtxMax.y &&
					ComparisonPos.y + ComparisonVtxMax.y >= BlockPos.y + BlockVtxMin.y &&
					ComparisonPos.z + ComparisonVtxMin.z - m_fBLOCKCORRECTIONCOLLISION <= BlockPos.z + BlockVtxMax.z &&
					ComparisonPos.z + ComparisonVtxMax.z + m_fBLOCKCORRECTIONCOLLISION >= BlockPos.z + BlockVtxMin.z)
				{//ブロックに当たったら
					switch (GetType())
					{
					case BLOCKTYPE05_FIRE:
						if (type == CObject::TYPE_PLAYER)
						{//プレイヤーのみ効果を発動
							if (CScene::GetMode() == CScene::MODE_GAME)
							{
								CGame::GetPlayer()->SetDamage(1,1);
							}
							else if (CScene::GetMode() == CScene::MODE_EDIT)
							{
								CEdit::GetPlayer()->SetDamage(1,1);
							}
						}
						break;
					case BLOCKTYPE14_TORNADO:
						if (type == CObject::TYPE_PLAYER)
						{//壁ジャンプを不可能にする
							((CPlayer*)pObj)->SetWallJumpBindTime(3);
						}
						break;
					case BLOCKTYPE16_SAVE:
						if (type == CObject::TYPE_PLAYER)
						{

						}
						break;
					default:
						break;

					}
				}
			}
			//リストを次に進める
			pObj = pNext;
		}
	}

}
//=========================================================================================================================

//===============================================================
//ブロックとの当たり判定を行う
//===============================================================
void CBlock::CollisionSquare(CObjectX* pObjX)
{
	D3DXVECTOR3 ComparisonPos = NULL_VECTOR3;        //比較用位置
	D3DXVECTOR3 ComparisonPosOld = NULL_VECTOR3;     //比較用1f前の位置
	D3DXVECTOR3 ComparisonVtxMax = NULL_VECTOR3;     //比較用最高頂点
	D3DXVECTOR3 ComparisonVtxMin = NULL_VECTOR3;     //比較用最低頂点
	D3DXVECTOR3 ComparisonSize = NULL_VECTOR3;       //比較サイズ
	D3DXVECTOR3 ComparisonMove = NULL_VECTOR3;       //比較移動量


	MOVETYPE MoveType = MOVETYPE_NONE;
	//==========================================================================================================
	bool bDodgeState = pObjX->GetDodgeState();       //回避しているかどうか
	bool& bIsLanding = pObjX->GetLanding();
	bool bIsLandingOld = pObjX->GetLanding();//1f前の着地状態
	bIsLanding = false;
	//bIsLanding = false;
	bool& bIsLiftLanding = pObjX->GetLiftLanding();  //リフトの上に乗っているかどうか
	bIsLiftLanding = false;

	bool& bSuccessBlockCollisionY = pObjX->GetSuccessBlockCollisionY();//ブロックとのX方向の当たり判定が成功したかどうか
	bSuccessBlockCollisionY = false;//この時点では当たったかどうか分からないのでfalse

	bool& bSuccessCollisionX = pObjX->GetSuccessBlockCollisionX();
	bSuccessCollisionX = false;

	bool& bIsWalling = pObjX->GetWalling();          //壁にくっついているかどうか
	bool bIsWallingOld = pObjX->GetWalling();        //1f前の壁くっつき状態
	bIsWalling = false;

	CObject*& pLandingObj = pObjX->GetLandingObj();
	pLandingObj = nullptr;

	bool bCollision = true;//当たり判定をするかどうか

	D3DXVECTOR3 Scale = NULL_VECTOR3;

	//====================================================================
	//判定開始
	//====================================================================

	//NearBlockSurchProcess(pObjX->GetPos(),pObjX->GetSize());//全てのブロックが近くのブロックをサーチする
	D3DXVECTOR3& ObjXPos = pObjX->GetPos();
	for (int nCntPriority = 0; nCntPriority < m_nMAXPRIORITY; nCntPriority++)
	{
		CObject * pObj = GetTopObject(nCntPriority);//トップオブジェクトを取得
		while (pObj != nullptr)
		{
			//次のオブジェクトを格納
			CObject* pNext = pObj->GetNextObject();

			//種類の取得（敵なら当たり判定）
			CObject::TYPE type = pObj->GetType();
			if (type == CObject::TYPE_BLOCK)
			{
				CBlock* pBlock = (CBlock*)pObj;

				float fLength = CCalculation::CalculationLength(pObjX->GetPos(), pBlock->GetPos());
				if (pBlock->m_type == BLOCKTYPE08_SUKESUKE && bDodgeState == true)
				{//スケスケブロックかつ回避状態なら
				//リストを次に進める
					pObj = pNext;
					continue;
				}
				else
				{
					Scale = pBlock->GetScale();
					if (Scale.x >= Scale.y - 0.1f && Scale.x <= Scale.y + 0.1f)
					{//拡大率がほとんど同じなら
						if (fLength > pBlock->GetSize().x * 3.0f)
						{
							bCollision = false;
						}
						else
						{
							bCollision = true;
						}
					}
					else
					{
						bCollision = true;
					}

					if (pBlock->m_bCollision == true && bCollision == true)
					{
						//if (pBlock->m_bNearBlockX = true && pBlock->m_bNearBlockY == true)
						//{
							if (bIsLandingOld == true)
							{
								//上下
								pBlock->ExtrusionCollisionY(pObjX, pBlock);

								//左右
								if (pBlock->m_bMove == false)
								{
									pBlock->ExtrusionCollisionX(pObjX, pBlock);
								}
							}
							else
							{
								//左右
								if (pBlock->m_bMove == false)
								{
									pBlock->ExtrusionCollisionX(pObjX, pBlock);
								}

								//上下
								pBlock->ExtrusionCollisionY(pObjX, pBlock);
							}
						//}
						//else
						//{
						//	if (pBlock->m_bNearBlockY == true)
						//	{
						//		//左右
						//		if (pBlock->m_bMove == false)
						//		{
						//			pBlock->ExtrusionCollisionX(pObjX, pBlock);
						//		}

						//		//上下
						//		pBlock->ExtrusionCollisionY(pObjX, pBlock);
						//	}
						//	else
						//	{
						//		//上下
						//		pBlock->ExtrusionCollisionY(pObjX, pBlock);

						//		//左右
						//		if (pBlock->m_bMove == false)
						//		{
						//			pBlock->ExtrusionCollisionX(pObjX, pBlock);
						//		}


						//	}
						//}
					}
				}
			}
			else
			{
				//リストを次に進める
				pObj = pNext;
				continue;
			}

			//リストを次に進める
			pObj = pNext;
		}
	}
}
//=========================================================================================================================

//===============================================================
//指定下ブロックの上に乗っている場合、位置を補正する
//===============================================================
void CBlock::LandingCorrection(D3DXVECTOR3& Pos, CObject* pSaveObj, D3DXVECTOR3 VtxMax, D3DXVECTOR3 VtxMin)
{
	CObject* pObj = nullptr;
	CObject::TYPE type = (CObject::TYPE)(0);
	D3DXVECTOR3 ComparisonPos = NULL_VECTOR3;        //比較用位置
	D3DXVECTOR3 ComparisonPosOld = NULL_VECTOR3;     //比較用1f前の位置
	D3DXVECTOR3 ComparisonVtxMax = NULL_VECTOR3;     //比較用最高頂点
	D3DXVECTOR3 ComparisonVtxMin = NULL_VECTOR3;     //比較用最低頂点
	D3DXVECTOR3 ComparisonSize = NULL_VECTOR3;       //サイズ
	D3DXVECTOR3 ComparisonMove = NULL_VECTOR3;
	CBlock* pBlock = nullptr;

	D3DXVECTOR3 MyVtxMin = NULL_VECTOR3;
	MyVtxMin.x = (float)(floor(VtxMin.x));
	MyVtxMin.y = (float)(floor(VtxMin.y));
	MyVtxMin.z = (float)(floor(VtxMin.z));

	if (pSaveObj != nullptr)
	{
		//種類の取得
		type = pSaveObj->GetType();
		if (type == CObject::TYPE_BLOCK)
		{
			CBlock* pBlock = (CBlock*)pSaveObj;

			ComparisonPos.x = (float)(floor(pBlock->GetPos().x));
			ComparisonPos.y = (float)(floor(pBlock->GetPos().y));
			ComparisonPos.z = (float)(floor(pBlock->GetPos().z));

			ComparisonPosOld.x = (float)(floor(pBlock->GetPosOld().x));
			ComparisonPosOld.y = (float)(floor(pBlock->GetPosOld().y));
			ComparisonPosOld.z = (float)(floor(pBlock->GetPosOld().z));

			ComparisonVtxMax.x = (float)(floor(pBlock->GetVtxMax().x));
			ComparisonVtxMax.y = (float)(floor(pBlock->GetVtxMax().y));
			ComparisonVtxMax.z = (float)(floor(pBlock->GetVtxMax().z));

			ComparisonVtxMin.x = (float)(floor(pBlock->GetVtxMin().x));
			ComparisonVtxMin.y = (float)(floor(pBlock->GetVtxMin().y));
			ComparisonVtxMin.z = (float)(floor(pBlock->GetVtxMin().z));

			ComparisonSize.x = (float)(floor(pBlock->GetSize().x));
			ComparisonSize.y = (float)(floor(pBlock->GetSize().y));
			ComparisonSize.z = (float)(floor(pBlock->GetSize().z));
			 

			float fPosY = fabsf(MyVtxMin.y);
			Pos.y = ComparisonPos.y + ComparisonVtxMax.y + fPosY;
		}
	}

}
//=========================================================================================================================

//===============================================================
//ブロックの情報を設定する
//===============================================================
void CBlock::SetBlockInfo(float fStartRot, float fRotSpeed, float fMoveLength, float fSpeed, int nBlinkSpeed, float fAddSpeed, float fJumpPower)
{
	m_fStartRot = fStartRot;                       //回転開始向き
	m_fRotSpeed = fRotSpeed;                       //回転速度
	m_fMoveLength = fMoveLength;                   //移動距離
	m_fSpeed = fSpeed;                             //移動速度
	m_BlinkingBlock.nBlinkingSpeed = nBlinkSpeed;  //点滅速度
	m_ArrowBlock.fAddSpeed = fAddSpeed;            //加速度
	m_JumpBlock.fJumpPower = fJumpPower;           //ジャンプ力
}
//=========================================================================================================================

//===============================================================
//ブロックの上に乗っているかどうかの判定をする
//===============================================================
void CBlock::RidingTopCollision(bool bJustTurn)
{
	//==========================
	//変数
	//==========================

	D3DXVECTOR3 ComparisonPos = NULL_VECTOR3;        //比較用位置
	D3DXVECTOR3 ComparisonPosOld = NULL_VECTOR3;     //比較用1f前の位置
	D3DXVECTOR3 ComparisonVtxMax = NULL_VECTOR3;     //比較用最高頂点
	D3DXVECTOR3 ComparisonVtxMin = NULL_VECTOR3;     //比較用最低頂点
	D3DXVECTOR3 ComparisonSize = NULL_VECTOR3;       //比較用最大サイズ
	//====================
	//対象
	//====================
	D3DXVECTOR3 Pos = NULL_VECTOR3;
	D3DXVECTOR3 VtxMin = NULL_VECTOR3;
	D3DXVECTOR3 VtxMax = NULL_VECTOR3;
	D3DXVECTOR3 PosOld = NULL_VECTOR3;
	D3DXVECTOR3 Size = NULL_VECTOR3;
	D3DXVECTOR3 Move = NULL_VECTOR3;
	//====================================================================

	//====================
	//ブロック
	//====================
	ComparisonPos.x = (float)(floor(GetPos().x));
	ComparisonPos.y = (float)(floor(GetPos().y));
	ComparisonPos.z = (float)(floor(GetPos().z));

	ComparisonPosOld.x = (float)(floor(GetPosOld().x));
	ComparisonPosOld.y = (float)(floor(GetPosOld().y));
	ComparisonPosOld.z = (float)(floor(GetPosOld().z));

	ComparisonVtxMax.x = (float)(floor(GetVtxMax().x));
	ComparisonVtxMax.y = (float)(floor(GetVtxMax().y));
	ComparisonVtxMax.z = (float)(floor(GetVtxMax().z));

	ComparisonVtxMin.x = (float)(floor(GetVtxMin().x));
	ComparisonVtxMin.y = (float)(floor(GetVtxMin().y));
	ComparisonVtxMin.z = (float)(floor(GetVtxMin().z));

	ComparisonSize.x = (float)(floor(GetSize().x));
	ComparisonSize.y = (float)(floor(GetSize().y));
	ComparisonSize.z = (float)(floor(GetSize().z));
	//=========================================================

	//for (int nCntPriority = 0; nCntPriority < m_nMAXPRIORITY; nCntPriority++)
	//{
	//	for (int nCntObj = 0; nCntObj < m_nMAXOBJECT; nCntObj++)
	//	{
	//		CObject* pObj = CObject::GetCObject(nCntObj, nCntPriority);

	//		if (pObj != nullptr)
	//		{
	//			if (pObj->GetType() == CObject::TYPE_PLAYER)
	//			{
	//				Pos = ((CPlayer*)pObj)->GetPos();
	//				PosOld = ((CPlayer*)pObj)->GetPosOld();
	//				Size.x = (float)(floor(((CPlayer*)pObj)->GetSize().x));
	//				Size.y = (float)(floor(((CPlayer*)pObj)->GetSize().y));
	//				Size.z = (float)(floor(((CPlayer*)pObj)->GetSize().z));
	//				VtxMax.x = (float)(floor(((CPlayer*)pObj)->GetVtxMax().x));
	//				VtxMax.y = (float)(floor(((CPlayer*)pObj)->GetVtxMax().y));
	//				VtxMax.z = (float)(floor(((CPlayer*)pObj)->GetVtxMax().z));
	//				VtxMin.x = (float)(floor(((CPlayer*)pObj)->GetVtxMin().x));
	//				VtxMin.y = (float)(floor(((CPlayer*)pObj)->GetVtxMin().y));
	//				VtxMin.z = (float)(floor(((CPlayer*)pObj)->GetVtxMin().z));
	//				Move.x = (float)(floor(((CPlayer*)pObj)->GetVtxMin().x));
	//				Move.y = (float)(floor(((CPlayer*)pObj)->GetVtxMin().y));
	//				Move.z = (float)(floor(((CPlayer*)pObj)->GetVtxMin().z));

	//				////上
	//				//if (Pos.x + Size.x / 2 > ComparisonPos.x + ComparisonVtxMin.x
	//				//	&& Pos.x - Size.x / 2 < ComparisonPos.x + ComparisonVtxMax.x
	//				//	&& Pos.y <= ComparisonPos.y + ComparisonVtxMax.y
	//				//	&& PosOld.y >= ComparisonPos.y + ComparisonVtxMax.y
	//				//	&& Pos.z + Size.z / 2 > ComparisonPos.z + ComparisonVtxMin.z
	//				//	&& Pos.z - Size.z / 2 < ComparisonPos.z + ComparisonVtxMax.z)
	//				//{//対象の下端がモデルの+Yに当たった時の処理
	//				//	Pos.x += m_move.x;
	//				//	((CPlayer*)pObj)->SetPos(Pos);
	//				//}
	//				//左
	//				if (ComparisonPos.x + ComparisonSize.x / 2 > Pos.x + VtxMin.x
	//					&& ComparisonPosOld.x + ComparisonSize.x / 2 - m_move.x <= Pos.x + VtxMin.x
	//					&& ComparisonPos.y + ComparisonSize.y > Pos.y
	//					&& ComparisonPos.y < Pos.y + VtxMax.y
	//					&& ComparisonPos.z + ComparisonSize.z / 2 > Pos.z + VtxMin.z
	//					&& ComparisonPos.z - ComparisonSize.z / 2 < Pos.z + VtxMax.z)
	//				{//対象がブロックの-Xに当たった時の処理（プレイヤーの位置をずらす）
	//			       //Pos.x = ComparisonPos.x + ComparisonSize.x / 2 + Size.x / 2 + 5.0f;
	//					Pos.x += m_move.x;
	//				   ((CPlayer*)pObj)->SetPos(Pos);
	//				}
	//				//右
	//				else if (ComparisonPos.x - ComparisonSize.x / 2 < Pos.x + VtxMax.x
	//					&& ComparisonPosOld.x - ComparisonSize.x / 2 - m_move.x >= Pos.x + VtxMax.x
	//					&& ComparisonPos.y + ComparisonSize.y > Pos.y
	//					&& ComparisonPos.y < Pos.y + VtxMax.y
	//					&& ComparisonPos.z + ComparisonSize.z / 2 > Pos.z + VtxMin.z
	//					&& ComparisonPos.z - ComparisonSize.z / 2 < Pos.z + VtxMax.z)
	//				{//対象がブロックの+Xに当たった時の処理
	//					Pos.x += m_move.x;
	//					((CPlayer*)pObj)->SetPos(Pos);
	//				}

	//			}

	//		}
	//	}
	//}
}
//======================================================================================================================================

//===============================================================
//リフトブロックのパターン
//===============================================================
void CBlock::LiftBlockPattern()
{
	//=======================
    //変数宣言
    //=======================
	D3DXVECTOR3 &Pos = CObjectX::GetPos();
	D3DXVECTOR3 &Move = GetMove();                                //移動量を取得
	D3DXVECTOR3 PosOld = CObjectX::GetPosOld();
	int nCntRideFrame = 0;
	float fRadius = 0.0f;
	float add_x = 0.0f;
	float add_y = 0.0f;
	//===================================================

	switch (m_nSubType)
	{
	case 0:
		LiftBlockRoundMove();
	break;
	case 1:
		// 三角関数を使用し、円の位置を割り出す。
		Move.x = cosf(m_fStartRot + m_fRot) * m_fSpeed;
		Move.y = sinf(m_fStartRot + m_fRot) * m_fSpeed;
		// 向きを変える
		m_fRot += m_fRotSpeed;		
		break;
	case 2:
		// 三角関数を使用し、円の位置を割り出す。
		Move.x = cosf(m_fStartRot - m_fRot) * m_fSpeed;
		Move.y = sinf(m_fStartRot - m_fRot) * m_fSpeed;

		// 向きを変える
		m_fRot += m_fRotSpeed;
		break;
	default:
		break;
	}

}
//======================================================================================================================================

//===============================================================
//リフトブロックの横の動き
//===============================================================
void CBlock::LiftBlockRoundMove()
{
	D3DXVECTOR3& Pos = CObjectX::GetPos();
	D3DXVECTOR3& Move = GetMove();
	bool bJustTurn = false;
	D3DXVECTOR3 SupportPos = GetSupportPos();

	//支点と現在地の距離を測る
	float fLength = sqrtf(powf(Pos.x - SupportPos.x, 2) + powf(Pos.y - SupportPos.y, 2) + powf(Pos.z - SupportPos.z, 2));

	//移動距離を超えたら
	if (fLength > m_fMoveLength)
	{
        m_bTurn = m_bTurn ? false : true;
        bJustTurn = true;
	}

	//往復させる処理
	if (m_bTurn == false)
	{
		Move.x = cosf(m_fStartRot) * m_fSpeed;
		Move.y = sinf(m_fStartRot) * m_fSpeed;
	}
	else
	{
		Move.x = cosf(m_fStartRot) * -m_fSpeed;
		Move.y = sinf(m_fStartRot) * -m_fSpeed;
	}

}
//======================================================================================================================================

//===============================================================
//点滅ブロックのパターン
//===============================================================
void CBlock::BlinkingBlockPattern()
{
	bool bCollisionPattern = false;

	if (m_nCntTime % m_BlinkingBlock.nBlinkingSpeed == 0)
	{
		m_BlinkingBlock.bBlinking = m_BlinkingBlock.bBlinking ? false : true;
	}

	if (m_type == BLOCKTYPE09_BLINKINGRED)
	{
		bCollisionPattern = false;
	}
	else if (m_type == BLOCKTYPE10_BLINKINGBRUE)
	{
		bCollisionPattern = true;
	}

	if (m_BlinkingBlock.bBlinking == bCollisionPattern)
	{
		SetOriginalColor(NORMAL_COL * 0.0f, 2);
		m_bCollision = false;
	}
	else
	{
		m_bCollision = true;
	}

}
//======================================================================================================================================

//===============================================================
//矢印ブロックのパターン
//===============================================================
void CBlock::ArrowBlockPattern()
{
	D3DXVECTOR3& Rot = GetRot();
	switch (m_nSubType)
	{
	case 0:
		Rot.y = 0.0f;
		break;
	case 1:
		Rot.y = D3DX_PI;
		break;
	default:
		break;
	}
}
//======================================================================================================================================

//===============================================================
//落ちるブロックの処理
//===============================================================
void CBlock::FallBlockProcess()
{
	D3DXVECTOR3& Pos = GetPos();
	if (m_FallBlock.bFall == true)
	{
		m_FallBlock.nRespawnCnt--;
		if (m_FallBlock.nRespawnCnt <= 0)
		{
			Pos = GetSupportPos();
			m_FallBlock.bFall = false;
			m_FallBlock.nReraiseCnt = 0;
			m_FallBlock.nRespawnCnt = m_nMAX_FALLBLOCKRESPAWNCNT;
			SetUseGravity(false);
			SetMove(NULL_VECTOR3);
			SetColorAlphaOnly(0.0f);
		}
	}
	else
	{
		if (m_FallBlock.nReraiseCnt < 100)
		{
			m_FallBlock.nReraiseCnt++;
			SetColorAlphaOnly(float(m_FallBlock.nReraiseCnt) * 0.01f);
		}
	}

}
//======================================================================================================================================

//=======================================================================
//情報表示処理
//=======================================================================
void CBlock::DispInfo()
{
	m_fRot = 0.0f;
	//回転開始位置
	if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
	{//シフトキーを押しながら・・・
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_1) == true)
		{
			m_fStartRot -= 0.1f;

		}
	}
	else if (CManager::GetInputKeyboard()->GetTrigger(DIK_1) == true)
	{
		m_fStartRot += 0.1f;
	}

	//回転速度
	if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
	{//シフトキーを押しながら・・・
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_2) == true)
		{
			m_fRotSpeed -= 0.01f;

		}
	}
	else if (CManager::GetInputKeyboard()->GetTrigger(DIK_2) == true)
	{
		m_fRotSpeed += 0.01f;
	}

	//移動距離
	if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
	{//シフトキーを押しながら・・・
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_3) == true)
		{
			m_fMoveLength -= 5.0f;

		}
	}
	else if (CManager::GetInputKeyboard()->GetTrigger(DIK_3) == true)
	{
		m_fMoveLength += 5.0f;
	}
	
	//移動速度
	if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
	{//シフトキーを押しながら・・・
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_4) == true)
		{
			m_fSpeed -= 0.1f;
		}
	}
	else if (CManager::GetInputKeyboard()->GetTrigger(DIK_4) == true)
	{
		m_fSpeed += 0.1f;
	}

	//点滅速度
	if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
	{//シフトキーを押しながら・・・
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_5) == true)
		{
			m_BlinkingBlock.nBlinkingSpeed -= 5;

		}
	}
	else if (CManager::GetInputKeyboard()->GetTrigger(DIK_5) == true)
	{
		m_BlinkingBlock.nBlinkingSpeed += 5;
	}

	//矢印ブロックの加速度
	if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
	{//シフトキーを押しながら・・・
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_6) == true)
		{
			m_ArrowBlock.fAddSpeed -= 0.2f;

		}
	}
	else if (CManager::GetInputKeyboard()->GetTrigger(DIK_6) == true)
	{
		m_ArrowBlock.fAddSpeed += 0.2f;
	}

	//ジャンプブロックのジャンプ力
	if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
	{//シフトキーを押しながら・・・
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_7) == true)
		{
			m_JumpBlock.fJumpPower -= 1.0f;
		}
	}
	else if (CManager::GetInputKeyboard()->GetTrigger(DIK_7) == true)
	{
		m_JumpBlock.fJumpPower += 1.0f;
	}

	CManager::GetDebugText()->PrintDebugText("回転開始位置（１）：%f\n", m_fStartRot);
	CManager::GetDebugText()->PrintDebugText("回転速度（２）：%f\n", m_fRotSpeed);
	CManager::GetDebugText()->PrintDebugText("移動距離（３）：%f\n", m_fMoveLength);
	CManager::GetDebugText()->PrintDebugText("移動速度（４）：%f\n", m_fSpeed);
	CManager::GetDebugText()->PrintDebugText("点滅速度（５）：%d\n", m_BlinkingBlock.nBlinkingSpeed);
	CManager::GetDebugText()->PrintDebugText("加速度（６）：%f\n", m_ArrowBlock.fAddSpeed);
	CManager::GetDebugText()->PrintDebugText("ジャンプ力（７）：%f", m_JumpBlock.fJumpPower);

	CObjectX::DispInfo();

}
//======================================================================================================================================

//=======================================================================
//近くにブロックがあるかどうかをサーチする
//=======================================================================
void CBlock::NearBlockSearch()
{
	CObject* pObj = nullptr;
	D3DXVECTOR3 Pos = GetPos();//ブロックの位置
	D3DXVECTOR3 VtxMax = GetVtxMax();//最大頂点
	D3DXVECTOR3 VtxMin = GetVtxMin();//最小頂点
	D3DXVECTOR3 Size = GetSize();    //サイズ
	D3DXVECTOR3 ComparisonPos = NULL_VECTOR3;   //対象の位置
	D3DXVECTOR3 ComparisonVtxMax = NULL_VECTOR3;//対象の最大頂点
	D3DXVECTOR3 ComparisonVtxMin = NULL_VECTOR3;//対象の最小頂点

	float fLength = 0.0f;//距離計算用
	float fLengthX = 0.0f;//X座標の距離計算用
	float fLengthY = 0.0f;//Y座標の距離計算用
	float fCheckLength = 0.0f;//比較用
	bool bSuccess = false;
	bool bEnd = false;//ループ終了フラグ
	for (int nCntPriority = 0; nCntPriority < m_nMAXPRIORITY; nCntPriority++)
	{
		pObj = GetTopObject(nCntPriority);//トップオブジェクトを取得
		if (bEnd == true)
		{
			break;
		}
		else
		{
			while (pObj != nullptr)
			{
				//次のオブジェクトを格納
				CObject* pNext = pObj->GetNextObject();

				//種類の取得（敵なら当たり判定）
				CObject::TYPE type = pObj->GetType();

				if (type == CObject::TYPE_BLOCK && pObj != this)
				{//対象が「ブロック」かつ、自分以外なら計算を開始する
					CBlock* pBlock = (CBlock*)pObj;
					ComparisonPos = pBlock->GetPos();
					ComparisonVtxMax = pBlock->GetVtxMax();
					ComparisonVtxMin = pBlock->GetVtxMin();

					if (Pos.x + VtxMin.x - 1.0f <= ComparisonPos.x + ComparisonVtxMax.x &&
						Pos.x + VtxMax.x + 1.0f >= ComparisonPos.x + ComparisonVtxMin.x &&
						Pos.y + VtxMin.y - 1.0f <= ComparisonPos.y + ComparisonVtxMax.y &&
						Pos.y + VtxMax.y + 1.0f >= ComparisonPos.y + ComparisonVtxMin.y)
					{//ブロック同士が重なっていたら・・・
						fLengthX = sqrtf(powf(ComparisonPos.x - Pos.x, 2));
						fLengthY = sqrtf(powf(ComparisonPos.y - Pos.y, 2));

						if (fLengthY > fLengthX)
						{//Yは必ず優先なので、ここで処理を終わる。Yの距離が遠いので、
							m_bNearBlockY = true;
							bEnd = true;
						}
						else
						{//Xの距離が遠いので、横に並んでいる
							m_bNearBlockX = true;
							bEnd = true;
						}
					}
				}
				else
				{
					pObj = pNext;
					continue;
				}
				//リストを次に進める
				pObj = pNext;
			}
		}
	}

	float fRandRot = CCalculation::CalculationRandVecXY();
	//if (m_bNearBlockX == true)
	//{
	//	CParticle::Create(CParticle::TYPE00_NORMAL, 20, 20.0f, 20.0f, Pos, D3DXVECTOR3(sinf(fRandRot) * 20.0f, cosf(fRandRot) * 20.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), true);
	//}

	//if (m_bNearBlockY == true)
	//{
	//	CParticle::Create(CParticle::TYPE00_NORMAL, 20, 20.0f, 20.0f, Pos, D3DXVECTOR3(sinf(fRandRot) * 10.0f, cosf(fRandRot) * 10.0f, 0.0f), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), true);
	//}

}
//======================================================================================================================================

//=======================================================================
//近くのブロックを探す処理
//=======================================================================
void CBlock::NearBlockSurchProcess(D3DXVECTOR3 Pos, D3DXVECTOR3 Size)
{
	CObject* pObj = nullptr;
	float fLength = 0.0f;
	for (int nCntPriority = 0; nCntPriority < m_nMAXPRIORITY; nCntPriority++)
	{
		pObj = GetTopObject(nCntPriority);//トップオブジェクトを取得
		while (pObj != nullptr)
		{
			//次のオブジェクトを格納
			CObject* pNext = pObj->GetNextObject();

			//種類の取得（敵なら当たり判定）
			CObject::TYPE type = pObj->GetType();

			if (type == CObject::TYPE_BLOCK)
			{
				CBlock* pBlock = (CBlock*)pObj;
				pBlock->m_bNearBlockX = false;
				pBlock->m_bNearBlockY = false;
				
				float fLength = CCalculation::CalculationLength(Pos, pBlock->GetPos());
				float fBlockSize = 0.0f;
				float fObjXSize = 0.0f;

				if (pBlock->GetSize().x > pBlock->GetSize().y)
				{
					fBlockSize = pBlock->GetSize().x;
				}
				else
				{
					fBlockSize = pBlock->GetSize().y;
				}

				if (Size.x > Size.y)
				{
					fObjXSize = Size.x;
				}
				else
				{
					fObjXSize = Size.y;
				}

				if (fLength < fBlockSize + fObjXSize)
				{
					pBlock->NearBlockSearch();//近くのブロックを探す
				}
			}
			else
			{
				pObj = pNext;
				continue;
			}

			//リストを次へ進める
			pObj = pNext;
		}
	}
}
//======================================================================================================================================

//=======================================================================
//X方向の押し出し判定を行う
//=======================================================================
void CBlock::ExtrusionCollisionX(CObjectX* pMyObjX, CBlock* pBlock)
{
	D3DXVECTOR3 MyPos = pMyObjX->GetPos();
	D3DXVECTOR3& Pos = pMyObjX->GetPos();              //位置を取得
	D3DXVECTOR3& Move = pMyObjX->GetMove();             //移動量を取得
	D3DXVECTOR3& PosOld = pMyObjX->GetPosOld();        //1f前の位置を取得
	D3DXVECTOR3 MyVtxMax = NULL_VECTOR3;             //自分自身の最大頂点
	D3DXVECTOR3 MyVtxMin = NULL_VECTOR3;             //自分自身の最小頂点

	D3DXVECTOR3 ComPos = pBlock->GetPos();             //ブロックの位置
	D3DXVECTOR3 ComVtxMax = pBlock->GetVtxMax();       //ブロックの最大頂点
	D3DXVECTOR3 ComVtxMin = pBlock->GetVtxMin();       //ブロックの最小頂点
	D3DXVECTOR3 ComMove = pBlock->GetMove();           //ブロックの最小頂点

	bool& bIsWalling = pMyObjX->GetWalling();          //壁にくっついているかどうか

	bool& bSuccessCollisionX = pMyObjX->GetSuccessBlockCollisionX();

	//押し出しに使う値の小数点を切り捨てる（微小な差で失敗しないため）
	MyVtxMax.x = (float)(floor(pMyObjX->GetVtxMax().x));
	MyVtxMax.y = (float)(floor(pMyObjX->GetVtxMax().y));
	MyVtxMax.z = (float)(floor(pMyObjX->GetVtxMax().z));

	MyVtxMin.x = (float)(floor(pMyObjX->GetVtxMin().x));
	MyVtxMin.y = (float)(floor(pMyObjX->GetVtxMin().y));
	MyVtxMin.z = (float)(floor(pMyObjX->GetVtxMin().z));

	ComPos.x = float(floor(pBlock->GetPos().x));
	ComPos.y = float(floor(pBlock->GetPos().y));
	ComPos.z = float(floor(pBlock->GetPos().z));

	ComVtxMax.x = (float)(floor(ComVtxMax.x));
	ComVtxMax.y = (float)(floor(ComVtxMax.y));
	ComVtxMax.z = (float)(floor(ComVtxMax.z));

	ComVtxMin.x = (float)(floor(ComVtxMin.x));
	ComVtxMin.y = (float)(floor(ComVtxMin.y));
	ComVtxMin.z = (float)(floor(ComVtxMin.z));
	// 					//左
	if (Pos.x + MyVtxMax.x > ComPos.x + ComVtxMin.x
		&& PosOld.x + MyVtxMax.x <= ComPos.x + ComVtxMin.x
		&& Pos.y + MyVtxMax.y > ComPos.y + ComVtxMin.y
		&& Pos.y + MyVtxMin.y < ComPos.y + ComVtxMax.y
		&& Pos.z + MyVtxMax.z > ComPos.z + ComVtxMin.z
		&& Pos.z + MyVtxMin.z < ComPos.z + ComVtxMax.z)
	{//対象がブロックの-Xに当たった時の処理
		Pos.x = ComPos.x + ComVtxMin.x - MyVtxMax.x;
		bIsWalling = true;
		bSuccessCollisionX = true;
	}

	//右
	else if (Pos.x + MyVtxMin.x < ComPos.x + ComVtxMax.x
		&& PosOld.x + MyVtxMin.x >= ComPos.x + ComVtxMax.x
		&& Pos.y + MyVtxMax.y > ComPos.y + ComVtxMin.y
		&& Pos.y + MyVtxMin.y < ComPos.y + ComVtxMax.y
		&& Pos.z + MyVtxMax.z > ComPos.z + ComVtxMin.z
		&& Pos.z + MyVtxMin.z < ComPos.z + ComVtxMax.z)
	{//対象がブロックの+Xに当たった時の処理
		Pos.x = ComPos.x + ComVtxMax.x - MyVtxMin.x;
		bIsWalling = true;
		bSuccessCollisionX = true;
	}
}
//======================================================================================================================================

//=======================================================================
//Y方向の押し出し判定を行う
//=======================================================================
void CBlock::ExtrusionCollisionY(CObjectX* pMyObjX, CBlock* pBlock)
{
	D3DXVECTOR3 MyVtxMax = NULL_VECTOR3;             //自分自身の最大頂点
	D3DXVECTOR3 MyVtxMin = NULL_VECTOR3;             //自分自身の最小頂点

	D3DXVECTOR3 ComPos = pBlock->GetPos();             //ブロックの位置
	D3DXVECTOR3 ComVtxMax = pBlock->GetVtxMax();       //ブロックの最大頂点
	D3DXVECTOR3 ComVtxMin = pBlock->GetVtxMin();       //ブロックの最小頂点[
	D3DXVECTOR3 ComMove = pBlock->GetMove();           //ブロックの最小頂点[

	D3DXVECTOR3& Pos = pMyObjX->GetPos();              //位置を取得
	D3DXVECTOR3& Move = pMyObjX->GetMove();             //移動量を取得
	D3DXVECTOR3 PosOld = pMyObjX->GetPosOld();        //1f前の位置を取得
	bool& bDodgeState = pMyObjX->GetDodgeState();      //回避状態かどうかを取得

	bool& bIsLiftLanding = pMyObjX->GetLiftLanding();  //リフトの上に乗っているかどうか

	bool& bIsLanding = pMyObjX->GetLanding();          //地面にいるかどうか

	bool& bIsJumping = pMyObjX->GetJumping();          //ジャンプしているかどうか

	bool& bSuccessBlockCollisionY = pMyObjX->GetSuccessBlockCollisionY();//ブロックとのX方向の当たり判定が成功したかどうか

	//押し出し失敗対策
	MyVtxMax.x = (float)(floor(pMyObjX->GetVtxMax().x));
	MyVtxMax.y = (float)(floor(pMyObjX->GetVtxMax().y));
	MyVtxMax.z = (float)(floor(pMyObjX->GetVtxMax().z));

	MyVtxMin.x = (float)(floor(pMyObjX->GetVtxMin().x));
	MyVtxMin.y = (float)(floor(pMyObjX->GetVtxMin().y));
	MyVtxMin.z = (float)(floor(pMyObjX->GetVtxMin().z));

	ComPos.x = float(floor(pBlock->GetPos().x));
	ComPos.y = float(floor(pBlock->GetPos().y));
	ComPos.z = float(floor(pBlock->GetPos().z));

	ComVtxMax.x = (float)(floor(ComVtxMax.x));
	ComVtxMax.y = (float)(floor(ComVtxMax.y));
	ComVtxMax.z = (float)(floor(ComVtxMax.z));

	ComVtxMin.x = (float)(floor(ComVtxMin.x));
	ComVtxMin.y = (float)(floor(ComVtxMin.y));
	ComVtxMin.z = (float)(floor(ComVtxMin.z));

	CObject*& pLandingObj = pMyObjX->GetLandingObj();

	//上
	if (Pos.x + MyVtxMax.x > ComPos.x + ComVtxMin.x
		&& Pos.x + MyVtxMin.x < ComPos.x + ComVtxMax.x
		&& Pos.y + MyVtxMin.y < ComPos.y + ComVtxMax.y
		&& PosOld.y + MyVtxMin.y - Move.y >= ComPos.y + ComVtxMax.y
		&& Pos.z + MyVtxMax.z > ComPos.z + ComVtxMin.z
		&& Pos.z + MyVtxMin.z < ComPos.z + ComVtxMax.z)
	{//対象の下端がモデルの+Yに当たった時の処理
		switch (this->GetType())
		{
		case BLOCKTYPE04_RIFT:
 			bIsLiftLanding = true;
			pLandingObj = pBlock;
			break;
		case BLOCKTYPE15_FALL:
			if (pMyObjX->GetType() == TYPE_PLAYER)
			{
				bIsLiftLanding = true;
				pLandingObj = pBlock;

				if (m_FallBlock.bFall == false)
				{
					SetUseGravity(true);
					m_FallBlock.bFall = true;//落ちるのを開始
				}
			}
			break;
		default:
			break;
		}

		float fPosY = fabsf(MyVtxMin.y);
		Pos.y = ComPos.y + ComVtxMax.y + fPosY;
		Pos += ComMove;

		bIsLanding = true;
		bSuccessBlockCollisionY = true;
	}
	//下
	else if (Pos.x + MyVtxMax.x > ComPos.x + ComVtxMin.x
		&& Pos.x + MyVtxMin.x < ComPos.x + ComVtxMax.x
		&& Pos.y + MyVtxMax.y > ComPos.y + ComVtxMin.y
		&& PosOld.y + MyVtxMax.y <= ComPos.y + ComVtxMin.y
		&& Pos.z + MyVtxMax.z > ComPos.z + ComVtxMin.z
		&& Pos.z + MyVtxMin.z < ComPos.z + ComVtxMax.z)
	{//対象の下端がモデルの+Yに当たった時の処理
		Pos.y = ComPos.y + ComVtxMin.y - MyVtxMax.y;
		bIsJumping = false;

		if (pMyObjX->GetType() == CObject::TYPE_PLAYER)
		{
			Move.y = 0.0f;
		}

		bSuccessBlockCollisionY = true;
	}
}
//======================================================================================================================================