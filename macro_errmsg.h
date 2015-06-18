#pragma once

#define	BASE_CFG		0x00000100
#define BASE_PARSE		0x00000200


//---------------ERROR CODE----------------------------
/*#define CODE_LACK_OPTION_INAME		BASE_CFG + 1;
#define CODE_LACK_OPTION_ONAME		BASE_CFG + 2;
#define CODE_INAME_NOT_EXIST		BASE_CFG + 3;
#define CODE_ONAME_NOT_EXIST		BASE_CFG + 4;
*/

//---------------MACRO FOR ERROR MESSAGE-------------
#define MSG_LACK_OPTION_INAME	"iname lacked!"
#define MSG_LACK_OPTION_ONAME	"oname lacked!"
#define MSG_INAME_NOT_EXIST		"file specified by INAME  not exist!"
#define MSG_ONAME_NOT_EXIST		"invalid path specified by ONAME!"

#define	MSG_BRACE_UNMATCHED		"ill-matched brace!"
#define MSG_SYNTAX_ERROR		"syntax error!"
#define MSG_LEFT_BRACE_LACKED	"left brace lacked!"
#define MSG_FIELD_TYPE_LACKED	"field type lacked!"
#define MSG_WRONG_FIELD_CHARACTER	"wrong field character"
#define MSG_FIELD_UNSUITABLE		"field parsed error!"
#define MSG_TABLE_UNSUITABLE	"table parsed error!"
#define	MSG_WRONG_OP			"operator parsed error!"
#define	MSG_LENGTH_TOO_LARGE	"length value between Parentheses is too large"