#pragma once

#include <string>
#include <map>
using namespace std;

//info about field from "iname"(option)
typedef	struct
{
	string	name;		//field name
	bool	primary;
//	int		para_type;
	string	type;		
	int		len;				//eg:number(2)...varchar(2)

	void clear()
	{
		name.clear();
		primary		= false;
		type		= "undefined";
		len			= 0;
	}
}s_field;

typedef struct  
{
	string	para_type;
	string	para_inst;
	int		para_chrc;	// 0 : transfer by value  1 : transfer by reference

	void clear()
	{
		para_type.clear();
		para_inst.clear();
		para_chrc = 0;
	}
}s_realPara;

//info about generated function 
typedef struct s_funInfo
{
	string	return_type;
	string	fun_name;
	map<int,s_realPara>		ls_para;

	s_funInfo(string ret, string fun, map<int, s_realPara> &ls) : return_type(ret), fun_name(fun), ls_para(ls)	{}
};

//
typedef	struct
{
	string				tab_name;		//table name
	string				st_name;			//correspond struct name
	vector<s_field>		ls_field;
	int					op_flag;
	map<int, s_funInfo>	id2func;		


	void clear()
	{
		tab_name.clear();
		ls_field.clear();
		op_flag = 0;
	}
}s_tab;


