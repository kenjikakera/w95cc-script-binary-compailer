//_ismbblead	�}���`�o�C�g�����̑� 1 �o�C�g�B���Ƃ��΁A�R�[�h �y�[�W 932 �̏ꍇ�A�L���͔͈͂� 0x81  -  0x9F ��  0xE0  -  0xFC �ł��B
//_ismbbtrail	�}���`�o�C�g�����̑� 2 �o�C�g�B���Ƃ��΁A�R�[�h �y�[�W 932 �̏ꍇ�A�L���Ȕ͈͂� 0x40 - 0x7E ��  0x80 - 0xEC �ł��B
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
#include "w95cc.h"
#include "w95cc0.h"


/*===========================================================================
=============================================================================
		define
=============================================================================
===========================================================================*/
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

#define	CALCODE_ADDRESS	6
#define	CALPRI_ADDRESS	12
#define	CALLEFT_ADDRESS	1

#define	CALCODE_VALUE	7
#define	CALPRI_VALUE	12
#define	CALLEFT_VALUE	1

#define	CALCODE_MUL		8
#define	CALPRI_MUL		11
#define	CALLEFT_MUL		0

#define	CALCODE_DIV		9
#define	CALPRI_DIV		11
#define	CALLEFT_DIV		0

#define	CALCODE_MOD		10
#define	CALPRI_MOD		11
#define	CALLEFT_MOD		0

#define	CALCODE_ADD		11
#define	CALPRI_ADD		10
#define	CALLEFT_ADD		0

#define	CALCODE_SUB		12
#define	CALPRI_SUB		10
#define	CALLEFT_SUB		0

#define	CALCODE_SHL		13
#define	CALPRI_SHL		9
#define	CALLEFT_SHL		0

#define	CALCODE_SHR		14
#define	CALPRI_SHR		9
#define	CALLEFT_SHR		0

#define	CALCODE_MORE	15
#define	CALPRI_MORE		8
#define	CALLEFT_MORE	0

#define	CALCODE_BIG		16
#define	CALPRI_BIG		8
#define	CALLEFT_BIG		0

#define	CALCODE_LESS	17
#define	CALPRI_LESS		8
#define	CALLEFT_LESS	0

#define	CALCODE_SMALL	18
#define	CALPRI_SMALL	8
#define	CALLEFT_SMALL	0

#define	CALCODE_EQU		19
#define	CALPRI_EQU		7
#define	CALLEFT_EQU		0

#define	CALCODE_NEQU	20
#define	CALPRI_NEQU		7
#define	CALLEFT_NEQU	0

#define	CALCODE_BITAND	21
#define	CALPRI_BITAND	6
#define	CALLEFT_BITAND	0

#define	CALCODE_BITXOR	22
#define	CALPRI_BITXOR	5
#define	CALLEFT_BITXOR	0

#define	CALCODE_BITOR	23
#define	CALPRI_BITOR	4
#define	CALLEFT_BITOR	0

#define	CALCODE_AND		24
#define	CALPRI_AND		3
#define	CALLEFT_AND		0

#define	CALCODE_OR		25
#define	CALPRI_OR		2
#define	CALLEFT_OR		0

#define	CALCODE_LE		26
#define	CALPRI_LE		0
#define	CALLEFT_LE		0

#define	CALCODE_LET		27
#define	CALPRI_LET		1
#define	CALLEFT_LET		0

#define	CALCODE_EXIT	28
#define	CALCODE_MAX		29

#define CALVALUE_MAX	16*1024
#define CALOPE_MAX		16*1024


//
//	btprs
//
#define N		4096					/* ��o�b�t�@�̑傫�� */
#define F		18						/* �Œ���v�� */
#define NIL		N						/* �؂̖��[ */
//
//	hash
//
#define HASH_BEFORE		0
#define HASH_NEXT		4
#define HASH_POINTER	8
#define HASH_TYPE		12
#define HASH_STR		13



/*===========================================================================
=============================================================================
		prototype
=============================================================================
===========================================================================*/
extern int Calc( UCHAR*,int );


/*===========================================================================
=============================================================================
		work
=============================================================================
===========================================================================*/
int nErrComp;
UCHAR *lpOutErr;						// Output file ��������� ( error �̎��ɍ폜�Ɏg�� )
FILE *oFile;							// �o�̓t�@�C��
UCHAR *LabelBuffer0,*lpLB0;				// label data ������o�b�t�@�[
UCHAR *LabelBuffer1,*lpLB1;				// �O���Q�Ƃ��������邽�߂̃o�b�t�@�[
UCHAR *LabelBuffer2,*lpLB2;				// ep0,ep1,ep2 �̎��̑O���Q�Ƃ��������邽�߂̃o�b�t�@�[
UCHAR LineBuffer[MAX_LINEBUFF];			// 1 �s���̃o�b�t�@�[

UCHAR *CalcMacBuff,*lpCc;				// cce �̓W�J�p�o�b�t�@�[
UCHAR *lpCe;							// cce �̏o�͂̂��߂̃|�C���^�[
UCHAR CalcMacMain[MAX_LINEBUFF];		// cce �̎��� $s �̓W�J�p
int nCalcMac;							// cce �W�J�̎��̔ԍ�

UCHAR sourcefilename[FILE_NAME_MAX];	// source filename
int nLineSource;						// source �ł� line ��

UCHAR orgfilename[FILE_NAME_MAX];		// macro ���̓W�J�� source filename
int nLineOrg,fLineOrg;					// macro ���̓W�J�� source �ł� line ��

FILE *pFile;							// preprocessor  �t�@�C��
int psize;
UCHAR *lpP,*lpPs;

//
//	output
//
UCHAR *OutputBuff,*lpOp;				// �o��


//
//	calc
//
int ExpCalcLeft[CALCODE_MAX];
int ExpCalcPri[CALCODE_MAX];
//int ExpCalcValue[CALVALUE_MAX];
UCHAR ExpCalcOpe[CALOPE_MAX];
int ExpCalcSp0,ExpCalcSp1;
UCHAR ExpCalcData[CALOPE_MAX*5],*lpCalcData;
UCHAR *lpExpCalcErr;

//
//	calc check
//
int CalcValue[CALVALUE_MAX],CalcType[CALVALUE_MAX],*lpCalcSp0,*lpCalcSp1;


//
//	btprs
//
unsigned long outcount = 0;				// �o�̓o�C�g���J�E���^
unsigned char text[N+F-1];				// �e�L�X�g�p�o�b�t�@
int dad[N+1], lson[N+1], rson[N+257];	// ��
int matchpos, matchlen;					// �Œ���v�ʒu, ��v��
UCHAR *lpBtprs,*lpBtprsEnd;


/*===========================================================================
=============================================================================
		btprs
=============================================================================
========================================================================== */
void insert_node(int r)
{
//
//		�� r ��؂ɑ}��
//
	int i, p, cmp;
	unsigned char *key;

	cmp = 1;
	key = &text[r];
	p = N + 1 + key[0];
	rson[r] = lson[r] = NIL;
	matchlen = 0;

	for ( ; ; ) {
		if (cmp >= 0) {
			if (rson[p] != NIL) p = rson[p];
			else {  rson[p] = r;  dad[r] = p;  return;  }
		} else {
			if (lson[p] != NIL) p = lson[p];
			else {  lson[p] = r;  dad[r] = p;  return;  }
		}
		for (i = 1; i < F; i++)
			if ((cmp = key[i] - text[p + i]) != 0)  break;
		if (i > matchlen) {
			matchpos = p;
			if ((matchlen = i) >= F)  break;
		}
	}

	dad[r] = dad[p];
	lson[r] = lson[p];
	rson[r] = rson[p];
	dad[lson[p]] = r;
	dad[rson[p]] = r;

	if (rson[dad[p]] == p) {
		 rson[dad[p]] = r;
	} else {
		lson[dad[p]] = r;
	}
	dad[p] = NIL;  /* p ���O�� */
}
//---------------------------------------------------------------------------
void delete_node(int p)
{
//
//		�� p ��؂������
//
	int  q;

	if (dad[p] == NIL) return;  /* ������Ȃ� */

	if (rson[p] == NIL) {
		q = lson[p];
	} else if (lson[p] == NIL) {
		q = rson[p];
	} else {
		q = lson[p];
		if (rson[q] != NIL) {
			do {  q = rson[q];  } while (rson[q] != NIL);
			rson[dad[q]] = lson[q];  dad[lson[q]] = dad[q];
			lson[q] = lson[p];  dad[lson[p]] = q;
		}
		rson[q] = rson[p];  dad[rson[p]] = q;
	}
	dad[q] = dad[p];
	if (rson[dad[p]] == p) rson[dad[p]] = q;
	else                   lson[dad[p]] = q;
	dad[p] = NIL;
}


//---------------------------------------------------------------------------
void BtprsMain( int size )
{
//
//		btprs main(�P����lzss/lz77�ł�)
//
	int i0,i1;							/* loop counter for init dictionary */
	int i, c, len, r, s, lastmatchlen, codeptr;
	unsigned char code[17], mask;
	unsigned long int incount = 0 ;			/* �ǂݍ��� byte */
	unsigned long int printcount = 0;		/* 1024 byte ���ɕ\�����邽�� */


	fwrite(&size, sizeof size, 1, oFile);		/* size ���������� */
	fwrite(&size, sizeof size, 1, oFile);		/* size ���������� */


	// init tree
	for (i = N + 1; i <= N + 256; i++) {
		 rson[i] = NIL;
	}
	for (i = 0; i < N; i++) {
		dad[i] = NIL;
	}

	code[0] = 0;
	codeptr = mask = 1;
	s = 0;
	r = N - F;

	/* ------------------------------------------------------------------------
		init dictionary ( 4096 - 18 bytes )
	------------------------------------------------------------------------ */
	i = 0;
									/* 0 �` 255 �܂ł� 13 byte �Â��߂� */
	for (i0 = 0; i0 < 256; i0++) {
		for (i1 = 0; i1 < 13; i1++) {
			text[i++] = i0;
		}
	}

									/* 0 �` 255 �܂łŖ��߂� */
	for (i1 = 0; i1 < 256; i1++) {
			text[i++] = i1;
	}

									/* 255 �` 0 �܂łŖ��߂� */
	for (i1 = 255; i1 >= 0; i1--) {
			text[i++] = i1;
	}

									/* 0 �� 128 byte ���߂� */
	for (i1 = 0; i1 < 128; i1++) {
			text[i++] = 0;
	}

									/* 0x20 �� 128 - 18 byte ���߂� */
	for (i1 = 0; i1 < 128 - F; i1++) {
			text[i++] = 0x20;
	}

									/* 0 �� 18 byte ���߂� */
	for (i1 = 0; i1 < F; i1++) {
			text[i++] = 0;
	}

	/* ------------------------------------------------------------------------
		read first 18 bytes to dictionary
	------------------------------------------------------------------------ */
	for (len = 0; len < F ; len++) {
		if( lpBtprs >= lpBtprsEnd ) {
			break;
		} else {
			text[r + len] = *lpBtprs++;
		}
	}

	incount = len;
	if (incount == 0) {
		return;
	}
	for (i = 1; i <= F; i++) {
		 insert_node(r - i);
	}
	insert_node(r);



	do {
		if (matchlen > len) matchlen = len;
		if (matchlen < 3 ) {
			matchlen = 1;  code[0] |= mask;  code[codeptr++] = text[r];
		} else {
			code[codeptr++] = (unsigned char) matchpos;
			code[codeptr++] = (unsigned char)
				(((matchpos >> 4) & 0xf0) | (matchlen - 3 ));
		}
		if ((mask <<= 1) == 0) {
			for (i = 0; i < codeptr; i++) putc(code[i], oFile);
			outcount += codeptr;
			code[0] = 0;  codeptr = mask = 1;
		}
		lastmatchlen = matchlen;
		for (i = 0; i < lastmatchlen; i++) {
			if( lpBtprs >= lpBtprsEnd )	break;
			c = *lpBtprs++;
			delete_node(s);  text[s] = c;
			if (s < F - 1) text[s + N] = c;
			s = (s + 1) & (N - 1);  r = (r + 1) & (N - 1);
			insert_node(r);
		}

		while (i++ < lastmatchlen) {
			delete_node(s);
			s = (s + 1) & (N - 1);  r = (r + 1) & (N - 1);
			if (--len) insert_node(r);
		}

	} while (len > 0);

	if (codeptr > 1) {
		for (i = 0; i < codeptr; i++) putc(code[i], oFile);
		outcount += codeptr;
	}

	/* ------------------------------------------------------------------------
		write oFile size
	------------------------------------------------------------------------ */
	rewind(oFile);							/* �擪�ɖ߂� */
	fwrite(&outcount, sizeof outcount, 1, oFile);		/* size ���������� */
}

