all:
	gcc -o cephfs_connect.out cephfs_connect.c -lrados
	gcc -o cephfs_open.out cephfs_open.c

clean:
	rm cephfs*.out