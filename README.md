这是一个Qt学习的git仓库，我将在其中记录我的Qt学习以及一些Qt项目。
配套文件中，“.pro文件解释.txt”是我入门时对.pro文件的学习记录；“qt学习.txt”是我目前Qt学习的知识点总结。

其余文件基本上都是我创建的项目文件，其中包含各知识点的应用。

后面将会有一个我已完成的律师事务系统项目，其中包含了qt和数据库之间的协同运行。

还有目前我正在学习的opencv在Qt上的使用，在尝试做一个基本的人脸识别。

同时请注意，关于opencv在QT上的运用，一定要注意编译器的选择，opencv现有两种形式：

第一种，即现在opencv官网上的，基本上都是基于Visual Stdio的 MSVC 版本；

这种情况下，你需要去下载vs的生成工具  https://visualstudio.microsoft.com/zh-hans/downloads/

以及下载windows SDK编译器 https://developer.microsoft.com/zh-cn/windows/downloads/sdk-archive/ 

而后进入QT Creator中的构建套件，去配置MSVC构建套件

最后在右键项目，找到添加库，选择外部库，再去链接opencv库文件，即opencv_world***.lid( *** 代表你所下载的版本号)

第二种，即使用MinGw编译器，此编译器是大多数只学习使用Qt所用的，当然此情况需要你在opencv官网或者GitHub-opencv仓库中下载MinGw版的opencv。

This is a git repository for learning Qt, where I will record my Qt learning and some Qt projects.

In the accompanying files, “.pro文件解释.txt” is my learning notes on the .pro file when I first started; “qt学习.txt” is a summary of my current knowledge points in Qt.

The rest of the files are mostly project files created by me, which include the application of various knowledge points.

Later, there will be a completed project of a legal affairs system, which involves the collaborative operation between Qt and the database. Also, I am currently learning to use OpenCV in Qt, trying to create a basic face recognition application.

In these days, I have completed three different levels face_ai work: basic face detection, face recognition matching, generating face recognition library through face recognition learing.

But now I also find that this face ai work has a lot of problems. For example, I have used different face model: .onnx and .xml, so, the face recognition library can be used in .onnx model as the work generates .xml flie.
