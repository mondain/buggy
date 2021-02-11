#pragma once

#include <jni.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

class Main
{
    public:
        uintptr_t selfId;

        uint8_t *data;

        Main();
        virtual ~Main();
        void open();
        void close();
};

class MainContext
{
    public:
        MainContext();
        Main* NewMain();
        Main* GetMain(long id);
        long RemoveMain(long id);
        ~MainContext();
};

static MainContext main_ctx;

#ifdef __cplusplus
   extern "C" {
#endif

JNIEXPORT jlong JNICALL Java_JavaMain_open(JNIEnv *env, jobject obj);

JNIEXPORT void JNICALL Java_JavaMain_close(JNIEnv *env, jobject obj, jlong id);

#ifdef __cplusplus
   }
#endif