/*
	非依存型make。
	非依存型と言うだけで(targetが無い)、nmkやmakerと同じ挙動。
	リバースエンジニアリングは一切行っておりません。
	途中からコメントの書き方が変わっています。ご了承下さい。
	自作のmake.asm(1992)をリライトしただけです。
*/
/**
mml2ms.c

Copyright 1996 kenjishinmyou

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/
/* 改行は mac0dh,dos0dh0ah,unix0ah
============================================================================
============================================================================
 		include
============================================================================
==========================================================================*/
#include <windows.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <mbctype.h>
#include <mbstring.h>
#include <time.h>
#include <conio.h>
#include <direct.h>





/*============================================================================
==============================================================================
 		prototype
==============================================================================
============================================================================*/
int makecall(int argv, char **argc );


/*============================================================================
==============================================================================
 		define
==============================================================================
============================================================================*/
/* マクロサイズ */
#define EXECAREASIZE	(1024*1024)
#define RULEAREASIZE	(1024*512)
#define MACROAREASIZE	(1024*512)


#define	ASCII_CR	0x0d
#define	ASCII_LF	0x0a
#define	ASCII_TAB	0x09
#define	ASCII_EOF	0x1a
#define MAX_BUFF	(1024*128)

#define MAXDEPENDENT	128	/* 128 files */
#define FILECOPYSIZE	(1024*1024)	/* file を copy するときのバッファーのサイズ */
#define MAX_ARGC		256

int fPrint;					/* 画面表示を行うか? */
int makemode;
BYTE szTool0[_MAX_PATH];	/* tool の名前 */
BYTE szTool1[_MAX_PATH];	/* tool の名前

int expand_macro_flag;
WORD option_flag;			/* オプション
							00bit 0x01 -S swap(kmake専用)
							01bit 0x02 -i ignore error
							02bit 0x04 -B 強制ビルド
							03bit 0x08 -e env
							04bit 0x10 -N 削除しない
							05bit 0x20 -A 全てのファイルを削除の後、実行
					*/


BYTE szCommand0[_MAX_PATH];
BYTE szCommand1[_MAX_PATH];


/*
	macro関連

 Dependent Macro を展開するための物 GetDependentTime で設定される
*/
char DependentBuff[MAX_BUFF];
char *DependentPoint[MAXDEPENDENT];
/* Target Macro を展開するための物 */
BYTE *TargetPoint;


/*
	リダイレクト関連
*/
int mRedirectIn,mRedirectOut,mRedirectPipeOut,mRedirectPipeIn;
int handle_in_new,handle_in_old;
int handle_out_new,handle_out_old;
BYTE szPipeOut[_MAX_PATH],szPipeIn[_MAX_PATH];

/*============================================================================
=============================================================================
  	sub
============================================================================
============================================================================*/
void printerr( void )
{
	LPVOID lpMsgBuf;
	FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, 
					NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // 既定の言語
					(LPTSTR) &lpMsgBuf, 0, NULL );
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, "Error", MB_OK | MB_ICONINFORMATION);
	/* バッファを解放する。 */
	LocalFree(lpMsgBuf);
}
/*----------------------------------------------------------------------------
	file sub
----------------------------------------------------------------------------*/
int ExistFile( BYTE *szFile )
{
	HANDLE	hFind;
	WIN32_FIND_DATA fd;

	hFind = FindFirstFile( szFile, &fd );
	if( hFind == INVALID_HANDLE_VALUE ) {
		return 0;
	}
	FindClose( hFind );
	return 1;
}
/*----------------------------------------------------------------------------*/

