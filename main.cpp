#include <iostream>
#include "Raycaster.h"

using namespace std;

int main()
{
    Raycaster raycaster(1024, 768);
    while (true)
    {
        raycaster.RunFrame();
    }
}
