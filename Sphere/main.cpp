//
// Created by max on 20/04/19.
//

#include <iostream>
#include <common/Window.h>
#include "common/Shader.h"

using namespace std;


int main(int argc, const char **argv)
{
    auto *w = new Window();

    auto *s = new Shader("Quad/Color");
    delete(s);

    delete(w);

    return 0;
}