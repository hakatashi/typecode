#include "../header/GV.h"
#include <math.h>

int MyIsChar(char source) {
	if (('0'<=source && source<='9')
		|| ('A'<=source && source<='Z')
		|| ('a'<=source && source<='z')
		|| source=='_'
		|| source=='#') return TRUE;
	else return FALSE;
}

void graph_string( float x, float y, const char *String, int Kind, float Size, int BoldFlag) {
	int i;
	int Length;
	int SizeX,SizeY;
	int MultiFlag;
	char TempChar[3];

	Length=strlen(String);
	GetGraphSize(img_char[Kind][BoldFlag][0],&SizeX,&SizeY);
	SetFontSize((int)Size);

	for (i=0;i<Length;i++) {
		if (i+1==Length) MultiFlag=FALSE;
		else {
			TempChar[0]=String[i];
			TempChar[1]=String[i+1];
			TempChar[2]='\0';
			MultiFlag=MultiByteCharCheck(TempChar,DX_CHARSET_SHFTJIS);
		}
		if (MultiFlag) {
			DrawString(
				x+(float)SizeX/(float)SizeY*Size*i+1,
				y,
				TempChar,
				GetColor(0,128,0));
			i++;
		} else {
			DrawExtendGraphF(
				x+(float)SizeX/(float)SizeY*Size*i,
				y,
				x+(float)SizeX/(float)SizeY*Size*(i+1),
				y+Size,
				img_char[Kind][BoldFlag][String[i]-' '],
				TRUE);
		}
	}
}

void graph_sheet(){
	DrawGraph(0,0,img_bg,FALSE);
}

