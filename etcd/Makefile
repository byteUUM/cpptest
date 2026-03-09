all:put get
put:put.cc
	g++ -o $@ $^ -letcd-cpp-api -lcpprest
get:get.cc
	g++ -o $@ $^ -letcd-cpp-api -lcpprest
.PHONY:clean
clean:
	rm -rf put get

