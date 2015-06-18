#pragma once

#include <fstream>
#include <algorithm>
#include <stdio.h>
#include <vector>
using namespace std;

#include "const.h"
#include "struct.h"
#include "parse.h"

void printContent(vector<s_tab> &ls_tab)
{
	//verify the result
	vector<s_tab>::iterator		itr;
	for (itr = ls_tab.begin()
	; itr != ls_tab.end()
	; itr ++)	
	{
		printf("table name: %s\n", itr->tab_name);
		printf("op flag:%d\n", itr->op_flag);

		vector<s_field>::iterator		itr_1;
		for (itr_1 = itr->ls_field.begin()
			; itr_1 != itr->ls_field.end()
			; itr_1 ++)
			printf("field name:%-20stype:%-5dprimary:%d\n", itr_1->name, itr_1->type, (itr_1->primary ? 1 : 0));
		printf("\n");
	}
}

string structName(const string &t_name)
{
	return ("s_" + t_name); 
}

void generate_struct_h(vector<s_tab> &ls_tab, string pathName)
{
//	printContent(ls_tab);	

	//open a file
	fstream	fs;
	fs.open (pathName.c_str(), ios_base::out|ios_base::trunc);	//write and trunc mode
	if (!fs.is_open())
	{
		//ERROR
		return ;
	}

	char	buf[LINE_MAX_LEN];
	fs << C_COMMENT_HEAD;
	fs << C_STRUCT_HEAD;

	//
	fs << C_CONN_STRUCT(C_CONN_TYPE);
	fs << "\n\n";

	vector<s_tab>::iterator		itr;
	vector<s_field>::iterator	itr_fd;
	for (itr = ls_tab.begin()
		; itr != ls_tab.end()
		; itr ++)	
	{
		fs << "typedef struct \n{\n";

		for (itr_fd = itr->ls_field.begin()
			; itr_fd != itr->ls_field.end()
			; itr_fd ++)
		{
			sprintf_s(buf, sizeof(buf), "\t%-15s%s", itr_fd->type.c_str(), itr_fd->name.c_str());
			fs << buf;
			if (0 != itr_fd->len)
				fs << "[" << itr_fd->len << "+2" << "]";
			fs << ";\n";
		}
		
		//hold the correspond struct name for use later
		itr->st_name = structName(itr->tab_name);
		fs << "}" << itr->st_name<<";\n\n";
	}

	fs.close();
}

//generate a instance with table's name
string varName(const string& t_name)
{
	char tmp[100] = {0};
	strcpy(tmp, t_name.c_str());
	char *q = tmp;
	ltrim(q, " _");
	rtrim(q, " _");

	char *p = NULL;
	++ q;
	char ch;
	while ('\0' != *q)
	{
		ch = *q;

		p = q +1 ;
		*q = '\0';
		strcat(tmp, p);
		q = p - 1;

		if ('_' == ch)	++ q;
	}

	return string(tmp);
}

string funcName(int opFlag, const string &t_name)
{
	//erase "_" from t_name and change the first char afterward to its upper form.
	char tmp[100] = {0};
	strcpy(tmp, t_name.c_str());
	char *q = tmp;
	ltrim(q, " _");
	rtrim(q, " _");

	if ('\0' != *q && 'a' <= *q && 'z' >= *q)		
		*q += ('A' - 'a');

	++ q;
	char *p = NULL;
	while ('\0' != *q)
	{
		if ('_' == *q)
		{
			p = q +1 ;
			*q = '\0';
			strcat(tmp, p);

			q = p - 1;
			if ('a' <= *q && 'z' >= *q)		*q += ('A' - 'a');
		}
		else  ++ q;
		
	}

	//connect strings
	switch (opFlag)
	{
		case op_INSERT:
			return string("insert") + tmp; 
		break;
		
		case op_SELECT:
			return string("select") + tmp; 
		break;

		case op_DELETE:
			return string("delete") + tmp; 
		break;
		
		default:	
			return string("undefined");
	}
}



void paraInfo(int opFlag, const string &t_name, map<int,s_realPara>	&ls_para)
{
	ls_para.clear();
	s_realPara	para;

	string stNm = structName(t_name);
	char *p = C_CONN_TYPE;

	switch (opFlag)
	{
	case op_INSERT:
	case op_DELETE:
		para.para_type = string("const ") + p + "*";
		para.para_inst = C_CONN_INST;
		para.para_chrc = 1;					//transfer by reference
		ls_para.insert(make_pair(0, para));

		para.clear();
		para.para_type = stNm;
		para.para_inst = varName(t_name);
		para.para_chrc = 1;					
		ls_para.insert(make_pair(1, para));
		break;

	case op_SELECT:
		para.para_type = string("const ") + p + "*";
		para.para_inst = C_CONN_INST;
		para.para_chrc = 1;					//transfer by reference
		ls_para.insert(make_pair(0, para));

		para.clear();
		para.para_type = stNm;
		para.para_inst = varName(t_name);
		para.para_chrc = 1;					
		ls_para.insert(make_pair(1, para));
		
		para.clear();
		para.para_type = "vector<" + stNm + ">";
		para.para_inst = "ls";
		para.para_chrc = 1;					
		ls_para.insert(make_pair(2, para));

		break;

	default:	
		;
	}
}

