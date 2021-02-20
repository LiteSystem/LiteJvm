# LiteJvm
一个jvm 虚拟机原型，用来学习java vm 原理， 支持 Java SE 8版本的classfile 规范, 


## 开发环境：
   g++   4.8.4 
   cmake 3.5 
   eclipse-cpp-2018-09-linux-gtk-x86_64 

## 编译工程：
  在工程目录下 执行命令 cmake . (注意有个. 号),   再执行命令 make  
  将在 bin目录产生 可执行文件 litejvm 
  


## 运行vm:     

 将test目录的测试文件 Fib.class 复制到bin目录下，在bin目录下用如下命令执行class 文件:  
./litejvm Fib.class   
