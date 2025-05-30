//========================================================
//
// ゲーム画面の処理
// Author : ShinaTaiyo
//
//========================================================
#include "main.h"
#include "input.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "tutorial.h"
#include "sound.h"
#include "fade.h"
#include <time.h>
#include "ranking.h"
#include "camera.h"
#include "light.h"
#include "field.h"
#include "wall.h"
#include "model.h"
#include "player.h"
#include "shadow.h"
#include <stdio.h>
#include "billboard.h"
#include "bullet.h"
#include "effect.h"
#include "emitter.h"
#include "meshfield.h"
#include "particle.h"
#include "edit.h"
#include "enemy.h"
#include "timer.h"
#include "score.h"
#include "Summon.h"
#include "pause.h"
#include "item.h"
#include "editcamera.h"
#include "editEnemy.h"
#include "scrap.h"
#include "debugproc.h"
#include "gauge.h"
#include "level.h"
#include "battery.h"
#include "bossgauge.h"
#include "enemygauge.h"
#include "attackmodel.h"
#include "boss.h"
#include "attackbillboard.h"
#include "ceiling.h"
#include "mission.h"
#include "missioncheck.h"
#include "UI.h"
#include "Rader.h"
#include "operate.h"
#include "Warning.h"
#include "combo.h"

//=======================================================================================================================================================================================================
// グローバル変数
//=======================================================================================================================================================================================================
GAMESTATE g_GameState = GAMESTATE_NONE;	//ゲームの状態
int g_nCountTime;						//ゲーム開始時から経った時間を管理する変数
int g_nChoosePauseCommand;				//ポーズメニューの選択項目
int g_nCntEnemy = 0;					//敵のパーツ数
bool g_bPause = false;					//ポーズ中かどうか
bool g_bHouse = false;					//ちびロボハウス内かどうか
bool g_bEdit;							//エディタ中かどうか
bool g_bEditEnemy;						//敵エディタ中かどうか
//=======================================================================================================================================================================================================
// 初期化処理
//=======================================================================================================================================================================================================
void InitGame(void)
{
	//変数
	g_nChoosePauseCommand = 0;		//ポーズメニューの選択項目
	g_nCountTime = 0;				//時間の初期化
	g_bPause = false;				//ポーズ解除状態
	g_bEdit = false;				//エディタ解除状態
	g_bEditEnemy = false;			//敵エディタ解除状態
	g_GameState = GAMESTATE_NORMAL;	//通常状態に設定

	InitPause();			//ポーズ
	InitSummon();			//敵召喚
	InitShadow();			//影
	InitField();			//地面
	InitStagingField();     //演出フィールド
	InitItem();				//アイテム
	InitPlayer();           //プレイヤー
	LoadPlayer();			//プレイヤーのパーツ情報の読み込み	
	LoadWeapon();			//プレイヤーの武器の読み込み
	InitScrap();			//スクラップ
	InitBullet();			//バレット
	InitAttackbillboard();	//攻撃ビルボード
	InitWarning();          //警告表示
	InitEffect();			//エフェクト
	InitParticle();			//パーティクル
	InitEmitter();			//エミッター
	InitEdit();				//モデルのエディット
	InitEnemyEditor();      //敵のエディット
	InitCamera();			//カメラ
	InitLight();			//ライト
	InitEditCamera();		//エディタ用カメラ
	InitWall();				//壁
	InitModel();            //モデル
	InitOutletDisp();       //充電器位置表示
	InitAttackModel();		//攻撃モデル
	InitEnemyGauge();		//敵の体力ゲージ
	InitEnemyWeakDisp();    //敵の弱点表記
	InitEnemy();			//敵
	InitBoss();				//ボス
	InitCeiling();			//天井

	//UI
	InitTimer();			//タイマー
	InitScore();			//スコア
	InitGauge();			//プレイヤーの各ステータスレベルゲージ
	InitLevel();			//プレイヤーの各ステータスレベル
	InitLevelUpDisplay();	//プレイヤーのレベルアップ表示
	InitLevelUpDisplay2D();	//プレイヤーのレベルアップ表示
	InitCandyDisp();        //キャンディ表示
	InitCandyNum();         //キャンディ数値
	InitBattery();			//プレイヤーの体力
	InitMission();			//ミッション
	InitBossGauge();		//ボスの体力ゲージ
	InitMissionCheck();		//ミッションのチェック
	InitGraspBoss();        //ボス位置把握表示
	InitRaderCom();			//レーダー
	InitUI();				//UI
	InitCombo();			//コンボ数

	ResetPlayerPos();	//プレイヤーの位置リセット

	//情報の読み込み
	LoadEnemy();	//敵情報のパーツ情報の読み込み
	LoadModeltxt();	//モデル情報の読み込み(txt)

//#ifdef _DEBUG	//Debug
//	LoadModeltxt();	//モデル情報の読み込み(txt)
//#else			//Release
//	LoadModelbin();	//モデル情報の読み込み(bin)
//#endif

	PlaySound(SOUND_LABEL_BGM013);
}

