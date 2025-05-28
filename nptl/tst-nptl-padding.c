/* Downstream-only test for verifying that fields that have been
   relocated into struct pthread padding actually use the padding.

   At present, only rtld_catch (downstream: rtld_catch_f) has been
   placed into padding.  */

#include <descr.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <support/check.h>

static int
do_test (void)
{
  struct pthread descr;

  /* Mark the entire descriptor as used.  */
  memset (&descr, 0xff, sizeof (descr));

  /* Mark the padding as unused.  */
#ifdef __x86_64__
  /* Special case: Usable padding is in the header.   */
  memset (&descr.header.__padding, 0, sizeof (descr.header.__padding));
#else
  /* The padding should be directly adjacent to the first real
     struct field.  */
  TEST_COMPARE (sizeof (descr.__padding), offsetof (struct pthread, list));

  /* Clear the unused tail of the padding.  */
  {
    char *base = (char *) &descr;
    char *end_of_header = base + sizeof (descr.header);
    char *end_of_padding = base + sizeof (descr.__padding);
    memset (end_of_header, 0, end_of_padding - end_of_header);
  }
#endif

  /* These fields are not in padding and should remain marked as used.   */
  TEST_COMPARE (descr.header.gscope_flag, -1);
  TEST_COMPARE ((intptr_t) descr.list.next, -1);
  TEST_COMPARE ((intptr_t) descr.list.prev, -1);

  /* But this field remains in padding.  */
  TEST_COMPARE ((intptr_t) descr.rtld_catch_f, 0);

  /* Write to all padding-relocated fields below to show that they
     have independent locations.  */
  struct rtld_catch *rtld_catch_dummy = (void *) "rtld_catch_dummy";
  descr.rtld_catch_f = rtld_catch_dummy;

  TEST_VERIFY (descr.rtld_catch_f == rtld_catch_dummy);

  return 0;
}

#include <support/test-driver.c>
