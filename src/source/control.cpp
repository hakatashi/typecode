#include "../header/GV.h"

void control_fasten() {
	int i,j,refer,Srefer;
	int flag;

	//Typeのぶんの一行をsourceに固定
	for ( refer=0 ; TypeStack[ sourceTypeParallel[TypingLine] ][ refer ]!='\0' ; refer++ ) ;
	for ( i=0 ; TypeInput[i]!='\0' ; i++ )
		TypeStack[ sourceTypeParallel[TypingLine] ][ refer+i ] = TypeInput[i];
	refer+=i;

	//TypeStackにTypeの行のぶんのコメントを付加する
	//sourceで「//」が最初にヒットするところを探す。
	flag=0;
	for ( i=0 ; source[ sourceTypeParallel[TypingLine] ][ i ]!='\0' ; i++ ) {
		if (source[ sourceTypeParallel[TypingLine] ][ i ] == '/' && source[ sourceTypeParallel[TypingLine] ][ i+1 ] == '/') {
			flag=1;
			break;
		}
	}
	//ヒットした場合
	if (flag==1) {
		//コメント付加処理

		//sourceで「//」から遡ってスペースとタブを取得。	
		for ( Srefer=i-1 ; Srefer>=0 ; Srefer-- ) {
			if ( source[ sourceTypeParallel[TypingLine] ][ Srefer ] != ' ' && source[ sourceTypeParallel[TypingLine] ][ Srefer ] != '\t' ) {
				break;
			}
		}
		Srefer++;

		//sourceのSreferの地点からTypeStackのreferの地点にコメント付加
		for ( i=0 ; source[ sourceTypeParallel[TypingLine] ][ Srefer+i ] != '\0' ; i++ )
			TypeStack[ sourceTypeParallel[TypingLine] ][ refer+i ]
				= source[ sourceTypeParallel[TypingLine] ][ Srefer+i ];
		refer+=i;
	}
	TypeStack[ sourceTypeParallel[TypingLine] ][ refer ] = '\0';

	//次の入力行の一つ手前の行までTypeStackに付加
	for ( i=sourceTypeParallel[TypingLine]+1 ; i<sourceTypeParallel[TypingLine+1] ; i++) {
		for ( j=0 ; source[i][j]!='\0' ; j++ ) TypeStack[i][j]=source[i][j];
		TypeStack[i][j]='\0';
	}

	//次の入力行のインデントを入力
	for ( i=0 ; i<sourceTypeIndent[TypingLine+1] ; i++ )
		TypeStack[ sourceTypeParallel[TypingLine+1] ][ i ]
			= source[ sourceTypeParallel[TypingLine+1] ][ i ];
	TypeStack[ sourceTypeParallel[TypingLine+1] ][ i ] = '\0';
}

void control_game_main() {
	int i;
	int SizeX,SizeY;
	double aim;

	//Enterキーが押された時にOKflagが立っているなら確定
	if (OKflag==TRUE && CheckHitKey( KEY_INPUT_RETURN ) == 1) {
		if (TypingLine+1<CodeTypeLength) {
			control_fasten();
			TypingLine++;
			ScreenCursor=0;
			TypeCursor=0;
			InputSeekX=0.0;
			InputSeekY+=20.0;
			for (i=0;i<CODE_CHAR_MAX;i++) TypeInput[i]='\0';
		} else {
			ScreenCursor=0;
			TypeCursor=0;
			InputSeekX=0.0;
			func_state=2;
		}
	}

	GetGraphSize(img_char[0][0][0],&SizeX,&SizeY);

	//InputScreenのスクロール処理
	//X座標
	if (sourceTypeIndent[TypingLine]+ScreenCursor<=85) aim=0.0;
	else aim=(sourceTypeIndent[TypingLine]+ScreenCursor-85)*12.0/(double)SizeY*(double)SizeX;
	InputScreenSeekX=(InputScreenSeekX-aim)/1.2+aim;
	//Y座標
	if (sourceTypeParallel[TypingLine]<=23) aim=0.0;
	else aim=(sourceTypeParallel[TypingLine]-23)*12.0;
	InputScreenSeekY=(InputScreenSeekY-aim)/1.2+aim;

	//TypeScreenのスクリーン処理
	InputSeekY=InputSeekY/1.6;

	CursorCnt++;
	StageTime++;
}

void control_main() {
	switch (func_state) {
	case 1:
		control_game_main();
		break;
	}
}