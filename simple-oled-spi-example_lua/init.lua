
-- setup SPI and connect display
function init_spi_display()
     -- Hardware SPI CLK  = GPIO14-->SCL OLED
     -- Hardware SPI MOSI = GPIO13-->SDA OLED
     -- Hardware SPI MISO = GPIO12 (not used)
     -- CS, D/C, and RES can be assigned freely to available GPIOs
     cs  = 8 -- GPIO15, pull-down 10k to GND
     dc  = 4 -- GPIO2 --> D/C OLED
     res = 0 -- GPIO16 --> RST OLED

     spi.setup(1, spi.MASTER, spi.CPOL_LOW, spi.CPHA_LOW, spi.DATABITS_8, 0)
     disp = u8g.ssd1306_128x64_spi(cs, dc, res)
end

function xbm_picture()
     disp:setFont(u8g.font_6x10)
     disp:drawStr( 62, 10, "The Inventor's")
     disp:drawStr( 62, 62, "House")
     disp:drawXBM( 0, -5, 60, 60, xbm_data )
end
--Converter Image to *.Mono with
--http://www.online-utility.org/image_converter.jsp?outputType=MONO
function bitmap_test(delay)
     file.open("prueba.MONO", "r")
     xbm_data = file.read()
     file.close()

      disp:firstPage()
      repeat
           xbm_picture()
      until disp:nextPage() == false

      tmr.wdclr()
end

init_spi_display()
bitmap_test()
