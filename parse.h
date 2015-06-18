#pragma once

#include <fstream>
#include <string>
#include <vector>
using namespace std;

#include <assert.h>

#include "keyWords.h"
#include "macro_funs.h"
#include "macro_errmsg.h"
#include "struct.h"
#include "const.h"


int getType(int line_no, const char* pch, s_field &field)
{
	//
	const char	*lp = strstr(pch, "("), *rp = strstr(pch, ")") ;
	if ( (lp != NULL && rp == NULL)
		|| (lp == NULL && rp != NULL) )		return -1;

	//parse info for length and type
	char s_type[LINE_MAX_LEN] = {0};
	int val = 0;
	if (NULL == lp && NULL == rp)
	{
		strcpy(s_type, pch);
	}
	else
	{
		int len = int(rp - lp);
		if (LENGTH_MAX_BTW_PARENTHESE < len)		
			PRINT_WITH_RETURN_EX(("line no:%d\t%s\n", line_no, MSG_LENGTH_TOO_LARGE), -1);

		char buf[10] = {0};
		memcpy(buf, lp+1, len-1);
		val = atoi(buf);

		memcpy(s_type, pch, int(lp-pch));
	}
		
	//compare with current exist type
	if (0 == stricmp (s_type, c_str_int)				//int	INTERGER
		|| 0 == stricmp (s_type, c_str_integer))			
	{
		if (val > LENGTH_MAX_HOST_INT)					
			PRINT_WITH_RETURN_EX(("line no:%d\t%s\n", line_no, MSG_LENGTH_TOO_LARGE), -1);
		else  
		{
			field.len	= val;
			field.type	= c_host_int;
		}
	}
	else if (0 == stricmp (s_type, c_str_number))		// NUMBER
	{
		if (field.len > LENGTH_MAX_HOST_INT)	
		{
			field.len	= val;
			field.type	= c_host_char;
		}
		else	
		{
			field.len	= 0;
			field.type	= c_host_int;
		}
	}
	else if (0 == stricmp (s_type, c_str_char)		//VARCHAR2  CHAR
		|| 0 == stricmp (s_type, c_str_varchar2))		
	{
		if (val > LENGTH_MAX_STRING)				
			PRINT_WITH_RETURN_EX(("line no:%d\t%s\n", line_no, MSG_LENGTH_TOO_LARGE), -1);
		else 
		{
			field.len	= val;
			field.type	= c_host_char;
		}
	}
	else PRINT_WITH_RETURN_EX(("line no:%d\tunknow type:%s\n", line_no, s_type), -1);		//type not found, may should be added
	
	return 0;
}

bool isAnnotation(const char* buf)
{
	bool flag(false);
	const char *p = buf;
	while (*p != '\0' && (*p == ' ' || *p == '\t'))		++p;

	if ('\0' != p)
	{
		const char *q = p;
		++ q;

		if ('/' == *p && '/' == *q)	flag = true;
	}
	
	return flag;
}

void rtrim(char *&pch, const char* exclude)
{
	if (NULL == pch || NULL == exclude)		return;

	int len = strlen(pch);
	char *p = pch + len;
	-- p;
	while (p != pch && NULL != strstr(exclude, p))	
	{
		*p = '\0';	
		-- p;
	}
}

void ltrim(char *&pch, const char* exclude)
{
	if (NULL == pch || NULL == exclude)		return;

	pch = strrev (pch);		//reverse 
	rtrim(pch, exclude);
	pch = strrev (pch);
}

void trim(char *&pch)
{
	if (NULL == pch)	return;

	while (*pch != '\0' && (*pch == '\t' || *pch == ' '))		++ pch;
	

	rtrim(pch, " \t");
/*	if (NULL == pch)	return;
	int len = strlen(pch);
	char *p = pch + len;
	-- p;
	while (p != pch && (*p == '\t' || *p == ' '))	-- p;

	++p;
	*p = '\0';*/
}

int opFlag(const char* pch)
{
	if (0 == stricmp(pch, OP_INSERT))		return op_INSERT;
	else if (0 == stricmp(pch, OP_SELECT))	return op_SELECT;
	else if (0 == stricmp(pch, OP_DELETE))	return op_DELETE;
	else return -1;
}

int primaryField(const char* pch)
{
	if (0 == stricmp(pch, PRIMARY))	return 0;
	else  return -1;
}

bool findKeyWord(const char* buf, const char* keyword)
{
	return !(NULL == strstr (buf, keyword));
}

//


