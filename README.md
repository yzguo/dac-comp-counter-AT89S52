# DAC + Stepper Controller

## Manual

1. baudrate = 9600, 波特率 9600
2. voltage level from 0 to 255, full is 4096 -- 2.048V ref
3. run stepper tick from 0 to 255
4. all num should be hexadecimal, 十六进制
5. all input ignore case, 忽略大小写
6. stepper PUL_ P3.2, DIR_ = P3.3, ENA_ = P3.4
7. ZERO = P3.5, 复位信号, 光电门被挡住时为 0
8. X 上移(红), Y 下移(蓝), 零位在下方(蓝)
9. 波长(nm)和步进电机(tick)的关系: $y=40.018x-232.37$

## Interfaces

UART input like `V\n` or `v\n`, output with format as follows:

```plain
Dec 16 2025 16:28:43

```

UART input like `I\n` or `i\n`, the stepper will go down to the end to initialize, and output with format as follows:

```plain
OK

```

UART input like `R\n` or `r\n`, output count number from voltage level 0 to 255 with format as follows:

```plain
0,num0;1,num1;...;255,num255;x;

```

UART input like `S10\n` or `s10\n`, output count number with voltage level 16 with format as follows:

```plain
16,num16

```

UART input like `P10X\n` or `p10y\n`, the stepper will run 16 ticks with `X/x` and `Y/y` for different directions, and output with format as follows:

```plain
OK

```
