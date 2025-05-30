//=======================================================================================================================================================================================================
//
// プレイヤーの処理
// Author : Shina Taiyo
//
//=======================================================================================================================================================================================================
#include "player.h"
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "camera.h"
#include "shadow.h"
#include "bullet.h"
#include "emitter.h"
#include <stdio.h>
#include <string.h>
#include "model.h"
#include "scrap.h"
#include "particle.h"
#include "debugproc.h"
#include "gauge.h"
#include "level.h"
#include "battery.h"
#include "enemy.h"
#include "attackmodel.h"
#include "boss.h"
#include "field.h"
#include "wall.h"
#include "dummy.h"
#include "Rader.h"
#include "effect.h"

//=======================================================================================================================================================================================================
// グローバル変数
//=======================================================================================================================================================================================================
LPDIRECT3DTEXTURE9 g_apTexturePlayer[MAX_PLAYERPARTS2][MAX_PLAYER_WEAPON][MAX_MODELMAT] = {};	//テクスチャへのポインタ
LPD3DXMESH g_pMeshPlayer = NULL;//メッシュ（頂点情報）へのポインタ
LPD3DXBUFFER g_pBuffMatPlayer = NULL;//マテリアルへのポインタ
DWORD g_dwNumMatPlayer = 0;//マテリアルの数
D3DXVECTOR3 g_posPlayer;//位置
D3DXVECTOR3 g_rotPlayer;//向き
D3DXMATRIX g_mtxWorldPlayer;//ワールドマトリックス
int g_nDelay = 0;//ディレイカウント
int g_nDelay2 = 0;//ディレイ２
int g_nDecideLevelUpPoint[MAX_LEVEL];//各レベルアップに必要な経験値量を決める
Player g_aPlayer;//プレイヤーの情報

//============================
// プレイヤーパーツ情報関連
//============================
ModelParts g_aPlayerInfo[MAX_PLAYERPARTS2];//プレイヤーの読み込み情報
int g_nCntParts = 0;//プレイヤーのパーツ数
ModelParts g_aWeaponInfo[MAX_PLAYER_WEAPON];//武器の読み込み情報
int g_nCntWeapon = 0;//武器の種類

//============================
// モーション情報関連
//============================
PlayerKey g_aPlayerKey[MAX_PLAYER_WEAPON][MAX_PLAYERPARTS2];//各キー情報
PlayerKeySet g_aPlayerKeySet[MAX_PLAYER_WEAPON][MAX_KEY][MAX_MOTION];//各キーセット情報
PlayerMotionSet g_aPlayerMotionSet[MAX_PLAYER_WEAPON][MAX_MOTION];//モーション情報

//=======================================================================================================================================================================================================
// 初期化処理
//=======================================================================================================================================================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスへのポインタと取得
	D3DXMATERIAL* pMat;//マテリアルのデータへのポインタ
	BYTE* pVtxBuff;//頂点バッファへのポインタ

	//==========================================================================================
	// プレイヤー情報の初期化
	//==========================================================================================
	g_aPlayer.pos = D3DXVECTOR3(0.0f, 0.0f, -2300.0f);
	g_aPlayer.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aPlayer.rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
	g_aPlayer.targetrot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aPlayer.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aPlayer.nIdxShadow = -1;//影のインデックス
	g_aPlayer.nIdxRaderDisplay = -1;//レーダーディスプレイのインデックス
	g_aPlayer.fWidth = 5.0f;
	g_aPlayer.fHeight = 10.0f;
	g_aPlayer.bIsLanding = false;//地面にいるかどうか
	g_aPlayer.nStateOld = 0;//1f前の状態を保存する
	g_aPlayer.State = PLAYERSTATE_NUTORAL;//プレイヤーの状態を表す変数
	//g_aPlayer.vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//プレイヤー全体の最大頂点値
	//g_aPlayer.vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//プレイヤー全体の最小頂点値
	g_aPlayer.nMode = 0;//モードの種類
	g_aPlayer.bMotionFlag = false;//モーションを使用状態にする
	g_aPlayer.nHp = 1500;
	g_aPlayer.bUse = true;
	g_aPlayer.nAttackTime = 0;//攻撃をしている時間を計測する
	g_aPlayer.bCompulsionAttack = false;//強制的に攻撃状態にする
	g_aPlayer.bMove = false;//プレイヤーが動いているかどうかの判定
	g_aPlayer.bInvincible = false;//無敵状態にする
	g_aPlayer.nInvincibleCnt = 0; //無敵状態にするカウント
	g_aPlayer.nNumLandingModel = 0;//どのモデルの上に乗っているか
	g_aPlayer.fMoveX = 0.0f;//X方向の移動量
	g_aPlayer.fMoveZ = 0.0f;//Z方向の移動量
	g_aPlayer.nCntCombo = 0;//コンボ判定カウント
	g_aPlayer.fCombo = 1.0f;//コンボ倍率

	//============================================
	//弱い攻撃を受けた時の変数
	//============================================
	g_aPlayer.bWeakDamageReaction = false;//弱い攻撃を受けたときに色とかを変える
	g_aPlayer.nWeakDamageReactionTime = 0;//弱い攻撃を受けたときにダメージ状態にする時間をカウントする

	//=================================
	//バッテリー充電関連
	//=================================

	//壁充電
	g_aPlayer.bCompulsionHealWall = false;//強制的に壁回復状態にする処理
	g_aPlayer.nHealWallTime = 0;//壁回復をしている時間を計測する
	g_aPlayer.HealWallSaveRot = NULL_VECTOR3;//充電中の向きを保存する
	g_aPlayer.bChargingFlag = false;//充電中にtrueにして、バッテリーの回復をするフラグ
	g_aPlayer.nNumUseWallOutlet = 0;//使用している壁コンセントのモデル番号を記録する

	//床充電
	g_aPlayer.bCompulsionHealGround = false;//強制的に床回復状態にする処理
	g_aPlayer.nHealGroundTime= 0;//床回復をしている時間を計測する
	g_aPlayer.HealGroundSaveRot = NULL_VECTOR3;//床充電中の向きを保存する
	g_aPlayer.bChargingGroundFlag = false;//充電中にtrueにして、バッテリーの回復をするフラグ
	g_aPlayer.nNumUseGroundOutlet = 0;//使用している床コンセントのモデル番号を記録する

	//=======================================================================
	// ステータス
	//=======================================================================

	//=================================
	// ちびロボ本体のステータス
	//=================================
	g_aPlayer.nLevel = 0;				//レベル
	g_aPlayer.nTibiroboScrap = 0;		//ちびロボスクラップ獲得数
	g_aPlayer.nTotalTibiroboScrap = 0;	//ちびロボスクラップ合計獲得数
	g_aPlayer.nMaxLevelUpPoint = 0;		//次のレベルまでに必要なスクラップ数
	g_aPlayer.nBattery = 1000;			//ちびロボのバッテリー（体力）
	g_aPlayer.nAllUpScrap = 0;			//全てのステータス強化スクラップ獲得数
	g_aPlayer.nTotalAllUpScrap = 0;     //全てのステータス強化スクラップ合計獲得数
	g_aPlayer.fSpeed = 0.0f;			//ちびロボの速さ
	g_aPlayer.nWeapon = 0;				//所持している武器
	g_aPlayer.fSuctionScrapRange = 0.0f;//スクラップ吸収範囲

	//=================================
	// 剣のステータス
	//=================================
	g_aPlayer.aSwordInfo.nLevel = 0;			//レベル
	g_aPlayer.aSwordInfo.nSwordScrap = 0;		//剣スクラップ獲得数
	g_aPlayer.aSwordInfo.nTotalSwordScrap = 0;	//剣スクラップ合計獲得数
	g_aPlayer.aSwordInfo.nMaxLevelUpPoint = 0;	//次のレベルまでに必要なスクラップ数
	g_aPlayer.aSwordInfo.nSwordPower = 0;		//剣の攻撃力
	g_aPlayer.aSwordInfo.nSwordAttackSpeed = 0;//剣の攻撃速度

   //=================================
   // ブーメランのステータス
   //=================================
	g_aPlayer.aBoomerangInfo.nLevel = 0;				   //レベル
	g_aPlayer.aBoomerangInfo.nBoomerangScrap = 0;		   //ブーメランスクラップ獲得数
	g_aPlayer.aBoomerangInfo.nTotalBoomerangScrap = 0;	   //ブーメランスクラップ合計獲得数
	g_aPlayer.aBoomerangInfo.nMaxLevelUpPoint = 0;		   //次のレベルまでに必要なスクラップ数
	g_aPlayer.aBoomerangInfo.nBoomerangPower = 0;	       //ブーメランの攻撃力
	g_aPlayer.aBoomerangInfo.fBoomerangRenge = 0.0f;	   //ブーメランの射程
	g_aPlayer.aBoomerangInfo.nBoomerangPiece = 0;		   //ブーメランの個数
	g_aPlayer.aBoomerangInfo.nMaxBoomerangPiece = 0;       //持てるブーメランの最大数
	g_aPlayer.aBoomerangInfo.BoomerangScale = NULL_VECTOR3;//ブーメランの大きさ
	g_aPlayer.aBoomerangInfo.bPossibleBoomerang = false;   //ブーメラン攻撃が可能かどうか

   //=================================
   // スピーカーのステータス
   //=================================
	g_aPlayer.aSpeakerInfo.nLevel = 0;				//レベル
	g_aPlayer.aSpeakerInfo.nSpeakerScrap = 0;		//スピーカースクラップ獲得数
	g_aPlayer.aSpeakerInfo.nTotalSpeakerScrap = 0;	//スピーカースクラップ合計獲得数
	g_aPlayer.aSpeakerInfo.nMaxLevelUpPoint = 0;	//次のレベルまでに必要なスクラップ数
	g_aPlayer.aSpeakerInfo.fSpeakerPower = 0.0f;	//スピーカーの攻撃力
	g_aPlayer.aSpeakerInfo.nPowerConsumption = 0;	//スピーカーの消費電力

	//================================
	// バッテリー
	//================================
	g_aPlayer.nMoveFrame = 0;//動いたフレーム数をカウントする
	g_aPlayer.nAttackCount = 0;//攻撃をしたときにカウントを開始する。
	g_aPlayer.nDamageCountTime = 0;//ダメージを受けている時間をカウントする
	g_aPlayer.bDamageState = false;//ダメージ状態かどうか

	//================================
	//リザルト関係
	//================================
	g_aPlayer.nTotalGetScrap = 0;   //スクラップ合計獲得数
	g_aPlayer.nTotalDefeatEnemy = 0;//合計敵撃破数

    //================================
	//回避関係
	//================================
	g_aPlayer.nDodgeCoolTime = 0;//次回避できるようになるまでのクールタイム
	g_aPlayer.nDodgeTime = 0;    //回避をしている時間
	g_aPlayer.bCompulsionDodge = false;//強制的に回避状態にする
	g_aPlayer.bPossibleDodge = false;  //強制的に回避状態にする

	//================================
	//ベクトル変数
	//================================

	g_aPlayer.fVXaim = 0.0f;
	g_aPlayer.fVZaim = 0.0f;
	g_aPlayer.fVaim = 0.0f;

	//================================
	// モデルの構造体
	//================================
	for (int nCntWeapon = 0; nCntWeapon < MAX_PLAYER_WEAPON; nCntWeapon++)
	{
		for (int nCntPlayerParts = 0; nCntPlayerParts < MAX_PLAYERPARTS2; nCntPlayerParts++)
		{
			g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].pMesh = NULL;
			g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].pBuffMat = NULL;
			g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].dwNumMat = 0;
			g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].mtxWorld = {};
			g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].nIdxModelParent = 0;
			g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].bUse = 0;
			g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].vtxMinModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//頂点の最小値
			g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].vtxMaxModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//頂点の最大値
			g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].bUse = false;
			g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].PartsPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//パーツの位置（ワールド座標）
		

			//==================================
			// プレイヤーパーツの情報(０～１７）
			//==================================
			if (nCntPlayerParts < MAX_PLAYERPARTS2 - 1)
			{
				//Xファイルの読み込み
				D3DXLoadMeshFromX(PLAYER_FILENAME[nCntPlayerParts],
					D3DXMESH_SYSTEMMEM,
					pDevice,
					NULL,
					&g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].pBuffMat,
					NULL,
					&g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].dwNumMat,
					&g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].pMesh);

				//マテリアルデータへのポインタを取得
				pMat = (D3DXMATERIAL*)g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].pBuffMat->GetBufferPointer();

				for (int nCntMat = 0; nCntMat < (int)g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].dwNumMat; nCntMat++)
				{
					g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].DiffUse[nCntMat] = pMat[nCntMat].MatD3D.Diffuse;
					g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].FormarDiffUse[nCntMat] = pMat[nCntMat].MatD3D.Diffuse;//元の色合い
				}

				//頂点数の取得
				int nNumVtx = g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].pMesh->GetNumVertices();

				//頂点フォーマットのサイズを取得
				DWORD sizeFVF = D3DXGetFVFVertexSize(g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].pMesh->GetFVF());

				//頂点バッファのロック
				g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

				for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
				{
					//頂点座標の代入
					D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;


					//================================
					// プレイヤーの最大最小を決める
					//================================

					//モデルの頂点座標の最小値と最大値を取得
					if (vtx.x < g_aPlayer.vtxMin.x)
					{//X座標の最小値
						g_aPlayer.vtxMin.x = vtx.x;
					}
					if (vtx.y < g_aPlayer.vtxMin.y)
					{//Y座標の最小値
						g_aPlayer.vtxMin.y = vtx.y;
					}
					if (vtx.z < g_aPlayer.vtxMin.z)
					{//Z座標の最小値
						g_aPlayer.vtxMin.z = vtx.z;
					}
					if (vtx.x > g_aPlayer.vtxMax.x)
					{//X座標の最大値
						g_aPlayer.vtxMax.x = vtx.x;
					}
					if (vtx.y > g_aPlayer.vtxMax.y)
					{//Y座標の最大値
						g_aPlayer.vtxMax.y = vtx.y;
					}
					if (vtx.z > g_aPlayer.vtxMax.z)
					{//Z座標の最大値
						g_aPlayer.vtxMax.z = vtx.z;
					}

					//====================================
					// パーツごとの最大最小を決める
					//====================================

					if (vtx.x < g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].vtxMinModel.x)
					{//X座標の最小値
						g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].vtxMinModel.x = vtx.x;
					}
					if (vtx.y < g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].vtxMinModel.y)
					{//X座標の最小値
						g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].vtxMinModel.y = vtx.y;
					}
					if (vtx.z < g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].vtxMinModel.z)
					{//X座標の最小値
						g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].vtxMinModel.z = vtx.z;
					}

					if (vtx.x > g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].vtxMaxModel.x)
					{//X座標の最大値
						g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].vtxMaxModel.x = vtx.x;
					}
					if (vtx.y > g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].vtxMaxModel.y)
					{//X座標の最大値
						g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].vtxMaxModel.y = vtx.y;
					}
					if (vtx.z > g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].vtxMaxModel.z)
					{//X座標の最大値
						g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].vtxMaxModel.z = vtx.z;
					}

					//頂点フォーマットのサイズ分ポインタを進める
					pVtxBuff += sizeFVF;
				}

				//種類ごとのサイズを求める
				g_aPlayer.Size =
				{
					g_aPlayer.vtxMax.x - g_aPlayer.vtxMin.x,
					g_aPlayer.vtxMax.y - g_aPlayer.vtxMin.y,
					g_aPlayer.vtxMax.z - g_aPlayer.vtxMin.z
				};

				//頂点バッファのアンロック
				g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].pMesh->UnlockVertexBuffer();
			}

			//=========================================================================================
			// プレイヤーの武器の情報（プレイヤーのパーツ数(18) + 武器３つ(パーツ数 + 1) * MAX_WEAPON)
			//=========================================================================================
			else if (nCntPlayerParts == MAX_PLAYERPARTS)
			{
				g_aPlayer.aWeapon[nCntWeapon].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_aPlayer.aWeapon[nCntWeapon].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_aPlayer.aWeapon[nCntWeapon].bUse = 0;
				g_aPlayer.aWeapon[nCntWeapon].nType = 0;

				//Xファイルの読み込み
				D3DXLoadMeshFromX(WEAPON_FILENAME[nCntWeapon],
					D3DXMESH_SYSTEMMEM,
					pDevice,
					NULL,
					&g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].pBuffMat,
					NULL,
					&g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].dwNumMat,
					&g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].pMesh);

				//マテリアルデータへのポインタを取得
				pMat = (D3DXMATERIAL*)g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].pBuffMat->GetBufferPointer();

				for (int nCntMat = 0; nCntMat < (int)g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].dwNumMat; nCntMat++)
				{
					g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].DiffUse[nCntMat] = pMat[nCntMat].MatD3D.Diffuse;
					g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].FormarDiffUse[nCntMat] = pMat[nCntMat].MatD3D.Diffuse;//元の色合い

				}

				//頂点数の取得
				int nNumVtx = g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].pMesh->GetNumVertices();

				//頂点フォーマットのサイズを取得
				DWORD sizeFVF = D3DXGetFVFVertexSize(g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].pMesh->GetFVF());

				//頂点バッファのロック(実際に読み込んでいるモデルのポインタをaWeaponに渡す）
				g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

				for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
				{
					//頂点座標の代入
					D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

					//モデルの頂点座標の最小値と最大値を取得
					if (vtx.x < g_aPlayer.aWeapon[nCntWeapon].vtxMin.x)
					{//X座標の最小値
						g_aPlayer.aWeapon[nCntWeapon].vtxMin.x = vtx.x;
					}
					if (vtx.y < g_aPlayer.aWeapon[nCntWeapon].vtxMin.y)
					{//Y座標の最小値
						g_aPlayer.aWeapon[nCntWeapon].vtxMin.y = vtx.y;
					}
					if (vtx.z < g_aPlayer.aWeapon[nCntWeapon].vtxMin.z)
					{//Z座標の最小値
						g_aPlayer.aWeapon[nCntWeapon].vtxMin.z = vtx.z;
					}
					if (vtx.x > g_aPlayer.aWeapon[nCntWeapon].vtxMax.x)
					{//X座標の最大値
						g_aPlayer.aWeapon[nCntWeapon].vtxMax.x = vtx.x;
					}
					if (vtx.y > g_aPlayer.aWeapon[nCntWeapon].vtxMax.y)
					{//Y座標の最大値
						g_aPlayer.aWeapon[nCntWeapon].vtxMax.y = vtx.y;
					}
					if (vtx.z > g_aPlayer.aWeapon[nCntWeapon].vtxMax.z)
					{//Z座標の最大値
						g_aPlayer.aWeapon[nCntWeapon].vtxMax.z = vtx.z;
					}

					//頂点フォーマットのサイズ分ポインタを進める
					pVtxBuff += sizeFVF;
				}

				//種類ごとのサイズを求める
				g_aPlayer.aWeapon[nCntWeapon].Size =
				{
					g_aPlayer.aWeapon[nCntWeapon].vtxMax.x - g_aPlayer.aWeapon[nCntWeapon].vtxMin.x,
					g_aPlayer.aWeapon[nCntWeapon].vtxMax.y - g_aPlayer.aWeapon[nCntWeapon].vtxMin.y,
					g_aPlayer.aWeapon[nCntWeapon].vtxMax.z - g_aPlayer.aWeapon[nCntWeapon].vtxMin.z
				};

				//頂点バッファのアンロック
				g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].pMesh->UnlockVertexBuffer();
			}

			//マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].dwNumMat; nCntMat++)
			{
				//テクスチャファイルが存在する
				if (pMat[nCntMat].pTextureFilename != NULL)
				{
					//テクスチャの読み込み
					D3DXCreateTextureFromFile(pDevice,
						pMat[nCntMat].pTextureFilename,
						&g_apTexturePlayer[nCntPlayerParts][nCntWeapon][nCntMat]);
				}
			}
		}

		//モーション情報の読み込み
		LoadMotion(nCntWeapon);
	}

	//=================================
	// 各レベルアップに必要な経験値量
	//=================================
	for (int nCntLevelUpPoint = 0; nCntLevelUpPoint < MAX_LEVEL; nCntLevelUpPoint++)
	{
		g_nDecideLevelUpPoint[nCntLevelUpPoint] = 0;
	}

	//=================================
	// レベルアップ必要経験値決定処理
	//=================================
	FILE* pFile;//ファイルポインタ
	char aString[100] = {};//文字列を読み込むための変数
	int nLevelNum = 0;//経験値の量を決めているレベル番号

	pFile = fopen(LEVEL_TXT, "r");

	if (pFile != NULL)//ファイルが開いているかどうか
	{
		while (1)
		{
			fscanf(pFile, "%s", &aString[0]);

			if (aString[0] == '#')
			{
				fgets(&aString[0], 100, pFile);
				continue;
			}
			else if (strcmp(&aString[0], "ENDSCRIPT") == 0)
			{
				fclose(pFile);
				break;//行の最後でこの文字列を判定させることにより、ループを終わらせる
			}
			else if (strcmp(&aString[0], "LEVELSET") == 0 && nLevelNum < MAX_LEVEL)
			{
				while (1)
				{
					fscanf(pFile, "%s", &aString[0]);
					if (aString[0] == '#')
					{
						fgets(&aString[0], 100, pFile);
						continue;
					}
					else  if (strcmp(&aString[0], "EXP") == 0)
					{
						fscanf(pFile, "%d", &g_nDecideLevelUpPoint[nLevelNum]);//インデックスの種類を決める
					}
					else if (strcmp(&aString[0], "ENDLEVELSET") == 0)
					{
						nLevelNum++;
						break;
					}
				}
			}
		}
	}

	//変数
	g_nDelay = 0;//ディレイカウント
	g_nDelay2 = 0;//ディレイカウント
}