void generate_h(vector<s_tab> &ls_tab, string pathName)
{
	fstream	fs;
	fs.open (pathName.c_str(), ios_base::out|ios_base::trunc);	//write and trunc mode
	if (!fs.is_open())
	{
		//ERROR
		return ;
	}
		
	char	buf[LINE_MAX_LEN];
	fs << C_COMMENT_HEAD;
	fs << C_INCLUDE<<"\n\n";

	vector<s_tab>::iterator		itr;
	for (itr = ls_tab.begin()
		; itr != ls_tab.end()
		; itr ++)	
	{
		int	opFlag = itr->op_flag;

		map<int, s_realPara>	ls;
		if (opFlag && op_INSERT)
		{
			//init 
			paraInfo(op_INSERT, itr->tab_name, ls);		
			itr->id2func.insert(make_pair(op_INSERT, s_funInfo("int", funcName(op_INSERT,itr->tab_name), ls)));
		}
		
		if (opFlag && op_SELECT)
		{
			paraInfo(op_SELECT, itr->tab_name, ls);		
			itr->id2func.insert(make_pair(op_SELECT, s_funInfo("int", funcName(op_SELECT,itr->tab_name), ls)));
		}

		if (opFlag && op_DELETE)
		{
			paraInfo(op_DELETE, itr->tab_name, ls);		
			itr->id2func.insert(make_pair(op_DELETE, s_funInfo("int", funcName(op_DELETE,itr->tab_name), ls)));
		}

		//write to .h
		map<int, s_funInfo>::iterator	itr_is;
		for (itr_is = itr->id2func.begin()
			; itr_is != itr->id2func.end()
			; itr_is ++)
		{
			fs << C_FUNC_DESC << "\n";


			fs <<itr_is->second.return_type << " " <<itr_is->second.fun_name << "(";
		
			int sz = itr_is->second.ls_para.size();
			for (int i=0; i<sz; i++)
			{
				if (i > 0)	fs << ", ";

				fs << itr_is->second.ls_para[i].para_type.c_str()
					<< (itr_is->second.ls_para[i].para_chrc == 0 ? " " : "& ") 
				    << itr_is->second.ls_para[i].para_inst.c_str();
			}

			fs << ");\n\n";
		}


	}
	fs.close();
}


void generate_cpp(vector<s_tab> &ls_tab, string pathName, string fn)
{
	fstream	fs;
	fs.open (pathName.c_str(), ios_base::out|ios_base::trunc);	//write and trunc mode
	if (!fs.is_open())
	{
		//ERROR
		return ;
	}

	char	buf[LINE_MAX_LEN];
	fs << C_COMMENT_HEAD;
	sprintf_s(buf, sizeof(buf), C_IMPLEMENT_HEAD, fn.c_str());
	fs <<buf<<"\n\n";

	vector<s_tab>::iterator		itr;
	for (itr = ls_tab.begin()
		; itr != ls_tab.end()
		; itr ++)	
	{
		//write to .h
		map<int, s_funInfo>::iterator	itr_is;
		for (itr_is = itr->id2func.begin()
			; itr_is != itr->id2func.end()
			; itr_is ++)
		{
//			fs << C_FUNC_DESC << "\n";
			fs <<itr_is->second.return_type << " " <<itr_is->second.fun_name << "(";

			int sz = itr_is->second.ls_para.size();
			for (int i=0; i<sz; i++)
			{
				if (i > 0)	fs << ", ";

				fs << itr_is->second.ls_para[i].para_type.c_str()
					<< (itr_is->second.ls_para[i].para_chrc == 0 ? " " : "& ") 
					<< itr_is->second.ls_para[i].para_inst.c_str();
			}

			fs << "\n{ \n\treturn 0;\n}\n\n";
		}

	}

	fs.close();
}

void generate_main_test_cpp(vector<s_tab> &ls_tab, string pathName, string fn, string struct_h)
{
	fstream	fs;
	fs.open (pathName.c_str(), ios_base::out|ios_base::trunc);	//write and trunc mode
	if (!fs.is_open())
	{
		//ERROR
		return ;
	}

	char	buf[LINE_MAX_LEN];
	fs << C_COMMENT_HEAD;
	sprintf_s(buf, sizeof(buf), C_TEST_MAIN_HEAD, fn.c_str(), struct_h.c_str());
	fs <<buf<<"\n\n";


	//store the parameter list	
	string			para_type, para_inst;
	vector<string>	ls_para;
	char arr_para[LINE_MAX_LEN] = {0};

	vector<s_tab>::iterator		itr;
	for (itr = ls_tab.begin()
		; itr != ls_tab.end()
		; itr ++)	
	{
		map<int, s_funInfo>::iterator	itr_is;
		for (itr_is = itr->id2func.begin()
			; itr_is != itr->id2func.end()
			; itr_is ++)
		{
			memset(arr_para, 0, sizeof(arr_para));
			if (itr_is->first & op_INSERT)
				fs << "\t"<< "//插入一条记录\n";
			else if (itr_is->first & op_SELECT)
				fs << "\t"<< "//选择记录\n";			
			else if (itr_is->first & op_DELETE)
				fs << "\t"<< "//删除记录\n";

			//travel parameters
			int sz = itr_is->second.ls_para.size();
			for (int i=0; i<sz; i++)
			{
				para_type = itr_is->second.ls_para[i].para_type;
				para_inst = itr_is->second.ls_para[i].para_inst;   
				if (ls_para.end() == find(ls_para.begin(), ls_para.end(), para_inst))	
				{
					fs << "\t" << para_type.c_str() << " " << para_inst.c_str() << ";\n";
					ls_para.push_back(para_inst);		//only evaluate "para_inst", "para_type" added may be better!
				}

				//
				if (i > 0)	strcat(arr_para, ",");
				strcat(arr_para, para_inst.c_str());
			}

			fs << "\t" << "if (0 == " << itr_is->second.fun_name << "(" << C_CONN_INST << ", " << arr_para <<"))\n\t{\n\t\t//do something\n\t}\n";
			fs << "\telse { /* print error! */ return 1;} \n\n";
		}
	}

	//
	fs << "\nreturn 0; \n}";
}