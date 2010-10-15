void Init_patched_include(void);
void Init_real_include_imp(void);

void
Init_real_include()
{
  Init_real_include_imp();
  Init_patched_include();
}
