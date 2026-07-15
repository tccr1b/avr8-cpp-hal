
#include "HAL/spi.hpp"
#include <inttypes.h>

/*
        ***_CMD_*** : Prefix for double byte command. (First byte of double byte commands)
    ***_CMDBYTE_*** : Prefix for single byte command sent to display directly.
    ***_CMDMASK_*** : Prefix for single byte command with multiple options. Used with bitwise operators.
*/

/*Set Contrast Control
Double byte command to select 1 out of 256 contrast steps. Contrast increases as the value increases. 
(RESET = 7Fh )*/
#define SSD1306_CMD_SET_CONTRAST_CTRL               0x81
/*Entire Display ON
Cmd Byte: |1|0|1|0|0|1|0|X0|
A4h, X0=0b: Resume to RAM content display (RESET)
        Output follows RAM content
A5h, X0=1b: Entire display ON
        Output ignores RAM content*/
#define SSD1306_CMDBYTE_ENTIRE_DISPLAY_RAM_CONTENT  0xA4
#define SSD1306_CMDBYTE_ENTIRE_DISPLAY_ON           0xA5
/*Set Normal/Inverse Display
Cmd Byte A: |1|0|1|0|0|1|1|X0|
A6h, X[0]=0b: Normal display (RESET)
        0 in RAM: OFF in display panel
        1 in RAM: ON in display panel
A7h, X[0]=1b: Inverse display
        0 in RAM: ON in display panel
        1 in RAM: OFF in display panel*/
