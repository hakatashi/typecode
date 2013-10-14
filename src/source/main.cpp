//GLOBAL_INSTANCEを定義した場合、メイン関数用のexturn処理を行う
#define GLOBAL_INSTANCE
//他ファイルのインクルード
#include "../header/GV.h"

//ループで必ず行う３大処理
int ProcessLoop(){
	if (ProcessMessage()!=0) return -1; //プロセス処理がエラーなら-1を返す
	if (ClearDrawScreen()!=0) return -1; //画面クリア処理がエラーなら-1を返す
	GetHitKeyStateAll_2(); //現在のキー入力処理を行う
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow){ //WindowsAPIの定型文
	ChangeWindowMode(TRUE); //ウィンドウモードで起動
	if(DxLib_Init() == -1 || SetDrawScreen( DX_SCREEN_BACK )!=0) return -1; //初期化と裏画面化

	func_state=0; //func_stateの初期化
	SetDrawMode(DX_DRAWMODE_ANISOTROPIC); //異方性フィルタリング法で描画する
	launch_ini(); //起動時初期化関数

	while(ProcessLoop()==0){ //メインループ

		//func_stateによって状況を振り分け
		switch(func_state) {
		case -1: //エラー
			break;
		case 0: //起動処理
			//読み込み処理
			if (load()==-1) { 
				//読み込み失敗
				func_state=-1; //エラーに送る
			} else {
				//読み込み成功
				gamestart_ini(); //ゲーム開始時初期化処理
				func_state=1; //ゲームメインに送る
			}
			break;
		case 1: //ゲームメイン
			type_main(); //打鍵処理
			graph_main(); //描画
			break;
		case 2: //Clear表示
			graph_main(); //描画
			break;
		}
		
		control_main(); //各種関数制御

		if ( CheckStateKey(KEY_INPUT_ESCAPE) == 1 ) break; //エスケープが入力されたらブレイク
		ScreenFlip(); //裏画面反映
	}

	DxLib_End(); //ＤＸライブラリ終了処理
	return 0; //プログラム終了
}