void graph_typescreen() {
	int i,c=0,state,SourcePoint,PoolPoint,Kindstate;
	/***************************
	PoolKind:
	 0 一致（灰色）
	 1 不一致（赤色）
	 2 未入力（黒色）
	***************************/
	char Pool[CODE_CHAR_MAX*2];
	int PoolKind[CODE_CHAR_MAX*2];
	int SizeX,SizeY;
	int Kind,BoldFlag;
	int temp;
	int ng_flag;
	double Length,aim;
	
	/***************************
	state:
	 0 記号のあと
	 1 単語の途中
	 2 単語のあと
	***************************/
	state=0;
	/***************************
	Kindstate:
	 0 一致（灰色）
	 1 不一致（赤色）
	***************************/
	Kindstate=0;
	SourcePoint=0;
	PoolPoint=0;
	ng_flag=FALSE;

	while(TypeInput[c]!='\0') {
		//一致状態の場合
		if (Kindstate==0) {
			switch(state) {
			case 0: //記号のあと
				//スペースの場合
				if (TypeInput[c]==' ') {
					Pool[PoolPoint]=' ';
					PoolKind[PoolPoint]=0;
					if (ScreenCursor==c) TypeCursor=PoolPoint;
					PoolPoint++;
					//sourceのほうにもスペースがある場合
					if (sourceType[TypingLine][SourcePoint]==' ') {
						//sourceを一個空繰りする
						SourcePoint++;
					}
				} else { //スペースでない場合
					//sourceがスペースでなくなるまで回す
					while (sourceType[TypingLine][SourcePoint]==' ') {
						Pool[PoolPoint]=' ';
						PoolKind[PoolPoint]=0;
						PoolPoint++;
						SourcePoint++;
					}
					//sourceとinputが一致している場合
					if (TypeInput[c]==sourceType[TypingLine][SourcePoint]) {
						//Poolに記録する
						Pool[PoolPoint]=TypeInput[c];
						PoolKind[PoolPoint]=0;
						if (ScreenCursor==c) TypeCursor=PoolPoint;
						PoolPoint++;
						//以下、sourceの読み進め処理
						//現在記録したcharが文字で、
						if (MyIsChar(sourceType[TypingLine][SourcePoint])) {
							//その次も文字ならば「単語の途中」で確定。
							if (MyIsChar(sourceType[TypingLine][SourcePoint+1])) {
								state=1;
								//sourceを一文字読み進める
								SourcePoint++;
							} else { //その次が文字でないならば「単語のあと」で確定。
								state=2;
								SourcePoint++;
							}
						} else { //現在記録したcharが文字でない（かつスペースではない→記号）場合は「記号のあと」で確定。
							state=0;
							SourcePoint++;
						}
					} else { //sourceとinputが一致していない場合
						//Kindstateを不一致に設定
						Kindstate=1;
					}
				}
				break;
			case 1: //単語の途中
				//sourceとinputが一致してる場合
				if (TypeInput[c]==sourceType[TypingLine][SourcePoint]) {
					//Poolに記録する
					Pool[PoolPoint]=TypeInput[c];
					PoolKind[PoolPoint]=0;
					if (ScreenCursor==c) TypeCursor=PoolPoint;
					PoolPoint++;
					//以下、sourceの読み進め処理
					//現在記録したcharは文字なので、
					//その次も文字ならば「単語の途中」で確定。
					if (MyIsChar(sourceType[TypingLine][SourcePoint+1])) {
						state=1;
						//sourceを一文字読み進める
						SourcePoint++;
					} else { //その次が文字でないならば「単語のあと」で確定。
						state=2;
						SourcePoint++;
					}
				} else { //一致していない場合、問答無用で不一致処理
					Kindstate=1;
				}
				break;
			case 2: //単語のあと
				//スペースでない場所を探す
				temp=0;
				while(sourceType[TypingLine][SourcePoint+temp]==' ') temp++;
				//次のcharが文字で、かつPoolにスペースが挿入されてない場合
				if (MyIsChar(sourceType[TypingLine][SourcePoint+temp]) && Pool[PoolPoint-1]!=' ') {
					//スペースならPoolに記録する
					if (TypeInput[c]==' ') {
						Pool[PoolPoint]=' ';
						PoolKind[PoolPoint]=0;
						if (ScreenCursor==c) TypeCursor=PoolPoint;
						PoolPoint++;
						SourcePoint++;
					} else { //スペースでない場合、問答無用で不一致処理
						Kindstate=1;
					}
				} else { //次のcharが記号、もしくはスペースが挿入されている場合
					//inputがスペースの場合
					if (TypeInput[c]==' ') {
						Pool[PoolPoint]=' ';
						PoolKind[PoolPoint]=0;
						if (ScreenCursor==c) TypeCursor=PoolPoint;
						PoolPoint++;
						//sourceのほうにもスペースがある場合
						if (sourceType[TypingLine][SourcePoint]==' ') {
							//sourceを一個空繰りする
							SourcePoint++;
						}
					} else { //スペースでない場合
						//sourceがスペースでなくなるまで回す
						while (sourceType[TypingLine][SourcePoint]==' ') {
							Pool[PoolPoint]=' ';
							PoolKind[PoolPoint]=0;
							PoolPoint++;
							SourcePoint++;
						}
						//sourceとinputが一致している場合
						if (TypeInput[c]==sourceType[TypingLine][SourcePoint]) {
							//Poolに記録する
							Pool[PoolPoint]=TypeInput[c];
							PoolKind[PoolPoint]=0;
							if (ScreenCursor==c) TypeCursor=PoolPoint;
							PoolPoint++;
							//以下、sourceの読み進め処理
							//現在記録したcharが文字で、
							if (MyIsChar(sourceType[TypingLine][SourcePoint])) {
								//その次も文字ならば「単語の途中」で確定。
								if (MyIsChar(sourceType[TypingLine][SourcePoint+1])) {
									state=1;
									//sourceを一文字読み進める
									SourcePoint++;
								} else { //その次が文字でないならば「単語のあと」で確定。
									state=2;
									SourcePoint++;
								}
							} else { //現在記録したcharが文字でない（かつスペースではない→記号）場合は「記号のあと」で確定。
								state=0;
								SourcePoint++;
							}
						} else { //sourceとinputが一致していない場合
							//Kindstateを不一致に設定
							Kindstate=1;
						}
					}
				}
				break;
			}
		}
		if (Kindstate==1) { //不一致状態の場合（上の処理で不一致状態となった場合を含む）
			//不一致状態としてPoolに記録
			Pool[PoolPoint]=TypeInput[c];
			PoolKind[PoolPoint]=1;
			ng_flag=TRUE;
			if (ScreenCursor==c) TypeCursor=PoolPoint;
			PoolPoint++;
		}
		c++;
	}

	while(sourceType[TypingLine][SourcePoint]!='\0') {
		Pool[PoolPoint]=sourceType[TypingLine][SourcePoint];
		PoolKind[PoolPoint]=2;
		ng_flag=TRUE;
		if (ScreenCursor==c) TypeCursor=PoolPoint;
		PoolPoint++;
		SourcePoint++;
		c++;
	}

	if (ng_flag==TRUE) OKflag=FALSE;
	else OKflag=TRUE;

	if (ScreenCursor==c) TypeCursor=PoolPoint;

	GetGraphSize(img_char[0][0][0],&SizeX,&SizeY);

	Length=(double)SizeX/(double)SizeY*18.0*PoolPoint;

	if (Length<=628.0) aim=0.0;
	else {
		if (TypeCursor<20) aim=0.0;
		else if (TypeCursor>PoolPoint-20) aim=Length-628.0;
		else aim=(Length-628.0)/(double)(PoolPoint-39)*(double)(TypeCursor-19);
	}
	InputSeekX=(InputSeekX-aim)/1.3+aim;

	for (i=0;i<PoolPoint;i++) {
		switch (PoolKind[i]) {
		case 0:
			Kind=3;
			BoldFlag=0;
			break;
		case 1:
			Kind=4;
			BoldFlag=1;
			break;
		case 2:
			Kind=0;
			BoldFlag=0;
			break;
		}
		DrawExtendGraphF(
			6.0f+(float)SizeX/(float)SizeY*18.0f*i-(float)InputSeekX,
			341.5f+(float)InputSeekY,
			6.0f+(float)SizeX/(float)SizeY*18.0f*(i+1)-(float)InputSeekX,
			341.5f+18.0f+(float)InputSeekY,
			img_char[Kind][BoldFlag][Pool[i]-' '],
			TRUE);
	}

	DrawLine(
		6.0f+18.0f/(float)SizeY*(float)SizeX*(float)TypeCursor-InputSeekX,
		340+InputSeekY,
		6.0f+18.0f/(float)SizeY*(float)SizeX*(float)TypeCursor-InputSeekX,
		340+20+InputSeekY,
		GetColor(1,0,0));

	for (i=1;i<7;i++) {
		graph_string(
			6.0f/*+sin((float)StageTime/200.0f)*10.0f*/,
			340.0f+20.0f*i+1.5f+(float)InputSeekY,
			sourceType[TypingLine+i],
			0,
			18.0f,
			FALSE);
	}
}

