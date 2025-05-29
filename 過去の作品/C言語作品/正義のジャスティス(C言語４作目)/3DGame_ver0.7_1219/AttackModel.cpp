//========================================================
//
//１１月７日：モデル読み込み[AttackModel.cpp]
//Author:ShinaTaiyo
//
//=========================================================

#include "AttackModel.h"
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "shadow.h"
#include "player.h"
#include <stdio.h>
#include <string.h>
#include "enemy.h"
#include "effect.h"
#include "particle.h"
#include "damage.h"
#include "emitter.h"
#include "bullet.h"
//グローバル変数
int g_nNumAttackModel = 0;//読み込むモデルの最大数を決める
#define AttackModel_SCALE (10.0f)
//#define SAVEAttackModel "data\\TEXTFILE\\SaveAttackModel.txt"

#define MAX_TEXTURE (32)


typedef struct
{
	LPD3DXMESH pMeshAttackModel;//メッシュ（頂点情報）へのポインタ（モデルの種類数分）
	LPD3DXBUFFER pBuffMatAttackModel;//マテリアルへのポインタ
	DWORD dwNumMatAttackModel;//マテリアルの数
	LPDIRECT3DTEXTURE9 apTextureAttackModel[MAX_TEXTURE];//テクスチャへのポインタ
}AttackModelMat;

D3DXVECTOR3 g_posAttackModel;//位置
D3DXVECTOR3 g_rotAttackModel;//向き
D3DXMATRIX g_mtxWorldAttackModel;//ワールドマトリックス

AttackModel g_aAttackModel[MAX_ATTACKMODEL];//モデル情報の構造体

AttackModelMat g_aAttackModelMat[NUM_ATTACKMODEL];//モデルのマテリアルや頂点の構造体


int g_nDelayAttack = 0;//ヒットストップ用変数

//int g_nldxShadow = -1;//対象の影のインデックス（番号）
//bool g_bIsLanding;//地面にいるかどうか