#define SSD1306_CMDBYTE_DISPLAY_NORMAL              0xA6
#define SSD1306_CMDBYTE_DISPLAY_INVERSE             0xA7
/*Set Display ON/OFF
Cmd Byte A: |1|0|1|0|1|1|1|X0|
AEh, X[0]=0b:Display OFF (sleep mode) (RESET)
AFh X[0]=1b:Display ON in normal mode*/
#define SSD1306_CMDBYTE_DISPLAY_OFF                 0xAE
#define SSD1306_CMDBYTE_DISPLAY_ON                  0xAF
/*Continuous Horizontal Scroll Setup
26h, X[0]=0, Right Horizontal Scroll
27h, X[0]=1, Left Horizontal Scroll
(Horizontal scroll by 1 column)
A : Set dummy byte 0x00
B : Define start page address (0x00 - 0x07)
C : Set time interval between each scroll step in terms of frame frequency (0x00 - 0x07)
D : Define end page address (0x00 - 0x07)
E : Set dummy byte 0x00
F : Set dummy byte 0xFF
*/
#define SSD1306_CMD_CONT_HSCROLL_RIGHT              0x26
#define SSD1306_CMD_CONT_HSCROLL_LEFT               0x27
/* Continuous Vertical and Horizontal Scroll Setup
29h, X1X0=01b : Vertical and Right Horizontal Scroll
2Ah, X1X0=10b : Vertical and Left Horizontal Scroll (Horizontal scroll by 1 column)
A : Set dummy byte 0x00
B : Define start page address (0x00 - 0x07)
C : Set time interval between each scroll step in terms of frame frequency (0x00 - 0x07)
D : Define end page address (0x00 - 0x07)
        The value of D[2:0] must be larger or equal to B[2:0]
E : Vertical scrolling offset
        e.g.:
        E[5:0]= 01h refer to offset =1 row
        E[5:0]= 3Fh refer to offset =63 rows
F : Set dummy byte 0xFF
Note: No continuous vertical scrolling is available.
*/
#define SSD1306_CMD_CONT_VnHSCROLL_VERT_RIGHT       0x29
#define SSD1306_CMD_CONT_VnHSCROLL_VERT_LEFT        0x2A
/* Deactivate scroll
Stop scrolling that is configured by command 26h/27h/29h/2Ah. 
Note: After sending 2Eh command to deactivate the scrolling action, the ram data needs to be rewritten.*/
#define SSD1306_CMDBYTE_DEACTIVATE_SCROLL           0x2E
/* Activate scroll
Start scrolling that is configured by the scrolling setup commands: 26h/27h/29h/2Ah with the following valid sequences:
Valid command sequence 1: 26h ;2Fh.
Valid command sequence 2: 27h ;2Fh.
Valid command sequence 3: 29h ;2Fh.
Valid command sequence 4: 2Ah ;2Fh.
    For example, if “26h; 2Ah; 2Fh.” commands are issued, the setting in the last scrolling setup command,
i.e. 2Ah in this case, will be executed. In other words, setting in the last scrolling setup command overwrites
the setting in the previous scrolling setup commands.*/
#define SSD1306_CMDBYTE_ACTIVATE_SCROLL             0x2F
/*Set Vertical Scroll Area
1- SEND CMD
2- SEND A[5:0]
3- SEND B[6:0]
A[5:0] :Set No. of rows in top fixed area. The No. of rows in top fixed area is referenced to the top of 
        the GDDRAM (i.e. row 0).[RESET = 0]
B[6:0] :Set No. of rows in scroll area. This is the number of rows to be used for vertical scrolling. 
        The scroll area starts in the first row below the top fixed area. [RESET = 64]

Note:
(1)  A[5:0]+B[6:0] <= MUX ratio
(2)  B[6:0] <= MUX ratio
(3a) Vertical scrolling offset (E[5:0] in 29h/2Ah) < B[6:0]
(3b) Set Display Start Line (X5X4X3X2X1X0 of 40h~7Fh) < B[6:0]
(4)  The last row of the scroll area shifts to the first row
of the scroll area.
(5) For 64d MUX display
A[5:0] = 0, B[6:0]=64 : whole area scrolls
A[5:0] = 0, B[6:0] < 64 : top area scrolls
A[5:0] + B[6:0] < 64 : central area scrolls
A[5:0] + B[6:0] = 64 : bottom area scrolls
*/
#define SSD1306_CMD_SET_VERT_SCROLL_AREA            0xA3
/*Set Lower Column Start Address for Page Addressing Mode
    Set the lower nibble of the column start address register for Page Addressing Mode using X[3:0]
as data bits. The initial display line register is reset to 0000b after RESET.
Note: This command is only for page addressing mode
*/
#define SSD1306_CMDMASK_SET_LOWER_COL_START_ADDR    0x00
/*Set Higher Column Start Address for Page Addressing Mode
    Set the higher nibble of the column start address register for Page Addressing Mode using X[3:0]
as data bits. The initial display line register is reset to 0000b after RESET.
Note: This command is only for page addressing mode
*/
#define SSD1306_CMDMASK_SET_HIGER_COL_START_ADDR    0x10
/*Set Memory Addressing Mode
A[1:0] = 00b, Horizontal Addressing Mode
A[1:0] = 01b, Vertical Addressing Mode
A[1:0] = 10b, Page Addressing Mode (RESET)
A[1:0] = 11b, Invalid
*/
#define SSD1306_CMD_SET_MEM_ADDR_MODE               0x20
/*Set Column Address
Setup column start and end address
A[6:0]: Column start address, range : 0-127d, (RESET=0d)
B[6:0]: Column end address, range   : 0-127d, (RESET =127d)
    Note: This command is only for horizontal or vertical
addressing mode.*/
#define SSD1306_CMD_SET_COL_ADDR                    0x21
/*Set Page Address
Setup page start and end address
A[2:0] : Page start Address, range: 0-7d, (RESET = 0d)
B[2:0] : Page end Address, range  : 0-7d, (RESET = 7d)
    Note: This command is only for horizontal or vertical
addressing mode.
*/
#define SSD1306_CMD_SET_PAGE_ADDR                   0x22
/*Set Page Start Address for Page Addressing Mode
Set GDDRAM Page Start Address (PAGE0~PAGE7) for Page Addressing Mode using X[2:0].
    Note: This command is only for page addressing mode
*/
#define SSD1306_CMDMASK_SET_PAGE_START_ADDR         0xB0
/*Set Display Start Line
Set display RAM display start line register from 0-63 using |0|1|X5|X3|X2|X1|X0|.
Display start line register is reset to 000000b during RESET.*/
#define SSD1306_CMDMASK_SET_DISPLAY_START_LINE      0x40
/*Set Segment Re-map
A0h, X[0]=0b: column address 0 is mapped to SEG0 (RESET)
A1h, X[0]=1b: column address 127 is mapped to SEG0*/
#define SSD1306_CMDMASK_SET_SEGMENT_REMAP           0xA0
/*Set Multiplex Ratio
Set MUX ratio to N+1 MUX
A-> |*|*|A5|A4|A3|A2|A1|A0|
N=  A[5:0] : from 16MUX to 64MUX, RESET=111111b (i.e. 63d, 64MUX)
    A[5:0] from 0 to 14 are invalid entry.*/
