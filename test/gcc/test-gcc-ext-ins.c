//========================================================================
// Test bit field insertion and extraction
//========================================================================
// We have removed support for the mips ins/ext instructions and
// unaligned load/store instructions since maven does not implement them
// for now. This test will make sure that the compiler can still
// generate correct code for the type of bit field manipulations which
// would otherwise use ins/ext or unaligned load/stores. Notice that we
// use a gcc attribute specifier to prevent the compiler from completely
// optimizing away the helper functions. With a mips target the extract
// field functions seem to generate ext instructions and the packed
// struct initializer seems to generate ins instructions. 

//------------------------------------------------------------------------
// Extract functions
//------------------------------------------------------------------------

__attribute__ ((noinline))
int extract_field_27to16( int word )
{
  return (word >> 16) & 0x00000fff;
}
 
__attribute__ ((noinline)) 
int extract_field_31to28( int word )
{
  return (word >> 28) & 0x0000000f;
}
 
__attribute__ ((noinline)) 
int extract_field_19to13( int word )
{
  return (word >> 13) & 0x0000003f;
}

//------------------------------------------------------------------------
// Insertion functions
//------------------------------------------------------------------------

__attribute__ ((noinline))
int insert_field_27to16( int word, int field )
{
  return (word & (~(0x0000fff << 16)) | (field << 16));
}
 
__attribute__ ((noinline)) 
int insert_field_31to28( int word, int field )
{
  return (word & (~(0x000000f << 28)) | (field << 28));
}
 
__attribute__ ((noinline)) 
int insert_field_19to13( int word, int field )
{
  return (word & (~(0x000003f << 13)) | (field << 13));
}

//------------------------------------------------------------------------
// Packed structure
//------------------------------------------------------------------------

struct PackedStruct
{
  unsigned int field_a : 13;
  unsigned int field_b : 7;
  unsigned int field_c : 9;
};

__attribute__ ((noinline)) 
void init_packed_struct( struct PackedStruct* packed_struct )
{
  packed_struct->field_a = 0xde;
  packed_struct->field_b = 0x3d;
  packed_struct->field_c = 0xbe;
}

__attribute__ ((noinline)) 
int verify_packed_struct( struct PackedStruct* packed_struct )
{
  if ( packed_struct->field_a != 0xde )
    return 0;

  if ( packed_struct->field_b != 0x3d )
    return 0;

  if ( packed_struct->field_c != 0xbe )
    return 0;

  return 1;
}

//------------------------------------------------------------------------
// test
//------------------------------------------------------------------------

int test()
{

  // Error exit status (will indicate which test failed)

  int error_code = 0;

  // Test bit field extraction

  error_code++;
  if ( extract_field_27to16( 0xdeadbeef ) != 0xead )
    return error_code;    

  error_code++;
  if ( extract_field_31to28( 0xdeadbeef ) != 0x00d )
    return error_code;

  error_code++;
  if ( extract_field_19to13( 0xdeadbeef ) != 0x02d )
    return error_code;

  // Test bit field insertion

  error_code++;
  if ( insert_field_27to16( 0xdeadbeef, 0x421 ) != 0xd421beef )
    return error_code;    

  error_code++;
  if ( insert_field_31to28( 0xdeadbeef, 0x003 ) != 0x3eadbeef )
    return error_code;

  error_code++;
  if ( insert_field_19to13( 0xdeadbeef, 0x014 ) != 0xdeaa9eef )
    return error_code;

  // Test with packed structure

  struct PackedStruct packed_struct;

  init_packed_struct( &packed_struct );

  error_code++;
  if ( !verify_packed_struct( &packed_struct ) )
    return error_code;    

  return 0;
}

