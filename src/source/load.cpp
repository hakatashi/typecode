#include "../header/GV.h"

int load_code() {
	int FileHandle;
	int cnt;
	int SpaceFlag,CommentFlag;
	int i;
	int CharCnt,LineCnt,CalCnt;
	int Indent;
	char MultiTest[4];
	char FileName[256]="Data/code/code.txt";
	char SourceTemp[CODE_CHAR_MAX];

	FileHandle = FileRead_open(FileName);

	cnt=0;
	LineCnt=0;
	CommentFlag=0;

	while(FileRead_eof(FileHandle)==0) {
		if (cnt>=CODE_LINE_MAX) {
			printfDx("error : %s : コードが長すぎます(max=%d)\n",FileName,CODE_LINE_MAX);
			return -1;
		}
		FileRead_gets(SourceTemp,CODE_CHAR_MAX,FileHandle);

		//タブをスペースに変換
		CalCnt=0;
		for (i=0;SourceTemp[i]!='\0';i++) {
			if (CalCnt>=CODE_CHAR_MAX) {
				printfDx("error : %s : 一行の文字数が長すぎます(line:%d)\n",FileName,cnt+1);
				return -1;
			} else if (SourceTemp[i]=='\t') {
				do {
					source[cnt][CalCnt]=' ';
					CalCnt++;
				} while( CalCnt%4 != 0 ) ;
			} else {
				source[cnt][CalCnt]=SourceTemp[i];
				CalCnt++;
			}
		}

		SpaceFlag=1;
		i=0;
		CharCnt=0;
		Indent=CODE_CHAR_MAX;

		while(source[cnt][i]!='\0') {

			//コメントアウトをカット
			if (CommentFlag==0 && source[cnt][i]=='/' && source[cnt][i+1]=='*') {
				CommentFlag=1;
			}
			if (CommentFlag==1 && source[cnt][i]=='*' && source[cnt][i+1]=='/') {
				CommentFlag=0;
				i+=2;
				continue;
			}
			if (CommentFlag==0 && source[cnt][i]=='/' && source[cnt][i+1]=='/') {
				break;
			}

			//インデントをカット
			if (SpaceFlag==1 && CommentFlag==0 && source[cnt][i]!=' ' && source[cnt][i]!='\t') SpaceFlag=0;

			//条件を満たすならsourceTypeに代入
			if (SpaceFlag==0 && CommentFlag==0) {
				//全角文字かどうかの試験
				MultiTest[0]=source[cnt][i];
				MultiTest[1]=source[cnt][i+1];
				MultiTest[2]=source[cnt][i+2];
				MultiTest[3]='\0';
				if (MultiByteCharCheck(MultiTest,DX_CHARSET_SHFTJIS)==TRUE) {
					printfDx("error : %s : 全角文字が含まれています(%d)\n",FileName,cnt+1);
					return -1;
				}
				//代入処理
				sourceType[LineCnt][CharCnt]=source[cnt][i];
				if (sourceType[LineCnt][CharCnt]=='\t') sourceType[LineCnt][CharCnt]=' ';
				//インデントを計算
				if (Indent>i) Indent=i;
				CharCnt++;
			}

			i++;
		}

		if (CharCnt>0) {
			for (i=CharCnt-1 ; i>=0 ; i++) {
				if ( sourceType[LineCnt][i] != ' ' && sourceType[LineCnt][i] != '\t') {
					break;
				} else {
					sourceType[LineCnt][i]='\0';
				}
			}
			sourceTypeParallel[LineCnt]=cnt;
			sourceTypeIndent[LineCnt]=Indent;
			sourceType[LineCnt][i+1]='\0';
			LineCnt++;
		}

		cnt++;
	}

	FileRead_close(FileHandle);

	CodeLength=cnt;
	CodeTypeLength=LineCnt;

	return 0;
}

void load_image() {
	img_bg = LoadGraph( "Data/img/sheet.png") ;
	LoadDivGraph( "Data/img/charBLACK.png" , 95, 95, 1, 30, 51, img_char[0][0]);
	LoadDivGraph( "Data/img/charBLACK_B.png" , 95, 95, 1, 30, 51, img_char[0][1]);
	LoadDivGraph( "Data/img/charBLUE.png" , 95, 95, 1, 30, 51, img_char[1][0]);
	LoadDivGraph( "Data/img/charBLUE_B.png" , 95, 95, 1, 30, 51, img_char[1][1]);
	LoadDivGraph( "Data/img/charBROWN.png" , 95, 95, 1, 30, 51, img_char[2][0]);
	LoadDivGraph( "Data/img/charBROWN_B.png" , 95, 95, 1, 30, 51, img_char[2][1]);
	LoadDivGraph( "Data/img/charGRAY.png" , 95, 95, 1, 30, 51, img_char[3][0]);
	LoadDivGraph( "Data/img/charGRAY_B.png" , 95, 95, 1, 30, 51, img_char[3][1]);
	LoadDivGraph( "Data/img/charRED.png" , 95, 95, 1, 30, 51, img_char[4][0]);
	LoadDivGraph( "Data/img/charRED_B.png" , 95, 95, 1, 30, 51, img_char[4][1]);
	LoadDivGraph( "Data/img/charGREEN.png" , 95, 95, 1, 30, 51, img_char[5][0]);
	LoadDivGraph( "Data/img/charGREEN_B.png" , 95, 95, 1, 30, 51, img_char[5][1]);
}

void load_keyword() {
	int FileHandle;
	int i;
	char FileName[256]="Data/key/cpp.kwd";

	FileHandle=FileRead_open(FileName);

	i=0;
	while(FileRead_eof(FileHandle)==0) {
		FileRead_gets(Keyword[0][i],KEYWORD_CHAR_MAX-1,FileHandle);
		i++;
	}
	KeywordNum[0]=i;

	FileRead_close(FileHandle);
}

int load(){
	load_image();
	load_keyword();
	if (load_code()==-1) return -1;
	return 0;
}