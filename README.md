# learn2
## 三角形
顶点数组对象：Vertex Array Object，VAO
顶点缓冲对象：Vertex Buffer Object，VBO
元素缓冲对象：Element Buffer Object，EBO 或 索引缓冲对象 Index Buffer Object，IBO
OpenGL中事物是3D的，而屏幕和窗口是2D的，因此OpenGL需要将3D坐标转为2D像素，处理过程即推行渲染管线（graphics pipeline）
图形渲染管线分多个阶段，每个阶段以上一阶段输出作为输入，并且可以并行执行，正因如此当今大多数显卡都有成千上万的小处理核心，它们在GPU上为每一个阶段运行各自的小程序，从而在图形渲染管线中快速处理你的数据。这些小程序叫做着色器(Shader)。
OpenGL着色器使用GLSL。

![Alt text](./images/pipeline.png)

以数组的形式传递3个3D坐标作为图形渲染管线的输入，用来表示一个三角形，这个数组叫做顶点数据(Vertex Data)；顶点数据是一系列顶点的集合。一个顶点(Vertex)是一个3D坐标的数据的集合。而这样一个顶点的数据是用顶点属性(Vertex Attribute)表示的，它可以包含任何我们想用的数据，但是简单起见，我们还是假定每个顶点只由一个3D位置(译注1)和一些颜色值组成的吧。
作出点、三角形、线提示的叫做图元（primitive）：GL_POINTS、GL_TRIANGLES、GL_LINE_STRIP
1. 顶点着色器主要的目的是把3D坐标转为另一种3D坐标，同时顶点着色器允许我们对顶点属性进行一些基本处理。
2. 几何着色器将一组顶点作为输入，这些顶点形成图元，并且能够通过发出新的顶点来形成新的图元来生成其他形状。
3. 图元装配阶段将顶点着色器（或几何着色器）输出的所有顶点作为输入，并将所有的点装配成指定图元的形状，例如两个三角形装配成的矩形。
4. 图元装配阶段的输出会被传入光栅化阶段，这里它会把图元映射为最终屏幕上相应的像素，生成供片段着色器使用的片段。
5. 片段着色器的主要目的是计算一个像素的最终颜色。通常，片段着色器包含3D场景的数据（比如光照、阴影、光的颜色等等），这些数据可以被用来计算最终像素的颜色。
6. 在所有对应颜色值确定以后，最终的对象将会被传到最后一个阶段，我们叫做Alpha测试和混合(Blending)阶段。这个阶段检测片段的对应的深度（和模板）值，用它们来判断这个像素是其它物体的前面还是后面，决定是否应该丢弃。这个阶段也会检查alpha值（alpha值定义了一个物体的透明度）并对物体进行混合(Blend)。所以，即使在片段着色器中计算出来了一个像素输出的颜色，在渲染多个三角形的时候最后的像素颜色也可能完全不同。
在现代OpenGL中，我们必须定义至少一个顶点着色器和一个片段着色器（因为GPU中没有默认的顶点/片段着色器）。

### 在本次绘制三角形的任务中
1. 顶点着色器和片段着色器源码
2. 创建顶点着色器、片段着色器并附加和编译源码
3. 创建着色程序并链接着色器，之后删除着色器
4. 设置顶点输入，创建VAO、VBO并作绑定
5. 在渲染循环中的渲染阶段激活着色程序并进行绘制

## 矩形
一个矩形可以由两个三角形组成
float vertices[] = {
    // 第一个三角形
    0.5f, 0.5f, 0.0f,   // 右上角
    0.5f, -0.5f, 0.0f,  // 右下角
    -0.5f, 0.5f, 0.0f,  // 左上角
    // 第二个三角形
    0.5f, -0.5f, 0.0f,  // 右下角
    -0.5f, -0.5f, 0.0f, // 左下角
    -0.5f, 0.5f, 0.0f   // 左上角
};
再修改需要渲染的顶点数，即可渲染出一个矩形。
但是明明4个点就可以渲染出的矩形，这里却需要6个顶点，非常浪费资源，不过我们有元素缓冲对象（EBO）可以解决这个问题(通过索引)。
EBO的使用类似于VBO。
1. 改动原先的三个顶点为四个顶点，并建立索引
2. 创建EBO并做绑定
3. 在渲染时用glDrawElements来替换glDrawArrays函数，表示我们要从索引缓冲区渲染三角形

![Alt text](./images/vertex_array_objects_ebo.png)

再次理解一下VBO、VAO、EBO：
VBO：用于存储顶点数据，如位置、颜色、法线等。
VAO：保存VBO、EBO的绑定状态以及顶点属性的配置，简化顶点数据的管理。
EBO：用于存储索引数据，通过索引重用顶点数据，减少数据冗余，优化渲染性能。
