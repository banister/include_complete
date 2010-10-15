/* (c) 2010 John Mair (banisterfiend), MIT license */

void Init_patched_include(void);
void Init_real_include_one(void);

void
Init_real_include()
{
  Init_real_include_one();
  Init_patched_include();
}