//=======================================================================================================================================================================================================
// 終了処理
//=======================================================================================================================================================================================================
void UninitGame(void)
{

		//ゲームオブジェクト
	    UninitLight();			//ライト
		UninitCeiling();		//天井
		UninitField();		    //フィールド
		UninitStagingField();   //演出フィールド
		UninitWall();			//壁
		UninitEnemy();		    //敵
		UninitBoss();			//ボス	
		UninitEmitter();		//エミッター
		UninitEnemyGauge();	    //敵の体力ゲージ
		UninitEnemyWeakDisp();  //敵の弱点表記
		UninitEditCamera();     //エディタ用カメラの設定
		UninitEdit();		    //エディタ
		//ゲームオブジェクト
	    UninitCamera();			//ゲームカメラの設定
	    UninitAttackbillboard();//攻撃ビルボード
	    UninitParticle();		//パーティクル
	    UninitBullet();			//弾
	    UninitOutletDisp();     //充電器位置表示
	    UninitScrap();			//スクラップ
	    UninitItem();			//アイテム
	    UninitModel();			//モデル
	    UninitPlayer();			//プレイヤー
	    UninitAttackModel();	//攻撃モデル
		UninitWarning();        //警告表示
	    UninitShadow();			//影
	    UninitEffect();			//エフェクト
	    UninitEnemyEditor();        //敵エディタ
 	    UninitSummon();             //召喚 

		//UI
		UninitCandyDisp();			//キャンディ表示
		UninitCandyNum();			//キャンディ数値
		UninitGauge();				//プレイヤーの各ステータスレベルゲージ
		UninitLevel();				//プレイヤーの各ステータスレベル
		UninitLevelUpDisplay();		//レベルアップ表示
		UninitLevelUpDisplay2D();	//レベルアップ表示
		UninitBattery();			//プレイヤーの体力
		UninitMission();			//ミッション
		UninitMissionCheck();		//ミッションのチェック
		UninitBossGauge();			//ボスの体力ゲージ
		UninitRaderCom();			//レーダー
		UninitGraspBoss();			//ボス位置把握表示
		UninitUI();					//UI
		UninitCombo();              //コンボ
		UninitTimer();				//タイマー
		UninitPause();				//ポーズ画面
}

