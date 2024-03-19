## jlink刷固件及解决报错克隆问题

* 1.首先将刷入的jlink与被刷入的jlink连接

如图![jlink](https://github.com/changfengpro/stm32/blob/master/jlink刷固件教程（解决报错盗版克隆）/图片/jlink(1).jpg?raw=true)

JLINK 被刷的jlink  
3.3  ->    3.3    
GND  ->    GND   
SWCLK  ->   C   
SWDIO  ->   D  

* 2.然后打开SEGGER - J-Link V7.22b文件夹中的J-Flash Lite V7.22b   

![jlink](https://github.com/changfengpro/stm32/blob/master/jlink刷固件教程（解决报错盗版克隆）/图片/jlink(5).png?raw=true)  

device是你的jlink中间的芯片的型号，我的是stm32f103。其他的与图片上一致即可，按OK进入烧写。

* 3.第一步点击红色箭头处的三个点，找到jlink刷固件教程（解决报错盗版克隆）\jlink固件\OB-STM32_V754_Addr-0x8000000_Change date to  to 30-02-02(Licensed-CBT6-128K_USE).hex  的.hex文件并选择。（另一个文件.hex的文件也可以试试）   
     第二步点击黄色箭头的erase Chip（擦除芯片的flash）     
     第三步点击黑色箭头的program device（烧写固件）     
![jlink](https://github.com/changfengpro/stm32/blob/master/jlink刷固件教程（解决报错盗版克隆）/图片/jlink(6).png?raw=true)

* 4.烧写完成后打开jlink刷固件教程（解决报错盗版克隆）\SEGGER - J-Link V7.22b\J-Link Commander V7.22b
  ![jlink](https://raw.githubusercontent.com/changfengpro/stm32/master/jlink%E5%88%B7%E5%9B%BA%E4%BB%B6%E6%95%99%E7%A8%8B%EF%BC%88%E8%A7%A3%E5%86%B3%E6%8A%A5%E9%94%99%E7%9B%97%E7%89%88%E5%85%8B%E9%9A%86%EF%BC%89/%E5%9B%BE%E7%89%87/jlink%EF%BC%883%EF%BC%89.png)       
  首次打开S/N可能为-1，这时就需要修改S/N。     
  打开jlink刷固件教程（解决报错盗版克隆）\JLink_V612j\JLink.exe
  输入图中命令即可修改，要注意的是S/N必须是八位数。       ![jlink](https://raw.githubusercontent.com/changfengpro/stm32/master/jlink%E5%88%B7%E5%9B%BA%E4%BB%B6%E6%95%99%E7%A8%8B%EF%BC%88%E8%A7%A3%E5%86%B3%E6%8A%A5%E9%94%99%E7%9B%97%E7%89%88%E5%85%8B%E9%9A%86%EF%BC%89/%E5%9B%BE%E7%89%87/jlink(4).png)

* 5.最后就是输入license     
* 打开jlink刷固件教程（解决报错盗版克隆）\SEGGER - J-Link V7.22b\J-Flash V7.22b和jlink刷固件教程（解决报错盗版克隆）\jlink固件\Jlink_ob注册神器_2019.exe      
* 在注册器中输入的是修改后的S/N，然后依次输入四条license即可。        
  ![jlink](https://github.com/changfengpro/stm32/blob/master/jlink刷固件教程（解决报错盗版克隆）/图片/jlink(2).jpg?raw=true)