#define SSD1306_CMD_SET_MULTIPLEX_RATIO             0xA8
/*Set COM Output Scan Direction
Command Byte: |1|1|0|0|X3|0|0|0|
C0h, X[3]=0b: normal mode (RESET) Scan from COM0 to COM[N–1]
C8h, X[3]=1b: remapped mode. Scan from COM[N-1] to COM0
Where N is the Multiplex ratio.
*/
#define SSD1306_CMDMASK_SET_COM_OUTPUT_SCAN_DIR     0xC0
/*Set Display Offset
Set vertical shift by COM from 0d~63d The value is reset to 00h after RESET.
Command Byte A: |*|*A5|A4|A3|A2|A1|A0|
*/
#define SSD1306_CMD_SET_DISPLAY_OFFSET              0xD3
/*Set COM Pins Hardware Configuration
    A[4]=0b, Sequential COM pin configuration
    A[4]=1b(RESET), Alternative COM pin configuration
    A[5]=0b(RESET), Disable COM Left/Right remap
    A[5]=1b, Enable COM Left/Right remap
*/
#define SSD1306_CMD_SET_COM_PINS_HWARE_CONFIG       0xDA
/*Set Display Clock Divide Ratio/Oscillator Frequency
Command Byte A: |A7|A6|A5|A4|A3|A2|A1|A0|
A[3:0] :Define the divide ratio (D) of the display clocks 
        (DCLK): Divide ratio= A[3:0] + 1, 
        RESET is 0000b (divide ratio = 1)
A[7:4] :Set the Oscillator Frequency, FOSC. Oscillator Frequency increases with the value of A[7:4] and vice versa. 
        RESET is 1000b Range:0000b~1111b
Frequency increases as setting value increases.
*/
#define SSD1306_CMD_SET_DISPLAY_CLK_DIV_RATIO       0xD5
/*Set Display Clock Divide Ratio/Oscillator Frequency
Command Byte A: |A7|A6|A5|A4|A3|A2|A1|A0|
A[3:0] :Define the divide ratio (D) of the display clocks 
        (DCLK): Divide ratio= A[3:0] + 1, 
        RESET is 0000b (divide ratio = 1)
A[7:4] :Set the Oscillator Frequency, FOSC. Oscillator Frequency increases with the value of A[7:4] and vice versa. 
        RESET is 1000b Range:0000b~1111b
Frequency increases as setting value increases.
*/
#define SSD1306_CMDBYTE_SET_DISPLAY_OSC_FREQ        0xD5
/*Set Pre-charge Period
Command Byte A: |A7|A6|A5|A4|A3|A2|A1|A0|
A[3:0] :Phase 1 period of up to 15 DCLK clocks, 0 is invalid entry
        (RESET=2h)
A[7:4] :Phase 2 period of up to 15 DCLK clocks, 0 is invalid entry
        (RESET=2h)
*/
#define SSD1306_CMD_SET_PRECHARGE_PERIOD            0xD9
/*Set V_COMH Deselect Level
Command Byte A: |0|A6|A5|A4|0|0|0|0|
A[6:4]  Hex V_COMH deselect level
000b    00h 0.65*Vcc
010b    20h 0.77*Vcc (RESET)
011b    30h 0.83*Vcc
*/
#define SSD1306_CMD_V_COMH_DESELECT_LEVEL           0xDB
/*NOP - No Operation
Command for no operation*/
#define SSD1306_CMD_NOP                             0xE3
/*Charge Pump Setting
A[2] = 0b, Disable charge pump(RESET)
A[2] = 1b, Enable charge pump during display on
Note: The Charge Pump must be enabled by the following command:
    8Dh; Charge Pump Setting
    14h; Enable Charge Pump
    AFh; Display ON
*/
#define SSD1306_CMD_CHARGE_PUMP_SETTING             0x8D

