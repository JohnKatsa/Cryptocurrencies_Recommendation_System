SUBDIRS = ./Project3/ ./Project2/

all: ./Project2/cluster ./Project3/recommendation

./Project2/cluster:
	$(MAKE) -C ./Project2/

./Project3/recommendation:
	$(MAKE) -C ./Project3/

clean:
	$(MAKE) clean -C ./Project3/
	$(MAKE) clean -C ./Project2/
