#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <math.h>


#include <metaUtils.h>
#include <metaObject.h>


//
// MetaObject Constructors
//
MetaObject::
MetaObject(void)
  {
  m_NDims = 0;
  m_ID = 0;
  m_Color[0]=1.0;m_Color[1]=0.0;m_Color[2]=0.0;m_Color[3]=1.0; // red by default
  m_ParentID = -1;
  m_BinaryData = false;
  strcpy(m_Name, ""); 
  m_Fields.clear();
  strcpy(m_FileName, "");
  Clear();
  m_ReadStream = new std::ifstream;
  m_WriteStream = new std::ofstream;
  strcpy(m_TransformName,"Affine");
  }

MetaObject::
MetaObject(const char * _fileName)
  {
  m_NDims = 0;
  m_Fields.clear();
  this->Read(_fileName);
  }

MetaObject::
MetaObject(unsigned int dim)
  {
  m_NDims = 0;
  m_ID = 0;
  m_Color[0]=1.0;m_Color[1]=0.0;m_Color[2]=0.0;m_Color[3]=1.0; // red by default
  m_ParentID = -1;
  m_BinaryData = false;
  strcpy(m_Name, ""); 
  m_Fields.clear();
  strcpy(m_FileName, "");
  Clear();
  m_ReadStream = new std::ifstream;
  m_WriteStream = new std::ofstream;
  InitializeEssential(dim);
  strcpy(m_TransformName,"Affine");
  }


MetaObject::
~MetaObject(void)
  {
  M_Destroy();
  }

//
//
void MetaObject::
FileName(const char *_fileName)
  {
  if(_fileName != NULL)
    {
    strcpy(m_FileName, _fileName);
    }
  }

const char * MetaObject::
FileName(void) const
  {
  return m_FileName;
  }

void MetaObject::
CopyInfo(const MetaObject * _object)
  {
  if(NDims() != _object->NDims())
    {
    std::cout << "MetaObject: CopyInfo: Warning: NDims not same size" << std::endl;
    }

  FileName(_object->FileName());
  Comment(_object->Comment());
  ObjectTypeName(_object->ObjectTypeName());
  ObjectSubTypeName(_object->ObjectSubTypeName());
  Position(_object->Position());
  Orientation(_object->Orientation());
  ElementSpacing(_object->ElementSpacing());
  ID(_object->ID());
  Color(_object->Color());
  ParentID(_object->ParentID());
  Name(_object->Name());
  BinaryData(_object->BinaryData());

  }

bool MetaObject::
Read(const char *_fileName)
  {
  if(DEBUG)  std::cout << "MetaObject: Read" << std::endl;

  if(_fileName != NULL)
    {
    strcpy(m_FileName, _fileName);
    }

  M_Destroy();

  Clear();

  M_SetupReadFields();

  m_ReadStream->open(m_FileName);
  if(!m_ReadStream->is_open())
    {
    std::cout << "MetaObject: Read: Cannot open file" << std::endl;
    return false;
    }

  bool result = M_Read();

  m_ReadStream->close();

  return result;
  }

bool MetaObject::
ReadStream(int _nDims, std::ifstream * _stream)
{
  if(DEBUG) std::cout << "MetaObject: ReadStream" << std::endl;

  M_Destroy();
  
  fflush(NULL);
  
  Clear();
  
  M_SetupReadFields();
 
  MET_FieldRecordType * mF = MET_GetFieldRecord("NDims",m_Fields);
  mF->value[0] = _nDims;
  mF->defined = true;
 
  m_ReadStream = _stream;
 
  bool result = M_Read();

  return result;
}



bool MetaObject::
Write(const char *_fileName)
  {
  if(_fileName != NULL)
    {
    FileName(_fileName);
    }

  M_SetupWriteFields();

  m_WriteStream->open(m_FileName);
  if(!m_WriteStream->is_open())
    {
    return false;
    }

  bool result = M_Write();

  m_WriteStream->close();

  return result;
  }