//=======================================================================================================================================================================================================
// 終了処理
//=======================================================================================================================================================================================================
void UninitPlayer(void)
{
	for (int nCntParts = 0; nCntParts < MAX_PLAYERPARTS2; nCntParts++)
	{
		for (int nCntWeapon = 0; nCntWeapon < MAX_PLAYER_WEAPON; nCntWeapon++)
		{
			//メッシュの破棄
			if (g_aPlayer.aModel[nCntParts][nCntWeapon].pMesh != NULL)
			{
				g_aPlayer.aModel[nCntParts][nCntWeapon].pMesh->Release();
				g_aPlayer.aModel[nCntParts][nCntWeapon].pMesh = NULL;
			}
			//マテリアルの破棄
			if (g_aPlayer.aModel[nCntParts][nCntWeapon].pBuffMat != NULL)
			{
				g_aPlayer.aModel[nCntParts][nCntWeapon].pBuffMat->Release();
				g_aPlayer.aModel[nCntParts][nCntWeapon].pBuffMat = NULL;
			}

			//テクスチャの破棄
			for (int nCntMat = 0; nCntMat < MAX_PLAYERMAT; nCntMat++)
			{
				if (g_apTexturePlayer[nCntParts][nCntWeapon][nCntMat] != NULL)
				{
					g_apTexturePlayer[nCntParts][nCntWeapon][nCntMat]->Release();
					g_apTexturePlayer[nCntParts][nCntWeapon][nCntMat] = NULL;
				}
			}
		}
	}

}