//=======================================================================================================================================================================================================
// 更新処理
//=======================================================================================================================================================================================================
void UpdateGame(void)
{
	bool bTimeUpFlag = false;
	bTimeUpFlag = CheckTime();	//時間切れかどうかの判定を取得
	Camera* pCamera = GetCamera();
	EnemyEditor* pEnemyEditor = GetEnemyEditor();
	if (GetTimer() <= 0)
	{//タイマーが０になったら、ゲームを終了させる。
		/*g_GameState = GAMESTATE_END;*/
		SetFade(MODE_RESULT);
		StopSound(SOUND_LABEL_BGM013);
		StopSound(SOUND_LABEL_BGM015);

	}

	if (GetKeyboardTrigger(DIK_P) || GetJoypadTrigger(JOYKEY_START))
	{//ポーズキーが押された
		g_bPause = g_bPause ? false : true;	//ポーズ画面表示・非表示の切り替え
	}

	//ポーズ中ではない場合
	if (g_bPause == false)
	{
		g_nCountTime++;//ゲーム開始時からの時間を計測する。

#ifdef _DEBUG
		if (GetKeyboardTrigger(DIK_F4) == true)
		{//F4キーが押された
			//オブジェクトエディタ切り替え
			g_bEdit = g_bEdit ? false : true;
		}
		if (GetKeyboardTrigger(DIK_F5) == true)
		{//F5キーが押された
			//エネミーエディタ切り替え
			g_bEditEnemy = g_bEditEnemy ? false : true;

			if (g_bEditEnemy == true)
			{
				pEnemyEditor->nIdxRaderDisp = SetRaderDisplay(RADERDISPLAY04_EDIT);
			}
			else if (g_bEditEnemy == false)
			{
				KillRaderDisplay(pEnemyEditor->nIdxRaderDisp);
			}
		}
		if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoypadTrigger(JOYKEY_BACK) == true)
		{//Enterキー or STARTが押された場合
			//リザルト画面へ
			SetFade(MODE_RESULT);
		}
#endif // _DEBUG

		UpdateField();	//フィールド
		UpdateStagingField();//演出フィールド
		UpdateWall();	//壁
		UpdateLight();	//ライト

		//ゲームフィールド
		if (g_bHouse == false)
		{
			//ゲーム中
			if (g_bEdit == false)
			{
				if (g_bEditEnemy == false)
				{
					g_nCountTime++;//ゲーム開始時からの時間を計測する。

					UpdateCamera();	//カメラ

					//ゲームオブジェクト
					UpdateModel();			//モデル
					UpdateAttackModel();	//攻撃モデル
					UpdateBossGauge();		//ボスの体力ゲージ
					UpdateItem();			//アイテム
					UpdateScrap();			//スクラップ
					UpdateBullet();			//弾
					UpdateAttackbillboard();//攻撃ビルボード
					UpdateWarning();        //警告表示
					UpdateShadow();			//影
					UpdateEffect();			//エフェクト
					UpdateParticle();		//パーティクル
					UpdateEmitter();		//エミッター
					UpdateEnemyGauge();		//敵の体力ゲージ
					UpdateEnemyWeakDisp();  //敵の弱点表記
					UpdateOutletDisp();     //充電器位置表示
					UpdatePlayer();	        //プレイヤー
					UpdateBoss();			//ボス
					
					if (pCamera->nMode == CAMERAMODE_REVOLUTION)
					{
						UpdateSummon();	//敵召喚

						//UI
						UpdateEnemy();	//敵(俯瞰視点の時は動かない）
						UpdateTimer();	//敵(俯瞰視点の時は動かない）
					}

					UpdateCandyDisp();      //キャンディ表示
					UpdateCandyNum();       //キャンディ数値
					UpdateScore();			//スコア
					UpdateGauge();			//プレイヤーの各ステータスレベルゲージ
					UpdateLevel();			//プレイヤーの各ステータスレベル
					UpdateLevelUpDisplay(); //レベルアップ表示
					UpdateLevelUpDisplay2D(); //レベルアップ表示
					UpdateMission();		//ミッション
					UpdateMissionCheck();	//ミッションのチェック
					UpdateBossGauge();		//ボスの体力ゲージ
					UpdateBattery();		//プレイヤーの体力
					UpdateRaderCom();       //レーダー
					UpdateGraspBoss();      //ボス位置把握表示
					UpdateUI();				//ゲーム画面UI
					UpdateCombo();			//コンボ数
				}
			}

			//エディタ中
			else
			{
				if (GetKeyboardTrigger(DIK_F6) == true)
				{//F6キーが押された
					//ステージ切り替え処理
					ChangeStage();
				}

				UpdateEdit();		//モデルエディタ
				UpdateEditCamera();	//エディタ用カメラ
			}

			if (g_bEditEnemy == true)
			{
				UpdateCamera();
				UpdateEnemyEditor();
				SetPositionRaderDisPlay(pEnemyEditor->nIdxRaderDisp, pEnemyEditor->pos, 0.0f);
				UpdateRaderCom();       //レーダー(敵エディタの時も動く)

			}
		}

		//チュートリアルフィールド
		else
		{
			//ゲーム中
			if (g_bEdit == false)
			{
				g_nCountTime++;//ゲーム開始時からの時間を計測する。

				//ゲームオブジェクト
				UpdateCamera();	//ゲームカメラ
				UpdateModel();	//モデル
				UpdatePlayer();	//プレイヤー
				UpdateShadow();	//影

				//UI
				UpdateTimer();	//タイマー
				UpdateScore();	//スコア
			}

			//エディタ中
			else
			{
				UpdateEdit();		//エディタ
				UpdateEditCamera();	//エディタ用カメラ
			}
		}
	}

	//ポーズ中の場合
	else
	{
		UpdatePause();	//ポーズ画面
		if (GetKeyboardTrigger(DIK_S) == true || GetJoypadTrigger(JOYKEY_DOWN) == true)
		{
			g_nChoosePauseCommand++;//下に選択を移動する。
		}
		if (GetKeyboardTrigger(DIK_W) == true || GetJoypadTrigger(JOYKEY_UP) == true)
		{
			g_nChoosePauseCommand--;//上に選択を移動する
		}

		if (g_nChoosePauseCommand < 0)
		{//選択場所を下側へ移動
			g_nChoosePauseCommand = 2;
		}
		if (g_nChoosePauseCommand > 2)
		{//選択場所を上側へ移動
			g_nChoosePauseCommand = 0;
		}

		if (g_nChoosePauseCommand == PAUSE_MENU_CONTINUE)
		{//コンティニューコマンドを選択
			SetEnablePause(PAUSE_MENU_CONTINUE);

			if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoypadTrigger(JOYKEY_A) == true)
			{
				g_bPause = false;
			/*	StopSound(SOUND_LABEL_BGM004);*/

				StopSound(SOUND_LABEL_BGM013);
				StopSound(SOUND_LABEL_BGM015);
			}
		}
		else if (g_nChoosePauseCommand == PAUSE_MENU_RETRY)
		{//リトライコマンドを選択
			SetEnablePause(PAUSE_MENU_RETRY);
			if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoypadTrigger(JOYKEY_A) == true)
			{
				SetFade(MODE_GAME);
				StopSound(SOUND_LABEL_BGM013);
				StopSound(SOUND_LABEL_BGM015);
			}
		}
		else if (g_nChoosePauseCommand == PAUSE_MENU_QUIT)
		{//タイトル画面に戻るコマンドを選択
			SetEnablePause(PAUSE_MENU_QUIT);
			if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoypadTrigger(JOYKEY_A) == true)
			{
				SetFade(MODE_TITLE);
				StopSound(SOUND_LABEL_BGM013);
				StopSound(SOUND_LABEL_BGM015);
			}
		}
	}
}