int  CopyFileHandle( int src, BYTE *lpout )
/**
	ハンドルとファイル名で指定されたデーターのコピー
*/
{
	int dist;
	long s;
	BYTE *lp;

	dist = _open( lpout,_O_RDWR|_O_CREAT,_S_IREAD | _S_IWRITE );
	if( dist == -1 ) return 28;
	_lseek( dist,0,SEEK_END );
	if( dist == -1 ) return 28;

	s = _lseek( src,0,SEEK_SET );
	if( s==-1 ) return 28;

	lp=malloc( FILECOPYSIZE );
	if( lp==NULL ) return 28;

	while( 1 ) {
		s = _read( src, lp, FILECOPYSIZE );
		if( s==0 ) break;
		if( s==-1 ) return 28;
		s = _write( dist,lp,s );
		if( s==-1 ) return 28;
	}

	_close( dist );

	return 0;
}
//----------------------------------------------------------------------------
void PintTimeFile( FILETIME *fp )
{
	SYSTEMTIME stFileTime;

	FileTimeToSystemTime(fp , &stFileTime);
	printf("%d年 %d月 %d日 %d時 %d分 %d秒 %d" ,
		stFileTime.wYear , stFileTime.wMonth ,
		stFileTime.wDay , stFileTime.wHour ,
		stFileTime.wMinute , stFileTime.wSecond, stFileTime.wMilliseconds
	);
	return;
}
//----------------------------------------------------------------------------
FILETIME GetFileTimeFile( BYTE *szFile )
{
	HANDLE	hFind;
	WIN32_FIND_DATA fd;

	hFind = FindFirstFile( szFile, &fd );
	if( hFind == INVALID_HANDLE_VALUE ) {
		fd.ftLastWriteTime.dwLowDateTime = 0;
		fd.ftLastWriteTime.dwHighDateTime = 0;
	} else {
		FindClose( hFind );
	}
	return fd.ftLastWriteTime;
}
/*============================================================================
============================================================================
  	macro
============================================================================
============================================================================*/
/*----------------------------------------------------------------------------
	expand macro
----------------------------------------------------------------------------*/
int ExpandMacroPathname( char *lpw, char **ppw, char ch0, char ch1 )
{
/**

	1文字 macro の pathname の分割収得
  もともと lpw に与えられた pathanem を、分割して必要な部分だけ lpw に書く
	バッファーは使わないで、lpw を直接書き換える。
	よって、lpw の元々の pathname は壊される

	変更したときのみ、**ppw へ lpw を書く
	呼び出し側の問題ではあるが、**ppw へ lpw を書けば、文字列の最後を 呼び出し側の lpw が示すようになる。
	書き変わらない場合には、元々呼び出し側の lpw が文字列の最後を指し示しているので問題ない


(in)	ch1 = type
		ch0 = $ なら、exit
		dx = pointer to full path

(out)	0:置換しなかった
		1:置換した
*/
	BYTE *lptmp,*lpworg,*lpext;
	BYTE ch;

	if( ch0=='$' ) {
		return 0;
	}

	switch( ch1 ) {
		case 'D':
			// D drive:\path\       c:\dir\ 
			lptmp = NULL;
			while( 1 ) {
				ch = *lpw;
				lpw++;
				if( ch==0 ) {
					if( lptmp!=NULL ) {
						lpw = lptmp;
						*lpw=0;
					}
					*ppw = lpw;
					return 1;
				} else if( _ismbblead(ch)!=0 ) {
					// 漢字の場合
					if( _ismbbtrail(*lpw)!=0 ) {
						lpw++;
					}
				} else if( ch=='\\' ) {
					lptmp = lpw;
				}
			}
			break;

		case 'F':
			// F base.ext           prog.asm
			lptmp = NULL;
			lpworg = lpw;
			while( 1 ) {
				ch = *lpw;
				lpw++;
				if( ch==0 ) {
					if( lptmp!=NULL ) {
						lpw = lpworg;
						do {
							ch = *lptmp++;
							*lpw++ = ch;
						} while( ch!= 0 );
						lpw--;
					}
					*ppw = lpw;
					return 1;
				} else if( _ismbblead(ch)!=0 ) {
					// 漢字の場合
					if( _ismbbtrail(*lpw)!=0 ) {
						lpw++;
					}
				} else if( ch=='\\' ) {
					lptmp = lpw;
				}
			}
			break;

		case 'B':
			// B base               prog
			lptmp = NULL;
			lpworg = lpw;
			while( 1 ) {
				ch = *lpw;
				lpw++;
				if( ch==0 ) {
					if( lptmp!=NULL ) {
						// '.' を発見する
						lpw = lpworg;
						lpext = NULL;
						do {
							ch = *lptmp++;
							if( ch=='.' ) lpext = lpw;
							*lpw++ = ch;
						} while( ch!=0 );
						lpw--;
						if( lpext!=NULL ) {
							lpw=lpext;
							*lpw=0;
						}
					}
					*ppw = lpw;
					return 1;
				} else if( _ismbblead(ch)!=0 ) {
					// 漢字の場合
					if( _ismbbtrail(*lpw)!=0 ) {
						lpw++;
					}
				} else if( ch=='\\' ) {
					lptmp = lpw;
				}
			}
			break;

		case 'R':
			// R drive:\path\base   c:\dir\prog
			lptmp = NULL;
			while( 1 ) {
				ch = *lpw;
				if( ch==0 ) {
					if( lptmp!=NULL ) {
						lpw = lptmp;
						*lpw = 0;
					}
					*ppw = lpw;
					return 1;
				}
				if( ch=='.' ) lptmp=lpw;
				lpw++;
			}
			break;
	}
	return 0;



}
/*----------------------------------------------------------------------------*/
int ExpandMacroSubSubSame( char *lpMacroname, char **ppw, char *lpmax, BYTE *lpMacroTop, BYTE* lpMacroEnd )
{
	BYTE ch;
	char *lpw;
	char *lpCmp;
	char *lpDep;
	char *lpTar;
	char *lpDx;
	int c;

	lpw=*ppw;

	if( *lpMacroname=='*' ){
		if( *(lpMacroname+1)=='*' ) {
			/*
				'**'
				全ての dependent 
				例外的に、これだけは 2 文字で 1 moji とみなす
			*/
			lpDep = DependentBuff;
			if( *lpDep!=0 ) {
				while(1) {
					lpDx = lpw;
					do{
						ch = *lpDep++;
						*lpw++ = ch;
					} while( ch!=0 );
					lpw--;
					/* lpDx はファイル名の先頭、lpwはファイル名の最後の0を示している */
					ExpandMacroPathname( lpDx, &lpw, *(lpMacroname-1), *(lpMacroname+2));
					if( *lpDep==0 ) {
						// exit
						*ppw=lpw;
						return 0;
					}
					*lpw++=' ';
				}
			}
			/* exit */
			*ppw=lpw;
			return 0;

		} else {
			/*
				'*'
				一番最初の target から拡張子を除いた pathname
			*/
			lpTar = TargetPoint;
			lpDx = lpw;
			do {
				ch = *lpTar++;
				*lpw++ = ch;
			} while( ch!=0 );
			lpw--;
			/ lpDx はファイル名の先頭、lpwはファイル名の最後の0を示している */
			if( ExpandMacroPathname( lpDx, &lpw, *(lpMacroname-1), *(lpMacroname+1))==0 ) {
				/* path置換記号(D,F,B,R)で置換しなかった場合、拡張子を取り除くために 'R' で置換する */
				ExpandMacroPathname( lpDx, &lpw, '(', 'R');
			}
			/* exit */
			*ppw=lpw;
			return 0;

		}
	} else if( *lpMacroname=='@' ) {
		/*
			'@'
			一番最初の target の pathname
		*/
		lpTar = TargetPoint;
		lpDx = lpw;
		do {
			ch = *lpTar++;
			*lpw++ = ch;
		} while( ch!=0 );
		lpw--;
		/* lpDx はファイル名の先頭、lpwはファイル名の最後の0を示している */
		ExpandMacroPathname( lpDx, &lpw, *(lpMacroname-1), *(lpMacroname+1));
		/* exit */
		*ppw=lpw;
		return 0;

	} else if( *lpMacroname=='?' ) {
		/*
			'?'
			target より新しい dependent
		*/
		c=0;
		if( DependentPoint[c] != NULL ) {
			while( 1 ) {
				lpDep = DependentPoint[c++];
				lpDx = lpw;
				do {
					ch = *lpDep++;
					*lpw++ = ch;
				} while( ch!=0 );
				lpw--;
				/* lpDx はファイル名の先頭、lpwはファイル名の最後の0を示している */
				ExpandMacroPathname( lpDx, &lpw, *(lpMacroname-1), *(lpMacroname+1));
				if( DependentPoint[c] == NULL ) break;
				*lpw++ = ' ';
			}
		}
		/* exit */
		*ppw=lpw;
		return 0;

	} else if( isdigit(*lpMacroname)!=0 ) {
		/*
			'1' - '9'
			任意の dependent
		*/
		c = (*lpMacroname)-'1';
		lpDep = DependentBuff;
		// c個の dependent を skip する
		while( c!=0 ) {
			if( *lpDep==0 ) return 0;
			do{
			} while( *lpDep++!=0 );
			c--;
		}
		lpDx = lpw;
		do{
			ch = *lpDep++;
			*lpw++ = ch;
		} while( ch!=0 );
		lpw--;
		ExpandMacroPathname( lpDx, &lpw, *(lpMacroname-1), *(lpMacroname+1));
		*ppw=lpw;
		return 0;

	} else {
		/*
			else 文字列macro
		*/
		while( 1 ) {
			ch = *lpMacroTop;
			if( ch==0 ) {			/* 0:終了 見つからなかった場合 */
				// 環境変数の展開
				if( !(option_flag&0x0008) ) {
					lpMacroTop = getenv( lpMacroname );
					if( lpMacroTop!=NULL ) {
						// 書き込み
						do {
							ch = *lpMacroTop++;
							*lpw++ = ch;
							if( lpw>=lpmax ) return 6;
						} while( ch!=0 );
						lpw--;
					}
				}
				// exit
				*ppw=lpw;
				return 0;
			} else if( ch==2 ) {	// 2:無効の場合 skip
				goto same_next2;
			} else {				// 1:有効の場合
				lpCmp = lpMacroname;
				do {
					lpMacroTop++;
					ch = *lpCmp++;
					if( *lpMacroTop!=ch ) goto same_next2a;
				} while( ch!=0 );
				// 書き込み
				do {
					lpMacroTop++;
					ch = *lpMacroTop;
					*lpw++ = ch;
					if( lpw>=lpmax ) return 6;
				} while( ch!=0 );
				lpw--;
				// exit
				*ppw=lpw;
				return 0;
			}

			// skip macro name
same_next2:
			do{
				lpMacroTop++;
same_next2a:;
			} while( *lpMacroTop!=0 );

			// skip macro data
			do{
				lpMacroTop++;
			} while( *lpMacroTop!=0 );
			lpMacroTop++;
		}
	}
	return 0;
}
//----------------------------------------------------------------------------
int ExpandMacroSubSubReplace( char *lpr, char *lpmax, char *lpold, char *lpnew, int cSize )
{
//
//	lpr で始まる文字列 ( 限度 lpmax ) 内の lpold を lpnew へ置換する
//
// cSize = 文字列をどれだけインサートするか ( マイナスはデリート )
//
	BYTE ch;
	char *lprTmp,*lpoldTmp;

	lpr--;
	while( 1 ) {
		lpr++;
		if( *lpr==0 ) break;
		// 一致するか調べる
		lprTmp = lpr;
		lpoldTmp = lpold;
		do{
			if( *lpoldTmp==0 ) {
				// 一致した場合
				if( cSize>0 ) {
					// インサート
					lprTmp=lpr;
					while( *lprTmp++!=0 );
					lprTmp--;
					do {
						*(lprTmp+cSize) = *lprTmp;
						lprTmp--;
					} while( lprTmp>lpr+cSize );
				} else if( cSize<0 ) {
					// デリート
					lprTmp=lpr;
					do{
						ch = *(lprTmp-cSize);
						*lprTmp++ = ch;
					} while( ch!=0 );
				}
				// 置換
				while( 1 ){
					ch = *lpnew++;
					if( ch==0 ) break;
					*lpr++ = ch;
				}
				// 置換して終了(まだ置換する物がある可能性有り)
				return 0;
			}
		} while( *lprTmp++ == *lpoldTmp++ );
	};

	// 置換終了
	return -1;
}
//----------------------------------------------------------------------------
int ExpandMacroSubSub( char **ppr, char **ppw, char *lpmax, BYTE *lpMacroTop, BYTE* lpMacroEnd )
{
	BYTE ch;
	int r;
	char *lpr,*lpw;
	char *lprtmp;
	char *lpwBak;
	char *ReplaceOld;
	char *ReplaceNew;
	int cReplace;						// 置換する際にどれだけずらすか


	lpr=*ppr;
	lpw=*ppw;

	if( *(lpr+1)!='(' ) {
		//
		//	1 文字 macro ( replace 無し )
		//
		lpr++;
		lprtmp=lpr;
		if( *lpr=='*' && *(lpr+1)=='*' ) {
			lpr++;
		}
		lpr++;
		ch = *lpr;
		*lpr = 0;
		r = ExpandMacroSubSubSame( lprtmp, &lpw, lpmax, lpMacroTop,lpMacroEnd );
		if( r!=0 ) return r;
		*lpr = ch;
		// exit
		*ppr=lpr;
		*ppw=lpw;
		return 0;

	}


	//
	//	macro name の切り出し & ':' が無いmacroか?
	//
	lpr+=2;
	lprtmp=lpr;
	do {
		ch=*lpr++;
		if( ch==')' ) {
			*(lpr-1)=0;
			r = ExpandMacroSubSubSame( lprtmp, &lpw, lpmax, lpMacroTop,lpMacroEnd );
			if( r!=0 ) return r;
			// exit
			*ppr=lpr;
			*ppw=lpw;
			return 0;
		}
	} while ( ch!=':' );


	//
	//	replace 有り
	//
	*(lpr-1)=0;
	lpwBak=lpw;
	r = ExpandMacroSubSubSame( lprtmp, &lpw, lpmax, lpMacroTop,lpMacroEnd );
	if( r!=0 ) return r;
	*lpw=0;							// end of expanding

	ReplaceOld=lpr;				// old
	cReplace=0;
	while(1){
		ch=*lpr++;
		if( ch=='=' ) break;
		cReplace--;
		if ( ch==0 ) return 21;
	}
	if( cReplace==0 ) return 21;
	*(lpr-1)=0;

	ReplaceNew = lpr;				// new
	while(1){
		ch=*lpr++;
		if( ch==')' ) break;
		cReplace++;
		if ( ch==0 ) return 21;
	}
	*(lpr-1)=0;

	while(1){
		r = ExpandMacroSubSubReplace( lpwBak, lpmax, ReplaceOld,ReplaceNew,cReplace );
		if(r==-1) break;
		if(r!=0) return 0;
	}

	// skip
	lpwBak--;
	do{
		lpwBak++;
	} while( *lpwBak!=0 );
	// exit
	*ppr=lpr;
	*ppw=lpwBak;
	return 0;
}
//----------------------------------------------------------------------------
int ExpandMacroSub( char *lpr, char *lpw, char *lpmax, BYTE *lpMacroTop, BYTE* lpMacroEnd )
{
	int r;
	BYTE ch;

	// macro を展開する
	r = ExpandMacroSubSub( &lpr, &lpw, lpmax, lpMacroTop,lpMacroEnd );
	if( r!=0 ) return r;

	// 残りを copy
	do{
		ch = *lpr++;
		*lpw++ = ch;
		if( lpw>=lpmax ) return 6;
	} while( ch!=0 );

	return 0;				// まだ展開を続ける

}
//----------------------------------------------------------------------------
int ExpandMacroMain( char *lpr,char *lpw, int maxsize, BYTE *lpMacroTop, BYTE* lpMacroEnd )
//
//	1回のcopyの中でに1個のmacroしか展開しない。
//
//	0:まだ展開を続ける
//	-1:展開終了
//	それ以外:error
//
{
	char *lpmax;				// buffer のリミット
	char *lpmacro;				// $( へのポインター
	char ch;

	lpmax=lpw+maxsize;
	lpmacro=NULL;
	while( 1 ) {
		ch=*lpr++;
		*lpw++=ch;
		if( lpw>=lpmax ) return 6;
		if( ch==0 ) return -1;
		if( ch=='$' ) {
			ch=*lpr;
			if( (ch=='*' && *(lpr+1)=='*') || ch=='?' || (ch>='0'&&ch<='9') ) {
				if( expand_macro_flag>=2 ) {
					return ExpandMacroSub( lpr-1,lpw-1,lpmax, lpMacroTop,lpMacroEnd );
				}
			} else if( ch=='@' ||  ch=='*' ) {
				if( expand_macro_flag>=1 ) {
					return ExpandMacroSub( lpr-1,lpw-1,lpmax, lpMacroTop,lpMacroEnd );
				}
			} else if( ch=='(' ) {					// $(の形の時
				lpmacro=lpr-1;
			} else if( (ch>='A'&&ch<='Z') || (ch>='a'&&ch<='z') ) {					// $(の形の時
				return ExpandMacroSub( lpr-1,lpw-1,lpmax, lpMacroTop,lpMacroEnd );
			}
		} else if( ch==')' && lpmacro!=NULL ) {
			ch=*(lpmacro+2);
			if( (ch=='*' && *(lpmacro+3)=='*') || ch=='?' || (ch>='0'&&ch<='9') ) {
				if( expand_macro_flag>=2 ) {
					return ExpandMacroSub( lpmacro,lpw-(lpr-lpmacro),lpmax, lpMacroTop,lpMacroEnd );
				}
				lpmacro=NULL;
			} else if( ch=='@' ||  ch=='*' ) {
				if( expand_macro_flag>=1 ) {
					return ExpandMacroSub( lpmacro,lpw-(lpr-lpmacro),lpmax, lpMacroTop,lpMacroEnd );
				}
				lpmacro=NULL;
			} else {
				return ExpandMacroSub( lpmacro,lpw-(lpr-lpmacro),lpmax, lpMacroTop,lpMacroEnd );
			}
		}
	}
}
//----------------------------------------------------------------------------
int ExpandMacro( char *lpr,char *lpw, int maxsize, BYTE *lpMacroTop, BYTE* lpMacroEnd )
{
	int r,m;
	m=0;
	while( 1 ) {
		if( m==0 ) {
			r=ExpandMacroMain( lpr,lpw,maxsize, lpMacroTop,lpMacroEnd );
			m=1;
		} else {
			r=ExpandMacroMain( lpw,lpr,maxsize, lpMacroTop,lpMacroEnd );
			m=0;
		}
		if(r==-1) return 0;
		if(r!=0) return r;
	}

}
//----------------------------------------------------------------------------
int SetMakMacroname( BYTE *lpMacroTop, BYTE* lpMacroEnd, BYTE* lpr, BYTE *lpMacroname)
{
	BYTE ch;
	BYTE *lpTmp0,*lpTmp1;

	//
	//	同じ名前の macro を捜し出して無効にする
	//
	// search same name
	while( 1 ) {
		ch = *lpMacroTop;
		if( ch==0 ) break;	// 0:終了
		if( ch==2 ) {		// 2:無効の場合 skip
			goto same_next2;
		} else {			// 1:有効の場合
			lpTmp0 = lpMacroname;
			lpTmp1 = lpMacroTop;
			do {
				lpMacroTop++;
				ch = *lpTmp0++;
				if( *lpMacroTop!=ch ) goto same_next2a;
			} while( ch!=0 );
			*lpTmp1 = 2;		// 無効にする
			goto same_next3;
		}

		// skip macro name
same_next2:
		do{
			lpMacroTop++;
same_next2a:;
		} while( *lpMacroTop!=0 );

		// skip macro data
same_next3:
		do{
			lpMacroTop++;
		} while( *lpMacroTop!=0 );
		lpMacroTop++;
	}


	//
	//	set name
	//
	// Macro control code
	*lpMacroTop++=0x01;
	if( lpMacroTop>=lpMacroEnd ) return 19;
	lpTmp0=lpMacroname;
	do{
		ch=*lpTmp0++;
		*lpMacroTop++=ch;
		if( lpMacroTop>=lpMacroEnd ) return 19;
	} while( ch!=0 );


	//
	//	set_data
	//
	// skip space
	do{
		ch=*lpr++;
	} while( ch==' ' || ch==ASCII_TAB );
	// data 部分は無しか? ( name だけ )
	if( ch!=0 ) {
		lpr--;
		// set data
		do{
			ch=*lpr++;
			*lpMacroTop++=ch;
			if( lpMacroTop>=lpMacroEnd ) return 19;
		} while( ch!=0 );
		lpMacroTop--;
		// 最後の space/tab を取る
		do{
			lpMacroTop--;
			ch=*lpMacroTop;
		} while( ch==' ' || ch==ASCII_TAB );
		lpMacroTop++;
	}
	*lpMacroTop++=0;			// ターミネート
	if( lpMacroTop>=lpMacroEnd ) return 19;
	*lpMacroTop=0;				// ターミネート
	return 0;
}
//----------------------------------------------------------------------------
int MakMacroname( BYTE *lpMacroTop,BYTE* lpMacroEnd,  BYTE* lpr)
{
	BYTE ch;
	BYTE *lpMacroname;

	// skip space/tab
	do{
		ch = *lpr++;
	} while( ch==' ' || ch==ASCII_TAB );
	if( ch==0 ) return 18;
	lpr--;

	// name
	lpMacroname=lpr;							// lpMacroname = macro name
	do{
		ch=*lpr++;
	} while( isalnum(ch)!=0 );
	*(lpr-1)=0;

	// = まで space/tab を skip
	while(1){
		if( ch=='=' ) return SetMakMacroname( lpMacroTop,lpMacroEnd, lpr,lpMacroname);
		if( ch!=' ' && ch!=ASCII_TAB ) return 18;
		ch=*lpr++;
	}
}

