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

#include "NetMD.h"

using namespace std;

void CNetMD::Open()
{
  m_netmd = netmd_init();
  if (!m_netmd)
    throw string("No NetMD devices detected");

	m_devh = netmd_open(m_netmd);
  if (!m_devh)
    throw string("Error opening netmd: ") + string(strerror(errno));

  unsigned char name[1024];
  if(!netmd_get_devname(m_devh, name, sizeof(name)))
		throw string("Could not get device name");

  m_name = (char*)name;

  int track = 0;
  int returnv;
  while(1)
  {
    char title[1024] = {0};
    int  titlesize = netmd_request_title(m_devh, track, title, sizeof(title));
    if (titlesize < 0)
    {
      m_tracks = track;
      break;
    }

    track++;
  }
}

float CNetMD::GetCurrentPlayPosition()
{
  if (!m_netmd || !m_devh)
    throw string("Device not opened");

  return netmd_get_playback_position(m_devh);
}

int CNetMD::GetCurrentTrack()
{
  if (!m_netmd || !m_devh)
    throw string("Device not opened");

  return netmd_get_current_track(m_devh);
}

void CNetMD::SetTrack(int track)
{
  if (!m_netmd || !m_devh)
    throw string("Device not opened");

  netmd_set_track(m_devh, track);
}

void CNetMD::Play()
{
  if (!m_netmd || !m_devh)
    throw string("Device not opened");

  netmd_play(m_devh);
}

void CNetMD::Stop()
{
  if (!m_netmd || !m_devh)
    throw string("Device not opened");

  netmd_stop(m_devh);
}

/*doesnt work right, DON'T USE!*/
float CNetMD::GetTrackLength(int track)
{
  struct netmd_track time;
  netmd_request_track_time(m_devh, track, &time);

  printf("%i %i %i\n", time.minute, time.second, time.tenth);

  return (float)time.minute * 60.0f + (float)time.second + (float)time.tenth / 10.0f;
}