/*===========================================================================
=============================================================================
		compiler
=============================================================================
========================================================================== */
void FreeMemCompiler( void )
{
//
//		free ����
//
	if( LabelBuffer0 != NULL ) {
		free( LabelBuffer0 );
		LabelBuffer0 = NULL;
	}
	if( LabelBuffer1 != NULL ) {
		free( LabelBuffer1 );
		LabelBuffer1 = NULL;
	}
	if( LabelBuffer2 != NULL ) {
		free( LabelBuffer2 );
		LabelBuffer2 = NULL;
	}
	if( OutputBuff != NULL ) {
		free( OutputBuff );
		OutputBuff = NULL;
	}
	if( CalcMacBuff != NULL ) {
		free( CalcMacBuff );
		CalcMacBuff = NULL;
	}
	if( lpP != NULL ) {
		free( lpP );
		lpP = NULL;
	}
}
/*---------------------------------------------------------------------------
		error
---------------------------------------------------------------------------*/
void err_exit_compiler( void )
{
	FreeMemCompiler();
	if( oFile!=NULL ) {
		fclose( oFile );
		oFile = NULL;
	}
	if( lpOutErr != NULL ) {
		remove( lpOutErr );
	}
	err_exit();
}
//---------------------------------------------------------------------------
void err_compiler( int n )				// line ���b�Z�[�W��\��
{
	nErrComp+=n;
	if( nErrComp >100 ) {
		err_exit_compiler();
	}
	if( fLineOrg != 0 ) {
		printf( "%s(%d)", orgfilename, nLineOrg );
	} else {
		printf( "%s(%d)", sourcefilename, nLineSource );
	}
}
//---------------------------------------------------------------------------
void err_next_compiler( void )				// line ���b�Z�[�W��\��
{
	if( fLineOrg == 1 ) {
		printf( "%s(%d) �W�J���� macro �̈ʒu�͂����ł�\n", sourcefilename, nLineSource );
	}
}
//---------------------------------------------------------------------------
UCHAR* SkipSp( UCHAR *lp0 )
{
	while( *lp0 == ' ' || *lp0 == '\t' ) {
		lp0++;
	}
	return lp0;
}
//---------------------------------------------------------------------------
UCHAR* SkipSpK( UCHAR *lp0 )
//
//	()��space��tab���΂�
//
{
	while( *lp0 == ' ' || *lp0 == '\t' || *lp0 == ')' || *lp0 == '(' ) {
		lp0++;
	}
	return lp0;
}
/*---------------------------------------------------------------------------*/
void PrintHash1( void )
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
			n++;
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
//---------------------------------------------------------------------------
//		label buffer0 �֘A
//---------------------------------------------------------------------------
//
//		table0 buffer
//
//00h     type
//         01h label
//01h-04h address
//05h     label string
//
int ExpLabel( UCHAR *lp0,UCHAR *lp1 )
{
//
//	label name �̓W�J ( ���x�������m�̘A����h�����߁A���x������W�J������� space ������ )
//
//(out)			0:�W�J�I��
//				1:1 �ł��W�J�ł��Ȃ� label ���������ꍇ
//
	UCHAR c;							// back up
	UCHAR *lp2,*lp4;					// string pointer
#if 0
	UCHAR *lp3,*lp5;
#endif
	int f = 0;

	while( *lp0 != 0 ) {
		if( __iscsymf( *lp0 ) != 0 ) {
			lp2 = lp0;
			lp0++;
			while( __iscsym( *lp0 ) != 0 ) {
				lp0++;
			}
			c = *lp0;					// ascii zero �ɂ���
			*lp0 = 0;

#if 1
			lp4 = HashTable[GetHashKey( lp2 )];
			while( lp4!=NULL ) {
				if( strcmp( lp2,lp4+HASH_STR ) == 0 ) {
					_ultoa( *(UINT*)(lp4+HASH_POINTER),lp1,10 );
					lp1 += strlen( lp1 );
					*lp0 = c;		// ascii zero �����ɖ߂�
					*lp1++ = ' ';
					goto next;
				}
				lp4 = *(UCHAR**)(lp4);
			}
#else
			lp4 = LabelBuffer0;			// search label
			while( lp4 < lpLB0 ) {
				lp3 = lp2;
				lp5 = lp4;
				lp4+=HASH_STR;
				while( *lp3 == *lp4 ) {
					if( *lp3 == 0 ) {
						_ultoa( *(UINT*)(lp5+HASH_POINTER),lp1,10 );
						lp1 += strlen( lp1 );
						*lp0 = c;		// ascii zero �����ɖ߂�
						*lp1++ = ' ';
						goto next;
					}
					lp3++;
					lp4++;
				}
				// skip
				while( *lp4++ != 0 ) {};
			}
#endif

			*lp0 = c;					// ascii zero �����ɖ߂�

			// ������Ȃ������ꍇ
			f = 1;
			while( lp0 != lp2 ) {
				*lp1++ = *lp2++;
			}
		} else {
			*lp1++ = *lp0++;
		}
next:;
	}
	*lp1 = 0;
	return f;
}
//---------------------------------------------------------------------------
//		label buffer1 �֘A
//---------------------------------------------------------------------------
//
//		table1 buffer
//
//00h     type
//          01h byte type
//          02h word type
//          03h dword type
//01h-04h op address
//05h-08h source file number
//09h-0ch fLineOrg
//0dh-10h nLineOrg
//11h-    label name ( string asciizero )
//   -    source file asciizero
//   -    orgfilename
//
void RegistBuffer1( UCHAR *lp0,UCHAR type )
//
//	�����ŉ����ł��Ȃ�������U�o�^����
//
{
	if( lpLB1 > &LabelBuffer1[MAX_LABEL1-MAX_LINEBUFF] ) {
		err_compiler(1);
		printf( "compiler error:���x���̑O���Q�Ƃ��������܂�\n" );
		err_next_compiler();
//		err_exit_compiler();
		return;
	}

	*lpLB1++ = type;
	*(UCHAR**)(lpLB1) = lpOp;
	lpLB1+=4;
	*(int*)(lpLB1) = nLineSource;
	lpLB1+=4;
	*(int*)(lpLB1) = fLineOrg;
	lpLB1+=4;
	*(int*)(lpLB1) = nLineOrg;
	lpLB1+=4;

	while( *lp0 != 0 ) {
		*lpLB1++ = *lp0++;
	}
	*lpLB1++ = 0;

	lp0 = sourcefilename;
	while( *lp0 != 0 ) {
		*lpLB1++ = *lp0++;
	}
	*lpLB1++ = 0;

	if( fLineOrg != 0 ) {
		lp0 = orgfilename;
		while( *lp0 != 0 ) {
			*lpLB1++ = *lp0++;
		}
	}
	*lpLB1++ = 0;
}
//---------------------------------------------------------------------------
void SetLabelBuffer1( void )
{
//
//		LabelBuffer1 �̓��e���g���Alabel �� address �����肷��
//
	UCHAR *lp1,*lp2,*lpErrTmp,type;
	UCHAR buff0[MAX_LINEBUFF];
	int n;

	// LabelBuffer1 �̓��e���g���Alabel �� address �����肷��
	lp1 = LabelBuffer1;
	while( lp1 < lpLB1 ) {
		type = *lp1++;
		lp2 = *(UCHAR**)(lp1);
		lp1+=16;
		if( ExpLabel( lp1,buff0 ) == 0 ) {
			n = Calc( buff0,1 );
			switch( type ) {
				case 1:				// byte
					if( n < -128 || n >= 256 ) {
						// error
						nLineSource = *(UINT*)(lp1-12);
						fLineOrg = *(UINT*)(lp1-8);
						nLineOrg = *(UINT*)(lp1-4);
						lpErrTmp = lp1;
						while( *lpErrTmp++ != 0 ) {};			// skip label
						strcpy( sourcefilename, lpErrTmp );
						while( *lpErrTmp++ != 0 ) {};			// skip sourcefilename
						strcpy( orgfilename, lpErrTmp );
//						strcpy( sourcefilename, lpErrTmp );
						err_compiler(1);
						printf( "compiler error:������ 8bit ����I�[�o�[�t���[���Ă��܂�\n" );
						err_next_compiler();
//						err_exit_compiler();
					} else {
						*lp2 = (UCHAR)n;
					}
					break;
				case 2:				// word
					if( n < -32768 || n >= 65536 ) {
						// error
						nLineSource = *(UINT*)(lp1-12);
						fLineOrg = *(UINT*)(lp1-8);
						nLineOrg = *(UINT*)(lp1-4);
						lpErrTmp = lp1;
						while( *lpErrTmp++ != 0 ) {};			// skip label
						strcpy( sourcefilename, lpErrTmp );
						while( *lpErrTmp++ != 0 ) {};			// skip sourcefilename
						strcpy( orgfilename, lpErrTmp );
//						strcpy( sourcefilename, lpErrTmp );
						err_compiler(1);
						printf( "compiler error:������ 16bit ����I�[�o�[�t���[���Ă��܂�\n" );
						err_next_compiler();
//						err_exit_compiler();
					} else {
						*(unsigned short*)(lp2) = (unsigned short)(n&0xffff);
					}
					break;
				case 3:				// dword
					*(int*)(lp2) = n;
					break;
			}
		} else {
			// error
			nLineSource = *(UINT*)(lp1-12);
			fLineOrg = *(UINT*)(lp1-8);
//			if( fLineOrg == 1 ) fLineOrg = 2;
			nLineOrg = *(UINT*)(lp1-4);
			lpErrTmp = lp1;
			while( *lpErrTmp++ != 0 ) {};			// skip label
			strcpy( sourcefilename, lpErrTmp );
			while( *lpErrTmp++ != 0 ) {};			// skip sourcefilename
			strcpy( orgfilename, lpErrTmp );
//			strcpy( sourcefilename, lpErrTmp );
			err_compiler(1);
			printf( "compiler error:�v�Z���ɊԈႢ������܂�\n" );
			err_next_compiler();
//			err_exit_compiler();
		}
		while( *lp1++ != 0 ) {};			// skip label
		while( *lp1++ != 0 ) {};			// skip sourcefilename
		while( *lp1++ != 0 ) {};			// skip orgfilename
	}
	return;
}
/*---------------------------------------------------------------------------
		calc0
---------------------------------------------------------------------------*/
int isexpdec( int c )
{
//
//		dec �� check ����
//
	if( c >= '0' && c <= '9' ) {
		return ( c-'0' );
	}

	return -1;
}
//---------------------------------------------------------------------------
isexpcalc( UCHAR c )
{
//
//		calc
//
//	if( c=='R' || c=='r' || c=='\t' || c==' ' || c=='!' || c=='%' || c=='&' ||
//	    ( c>='(' && c<='+' ) || c=='-' || ( c>='/' && c<='9' ) ||
//	    ( c>='<' && c<='>' ) || c=='^' || c=='|' || c=='~' ) {
	if( ( c>='a' && c<='z' ) || (c>='A' && c<='Z' ) || c=='\t' || c==' ' || c=='!' || c=='%' || c=='&' ||
	    ( c>='(' && c<='+' ) || c=='-' || ( c>='/' && c<='9' ) ||
	    ( c>='<' && c<='>' ) || c=='^' || c=='|' || c=='~' ) {
		return 1;
	}
	return 0;
}
//---------------------------------------------------------------------------
void ExpCalcError( UCHAR* lp0 )
{
	err_compiler(1);
	if( lpExpCalcErr == NULL ){
		printf( "compiler error:%s\n",lp0 );
	} else {
		printf( "compiler error:%s(%s)\n",lp0,lpExpCalcErr );
	}
	err_next_compiler();
//	err_exit_compiler();
}
//---------------------------------------------------------------------------
void ExpCalcSub( void )
{
	*lpCalcData = ((ExpCalcOpe[ExpCalcSp0]&0xff)+0x80);
	lpCalcData++;
//	putc( ((ExpCalcOpe[ExpCalcSp0]&0xff)+0x80), oFile );
	switch( ExpCalcOpe[ExpCalcSp0] ) {
//		case CALCODE_LS:
//			break;
		case CALCODE_PLUS:
			if( ExpCalcSp1 <= 0 ) {
				ExpCalcError( "'+' �̌�ɐ��l������܂���" );
			}
//			printf( "plus \n" );
//			ExpCalcValue[ExpCalcSp1] = ExpCalcValue[ExpCalcSp1];
			break;
		case CALCODE_MINUS:
			if( ExpCalcSp1 <= 0 ) {
				ExpCalcError( "'-' �̌�ɐ��l������܂���" );
			}
//			printf( "minus \n" );
//			ExpCalcValue[ExpCalcSp1] = - ExpCalcValue[ExpCalcSp1];
			break;
		case CALCODE_NOT:
			if( ExpCalcSp1 <= 0 ) {
				ExpCalcError( "'~' �̌�ɐ��l������܂���" );
			}
//			printf( "neg \n" );
//			ExpCalcValue[ExpCalcSp1] = ~ExpCalcValue[ExpCalcSp1];
			break;
		case CALCODE_NEG:
			if( ExpCalcSp1 <= 0 ) {
				ExpCalcError( "'!' �̌�ɐ��l������܂���" );
			}
//			printf( "not \n" );
//			ExpCalcValue[ExpCalcSp1] = !ExpCalcValue[ExpCalcSp1];
			break;
		case CALCODE_MUL:
			if( ExpCalcSp1 <= 1 ) {
				ExpCalcError( "'*' ���ُ�ł�" );
			}
//			printf( "* \n" );
//			ExpCalcValue[ExpCalcSp1-1] = ExpCalcValue[ExpCalcSp1-1] * ExpCalcValue[ExpCalcSp1];
			ExpCalcSp1--;
			break;
		case CALCODE_DIV:
			if( ExpCalcSp1 <= 1 ) {
				ExpCalcError( "'/' ���ُ�ł�" );
			}
//			printf( "/ \n" );
//			ExpCalcValue[ExpCalcSp1-1] = ExpCalcValue[ExpCalcSp1-1] / ExpCalcValue[ExpCalcSp1];
			ExpCalcSp1--;
			break;
		case CALCODE_MOD:
			if( ExpCalcSp1 <= 1 ) {
				ExpCalcError( "'%' ���ُ�ł�" );
			}
//			printf( "mod \n" );
//			ExpCalcValue[ExpCalcSp1-1] = ExpCalcValue[ExpCalcSp1-1] % ExpCalcValue[ExpCalcSp1];
			ExpCalcSp1--;
			break;
		case CALCODE_ADD:
			if( ExpCalcSp1 <= 1 ) {
				ExpCalcError( "'+' ���ُ�ł�" );
			}
//			printf( "+ \n" );
//			ExpCalcValue[ExpCalcSp1-1] = ExpCalcValue[ExpCalcSp1-1] + ExpCalcValue[ExpCalcSp1];
			ExpCalcSp1--;
			break;
		case CALCODE_SUB:
			if( ExpCalcSp1 <= 1 ) {
				ExpCalcError( "'-' ���ُ�ł�" );
			}
//			printf( "- \n" );
//			ExpCalcValue[ExpCalcSp1-1] = ExpCalcValue[ExpCalcSp1-1] - ExpCalcValue[ExpCalcSp1];
			ExpCalcSp1--;
			break;
		case CALCODE_SHL:
			if( ExpCalcSp1 <= 1 ) {
				ExpCalcError( "'<<' ���ُ�ł�" );
			}
//			printf( "<< \n" );
//			ExpCalcValue[ExpCalcSp1-1] = ExpCalcValue[ExpCalcSp1-1] << ExpCalcValue[ExpCalcSp1];
			ExpCalcSp1--;
			break;
		case CALCODE_SHR:
			if( ExpCalcSp1 <= 1 ) {
				ExpCalcError( "'>>' ���ُ�ł�" );
			}
//			printf( ">> \n" );
//			ExpCalcValue[ExpCalcSp1-1] = ExpCalcValue[ExpCalcSp1-1] >> ExpCalcValue[ExpCalcSp1];
			ExpCalcSp1--;
			break;
		case CALCODE_MORE:
			if( ExpCalcSp1 <= 1 ) {
				ExpCalcError( "'>=' ���ُ�ł�" );
			}
//			printf( ">= \n" );
//			ExpCalcValue[ExpCalcSp1-1] = ( ExpCalcValue[ExpCalcSp1-1] >= ExpCalcValue[ExpCalcSp1] );
			ExpCalcSp1--;
			break;
		case CALCODE_BIG:
			if( ExpCalcSp1 <= 1 ) {
				ExpCalcError( "'>' ���ُ�ł�" );
			}
//			printf( "> \n" );
//			ExpCalcValue[ExpCalcSp1-1] = ( ExpCalcValue[ExpCalcSp1-1] > ExpCalcValue[ExpCalcSp1] );
			ExpCalcSp1--;
			break;
		case CALCODE_LESS:
			if( ExpCalcSp1 <= 1 ) {
				ExpCalcError( "'<=' ���ُ�ł�" );
			}
//			printf( "< \n" );
//			ExpCalcValue[ExpCalcSp1-1] = ( ExpCalcValue[ExpCalcSp1-1] <= ExpCalcValue[ExpCalcSp1] );
			ExpCalcSp1--;
			break;
		case CALCODE_SMALL:
			if( ExpCalcSp1 <= 1 ) {
				ExpCalcError( "'<' ���ُ�ł�" );
			}
//			printf( "<= \n" );
//			ExpCalcValue[ExpCalcSp1-1] = ( ExpCalcValue[ExpCalcSp1-1] < ExpCalcValue[ExpCalcSp1] );
			ExpCalcSp1--;
			break;
		case CALCODE_EQU:
			if( ExpCalcSp1 <= 1 ) {
				ExpCalcError( "'==' ���ُ�ł�" );
			}
//			printf( "== \n" );
//			ExpCalcValue[ExpCalcSp1-1] = ( ExpCalcValue[ExpCalcSp1-1] == ExpCalcValue[ExpCalcSp1] );
			ExpCalcSp1--;
			break;
		case CALCODE_NEQU:
			if( ExpCalcSp1 <= 1 ) {
				ExpCalcError( "'!=' ���ُ�ł�" );
			}
//			printf( "!= \n" );
//			ExpCalcValue[ExpCalcSp1-1] = ( ExpCalcValue[ExpCalcSp1-1] != ExpCalcValue[ExpCalcSp1] );
			ExpCalcSp1--;
			break;
		case CALCODE_BITAND:
			if( ExpCalcSp1 <= 1 ) {
				ExpCalcError( "'&' ���ُ�ł�" );
			}
//			printf( "& \n" );
//			ExpCalcValue[ExpCalcSp1-1] = ( ExpCalcValue[ExpCalcSp1-1] & ExpCalcValue[ExpCalcSp1] );
			ExpCalcSp1--;
			break;
		case CALCODE_BITXOR:
			if( ExpCalcSp1 <= 1 ) {
				ExpCalcError( "'^' ���ُ�ł�" );
			}
//			printf( "^ \n" );
//			ExpCalcValue[ExpCalcSp1-1] = ( ExpCalcValue[ExpCalcSp1-1] ^ ExpCalcValue[ExpCalcSp1] );
			ExpCalcSp1--;
			break;
		case CALCODE_BITOR:
			if( ExpCalcSp1 <= 1 ) {
				ExpCalcError( "'|' ���ُ�ł�" );
			}
//			printf( "| \n" );
//			ExpCalcValue[ExpCalcSp1-1] = ( ExpCalcValue[ExpCalcSp1-1] | ExpCalcValue[ExpCalcSp1] );
			ExpCalcSp1--;
			break;
		case CALCODE_AND:
			if( ExpCalcSp1 <= 1 ) {
				ExpCalcError( "'&&' ���ُ�ł�" );
			}
//			printf( "&& \n" );
//			ExpCalcValue[ExpCalcSp1-1] = ( ExpCalcValue[ExpCalcSp1-1] && ExpCalcValue[ExpCalcSp1] );
			ExpCalcSp1--;
			break;
		case CALCODE_OR:
			if( ExpCalcSp1 <= 1 ) {
				ExpCalcError( "'||' ���ُ�ł�" );
			}
//			printf( "|| \n" );
//			ExpCalcValue[ExpCalcSp1-1] = ( ExpCalcValue[ExpCalcSp1-1] || ExpCalcValue[ExpCalcSp1] );
			ExpCalcSp1--;
			break;
		case CALCODE_ADDRESS:
			if( ExpCalcSp1 <= 0 ) {
				ExpCalcError( "'&' �̌�ɐ��l������܂���" );
			}
//			printf( "address \n" );
			break;
		case CALCODE_VALUE:
			if( ExpCalcSp1 <= 0 ) {
				ExpCalcError( "'*' �̌�ɐ��l������܂���" );
			}
//			printf( "value \n" );
			break;
		case CALCODE_LET:
			if( ExpCalcSp1 <= 1 ) {
				ExpCalcError( "'=' ���ُ�ł�" );
			}
//			printf( "let \n" );
			break;
//		case CALCODE_LE:
//			break;
	}
	ExpCalcSp0--;
}
//---------------------------------------------------------------------------
int ExpCalcMain( UCHAR *lp0 )
//
//	lp0 �ŗ^����ꂽ������ ExpCalc �֓����
//
//(in)	lp0		= ����
//		mode	= 
//
//(out)	0:����I��
//		1:���x���̑O���Q�Ƃ������ł��Ȃ�����
//
{
	int c,d,v;
	int f;					// �O���Q�ƂŁA�����ł��Ȃ�������
	int before;				// �ȑO�̌^  1:���l / ���W�X�^�[ 0:����ȊO
	UCHAR *lpTmp,*lp0org;
	UCHAR buff0[MAX_LINEBUFF];
	UCHAR buff1[MAX_LINEBUFF];

	f = 0;

	// set pointer
	lpCalcData = ExpCalcData;

	// skip space
	while( *lp0=='\t' || *lp0==' ' ) {
		lp0++;
	}
	if( isexpcalc( *lp0 ) == 0 ){
		ExpCalcError( "�v�Z�����ُ�ł�" );
	}

	ExpCalcOpe[0]=0;							// �ԕ�
	ExpCalcPri[0]=-1;

	ExpCalcSp0=ExpCalcSp1=0;
	before = 0;
	while( 1 ) {
		while( *lp0=='\t' || *lp0==' ' ) {
			lp0++;
		}

		// c = GetCharData();
		lp0org = lp0;
		c = *lp0++;
		if(( c == 'r' || c == 'R' ) && isexpdec( *lp0  != -1 )) {
			//
			//	���W�X�^
			//
			v = isexpdec( *lp0++ );
			while(( d = isexpdec( c = *lp0++ )) != - 1 ) {
				v = v*10 + d;
			}
			lp0--;
			if( isalpha(*lp0 ) ) {					// r000a ���̏ꍇ�A���x���ƌ��Ȃ�
				lp0 = lp0org;
				c = *lp0++;
				goto type_else;
			}
			before = 1;
			ExpCalcSp1++;
			// write
			*lpCalcData = 2;
			lpCalcData++;
			*(int*)(lpCalcData) = v;
			lpCalcData+=4;

		} else if(( d = isexpdec( c )) != - 1 ) {
			//
			//	���l
			//
			v = d;
			while(( d = isexpdec( c = *lp0++ )) != - 1 ) {
				v = v*10 + d;
			}
			lp0--;
type_imm:
			before = 1;
			ExpCalcSp1++;
			// write
			*lpCalcData = 1;
			lpCalcData++;
			*(int*)(lpCalcData) = v;
			lpCalcData+=4;

		} else {
type_else:
			switch( c ) {
				case '(':
					if( before != 0 ) {
						ExpCalcError( "�v�Z���� '(' ���ُ�ł�" );
					}
					d = CALCODE_LS;
					before = 0;
					break;
				case '~':
					d = CALCODE_NOT;
					break;
				case '!':
					// c = GetCharData();
					c = *lp0++;
					if( c != '=' ) {
						// UnGetCharData( c );
						lp0--;
						d = CALCODE_NEG;
					} else {
						d = CALCODE_NEQU;
						before = 0;
					}
					break;
				case '*':
					if( before == 0 ) {
						d = CALCODE_VALUE;
					} else {
						d = CALCODE_MUL;
						before = 0;
					}
					break;
				case '/':
					if( before == 0 ) {
						ExpCalcError( "�v�Z���� '/' ���ُ�ł�" );
					}
					d = CALCODE_DIV;
					before = 0;
					break;
				case '%':
					if( before == 0 ) {
						ExpCalcError( "�v�Z���� '%' ���ُ�ł�" );
					}
					d = CALCODE_MOD;
					before = 0;
					break;
				case '<':
					// c = GetCharData();
					c = *lp0++;
					if( c == '<' ) {
						if( before == 0 ) {
							ExpCalcError( "�v�Z���� '<<' ���ُ�ł�" );
						}
						d = CALCODE_SHL;
						before = 0;
					} else if( c == '=' ) {
						if( before == 0 ) {
							ExpCalcError( "�v�Z���� '<=' ���ُ�ł�" );
						}
						d = CALCODE_LESS;
						before = 0;
					} else {
						if( before == 0 ) {
							ExpCalcError( "�v�Z���� '<' ���ُ�ł�" );
						}
						// UnGetCharData( c );
						lp0--;
						d = CALCODE_SMALL;
						before = 0;
					}
					break;
				case '>':
					// c = GetCharData();
					c = *lp0++;
					if( c == '>' ) {
						if( before == 0 ) {
							ExpCalcError( "�v�Z���� '>>' ���ُ�ł�" );
						}
						d = CALCODE_SHR;
						before = 0;
					} else if( c == '=' ) {
						if( before == 0 ) {
							ExpCalcError( "�v�Z���� '>=' ���ُ�ł�" );
						}
						d = CALCODE_MORE;
						before = 0;
					} else {
						if( before == 0 ) {
							ExpCalcError( "�v�Z���� '>' ���ُ�ł�" );
						}
						// UnGetCharData( c );
						lp0--;
						d = CALCODE_BIG;
						before = 0;
					}
					break;
				case '=':
					// c = GetCharData();
					c = *lp0++;
					if( c == '=' ) {
						if( before == 0 ) {
							ExpCalcError( "�v�Z���� '==' ���ُ�ł�" );
						}
						d = CALCODE_EQU;
						before = 0;
					} else {
						// UnGetCharData( c );
						lp0--;
						if( before == 0 ) {
							ExpCalcError( "�v�Z���� '=' ���ُ�ł�" );
						}
						d = CALCODE_LET;
						before = 0;
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
					// c = GetCharData();
					c = *lp0++;
					if( c != '&' ) {
						// UnGetCharData( c );
						lp0--;
						if( before == 0 ) {
							d = CALCODE_ADDRESS;
						} else {
							d = CALCODE_BITAND;
							before = 0;
						}
					} else {
						if( before == 0 ) {
							ExpCalcError( "�v�Z���� '&&' ���ُ�ł�" );
						}
						d = CALCODE_AND;
						before = 0;
					}
					break;
				case '^':
					if( before == 0 ) {
						ExpCalcError( "�v�Z���� '^' ���ُ�ł�" );
					}
					d = CALCODE_BITXOR;
					before = 0;
					break;
				case '|':
					// c = GetCharData();
					c = *lp0++;
					if( c != '|' ) {
						if( before == 0 ) {
							ExpCalcError( "�v�Z���� '|' ���ُ�ł�" );
						}
						// UnGetCharData( c );
						lp0--;
						d = CALCODE_BITOR;
						before = 0;
					} else {
						if( before == 0 ) {
							ExpCalcError( "�v�Z���� '||' ���ُ�ł�" );
						}
						d = CALCODE_OR;
						before = 0;
					}
					break;
				case ')':
					if( before == 0 ) {
						ExpCalcError( "�v�Z���� ')' ���ُ�ł�" );
					}
					d = CALCODE_LE;
					break;
				case 0:
				case '\n':
					// UnGetCharData( c );
					lp0--;
					d = CALCODE_EXIT;
					break;
				default:
					//
					// 	���x��
					//
					// token �̐؂蕪��
					lpTmp = buff0;
					*lpTmp++=c;
					while( __iscsym(*lp0) ) {
						*lpTmp++=*lp0++;
					}
					*lpTmp=0;
					// �O���Q�Ƃ�?
					if( ExpLabel( buff0,buff1 ) == 0 ) {
						v = Calc( buff1,1 );
					} else {
						v = 1;							// ���̒l
						f = 1;							// �O���Q�ƗL��
					}
					goto type_imm;
			}
			if( d == CALCODE_EXIT ) {
				break;
			}

			while( 1 ) {
				if( ExpCalcOpe[ExpCalcSp0] == CALCODE_LS ) {
					break;
				}
				if( ExpCalcLeft[d] == 0 ) {
					// ������E
					if( ExpCalcPri[d] > ExpCalcPri[ExpCalcOpe[ExpCalcSp0]] ) {
						break;
					}
				} else {
					// �E���獶
					if( ExpCalcPri[d] >= ExpCalcPri[ExpCalcOpe[ExpCalcSp0]] ) {
						break;
					}
				}
				ExpCalcSub();
			}
			if( d != CALCODE_LE ) {
				ExpCalcOpe[++ExpCalcSp0] = d;
			} else {
				ExpCalcSp0--;					// '(' ����菜��
			}
		}
	}

	while( ExpCalcSp0>0 ) {					// ���Z�q stack ����ɂȂ�܂�
		ExpCalcSub();
	}
	// end mark
	*lpCalcData = 0;
//	putc( 0, oFile );
//	return ExpCalcValue[1];
	return f;
}
//---------------------------------------------------------------------------
void PrintCalc( UCHAR *lp )
{


	while( 1 ) {
		switch( *((UCHAR*)lp)++ ) {
			case 0:
				printf( "\n" );
				return;
			case 1:								// ���l
				printf( "%d ",*(int*)(lp) );
				(UCHAR*)lp+=4;
				break;
			case 2:								// reg
				printf( "r%d ",*(int*)(lp) );
				(UCHAR*)lp+=4;
				break;
			case CALCODE_PLUS+0x80:
				printf( "plus " );
				break;
			case CALCODE_MINUS+0x80:
				printf( "minus " );
				break;
			case CALCODE_NOT+0x80:
				printf( "not " );
				break;
			case CALCODE_NEG+0x80:
				printf( "neg " );
				break;
			case CALCODE_MUL+0x80:
				printf( "MUL " );
				break;
			case CALCODE_DIV+0x80:
				printf( "DIV " );
				break;
			case CALCODE_MOD+0x80:
				printf( "MOD " );
				break;
			case CALCODE_ADD+0x80:
				printf( "ADD " );
				break;
			case CALCODE_SUB+0x80:
				printf( "SUB " );
				break;
			case CALCODE_SHL+0x80:
				printf( "SHL " );
				break;
			case CALCODE_SHR+0x80:
				printf( "SHR " );
				break;
			case CALCODE_MORE+0x80:
				printf( "MORE " );
				break;
			case CALCODE_BIG+0x80:
				printf( "BIG " );
				break;
			case CALCODE_LESS+0x80:
				printf( "LESS " );
				break;
			case CALCODE_SMALL+0x80:
				printf( "SMALL " );
				break;
			case CALCODE_EQU+0x80:
				printf( "EQU " );
				break;
			case CALCODE_NEQU+0x80:
				printf( "NEQU " );
				break;
			case CALCODE_BITAND+0x80:
				printf( "BITAND " );
				break;
			case CALCODE_BITXOR+0x80:
				printf( "BITXOR " );
				break;
			case CALCODE_BITOR+0x80:
				printf( "BITOR " );
				break;
			case CALCODE_AND+0x80:
				printf( "AND " );
				break;
			case CALCODE_OR+0x80:
				printf( "OR " );
				break;
			case CALCODE_LET+0x80:
				printf( "LET " );
				break;
			case CALCODE_ADDRESS+0x80:
				printf( "address " );
				break;
			case CALCODE_VALUE+0x80:
				printf( "value " );
				break;
		}
	}
}
/*---------------------------------------------------------------------------
		opt/calc check
---------------------------------------------------------------------------*/
void CehckCalc0( UCHAR *lpSocNow )
{
//
//	check calc
//
//type
//	reg     = 2 �ȊO�̏ꍇ�ɂ� 1
//	value   = ���O�� reg �̏ꍇ�ɂ� 2�A����ȊO�̎��ɂ� reg �Ɠ���
//	address = ���O�����l�̏ꍇ�ɂ� error
//	���Z�q  = 2 �ȊO�̏ꍇ�ɂ� 0
//
//
//	get int para ( int �̃p�����[�^�[�̏E�� / prereg.txt ���Q�� )
//
	int n;

	lpCalcSp0 = CalcValue;
	lpCalcSp1 = CalcType;

	while( 1 ) {
		switch( *((UCHAR*)lpSocNow)++ ) {
			case 0:
				return;
			case 1:								// ���l
				++lpCalcSp0;
				++lpCalcSp1;
				*lpCalcSp1 = -1;
				//*lpCalcSp0 = *(int*)(lpSocNow);
				(UCHAR*)lpSocNow+=4;
				break;
			case 2:								// reg
				n = 0;
				++lpCalcSp0;
				++lpCalcSp1;
				*lpCalcSp1 = n;
				*lpCalcSp0 = 0;
				(UCHAR*)lpSocNow+=4;
				break;
			case CALCODE_PLUS+0x80:
				*lpCalcSp1 = -1;
//				*lpCalcSp0 = *lpCalcSp0;
				break;
			case CALCODE_MINUS+0x80:
				*lpCalcSp1 = -1;
				*lpCalcSp0 = -(*lpCalcSp0);
				break;
			case CALCODE_NOT+0x80:
				*lpCalcSp1 = -1;
				*lpCalcSp0 = ~(*lpCalcSp0);
				break;
			case CALCODE_NEG+0x80:
				*lpCalcSp1 = -1;
				*lpCalcSp0 = !(*lpCalcSp0);
				break;
			case CALCODE_MUL+0x80:
				//*(lpCalcSp0-1) = ( *(lpCalcSp0-1) * (*lpCalcSp0));
				lpCalcSp0--;
				lpCalcSp1--;
				*lpCalcSp1 = -1;
				break;
			case CALCODE_DIV+0x80:
				//*(lpCalcSp0-1) = ( *(lpCalcSp0-1) / (*lpCalcSp0));
				lpCalcSp0--;
				lpCalcSp1--;
				*lpCalcSp1 = -1;
				break;
			case CALCODE_MOD+0x80:
				//*(lpCalcSp0-1) = ( *(lpCalcSp0-1) % (*lpCalcSp0));
				lpCalcSp0--;
				lpCalcSp1--;
				*lpCalcSp1 = -1;
				break;
			case CALCODE_ADD+0x80:
				//*(lpCalcSp0-1) = ( *(lpCalcSp0-1) + (*lpCalcSp0));
				lpCalcSp0--;
				lpCalcSp1--;
				*lpCalcSp1 = -1;
				break;
			case CALCODE_SUB+0x80:
				//*(lpCalcSp0-1) = ( *(lpCalcSp0-1) - (*lpCalcSp0));
				lpCalcSp0--;
				lpCalcSp1--;
				*lpCalcSp1 = -1;
				break;
			case CALCODE_SHL+0x80:
				//*(lpCalcSp0-1) = ( *(lpCalcSp0-1) << (*lpCalcSp0));
				lpCalcSp0--;
				lpCalcSp1--;
				*lpCalcSp1 = -1;
				break;
			case CALCODE_SHR+0x80:
				//*(lpCalcSp0-1) = ( *(lpCalcSp0-1) >> (*lpCalcSp0));
				lpCalcSp0--;
				lpCalcSp1--;
				*lpCalcSp1 = -1;
				break;
			case CALCODE_MORE+0x80:
				//*(lpCalcSp0-1) = ( *(lpCalcSp0-1) >= (*lpCalcSp0));
				lpCalcSp0--;
				lpCalcSp1--;
				*lpCalcSp1 = -1;
				break;
			case CALCODE_BIG+0x80:
				//*(lpCalcSp0-1) = ( *(lpCalcSp0-1) > (*lpCalcSp0));
				lpCalcSp0--;
				lpCalcSp1--;
				*lpCalcSp1 = -1;
				break;
			case CALCODE_LESS+0x80:
				//*(lpCalcSp0-1) = ( *(lpCalcSp0-1) <= (*lpCalcSp0));
				lpCalcSp0--;
				lpCalcSp1--;
				*lpCalcSp1 = -1;
				break;
			case CALCODE_SMALL+0x80:
				//*(lpCalcSp0-1) = ( *(lpCalcSp0-1) < (*lpCalcSp0));
				lpCalcSp0--;
				lpCalcSp1--;
				*lpCalcSp1 = -1;
				break;
			case CALCODE_EQU+0x80:
				//*(lpCalcSp0-1) = ( *(lpCalcSp0-1) == (*lpCalcSp0));
				lpCalcSp0--;
				lpCalcSp1--;
				*lpCalcSp1 = -1;
				break;
			case CALCODE_NEQU+0x80:
				//*(lpCalcSp0-1) = ( *(lpCalcSp0-1) != (*lpCalcSp0));
				lpCalcSp0--;
				lpCalcSp1--;
				*lpCalcSp1 = -1;
				break;
			case CALCODE_BITAND+0x80:
				//*(lpCalcSp0-1) = ( *(lpCalcSp0-1) & (*lpCalcSp0));
				lpCalcSp0--;
				lpCalcSp1--;
				*lpCalcSp1 = -1;
				break;
			case CALCODE_BITXOR+0x80:
				//*(lpCalcSp0-1) = ( *(lpCalcSp0-1) ^ (*lpCalcSp0));
				lpCalcSp0--;
				lpCalcSp1--;
				*lpCalcSp1 = -1;
				break;
			case CALCODE_BITOR+0x80:
				//*(lpCalcSp0-1) = ( *(lpCalcSp0-1) | (*lpCalcSp0));
				lpCalcSp0--;
				lpCalcSp1--;
				*lpCalcSp1 = -1;
				break;
			case CALCODE_AND+0x80:
				//*(lpCalcSp0-1) = ( *(lpCalcSp0-1) && (*lpCalcSp0));
				lpCalcSp0--;
				lpCalcSp1--;
				*lpCalcSp1 = -1;
				break;
			case CALCODE_OR+0x80:
				//*(lpCalcSp0-1) = ( *(lpCalcSp0-1) || (*lpCalcSp0));
				lpCalcSp0--;
				lpCalcSp1--;
				*lpCalcSp1 = -1;
				break;
			case CALCODE_LET+0x80:
				if( *(lpCalcSp1-1) == -1 ) {
					ExpCalcError( "��������ُ�" );
				}
				//REGDATA(*(lpCalcSp1-1)) = *lpCalcSp0;
				*(lpCalcSp0-1) = *lpCalcSp0;
				lpCalcSp0--;
				lpCalcSp1--;
				*lpCalcSp1 = -1;
				break;
			case CALCODE_ADDRESS+0x80:
				if( *lpCalcSp1 == -1 ) {
					ExpCalcError( "�A�h���X���Z�q���ُ�" );
				}
				*lpCalcSp0 = *lpCalcSp1;
				*lpCalcSp1 = -1;
				break;
			case CALCODE_VALUE+0x80:
				n = *lpCalcSp0;
				*lpCalcSp1 = n;
				*lpCalcSp0 = 0;
				break;
		}
	}
}
//---------------------------------------------------------------------------
void OptCalc0( UCHAR *lpRead )
{
//
//	calc �̍œK��
//
	int swap;
	int f,fBefore,nValue;
	UCHAR WB[CALOPE_MAX*5],*lpWB,*lp0,c;

//	if( nWarning == 1 ){
//		err_compiler(1);
//		printf( "calc before:" );
//		PrintCalc( lpRead );
//	}

	swap = 0;
	f = TRUE;
	while( f ==  TRUE ) {
		f = FALSE;

		//
		//	�P�����Z�q / �P���� 2 �����Z�q�̍œK�� lpRread -> lpWB
		//
		if( swap == 0 ) {
			lp0 = lpRead;
			lpWB = WB;
			swap = 1;
		} else {
			lp0 = WB;
			lpWB = lpRead;
			swap = 0;
		}
		nValue = 0;
		fBefore = 0;
		while( ( c= *lp0 ) != 0 ) {
			lp0++;
			switch( c ) {
				case 1:								// ���l
					*lpWB++ = 1;
					*(int*)(lpWB) = *(int*)(lp0);
					lp0+=4;
					lpWB+=4;
					fBefore=1;
					nValue++;
					break;
				case 2:								// reg
					*lpWB++ = 2;
					*(int*)(lpWB) = *(int*)(lp0);
					lp0+=4;
					lpWB+=4;
					fBefore=2;
					nValue = 0;
					break;
				case CALCODE_PLUS+0x80:
					f = TRUE;
					// *lpWB++ = CALCODE_PLUS+0x80;
					break;
				case CALCODE_MINUS+0x80:
					if( fBefore == 1 ) {
						f = TRUE;
						*(int*)(lpWB-4) = -(*(int*)(lpWB-4));
					} else {
						fBefore = 0;
						nValue = 0;
						*lpWB++ = CALCODE_MINUS+0x80;
					}
					break;
				case CALCODE_NOT+0x80:
					if( fBefore == 1 ) {
						f = TRUE;
						*(int*)(lpWB-4) = ~(*(int*)(lpWB-4));
					} else {
						fBefore = 0;
						nValue = 0;
						*lpWB++ = CALCODE_NOT+0x80;
					}
					break;
				case CALCODE_NEG+0x80:
					if( fBefore == 1 ) {
						f = TRUE;
						*(int*)(lpWB-4) = !(*(int*)(lpWB-4));
					} else {
						fBefore = 0;
						nValue = 0;
						*lpWB++ = CALCODE_NEG+0x80;
					}
					break;
				case CALCODE_MUL+0x80:
					if( nValue >= 2 ) {
						lpWB-=5;
						*(int*)(lpWB-4) = (*(int*)(lpWB-4)) * (*(int*)(lpWB+1));
						nValue--;
					} else {
						fBefore = 0;
						nValue = 0;
						*lpWB++ = CALCODE_MUL+0x80;
					}
					break;
				case CALCODE_DIV+0x80:
					if( nValue >= 2 ) {
						lpWB-=5;
						*(int*)(lpWB-4) = (*(int*)(lpWB-4)) / (*(int*)(lpWB+1));
						nValue--;
					} else {
						fBefore = 0;
						nValue = 0;
						*lpWB++ = CALCODE_DIV+0x80;
					}
					break;
				case CALCODE_MOD+0x80:
					if( nValue >= 2 ) {
						lpWB-=5;
						*(int*)(lpWB-4) = (*(int*)(lpWB-4)) % (*(int*)(lpWB+1));
						nValue--;
					} else {
						fBefore = 0;
						nValue = 0;
						*lpWB++ = CALCODE_MOD+0x80;
					}
					break;
				case CALCODE_ADD+0x80:
					if( nValue >= 2 ) {
						lpWB-=5;
						*(int*)(lpWB-4) = (*(int*)(lpWB-4)) + (*(int*)(lpWB+1));
						nValue--;
					} else {
						fBefore = 0;
						nValue = 0;
						*lpWB++ = CALCODE_ADD+0x80;
					}
					break;
				case CALCODE_SUB+0x80:
					if( nValue >= 2 ) {
						lpWB-=5;
						*(int*)(lpWB-4) = (*(int*)(lpWB-4)) - (*(int*)(lpWB+1));
						nValue--;
					} else {
						fBefore = 0;
						nValue = 0;
						*lpWB++ = CALCODE_SUB+0x80;
					}
					break;
				case CALCODE_SHL+0x80:
					if( nValue >= 2 ) {
						lpWB-=5;
						*(int*)(lpWB-4) = (*(int*)(lpWB-4)) << (*(int*)(lpWB+1));
						nValue--;
					} else {
						fBefore = 0;
						nValue = 0;
						*lpWB++ = CALCODE_SHL+0x80;
					}
					break;
				case CALCODE_SHR+0x80:
					if( nValue >= 2 ) {
						lpWB-=5;
						*(int*)(lpWB-4) = (*(int*)(lpWB-4)) >> (*(int*)(lpWB+1));
						nValue--;
					} else {
						fBefore = 0;
						nValue = 0;
						*lpWB++ = CALCODE_SHR+0x80;
					}
					break;
				case CALCODE_MORE+0x80:
					if( nValue >= 2 ) {
						lpWB-=5;
						*(int*)(lpWB-4) = ((*(int*)(lpWB-4)) >= (*(int*)(lpWB+1)));
						nValue--;
					} else {
						fBefore = 0;
						nValue = 0;
						*lpWB++ = CALCODE_MORE+0x80;
					}
					break;
				case CALCODE_BIG+0x80:
					if( nValue >= 2 ) {
						lpWB-=5;
						*(int*)(lpWB-4) = ((*(int*)(lpWB-4)) > (*(int*)(lpWB+1)));
						nValue--;
					} else {
						fBefore = 0;
						nValue = 0;
						*lpWB++ = CALCODE_BIG+0x80;
					}
					break;
				case CALCODE_LESS+0x80:
					if( nValue >= 2 ) {
						lpWB-=5;
						*(int*)(lpWB-4) = ((*(int*)(lpWB-4)) <= (*(int*)(lpWB+1)));
						nValue--;
					} else {
						fBefore = 0;
						nValue = 0;
						*lpWB++ = CALCODE_LESS+0x80;
					}
					break;
				case CALCODE_SMALL+0x80:
					if( nValue >= 2 ) {
						lpWB-=5;
						*(int*)(lpWB-4) = ((*(int*)(lpWB-4)) < (*(int*)(lpWB+1)));
						nValue--;
					} else {
						fBefore = 0;
						nValue = 0;
						*lpWB++ = CALCODE_SMALL+0x80;
					}
					break;
				case CALCODE_EQU+0x80:
					if( nValue >= 2 ) {
						lpWB-=5;
						*(int*)(lpWB-4) = ((*(int*)(lpWB-4)) == (*(int*)(lpWB+1)));
						nValue--;
					} else {
						fBefore = 0;
						nValue = 0;
						*lpWB++ = CALCODE_EQU+0x80;
					}
					break;
				case CALCODE_NEQU+0x80:
					if( nValue >= 2 ) {
						lpWB-=5;
						*(int*)(lpWB-4) = ((*(int*)(lpWB-4)) != (*(int*)(lpWB+1)));
						nValue--;
					} else {
						fBefore = 0;
						nValue = 0;
						*lpWB++ = CALCODE_NEQU+0x80;
					}
					break;
				case CALCODE_BITAND+0x80:
					if( nValue >= 2 ) {
						lpWB-=5;
						*(int*)(lpWB-4) = ((*(int*)(lpWB-4)) & (*(int*)(lpWB+1)));
						nValue--;
					} else {
						fBefore = 0;
						nValue = 0;
						*lpWB++ = CALCODE_BITAND+0x80;
					}
					break;
				case CALCODE_BITXOR+0x80:
					if( nValue >= 2 ) {
						lpWB-=5;
						*(int*)(lpWB-4) = ((*(int*)(lpWB-4)) ^ (*(int*)(lpWB+1)));
						nValue--;
					} else {
						fBefore = 0;
						nValue = 0;
						*lpWB++ = CALCODE_BITXOR+0x80;
					}
					break;
				case CALCODE_BITOR+0x80:
					if( nValue >= 2 ) {
						lpWB-=5;
						*(int*)(lpWB-4) = ((*(int*)(lpWB-4)) | (*(int*)(lpWB+1)));
						nValue--;
					} else {
						fBefore = 0;
						nValue = 0;
						*lpWB++ = CALCODE_BITOR+0x80;
					}
					break;
				case CALCODE_AND+0x80:
					if( nValue >= 2 ) {
						lpWB-=5;
						*(int*)(lpWB-4) = ((*(int*)(lpWB-4)) && (*(int*)(lpWB+1)));
						nValue--;
					} else {
						fBefore = 0;
						nValue = 0;
						*lpWB++ = CALCODE_AND+0x80;
					}
					break;
				case CALCODE_OR+0x80:
					if( nValue >= 2 ) {
						lpWB-=5;
						*(int*)(lpWB-4) = ((*(int*)(lpWB-4)) || (*(int*)(lpWB+1)));
						nValue--;
					} else {
						fBefore = 0;
						nValue = 0;
						*lpWB++ = CALCODE_OR+0x80;
					}
					break;
				case CALCODE_LET+0x80:
					fBefore = 0;
					nValue = 0;
					*lpWB++ = CALCODE_LET+0x80;
					break;
				case CALCODE_ADDRESS+0x80:
					if(( fBefore == 2) && (*(lpWB-5) == 2) ) {		// 1 �O�� reg ��
						f = TRUE;
						*(lpWB-5) = 1;							// 1 �O�𑦒l�ɂ���
						fBefore = 1;
						nValue = 1;
					} else if( fBefore == 0 ) {
						nValue = 0;
						*lpWB++ = CALCODE_ADDRESS+0x80;
					} else {
						ExpCalcError( "�A�h���X���Z�q���ُ�" );
					}
					break;
				case CALCODE_VALUE+0x80:
					nValue = 0;
					if( (fBefore == 1) && (*(lpWB-5) == 1) ) {		// 1 �O�����l��
						f = TRUE;
						*(lpWB-5) = 2;							// 1 �O�� reg �ɂ���
						fBefore = 2;
					} else {
						fBefore = 0;
						*lpWB++ = CALCODE_VALUE+0x80;
					}
					break;
			}
		}
		*lpWB = 0;
	}


//	if( nWarning == 1 ){
//		err_compiler(1);
//		printf( "calc end:" );
//		PrintCalc( lpRead );
//	}
	return;
}
//---------------------------------------------------------------------------
void CehckCalc1( UCHAR *lpSocNow )
{
//
//	check calc
//
//type
//	reg     = 2 �ȊO�̏ꍇ�ɂ� 1
//	value   = ���O�� reg �̏ꍇ�ɂ� 2�A����ȊO�̎��ɂ� reg �Ɠ���
//	address = ���O�����l�̏ꍇ�ɂ� error
//	���Z�q  = 2 �ȊO�̏ꍇ�ɂ� 0
//
	int n;

	lpCalcSp0 = CalcValue;
	lpCalcSp1 = CalcType;

	while( 1 ) {
		switch( *((UCHAR*)lpSocNow)++ ) {
			case 0:
				if( *lpCalcSp1 == -1 ) {
					ExpCalcError( "���W�X�^�[�̎w�肪�ُ�" );
				}
				return;
			case 1:								// ���l
				++lpCalcSp0;
				++lpCalcSp1;
				*lpCalcSp1 = -1;
				//*lpCalcSp0 = *(int*)(lpSocNow);
				(UCHAR*)lpSocNow+=4;
				break;
			case 2:								// reg
				n = 0;
				++lpCalcSp0;
				++lpCalcSp1;
				*lpCalcSp1 = n;
				*lpCalcSp0 = 0;
				(UCHAR*)lpSocNow+=4;
				break;
			case CALCODE_PLUS+0x80:
				*lpCalcSp1 = -1;
//				*lpCalcSp0 = *lpCalcSp0;
				break;
			case CALCODE_MINUS+0x80:
				*lpCalcSp1 = -1;
				*lpCalcSp0 = -(*lpCalcSp0);
				break;
			case CALCODE_NOT+0x80:
				*lpCalcSp1 = -1;
				*lpCalcSp0 = ~(*lpCalcSp0);
				break;
			case CALCODE_NEG+0x80:
				*lpCalcSp1 = -1;
				*lpCalcSp0 = !(*lpCalcSp0);
				break;
			case CALCODE_MUL+0x80:
				//*(lpCalcSp0-1) = ( *(lpCalcSp0-1) * (*lpCalcSp0));
				lpCalcSp0--;
				lpCalcSp1--;
				*lpCalcSp1 = -1;
				break;
			case CALCODE_DIV+0x80:
				//*(lpCalcSp0-1) = ( *(lpCalcSp0-1) / (*lpCalcSp0));
				lpCalcSp0--;
				lpCalcSp1--;
				*lpCalcSp1 = -1;
				break;
			case CALCODE_MOD+0x80:
				//*(lpCalcSp0-1) = ( *(lpCalcSp0-1) % (*lpCalcSp0));
				lpCalcSp0--;
				lpCalcSp1--;
				*lpCalcSp1 = -1;
				break;
			case CALCODE_ADD+0x80:
				//*(lpCalcSp0-1) = ( *(lpCalcSp0-1) + (*lpCalcSp0));
				lpCalcSp0--;
				lpCalcSp1--;
				*lpCalcSp1 = -1;
				break;
			case CALCODE_SUB+0x80:
				//*(lpCalcSp0-1) = ( *(lpCalcSp0-1) - (*lpCalcSp0));
				lpCalcSp0--;
				lpCalcSp1--;
				*lpCalcSp1 = -1;
				break;
			case CALCODE_SHL+0x80:
				//*(lpCalcSp0-1) = ( *(lpCalcSp0-1) << (*lpCalcSp0));
				lpCalcSp0--;
				lpCalcSp1--;
				*lpCalcSp1 = -1;
				break;
			case CALCODE_SHR+0x80:
				//*(lpCalcSp0-1) = ( *(lpCalcSp0-1) >> (*lpCalcSp0));
				lpCalcSp0--;
				lpCalcSp1--;
				*lpCalcSp1 = -1;
				break;
			case CALCODE_MORE+0x80:
				//*(lpCalcSp0-1) = ( *(lpCalcSp0-1) >= (*lpCalcSp0));
				lpCalcSp0--;
				lpCalcSp1--;
				*lpCalcSp1 = -1;
				break;
			case CALCODE_BIG+0x80:
				//*(lpCalcSp0-1) = ( *(lpCalcSp0-1) > (*lpCalcSp0));
				lpCalcSp0--;
				lpCalcSp1--;
				*lpCalcSp1 = -1;
				break;
			case CALCODE_LESS+0x80:
				//*(lpCalcSp0-1) = ( *(lpCalcSp0-1) <= (*lpCalcSp0));
				lpCalcSp0--;
				lpCalcSp1--;
				*lpCalcSp1 = -1;
				break;
			case CALCODE_SMALL+0x80:
				//*(lpCalcSp0-1) = ( *(lpCalcSp0-1) < (*lpCalcSp0));
				lpCalcSp0--;
				lpCalcSp1--;
				*lpCalcSp1 = -1;
				break;
			case CALCODE_EQU+0x80:
				//*(lpCalcSp0-1) = ( *(lpCalcSp0-1) == (*lpCalcSp0));
				lpCalcSp0--;
				lpCalcSp1--;
				*lpCalcSp1 = -1;
				break;
			case CALCODE_NEQU+0x80:
				//*(lpCalcSp0-1) = ( *(lpCalcSp0-1) != (*lpCalcSp0));
				lpCalcSp0--;
				lpCalcSp1--;
				*lpCalcSp1 = -1;
				break;
			case CALCODE_BITAND+0x80:
				//*(lpCalcSp0-1) = ( *(lpCalcSp0-1) & (*lpCalcSp0));
				lpCalcSp0--;
				lpCalcSp1--;
				*lpCalcSp1 = -1;
				break;
			case CALCODE_BITXOR+0x80:
				//*(lpCalcSp0-1) = ( *(lpCalcSp0-1) ^ (*lpCalcSp0));
				lpCalcSp0--;
				lpCalcSp1--;
				*lpCalcSp1 = -1;
				break;
			case CALCODE_BITOR+0x80:
				//*(lpCalcSp0-1) = ( *(lpCalcSp0-1) | (*lpCalcSp0));
				lpCalcSp0--;
				lpCalcSp1--;
				*lpCalcSp1 = -1;
				break;
			case CALCODE_AND+0x80:
				//*(lpCalcSp0-1) = ( *(lpCalcSp0-1) && (*lpCalcSp0));
				lpCalcSp0--;
				lpCalcSp1--;
				*lpCalcSp1 = -1;
				break;
			case CALCODE_OR+0x80:
				//*(lpCalcSp0-1) = ( *(lpCalcSp0-1) || (*lpCalcSp0));
				lpCalcSp0--;
				lpCalcSp1--;
				*lpCalcSp1 = -1;
				break;
			case CALCODE_LET+0x80:
				if( *(lpCalcSp1-1) == -1 ) {
					ExpCalcError( "��������ُ�" );
				}
				//REGDATA(*(lpCalcSp1-1)) = *lpCalcSp0;
				*(lpCalcSp0-1) = *lpCalcSp0;
				lpCalcSp0--;
				lpCalcSp1--;
				*lpCalcSp1 = -1;
				break;
			case CALCODE_ADDRESS+0x80:
				if( *lpCalcSp1 == -1 ) {
					ExpCalcError( "�A�h���X���Z�q���ُ�" );
				}
				*lpCalcSp0 = *lpCalcSp1;
				*lpCalcSp1 = -1;
				break;
			case CALCODE_VALUE+0x80:
				n = *lpCalcSp0;
				*lpCalcSp1 = n;
				*lpCalcSp0 = 0;
				break;
		}
	}
}
//---------------------------------------------------------------------------
void CehckCalc2( UCHAR *lpSocNow, int *type )
{
//
//	check calc
//
//type
//	reg     = 2 �ȊO�̏ꍇ�ɂ� 1
//	value   = ���O�� reg �̏ꍇ�ɂ� 2�A����ȊO�̎��ɂ� reg �Ɠ���
//	address = ���O�����l�̏ꍇ�ɂ� error
//	���Z�q  = 2 �ȊO�̏ꍇ�ɂ� 0
//
	int n;

	lpCalcSp0 = CalcValue;
	lpCalcSp1 = CalcType;

	while( 1 ) {
		switch( *((UCHAR*)lpSocNow)++ ) {
			case 0:
				*type = 1;
				if( *lpCalcSp1 == -1 ) {
					*type = 0;					// ���l�̏ꍇ
				}
				return;
			case 1:								// ���l
				++lpCalcSp0;
				++lpCalcSp1;
				*lpCalcSp1 = -1;
				//*lpCalcSp0 = *(int*)(lpSocNow);
				(UCHAR*)lpSocNow+=4;
				break;
			case 2:								// reg
				n = 0;
				++lpCalcSp0;
				++lpCalcSp1;
				*lpCalcSp1 = n;
				*lpCalcSp0 = 0;
				(UCHAR*)lpSocNow+=4;
				break;
			case CALCODE_PLUS+0x80:
				*lpCalcSp1 = -1;
//				*lpCalcSp0 = *lpCalcSp0;
				break;
			case CALCODE_MINUS+0x80:
				*lpCalcSp1 = -1;
				*lpCalcSp0 = -(*lpCalcSp0);
				break;
			case CALCODE_NOT+0x80:
				*lpCalcSp1 = -1;
				*lpCalcSp0 = ~(*lpCalcSp0);
				break;
			case CALCODE_NEG+0x80:
				*lpCalcSp1 = -1;
				*lpCalcSp0 = !(*lpCalcSp0);
				break;
			case CALCODE_MUL+0x80:
				//*(lpCalcSp0-1) = ( *(lpCalcSp0-1) * (*lpCalcSp0));
				lpCalcSp0--;
				lpCalcSp1--;
				*lpCalcSp1 = -1;
				break;
			case CALCODE_DIV+0x80:
				//*(lpCalcSp0-1) = ( *(lpCalcSp0-1) / (*lpCalcSp0));
				lpCalcSp0--;
				lpCalcSp1--;
				*lpCalcSp1 = -1;
				break;
			case CALCODE_MOD+0x80:
				//*(lpCalcSp0-1) = ( *(lpCalcSp0-1) % (*lpCalcSp0));
				lpCalcSp0--;
				lpCalcSp1--;
				*lpCalcSp1 = -1;
				break;
			case CALCODE_ADD+0x80:
				//*(lpCalcSp0-1) = ( *(lpCalcSp0-1) + (*lpCalcSp0));
				lpCalcSp0--;
				lpCalcSp1--;
				*lpCalcSp1 = -1;
				break;
			case CALCODE_SUB+0x80:
				//*(lpCalcSp0-1) = ( *(lpCalcSp0-1) - (*lpCalcSp0));
				lpCalcSp0--;
				lpCalcSp1--;
				*lpCalcSp1 = -1;
				break;
			case CALCODE_SHL+0x80:
				//*(lpCalcSp0-1) = ( *(lpCalcSp0-1) << (*lpCalcSp0));
				lpCalcSp0--;
				lpCalcSp1--;
				*lpCalcSp1 = -1;
				break;
			case CALCODE_SHR+0x80:
				//*(lpCalcSp0-1) = ( *(lpCalcSp0-1) >> (*lpCalcSp0));
				lpCalcSp0--;
				lpCalcSp1--;
				*lpCalcSp1 = -1;
				break;
			case CALCODE_MORE+0x80:
				//*(lpCalcSp0-1) = ( *(lpCalcSp0-1) >= (*lpCalcSp0));
				lpCalcSp0--;
				lpCalcSp1--;
				*lpCalcSp1 = -1;
				break;
			case CALCODE_BIG+0x80:
				//*(lpCalcSp0-1) = ( *(lpCalcSp0-1) > (*lpCalcSp0));
				lpCalcSp0--;
				lpCalcSp1--;
				*lpCalcSp1 = -1;
				break;
			case CALCODE_LESS+0x80:
				//*(lpCalcSp0-1) = ( *(lpCalcSp0-1) <= (*lpCalcSp0));
				lpCalcSp0--;
				lpCalcSp1--;
				*lpCalcSp1 = -1;
				break;
			case CALCODE_SMALL+0x80:
				//*(lpCalcSp0-1) = ( *(lpCalcSp0-1) < (*lpCalcSp0));
				lpCalcSp0--;
				lpCalcSp1--;
				*lpCalcSp1 = -1;
				break;
			case CALCODE_EQU+0x80:
				//*(lpCalcSp0-1) = ( *(lpCalcSp0-1) == (*lpCalcSp0));
				lpCalcSp0--;
				lpCalcSp1--;
				*lpCalcSp1 = -1;
				break;
			case CALCODE_NEQU+0x80:
				//*(lpCalcSp0-1) = ( *(lpCalcSp0-1) != (*lpCalcSp0));
				lpCalcSp0--;
				lpCalcSp1--;
				*lpCalcSp1 = -1;
				break;
			case CALCODE_BITAND+0x80:
				//*(lpCalcSp0-1) = ( *(lpCalcSp0-1) & (*lpCalcSp0));
				lpCalcSp0--;
				lpCalcSp1--;
				*lpCalcSp1 = -1;
				break;
			case CALCODE_BITXOR+0x80:
				//*(lpCalcSp0-1) = ( *(lpCalcSp0-1) ^ (*lpCalcSp0));
				lpCalcSp0--;
				lpCalcSp1--;
				*lpCalcSp1 = -1;
				break;
			case CALCODE_BITOR+0x80:
				//*(lpCalcSp0-1) = ( *(lpCalcSp0-1) | (*lpCalcSp0));
				lpCalcSp0--;
				lpCalcSp1--;
				*lpCalcSp1 = -1;
				break;
			case CALCODE_AND+0x80:
				//*(lpCalcSp0-1) = ( *(lpCalcSp0-1) && (*lpCalcSp0));
				lpCalcSp0--;
				lpCalcSp1--;
				*lpCalcSp1 = -1;
				break;
			case CALCODE_OR+0x80:
				//*(lpCalcSp0-1) = ( *(lpCalcSp0-1) || (*lpCalcSp0));
				lpCalcSp0--;
				lpCalcSp1--;
				*lpCalcSp1 = -1;
				break;
			case CALCODE_LET+0x80:
				if( *(lpCalcSp1-1) == -1 ) {
					ExpCalcError( "��������ُ�" );
				}
				//REGDATA(*(lpCalcSp1-1)) = *lpCalcSp0;
				*(lpCalcSp0-1) = *lpCalcSp0;
				lpCalcSp0--;
				lpCalcSp1--;
				*lpCalcSp1 = -1;
				break;
			case CALCODE_ADDRESS+0x80:
				if( *lpCalcSp1 == -1 ) {
					ExpCalcError( "�A�h���X���Z�q���ُ�" );
				}
				*lpCalcSp0 = *lpCalcSp1;
				*lpCalcSp1 = -1;
				break;
			case CALCODE_VALUE+0x80:
				n = *lpCalcSp0;
				*lpCalcSp1 = n;
				*lpCalcSp0 = 0;
				break;
		}
	}
}
//---------------------------------------------------------------------------
void WriteCalc0( UCHAR *lpSocNow )
{
//
//	write calc
//
	UCHAR c;

	do{
		c = *lpSocNow++;
		*lpOp++ = c;
		if( c == 1 || c == 2 ) {
			*(int*)(lpOp) = *(int*)(lpSocNow);
			lpSocNow+=4;
			lpOp+=4;
		}
	} while( c != 0 );
}
//---------------------------------------------------------------------------
void WriteCalc2( UCHAR *lpSocNow, int type )
{
//
//	write calc
//
	UCHAR c;

	*lpOp++ = (BYTE)type;

	do{
		c = *lpSocNow++;
		*lpOp++ = c;
		if( c == 1 || c == 2 ) {
			*(int*)(lpOp) = *(int*)(lpSocNow);
			lpSocNow+=4;
			lpOp+=4;
		}
	} while( c != 0 );
}
/*---------------------------------------------------------------------------
		preprocess message #
---------------------------------------------------------------------------*/
void CheckPreprocessMessage( UCHAR *lp0 )
{
	UCHAR *lp1;
	int c;

	// #line
	if( *(UINT*)(lp0+1) == (UINT)'l' + (UINT)'i'*0x100 + (UINT)'n'*0x10000 + (UINT)'e'*0x1000000 ) {
		lp0+=5;
		// get line num
		lp0 = SkipSp( lp0 );
		c = 0;
		if( *lp0 == '-' ) {
			c = 1;
			lp0++;
		}
		nLineSource = 0;
		while( *lp0 >= '0' && *lp0 <= '9' ) {
			nLineSource = nLineSource*10 + (*lp0++) - '0';
		}
		if( c == 1 ) {
			nLineSource = -nLineSource;
		}
		// get filename
		lp0 = SkipSp( lp0 );
		if( *lp0 == '"' ) {
			lp0++;
			lp1 = sourcefilename;
			while( *lp0 != 0 ) {
				// "
				if( *lp0 == '"' ) {
					break;
				// yenmark
				} else if( *lp0 == '\\') {
					lp0++;
					*lp1++=*lp0++;
				// kanji
				} else if( _ismbblead( *lp0 )) {
					*lp1++=*lp0++;
					if( _ismbbtrail( *lp0 )) {
						*lp1++=*lp0++;
					}
				// else
				} else {
					*lp1++=*lp0++;
				}
			}
			*lp1 = 0;
		}
	// #l2e
	} else if( ((*(UINT*)(lp0+1))&0xffffff) == (UINT)'l' + (UINT)'2'*0x100 + (UINT)'e'*0x10000 ) {
		fLineOrg = 0;
	// #l2
	} else if( ((*(UINT*)(lp0+1))&0xffff) == (UINT)'l' + (UINT)'2'*0x100 ) {
		fLineOrg = 1;
		lp0+=3;
		// get line num
		lp0 = SkipSp( lp0 );
		c = 0;
		if( *lp0 == '-' ) {
			c = 1;
			lp0++;
		}
		nLineOrg = 0;
		while( *lp0 >= '0' && *lp0 <= '9' ) {
			nLineOrg = nLineOrg*10 + (*lp0++) - '0';
		}
		if( c == 1 ) {
			nLineOrg = -nLineOrg;
		}
		// get filename
		lp0 = SkipSp( lp0 );
		if( *lp0 == '"' ) {
			lp0++;
			lp1 = orgfilename;
			while( *lp0 != 0 ) {
				// "
				if( *lp0 == '"' ) {
					break;
				// yenmark
				} else if( *lp0 == '\\') {
					lp0++;
					*lp1++=*lp0++;
				// kanji
				} else if( _ismbblead( *lp0 )) {
					*lp1++=*lp0++;
					if( _ismbbtrail( *lp0 )) {
						*lp1++=*lp0++;
					}
				// else
				} else {
					*lp1++=*lp0++;
				}
			}
			*lp1 = 0;
		}
	} else {
		err_compiler(1);
		printf( "compiler error:������ # ���L��܂�\n" );
		err_next_compiler();
//		err_exit_compiler();
	}
}
/*---------------------------------------------------------------------------
		sub
---------------------------------------------------------------------------*/
int SearchLabel( UCHAR *lp0, UCHAR *lp1 )
//
//	lp0 = str
//	lp1 = �Ō�̂P��
//
#if 1
{
	UCHAR *lp2,*lp3;

	lp2 = HashTable[GetHashKey( lp0 )];
	while( lp2!=NULL ) {
		if( *(lp2+HASH_TYPE) == 1 ) {				// check type
			lp2+=HASH_STR;
			lp3 = lp0;
			while( 1 ) {
				if( *lp3 != *lp2 ) {
					while( *lp2++ != 0 ){
					}
					break;
				}
				if( *lp3 == 0 ) {
					return TRUE;
				}
				lp3++;
				lp2++;
			}
		// } else {
		}
		lp2 = *(UCHAR**)(lp2);
	}
	return FALSE;
}
#else
{
	UCHAR *lp2,*lp3;

	lp2 = LabelBuffer0;
	while( lp2 < lp1 ) {
		if( *(lp2+HASH_TYPE) == 1 ) {				// check type
			lp2+=HASH_STR;
			lp3 = lp0;
			while( 1 ) {
				if( *lp3 != *lp2 ) {
					while( *lp2++ != 0 ){
					}
					break;
				}
				if( *lp3 == 0 ) {
					return TRUE;
				}
				lp3++;
				lp2++;
			}
		// } else {
		}
	}
	return FALSE;
}
#endif
/*---------------------------------------------------------------------------
		check
---------------------------------------------------------------------------*/
UCHAR* CheckLabel( UCHAR* lp0 )
{
//
//		label �̏���
//
	UCHAR *lp1,*lp2,*lp3,*lp4;
	int nKey;

	lp1 = SkipSp( lp0 );
	if( __iscsymf( *lp1 ) != 0 ) {
		lp2 = lp1;
		lp1++;
		while( __iscsym( *lp1 ) != 0 ) {
			lp1++;
		}
		if( *lp1 == ':' ) {
			// error check
			if( lpLB0 > &LabelBuffer0[MAX_LABEL0-MAX_LINEBUFF] ) {
				err_compiler(1);
				printf( "compiler error:���x���̗ʂ��������܂�\n" );
				err_next_compiler();
//				err_exit_compiler();
				return NULL;
			}
			lp4 = lpLB0;						// lp4 = search end address
			lpLB0 += 8;
			*(UCHAR**)(lpLB0) = lpOp-(UINT)OutputBuff;// address
			lpLB0 += 4;
			*lpLB0++ = 0x01;					// type 0x01
			lp3 = lpLB0;						// lp3 = search string
			while( lp2 < lp1 ) {
				*lpLB0++ = *lp2++;
			}
			*lpLB0++ = 0;
			if( SearchLabel( lp3,lp4 ) == TRUE ) {
				err_compiler(1);
				printf( "compiler error:���x���� 2 �o�^�ł�\n" );
				err_next_compiler();
//				err_exit_compiler();
				return NULL;
			}
			//*lpLB0 = 0;						// end mark
			lp1++;

			// Hash �ɓo�^
			nKey = GetHashKey( lp3 );
			if( HashTable[nKey]!=NULL ) {				// ���O�̓���hashkey�̃f�[�^�[�� ����̃f�[�^�[�ւ� pointer ��o�^
				*((UCHAR**)(HashTable[nKey]+HASH_NEXT)) = lp4;
			}
			(*(UCHAR**)lp4) = HashTable[nKey];			// ���O�� ����hashkey�̃f�[�^�[�ւ� pointer ��o�^
			(*(UCHAR**)(lp4+HASH_NEXT)) = NULL;			// ���� ����hashkey�̃f�[�^�[�ւ� pointer(NULL) ��o�^
			HashTable[nKey] = lp4;
			return lp1;
		}
	}
	return lp0;
}
//---------------------------------------------------------------------------
int CheckLineCompiler( UCHAR *lp0 )
{
//
//		check line
//
#if ASM_FLAG
	int n;

	_asm {
		xor		edx,edx

		; skip
		mov		ebx,[lp0]
loop00:	mov		al,[ebx]
		cmp		al,0
		je		next99
		inc		ebx
		cmp		al,' '
		je		loop00
		cmp		al,09h					; tab
		je		loop00
		dec		ebx

		mov		eax,[ebx]
		mov		ecx,eax
		and		eax,0ffffffh
		shr		ecx,24

		; r ( *
		mov		edx,9
		cmp		al,'r'
		je		next99
		cmp		al,'('
		je		next99
		cmp		al,'*'
		je		next99

		; ep0
		cmp		eax,'e' + 'p'*100h + '0'*10000h
		jne		next00
		mov		edx,4
		cmp		cl,' '
		je		next99
		cmp		cl,09h					; tab
		je		next99
		jmp		next98
next00:

		; ep1
		cmp		eax,'e' + 'p'*100h + '1'*10000h
		jne		next01
		mov		edx,5
		cmp		cl,' '
		je		next99
		cmp		cl,09h					; tab
		je		next99
		jmp		next98
next01:

		; ep2
		cmp		eax,'e' + 'p'*100h + '2'*10000h
		jne		next02
		mov		edx,6
		cmp		cl,' '
		je		next99
		cmp		cl,09h					; tab
		je		next99
		jmp		next98
next02:

		; ep3 ���W�X�^
		cmp		eax,'e' + 'p'*100h + '3'*10000h
		jne		next10
		mov		edx,10
		cmp		cl,' '
		je		next99
		cmp		cl,09h					; tab
		je		next99
		jmp		next98
next10:

		; ep4 �C�~�f�B�G�C�g
		cmp		eax,'e' + 'p'*100h + '4'*10000h
		jne		next11
		mov		edx,11
		cmp		cl,' '
		je		next99
		cmp		cl,09h					; tab
		je		next99
		jmp		next98
next11:

		; ep5 ���W�X�^/�C�~�f�B�G�C�g
		cmp		eax,'e' + 'p'*100h + '5'*10000h
		jne		next12
		mov		edx,12
		cmp		cl,' '
		je		next99
		cmp		cl,09h					; tab
		je		next99
		jmp		next98
next12:

		; ep6 �ȗ���
		cmp		eax,'e' + 'p'*100h + '6'*10000h
		jne		next13
		mov		edx,13
		cmp		cl,' '
		je		next99
		cmp		cl,09h					; tab
		je		next99
		jmp		next98
next13:

		; cce
		cmp		eax,'c' + 'c'*100h + 'e'*10000h
		jne		next07
		mov		edx,8
		cmp		cl,' '
		je		next99
		cmp		cl,09h					; tab
		je		next99
		jmp		next98
next07:

		; cci
		cmp		eax,'c' + 'c'*100h + 'i'*10000h
		jne		next06
		mov		edx,7
		cmp		cl,' '
		je		next99
		cmp		cl,09h					; tab
		je		next99
		jmp		next98
next06:

		mov		ecx,eax
		and		eax,000ffffh
		shr		ecx,16

		; dd
		cmp		eax,'d' + 'd'*100h
		jne		next03
		mov		edx,3
		cmp		cl,' '
		je		next99
		cmp		cl,09h					; tab
		je		next99
		jmp		next98
next03:

		; dw
		cmp		eax,'d' + 'w'*100h
		jne		next04
		mov		edx,2
		cmp		cl,' '
		je		next99
		cmp		cl,09h					; tab
		je		next99
		jmp		next98
next04:

		; db
		cmp		eax,'d' + 'b'*100h
		jne		next98
		mov		edx,1
		cmp		cl,' '
		je		next99
		cmp		cl,09h					; tab
		je		next99


next98:	mov		edx,100

next99:
		mov		[n],edx
	}

	return n;

#else
	UCHAR c;
	UINT d,b;
	while( 1 ){						// skip space
		c = *lp0;
		if( c == 0 ) {
			return 0;
		} else 	if( c != ' ' && c != '\t' ) {
			break;
		}
		lp0++;
	}


	d = *(UINT*)(lp0);
	b = (d>>24);
	d &= 0xffffff;
	if( d==(UINT)'r' ) {
		return 9;
	}
	if( d==(UINT)'(' ) {
		return 9;
	}
	if( d==(UINT)'*' ) {
		return 9;
	}
	if(( d == (UINT)'e' + (UINT)'p'*0x100 + (UINT)'0'*0x10000 ) && ( b == ' ' || b == '\t' )){
		return 4;
	}
	if(( d == (UINT)'e' + (UINT)'p'*0x100 + (UINT)'1'*0x10000 ) && ( b == ' ' || b == '\t' )){
		return 5;
	}
	if(( d == (UINT)'e' + (UINT)'p'*0x100 + (UINT)'2'*0x10000 ) && ( b == ' ' || b == '\t' )){
		return 6;
	}
	if(( d == (UINT)'c' + (UINT)'c'*0x100 + (UINT)'e'*0x10000 ) && ( b == ' ' || b == '\t' )){
		return 8;
	}
	if(( d == (UINT)'c' + (UINT)'c'*0x100 + (UINT)'i'*0x10000 ) && ( b == ' ' || b == '\t' )){
		return 7;
	}
	b = (d>>16);
	d &= 0xffff;
	if(( d == (UINT)'d' + (UINT)'d'*0x100 ) && ( b == ' ' || b == '\t' )){
		return 3;
	}
	if(( d == (UINT)'d' + (UINT)'w'*0x100 ) && ( b == ' ' || b == '\t' )){
		return 2;
	}
	if(( d == (UINT)'d' + (UINT)'b'*0x100 ) && ( b == ' ' || b == '\t' )){
		return 1;
	}
	return 100;
#endif
}
//---------------------------------------------------------------------------
//		get token
//---------------------------------------------------------------------------
UCHAR* GetStringToken( UCHAR *lp0, UCHAR **lpOp )
{
//
//		sting data ���E�����A�o�͂���
//
	UCHAR c;

	lp0++;
	while( 1 ) {
		c = *lp0;
		if( c == '"' ) {
			lp0++;
			return lp0;
		} else if( c == 0 ) {
			err_compiler(1);
			printf( "compiler error:������̍Ō�� \" ��������܂���\n" );
			err_next_compiler();
//			err_exit_compiler();
			return NULL;
		} else if( c == '\\' ) {
			lp0++;
			switch( c = *lp0 ) {
#if 0
				case 'a':						// �R���p�C���[�ւ́A"\" ���t������Ԃ̂܂œn��
				case 'b':
				case 'f':
				case 'n':
				case 'r':
				case 't':
				case 'v':
					*(*lpOp)++ = '\\';
					err_compiler(0);
					printf( "compiler warning:esc code( %d )\n", c );
					err_next_compiler();
					break;
#endif
				case '\'':						// \ ������āA��ɑ����L�������ɂ���
				case '"':
				case '\\':
				case '?':
				case ' ':
					*(*lpOp)++ = c;
					lp0++;
					break;
				default:						// ����ȊO�̏ꍇ�A\ ���o�͂��� warning
					*(*lpOp)++ = '\\';
					err_compiler(0);
					printf( "compiler warning:esc code( %d )\n", c );
					err_next_compiler();
					break;
			}
		} else if( _ismbblead( c )) {
			*(*lpOp)++ = c;
			lp0++;
			c = *lp0;
			if( _ismbbtrail( c )) {
				*(*lpOp)++ = c;
				lp0++;
			}
		} else {
			*(*lpOp)++ = c;
			lp0++;
		}
	}

}
//---------------------------------------------------------------------------
UCHAR* GetLineToken( UCHAR *lp0, UCHAR **lpOp )
{
//
//		line data ���E�����A�o�͂���
//
	UCHAR c;

	while( 1 ) {
		c = *lp0;
		if( c == ' ' || c=='\t' ) {
			*(*lpOp)++ = c;
			lp0++;
		} else if( c < ' ' ) {
			return lp0;
		} else if( c == 0 ) {
			err_compiler(1);
			printf( "compiler error:������̍Ō�� \" ��������܂���\n" );
			err_next_compiler();
//			err_exit_compiler();
			return NULL;
		} else if( c == '\\' ) {
			lp0++;
			switch( c = *lp0 ) {
#if 0
				case 'a':						// �R���p�C���[�ւ́A"\" ���t������Ԃ̂܂œn��
				case 'b':
				case 'f':
				case 'n':
				case 'r':
				case 't':
				case 'v':
					*(*lpOp)++ = '\\';
					err_compiler(0);
					printf( "compiler warning:esc code( %d )\n", c );
					err_next_compiler();
					break;
#endif
				case '\'':						// \ ������āA��ɑ����L�������ɂ���
				case '"':
				case '\\':
				case '?':
				case ' ':
					*(*lpOp)++ = c;
					lp0++;
					break;
				default:						// ����ȊO�̏ꍇ�A\ ���o�͂��� warning
					*(*lpOp)++ = '\\';
					err_compiler(0);
					printf( "compiler warning:esc code( %d )\n", c );
					err_next_compiler();
					break;
			}
		} else if( _ismbblead( c )) {
			*(*lpOp)++ = c;
			lp0++;
			c = *lp0;
			if( _ismbbtrail( c )) {
				*(*lpOp)++ = c;
				lp0++;
			}
		} else {
			*(*lpOp)++ = c;
			lp0++;
		}
	}
}
//---------------------------------------------------------------------------
UCHAR* GetNumToken( UCHAR *lp0, UCHAR *lp1 )
{
//
//		num data �̏E�� ( �o�͂͂��Ȃ� )
//
	UCHAR c;

	c = *lp0;
	while( c != 0 ) {
		if( _ismbblead( c )) {
			err_compiler(1);
			printf( "compiler error:���l�ɑS�p�����������Ă��܂�\n" );
			err_next_compiler();
//			err_exit_compiler();
			return NULL;
		} else if( c == ',' ) {
			break;
		} else {
			*lp1++ = c;
			lp0++;
		}
		c = *lp0;
	}
	*lp1 = 0;				// end
	return lp0;
}
//---------------------------------------------------------------------------
//		Set db,dw,dd,ep3,ep4,ep5,ep6
//---------------------------------------------------------------------------
void SetDB( UCHAR *lp0 )
{
//
//		db ������͂��āA�f�[�^�[���o��
//
	UCHAR buff0[MAX_LINEBUFF];
	UCHAR buff1[MAX_LINEBUFF];
	int n;

	lp0--;
	do{
		lp0++;
		lp0 = SkipSp( lp0 );				// skip space
		if( *lp0 == '"' ) {
			// string
			do{
				lp0 = GetStringToken( lp0,&lpOp );
				if( lp0==NULL ) return;
				lp0 = SkipSp( lp0 );				// skip space
			} while ( *lp0 == '"' );
		} else {
			// �萔
			lp0 = GetNumToken( lp0,buff0 );
			if( lp0== NULL ) return;
			if( ExpLabel( buff0,buff1 ) == 0 ) {
				n = Calc( buff1,1 );
				if( n < -128 || n >= 256 ) {
					err_compiler(1);
					printf( "compiler error:������ 8bit ����I�[�o�[�t���[���Ă��܂�\n" );
					err_next_compiler();
//					err_exit_compiler();
					return;
				}
				*lpOp++ = (UCHAR)(n&0xff);
			} else {
				RegistBuffer1( buff1,1 );		// �����ŉ����ł��Ȃ�����o�^����
				*lpOp++ = 0;
			}
			lp0 = SkipSp( lp0 );				// skip space
		}
	} while( *lp0 == ',' );
}
//---------------------------------------------------------------------------
void SetDW( UCHAR *lp0 )
{
//
//		dw ������͂��āA�f�[�^�[���o��
//
	UCHAR buff0[MAX_LINEBUFF];
	UCHAR buff1[MAX_LINEBUFF];
	int n;

	lp0--;
	do{
		lp0++;
		lp0 = SkipSp( lp0 );				// skip space
		// �萔
		lp0 = GetNumToken( lp0,buff0 );
		if( ExpLabel( buff0,buff1 ) == 0 ) {
			n = Calc( buff1,1 );
			if( n < -32768 || n >= 65536 ) {
				err_compiler(1);
				printf( "compiler error:������ 16bit ����I�[�o�[�t���[���Ă��܂�\n" );
				err_next_compiler();
//				err_exit_compiler();
				return;
			}
			*(unsigned short*)(lpOp) = (unsigned short)(n&0xffff);
			lpOp+=2;
		} else {
			RegistBuffer1( buff1,2 );		// �����ŉ����ł��Ȃ�����o�^����
			*(short*)(lpOp) = 0;
			lpOp+=2;
		}
		lp0 = SkipSp( lp0 );				// skip space
	} while( *lp0 == ',' );
}
//---------------------------------------------------------------------------
void SetDD( UCHAR *lp0 )
{
//
//		dw ������͂��āA�f�[�^�[���o��
//
	UCHAR buff0[MAX_LINEBUFF];
	UCHAR buff1[MAX_LINEBUFF];
	int n;

	lp0--;
	do{
		lp0++;
		lp0 = SkipSp( lp0 );				// skip space
		// �萔
		lp0 = GetNumToken( lp0,buff0 );
		if( ExpLabel( buff0,buff1 ) == 0 ) {
			n = Calc( buff1,1 );
			*(int*)(lpOp) = n;
			lpOp+=4;
		} else {
			RegistBuffer1( buff1,3 );		// �����ŉ����ł��Ȃ�����o�^����
			*(int*)(lpOp) = 0;
			lpOp+=4;
		}
		lp0 = SkipSp( lp0 );				// skip space
	} while( *lp0 == ',' );
}
//---------------------------------------------------------------------------
void SetEP3( UCHAR *lp0 )
{
//
//		ep3(���W�X�^r0-r127)
//
	UCHAR buff0[MAX_LINEBUFF];
	UCHAR buff1[MAX_LINEBUFF];
	int n;

	lp0 = SkipSp( lp0 );				// skip space
	// reg
	if(*lp0!='r'&&*lp0!='R') {
		err_compiler(1);
		printf( "compiler error:ep3��regster�w�肳��Ă��܂���\n" );
		err_next_compiler();
	}
	lp0++;
	// ���W�X�^�ԍ�
	lp0 = GetNumToken( lp0,buff0 );
	if( ExpLabel( buff0,buff1 ) == 0 ) {
		n = Calc( buff1,1 );
		if( n < 0 || n > 127 ) {
			err_compiler(1);
			printf( "compiler error:ep3��register�ԍ���r0-r127����͂ݏo�Ă��܂�\n" );
			err_next_compiler();
//			err_exit_compiler();
			return;
		}
		*(char*)(lpOp) = (char)n;
		lpOp++;
	} else {
		err_compiler(1);
		printf( "compiler error:ep3��register�ԍ���r0-r127����͂ݏo�Ă��܂�\n" );
		err_next_compiler();
//		err_exit_compiler();
		return;
	}
	lp0 = SkipSp( lp0 );				// skip space
}
//---------------------------------------------------------------------------
void SetEP4( UCHAR *lp0 )
{
//
//		ep4(���l-128�`+127)
//
	UCHAR buff0[MAX_LINEBUFF];
	UCHAR buff1[MAX_LINEBUFF];
	int n;

	lp0 = SkipSp( lp0 );				// skip space
	// ���l
	lp0 = GetNumToken( lp0,buff0 );
	if( ExpLabel( buff0,buff1 ) == 0 ) {
		n = Calc( buff1,1 );
		if( n < -128 || n > 127 ) {
			err_compiler(1);
			printf( "compiler error:ep4�̑��l��-128�`+127����͂ݏo�Ă��܂�\n" );
			err_next_compiler();
//			err_exit_compiler();
			return;
		}
		*(char*)(lpOp) = (char)n;
		lpOp++;
	} else {
		err_compiler(1);
		printf( "compiler error:ep4�̑��l��-128�`+127����͂ݏo�Ă��܂�\n" );
		err_next_compiler();
//		err_exit_compiler();
		return;
	}
	lp0 = SkipSp( lp0 );				// skip space
}
//---------------------------------------------------------------------------
void SetEP5( UCHAR *lp0 )
{
//
//		ep5(���W�X�^0-127�����l-128�`+127)
//
//	0: 0:imm / 1:reg
//	1:imm/reg
//
	UCHAR buff0[MAX_LINEBUFF];
	UCHAR buff1[MAX_LINEBUFF];
	int n;

	lp0 = SkipSp( lp0 );				// skip space
	if(*lp0!='r'&&*lp0!='R') {
		// ���l
		*(char*)(lpOp) = 0;
		lpOp++;
		lp0 = GetNumToken( lp0,buff0 );
		if( ExpLabel( buff0,buff1 ) == 0 ) {
			n = Calc( buff1,1 );
			if( n < -128 || n > 127 ) {
				err_compiler(1);
				printf( "compiler error:ep5�̑��l��-128�`+127����͂ݏo�Ă��܂�\n" );
				err_next_compiler();
//				err_exit_compiler();
				return;
			}
			*(char*)(lpOp) = (char)n;
			lpOp++;
		} else {
			err_compiler(1);
			printf( "compiler error:ep5�̑��l��-128�`+127����͂ݏo�Ă��܂�\n" );
			err_next_compiler();
//			err_exit_compiler();
			return;
		}
	} else {
		// ���W�X�^
		lp0++;
		// ���W�X�^�ԍ�
		*(char*)(lpOp) = 1;
		lpOp++;
		lp0 = GetNumToken( lp0,buff0 );
		if( ExpLabel( buff0,buff1 ) == 0 ) {
			n = Calc( buff1,1 );
			if( n < 0 || n > 127 ) {
				err_compiler(1);
				printf( "compiler error:ep3��register�ԍ���r0-r127����͂ݏo�Ă��܂�\n" );
				err_next_compiler();
//				err_exit_compiler();
				return;
			}
			*(char*)(lpOp) = (char)n;
			lpOp++;
		} else {
			err_compiler(1);
			printf( "compiler error:ep3��register�ԍ���r0-r127����͂ݏo�Ă��܂�\n" );
			err_next_compiler();
//			err_exit_compiler();
			return;
		}
	}
	lp0 = SkipSp( lp0 );				// skip space
}
//---------------------------------------------------------------------------
void SetEP6( UCHAR *lp0 )
{
//
//		ep6(�ȗ���)
//	0:reg
//	1: 0:== / 1:!= / 2:>= / 3:> / 4:< / 5:<=
//	2: 0:imm / 1:reg
//	3:imm/reg
//
	UCHAR buff0[MAX_LINEBUFF];
	UCHAR buff1[MAX_LINEBUFF];
	int n,f;

	//
	//	���W�X�^
	//
	lp0 = SkipSpK( lp0 );				// skip space & kakko
	// reg
	if(*lp0!='r'&&*lp0!='R') {
		err_compiler(1);
		printf( "compiler error:ep6��regster�w�肳��Ă��܂���\n" );
		err_next_compiler();
	}
	lp0++;
	// ���W�X�^�ԍ�
	n=0;
	f=0;
	while(*lp0>='0' && *lp0<='9' ) {
		f=1;
		n*=10;
		n+=(*lp0-'0');
		lp0++;
	}
	if( f==0 || n < 0 || n > 127 ) {
		err_compiler(1);
		printf( "compiler error:ep6�̍ŏ���register�ԍ��̎w�肪�ُ�ł�\n" );
		err_next_compiler();
//		err_exit_compiler();
		return;
	}
	*(char*)(lpOp) = (char)n;
	lpOp++;
	lp0 = SkipSpK( lp0 );				// skip space & kakko

	//
	// ��r��
	//
	if(*lp0=='=' && *(lp0+1)=='=' ) {
		lp0+=2;
		*(char*)(lpOp) = 0;
		lpOp++;
	} else if(*lp0=='!' && *(lp0+1)=='=' ) {
		lp0+=2;
		*(char*)(lpOp) = 1;
		lpOp++;
	} else if(*lp0=='>' && *(lp0+1)=='=' ) {
		lp0+=2;
		*(char*)(lpOp) = 2;
		lpOp++;
	} else if(*lp0=='<' && *(lp0+1)=='=' ) {
		lp0+=2;
		*(char*)(lpOp) = 5;
		lpOp++;
	} else if(*lp0=='>' && *(lp0+1)!='=' ) {
		lp0++;
		*(char*)(lpOp) = 3;
		lpOp++;
	} else if(*lp0=='<' && *(lp0+1)!='=' ) {
		lp0++;
		*(char*)(lpOp) = 4;
		lpOp++;
	} else {
		err_compiler(1);
		printf( "compiler error:ep6�Ŕ�r�����ُ�ł�\n" );
		err_next_compiler();
//		err_exit_compiler();
		return;
	}

	//
	// ���l/���W�X�^
	//
	lp0 = SkipSpK( lp0 );				// skip space & kakko
	if(*lp0!='r'&&*lp0!='R') {
		// ���l
		*(char*)(lpOp) = 0;
		lpOp++;
		lp0 = GetNumToken( lp0,buff0 );
		if( ExpLabel( buff0,buff1 ) == 0 ) {
			n = Calc( buff1,1 );
			if( n < -128 || n > 127 ) {
				err_compiler(1);
				printf( "compiler error:ep6�̑��l��-128�`+127����͂ݏo�Ă��܂�\n" );
				err_next_compiler();
//				err_exit_compiler();
				return;
			}
			*(char*)(lpOp) = (char)n;
			lpOp++;
		} else {
			err_compiler(1);
			printf( "compiler error:ep6�̑��l��-128�`+127����͂ݏo�Ă��܂�\n" );
			err_next_compiler();
//			err_exit_compiler();
			return;
		}
	} else {
		// ���W�X�^
		lp0++;
		// ���W�X�^�ԍ�
		*(char*)(lpOp) = 1;
		lpOp++;
		lp0 = GetNumToken( lp0,buff0 );
		if( ExpLabel( buff0,buff1 ) == 0 ) {
			n = Calc( buff1,1 );
			if( n < 0 || n > 127 ) {
				err_compiler(1);
				printf( "compiler error:ep6��register�ԍ���r0-r127����͂ݏo�Ă��܂�\n" );
				err_next_compiler();
//				err_exit_compiler();
				return;
			}
			*(char*)(lpOp) = (char)n;
			lpOp++;
		} else {
			err_compiler(1);
			printf( "compiler error:ep6��register�ԍ���r0-r127����͂ݏo�Ă��܂�\n" );
			err_next_compiler();
//			err_exit_compiler();
			return;
		}
	}
	lp0 = SkipSpK( lp0 );				// skip space & kakko
}
//---------------------------------------------------------------------------
//		label buffer2 �֘A
//---------------------------------------------------------------------------
//
//		table2 buffer
//
//00h     type
//          04h ep0
//          05h ep1
//01h-04h op address
//05h-08h size
//09h-0ch source file number
//0dh-10h fLineOrg
//11h-14h nLineOrg
//15h-    calc data ( string asciizero )
//   -    source file asciizero
//   -    orgfilename
//
void RegistBuffer2( UCHAR *lpStr,UCHAR* lpOp, int nOp,UCHAR type )
//
//	�����ŉ����ł��Ȃ�������U�o�^����
//
{
	UCHAR *lp0;

	if( lpLB2 > &LabelBuffer2[MAX_LABEL2-MAX_LINEBUFF] ) {
		err_compiler(1);
		printf( "compiler error:ep0,ep1�̎��̃��x���̑O���Q�Ƃ��������܂�\n" );
		err_next_compiler();
//		err_exit_compiler();
		return;
	}

	*lpLB2++ = type;
	*(UCHAR**)(lpLB2) = lpOp;
	lpLB2+=4;
	*(int*)(lpLB2) = nOp;
	lpLB2+=4;
	*(int*)(lpLB2) = nLineSource;
	lpLB2+=4;
	*(int*)(lpLB2) = fLineOrg;
	lpLB2+=4;
	*(int*)(lpLB2) = nLineOrg;
	lpLB2+=4;

	while( *lpStr != 0 ) {
		*lpLB2++ = *lpStr++;
	}
	*lpLB2++ = 0;

	lp0 = sourcefilename;
	while( *lp0 != 0 ) {
		*lpLB2++ = *lp0++;
	}
	*lpLB2++ = 0;

	if( fLineOrg != 0 ) {
		lp0 = orgfilename;
		while( *lp0 != 0 ) {
			*lpLB2++ = *lp0++;
		}
	}
	*lpLB2++ = 0;
}
//---------------------------------------------------------------------------
void SetLabelBuffer2( void )
{
//
//		LabelBuffer2 �̓��e���g���Alabel �� address �����肷��
//
	UCHAR *lpOpOrg,*lpOpRet,*lpExpCalc,*lpTmp,type;
	int nOp,f,t;

	// LabelBuffer1 �̓��e���g���Alabel �� address �����肷��
	lpOpRet = lpOp;
	lpTmp = LabelBuffer2;
	while( lpTmp < lpLB2 ) {
		type        = *lpTmp++;
		lpOp        = *(UCHAR**)(lpTmp);
		lpTmp+=4;
		nOp         = *(UINT*)(lpTmp);
		lpTmp+=4;
		nLineSource = *(int*)(lpTmp);
		lpTmp+=4;
		fLineOrg    = *(int*)(lpTmp);
		lpTmp+=4;
		nLineOrg    = *(int*)(lpTmp);
		lpTmp+=4;
		lpExpCalc   = lpTmp;
		while( *lpTmp++ != 0 ) {};			// skip calc data
		strcpy( sourcefilename, lpTmp );
		while( *lpTmp++ != 0 ) {};			// skip sourcefilename
		strcpy( orgfilename, lpTmp );
		while( *lpTmp++ != 0 ) {};			// skip orgfilename
		switch( type ) {
			case 4:							// ep0
//				lp0+=4;
				lpExpCalcErr = lpExpCalc;
				lpOpOrg = lpOp;
				f = ExpCalcMain( lpExpCalc );
				OptCalc0( ExpCalcData );
				CehckCalc0( ExpCalcData );
				WriteCalc0( ExpCalcData );
				if( (f==1) || (nOp!=lpOp-lpOpOrg) ) {					// ���x���̑O���Q�Ƃ̉\���̂���ꍇ
//					RegistBuffer2( lpExpCalcErr,lpOpOrg,lpOp-lpOpOrg,4 );
					ExpCalcError( "���x���A�������͎��ʎq���ُ�ł�" );
				}
				lpExpCalcErr = NULL;
				break;
			case 5:							// ep1
//				lp0+=4;
				lpExpCalcErr = lpExpCalc;
				lpOpOrg = lpOp;
				f = ExpCalcMain( lpExpCalc );
				OptCalc0( ExpCalcData );
				CehckCalc1( ExpCalcData );
				WriteCalc0( ExpCalcData );
				if( (f==1) || (nOp!=lpOp-lpOpOrg) ) {					// ���x���̑O���Q�Ƃ̉\���̂���ꍇ
//					RegistBuffer2( lpExpCalcErr,lpOpOrg,lpOp-lpOpOrg,5 );
					ExpCalcError( "���x���A�������͎��ʎq���ُ�ł�" );
				}
				lpExpCalcErr = NULL;
				break;
			case 6:							// ep2
//				lp0+=4;
				lpExpCalcErr = lpExpCalc;
				lpOpOrg = lpOp;
				f = ExpCalcMain( lpExpCalc );
				OptCalc0( ExpCalcData );
				CehckCalc2( ExpCalcData,&t );
				WriteCalc2( ExpCalcData,t );
				if( (f==1) || (nOp!=lpOp-lpOpOrg) ) {					// ���x���̑O���Q�Ƃ̉\���̂���ꍇ
//					RegistBuffer2( lpExpCalcErr,lpOpOrg,lpOp-lpOpOrg,5 );
					ExpCalcError( "���x���A�������͎��ʎq���ُ�ł�" );
				}
				lpExpCalcErr = NULL;
				break;
		}

	}
	lpOp = lpOpRet;
	return;
}
/*---------------------------------------------------------------------------
		main
---------------------------------------------------------------------------*/
void compiler_main( UCHAR *lpIn, UCHAR *lpOut,int fBtprs )
{
	UCHAR *lp0,*lpOpOrg,c;
	int size,f,t;

	//
	//		������
	//
	// alloc
	LabelBuffer0 = NULL;			// label data ������o�b�t�@�[
	LabelBuffer1 = NULL;			// �O���Q�Ƃ��������邽�߂̃o�b�t�@�[
	LabelBuffer2 = NULL;			// ep0,ep1,ep2 �̎��̑O���Q�Ƃ��������邽�߂̃o�b�t�@�[
	OutputBuff = NULL;				// �o��
	CalcMacBuff = NULL;				// cce �̓W�J�p�o�b�t�@�[
	lpP = NULL;
	// output
	lpOutErr = NULL;
	oFile=NULL;


	// alloc
	lpLB0 = LabelBuffer0 = malloc( MAX_LABEL0 );
	lpLB1 = LabelBuffer1 = malloc( MAX_LABEL1 );
	lpLB2 = LabelBuffer2 = malloc( MAX_LABEL2 );
	lpOp = OutputBuff = malloc( MAX_OUTPUT );
	lpCc = CalcMacBuff = malloc( MAX_CALCMACBUFF );

	// input
	if(( pFile = fopen( lpIn, "r")) == NULL) {
		printf( "err05:�v���v���Z�X�t�@�C�����J���܂���(%s)",lpIn );
		err_exit_compiler();
	}
	fseek( pFile, 0L, SEEK_END);				// size �E��
	psize = ftell( pFile );
	rewind( pFile );							// �擪�ɖ߂�
	lpPs = lpP = malloc( psize+1 );
	psize = fread( lpP, 1,psize, pFile );
	fclose( pFile );
	*(lpP+psize) = 0x1a;						// eof

	// init
	nErrComp = 0;
	nLineSource = 0;					// ���݂� source �ł̃��C��
	strcpy( sourcefilename,"" );		// source filename
	fLineOrg = 0;						// macro ���̓W�J�� source
	InitHash();
	lpCe = NULL;						// cce �̏o�͂̂��߂̃|�C���^�[

	//
	//	setup calc
	//
	ExpCalcPri[CALCODE_LS] = CALPRI_LS;
	ExpCalcLeft[CALCODE_LS] = CALLEFT_LS;
	ExpCalcPri[CALCODE_PLUS] = CALPRI_PLUS;
	ExpCalcLeft[CALCODE_PLUS] = CALLEFT_PLUS;
	ExpCalcPri[CALCODE_MINUS] = CALPRI_MINUS;
	ExpCalcLeft[CALCODE_MINUS] = CALLEFT_MINUS;
	ExpCalcPri[CALCODE_NOT] = CALPRI_NOT;
	ExpCalcLeft[CALCODE_NOT] = CALLEFT_NOT;
	ExpCalcPri[CALCODE_NEG] = CALPRI_NEG;
	ExpCalcLeft[CALCODE_NEG] = CALLEFT_NEG;
	ExpCalcPri[CALCODE_ADDRESS] = CALPRI_ADDRESS;
	ExpCalcLeft[CALCODE_ADDRESS] = CALLEFT_ADDRESS;
	ExpCalcPri[CALCODE_VALUE] = CALPRI_VALUE;
	ExpCalcLeft[CALCODE_VALUE] = CALLEFT_VALUE;
	ExpCalcPri[CALCODE_MUL] = CALPRI_MUL;
	ExpCalcLeft[CALCODE_MUL] = CALLEFT_MUL;
	ExpCalcPri[CALCODE_DIV] = CALPRI_DIV;
	ExpCalcLeft[CALCODE_DIV] = CALLEFT_DIV;
	ExpCalcPri[CALCODE_MOD] = CALPRI_MOD;
	ExpCalcLeft[CALCODE_MOD] = CALLEFT_MOD;
	ExpCalcPri[CALCODE_ADD] = CALPRI_ADD;
	ExpCalcLeft[CALCODE_ADD] = CALLEFT_ADD;
	ExpCalcPri[CALCODE_SUB] = CALPRI_SUB;
	ExpCalcLeft[CALCODE_SUB] = CALLEFT_SUB;
	ExpCalcPri[CALCODE_SHL] = CALPRI_SHL;
	ExpCalcLeft[CALCODE_SHL] = CALLEFT_SHL;
	ExpCalcPri[CALCODE_SHR] = CALPRI_SHR;
	ExpCalcLeft[CALCODE_SHR] = CALLEFT_SHR;
	ExpCalcPri[CALCODE_MORE] = CALPRI_MORE;
	ExpCalcLeft[CALCODE_MORE] = CALLEFT_MORE;
	ExpCalcPri[CALCODE_BIG] = CALPRI_BIG;
	ExpCalcLeft[CALCODE_BIG] = CALLEFT_BIG;
	ExpCalcPri[CALCODE_LESS] = CALPRI_LESS;
	ExpCalcLeft[CALCODE_LESS] = CALLEFT_LESS;
	ExpCalcPri[CALCODE_SMALL] = CALPRI_SMALL;
	ExpCalcLeft[CALCODE_SMALL] = CALLEFT_SMALL;
	ExpCalcPri[CALCODE_EQU] = CALPRI_EQU;
	ExpCalcLeft[CALCODE_EQU] = CALLEFT_EQU;
	ExpCalcPri[CALCODE_NEQU] = CALPRI_NEQU;
	ExpCalcLeft[CALCODE_NEQU] = CALLEFT_NEQU;
	ExpCalcPri[CALCODE_BITAND] = CALPRI_BITAND;
	ExpCalcLeft[CALCODE_BITAND] = CALLEFT_BITAND;
	ExpCalcPri[CALCODE_BITXOR] = CALPRI_BITXOR;
	ExpCalcLeft[CALCODE_BITXOR] = CALLEFT_BITXOR;
	ExpCalcPri[CALCODE_BITOR] = CALPRI_BITOR;
	ExpCalcLeft[CALCODE_BITOR] = CALLEFT_BITOR;
	ExpCalcPri[CALCODE_LE] = CALPRI_LE;
	ExpCalcLeft[CALCODE_LE] = CALLEFT_LE;
	ExpCalcPri[CALCODE_LET] = CALPRI_LET;
	ExpCalcLeft[CALCODE_LET] = CALLEFT_LET;


	// output
	nLineSource--;
	while( 1 ) {

		if( lpCe == NULL ) {
			// calcexp ���W�J���ł͂Ȃ��ꍇ ( �ʏ펞 )

			if( *lpPs==0x1a ) break;

			nLineSource++;							// ���݂� source �ł̃��C��

			// 0x0a(lf) �����̍s���΂�
			while( *lpPs==0x0a ) {
				lpPs++;
				nLineSource++;							// ���݂� source �ł̃��C��
			}

			// 1 �s�E��
			lp0 = LineBuffer;
			while( 1 ) {
				c = *lpPs++;
				if( c == 0x0a ) {						// lf
					break;
				} else if( c==0x1a ) {					// eof
					lpPs--;
					break;
				}
				*lp0++ = c;
				if( lp0 >= &LineBuffer[MAX_LINEBUFF] ) {
					err_compiler(1);
					printf( "compiler error:1 �s���������܂�\n" );
					err_next_compiler();
					err_exit_compiler();
				}
			}
			*lp0 = 0;
		} else {
			// calcexp ���W�J���̏ꍇ

			// 1 �s�E��
			lp0 = LineBuffer;
			while( 1 ) {
				if( *lpCe=='$' ) {						// '$' �̓V�t�gjis��1byte�ڂł�2byte�ڂł��Ȃ��B
					lpCe++;
					if( *lpCe=='s' ) {
						lpCe++;
						strcpy( lp0,CalcMacMain );
						lp0 += strlen( lp0 );
					} else if( *lpCe=='n' ) {
						lpCe++;
						_itoa( nCalcMac, lp0, 10 );
						lp0 += strlen( lp0 );
					} else {
						*lp0++='$';
					}
				} else if( *lpCe==0 ) {
					*lp0++=*lpCe++;
					break;
				} else {
					*lp0++=*lpCe++;
				}
			}
			if( lpCe==lpCc ) lpCe=NULL;
		}

		if( *LineBuffer == '#' ) {
			// preprocess
			CheckPreprocessMessage( LineBuffer );
		} else {
			// preprocess �ł͖����Ƃ�
			lp0 = CheckLabel( LineBuffer );				// label �̏���
			if( lp0!=NULL && *lp0 != 0 ) {
				lp0 = SkipSp( lp0 );
				switch( CheckLineCompiler( lp0 ) ){
					case 0:							// ��s
						break;
					case 1:							// db
						lp0+=2;
						SetDB( lp0 );
						break;
					case 2:							// dw
						lp0+=2;
						SetDW( lp0 );
						break;
					case 3:							// dd
						lp0+=2;
						SetDD( lp0 );
						break;
					case 4:							// ep0
						lp0+=4;
						lpExpCalcErr = lp0;
						lpOpOrg = lpOp;
						f = ExpCalcMain( lp0 );
						OptCalc0( ExpCalcData );
						CehckCalc0( ExpCalcData );
						WriteCalc0( ExpCalcData );
						if( f ) {					// ���x���̑O���Q�Ƃ̉\���̂���ꍇ
							RegistBuffer2( lpExpCalcErr,lpOpOrg,lpOp-lpOpOrg,4 );
						}
						lpExpCalcErr = NULL;
						break;
					case 5:							// ep1
						lp0+=4;
						lpExpCalcErr = lp0;
						lpOpOrg = lpOp;
						f = ExpCalcMain( lp0 );
						OptCalc0( ExpCalcData );
						CehckCalc1( ExpCalcData );
						WriteCalc0( ExpCalcData );
						if( f ) {					// ���x���̑O���Q�Ƃ̉\���̂���ꍇ
							RegistBuffer2( lpExpCalcErr,lpOpOrg,lpOp-lpOpOrg,5 );
						}
						lpExpCalcErr = NULL;
						break;
					case 6:							// ep2
						lp0+=4;
						lpExpCalcErr = lp0;
						lpOpOrg = lpOp;
						f = ExpCalcMain( lp0 );
						OptCalc0( ExpCalcData );
						CehckCalc2( ExpCalcData,&t );
						WriteCalc2( ExpCalcData,t );
						if( f ) {					// ���x���̑O���Q�Ƃ̉\���̂���ꍇ
							RegistBuffer2( lpExpCalcErr,lpOpOrg,lpOp-lpOpOrg,5 );
						}
						lpExpCalcErr = NULL;
						break;
					case 7:							// cci
						lpCc = CalcMacBuff;
						break;
					case 8:							// cce
						lp0+=3;
						lp0 = SkipSp( lp0 );		// skip space
						lp0 = GetLineToken( lp0,&lpCc );
						*lpCc++=0;
						break;
					case 9:							// �v�Z���̏ꍇ r ( *
						if( lpCc != CalcMacBuff ) {
							strcpy( CalcMacMain,lp0 );
							lpCe = CalcMacBuff;			// cce �̏o�͂̂��߂̃|�C���^�[
							nCalcMac++;
						} else {
							err_compiler(1);
							printf( "compiler error:�v�Z���̖��߂��o�^����Ă��܂��� cce �œo�^���Ă�������(%s)\n",lp0 );
							err_next_compiler();
//							err_exit_compiler();
						}
						break;
					case 10:						// ep3
						lp0+=4;
						SetEP3( lp0 );
						break;
					case 11:						// ep4
						lp0+=4;
						SetEP4( lp0 );
						break;
					case 12:						// ep5
						lp0+=4;
						SetEP5( lp0 );
						break;
					case 13:						// ep6
						lp0+=4;
						SetEP6( lp0 );
						break;
					default:						// else
						err_compiler(1);
						printf( "compiler error:�V���^�b�N�X�G���[�ł�(%s)\n",lp0 );
						err_next_compiler();
//						err_exit_compiler();
						break;
				}
			}
		}
	}
	free( lpP );
	lpP=NULL;


	// ���x���o�b�t�@�[�̓��e�𐸎Z����
	SetLabelBuffer1();
	SetLabelBuffer2();
	size = lpOp - OutputBuff;

	// create
	lpOutErr = NULL;
	if(( oFile = fopen( lpOut, "wb")) == NULL) {
		printf( "err04:�o�̓t�@�C�����J���܂���(%s)",lpOut );
		err_exit_compiler();
	}
	lpOutErr = lpOut;

	if( nErrComp!=0 ) {
		err_exit_compiler();
	}

	if( fBtprs == 1 ) {
		// btprs
		lpBtprs = OutputBuff;
		lpBtprsEnd = lpOp;
		BtprsMain( size );
	} else {
		// write
		fwrite( OutputBuff, 1, size, oFile );
	}
	fclose( oFile );
	oFile=NULL;
	// HashTable �̐��\��T��
//	PrintHash1();
	FreeMemCompiler();
}
