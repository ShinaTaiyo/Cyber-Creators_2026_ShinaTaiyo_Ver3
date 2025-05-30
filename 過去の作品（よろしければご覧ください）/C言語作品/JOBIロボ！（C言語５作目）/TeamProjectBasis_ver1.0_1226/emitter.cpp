//========================================================
//
//９月２６日：ハッカソンの基盤作り[Emitter.cpp]
//Author:ShinaTaiyo
//
//=========================================================

#include "emitter.h"
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "shadow.h"
#include "effect.h"
#include "attackmodel.h"
#include "boss.h"
#define EMITTER_SCALE (10.0f)
//グローバル変数
D3DXVECTOR3 g_posEmitter;//位置
//D3DXVECTOR3 g_rotEmitter;//向き
D3DXMATRIX g_mtxWorldEmitter;//ワールドマトリックス(float4*4)

Emitter g_aEmitter[MAX_EMITTER];//弾の構造体情報



//タイトル画面の初期化処理
void InitEmitter(void)
{

	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	for (int nCntEmitter = 0; nCntEmitter < MAX_EMITTER; nCntEmitter++)
	{
		g_aEmitter[nCntEmitter].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//位置
		g_aEmitter[nCntEmitter].SupportPos = NULL_VECTOR3;//決めた位置で処理をしたいときに使う
		g_aEmitter[nCntEmitter].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//移動量
		g_aEmitter[nCntEmitter].AddMove = NULL_VECTOR3;//移動量を追加する
		g_aEmitter[nCntEmitter].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//向き
		g_aEmitter[nCntEmitter].nLife = 0;//体力
		g_aEmitter[nCntEmitter].fRWidth = 0.0f;//幅（半径）
		g_aEmitter[nCntEmitter].fRHeight = 0.0f;//高さ（半径）
		g_aEmitter[nCntEmitter].nType = 0;//種類
		g_aEmitter[nCntEmitter].bUse = false;//使用しているかどうか
		g_aEmitter[nCntEmitter].nIdxShadow = -1;//影のインデックス
		g_aEmitter[nCntEmitter].fScale = 0.0f;//大きさ
		g_aEmitter[nCntEmitter].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);//色
		g_aEmitter[nCntEmitter].fRedution = 0.0f;//エフェクトを小さくするスピード
		g_aEmitter[nCntEmitter].EmitterType = EMITTERTYPE_EFFECT;//エミッターで呼び出す処理の種類
		g_aEmitter[nCntEmitter].nSummonLife = 0;//呼び出す処理の体力
		g_aEmitter[nCntEmitter].SummonMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEmitter[nCntEmitter].fRotMove = 0.0f;//動く向きの数値を決める
		g_aEmitter[nCntEmitter].nNumAttack = -1;//エミッターを呼び出している攻撃の番号を格納する
		g_aEmitter[nCntEmitter].fLength = 0.0f;//距離
		g_aEmitter[nCntEmitter].nTargetType = 0;//どのモデルを中心にエミッターをだすか
		g_aEmitter[nCntEmitter].fVXaim3D = 0.0f;
		g_aEmitter[nCntEmitter].fVYaim3D = 0.0f;
		g_aEmitter[nCntEmitter].fVZaim3D = 0.0f;
		g_aEmitter[nCntEmitter].fVLaim3D = 0.0f;
		g_aEmitter[nCntEmitter].nCntTime = 0;//エミッターが出現してからの時間をカウントする
	}
}