//
//
void MetaObject::
PrintInfo(void) const
  {
  int i, j;

  std::cout << "FileName = _" << m_FileName << "_" << std::endl;
  std::cout << "Comment = _" << m_Comment << "_" << std::endl;
  std::cout << "ObjectType = _" << m_ObjectTypeName << "_" << std::endl;
  std::cout << "ObjectSubType = _" << m_ObjectSubTypeName << "_" << std::endl;
  std::cout << "TransformName = _" << m_TransformName << "_" << std::endl;
  std::cout << "NDims = " << m_NDims << std::endl;
  std::cout << "Name = " << m_Name << std::endl;
  std::cout << "ID = " << m_ID << std::endl;
  std::cout << "ParentID = " << m_ParentID << std::endl;
  if(m_BinaryData)
    std::cout << "BinaryData = True" << std::endl;
  else
    std::cout << "BinaryData = False" << std::endl;
  std::cout << "Color = " ;
  for(i=0; i<4; i++)
    {
    std::cout << m_Color[i] << " ";
    }
  std::cout << std::endl;

  std::cout << "Position = ";
  for(i=0; i<m_NDims; i++)
    {
    std::cout << m_Position[i] << " ";
    }
  std::cout << std::endl;
  
  std::cout << "Orientation = ";
  std::cout << std::endl;
  for(i=0; i<m_NDims; i++)
    {
    for(j=0; j<m_NDims; j++)
      {
      std::cout << m_Orientation[i*m_NDims+j] << " ";
      }
    std::cout << std::endl;
    }

  std::cout << "ElementSpacing = ";
  for(i=0; i<m_NDims; i++)
    {
    std::cout << m_ElementSpacing[i] << " ";
    }
  std::cout << std::endl;

  }

const char * MetaObject::
Comment(void) const
  {
  return m_Comment;
  }

void MetaObject::
Comment(const char * _comment)
  {
  strcpy(m_Comment, _comment);
  }

const char * MetaObject::
ObjectTypeName(void) const
  {
  return (char *)m_ObjectTypeName;
  }

void MetaObject::
ObjectTypeName(const char * _objectTypeName)
  {
  strcpy(m_ObjectTypeName, _objectTypeName);
  }

const char * MetaObject::
ObjectSubTypeName(void) const
  {
  return m_ObjectSubTypeName;
  }

void MetaObject::
ObjectSubTypeName(const char * _objectSubTypeName)
  {
  strcpy(m_ObjectSubTypeName, _objectSubTypeName);
  }

const char * MetaObject::
TransformName(void) const
  {
  return m_TransformName;
  }

void MetaObject::
TransformName(const char * _transformName)
  {
  strcpy(m_TransformName, _transformName);
  }


int MetaObject::
NDims(void) const
  {
  return m_NDims;
  }

const float * MetaObject::
Position(void) const
  {
  return m_Position;
  }

float MetaObject::
Position(int _i) const
  {
  return m_Position[_i];
  }

void MetaObject::
Position(const float * _position)
  {
  int i;
  for(i=0; i<m_NDims; i++)
    {
    m_Position[i] = _position[i];
    }
  }

void MetaObject::
Position(int _i, float _value)
  {
  m_Position[_i] = _value;
  }

//
//
const float * MetaObject::
Orientation(void) const
  {
  return m_Orientation;
  }

float MetaObject::
Orientation(int _i, int _j) const
  {
  return m_Orientation[_i*m_NDims+_j];
  }

void MetaObject::
Orientation(const float * _orientation)
  {
  int i;
  for(i=0; i<m_NDims*m_NDims; i++)
    {
    m_Orientation[i] = _orientation[i];
    }
  }

void MetaObject::
Orientation(int _i, int _j, float _value)
  {
  m_Orientation[_i*m_NDims+_j] = _value;
  }

const float * MetaObject::
ElementSpacing(void) const
  {
  return m_ElementSpacing;
  }

float MetaObject::
ElementSpacing(int _i) const
  {
  return m_ElementSpacing[_i];
  }

void MetaObject::
ElementSpacing(const float * _elementSpacing)
  {
  int i;
  for(i=0; i<m_NDims; i++)
    {
    m_ElementSpacing[i] = _elementSpacing[i];
    }
  }

void MetaObject::
ElementSpacing(int _i, float _value)
  {
  m_ElementSpacing[_i] = _value;
  }


void  MetaObject::
Name(const char *_Name)
{
  if(_Name != NULL)
  {
    strcpy(m_Name, _Name);
  }
}
      
const char  * MetaObject::
Name(void) const
{
  return m_Name;
}

   
const float * MetaObject::
Color(void) const
{
  return m_Color;
}

