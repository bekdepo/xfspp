/* win32/handle.hpp
 *
 * Copyright (C) 2007 Antonio Di Monaco
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#pragma once
#ifndef __HANDLE_HPP__
#define __HANDLE_HPP__

#include <windows.h>

#include "win32/error.hpp"

namespace Windows
{
  inline BOOL _stdcall nullCloseHandle(HANDLE) { return TRUE; }

  template< typename T = HANDLE, typename R = BOOL >
  class Handle
  {
    Handle(const Handle &o);
    Handle &operator=(const Handle &o);

    T _h;
    Error _lastError;
    R (_stdcall *_closeF)(T);

    void setLastError()
    {
      _lastError = ((_h == NULL) || (_h == INVALID_HANDLE_VALUE))? Error() : Error(ERROR_SUCCESS);
    }

  protected:
    void setHandle(T h)
    {
      _h = h;
      setLastError();
    }

    explicit Handle(T h = NULL,
                    R (_stdcall *closeF)(T) = CloseHandle) :
      _h(h),
      _closeF(closeF)
    {
      setLastError();
    }

  public:
    Handle(Handle &&o) :
      _h(o._h),
      _lastError(o._lastError)
      _closeF(o._closeF)
    {
      o._h = NULL;
      o._lastError = ERROR_SUCCESS;
    }
    ~Handle()
    {
      if (_h != NULL)
      {
        _closeF(_h);
        _h = NULL;
      }
    }

    T handle() const
    {
      return _h;
    }

    Error lastError() const
    {
      return _lastError;
    }

    Handle &operator=(Handle &&o)
    {
      _h = o._h;
      _lastError = o._lastError;
      _closeF = o._closeF;
      o._h = NULL;
      o._lastError = ERROR_SUCCESS;

      return *this;
    }
  };
}

#endif
