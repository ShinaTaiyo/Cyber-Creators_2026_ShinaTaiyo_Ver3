//=======================================================================================================================================================================================================================
//
// チュートリアル画面の処理
// Author : Atsumu Kuboichi
//
//=======================================================================================================================================================================================================================
#include "tutorial.h"
#include "field.h"
#include "camera.h"
#include "wall.h"
#include "player.h"
#include "shadow.h"
#include "input.h"
#include "fade.h"
#include "attackmodel.h"
#include "bullet.h"
#include "operate.h"
#include "check.h"
#include "sound.h"
#include "model.h"
#include "rule.h"
#include "dummy.h"
#include "billboard.h"
#include "particle.h"
#include "effect.h"
#include "light.h"

//=======================================================================================================================================================================================================================
// グローバル変数
//=======================================================================================================================================================================================================================
bool bLoad = false;	//マップ情報が更新済みかどうか

//=======================================================================================================================================================================================================================
// 初期化処理
//=======================================================================================================================================================================================================================
void InitTutorial(void)
{
	InitShadow();		//影
	InitCamera();		//カメラ
	InitLight();		//ライト
	InitField();        //フィールド
	InitWall();         //壁
	InitModel();        //モデル
	LoadModeltxt();		//モデル情報の読み込み(txt)

	InitPlayer();       //プレイヤー
	LoadPlayer();		//プレイヤーのパーツ情報の読み込み	
	LoadWeapon();		//プレイヤーの武器の読み込み
	InitAttackModel();	//攻撃演出
	InitBullet();		//弾
	InitOperate();		//チュートリアル用UI
	InitDummy();		//ダミー人形
	InitParticle();     //パーティクル
	InitBillboard();	//ビルボード
	InitEffect();		//エフェクト
	InitRule();			//ルール説明画面の初期化処理
	ResetPlayerPos();	//プレイヤーの位置リセット

	//モデルの構造体情報を取得
	Model* pModel = GetModel();

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++, pModel++)
	{
		if (pModel->nUse == 1)
		{//機械の上に設定
			SetBillboard(D3DXVECTOR3(pModel->pos.x, pModel->vtxMax.y, pModel->pos.z), pModel->nUse, 0, 200.0f, 50.0f);//ビルボードの設定
		}
	}

	PlaySound(SOUND_LABEL_BGM012);
}

//=======================================================================================================================================================================================================================
// 終了処理
//=======================================================================================================================================================================================================================
void UninitTutorial(void)
{
	UninitShadow();		//影
	UninitCamera();		//カメラ
	UninitField();        //フィールド
	UninitWall();         //壁
	UninitModel();        //モデル
	UninitPlayer();       //プレイヤー
	UninitAttackModel();	//攻撃演出
	UninitBullet();		//弾
	UninitOperate();		//チュートリアル用UI
	UninitDummy();		//ダミー人形
	UninitParticle();     //パーティクル
	UninitBillboard();	//ビルボード
	UninitEffect();		//エフェクト
	UninitRule();			//ルール説明画面の初期化処理
}

//=======================================================================================================================================================================================================================
// 更新処理
//=======================================================================================================================================================================================================================
void UpdateTutorial(void)
{
	Model* pModel = GetModel();			//モデルの構造体の情報を取得
	Player* pPlayer = GetPlayer();		//プレイヤーの構造体の情報を取得
	Rule* pRuleScreen = GetRuleScreen();//ルール説明画面の構造体の情報の取得

	if (bLoad)
	{//マップ情報が更新されている場合
		if (!pRuleScreen->bUse)
		{//ルール説明画面を使用していない場合
			UpdateCamera();		//カメラ
			UpdateField();		//フィールド
			UpdateWall();		//壁
			UpdatePlayer();		//プレイヤー
			UpdateShadow();		//影
			UpdateAttackModel();//攻撃演出
			UpdateBullet();		//弾
			UpdateOperate();	//チュートリアル用UI
			UpdateModel();		//モデル
			UpdateDummy();		//ダミー人形
			UpdateBillboard();	//ビルボード
			UpdateParticle();   //パーティクル
			UpdateEffect();		//エフェクト

			for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++, pModel++)
			{
				if (pPlayer->pos.x >= pModel->pos.x + pModel->vtxMin.x && pPlayer->pos.x <= pModel->pos.x + pModel->vtxMax.x
					&& pPlayer->pos.z >= pModel->pos.z + (pModel->vtxMin.z - 50.0f) && pPlayer->pos.z <= pModel->pos.z + pModel->vtxMax.z
					&& pModel->nType == 23 && GetJoypadTrigger(JOYKEY_A))
				{//プレイヤーが機械の前でAボタンが押された場合
					InitRule();//ルール画面の初期化処理
					pRuleScreen->bUse = true;//ルール説明画面を表示
				}
			}

			if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(JOYKEY_START))
			{//Enterキー or STARTボタンが押された場合
				PlaySound(SOUND_LABEL_SE_DECIDE_000);
				StopSound(SOUND_LABEL_BGM012);
				//ゲーム画面へ
				SetFade(MODE_GAME);
			}
		}
		else
		{//ルール説明画面を使用している場合
			UpdateRule();	//ルール説明画面
		}
	}
	else
	{//マップ情報が更新されていない場合
		UpdateRule();//ルール説明画面

		if (!pRuleScreen->bUse)
		{//最初のルール説明画面が閉じた場合
			bLoad = true;
		}
	}
}

//=======================================================================================================================================================================================================================
// 描画処理
//=======================================================================================================================================================================================================================
void DrawTutorial(void)
{
	Rule* pRuleScreen = GetRuleScreen();//ルール説明画面の構造体の情報の取得

	if (bLoad)
	{//マップ情報が更新されている場合
		SetCamera();		//カメラのセット
		DrawModel();		//モデル
		DrawDummy();		//ダミー人形
		DrawPlayer();		//プレイヤー
		DrawField();		//フィールド
		DrawWall();			//壁
		DrawShadow();		//影
		DrawBillboard();	//ビルボード
		DrawAttackModel();	//攻撃演出
		DrawEffect();		//エフェクト
		DrawBullet();		//弾
		DrawParticle();     //パーティクル
		DrawOperate();		//チュートリアル用UI
	}
	
	if (pRuleScreen->bUse)
	{//ルール説明画面を使用している場合
		DrawRule();	//ルール説明画面
	}
}
