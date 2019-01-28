OBJS = user.o tweet.o cluster.o recommendation.o methods.o ../Project2/obj/*
SOURCE = user.cpp tweet.cpp cluster.cpp methods.cpp recommendation.cpp
OUT = recommendation
CC = g++ -std=c++11
recommendation : $(OBJS)
	$(CC) -g -lm $(OBJS) -o $(OUT)
methods.o : methods.cpp
	$(CC) -c methods.cpp -g
user.o: user.cpp
	$(CC) -c user.cpp -g
tweet.o : tweet.cpp
	$(CC) -c tweet.cpp -g
cluster.o : cluster.cpp
	$(CC) -c cluster.cpp -g
recommendation.o: recommendation.cpp
	$(CC) -c recommendation.cpp -g
clean :
	@rm user.o tweet.o cluster.o recommendation.o methods.o $(OUT)
	@rm uj_input cj_input cj_input2 outcj outcj2
