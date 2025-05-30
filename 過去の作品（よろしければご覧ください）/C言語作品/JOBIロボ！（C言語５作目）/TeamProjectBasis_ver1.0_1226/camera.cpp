//========================================================
//
// カメラの処理
// Author : ShinaTaiyo
//
//=========================================================
#include "camera.h"
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "player.h"
#include "game.h"
#include "editEnemy.h"
#include "boss.h"
#include "emitter.h"
#include "field.h"

//=========================================================
// グローバル変数
//=========================================================
Camera g_Camera;//カメラの情報
float g_fCameraPosVX;//カメラのXの支点の位置を変える
float g_fCameraPosVY;//カメラのYの支点の位置を変える
float g_fCameraPosVZ;//カメラのZの支点の位置を変える
float g_fLength;//カメラとプレイヤーの距離
float g_fAngle;//カメラとプレイヤーの角度
int g_nCameraDelay;//ディレイカウント

//=========================================================
// 初期化処理
//=========================================================
void InitCamera(void)
{
	Player* pPlayer = GetPlayer();//プレイヤーの構造体の情報を取得

	g_nCameraDelay = 0;//ディレイカウント
	g_Camera.PosV = D3DXVECTOR3(pPlayer->pos.x, 0.0f,800.0f);//後ろから見るので、プレイヤーのXの初期地点を決めれば大丈夫（Y,Zは定数(視点からの距離)
	g_Camera.PosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//注視点
	g_Camera.VecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);//上方向ベクトル		
	g_Camera.DecayRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//カメラの向きを変える時に減衰させる処理
	g_Camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Camera.move = NULL_VECTOR3;//移動量
	g_Camera.center = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//カメラとプレイヤー間の中間点
	g_Camera.fXVaim = 0.0f;
	g_Camera.fZVaim = 0.0f;
	g_Camera.fRot = 0.0f;
	g_Camera.nModeOld = 0;//1f前のカメラモード
	g_Camera.nCntModeTime = 0;//そのカメラモードになってからの時間
	g_Camera.nMode = CAMERAMODE_REVOLUTION;//公転モードに初期化する
	g_Camera.bBossAppear = false;//ボスが出現したら使う
	g_Camera.fVXaim3D = 0.0f;//X方向ベクトル
	g_Camera.fVYaim3D = 0.0f;//Y方向ベクトル
	g_Camera.fVZaim3D = 0.0f;//Z方向ベクトル
	g_Camera.fVLaim3D = 0.0f;//総合ベクトル
}

//==============================
// 終了処理
//==============================
void UninitCamera(void)
{
	
}