//タイトル画面の初期化処理
void InitAttackModel(void)
{

	//for (int nCntAttackModel = 0; nCntAttackModel < NUM_ATTACKMODEL; nCntAttackModel++)
	//{
	//	g_dwNumMatAttackModel[nCntAttackModel] = 0;//マテリアルの数
	//}
	//g_nldxShadow = 0;//対象の影のインデックス

	g_nNumAttackModel = 2;

	g_nDelayAttack = 0;//ヒットストップ用変数

	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスへのポインタと取得

	g_posAttackModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotAttackModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	g_nNumAttackModel = 0;

	//g_bIsLanding = false;//地面にいるかどうか



	////モデルの読み込み
	//D3DXLoadMeshFromX("data\\AttackModel\\bill.x",
	//	D3DXMESH_SYSTEMMEM,
	//	pDevice,
	//	NULL,
	//	&g_pBuffMatAttackModel,
	//	NULL,
	//	&g_dwNumMatAttackModel,
	//	&g_pMeshAttackModel[AttackModelTYPE_BILL]);

	//D3DXLoadMeshFromX("data\\AttackModel\\BigBriddge.x",
	//	D3DXMESH_SYSTEMMEM,
	//	pDevice,
	//	NULL,
	//	&g_pBuffMatAttackModel,
	//	NULL,
	//	&g_dwNumMatAttackModel,
	//	&g_pMeshAttackModel[AttackModelTYPE_BRIDDGE]);


	for (int nCntMatAttackModel = 0; nCntMatAttackModel < NUM_ATTACKMODEL; nCntMatAttackModel++)
	{
		g_aAttackModelMat[nCntMatAttackModel].dwNumMatAttackModel = {};
		g_aAttackModelMat[nCntMatAttackModel].pBuffMatAttackModel = {};
		g_aAttackModelMat[nCntMatAttackModel].pMeshAttackModel = {};
		for (int nCntTexture = 0; nCntTexture < MAX_TEXTURE; nCntTexture++)
		{
			g_aAttackModelMat[nCntMatAttackModel].apTextureAttackModel[nCntTexture] = {};//一つのモデルに関連するテクスチャの初期化
		}
	}


	for (int nCnt = 0; nCnt < MAX_ATTACKMODEL; nCnt++)
	{
		g_aAttackModel[nCnt].bUse = false;//使用しているかどうか
		g_aAttackModel[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//位置
		g_aAttackModel[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//向き
		g_aAttackModel[nCnt].nIdxShadow = -1;
		g_aAttackModel[nCnt].nType = -1;
		g_aAttackModel[nCnt].vtxMinAttackModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//それぞれの座標のモデルの位置の最小値
		g_aAttackModel[nCnt].vtxMaxAttackModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//それぞれの座標のモデルの位置の最大値
		g_aAttackModel[nCnt].nNumVtx = 0;//頂点数
		g_aAttackModel[nCnt].sizeFVF;//頂点フォーマットのサイズ
		g_aAttackModel[nCnt].pVtxBuff;//頂点バッファへのポインタ
		g_aAttackModel[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//移動量
		g_aAttackModel[nCnt].nLife = 0;//体力
		g_aAttackModel[nCnt].bOneFlag = false;//一回だけ発動させる処理
	}


	//モデルの読み込み
	D3DXLoadMeshFromX("data\\MODEL\\FireTornado.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_aAttackModelMat[0].pBuffMatAttackModel,
		NULL,
		&g_aAttackModelMat[0].dwNumMatAttackModel,
		&g_aAttackModelMat[0].pMeshAttackModel);

	//モデルの読み込み
	D3DXLoadMeshFromX("data\\MODEL\\Attack\\meteo.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_aAttackModelMat[1].pBuffMatAttackModel,
		NULL,
		&g_aAttackModelMat[1].dwNumMatAttackModel,
		&g_aAttackModelMat[1].pMeshAttackModel);


	//モデルの読み込み
	D3DXLoadMeshFromX("data\\MODEL\\Attack\\KillNeedle.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_aAttackModelMat[2].pBuffMatAttackModel,
		NULL,
		&g_aAttackModelMat[2].dwNumMatAttackModel,
		&g_aAttackModelMat[2].pMeshAttackModel);
	


	

	//for (int nCntAttackModel = 0; nCntAttackModel < MAX_ATTACKMODEL; nCntAttackModel++)
	//{//全てのモデルに割り当てる
	//	for (int nCntMat = 0; nCntMat < NUM_ATTACKMODEL; nCntMat++)
	//	{//種類を割り当てるfor文
	//		if (g_aAttackModel[nCntAttackModel].nType == nCntMat)
	//		{//モデルの種類が一致

	//			//頂点数の取得
	//			g_aAttackModel[nCntAttackModel].nNumVtx = g_aAttackModelMat[nCntMat].pMeshAttackModel->GetNumVertices();

	//			//頂点フォーマットのサイズを取得
	//			g_aAttackModel[nCntAttackModel].sizeFVF = D3DXGetFVFVertexSize(g_aAttackModelMat[nCntMat].pMeshAttackModel->GetFVF());

	//			//頂点バッファのロック
	//			g_aAttackModelMat[nCntMat].pMeshAttackModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&g_aAttackModel[nCntAttackModel].pVtxBuff);


	//			for (int nCntVtx = 0; nCntVtx < g_aAttackModel[nCntAttackModel].nNumVtx; nCntVtx++)
	//			{
	//				//頂点座標の代入
	//				D3DXVECTOR3 vtx = *(D3DXVECTOR3*)g_aAttackModel[nCntAttackModel].pVtxBuff;

	//				//====================================================
	//				//頂点座標を比較してモデルの最小値最大値を取得
	//				//====================================================
	//				if (vtx.x > g_aAttackModel[nCntAttackModel].vtxMaxAttackModel.x)
	//				{
	//					g_aAttackModel[nCntAttackModel].vtxMaxAttackModel.x = vtx.x;
	//				}
	//				else if (vtx.x < g_aAttackModel[nCntAttackModel].vtxMinAttackModel.x)
	//				{//今回読み込んだ頂点が、一番小さい頂点より小さい場合
	//					g_aAttackModel[nCntAttackModel].vtxMinAttackModel.x = vtx.x;
	//				}
	//				else if (vtx.y > g_aAttackModel[nCntAttackModel].vtxMaxAttackModel.y)
	//				{//今回読み込んだ頂点が、一番小さい頂点より小さい場合
	//					g_aAttackModel[nCntAttackModel].vtxMaxAttackModel.y = vtx.y;
	//				}
	//				else if (vtx.y < g_aAttackModel[nCntAttackModel].vtxMinAttackModel.y)
	//				{//今回読み込んだ頂点が、一番小さい頂点より小さい場合
	//					g_aAttackModel[nCntAttackModel].vtxMinAttackModel.y = vtx.y;
	//				}
	//				else if (vtx.z > g_aAttackModel[nCntAttackModel].vtxMaxAttackModel.z)
	//				{//今回読み込んだ頂点が、一番小さい頂点より小さい場合
	//					g_aAttackModel[nCntAttackModel].vtxMaxAttackModel.z = vtx.z;
	//				}
	//				else if (vtx.z < g_aAttackModel[nCntAttackModel].vtxMinAttackModel.z)
	//				{//今回読み込んだ頂点が、一番小さい頂点より小さい場合
	//					g_aAttackModel[nCntAttackModel].vtxMinAttackModel.z = vtx.z;
	//				}
	//				//========================================================================

	//				//頂点フォーマットのサイズ文ポインタを進める
	//				g_aAttackModel[nCntAttackModel].pVtxBuff += g_aAttackModel[nCntAttackModel].sizeFVF;

	//			}

	//			//頂点バッファのアンロック
	//			g_aAttackModelMat[nCntMat].pMeshAttackModel->UnlockVertexBuffer();

	//			//種類特定したし、もうタイプのfor文回す必要ないので、break
	//			break;

	//		}
	//	}
	//}
	//頂点数の取得

	D3DXMATERIAL* pMat;//マテリアルへのポインタ
	for (int nCntATTACKMODELSET = 0; nCntATTACKMODELSET < NUM_ATTACKMODEL; nCntATTACKMODELSET++)
	{
		//マテリアルへのポインタを取得（セットしたモデルと同じタイプ）
		pMat = (D3DXMATERIAL*)g_aAttackModelMat[nCntATTACKMODELSET].pBuffMatAttackModel->GetBufferPointer();
		int nNumTexture = 0;
		for (int nCntMat = 0; nCntMat < (int)g_aAttackModelMat[nCntATTACKMODELSET].dwNumMatAttackModel; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{//テクスチャファイルが存在する
					//テクスチャの読み込み
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_aAttackModelMat[nCntATTACKMODELSET].apTextureAttackModel[nNumTexture]);
				nNumTexture++;
			}
		}
	}

}

//==============================
//タイトル画面の終了処理
//==============================
void UninitAttackModel(void)
{
	for (int nCnt = 0; nCnt < NUM_ATTACKMODEL; nCnt++)
	{
		//メッシュの破棄
		if (g_aAttackModelMat[nCnt].pMeshAttackModel != NULL)
		{
			g_aAttackModelMat[nCnt].pMeshAttackModel->Release();
			g_aAttackModelMat[nCnt].pMeshAttackModel = NULL;
		}
		//マテリアルの破棄
		if (g_aAttackModelMat[nCnt].pBuffMatAttackModel != NULL)
		{
			g_aAttackModelMat[nCnt].pBuffMatAttackModel->Release();
			g_aAttackModelMat[nCnt].pBuffMatAttackModel = NULL;
		}
	}
}
//============================
//モデルの更新処理
//============================
void UpdateAttackModel(void)
{

	g_nDelayAttack++;

	float fRandumRot = float(rand() % 200 + 1) / 100;
	float fRandumSpeed = float(rand() % 100 + 30) / 10;
	float fRandumMove = float(rand() % 21);

	float fRandumRot2 = float(rand() % 200 + 1) / 100;
	float fRandumSpeed2 = float(rand() % 100 + 30) / 10;
	float fRandumMove2 = float(rand() % 21);


	float fRandumColor1 = float(rand() % 100) / 100;
	float fRandumColor2 = float(rand() % 100) / 100;
	float fRandumColor3 = float(rand() % 100) / 100;


	float fLength = 0.0f;
	Player* pPlayer = GetPlayer();
	for (int nCntAttack = 0; nCntAttack < MAX_ATTACKMODEL; nCntAttack++)
	{
		if (g_aAttackModel[nCntAttack].bUse == true)
		{
	        Enemy* pEnemy = GetEnemy();

			g_aAttackModel[nCntAttack].nLife -= 1;//体力を減らす


			switch (g_aAttackModel[nCntAttack].nType)
			{
			case ATTACKMODELTYPE_FIRETORNADO://プレイヤーの技：ファイアトルネード
			

				g_aAttackModel[nCntAttack].rot.y += 0.1f;

			

				SetParticle(PARTICLETYPE_GRAVITY, 300, 10.0f, 0.01f, g_aAttackModel[nCntAttack].pos,
					D3DXVECTOR3(sinf(-D3DX_PI * fRandumRot) * fRandumSpeed, fRandumMove, cosf(-D3DX_PI * fRandumRot) * fRandumSpeed),
					D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), 0.0f, PARTICLETEX_NORMAL);

			
				SetParticle(PARTICLETYPE_GRAVITY, 300, 10.0f, 0.01f, g_aAttackModel[nCntAttack].pos,
					D3DXVECTOR3(sinf(-D3DX_PI * fRandumRot2) * fRandumSpeed2, fRandumMove2, cosf(-D3DX_PI * fRandumRot2) * fRandumSpeed2),
					D3DXCOLOR(1.0f, 0.3f, 0.3f, 1.0f), 0.0f, PARTICLETEX_NORMAL);

				if (g_aAttackModel[nCntAttack].bOneFlag == true)
				{//一度だけエミッターを周囲に召喚する
					for (int nCntEmitter = 0; nCntEmitter < 6; nCntEmitter++)
					{
						float fEmitterPos = 0;
						fEmitterPos = (2.0f / 6) * nCntEmitter;

						SetEmitterEffect(EMITTERTYPE_TORNADO, 1000, 10, 20.0f, 0.1f, g_aAttackModel[nCntAttack].pos, D3DXVECTOR3(0.0f, 0.001f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), fEmitterPos,nCntAttack);

					}
					g_aAttackModel[nCntAttack].bOneFlag = false;//この処理をもう発動させない
				}

				for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				{
					if (pEnemy->bUse == true)
					{
						if (pEnemy->pos.x + pEnemy->Size.x / 2 >= g_aAttackModel[nCntAttack].pos.x + g_aAttackModel[nCntAttack].vtxMinAttackModel.x &&
							pEnemy->pos.x - pEnemy->Size.x / 2 <= g_aAttackModel[nCntAttack].pos.x + g_aAttackModel[nCntAttack].vtxMaxAttackModel.x &&
							pEnemy->pos.y + pEnemy->Size.y / 2 >= g_aAttackModel[nCntAttack].pos.y + g_aAttackModel[nCntAttack].vtxMinAttackModel.y &&
							pEnemy->pos.y - pEnemy->Size.y / 2 <= g_aAttackModel[nCntAttack].pos.y + g_aAttackModel[nCntAttack].vtxMaxAttackModel.y &&
							pEnemy->pos.z + pEnemy->Size.z / 2 >= g_aAttackModel[nCntAttack].pos.z + g_aAttackModel[nCntAttack].vtxMinAttackModel.z &&
							pEnemy->pos.z - pEnemy->Size.z / 2 <= g_aAttackModel[nCntAttack].pos.z + g_aAttackModel[nCntAttack].vtxMaxAttackModel.z)
						{
							if (g_nDelayAttack % 4 == 0)
							{
								//pEnemy->nHp -= 15;//１０ダメージを与える
								SetDamageEnemy(15, nCntEnemy);
								
								SetDamage(/*Damage*/ 15,
									/*Life*/ 100,
									/*Pos*/ pEnemy->pos,
									/*半径*/ 20.0f,
									D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));
							}
						}
					}

				}
				break;
			case ATTACKMODELTYPE_METEOBALL://プレイヤーの技：メテオボール


				g_aAttackModel[nCntAttack].rot.y += 0.1f;
				SetEffect(EFFECTTYPE_NORMAL, 200, 300.0f, 2.0f, g_aAttackModel[nCntAttack].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 0.0f);


				if (g_nDelayAttack % 15 == 0)
				{
						float fRandumRot3 = float(rand() % 100 - 50) / 100;
						
						SetEmitterEffect(-1, 1000, 1000, 50.0f, 1.0f, g_aAttackModel[nCntAttack].pos, D3DXVECTOR3(sinf(pPlayer->rot.y + fRandumRot3) * 10.0f,fRandumMove, cosf(pPlayer->rot.y + fRandumRot3) * 10.0f), D3DXCOLOR(fRandumColor1, fRandumColor2, fRandumColor3, 1.0f),0.0f, nCntAttack);
				}

				for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				{
					if (pEnemy->bUse == true)
					{
						if (pEnemy->pos.x + pEnemy->Size.x / 2 >= g_aAttackModel[nCntAttack].pos.x + g_aAttackModel[nCntAttack].vtxMinAttackModel.x &&
							pEnemy->pos.x - pEnemy->Size.x / 2 <= g_aAttackModel[nCntAttack].pos.x + g_aAttackModel[nCntAttack].vtxMaxAttackModel.x &&
							pEnemy->pos.y + pEnemy->Size.y >= g_aAttackModel[nCntAttack].pos.y + g_aAttackModel[nCntAttack].vtxMinAttackModel.y &&
							pEnemy->pos.y <= g_aAttackModel[nCntAttack].pos.y + g_aAttackModel[nCntAttack].vtxMaxAttackModel.y &&
							pEnemy->pos.z + pEnemy->Size.z / 2 >= g_aAttackModel[nCntAttack].pos.z + g_aAttackModel[nCntAttack].vtxMinAttackModel.z &&
							pEnemy->pos.z - pEnemy->Size.z / 2 <= g_aAttackModel[nCntAttack].pos.z + g_aAttackModel[nCntAttack].vtxMaxAttackModel.z)
						{
							if (g_nDelayAttack % 20 == 0)
							{
								//pEnemy->nHp -= 50;//１０ダメージを与える
								SetDamageEnemy(50, nCntEnemy);
								SetDamage(/*Damage*/ 50,
									/*Life*/ 100,
									/*Pos*/ pEnemy->pos,
									/*半径*/ 50.0f,
									D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));
							}
						}
					}

				}
				break;
			case ATTACKMODELTYPE_KILLNEEDLE://プレイヤーの技キルニードル

				Bullet* pBullet = GetBullet();

				//g_aAttackModel[nCntAttack].rot.y += 0.1f;
				//SetEffect(EFFECTTYPE_NORMAL, 200, 300.0f, 2.0f, g_aAttackModel[nCntAttack].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 0.0f);


		/*		if (g_nDelayAttack % 15 == 0)
				{
					float fRandumRot3 = float(rand() % 100 - 50) / 100;

					SetEmitterEffect(-1, 1000, 1000, 50.0f, 1.0f, g_aAttackModel[nCntAttack].pos, D3DXVECTOR3(sinf(pPlayer->rot.y + fRandumRot3) * 10.0f, fRandumMove, cosf(pPlayer->rot.y + fRandumRot3) * 10.0f), D3DXCOLOR(fRandumColor1, fRandumColor2, fRandumColor3, 1.0f), 0.0f, nCntAttack);
				}*/

				
				
				for (int nCnt = 0; nCnt < 2; nCnt++)
				{
					SetParticle(PARTICLETYPE_NORMAL, 300, 5.0f, 0.5f, g_aAttackModel[nCntAttack].pos, D3DXVECTOR3(sinf(-D3DX_PI * fRandumRot) * fRandumSpeed, fRandumMove, cosf(-D3DX_PI * fRandumRot) * fRandumSpeed),
						D3DXCOLOR(0.5f, 1.0f, 0.5f, 1.0f), 0.0f, PARTICLETEX_NORMAL);
				}

				for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				{
					if (pEnemy->bUse == true)
					{
						if (pEnemy->pos.x + pEnemy->Size.x / 2 >= g_aAttackModel[nCntAttack].pos.x + g_aAttackModel[nCntAttack].vtxMinAttackModel.x &&
							pEnemy->pos.x - pEnemy->Size.x / 2 <= g_aAttackModel[nCntAttack].pos.x + g_aAttackModel[nCntAttack].vtxMaxAttackModel.x &&
							pEnemy->pos.y + pEnemy->Size.y>= g_aAttackModel[nCntAttack].pos.y + g_aAttackModel[nCntAttack].vtxMinAttackModel.y &&
							pEnemy->pos.y<= g_aAttackModel[nCntAttack].pos.y + g_aAttackModel[nCntAttack].vtxMaxAttackModel.y &&
							pEnemy->pos.z + pEnemy->Size.z / 2 >= g_aAttackModel[nCntAttack].pos.z + g_aAttackModel[nCntAttack].vtxMinAttackModel.z &&
							pEnemy->pos.z - pEnemy->Size.z / 2 <= g_aAttackModel[nCntAttack].pos.z + g_aAttackModel[nCntAttack].vtxMaxAttackModel.z)
						{
							if (g_nDelayAttack % 10 == 0)
							{
								//pEnemy->nHp -= 25;//１０ダメージを与える
								SetDamageEnemy(25, nCntEnemy);
								SetDamage(/*Damage*/25,
									/*Life*/ 100,
									/*Pos*/ pEnemy->pos,
									/*半径*/ 15.0f,
									D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
							}
						}
					}

				}
				float fRandumRot = float(rand() % 200 + 1) / 100;
				float fRandumSpeed = float(rand() % 50) / 10;
				float fRandumMove = float(rand() % 21 - 10);
				for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++, pBullet++)
				{
					if (pBullet->bUse == true)
					{
						int nData = 100;
						if (pBullet->pos.x + pBullet->fRWidth / 2 >= g_aAttackModel[nCntAttack].pos.x + g_aAttackModel[nCntAttack].vtxMinAttackModel.x &&
							pBullet->pos.x - pBullet->fRWidth / 2 <= g_aAttackModel[nCntAttack].pos.x + g_aAttackModel[nCntAttack].vtxMaxAttackModel.x &&
							pBullet->pos.y + pBullet->fRHeight / 2 >= g_aAttackModel[nCntAttack].pos.y + g_aAttackModel[nCntAttack].vtxMinAttackModel.y &&
							pBullet->pos.y - pBullet->fRHeight / 2 <= g_aAttackModel[nCntAttack].pos.y + g_aAttackModel[nCntAttack].vtxMaxAttackModel.y &&
							pBullet->pos.z + pBullet->fRWidth / 2 >= g_aAttackModel[nCntAttack].pos.z + g_aAttackModel[nCntAttack].vtxMinAttackModel.z &&
							pBullet->pos.z - pBullet->fRWidth / 2 <= g_aAttackModel[nCntAttack].pos.z + g_aAttackModel[nCntAttack].vtxMaxAttackModel.z)
						{
							if (pBullet->nType != BULLETTYPE_NORMAL && pBullet->nType != BULLETTYPE_FIRE)
							{
								pBullet->bUse = false;
								for (int nCnt = 0; nCnt < 5; nCnt++)
								{
									SetParticle(PARTICLETYPE_NORMAL, 300, 10.0f, 0.1f, pBullet->pos, D3DXVECTOR3(sinf(-D3DX_PI * fRandumRot) * fRandumSpeed, fRandumMove, cosf(-D3DX_PI * fRandumRot) * fRandumSpeed),
										D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f),0.0f, PARTICLETEX_NORMAL);
								}
								if (pBullet->bUse == false)
								{
									KillShadow(pBullet->nIdxShadow);
								}
							}
						}
					}
				}
				break;

			}

			/*	fLength = sqrtf((g_aAttackModel[nCnt].pos.x - pPlayer->pos.x) * (g_aAttackModel[nCnt].pos.x - pPlayer->pos.x) +
					(g_aAttackModel[nCnt].pos.z - pPlayer->pos.z) * (g_aAttackModel[nCnt].pos.z - pPlayer->pos.z) +
						(g_aAttackModel[nCnt].pos.y - pPlayer->pos.y) * (g_aAttackModel[nCnt].pos.y - pPlayer->pos.y)) / 3;

				if (fLength <= 20.0f)
				{
					pPlayer->pos = pPlayer->posOld;
				}*/



			g_aAttackModel[nCntAttack].pos += g_aAttackModel[nCntAttack].move;

			//影の位置を設定
			SetPositionShadow(g_aAttackModel[nCntAttack].nIdxShadow, g_aAttackModel[nCntAttack].pos, -1,-1,D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));

			if (g_aAttackModel[nCntAttack].nLife <= 0)
			{
				g_aAttackModel[nCntAttack].bUse = false;
			}

			if (g_aAttackModel[nCntAttack].bUse == false)
			{
				KillShadow(g_aAttackModel[nCntAttack].nIdxShadow);
			}
		}
	}
}