/* Software Configuration
SSD1306 has internal command registers that are used to configure the operations of the driver IC.
After reset, the registers should be set with appropriate values in order to function well. The registers
can be accessed by MPU interface in either 6800, 8080, SPI type with D/C# pin pull low or using I2C
interface. Below is an example of initialization flow of SSD1306. The values of registers depend on
different condition and application.

Software Initialization Flow Chart
1)  Set MUX Ratio A8h, 3Fh
2)  Set Display Offset D3h, 00h
3)  Set Display Start Line 40h
4)  Set Segment re-map A0h/A1h
5)  Set COM Output Scan Direction C0h/C8h
6)  Set COM Pins hardware configuration DAh, 02
7)  Set Contrast Control 81h, 7Fh
8)  Disable Entire Display On A4h
9)  Set Normal Display A6h
10) Set Osc Frequency D5h, 80h
11) Enable charge pump regulator 8Dh, 14h
12) Display On AFh
*/

namespace ssd1306{} // namespace ssd1306

enum class ScrollStepInterval : uint8_t{
    _5Frames    = 0x00,
    _64Frames   = 0x01,
    _128Frames  = 0x02,
    _256Frames  = 0x03,
    _3Frames    = 0x04,
    _4Frames    = 0x05,
    _25Frames   = 0x06,
    _2Frames    = 0x07
};

enum class ScrollMode : uint8_t{
    Horizontal,
    HorizontalAndVertical,
};

enum class COMOutputScanDir : uint8_t{
    NormalMode  = 0x00,
    RemappedMode= 0x08,
};

enum class DisplayPage : uint8_t{
       Page0 = 0x00,
       Page1 = 0x01,
       Page2 = 0x02,
       Page3 = 0x03,
       Page4 = 0x04,
       Page5 = 0x05,
       Page6 = 0x06,
       Page7 = 0x07
};

enum class DisplayMode : uint8_t{
    Normal,
    Inverse,
};

enum class MemoryAddressingMode : uint8_t{
    Horizontal  = 0x00,
    Vertical    = 0x01,
    Page        = 0x02,
};

enum class x0 : uint8_t{

};

enum class x0 : uint8_t{

};


class SSD1306{
private:
    void sendCommand(uint8_t a);
    struct DisplayScroll{
        void activate();
        void deactivate();
        DisplayScroll& setVerticalArea();
    };
    struct Display{
        void on();
        void off();
        void setOffset();
        void setStartLine();
        void disableEntire();
        void enableEntire();
    };
    
    void setPrechargePeriod(uint8_t, uint8_t);
    void setMultiplexerRatio();
    void setDisplayOffset();
    void setDisplayStartLine();
    void setSegmentRemap();
    void setCOMOutputScanDir();
    void setCOMPinsHardwareConfig();
    void setOscilatorFrequency();
    void enableChargePump();
    void convertToXY();
    
public:
    enum class InterfaceType : uint8_t{
        SPI,
        I2C,
        Bitbanging,
    };
    typedef struct config_t{
        DisplayMode display_mode;
        bool scrolling_enable;
    };
    void displayOn();
    void displayOff();
    void drawRectangle();
    void drawLine();
    void setPixel();
    void drawCircle();
    SSD1306::DisplayScroll Scrolling;
    SSD1306::Display Display;

    void initialize(SSD1306::config_t* config){}

    SSD1306(SSD1306::InterfaceType iface);
    ~SSD1306();
};


SSD1306::SSD1306(SSD1306::InterfaceType iface){}
SSD1306::~SSD1306(){}


void foo(){
    SSD1306 oled(SSD1306::InterfaceType::SPI);
    SSD1306::config_t dispConfig;
        dispConfig.display_mode = DisplayMode::Normal;
        dispConfig.scrolling_enable = true;

    oled.initialize(&dispConfig);

    oled.Scrolling.activate();
    oled.Scrolling.setVerticalArea().activate();
    oled.Scrolling.deactivate();
    oled.displayOn();
    oled.Display.off();
    oled.Display.on();
    oled.Scrolling.setVerticalArea().activate();

}