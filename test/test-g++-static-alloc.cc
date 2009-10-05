//========================================================================
// Static Initializer Test for Arrays Floats
//========================================================================
// Test to make sure static initialization of ints and floats works 
// correctly on arrays 

//------------------------------------------------------------------------
// Transform_float 
//------------------------------------------------------------------------

class Transform_float{
  public:

    float T[2][2];

    // Do not inline this function 
    
    static Transform_float scale(float sx) __attribute__((noinline));
};

Transform_float Transform_float::scale (float sx){
  Transform_float ret;
  // Create local Transform_float Object and do static initialization
  
  ret.T[0][0] = sx;
  return ret;
}

//------------------------------------------------------------------------
// Transform_int
//------------------------------------------------------------------------

class Transform_int{
  public:

    int T[2][2];

    // Do not inline this function 
    
    static Transform_int scale(int sx) __attribute__((noinline));
};

Transform_int Transform_int::scale (int sx){
  Transform_int ret;
  // Create local Transform_int Object and do static initialization
  
  ret.T[0][0] = sx;
  return ret;
}

int test(){
  // First test to make sure ints work

  Transform_int U_int = Transform_int::scale(1);
  if(U_int.T[0][0] != 1)
    return 1;

  // Test to make sure floats work correctly

  Transform_float U = Transform_float::scale(1.0);
  if(U.T[0][0] != 1.0)
    return 1;
  return 0;
}
