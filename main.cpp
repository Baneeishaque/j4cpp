#include <iostream>
#include <jni.h>
#include <windows.h>

typedef jint(JNICALL *CreateJavaVM)(JavaVM **, void **, void *);

void loadJvmDll(CreateJavaVM *createJavaVM) {
    const char *jvmDLLPath = "jre/bin/server/jvm.dll";
    HINSTANCE jvmPtr = LoadLibrary(jvmDLLPath);
    if (jvmPtr == nullptr) {
        DWORD errorCode = GetLastError();
        if (errorCode == 126) {

            // "The specified module could not be found."
            // load msvcr100.dll from the bundled JRE, then try again
            std::cout << "Failed to load jvm.dll. Trying to load msvcr100.dll first ..." << std::endl;

            HINSTANCE hinstVCR = LoadLibrary(TEXT("jre\\bin\\msvcr100.dll"));
            if (hinstVCR != nullptr) {
                jvmPtr = LoadLibrary(jvmDLLPath);
            }
        }
        printf("Error code: %d\n", errorCode);
    }

    *createJavaVM = (CreateJavaVM) GetProcAddress(jvmPtr, "JNI_CreateJavaVM");
    if (*createJavaVM == nullptr) {
        std::printf("obtain JNI_CreateJavaVM address\n");
        return;
    }

}

int main() {
    std::cout << "Loading JVM DLL" << std::endl;
    CreateJavaVM createJavaVM;
    loadJvmDll(&createJavaVM);
    std::cout << "Creating JVM Instance" << std::endl;
    // https://docs.oracle.com/javase/10/docs/specs/jni/invocation.html
    JavaVM *jvm;
    JNIEnv *env;
    JavaVMInitArgs vm_args;
    auto *options = new JavaVMOption[4];
    options[0].optionString = (char *) "-Djava.compiler=NONE";
    options[1].optionString = (char *) "-Djava.class.path=c:\\dev\\j4cpp";
    options[2].optionString = (char *) "-Djava.library.path=.";
    options[3].optionString = (char *) "-verbose:jni";
    vm_args.version = JNI_VERSION_1_8;
    vm_args.nOptions = 4;
    vm_args.options = options;
    vm_args.ignoreUnrecognized = false;
    std::cout << "Starting VM..." << std::endl;
    createJavaVM(&jvm, (void **) &env, &vm_args);
    std::cout << "VM started" << std::endl;
    delete[] options;
    std::printf("Loading class\n");
    jclass class_System = env->FindClass("java/lang/System");
    if (class_System == nullptr) {
        return 61;
    }
    jfieldID field_System_out = env->GetStaticFieldID(class_System, "out", "Ljava/io/PrintStream;");
    if (field_System_out == nullptr) {
        return 62;
    }
    jobject object_System_out = env->GetStaticObjectField(class_System, field_System_out);
    if (object_System_out == nullptr) {
        return 63;
    }
    jclass class_PrintStream = env->FindClass("java/io/PrintStream");
    if (class_PrintStream == nullptr) {
        return 64;
    }
    jmethodID method_println = env->GetMethodID(class_PrintStream, "println", "(I)V");
    if (method_println == nullptr) {
        return 65;
    }
    env->CallObjectMethod(object_System_out, method_println, 777);
    std::cout << "Method Called" << std::endl;
    jvm->DestroyJavaVM();
    return 0;
}