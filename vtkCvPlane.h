/*
 *  vtkCvPlane.h
 *  
 *
 *  Created by Atsushi Yamada on 12/08/28.
 *  Copyright 2012 Brigham and Women's Hospital. All rights reserved.
 *
 */

#ifndef VTKCVPLANE_H_
#define VTKCVPLANE_H_

// NeedleTrackerModule includes
//#include "qSlicerNeedleTrackerModuleExport.h"

// VTK includes
#include <vtkPlaneSource.h>

class Q_SLICER_QTMODULES_NEEDLETRACKER_EXPORT vtkCvPlane : 
  public vtkPlaneSource
{

public:
  
  static vtkCvPlane *New();
  vtkTypeRevisionMacro(vtkCvPlane, vtkPlaneSource);
  void PrintSelf(ostream& os, vtkIndent indent);

protected:
  
  vtkCvPlane();
  virtual ~vtkCvPlane();
  
private:
  


};


#endif /* VTKCVPLANE_H_ */