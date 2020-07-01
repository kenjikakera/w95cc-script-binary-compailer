//_ismbblead	マルチバイト文字の第 1 バイト。たとえば、コード ページ 932 の場合、有効は範囲は 0x81  -  0x9F と  0xE0  -  0xFC です。
//_ismbbtrail	マルチバイト文字の第 2 バイト。たとえば、コード ページ 932 の場合、有効な範囲は 0x40 - 0x7E と  0x80 - 0xEC です。
/* ==========================================================================
=============================================================================
		include
=============================================================================
========================================================================== */
//;;;make
#include <windows.h>
#include <mmsystem.h>
//;;;make


#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mbstring.h>
#include <time.h>
#include <ctype.h>
#include <sys\stat.h>
#include <sys\types.h>
#include <setjmp.h>
#include "w95cc.h"
#include "compiler.h"

//typedef unsigned int DWORD;
//typedef unsigned short WORD;
//typedef unsigned char UCHAR;
//typedef unsigned char BYTE;

#include "make.h"

#define W95CC_MODE	1

/*===========================================================================
=============================================================================
		define
=============================================================================
===========================================================================*/


//
//	token
//
#define TOKEN_AUTO				"auto"
#define NUM_AUTO				0x00000000
#define TOKEN_DOUBLE			"double"
#define NUM_DOUBLE				0x00000001
#define TOKEN_INT				"int"
#define NUM_INT					0x00000002
#define TOKEN_STRUCT			"struct"
#define NUM_STRUCT				0x00000003
#define TOKEN_BREAK				"break"
#define NUM_BREAK				0x00000004
#define TOKEN_ELSE				"else"
#define NUM_ELSE				0x00000005
#define TOKEN_LONG				"long"
#define NUM_LONG				0x00000006
#define TOKEN_SWITCH			"switch"
#define NUM_SWITCH				0x00000007
#define TOKEN_CASE				"case"
#define NUM_CASE				0x00000008
#define TOKEN_ENUM				"enum"
#define NUM_ENUM				0x00000009
#define TOKEN_REGISTER			"register"
#define NUM_REGISTER			0x0000000a
#define TOKEN_TYPEDEF			"typedef"
#define NUM_TYPEDEF				0x0000000b
#define TOKEN_CHAR				"char"
#define NUM_CHAR				0x0000000c
#define TOKEN_EXTERN			"extern"
#define NUM_EXTERN				0x0000000d
#define TOKEN_RETURN			"return"
#define NUM_RETURN				0x0000000e
#define TOKEN_UNION				"union"
#define NUM_UNION				0x0000000f
#define TOKEN_CONST				"const"
#define NUM_CONST				0x00000010
#define TOKEN_FLOAT				"float"
#define NUM_FLOAT				0x00000011
#define TOKEN_SHORT				"short"
#define NUM_SHORT				0x00000012
#define TOKEN_UNSIGNED			"unsigned"
#define NUM_UNSIGNED			0x00000013
#define TOKEN_CONTINUE			"continue"
#define NUM_CONTINUE			0x00000014
#define TOKEN_FOR				"for"
#define NUM_FOR					0x00000015
#define TOKEN_SIGNED			"signed"
#define NUM_SIGNED				0x00000016
#define TOKEN_VOID				"void"
#define NUM_VOID				0x00000017
#define TOKEN_DEFAULT			"default"
#define NUM_DEFAULT				0x00000018
#define TOKEN_GOTO				"goto"
#define NUM_GOTO				0x00000019
#define TOKEN_SIZEOF			"sizeof"
#define NUM_SIZEOF				0x0000001a
#define TOKEN_VOLATILE			"volatile"
#define NUM_VOLATILE			0x0000001b
#define TOKEN_DO				"do"
#define NUM_DO					0x0000001c
#define TOKEN_IF				"if"
#define NUM_IF					0x0000001d
#define TOKEN_STATIC			"static"
#define NUM_STATIC				0x0000001e
#define TOKEN_WHILE				"while"
#define NUM_WHILE				0x0000001f

#define TOKEN_PCALC				"#calc"
#define NUM_PCALC				0x00000100
#define TOKEN_PDEFINE			"#define"
#define NUM_PDEFINE				0x00000101
#define TOKEN_PADEFINE			"#adefine"
#define NUM_PADEFINE			0x00000102
#define TOKEN_PELIF				"#elif"
#define NUM_PELIF				0x00000103
#define TOKEN_PELSEIF			"#elseif"
#define NUM_PELSEIF				0x00000103
#define TOKEN_PELSE				"#else"
#define NUM_PELSE				0x00000104
#define TOKEN_PENDIF			"#endif"
#define NUM_PENDIF				0x00000105
#define TOKEN_PENDM				"#endm"
#define NUM_PENDM				0x00000106
#define TOKEN_PIF				"#if"
#define NUM_PIF					0x00000107
#define TOKEN_PIFDEF			"#ifdef"
#define NUM_PIFDEF				0x00000108
#define TOKEN_PIFNDEF			"#ifndef"
#define NUM_PIFNDEF				0x00000109
#define TOKEN_PERROR			"#error"
#define NUM_PERROR				0x0000010a
#define TOKEN_PINCLUDE			"#include"
#define NUM_PINCLUDE			0x0000010b
#define TOKEN_PLINE				"#line"
#define NUM_PLINE				0x0000010c
#define TOKEN_PL2				"#l2"
#define NUM_PL2					0x0000010d
#define TOKEN_PL2E				"#l2e"
#define NUM_PL2E				0x0000010e
#define TOKEN_PMACRO			"#macro"
#define NUM_PMACRO				0x0000010f
#define TOKEN_PAMACRO			"#amacro"
#define NUM_PAMACRO				0x00000110
#define TOKEN_PMES				"#mes"
#define NUM_PMES				0x00000111
#define TOKEN_PUNDEF			"#undef"
#define NUM_PUNDEF				0x00000112
#define TOKEN_PMLE				"#mle"
#define NUM_PMLE				0x00000113
#define TOKEN_PLET				"#let"
#define NUM_PLET				0x00000114
#define TOKEN_PUNSTR			"#unstr"
#define NUM_PUNSTR				0x00000115
#define TOKEN_PPUSHNUM			"#pushnum"
#define NUM_PPUSHNUM			0x00000116
#define TOKEN_PPOPNUM			"#popnum"
#define NUM_PPOPNUM				0x00000117
#define TOKEN_PGETNUM			"#getnum"
#define NUM_PGETNUM				0x00000118
#define TOKEN_PADDNUMSP			"#addnumsp"
#define NUM_PADDNUMSP			0x00000119
#define TOKEN_PSUBNUMSP			"#subnumsp"
#define NUM_PSUBNUMSP			0x0000011a
#define TOKEN_PGETNUMSP			"#getnumsp"
#define NUM_PGETNUMSP			0x0000011b
#define TOKEN_PSEARCHNUM		"#searchnum"
#define NUM_PSEARCHNUM			0x0000011c
#define TOKEN_PPUSHSTR			"#pushstr"
#define NUM_PPUSHSTR			0x0000011d
#define TOKEN_PPOPSTR			"#popstr"
#define NUM_PPOPSTR				0x0000011e
#define TOKEN_PGETSTR			"#getstr"
#define NUM_PGETSTR				0x0000011f
#define TOKEN_PADDSTRSP			"#addstrsp"
#define NUM_PADDSTRSP			0x00000120
#define TOKEN_PSUBSTRSP			"#substrsp"
#define NUM_PSUBSTRSP			0x00000121
#define TOKEN_PGETSTRSP			"#getstrsp"
#define NUM_PGETSTRSP			0x00000122
#define TOKEN_PEXPDEFINE		"#expdefine"
#define NUM_PEXPDEFINE			0x00000123
#define TOKEN_PDELETEFILE		"#deletefile"
#define NUM_PDELETEFILE			0x00000124
#define TOKEN_PCOPYFILE			"#copyfile"
#define NUM_PCOPYFILE			0x00000125
#define TOKEN_PEXISTFILE		"#existfile"
#define NUM_PEXISTFILE			0x00000126
#define TOKEN_PEXEC0			"#exec0"
#define NUM_PEXEC0				0x00000127
#define TOKEN_PEXEC1			"#exec1"
#define NUM_PEXEC1				0x00000128
#define TOKEN_POPENFILE			"#openfile"
#define NUM_POPENFILE			0x00000129
#define TOKEN_PPUTFILE			"#putfile"
#define NUM_PPUTFILE			0x0000012a
#define TOKEN_PCLOSEFILE		"#closefile"
#define NUM_PCLOSEFILE			0x0000012b
#define TOKEN_PCRFILE			"#crfile"
#define NUM_PCRFILE				0x0000012c
#define TOKEN_PFUNCTION			"#function"
#define NUM_PFUNCTION			0x0000012d
#define TOKEN_PAFUNCTION		"#afunction"
#define NUM_PAFUNCTION			0x0000012e
#define TOKEN_PENDF				"#endf"
#define NUM_PENDF				0x0000012f
#define TOKEN_PFUNCTIONSTACK	"#functionstack"
#define NUM_PFUNCTIONSTACK		0x00000130
#define TOKEN_PFUNCTIONCALL		"#functioncall"
#define NUM_PFUNCTIONCALL		0x00000131
#define TOKEN_PAFUNCTIONCALL	"#afunctioncall"
#define NUM_PAFUNCTIONCALL		0x00000132
#define TOKEN_PMBC				"#mbc"
#define NUM_PMBC				0x00000133
#define TOKEN_PMBCOFF			"#mbcoff"
#define NUM_PMBCOFF				0x00000134
#define TOKEN_PEND				"#end"
#define NUM_PEND				0x00000135

#define TOKEN_BLANK				"blank"
#define NUM_BLANK				0x00000200
#define TOKEN_DEFINED			"defined"
#define NUM_DEFINED				0x00000201

#define NUM_TOP					NUM_AUTO
#define NUM_END					NUM_DEFINED

#define NUM_DEFINED_TOP			0x10000000
#define NUM_DEFINED_END			0x3FFFFFFF

#define NUM_UNDEFINED			0xffffffff
#define CCNAME					"____W95CC__"


//
//	定義済み macro
//
#define TOKEN___DATE__			"__DATE__"
#define TOKEN___TIME__			"__TIME__"
#define TOKEN___TIMESTAMP__		"__TIMESTAMP__"
#define TOKEN___LINE__			"__LINE__"
#define TOKEN___FILE__			"__FILE__"
#define TOKEN__FILE__0D			"__FILE__0D"
#define TOKEN__FILE__0DP		"__FILE__0DP"
#define TOKEN__FILE__0DPF		"__FILE__0DPF"
#define TOKEN__FILE__0DPFE		"__FILE__0DPFE"
#define TOKEN__FILE__0P			"__FILE__0P"
#define TOKEN__FILE__0PF		"__FILE__0PF"
#define TOKEN__FILE__0PFE		"__FILE__0PFE"
#define TOKEN__FILE__0F			"__FILE__0F"
#define TOKEN__FILE__0FE		"__FILE__0FE"
#define TOKEN__FILE__0E			"__FILE__0E"
#define TOKEN__FILE__1D			"__FILE__1D"
#define TOKEN__FILE__1DP		"__FILE__1DP"
#define TOKEN__FILE__1DPF		"__FILE__1DPF"
#define TOKEN__FILE__1DPFE		"__FILE__1DPFE"
#define TOKEN__FILE__1P			"__FILE__1P"
#define TOKEN__FILE__1PF		"__FILE__1PF"
#define TOKEN__FILE__1PFE		"__FILE__1PFE"
#define TOKEN__FILE__1F			"__FILE__1F"
#define TOKEN__FILE__1FE		"__FILE__1FE"
#define TOKEN__FILE__1E			"__FILE__1E"
#define TOKEN__FILE__2D			"__FILE__2D"
#define TOKEN__FILE__2DP		"__FILE__2DP"
#define TOKEN__FILE__2DPF		"__FILE__2DPF"
#define TOKEN__FILE__2DPFE		"__FILE__2DPFE"
#define TOKEN__FILE__2P			"__FILE__2P"
#define TOKEN__FILE__2PF		"__FILE__2PF"
#define TOKEN__FILE__2PFE		"__FILE__2PFE"
#define TOKEN__FILE__2F			"__FILE__2F"
#define TOKEN__FILE__2FE		"__FILE__2FE"
#define TOKEN__FILE__2E			"__FILE__2E"
#define TOKEN__FILE__3D			"__FILE__3D"
#define TOKEN__FILE__3DP		"__FILE__3DP"
#define TOKEN__FILE__3DPF		"__FILE__3DPF"
#define TOKEN__FILE__3DPFE		"__FILE__3DPFE"
#define TOKEN__FILE__3P			"__FILE__3P"
#define TOKEN__FILE__3PF		"__FILE__3PF"
#define TOKEN__FILE__3PFE		"__FILE__3PFE"
#define TOKEN__FILE__3F			"__FILE__3F"
#define TOKEN__FILE__3FE		"__FILE__3FE"
#define TOKEN__FILE__3E			"__FILE__3E"
#define TOKEN__FILE__4D			"__FILE__4D"
#define TOKEN__FILE__4DP		"__FILE__4DP"
#define TOKEN__FILE__4DPF		"__FILE__4DPF"
#define TOKEN__FILE__4DPFE		"__FILE__4DPFE"
#define TOKEN__FILE__4P			"__FILE__4P"
#define TOKEN__FILE__4PF		"__FILE__4PF"
#define TOKEN__FILE__4PFE		"__FILE__4PFE"
#define TOKEN__FILE__4F			"__FILE__4F"
#define TOKEN__FILE__4FE		"__FILE__4FE"
#define TOKEN__FILE__4E			"__FILE__4E"
#define TOKEN__FILE__5D			"__FILE__5D"
#define TOKEN__FILE__5DP		"__FILE__5DP"
#define TOKEN__FILE__5DPF		"__FILE__5DPF"
#define TOKEN__FILE__5DPFE		"__FILE__5DPFE"
#define TOKEN__FILE__5P			"__FILE__5P"
#define TOKEN__FILE__5PF		"__FILE__5PF"
#define TOKEN__FILE__5PFE		"__FILE__5PFE"
#define TOKEN__FILE__5F			"__FILE__5F"
#define TOKEN__FILE__5FE		"__FILE__5FE"
#define TOKEN__FILE__5E			"__FILE__5E"
#define TOKEN__FILE__6D			"__FILE__6D"
#define TOKEN__FILE__6DP		"__FILE__6DP"
#define TOKEN__FILE__6DPF		"__FILE__6DPF"
#define TOKEN__FILE__6DPFE		"__FILE__6DPFE"
#define TOKEN__FILE__6P			"__FILE__6P"
#define TOKEN__FILE__6PF		"__FILE__6PF"
#define TOKEN__FILE__6PFE		"__FILE__6PFE"
#define TOKEN__FILE__6F			"__FILE__6F"
#define TOKEN__FILE__6FE		"__FILE__6FE"
#define TOKEN__FILE__6E			"__FILE__6E"
#define TOKEN__FILE__7D			"__FILE__7D"
#define TOKEN__FILE__7DP		"__FILE__7DP"
#define TOKEN__FILE__7DPF		"__FILE__7DPF"
#define TOKEN__FILE__7DPFE		"__FILE__7DPFE"
#define TOKEN__FILE__7P			"__FILE__7P"
#define TOKEN__FILE__7PF		"__FILE__7PF"
#define TOKEN__FILE__7PFE		"__FILE__7PFE"
#define TOKEN__FILE__7F			"__FILE__7F"
#define TOKEN__FILE__7FE		"__FILE__7FE"
#define TOKEN__FILE__7E			"__FILE__7E"
#define TOKEN___FILE0__			"__FILE0__"
#define TOKEN__FILE0__0D		"__FILE0__0D"
#define TOKEN__FILE0__0DP		"__FILE0__0DP"
#define TOKEN__FILE0__0DPF		"__FILE0__0DPF"
#define TOKEN__FILE0__0DPFE		"__FILE0__0DPFE"
#define TOKEN__FILE0__0P		"__FILE0__0P"
#define TOKEN__FILE0__0PF		"__FILE0__0PF"
#define TOKEN__FILE0__0PFE		"__FILE0__0PFE"
#define TOKEN__FILE0__0F		"__FILE0__0F"
#define TOKEN__FILE0__0FE		"__FILE0__0FE"
#define TOKEN__FILE0__0E		"__FILE0__0E"
#define TOKEN__FILE0__1D		"__FILE0__1D"
#define TOKEN__FILE0__1DP		"__FILE0__1DP"
#define TOKEN__FILE0__1DPF		"__FILE0__1DPF"
#define TOKEN__FILE0__1DPFE		"__FILE0__1DPFE"
#define TOKEN__FILE0__1P		"__FILE0__1P"
#define TOKEN__FILE0__1PF		"__FILE0__1PF"
#define TOKEN__FILE0__1PFE		"__FILE0__1PFE"
#define TOKEN__FILE0__1F		"__FILE0__1F"
#define TOKEN__FILE0__1FE		"__FILE0__1FE"
#define TOKEN__FILE0__1E		"__FILE0__1E"
#define TOKEN__FILE0__2D		"__FILE0__2D"
#define TOKEN__FILE0__2DP		"__FILE0__2DP"
#define TOKEN__FILE0__2DPF		"__FILE0__2DPF"
#define TOKEN__FILE0__2DPFE		"__FILE0__2DPFE"
#define TOKEN__FILE0__2P		"__FILE0__2P"
#define TOKEN__FILE0__2PF		"__FILE0__2PF"
#define TOKEN__FILE0__2PFE		"__FILE0__2PFE"
#define TOKEN__FILE0__2F		"__FILE0__2F"
#define TOKEN__FILE0__2FE		"__FILE0__2FE"
#define TOKEN__FILE0__2E		"__FILE0__2E"
#define TOKEN__FILE0__3D		"__FILE0__3D"
#define TOKEN__FILE0__3DP		"__FILE0__3DP"
#define TOKEN__FILE0__3DPF		"__FILE0__3DPF"
#define TOKEN__FILE0__3DPFE		"__FILE0__3DPFE"
#define TOKEN__FILE0__3P		"__FILE0__3P"
#define TOKEN__FILE0__3PF		"__FILE0__3PF"
#define TOKEN__FILE0__3PFE		"__FILE0__3PFE"
#define TOKEN__FILE0__3F		"__FILE0__3F"
#define TOKEN__FILE0__3FE		"__FILE0__3FE"
#define TOKEN__FILE0__3E		"__FILE0__3E"
#define TOKEN__FILE0__4D		"__FILE0__4D"
#define TOKEN__FILE0__4DP		"__FILE0__4DP"
#define TOKEN__FILE0__4DPF		"__FILE0__4DPF"
#define TOKEN__FILE0__4DPFE		"__FILE0__4DPFE"
#define TOKEN__FILE0__4P		"__FILE0__4P"
#define TOKEN__FILE0__4PF		"__FILE0__4PF"
#define TOKEN__FILE0__4PFE		"__FILE0__4PFE"
#define TOKEN__FILE0__4F		"__FILE0__4F"
#define TOKEN__FILE0__4FE		"__FILE0__4FE"
#define TOKEN__FILE0__4E		"__FILE0__4E"
#define TOKEN__FILE0__5D		"__FILE0__5D"
#define TOKEN__FILE0__5DP		"__FILE0__5DP"
#define TOKEN__FILE0__5DPF		"__FILE0__5DPF"
#define TOKEN__FILE0__5DPFE		"__FILE0__5DPFE"
#define TOKEN__FILE0__5P		"__FILE0__5P"
#define TOKEN__FILE0__5PF		"__FILE0__5PF"
#define TOKEN__FILE0__5PFE		"__FILE0__5PFE"
#define TOKEN__FILE0__5F		"__FILE0__5F"
#define TOKEN__FILE0__5FE		"__FILE0__5FE"
#define TOKEN__FILE0__5E		"__FILE0__5E"
#define TOKEN__FILE0__6D		"__FILE0__6D"
#define TOKEN__FILE0__6DP		"__FILE0__6DP"
#define TOKEN__FILE0__6DPF		"__FILE0__6DPF"
#define TOKEN__FILE0__6DPFE		"__FILE0__6DPFE"
#define TOKEN__FILE0__6P		"__FILE0__6P"
#define TOKEN__FILE0__6PF		"__FILE0__6PF"
#define TOKEN__FILE0__6PFE		"__FILE0__6PFE"
#define TOKEN__FILE0__6F		"__FILE0__6F"
#define TOKEN__FILE0__6FE		"__FILE0__6FE"
#define TOKEN__FILE0__6E		"__FILE0__6E"
#define TOKEN__FILE0__7D		"__FILE0__7D"
#define TOKEN__FILE0__7DP		"__FILE0__7DP"
#define TOKEN__FILE0__7DPF		"__FILE0__7DPF"
#define TOKEN__FILE0__7DPFE		"__FILE0__7DPFE"
#define TOKEN__FILE0__7P		"__FILE0__7P"
#define TOKEN__FILE0__7PF		"__FILE0__7PF"
#define TOKEN__FILE0__7PFE		"__FILE0__7PFE"
#define TOKEN__FILE0__7F		"__FILE0__7F"
#define TOKEN__FILE0__7FE		"__FILE0__7FE"
#define TOKEN__FILE0__7E		"__FILE0__7E"



//
//	calc
//
#define	CALCODE_LS		1
#define	CALPRI_LS		13
#define	CALLEFT_LS		0
#define	CALCODE_PLUS	2
#define	CALPRI_PLUS		12
#define	CALLEFT_PLUS	1
#define	CALCODE_MINUS	3
#define	CALPRI_MINUS	12
#define	CALLEFT_MINUS	1
#define	CALCODE_NOT		4
#define	CALPRI_NOT		12
#define	CALLEFT_NOT		1
#define	CALCODE_NEG		5
#define	CALPRI_NEG		12
#define	CALLEFT_NEG		1
#define	CALCODE_MUL		6
#define	CALPRI_MUL		11
#define	CALLEFT_MUL		0
#define	CALCODE_DIV		7
#define	CALPRI_DIV		11
#define	CALLEFT_DIV		0
#define	CALCODE_MOD		8
#define	CALPRI_MOD		11
#define	CALLEFT_MOD		0
#define	CALCODE_ADD		9
#define	CALPRI_ADD		10
#define	CALLEFT_ADD		0
#define	CALCODE_SUB		10
#define	CALPRI_SUB		10
#define	CALLEFT_SUB		0
#define	CALCODE_SHL		11
#define	CALPRI_SHL		9
#define	CALLEFT_SHL		0
#define	CALCODE_SHR		12
#define	CALPRI_SHR		9
#define	CALLEFT_SHR		0
#define	CALCODE_MORE	13
#define	CALPRI_MORE		8
#define	CALLEFT_MORE	0
#define	CALCODE_BIG		14
#define	CALPRI_BIG		8
#define	CALLEFT_BIG		0
#define	CALCODE_LESS	15
#define	CALPRI_LESS		8
#define	CALLEFT_LESS	0
#define	CALCODE_SMALL	16
#define	CALPRI_SMALL	8
#define	CALLEFT_SMALL	0
#define	CALCODE_EQU		17
#define	CALPRI_EQU		7
#define	CALLEFT_EQU		0
#define	CALCODE_NEQU	18
#define	CALPRI_NEQU		7
#define	CALLEFT_NEQU	0
#define	CALCODE_BITAND	19
#define	CALPRI_BITAND	6
#define	CALLEFT_BITAND	0
#define	CALCODE_BITXOR	20
#define	CALPRI_BITXOR	5
#define	CALLEFT_BITXOR	0
#define	CALCODE_BITOR	21
#define	CALPRI_BITOR	4
#define	CALLEFT_BITOR	0
#define	CALCODE_AND		22
#define	CALPRI_AND		3
#define	CALLEFT_AND		0
#define	CALCODE_OR		23
#define	CALPRI_OR		2
#define	CALLEFT_OR		0
#define	CALCODE_LE		24
#define	CALPRI_LE		0
#define	CALLEFT_LE		0
#define	CALCODE_EXIT	25
#define	CALCODE_MAX		26
#define CALVALUE_MAX	16*1024
#define CALOPE_MAX		16*1024


//
//	mode
//
#define MODE_NORMAL		0
#define MODE_INCLUDE	1



//
//	hash
//
#define HASH_BEFORE		0
#define HASH_NEXT		4
#define HASH_DATA		8
#define HASH_NUM		12
#define HASH_STR		16


//
//	macro
//
#define MACROPARA		6

/*===========================================================================
=============================================================================
		struct
=============================================================================
===========================================================================*/
// window
typedef struct tagFILEDATA {
	BYTE *lpUnGet;
	BYTE *pTop;
	BYTE *pEnd;
	BYTE *pNow;
	DWORD size;
	int line;										// line
	char filename[FILE_NAME_MAX];					// filename
	int nMacroNest;									// macro buffer pointer
} FILEDATA;


/*===========================================================================
=============================================================================
		prototype
=============================================================================
===========================================================================*/
void yen2esc( UCHAR* );
int Calc( UCHAR*,int );
void puttoken( UCHAR* );
void ExpDefine( UCHAR*, UCHAR*, int, int* );


/*===========================================================================
=============================================================================
		work
=============================================================================
===========================================================================*/
jmp_buf main_sj;						// error 終了の時に、mainprogへ戻るための物


int nError;							// error の数
int InputFlag;						// Inputfile を指定したか
int PreFlag;						// Preprocess file の作成方法
									//	0:tmp file にする / 1:Preprocesser file として出力して残す
int	OutputFlag;						// Compiler を実行するか
int fOpenPre;						// Preprocess file を作ったか

UCHAR filenamei[MAXPATHNAME],filenameo[MAXPATHNAME],filenamep[MAXPATHNAME];

UCHAR **lpRegist;								// token 自身への pointer
//UINT nRegist[MAX_REGISTNUM];					// token 固有番号
UCHAR *RegistBuffer;
DWORD maxRegistbuff;							// macro の定義名の bufferのサイズ
UINT cRegist = 0;								// token を登録するための通し番号
UCHAR *lpRegistEnd;

UINT cDefine = NUM_DEFINED_TOP;
UCHAR *DefineBuffer;
UCHAR **lpDefine;
UCHAR *lpDefineEnd;

// macro 展開用 ( error のための物 )
int MacroLine[MAXMACRONEST];					// マクロを登録している行
UCHAR *lpMacroFile[MAXMACRONEST];				// マクロを登録しているファイル名を指し示すポインター
UCHAR *lpMacroData[MAXMACRONEST];				// マクロデーターへのポインター


UCHAR IncludePath[MAXINCLUDEPATH][MAXPATHNAME];
int nIncludePath = 0;

int NowLine = -1;
int nMacroNest = 0;
struct tm *nowtime;


int nWarning;							// warning

UCHAR CalcPutToken[CALCMAXPUTTOKEN];	// calc の時の puttoken buffer
UCHAR CalcRegist[CALCMAXREGIST];		// #calc の時の regist name
int nCalcPutToken = 0;
int CalcMode = 0;
UCHAR *lpCalcErr;


BYTE *lpUnGetBuffer;					// unget 用の buffer
BYTE *lpUnGet;

FILEDATA FileData[MAXINCLUDEFILE];		// 読み込み file の構造体
FILEDATA *pFileData[MAXINCLUDEFILE];

int NowFile;							// 現在読み込んでいる file 番号
int OldNowFile = -1;
UCHAR *MacroDefineFile;					// macro の定義されている filename を保持する
UCHAR *lpFile,*lpFileEnd;

FILE *pFile;							// preprocessor  出力ファイル
UCHAR *lpPre,*PreBuff;

FILE *putfile[MAXPUTFILE];				// 出力ファイル

int nTokenStart,nTokenEnd;
//;;;make
//	DWORD dw0;
//	DWORD dw1,dw2;
//	DWORD dw3,dw4;
//	DWORD dw5,dw6;
//	DWORD dw7,dw8;
//	DWORD dw9,dw10;
//	DWORD dw11,dw12;


//
//	#if
//
int if_flag[MAXIFNEST];
int if_else[MAXIFNEST];					// else が 2 個続かないかチェック
int if_use[MAXIFNEST];					// その階層で、一度でも条件に引っかかったか
int nest;
int fNowIf;								// 現在の行は #if/#ifdef/#ifndef/#else 等か


//
//	calc
//
int CalcLeft[CALCODE_MAX];
int CalcPri[CALCODE_MAX];
int CalcValue[CALVALUE_MAX];
UCHAR CalcOpe[CALOPE_MAX];
int CalcSp0,CalcSp1;


//
//	token の 1byte 目
//
UCHAR TokenFirst[256] = {				// char type
										// 0x00:throw
										// 0x01:ignore
										// 0x02:error
										// 0x03:lf
										// 0x04:/
										// 0x05:"
										// 0x06:'
										// 0x07:<
										// 0x08:name
										// 0x09:preprocessor command
										// 0x0a:number
										// 0x0b:()[]
										// 0x0c:\ 
										// 0x0d:; 
										// 0x0e:, 
										// 0xff:eof
		0x02,							// 00    ^@ 
		0x02,							// 01 sh ^A 
		0x02,							// 02 sx ^B 
		0x02,							// 03 ex ^C 
		0x02,							// 04 et ^D 
		0x02,							// 05 eq ^E 
		0x02,							// 06 ak ^F 
		0x02,							// 07 bl ^G 
		0x02,							// 08 bs ^H 
		0x00,							// 09 ht ^I 
		0x03,							// 0a lf ^J 
		0x02,							// 0b hm ^K 
		0x02,							// 0c cl ^L 
		0x01,							// 0d cr ^M 
		0x02,							// 0e so ^N 
		0x02,							// 0f si ^O 
		0x02,							// 10 de ^P 
		0x02,							// 11 d1 ^Q 
		0x02,							// 12 d2 ^R 
		0x02,							// 13 d3 ^S 
		0x02,							// 14 d4 ^T 
		0x02,							// 15 nk ^U 
		0x02,							// 16 sn ^V 
		0x02,							// 17 eb ^W 
		0x02,							// 18 cn ^X 
		0x02,							// 19 em ^Y 
		0xff,							// 1a sb ^Z 
		0x02,							// 1b ec ^[ 
		0x02,							// 1c → ^\ 
		0x02,							// 1d ← ^] 
		0x02,							// 1e ↑ ^^ 
		0x02,							// 1f ↓ ^_ 
		0x00,							// 20   
		0x00,							// 21 ! 
		0x05,							// 22 " 
		0x09,							// 23 # 
		0x00,							// 24 $ 
		0x0b,							// 25 % 
		0x0b,							// 26 & 
		0x06,							// 27 ' 
		0x0b,							// 28 ( 
		0x0b,							// 29 ) 
		0x0b,							// 2a * 
		0x0b,							// 2b + 
		0x0e,							// 2c , 
		0x0b,							// 2d - 
		0x0b,							// 2e . 
		0x04,							// 2f / 
		0x0a,							// 30 0 
		0x0a,							// 31 1 
		0x0a,							// 32 2 
		0x0a,							// 33 3 
		0x0a,							// 34 4 
		0x0a,							// 35 5 
		0x0a,							// 36 6 
		0x0a,							// 37 7 
		0x0a,							// 38 8 
		0x0a,							// 39 9 
		0x0b,							// 3a : 
		0x0d,							// 3b ; 
		0x07,							// 3c < 
		0x0b,							// 3d = 
		0x0b,							// 3e > 
		0x0b,							// 3f ? 
		0x00,							// 40 @ 
		0x08,							// 41 A 
		0x08,							// 42 B 
		0x08,							// 43 C 
		0x08,							// 44 D 
		0x08,							// 45 E 
		0x08,							// 46 F 
		0x08,							// 47 G 
		0x08,							// 48 H 
		0x08,							// 49 I 
		0x08,							// 4a J 
		0x08,							// 4b K 
		0x08,							// 4c L 
		0x08,							// 4d M 
		0x08,							// 4e N 
		0x08,							// 4f O 
		0x08,							// 50 P 
		0x08,							// 51 Q 
		0x08,							// 52 R 
		0x08,							// 53 S 
		0x08,							// 54 T 
		0x08,							// 55 U 
		0x08,							// 56 V 
		0x08,							// 57 W 
		0x08,							// 58 X 
		0x08,							// 59 Y 
		0x08,							// 5a Z 
		0x0b,							// 5b [ 
		0x0c,							// 5c \ 
		0x0b,							// 5d ] 
		0x0b,							// 5e ^ 
		0x08,							// 5f _ 
		0x00,							// 60 ` 
		0x08,							// 61 a 
		0x08,							// 62 b 
		0x08,							// 63 c 
		0x08,							// 64 d 
		0x08,							// 65 e 
		0x08,							// 66 f 
		0x08,							// 67 g 
		0x08,							// 68 h 
		0x08,							// 69 i 
		0x08,							// 6a j 
		0x08,							// 6b k 
		0x08,							// 6c l 
		0x08,							// 6d m 
		0x08,							// 6e n 
		0x08,							// 6f o 
		0x08,							// 70 p 
		0x08,							// 71 q 
		0x08,							// 72 r 
		0x08,							// 73 s 
		0x08,							// 74 t 
		0x08,							// 75 u 
		0x08,							// 76 v 
		0x08,							// 77 w 
		0x08,							// 78 x 
		0x08,							// 79 y 
		0x08,							// 7a z 
		0x0b,							// 7b { 
		0x0b,							// 7c | 
		0x0b,							// 7d } 
		0x00,							// 7e ~ 
		0x02,							// 7f   
		0x02,							// 80   
		0x02,							// 81   
		0x02,							// 82   
		0x02,							// 83   
		0x02,							// 84   
		0x02,							// 85   
		0x02,							// 86   
		0x02,							// 87   
		0x02,							// 88   
		0x02,							// 89   
		0x02,							// 8a   
		0x02,							// 8b   
		0x02,							// 8c   
		0x02,							// 8d   
		0x02,							// 8e   
		0x02,							// 8f   
		0x02,							// 90   
		0x02,							// 91   
		0x02,							// 92   
		0x02,							// 93   
		0x02,							// 94   
		0x02,							// 95   
		0x02,							// 96   
		0x02,							// 97   
		0x02,							// 98   
		0x02,							// 99   
		0x02,							// 9a   
		0x02,							// 9b   
		0x02,							// 9c   
		0x02,							// 9d   
		0x02,							// 9e   
		0x02,							// 9f   
		0x02,							// a0  
		0x02,							// a1 ｡ 
		0x02,							// a2 ｢ 
		0x02,							// a3 ｣ 
		0x02,							// a4 ､ 
		0x02,							// a5 ･ 
		0x02,							// a6 ｦ 
		0x02,							// a7 ｧ 
		0x02,							// a8 ｨ 
		0x02,							// a9 ｩ 
		0x02,							// aa ｪ 
		0x02,							// ab ｫ 
		0x02,							// ac ｬ 
		0x02,							// ad ｭ 
		0x02,							// ae ｮ 
		0x02,							// af ｯ 
		0x02,							// b0 ｰ 
		0x02,							// b1 ｱ 
		0x02,							// b2 ｲ 
		0x02,							// b3 ｳ 
		0x02,							// b4 ｴ 
		0x02,							// b5 ｵ 
		0x02,							// b6 ｶ 
		0x02,							// b7 ｷ 
		0x02,							// b8 ｸ 
		0x02,							// b9 ｹ 
		0x02,							// ba ｺ 
		0x02,							// bb ｻ 
		0x02,							// bc ｼ 
		0x02,							// bd ｽ 
		0x02,							// be ｾ 
		0x02,							// bf ｿ 
		0x02,							// c0 ﾀ 
		0x02,							// c1 ﾁ 
		0x02,							// c2 ﾂ 
		0x02,							// c3 ﾃ 
		0x02,							// c4 ﾄ 
		0x02,							// c5 ﾅ 
		0x02,							// c6 ﾆ 
		0x02,							// c7 ﾇ 
		0x02,							// c8 ﾈ 
		0x02,							// c9 ﾉ 
		0x02,							// ca ﾊ 
		0x02,							// cb ﾋ 
		0x02,							// cc ﾌ 
		0x02,							// cd ﾍ 
		0x02,							// ce ﾎ 
		0x02,							// cf ﾏ 
		0x02,							// d0 ﾐ 
		0x02,							// d1 ﾑ 
		0x02,							// d2 ﾒ 
		0x02,							// d3 ﾓ 
		0x02,							// d4 ﾔ 
		0x02,							// d5 ﾕ 
		0x02,							// d6 ﾖ 
		0x02,							// d7 ﾗ 
		0x02,							// d8 ﾘ 
		0x02,							// d9 ﾙ 
		0x02,							// da ﾚ 
		0x02,							// db ﾛ 
		0x02,							// dc ﾜ 
		0x02,							// dd ﾝ 
		0x02,							// de ﾞ 
		0x02,							// df ﾟ 
		0x02,							// e0   
		0x02,							// e1   
		0x02,							// e2   
		0x02,							// e3   
		0x02,							// e4   
		0x02,							// e5   
		0x02,							// e6   
		0x02,							// e7   
		0x02,							// e8   
		0x02,							// e9   
		0x02,							// ea   
		0x02,							// eb   
		0x02,							// ec   
		0x02,							// ed   
		0x02,							// ee   
		0x02,							// ef   
		0x02,							// f0   
		0x02,							// f1   
		0x02,							// f2   
		0x02,							// f3   
		0x02,							// f4   
		0x02,							// f5   
		0x02,							// f6   
		0x02,							// f7   
		0x02,							// f8   
		0x02,							// f9   
		0x02,							// fa   
		0x02,							// fb   
		0x02,							// fc   
		0x02,							// fd   
		0x02,							// fe   
		0x02							// ff   
};

