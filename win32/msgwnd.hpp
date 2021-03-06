/* win32/msgwnd.hpp
 *
 * Copyright (C) 2007 Antonio Di Monaco
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#pragma once
#ifndef __MSG_WND_HPP__
#define __MSG_WND_HPP__

#include <functional>
#include <memory>

#include <windows.h>

#include "win32/thread.hpp"

namespace Windows
{
  class MsgWnd
  {
    MsgWnd(const MsgWnd &o);
    MsgWnd &operator=(const MsgWnd &o);

    bool _closing;
    HINSTANCE _hInstance;
    std::function< void (UINT, WPARAM, LPARAM) > _f;
    HWND _hWnd;
    std::unique_ptr< Thread > _t;

    static LRESULT CALLBACK wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

  public:
    explicit MsgWnd(HINSTANCE hInstance, std::function< void (UINT, WPARAM, LPARAM) > f);
    ~MsgWnd();

    HWND handle() const;
    
    void start();
    void close();
  };
}

#endif
