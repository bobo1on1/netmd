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

#ifndef CRECORDER
#define CRECORDER

#include <portaudio.h>
#include "condition.h"
#include <stdint.h>

class CRecorder
{
  public:
    CRecorder();
    ~CRecorder();

    void Setup();
    void Start();
    void Stop();
    void Wait();
    bool GetData(int16_t* buff, int frames);
    int  GetNrChannels() { return m_channels; }
    int  GetBuffFill();
    void WaitForBuffFill();
    void Purge();

  private:
    PaStream*  m_stream;          //handle for portaudio
    int        m_channels;
    CCondition m_callbacksignal;
    int16_t*   m_buff;
    int        m_buffsize;
    int        m_bufffill;

    static int PaStreamCallback(const void *input, void *output, unsigned long framecount,
			                          const PaStreamCallbackTimeInfo* timeinfo, PaStreamCallbackFlags statusflags,
				                        void *userdata);

    int                Callback(const void *input, void *output, unsigned long framecount,
			                          const PaStreamCallbackTimeInfo* timeinfo, PaStreamCallbackFlags statusflags,
				                        void *userdata);
};

#endif //CRECORDER
