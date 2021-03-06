/* win32/timer.hpp
 *
 * Copyright (C) 2007 Antonio Di Monaco
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#pragma once
#ifndef __TIMER_HPP__
#define __TIMER_HPP__

#include <functional>

#include "win32/synch.hpp"
#include "win32/thread.hpp"

namespace Windows
{
  class Timer : public Handle<>
  {
    std::function< void () > _f;
    Synch::Event _cancelEvent;
    Thread _thread;

    Timer(const Timer &o);
    Timer &operator=(const Timer &o);
    Timer(Timer &&o);
    Timer &operator=(Timer &&o);

  public:
    explicit Timer(int timeout, std::function< void () > f);
    ~Timer();

    void cancel();
  };
}

#endif