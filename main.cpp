#include <iostream>
#include <jni.h>

int main() {
    // https://docs.oracle.com/javase/8/docs/technotes/guides/jni/spec/invocation.html
    JavaVM *jvm;
    JNIEnv *env;
    JavaVMInitArgs vm_args;
    auto *options = new JavaVMOption[1];
    options[0].optionString = (char *) "-Djava.class.path=";
    vm_args.version = JNI_VERSION_10;
    vm_args.nOptions = 1;
    vm_args.options = options;
    vm_args.ignoreUnrecognized = false;
    std::cout << "Starting VM..." << std::endl;
    JNI_CreateJavaVM(&jvm, (void **) &env, &vm_args);
    std::cout << "VM started" << std::endl;
    delete[] options;
    jclass cls = env->FindClass("Main");
    jmethodID mid = env->GetStaticMethodID(cls, "test", "(I)V");
    env->CallStaticVoidMethod(cls, mid, 100);
    jvm->DestroyJavaVM();
    return 0;
}