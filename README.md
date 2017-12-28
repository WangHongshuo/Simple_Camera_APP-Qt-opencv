# VideoCameraApp-Qt

Based on Opencv.

简单的摄像头界面程序，开关摄像头，简单的图像处理。

Log:

2017.12.28:

Fixed bugs when initialization failed and now it can process images only have 1 or 3 channels, but it's very heavy. (            

修复了初始化失败可能导致的错误，现在支持处理单通道和3通道图像。但是速度依然很慢。       

2017.12.27:     

Finished basic function. Now it can detect all the camera device in the computer and select one camera to get image, but the face detection function performed badly, it will take 150ms to process each frame, I don't whether I can optimize it or not.  (            

重写完成，可以检测所有的摄像头并选用不同的摄像头，但人脸检测函数要花费150ms处理每一帧，不知道能不能优化得了。      

2017.12.26:     

Ready to rebuild this project and add some new function.       

准备重写和增加一些新功能。     

2017.09.21:     
 
Fix bugs and add a button, now the project can works with opencv3.3.           

修复了许多错误，添加了一个保存按钮，支持opencv3.3。               


2017.09.05:      

Fix a bug may lead to a crash.          

修复一个可能导致程序崩溃的错误              