void  MetaObject::
Color(float _r, float _g, float _b, float _a)
{
  m_Color[0] = _r;
  m_Color[1] = _g;
  m_Color[2] = _b;
  m_Color[3] = _a;
}
void MetaObject::
Color(const float * _color)
{
  for(unsigned int i=0; i<4; i++)
  {
    m_Color[i] = _color[i];
  }
}


void  MetaObject::
ID(unsigned int _id)
{
  m_ID = _id;
}
      
unsigned int  MetaObject::
ID(void) const
{
  return m_ID;
}

void  MetaObject::
ParentID(int _parentId)
{
  m_ParentID = _parentId;
}
      
int   MetaObject::ParentID(void) const
{
  return m_ParentID;
}


void  MetaObject::BinaryData(bool _binaryData)
{
  m_BinaryData = _binaryData;
}
      
bool   MetaObject::BinaryData(void) const
{
  return m_BinaryData;
}


void MetaObject::
Clear(void)
  {
  if(DEBUG)  std::cout << "MetaObject: Clear()" << std::endl;
  strcpy(m_Comment, "");
  strcpy(m_ObjectTypeName, "");
  strcpy(m_ObjectSubTypeName, "");
  strcpy(m_Name, "");

  memset(m_Position, 0, 10*sizeof(float));
  memset(m_Orientation, 0, 100*sizeof(float));
  memset(m_ElementSpacing, 0, 10*sizeof(float));
  memset(m_Color, 0, 4*sizeof(float));

  m_ID = 0;
  m_Color[0]=1.0;m_Color[1]=0.0;m_Color[2]=0.0;m_Color[3]=1.0; // red by default
  m_ParentID = -1;
  m_BinaryData = false;

  int i;
  for(i=0; i<m_NDims; i++)
    {
    m_ElementSpacing[i] = 1;
    m_Orientation[i*m_NDims+i] = 1;
    }

  std::vector<MET_FieldRecordType *>::iterator fieldIter;
  for(fieldIter=m_Fields.begin(); fieldIter!=m_Fields.end(); fieldIter++)
    {
    delete * fieldIter;
    }
  m_Fields.clear();
  }

bool MetaObject::
InitializeEssential(int _nDims)
  {
  if(DEBUG) std::cout << "MetaObject: Initialize" << std::endl;

  M_Destroy();

  if(_nDims > 10)
    {
    std::cout 
      << "MetaObject: Initialize: Warning: Number of dimensions limited to 10" 
      << std::endl
      << "Resetting number of dimensions to 10"
      << std::endl;
    _nDims = 10;
    }

  if(_nDims < 0)
    {
    std::cout
      << "MetaObject: Initialize: Warning: Number of dimensions must be >= 0"
      << std::endl
      << "Resetting number of dimensions to 0"
      << std::endl;
    _nDims = 0;
    }

  m_NDims = _nDims;

  return true;
  }

void MetaObject::
M_Destroy(void)
  {
  if(DEBUG) std::cout << "MetaObject: Destroy" << std::endl;
  }

void MetaObject::
M_SetupReadFields(void)
  {
  m_Fields.clear();
  if(DEBUG) std::cout << "MetaObject: SetupReadFields" << std::endl;

  MET_FieldRecordType * mF;

  mF = new MET_FieldRecordType;
  MET_InitReadField(mF, "Comment", MET_STRING, false);
  m_Fields.push_back(mF);

  mF = new MET_FieldRecordType;
  MET_InitReadField(mF, "ObjectType", MET_STRING, false);
  m_Fields.push_back(mF);

  mF = new MET_FieldRecordType;
  MET_InitReadField(mF, "ObjectSubType", MET_STRING, false);
  m_Fields.push_back(mF);

  mF = new MET_FieldRecordType;
  MET_InitReadField(mF, "TransformType", MET_STRING, false);
  m_Fields.push_back(mF);

  mF = new MET_FieldRecordType;
  MET_InitReadField(mF, "NDims", MET_INT, true);
  mF->required = true;
  m_Fields.push_back(mF);

  int nDimsRecordNumber = MET_GetFieldRecordNumber("NDims",m_Fields);

  mF = new MET_FieldRecordType;
  MET_InitReadField(mF, "Name", MET_STRING, false);
  m_Fields.push_back(mF);

  mF = new MET_FieldRecordType;
  MET_InitReadField(mF, "ID", MET_INT, false);
  m_Fields.push_back(mF);

  mF = new MET_FieldRecordType;
  MET_InitReadField(mF, "ParentID", MET_INT, false);
  m_Fields.push_back(mF);

  mF = new MET_FieldRecordType;
  MET_InitReadField(mF, "BinaryData", MET_STRING, false);
  m_Fields.push_back(mF);

  mF = new MET_FieldRecordType;
  MET_InitReadField(mF, "Color", MET_FLOAT_ARRAY, false,-1,4);
  m_Fields.push_back(mF);

  mF = new MET_FieldRecordType;
  MET_InitReadField(mF, "Position", MET_FLOAT_ARRAY, false,
                     nDimsRecordNumber);
  m_Fields.push_back(mF);

  mF = new MET_FieldRecordType;
  MET_InitReadField(mF, "Orientation", MET_FLOAT_MATRIX, false,
                    nDimsRecordNumber);
  m_Fields.push_back(mF);

  mF = new MET_FieldRecordType;
  MET_InitReadField(mF, "ElementSpacing", MET_FLOAT_ARRAY, false,
                     nDimsRecordNumber);
  mF->required = false;
  m_Fields.push_back(mF);
  }


