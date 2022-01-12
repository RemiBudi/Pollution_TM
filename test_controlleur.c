#include "CUnit.h"
#include "Basic.h"
#include "Automated.h"
#include "Console.h"

#include "controlleur.h"

#include <stdio.h>  // for printf

/* Test Suite setup and cleanup functions: */

int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }

/************* Test case functions ****************/

void test_ecartVal(void)
{
  CU_ASSERT(CU_TRUE);
  CU_ASSERT_NOT_EQUAL(2, -1);
  CU_ASSERT(ecartVal(15,10));
  CU_ASSERT(!ecartVal(12,10));     
}

void test_detectionPollution(void){


  CU_ASSERT(detectionPollution(18,16,7));
  CU_ASSERT(!detectionPollution(10,5,7));

}

void test_send(void){

  detectionPollution(18,16,7);
  CU_ASSERT(notif_sent);


}

/************* Test Runner Code goes here **************/

int main ( void )
{
   CU_pSuite pSuite = NULL;

   /* initialize the CUnit test registry */
   if ( CUE_SUCCESS != CU_initialize_registry() )
      return CU_get_error();

   /* add a suite to the registry */
   pSuite = CU_add_suite( "max_test_suite", init_suite, clean_suite );
   if ( NULL == pSuite ) {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* add the tests to the suite */
   if ( (NULL == CU_add_test(pSuite, "Exigence 3 (fonction ecartVal): *******", test_ecartVal))
     || (NULL == CU_add_test(pSuite, "Exigence 6 (fonction detectionPollution): ******", test_ecartVal))
     || (NULL == CU_add_test(pSuite, "Exigence 1 (fonction detectionPollution): ******", test_send))
     )

   {
      CU_cleanup_registry();
      return CU_get_error();
   }

   // Run all tests using the basic interface
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   printf("\n");
   CU_basic_show_failures(CU_get_failure_list());
   printf("\n\n");
/*
   // Run all tests using the automated interface
   CU_automated_run_tests();
   CU_list_tests_to_file();

   // Run all tests using the console interface
   CU_console_run_tests();
*/
   /* Clean up registry and return */
   CU_cleanup_registry();
   return CU_get_error();
}