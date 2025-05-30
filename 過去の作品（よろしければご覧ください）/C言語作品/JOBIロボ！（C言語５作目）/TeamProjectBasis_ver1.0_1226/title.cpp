//=========================================================
//
// タイトル画面の処理
// Author : Ueda Kou
//
//=========================================================
#include "title.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "player.h"
#include "field.h"
#include "wall.h"
#include "camera.h"
#include "light.h"
#include "model.h"
#include "billboard.h"

//=========================================================
// グローバル変数
//=========================================================
//++++++++++++++++++++++++++++++++
// タイトル全体
//++++++++++++++++++++++++++++++++
int g_nCntTitle;
int g_nSelect;
//++++++++++++++++++++++++++++++++
// タイトルロゴ
//++++++++++++++++++++++++++++++++
LPDIRECT3DTEXTURE9 g_pTextureTitleLogo = {};	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitleLogo = NULL;	//頂点バッファへのポインタ
TITLELOGO g_aTitleLogo;										//構造体情報
//++++++++++++++++++++++++++++++++
// タイトルメニュー
//++++++++++++++++++++++++++++++++
LPDIRECT3DTEXTURE9 g_pTextureTitleMenu[TITLEMENU_MAX] = {};	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitleMenu = NULL;	//頂点バッファへのポインタ
TITLEMENU g_aTitleMenu[TITLEMENU_MAX];

//*******************************************************************
// タイトル全体処理
//*******************************************************************
//=========================================================
// 初期化処理
//=========================================================
void InitTitle(void)
{
	g_nCntTitle = 0;//経過時間
	g_nSelect = 0;	//選択
	
	InitTitleLogo();//ロゴ
	InitTitleMenu();//メニュー
	InitPlayer();	//プレイヤー
	InitField();	//フィールド
	InitWall();		//壁
	InitCamera();	//カメラ
	InitLight();	//ライト
	InitBG();		//背景
	InitModel();	//モデル

	LoadPlayer();	//プレイヤーのパーツ情報の読み込み	
	LoadWeapon();	//プレイヤーの武器の読み込み
	LoadModeltxt();	//モデル情報の読み込み(txt)

	PlaySound(SOUND_LABEL_BGM001);
}

//=========================================================
// 終了処理
//=========================================================
void UninitTitle(void)
{
	g_nCntTitle = 0;
	UninitTitleLogo();//ロゴ
	UninitTitleMenu();//メニュー
	UninitPlayer();	//プレイヤー
	UninitField();	//フィールド
	UninitWall();		//壁
	UninitCamera();	//カメラ
	UninitLight();	//ライト
	UninitBG();		//背景
	UninitModel();	//モデル

	
}

//=========================================================
// 更新処理
//=========================================================
void UpdateTitle(void)
{
	UpdateTitleLogo();	//ロゴ
	UpdateTitleMenu();	//メニュー
	UpdatePlayer();		//プレイヤー
	UpdateField();		//フィールド
	UpdateWall();		//壁
	UpdateCamera();		//カメラ
	UpdateModel();		//モデル

	g_nCntTitle++;

	// 注意!!(これより後ろに処理を書かない
	SelectTitle();		// メインメニュー
}

//=========================================================
// 描画処理
//=========================================================
void DrawTitle(void)
{
	SetCamera();	//カメラ
	DrawModel();	//モデル
	DrawPlayer();	//プレイヤー
	DrawField();	//フィールド
	DrawWall();		//壁
	DrawTitleLogo();//ロゴ
	DrawTitleMenu();//メニュー
}

