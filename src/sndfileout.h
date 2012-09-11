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

#ifndef SNDFILEOUT
#define SNDFILEOUT

#include <string>
#include <stdio.h>
#include <sndfile.h>
#include <stdint.h>

class CSndFileOut
{
  public:
    CSndFileOut(const std::string& filename);
    ~CSndFileOut();
    void Write(int16_t* buff, int frames);

  private:
    std::string m_filename;
    SNDFILE*    m_file;
};

#endif //SNDFILEOUT

