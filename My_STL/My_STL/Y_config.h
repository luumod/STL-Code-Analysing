#ifndef Y_CONFIG_H_
#define Y_CONFIG_H_
#include <iostream>
/*
配置文件
*/


#define _YLH_BEGIN namespace ylh {
#define _YLH_END }
#define USING_YLH using namespace ylh;
#define VECTOR_THROW_BAD_ALLOC std::cerr<<"Bad alloc!\n"; exit(0);
#define _TRY try 
#define _CATCH(info) catch(info)
#define NULL_vector "Null Vector!\n"

#define NULL_VECTOR 0
#define EXIT(info) exit(info)


#endif // !Y_CONFIG_H_
