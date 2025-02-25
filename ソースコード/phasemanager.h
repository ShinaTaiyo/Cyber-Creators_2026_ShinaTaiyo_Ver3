//==========================================================================================
//
//１１月２９日：フェーズマネージャーを作る[phasemanager.h]
//Author:ShinaTaiyo
//
//==========================================================================================

#ifndef _PHASEMANAGER_H_  
#define _PHASEMANAGER_H_

//============================
//インクルード
//============================
#include "main.h"
#include "object.h"
#include "enemy.h"

//========================================================

//======================
//前方宣言
//======================

//============================
//フェーズ管理クラス
//============================
class CPhaseManager : public CObject
{
public:
	CPhaseManager(int nNumStage,int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::PHASEMANAGER, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_NONE);  //コンストラクタ
	~CPhaseManager();                          //デストラクタ
	HRESULT Init() override;                   //初期化処理
	void Uninit() override;                    //終了処理
	void Update() override;                    //更新処理
	void Draw() override;                      //描画処理
	void SetDeath() override;                  //死亡フラグを設定
	static CPhaseManager * Create();           //生成処理
	void PushPhaseInfo(D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, D3DXVECTOR3 Scale, int nLife, int nEnemyType, int nTypeNum, int nPhaseNum,float fNormalSpeed,float fSensingRange,int nNumDivision, vector<CEnemy::MoveAiInfo> VecMoveAi);//フェーズ情報を設定
private:
	struct PhaseSaveInfo
	{
		D3DXVECTOR3 Pos;                      //位置
		D3DXVECTOR3 Rot;                      //向き
		D3DXVECTOR3 Scale;                    //拡大率
		int nLife;                            //体力
		int nEnemyType;                       //敵タイプ
		int nTypeNum;                         //クラスごとのタイプ
		int nPhaseNum;                        //フェーズ番号を設定
		float fNormalSpeed;                   //普通の速度
		float fSensingRange;                  //索敵距離
		vector<CEnemy::MoveAiInfo> VecMoveAi; //移動AIのVector
		int nNumDivision;                     //分裂回数
	};
	static list<PhaseSaveInfo>s_PhaseList;    //フェーズ情報のリスト
	static int s_MaxPhase;                    //フェーズの最大数
	static int s_nNowPhase;                   //現在のフェーズ番号
	static bool s_bStartFade;                 //フェードを開始する
	static int s_nNowStage;                   //現在のステージ番号
	void AdvancePhase();                      //次のフェーズに移行する処理
};
#endif