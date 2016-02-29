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

// Fps.cpp: implementation of the Fps class.
//
//////////////////////////////////////////////////////////////////////

// ‘§±‡“ÎÕ∑
#include "stdafx.h"
#include "TrackerFPS.h"
#include "TrackerReturnCode.h"

#include <stdio.h>

struct FpsInternalState{
	DWORD fps;
	bool enabled; //should we wait? 
	bool performanceCounter;
	
	DWORD minFrameTime; //no less than minFrameTime milliseconds per frame.
	DWORD nextWaiting;
	DWORD lastTickCount;
	DWORD tickCount;
	DWORD lastTickDifference;
	
	LARGE_INTEGER frequency;
	LARGE_INTEGER lastperformanceCount;
	LARGE_INTEGER performanceCount;
	LONGLONG nextCountWait;
	LONGLONG neededStep;
	LONGLONG lastPerformanceDifference; 
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TrackerFps::TrackerFps()
{
	state = NULL;
}

TrackerFps::~TrackerFps()
{
	delete state;
}


HRESULT TrackerFps::Restart(){
	if(state==NULL)
		return ReturnCode::FPS_NOT_INITIALIZED;
	return Start(state->fps);
}

HRESULT TrackerFps::Start(DWORD fps){
	delete state;
	state = new FpsInternalState();
	if(state==NULL)
		return ReturnCode::OUT_OF_MEM;
	state->fps = fps;
	state->enabled = (fps!=0);
		if(QueryPerformanceFrequency(&(state->frequency))){
			if(QueryPerformanceCounter(&(state->performanceCount))){
				state->lastperformanceCount = state->performanceCount;
				state->performanceCounter = true;		
				state->nextCountWait = state->neededStep = (state->enabled)?((state->frequency.QuadPart) / fps):0;
				return ReturnCode::OK;
			}
		}
		state->performanceCounter = false;
		state->nextWaiting = state->minFrameTime = (state->enabled)?1000 / fps:0; //tenth of milliseconds by frame;
		state->lastTickCount = state->tickCount = ::GetTickCount();
	return ReturnCode::OK;
}

HRESULT TrackerFps::Wait(){
	if(state==NULL)
		return ReturnCode::FPS_NOT_INITIALIZED;
	if(!state->enabled)
		state->nextCountWait = 0;
	if(state->performanceCounter){
		state->lastperformanceCount = state->performanceCount;
		do{
			QueryPerformanceCounter(&(state->performanceCount));
			state->lastPerformanceDifference = state->performanceCount.QuadPart 
			- state->lastperformanceCount.QuadPart;
		}while(	state->lastPerformanceDifference < state->nextCountWait);
		state->nextCountWait = state->neededStep + state->nextCountWait - state->lastPerformanceDifference; 
		if(state->nextCountWait < 0)
			state->nextCountWait = 0;
	}else{
		if(!state->enabled)
			state->nextWaiting = 0;
		state->lastTickCount = state->tickCount;
		do{
			state->tickCount = ::GetTickCount();
			state->lastTickDifference = state->tickCount - state->lastTickCount;
		}while(	state->lastTickDifference < state->nextWaiting);
		state->nextWaiting = (state->lastTickDifference> state->minFrameTime + state->nextWaiting)
			?0: state->minFrameTime + state->nextWaiting - state->lastTickDifference; 
	}
	return ReturnCode::OK;
}

DWORD TrackerFps::GetLastWait(){
	if(state->performanceCounter){
		return DWORD((state->lastPerformanceDifference*1000000)/state->frequency.QuadPart);
	}else{
		return state->lastTickDifference*1000;
	}
}
