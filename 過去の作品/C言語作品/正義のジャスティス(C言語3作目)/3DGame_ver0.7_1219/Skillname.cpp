//========================================================
//
//８月３０日：エフェクトもう一回見直す[Skillname.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#include "main.h"
#include "game.h"
#include "enemy.h"
#include "SkillName.h"
#include "input.h"
#include "bullet.h"
#include "player.h"
#include "sound.h"

//マクロ定義
#define NUM_SKILLTEX
#define SKILLNAME_WIDTH (120.0f)
#define SKILLNAME_HEIGHT (80.0f)

//グローバル変数
LPDIRECT3DTEXTURE9 g_apTextureSkillName = NULL;//テクスチャ3枚分のポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSkillName = NULL;//頂点バッファへのポインタ

Skill g_aSkill;//スキル構造体のグローバル変数

//float g_fSkillGage = 0;
//int g_nSkillGage_Count = 0;
//Result_2 g_aResult_2;
//============================
//スキルネームの初期化処理
//============================
void InitSkillName(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBG;

	g_aSkill.fSkillPattern = 0.0f;
	g_aSkill.nSkillType = 0;
	g_aSkill.pos = D3DXVECTOR3(130.0f, 605.0f, 0.0f);

	/*g_aResult_2.bResultFlag2 = false;*/

	//g_bResultFlag2 = false;

	/* g_fSkillGage = 0;
	 g_nSkillGage_Count = 0;*/

	/*g_aSkillGage.nSkillGageCount = 0;
	g_aSkillGage.fSkillGage = 0;*/
	/*g_fAnimationPattern = 0;*/

	//デバイスの取得
	pDevice = GetDevice();


	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\3DGame_SkillTable.png",
		&g_apTextureSkillName);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,//４は必要な頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffSkillName,
		NULL);

	VERTEX_2D* pVtx;
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffSkillName->Lock(0, 0, (void**)&pVtx, 0);
	pVtx[0].pos = D3DXVECTOR3(g_aSkill.pos.x-SKILLNAME_WIDTH,g_aSkill.pos.y-SKILLNAME_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aSkill.pos.x+SKILLNAME_WIDTH,g_aSkill.pos.y-SKILLNAME_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aSkill.pos.x-SKILLNAME_WIDTH,g_aSkill.pos.y+SKILLNAME_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aSkill.pos.x+SKILLNAME_WIDTH,g_aSkill.pos.y+SKILLNAME_HEIGHT, 0.0f);
																			
	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f/6.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f / 6.0f, 1.0f);

	pVtx += 4;//頂点データのポインタを４つ分進める。

//頂点バッファをアンロックする
	g_pVtxBuffSkillName->Unlock();
}

//==========================
//スキルネームの終了処理
//==========================
void UninitSkillName(void)
{
	//テクスチャの破棄

	if (g_apTextureSkillName != NULL)
	{
		g_apTextureSkillName->Release();
		g_apTextureSkillName = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffSkillName != NULL)
	{
		g_pVtxBuffSkillName->Release();
		g_pVtxBuffSkillName = NULL;
	}
}

//==================================
//スキルネームの更新処理
//==================================
void UpdateSkillName(void)
{
	
	Player* pPlayer = GetPlayer();//プレイヤー構造体の情報をゲットする。
	Enemy* pEnemy = GetEnemy();//エネミー構造体の情報をゲットする。
	//g_aSkillGage.nSkillGageCount++;
	VERTEX_2D* pVtx;
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffSkillName->Lock(0, 0, (void**)&pVtx, 0);


	if (GetKeyboardTrigger(DIK_N)==true||GetJoyPadTrigger(JOYKEY_Y)==true)
	{//スキル選択用変数（焦点が右にスライド）
		PlaySound(SOUND_LABEL_SE_SELECT_000);
		g_aSkill.nSkillType++;
		//PlaySound(SOUND_LABEL_SE_SELECT_000);
	}
	if (GetKeyboardTrigger(DIK_B) == true)
	{//スキル選択用変数（焦点が右にスライド）
		PlaySound(SOUND_LABEL_SE_SELECT_000);

		g_aSkill.nSkillType--;
		//PlaySound(SOUND_LABEL_SE_SELECT_000);
	}

	if (g_aSkill.nSkillType > 5)
	{
		g_aSkill.nSkillType = 0;
	}
	if (g_aSkill.nSkillType < 0)
	{
		g_aSkill.nSkillType = 5;
	}


	

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2((1.0f / 6.0f) * g_aSkill.nSkillType, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f / 6.0f + (1.0f/6.0f) * g_aSkill.nSkillType, 0.0f);
	pVtx[2].tex = D3DXVECTOR2((1.0f / 6.0f) * g_aSkill.nSkillType, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f / 6.0f + (1.0f / 6.0f) * g_aSkill.nSkillType, 1.0f);


	if (pPlayer->pos.x >= g_aSkill.pos.x - SKILLNAME_WIDTH && pPlayer->pos.x <= g_aSkill.pos.x + SKILLNAME_WIDTH
		&& pPlayer->pos.y >= g_aSkill.pos.y - SKILLNAME_HEIGHT && pPlayer->pos.y <= g_aSkill.pos.y + SKILLNAME_HEIGHT)
	{
		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
	}
	else
	{

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
	}

	






	//頂点バッファをアンロックする
	g_pVtxBuffSkillName->Unlock();
}

//==================================
//スキルネームの描画処理
//==================================
void DrawSkillName(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBG;
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffSkillName, 0, sizeof(VERTEX_2D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

		//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_apTextureSkillName);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,
		2);

}

//======================================
//スキルネーム構造体の取得
//======================================
Skill* GetSkill(void)
{
	return &g_aSkill;
}
