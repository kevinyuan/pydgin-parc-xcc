//========================================================================
// Static Initializer Test
//========================================================================
// Static initializers rely on correctly setting up the linker script so
// we do a quick test here and make sure it works.

//------------------------------------------------------------------------
// Foo
//------------------------------------------------------------------------

class Foo
{
 public:

  Foo( int val )
  {
    m_val = val;
  }

  int get_value() const
  {
    return m_val;
  }
 
 private: 
  int m_val;

};

__attribute__ ((noinline))
bool operator!=( const Foo& foo_a, const Foo& foo_b )
{
  return ( foo_a.get_value() != foo_b.get_value() );
}

//------------------------------------------------------------------------
// Free function for initialization
//------------------------------------------------------------------------

__attribute__ ((noinline))
Foo mk_foo( int val )
{
  return Foo(val);
}

//------------------------------------------------------------------------
// Initialize some data
//------------------------------------------------------------------------

Foo s_foo_0(42);
Foo s_foo_1 = mk_foo(13);

//------------------------------------------------------------------------
// test
//------------------------------------------------------------------------

int test()
{
  // Error exit status (will indicate which test failed)

  int error_code = 0;

  // Test static initialization

  Foo foo_0(42);

  error_code++;
  if ( s_foo_0 != foo_0 )
    return error_code;

  // Test static initialization with function

  Foo foo_1(13);

  error_code++;
  if ( s_foo_1 != foo_1 )
    return error_code;

  // If all tests passed return zero

  return 0;
}

