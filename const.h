#pragma once


//////////////////////////type from different DBRS or even self defined
const	char*	c_str_int		= "INT";
const	char*	c_str_integer	= "INTEGER";
const	char*	c_str_number	= "NUMBER";
const	char*	c_str_char		= "CHAR";
const	char*	c_str_varchar2	= "VARCHAR2";


///////////////////////////host type
const	char*		c_host_int = "int";
const	char*		c_host_char = "char";


////////////////////////////////basic struct for parse file
//operation flag for generating CPP
#define 	op_INSERT	(1<<0)
#define 	op_SELECT	(1<<1)
#define		op_DELETE	(1<<2)
////////////////////////////////
//field character
#define		chrc_PRIMARY	(1<<0)

////////////////////////////////////
#define		LINE_MAX_LEN				256
#define		LENGTH_MAX_BTW_PARENTHESE	4
#define		LENGTH_MAX_STRING			1000				//may be modified later
#define		LENGTH_MAX_NUMBER			20					//for oracle NUMBER type.may be modified later
#define		LENGTH_MAX_HOST_INT			11					//if > 11.int type (32bit)may not hold the db value, use char type instead


////////////////////////////////////file header
const char* C_COMMENT_HEAD = {"//	this file is generated by auto_gen Application\n"
					  "//	author:	wangwei\n"
					  "//	company: tdr\n\n"};

const char* C_STRUCT_HEAD = { "#pragma once\n" };

const char* C_INCLUDE = { "#pragma once\n"
						"#include <vector>\n"
						"using namespace std;\n"};
const char* C_TEST_MAIN_HEAD = { "#include \"%s\" \n" 
								"#include \"%s\" \n" 
								"int main() \n{\n" };		//a better method needed!

const char* C_TEST_MAIN_BODY = { "" };

const char* C_IMPLEMENT_HEAD = { "#include \"%s\" \n"};


const char* C_FUNC_DESC = {"//\t函数名：\n"
							"//\t功能：\n"
							"//\t参数说明：\n"
							"//\t返回值："};

//should be set through iname(option) file
//will be improved later
const char* C_CONN_INST = "dbConn";
#define C_CONN_TYPE	"s_dbConnection";
#define C_CONN_STRUCT(TYPE)	"typedef struct\n{"			\
	"\n\tchar	err[256]"		\
	"\n\t...	d*b\t\t//replaced with real DBRS connection handler\n}"##TYPE##";"
