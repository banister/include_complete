/* (c) 2010 John Mair (banisterfiend), MIT license */

void Init_patched_include(void);
void Init_include_complete_one(void);

void
Init_include_complete()
{
  Init_include_complete_one();
  Init_patched_include();
}
