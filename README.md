本项目以 STM32F103C8T6 为主控，驱动 0.96 寸 OLED 屏与 MPU6050 姿态传感器，实现了一块可以真正戴在手腕上的智能手表。从面包板原型验证，到自绘 PCB 打板焊接，再到佩戴成品，完整走通了嵌入式小项目的开发流程。
代码基于江科大（江协科技）STM32 教程移植修改而成，在此基础上扩展了手表 UI、菜单交互、游戏等应用层功能。
功能特性
-  首页时钟 UI —— 日期、时间、电量百分比一屏显示
-  日期时间控制 —— 按键设置时间与日期
-  滑动菜单页面 —— 手势滑动切换功能页面
-  秒表 —— 计时/暂停/清零
-  手电筒 —— 屏幕全亮应急照明
-  水平仪 —— 基于 MPU6050 姿态解算
-  谷歌小恐龙游戏 —— 断网小恐龙移植版
-  动态表情包 —— OLED 动画表情
-  ADC 电池电压检测 & 长按关机 —— 实时电量显示，长按电源键开关机


-  硬件部分：
-  1.主控：	STM32F103C8T6
2.显示：	0.96" OLED（SSD1306）
3.姿态传感器：	MPU6050
4.电源：	3.7V 锂电池
5.下载调试：	ST-LINK V2
6.结构：	面包板原型 → 自绘 PCB


实物展示：
面包板原型验证：
<img width="4284" height="5712" alt="957a79fcb29eddec083d51dd05c9160" src="https://github.com/user-attachments/assets/e1e234c7-49fa-4cc3-979c-8d51a307514a" />
STM32F103C8T6 最小系统板 + OLED + MPU6050，通过 ST-LINK V2 下载程序，先在面包板上验证全部功能。

成品佩戴效果：
<img width="554" height="1059" alt="360d62b69f3287ccafcce25d8788761" src="https://github.com/user-attachments/assets/0159f670-de28-4f9a-b374-4f3d1b717c43" />
电池续航显示 68%，日期时间实时刷新，按键呼出菜单与设置页面。

自绘 PCB（3D 预览）：
<img width="1117" height="1077" alt="14520418c4e1d9b8ca839dfcd577504" src="https://github.com/user-attachments/assets/b6022ffa-4421-4ed5-8f5f-ca1c4de41cfc" />
立创 EDA 绘制并打板：集成主控、OLED/MPU6050 排针接口、按键、LED 指示、锂电池充电管理与 USB 供电。