void graph_inputscreen() {
	char Pool[CODE_CHAR_MAX];
	char TempLine[CODE_CHAR_MAX];
	int PoolNum;
	int i,j;
	int LineCnt;
	int BlueFlag,ColorFlag;
	int SizeX,SizeY;
	/***************
	ColorFlag:
	 0 通常（黒または青）
	 1 茶色
	 2 緑色（行中タイプ）
	 3 緑色（行後タイプ「//～～」）
	***************/

	GetGraphSize(img_char[0][FALSE][0],&SizeX,&SizeY);

	PoolNum=0;
	ColorFlag=0;
	for ( LineCnt=(int)(InputScreenSeekY/12.0) ; LineCnt<=sourceTypeParallel[TypingLine] ; LineCnt++ ) {

		//TempLineに一時的に一行格納
		//入力中の行でない場合
		if (LineCnt<sourceTypeParallel[TypingLine]) {
			//TempLineにTypeStackの内容を格納
			for (i=0;TypeStack[LineCnt][i]!='\0';i++) TempLine[i]=TypeStack[LineCnt][i];
			//EOSを二個格納
			TempLine[i]='\0';
			TempLine[i+1]='\0';
		}
		//入力中の行の場合
		else {
			//TempLineにTypeStackの内容を格納
			for (i=0;TypeStack[LineCnt][i]!='\0';i++) TempLine[i]=TypeStack[LineCnt][i];
			//TempLineにTypeInputの内容を格納
			for (j=0;TypeInput[j]!='\0';j++) TempLine[i+j]=TypeInput[j];
			//EOSを二個格納
			TempLine[i+j]='\0';
			TempLine[i+j+1]='\0';
		}

		if (TempLine[0]!='\0') {

			i=0;
			do {
				//通常フラグが立っている場合
				if (ColorFlag==0) {
					//入力文字が行中コメント開始の場合
					if (TempLine[i]=='/' && TempLine[i+1]=='*') {
						//Poolに文字が入っている場合は、
						if (PoolNum>0) {
							//EOSを付加
							Pool[PoolNum]='\0';
							//強調語リストに一致する場合は青色フラグを立てる
							BlueFlag=0;
							for (j=0;j<KeywordNum[0];j++) {
								if (strcmp(Keyword[0][j],Pool)==0) {
									BlueFlag=1;
									break;
								}
							}
							//文字描画
							graph_string(
								6.0f+12.0f/(float)SizeY*(float)SizeX*(float)(i-PoolNum)-(float)InputScreenSeekX,
								32.0f+12.0f*(float)LineCnt-(float)InputScreenSeekY,
								Pool, BlueFlag?1:0, 12.0f, BlueFlag?TRUE:FALSE);
						}
						//行中緑色フラグを立てる
						ColorFlag=2;
						//カウンタを戻す
						Pool[0]='/';
						Pool[1]='*';
						PoolNum=2;
						//読み取り位置を一つ回す
						i++;
					}
					//入力文字が行後コメント開始の場合
					else if (TempLine[i]=='/' && TempLine[i+1]=='/') {
						//Poolに文字が入っている場合は、
						if (PoolNum>0) {
							//EOSを付加
							Pool[PoolNum]='\0';
							//強調語リストに一致する場合は青色フラグを立てる
							BlueFlag=0;
							for (j=0;j<KeywordNum[0];j++) {
								if (strcmp(Keyword[0][j],Pool)==0) {
									BlueFlag=1;
									break;
								}
							}
							//文字描画
							graph_string(
								6.0f+12.0f/(float)SizeY*(float)SizeX*(float)(i-PoolNum)-(float)InputScreenSeekX,
								32.0f+12.0f*(float)LineCnt-(float)InputScreenSeekY,
								Pool, BlueFlag?1:0, 12.0f, BlueFlag?TRUE:FALSE);
						}
						//行中緑色フラグを立てる
						ColorFlag=3;
						//カウンタを戻す
						Pool[0]='/';
						Pool[1]='/';
						PoolNum=2;
						//読み取り位置を一つ回す
						i++;
					}
					//入力文字が引用符の場合
					else if (TempLine[i]=='"' || TempLine[i]==39/*「'」=39*/) {
						//Poolに文字が入っている場合は、
						if (PoolNum>0) {
							//EOSを付加
							Pool[PoolNum]='\0';
							//強調語リストに一致する場合は青色フラグを立てる
							BlueFlag=0;
							for (j=0;j<KeywordNum[0];j++) {
								if (strcmp(Keyword[0][j],Pool)==0) {
									BlueFlag=1;
									break;
								}
							}
							//文字描画
							graph_string(
								6.0f+12.0f/(float)SizeY*(float)SizeX*(float)(i-PoolNum)-(float)InputScreenSeekX,
								32.0f+12.0f*(float)LineCnt-(float)InputScreenSeekY,
								Pool, BlueFlag?1:0, 12.0f, BlueFlag?TRUE:FALSE);
						}
						//茶色フラグを立てる
						ColorFlag=1;
						//カウンタを戻してPoolに（引用符から）記録開始
						Pool[0]=TempLine[i];
						PoolNum=1;
					}
					//入力文字が文字列の場合
					else if (('0'<=TempLine[i] && TempLine[i]<='9')
						|| ('A'<=TempLine[i] && TempLine[i]<='Z')
						|| ('a'<=TempLine[i] && TempLine[i]<='z')
						|| TempLine[i]=='_'
						|| TempLine[i]=='#') {
							//Poolに一文字追加
							Pool[PoolNum]=TempLine[i];
							PoolNum++;
					}
					//入力文字が（その他→）記号もしくはEOSの場合
					else {
						//Poolに文字が入っている場合は、
						if (PoolNum>0) {
							//EOSを付加
							Pool[PoolNum]='\0';
							//強調語リストに一致する場合は青色フラグを立てる
							BlueFlag=0;
							for (j=0;j<KeywordNum[0];j++) {
								if (strcmp(Keyword[0][j],Pool)==0) {
									BlueFlag=1;
									break;
								}
							}
							//文字描画
							graph_string(
								6.0f+12.0f/(float)SizeY*(float)SizeX*(float)(i-PoolNum)-(float)InputScreenSeekX,
								32.0f+12.0f*(float)LineCnt-(float)InputScreenSeekY,
								Pool, BlueFlag?1:0, 12.0f, BlueFlag?TRUE:FALSE);
						}
						//Poolに記号を一文字だけ代入する
						Pool[0]=TempLine[i];
						Pool[1]='\0';
						//描画
						graph_string(
							6.0f+12.0f/(float)SizeY*(float)SizeX*(float)i-(float)InputScreenSeekX,
							32.0f+12.0f*(float)LineCnt-(float)InputScreenSeekY,
							Pool, 0, 12.0f, FALSE);
						//カウンタを戻す
						PoolNum=0;
					}
				}
				//茶色フラグが立っている場合
				else if (ColorFlag==1) {
					//とりあえずPoolに一文字追加
					Pool[PoolNum]=TempLine[i];
					PoolNum++;
					//引用符またはEOSが来ていた場合
					if (TempLine[i]=='"' || TempLine[i]==39/*「'」*/ || TempLine[i]=='\0') {
						//さらにEOSを追加
						Pool[PoolNum]='\0';
						//描画
						graph_string(
							6.0f+12.0f/(float)SizeY*(float)SizeX*(float)(i-PoolNum+1)-(float)InputScreenSeekX,
							32.0f+12.0f*(float)LineCnt-(float)InputScreenSeekY,
							Pool, 2, 12.0f, FALSE);
						//茶色フラグを折る
						ColorFlag=0;
						//カウンタを戻す
						PoolNum=0;
					}
				}
				//行中緑色フラグが立っている場合
				else if (ColorFlag==2) {
					//行中終了記号の場合
					if ( TempLine[i]=='*' && TempLine[i+1]=='/' ) {
						//Poolに追加
						Pool[PoolNum]='*';
						Pool[PoolNum+1]='/';
						Pool[PoolNum+2]='\0';
						//描画
						graph_string(
							6.0f+12.0f/(float)SizeY*(float)SizeX*(float)(i-PoolNum)-(float)InputScreenSeekX,
							32.0f+12.0f*(float)LineCnt-(float)InputScreenSeekY,
							Pool, 5, 12.0f, FALSE);
						//行中緑色フラグを折る
						ColorFlag=0;
						//カウンタを戻す
						PoolNum=0;
						//読み取り位置を一つ回す
						i++;
					}
					//EOSの場合
					else if (TempLine[i]=='\0') {
						//Poolに追加
						Pool[PoolNum]='\0';
						//描画
						graph_string(
							6.0f+12.0f/(float)SizeY*(float)SizeX*(float)(i-PoolNum)-(float)InputScreenSeekX,
							32.0f+12.0f*(float)LineCnt-(float)InputScreenSeekY,
							Pool, 5, 12.0f, FALSE);
						//※行中緑色フラグは折らない
						//カウンタを戻す
						PoolNum=0;
					}
					//それ以外の場合
					else {
						//Poolに追加
						Pool[PoolNum]=TempLine[i];
						PoolNum++;
					}
				}
				//行後緑色フラグが立っている場合
				else if (ColorFlag==3) {
					//EOSの場合
					if (TempLine[i]=='\0') {
						//Poolに追加
						Pool[PoolNum]='\0';
						//描画
						graph_string(
							6.0f+12.0f/(float)SizeY*(float)SizeX*(float)(i-PoolNum)-(float)InputScreenSeekX,
							32.0f+12.0f*(float)LineCnt-(float)InputScreenSeekY,
							Pool, 5, 12.0f, FALSE);
						//行後緑色フラグを折る
						ColorFlag=0;
						//カウンタを戻す
						PoolNum=0;
					}
					//それ以外の場合
					else {
						//Poolに追加
						Pool[PoolNum]=TempLine[i];
						PoolNum++;
					}
				}
				i++;
			} while(TempLine[i-1]!='\0'); //←EOSが来た場合も描画処理を完了させないといけないためi-1で指定
		}
	}

	if (CursorCnt/40%2==0) {
		DrawLine(
			6+12.0f/(float)SizeY*(float)SizeX*((float)ScreenCursor+sourceTypeIndent[TypingLine])-(float)InputScreenSeekX,
			32+12*sourceTypeParallel[TypingLine]-(float)InputScreenSeekY,
			6+12.0f/(float)SizeY*(float)SizeX*((float)ScreenCursor+sourceTypeIndent[TypingLine])-(float)InputScreenSeekX,
			44+12*sourceTypeParallel[TypingLine]-(float)InputScreenSeekY,
			GetColor(1,0,0));
	}
}

void graph_main(){
	SetDrawArea(0,0,640,480);
	graph_sheet();
	SetDrawArea(6,30,634,340);
	graph_inputscreen();
	if (func_state==1) {
		SetDrawArea(6,340,634,480);
		graph_typescreen();
	}
	if (func_state==2) {
		SetDrawArea(0,0,640,480);
		graph_string(245.0f,215.0f,"CLEAR",0,51.0f,FALSE);
	}
}