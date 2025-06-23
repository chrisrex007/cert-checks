// Test cases for EXP00-C check

// BAD: Macro without proper parentheses
#define BAD_SQUARE(x) x * x
#define BAD_ADD(a, b) a + b
#define bad(x) (x)*(x) 

// GOOD: Macro with proper parentheses
#define GOOD_SQUARE(x) ((x) * (x))
#define GOOD_ADD(a, b) ((a) + (b))

// BAD: Complex expression without outer parentheses
#define BAD_COMPLEX(x, y) x * y + 1

// GOOD: Complex expression with outer parentheses
#define GOOD_COMPLEX(x, y) ((x) * (y) + 1)

int main() {
    int a = 5;
    int b = 3;

    // These will demonstrate the problems
    int result1 = BAD_SQUARE(a + 1);    // Expands to: a + 1 * a + 1
    int result2 = GOOD_SQUARE(a + 1);   // Expands to: ((a + 1) * (a + 1))
    (void)result1;
    (void)result2;
    return 0;
}
