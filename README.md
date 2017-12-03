# HVN3 Engine
*A WIP 2D game engine*

HVN3 Engine is a 2D game engine built around [Allegro 5](https://github.com/liballeg/allegro5) that offers many abstractions to ease game development. With a focus on modularity and freedom, engine components can easily be customized or swapped-out with user-created implementations. 

For those that just want to make a game without worrying about all the work that goes on in the background, getting started is extremely simple! The following code is enough to create a display and set up everything you need:

```cpp
#include "hvn3/hvn3.h"
using namespace hvn3;

int main(int argc, char *argv[]) {

  // Create the game manager object, which offers access to scenes, resources, and more.
  GameManager game(argc, argv);
  
  // Run the game loop.
  game.Loop();
  
  return 0;
  
}
```

The engine is currently a work-in-progress, and there is much more work to be done. While the implementation is subject to change, the engine itself is very stable and usuable in its current state.

Putting this engine together has been a valuable learning experience, and my abilities have improved dramatically throughout the time I've spent working on it. As a result, there is some degree of inconsistency between older and newer code that I'm actively working to smooth out.
