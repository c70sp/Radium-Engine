# Reactor++

### A C++ nuclear reactor simulation.

Current status:
<br>
Finished the rendering queue thingy. Now works as intended.
Mesh3D's are created via the reactor script,
from there they are being pushed to into the render queue of app.
In the reactors main loop the apps draw methode gets called.
The draw methode will call the preDraw methode and then render one object after the other from the draw queue.
Following that, it will clear the queue, swaps buffers and polls for events.

Should maybe push to git for version control.

Window has no more border (decoration, this can be toggled via glfwWindowHint), and also has a fixed size now.



TODO:
- More performance by making circles with max area methode, not triangle fan (https://www.humus.name/index.php?page=Comments&ID=228)






<br><br><br><br>

### How to do shit / Documentation of API

- mesh->setZ(int zLayer), does from -100 to 100, meaning a total of 200 possible zLayers,
  could be increased by deviding input by 1000 to get dec. as well