// init
#if 0
UCHAR isname0[256];						// 英数字名か check する
UCHAR isname1[256];						// 英数字名か check する
UCHAR iskanji[256];						// 漢字の 1 文字目か
UCHAR iscalc[256];						// 計算式か
#if USE_OCT
UCHAR isoct[256];						// 8進かチェック
#endif
UCHAR isbin[256];
UCHAR isdec[256];
UCHAR ishex[256];
#endif


//
//	hash
//
UCHAR *HashTable[HASHTABLESIZE];
UCHAR HashData[256];


//
//	num stack
//
int num_sp;
int num_stack[MAX_NUMSTACK];


//
//	num stack
//
int str_sp;
UCHAR* str_stack[MAX_STRSTACK];
UCHAR str_stackarea[MAXSTRSTACKAREA];


//
//	func
//
UCHAR *fUseFunc;								// function を使用したかどうかフラグ ( 0:未使用 1:使用した 2:使用した物で登録済み )
UCHAR FuncStack[MAXTOKEN];						// stack の文字
UCHAR FuncCall[MAXTOKEN];						// call するときの命令
UCHAR FuncCalla[MAXTOKEN];						// call するときの命令


//
//	全角文字の処理
//
UCHAR mbcBuff[MAXTOKEN];						// #mbc buffer

/*===========================================================================
=============================================================================
		program
=============================================================================
========================================================================== */
/*---------------------------------------------------------------------------
		is
---------------------------------------------------------------------------*/
void InitIs( void )
{
	ZeroMemory( HashData, 256 );
	HashData['0'] = 0;
	HashData['1'] = 1;
	HashData['2'] = 2;
	HashData['3'] = 3;
	HashData['4'] = 4;
	HashData['5'] = 5;
	HashData['6'] = 6;
	HashData['7'] = 7;
	HashData['8'] = 8;
	HashData['9'] = 9;
	HashData['@'] = 10;
	HashData['A'] = 11;
	HashData['B'] = 12;
	HashData['C'] = 13;
	HashData['D'] = 14;
	HashData['E'] = 15;
	HashData['F'] = 16;
	HashData['G'] = 17;
	HashData['H'] = 18;
	HashData['I'] = 19;
	HashData['J'] = 20;
	HashData['K'] = 21;
	HashData['L'] = 22;
	HashData['M'] = 23;
	HashData['N'] = 24;
	HashData['O'] = 25;
	HashData['P'] = 26;
	HashData['Q'] = 27;
	HashData['R'] = 28;
	HashData['S'] = 29;
	HashData['T'] = 30;
	HashData['U'] = 31;
	HashData['V'] = 32;
	HashData['W'] = 33;
	HashData['X'] = 34;
	HashData['Y'] = 35;
	HashData['Z'] = 36;
	HashData['_'] = 37;
	HashData['a'] = 38;
	HashData['b'] = 39;
	HashData['c'] = 40;
	HashData['d'] = 41;
	HashData['e'] = 42;
	HashData['f'] = 43;
	HashData['g'] = 44;
	HashData['h'] = 45;
	HashData['i'] = 46;
	HashData['j'] = 47;
	HashData['k'] = 48;
	HashData['l'] = 49;
	HashData['m'] = 50;
	HashData['n'] = 51;
	HashData['o'] = 52;
	HashData['p'] = 53;
	HashData['q'] = 54;
	HashData['r'] = 55;
	HashData['s'] = 56;
	HashData['t'] = 57;
	HashData['u'] = 58;
	HashData['v'] = 59;
	HashData['w'] = 60;
	HashData['x'] = 61;
	HashData['y'] = 62;
	HashData['z'] = 63;

#if 0
	//	英数字名か check する
	ZeroMemory( isname0, 256 );
	isname0['@'] = 1;
	isname0['A'] = 1;
	isname0['B'] = 1;
	isname0['C'] = 1;
	isname0['D'] = 1;
	isname0['E'] = 1;
	isname0['F'] = 1;
	isname0['G'] = 1;
	isname0['H'] = 1;
	isname0['I'] = 1;
	isname0['J'] = 1;
	isname0['K'] = 1;
	isname0['L'] = 1;
	isname0['M'] = 1;
	isname0['N'] = 1;
	isname0['O'] = 1;
	isname0['P'] = 1;
	isname0['Q'] = 1;
	isname0['R'] = 1;
	isname0['S'] = 1;
	isname0['T'] = 1;
	isname0['U'] = 1;
	isname0['V'] = 1;
	isname0['W'] = 1;
	isname0['X'] = 1;
	isname0['Y'] = 1;
	isname0['Z'] = 1;
	isname0['_'] = 1;
	isname0['a'] = 1;
	isname0['b'] = 1;
	isname0['c'] = 1;
	isname0['d'] = 1;
	isname0['e'] = 1;
	isname0['f'] = 1;
	isname0['g'] = 1;
	isname0['h'] = 1;
	isname0['i'] = 1;
	isname0['j'] = 1;
	isname0['k'] = 1;
	isname0['l'] = 1;
	isname0['m'] = 1;
	isname0['n'] = 1;
	isname0['o'] = 1;
	isname0['p'] = 1;
	isname0['q'] = 1;
	isname0['r'] = 1;
	isname0['s'] = 1;
	isname0['t'] = 1;
	isname0['u'] = 1;
	isname0['v'] = 1;
	isname0['w'] = 1;
	isname0['x'] = 1;
	isname0['y'] = 1;
	isname0['z'] = 1;

	//	英数字名か check する
	ZeroMemory( isname1, 256 );
	isname1['0'] = 1;
	isname1['1'] = 1;
	isname1['2'] = 1;
	isname1['3'] = 1;
	isname1['4'] = 1;
	isname1['5'] = 1;
	isname1['6'] = 1;
	isname1['7'] = 1;
	isname1['8'] = 1;
	isname1['9'] = 1;
	isname1['@'] = 1;
	isname1['A'] = 1;
	isname1['B'] = 1;
	isname1['C'] = 1;
	isname1['D'] = 1;
	isname1['E'] = 1;
	isname1['F'] = 1;
	isname1['G'] = 1;
	isname1['H'] = 1;
	isname1['I'] = 1;
	isname1['J'] = 1;
	isname1['K'] = 1;
	isname1['L'] = 1;
	isname1['M'] = 1;
	isname1['N'] = 1;
	isname1['O'] = 1;
	isname1['P'] = 1;
	isname1['Q'] = 1;
	isname1['R'] = 1;
	isname1['S'] = 1;
	isname1['T'] = 1;
	isname1['U'] = 1;
	isname1['V'] = 1;
	isname1['W'] = 1;
	isname1['X'] = 1;
	isname1['Y'] = 1;
	isname1['Z'] = 1;
	isname1['_'] = 1;
	isname1['a'] = 1;
	isname1['b'] = 1;
	isname1['c'] = 1;
	isname1['d'] = 1;
	isname1['e'] = 1;
	isname1['f'] = 1;
	isname1['g'] = 1;
	isname1['h'] = 1;
	isname1['i'] = 1;
	isname1['j'] = 1;
	isname1['k'] = 1;
	isname1['l'] = 1;
	isname1['m'] = 1;
	isname1['n'] = 1;
	isname1['o'] = 1;
	isname1['p'] = 1;
	isname1['q'] = 1;
	isname1['r'] = 1;
	isname1['s'] = 1;
	isname1['t'] = 1;
	isname1['u'] = 1;
	isname1['v'] = 1;
	isname1['w'] = 1;
	isname1['x'] = 1;
	isname1['y'] = 1;
	isname1['z'] = 1;

	//	kanji の 1 文字目か check する
	ZeroMemory( iskanji, 0xe0 );
	for( c=0x7f;c<0xa0;c++ ) {
		iskanji[c] = 1;
	}
	for( c=0xe0;c<256;c++ ) {
		iskanji[c] = 1;
	}

	//	kanji の 1 文字目か check する
	ZeroMemory( iscalc, 256 );
	iscalc['\t'] = 1;
	iscalc[' '] = 1;
	iscalc['!'] = 1;
	iscalc['%'] = 1;
	iscalc['&'] = 1;
	iscalc['('] = 1;
	iscalc[')'] = 1;
	iscalc['*'] = 1;
	iscalc['+'] = 1;
	iscalc['-'] = 1;
	iscalc['/'] = 1;
	iscalc['0'] = 1;
	iscalc['1'] = 1;
	iscalc['2'] = 1;
	iscalc['3'] = 1;
	iscalc['4'] = 1;
	iscalc['5'] = 1;
	iscalc['6'] = 1;
	iscalc['7'] = 1;
	iscalc['8'] = 1;
	iscalc['9'] = 1;
	iscalc['<'] = 1;
	iscalc['='] = 1;
	iscalc['>'] = 1;
	iscalc['^'] = 1;
	iscalc['|'] = 1;
	iscalc['~'] = 1;

	//	oct の 1 文字目か check する
#if USE_OCT
	memset( isoct, 255,256 );
	isoct['0'] = 0;
	isoct['1'] = 1;
	isoct['2'] = 2;
	isoct['3'] = 3;
	isoct['4'] = 4;
	isoct['5'] = 5;
	isoct['6'] = 6;
	isoct['7'] = 7;
#endif

	//	bin の 1 文字目か check する
	memset( isbin, 255,256 );
	isbin['0'] = 0;
	isbin['1'] = 1;

	//	dec の 1 文字目か check する
	memset( ishex, 255,256 );
	ishex['0'] = 0;
	ishex['1'] = 1;
	ishex['2'] = 2;
	ishex['3'] = 3;
	ishex['4'] = 4;
	ishex['5'] = 5;
	ishex['6'] = 6;
	ishex['7'] = 7;
	ishex['8'] = 8;
	ishex['9'] = 9;
	ishex['A'] = 10;
	ishex['B'] = 11;
	ishex['C'] = 12;
	ishex['D'] = 13;
	ishex['E'] = 14;
	ishex['F'] = 15;
	ishex['a'] = 10;
	ishex['b'] = 11;
	ishex['c'] = 12;
	ishex['d'] = 13;
	ishex['e'] = 14;
	ishex['f'] = 15;

	//	dec の 1 文字目か check する
	memset( isdec, 255,256 );
	isdec['0'] = 0;
	isdec['1'] = 1;
	isdec['2'] = 2;
	isdec['3'] = 3;
	isdec['4'] = 4;
	isdec['5'] = 5;
	isdec['6'] = 6;
	isdec['7'] = 7;
	isdec['8'] = 8;
	isdec['9'] = 9;
#endif
}
/*-------------------------------------------------------------------------*/
int _isname0( int c )
//
//	英数字名か check する
//
{
	if( c <  '@' ) return 0;
	if( c <= 'Z' ) return 1;
	if( c == '_' ) return 1;
	if( c <  'a' ) return 0;
	if( c <= 'z' ) return 1;
	return 0;
}
/*-------------------------------------------------------------------------*/
int _isname1( int c )
//
//	英数字名か check する
//
{
	if( c<0 ) {
		return 0;
	}
	if( c>255 ) {
		return 0;
	}


	if( c <  '0' ) return 0;
	if( c <= '9' ) return 1;
	if( c <  '@' ) return 0;
	if( c <= 'Z' ) return 1;
	if( c == '_' ) return 1;
	if( c <  'a' ) return 0;
	if( c <= 'z' ) return 1;
	return 0;
}
/*-------------------------------------------------------------------------*/
int _iskanji( int c )
//
//	漢字の第一バイトか
//
{
	if( c <= 0x7e ) return 0;
	if( c >= 0xe0 ) return 1;
	if( c >= 0xa0 ) return 0;
	return 1;
}
/*-------------------------------------------------------------------------*/
int _iscalc( UCHAR c )
//
//	calc
//
{
	if( c=='\t' || c==' ' || c=='!' || c=='%' || c=='&' ||
	    ( c>='(' && c<='+' ) || c=='-' || ( c>='/' && c<='9' ) ||
	    ( c>='<' && c<='>' ) || c=='^' || c=='|' || c=='~' ) {
		return 1;
	}
	return 0;
}
/*-------------------------------------------------------------------------*/
int _isbin( int c )
//
//	bin か check する
//
{
	if( c == '0' ) return 0;
	if( c == '1' ) return 1;

	return -1;
}
/*-------------------------------------------------------------------------*/
int _ishex( int c )
//
//	hex か check する
//
{
	if( c >= '0' && c <= '9' ) {
		return ( c-'0' );
	}

	if( c >= 'A' && c <= 'F' ) {
		return ( c-'A'+10 );
	}

	if( c >= 'a' && c <= 'f' ) {
		return ( c-'a'+10 );
	}

	return -1;
}
/*-------------------------------------------------------------------------*/
int _isdec( int c )
//
//	dec か check する
//
{
	if( c >= '0' && c <= '9' ) {
		return ( c-'0' );
	}

	return -1;
}
/*---------------------------------------------------------------------------
		yen
---------------------------------------------------------------------------*/
void addendyen( UCHAR *lp )
//
//	文字列の最後が \ ではない場合、\ を追加する
//
{
	if( *lp == 0 ){
		return;
	}

	while( *lp != 0 ) {
//		if( _ismbblead( *lp )) {
		if( _iskanji(*lp)) {
			*lp+=2;
		} else {
			if( *lp++ == '\\' && *lp == 0 ) {
				return;
			}
		}
	}
	*lp++ = '\\';
	*lp = 0;
}
/*-------------------------------------------------------------------------*/
void delendyen( UCHAR *lp )
//
//	文字列の最後が \ の場合、\ を削除する
//
{
	while( *lp != 0 ) {
//		if( _ismbblead( *lp )) {
		if( _iskanji(*lp) ) {
			lp+=2;
		} else {
			if( (*lp++ == '\\') && (*lp == 0) ) {
				*(lp-1) = 0;
				return;
			}
		}
	}
}
/*---------------------------------------------------------------------------
		delete preprocessor
---------------------------------------------------------------------------*/
void DeleteP()
{
	int c;

	// close all
	// _fcloseall();
	if( pFile!=NULL ) {
		fclose( pFile );
		pFile=NULL;
	}
	for( c=0;c<MAXPUTFILE;c++ ) {
		if( putfile[c]!=NULL ) {
			 fclose( putfile[c] );
			 putfile[c]=NULL;
		}
	}

	// delete
	if( fOpenPre == 1 && PreFlag == 0 ) {
		remove( filenamep );
	}
}

/*---------------------------------------------------------------------------
		error
---------------------------------------------------------------------------*/
void FreeMemW95cc( void )
{
	int c;

	// free
	for(c=0;c<MAXINCLUDEFILE;c++) {
		if( FileData[c].pTop!=NULL ) {
			free( FileData[c].pTop );
			FileData[c].pTop = NULL;
		}
	}

	if( lpDefine!=NULL ) {
		free( lpDefine );
		lpDefine = NULL;
	}
	if( fUseFunc!=NULL ) {
		free( fUseFunc );
		fUseFunc = NULL;
	}
	if( lpRegist!=NULL ) {
		free( lpRegist );
		lpRegist = NULL;
	}
	if( RegistBuffer!=NULL ) {
		free( RegistBuffer );
		RegistBuffer = NULL;
	}
	if( DefineBuffer!=NULL ) {
		free( DefineBuffer );
		DefineBuffer = NULL;
	}
	if( MacroDefineFile!=NULL ) {
		free( MacroDefineFile );
		MacroDefineFile = NULL;
	}
	if( PreBuff!=NULL ) {
		free( PreBuff );
		PreBuff = NULL;
	}
	if( lpUnGetBuffer!=NULL ) {
		free( lpUnGetBuffer );
		lpUnGetBuffer = NULL;
	}
}
/*-------------------------------------------------------------------------*/
void err_exit( void )				// error exit
{
	printf( "\n" );
	FreeMemW95cc();
	DeleteP();
	fprintf( stderr,"error exit!!\n" );
	// exit(EXIT_FAILURE);
	longjmp( main_sj,EXIT_FAILURE );
}
/*-------------------------------------------------------------------------*/
void err_pre0( int n )				// line メッセージを表示
{
	if( nError>100 ) {
		fprintf( stderr,"error が多すぎますので中断します。\n" );
		err_exit();
	}
	printf( "%s(%d)", FileData[NowFile].filename, FileData[NowFile].line );
	nError+=n;
}
/*-------------------------------------------------------------------------*/
void macroerror( void )
{
	err_pre0(1);
	printf( "preprocessor error:macro buffer が一杯で展開できません ( 最大 %d bytes )\n", UNGETBUFFER );
}
/*---------------------------------------------------------------------------*/
void PrintHash0( void )
{
	int d0,d1,d2,d3,d4,d5,d6;
	UCHAR *lp;
	int n,c;

	d0 = 0;
	d1 = 0;
	d2 = 0;
	d3 = 0;
	d4 = 0;
	d5 = 0;
	d6 = 0;
	for( c=0;c<HASHTABLESIZE;c++ ) {
		n=0;
		lp = HashTable[c];
		while( lp!=NULL ) {
			if( *(int*)(lp+HASH_DATA) != NUM_UNDEFINED ) n++;
			lp = *(UCHAR**)lp;
		}
		switch( n ) {
			case 0:
				d0++;
				break;
			case 1:
				d1++;
				break;
			case 2:
				d2++;
				break;
			case 3:
				d3++;
				break;
			case 4:
				d4++;
				break;
			case 5:
				d5++;
				break;
			default:
				d6++;
				break;
		}
	}
	fprintf( stderr,"0:%d 1:%d 2:%d 3:%d 4:%d 5:%d 6>:%d \n",d0,d1,d2,d3,d4,d5,d6 );
}
/*---------------------------------------------------------------------------
		exec sub
---------------------------------------------------------------------------*/
BOOL ExecSub0( UCHAR *lp0 )
//
//		exe を実行します。終了を待たずにプリプロセスを次に進めます。
//
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory( &si,sizeof( STARTUPINFO ));
	si.cb          = sizeof( STARTUPINFO );
	si. dwFlags    = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOWNORMAL;
	return CreateProcess( NULL,lp0,NULL,NULL,FALSE,0,NULL,NULL,&si,&pi );
}
//---------------------------------------------------------------------------
BOOL ExecSub1( UCHAR *lp0 )
//
//		exe を実行します。終了を待ってプリプロセスを次に進めます。
//

{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory( &si,sizeof( STARTUPINFO ));
	si.cb          = sizeof( STARTUPINFO );
	si. dwFlags    = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOWNORMAL;
	if( CreateProcess( NULL,lp0,NULL,NULL,FALSE,0,NULL,NULL,&si,&pi ) ) {
		WaitForSingleObject( pi.hProcess,INFINITE );
		return TRUE;
	}
	return FALSE;
}
/*---------------------------------------------------------------------------
		file open
---------------------------------------------------------------------------*/
BOOL open_file( int num )
//
//	file open
//
{
	FILE *fp;

	if ((fp = fopen(FileData[num].filename, "r")) != NULL) {
		// get size
		fseek(fp, 0L, SEEK_END);
		FileData[num].size = ftell(fp);
		rewind(fp);
		// alloc
		FileData[num].pTop = malloc( FileData[num].size+2 );
		FileData[num].pNow = FileData[num].pTop;
		// read file
		FileData[num].size = fread( FileData[num].pTop, sizeof(char) ,FileData[num].size,fp );
		FileData[num].pEnd = FileData[num].pTop + FileData[num].size;
		fclose(fp);

		// set para
		FileData[num].line = 1;
		FileData[num].lpUnGet = lpUnGet;
		FileData[num].nMacroNest = nMacroNest;
		return TRUE;
	}
	return FALSE;
}

/*---------------------------------------------------------------------------
		global number
---------------------------------------------------------------------------*/
int GetGlobalNum( void )
//
//	一意の数を出力する
//
{
	static int num = 0;
	return num++;
}
/*---------------------------------------------------------------------------
		if / else の処理
---------------------------------------------------------------------------*/
void set_if( int c )
{
	nest++;
	if( nest >= MAXIFNEST ) {
		nest--;
		err_pre0(1);
		printf( "preprocessor error:#if/#ifdef/#ifndef/#else の nest が制限以上です ( 最大 %d )\n", MAXIFNEST-1 );
	} else {
		if_flag[nest] = c;
		if_else[nest]=0;				// else の数
	}
}
/*-------------------------------------------------------------------------*/
void set_elif( int c )
{
	if_flag[nest] = c;
}
/*---------------------------------------------------------------------------
		line
---------------------------------------------------------------------------*/
void PrintLine( void )
//
//	現在の line の表示
//
{
	static UCHAR *lpOld = NULL;
	UCHAR buffer[MAXPATHNAME+256];
	int line;
	UCHAR *lp0;

	if( nMacroNest > FileData[NowFile].nMacroNest ){
//		sprintf( buffer,"%d+;",nMacroNest );
//		puttoken( buffer );

		line = MacroLine[nMacroNest];
		lp0 = lpMacroFile[nMacroNest];
	} else {
//		sprintf( buffer,"%d*:",NowFile );
//		puttoken( buffer );

		line = FileData[NowFile].line;
		lp0 = FileData[NowFile].filename;
	}

	NowLine = line;
	if( lpOld == NULL || strcmp( lp0,lpOld ) || (NowFile!=OldNowFile) ) {
		OldNowFile = NowFile;
		sprintf( buffer,"#line %d \"%s\"\n",line, lp0 );
		lpOld = lp0;
		yen2esc( buffer );
	} else {
		sprintf( buffer,"#line %d\n",line );
	}

	puttoken( buffer );
}
/*-------------------------------------------------------------------------*/
void NextLine( void )
//
//	次の line にする
//
{
	static int a=0;
	a++;
	if( a==89673 ) {
		printf( "%d\n", nMacroNest );
		fflush( stdout );
	}

	if( nMacroNest > FileData[NowFile].nMacroNest ){
		MacroLine[nMacroNest]++;
	} else {
		FileData[NowFile].line++;
	}
	NowLine++;
}
/*---------------------------------------------------------------------------
		putchar
---------------------------------------------------------------------------*/
#if 0
           |00 - 3F|40 - 7E|7F|80|81 - 9F|A0 - DF|E0 - EF|F0|F1 - FC|FD - FF|
           |-------+-------+--+--+-------+-------+-------+--+-------+-------|
           |-------+-------+--+--+-------+-------+-------+--+-------+-------|
1バイト文字|---------------|             |-------|
  1st byte                       |-------|       |-------|==|
  2nd byte         |-------|  |-------------------------------------|
#endif


#if 0
UCHAR* PutCharData( int c, UCHAR* lpToken )
//
//	put char data
//
{
	if( _ismbblead( c & 0xff ) && _ismbbtrail( (c>>8) & 0xff )){
		*lpToken++ = ( c & 0xff );
		*lpToken++ = ( (c>>8) & 0xff );
	} else {
		*lpToken++ = c;
	}
	return lpToken;
}
#endif
/*---------------------------------------------------------------------------
		ungetchar
---------------------------------------------------------------------------*/
#if 0
void UnGetCharData( DWORD c )
//
//	char data を 戻す ( mb 対応 )
//
{
	if( lpUnGet <= lpUnGetBuffer ) {
		macroerror();
	}
	if( _ismbblead( c & 0xff ) ){
		*lpUnGet = (UCHAR)( (c>>8) & 0xff );
		lpUnGet--;
		*lpUnGet = (UCHAR)( c & 0xff );
		lpUnGet--;
	} else {
		*lpUnGet = (UCHAR)( c & 0xff );
		lpUnGet--;
	}
	return;
}
#endif


/*-------------------------------------------------------------------------*/
#if ASM_FLAG
extern void UnGetCharDataOne( DWORD );
#else
void UnGetCharDataOne( DWORD c )
//
//	char data を 戻す ( mb 対応 )
//
{
	if( lpUnGet <= lpUnGetBuffer ) {
		macroerror();
	}
	*lpUnGet-- = (UCHAR)(c);
	return;
}
#endif

/*-------------------------------------------------------------------------*/
#if ASM_FLAG
extern void UnGetStrData( UCHAR* );
#else
void UnGetStrData( UCHAR* lpData )
//
//	str data を 戻す ( mb 対応 )
//
{
	UCHAR* lpCopy;
	int size_a;

	size_a = strlen( lpData );
	if( lpUnGet-size_a < lpUnGetBuffer ) {
		err_pre0(1);
		printf( "preprocessor error:macro の展開用 buffer が一杯です ( 最大 %d bytes )\n", UNGETBUFFER );
		return;
	}

	for( lpCopy = lpData + size_a - 1; lpCopy >= lpData; lpCopy-- ) {
		*lpUnGet = *lpCopy;
		lpUnGet--;
	}
}
#endif
/*---------------------------------------------------------------------------
		getchar
---------------------------------------------------------------------------*/
int GetCharDataCheck( void )
{
	free( FileData[NowFile].pTop );
	FileData[NowFile].pTop = NULL;
	if( NowFile != 0 ) {
		NowFile--;
		puttoken( "\n" );
		PrintLine();
		return 1;
	}
	return 0;
}
/*-------------------------------------------------------------------------*/
#if 0
int GetCharDataSub( void )
//
//	get char ( 1 byte 拾得 ) の sub
//
{
	int c;


	if( lpUnGet < FileData[NowFile].lpUnGet ) {
		lpUnGet++;
		return *lpUnGet;
//;;; check -1(EOF) & MB
	}


TOP:
	if( FileData[NowFile].pNow < FileData[NowFile].pEnd ) {
		c = *(FileData[NowFile].pNow);
		FileData[NowFile].pNow++;
		if( c!=0x1a ) {
			return c;
		}
	}
	if( GetCharDataCheck( ) ) {
		goto TOP;
	}
	c = EOF;
	return c;
}
/*-------------------------------------------------------------------------*/
int GetCharData( void )
//
//	get char ( mb 対応 )
//
{
	int d0,d1;


	d0 = GetCharDataSub();

	//
	//	mb
	//
	if( _ismbblead( d0 ) ) {
		d1 = GetCharDataSub();
		if( _ismbbtrail( d1 ) ) {
			d0 += (d1<<8);
		} else {
			err_pre0(1);
			printf( "preprocessor error:mb の 2byte 目が異常です\n" );
			return d0
		}
	}
	return d0;
}
#endif

/*-------------------------------------------------------------------------*/

#if ASM_FLAG
extern int GetCharDataOne( void );
#else
int GetCharDataOne( void )
//
//	get char ( 1 byte 拾得 ) の sub
//
{
	int c;


	if( lpUnGet < FileData[NowFile].lpUnGet ) {
		lpUnGet++;
		return *lpUnGet;
//;;; check -1(EOF) & MB
	}


TOP:
	if( FileData[NowFile].pNow < FileData[NowFile].pEnd ) {
		c = *(FileData[NowFile].pNow);
		FileData[NowFile].pNow++;
		if( c!=0x1a ) {
			return c;
		}
	}
	if( GetCharDataCheck( ) ) {
		goto TOP;
	}
	c = 0x1a;
	return c;
}
#endif



//---------------------------------------------------------------------------
//		hash サブルーチン
//		HASHTABLESIZE 個の hash バンク
//---------------------------------------------------------------------------
DWORD GetHashKey( UCHAR *lpName )
//
//	string から hash key を計算する
//
{
	DWORD nKey = 0;
	UCHAR d;

	while((d=*lpName++)) {
		nKey = (nKey << 5) + HashData[d];
		nKey = (nKey&0x1ffff) + (nKey>>17);
	}
	return (nKey&0x1ffff);
}
/*-------------------------------------------------------------------------*/
DWORD GetHashKeyNum( UCHAR *lpName,int *c )
//
//	string から hash key を計算する
//
{
	DWORD nKey = 0;
	UCHAR d;

	(*c)=1;
	while((d=*lpName++)) {
		nKey = (nKey << 5) + HashData[d];
		nKey = (nKey&0x1ffff) + (nKey>>17);
		(*c)++;
	}
	return (nKey&0x1ffff);
}
/*-------------------------------------------------------------------------*/
void InitHash( void )
//
//	init Hash
//
{
	int c;

	for( c=0;c<HASHTABLESIZE;c++ ) HashTable[c]=NULL;
}

