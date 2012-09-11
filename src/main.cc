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

#include <iostream>
#include <portaudio.h>
#include <stdio.h>
#include <signal.h>

extern "C"
{
  #include "libnetmd.h"
}

#include "NetMD.h"
#include "recording.h"
#include "sndfileout.h"

using namespace std;

volatile bool g_stop = false;
void SignalHandler(int signal);
void record();

int main()
{
  signal(SIGTERM, SignalHandler);
  signal(SIGINT,  SignalHandler);

  try
  {
    record();
  }
  catch (string error)
  {
    printf("ERROR: %s\n", error.c_str());
    return 1;
  }

  return 0;
}

void record()
{
  CNetMD netmd;
  netmd.Open();

  CRecorder recorder;
  recorder.Setup();
  recorder.Start();

  printf("Name: %s\n", netmd.GetName().c_str());
  printf("Tracks: %i\n", netmd.GetNrTracks());

  for (int i = 0; i < netmd.GetNrTracks() && !g_stop; i++)
  {
    char filename[1024];
    sprintf(filename, "Track%03i.wav", i + 1);

    CSndFileOut outfile(filename);

    int currenttrack = i + 1;
    printf("Recording track %i\n", currenttrack);

    netmd.SetTrack(currenttrack);
    netmd.Play();

    printf("Waiting for device to start\n");
    float position;
    do
    {
      position = netmd.GetCurrentPlayPosition();
      recorder.Purge();
    }
    while (!g_stop && position < 0.001);
    printf("Device started at position %f\n", position);

    float prevtime = 0;
    float currenttime = 0;
    int   displaycount = 0;
    while(!g_stop && currenttime >= prevtime && netmd.GetCurrentTrack() == i)
    {
      prevtime = currenttime;
      currenttime = netmd.GetCurrentPlayPosition();

      displaycount = 0;
      printf("\rTrack %i, Position: %f", currenttrack, currenttime);
      fflush(stdout);

      recorder.WaitForBuffFill();

      int16_t buff[1024 * recorder.GetNrChannels()];
      while (recorder.GetData(buff, 1024))
      {
        outfile.Write(buff, 1024);
      }
    }
    printf("\n\n");

    netmd.Stop();
  }

  recorder.Stop();
}

void SignalHandler(int signal)
{
  if (signal == SIGINT)
    printf("Caught SIGINT\n");
  else if (signal == SIGTERM)
    printf("Caught SIGTERM\n");
  else
    printf("Caught %i\n", signal);

  if (signal == SIGINT || signal == SIGTERM)
    g_stop = true;
}