//=======================================================================================================================================================================================================
// 更新処理
//=======================================================================================================================================================================================================
void UpdatePlayer(void)
{
	Camera* pCamera = GetCamera();	//カメラの情報を取得
	Enemy* pEnemy = GetEnemy();		//敵の情報を取得
	Boss* pBoss = GetBoss();		//ボスの情報を取得
	Wall* pWall = GetWall();		//壁の情報を取得

	if (GetMode() == MODE_GAME)
	{//ゲーム画面の場合
		g_nDelay2++;//ゲームモードの時だけカウントをする
	}

	if (GetMode() != MODE_TITLE && pCamera->nMode == CAMERAMODE_REVOLUTION)
	{//タイトル画面ではない場合

		//============================
		// 1f前の状態を保存する
		//============================
		g_aPlayer.nStateOld = g_aPlayer.State;

		//========================================================================
		// プレイヤーのパーツの位置を取得するため、マトリックスの計算を開始する
		//========================================================================
		CalculateMatrixPlayer();

		g_nDelay++;//ディレイカウント（何フレームに一回発動したい処理などをする時に使っている。）

		//=========================================================================================
		//最初のフレームの時だけ、どのモデルの上に乗っているか分からないので、値を-1にする
		//=========================================================================================
		if (g_nDelay == 1)
		{
			g_aPlayer.nNumLandingModel = -1;
			g_aPlayer.bPossibleDodge = true;

			//影の設定
			g_aPlayer.nIdxShadow = SetShadow();

			g_aPlayer.nIdxRaderDisplay = SetRaderDisplay(RADERDISPLAY00_PLAYER);
		}

		if (g_aPlayer.pos.y <= 0.0f)
		{//地面についたら
			g_aPlayer.pos.y = 0.0f;
			g_aPlayer.bIsLanding = true;
		}

		if (GetKeyboardTrigger(DIK_F2))
		{//F2が押された場合
			g_aPlayer.nMode++;

			if (g_aPlayer.nMode > PLAYERMODE_EDIT)
			{
				g_aPlayer.nMode = PLAYERMODE_NORMAL;
			}
		}

		//=======================================================================
		// プレイヤーの動き方のを決める（普通モードとエディットモード）
		//=======================================================================
		if (g_aPlayer.nMode == PLAYERMODE_NORMAL)
		{//普通モードの場合
			if (g_aPlayer.bIsLanding == false)
			{//プレイヤーが地面に立っていない場合
				g_aPlayer.move.y += -1.2f;
			}
			else
			{//プレイヤーが地面に立っている場合
				g_aPlayer.move.y = 0.0f;

				if (g_aPlayer.bIsLanding == true)
				{
					Model* pModel = GetModel();		//モデルの情報を取得

					//===================================================
					//リフトの上に乗り続ける
					//===================================================
					for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++, pModel++)
					{
						if (pModel->bUse == true && g_aPlayer.nNumLandingModel == nCntModel)
						{
							g_aPlayer.pos.y = pModel->pos.y + pModel->vtxMax.y;
						}
					}
				}
			}
		}
		else if (g_aPlayer.nMode == PLAYERMODE_EDIT)
		{//エディットモードの場合
			if (GetKeyboardPress(DIK_UP))
			{//↑キーが押された場合
				g_aPlayer.pos.y += 5.0f;
			}
			else if (GetKeyboardPress(DIK_DOWN))
			{//↓キーが押された場合
				g_aPlayer.pos.y += -5.0f;
			}
		}

		//===========================================================================================
		// プレイヤーの移動処理
		//===========================================================================================
		if (pCamera->nMode == CAMERAMODE_REVOLUTION)
		{
			if (!g_aPlayer.bCompulsionDodge)
			{//回避状態じゃないときだけ移動の入力の受け付けを行う。
				g_aPlayer.fMoveX = 0.0f;
				g_aPlayer.fMoveZ = 0.0f;
				if (GetKeyboardPress(DIK_W) || (float)(GetStick().abAnglePress[STICKTYPE_LEFT][STICKANGLE_UP]) > 0.1f)
				{
					g_aPlayer.fMoveZ = 1.0f;
				}
				else if (GetKeyboardPress(DIK_S) || (float)(GetStick().abAnglePress[STICKTYPE_LEFT][STICKANGLE_DOWN]) > 0.1f)
				{
					g_aPlayer.fMoveZ = -1.0f;
				}

				if (GetKeyboardPress(DIK_D) || (float)(GetStick().abAnglePress[STICKTYPE_LEFT][STICKANGLE_RIGHT]) > 0.1f)
				{
					g_aPlayer.fMoveX = 1.0f;
				}
				else if (GetKeyboardPress(DIK_A) || (float)(GetStick().abAnglePress[STICKTYPE_LEFT][STICKANGLE_LEFT]) > 0.1f)
				{
					g_aPlayer.fMoveX = -1.0f;
				}
			}

			//============================
			// 移動ボタンを押していたら
			//============================
			if (g_aPlayer.fMoveX != 0.0f || g_aPlayer.fMoveZ != 0.0f)
			{
				g_aPlayer.bMove = true;//移動状態
			}
			else
			{
				g_aPlayer.bMove = false;//待機状態
			}

			//================================
			//回避処理
			//================================
			float fDodgeSpeed = 1.0f;

			if (g_aPlayer.bMove == true)
			{
				if (GetJoypadTrigger(JOYKEY_A) == true && g_aPlayer.bPossibleDodge == true && g_aPlayer.bCompulsionHealGround == false &&
					g_aPlayer.bCompulsionHealWall == false)
				{
					PlaySound(SOUND_LABEL_SE_DODGE_000);
					g_aPlayer.bCompulsionDodge = true;
					g_aPlayer.bPossibleDodge = false;
				}
			}

			if (g_aPlayer.bCompulsionDodge == true)
			{
				for (int nCntWeapon = 0; nCntWeapon < MAX_PLAYER_WEAPON; nCntWeapon++)
				{
					g_aPlayer.aModel[19][nCntWeapon].bUse = false;
				}

				g_aPlayer.nDodgeTime++;
				fDodgeSpeed = 5.0f;

				if (g_aPlayer.nDodgeTime >= 10)
				{
					g_aPlayer.nDodgeTime = 0;
					g_aPlayer.bCompulsionDodge = false;
				}
			}
			else
			{
				for (int nCntWeapon = 0; nCntWeapon < MAX_PLAYER_WEAPON; nCntWeapon++)
				{
					g_aPlayer.aModel[19][nCntWeapon].bUse = true;
				}
			}

			if (g_aPlayer.bCompulsionDodge == false && g_aPlayer.bPossibleDodge == false)
			{
				g_aPlayer.nDodgeCoolTime++;

				if (g_aPlayer.nDodgeCoolTime >= 80)
				{
					g_aPlayer.bPossibleDodge = true;
					g_aPlayer.nDodgeCoolTime = 0;
				}
			}

			//================================
			// 移動処理開始
			//================================
			if (g_aPlayer.bMove == true && g_aPlayer.bDamageState == false && g_aPlayer.bCompulsionHealWall == false && g_aPlayer.bCompulsionHealGround == false)
			{//（ダメージ、壁充電状態、床充電）状態じゃなければ移動を開始する。
				g_aPlayer.State = PLAYERSTATE_MOVE;

				//移動をしたフレーム数t
				g_aPlayer.nMoveFrame++;

				g_aPlayer.move.x = sinf(atan2f(g_aPlayer.fMoveX, g_aPlayer.fMoveZ) + pCamera->rot.y) * g_aPlayer.fSpeed * fDodgeSpeed;
				g_aPlayer.move.z = cosf(atan2f(g_aPlayer.fMoveX, g_aPlayer.fMoveZ) + pCamera->rot.y) * g_aPlayer.fSpeed * fDodgeSpeed;

				g_aPlayer.move.x += (0.0f - g_aPlayer.move.x) * 0.5f;
				g_aPlayer.move.z += (0.0f - g_aPlayer.move.z) * 0.5f;

				if (g_aPlayer.nMoveFrame % 20 == 0)
				{
					/*PlaySound(SOUND_LABEL_SE_TIBIROBOWALK);*/
				}

				g_aPlayer.targetrot.y = atan2f(g_aPlayer.fMoveX, g_aPlayer.fMoveZ) + pCamera->rot.y + D3DX_PI;

				g_aPlayer.rot.y += ((g_aPlayer.targetrot.y) - g_aPlayer.rot.y) * 0.5f;
			}
			else
			{
				g_aPlayer.State = PLAYERSTATE_NUTORAL;
				g_aPlayer.move.x = 0.0f;
				g_aPlayer.move.z = 0.0f;
			}
		}

		//============================
		//ボスにベクトルを向ける
		//============================
		g_aPlayer.fVXaim = pBoss->pos.x - g_aPlayer.pos.x;
		g_aPlayer.fVZaim = pBoss->pos.z - g_aPlayer.pos.z;
		g_aPlayer.fVaim = atan2f(g_aPlayer.fVXaim, g_aPlayer.fVZaim);

		g_aPlayer.posOld = g_aPlayer.pos;
		g_aPlayer.pos += g_aPlayer.move;

		g_aPlayer.nNumLandingModel = -1;//この時点では、モデルの上に乗っているか分からないので、番号を-1にする

		g_aPlayer.bIsLanding = false;//地面についているかどうか	

		g_aPlayer.bIsLanding = bCollisionModel(&g_aPlayer.pos, &g_aPlayer.posOld, g_aPlayer.Size, g_aPlayer.nNumLandingModel);

		if (g_aPlayer.nMoveFrame  == 20)
		{
			SubBattery(1);
			g_aPlayer.nMoveFrame = 0;
		}

		//=================================================
		//強制的に回避状態にする処理（移動より優先度高）
		//=================================================
		if (g_aPlayer.bCompulsionDodge == true)
		{
			g_aPlayer.State = PLAYERSTATE_DODGE;
		}

		//=============================
		// 攻撃ボタン入力処理
		//=============================
		if (g_aPlayer.bDamageState == false)
		{
			if (g_aPlayer.State != PLAYERSTATE_HEALGROUND && g_aPlayer.State != PLAYERSTATE_HEALWALL
				&& g_aPlayer.bCompulsionHealWall == false && g_aPlayer.bCompulsionHealGround == false)
			{
			    //剣攻撃
				if (GetKeyboardTrigger(DIK_K) == true || GetJoypadTrigger(JOYKEY_X) == true)
				{
					g_aPlayer.State = PLAYERSTATE_ATTACK;//攻撃状態
					g_aPlayer.nWeapon = 0;//剣を所持
					g_aPlayer.nAttackCount += 1;//攻撃カウント＋１
					g_aPlayer.bCompulsionAttack = true;//強制的に攻撃状態にする

					if (g_aPlayer.nAttackTime == 0)
					{
						SubBattery(1);
					}
				}
				//ブーメラン攻撃
				else if (GetJoypadTrigger(JOYKEY_Y) == true && g_aPlayer.aBoomerangInfo.bPossibleBoomerang == true)
				{
					g_aPlayer.State = PLAYERSTATE_ATTACK;//攻撃状態
					g_aPlayer.nWeapon = 1;//ブーメランを所持
					g_aPlayer.bCompulsionAttack = true;//強制的に攻撃状態にする
					g_aPlayer.aModel[19][g_aPlayer.nWeapon].bUse = false;//武器を非表示
				}
				//スピーカー攻撃
				else if (GetJoypadTrigger(JOYKEY_B) == true)
				{
					g_aPlayer.State = PLAYERSTATE_ATTACK;//攻撃状態
					g_aPlayer.nWeapon = 2;//スピーカーを所持
					g_aPlayer.bCompulsionAttack = true;//強制的に攻撃状態にする

					if (g_aPlayer.nAttackTime == 0)
					{
						PlaySound(SOUND_LABEL_SE_SPEAKERWAVE_000);
					}
				}
			}
		}

		//======================================
		//攻撃強制状態だったら
		//======================================
		if (g_aPlayer.bCompulsionAttack)
		{//攻撃状態の場合
			g_aPlayer.State = PLAYERSTATE_ATTACK;
		}
		if (g_aPlayer.State == PLAYERSTATE_ATTACK)
		{
			g_aPlayer.nAttackTime++;
		}

	//============================================================================
	//プレイヤー回復（充電）処理
	//============================================================================
		Model* pModel = GetModel();		//モデルの情報を取得

		for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++, pModel++)
		{
			if (pModel->bUse == true && pModel->nType == 41)
			{//壁コンセントのモデルを使用していたら

				//===========================================
				//プレイヤーと壁コンセントとの距離を取る
				//===========================================
				float fLength = sqrtf((g_aPlayer.pos.x - pModel->pos.x) * (g_aPlayer.pos.x - pModel->pos.x) +
					(g_aPlayer.pos.y - pModel->pos.y) * (g_aPlayer.pos.y - pModel->pos.y) +
					(g_aPlayer.pos.z - pModel->pos.z) * (g_aPlayer.pos.z - pModel->pos.z) / 3.0f);

				if (fLength <= 100.0f && GetJoypadTrigger(JOYKEY_A) == true && g_aPlayer.bCompulsionHealWall == false)
				{//範囲内でAボタンが押された場合
					g_aPlayer.bCompulsionHealWall = true;
					g_aPlayer.pos = D3DXVECTOR3(pModel->pos.x + sinf(pModel->rot.y + D3DX_PI) * 37.0f, 10.0f, pModel->pos.z + cosf(pModel->rot.y + D3DX_PI) * 37.0f);
					g_aPlayer.HealWallSaveRot.y = pModel->rot.y + D3DX_PI;
					g_aPlayer.nNumUseWallOutlet = nCntModel;//使用している壁コンセントのモデル番号を記録する
					g_aPlayer.bChargingFlag = true;//壁充電中のフラグをtrueにする
				}
			}
			if (pModel->bUse == true && pModel->nType == 19)
			{//床コンセントのモデルを使用していたら

				//===========================================
				//プレイヤーと床コンセントとの距離を取る
				//===========================================
				float fLength = sqrtf((g_aPlayer.pos.x - pModel->pos.x) * (g_aPlayer.pos.x - pModel->pos.x) +
					(g_aPlayer.pos.y - pModel->pos.y) * (g_aPlayer.pos.y - pModel->pos.y) +
					(g_aPlayer.pos.z - pModel->pos.z) * (g_aPlayer.pos.z - pModel->pos.z) / 3);

				if (fLength <= 100.0f && GetJoypadTrigger(JOYKEY_A) == true && g_aPlayer.bCompulsionHealGround == false)
				{//範囲内でAボタンが押された場合
					g_aPlayer.bCompulsionHealGround = true;
					g_aPlayer.pos = D3DXVECTOR3(pModel->pos.x + sinf(pModel->rot.y + D3DX_PI) * 30.0f + 10.0f, 0.0f, pModel->pos.z + cosf(pModel->rot.y + D3DX_PI) * 30.0f);
					g_aPlayer.HealGroundSaveRot.y = pModel->rot.y + D3DX_PI;
					g_aPlayer.nNumUseGroundOutlet = nCntModel;//使用している床コンセントのモデル番号を記録する
					g_aPlayer.bChargingGroundFlag = true;//床充電中のフラグをtrueにする
				}
			}
		}

		//==================================================
		//ステート強制フラグがtrueの間は、その状態に固定
		//==================================================
		if (g_aPlayer.bCompulsionHealWall)
		{
			g_aPlayer.State = PLAYERSTATE_HEALWALL;//壁回復状態
			g_aPlayer.rot.y = g_aPlayer.HealWallSaveRot.y;
			g_aPlayer.aModel[19][g_aPlayer.nWeapon].bUse = false;//武器を非表示
		}

		if (g_aPlayer.bCompulsionHealGround)
		{
			g_aPlayer.State = PLAYERSTATE_HEALGROUND;//床回復状態
			g_aPlayer.rot.y = g_aPlayer.HealGroundSaveRot.y;
			g_aPlayer.aModel[19][g_aPlayer.nWeapon].bUse = false;//武器を非表示
		}

		//=======================================================================================================================

		//===============================================================================================================================================
		// 攻撃の当たり判定処理(攻撃ボタン押した時の所でやると、ボタン押している時しか反応しないので、現在使用している武器がどれかで判断したものを書く。
		//===============================================================================================================================================

		//================================================
		// プレイヤーの使用している武器が「剣」だったら
		//================================================
		D3DXVECTOR3 CollisionVertex = D3DXVECTOR3(sinf(-D3DX_PI * g_aPlayer.rot.y) * 35.0f + g_aPlayer.aModel[18][0].PartsPos.x,
			g_aPlayer.aModel[18][0].PartsPos.y,
			cosf(-D3DX_PI * g_aPlayer.rot.y) * 35.0f + g_aPlayer.aModel[18][0].PartsPos.z);

		g_aPlayer.aModel[18][0].PartsPos = CollisionVertex;

		//==================================
		//充電状態なら絶対攻撃をしない
		//==================================
		if (g_aPlayer.State != PLAYERSTATE_HEALGROUND && g_aPlayer.State != PLAYERSTATE_HEALWALL)
		{
			if (g_aPlayer.State == PLAYERSTATE_ATTACK && g_aPlayer.nWeapon == 0)
			{
				if (g_aPlayer.nAttackTime == 10 || g_aPlayer.nAttackTime == 26)
				{
					PlaySound(SOUND_LABEL_SE_SWORDSWING_000);
				}

				//========================
				//敵との当たり判定
				//========================
				for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				{
					if (pEnemy->bUse)
					{//敵が使用されている場合
						float fLength = sqrtf((g_aPlayer.aModel[18][0].PartsPos.x - pEnemy->pos.x) * (g_aPlayer.aModel[18][0].PartsPos.x - pEnemy->pos.x) +
							(g_aPlayer.aModel[18][0].PartsPos.z - pEnemy->pos.z) * (g_aPlayer.aModel[18][0].PartsPos.z - pEnemy->pos.z) +
							(g_aPlayer.aModel[18][0].PartsPos.y - pEnemy->pos.y) * (g_aPlayer.aModel[18][0].PartsPos.y - pEnemy->pos.y)) / 3;

						if (fLength <= 35.0f)
						{
							if ((g_aPlayer.nAttackTime >= 7 && g_aPlayer.nAttackTime <= 17) || (g_aPlayer.nAttackTime >= 20 && g_aPlayer.nAttackTime <= 34))
							{
								SetDamageEnemy(g_aPlayer.aSwordInfo.nSwordPower, nCntEnemy, PLAYERATTACK00_SWORD, 0);
							}
						}
					}
				}

				//==========================
				//ボスとの当たり判定
				//==========================
				if (pBoss->bUse)
				{//ボスが使用されている場合
					float fLengthBoss = sqrtf((g_aPlayer.aModel[18][0].PartsPos.x - pBoss->pos.x) * (g_aPlayer.aModel[18][0].PartsPos.x - pBoss->pos.x) +
						(g_aPlayer.aModel[18][0].PartsPos.z - pBoss->pos.z) * (g_aPlayer.aModel[18][0].PartsPos.z - pBoss->pos.z) +
						(g_aPlayer.aModel[18][0].PartsPos.y - pBoss->pos.y) * (g_aPlayer.aModel[18][0].PartsPos.y - pBoss->pos.y)) / 3;

					if (fLengthBoss <= 35.0f)
					{
						if ((g_aPlayer.nAttackTime >= 7 && g_aPlayer.nAttackTime <= 17) || (g_aPlayer.nAttackTime >= 20 && g_aPlayer.nAttackTime <= 34))
						{
							SetDamageBoss(g_aPlayer.aSwordInfo.nSwordPower, PLAYERATTACK00_SWORD, 0);
						}
					}
				}

				//==============================
				//ダミーとの当たり判定
				//==============================
				Dummy* pDummy = GetDummy();	//ダミーの情報を取得

				if (pDummy->bUse)
				{//ダミーが使用されている場合
					float fLengthDummy = sqrtf((g_aPlayer.aModel[18][0].PartsPos.x - pDummy->pos.x) * (g_aPlayer.aModel[18][0].PartsPos.x - pDummy->pos.x) +
						(g_aPlayer.aModel[18][0].PartsPos.z - pDummy->pos.z) * (g_aPlayer.aModel[18][0].PartsPos.z - pDummy->pos.z) +
						(g_aPlayer.aModel[18][0].PartsPos.y - pDummy->pos.y) * (g_aPlayer.aModel[18][0].PartsPos.y - pDummy->pos.y)) / 3;

					if (fLengthDummy <= 35.0f)
					{
						if ((g_aPlayer.nAttackTime >= 7 && g_aPlayer.nAttackTime <= 17) || (g_aPlayer.nAttackTime >= 20 && g_aPlayer.nAttackTime <= 34))
						{
							DamageDummy(g_aPlayer.aSwordInfo.nSwordPower, PLAYERATTACK00_SWORD);
						}
					}
				}
			}


			//========================================================
			// プレイヤーの装備している武器が「ブーメラン」だったら
			//========================================================
			if (g_aPlayer.aBoomerangInfo.nBoomerangPiece == g_aPlayer.aBoomerangInfo.nMaxBoomerangPiece)
			{
				g_aPlayer.aBoomerangInfo.bPossibleBoomerang = true;
			}
			else
			{
				g_aPlayer.aBoomerangInfo.bPossibleBoomerang = false;
			}

			if (g_aPlayer.State == PLAYERSTATE_ATTACK && g_aPlayer.nWeapon == 1 && g_aPlayer.aBoomerangInfo.bPossibleBoomerang == true)
			{//持っているブーメランが最大数と同じ数になったら攻撃できるようにする。（一気に投げる）
				PlaySound(SOUND_LABEL_SE_BOOMERANGTHROW_000);
				SubBattery(1);//攻撃するたび、電力が１減る

				g_aPlayer.aBoomerangInfo.nBoomerangPiece = 0;//全てのブーメランを一気に投げる
				float fRot = 0.0f;//向きを増やす
				for (int nCntBoomerang = 0; nCntBoomerang < g_aPlayer.aBoomerangInfo.nMaxBoomerangPiece; nCntBoomerang++)
				{
					if (nCntBoomerang % 2 == 0)
					{
						SetAttackModel(ATTACKMODEL00_BOOMERANG, 600, D3DXVECTOR3(g_aPlayer.pos.x, g_aPlayer.pos.y + 50.0f, g_aPlayer.pos.z), D3DXVECTOR3(sinf(g_aPlayer.rot.y + D3DX_PI + fRot) * 20.0f, 0.0f, cosf(g_aPlayer.rot.y + D3DX_PI + fRot) * 20.0f),
							NULL_VECTOR3, g_aPlayer.aBoomerangInfo.BoomerangScale, 0.0f, g_aPlayer.rot.y + D3DX_PI, 0.15f, g_aPlayer.pos, ATTACKMODELBEHAVIOR01_ANOTHER);
					}
					else
					{
						SetAttackModel(ATTACKMODEL00_BOOMERANG, 600, D3DXVECTOR3(g_aPlayer.pos.x, g_aPlayer.pos.y + 50.0f, g_aPlayer.pos.z), D3DXVECTOR3(sinf(g_aPlayer.rot.y + D3DX_PI - fRot) * 20.0f, 0.0f, cosf(g_aPlayer.rot.y + D3DX_PI - fRot) * 20.0f),
							NULL_VECTOR3, g_aPlayer.aBoomerangInfo.BoomerangScale, 0.0f, g_aPlayer.rot.y + D3DX_PI, 0.15f, g_aPlayer.pos, ATTACKMODELBEHAVIOR01_ANOTHER);
					}
					if (nCntBoomerang % 2 == 0)
					{//偶数だったら
						fRot += 0.1f;
					}
				}

			}

			//===============================================================
			// プレイヤーの装備している武器が「スピーカー」だったら
			//===============================================================
			if (g_aPlayer.State == PLAYERSTATE_ATTACK && g_aPlayer.nWeapon == 2)
			{
				float fSpace = 0.5f;
				float fWide = 0.5f;

				if (g_aPlayer.nAttackTime == 2)
				{
					SubBattery(g_aPlayer.aSpeakerInfo.nPowerConsumption);//スピーカーの電力消費量分、バッテリー減少

					for (int nCntAttack = 0; nCntAttack < 3; nCntAttack++)
					{
						SetAttackModel(ATTACKMODEL01_WAVE, 30, D3DXVECTOR3(g_aPlayer.aModel[18][2].PartsPos.x, g_aPlayer.aModel[18][2].PartsPos.y, g_aPlayer.aModel[18][2].PartsPos.z),
							D3DXVECTOR3(sinf(g_aPlayer.rot.y + D3DX_PI - fSpace + (fSpace * nCntAttack)) * 10.0f, 0.0f, cosf(g_aPlayer.rot.y + D3DX_PI - fSpace + (fSpace * nCntAttack)) * 10.0f),
							D3DXVECTOR3(0.0f, g_aPlayer.rot.y - fWide + (fWide * nCntAttack), 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 0.0f, 0.0f, 0.3f, NULL_VECTOR3, 0);
					}
				}

				if (g_aPlayer.nAttackTime % 10 == 0)
				{
					for (int nCntAttack = 0; nCntAttack < 3; nCntAttack++)
					{
						SetAttackModel(ATTACKMODEL01_WAVE, 30, D3DXVECTOR3(g_aPlayer.aModel[18][2].PartsPos.x, g_aPlayer.aModel[18][2].PartsPos.y, g_aPlayer.aModel[18][2].PartsPos.z),
							D3DXVECTOR3(sinf(g_aPlayer.rot.y + D3DX_PI - fSpace + (fSpace * nCntAttack)) * 10.0f, 0.0f, cosf(g_aPlayer.rot.y + D3DX_PI - fSpace + (fSpace * nCntAttack)) * 10.0f),
							D3DXVECTOR3(0.0f, g_aPlayer.rot.y - fWide + (fWide * nCntAttack), 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 0.0f, 0.0f, 0.3f, NULL_VECTOR3, 0);
					}
				}
			}
		}

		//============================================================================================================================================

		//============================================
		//ダメージを受けた時にダメージ状態にする処理
		//============================================
		if (g_aPlayer.bDamageState == true)
		{
			g_aPlayer.State = PLAYERSTATE_DAMAGE;

			g_aPlayer.nDamageCountTime++;

			if (g_aPlayer.nDamageCountTime == 60)
			{
				g_aPlayer.nDamageCountTime = 0;
				g_aPlayer.bDamageState = false;
			}
		}

		//=================================================
		//プレイヤーの行動範囲制限
		//=================================================
		for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++, pWall++)
		{
			//1枚目(上)
			if (g_aPlayer.pos.z > pWall->pos.z - COLLISION_WALL && nCntWall == 0)
			{
				g_aPlayer.pos.z = pWall->pos.z - COLLISION_WALL;
			}
			//2枚目(右)
			if (g_aPlayer.pos.x > pWall->pos.x - COLLISION_WALL && nCntWall == 1)
			{
				g_aPlayer.pos.x = pWall->pos.x - COLLISION_WALL;
			}
			//3枚目(下)
			if (g_aPlayer.pos.z < pWall->pos.z + COLLISION_WALL && nCntWall == 2)
			{
				g_aPlayer.pos.z = pWall->pos.z + COLLISION_WALL;
			}
			//4枚目(左)
			if (g_aPlayer.pos.x < pWall->pos.x + COLLISION_WALL && nCntWall == 3)
			{
				g_aPlayer.pos.x = pWall->pos.x + COLLISION_WALL;
			}
		}

		//レベルアップ処理
		LevelPlayer();

		//ステータス処理
		StatusPlayer();

		//影の位置を設定
		SetPositionShadow(g_aPlayer.nIdxShadow, g_aPlayer.pos);

		if (GetMode() != MODE_TITLE && GetMode() != MODE_TUTORIAL)
		{
			if (g_nDelay2 == 1)
			{
				g_aPlayer.nIdxRaderDisplay = SetRaderDisplay(RADERDISPLAY00_PLAYER);
			}

			//レーダーディスプレイの位置を設定
			SetPositionRaderDisPlay(g_aPlayer.nIdxRaderDisplay, g_aPlayer.pos, -g_aPlayer.rot.y + D3DX_PI);
		}
	}

	//============================================
	//無敵状態の処理
	//===========================================
	if (g_aPlayer.bInvincible)
	{
		g_aPlayer.nInvincibleCnt++;

		if (g_aPlayer.nInvincibleCnt % 2 == 0)
		{
			for (int nCntParts = 0; nCntParts < MAX_PLAYERPARTS2; nCntParts++)
			{
				for (int nCntWeapon = 0; nCntWeapon < MAX_PLAYER_WEAPON; nCntWeapon++)
				{
					for (int nCntMat = 0; nCntMat < (int)g_aPlayer.aModel[nCntParts][nCntWeapon].dwNumMat; nCntMat++)
					{
						g_aPlayer.aModel[nCntParts][nCntWeapon].DiffUse[nCntMat].a = 1.0f;
					}
				}
			}
		}
		else
		{
			for (int nCntParts = 0; nCntParts < MAX_PLAYERPARTS2; nCntParts++)
			{
				for (int nCntWeapon = 0; nCntWeapon < MAX_PLAYER_WEAPON; nCntWeapon++)
				{
					for (int nCntMat = 0; nCntMat < (int)g_aPlayer.aModel[nCntParts][nCntWeapon].dwNumMat; nCntMat++)
					{
						g_aPlayer.aModel[nCntParts][nCntWeapon].DiffUse[nCntMat].a = 0.0f;
					}
				}
			}
		}

		if (g_aPlayer.nInvincibleCnt == 120)
		{
			for (int nCntParts = 0; nCntParts < MAX_PLAYERPARTS2; nCntParts++)
			{
				for (int nCntWeapon = 0; nCntWeapon < MAX_PLAYER_WEAPON; nCntWeapon++)
				{
					for (int nCntMat = 0; nCntMat < (int)g_aPlayer.aModel[nCntParts][nCntWeapon].dwNumMat; nCntMat++)
					{
						g_aPlayer.aModel[nCntParts][nCntWeapon].DiffUse[nCntMat].a = 1.0f;
					}
				}
			}

			g_aPlayer.bInvincible = false;
			g_aPlayer.nInvincibleCnt = 0;
		}
	}


	//===============================================================
	//プレイヤーの状態が変わった時にカウント系を０にする
	//===============================================================

	//体のモーション処理
	PlayerMotion(g_aPlayer.nWeapon);

	//脚のモーション処理
	PlayerMotion2(g_aPlayer.nWeapon);

	//================================================================================
	//弱攻撃（バレットなど）を喰らった時に、プレイヤーの色を一瞬赤色に変える
	//================================================================================
	if (g_aPlayer.bWeakDamageReaction == true)
	{
		g_aPlayer.nWeakDamageReactionTime++;

		for (int nCntParts = 0; nCntParts < MAX_PLAYERPARTS2; nCntParts++)
		{
			for (int nCntWeapon = 0; nCntWeapon < MAX_PLAYER_WEAPON; nCntWeapon++)
			{
				for (int nCntMat = 0; nCntMat < (int)g_aPlayer.aModel[nCntParts][nCntWeapon].dwNumMat; nCntMat++)
				{
					g_aPlayer.aModel[nCntParts][nCntWeapon].DiffUse[nCntMat].r = 1.0f;
					g_aPlayer.aModel[nCntParts][nCntWeapon].DiffUse[nCntMat].g = 0.0f;
					g_aPlayer.aModel[nCntParts][nCntWeapon].DiffUse[nCntMat].b = 0.0f;
				}
			}
		}

		if (g_aPlayer.nWeakDamageReactionTime >= 10)
		{
			g_aPlayer.bWeakDamageReaction = false;
			g_aPlayer.nWeakDamageReactionTime = 0;
		}
	}
	else
	{
		for (int nCntParts = 0; nCntParts < MAX_PLAYERPARTS2; nCntParts++)
		{
			for (int nCntWeapon = 0; nCntWeapon < MAX_PLAYER_WEAPON; nCntWeapon++)
			{
				for (int nCntMat = 0; nCntMat < (int)g_aPlayer.aModel[nCntParts][nCntWeapon].dwNumMat; nCntMat++)
				{
					g_aPlayer.aModel[nCntParts][nCntWeapon].DiffUse[nCntMat].r = g_aPlayer.aModel[nCntParts][nCntWeapon].FormarDiffUse[nCntMat].r;
					g_aPlayer.aModel[nCntParts][nCntWeapon].DiffUse[nCntMat].g = g_aPlayer.aModel[nCntParts][nCntWeapon].FormarDiffUse[nCntMat].g;
					g_aPlayer.aModel[nCntParts][nCntWeapon].DiffUse[nCntMat].b = g_aPlayer.aModel[nCntParts][nCntWeapon].FormarDiffUse[nCntMat].b;
				}
			}
		}
	}

	//=======================================================
	//ブーメラン攻撃中の時に、武器の描画を消す処理
	//=======================================================
	if (g_aPlayer.State == PLAYERSTATE_ATTACK && g_aPlayer.nWeapon == PLAYERATTACK01_BOOMERANG)
	{
		g_aPlayer.aModel[19][1].bUse = false;
	}

	//コンボ処理
	g_aPlayer.nCntCombo--;

	//コンボ許容カウントが0になった場合
	if (g_aPlayer.nCntCombo <= 0)
	{
		//0以下にならないように調整
		g_aPlayer.nCntCombo = 0;

		//コンボ倍率をリセット
		g_aPlayer.fCombo = 1.0f;
	}
}

