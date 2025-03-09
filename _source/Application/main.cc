/*------------------------------------------------------------------------------------------------------------
* Created by elecfrog on 2025/3/9.
* Copyright (c) 2025 East10 All rights reserved.
------------------------------------------------------------------------------------------------------------ */

#include "Runtime/Engine/Engine.h"
#include "Core/Log/LogSystem.h"

int main() try {

    Sparrow::g_Engine.Run();
}
catch (const std::exception &e) {
    LOG_FATAL(Sparrow::LogModule::App, " Application Fatal: {0}", e.what());
    return EXIT_FAILURE;
}
