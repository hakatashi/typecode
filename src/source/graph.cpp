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
	 0 ��v�i�D�F�j
	 1 �s��v�i�ԐF�j
	 2 �����́i���F�j
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
	 0 �L���̂���
	 1 �P��̓r��
	 2 �P��̂���
	***************************/
	state=0;
	/***************************
	Kindstate:
	 0 ��v�i�D�F�j
	 1 �s��v�i�ԐF�j
	***************************/
	Kindstate=0;
	SourcePoint=0;
	PoolPoint=0;
	ng_flag=FALSE;

	while(TypeInput[c]!='\0') {
		//��v��Ԃ̏ꍇ
		if (Kindstate==0) {
			switch(state) {
			case 0: //�L���̂���
				//�X�y�[�X�̏ꍇ
				if (TypeInput[c]==' ') {
					Pool[PoolPoint]=' ';
					PoolKind[PoolPoint]=0;
					if (ScreenCursor==c) TypeCursor=PoolPoint;
					PoolPoint++;
					//source�̂ق��ɂ��X�y�[�X������ꍇ
					if (sourceType[TypingLine][SourcePoint]==' ') {
						//source�����J�肷��
						SourcePoint++;
					}
				} else { //�X�y�[�X�łȂ��ꍇ
					//source���X�y�[�X�łȂ��Ȃ�܂ŉ�
					while (sourceType[TypingLine][SourcePoint]==' ') {
						Pool[PoolPoint]=' ';
						PoolKind[PoolPoint]=0;
						PoolPoint++;
						SourcePoint++;
					}
					//source��input����v���Ă���ꍇ
					if (TypeInput[c]==sourceType[TypingLine][SourcePoint]) {
						//Pool�ɋL�^����
						Pool[PoolPoint]=TypeInput[c];
						PoolKind[PoolPoint]=0;
						if (ScreenCursor==c) TypeCursor=PoolPoint;
						PoolPoint++;
						//�ȉ��Asource�̓ǂݐi�ߏ���
						//���݋L�^����char�������ŁA
						if (MyIsChar(sourceType[TypingLine][SourcePoint])) {
							//���̎��������Ȃ�΁u�P��̓r���v�Ŋm��B
							if (MyIsChar(sourceType[TypingLine][SourcePoint+1])) {
								state=1;
								//source���ꕶ���ǂݐi�߂�
								SourcePoint++;
							} else { //���̎��������łȂ��Ȃ�΁u�P��̂��Ɓv�Ŋm��B
								state=2;
								SourcePoint++;
							}
						} else { //���݋L�^����char�������łȂ��i���X�y�[�X�ł͂Ȃ����L���j�ꍇ�́u�L���̂��Ɓv�Ŋm��B
							state=0;
							SourcePoint++;
						}
					} else { //source��input����v���Ă��Ȃ��ꍇ
						//Kindstate��s��v�ɐݒ�
						Kindstate=1;
					}
				}
				break;
			case 1: //�P��̓r��
				//source��input����v���Ă�ꍇ
				if (TypeInput[c]==sourceType[TypingLine][SourcePoint]) {
					//Pool�ɋL�^����
					Pool[PoolPoint]=TypeInput[c];
					PoolKind[PoolPoint]=0;
					if (ScreenCursor==c) TypeCursor=PoolPoint;
					PoolPoint++;
					//�ȉ��Asource�̓ǂݐi�ߏ���
					//���݋L�^����char�͕����Ȃ̂ŁA
					//���̎��������Ȃ�΁u�P��̓r���v�Ŋm��B
					if (MyIsChar(sourceType[TypingLine][SourcePoint+1])) {
						state=1;
						//source���ꕶ���ǂݐi�߂�
						SourcePoint++;
					} else { //���̎��������łȂ��Ȃ�΁u�P��̂��Ɓv�Ŋm��B
						state=2;
						SourcePoint++;
					}
				} else { //��v���Ă��Ȃ��ꍇ�A�ⓚ���p�ŕs��v����
					Kindstate=1;
				}
				break;
			case 2: //�P��̂���
				//�X�y�[�X�łȂ��ꏊ��T��
				temp=0;
				while(sourceType[TypingLine][SourcePoint+temp]==' ') temp++;
				//����char�������ŁA����Pool�ɃX�y�[�X���}������ĂȂ��ꍇ
				if (MyIsChar(sourceType[TypingLine][SourcePoint+temp]) && Pool[PoolPoint-1]!=' ') {
					//�X�y�[�X�Ȃ�Pool�ɋL�^����
					if (TypeInput[c]==' ') {
						Pool[PoolPoint]=' ';
						PoolKind[PoolPoint]=0;
						if (ScreenCursor==c) TypeCursor=PoolPoint;
						PoolPoint++;
						SourcePoint++;
					} else { //�X�y�[�X�łȂ��ꍇ�A�ⓚ���p�ŕs��v����
						Kindstate=1;
					}
				} else { //����char���L���A�������̓X�y�[�X���}������Ă���ꍇ
					//input���X�y�[�X�̏ꍇ
					if (TypeInput[c]==' ') {
						Pool[PoolPoint]=' ';
						PoolKind[PoolPoint]=0;
						if (ScreenCursor==c) TypeCursor=PoolPoint;
						PoolPoint++;
						//source�̂ق��ɂ��X�y�[�X������ꍇ
						if (sourceType[TypingLine][SourcePoint]==' ') {
							//source�����J�肷��
							SourcePoint++;
						}
					} else { //�X�y�[�X�łȂ��ꍇ
						//source���X�y�[�X�łȂ��Ȃ�܂ŉ�
						while (sourceType[TypingLine][SourcePoint]==' ') {
							Pool[PoolPoint]=' ';
							PoolKind[PoolPoint]=0;
							PoolPoint++;
							SourcePoint++;
						}
						//source��input����v���Ă���ꍇ
						if (TypeInput[c]==sourceType[TypingLine][SourcePoint]) {
							//Pool�ɋL�^����
							Pool[PoolPoint]=TypeInput[c];
							PoolKind[PoolPoint]=0;
							if (ScreenCursor==c) TypeCursor=PoolPoint;
							PoolPoint++;
							//�ȉ��Asource�̓ǂݐi�ߏ���
							//���݋L�^����char�������ŁA
							if (MyIsChar(sourceType[TypingLine][SourcePoint])) {
								//���̎��������Ȃ�΁u�P��̓r���v�Ŋm��B
								if (MyIsChar(sourceType[TypingLine][SourcePoint+1])) {
									state=1;
									//source���ꕶ���ǂݐi�߂�
									SourcePoint++;
								} else { //���̎��������łȂ��Ȃ�΁u�P��̂��Ɓv�Ŋm��B
									state=2;
									SourcePoint++;
								}
							} else { //���݋L�^����char�������łȂ��i���X�y�[�X�ł͂Ȃ����L���j�ꍇ�́u�L���̂��Ɓv�Ŋm��B
								state=0;
								SourcePoint++;
							}
						} else { //source��input����v���Ă��Ȃ��ꍇ
							//Kindstate��s��v�ɐݒ�
							Kindstate=1;
						}
					}
				}
				break;
			}
		}
		if (Kindstate==1) { //�s��v��Ԃ̏ꍇ�i��̏����ŕs��v��ԂƂȂ����ꍇ���܂ށj
			//�s��v��ԂƂ���Pool�ɋL�^
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
	 0 �ʏ�i���܂��͐j
	 1 ���F
	 2 �ΐF�i�s���^�C�v�j
	 3 �ΐF�i�s��^�C�v�u//�`�`�v�j
	***************/

	GetGraphSize(img_char[0][FALSE][0],&SizeX,&SizeY);

	PoolNum=0;
	ColorFlag=0;
	for ( LineCnt=(int)(InputScreenSeekY/12.0) ; LineCnt<=sourceTypeParallel[TypingLine] ; LineCnt++ ) {

		//TempLine�Ɉꎞ�I�Ɉ�s�i�[
		//���͒��̍s�łȂ��ꍇ
		if (LineCnt<sourceTypeParallel[TypingLine]) {
			//TempLine��TypeStack�̓��e���i�[
			for (i=0;TypeStack[LineCnt][i]!='\0';i++) TempLine[i]=TypeStack[LineCnt][i];
			//EOS���i�[
			TempLine[i]='\0';
			TempLine[i+1]='\0';
		}
		//���͒��̍s�̏ꍇ
		else {
			//TempLine��TypeStack�̓��e���i�[
			for (i=0;TypeStack[LineCnt][i]!='\0';i++) TempLine[i]=TypeStack[LineCnt][i];
			//TempLine��TypeInput�̓��e���i�[
			for (j=0;TypeInput[j]!='\0';j++) TempLine[i+j]=TypeInput[j];
			//EOS���i�[
			TempLine[i+j]='\0';
			TempLine[i+j+1]='\0';
		}

		if (TempLine[0]!='\0') {

			i=0;
			do {
				//�ʏ�t���O�������Ă���ꍇ
				if (ColorFlag==0) {
					//���͕������s���R�����g�J�n�̏ꍇ
					if (TempLine[i]=='/' && TempLine[i+1]=='*') {
						//Pool�ɕ����������Ă���ꍇ�́A
						if (PoolNum>0) {
							//EOS��t��
							Pool[PoolNum]='\0';
							//�����ꃊ�X�g�Ɉ�v����ꍇ�͐F�t���O�𗧂Ă�
							BlueFlag=0;
							for (j=0;j<KeywordNum[0];j++) {
								if (strcmp(Keyword[0][j],Pool)==0) {
									BlueFlag=1;
									break;
								}
							}
							//�����`��
							graph_string(
								6.0f+12.0f/(float)SizeY*(float)SizeX*(float)(i-PoolNum)-(float)InputScreenSeekX,
								32.0f+12.0f*(float)LineCnt-(float)InputScreenSeekY,
								Pool, BlueFlag?1:0, 12.0f, BlueFlag?TRUE:FALSE);
						}
						//�s���ΐF�t���O�𗧂Ă�
						ColorFlag=2;
						//�J�E���^��߂�
						Pool[0]='/';
						Pool[1]='*';
						PoolNum=2;
						//�ǂݎ��ʒu�����
						i++;
					}
					//���͕������s��R�����g�J�n�̏ꍇ
					else if (TempLine[i]=='/' && TempLine[i+1]=='/') {
						//Pool�ɕ����������Ă���ꍇ�́A
						if (PoolNum>0) {
							//EOS��t��
							Pool[PoolNum]='\0';
							//�����ꃊ�X�g�Ɉ�v����ꍇ�͐F�t���O�𗧂Ă�
							BlueFlag=0;
							for (j=0;j<KeywordNum[0];j++) {
								if (strcmp(Keyword[0][j],Pool)==0) {
									BlueFlag=1;
									break;
								}
							}
							//�����`��
							graph_string(
								6.0f+12.0f/(float)SizeY*(float)SizeX*(float)(i-PoolNum)-(float)InputScreenSeekX,
								32.0f+12.0f*(float)LineCnt-(float)InputScreenSeekY,
								Pool, BlueFlag?1:0, 12.0f, BlueFlag?TRUE:FALSE);
						}
						//�s���ΐF�t���O�𗧂Ă�
						ColorFlag=3;
						//�J�E���^��߂�
						Pool[0]='/';
						Pool[1]='/';
						PoolNum=2;
						//�ǂݎ��ʒu�����
						i++;
					}
					//���͕��������p���̏ꍇ
					else if (TempLine[i]=='"' || TempLine[i]==39/*�u'�v=39*/) {
						//Pool�ɕ����������Ă���ꍇ�́A
						if (PoolNum>0) {
							//EOS��t��
							Pool[PoolNum]='\0';
							//�����ꃊ�X�g�Ɉ�v����ꍇ�͐F�t���O�𗧂Ă�
							BlueFlag=0;
							for (j=0;j<KeywordNum[0];j++) {
								if (strcmp(Keyword[0][j],Pool)==0) {
									BlueFlag=1;
									break;
								}
							}
							//�����`��
							graph_string(
								6.0f+12.0f/(float)SizeY*(float)SizeX*(float)(i-PoolNum)-(float)InputScreenSeekX,
								32.0f+12.0f*(float)LineCnt-(float)InputScreenSeekY,
								Pool, BlueFlag?1:0, 12.0f, BlueFlag?TRUE:FALSE);
						}
						//���F�t���O�𗧂Ă�
						ColorFlag=1;
						//�J�E���^��߂���Pool�Ɂi���p������j�L�^�J�n
						Pool[0]=TempLine[i];
						PoolNum=1;
					}
					//���͕�����������̏ꍇ
					else if (('0'<=TempLine[i] && TempLine[i]<='9')
						|| ('A'<=TempLine[i] && TempLine[i]<='Z')
						|| ('a'<=TempLine[i] && TempLine[i]<='z')
						|| TempLine[i]=='_'
						|| TempLine[i]=='#') {
							//Pool�Ɉꕶ���ǉ�
							Pool[PoolNum]=TempLine[i];
							PoolNum++;
					}
					//���͕������i���̑����j�L����������EOS�̏ꍇ
					else {
						//Pool�ɕ����������Ă���ꍇ�́A
						if (PoolNum>0) {
							//EOS��t��
							Pool[PoolNum]='\0';
							//�����ꃊ�X�g�Ɉ�v����ꍇ�͐F�t���O�𗧂Ă�
							BlueFlag=0;
							for (j=0;j<KeywordNum[0];j++) {
								if (strcmp(Keyword[0][j],Pool)==0) {
									BlueFlag=1;
									break;
								}
							}
							//�����`��
							graph_string(
								6.0f+12.0f/(float)SizeY*(float)SizeX*(float)(i-PoolNum)-(float)InputScreenSeekX,
								32.0f+12.0f*(float)LineCnt-(float)InputScreenSeekY,
								Pool, BlueFlag?1:0, 12.0f, BlueFlag?TRUE:FALSE);
						}
						//Pool�ɋL�����ꕶ�������������
						Pool[0]=TempLine[i];
						Pool[1]='\0';
						//�`��
						graph_string(
							6.0f+12.0f/(float)SizeY*(float)SizeX*(float)i-(float)InputScreenSeekX,
							32.0f+12.0f*(float)LineCnt-(float)InputScreenSeekY,
							Pool, 0, 12.0f, FALSE);
						//�J�E���^��߂�
						PoolNum=0;
					}
				}
				//���F�t���O�������Ă���ꍇ
				else if (ColorFlag==1) {
					//�Ƃ肠����Pool�Ɉꕶ���ǉ�
					Pool[PoolNum]=TempLine[i];
					PoolNum++;
					//���p���܂���EOS�����Ă����ꍇ
					if (TempLine[i]=='"' || TempLine[i]==39/*�u'�v*/ || TempLine[i]=='\0') {
						//�����EOS��ǉ�
						Pool[PoolNum]='\0';
						//�`��
						graph_string(
							6.0f+12.0f/(float)SizeY*(float)SizeX*(float)(i-PoolNum+1)-(float)InputScreenSeekX,
							32.0f+12.0f*(float)LineCnt-(float)InputScreenSeekY,
							Pool, 2, 12.0f, FALSE);
						//���F�t���O��܂�
						ColorFlag=0;
						//�J�E���^��߂�
						PoolNum=0;
					}
				}
				//�s���ΐF�t���O�������Ă���ꍇ
				else if (ColorFlag==2) {
					//�s���I���L���̏ꍇ
					if ( TempLine[i]=='*' && TempLine[i+1]=='/' ) {
						//Pool�ɒǉ�
						Pool[PoolNum]='*';
						Pool[PoolNum+1]='/';
						Pool[PoolNum+2]='\0';
						//�`��
						graph_string(
							6.0f+12.0f/(float)SizeY*(float)SizeX*(float)(i-PoolNum)-(float)InputScreenSeekX,
							32.0f+12.0f*(float)LineCnt-(float)InputScreenSeekY,
							Pool, 5, 12.0f, FALSE);
						//�s���ΐF�t���O��܂�
						ColorFlag=0;
						//�J�E���^��߂�
						PoolNum=0;
						//�ǂݎ��ʒu�����
						i++;
					}
					//EOS�̏ꍇ
					else if (TempLine[i]=='\0') {
						//Pool�ɒǉ�
						Pool[PoolNum]='\0';
						//�`��
						graph_string(
							6.0f+12.0f/(float)SizeY*(float)SizeX*(float)(i-PoolNum)-(float)InputScreenSeekX,
							32.0f+12.0f*(float)LineCnt-(float)InputScreenSeekY,
							Pool, 5, 12.0f, FALSE);
						//���s���ΐF�t���O�͐܂�Ȃ�
						//�J�E���^��߂�
						PoolNum=0;
					}
					//����ȊO�̏ꍇ
					else {
						//Pool�ɒǉ�
						Pool[PoolNum]=TempLine[i];
						PoolNum++;
					}
				}
				//�s��ΐF�t���O�������Ă���ꍇ
				else if (ColorFlag==3) {
					//EOS�̏ꍇ
					if (TempLine[i]=='\0') {
						//Pool�ɒǉ�
						Pool[PoolNum]='\0';
						//�`��
						graph_string(
							6.0f+12.0f/(float)SizeY*(float)SizeX*(float)(i-PoolNum)-(float)InputScreenSeekX,
							32.0f+12.0f*(float)LineCnt-(float)InputScreenSeekY,
							Pool, 5, 12.0f, FALSE);
						//�s��ΐF�t���O��܂�
						ColorFlag=0;
						//�J�E���^��߂�
						PoolNum=0;
					}
					//����ȊO�̏ꍇ
					else {
						//Pool�ɒǉ�
						Pool[PoolNum]=TempLine[i];
						PoolNum++;
					}
				}
				i++;
			} while(TempLine[i-1]!='\0'); //��EOS�������ꍇ���`�揈�������������Ȃ��Ƃ����Ȃ�����i-1�Ŏw��
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