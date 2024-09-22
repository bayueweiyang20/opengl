# GLSL着色器语言学习
着色器的开头总是要声明版本，接着是输入和输出变量、uniform和main函数。每个着色器的入口点都是main函数，在这个函数中我们处理所有的输入变量，并将结果输出到输出变量中。  
#version version_number  
in type in_variable_name;  
in type in_variable_name;  

out type out_variable_name;  

uniform type uniform_name;  

void main()  
{  
  // 处理输入并进行一些图形操作  
  ...  
  // 输出处理过的结果到输出变量  
  out_variable_name = weird_stuff_we_processed;  
}  
顶点着色器每个输入变量也叫顶点属性(Vertex Attribute)。  
in、out实现输入输出。  
顶点着色器的输入特殊在，它从顶点数据中直接接收输入。为了定义顶点数据该如何管理，我们使用location这一元数据指定输入变量，这样我们才可以在CPU上配置顶点属性。如layout (location = 0)。顶点着色器需要为它的输入提供一个额外的layout标识，这样我们才能把它链接到顶点数据。  
片段着色器，它需要一个vec4颜色输出变量，因为片段着色器需要生成一个最终输出的颜色。如果你在片段着色器没有定义输出颜色，OpenGL会把你的物体渲染为黑色（或白色）。  
从一个着色器向另一个着色器发送数据，我们必须在发送方着色器中声明一个输出，在接收方着色器中声明一个类似的输入。当类型和名字都一样的时候，OpenGL就会把两个变量链接到一起，它们之间就能发送数据了（这是在链接程序对象时完成的）。  
