#include <windows.h>
#include <stdio.h>
#include <stdint.h>

static DWORD last_exception;

static int record_exception(DWORD code)
{
    last_exception = code;
    return EXCEPTION_EXECUTE_HANDLER;
}

static void print_result(const char *name, char *ret, char *base)
{
    if (!ret)
    {
        printf("%s: returned NULL\n", name);
        return;
    }

    printf("%s: returned %p, delta_from_str=%lld\n", name, (void *)ret,
           (long long)((intptr_t)(uintptr_t)ret - (intptr_t)(uintptr_t)base));
}

int main(void)
{
    char str[] = "ab";
    char *ret;

    last_exception = 0;
    __try
    {
        ret = CharPrevA(NULL, str + 1);
        print_result("CharPrevA", ret, str);
    }
    __except (record_exception(GetExceptionCode()))
    {
        printf("CharPrevA: exception 0x%08lx\n", (unsigned long)last_exception);
    }

    last_exception = 0;
    __try
    {
        ret = CharPrevExA(CP_ACP, NULL, str + 1, 0);
        print_result("CharPrevExA", ret, str);
    }
    __except (record_exception(GetExceptionCode()))
    {
        printf("CharPrevExA: exception 0x%08lx\n", (unsigned long)last_exception);
    }

    return 0;
}
