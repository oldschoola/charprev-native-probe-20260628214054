#include <windows.h>
#include <stdio.h>
#include <stdint.h>

static DWORD last_exception;

static int record_exception(DWORD code)
{
    last_exception = code;
    return EXCEPTION_EXECUTE_HANDLER;
}

static long long delta_from(char *ptr, char *base)
{
    return (long long)((intptr_t)(uintptr_t)ptr - (intptr_t)(uintptr_t)base);
}

static void print_result(const char *name, char *ret, char *base)
{
    if (!ret)
    {
        printf("%s: returned NULL\n", name);
        return;
    }

    printf("%s: returned %p, delta_from_base=%lld\n", name, (void *)ret, delta_from(ret, base));
}

static void probe_CharPrevA(const char *name, char *start, char *current, char *base)
{
    char *ret;

    printf("%s: start_delta=%lld current_delta=%lld\n", name,
           start ? delta_from(start, base) : -9999, current ? delta_from(current, base) : -9999);

    last_exception = 0;
    __try
    {
        ret = CharPrevA(start, current);
        print_result(name, ret, base);
    }
    __except (record_exception(GetExceptionCode()))
    {
        printf("%s: exception 0x%08lx\n", name, (unsigned long)last_exception);
    }
}

static void probe_CharPrevExA(const char *name, WORD cp, char *start, char *current, char *base)
{
    char *ret;

    printf("%s: cp=%u start_delta=%lld current_delta=%lld\n", name, cp,
           start ? delta_from(start, base) : -9999, current ? delta_from(current, base) : -9999);

    last_exception = 0;
    __try
    {
        ret = CharPrevExA(cp, start, current, 0);
        print_result(name, ret, base);
    }
    __except (record_exception(GetExceptionCode()))
    {
        printf("%s: exception 0x%08lx\n", name, (unsigned long)last_exception);
    }
}

int main(void)
{
    char ascii[] = "abc";
    char dbcs[] = { (char)0x82, (char)0xa0, 'Z', 0 }; /* CP932: one 2-byte char, then Z */

    probe_CharPrevA("CharPrevA ascii valid", ascii, ascii + 2, ascii);
    probe_CharPrevA("CharPrevA ascii null start +1", NULL, ascii + 1, ascii);
    probe_CharPrevA("CharPrevA ascii null start +2", NULL, ascii + 2, ascii);
    probe_CharPrevA("CharPrevA ascii null start +0", NULL, ascii, ascii);

    probe_CharPrevExA("CharPrevExA ACP ascii valid", CP_ACP, ascii, ascii + 2, ascii);
    probe_CharPrevExA("CharPrevExA ACP ascii null start +1", CP_ACP, NULL, ascii + 1, ascii);
    probe_CharPrevExA("CharPrevExA ACP ascii null start +2", CP_ACP, NULL, ascii + 2, ascii);
    probe_CharPrevExA("CharPrevExA ACP ascii null start +0", CP_ACP, NULL, ascii, ascii);

    probe_CharPrevExA("CharPrevExA 932 dbcs valid", 932, dbcs, dbcs + 2, dbcs);
    probe_CharPrevExA("CharPrevExA 932 dbcs null start +2", 932, NULL, dbcs + 2, dbcs);

    return 0;
}
