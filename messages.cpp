﻿// 
// AYA version 5
//
// ログメッセージ
// written by umeici. 2004
// 

#if defined(WIN32) || defined(_WIN32_WCE)
# include "stdafx.h"
#endif

#include "messages.h"

#include "globaldef.h"
#include "wsex.h"
#include "misc.h"

//////////DEBUG/////////////////////////
#ifdef _WINDOWS
#ifdef _DEBUG
#include <crtdbg.h>
#define new new( _NORMAL_BLOCK, __FILE__, __LINE__)
#endif
#endif
////////////////////////////////////////

static void ClearMessageArrays()
{
	yayamsg::msgf.clear();
	yayamsg::msge.clear();
	yayamsg::msgw.clear();
	yayamsg::msgn.clear();
	yayamsg::msgj.clear();
}

/* -----------------------------------------------------------------------
 *  関数名  ：  yayamsg::IsEmpty
 *  機能概要：  エラーメッセージテーブルが空かどうかを判定します
 * -----------------------------------------------------------------------
 */
bool yayamsg::IsEmpty(void)
{
	return yayamsg::msgf.empty() && yayamsg::msge.empty() && yayamsg::msgw.empty();
}


/* -----------------------------------------------------------------------
 *  関数名  ：  yayamsg::LoadMessageFromTxt
 *  機能概要：  テキストファイルからエラーメッセージテーブルに読み込みます
 * -----------------------------------------------------------------------
 */
bool yayamsg::LoadMessageFromTxt(const yaya::string_t &file,char cset)
{
	FILE *fp = yaya::w_fopen(file.c_str(), L"rb");

	if (fp == NULL) {
		return false;
	}

	MessageArray *ptr = NULL;

	yaya::string_t line;

	ClearMessageArrays();

	while ( true )
	{
		if (yaya::ws_fgets(line, fp, cset, 0 /*no_enc*/, 1 /*skip_bom*/, 1 /*cut_heading_space*/) == yaya::WS_EOF) {
			break;
		}

		CutCrLf(line);

		if ( line.substr(0,3)==L"!!!" ) {
			yaya::string_t&type = line.substr(3);

			if ( type == L"msgf" ) {
				ptr = &msgf;
			}
			else if ( type == L"msge" ) {
				ptr = &msge;
			}
			else if ( type == L"msgw" ) {
				ptr = &msgw;
			}
			else if ( type == L"msgn" ) {
				ptr = &msgn;
			}
			else if ( type == L"msgj" ) {
				ptr = &msgj;
			}
			else {
				ptr = NULL;
			}
			continue;
		}
		
		if ( line.substr(0,2)==L"//" ) {
			continue;
		}
		
		if ( line.substr(0,1)==L"*" ) {
			if ( ptr ) {
				line=line.substr(1);
				yaya::ws_replace(line,L"\\n", L"\r\n");
				ptr->push_back(line);
			}
			continue;
		}
	}

	fclose(fp);

	return true;
}

/* -----------------------------------------------------------------------
 *  関数名  ：  yayamsg::GetTextFromTable
 *  機能概要：  エラーメッセージテーブルから文字列を抜き出します。
 * -----------------------------------------------------------------------
 */
const yaya::string_t yayamsg::GetTextFromTable(int mode,int id)
{
	yayamsg::MessageArray *ptr;
	yaya::char_t *emsg;

	if (mode == E_F) {
		ptr = &yayamsg::msgf;
		emsg = L"fatal F";
	}
	else if (mode == E_E) {
		ptr = &yayamsg::msge;
		emsg = L"error E";
	}
	else if (mode == E_W) {
		ptr = &yayamsg::msgw;
		emsg = L"warning W";
	}
	else if (mode == E_N) {
		ptr = &yayamsg::msgn;
		emsg = L"note N";
	}
	else {
		ptr = &yayamsg::msgj;
		emsg = L"//msg M";
	}

	if ( id < 0 || ptr->size() <= static_cast<size_t>(id) )//catch overflow
		return emsg+std::to_wstring(id)+L" : (please specify messagetxt)\r\n";
	else
		return (*ptr)[id];
}

namespace yayamsg {

	// fatal
	MessageArray msgf;

	// error
	MessageArray msge;

	// warning
	MessageArray msgw;

	// note
	MessageArray msgn;

	// other
	MessageArray msgj;

}
