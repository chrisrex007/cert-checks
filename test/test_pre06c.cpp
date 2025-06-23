// Test file for PRE06-C check
// This should be a .h file to trigger the check

#include "test_header_bad.h"  // Header without include guard
#include "test_header_good.h" // Header with include guard

int main() {
    return 0;
}
