#pragma once


//获取宏内参数的个数
#define Y_TUPLE_SIZE_II(X) Y_TUPLE_SIZE_I X
#define Y_TUPLE_SIZE_PREFIX__Y_TUPLE_SIZE_POSTFIX ,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,0		//该句用于处理 MPL_ARGS_SIZE()的情况
#define Y_TUPLE_SIZE_I(__p0,__p1,__p2,__p3,__p4,__p5,__p6,__p7,__p8,__p9,__p10,__p11,__p12,__p13,__p14,__p15,__p16,__p17,__p18,__p19,__p20,__p21,__p22,__p23,__p24,__p25,__p26,__p27,__p28,__p29,__p30,__p31,__n,...) __n
#define MPL_ARGS_SIZE(...) Y_TUPLE_SIZE_II((Y_TUPLE_SIZE_PREFIX_##__VA_ARGS__##_Y_TUPLE_SIZE_POSTFIX,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0))


