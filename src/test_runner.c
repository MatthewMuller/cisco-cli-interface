#include "test_framework.h"

int main(int argc, char *argv[]) {
    (void)argc; // Unused parameter
    (void)argv; // Unused parameter
    
    printf("Cisco CLI Interface - Unit Test Suite\n");
    printf("=====================================\n\n");
    
    run_all_tests();
    
    // Return exit code based on test results
    if (test_ctx.failed_tests > 0) {
        return 1; // Exit with error if any tests failed
    }
    
    return 0; // Exit successfully if all tests passed
}