//=======================================================================================================================================================================================================
// 描画処理
//=======================================================================================================================================================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans, mtxParent;	//計算用マトリックス
	D3DMATERIAL9 matDef;					//現在のマテリアル保存用
	D3DXMATERIAL* pMat;						//マテリアルのデータへのポインタ

	//使用している場合
	if (g_aPlayer.bUse)
	{//プレイヤーを使用されている場合
		//プレイヤーのワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aPlayer.mtxWorld);

		//プレイヤーの向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayer.rot.y, g_aPlayer.rot.x, g_aPlayer.rot.z);
		D3DXMatrixMultiply(&g_aPlayer.mtxWorld, &g_aPlayer.mtxWorld, &mtxRot);

		//プレイヤーの位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_aPlayer.pos.x, g_aPlayer.pos.y, g_aPlayer.pos.z);
		D3DXMatrixMultiply(&g_aPlayer.mtxWorld, &g_aPlayer.mtxWorld, &mtxTrans);

		//所持している武器の番号を保存
		int nWeapon = g_aPlayer.nWeapon;

		for (int nCntParts = 0; nCntParts < MAX_PLAYERPARTS2; nCntParts++)
		{
			if (g_aPlayer.aModel[nCntParts][nWeapon].bUse)
			{//パーツが使用されている場合

				//各パーツモデルのワールドマトリックスの初期化
				D3DXMatrixIdentity(&g_aPlayer.aModel[nCntParts][nWeapon].mtxWorld);

				//各パーツモデルの向きを反映
				D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayer.aModel[nCntParts][nWeapon].rot.y, g_aPlayer.aModel[nCntParts][nWeapon].rot.x, g_aPlayer.aModel[nCntParts][nWeapon].rot.z);
				D3DXMatrixMultiply(&g_aPlayer.aModel[nCntParts][nWeapon].mtxWorld, &g_aPlayer.aModel[nCntParts][nWeapon].mtxWorld, &mtxRot);

				//各パーツモデルの位置を反映
				D3DXMatrixTranslation(&mtxTrans, g_aPlayer.aModel[nCntParts][nWeapon].pos.x, g_aPlayer.aModel[nCntParts][nWeapon].pos.y, g_aPlayer.aModel[nCntParts][nWeapon].pos.z);
				D3DXMatrixMultiply(&g_aPlayer.aModel[nCntParts][nWeapon].mtxWorld, &g_aPlayer.aModel[nCntParts][nWeapon].mtxWorld, &mtxTrans);

				//親の番号を保存
				int nParent = g_aPlayer.aModel[nCntParts][nWeapon].nIdxModelParent;

				//親のマトリックスを反映
				if (nParent != -1)
				{//親の番号を持つ場合
					mtxParent = g_aPlayer.aModel[nParent][nWeapon].mtxWorld;//親モデルのマトリックス
				}
				else
				{//親の番号を持たない場合
					mtxParent = g_aPlayer.mtxWorld;	//プレイヤーのマトリックス
				}

				//親のマトリックスを掛け合わせる
				D3DXMatrixMultiply(&g_aPlayer.aModel[nCntParts][nWeapon].mtxWorld, &g_aPlayer.aModel[nCntParts][nWeapon].mtxWorld, &mtxParent);

				//ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &g_aPlayer.aModel[nCntParts][nWeapon].mtxWorld);

				//現在のマテリアルを取得
				pDevice->GetMaterial(&matDef);

				//マテリアルデータへのポインタを取得
				pMat = (D3DXMATERIAL*)g_aPlayer.aModel[nCntParts][nWeapon].pBuffMat->GetBufferPointer();

				for (int nCntMat = 0; nCntMat < (int)g_aPlayer.aModel[nCntParts][nWeapon].dwNumMat; nCntMat++)
				{
					//===============================
					//現在の色を反映させる
					//===============================
					pMat[nCntMat].MatD3D.Diffuse = g_aPlayer.aModel[nCntParts][nWeapon].DiffUse[nCntMat];

					//マテリアルの設定
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

					if (nWeapon == 1)
					{//ブーメランの場合
						//テクスチャの設定
						pDevice->SetTexture(0, g_apTexturePlayer[nCntParts][nWeapon][nCntMat]);
					}
					else
					{
						//テクスチャの設定
						pDevice->SetTexture(0, NULL);
					}

					//モデル(パーツ)の描画
					g_aPlayer.aModel[nCntParts][nWeapon].pMesh->DrawSubset(nCntMat);
				}

				//保存していたマテリアルを戻す
				pDevice->SetMaterial(&matDef);
			}
		}
	}
}

