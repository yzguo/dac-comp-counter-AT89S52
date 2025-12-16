# DAC + Stepper Controller

## Manual

1. baudrate = 9600, 波特率 9600
2. voltage level from 0 to 255, full is 4096 -- 2.048V ref
3. run stepper tick from 0 to 255
4. all num should be hexadecimal, 十六进制
5. all input ignore case, 忽略大小写

## Interfaces

UART input like `I\n` or `i\n`, output with format as follows:

```plain
UART Initialization!

```

UART input like `R\n` or `r\n`, output count number from voltage level 0 to 255 with format as follows:

```plain
#
0 num0
1 num1
...
255 num255

```

UART input like `S10\n` or `s10\n`, output count number with voltage level 16 with format as follows:

```plain
#
16 num16

```

UART input like `PX10\n` or `py10\n`, the stepper will run 16 ticks with `X/x` and `Y/y` for different directions.
