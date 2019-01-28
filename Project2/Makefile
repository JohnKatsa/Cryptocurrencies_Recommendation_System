all: ./obj/cluster.o ./obj/configure.o ./obj/data.o ./obj/random_init.o ./obj/kmeanspp_init.o ./obj/lsh_item.o ./obj/lsh_bucket.o ./obj/lsh_hashtable.o ./obj/lsh.o ./obj/cube.o ./obj/lloyd_assign.o ./obj/nearest_neighbour.o ./obj/kmeans_update.o ./obj/pam_lloyd_update.o ./obj/silhouette.o

#cluster : ./obj/cluster.o ./obj/configure.o ./obj/data.o ./obj/random_init.o ./obj/kmeanspp_init.o ./obj/lsh_item.o ./obj/lsh_bucket.o ./obj/lsh_hashtable.o ./obj/lsh.o ./obj/cube.o ./obj/lloyd_assign.o ./obj/nearest_neighbour.o ./obj/kmeans_update.o ./obj/pam_lloyd_update.o ./obj/silhouette.o
#	@g++ -std=c++11 -o cluster ./obj/cluster.o ./obj/configure.o ./obj/data.o ./obj/random_init.o ./obj/kmeanspp_init.o ./obj/lsh_item.o ./obj/lsh_bucket.o ./obj/lsh_hashtable.o ./obj/lsh.o ./obj/cube.o ./obj/lloyd_assign.o ./obj/nearest_neighbour.o ./obj/kmeans_update.o ./obj/pam_lloyd_update.o ./obj/silhouette.o -lm -g
#	@echo "Clustering ready for use!"

./obj/cluster.o : ./clustering/cluster.cpp ./clustering/cluster.h
	@g++ -std=c++11 -c ./clustering/cluster.cpp -o ./obj/cluster.o
	@echo "Configuring . . ."

./obj/configure.o : ./configuration/configure.cpp ./configuration/configure.h
	@g++ -std=c++11 -c ./configuration/configure.cpp -o ./obj/configure.o

./obj/data.o : ./configuration/data.cpp ./configuration/data.h
	@g++ -std=c++11 -c ./configuration/data.cpp -o ./obj/data.o
	@echo "Installing Cluster Initialization Modules . . ."

./obj/random_init.o : ./init/Random_Selection_of_k_points/random_init.cpp ./init/Random_Selection_of_k_points/random_init.h
	@g++ -std=c++11 -c ./init/Random_Selection_of_k_points/random_init.cpp -o ./obj/random_init.o

./obj/kmeanspp_init.o : ./init/kmeanspp/kmeanspp_init.cpp ./init/kmeanspp/kmeanspp_init.h
	@g++ -std=c++11 -c ./init/kmeanspp/kmeanspp_init.cpp -o ./obj/kmeanspp_init.o
	@echo "Installing Cluster Assignment Modules . . ."

./obj/lsh_item.o : ./assign/cube/lsh/lsh_item.cpp ./assign/cube/lsh/lsh_item.h
	@g++ -std=c++11 -c ./assign/cube/lsh/lsh_item.cpp -o ./obj/lsh_item.o

./obj/lsh_bucket.o : ./assign/cube/lsh/lsh_bucket.cpp ./assign/cube/lsh/lsh_bucket.h
	@g++ -std=c++11 -c ./assign/cube/lsh/lsh_bucket.cpp -o ./obj/lsh_bucket.o

./obj/lsh_hashtable.o : ./assign/cube/lsh/lsh_hashtable.cpp ./assign/cube/lsh/lsh_hashtable.h
	@g++ -std=c++11 -c ./assign/cube/lsh/lsh_hashtable.cpp -o ./obj/lsh_hashtable.o

./obj/lsh.o : ./assign/cube/lsh/lsh.cpp ./assign/cube/lsh/lsh.h
	@g++ -std=c++11 -c ./assign/cube/lsh/lsh.cpp -o ./obj/lsh.o

./obj/cube.o : ./assign/cube/cube.cpp ./assign/cube/cube.h
	@g++ -std=c++11 -c ./assign/cube/cube.cpp -o ./obj/cube.o

./obj/lloyd_assign.o : ./assign/lloyd_assign/lloyd_assign.cpp ./assign/lloyd_assign/lloyd_assign.h
	@g++ -std=c++11 -c ./assign/lloyd_assign/lloyd_assign.cpp -o ./obj/lloyd_assign.o

./obj/nearest_neighbour.o : ./assign/cube/nearest_neighbour/nearest_neighbour.cpp ./assign/cube/nearest_neighbour/nearest_neighbour.h
	@g++ -std=c++11 -c ./assign/cube/nearest_neighbour/nearest_neighbour.cpp -o ./obj/nearest_neighbour.o
	@echo "Installing Cluster Update Modules . . ."

./obj/kmeans_update.o : ./update/kmeans_update/kmeans_update.cpp ./update/kmeans_update/kmeans_update.h
	@g++ -std=c++11 -c ./update/kmeans_update/kmeans_update.cpp -o ./obj/kmeans_update.o

./obj/pam_lloyd_update.o : ./update/PAM_a_la_Lloyd/pam_lloyd_update.cpp ./update/PAM_a_la_Lloyd/pam_lloyd_update.h
	@g++ -std=c++11 -c ./update/PAM_a_la_Lloyd/pam_lloyd_update.cpp -o ./obj/pam_lloyd_update.o
	@echo "Installing Silhouette Modules. . ."

./obj/silhouette.o : ./clustering/silhouette.cpp ./clustering/silhouette.h
	@g++ -std=c++11 -c ./clustering/silhouette.cpp -o ./obj/silhouette.o
	@echo "Finalizing Clustering. . ."

clean :
	@rm -rf ./obj/*
