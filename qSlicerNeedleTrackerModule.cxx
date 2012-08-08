/*==============================================================================

  Program: 3D Slicer

  Portions (c) Copyright Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

==============================================================================*/

// Qt includes
#include <QtPlugin>

// ExtensionTemplate Logic includes
#include <vtkSlicerNeedleTrackerLogic.h>

// ExtensionTemplate includes
#include "qSlicerNeedleTrackerModule.h"
#include "qSlicerNeedleTrackerModuleWidget.h"

//-----------------------------------------------------------------------------
Q_EXPORT_PLUGIN2(qSlicerNeedleTrackerModule, qSlicerNeedleTrackerModule);

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicerNeedleTrackerModulePrivate
{
public:
  qSlicerNeedleTrackerModulePrivate();
};

//-----------------------------------------------------------------------------
// qSlicerNeedleTrackerModulePrivate methods

//-----------------------------------------------------------------------------
qSlicerNeedleTrackerModulePrivate::qSlicerNeedleTrackerModulePrivate()
{
}

//-----------------------------------------------------------------------------
// qSlicerNeedleTrackerModule methods

//-----------------------------------------------------------------------------
qSlicerNeedleTrackerModule::qSlicerNeedleTrackerModule(QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qSlicerNeedleTrackerModulePrivate)
{
}

//-----------------------------------------------------------------------------
qSlicerNeedleTrackerModule::~qSlicerNeedleTrackerModule()
{
}

//-----------------------------------------------------------------------------
QString qSlicerNeedleTrackerModule::helpText()const
{
  return "This is a loadable module bundled in an extension";
}

//-----------------------------------------------------------------------------
QString qSlicerNeedleTrackerModule::acknowledgementText()const
{
  return "This work was was partially funded by NIH grant 3P41RR013218-12S1";
}

//-----------------------------------------------------------------------------
QStringList qSlicerNeedleTrackerModule::contributors()const
{
  QStringList moduleContributors;
  moduleContributors << QString("Jean-Christophe Fillion-Robin (Kitware)");
  return moduleContributors;
}

//-----------------------------------------------------------------------------
QIcon qSlicerNeedleTrackerModule::icon()const
{
  return QIcon(":/Icons/NeedleTracker.png");
}

//-----------------------------------------------------------------------------
QStringList qSlicerNeedleTrackerModule::categories() const
{
  return QStringList() << "Examples";
}

//-----------------------------------------------------------------------------
QStringList qSlicerNeedleTrackerModule::dependencies() const
{
  return QStringList();
}

//-----------------------------------------------------------------------------
void qSlicerNeedleTrackerModule::setup()
{
  this->Superclass::setup();
}

//-----------------------------------------------------------------------------
qSlicerAbstractModuleRepresentation * qSlicerNeedleTrackerModule::createWidgetRepresentation()
{
  return new qSlicerNeedleTrackerModuleWidget;
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qSlicerNeedleTrackerModule::createLogic()
{
  return vtkSlicerNeedleTrackerLogic::New();
}
