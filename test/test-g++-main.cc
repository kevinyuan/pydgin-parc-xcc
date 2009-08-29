//========================================================================
// Top-level test harness
//========================================================================
// Each source C++ file will compile into an object file with one global
// entry point called test. We just call the test function from main and
// return the result as the exit status.

int test();

int main( int argc, char* argv[] )
{
  return test();
}
