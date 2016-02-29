/*
Copyright (c) <2006-2009> Sergio Fierens, Matias Saguir
Copyright (c) <2009> Jade Ferret

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#if !defined(AFX_GLEDTIMER_H__BE2CA8D8_D969_4FF5_BBFD_2257F47F732F__INCLUDED_)
#define AFX_GLEDTIMER_H__BE2CA8D8_D969_4FF5_BBFD_2257F47F732F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>


struct FpsInternalState;

class TrackerFps
{
private:
	FpsInternalState* state;

public:
	TrackerFps();
	virtual ~TrackerFps();

	HRESULT Start(DWORD fps);

	HRESULT Restart();

	HRESULT Wait();

	DWORD GetLastWait();
};

#endif // !defined(AFX_GLEDTIMER_H__BE2CA8D8_D969_4FF5_BBFD_2257F47F732F__INCLUDED_)