//=======================================================================================================================================================================================================
// 描画処理
//=======================================================================================================================================================================================================
void DrawGame(void)
{
	//ゲームフィールド
	if (g_bHouse == false)
	{
		//エディタ中
		if (g_bEdit == true)
		{
			//ゲームオブジェクト
			DrawCeiling();		//天井
			DrawField();		//フィールド
			DrawStagingField(); //演出フィールド
			DrawWall();			//壁
			DrawEnemy();		//敵
			DrawBoss();			//ボス	
			DrawEmitter();		//エミッター
			DrawEnemyGauge();	//敵の体力ゲージ
			DrawEnemyWeakDisp();//敵の弱点表記
			SetEditCamera();//エディタ用カメラの設定
			DrawEdit();		//エディタ
		}
		//ゲーム中
		else
		{
			//ゲームオブジェクト
			DrawCeiling();			//天井
			DrawField();			//フィールド
			DrawStagingField();		//演出フィールド
			DrawWall();				//壁
			SetCamera();			//ゲームカメラの設定
			DrawAttackbillboard();	//攻撃ビルボード
			DrawParticle();			//パーティクル
			DrawEnemyGauge();		//敵の体力ゲージ
			DrawEnemyWeakDisp();	//敵の弱点表記
			DrawBullet();			//弾
			DrawOutletDisp();       //充電器位置表示
			DrawScrap();			//スクラップ
			DrawItem();				//アイテム
			DrawModel();			//モデル
			DrawPlayer();			//プレイヤー
			DrawAttackModel();		//攻撃モデル
			DrawWarning();          //警告表示
			DrawEnemy();			//敵
			DrawBoss();				//ボス	
			DrawShadow();			//影
			DrawEffect();			//エフェクト
			
			//UI
			DrawCandyDisp();		//キャンディ表示
			DrawCandyNum();			//キャンディ数値
			DrawGauge();			//プレイヤーの各ステータスレベルゲージ
			DrawLevel();			//プレイヤーの各ステータスレベル
			DrawLevelUpDisplay();	//レベルアップ表示
			DrawLevelUpDisplay2D();	//レベルアップ表示

			DrawBattery();			//プレイヤーの体力
			DrawMission();			//ミッション
			DrawMissionCheck();		//ミッションのチェック
			DrawBossGauge();		//ボスの体力ゲージ
			DrawRaderCom();			//レーダー
			DrawGraspBoss();		//ボス位置把握表示
			DrawComboUI();			//コンボUI
			DrawComboNum();			//コンボ倍率
			DrawUI();				//UI
			DrawTimer();			//タイマー
			DrawScore();			//スコア

	
			//===========================
			//敵エディタを使う
			//===========================
			if (g_bEditEnemy == true)
			{
				DrawEnemyEditor();
				DrawSummon();
			}

			//===============================================================
	
		}
	}

	//ちびロボハウス
	else
	{
		//ゲームオブジェクト
		DrawField();	//フィールド
		DrawWall();		//壁
		DrawPlayer();	//プレイヤー

		//エディタ中
		if (g_bEdit == true)
		{
			SetEditCamera();//エディタ用カメラの設定
			DrawEdit();		//エディタ
		}
		//ゲーム中
		else
		{
			SetCamera();//ゲーム用カメラの設定
			DrawModel();	//モデル

			//UI
			DrawTimer();//タイマー
			DrawScore();//スコア
		}
	}

	//ポーズ中
	if (g_bPause == true)
	{
		DrawPause();//ポーズ画面
	}

}

