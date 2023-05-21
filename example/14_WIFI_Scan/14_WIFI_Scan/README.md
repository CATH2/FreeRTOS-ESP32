常用命令
esptool.py -h
esptool.py read_flash -h
idf.py set-target esp32c3
idf.py set-target esp32s2
idf.py set-target esp32

esptool.py --port COM9 read_flash_status
esptool.py --port COM9 flash_id
esptool.py --port COM9 chip_id
esptool.py --port COM9 image_info
esptool.py --port COM9 read_mac
esptool.py --port COM9 erase_flash
esptool.py --port COM9 verify_flash

idf.py -p COM9 flash monitor
idf.py -p COM7 flash monitor
idf.py -p COM5 flash monitor
idf.py -p COM6 flash monitor
idf.py -p COM4 flash monitor
idf.py -p COM11 flash monitor
idf.py -p COM10 flash monitor
idf.py -p COM8 flash monitor
idf.py -p /dev/ttyUSB0 flash monitor
idf.py menuconfig
idf.py fullclean