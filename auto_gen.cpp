// auto_gen.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "io.h"
#include <fstream>

#include <string.h>
#include <string>
#include <map>
using namespace std;

#include "parse.h"
#include "macro_funs.h"
#include "generate.h"


///////////////////////////////////////////////////
void getName(string iname, string &fn)
{
	const char *p = iname.c_str();
	int		len = iname.length();
	const char *q = p + len - 1;
	while (q != p && *q != '\\' && *q != '/')	--q;

	char tmp[100] = {0};
	++q;
	int ch = int('.');
	int sz = strlen(q);
	memccpy(tmp, q, ch, sz);

	fn = tmp;
}

//-----------------------------------------------------

int _tmain(int argc, _TCHAR* argv[])
{
	
	//print
//	printf("%d", argc);
	
	if (0 == argc
		|| 1 == argc
		)//|| 2 == argc
	{
		const char usage[][100] = {
			"USAGE\t\tDESCRIPTION\n"
			"iname\t\tfile to be input for parsing.\n"
			, "oname\t\tfile path to be stored for auto generated files.\n"
			};
		int len = sizeof(usage) / 100;
		for (int i=0; i<len; i++)
			printf("%s", usage[i]);
	}
	else
	{
		map<string, string>	ls_tv;
		char	tmp[256];
		for (int i=1; i<argc; i++)
		{
			strcpy(tmp, argv[i]);
			char *p = strtok(tmp, "=");
			char *q = strtok(NULL, "=");

	//		printf("%s = %s\n", p, q);
			ls_tv.insert(make_pair(p, q));
		}

		//
		if (ls_tv.end() == ls_tv.find("iname"))
			PRINT_WITH_RETURN(MSG_LACK_OPTION_INAME, 1);
		else 
		{
			fstream f;
			
			f.open(ls_tv["iname"].c_str(),ios::in);
			if(!f)		//is it a true file? 
			PRINT_WITH_RETURN(MSG_INAME_NOT_EXIST, 1);
			else f.close();
		}

		if (ls_tv.end() == ls_tv.find("oname"))
			PRINT_WITH_RETURN(MSG_LACK_OPTION_ONAME, 1);
		else if (0 != _access(ls_tv["oname"].c_str(), 0))
			PRINT_WITH_RETURN(MSG_ONAME_NOT_EXIST, 1);

		//parse
		vector<s_tab>	ls_tab;
		if (!parse(ls_tv["iname"].c_str(), ls_tab))		return 1;

		//generate
		string	fn;
		getName(ls_tv["iname"], fn);

		char pathName[100] = {0};
		strcpy(pathName, ls_tv["oname"].c_str());
		char *p = pathName;
		rtrim(p, " /\\");

		strcat(pathName, "\\");		//for windows is '\', for linux is'/' 
		strcat(pathName, fn.c_str());
		rtrim(p, ".");
//		strcat(pathName, "_struct.h");
		generate_struct_h(ls_tab, pathName + string("_struct.h"));

		//
		generate_h(ls_tab, pathName+ string(".h"));
		generate_cpp(ls_tab, pathName+ string(".cpp"), fn+"h");
		generate_main_test_cpp(ls_tab, pathName+ string(".cpp"), fn+"h", fn+"_struct.h");
	
	}

	return 0;
}

