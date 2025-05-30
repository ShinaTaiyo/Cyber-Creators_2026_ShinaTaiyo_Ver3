//==================================================================================================================
//
//２０２５年１月８日：モーションキャラクタークラスを作る[character.cpp]
//Author:ShinaTaiyo
//
//===================================================================================================================

//=============================================================
//インクルードファイル
//=============================================================
#include "character.h"
#include "manager.h"
#include "objectXInfo.h"
#include "debugtext.h"
#include "calculation.h"
//===================================================================================================================

//=============================================================
//静的メンバ宣言
//=============================================================
vector<CCharacter::CHARACTERMOTIONDATA> CCharacter::s_VecMotionInfo = {};//キャラクターごとのモーション情報
//===================================================================================================================

//=============================================================
//コンストラクタ
//=============================================================
CCharacter::CCharacter(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CObjectX(nPri, bUseintPri, type, ObjType), 
m_nIdxCharacter(0), m_NowMotionInfo({})
{

}
//===================================================================================================================

//=============================================================
//デストラクタ
//=============================================================
CCharacter::~CCharacter()
{

}
//===================================================================================================================

//=============================================================
//初期化処理
//=============================================================
HRESULT CCharacter::Init()
{
    CObjectX::Init();//オブジェクトXの初期化処理
	return S_OK;
}
//===================================================================================================================

//=============================================================
//終了処理
//=============================================================
void CCharacter::Uninit()
{
    CObjectX::Uninit();//オブジェクトXの終了処理
}
//===================================================================================================================

//=============================================================
//更新処理
//=============================================================
void CCharacter::Update()
{
    CObjectX::Update();//オブジェクトXの更新処理

    //デバッグ表示
    CManager::GetDebugText()->PrintDebugText("重力を使用するかどうか：%d\n", GetMoveInfo().GetUseGravity());

    for (auto it : m_VecModelParts)
    {
        it->ExtraUpdate();//それぞれのモデルパーツの更新処理を呼ぶ
    }
}
//===================================================================================================================

//=============================================================
//描画処理
//=============================================================
void CCharacter::Draw()
{
    CObjectX::Draw();//オブジェクトXの描画処理

    for (auto it : m_VecModelParts)
    {
        it->ExtraDraw();//それぞれのモデルパーツの描画処理を呼ぶ（ExtraDrawは、オブジェクトのDrawallからは呼ばれないようにするため)
    }
}
//===================================================================================================================

//=============================================================
//死亡フラグ設定処理
//=============================================================
void CCharacter::SetDeath()
{
    CObjectX::SetDeath();//オブジェクトXの死亡フラグ設定処理

    for (auto it : m_VecModelParts)
    {
        it->SetUseDeath(true);//死亡フラグを使用する
        it->SetDeath();       //死亡フラグを設定する
    }

    m_VecModelParts.clear();//全てのモデルパーツの死亡フラグの設定が終わったので、データをクリアする
}
//===================================================================================================================

//=============================================================
//次のモーションを設定する
//=============================================================
void CCharacter::SetNextMotion(int nNext)
{
    if (nNext >= s_VecMotionInfo[m_nIdxCharacter].nNumMotion)
    {//指定したモーションが、モーション最大数（配列）を超えていたら初期モーションを指定する
        nNext = 0;
    }
    m_NowMotionInfo.nNextMotion = nNext;//設定する
}
//===================================================================================================================

//=============================================================
//ダメージを与える
//=============================================================
void CCharacter::SetDamage(int nDamage, int nHitStopTime)
{
    for (auto ModelPartsIt : m_VecModelParts)
    {//範囲ベースfor文
        ModelPartsIt->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), nHitStopTime, false, false, true);//全てのモデルパーツの色合いを変える
    }

    CObjectX::SetDamage(nDamage, nHitStopTime);//オブジェクトXのダメージ処理
}
//===================================================================================================================

