到这里就已经通过入门阶段，接下来进入光照阶段。 
# 颜色
现实中我们所看到的颜色是反射的颜色。  
当有光源时，比如白色的光源，照在一个蓝色的物体上时，我们所看到的是，两种颜色值相乘的结果，在计算机图形学中：  
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);  
glm::vec3 toyColor(1.0f, 0.5f, 0.31f);  
glm::vec3 result = lightColor * toyColor; // = (1.0f, 0.5f, 0.31f);  
是这样计算的。  
## 创建一个光照场景
因为我们还要创建一个表示灯（光源）的立方体，所以我们还要为这个灯创建一个专门的VAO。当然我们也可以让这个灯和其它物体使用同一个VAO，简单地对它的模型矩阵做一些变换就好了，然而接下来我们会频繁地对顶点数据和属性指针做出修改，我们并不想让这些修改影响到灯，因此我们有必要为灯创建一个新的VAO。  
主要做的改变是单独为光源（这里是一个立方体）设置了顶点着色器和片段着色器，这样区分开来比较方便和清晰。这也能帮助我们更进一步了解渲染的流程。  
# 基础光照
现实中光照很复杂，因此我们使用的是OpenGL中的简化模型，其中一种是风氏光照模型，风氏光照模型的主要结构由3个分量组成：环境(Ambient)、漫反射(Diffuse)和镜面(Specular)光照。  
![Alt text](./images/basic_lighting_phong.png)  
## 环境光照
光能够不停的在物体上进行反射，可以到达几乎所有地方，这叫做全局照明，但是过于复杂，我们目前先使用简化模型，即环境光照。  
把环境光照添加到场景里非常简单。我们用光的颜色乘以一个很小的常量环境因子，再乘以物体的颜色，然后将最终结果作为片段的颜色。
这里常量环境因子使用0.1：  
![Alt text](./images/ambient_lighting.png)   
## 漫反射光照
漫反射光照使物体上与光线方向越接近的片段能从光源处获得更多的亮度：  
![Alt text](./images/diffuse_light.png)   
为了测量光线和片段的角度，我们使用一个叫做法向量(Normal Vector)的东西，它是垂直于片段表面的一个向量（这里以黄色箭头表示）。  
法向量是一个垂直于顶点表面的（单位）向量。由于顶点本身并没有表面（它只是空间中一个独立的点），我们利用它周围的顶点来计算出这个顶点的表面。我们能够使用一个小技巧，使用叉乘对立方体所有的顶点计算法向量，但是由于3D立方体不是一个复杂的形状，所以我们可以简单地把法线数据手工添加到顶点数据中。由于我们向顶点数组添加了额外的数据（新属性），所以我们应该更新光照的顶点着色器。    
## 镜面光照
想象一下，一个光源照向镜子上，其中有光、镜子和眼睛的位置，就是镜面反射的概念，这里需要两个向量：光的反射方向、人眼的观察方向，其中光的反射方向可以通过光的入射方向和法线计算得出，人眼的观察方向可以直接通过位置算出，而射向眼睛的光可以通过这两个向量以及镜面强度和高光的反光度还有光的颜色求出。  
  
最后光照这一块总结一下：首先设置一个场景，有一个光源和一个物体，对于光源，使用和物体一样的顶点数据，即VBO一样，不过属性不同，即VAO不同，眼睛观察到的物体是通过物体本身颜色以及光照颜色所复合而成，而光照又包括三种，即环境光照、漫反射光照、镜面光照，环境光照只需要一个常量环境因子，漫反射光照由光照反方向和物体法线向量点乘得到diff，镜面光照则比较通俗易懂，需要光的反射方向和观察方向求出。  
对于上述模型，注意向量在使用时需要标准化，光源的模型目前比较简单，使用的是一个点光源，物体模型也比较简单，因此法向量可直接得到（以后则需要另外求出）。  
在物体的着色器中，有顶点位置和法向量两个属性，需要从顶点着色器传输顶点的世界坐标和法向量给片段着色器使用，在片段着色器中则需要使用到这两个数据并另外通过uniform获得摄像机的位置坐标，求出三种光照信息，最终得到更加真实的显示效果。  
  
# 材质
在现实世界里，每个物体会对光产生不同的反应。比如，钢制物体看起来通常会比陶土花瓶更闪闪发光，一个木头箱子也不会与一个钢制箱子反射同样程度的光。有些物体反射光的时候不会有太多的散射(Scatter)，因而产生较小的高光点，而有些物体则会散射很多，产生一个有着更大半径的高光点。如果我们想要在OpenGL中模拟多种类型的物体，我们必须针对每种表面定义不同的材质(Material)属性。  
我们已经为三个光照分量定义一个材质颜色(Material Color)：环境光照(Ambient Lighting)、漫反射光照(Diffuse Lighting)和镜面光照(Specular Lighting)。通过为每个分量指定一个颜色，我们就能够对表面的颜色输出有细粒度的控制了。现在，我们再添加一个反光度(Shininess)分量，结合上述的三个颜色，我们就有了全部所需的材质属性了。  
ambient材质向量定义了在环境光照下这个表面反射的是什么颜色，通常与表面的颜色相同。diffuse材质向量定义了在漫反射光照下表面的颜色。漫反射颜色（和环境光照一样）也被设置为我们期望的物体颜色。specular材质向量设置的是表面上镜面高光的颜色（或者甚至可能反映一个特定表面的颜色）。最后，shininess影响镜面高光的散射/半径。  
在片段着色器中，我们创建一个结构体(Struct)来储存物体的材质属性。使用uniform来访问material。  
目前光源只有颜色和位置，而没有另外设置强度，因此我们也创建一个结构体存储光源的属性。对于不同的反射我们希望光的强度有所区分。  
