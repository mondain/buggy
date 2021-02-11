
#include <iostream>

#include "main.h"

Main::Main()
{
    data = (uint8_t*) malloc(1024);
}

void Main::open()
{
	std::cout << "Main open" << std::endl;
}

void Main::close()
{
	std::cout << "Main close" << std::endl;
    free(data);
	std::cout << "Main closed" << std::endl;
}

Main::~Main() {
	std::cout << "Main destruct" << std::endl;
}

MainContext::MainContext()
{
}

Main* MainContext::NewMain()
{
    Main *m = new Main();
    if (m != 0) {
	    std::cout << "Main new: " << (uintptr_t) m << std::endl;
        return m;
    }
    return 0;
}

Main* MainContext::GetMain(long id)
{
    uintptr_t locate = (uintptr_t) id;
    Main *m = (Main *) locate;
    if (m != 0) {
        return m;
    }
    return 0;
}

long MainContext::RemoveMain(long id)
{
	std::cout << "Main remove: " << id << std::endl;
    uintptr_t locate = (uintptr_t) id;
    Main *m = (Main *) locate;
    if (m != 0) {
        m->close();
        delete m;
    } else {
        std::cerr << "Main not found at remove" << std::endl;
    }
    return 0;
}

MainContext::~MainContext()
{
}

jlong JNICALL Java_JavaMain_open(JNIEnv *env, jobject obj)
{
    jlong ret = 0;
    Main *m = main_ctx.NewMain();
    if (m != 0) {
        m->open();
        m->selfId = (uintptr_t) m;
        ret = m->selfId;
        std::cout << "JMain opened: " << (uintptr_t) ret << std::endl;
    }
    return ret;
}

void JNICALL Java_JavaMain_close(JNIEnv *env, jobject obj, jlong id)
{
	std::cout << "JMain close: " << id << std::endl;
    main_ctx.RemoveMain(id);
	std::cout << "JMain closed" << std::endl;
}