//==============================
//タイトル画面の終了処理
//==============================
void UninitEmitter(void)
{

}
//============================
//タイトル画面の更新処理
//============================
void UpdateEmitter(void)
{

	for (int nCntEmitter = 0; nCntEmitter < MAX_EMITTER; nCntEmitter++)
	{

		if (g_aEmitter[nCntEmitter].bUse == true)
		{
			AttackModel* pAttackModel = GetAttackModel();
			Boss* pBoss = GetBoss();
			//体力を減らす
			g_aEmitter[nCntEmitter].nLife -= 1;
			g_aEmitter[nCntEmitter].nCntTime++;

			if (g_aEmitter[nCntEmitter].nType == EMITTERTYPE_CURVE)
			{
				g_aEmitter[nCntEmitter].fRotMove += 0.005f;

				g_aEmitter[nCntEmitter].move.x = sinf(D3DX_PI * g_aEmitter[nCntEmitter].fRotMove) * 5.0f;
				g_aEmitter[nCntEmitter].move.z = cosf(D3DX_PI * g_aEmitter[nCntEmitter].fRotMove) * 5.0f;
			}
			else if (g_aEmitter[nCntEmitter].nType == EMITTERTYPE_TORNADO)
			{
				if (g_aEmitter[nCntEmitter].nTargetType == EMITTERTARGET_ATTACKMODEL)
				{
					for (int nCntAttackModel = 0; nCntAttackModel < MAX_ATTACKMODEL; nCntAttackModel++, pAttackModel++)
					{
						if (pAttackModel->bUse == true && nCntAttackModel == g_aEmitter[nCntEmitter].nNumAttack && pAttackModel->nType == ATTACKMODEL05_TORNADO)
						{
							g_aEmitter[nCntEmitter].fRotMove += 0.1f;

							g_aEmitter[nCntEmitter].pos.x = sinf(D3DX_PI * g_aEmitter[nCntEmitter].fRotMove) * 100.0f + pAttackModel->pos.x;
							g_aEmitter[nCntEmitter].pos.z = cosf(D3DX_PI * g_aEmitter[nCntEmitter].fRotMove) * 100.0f + pAttackModel->pos.z;
						}
					}
				}
				else if (g_aEmitter[nCntEmitter].nTargetType == EMITTERTARGET_BOSS)
				{
					if (pBoss->bUse == true)
					{
						if (g_aEmitter[nCntEmitter].nNumAttack == 0)
						{
							g_aEmitter[nCntEmitter].fRotMove += 0.1f;
						}
						else
						{
							g_aEmitter[nCntEmitter].fRotMove += -0.1f;
						}

						g_aEmitter[nCntEmitter].fLength *= 1.03f;

						g_aEmitter[nCntEmitter].pos.x = sinf(D3DX_PI * g_aEmitter[nCntEmitter].fRotMove) * g_aEmitter[nCntEmitter].fLength + g_aEmitter[nCntEmitter].SupportPos.x;
						g_aEmitter[nCntEmitter].pos.z = cosf(D3DX_PI * g_aEmitter[nCntEmitter].fRotMove) * g_aEmitter[nCntEmitter].fLength + g_aEmitter[nCntEmitter].SupportPos.z;
					}
				}

			}
			else if (g_aEmitter[nCntEmitter].nType == EMITTERTYPE_ROTATION)
			{
				for (int nCntAttackModel = 0; nCntAttackModel < MAX_ATTACKMODEL; nCntAttackModel++, pAttackModel++)
				{
					if (pAttackModel->bUse == true && nCntAttackModel == g_aEmitter[nCntEmitter].nNumAttack)
					{
						g_aEmitter[nCntEmitter].fRotMove += 0.04f;
						g_aEmitter[nCntEmitter].pos.x = sinf(D3DX_PI * g_aEmitter[nCntEmitter].fRotMove) * g_aEmitter[nCntEmitter].fLength + g_aEmitter[nCntEmitter].SupportPos.x;
						g_aEmitter[nCntEmitter].pos.z = cosf(D3DX_PI * g_aEmitter[nCntEmitter].fRotMove) * g_aEmitter[nCntEmitter].fLength + g_aEmitter[nCntEmitter].SupportPos.z;
						break;
					}
					else if (pAttackModel->bUse == false && nCntAttackModel == g_aEmitter[nCntEmitter].nNumAttack)
					{
						g_aEmitter[nCntEmitter].bUse = false;
						break;
					}
				}
			}
			else if (g_aEmitter[nCntEmitter].nType == EMITTERTYPE_GATHERING)
			{
				g_aEmitter[nCntEmitter].move *= 1.1f;

				if (g_aEmitter[nCntEmitter].pos.x >= g_aEmitter[nCntEmitter].SupportPos.x - 61.0f &&
					g_aEmitter[nCntEmitter].pos.x <= g_aEmitter[nCntEmitter].SupportPos.x + 61.0f &&
					g_aEmitter[nCntEmitter].pos.y >= g_aEmitter[nCntEmitter].SupportPos.y - 61.0f &&
					g_aEmitter[nCntEmitter].pos.y <= g_aEmitter[nCntEmitter].SupportPos.y + 61.0f &&
					g_aEmitter[nCntEmitter].pos.z >= g_aEmitter[nCntEmitter].SupportPos.z - 61.0f &&
					g_aEmitter[nCntEmitter].pos.z <= g_aEmitter[nCntEmitter].SupportPos.z + 61.0f)
				{
					g_aEmitter[nCntEmitter].bUse = false;
				}
			}
			else if (g_aEmitter[nCntEmitter].nType == EMITTERTYPE_ACCELEHORMING)
			{
				if (g_aEmitter[nCntEmitter].nCntTime == 1)
				{
					g_aEmitter[nCntEmitter].AddMove = ONE_VECTOR3;
				}
				g_aEmitter[nCntEmitter].AddMove *= 1.15f;
				D3DXVECTOR3 BulletMove = NULL_VECTOR3;
				D3DXVECTOR3 SaveMove = NULL_VECTOR3;

				for (int nCntModel = 0; nCntModel < MAX_ATTACKMODEL; nCntModel++, pAttackModel++)
				{
					if (pAttackModel->bUse == true && nCntModel == g_aEmitter[nCntEmitter].nNumAttack)
					{
						g_aEmitter[nCntEmitter].fVXaim3D = pAttackModel->SupportPos.x - g_aEmitter[nCntEmitter].pos.x;
						g_aEmitter[nCntEmitter].fVYaim3D = pAttackModel->SupportPos.y - g_aEmitter[nCntEmitter].pos.y;
						g_aEmitter[nCntEmitter].fVZaim3D = pAttackModel->SupportPos.z - g_aEmitter[nCntEmitter].pos.z;
						g_aEmitter[nCntEmitter].fVLaim3D = sqrtf((g_aEmitter[nCntEmitter].fVXaim3D * g_aEmitter[nCntEmitter].fVXaim3D) +
							(g_aEmitter[nCntEmitter].fVYaim3D * g_aEmitter[nCntEmitter].fVYaim3D) +
							(g_aEmitter[nCntEmitter].fVZaim3D * g_aEmitter[nCntEmitter].fVZaim3D));

						BulletMove.x = g_aEmitter[nCntEmitter].fVXaim3D / g_aEmitter[nCntEmitter].fVLaim3D * g_aEmitter[nCntEmitter].AddMove.x;
						BulletMove.y = g_aEmitter[nCntEmitter].fVYaim3D / g_aEmitter[nCntEmitter].fVLaim3D * g_aEmitter[nCntEmitter].AddMove.y;
						BulletMove.z = g_aEmitter[nCntEmitter].fVZaim3D / g_aEmitter[nCntEmitter].fVLaim3D * g_aEmitter[nCntEmitter].AddMove.z;

						break;
					}
				}

				g_aEmitter[nCntEmitter].move += BulletMove;

			}

			SetEffect(EFFECTTYPE_NORMAL, g_aEmitter[nCntEmitter].nSummonLife, g_aEmitter[nCntEmitter].fScale, g_aEmitter[nCntEmitter].fScale, g_aEmitter[nCntEmitter].fRedution,
				g_aEmitter[nCntEmitter].pos, NULL_VECTOR3, g_aEmitter[nCntEmitter].col,0.0f,0);

			//移動処理
			g_aEmitter[nCntEmitter].pos += g_aEmitter[nCntEmitter].move;

			if (g_aEmitter[nCntEmitter].nLife <= 0)
			{
				g_aEmitter[nCntEmitter].bUse = false;
			}

		}
			

	}
		
}