//================================
//タイトル画面の描画処理
//================================
void DrawAttackModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();;//デバイスへのポインタと取得

			D3DXMATRIX mtxRot, mtxTrans;//計算用マトリックス

			D3DMATERIAL9 matDef;//現在のマテリアル保存用

			D3DXMATERIAL* pMat;//マテリアルデータへのポインタ

			//現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);
	
	for (int nCntAttackModel = 0; nCntAttackModel < MAX_ATTACKMODEL; nCntAttackModel++)
	{
		if (g_aAttackModel[nCntAttackModel].bUse == true)
		{

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_mtxWorldAttackModel);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aAttackModel[nCntAttackModel].rot.y, g_aAttackModel[nCntAttackModel].rot.x, g_aAttackModel[nCntAttackModel].rot.z);

			D3DXMatrixMultiply(&g_mtxWorldAttackModel, &g_mtxWorldAttackModel, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aAttackModel[nCntAttackModel].pos.x, g_aAttackModel[nCntAttackModel].pos.y, g_aAttackModel[nCntAttackModel].pos.z);
			D3DXMatrixMultiply(&g_mtxWorldAttackModel, &g_mtxWorldAttackModel, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldAttackModel);

		

			for (int nCntATTACKMODELSET = 0; nCntATTACKMODELSET < NUM_ATTACKMODEL; nCntATTACKMODELSET++)
			{//読み込むモデルの種類数分for分を回し、同じタイプのマテリアルのポインタや、、
			//パーツの描画、マテリアルの設定をする。
				if (g_aAttackModel[nCntAttackModel].nType == nCntATTACKMODELSET)
				{
					//マテリアルへのポインタを取得（セットしたモデルと同じタイプ）
					pMat = (D3DXMATERIAL*)g_aAttackModelMat[nCntATTACKMODELSET].pBuffMatAttackModel->GetBufferPointer();

					int nNumTexture = 0;
					//セットされたモデルとタイプが同じもののモデルのマテリアルの数分読み込む。
					for (int nCntMat = 0; nCntMat < (int)g_aAttackModelMat[nCntATTACKMODELSET].dwNumMatAttackModel; nCntMat++)
					{
						//マテリアルの設定
						pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

						if (pMat[nCntMat].pTextureFilename != NULL)
						{
							//テクスチャの設定
							pDevice->SetTexture(0, g_aAttackModelMat[nCntATTACKMODELSET].apTextureAttackModel[nNumTexture]);
							nNumTexture++;
						}
						else
						{
							//テクスチャの設定
							pDevice->SetTexture(0, g_aAttackModelMat[nCntATTACKMODELSET].apTextureAttackModel[nNumTexture]);
							nNumTexture++;
						}
						//モデル（パーツ）の描画
						g_aAttackModelMat[nCntATTACKMODELSET].pMeshAttackModel->DrawSubset(nCntMat);
					}
					break;
				}
			}

			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		}
	}

			//保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
}

void SetAttackModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, int nType, int nLife)
{
	for (int nCntAttackModel = 0; nCntAttackModel < MAX_ATTACKMODEL; nCntAttackModel++)
	{
		if (g_aAttackModel[nCntAttackModel].bUse == false)
		{
			g_aAttackModel[nCntAttackModel].bUse = true;//使用しているかどうか
			g_aAttackModel[nCntAttackModel].pos = pos;//位置
			g_aAttackModel[nCntAttackModel].rot = rot;//向き
			g_aAttackModel[nCntAttackModel].nType = nType;//種類
			g_aAttackModel[nCntAttackModel].nLife = nLife;//体力
			g_aAttackModel[nCntAttackModel].nIdxShadow = SetShadow();//影のインデックスを設定
			g_aAttackModel[nCntAttackModel].move = move;
			g_aAttackModel[nCntAttackModel].bOneFlag = true;//一回だけ発動させる処理
			g_aAttackModel[nCntAttackModel].vtxMaxAttackModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aAttackModel[nCntAttackModel].vtxMinAttackModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			//=========================
			//サイズを取得
			//=========================
			for (int nCntMat = 0; nCntMat < NUM_ATTACKMODEL; nCntMat++)
			{//種類を割り当てるfor文
				if (g_aAttackModel[nCntAttackModel].nType == nCntMat)
				{//モデルの種類が一致

					//頂点数の取得
					g_aAttackModel[nCntAttackModel].nNumVtx = g_aAttackModelMat[nCntMat].pMeshAttackModel->GetNumVertices();

					//頂点フォーマットのサイズを取得
					g_aAttackModel[nCntAttackModel].sizeFVF = D3DXGetFVFVertexSize(g_aAttackModelMat[nCntMat].pMeshAttackModel->GetFVF());

					//頂点バッファのロック
					g_aAttackModelMat[nCntMat].pMeshAttackModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&g_aAttackModel[nCntAttackModel].pVtxBuff);


					for (int nCntVtx = 0; nCntVtx < g_aAttackModel[nCntAttackModel].nNumVtx; nCntVtx++)
					{
						//頂点座標の代入
						D3DXVECTOR3 vtx = *(D3DXVECTOR3*)g_aAttackModel[nCntAttackModel].pVtxBuff;

						//====================================================
						//頂点座標を比較してモデルの最小値最大値を取得
						//====================================================
						if (vtx.x > g_aAttackModel[nCntAttackModel].vtxMaxAttackModel.x)
						{
							g_aAttackModel[nCntAttackModel].vtxMaxAttackModel.x = vtx.x;
						}
						else if (vtx.x < g_aAttackModel[nCntAttackModel].vtxMinAttackModel.x)
						{//今回読み込んだ頂点が、一番小さい頂点より小さい場合
							g_aAttackModel[nCntAttackModel].vtxMinAttackModel.x = vtx.x;
						}
						else if (vtx.y > g_aAttackModel[nCntAttackModel].vtxMaxAttackModel.y)
						{//今回読み込んだ頂点が、一番小さい頂点より小さい場合
							g_aAttackModel[nCntAttackModel].vtxMaxAttackModel.y = vtx.y;
						}
						else if (vtx.y < g_aAttackModel[nCntAttackModel].vtxMinAttackModel.y)
						{//今回読み込んだ頂点が、一番小さい頂点より小さい場合
							g_aAttackModel[nCntAttackModel].vtxMinAttackModel.y = vtx.y;
						}
						else if (vtx.z > g_aAttackModel[nCntAttackModel].vtxMaxAttackModel.z)
						{//今回読み込んだ頂点が、一番小さい頂点より小さい場合
							g_aAttackModel[nCntAttackModel].vtxMaxAttackModel.z = vtx.z;
						}
						else if (vtx.z < g_aAttackModel[nCntAttackModel].vtxMinAttackModel.z)
						{//今回読み込んだ頂点が、一番小さい頂点より小さい場合
							g_aAttackModel[nCntAttackModel].vtxMinAttackModel.z = vtx.z;
						}
						//========================================================================

						//頂点フォーマットのサイズ文ポインタを進める
						g_aAttackModel[nCntAttackModel].pVtxBuff += g_aAttackModel[nCntAttackModel].sizeFVF;

					}

					//頂点バッファのアンロック
					g_aAttackModelMat[nCntMat].pMeshAttackModel->UnlockVertexBuffer();

					//種類特定したし、もうタイプのfor文回す必要ないので、break
					break;

				}
			}

			//==================================================================================================

			break;
		}
	}
}

int GetNumAttackModel(void)
{
	return g_nNumAttackModel;//読み込むモデルの最大数を取得する
}

AttackModel* GetAttackModel(void)//構造体の情報を取得する
{
	return &g_aAttackModel[0];
}