void MetaObject::
M_SetupWriteFields(void)
  {

  m_Fields.clear();

  MET_FieldRecordType * mF;

  if(strlen(m_Comment)>0)
    {
    mF = new MET_FieldRecordType;
    MET_InitWriteField(mF, "Comment", MET_STRING, strlen(m_Comment), m_Comment);
    m_Fields.push_back(mF);
    }

  if(strlen(m_ObjectTypeName)>0)
    {
    mF = new MET_FieldRecordType;
    MET_InitWriteField(mF, "ObjectType", MET_STRING, strlen(m_ObjectTypeName),
                      m_ObjectTypeName);
    m_Fields.push_back(mF);
    }

  if(strlen(m_ObjectSubTypeName)>0)
    {
    mF = new MET_FieldRecordType;
    MET_InitWriteField(mF, "ObjectSubType", MET_STRING,
                      strlen(m_ObjectSubTypeName),
                      m_ObjectSubTypeName);
    m_Fields.push_back(mF);
    }

  if(strlen(m_TransformName)>0)
    {
    mF = new MET_FieldRecordType;
    MET_InitWriteField(mF, "TransformType", MET_STRING, strlen(m_TransformName),
                      m_TransformName);
    m_Fields.push_back(mF);
    }

  mF = new MET_FieldRecordType;
  MET_InitWriteField(mF, "NDims", MET_INT, m_NDims);
  m_Fields.push_back(mF);
  
  if(strlen(m_Name)>0)
    {
    mF = new MET_FieldRecordType;
    MET_InitWriteField(mF, "Name", MET_STRING,
                      strlen(m_Name),m_Name);
    m_Fields.push_back(mF);
    }

  mF = new MET_FieldRecordType;
  MET_InitWriteField(mF, "ID", MET_INT, m_ID);
  m_Fields.push_back(mF);

  mF = new MET_FieldRecordType;
  MET_InitWriteField(mF, "ParentID", MET_INT, m_ParentID);
  m_Fields.push_back(mF);

  mF = new MET_FieldRecordType;
  if(m_BinaryData)
    MET_InitWriteField(mF, "BinaryData", MET_STRING, strlen("True"), "True");
  else
    MET_InitWriteField(mF, "BinaryData", MET_STRING, strlen("False"), "False");
  m_Fields.push_back(mF);

  mF = new MET_FieldRecordType;
  MET_InitWriteField(mF, "Color", MET_FLOAT_ARRAY, 4,
                       m_Color);
  m_Fields.push_back(mF);

  bool valSet = false;
  int i;
  for(i=0; i<m_NDims; i++)
    {
    if(m_Position[i] != 0)
      {
      valSet = true;
      break;
      }
    }
  if(valSet)
    {
    mF = new MET_FieldRecordType;
    MET_InitWriteField(mF, "Position", MET_FLOAT_ARRAY, m_NDims,
                       m_Position);
    m_Fields.push_back(mF);
    }

  for(i=0; i<m_NDims*m_NDims; i++)
    {
    if(m_Position[i] != 0)
      {
      valSet = true;
      break;
      }
    }
  if(valSet)
    {
    mF = new MET_FieldRecordType;
    MET_InitWriteField(mF, "Orientation", MET_FLOAT_MATRIX, m_NDims,
                       m_Orientation);
    m_Fields.push_back(mF);
    }

  
  for(i=0; i<m_NDims; i++)
    {
    if(m_ElementSpacing[i] != 0)
      {
      valSet = true;
      break;
      }
    }
  if(valSet)
    {
    mF = new MET_FieldRecordType;
    MET_InitWriteField(mF, "ElementSpacing", MET_FLOAT_ARRAY, m_NDims,
                       m_ElementSpacing);
    m_Fields.push_back(mF);
    }
  }

