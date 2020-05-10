#include "../include/Engine.h"

int main(int argc, char const *argv[]) {
    Engine engine = Engine::GetInstance();
    engine.Init();
    engine.StartRendering();

    return 0;
}