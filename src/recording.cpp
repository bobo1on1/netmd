/*
 * main.cc
 * Copyright (C) bob 2009 <bob@>
 * 
 * netmd is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * netmd is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <string>
#include <string.h>
#include <stdlib.h>

#include "recording.h"
#include "lock.h"

using namespace std;
CRecorder::CRecorder()
{
  Pa_Initialize();
  m_buff = NULL;
  m_buffsize = 0;
  m_bufffill = 0;
  m_channels = 2;
}

CRecorder::~CRecorder()
{
  Pa_Terminate();
  free(m_buff);
}

void CRecorder::Setup()
{
  int returnv;

  returnv = Pa_OpenDefaultStream(&m_stream, m_channels, 0, paInt16, 44100, 1024, PaStreamCallback, this);
  if (returnv != paNoError)
    throw string(Pa_GetErrorText(returnv));
}

void CRecorder::Start()
{
  int returnv;
  
  m_bufffill = 0;

  returnv = Pa_StartStream(m_stream);
  if (returnv != paNoError)
    throw string(Pa_GetErrorText(returnv));
}

void CRecorder::Stop()
{
  int returnv;

  returnv = Pa_AbortStream(m_stream);
  if (returnv != paNoError)
    throw string(Pa_GetErrorText(returnv));
}

void CRecorder::Wait()
{
  CLock lock(m_callbacksignal);
  m_callbacksignal.Wait();
}

int CRecorder::GetBuffFill()
{
  CLock lock(m_callbacksignal);
  return m_bufffill;
}

void CRecorder::WaitForBuffFill()
{
  CLock lock(m_callbacksignal);
  while (!m_bufffill)
    m_callbacksignal.Wait();
}

void CRecorder::Purge()
{
  CLock lock(m_callbacksignal);
  m_bufffill = 0;
}

bool CRecorder::GetData(int16_t* buff, int frames)
{
  CLock lock(m_callbacksignal);

  if (m_bufffill < frames)
    return false;

  memcpy(buff, m_buff, frames * m_channels * sizeof(int16_t));
  memmove(m_buff, m_buff + frames * m_channels, (m_bufffill - frames) * m_channels * sizeof(int16_t));
  m_bufffill -= frames;

  return true;
}

int CRecorder::PaStreamCallback(const void *input, void *output, unsigned long framecount,
			                          const PaStreamCallbackTimeInfo* timeinfo, PaStreamCallbackFlags statusflags,
				                        void *userdata)
{
  CRecorder* recorder = (CRecorder*)userdata;

  return recorder->Callback(input, output, framecount, timeinfo, statusflags, userdata);

}

int CRecorder::Callback(const void *input, void *output, unsigned long framecount,
			                          const PaStreamCallbackTimeInfo* timeinfo, PaStreamCallbackFlags statusflags,
				                        void *userdata)
{
  CLock lock(m_callbacksignal);

  if (m_buffsize - m_bufffill < framecount)
  {
    m_buffsize = (m_bufffill + framecount) * 2;
    m_buff = (int16_t*)realloc(m_buff, m_buffsize * m_channels * sizeof(int16_t));
    printf("\nResized buffer to hold %i frames\n", m_buffsize);
  }

  memcpy(m_buff + m_bufffill * m_channels, input, framecount * m_channels * sizeof(int16_t));
  m_bufffill += framecount;

  //printf("%i in buffer\n", m_bufffill);

  m_callbacksignal.Signal();

  return paContinue;
}