//=======================================================================================================================================================================================================
// プレイヤーの構造体の情報を取得する。
//=======================================================================================================================================================================================================
Player* GetPlayer(void)
{
	return &g_aPlayer;
}

//=======================================================================================================================================================================================================
// プレイヤーのパーツ情報の読み込み
//=======================================================================================================================================================================================================
void LoadPlayer(void)
{
	int nCntParts = 0;
	char aDataSearch[MAX_TEXT];

	//ファイルを開く
	FILE* pFile = fopen(PLAYER_TXT, "r");

	//ファイルが開けない場合
	if (pFile == NULL)
	{
		return;
	}

	//ENDが見つかるまで読み込みを繰り返す
	while (1)
	{
		int nResult = fscanf(pFile, "%s", aDataSearch);	//検索

		//END_PARTSが見つかった場合
		if (nResult == EOF)
		{//読み込み終了
			fclose(pFile);
			break;
		}

		//モデルセットが見つかった場合
		else if (!strcmp(aDataSearch, "PARTSSET"))
		{
			//項目ごとのデータを代入
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch);	//検索

				//ENDMODELセットが見つかった場合
				if (!strcmp(aDataSearch, "END_PARTSSET"))
				{//モデルデータの読み込みを終了
					break;
				}
				//データの読み込み
				else if (!strcmp(aDataSearch, "POS"))
				{//位置
					fscanf(pFile, "%f", &g_aPlayerInfo[nCntParts].pos.x);
					fscanf(pFile, "%f", &g_aPlayerInfo[nCntParts].pos.y);
					fscanf(pFile, "%f", &g_aPlayerInfo[nCntParts].pos.z);
				}
				else if (!strcmp(aDataSearch, "ROT"))
				{//向き
					fscanf(pFile, "%f", &g_aPlayerInfo[nCntParts].rot.x);
					fscanf(pFile, "%f", &g_aPlayerInfo[nCntParts].rot.y);
					fscanf(pFile, "%f", &g_aPlayerInfo[nCntParts].rot.z);
				}
				else if (!strcmp(aDataSearch, "INDEX"))
				{//種類
					fscanf(pFile, "%d", &g_aPlayerInfo[nCntParts].nType);
				}
				else if (!strcmp(aDataSearch, "PARENT"))
				{//親の番号
					fscanf(pFile, "%d", &g_aPlayerInfo[nCntParts].nIdxModelParent);
				}
			}

			nCntParts++;				//パーツ数を加算
			g_nCntParts = nCntParts;	//パーツ数を保存
		}

		else
		{//スキップして読み込みを続ける
			continue;
		}
	}

	//プレイヤーの設定
	for (int nCntSet = 0; nCntSet < g_nCntParts; nCntSet++)
	{
		SetPlayerParts(g_aPlayerInfo[nCntSet].pos, g_aPlayerInfo[nCntSet].rot, g_aPlayerInfo[nCntSet].nType, g_aPlayerInfo[nCntSet].nIdxModelParent);
	}
}

//=======================================================================================================================================================================================================
// プレイヤーの武器情報の読み込み
//=======================================================================================================================================================================================================
void LoadWeapon(void)
{
	int nCntWeapon = 0;
	char aDataSearch[MAX_TEXT];

	//ファイルを開く
	FILE* pFile = fopen(WEAPON_TXT, "r");

	//ファイルが開けない場合
	if (pFile == NULL)
	{
		return;
	}

	//ENDが見つかるまで読み込みを繰り返す
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch);	//検索

		//END_PARTSが見つかった場合
		if (!strcmp(aDataSearch, "END_PARTS"))
		{//読み込み終了
			fclose(pFile);
			break;
		}

		//コメントアウトが見つかった場合
		if (aDataSearch[0] == '#')
		{//スキップして読み込みを続ける
			continue;
		}

		//モデルセットが見つかった場合
		if (!strcmp(aDataSearch, "PARTSSET"))
		{
			//項目ごとのデータを代入
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch);	//検索

				//ENDMODELセットが見つかった場合
				if (!strcmp(aDataSearch, "END_PARTSSET"))
				{//モデルデータの読み込みを終了
					break;
				}
				//データの読み込み
				else if (!strcmp(aDataSearch, "POSx"))
				{//位置のX座標
					fscanf(pFile, "%f", &g_aWeaponInfo[nCntWeapon].pos.x);
				}
				else if (!strcmp(aDataSearch, "POSy"))
				{//位置のY座標
					fscanf(pFile, "%f", &g_aWeaponInfo[nCntWeapon].pos.y);
				}
				else if (!strcmp(aDataSearch, "POSz"))
				{//位置のZ座標
					fscanf(pFile, "%f", &g_aWeaponInfo[nCntWeapon].pos.z);
				}
				else if (!strcmp(aDataSearch, "ROTx"))
				{//向きのX座標
					fscanf(pFile, "%f", &g_aWeaponInfo[nCntWeapon].rot.x);
				}
				else if (!strcmp(aDataSearch, "ROTy"))
				{//向きのY座標
					fscanf(pFile, "%f", &g_aWeaponInfo[nCntWeapon].rot.y);
				}
				else if (!strcmp(aDataSearch, "ROTz"))
				{//向きのZ座標
					fscanf(pFile, "%f", &g_aWeaponInfo[nCntWeapon].rot.z);
				}
				else if (!strcmp(aDataSearch, "TYPE"))
				{//種類
					fscanf(pFile, "%d", &g_aWeaponInfo[nCntWeapon].nType);
				}
				else if (!strcmp(aDataSearch, "PARENT"))
				{//親の番号
					fscanf(pFile, "%d", &g_aWeaponInfo[nCntWeapon].nIdxModelParent);
				}
			}

			nCntWeapon++;				//パーツ数を加算
			g_nCntWeapon = nCntWeapon;	//パーツ数を保存
		}
	}

	//プレイヤーの武器の設定
	for (int nCntSet = 0; nCntSet < g_nCntWeapon; nCntSet++)
	{
		SetPlayerWeapon(g_aWeaponInfo[nCntSet].pos, g_aWeaponInfo[nCntSet].rot, g_aWeaponInfo[nCntSet].nType, g_aWeaponInfo[nCntSet].nIdxModelParent, nCntSet);
	}
}