//=============================================================
//モーション情報を割り当てる
//=============================================================
int CCharacter::RegistMotion(string String, CCharacter* pCharacter)
{
	int nIdx = 0;//モーション番号
	for (auto it = s_VecMotionInfo.begin(); it != s_VecMotionInfo.end(); it++)
	{
		if (it->FileName == String)
		{//指定したファイルパスがモーション情報の動的配列に存在したら
            LoadModelParts(String, pCharacter);//既に存在するファイルなので、モデルパーツ情報だけ読み込む
            pCharacter->m_nIdxCharacter = nIdx;//キャラクター番号を設定
			return nIdx;
		}
		nIdx++;
	}

	SaveMotion(String);//まだモーションが存在していないので、新しいモーション情報を保存する
    LoadModelParts(String, pCharacter);//モデルパーツ情報を読み込む
    pCharacter->m_nIdxCharacter = nIdx;//キャラクター番号を設定
	return nIdx;
}
//===================================================================================================================

//=============================================================
//キャラクターのサイズを設定する
//=============================================================
void CCharacter::SetSize()
{
    CObjectX::SetSize();//サイズを設定 
}
//===================================================================================================================

//=============================================================
//モーション処理を行う
//=============================================================
void CCharacter::MotionProcess()
{
    m_NowMotionInfo.nMotionOld = m_NowMotionInfo.nNowMotion;    //1f前のモーションを設定する
    m_NowMotionInfo.nNowMotion = m_NowMotionInfo.nNextMotion;   //現在のモーションを更新する

    if (m_NowMotionInfo.nNowMotion != m_NowMotionInfo.nMotionOld)
    {//1f前のモーションと異なる場合、フレーム数とキーカウントをリセットし、ブレンド開始
        m_NowMotionInfo.nCntFrame = 0;
        m_NowMotionInfo.nCntKey = 0;
        m_NowMotionInfo.bNowBrending = true;
        CManager::GetDebugText()->PrintDebugText("フレーム数、キーカウントリセット！\n");
    }

    //参照する値を変数に格納しておく
    int nNowMotion = m_NowMotionInfo.nNowMotion;                                                      //現在のモーション番号を格納する
    int nNowKey = m_NowMotionInfo.nCntKey;                                                            //現在のキーを格納する
    int nNextKey = (nNowKey + 1) % s_VecMotionInfo[m_nIdxCharacter].VecMotion[nNowMotion].nNumKey;    //次のキー番号を格納する
    int nNowFrame = m_NowMotionInfo.nCntFrame;                                                        //フレーム数を格納
    int nMaxFrame = s_VecMotionInfo[m_nIdxCharacter].VecMotion[nNowMotion].VecKeySet[nNowKey].nFrame; //現在のキーセットの最大フレーム数を格納
    float fRatioFrame = static_cast<float>(nNowFrame) / static_cast<float>(nMaxFrame);                //現在のフレームの最大フレームに対しての割合を格納


    int nSize = m_VecModelParts.size();        //配列の大きさを格納
    int nBrendCheck = 0;                       //モーションブレンドが発動している場合、モーションブレンドが完了したモデルパーツの数をカウントする
    bool bOverArray = false;                   //配列外アクセスをしたかどうか
    for (int nCntParts = 0; nCntParts < nSize; nCntParts++)
    {
        D3DXVECTOR3 NowPos = { 0.0f,0.0f,0.0f };//現在のキーの位置
        D3DXVECTOR3 NextPos = { 0.0f,0.0f,0.0f };//次のキーの位置
        D3DXVECTOR3 NowRot = { 0.0f,0.0f,0.0f };//現在のキーの向き
        D3DXVECTOR3 NextRot = { 0.0f,0.0f,0.0f };//次のキーの向き

        if (m_nIdxCharacter >= 0 && static_cast<unsigned int>(m_nIdxCharacter) < s_VecMotionInfo.size())
        {
            //配列外チェック
            if (nNowMotion >= 0 && static_cast<unsigned int>(nNowMotion) < s_VecMotionInfo[m_nIdxCharacter].VecMotion.size())
            {
                if (nNowKey >= 0 && static_cast<unsigned int>(nNowKey) < s_VecMotionInfo[m_nIdxCharacter].VecMotion[nNowMotion].VecKeySet.size())
                {
                    if (nCntParts >= 0 && static_cast<unsigned int>(nCntParts) < s_VecMotionInfo[m_nIdxCharacter].VecMotion[nNowMotion].VecKeySet[nNowKey].VecKey.size())
                    {

                    }
                    else
                    {
                        assert("モデルパーツが配列外アクセス");
                    }
                }
                else
                {
                    assert("キーが配列外アクセス");
                }
            }
            else
            {
                assert("モーションが配列外アクセス");
            }
        }
        else
        {
            assert("キャラクターが配列外アクセス");
        }

        //モーションブレンドをするかしないかによって参照する位置や向きが変わる
        if (m_NowMotionInfo.bNowBrending == false)
        {//モーションブレンド状態じゃなければ
            NowPos = s_VecMotionInfo[m_nIdxCharacter].VecMotion[nNowMotion].VecKeySet[nNowKey].VecKey[nCntParts].Pos;   //現在のモーションのキーセットのキー情報の位置
            NextPos = s_VecMotionInfo[m_nIdxCharacter].VecMotion[nNowMotion].VecKeySet[nNextKey].VecKey[nCntParts].Pos; //次のモーションのキーセットのキー情報の位置

            NowRot = s_VecMotionInfo[m_nIdxCharacter].VecMotion[nNowMotion].VecKeySet[nNowKey].VecKey[nCntParts].Rot;   //現在のモーションのキーセットのキー情報の向き
            NextRot = s_VecMotionInfo[m_nIdxCharacter].VecMotion[nNowMotion].VecKeySet[nNextKey].VecKey[nCntParts].Rot; //次のモーションのキーセットのキー情報の向き
        }
        else
        {//モーションブレンド状態ならば

            //モデルパーツの現在の位置から徐々にブレンドするモーションのキーの位置に近づけていく
            NowPos = m_VecModelParts[nCntParts]->GetPosInfo().GetPos();
            NextPos = s_VecMotionInfo[m_nIdxCharacter].VecMotion[nNowMotion].VecKeySet[0].VecKey[nCntParts].Pos + m_VecModelParts[nCntParts]->GetPosInfo().GetSupportPos();

            //モデルパーツの現在の向きから徐々にブレンドするモーションのキーの向きに近づけていく
            NowRot = m_VecModelParts[nCntParts]->GetRotInfo().GetRot();
            NextRot = s_VecMotionInfo[m_nIdxCharacter].VecMotion[nNowMotion].VecKeySet[0].VecKey[nCntParts].Rot;
        }

        D3DXVECTOR3 DifferencePos = NextPos - NowPos;//次の位置と現在の位置の差分を求める
        D3DXVECTOR3 DifferenceRot = NextRot - NowRot;//次の向きと現在の向きの差分を求める

        if (m_NowMotionInfo.bNowBrending == true)
        {//モーションブレンドが成功したかどうかを判定
            if (DifferencePos.x < s_fMotionBrend_CheckDefference && 
                DifferencePos.y < s_fMotionBrend_CheckDefference && 
                DifferencePos.z < s_fMotionBrend_CheckDefference &&
                DifferenceRot.x < s_fMotionBrend_CheckDefference && 
                DifferenceRot.y < s_fMotionBrend_CheckDefference && 
                DifferenceRot.z < s_fMotionBrend_CheckDefference)
            {//全ての軸の向きの差分が指定した値以下になったらブレンドを完了する
                nBrendCheck++;
            }
        }

        //ジンバルロックを回避
        CCalculation::CorrectionRot(DifferenceRot.x);
        CCalculation::CorrectionRot(DifferenceRot.y);
        CCalculation::CorrectionRot(DifferenceRot.z);

        //上記で求めたデータを使用し、モーションした位置と向きを求める
        D3DXVECTOR3 DecisionPos = { 0.0f,0.0f,0.0f };
        D3DXVECTOR3 DecisionRot = { 0.0f,0.0f,0.0f };

        if (m_NowMotionInfo.bNowBrending == false)
        {//現在のキーの値と次のキーの値に向けて動かす
            DecisionPos = (DifferencePos * fRatioFrame) + m_VecModelParts[nCntParts]->GetPosInfo().GetSupportPos() + NowPos;//各パーツの初期位置を基準に位置を決定
            DecisionRot = (DifferenceRot * fRatioFrame) + NowRot;//各パーツの最初の向きがRot0.0fなので、位置みたいな補正はいらない
        }
        else
        {//ブレンド処理
            DecisionPos = (DifferencePos * s_fMotionBrend_Speed) + NowPos;
            DecisionRot = (DifferenceRot * s_fMotionBrend_Speed) + NowRot;
        }

        //位置と向きを設定
        m_VecModelParts[nCntParts]->GetPosInfo().SetPos(DecisionPos);
        m_VecModelParts[nCntParts]->GetRotInfo().SetRot(DecisionRot);
    }

    if (m_NowMotionInfo.bNowBrending == false)
    {//モーションブレンドが終わるまではフレーム数をカウントしない
        m_NowMotionInfo.nCntFrame++;//フレーム数を＋１
        if (m_NowMotionInfo.nCntFrame >= s_VecMotionInfo[m_nIdxCharacter].VecMotion[nNowMotion].VecKeySet[nNowKey].nFrame)
        {//フレーム数が現在のモーションの最大に達したら、キーを次に進め、フレーム数を０にする
            m_NowMotionInfo.nCntKey++;
            m_NowMotionInfo.nCntFrame = 0;

            if (m_NowMotionInfo.nCntKey >= s_VecMotionInfo[m_nIdxCharacter].VecMotion[nNowMotion].nNumKey)
            {//キーが現在のモーションの最大に達したらキーを０にする
                m_NowMotionInfo.nCntKey = 0;

                if (s_VecMotionInfo[m_nIdxCharacter].VecMotion[nNowMotion].bLoop == false)
                {//ループしないなら初期モーションに戻す
                    m_NowMotionInfo.nNextMotion = 0;
                }
            }
        }
    }
    else
    {//モーションブレンド中なら
        if (nBrendCheck == nSize)
        {//全てのパーツのモーションブレンドが完了した場合、モーションブレンドを終了する
            m_NowMotionInfo.bNowBrending = false;
        }
    }
}
//===================================================================================================================

