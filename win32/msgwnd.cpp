/* win32/msgwnd.cpp
 *
 * Copyright (C) 2007 Antonio Di Monaco
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#include "win32/msgwnd.hpp"
#include "win32/synch.hpp"
#include "win32/uuid.hpp"
#include "util/memory.hpp"
#include "util/methodscope.hpp"

using namespace Windows;

MsgWnd::MsgWnd(HINSTANCE hInstance, std::function< void (UINT, WPARAM, LPARAM) > f) :
  _closing(false),
  _hInstance(hInstance),
  _f(f),
  _hWnd(NULL)
{

}

MsgWnd::~MsgWnd()
{
  close();
}

HWND MsgWnd::handle() const
{
  return _hWnd;
}

void MsgWnd::start()
{
  Synch::Semaphore sem(0,1);

  _t.reset(new Thread([this, &sem] ()
    {
      WNDCLASSEX wx;

      std::wstring uuidStr(Uuid().value());

      clearMem(wx);
      wx.cbSize = sizeof(WNDCLASSEX);
      wx.lpfnWndProc = wndProc;
      wx.lpszClassName = uuidStr.c_str();
      wx.hInstance = _hInstance;

      if (!RegisterClassEx(&wx))
      {
        sem.unlock();
        return;
      }

      _hWnd = CreateWindowEx(0,uuidStr.c_str(),NULL,0,0,0,0,0,HWND_MESSAGE,NULL,_hInstance,
                             reinterpret_cast< LPVOID >(this));

      sem.unlock();

      MethodScope classScope([this, uuidStr] () { UnregisterClass(uuidStr.c_str(),_hInstance); });

      if (_hWnd)
      {
        MSG msg;
        BOOL bRet;
      
        while ((bRet = GetMessage(&msg,NULL,0,0)) != 0)
        {
          if (bRet == -1)
            return;
          else
          {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
          }
        }
      }
    }));

  sem.lock();
}

void MsgWnd::close()
{
  _closing = true;
  PostMessage(handle(),WM_CLOSE,0,0);
}

LRESULT CALLBACK MsgWnd::wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg)
  {
    case WM_NCCREATE:
      SetWindowLongPtr(hWnd,GWL_USERDATA,reinterpret_cast< LONG >(reinterpret_cast< LPCREATESTRUCT >(lParam)->lpCreateParams));
      break;

    case WM_CLOSE:
      {
        MsgWnd *me = reinterpret_cast< MsgWnd * >(GetWindowLongPtr(hWnd,GWLP_USERDATA));

        if (me && me->_closing)
        {
          DestroyWindow(hWnd);
          return 0;
        }
      }
      return 1;

    case WM_DESTROY:
      {
        MsgWnd *me = reinterpret_cast< MsgWnd * >(GetWindowLongPtr(hWnd,GWLP_USERDATA));

        if (me && me->_closing)
        {
          PostQuitMessage(0);
          return 0;
        }
      }
      return 1;

    default:
      {
        MsgWnd *me = reinterpret_cast< MsgWnd * >(GetWindowLongPtr(hWnd,GWLP_USERDATA));

        if (me && !me->_closing)
          me->_f(uMsg,wParam,lParam);
      }
      return 0;
  }

  return DefWindowProc(hWnd,uMsg,wParam,lParam);
}
