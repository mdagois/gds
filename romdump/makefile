TARGET=romdump
ROM=rom.gb

$(TARGET): main.cpp
	@g++ -o $@ $<

run: $(TARGET)
	@./$(TARGET) $(ROM)