//=============================================================
//モーション情報を保存する
//=============================================================
void CCharacter::SaveMotion(string MotionFileName)
{
    //=========================================
    //モーション情報の読み込みで使う変数
    //=========================================
    int nNumMotion = 0;      //モーションの数
    int nNumKeySet = 0;      //キーセットの数
    int nNumKey = 0;         //キーの数
    bool bLoop = false;      //ループするかどうかを数値で読み取る
    //====================================================================

    CHARACTERMOTIONDATA CharacterMotionData = {};
    CharacterMotionData.FileName = MotionFileName;//新しいモーション情報のファイルパスを保存する
    fstream ReadingFile;//読み取り用ファイル
    string Reading_Buff;//読み取り用ファイルの文字列

    //ファイルを開く
    ReadingFile.open(MotionFileName, ios::in);//読み取りモードで開く

    while (!ReadingFile.eof())//EOFに到達するまで繰り返す
    {
        ReadingFile >> Reading_Buff;
    
        if (Reading_Buff == "#")
        {//その行をスキップ
            getline(ReadingFile, Reading_Buff);
        }
        else if (Reading_Buff == "MOTIONSET")
        {//ファイル読み込みを終わらせる
            CharacterMotionData.VecMotion.push_back({});//モーション情報の設定をするので、初期値を追加（KEYSETはvectorなので自動的に初期化される)
            auto MotionIt = CharacterMotionData.VecMotion.begin();//設定するモーションの先頭イテレータを取得
            if (nNumMotion != 0)
            {
                advance(MotionIt,nNumMotion);//現在のモーションの総数　ー　１、イテレータを進める（最初は０なので、モーション総数が１の時は０を指したいから）
            }
            while (Reading_Buff != "END_MOTIONSET")
            {//モーション設定が終わるまで繰り返す
                ReadingFile >> Reading_Buff;//単語を読み取る

                if (Reading_Buff == "#")
                {//その行を読み込んでスキップ
                    getline(ReadingFile, Reading_Buff);
                }
                else if (Reading_Buff == "LOOP")
                {//ループするかどうか
                    ReadingFile >> Reading_Buff;//イコール
                    ReadingFile >> MotionIt->bLoop;//ループするかどうかを設定
                }
                else if (Reading_Buff == "NUM_KEY")
                {//キーの数
                    ReadingFile >> Reading_Buff;//イコール
                    ReadingFile >> MotionIt->nNumKey;//キー総数を設定
                }
                else if (Reading_Buff == "KEYSET")
                {//キー情報設定
                    MotionIt->VecKeySet.push_back({});//現在のモーションのキーセット情報を設定するので、キーセット情報の初期値を代入
                    auto KeySetIt = MotionIt->VecKeySet.begin();//キーセット情報の先頭をイテレータで指す
                    if (nNumKeySet != 0)
                    {
                        advance(KeySetIt, nNumKeySet);//現在のキーセット情報の総数分イテレータを進める
                    }
                    while (Reading_Buff != "END_KEYSET")
                    {//キー設定が終わるまで繰り返す
                        ReadingFile >> Reading_Buff;//単語を読み取る
                        if (Reading_Buff == "#")
                        {//その行を読み込んでスキップ
                            getline(ReadingFile, Reading_Buff);
                        }
                        else if (Reading_Buff == "FRAME")
                        {//フレーム数
                            ReadingFile >> Reading_Buff;//イコール
                            ReadingFile >> KeySetIt->nFrame;//フレーム数を設定
                        }
                        else if (Reading_Buff == "KEY")
                        {//キーデータ設定
                            KeySetIt->VecKey.push_back({});//現在のキーセット情報のキーデータを設定する
                            auto KeyIt = KeySetIt->VecKey.begin();//キーデータ情報の先頭をイテレータで指す
                            if (nNumKey != 0)
                            {
                                advance(KeyIt, nNumKey);//現在のキーデータ情報の総数イテレータを進める
                            }
                            while (Reading_Buff != "END_KEY")
                            {//キーデータの設定が終わるまで繰り返す
                                ReadingFile >> Reading_Buff;//単語を読み取る
                                if (Reading_Buff == "#")
                                {//その行を読み込んでスキップ
                                    getline(ReadingFile, Reading_Buff);
                                }
                                else if(Reading_Buff == "POS")
                                {//位置を読み込む
                                    ReadingFile >> Reading_Buff;//イコール
                                    ReadingFile >> KeyIt->Pos.x; //位置X
                                    ReadingFile >> KeyIt->Pos.y; //位置Y
                                    ReadingFile >> KeyIt->Pos.z; //位置Z
                                }
                                else if (Reading_Buff == "ROT")
                                {//向きを読み込む
                                    ReadingFile >> Reading_Buff; //イコール
                                    ReadingFile >> KeyIt->Rot.x; //向きX
                                    ReadingFile >> KeyIt->Rot.y; //向きY
                                    ReadingFile >> KeyIt->Rot.z; //向きZ
                                }
                                else if (Reading_Buff == "END_KEY")
                                {//キーデータ設定終了
                                    nNumKey++;
                                }
                            }
                        }
                        else if (Reading_Buff == "END_KEYSET")
                        {//キーセット設定終了
                            nNumKeySet++;
                            nNumKey = 0;
                        }
                    }
                }
                else if (Reading_Buff == "END_MOTIONSET")
                {//モーション設定終了
                    MotionIt->nNumKey = nNumKeySet;//現在設定していたモーションのキー総数を保存
                    nNumMotion++;
                    nNumKey = 0;
                    nNumKeySet = 0;
                }
            }
        }
    }

    CharacterMotionData.nNumMotion = nNumMotion;//モーション総数を格納する

    s_VecMotionInfo.push_back(CharacterMotionData);//上記の処理で新しいモーション情報を設定したのでキャラクターモーションのベクターに保存

    ReadingFile.close();//ファイルを閉じる
}
//===================================================================================================================

