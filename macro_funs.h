#pragma once

//---------------MACRO FOR FUNCTIONS-----------------
#define PRINT_WITH_NORETURN(MSG)	\
	do								\
	{								\
		printf MSG;					\
	} while(0)

#define PRINT_WITH_RETURN(MSG, RET)	\
	do								\
	{								\
		printf("%s\n", MSG);		\
		return RET;					\
	} while(0)

#define PRINT_WITH_RETURN_EX(MSG, RET)	\
	do								\
	{								\
	printf MSG;						\
	return RET;						\
	} while(0)

#define PRINT_LINE_WITH_RETURN(LN, MSG, RET)		\
	do										\
	{										\
		printf("line no:%d\t%s", LN, MSG);	\
		return RET;							\
	} while(0)