//============================================================================
//============================================================================
//  	exec
//============================================================================
//============================================================================
//----------------------------------------------------------------------------
//	file time
//----------------------------------------------------------------------------
FILETIME GetTargetTime( BYTE **ppExec )
{
	BYTE *lpExec;
	WORD wd;
	FILETIME ftr,fttmp;

	lpExec = *ppExec;

	// target time を最新にする
	ftr.dwLowDateTime = 0xffffffff;
	ftr.dwHighDateTime = 0xffffffff;

	do{
		// get target timestamp
		fttmp = GetFileTimeFile( lpExec );
		if( fttmp.dwLowDateTime==0 && fttmp.dwHighDateTime==0 ) {
			// file が1個でも無い場合
			// 残りの target を全て skip
			do {
				wd = *((WORD*)lpExec);
				lpExec++;
			} while( wd!=0 );
			lpExec++;
			// time を最古にする
			ftr.dwLowDateTime = 0x0;
			ftr.dwHighDateTime = 0x0;
			// exit
			*ppExec = lpExec;
			return ftr;
		}
		// file がある場合、より古いタイムにする
		if( CompareFileTime(&ftr,&fttmp)>0 ) {
			ftr=fttmp;
		}
		// 次の target へ
		lpExec--;
		do {
			lpExec++;
		} while( *lpExec!=0 );
		lpExec++;

	} while( *lpExec!=0 );
	lpExec++;

	// exit
	*ppExec = lpExec;
	return ftr;
}
//----------------------------------------------------------------------------
int DependentTime( BYTE *lpDep,FILETIME ftCmp,BYTE **DependentPoint )
{
//
//	lpDep の複数のファイル名の日付を ft と比較する
//
//(out)	0:ターゲットがディペンデントと同じか古い
//		-1:ターゲットがディペンデントより新しい
//		それ以外:error
//
	int nNew;
	BYTE ch;
	WORD wd;
	FILETIME ftDep;

	nNew=0;
	do {
		ftDep = GetFileTimeFile( lpDep );
		if( ftDep.dwLowDateTime==0 && ftDep.dwHighDateTime==0 ) {
			// file が無い場合
			if( fPrint ) printf( "%s が見つかりません\n",lpDep );
			lpDep+=1+strlen( lpDep );
			if( !(option_flag&0x0002) ) return 22;
			// 残りの dependent を全て skip
			do{
				wd = *((WORD*)lpDep);
				lpDep++;
			} while( wd!=0 );
			return -1;
		} else if( CompareFileTime(&ftDep,&ftCmp)>0 ) {
			// file がより新しい場合 ( 同じ時はダメ )
			DependentPoint[nNew++] = lpDep;
		}
		// 次の depdedent へ
		do{
			ch = *lpDep++;
		} while( ch!=0 );
	} while( *lpDep!=0 );
	lpDep++;
	DependentPoint[nNew] = NULL;

	// 1 file でも新しい場合
	// 強制コンパイルの場合
	if( nNew>0 || (option_flag&0x0004) ) {
		return 0;
	}
	return -1;
}
//----------------------------------------------------------------------------
int GetDependentTime( BYTE **ppExec, FILETIME ftcmp, BYTE* lpMacroTop, BYTE* lpMacroEnd )
{
//
//	dependent の中で一番新しい物の time と ft の time を比較する
//
//(out)	0:ターゲットがディペンデントと同じか古い
//		-1:ターゲットがディペンデントより新しい
//		それ以外:error
//
	BYTE *lpDependent,*lpExec;
	char ReadLineBuff[MAX_BUFF];
	char Buff[MAX_BUFF];
	BYTE ch;
	char *lpBuff;
	int r;

	expand_macro_flag=1;
	lpExec = *ppExec;
	lpDependent = DependentBuff;

	// init dependent buffer
	*(WORD*)lpDependent = 0;

	while(1) {
		if( *lpExec==0 ) {
			lpExec++;
			// end mark
			*lpDependent++=0;
			// get file time
			r = DependentTime( DependentBuff,ftcmp,DependentPoint );
			// exit
			*ppExec = lpExec;
			return r;
		}

		//
		//	1 file の名前を転送し、マクロ展開する
		//
		// 1 line 書き込み
		lpBuff=ReadLineBuff;
		do {
			ch=*lpExec++;
			*lpBuff++=ch;
		} while( ch!=0 );
		// macro 展開
		r=ExpandMacro( ReadLineBuff,Buff,MAX_BUFF,lpMacroTop,lpMacroEnd );
		if(r!=0) return r;

		//
		//	1 file の名前を転送する
		//
		lpBuff=ReadLineBuff;
		do{
			// skip space/tab
			do {
				ch=*lpBuff++;
			} while( ch==' ' || ch==ASCII_TAB );
			lpBuff--;
			// filename の copy
			do {
				ch=*lpBuff++;
				*lpDependent++=ch;
			} while( ch>' ' );
			*(lpDependent-1)=0;
		} while( ch!=0 );
	}
}
//----------------------------------------------------------------------------
//	command と option の分割
//----------------------------------------------------------------------------
int isfilename( BYTE ch )
{
	if( ch<=' ' ) return 0;
	if( ch=='*' ) return 0;
	if( ch=='/' ) return 0;
	if( ch=='<' ) return 0;
	if( ch=='>' ) return 0;
	if( ch=='?' ) return 0;
	return 1;

}
//----------------------------------------------------------------------------
BYTE* CheckRedirectPipe( BYTE *lpPath )
//
//	リダイレクトのチェック
//	>,>>,< 以外のリダイレクトの場合( | )、carry on
//
{
	while( *lpPath!=0 ) {
		if( *lpPath=='|' ) return lpPath;
		lpPath++;
	}
	return NULL;
}
//----------------------------------------------------------------------------
BYTE* SearchCharSpace( BYTE *lpPath, BYTE ch )
//
//	space,0x0 等で終了する文字列の中に ch があるか調べる。
//
//	0以外(真) : ある ( 最初の ch を指すポインター )
//	0(偽)     : 無い
//
{
	while( isfilename(*lpPath) ) {
		if( *lpPath==ch ) return lpPath;
		lpPath++;
	}
	return NULL;
}
//----------------------------------------------------------------------------
BYTE* SearchExtSpace( BYTE *lpPath )
//
//	space,0x0 等で終了する文字列の中に拡張子があるか調べる。
//
//	0以外(真) : ある ( 最初の '.' を指すポインター、'.' は複数ある可能性があるので、最初の '.' が拡張子とは限らない。最後の'.'が拡張子である )
//	0(偽)     : 無い
//
{
	while( isfilename(*lpPath) ) {
		if( *lpPath=='.' ) return lpPath;
		lpPath++;
	}
	return NULL;
}
//----------------------------------------------------------------------------
BYTE* strcpySpace( BYTE* lpw,BYTE* lpr )
//
//	space,0x0 等で終了する文字列を copy する。
//
//	0以外(真) : ある ( lpw の最後の0を指すポインター )
//	0(偽)     : 無い
//
{
	while( isfilename(*lpr) ) {
		*lpw++=*lpr++;
	}
	*lpw=0;
	return lpw;
}
//----------------------------------------------------------------------------
void optcopy( BYTE* lpbuff1, BYTE* lpPath )
//
//	option 部分をcopyする
//
{
	while( isfilename(*lpPath) ) lpPath++;
	strcpy( lpbuff1,lpPath );
}
//----------------------------------------------------------------------------
//	main
//----------------------------------------------------------------------------
int SetCommandLineCommand( BYTE* lpPath,BYTE* lpbuff0,BYTE* lpbuff1 )
//
//	command.com のコマンドか ? command.com 経由にする
//
{
	BYTE *lptmp;

	// comspec の転送
	lptmp = getenv( "COMSPEC" );
	if( lptmp==NULL ) return 24;
	strcpy( lpbuff0,lptmp );

	// command / cmd の設定
	if(( GetVersion() & 0x80000000)==0 ) {				// Windows NT/2000
		strcpy( lpbuff1,"cmd /c " );
	} else {
		GetWindowsDirectory( szCommand0, _MAX_PATH); 
		strcpy( lpbuff1,"command /c " );
	}

	strcat( lpbuff1,lpPath );
	return 0;
}
//----------------------------------------------------------------------------
int SetCommandLineSub( BYTE *lpPath, BYTE *szFile0, BYTE *lpFile )
{
	BYTE *lpExt;

	// 拡張子があるか?
	if( SearchExtSpace( lpPath ) ){
		// 拡張子有り
		strcpySpace( lpFile,lpPath );
		if( ExistFile( szFile0 ) ) return 1;
	} else {
		// 拡張子無し
		lpExt = strcpySpace( lpFile,lpPath );
		// .com
		strcpy( lpExt,".com" );
		if( ExistFile( szFile0 ) ) return 1;
		// .exe
		strcpy( lpExt,".exe" );
		if( ExistFile( szFile0 ) ) return 1;
		// .bat
		strcpy( lpExt,".bat" );
		if( ExistFile( szFile0 ) ) return 1;
	}
	return 0;
}
//----------------------------------------------------------------------------
BYTE* AddYenMark( BYTE* lppath )
{
	BYTE ch;

	while( 1 ) {
		ch = *lppath++;
		if( _ismbblead(ch) ) {
			// 漢字
			if( _ismbbtrail(*lppath) ) lppath++;
		} else if( ch==0 ) {
			*(lppath-1)='\\';
			*lppath=0;
			return lppath;
		} else if( ch=='\\' && (*lppath)==0 ) {
			return lppath;
		}
	}
}
//----------------------------------------------------------------------------
int SearchExt( BYTE *lpFile, BYTE *lpExt )
//
//	拡張子を調べる
//
{
	BYTE *lptmp;

	// 最後の '.' を見つける
	lptmp=NULL;
	while( *lpFile!=0 ) {
		if( *lpFile=='.' ) lptmp=lpFile;
		lpFile++;
	}

	if( lptmp==NULL ) return 0;
	if( stricmp( lptmp,lpExt ) )return 0;

	return 1;
}
//----------------------------------------------------------------------------
int SetCommandLine( BYTE *lpPath, BYTE *lpbuff0, BYTE *lpbuff1, int *pfTool )
//
//	command line を解析する
//
{
	int f0,f1;
	BYTE szFile0[_MAX_PATH];
	BYTE *lpFile;
	BYTE *lpEnv;
	BYTE *lptmp;

	//	登録されたツールか?
	*pfTool=0;
	if(	makemode==1 ) {
		// いったん、szFile0 へ copy する
		strcpySpace( szFile0,lpPath );
		if( stricmp( szFile0,szTool0 )==0 ||  stricmp( szFile0,szTool1 )==0 ) {
//			if( CheckRedirectPipe( lpPath )==NULL ) {
				// '|' がない場合
				strcpy( lpbuff0,szFile0 );
				// オプション転送
				optcopy( lpbuff1, lpPath );
				*pfTool=1;
				return 0;
//			}
		}
	}


	//
	//	まず、fullpath の場合
	//	拡張子が無い場合、.com -> .exe -> .bat の順で探す
	//	探せない場合、実行できない
	//
	// fullpath か ?
	if( SearchCharSpace( lpPath,'\\' ) ) {
		f0 = SetCommandLineSub( lpPath,szFile0,szFile0 );
	} else {
		//
		//	カレント- > path を探す
		//	拡張子が無い場合、.com -> .exe -> .bat の順で探す
		//	無い場合には、次の処理へ
		//
		lpFile = szFile0;
		lpEnv = getenv( "PATH" );
		while( 1 ) {
			f0 = SetCommandLineSub( lpPath,szFile0,lpFile );
			if( f0==1 ) break;
			// path 名をcopy
			if( *lpEnv==0 ) break;
			lptmp = szFile0;
			while(1) {
				if( *lpEnv==';' ) {
					lpEnv++;
					*lptmp=0;
					break;
				} else if( *lpEnv==0 ) {
					*lptmp=0;
					break;
				}
				*lptmp++ = *lpEnv++;
			}
			lpFile = AddYenMark(szFile0);
		};
	}


	// '|'リダイレクトがあるか? ある場合、command.com 経由にする
//	if( f0==1 && CheckRedirectPipe( lpPath )==NULL ) {
	if( f0==1 ) {
		// '|' がない場合
		f1 = 0;
		// .com
		if( SearchExt( szFile0,".com" ) ){
			f1 = 1;
		} else {
			// .exe
			if( SearchExt( szFile0,".exe" ) ) f1 = 1;
		}
		if( f1 ) {
			strcpy( lpbuff0,szFile0 );
			// オプション転送
			optcopy( lpbuff1, lpPath );
			return 0;
		}
		// .com/.exe 以外の時は、.bat か command のコマンドだとみなす
	}
	return SetCommandLineCommand( lpPath,lpbuff0,lpbuff1 );
}
//----------------------------------------------------------------------------
int SetRedirect( BYTE *lpr, BYTE *lpin, BYTE *lpout, BYTE **ppNext  )
{
	BYTE ch;
	BYTE *lpPack,*lpBackup,*lpw;
	BYTE szTmpFile[_MAX_PATH],szTmpPath[_MAX_PATH];


	mRedirectOut = 0;
	mRedirectIn = 0;
	mRedirectPipeOut = 0;

	do{
		ch=*lpr++;
		if( ch=='>' ) {
			if( mRedirectOut!=0 ) return 29;
			// outfile
			//
			//	リダイレクト出力の filename の設定
			//
			lpBackup=lpPack=lpr-1;
			if( *lpr=='>' ) {
				mRedirectOut = 2;
				lpr++;
			} else {
				mRedirectOut = 1;
			}
			lpw = lpout;
			// skip space/tab
			do{
				ch=*lpr++;
			} while( ch==' ' || ch==ASCII_TAB );
			if( ch==0 ) return 27;
			goto setfilename;
		} else if( ch=='<' ) {
			// infile
			//
			//	リダイレクト入力の filename の設定
			//
			if( mRedirectIn!=0 ) return 30;
			if( mRedirectPipeIn!=0 ) return 30;
			mRedirectIn = 1;
			lpBackup=lpPack=lpr-1;
			lpw = lpin;
			// skip space/tab
			do{
				ch=*lpr++;
			} while( ch==' ' || ch==ASCII_TAB );
			if( ch==0 ) return 28;
setfilename:
			// copy file name
			do{
				*lpw++=ch;
				ch=*lpr++;
			} while( ch>' ' );
			lpr--;
			*lpw++=0;
			// パラメーター文字列を詰める
			do{
				ch=*lpr++;
				*lpPack++=ch;
			} while( ch!=0 );
			lpr=lpBackup;
			ch=1;							// 次の while ですぐに終了しないため
		} else if( ch=='|' ) {
			if( mRedirectOut!=0 ) return 29;
			// outfile
			//
			//	パイプ出力
			//
			mRedirectPipeOut = 1;
			*(lpr-1)=0;						// パイプを 0 にする
			do{
				ch=*lpr++;
			} while(ch==' ' || ch==ASCII_TAB );
			if( ch==0 ) return 31;
			lpr--;
			*ppNext=lpr;					// パイプ処理の次の命令を示す
			break;
		}
	} while( ch!=0 );


	//
	//	in
	//
	if( mRedirectIn!=0 ) {
		fflush( stdin );
		handle_in_old = dup(_fileno(stdin));			// 標準入力を二重化
		if( handle_in_old==-1 ) return 27;
		handle_in_new = _open( lpin,0,S_IREAD | S_IWRITE );
		if( handle_in_new == -1 ) return 27;
		if( _dup2(handle_in_new,_fileno(stdin))==-1 ) return 27;
	}


	//
	//	out
	//
	if( mRedirectOut!=0 ) {
		// setvbuf で 0 にしているので fflush と _commit は念のために入れている。
		fflush( stdout );
		handle_out_old = dup(_fileno(stdout));			// 標準出力を二重化
		if( handle_out_old==-1 ) return 28;
		// '>>' か?
		if( mRedirectOut==2 ) {
			GetTempPath( _MAX_PATH,szTmpPath );
			GetTempFileName( szTmpPath,"SMK",0,szTmpFile );
			handle_out_new = _open( szTmpFile,_O_RDWR|_O_CREAT|_O_TRUNC|_O_TEMPORARY,_S_IREAD | _S_IWRITE );
		} else {
			handle_out_new = _open( lpout,_O_RDWR|_O_CREAT|_O_TRUNC,S_IREAD | S_IWRITE );
		}
		if( handle_out_new == -1 ) return 28;
		if( _dup2(handle_out_new,_fileno(stdout))==-1 ) return 28;
	}


	//
	//	パイプ out
	//
	if( mRedirectPipeOut!=0 ) {
		// setvbuf で 0 にしているので fflush と _commit は念のために入れている。
		fflush( stdout );
		handle_out_old = dup(_fileno(stdout));			// 標準出力を二重化
		if( handle_out_old==-1 ) return 28;
		GetTempPath( _MAX_PATH,szTmpPath );
		GetTempFileName( szTmpPath,"SMK",0,szPipeOut );
		handle_out_new = _open( szPipeOut,_O_RDWR|_O_CREAT|_O_TRUNC,_S_IREAD | _S_IWRITE );
		if( handle_out_new == -1 ) {
			szPipeOut[0]=0;
			return 28;
		}
		if( _dup2(handle_out_new,_fileno(stdout))==-1 ) return 28;
	}


	//
	//	パイプ in
	//
	if( mRedirectPipeIn!=0 ) {
		fflush( stdin );
		handle_in_old = dup(_fileno(stdin));			// 標準入力を二重化
		if( handle_in_old==-1 ) return 27;
		handle_in_new = _open( szPipeIn,0,S_IREAD | S_IWRITE );
		if( handle_in_new == -1 ) return 27;
		if( _dup2(handle_in_new,_fileno(stdin))==-1 ) return 27;
	}

	return 0;
}
//----------------------------------------------------------------------------
int ResetRedirect( BYTE *lpOut )
{
	int r;

	if( mRedirectIn!=0 ) {
		// setvbuf で 0 にしているので fflush と _commit は念のために入れている。
		_commit( handle_in_old );
		_commit( handle_in_new );
		if( _dup2(handle_in_old,_fileno(stdin))==-1 ) return 27;
		_close( handle_in_old );
		handle_in_old=-1;
		_close( handle_in_new );
		handle_in_new=-1;
	}


	if( mRedirectOut!=0 ) {
		// setvbuf で 0 にしているので fflush と _commit は念のために入れている。
		fflush(stdout);
		_commit( handle_out_old );
		_commit( handle_out_new );
		if( _dup2(handle_out_old,_fileno(stdout))==-1 ) return 28;
		r=0;
		if( mRedirectOut==2 ) {
			r = CopyFileHandle( handle_out_new,lpOut );
		}
		_close( handle_out_old );
		handle_out_old=-1;
		_close( handle_out_new );
		handle_out_new=-1;
		if(r!=0) return r;
	}

	return 0;
}

