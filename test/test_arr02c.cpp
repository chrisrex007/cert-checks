// Test cases for ARR02-C check

// BAD: Array without explicit bound
int bad_array[] = { 1, 2, 3, 4, 5 };

// GOOD: Array with explicit bound
int good_array[5] = { 1, 2, 3, 4, 5 };

// BAD: String array without bound
char bad_string[] = "hello";

// GOOD: String array with bound
char good_string[6] = "hello";

int main() {
    // BAD: Local array without bound
    int local_bad[] = { 10, 20, 30 };

    // GOOD: Local array with bound
    int local_good[3] = { 10, 20, 30 };

    return 0;
}
