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

#ifndef NETMD
#define NETMD

#include <string>

extern "C"
{
  #include "libnetmd.h"
}

class CNetMD
{
  public:
    void               Open();
    int                GetNrTracks() { return m_tracks; }
    const std::string& GetName()     { return m_name; }
    float              GetCurrentPlayPosition();
    int                GetCurrentTrack();
    float              GetTrackLength(int track);
    void               SetTrack(int track);
    void               Play();
    void               Stop();

  private:
    struct usb_device* m_netmd;
    usb_dev_handle*    m_devh;

    int                m_tracks;
    std::string        m_name;

};

#endif //NETMD