//================================
//タイトル画面の描画処理
//================================
void DrawEmitter(void)
{

}

void SetEmitterEffect(int nType, int nTargetType, int nLife, int nSummonLife, float fScale, float fRedution, 
	D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRotMove, int nNumAttack, float fLength, D3DXVECTOR3 SupportPos)
{
	for (int nCntEmitter = 0; nCntEmitter < MAX_EMITTER; nCntEmitter++)
	{
		if (g_aEmitter[nCntEmitter].bUse == false)
		{
			g_aEmitter[nCntEmitter].bUse = true;//使用しているかどうか
			g_aEmitter[nCntEmitter].pos = pos;//位置
			g_aEmitter[nCntEmitter].move = move;//移動量
			g_aEmitter[nCntEmitter].AddMove = NULL_VECTOR3;//移動量を追加する
			g_aEmitter[nCntEmitter].col = col;//色
			g_aEmitter[nCntEmitter].nLife = nLife;//体力
			g_aEmitter[nCntEmitter].nType = nType;//種類
			g_aEmitter[nCntEmitter].nTargetType = nTargetType;//どのモデルを中心にエミッターをだすか
			g_aEmitter[nCntEmitter].fScale = fScale;//大きさ
			g_aEmitter[nCntEmitter].fRedution = fRedution;//エフェクトを小さくするスピード
			g_aEmitter[nCntEmitter].nSummonLife = nSummonLife;//呼び出す処理の体力
			g_aEmitter[nCntEmitter].fRotMove = fRotMove;//動く向きの数値を決める
			g_aEmitter[nCntEmitter].nNumAttack = nNumAttack;//エミッターを呼び出している攻撃の番号を格納する
			g_aEmitter[nCntEmitter].fLength = fLength;//距離
			g_aEmitter[nCntEmitter].SupportPos = SupportPos;//決めた位置で処理をする時に使う
			g_aEmitter[nCntEmitter].fVXaim3D = 0.0f;
			g_aEmitter[nCntEmitter].fVYaim3D = 0.0f;
			g_aEmitter[nCntEmitter].fVZaim3D = 0.0f;
			g_aEmitter[nCntEmitter].fVLaim3D = 0.0f;
			g_aEmitter[nCntEmitter].nCntTime = 0;//エミッターが出現してからの時間をカウントする
			break;
		}
	}
}
