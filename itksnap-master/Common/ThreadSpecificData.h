/*=========================================================================

  Program:   ITK-SNAP
  Module:    $RCSfile: SNAPLevelSetFunction.h,v $
  Language:  C++
  Date:      $Date: 2007/12/30 04:05:14 $
  Version:   $Revision: 1.2 $
  Copyright (c) 2007 Paul A. Yushkevich
  
  This file is part of ITK-SNAP 

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0.txt

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  -----

  Copyright (c) 2003 Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information. 

=========================================================================*/
#ifndef THREADSPECIFICDATA_H
#define THREADSPECIFICDATA_H

#include <cstdlib>

class IRISException;

/** Support class for ThreadSpecificData */
class ThreadSpecificDataSupport
{
public:
  ThreadSpecificDataSupport();
  ~ThreadSpecificDataSupport();

  /**
    Get a pointer to the cached data, allocating memory of data_size bytes
    if cached data does not exist. The data will be automatically deallocated
    when the thread terminates
    */
  void *GetPtrCreateIfNeeded(size_t datasize);

  /**
    Get a pointer. If it has not been created (a value already assigned),
    then this will return NULL.
    */
  void *GetPtr() const;

protected:

  static void Deleter(void *p);

  // The key. To avoid having to do platform-specific stuff here,
  // we just make this key a void pointer. The key is used to create
  // specific data objects;
  void *m_KeyPointer;
};

/**
  Encapsulates the functionality provided by pthread_key_create,
  pthread_getspecific and pthread_setspecific. This allows an instance
  of an atomic type to be cached on a per-thread basis. To use this class
  in an ITK filter, just add a variable of this type to the class. Then,
  inside of the threaded code, use the object as you would a regular variable
  to set and get the values.
  */
template <class TAtomic>
class ThreadSpecificData
{
public:
  typedef ThreadSpecificData<TAtomic> Self;

  /**
    Assign a value to the cache. Make sure to do this before trying to access
    the value using the cast operator below, or you will get a segfault.
    */
  Self & operator = (const TAtomic &value)
  {
    TAtomic *p = (TAtomic *)(m_Support.GetPtrCreateIfNeeded(sizeof(TAtomic)));
    *p = value;
    return *this;
  }

  /**
    Get the value from the cache. If the value has not been previously set,
    this will probably segfault. For speed, there is no check implemented!
    */
  operator TAtomic() const
  {
    TAtomic *p = (TAtomic *)(m_Support.GetPtr());
    return *p;
  }

protected:

  ThreadSpecificDataSupport m_Support;
};

#endif // THREADSPECIFICDATA_H
