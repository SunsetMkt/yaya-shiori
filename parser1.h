// 
// AYA version 5
//
// �\�����/���ԃR�[�h�̐������s���N���X�@CParser1
// written by umeici. 2004
// 
// �\����͎���CParser0�����x����CParser1::CheckExecutionCode�����s����܂��B
//

#ifndef	PARSER1H
#define	PARSER1H

//----

#if defined(WIN32) || defined(_WIN32_WCE)
# include "stdafx.h"
#endif

#include <vector>

#include "globaldef.h"

class	CVecint
{
public:
	std::vector<int> i_array;
};

class CAyaVM;
class CStatement;

//----

class	CParser1
{
private:
	CAyaVM &vm;

	CParser1(void);

public:
	CParser1(CAyaVM &vmr) : vm(vmr) {
		; //NOOP
	}

	char	CheckExecutionCode(const aya::string_t& dicfilename);
	char	CheckExecutionCode1(CStatement& st, const aya::string_t& dicfilename);

protected:
	char	CheckNomialCount(CStatement& st, const aya::string_t& dicfilename);
	char	CheckSubstSyntax(CStatement& st, const aya::string_t& dicfilename);
	char	CheckFeedbackOperatorPos(CStatement& st, const aya::string_t& dicfilename);
	char	SetFormulaType(CStatement& st, const aya::string_t& dicfilename);
	char	SetBreakJumpNo(const aya::string_t& dicfilename);
	char	CheckCaseSyntax(const aya::string_t& dicfilename);
	char	CheckIfSyntax(const aya::string_t& dicfilename);
	char	CheckElseSyntax(const aya::string_t& dicfilename);
	char	CheckForSyntax(const aya::string_t& dicfilename);
	char	CheckForeachSyntax(const aya::string_t& dicfilename);
	char	SetIfJumpNo(const aya::string_t& dicfilename);
	char	CheckFunctionArgument(CStatement& st, const aya::string_t& dicfilename);

	void	CompleteSetting(void);
};

//----

#endif
