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

#include "sndfileout.h"

using namespace std;

CSndFileOut::CSndFileOut(const std::string& filename)
{
  m_filename = filename;

  SF_INFO sfinfo = {};
  sfinfo.frames = 1024;
  sfinfo.channels = 2;
  sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16 | SF_ENDIAN_FILE;
  sfinfo.samplerate = 44100;
  m_file = sf_open(m_filename.c_str(), SFM_WRITE, &sfinfo);

  if (!m_file)
  {
    throw string("Unable to open " + m_filename);
  }
}

CSndFileOut::~CSndFileOut()
{
  if (m_file)
  {
    sf_close(m_file);
  }
}

void CSndFileOut::Write(int16_t* buff, int frames)
{
  if (sf_writef_short(m_file, buff, frames) != frames)
  {
    throw string("ERROR:" + m_filename + " " + sf_strerror(m_file));
  }
}
