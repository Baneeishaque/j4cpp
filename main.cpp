#include <iostream>
#include <jni.h>

int main() {
    // https://docs.oracle.com/javase/10/docs/specs/jni/invocation.html
    JavaVM *jvm;       /* denotes a Java VM */
    JNIEnv *env;       /* pointer to native method interface */
    JavaVMInitArgs vm_args; /* JDK/JRE 10 VM initialization arguments */
    JavaVMOption *options = new JavaVMOption[1];
    options[0].optionString = (char*) "-Djava.compiler=NONE";           /* disable JIT */
    options[1].optionString = (char*) "-Djava.class.path=c:/dev/j4cpp"; /* user classes */
    options[2].optionString = (char*) "-Djava.library.path=.";  /* set native library path */
    options[3].optionString = (char*) "-verbose:jni";                   /* print JNI-related messages */
    vm_args.version = JNI_VERSION_1_8;
    vm_args.nOptions = 1;
    vm_args.options = options;
    vm_args.ignoreUnrecognized = false;
    std::cout << "Starting VM..." << std::endl;
    JNI_CreateJavaVM(&jvm, (void **) &env, &vm_args);
    std::cout << "VM started" << std::endl;
    delete options;
/* invoke the Main.test method using the JNI */
    jclass cls = env->FindClass("J4Cpp");
    jmethodID mid = env->GetStaticMethodID(cls, "test", "()V");
    env->CallStaticVoidMethod(cls, mid, 100);
/* We are done. */
    jvm->DestroyJavaVM();
    return 0;
}