//=======================================================================================================================================================================================================
// プレイヤーのパーツをセットする
//=======================================================================================================================================================================================================
void SetPlayerParts(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nIdx)
{
	for (int nCntWeapon = 0; nCntWeapon < MAX_PLAYER_WEAPON; nCntWeapon++)
	{
		if (g_aPlayer.aModel[nType][nCntWeapon].bUse == false)
		{//パーツが使用されていない場合
			g_aPlayer.aModel[nType][nCntWeapon].bUse = true;//使用する
			g_aPlayer.aModel[nType][nCntWeapon].pos = pos;//位置
			g_aPlayer.aModel[nType][nCntWeapon].rot = rot;//向き
			g_aPlayer.aModel[nType][nCntWeapon].nIdxModelParent = nIdx;//親の番号
		}
	}
}

//=======================================================================================================================================================================================================
// プレイヤーの武器をセットする
//=======================================================================================================================================================================================================
void SetPlayerWeapon(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nIdx, int nCnt)
{
	if (!g_aPlayer.aModel[nType][nCnt].bUse)
	{//武器が使用されていない場合
		g_aPlayer.aModel[nType][nCnt].bUse = true;//使用する
		g_aPlayer.aModel[nType][nCnt].pos = pos;//位置
		g_aPlayer.aModel[nType][nCnt].rot = rot;//向き
		g_aPlayer.aModel[nType][nCnt].nIdxModelParent = nIdx;//親の番号
	}
}

//============================================================================================================================================================================================================
// モーション情報の読み込み処理
//============================================================================================================================================================================================================
void LoadMotion(int nWeapon)
{
	int nMotion = 0;							//モーションの種類番号
	int nCntParts = 0;							//パーツ数
	int nCntKey[MAX_PLAYER_WEAPON][MAX_MOTION];	//モーションごとのキー数
	char aDataSearch[MAX_TEXT];					//データ検索用

	//キー数の初期化
	for (int nCnt = 0; nCnt < MAX_MOTION; nCnt++)
	{
		nCntKey[nWeapon][nCnt] = 0;
	}

	//ファイルを開く
	FILE* pFile = fopen(MOTION_FILENAME[nWeapon], "r");

	//ファイルが開けない場合
	if (pFile == NULL)
	{
		return;
	}

	while (1)
	{
		int nResult = fscanf(pFile, "%s", aDataSearch);	//検索

		//モーションセットが見つかった場合
		if (!strcmp(aDataSearch, "MOTIONSET"))
		{
			//モーションの種類を読み込む
			fscanf(pFile, "%d", &nMotion);

			//END_MOTIONSETが見つかるまで読み込みを繰り返す
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch);	//検索

				//END_MOTIONSETが見つかった場合
				if (!strcmp(aDataSearch, "END_MOTIONSET"))
				{//モーション読み込み終了
					break;
				}

				//コメントアウトが見つかった場合
				if (aDataSearch[0] == '#')
				{//スキップして読み込みを続ける
					continue;
				}

				//ループ判定が見つかった場合
				if (!strcmp(aDataSearch, "LOOP"))
				{//ループするかどうかを取得
					fscanf(pFile, "%d", &g_aPlayerMotionSet[nWeapon][nMotion].nMotionLoop);
				}

				//キーセットが見つかった場合
				if (!strcmp(aDataSearch, "KEYSET"))
				{
					while (1)
					{
						fscanf(pFile, "%s", aDataSearch);	//検索

						//コメントアウトが見つかった場合
						if (aDataSearch[0] == '#')
						{//スキップして読み込みを続ける
							continue;
						}

						//KEYが見つかった場合
						if (!strcmp(aDataSearch, "KEY"))
						{
							//項目ごとのデータを代入
							while (1)
							{
								fscanf(pFile, "%s", aDataSearch);	//検索

								//FRAMEが見つかった場合
								if (!strcmp(aDataSearch, "FRAME"))
								{//フレーム数を読み込む
									fscanf(pFile, "%d", &g_aPlayerKeySet[nWeapon][nCntKey[nWeapon][nMotion]][nMotion].nMaxFrame);
								}

								//データの読み込み
								else if (!strcmp(aDataSearch, "POS"))
								{//位置
									fscanf(pFile, "%f", &g_aPlayerKeySet[nWeapon][nCntKey[nWeapon][nMotion]][nMotion].key[nCntParts].pos.x);
									fscanf(pFile, "%f", &g_aPlayerKeySet[nWeapon][nCntKey[nWeapon][nMotion]][nMotion].key[nCntParts].pos.y);
									fscanf(pFile, "%f", &g_aPlayerKeySet[nWeapon][nCntKey[nWeapon][nMotion]][nMotion].key[nCntParts].pos.z);
								}
								else if (!strcmp(aDataSearch, "ROT"))
								{//向き
									fscanf(pFile, "%f", &g_aPlayerKeySet[nWeapon][nCntKey[nWeapon][nMotion]][nMotion].key[nCntParts].rot.x);
									fscanf(pFile, "%f", &g_aPlayerKeySet[nWeapon][nCntKey[nWeapon][nMotion]][nMotion].key[nCntParts].rot.y);
									fscanf(pFile, "%f", &g_aPlayerKeySet[nWeapon][nCntKey[nWeapon][nMotion]][nMotion].key[nCntParts].rot.z);
								}

								//END_KEYセットが見つかった場合
								else if (!strcmp(aDataSearch, "END_KEY"))
								{//モデルデータの読み込みを終了
									nCntParts++;	//データ数を加算
									break;
								}
							}
						}

						//END_KEYSETが見つかった場合
						if (!strcmp(aDataSearch, "END_KEYSET"))
						{
							nCntParts = 0;
							nCntKey[nWeapon][nMotion]++;												//キー数を加算
							g_aPlayerMotionSet[nWeapon][nMotion].nMaxKey = nCntKey[nWeapon][nMotion];	//合計キー数を保存
							break;
						}
					}
				}
			}
		}

		//EOFが見つかった場合
		else if (nResult == EOF)
		{//ファイルを閉じる
			fclose(pFile);
			break;
		}

		//モーションセット以外が見つかった場合
		else
		{//検索を続ける
			continue;
		}
	}
}

//============================================================================================================================================================================================================
// 体のモーション処理
//============================================================================================================================================================================================================
void PlayerMotion(int nWeapon)
{
	int nState = g_aPlayer.State;																									//プレイヤーの状態
	int nCntKey = g_aPlayer.aMotionSet[nWeapon][nState].nCntKey;																	//現在のキーを保存
	int nNextKey = (nCntKey + 1) % g_aPlayerMotionSet[nWeapon][nState].nMaxKey;														//次のキーを求める
	float fFrame = (float)g_aPlayer.aMotionSet[nWeapon][nState].nCntFrame / g_aPlayerKeySet[nWeapon][nCntKey][nState].nMaxFrame;	//現在のフレームと全体フレームの割合

	//パーツごとに情報を更新
	for (int nCntParts = 0; nCntParts < MAX_PLAYERPARTS2; nCntParts++)
	{
		if (nCntParts < 10 || nCntParts > 13)
		{
			float PlayerPosX = g_aPlayerKeySet[nWeapon][nCntKey][nState].key[nCntParts].pos.x;
			float PlayerPosY = g_aPlayerKeySet[nWeapon][nCntKey][nState].key[nCntParts].pos.y;
			float PlayerPosZ = g_aPlayerKeySet[nWeapon][nCntKey][nState].key[nCntParts].pos.z;
			float PlayerRotX = g_aPlayerKeySet[nWeapon][nCntKey][nState].key[nCntParts].rot.x;
			float PlayerRotY = g_aPlayerKeySet[nWeapon][nCntKey][nState].key[nCntParts].rot.y;
			float PlayerRotZ = g_aPlayerKeySet[nWeapon][nCntKey][nState].key[nCntParts].rot.z;

			float NextPlayerPosX = g_aPlayerKeySet[nWeapon][nNextKey][nState].key[nCntParts].pos.x;
			float NextPlayerPosY = g_aPlayerKeySet[nWeapon][nNextKey][nState].key[nCntParts].pos.y;
			float NextPlayerPosZ = g_aPlayerKeySet[nWeapon][nNextKey][nState].key[nCntParts].pos.z;
			float NextPlayerRotX = g_aPlayerKeySet[nWeapon][nNextKey][nState].key[nCntParts].rot.x;
			float NextPlayerRotY = g_aPlayerKeySet[nWeapon][nNextKey][nState].key[nCntParts].rot.y;
			float NextPlayerRotZ = g_aPlayerKeySet[nWeapon][nNextKey][nState].key[nCntParts].rot.z;

			//次のキーまでの差分を求める
			float fPosx = NextPlayerPosX - PlayerPosX;	// X座標
			float fPosy = NextPlayerPosY - PlayerPosY;	// Y座標
			float fPosz = NextPlayerPosZ - PlayerPosZ;	// Z座標
			float fRotx = NextPlayerRotX - PlayerRotX;	// X軸の向き
			float fRoty = NextPlayerRotY - PlayerRotY;	// Y軸の向き
			float fRotz = NextPlayerRotZ - PlayerRotZ;	// Z軸の向き

			//向きを更新
			g_aPlayer.aModel[nCntParts][nWeapon].rot.x = g_aPlayerInfo[nCntParts].rot.x + PlayerRotX + fRotx * fFrame;//X軸
			g_aPlayer.aModel[nCntParts][nWeapon].rot.y = g_aPlayerInfo[nCntParts].rot.y + PlayerRotY + fRoty * fFrame;//Y軸
			g_aPlayer.aModel[nCntParts][nWeapon].rot.z = g_aPlayerInfo[nCntParts].rot.z + PlayerRotZ + fRotz * fFrame;//Z軸

			//位置を更新
			g_aPlayer.aModel[nCntParts][nWeapon].pos.x = g_aPlayerInfo[nCntParts].pos.x + PlayerPosX + fPosx * fFrame;//X座標
			g_aPlayer.aModel[nCntParts][nWeapon].pos.y = g_aPlayerInfo[nCntParts].pos.y + PlayerPosY + fPosy * fFrame;//Y座標
			g_aPlayer.aModel[nCntParts][nWeapon].pos.z = g_aPlayerInfo[nCntParts].pos.z + PlayerPosZ + fPosz * fFrame;//Z座標
		}
	}

	//キー数の更新
	if (g_aPlayer.aMotionSet[nWeapon][nState].nCntFrame == 0 || g_aPlayer.aMotionSet[nWeapon][nState].nCntFrame >= g_aPlayerKeySet[nWeapon][nCntKey][nState].nMaxFrame)
	{
		if (g_aPlayer.aMotionSet[nWeapon][nState].nCntFrame >= g_aPlayerKeySet[nWeapon][nCntKey][nState].nMaxFrame)
		{
			//フレーム数カウンタをリセット
			g_aPlayer.aMotionSet[nWeapon][nState].nCntFrame = 0;
		}

		//キー数を加算
		g_aPlayer.aMotionSet[nWeapon][nState].nCntKey++;

		//キー数が最大数になった
		if (g_aPlayer.aMotionSet[nWeapon][nState].nCntKey >= g_aPlayerMotionSet[nWeapon][nState].nMaxKey)
		{
			//ループしない場合
			if (g_aPlayerMotionSet[nWeapon][nState].nMotionLoop == 1)
			{
				//ニュートラル状態
				g_aPlayer.State = PLAYERSTATE_NUTORAL;
				g_aPlayer.bCompulsionAttack = false;    //強制的に攻撃状態にする
				g_aPlayer.bCompulsionHealWall = false;  //強制的に壁回復状態にする
				g_aPlayer.bCompulsionHealGround = false;//強制的に床回復状態にする
				g_aPlayer.bCompulsionDodge = false;     //強制的に回避状態にする
				g_aPlayer.aModel[19][1].bUse = true;//ブーメラン状態が終わったときにブーメランの描画を復活
				g_aPlayer.nAttackTime = 0;


				if (g_aPlayer.nStateOld == PLAYERSTATE_HEALWALL)
				{//前のステートが壁充電状態だったら
					g_aPlayer.bInvincible = true;//無敵状態にする

					Model* pModel = GetModel();

					for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++, pModel++)
					{
						if (pModel->bUse == true && pModel->nType == 41 && g_aPlayer.nNumUseWallOutlet == nCntModel)
						{
							pModel->bUse = false;
							KillRaderDisplay(pModel->nIdxRaderDisplay);
						}
					}
				}
				else if (g_aPlayer.nStateOld == PLAYERSTATE_HEALGROUND)
				{
					g_aPlayer.bInvincible = true;//無敵状態にする

					Model* pModel = GetModel();

					for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++, pModel++)
					{
						if (pModel->bUse == true && pModel->nType == 19 && g_aPlayer.nNumUseGroundOutlet == nCntModel)
						{
							pModel->bUse = false;
							KillRaderDisplay(pModel->nIdxRaderDisplay);


						}
					}
				}
			}

			//キー数を初期化
			g_aPlayer.aMotionSet[nWeapon][nState].nCntKey = 0;
		}
	}

	//===========================================================================
	//プラグがコンセントの差し込み口に刺さったときに固定し、充電を開始する
	//===========================================================================
	if (g_aPlayer.State == PLAYERSTATE_HEALWALL && nCntKey == 18 && g_aPlayer.bChargingFlag == true)
	{
		SubBattery(-500);
		PlaySound(SOUND_LABEL_SE_CHARGINGCOMPLETION_000);
		g_aPlayer.bChargingFlag = false;
	}
	else if (g_aPlayer.State == PLAYERSTATE_HEALGROUND && nCntKey == 18 && g_aPlayer.bChargingGroundFlag)
	{
		SubBattery(-500);
		PlaySound(SOUND_LABEL_SE_CHARGINGCOMPLETION_000);
		g_aPlayer.bChargingGroundFlag = false;
	}

	//フレーム数更新
	g_aPlayer.aMotionSet[nWeapon][nState].nCntFrame++;

	PrintDebugProc("現在のキー：%d\n", g_aPlayer.aMotionSet[nWeapon][nState].nCntKey);
	PrintDebugProc("現在のプレイヤーの状態：%d\n", g_aPlayer.State);

}