//=============================================================
//モーションファイルからモデルパーツを読み込む
//=============================================================
void CCharacter::LoadModelParts(string MotionFileName, CCharacter* pCharacter)
{
    fstream ReadingFile;//読み取り用ファイル
    string Reading_Buff;//読み取り用ファイルの文字列
    CObjectXInfo* pObjectXInfo = CManager::GetObjectXInfo();

    int nIdxParent = 0;//親マトリックスの番号
    int nPartsIdx = 0;//各パーツのインデックス
    int nNumSetParts = 0;//設定したパーツ数
    D3DXVECTOR3 Pos = { 0.0f,0.0f,0.0f };//位置
    D3DXVECTOR3 Rot = { 0.0f,0.0f,0.0f };//向き
    //ファイルを開く
    ReadingFile.open(MotionFileName, ios::in);//読み取りモードで開く
    while (!ReadingFile.eof())//EOFに到達するまで繰り返す
    {
        ReadingFile >> Reading_Buff;//単語を読み込む
        if (Reading_Buff == "#")
        {//その行をスキップ
            getline(ReadingFile, Reading_Buff);
        }
        else if (Reading_Buff == "MODEL_FILENAME")
        {//モデルパーツを読み込む
            ReadingFile >> Reading_Buff;//イコール
            ReadingFile >> Reading_Buff;//モデルパーツのパス
            pCharacter->m_VecModelParts.push_back(CModelParts::Create(Reading_Buff,pCharacter->GetType()));//モデルパーツ数を追加
        }
        else if (Reading_Buff == "CHARACTERSET")
        {//各モデルパーツの情報を設定する
            while (Reading_Buff != "END_CHARACTERSET")
            {//各モデルパーツの設定が終わるまで繰り返す
                ReadingFile >> Reading_Buff;//単語を読み込む
                if (Reading_Buff == "#")
                {//その行をスキップ
                    getline(ReadingFile, Reading_Buff);
                }
                else if (Reading_Buff == "PARTSSET")
                {//１つのパーツの設定をする
                    auto PartsIt = pCharacter->m_VecModelParts.begin();//パーツのベクターの先頭を指す
                    if (nNumSetParts != 0)
                    {
                        advance(PartsIt, nNumSetParts);//最初のイテレータが指すのは０番なので、設定したパーツ数ー１分、イテレータを進める
                    }
                    while (Reading_Buff != "END_PARTSSET")
                    {//モデルパーツの設定が終わるまで繰り返す
                        ReadingFile >> Reading_Buff;//単語を読み込む
                        if (Reading_Buff == "#")
                        {//その行をスキップ
                            getline(ReadingFile, Reading_Buff);
                        }
                        else if (Reading_Buff == "INDEX")
                        {//インデックス
                            ReadingFile >> Reading_Buff;//イコール
                            ReadingFile >> nPartsIdx;   //モデルパーツのインデックス
                            (*PartsIt)->SetPartsIdx(nPartsIdx);//モデルパーツごとのインデックスを設定
                        }
                        else if (Reading_Buff == "PARENT")
                        {//親モデルパーツ番号
                            ReadingFile >> Reading_Buff;//イコール
                            ReadingFile >> nIdxParent;   //モデルパーツのインデックス
                            //親モデルパーツを設定
                            if (nIdxParent >= 0)
                            {//モデルパーツに親が存在する
                                if (static_cast<unsigned int>(nIdxParent) < pCharacter->m_VecModelParts.size())
                                {
                                    (*PartsIt)->GetDrawInfo().SetUseMatrixChild(true, &pCharacter->m_VecModelParts[nIdxParent]->GetDrawInfo().GetMatrixWorld());//モデルパーツごとのインデックスを設定
                                }
                                else
                                {
                                    assert("モデルパーツのインデックスが配列外アクセス");
                                }
                            }
                            else
                            {//モデルパーツに親が存在しないので、キャラクターを親にする
                                (*PartsIt)->GetDrawInfo().SetUseMatrixChild(true, &pCharacter->GetDrawInfo().GetMatrixWorld());//モデルパーツごとのインデックスを設定
                            }
                        }
                        else if (Reading_Buff == "POS")
                        {//位置
                            ReadingFile >> Reading_Buff;//イコール
                            ReadingFile >> Pos.x;       //位置X
                            ReadingFile >> Pos.y;       //位置Y
                            ReadingFile >> Pos.z;       //位置Z

                            (*PartsIt)->GetPosInfo().SetSupportPos(Pos); //初期位置を設定
                            (*PartsIt)->GetPosInfo().SetPos(Pos);        //位置を設定
                        }
                        else if (Reading_Buff == "ROT")
                        {//向き
                            ReadingFile >> Reading_Buff; //イコール
                            ReadingFile >> Rot.x;        //向きX
                            ReadingFile >> Rot.y;        //向きY
                            ReadingFile >> Rot.z;        //向きZ

                            (*PartsIt)->GetPosInfo().SetSupportPos(Pos); //初期向きを設定
                            (*PartsIt)->GetPosInfo().SetPos(Pos);        //向きを設定
                        }
                        else if (Reading_Buff == "END_PARTSSET")
                        {//パーツ設定を終了する
                            nNumSetParts++;//設定が完了したパーツ数＋１
                        }
                    }
                }
            }
        }
    }

    ReadingFile.close();//ファイルを閉じる
}
//===================================================================================================================