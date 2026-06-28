#include <windows.h>
#include <stdio.h>

static unsigned int failures;

static void check_ptr(const char *name, char *got, char *expected)
{
    if (got != expected)
    {
        printf("FAIL %s: got %p, expected %p\n", name, (void *)got, (void *)expected);
        failures++;
    }
    else
    {
        printf("PASS %s: got %p\n", name, (void *)got);
    }
}

int main(void)
{
    char str[] = "abc";
    char *ret;

    ret = CharPrevA(str, str + 2);
    check_ptr("CharPrevA(str, str + 2)", ret, str + 1);

    ret = CharPrevA(str, str);
    check_ptr("CharPrevA(str, str)", ret, str);

    ret = CharPrevA(NULL, str + 1);
    check_ptr("CharPrevA(NULL, str + 1)", ret, str);

    ret = CharPrevA(NULL, str + 2);
    check_ptr("CharPrevA(NULL, str + 2)", ret, str + 1);

    ret = CharPrevExA(CP_ACP, NULL, str + 1, 0);
    check_ptr("CharPrevExA(CP_ACP, NULL, str + 1, 0)", ret, str);

    ret = CharPrevExA(CP_ACP, NULL, str + 2, 0);
    check_ptr("CharPrevExA(CP_ACP, NULL, str + 2, 0)", ret, str + 1);

    if (failures)
    {
        printf("v3 CharPrev native behavior check failed: %u failure(s)\n", failures);
        return 1;
    }

    printf("v3 CharPrev native behavior check passed\n");
    return 0;
}