//============================================================================================================================================================================================================
// 脚のモーション処理
//============================================================================================================================================================================================================
void PlayerMotion2(int nWeapon)
{
	int nState = 0;
	int nCntKey = 0;
	int nNextKey;
	float fFrame;

	//パーツごとに情報を更新
	for (int nCntParts = 0; nCntParts < MAX_PLAYERPARTS2; nCntParts++)
	{
		if (nCntParts >= 10 && nCntParts <= 13)
		{
			if (g_aPlayer.State == PLAYERSTATE_ATTACK)
			{//攻撃状態の場合
				if (g_aPlayer.bMove)
				{//動いている場合
					nState = (int)PLAYERSTATE_MOVE;	//移動状態
				}
				else
				{//動いていない場合
					nState = (int)g_aPlayer.State;	//状態を合わせる
				}
			}
			else
			{//攻撃状態ではない場合
				nState = (int)g_aPlayer.State;	//状態を合わせる
			}

			nCntKey = g_aPlayer.aMotionSet2[nWeapon][nState].nCntKey;																//現在のキーを保存
			nNextKey = (nCntKey + 1) % g_aPlayerMotionSet[nWeapon][nState].nMaxKey;													//次のキーを求める
			fFrame = (float)g_aPlayer.aMotionSet2[nWeapon][nState].nCntFrame / g_aPlayerKeySet[nWeapon][nCntKey][nState].nMaxFrame;	//現在のフレームと全体フレームの割合

			float PlayerPosX = g_aPlayerKeySet[nWeapon][nCntKey][nState].key[nCntParts].pos.x;
			float PlayerPosY = g_aPlayerKeySet[nWeapon][nCntKey][nState].key[nCntParts].pos.y;
			float PlayerPosZ = g_aPlayerKeySet[nWeapon][nCntKey][nState].key[nCntParts].pos.z;
			float PlayerRotX = g_aPlayerKeySet[nWeapon][nCntKey][nState].key[nCntParts].rot.x;
			float PlayerRotY = g_aPlayerKeySet[nWeapon][nCntKey][nState].key[nCntParts].rot.y;
			float PlayerRotZ = g_aPlayerKeySet[nWeapon][nCntKey][nState].key[nCntParts].rot.z;

			float NextPlayerPosX = g_aPlayerKeySet[nWeapon][nNextKey][nState].key[nCntParts].pos.x;
			float NextPlayerPosY = g_aPlayerKeySet[nWeapon][nNextKey][nState].key[nCntParts].pos.y;
			float NextPlayerPosZ = g_aPlayerKeySet[nWeapon][nNextKey][nState].key[nCntParts].pos.z;
			float NextPlayerRotX = g_aPlayerKeySet[nWeapon][nNextKey][nState].key[nCntParts].rot.x;
			float NextPlayerRotY = g_aPlayerKeySet[nWeapon][nNextKey][nState].key[nCntParts].rot.y;
			float NextPlayerRotZ = g_aPlayerKeySet[nWeapon][nNextKey][nState].key[nCntParts].rot.z;

			//次のキーまでの差分を求める
			float fPosx = NextPlayerPosX - PlayerPosX;	// X座標
			float fPosy = NextPlayerPosY - PlayerPosY;	// Y座標
			float fPosz = NextPlayerPosZ - PlayerPosZ;	// Z座標
			float fRotx = NextPlayerRotX - PlayerRotX;	// X軸の向き
			float fRoty = NextPlayerRotY - PlayerRotY;	// Y軸の向き
			float fRotz = NextPlayerRotZ - PlayerRotZ;	// Z軸の向き

			//向きを更新
			g_aPlayer.aModel[nCntParts][nWeapon].rot.x = g_aPlayerInfo[nCntParts].rot.x + PlayerRotX + fRotx * fFrame;//X軸
			g_aPlayer.aModel[nCntParts][nWeapon].rot.y = g_aPlayerInfo[nCntParts].rot.y + PlayerRotY + fRoty * fFrame;//Y軸
			g_aPlayer.aModel[nCntParts][nWeapon].rot.z = g_aPlayerInfo[nCntParts].rot.z + PlayerRotZ + fRotz * fFrame;//Z軸

			//位置を更新
			g_aPlayer.aModel[nCntParts][nWeapon].pos.x = g_aPlayerInfo[nCntParts].pos.x + PlayerPosX + fPosx * fFrame;//X座標
			g_aPlayer.aModel[nCntParts][nWeapon].pos.y = g_aPlayerInfo[nCntParts].pos.y + PlayerPosY + fPosy * fFrame;//Y座標
			g_aPlayer.aModel[nCntParts][nWeapon].pos.z = g_aPlayerInfo[nCntParts].pos.z + PlayerPosZ + fPosz * fFrame;//Z座標
		}
	}

	//キー数の更新
	if (g_aPlayer.aMotionSet2[nWeapon][nState].nCntFrame == 0 || g_aPlayer.aMotionSet2[nWeapon][nState].nCntFrame >= g_aPlayerKeySet[nWeapon][nCntKey][nState].nMaxFrame)
	{
		if (g_aPlayer.aMotionSet2[nWeapon][nState].nCntFrame >= g_aPlayerKeySet[nWeapon][nCntKey][nState].nMaxFrame)
		{
			//フレーム数カウンタをリセット
			g_aPlayer.aMotionSet2[nWeapon][nState].nCntFrame = 0;
		}

		//キー数を加算
		g_aPlayer.aMotionSet2[nWeapon][nState].nCntKey++;

		//キー数が最大数になった
		if (g_aPlayer.aMotionSet2[nWeapon][nState].nCntKey >= g_aPlayerMotionSet[nWeapon][nState].nMaxKey)
		{
			//ループしない場合
			if (g_aPlayerMotionSet[nWeapon][nState].nMotionLoop == 1)
			{
				//ニュートラル状態
				g_aPlayer.State = PLAYERSTATE_NUTORAL;
			}

			//キー数を初期化
			g_aPlayer.aMotionSet2[nWeapon][nState].nCntKey = 0;
		}
	}

	//フレーム数更新
	g_aPlayer.aMotionSet2[nWeapon][nState].nCntFrame++;
}

//=======================================================================================================================================================================================================
// ダメージ処理
//=======================================================================================================================================================================================================
void SetDamagePlayer(int nDamage)
{
	g_aPlayer.nHp -= nDamage;
}

//=======================================================================================================================================================================================================
// プレイヤーが死んだかどうかの判定を返す
//=======================================================================================================================================================================================================
bool bPlayerDeath(void)
{
	bool bDeath = false;

	if (g_aPlayer.nHp <= 0)
	{
		bDeath = true;
		g_aPlayer.bUse = false;
	}

	return bDeath;
}

//=======================================================================================================================================================================================================
// スクラップ回収処理
//=======================================================================================================================================================================================================
void AddScrapPlayer(SCRAPTYPE ScrapType, int nScrap)//スクラップを回収したときに呼ぶ関数。
{
	g_aPlayer.nTotalGetScrap++;//合計獲得数をプラス

	switch (ScrapType)
	{
	case SCRAPTYPE00_TIBIROBO:

		for (int nCntParticle = 0; nCntParticle < 50; nCntParticle++)
		{
			float fRandumRot = float(rand() % 200 + 1) / 100;
			float fRandumSpeed = float(rand() % 50 - 25) / 10;
			float fRandumMove = float(rand() % 50 - 25) / 10;

			SetParticle(PARTICLETYPE_NORMAL, 100, 8.0f, 0.4f, D3DXVECTOR3(g_aPlayer.pos.x, g_aPlayer.pos.y + g_aPlayer.Size.y / 2, g_aPlayer.pos.z), D3DXVECTOR3(sinf(-D3DX_PI * fRandumRot) * fRandumSpeed, fRandumMove, cosf(-D3DX_PI * fRandumRot) * fRandumSpeed),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, PARTICLETEX_NORMAL,0);
		}

		g_aPlayer.nTibiroboScrap += nScrap;//ちびロボスクラップを獲得
		g_aPlayer.nTotalTibiroboScrap += nScrap;//ちびロボスクラップ合計獲得量

		AddGauge(SCRAPTYPE00_TIBIROBO, g_aPlayer.nTibiroboScrap, g_nDecideLevelUpPoint[g_aPlayer.nLevel]);

		break;

	case SCRAPTYPE01_SWORD:

		for (int nCntParticle = 0; nCntParticle < 50; nCntParticle++)
		{
			float fRandumRot = float(rand() % 200 + 1) / 100;
			float fRandumSpeed = float(rand() % 50 - 25) / 10;
			float fRandumMove = float(rand() % 50 - 25) / 10;
			SetParticle(PARTICLETYPE_NORMAL, 100, 8.0f, 0.4f, D3DXVECTOR3(g_aPlayer.pos.x, g_aPlayer.pos.y + g_aPlayer.Size.y / 2, g_aPlayer.pos.z), D3DXVECTOR3(sinf(-D3DX_PI * fRandumRot) * fRandumSpeed, fRandumMove, cosf(-D3DX_PI * fRandumRot) * fRandumSpeed),
				D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), 0.0f, PARTICLETEX_NORMAL,0);
		}

		g_aPlayer.aSwordInfo.nSwordScrap += nScrap;//剣スクラップを獲得
		g_aPlayer.aSwordInfo.nTotalSwordScrap += nScrap;//剣スクラップ合計獲得量

		AddGauge(SCRAPTYPE01_SWORD, g_aPlayer.aSwordInfo.nSwordScrap, g_nDecideLevelUpPoint[g_aPlayer.aSwordInfo.nLevel]);

		break;

	case SCRAPTYPE02_BOOMERANG:

		for (int nCntParticle = 0; nCntParticle < 50; nCntParticle++)
		{
			float fRandumRot = float(rand() % 200 + 1) / 100;
			float fRandumSpeed = float(rand() % 50 - 25) / 10;
			float fRandumMove = float(rand() % 50 - 25) / 10;
			SetParticle(PARTICLETYPE_NORMAL, 100, 8.0f, 0.4f, D3DXVECTOR3(g_aPlayer.pos.x, g_aPlayer.pos.y + g_aPlayer.Size.y / 2, g_aPlayer.pos.z), D3DXVECTOR3(sinf(-D3DX_PI * fRandumRot) * fRandumSpeed, fRandumMove, cosf(-D3DX_PI * fRandumRot) * fRandumSpeed),
				D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 0.0f, PARTICLETEX_NORMAL,0);
		}

		g_aPlayer.aBoomerangInfo.nBoomerangScrap += nScrap;//ブーメランスクラップを獲得
		g_aPlayer.aBoomerangInfo.nTotalBoomerangScrap += nScrap;//ブーメランスクラップ合計獲得量

		AddGauge(SCRAPTYPE02_BOOMERANG, g_aPlayer.aBoomerangInfo.nBoomerangScrap, g_nDecideLevelUpPoint[g_aPlayer.aBoomerangInfo.nLevel]);

		break;

	case SCRAPTYPE03_SPEAKER:

		for (int nCntParticle = 0; nCntParticle < 50; nCntParticle++)
		{
			float fRandumRot = float(rand() % 200 + 1) / 100;
			float fRandumSpeed = float(rand() % 50 - 25) / 10;
			float fRandumMove = float(rand() % 50 - 25) / 10;
			SetParticle(PARTICLETYPE_NORMAL, 100, 8.0f, 0.4f, D3DXVECTOR3(g_aPlayer.pos.x, g_aPlayer.pos.y + g_aPlayer.Size.y / 2, g_aPlayer.pos.z), D3DXVECTOR3(sinf(-D3DX_PI * fRandumRot) * fRandumSpeed, fRandumMove, cosf(-D3DX_PI * fRandumRot) * fRandumSpeed),
				D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 0.0f, PARTICLETEX_NORMAL,0);
		}

		g_aPlayer.aSpeakerInfo.nSpeakerScrap += nScrap;//スピーカースクラップを獲得
		g_aPlayer.aSpeakerInfo.nTotalSpeakerScrap += nScrap;//スピーカースクラップ合計獲得量

		AddGauge(SCRAPTYPE03_SPEAKER, g_aPlayer.aSpeakerInfo.nSpeakerScrap, g_nDecideLevelUpPoint[g_aPlayer.aSpeakerInfo.nLevel]);

		break;

	case SCRAPTYPE04_ALL:

		for (int nCntParticle = 0; nCntParticle < 50; nCntParticle++)
		{
			float fRandumRot = float(rand() % 200 + 1) / 100;
			float fRandumSpeed = float(rand() % 50 - 25) / 10;
			float fRandumMove = float(rand() % 50 - 25) / 10;
			float fRandumColorR = float(rand() % 100 + 1) / 100;
			float fRandumColorG = float(rand() % 100 + 1) / 100;
			float fRandumColorB = float(rand() % 100 + 1) / 100;
			SetParticle(PARTICLETYPE_NORMAL, 100, 8.0f, 0.4f, D3DXVECTOR3(g_aPlayer.pos.x, g_aPlayer.pos.y + g_aPlayer.Size.y / 2, g_aPlayer.pos.z), D3DXVECTOR3(sinf(-D3DX_PI * fRandumRot) * fRandumSpeed, fRandumMove, cosf(-D3DX_PI * fRandumRot) * fRandumSpeed),
				D3DXCOLOR(fRandumColorR, fRandumColorG, fRandumColorB, 1.0f), 0.0f, PARTICLETEX_NORMAL,0);
		}

		g_aPlayer.nTibiroboScrap += nScrap;//ちびロボスクラップを獲得
		g_aPlayer.aSwordInfo.nSwordScrap += nScrap;//剣スクラップを獲得
		g_aPlayer.aBoomerangInfo.nBoomerangScrap += nScrap;//ブーメランスクラップを獲得
		g_aPlayer.aSpeakerInfo.nSpeakerScrap += nScrap;//スピーカースクラップを獲得
		g_aPlayer.nTotalAllUpScrap += nScrap;//全ステータス強化スクラップ合計獲得量

		AddGauge(SCRAPTYPE00_TIBIROBO, g_aPlayer.nTibiroboScrap, g_nDecideLevelUpPoint[g_aPlayer.nLevel]);
		AddGauge(SCRAPTYPE01_SWORD, g_aPlayer.aSwordInfo.nSwordScrap, g_nDecideLevelUpPoint[g_aPlayer.aSwordInfo.nLevel]);
		AddGauge(SCRAPTYPE02_BOOMERANG, g_aPlayer.aBoomerangInfo.nBoomerangScrap, g_nDecideLevelUpPoint[g_aPlayer.aBoomerangInfo.nLevel]);
		AddGauge(SCRAPTYPE03_SPEAKER, g_aPlayer.aSpeakerInfo.nSpeakerScrap, g_nDecideLevelUpPoint[g_aPlayer.aSpeakerInfo.nLevel]);

		break;
	}
}