//=========================================================
// 選択処理
//=========================================================
void SelectTitle(void)
{
	if (GetKeyboardTrigger(DIK_UP) == true || GetKeyboardTrigger(DIK_W) == true || GetJoypadTrigger(JOYKEY_UP))
	{// 上矢印キーが押されたら
		g_nSelect--;
		if (g_nSelect < 0)
		{
			g_nSelect = TITLEMENU_MAX - 1;
		}

		PlaySound(SOUND_LABEL_SE_SELECT_000);

		SelectTitleMenu();
	}
	else if (GetKeyboardTrigger(DIK_DOWN) == true || GetKeyboardTrigger(DIK_S) == true || GetJoypadTrigger(JOYKEY_DOWN))
	{// 下矢印キーが押されたら
		g_nSelect++;
		if (g_nSelect >= TITLEMENU_MAX)
		{
			g_nSelect = 0;
		}

		PlaySound(SOUND_LABEL_SE_SELECT_000);


		SelectTitleMenu();
	}

	//Enterキー or STARTボタンが押された場合
	if ((GetKeyboardTrigger(DIK_RETURN) == true && g_nCntTitle > TITLELOGO_TIME || GetJoypadTrigger(JOYKEY_START) == true || GetJoypadTrigger(JOYKEY_A) == true) && g_nCntTitle > TITLELOGO_TIME)
	{

		PlaySound(SOUND_LABEL_SE_DECIDE_000);

		switch (g_nSelect)
		{
		case TITLEMENU_START:
			StopSound(SOUND_LABEL_BGM001);
			SetFade(MODE_TUTORIAL);	//チュートリアル画面へ移行
			break;
		case TITLEMENU_RANKING:
			SetFade(MODE_RANKING);	//ランキング画面へ移行
			break;
		case TITLEMENU_END:
			SetMode(MODE_END);	//終了画面へ移行
			break;
		}

	}
}

//*******************************************************************
// タイトルロゴ処理
//*******************************************************************
//=========================================================
// 初期化処理
//=========================================================
void InitTitleLogo(void)
{
	// 初期化
	{
		g_aTitleLogo.pos = TITLELOGO_POS;								// 位置初期化
		g_aTitleLogo.postarget = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置(目標)初期化
		g_aTitleLogo.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0);			// 向き初期化
		g_aTitleLogo.rottarget = D3DXVECTOR3(0.0f, 0.0f, 0.0);		// 向き(目標)初期化
		g_aTitleLogo.size = TITLELOGO_SIZE;								// サイズ初期化
		g_aTitleLogo.fScale = 1.0f;									// サイズ(倍率)初期化
		g_aTitleLogo.fScaletarget = 1.0f;							// サイズ(目標)初期化
		g_aTitleLogo.fAngle = atan2f(TITLELOGO_SIZE.x, TITLELOGO_SIZE.y);	// 頂点角度
		g_aTitleLogo.fLeng = sqrtf((TITLELOGO_SIZE.x) * (TITLELOGO_SIZE.x) + (TITLELOGO_SIZE.y) * (TITLELOGO_SIZE.y)) * 0.5f;
		g_aTitleLogo.nArrivalType = TITLEROGO_ROLL;						// 出現方法初期化
	}

	VERTEX_2D* pVtx;						//頂点情報へのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスの取得

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Title_bg_001.png"
		, &g_pTextureTitleLogo);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitleLogo,
		NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitleLogo->Lock(0, 0, (void**)&pVtx, 0);

	// デフォルト設定

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_aTitleLogo.pos.x - (g_aTitleLogo.size.x * 0.5f) * g_aTitleLogo.fScale, g_aTitleLogo.pos.y - (g_aTitleLogo.size.y * 0.5f) * g_aTitleLogo.fScale, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aTitleLogo.pos.x + (g_aTitleLogo.size.x * 0.5f) * g_aTitleLogo.fScale, g_aTitleLogo.pos.y - (g_aTitleLogo.size.y * 0.5f) * g_aTitleLogo.fScale, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aTitleLogo.pos.x - (g_aTitleLogo.size.x * 0.5f) * g_aTitleLogo.fScale, g_aTitleLogo.pos.y + (g_aTitleLogo.size.y * 0.5f) * g_aTitleLogo.fScale, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aTitleLogo.pos.x + (g_aTitleLogo.size.x * 0.5f) * g_aTitleLogo.fScale, g_aTitleLogo.pos.y + (g_aTitleLogo.size.y * 0.5f) * g_aTitleLogo.fScale, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	
	//頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 出現方法別処理
	switch (g_aTitleLogo.nArrivalType)
	{
	case TITLELOGO_NORMAL:
		break;
	case TITLEROGO_ROLL:
		// もし回転出現なら位置を変更する
		g_aTitleLogo.pos = TITLELOGO_ARR_ROLL_POS;
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aTitleLogo.pos.x - (g_aTitleLogo.size.x * 0.5f) * g_aTitleLogo.fScale, g_aTitleLogo.pos.y - (g_aTitleLogo.size.y * 0.5f) * g_aTitleLogo.fScale, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aTitleLogo.pos.x + (g_aTitleLogo.size.x * 0.5f) * g_aTitleLogo.fScale, g_aTitleLogo.pos.y - (g_aTitleLogo.size.y * 0.5f) * g_aTitleLogo.fScale, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aTitleLogo.pos.x - (g_aTitleLogo.size.x * 0.5f) * g_aTitleLogo.fScale, g_aTitleLogo.pos.y + (g_aTitleLogo.size.y * 0.5f) * g_aTitleLogo.fScale, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aTitleLogo.pos.x + (g_aTitleLogo.size.x * 0.5f) * g_aTitleLogo.fScale, g_aTitleLogo.pos.y + (g_aTitleLogo.size.y * 0.5f) * g_aTitleLogo.fScale, 0.0f);
		break;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffTitleLogo->Unlock();
}

