# ifndef CPPAD_EXAMPLE_VALVECTOR_CLASS_HPP
# define CPPAD_EXAMPLE_VALVECTOR_CLASS_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2024 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cmath>
# include <iostream>
# include <cassert>
# include <cppad/utility/vector.hpp>
# include <cppad/base_require.hpp>
// ============================================================================
// Macros
// ============================================================================
# define CPPAD_VALVECTOR_ASSERT_KNOWN(exp, msg) \
   if( ! (exp ) ) \
   {  std::cerr << "valvector: " << msg << "\n"; \
      assert( exp ); \
   }
//
# define CPPAD_VALVECTOR_UNARY_NOT_AVAILABLE(type, fun) \
   inline type fun(const valvector& x) \
   {  CPPAD_VALVECTOR_ASSERT_KNOWN( false, #fun " is not available" ) \
      return type(); \
   }
//
# define CPPAD_VALVECTOR_STD_MATH_MEMBER(fun) \
   valvector fun(void) const \
   {  valvector result; \
      result.resize( size() ); \
      for(size_t i = 0; i < size(); ++i) \
         result.vec_[i] = std::fun( vec_[i] ); \
      return result; \
   }
//
# define CPPAD_VALVECTOR_MEMBER2FUNCTION(fun) \
   inline valvector fun(const valvector& x) \
   {  return x.fun(); \
   }
//
# define CPPAD_VALVECTOR_BINARY_NUMERIC_OP(op, compound_op) \
   valvector operator op(const valvector& other) const \
   {  valvector result;   \
      if( size() == 1 ) \
      {  result.resize( other.size() ); \
         for(size_t i = 0; i < other.size(); ++i) \
            result.vec_[i] = vec_[0] op other.vec_[i]; \
      } \
      else if( other.size() == 1 ) \
      {  result.resize( size() ); \
         for(size_t i = 0; i < size(); ++i) \
            result.vec_[i] = vec_[i] op other.vec_[0]; \
      } \
      else \
      {  CPPAD_VALVECTOR_ASSERT_KNOWN(  \
            size() == other.size() , \
            "size error using " #op " operator" \
         ) \
         result.vec_.resize( size() ); \
         for(size_t i = 0; i < size(); ++i) \
            result.vec_[i] = vec_[i] op other.vec_[i]; \
      } \
      return result; \
   } \
   valvector& operator compound_op(const valvector& other) \
   {  if( size() == 1 ) \
      {  scalar_type vec_0 = vec_[0]; \
         resize( other.size() ); \
         for(size_t i = 0; i < other.size(); ++i) \
            vec_[i] = vec_0 op other.vec_[i]; \
      } \
      else if( other.size() == 1 ) \
      {  for(size_t i = 0; i < size(); ++i) \
            vec_[i] = vec_[i] op other.vec_[0]; \
      } \
      else \
      {  CPPAD_VALVECTOR_ASSERT_KNOWN(  \
            size() == other.size() , \
            "size error using " #compound_op " operator" \
         ) \
         for(size_t i = 0; i < size(); ++i) \
            vec_[i] = vec_[i] op other.vec_[i]; \
      } \
      return *this; \
   }
// ============================================================================
// valvector
// ============================================================================
//
// valvector
// Forward declare
class valvector;
//
// CondExpOp
// Forward decalre
namespace CppAD {
   inline valvector CondExpOp(
      enum CompareOp         cop          ,
      const valvector&       left         ,
      const valvector&       right        ,
      const valvector&       exp_if_true  ,
      const valvector&       exp_if_false
   );
}
class valvector {
   //
   // friend
   friend inline valvector CppAD::CondExpOp(
      enum CppAD::CompareOp  cop          ,
      const valvector&       left         ,
      const valvector&       right        ,
      const valvector&       exp_if_true  ,
      const valvector&       exp_if_false
   );
public:
   //
   // scalar_type, vector_type
   typedef double                     scalar_type;
   typedef CppAD::vector<scalar_type> vector_type;
   //
private:
   // vec_
   vector_type vec_;
public:
   // -----------------------------------------------------------------------
   // default ctor
   valvector(void) : vec_(1)
   {  vec_[0] = scalar_type(0); }
   //
   // ctor of scalar
   valvector(int s) : vec_(1)
   {  vec_[0] = scalar_type(s); }
   valvector(long int s) : vec_(1)
   {  vec_[0] = scalar_type(s); }
   valvector(double s) : vec_(1)
   {  vec_[0] = scalar_type(s); }
   valvector(long double s) : vec_(1)
   {  vec_[0] = scalar_type(s); }
   valvector(size_t s) : vec_(1)
   {  vec_[0] = scalar_type(s); }
   //
   valvector(const vector_type& vec) : vec_(vec)
   { }
   valvector(const valvector& other) : vec_( other.vec_)
   { }
   valvector(valvector&& other)
   {  vec_.swap( other.vec_ );
   }
   valvector(std::initializer_list<scalar_type> list) : vec_(list)
   {  CPPAD_VALVECTOR_ASSERT_KNOWN(
         vec_.size() != 0,
         "Cannot create a valvector with size zero."
      )
   }
   // -----------------------------------------------------------------------
   // assignments
   valvector& operator=(const valvector& other)
   {  vec_ = other.vec_;
      return *this;
   }
   valvector& operator=(valvector&& other)
   {  vec_.swap( other.vec_ );
      return *this;
   }
   // -------------------------------------------------------------------------
   // resize
   void resize(size_t n)
   {  assert( n != 0 );
      vec_.resize(n);
   }
   // -------------------------------------------------------------------------
   // data
   vector_type& data(void)
   {  return vec_; }
   const vector_type& data(void) const
   {  return vec_; }
   // -------------------------------------------------------------------------
   // Unary operators and functions
   //
   // size
   size_t size(void) const
   {  return vec_.size(); }
   //
   // iszero
   bool iszero(void) const
   {  bool        result = true;
      scalar_type zero   = 0;
      for(size_t i = 0; i < size(); ++i)
         result &= vec_[i] == zero;
      return result;
   }
   //
   // isone
   bool isone(void) const
   {  bool        result = true;
      scalar_type one    = 1;
      for(size_t i = 0; i < size(); ++i)
         result &= vec_[i] == one;
      return result;
   }
   //
   // unary operators
   valvector operator+(void) const
   {  return *this; }
   valvector operator-(void) const
   {  valvector result;
      result.resize( size() );
      for(size_t i = 0; i < size(); ++i)
         result.vec_[i] = - vec_[i];
      return result;
   }
   //
   // Standard Math Fucntons
   CPPAD_VALVECTOR_STD_MATH_MEMBER(acos)
   CPPAD_VALVECTOR_STD_MATH_MEMBER(acosh)
   CPPAD_VALVECTOR_STD_MATH_MEMBER(asin)
   CPPAD_VALVECTOR_STD_MATH_MEMBER(asinh)
   CPPAD_VALVECTOR_STD_MATH_MEMBER(atan)
   CPPAD_VALVECTOR_STD_MATH_MEMBER(atanh)
   CPPAD_VALVECTOR_STD_MATH_MEMBER(cos)
   CPPAD_VALVECTOR_STD_MATH_MEMBER(cosh)
   CPPAD_VALVECTOR_STD_MATH_MEMBER(erf)
   CPPAD_VALVECTOR_STD_MATH_MEMBER(erfc)
   CPPAD_VALVECTOR_STD_MATH_MEMBER(exp)
   CPPAD_VALVECTOR_STD_MATH_MEMBER(expm1)
   CPPAD_VALVECTOR_STD_MATH_MEMBER(fabs)
   CPPAD_VALVECTOR_STD_MATH_MEMBER(log)
   CPPAD_VALVECTOR_STD_MATH_MEMBER(log1p)
   CPPAD_VALVECTOR_STD_MATH_MEMBER(log10)
   CPPAD_VALVECTOR_STD_MATH_MEMBER(sin)
   CPPAD_VALVECTOR_STD_MATH_MEMBER(sinh)
   CPPAD_VALVECTOR_STD_MATH_MEMBER(sqrt)
   CPPAD_VALVECTOR_STD_MATH_MEMBER(tan)
   CPPAD_VALVECTOR_STD_MATH_MEMBER(tanh)
   //
   // sign
   valvector sign(void) const
   {  valvector result;
      result.resize( size() );
      scalar_type zero = scalar_type(0);
      for(size_t i = 0; i < size(); ++i)
      {  if( vec_[i] < zero )
            result.vec_[i] = scalar_type(-1);
         if( vec_[i] == zero )
            result.vec_[i] = zero;
         if( vec_[i] > zero )
            result.vec_[i] = scalar_type(1);
      }
      return result;
   }
   // -----------------------------------------------------------------------
   // Binary Operators and functions
   //
   CPPAD_VALVECTOR_BINARY_NUMERIC_OP(+, +=)
   CPPAD_VALVECTOR_BINARY_NUMERIC_OP(-, -=)
   CPPAD_VALVECTOR_BINARY_NUMERIC_OP(*, *=)
   CPPAD_VALVECTOR_BINARY_NUMERIC_OP(/, /=)
   //
   // ==, !=
   bool operator==(const valvector& other) const
   {  bool result = true;
      if( size() == 1 )
      {  for(size_t i = 0; i < other.size(); ++i)
            result &= vec_[0] == other.vec_[i];
      }
      else if( other.size() == 1 )
      {  for(size_t i = 0; i < other.size(); ++i)
            result &= vec_[i] == other.vec_[0];
      }
      else
      {  CPPAD_VALVECTOR_ASSERT_KNOWN(
            size() == other.size() ,
            "size error using azmul function"
         )
         for(size_t i = 0; i < size(); ++i)
            result &= vec_[i] == other.vec_[i];
      }
      return result;
   }
   bool operator!=(const valvector& other) const
   {  return ! (*this == other); }
   //
   // azmul
   valvector azmul(const valvector& other) const
   {  valvector  result;
      scalar_type zero(0);
      if( size() == 1 )
      {  if( vec_[0] == zero )
         {  result.resize(1);
            result.vec_[0] = zero;
         }
         else
         {  result.resize( other.size() );
            for(size_t i = 0; i < other.size(); ++i)
               result.vec_[i] = vec_[0] * other.vec_[i];
         }
      }
      else if( other.size() == 1 )
      {  result.resize( size() );
         for(size_t i = 0; i < size(); ++i)
            result.vec_[i] = vec_[i] * other.vec_[0];
      }
      else
      {  CPPAD_VALVECTOR_ASSERT_KNOWN(
            size() == other.size() ,
            "size error using azmul function"
         )
         result.vec_.resize( size() );
         for(size_t i = 0; i < size(); ++i)
         {  if( vec_[i] == zero )
               result.vec_[i] = zero;
            else
               result.vec_[i] = vec_[i] * other.vec_[i];
         }
      }
      return result;
   }
   //
   // pow
   valvector pow(const valvector& other) const
   {  valvector  result;
      if( size() == 1 )
      {  result.resize( other.size() );
         for(size_t i = 0; i < other.size(); ++i)
            result.vec_[i] = std::pow(vec_[0],  other.vec_[i] );
      }
      else if( other.size() == 1 )
      {  result.resize( size() );
         for(size_t i = 0; i < size(); ++i)
            result.vec_[i] = std::pow(vec_[i], other.vec_[0] );
      }
      else
      {  CPPAD_VALVECTOR_ASSERT_KNOWN(
            size() == other.size() ,
            "size error using pow function"
         )
         result.vec_.resize( size() );
         for(size_t i = 0; i < size(); ++i)
            result.vec_[i] = std::pow( vec_[i] , other.vec_[i] );
      }
      return result;
   }

   // -----------------------------------------------------------------------
   // output
   std::ostream& output(std::ostream& os)  const
   {  os << "{ ";
      for(size_t i = 0; i < vec_.size(); ++i)
      {  os << vec_[i];
         if( i + 1 < vec_.size() )
            os << ", ";
      }
      os << " }";
      return os;
   }
};
//
// ostream << valvector
inline std::ostream& operator << (
   std::ostream&    os ,
   const valvector& v  )
{  return v.output(os);
}
// ============================================================================
// CppAD namespace
// ============================================================================
namespace CppAD {
   //
   // numeric_limits
   CPPAD_NUMERIC_LIMITS(valvector::scalar_type, valvector)
   //
   // to_string_struct
   CPPAD_TO_STRING(valvector)
   //
   // -----------------------------------------------------------------------
   // Unary operators and functions
   //
   // Identical
   inline bool IdenticalCon(const valvector& x)
   {  return true;
   }
   inline bool IdenticalZero(const valvector& x)
   {  return x.iszero();
   }
   inline bool IdenticalOne(const valvector& x)
   {  return x.isone();
   }
   //
   // a
   CPPAD_VALVECTOR_UNARY_NOT_AVAILABLE(bool, GreaterThanZero)
   CPPAD_VALVECTOR_UNARY_NOT_AVAILABLE(bool, GreaterThanOrZero)
   CPPAD_VALVECTOR_UNARY_NOT_AVAILABLE(bool, LessThanZero)
   CPPAD_VALVECTOR_UNARY_NOT_AVAILABLE(bool, LessThanOrZero)
   //
   // Unary function
   CPPAD_VALVECTOR_MEMBER2FUNCTION(acos)
   CPPAD_VALVECTOR_MEMBER2FUNCTION(acosh)
   CPPAD_VALVECTOR_MEMBER2FUNCTION(asin)
   CPPAD_VALVECTOR_MEMBER2FUNCTION(asinh)
   CPPAD_VALVECTOR_MEMBER2FUNCTION(atan)
   CPPAD_VALVECTOR_MEMBER2FUNCTION(atanh)
   CPPAD_VALVECTOR_MEMBER2FUNCTION(cos)
   CPPAD_VALVECTOR_MEMBER2FUNCTION(cosh)
   CPPAD_VALVECTOR_MEMBER2FUNCTION(erf)
   CPPAD_VALVECTOR_MEMBER2FUNCTION(erfc)
   CPPAD_VALVECTOR_MEMBER2FUNCTION(exp)
   CPPAD_VALVECTOR_MEMBER2FUNCTION(expm1)
   CPPAD_VALVECTOR_MEMBER2FUNCTION(fabs)
   CPPAD_VALVECTOR_MEMBER2FUNCTION(log)
   CPPAD_VALVECTOR_MEMBER2FUNCTION(log1p)
   CPPAD_VALVECTOR_MEMBER2FUNCTION(log10)
   CPPAD_VALVECTOR_MEMBER2FUNCTION(sin)
   CPPAD_VALVECTOR_MEMBER2FUNCTION(sinh)
   CPPAD_VALVECTOR_MEMBER2FUNCTION(sqrt)
   CPPAD_VALVECTOR_MEMBER2FUNCTION(tan)
   CPPAD_VALVECTOR_MEMBER2FUNCTION(tanh)
   inline valvector abs(const valvector& x)
   {  return fabs(x); }
   CPPAD_VALVECTOR_MEMBER2FUNCTION(sign)
   // -----------------------------------------------------------------------
   // Binary opeators and functins
   //
   // EqualOpSeq
   inline bool EqualOpSeq(const valvector& left, const valvector& right)
   {  return left == right; }
   //
   // IdenticalEqualCond
   inline bool IdenticalEqualCon(
      const valvector& left  ,
      const valvector& right )
   {  return left == right; }
   //
   // azmul
   inline valvector azmul(
      const valvector& left  ,
      const valvector& right )
   {  return left.azmul(right); }
   //
   // Binary functions
   inline valvector pow(
      const valvector& left  ,
      const valvector& right )
   {  return left.pow(right); }
   //
   // Integer
   CPPAD_VALVECTOR_UNARY_NOT_AVAILABLE(int, Integer)
   //
   // abs_geq
   inline bool abs_geq(const valvector& x, const valvector& y)
   {  CPPAD_VALVECTOR_ASSERT_KNOWN( false, "abs_geq is not available" )
      return bool();
   }
   // ------------------------------------------------------------------------
   // CondExpOp
   inline valvector CondExpOp(
      enum CompareOp         cop          ,
      const valvector&       left         ,
      const valvector&       right        ,
      const valvector&       exp_if_true  ,
      const valvector&       exp_if_false )
   {  //
      // result_size
      size_t result_size = std::max(left.size(), right.size());
      result_size        = std::max(result_size, exp_if_true.size());
      result_size        = std::max(result_size, exp_if_false.size());
      //
      // size_ok
      bool size_ok  = true;
      size_ok &= left.size()         ==1 || left.size()         ==result_size;
      size_ok &= right.size()        ==1 || right.size()        ==result_size;
      size_ok &= exp_if_true.size()  ==1 || exp_if_true.size()  ==result_size;
      size_ok &= exp_if_false.size() ==1 || exp_if_false.size() ==result_size;
      CPPAD_VALVECTOR_ASSERT_KNOWN(
         size_ok,
         "argument sizes do not agree in conditional expression"
      );
      //
      // result
      valvector result;
      result.resize(result_size);
      //
      // inc_left, inc_right, inc_true, inc_false
      size_t inc_left  = size_t( left.size() != 1 );
      size_t inc_right = size_t( right.size() != 1 );
      size_t inc_true  = size_t( exp_if_true.size() != 1 );
      size_t inc_false = size_t( exp_if_false.size() != 1 );
      //
      // index_left, index_right, index_true, index_false
      size_t index_left  = 0;
      size_t index_right = 0;
      size_t index_true  = 0;
      size_t index_false = 0;
      for(size_t index_result = 0; index_result < result_size; ++index_result)
      {  switch( cop )
         {
            case CompareLt:
            if( left.vec_[index_left] < right.vec_[index_right] )
               result.vec_[index_result] = exp_if_true.vec_[index_true];
            else
               result.vec_[index_result] = exp_if_false.vec_[index_false];;
            break;

            case CompareLe:
            if( left.vec_[index_left] <= right.vec_[index_right] )
               result.vec_[index_result] = exp_if_true.vec_[index_true];
            else
               result.vec_[index_result] = exp_if_false.vec_[index_false];;
            break;

            case CompareEq:
            if( left.vec_[index_left] == right.vec_[index_right] )
               result.vec_[index_result] = exp_if_true.vec_[index_true];
            else
               result.vec_[index_result] = exp_if_false.vec_[index_false];;
            break;

            case CompareGe:
            if( left.vec_[index_left] >= right.vec_[index_right] )
               result.vec_[index_result] = exp_if_true.vec_[index_true];
            else
               result.vec_[index_result] = exp_if_false.vec_[index_false];;
            break;

            case CompareGt:
            if( left.vec_[index_left] > right.vec_[index_right] )
               result.vec_[index_result] = exp_if_true.vec_[index_true];
            else
               result.vec_[index_result] = exp_if_false.vec_[index_false];;
            break;

            default:
            assert(false);
            result.vec_[index_result] = exp_if_true.vec_[index_true];
         }
         // index_left, index_right, index_true, index_false
         index_left  += inc_left;
         index_right += inc_right;
         index_true  += inc_true;
         index_false += inc_false;
      }
      // result
      return result;
   }
   CPPAD_COND_EXP_REL(valvector)
}
// ===========================================================================

# undef CPPAD_VALVECTOR_ASSERT_KNOWN
# undef CPPAD_VALVECTOR_UNARY_NOT_AVAILABLE
# undef CPPAD_VALVECTOR_STD_MATH_MEMBER
# undef CPPAD_VALVECTOR_MEMBER2FUNCTION
# undef CPPAD_VALVECTOR_BINARY_NUMERIC_OP


# endif
