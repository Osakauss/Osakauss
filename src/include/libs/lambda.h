#ifndef __LAMBDA_HEADER__
#define __LAMBDA_HEADER__
#define lambda(return_type, function_body) ({ return_type __fn__ function_body __fn__; })
#endif