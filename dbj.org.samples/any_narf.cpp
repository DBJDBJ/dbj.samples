// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "stdafx.h"

DBJ_TEST_SPACE_OPEN(narf)

DBJ_TEST_UNIT("dbj narf")
{
 auto DBJ_UNUSED(naf) = 
    DBJ_TEST_ATOM(dbj::narf::make({ 1,2,3 }));
};

DBJ_TEST_SPACE_CLOSE(narf)
