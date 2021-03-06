/* sp/task.cpp
 *
 * Copyright (C) 2007 Antonio Di Monaco
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#include "sp/task.hpp"
#include "log/log.hpp"

LambdaTask::LambdaTask(DWORD i, std::function< void (DWORD) > f) : _i(i), _f(f)
{
  ::Log::Method(__FUNCSIG__,LOG_STRING("i = " << i));
}

LambdaTask::~LambdaTask()
{
  ::Log::Method(__FUNCSIG__,LOG_STRING("i = " << _i));
}

void LambdaTask::operator()(DWORD id)
{
  ::Log::Method m(__FUNCSIG__,LOG_STRING("id = " << id));
  _f(id);
}

void LambdaTask::timeoutF()
{
  ::Log::Method(__FUNCSIG__,LOG_STRING("i = " << _i));
}

void LambdaTask::cancelF()
{
  ::Log::Method(__FUNCSIG__,LOG_STRING("i = " << _i));
}