//----------------------------------------------------------------------------
int ToolExec( BYTE *lpCmd, BYTE *lpPara )
//
//	tool に登録されている物を実行する
//
{
	BYTE ch;
	int f,c;
	BYTE buff0[MAX_BUFF],*lpw,*lpr;
	int argv;
	BYTE *argc[MAX_ARGC];

	// cmd
	lpw=buff0;
	argc[0] = lpw;
	strcpy( lpw,lpCmd );
	lpw+=1+strlen(lpw);
	argv=1;

	// para
	lpr=lpPara;
	while( *lpr!=0 ){
		// skip space/tab
		do{
			ch=*lpr++;
		}while(ch==' '|| ch==ASCII_TAB );
		if( ch==0 ) break;
		lpr--;
		// trans para
		argc[argv++]=lpw;
		f=0;
		while( 1 ) {
			ch=*lpr++;
			if( ch=='"' ) {
				f=~f;
			} else if( ch=='\\' ) {
				c=1;
				// yen マークの個数を数える
				while( *lpr=='\\' ){
					c++;
					lpr++;
				}
				if( *lpr=='"' ) {
					if( c&1 ) {
						*lpw++='"';
						lpr++;
					}
					c>>=1;
				}
				// yen マークの書き込み
				while( c>0 ) {
					*lpw++='\\';
					c--;
				}
			} else if( f==0 && (ch==' ' || ch==ASCII_TAB) ) {
				break;
			} else if( _ismbblead(ch) ) {
				// 漢字
				*lpw++=ch;
				if( _ismbbtrail(*lpr) ) *lpw++=*lpr++;
			} else if( ch==0 ) {
				break;
			} else {
				*lpw++=ch;
			}
		}
		lpr--;
		*lpw++=0;
	}

	return makecall( argv, argc );
}
//----------------------------------------------------------------------------
int ExecSub( BYTE *lpPath )
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	char buff0[MAX_BUFF],buff1[MAX_BUFF];
	BYTE *lpTar;
	int exitcode,execerror;
	int r;
	BYTE szRedirectIn[_MAX_PATH],szRedirectOut[_MAX_PATH];
	int fTool;

	mRedirectPipeIn=0;

