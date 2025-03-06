#include "App/app.h"

int main()
{
    bool suc = OGLRenderer::App::App::GetInstance().Init();
    if (!suc)
        return -1;

    OGLRenderer::App::App::GetInstance().Run();
    return 0;
}