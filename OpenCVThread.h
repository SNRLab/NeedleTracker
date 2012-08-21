/*
 *  OpenCVThread.h
 *  
 *
 *  Created by Atsushi Yamada on 12/08/21.
 *  Copyright 2012 Brigham and Women's Hospital. All rights reserved.
 *
 */

#include <QThread.h>

class OpenCVThread : public QThread
{
  Q_OBJECT
  
public:
  OpenCVThread();
  
  void setMessage(const QString &message);
  void stop();
  
protected:
  void run();
  
private:
  QString messageStr;
  volatile bool stopped;
  
};