startexec:
	r=SetCommandLine( lpPath,buff0,buff1,&fTool );
	if(r!=0) return r;

	// 表示
	printf( "\t%s%s\n",buff0,buff1 );

	// リダイレクトの処理
	r=SetRedirect( buff1,szRedirectIn,szRedirectOut,&lpPath );
	if(r!=0) return r;

	if( fTool==0 ) {
		// exec
		//ZeroMemory( &si, sizeof( STARTUPINFO ) );
		GetStartupInfo( &si );
		ZeroMemory( &pi, sizeof( PROCESS_INFORMATION ) );
		si.cb			= sizeof( STARTUPINFO );
		si.lpDesktop	= NULL;
		si.lpTitle		= NULL;
		si.dwX			= 0;
		si.dwY			= 0;
		si.dwXSize		= 0;
		si.dwYSize		= 0;
		si.dwFlags		|= STARTF_USESTDHANDLES;
		si.hStdInput	= GetStdHandle(STD_INPUT_HANDLE);
		si.hStdOutput	= GetStdHandle(STD_OUTPUT_HANDLE);
		si.hStdError	= GetStdHandle(STD_ERROR_HANDLE);
		si.wShowWindow	= SW_SHOWNORMAL;	// 起動時のウインドウタイプ
		pi.hProcess		= NULL;
		execerror=1;
		exitcode=0;

		r= CreateProcess( buff0, buff1, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi );

		CloseHandle( pi.hThread );
	    // プロセスが終了するまでまつ
	    // この関数についてはヘルプをみればすぐわかる
	    WaitForSingleObject( pi.hProcess, INFINITE );

		if( r!=0 ) {
			do{
				GetExitCodeProcess( pi.hProcess,&exitcode );
			} while( exitcode==STILL_ACTIVE );
			execerror=0;
		}


		CloseHandle( pi.hProcess );
	} else {
		// argv,argcを設定して、ツールを呼ぶ
		execerror=0;
		exitcode = ToolExec( buff0,buff1 );
	}


	//	リダイレクトの後処理
	r = ResetRedirect( szRedirectOut );
	if(r!=0) return r;


	//
	//	error check
	//
	if( (exitcode+execerror)!=0 ) {
		// target files の削除
		if( !(option_flag&0x0010) ) {
			lpTar = TargetPoint;
			do {
				DeleteFile( lpTar );
				// printf( "delete %s\n",lpTar );
				while( *lpTar++!=0 );
			} while( *lpTar!=0 );
		}
		// error表示 & 終了
		if( !(option_flag&0x0002) ) {
			if( execerror!=0 ) {
				printf( "execerror / %s%s\n", buff0,buff1 );
				return 25;
			} else {
				printf( "errorlevel=%d / %s%s\n",exitcode, buff0,buff1 );
				return 26;
			}
		}
	}

	if( mRedirectPipeIn!=0 ) {
		// setvbuf で 0 にしているので fflush と _commit は念のために入れている。
		_commit( handle_in_old );
		_commit( handle_in_new );
		if( _dup2(handle_in_old,_fileno(stdin))==-1 ) return 27;
		_close( handle_in_old );
		handle_in_old=-1;
		_close( handle_in_new );
		handle_in_new=-1;
		remove( szPipeIn );
		szPipeIn[0]=0;
		mRedirectPipeIn=0;
	}

	if( mRedirectPipeOut!=0 ) {
		mRedirectPipeOut=0;
		// setvbuf で 0 にしているので fflush と _commit は念のために入れている。
		fflush(stdout);
		_commit( handle_out_old );
		_commit( handle_out_new );
		if( _dup2(handle_out_old,_fileno(stdout))==-1 ) return 28;
		_close( handle_out_old );
		handle_out_old=-1;
		_close( handle_out_new );
		handle_out_new=-1;
		strcpy( szPipeIn,szPipeOut );
		szPipeOut[0]=0;
		mRedirectPipeIn=1;
		goto startexec;
	}

	return 0;
}
//----------------------------------------------------------------------------
int ExecMain( BYTE **ppExec,BYTE *lpRule, BYTE *lpMacroTop, BYTE* lpMacroEnd )
{
	BYTE *lpExec,*lpb,*lpe,*lpTar,*lpDep,*lpTmp;
	char ReadLineBuff[MAX_BUFF];
	char Buff[MAX_BUFF];
	char ch;
	int r;

	lpExec = *ppExec;
	expand_macro_flag = 2;
	if( *(WORD*)lpExec!=0x00ff ) {
		// ffh,00h ( 暗黙 rule 適用 ) じゃない場合
		while( *lpExec!=0x00 ) {
			lpb=ReadLineBuff;
			lpe=ReadLineBuff+MAX_BUFF;
			while( 1 ) {
				ch=*lpExec++;
				*lpb++=ch;
				if( lpb>=lpe ) return 12;
				if( ch==0 ) break;
			}
			r=ExpandMacro( ReadLineBuff,Buff,MAX_BUFF,lpMacroTop,lpMacroEnd );
			if(r!=0) return r;
			r=ExecSub( ReadLineBuff );
			if(r!=0) return r;
		}
		lpExec++;
		*ppExec = lpExec;
		return 0;
	} else {
		// ffh,00h ( 暗黙 rule 適用 ) の場合
		lpExec+=2;

		// target の拡張子を拾得
		lpTmp = TargetPoint;
		lpTar = NULL;
		while( *lpTmp!=0 ) {
			if( *lpTmp=='.' ) lpTar = lpTmp;
			lpTmp++;
		}
		if( lpTar==NULL ) return 23;
		if( *(lpTar+1)=='\\' ) return 23;			// ".\" や "..\" の場合には erro

		// dependent の拡張子を拾得
		lpTmp = DependentBuff;
		lpDep = NULL;
		while( *lpTmp!=0 ) {
			if( *lpTmp=='.' ) lpDep = lpTmp;
			lpTmp++;
		}
		if( lpDep==NULL ) return 23;
		if( *(lpDep+1)=='\\' ) return 23;			// ".\" や "..\" の場合には erro

		while( 1 ) {
			ch = *lpRule;
			if( ch==0 ) return 23;				// 0:終了
			if( ch!=2 ) {
				lpRule++;
				// dependent の拡張子 と dependent_ext が一致するか
				lpTmp=lpDep;
				do{
					ch = *lpTmp;
					if( *lpRule!=ch ) goto skip_rule;
					lpTmp++;
					lpRule++;
				} while( ch!=0 );
				// target の拡張子 と target_ext が一致するか
				lpTmp=lpTar;
				do{
					ch = *lpTmp;
					if( *lpRule!=ch ) goto skip_rule;
					lpTmp++;
					lpRule++;
				} while( ch!=0 );
				// exec
				while( *lpRule!=0x00 ) {
					lpb=ReadLineBuff;
					lpe=ReadLineBuff+MAX_BUFF;
					while( 1 ) {
						ch=*lpRule++;
						*lpb++=ch;
						if( lpb>=lpe ) return 12;
						if( ch==0 ) break;
					}
					r=ExpandMacro( ReadLineBuff,Buff,MAX_BUFF,lpMacroTop,lpMacroEnd );
					if(r!=0) return r;
					r=ExecSub( ReadLineBuff );
					if(r!=0) return r;
				}
				*ppExec = lpExec;
				return 0;
			}
			// skip rule
			lpRule--;
			do {
				lpRule++;
skip_rule:;
			} while( *(WORD*)lpRule!=0 );
			lpRule+=2;
		}
		*ppExec = lpExec;
		return 0;
	}

}
//----------------------------------------------------------------------------
int MainMakLoop( BYTE *lpExec,BYTE *lpRule, BYTE *lpMacro, BYTE *lpMacroEnd )
{
//[exec_data]
//	db 1		; control byte
//	dw 0		; コマンド/ディテクティブオプション
//
//	db "target",0	; ターゲット
//	db 0		; end mark
//
//	db "datefile",0	; 比較対象
//	db "datefile",0
//	db "datefile",0
//	db "datefile",0
//	db 0		; end mark
//
//	db "command",0	; コマンド
//	db "command",0
//	db "command",0
//	db 0		; end mark
//
//	db 3		; control byte
//	db "command",0	; コマンド
//	db 0		; end mark
//
	int r;
	WORD wd;
	FILETIME ft;

	while( 1 ) {
		switch( *lpExec++ ){
			case 1:			// control byte = command
				// get option
				option_flag = *((WORD*)lpExec);
				lpExec+=2;
				TargetPoint = lpExec;						// macro 展開のために target へのポインターを保持する
				ft = GetTargetTime( &lpExec );
				r = GetDependentTime( &lpExec,ft,lpMacro,lpMacroEnd );
				if( r==-1 ){
					// skip
					if( *((WORD*)lpExec) == 0x00ff ){
						// ffh,00h ( 暗黙 rule 適用 )
						lpExec+=2;
					} else {
						// command を skip
						do {
							wd = *(WORD*)lpExec;
							lpExec++;
						} while( wd!=0 );
						lpExec++;
					}
					break;
				}
				if( r!=0 ) return r;
				// break;						// case 3 へ進む
			case 3:			// control byte = commandonly
				r = ExecMain( &lpExec,lpRule,lpMacro,lpMacroEnd );
				if( r!=0 ) return r;
				break;
			default:		// control byte = 終了
				return 0;
		}
	}


}
//============================================================================
//============================================================================
//  	read
//============================================================================
//============================================================================
//----------------------------------------------------------------------------
//	target
//----------------------------------------------------------------------------
BYTE* MakTarget( BYTE* lpExec, BYTE* lpExecEnd, BYTE* lpr )
{
	BYTE ch;
	BYTE *lpDel;
	int f;

	// control code
	if(lpExec>=lpExecEnd-3) return NULL;
	*lpExec++=0x01;

	// コマンド/ディテクティブオプションを記録
	*((WORD*)lpExec)=option_flag;
	lpExec+=2;

	//
	//	target の書き込み
	//
	f=0;
	while( 1 ) {
		// skip space/tab
		do{
			ch = *lpr++;
		} while( ch==' ' || ch==ASCII_TAB );
		if(ch==':') break;

		lpDel = lpExec;
		while( 1 ) {
			if( ch==' ' || ch==ASCII_TAB ) {
				*lpExec++=0x00;
				if(lpExec>=lpExecEnd) return NULL;
				if( option_flag&0x0020 ) {
					DeleteFile( lpDel );
				}
				break;
			} else if( ch==':' && ( *lpr==' ' || *lpr==ASCII_TAB )) {
				lpr++;
				*lpExec++=0x00;
				if(lpExec>=lpExecEnd) return NULL;
				if( option_flag&0x0020 ) {
					DeleteFile( lpDel );
				}
				goto target_next4;
			}
			f=1;
			*lpExec++ = ch;
			if(lpExec>=lpExecEnd) return NULL;
			ch = *lpr++;
		}
	}
target_next4:
	if( f!=1 ) return (BYTE*)1;
	*lpExec++=0x00;
	if(lpExec>=lpExecEnd) return NULL;


	//
	//	dependent
	//
	while( 1 ) {
		// skip space/tab
		do{
			ch = *lpr++;
		} while( ch==' ' || ch==ASCII_TAB );
		if(ch==0) break;

		f=2;
		while( 1 ) {
			*lpExec++=ch;
			if(lpExec>=lpExecEnd) return NULL;
			ch=*lpr++;
			if( ch==0 ) {
				*lpExec++=0x00;
				if(lpExec>=lpExecEnd) return NULL;
				goto dependent_next4;
			} else if( ch==' ' || ch==ASCII_TAB ) {
				*lpExec++=0x00;
				if(lpExec>=lpExecEnd) return NULL;
				break;
			}
		}
	}
dependent_next4:
	if( f!=2 ) return (BYTE*)2;
	*lpExec++=0x00;
	if(lpExec>=lpExecEnd) return NULL;

	return lpExec;
}
//----------------------------------------------------------------------------
//	command
//----------------------------------------------------------------------------
BYTE* MakCommand( BYTE* lpExec, BYTE* lpExecEnd, BYTE* lpr,int b)
{
	BYTE ch;

	// skip space/tab
	do{
		ch = *lpr++;
	} while( ch==' ' || ch==ASCII_TAB );

	// command の書き込み
	while( 1 ) {
		*lpExec++ = ch;
		if(lpExec>=lpExecEnd) return NULL;
		if( ch==0 ) return lpExec;
		ch = *lpr++;
	}
}
//----------------------------------------------------------------------------
BYTE* MakCommandEnd( BYTE* lpExec, BYTE* lpExecEnd, int b)
//
//	end mark を書き込む
//
{
	switch( b ) {
		case 2:					// 以前が command
			*lpExec++=0x00;
			if(lpExec>=lpExecEnd) return NULL;
			break;
		case 1:					// 以前が target
			*lpExec++=0xff;		// ffh,00h ( 暗黙 rule 適用 )
			if(lpExec>=lpExecEnd) return NULL;
			*lpExec++=0x00;
			if(lpExec>=lpExecEnd) return NULL;
			break;
	}
	return lpExec;
}
//----------------------------------------------------------------------------
BYTE* MakCommandEndEnd( BYTE* lpExec, BYTE* lpExecEnd, int b)
//
//	end mark を書き込む
//
{
	*lpExec++=0x00;
	if(lpExec>=lpExecEnd) return NULL;
	return lpExec;
}
//----------------------------------------------------------------------------
//	rule
//----------------------------------------------------------------------------
BYTE* MakRule( BYTE* lpRule, BYTE* lpRuleEnd, BYTE *lpRuleTop, BYTE* lpr)
{
//lpRuleNow,lpBx
//lpRuleTmp,lpDi
//lpDel,lpBp
//lpCmp,lpSi

	BYTE ch;
	BYTE *lpBx,*lpDi,*lpBp,*lpSi;

	// rule control code
	*lpRule++=0x01;
	if( lpRule>=lpRuleEnd ) return (BYTE*)2;


	//
	//	rule の書き込み
	//
	// skip space/tab
	do{
		ch = *lpr++;
	} while( ch==' ' || ch==ASCII_TAB );

	// .dependent_ext
	if( ch!='.' ) return (BYTE*)1;	// 1 byte 目
	lpBx=lpRule;						// dependent_Ext
	do{
		*lpRule++=ch;
		if( lpRule>=lpRuleEnd ) return (BYTE*)2;
		ch=*lpr++;
		if( ch==0 ) return (BYTE*)1;
		if( ch<=' ' ) {
			// skip space/tab
			do{
				ch = *lpr++;
			} while( ch==' ' || ch==ASCII_TAB );
			if( ch!='.' ) return (BYTE*)1;
			break;
		}
	} while	( ch!='.' );

	// .dependent_ext end
	*lpRule++=0;
	if( lpRule>=lpRuleEnd ) return (BYTE*)2;
	// ch='.'						// 直前のwhileの終了条件が ch=='.' なので必要ない
	do{
		*lpRule++=ch;
		if( lpRule>=lpRuleEnd ) return (BYTE*)2;
		ch = *lpr++;
		if( ch==0 ) return (BYTE*)1;
		if( ch<=' ' ) {
			// skip space/tab
			do{
				ch = *lpr++;
			} while( ch==' ' || ch==ASCII_TAB );
			if( ch!=':' ) return (BYTE*)1;
			break;
		}
	} while	( ch!=':' );

	// .dependent_ext end
	*lpRule++=0x00;
	if( lpRule>=lpRuleEnd ) return (BYTE*)2;

	// skip space/tab
	do{
		ch = *lpr++;
	} while( ch==' ' || ch==ASCII_TAB );
	if( ch!=0 ) return (BYTE*)1;


	//
	//	同じ rule を無効にする
	//
	lpDi=lpRuleTop;
//#check_loop3:
	while(1){
		lpBp=lpDi;
		ch=*lpDi;
		if( ch==0 ) return lpRule;
		if( ch!=2 ) {
			lpDi++;
			if( lpDi>=lpBx ) return lpRule;
			// check dependent+target
			lpSi = lpBx;
			// dependent
//#check_loop0:				; dependent
			while(1){
				ch=*lpSi;
				if( *lpDi!=ch ) break;
				lpSi++;
				lpDi++;
				if( ch==0 ) {
//#check_loop1:				; target
					while(1){
						// target
						ch=*lpSi;
						if( *lpDi!=ch ) break;
						lpSi++;
						lpDi++;
						if( ch==0 ) {
							// 無効にする
							*lpBp=0x02;
							break;
						}
					}
					break;
				}
			}
		}

//#check_skip:
		// skip
		lpDi--;
		do{
			lpDi++;
		}while(*lpDi!=0);
		lpDi+=2;
	}


}
//----------------------------------------------------------------------------
BYTE* MakRulecommand( BYTE* lpRule, BYTE* lpRuleEnd, BYTE* lpr)
{
	BYTE ch;

	// skip space/tab
	do{
		ch = *lpr++;
	} while( ch==' ' || ch==ASCII_TAB );

	// command の書き込み
	while( 1 ) {
		*lpRule++ = ch;
		if( lpRule>=lpRuleEnd ) return NULL;
		if( ch==0 ) return lpRule;
		ch = *lpr++;
	}
}
//-----------------------------------------------------------------------------
BYTE* MakRuleEnd( BYTE* lpRule, BYTE* lpRuleEnd, int b)
//
//	end mark を書き込む
//
{
	switch( b ) {
		case 5:					// 以前が暗黙のコマンド行
			*lpRule++=0x00;
			if( lpRule>=lpRuleEnd ) return NULL;
			break;
		case 4:					// 暗黙のコマンド行定義
			return (BYTE*)1;
	}
	return lpRule;
}
//-----------------------------------------------------------------------------
BYTE* MakRuleEndEnd( BYTE* lpRule, BYTE* lpRuleEnd, int b)
//
//	end mark を書き込む
//
{
	*lpRule++=0x00;
	if( lpRule>=lpRuleEnd ) return NULL;
	return lpRule;

}
//----------------------------------------------------------------------------
//	command only 行
//----------------------------------------------------------------------------
BYTE* MakCommandonly( BYTE* lpExec, BYTE* lpExecEnd, BYTE* lpr,int b)
{
	BYTE ch;

	if( *lpr++!=':' ) return (BYTE*)1;

	// control code
	*lpExec++=0x03;
	if(lpExec>=lpExecEnd) return NULL;

	// command の書き込み
	while( 1 ) {
		ch = *lpr++;
		*lpExec++ = ch;
		if(lpExec>=lpExecEnd) return NULL;
		if( ch==0 ) {
			// end mark
			*lpExec++ = 0;
			if(lpExec>=lpExecEnd) return NULL;
			return lpExec;
		}
	}
}
//----------------------------------------------------------------------------
//	detective
//----------------------------------------------------------------------------
int MakDetective( BYTE* lpr )
{
	BYTE ch;

	if( *lpr++!='.' ) return 15;

	if( *lpr=='i' && *(lpr+1)=='+' ) {
		option_flag|=0x0002;
		lpr+=2;
	} else if( *lpr=='i' && *(lpr+1)=='-' ) {
		option_flag&=0xfffd;
		lpr+=2;
	} else if( *lpr=='i' ) {
		option_flag|=0x0002;
		lpr++;
	}

	else if( *lpr=='S' && *(lpr+1)=='+' ) {
		option_flag|=0x0001;
		lpr+=2;
	} else if( *lpr=='S' && *(lpr+1)=='-' ) {
		option_flag&=0xfffe;
		lpr+=2;
	} else if( *lpr=='S' ) {
		option_flag|=0x0001;
		lpr++;
	}

	else if( *lpr=='B' && *(lpr+1)=='+' ) {
		option_flag|=0x0004;
		lpr+=2;
	} else if( *lpr=='B' && *(lpr+1)=='-' ) {
		option_flag&=0xfffb;
		lpr+=2;
	} else if( *lpr=='B' ) {
		option_flag|=0x0004;
		lpr++;
	}

	else if( *lpr=='e' && *(lpr+1)=='+' ) {
		option_flag|=0x0008;
		lpr+=2;
	} else if( *lpr=='e' && *(lpr+1)=='-' ) {
		option_flag&=0xfff7;
		lpr+=2;
	} else if( *lpr=='e' ) {
		option_flag|=0x0008;
		lpr++;
	}

	else if( *lpr=='N' && *(lpr+1)=='+' ) {
		option_flag|=0x0010;
		lpr+=2;
	} else if( *lpr=='N' && *(lpr+1)=='-' ) {
		option_flag&=0xffef;
		lpr+=2;
	} else if( *lpr=='N' ) {
		option_flag|=0x0010;
		lpr++;
	}

	else if( *lpr=='A' && *(lpr+1)=='+' ) {
		option_flag|=0x0020;
		lpr+=2;
	} else if( *lpr=='A' && *(lpr+1)=='-' ) {
		option_flag&=0xffdf;
		lpr+=2;
	} else if( *lpr=='A' ) {
		option_flag|=0x0020;
		lpr++;
	}

	do{
		ch = *lpr++;
		if(ch==0) return 0;
	} while( ch==' ' || ch==ASCII_TAB );

	return 15;
}
//----------------------------------------------------------------------------
//	check line
//----------------------------------------------------------------------------
int CheckLine( char *lpBuff, int BeforeLine )
//
// line を調べて、解析する
//
//	0:空行 / 1:target / 2:command / 3:define macro / 4:暗黙のルール / 5:ディティクティブ / 6:単一コマンド
//	-1は無効な行
//
{
	BYTE ch;

	switch( *lpBuff ) {
		case 0:				// 空行
			return 0;
		case ':':			// 行頭が : の場合、6:単一コマンド
			return 6;
		case '=':			// 行頭が = の場合
			return -1;
		case ' ':
		case ASCII_TAB:
			while( 1 ) {
				lpBuff++;
				switch( *lpBuff ) {
					case 0:
						return 0;
					case ' ':
					case ASCII_TAB:
						break;
					default:
						// command check
						if( BeforeLine!=0 ) {
							return 2;
						}
						// macro か command か ( = があれば macro )
						while( 1 ) {
							lpBuff++;
							switch( *lpBuff ) {
								case '=':
									return 3;
								case 0:
									return 2;
							}
						}
				}
			}
		case '.':			// 行頭が . の場合、1:target -> 5:ディテクティブ -> 4:暗黙のルール
			// 相対ディレクトリーか? ".\"
			if(*(lpBuff+1)=='\\') {
				// 以下のdefaultへ行く
			// 相対ディレクトリーか? "..\"
			} if(*(lpBuff+1)=='.' && *(lpBuff+2)=='\\') {
				// 以下のdefaultへ行く
			// ディテクティブか暗黙のルールか? ( 暗黙のルールなら、'.'がもう一個ある
			} else {
				while( 1 ) {
					lpBuff++;
					switch( *lpBuff ) {
						case '.':
							return 4;
						case 0:
							return 5;
					}
				}
			}
		default:
			lpBuff--;
			while( 1 ) {
				lpBuff++;
				switch( *lpBuff ) {
					case 0:
						return 0;
					case '=':
						return 3;
					case '$':							// macroの場合、スキップする
						lpBuff++;
						ch=*lpBuff;
						if( ch=='*' ){
							if( *(lpBuff+1)=='*' ) {
								lpBuff++;
							}
						// } else if( ch=='@' || ch=='?' || (isalnum(*(lpBuff+1))!=0 ) || 
						} else if( ch=='(' ) {
							do {
								lpBuff++;
								ch=*lpBuff;
								if( ch==0 ) return 0;
							} while( ch!=')' );
						}
						break;
					case ':':
						switch( *(lpBuff-1)){			// ':'の直前が space/tab か?
							case ' ':
							case ASCII_TAB:
								while( 1 ) {			// ':'の後が space/tab だけなら暗黙のルール、それ以外の文字があるならターゲット
									lpBuff++;
									switch( *lpBuff ) {
										case ' ':
										case ASCII_TAB:
											break;
										case 0:
											return 4;
										default:
											return 1;
									}
								}
							default:
								switch( *(lpBuff+1) ) {
									case 0:				// ':'の直後が 0 なら暗黙のルール
										return 4;
									case ' ':
									case ASCII_TAB:		// ':'の直後が space/tab か?
										while( 1 ) {			// ':'の後が space/tab だけなら暗黙のルール、それ以外の文字があるならターゲット
											lpBuff++;
											switch( *lpBuff ) {
												case ' ':
												case ASCII_TAB:
													break;
												case 0:
													return 4;
												default:
													return 1;
											}
										}
								}
						}
						break;
				}
			}
			break;
	}
}
//----------------------------------------------------------------------------
//	read
//----------------------------------------------------------------------------
int ReadLine( FILE *fp, char *lpBuff )
{
	int c,c2;
	char *lpw;

	lpw=lpBuff;
	while(1) {
		c = fgetc( fp );
		if( c==EOF ) {
			// EOF
			if(lpw==lpBuff) return -1;
			*lpw=0;
			return 0;
		} else if( _ismbblead(c)!=0 ) {
			// 漢字
			*lpw++=(char)c;
			c = fgetc( fp );
			if( _ismbbtrail(c)!=0 ) {
				*lpw++=(char)c;
			} else {
				ungetc(c,fp);
			}
		} else if( c=='#' ) {
			// skip line
			while(1){
				c = fgetc( fp );
				if( c==EOF || c==ASCII_CR || c==ASCII_LF ) break;
			}
			ungetc(c,fp);
		} else if( c==ASCII_CR ) {
			// 改行は mac0dh,dos0dh0ah
			c = fgetc( fp );
			if( c!=ASCII_LF ) ungetc(c,fp);
			*lpw=0;
			return 0;
		} else if( c==ASCII_LF ) {
			// 改行は unix0ah
			*lpw=0;
			return 0;
		} else if( c==' ' || c==ASCII_TAB ) {
			// space/tab の次に \ マークの場合、改行をスキップする
			c2 = fgetc( fp );
			if( c2=='\\' ) {
				c2 = fgetc( fp );
				if( c2==ASCII_CR ) {
					// 改行は mac0dh,dos0dh0ah
					if( c!=ASCII_LF ) ungetc(c,fp);
					*lpw++=(char)c;
				} else if( c2==ASCII_LF ) {
					// 改行は unix0ah
					*lpw++=(char)c;
				} else {
					*lpw++=(char)c;
					*lpw++=(char)'\\';
					ungetc(c2,fp);
				}
			} else {
				*lpw++=(char)c;
				ungetc(c2,fp);
			}
		} else {
			*lpw++=(char)c;
		}
	}
}
//----------------------------------------------------------------------------
int ReadMakLoop( FILE *fp, BYTE *lpExec, BYTE* lpExecEnd, BYTE *lpRule, BYTE* lpRuleEnd, BYTE *lpMacroTop, BYTE* lpMacroEnd )
{
	int r,m;
	int b;						// 直前の行のモード
	char ReadLineBuff[MAX_BUFF];
	char Buff[MAX_BUFF];
	char *lpRuleTop;

	lpRuleTop=lpRule;

	b=0;
	while(1) {
		r=ReadLine(fp,ReadLineBuff);
		if( r==-1 ) break;	// eof
		m=CheckLine(ReadLineBuff,b);							// 0:空行 / 1:target / 2:command / 3:define macro / 4:暗黙のルール / 5:ディテクティブ / 6:単一コマンド
		// printf( "%d:%s[cr][lf]\n",m,ReadLineBuff );
		if( m==-1 ) return 5;
		if( m!=3 ) {
			r=ExpandMacro( ReadLineBuff,Buff,MAX_BUFF,lpMacroTop,lpMacroEnd );
			if(r!=0) return r;
		}
		// printf( "%d:%s[cr][lf]\n",m,ReadLineBuff );
		switch( m ) {
			case 0:						// 0:空行
				break;
			case 1:						// 1:target
				lpExec=MakCommandEnd(lpExec,lpExecEnd,b);
				if( lpExec==NULL ) return 8;
				lpRule=MakRuleEnd(lpRule,lpRuleEnd,b);
				if( lpRule==NULL ) return 11;
				if( lpRule==(BYTE*)1 ) return 9;
				b=1;
				lpExec=MakTarget(lpExec,lpExecEnd,ReadLineBuff);
				if( lpExec==NULL ) return 8;
				if( lpExec==(BYTE*)1 ) return 13;
				if( lpExec==(BYTE*)2 ) return 14;
				break;
			case 2:						// 2:command
				if(b==2||b==1) {
					b=2;
					lpExec=MakCommand(lpExec,lpExecEnd,ReadLineBuff,b);
					if( lpExec==NULL ) return 8;
				} else if(b==5||b==4) {
					b=5;
					lpRule=MakRulecommand(lpRule,lpRuleEnd,ReadLineBuff);
					if( lpRule==NULL ) return 11;
				} else {
					// error
					return 6;
				}
				break;
			case 3:						// 3:define macro
				lpExec=MakCommandEnd(lpExec,lpExecEnd,b);
				if( lpExec==NULL ) return 8;
				lpRule=MakRuleEnd(lpRule,lpRuleEnd,b);
				if( lpRule==NULL ) return 11;
				if( lpRule==(BYTE*)1 ) return 9;
				b=0;
				r=MakMacroname( lpMacroTop,lpMacroEnd,ReadLineBuff);
				if( r!=0 ) return r;
				break;
			case 4:						// 4:暗黙のコマンド行
				lpExec=MakCommandEnd(lpExec,lpExecEnd,b);
				if( lpExec==NULL ) return 8;
				lpRule=MakRuleEnd(lpRule,lpRuleEnd,b);
				if( lpRule==NULL ) return 11;
				if( lpRule==(BYTE*)1 ) return 9;
				b=4;
				lpRule=MakRule(lpRule,lpRuleEnd,lpRuleTop,ReadLineBuff);
				if( lpRule==NULL ) return 11;
				if( lpRule==(BYTE*)1 ) return 16;
				if( lpRule==(BYTE*)2 ) return 17;
				break;
			case 5:						// 5:ディテクティブ
				lpExec=MakCommandEnd(lpExec,lpExecEnd,b);
				if( lpExec==NULL ) return 8;
				lpRule=MakRuleEnd(lpRule,lpRuleEnd,b);
				if( lpRule==NULL ) return 11;
				if( lpRule==(BYTE*)1 ) return 9;
				b=0;
				r=MakDetective(ReadLineBuff);
				if( r!=0 ) return r;
				break;
			case 6:						// 6:単一コマンド
				lpExec=MakCommandEnd(lpExec,lpExecEnd,b);
				if( lpExec==NULL ) return 8;
				lpRule=MakRuleEnd(lpRule,lpRuleEnd,b);
				if( lpRule==NULL ) return 11;
				if( lpRule==(BYTE*)1 ) return 9;
				b=0;
				lpExec=MakCommandonly(lpExec,lpExecEnd,ReadLineBuff,b);
				if( lpExec==NULL ) return 8;
				if( lpExec==(BYTE*)1 ) return 10;
				break;
		}
	}
	lpExec=MakCommandEnd(lpExec,lpExecEnd,b);
	if( lpExec==NULL ) return 8;
	lpExec=MakCommandEndEnd(lpExec,lpExecEnd,b);
	if( lpExec==NULL ) return 8;
	lpRule=MakRuleEnd(lpRule,lpRuleEnd,b);
	if( lpRule==NULL ) return 11;
	if( lpRule==(BYTE*)1 ) return 9;
	lpRule=MakRuleEndEnd(lpRule,lpRuleEnd,b);
	if( lpRule==NULL ) return 11;
	return 0;
}
//----------------------------------------------------------------------------
int ReadMak( char *szMakeName, BYTE *lpExec, BYTE *lpExecEnd, BYTE *lpRule, BYTE *lpRuleEnd, BYTE *lpMacro, BYTE *lpMacroEnd )
{
	FILE *fp;
	int r;

	r=0;
	expand_macro_flag=0;
	fp=NULL;

	handle_in_old=-1;
	handle_in_new=-1;
	handle_out_old=-1;
	handle_out_new=-1;

	fp = fopen( szMakeName,"r" );
	if( fp==NULL ) return 4;

	// まず最初に.makファイルを lpExec のバッファーに読み込む
	if( r==0 ) {
		r = ReadMakLoop( fp,lpExec,lpExecEnd,lpRule,lpRuleEnd,lpMacro,lpMacroEnd );
	}

	if( fp==NULL ) fclose( fp );

	szPipeOut[0]=0;
	szPipeIn[0]=0;

	// lpExecを実行する
	if( r==0 ) {
		r = MainMakLoop( lpExec,lpRule,lpMacro,lpMacroEnd );
	}


	if( handle_in_old!=0 ) {
		_commit( handle_in_old );
		_dup2(handle_in_old,_fileno(stdin));
		_close( handle_in_old );
		handle_in_old=-1;
	}
	if( handle_in_new!=0 ) {
		_commit( handle_in_new );
		_close( handle_in_new );
		handle_in_new=-1;
	}
	if( handle_out_old!=0 ) {
		fflush(stdout);
		_commit( handle_out_old );
		_dup2(handle_out_old,_fileno(stdout));
		_close( handle_out_old );
		handle_out_old=-1;
	}
	if( handle_out_new!=0 ) {
		_commit( handle_out_new );
		_close( handle_out_new );
		handle_out_new=-1;
	}

	if(szPipeOut[0]!=0) remove(szPipeOut);
	if(szPipeIn[0]!=0) remove(szPipeIn);

	return r;
}
//============================================================================
//============================================================================
//  	make main
//============================================================================
//============================================================================
int DefMacro( BYTE *lpMacroTop, BYTE *lpMacroEnd, char *lpa )
{
	char buff[MAX_BUFF];
	char *lpt;

	strcpy( buff,lpa );

	lpt = buff;
	while( 1 ) {
		if( *lpt=='=' ) {
			*lpt=0;
			return SetMakMacroname( lpMacroTop,lpMacroEnd, lpt+1,buff);
		}
		if( *lpt==0 ) {
			return SetMakMacroname( lpMacroTop,lpMacroEnd, lpt,buff);
		}
		lpt++;
	}
}
//----------------------------------------------------------------------------
int smake(int argc, char **argv, char *defmak, int flag, int mode )
//
//	
//

