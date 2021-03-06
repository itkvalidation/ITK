/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef __itkMatrix_hxx
#define __itkMatrix_hxx

#include "itkMatrix.h"
#include "itkNumericTraits.h"

namespace itk
{
/**
 *  Product by a Vector
 */
template< class T, unsigned int NRows, unsigned int NColumns >
Vector< T, NRows >
Matrix< T, NRows, NColumns >
::operator*(const Vector< T, NColumns > & vect) const
{
  Vector< T, NRows > result;
  for ( unsigned int r = 0; r < NRows; r++ )
    {
    T sum = NumericTraits< T >::Zero;
    for ( unsigned int c = 0; c < NColumns; c++ )
      {
      sum += m_Matrix(r, c) * vect[c];
      }
    result[r] = sum;
    }
  return result;
}

/**
 *  Product by a Point
 */
template< class T, unsigned int NRows, unsigned int NColumns >
Point< T, NRows >
Matrix< T, NRows, NColumns >
::operator*(const Point< T, NColumns > & pnt) const
{
  Point< T, NRows > result;
  for ( unsigned int r = 0; r < NRows; r++ )
    {
    T sum = NumericTraits< T >::Zero;
    for ( unsigned int c = 0; c < NColumns; c++ )
      {
      sum += m_Matrix(r, c) * pnt[c];
      }
    result[r] = sum;
    }
  return result;
}

/**
 *  Product by a CovariantVector
 */
template< class T, unsigned int NRows, unsigned int NColumns >
CovariantVector< T, NRows >
Matrix< T, NRows, NColumns >
::operator*(const CovariantVector< T, NColumns > & covect) const
{
  CovariantVector< T, NRows > result;
  for ( unsigned int r = 0; r < NRows; r++ )
    {
    T sum = NumericTraits< T >::Zero;
    for ( unsigned int c = 0; c < NColumns; c++ )
      {
      sum += m_Matrix(r, c) * covect[c];
      }
    result[r] = sum;
    }
  return result;
}

/**
 *  Product by a matrix
 */
template< class T, unsigned int NRows, unsigned int NColumns >
Matrix< T, NRows, NColumns >
Matrix< T, NRows, NColumns >
::operator*(const Self & matrix) const
{
  Self result;

  result = m_Matrix * matrix.m_Matrix;
  return result;
}

/**
 *  Matrix Addition
 */
template< class T, unsigned int NRows, unsigned int NColumns >
Matrix< T, NRows, NColumns >
Matrix< T, NRows, NColumns >
::operator+(const Self & matrix) const
{
  Self result;

  for ( unsigned int r = 0; r < NRows; r++ )
    {
    for ( unsigned int c = 0; c < NColumns; c++ )
      {
      result.m_Matrix(r, c) = m_Matrix(r, c) + matrix.m_Matrix(r, c);
      }
    }
  return result;
}

/**
 *  Matrix Addition in-place
 */
template< class T, unsigned int NRows, unsigned int NColumns >
const Matrix< T, NRows, NColumns > &
Matrix< T, NRows, NColumns >
::operator+=(const Self & matrix)
{
  for ( unsigned int r = 0; r < NRows; r++ )
    {
    for ( unsigned int c = 0; c < NColumns; c++ )
      {
      m_Matrix(r, c) += matrix.m_Matrix(r, c);
      }
    }
  return *this;
}

/**
 *  Matrix Subtraction
 */
template< class T, unsigned int NRows, unsigned int NColumns >
Matrix< T, NRows, NColumns >
Matrix< T, NRows, NColumns >
::operator-(const Self & matrix) const
{
  Self result;

  for ( unsigned int r = 0; r < NRows; r++ )
    {
    for ( unsigned int c = 0; c < NColumns; c++ )
      {
      result.m_Matrix(r, c) = m_Matrix(r, c) - matrix.m_Matrix(r, c);
      }
    }
  return result;
}

/**
 *  Matrix subtraction in-place
 */
template< class T, unsigned int NRows, unsigned int NColumns >
const Matrix< T, NRows, NColumns > &
Matrix< T, NRows, NColumns >
::operator-=(const Self & matrix)
{
  for ( unsigned int r = 0; r < NRows; r++ )
    {
    for ( unsigned int c = 0; c < NColumns; c++ )
      {
      m_Matrix(r, c) -= matrix.m_Matrix(r, c);
      }
    }
  return *this;
}

/**
 *  Product by a vnl_matrix
 */
template< class T, unsigned int NRows, unsigned int NColumns >
vnl_matrix< T >
Matrix< T, NRows, NColumns >
::operator*(const vnl_matrix< T > & matrix) const
{
  return m_Matrix * matrix;
}

/**
 *  Product by a matrix
 */
template< class T, unsigned int NRows, unsigned int NColumns >
void
Matrix< T, NRows, NColumns >
::operator*=(const Self & matrix)
{
  m_Matrix *= matrix.m_Matrix;
}

/**
 *  Product by a vnl_matrix
 */
template< class T, unsigned int NRows, unsigned int NColumns >
void
Matrix< T, NRows, NColumns >
::operator*=(const vnl_matrix< T > & matrix)
{
  m_Matrix *= matrix;
}

/**
 *  Product by a vnl_vector
 */
template< class T, unsigned int NRows, unsigned int NColumns >
vnl_vector< T >
Matrix< T, NRows, NColumns >
::operator*(const vnl_vector< T > & vc) const
{
  return m_Matrix * vc;
}
} // end namespace itk

#endif
