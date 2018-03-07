SRC=src
.PHONY: test1 test2 test3

build:
	g++ -Wall $(SRC)/bulletin.cpp -o bulletin

build_prime:
	clang -Wall -pedantic -O2 $(SRC)/bulletin_prime.c -o bulletin

clean:
	rm -f bulletin output*



# Test cases
# 3 clients where one leaves early
test1:	build
	(./bulletin -D -c test1/client1.txt -i test1/messages1.txt -o output1.txt) & \
	(./bulletin -D -c test1/client2.txt -i test1/messages2.txt -o output2.txt) & \
	(./bulletin -D -c test1/client3.txt -i test1/messages3.txt -o output3.txt) & \
	wait

# 4 clients where one leaves late
test2:	build
	(./bulletin -D -c test2/client1.txt -i test2/messages1.txt -o output1.txt) & \
	(./bulletin -D -c test2/client2.txt -i test2/messages2.txt -o output2.txt) & \
	(./bulletin -D -c test2/client3.txt -i test2/messages3.txt -o output3.txt) & \
	(./bulletin -D -c test2/client4.txt -i test2/messages4.txt -o output4.txt) & \
	wait

# 7 clients that all join and leave at the same time
test3:	build
	(./bulletin -D -c test3/clientA.txt -i test3/messagesA.txt -o outputA.txt) & \
	(./bulletin -D -c test3/clientB.txt -i test3/messagesB.txt -o outputB.txt) & \
	(./bulletin -D -c test3/clientC.txt -i test3/messagesC.txt -o outputC.txt) & \
	(./bulletin -D -c test3/clientD.txt -i test3/messagesD.txt -o outputD.txt) & \
	(./bulletin -D -c test3/clientE.txt -i test3/messagesE.txt -o outputE.txt) & \
	(./bulletin -D -c test3/clientF.txt -i test3/messagesF.txt -o outputF.txt) & \
	(./bulletin -D -c test3/clientG.txt -i test3/messagesG.txt -o outputG.txt) & \
	wait

# 4 clients where each comes and goes quicker, the ring should reform multiple
# times
test4:	build
	(./bulletin -D -c test4/client1.txt -i test4/messages1.txt -o output1.txt) & \
	(./bulletin -D -c test4/client2.txt -i test4/messages2.txt -o output2.txt) & \
	(./bulletin -D -c test4/client3.txt -i test4/messages3.txt -o output3.txt) & \
	(./bulletin -D -c test4/client4.txt -i test4/messages4.txt -o output4.txt) & \
	wait

# just a two node test with many messages
test5:	build
	(./bulletin -D -c test5/client1.txt -i test5/messages1.txt -o output1.txt) & \
	(./bulletin -D -c test5/client2.txt -i test5/messages2.txt -o output2.txt) & \
	wait