//=========================================================
// 終了処理
//=========================================================
void UninitTitleLogo(void)
{
	//頂点バッファの破棄
	if (g_pVtxBuffTitleLogo != NULL)
	{
		g_pVtxBuffTitleLogo->Release();
		g_pVtxBuffTitleLogo = NULL;
	}
	//テクスチャの破棄
	if (g_pTextureTitleLogo != NULL)
	{
		g_pTextureTitleLogo->Release();
		g_pTextureTitleLogo = NULL;
	}
}

//=========================================================
// 更新処理
//=========================================================
void UpdateTitleLogo(void)
{
	VERTEX_2D* pVtx;						//頂点情報へのポインタ
	if ((GetKeyboardTrigger(DIK_RETURN) == true || GetJoypadTrigger(JOYKEY_START) == true || GetJoypadTrigger(JOYKEY_A)) && g_nCntTitle < TITLELOGO_TIME)
	{// タイトルアニメーションを強制終了
		g_nCntTitle = TITLELOGO_TIME - 1;

		//頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffTitleLogo->Lock(0, 0, (void**)&pVtx, 0);

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(TITLELOGO_RED_POS.x - (g_aTitleLogo.size.x * 0.5f) * TITLELOGO_RED_SCALE, TITLELOGO_RED_POS.y - (g_aTitleLogo.size.y * 0.5f) * TITLELOGO_RED_SCALE, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(TITLELOGO_RED_POS.x + (g_aTitleLogo.size.x * 0.5f) * TITLELOGO_RED_SCALE, TITLELOGO_RED_POS.y - (g_aTitleLogo.size.y * 0.5f) * TITLELOGO_RED_SCALE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(TITLELOGO_RED_POS.x - (g_aTitleLogo.size.x * 0.5f) * TITLELOGO_RED_SCALE, TITLELOGO_RED_POS.y + (g_aTitleLogo.size.y * 0.5f) * TITLELOGO_RED_SCALE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(TITLELOGO_RED_POS.x + (g_aTitleLogo.size.x * 0.5f) * TITLELOGO_RED_SCALE, TITLELOGO_RED_POS.y + (g_aTitleLogo.size.y * 0.5f) * TITLELOGO_RED_SCALE, 0.0f);
		//頂点バッファをアンロックする
		g_pVtxBuffTitleLogo->Unlock();
	}
	else
	{// タイトルアニメーション
		// 出現方法
		if (g_nCntTitle <= TITLELOGO_ARR_TIME)
		{
			switch (g_aTitleLogo.nArrivalType)
			{
			case TITLELOGO_NORMAL:
				break;
			case TITLEROGO_ROLL:
				{
			
					if (g_nCntTitle == 0)	// 目標率設定
					{
						g_aTitleLogo.rottarget = g_aTitleLogo.rot - (TITLELOGO_ARR_ROLL_ROT) / TITLELOGO_RED_TIME;
						// 目標位置 = 現在座標と目標座標の差異 / 変更にかける時間
						g_aTitleLogo.postarget.x = (g_aTitleLogo.pos.x - TITLELOGO_POS.x) / TITLELOGO_RED_TIME;
						g_aTitleLogo.postarget.y = (g_aTitleLogo.pos.y - TITLELOGO_POS.y) / TITLELOGO_RED_TIME;
						g_aTitleLogo.postarget.z = 0.0f;
					}
					else
					{
						g_aTitleLogo.pos -= g_aTitleLogo.postarget;
						g_aTitleLogo.rot.z += g_aTitleLogo.rottarget.z;
					}

					//頂点バッファをロックし、頂点情報へのポインタを取得
					g_pVtxBuffTitleLogo->Lock(0, 0, (void**)&pVtx, 0);

					//頂点座標の設定
					pVtx[0].pos = D3DXVECTOR3(g_aTitleLogo.pos.x + sinf(g_aTitleLogo.rot.z - (D3DX_PI - g_aTitleLogo.fAngle)) * g_aTitleLogo.fLeng * g_aTitleLogo.fScale, g_aTitleLogo.pos.y + cosf(g_aTitleLogo.rot.z - (D3DX_PI - g_aTitleLogo.fAngle)) * g_aTitleLogo.fLeng * g_aTitleLogo.fScale, 0.0f);
					pVtx[1].pos = D3DXVECTOR3(g_aTitleLogo.pos.x + sinf(g_aTitleLogo.rot.z + (D3DX_PI - g_aTitleLogo.fAngle)) * g_aTitleLogo.fLeng * g_aTitleLogo.fScale, g_aTitleLogo.pos.y + cosf(g_aTitleLogo.rot.z + (D3DX_PI - g_aTitleLogo.fAngle)) * g_aTitleLogo.fLeng * g_aTitleLogo.fScale, 0.0f);
					pVtx[2].pos = D3DXVECTOR3(g_aTitleLogo.pos.x + sinf(g_aTitleLogo.rot.z - (g_aTitleLogo.fAngle)) * g_aTitleLogo.fLeng * g_aTitleLogo.fScale, g_aTitleLogo.pos.y + cosf(g_aTitleLogo.rot.z - (g_aTitleLogo.fAngle)) * g_aTitleLogo.fLeng * g_aTitleLogo.fScale, 0.0f);
					pVtx[3].pos = D3DXVECTOR3(g_aTitleLogo.pos.x + sinf(g_aTitleLogo.rot.z + (g_aTitleLogo.fAngle)) * g_aTitleLogo.fLeng * g_aTitleLogo.fScale, g_aTitleLogo.pos.y + cosf(g_aTitleLogo.rot.z + (g_aTitleLogo.fAngle)) * g_aTitleLogo.fLeng * g_aTitleLogo.fScale, 0.0f);
					//頂点バッファをアンロックする
					g_pVtxBuffTitleLogo->Unlock();
				}
				break;
			}
		}
		// タイトル縮小
		else if (g_nCntTitle >= TITLELOGO_ARR_TIME + TITLELOGO_RED_START && g_nCntTitle < TITLELOGO_ARR_TIME + TITLELOGO_RED_START + TITLELOGO_RED_TIME)
		{
			if (g_nCntTitle == TITLELOGO_ARR_TIME + TITLELOGO_RED_START)	// 目標率設定
			{
				// 目標サイズ = 現在倍率と目標倍率の差異 / 変更にかける時間
				g_aTitleLogo.fScaletarget = (g_aTitleLogo.fScale - TITLELOGO_RED_SCALE) / TITLELOGO_RED_TIME;
				// 目標位置 = 現在座標と目標座標の差異 / 変更にかける時間
				g_aTitleLogo.postarget.x = (g_aTitleLogo.pos.x - TITLELOGO_RED_POS.x) / TITLELOGO_RED_TIME;
				g_aTitleLogo.postarget.y = (g_aTitleLogo.pos.y - TITLELOGO_RED_POS.y) / TITLELOGO_RED_TIME;
				g_aTitleLogo.postarget.z = 0.0f;
			}
			else
			{
				g_aTitleLogo.fScale -= g_aTitleLogo.fScaletarget;
				g_aTitleLogo.pos -= g_aTitleLogo.postarget;
			}

			//頂点バッファをロックし、頂点情報へのポインタを取得
			g_pVtxBuffTitleLogo->Lock(0, 0, (void**)&pVtx, 0);

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aTitleLogo.pos.x - (g_aTitleLogo.size.x * 0.5f) * g_aTitleLogo.fScale, g_aTitleLogo.pos.y - (g_aTitleLogo.size.y * 0.5f) * g_aTitleLogo.fScale, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aTitleLogo.pos.x + (g_aTitleLogo.size.x * 0.5f) * g_aTitleLogo.fScale, g_aTitleLogo.pos.y - (g_aTitleLogo.size.y * 0.5f) * g_aTitleLogo.fScale, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aTitleLogo.pos.x - (g_aTitleLogo.size.x * 0.5f) * g_aTitleLogo.fScale, g_aTitleLogo.pos.y + (g_aTitleLogo.size.y * 0.5f) * g_aTitleLogo.fScale, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aTitleLogo.pos.x + (g_aTitleLogo.size.x * 0.5f) * g_aTitleLogo.fScale, g_aTitleLogo.pos.y + (g_aTitleLogo.size.y * 0.5f) * g_aTitleLogo.fScale, 0.0f);
			//頂点バッファをアンロックする
			g_pVtxBuffTitleLogo->Unlock();
		}
	}
}

//=========================================================
// 描画処理
//=========================================================
void DrawTitleLogo(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();						//デバイスの取得
	pDevice->SetStreamSource(0, g_pVtxBuffTitleLogo, 0, sizeof(VERTEX_2D));//頂点バッファをデータストリームに設定
	pDevice->SetFVF(FVF_VERTEX_2D);									//頂点フォーマットの設定
	pDevice->SetTexture(0, g_pTextureTitleLogo);				//テクスチャの設定
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);				//ポリゴンの描画
}

//*******************************************************************
// タイトル選択処理
//*******************************************************************
//=========================================================
// 初期化処理
//=========================================================
void InitTitleMenu(void)
{
	// 初期化
	for (int nCnt = 0; nCnt < TITLEMENU_MAX; nCnt++)
	{
		g_aTitleMenu[nCnt].pos.x = TITLEMENU_POS.x ;														// 位置初期化
		g_aTitleMenu[nCnt].pos.y = TITLEMENU_POS.y + (TITLEMENU_SIZE .y * nCnt) + (TITLEMENU_FALL * nCnt);	// 位置初期化
		g_aTitleMenu[nCnt].pos.z = TITLEMENU_POS.z;															// 位置初期化
		g_aTitleMenu[nCnt].size = TITLEMENU_SIZE;															// サイズ初期化
		g_aTitleMenu[nCnt].col = TITLEMENU_COL_NOTSELECT;													// 色初期化
		g_aTitleMenu[nCnt].col.a = 0.0f;																	// 色初期化
		g_aTitleMenu[nCnt].fColaTarget = 0.0f;																// 色a(目標)初期化
		g_aTitleMenu[nCnt].nType = nCnt;																	// 種類割り当て
		g_aTitleMenu[nCnt].bUse = false;																	// 非表示にする
	}
	g_aTitleMenu[0].col = TITLEMENU_COL_SELECT;		// 選択初期
	g_aTitleMenu[0].col.a = 0.0f;					// 選択初期

	VERTEX_2D* pVtx;						//頂点情報へのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスの取得

	for (int nCnt = 0; nCnt < TITLEMENU_MAX; nCnt++)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(
			pDevice,
			cTITLEMENU_TEXTURE_FILENAME[nCnt],
			&g_pTextureTitleMenu[nCnt]);
	}
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * TITLEMENU_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitleMenu,
		NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitleMenu->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < TITLEMENU_MAX; nCnt++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aTitleMenu[nCnt].pos.x - (g_aTitleMenu[nCnt].size.x * 0.5f), g_aTitleMenu[nCnt].pos.y - (g_aTitleMenu[nCnt].size.y * 0.5f), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aTitleMenu[nCnt].pos.x + (g_aTitleMenu[nCnt].size.x * 0.5f), g_aTitleMenu[nCnt].pos.y - (g_aTitleMenu[nCnt].size.y * 0.5f), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aTitleMenu[nCnt].pos.x - (g_aTitleMenu[nCnt].size.x * 0.5f), g_aTitleMenu[nCnt].pos.y + (g_aTitleMenu[nCnt].size.y * 0.5f), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aTitleMenu[nCnt].pos.x + (g_aTitleMenu[nCnt].size.x * 0.5f), g_aTitleMenu[nCnt].pos.y + (g_aTitleMenu[nCnt].size.y * 0.5f), 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = g_aTitleMenu[nCnt].col;
		pVtx[1].col = g_aTitleMenu[nCnt].col;
		pVtx[2].col = g_aTitleMenu[nCnt].col;
		pVtx[3].col = g_aTitleMenu[nCnt].col;

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffTitleMenu->Unlock();
}
//=========================================================
// 終了処理
//=========================================================
void UninitTitleMenu(void)
{
	//頂点バッファの破棄
	if (g_pVtxBuffTitleMenu != NULL)
	{
		g_pVtxBuffTitleMenu->Release();
		g_pVtxBuffTitleMenu = NULL;
	}
	//テクスチャの破棄
	for (int nCnt = 0; nCnt < TITLEMENU_MAX; nCnt++)
	{
		if (g_pTextureTitleMenu[nCnt] != NULL)
		{
			g_pTextureTitleMenu[nCnt]->Release();
			g_pTextureTitleMenu[nCnt] = NULL;
		}
	}
}
//=========================================================
// 更新処理
//=========================================================
void UpdateTitleMenu(void)
{
	VERTEX_2D* pVtx;//頂点情報へのポインタ

	if (g_nCntTitle >= TITLELOGO_TIME)
	{
		//頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffTitleMenu->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCnt = 0; nCnt < TITLEMENU_MAX; nCnt++)
		{
			if (g_nCntTitle < TITLELOGO_TIME + TITLEMENU_TIME)
			{
				if (g_nCntTitle == TITLELOGO_TIME)
				{
					g_aTitleMenu[nCnt].bUse = true;
					g_aTitleMenu[nCnt].fColaTarget = (g_aTitleMenu[nCnt].col.a - TITLEMENU_COL_A) / TITLEMENU_TIME;
				}
				else
				{
					g_aTitleMenu[nCnt].col.a -= g_aTitleMenu[nCnt].fColaTarget;
				}
				//頂点カラーの設定
				pVtx[0].col = g_aTitleMenu[nCnt].col;
				pVtx[1].col = g_aTitleMenu[nCnt].col;
				pVtx[2].col = g_aTitleMenu[nCnt].col;
				pVtx[3].col = g_aTitleMenu[nCnt].col;

				pVtx += 4;

			}
		}
		//頂点バッファをアンロックする
		g_pVtxBuffTitleMenu->Unlock();
	}
}

