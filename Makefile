cc=gcc
target = test 
#查找所有的.c 文件
source = $(shell find ./ -name "*.c")
deps = $(shell find ./ -name "*.h")

$(target): $(source)
	$(cc) -o $(target) $(source) -ljson-c 

clean:
	rm -rf $(target) *~ 
