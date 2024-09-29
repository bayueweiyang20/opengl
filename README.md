# 坐标系统
局部空间、世界空间、视觉空间、裁剪空间、屏幕空间五个状态。  
![Alt text](./images/coordinate_systems.png)  
1. 局部坐标是对象相对于局部原点的坐标，也是物体起始的坐标。
2. 下一步是将局部坐标变换为世界空间坐标，世界空间坐标是处于一个更大的空间范围的。这些坐标相对于世界的全局原点，它们会和其它物体一起相对于世界的原点进行摆放。
3. 接下来我们将世界坐标变换为观察空间坐标，使得每个坐标都是从摄像机或者说观察者的角度进行观察的。
4. 坐标到达观察空间之后，我们需要将其投影到裁剪坐标。裁剪坐标会被处理至-1.0到1.0的范围内，并判断哪些顶点将会出现在屏幕上。
5. 最后，我们将裁剪坐标变换为屏幕坐标，我们将使用一个叫做视口变换(Viewport Transform)的过程。视口变换将位于-1.0到1.0范围的坐标变换到由glViewport函数所定义的坐标范围内。最后变换出来的坐标将会送到光栅器，将其转化为片段。
我们之所以将顶点变换到各个不同的空间的原因是有些操作在特定的坐标系统中才有意义且更方便。例如，当需要对物体进行修改的时候，在局部空间中来操作会更说得通；如果要对一个物体做出一个相对于其它物体位置的操作时，在世界坐标系中来做这个才更说得通，等等。如果我们愿意，我们也可以定义一个直接从局部空间变换到裁剪空间的变换矩阵，但那样会失去很多灵活性。  
局部空间：物体所占用的坐标边界所围成的空间。  
世界空间：局部空间通过使用模型矩阵变换而来。  
观察矩阵：相当于有了一个摄像机观察世界空间，用到观察矩阵。  
裁剪空间：顾名思义，裁掉范围之外的点，显示剩余的点。用到投影矩阵，有两种投影方式：正视投影、透视投影，后者更接近于眼睛所看到的世界（近大远小，比如火车轨道），有个梗（鸽子为什么这么大）就是因为这个原理。  
最后三个矩阵和原本的坐标运算时注意顺序。最后的顶点应该被赋值到顶点着色器中的gl_Position，OpenGL将会自动进行透视除法和裁剪。  
  
# 3D显示
1. 首先创建一个模型矩阵。这个模型矩阵包含了位移、缩放与旋转操作，它们会被应用到所有物体的顶点上，以变换它们到全局的世界空间。通过将顶点坐标乘以这个模型矩阵，我们将该顶点坐标变换到世界坐标。
2. 接下来我们需要创建一个观察矩阵。我们想要在场景里面稍微往后移动，以使得物体变成可见的。将摄像机向后移动，和将整个场景向前移动是一样的。（右手系）
3. 最后我们需要做的是定义一个投影矩阵。我们希望在场景中使用透视投影。
4. 既然我们已经创建了变换矩阵，我们应该将它们传入着色器。首先，让我们在顶点着色器中声明一个uniform变换矩阵然后将它乘以顶点坐标，然后我们将矩阵传入着色器。

# 更加3D
拓展我们的2D平面到3D需要更多的顶点，这里我们渲染一个正方体需要36个顶点。成功渲染出一个正方体，但是由于渲染时会有非常混乱的覆盖关系，导致显示起来怪怪的，这时候我们运用z缓冲（深度缓冲），它允许OpenGL决定何时覆盖一个像素而何时不覆盖。  
我们要告诉OpenGL我们想要启用深度测试；它默认是关闭的，通过glEnable函数来开启深度测试。因为我们使用了深度测试，我们也要在每次渲染迭代之前清除深度缓冲。  
  
# 摄像机
通过反方向移动物体模拟我们（摄像机）的移动。  
![Alt text](./images/camera_axes.png)  
1. 摄像机位置
2. 摄像机方向：原点指向摄像机位置的方向向量。normalize标准化
3. 摄像机右轴：摄像机方向和一个上向量叉乘（cross）后normalize。
4. 摄像机上轴：摄像机方向和摄像机右轴向量叉乘。
## LookAt矩阵
创建一个看着(Look at)给定目标的观察矩阵。  
GLM已经提供了计算支持。我们要做的只是定义一个摄像机位置，一个目标位置和一个表示世界空间中的上向量的向量（我们计算右向量使用的那个上向量）。接着GLM就会创建一个LookAt矩阵，我们可以把它当作我们的观察矩阵。  
view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),glm::vec3(0.0f, 0.0f, 0.0f),glm::vec3(0.0f, 1.0f, 0.0f));    
## 自由移动
在输入函数中增加W、A、S、D的按键判断，控制摄像头的位置。
## 移动速度
常量移动速度，实际情况下根据处理器的能力不同，有些人可能会比其他人每秒绘制更多帧，也就是以更高的频率调用processInput函数。结果就是，根据配置的不同，有些人可能移动很快，而有些人会移动很慢。当你发布你的程序的时候，你必须确保它在所有硬件上移动速度都一样。  
图形程序和游戏通常会跟踪一个时间差(Deltatime)变量，它储存了渲染上一帧所用的时间。我们把所有速度都去乘以deltaTime值。结果就是，如果我们的deltaTime很大，就意味着上一帧的渲染花费了更多时间，所以这一帧的速度需要变得更高来平衡渲染所花去的时间。使用这种方法时，无论你的电脑快还是慢，摄像机的速度都会相应平衡，这样每个用户的体验就都一样了。
## 视角移动
上面使用的是键盘控制移动，不是很方便，这里加入鼠标控制控制面朝方向。  
一共有3种欧拉角：俯仰角(Pitch)、偏航角(Yaw)和滚转角(Roll)。  
俯仰角是描述我们如何往上或往下看的角，偏航角表示我们往左和往右看的程度，滚转角代表我们如何翻滚摄像机，每个欧拉角都有一个值来表示，把三个角结合起来我们就能够计算3D空间中任何的旋转向量了。对于我们的摄像机系统来说，我们只关心俯仰角和偏航角，给定一个俯仰角和偏航角，我们可以把它们转换为一个代表新的方向向量的3D向量。  
direction.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));   
direction.y = sin(glm::radians(pitch));  
direction.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));  
## 鼠标输入
1. 首先要有一个捕捉光标，用于判断焦点是否在程序中，通过颜色区分。  
2. 俯仰角和偏航角是通过鼠标移动控制的，因此需要GLFW监听鼠标移动事件，这里采用回调函数，在glfw回调函数中加入鼠标移动参数。  
3. 初始化鼠标当前位置，计算帧偏移量并更新位置，设置灵敏度，xoffset *= sensitivity，全局变量pitch、yaw，即两个角度，对俯仰角pitch作范围限制，根据xoffset等更新，最后根据俯仰角、偏航角计算方向向量camerafront。  
4. 第一次获取焦点时会跳一下，因为鼠标位置距离初始化的位置很远，这里我们用bool变量判断是否鼠标第一次进入屏幕，然后更新鼠标位置。
## 缩放（zoom）
在获得投影矩阵中有一个fov（field of view）参数，定义了场景中可以看到的范围，这里使用鼠标滚轮的回调函数。  
  
在上述的回调函数还有捕捉鼠标，都用到了glfw中的一些函数，都是在渲染循环之前就以及设置好了的，注意理解。  
  
摄像头使用篇幅较长，比较乱，因此我们创建一个摄像机类。这里也比较复杂，要仔细看。