//=========================================================
// 描画処理
//=========================================================
void DrawTitleMenu(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスの取得
	for (int nCnt = 0; nCnt < TITLEMENU_MAX; nCnt++)
	{
		if (g_aTitleMenu[nCnt].bUse == true)
		{
			pDevice->SetStreamSource(0, g_pVtxBuffTitleMenu, 0, sizeof(VERTEX_2D));//頂点バッファをデータストリームに設定
			pDevice->SetFVF(FVF_VERTEX_2D);									//頂点フォーマットの設定
			pDevice->SetTexture(0, g_pTextureTitleMenu[nCnt]);				//テクスチャの設定
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCnt, 2);				//ポリゴンの描画
		}
	}
}
//=========================================================
// 選択時処理
//=========================================================
void SelectTitleMenu(void)
{
	VERTEX_2D* pVtx;						//頂点情報へのポインタ
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitleMenu->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCnt = 0; nCnt < TITLEMENU_MAX; nCnt++)
	{
		if (g_nSelect == nCnt)
		{
			g_aTitleMenu[nCnt].col.r = TITLEMENU_COL_SELECT.r;
			g_aTitleMenu[nCnt].col.g = TITLEMENU_COL_SELECT.g;
			g_aTitleMenu[nCnt].col.b = TITLEMENU_COL_SELECT.b;
		}
		else
		{
			g_aTitleMenu[nCnt].col.r = TITLEMENU_COL_NOTSELECT.r;
			g_aTitleMenu[nCnt].col.g = TITLEMENU_COL_NOTSELECT.g;
			g_aTitleMenu[nCnt].col.b = TITLEMENU_COL_NOTSELECT.b;
		}
		//頂点カラーの設定
		pVtx[0].col = g_aTitleMenu[nCnt].col;
		pVtx[1].col = g_aTitleMenu[nCnt].col;
		pVtx[2].col = g_aTitleMenu[nCnt].col;
		pVtx[3].col = g_aTitleMenu[nCnt].col;

		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffTitleMenu->Unlock();
}