//=======================================================================================================================================================================================================
// プレイヤーのレベルに関する処理を行う。
//=======================================================================================================================================================================================================
void LevelPlayer(void)
{
	//========================
	// プレイヤー強化
	//========================
	if (g_aPlayer.nTibiroboScrap >= g_nDecideLevelUpPoint[g_aPlayer.nLevel] && g_aPlayer.nLevel < MAX_LEVEL)//プレイヤー強化スクラップが、レベルアップする量まで溜まったら
	{
		g_aPlayer.nTibiroboScrap = 0;//レベルアップ必要量に達したので、スクラップ獲得量をリセット
		g_aPlayer.nLevel += 1;//レベルを１上げる

		AddLevel(0);
	}
	
	//========================
	// 剣攻撃強化
	//========================
	if (g_aPlayer.aSwordInfo.nSwordScrap >= g_nDecideLevelUpPoint[g_aPlayer.aSwordInfo.nLevel] && g_aPlayer.nLevel < MAX_LEVEL)//剣攻撃強化スクラップが、レベルアップする量まで溜まったら
	{
		g_aPlayer.aSwordInfo.nSwordScrap = 0;//レベルアップ必要量に達したので、スクラップ獲得量をリセット
		g_aPlayer.aSwordInfo.nLevel += 1;//レベルを１上げる

		AddLevel(1);
	}

	//========================
	// ブーメラン攻撃強化
	//========================
	if (g_aPlayer.aBoomerangInfo.nBoomerangScrap >= g_nDecideLevelUpPoint[g_aPlayer.aBoomerangInfo.nLevel] && g_aPlayer.nLevel < MAX_LEVEL)//ブーメラン攻撃強化スクラップが、レベルアップする量まで溜まったら
	{
		g_aPlayer.aBoomerangInfo.nBoomerangScrap = 0;//レベルアップ必要量に達したので、スクラップ獲得量をリセット
		g_aPlayer.aBoomerangInfo.nLevel += 1;//レベルを１上げる

		AddLevel(2);
	}
	
	//========================
	// スピーカー攻撃強化
	//========================
	if (g_aPlayer.aSpeakerInfo.nSpeakerScrap >= g_nDecideLevelUpPoint[g_aPlayer.aSpeakerInfo.nLevel] && g_aPlayer.nLevel < MAX_LEVEL)//スピーカー攻撃強化スクラップが、レベルアップする量まで溜まったら
	{
		g_aPlayer.aSpeakerInfo.nSpeakerScrap = 0;//レベルアップ必要量に達したので、スクラップ獲得量をリセット
		g_aPlayer.aSpeakerInfo.nLevel += 1;//レベルを１上げる

		AddLevel(3);
	}
}

//=======================================================================================================================================================================================================
// プレイヤーのステータスに関する処理を行う。
//=======================================================================================================================================================================================================
void StatusPlayer(void)
{
	int nSaveBoomerangPiece = g_aPlayer.aBoomerangInfo.nMaxBoomerangPiece;//1F前の持てるブーメランの最大数(違っていたら

	//===============================
	// プレイヤーのステータス
	//===============================
	g_aPlayer.fSpeed = FIRST_PLAYERSPEED + (0.4f * g_aPlayer.nLevel);
	g_aPlayer.nBattery = 1000 + (100 * g_aPlayer.nLevel);
	g_aPlayer.fSuctionScrapRange = 20.0f + (15.0f * g_aPlayer.nLevel);

	//===============================
	// 剣のステータス
	//===============================
	g_aPlayer.aSwordInfo.nSwordPower = 10 + (int)(0.6f * g_aPlayer.aSwordInfo.nLevel);//剣の攻撃力
	float fData = 1.0f;

	for (int nCnt = 0; nCnt < g_aPlayer.aSwordInfo.nLevel; nCnt++)
	{
		fData *= 1.1f;
	}

	g_aPlayer.aSwordInfo.nSwordAttackSpeed = 4 * (int)fData;//剣の攻撃速度

	//===============================
	// ブーメランのステータス
	//===============================
	g_aPlayer.aBoomerangInfo.nBoomerangPower = 5 + (int)(0.5f * g_aPlayer.aBoomerangInfo.nLevel);//ブーメランの攻撃力
	g_aPlayer.aBoomerangInfo.fBoomerangRenge = 200.0f + (30.0f * g_aPlayer.aBoomerangInfo.nLevel);//ブーメランの射程
	g_aPlayer.aBoomerangInfo.nMaxBoomerangPiece = 1 + (g_aPlayer.aBoomerangInfo.nLevel / 3);//ブーメランの個数
	float fBoomerangScale = 1.0f + (0.2f * g_aPlayer.aBoomerangInfo.nLevel);//ブーメランの大きさ
	g_aPlayer.aBoomerangInfo.BoomerangScale = D3DXVECTOR3(fBoomerangScale, fBoomerangScale, fBoomerangScale);//ブーメランの大きさ反映

	if (nSaveBoomerangPiece != g_aPlayer.aBoomerangInfo.nMaxBoomerangPiece)
	{//レベルが上がったので、現在持っているブーメランの個数を１つ増やす
		g_aPlayer.aBoomerangInfo.nBoomerangPiece += 1;
	}

	//===============================
	// スピーカーのステータス
	//===============================
	g_aPlayer.aSpeakerInfo.fSpeakerPower = 7.0f + 7.0f * (0.3f * g_aPlayer.aSpeakerInfo.nLevel);//スピーカーの攻撃力
	g_aPlayer.aSpeakerInfo.nPowerConsumption = 100;//スピーカーの消費電力
}

//=======================================================================================================================================================================================================
// プレイヤーのマトリックスを計算する関数
//=======================================================================================================================================================================================================
void CalculateMatrixPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans, mtxParent;	//計算用マトリックス

	//所持している武器の番号を保存
	int nWeapon = g_aPlayer.nWeapon;
	
	//使用している場合
	if (g_aPlayer.bUse)
	{
		//プレイヤーのワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aPlayer.mtxWorld);

		//プレイヤーの向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayer.rot.y, g_aPlayer.rot.x, g_aPlayer.rot.z);
		D3DXMatrixMultiply(&g_aPlayer.mtxWorld, &g_aPlayer.mtxWorld, &mtxRot);

		//プレイヤーの位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_aPlayer.pos.x, g_aPlayer.pos.y, g_aPlayer.pos.z);
		D3DXMatrixMultiply(&g_aPlayer.mtxWorld, &g_aPlayer.mtxWorld, &mtxTrans);

		for (int nCntParts = 0; nCntParts < MAX_PLAYERPARTS2; nCntParts++)
		{
			//各パーツモデルのワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aPlayer.aModel[nCntParts][nWeapon].mtxWorld);

			//各パーツモデルの向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayer.aModel[nCntParts][nWeapon].rot.y, g_aPlayer.aModel[nCntParts][nWeapon].rot.x, g_aPlayer.aModel[nCntParts][nWeapon].rot.z);
			D3DXMatrixMultiply(&g_aPlayer.aModel[nCntParts][nWeapon].mtxWorld, &g_aPlayer.aModel[nCntParts][nWeapon].mtxWorld, &mtxRot);

			//各パーツモデルの位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aPlayer.aModel[nCntParts][nWeapon].pos.x, g_aPlayer.aModel[nCntParts][nWeapon].pos.y, g_aPlayer.aModel[nCntParts][nWeapon].pos.z);
			D3DXMatrixMultiply(&g_aPlayer.aModel[nCntParts][nWeapon].mtxWorld, &g_aPlayer.aModel[nCntParts][nWeapon].mtxWorld, &mtxTrans);

			//親の番号を保存
			int nParent = g_aPlayer.aModel[nCntParts][nWeapon].nIdxModelParent;

			//親のマトリックスを反映
			if (nParent != -1)
			{//親の番号を持つ場合
				mtxParent = g_aPlayer.aModel[nParent][nWeapon].mtxWorld;	//親モデルのマトリックス
			}
			else
			{//親の番号を持たない場合
				mtxParent = g_aPlayer.mtxWorld;	//プレイヤーのマトリックス
			}

			//親のマトリックスを掛け合わせる
			D3DXMatrixMultiply(&g_aPlayer.aModel[nCntParts][nWeapon].mtxWorld, &g_aPlayer.aModel[nCntParts][nWeapon].mtxWorld, &mtxParent);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aPlayer.aModel[nCntParts][nWeapon].mtxWorld);

			D3DXVECTOR3 posSword;
			D3DXVECTOR3 posZero(0.0f, 0.0f, 0.0f);

			D3DXVec3TransformCoord(&posSword, &posZero, &g_aPlayer.aModel[nCntParts][nWeapon].mtxWorld);//左の引数のローカル座標が真ん中の引数のワールド座標を代入する

			//ワールド座標を代入
			g_aPlayer.aModel[nCntParts][nWeapon].PartsPos = posSword;
		}
	}
}

//=======================================================================================================================================================================================================
//プレイヤーに攻撃がヒットした時に呼ぶ関数
//=======================================================================================================================================================================================================
void AttackHitPlayer(int nSubBattery)
{
	if (!g_aPlayer.bDamageState && !g_aPlayer.bInvincible && g_aPlayer.State != PLAYERSTATE_HEALWALL && g_aPlayer.State != PLAYERSTATE_HEALGROUND &&
		g_aPlayer.State != PLAYERSTATE_DODGE)
	{
		PlaySound(SOUND_LABEL_SE_DAMAGE_000);

		SubBattery(nSubBattery);
		g_aPlayer.bDamageState = true;//ダメージ状態にする
		g_aPlayer.bInvincible = true;//無敵状態にする

		//コンボ倍率をリセット
		g_aPlayer.fCombo = 1.0f;
	}
}

//=======================================================================================================================================================================================================
//プレイヤーの位置リセット処理
//=======================================================================================================================================================================================================
void ResetPlayerPos(void)
{
	//向きをリセット(前向き)
	g_aPlayer.rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
	
	switch (GetMode())
	{
	case MODE_TUTORIAL:	//チュートリアル
		g_aPlayer.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		break;
	case MODE_GAME:	//ゲーム
		g_aPlayer.pos = D3DXVECTOR3(0.0f, 0.0f, -2200.0f);
		break;
	default:
		break;
	}
}