#include <iostream>
#include "Raycaster.h"

using namespace std;

int main()
{
    Raycaster raycaster(1024, 768);
    raycaster.RunGameLoop();
    return 0;
}
