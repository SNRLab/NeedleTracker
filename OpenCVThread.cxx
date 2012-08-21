/*
 *  OpenCVThread.cxx
 *  
 *
 *  Created by Atsushi Yamada on 12/08/21.
 *  Copyright 2012 Brigham and Women's Hospital. All rights reserved.
 *
 */

#include "OpenCVThread.h"

OpenCVThread::OpenCVThread()
{
  this->stopped = false;
}

void OpenCVThread::run()
{
  while (!this->stopped)
    //cerr << qPrintable(messageStr);
  this->stopped = false;
  //cerr << endl;
}

void OpenCVThread::stop()
{
  this->stopped = true;
}


