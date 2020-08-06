#include <iostream>
#include <jni.h>
#include <windows.h>
#include <string>
#include <vector>

using namespace std;

typedef jint(JNICALL *CreateJavaVM)(JavaVM **, void **, void *);

int testCustomJavaMethod(JNIEnv *env, const char *classPath1, const char *className, const char *methodName);

void loadJvmDll(CreateJavaVM *createJavaVM);

void testBuiltinMethod(JNIEnv *env);

void createJVM(JavaVM *&jvm, JNIEnv *&env);

int main() {
    JavaVM *jvm;
    JNIEnv *env;

    createJVM(jvm, env);
    testBuiltinMethod(env);
    testCustomJavaMethod(env, "classes", "J4Cpp", "test");

    jvm->DestroyJavaVM();
    return 0;
}

void createJVM(JavaVM *&jvm, JNIEnv *&env) {
    cout << "Loading JVM DLL" << endl;
    CreateJavaVM createJavaVM;
    loadJvmDll(&createJavaVM);
    cout << "Creating JVM Instance" << endl;
    // https://docs.oracle.com/javase/10/docs/specs/jni/invocation.html
    JavaVMInitArgs vm_args;
    auto *options = new JavaVMOption[3];
    options[0].optionString = (char *) "-Djava.class.path=";
    options[1].optionString = (char *) "-Djava.library.path=";
    options[2].optionString = (char *) "-verbose:jni";
    vm_args.version = JNI_VERSION_1_8;
    vm_args.nOptions = 3;
    vm_args.options = options;
    vm_args.ignoreUnrecognized = false;
    cout << "Starting VM..." << endl;
    createJavaVM(&jvm, (void **) &env, &vm_args);
    cout << "VM started" << endl;

    delete[] options;
}

void testBuiltinMethod(JNIEnv *env) {
    std::printf("Loading class\n");
    jclass class_System = env->FindClass("java/lang/System");
    jfieldID field_System_out = env->GetStaticFieldID(class_System, "out", "Ljava/io/PrintStream;");
    jobject object_System_out = env->GetStaticObjectField(class_System, field_System_out);
    jclass class_PrintStream = env->FindClass("java/io/PrintStream");
    jmethodID method_println = env->GetMethodID(class_PrintStream, "println", "(I)V");
    env->CallObjectMethod(object_System_out, method_println, 777);
    std::cout << "Method Called" << std::endl;
}

int testCustomJavaMethod(JNIEnv *env, const char *classPath1, const char *className, const char *methodName) {
    vector<string> classPath;
    classPath.emplace_back(classPath1);

    jclass urlClass = env->FindClass("java/net/URL");
    jobjectArray urlArray = env->NewObjectArray(classPath.size(), urlClass, nullptr);

    size_t i = 0;
    for (const string &classPathURL : classPath) {
        jstring urlStr = env->NewStringUTF(classPathURL.c_str());
        jclass fileClass = env->FindClass("java/io/File");
        jmethodID fileCtor = env->GetMethodID(fileClass, "<init>", "(Ljava/lang/String;)V");
        jobject file = env->NewObject(fileClass, fileCtor, urlStr);
        jmethodID toUriMethod = env->GetMethodID(fileClass, "toURI", "()Ljava/net/URI;");
        jobject uri = env->CallObjectMethod(file, toUriMethod);
        jclass uriClass = env->FindClass("java/net/URI");
        jmethodID toUrlMethod = env->GetMethodID(uriClass, "toURL", "()Ljava/net/URL;");
        jobject url = env->CallObjectMethod(uri, toUrlMethod);

        env->SetObjectArrayElement(urlArray, i++, url);
    }

    jclass threadClass = env->FindClass("java/lang/Thread");
    jmethodID threadGetCurrent = env->GetStaticMethodID(threadClass, "currentThread", "()Ljava/lang/Thread;");
    jobject thread = env->CallStaticObjectMethod(threadClass, threadGetCurrent);
    jmethodID threadGetLoader = env->GetMethodID(threadClass, "getContextClassLoader", "()Ljava/lang/ClassLoader;");
    jobject contextClassLoader = env->CallObjectMethod(thread, threadGetLoader);
    jclass urlClassLoaderClass = env->FindClass("java/net/URLClassLoader");
    jmethodID urlClassLoaderCtor = env->GetMethodID(
            urlClassLoaderClass,
            "<init>",
            "([Ljava/net/URL;Ljava/lang/ClassLoader;)V"
    );
    jobject urlClassLoader = env->NewObject(urlClassLoaderClass, urlClassLoaderCtor, urlArray, contextClassLoader);
    jmethodID threadSetLoader = env->GetMethodID(threadClass, "setContextClassLoader", "(Ljava/lang/ClassLoader;)V");
    env->CallVoidMethod(thread, threadSetLoader, urlClassLoader);
    jmethodID loadClass = env->GetMethodID(urlClassLoaderClass, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");
    jstring mainClassNameUTF = env->NewStringUTF(className);
    jobject mainClass = env->CallObjectMethod(urlClassLoader, loadClass, mainClassNameUTF);
    jmethodID mainMethod = env->GetStaticMethodID((jclass) mainClass, methodName, "()V");

    jobjectArray appArgs = env->NewObjectArray(0, env->FindClass("java/lang/String"), nullptr);
    env->CallStaticVoidMethod((jclass) mainClass, mainMethod, appArgs);

    return 0;
}

void loadJvmDll(CreateJavaVM *createJavaVM) {
    const char *jvmDLLPath = "jre/bin/server/jvm.dll";
    HINSTANCE jvmDll = LoadLibrary(jvmDLLPath);
    if (jvmDll == nullptr) {
        DWORD lastErrorCode = GetLastError();
        if (lastErrorCode == 126) {
            // "The specified module could not be found."
            // load msvcr100.dll from the bundled JRE, then try again
            std::cout << "Failed to load jvm.dll. Trying to load msvcr100.dll first ..." << std::endl;

            HINSTANCE hinstVCR = LoadLibrary("jre\\bin\\msvcr100.dll");
            if (hinstVCR != nullptr) {
                jvmDll = LoadLibrary(jvmDLLPath);
            }
        }
        printf("Error: %d\n", lastErrorCode);
    }

    *createJavaVM = (CreateJavaVM) GetProcAddress(jvmDll, "JNI_CreateJavaVM");
}