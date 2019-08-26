circuittool: src/circuittool.cpp
	git submodule update --init --recursive
	g++ -O3 -o circuittool src/circuittool.cpp  -march=native   -lssl -lcrypto -lemp-tool -lrelic -lpthread  -lssl -lcrypto -Idependencies/emp-ag2pc/
	@echo " try running cp -r -f input_output_example1 input_output ; ./circuittool 1  1234567 & ./circuittool 2 1234567"
