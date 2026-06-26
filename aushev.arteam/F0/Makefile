CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -std=c++11

TARGET = dota_analyzer
SRCS = main.cpp hero.cpp file_handler.cpp draft_analyzer.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