{
	int c,f,d,r;
	char szMakeName[_MAX_PATH];
	BYTE ExecArea[EXECAREASIZE];
	BYTE RuleArea[RULEAREASIZE];
	BYTE MacroArea[MACROAREASIZE];
	ExecArea[0]=0;
	ExecArea[1]=0;
	ExecArea[2]=0;
	ExecArea[3]=0;
	RuleArea[0]=0;
	RuleArea[1]=0;
	RuleArea[2]=0;
	RuleArea[3]=0;
	MacroArea[0]=0;
	MacroArea[1]=0;
	MacroArea[2]=0;
	MacroArea[3]=0;


	setvbuf( stdout,NULL,_IONBF,0 );
//	setvbuf( stdin,NULL,_IONBF,0 );


	fPrint = flag;
	makemode = mode;

	//
	//	set command option
	//
	szMakeName[0]=0;
	c = 1;
	d = 0;
//	fIgnore = 0;
//	fAll = 0;
//	fEnv = 0;
//	fTarget = 0;
//	fDel = 0;
	option_flag=0;
	while( argc != c ) {
		if( argv[c][0] == '/' || argv[c][0] == '-' ) {
			f = 1;
			while( argv[c][f] != 0 ) {
				if( argv[c][f] == 'h' || argv[c][f] == 'H' || argv[c][f] == '?' ) {
					// 1:usage();
					return 1;
				} else if( argv[c][f] == 'i' ) {
					// -i		実行プログラムの error を無視して続けます。
					//fIgnore = 1;
					// 01bit -i ignore error
					option_flag|=0x0002;
				} else if( argv[c][f] == 'f' ) {
					// -f<makefile>	makefile の指定をします。
					f++;
					strcpy( szMakeName, argv[c]+f );
					while( argv[c][f]>' ' ) f++;
					f--;
					d++;
				} else if( argv[c][f] == 'S' ) {
					// -S		メモリーを swap して最大限のメモリーを実行プログラムが使用できるようにします。
					// win32では無意味
					// 00bit -S swap(kmake専用)
					option_flag|=0x0001;
				} else if( argv[c][f] == 'B' ) {
					// -B		無条件で全てのコマンド行を実行します。
					//fAll = 1;
					// 02bit -B 強制ビルド
					option_flag|=0x0004;
				} else if( argv[c][f] == 'e' ) {
					// -e		マクロを環境変数で展開しないようにします。
					//fEnv = 1;
					// 03bit -e env
					option_flag|=0x0008;
				} else if( argv[c][f] == 'N' ) {
					// -N		ターゲットの作成に失敗したときにターゲットを削除しない
					//fTarget = 1;
					// 04bit -N 削除しない
					option_flag|=0x0010;
				} else if( argv[c][f] == 'A' ) {
					// -A		全てのターゲットファイルを削除してから make を実行します。
					//fDel = 1;
					// 05bit -A 全てのファイルを削除の後、実行
					option_flag|=0x0020;
				} else if( argv[c][f] == 'D' ) {
					// -D		macro
					f++;
					r=DefMacro( MacroArea,MacroArea+MACROAREASIZE,&argv[c][f] );
					if( r!=0 ) return r;
					break;
				} else {
					// 2:error( "err03(15):オプションが異常です" );
					return 2;
				}
				f++;
			}
		} else {
			// -f を使用しなくてもファイル名の指定は出来ます。
			// macroかファイルネームか設定
			f=0;
			while( 1 ) {
				if( argv[c][f]=='=' ) {
					r=DefMacro( MacroArea,MacroArea+MACROAREASIZE,argv[c] );
					if( r!=0 ) return r;
					break;
				} else if( argv[c][f]==0 ) {
					// '=' が無かった場合
					strcpy( szMakeName, argv[c] );
					d++;
					break;
				}
				f++;
			}
		}
		c++;
	}

	// ファイル名の指定が無いとき
	if( d==0 ) {
		strcpy( szMakeName, defmak );
	}
	if( szMakeName[0]==0 ) {
		return 3;
	}

	// read mak
	r=ReadMak( szMakeName,ExecArea,ExecArea+EXECAREASIZE,RuleArea,RuleArea+RULEAREASIZE,MacroArea,MacroArea+MACROAREASIZE );
	return r;
}