bool MetaObject::
M_Read(void)
  {

  if(!MET_Read(*m_ReadStream, m_Fields))
    {
    std::cout << "MetaObject: Read: MET_Read Failed" << std::endl;
    return false;
    }

  MET_FieldRecordType * mF;

  mF = MET_GetFieldRecord("Comment",m_Fields);
  if(mF && mF->defined)
    {
    strcpy(m_Comment, (char *)(mF->value));
    }

  mF = MET_GetFieldRecord("ObjectType",m_Fields);
  if(mF && mF->defined)
    {
    strcpy(m_ObjectTypeName, (char *)(mF->value));
    }

  mF = MET_GetFieldRecord("ObjectSubType",m_Fields);
  if(mF && mF->defined)
    {
    strcpy(m_ObjectSubTypeName, (char *)(mF->value));
    }

  mF = MET_GetFieldRecord("NDims",m_Fields);
  if(mF && mF->defined)
    {
    m_NDims = (int)mF->value[0];
    }

  if(m_NDims>0)
    {
    MetaObject::InitializeEssential(m_NDims);
    }
 
  mF = MET_GetFieldRecord("Name",m_Fields);
  if(mF && mF->defined)
    {
    strcpy(m_Name, (char *)(mF->value));
    }

  mF = MET_GetFieldRecord("ID",m_Fields);
  if(mF && mF->defined)
    {
    m_ID = (int)mF->value[0];
    }

  mF = MET_GetFieldRecord("ParentID",m_Fields);
  if(mF && mF->defined)
    {
    m_ParentID = (int)mF->value[0];
    }

  mF = MET_GetFieldRecord("BinaryData", m_Fields);
  if(mF && mF->defined)
    {
    if(((char *)(mF->value))[0] == 'T' || ((char *)(mF->value))[0] == 't' 
       || ((char *)(mF->value))[0] == '1')
      m_BinaryData = true;
    else
      m_BinaryData = false;
    }

  int i;
  mF = MET_GetFieldRecord("Color",m_Fields);
  if(mF && mF->defined)
    {
    for(i=0; i<mF->length; i++)
      {
      m_Color[i] = mF->value[i];
      }
    }

  mF = MET_GetFieldRecord("Position",m_Fields);
  if(mF && mF->defined)
    {
    for(i=0; i<mF->length; i++)
      {
      m_Position[i] = mF->value[i];
      }
    }

  mF = MET_GetFieldRecord("Orientation",m_Fields);
  if(mF && mF->defined)
    {
    int len = mF->length;
    for(i=0; i<len*len; i++)
      {
      m_Orientation[i] = mF->value[i];
      }
    }

  mF = MET_GetFieldRecord("ElementSpacing",m_Fields);
  if(mF && mF->defined)
    {
    for(i=0; i<mF->length; i++)
      {
      m_ElementSpacing[i] = mF->value[i];
      if (DEBUG) 
        std::cout << "metaObject: M_Read: elementSpacing[" << i << "] = " 
                  << m_ElementSpacing[i] << std::endl;
      }
    }
  else
    {
    for(i=0; i<mF->length; i++)
      {
      m_ElementSpacing[i] = 1;
      if (DEBUG) 
        std::cout << "metaObject: M_Read: elementSpacing[" << i << "] = " 
                  << m_ElementSpacing[i] << std::endl;
      }
    }

  return true;
  }

bool MetaObject::
M_Write(void)
  {
  if(!MET_Write(*m_WriteStream, m_Fields))
    {
    std::cout << "MetaObject: Write: MET_Write Failed" << std::endl;
    return false;
    }

  return true;
  }