bool getPara(int line_no, const char* buf, s_field &field)
{
	bool	ret(true);

	char s[LINE_MAX_LEN];
	strcpy(s, buf);

	char *p = strtok(s, " \t");
	int i = -1;			//word ordinal
	while (NULL != p)
	{
		i ++;
		char *q = p;
//		strupr (q);		//transfer to upper string
		trim(q);		//remove ' 'or '\t'
		
		if (0 == i)		
		{
//			strupr(q);
			assert(0 == stricmp (q, FIELD));
		}
		else if (1 == i)	field.name = q;
		else if (2 == i)	
		{
			int type = getType(line_no, q, field);
			if (-1 == type)		PRINT_LINE_WITH_RETURN(line_no, MSG_FIELD_TYPE_LACKED, false);
		}
		else if (3 == i)	
		{
			int chrct = primaryField(q);
			if (-1 == chrct)		PRINT_LINE_WITH_RETURN(line_no, MSG_WRONG_FIELD_CHARACTER, false);
			else field.primary = true;
		}
		else	PRINT_LINE_WITH_RETURN(line_no, MSG_FIELD_UNSUITABLE, false);

		p = strtok(NULL, " \t");
	}

	return ret;	
}

bool tabName(int line_no, const char* buf, s_tab &tab)
{
	bool	ret(true);
	
	char s[LINE_MAX_LEN];
	strcpy(s, buf);

	char *p = strtok(s, " \t");
	int i = -1;			//hold the word ordinal
	while (NULL != p)
	{
		i ++;
		char *q = p;
		
		trim(q);		//remove ' 'or '\t'

		if (0 == i)		
		{
//			strupr(q);
			assert(0 == stricmp(q, TAB));
		}
		else if (1 == i)	tab.tab_name = q;
		else
		{
			PRINT_LINE_WITH_RETURN(line_no, MSG_TABLE_UNSUITABLE, false);
		}

		p = strtok(NULL, " \t");
	}

	return ret;
}

bool getOpFlag(int line_no, const char* buf, int &flag)
{
	bool	ret(true);
	char s[LINE_MAX_LEN];
	strcpy(s, buf);

	char *p = strtok(s, " \t");
	int i = -1;			//word ordinal
	while (NULL != p)
	{
		i ++;
		char *q = p;
//		strupr (q);		//transfer to upper string
		trim(q);		//remove ' 'or '\t'

		if (0 == i)		
		{
//			strupr(q);
			assert(0 == stricmp (q, OP));
		}
		else
		{
			int  f = opFlag(q);
			if (-1 == f)
				PRINT_WITH_RETURN_EX(("line no:%d:\'%s\'\t%s]", line_no, q, MSG_WRONG_OP), false);
			else  flag |= f;
		}

		p = strtok(NULL, " \t");
	}

	return ret;
}



//parse the input script file.
bool parse(const char* filename, vector<s_tab>	&ls_tab)
{
	fstream		fs;
	//open file
	fs.open (filename, ios_base::in); 
	if (!fs.is_open())
	{
		//ERROR
		return false;
	}

	//
	int		line_no = 0;			//count the lines for report error!
	char	buf[LINE_MAX_LEN];
	bool	begin = false;
	string	t_name;
	s_tab	tab;
	s_field	field;
	
	bool ret(true);
	while (!fs.eof())
	{
		fs.getline(buf, LINE_MAX_LEN);
		++ line_no;

		char *p = buf;
		trim(p);

		if (0 == strlen(p))			continue;
		else if (isAnnotation(p))	continue;

		char line[LINE_MAX_LEN] = {0};
		strcpy_s(line, sizeof(line), p);

		strupr (line);		//transfer to upper string

		if (findKeyWord(line, L_B))			
		{
			if (!begin)			
			{
				begin = true;
				tab.clear();
			}
			else	PRINT_WITH_RETURN_EX(("line no:%d\t%s", line_no, MSG_BRACE_UNMATCHED), false);
		}
		else if (findKeyWord(line, TAB))
		{
			if (!begin)		PRINT_LINE_WITH_RETURN(line_no, MSG_LEFT_BRACE_LACKED, false);	
			else if (!tabName(line_no, p, tab))		
			{
				ret = false;
				break;	
			}
		}
		else if (findKeyWord(line, FIELD))
		{
			if (!begin)		PRINT_LINE_WITH_RETURN(line_no, MSG_LEFT_BRACE_LACKED, false);	
			
			field.clear();
			if (!getPara(line_no, p, field))			
			{
				ret = false;
				break;	
			}
			else tab.ls_field.push_back(field);
		}
		else if (findKeyWord(line, OP))
		{
			if (!begin)		PRINT_LINE_WITH_RETURN(line_no, MSG_LEFT_BRACE_LACKED, false);	

			if (!getOpFlag(line_no, p, tab.op_flag))	
			{
				ret = false;
				break;
			}
		}
		else if (findKeyWord(line, R_B))
		{
			ls_tab.push_back(tab);
			begin = false;
		}
		else	PRINT_WITH_RETURN_EX(("line no:%d\t%s", line_no, MSG_SYNTAX_ERROR), false);
	}
	
	//
	return ret;

}