//============================================================================
//============================================================================
//  	error code
//============================================================================
//============================================================================
void GetExecNameExt( BYTE *lpw,BYTE *lpr )
//
//	フルパス名からファイル名と拡張子を収得
//
{
	BYTE ch;
	BYTE *lpname;

	lpname=lpr;
	do{
		ch=*lpr++;
		if( ch=='\\' ) {
			lpname=lpr;
		} else if( _ismbblead(ch)!=0 ) {
			// 漢字
			if( _ismbbtrail(*lpr)!=0 ) lpr++;
		}
	} while( *lpr!=0 );
	strcpy( lpw,lpname );
}
//----------------------------------------------------------------------------
void CutExt( BYTE *lpw )
//
//	拡張子を消す
//
{
	BYTE ch;
	BYTE *lpext;

	lpext=NULL;
	do{
		ch=*lpw;
		if( ch=='.' ) lpext=lpw;
		lpw++;
	} while( ch!=0 );
	if( lpext!=NULL ) *lpext=0;
}
//----------------------------------------------------------------------------
void printmakeerror( BYTE *szExe,int n )
{
	BYTE buff0[_MAX_PATH];
	BYTE buff1[_MAX_PATH];

	switch( n ) {
		case 1:
			// 1:usage();
			GetExecNameExt( buff0,szExe );
			strcpy( buff1,buff0 );
			CutExt( buff1 );
			if( makemode==1 ) {
				printf( "【makeモード時の説明】\n" );
				printf( "make モードはオプションの先頭で -f でファイルを指定してください\n" );
				printf( "make 内で tool を使用する場合には、ファイル名だけ\n" );
				printf( "( 拡張子はあっても良い %s,%s )で指定してください\n\n",buff0,buff1 );
			}
			printf( "%s ver1.30 : simple make for windows console (C) KENJI 1999.8-\n",buff0 );
			if( makemode==0 ) {
				printf( "Usage: %s [-options] [makefile] [-options]\n",buff1 );
				printf( "	-fmakefile	makefile の指定 (default:makefile.mak)\n" );
			} else {
				printf( "Usage: %s -fmakefilename [-options] [makefile] [-options]\n",buff1 );
				printf( "	-fmakefile	makefile の指定 (必須で必ず最初に指定すること)\n" );
			}
			printf( "	-i		実行プログラムの error を無視して続ける\n" );
			printf( "	-S		メモリーを swap して最大限のメモリーを実行プログラムへ\n" );
			printf( "	-B		無条件で全てのコマンド行を実行(error時の処理は-iで指定)\n" );
			printf( "	-e		マクロを環境変数で展開しない\n" );
			printf( "	-N		ターゲットの作成に失敗したときにターゲットを削除しない\n" );
			printf( "	-A		全てのターゲットファイルを削除してから make を実行\n" );
			printf( "	-Dmacro=data	マクロを登録する\n" );
			printf( "	-h or -? or -H	usage 表示\n" );
			break;
		case 2:
			printf( "make02:オプションが異常です\n" );
			break;
		case 3:
			printf( "make03:MAKEファイルネームが指定されていません\n" );
			break;
		case 4:
			printf( "make04:ファイルがopenできません\n" );
			break;
		case 5:
			printf( "make05:無効な行があります\n" );
			break;
		case 6:
			printf( "make06:macroが大きすぎます\n" );
			break;
		case 7:
			printf( "make07:target が指定されていないのに command 行が存在します\n" );
			break;
		case 8:
			printf( "make08:バッファーの容量が足りません。.mak ファイルが大きすぎます。\n" );
			break;
		case 9:
			printf( "make09:暗黙のコマンド行が指定されていません\n" );
			break;
		case 10:
			printf( "make10:コマンドオンリー行が異常です\n" );
			break;
		case 11:
			printf( "make11: RULEバッファーの容量が足りません\n" );
			break;
		case 12:
			printf( "make12:マクロの実行時展開の size が 64kb 以上です\n" );
			break;
		case 13:
			printf( "make13:err0014:target が指定されていません\n" );
			break;
		case 14:
			printf( "make14:dependent が指定されていません\n" );
			break;
		case 15:
			printf( "make15:ディテクティブが異常です\n" );
			break;
		case 16:
			printf( "make16:暗黙のコマンド行が異常です\n" );
			break;
		case 17:
			printf( "make17:暗黙のコマンドの size が 64kb 以上です\n" );
			break;
		case 18:
			printf( "make18:macro name が異常です\n" );
			break;
		case 19:
			printf( "make19:macro が計 64kb 以上登録されています\n" );
			break;
		case 20:
			printf( "make20:コマンドラインのマクロが異常です\n" );
			break;
		case 21:
			printf( "make21:マクロ内の文字列の置換が異常です\n" );
			break;
		case 22:
			printf( "make22:dependent file が見つかりません\n" );
			break;
		case 23:
			printf( "make23:コマンド行もしくは対応する暗黙のコマンドが存在しません\n" );
			break;
		case 24:
			printf( "make24:環境変数 comspec が定義されていません\n" );
			break;
		case 25:
			printf( "make25:exec が失敗しました\n" );
			break;
		case 26:
			printf( "make26:子プロセスがエラー終了しました\n" );
			break;
		case 27:
			printf( "make27:リダイレクトファイル/パイプが書き込めません\n" );
			break;
		case 28:
			printf( "make28:リダイレクトファイル/パイプが読み込めません\n" );
			break;
		case 29:
			printf( "make29:出力リダイレクトやパイプが 2 個あります\n" );
			break;
		case 30:
			printf( "make30:入力リダイレクトやパイプが 2 個あります\n" );
			break;
		case 31:
			printf( "make31:パイプ処理の次のコマンドが存在しません\n" );
			break;
	}
}

//============================================================================
//============================================================================
//  	make をプログラムへ内蔵するためのサポート
//============================================================================
//============================================================================
int makechk( int argc, char **argv )
//
//	オプションの先頭に -f があるか ?
//
{
	if( argc<=1 ) return 0;
	if( ( argv[1][0]=='/' || argv[1][0]=='-' ) && argv[1][1]=='f' ) return 1;
	return 0;
}
//----------------------------------------------------------------------------
void makeregist( BYTE *lpTool )
//
//	make へプログラムを登録
//
{
	GetExecNameExt( szTool0,lpTool );
	strcpy( szTool1,szTool0 );
	CutExt( szTool1 );
}
