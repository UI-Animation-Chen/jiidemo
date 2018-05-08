/**
 * the invocation interface demo.
 * cli: g++ -I<headers path> -L<jvm library path> main.cpp -o main -ljvm
 * NOTE: the last part: -ljvm.
 * jvm library path: java_home/jre/lib/amd64/server/
 * libjvm.so: java_home/jre/lib/amd64/server/libjvm.so.
 */

#include<iostream>
#include<cstdlib>
#include<pthread.h>
#include<unistd.h>

#include<jni.h>

#define USER_CLASSPATH "." /* where class is */

void printArgCV(int argc, const char * argv[]) {
	std::cout << "argc: " << argc << "\n";
	for (int i = 0; i < argc; i++) {
		std::cout << "argv[" << i << "]: \"" << argv[i] << "\".\n";
	}
}

/**
 * do not need any shared library if use RegisterNatives.
 */
void JNICALL javaCall(JNIEnv *env, jobject thiz) {
	std::cout << "javaCall has executed\n";
}
/**
 * register a native method(here is the javaCall) to jvm.
 */
void registerJavaCall(JNIEnv *env, jclass javaCls) {
	JNINativeMethod nm;
	char cstr[] = "javaCall";
	nm.name = cstr;
	char cstr1[] = "()V";
	nm.signature = cstr1;
	nm.fnPtr = (void*)javaCall;
	env->RegisterNatives(javaCls, &nm, 1);
}

void * callJava1(void * arg) {
    JavaVM *jvm;
    jvm = (JavaVM *)arg;
    JNIEnv *env;
    jint result;
    result = jvm->AttachCurrentThread((void**)&env, NULL);
    if (result < 0) {
        return NULL;
    }
    
    jclass cls;
    cls = env->FindClass("Hello");
    if (cls == NULL) {
        goto detach;
    }
    jmethodID mid;
    mid = env->GetStaticMethodID(cls, "showThread", "()V");
    if (mid == NULL) {
        goto detach;
    }
    env->CallStaticVoidMethod(cls, mid, NULL);
    
detach:
    if (env->ExceptionOccurred()) {
        env->ExceptionDescribe();
    }
    jvm->DetachCurrentThread();
    
    return NULL;
}

/**
 * create native thread, then attach to jvm.
 */
void createNativeThread(pthread_t pthreads[], int len, JavaVM *jvm) {
    for (int i = 0; i < len; i++) {
        pthread_create(&pthreads[i], NULL, callJava1, jvm);
        //pthread_join(pthreads[i], NULL);
    }
}

void invokeJavaMain() {
	JavaVMInitArgs vm_args;
	vm_args.version = JNI_VERSION_1_6;
	vm_args.nOptions = 1;
	vm_args.ignoreUnrecognized = JNI_TRUE;
	JavaVMOption options[1];
	char carr[] = "-Djava.class.path=" USER_CLASSPATH;
	options[0].optionString = carr;
	vm_args.options = options;
	
	JNIEnv *env;
	JavaVM *jvm;
	jint result;
	/**
	 * when the JNI_CreateJavaVM function returns successfully, the
	 * current native thread has bootstrapped itself into the java
	 * virtual machine.(Is this native thread the java main thread?)
	 */
	result = JNI_CreateJavaVM(&jvm, (void**)&env, &vm_args);
	if (result < 0) {
		std::cout << "can't create java vm.\n";
		std::exit(1);
	}

	jclass javaCls;
	javaCls = env->FindClass("Hello");
	if (javaCls == NULL) {
		goto destroy;
	}

	registerJavaCall(env, javaCls);

	jmethodID mid;
	mid = env->GetStaticMethodID(javaCls, "main", "([Ljava/lang/String;)V");
	if (mid == NULL) {
		goto destroy;
	}

	jstring jstr;
	jstr = env->NewStringUTF("argument from c.");
	if (jstr == NULL) {
		goto destroy;
	}

	jclass strCls;
	strCls = env->FindClass("java/lang/String");

	jobjectArray args;
	args = env->NewObjectArray(1, strCls, jstr);
	if (args == NULL) {
		goto destroy;
	}
    
    pthread_t pthreads[3];
    createNativeThread(pthreads, 3, jvm);

    // this method will return after java main method returned.
	env->CallStaticVoidMethod(javaCls, mid, args);

	std::cout << "below java-main in cpp\n";
    
    sleep(1);
	
destroy:
	if (env->ExceptionOccurred()) {
		env->ExceptionDescribe();
	}
	jvm->DestroyJavaVM();
}

int main(int argc, const char * argv[]) {
	invokeJavaMain();
	//printArgCV(argc, argv);
	return 0;
}






