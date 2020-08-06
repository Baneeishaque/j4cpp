#include <jni.h>
#include <windows.h>

typedef int(*Pow2)(int, const char *);

int main() {
    HMODULE library = LoadLibrary("j4cs.dll");
    auto pow2 = (Pow2) GetProcAddress(library, "pow2");
    const char *jreHome = "jre";
    int result = pow2(12, jreHome);
    printf("Result: %d\n", result);
    return 0;
}