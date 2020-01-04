CFLAGS=-O3

cfireworks: cfireworks.c
	$(CC) $(CFLAGS) cfireworks.c -o cfireworks

clean:
	rm cfireworks
