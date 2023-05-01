CFLAGS = -std=c++17 -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

vulkanIntroTuto: main.cpp
	g++ $(CFLAGS) -o vulkanIntroTuto main.cpp $(LDFLAGS)

.PHONY: test clean

test: vulkanIntroTuto
	./vulkanIntroTuto

clean:
	rm -f vulkanIntroTuto