//============================
// 更新処理
//============================
void UpdateCamera(void)
{
	g_nCameraDelay++;//ディレイカウント

	Boss* pBoss = GetBoss();//ボスの構造体の情報を取得
	Player* pPlayer = GetPlayer();//プレイヤーの構造体の情報を取得
	EnemyEditor* pEnemyEditor = GetEnemyEditor();//敵エディタの情報を取得

	g_Camera.nModeOld = g_Camera.nMode;//1f前のカメラモード

	//===========================
	//カメラモードを決める
	//===========================
	if (GetMode() == MODE_TITLE)
	{
		g_Camera.nMode = CAMERAMODE_ROTATION;//自転モード
	}
	else if (GetMode() == MODE_GAME)
	{
		if (g_nCameraDelay == 1)
		{
			g_Camera.nMode = CAMERAMODE_BIRDSEYE;
		}

		if (pBoss->bUse == true && g_Camera.bBossAppear == false)
		{//ボスが出現し、ボス出現判定フラグがfalseだったらカメラモードを一度だけボス出現演出モードにする
			g_Camera.bBossAppear = true;
			g_Camera.nMode = CAMERAMODE_BOSSAPPEAR;
		}
	}

	if (GetMode() == MODE_TUTORIAL)
	{
		g_Camera.nMode = CAMERAMODE_REVOLUTION;
	}

	if (g_Camera.nModeOld != g_Camera.nMode)
	{//カメラモードが1f前と違ったら、モードのカウントを０にする
		g_Camera.nCntModeTime = 0;
		g_Camera.fRot = D3DX_PI;
	}

	//====================================================
	//通常カメラモード
	//====================================================
	if (g_Camera.nMode == CAMERAMODE_REVOLUTION)
	{
		//posV.yとposV.zが注視点からの距離になるため（カメラは中止点の後ろから見るから）、posV.yとposV.zをsqrtfして,ちゃんとした距離を求める。
		g_Camera.fLength = sqrtf(GAME_POSV_Y * GAME_POSV_Y + GAME_POSV_Z * GAME_POSV_Z) / 2.0f;

		//視点の位置を更新
		g_Camera.PosV = D3DXVECTOR3(pPlayer->pos.x, pPlayer->pos.y + GAME_POSV_Y, pPlayer->pos.z - GAME_POSV_Z);

		//カメラとプレイヤー間の中間点を求める
		g_Camera.center = D3DXVECTOR3(pPlayer->pos.x, (g_Camera.PosV.y - pPlayer->pos.y) / 2.0f, (pPlayer->pos.z - g_Camera.PosV.z) / 2.0f);

		if (GetEditEnemyMode() == false)
		{
			//注視点をプレイヤーの位置に更新
			g_Camera.PosR.x = pPlayer->pos.x;
			g_Camera.PosR.y = pPlayer->vtxMax.y * 4.0f + pPlayer->pos.y;
			g_Camera.PosR.z = pPlayer->pos.z;

			//視点の位置の更新
			g_Camera.PosV.x = sinf(g_Camera.rot.y + D3DX_PI) * g_Camera.fLength + pPlayer->pos.x;
			g_Camera.PosV.z = cosf(g_Camera.rot.y + D3DX_PI) * g_Camera.fLength + pPlayer->pos.z;

			//カメラとプレイヤー間の中間点を求める
			g_Camera.center = D3DXVECTOR3((g_Camera.PosV.x - pPlayer->pos.x) / 2.0f, (g_Camera.PosV.y - pPlayer->pos.y) / 2.0f, (g_Camera.PosV.z - pPlayer->pos.z) / 2.0f);
		}
		else
		{//敵エディタモードだった場合
			g_Camera.PosR.x = pEnemyEditor->pos.x;
			g_Camera.PosR.y = pEnemyEditor->pos.y;
			g_Camera.PosR.z = pEnemyEditor->pos.z;

			//視点の位置の更新
			g_Camera.PosV.x = sinf(g_Camera.rot.y + D3DX_PI) * g_Camera.fLength + pEnemyEditor->pos.x;
			g_Camera.PosV.z = cosf(g_Camera.rot.y + D3DX_PI) * g_Camera.fLength + pEnemyEditor->pos.z;

			//カメラとプレイヤー間の中間点を求める
			g_Camera.center = D3DXVECTOR3((g_Camera.PosV.x - pPlayer->pos.x) / 2.0f, (g_Camera.PosV.y - pPlayer->pos.y) / 2.0f, (g_Camera.PosV.z - pPlayer->pos.z) / 2.0f);
		}
	}
	
	//=========================================
	//プレイヤー視点見渡しモード
	//=========================================
	else if (g_Camera.nMode == CAMERAMODE_ROTATION)
	{
		if (GetMode() == MODE_TITLE)
		{
			g_Camera.rot = D3DXVECTOR3(10.0f, D3DX_PI, 0.0f);
			g_Camera.PosV = D3DXVECTOR3(pPlayer->pos.x - 25.0f, 35.0f, pPlayer->pos.z + 110.0f);

			//posV.yとposV.zが注視点からの距離になるため（カメラは中止点の後ろから見るから）、posV.yとposV.zをsqrtfして,ちゃんとした距離を求める。
			g_Camera.fLength = sqrtf(TITLE_POSV_Y * TITLE_POSV_Y + TITLE_POSV_Z * TITLE_POSV_Z) / 2.0f;

			g_Camera.PosR.x = -sinf(g_Camera.rot.x) * g_Camera.fLength + g_Camera.PosV.x;
			g_Camera.PosR.y = -tanf(g_Camera.rot.y) * g_Camera.fLength + g_Camera.PosV.y;
			g_Camera.PosR.z = -cosf(g_Camera.rot.z) * g_Camera.fLength + g_Camera.PosV.z;
		}
		else
		{
			g_Camera.PosR = D3DXVECTOR3(sinf(D3DX_PI + g_Camera.rot.y) * g_Camera.fLength + g_Camera.PosV.x,
				0.0f,
				cosf(D3DX_PI + g_Camera.rot.y) * g_Camera.fLength + g_Camera.PosV.z);//視点
		}
	}
	
	//================================================
	//俯瞰視点モード
	//================================================
	if (g_Camera.nMode == CAMERAMODE_BIRDSEYE)
	{
		if (g_nCameraDelay >= 1 && g_nCameraDelay <= 360)
		{
			g_Camera.fRot += ((D3DX_PI * 2.0f) / 360.0f);

			g_Camera.PosR.x = sinf(g_Camera.fRot) * 1500.0f;
			g_Camera.PosR.y = 0.0f;
			g_Camera.PosR.z = cosf(g_Camera.fRot) * 1500.0f;

			//視点の位置の更新
			g_Camera.PosV.x = 0.0f;
			g_Camera.PosV.z = 10.0f;
			g_Camera.PosV.y = 1300.0f;
		}
		else if (g_nCameraDelay >= 361)
		{
			g_Camera.fRot = 0.0f;

			//=============================================
			//視点を移動させる目標の位置を決める
			//=============================================
			D3DXVECTOR3 targetpos;
			targetpos.x = sinf(g_Camera.rot.y + D3DX_PI) * g_Camera.fLength + pPlayer->pos.x;
			targetpos.y = pPlayer->pos.y + GAME_POSV_Y;
			targetpos.z = cosf(g_Camera.rot.y + D3DX_PI) * g_Camera.fLength + pPlayer->pos.z;

			g_Camera.PosR = pPlayer->pos;

			//==========================
	        //３次元自機狙い処理
	        //==========================
			g_Camera.fVXaim3D = targetpos.x - g_Camera.PosV.x;
			g_Camera.fVYaim3D = targetpos.y - g_Camera.PosV.y;
			g_Camera.fVZaim3D = targetpos.z - g_Camera.PosV.z;

			D3DXVECTOR3 CameraPosVmove = NULL_VECTOR3;

			g_Camera.fVLaim3D = sqrtf((g_Camera.fVXaim3D * g_Camera.fVXaim3D) + (g_Camera.fVYaim3D * g_Camera.fVYaim3D)
				+ (g_Camera.fVZaim3D * g_Camera.fVZaim3D));

			CameraPosVmove.x = g_Camera.fVXaim3D / g_Camera.fVLaim3D * 30.0f;
			CameraPosVmove.y = g_Camera.fVYaim3D / g_Camera.fVLaim3D * 30.0f;
			CameraPosVmove.z = g_Camera.fVZaim3D / g_Camera.fVLaim3D * 30.0f;

			g_Camera.move = CameraPosVmove;
			g_Camera.PosV += g_Camera.move;

			if (g_Camera.PosV.x >= targetpos.x - 30.0f && g_Camera.PosV.x <= targetpos.x + 30.0f &&
				g_Camera.PosV.y >= targetpos.y - 30.0f && g_Camera.PosV.y <= targetpos.y + 30.0f &&
				g_Camera.PosV.z >= targetpos.z - 30.0f && g_Camera.PosV.z <= targetpos.z + 30.0f)
			{
				g_Camera.nMode = CAMERAMODE_REVOLUTION;//目的の位置に達したので、カメラモードを変える
			}		
		}

		//カメラとプレイヤー間の中間点を求める
		g_Camera.center = D3DXVECTOR3((g_Camera.PosV.x - pPlayer->pos.x) / 2.0f, (g_Camera.PosV.y - pPlayer->pos.y) / 2.0f, (g_Camera.PosV.z - pPlayer->pos.z) / 2.0f);
	}

	//=============================================
	//ボス出現演出モード
	//=============================================
	else if (g_Camera.nMode == CAMERAMODE_BOSSAPPEAR)
	{
		g_Camera.nCntModeTime++;

		if (g_Camera.nCntModeTime == 1)
		{
			pBoss->pos.y = -BOSS_SETPOS_Y;
			SetStagingField(STAGINGFIELD00_MAGICCIRCLE, 720, 300.0f, 300.0f, 0.12f, D3DXVECTOR3(pBoss->pos.x, 1.0f, pBoss->pos.z), NULL_VECTOR3);
		}

		g_Camera.PosR = D3DXVECTOR3(pBoss->pos.x,0.0f,pBoss->pos.z);

		if (g_Camera.nCntModeTime >= 0 && g_Camera.nCntModeTime <= BOSSAPPEAR_TIME)
		{//300fの間、ボスの周りをカメラが周る
			g_Camera.fRot += ((D3DX_PI * 2.0f) / BOSSAPPEAR_TIME);
			pBoss->pos.y += (BOSS_SETPOS_Y / (float)(BOSSAPPEAR_TIME));

			g_Camera.PosV.x = sinf(g_Camera.fRot) * 400.0f + g_Camera.PosR.x;
			g_Camera.PosV.y = 200.0f + g_Camera.PosR.y;
			g_Camera.PosV.z = cosf(g_Camera.fRot) * 400.0f + g_Camera.PosR.z;

			float fColR = float(rand() % 100 + 1) / 100.0f;
			float fColG = float(rand() % 100 + 1) / 100.0f;
			float fColB = float(rand() % 100 + 1) / 100.0f;
			float fRandRot = float(rand() % 200 + 1) / 100.0f;
			int nLottery = rand() % 2;

			if (g_Camera.nCntModeTime % 4 == 0)
			{
				SetEmitterEffect(EMITTERTYPE_TORNADO, EMITTERTARGET_BOSS, 300, 50, 35.0f, 0.5f, NULL_VECTOR3,D3DXVECTOR3(0.0f,10.0f,0.0f),
					D3DXCOLOR(fColR, fColG, fColB, 1.0f), fRandRot, nLottery, 100.0f, D3DXVECTOR3(pBoss->pos.x, 0.0f, pBoss->pos.z));
			}
		}

		if (g_Camera.nCntModeTime >= BOSSAPPEAR_TIME + 60)
		{//360f以降、プレイヤーの所にカメラを移動させ、プレイヤーの近くに来たらボス出現演出モードを終了
			D3DXVECTOR3 targetpos;

			targetpos.x = sinf(g_Camera.rot.y + D3DX_PI) * g_Camera.fLength + pPlayer->pos.x;
			targetpos.y = pPlayer->pos.y + GAME_POSV_Y;
			targetpos.z = cosf(g_Camera.rot.y + D3DX_PI) * g_Camera.fLength + pPlayer->pos.z;
			g_Camera.PosR = D3DXVECTOR3(pBoss->pos.x,pBoss->pos.y + pBoss->Size.y / 2.0f,pBoss->pos.z);

			//==========================
			//３次元自機狙い処理
			//==========================
			g_Camera.fVXaim3D = targetpos.x - g_Camera.PosV.x;
			g_Camera.fVYaim3D = targetpos.y - g_Camera.PosV.y;
			g_Camera.fVZaim3D = targetpos.z - g_Camera.PosV.z;

			D3DXVECTOR3 CameraPosVmove = NULL_VECTOR3;

			g_Camera.fVLaim3D = sqrtf((g_Camera.fVXaim3D * g_Camera.fVXaim3D) + (g_Camera.fVYaim3D * g_Camera.fVYaim3D)
				+ (g_Camera.fVZaim3D * g_Camera.fVZaim3D));

			CameraPosVmove.x = g_Camera.fVXaim3D / g_Camera.fVLaim3D * 50.0f;
			CameraPosVmove.y = g_Camera.fVYaim3D / g_Camera.fVLaim3D * 50.0f;
			CameraPosVmove.z = g_Camera.fVZaim3D / g_Camera.fVLaim3D * 50.0f;

			g_Camera.move = CameraPosVmove;
			g_Camera.PosV += g_Camera.move;

			if (g_Camera.PosV.x >= targetpos.x - 30.0f && g_Camera.PosV.x <= targetpos.x + 30.0f &&
				g_Camera.PosV.y >= targetpos.y - 30.0f && g_Camera.PosV.y <= targetpos.y + 30.0f &&
				g_Camera.PosV.z >= targetpos.z - 30.0f && g_Camera.PosV.z <= targetpos.z + 30.0f)
			{
				g_Camera.nMode = CAMERAMODE_REVOLUTION;//目的の位置に達したので、カメラモードを変える
				pBoss->bBossAppear = false;//ボス出現演出が終わった
				g_Camera.nMode = CAMERAMODE_REVOLUTION;
			}
		}
	}

	//==============================================
	//通常モード
	//==============================================
	if (g_Camera.nMode == CAMERAMODE_REVOLUTION)
	{//通常モードまでカメラの操作を受け付けない。
		if (GetKeyboardPress(DIK_O))
		{
			g_fCameraPosVY += 5.0f;
		}
		if (GetKeyboardPress(DIK_L))
		{
			g_fCameraPosVY -= 5.0f;
		}

		if (GetKeyboardPress(DIK_U) || GetJoypadPress(JOYKEY_RB) || (float)(GetStick().abAnglePress[STICKTYPE_RIGHT][STICKANGLE_RIGHT]) > 0.1f)
		{
			g_Camera.DecayRot.y += 0.08f;
		}
		if (GetKeyboardPress(DIK_J) || GetJoypadPress(JOYKEY_LB) || (float)(GetStick().abAnglePress[STICKTYPE_RIGHT][STICKANGLE_LEFT]) > 0.1f)
		{
			g_Camera.DecayRot.y -= 0.08f;
		}

		if (GetKeyboardTrigger(DIK_C))
		{
			g_Camera.nMode++;

			if (g_Camera.nMode == 2)
			{
				g_Camera.nMode = 0;
			}
		}

		if (GetKeyboardPress(DIK_Z))
		{
			g_Camera.fLength += 10.0f;
		}
		if (GetKeyboardPress(DIK_X))
		{
			g_Camera.fLength -= 10.0f;
		}
	}

	g_Camera.DecayRot.y += (0.0f - g_Camera.DecayRot.y) * 0.5f;//カメラの向きを変える時に減衰させる処理（減衰 = Decay）
	g_Camera.rot.y += g_Camera.DecayRot.y;//カメラの向きを変える
}

//================================
// 描画処理
//================================
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&g_Camera.mtxProjection);

	//プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(&g_Camera.mtxProjection,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		10.0f,
		9000.0f);

	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_Camera.mtxProjection);

	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&g_Camera.mtxView);

	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&g_Camera.mtxView,
		&g_Camera.PosV,
		&g_Camera.PosR,
		&g_Camera.VecU);

	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &g_Camera.mtxView);
}

//====================
// カメラ情報の取得
//====================
Camera* GetCamera(void)
{
	return &g_Camera;
}