//=======================================================================================================================================================================================================
// ステージ切り替え
//=======================================================================================================================================================================================================
void ChangeStage(void)
{
	g_bHouse = g_bHouse ? false : true;
}

//=======================================================================================================================================================================================================
// ゲームの状態の設定
//=======================================================================================================================================================================================================
void SetGameState(GAMESTATE state)
{
	g_GameState = state;
}

//=======================================================================================================================================================================================================
// ゲームの状態の取得
//=======================================================================================================================================================================================================
GAMESTATE GetGameState(void)
{
	return g_GameState;
}

//=======================================================================================================================================================================================================
// エディタ中かどうかの判定の取得
//=======================================================================================================================================================================================================
bool GetEditMode(void)
{
	return g_bEdit;
}

//=======================================================================================================================================================================================================
// ちびロボハウス内かどうかの判定の取得
//=======================================================================================================================================================================================================
bool GetHouse(void)
{
	return g_bHouse;
}

//=======================================================================================================================================================================================================
// ポーズ中かどうかの判定を取得
//=======================================================================================================================================================================================================
bool GetPause(void)
{
	return g_bPause;
}

//=======================================================================================================================================================================================================
// 敵エディタ中かどうかの判定の取得
//=======================================================================================================================================================================================================
bool GetEditEnemyMode(void)
{
	return g_bEditEnemy;
}