/*---------------------------------------------------------------------------
		regist token
/*-------------------------------------------------------------------------*/
UINT GetTokenNum( UCHAR *lpName, int *lpNum )
//
//	token の番号を拾得する
//
//(out)	UINT   = NUM number
//		*lpNum = regist number
//
{
#if ASM_FLAG
	UCHAR *lp;
	UINT n;
//	dw4 = timeGetTime();

//	printf( "%s:",lpName );

	lp = HashTable[GetHashKey( lpName )];
	if( lp == NULL ) return NUM_UNDEFINED;

	_asm {
		mov		[n],NUM_UNDEFINED
		mov		ebx,[lpName]
		mov		al,[ebx]
		cmp		al,'#'
		je		next02
		cmp		al,'_'
		je		next02
		cmp		al,'A'
		jb		exitall
		cmp		al,'z'
		ja		exitall
		cmp		al,'Z'
		jb		next02
		cmp		al,'a'
		jb		exitall
next02:

		push	esi
		push	edi


		mov		edx,[lp]

loop00:	cmp		dword ptr [edx+HASH_DATA],NUM_UNDEFINED
		je		next00

		mov		esi,ebx
		lea		edi,[edx+HASH_STR]
loop01:	mov		al,[esi]
		inc		esi
		cmp		al,[edi]
		jne		next00
		inc		edi
		cmp		al,0
		jne		loop01

		mov		eax,[edx+HASH_DATA]
		mov		ebx,[edx+HASH_NUM]
		mov		ecx,[lpNum]
		mov		[n],eax
		mov		[ecx],ebx
		jmp		next01

next00:	mov		edx,[edx]
		cmp		edx,0
		jne		loop00
next01:	pop		edi
		pop		esi
exitall:
	}
//	printf( "%d\n",n );
//	dw3+=timeGetTime()-dw4;
	return n;
#else
	int c;
	UINT n;
	UCHAR *lp;

	// 空白などの無効な物を飛ばす
	n = *(UINT*)(lpName);
	if(  n == (UINT)' ' + (UINT)0x0d*0x100 + (UINT)0x0a*0x10000 + (UINT)0x00*0x1000000 ) {
		return NUM_UNDEFINED;
	}
	if(  n == (UINT)'\t' + (UINT)0x0d*0x100 + (UINT)0x0a*0x10000 + (UINT)0x00*0x1000000 ) {
		return NUM_UNDEFINED;
	}
	n &= 0xffffff;
	if(  n == (UINT)0x0d + (UINT)0x0a*0x100 + (UINT)0x00*0x10000 ) {
		return NUM_UNDEFINED;
	}
	n &= 0xffff;
	if(  n == (UINT)0x0d + (UINT)0x00*0x100 ) {
		return NUM_UNDEFINED;
	}
	if(  n == (UINT)0x0a + (UINT)0x00*0x100 ) {
		return NUM_UNDEFINED;
	}
	if(  n == (UINT)' ' + (UINT)0x00*0x100 ) {
		return NUM_UNDEFINED;
	}
	if(  n == (UINT)'\t' + (UINT)0x00*0x100 ) {
		return NUM_UNDEFINED;
	}
	n &= 0xff;
	if(  n == 0 ) {
		return NUM_UNDEFINED;
	}


	lp = HashTable[GetHashKey( lpName )];
	if( lp == NULL ) return NUM_UNDEFINED;

	while( 1 ) {
		if( *(INT*)(lp+HASH_DATA) != NUM_UNDEFINED && strcmp( lpName,lp+HASH_STR ) == 0 ) {
			*lpNum = *(INT*)(lp+HASH_NUM);
			return *(INT*)(lp+HASH_DATA);
		}
		lp = *(UCHAR**)(lp);
		if( lp == NULL ) return NUM_UNDEFINED;
	}
#endif
}
/*-------------------------------------------------------------------------*/
BYTE* GetTokenStr( UINT num )
//
//	token の文字列を拾得する ( 番号指定 )
//
{
	UCHAR *lp;

	if( num >= MAX_REGISTNUM || num < 0 ) {
		err_pre0(1);
		printf( "preprocessor error:登録されていないものを削除しようとしました\n" );
		return NULL;
	}

	lp = ((lpRegist[num]) + HASH_STR);
	return lp;
}
/*-------------------------------------------------------------------------*/
void UndefTokenNum( UINT num )
//
//	token を削除 ( 番号指定 )
//
{
	UCHAR *lp;

	if( num >= MAX_REGISTNUM || num < 0 ) {
		err_pre0(1);
		printf( "preprocessor error:登録されていないものを削除しようとしました\n" );
		return;
	}

	if( (*(INT*)(lpRegist[num]+HASH_DATA)) >= NUM_TOP && (*(INT*)(lpRegist[num]+HASH_DATA)) <= NUM_END ) {
		err_pre0(1);
		printf( "preprocessor error:\"%s\" はキーワードです。削除できません\n", lpRegist[num]+HASH_STR );
		return;
	}
	*(INT*)(lpRegist[num]+HASH_DATA) = NUM_UNDEFINED;


	// 直前に同じ hashkeyに登録した token が無効な物しかない場合、チェーンから切り取る
	// 逆に重くなる場合、以下のルーチンを削除する
	if( *(UCHAR**)(lpRegist[num])==NULL) return;
	lp = lpRegist[num];
	while( *(UCHAR**)lp != NULL ) {
		lp = *(UCHAR**)lp;
	}

	while( 1 ) {
		if( *(int*)(lp+HASH_DATA) == NUM_UNDEFINED ) {
			lp = *(UCHAR**)(lp+HASH_NEXT);
			if( lp==lpRegist[num] ) {
				*(UCHAR**)(lpRegist[num]) = NULL;
				return;
			} else {
				*(UCHAR**)(lp) = NULL;
			}
		} else {
			return;
		}
	}
}
/*-------------------------------------------------------------------------*/
void UndefToken( UCHAR *lpName )
//
//	token を削除
//
{
	UINT num;

	if( GetTokenNum( lpName, &num ) == NUM_UNDEFINED ) {
		err_pre0(1);
		printf( "preprocessor error:\"%s\" は登録されていないので削除できません\n", lpName );
		return;
	}
	UndefTokenNum( num );
	return;
}
/*-------------------------------------------------------------------------*/
void CheckRegistToken( UCHAR *lpName )
{
	UINT c,num1;

//	dw1++;

	if( cDefine >= NUM_DEFINED_END ) {
		err_pre0(1);
		printf( "preprocessor error:#define/#macro/#fucntionの個数が多すぎます(最大%dbytes)\n", NUM_DEFINED_END-NUM_DEFINED_TOP+1 );
		return;
	}
	if( lpDefineEnd > DefineBuffer+MAX_DEFINEBUFF-MAX_MACROEXP*2 ) {		// macro buffer が破綻しないか？
		err_pre0(1);
		printf( "preprocessor error:#define/#macro/#functionのbufferが一杯です(最大%dbytes)\n", MAX_DEFINEBUFF );
		return;
	}

	if( *lpName != '#' && __iscsymf( *lpName ) == 0 ) {
		err_pre0(1);
		printf( "preprocessor error:\"%s\" は登録できません\n", lpName );
		return;
	}

	//
	//	すでに登録済みの時は、削除して、waring を表示、そして再定義
	//
	if( ( c = GetTokenNum( lpName, &num1 )) != NUM_UNDEFINED ) {
		if( c >= NUM_TOP && c <= NUM_END ) {
			err_pre0(1);
			printf( "preprocessor error:\"%s\" はキーワードです。再定義できません\n", lpName );
			return;
		}
		err_pre0(0);
		printf( "preprocessor warning:すでに定義済みの \"%s\" を再定義しようとしています\n", lpName );
		UndefTokenNum( num1 );
	}

	//
	//	token
	//
	if( cRegist >= MAX_REGISTNUM ) {
		err_pre0(1);
		printf( "preprocessor error:#define/#macro/#functionの数が多すぎて(最大%d個)\"%s\"を定義出来ませんでした\n", MAX_REGISTNUM, lpName );
		return;
	}
}
/*-------------------------------------------------------------------------*/
void RegistToken( UCHAR *lpName, UINT num0 )
//
//	token を登録する
//
{
	UINT s,nKey;
	UCHAR *lp;

//	dw3++;


	//
	//	regist token
	//
	nKey = GetHashKeyNum( lpName,&s );
	if( lpRegistEnd + s >= RegistBuffer + maxRegistbuff-16 ) {
		err_pre0(1);
		printf( "preprocessor error:macro の定義名の buffer が一杯で定義できません ( 最大 %d bytes )\n", maxRegistbuff );
		return;
	}
	if( HashTable[nKey]!=NULL ) {				// 直前の同じhashkeyのデーターに 今回のデーターへの pointer を登録
		*((UCHAR**)(HashTable[nKey]+HASH_NEXT)) = lpRegistEnd;
	}
	lp = lpRegist[cRegist] = lpRegistEnd;		// token(string) の先頭address を登録
	(*(UCHAR**)lpRegistEnd) = HashTable[nKey];	// 直前の 同じhashkeyのデーターへの pointer を登録
	lpRegistEnd+=4;
	(*(UCHAR**)lpRegistEnd) = NULL;				// 次の 同じhashkeyのデーターへの pointer(NULL) を登録
	lpRegistEnd+=4;
	(*(UINT*)lpRegistEnd) = num0;				// token 固有番号 を登録
	lpRegistEnd+=4;
	(*(UINT*)lpRegistEnd) = cRegist;			// token を登録するための通し番号
	lpRegistEnd+=4;
	strcpy( lpRegistEnd, lpName );				// token(string) を転送
	lpRegistEnd+=s;
	HashTable[nKey] = lp;
	cRegist++;
}
/*---------------------------------------------------------------------------
		put token
---------------------------------------------------------------------------*/
void RegistDefineNum( UCHAR *lpName, int d )
{
//
//		数値を define 登録
//
	int c;
	UCHAR *lpMax;

	if( GetTokenNum( lpName, &c ) != NUM_UNDEFINED ) {
		UndefTokenNum( c );
	}
	lpMax = lpDefineEnd;
	CheckRegistToken( lpName );
	RegistToken( lpName, cDefine );

	lpDefine[cRegist-1] = lpDefineEnd;
	((int*)lpDefineEnd)+=MACROPARA;
	itoa( d, lpDefineEnd, 10 );
	lpDefineEnd += ( strlen( lpDefineEnd ) + 1 );
	*(((int*)lpDefine[cRegist-1])) = 0;					// type define
	*(((int*)lpDefine[cRegist-1])+1) = 0;
	cDefine++;
	if( lpDefineEnd-lpMax > MAX_MACROEXP ) {		// macro buffer が破綻しないか？
		err_pre0(1);
		printf( "preprocessor error:１個の macro としては、大きすぎます ( 最大 %d bytes )\n", MAX_MACROEXP );
	}
}
/*-------------------------------------------------------------------------*/
void RegistDefineStr( UCHAR *lpName, UCHAR *lpDef )
{
//
//		str を define 登録
//
	int c;
	UCHAR *lpMax;

	if( GetTokenNum( lpName, &c ) != NUM_UNDEFINED ) {
		UndefTokenNum( c );
	}
	lpMax = lpDefineEnd;
	CheckRegistToken( lpName );
	RegistToken( lpName, cDefine );

	lpDefine[cRegist-1] = lpDefineEnd;
	((int*)lpDefineEnd)+=MACROPARA;
	strcpy( lpDefineEnd,lpDef );
	lpDefineEnd += ( strlen( lpDefineEnd ) + 1 );
	*(((int*)lpDefine[cRegist-1])) = 0;					// type define
	*(((int*)lpDefine[cRegist-1])+1) = 0;
	cDefine++;
	if( lpDefineEnd-lpMax > MAX_MACROEXP ) {		// macro buffer が破綻しないか？
		err_pre0(1);
		printf( "preprocessor error:１個の macro としては、大きすぎます ( 最大 %d bytes )\n", MAX_MACROEXP );
	}
}
/*-------------------------------------------------------------------------*/
void puttoken( UCHAR *lp )
//
//	puttoken
//
{
	int c,d;
//	dw6 = timeGetTime();

	if( CalcMode != 0 ) {
		while( _iscalc(*lp)) {
			CalcPutToken[nCalcPutToken]=*lp++;
			nCalcPutToken++;
		}
		if( *lp == 0 ) {
//	dw5+=timeGetTime()-dw6;
			return;
		}
		CalcPutToken[nCalcPutToken] = 0;
		d = Calc( CalcPutToken,0 );
		switch( CalcMode ) {
			case 1:					// #calc
				RegistDefineNum( CalcRegist, d );
				break;
			case 2:					// #if
				if( d != 0 ){
					set_if( 0 );
					if_use[nest]=1;
				} else {
					set_if( 1 );
					if_use[nest]=0;
				}
				break;
			case 3:					// #elif
				if( d != 0 ){
					set_elif( 0 );
					if_use[nest]=1;
				} else {
					set_elif( 1 );
					if_use[nest]=0;
				}
				break;
			case 4:					// #pushnum
				if ( d >= MAX_NUMSTACK ) {
					err_pre0(1);
					printf( "preprocessor error:#pushnum の stack が overflow しました\n" );
				} else {
					num_stack[num_sp++] = d;
				}
				break;
			case 5:					// #getnum
				d = num_sp-d;
				if( d < 0 || d >= MAX_NUMSTACK ) {
					err_pre0(1);
					printf( "preprocessor error:#getnum の stack の範囲が異常です\n" );
				} else {
					RegistDefineNum( CalcRegist, num_stack[d] );
				}
				break;
			case 6:					// #addnumsp
				if ( (num_sp+d) < 0 || (num_sp+d) >= MAX_NUMSTACK ) {
					err_pre0(1);
					printf( "preprocessor error:#addnumsp で stack が overflow しました\n" );
				} else {
					num_sp+=d;
				}
				break;
			case 7:					// #subnumsp
				if ( (num_sp-d) < 0 || (num_sp-d) >= MAX_NUMSTACK ) {
					err_pre0(1);
					printf( "preprocessor error:#subnumsp で stack が overflow しました\n" );
				} else {
					num_sp-=d;
				}
				break;
			case 8:					// #searchnum
				c = num_sp;
				c--;
				while( 1 ) {
					if( c<0 ) {
						RegistDefineNum( CalcRegist, 65535 );
						break;
					}
					if( num_stack[c]==d ) {
						RegistDefineNum( CalcRegist, num_sp-c );
						break;
					}
					c--;
				}
				break;
			case 9:					// #getstr
				d = str_sp-d;
				if( d < 0 || d >= MAX_STRSTACK ) {
					err_pre0(1);
					printf( "preprocessor error:#getstr の stack の範囲が異常です\n" );
				} else {
					RegistDefineStr( CalcRegist, str_stack[d] );
				}
				break;
			case 10:				// #addstrsp
				if ( (str_sp+d) < 0 || (str_sp+d) >= MAX_STRSTACK ) {
					err_pre0(1);
					printf( "preprocessor error:#addstrsp で stack が overflow しました\n" );
				} else {
					str_sp+=d;
				}
				break;
			case 11:				// #substrsp
				if ( (str_sp-d) < 0 || (str_sp-d) >= MAX_STRSTACK ) {
					err_pre0(1);
					printf( "preprocessor error:#substrsp で stack が overflow しました\n" );
				} else {
					str_sp-=d;
				}
				break;
			case 12:				// #openfile
				if ( d >= MAXPUTFILE ) {
					err_pre0(1);
					printf( "preprocessor error:#openfile の file 番号が %d 以上です\n",MAXPUTFILE );
				} else {
					if( putfile[d]!= NULL ) fclose( putfile[d] );
					if(( putfile[d] = fopen( CalcRegist, "a" )) == NULL) {
						err_pre0(1);
						printf( "preprocessor error:#openfile(%s) が出来ません\n",CalcRegist );
					}
				}
				break;
			case 13:				// #putfile
				if ( d >= MAXPUTFILE ) {
					err_pre0(1);
					printf( "preprocessor error:#putfile の file 番号が %d 以上です\n",MAXPUTFILE );
				} else {
					if( putfile[d]!= NULL ) {
						fputs( CalcRegist,putfile[d] );
					} else {
						err_pre0(1);
						printf( "preprocessor error:#putfile(%s) が出来ません\n",CalcRegist );
					}
				}
				break;
			case 14:				// #closefile
				if ( d >= MAXPUTFILE ) {
					err_pre0(1);
					printf( "preprocessor error:#closefile の file 番号が %d 以上です\n",MAXPUTFILE );
				} else {
					if( putfile[d]!= NULL ) {
						fclose( putfile[d] );
						putfile[d] = NULL;
					}
				}
				break;
			case 15:				// #crfile
				if ( d >= MAXPUTFILE ) {
					err_pre0(1);
					printf( "preprocessor error:#crfile の file 番号が %d 以上です\n",MAXPUTFILE );
				} else {
					if( putfile[d]!= NULL ) {
						fprintf( putfile[d],"\n" );
					} else {
						err_pre0(1);
						printf( "preprocessor error:#crfile が出来ません\n" );
					}
				}
				break;
		}
		CalcMode = 0;
		nCalcPutToken = 0;
	}

	// 出力
	if( lpPre > &PreBuff[PREBUFFERSIZE-PREBUFFERLIMMIT] ) {
		fwrite(PreBuff, lpPre-PreBuff, 1, pFile);
		lpPre = PreBuff;
	}
	while( *lp!= 0 ) {
		*lpPre++ = *lp++;
	}
//	fputs( lp, pFile );
//	dw5+=timeGetTime()-dw6;
}
/*---------------------------------------------------------------------------
		esc
---------------------------------------------------------------------------*/
int GetEscCode( void )
{
	int c;
#if USE_OCT
	int d;
#endif
	UINT num0, num1;

	switch( c = GetCharDataOne() ) {
		case 'a':
			c = 0x07;
			break;
		case 'b':
			c = 0x08;
			break;
		case 'f':
			c = 0x0c;
			break;
		case 'n':
			c = '\n';
			break;
		case 'r':
			c = 0x0d;
			break;
		case 't':
			c = '\t';
			break;
		case 'v':
			c = 0x0b;
			break;
		case '\'':
		case '"':
		case '\\':
		case '?':
		case ' ':
			break;
#if USE_OCT
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
			d = 0;
			num0 = isoct[c];
			while( d < 2 ) {
				if( (num1 = isoct[c = GetCharDataOne()]) == -1 ) {
					UnGetCharDataOne( c );
					break;
				}
				num0 = ( num0 << 3 ) | num1;
				d++;
			}
			c = num0;
			break;
#endif
		case 'x':
			num0 = 0;
			while( ( num1 = _ishex(c = GetCharDataOne())) != -1 ) {
				num0 = ( num0 << 4 ) | num1;
			}
			UnGetCharDataOne( c );
			c = num0;
			break;
		case '\n':
			break;
		default:
			err_pre0(0);
			printf( "preprocessor warning gesc:esc code( %d )\n", c );
	}
	return c;
}
/*-------------------------------------------------------------------------*/
UCHAR* SetEsc0( UCHAR* lpToken )
{
	int c;
#if USE_OCT
	int d;
#endif

	switch( c = GetCharDataOne() ) {
		case '>':						// コンパイラーへは、"\<","\>" の場合、"<", ">" へ変換して渡す
		case '<':
		case '}':
		case '{':
		case '#':
			*lpToken++ = c;
			break;
		case 'a':						// コンパイラーへは、"\" が付いた状態のまで渡す
		case 'b':
		case 'f':
		case 'n':
		case 'r':
		case 't':
		case 'v':
		case '\'':
		case '"':
		case '\\':
		case '?':
		case ' ':
			*lpToken++ = '\\';
			*lpToken++ = c;
			break;
#if USE_OCT
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
			*lpToken++ = '\\';
			d = 0;
			*lpToken++ = c;
			while( d < 2 ) {
				if( isoct[c = GetCharDataOne()] == -1 ) {
					UnGetCharDataOne( c );
					break;
				}
				*lpToken++ = c;
				d++;
			}
			break;
#endif
		case 'x':
			*lpToken++ = '\\';
			while( _ishex(c = GetCharDataOne()) != -1 ) {
				*lpToken++ = c;
			}
			UnGetCharDataOne( c );
			break;
		case '\n':
			*lpToken++ = '\\';
			break;
		default:
			*lpToken++ = '\\';
			err_pre0(0);
			printf( "preprocessor warning esc0:esc code( %d )\n", c );
			*lpToken++ = c;
			break;
	}
	return lpToken;
}
/*-------------------------------------------------------------------------*/
UCHAR* SetEsc1( UCHAR* lpToken )
{
//
//	SetEsc0 を、esc 文字解析して、そのまま出力して、ポインターを進めるだけのバーション
//
	int c;
#if USE_OCT
	int d;
#endif

	switch( c = GetCharDataOne() ) {
		case '>':						// コンパイラーへは、"\<", "\>", "\#" の場合、"<", ">", "#" へ変換して渡す
		case '<':
		case '}':
		case '{':
		case '#':
//			*lpToken++ = '\\';
//			*lpToken++ = c;
//			break;
		case 'a':						// コンパイラーへは、"\" が付いた状態のまで渡す
		case 'b':
		case 'f':
		case 'n':
		case 'r':
		case 't':
		case 'v':
		case '\'':
		case '"':
		case '\\':
		case '?':
		case ' ':
			*lpToken++ = '\\';
			*lpToken++ = c;
			break;
#if USE_OCT
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
			*lpToken++ = '\\';
			d = 0;
			*lpToken++ = c;
			while( d < 2 ) {
				if( isoct[c = GetCharDataOne()] == -1 ) {
					UnGetCharDataOne( c );
					break;
				}
				*lpToken++ = c;
				d++;
			}
			break;
#endif
		case 'x':
			*lpToken++ = '\\';
			while( _ishex((c = GetCharDataOne())) != -1 ) {
				*lpToken++ = c;
			}
			UnGetCharDataOne( c );
			break;
		case '\n':
			*lpToken++ = '\\';
			*lpToken++ = '\n';
			break;
		default:
			*lpToken++ = '\\';
			err_pre0(0);
			printf( "preprocessor warning esc1:esc code( %d )\n", c );
			*lpToken++ = c;
			break;
	}
	return lpToken;
}
/*-------------------------------------------------------------------------*/
void yen2esc( UCHAR *lpStr )
//
//	string 中の 1 個の yen mark を 2 個の yen mark ( esc seqence ) に変換する
//
{
	UCHAR buffer[YEN2ESC_BUFFER];
	UCHAR *lpBuffer,*lpWrite,c;

	lpWrite = lpStr;
	lpBuffer = buffer;
	while( (c = *lpWrite++) != 0 ) {
//		if( _ismbblead( c ) ) {
		if( _iskanji(c) ) {
			*lpBuffer++ = c;
			*lpBuffer++ = *lpWrite++;
		} else if( c == '\\' ) {
			*lpBuffer++ = '\\';
			*lpBuffer++ = '\\';
		} else {
			*lpBuffer++ = c;
		}
	}
	*lpBuffer++ = 0;
	strcpy( lpStr, buffer );
}
//---------------------------------------------------------------------------
UCHAR* esc2yen( UCHAR *lp0 )
{
//
//	string 中の 2 個の yen mark  ( esc seqence )  を 1 個の yen mark に変換する
//
	UCHAR c,*lpOut,*lpOrg;

	lpOrg=lpOut=lp0;
	if( *lp0!='"' ) {
		err_pre0(1);
		printf( "preprocessor error:文字列の最初の \" が見つかりません\n" );
		return NULL;
	}
	lp0++;
	while( 1 ) {
		c = *lp0;
		if( c == '"' ) {
			*lpOut=0;
			return lpOrg;
		} else if( c == 0 ) {
			err_pre0(1);
			printf( "preprocessor error:文字列の最後の \" が見つかりません\n" );
			return NULL;
		} else if( c == '\\' ) {
			lp0++;
			switch( c = *lp0 ) {
#if 0
				case 'a':						// コンパイラーへは、"\" が付いた状態のまで渡す
				case 'b':
				case 'f':
				case 'n':
				case 'r':
				case 't':
				case 'v':
					*lpOut++ = '\\';
					err_compiler(0);
					printf( "compiler warning:esc code( %d )\n", c );
					err_next_compiler();
					break;
#endif
				case '\'':						// \ を取って、後に続く記号だけにする
				case '"':
				case '\\':
				case '?':
				case ' ':
//;;;make
				case '0':
					*lpOut++ = c;
					lp0++;
					break;
				default:						// それ以外の場合、\ を出力して warning
					*lpOut++ = '\\';
					err_pre0(0);
					printf( "preprocessor warning:esc code( %d )\n", c );
					break;
			}
		} else if( _ismbblead( c )) {
			*lpOut++ = c;
			lp0++;
			c = *lp0;
			if( _ismbbtrail( c )) {
				*lpOut++ = c;
				lp0++;
			}
		} else {
			*lpOut++ = c;
			lp0++;
		}
	}
}
/*---------------------------------------------------------------------------
		get token
---------------------------------------------------------------------------*/
int GetTokenSub0x04( void )
//
//		コメントを無視 ( "//", "/*"-"*/" )
//
{
	int c,d;
	c = GetCharDataOne();
	if( c == '/' ) {
		do {
			c = GetCharDataOne();
		} while( c != 0x1a && c != '\n' );
		UnGetCharDataOne( c );
		return 1;
	} else if ( c == '*' ) {
		c = ' ';
		d = ' ';
		while( c != 0x0a && ( c != '/' || d != '*' ) ) {
			d = c;
			c = GetCharDataOne();
			if( c == '\n' ) {
				if( nMacroNest > FileData[NowFile].nMacroNest ){
					MacroLine[nMacroNest]++;
				} else {
					FileData[NowFile].line++;
				}
//			} else if( _iskanji(c) ) {
//				GetCharDataOne();				// 空読み
			}
		}
		return 1;
	}
	UnGetCharDataOne( c );
	return 0;
}
/*-------------------------------------------------------------------------*/
void GetTokenSub0x0e( void )
//
//		コメントを無視 ( ";" )
//
{
	int c;
	do {
		c = GetCharDataOne();
	} while( c != 0x1a && c != '\n' );
	UnGetCharDataOne( c );
}
/*-------------------------------------------------------------------------*/
UCHAR* GetTokenSub0x05( UCHAR* lpToken, int mode, int gnum, int nMacLocal, UCHAR **lpPointer )
//
//	文字列を token にする
//
{
	UCHAR macrobuffer[MAX_MACROEXP*4];
	int c,c_bak,f,d,t;
	UCHAR *lp0,*lp1;
	UINT num;
	UCHAR token[MAXTOKEN];
//;;;make MAX_MACROEXP

	c = '"';
	*lpToken++ = c;


	if( if_flag[nest] == 0 ) {
		do {
			f = 0;
			// buffer ベタでへ展開しながら、マクロの展開。マクロ展開した場合、f=1 にする
			lp0 = macrobuffer;
//;;;make ここでの出力のチェック、展開を何度も行う
			while( 1 ) {
				c = GetCharDataOne();
				if( c == '"' ) {
					*lp0++ = '"';
					*lp0++ = 0;
					// その後、buffer から 読み込み buffer へ戻す
					UnGetStrData( macrobuffer );
					break;
				} else if( c == 0x1a ) {
					err_pre0(1);
					printf( "preprocessor error:\" が閉じる前に eof があります\n" );
					err_exit();
				} else if( c == 0x0a ) {
					err_pre0(1);
					printf( "preprocessor error:\" が閉じる前に改行があります\n" );
					err_exit();
				} else if ( c=='<' || c=='{' ) {
					c_bak = c;
					lp1 = lp0;
					*lp0++ = c;
					while ( 1 ) {
						c = GetCharDataOne();
						if( c == 0x1a ) {
							err_pre0(1);
							printf( "preprocessor error:< が閉じる前に eof があります\n" );
							err_exit();
						} else if( c == 0x0a ) {
							err_pre0(1);
							printf( "preprocessor error:< が閉じる前に改行があります\n" );
							err_exit();
						} else if( c=='"' ) {
							err_pre0(1);
							printf( "preprocessor error:< が閉じる前に \" があります\n" );
							break;
						} else if( c=='<' || c=='{' ) {
							lp1 = lp0;
							*lp0++ = c_bak;
							c_bak = c;
						} else if( c=='>' || c=='}' ) {
							if( c=='>' && c_bak=='{' ) {
								err_pre0(1);
								printf( "preprocessor error:> と { が交差しています\n" );
								break;
							} else if( c=='}' && c_bak=='<' ) {
								err_pre0(1);
								printf( "preprocessor error:< と } が交差しています\n" );
								break;
							}
							*lp1 = 0;
							*lp0 = 0;
							// local label として、チェックする
							if( lpPointer != NULL ) {
								// local define で登録されているか
								for( t=0; t<nMacLocal; t++ ) {
									if( strcmp( lp1+1,lpPointer[t] ) == 0 ) {
										sprintf( token, CCNAME"M__%d__%s",gnum,lp1+1 );
										*lp1++ = c_bak;
										strcpy( lp1, token );
										lp1 += strlen( token );
										*lp1++ = c;
										*lp1 = 0;
										lp0 = lp1;
										break;
									}
								}
								if( t == nMacLocal ) {
									*lp0++ = c;
									*lp1 = c_bak;
								}
							} else {
								f = 1;
								// #define で登録されているか
								if( GetTokenNum( lp1+1, &num ) == NUM_UNDEFINED ) {
									err_pre0(1);
									printf( "preprocessor error:\"%s\" は登録されていません\n", lp1+1 );
									break;
								}
								if( *(((int*)lpDefine[num])) > 1 ) {
									err_pre0(1);
									printf( "preprocessor error:\"%s\" は #define/#adefine で登録されたものではありません\n", lp1+1 );
									break;
								}
								// macrobuffer へ出力して、読み込み buffer へ戻す
								ExpDefine( lp0+1, lp1+1, num, &d );

								// 2 重登録の展開
								if( c=='>' && GetTokenNum( lp0+1,&num ) != NUM_UNDEFINED && *(((int*)lpDefine[num])) <= 1 ) {
									UnGetCharDataOne( '>' );
									UnGetStrData( lp0+1 );
									UnGetCharDataOne( '<' );
								} else {
									UnGetStrData( lp0+1 );
								}
								// 直前までを、読み込み buffer へ戻す
								UnGetStrData( macrobuffer );
								lp0 = macrobuffer;
								// macrobuffer:直前までのデーター
								//       lp1+1:token
								//       lp0+1:defined
							}
							break;
						} else if( _iskanji(c) ) {
							*lp0++ = c;
							*lp0++ = GetCharDataOne();
						} else {
							*lp0++ = c;
						}
					}
//;;;make
				} else if( mode == MODE_NORMAL && c == '\\' ) {
					lp0 = SetEsc1( lp0 );
				} else if( _iskanji(c) ) {
					*lp0++ = c;
					*lp0++ = GetCharDataOne();
				} else {
					*lp0++ = c;
				}
			}
		} while( f == 1 );
	}


//	while( c == '"' ) {
		//
		//	put string
		//
		while(( c = GetCharDataOne() ) != '"' ) {
			if( c == 0x1a ) {
				err_pre0(1);
				printf( "preprocessor error:\" が閉じる前に eof があります\n" );
				err_exit();
			} else if( mode == MODE_NORMAL && c == '\\' ) {
				lpToken = SetEsc0( lpToken );
			} else if( _iskanji(c) ) {
				*lpToken++ = c;
				*lpToken++ = GetCharDataOne();
			} else {
				*lpToken++ = c;
			}
		}
//		//
//		//	skip space/tab
//		//
//		do {
//			c = GetCharDataOne();
//		} while( c == ' ' || c == '\t' );
//	}
//	UnGetCharDataOne( c );

	*lpToken++ = '"';

	return lpToken;
}
/*-------------------------------------------------------------------------*/
UCHAR* GetTokenSub0x06( UCHAR* lpToken, int mode )
//
//	文字定数の展開
//
{
	int c,d;
	UINT num0;

	d = 0;
	num0 = 0;
	while(( c = GetCharDataOne() ) != '\'' ) {
		if( mode == MODE_NORMAL && c == '\\' )	c = GetEscCode();
		if( c == 0x1a ) {
			err_pre0(1);
			printf( "preprocessor error:' が閉じる前に eof があります\n" );
			err_exit();
		}
		if( _iskanji(c) ){
			num0 = ( num0 << 16 ) | c + (GetCharDataOne()<<8);
			d+=2;
		} else {
			num0 = ( num0 << 8 ) | c;
			d++;
		}
		if( d > 4 ) {
			err_pre0(1);
			printf( "preprocessor error:' に囲まれた文字が半角 4 文字以上です\n" );
			break;
		}
	}
	if( d == 0 ) {
		err_pre0(1);
		printf( "preprocessor error:' に囲まれた文字が空白です\n" );
	}
	itoa( num0, lpToken, 10 );
	lpToken += strlen( lpToken );
	return lpToken;
}
/*-------------------------------------------------------------------------*/
UCHAR* GetTokenSub0x07( UCHAR* lpToken )
//
//	'<' - '>' で囲まれた文字の処理 ( #include 時のみ )
//
{
	int c;

	*lpToken++ = '<';
	do {
		c = *lpToken++ = GetCharDataOne();
	} while( c != 0x1a && c != '>' );
	return lpToken;
}
/*-------------------------------------------------------------------------*/
UCHAR* GetTokenSub0x0a( UCHAR* lpToken, int c )
//
//	8,16 進数を 10 進数へ変換
//
{
	UINT num0, num1;

	if( c == '0' ) {
		c = GetCharDataOne();

		if( c == 'x'  ) {		// hex ( 16 進数 ) 0x???? の形
			num0 = 0;
			while( ( num1 = _ishex(c = GetCharDataOne())) != -1 ) {
				num0 = ( num0 << 4 ) | num1;
			}
			UnGetCharDataOne( c );
			itoa( num0, lpToken, 10 );
			lpToken += strlen( lpToken );
			return lpToken;
		} else if( c == 'n'  ) {		// bin ( 2 進数 ) 0n???? の形
			num0 = 0;
			while( ( num1 = _isbin(c = GetCharDataOne())) != -1 ) {
				num0 = ( num0 << 1 ) | num1;
			}
			UnGetCharDataOne( c );
			itoa( num0, lpToken, 10 );
			lpToken += strlen( lpToken );
			return lpToken;
		}
#if USE_OCT
		UnGetCharDataOne( c );		// oct ( 8 進数 )
		num0 = 0;
		while( ( num1 = isoct[c = GetCharDataOne()]) != -1 ) {
			num0 = ( num0 << 3 ) | num1;
		}
		UnGetCharDataOne( c );
		itoa( num0, lpToken, 10 );
		lpToken += strlen( lpToken );
		return lpToken;
#endif
	}

	UnGetCharDataOne( c );			// 10 進数
	num0 = 0;
	while( ( num1 = _isdec(c = GetCharDataOne())) != -1 ) {
		num0 = num0 * 10 + num1;
	}
	UnGetCharDataOne( c );
	itoa( num0, lpToken, 10 );
	lpToken += strlen( lpToken );
	return lpToken;
}
/*-------------------------------------------------------------------------*/
void SkipSpace( void )
//
//	space / comment を飛ばす ( lf は飛ばさない )
//
{
#if 0
           |00 - 3F|40 - 7E|7F|80|81 - 9F|A0 - DF|E0 - EF|F0|F1 - FC|FD - FF|
           |-------+-------+--+--+-------+-------+-------+--+-------+-------|
           |-------+-------+--+--+-------+-------+-------+--+-------+-------|
1バイト文字|---------------|             |-------|
  1st byte                       |-------|       |-------|==|
  2nd byte         |-------|  |-------------------------------------|
#endif

	int c;
	while( 1 ) {
		switch( c = GetCharDataOne() ){
			case '\t':
			case ' ':
				break;
			case ';':
				GetTokenSub0x0e();
				return;
			case '/':
				if( GetTokenSub0x04() == 1 ) {
					break;
				}
				UnGetCharDataOne( '/' );
				return;
			case -1:
			case 0x1a:
			case 0xff:
				return;

			default:
				UnGetCharDataOne( c );
				return;
		}
	}
}
/*-------------------------------------------------------------------------*/
UCHAR* GetTokenDefine( UCHAR* lpToken )
//
//	#define 文の token 拾得
//
{
	int c;
	UCHAR *lpRet,*lp0;

	lpRet = lpToken;
	lp0 = lpToken;

	while( 1 ) {
		c = GetCharDataOne();
		if( _iskanji(c) ){
			*lpToken++ = c;
			*lpToken++ = GetCharDataOne();
		} else {
			switch( TokenFirst[c] ) {
				case 0x01:						// ignore
					break;

				case 0x04:						// '/' "//" or "/*"
					if( GetTokenSub0x04() == 1 ) {
						*lpToken++ = ' ';
						break;
					}
					*lpToken++ = '/';
					break;
				case 0x05:						//  "
					lpToken = GetTokenSub0x05( lpToken, MODE_NORMAL,0,0,NULL );
					break;

				case 0x06:						// '
					lpToken = GetTokenSub0x06( lpToken, MODE_NORMAL );
					break;

				case 0x03:						// 'lf'
				case 0xff:						// eof
					UnGetCharDataOne( c );
					while( lp0<lpToken && ( *(lpToken-1) == ' ' || *(lpToken-1) == '\t' )) {
						lpToken--;
					}
					*lpToken++ = 0;
					return lpRet;

				case 0x00:						// throw
				case 0x02:						// err_main
				case 0x07:						// <
				case 0x08:						// name
				case 0x09:						// preprocessor command
				case 0x0a:						// num
				case 0x0b:						// 
				case 0x0e:						// ,
					*lpToken++ = c;
					break;

				case 0x0d:						// ;
					 GetTokenSub0x0e();
					*lpToken++ = ' ';
					break;

				case 0x0c:						// \ 
					*lpToken++ = c;
					c = GetCharDataOne();
					if( c == '\'' || c == '"' || c == ' ' ) {
						*lpToken++ = c;
						lp0 = lpToken;
					} else {
						UnGetCharDataOne( c );
					}
					break;


			}
		}
	}
}
/*-------------------------------------------------------------------------*/
UCHAR* GetTokenInclude( UCHAR* lpToken )
//
//	#include 文のパラメーターを拾得する
//
{
	int c;
	UCHAR* lpRet;

	lpRet = lpToken;

	while( 1 ) {
		c = GetCharDataOne();
		if( _iskanji(c) ){
			err_pre0(1);
			printf( "preprocessor error:#include のパラメーターが異常です\n" );
			return NULL;
		}
		switch( TokenFirst[c] ) {
			case 0x03:						// 'lf'
			case 0xff:						// eof
				UnGetCharDataOne( c );
				*lpToken++ = 0;
				return lpRet;

			case 0x00:						// throw
			case 0x0b:						// 
			case 0x0e:						// ,
			case 0x0c:						// \ 
				*lpToken++ = c;
				*lpToken++ = 0;
				return lpRet;

			case 0x0d:						// ;
				GetTokenSub0x0e();
				*lpToken++ = ' ';
				break;

			case 0x01:						// ignore
				break;

			case 0x02:						// err_main
				err_pre0(1);
				printf( "preprocessor error:syntax error code(%d)\n", c );
				break;

			case 0x04:						// '/' "//" or "/*"
				if( GetTokenSub0x04() == 1 ) {
					*lpToken++ = ' ';
					break;
				}
				*lpToken++ = '/';
				break;

			case 0x05:						//  "
				lpToken = GetTokenSub0x05( lpToken, MODE_INCLUDE,0,0,NULL );
				*lpToken++ = 0;
				return lpRet;

			case 0x06:						// '
				lpToken = GetTokenSub0x06( lpToken, MODE_INCLUDE );
				*lpToken++ = 0;
				return lpRet;

			case 0x07:						// <
				lpToken = GetTokenSub0x07( lpToken );
				*lpToken++ = 0;
				return lpRet;

			case 0x08:						// name
			case 0x09:						// preprocessor command
				*lpToken++ = c;
				while( _isname1(c = GetCharDataOne())) {
					*lpToken++ = c;
				}
				UnGetCharDataOne( c );
				*lpToken++ = 0;
				return lpRet;

			case 0x0a:						// num
				lpToken = GetTokenSub0x0a( lpToken, c );
				return lpRet;

		}
	}
	*lpToken++ = 0;
	return NULL;
}
/*-------------------------------------------------------------------------*/
void GetTokenSkipLf()
{
//
//		lf or EOF まで文字をスキップ
//
	int c;

	do {
		c = GetCharDataOne();
	} while( c!='\n' && c!=0x1a );
	UnGetCharDataOne( c );
}
/*-------------------------------------------------------------------------*/
#if ASM_FLAG
UCHAR* GetTokenSub0x08( UCHAR* );
#endif
UCHAR* GetToken( UCHAR* lpToken )
{
	int c;
	UCHAR* lpRet;

//;;;make
//	dw2 = timeGetTime();

	lpRet = lpToken;

	//
	//	全角文字が含まれるか
	//
	while( (c = GetCharDataOne()) != 0x1a ) {
		if( c == 0 ) {
			break;
		} else if( _iskanji(c) ){
			if( if_flag[nest] == 0 ) {
				if( mbcBuff[0]==0 ) {
					GetCharDataOne();				// 2byte 目の空読み
					err_pre0(1);
					printf( "preprocessor error:token に全角文字は使えません\n" );
					while( _iskanji(c=GetCharDataOne()) ){
						GetCharDataOne();				// 2byte 目の空読み
					}
					UnGetCharDataOne( c );
				} else {
					UnGetCharDataOne( c );
					strcpy( lpToken,mbcBuff );
					return lpRet;
				}
			} else {
				GetCharDataOne();				// 2byte 目の空読み
				fNowIf = 0;
				GetTokenSkipLf();
				*lpToken++ = 0;
//	dw1+=timeGetTime()-dw2;
				return lpRet;
			}
		} else {
			switch( TokenFirst[c] ) {
				case 0x03:						// 'lf'
					NextLine();
					fNowIf = 0;
				case 0x00:						// throw
				case 0x0b:						// 
				case 0x0e:						// ,
				case 0x0c:						// \ 
					*lpToken++ = c;
					*lpToken++ = 0;
//	dw1+=timeGetTime()-dw2;
					return lpRet;

				case 0x01:						// ignore
					break;

				case 0x02:						// err_main
					if( if_flag[nest] == 0 ) {
						err_pre0(1);
						printf( "preprocessor error:syntax error code(%d)\n", c );
					}
					break;

				case 0x0d:						// ;
					GetTokenSub0x0e();
					*lpToken++ = ' ';
					break;

				case 0x04:						// '/' "//" or "/*"
					if( if_flag[nest] == 0 ) {
						if( GetTokenSub0x04() == 1 ) {
							*lpToken++ = ' ';
							break;
						}
						*lpToken++ = '/';
						*lpToken++ = 0;
						return lpRet;
					} else if( fNowIf == 1 ) {
						*lpToken++ = '/';
					}
					break;

				case 0x05:						//  "
					if( if_flag[nest] == 0 ) {
						lpToken = GetTokenSub0x05( lpToken, MODE_NORMAL,0,0,NULL );
						*lpToken++ = 0;
					}
//	dw1+=timeGetTime()-dw2;
					return lpRet;

				case 0x06:						// '
					if( if_flag[nest] == 0 || fNowIf == 1 ) {
						lpToken = GetTokenSub0x06( lpToken, MODE_NORMAL );
						*lpToken++ = 0;
					}
//	dw1+=timeGetTime()-dw2;
					return lpRet;

				case 0x07:						// <
					if( if_flag[nest] == 0 || fNowIf == 1 ) {
						*lpToken++ = c;
						*lpToken++ = 0;
//	dw1+=timeGetTime()-dw2;
						return lpRet;
					}
//					if( if_flag[nest] == 0 || fNowIf == 1 ) {
//						*lpToken++ = '<';
//					}
//					break;


				case 0x09:						// preprocessor command
				case 0x08:						// name
					*lpToken++ = c;
//#if ASM_FLAG
//					lpToken = GetTokenSub0x08( lpToken );
//#else
					while( _isname1(c = GetCharDataOne())) {
						*lpToken++ = c;
					}
					UnGetCharDataOne( c );
					*lpToken++ = 0;
//#endif
//	dw1+=timeGetTime()-dw2;
					return lpRet;

				case 0x0a:						// num
					if( if_flag[nest] == 0 || fNowIf == 1 ) {
						lpToken = GetTokenSub0x0a( lpToken, c );
					}
//	dw1+=timeGetTime()-dw2;
					return lpRet;
			}
		}
	}
	*lpToken = 0;
//	dw1+=timeGetTime()-dw2;
	return NULL;
}
/*-------------------------------------------------------------------------*/
UCHAR* GetPara( UCHAR* lpStr, int type, int gnum )
//
//	パラメーターの拾得
//
{
	int d,nest;

	nest = 0;
	while( 1 ) {
		d = GetCharDataOne();
		if( d == 0 || d == 0x1a ) {
			if( type == 1 && nest == 0) {
				UnGetCharDataOne( d );
				return lpStr;
			}
			break;
		} else if( d == '(' ) {
//			if( type == 1 ) {
//				UnGetCharDataOne( d );
//				return lpStr;
//			}
			*lpStr++ = '(';
			nest++;
		} else if( d == ')' ) {
//			if( type == 1 ) {
//				UnGetCharDataOne( d );
//				return lpStr;
//			}
			if( nest == 0 ) {
				break;
			}
			*lpStr++ = ')';
			nest--;
		} else if( d == ',' ) {
			if( nest == 0 ) {
				break;
			}
			*lpStr++ = ',';
		} else if( d == '"' ) {
			lpStr = GetTokenSub0x05( lpStr, MODE_NORMAL,0,0,NULL );
		} else if( d == '\'' ) {
			lpStr = GetTokenSub0x06( lpStr, MODE_NORMAL );
		} else if( d == ';' ) {
			GetTokenSub0x0e();
			*lpStr++ = ' ';
		} else if( d == '/' ) {
			if( GetTokenSub0x04() == 1 ) {
				*lpStr++ = ' ';
			} else {
				*lpStr++ = '/';
			}
		} else if( _isname0(d) ) {
			*lpStr++ = d;
			while( _isname1(d = GetCharDataOne()) ) {
				*lpStr++ = d;
			}
			UnGetCharDataOne( d );
		} else if( d == '\n' ){				//	|| d == ' ' || d == '\t' ) {		// lf,space,tab
			if( type == 1 && nest == 0) {
				UnGetCharDataOne( d );
				return lpStr;
			}
			*lpStr++ = d;
		} else if( d == '\\' ) {
			d = GetCharDataOne();
			switch( d ) {
				case 'a':						// コンパイラーへは、"\" が付いた状態のまで渡す
				case 'b':
				case 'f':
				case 'n':
				case 'r':
				case 't':
				case 'v':
				case '\'':
				case '"':
				case '\\':
				case '?':
				case ' ':
//;;;make
				case '0':
					*lpStr++ = '\\';
					*lpStr++ = d;
					break;
				case '$':						// \$ の場合、skip
					break;
				default:
					UnGetCharDataOne( d );
					*lpStr++ = '\\';
			}
		} else if( _iskanji(d) ) {
			*lpStr++ = d;
			*lpStr++ = GetCharDataOne();
		} else { 
			*lpStr++ = d;
		}
	}
	UnGetCharDataOne( d );
	return lpStr;
}
/*---------------------------------------------------------------------------
		expression
---------------------------------------------------------------------------*/
void ExpDefineModeError( mode )
{
	if( mode != 0 ) {
		err_pre0(1);
		printf( "preprocessor error:#@ や # は macro のパラメーターの前に使用して下さい\n" );
	}
}
/*-------------------------------------------------------------------------*/
UCHAR* DeleteBeforeSpace( UCHAR* lpTop, UCHAR* lpStr, int mode )
{
	while( lpTop<lpStr && ( *(lpStr-1) == ' ' || *(lpStr-1) == '\t' )) {
		lpStr--;
	}
	return lpStr;
}
/*-------------------------------------------------------------------------*/
void ExpDefine__LINE__( UCHAR* lpBuffer )
//
//	__LINE__ を展開
//
{
	itoa( FileData[NowFile].line, lpBuffer, 10 );
}
/*-------------------------------------------------------------------------*/
void ExpDefine__FILE__( UCHAR* lpBuffer )
//
//	__FILE__ を展開
//
{
	if( _fullpath( lpBuffer+1, FileData[NowFile].filename , _MAX_PATH ) != NULL ) {
		yen2esc( lpBuffer+1 );
		lpBuffer[0]  = '"';
		strcat( lpBuffer, "\"" );
	} else {
		err_pre0(1);
		printf( "preprocessor error:__FILE__ が展開できません ( path = %s )\n", FileData[NowFile].filename );
	}
}
/*-------------------------------------------------------------------------*/
void ExpDefine__FILE__D( UCHAR* lpBuffer, int type )
{
	UCHAR path_buffer[_MAX_PATH];
//	UCHAR drive[_MAX_DRIVE];
	UCHAR dir[_MAX_DIR];
	UCHAR fname[_MAX_FNAME];
	UCHAR ext[_MAX_EXT];

	if( ( type & 2 ) == 2 ) {
		*lpBuffer++ = '"';
	}

	if( _fullpath( path_buffer, FileData[NowFile].filename , _MAX_PATH ) == NULL ) {
		err_pre0(1);
		printf( "preprocessor error:__FILE__%dD が展開できません ( path = %s )\n", type,FileData[NowFile].filename );
		return;
	}

	 _splitpath( path_buffer, lpBuffer, dir, fname, ext );

//	if( ( type & 1 ) == 1 ) {
//		yen2esc( lpBuffer );
//	}
	if( ( type & 2 ) == 2 ) {
		strcat( lpBuffer, "\"" );
	}
}
/*-------------------------------------------------------------------------*/
void ExpDefine__FILE__DP( UCHAR* lpBuffer, int type )
{
	UCHAR path_buffer[_MAX_PATH];
//	UCHAR drive[_MAX_DRIVE];
	UCHAR dir[_MAX_DIR];
	UCHAR fname[_MAX_FNAME];
	UCHAR ext[_MAX_EXT];

	if( ( type & 2 ) == 2 ) {
		*lpBuffer++ = '"';
	}

	if( _fullpath( path_buffer, FileData[NowFile].filename , _MAX_PATH ) == NULL ) {
		err_pre0(1);
		printf( "preprocessor error:__FILE__%dDP が展開できません ( path = %s )\n", type,FileData[NowFile].filename );
		return;
	}

	 _splitpath( path_buffer, lpBuffer, dir, fname, ext );
	strcat( lpBuffer, dir );

	if( ( type & 1 ) == 1 ) {
		yen2esc( lpBuffer );
	}
	if( ( type & 2 ) == 2 ) {
		strcat( lpBuffer, "\"" );
	}
}
/*-------------------------------------------------------------------------*/
void ExpDefine__FILE__DPF( UCHAR* lpBuffer, int type )
{
	UCHAR path_buffer[_MAX_PATH];
//	UCHAR drive[_MAX_DRIVE];
	UCHAR dir[_MAX_DIR];
	UCHAR fname[_MAX_FNAME];
	UCHAR ext[_MAX_EXT];

	if( ( type & 2 ) == 2 ) {
		*lpBuffer++ = '"';
	}

	if( _fullpath( path_buffer, FileData[NowFile].filename , _MAX_PATH ) == NULL ) {
		err_pre0(1);
		printf( "preprocessor error:__FILE__%dDPF が展開できません ( path = %s )\n", type,FileData[NowFile].filename );
		return;
	}

	 _splitpath( path_buffer, lpBuffer, dir, fname, ext );
	strcat( lpBuffer, dir );
	strcat( lpBuffer, fname );

	if( ( type & 1 ) == 1 ) {
		yen2esc( lpBuffer );
	}
	if( ( type & 2 ) == 2 ) {
		strcat( lpBuffer, "\"" );
	}
}
/*-------------------------------------------------------------------------*/
void ExpDefine__FILE__DPFE( UCHAR* lpBuffer, int type )
{
	if( ( type & 2 ) == 2 ) {
		*lpBuffer++ = '"';
	}
	if( _fullpath( lpBuffer, FileData[NowFile].filename , _MAX_PATH ) != NULL ) {
		if( ( type & 1 ) == 1 ) {
			yen2esc( lpBuffer );
		}
	} else {
		err_pre0(1);
		printf( "preprocessor error:__FILE__%dDPFE が展開できません ( path = %s )\n", type, FileData[NowFile].filename );
		return;
	}
	if( ( type & 2 ) == 2 ) {
		strcat( lpBuffer, "\"" );
	}
}
/*-------------------------------------------------------------------------*/
void ExpDefine__FILE__P( UCHAR* lpBuffer, int type )
{
	UCHAR path_buffer[_MAX_PATH];
	UCHAR drive[_MAX_DRIVE];
//	UCHAR dir[_MAX_DIR];
	UCHAR fname[_MAX_FNAME];
	UCHAR ext[_MAX_EXT];

	if( ( type & 2 ) == 2 ) {
		*lpBuffer++ = '"';
	}

	if( _fullpath( path_buffer, FileData[NowFile].filename , _MAX_PATH ) == NULL ) {
		err_pre0(1);
		printf( "preprocessor error:__FILE__%dP が展開できません ( path = %s )\n", type,FileData[NowFile].filename );
		return;
	}

	 _splitpath( path_buffer, drive, lpBuffer, fname, ext );

	if( ( type & 1 ) == 1 ) {
		yen2esc( lpBuffer );
	}
	if( ( type & 2 ) == 2 ) {
		strcat( lpBuffer, "\"" );
	}
}
/*-------------------------------------------------------------------------*/
void ExpDefine__FILE__PF( UCHAR* lpBuffer, int type )
{
	UCHAR path_buffer[_MAX_PATH];
	UCHAR drive[_MAX_DRIVE];
//	UCHAR dir[_MAX_DIR];
	UCHAR fname[_MAX_FNAME];
	UCHAR ext[_MAX_EXT];

	if( ( type & 2 ) == 2 ) {
		*lpBuffer++ = '"';
	}

	if( _fullpath( path_buffer, FileData[NowFile].filename , _MAX_PATH ) == NULL ) {
		err_pre0(1);
		printf( "preprocessor error:__FILE__%dPF が展開できません ( path = %s )\n", type,FileData[NowFile].filename );
		return;
	}

	 _splitpath( path_buffer, drive, lpBuffer, fname, ext );
	strcat( lpBuffer, fname );

	if( ( type & 1 ) == 1 ) {
		yen2esc( lpBuffer );
	}
	if( ( type & 2 ) == 2 ) {
		strcat( lpBuffer, "\"" );
	}
}
/*-------------------------------------------------------------------------*/
void ExpDefine__FILE__PFE( UCHAR* lpBuffer, int type )
{
	UCHAR path_buffer[_MAX_PATH];
	UCHAR drive[_MAX_DRIVE];
//	UCHAR dir[_MAX_DIR];
	UCHAR fname[_MAX_FNAME];
	UCHAR ext[_MAX_EXT];

	if( ( type & 2 ) == 2 ) {
		*lpBuffer++ = '"';
	}

	if( _fullpath( path_buffer, FileData[NowFile].filename , _MAX_PATH ) == NULL ) {
		err_pre0(1);
		printf( "preprocessor error:__FILE__%dPFE が展開できません ( path = %s )\n", type,FileData[NowFile].filename );
		return;
	}

	 _splitpath( path_buffer, drive, lpBuffer, fname, ext );
	strcat( lpBuffer, fname );
	strcat( lpBuffer, ext );

	if( ( type & 1 ) == 1 ) {
		yen2esc( lpBuffer );
	}
	if( ( type & 2 ) == 2 ) {
		strcat( lpBuffer, "\"" );
	}
}
/*-------------------------------------------------------------------------*/
void ExpDefine__FILE__F( UCHAR* lpBuffer, int type )
{
	UCHAR path_buffer[_MAX_PATH];
	UCHAR drive[_MAX_DRIVE];
	UCHAR dir[_MAX_DIR];
//	UCHAR fname[_MAX_FNAME];
	UCHAR ext[_MAX_EXT];

	if( ( type & 2 ) == 2 ) {
		*lpBuffer++ = '"';
	}

	if( _fullpath( path_buffer, FileData[NowFile].filename , _MAX_PATH ) == NULL ) {
		err_pre0(1);
		printf( "preprocessor error:__FILE__%dF が展開できません ( path = %s )\n", type,FileData[NowFile].filename );
		return;
	}

	 _splitpath( path_buffer, drive, dir, lpBuffer, ext );

	if( ( type & 1 ) == 1 ) {
		yen2esc( lpBuffer );
	}
	if( ( type & 2 ) == 2 ) {
		strcat( lpBuffer, "\"" );
	}
}
/*-------------------------------------------------------------------------*/
void ExpDefine__FILE__FE( UCHAR* lpBuffer, int type )
{
	UCHAR path_buffer[_MAX_PATH];
	UCHAR drive[_MAX_DRIVE];
	UCHAR dir[_MAX_DIR];
//	UCHAR fname[_MAX_FNAME];
	UCHAR ext[_MAX_EXT];

	if( ( type & 2 ) == 2 ) {
		*lpBuffer++ = '"';
	}

	if( _fullpath( path_buffer, FileData[NowFile].filename , _MAX_PATH ) == NULL ) {
		err_pre0(1);
		printf( "preprocessor error:__FILE__%dFE が展開できません ( path = %s )\n", type,FileData[NowFile].filename );
		return;
	}

	 _splitpath( path_buffer, drive, dir, lpBuffer, ext );
	strcat( lpBuffer, ext );

	if( ( type & 1 ) == 1 ) {
		yen2esc( lpBuffer );
	}
	if( ( type & 2 ) == 2 ) {
		strcat( lpBuffer, "\"" );
	}
}
/*-------------------------------------------------------------------------*/
void ExpDefine__FILE__E( UCHAR* lpBuffer, int type )
{
	UCHAR path_buffer[_MAX_PATH];
	UCHAR drive[_MAX_DRIVE];
	UCHAR dir[_MAX_DIR];
	UCHAR fname[_MAX_FNAME];
//	UCHAR ext[_MAX_EXT];

	if( ( type & 2 ) == 2 ) {
		*lpBuffer++ = '"';
	}

	if( _fullpath( path_buffer, FileData[NowFile].filename , _MAX_PATH ) == NULL ) {
		err_pre0(1);
		printf( "preprocessor error:__FILE__%dE が展開できません ( path = %s )\n", type,FileData[NowFile].filename );
		return;
	}

	 _splitpath( path_buffer, drive, dir, fname, lpBuffer );

	if( ( type & 1 ) == 1 ) {
		yen2esc( lpBuffer );
	}
	if( ( type & 2 ) == 2 ) {
		strcat( lpBuffer, "\"" );
	}
}
/*-------------------------------------------------------------------------*/
void ExpDefine__FILE0__( UCHAR* lpBuffer )
//
//	__FILE0__ を展開
//
{
	if( _fullpath( lpBuffer+1, FileData[0].filename , _MAX_PATH ) != NULL ) {
		yen2esc( lpBuffer+1 );
		lpBuffer[0]  = '"';
		strcat( lpBuffer, "\"" );
	} else {
		err_pre0(1);
		printf( "preprocessor error:__FILE0__ が展開できません ( path = %s )\n", FileData[0].filename );
	}
}
/*-------------------------------------------------------------------------*/
void ExpDefine__FILE0__D( UCHAR* lpBuffer, int type )
{
	UCHAR path_buffer[_MAX_PATH];
//	UCHAR drive[_MAX_DRIVE];
	UCHAR dir[_MAX_DIR];
	UCHAR fname[_MAX_FNAME];
	UCHAR ext[_MAX_EXT];

	if( ( type & 2 ) == 2 ) {
		*lpBuffer++ = '"';
	}

	if( _fullpath( path_buffer, FileData[0].filename , _MAX_PATH ) == NULL ) {
		err_pre0(1);
		printf( "preprocessor error:__FILE0__%dD が展開できません ( path = %s )\n", type,FileData[0].filename );
		return;
	}

	 _splitpath( path_buffer, lpBuffer, dir, fname, ext );

//	if( ( type & 1 ) == 1 ) {
//		yen2esc( lpBuffer );
//	}
	if( ( type & 2 ) == 2 ) {
		strcat( lpBuffer, "\"" );
	}
}
/*-------------------------------------------------------------------------*/
void ExpDefine__FILE0__DP( UCHAR* lpBuffer, int type )
{
	UCHAR path_buffer[_MAX_PATH];
//	UCHAR drive[_MAX_DRIVE];
	UCHAR dir[_MAX_DIR];
	UCHAR fname[_MAX_FNAME];
	UCHAR ext[_MAX_EXT];

	if( ( type & 2 ) == 2 ) {
		*lpBuffer++ = '"';
	}

	if( _fullpath( path_buffer, FileData[0].filename , _MAX_PATH ) == NULL ) {
		err_pre0(1);
		printf( "preprocessor error:__FILE0__%dDP が展開できません ( path = %s )\n", type,FileData[0].filename );
		return;
	}

	 _splitpath( path_buffer, lpBuffer, dir, fname, ext );
	strcat( lpBuffer, dir );

	if( ( type & 1 ) == 1 ) {
		yen2esc( lpBuffer );
	}
	if( ( type & 2 ) == 2 ) {
		strcat( lpBuffer, "\"" );
	}
}
/*-------------------------------------------------------------------------*/
void ExpDefine__FILE0__DPF( UCHAR* lpBuffer, int type )
{
	UCHAR path_buffer[_MAX_PATH];
//	UCHAR drive[_MAX_DRIVE];
	UCHAR dir[_MAX_DIR];
	UCHAR fname[_MAX_FNAME];
	UCHAR ext[_MAX_EXT];

	if( ( type & 2 ) == 2 ) {
		*lpBuffer++ = '"';
	}

	if( _fullpath( path_buffer, FileData[0].filename , _MAX_PATH ) == NULL ) {
		err_pre0(1);
		printf( "preprocessor error:__FILE0__%dDPF が展開できません ( path = %s )\n", type,FileData[0].filename );
		return;
	}

	 _splitpath( path_buffer, lpBuffer, dir, fname, ext );
	strcat( lpBuffer, dir );
	strcat( lpBuffer, fname );

	if( ( type & 1 ) == 1 ) {
		yen2esc( lpBuffer );
	}
	if( ( type & 2 ) == 2 ) {
		strcat( lpBuffer, "\"" );
	}
}
/*-------------------------------------------------------------------------*/
void ExpDefine__FILE0__DPFE( UCHAR* lpBuffer, int type )
{
	if( ( type & 2 ) == 2 ) {
		*lpBuffer++ = '"';
	}
	if( _fullpath( lpBuffer, FileData[0].filename , _MAX_PATH ) != NULL ) {
		if( ( type & 1 ) == 1 ) {
			yen2esc( lpBuffer );
		}
	} else {
		err_pre0(1);
		printf( "preprocessor error:__FILE0__%dDPFE が展開できません ( path = %s )\n", type, FileData[0].filename );
		return;
	}
	if( ( type & 2 ) == 2 ) {
		strcat( lpBuffer, "\"" );
	}
}
/*-------------------------------------------------------------------------*/
void ExpDefine__FILE0__P( UCHAR* lpBuffer, int type )
{
	UCHAR path_buffer[_MAX_PATH];
	UCHAR drive[_MAX_DRIVE];
//	UCHAR dir[_MAX_DIR];
	UCHAR fname[_MAX_FNAME];
	UCHAR ext[_MAX_EXT];

	if( ( type & 2 ) == 2 ) {
		*lpBuffer++ = '"';
	}

	if( _fullpath( path_buffer, FileData[0].filename , _MAX_PATH ) == NULL ) {
		err_pre0(1);
		printf( "preprocessor error:__FILE0__%dP が展開できません ( path = %s )\n", type,FileData[0].filename );
		return;
	}

	 _splitpath( path_buffer, drive, lpBuffer, fname, ext );

	if( ( type & 1 ) == 1 ) {
		yen2esc( lpBuffer );
	}
	if( ( type & 2 ) == 2 ) {
		strcat( lpBuffer, "\"" );
	}
}
/*-------------------------------------------------------------------------*/
void ExpDefine__FILE0__PF( UCHAR* lpBuffer, int type )
{
	UCHAR path_buffer[_MAX_PATH];
	UCHAR drive[_MAX_DRIVE];
//	UCHAR dir[_MAX_DIR];
	UCHAR fname[_MAX_FNAME];
	UCHAR ext[_MAX_EXT];

	if( ( type & 2 ) == 2 ) {
		*lpBuffer++ = '"';
	}

	if( _fullpath( path_buffer, FileData[0].filename , _MAX_PATH ) == NULL ) {
		err_pre0(1);
		printf( "preprocessor error:__FILE0__%dPF が展開できません ( path = %s )\n", type,FileData[0].filename );
		return;
	}

	 _splitpath( path_buffer, drive, lpBuffer, fname, ext );
	strcat( lpBuffer, fname );

	if( ( type & 1 ) == 1 ) {
		yen2esc( lpBuffer );
	}
	if( ( type & 2 ) == 2 ) {
		strcat( lpBuffer, "\"" );
	}
}
/*-------------------------------------------------------------------------*/
void ExpDefine__FILE0__PFE( UCHAR* lpBuffer, int type )
{
	UCHAR path_buffer[_MAX_PATH];
	UCHAR drive[_MAX_DRIVE];
//	UCHAR dir[_MAX_DIR];
	UCHAR fname[_MAX_FNAME];
	UCHAR ext[_MAX_EXT];

	if( ( type & 2 ) == 2 ) {
		*lpBuffer++ = '"';
	}

	if( _fullpath( path_buffer, FileData[0].filename , _MAX_PATH ) == NULL ) {
		err_pre0(1);
		printf( "preprocessor error:__FILE0__%dPFE が展開できません ( path = %s )\n", type,FileData[0].filename );
		return;
	}

	 _splitpath( path_buffer, drive, lpBuffer, fname, ext );
	strcat( lpBuffer, fname );
	strcat( lpBuffer, ext );

	if( ( type & 1 ) == 1 ) {
		yen2esc( lpBuffer );
	}
	if( ( type & 2 ) == 2 ) {
		strcat( lpBuffer, "\"" );
	}
}
/*-------------------------------------------------------------------------*/
void ExpDefine__FILE0__F( UCHAR* lpBuffer, int type )
{
	UCHAR path_buffer[_MAX_PATH];
	UCHAR drive[_MAX_DRIVE];
	UCHAR dir[_MAX_DIR];
//	UCHAR fname[_MAX_FNAME];
	UCHAR ext[_MAX_EXT];

	if( ( type & 2 ) == 2 ) {
		*lpBuffer++ = '"';
	}

	if( _fullpath( path_buffer, FileData[0].filename , _MAX_PATH ) == NULL ) {
		err_pre0(1);
		printf( "preprocessor error:__FILE0__%dF が展開できません ( path = %s )\n", type,FileData[0].filename );
		return;
	}

	 _splitpath( path_buffer, drive, dir, lpBuffer, ext );

	if( ( type & 1 ) == 1 ) {
		yen2esc( lpBuffer );
	}
	if( ( type & 2 ) == 2 ) {
		strcat( lpBuffer, "\"" );
	}
}
/*-------------------------------------------------------------------------*/
void ExpDefine__FILE0__FE( UCHAR* lpBuffer, int type )
{
	UCHAR path_buffer[_MAX_PATH];
	UCHAR drive[_MAX_DRIVE];
	UCHAR dir[_MAX_DIR];
//	UCHAR fname[_MAX_FNAME];
	UCHAR ext[_MAX_EXT];

	if( ( type & 2 ) == 2 ) {
		*lpBuffer++ = '"';
	}

	if( _fullpath( path_buffer, FileData[0].filename , _MAX_PATH ) == NULL ) {
		err_pre0(1);
		printf( "preprocessor error:__FILE0__%dFE が展開できません ( path = %s )\n", type,FileData[0].filename );
		return;
	}

	 _splitpath( path_buffer, drive, dir, lpBuffer, ext );
	strcat( lpBuffer, ext );

	if( ( type & 1 ) == 1 ) {
		yen2esc( lpBuffer );
	}
	if( ( type & 2 ) == 2 ) {
		strcat( lpBuffer, "\"" );
	}
}
/*-------------------------------------------------------------------------*/
void ExpDefine__FILE0__E( UCHAR* lpBuffer, int type )
{
	UCHAR path_buffer[_MAX_PATH];
	UCHAR drive[_MAX_DRIVE];
	UCHAR dir[_MAX_DIR];
	UCHAR fname[_MAX_FNAME];
//	UCHAR ext[_MAX_EXT];

	if( ( type & 2 ) == 2 ) {
		*lpBuffer++ = '"';
	}

	if( _fullpath( path_buffer, FileData[0].filename , _MAX_PATH ) == NULL ) {
		err_pre0(1);
		printf( "preprocessor error:__FILE0__%dE が展開できません ( path = %s )\n", type,FileData[0].filename );
		return;
	}

	 _splitpath( path_buffer, drive, dir, fname, lpBuffer );

	if( ( type & 1 ) == 1 ) {
		yen2esc( lpBuffer );
	}
	if( ( type & 2 ) == 2 ) {
		strcat( lpBuffer, "\"" );
	}
}
/*-------------------------------------------------------------------------*/
void StrLower( UCHAR *lp )
{
//
//	ank 大文字を ank 小文字へ変換
//
	while( *lp != 0 ) {
//		if( _ismbblead( *lp )) {
		if( _iskanji(*lp)) {
			lp+=2;
		} else if( *lp >= 'A' && *lp <= 'Z' ) {
			*lp+='a'-'A';
		} else {
			lp++;
		}
	}

}
/*-------------------------------------------------------------------------*/
void ExpDefine( UCHAR* lpBuffer, UCHAR* lpName, int num, int* lpFlag )
//
//	macro を展開
//
{
//;;;make nset がおかしい

	UCHAR *lp0,*lp1,*lp2,*lp3,*lp4,*lpPara[MAXDEFINEPARA],*lpObject[MAXDEFINEPARA];
	UCHAR buffer0[MAX_MACROEXP],buffer1[MAX_MACROEXP],para0[MAXTOKEN];
//	UCHAR token[MAXTOKEN];
	int c,d,d2,nPara,nLocal,type,typepara,mode,nFile,nLine,MinPara,nUsePara,fUsePara;
//;;;make MAX_MACROEXP


	*lpFlag = 0;

	//
	//	token start
	//
	if( num >= nTokenStart && num <= nTokenEnd ) {
		switch( num-nTokenStart ) {
			case 0:
				ExpDefine__LINE__( lpBuffer );
				return;
			case 1:
				ExpDefine__FILE__( lpBuffer );
				StrLower( lpBuffer );
				return;
			case 2:
				ExpDefine__FILE__D( lpBuffer,0 );
				StrLower( lpBuffer );
				return;
			case 3:
				ExpDefine__FILE__DP( lpBuffer,0 );
				StrLower( lpBuffer );
				return;
			case 4:
				ExpDefine__FILE__DPF( lpBuffer,0 );
				StrLower( lpBuffer );
				return;
			case 5:
				ExpDefine__FILE__DPFE( lpBuffer,0 );
				StrLower( lpBuffer );
				return;
			case 6:
				ExpDefine__FILE__P( lpBuffer,0 );
				StrLower( lpBuffer );
				return;
			case 7:
				ExpDefine__FILE__PF( lpBuffer,0 );
				StrLower( lpBuffer );
				return;
			case 8:
				ExpDefine__FILE__PFE( lpBuffer,0 );
				StrLower( lpBuffer );
				return;
			case 9:
				ExpDefine__FILE__F( lpBuffer,0 );
				StrLower( lpBuffer );
				return;
			case 10:
				ExpDefine__FILE__FE( lpBuffer,0 );
				StrLower( lpBuffer );
				return;
			case 11:
				ExpDefine__FILE__E( lpBuffer,0 );
				StrLower( lpBuffer );
				return;
			case 12:
				ExpDefine__FILE__D( lpBuffer,1 );
				StrLower( lpBuffer );
				return;
			case 13:
				ExpDefine__FILE__DP( lpBuffer,1 );
				StrLower( lpBuffer );
				return;
			case 14:
				ExpDefine__FILE__DPF( lpBuffer,1 );
				StrLower( lpBuffer );
				return;
			case 15:
				ExpDefine__FILE__DPFE( lpBuffer,1 );
				StrLower( lpBuffer );
				return;
			case 16:
				ExpDefine__FILE__P( lpBuffer,1 );
				StrLower( lpBuffer );
				return;
			case 17:
				ExpDefine__FILE__PF( lpBuffer,1 );
				StrLower( lpBuffer );
				return;
			case 18:
				ExpDefine__FILE__PFE( lpBuffer,1 );
				StrLower( lpBuffer );
				return;
			case 19:
				ExpDefine__FILE__F( lpBuffer,1 );
				StrLower( lpBuffer );
				return;
			case 20:
				ExpDefine__FILE__FE( lpBuffer,1 );
				StrLower( lpBuffer );
				return;
			case 21:
				ExpDefine__FILE__E( lpBuffer,1 );
				StrLower( lpBuffer );
				return;
			case 22:
				ExpDefine__FILE__D( lpBuffer,2 );
				StrLower( lpBuffer );
				return;
			case 23:
				ExpDefine__FILE__DP( lpBuffer,2 );
				StrLower( lpBuffer );
				return;
			case 24:
				ExpDefine__FILE__DPF( lpBuffer,2 );
				StrLower( lpBuffer );
				return;
			case 25:
				ExpDefine__FILE__DPFE( lpBuffer,2 );
				StrLower( lpBuffer );
				return;
			case 26:
				ExpDefine__FILE__P( lpBuffer,2 );
				StrLower( lpBuffer );
				return;
			case 27:
				ExpDefine__FILE__PF( lpBuffer,2 );
				StrLower( lpBuffer );
				return;
			case 28:
				ExpDefine__FILE__PFE( lpBuffer,2 );
				StrLower( lpBuffer );
				return;
			case 29:
				ExpDefine__FILE__F( lpBuffer,2 );
				StrLower( lpBuffer );
				return;
			case 30:
				ExpDefine__FILE__FE( lpBuffer,2 );
				StrLower( lpBuffer );
				return;
			case 31:
				ExpDefine__FILE__E( lpBuffer,2 );
				StrLower( lpBuffer );
				return;
			case 32:
				ExpDefine__FILE__D( lpBuffer,3 );
				StrLower( lpBuffer );
				return;
			case 33:
				ExpDefine__FILE__DP( lpBuffer,3 );
				StrLower( lpBuffer );
				return;
			case 34:
				ExpDefine__FILE__DPF( lpBuffer,3 );
				StrLower( lpBuffer );
				return;
			case 35:
				ExpDefine__FILE__DPFE( lpBuffer,3 );
				StrLower( lpBuffer );
				return;
			case 36:
				ExpDefine__FILE__P( lpBuffer,3 );
				StrLower( lpBuffer );
				return;
			case 37:
				ExpDefine__FILE__PF( lpBuffer,3 );
				StrLower( lpBuffer );
				return;
			case 38:
				ExpDefine__FILE__PFE( lpBuffer,3 );
				StrLower( lpBuffer );
				return;
			case 39:
				ExpDefine__FILE__F( lpBuffer,3 );
				StrLower( lpBuffer );
				return;
			case 40:
				ExpDefine__FILE__FE( lpBuffer,3 );
				StrLower( lpBuffer );
				return;
			case 41:
				ExpDefine__FILE__E( lpBuffer,3 );
				StrLower( lpBuffer );
				return;
			case 42:
				ExpDefine__FILE__D( lpBuffer,0 );
				return;
			case 43:
				ExpDefine__FILE__DP( lpBuffer,0 );
				return;
			case 44:
				ExpDefine__FILE__DPF( lpBuffer,0 );
				return;
			case 45:
				ExpDefine__FILE__DPFE( lpBuffer,0 );
				return;
			case 46:
				ExpDefine__FILE__P( lpBuffer,0 );
				return;
			case 47:
				ExpDefine__FILE__PF( lpBuffer,0 );
				return;
			case 48:
				ExpDefine__FILE__PFE( lpBuffer,0 );
				return;
			case 49:
				ExpDefine__FILE__F( lpBuffer,0 );
				return;
			case 50:
				ExpDefine__FILE__FE( lpBuffer,0 );
				return;
			case 51:
				ExpDefine__FILE__E( lpBuffer,0 );
				return;
			case 52:
				ExpDefine__FILE__D( lpBuffer,1 );
				return;
			case 53:
				ExpDefine__FILE__DP( lpBuffer,1 );
				return;
			case 54:
				ExpDefine__FILE__DPF( lpBuffer,1 );
				return;
			case 55:
				ExpDefine__FILE__DPFE( lpBuffer,1 );
				return;
			case 56:
				ExpDefine__FILE__P( lpBuffer,1 );
				return;
			case 57:
				ExpDefine__FILE__PF( lpBuffer,1 );
				return;
			case 58:
				ExpDefine__FILE__PFE( lpBuffer,1 );
				return;
			case 59:
				ExpDefine__FILE__F( lpBuffer,1 );
				return;
			case 60:
				ExpDefine__FILE__FE( lpBuffer,1 );
				return;
			case 61:
				ExpDefine__FILE__E( lpBuffer,1 );
				return;
			case 62:
				ExpDefine__FILE__D( lpBuffer,2 );
				return;
			case 63:
				ExpDefine__FILE__DP( lpBuffer,2 );
				return;
			case 64:
				ExpDefine__FILE__DPF( lpBuffer,2 );
				return;
			case 65:
				ExpDefine__FILE__DPFE( lpBuffer,2 );
				return;
			case 66:
				ExpDefine__FILE__P( lpBuffer,2 );
				return;
			case 67:
				ExpDefine__FILE__PF( lpBuffer,2 );
				return;
			case 68:
				ExpDefine__FILE__PFE( lpBuffer,2 );
				return;
			case 69:
				ExpDefine__FILE__F( lpBuffer,2 );
				return;
			case 70:
				ExpDefine__FILE__FE( lpBuffer,2 );
				return;
			case 71:
				ExpDefine__FILE__E( lpBuffer,2 );
				return;
			case 72:
				ExpDefine__FILE__D( lpBuffer,3 );
				return;
			case 73:
				ExpDefine__FILE__DP( lpBuffer,3 );
				return;
			case 74:
				ExpDefine__FILE__DPF( lpBuffer,3 );
				return;
			case 75:
				ExpDefine__FILE__DPFE( lpBuffer,3 );
				return;
			case 76:
				ExpDefine__FILE__P( lpBuffer,3 );
				return;
			case 77:
				ExpDefine__FILE__PF( lpBuffer,3 );
				return;
			case 78:
				ExpDefine__FILE__PFE( lpBuffer,3 );
				return;
			case 79:
				ExpDefine__FILE__F( lpBuffer,3 );
				return;
			case 80:
				ExpDefine__FILE__FE( lpBuffer,3 );
				return;
			case 81:
				ExpDefine__FILE__E( lpBuffer,3 );
				return;
			case 82:
				ExpDefine__FILE0__( lpBuffer );
				StrLower( lpBuffer );
				return;
			case 83:
				ExpDefine__FILE0__D( lpBuffer,0 );
				StrLower( lpBuffer );
				return;
			case 84:
				ExpDefine__FILE0__DP( lpBuffer,0 );
				StrLower( lpBuffer );
				return;
			case 85:
				ExpDefine__FILE0__DPF( lpBuffer,0 );
				StrLower( lpBuffer );
				return;
			case 86:
				ExpDefine__FILE0__DPFE( lpBuffer,0 );
				StrLower( lpBuffer );
				return;
			case 87:
				ExpDefine__FILE0__P( lpBuffer,0 );
				StrLower( lpBuffer );
				return;
			case 88:
				ExpDefine__FILE0__PF( lpBuffer,0 );
				StrLower( lpBuffer );
				return;
			case 89:
				ExpDefine__FILE0__PFE( lpBuffer,0 );
				StrLower( lpBuffer );
				return;
			case 90:
				ExpDefine__FILE0__F( lpBuffer,0 );
				StrLower( lpBuffer );
				return;
			case 91:
				ExpDefine__FILE0__FE( lpBuffer,0 );
				StrLower( lpBuffer );
				return;
			case 92:
				ExpDefine__FILE0__E( lpBuffer,0 );
				StrLower( lpBuffer );
				return;
			case 93:
				ExpDefine__FILE0__D( lpBuffer,1 );
				StrLower( lpBuffer );
				return;
			case 94:
				ExpDefine__FILE0__DP( lpBuffer,1 );
				StrLower( lpBuffer );
				return;
			case 95:
				ExpDefine__FILE0__DPF( lpBuffer,1 );
				StrLower( lpBuffer );
				return;
			case 96:
				ExpDefine__FILE0__DPFE( lpBuffer,1 );
				StrLower( lpBuffer );
				return;
			case 97:
				ExpDefine__FILE0__P( lpBuffer,1 );
				StrLower( lpBuffer );
				return;
			case 98:
				ExpDefine__FILE0__PF( lpBuffer,1 );
				StrLower( lpBuffer );
				return;
			case 99:
				ExpDefine__FILE0__PFE( lpBuffer,1 );
				StrLower( lpBuffer );
				return;
			case 100:
				ExpDefine__FILE0__F( lpBuffer,1 );
				StrLower( lpBuffer );
				return;
			case 101:
				ExpDefine__FILE0__FE( lpBuffer,1 );
				StrLower( lpBuffer );
				return;
			case 102:
				ExpDefine__FILE0__E( lpBuffer,1 );
				StrLower( lpBuffer );
				return;
			case 103:
				ExpDefine__FILE0__D( lpBuffer,2 );
				StrLower( lpBuffer );
				return;
			case 104:
				ExpDefine__FILE0__DP( lpBuffer,2 );
				StrLower( lpBuffer );
				return;
			case 105:
				ExpDefine__FILE0__DPF( lpBuffer,2 );
				StrLower( lpBuffer );
				return;
			case 106:
				ExpDefine__FILE0__DPFE( lpBuffer,2 );
				StrLower( lpBuffer );
				return;
			case 107:
				ExpDefine__FILE0__P( lpBuffer,2 );
				StrLower( lpBuffer );
				return;
			case 108:
				ExpDefine__FILE0__PF( lpBuffer,2 );
				StrLower( lpBuffer );
				return;
			case 109:
				ExpDefine__FILE0__PFE( lpBuffer,2 );
				StrLower( lpBuffer );
				return;
			case 110:
				ExpDefine__FILE0__F( lpBuffer,2 );
				StrLower( lpBuffer );
				return;
			case 111:
				ExpDefine__FILE0__FE( lpBuffer,2 );
				StrLower( lpBuffer );
				return;
			case 112:
				ExpDefine__FILE0__E( lpBuffer,2 );
				StrLower( lpBuffer );
				return;
			case 113:
				ExpDefine__FILE0__D( lpBuffer,3 );
				StrLower( lpBuffer );
				return;
			case 114:
				ExpDefine__FILE0__DP( lpBuffer,3 );
				StrLower( lpBuffer );
				return;
			case 115:
				ExpDefine__FILE0__DPF( lpBuffer,3 );
				StrLower( lpBuffer );
				return;
			case 116:
				ExpDefine__FILE0__DPFE( lpBuffer,3 );
				StrLower( lpBuffer );
				return;
			case 117:
				ExpDefine__FILE0__P( lpBuffer,3 );
				StrLower( lpBuffer );
				return;
			case 118:
				ExpDefine__FILE0__PF( lpBuffer,3 );
				StrLower( lpBuffer );
				return;
			case 119:
				ExpDefine__FILE0__PFE( lpBuffer,3 );
				StrLower( lpBuffer );
				return;
			case 120:
				ExpDefine__FILE0__F( lpBuffer,3 );
				StrLower( lpBuffer );
				return;
			case 121:
				ExpDefine__FILE0__FE( lpBuffer,3 );
				StrLower( lpBuffer );
				return;
			case 122:
				ExpDefine__FILE0__E( lpBuffer,3 );
				StrLower( lpBuffer );
				return;
			case 123:
				ExpDefine__FILE0__D( lpBuffer,0 );
				return;
			case 124:
				ExpDefine__FILE0__DP( lpBuffer,0 );
				return;
			case 125:
				ExpDefine__FILE0__DPF( lpBuffer,0 );
				return;
			case 126:
				ExpDefine__FILE0__DPFE( lpBuffer,0 );
				return;
			case 127:
				ExpDefine__FILE0__P( lpBuffer,0 );
				return;
			case 128:
				ExpDefine__FILE0__PF( lpBuffer,0 );
				return;
			case 129:
				ExpDefine__FILE0__PFE( lpBuffer,0 );
				return;
			case 130:
				ExpDefine__FILE0__F( lpBuffer,0 );
				return;
			case 131:
				ExpDefine__FILE0__FE( lpBuffer,0 );
				return;
			case 132:
				ExpDefine__FILE0__E( lpBuffer,0 );
				return;
			case 133:
				ExpDefine__FILE0__D( lpBuffer,1 );
				return;
			case 134:
				ExpDefine__FILE0__DP( lpBuffer,1 );
				return;
			case 135:
				ExpDefine__FILE0__DPF( lpBuffer,1 );
				return;
			case 136:
				ExpDefine__FILE0__DPFE( lpBuffer,1 );
				return;
			case 137:
				ExpDefine__FILE0__P( lpBuffer,1 );
				return;
			case 138:
				ExpDefine__FILE0__PF( lpBuffer,1 );
				return;
			case 139:
				ExpDefine__FILE0__PFE( lpBuffer,1 );
				return;
			case 140:
				ExpDefine__FILE0__F( lpBuffer,1 );
				return;
			case 141:
				ExpDefine__FILE0__FE( lpBuffer,1 );
				return;
			case 142:
				ExpDefine__FILE0__E( lpBuffer,1 );
				return;
			case 143:
				ExpDefine__FILE0__D( lpBuffer,2 );
				return;
			case 144:
				ExpDefine__FILE0__DP( lpBuffer,2 );
				return;
			case 145:
				ExpDefine__FILE0__DPF( lpBuffer,2 );
				return;
			case 146:
				ExpDefine__FILE0__DPFE( lpBuffer,2 );
				return;
			case 147:
				ExpDefine__FILE0__P( lpBuffer,2 );
				return;
			case 148:
				ExpDefine__FILE0__PF( lpBuffer,2 );
				return;
			case 149:
				ExpDefine__FILE0__PFE( lpBuffer,2 );
				return;
			case 150:
				ExpDefine__FILE0__F( lpBuffer,2 );
				return;
			case 151:
				ExpDefine__FILE0__FE( lpBuffer,2 );
				return;
			case 152:
				ExpDefine__FILE0__E( lpBuffer,2 );
				return;
			case 153:
				ExpDefine__FILE0__D( lpBuffer,3 );
				return;
			case 154:
				ExpDefine__FILE0__DP( lpBuffer,3 );
				return;
			case 155:
				ExpDefine__FILE0__DPF( lpBuffer,3 );
				return;
			case 156:
				ExpDefine__FILE0__DPFE( lpBuffer,3 );
				return;
			case 157:
				ExpDefine__FILE0__P( lpBuffer,3 );
				return;
			case 158:
				ExpDefine__FILE0__PF( lpBuffer,3 );
				return;
			case 159:
				ExpDefine__FILE0__PFE( lpBuffer,3 );
				return;
			case 160:
				ExpDefine__FILE0__F( lpBuffer,3 );
				return;
			case 161:
				ExpDefine__FILE0__FE( lpBuffer,3 );
				return;
			case 162:
				ExpDefine__FILE0__E( lpBuffer,3 );
				return;
		}
	}



	lp0 = lpDefine[num];
	type = *((int*)lp0)++;			// type によって、#macro か #define か
	nPara = *((int*)lp0)++;
	nLocal = *((int*)lp0)++;
	MinPara = *((int*)lp0)++;
	nFile = *((int*)lp0)++;
	nLine = *((int*)lp0)++;

	//
	//	GetPara の type
	//
	typepara = 0;
	if( type == 1 ) {
		typepara++;
	}


	//
	//	macro の para を展開
	//
	if( nPara != 0 ) {

		//
		//	'(' が無かった場合は、macro ではなく、ただの token
		//
		SkipSpace();
		d = GetCharDataOne();
		if( d != '(' ) {
			if( type == 1 ) {					// #adefine のときには、( はいらない
				UnGetCharDataOne( d );
			} else {
				UnGetCharDataOne( d );
				lpBuffer+=strlen(lpBuffer);
				strcpy( lpBuffer, lpRegist[num]+HASH_STR );
				lpBuffer+=strlen(lpBuffer);
				*lpBuffer=0;
				*lpFlag = 1;
				return;
			}
		}


		//
		//	para への pointer の拾得
		//
		for( c=0; c<nPara; c++ ){
			d = *((int*)lp0)++;
			lpPara[c] = lp0;
			lp0 += d;
		}


		//
		//	 実 para を buffer0 へ copy して、それへの pointer を拾得
		//
		lp1 = buffer0;
		nUsePara=0;
		fUsePara=1;
		for( c=0; c<nPara; c++ ){
			SkipSpace();
			lpObject[c] = lp1;
			if( fUsePara ) nUsePara++;
			lp1 = GetPara( lp1,typepara,-1 );
			*lp1++ = 0;
			SkipSpace();
			d = GetCharDataOne();
			d2 = 0;
			if( d == ')' ) {
				SkipSpace();
				d2 = GetCharDataOne();
				UnGetCharDataOne( d2 );
			}
			if( d2 != ','&& d2 != 0 ) {
				UnGetCharDataOne( d );
			} else if( d == ',' ) {
				if( c == nPara-1 ) {
					UnGetCharDataOne( d );
				}
			} else {
				if( c == nPara-1 && type == 1 ) {
					UnGetCharDataOne( d );
				} else {
					err_pre0(1);
					printf( "preprocessor error:macro0 の %d 番目のパラメーターが異常です\n",c+1 );
					return;
				}
			}
		}
		if( nUsePara<MinPara ) {
			err_pre0(1);
			printf( "preprocessor error:macro0 の実パラメーターが少ないです。( 仮パラメーター %d 個,実パラメーター %d 個 )\n", nPara,nUsePara );
			return;
		}
		SkipSpace();
		d = GetCharDataOne();
		if( d != ')' ) {
			if( nPara == 0 || type == 1 ) {
				UnGetCharDataOne( d );
			} else {
				err_pre0(1);
				printf( "preprocessor error:macro の実パラメーターが異常、もしくは実パラメーターが仮パラメーターより多いです ( 仮パラメーター %d 個 )\n", nPara+1 );
				return;
			}
		}
	} else {
		lp2 = buffer1;
	}


	//
	//	para を展開しながら buffer1 へ copy する
	//
	UnGetCharDataOne( 0 );
	UnGetStrData( lp0 );
	lp2 = buffer1;
	mode = 0;
	while( 1 ){
		c = GetCharDataOne();
		if( c == 0 ) {
			*lp2++ = 0;
			break;
		} else if( c == '#' ) {
			c = GetCharDataOne();
			if( c == '@' ){
				mode |= 1;
			} else if( c == '#' ){
				mode |= 2;
			} else {
				UnGetCharDataOne( c );
				mode |= 4;
			}
		} else if( c == '\\' ) {
			*lp2++ = '\\';
			if( _iskanji((*lp2++ = GetCharDataOne())) ) {
				*lp2++ = GetCharDataOne();
			}
		} else if( c == '"' ) {
			if( (mode & 2) == 2 ) {
				lp2 = DeleteBeforeSpace( buffer0, lp2, mode );
				mode &= (~2);
			}
			ExpDefineModeError( mode );
			lp2 = GetTokenSub0x05( lp2, MODE_NORMAL,0,0,NULL );
		} else if( c == '\'' ) {
			if( (mode & 2) == 2 ) {
				lp2 = DeleteBeforeSpace( buffer0, lp2, mode );
				mode &= (~2);
			}
			ExpDefineModeError( mode );
			lp2 = GetTokenSub0x06( lp2, MODE_NORMAL );
		} else if( c == ';' ) {
			GetTokenSub0x0e();
			*lp2++ = ' ';
		} else if( c == '/' ) {
			if( GetTokenSub0x04() == 1 ) {
				*lp2++ = ' ';
			} else {
				*lp2++ = '/';
			}
		} else if( _isname0(c) ) {
			lp3 = para0;
			*lp3++ = c;
			while( _isname1(c = GetCharDataOne()) ) {
				*lp3++ = c;
			}
			UnGetCharDataOne( c );
			*lp3++ = 0;
			lp3 = para0;
			if( strcmp( para0, lpName ) == 0 ) {
				lp2 += strlen( lp2 );
			} else if( strcmp( para0, lpRegist[num]+HASH_STR ) == 0 ) {
				lp2 += strlen( lp2 );
			} else {
				for( c=0;c<nPara;c++ ) {
					if( strcmp( para0,lpPara[c] ) == 0 ) {
						lp3 = lpObject[c];
						break;
					}
				}
			}
			if( (mode & 2) == 2 ) {
				lp2 = DeleteBeforeSpace( buffer0, lp2, mode );
				mode &= (~2);
			}
			if( lp3 == para0 ) {
				ExpDefineModeError( mode );
				strcpy( lp2, lp3 );
				lp2 += strlen( lp2 );
			} else {
				if( (mode & 1) == 1 ) {
						*lp2++ = '\'';
				} else if( (mode & 4) == 4 ) {
						*lp2++ = '"';
				}
				if( (mode & 4) == 4 ) {
					while( *lp3 != 0 ) {
						if( *lp3 == '"' ) {
							*lp2++ = '\\';
							*lp2++ = '"';
							lp3++;
						} else if( *lp3 == '\\' ) {
							*lp2++ = '\\';
							*lp2++ = '\\';
							lp3++;
//						} else if( _ismbblead( *lp3 )) {
						} else if( _iskanji(*lp3)) {
							*lp2++ = *lp3++;
							*lp2++ = *lp3++;
						} else {
							// macro の実パラメーターの最後が space か tab の場合、削除する
							if( *lp3 == ' ' || *lp3 == '\t' ) {
								lp4 = lp3;
								while( *lp3 == ' ' || *lp3 == '\t' ) {
									lp3++;
								}
								// space / tab の最後が 0 じゃない場合、1 文字だけ出力
								if( *lp3 != 0 ) {
									lp3 = lp4;
									*lp2++ = *lp3++;
								}
							} else {
								*lp2++ = *lp3++;
							}
						}
					}
				} else {
					strcpy( lp2, lp3 );
					lp2 += strlen( lp2 );
				}
				if( (mode & 1) == 1 ) {
					*lp2++ = '\'';
				} else if( (mode & 4) == 4 ) {
					*lp2++ = '"';
				}
				mode = 0;
			}
		} else if( _iskanji(c) ) {
			*lp2++ = c;
			*lp2++ = GetCharDataOne();
		} else {
			*lp2++ = c;
		}
	}

#if 0
//;;;make
//	macro の展開の最中、リング化してしまうときの処理
//	aaa -> bbb -> ccc -> aaa とか
//;;;make
	//
	//	再度 macro を展開する
	//
	UnGetCharDataOne( 0 );
	UnGetStrData( buffer1 );
	lp1 = buffer1;
	while(( lp1 = GetToken( token )) != NULL ){
		d = GetTokenNum( lp1, &c );
		if( strcmp( lp1, lpName ) == 0 ) {
//			strcpy( lpBuffer, "__" );
//			strcpy( lpBuffer,lp1 );
			ExpDefine( lpBuffer, lpName, c, &d );
		} else if( c == num ) {
			strcpy( lpBuffer,lp1 );
//			ExpDefine( lpBuffer, lpName, c, &d );
		} else if( d >= NUM_DEFINED_TOP && d <= NUM_DEFINED_END ) {
			strcpy( lpBuffer,lp1 );
//			ExpDefine( lpBuffer, lpName, c, &d );
		} else {
			strcpy( lpBuffer,lp1 );
		}
		lpBuffer+=strlen(lpBuffer);
	}
	*lpBuffer=0;
	return;


#else
	strcpy( lpBuffer, buffer1 );
	lpBuffer+=strlen(lpBuffer);
	*lpBuffer=0;
	return;
#endif
}
/*-------------------------------------------------------------------------*/
void ExpMacroError( BYTE *lp1 )
//
//	error
//
{
	sprintf( lp1,"#mle " );
	lp1+=strlen( lp1 );
	*lp1++ = 0;
	return;
}
/*-------------------------------------------------------------------------*/
UCHAR* ExpMacro( int num, int *lpFlag )
//
//	macro を展開
//
//(in)	num     = 何個目のマクロか
//
//(out)	*lpFlag = 1:macro / 2:defineか
//		return  = macro の top
{
	UCHAR *lp0,*lp0org,*lp1,*lp3,*lpPara[MAXDEFINEPARA],*lpParaData[MAXDEFINEPARA];
	static	UCHAR buffer0[MAX_MACROEXP];
	UCHAR para0[MAXTOKEN];
	int c,d,d2,nPara,nLocal,type,typepara,gnum,nLine,MinPara,nUsePara,fUsePara;
//;;;make MAX_MACROEXP

	lp1 = buffer0;

	//	#l2( line2 )
//;;;make
//	if( nMacroNest > FileData[NowFile].nMacroNest ){
//		c = MacroLine[nMacroNest];
//		lp3 = lpMacroFile[nMacroNest];
//	} else {
		nLine = FileData[NowFile].line;
		lp3 = FileData[NowFile].filename;
//	}
	sprintf( lp1,"\n#l2 %d \"%s\"\n",nLine, lp3 );
	yen2esc( lp1 );
	lp1+=strlen( lp1 );

	lp0org = lp0 = lpDefine[num];
	type = *((int*)lp0)++;			// type によって、#macro か #define か
	nPara = *((int*)lp0)++;
	nLocal = *((int*)lp0)++;
	MinPara = *((int*)lp0)++;

	gnum = GetGlobalNum();

	//
	//	GetPara の type
	//
	typepara = 0;
	if( type == 3 ) {
		typepara++;
	}

	//
	//	macro の nest の数
	//
	nMacroNest++;
	if( nMacroNest >= MAXMACRONEST ) {
		err_pre0(1);
		printf( "preprocessor error:macro の nset が深すぎます ( 最大 %d 回 )\n", MAXMACRONEST );
		ExpMacroError( lp1 );
		return buffer0;
	}

	NowLine = -1;
	OldNowFile = -1;

	lpMacroData[nMacroNest] = lpDefine[num];
	lpMacroFile[nMacroNest] = *((UCHAR**)lp0)++;
	MacroLine[nMacroNest] = *((int*)lp0)++;
	*lpFlag = 1;
	MacroLine[nMacroNest] -= 2;

	// マクロ内で同じマクロを二重に展開していないかチェックする
	for( c=1;c<=nMacroNest-1;c++ ) {
		if( lp0org==lpMacroData[c] ) {
			err_pre0(1);
			printf( "preprocessor error:macro の中で同名の macro を二度展開しています\n" );
			printf( "%s(%d) 使用されている場所はここです\n", lpMacroFile[c], MacroLine[c] );
			ExpMacroError( lp1 );
			return buffer0;
		}
	}


	//
	//	macro の para を展開
	//
	if( (nPara+nLocal) == 0 ) {
		strcpy( lp1,lp0 );
		lp1+=strlen( lp1 );

		//
		//	現在の line
		//
		sprintf( lp1,"\n#mle " );
		lp1+=strlen( lp1 );
		*lp1++ = 0;
	} else {
		//
		//	'(' が無かった場合は、macro ではなく、ただの token
		//
		SkipSpace();
		if( type == 2 ) {
			d = GetCharDataOne();
			if( d != '(' ) {
				*lpFlag = 2;
				UnGetCharDataOne( d );	
				return lpRegist[num]+HASH_STR;
			}
		}

		//
		//	para name / local name への pointer の拾得
		//
		for( c=0; c<(nPara+nLocal); c++ ){
			d = *((int*)lp0)++;
			lpPara[c] = lp0;
			lp0 += d;
		}


		//
		//	 実 para を buffer0 へ #define 命令として登録
		//
		nUsePara=0;
		fUsePara=1;
		for( c=0; c<nPara; c++ ){
			SkipSpace();
			if( fUsePara ) nUsePara++;
			sprintf( lp1, TOKEN_PDEFINE" "CCNAME"M__%d__%s ",gnum,lpPara[c] );
			lp1 += strlen( lp1 );
			lpParaData[c] = lp1;
			lp1 = GetPara( lp1,typepara,gnum );
			*lp1++ = '\n';
			MacroLine[nMacroNest]--;

			SkipSpace();
			d = GetCharDataOne();
			d2 = 0;
			fUsePara=0;
			if( d == ')' ) {
				SkipSpace();
				d2 = GetCharDataOne();
				UnGetCharDataOne( d2 );
			}
			if( d2 != ',' && d2 != 0 ) {
				UnGetCharDataOne( d );
			} else if( d == ',' ) {
				fUsePara=1;
				if( c == (nPara+nLocal)-1 ) {
					UnGetCharDataOne( d );
				}
			} else {
				if( type == 3 && ( c == nPara-1 || d == '\n' || d == 0x1a )) {
					UnGetCharDataOne( d );
				} else {
					err_pre0(1);
					printf( "preprocessor error:macro1 の %d 番目のパラメーターが異常です\n",c+1 );
					ExpMacroError( lp1 );
					return buffer0;
				}
			}
		}
		if( nUsePara<MinPara ) {
			err_pre0(1);
			printf( "preprocessor error:macro1 の実パラメーターが少ないです。( 仮パラメーター %d 個,実パラメーター %d 個 )\n", nPara,nUsePara );
			ExpMacroError( lp1 );
			return buffer0;
		}
		SkipSpace();
		d = GetCharDataOne();
		if( d != ')' ) {
			if( nPara == 0 || type == 3 ) {
				UnGetCharDataOne( d );
			} else {
				err_pre0(1);
				printf( "preprocessor error:macro の実パラメーターが異常、もしくは実パラメーターが仮パラメーターより多いです ( 仮パラメーター %d 個 )\n", (nPara+nLocal)+1 );
				ExpMacroError( lp1 );
				return buffer0;
			}
		}

		//
		//	para name を変換しながら copy する
		//
		UnGetCharDataOne( 0 );
		UnGetStrData( lp0 );
		while( 1 ){
			c = GetCharDataOne();
			if( c == 0 ) {
				break;
			} else if( c == '\\' ) {
				c = GetCharDataOne();
				if( c!='0' ) {
					// esc
					*lp1++ = '\\';
					if( _iskanji(c) ) {
						*lp1++ = c;
						*lp1++ = GetCharDataOne();
					} else {
						*lp1++ = c;
					}
				} else {
					// \0 で始まる場合、パラメーターを直接展開する
					lp3 = para0;
					while( _isname1(c = GetCharDataOne())) {
						*lp3++ = c;
					}
					UnGetCharDataOne( c );
					*lp3++ = 0;
					lp3 = para0;
					for( c=0;c<(nPara+nLocal);c++ ) {
						if( strcmp( para0,lpPara[c] ) == 0 ) {
							lp3 = NULL;
							break;
						}
					}
					if( lp3 == para0 ) {
						// パラメーターではない場合
						err_pre0(1);
						printf( "preprocessor error:macro のパラメーターで無いものに \\0 がついています ( \"%s\" )\n", para0 );
						ExpMacroError( lp1 );
						return buffer0;
					} else if( c>=nPara ){
						// ローカルの時
						err_pre0(1);
						printf( "preprocessor error:macro のローカルパラメーターに \\0 がついています ( \"%s\" )\n", para0 );
						ExpMacroError( lp1 );
						return buffer0;
					} else {
						lp3 = lpParaData[c];
						while( *lp3>=' ' ) {
							*lp1++ = *lp3++;
						}
					}
				}
			} else if( c == '"' ) {
				lp1 = GetTokenSub0x05( lp1, MODE_NORMAL,gnum,nPara+nLocal,lpPara );
			} else if( c == '\'' ) {
				lp1 = GetTokenSub0x06( lp1, MODE_NORMAL );
			} else if( c == ';' ) {
				GetTokenSub0x0e();
				*lp1++ = ' ';
			} else if( c == '/' ) {
				if( GetTokenSub0x04() == 1 ) {
					*lp1++ = ' ';
				} else {
					*lp1++ = '/';
				}
			} else if( _isname0(c) ) {
				// パラメーターを展開するかチェック
				lp3 = para0;
				*lp3++ = c;
				while( _isname1(c = GetCharDataOne())) {
					*lp3++ = c;
				}
				UnGetCharDataOne( c );
				*lp3++ = 0;
				lp3 = para0;
				for( c=0;c<(nPara+nLocal);c++ ) {
					if( strcmp( para0,lpPara[c] ) == 0 ) {
						lp3 = NULL;
						break;
					}
				}
				if( lp3 == para0 ) {
					strcpy( lp1, lp3 );
					lp1 += strlen( lp1 );
				} else {
					sprintf( lp1, CCNAME"M__%d__%s",gnum,para0 );
					lp1 += strlen( lp1 );
				}
			} else if( _iskanji(c) ) {
				*lp1++ = c;
				*lp1++ = GetCharDataOne();
			} else {
				*lp1++ = c;
			}
		}

		//
		//	パラメーターを #undef
		//
		*lp1++ = '\n';
		for( c=(nPara+nLocal-1); c>=0; c-- ){
			sprintf( lp1, TOKEN_PUNDEF" "CCNAME"M__%d__%s\n",gnum,lpPara[c] );
			lp1 += strlen( lp1 );
		}

		//
		//	現在の line
		//
		sprintf( lp1,"#mle " );
		lp1+=strlen( lp1 );
		*lp1++ = 0;
	}



	return buffer0;
}
/*-------------------------------------------------------------------------*/
UCHAR* ExpFunction( int num, int *lpFlag )
//
//	function を展開
//
{
	UCHAR *lp0,*lp0org,*lp1;
	static UCHAR buffer0[MAX_MACROEXP];
	int c,d,nPara,nLocal,type,typepara,gnum;

	lp1 = buffer0;

	lp0org = lp0 = lpDefine[num];
	type = *((int*)lp0)++;			// type によって、#afunction か #function か
	nPara = *((int*)lp0)++;
	nLocal = *((int*)lp0)++;
	//MinPara = *((int*)lp0)++;

	gnum = GetGlobalNum();

	*lpFlag = 1;


	//	GetPara の type
	//
	typepara = 0;
	if( type == 5 ) {
		typepara++;
	}

	//
	//	macro の para を展開
	//
	if( typepara == 0 ) {
		strcpy( lp1,FuncCall );
		lp1 += strlen( lp1 );
		*lp1++ = '(';
	} else {
		strcpy( lp1,FuncCalla );
		lp1 += strlen( lp1 );
		*lp1++ = ' ';
	}



	//
	//	'(' が無かった場合は、function ではなく、ただの token
	//
	SkipSpace();
	if( type == 4 ) {
		d = GetCharDataOne();
		if( d != '(' ) {
			*lpFlag = 2;
			UnGetCharDataOne( d );	
			return lpRegist[num]+HASH_STR;
		}
	}


	//
	//	function を使用した
	//
	if( fUseFunc[num]==0 ) fUseFunc[num]=1;		// function を使用したかどうかフラグ ( 0:未使用 1:使用した 2:使用した物で登録済み )


	//
	//
	//	サブルーチンのラベル名、スタックポインター、パラメーター数を出力する
	//
	sprintf( lp1, CCNAME"_FUNC_%s,%s,%d ",GetTokenStr(num),FuncStack,nPara+nLocal );
	lp1 += strlen( lp1 );
	*lp1 = 0;

	//
	//	 実 para を copy する
	//
	for( c=0; c<nPara; c++ ){
		// , を出力
		*lp1++ = ',';
		*lp1 = 0;

		SkipSpace();
		lp1 = GetPara( lp1,typepara,gnum );

		SkipSpace();
		d = GetCharDataOne();
		if( d==',' ) {
			if( c==nPara-1 ) {
				// 最後のパラメーターに , が付く場合 error
				err_pre0(0);
				printf( "preprocessor warning:function のパラメーターが %d 個より多いです\n",c+1 );
				ExpMacroError( lp1 );
				return buffer0;
			}
		} else if( d==')' ) {
			if( type==4 && c==nPara-1 ) {
				// 最後のパラメーターの後の ) の場合
				UnGetCharDataOne( d );
			} else {
				// 最後のパラメーター以外に ) が付く場合 error
				err_pre0(0);
				printf( "preprocessor error:function の ) が異常です\n" );
				ExpMacroError( lp1 );
				return buffer0;
			}
		} else {
			if( type==5 && c==nPara-1 ) {
				// 最後のパラメーターの後に ) か , 以外の文字の場合
				UnGetCharDataOne( d );
			} else {
				// 最後のパラメーターに ) か , 以外の文字が付く場合 error
				err_pre0(0);
				printf( "preprocessor error:function の %d 番目のパラメーターが異常です\n",c+1 );
				ExpMacroError( lp1 );
				return buffer0;
			}
		}
	}

	// 最後のパラメーターの ) をチェック
	if( type==4 ) {
		d = GetCharDataOne();
		if( d==')' ) {
			*lp1++ = ')';
			*lp1 = 0;
		} else {
			err_pre0(0);
			printf( "preprocessor error:function の ) がありません\n" );
			ExpMacroError( lp1 );
			return buffer0;
		}
	}
	*lp1 = 0;

	return buffer0;
}
//---------------------------------------------------------------------------
BYTE* EndExpFunctionSub( int num )
//
//	function ソースファイルの最後に展開
//
//#ifdef _game_function_use
//#define	_game_function_p1	(*(rsp+4))
//#define	_game_function_p2	(*(rsp+3))
//#define	_game_function_l1	(*(rsp+2))
//#define	_game_function_l2	(*(rsp+1))
//____W95CC___FUNC__game:
//	_if _game_function_p1==_game_function_p2
//		_return
//	_end
//
//	_game_function_p1 = _game_function_p1 + 1
//	_game_function_p2 = _game_function_p2 - 1
//
//	_dw_int _game_function_p1,_game_function_p1
//
//	_dw_int rsp,rsp
//	_game _game_function_p1,_game_function_p2
//
//	_return
//#undef	_game_function_p1
//#undef	_game_function_p2
//#undef	_game_function_l1
//#undef	_game_function_l2
//#endif
{
	UCHAR *lp0,*lp0org,*lp1,*lp3,*lpPara[MAXDEFINEPARA],*lpParaData[MAXDEFINEPARA];
	static	UCHAR buffer0[MAX_MACROEXP];
	UCHAR para0[MAXTOKEN];
	int c,d,nPara,nLocal,type,typepara,gnum;

	lp1 = buffer0;

	lp0org = lp0 = lpDefine[num];
	type = *((int*)lp0)++;			// type によって、#macro か #define か
	nPara = *((int*)lp0)++;
	nLocal = *((int*)lp0)++;
//	MinPara = *((int*)lp0)++;
	lp0+=4;


	gnum = GetGlobalNum();

	//
	//	GetPara の type
	//
	typepara = 0;
	if( type == 5 ) {
		typepara++;
	}


	NowLine = -1;
	OldNowFile = -1;

	// 現在の行やfileも無理矢理 #function の行にする
	strcpy( FileData[NowFile].filename, *((UCHAR**)lp0)++);
	FileData[NowFile].line = (*((int*)lp0)++);

	//
	//	macro の para を展開
	//
	if( (nPara+nLocal) == 0 ) {
		strcpy( lp1,lp0 );
		lp1+=strlen( lp1 );
		*lp1++ = 0;
	} else {
		SkipSpace();

		//
		//	para name / local name への pointer の拾得
		//
		for( c=0; c<(nPara+nLocal); c++ ){
			d = *((int*)lp0)++;
			lpPara[c] = lp0;
			lp0 += d;
		}


		//
		//	 para を  #define 命令として登録
		//
		for( c=0; c<nPara+nLocal; c++ ){
			sprintf( lp1, TOKEN_PDEFINE" "CCNAME"M__%d__%s (*(%s+%d))",gnum,lpPara[c],FuncStack,nPara+nLocal-c );
			lp1 += strlen( lp1 );
			*lp1++ = '\n';
			FileData[NowFile].line --;
		}


		//
		//		ラベルを出力する
		//
		sprintf( lp1, CCNAME"_FUNC_%s:\n",GetTokenStr(num) );
		lp1+=strlen( lp1 );
		FileData[NowFile].line --;


		//
		//	para name を変換しながら copy する
		//
		UnGetCharDataOne( 0 );
		UnGetStrData( lp0 );
		while( 1 ){
			c = GetCharDataOne();
			if( c == 0 ) {
				break;
			} else if( c == '\\' ) {
				c = GetCharDataOne();
				if( c!='0' ) {
					// esc
					*lp1++ = '\\';
					if( _iskanji(c) ) {
						*lp1++ = c;
						*lp1++ = GetCharDataOne();
					} else {
						*lp1++ = c;
					}
				} else {
					// \0 で始まる場合、パラメーターを直接展開する
					lp3 = para0;
					while( _isname1(c = GetCharDataOne())) {
						*lp3++ = c;
					}
					UnGetCharDataOne( c );
					*lp3++ = 0;
					lp3 = para0;
					for( c=0;c<(nPara+nLocal);c++ ) {
						if( strcmp( para0,lpPara[c] ) == 0 ) {
							lp3 = NULL;
							break;
						}
					}
					if( lp3 == para0 ) {
						// パラメーターではない場合
						err_pre0(1);
						printf( "preprocessor error:macro のパラメーターで無いものに \\0 がついています ( \"%s\" )\n", para0 );
						ExpMacroError( lp1 );
						return buffer0;
					} else if( c>=nPara ){
						// ローカルの時
						err_pre0(1);
						printf( "preprocessor error:macro のローカルパラメーターに \\0 がついています ( \"%s\" )\n", para0 );
						ExpMacroError( lp1 );
						return buffer0;
					} else {
						lp3 = lpParaData[c];
						while( *lp3>=' ' ) {
							*lp1++ = *lp3++;
						}
					}
				}
			} else if( c == '"' ) {
				lp1 = GetTokenSub0x05( lp1, MODE_NORMAL,gnum,nPara+nLocal,lpPara );
			} else if( c == '\'' ) {
				lp1 = GetTokenSub0x06( lp1, MODE_NORMAL );
			} else if( c == ';' ) {
				GetTokenSub0x0e();
				*lp1++ = ' ';
			} else if( c == '/' ) {
				if( GetTokenSub0x04() == 1 ) {
					*lp1++ = ' ';
				} else {
					*lp1++ = '/';
				}
			} else if( _isname0(c) ) {
				// パラメーターを展開するかチェック
				lp3 = para0;
				*lp3++ = c;
				while( _isname1(c = GetCharDataOne())) {
					*lp3++ = c;
				}
				UnGetCharDataOne( c );
				*lp3++ = 0;
				lp3 = para0;
				for( c=0;c<(nPara+nLocal);c++ ) {
					if( strcmp( para0,lpPara[c] ) == 0 ) {
						lp3 = NULL;
						break;
					}
				}
				if( lp3 == para0 ) {
					strcpy( lp1, lp3 );
					lp1 += strlen( lp1 );
				} else {
					sprintf( lp1, CCNAME"M__%d__%s",gnum,para0 );
					lp1 += strlen( lp1 );
				}
			} else if( _iskanji(c) ) {
				*lp1++ = c;
				*lp1++ = GetCharDataOne();
			} else {
				*lp1++ = c;
			}
		}

		//
		//	パラメーターを #undef
		//
		*lp1++ = '\n';
		for( c=(nPara+nLocal-1); c>=0; c-- ){
			sprintf( lp1, TOKEN_PUNDEF" "CCNAME"M__%d__%s\n",gnum,lpPara[c] );
			lp1 += strlen( lp1 );
		}

		*lp1++ = 0;
	}


	return buffer0;
}
//---------------------------------------------------------------------------
BYTE* EndExpFunction( void )
//
//	function ソースファイルの最後に展開
//
{
	UINT num;

	for( num=0;num<cRegist;num++ ) {
		if( fUseFunc[num]==1 ) {
			fUseFunc[num]=2;
			return EndExpFunctionSub( num );
		}
	}
	return NULL;
}
/*---------------------------------------------------------------------------
		command
---------------------------------------------------------------------------*/
void CommandDefine( int nType )
//
//	#define 命令
//
{
	UCHAR *lp0,*lp1,*lpSize,*lpMax;
	int c,nPara,MinPara;
	UCHAR token[MAXTOKEN];

	SkipSpace();
	lp0 = GetToken( token );

	CheckRegistToken( lp0 );
	RegistToken( lp0, cDefine );

	lpMax = lpDefineEnd;
	lpDefine[cRegist-1] = lpDefineEnd;
	((int*)lpDefineEnd)+=MACROPARA-2;
	*((int*)lpDefineEnd)++ = NowFile;
	*((int*)lpDefineEnd)++ = FileData[NowFile].line;


	nPara = 0;
	c = GetCharDataOne();
	if( c == '(' ) {
		SkipSpace();
		//
		//	パラメーター拾得
		//
		while( 1 ) {
			SkipSpace();
			c = GetCharDataOne();
			if( c == ')' ) {
				break;
			} else if( nPara == 0 ) {
				UnGetCharDataOne( c );
			} else if( nPara != 0 ) {
				if( c != ',' ) {
					err_pre0(1);
					printf( "preprocessor error:macro の %d 番目の仮パラメーターが異常です\n",nPara+1 );
					return;
				}
			}
			SkipSpace();

			lpSize = lpDefineEnd;
			((int*)lpDefineEnd)++;
			lp1 = lpDefineEnd;

			c = GetCharDataOne();
			if( !_isname0(c) ) {
				err_pre0(1);
				printf( "preprocessor error:macro の %d 番目の仮パラメーターが異常です\n",nPara+1 );
				return;
			}
			*lpDefineEnd++ = c;
			while( _isname1(c = GetCharDataOne())) {
				*lpDefineEnd++ = c;
			}
			UnGetCharDataOne( c );
			*lpDefineEnd++ = 0;
			*((int*)lpSize) = strlen( lp1 ) + 1;

			nPara++;
			if( nPara > MAXDEFINEPARA ) {
				err_pre0(1);
				printf( "preprocessor error:macro の仮パラメーターが多すぎます ( 最大 %d 個 )\n",MAXDEFINEPARA );
				return;
			}
		}
	} else {
		UnGetCharDataOne( c );
	}
	MinPara=0;

	//
	//	macro 本体部分の登録
	//
	SkipSpace();
	GetTokenDefine( lpDefineEnd );			// macro 本体部分を拾得
	lpDefineEnd += ( strlen( lpDefineEnd ) + 1 );
	*(((int*)lpDefine[cRegist-1])) = nType;					// type 0:#define/ 1:#adeine
	*(((int*)lpDefine[cRegist-1])+1) = nPara;
	*(((int*)lpDefine[cRegist-1])+2) = 0;
	*(((int*)lpDefine[cRegist-1])+3) = MinPara;				// min
	cDefine++;

	if( lpDefineEnd-lpMax > MAX_MACROEXP ) {		// macro buffer が破綻しないか？
		err_pre0(1);
		printf( "preprocessor error:１個の macro としては、大きすぎます ( 最大 %d bytes )\n", MAX_MACROEXP );
	}

}
/*-------------------------------------------------------------------------*/
void CommandUnstr( void )
//
//
//
{
	UCHAR *lp0;
	int c;
	UCHAR token[MAXTOKEN];

	SkipSpace();
	lp0 = GetToken( token );

	if( GetTokenNum( token, &c ) == NUM_UNDEFINED ) {
		err_pre0(0);
		printf( "preprocessor warning:#unstr のパラメーターに登録されてない定数 \"%s\" を使用しました\n", lp0 );
		return;
	}

	if( *(((int*)lpDefine[c])+1) != 0 ) {
		err_pre0(0);
		printf( "preprocessor warning:#unstr のパラメーターにパラメーターがある文字列定数ではない \"%s\" を使用しました\n", lp0 );
		return;
	}

	lp0 = lpDefine[c]+MACROPARA*4;

	if( *lp0 != '\"' ) {
		err_pre0(0);
		printf( "preprocessor warning:#unstr のパラメーターに文字列定数ではない \"%s\" を使用しました\n", lp0 );
		return;
	}

	c = strlen( lp0 );


	if( *(lp0+c-1) != '\"' ) {
		err_pre0(0);
		printf( "preprocessor warning:#unstr のパラメーターに文字列定数ではない \"%s\" を使用しました\n", lp0 );
		return;
	}

	*(lp0+c-1) = 0;
	strcpy( lp0,lp0+1 );
}
/*-------------------------------------------------------------------------*/
void CommandInclude( void )
//
//	#include 命令
//
{
	UCHAR *lp0,*lpPath0,*lpPath1;
	int c,d;
	UCHAR token[MAXTOKEN];

	if( NowFile >= (MAXINCLUDEFILE-1) ) {
		err_pre0(1);
		printf( "preprocessor error:#include more then %s\n", (NowFile+1) );
		return;
	}
	SkipSpace();
	lp0 = GetTokenInclude( token );
	*(lp0+(strlen( lp0 )-1)) = 0;
	d = 0;

	if( *lp0 == '"' ) {
		// 絶対 path
		if( *(lp0+1) == '/' || *(lp0+1) == '\\' || *(lp0+2) == ':' ) {
			NowFile++;
			strcpy( FileData[NowFile].filename, lp0+1 );
			if( open_file( NowFile ) == FALSE ) {
				err_pre0(1);
				printf( "preprocessor error:\"%s\" が開けません\n", lp0+1 );
				return;
			}
			puttoken( "\n" );
			PrintLine();
			d = 1;
		// 相対 path
		} else {
			c=NowFile;
			while( c >= 0 && d == 0 ){
				strcpy( FileData[NowFile+1].filename, FileData[NowFile].filename );
				lpPath0 = FileData[NowFile+1].filename;
				lpPath1 = lpPath0;
				while( *lpPath1 != 0 ) {
					if( *lpPath1 == '\\' ) {
						lpPath0 = lpPath1 + 1;
					}
					lpPath1++;
				}
				strcpy( lpPath0, lp0+1 );
				if( open_file( (NowFile+1) ) == TRUE ) {
					NowFile++;
					puttoken( "\n" );
					PrintLine();
					d = 1;
				}
				c--;
			}
		}
	}

	c = 0;
	while( c < nIncludePath && d == 0 ){
		strcpy( FileData[NowFile+1].filename, IncludePath[c] );
		strcat( FileData[NowFile+1].filename, lp0+1 );
		if( open_file( (NowFile+1) ) == TRUE ) {
			NowFile++;
			puttoken( "\n" );
			PrintLine();
			d = 1;
		}
		c++;
	}

	if( d == 0 ) {
		err_pre0(1);
		printf( "preprocessor error:\"%s\" が開けません\n", lp0+1 );
		return;
	}
}
/*-------------------------------------------------------------------------*/
void CommandMacro( int nType )
//
//	#macro 命令
//
{
	UCHAR *lp0,*lp1,*lpSize,*lpMax;
	int c,nPara,nLocal,flag,MinPara;
	UCHAR token[MAXTOKEN];

	//
	//	マクロのパラメーターは
	// 		00h:type 2:#macro / 3:#amacro
	//		04h:パラメーターの数
	//		08h:localc ラベルの数
	//		0ch:最小限のパラメーターの数
	//		10h:マクロが記述されているファイルのファイル名へのポインター ( error の時に利用する ) MacroDefineFile(ポインターはlpFileEnd)にファイル名を保持し、ここではポインターを記録するff
	//		14h:マクロが記述されている行を登録する ( error の時に利用する )


	SkipSpace();
	lp0 = GetToken( token );							// マクロ名拾得

	CheckRegistToken( lp0 );							// マクロ名の２重登録チェック
	RegistToken( lp0, cDefine );						// マクロ名登録

	lpMax = lpDefineEnd;								// マクロバッファーが破綻していないかのチェックのため
	lpDefine[cRegist-1] = lpDefineEnd;					// マクロ実体へのポインターをセットする
	((int*)lpDefineEnd)+=MACROPARA-2;					// パラメーターの分だけポインターを進める
	// *((int*)lpDefineEnd)++ = NowFile;
	*((UCHAR**)lpDefineEnd)++ = lpFileEnd;				// マクロが記述されているファイルのファイル名を登録する ( error の時に利用する ) MacroDefineFile(ポインターはlpFileEnd)にファイル名を保持し、ここではポインターを記録する
	lp1 = lpFileEnd;
	strcpy( lpFileEnd, FileData[NowFile].filename );	// ファイル名はバッファーへ転送する
	lpFileEnd += strlen( lp1 ) +1;
	*((int*)lpDefineEnd)++ = FileData[NowFile].line;	// マクロが記述されている行を登録する ( error の時に利用する )


	//
	//	パラメーター拾得
	//
	SkipSpace();
	nPara = 0;
	c = GetCharDataOne();
	if( c == '(' ) {
		SkipSpace();
		while( 1 ) {
			SkipSpace();
			c = GetCharDataOne();
			if( c == ')' ) {
				break;
			} else if( nPara == 0 ) {
				UnGetCharDataOne( c );
			} else if( nPara != 0 ) {
				if( c != ',' ) {
					err_pre0(1);
					printf( "preprocessor error:macro/fucntion の %d 番目の仮パラメーターが異常です\n",nPara+1 );
					return;
				}
			}
			SkipSpace();

			lpSize = lpDefineEnd;
			((int*)lpDefineEnd)++;
			lp1 = lpDefineEnd;

			c = GetCharDataOne();
			if( !_isname0(c) ) {
				err_pre0(1);
				printf( "preprocessor error:macro/function の %d 番目の仮パラメーターが異常です\n",nPara+1 );
				return;
			}
			*lpDefineEnd++ = c;
			while( _isname1(c = GetCharDataOne())) {
				*lpDefineEnd++ = c;
			}
			UnGetCharDataOne( c );
			*lpDefineEnd++ = 0;
			*((int*)lpSize) = strlen( lp1 ) + 1;

			nPara++;
			if( nPara > MAXDEFINEPARA ) {
				err_pre0(1);
				printf( "preprocessor error:macro/function の仮パラメーターが多すぎます ( 最大 %d 個 )\n",MAXDEFINEPARA );
				return;
			}
		}
	} else {
		UnGetCharDataOne( c );
	}


	//
	//	local label の拾得
	//
	SkipSpace();
	nLocal = 0;
	c = GetCharDataOne();
	if( c == '(' ) {
		SkipSpace();
		while( 1 ) {
			SkipSpace();
			c = GetCharDataOne();
			if( c == ')' ) {
				break;
			} else if( nLocal == 0 ) {
				UnGetCharDataOne( c );
			} else if( nLocal != 0 ) {
				if( c != ',' ) {
					err_pre0(1);
					printf( "preprocessor error:macro/function の %d 番目のローカルパラメーターが異常です\n",nLocal+1 );
					return;
				}
			}
			SkipSpace();

			lpSize = lpDefineEnd;
			((int*)lpDefineEnd)++;
			lp1 = lpDefineEnd;

			c = GetCharDataOne();
			if( !_isname0(c) ) {
				err_pre0(1);
				printf( "preprocessor error:macro/function の %d 番目のローカルパラメーターが異常です\n",nLocal+1 );
				return;
			}
			*lpDefineEnd++ = c;
			while( _isname1(c = GetCharDataOne())) {
				*lpDefineEnd++ = c;
			}
			UnGetCharDataOne( c );
			*lpDefineEnd++ = 0;
			*((int*)lpSize) = strlen( lp1 ) + 1;

			nLocal++;
			if( nLocal > MAXDEFINELOCAL ) {
				err_pre0(1);
				printf( "preprocessor error:macro/function のローカルパラメーターが多すぎます ( 最大 %d 個 )\n",MAXDEFINELOCAL );
				return;
			}
		}
	} else {
		UnGetCharDataOne( c );
	}

	//
	//	min para の拾得
	//
	SkipSpace();
	c = GetCharDataOne();
	if( c == '(' ) {
		SkipSpace();
		c = GetCharDataOne();
		if( c=='*' ) {
			MinPara=nPara;
		} else if( c>='0' && c<='9' ) {
			MinPara=c-'0';
			c = GetCharDataOne();
			while( c>='0' && c<='9' ) {
				MinPara=MinPara*10 + c-'0';
				c = GetCharDataOne();
			}
			UnGetCharDataOne( c );
		} else {
			err_pre0(1);
			printf( "preprocessor error:macro/function の 最小パラメーター数の指定が異常です\n" );
			return;
		}
		SkipSpace();
		c = GetCharDataOne();
		if( c != ')' ) {
			err_pre0(1);
			printf( "preprocessor error:macro/function の 最小パラメーター数の指定が異常です\n" );
			return;
		}
	} else {
		MinPara=0;
		UnGetCharDataOne( c );
	}

	//
	//	macro 本体部分の登録 ( #endm まで )
	//
	SkipSpace();
	flag = 0;
	while( 1 ) {
		c = GetCharDataOne();
		if( c == 0x1a ) {
			err_pre0(1);
			if( nType<4 ) {
				printf( "preprocessor error:macro の定義が #endm で閉じていません\n" );
			} else {
				printf( "preprocessor error:function の定義が #endf で閉じていません\n" );
			}
			return;
		} else if( c == '\n' ) {
			flag = 1;
			*lpDefineEnd++ = c;
			NextLine();
		} else if( c == '\t' ||  c == ' ' ) {
			*lpDefineEnd++ = c;
		} else if( c == '#' && flag == 1 ) {
			lp0 = lpDefineEnd;
			*lpDefineEnd++ = c;
			c = GetCharDataOne();
			if( _isname0(c) ) {
				*lpDefineEnd++ = c;
				while( _isname1(c = GetCharDataOne())) {
					*lpDefineEnd++ = c;
				}
				UnGetCharDataOne( c );
				*lpDefineEnd = 0;
				if( nType<4 ) {
					if( strcmp( lp0,TOKEN_PENDM ) == 0 ) {
						lpDefineEnd-=(sizeof( TOKEN_PENDM )-1);
						*lpDefineEnd++=0;
						break;
					}
				} else {
					if( strcmp( lp0,TOKEN_PENDF ) == 0 ) {
						lpDefineEnd-=(sizeof( TOKEN_PENDF )-1);
						*lpDefineEnd++=0;
						break;
					}
				}
			} else {
				UnGetCharDataOne( c );
			}
			flag = 0;
		} else if( _iskanji(c) ) {
			flag = 0;
			*lpDefineEnd++ = c;
			*lpDefineEnd++ = GetCharDataOne();
		} else {
			flag = 0;
			*lpDefineEnd++ = c;
		}
	}

	*(((int*)lpDefine[cRegist-1])) = nType;					// type ( 2:#macro 3:#amacro 3:#function 4:#afunction )
	*(((int*)lpDefine[cRegist-1])+1) = nPara;
	*(((int*)lpDefine[cRegist-1])+2) = nLocal;
	*(((int*)lpDefine[cRegist-1])+3) = MinPara;				// min
	cDefine++;

	// #fucntion の場合、利用フラグを 0 にする
	if( nType>=4 ) fUseFunc[cRegist-1]=0;


	if( lpDefineEnd-lpMax > MAX_MACROEXP ) {		// macro buffer が破綻しないか？
		err_pre0(1);
		printf( "preprocessor error:１個の macro としては、大きすぎます ( 最大 %d bytes )\n", MAX_MACROEXP );
	}

}
/*-------------------------------------------------------------------------*/
void CalcError( int mode,UCHAR* lp0 )
{
	if( mode == 0 ) {
		err_pre0(1);
		printf( "preprocessor error:%s\n",lp0 );
		return;
	} else {
		err_compiler(1);
		printf( "compiler error:%s(%s)\n",lp0,lpCalcErr );
		err_next_compiler();
//		err_exit_compiler();
	}
}
/*-------------------------------------------------------------------------*/
void CalcSub( int mode )
{
	switch( CalcOpe[CalcSp0] ) {
//		case CALCODE_LS:
//			break;
		case CALCODE_PLUS:
			if( CalcSp1 <= 0 ) {
				CalcError( mode, "'+' の後に数値がありません" );
			}
			CalcValue[CalcSp1] = CalcValue[CalcSp1];
			break;
		case CALCODE_MINUS:
			if( CalcSp1 <= 0 ) {
				CalcError( mode, "'-' の後に数値がありません" );
			}
			CalcValue[CalcSp1] = - CalcValue[CalcSp1];
			break;
		case CALCODE_NOT:
			if( CalcSp1 <= 0 ) {
				CalcError( mode, "'~' の後に数値がありません" );
			}
			CalcValue[CalcSp1] = ~CalcValue[CalcSp1];
			break;
		case CALCODE_NEG:
			if( CalcSp1 <= 0 ) {
				CalcError( mode, "'!' の後に数値がありません" );
			}
			CalcValue[CalcSp1] = !CalcValue[CalcSp1];
			break;
		case CALCODE_MUL:
			if( CalcSp1 <= 1 ) {
				CalcError( mode, "'*' が異常です" );
			}
			CalcValue[CalcSp1-1] = CalcValue[CalcSp1-1] * CalcValue[CalcSp1];
			CalcSp1--;
			break;
		case CALCODE_DIV:
			if( CalcSp1 <= 1 ) {
				CalcError( mode, "'/' が異常です" );
			}
			CalcValue[CalcSp1-1] = CalcValue[CalcSp1-1] / CalcValue[CalcSp1];
			CalcSp1--;
			break;
		case CALCODE_MOD:
			if( CalcSp1 <= 1 ) {
				CalcError( mode, "'%' が異常です" );
			}
			CalcValue[CalcSp1-1] = CalcValue[CalcSp1-1] % CalcValue[CalcSp1];
			CalcSp1--;
			break;
		case CALCODE_ADD:
			if( CalcSp1 <= 1 ) {
				CalcError( mode, "'+' が異常です" );
			}
			CalcValue[CalcSp1-1] = CalcValue[CalcSp1-1] + CalcValue[CalcSp1];
			CalcSp1--;
			break;
		case CALCODE_SUB:
			if( CalcSp1 <= 1 ) {
				CalcError( mode, "'-' が異常です" );
			}
			CalcValue[CalcSp1-1] = CalcValue[CalcSp1-1] - CalcValue[CalcSp1];
			CalcSp1--;
			break;
		case CALCODE_SHL:
			if( CalcSp1 <= 1 ) {
				CalcError( mode, "'<<' が異常です" );
			}
			CalcValue[CalcSp1-1] = CalcValue[CalcSp1-1] << CalcValue[CalcSp1];
			CalcSp1--;
			break;
		case CALCODE_SHR:
			if( CalcSp1 <= 1 ) {
				CalcError( mode, "'>>' が異常です" );
			}
			CalcValue[CalcSp1-1] = CalcValue[CalcSp1-1] >> CalcValue[CalcSp1];
			CalcSp1--;
			break;
		case CALCODE_MORE:
			if( CalcSp1 <= 1 ) {
				CalcError( mode, "'>=' が異常です" );
			}
			CalcValue[CalcSp1-1] = ( CalcValue[CalcSp1-1] >= CalcValue[CalcSp1] );
			CalcSp1--;
			break;
		case CALCODE_BIG:
			if( CalcSp1 <= 1 ) {
				CalcError( mode, "'>' が異常です" );
			}
			CalcValue[CalcSp1-1] = ( CalcValue[CalcSp1-1] > CalcValue[CalcSp1] );
			CalcSp1--;
			break;
		case CALCODE_LESS:
			if( CalcSp1 <= 1 ) {
				CalcError( mode, "'<=' が異常です" );
			}
			CalcValue[CalcSp1-1] = ( CalcValue[CalcSp1-1] <= CalcValue[CalcSp1] );
			CalcSp1--;
			break;
		case CALCODE_SMALL:
			if( CalcSp1 <= 1 ) {
				CalcError( mode, "'<' が異常です" );
			}
			CalcValue[CalcSp1-1] = ( CalcValue[CalcSp1-1] < CalcValue[CalcSp1] );
			CalcSp1--;
			break;
		case CALCODE_EQU:
			if( CalcSp1 <= 1 ) {
				CalcError( mode, "'==' が異常です" );
			}
			CalcValue[CalcSp1-1] = ( CalcValue[CalcSp1-1] == CalcValue[CalcSp1] );
			CalcSp1--;
			break;
		case CALCODE_NEQU:
			if( CalcSp1 <= 1 ) {
				CalcError( mode, "'!=' が異常です" );
			}
			CalcValue[CalcSp1-1] = ( CalcValue[CalcSp1-1] != CalcValue[CalcSp1] );
			CalcSp1--;
			break;
		case CALCODE_BITAND:
			if( CalcSp1 <= 1 ) {
				CalcError( mode, "'&' が異常です" );
			}
			CalcValue[CalcSp1-1] = ( CalcValue[CalcSp1-1] & CalcValue[CalcSp1] );
			CalcSp1--;
			break;
		case CALCODE_BITXOR:
			if( CalcSp1 <= 1 ) {
				CalcError( mode, "'^' が異常です" );
			}
			CalcValue[CalcSp1-1] = ( CalcValue[CalcSp1-1] ^ CalcValue[CalcSp1] );
			CalcSp1--;
			break;
		case CALCODE_BITOR:
			if( CalcSp1 <= 1 ) {
				CalcError( mode, "'|' が異常です" );
			}
			CalcValue[CalcSp1-1] = ( CalcValue[CalcSp1-1] | CalcValue[CalcSp1] );
			CalcSp1--;
			break;
		case CALCODE_AND:
			if( CalcSp1 <= 1 ) {
				CalcError( mode, "'&&' が異常です" );
			}
			CalcValue[CalcSp1-1] = ( CalcValue[CalcSp1-1] && CalcValue[CalcSp1] );
			CalcSp1--;
			break;
		case CALCODE_OR:
			if( CalcSp1 <= 1 ) {
				CalcError( mode, "'||' が異常です" );
			}
			CalcValue[CalcSp1-1] = ( CalcValue[CalcSp1-1] || CalcValue[CalcSp1] );
			CalcSp1--;
			break;
//		case CALCODE_LE:
//			break;
	}
	CalcSp0--;
}
/*-------------------------------------------------------------------------*/
int Calc( UCHAR *lp0,int mode )
{
	int c,d;
	int before;

	lpCalcErr = lp0;

	// skip space
	while( *lp0=='\t' || *lp0==' ' ) {
		lp0++;
	}
	if( !_iscalc(*lp0) ){
		CalcError( mode, "計算式が異常です" );
	}

	CalcOpe[0]=0;							// 番兵
	CalcPri[0]=-1;

	CalcSp0=CalcSp1=0;
	before = 0;
	while( 1 ) {
		while( *lp0=='\t' || *lp0==' ' ) {
			lp0++;
		}

		// c = GetCharDataOne();
		c = *lp0++;
		if(( d = _isdec(c)) != -1 ) {
			CalcValue[++CalcSp1] = d;
			// while(( d = _isdec(c = GetCharDataOne())) != -1 ) {
			while(( d = _isdec((c = *lp0++))) != -1 ) {
				CalcValue[CalcSp1] = CalcValue[CalcSp1]*10 + d;
			}
			// UnGetCharDataOne( c );
			lp0--;
			before = 1;
		} else {
			switch( c ) {
				case '(':
					if( before == 1 ) {
						CalcError( mode, "計算式の '(' が異常です" );
					}
					d = CALCODE_LS;
					before = 0;
					break;
				case '~':
					d = CALCODE_NOT;
					break;
				case '!':
					// c = GetCharDataOne();
					c = *lp0++;
					if( c != '=' ) {
						// UnGetCharDataOne( c );
						lp0--;
						d = CALCODE_NEG;
					} else {
						d = CALCODE_NEQU;
						before = 0;
					}
					break;
				case '*':
					if( before == 0 ) {
						CalcError( mode, "計算式の '*' が異常です" );
					}
					d = CALCODE_MUL;
					before = 0;
					break;
				case '/':
					if( before == 0 ) {
						CalcError( mode, "計算式の '/' が異常です" );
					}
					d = CALCODE_DIV;
					before = 0;
					break;
				case '%':
					if( before == 0 ) {
						CalcError( mode, "計算式の '%' が異常です" );
					}
					d = CALCODE_MOD;
					before = 0;
					break;
				case '<':
					// c = GetCharDataOne();
					c = *lp0++;
					if( c == '<' ) {
						if( before == 0 ) {
							CalcError( mode, "計算式の '<<' が異常です" );
						}
						d = CALCODE_SHL;
						before = 0;
					} else if( c == '=' ) {
						if( before == 0 ) {
							CalcError( mode, "計算式の '<=' が異常です" );
						}
						d = CALCODE_LESS;
						before = 0;
					} else {
						if( before == 0 ) {
							CalcError( mode, "計算式の '<' が異常です" );
						}
						// UnGetCharDataOne( c );
						lp0--;
						d = CALCODE_SMALL;
						before = 0;
					}
					break;
				case '>':
					// c = GetCharDataOne();
					c = *lp0++;
					if( c == '>' ) {
						if( before == 0 ) {
							CalcError( mode, "計算式の '>>' が異常です" );
						}
						d = CALCODE_SHR;
						before = 0;
					} else if( c == '=' ) {
						if( before == 0 ) {
							CalcError( mode, "計算式の '>=' が異常です" );
						}
						d = CALCODE_MORE;
						before = 0;
					} else {
						if( before == 0 ) {
							CalcError( mode, "計算式の '>' が異常です" );
						}
						// UnGetCharDataOne( c );
						lp0--;
						d = CALCODE_BIG;
						before = 0;
					}
					break;
				case '=':
					// c = GetCharDataOne();
					c = *lp0++;
					if( c == '=' ) {
						if( before == 0 ) {
							CalcError( mode, "計算式の '==' が異常です" );
						}
						d = CALCODE_EQU;
						before = 0;
					} else {
						// UnGetCharDataOne( c );
						lp0--;
						// UnGetCharDataOne( '=' );
						lp0--;
						d = CALCODE_EXIT;
					}
					break;
				case '+':
					if( before == 0 ) {
						d = CALCODE_PLUS;
					} else {
						d = CALCODE_ADD;
						before = 0;
					}
					break;
				case '-':
					if( before == 0 ) {
						d = CALCODE_MINUS;
					} else {
						d = CALCODE_SUB;
						before = 0;
					}
					break;
				case '&':
					// c = GetCharDataOne();
					c = *lp0++;
					if( c != '&' ) {
						if( before == 0 ) {
							CalcError( mode, "計算式の '&' が異常です" );
						}
						// UnGetCharDataOne( c );
						lp0--;
						d = CALCODE_BITAND;
						before = 0;
					} else {
						if( before == 0 ) {
							CalcError( mode, "計算式の '&&' が異常です" );
						}
						d = CALCODE_AND;
						before = 0;
					}
					break;
				case '^':
					if( before == 0 ) {
						CalcError( mode, "計算式の '^' が異常です" );
					}
					d = CALCODE_BITXOR;
					before = 0;
					break;
				case '|':
					// c = GetCharDataOne();
					c = *lp0++;
					if( c != '|' ) {
						if( before == 0 ) {
							CalcError( mode, "計算式の '|' が異常です" );
						}
						// UnGetCharDataOne( c );
						lp0--;
						d = CALCODE_BITOR;
						before = 0;
					} else {
						if( before == 0 ) {
							CalcError( mode, "計算式の '||' が異常です" );
						}
						d = CALCODE_OR;
						before = 0;
					}
					break;
				case ')':
					if( before == 0 ) {
							CalcError( mode, "計算式の ')' が異常です" );
					}
					d = CALCODE_LE;
					break;
				default:
					// UnGetCharDataOne( c );
					lp0--;
					d = CALCODE_EXIT;
					break;
			}
			if( d == CALCODE_EXIT ) {
				break;
			}

			while( 1 ) {
				if( CalcOpe[CalcSp0] == CALCODE_LS ) {
					break;
				}
				if( CalcLeft[d] == 0 ) {
					// 左から右
					if( CalcPri[d] > CalcPri[CalcOpe[CalcSp0]] ) {
						break;
					}
				} else {
					// 右から左
					if( CalcPri[d] >= CalcPri[CalcOpe[CalcSp0]] ) {
						break;
					}
				}
				CalcSub( mode );
			}
			if( d != CALCODE_LE ) {
				CalcOpe[++CalcSp0] = d;
			} else {
				CalcSp0--;					// '(' を取り除く
			}
		}
	}

	while( CalcSp0>0 ) {					// 演算子 stack が空になるまで
		CalcSub( mode );
	}
	return CalcValue[1];
}
/*---------------------------------------------------------------------------
		main
---------------------------------------------------------------------------*/
void tokenmain( void )
{
	int c,d;
	UINT num;
	struct _stat fstatbuff;
	UCHAR *lp0,*lp1,*lp2;
	UCHAR token[MAXTOKEN];					// token buffer
	UCHAR macrobuffer[MAX_MACROEXP];
	HANDLE	hFind0;
	WIN32_FIND_DATA fd0;
//;;;make MAX_MACROEXP

	//
	//	定義済み macro の定義
	//
	// __DATE__
	lp1 = asctime( nowtime );
	token[0]  = '"';
	token[1]  = *(lp1+4);
	token[2]  = *(lp1+5);
	token[3]  = *(lp1+6);
	token[4]  = *(lp1+7);
	token[5]  = *(lp1+8);
	token[6]  = *(lp1+9);
	token[7]  = *(lp1+10);
	token[8]  = *(lp1+20);
	token[9]  = *(lp1+21);
	token[10] = *(lp1+22);
	token[11] = *(lp1+23);
	token[12] = '"';
	token[13] = '\n';
	token[14] = 0;
	UnGetStrData( token );
	UnGetStrData( " "TOKEN___DATE__" " );
	CommandDefine( 0 );
	// __TIME__
	lp1 = asctime( nowtime );
	token[0]  = '"';
	token[1]  = *(lp1+11);
	token[2]  = *(lp1+12);
	token[3]  = *(lp1+13);
	token[4]  = *(lp1+14);
	token[5]  = *(lp1+15);
	token[6]  = *(lp1+16);
	token[7]  = *(lp1+17);
	token[8]  = *(lp1+18);
	token[9]  = '"';
	token[10] = '\n';
	token[11] = 0;
	UnGetStrData( token );
	UnGetStrData( " "TOKEN___TIME__" " );
	CommandDefine( 0 );
	//	__TIMESTAMP__
	_stat( FileData[NowFile].filename, &fstatbuff );
	strcpy( &token[1], ctime( &fstatbuff.st_mtime ) );
	token[0] = '"';
	token[25] = '"';
	token[26] = '\n';
	token[27] = 0;
	UnGetStrData( token );
	UnGetStrData( " "TOKEN___TIMESTAMP__" " );
	CommandDefine( 0 );
	// __LINE__
	nTokenStart = cRegist;
	UnGetStrData( " "TOKEN___LINE__" 0" );
	CommandDefine( 0 );
	// __FILE__
	UnGetStrData( " "TOKEN___FILE__" 0" );
	CommandDefine( 0 );
	// __FILE__0D
	UnGetStrData( " "TOKEN__FILE__0D" 0" );
	CommandDefine( 0 );
	// __FILE__0DP
	UnGetStrData( " "TOKEN__FILE__0DP" 0" );
	CommandDefine( 0 );
	// __FILE__0DPF
	UnGetStrData( " "TOKEN__FILE__0DPF" 0" );
	CommandDefine( 0 );
	// __FILE__0DPFE
	UnGetStrData( " "TOKEN__FILE__0DPFE" 0" );
	CommandDefine( 0 );
	// __FILE__0P
	UnGetStrData( " "TOKEN__FILE__0P" 0" );
	CommandDefine( 0 );
	// __FILE__0PF
	UnGetStrData( " "TOKEN__FILE__0PF" 0" );
	CommandDefine( 0 );
	// __FILE__0PFE
	UnGetStrData( " "TOKEN__FILE__0PFE" 0" );
	CommandDefine( 0 );
	// __FILE__0F
	UnGetStrData( " "TOKEN__FILE__0F" 0" );
	CommandDefine( 0 );
	// __FILE__0FE
	UnGetStrData( " "TOKEN__FILE__0FE" 0" );
	CommandDefine( 0 );
	// __FILE__0E
	UnGetStrData( " "TOKEN__FILE__0E" 0" );
	CommandDefine( 0 );
	// __FILE__1D
	UnGetStrData( " "TOKEN__FILE__1D" 0" );
	CommandDefine( 0 );
	// __FILE__1DP
	UnGetStrData( " "TOKEN__FILE__1DP" 0" );
	CommandDefine( 0 );
	// __FILE__1DPF
	UnGetStrData( " "TOKEN__FILE__1DPF" 0" );
	CommandDefine( 0 );
	// __FILE__1DPFE
	UnGetStrData( " "TOKEN__FILE__1DPFE" 0" );
	CommandDefine( 0 );
	// __FILE__1P
	UnGetStrData( " "TOKEN__FILE__1P" 0" );
	CommandDefine( 0 );
	// __FILE__1PF
	UnGetStrData( " "TOKEN__FILE__1PF" 0" );
	CommandDefine( 0 );
	// __FILE__1PFE
	UnGetStrData( " "TOKEN__FILE__1PFE" 0" );
	CommandDefine( 0 );
	// __FILE__1F
	UnGetStrData( " "TOKEN__FILE__1F" 0" );
	CommandDefine( 0 );
	// __FILE__1FE
	UnGetStrData( " "TOKEN__FILE__1FE" 0" );
	CommandDefine( 0 );
	// __FILE__1E
	UnGetStrData( " "TOKEN__FILE__1E" 0" );
	CommandDefine( 0 );
	// __FILE__2D
	UnGetStrData( " "TOKEN__FILE__2D" 0" );
	CommandDefine( 0 );
	// __FILE__2DP
	UnGetStrData( " "TOKEN__FILE__2DP" 0" );
	CommandDefine( 0 );
	// __FILE__2DPF
	UnGetStrData( " "TOKEN__FILE__2DPF" 0" );
	CommandDefine( 0 );
	// __FILE__2DPFE
	UnGetStrData( " "TOKEN__FILE__2DPFE" 0" );
	CommandDefine( 0 );
	// __FILE__2P
	UnGetStrData( " "TOKEN__FILE__2P" 0" );
	CommandDefine( 0 );
	// __FILE__2PF
	UnGetStrData( " "TOKEN__FILE__2PF" 0" );
	CommandDefine( 0 );
	// __FILE__2PFE
	UnGetStrData( " "TOKEN__FILE__2PFE" 0" );
	CommandDefine( 0 );
	// __FILE__2F
	UnGetStrData( " "TOKEN__FILE__2F" 0" );
	CommandDefine( 0 );
	// __FILE__2FE
	UnGetStrData( " "TOKEN__FILE__2FE" 0" );
	CommandDefine( 0 );
	// __FILE__2E
	UnGetStrData( " "TOKEN__FILE__2E" 0" );
	CommandDefine( 0 );
	// __FILE__3D
	UnGetStrData( " "TOKEN__FILE__3D" 0" );
	CommandDefine( 0 );
	// __FILE__3DP
	UnGetStrData( " "TOKEN__FILE__3DP" 0" );
	CommandDefine( 0 );
	// __FILE__3DPF
	UnGetStrData( " "TOKEN__FILE__3DPF" 0" );
	CommandDefine( 0 );
	// __FILE__3DPFE
	UnGetStrData( " "TOKEN__FILE__3DPFE" 0" );
	CommandDefine( 0 );
	// __FILE__3P
	UnGetStrData( " "TOKEN__FILE__3P" 0" );
	CommandDefine( 0 );
	// __FILE__3PF
	UnGetStrData( " "TOKEN__FILE__3PF" 0" );
	CommandDefine( 0 );
	// __FILE__3PFE
	UnGetStrData( " "TOKEN__FILE__3PFE" 0" );
	CommandDefine( 0 );
	// __FILE__3F
	UnGetStrData( " "TOKEN__FILE__3F" 0" );
	CommandDefine( 0 );
	// __FILE__3FE
	UnGetStrData( " "TOKEN__FILE__3FE" 0" );
	CommandDefine( 0 );
	// __FILE__3E
	UnGetStrData( " "TOKEN__FILE__3E" 0" );
	CommandDefine( 0 );
	// __FILE__4D
	UnGetStrData( " "TOKEN__FILE__4D" 0" );
	CommandDefine( 0 );
	// __FILE__4DP
	UnGetStrData( " "TOKEN__FILE__4DP" 0" );
	CommandDefine( 0 );
	// __FILE__4DPF
	UnGetStrData( " "TOKEN__FILE__4DPF" 0" );
	CommandDefine( 0 );
	// __FILE__4DPFE
	UnGetStrData( " "TOKEN__FILE__4DPFE" 0" );
	CommandDefine( 0 );
	// __FILE__4P
	UnGetStrData( " "TOKEN__FILE__4P" 0" );
	CommandDefine( 0 );
	// __FILE__4PF
	UnGetStrData( " "TOKEN__FILE__4PF" 0" );
	CommandDefine( 0 );
	// __FILE__4PFE
	UnGetStrData( " "TOKEN__FILE__4PFE" 0" );
	CommandDefine( 0 );
	// __FILE__4F
	UnGetStrData( " "TOKEN__FILE__4F" 0" );
	CommandDefine( 0 );
	// __FILE__4FE
	UnGetStrData( " "TOKEN__FILE__4FE" 0" );
	CommandDefine( 0 );
	// __FILE__4E
	UnGetStrData( " "TOKEN__FILE__4E" 0" );
	CommandDefine( 0 );
	// __FILE__5D
	UnGetStrData( " "TOKEN__FILE__5D" 0" );
	CommandDefine( 0 );
	// __FILE__5DP
	UnGetStrData( " "TOKEN__FILE__5DP" 0" );
	CommandDefine( 0 );
	// __FILE__5DPF
	UnGetStrData( " "TOKEN__FILE__5DPF" 0" );
	CommandDefine( 0 );
	// __FILE__5DPFE
	UnGetStrData( " "TOKEN__FILE__5DPFE" 0" );
	CommandDefine( 0 );
	// __FILE__5P
	UnGetStrData( " "TOKEN__FILE__5P" 0" );
	CommandDefine( 0 );
	// __FILE__5PF
	UnGetStrData( " "TOKEN__FILE__5PF" 0" );
	CommandDefine( 0 );
	// __FILE__5PFE
	UnGetStrData( " "TOKEN__FILE__5PFE" 0" );
	CommandDefine( 0 );
	// __FILE__5F
	UnGetStrData( " "TOKEN__FILE__5F" 0" );
	CommandDefine( 0 );
	// __FILE__5FE
	UnGetStrData( " "TOKEN__FILE__5FE" 0" );
	CommandDefine( 0 );
	// __FILE__5E
	UnGetStrData( " "TOKEN__FILE__5E" 0" );
	CommandDefine( 0 );
	// __FILE__6D
	UnGetStrData( " "TOKEN__FILE__6D" 0" );
	CommandDefine( 0 );
	// __FILE__6DP
	UnGetStrData( " "TOKEN__FILE__6DP" 0" );
	CommandDefine( 0 );
	// __FILE__6DPF
	UnGetStrData( " "TOKEN__FILE__6DPF" 0" );
	CommandDefine( 0 );
	// __FILE__6DPFE
	UnGetStrData( " "TOKEN__FILE__6DPFE" 0" );
	CommandDefine( 0 );
	// __FILE__6P
	UnGetStrData( " "TOKEN__FILE__6P" 0" );
	CommandDefine( 0 );
	// __FILE__6PF
	UnGetStrData( " "TOKEN__FILE__6PF" 0" );
	CommandDefine( 0 );
	// __FILE__6PFE
	UnGetStrData( " "TOKEN__FILE__6PFE" 0" );
	CommandDefine( 0 );
	// __FILE__6F
	UnGetStrData( " "TOKEN__FILE__6F" 0" );
	CommandDefine( 0 );
	// __FILE__6FE
	UnGetStrData( " "TOKEN__FILE__6FE" 0" );
	CommandDefine( 0 );
	// __FILE__6E
	UnGetStrData( " "TOKEN__FILE__6E" 0" );
	CommandDefine( 0 );
	// __FILE__7D
	UnGetStrData( " "TOKEN__FILE__7D" 0" );
	CommandDefine( 0 );
	// __FILE__7DP
	UnGetStrData( " "TOKEN__FILE__7DP" 0" );
	CommandDefine( 0 );
	// __FILE__7DPF
	UnGetStrData( " "TOKEN__FILE__7DPF" 0" );
	CommandDefine( 0 );
	// __FILE__7DPFE
	UnGetStrData( " "TOKEN__FILE__7DPFE" 0" );
	CommandDefine( 0 );
	// __FILE__7P
	UnGetStrData( " "TOKEN__FILE__7P" 0" );
	CommandDefine( 0 );
	// __FILE__7PF
	UnGetStrData( " "TOKEN__FILE__7PF" 0" );
	CommandDefine( 0 );
	// __FILE__7PFE
	UnGetStrData( " "TOKEN__FILE__7PFE" 0" );
	CommandDefine( 0 );
	// __FILE__7F
	UnGetStrData( " "TOKEN__FILE__7F" 0" );
	CommandDefine( 0 );
	// __FILE__7FE
	UnGetStrData( " "TOKEN__FILE__7FE" 0" );
	CommandDefine( 0 );
	// __FILE__7E
	UnGetStrData( " "TOKEN__FILE__7E" 0" );
	CommandDefine( 0 );
	// __FILE0__
	UnGetStrData( " "TOKEN___FILE0__" 0" );
	CommandDefine( 0 );
	// __FILE0__0D
	UnGetStrData( " "TOKEN__FILE0__0D" 0" );
	CommandDefine( 0 );
	// __FILE0__0DP
	UnGetStrData( " "TOKEN__FILE0__0DP" 0" );
	CommandDefine( 0 );
	// __FILE0__0DPF
	UnGetStrData( " "TOKEN__FILE0__0DPF" 0" );
	CommandDefine( 0 );
	// __FILE0__0DPFE
	UnGetStrData( " "TOKEN__FILE0__0DPFE" 0" );
	CommandDefine( 0 );
	// __FILE0__0P
	UnGetStrData( " "TOKEN__FILE0__0P" 0" );
	CommandDefine( 0 );
	// __FILE0__0PF
	UnGetStrData( " "TOKEN__FILE0__0PF" 0" );
	CommandDefine( 0 );
	// __FILE0__0PFE
	UnGetStrData( " "TOKEN__FILE0__0PFE" 0" );
	CommandDefine( 0 );
	// __FILE0__0F
	UnGetStrData( " "TOKEN__FILE0__0F" 0" );
	CommandDefine( 0 );
	// __FILE0__0FE
	UnGetStrData( " "TOKEN__FILE0__0FE" 0" );
	CommandDefine( 0 );
	// __FILE0__0E
	UnGetStrData( " "TOKEN__FILE0__0E" 0" );
	CommandDefine( 0 );
	// __FILE0__1D
	UnGetStrData( " "TOKEN__FILE0__1D" 0" );
	CommandDefine( 0 );
	// __FILE0__1DP
	UnGetStrData( " "TOKEN__FILE0__1DP" 0" );
	CommandDefine( 0 );
	// __FILE0__1DPF
	UnGetStrData( " "TOKEN__FILE0__1DPF" 0" );
	CommandDefine( 0 );
	// __FILE0__1DPFE
	UnGetStrData( " "TOKEN__FILE0__1DPFE" 0" );
	CommandDefine( 0 );
	// __FILE0__1P
	UnGetStrData( " "TOKEN__FILE0__1P" 0" );
	CommandDefine( 0 );
	// __FILE0__1PF
	UnGetStrData( " "TOKEN__FILE0__1PF" 0" );
	CommandDefine( 0 );
	// __FILE0__1PFE
	UnGetStrData( " "TOKEN__FILE0__1PFE" 0" );
	CommandDefine( 0 );
	// __FILE0__1F
	UnGetStrData( " "TOKEN__FILE0__1F" 0" );
	CommandDefine( 0 );
	// __FILE0__1FE
	UnGetStrData( " "TOKEN__FILE0__1FE" 0" );
	CommandDefine( 0 );
	// __FILE0__1E
	UnGetStrData( " "TOKEN__FILE0__1E" 0" );
	CommandDefine( 0 );
	// __FILE0__2D
	UnGetStrData( " "TOKEN__FILE0__2D" 0" );
	CommandDefine( 0 );
	// __FILE0__2DP
	UnGetStrData( " "TOKEN__FILE0__2DP" 0" );
	CommandDefine( 0 );
	// __FILE0__2DPF
	UnGetStrData( " "TOKEN__FILE0__2DPF" 0" );
	CommandDefine( 0 );
	// __FILE0__2DPFE
	UnGetStrData( " "TOKEN__FILE0__2DPFE" 0" );
	CommandDefine( 0 );
	// __FILE0__2P
	UnGetStrData( " "TOKEN__FILE0__2P" 0" );
	CommandDefine( 0 );
	// __FILE0__2PF
	UnGetStrData( " "TOKEN__FILE0__2PF" 0" );
	CommandDefine( 0 );
	// __FILE0__2PFE
	UnGetStrData( " "TOKEN__FILE0__2PFE" 0" );
	CommandDefine( 0 );
	// __FILE0__2F
	UnGetStrData( " "TOKEN__FILE0__2F" 0" );
	CommandDefine( 0 );
	// __FILE0__2FE
	UnGetStrData( " "TOKEN__FILE0__2FE" 0" );
	CommandDefine( 0 );
	// __FILE0__2E
	UnGetStrData( " "TOKEN__FILE0__2E" 0" );
	CommandDefine( 0 );
	// __FILE0__3D
	UnGetStrData( " "TOKEN__FILE0__3D" 0" );
	CommandDefine( 0 );
	// __FILE0__3DP
	UnGetStrData( " "TOKEN__FILE0__3DP" 0" );
	CommandDefine( 0 );
	// __FILE0__3DPF
	UnGetStrData( " "TOKEN__FILE0__3DPF" 0" );
	CommandDefine( 0 );
	// __FILE0__3DPFE
	UnGetStrData( " "TOKEN__FILE0__3DPFE" 0" );
	CommandDefine( 0 );
	// __FILE0__3P
	UnGetStrData( " "TOKEN__FILE0__3P" 0" );
	CommandDefine( 0 );
	// __FILE0__3PF
	UnGetStrData( " "TOKEN__FILE0__3PF" 0" );
	CommandDefine( 0 );
	// __FILE0__3PFE
	UnGetStrData( " "TOKEN__FILE0__3PFE" 0" );
	CommandDefine( 0 );
	// __FILE0__3F
	UnGetStrData( " "TOKEN__FILE0__3F" 0" );
	CommandDefine( 0 );
	// __FILE0__3FE
	UnGetStrData( " "TOKEN__FILE0__3FE" 0" );
	CommandDefine( 0 );
	// __FILE0__3E
	UnGetStrData( " "TOKEN__FILE0__3E" 0" );
	CommandDefine( 0 );
	// __FILE0__4D
	UnGetStrData( " "TOKEN__FILE0__4D" 0" );
	CommandDefine( 0 );
	// __FILE0__4DP
	UnGetStrData( " "TOKEN__FILE0__4DP" 0" );
	CommandDefine( 0 );
	// __FILE0__4DPF
	UnGetStrData( " "TOKEN__FILE0__4DPF" 0" );
	CommandDefine( 0 );
	// __FILE0__4DPFE
	UnGetStrData( " "TOKEN__FILE0__4DPFE" 0" );
	CommandDefine( 0 );
	// __FILE0__4P
	UnGetStrData( " "TOKEN__FILE0__4P" 0" );
	CommandDefine( 0 );
	// __FILE0__4PF
	UnGetStrData( " "TOKEN__FILE0__4PF" 0" );
	CommandDefine( 0 );
	// __FILE0__4PFE
	UnGetStrData( " "TOKEN__FILE0__4PFE" 0" );
	CommandDefine( 0 );
	// __FILE0__4F
	UnGetStrData( " "TOKEN__FILE0__4F" 0" );
	CommandDefine( 0 );
	// __FILE0__4FE
	UnGetStrData( " "TOKEN__FILE0__4FE" 0" );
	CommandDefine( 0 );
	// __FILE0__4E
	UnGetStrData( " "TOKEN__FILE0__4E" 0" );
	CommandDefine( 0 );
	// __FILE0__5D
	UnGetStrData( " "TOKEN__FILE0__5D" 0" );
	CommandDefine( 0 );
	// __FILE0__5DP
	UnGetStrData( " "TOKEN__FILE0__5DP" 0" );
	CommandDefine( 0 );
	// __FILE0__5DPF
	UnGetStrData( " "TOKEN__FILE0__5DPF" 0" );
	CommandDefine( 0 );
	// __FILE0__5DPFE
	UnGetStrData( " "TOKEN__FILE0__5DPFE" 0" );
	CommandDefine( 0 );
	// __FILE0__5P
	UnGetStrData( " "TOKEN__FILE0__5P" 0" );
	CommandDefine( 0 );
	// __FILE0__5PF
	UnGetStrData( " "TOKEN__FILE0__5PF" 0" );
	CommandDefine( 0 );
	// __FILE0__5PFE
	UnGetStrData( " "TOKEN__FILE0__5PFE" 0" );
	CommandDefine( 0 );
	// __FILE0__5F
	UnGetStrData( " "TOKEN__FILE0__5F" 0" );
	CommandDefine( 0 );
	// __FILE0__5FE
	UnGetStrData( " "TOKEN__FILE0__5FE" 0" );
	CommandDefine( 0 );
	// __FILE0__5E
	UnGetStrData( " "TOKEN__FILE0__5E" 0" );
	CommandDefine( 0 );
	// __FILE0__6D
	UnGetStrData( " "TOKEN__FILE0__6D" 0" );
	CommandDefine( 0 );
	// __FILE0__6DP
	UnGetStrData( " "TOKEN__FILE0__6DP" 0" );
	CommandDefine( 0 );
	// __FILE0__6DPF
	UnGetStrData( " "TOKEN__FILE0__6DPF" 0" );
	CommandDefine( 0 );
	// __FILE0__6DPFE
	UnGetStrData( " "TOKEN__FILE0__6DPFE" 0" );
	CommandDefine( 0 );
	// __FILE0__6P
	UnGetStrData( " "TOKEN__FILE0__6P" 0" );
	CommandDefine( 0 );
	// __FILE0__6PF
	UnGetStrData( " "TOKEN__FILE0__6PF" 0" );
	CommandDefine( 0 );
	// __FILE0__6PFE
	UnGetStrData( " "TOKEN__FILE0__6PFE" 0" );
	CommandDefine( 0 );
	// __FILE0__6F
	UnGetStrData( " "TOKEN__FILE0__6F" 0" );
	CommandDefine( 0 );
	// __FILE0__6FE
	UnGetStrData( " "TOKEN__FILE0__6FE" 0" );
	CommandDefine( 0 );
	// __FILE0__6E
	UnGetStrData( " "TOKEN__FILE0__6E" 0" );
	CommandDefine( 0 );
	// __FILE0__7D
	UnGetStrData( " "TOKEN__FILE0__7D" 0" );
	CommandDefine( 0 );
	// __FILE0__7DP
	UnGetStrData( " "TOKEN__FILE0__7DP" 0" );
	CommandDefine( 0 );
	// __FILE0__7DPF
	UnGetStrData( " "TOKEN__FILE0__7DPF" 0" );
	CommandDefine( 0 );
	// __FILE0__7DPFE
	UnGetStrData( " "TOKEN__FILE0__7DPFE" 0" );
	CommandDefine( 0 );
	// __FILE0__7P
	UnGetStrData( " "TOKEN__FILE0__7P" 0" );
	CommandDefine( 0 );
	// __FILE0__7PF
	UnGetStrData( " "TOKEN__FILE0__7PF" 0" );
	CommandDefine( 0 );
	// __FILE0__7PFE
	UnGetStrData( " "TOKEN__FILE0__7PFE" 0" );
	CommandDefine( 0 );
	// __FILE0__7F
	UnGetStrData( " "TOKEN__FILE0__7F" 0" );
	CommandDefine( 0 );
	// __FILE0__7FE
	UnGetStrData( " "TOKEN__FILE0__7FE" 0" );
	CommandDefine( 0 );
	// __FILE0__7E
	nTokenEnd = cRegist;
	UnGetStrData( " "TOKEN__FILE0__7E" 0" );
	CommandDefine( 0 );


	FileData[NowFile].line -=3;

	//
	//	定義済み命令の登録
	//
//	dw7 = 0;
//	dw9 = 0;
//	dw11 = 0;
//	while(( lp0 = GetToken( token )) != NULL ){
	while( 1 ) {
//	dw10 = dw6 = timeGetTime();
		lp0 = GetToken( token );
		if( lp0 == NULL ) {
//			dw9+=timeGetTime()-dw10;
			// #function で出力していない物がある場合、出力する
			lp0 = EndExpFunction();
			if( lp0==NULL ) {
				// 終了
				break;
			} else {
//;;;make				puttoken( lp0 );
				UnGetStrData( lp0 );
			}
		} else {
//	dw12 = timeGetTime();
//	dw9+=dw12-dw10;
			num = GetTokenNum( lp0, &c );
//	dw8 = timeGetTime();
//	dw11+=dw8-dw12;
			switch( num ) {

				//
				//	#calc
				//
				case NUM_PCALC:
					if( if_flag[nest] == 0 ) {
						CalcMode = 1;
						SkipSpace();
						//
						//	calc の結果を入れる定数名 buffer へ一時的に copy しておく
						//
						lp0 = GetToken( token );
						if( ( strlen( lp0 ) + 1 ) > CALCMAXREGIST ) {
							CalcMode = 0;
							err_pre0(1);
							printf( "preprocessor error:#calc の定義名の buffer が一杯で定義できません ( 最大 %d bytes )\n", CALCMAXREGIST );
						} else {
							strcpy( CalcRegist, lp0 );
						}
					}
					break;


				//
				//	#pushnum
				//
				case NUM_PPUSHNUM:
					if( if_flag[nest] == 0 ) {
						CalcMode = 4;
						SkipSpace();
					}
					break;


				//
				//	#popnum
				//
				case NUM_PPOPNUM:
					if( if_flag[nest] == 0 ) {
						SkipSpace();
						lp0 = GetToken( token );
						if( num_sp <= 0 ) {
							err_pre0(1);
							printf( "preprocessor error:#popnum が overflow しました\n" );
						} else {
							RegistDefineNum( token, num_stack[--num_sp] );
						}
					}
					break;


				//
				//	#getnum
				//
				case NUM_PGETNUM:
					if( if_flag[nest] == 0 ) {
						CalcMode = 5;
						SkipSpace();
						//
						//	getnum の結果を入れる定数名 buffer へ一時的に copy しておく
						//
						lp0 = GetToken( token );
						if( ( strlen( lp0 ) + 1 ) > CALCMAXREGIST ) {
							CalcMode = 0;
							err_pre0(1);
							printf( "preprocessor error:#getnum の定義名の buffer が一杯で定義できません ( 最大 %d bytes )\n", CALCMAXREGIST );
						} else {
							strcpy( CalcRegist, lp0 );
						}
					}
					break;


				//
				//	#addnumsp
				//
				case NUM_PADDNUMSP:
					if( if_flag[nest] == 0 ) {
						CalcMode = 6;
						SkipSpace();
					}
					break;


				//
				//	#subnumsp
				//
				case NUM_PSUBNUMSP:
					if( if_flag[nest] == 0 ) {
						CalcMode = 7;
						SkipSpace();
					}
					break;


				//
				//	#getnumsp
				//
				case NUM_PGETNUMSP:
					if( if_flag[nest] == 0 ) {
						SkipSpace();
						lp0 = GetToken( token );
						RegistDefineNum( token, num_sp );
					}
					break;


				//
				//	#searchnum
				//
				case NUM_PSEARCHNUM:
					if( if_flag[nest] == 0 ) {
						CalcMode = 8;
						SkipSpace();
						//
						//	searchnum の結果を入れる定数名 buffer へ一時的に copy しておく
						//
						lp0 = GetToken( token );
						if( ( strlen( lp0 ) + 1 ) > CALCMAXREGIST ) {
							CalcMode = 0;
							err_pre0(1);
							printf( "preprocessor error:#searchnum の定義名の buffer が一杯で定義できません ( 最大 %d bytes )\n", CALCMAXREGIST );
							err_exit();
						} else {
							strcpy( CalcRegist, lp0 );
						}
					}
					break;


				//
				//	#pushstr
				//
				case NUM_PPUSHSTR:
					if( if_flag[nest] == 0 ) {
						SkipSpace();
						lp0 = GetToken( token );
						num = GetTokenNum( lp0, &c );
						lp1 = macrobuffer;
						lp2 = token;
						while( num != NUM_UNDEFINED ) {
							// 定義されている物
							ExpDefine( lp1, lp2, c, &d );
							num = GetTokenNum( lp1, &c );
							lp0 = lp1;
							lp1 = lp2;
							lp2 = lp0;
						}
						c = strlen( lp0 );
						lp1 = str_stack[str_sp];
						if( lp1+c >= &str_stackarea[MAXSTRSTACKAREA] ) {
							err_pre0(1);
							printf( "preprocessor error:#pushstr の buffer が一杯です ( 最大 %d bytes )\n", MAXSTRSTACKAREA );
						} else {
							strcpy( lp1,lp0 );
							str_sp++;
							str_stack[str_sp] = lp1+c+1;
						}
					}
					break;


				//
				//	#popstr
				//
				case NUM_PPOPSTR:
					if( if_flag[nest] == 0 ) {
						SkipSpace();
						lp0 = GetToken( token );
						if( str_sp > 0 ) {
							RegistDefineStr( lp0, str_stack[--str_sp] );
						} else {
							err_pre0(1);
							printf( "preprocessor error:#popstr が overflowしました\n" );
						}
					}
					break;


				//
				//	#getstr
				//
				case NUM_PGETSTR:
					if( if_flag[nest] == 0 ) {
						CalcMode = 9;
						SkipSpace();
						//
						//	getstr の結果を入れる定数名 buffer へ一時的に copy しておく
						//
						lp0 = GetToken( token );
						if( ( strlen( lp0 ) + 1 ) > CALCMAXREGIST ) {
							CalcMode = 0;
							err_pre0(1);
							printf( "preprocessor error:#getstr の定義名の buffer が一杯で定義できません ( 最大 %d bytes )\n", CALCMAXREGIST );
						} else {
							strcpy( CalcRegist, lp0 );
						}
					}
					break;


				//
				//	#addstrsp
				//
				case NUM_PADDSTRSP:
					if( if_flag[nest] == 0 ) {
						CalcMode = 10;
						SkipSpace();
					}
					break;


				//
				//	#substrsp
				//
				case NUM_PSUBSTRSP:
					if( if_flag[nest] == 0 ) {
						CalcMode = 11;
						SkipSpace();
					}
					break;


				//
				//	#getstrsp
				//
				case NUM_PGETSTRSP:
					if( if_flag[nest] == 0 ) {
						SkipSpace();
						lp0 = GetToken( token );
						RegistDefineNum( token, str_sp );
					}
					break;


				//
				//	#expdefine
				//
				case NUM_PEXPDEFINE:
					if( if_flag[nest] == 0 ) {
						SkipSpace();
						lp0 = GetToken( token );
						num = GetTokenNum( lp0, &c );
						lp1 = macrobuffer;
						lp2 = token;
						while( num != NUM_UNDEFINED ) {
							// 定義されている物
							ExpDefine( lp1, lp2, c, &d );
							num = GetTokenNum( lp1, &c );
							lp0 = lp1;
							lp1 = lp2;
							lp2 = lp0;
						}
						UnGetStrData( lp0 );
						UnGetStrData( TOKEN_PDEFINE" " );
					}
					break;


				//
				//	#deletefile
				//
				case NUM_PDELETEFILE:
					fNowIf = 0;
					if( if_flag[nest] == 0 ) {
						SkipSpace();
						lp0 = GetToken( token );
						if( esc2yen( lp0 )!= NULL ) {
							if( remove( lp0 )!= 0 ) {
								err_pre0(0);
								printf( "preprocessor warning:%sが削除できません\n", lp0 );
							}
						} else {
							err_pre0(0);
							printf( "preprocessor warning:#deletefile(%s) が\"でくくられていません\n", lp0 );
						}
					}
					break;


				//
				//	#copyfile
				//
				case NUM_PCOPYFILE:
					fNowIf = 0;
					if( if_flag[nest] == 0 ) {
						SkipSpace();
						lp0 = GetToken( token );
						if( esc2yen( lp0 )!= NULL ) {
							strcpy( macrobuffer,lp0 );

							SkipSpace();
							lp0 = GetToken( token );
							if( esc2yen( lp0 )!= NULL ) {

								if( CopyFile( macrobuffer,lp0,FALSE )== 0 ) {
									err_pre0(0);
									printf( "preprocessor warning:%sが%sへcopyできません\n", macrobuffer,lp0 );
								}
							} else {
								err_pre0(0);
								printf( "preprocessor warning:#copyfile(%s) が\"でくくられていません\n", lp0 );
							}
						} else {
							err_pre0(0);
							printf( "preprocessor warning:#copyfile(%s) が\"でくくられていません\n", lp0 );
						}
					}
					break;


				//
				//	#existfile
				//
				case NUM_PEXISTFILE:
					fNowIf = 0;
					if( if_flag[nest] == 0 ) {
						SkipSpace();
						lp0 = GetToken( token );
						strcpy( macrobuffer,lp0 );
						SkipSpace();
						lp0 = GetToken( token );
						if( esc2yen( lp0 )!= NULL ) {
							hFind0 = FindFirstFile( lp0, &fd0 );
							FindClose( hFind0 );
							if( hFind0 == INVALID_HANDLE_VALUE ) {
								RegistDefineNum( macrobuffer, 0 );
							} else {
								RegistDefineNum( macrobuffer, 1 );
							}
						} else {
							err_pre0(0);
							printf( "preprocessor warning:#existfile(%s) が\"でくくられていません\n", lp0 );
						}
					}
					break;


				//
				//	#exec0
				//
				case NUM_PEXEC0:
					fNowIf = 0;
					if( if_flag[nest] == 0 ) {
						SkipSpace();
						lp0 = GetToken( token );
						if( esc2yen( lp0 )!= NULL ) {
							if( ExecSub0( lp0 )==FALSE ) {
								err_pre0(0);
								printf( "preprocessor warning:#exec0(%s) が起動できませんでした\n", lp0 );
							}
						} else {
							err_pre0(0);
							printf( "preprocessor warning:#exec0(%s) が\"でくくられていません\n", lp0 );
						}
					}
					break;


				//
				//	#exec1
				//
				case NUM_PEXEC1:
					fNowIf = 0;
					if( if_flag[nest] == 0 ) {
						SkipSpace();
						lp0 = GetToken( token );
						if( esc2yen( lp0 )!= NULL ) {
							if( ExecSub1( lp0 )==FALSE ) {
								err_pre0(0);
								printf( "preprocessor warning:#exec1(%s) が起動できませんでした\n", lp0 );
							}
						} else {
							err_pre0(0);
							printf( "preprocessor warning:#exec1(%s) が\"でくくられていません\n", lp0 );
						}
					}
					break;


				//
				//	#openfile
				//
				case NUM_POPENFILE:
					fNowIf = 0;
					if( if_flag[nest] == 0 ) {
						CalcMode = 12;
						SkipSpace();
						//
						//	openfile の結果を入れる定数名 buffer へ一時的に copy しておく
						//
						lp0 = GetToken( token );
						if( esc2yen( lp0 )!= NULL ) {
							if( ( strlen( lp0 ) + 1 ) > CALCMAXREGIST ) {
								CalcMode = 0;
								err_pre0(1);
								printf( "preprocessor error:#openfile の定義名の buffer が一杯で定義できません ( 最大 %d bytes )\n", CALCMAXREGIST );
								err_exit();
							} else {
								strcpy( CalcRegist, lp0 );
							}
						} else {
							err_pre0(0);
							printf( "preprocessor warning:#openfile(%s) が\"でくくられていません\n", lp0 );
						}
					}
					break;


				//
				//	#putfile
				//
				case NUM_PPUTFILE:
					fNowIf = 0;
					if( if_flag[nest] == 0 ) {
						CalcMode = 13;
						SkipSpace();
						//
						//	putfile の結果を入れる定数名 buffer へ一時的に copy しておく
						//
						lp0 = GetToken( token );
						if( esc2yen( lp0 )!= NULL ) {
							if( ( strlen( lp0 ) + 1 ) > CALCMAXREGIST ) {
								CalcMode = 0;
								err_pre0(1);
								printf( "preprocessor error:#putfile の定義名の buffer が一杯で定義できません ( 最大 %d bytes )\n", CALCMAXREGIST );
								err_exit();
							} else {
								strcpy( CalcRegist, lp0 );
							}
						} else {
							err_pre0(0);
							printf( "preprocessor warning:#putfile(%s) が\"でくくられていません\n", lp0 );
						}
					}
					break;


				//
				//	#closefile
				//
				case NUM_PCLOSEFILE:
					if( if_flag[nest] == 0 ) {
						CalcMode = 14;
						SkipSpace();
					}
					break;


				//
				//	#crfile
				//
				case NUM_PCRFILE:
					if( if_flag[nest] == 0 ) {
						CalcMode = 15;
						SkipSpace();
					}
					break;


				//
				//	#define
				//
				case NUM_PDEFINE:
					fNowIf = 0;
					if( if_flag[nest] == 0 ) {
						CommandDefine( 0 );
					}
					break;


				//
				//	#unstr
				//
				case NUM_PUNSTR:
					fNowIf = 0;
					if( if_flag[nest] == 0 ) {
						CommandUnstr();
					}
					break;


				//
				//	#adefine
				//
				case NUM_PADEFINE:
					fNowIf = 0;
					if( if_flag[nest] == 0 ) {
						CommandDefine( 1 );
					}
					break;


				//
				//	#elif
				//
				case NUM_PELIF:
					fNowIf = 1;
					if( if_flag[nest] == 1 && if_use[nest]==0 ) {
						if_flag[nest] = 0;
						CalcMode = 3;
					} else {
						if_flag[nest] = 1;
					}
					break;


				//
				//	#else
				//
				case NUM_PELSE:
					fNowIf = 1;
					if( if_else[nest] != 0 ) {
						err_pre0(1);
						printf( "preprocessor error:#else が 2 つ以上あります\n" );
					} else {
						if( if_flag[nest] == 1 && if_use[nest]==0 ) {
							if_flag[nest] = 0;
						} else {
							if_flag[nest] = 1;
						}
						if_else[nest]++;
					}
					break;


				//
				//	#endif
				//
				case NUM_PENDIF:
					fNowIf = 0;
					nest--;
					if( nest < 0 ) {
						err_pre0(1);
						printf( "preprocessor error:#if/#ifdef/#ifndef/#endif の対応がおかしいです\n" );
					} else {
						if( if_flag[nest] == 0 ) {
							NowLine = -1;
							OldNowFile = -1;
						}
					}
					break;


				//
				//	#if
				//
				case NUM_PIF:
					fNowIf = 1;
					if( if_flag[nest] == 0 ) {
						CalcMode = 2;
					} else {
						set_if( 2 );
						if_use[nest]=1;				// この階層は skip
					}
					break;


				//
				//	#ifdef
				//
				case NUM_PIFDEF:
					fNowIf = 1;
					SkipSpace();
					lp0 = GetToken( token );
					UnGetCharDataOne( ')' );
					UnGetStrData( lp0 );
					UnGetStrData( TOKEN_PIF" "TOKEN_DEFINED"(" );
					break;


				//
				//	#ifndef
				//
				case NUM_PIFNDEF:
					fNowIf = 1;
					SkipSpace();
					lp0 = GetToken( token );
					UnGetStrData( ")" );
					UnGetStrData( lp0 );
					UnGetStrData( TOKEN_PIF" !"TOKEN_DEFINED"(" );
					break;


				//
				//	#error
				//
				case NUM_PERROR:
					if( if_flag[nest] == 0 ) {
						SkipSpace();
						lp0 = GetTokenDefine( token );
						err_pre0(1);
						printf( "preprocessor error:%s\n", lp0 );
					}
					break;


				//
				//	#include
				//
				case NUM_PINCLUDE:
					fNowIf = 0;
					if( if_flag[nest] == 0 ) {
						CommandInclude();
					}
					break;


				//
				//	#line
				//
				case NUM_PLINE:
					fNowIf = 0;
					puttoken( TOKEN_PLINE );
					lp0 = GetTokenDefine( token );
					puttoken( lp0 );
//;;;make
					NowLine--;
					break;


				//
				//	#l2 ( line2 )
				//
				case NUM_PL2:
					fNowIf = 0;
					puttoken( TOKEN_PL2 );
					lp0 = GetTokenDefine( token );
					puttoken( lp0 );
//;;;make
					NowLine--;
					break;


				//
				//	#l2e ( line2 end )
				//
				case NUM_PL2E:
					fNowIf = 0;
					puttoken( TOKEN_PL2E );
					lp0 = GetTokenDefine( token );
					puttoken( lp0 );
//;;;make
					NowLine--;
					break;


				//
				//
				//
				//	#macro
				//
				case NUM_PMACRO:
					fNowIf = 0;
					if( if_flag[nest] == 0 ) {
						CommandMacro( 2 );
						NowLine = -1;
						OldNowFile = -1;
					}
					break;


				//
				//	#amacro
				//
				case NUM_PAMACRO:
					fNowIf = 0;
					if( if_flag[nest] == 0 ) {
						CommandMacro( 3 );
						NowLine = -1;
						OldNowFile = -1;
					}
					break;


				//
				//	#function
				//
				case NUM_PFUNCTION:
					fNowIf = 0;
					if( if_flag[nest] == 0 ) {
						CommandMacro( 4 );
						NowLine = -1;
						OldNowFile = -1;
					}
					break;


				//
				//	#afunction
				//
				case NUM_PAFUNCTION:
					fNowIf = 0;
					if( if_flag[nest] == 0 ) {
						CommandMacro( 5 );
						NowLine = -1;
						OldNowFile = -1;
					}
					break;


				//
				//	#functionstack
				//
				case NUM_PFUNCTIONSTACK:
					fNowIf = 0;
					if( if_flag[nest] == 0 ) {
						SkipSpace();
						GetTokenDefine( FuncStack );
						NowLine = -1;
						OldNowFile = -1;
					}
					break;


				//
				//	#functioncall
				//
				case NUM_PFUNCTIONCALL:
					fNowIf = 0;
					if( if_flag[nest] == 0 ) {
						SkipSpace();
						GetTokenDefine( FuncCall );
						NowLine = -1;
						OldNowFile = -1;
					}
					break;


				//
				//	#afunctioncall
				//
				case NUM_PAFUNCTIONCALL:
					fNowIf = 0;
					if( if_flag[nest] == 0 ) {
						SkipSpace();
						GetTokenDefine( FuncCalla );
						NowLine = -1;
						OldNowFile = -1;
					}
					break;


				//
				//	#mbc
				//
				case NUM_PMBC:
					fNowIf = 0;
					if( if_flag[nest] == 0 ) {
						SkipSpace();
						GetTokenDefine( mbcBuff );
						NowLine = -1;
						OldNowFile = -1;
					}
					break;


				//
				//	#mbcoff
				//
				case NUM_PMBCOFF:
					fNowIf = 0;
					if( if_flag[nest] == 0 ) {
						mbcBuff[0]=0;
						NowLine = -1;
						OldNowFile = -1;
					}
					break;


				//
				//	#end
				//
				case NUM_PEND:
					err_pre0(1);
					printf( "preprocessor error:#endを使っています。#endifにしてください\n", lp0 );
					break;


				//
				//	#mes
				//
				case NUM_PMES:
					fNowIf = 0;
					if( if_flag[nest] == 0 ) {
						SkipSpace();
						lp0 = GetTokenDefine( token );
						printf( "%s\n", lp0 );
					}
					break;


				//
				//	#undef
				//
				case NUM_PUNDEF:
					fNowIf = 1;
					if( if_flag[nest] == 0 ) {
						SkipSpace();
						lp0 = GetToken( token );
						if( GetTokenNum( token, &c ) != NUM_UNDEFINED ) {
							UndefTokenNum( c );
						}
					}
					break;


				//
				//	#mle ( macro line end )
				//
				case NUM_PMLE:
					fNowIf = 0;
					nMacroNest--;
					if( nMacroNest <0 ) {
						err_pre0(1);
						printf( "preprocessor error:macro が異常か #mle が不用意に入っています\n", MAXMACRONEST );
						nMacroNest=0;
					}
					// 必要か？ただspace/tabの空読みしているだけではないか？
					//c = GetCharData();
					if( _iskanji(c = GetCharDataOne()) ) {
						c += (GetCharDataOne()<<8);
					}
					NowLine = -1;
					OldNowFile = -1;
					puttoken( "#l2e\n" );
					PrintLine();
					break;


				//
				//	#let
				//
				case NUM_PLET:
					fNowIf = 0;
					nMacroNest--;
					if( nMacroNest <0 ) {
						err_pre0(1);
						printf( "preprocessor error:macro が異常か #let が不用意に入っています\n", MAXMACRONEST );
						nMacroNest=0;
					}
					// 必要か？ただspace/tabの空読みしているだけではないか？
					//c = GetCharData();
					if( _iskanji(c = GetCharDataOne()) ) {
						c += (GetCharDataOne()<<8);
					}
					NowLine = -1;
					OldNowFile = -1;
					puttoken( "#l2e\n" );
					PrintLine();
					break;


				//
				//	blank
				//
				case NUM_BLANK:
					if( if_flag[nest] == 0 ) {
						SkipSpace();
						if(( c = GetCharDataOne()) == '(' ) {
							d = 0;
						} else {
							UnGetCharDataOne( c );
							d = 1;
						}
						SkipSpace();
//						if( _isname0(c = GetCharDataOne()) == 0 ) {
//							err_pre0(1);
//							printf( "preprocessor error:define で評価する識別子が異常です\n" );
//							err_exit();
//						}
//						UnGetCharDataOne( c );
						lp0 = GetToken( token );
						if( d == 0 ) {
							SkipSpace();
							if(( c = GetCharDataOne()) != ')' ) {
								err_pre0(1);
								printf( "preprocessor error:blank 演算子のかっこが閉じていないか、単純な #define 以外の macro が指定されています\n" );
								break;
							}
						}
						if(( num = GetTokenNum( lp0, &c )) == NUM_UNDEFINED ) {
							while( 1 ) {
								c = *lp0++;
								if( c == 0 ) {
									puttoken( "1" );
									break;
								} else if( c == '"' && *lp0 == '"' && *(lp0+1) == 0 ) {
									puttoken( "1" );
									break;
								} else if( c != '\n' && c != ' ' && c != '\t' ) {
									puttoken( "0" );
									break;
								}
							}
						} else if( num >= NUM_TOP && num <= NUM_END ) {
							err_pre0(1);
							printf( "preprocessor error:予約語を blank 演算子で評価しようとしています(%s)\n",lp0 );
							break;
						} else {
							if( *(((int*)lpDefine[c])) <= 1 ) {
								// パラメーター有りの #define/#adefine は、0
								if( *(((int*)lpDefine[c])+1) != 0 ||
								    *(((int*)lpDefine[c])+2) != 0 ) {
									puttoken( "0" );
								// パラメーター無しの #define/#adefine は、再度 blank チェックが必要
								} else {
									ExpDefine( macrobuffer, token, c, &d );
									lp0 = macrobuffer;
									while( 1 ) {
										c = *lp0++;
										if( c == 0 ) {
											puttoken( "1" );
											break;
										} else if( c != '\n' && c != ' ' && c != '\t' ) {
											if( d == 0 ) {
												UnGetCharDataOne( ')' );
											} else {
												UnGetCharDataOne( ' ' );
											}
											UnGetStrData( macrobuffer );
											if( d == 0 ) {
												UnGetCharDataOne( '(' );
											}
											UnGetStrData( TOKEN_BLANK " " );
											break;
										}
									}
								}
							// #macro/#amacro は、0
							} else if( *(((int*)lpDefine[c])) <= 1 ) {
								puttoken( "0" );
							} else {
								err_pre0(1);
								printf( "preprocessor error:blank で評価する識別子が異常です\n" );
								break;
							}
						}
					}
					break;


				//
				//	defined
				//
				case NUM_DEFINED:
					if( if_flag[nest] == 0 ) {
						SkipSpace();
						if(( c = GetCharDataOne()) == '(' ) {
							d = 0;
						} else {
							UnGetCharDataOne( c );
							d = 1;
						}
						SkipSpace();
						if( !_isname0(c = GetCharDataOne())) {
							err_pre0(1);
							printf( "preprocessor error:defined で評価する識別子が異常です\n" );
							break;
						}
						UnGetCharDataOne( c );
						lp0 = GetToken( token );
						if( GetTokenNum( lp0, &c ) == NUM_UNDEFINED ) {
							puttoken( "0" );
						} else {
							puttoken( "1" );
						}
						if( d == 0 ) {
							SkipSpace();
							if(( c = GetCharDataOne()) != ')' ) {
								err_pre0(1);
								printf( "preprocessor error:defined のかっこが閉じていません\n" );
								break;
							}
						}
					}
					break;


				//
				//	定義されていないもの
				//
				case NUM_UNDEFINED:
					if( if_flag[nest] == 0 ) {
						if( *lp0=='#' ) {
							err_pre0(1);
							printf( "preprocessor error:無効な # があります ( %s )\n",lp0 );
							break;
						}
						puttoken( lp0 );
						d = 0;
						if( nMacroNest > FileData[NowFile].nMacroNest ){
							if( NowLine != MacroLine[nMacroNest] || ( MacroLine[nMacroNest] % MAXLINE ) == 0 ) {
								d = 1;
							}
						} else {
							if( NowLine != FileData[NowFile].line || ( FileData[NowFile].line % MAXLINE ) == 0 ) {
								d = 1;
							}
						}
						if( d == 1 ) {
							// token の最後が lf か
							c = strlen( lp0 );
							if( c != 0 && *(lp0+c-1)=='\n' ) {
								// #line を出力
								PrintLine();
							}
						}
#if 0
					} else {
						if( *lp0=='#' ) {
							if( strcmp( lp0,TOKEN_PENDM ) == 0 ) {
								// #endm の場合、何も無し
							} else if( strcmp( lp0,TOKEN_PENDF ) == 0 ) {
								// #endf の場合、何も無し
							} else {
								err_pre0(0);
								printf( "preprocessor warning: #if で無効化されている部分で無効な # があります ( %s )\n", lp0 );
								printf( "warning は指定されている行より後にありますので注意して下さい\n" );
							}
						}
#endif
					}
					break;




				//
				//	#define/#adefine/#macro/#amacro/#function/#afunction で定義されているもの
				//
				default:
					if( if_flag[nest] == 0 ) {
						// #define
						if( *(((int*)lpDefine[c])) <= 1 ) {
							ExpDefine( macrobuffer, token, c, &d );
							UnGetStrData( macrobuffer );
						// #macro
						} else if( *(((int*)lpDefine[c])) <= 3 ) {
							lp0 = ( ExpMacro( c, &d ) );
							if( lp0!=NULL ) {
								if( d == 2 ) {
									puttoken( lp0 );
								} else {
									UnGetStrData( lp0 );
								}
							}
						// #function
						} else {
							lp0 = ( ExpFunction( c, &d ) );
							if( lp0!=NULL ) {
								if( d == 2 ) {
									puttoken( lp0 );
								} else {
									UnGetStrData( lp0 );
								}
							}
						}
					}
					break;
			}
		}
//	dw2=timeGetTime();
//	dw7+=dw2-dw8;
//	dw5+=dw2-dw6;
	}
}
/*---------------------------------------------------------------------------*/
int mainprog(int argc, char **argv)
{
	UCHAR *lp;
	int c,d,f,n,fBtprs;
	time_t aclock;
	int sj;					// ワークの初期化

//	dw0 = timeGetTime();


	//
	//	init
	//
	pFile=NULL;

	nError=0;
	for(c=0;c<MAXINCLUDEFILE;c++) {
		FileData[c].pTop=NULL;
		pFileData[c] = &FileData[c];
	}
	lpDefine = malloc( (MAX_REGISTNUM)*4 );
	fUseFunc = malloc( MAX_REGISTNUM );
	for( c=0;c<MAX_REGISTNUM;c++ ) fUseFunc[c]=0;
	lpRegist = malloc( (MAX_REGISTNUM)*4 );
	maxRegistbuff = MAX_REGISTBUFF;
	lpRegistEnd = RegistBuffer = malloc( maxRegistbuff );
	lpDefineEnd = DefineBuffer = malloc( MAX_DEFINEBUFF );
	lpFileEnd = MacroDefineFile = malloc( MAX_MACROFILE );
	lpPre = PreBuff = malloc( PREBUFFERSIZE );
	lpUnGetBuffer = malloc( UNGETBUFFER+4 );
	lpUnGet = lpUnGetBuffer + UNGETBUFFER;
	InitIs();
	InitHash();
	num_sp = 0;
	str_sp = 0;
	str_stack[0] = str_stackarea;
	for( c=0;c<MAXPUTFILE;c++ ) {
		putfile[c]=NULL;
	}
	FuncStack[0]=0;				// stack の文字
	FuncCall[0]=0;				// call するときの命令
	mbcBuff[0]=0;				// #mbc buffer

	//
	//	error の時
	//
	sj=setjmp( main_sj );
	if( sj != 0 ) {
		// error 終了の時の処理
		return sj;
	}


	//
	//	open file
	//
	if (argc < 2) {
#if W95CC_MODE
		printf( "w95cc ver1.0 : soc compiler for w95ga copyright (C)KENJI 1998.12-\n" );
		printf( "usage:w95cc [-options] file1 [file2]\n" );
#else
		printf( "acc ver1.0 : サンプルコンパイラ copyright (C)KENJI 1998.12-\n" );
		printf( "usage:acc [-options] file1 [file2]\n" );
#endif
		printf( "[option(preprocessor)]\n" );
		printf( "     -pfile  preprocessor file を出力する\n" );
		printf( "     -ifile  include path を追加する\n" );
		printf( "[option(compiler)]\n" );
		printf( "     -n      compiler を実行しない ( [file2] を指定しない場合にも preprocessor\n" );
		printf( "             のみで compiler を実行しません )\n" );
#if W95CC_MODE
		printf( "     -b      prs を実行しない\n" );
#endif
		printf( "     -w2     Warning level 2\n" );
		printf( "makeモードの説明は >w95cc.exe /f /?[ret]\n");
		err_exit();
	}


	//
	//	include path の初期化 & 追加
	//
	for( c=0; c<MAXINCLUDEPATH; c++ ) {
		IncludePath[c][0]=0;
	}


	InputFlag = 0;		// input
	OutputFlag = 0;		// output
	PreFlag = 0;		// preprocessor file
#if W95CC_MODE
	fBtprs = 1;			// btprs を実行する
#else
	fBtprs = 0;			// btprs を実行する
#endif
	n = 1;				// compiler を使う
	nWarning = 1;
	d = 0;
	c = 1;
	while( argc != c ) {
		if( argv[c][0] == '/' || argv[c][0] == '-' ) {
			f = 1;
			while( argv[c][f] != 0 ) {
				if( argv[c][f] == 'p' || argv[c][f] == 'P' ) {
					f++;
					if ( argv[c][f] == 0 ) {
						printf( "err03:i オプションの filename が異常です" );
						err_exit();
					}
					strcpy( filenamep, &argv[c][f] );
					PreFlag = 1;
					break;
				} else if( argv[c][f] == 'i' || argv[c][f] == 'I' ) {
					f++;
					if ( argv[c][f] == 0 ) {
						printf( "err03:i オプションの path が異常です" );
						err_exit();
					}
					strcpy( IncludePath[nIncludePath], &argv[c][f] );
					addendyen( IncludePath[nIncludePath] );
					nIncludePath++;
					break;
				} else if( argv[c][f] == 'n' || argv[c][f] == 'N' ) {
					n = 0;
#if W95CC_MODE
				} else if( argv[c][f] == 'b' || argv[c][f] == 'B' ) {
					fBtprs = 0;
#endif
				} else if( argv[c][f] == 'w' || argv[c][f] == 'W' ) {
					f++;
					if( argv[c][f] >= '1' && argv[c][f] <= '2' ) {
						nWarning = argv[c][f];
					} else {
						printf( "err03:-w オプションが異常です" );
						err_exit();
					}
				} else {
					printf( "err03:オプションが異常です" );
					err_exit();
				}
				f++;
			}
		} else if( d == 0 ) {
			strcpy( filenamei, argv[c] );
			InputFlag = 1;
			d++;
		} else {
			strcpy( filenameo, argv[c] );
			OutputFlag = 1;
			d++;
		}
		c++;
	}
	if( InputFlag == 0 ) {
		printf( "err01:入力ファイルが指定されていません" );
		err_exit();
	}


	//
	//	削除
	//
	remove( filenameo );


	//
	//	環境変数から、include path を拾得
	//
	if( (lp = getenv( "include" )) != NULL ) {
		c = 0;
		while( *lp != 0 ) {
			if( *lp == ';' || ( *lp == ',' )) {
				IncludePath[nIncludePath][c++] = 0;
				addendyen( IncludePath[nIncludePath] );
				nIncludePath++;
				lp++;
//			} else if( _ismbblead( *lp )) {
			} else if( _iskanji(*lp)) {
				IncludePath[nIncludePath][c++] = *lp++;
				IncludePath[nIncludePath][c++] = *lp++;
			} else {
				IncludePath[nIncludePath][c++] = *lp++;
			}
		}
	}


	//
	//	現在の時間を拾得
	//
	time( & aclock );
	nowtime = localtime( &aclock );


	//
	//	preprocessor file を開く
	//
	if( PreFlag == 0 ) {
		c = 0;
		if( (lp = getenv( "temp" )) != NULL ) {
			strcpy( filenamep, lp );
			addendyen( filenamep );
			c = strlen( filenamep );
		}
		strcpy( filenamep+c,"W9XXXXXX" );
		if( _mktemp( filenamep+c ) == NULL ) {
			printf( "err02:preprocessor 用出力ファイルが開きません(%s)",filenamep );
			err_exit();
		}
		if(( pFile = fopen(filenamep, "w")) == NULL) {
			if( c == 0 ) {
				printf( "err02:preprocessor 用出力ファイルが開きません(%s)",filenamep );
				err_exit();
			} else {
				// 再度 file を作る ( 今度は temp をみない )
				strcpy( filenamep,"W9XXXXXX" );
				if( _mktemp( filenamep ) == NULL ) {
					printf( "err02:preprocessor 用出力ファイルが開きません(%s)",filenamep );
					err_exit();
				}
				if(( pFile = fopen(filenamep, "w")) == NULL) {
					printf( "err02:preprocessor 用出力ファイルが開きません(%s)",filenamep );
					err_exit();
				}
			}
		}
	} else {
		if(( pFile = fopen(filenamep, "w")) == NULL) {
			printf( "err02:preprocessor 用出力ファイルが開きません(%s)",filenamep );
			err_exit();
		}
	}
	fOpenPre = 1;


	//
	//	source file を開く
	//
	NowFile = 0;
	strcpy( FileData[NowFile].filename, filenamei );
	if( open_file( (NowFile) ) == FALSE ) {
		printf( "err01:入力ファイルが開きません(%s)",FileData[NowFile].filename );
		err_exit();
	}
	PrintLine();




	//
	//	setup calc
	//
	CalcPri[CALCODE_LS] = CALPRI_LS;
	CalcLeft[CALCODE_LS] = CALLEFT_LS;
	CalcPri[CALCODE_PLUS] = CALPRI_PLUS;
	CalcLeft[CALCODE_PLUS] = CALLEFT_PLUS;
	CalcPri[CALCODE_MINUS] = CALPRI_MINUS;
	CalcLeft[CALCODE_MINUS] = CALLEFT_MINUS;
	CalcPri[CALCODE_NOT] = CALPRI_NOT;
	CalcLeft[CALCODE_NOT] = CALLEFT_NOT;
	CalcPri[CALCODE_NEG] = CALPRI_NEG;
	CalcLeft[CALCODE_NEG] = CALLEFT_NEG;
	CalcPri[CALCODE_MUL] = CALPRI_MUL;
	CalcLeft[CALCODE_MUL] = CALLEFT_MUL;
	CalcPri[CALCODE_DIV] = CALPRI_DIV;
	CalcLeft[CALCODE_DIV] = CALLEFT_DIV;
	CalcPri[CALCODE_MOD] = CALPRI_MOD;
	CalcLeft[CALCODE_MOD] = CALLEFT_MOD;
	CalcPri[CALCODE_ADD] = CALPRI_ADD;
	CalcLeft[CALCODE_ADD] = CALLEFT_ADD;
	CalcPri[CALCODE_SUB] = CALPRI_SUB;
	CalcLeft[CALCODE_SUB] = CALLEFT_SUB;
	CalcPri[CALCODE_SHL] = CALPRI_SHL;
	CalcLeft[CALCODE_SHL] = CALLEFT_SHL;
	CalcPri[CALCODE_SHR] = CALPRI_SHR;
	CalcLeft[CALCODE_SHR] = CALLEFT_SHR;
	CalcPri[CALCODE_MORE] = CALPRI_MORE;
	CalcLeft[CALCODE_MORE] = CALLEFT_MORE;
	CalcPri[CALCODE_BIG] = CALPRI_BIG;
	CalcLeft[CALCODE_BIG] = CALLEFT_BIG;
	CalcPri[CALCODE_LESS] = CALPRI_LESS;
	CalcLeft[CALCODE_LESS] = CALLEFT_LESS;
	CalcPri[CALCODE_SMALL] = CALPRI_SMALL;
	CalcLeft[CALCODE_SMALL] = CALLEFT_SMALL;
	CalcPri[CALCODE_EQU] = CALPRI_EQU;
	CalcLeft[CALCODE_EQU] = CALLEFT_EQU;
	CalcPri[CALCODE_NEQU] = CALPRI_NEQU;
	CalcLeft[CALCODE_NEQU] = CALLEFT_NEQU;
	CalcPri[CALCODE_BITAND] = CALPRI_BITAND;
	CalcLeft[CALCODE_BITAND] = CALLEFT_BITAND;
	CalcPri[CALCODE_BITXOR] = CALPRI_BITXOR;
	CalcLeft[CALCODE_BITXOR] = CALLEFT_BITXOR;
	CalcPri[CALCODE_BITOR] = CALPRI_BITOR;
	CalcLeft[CALCODE_BITOR] = CALLEFT_BITOR;
	CalcPri[CALCODE_LE] = CALPRI_LE;
	CalcLeft[CALCODE_LE] = CALLEFT_LE;


	//
	//	if の nest/flag を設定
	//
	nest = 0;
	if_flag[nest] = 0;
	if_else[nest] = 0;
	fNowIf = 0;


	//
	//	定義済み命令の登録
	//
	RegistToken( "",0 );								// 番兵
//	RegistToken( TOKEN_AUTO,NUM_AUTO );
//	RegistToken( TOKEN_DOUBLE,NUM_DOUBLE );
//	RegistToken( TOKEN_INT,NUM_INT );
//	RegistToken( TOKEN_STRUCT,NUM_STRUCT );
//	RegistToken( TOKEN_BREAK,NUM_BREAK );
//	RegistToken( TOKEN_ELSE,NUM_ELSE );
//	RegistToken( TOKEN_LONG,NUM_LONG );
//	RegistToken( TOKEN_SWITCH,NUM_SWITCH );
//	RegistToken( TOKEN_CASE,NUM_CASE );
//	RegistToken( TOKEN_ENUM,NUM_ENUM );
//	RegistToken( TOKEN_REGISTER,NUM_REGISTER );
//	RegistToken( TOKEN_TYPEDEF,NUM_TYPEDEF );
//	RegistToken( TOKEN_CHAR,NUM_CHAR );
//	RegistToken( TOKEN_EXTERN,NUM_EXTERN );
//	RegistToken( TOKEN_RETURN,NUM_RETURN );
//	RegistToken( TOKEN_UNION,NUM_UNION );
//	RegistToken( TOKEN_CONST,NUM_CONST );
//	RegistToken( TOKEN_FLOAT,NUM_FLOAT );
//	RegistToken( TOKEN_SHORT,NUM_SHORT );
//	RegistToken( TOKEN_UNSIGNED,NUM_UNSIGNED );
//	RegistToken( TOKEN_CONTINUE,NUM_CONTINUE );
//	RegistToken( TOKEN_FOR,NUM_FOR );
//	RegistToken( TOKEN_SIGNED,NUM_SIGNED );
//	RegistToken( TOKEN_VOID,NUM_VOID );
//	RegistToken( TOKEN_DEFAULT,NUM_DEFAULT );
//	RegistToken( TOKEN_GOTO,NUM_GOTO );
//	RegistToken( TOKEN_SIZEOF,NUM_SIZEOF );
//	RegistToken( TOKEN_VOLATILE,NUM_VOLATILE );
//	RegistToken( TOKEN_DO,NUM_DO );
//	RegistToken( TOKEN_IF,NUM_IF );
//	RegistToken( TOKEN_STATIC,NUM_STATIC );
//	RegistToken( TOKEN_WHILE,NUM_WHILE );
	RegistToken( TOKEN_PCALC,NUM_PCALC );
	RegistToken( TOKEN_PDEFINE,NUM_PDEFINE );
	RegistToken( TOKEN_PADEFINE,NUM_PADEFINE );
	RegistToken( TOKEN_PELIF,NUM_PELIF );
	RegistToken( TOKEN_PELSEIF,NUM_PELSEIF );
	RegistToken( TOKEN_PELSE,NUM_PELSE );
	RegistToken( TOKEN_PENDIF,NUM_PENDIF );
	RegistToken( TOKEN_PIF,NUM_PIF );
	RegistToken( TOKEN_PIFDEF,NUM_PIFDEF );
	RegistToken( TOKEN_PIFNDEF,NUM_PIFNDEF );
	RegistToken( TOKEN_PERROR,NUM_PERROR );
	RegistToken( TOKEN_PINCLUDE,NUM_PINCLUDE );
	RegistToken( TOKEN_PLINE,NUM_PLINE );
	RegistToken( TOKEN_PL2,NUM_PL2 );
	RegistToken( TOKEN_PL2E,NUM_PL2E );
	RegistToken( TOKEN_PMACRO,NUM_PMACRO );
	RegistToken( TOKEN_PAMACRO,NUM_PAMACRO );
	RegistToken( TOKEN_PMES,NUM_PMES );
	RegistToken( TOKEN_PUNDEF,NUM_PUNDEF );
	RegistToken( TOKEN_PMLE,NUM_PMLE );
	RegistToken( TOKEN_PLET,NUM_PLET );
	RegistToken( TOKEN_PUNSTR,NUM_PUNSTR );
	RegistToken( TOKEN_PPUSHNUM,NUM_PPUSHNUM );
	RegistToken( TOKEN_PPOPNUM,NUM_PPOPNUM );
	RegistToken( TOKEN_PGETNUM,NUM_PGETNUM );
	RegistToken( TOKEN_PADDNUMSP,NUM_PADDNUMSP );
	RegistToken( TOKEN_PSUBNUMSP,NUM_PSUBNUMSP );
	RegistToken( TOKEN_PGETNUMSP,NUM_PGETNUMSP );
	RegistToken( TOKEN_PSEARCHNUM,NUM_PSEARCHNUM );
	RegistToken( TOKEN_PPUSHSTR,NUM_PPUSHSTR );
	RegistToken( TOKEN_PPOPSTR,NUM_PPOPSTR );
	RegistToken( TOKEN_PGETSTR,NUM_PGETSTR );
	RegistToken( TOKEN_PADDSTRSP,NUM_PADDSTRSP );
	RegistToken( TOKEN_PSUBSTRSP,NUM_PSUBSTRSP );
	RegistToken( TOKEN_PGETSTRSP,NUM_PGETSTRSP );
	RegistToken( TOKEN_PEXPDEFINE,NUM_PEXPDEFINE );
	RegistToken( TOKEN_PDELETEFILE,NUM_PDELETEFILE );
	RegistToken( TOKEN_PCOPYFILE,NUM_PCOPYFILE );
	RegistToken( TOKEN_PEXISTFILE,NUM_PEXISTFILE );
	RegistToken( TOKEN_PEXEC0,NUM_PEXEC0 );
	RegistToken( TOKEN_PEXEC1,NUM_PEXEC1 );
	RegistToken( TOKEN_POPENFILE,NUM_POPENFILE );
	RegistToken( TOKEN_PPUTFILE,NUM_PPUTFILE );
	RegistToken( TOKEN_PCLOSEFILE,NUM_PCLOSEFILE );
	RegistToken( TOKEN_PCRFILE,NUM_PCRFILE );
	RegistToken( TOKEN_PFUNCTION,NUM_PFUNCTION );
	RegistToken( TOKEN_PAFUNCTION,NUM_PAFUNCTION );
	RegistToken( TOKEN_PENDF,NUM_PENDF );
	RegistToken( TOKEN_PFUNCTIONSTACK,NUM_PFUNCTIONSTACK );
	RegistToken( TOKEN_PFUNCTIONCALL,NUM_PFUNCTIONCALL );
	RegistToken( TOKEN_PAFUNCTIONCALL,NUM_PAFUNCTIONCALL );
	RegistToken( TOKEN_PMBC,NUM_PMBC );
	RegistToken( TOKEN_PEND,NUM_PEND );
	RegistToken( TOKEN_PMBCOFF,NUM_PMBCOFF );
	RegistToken( TOKEN_BLANK,NUM_BLANK );
	RegistToken( TOKEN_DEFINED,NUM_DEFINED );


	//
	//	preprocess
	//
	tokenmain();


	//
	//	close
	//
	if( nest != 0 ){
		err_pre0(1);
		printf( "preprocessor error:#if/#ifdef/#ifndef/#elif/#else/#endif の nest が閉じていません\n" );
		err_exit();
	}
	fwrite(PreBuff, lpPre-PreBuff, 1, pFile);
	fclose( pFile );
	pFile=NULL;
	// HashTable の性能を探る
//	PrintHash0();
	FreeMemW95cc();


	//
	//	compiler
	//
	if((n == 1) && (OutputFlag == 1 )&& (nError==0)) {
		compiler_main( filenamep, filenameo, fBtprs );
	}


	DeleteP();

#if 0
	dw0 = timeGetTime() - dw0;

	fprintf( stderr,"time dw0:%d\n",dw0 );
	fprintf( stderr,"time dw1:%d\n",dw1 );
	fprintf( stderr,"time dw3:%d\n",dw3 );
	fprintf( stderr,"all      time dw5 :%d\n",dw5 );
	fprintf( stderr,"else     time dw7 :%d\n",dw7 );
	fprintf( stderr,"gettoken time dw9 :%d\n",dw9 );
	fprintf( stderr,"tokennum time dw11:%d\n",dw11 );
#endif
//	scanf("%c",&c);


	if( nError!=0 ) {
		err_exit();
	}
	fprintf( stderr,"success\n" );
	return EXIT_SUCCESS;
}
//----------------------------------------------------------------------------
int makecall(int argv, char **argc )
//
//	make プログラムから呼ばれる
//
{
//	int c;
	//for(c=0;c<argv;c++)printf("[%d]%s\n",c,argc[c]);
	return mainprog( argv, argc );
}
//----------------------------------------------------------------------------
int main(int argc, char **argv)
{
	int r;

	// オプションの先頭に -f があるか ?
	if( makechk( argc,argv ) ){
		// make へプログラムを登録
		makeregist( argv[0] );
		r=smake( argc,argv,"",1,1 );
		if(r!=0) printmakeerror( argv[0],r );
		return r;
	} else {
		return mainprog( argc,argv );
	}
}
