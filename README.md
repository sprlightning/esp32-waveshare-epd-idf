# esp32-waveshare-epd-idf
Use waveshare's ePaper drivers on ESP-IDF.

## 基本信息

此例程及其驱动是[微雪电子](https://www.waveshare.net/)基于Arduino进行开发的，例程均在E-Paper ESP32 Driver Board上进行了验证；最新版驱动可访问微雪电子官网[例程](https://www.waveshare.net/wiki/E-Paper_ESP32_Driver_Board#.E7.A8.8B.E5.BA.8F)；

我对2025-11月版本的驱动进行了改动，使其适配ESP-IDF 5.1.4环境，并增加了对GoodDisplay ESP-L的支持；

其中[GoodDisplay](https://www.good-display.cn/)墨水屏驱动的开源地址是：[ESP32epdx](https://github.com/gooddisplayshare/ESP32epdx)，经过对比，二者大部分相似，但GD的内容比微雪的较新；从bug上来说，微雪的驱动相对稳定。

我使用的是GoodDisplay的2.13寸触摸屏（GDEY0213B74）和其配套的ESP32-L开发板，在使用GD的[GDEY0213B74驱动](https://github.com/gooddisplayshare/ESP32epdx/tree/main/examples/2-Colors%20(BW)/2.13/GDEY0213B74)时遇到的bug包括无法**正常显示中文字符串**，如“你好abc”会显示成“你  abc”，我仔细对比了两边的驱动，发现微雪的中文字符串驱动比GD多对比了一组数据，我尝试过将微雪驱动放到GD的驱动，可惜没有改善bug，所以原因可能涉及到更底层的寄存器操作，我没有深入研究；此外GD驱动的bug还有各种初始化函数调度混乱，存在过多不必要的休眠，还有就是刷新显示的时候黑白屏闪烁的情况相对严重，起初以为是过多的RST导致，但后来排除了这个原因，还是涉及到底层寄存器的操作，没深入研究。

微雪显示时钟是调用函数**Paint_DrawTime**，其本质是将输入的时间转化为字符串存入一个简单的时钟数据结构，然后将时钟数据结构的内容通过英文字符串显示函数在局部进行更新，时钟更新期间字符及屏幕会闪烁；而GD驱动的一个好处是其有一个新增的时钟函数**EPD_Dis_Part_Time**，其通过调用新增的**EPD_Dis_Part_RAM**在局部刷新写入数据，可以非常流畅且连贯的更新时钟显示，时钟显示期间屏幕稳定且不闪烁，但是我仔细分析这个函数，发现它其实是预定义了一组数字的Image data，传入的数字会转化为Image data，本质上是在局部更新Image。而这个Image data存在于GD的Image.h头文件，本来想将GD的时钟函数移植到微雪的，但发现其显示的尺寸有点奇怪，试图显示较小的时钟尺寸时会发生字符错位，这种尺寸不通用的情况使我放弃了这个移植计划。当然这也表明了一件事，图片data的刷新可以做到屏幕不闪烁。但是这不是根本问题，我注意到其他墨水屏在更新字符的时候同样可以做到屏幕不闪烁，这多少有些奇怪了。

此外之前试图使用GxEPD2，这是个集成了GoodDisplay和微雪电子的ePaper驱动库，可是其高度依赖一个Arduino图形库GFX，那个GFX图形库移植较复杂，而我因为某些原因只能使用纯ESP-IDF环境开发，这使我放弃了这个GxEPD2驱动库。

## 基本使用

### Step 1

将整个esp32-waveshare-epd-idf文件夹复制到你的IDF项目的components文件夹中，在项目main目录的CMakeLists.txt中添加对esp32-waveshare-epd-idf的依赖；值得注意的是，本项目的前置需求是[pschatzmann](https://github.com/pschatzmann)的[arduino-audio-tools](https://github.com/pschatzmann/arduino-audio-tools)，其内置的NoArduino.h使用ESP-IDF方法重构了Arduino基本函数，使得非Arduino项目能在不引入Arduino组件的情况下使用Arduino的基本函数，例如常用的延时、IO操作、串口打印等；下面是main目录的CMakeLists.txt示例：

```cmake
    idf_component_register(SRCS "main.cpp"
                            INCLUDE_DIRS "."
                            REQUIRES esp32-waveshare-epd-idf)
```

### Step 2

参考下面的E-Paper 程序对应参考表，根据你的屏幕修改esp32-waveshare-epd-idf的CMakeLists.txt的**EPD_DEMO_PATH**，默认使用的是"examples/epd2in13_V4-demo"，其同样适用于GoodDisplay的2.13寸触摸屏（GDEY0213B74）；

### Step 3

参考demo，在你的main.cpp中引入对应屏幕的demo头文件，其setup的内容放到main.cpp的app_main()中即可，如果loop()有内容，那就创建一个task()，把loop()的内容放到task里并在app_main()末尾调用启动任务调度器启动此task()即可。

### 注意事项

在src/NverUseArduino.h中有两个开关，**NoArduino**和**USE_GxEPD**；

开关**NoArduino**控制**是否不使用Arduino**，默认置1是不使用Arduino，其会调用arduino-audio-tools的NoArduino.h，否则会调用Arduino.h；

开关**USE_GxEPD**控制**是否使用GoodDisplay的ESP32-L开发板**，其默认置1是启用GoodDisplay ESP32-L，如使用微雪的[电子墨水屏无线网络驱动板](https://www.waveshare.net/shop/e-Paper-ESP32-Driver-Board.htm)，需要将开关**USE_GxEPD**置0，否则会影响SPI和墨水屏控制IO的定义。同样的，如果你没有使用上述开发板，使用的是自定义板子，那请修改为实际IO值；




## E-Paper 程序对应参考表

| 屏幕型号 | 程序 | 显示颜色 | 灰度 | 分辨率 | 显示尺寸(mm) | 刷新时间(s) | 局部刷新 | 控制接口 |
|------|------|---------|------|--------|-------------|------------|----------|----------|
| 1.54inch e-Paper | epd1in54_V2-demo | 黑、白 | 2 | 200 × 200 | 27.00 × 27.00 | 2 | √ | SPI |
| 1.54inch e-Paper (B) | epd1in54b_V2-demo | 红、黑、白 | 2 | 200 × 200 | 27.60 × 27.60 | 8 | × | SPI |
| 2.13inch e-Paper | epd2in13_V4-demo | 黑、白 | 2 | 250 × 122 | 48.55 × 23.70 | 2 | √ | SPI |
| 2.13inch e-Paper (B) | epd2in13b_V4-demo | 红、黑、白 | 2 | 212 × 104 | 48.55 × 23.70 | 15 | × | SPI |
| 2.13inch e-Paper (D) | epd2in13d-demo | ? | ? | ? | ? | ? | ? | SPI |
| 2.66inch e-Paper | epd2in66-demo | 黑、白 | 2 | 296 × 152 | 30.704 × 60.088 | 6 | √ | SPI |
| 2.66inch e-Paper (B) | epd2in66b-demo | 红、黑、白 | 2 | 296 × 152 | 30.704 × 60.088 | 15 | × | SPI |
| 2.7inch e-Paper | epd2in7_V2-demo | 黑、白 | 4 | 264 × 176 | 57.288 × 38.192 | 6 | × | SPI |
| 2.7inch e-Paper (B) | epd2in7b_V2-demo | 红、黑、白 | 2 | 264 × 176 | 57.288 × 38.192 | 15 | × | SPI |
| 2.9inch e-Paper | epd2in9_V2-demo | 黑、白 | 2 | 296 × 128 | 66.89 × 29.05 | 2 | √ | SPI |
| 2.9inch e-Paper (B) | epd2in9b_V3-demo | 红、黑、白 | 2 | 296 × 128 | 66.90 × 29.06 | 15 | √ | SPI |
| 3.7inch e-Paper | epd3in7-demo | 黑、白 | 4 | 480 × 280 | 47.32 × 81.12 | 3 | √ | SPI |
| 4.01inch e-Paper (F) | epd4in01f-demo | ? | ? | ? | ? | ? | ? | SPI |
| 4.2inch e-Paper | epd4in2-demo | 黑、白 | 4 | 400 × 300 | 84.80 × 63.60 | 4 | √ | SPI |
| 4.2inch e-Paper (B) | epd4in2b_V2-demo | 红、黑、白 | 2 | 400 × 300 | 84.80 × 63.60 | 15 | × | SPI |
| 4.26inch e-Paper | epd4in26-demo | 黑、白 | 4 | 800 × 480 | 92.80 × 55.68 | 4 | √ | SPI |
| 5.65inch e-Paper (F) | epd5in65f-demo | ? | ? | ? | ? | ? | ? | SPI |
| 5.83inch e-Paper | epd5in83_V2-demo | 黑、白 | 2 | 648 × 480 | 119.232 × 88.320 | 5 | × | SPI |
| 5.83inch e-Paper (B) | epd5in83b_V2-demo | ? | ? | ? | ? | ? | ? | SPI |
| 7.5inch e-Paper | epd7in5_V2-demo | 黑、白 | 2 | 800 × 480 | 163.20 × 97.92 | 5 | √ | SPI |
| 7.5inch e-Paper (B) | epd7in5b_V2-demo | 红、黑、白 | 2 | 800 × 480 | 163.20 × 97.92 | 16 | × | SPI |
| 13.3inch e-Paper HAT (B) | epd13in13b-demo | ? | ? | ? | ? | ? | ? | SPI |
| 13.3inch e-Paper HAT (K) | epd13in3k-demo | 黑、白 | 4 | 960×680 | 275.52 × 195.16 | 5 | √ | SPI |
